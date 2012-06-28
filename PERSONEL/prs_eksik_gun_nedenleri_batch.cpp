#include "prs_eksik_gun_nedenleri_batch_class.h"
#include "prs_eksik_gun_nedenleri_batch_open.h"
#include "ui_prs_eksik_gun_nedenleri_batch.h"
#include "print.h"
#include "sube_console_utils.h"
#include "e9_gui_utils.h"
#include "cari_console_utils.h"
#include "PERSONEL/prs_gui_utils.h"
#include "adak_std_utils.h"
#include "cari_struct.h"
#include "prs_gui_utils.h"
#include "cari_kart_arama_open.h"
#include "e9_console_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_PRS_EKSIK_GUN_NEDENLERI_BATCH
***************************************************************************************/

void OPEN_PRS_EKSIK_GUN_NEDENLERI_BATCH ( QWidget * parent )
{
    PRS_EKSIK_GUN_NEDENLERI_BATCH * F = new PRS_EKSIK_GUN_NEDENLERI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRS_EKSIK_GUN_NEDENLERI_BATCH::PRS_EKSIK_GUN_NEDENLERI_BATCH
***************************************************************************************/

PRS_EKSIK_GUN_NEDENLERI_BATCH::PRS_EKSIK_GUN_NEDENLERI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::PRS_EKSIK_GUN_NEDENLERI_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );

    m_cari_hesap_id = 0;
    m_personel_id   = 0;
    m_sicil_no.clear();
}

/**************************************************************************************
                   PRS_EKSIK_GUN_NEDENLERI_BATCH::SETUP_FORM
***************************************************************************************/

void PRS_EKSIK_GUN_NEDENLERI_BATCH::SETUP_FORM ()
{
    SET_PAGE_TITLE    ( tr ( "PRS - EKSİK GÜN NEDENLERİ" ) );
    SET_HELP_PAGE     ("personel-bordrosu_eksik-gun-nedenleri");

    // Butonumuzun eklenecegi widgeti register ediyoruz.
    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Kaydet" );

    SET_AUTO_EXIT_BATCH             ( true );

    // Programa ilk girildiginde focuslanacak widget setlenmelidir.
    SET_FIRST_FOCUS_WIDGET ( m_ui->searchEdit_sicil_no );

    E9_FILL_COMBOBOX_AYLAR( m_ui->comboBox_bordro_ay );

    PRS_FILL_COMBO_BOX_BORDRO_YILLARI( m_ui->comboBox_bordro_yil );

    int current_month = QDate::currentDate().month();
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_bordro_ay, E9_GET_AYLAR_STRING( current_month));

    PRS_FILL_COMBO_BOX_EKSIK_GUN_NEDENLERI( m_ui->comboBox_eksik_gun_nedenleri );

    SET_SETTING_NAME  ("Eksik Gün Nedenleri");
}

/**************************************************************************************
                   PRS_EKSIK_GUN_NEDENLERI_BATCH::CHECK_VAR
***************************************************************************************/

int PRS_EKSIK_GUN_NEDENLERI_BATCH::CHECK_VAR ( QObject * object )
{   

    if ( object EQ m_ui->searchEdit_sicil_no ) {
        CARI_HESAP_STRUCT hesap_bilgileri;
        CARI_SET_HESAP_STRUCT_DEFAULTS( &hesap_bilgileri );

        CARI_KART_BILGILERINI_OKU( m_cari_hesap_id, &hesap_bilgileri );

        m_ui->lineEdit_ad_soyad->setText( hesap_bilgileri.cari_hesap_ismi );
    }
    else if ( object EQ m_ui->commaEdit_eksik_gun_sayisi ) {
        if ( m_ui->commaEdit_eksik_gun_sayisi->GET_DOUBLE() > 30.00 ) {
            MSG_WARNING( tr(" Eksik Gün Sayısı 30 Fazla olamaz."), m_ui->commaEdit_eksik_gun_sayisi );
            return ADAK_FAIL_UNDO;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EKSIK_GUN_NEDENLERI_BATCH::CHECK_RUN
***************************************************************************************/

int PRS_EKSIK_GUN_NEDENLERI_BATCH::CHECK_RUN ()
{
    if ( m_ui->searchEdit_sicil_no->GET_TEXT().isEmpty()) {
        MSG_WARNING(  tr("Personelin Sicil Numarasini Giriniz !.."), m_ui->searchEdit_sicil_no );
        return ADAK_FAIL;
    }
    if( m_ui->commaEdit_eksik_gun_sayisi->GET_DOUBLE() EQ 0.00 ) {
        MSG_WARNING( tr( "Lütfen Eksik Gün Sayısını Giriniz." ), m_ui->commaEdit_eksik_gun_sayisi );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EKSIK_GUN_NEDENLERI_BATCH::RUN_BATCH
***************************************************************************************/

void PRS_EKSIK_GUN_NEDENLERI_BATCH::RUN_BATCH ()
{
    m_ay = E9_GET_AYLAR_ENUM( m_ui->comboBox_bordro_ay->currentText() );
    m_yil= QVariant ( m_ui->comboBox_bordro_yil->currentText() ).toInt();

    QString eksik_gun_neden_kodu;

    int eksik_gun_id = PRS_GET_EKSIK_GUN_NEDENLERI_ENUM( m_ui->comboBox_eksik_gun_nedenleri->currentText());
    eksik_gun_neden_kodu    = QVariant( eksik_gun_id ).toString();
    if (  eksik_gun_id < 10  ) {
         eksik_gun_neden_kodu    = tr("0")+ QVariant( eksik_gun_id ).toString();
    }

    DB->START_TRANSACTION();
    SQL_QUERY query( DB );

    query.PREPARE_INSERT( "prs_eksik_gun_nedenleri", "eksik_gun_id",
                          "personel_id, bordro_donemi_ay, bordro_donemi_yil, "
                          "eksik_gun_sayisi, eksik_gun_neden_kodu, "
                          "bas_tarihi, bts_tarihi ");

    query.SET_VALUE(":personel_id"          , m_personel_id );
    query.SET_VALUE(":bordro_donemi_ay"     , E9_GET_AYLAR_ENUM( m_ui->comboBox_bordro_ay->currentText() ));
    query.SET_VALUE(":bordro_donemi_yil"    , m_ui->comboBox_bordro_yil->currentText().toInt() );
    query.SET_VALUE(":eksik_gun_sayisi"     , m_ui->commaEdit_eksik_gun_sayisi->GET_DOUBLE());
    query.SET_VALUE(":eksik_gun_neden_kodu" , eksik_gun_neden_kodu );
    query.SET_VALUE(":bas_tarihi"           , m_ui->adakDate_bas_tarihi->DATE() );
    query.SET_VALUE(":bts_tarihi"           , m_ui->adakDate_bts_tarihi->DATE() );

    query.INSERT();

    DB->COMMIT_TRANSACTION();

    MSG_INFO( tr("Personele ( %1 ) Eksik Gün Eklendi.").arg( m_ui->commaEdit_eksik_gun_sayisi->GET_DOUBLE() ), NULL );

}

/**************************************************************************************
                   PRS_EKSIK_GUN_NEDENLERI_BATCH::CHECK_EXIT
***************************************************************************************/

int PRS_EKSIK_GUN_NEDENLERI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EKSIK_GUN_NEDENLERI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void   PRS_EKSIK_GUN_NEDENLERI_BATCH::SEARCH_EDIT_CLICKED( QWidget *widget, QLineEdit *line_edit )
{
    if ( widget EQ m_ui->searchEdit_sicil_no ) {

        int cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA, this, 0, E9_PROGRAMI, PERSONEL_MODULU, "PERSONEL ARAMA", "DELETED" );

        if ( cari_hesap_id < 1 ) {
            return;
        }

        SQL_QUERY       sql_query( DB );
        sql_query.PREPARE_SELECT("prs_personeller","personel_id, sicil_no ",
                                 "cari_hesap_id = :cari_hesap_id");

        sql_query.SET_VALUE( ":cari_hesap_id"   , cari_hesap_id );

        if ( sql_query.SELECT() NE 0 ) {
            sql_query.NEXT();
            m_personel_id   = sql_query.VALUE(0).toInt();
            m_sicil_no      = sql_query.VALUE(1).toString();
            m_cari_hesap_id = cari_hesap_id;

            line_edit->setText( m_sicil_no );
        }
    }
}

