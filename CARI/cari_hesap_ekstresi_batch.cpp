#include <QMessageBox>
#include <QDate>
#include "e9_enum.h"
#include "cari_hesap_ekstresi_batch_class.h"
#include "ui_cari_hesap_ekstresi_batch.h"
#include "print.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "cari_console_utils.h"
#include "e9_log.h"
#include "adres_console_utils.h"
#include "cari_hesap_ekstresi_raporu_open.h"
#include "cari_kart_arama_open.h"
#include "cari_struct.h"

extern ADAK_SQL * DB;



/**************************************************************************************
                   OPEN_CARI_HESAP_EKSTRESI_BATCH
***************************************************************************************/

void OPEN_CARI_HESAP_EKSTRESI_BATCH ( int p_program_id, int p_modul_id, int cari_hesap_id ,  QWidget * parent )
{
    CARI_HESAP_EKSTRESI_BATCH * B = new CARI_HESAP_EKSTRESI_BATCH ( p_program_id, p_modul_id, cari_hesap_id, parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   CARI_HESAP_EKSTRESI_BATCH::CARI_HESAP_EKSTRESI_BATCH
***************************************************************************************/

CARI_HESAP_EKSTRESI_BATCH::CARI_HESAP_EKSTRESI_BATCH ( int p_program_id, int p_modul_id, int cari_hesap_id , QWidget *parent) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::CARI_HESAP_EKSTRESI_BATCH)
{
    m_cari_hesap_id = cari_hesap_id;
    m_program_id    = p_program_id;
    m_modul_id      = p_modul_id;

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   CARI_HESAP_EKSTRESI_BATCH::SETUP_FORM
***************************************************************************************/

void CARI_HESAP_EKSTRESI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE     ( "cari-hesap-islemleri_cari-hesap-ekstresi" );
    SET_PAGE_TITLE    ( tr ( "CARİ - CARİ HESAP EKSTRESİ" ) ) ;

    REGISTER_BUTTON_WIDGET          ( m_ui->widget_batch_buttons );

    m_ui->adakDate_bsl_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bts_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE() );

    m_ui->checkbox_tek_hesap->setChecked( true );
    m_ui->frame_tek_hesap->setEnabled( true );
    m_ui->checkbox_bakiyesiz_hesaplar->setChecked( true );
    m_ui->checkbox_bakiyesiz_hesaplar->setEnabled( false );

    m_ui->frame_hesap_araligi->setDisabled ( true );
    m_ui->frame_bakiyesiz_hesaplar->setDisabled ( true );
    m_ui->frame_calismamis_hesaplar->setDisabled ( true );

    SET_NAME_OF_RUN_BATCH_BUTTON    ( tr ( "Rapor Al" ) );
    SET_FIRST_FOCUS_WIDGET          ( m_ui->searchedit_tek_hesap );
    FOCUS_FIRST_WIDGET              ();

    if ( m_cari_hesap_id NE -1 ) {
        m_ui->checkbox_tek_hesap->setChecked(true);
        m_ui->frame_hesap_araligi->setDisabled(true);
        m_ui->checkbox_hesap_araligi->setCheckState(Qt::Unchecked);
        m_ui->frame_tek_hesap->setEnabled(true);
        SQL_QUERY query(DB);

        query.PREPARE_SELECT("car_hesaplar","cari_hesap_kodu,adres_id","cari_hesap_id = :cari_hesap_id");
        query.SET_VALUE(":cari_hesap_id" , m_cari_hesap_id);

        if( query.SELECT() NE 0 ) {
            query.NEXT();
            m_ui->searchedit_tek_hesap->SET_TEXT(query.VALUE(0).toString());

            m_ui->lineedit_tek_hesap_ismi->setText(ADR_GET_FIRMA_SAHIS_ADI(query.VALUE(0).toInt()));
        }

    }

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
       m_ui->frame_entegre_filtresi->setVisible( true );
    }
    SET_ENTER_KEY_FOR_RUN_BATCH     ( true );
    SET_SETTING_NAME                ( "CARI_HESAP_EKSTRESI" );
}

/**************************************************************************************
                   CARI_HESAP_EKSTRESI_BATCH::CHECK_VAR
***************************************************************************************/

int CARI_HESAP_EKSTRESI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( ( object EQ m_ui->searchedit_bas_hesabi ) OR
              ( object EQ m_ui->searchedit_bts_hesabi ) OR
              ( object EQ m_ui->searchedit_tek_hesap ) ) {

        QString cari_hesap_kodu;

        if ( object EQ m_ui->searchedit_bas_hesabi ) {
            if ( m_ui->searchedit_bas_hesabi->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bas_hesap_ismi->clear();
                return ADAK_OK;
            }
            cari_hesap_kodu = m_ui->searchedit_bas_hesabi->GET_TEXT();
        }
        else if ( object EQ m_ui->searchedit_bts_hesabi ) {

            if ( m_ui->searchedit_bts_hesabi->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bts_hesap_ismi->clear();
                return ADAK_OK;
            }
            cari_hesap_kodu = m_ui->searchedit_bts_hesabi->GET_TEXT();
        }
        else {
            if ( m_ui->searchedit_tek_hesap->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_tek_hesap_ismi->clear();
                return ADAK_OK;
            }
            cari_hesap_kodu = m_ui->searchedit_tek_hesap->GET_TEXT();
        }

        int cari_hesap_id = CARI_FIND_HESAP_ID ( cari_hesap_kodu );

        if ( cari_hesap_id EQ 0 ) {
            MSG_WARNING( tr ( "Cari Hesap Kodu bulunamadı!.."), m_ui->searchedit_tek_hesap );

            return ADAK_FAIL;
        }

        if ( object EQ m_ui->searchedit_bas_hesabi ) {
            m_ui->lineedit_bas_hesap_ismi->setText ( CARI_FIND_HESAP_ISMI ( cari_hesap_id ) );
        }
        else if ( object EQ m_ui->searchedit_bts_hesabi ) {
            m_ui->lineedit_bts_hesap_ismi->setText ( CARI_FIND_HESAP_ISMI ( cari_hesap_id ) );
        }
        else {
            m_ui->lineedit_tek_hesap_ismi->setText ( CARI_FIND_HESAP_ISMI ( cari_hesap_id ) );
        }
    }
    else if ( object EQ m_ui->checkbox_tek_hesap ) {

        if ( m_ui->checkbox_tek_hesap->isChecked() EQ false ) {
             m_ui->searchedit_tek_hesap->SET_TEXT ( "" );;
             m_ui->lineedit_tek_hesap_ismi->clear();
             m_ui->frame_tek_hesap->setDisabled ( true );
             m_ui->checkbox_hesap_araligi->setCheckState ( Qt::Checked );
             m_ui->frame_hesap_araligi->setEnabled ( true );
             SET_FOCUS( m_ui->searchedit_bas_hesabi );

             m_ui->checkbox_bakiyesiz_hesaplar->setChecked( false);
             m_ui->checkbox_bakiyesiz_hesaplar->setEnabled( true );

        }
        else {
            SET_FOCUS( m_ui->searchedit_tek_hesap );
            m_ui->frame_tek_hesap->setEnabled ( true );
            m_ui->checkbox_hesap_araligi->setCheckState ( Qt::Unchecked );
            m_ui->searchedit_bas_hesabi->SET_TEXT ( "" );;
            m_ui->searchedit_bts_hesabi->SET_TEXT ( "" );
            m_ui->lineedit_bas_hesap_ismi->clear();
            m_ui->lineedit_bts_hesap_ismi->clear();
            m_ui->frame_hesap_araligi->setDisabled ( true );

            m_ui->checkbox_bakiyesiz_hesaplar->setEnabled( false );
            m_ui->checkbox_bakiyesiz_hesaplar->setChecked( true );
            m_ui->frame_bakiyesiz_hesaplar->setEnabled( false );
        }
    }
    else if ( object EQ m_ui->checkbox_hesap_araligi ) {

        if ( m_ui->checkbox_hesap_araligi->isChecked() EQ false ) {

            m_ui->searchedit_bas_hesabi->SET_TEXT("");;
            m_ui->searchedit_bts_hesabi->SET_TEXT("");
            m_ui->lineedit_bas_hesap_ismi->clear();
            m_ui->lineedit_bts_hesap_ismi->clear();
            m_ui->frame_hesap_araligi->setDisabled ( true );

            m_ui->frame_tek_hesap->setEnabled ( true );
            m_ui->checkbox_tek_hesap->setCheckState ( Qt::Checked );
            SET_FOCUS( m_ui->searchedit_tek_hesap );

        }
        else {
            m_ui->frame_hesap_araligi->setEnabled ( true );
            m_ui->checkbox_bakiyesiz_hesaplar->setEnabled( true );
            m_ui->checkbox_bakiyesiz_hesaplar->setChecked( false );

            m_ui->checkbox_tek_hesap->setCheckState(Qt::Unchecked);
            m_ui->searchedit_tek_hesap->SET_TEXT("");
            m_ui->lineedit_tek_hesap_ismi->clear();
            m_ui->frame_tek_hesap->setDisabled(true);
            SET_FOCUS( m_ui->searchedit_bas_hesabi );
        }
    }
    else if ( object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ false ) {

            m_ui->adakDate_bsl_tarihi->SET_DATE( MALI_YIL_FIRST_DATE()  );
            m_ui->adakDate_bts_tarihi->SET_DATE(MALI_YIL_365_DATE() );
            m_ui->frame_tarih_araligi->setDisabled ( true );

        }
        else {
            m_ui->frame_tarih_araligi->setEnabled ( true);
            SET_FOCUS( m_ui->adakDate_bsl_tarihi );
        }
    }
    else if ( object EQ m_ui->checkbox_bakiyesiz_hesaplar ) {

        if ( m_ui->checkbox_bakiyesiz_hesaplar->isChecked() EQ false ) {
            m_ui->frame_bakiyesiz_hesaplar->setDisabled(true);
        }
        else {
            m_ui->frame_bakiyesiz_hesaplar->setEnabled(true);
        }
    }
    else if ( object EQ m_ui->checkbox_calismamis_hesaplar ) {

        if ( m_ui->checkbox_calismamis_hesaplar->isChecked() EQ false ) {
            m_ui->frame_calismamis_hesaplar->setDisabled(true);
        }
        else {
            m_ui->frame_calismamis_hesaplar->setEnabled(true);
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HESAP_EKSTRESI_BATCH::CHECK_RUN
***************************************************************************************/

int CARI_HESAP_EKSTRESI_BATCH::CHECK_RUN()
{  
    if ( m_ui->searchedit_bas_hesabi->GET_TEXT().isEmpty() EQ false AND m_ui->searchedit_bts_hesabi->GET_TEXT().isEmpty() EQ false ) {
        if (  m_ui->searchedit_bas_hesabi->GET_TEXT() > m_ui->searchedit_bts_hesabi->GET_TEXT() ) {
            MSG_WARNING( tr ( "Başlangıç hesap kodu ,bitiş hesap kodundan büyük olamaz!..."), NULL );
            return ADAK_FAIL;
        }
    }
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bsl_tarihi, m_ui->adakDate_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    if ( m_ui->checkbox_hesap_araligi->isChecked() EQ false AND m_ui->checkbox_tek_hesap->isChecked() EQ false  ) {
        MSG_WARNING( tr("Hesap kriteri seçmelisiniz!.."), m_ui->checkbox_tarih_araligi );

        m_ui->checkbox_hesap_araligi->setChecked( true );
        return ADAK_FAIL;
    }
    if ( m_ui->checkbox_hesap_araligi->isChecked() EQ true ) {
        if ( m_ui->searchedit_bas_hesabi->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Başlangıç cari hesap kodunu girmelisiniz!.."), m_ui->searchedit_bas_hesabi );

            return ADAK_FAIL;
        }
        if ( m_ui->searchedit_bts_hesabi->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Bitiş cari hesap kodunu girmelisiniz!.."), m_ui->searchedit_bts_hesabi );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkbox_tek_hesap->isChecked() EQ true ) {
        if (m_ui->searchedit_tek_hesap->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Cari Hesap Kodunu girmelisiniz!.."), m_ui->searchedit_tek_hesap );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HESAP_EKSTRESI_BATCH::RUN_BATCH
***************************************************************************************/

void CARI_HESAP_EKSTRESI_BATCH::RUN_BATCH()
{
    CARI_RAPOR_VARS * CARI_RV = new CARI_RAPOR_VARS;

    if ( m_ui->checkbox_hesap_araligi->isChecked() EQ true ) {
        CARI_RV->bas_hesap_kodu     = m_ui->searchedit_bas_hesabi->GET_TEXT();
        CARI_RV->bts_hesap_kodu     = m_ui->searchedit_bts_hesabi->GET_TEXT();
        CARI_RV->bakiyesi_hesaplar  = m_ui->checkbox_bakiyesiz_hesaplar->isChecked();
    }
    else {
        CARI_RV->bas_hesap_kodu     = m_ui->searchedit_tek_hesap->GET_TEXT();
        CARI_RV->bts_hesap_kodu     = m_ui->searchedit_tek_hesap->GET_TEXT();
        CARI_RV->bakiyesi_hesaplar  = true; // tek hesapta her durumda bakiyesizleri getirmeli
    }

    QList <int> grup_idler;

    CARI_RV->baslangic_tarihi    = m_ui->adakDate_bsl_tarihi->QDATE();
    CARI_RV->bitis_tarihi        = m_ui->adakDate_bts_tarihi->QDATE();
    CARI_RV->calismamis_hesaplar = m_ui->checkbox_calismamis_hesaplar->isChecked();
    CARI_RV->nakli_yekun         = m_ui->checkbox_nakli_yekun->isChecked();
    CARI_RV->CARI_ID_LIST        = grup_idler;
    CARI_RV->program_id          = m_program_id;
    CARI_RV->modul_id            = m_modul_id;

    QString log_detaylari = QObject::tr("Cari Bakiye Raporu, ") ;

    if(m_ui->checkbox_tek_hesap->isChecked() EQ true ) {
        log_detaylari.append("\nHesap       : " + m_ui->searchedit_tek_hesap->GET_TEXT() + " "
                             + m_ui->lineedit_tek_hesap_ismi->text() + "\n" );
    }
    else {
        log_detaylari.append("\nHesap Aralığı : " +  m_ui->searchedit_bas_hesabi->GET_TEXT() + " "
                             + m_ui->lineedit_bas_hesap_ismi->text()
                             + " - " + m_ui->searchedit_bts_hesabi->GET_TEXT() + " " +
                             m_ui->lineedit_bts_hesap_ismi->text() + "\n"  );
    }

    log_detaylari.append ( QObject::tr ( "Tarih Aralığı    : ") +
                           m_ui->adakDate_bsl_tarihi->QDATE().toString("dd MMMM yyyy") + " - " +
                           m_ui->adakDate_bts_tarihi->QDATE().toString("dd MMMM yyyy") + "\n");

    if ( CARI_RV->nakli_yekun EQ 1 ) {
         log_detaylari.append ( "Nakli Yekün " );
    }

    if ( CARI_RV->calismamis_hesaplar EQ true ) {
        if ( CARI_RV->nakli_yekun EQ 1 ) {
             log_detaylari.append ( " , " );
        }
        log_detaylari.append ( "Çalışmamış Hesaplar " );
    }
    if ( CARI_RV->bakiyesi_hesaplar EQ true ) {
        if( CARI_RV->calismamis_hesaplar EQ true OR CARI_RV->nakli_yekun EQ 1) {
             log_detaylari.append(" , ");
        }
        log_detaylari.append ( "Bakiyesiz Hesaplar ");
    }


    DB->START_TRANSACTION  ();

    E9_LOG_KAYDI_EKLE ( CARI_MODULU ,LOG_CARI_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari );

    DB->COMMIT_TRANSACTION ();

    OPEN_REPORT_SHOWER     ( GET_CARI_HESAP_EKSTRESI_RAPORU (CARI_RV), nativeParentWidget() );

    //delete CARI_RV;
}

/**************************************************************************************
                   CARI_HESAP_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void CARI_HESAP_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * p_widget ,QLineEdit * p_line_edit )
{
    Q_UNUSED ( p_widget );

    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( p_line_edit->text(), SAHIS_ARTI_FIRMA , this, 1, m_program_id, m_modul_id  );

    if ( cari_hesap_id EQ -1 ) {
        return;
    }

    p_line_edit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id ) );
}
