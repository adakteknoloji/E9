#include <QMessageBox>
#include "cari_fis_arama_class.h"
#include "ui_cari_fis_arama.h"
#include "adak_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "cari_enum.h"
#include "e9_enum.h"
#include "cari_kart_arama_open.h"
#include "cari_console_utils.h"
#include "cari_gui_utils.h"
#include "cari_hareket_fisi_open.h"
#include "cari_odeme_fisi_open.h"

#define     FIS_ID_COLUMN           0
#define     FIS_TARIHI_COLUMN       1
#define     FIS_NO_COLUMN           2
#define     CARI_HESAP_ADI          3
#define     ACIKLAMA_COLUMN         4
#define     TOPLAM_BORC_COLUMN      5
#define     TOPLAM_ALACAK_COLUMN    6

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_CARI_FIS_ARAMA
***************************************************************************************/

int OPEN_CARI_FIS_ARAMA ( int p_cari_modulu_fis_turu , int p_fis_kaynagi, CARI_FIS_TURU p_cari_fis_turu, QWidget * p_parent )
{
    int secilen_id = -1;
    CARI_FIS_ARAMA * A = new CARI_FIS_ARAMA ( p_cari_modulu_fis_turu, p_fis_kaynagi, p_cari_fis_turu, p_parent);
    A->m_secilen_id = &secilen_id;
    A->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   CARI_FIS_ARAMA::CARI_FIS_ARAMA
***************************************************************************************/

CARI_FIS_ARAMA::CARI_FIS_ARAMA ( int p_cari_modulu_fis_turu, int p_fis_kaynagi, CARI_FIS_TURU p_cari_fis_turu, QWidget * p_parent )
: ARAMA_KERNEL ( p_parent ), m_ui ( new Ui::CARI_FIS_ARAMA )
{
    m_modul_id             = p_fis_kaynagi;
    m_cari_modulu_fis_turu = p_cari_modulu_fis_turu;
    m_cari_fis_turu        = p_cari_fis_turu;

    m_ui->setupUi      (this);
    START_ARAMA_KERNEL (this, DB );
}

/**************************************************************************************
                   CARI_FIS_ARAMA::SETUP_FORM
***************************************************************************************/

void CARI_FIS_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE( "cari-hesap-islemleri" );

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_SORTING ( false );

    m_cari_hesap_id = 0;

    m_ui->date_frame->setEnabled                ( false );
    m_ui->frame_cari_hesap_bilgisi->setEnabled  ( false );


    CARI_FILL_CARI_ENT_FIS_TURU_COMBO_BOX( m_ui->comboBox_ent_fis_turu );

    if ( m_cari_fis_turu EQ ENUM_CARI_ENT_FISI ) {

        SET_FIRST_FOCUS_WIDGET        ( m_ui->comboBox_ent_fis_turu );
        SET_PAGE_TITLE ( tr ( "CARİ ENTEGRASYON FİŞLERİ" ) );
         SET_HELP_PAGE ( "cari-hesap-islemleri_cari-entegrasyon-fisleri" );
        m_ui->checkbox_cari_hesap->setVisible( false );
        m_ui->frame_cari_hesap_bilgisi->setVisible( false );

        m_ui->checkBox_ent_fis_turu->setChecked( true );
    }
    else {
        m_ui->checkbox_tarih_araligi->setChecked( true );
        m_ui->date_frame->setEnabled( true );
        SET_FIRST_FOCUS_WIDGET        ( m_ui->adakDate_bas_tarihi );
        SET_PAGE_TITLE ( tr ( "CARİ - FİŞ ARAMA" ) );
        m_ui->checkBox_ent_fis_turu->setVisible( false );
        m_ui->frame_ent_fis_turu->setVisible(  false );
    }

    m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );

    switch ( m_cari_modulu_fis_turu ) {

        case ENUM_CARI_HAREKET_FISI :

            m_ui->frame_cari_hesap_bilgisi->setHidden ( true );
            m_ui->checkbox_cari_hesap->setHidden ( true );
            break;

        case ENUM_CARI_ACILIS_FISI  :

            m_ui->frame_cari_hesap_bilgisi->setHidden ( true );
            m_ui->checkbox_cari_hesap->setHidden      ( true );

            m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_bitis_tarih->SET_DATE( MALI_YIL_FIRST_DATE() );

            m_ui->date_frame->setHidden(true);
            m_ui->checkbox_tarih_araligi->setHidden(true);
            break;

        default :
            break;
    }

    SET_HEADERS ( QStringList() << tr ( "Fis Id" )      << tr ( "Fiş Tarihi" ) << tr ( "Fiş No" ) << tr(" Cari Hesap Adı") << tr ( "Açıklama" )
                                << tr ( "Toplam Borç" ) << tr ( "Toplam Alacak" ) );

    SET_ENTER_KEY_FOR_RUN_BATCH         ( true );

    m_ui->tableWidget->setColumnWidth   ( FIS_NO_COLUMN,          50  );
    m_ui->tableWidget->setColumnWidth   ( FIS_TARIHI_COLUMN,      250 );
    m_ui->tableWidget->setColumnWidth   ( CARI_HESAP_ADI,         180 );
    m_ui->tableWidget->setColumnWidth   ( ACIKLAMA_COLUMN,        220 );
    m_ui->tableWidget->setColumnWidth   ( TOPLAM_BORC_COLUMN,     120 );
    m_ui->tableWidget->setColumnWidth   ( TOPLAM_ALACAK_COLUMN,   120 );

    m_ui->tableWidget->hideColumn       ( FIS_ID_COLUMN );


    m_ui->frame_entegre_filtresi->setHidden(true);

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ 1 ) {
        m_ui->frame_entegre_filtresi->setHidden(false);
    }

    SET_SETTING_NAME                    ( "CARI_FIS_ARAMA" );
    SET_FIND_BUTTON_NAME                ( tr ( "Fişleri Bul" ) );
}

/**************************************************************************************
                   CARI_FIS_ARAMA::CHECK_VAR
***************************************************************************************/

int CARI_FIS_ARAMA::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_cari_modulu_fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
            MSG_WARNING( tr ( "Tarih Aralığını Değiştiremezsiniz!.." ), m_ui->checkbox_tarih_araligi );
            m_ui->checkbox_tarih_araligi->setChecked( false );
            return ADAK_FAIL;
        }

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ true ) {
            m_ui->date_frame->setEnabled ( true );
            SET_FOCUS( m_ui->adakDate_bas_tarihi);
        }
        else {
            m_ui->date_frame->setDisabled(true);
            m_ui->adakDate_bas_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
        }
    }
    else if ( p_object EQ m_ui->checkbox_cari_hesap ) {

        if ( m_ui->checkbox_cari_hesap->isChecked() EQ true ) {
            m_ui->frame_cari_hesap_bilgisi->setEnabled ( true );
            SET_FOCUS( m_ui->searchEdit_cari_hesap_kodu );
        }
        else {
            m_ui->frame_cari_hesap_bilgisi->setEnabled ( false );
            m_ui->searchEdit_cari_hesap_kodu->SET_TEXT("");
            m_ui->lineEdit_cari_hesap_ismi->clear();
            m_cari_hesap_id = -1;
        }
    }
    else if ( p_object EQ m_ui->searchEdit_cari_hesap_kodu ) {

        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_cari_hesap_ismi->clear();
            m_cari_hesap_id = -1;
            return ADAK_OK;
        }

        QString cari_hesap_ismi;
        int     cari_hesap_id;

        int select = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi );

        if ( select EQ 0 ) {
            MSG_WARNING(  tr ( "Cari hesap kodu bulunamadı"), m_ui->searchEdit_cari_hesap_kodu  );

            return ADAK_FAIL_UNDO;
        }


        m_ui->lineEdit_cari_hesap_ismi->setText ( cari_hesap_ismi);
        m_cari_hesap_id =  cari_hesap_id;

    }
    else if ( p_object EQ m_ui->comboBox_ent_fis_turu ) {
        m_modul_id = CARI_GET_ENT_FIS_MODUL_ID( m_ui->comboBox_ent_fis_turu->currentText() );
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_FIS_ARAMA::CHECK_RUN
***************************************************************************************/

int CARI_FIS_ARAMA::CHECK_RUN()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("car_fisler","fis_id","fis_id =:fis_id");

    for ( int i = 0 ; i < 3 ; i++ )  {
        query.SET_VALUE(":fis_id",1);
        query.SELECT();
    }


    if ( m_ui->checkbox_cari_hesap->isChecked() EQ true ) {
        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Cari hesap kodunu seçmelisiniz" ), m_ui->searchEdit_cari_hesap_kodu );
            return ADAK_FAIL;
        }
        if ( m_cari_hesap_id < 1 ) {
            MSG_WARNING( tr ( "Cari hesap seçimi yapılmamış" ), m_ui->searchEdit_cari_hesap_kodu  );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_FIS_ARAMA::SEARCH
***************************************************************************************/

void CARI_FIS_ARAMA::SEARCH()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT ( "car_fisler"," fis_id, fis_no, fis_tarihi,aciklama,toplam_borc,toplam_alacak, cari_hesap_id ", "fis_turu = :fis_turu"  );
    select_query.SET_VALUE      (":fis_turu",m_cari_modulu_fis_turu);


    if( m_modul_id EQ 0 ) {
        select_query.AND_EKLE ("program_id != :program_id");
        select_query.SET_VALUE(":program_id",E9_PROGRAMI  );
    }
    else {
        // Tum ent fisleri gosterilecek
        if ( m_modul_id EQ -1 ) {
            select_query.AND_EKLE ("modul_id != :modul_id");
            select_query.SET_VALUE(":modul_id", CARI_MODULU );
        }
        else {
            select_query.AND_EKLE ("modul_id = :modul_id");
            select_query.SET_VALUE(":modul_id",m_modul_id);
        }
    }

    if ( m_ui->checkbox_tarih_araligi->isChecked() EQ true ) {
        select_query.AND_EKLE ("fis_tarihi BETWEEN :fis_tarihi_1 AND :fis_tarihi_2");
        select_query.SET_VALUE(":fis_tarihi_1" , m_ui->adakDate_bas_tarihi->DATE() );
        select_query.SET_VALUE(":fis_tarihi_2" , m_ui->adakDate_bitis_tarih->DATE() );
    }

    if ( m_ui->checkbox_cari_hesap->isChecked() EQ true ) {
        select_query.AND_EKLE ("cari_hesap_id = :cari_hesap_id");
        select_query.SET_VALUE(":cari_hesap_id" , m_cari_hesap_id );
    }

    if ( select_query.SELECT("fis_tarihi,fis_no") NE 0 ) {

        int current_row;

        while ( select_query.NEXT() EQ true ) {

            current_row = ADD_NEW_ROW();

            m_ui->tableWidget->item ( current_row, TOPLAM_BORC_COLUMN   )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
            m_ui->tableWidget->item ( current_row, TOPLAM_ALACAK_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );

            m_ui->tableWidget->item ( current_row, FIS_ID_COLUMN        )->setText ( select_query.VALUE(0).toString() );
            m_ui->tableWidget->item ( current_row, FIS_NO_COLUMN        )->setText ( select_query.VALUE(1).toString() );
            m_ui->tableWidget->item ( current_row, FIS_TARIHI_COLUMN    )->setText ( select_query.VALUE(2).toDate().toString ( "dd MMMM yyyy dddd" ) );
            m_ui->tableWidget->item ( current_row, CARI_HESAP_ADI       )->setText ( CARI_GET_FIRMA_SAHIS_ADI( select_query.VALUE(6).toInt() ).mid(0, 30 ) );
            m_ui->tableWidget->item ( current_row, ACIKLAMA_COLUMN      )->setText ( select_query.VALUE(3).toString().mid( 0, 50 ) );
            m_ui->tableWidget->item ( current_row, TOPLAM_BORC_COLUMN   )->setText ( VIRGUL_EKLE ( select_query.VALUE(4).toString() ) );
            m_ui->tableWidget->item ( current_row, TOPLAM_ALACAK_COLUMN )->setText ( VIRGUL_EKLE ( select_query.VALUE(5).toString() ) );
        }
    }
}

/**************************************************************************************
                   CARI_FIS_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int CARI_FIS_ARAMA::SINGLE_LINE_SELECTED ( int p_selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( p_selected_row_number, 0 )->text().toInt();

    if ( m_cari_fis_turu EQ ENUM_CARI_FISI ) {
        return ADAK_EXIT;
    }

    if ( m_ui->comboBox_ent_fis_turu->currentText() EQ QObject::tr("Çek-Senet Entegrasyon Fişi ( Ödeme )")) {
        OPEN_CARI_ODEME_FISI ( *m_secilen_id , m_modul_id , nativeParentWidget() );
    }
    else {
        OPEN_CARI_HAREKET_FISI ( *m_secilen_id , ENUM_CARI_HAREKET_FISI, m_modul_id , nativeParentWidget() );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_FIS_ARAMA::SEARCH_EDIT_CLICKED
***************************************************************************************/

void CARI_FIS_ARAMA::SEARCH_EDIT_CLICKED ( QWidget * p_widget , QLineEdit * p_lineEdit )
{
    Q_UNUSED ( p_widget );

    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( p_lineEdit->text() ,SAHIS_ARTI_FIRMA, this , 1, E9_PROGRAMI, CARI_MODULU );


    if ( cari_hesap_id < 1 ) {
        return;
    }
    m_cari_hesap_id = cari_hesap_id;

    p_lineEdit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id ) );
}
