#include <QDate>
#include "muh_hesap_ekstresi_batch_class.h"
#include "muh_hesap_ekstresi_batch_open.h"
#include "ui_muh_hesap_ekstresi_batch.h"
#include "print.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_enum.h"
#include "e9_log.h"
#include "yonetim.h"
#include "muh_hesap_arama_open.h"
#include "muh_mizan_raporu_open.h"
#include "muh_hesap_ekstresi_raporu_open.h"
#include "adak_std_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
       OPEN_MUH_HESAP_EKSTRESI_BATCH
***************************************************************************************/

void OPEN_MUH_HESAP_EKSTRESI_BATCH (QString p_secili_hesap_kodu, int p_rapor_turu ,QWidget * p_parent )
{
    MUH_HESAP_EKSTRESI_BATCH * B = new MUH_HESAP_EKSTRESI_BATCH (p_secili_hesap_kodu,p_rapor_turu, p_parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
        MUH_HESAP_EKSTRESI_BATCH::MUH_HESAP_EKSTRESI_BATCH
***************************************************************************************/

MUH_HESAP_EKSTRESI_BATCH::MUH_HESAP_EKSTRESI_BATCH (QString p_secili_hesap_kodu , int p_rapor_turu, QWidget *p_parent)  : BATCH_KERNEL ( p_parent ), m_ui ( new Ui::MUH_HESAP_EKSTRESI_BATCH)
{
    m_secili_hesap     = p_secili_hesap_kodu;
    m_rapor_turu       = p_rapor_turu;

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
             MUH_HESAP_EKSTRESI_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_HESAP_EKSTRESI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE( "genel-muhasebe_hesap-ekstresi-muavin" );
    SET_PAGE_TITLE( tr ( "MUH - HESAP EKSTRESİ ( MUAVİN )" ) );

    REGISTER_BUTTON_WIDGET( m_ui->widget_batch_buttons );

    SET_SETTING_NAME( "MUH_HESAP_EKSTRESI_BATCH" );

    SET_NAME_OF_RUN_BATCH_BUTTON( tr ( "Rapor Al" ) );
    SET_ENTER_KEY_FOR_RUN_BATCH( true );

    m_ui->adakDate_baslangic_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bitis_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE() );

    m_ui->frame_sadece_ana_hesaplari_goster->setDisabled ( true );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->comboBox_sirket_turu->setVisible(true);
        m_ui->label_sirket_turu->setVisible(true);
        m_ui->frame_sirket->setVisible(true);
    }

    if (m_secili_hesap.isEmpty() EQ false) {
        m_ui->frame_tek_hesap->setEnabled(true);
        m_ui->checkbox_tek_hesap->setChecked(true);
        m_ui->frame_hesap_araligi->setEnabled(false);
        m_ui->checkbox_hesap_araligi->setChecked(false);
        m_ui->search_edit_tek_hesap_kodu->SET_TEXT(m_secili_hesap);
        SQL_QUERY query ( DB );

        query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi" , "tam_hesap_kodu = :tam_hesap_kodu");

        query.SET_VALUE      ( ":tam_hesap_kodu" , m_secili_hesap );

        if ( query.SELECT() EQ 0 ) {
            m_ui->search_edit_tek_hesap_kodu->SET_TEXT("");
        }
        else {
            query.NEXT();
            m_ui->lineedit_tek_hesap_ismi->setText(query.VALUE(0).toString());
        }
    }

    m_ui->checkbox_hesap_araligi->setChecked( false );
    m_ui->frame_hesap_araligi->setEnabled( false );

    m_ui->checkbox_tek_hesap->setChecked( true );
    // tum bakiler tek hesapta bunlar degıstırılmez
    m_ui->frame_bakiyesiz_hesaplar->setEnabled( false );
    m_ui->frame_calismamis_hesaplar->setEnabled( false );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_bakiye, "Tüm Hesaplar");
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_islem, "Tüm Hesaplar");

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("e9_kullanici_yetkileri" , "hesap_ekstresi_nakli_yekun" ,
                             "kullanici_id = :kullanici_id");
    sql_query.SET_VALUE(":kullanici_id" , KULLANICI_ID());

    int nakli_yekun = 1;

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();

        nakli_yekun = sql_query.VALUE(0).toInt();
    }

    m_ui->checkbox_nakli_yekun->setChecked(QVariant(nakli_yekun).toBool());

    if ( m_rapor_turu NE ENUM_MUH_HESAP_EKSTRESI ) {

        m_ui->checkbox_nakli_yekun->setChecked(true);
        m_ui->frame_bakiyesiz_hesaplar->setHidden(true);
        m_ui->frame_calismamis_hesaplar->setHidden(true);
        m_ui->frame_hesap_araligi->setHidden(true);
        m_ui->frame_nakli_yekun->setHidden(true);
        m_ui->frame_sadece_ana_hesaplari_goster->setHidden(true);
        m_ui->frame_tek_hesap->setHidden(true);

        m_ui->checkbox_ana_hesaplar->setHidden(true);
        m_ui->checkbox_hesap_araligi->setHidden(true);
        m_ui->checkbox_nakli_yekun->setHidden(true);
        m_ui->checkbox_tarih_araligi->setHidden(true);
        m_ui->checkbox_tek_hesap->setHidden(true);

        SET_PAGE_TITLE(QObject::tr( "KASA RAPORU" ) );
        SET_HELP_PAGE( "kasa-perakende_kasa-raporu" );

        if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 1 ) {
            m_ui->checkbox_tek_hesap->setHidden(false);
            m_ui->frame_tek_hesap->setHidden(false);
        }
    }

    SET_FIRST_FOCUS_WIDGET(m_ui->search_edit_tek_hesap_kodu);

    FOCUS_FIRST_WIDGET();
}

/**************************************************************************************
             MUH_HESAP_EKSTRESI_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_HESAP_EKSTRESI_BATCH::CHECK_VAR ( QObject * object )
{
    SQL_QUERY sql_query ( DB );

    if ( object EQ m_ui->search_edit_bas_hesap_kodu OR
              object EQ m_ui->search_edit_bts_hesap_kodu OR
              object EQ m_ui->search_edit_tek_hesap_kodu ) {

        QString tam_hesap_kodu;

        if ( object EQ m_ui->search_edit_bas_hesap_kodu ) {
            if ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bas_hesap_ismi->clear();
                return ADAK_OK;
            }
            tam_hesap_kodu = m_ui->search_edit_bas_hesap_kodu->GET_TEXT();
        }
        else if ( object EQ m_ui->search_edit_bts_hesap_kodu ) {
            if ( m_ui->search_edit_bts_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bts_hesap_ismi->clear();
                return ADAK_OK;
            }
            tam_hesap_kodu = m_ui->search_edit_bts_hesap_kodu->GET_TEXT();
        }
        else {
            if ( m_ui->search_edit_tek_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_tek_hesap_ismi->clear();
                return ADAK_OK;
            }
            tam_hesap_kodu = m_ui->search_edit_tek_hesap_kodu->GET_TEXT();
        }
        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi" , "tam_hesap_kodu = :tam_hesap_kodu");

        sql_query.SET_VALUE      ( ":tam_hesap_kodu" , tam_hesap_kodu );

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING( tr ( "Hesap bulunamadı!.." ), m_ui->search_edit_tek_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        sql_query.NEXT();

        if ( object EQ m_ui->search_edit_bas_hesap_kodu ) {
            m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
        }
        else if ( object EQ m_ui->search_edit_bts_hesap_kodu ) {
            m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
        }
        else {
            m_ui->lineedit_tek_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
        }
    }
    else if ( object EQ m_ui->checkbox_tek_hesap ) {

        if ( m_ui->checkbox_tek_hesap->isChecked() EQ false ) {
            m_ui->search_edit_tek_hesap_kodu->SET_TEXT ( "" );
            m_ui->lineedit_tek_hesap_ismi->clear();
            m_ui->frame_tek_hesap->setDisabled ( true );

            m_ui->checkbox_hesap_araligi->setCheckState ( Qt::Checked );
            m_ui->frame_hesap_araligi->setEnabled ( true );
            SET_FOCUS( m_ui->search_edit_tek_hesap_kodu );
            SET_FOCUS(m_ui->search_edit_bas_hesap_kodu);

            // tum bakiler tek hesapta bunlar degıstırılmez
            m_ui->frame_bakiyesiz_hesaplar->setEnabled( true );
            m_ui->frame_calismamis_hesaplar->setEnabled( true );

            FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_bakiye, "Bakiyeli");
            FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_islem, "Çalışmış");
        }
        else {
            m_ui->frame_tek_hesap->setEnabled ( true );
            SET_FOCUS(m_ui->search_edit_tek_hesap_kodu);

            m_ui->checkbox_hesap_araligi->setCheckState ( Qt::Unchecked );
            m_ui->search_edit_bas_hesap_kodu->SET_TEXT ( "" );
            m_ui->search_edit_bts_hesap_kodu->SET_TEXT ( "" );
            m_ui->lineedit_bas_hesap_ismi->clear();
            m_ui->lineedit_bts_hesap_ismi->clear();
            m_ui->frame_hesap_araligi->setDisabled ( true );

            // tum bakiler tek hesapta bunlar degıstırılmez
            m_ui->frame_bakiyesiz_hesaplar->setEnabled( false );
            m_ui->frame_calismamis_hesaplar->setEnabled( false );

            FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_bakiye, "Tüm Hesaplar");
            FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_islem, "Tüm Hesaplar");
        }
    }
    else if ( object EQ m_ui->checkbox_hesap_araligi ) {

        if ( m_ui->checkbox_hesap_araligi->isChecked() EQ false ) {

            m_ui->search_edit_bas_hesap_kodu->SET_TEXT ( "" );
            m_ui->search_edit_bts_hesap_kodu->SET_TEXT ( "" );
            m_ui->lineedit_bas_hesap_ismi->clear();
            m_ui->lineedit_bts_hesap_ismi->clear();
            m_ui->frame_hesap_araligi->setDisabled ( true );

            m_ui->frame_tek_hesap->setEnabled ( true );
            SET_FOCUS(m_ui->search_edit_tek_hesap_kodu);
            m_ui->checkbox_tek_hesap->setCheckState ( Qt::Checked );
            SET_FOCUS( m_ui->search_edit_bas_hesap_kodu );

            // tum bakiler tek hesapta bunlar degıstırılmez
            m_ui->frame_bakiyesiz_hesaplar->setEnabled( false );
            m_ui->frame_calismamis_hesaplar->setEnabled( false );

            FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_bakiye, "Tüm Hesaplar");
            FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_islem, "Tüm Hesaplar");

        }
        else {
            m_ui->frame_hesap_araligi->setEnabled ( true );

            SET_FOCUS(m_ui->search_edit_bas_hesap_kodu);
            m_ui->checkbox_tek_hesap->setCheckState ( Qt::Unchecked );
            m_ui->search_edit_tek_hesap_kodu->SET_TEXT ( "" );
            m_ui->lineedit_tek_hesap_ismi->clear();
            m_ui->frame_tek_hesap->setDisabled ( true );

            // tum bakiler tek hesapta bunlar degıstırılmez
            m_ui->frame_bakiyesiz_hesaplar->setEnabled( true );
            m_ui->frame_calismamis_hesaplar->setEnabled( true );

            FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_bakiye, "Bakiyeli");
            FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_islem, "Çalışmış");
        }
    }
    else if ( object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ false ) {
             m_ui->adakDate_baslangic_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
             m_ui->adakDate_bitis_tarihi->SET_DATE( MALI_YIL_365_DATE() );
             m_ui->frame_tarih_araligi->setDisabled ( true );
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled ( true );
            SET_FOCUS( m_ui->adakDate_baslangic_tarihi);
        }
    }
    else if ( object EQ m_ui->checkbox_nakli_yekun ) {

        if ( m_ui->checkbox_nakli_yekun->isChecked() EQ false ) {
            m_ui->frame_nakli_yekun->setDisabled ( true );
        }
        else {
            m_ui->frame_nakli_yekun->setEnabled ( true );
        }
    }
    else if ( object EQ m_ui->checkbox_ana_hesaplar ) {

        if ( m_ui->checkbox_ana_hesaplar->isChecked() EQ false ) {
            m_ui->frame_sadece_ana_hesaplari_goster->setDisabled ( true );
        }
        else {
            m_ui->frame_sadece_ana_hesaplari_goster->setEnabled ( true );
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
          MUH_HESAP_EKSTRESI_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_HESAP_EKSTRESI_BATCH::CHECK_RUN()
{

    if ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT().isEmpty() EQ false AND m_ui->search_edit_bts_hesap_kodu->GET_TEXT().isEmpty() EQ false ) {
        if ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT() > m_ui->search_edit_bts_hesap_kodu->GET_TEXT() ) {
            MSG_WARNING( tr ( "Başlangıç hesap kodu ,bitiş hesap kodundan büyük olamaz!..." ), m_ui->search_edit_bas_hesap_kodu );
            return ADAK_FAIL;
        }
    }

    if ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT().isEmpty() EQ false AND m_ui->search_edit_bts_hesap_kodu->GET_TEXT().isEmpty() EQ false ) {
        if ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT() > m_ui->search_edit_bts_hesap_kodu->GET_TEXT() ) {
            MSG_WARNING( tr ( "Başlangıç hesap kodu ,bitiş hesap kodundan büyük olamaz!..." ), m_ui->search_edit_bts_hesap_kodu );
            return ADAK_FAIL;
        }
    }

    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_baslangic_tarihi, m_ui->adakDate_bitis_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    if ( m_ui->checkbox_hesap_araligi->isChecked() EQ false AND
         m_ui->checkbox_tek_hesap->isChecked() EQ false  ) {
        MSG_WARNING( tr ( "Hesap kriteri seçmelisiniz!.." ), NULL );
        return ADAK_FAIL;
    }

    if ( m_ui->checkbox_hesap_araligi->isChecked() EQ true ) {
        if ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
             MSG_WARNING( tr ( "Başlangıç hesap kodunu girmelisiniz!.." ), m_ui->search_edit_bas_hesap_kodu );

            return ADAK_FAIL;
        }
        if ( m_ui->search_edit_bts_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
             MSG_WARNING( tr("Bitiş hesap kodunu girmelisiniz!.."), m_ui->search_edit_bts_hesap_kodu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkbox_tek_hesap->isChecked() EQ true ) {
        if ( m_ui->search_edit_tek_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Hesap kodunu girmelisiniz!.." ), m_ui->search_edit_tek_hesap_kodu );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
          MUH_HESAP_EKSTRESI_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_HESAP_EKSTRESI_BATCH::RUN_BATCH()
{
    MUH_RAPOR_VARS * MUH_RV   = new MUH_RAPOR_VARS;

    if ( m_ui->checkbox_hesap_araligi->isChecked() EQ true ) {
        MUH_RV->bas_hesap_kodu      = m_ui->search_edit_bas_hesap_kodu->GET_TEXT();
        MUH_RV->bts_hesap_kodu      = m_ui->search_edit_bts_hesap_kodu->GET_TEXT();
    }
    if ( m_ui->checkbox_tek_hesap->isChecked() EQ true ) {
        MUH_RV->bas_hesap_kodu = m_ui->search_edit_tek_hesap_kodu->GET_TEXT();
        MUH_RV->bts_hesap_kodu = m_ui->search_edit_tek_hesap_kodu->GET_TEXT();
    }

    MUH_RV->sadece_ana_hesaplar = m_ui->checkbox_ana_hesaplar->isChecked();
    MUH_RV->baslangic_tarihi    = m_ui->adakDate_baslangic_tarihi->QDATE();
    MUH_RV->bitis_tarihi        = m_ui->adakDate_bitis_tarihi->QDATE();
    MUH_RV->nakli_yekun         = m_ui->checkbox_nakli_yekun->isChecked();

    MUH_RV->bakiyesiz_hesaplar  = m_ui->combo_box_bakiye->currentIndex();
    MUH_RV->calismamis_hesaplar = m_ui->combo_box_islem->currentIndex();

    MUH_RV->bas_gun_no          = m_ui->adakDate_baslangic_tarihi->MALI_YIL_ARRAY_INDIS();
    MUH_RV->bts_gun_no          = m_ui->adakDate_bitis_tarihi->MALI_YIL_ARRAY_INDIS();

    QString log_detaylari = QObject::tr ("Hesap Ekstresi, " ) ;

    if ( m_ui->checkbox_tek_hesap->isChecked() EQ true ) {
         log_detaylari.append ( "\nHesap       : " + m_ui->search_edit_tek_hesap_kodu->GET_TEXT() + " "
                               + m_ui->lineedit_tek_hesap_ismi->text() + "\n" );
    }
    else {
        log_detaylari.append ( "\nHesap Aralığı : " +  m_ui->search_edit_bas_hesap_kodu->GET_TEXT() + " "
                              + m_ui->lineedit_bas_hesap_ismi->text()
                              + " - " + m_ui->search_edit_bts_hesap_kodu->GET_TEXT() + " " +
                              m_ui->lineedit_bts_hesap_ismi->text() + "\n"  );
    }
    log_detaylari.append ( QObject::tr ( "Tarih Aralığı    : " ) +
                         m_ui->adakDate_baslangic_tarihi->QDATE().toString("dd MMMM yyyy") + " - " +
                         m_ui->adakDate_bitis_tarihi->QDATE().toString("dd MMMM yyyy") + "\n" );


    if ( MUH_RV->nakli_yekun EQ true ) {
        log_detaylari.append ( "Nakli Yekün " );
    }

    if ( MUH_RV->calismamis_hesaplar NE 0) {
        if ( MUH_RV->nakli_yekun EQ true) {
             log_detaylari.append ( " , " );
        }
        log_detaylari.append ( "Çalışmamış Hesaplar " );
    }
    if ( MUH_RV->bakiyesiz_hesaplar EQ true ) {
        if ( MUH_RV->calismamis_hesaplar NE 0 OR MUH_RV->nakli_yekun EQ true) {
             log_detaylari.append ( " , " );
        }
        log_detaylari.append ( "Bakiyesiz Hesaplar " );
    }

    DB->START_TRANSACTION();
    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU ,LOG_MUH_RAPORLAR ,LOG_ISLEM_RAPOR_ALINDI, log_detaylari );
    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER ( GET_HESAP_EKSTRESI_RAPORU(MUH_RV), nativeParentWidget() );

    delete MUH_RV;
}

/**************************************************************************************
                   MUH_HESAP_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED
****************************************************************************************/

void MUH_HESAP_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * p_widget, QLineEdit * p_lineEdit )
{

    int hesap_id = 0;

    if ( p_widget EQ m_ui->search_edit_bas_hesap_kodu ) {
       hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT(),this );
    }

    else if ( p_widget EQ m_ui->search_edit_bts_hesap_kodu ) {
        hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->search_edit_bts_hesap_kodu->GET_TEXT(),this );
    }

    else {
        hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->search_edit_tek_hesap_kodu->GET_TEXT(),this );
    }
    if ( hesap_id EQ -1 ) {
        return;
    }
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      (  ":hesap_id", hesap_id  );

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        if ( p_widget EQ m_ui->search_edit_bas_hesap_kodu ) {
            p_lineEdit->setText ( sql_query.VALUE(0).toString() );
        }
        else if ( p_widget EQ m_ui->search_edit_bts_hesap_kodu ) {
            p_lineEdit->setText ( sql_query.VALUE(0).toString() );
        }
        else {
            p_lineEdit->setText ( sql_query.VALUE(0).toString() );
        }
    }
}
