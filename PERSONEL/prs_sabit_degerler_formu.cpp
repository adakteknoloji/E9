#include "prs_sabit_degerler_formu_class.h"
#include "prs_sabit_degerler_formu_open.h"
#include "e9_log.h"
 
#include "prs_con_utils.h"
#include "prs_gui_utils.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QDialog>
#include <QtGui>
#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "adak_std_utils.h"
#include "prs_ibraname_maddeleri_batch_open.h"

extern ADAK_SQL          * DB;
extern PRS_SABITLER       G_PRS_SABITLER;
extern PRS_SABITLER       G_PRS_DEFAULT_SABITLER;


/**************************************************************************************
                   OPEN_PRS_SABIT_DEGERLER_FORMU
***************************************************************************************/

void OPEN_PRS_SABIT_DEGERLER_FORMU( QWidget *parent )
{
    PRS_SABIT_DEGERLER_FORMU * F = new PRS_SABIT_DEGERLER_FORMU(parent);

    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::PRS_SABIT_DEGERLER_FORMU
**************************************************************************************/

PRS_SABIT_DEGERLER_FORMU:: PRS_SABIT_DEGERLER_FORMU(QWidget *parent) : FORM_KERNEL(parent), m_ui(new Ui:: PRS_SABIT_DEGERLER_FORMU)
{
    m_ui->setupUi    (this);
    START_FORM_KERNEL(this,DB);
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::SETUP_FORM
***************************************************************************************/

void  PRS_SABIT_DEGERLER_FORMU::SETUP_FORM()
{
    SET_HELP_PAGE       ( "personel-bordrosu_sabit-degerler" );
    SET_PAGE_TITLE      ( tr ( "PRS - SABİT DEĞER TANIMLARI FORMU" ) );

    SET_SETTING_NAME    ( "PRS_SABIT_DEGERLER_FORMU" );

    REGISTER_CHANGER_BUTTON( m_ui->push_button_return_defaults_odenek_ve_kesintiler );

    SET_SINGLE_RECORD_ID (1);

    REGISTER_BUTTONS_WIDGET (m_ui->navigation_buttons_widget);

    REGISTER_CHANGER_BUTTON (m_ui->push_button_return_defaults);
    REGISTER_SAVER_BUTTON   (m_ui->pushButton_ibraname_maddeleri);    

    m_ui->combo_box_gelir_vergisi_1->setCurrentIndex(1);
    m_ui->combo_box_gelir_vergisi_2->setCurrentIndex(1);
    m_ui->combo_box_gelir_vergisi_3->setCurrentIndex(1);
    m_ui->combo_box_gelir_vergisi_4->setCurrentIndex(1);
    m_ui->combo_box_gelir_vergisi_5->setCurrentIndex(1);


    PRS_FILL_SABIT_DEGERLER_STRUCT ();

    m_ui->commaEdit_gunluk_yemek_yardimi_yuzdesi->SET_PRECISION( 1 );
    m_ui->commaEdit_haftalik_calisilan_gun_sayisi->SET_PRECISION( 1 );

    m_ui->commaEdit_isci_tehlike_sinifi->setEnabled( false );
    m_ui->commaEdit_isveren_tehlike_sinifi->setEnabled( false );

    SET_FIRST_FOCUS_WIDGET ( m_ui->comma_edit_isszlk_sig_isci_yuzdesi );

    m_ui->comboBox_frm_tabi_sig_kolu->addItems(QStringList() << "SSK" << "Emekli Sandığı" << "Bağkur");
    m_ui->comboBox_muh_fis_hesaplari_birlestir->addItems(QStringList() << "Her Personel İçin Detaylar" << "Bordro Toplamları" );

    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_aile_yardimi_kimlere_verilir);
    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_cocuk_yardimi_kimlere_verilir);
    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_dogum_yardimi_kimlere_verilir);
    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_evlenme_yardimi_kimlere_verilir);
    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_gorev_harcirahi_kimlere_verilir);
    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_ihbar_tazminati_kimlere_verilir);
    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_kidem_tazminati_kimlere_verilir);
    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_olum_yardimi_kimlere_verilir);
    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_yakacak_yardimi_kimlere_verilir);
    PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(m_ui->combo_box_yemek_parasi_kimlere_verilir);

    QStringList tutar_yuzde_turu_items;

    tutar_yuzde_turu_items << QObject::tr("TUTAR") << QObject::tr("YÜZDE");

    m_ui->combo_box_aile_yardimi_yuzde_mi->addItems     ( tutar_yuzde_turu_items );
    m_ui->combo_box_cocuk_yardimi_yuzde_mi->addItems    ( tutar_yuzde_turu_items );
    m_ui->combo_box_dogum_yardimi_yuzde_mi->addItems    ( tutar_yuzde_turu_items );
    m_ui->combo_box_evlenme_yardimi_yuzde_mi->addItems  ( tutar_yuzde_turu_items );
    m_ui->combo_box_gorev_harcirahi_yuzde_mi->addItems  ( tutar_yuzde_turu_items );
    m_ui->combo_box_ihbar_tazminati_yuzde_mi->addItems  ( tutar_yuzde_turu_items );
    m_ui->combo_box_kidem_tazminati_yuzde_mi->addItems  ( tutar_yuzde_turu_items );
    m_ui->combo_box_olum_yardimi_yuzde_mi->addItems     ( tutar_yuzde_turu_items );
    m_ui->combo_box_yakacak_yardimi_yuzde_mi->addItems  ( tutar_yuzde_turu_items );
    m_ui->combo_box_yemek_parasi_yuzde_mi->addItems     ( tutar_yuzde_turu_items );
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void  PRS_SABIT_DEGERLER_FORMU::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::NEW_RECORD
**************************************************************************************/

void  PRS_SABIT_DEGERLER_FORMU::NEW_RECORD()
{
    m_ui->tab_widget->setCurrentIndex(0);
}
/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::GET_RECORD
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::GET_RECORD(int record_id)
{
    SQL_QUERY       sql_query( DB );

    double tehlike_sinifi_isci_yuzdesi      = 0.00;
    double tehlike_sinifi_isveren_yuzdesi   = 0.00;

    m_ui->tab_widget->setCurrentIndex(0);

    sql_query.PREPARE_SELECT("prs_sabit_degerler",
                             "issizlik_sig_isci_yuzdesi, issizlik_sig_isveren_yuzdesi, "
                             "sakatlik_ind_derece_1_tutari, sakatlik_ind_derece_2_tutari, sakatlik_ind_derece_3_tutari, "
                             "tehlike_sinifi, ssk_primi_isci_yuzdesi, ssk_primi_isveren_yuzdesi, ssk_taban_degeri, "
                             "ssk_tavan_degeri, ssk_16_yas_alti_asgari_ucret, ssk_16_yas_ustu_asgari_ucret, damga_vergisi, "
                             "glr_ver_dilim_bas_tutar_array,glr_ver_dilim_bts_tutar_array,gelir_vergisi_yuzdeleri_array , "
                             "evli_es_clsmyr_asggecind_array, "
                             "evli_es_clsyr_asggecind_array , bekar_asg_gec_indirimler_array , "
                             "ihbr_taz_bas_trh_turleri_array ,"
                             "ihbr_taz_bas_trh_orj_deg_array , ihbr_taz_bas_trh_ay_deg_array ,"
                             "ihbr_taz_bts_trh_turleri_array ,"
                             "ihbr_taz_bts_trh_orj_deg_array , ihbr_taz_bts_trh_ay_deg_array , "
                             "ihbar_taz_hafta_deg_array , frm_tabi_sigorta_kolu,"
                             "frm_sendika_uyesi_mi,sendika_aidati , "
                             "tehlike_sinifi_isci_yuzdesi, tehlike_sinifi_isveren_yuzdesi , "
                             "gecerli_glr_ver_dilimi_sayisi , ssk_isveren_taban_deger, ssk_isveren_tavan_deger ,"
                             "prs_sgdp_primi_isci_yuzdesi, prs_sgdp_primi_isveren_yuzdesi  , muh_fis_hesaplari_birlestir , "
                             "isy_5510_nolu_kanuna_tabi_mi, isy_5510_nolu_kanun_yuzdesi, haftalik_calisilan_gun_sayisi, "
                             "gunluk_yemek_yardimi_yuzdesi ",
                             "sabit_degerler_id = :sabit_degerler_id");

    sql_query.SET_VALUE( ":sabit_degerler_id",        record_id );

    if( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();


    int isy_5510_nolu_kanuna_tabi_mi                          = sql_query.VALUE( "isy_5510_nolu_kanuna_tabi_mi" ).toInt();

    m_ui->checkBox_isy_5510_nolu_kanuna_tabi_mi->setChecked( false );
    if ( isy_5510_nolu_kanuna_tabi_mi EQ 1 ) {
        m_ui->checkBox_isy_5510_nolu_kanuna_tabi_mi->setChecked( true );
    }

    m_ui->commaEdit_isy_5510_nolu_kanun_yuzdesi->SET_DOUBLE( sql_query.VALUE( "isy_5510_nolu_kanun_yuzdesi").toDouble() );

    int muh_fis_hesaplari_birlestir                           = sql_query.VALUE("muh_fis_hesaplari_birlestir").toInt();
    m_ui->comma_edit_isszlk_sig_isci_yuzdesi->SET_DOUBLE      ( sql_query.VALUE("issizlik_sig_isci_yuzdesi").toDouble() * 100);
    m_ui->comma_edit_isszlk_sig_isveren_yuzdesi->SET_DOUBLE   ( sql_query.VALUE("issizlik_sig_isveren_yuzdesi").toDouble() * 100);
    m_ui->comma_edit_sakatlik_indirimi_derece_1->SET_DOUBLE   ( sql_query.VALUE("sakatlik_ind_derece_1_tutari").toDouble());
    m_ui->comma_edit_sakatlik_indirimi_derece_2->SET_DOUBLE   ( sql_query.VALUE("sakatlik_ind_derece_2_tutari").toDouble());
    m_ui->comma_edit_sakatlik_indirimi_derece_3->SET_DOUBLE   ( sql_query.VALUE("sakatlik_ind_derece_3_tutari").toDouble());

    m_ui->combo_box_tehlike_sinifi->setCurrentIndex(
    m_ui->combo_box_tehlike_sinifi->findText                  ( sql_query.VALUE("tehlike_sinifi").toString()));
    m_ui->comma_edit_ssk_primi_isci_yuzdesi->SET_DOUBLE       ( sql_query.VALUE("ssk_primi_isci_yuzdesi").toDouble() * 100);
    m_ui->comma_edit_ssk_primi_isveren_yuzdesi->SET_DOUBLE    ( sql_query.VALUE("ssk_primi_isveren_yuzdesi").toDouble() * 100);
    m_ui->comma_edit_ssk_taban->SET_DOUBLE                    ( sql_query.VALUE("ssk_taban_degeri").toDouble());
    m_ui->comma_edit_ssk_tavan->SET_DOUBLE                    ( sql_query.VALUE("ssk_tavan_degeri").toDouble());
    m_ui->comma_edit_16_yas_alti_asg_ucret->SET_DOUBLE        ( sql_query.VALUE("ssk_16_yas_alti_asgari_ucret").toDouble());
    m_ui->comma_edit_16_yas_ustu_asg_ucret->SET_DOUBLE        ( sql_query.VALUE("ssk_16_yas_ustu_asgari_ucret").toDouble());
    m_ui->comma_edit_damga_vergisi_orani->SET_DOUBLE          ( sql_query.VALUE("damga_vergisi").toDouble() * 100);

    m_ui->comma_edit_ssk_isveren_taban->SET_DOUBLE            ( sql_query.VALUE("ssk_isveren_taban_deger").toDouble() );
    m_ui->comma_edit_ssk_isveren_tavan->SET_DOUBLE            ( sql_query.VALUE("ssk_isveren_tavan_deger").toDouble() );

    m_ui->commaEdit_prs_sgdp_isci_yuzdesi->SET_DOUBLE         ( sql_query.VALUE("prs_sgdp_primi_isci_yuzdesi").toDouble() * 100 );
    m_ui->commaEdit_prs_sgdp_isveren_yuzdesi->SET_DOUBLE      ( sql_query.VALUE("prs_sgdp_primi_isveren_yuzdesi").toDouble() * 100 );

    m_ui->commaEdit_haftalik_calisilan_gun_sayisi->SET_DOUBLE ( sql_query.VALUE("haftalik_calisilan_gun_sayisi").toDouble() );
    m_ui->commaEdit_gunluk_yemek_yardimi_yuzdesi->SET_DOUBLE  ( sql_query.VALUE("gunluk_yemek_yardimi_yuzdesi").toDouble() * 100 );

    m_ui->comboBox_muh_fis_hesaplari_birlestir->setCurrentIndex( muh_fis_hesaplari_birlestir ); // 0 ise personel_detaylari 1 ise toplamlar

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("glr_ver_dilim_bas_tutar_array").toString() , G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI,
                         GELIR_VERGISI_DILIMI_SAYISI);
    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("glr_ver_dilim_bts_tutar_array").toString() , G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI,
                         GELIR_VERGISI_DILIMI_SAYISI);
    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("gelir_vergisi_yuzdeleri_array").toString() , G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI,
                         GELIR_VERGISI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("evli_es_clsmyr_asggecind_array").toString() ,
                         G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI , MAX_COCUK_SAYISI);
    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("evli_es_clsyr_asggecind_array").toString()  ,
                         G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI  , MAX_COCUK_SAYISI);
    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("bekar_asg_gec_indirimler_array").toString()  , G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI ,
                         MAX_COCUK_SAYISI);

    UNPACK_INT_ARRAY    (sql_query.VALUE("ihbr_taz_bas_trh_turleri_array").toString() ,  M_IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI,
                         IHBAR_TAZMINATI_DILIMI_SAYISI);


    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbr_taz_bas_trh_orj_deg_array").toString() , M_IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI,
                         IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbr_taz_bas_trh_ay_deg_array").toString()  , M_IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI,
                         IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_INT_ARRAY    (sql_query.VALUE("ihbr_taz_bts_trh_turleri_array").toString() , M_IHBAR_TAZ_BITIS_TARIHI_TURLERI ,
                         IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbr_taz_bts_trh_orj_deg_array").toString() , M_IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI ,
                         IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbr_taz_bts_trh_ay_deg_array").toString() , M_IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI,
                         IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbar_taz_hafta_deg_array").toString() , M_IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI ,
                         IHBAR_TAZMINATI_DILIMI_SAYISI );

    tehlike_sinifi_isci_yuzdesi     = sql_query.VALUE("tehlike_sinifi_isci_yuzdesi").toDouble();

    tehlike_sinifi_isveren_yuzdesi  = sql_query.VALUE("tehlike_sinifi_isveren_yuzdesi").toDouble();

    m_ui->comboBox_frm_tabi_sig_kolu->setCurrentIndex(m_ui->comboBox_frm_tabi_sig_kolu->findText(PRS_GET_FIRMA_SIGORTA_KOLU_STRING(sql_query.VALUE("frm_tabi_sigorta_kolu").toInt())));

    m_ui->checkBox_sendika_uyesi_mi->setChecked(sql_query.VALUE("frm_sendika_uyesi_mi").toBool());
    m_ui->commaEdit_sendika_aidati->SET_DOUBLE(sql_query.VALUE("sendika_aidati").toDouble());

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_1->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[0]  );
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_1->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[0] );
    m_ui->comma_edit_gelir_vergisi_orani_1->SET_DOUBLE     ( G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[0] * 100 );

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_2->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[1]);
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_2->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[1] );
    m_ui->comma_edit_gelir_vergisi_orani_2->SET_DOUBLE     ( G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[1] * 100 );

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_3->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[2]);
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_3->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[2]);
    m_ui->comma_edit_gelir_vergisi_orani_3->SET_DOUBLE     ( G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[2] * 100 );

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_4->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[3]  );
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_4->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[3] );
    m_ui->comma_edit_gelir_vergisi_orani_4->SET_DOUBLE     ( G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[3] * 100 );

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_5->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[4]  );
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_5->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[4]);
    m_ui->comma_edit_gelir_vergisi_orani_5->SET_DOUBLE     ( G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[4] * 100 );



    m_ui->comma_edit_evli_es_calismiyor_cocuk_yok->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ] );
    m_ui->comma_edit_evli_es_calismiyor_1_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI  [ PRS_TEK_COCUK ] );
    m_ui->comma_edit_evli_es_calismiyor_2_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI  [ PRS_IKI_COCUK ] );
    m_ui->comma_edit_evli_es_calismiyor_3_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI  [ PRS_UC_COCUK  ] );
    m_ui->comma_edit_evli_es_calismiyor_4_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI  [ PRS_DORT_COCUK] );

    m_ui->comma_edit_evli_es_calisiyor_cocuk_yok->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI  [ PRS_COCUK_YOK ] );
    m_ui->comma_edit_evli_es_calisiyor_1_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI    [ PRS_TEK_COCUK ] );
    m_ui->comma_edit_evli_es_calisiyor_2_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI    [ PRS_IKI_COCUK ] );
    m_ui->comma_edit_evli_es_calisiyor_3_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI    [ PRS_UC_COCUK  ] );
    m_ui->comma_edit_evli_es_calisiyor_4_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI    [ PRS_DORT_COCUK] );

    m_ui->comma_edit_bekar_cocuk_yok->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_COCUK_YOK ] );
    m_ui->comma_edit_bekar_1_cocuk->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI   [ PRS_TEK_COCUK ] );
    m_ui->comma_edit_bekar_2_cocuk->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI   [ PRS_IKI_COCUK ] );
    m_ui->comma_edit_bekar_3_cocuk->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI   [ PRS_UC_COCUK  ] );
    m_ui->comma_edit_bekar_4_cocuk->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI   [ PRS_DORT_COCUK] );

    m_ui->commaEdit_isci_tehlike_sinifi->SET_DOUBLE   ( tehlike_sinifi_isci_yuzdesi   * 100 );
    m_ui->commaEdit_isveren_tehlike_sinifi->SET_DOUBLE( tehlike_sinifi_isveren_yuzdesi* 100 );

    for ( int i=0; i< IHBAR_TAZMINATI_DILIMI_SAYISI ; i++ ) {

        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_turu         = M_IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI[i] ;
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_orj_degeri   = M_IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI[i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_ay_degeri    = M_IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI[i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_turu             = M_IHBAR_TAZ_BITIS_TARIHI_TURLERI [i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_orj_degeri       = M_IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI[i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_ay_degeri        = M_IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI[i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].tazminat_hafta_degeri         = M_IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI[i];
    }

    m_ui->combo_box_ihbar_taz_bas_suresi_1->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].baslangic_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bas_suresi_2->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].baslangic_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bas_suresi_3->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].baslangic_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bas_suresi_4->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].baslangic_tarihi_turu );

    m_ui->comma_edit_ihbar_taz_bas_suresi_1->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].baslangic_tarihi_orj_degeri);
    m_ui->comma_edit_ihbar_taz_bas_suresi_2->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].baslangic_tarihi_orj_degeri);
    m_ui->comma_edit_ihbar_taz_bas_suresi_3->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].baslangic_tarihi_orj_degeri);
    m_ui->comma_edit_ihbar_taz_bas_suresi_4->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].baslangic_tarihi_orj_degeri);

    m_ui->combo_box_ihbar_taz_bit_suresi_1->setCurrentIndex  ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].bitis_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bit_suresi_2->setCurrentIndex  ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].bitis_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bit_suresi_3->setCurrentIndex  ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].bitis_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bit_suresi_4->setCurrentIndex  ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].bitis_tarihi_turu );

    m_ui->comma_edit_ihbar_taz_bit_suresi_1->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].bitis_tarihi_orj_degeri);
    m_ui->comma_edit_ihbar_taz_bit_suresi_2->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].bitis_tarihi_orj_degeri);
    m_ui->comma_edit_ihbar_taz_bit_suresi_3->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].bitis_tarihi_orj_degeri);
    m_ui->comma_edit_ihbar_taz_bit_suresi_4->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].bitis_tarihi_orj_degeri);

    m_ui->comma_edit_ihbar_taz_tutari_1->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].tazminat_hafta_degeri);
    m_ui->comma_edit_ihbar_taz_tutari_2->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].tazminat_hafta_degeri);
    m_ui->comma_edit_ihbar_taz_tutari_3->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].tazminat_hafta_degeri);
    m_ui->comma_edit_ihbar_taz_tutari_4->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].tazminat_hafta_degeri);

    m_sabit_degerler_id = record_id;

    /////////////////////////////////     SSK Bilgileri   /////////////////////////////////////////////

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_isszlk_sig_isci_yuzdesi , G_PRS_DEFAULT_SABITLER.issizlik_sig_isci_yuzdesi, PRS_YUZDE);

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_isszlk_sig_isveren_yuzdesi, G_PRS_DEFAULT_SABITLER.issizlik_sig_isveren_yuzdesi, PRS_YUZDE);

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_sakatlik_indirimi_derece_1, G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_1_tutari, PRS_TUTAR );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_sakatlik_indirimi_derece_2, G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_2_tutari, PRS_TUTAR );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_sakatlik_indirimi_derece_3, G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_3_tutari, PRS_TUTAR );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_ssk_taban, G_PRS_DEFAULT_SABITLER.ssk_taban_degeri, PRS_TUTAR );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_ssk_tavan, G_PRS_DEFAULT_SABITLER.ssk_tavan_degeri, PRS_TUTAR );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_16_yas_alti_asg_ucret, G_PRS_DEFAULT_SABITLER.ssk_16_yas_alti_asgari_ucret, PRS_TUTAR );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_16_yas_ustu_asg_ucret, G_PRS_DEFAULT_SABITLER.ssk_16_yas_ustu_asgari_ucret, PRS_TUTAR );

    ////////////////////////////////////      Vergi Bilgileri    ///////////////////////////////////////////////

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_1,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 0 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_2,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 1 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_3,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 2 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_4,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 3 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_5,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 4 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_1,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 0 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_2,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 1 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_3,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 2 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_4,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 3 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_5,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 4 ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_1,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 0 ], PRS_YUZDE );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_2,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 1 ], PRS_YUZDE  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_3,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 2 ], PRS_YUZDE  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_4,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 3 ], PRS_YUZDE  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_5,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 4 ], PRS_YUZDE  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_damga_vergisi_orani,  G_PRS_DEFAULT_SABITLER.damga_vergisi, PRS_YUZDE  );

    /////////////////////////////////     Asgari Gecim Indirimi    /////////////////////////////////////////////


    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_cocuk_yok,
                                           G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ], PRS_TUTAR   );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_1_cocuk,
                                           G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_TEK_COCUK ], PRS_TUTAR   );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_2_cocuk,
                                          G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_IKI_COCUK ], PRS_TUTAR   );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_3_cocuk,
                                           G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_UC_COCUK ], PRS_TUTAR   );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_4_cocuk,
                                           G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_DORT_COCUK ], PRS_TUTAR   );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_cocuk_yok,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_1_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_TEK_COCUK ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_2_cocuk,
                                          G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_IKI_COCUK ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_3_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_UC_COCUK ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_4_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_DORT_COCUK ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_cocuk_yok,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ], PRS_TUTAR  );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_1_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_TEK_COCUK ], PRS_TUTAR );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_2_cocuk,
                                          G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_IKI_COCUK ], PRS_TUTAR );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_3_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_UC_COCUK ], PRS_TUTAR );

    PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_4_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_DORT_COCUK ], PRS_TUTAR );


    SET_DEFAULT_ODENEK_VE_KESINTIKER();

    sql_query.PREPARE_SELECT("prs_firma_odenekleri",
                             "dogum_yardimi_var_mi_enum, dogum_yardimi_ssk_kesintisi, "
                             "dogum_yardimi_gelir_vergisi, dogum_yardimi_damga_vergisi, "
                             "dogum_yardimi_yuzde_mi, dogum_yardimi_yuzdesi, dogum_yardimi_tutari, "
                             "olum_yardimi_var_mi_enum, olum_yardimi_ssk_kesintisi, "
                             "olum_yardimi_gelir_vergisi, olum_yardimi_damga_vergisi, "
                             "olum_yardimi_yuzde_mi, olum_yardimi_yuzdesi, olum_yardimi_tutari, "
                             "cocuk_yardimi_var_mi_enum, cocuk_yardimi_ssk_kesintisi, "
                             "cocuk_yardimi_gelir_vergisi, cocuk_yardimi_damga_vergisi, "
                             "cocuk_yardimi_yuzde_mi, cocuk_yardimi_yuzdesi, cocuk_yardimi_tutari, "
                             "yakacak_yardimi_var_mi_enum, yakacak_yardimi_ssk_kesintisi, "
                             "yakacak_yardimi_gelir_vergisi, yakacak_yardimi_damga_vergisi, "
                             "yakacak_yardimi_yuzde_mi, yakacak_yardimi_yuzdesi, yakacak_yardimi_tutari, "
                             "aile_yardimi_var_mi_enum, aile_yardimi_ssk_kesintisi, "
                             "aile_yardimi_gelir_vergisi, aile_yardimi_damga_vergisi, "
                             "aile_yardimi_yuzdesi, aile_yardimi_tutari, yemek_parasi_var_mi_enum, "
                             "yemek_parasi_ssk_kesintisi, yemek_parasi_gelir_vergisi, "
                             "yemek_parasi_damga_vergisi, aile_yardimi_yuzde_mi,yemek_parasi_yuzde_mi, yemek_parasi_yuzdesi, "
                             "yemek_parasi_tutari, evlenme_yardimi_var_mi_enum, evlenme_yardimi_ssk_kesintisi, "
                             "evlenme_yardimi_gelir_vergisi, evlenme_yardimi_damga_vergisi, "
                             "evlenme_yardimi_yuzde_mi, evlenme_yardimi_yuzdesi, evlenme_yardimi_tutari, "
                             "gorev_harcirahi_var_mi_enum, gorev_harcirahi_ssk_kesintisi, "
                             "gorev_harcirahi_gelir_vergisi, gorev_harcirahi_damga_vergisi, "
                             "gorev_harcirahi_yuzde_mi, gorev_harcirahi_yuzdesi, gorev_harcirahi_tutari, "
                             "ihbar_tazminati_var_mi_enum, ihbar_tazminati_ssk_kesintisi, "
                             "ihbar_tazminati_gelir_vergisi, ihbar_tazminati_damga_vergisi, "
                             "ihbar_tazminati_yuzde_mi, ihbar_tazminati_yuzdesi, ihbar_tazminati_tutari, "
                             "kidem_tazminati_var_mi_enum, kidem_tazminati_ssk_kesintisi, "
                             "kidem_tazminati_gelir_vergisi, kidem_tazminati_damga_vergisi, "
                             "kidem_tazminati_yuzde_mi, kidem_tazminati_yuzdesi, kidem_tazminati_tutari ",
                             "odenekler_id = :odenekler_id");

    sql_query.SET_VALUE( ":odenekler_id",        record_id );

    if( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_aile_yardimi_kimlere_verilir ,
                                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(sql_query.VALUE("aile_yardimi_var_mi_enum").toInt()));

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_cocuk_yardimi_kimlere_verilir ,
                                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(sql_query.VALUE("cocuk_yardimi_var_mi_enum").toInt()));

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_dogum_yardimi_kimlere_verilir ,
                                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(sql_query.VALUE("dogum_yardimi_var_mi_enum").toInt()));

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_evlenme_yardimi_kimlere_verilir ,
                                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(sql_query.VALUE("evlenme_yardimi_var_mi_enum").toInt()));

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_gorev_harcirahi_kimlere_verilir ,
                                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(sql_query.VALUE("gorev_harcirahi_var_mi_enum").toInt()));

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_olum_yardimi_kimlere_verilir ,
                                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(sql_query.VALUE("olum_yardimi_var_mi_enum").toInt()));

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_yakacak_yardimi_kimlere_verilir ,
                                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(sql_query.VALUE("yakacak_yardimi_var_mi_enum").toInt()));

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_yemek_parasi_kimlere_verilir ,
                                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(sql_query.VALUE("yemek_parasi_var_mi_enum").toInt()));


        HIDE_OR_SHOW_GROUP_BOX  ( m_ui->combo_box_aile_yardimi_kimlere_verilir,       m_ui->group_box_aile_yardimi    );
        HIDE_OR_SHOW_GROUP_BOX  ( m_ui->combo_box_cocuk_yardimi_kimlere_verilir,      m_ui->group_box_cocuk_yardimi   );
        HIDE_OR_SHOW_GROUP_BOX  ( m_ui->combo_box_dogum_yardimi_kimlere_verilir,      m_ui->group_box_dogum_yardimi   );
        HIDE_OR_SHOW_GROUP_BOX  ( m_ui->combo_box_evlenme_yardimi_kimlere_verilir,    m_ui->group_box_evlenme_yardimi );
        HIDE_OR_SHOW_GROUP_BOX  ( m_ui->combo_box_gorev_harcirahi_kimlere_verilir,    m_ui->group_box_gorev_harcirahi );
        HIDE_OR_SHOW_GROUP_BOX  ( m_ui->combo_box_olum_yardimi_kimlere_verilir,       m_ui->group_box_olum_yardimi    );
        HIDE_OR_SHOW_GROUP_BOX  ( m_ui->combo_box_yakacak_yardimi_kimlere_verilir,    m_ui->group_box_yakacak_yardimi );
        HIDE_OR_SHOW_GROUP_BOX  ( m_ui->combo_box_yemek_parasi_kimlere_verilir,       m_ui->group_box_yemek_parasi    );

        m_ui->check_box_aile_yardimi_ssk_kesintisi->setChecked      ( sql_query.VALUE("aile_yardimi_ssk_kesintisi").toInt());
        m_ui->check_box_aile_yardimi_gelir_vergisi->setChecked      ( sql_query.VALUE("aile_yardimi_gelir_vergisi").toInt());
        m_ui->check_box_aile_yardimi_damga_vergisi->setChecked      ( sql_query.VALUE("aile_yardimi_damga_vergisi").toInt());

        m_ui->check_box_cocuk_yardimi_ssk_kesintisi->setChecked     ( sql_query.VALUE("cocuk_yardimi_ssk_kesintisi").toInt());
        m_ui->check_box_cocuk_yardimi_gelir_vergisi->setChecked     ( sql_query.VALUE("cocuk_yardimi_gelir_vergisi").toInt());
        m_ui->check_box_cocuk_yardimi_damga_vergisi->setChecked     ( sql_query.VALUE("cocuk_yardimi_damga_vergisi").toInt());

        m_ui->check_box_dogum_yardimi_ssk_kesintisi->setChecked     ( sql_query.VALUE("dogum_yardimi_ssk_kesintisi").toInt());
        m_ui->check_box_dogum_yardimi_gelir_vergisi->setChecked     ( sql_query.VALUE("dogum_yardimi_gelir_vergisi").toInt());
        m_ui->check_box_dogum_yardimi_damga_vergisi->setChecked     ( sql_query.VALUE("dogum_yardimi_damga_vergisi").toInt());

        m_ui->check_box_evlenme_yardimi_ssk_kesintisi->setChecked   ( sql_query.VALUE("evlenme_yardimi_ssk_kesintisi").toInt());
        m_ui->check_box_evlenme_yardimi_gelir_vergisi->setChecked   ( sql_query.VALUE("evlenme_yardimi_gelir_vergisi").toInt());
        m_ui->check_box_evlenme_yardimi_damga_vergisi->setChecked   ( sql_query.VALUE("evlenme_yardimi_damga_vergisi").toInt());

        m_ui->check_box_gorev_harcirahi_ssk_kesintisi->setChecked   ( sql_query.VALUE("gorev_harcirahi_ssk_kesintisi").toInt());
        m_ui->check_box_gorev_harcirahi_gelir_vergisi->setChecked   ( sql_query.VALUE("gorev_harcirahi_gelir_vergisi").toInt());
        m_ui->check_box_gorev_harcirahi_damga_vergisi->setChecked   ( sql_query.VALUE("gorev_harcirahi_damga_vergisi").toInt());


        m_ui->check_box_olum_yardimi_ssk_kesintisi->setChecked      ( sql_query.VALUE("olum_yardimi_ssk_kesintisi").toInt());
        m_ui->check_box_olum_yardimi_gelir_vergisi->setChecked      ( sql_query.VALUE("olum_yardimi_gelir_vergisi").toInt());
        m_ui->check_box_olum_yardimi_damga_vergisi->setChecked      ( sql_query.VALUE("olum_yardimi_damga_vergisi").toInt());

        m_ui->check_box_yakacak_yardimi_ssk_kesintisi->setChecked   ( sql_query.VALUE("yakacak_yardimi_ssk_kesintisi").toInt());
        m_ui->check_box_yakacak_yardimi_gelir_vergisi->setChecked   ( sql_query.VALUE("yakacak_yardimi_gelir_vergisi").toInt());
        m_ui->check_box_yakacak_yardimi_damga_vergisi->setChecked   ( sql_query.VALUE("yakacak_yardimi_damga_vergisi").toInt());

        m_ui->check_box_yemek_parasi_ssk_kesintisi->setChecked      ( sql_query.VALUE("yemek_parasi_ssk_kesintisi").toInt());
        m_ui->check_box_yemek_parasi_gelir_vergisi->setChecked      ( sql_query.VALUE("yemek_parasi_gelir_vergisi").toInt());
        m_ui->check_box_yemek_parasi_damga_vergisi->setChecked      ( sql_query.VALUE("yemek_parasi_damga_vergisi").toInt());


        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_aile_yardimi_yuzde_mi , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(sql_query.VALUE("aile_yardimi_yuzde_mi").toInt()));
        m_ui->comma_edit_aile_yardimi_tutari->SET_DOUBLE            ( sql_query.VALUE("aile_yardimi_tutari").toDouble());
        m_ui->comma_edit_aile_yardimi_yuzdesi->SET_DOUBLE           ( sql_query.VALUE("aile_yardimi_yuzdesi").toDouble() * 100);

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_cocuk_yardimi_yuzde_mi , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(sql_query.VALUE("cocuk_yardimi_yuzde_mi").toInt()));
        m_ui->comma_edit_cocuk_yardimi_tutari->SET_DOUBLE           ( sql_query.VALUE("cocuk_yardimi_tutari").toDouble());
        m_ui->comma_edit_cocuk_yardimi_yuzdesi->SET_DOUBLE          ( sql_query.VALUE("cocuk_yardimi_yuzdesi").toDouble() * 100);

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_dogum_yardimi_yuzde_mi , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(sql_query.VALUE("dogum_yardimi_yuzde_mi").toInt()));
        m_ui->comma_edit_dogum_yardimi_tutari->SET_DOUBLE           ( sql_query.VALUE("dogum_yardimi_tutari").toDouble());
        m_ui->comma_edit_dogum_yardimi_yuzdesi->SET_DOUBLE          ( sql_query.VALUE("dogum_yardimi_yuzdesi").toDouble() * 100);

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_evlenme_yardimi_yuzde_mi , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(sql_query.VALUE("evlenme_yardimi_yuzde_mi").toInt()));
        m_ui->comma_edit_evlenme_yardimi_tutari->SET_DOUBLE         ( sql_query.VALUE("evlenme_yardimi_tutari").toDouble());
        m_ui->comma_edit_evlenme_yardimi_yuzdesi->SET_DOUBLE        ( sql_query.VALUE("evlenme_yardimi_yuzdesi").toDouble() * 100);

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_gorev_harcirahi_yuzde_mi , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(sql_query.VALUE("gorev_harcirahi_yuzde_mi").toInt()));
        m_ui->comma_edit_gorev_harcirahi_tutari->SET_DOUBLE         ( sql_query.VALUE("gorev_harcirahi_tutari").toDouble());
        m_ui->comma_edit_gorev_harcirahi_yuzdesi->SET_DOUBLE        ( sql_query.VALUE("gorev_harcirahi_yuzdesi").toDouble() * 100);

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_olum_yardimi_yuzde_mi , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(sql_query.VALUE("olum_yardimi_yuzde_mi").toInt()));
        m_ui->comma_edit_olum_yardimi_tutari->SET_DOUBLE            ( sql_query.VALUE("olum_yardimi_tutari").toDouble());
        m_ui->comma_edit_olum_yardimi_yuzdesi->SET_DOUBLE           ( sql_query.VALUE("olum_yardimi_yuzdesi").toDouble() * 100);

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_yakacak_yardimi_yuzde_mi , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(sql_query.VALUE("yakacak_yardimi_yuzde_mi").toInt()));
        m_ui->comma_edit_yakacak_yardimi_tutari->SET_DOUBLE         ( sql_query.VALUE("yakacak_yardimi_tutari").toDouble());
        m_ui->comma_edit_yakacak_yardimi_yuzdesi->SET_DOUBLE        ( sql_query.VALUE("yakacak_yardimi_yuzdesi").toDouble() * 100);

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_yemek_parasi_yuzde_mi , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(sql_query.VALUE("yemek_parasi_yuzde_mi").toInt()));
        m_ui->comma_edit_yemek_parasi_tutari->SET_DOUBLE            ( sql_query.VALUE("yemek_parasi_tutari").toDouble());
        m_ui->comma_edit_yemek_parasi_yuzdesi->SET_DOUBLE           ( sql_query.VALUE("yemek_parasi_yuzdesi").toDouble() * 100);

    }


    return ADAK_OK;
}


/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::CHECK_VAR
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::CHECK_VAR (QObject * object)
{
    HIGHLIGHT_NON_DEFAULT_VALUES ( object );

    if ( object EQ m_ui->combo_box_tehlike_sinifi ) {

        int secilen_tehlike =  m_ui->combo_box_tehlike_sinifi->currentText().toInt(); // dizi 0 baslar

        m_ui->comma_edit_ssk_primi_isveren_yuzdesi->SET_DOUBLE( ( m_ui->commaEdit_isveren_tehlike_sinifi->GET_DOUBLE() + (  ( secilen_tehlike + 1 ) / 2.0 ) ) );
        m_ui->comma_edit_ssk_primi_isci_yuzdesi->SET_DOUBLE( m_ui->commaEdit_isci_tehlike_sinifi->GET_DOUBLE() );

       // TEHLIKE_SINIFINA_GORE_SSK_PRIMI_BELIRLE();
    }
    else  if ( object EQ   m_ui->combo_box_gelir_vergisi_1 ) {
        GELIR_VERGISI_DILIMI_ARALIKLARINI_BELIRLE ( m_ui->combo_box_gelir_vergisi_1->currentIndex(),
                                                    m_ui->label_gelir_vergisi_1,
                                                    m_ui->comma_edit_gelir_vergisi_dilimi_bit_1 );
    }
    else  if ( object EQ   m_ui->combo_box_gelir_vergisi_2 ) {
        GELIR_VERGISI_DILIMI_ARALIKLARINI_BELIRLE ( m_ui->combo_box_gelir_vergisi_2->currentIndex(),
                                            m_ui->label_gelir_vergisi_2, m_ui->comma_edit_gelir_vergisi_dilimi_bit_2 );
    }
    else  if ( object EQ   m_ui->combo_box_gelir_vergisi_3 ) {
        GELIR_VERGISI_DILIMI_ARALIKLARINI_BELIRLE ( m_ui->combo_box_gelir_vergisi_3->currentIndex(),
                                            m_ui->label_gelir_vergisi_3, m_ui->comma_edit_gelir_vergisi_dilimi_bit_3 );
    }
    else  if ( object EQ   m_ui->combo_box_gelir_vergisi_4 ) {
        GELIR_VERGISI_DILIMI_ARALIKLARINI_BELIRLE ( m_ui->combo_box_gelir_vergisi_4->currentIndex(),
                                            m_ui->label_gelir_vergisi_4, m_ui->comma_edit_gelir_vergisi_dilimi_bit_4 );
    }
    else  if ( object EQ   m_ui->combo_box_gelir_vergisi_5 ) {
        GELIR_VERGISI_DILIMI_ARALIKLARINI_BELIRLE ( m_ui->combo_box_gelir_vergisi_5->currentIndex(),
                                            m_ui->label_gelir_vergisi_5, m_ui->comma_edit_gelir_vergisi_dilimi_bit_5 );
    }
    else if ( object EQ m_ui->commaEdit_isci_tehlike_sinifi ) {
        G_PRS_SABITLER.tehlike_sinifi_isci_yuzdesi = m_ui->commaEdit_isci_tehlike_sinifi->GET_DOUBLE();
    }

    else if ( object EQ m_ui->commaEdit_isveren_tehlike_sinifi ) {
        G_PRS_SABITLER.tehlike_sinifi_isveren_yuzdesi = m_ui->commaEdit_isveren_tehlike_sinifi->GET_DOUBLE();
    }

    if ( object  EQ m_ui->combo_box_aile_yardimi_kimlere_verilir) {

        ODENEK_KALDIRILSIN_MI ( m_ui->combo_box_aile_yardimi_kimlere_verilir );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_aile_yardimi_kimlere_verilir, m_ui->group_box_aile_yardimi );
    }
    else if ( object EQ m_ui->combo_box_cocuk_yardimi_kimlere_verilir) {

        ODENEK_KALDIRILSIN_MI ( m_ui->combo_box_cocuk_yardimi_kimlere_verilir );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_cocuk_yardimi_kimlere_verilir, m_ui->group_box_cocuk_yardimi );
    }
    else if ( object EQ m_ui->combo_box_dogum_yardimi_kimlere_verilir) {

        ODENEK_KALDIRILSIN_MI ( m_ui->combo_box_dogum_yardimi_kimlere_verilir );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_dogum_yardimi_kimlere_verilir, m_ui->group_box_dogum_yardimi );
    }
    else if ( object EQ m_ui->combo_box_evlenme_yardimi_kimlere_verilir) {

        ODENEK_KALDIRILSIN_MI ( m_ui->combo_box_evlenme_yardimi_kimlere_verilir );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_evlenme_yardimi_kimlere_verilir, m_ui->group_box_evlenme_yardimi );
    }
    else if ( object EQ m_ui->combo_box_gorev_harcirahi_kimlere_verilir) {

        ODENEK_KALDIRILSIN_MI ( m_ui->combo_box_gorev_harcirahi_kimlere_verilir );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_gorev_harcirahi_kimlere_verilir, m_ui->group_box_gorev_harcirahi );
    }
    else if ( object EQ m_ui->combo_box_olum_yardimi_kimlere_verilir) {

        ODENEK_KALDIRILSIN_MI ( m_ui->combo_box_olum_yardimi_kimlere_verilir );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_olum_yardimi_kimlere_verilir, m_ui->group_box_olum_yardimi );
    }
    else if ( object EQ m_ui->combo_box_yakacak_yardimi_kimlere_verilir) {

        ODENEK_KALDIRILSIN_MI ( m_ui->combo_box_yakacak_yardimi_kimlere_verilir );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_yakacak_yardimi_kimlere_verilir, m_ui->group_box_yakacak_yardimi );
    }
    else if ( object EQ m_ui->combo_box_yemek_parasi_kimlere_verilir) {

        ODENEK_KALDIRILSIN_MI ( m_ui->combo_box_yemek_parasi_kimlere_verilir );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_yemek_parasi_kimlere_verilir, m_ui->group_box_yemek_parasi );
    }

    else if ( object EQ m_ui->combo_box_aile_yardimi_yuzde_mi ) {

        if ( m_ui->combo_box_aile_yardimi_yuzde_mi->currentIndex() EQ PRS_YUZDE ) {

            m_ui->comma_edit_aile_yardimi_tutari->clear();
        }
        else if ( m_ui->combo_box_aile_yardimi_yuzde_mi->currentIndex() EQ PRS_TUTAR ){

            m_ui->comma_edit_aile_yardimi_yuzdesi->clear();
        }
    }

    else if ( object EQ m_ui->combo_box_cocuk_yardimi_yuzde_mi ) {

        if ( m_ui->combo_box_cocuk_yardimi_yuzde_mi->currentIndex() EQ PRS_YUZDE ) {

            m_ui->comma_edit_cocuk_yardimi_tutari->clear();
        }
        else if ( m_ui->combo_box_cocuk_yardimi_yuzde_mi->currentIndex() EQ PRS_TUTAR ){

            m_ui->comma_edit_cocuk_yardimi_yuzdesi->clear();
        }
    }

    else if ( object EQ m_ui->combo_box_dogum_yardimi_yuzde_mi ) {

        if ( m_ui->combo_box_dogum_yardimi_yuzde_mi->currentIndex() EQ PRS_YUZDE ) {

            m_ui->comma_edit_dogum_yardimi_tutari->clear();
        }
        else if ( m_ui->combo_box_dogum_yardimi_yuzde_mi->currentIndex() EQ PRS_TUTAR ){

            m_ui->comma_edit_dogum_yardimi_yuzdesi->clear();
        }
    }

    else if ( object EQ m_ui->combo_box_evlenme_yardimi_yuzde_mi ) {

        if ( m_ui->combo_box_evlenme_yardimi_yuzde_mi->currentIndex() EQ PRS_YUZDE ) {

            m_ui->comma_edit_evlenme_yardimi_tutari->clear();
        }
        else if ( m_ui->combo_box_evlenme_yardimi_yuzde_mi->currentIndex() EQ PRS_TUTAR ){

            m_ui->comma_edit_evlenme_yardimi_yuzdesi->clear();
        }
    }

    else if ( object EQ m_ui->combo_box_gorev_harcirahi_yuzde_mi ) {

        if ( m_ui->combo_box_gorev_harcirahi_yuzde_mi->currentIndex() EQ PRS_YUZDE ) {

            m_ui->comma_edit_gorev_harcirahi_tutari->clear();
        }
        else if ( m_ui->combo_box_gorev_harcirahi_yuzde_mi->currentIndex() EQ PRS_TUTAR ){

            m_ui->comma_edit_gorev_harcirahi_yuzdesi->clear();
        }
    }

    else if ( object EQ m_ui->combo_box_olum_yardimi_yuzde_mi ) {

        if ( m_ui->combo_box_olum_yardimi_yuzde_mi->currentIndex() EQ PRS_YUZDE ) {

            m_ui->comma_edit_olum_yardimi_tutari->clear();
        }
        else if ( m_ui->combo_box_olum_yardimi_yuzde_mi->currentIndex() EQ PRS_TUTAR ){

            m_ui->comma_edit_olum_yardimi_yuzdesi->clear();
        }
    }

    else if ( object EQ m_ui->combo_box_yakacak_yardimi_yuzde_mi ) {

        if ( m_ui->combo_box_yakacak_yardimi_yuzde_mi->currentIndex() EQ PRS_YUZDE ) {

            m_ui->comma_edit_yakacak_yardimi_tutari->clear();
        }
        else if ( m_ui->combo_box_yakacak_yardimi_yuzde_mi->currentIndex() EQ PRS_TUTAR ){

            m_ui->comma_edit_yakacak_yardimi_yuzdesi->clear();
        }
    }

    else if ( object EQ m_ui->combo_box_yemek_parasi_yuzde_mi ) {

        if ( m_ui->combo_box_yemek_parasi_yuzde_mi->currentIndex() EQ PRS_YUZDE ) {

            m_ui->comma_edit_yemek_parasi_tutari->clear();
        }
        else if ( m_ui->combo_box_yemek_parasi_yuzde_mi->currentIndex() EQ PRS_TUTAR ){

            m_ui->comma_edit_yemek_parasi_yuzdesi->clear();
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::CHECK_EMPTY
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::CHECK_EMPTY ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::CHECK_ADD
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::ADD_RECORD
**************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::ADD_RECORD ()
{
    return 1;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::CHECK_UPDATE
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::CHECK_UPDATE (int record_id)
{
    Q_UNUSED(record_id);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::UPDATE_RECORD
**************************************************************************************/


void PRS_SABIT_DEGERLER_FORMU::UPDATE_RECORD (int record_id)
{
    SQL_QUERY       sql_query( DB );


    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[0]               =  m_ui->comma_edit_gelir_vergisi_dilimi_bas_1->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[0]                   =  m_ui->comma_edit_gelir_vergisi_dilimi_bit_1->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[0]                                   =  m_ui->comma_edit_gelir_vergisi_orani_1->GET_DOUBLE() / 100;
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[1]               =  m_ui->comma_edit_gelir_vergisi_dilimi_bas_2->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[1]                   =  m_ui->comma_edit_gelir_vergisi_dilimi_bit_2->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[1]                                   =  m_ui->comma_edit_gelir_vergisi_orani_2->GET_DOUBLE() / 100;
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[2]               =  m_ui->comma_edit_gelir_vergisi_dilimi_bas_3->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[2]                   =  m_ui->comma_edit_gelir_vergisi_dilimi_bit_3->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[2]                                   =  m_ui->comma_edit_gelir_vergisi_orani_3->GET_DOUBLE() / 100;
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[3]               =  m_ui->comma_edit_gelir_vergisi_dilimi_bas_4->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[3]                   =  m_ui->comma_edit_gelir_vergisi_dilimi_bit_4->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[3]                                   =  m_ui->comma_edit_gelir_vergisi_orani_4->GET_DOUBLE() / 100;
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[4]               =  m_ui->comma_edit_gelir_vergisi_dilimi_bas_5->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[4]                   =  m_ui->comma_edit_gelir_vergisi_dilimi_bit_5->GET_DOUBLE();
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[4]                                   =  m_ui->comma_edit_gelir_vergisi_orani_5->GET_DOUBLE() / 100;

    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ]      =  m_ui->comma_edit_evli_es_calismiyor_cocuk_yok->GET_DOUBLE();
    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_TEK_COCUK ]      =  m_ui->comma_edit_evli_es_calismiyor_1_cocuk->GET_DOUBLE();
    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_IKI_COCUK ]      =  m_ui->comma_edit_evli_es_calismiyor_2_cocuk->GET_DOUBLE();
    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_UC_COCUK  ]      =  m_ui->comma_edit_evli_es_calismiyor_3_cocuk->GET_DOUBLE();
    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_DORT_COCUK]      =  m_ui->comma_edit_evli_es_calismiyor_4_cocuk->GET_DOUBLE();

    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ]       =  m_ui->comma_edit_evli_es_calisiyor_cocuk_yok->GET_DOUBLE();
    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_TEK_COCUK ]       =  m_ui->comma_edit_evli_es_calisiyor_1_cocuk->GET_DOUBLE();
    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_IKI_COCUK ]       =  m_ui->comma_edit_evli_es_calisiyor_2_cocuk->GET_DOUBLE();
    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_UC_COCUK  ]       =  m_ui->comma_edit_evli_es_calisiyor_3_cocuk->GET_DOUBLE();
    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_DORT_COCUK]       =  m_ui->comma_edit_evli_es_calisiyor_4_cocuk->GET_DOUBLE();

    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ]                   =  m_ui->comma_edit_bekar_cocuk_yok->GET_DOUBLE();
    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_TEK_COCUK ]                   =  m_ui->comma_edit_bekar_1_cocuk->GET_DOUBLE();
    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_IKI_COCUK ]                   =  m_ui->comma_edit_bekar_2_cocuk->GET_DOUBLE();
    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_UC_COCUK  ]                   =  m_ui->comma_edit_bekar_3_cocuk->GET_DOUBLE();
    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_DORT_COCUK]                   =  m_ui->comma_edit_bekar_4_cocuk->GET_DOUBLE();


    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].baslangic_tarihi_turu         =  m_ui->combo_box_ihbar_taz_bas_suresi_1->currentIndex();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].baslangic_tarihi_turu         =  m_ui->combo_box_ihbar_taz_bas_suresi_2->currentIndex();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].baslangic_tarihi_turu         =  m_ui->combo_box_ihbar_taz_bas_suresi_3->currentIndex();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].baslangic_tarihi_turu         =  m_ui->combo_box_ihbar_taz_bas_suresi_4->currentIndex();

    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].baslangic_tarihi_orj_degeri   =  m_ui->comma_edit_ihbar_taz_bas_suresi_1->GET_DOUBLE();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].baslangic_tarihi_orj_degeri   =  m_ui->comma_edit_ihbar_taz_bas_suresi_2->GET_DOUBLE();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].baslangic_tarihi_orj_degeri   =  m_ui->comma_edit_ihbar_taz_bas_suresi_3->GET_DOUBLE();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].baslangic_tarihi_orj_degeri   =  m_ui->comma_edit_ihbar_taz_bas_suresi_4->GET_DOUBLE();

    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].baslangic_tarihi_ay_degeri    =  PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR ( m_ui->combo_box_ihbar_taz_bas_suresi_1->currentIndex(), m_ui->comma_edit_ihbar_taz_bas_suresi_1->GET_DOUBLE() );
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].baslangic_tarihi_ay_degeri    =  PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR ( m_ui->combo_box_ihbar_taz_bas_suresi_2->currentIndex(), m_ui->comma_edit_ihbar_taz_bas_suresi_2->GET_DOUBLE() );
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].baslangic_tarihi_ay_degeri    =  PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR ( m_ui->combo_box_ihbar_taz_bas_suresi_3->currentIndex(), m_ui->comma_edit_ihbar_taz_bas_suresi_3->GET_DOUBLE() );
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].baslangic_tarihi_ay_degeri    =  PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR ( m_ui->combo_box_ihbar_taz_bas_suresi_4->currentIndex(), m_ui->comma_edit_ihbar_taz_bas_suresi_4->GET_DOUBLE() );

    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].bitis_tarihi_turu             =  m_ui->combo_box_ihbar_taz_bit_suresi_1->currentIndex();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].bitis_tarihi_turu             =  m_ui->combo_box_ihbar_taz_bit_suresi_2->currentIndex();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].bitis_tarihi_turu             =  m_ui->combo_box_ihbar_taz_bit_suresi_3->currentIndex();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].bitis_tarihi_turu             =  m_ui->combo_box_ihbar_taz_bit_suresi_4->currentIndex();

    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].bitis_tarihi_orj_degeri       =  m_ui->comma_edit_ihbar_taz_bit_suresi_1->GET_DOUBLE();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].bitis_tarihi_orj_degeri       =  m_ui->comma_edit_ihbar_taz_bit_suresi_2->GET_DOUBLE();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].bitis_tarihi_orj_degeri       =  m_ui->comma_edit_ihbar_taz_bit_suresi_3->GET_DOUBLE();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].bitis_tarihi_orj_degeri       =  m_ui->comma_edit_ihbar_taz_bit_suresi_4->GET_DOUBLE();

    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].bitis_tarihi_ay_degeri        =  PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR ( m_ui->combo_box_ihbar_taz_bit_suresi_1->currentIndex(), m_ui->comma_edit_ihbar_taz_bit_suresi_1->GET_DOUBLE() );
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].bitis_tarihi_ay_degeri        =  PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR ( m_ui->combo_box_ihbar_taz_bit_suresi_2->currentIndex(), m_ui->comma_edit_ihbar_taz_bit_suresi_2->GET_DOUBLE());
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].bitis_tarihi_ay_degeri        =  PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR ( m_ui->combo_box_ihbar_taz_bit_suresi_3->currentIndex(), m_ui->comma_edit_ihbar_taz_bit_suresi_3->GET_DOUBLE() );
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].bitis_tarihi_ay_degeri        =  PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR ( m_ui->combo_box_ihbar_taz_bit_suresi_4->currentIndex(), m_ui->comma_edit_ihbar_taz_bit_suresi_4->GET_DOUBLE() );

    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].tazminat_hafta_degeri         = m_ui->comma_edit_ihbar_taz_tutari_1->GET_DOUBLE();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].tazminat_hafta_degeri         = m_ui->comma_edit_ihbar_taz_tutari_2->GET_DOUBLE();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].tazminat_hafta_degeri         = m_ui->comma_edit_ihbar_taz_tutari_3->GET_DOUBLE();
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].tazminat_hafta_degeri         = m_ui->comma_edit_ihbar_taz_tutari_4->GET_DOUBLE();


    for ( int i=0; i< IHBAR_TAZMINATI_DILIMI_SAYISI ; i++ ) {

        M_IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI        [i] = G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_turu;
        M_IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI  [i] = G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_orj_degeri;
        M_IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI   [i] = G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_ay_degeri;
        M_IHBAR_TAZ_BITIS_TARIHI_TURLERI            [i] = G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_turu;
        M_IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI      [i] = G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_orj_degeri;
        M_IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI       [i] = G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_ay_degeri;
        M_IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI        [i] = G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].tazminat_hafta_degeri;
    }

    G_PRS_SABITLER.tehlike_sinifi_isci_yuzdesi       = m_ui->commaEdit_isci_tehlike_sinifi->GET_DOUBLE() / 100.0;
    G_PRS_SABITLER.tehlike_sinifi_isveren_yuzdesi    = m_ui->commaEdit_isveren_tehlike_sinifi->GET_DOUBLE() / 100.0;

    G_PRS_SABITLER.prs_sgdp_primi_isci_yuzdesi       = m_ui->commaEdit_prs_sgdp_isci_yuzdesi->GET_DOUBLE();
    G_PRS_SABITLER.prs_sgdp_primi_isveren_yuzdesi    = m_ui->commaEdit_prs_sgdp_isveren_yuzdesi->GET_DOUBLE();
    G_PRS_SABITLER.muh_fis_hesaplari_birlestir       = PRS_GET_BORDRO_MUH_HESAPLARI_BIRLESTIR_ENUM( m_ui->comboBox_muh_fis_hesaplari_birlestir->currentText());

    G_PRS_SABITLER.haftalik_calisilan_gun_sayisi     = m_ui->commaEdit_haftalik_calisilan_gun_sayisi->GET_DOUBLE();
    G_PRS_SABITLER.gunluk_yemek_yardimi_yuzdesi      = m_ui->commaEdit_gunluk_yemek_yardimi_yuzdesi->GET_DOUBLE() / 100.0;

    sql_query.PREPARE_UPDATE("prs_sabit_degerler","sabit_degerler_id",
                             "issizlik_sig_isci_yuzdesi                 , "
                             "issizlik_sig_isveren_yuzdesi              , "
                             "sakatlik_ind_derece_1_tutari              , "
                             "sakatlik_ind_derece_2_tutari              , "
                             "sakatlik_ind_derece_3_tutari              , "
                             "tehlike_sinifi                            , "
                             "ssk_primi_isci_yuzdesi                    , "
                             "ssk_primi_isveren_yuzdesi                 , "
                             "ssk_taban_degeri                          , "
                             "ssk_tavan_degeri                          , "
                             "ssk_16_yas_alti_asgari_ucret              , "
                             "ssk_16_yas_ustu_asgari_ucret              , "
                             "damga_vergisi                             , "
                             "gecerli_glr_ver_dilimi_sayisi             , "
                             "glr_ver_dilim_bas_tutar_array             , "
                             "glr_ver_dilim_bts_tutar_array             , "
                             "gelir_vergisi_yuzdeleri_array             , "
                             "evli_es_clsmyr_asggecind_array            , "
                             "evli_es_clsyr_asggecind_array             , "
                             "bekar_asg_gec_indirimler_array            , "
                             "ihbr_taz_bas_trh_turleri_array            , "
                             "ihbr_taz_bas_trh_orj_deg_array            , "
                             "ihbr_taz_bas_trh_ay_deg_array             , "
                             "ihbr_taz_bts_trh_turleri_array            , "
                             "ihbr_taz_bts_trh_orj_deg_array            , "
                             "ihbr_taz_bts_trh_ay_deg_array             , "
                             "ihbar_taz_hafta_deg_array                 , "
                             "frm_tabi_sigorta_kolu                     , "
                             "frm_sendika_uyesi_mi                      , "
                             "sendika_aidati                            , "
                             "tehlike_sinifi_isci_yuzdesi               , "
                             "tehlike_sinifi_isveren_yuzdesi            , "
                             "ssk_isveren_taban_deger                   , "
                             "ssk_isveren_tavan_deger                   , "
                             "prs_sgdp_primi_isci_yuzdesi               , "
                             "prs_sgdp_primi_isveren_yuzdesi            , "
                             "muh_fis_hesaplari_birlestir               , "
                             "isy_5510_nolu_kanuna_tabi_mi              , "
                             "isy_5510_nolu_kanun_yuzdesi               , "
                             "haftalik_calisilan_gun_sayisi             , "
                             "gunluk_yemek_yardimi_yuzdesi                ",
                             "sabit_degerler_id   = :sabit_degerler_id  ");

    sql_query.SET_VALUE( ":issizlik_sig_isci_yuzdesi"                     , m_ui->comma_edit_isszlk_sig_isci_yuzdesi->GET_DOUBLE() / 100.0 );
    sql_query.SET_VALUE( ":issizlik_sig_isveren_yuzdesi"                  , m_ui->comma_edit_isszlk_sig_isveren_yuzdesi->GET_DOUBLE() / 100.0);
    sql_query.SET_VALUE( ":sakatlik_ind_derece_1_tutari"                  , m_ui->comma_edit_sakatlik_indirimi_derece_1->GET_DOUBLE());
    sql_query.SET_VALUE( ":sakatlik_ind_derece_2_tutari"                  , m_ui->comma_edit_sakatlik_indirimi_derece_2->GET_DOUBLE());
    sql_query.SET_VALUE( ":sakatlik_ind_derece_3_tutari"                  , m_ui->comma_edit_sakatlik_indirimi_derece_3->GET_DOUBLE());
    sql_query.SET_VALUE( ":tehlike_sinifi"                                , m_ui->combo_box_tehlike_sinifi->currentText().toInt());
    sql_query.SET_VALUE( ":ssk_primi_isci_yuzdesi"                        , m_ui->comma_edit_ssk_primi_isci_yuzdesi->GET_DOUBLE() / 100.0);
    sql_query.SET_VALUE( ":ssk_primi_isveren_yuzdesi"                     , m_ui->comma_edit_ssk_primi_isveren_yuzdesi->GET_DOUBLE() / 100.0);
    sql_query.SET_VALUE( ":ssk_taban_degeri"                              , m_ui->comma_edit_ssk_taban->GET_DOUBLE());
    sql_query.SET_VALUE( ":ssk_tavan_degeri"                              , m_ui->comma_edit_ssk_tavan->GET_DOUBLE());
    sql_query.SET_VALUE( ":ssk_16_yas_alti_asgari_ucret"                  , m_ui->comma_edit_16_yas_alti_asg_ucret->GET_DOUBLE());
    sql_query.SET_VALUE( ":ssk_16_yas_ustu_asgari_ucret"                  , m_ui->comma_edit_16_yas_ustu_asg_ucret->GET_DOUBLE());
    sql_query.SET_VALUE( ":damga_vergisi"                                 , m_ui->comma_edit_damga_vergisi_orani->GET_DOUBLE() / 100.0);
    sql_query.SET_VALUE( ":gecerli_glr_ver_dilimi_sayisi"                 , G_PRS_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi );
    sql_query.SET_VALUE( ":glr_ver_dilim_bas_tutar_array"                 , PACK_DOUBLE_ARRAY(G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI,GELIR_VERGISI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":glr_ver_dilim_bts_tutar_array"                 , PACK_DOUBLE_ARRAY(G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI ,GELIR_VERGISI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":gelir_vergisi_yuzdeleri_array"                 , PACK_DOUBLE_ARRAY(G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI , GELIR_VERGISI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":evli_es_clsmyr_asggecind_array"                , PACK_DOUBLE_ARRAY(G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI ,MAX_COCUK_SAYISI));
    sql_query.SET_VALUE( ":evli_es_clsyr_asggecind_array"                 , PACK_DOUBLE_ARRAY(G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI , MAX_COCUK_SAYISI));
    sql_query.SET_VALUE( ":bekar_asg_gec_indirimler_array"                , PACK_DOUBLE_ARRAY(G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI, MAX_COCUK_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bas_trh_turleri_array"                , PACK_INT_ARRAY(M_IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI , IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bas_trh_orj_deg_array"                , PACK_DOUBLE_ARRAY(M_IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI,IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bas_trh_ay_deg_array"                 , PACK_DOUBLE_ARRAY(M_IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI ,IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bts_trh_turleri_array"                , PACK_INT_ARRAY(M_IHBAR_TAZ_BITIS_TARIHI_TURLERI , IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bts_trh_orj_deg_array"                , PACK_DOUBLE_ARRAY(M_IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI , IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bts_trh_ay_deg_array"                 , PACK_DOUBLE_ARRAY(M_IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI , IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbar_taz_hafta_deg_array"                     , PACK_DOUBLE_ARRAY(M_IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI , IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":frm_tabi_sigorta_kolu"                         , PRS_GET_FIRMA_SIGORTA_KOLU_INT(m_ui->comboBox_frm_tabi_sig_kolu->currentText()));
    sql_query.SET_VALUE( ":frm_sendika_uyesi_mi"                          , QVariant(m_ui->checkBox_sendika_uyesi_mi->isChecked()).toInt());
    sql_query.SET_VALUE( ":sendika_aidati"                                , m_ui->commaEdit_sendika_aidati->GET_DOUBLE());
    sql_query.SET_VALUE( ":tehlike_sinifi_isci_yuzdesi"                   , m_ui->commaEdit_isci_tehlike_sinifi->GET_DOUBLE() / 100    );
    sql_query.SET_VALUE( ":tehlike_sinifi_isveren_yuzdesi"                , m_ui->commaEdit_isveren_tehlike_sinifi->GET_DOUBLE() / 100  );
    sql_query.SET_VALUE( ":ssk_isveren_taban_deger"                       , m_ui->comma_edit_ssk_isveren_taban->GET_DOUBLE() );
    sql_query.SET_VALUE( ":ssk_isveren_tavan_deger"                       , m_ui->comma_edit_ssk_isveren_tavan->GET_DOUBLE() );
    sql_query.SET_VALUE( ":prs_sgdp_primi_isci_yuzdesi"                   , m_ui->commaEdit_prs_sgdp_isci_yuzdesi->GET_DOUBLE() / 100.0  );
    sql_query.SET_VALUE( ":prs_sgdp_primi_isveren_yuzdesi"                , m_ui->commaEdit_prs_sgdp_isveren_yuzdesi->GET_DOUBLE() / 100.0  );
    sql_query.SET_VALUE( ":muh_fis_hesaplari_birlestir"                   , PRS_GET_BORDRO_MUH_HESAPLARI_BIRLESTIR_ENUM( m_ui->comboBox_muh_fis_hesaplari_birlestir->currentText()) );
    sql_query.SET_VALUE( ":isy_5510_nolu_kanuna_tabi_mi"                  , QVariant ( m_ui->checkBox_isy_5510_nolu_kanuna_tabi_mi->isChecked() ).toInt() );
    sql_query.SET_VALUE( ":isy_5510_nolu_kanun_yuzdesi"                   , m_ui->commaEdit_isy_5510_nolu_kanun_yuzdesi->GET_DOUBLE() );
    sql_query.SET_VALUE( ":haftalik_calisilan_gun_sayisi"                 , G_PRS_SABITLER.haftalik_calisilan_gun_sayisi );
    sql_query.SET_VALUE( ":gunluk_yemek_yardimi_yuzdesi"                  , G_PRS_SABITLER.gunluk_yemek_yardimi_yuzdesi );
    sql_query.SET_VALUE( ":sabit_degerler_id"                             , record_id );

    sql_query.UPDATE();


    //

    sql_query.PREPARE_UPDATE("prs_firma_odenekleri","odenekler_id",
                             "dogum_yardimi_var_mi_enum   , "
                             "dogum_yardimi_ssk_kesintisi , "
                             "dogum_yardimi_gelir_vergisi , "
                             "dogum_yardimi_damga_vergisi , "
                             "dogum_yardimi_yuzde_mi      , "
                             "dogum_yardimi_yuzdesi       , "
                             "dogum_yardimi_tutari        , "
                             "olum_yardimi_var_mi_enum    , "
                             "olum_yardimi_ssk_kesintisi  , "
                             "olum_yardimi_gelir_vergisi  , "
                             "olum_yardimi_damga_vergisi  , "
                             "olum_yardimi_yuzde_mi       , "
                             "olum_yardimi_yuzdesi           , "
                             "olum_yardimi_tutari            , "
                             "cocuk_yardimi_var_mi_enum      , "
                             "cocuk_yardimi_ssk_kesintisi    , "
                             "cocuk_yardimi_gelir_vergisi    , "
                             "cocuk_yardimi_damga_vergisi    , "
                             "cocuk_yardimi_yuzde_mi         , "
                             "cocuk_yardimi_yuzdesi          , "
                             "cocuk_yardimi_tutari           , "
                             "yakacak_yardimi_var_mi_enum    , "
                             "yakacak_yardimi_ssk_kesintisi  , "
                             "yakacak_yardimi_gelir_vergisi  , "
                             "yakacak_yardimi_damga_vergisi  , "
                             "yakacak_yardimi_yuzde_mi       , "
                             "yakacak_yardimi_yuzdesi        , "
                             "yakacak_yardimi_tutari         , "
                             "aile_yardimi_var_mi_enum       , "
                             "aile_yardimi_ssk_kesintisi     , "
                             "aile_yardimi_gelir_vergisi     , "
                             "aile_yardimi_damga_vergisi     , "
                             "aile_yardimi_yuzde_mi          , "
                             "aile_yardimi_yuzdesi           , "
                             "aile_yardimi_tutari            , "
                             "yemek_parasi_var_mi_enum       , "
                             "yemek_parasi_ssk_kesintisi     , "
                             "yemek_parasi_gelir_vergisi     , "
                             "yemek_parasi_damga_vergisi     , "
                             "yemek_parasi_yuzde_mi          , "
                             "yemek_parasi_yuzdesi           , "
                             "yemek_parasi_tutari            , "
                             "evlenme_yardimi_var_mi_enum    , "
                             "evlenme_yardimi_ssk_kesintisi  , "
                             "evlenme_yardimi_gelir_vergisi  , "
                             "evlenme_yardimi_damga_vergisi  , "
                             "evlenme_yardimi_yuzde_mi       , "
                             "evlenme_yardimi_yuzdesi        , "
                             "evlenme_yardimi_tutari         , "
                             "gorev_harcirahi_var_mi_enum    , "
                             "gorev_harcirahi_ssk_kesintisi  , "
                             "gorev_harcirahi_gelir_vergisi  , "
                             "gorev_harcirahi_damga_vergisi  , "
                             "gorev_harcirahi_yuzde_mi       , "
                             "gorev_harcirahi_yuzdesi        , "
                             "gorev_harcirahi_tutari         , "
                             "ihbar_tazminati_var_mi_enum    , "
                             "ihbar_tazminati_ssk_kesintisi  , "
                             "ihbar_tazminati_gelir_vergisi  , "
                             "ihbar_tazminati_damga_vergisi  , "
                             "ihbar_tazminati_yuzde_mi       , "
                             "ihbar_tazminati_yuzdesi        , "
                             "ihbar_tazminati_tutari         , "
                             "kidem_tazminati_var_mi_enum    , "
                             "kidem_tazminati_ssk_kesintisi  , "
                             "kidem_tazminati_gelir_vergisi  , "
                             "kidem_tazminati_damga_vergisi  , "
                             "kidem_tazminati_yuzde_mi       , "
                             "kidem_tazminati_yuzdesi        , "
                             "kidem_tazminati_tutari         ",
                             "odenekler_id   = :odenekler_id"   );

    sql_query.SET_VALUE( ":dogum_yardimi_var_mi_enum"           , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_dogum_yardimi_kimlere_verilir->currentText()));
    sql_query.SET_VALUE( ":dogum_yardimi_ssk_kesintisi"         , QVariant(m_ui->check_box_dogum_yardimi_ssk_kesintisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":dogum_yardimi_gelir_vergisi"         , QVariant(m_ui->check_box_dogum_yardimi_gelir_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":dogum_yardimi_damga_vergisi"         , QVariant(m_ui->check_box_dogum_yardimi_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":dogum_yardimi_yuzde_mi"              , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_dogum_yardimi_yuzde_mi->currentText()));
    sql_query.SET_VALUE( ":dogum_yardimi_yuzdesi"               , m_ui->comma_edit_dogum_yardimi_yuzdesi->GET_DOUBLE() / 100 );
    sql_query.SET_VALUE( ":dogum_yardimi_tutari"                , m_ui->comma_edit_dogum_yardimi_tutari->GET_DOUBLE() );

    sql_query.SET_VALUE( ":olum_yardimi_var_mi_enum"            , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_olum_yardimi_kimlere_verilir->currentText())  );
    sql_query.SET_VALUE( ":olum_yardimi_ssk_kesintisi"          , QVariant(m_ui->check_box_olum_yardimi_ssk_kesintisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":olum_yardimi_gelir_vergisi"          , QVariant(m_ui->check_box_olum_yardimi_gelir_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":olum_yardimi_damga_vergisi"          , QVariant(m_ui->check_box_olum_yardimi_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":olum_yardimi_yuzde_mi"               , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_olum_yardimi_yuzde_mi->currentText()) );
    sql_query.SET_VALUE( ":olum_yardimi_yuzdesi"                , m_ui->comma_edit_olum_yardimi_yuzdesi->GET_DOUBLE() / 100  );
    sql_query.SET_VALUE( ":olum_yardimi_tutari"                 , m_ui->comma_edit_olum_yardimi_tutari->GET_DOUBLE());

    sql_query.SET_VALUE( ":cocuk_yardimi_var_mi_enum"           , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_cocuk_yardimi_kimlere_verilir->currentText())  );
    sql_query.SET_VALUE( ":cocuk_yardimi_ssk_kesintisi"         , QVariant(m_ui->check_box_cocuk_yardimi_ssk_kesintisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":cocuk_yardimi_gelir_vergisi"         , QVariant(m_ui->check_box_cocuk_yardimi_gelir_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":cocuk_yardimi_damga_vergisi"         , QVariant(m_ui->check_box_cocuk_yardimi_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":cocuk_yardimi_yuzde_mi"              , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_cocuk_yardimi_yuzde_mi->currentText()) );
    sql_query.SET_VALUE( ":cocuk_yardimi_yuzdesi"               , m_ui->comma_edit_cocuk_yardimi_yuzdesi->GET_DOUBLE() / 100 );
    sql_query.SET_VALUE( ":cocuk_yardimi_tutari"                , m_ui->comma_edit_cocuk_yardimi_tutari->GET_DOUBLE());

    sql_query.SET_VALUE( ":yakacak_yardimi_var_mi_enum"         , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_yakacak_yardimi_kimlere_verilir->currentText()) );
    sql_query.SET_VALUE( ":yakacak_yardimi_ssk_kesintisi"       , QVariant(m_ui->check_box_yakacak_yardimi_ssk_kesintisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":yakacak_yardimi_gelir_vergisi"       , QVariant(m_ui->check_box_yakacak_yardimi_gelir_vergisi->isChecked()).toInt()  );
    sql_query.SET_VALUE( ":yakacak_yardimi_damga_vergisi"       , QVariant(m_ui->check_box_yakacak_yardimi_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":yakacak_yardimi_yuzde_mi"            , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_yakacak_yardimi_yuzde_mi->currentText())  );
    sql_query.SET_VALUE( ":yakacak_yardimi_yuzdesi"             , m_ui->comma_edit_yakacak_yardimi_yuzdesi->GET_DOUBLE() / 100 );
    sql_query.SET_VALUE( ":yakacak_yardimi_tutari"              , m_ui->comma_edit_yakacak_yardimi_tutari->GET_DOUBLE() );

    sql_query.SET_VALUE( ":aile_yardimi_var_mi_enum"            , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_aile_yardimi_kimlere_verilir->currentText())  );
    sql_query.SET_VALUE( ":aile_yardimi_ssk_kesintisi"          , QVariant(m_ui->check_box_aile_yardimi_ssk_kesintisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":aile_yardimi_gelir_vergisi"          , QVariant(m_ui->check_box_aile_yardimi_gelir_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":aile_yardimi_damga_vergisi"          , QVariant(m_ui->check_box_aile_yardimi_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":aile_yardimi_yuzde_mi"               , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_aile_yardimi_yuzde_mi->currentText()) );
    sql_query.SET_VALUE( ":aile_yardimi_yuzdesi"                , m_ui->comma_edit_aile_yardimi_yuzdesi->GET_DOUBLE() / 100);
    sql_query.SET_VALUE( ":aile_yardimi_tutari"                 , m_ui->comma_edit_aile_yardimi_tutari->GET_DOUBLE());

    sql_query.SET_VALUE( ":yemek_parasi_var_mi_enum"            , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_yemek_parasi_kimlere_verilir->currentText())  );
    sql_query.SET_VALUE( ":yemek_parasi_ssk_kesintisi"          , QVariant(m_ui->check_box_yemek_parasi_ssk_kesintisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":yemek_parasi_gelir_vergisi"          , QVariant(m_ui->check_box_yemek_parasi_gelir_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":yemek_parasi_damga_vergisi"          , QVariant(m_ui->check_box_yemek_parasi_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":yemek_parasi_yuzde_mi"               , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_yemek_parasi_yuzde_mi->currentText()) );
    sql_query.SET_VALUE( ":yemek_parasi_yuzdesi"                , ROUND(m_ui->comma_edit_yemek_parasi_yuzdesi->GET_DOUBLE() / 100) );
    sql_query.SET_VALUE( ":yemek_parasi_tutari"                 , m_ui->comma_edit_yemek_parasi_tutari->GET_DOUBLE());

    sql_query.SET_VALUE( ":evlenme_yardimi_var_mi_enum"         , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_evlenme_yardimi_kimlere_verilir->currentText())  );
    sql_query.SET_VALUE( ":evlenme_yardimi_ssk_kesintisi"       , QVariant(m_ui->check_box_evlenme_yardimi_ssk_kesintisi->isChecked()).toInt()  );
    sql_query.SET_VALUE( ":evlenme_yardimi_gelir_vergisi"       , QVariant(m_ui->check_box_evlenme_yardimi_gelir_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":evlenme_yardimi_damga_vergisi"       , QVariant(m_ui->check_box_evlenme_yardimi_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":evlenme_yardimi_yuzde_mi"            , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_evlenme_yardimi_yuzde_mi->currentText())  );
    sql_query.SET_VALUE( ":evlenme_yardimi_yuzdesi"             , m_ui->comma_edit_evlenme_yardimi_yuzdesi->GET_DOUBLE()/ 100 );
    sql_query.SET_VALUE( ":evlenme_yardimi_tutari"              , m_ui->comma_edit_evlenme_yardimi_tutari->GET_DOUBLE() );

    sql_query.SET_VALUE( ":gorev_harcirahi_var_mi_enum"         , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_gorev_harcirahi_kimlere_verilir->currentText()) );
    sql_query.SET_VALUE( ":gorev_harcirahi_ssk_kesintisi"       , QVariant(m_ui->check_box_gorev_harcirahi_ssk_kesintisi->isChecked()).toInt()  );
    sql_query.SET_VALUE( ":gorev_harcirahi_gelir_vergisi"       , QVariant(m_ui->check_box_gorev_harcirahi_gelir_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":gorev_harcirahi_damga_vergisi"       , QVariant(m_ui->check_box_gorev_harcirahi_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":gorev_harcirahi_yuzde_mi"            , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_gorev_harcirahi_yuzde_mi->currentText()) );
    sql_query.SET_VALUE( ":gorev_harcirahi_yuzdesi"             , m_ui->comma_edit_gorev_harcirahi_yuzdesi->GET_DOUBLE() / 100 );

    sql_query.SET_VALUE( ":gorev_harcirahi_tutari"              , m_ui->comma_edit_gorev_harcirahi_tutari->GET_DOUBLE() );

    sql_query.SET_VALUE( ":ihbar_tazminati_var_mi_enum"         , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_ihbar_tazminati_kimlere_verilir->currentText()) );
    sql_query.SET_VALUE( ":ihbar_tazminati_ssk_kesintisi"       , QVariant(m_ui->check_box_ihbar_tazminati_ssk_kesintisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":ihbar_tazminati_gelir_vergisi"       , QVariant(m_ui->check_box_ihbar_tazminati_gelir_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":ihbar_tazminati_damga_vergisi"       , QVariant(m_ui->check_box_ihbar_tazminati_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":ihbar_tazminati_yuzde_mi"            , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_ihbar_tazminati_yuzde_mi->currentText()) );
    sql_query.SET_VALUE( ":ihbar_tazminati_yuzdesi"             , m_ui->comma_edit_ihbar_tazminati_yuzdesi->GET_DOUBLE() / 100 );
    sql_query.SET_VALUE( ":ihbar_tazminati_tutari"              , m_ui->comma_edit_ihbar_tazminati_tutari->GET_DOUBLE() );

    sql_query.SET_VALUE( ":kidem_tazminati_var_mi_enum"         , PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(m_ui->combo_box_kidem_tazminati_kimlere_verilir->currentText()) );
    sql_query.SET_VALUE( ":kidem_tazminati_ssk_kesintisi"       , QVariant(m_ui->check_box_kidem_tazminati_ssk_kesintisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":kidem_tazminati_gelir_vergisi"       , QVariant(m_ui->check_box_kidem_tazminati_gelir_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":kidem_tazminati_damga_vergisi"       , QVariant(m_ui->check_box_kidem_tazminati_damga_vergisi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":kidem_tazminati_yuzde_mi"            , PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(m_ui->combo_box_kidem_tazminati_yuzde_mi->currentText())  );
    sql_query.SET_VALUE( ":kidem_tazminati_yuzdesi"             , m_ui->comma_edit_kidem_tazminati_yuzdesi->GET_DOUBLE() / 100 );
    sql_query.SET_VALUE( ":kidem_tazminati_tutari"              , m_ui->comma_edit_kidem_tazminati_tutari->GET_DOUBLE() );

    sql_query.SET_VALUE( ":odenekler_id"                        , record_id );

    sql_query.UPDATE();


    E9_LOG_KAYDI_EKLE ( PERSONEL_MODULU , LOG_PRS_SABIT_DEGERLER ,LOG_ISLEM_UPDATE ,QObject::tr("Sabit Değerler"));
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::CHECK_DELETE
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::CHECK_DELETE(int record_id)
{
    Q_UNUSED(record_id);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::DELETE_RECORD
**************************************************************************************/

void PRS_SABIT_DEGERLER_FORMU::DELETE_RECORD(int record_id)
{
    Q_UNUSED(record_id);
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::FIND_FIRST_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::FIND_LAST_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::FIND_NEXT_RECORD
**************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::FIND_NEXT_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::FIND_PREV_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::SELECT_RECORD
**************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::SELECT_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::LOCK_RECORD
***************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::LOCK_RECORD(int record_id)
{
    return DB->LOCK_ROW ( "sabit_degerler_id","prs_sabit_degerler", QString ( "sabit_degerler_id = %1").arg ( record_id ) );
}


/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::UNLOCK_RECORD
***************************************************************************************/

void PRS_SABIT_DEGERLER_FORMU::UNLOCK_RECORD(int record_id)
{
    DB->UNLOCK_ROW ( "sabit_degerler_id","prs_sabit_degerler", QString ( "sabit_degerler_id = %1").arg ( record_id ) );
}


/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::FIND_ENTERED_RECORD
**************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::FIND_RECORD()
{
    return 0;
}


/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::TEHLIKE_SINIFINA_GORE_SSK_PRIMI_BELIRLE
***************************************************************************************/

void PRS_SABIT_DEGERLER_FORMU::TEHLIKE_SINIFINA_GORE_SSK_PRIMI_BELIRLE()
{
    m_ui->comma_edit_ssk_primi_isveren_yuzdesi->SET_DOUBLE( G_PRS_SABITLER.ssk_primi_isveren_yuzdesi +
                                                         QVariant( m_ui->combo_box_tehlike_sinifi->currentText().toInt() -
                                                         G_PRS_SABITLER.tehlike_sinifi ).toDouble()/ 100 );

    G_PRS_SABITLER.tehlike_sinifi = m_ui->combo_box_tehlike_sinifi->currentText().toInt();
    G_PRS_SABITLER.ssk_primi_isveren_yuzdesi = m_ui->comma_edit_ssk_primi_isveren_yuzdesi->GET_DOUBLE();
}


/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::GELIR_VERGISI_DILIMI_ARALIKLARINI_BELIRLE
***************************************************************************************/

void PRS_SABIT_DEGERLER_FORMU::GELIR_VERGISI_DILIMI_ARALIKLARINI_BELIRLE ( int index, QLabel * label_gelir_vergisi ,
                                                                QLineEdit * line_edit_gelir_vergisi_dilimi_bitis )
{
    if ( index EQ 1 ){

        label_gelir_vergisi->setText("TL arasinda");
        line_edit_gelir_vergisi_dilimi_bitis->setHidden(false);
    }
    else  {

        label_gelir_vergisi->setText("TL üzerinde");
        line_edit_gelir_vergisi_dilimi_bitis->setHidden(true);
    }
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::SHOW_SABIT_DEGERLER_DEFAULTS
***************************************************************************************/

void PRS_SABIT_DEGERLER_FORMU::SHOW_SABIT_DEGERLER_DEFAULTS ()
{

    m_ui->comma_edit_isszlk_sig_isci_yuzdesi->SET_DOUBLE      ( G_PRS_SABITLER.issizlik_sig_isci_yuzdesi * 100);
    m_ui->comma_edit_isszlk_sig_isveren_yuzdesi->SET_DOUBLE   ( G_PRS_SABITLER.issizlik_sig_isveren_yuzdesi * 100);
    m_ui->comma_edit_sakatlik_indirimi_derece_1->SET_DOUBLE   ( G_PRS_SABITLER.sakatlik_ind_derece_1_tutari);
    m_ui->comma_edit_sakatlik_indirimi_derece_2->SET_DOUBLE   ( G_PRS_SABITLER.sakatlik_ind_derece_2_tutari);
    m_ui->comma_edit_sakatlik_indirimi_derece_3->SET_DOUBLE   ( G_PRS_SABITLER.sakatlik_ind_derece_3_tutari);
    m_ui->combo_box_tehlike_sinifi->setCurrentIndex(
                   m_ui->combo_box_tehlike_sinifi->findText   ( QVariant(G_PRS_SABITLER.tehlike_sinifi).toString()) );
    m_ui->comma_edit_ssk_primi_isci_yuzdesi->SET_DOUBLE       ( G_PRS_SABITLER.ssk_primi_isci_yuzdesi * 100);
    m_ui->comma_edit_ssk_primi_isveren_yuzdesi->SET_DOUBLE    ( G_PRS_SABITLER.ssk_primi_isveren_yuzdesi * 100);
    m_ui->comma_edit_ssk_taban->SET_DOUBLE                    ( G_PRS_SABITLER.ssk_taban_degeri);
    m_ui->comma_edit_ssk_tavan->SET_DOUBLE                    ( G_PRS_SABITLER.ssk_tavan_degeri);
    m_ui->comma_edit_16_yas_alti_asg_ucret->SET_DOUBLE        ( G_PRS_SABITLER.ssk_16_yas_alti_asgari_ucret);
    m_ui->comma_edit_16_yas_ustu_asg_ucret->SET_DOUBLE        ( G_PRS_SABITLER.ssk_16_yas_ustu_asgari_ucret);
    m_ui->comma_edit_damga_vergisi_orani->SET_DOUBLE          ( G_PRS_SABITLER.damga_vergisi * 100);
    m_ui->commaEdit_prs_sgdp_isci_yuzdesi->SET_DOUBLE         ( G_PRS_SABITLER.prs_sgdp_primi_isci_yuzdesi);
    m_ui->commaEdit_prs_sgdp_isveren_yuzdesi->SET_DOUBLE      ( G_PRS_SABITLER.prs_sgdp_primi_isveren_yuzdesi);

    m_ui->commaEdit_haftalik_calisilan_gun_sayisi->SET_DOUBLE ( G_PRS_SABITLER.haftalik_calisilan_gun_sayisi);
    m_ui->commaEdit_gunluk_yemek_yardimi_yuzdesi->SET_DOUBLE  ( G_PRS_SABITLER.gunluk_yemek_yardimi_yuzdesi);


    m_ui->comma_edit_ssk_isveren_taban->SET_DOUBLE( G_PRS_SABITLER.ssk_isveren_taban_deger );
    m_ui->comma_edit_ssk_isveren_tavan->SET_DOUBLE( G_PRS_SABITLER.ssk_isveren_tavan_deger );

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_1->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[0]  );
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_1->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[0]  );
    m_ui->comma_edit_gelir_vergisi_orani_1->SET_DOUBLE(  G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[0] * 100 );

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_2->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[1]);
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_2->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[1]);
    m_ui->comma_edit_gelir_vergisi_orani_5->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[1] * 100);

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_3->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[2]);
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_3->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[2]);
    m_ui->comma_edit_gelir_vergisi_orani_5->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[2] * 100 );

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_4->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[3]);
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_4->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[3] );
    m_ui->comma_edit_gelir_vergisi_orani_5->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[3] * 100 );

    m_ui->comma_edit_gelir_vergisi_dilimi_bas_5->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[4] );
    m_ui->comma_edit_gelir_vergisi_dilimi_bit_5->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[4] );
    m_ui->comma_edit_gelir_vergisi_orani_5->SET_DOUBLE( G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[4] * 100);

    m_ui->comma_edit_evli_es_calismiyor_cocuk_yok->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ] );
    m_ui->comma_edit_evli_es_calismiyor_1_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI  [ PRS_TEK_COCUK ] );
    m_ui->comma_edit_evli_es_calismiyor_2_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI  [ PRS_IKI_COCUK ] );
    m_ui->comma_edit_evli_es_calismiyor_3_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI  [ PRS_UC_COCUK  ] );
    m_ui->comma_edit_evli_es_calismiyor_4_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI  [ PRS_DORT_COCUK] );

    m_ui->comma_edit_evli_es_calisiyor_cocuk_yok->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI  [ PRS_COCUK_YOK ] );
    m_ui->comma_edit_evli_es_calisiyor_1_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI    [ PRS_TEK_COCUK ] );
    m_ui->comma_edit_evli_es_calisiyor_2_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI    [ PRS_IKI_COCUK ] );
    m_ui->comma_edit_evli_es_calisiyor_3_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI    [ PRS_UC_COCUK  ] );
    m_ui->comma_edit_evli_es_calisiyor_4_cocuk->SET_DOUBLE( G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI    [ PRS_DORT_COCUK] );

    m_ui->comma_edit_bekar_cocuk_yok->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_COCUK_YOK ] );
    m_ui->comma_edit_bekar_1_cocuk->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI   [ PRS_TEK_COCUK ] );
    m_ui->comma_edit_bekar_2_cocuk->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI   [ PRS_IKI_COCUK ] );
    m_ui->comma_edit_bekar_3_cocuk->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI   [ PRS_UC_COCUK  ] );
    m_ui->comma_edit_bekar_4_cocuk->SET_DOUBLE( G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI   [ PRS_DORT_COCUK] );


    m_ui->combo_box_ihbar_taz_bas_suresi_1->setCurrentIndex  ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].baslangic_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bas_suresi_2->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].baslangic_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bas_suresi_3->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].baslangic_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bas_suresi_4->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].baslangic_tarihi_turu );

    m_ui->comma_edit_ihbar_taz_bas_suresi_1->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].baslangic_tarihi_orj_degeri );
    m_ui->comma_edit_ihbar_taz_bas_suresi_2->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].baslangic_tarihi_orj_degeri );
    m_ui->comma_edit_ihbar_taz_bas_suresi_3->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].baslangic_tarihi_orj_degeri );
    m_ui->comma_edit_ihbar_taz_bas_suresi_4->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].baslangic_tarihi_orj_degeri );

    m_ui->combo_box_ihbar_taz_bit_suresi_1->setCurrentIndex  ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].bitis_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bit_suresi_2->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].bitis_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bit_suresi_3->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].bitis_tarihi_turu );
    m_ui->combo_box_ihbar_taz_bit_suresi_4->setCurrentIndex( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].bitis_tarihi_turu );

    m_ui->comma_edit_ihbar_taz_bit_suresi_1->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].bitis_tarihi_orj_degeri );
    m_ui->comma_edit_ihbar_taz_bit_suresi_2->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].bitis_tarihi_orj_degeri );
    m_ui->comma_edit_ihbar_taz_bit_suresi_3->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].bitis_tarihi_orj_degeri );
    m_ui->comma_edit_ihbar_taz_bit_suresi_4->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].bitis_tarihi_orj_degeri );

    m_ui->comma_edit_ihbar_taz_tutari_1->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].tazminat_hafta_degeri );
    m_ui->comma_edit_ihbar_taz_tutari_2->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].tazminat_hafta_degeri );
    m_ui->comma_edit_ihbar_taz_tutari_3->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].tazminat_hafta_degeri );
    m_ui->comma_edit_ihbar_taz_tutari_4->SET_DOUBLE ( G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].tazminat_hafta_degeri );

}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::HIGHLIGHT_NON_DEFAULT_VALUES
***************************************************************************************/

void PRS_SABIT_DEGERLER_FORMU::HIGHLIGHT_NON_DEFAULT_VALUES ( QObject * object )
{

    /////////////////////////////////     SSK Bilgileri   /////////////////////////////////////////////

    if ( object EQ m_ui->comma_edit_isszlk_sig_isci_yuzdesi ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_isszlk_sig_isci_yuzdesi , G_PRS_DEFAULT_SABITLER.issizlik_sig_isci_yuzdesi, PRS_YUZDE);
    }

    else if ( object EQ m_ui->comma_edit_isszlk_sig_isveren_yuzdesi ) {
        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_isszlk_sig_isveren_yuzdesi, G_PRS_DEFAULT_SABITLER.issizlik_sig_isveren_yuzdesi, PRS_YUZDE);
    }

    else if ( object EQ m_ui->comma_edit_sakatlik_indirimi_derece_1 ) {
        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_sakatlik_indirimi_derece_1, G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_1_tutari, PRS_TUTAR );
    }

    else if ( object EQ m_ui->comma_edit_sakatlik_indirimi_derece_2 ) {
        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_sakatlik_indirimi_derece_2, G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_2_tutari, PRS_TUTAR  );
    }

    else if ( object EQ m_ui->comma_edit_sakatlik_indirimi_derece_3 ) {
        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_sakatlik_indirimi_derece_3, G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_3_tutari, PRS_TUTAR  );
    }

    else if ( object EQ m_ui->comma_edit_ssk_taban ) {
        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_ssk_taban, G_PRS_DEFAULT_SABITLER.ssk_taban_degeri, PRS_TUTAR  );
    }

     else if ( object EQ m_ui->comma_edit_ssk_tavan ) {
         PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_ssk_tavan, G_PRS_DEFAULT_SABITLER.ssk_tavan_degeri, PRS_TUTAR  );
    }

    else if ( object EQ m_ui->comma_edit_ssk_isveren_taban ) {
         PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_ssk_isveren_taban, G_PRS_DEFAULT_SABITLER.ssk_isveren_taban_deger, PRS_TUTAR  );
     }

     else if ( object EQ m_ui->comma_edit_ssk_tavan ) {
         PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_ssk_isveren_tavan, G_PRS_DEFAULT_SABITLER.ssk_isveren_tavan_deger, PRS_TUTAR  );
     }

    else if ( object EQ m_ui->comma_edit_16_yas_alti_asg_ucret ) {
        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_16_yas_alti_asg_ucret, G_PRS_DEFAULT_SABITLER.ssk_16_yas_alti_asgari_ucret, PRS_TUTAR  );
    }

    else if ( object EQ m_ui->comma_edit_16_yas_ustu_asg_ucret ) {
        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_16_yas_ustu_asg_ucret, G_PRS_DEFAULT_SABITLER.ssk_16_yas_ustu_asgari_ucret, PRS_TUTAR  );
    }

    ////////////////////////////////////      Vergi Bilgileri    ///////////////////////////////////////////////

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bas_1 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_1,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 0 ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bas_2 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_2,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 1 ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bas_3 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_3,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 2 ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bas_4 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_4,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 3 ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bas_5 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bas_5,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ 4 ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bit_1 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_1,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 0 ], PRS_TUTAR  );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bit_2 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_2,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 1 ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bit_3 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_3,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 2 ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bit_4 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_4,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 3 ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_dilimi_bit_5 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_dilimi_bit_5,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[ 4 ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_orani_1 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_1,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 0 ], PRS_YUZDE  );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_orani_2 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_2,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 1 ], PRS_YUZDE  );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_orani_3 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_3,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 2 ], PRS_YUZDE  );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_orani_4 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_4,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 3 ], PRS_YUZDE  );
    }

    else if ( object EQ m_ui->comma_edit_gelir_vergisi_orani_5 ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_gelir_vergisi_orani_5,
                                           G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[ 4 ], PRS_YUZDE  );
    }

    else if ( object EQ m_ui->comma_edit_damga_vergisi_orani ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_damga_vergisi_orani,G_PRS_DEFAULT_SABITLER.damga_vergisi, PRS_YUZDE  );
    }

    /////////////////////////////////     Asgari Gecim Indirimi    /////////////////////////////////////////////

    else if ( object EQ m_ui->comma_edit_bekar_cocuk_yok ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_cocuk_yok,
                                           G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ] , PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_bekar_1_cocuk ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_1_cocuk,
                                           G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_TEK_COCUK ], PRS_TUTAR    );
    }

   else if ( object EQ m_ui->comma_edit_bekar_2_cocuk ) {

       PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_2_cocuk,
                                          G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_IKI_COCUK ], PRS_TUTAR    );
    }

    else if ( object EQ m_ui->comma_edit_bekar_3_cocuk ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_3_cocuk,
                                           G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_UC_COCUK ], PRS_TUTAR    );
    }

    else if ( object EQ m_ui->comma_edit_bekar_4_cocuk ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_bekar_4_cocuk,
                                           G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI[ PRS_DORT_COCUK ], PRS_TUTAR    );
    }

    else if ( object EQ m_ui->comma_edit_evli_es_calismiyor_cocuk_yok ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_cocuk_yok,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_evli_es_calismiyor_1_cocuk ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_1_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_TEK_COCUK ], PRS_TUTAR   );
    }

   else if ( object EQ m_ui->comma_edit_evli_es_calismiyor_2_cocuk ) {

       PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_2_cocuk,
                                          G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_IKI_COCUK ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_evli_es_calismiyor_3_cocuk ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_3_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_UC_COCUK ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_evli_es_calismiyor_4_cocuk ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calismiyor_4_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[ PRS_DORT_COCUK ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_evli_es_calisiyor_cocuk_yok ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_cocuk_yok,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_COCUK_YOK ], PRS_TUTAR   );
    }

    else if ( object EQ m_ui->comma_edit_evli_es_calisiyor_1_cocuk ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_1_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_TEK_COCUK ], PRS_TUTAR  );
    }

   else if ( object EQ m_ui->comma_edit_evli_es_calisiyor_2_cocuk ) {

       PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_2_cocuk,
                                          G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_IKI_COCUK ], PRS_TUTAR  );
    }

    else if ( object EQ m_ui->comma_edit_evli_es_calisiyor_3_cocuk ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_3_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_UC_COCUK ], PRS_TUTAR  );
    }

    else if ( object EQ m_ui->comma_edit_evli_es_calisiyor_4_cocuk ) {

        PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( m_ui->comma_edit_evli_es_calisiyor_4_cocuk,
                                           G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[ PRS_DORT_COCUK ], PRS_TUTAR  );
    }
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::SAVER_BUTTON_CLICKED
***************************************************************************************/

void PRS_SABIT_DEGERLER_FORMU::SAVER_BUTTON_CLICKED(QAbstractButton * button, int record_id)
{
   Q_UNUSED(record_id);


   if (button EQ m_ui->pushButton_ibraname_maddeleri) {
       SHOW_PRS_IBRANAME_MADDELERI_BATCH(record_id , this);
    }
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::CHANGER_BUTTON_CLICKED
 **************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    if ( m_ui->push_button_return_defaults EQ p_button ) {
        return SABIT_DEGERLERI_GUNCELLE();
    }
    else if( p_button EQ m_ui->push_button_return_defaults_odenek_ve_kesintiler ) {

        SET_DEFAULT_ODENEK_VE_KESINTIKER();

        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_aile_yardimi_kimlere_verilir,       m_ui->group_box_aile_yardimi    );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_cocuk_yardimi_kimlere_verilir,      m_ui->group_box_cocuk_yardimi   );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_dogum_yardimi_kimlere_verilir,      m_ui->group_box_dogum_yardimi   );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_evlenme_yardimi_kimlere_verilir,    m_ui->group_box_evlenme_yardimi );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_gorev_harcirahi_kimlere_verilir,    m_ui->group_box_gorev_harcirahi );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_ihbar_tazminati_kimlere_verilir,    m_ui->group_box_ihbar_tazminati );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_kidem_tazminati_kimlere_verilir,    m_ui->group_box_kidem_tazminati );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_olum_yardimi_kimlere_verilir,       m_ui->group_box_olum_yardimi    );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_yakacak_yardimi_kimlere_verilir,    m_ui->group_box_yakacak_yardimi );
        HIDE_OR_SHOW_GROUP_BOX( m_ui->combo_box_yemek_parasi_kimlere_verilir,       m_ui->group_box_yemek_parasi    );

        return ADAK_RECORD_CHANGED;
    }

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::SABIT_DEGERLERI_GUNCELLE
 **************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::SABIT_DEGERLERI_GUNCELLE ()
{

    QString xml_path = "http://www.e9ticaripaket.com/prs_sabit_degerler.xml";

    const QString html = QVariant ( xml_path ).toByteArray();

    QNetworkAccessManager * network_access_manager = new QNetworkAccessManager();
    QUrl url(html);
    QNetworkReply * network_reply = network_access_manager->get ( QNetworkRequest ( url ) );
    QEventLoop answer_loop;
    answer_loop.connect ( network_reply, SIGNAL ( finished() ), &answer_loop, SLOT ( quit() ) );
    QTimer bekleme_suresi;
    answer_loop.connect ( &bekleme_suresi, SIGNAL ( timeout() ), &answer_loop, SLOT ( quit() ) );
    bekleme_suresi.start ( 10000 );
    answer_loop.exec();
    QByteArray result = network_reply->readAll();

    PRS_SABITLER YENI_SABIT_DEGERLER = { QDate::currentDate(),
                                         0.010, 0.00, 0, 0, 0, 0, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                                         {0.0,      0,   0,  0,      0},
                                         {0,        0,   0,  0,      0},
                                         {0.00,     0.00,   0.00,   0.00,       0.00 },
                                          0,                                                    // gecerli_gelir_vergisi_dilimi_sayisi;
                                         {0.0,    0.0,  0.0,  0.0,      0.0},                   // EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI
                                         {0.0,    0.0,  0.0,  0.0,      0.0},                   // EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI
                                         {0.0,    0.0,  0.0,  0.0,      0.0},                   // BEKAR_ASG_GEC_INDIRIMLERI
                                         {{PRS_AY,  0,      0,      PRS_AY,     0,      0,     0},
                                         {PRS_AY,   0,      0,      PRS_YIL,    0.0,    0,     0},
                                         {PRS_YIL,  0.0,    0,      PRS_YIL,    0,      0,     0},
                                         {PRS_YIL,  0,      0,      PRS_YIL,    0,      0,     0}},
                                         0.00, 0.00,
                                         0.00, 0.00 ,  0.00, 0.00,
                                         0 ,            // muh_fis_hesaplari_birlestir
                                         0 , 5.00 ,    // isy_5510_nolu_kanuna_tabi_mi ve yuzdesi
                                         5.0, 6.0 };  // haftalik calisilan gun sayisi, gunluk yuzde

    if ( result.size() NE 0 ) {
        QXmlStreamReader xml_reader( result );

        while(!xml_reader.atEnd() && !xml_reader.hasError()) {
               QXmlStreamReader::TokenType token = xml_reader.readNext();
               if(token == QXmlStreamReader::StartDocument) {
                   continue;
               }
               if(token == QXmlStreamReader::StartElement) {
                   if(xml_reader.name() == "SABIT_DEGERLER") {
                       continue;
                   }
                   if(xml_reader.name() == "SINGLE") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("sgk_aciklama_tarihi")) {
                          YENI_SABIT_DEGERLER.sgk_aciklama_tarihi  = QDate::fromString( attrs.value("sgk_aciklama_tarihi").toString(), "yyyy.MM.dd");
                      }
                      if ( attrs.hasAttribute("issizlik_sig_isci_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.issizlik_sig_isci_yuzdesi     = QVariant ( attrs.value("issizlik_sig_isci_yuzdesi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("issizlik_sig_isveren_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.issizlik_sig_isveren_yuzdesi  = QVariant ( attrs.value("issizlik_sig_isveren_yuzdesi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("sakatlik_ind_derece_1_tutari") ) {
                          YENI_SABIT_DEGERLER.sakatlik_ind_derece_1_tutari  = QVariant ( attrs.value("sakatlik_ind_derece_1_tutari").toString() ).toInt();
                      }
                      if ( attrs.hasAttribute("sakatlik_ind_derece_2_tutari") ) {
                          YENI_SABIT_DEGERLER.sakatlik_ind_derece_2_tutari  = QVariant ( attrs.value("sakatlik_ind_derece_2_tutari").toString() ).toInt();
                      }
                      if ( attrs.hasAttribute("sakatlik_ind_derece_3_tutari") ) {
                          YENI_SABIT_DEGERLER.sakatlik_ind_derece_3_tutari  = QVariant ( attrs.value("sakatlik_ind_derece_3_tutari").toString() ).toInt();
                      }
                      if ( attrs.hasAttribute("tehlike_sinifi") ) {
                          YENI_SABIT_DEGERLER.tehlike_sinifi                = QVariant ( attrs.value("tehlike_sinifi").toString() ).toInt();
                      }
                      if ( attrs.hasAttribute("ssk_primi_isci_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.ssk_primi_isci_yuzdesi        = QVariant ( attrs.value("ssk_primi_isci_yuzdesi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("ssk_primi_isveren_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.ssk_primi_isveren_yuzdesi     = QVariant ( attrs.value("ssk_primi_isveren_yuzdesi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("ssk_taban_degeri") ) {
                          YENI_SABIT_DEGERLER.ssk_taban_degeri              = QVariant ( attrs.value("ssk_taban_degeri").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("ssk_tavan_degeri") ) {
                          YENI_SABIT_DEGERLER.ssk_tavan_degeri              = QVariant ( attrs.value("ssk_tavan_degeri").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("ssk_16_yas_alti_asgari_ucret") ) {
                          YENI_SABIT_DEGERLER.ssk_16_yas_alti_asgari_ucret  = QVariant ( attrs.value("ssk_16_yas_alti_asgari_ucret").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("ssk_16_yas_ustu_asgari_ucret") ) {
                          YENI_SABIT_DEGERLER.ssk_16_yas_ustu_asgari_ucret  = QVariant ( attrs.value("ssk_16_yas_ustu_asgari_ucret").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("damga_vergisi") ) {
                          YENI_SABIT_DEGERLER.damga_vergisi                 = QVariant ( attrs.value("damga_vergisi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("gecerli_gelir_vergisi_dilimi_sayisi") ) {
                          YENI_SABIT_DEGERLER.gecerli_gelir_vergisi_dilimi_sayisi   = QVariant ( attrs.value("gecerli_gelir_vergisi_dilimi_sayisi").toString() ).toInt();
                      }
                   }
                   if(xml_reader.name() == "GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("value_0")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[0] = QVariant( attrs.value("value_0").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_1")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[1] = QVariant( attrs.value("value_1").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_2")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[2] = QVariant( attrs.value("value_2").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_3")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[3] = QVariant( attrs.value("value_3").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_4")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[4] = QVariant( attrs.value("value_4").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("value_0")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[0] = QVariant( attrs.value("value_0").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_1")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[1] = QVariant( attrs.value("value_1").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_2")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[2] = QVariant( attrs.value("value_2").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_3")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[3] = QVariant( attrs.value("value_3").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_4")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[4] = QVariant( attrs.value("value_4").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "GELIR_VERGISI_YUZDELERI") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("value_0")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[0] = QVariant( attrs.value("value_0").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_1")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[1] = QVariant( attrs.value("value_1").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_2")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[2] = QVariant( attrs.value("value_2").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_3")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[3] = QVariant( attrs.value("value_3").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_4")) {
                          YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[4] = QVariant( attrs.value("value_4").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("value_0")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[0] = QVariant( attrs.value("value_0").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_1")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[1] = QVariant( attrs.value("value_1").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_2")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[2] = QVariant( attrs.value("value_2").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_3")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[3] = QVariant( attrs.value("value_3").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_4")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI[4] = QVariant( attrs.value("value_4").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("value_0")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[0] = QVariant( attrs.value("value_0").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_1")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[1] = QVariant( attrs.value("value_1").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_2")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[2] = QVariant( attrs.value("value_2").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_3")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[3] = QVariant( attrs.value("value_3").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_4")) {
                          YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI[4] = QVariant( attrs.value("value_4").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "BEKAR_ASG_GEC_INDIRIMLERI") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("value_0")) {
                          YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI[0] = QVariant( attrs.value("value_0").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_1")) {
                          YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI[1] = QVariant( attrs.value("value_1").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_2")) {
                          YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI[2] = QVariant( attrs.value("value_2").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_3")) {
                          YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI[3] = QVariant( attrs.value("value_3").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("value_4")) {
                          YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI[4] = QVariant( attrs.value("value_4").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "IHBAR_TAZMINATI_TUTARLARI_0") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("baslangic_tarihi_turu")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[0].baslangic_tarihi_turu        = QVariant( attrs.value("baslangic_tarihi_turu").toString()).toInt();
                      }
                      if(attrs.hasAttribute("baslangic_tarihi_orj_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[0].baslangic_tarihi_orj_degeri  = QVariant( attrs.value("baslangic_tarihi_orj_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("baslangic_tarihi_ay_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[0].baslangic_tarihi_ay_degeri   = QVariant( attrs.value("baslangic_tarihi_ay_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_turu")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[0].bitis_tarihi_turu            = QVariant( attrs.value("bitis_tarihi_turu").toString()).toInt();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_orj_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[0].bitis_tarihi_orj_degeri      = QVariant( attrs.value("bitis_tarihi_orj_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_ay_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[0].bitis_tarihi_ay_degeri       = QVariant( attrs.value("bitis_tarihi_ay_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("tazminat_hafta_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[0].tazminat_hafta_degeri        = QVariant( attrs.value("tazminat_hafta_degeri").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "IHBAR_TAZMINATI_TUTARLARI_1") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("baslangic_tarihi_turu")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[1].baslangic_tarihi_turu        = QVariant( attrs.value("baslangic_tarihi_turu").toString()).toInt();
                      }
                      if(attrs.hasAttribute("baslangic_tarihi_orj_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[1].baslangic_tarihi_orj_degeri  = QVariant( attrs.value("baslangic_tarihi_orj_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("baslangic_tarihi_ay_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[1].baslangic_tarihi_ay_degeri   = QVariant( attrs.value("baslangic_tarihi_ay_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_turu")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[1].bitis_tarihi_turu            = QVariant( attrs.value("bitis_tarihi_turu").toString()).toInt();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_orj_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[1].bitis_tarihi_orj_degeri      = QVariant( attrs.value("bitis_tarihi_orj_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_ay_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[1].bitis_tarihi_ay_degeri       = QVariant( attrs.value("bitis_tarihi_ay_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("tazminat_hafta_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[1].tazminat_hafta_degeri        = QVariant( attrs.value("tazminat_hafta_degeri").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "IHBAR_TAZMINATI_TUTARLARI_2") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("baslangic_tarihi_turu")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[2].baslangic_tarihi_turu        = QVariant( attrs.value("baslangic_tarihi_turu").toString()).toInt();
                      }
                      if(attrs.hasAttribute("baslangic_tarihi_orj_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[2].baslangic_tarihi_orj_degeri  = QVariant( attrs.value("baslangic_tarihi_orj_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("baslangic_tarihi_ay_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[2].baslangic_tarihi_ay_degeri   = QVariant( attrs.value("baslangic_tarihi_ay_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_turu")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[2].bitis_tarihi_turu            = QVariant( attrs.value("bitis_tarihi_turu").toString()).toInt();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_orj_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[2].bitis_tarihi_orj_degeri      = QVariant( attrs.value("bitis_tarihi_orj_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_ay_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[2].bitis_tarihi_ay_degeri       = QVariant( attrs.value("bitis_tarihi_ay_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("tazminat_hafta_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[2].tazminat_hafta_degeri        = QVariant( attrs.value("tazminat_hafta_degeri").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "IHBAR_TAZMINATI_TUTARLARI_3") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();
                      if(attrs.hasAttribute("baslangic_tarihi_turu")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[3].baslangic_tarihi_turu        = QVariant( attrs.value("baslangic_tarihi_turu").toString()).toInt();
                      }
                      if(attrs.hasAttribute("baslangic_tarihi_orj_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[3].baslangic_tarihi_orj_degeri  = QVariant( attrs.value("baslangic_tarihi_orj_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("baslangic_tarihi_ay_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[3].baslangic_tarihi_ay_degeri   = QVariant( attrs.value("baslangic_tarihi_ay_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_turu")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[3].bitis_tarihi_turu            = QVariant( attrs.value("bitis_tarihi_turu").toString()).toInt();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_orj_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[3].bitis_tarihi_orj_degeri      = QVariant( attrs.value("bitis_tarihi_orj_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("bitis_tarihi_ay_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[3].bitis_tarihi_ay_degeri       = QVariant( attrs.value("bitis_tarihi_ay_degeri").toString()).toDouble();
                      }
                      if(attrs.hasAttribute("tazminat_hafta_degeri")) {
                          YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[3].tazminat_hafta_degeri        = QVariant( attrs.value("tazminat_hafta_degeri").toString()).toDouble();
                      }
                   }

                   if(xml_reader.name() == "SINGLE") {
                      QXmlStreamAttributes attrs = xml_reader.attributes();

                      if ( attrs.hasAttribute("tehlike_sinifi_isci_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.tehlike_sinifi_isci_yuzdesi       = QVariant ( attrs.value("tehlike_sinifi_isci_yuzdesi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("tehlike_sinifi_isveren_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.tehlike_sinifi_isveren_yuzdesi    = QVariant ( attrs.value("tehlike_sinifi_isveren_yuzdesi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("ssk_isveren_taban_deger") ) {
                          YENI_SABIT_DEGERLER.ssk_isveren_taban_deger           = QVariant ( attrs.value("ssk_isveren_taban_deger").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("ssk_isveren_tavan_deger") ) {
                          YENI_SABIT_DEGERLER.ssk_isveren_tavan_deger           = QVariant ( attrs.value("ssk_isveren_tavan_deger").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("prs_sgdp_primi_isci_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.prs_sgdp_primi_isci_yuzdesi        = QVariant ( attrs.value("prs_sgdp_primi_isci_yuzdesi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("prs_sgdp_primi_isveren_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.prs_sgdp_primi_isveren_yuzdesi        = QVariant ( attrs.value("prs_sgdp_primi_isveren_yuzdesi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("muh_fis_hesaplari_birlestir") ) {
                          YENI_SABIT_DEGERLER.muh_fis_hesaplari_birlestir       = QVariant ( attrs.value("muh_fis_hesaplari_birlestir").toString() ).toInt();
                      }
                      if ( attrs.hasAttribute("isy_5510_nolu_kanuna_tabi_mi") ) {
                          YENI_SABIT_DEGERLER.isy_5510_nolu_kanuna_tabi_mi       = QVariant ( attrs.value("isy_5510_nolu_kanuna_tabi_mi").toString() ).toInt();
                      }
                      if ( attrs.hasAttribute("isy_5510_nolu_kanun_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.isy_5510_nolu_kanun_yuzdesi       = QVariant ( attrs.value("isy_5510_nolu_kanun_yuzdesi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("haftalik_calisilan_gun_sayisi") ) {
                          YENI_SABIT_DEGERLER.haftalik_calisilan_gun_sayisi       = QVariant ( attrs.value("haftalik_calisilan_gun_sayisi").toString() ).toDouble();
                      }
                      if ( attrs.hasAttribute("gunluk_yemek_yardimi_yuzdesi") ) {
                          YENI_SABIT_DEGERLER.gunluk_yemek_yardimi_yuzdesi       = QVariant ( attrs.value("gunluk_yemek_yardimi_yuzdesi").toString() ).toDouble();
                      }
                   }
               }
           }


        m_ui->comma_edit_isszlk_sig_isci_yuzdesi->SET_DOUBLE        ( YENI_SABIT_DEGERLER.issizlik_sig_isci_yuzdesi     * 100);
        m_ui->comma_edit_isszlk_sig_isveren_yuzdesi->SET_DOUBLE     ( YENI_SABIT_DEGERLER.issizlik_sig_isveren_yuzdesi  * 100);
        m_ui->comma_edit_sakatlik_indirimi_derece_1->SET_DOUBLE     ( YENI_SABIT_DEGERLER.sakatlik_ind_derece_1_tutari);
        m_ui->comma_edit_sakatlik_indirimi_derece_2->SET_DOUBLE     ( YENI_SABIT_DEGERLER.sakatlik_ind_derece_2_tutari);
        m_ui->comma_edit_sakatlik_indirimi_derece_3->SET_DOUBLE     ( YENI_SABIT_DEGERLER.sakatlik_ind_derece_3_tutari);
        m_ui->combo_box_tehlike_sinifi->setCurrentIndex(
        m_ui->combo_box_tehlike_sinifi->findText                    ( QVariant ( YENI_SABIT_DEGERLER.tehlike_sinifi ).toString()));
        m_ui->comma_edit_ssk_primi_isci_yuzdesi->SET_DOUBLE         ( YENI_SABIT_DEGERLER.ssk_primi_isci_yuzdesi    * 100);
        m_ui->comma_edit_ssk_primi_isveren_yuzdesi->SET_DOUBLE      ( YENI_SABIT_DEGERLER.ssk_primi_isveren_yuzdesi * 100);
        m_ui->comma_edit_ssk_taban->SET_DOUBLE                      ( YENI_SABIT_DEGERLER.ssk_taban_degeri);
        m_ui->comma_edit_ssk_tavan->SET_DOUBLE                      ( YENI_SABIT_DEGERLER.ssk_tavan_degeri);
        m_ui->comma_edit_16_yas_alti_asg_ucret->SET_DOUBLE          ( YENI_SABIT_DEGERLER.ssk_16_yas_alti_asgari_ucret);
        m_ui->comma_edit_16_yas_ustu_asg_ucret->SET_DOUBLE          ( YENI_SABIT_DEGERLER.ssk_16_yas_ustu_asgari_ucret);
        m_ui->comma_edit_damga_vergisi_orani->SET_DOUBLE            ( YENI_SABIT_DEGERLER.damga_vergisi * 100);
        m_ui->comma_edit_ssk_isveren_taban->SET_DOUBLE              ( YENI_SABIT_DEGERLER.ssk_isveren_taban_deger );
        m_ui->comma_edit_ssk_isveren_tavan->SET_DOUBLE              ( YENI_SABIT_DEGERLER.ssk_isveren_tavan_deger );
        m_ui->commaEdit_prs_sgdp_isci_yuzdesi->SET_DOUBLE           ( YENI_SABIT_DEGERLER.prs_sgdp_primi_isci_yuzdesi * 100 );
        m_ui->commaEdit_prs_sgdp_isveren_yuzdesi->SET_DOUBLE        ( YENI_SABIT_DEGERLER.prs_sgdp_primi_isveren_yuzdesi * 100 );
        m_ui->comboBox_muh_fis_hesaplari_birlestir->setCurrentIndex ( YENI_SABIT_DEGERLER.muh_fis_hesaplari_birlestir );

        m_ui->comma_edit_gelir_vergisi_dilimi_bas_1->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[0]  );
        m_ui->comma_edit_gelir_vergisi_dilimi_bit_1->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[0] );
        m_ui->comma_edit_gelir_vergisi_orani_1->SET_DOUBLE          ( YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[0] * 100 );

        m_ui->comma_edit_gelir_vergisi_dilimi_bas_2->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[1]);
        m_ui->comma_edit_gelir_vergisi_dilimi_bit_2->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[1] );
        m_ui->comma_edit_gelir_vergisi_orani_2->SET_DOUBLE          ( YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[1] * 100 );

        m_ui->comma_edit_gelir_vergisi_dilimi_bas_3->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[2]);
        m_ui->comma_edit_gelir_vergisi_dilimi_bit_3->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[2]);
        m_ui->comma_edit_gelir_vergisi_orani_3->SET_DOUBLE          ( YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[2] * 100 );

        m_ui->comma_edit_gelir_vergisi_dilimi_bas_4->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[3]  );
        m_ui->comma_edit_gelir_vergisi_dilimi_bit_4->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[3] );
        m_ui->comma_edit_gelir_vergisi_orani_4->SET_DOUBLE          ( YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[3] * 100 );

        m_ui->comma_edit_gelir_vergisi_dilimi_bas_5->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[4]  );
        m_ui->comma_edit_gelir_vergisi_dilimi_bit_5->SET_DOUBLE     ( YENI_SABIT_DEGERLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[4]);
        m_ui->comma_edit_gelir_vergisi_orani_5->SET_DOUBLE          ( YENI_SABIT_DEGERLER.GELIR_VERGISI_YUZDELERI[4] * 100 );


        m_ui->comma_edit_evli_es_calismiyor_cocuk_yok->SET_DOUBLE   ( YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI    [ PRS_COCUK_YOK ] );
        m_ui->comma_edit_evli_es_calismiyor_1_cocuk->SET_DOUBLE     ( YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI    [ PRS_TEK_COCUK ] );
        m_ui->comma_edit_evli_es_calismiyor_2_cocuk->SET_DOUBLE     ( YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI    [ PRS_IKI_COCUK ] );
        m_ui->comma_edit_evli_es_calismiyor_3_cocuk->SET_DOUBLE     ( YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI    [ PRS_UC_COCUK  ] );
        m_ui->comma_edit_evli_es_calismiyor_4_cocuk->SET_DOUBLE     ( YENI_SABIT_DEGERLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI    [ PRS_DORT_COCUK] );

        m_ui->comma_edit_evli_es_calisiyor_cocuk_yok->SET_DOUBLE    ( YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI     [ PRS_COCUK_YOK ] );
        m_ui->comma_edit_evli_es_calisiyor_1_cocuk->SET_DOUBLE      ( YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI     [ PRS_TEK_COCUK ] );
        m_ui->comma_edit_evli_es_calisiyor_2_cocuk->SET_DOUBLE      ( YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI     [ PRS_IKI_COCUK ] );
        m_ui->comma_edit_evli_es_calisiyor_3_cocuk->SET_DOUBLE      ( YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI     [ PRS_UC_COCUK  ] );
        m_ui->comma_edit_evli_es_calisiyor_4_cocuk->SET_DOUBLE      ( YENI_SABIT_DEGERLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI     [ PRS_DORT_COCUK] );

        m_ui->comma_edit_bekar_cocuk_yok->SET_DOUBLE                ( YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI                 [ PRS_COCUK_YOK ] );
        m_ui->comma_edit_bekar_1_cocuk->SET_DOUBLE                  ( YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI                 [ PRS_TEK_COCUK ] );
        m_ui->comma_edit_bekar_2_cocuk->SET_DOUBLE                  ( YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI                 [ PRS_IKI_COCUK ] );
        m_ui->comma_edit_bekar_3_cocuk->SET_DOUBLE                  ( YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI                 [ PRS_UC_COCUK  ] );
        m_ui->comma_edit_bekar_4_cocuk->SET_DOUBLE                  ( YENI_SABIT_DEGERLER.BEKAR_ASG_GEC_INDIRIMLERI                 [ PRS_DORT_COCUK] );


        m_ui->commaEdit_isci_tehlike_sinifi->SET_DOUBLE             ( YENI_SABIT_DEGERLER.tehlike_sinifi_isci_yuzdesi   * 100 );
        m_ui->commaEdit_isveren_tehlike_sinifi->SET_DOUBLE          ( YENI_SABIT_DEGERLER.tehlike_sinifi_isveren_yuzdesi* 100 );

        m_ui->combo_box_ihbar_taz_bas_suresi_1->setCurrentIndex     ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].baslangic_tarihi_turu );
        m_ui->combo_box_ihbar_taz_bas_suresi_2->setCurrentIndex     ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].baslangic_tarihi_turu );
        m_ui->combo_box_ihbar_taz_bas_suresi_3->setCurrentIndex     ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].baslangic_tarihi_turu );
        m_ui->combo_box_ihbar_taz_bas_suresi_4->setCurrentIndex     ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].baslangic_tarihi_turu );

        m_ui->comma_edit_ihbar_taz_bas_suresi_1->SET_DOUBLE         ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].baslangic_tarihi_orj_degeri);
        m_ui->comma_edit_ihbar_taz_bas_suresi_2->SET_DOUBLE         ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].baslangic_tarihi_orj_degeri);
        m_ui->comma_edit_ihbar_taz_bas_suresi_3->SET_DOUBLE         ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].baslangic_tarihi_orj_degeri);
        m_ui->comma_edit_ihbar_taz_bas_suresi_4->SET_DOUBLE         ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].baslangic_tarihi_orj_degeri);

        m_ui->combo_box_ihbar_taz_bit_suresi_1->setCurrentIndex     ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].bitis_tarihi_turu );
        m_ui->combo_box_ihbar_taz_bit_suresi_2->setCurrentIndex     ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].bitis_tarihi_turu );
        m_ui->combo_box_ihbar_taz_bit_suresi_3->setCurrentIndex     ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].bitis_tarihi_turu );
        m_ui->combo_box_ihbar_taz_bit_suresi_4->setCurrentIndex     ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].bitis_tarihi_turu );

        m_ui->comma_edit_ihbar_taz_bit_suresi_1->SET_DOUBLE         ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].bitis_tarihi_orj_degeri);
        m_ui->comma_edit_ihbar_taz_bit_suresi_2->SET_DOUBLE         ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].bitis_tarihi_orj_degeri);
        m_ui->comma_edit_ihbar_taz_bit_suresi_3->SET_DOUBLE         ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].bitis_tarihi_orj_degeri);
        m_ui->comma_edit_ihbar_taz_bit_suresi_4->SET_DOUBLE         ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].bitis_tarihi_orj_degeri);

        m_ui->comma_edit_ihbar_taz_tutari_1->SET_DOUBLE             ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 0 ].tazminat_hafta_degeri);
        m_ui->comma_edit_ihbar_taz_tutari_2->SET_DOUBLE             ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 1 ].tazminat_hafta_degeri);
        m_ui->comma_edit_ihbar_taz_tutari_3->SET_DOUBLE             ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 2 ].tazminat_hafta_degeri);
        m_ui->comma_edit_ihbar_taz_tutari_4->SET_DOUBLE             ( YENI_SABIT_DEGERLER.IHBAR_TAZMINATI_TUTARLARI[ 3 ].tazminat_hafta_degeri);

        m_ui->commaEdit_isy_5510_nolu_kanun_yuzdesi->SET_DOUBLE     ( YENI_SABIT_DEGERLER.isy_5510_nolu_kanun_yuzdesi );

        if ( YENI_SABIT_DEGERLER.isy_5510_nolu_kanuna_tabi_mi EQ 1 ) {
            m_ui->checkBox_isy_5510_nolu_kanuna_tabi_mi->setChecked( true );
        }
        else {
            m_ui->checkBox_isy_5510_nolu_kanuna_tabi_mi->setChecked( false );
        }

        m_ui->commaEdit_haftalik_calisilan_gun_sayisi->SET_DOUBLE   ( YENI_SABIT_DEGERLER.haftalik_calisilan_gun_sayisi );
        m_ui->commaEdit_gunluk_yemek_yardimi_yuzdesi->SET_DOUBLE    ( YENI_SABIT_DEGERLER.gunluk_yemek_yardimi_yuzdesi);

        return ADAK_RECORD_CHANGED;
    }
    else {
        MSG_WARNING( tr("XML dosyasi okunamadi..."), NULL );
    }
    return ADAK_RECORD_UNCHANGED;
}

/***************************************************************************************
                   PRS_ODENEK_VE_KESINTILER_FORMU::SET_DEFAULT_ODENEK_VE_KESINTIKER
***************************************************************************************/

void PRS_SABIT_DEGERLER_FORMU::SET_DEFAULT_ODENEK_VE_KESINTIKER()
{

    m_ui->comma_edit_aile_yardimi_tutari->clear();
    m_ui->comma_edit_aile_yardimi_yuzdesi->clear();
    m_ui->comma_edit_cocuk_yardimi_tutari->clear();
    m_ui->comma_edit_cocuk_yardimi_yuzdesi->clear();
    m_ui->comma_edit_dogum_yardimi_tutari->clear();
    m_ui->comma_edit_dogum_yardimi_yuzdesi->clear();
    m_ui->comma_edit_evlenme_yardimi_tutari->clear();
    m_ui->comma_edit_evlenme_yardimi_yuzdesi->clear();
    m_ui->comma_edit_gorev_harcirahi_tutari->clear();
    m_ui->comma_edit_gorev_harcirahi_yuzdesi->clear();
    m_ui->comma_edit_ihbar_tazminati_tutari->clear();
    m_ui->comma_edit_ihbar_tazminati_yuzdesi->clear();
    m_ui->comma_edit_kidem_tazminati_tutari->clear();
    m_ui->comma_edit_kidem_tazminati_yuzdesi->clear();
    m_ui->comma_edit_olum_yardimi_tutari->clear();
    m_ui->comma_edit_olum_yardimi_yuzdesi->clear();
    m_ui->comma_edit_yakacak_yardimi_tutari->clear();
    m_ui->comma_edit_yakacak_yardimi_yuzdesi->clear();
    m_ui->comma_edit_yemek_parasi_tutari->clear();
    m_ui->comma_edit_yemek_parasi_yuzdesi->clear();

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_aile_yardimi_kimlere_verilir ,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_cocuk_yardimi_kimlere_verilir ,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_dogum_yardimi_kimlere_verilir ,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_evlenme_yardimi_kimlere_verilir ,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_gorev_harcirahi_kimlere_verilir ,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_olum_yardimi_kimlere_verilir ,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_yakacak_yardimi_kimlere_verilir ,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_yemek_parasi_kimlere_verilir ,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_ihbar_tazminati_kimlere_verilir,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_kidem_tazminati_kimlere_verilir ,
                               PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(PRS_TUM_PERSONELE_UYGULANIR));

    m_ui->combo_box_aile_yardimi_yuzde_mi->setCurrentIndex(m_ui->combo_box_aile_yardimi_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));
    m_ui->combo_box_cocuk_yardimi_yuzde_mi->setCurrentIndex(m_ui->combo_box_cocuk_yardimi_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));
    m_ui->combo_box_dogum_yardimi_yuzde_mi->setCurrentIndex(m_ui->combo_box_dogum_yardimi_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));
    m_ui->combo_box_evlenme_yardimi_yuzde_mi->setCurrentIndex(m_ui->combo_box_evlenme_yardimi_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));
    m_ui->combo_box_gorev_harcirahi_yuzde_mi->setCurrentIndex(m_ui->combo_box_gorev_harcirahi_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));
    m_ui->combo_box_ihbar_tazminati_yuzde_mi->setCurrentIndex(m_ui->combo_box_ihbar_tazminati_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));
    m_ui->combo_box_kidem_tazminati_yuzde_mi->setCurrentIndex(m_ui->combo_box_kidem_tazminati_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));
    m_ui->combo_box_olum_yardimi_yuzde_mi->setCurrentIndex(m_ui->combo_box_olum_yardimi_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));
    m_ui->combo_box_yakacak_yardimi_yuzde_mi->setCurrentIndex(m_ui->combo_box_yakacak_yardimi_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));
    m_ui->combo_box_yemek_parasi_yuzde_mi->setCurrentIndex(m_ui->combo_box_yemek_parasi_yuzde_mi->findText(PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(PRS_TUTAR)));




    m_ui->group_box_ihbar_tazminati->setToolTip( tr("<font color = darkred> İHBAR TAZMİNATI Tüm Personele Şartları Sağlaması Halinde Kanunen Verilmelidir."));
    m_ui->combo_box_ihbar_tazminati_kimlere_verilir->setToolTip( tr("<font color = darkred> İHBAR TAZMİNATI Tüm Personele Şartları Sağlaması Halinde Kanunen Verilmelidir."));
    m_ui->group_box_kidem_tazminati->setToolTip( tr("<font color = darkred> KIDEM TAZMİNATI Tüm Personele Şartları Sağlaması Halinde Kanunen Verilmelidir."));
    m_ui->combo_box_kidem_tazminati_kimlere_verilir->setToolTip( tr("<font color = darkred> KIDEM TAZMİNATI Tüm Personele Şartları Sağlaması Halinde Kanunen Verilmelidir."));

    m_ui->group_box_ihbar_tazminati->setEnabled(false);
    m_ui->group_box_kidem_tazminati->setEnabled(false);

    m_ui->combo_box_ihbar_tazminati_kimlere_verilir->setEnabled(false);
    m_ui->combo_box_kidem_tazminati_kimlere_verilir->setEnabled(false);

    m_ui->check_box_ihbar_tazminati_ssk_kesintisi->setChecked(false);
    m_ui->check_box_ihbar_tazminati_gelir_vergisi->setChecked(true);
    m_ui->check_box_ihbar_tazminati_damga_vergisi->setChecked(true);

    m_ui->check_box_kidem_tazminati_ssk_kesintisi->setChecked(false);
    m_ui->check_box_kidem_tazminati_gelir_vergisi->setChecked(false);
    m_ui->check_box_kidem_tazminati_damga_vergisi->setChecked(true);

}

/***************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::HIDE_OR_SHOW_GROUP_BOX
***************************************************************************************/

void PRS_SABIT_DEGERLER_FORMU::HIDE_OR_SHOW_GROUP_BOX( QComboBox* combo_box_odenek, QGroupBox* group_box_odenek )
{

    if ( ( combo_box_odenek->currentIndex() EQ 0 ) OR
         ( combo_box_odenek->currentIndex() EQ 1 ) ) {

        group_box_odenek->setEnabled(true);
    }
    else if ( combo_box_odenek->currentIndex() EQ 2 ) {

        group_box_odenek->setEnabled(false);
    }
}

/**************************************************************************************
                   PRS_SABIT_DEGERLER_FORMU::ODENEK_KALDIRILSIN_MI
**************************************************************************************/

int PRS_SABIT_DEGERLER_FORMU::ODENEK_KALDIRILSIN_MI( QComboBox* combo_box )
{
    int       odenek_kaldirilsin_mi;

    if ( combo_box->currentIndex() EQ 2 ) {

        odenek_kaldirilsin_mi = MSG_YES_NO_CANCEL( tr("Personel Ödeneği <b>Kaldırılıyor</b>, Devam Edilsin Mi ?"),
                                           NULL );

        if ( odenek_kaldirilsin_mi EQ ADAK_NO OR odenek_kaldirilsin_mi EQ ADAK_CANCEL ) {

            combo_box->setCurrentIndex(0);
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}
