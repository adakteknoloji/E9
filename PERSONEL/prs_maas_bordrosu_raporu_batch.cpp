#include "prs_maas_bordrosu_raporu_batch_class.h"
#include "prs_maas_bordrosu_raporu_batch_open.h"
#include "print.h"
#include "prs_enum.h"
#include "prs_struct.h"
#include "e9_log.h"
#include "adak_std_utils.h"
#include "cari_console_utils.h"
#include "e9_gui_utils.h"
#include "cari_kart_arama_open.h"
#include "prs_maas_bordrosu_raporu_open.h"
#include "cari_struct.h"
#include "prs_con_utils.h"
#include "prs_gui_utils.h"
#include "e9_console_utils.h"
#include <QDebug>

extern ADAK_SQL      *      DB;
extern PRS_SABITLER          G_PRS_SABITLER;


void SHOW_PRS_MAAS_BORDROSU_RAPORU_BATCH ( int p_sube_id , QWidget * parent)
{
    PRS_MAAS_BORDROSU_RAPORU_BATCH * B = new PRS_MAAS_BORDROSU_RAPORU_BATCH( p_sube_id, parent);

    B->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_RAPORU_BATCH::PRS_MAAS_BORDROSU_RAPORU_BATCH
***************************************************************************************/

PRS_MAAS_BORDROSU_RAPORU_BATCH::PRS_MAAS_BORDROSU_RAPORU_BATCH(int p_sube_id , QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::PRS_MAAS_BORDROSU_RAPORU_BATCH)
{
    m_ui->setupUi       ( this);
    START_BATCH_KERNEL  ( this, DB );

    m_sube_id   = p_sube_id;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_RAPORU_BATCH::SETUP_FORM
**************************************************************************************/

void PRS_MAAS_BORDROSU_RAPORU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ( "personel-bordrosu_maas-bordrosu-raporu" );
    SET_PAGE_TITLE      ( tr ("PRS - MAAŞ BORDROSU RAPORU") );

    REGISTER_BUTTON_WIDGET          ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME    ( "PRS_MAAS_BORDROSU_RAPORU_BATCH" );
    SET_NAME_OF_RUN_BATCH_BUTTON    ( tr ("MAAŞ BORDROSU RAPORU AL") );

    COMBO_BOX_BORDRO_YILI_DOLDUR ();

    E9_FILL_COMBOBOX_AYLAR ( m_ui->combo_box_aylar );

    m_ay       = E9_GET_AYLAR_ENUM( m_ui->combo_box_aylar->currentText() );
    m_yil      = m_ui->combo_box_yillar->currentText().toInt();

    m_ui->frame_personel->setHidden(true);
    m_ui->frame_sicil_no->setEnabled(false);

    m_ui->search_edit_sicil_no->SET_BUTTON_SIZE( 30, 50);

    SET_FIRST_FOCUS_WIDGET ( m_ui->combo_box_aylar );
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_RAPORU_BATCH::CHECK_BOX_CHANGED
***************************************************************************************/

void PRS_MAAS_BORDROSU_RAPORU_BATCH::CHECK_BOX_CHANGED()
{
    if ( m_ui->check_box_personel_arama->isChecked() EQ true ) {

        m_ui->frame_sicil_no->setEnabled(true);
    }
    else {

        m_ui->frame_sicil_no->setEnabled(false);
        m_ui->line_edit_personel_adi_soyadi->clear();
        m_ui->frame_personel->setHidden(true);
    }
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_RAPORU_BATCH::CHECK_VAR
**************************************************************************************/

int PRS_MAAS_BORDROSU_RAPORU_BATCH::CHECK_VAR(QObject * object)
{
    if ( object EQ m_ui->search_edit_sicil_no ) {
        m_ui->frame_personel->setHidden(false);
        m_sicil_no   = m_ui->search_edit_sicil_no->GET_TEXT();
    }
    else if ( object EQ m_ui->combo_box_aylar ) {
        m_ay = E9_GET_AYLAR_ENUM( m_ui->combo_box_aylar->currentText() );
    }
    else if ( object EQ m_ui->combo_box_yillar ) {
        m_yil = m_ui->combo_box_yillar->currentText().toInt();
    }
    else if ( object EQ m_ui->check_box_personel_arama ) {
        CHECK_BOX_CHANGED();
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_RAPORU_BATCH::CHECK_RUN
**************************************************************************************/

int PRS_MAAS_BORDROSU_RAPORU_BATCH::CHECK_RUN()
{
    if ( m_ui->combo_box_aylar->currentIndex() < 0 OR
         m_ui->combo_box_yillar->currentIndex() < 0) {

        MSG_WARNING(  tr("Ay ve Yıl Bilgilerini Doğru Girmelisiniz !"), m_ui->combo_box_aylar );

        return ADAK_FAIL;
     }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_RAPORU_BATCH::RUN_BATCH
***************************************************************************************/

void PRS_MAAS_BORDROSU_RAPORU_BATCH::RUN_BATCH()
{
    int     bordro_id       = 0;
    int     cari_hesap_id   = 0;

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "prs_personeller",
                          "cari_hesap_id ", "sube_id = :sube_id" );

    query.SET_VALUE     ( ":sube_id", m_sube_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        cari_hesap_id   = query.VALUE( 0 ).toInt();   
    }

    CARI_HESAP_STRUCT hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri  );

    PRS_FILL_SABIT_DEGERLER_STRUCT    ();
    PRS_FILL_FIRMA_ODENEKLERI_STRUCT  ();

    if ( PRS_VERILEN_DONEM_BORDROSU_KESILMIS_MI ( m_ay, m_yil, m_sube_id ) EQ  ADAK_FAIL ) {
        MSG_WARNING( tr("Girilen Dönem İçin Hazırlanmış Bordro Bulunamamıştır !.."), m_ui->combo_box_aylar );
        return;
    }

    if ( m_ui->check_box_personel_arama->isChecked() EQ true ) {

        bordro_id = PRS_GET_BORDRO_ID ( m_ay, m_yil , m_sube_id );

        if( PRS_PERSONEL_BORDRODA_MEVCUT_MU ( bordro_id, m_personel_id ) EQ ADAK_FAIL ){

            MSG_WARNING( hesap_bilgileri.cari_hesap_ismi.toUpper() +
                                   tr(" İçin Hazırlanmış ") + E9_GET_AYLAR_STRING( m_ay ) +" " +
                                   QVariant(m_yil).toString() + tr(" Bordrosu Bulunamamıştır !.."), NULL );
            return;
        }
        OPEN_REPORT_SHOWER( SHOW_MAAS_BORDROSU_RAPORU( bordro_id, m_sicil_no, m_ay, m_yil ), nativeParentWidget() );
    }

    else if ( m_ui->check_box_personel_arama->isChecked() EQ false ) {

        bordro_id = PRS_GET_BORDRO_ID ( m_ay, m_yil , m_sube_id );

        OPEN_REPORT_SHOWER( SHOW_MAAS_BORDROSU_RAPORU( bordro_id, "-1", m_ay, m_yil ), nativeParentWidget(), QPrinter::Landscape );
    }

    QString log_detaylari = QObject::tr("Maaş Bordrosu Raporu , ") + m_ui->combo_box_aylar->currentText()
                            + " " + m_ui->combo_box_yillar->currentText();

    if(m_ui->check_box_personel_arama->isChecked() EQ true ) {
        log_detaylari.append("\nSicil No : " + m_ui->search_edit_sicil_no->GET_TEXT());
    }

    DB->START_TRANSACTION();

    E9_LOG_KAYDI_EKLE(PERSONEL_MODULU , LOG_PRS_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari);

    DB->COMMIT_TRANSACTION();

}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_RAPORU_BATCH::CHECK_EXIT
***************************************************************************************/

int PRS_MAAS_BORDROSU_RAPORU_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}

/**************************************************************************************
                  PRS_MAAS_BORDROSU_RAPORU_BATCH::PERSONELI_SEC
**************************************************************************************/

void PRS_MAAS_BORDROSU_RAPORU_BATCH::PERSONELI_SEC()
{
    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA, this, 0, E9_PROGRAMI, PERSONEL_MODULU , "Personel Arama", "DELETED" );


    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id",
                             "cari_hesap_id = :cari_hesap_id");

    sql_query.SET_VALUE( ":cari_hesap_id"   , cari_hesap_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();
        m_personel_id = sql_query.VALUE(0).toInt();
    }

    if ( m_personel_id <= 0 ) {
        return;
    }


    m_sicil_no  =  m_ui->search_edit_sicil_no->GET_TEXT();

    m_ui->frame_personel->setHidden(false);
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_RAPORU_BATCH::COMBO_BOX_BORDRO_YILI_DOLDUR
***************************************************************************************/

void PRS_MAAS_BORDROSU_RAPORU_BATCH::COMBO_BOX_BORDRO_YILI_DOLDUR ()
{
    PRS_FILL_COMBO_BOX_BORDRO_YILLARI( m_ui->combo_box_yillar );

    int current_month = QDate::currentDate().month();
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_aylar, E9_GET_AYLAR_STRING( current_month));
}

/***************************************************************************************
                   PRS_MAAS_BORDROSU_RAPORU_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void PRS_MAAS_BORDROSU_RAPORU_BATCH::SEARCH_EDIT_CLICKED( QWidget *, QLineEdit * line_edit)
{
    Q_UNUSED(line_edit);
    PERSONELI_SEC ();
}
