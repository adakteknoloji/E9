#include "prs_enum.h"
#include "prs_struct.h"
#include "prs_gui_utils.h"
#include "prs_isten_ayrilma_batch_class.h"
#include "prs_isten_ayrilma_batch_open.h"
#include "print.h"
#include "e9_log.h"
#include "cari_console_utils.h"
#include "prs_con_utils.h"
#include "adres_console_utils.h"
#include "cari_kart_arama_open.h"
#include "prs_isten_ayrilma_raporu_open.h"
#include "cari_struct.h"
#include "dmr_con_utils.h"


extern ADAK_SQL  * DB;


void SHOW_PRS_ISTEN_AYRILMA_BATCH (QWidget * parent)
{
    PRS_ISTEN_AYRILMA_BATCH * B = new PRS_ISTEN_AYRILMA_BATCH(parent);

    B->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRS_ISTEN_AYRILMA_BATCH::PRS_ISTEN_AYRILMA_BATCH
***************************************************************************************/

PRS_ISTEN_AYRILMA_BATCH::PRS_ISTEN_AYRILMA_BATCH(QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::PRS_ISTEN_AYRILMA_BATCH)
{
    m_ui->setupUi     ( this);
    START_BATCH_KERNEL( this, DB );
}

/**************************************************************************************
                 PRS_ISTEN_AYRILMA_BATCH::SETUP_FORM
**************************************************************************************/

void PRS_ISTEN_AYRILMA_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE      ( tr ( "PRS - İŞTEN AYRILMA" ) );
    SET_HELP_PAGE       ("personel-bordrosu_isten-ayrilma-islemi");

    m_ui->adakDate_cikis_tarihi->MALI_YIL_KISITI_YOK( true );

    REGISTER_BUTTON_WIDGET(m_ui->widget_batch_buttons);

    SET_SETTING_NAME    ( "PRS_ISTEN_AYR_BATCH" );

    SET_NAME_OF_RUN_BATCH_BUTTON("İŞTEN AYRILMA RAPORU AL");

    PRS_FILL_COMBO_BOX_ISTEN_CIKMA_NEDENLERI ( m_ui->combo_box_ayrilma_nedeni );


    m_ui->adakDate_cikis_tarihi->SET_DATE( QDate::currentDate());
    m_ui->search_edit_sicil_no->SET_BUTTON_SIZE( 30, 50);
    m_ui->line_edit_personel_adi_soyadi->setReadOnly(true);

    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_cikis_tarihi );
}

/**************************************************************************************
                   PRS_ISTEN_AYRILMA_BATCH::CHECK_VAR
***************************************************************************************/

int PRS_ISTEN_AYRILMA_BATCH::CHECK_VAR(QObject * object)
{
    if ( object EQ m_ui->search_edit_sicil_no ) {

        if ( m_ui->search_edit_sicil_no->GET_TEXT().isEmpty() EQ true ) {
            m_ui->line_edit_personel_adi_soyadi->clear();
            m_personel_id = 0;
            return ADAK_OK;
        }

        SQL_QUERY       sql_query( DB );

        sql_query.PREPARE_SELECT( "prs_personeller , prs_personel_kisisel_blgler",
                                 "prs_personeller.personel_id, cari_hesap_id , "
                                 "prs_personeller.isten_ayrildi_mi ,"
                                 "ise_giris_tarihi", "sicil_no = :sicil_no AND "
                                 "prs_personeller.personel_id = prs_personel_kisisel_blgler.personel_id");

        sql_query.SET_VALUE( ":sicil_no"  , m_ui->search_edit_sicil_no->GET_TEXT()  );

        if ( sql_query.SELECT() EQ 0 ) {
            m_personel_id = 0;
            m_ui->search_edit_sicil_no->CLEAR();

            MSG_WARNING(tr("Aradığınız personel bulunamadı.") , m_ui->search_edit_sicil_no);
            return ADAK_FAIL;
        }

        sql_query.NEXT();

        m_personel_id        = sql_query.VALUE( 0 ).toInt();
        int cari_hesap_id    = sql_query.VALUE( 1 ).toInt();
        int isten_ayrildi_mi = sql_query.VALUE( 2 ).toInt();
        m_ise_giris_tarihi   = QDate::fromString ( sql_query.VALUE( 3 ).toString() , "yyyy.MM.dd");

        if ( isten_ayrildi_mi EQ 1 ) {
            m_personel_id = 0;
            m_ui->search_edit_sicil_no->CLEAR();

            MSG_WARNING(tr("Personel zaten işten ayrılmıştır." ) , m_ui->search_edit_sicil_no);
            return ADAK_FAIL;
        }

        QStringList demirbas_id_list = DMR_GET_ID_KODU_ADI_LIST ( m_personel_id );

        if ( demirbas_id_list.isEmpty() EQ false ) {
            m_ui->search_edit_sicil_no->CLEAR();
            m_personel_id = 0;

            MSG_WARNING( tr("Personele zimmetli demirbaşlar bulunmaktadır."
                            "İşten ayrılma işleminden önce lütfen demirbaş modülünden "
                            "personele ait zimmetleri kaldırınız."),m_ui->search_edit_sicil_no);
            return ADAK_FAIL;
        }

        CARI_HESAP_STRUCT hesap_bilgileri;
        CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

        CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri );

        m_ui->line_edit_personel_adi_soyadi->setText    ( hesap_bilgileri.cari_hesap_ismi );

    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_ISTEN_AYRILMA_BATCH::CHECK_RUN
***************************************************************************************/

int PRS_ISTEN_AYRILMA_BATCH::CHECK_RUN()
{
    if ( m_ui->search_edit_sicil_no->GET_TEXT().isEmpty()) {
        MSG_WARNING(  tr("Personelin Sicil Numarasini Giriniz !.."), m_ui->search_edit_sicil_no );
        return ADAK_FAIL;
    }
    if ( m_ui->combo_box_ayrilma_nedeni->currentIndex() EQ 0 ) {
        MSG_WARNING( tr( "İşten Ayrılma Nedenini Girmek Zorundasınız..." ), m_ui->combo_box_ayrilma_nedeni );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                  PRS_ISTEN_AYRILMA_BATCH::RUN_BATCH
**************************************************************************************/

void PRS_ISTEN_AYRILMA_BATCH::RUN_BATCH()
{

    QString         log_detaylari;
    double          brut_ihbar_tazminati_tutari          = 0;
    double          net_ihbar_tazminati_tutari           = 0;
    double          ihbar_tazminati_gelir_vergisi_tutari = 0;
    double          ihbar_tazminati_damga_vergisi_tutari = 0;
    double          brut_kidem_tazminati_tutari          = 0;
    double          net_kidem_tazminati_tutari           = 0;
    double          kidem_tazminati_damga_vergisi_tutari = 0;
    int             isten_ayrilan_id;
    int             rapor_gosterilsin_mi;

    PRS_CALISMA_SURESI calisma_suresi = PRS_CALISMA_SURESINI_HESAPLA ( m_ise_giris_tarihi, m_ui->adakDate_cikis_tarihi->QDATE() );

    if ( m_ui->check_box_ihbar_tazminati->isChecked() EQ true ) {

        net_ihbar_tazminati_tutari = PRS_IHBAR_TAZMINATI_TUTARINI_HESAPLA ( m_personel_id, calisma_suresi,
                                                                    &brut_ihbar_tazminati_tutari, &net_ihbar_tazminati_tutari,
                                                                    &ihbar_tazminati_gelir_vergisi_tutari, & ihbar_tazminati_damga_vergisi_tutari);
    }

    if ( m_ui->check_box_kidem_tazminati->isChecked() EQ true ) {

        if ( PRS_KIDEM_TAZMINATI_ALMAYA_HAK_KAZANMIS_MI ( calisma_suresi ) EQ ADAK_FAIL ) {

            MSG_WARNING( QObject::tr("Personel Henüz Kıdem Tazminatı Almaya Hak Kazanmamıştır! <br>"), NULL );

        }
        else {

            net_kidem_tazminati_tutari = PRS_KIDEM_TAZMINATI_TUTARINI_HESAPLA ( m_personel_id, calisma_suresi,
                                                                    &brut_kidem_tazminati_tutari, &net_kidem_tazminati_tutari,
                                                                    &kidem_tazminati_damga_vergisi_tutari );
        }
    }

    DB->START_TRANSACTION();
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_UPDATE("prs_personeller","personel_id",
                             "isten_ayrildi_mi",
                             "personel_id   = :personel_id ");

    sql_query.SET_VALUE( ":isten_ayrildi_mi"   , 1       );
    sql_query.SET_VALUE( ":personel_id"        , m_personel_id   );

    sql_query.UPDATE();

    sql_query.PREPARE_UPDATE("prs_personel_kisisel_blgler","kisisel_bilgi_id",
                             "isten_cikis_tarihi",
                             "personel_id   = :personel_id ");

    sql_query.SET_VALUE( ":isten_cikis_tarihi" , m_ui->adakDate_cikis_tarihi->DATE() );
    sql_query.SET_VALUE( ":personel_id"        , m_personel_id   );

    sql_query.UPDATE();


    sql_query.PREPARE_INSERT("prs_isten_ayrilanlar ","isten_ayrilan_id",
                             "personel_id, isten_ayrilma_tarihi, "
                             "isten_ayrilma_sekli_enum, isten_ayrilma_sekli_text, "
                             "calisma_suresi_yil, calisma_suresi_ay, calisma_suresi_gun, "
                             "ihbar_taz_var_mi, net_ihbar_taz_tutari, "
                             "brut_ihbar_taz_tutari, ihbar_taz_gelir_vergisi_tutari, "
                             "ihbar_taz_damga_vergisi_tutari, kidem_taz_var_mi, "
                             "net_kidem_taz_tutari, brut_kidem_taz_tutari, "
                             "kidem_taz_damga_vergisi_tutari ");


    sql_query.SET_VALUE( ":personel_id"                             , m_personel_id );
    sql_query.SET_VALUE( ":isten_ayrilma_tarihi"                    , m_ui->adakDate_cikis_tarihi->DATE() );
    sql_query.SET_VALUE( ":isten_ayrilma_sekli_enum"                , m_ui->combo_box_ayrilma_nedeni->currentIndex() );
    sql_query.SET_VALUE( ":isten_ayrilma_sekli_text"                , m_ui->combo_box_ayrilma_nedeni->currentText());
    sql_query.SET_VALUE( ":calisma_suresi_yil"                      , calisma_suresi.year );
    sql_query.SET_VALUE( ":calisma_suresi_ay"                       , calisma_suresi.month );
    sql_query.SET_VALUE( ":calisma_suresi_gun"                      , calisma_suresi.day );
    sql_query.SET_VALUE( ":ihbar_taz_var_mi"                        , QVariant (m_ui->check_box_ihbar_tazminati->isChecked()).toInt() );
    sql_query.SET_VALUE( ":net_ihbar_taz_tutari"                    , net_ihbar_tazminati_tutari );
    sql_query.SET_VALUE( ":brut_ihbar_taz_tutari"                   , brut_ihbar_tazminati_tutari );
    sql_query.SET_VALUE( ":ihbar_taz_gelir_vergisi_tutari"          , ihbar_tazminati_gelir_vergisi_tutari );
    sql_query.SET_VALUE( ":ihbar_taz_damga_vergisi_tutari"          , ihbar_tazminati_damga_vergisi_tutari  );
    sql_query.SET_VALUE( ":kidem_taz_var_mi"                        , QVariant (m_ui->check_box_kidem_tazminati->isChecked()).toInt());
    sql_query.SET_VALUE( ":net_kidem_taz_tutari"                    , net_kidem_tazminati_tutari  );
    sql_query.SET_VALUE( ":brut_kidem_taz_tutari"                   , brut_kidem_tazminati_tutari      );
    sql_query.SET_VALUE( ":kidem_taz_damga_vergisi_tutari"          , kidem_tazminati_damga_vergisi_tutari );

    isten_ayrilan_id = sql_query.INSERT();

    //  muhasebe ent fonksiyonu cagrilmali

    PRS_KIDEM_IHBAR_ENT_STRUCT M_STRUCT;

    PRS_CLEAR_KIDEM_IHBAR_ENT_STRUCT( &M_STRUCT );

    M_STRUCT.kidem_tazminati_tutari         = brut_kidem_tazminati_tutari;
    M_STRUCT.kidem_taz_damga_vergisi_tutari = kidem_tazminati_damga_vergisi_tutari;
    M_STRUCT.ihbar_tazminati_tutari         = brut_ihbar_tazminati_tutari;
    M_STRUCT.ihbar_taz_damga_vergisi_tutari = ihbar_tazminati_damga_vergisi_tutari;
    M_STRUCT.ihbar_taz_gelir_vergisi_tutari = ihbar_tazminati_gelir_vergisi_tutari;
    M_STRUCT.personel_id                    = m_personel_id;
    M_STRUCT.tarih                          = m_ui->adakDate_cikis_tarihi->DATE();
    M_STRUCT.program_id                     = E9_PROGRAMI;
    M_STRUCT.modul_id                       = PERSONEL_MODULU;
    M_STRUCT.kidem_ihbar_id                 = isten_ayrilan_id;

    PRS_KIDEM_IHBAR_TAZMINATI_ENTEGRASTONU ( &M_STRUCT );


    // burda personeli cari aramada gorunmesin diye irasetliyoruz

    CARI_HESAP_FILTRE_STR_UPDATE( PRS_GET_PERSONEL_CARI_HESAP_ID( m_personel_id ), "DELETED");

    QString alinacak_tazminatlar;

    if ( m_ui->check_box_ihbar_tazminati->isChecked() EQ true ) {
        alinacak_tazminatlar.append("İhbar Tazminatı ");
    }
    if ( m_ui->check_box_kidem_tazminati->isChecked() EQ true ) {
        if(m_ui->check_box_ihbar_tazminati->isChecked() EQ true ) {
            alinacak_tazminatlar.append(" , ");
        }
        alinacak_tazminatlar.append("Kıdem Tazminatı");
    }


    log_detaylari = "Personel ( " + m_ui->line_edit_personel_adi_soyadi->text().toUpper() +" ) işten ayrılmıştır.";
    log_detaylari.append("/nPersonel Sicil No : " + m_ui->search_edit_sicil_no->GET_TEXT() +
                         "\nAyrılma Nedeni : " + m_ui->combo_box_ayrilma_nedeni->currentText() +
                         "\nAlınacak Tazminatlar : " + alinacak_tazminatlar );

    E9_LOG_KAYDI_EKLE(PERSONEL_MODULU , LOG_PRS_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari);

    DB->COMMIT_TRANSACTION();

    rapor_gosterilsin_mi = MSG_YES_NO(
                           m_ui->line_edit_personel_adi_soyadi->text() +" " +
                           tr("İşten Ayrılmıştır !<br><br>" "Ibranameyi Görmek İster Misiniz?"),
                           NULL );

    if ( rapor_gosterilsin_mi EQ ADAK_YES OR rapor_gosterilsin_mi EQ ADAK_NO ) {

        OPEN_REPORT_SHOWER( SHOW_ISTEN_AYRILMA_RAPORU( m_personel_id, isten_ayrilan_id ),nativeParentWidget() );
    }
}

/**************************************************************************************
                   PRS_ISTEN_AYRILMA_BATCH::CHECK_EXIT
***************************************************************************************/

int PRS_ISTEN_AYRILMA_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}


/**************************************************************************************
                   PRS_ISTEN_AYRILMA_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void PRS_ISTEN_AYRILMA_BATCH::SEARCH_EDIT_CLICKED( QWidget  * widget , QLineEdit * lineEdit )
{
    Q_UNUSED ( widget );

    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA, this, 0, E9_PROGRAMI, PERSONEL_MODULU, "PERSONEL ARAMA", "DELETED" );

    if ( cari_hesap_id > 0 ) {

        SQL_QUERY       sql_query( DB );

        sql_query.PREPARE_SELECT("prs_personeller","personel_id , sicil_no ",
                                 "cari_hesap_id = :cari_hesap_id");

        sql_query.SET_VALUE( ":cari_hesap_id"   , cari_hesap_id );

        if ( sql_query.SELECT() NE 0 ) {
            sql_query.NEXT();
            m_personel_id = sql_query.VALUE(0).toInt();
            lineEdit->setText( sql_query.VALUE(1).toString());
        }
    }
    else {
        m_personel_id = 0;
    }
}
