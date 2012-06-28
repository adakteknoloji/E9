#include "adak_sql.h"
#include "muh_console_utils.h"
#include "ui_sube_entegrasyon_hesaplari_formu.h"
#include "sube_entegrasyon_hesaplari_formu_class.h"
 
#include "e9_log.h"
 
#include "muh_hesap_arama_open.h"
#include "muh_enum.h"
#include "muh_struct.h"

extern ADAK_SQL * DB;

#define ONTANIMLI_ENTEGRASYON_HESAPLARI_SAYISI 59

/**************************************************************************************
                   OPEN_SUBE_ENTEGRASYON_HESAPLARI_FORMU
***************************************************************************************/

void OPEN_SUBE_ENTEGRASYON_HESAPLARI_FORMU ( int p_sube_id, QWidget * p_parent )
{
    SUBE_ENTEGRASYON_HESAPLARI_FORMU * F = new SUBE_ENTEGRASYON_HESAPLARI_FORMU ( p_sube_id, p_parent );
    F->EXEC( FULL_SCREEN );

}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::SUBE_ENTEGRASYON_HESAPLARI_FORMU
***************************************************************************************/

SUBE_ENTEGRASYON_HESAPLARI_FORMU::SUBE_ENTEGRASYON_HESAPLARI_FORMU ( int p_sube_id, QWidget * p_parent) : FORM_KERNEL ( p_parent ), m_ui ( new Ui::SUBE_ENTEGRASYON_HESAPLARI_FORMU )
{
    m_sube_id = p_sube_id;

    m_ui->setupUi               ( this );
    START_FORM_KERNEL           ( this, DB );
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::SETUP_FORM
***************************************************************************************/

void SUBE_ENTEGRASYON_HESAPLARI_FORMU::SETUP_FORM()
{
    SET_HELP_PAGE       ( "sube-entegrasyon-hesaplari" );
    SET_PAGE_TITLE      ( tr ( "ŞUBE - ENTEGRASYON HESAPLARI" ) );

    SET_SETTING_NAME    ( "SUBE_ENT_HESAPLARI_FORMU" );
    m_ui->lineEdit_personel_giderleri_hesap_ismi->setMaxLength (100);
     

    SET_SINGLE_RECORD_ID        ( m_sube_id );
    SET_FIRST_FOCUS_WIDGET      ( m_ui->searchEdit_portfoydeki_cekler );

    REGISTER_BUTTONS_WIDGET     ( m_ui->navigation_buttons_widget );

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_prs_giderleri_hesabi);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_personel_giderleri_hesap_ismi);

    WIDGET_DIZILERINI_DUZENLE();

}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void SUBE_ENTEGRASYON_HESAPLARI_FORMU::CLEAR_FORM_MEMBERS()
{

}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::NEW_RECORD
***************************************************************************************/

void  SUBE_ENTEGRASYON_HESAPLARI_FORMU::NEW_RECORD()
{
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::GET_RECORD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::GET_RECORD ( int p_sube_id )
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_subeler","sube_port_cekler_hsp_id, sube_tah_cekler_hsp_id, sube_tem_cekler_hsp_id, "
                               "sube_kar_cekler_hsp_id, sube_port_snt_hsp_id, "
                               "sube_tah_snt_hsp_id, sube_tem_snt_hsp_id, sube_pro_snt_hsp_id, "
                               "sube_port_snt_ileri_trh_hsp_id, sube_tah_snt_ileri_trh_hsp_id, "
                               "sube_tem_snt_ileri_trh_hsp_id, sube_borc_snt_hsp_id, "
                               "sube_borc_snt_ileri_trh_hsp_id, sube_sup_alacaklar_hsp_id, "
                               "sube_tah_edlmyn_cekler_hsp_id, sube_tah_edlmyn_snt_hsp_id, "
                               "sube_yurtici_satis_hsp_id, sube_hizmet_urt_maliyet_hsp_id, "
                               "sube_prs_gdrlr_hsp_id,           "
                               "sube_sarf_malzemeleri_hsp_id,sube_imalattan_giris_hsp_id  ,"
                               "kasa_ent_hesap_id , cari_alici_ent_hesap_id, cari_satici_ent_hesap_id , banka_ent_hesap_id ,"
                               "ver_cek_ent_hesap_id , odeme_emirleri_ent_hesap_id         ,"
                               "pos_ent_hesap_id , kk_ent_hesap_id ,                        "
                               "prk_gider_ent_hesap_id , urunler_ent_hesap_id ,             "
                               "hizmetler_ent_hesap_id,                                     "
                               "demirbaslar_ent_hesap_id, pos_kom_gideri_ent_hesap_id,      "
                               "prs_avanslari_ent_hesap_id ,                                "
                               "personel_borclari_ent_hesap_id ,                            "
                               "prs_damga_vergisi_ent_hesap_id ,                            "
                               "personel_vergi_ent_hesap_id,                                "
                               "prk_gelir_ent_hesap_id,                                     "
                               "prs_ssk_isci_payi_hesap_id,                                 "
                               "prs_ssk_isveren_payi_hesap_id,                              "
                               "prs_issizlik_sgk_isci_hsp_id,                               "
                               "prs_issizlik_sgk_isv_hsp_id,                                "
                               "asg_gecim_ind_hesap_id,                                     "
                               "isveren_payi_giderleri_hps_id,                              "
                               "bir_amortismanlar_hsp_id,                                   "
                               "amor_giderleri_hsp_id,                                      "
                               "sgdp_prim_yuzde_isc_hsp_id,                                 "
                               "sgdp_prim_yuzde_isv_hsp_id,                                 "
                               "kidem_tazminati_hesap_id,                                   "
                               "ihbar_tazminati_hesap_id,                                   "
                               "dmr_satis_kar_ent_hesap_id,                                 "
                               "dmr_satis_zarar_ent_hesap_id,                               "
                               "sendika_odeme_hesap_id,                                     "
                               "smmm_alinan_hizmet_hesap_id,                                "
                               "smmm_gelir_vergisi_hesap_id,                                "
                               "smmm_satilan_hizmet_hesap_id,                               "
                               "isy_5510_nolu_kanun_hesap_id,                               "
                               "satistan_iade_hesap_id,                                     "
                               "ortaklara_borclar_hesap_id                                  ",
                               " sube_id = :sube_id" );
    sql_query.SET_VALUE      ( ":sube_id", p_sube_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }
    sql_query.NEXT();

    QString muh_hesap_kodu;
    QString muh_hesap_ismi;

    MUH_GET_HESAP_KODU_HESAP_ISMI (sql_query.VALUE("sube_prs_gdrlr_hsp_id").toInt() ,muh_hesap_kodu ,muh_hesap_ismi);
    m_ui->searchEdit_prs_giderleri_hesabi->SET_TEXT (muh_hesap_kodu);
    m_ui->lineEdit_personel_giderleri_hesap_ismi->setText (muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_port_cekler_hsp_id").toInt() , muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_portfoydeki_cekler->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_port_cekler_hesap_ismi->setText ( muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_tah_cekler_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_tah_ver_cekler->SET_TEXT ( muh_hesap_kodu);
    m_ui->lineedit_tah_cekler_hesap_ismi->setText ( muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_tem_cekler_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_tem_ver_cekler->SET_TEXT ( muh_hesap_kodu);
    m_ui->lineedit_tem_cekler_hesap_ismi->setText ( muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_kar_cekler_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_kar_cekler->SET_TEXT ( muh_hesap_kodu);
    m_ui->lineedit_karsiliksiz_cekler_hesap_ismi->setText ( muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI (  sql_query.VALUE("sube_port_snt_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_port_senetler->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_port_senetler_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_tah_snt_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_tah_ver_senetler->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_tah_senetler_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_tem_snt_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_tem_ver_senetler->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_tem_senetler_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_pro_snt_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_pro_senetler->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_pro_senetler_hesap_ismi->setText ( muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI( sql_query.VALUE("sube_port_snt_ileri_trh_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_port_senetler_ileri_tarih->SET_TEXT ( muh_hesap_kodu);
    m_ui->lineedit_port_senetler_ileri_tarih_hesap_ismi->setText ( muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_tah_snt_ileri_trh_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_tah_ver_senetler_ileri_tarih->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_tah_senetler_ileri_tarih_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_tem_snt_ileri_trh_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_tem_ver_senetler_ileri_tarih->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_tem_senetler_ileri_tarih_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_borc_snt_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_borc_senetleri->SET_TEXT ( muh_hesap_kodu);
    m_ui->lineedit_borc_senetler_hesap_ismi->setText ( muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI( sql_query.VALUE("sube_borc_snt_ileri_trh_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_borc_senetleri_ileri_tarih->SET_TEXT ( muh_hesap_kodu);
    m_ui->lineedit_borc_senetleri_ileri_tarih_hesap_ismi->setText ( muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_sup_alacaklar_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_sup_alacaklar->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_supheli_alacaklar_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_tah_edlmyn_cekler_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_tah_edilemeyen_cekler->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_tah_edilemeyen_cek_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI( sql_query.VALUE("sube_tah_edlmyn_snt_hsp_id").toInt(), muh_hesap_kodu, muh_hesap_ismi );
    m_ui->searchEdit_tah_edilemeyen_senetler->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineedit_tah_edilemeyen_snt_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_yurtici_satis_hsp_id").toInt(),muh_hesap_kodu,muh_hesap_ismi );
    m_ui->searchEdit_yurtici_sts_hesap_kodu->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineEdit_yurtici_sts_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_hizmet_urt_maliyet_hsp_id").toInt(),muh_hesap_kodu,muh_hesap_ismi );
    m_ui->searchEdit_hizmet_uretim_maliyeti_hesap_kodu->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineEdit_hizmet_uretim_maliyeti_hesap_ismi->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_sarf_malzemeleri_hsp_id").toInt(),muh_hesap_kodu,muh_hesap_ismi );
    m_ui->searchEdit_sarf_malzemeleri->SET_TEXT ( muh_hesap_kodu );
    m_ui->lineEdit_sarf_malzemeleri->setText ( muh_hesap_ismi );

    MUH_GET_HESAP_KODU_HESAP_ISMI ( sql_query.VALUE("sube_imalattan_giris_hsp_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_imalattan_giris_hsp_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_imalattan_giris_hsp_ismi->setText(muh_hesap_ismi);


    // E9_SABITDEGERLEDEN GELEN

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("kasa_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_kasa_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_kasa_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("cari_alici_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_cari_alici_ent_hesap_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_cari_alici_ent_hesap_id->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("cari_satici_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_cari_satici_ent_hesap_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_cari_satici_ent_hesap_id->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("banka_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_banka_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_banka_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("ver_cek_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_ver_cekler_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_ver_cekler_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("odeme_emirleri_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_odeme_emirlari_muh_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi->setText(muh_hesap_ismi);


    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("pos_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_pos_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_pos_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("kk_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_kk_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_kk_hesap_kodu->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("prk_gider_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_prk_gider_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_prk_gider_hesap_ismi->setText(muh_hesap_ismi);


    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("urunler_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_urunler_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_urunler_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("hizmetler_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_hizmetler_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_hizmetler_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("demirbaslar_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_demirbaslar_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_demirbaslar_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("pos_kom_gideri_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_pos_kom_gideri_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_pos_kom_gideri_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("prs_avanslari_ent_hesap_id").toInt() ,muh_hesap_kodu ,muh_hesap_ismi);
    m_ui->searchEdit_prs_avanslar_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_prs_avanslar_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("personel_borclari_ent_hesap_id").toInt(),muh_hesap_kodu ,muh_hesap_ismi);
    m_ui->searchEdit_prs_borclar_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_prs_borclar_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("prs_damga_vergisi_ent_hesap_id").toInt(),muh_hesap_kodu ,muh_hesap_ismi);
    m_ui->searchEdit_prs_damga_vergisi_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_damga_vergisi_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("personel_vergi_ent_hesap_id").toInt(),muh_hesap_kodu,muh_hesap_ismi);
    m_ui->searchEdit_prs_vergi_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_vergi_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("prk_gelir_ent_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_prk_gelir_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_prk_gelir_hesap_ismi->setText(muh_hesap_ismi);


    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("prs_ssk_isci_payi_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_personel_ssk_prim_yuzdesi_isci_payi_hesabi->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_personel_ssk_prim_yuzdesi_isci_payi_hesabi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("prs_ssk_isveren_payi_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_personel_ssk_prim_yuzdesi_isveren_payi_hesabi->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_personel_ssk_prim_yuzdesi_isveren_payi_hesabi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("prs_issizlik_sgk_isci_hsp_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_personel_issizlik_sigortasi_isci_yuzdesi_hesabi->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_personel_issizlik_sigortasi_isci_yuzdesi_hesabi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("prs_issizlik_sgk_isv_hsp_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_personel_issizlik_sigortasi_isveren_yuzdesi_hesabi->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_personel_issizlik_sigortasi_isveren_yuzdesi_hesabi->setText(muh_hesap_ismi);


    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("asg_gecim_ind_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_personel_asgari_odeme_ent_hesabi->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_personel_asgari_odeme_ent_hesabi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("isveren_payi_giderleri_hps_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_isveren_paylari_gider_hesabi->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_isveren_paylari_gider_hesabi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("bir_amortismanlar_hsp_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_bir_amortismanlar_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_bir_amortismanlar_hesap_ismi->setText(muh_hesap_ismi);


    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("amor_giderleri_hsp_id").toInt() , muh_hesap_kodu , muh_hesap_ismi );
    m_ui->searchEdit_amor_giderleri_hsp_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_amor_giderleri_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("sgdp_prim_yuzde_isc_hsp_id").toInt() , muh_hesap_kodu , muh_hesap_ismi );
    m_ui->searchEdit_prs_sgdp_prim_yuzde_isc_hsp_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_prs_sgdp_prim_yuzde_isc_hsp_id->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("sgdp_prim_yuzde_isv_hsp_id").toInt() , muh_hesap_kodu , muh_hesap_ismi );
    m_ui->searchEdit_prs_sgdp_prim_yuzde_isv_hsp_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_prs_sgdp_prim_yuzde_isv_hsp_id->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("kidem_tazminati_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi );
    m_ui->searchEdit_kidem_tazminati_hesap_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_kidem_tazminati_hesap_id->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("ihbar_tazminati_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi );
    m_ui->searchEdit_ihbar_tazminati_hesap_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_ihbar_tazminati_hesap_id->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("dmr_satis_kar_ent_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi );
    m_ui->searchEdit_dmr_satis_kar_hesabi->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_dmr_satis_kar_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("dmr_satis_zarar_ent_hesap_id").toInt() , muh_hesap_kodu ,muh_hesap_ismi);
    m_ui->searchEdit_dmr_satis_zarar_hesabi->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_dmr_satis_zarar_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("sendika_odeme_hesap_id").toInt() , muh_hesap_kodu ,muh_hesap_ismi);
    m_ui->searchEdit_sendika_odeme_hesap_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_sendika_odeme_hesap_id->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("smmm_alinan_hizmet_hesap_id").toInt() ,muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_smmm_al_hizmet_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_smmm_al_hizmet_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("smmm_satilan_hizmet_hesap_id").toInt() ,muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_smmm_sat_hizmet_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_smmm_sat_hizmet_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("smmm_gelir_vergisi_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_smmm_gelir_vergisi_hesap_kodu->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_smmm_gelir_vergisi_hesap_ismi->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("isy_5510_nolu_kanun_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_isy_5510_nolu_kanun_hesap_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_isy_5510_nolu_kanun_hesap_id->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("satistan_iade_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_satistan_iade_hesap_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_satistan_iade_hesap_id->setText(muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(sql_query.VALUE("ortaklara_borclar_hesap_id").toInt() , muh_hesap_kodu , muh_hesap_ismi);
    m_ui->searchEdit_ortaklara_borclar_hesap_id->SET_TEXT(muh_hesap_kodu);
    m_ui->lineEdit_ortaklara_borclar_hesap_id->setText(muh_hesap_ismi);

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_VAR
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_VAR ( QObject * p_object )
{

    for ( int i = 0 ; i < ONTANIMLI_ENTEGRASYON_HESAPLARI_SAYISI ; i++ ) {

        if ( p_object EQ M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY[i] ) {

            if ( M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
                M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY[i]->clear();
                return ADAK_OK;
            }
            MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

            MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

            if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY[i]->GET_TEXT()) EQ 0 ) {
                MSG_WARNING(  tr ( "Muhasebe hesabı bulunamadı." ), M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {
                MSG_INFO("Aradıgınız hesap tali değil.Lütfen değiştiriniz." , M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY[i]);
                return ADAK_FAIL_UNDO;
            }
            M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY[i]->setText(MUHASEBE_HESABI->hesap_ismi);
            return ADAK_OK;
        }
    }

  return ADAK_OK;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_EMPTY
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->searchEdit_yurtici_sts_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Yurtiçi Satışlar hesabını boş bırakamazsınız.",m_ui->searchEdit_yurtici_sts_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_hizmet_uretim_maliyeti_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Hizmet Üretim Maliyeti hesabını boş bırakamazsınız." , m_ui->searchEdit_yurtici_sts_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_prs_giderleri_hesabi->GET_TEXT().isEmpty() EQ true ){
        MSG_WARNING( " Personel Giderleri Hesabını boş bırakamazsınız." , m_ui->searchEdit_prs_giderleri_hesabi);
    }
    if ( m_ui->searchEdit_portfoydeki_cekler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Portföydeki Çekler hesabını boş bırakamazsınız." , m_ui->searchEdit_portfoydeki_cekler);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_tah_ver_cekler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Tahsile Verilen Çekler hesabını boş bırakamazsınız." , m_ui->searchEdit_tah_ver_cekler);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_tem_ver_cekler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Teminata Verilen Çekler hesabını boş bırakamazsınız." , m_ui->searchEdit_tem_ver_cekler);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_kar_cekler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Karşılıksız Çekler hesabını boş bırakamazsınız." , m_ui->searchEdit_kar_cekler);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_port_senetler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Portföydeki Senetler hesabını boş bırakamazsınız." , m_ui->searchEdit_port_senetler);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_tah_ver_senetler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Tahsile Verilen Senetler hesabını boş bırakamazsınız." , m_ui->searchEdit_tah_ver_senetler);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_tem_ver_senetler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Teminata Verilen Senetler hesabını boş bırakamazsınız." , m_ui->searchEdit_tem_ver_senetler);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_pro_senetler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Protestolu Senetler hesabını boş bırakamazsınız." , m_ui->searchEdit_pro_senetler);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_port_senetler_ileri_tarih->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Vadesi 1 yıldan fazla olan Portföydeki Senetler hesabını boş bırakamazsınız." , m_ui->searchEdit_port_senetler_ileri_tarih);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_tah_ver_senetler_ileri_tarih->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Vadesi 1 yıldan fazla olan Tahsile Verilen Senetler hesabını boş bırakamazsınız." , m_ui->searchEdit_tah_ver_senetler_ileri_tarih);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_tem_ver_senetler_ileri_tarih->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Vadesi 1 yıldan fazla olan Teminata Verilen Senetler hesabını boş bırakamazsınız." , m_ui->searchEdit_tem_ver_senetler_ileri_tarih);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_borc_senetleri->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Borç Senetleri hesabını boş bırakamazsınız." , m_ui->searchEdit_borc_senetleri);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_borc_senetleri_ileri_tarih->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Vadesi 1 yıldan fazla olan Borç Senetleri hesabını boş bırakamazsınız." , m_ui->searchEdit_borc_senetleri_ileri_tarih);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_sup_alacaklar->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Şüpheli Alacaklar hesabını boş bırakamazsınız." , m_ui->searchEdit_sup_alacaklar);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_tah_edilemeyen_cekler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Tahsil Edilemeyen Çekler  hesabını boş bırakamazsınız." , m_ui->searchEdit_tah_edilemeyen_cekler);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_tah_edilemeyen_senetler->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Tahsil Edilemeyen Senetler hesabını boş bırakamazsınız." , m_ui->searchEdit_tah_edilemeyen_senetler);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_imalattan_giris_hsp_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "İmalattan Giriş hesabını boş bırakamazsınız." , m_ui->searchEdit_imalattan_giris_hsp_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_kasa_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Kasa Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_kasa_hesap_kodu);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_cari_alici_ent_hesap_id->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Cari Alıcı Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_cari_alici_ent_hesap_id);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_cari_satici_ent_hesap_id->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Cari Satıcı Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_cari_satici_ent_hesap_id);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_banka_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Banka Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_banka_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_ver_cekler_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Verilen Çekler Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_ver_cekler_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_pos_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Pos Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_pos_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_pos_kom_gideri_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Pos Komisyon Gideri Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_pos_kom_gideri_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_kk_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Kredi Kartı Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_kk_hesap_kodu);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_prk_gider_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Perakende Giderler Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_prk_gider_hesap_kodu);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_urunler_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Ürünler Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_urunler_hesap_kodu);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_hizmetler_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Hizmetler Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_hizmetler_hesap_kodu);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_demirbaslar_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Demirbaşlar Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_demirbaslar_hesap_kodu);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_bir_amortismanlar_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Birikmiş Amortismanlar Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_bir_amortismanlar_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_amor_giderleri_hsp_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Amortisman Giderleri Entegrasyon Hesabını boş bırakamazsınız."),m_ui->searchEdit_amor_giderleri_hsp_kodu);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_prs_avanslar_hesap_kodu->GET_TEXT().isEmpty() EQ true){
        MSG_INFO(QObject::tr("Personel Avansları Entegrasyon Hesabini boş birakamazsiniz."),m_ui->searchEdit_prs_avanslar_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_prs_borclar_hesap_kodu->GET_TEXT().isEmpty()EQ true){
        MSG_INFO(QObject::tr("Personele Borçlar Entegrasyon Hesabini boş birakamazsiniz."),m_ui->searchEdit_prs_borclar_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_prs_damga_vergisi_hesap_kodu->GET_TEXT().isEmpty() EQ true){
        MSG_INFO(QObject::tr("Personel Damga Vergisi Entegrasyon Hesabini boş birakamazsiniz."),m_ui->searchEdit_prs_damga_vergisi_hesap_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_prs_vergi_hesap_kodu->GET_TEXT().isEmpty() EQ true){
        MSG_INFO(QObject::tr("Personel Ödenecek Vergi ve Fonları Entegrasyon Hesabini boş birakamazsiniz."),m_ui->searchEdit_prs_vergi_hesap_kodu);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_demirbaslar_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Sabit Kıymet Satış Karı Entegrasyon Hesabini boş birakamazsiniz."),m_ui->searchEdit_dmr_satis_kar_hesabi);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_isy_5510_nolu_kanun_hesap_id->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("5510 nolu kanun Entegrasyon Hesabini boş birakamazsiniz."),m_ui->searchEdit_isy_5510_nolu_kanun_hesap_id);
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_satistan_iade_hesap_id->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Satıştan İndirim Entegrasyon Hesabini boş birakamazsiniz."), m_ui->searchEdit_satistan_iade_hesap_id );
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_ortaklara_borclar_hesap_id->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Satıştan İndirim Entegrasyon Hesabini boş birakamazsiniz."), m_ui->searchEdit_ortaklara_borclar_hesap_id );
        return ADAK_FAIL;
    }


    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_ADD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::ADD_RECORD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::ADD_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_UPDATE
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_UPDATE ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::UPDATE_RECORD
***************************************************************************************/

void SUBE_ENTEGRASYON_HESAPLARI_FORMU::UPDATE_RECORD ( int sube_id )
{

    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_UPDATE ( "sub_subeler","sube_id",
                               "sube_port_cekler_hsp_id             ,  "
                               "sube_tah_cekler_hsp_id              ,  "
                               "sube_tem_cekler_hsp_id              ,  "
                               "sube_kar_cekler_hsp_id              ,  "
                               "sube_port_snt_hsp_id                ,  "
                               "sube_tah_snt_hsp_id                 ,  "
                               "sube_tem_snt_hsp_id                 ,  "
                               "sube_pro_snt_hsp_id                 ,  "
                               "sube_port_snt_ileri_trh_hsp_id      ,  "
                               "sube_tah_snt_ileri_trh_hsp_id       ,  "
                               "sube_tem_snt_ileri_trh_hsp_id       ,  "
                               "sube_borc_snt_hsp_id                ,  "
                               "sube_borc_snt_ileri_trh_hsp_id      ,  "
                               "sube_sup_alacaklar_hsp_id           ,  "
                               "sube_tah_edlmyn_cekler_hsp_id       ,  "
                               "sube_tah_edlmyn_snt_hsp_id          ,  "
                               "sube_yurtici_satis_hsp_id           ,  "
                               "sube_hizmet_urt_maliyet_hsp_id      ,  "
                               "sube_prs_gdrlr_hsp_id               ,  "
                               "sube_sarf_malzemeleri_hsp_id        ,  "
                               "sube_imalattan_giris_hsp_id         ,  "
                               "kasa_ent_hesap_id                   ,  " // e9_sabit degerler
                               "cari_alici_ent_hesap_id             ,  "
                               "cari_satici_ent_hesap_id            ,  "
                               "banka_ent_hesap_id                  ,  "
                               "ver_cek_ent_hesap_id                ,  "
                               "odeme_emirleri_ent_hesap_id         ,  "
                               "pos_ent_hesap_id                    ,  "
                               "kk_ent_hesap_id                     ,  "
                               "prk_gider_ent_hesap_id              ,  "
                               "urunler_ent_hesap_id                ,  "
                               "hizmetler_ent_hesap_id              ,  "
                               "demirbaslar_ent_hesap_id            ,  "
                               "pos_kom_gideri_ent_hesap_id         ,  "
                               "prs_avanslari_ent_hesap_id          ,  "
                               "personel_borclari_ent_hesap_id      ,  "
                               "prs_damga_vergisi_ent_hesap_id      ,  "
                               "personel_vergi_ent_hesap_id         ,  "
                               "prk_gelir_ent_hesap_id              ,  "
                               "prs_ssk_isci_payi_hesap_id          ,  "
                               "prs_ssk_isveren_payi_hesap_id       ,  "
                               "prs_issizlik_sgk_isci_hsp_id        ,  "
                               "prs_issizlik_sgk_isv_hsp_id         ,  "
                               "asg_gecim_ind_hesap_id              ,  "
                               "isveren_payi_giderleri_hps_id       ,  "
                               "bir_amortismanlar_hsp_id            ,  "
                               "amor_giderleri_hsp_id               ,  "
                               "sgdp_prim_yuzde_isc_hsp_id          ,  "
                               "sgdp_prim_yuzde_isv_hsp_id          ,  "
                               "kidem_tazminati_hesap_id            ,  "
                               "ihbar_tazminati_hesap_id            ,  "
                               "dmr_satis_kar_ent_hesap_id          ,  "
                               "dmr_satis_zarar_ent_hesap_id        ,  "
                               "sendika_odeme_hesap_id              ,  "
                               "smmm_alinan_hizmet_hesap_id         ,  "
                               "smmm_gelir_vergisi_hesap_id         ,  "
                               "smmm_satilan_hizmet_hesap_id        ,  "
                               "isy_5510_nolu_kanun_hesap_id        ,  "
                               "satistan_iade_hesap_id              ,  "
                               "ortaklara_borclar_hesap_id             ",
                               "sube_id                          = :sube_id" );

    sql_query.SET_VALUE     ( ":sube_port_cekler_hsp_id"                    , MUH_GET_HESAP_ID( m_ui->searchEdit_portfoydeki_cekler->GET_TEXT() )                              );
    sql_query.SET_VALUE     ( ":sube_tah_cekler_hsp_id"                     , MUH_GET_HESAP_ID( m_ui->searchEdit_tah_ver_cekler->GET_TEXT() )                                  );
    sql_query.SET_VALUE     ( ":sube_tem_cekler_hsp_id"                     , MUH_GET_HESAP_ID( m_ui->searchEdit_tem_ver_cekler->GET_TEXT() )                                  );
    sql_query.SET_VALUE     ( ":sube_kar_cekler_hsp_id"                     , MUH_GET_HESAP_ID( m_ui->searchEdit_kar_cekler->GET_TEXT() )                                      );
    sql_query.SET_VALUE     ( ":sube_port_snt_hsp_id"                       , MUH_GET_HESAP_ID( m_ui->searchEdit_port_senetler->GET_TEXT() )                                   );
    sql_query.SET_VALUE     ( ":sube_tah_snt_hsp_id"                        , MUH_GET_HESAP_ID( m_ui->searchEdit_tah_ver_senetler->GET_TEXT() )                                );
    sql_query.SET_VALUE     ( ":sube_tem_snt_hsp_id"                        , MUH_GET_HESAP_ID( m_ui->searchEdit_tem_ver_senetler->GET_TEXT() )                                );
    sql_query.SET_VALUE     ( ":sube_pro_snt_hsp_id"                        , MUH_GET_HESAP_ID( m_ui->searchEdit_pro_senetler->GET_TEXT() )                                    );
    sql_query.SET_VALUE     ( ":sube_port_snt_ileri_trh_hsp_id"             , MUH_GET_HESAP_ID( m_ui->searchEdit_port_senetler_ileri_tarih->GET_TEXT())                        );
    sql_query.SET_VALUE     ( ":sube_tah_snt_ileri_trh_hsp_id"              , MUH_GET_HESAP_ID( m_ui->searchEdit_tah_ver_senetler_ileri_tarih->GET_TEXT())                     );
    sql_query.SET_VALUE     ( ":sube_tem_snt_ileri_trh_hsp_id"              , MUH_GET_HESAP_ID( m_ui->searchEdit_tem_ver_senetler->GET_TEXT())                                 );
    sql_query.SET_VALUE     ( ":sube_borc_snt_hsp_id"                       , MUH_GET_HESAP_ID( m_ui->searchEdit_borc_senetleri->GET_TEXT())                                   );
    sql_query.SET_VALUE     ( ":sube_borc_snt_ileri_trh_hsp_id"             , MUH_GET_HESAP_ID( m_ui->searchEdit_borc_senetleri_ileri_tarih->GET_TEXT())                       );
    sql_query.SET_VALUE     ( ":sube_sup_alacaklar_hsp_id"                  , MUH_GET_HESAP_ID( m_ui->searchEdit_sup_alacaklar->GET_TEXT() )                                   );
    sql_query.SET_VALUE     ( ":sube_tah_edlmyn_cekler_hsp_id"              , MUH_GET_HESAP_ID( m_ui->searchEdit_tah_edilemeyen_cekler->GET_TEXT() )                           );
    sql_query.SET_VALUE     ( ":sube_tah_edlmyn_snt_hsp_id"                 , MUH_GET_HESAP_ID( m_ui->searchEdit_tah_edilemeyen_senetler->GET_TEXT())                          );
    sql_query.SET_VALUE     ( ":sube_yurtici_satis_hsp_id"                  , MUH_GET_HESAP_ID( m_ui->searchEdit_yurtici_sts_hesap_kodu->GET_TEXT() )                          );
    sql_query.SET_VALUE     ( ":sube_hizmet_urt_maliyet_hsp_id"             , MUH_GET_HESAP_ID( m_ui->searchEdit_hizmet_uretim_maliyeti_hesap_kodu->GET_TEXT())                );
    sql_query.SET_VALUE     ( ":sube_prs_gdrlr_hsp_id"                      , MUH_GET_HESAP_ID( m_ui->searchEdit_prs_giderleri_hesabi->GET_TEXT() )                            );
    sql_query.SET_VALUE     ( ":sube_sarf_malzemeleri_hsp_id"               , MUH_GET_HESAP_ID( m_ui->searchEdit_sarf_malzemeleri->GET_TEXT())                                 );
    sql_query.SET_VALUE     ( ":sube_imalattan_giris_hsp_id"                , MUH_GET_HESAP_ID( m_ui->searchEdit_imalattan_giris_hsp_kodu->GET_TEXT())                         );
    sql_query.SET_VALUE     ( ":kasa_ent_hesap_id"                          , MUH_GET_HESAP_ID( m_ui->searchEdit_kasa_hesap_kodu->GET_TEXT() )                                 );
    sql_query.SET_VALUE     ( ":cari_alici_ent_hesap_id"                    , MUH_GET_HESAP_ID( m_ui->searchEdit_cari_alici_ent_hesap_id->GET_TEXT())                          );
    sql_query.SET_VALUE     ( ":cari_satici_ent_hesap_id"                   , MUH_GET_HESAP_ID( m_ui->searchEdit_cari_satici_ent_hesap_id->GET_TEXT())                         );
    sql_query.SET_VALUE     ( ":banka_ent_hesap_id"                         , MUH_GET_HESAP_ID( m_ui->searchEdit_banka_hesap_kodu->GET_TEXT())                                 );
    sql_query.SET_VALUE     ( ":ver_cek_ent_hesap_id"                       , MUH_GET_HESAP_ID( m_ui->searchEdit_ver_cekler_hesap_kodu->GET_TEXT())                            );
    sql_query.SET_VALUE     ( ":odeme_emirleri_ent_hesap_id"                , MUH_GET_HESAP_ID( m_ui->searchEdit_odeme_emirlari_muh_hesap_kodu->GET_TEXT())                    );
    sql_query.SET_VALUE     ( ":pos_ent_hesap_id"                           , MUH_GET_HESAP_ID( m_ui->searchEdit_pos_hesap_kodu->GET_TEXT())                                   );
    sql_query.SET_VALUE     ( ":kk_ent_hesap_id"                            , MUH_GET_HESAP_ID( m_ui->searchEdit_kk_hesap_kodu->GET_TEXT())                                    );
    sql_query.SET_VALUE     ( ":prk_gider_ent_hesap_id"                     , MUH_GET_HESAP_ID( m_ui->searchEdit_prk_gider_hesap_kodu->GET_TEXT())                             );
    sql_query.SET_VALUE     ( ":urunler_ent_hesap_id"                       , MUH_GET_HESAP_ID( m_ui->searchEdit_urunler_hesap_kodu->GET_TEXT())                               );
    sql_query.SET_VALUE     ( ":hizmetler_ent_hesap_id"                     , MUH_GET_HESAP_ID( m_ui->searchEdit_hizmetler_hesap_kodu->GET_TEXT())                             );
    sql_query.SET_VALUE     ( ":demirbaslar_ent_hesap_id"                   , MUH_GET_HESAP_ID( m_ui->searchEdit_demirbaslar_hesap_kodu->GET_TEXT())                           );
    sql_query.SET_VALUE     ( ":pos_kom_gideri_ent_hesap_id"                , MUH_GET_HESAP_ID( m_ui->searchEdit_pos_kom_gideri_hesap_kodu->GET_TEXT())                        );
    sql_query.SET_VALUE     ( ":prs_avanslari_ent_hesap_id"                 , MUH_GET_HESAP_ID( m_ui->searchEdit_prs_avanslar_hesap_kodu->GET_TEXT())                          );
    sql_query.SET_VALUE     ( ":personel_borclari_ent_hesap_id"             , MUH_GET_HESAP_ID( m_ui->searchEdit_prs_borclar_hesap_kodu->GET_TEXT())                           );
    sql_query.SET_VALUE     ( ":prs_damga_vergisi_ent_hesap_id"             , MUH_GET_HESAP_ID( m_ui->searchEdit_prs_damga_vergisi_hesap_kodu->GET_TEXT())                     );
    sql_query.SET_VALUE     ( ":personel_vergi_ent_hesap_id"                , MUH_GET_HESAP_ID( m_ui->searchEdit_prs_vergi_hesap_kodu->GET_TEXT())                             );
    sql_query.SET_VALUE     ( ":prk_gelir_ent_hesap_id"                     , MUH_GET_HESAP_ID( m_ui->searchEdit_prk_gelir_hesap_kodu->GET_TEXT())                             );
    sql_query.SET_VALUE     ( ":prs_ssk_isci_payi_hesap_id"                 , MUH_GET_HESAP_ID( m_ui->searchEdit_personel_ssk_prim_yuzdesi_isci_payi_hesabi->GET_TEXT())       );
    sql_query.SET_VALUE     ( ":prs_ssk_isveren_payi_hesap_id"              , MUH_GET_HESAP_ID( m_ui->searchEdit_personel_ssk_prim_yuzdesi_isveren_payi_hesabi->GET_TEXT())    );
    sql_query.SET_VALUE     ( ":prs_issizlik_sgk_isci_hsp_id"               , MUH_GET_HESAP_ID( m_ui->searchEdit_personel_issizlik_sigortasi_isci_yuzdesi_hesabi->GET_TEXT())  );
    sql_query.SET_VALUE     ( ":prs_issizlik_sgk_isv_hsp_id"                , MUH_GET_HESAP_ID( m_ui->searchEdit_personel_issizlik_sigortasi_isveren_yuzdesi_hesabi->GET_TEXT())   );
    sql_query.SET_VALUE     ( ":asg_gecim_ind_hesap_id"                     , MUH_GET_HESAP_ID( m_ui->searchEdit_personel_asgari_odeme_ent_hesabi->GET_TEXT())                 );
    sql_query.SET_VALUE     ( ":isveren_payi_giderleri_hps_id"              , MUH_GET_HESAP_ID( m_ui->searchEdit_isveren_paylari_gider_hesabi->GET_TEXT()));
    sql_query.SET_VALUE     ( ":bir_amortismanlar_hsp_id"                   , MUH_GET_HESAP_ID( m_ui->searchEdit_bir_amortismanlar_hesap_kodu->GET_TEXT()));
    sql_query.SET_VALUE     ( ":amor_giderleri_hsp_id"                      , MUH_GET_HESAP_ID( m_ui->searchEdit_amor_giderleri_hsp_kodu->GET_TEXT()));
    sql_query.SET_VALUE     ( ":sgdp_prim_yuzde_isc_hsp_id"                 , MUH_GET_HESAP_ID( m_ui->searchEdit_prs_sgdp_prim_yuzde_isc_hsp_id->GET_TEXT()));
    sql_query.SET_VALUE     ( ":sgdp_prim_yuzde_isv_hsp_id"                 , MUH_GET_HESAP_ID( m_ui->searchEdit_prs_sgdp_prim_yuzde_isv_hsp_id->GET_TEXT()));
    sql_query.SET_VALUE     ( ":kidem_tazminati_hesap_id"                   , MUH_GET_HESAP_ID( m_ui->searchEdit_kidem_tazminati_hesap_id->GET_TEXT()));
    sql_query.SET_VALUE     ( ":ihbar_tazminati_hesap_id"                   , MUH_GET_HESAP_ID( m_ui->searchEdit_ihbar_tazminati_hesap_id->GET_TEXT()));
    sql_query.SET_VALUE     ( ":dmr_satis_kar_ent_hesap_id"                 , MUH_GET_HESAP_ID( m_ui->searchEdit_dmr_satis_kar_hesabi->GET_TEXT()));
    sql_query.SET_VALUE     ( ":dmr_satis_zarar_ent_hesap_id"               , MUH_GET_HESAP_ID( m_ui->searchEdit_dmr_satis_zarar_hesabi->GET_TEXT()));
    sql_query.SET_VALUE     ( ":sendika_odeme_hesap_id"                     , MUH_GET_HESAP_ID( m_ui->searchEdit_sendika_odeme_hesap_id->GET_TEXT()));
    sql_query.SET_VALUE     ( ":smmm_alinan_hizmet_hesap_id"                , MUH_GET_HESAP_ID( m_ui->searchEdit_smmm_al_hizmet_hesap_kodu->GET_TEXT()));
    sql_query.SET_VALUE     ( ":smmm_gelir_vergisi_hesap_id"                , MUH_GET_HESAP_ID( m_ui->searchEdit_smmm_gelir_vergisi_hesap_kodu->GET_TEXT()));
    sql_query.SET_VALUE     ( ":smmm_satilan_hizmet_hesap_id"               , MUH_GET_HESAP_ID( m_ui->searchEdit_smmm_sat_hizmet_hesap_kodu->GET_TEXT()));
    sql_query.SET_VALUE     ( ":isy_5510_nolu_kanun_hesap_id"               , MUH_GET_HESAP_ID( m_ui->searchEdit_isy_5510_nolu_kanun_hesap_id->GET_TEXT()));
    sql_query.SET_VALUE     ( ":satistan_iade_hesap_id"                     , MUH_GET_HESAP_ID( m_ui->searchEdit_satistan_iade_hesap_id->GET_TEXT()));
    sql_query.SET_VALUE     ( ":ortaklara_borclar_hesap_id"                 , MUH_GET_HESAP_ID( m_ui->searchEdit_ortaklara_borclar_hesap_id->GET_TEXT()));
    sql_query.SET_VALUE     ( ":sube_id"                                    , sube_id                                     );
    sql_query.UPDATE();

    sql_query.PREPARE_SELECT ( "sub_subeler","sube_kodu,sube_adi","sube_id = :sube_id" );
    sql_query.SET_VALUE      ( ":sube_id" , sube_id );

    QString sube_kodu_adi = "Şube Kodu / Adı :" ;
    if ( sql_query.SELECT() NE 0 ) {
         sql_query.NEXT();
        sube_kodu_adi.append ( sql_query.VALUE(0).toString() + "  " + sql_query.VALUE(1).toString() );
    }

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_ENT_HESAPLARI , LOG_ISLEM_UPDATE , sube_kodu_adi);

}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_DELETE
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::DELETE_RECORD
***************************************************************************************/

void SUBE_ENTEGRASYON_HESAPLARI_FORMU::DELETE_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_FIRST_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_LAST_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_NEXT_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_PREV_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::SELECT_RECORD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::SELECT_RECORD()
{

    return 0;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::LOCK_RECORD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::LOCK_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
    // Bu ekran şube fişi içerisinden çağrıldığı için kaydı tekrar loc etmeye gerek yok
    // Şube fişi içinde zaten lock edilmiş durumda
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::UNLOCK_RECORD
***************************************************************************************/

void SUBE_ENTEGRASYON_HESAPLARI_FORMU::UNLOCK_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_RECORD
***************************************************************************************/

int SUBE_ENTEGRASYON_HESAPLARI_FORMU::FIND_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void SUBE_ENTEGRASYON_HESAPLARI_FORMU::SEARCH_EDIT_CLICKED ( QWidget * searchEdit, QLineEdit * lineEdit )
{
    Q_UNUSED(searchEdit);

    int hesap_id = OPEN_MUH_HESAP_ARAMA ( lineEdit->text(), this, 1 );
    if ( hesap_id < 1 ) {
        return;
    }

    MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

    MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

    if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , hesap_id) EQ 0 ) {
        MSG_WARNING(  tr ( "Muhasebe hesabı bulunamadı." ), NULL );

        return ;
    }

    if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {
        MSG_WARNING(  tr ( "Seçtiğiniz hesap tali değil" ), NULL );
        return;
    }
    lineEdit->setText( MUHASEBE_HESABI->tam_hesap_kodu );

}

/**************************************************************************************
                   SUBE_ENTEGRASYON_HESAPLARI_FORMU::WIDGET_DIZILERINI_DUZENLE
***************************************************************************************/

void SUBE_ENTEGRASYON_HESAPLARI_FORMU::WIDGET_DIZILERINI_DUZENLE()
{

    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY = new  QSearchEdit * [ONTANIMLI_ENTEGRASYON_HESAPLARI_SAYISI];
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY   = new  QLineEdit   * [ONTANIMLI_ENTEGRASYON_HESAPLARI_SAYISI];

    //ONTANIMLI ENTEGRASYON HESAPLARI

    // SEARCHEDITLER
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [0]     = m_ui->searchEdit_kasa_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [1]     = m_ui->searchEdit_cari_alici_ent_hesap_id;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [2]     = m_ui->searchEdit_banka_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [3]     = m_ui->searchEdit_ver_cekler_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [4]     = m_ui->searchEdit_odeme_emirlari_muh_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [5]     = m_ui->searchEdit_pos_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [6]     = m_ui->searchEdit_pos_kom_gideri_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [7]     = m_ui->searchEdit_kk_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [8]     = m_ui->searchEdit_prk_gider_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [9]     = m_ui->searchEdit_prk_gelir_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [10]    = m_ui->searchEdit_urunler_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [11]    = m_ui->searchEdit_hizmetler_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [12]    = m_ui->searchEdit_demirbaslar_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [13]    = m_ui->searchEdit_bir_amortismanlar_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [14]    = m_ui->searchEdit_prs_avanslar_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [15]    = m_ui->searchEdit_prs_borclar_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [16]    = m_ui->searchEdit_prs_damga_vergisi_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [17]    = m_ui->searchEdit_prs_vergi_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [18]    = m_ui->searchEdit_personel_asgari_odeme_ent_hesabi;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [19]    = m_ui->searchEdit_personel_ssk_prim_yuzdesi_isci_payi_hesabi;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [20]    = m_ui->searchEdit_personel_ssk_prim_yuzdesi_isveren_payi_hesabi;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [21]    = m_ui->searchEdit_personel_issizlik_sigortasi_isci_yuzdesi_hesabi;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [22]    = m_ui->searchEdit_personel_issizlik_sigortasi_isveren_yuzdesi_hesabi;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [23]    = m_ui->searchEdit_isveren_paylari_gider_hesabi;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [24]    = m_ui->searchEdit_amor_giderleri_hsp_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [25]    = m_ui->searchEdit_prs_sgdp_prim_yuzde_isc_hsp_id;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [26]    = m_ui->searchEdit_prs_sgdp_prim_yuzde_isv_hsp_id;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [27]    = m_ui->searchEdit_kidem_tazminati_hesap_id;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [28]    = m_ui->searchEdit_ihbar_tazminati_hesap_id;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [29]    = m_ui->searchEdit_dmr_satis_kar_hesabi;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [30]    = m_ui->searchEdit_sendika_odeme_hesap_id;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [31]    = m_ui->searchEdit_smmm_al_hizmet_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [32]    = m_ui->searchEdit_smmm_sat_hizmet_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [33]    = m_ui->searchEdit_smmm_gelir_vergisi_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [34]    = m_ui->searchEdit_yurtici_sts_hesap_kodu;              // SUBE
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [35]    = m_ui->searchEdit_hizmet_uretim_maliyeti_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [36]    = m_ui->searchEdit_sarf_malzemeleri;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [37]    = m_ui->searchEdit_imalattan_giris_hsp_kodu;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [38]    = m_ui->searchEdit_prs_giderleri_hesabi;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [39]    = m_ui->searchEdit_portfoydeki_cekler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [40]    = m_ui->searchEdit_tah_ver_cekler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [41]    = m_ui->searchEdit_tem_ver_cekler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [42]    = m_ui->searchEdit_kar_cekler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [43]    = m_ui->searchEdit_port_senetler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [44]    = m_ui->searchEdit_tah_ver_senetler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [45]    = m_ui->searchEdit_tem_ver_senetler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [46]    = m_ui->searchEdit_pro_senetler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [47]    = m_ui->searchEdit_port_senetler_ileri_tarih;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [48]    = m_ui->searchEdit_tah_ver_senetler_ileri_tarih;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [49]    = m_ui->searchEdit_tem_ver_senetler_ileri_tarih;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [50]    = m_ui->searchEdit_borc_senetleri;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [51]    = m_ui->searchEdit_borc_senetleri_ileri_tarih;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [52]    = m_ui->searchEdit_sup_alacaklar;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [53]    = m_ui->searchEdit_tah_edilemeyen_cekler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [54]    = m_ui->searchEdit_tah_edilemeyen_senetler;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [55]    = m_ui->searchEdit_isy_5510_nolu_kanun_hesap_id;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [56]    = m_ui->searchEdit_cari_satici_ent_hesap_id;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [57]    = m_ui->searchEdit_satistan_iade_hesap_id;
    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY    [58]    = m_ui->searchEdit_ortaklara_borclar_hesap_id;


    // LINEEDITLER
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [0]     = m_ui->lineEdit_kasa_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [1]     = m_ui->lineEdit_cari_alici_ent_hesap_id;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [2]     = m_ui->lineEdit_banka_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [3]     = m_ui->lineEdit_ver_cekler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [4]     = m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [5]     = m_ui->lineEdit_pos_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [6]     = m_ui->lineEdit_pos_kom_gideri_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [7]     = m_ui->lineEdit_kk_hesap_kodu;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [8]     = m_ui->lineEdit_prk_gider_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [9]     = m_ui->lineEdit_prk_gelir_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [10]    = m_ui->lineEdit_urunler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [11]    = m_ui->lineEdit_hizmetler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [12]    = m_ui->lineEdit_demirbaslar_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [13]    = m_ui->lineEdit_bir_amortismanlar_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [14]    = m_ui->lineEdit_prs_avanslar_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [15]    = m_ui->lineEdit_prs_borclar_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [16]    = m_ui->lineEdit_damga_vergisi_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [17]    = m_ui->lineEdit_vergi_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [18]    = m_ui->lineEdit_personel_asgari_odeme_ent_hesabi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [19]    = m_ui->lineEdit_personel_ssk_prim_yuzdesi_isci_payi_hesabi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [20]    = m_ui->lineEdit_personel_ssk_prim_yuzdesi_isveren_payi_hesabi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [21]    = m_ui->lineEdit_personel_issizlik_sigortasi_isci_yuzdesi_hesabi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [22]    = m_ui->lineEdit_personel_issizlik_sigortasi_isveren_yuzdesi_hesabi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [23]    = m_ui->lineEdit_isveren_paylari_gider_hesabi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [24]    = m_ui->lineEdit_amor_giderleri_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [25]    = m_ui->lineEdit_prs_sgdp_prim_yuzde_isc_hsp_id;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [26]    = m_ui->lineEdit_prs_sgdp_prim_yuzde_isv_hsp_id;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [27]    = m_ui->lineEdit_kidem_tazminati_hesap_id;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [28]    = m_ui->lineEdit_ihbar_tazminati_hesap_id;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [29]    = m_ui->lineEdit_dmr_satis_kar_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [30]    = m_ui->lineEdit_sendika_odeme_hesap_id;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [31]    = m_ui->lineEdit_smmm_al_hizmet_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [32]    = m_ui->lineEdit_smmm_sat_hizmet_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [33]    = m_ui->lineEdit_smmm_gelir_vergisi_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [34]    = m_ui->lineEdit_yurtici_sts_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [35]    = m_ui->lineEdit_hizmet_uretim_maliyeti_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [36]    = m_ui->lineEdit_sarf_malzemeleri;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [37]    = m_ui->lineEdit_imalattan_giris_hsp_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [38]    = m_ui->lineEdit_personel_giderleri_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [39]    = m_ui->lineedit_port_cekler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [40]    = m_ui->lineedit_tah_cekler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [41]    = m_ui->lineedit_tem_cekler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [42]    = m_ui->lineedit_karsiliksiz_cekler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [43]    = m_ui->lineedit_port_senetler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [44]    = m_ui->lineedit_tah_senetler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [45]    = m_ui->lineedit_tem_senetler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [46]    = m_ui->lineedit_pro_senetler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [47]    = m_ui->lineedit_port_senetler_ileri_tarih_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [48]    = m_ui->lineedit_tah_senetler_ileri_tarih_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [49]    = m_ui->lineedit_tem_senetler_ileri_tarih_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [50]    = m_ui->lineedit_borc_senetler_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [51]    = m_ui->lineedit_borc_senetleri_ileri_tarih_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [52]    = m_ui->lineedit_supheli_alacaklar_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [53]    = m_ui->lineedit_tah_edilemeyen_cek_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [54]    = m_ui->lineedit_tah_edilemeyen_snt_hesap_ismi;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [55]    = m_ui->lineEdit_isy_5510_nolu_kanun_hesap_id;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [56]    = m_ui->lineEdit_cari_satici_ent_hesap_id;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [57]    = m_ui->lineEdit_satistan_iade_hesap_id;
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY      [58]    = m_ui->lineEdit_ortaklara_borclar_hesap_id;

}
