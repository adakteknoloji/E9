#include "prs_enum.h"
#include "muh_fis_utils.h"
#include "cari_fis_utils.h"
#include "muh_enum.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "sube_console_utils.h"
#include "cari_struct.h"
#include "cari_console_utils.h"
#include "adak_sql.h"
#include "e9_gui_utils.h"
#include "cari_enum.h"
#include "prs_struct.h"
#include "prs_con_utils.h"
#include "sube_struct.h"
#include "e9_enum.h"
#include "e9_console_utils.h"

extern ADAK_SQL     *        DB;
extern PRS_SABITLER           G_PRS_SABITLER;
extern PRS_SABITLER           G_PRS_DEFAULT_SABITLER;
extern PRS_FIRMA_ODENEKLERI   G_PRS_FRM_ODENEKLERI;

PRS_RAPOR_VARS                PRS_RV;
PRS_SABITLER                  G_PRS_SABITLER;
PRS_FIRMA_ODENEKLERI          G_PRS_FRM_ODENEKLERI;


PRS_SABITLER                  G_PRS_DEFAULT_SABITLER = { QDate::currentDate(),
                                 0.01, 0.02, 700, 350, 170, 1, 0.14, 0.205, 796.50, 5177.40, 679.50, 796.50, 0.0066,
                                 {0.0,      9400,   23000,  53000,      53000},
                                 {9400,     23000,  53000,  999999999,  999999999},
                                 {0.15,     0.20,   0.27,   0.35,       0.35 },
                                  5,                                                    // gecerli_gelir_vergisi_dilimi_sayisi;
                                 {71.69,    80.65,  89.61,  95.58,      101.55},        // EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI
                                 {59.74,    68.70,  77.66,  83.63,      89.61},         // EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI
                                 {54.68,    62.88,  71.08,  76.55,      82.01},         // BEKAR_ASG_GEC_INDIRIMLERI
                                 {{PRS_AY,  0,      0,      PRS_AY,     6,      6,      2},
                                 {PRS_AY,   6,      6,      PRS_YIL,    1.5,    18,     4},
                                 {PRS_YIL,  1.5,    18,     PRS_YIL,    3,      36,     6},
                                 {PRS_YIL,  3,      36,     PRS_YIL,    100,    1200,   8}},
                                 0.14, 0.195,
                                 796.50, 5177.40 ,  0.075,    0.225,
                                 0,     // muh_fis_hesaplari_birlestir
                                 0, 5.00,   // isy_5510_nolu_kanuna_tabi_mi ve yuzdesi
                                 5.0 , 6.0 };   // calisilan gun sayisi, gunluk yemek yuzdesi


/***************************************************************************************
                      ADRESI_OLUSTUR
****************************************************************************************/

QString ADRESI_OLUSTUR ( QString mahalle,  QString cadde, QString sokak, QString bina_no, QString kat,
                         QString daire_no, QString semt,  QString ilce,  QString sehir,   QString posta_kodu )
{
    if ( mahalle.isEmpty() EQ false ) {
        mahalle = mahalle + " Mah. ";
    }
    if ( cadde.isEmpty() EQ false ) {
        cadde = cadde + " Cad.\n";
    }
    if ( sokak.isEmpty() EQ false ) {
        sokak = sokak + " Sk.";
    }
    if ( bina_no.isEmpty() EQ false ) {
        bina_no = "\nNo: " + bina_no;
    }
    if ( kat.isEmpty() EQ false ) {
        kat = " Kat: " + kat;
    }
    if ( daire_no.isEmpty() EQ false ) {
        daire_no = " D: " + daire_no;
    }
    if ( semt.isEmpty() EQ false ) {
        semt = " " + semt + " ";
    }
    return mahalle + cadde + sokak + bina_no + kat + daire_no + "\n" + semt + ilce + "\n" + sehir + " " + posta_kodu;
}


/***************************************************************************************
                   PRS_CREATE_SABIT_DEGERLER
***************************************************************************************/

int  PRS_CREATE_SABIT_DEGERLER()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "prs_sabit_degerler" ,"sabit_degerler_id" );

    if( sql_query.SELECT() NE 0 ) {
        return 0;
    }

    int     IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI        [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double  IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI  [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double  IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI   [IHBAR_TAZMINATI_DILIMI_SAYISI];
    int     IHBAR_TAZ_BITIS_TARIHI_TURLERI            [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double  IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI      [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double  IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI       [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double  IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI        [IHBAR_TAZMINATI_DILIMI_SAYISI];

    PRS_SET_SABIT_DEGERLER_DEFAULTS();

    for ( int i=0; i< IHBAR_TAZMINATI_DILIMI_SAYISI ; i++ ) {

        IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI        [i] = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_turu;
        IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI  [i] = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_orj_degeri;
        IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI   [i] = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_ay_degeri;
        IHBAR_TAZ_BITIS_TARIHI_TURLERI            [i] = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_turu;
        IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI      [i] = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_orj_degeri;
        IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI       [i] = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_ay_degeri;
        IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI        [i] = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].tazminat_hafta_degeri;
    }

    sql_query.PREPARE_INSERT ("prs_sabit_degerler","sabit_degerler_id" ,
                              " issizlik_sig_isci_yuzdesi       , issizlik_sig_isveren_yuzdesi      , "
                              " sakatlik_ind_derece_1_tutari     , sakatlik_ind_derece_2_tutari     , "
                              " sakatlik_ind_derece_3_tutari     , tehlike_sinifi                   , "
                              " ssk_primi_isci_yuzdesi           , ssk_primi_isveren_yuzdesi        , "
                              " ssk_taban_degeri                 , ssk_tavan_degeri                 , "
                              " ssk_16_yas_alti_asgari_ucret     , ssk_16_yas_ustu_asgari_ucret     , "
                              " damga_vergisi                    , gecerli_glr_ver_dilimi_sayisi    , "
                              " glr_ver_dilim_bas_tutar_array    , glr_ver_dilim_bts_tutar_array    , "
                              " gelir_vergisi_yuzdeleri_array    , evli_es_clsmyr_asggecind_array   , "
                              " evli_es_clsyr_asggecind_array    , bekar_asg_gec_indirimler_array   , "
                              " ihbr_taz_bas_trh_turleri_array   , ihbr_taz_bas_trh_orj_deg_array   , "
                              " ihbr_taz_bas_trh_ay_deg_array    , ihbr_taz_bts_trh_turleri_array   , "
                              " ihbr_taz_bts_trh_orj_deg_array   , ihbr_taz_bts_trh_ay_deg_array    , "
                              " ihbar_taz_hafta_deg_array        , frm_tabi_sigorta_kolu            , "
                              " tehlike_sinifi_isci_yuzdesi      , tehlike_sinifi_isveren_yuzdesi   , "
                              " ssk_isveren_taban_deger          , ssk_isveren_tavan_deger          , "
                              " prs_sgdp_primi_isci_yuzdesi      , prs_sgdp_primi_isveren_yuzdesi   , "
                              " muh_fis_hesaplari_birlestir    ");


    sql_query.SET_VALUE( ":issizlik_sig_isci_yuzdesi"            , G_PRS_DEFAULT_SABITLER.issizlik_sig_isci_yuzdesi     );
    sql_query.SET_VALUE( ":issizlik_sig_isveren_yuzdesi"         , G_PRS_DEFAULT_SABITLER.issizlik_sig_isveren_yuzdesi  );
    sql_query.SET_VALUE( ":sakatlik_ind_derece_1_tutari"         , G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_1_tutari  );
    sql_query.SET_VALUE( ":sakatlik_ind_derece_2_tutari"         , G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_2_tutari  );
    sql_query.SET_VALUE( ":sakatlik_ind_derece_3_tutari"         , G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_3_tutari  );
    sql_query.SET_VALUE( ":tehlike_sinifi"                       , G_PRS_DEFAULT_SABITLER.tehlike_sinifi                );
    sql_query.SET_VALUE( ":ssk_primi_isci_yuzdesi"               , G_PRS_DEFAULT_SABITLER.ssk_primi_isci_yuzdesi        );
    sql_query.SET_VALUE( ":ssk_primi_isveren_yuzdesi"            , G_PRS_DEFAULT_SABITLER.ssk_primi_isveren_yuzdesi     );
    sql_query.SET_VALUE( ":ssk_taban_degeri"                     , G_PRS_DEFAULT_SABITLER.ssk_taban_degeri              );
    sql_query.SET_VALUE( ":ssk_tavan_degeri"                     , G_PRS_DEFAULT_SABITLER.ssk_tavan_degeri              );
    sql_query.SET_VALUE( ":ssk_16_yas_alti_asgari_ucret"         , G_PRS_DEFAULT_SABITLER.ssk_16_yas_alti_asgari_ucret  );
    sql_query.SET_VALUE( ":ssk_16_yas_ustu_asgari_ucret"         , G_PRS_DEFAULT_SABITLER.ssk_16_yas_ustu_asgari_ucret  );
    sql_query.SET_VALUE( ":damga_vergisi"                        , G_PRS_DEFAULT_SABITLER.damga_vergisi                 );
    sql_query.SET_VALUE( ":gecerli_glr_ver_dilimi_sayisi"        , G_PRS_DEFAULT_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi );
    sql_query.SET_VALUE( ":glr_ver_dilim_bas_tutar_array"        , PACK_DOUBLE_ARRAY(G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI , GELIR_VERGISI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":glr_ver_dilim_bts_tutar_array"        , PACK_DOUBLE_ARRAY(G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI     , GELIR_VERGISI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":gelir_vergisi_yuzdeleri_array"        , PACK_DOUBLE_ARRAY(G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI ,GELIR_VERGISI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":evli_es_clsmyr_asggecind_array"       , PACK_DOUBLE_ARRAY(G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI,MAX_COCUK_SAYISI));
    sql_query.SET_VALUE( ":evli_es_clsyr_asggecind_array"        , PACK_DOUBLE_ARRAY(G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI , MAX_COCUK_SAYISI));
    sql_query.SET_VALUE( ":bekar_asg_gec_indirimler_array"       , PACK_DOUBLE_ARRAY(G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI , MAX_COCUK_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bas_trh_turleri_array"       , PACK_INT_ARRAY(IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI , IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bas_trh_orj_deg_array"       , PACK_DOUBLE_ARRAY(IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI,IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bas_trh_ay_deg_array"        , PACK_DOUBLE_ARRAY(IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI , IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bts_trh_turleri_array"       , PACK_INT_ARRAY(IHBAR_TAZ_BITIS_TARIHI_TURLERI , IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bts_trh_orj_deg_array"       , PACK_DOUBLE_ARRAY(IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI,IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbr_taz_bts_trh_ay_deg_array"        , PACK_DOUBLE_ARRAY(IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI,IHBAR_TAZMINATI_DILIMI_SAYISI));
    sql_query.SET_VALUE( ":ihbar_taz_hafta_deg_array"            , PACK_DOUBLE_ARRAY(IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI,IHBAR_TAZMINATI_DILIMI_SAYISI));

    sql_query.SET_VALUE( ":tehlike_sinifi_isci_yuzdesi"          , G_PRS_DEFAULT_SABITLER.tehlike_sinifi_isci_yuzdesi   );
    sql_query.SET_VALUE( ":tehlike_sinifi_isveren_yuzdesi"       , G_PRS_DEFAULT_SABITLER.tehlike_sinifi_isveren_yuzdesi);

    sql_query.SET_VALUE( ":ssk_isveren_taban_deger"              , G_PRS_DEFAULT_SABITLER.ssk_isveren_taban_deger );
    sql_query.SET_VALUE( ":ssk_isveren_tavan_deger"              , G_PRS_DEFAULT_SABITLER.ssk_isveren_tavan_deger );
    sql_query.SET_VALUE( ":prs_sgdp_primi_isci_yuzdesi"          , G_PRS_DEFAULT_SABITLER.prs_sgdp_primi_isci_yuzdesi );
    sql_query.SET_VALUE( ":prs_sgdp_primi_isveren_yuzdesi"       , G_PRS_DEFAULT_SABITLER.prs_sgdp_primi_isveren_yuzdesi );
    sql_query.SET_VALUE( ":muh_fis_hesaplari_birlestir"          , G_PRS_DEFAULT_SABITLER.muh_fis_hesaplari_birlestir );

    sql_query.SET_VALUE( ":frm_tabi_sigorta_kolu"                , PRS_SSK);

    return sql_query.INSERT();
}

/***************************************************************************************
                   PRS_SET_SABIT_DEGERLER_DEFAULTS
 ***************************************************************************************/

void  PRS_SET_SABIT_DEGERLER_DEFAULTS ()
{

    G_PRS_SABITLER.issizlik_sig_isci_yuzdesi                      =  G_PRS_DEFAULT_SABITLER.issizlik_sig_isci_yuzdesi;
    G_PRS_SABITLER.issizlik_sig_isveren_yuzdesi                   =  G_PRS_DEFAULT_SABITLER.issizlik_sig_isveren_yuzdesi;
    G_PRS_SABITLER.sakatlik_ind_derece_1_tutari                   =  G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_1_tutari;
    G_PRS_SABITLER.sakatlik_ind_derece_2_tutari                   =  G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_2_tutari;
    G_PRS_SABITLER.sakatlik_ind_derece_3_tutari                   =  G_PRS_DEFAULT_SABITLER.sakatlik_ind_derece_3_tutari;
    G_PRS_SABITLER.tehlike_sinifi                                 =  G_PRS_DEFAULT_SABITLER.tehlike_sinifi;
    G_PRS_SABITLER.ssk_primi_isci_yuzdesi                         =  G_PRS_DEFAULT_SABITLER.ssk_primi_isci_yuzdesi;
    G_PRS_SABITLER.ssk_primi_isveren_yuzdesi                      =  G_PRS_DEFAULT_SABITLER.ssk_primi_isveren_yuzdesi;
    G_PRS_SABITLER.ssk_taban_degeri                               =  G_PRS_DEFAULT_SABITLER.ssk_taban_degeri;
    G_PRS_SABITLER.ssk_tavan_degeri                               =  G_PRS_DEFAULT_SABITLER.ssk_tavan_degeri;
    G_PRS_SABITLER.ssk_16_yas_alti_asgari_ucret                   =  G_PRS_DEFAULT_SABITLER.ssk_16_yas_alti_asgari_ucret;
    G_PRS_SABITLER.ssk_16_yas_ustu_asgari_ucret                   =  G_PRS_DEFAULT_SABITLER.ssk_16_yas_ustu_asgari_ucret;
    G_PRS_SABITLER.damga_vergisi                                  =  G_PRS_DEFAULT_SABITLER.damga_vergisi;

    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[0] =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[0];
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[0]     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[0];
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[0]                     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[0];
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[1] =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[1];
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[1]     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[1];
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[1]                     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[1];
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[2] =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[2];
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[2]     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[2];
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[2]                     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[2];
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[3] =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[3];
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[3]     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[3];
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[3]                     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[3];
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[4] =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[4];
    G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[4]     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[4];
    G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[4]                     =  G_PRS_DEFAULT_SABITLER.GELIR_VERGISI_YUZDELERI[4];

    G_PRS_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi            =  G_PRS_DEFAULT_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi;

    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_COCUK_YOK ]  = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_COCUK_YOK ];
    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_TEK_COCUK ]  = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_TEK_COCUK ];
    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_IKI_COCUK ]  = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_IKI_COCUK ];
    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_UC_COCUK  ]  = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_UC_COCUK  ];
    G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_DORT_COCUK]  = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ PRS_DORT_COCUK];

    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_COCUK_YOK ]  = G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_COCUK_YOK ];
    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_TEK_COCUK ]  = G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_TEK_COCUK ];
    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_IKI_COCUK ]  = G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_IKI_COCUK ];
    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_UC_COCUK  ]  = G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_UC_COCUK  ];
    G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_DORT_COCUK]  = G_PRS_DEFAULT_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ PRS_DORT_COCUK];



    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_COCUK_YOK ]   = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_COCUK_YOK ];
    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_TEK_COCUK ]   = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_TEK_COCUK ];
    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_IKI_COCUK ]   = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_IKI_COCUK ];
    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_UC_COCUK  ]   = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_UC_COCUK  ];
    G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_DORT_COCUK]   = G_PRS_DEFAULT_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ PRS_DORT_COCUK];

    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI [ 0 ]   = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI [ 0 ];
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI [ 1 ]   = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI [ 1 ];
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI [ 2 ]   = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI [ 2 ];
    G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI [ 3 ]   = G_PRS_DEFAULT_SABITLER.IHBAR_TAZMINATI_TUTARLARI [ 3 ];


    G_PRS_SABITLER.ssk_taban_degeri                 = G_PRS_DEFAULT_SABITLER.ssk_isveren_taban_deger;
    G_PRS_SABITLER.ssk_isveren_tavan_deger          = G_PRS_DEFAULT_SABITLER.ssk_isveren_tavan_deger;

    G_PRS_SABITLER.prs_sgdp_primi_isci_yuzdesi      = G_PRS_DEFAULT_SABITLER.prs_sgdp_primi_isci_yuzdesi;
    G_PRS_SABITLER.prs_sgdp_primi_isveren_yuzdesi   = G_PRS_DEFAULT_SABITLER.prs_sgdp_primi_isveren_yuzdesi;
    G_PRS_SABITLER.muh_fis_hesaplari_birlestir      = G_PRS_DEFAULT_SABITLER.muh_fis_hesaplari_birlestir;

}


/***************************************************************************************
                   PRS_CREATE_FIRMA_ODENEKLERI
****************************************************************************************/

void  PRS_CREATE_FIRMA_ODENEKLERI ()
{
    SQL_QUERY sql_query( DB );

    sql_query.PREPARE_SELECT ( "prs_firma_odenekleri" ,"odenekler_id" );

    if( sql_query.SELECT() NE 0 ) {
        return;
    }

    sql_query.PREPARE_INSERT("prs_firma_odenekleri","odenekler_id",
                             "dogum_yardimi_var_mi_enum   , "
                             "olum_yardimi_var_mi_enum    , "
                             "cocuk_yardimi_var_mi_enum   , "
                             "yakacak_yardimi_var_mi_enum , "
                             "aile_yardimi_var_mi_enum    , "
                             "yemek_parasi_var_mi_enum    , "
                             "evlenme_yardimi_var_mi_enum , "
                             "gorev_harcirahi_var_mi_enum , "
                             "ihbar_tazminati_var_mi_enum , "
                             "kidem_tazminati_var_mi_enum , "
                             "dogum_yardimi_yuzde_mi      , "
                             "olum_yardimi_yuzde_mi       , "
                             "cocuk_yardimi_yuzde_mi      , "
                             "yakacak_yardimi_yuzde_mi    , "
                             "aile_yardimi_yuzde_mi       , "
                             "yemek_parasi_yuzde_mi       , "
                             "evlenme_yardimi_yuzde_mi    , "
                             "gorev_harcirahi_yuzde_mi    , "
                             "ihbar_tazminati_yuzde_mi    , "
                             "kidem_tazminati_yuzde_mi    ");

    sql_query.SET_VALUE(":dogum_yardimi_var_mi_enum"   , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":olum_yardimi_var_mi_enum"    , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":cocuk_yardimi_var_mi_enum"   , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":yakacak_yardimi_var_mi_enum" , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":aile_yardimi_var_mi_enum"    , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":yemek_parasi_var_mi_enum"    , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":evlenme_yardimi_var_mi_enum" , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":gorev_harcirahi_var_mi_enum" , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":ihbar_tazminati_var_mi_enum" , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":kidem_tazminati_var_mi_enum" , PRS_TUM_PERSONELE_UYGULANIR );
    sql_query.SET_VALUE(":dogum_yardimi_yuzde_mi"      , PRS_TUTAR);
    sql_query.SET_VALUE(":olum_yardimi_yuzde_mi"       , PRS_TUTAR);
    sql_query.SET_VALUE(":cocuk_yardimi_yuzde_mi"      , PRS_TUTAR);
    sql_query.SET_VALUE(":yakacak_yardimi_yuzde_mi"    , PRS_TUTAR);
    sql_query.SET_VALUE(":aile_yardimi_yuzde_mi"       , PRS_TUTAR);
    sql_query.SET_VALUE(":yemek_parasi_yuzde_mi"       , PRS_TUTAR);
    sql_query.SET_VALUE(":evlenme_yardimi_yuzde_mi"    , PRS_TUTAR);
    sql_query.SET_VALUE(":gorev_harcirahi_yuzde_mi"    , PRS_TUTAR);
    sql_query.SET_VALUE(":ihbar_tazminati_yuzde_mi"    , PRS_TUTAR);
    sql_query.SET_VALUE(":kidem_tazminati_yuzde_mi"    , PRS_TUTAR);
    sql_query.INSERT();
}

/***************************************************************************************
                   PRS_FILL_SABIT_DEGERLER_STRUCT
***************************************************************************************/

void  PRS_FILL_SABIT_DEGERLER_STRUCT ()
{
    SQL_QUERY       sql_query( DB );
    int             IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI        [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI  [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI   [IHBAR_TAZMINATI_DILIMI_SAYISI];
    int             IHBAR_TAZ_BITIS_TARIHI_TURLERI            [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI      [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI       [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI        [IHBAR_TAZMINATI_DILIMI_SAYISI];

    sql_query.PREPARE_SELECT("prs_sabit_degerler",
                             " issizlik_sig_isci_yuzdesi, issizlik_sig_isveren_yuzdesi, "
                             "sakatlik_ind_derece_1_tutari, sakatlik_ind_derece_2_tutari, sakatlik_ind_derece_3_tutari, "
                             "tehlike_sinifi, ssk_primi_isci_yuzdesi, ssk_primi_isveren_yuzdesi, "
                             "ssk_taban_degeri, ssk_tavan_degeri, ssk_16_yas_alti_asgari_ucret, "
                             "ssk_16_yas_ustu_asgari_ucret, damga_vergisi, gecerli_glr_ver_dilimi_sayisi, "
                             "glr_ver_dilim_bas_tutar_array  , glr_ver_dilim_bts_tutar_array , gelir_vergisi_yuzdeleri_array, "
                             "evli_es_clsmyr_asggecind_array , evli_es_clsyr_asggecind_array , bekar_asg_gec_indirimler_array ,"
                             "ihbr_taz_bas_trh_turleri_array , ihbr_taz_bas_trh_orj_deg_array , ihbr_taz_bas_trh_ay_deg_array ,"
                             "ihbr_taz_bts_trh_turleri_array , ihbr_taz_bts_trh_orj_deg_array ,ihbr_taz_bts_trh_ay_deg_array,"
                             "ihbar_taz_hafta_deg_array , ssk_isveren_taban_deger, ssk_isveren_tavan_deger , "
                             "prs_sgdp_primi_isci_yuzdesi,prs_sgdp_primi_isveren_yuzdesi , "
                             "muh_fis_hesaplari_birlestir , haftalik_calisilan_gun_sayisi, "
                             "gunluk_yemek_yardimi_yuzdesi  ",
                             "sabit_degerler_id = :sabit_degerler_id");

    sql_query.SET_VALUE( ":sabit_degerler_id",     1 );

    if( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    G_PRS_SABITLER.issizlik_sig_isci_yuzdesi            =   sql_query.VALUE("issizlik_sig_isci_yuzdesi").toDouble();
    G_PRS_SABITLER.issizlik_sig_isveren_yuzdesi         =   sql_query.VALUE("issizlik_sig_isveren_yuzdesi").toDouble();
    G_PRS_SABITLER.sakatlik_ind_derece_1_tutari         =   sql_query.VALUE("sakatlik_ind_derece_1_tutari").toDouble();
    G_PRS_SABITLER.sakatlik_ind_derece_2_tutari         =   sql_query.VALUE("sakatlik_ind_derece_2_tutari").toDouble();
    G_PRS_SABITLER.sakatlik_ind_derece_3_tutari         =   sql_query.VALUE("sakatlik_ind_derece_3_tutari").toDouble();
    G_PRS_SABITLER.tehlike_sinifi                       =   sql_query.VALUE("tehlike_sinifi").toInt();
    G_PRS_SABITLER.ssk_primi_isci_yuzdesi               =   sql_query.VALUE("ssk_primi_isci_yuzdesi").toDouble();
    G_PRS_SABITLER.ssk_primi_isveren_yuzdesi            =   sql_query.VALUE("ssk_primi_isveren_yuzdesi").toDouble();
    G_PRS_SABITLER.ssk_taban_degeri                     =   sql_query.VALUE("ssk_taban_degeri").toDouble();
    G_PRS_SABITLER.ssk_tavan_degeri                     =   sql_query.VALUE("ssk_tavan_degeri").toDouble();
    G_PRS_SABITLER.ssk_16_yas_alti_asgari_ucret         =   sql_query.VALUE("ssk_16_yas_alti_asgari_ucret").toDouble();
    G_PRS_SABITLER.ssk_16_yas_ustu_asgari_ucret         =   sql_query.VALUE("ssk_16_yas_ustu_asgari_ucret").toDouble();
    G_PRS_SABITLER.damga_vergisi                        =   sql_query.VALUE("damga_vergisi").toDouble();
    G_PRS_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi  =   sql_query.VALUE("gecerli_glr_ver_dilimi_sayisi").toInt();

    G_PRS_SABITLER.ssk_isveren_taban_deger              =   sql_query.VALUE("ssk_isveren_taban_deger").toDouble();
    G_PRS_SABITLER.ssk_isveren_tavan_deger              =   sql_query.VALUE("ssk_isveren_tavan_deger").toDouble();

    G_PRS_SABITLER.prs_sgdp_primi_isci_yuzdesi          =   sql_query.VALUE("prs_sgdp_primi_isci_yuzdesi").toDouble();
    G_PRS_SABITLER.prs_sgdp_primi_isveren_yuzdesi       =   sql_query.VALUE("prs_sgdp_primi_isveren_yuzdesi").toDouble();
    G_PRS_SABITLER.muh_fis_hesaplari_birlestir          =   sql_query.VALUE("muh_fis_hesaplari_birlestir").toInt();

    G_PRS_SABITLER.haftalik_calisilan_gun_sayisi        =   sql_query.VALUE("haftalik_calisilan_gun_sayisi").toDouble();
    G_PRS_SABITLER.gunluk_yemek_yardimi_yuzdesi         =   sql_query.VALUE("gunluk_yemek_yardimi_yuzdesi").toDouble();

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("glr_ver_dilim_bas_tutar_array").toString() , G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI ,
                        GELIR_VERGISI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("glr_ver_dilim_bts_tutar_array").toString() , G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI,
                        GELIR_VERGISI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("gelir_vergisi_yuzdeleri_array").toString() , G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI,
                        GELIR_VERGISI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("evli_es_clsmyr_asggecind_array").toString() , G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI,
                        MAX_COCUK_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("evli_es_clsyr_asggecind_array").toString() , G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI,
                        MAX_COCUK_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("bekar_asg_gec_indirimler_array").toString() , G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI,
                        MAX_COCUK_SAYISI);

    UNPACK_INT_ARRAY    (sql_query.VALUE("ihbr_taz_bas_trh_turleri_array").toString() , IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI,
                     IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbr_taz_bas_trh_orj_deg_array").toString() , IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI,
                        IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbr_taz_bas_trh_ay_deg_array").toString() , IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI,
                        IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_INT_ARRAY    (sql_query.VALUE("ihbr_taz_bts_trh_turleri_array").toString() , IHBAR_TAZ_BITIS_TARIHI_TURLERI,
                     IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbr_taz_bts_trh_orj_deg_array").toString() , IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI,
                        IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbr_taz_bts_trh_ay_deg_array").toString() , IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI,
                        IHBAR_TAZMINATI_DILIMI_SAYISI);

    UNPACK_DOUBLE_ARRAY (sql_query.VALUE("ihbar_taz_hafta_deg_array").toString() , IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI,
                        IHBAR_TAZMINATI_DILIMI_SAYISI);


    for ( int i=0; i< IHBAR_TAZMINATI_DILIMI_SAYISI ; i++ ) {

        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_turu         = IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI[i] ;
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_orj_degeri   = IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI[i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_ay_degeri    = IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI[i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_turu             = IHBAR_TAZ_BITIS_TARIHI_TURLERI [i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_orj_degeri       = IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI[i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_ay_degeri        = IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI[i];
        G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].tazminat_hafta_degeri         = IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI[i];
    }

}




/***************************************************************************************
                   PRS_FILL_FIRMA_ODENEKLERI_STRUCT
***************************************************************************************/

void  PRS_FILL_FIRMA_ODENEKLERI_STRUCT ()
{
    SQL_QUERY       sql_query     ( DB );

    sql_query.PREPARE_SELECT("prs_firma_odenekleri",
                             "dogum_yardimi_var_mi_enum, dogum_yardimi_ssk_kesintisi, "
                             "dogum_yardimi_gelir_vergisi, dogum_yardimi_damga_vergisi, "
                             "dogum_yardimi_yuzde_mi, dogum_yardimi_yuzdesi, dogum_yardimi_tutari, "
                             "olum_yardimi_var_mi_enum, olum_yardimi_ssk_kesintisi, "
                             "olum_yardimi_gelir_vergisi, olum_yardimi_damga_vergisi, "
                             "olum_yardimi_yuzde_mi, olum_yardimi_yuzdesi, olum_yardimi_tutari, "
                             "cocuk_yardimi_var_mi_enum, cocuk_yardimi_ssk_kesintisi, "
                             "cocuk_yardimi_gelir_vergisi, cocuk_yardimi_damga_vergisi, aile_yardimi_yuzde_mi,"
                             "cocuk_yardimi_yuzde_mi, cocuk_yardimi_yuzdesi, cocuk_yardimi_tutari, "
                             "yakacak_yardimi_var_mi_enum, yakacak_yardimi_ssk_kesintisi, "
                             "yakacak_yardimi_gelir_vergisi, yakacak_yardimi_damga_vergisi, "
                             "yakacak_yardimi_yuzde_mi, yakacak_yardimi_yuzdesi, yakacak_yardimi_tutari, "
                             "aile_yardimi_var_mi_enum, aile_yardimi_ssk_kesintisi, "
                             "aile_yardimi_gelir_vergisi, aile_yardimi_damga_vergisi, "
                             "aile_yardimi_yuzdesi, aile_yardimi_tutari, yemek_parasi_var_mi_enum, "
                             "yemek_parasi_ssk_kesintisi, yemek_parasi_gelir_vergisi, "
                             "yemek_parasi_damga_vergisi, yemek_parasi_yuzde_mi, yemek_parasi_yuzdesi, "
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

    sql_query.SET_VALUE( ":odenekler_id",    1 );



       // Dikkat !! Yalnizca firma odenekleri icin firmada odenegin uygulanip uygulanmadigini yada kimlere uygulandigini belirtmek icin kullanilir


    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.ek_odenek_mi                  = 0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.ek_odenek_mi                 = 0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.ek_odenek_mi                 = 0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.ek_odenek_mi               = 0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.ek_odenek_mi               = 0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.ek_odenek_mi               = 0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.ek_odenek_mi               = 0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.ek_odenek_mi                  = 0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.ek_odenek_mi               = 0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.ek_odenek_mi                  = 0;
    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.ssk_kesinti_matrahi           = 0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.ssk_kesinti_matrahi          = 0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.ssk_kesinti_matrahi          = 0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.ssk_kesinti_matrahi        = 0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.ssk_kesinti_matrahi        = 0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.ssk_kesinti_matrahi        = 0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.ssk_kesinti_matrahi        = 0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.ssk_kesinti_matrahi           = 0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.ssk_kesinti_matrahi        = 0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.ssk_kesinti_matrahi           = 0;
    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.ssk_kesinti_tutari            = 0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.ssk_kesinti_tutari           = 0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.ssk_kesinti_tutari           = 0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.ssk_kesinti_tutari         = 0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.ssk_kesinti_tutari         = 0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.ssk_kesinti_tutari         = 0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.ssk_kesinti_tutari         = 0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.ssk_kesinti_tutari            = 0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.ssk_kesinti_tutari         = 0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.ssk_kesinti_tutari            = 0;
    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.gelir_vergisi_matrahi         = 0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.gelir_vergisi_matrahi        = 0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.gelir_vergisi_matrahi        = 0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.gelir_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.gelir_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.gelir_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.gelir_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.gelir_vergisi_matrahi         = 0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.gelir_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.gelir_vergisi_matrahi         = 0;
    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.gelir_vergisi_tutari          = 0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.gelir_vergisi_tutari         = 0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.gelir_vergisi_tutari         = 0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.gelir_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.gelir_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.gelir_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.gelir_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.gelir_vergisi_tutari          = 0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.gelir_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.gelir_vergisi_tutari          = 0;
    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.damga_vergisi_matrahi         = 0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.damga_vergisi_matrahi        = 0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.damga_vergisi_matrahi        = 0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.damga_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.damga_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.damga_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.damga_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.damga_vergisi_matrahi         = 0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.damga_vergisi_matrahi      = 0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.damga_vergisi_matrahi         = 0;
    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.damga_vergisi_tutari          = 0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.damga_vergisi_tutari         = 0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.damga_vergisi_tutari         = 0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.damga_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.damga_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.damga_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.damga_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.damga_vergisi_tutari          = 0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.damga_vergisi_tutari       = 0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.damga_vergisi_tutari          = 0;
    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.odenek_id                     = 0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.odenek_id                    = 0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.odenek_id                    = 0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.odenek_id                  = 0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.odenek_id                  = 0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.odenek_id                  = 0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.odenek_id                  = 0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.odenek_id                     = 0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.odenek_id                  = 0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.odenek_id                     = 0;
    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.net_tutari                    = 0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.net_tutari                   = 0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.net_tutari                   = 0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.net_tutari                 = 0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.net_tutari                 = 0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.net_tutari                 = 0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.net_tutari                 = 0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.net_tutari                    = 0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.net_tutari                 = 0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.net_tutari                    = 0;

    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.odenek_adi.clear();
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.odenek_adi.clear();
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.odenek_adi.clear();
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.odenek_adi.clear();
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.odenek_adi.clear();
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.odenek_adi.clear();
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.odenek_adi.clear();
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.odenek_adi.clear();
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.odenek_adi.clear();
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.odenek_adi.clear();

    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.var_mi_durumu                =  0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.var_mi_durumu               =  0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.var_mi_durumu               =  0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.var_mi_durumu             =  0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.var_mi_durumu             =  0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.var_mi_durumu             =  0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.var_mi_durumu             =  0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.var_mi_durumu                =  0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.var_mi_durumu             =  0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.var_mi_durumu                =  0;

    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.ssk_kesintisi_iceriyor_mu    =  0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.ssk_kesintisi_iceriyor_mu   =  0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.ssk_kesintisi_iceriyor_mu   =  0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.ssk_kesintisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.ssk_kesintisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.ssk_kesintisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.ssk_kesintisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.ssk_kesintisi_iceriyor_mu    =  0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.ssk_kesintisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.ssk_kesintisi_iceriyor_mu    =  0;

    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.gelir_vergisi_iceriyor_mu    =  0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.gelir_vergisi_iceriyor_mu   =  0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.gelir_vergisi_iceriyor_mu   =  0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.gelir_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.gelir_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.gelir_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.gelir_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.gelir_vergisi_iceriyor_mu    =  0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.gelir_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.gelir_vergisi_iceriyor_mu    =  0;

    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.damga_vergisi_iceriyor_mu    =  0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.damga_vergisi_iceriyor_mu   =  0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.damga_vergisi_iceriyor_mu   =  0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.damga_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.damga_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.damga_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.damga_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.damga_vergisi_iceriyor_mu    =  0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.damga_vergisi_iceriyor_mu =  0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.damga_vergisi_iceriyor_mu    =  0;

    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.yuzde_mi                     =  0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.yuzde_mi                    =  0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.yuzde_mi                    =  0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.yuzde_mi                  =  0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.yuzde_mi                  =  0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.yuzde_mi                  =  0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.yuzde_mi                  =  0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.yuzde_mi                     =  0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.yuzde_mi                  =  0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.yuzde_mi                     =  0;

    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.brut_tutari                  =  0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.brut_tutari                 =  0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.brut_tutari                 =  0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.brut_tutari               =  0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.brut_tutari               =  0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.brut_tutari               =  0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.brut_tutari               =  0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.brut_tutari                  =  0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.brut_tutari               =  0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.brut_tutari                  =  0;

    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.yuzde_orani                  =  0;
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.yuzde_orani                 =  0;
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.yuzde_orani                 =  0;
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.yuzde_orani               =  0;
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.yuzde_orani               =  0;
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.yuzde_orani               =  0;
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.yuzde_orani               =  0;
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.yuzde_orani                  =  0;
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.yuzde_orani               =  0;
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.yuzde_orani                  =  0;


    G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.aciklama.clear();
    G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.aciklama.clear();
    G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.aciklama.clear();
    G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.aciklama.clear();
    G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.aciklama.clear();
    G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.aciklama.clear();
    G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.aciklama.clear();
    G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.aciklama.clear();
    G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.aciklama.clear();
    G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.aciklama.clear();


    if( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();


        G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.odenek_adi                   = "Aile Yardimi";
        G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.odenek_adi                  = "Cocuk Yardimi";
        G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.odenek_adi                  = "Dogum Yardimi";
        G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.odenek_adi                = "Evlenme Yardimi";
        G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.odenek_adi                = "Gorev Harcirahi";
        G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.odenek_adi                = "Ihbar Tazminati";
        G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.odenek_adi                = "Kidem Tazminati";
        G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.odenek_adi                   = "Olum Yardimi";
        G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.odenek_adi                = "Yakacak Yardimi";
        G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.odenek_adi                   = "Yemek Parasi";

        G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.var_mi_durumu                =  sql_query.VALUE("aile_yardimi_var_mi_enum").toInt();
        G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.var_mi_durumu               =  sql_query.VALUE("cocuk_yardimi_var_mi_enum").toInt();
        G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.var_mi_durumu               =  sql_query.VALUE("dogum_yardimi_var_mi_enum").toInt();
        G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.var_mi_durumu             =  sql_query.VALUE("evlenme_yardimi_var_mi_enum").toInt();
        G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.var_mi_durumu             =  sql_query.VALUE("gorev_harcirahi_var_mi_enum").toInt();
        G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.var_mi_durumu             =  sql_query.VALUE("ihbar_tazminati_var_mi_enum").toInt();
        G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.var_mi_durumu             =  sql_query.VALUE("kidem_tazminati_var_mi_enum").toInt();
        G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.var_mi_durumu                =  sql_query.VALUE("olum_yardimi_var_mi_enum").toInt();
        G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.var_mi_durumu             =  sql_query.VALUE("yakacak_yardimi_var_mi_enum").toInt();
        G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.var_mi_durumu                =  sql_query.VALUE("yemek_parasi_var_mi_enum").toInt();

        G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.ssk_kesintisi_iceriyor_mu    =  sql_query.VALUE("aile_yardimi_ssk_kesintisi").toInt();
        G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.ssk_kesintisi_iceriyor_mu   =  sql_query.VALUE("cocuk_yardimi_ssk_kesintisi").toInt();
        G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.ssk_kesintisi_iceriyor_mu   =  sql_query.VALUE("dogum_yardimi_ssk_kesintisi").toInt();
        G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.ssk_kesintisi_iceriyor_mu =  sql_query.VALUE("evlenme_yardimi_ssk_kesintisi").toInt();
        G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.ssk_kesintisi_iceriyor_mu =  sql_query.VALUE("gorev_harcirahi_ssk_kesintisi").toInt();
        G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.ssk_kesintisi_iceriyor_mu =  sql_query.VALUE("ihbar_tazminati_ssk_kesintisi").toInt();
        G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.ssk_kesintisi_iceriyor_mu =  sql_query.VALUE("kidem_tazminati_ssk_kesintisi").toInt();
        G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.ssk_kesintisi_iceriyor_mu    =  sql_query.VALUE("olum_yardimi_ssk_kesintisi").toInt();
        G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.ssk_kesintisi_iceriyor_mu =  sql_query.VALUE("yakacak_yardimi_ssk_kesintisi").toInt();
        G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.ssk_kesintisi_iceriyor_mu    =  sql_query.VALUE("yemek_parasi_ssk_kesintisi").toInt();

        G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.gelir_vergisi_iceriyor_mu    =  sql_query.VALUE("aile_yardimi_gelir_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.gelir_vergisi_iceriyor_mu   =  sql_query.VALUE("cocuk_yardimi_gelir_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.gelir_vergisi_iceriyor_mu   =  sql_query.VALUE("dogum_yardimi_gelir_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.gelir_vergisi_iceriyor_mu =  sql_query.VALUE("evlenme_yardimi_gelir_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.gelir_vergisi_iceriyor_mu =  sql_query.VALUE("gorev_harcirahi_gelir_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.gelir_vergisi_iceriyor_mu =  sql_query.VALUE("ihbar_tazminati_gelir_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.gelir_vergisi_iceriyor_mu =  sql_query.VALUE("kidem_tazminati_gelir_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.gelir_vergisi_iceriyor_mu    =  sql_query.VALUE("olum_yardimi_gelir_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.gelir_vergisi_iceriyor_mu =  sql_query.VALUE("yakacak_yardimi_gelir_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.gelir_vergisi_iceriyor_mu    =  sql_query.VALUE("yemek_parasi_gelir_vergisi").toInt();

        G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.damga_vergisi_iceriyor_mu    =  sql_query.VALUE("aile_yardimi_damga_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.damga_vergisi_iceriyor_mu   =  sql_query.VALUE("cocuk_yardimi_damga_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.damga_vergisi_iceriyor_mu   =  sql_query.VALUE("dogum_yardimi_damga_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.damga_vergisi_iceriyor_mu =  sql_query.VALUE("evlenme_yardimi_damga_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.damga_vergisi_iceriyor_mu =  sql_query.VALUE("gorev_harcirahi_damga_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.damga_vergisi_iceriyor_mu =  sql_query.VALUE("ihbar_tazminati_damga_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.damga_vergisi_iceriyor_mu =  sql_query.VALUE("kidem_tazminati_damga_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.damga_vergisi_iceriyor_mu    =  sql_query.VALUE("olum_yardimi_damga_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.damga_vergisi_iceriyor_mu =  sql_query.VALUE("yakacak_yardimi_damga_vergisi").toInt();
        G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.damga_vergisi_iceriyor_mu    =  sql_query.VALUE("yemek_parasi_damga_vergisi").toInt();

        G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.yuzde_mi                     =  sql_query.VALUE("aile_yardimi_yuzde_mi").toInt();
        G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.yuzde_mi                    =  sql_query.VALUE("cocuk_yardimi_yuzde_mi").toInt();
        G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.yuzde_mi                    =  sql_query.VALUE("dogum_yardimi_yuzde_mi").toInt();
        G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.yuzde_mi                  =  sql_query.VALUE("evlenme_yardimi_yuzde_mi").toInt();
        G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.yuzde_mi                  =  sql_query.VALUE("gorev_harcirahi_yuzde_mi").toInt();
        G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.yuzde_mi                  =  sql_query.VALUE("ihbar_tazminati_yuzde_mi").toInt();
        G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.yuzde_mi                  =  sql_query.VALUE("kidem_tazminati_yuzde_mi").toInt();
        G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.yuzde_mi                     =  sql_query.VALUE("olum_yardimi_yuzde_mi").toInt();
        G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.yuzde_mi                  =  sql_query.VALUE("yakacak_yardimi_yuzde_mi").toInt();
        G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.yuzde_mi                     =  sql_query.VALUE("yemek_parasi_yuzde_mi").toInt();

        G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.brut_tutari                  =  sql_query.VALUE("aile_yardimi_tutari").toDouble();
        G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.brut_tutari                 =  sql_query.VALUE("cocuk_yardimi_tutari").toDouble();
        G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.brut_tutari                 =  sql_query.VALUE("dogum_yardimi_tutari").toDouble();
        G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.brut_tutari               =  sql_query.VALUE("evlenme_yardimi_tutari").toDouble();
        G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.brut_tutari               =  sql_query.VALUE("gorev_harcirahi_tutari").toDouble();
        G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.brut_tutari               =  sql_query.VALUE("ihbar_tazminati_tutari").toDouble();
        G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.brut_tutari               =  sql_query.VALUE("kidem_tazminati_tutari").toDouble();
        G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.brut_tutari                  =  sql_query.VALUE("olum_yardimi_tutari").toDouble();
        G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.brut_tutari               =  sql_query.VALUE("yakacak_yardimi_tutari").toDouble();
        G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.brut_tutari                  =  sql_query.VALUE("yemek_parasi_tutari").toDouble();

        G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.yuzde_orani                  =  sql_query.VALUE("aile_yardimi_yuzdesi").toDouble();
        G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.yuzde_orani                 =  sql_query.VALUE("cocuk_yardimi_yuzdesi").toDouble();
        G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.yuzde_orani                 =  sql_query.VALUE("dogum_yardimi_yuzdesi").toDouble();
        G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.yuzde_orani               =  sql_query.VALUE("evlenme_yardimi_yuzdesi").toDouble();
        G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.yuzde_orani               =  sql_query.VALUE("gorev_harcirahi_yuzdesi").toDouble();
        G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.yuzde_orani               =  sql_query.VALUE("ihbar_tazminati_yuzdesi").toDouble();
        G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.yuzde_orani               =  sql_query.VALUE("kidem_tazminati_yuzdesi").toDouble();
        G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.yuzde_orani                  =  sql_query.VALUE("olum_yardimi_yuzdesi").toDouble();
        G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.yuzde_orani               =  sql_query.VALUE("yakacak_yardimi_yuzdesi").toDouble();
        G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.yuzde_orani                  =  sql_query.VALUE("yemek_parasi_yuzdesi").toDouble();

        QString aciklama  = ("Sabit Firma Ödeneği");
        G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.aciklama     = aciklama;
        G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.aciklama    = aciklama;
        G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.aciklama    = aciklama;
        G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.aciklama  = aciklama;
        G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.aciklama  = aciklama;
        G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.aciklama  = aciklama;
        G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.aciklama  = aciklama;
        G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.aciklama     = aciklama;
        G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.aciklama  = aciklama;
        G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.aciklama     = aciklama;

    }
}



/*******************************************************************************/
/*          PRS_PERSONEL_ICIN_KAYITLI_BORDRO_VAR_MI                                */
/*******************************************************************************/

int PRS_PERSONEL_ICIN_KAYITLI_BORDRO_VAR_MI ( int personel_id )
{
    SQL_QUERY       sql_query  ( DB );

    sql_query.PREPARE_SELECT("prs_bordro_personelleri","bordro_id", "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", personel_id );

    if ( sql_query.SELECT() NE 0 ) {

        return ADAK_OK;
    }
    return ADAK_FAIL;
}


/***************************************************************************************
                   PRS_VERILEN_DONEM_BORDROSU_KESILMIS_MI
****************************************************************************************/

int PRS_VERILEN_DONEM_BORDROSU_KESILMIS_MI ( int ay, int yil, int p_sube_id  )
{
    SQL_QUERY       sql_query        ( DB );

    sql_query.PREPARE_SELECT(  "prs_bordrolar","bordro_id",
                               "bordro_donemi_ay        = :bordro_donemi_ay "
                               "AND bordro_donemi_yil   = :bordro_donemi_yil "
                               "AND sube_id             = :sube_id ");

    sql_query.SET_VALUE( ":bordro_donemi_ay"    , ay );
    sql_query.SET_VALUE( ":bordro_donemi_yil"   , yil );
    sql_query.SET_VALUE( ":sube_id"             , p_sube_id );

    if ( sql_query.SELECT() NE 0 ) {
        return ADAK_OK;
    }

    return ADAK_FAIL;
}


/***************************************************************************************
                   PRS_ASGARI_GECIM_INDIRIMI_HESAPLA
***************************************************************************************/

double  PRS_ASGARI_GECIM_INDIRIMI_HESAPLA ( int personel_id )
{
    SQL_QUERY   sql_query ( DB );

    sql_query.PREPARE_SELECT("prs_personeller", "aile_yardimi_var_mi", "personel_id = :personel_id");
    sql_query.SET_VALUE     (":personel_id", personel_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return 0.00;
    }

    sql_query.NEXT();

    if ( sql_query.VALUE(0) EQ 0 ) {
        return 0.00;
    }

    double       asg_gecim_indirim_tutari =  0;
    int          medeni_hal               =  PRS_BEKAR;
    int          cocuk_sayisi             =  -1;
    int          esi_calisiyor_mu         =  PRS_CALISMIYOR;

    CARI_HESAP_STRUCT * cari_hesap_bilgileri =  new CARI_HESAP_STRUCT;
    CARI_SET_HESAP_STRUCT_DEFAULTS(cari_hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( PRS_GET_CARI_HESAP_ID( personel_id ) , cari_hesap_bilgileri );

    medeni_hal   =  PRS_GET_MEDENI_HAL_ENUM( cari_hesap_bilgileri->medeni_hali );

    sql_query.PREPARE_SELECT( "prs_personel_aile_uyeleri",
                              "akrabalik_derecesi, dogum_tarihi, asgari_gecim_ind_durumu ",
                              "personel_id = :personel_id "
                              "AND akrabalik_derecesi = :akrabalik_derecesi");

    sql_query.SET_VALUE( ":personel_id"           , personel_id);
    sql_query.SET_VALUE( ":akrabalik_derecesi"    , PRS_COCUK  );

    cocuk_sayisi = sql_query.SELECT();

    if ( medeni_hal EQ PRS_EVLI ) {

        sql_query.PREPARE_SELECT ( "prs_personel_aile_uyeleri",
                                   "calisma_durumu",
                                   "personel_id = :personel_id "
                                   "AND akrabalik_derecesi = :akrabalik_derecesi");

        sql_query.SET_VALUE( ":personel_id"             , personel_id);
        sql_query.SET_VALUE( ":akrabalik_derecesi"     , PRS_ES  );

        if ( sql_query.SELECT() NE 0 ) {
            sql_query.NEXT();
            esi_calisiyor_mu   = sql_query.VALUE(0).toInt();
        }
        if ( esi_calisiyor_mu EQ PRS_CALISIYOR ) {
            asg_gecim_indirim_tutari = G_PRS_SABITLER.EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI [ cocuk_sayisi ];
        }
        else if ( esi_calisiyor_mu EQ PRS_CALISMIYOR ) {
            asg_gecim_indirim_tutari = G_PRS_SABITLER.EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI [ cocuk_sayisi ];
        }
    }
    if ( (medeni_hal EQ PRS_BEKAR) OR ( medeni_hal EQ PRS_DUL ) OR ( medeni_hal EQ PRS_BOSANMIS) ) {
        asg_gecim_indirim_tutari = G_PRS_SABITLER.BEKAR_ASG_GEC_INDIRIMLERI [ cocuk_sayisi ];
    }

    return asg_gecim_indirim_tutari;
}

/***************************************************************************************
                   PRS_GELIR_VERGISI_TUTARINI_HESAPLA
****************************************************************************************/

double * PRS_GELIR_VERGISI_TUTARINI_HESAPLA ( int personel_id, double gelir_vergi_matrahi, double *kumulatif_vergi_matrahi )
{
    double* gelir_vergisi_tutarlari = new double [ GELIR_VERGISI_DILIMI_SAYISI ];
    double  gelir_vergisi_tutari = 0.0 ;

    gelir_vergi_matrahi = gelir_vergi_matrahi - PRS_GET_OZEL_DURUM_INDIRIM_TUTARI( PRS_GET_OZEL_DURUM_TURU_ENUM( personel_id ));

    for ( int i =0; i < G_PRS_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi; i++ ) {
        gelir_vergisi_tutarlari[i] = 0.0 ;
    }
    if ( gelir_vergi_matrahi <= 0 ) {
        return gelir_vergisi_tutarlari;
    }

    for ( int i =0; i < G_PRS_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi; i++ ) {

        if ( gelir_vergi_matrahi NE 0.0 ) {

            if ( gelir_vergi_matrahi + *kumulatif_vergi_matrahi <= G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[i] ) {

                gelir_vergisi_tutari += gelir_vergi_matrahi * G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[i];

                *kumulatif_vergi_matrahi += gelir_vergi_matrahi;
                gelir_vergi_matrahi = 0.0;
                gelir_vergisi_tutarlari[i] = gelir_vergisi_tutari;
                gelir_vergisi_tutarlari[i] = ROUND( gelir_vergisi_tutarlari[i] );
                break;
            }
            else if ( gelir_vergi_matrahi + *kumulatif_vergi_matrahi > G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[i] ) {

                if ( *kumulatif_vergi_matrahi < G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[i] ) {

                    gelir_vergisi_tutari += (G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[i] - *kumulatif_vergi_matrahi )
                                           * G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[i];

                    gelir_vergi_matrahi =  gelir_vergi_matrahi -
                                           ( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[i] - *kumulatif_vergi_matrahi );

                    *kumulatif_vergi_matrahi += ( G_PRS_SABITLER.GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[i] - *kumulatif_vergi_matrahi );

                    gelir_vergisi_tutarlari[i] = gelir_vergisi_tutari;
                    gelir_vergisi_tutarlari[i] = ROUND( gelir_vergisi_tutarlari[i] );
                    gelir_vergisi_tutari = 0.0;
                }
            }
        }
    }
    return gelir_vergisi_tutarlari;
}


/**************************************************************************************
                   PRS_DAMGA_VERGISI_TUTARINI_HESAPLA
**************************************************************************************/

double PRS_DAMGA_VERGISI_TUTARINI_HESAPLA ( double brut_ucret )
{
    double   damga_vergisi_tutari;

    damga_vergisi_tutari = ROUND( brut_ucret * G_PRS_SABITLER.damga_vergisi );

    return damga_vergisi_tutari;
}

/***************************************************************************************
                   PRS_GET_PERSONEL_KUMULATIF_VERGI_MATRAHI
***************************************************************************************/

double  PRS_GET_PERSONEL_KUMULATIF_VERGI_MATRAHI ( int personel_id )
{
    SQL_QUERY       sql_query ( DB );

    sql_query.PREPARE_SELECT("prs_personeller","kumulatif_vergi_matrahi" , "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", personel_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return -1;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toDouble();
}


/**************************************************************************************
                   PRS_GET_PERSONEL_BRUT_UCRET
***************************************************************************************/

double  PRS_GET_PERSONEL_BRUT_UCRET ( int personel_id )
{
    SQL_QUERY       sql_query ( DB );

    sql_query.PREPARE_SELECT("prs_personeller","brut_ucret" ," personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", personel_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return -1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toDouble();
}

/*********************************************************************************
                 PRS_GET_PERSONEL_NET_UCRET
 *********************************************************************************/

double    PRS_GET_PERSONEL_NET_UCRET (int personel_id , int bordro_id)
{

    SQL_QUERY       sql_query ( DB );

    sql_query.PREPARE_SELECT("prs_bordro_personelleri","net_ucret","personel_id = :personel_id AND bordro_id=:bordro_id");

    sql_query.SET_VALUE( ":personel_id", personel_id );
    sql_query.SET_VALUE( ":bordro_id" ,bordro_id);

    if ( sql_query.SELECT() EQ 0 ) {
        return -1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toDouble();


}

/**************************************************************************************
                   PRS_GET_PERSONEL_ISE_GIRIS_TARIHI
***************************************************************************************/

QDate  PRS_GET_PERSONEL_ISE_GIRIS_TARIHI ( int personel_id )
{
    SQL_QUERY       sql_query ( DB );

    sql_query.PREPARE_SELECT("prs_personel_kisisel_blgler ","ise_giris_tarihi",
                             "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", personel_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return QDate::currentDate();
    }

    sql_query.NEXT();

    return QDate::fromString( sql_query.VALUE("ise_giris_tarihi").toString(), "yyyy.MM.dd");
}

/**************************************************************************************
                   PRS_GET_PERSONEL_CEP_TELEFONU
***************************************************************************************/

QString  PRS_GET_PERSONEL_CEP_TELEFONU ( int personel_id )
{

    CARI_HESAP_STRUCT hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( PRS_GET_CARI_HESAP_ID(personel_id), &hesap_bilgileri  );


    return ("("+ hesap_bilgileri.cep_tel_ulke_kodu + ") "+ hesap_bilgileri.cep_tel_gsm_kodu+ "-"+ hesap_bilgileri.cep_telefonu);
}

/**************************************************************************************
                   PRS_GET_PERSONEL_SICIL_NO
**************************************************************************************/

QString  PRS_GET_PERSONEL_SICIL_NO ( int personel_id )
{
    SQL_QUERY       sql_query ( DB );

    sql_query.PREPARE_SELECT(" prs_personeller","sicil_no","personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", personel_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return "";
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toString();
}

/***************************************************************************************
                   PRS_GET_PERSONEL_ID
***************************************************************************************/

int  PRS_GET_PERSONEL_ID ( QString sicil_no )
{
    SQL_QUERY       sql_query ( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id", "sicil_no = :sicil_no");

    sql_query.SET_VALUE( ":sicil_no", sicil_no );

    if ( sql_query.SELECT() EQ 0 ) {
        return -1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/***************************************************************************************
                   PRS_GET_ISTEN_AYRILAN_ID
****************************************************************************************/

int PRS_GET_ISTEN_AYRILAN_ID( int personel_id )
{
    SQL_QUERY     sql_query   ( DB );

    sql_query.PREPARE_SELECT("prs_isten_ayrilanlar","isten_ayrilan_id ",
                             "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id",   personel_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
                   PRS_GET_BORDRO_ID
***************************************************************************************/

int PRS_GET_BORDRO_ID ( int ay, int yil, int p_sube_id  )
{
    SQL_QUERY       sql_query ( DB );

    sql_query.PREPARE_SELECT( "prs_bordrolar","bordro_id",
                              "bordro_donemi_ay = :bordro_donemi_ay "
                              "AND bordro_donemi_yil = :bordro_donemi_yil "
                              "AND sube_id = :sube_id ");

    sql_query.SET_VALUE( ":bordro_donemi_ay"    , ay );
    sql_query.SET_VALUE( ":bordro_donemi_yil"   , yil );
    sql_query.SET_VALUE( ":sube_id"             , p_sube_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        return sql_query.VALUE("bordro_id").toInt();
    }
    return -1;
}


/**************************************************************************************
                   PRS_GET_OZEL_DURUM_TURU_ENUM
***************************************************************************************/

int PRS_GET_OZEL_DURUM_TURU_ENUM ( int personel_id )
{
    SQL_QUERY       sql_query           ( DB );
    int             ozel_durum;

    sql_query.PREPARE_SELECT("prs_personel_kisisel_blgler", "ozel_durum_enum"," personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id"   , personel_id  );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        ozel_durum   = sql_query.VALUE(0).toInt();

        switch( ozel_durum ) {
            case PRS_OZURLU_DERECE_1 :
                return PRS_OZURLU_DERECE_1;
            case PRS_OZURLU_DERECE_2 :
                return PRS_OZURLU_DERECE_2;
            case PRS_OZURLU_DERECE_3 :
                return PRS_OZURLU_DERECE_3;
            case PRS_ESKI_HUKUMLU  :
                return PRS_ESKI_HUKUMLU;
            case PRS_TEROR_MAGDURU  :
                return PRS_TEROR_MAGDURU;
            default :
                return 0;
        }
    }
    return 0;
}

/**************************************************************************************
                   PRS_GET_OZEL_DURUM_INDIRIM_TUTARI
***************************************************************************************/

double PRS_GET_OZEL_DURUM_INDIRIM_TUTARI ( int ozel_durum )
{
    SQL_QUERY       sql_query           ( DB );
    double          sakatlik_indirimi_tutari = 0;

    sql_query.PREPARE_SELECT( "prs_sabit_degerler","sakatlik_ind_derece_1_tutari, sakatlik_ind_derece_2_tutari, sakatlik_ind_derece_3_tutari ");


    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        switch ( ozel_durum ){
            case PRS_OZURLU_DERECE_1 :
                sakatlik_indirimi_tutari = sql_query.VALUE("sakatlik_ind_derece_1_tutari").toDouble();
                break;
            case PRS_OZURLU_DERECE_2  :
                sakatlik_indirimi_tutari = sql_query.VALUE("sakatlik_ind_derece_2_tutari").toDouble();
                break;
            case PRS_OZURLU_DERECE_3  :
                sakatlik_indirimi_tutari = sql_query.VALUE("sakatlik_ind_derece_3_tutari").toDouble();
                break;
            default :
                break;
        }
    }
    return sakatlik_indirimi_tutari;
}

/************************************************************************************
                   PRS_DAYS_IN_MONTH
*************************************************************************************/

int PRS_DAYS_IN_MONTH(int month, int year)
{

    /*  This function returns the number of days for the given month in the given year    */

    return (30 + (((month & 9) == 8) || ((month & 9) == 1)) - (month == 2) - (!(((year % 4) == 0)
           && (((year % 100) != 0) || ((year % 400) == 0))) && (month == 2)));

}

/************************************************************************************
                   PRS_GET_CALISILAN_GUN_SAYISI
*************************************************************************************/
int PRS_GET_CALISILAN_GUN_SAYISI ( int personel_id, int bordro_donemi_ay, int bordro_donemi_yil)
{

    SQL_QUERY       query ( DB );

    query.PREPARE_SELECT( "prs_personel_kisisel_blgler","ise_giris_tarihi" ,"personel_id = :personel_id" );
    query.SET_VALUE( ":personel_id",personel_id);
    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    QDate gecici_tarih ;
    gecici_tarih = QDate::fromString ( query.VALUE ( 0 ).toString(), "yyyy.MM.dd" );

    int ise_giris_tarihi_yil    = gecici_tarih.year();
    int ise_giris_tarihi_ay     = gecici_tarih.month();
    int ise_giris_tarihi_gun    = gecici_tarih.day();
    int calisilan_gun_sayisi;

    int bordro_donemi_ayi_gun_sayisi = PRS_DAYS_IN_MONTH (bordro_donemi_ay, bordro_donemi_yil);

    // Personel Ise Bu Ay mi Girdi ?
    if ( (ise_giris_tarihi_yil EQ bordro_donemi_yil) AND
         (ise_giris_tarihi_ay EQ bordro_donemi_ay)) {

        if  ( ise_giris_tarihi_gun < 30){
            calisilan_gun_sayisi = (bordro_donemi_ayi_gun_sayisi - ise_giris_tarihi_gun);

        }

        else if (ise_giris_tarihi_gun >=30 ){
           calisilan_gun_sayisi = (bordro_donemi_ayi_gun_sayisi - ise_giris_tarihi_gun +1);

        }

        else if (bordro_donemi_ayi_gun_sayisi >=30 OR bordro_donemi_ayi_gun_sayisi == 28)
        {
           calisilan_gun_sayisi=30 ;
        }
    }

    else {
        calisilan_gun_sayisi=30 ;
    }

    return calisilan_gun_sayisi ;
}

/*************************************************************************************
                   PRS_GET_BORDRO_KESIM_TARIHI
 *************************************************************************************/
QString PRS_GET_BORDRO_KESIM_TARIHI ( int bordro_id)
{
    SQL_QUERY       query ( DB );

    query.PREPARE_SELECT( "prs_bordrolar ","bordro_kesim_tarihi",
                          "bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , bordro_id);

    if ( query.SELECT() EQ 0 ) {
        return "";
    }

    query.NEXT();

    QString bordro_kesim_tarih ;
    bordro_kesim_tarih = query.VALUE ( 0 ).toString();

    return bordro_kesim_tarih;

}

/***********************************************************************************
                   PRS_GET_PUANTAJ_GUN_SAYISI
 ***********************************************************************************/
int PRS_GET_PUANTAJ_GUN_SAYISI(int personel_id , int p_bordro_donem_yili, int p_bordro_donem_ay )
{
    SQL_QUERY   query( DB) ;

    query.PREPARE_SELECT ("prs_puantaj" ,"gun_sayisi",
                          "personel_id              = :personel_id          "
                          "AND bordro_donemi_yil    = :bordro_donemi_yil    "
                          "AND bordro_donemi_ay     = :bordro_donemi_ay     ");
    query.SET_VALUE (":personel_id"       ,  personel_id   );
    query.SET_VALUE (":bordro_donemi_yil" ,  p_bordro_donem_yili );
    query.SET_VALUE (":bordro_donemi_ay"  ,  p_bordro_donem_ay );


    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/***********************************************************************************
                  PRS_ GET_PUANTAJ_TEMEL_PARA_BIRIM_TUTARI
 ***********************************************************************************/

double PRS_GET_PUANTAJ_TEMEL_PARA_BIRIM_TUTARI (int personel_id, int yil, int ay )
{
    SQL_QUERY           query(DB);

    query.PREPARE_SELECT ("prs_puantaj ","temel_para_birim_tutari",
                            "personel_id = :personel_id  "
                            "AND bordro_donemi_yil    = :bordro_donemi_yil  "
                            "AND bordro_donemi_ay     = :bordro_donemi_ay   ");
    query.SET_VALUE (":personel_id"       ,  personel_id   );
    query.SET_VALUE (":bordro_donemi_yil" ,  yil );
    query.SET_VALUE (":bordro_donemi_ay"  ,  ay );


    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toDouble();

}

/***********************************************************************************
                   PRS_GET_DOVIZ_ID
***********************************************************************************/
int  PRS_GET_DOVIZ_ID (int personel_id)
{
     SQL_QUERY     query(DB);

     query.PREPARE_SELECT( "prs_personeller","cari_hesap_id","personel_id = :personel_id" );
     query.SET_VALUE( ":personel_id",personel_id);

     if ( query.SELECT() EQ 0 ) {
         return 0;
     }

     query.NEXT();

     return  CARI_GET_PARA_BIRIM_ID( query.VALUE("cari_hesap_id").toInt() );
}


/***********************************************************************************
                   PRS_KIDEM_TAZMINATI_ALMAYA_HAK_KAZANMIS_MI
***********************************************************************************/

int PRS_KIDEM_TAZMINATI_ALMAYA_HAK_KAZANMIS_MI ( PRS_CALISMA_SURESI calisma_suresi )
{
    if ( calisma_suresi.year < 1 ){

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/***************************************************************************************
                   PRS_GET_FIRMA_SIGORTA_TURU
****************************************************************************************/

QString PRS_GET_FIRMA_SIGORTA_TURU ()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT( "prs_sabit_degerler","frm_tabi_sigorta_kolu",
                              "sabit_degerler_id =:sabit_degerler_id" );
    sql_query.SET_VALUE(":sabit_degerler_id",1);

    if ( sql_query.SELECT() EQ 0 ) {
        return "";
    }

    sql_query.NEXT();

    return PRS_GET_FIRMA_SIGORTA_KOLU_STRING(sql_query.VALUE(0).toInt());
}

/**************************************************************************************
                   PRS_PERSONEL_ISTEN_AYRILDI_MI
***************************************************************************************/

int PRS_PERSONEL_ISTEN_AYRILDI_MI ( int personel_id )
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT( "prs_personeller","isten_ayrildi_mi",
                              "personel_id = :personel_id" );

    sql_query.SET_VALUE( ":personel_id"  , personel_id  );

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        int isten_ayrildi_mi = sql_query.VALUE( "isten_ayrildi_mi").toInt();

        if ( isten_ayrildi_mi EQ 0 ) { // isten ayrilmamis ise
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}


/**************************************************************************************
                  PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR
**************************************************************************************/

double PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR ( int zaman_turu, double orj_deger )
{
    double hafta_degeri;

    switch( zaman_turu ) {
        case PRS_HAFTA:
            hafta_degeri = orj_deger / 4;
            break;
         case PRS_AY:
            hafta_degeri = orj_deger;
            break;
        case PRS_YIL  :
            hafta_degeri = orj_deger * 12;
            break;
        default :
            hafta_degeri = 0;
            break;
    }
    return hafta_degeri;
}

/**************************************************************************************
                   PRS_CALISMA_SURESINI_HESAPLA
***************************************************************************************/

PRS_CALISMA_SURESI PRS_CALISMA_SURESINI_HESAPLA ( QDate ise_giris_tarihi, QDate isten_cikis_tarihi )
{
    PRS_CALISMA_SURESI   calisma_suresi;

    calisma_suresi.year    = isten_cikis_tarihi.year()  - ise_giris_tarihi.year();
    calisma_suresi.month   = isten_cikis_tarihi.month() - ise_giris_tarihi.month();
    calisma_suresi.day     = isten_cikis_tarihi.day()   - ise_giris_tarihi.day();

    if ( calisma_suresi.month < 0 ) {
        calisma_suresi.year  = calisma_suresi.year -1;
        calisma_suresi.month = 12 + calisma_suresi.month;
    }

    if ( calisma_suresi.day < 0 ) {
        calisma_suresi.month = calisma_suresi.month -1;
        calisma_suresi.day   = 30 + calisma_suresi.day;
    }

    return calisma_suresi;
}




/**************************************************************************************
                   PRS_IHBAR_TAZMINATI_TUTARINI_HESAPLA
**************************************************************************************/

double PRS_IHBAR_TAZMINATI_TUTARINI_HESAPLA ( int personel_id, PRS_CALISMA_SURESI calisma_suresi,
                                          double * brut_ihbar_tazminati_tutari,   double * net_ihbar_tazminati_tutari,
                                          double * ihbar_tazminati_gelir_vergisi_tutari, double * ihbar_tazminati_damga_vergisi_tutari)
{
    int     calisilan_ay_sayisi;
    double  tazminat_hafta_degeri = 0;
    double  brut_ucret;
    double  brut_gunluk_ucret;
    double  kumulatif_vergi_matrahi;
    double * gelir_vergileri;

    brut_ucret              =  PRS_GET_PERSONEL_BRUT_UCRET( personel_id );
    kumulatif_vergi_matrahi =  PRS_GET_PERSONEL_KUMULATIF_VERGI_MATRAHI( personel_id );

    calisilan_ay_sayisi     =  calisma_suresi.year * 12 + calisma_suresi.month;

    for ( int i=0; i< IHBAR_TAZMINATI_DILIMI_SAYISI; i++ ) {

        if ( (calisilan_ay_sayisi >= G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].baslangic_tarihi_ay_degeri) AND
             (calisilan_ay_sayisi <= G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].bitis_tarihi_ay_degeri)) {

            tazminat_hafta_degeri = G_PRS_SABITLER.IHBAR_TAZMINATI_TUTARLARI[i].tazminat_hafta_degeri;
        }
    }

    brut_gunluk_ucret                      = ROUND ( brut_ucret / 30 );
    *brut_ihbar_tazminati_tutari           = ROUND ( brut_gunluk_ucret * tazminat_hafta_degeri * 7 );

    gelir_vergileri = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( personel_id, *brut_ihbar_tazminati_tutari, &kumulatif_vergi_matrahi );

    for ( int i = 0; i < GELIR_VERGISI_DILIMI_SAYISI; i++ ) {

        *ihbar_tazminati_gelir_vergisi_tutari += (double)gelir_vergileri[i];
        *ihbar_tazminati_gelir_vergisi_tutari  = ROUND ( *ihbar_tazminati_gelir_vergisi_tutari );
    }

    *ihbar_tazminati_damga_vergisi_tutari  = PRS_DAMGA_VERGISI_TUTARINI_HESAPLA( *brut_ihbar_tazminati_tutari );
    *ihbar_tazminati_damga_vergisi_tutari  = ROUND ( *ihbar_tazminati_damga_vergisi_tutari );

    *net_ihbar_tazminati_tutari            = ROUND ( *brut_ihbar_tazminati_tutari - *ihbar_tazminati_gelir_vergisi_tutari - *ihbar_tazminati_damga_vergisi_tutari );

    return *net_ihbar_tazminati_tutari;
}

/*************************************************************************************
                   PRS_KIDEM_TAZMINATI_TUTARINI_HESAPLA
**************************************************************************************/

double PRS_KIDEM_TAZMINATI_TUTARINI_HESAPLA ( int personel_id, PRS_CALISMA_SURESI calisma_suresi,
                                          double * brut_kidem_tazminati_tutari, double * net_kidem_tazminati_tutari,
                                          double * kidem_tazminati_damga_vergisi_tutari )
{
    double  brut_ucret;

    brut_ucret                            = PRS_GET_PERSONEL_BRUT_UCRET( personel_id );

    *brut_kidem_tazminati_tutari          = (calisma_suresi.year  * brut_ucret) +
                                            (calisma_suresi.month/12 * brut_ucret) +
                                            (calisma_suresi.day/30 /12   * brut_ucret);
    *brut_kidem_tazminati_tutari          = ROUND (* brut_kidem_tazminati_tutari );

    *kidem_tazminati_damga_vergisi_tutari = PRS_DAMGA_VERGISI_TUTARINI_HESAPLA( *brut_kidem_tazminati_tutari );
    *kidem_tazminati_damga_vergisi_tutari = ROUND ( *kidem_tazminati_damga_vergisi_tutari );

    *net_kidem_tazminati_tutari            = ROUND ( *brut_kidem_tazminati_tutari - *kidem_tazminati_damga_vergisi_tutari );

    return *net_kidem_tazminati_tutari;
}


/**************************************************************************************
                   PRS_FIRMA_SENDIKALI_MI
***************************************************************************************/

int PRS_FIRMA_SENDIKALI_MI ()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_sabit_degerler","frm_sendika_uyesi_mi");

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        if ( sql_query.VALUE(0).toInt() EQ 1 ) {

           return ADAK_OK;
        }
    }
    return ADAK_FAIL;
}


/**************************************************************************************
                   PRS_PERSONEL_SENDIKALI_MI
***************************************************************************************/

int PRS_PERSONEL_SENDIKALI_MI ( int p_personel_id )
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personel_kisisel_blgler ","sendika_uyesi_mi",
                             "personel_id = :personel_id ");

    sql_query.SET_VALUE( ":personel_id" , p_personel_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        if ( sql_query.VALUE(0).toInt() EQ 0 ) {

           return ADAK_OK;
        }
    }
    return ADAK_FAIL;
}



/*********************************************************************
                  PRS_GET_SENDIKA_UCRETI
**********************************************************************/

double PRS_GET_SENDIKA_UCRETI ( int p_personel_id ){

    // 0.07 sendika ucreti yuzdesi sabit degerlere cekilecek !!
    double sendika_ucreti = 0.07 * PRS_GET_PERSONEL_BRUT_UCRET( p_personel_id);

    return sendika_ucreti;
}

/***********************************************************************
                   PRS_GET_EK_ODENEK_UCRETI
 ************************************************************************/
double PRS_GET_EK_ODENEK_UCRETI(int personel_id){

    {
        SQL_QUERY       sql_query ( DB );


        sql_query.PREPARE_SELECT("prs_ek_kesintiler_odenekler ","tutar",
                                 "personel_id = :personel_id" );

        sql_query.SET_VALUE( ":personel_id", personel_id );

        if ( sql_query.SELECT() EQ 0 ) {
            return -1;
        }

        sql_query.NEXT();

        return sql_query.VALUE(0).toDouble();
    }
}

/**************************************************************************************
                   PRS_ODENEK_STRUCTINI_KOPYALA
***************************************************************************************/

void PRS_ODENEK_STRUCTINI_KOPYALA ( PRS_ODENEK_STRUCT * ODENEK_HEDEF, PRS_ODENEK_STRUCT * ODENEK_KAYNAK )
{
    ODENEK_HEDEF->odenek_id                  = ODENEK_KAYNAK->odenek_id ;
    ODENEK_HEDEF->aciklama                   = ODENEK_KAYNAK->aciklama;
    ODENEK_HEDEF->brut_tutari                = ODENEK_KAYNAK->brut_tutari;
    ODENEK_HEDEF->damga_vergisi_iceriyor_mu  = ODENEK_KAYNAK->damga_vergisi_iceriyor_mu;
    ODENEK_HEDEF->damga_vergisi_matrahi      = ODENEK_KAYNAK->damga_vergisi_matrahi;
    ODENEK_HEDEF->damga_vergisi_tutari       = ODENEK_KAYNAK->damga_vergisi_tutari;
    ODENEK_HEDEF->ek_odenek_mi               = ODENEK_KAYNAK->ek_odenek_mi;
    ODENEK_HEDEF->gelir_vergisi_iceriyor_mu  = ODENEK_KAYNAK->gelir_vergisi_iceriyor_mu;
    ODENEK_HEDEF->gelir_vergisi_matrahi      = ODENEK_KAYNAK->gelir_vergisi_matrahi;
    ODENEK_HEDEF->gelir_vergisi_tutari       = ODENEK_KAYNAK->gelir_vergisi_tutari;
    ODENEK_HEDEF->net_tutari                 = ODENEK_KAYNAK->net_tutari;
    ODENEK_HEDEF->odenek_adi                 = ODENEK_KAYNAK->odenek_adi;
    ODENEK_HEDEF->ssk_kesintisi_iceriyor_mu  = ODENEK_KAYNAK->ssk_kesintisi_iceriyor_mu;
    ODENEK_HEDEF->ssk_kesinti_matrahi        = ODENEK_KAYNAK->ssk_kesinti_matrahi;
    ODENEK_HEDEF->ssk_kesinti_tutari         = ODENEK_KAYNAK->ssk_kesinti_tutari;
    ODENEK_HEDEF->var_mi_durumu              = ODENEK_KAYNAK->var_mi_durumu;
    ODENEK_HEDEF->yuzde_mi                   = ODENEK_KAYNAK->yuzde_mi;
    ODENEK_HEDEF->yuzde_orani                = ODENEK_KAYNAK->yuzde_orani;
}


/*******************************************************************************
                   PRS_ODENEK_STRUCTINI_SIFIRLA
*******************************************************************************/
void PRS_ODENEK_STRUCTINI_SIFIRLA(PRS_ODENEK_STRUCT * ODENEK)
{
    ODENEK->brut_tutari               = 0 ;
    ODENEK->damga_vergisi_iceriyor_mu = 0 ;
    ODENEK->damga_vergisi_matrahi     = 0 ;
    ODENEK->damga_vergisi_tutari      = 0 ;
    ODENEK->ek_odenek_mi              = 0 ;
    ODENEK->gelir_vergisi_iceriyor_mu = 0 ;
    ODENEK->gelir_vergisi_matrahi     = 0 ;
    ODENEK->gelir_vergisi_tutari      = 0 ;
    ODENEK->net_tutari                = 0 ;
    ODENEK->odenek_id                 = 0 ;
    ODENEK->ssk_kesintisi_iceriyor_mu = 0 ;
    ODENEK->ssk_kesinti_matrahi       = 0 ;
    ODENEK->ssk_kesinti_tutari        = 0 ;
    ODENEK->var_mi_durumu             = 0 ;
    ODENEK->yuzde_mi                  = 0 ;
    ODENEK->yuzde_orani               = 0 ;
    ODENEK->aciklama.clear();
    ODENEK->odenek_adi.clear();
}

/*******************************************************************************
                   PRS_KESINTI_STRUCTINI_SIFIRLA
*******************************************************************************/
void PRS_KESINTI_STRUCTINI_SIFIRLA(PRS_KESINTI_STRUCT * KESINTI)
{
    KESINTI->kesinti_id         = 0 ;
    KESINTI->ek_kesinti_mi      = 0 ;
    KESINTI->yuzde_mi           = 0 ;
    KESINTI->yuzde_orani        = 0 ;
    KESINTI->tutari             = 0 ;
    KESINTI->kesinti_adi.clear();
    KESINTI->aciklama.clear();

}

/**************************************************************************************
                   PRS_KESINTI_STRUCTINI_KOPYALA
***************************************************************************************/

void PRS_KESINTI_STRUCTINI_KOPYALA ( PRS_KESINTI_STRUCT * KESINTI_HEDEF, PRS_KESINTI_STRUCT * KESINTI_KAYNAK )
{
    KESINTI_HEDEF->aciklama         = KESINTI_KAYNAK->aciklama ;
    KESINTI_HEDEF->ek_kesinti_mi    = KESINTI_KAYNAK->ek_kesinti_mi;
    KESINTI_HEDEF->kesinti_adi      = KESINTI_KAYNAK->kesinti_adi;
    KESINTI_HEDEF->tutari           = KESINTI_KAYNAK->tutari;
    KESINTI_HEDEF->yuzde_mi         = KESINTI_KAYNAK->yuzde_mi;
    KESINTI_HEDEF->yuzde_orani      = KESINTI_KAYNAK->yuzde_orani;
}

/**************************************************************************************
                   PRS_BIR_ONCEKI_AYIN_BORDROSU_KESILMIS_MI
***************************************************************************************/

int PRS_BIR_ONCEKI_AYIN_BORDROSU_KESILMIS_MI (int ay, int yil)
{
    SQL_QUERY       sql_query                   ( DB );
    int             onceki_donem_ay;
    int             onceki_donem_yil;

    sql_query.PREPARE_SELECT( "prs_bordrolar","bordro_id");

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_OK;
    }

    if ( ay EQ ENUM_OCAK ) {

        return ADAK_OK;
    }

    onceki_donem_ay  = ay -1;
    onceki_donem_yil = yil;

    sql_query.PREPARE_SELECT( "prs_bordrolar","bordro_id", "bordro_donemi_ay = :bordro_donemi_ay "
                              "AND bordro_donemi_yil = :bordro_donemi_yil");

    sql_query.SET_VALUE( ":bordro_donemi_ay"    , onceki_donem_ay );
    sql_query.SET_VALUE( ":bordro_donemi_yil"   , onceki_donem_yil );

    if ( sql_query.SELECT() EQ 0 ) {

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                 PRS_PERSONEL_BORDRODA_MEVCUT_MU
**************************************************************************************/

int PRS_PERSONEL_BORDRODA_MEVCUT_MU (int bordro_id, int personel_id)
{
    SQL_QUERY       sql_query   ( DB );

    sql_query.PREPARE_SELECT( "prs_bordro_personelleri","bordro_personel_id",
                              "bordro_id = :bordro_id AND personel_id = :personel_id");

    sql_query.SET_VALUE( ":bordro_id"    , bordro_id );
    sql_query.SET_VALUE( ":personel_id"  , personel_id );

    if ( sql_query.SELECT() EQ 0 ) {

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
             PRS_BORDRO_STRUCTINI_SIFIRLA
***************************************************************************************/

void PRS_BORDRO_STRUCTINI_SIFIRLA ( PRS_BORDRO_STRUCT * BORDRO_HESAP )
{
    BORDRO_HESAP->bordro_id                     = 0;
    BORDRO_HESAP->bordro_donemi_ay              = 0;
    BORDRO_HESAP->bordro_donemi_yil             = 0;
    BORDRO_HESAP->toplam_brut_ucret             = 0;
    BORDRO_HESAP->toplam_net_ucret              = 0;
    BORDRO_HESAP->toplam_brut_odenek_tutari     = 0;
    BORDRO_HESAP->toplam_net_odenek_tutari      = 0;
    BORDRO_HESAP->toplam_kesinti_tutari         = 0;
    BORDRO_HESAP->toplam_ssk_kesintisi_tutari   = 0;
    BORDRO_HESAP->toplam_gelir_vergisi_tutari   = 0;
    BORDRO_HESAP->toplam_damga_vergisi_tutari   = 0;
    BORDRO_HESAP->personel_sayisi               = 0;

}

/**************************************************************************************
                   PRS_PERSONEL_STRUCTINI_SIFIRLA
**************************************************************************************/

void PRS_PERSONEL_STRUCTINI_SIFIRLA ( PRS_PERSONEL_STRUCT * PERSONEL_HESAP )
{
    PERSONEL_HESAP->asg_gec_indirimi_tutari                 = 0;
    PERSONEL_HESAP->bordro_oncesi_kumulatif_vergi_matrahi   = 0;
    PERSONEL_HESAP->bordro_sonrasi_kumulatif_vergi_matrahi  = 0;
    PERSONEL_HESAP->brut_ucret                              = 0;
    PERSONEL_HESAP->calisilan_gun_sayisi                    = 0;
    PERSONEL_HESAP->damga_vergisi_tutari                    = 0;
    PERSONEL_HESAP->gelir_vergisi_tutari                    = 0;
    PERSONEL_HESAP->gelir_vergi_matrahi                     = 0;
    PERSONEL_HESAP->issizlik_primi_tutari                   = 0;
    PERSONEL_HESAP->kesinti_sayisi                          = 0;
    PERSONEL_HESAP->net_ucret                               = 0;
    PERSONEL_HESAP->odenek_sayisi                           = 0;
    PERSONEL_HESAP->personel_id                             = 0;
    PERSONEL_HESAP->ssk_primi_tutari                        = 0;
    PERSONEL_HESAP->toplam_brut_odenek_tutari               = 0;
    PERSONEL_HESAP->toplam_kesinti_tutari                   = 0;
    PERSONEL_HESAP->toplam_net_odenek_tutari                = 0;

    PERSONEL_HESAP->bordro_bir_ay_once_kalan_ek_odenek      = 0;
    PERSONEL_HESAP->bordro_iki_ay_once_kalan_ek_odenek      = 0;
    PERSONEL_HESAP->bir_ay_once_kalan_ek_odenek             = 0;
    PERSONEL_HESAP->iki_ay_once_kalan_ek_odenek             = 0;
    PERSONEL_HESAP->calisma_sekli                           = 0;
    PERSONEL_HESAP->yemek_verilen_gun_sayisi              = 0;
    PERSONEL_HESAP->yemek_yardimi_tutari                    = 0;


}

/**********************************************************************************
                   PRS_GET_FIRMA_SIGORTA_KOLU_STRING()
***********************************************************************************/

QString PRS_GET_FIRMA_SIGORTA_KOLU_STRING (int p_frm_sigorta_kolu)
{
    switch (p_frm_sigorta_kolu) {
        case PRS_SSK :
            return QObject::tr("SSK");
        case PRS_EMEKLI_SANDIGI:
            return QObject::tr("Emekli Sandığı");
        case PRS_BAGKUR :
            return QObject::tr("Bağkur");
    default :
            break;
    }
    return "";
}


/**********************************************************************************
                PRS_GET_FIRMA_SIGORTA_KOLU_INT()
***********************************************************************************/

int PRS_GET_FIRMA_SIGORTA_KOLU_INT (QString p_frm_sigorta_kolu)
{
    if (p_frm_sigorta_kolu EQ QObject::tr("SSK")) {
        return PRS_SSK;
    }
    else if ( p_frm_sigorta_kolu EQ QObject::tr("Emekli Sandığı")) {
        return PRS_EMEKLI_SANDIGI;
    }
    else if ( p_frm_sigorta_kolu EQ QObject::tr("Bağkur")) {
        return PRS_BAGKUR;
    }
    return -1;
}

/*********************************************************************
                    PRS_GET_CARI_HESAP_ID
  ********************************************************************/

int PRS_GET_CARI_HESAP_ID ( int prs_id )
{
    SQL_QUERY   sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","cari_hesap_id", "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", prs_id );

    int prs_cari_hesap_id = 0;

        if ( sql_query.SELECT() NE 0 )
        {
            sql_query.NEXT();
            prs_cari_hesap_id   = sql_query.VALUE( 0 ).toInt();
        }

     return prs_cari_hesap_id;
}

  /*********************************************************************
                    PRS_PERSONEL_MAASI_BRUT_MU
  ********************************************************************/

bool PRS_PERSONEL_MAASI_BRUT_MU ( int prs_id )
{
    SQL_QUERY   sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","ucret_sekli", "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", prs_id );

    if ( sql_query.SELECT() NE 0 )
    {
        sql_query.NEXT();
        if ( sql_query.VALUE( 0 ).toInt() EQ PRS_BRUT ){
            return true;
        }
    }
    return false;
}

/****************************************************************************
                  PRS_COMBO_BOX_AY
 ****************************************************************************/
int PRS_COMBO_BOX_AY()
{
    SQL_QUERY        query(DB);
    int ay;
    query.PREPARE_SELECT("prs_bordrolar","bordro_donemi_ay" );
    if(query.SELECT() EQ 0){
       return 0;
    }

    query.NEXT();
    ay=query.VALUE(0).toInt();

    return ay;
}

/****************************************************************************
                  PRS_ENT_FISLERINI_OLUSTUR
****************************************************************************/

void PRS_ENTEGRASYON_FISLERINI_OLUSTUR ( int p_bordro_id , int p_program_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    MUH_FIS_STRUCT          * MUH_FIS           = new MUH_FIS_STRUCT;
    MUH_FIS_SATIRI_STRUCT   * MUH_FIS_SATIRI    = new MUH_FIS_SATIRI_STRUCT;
    CARI_FIS_STRUCT         * CARI_FIS          = new CARI_FIS_STRUCT;
    CARI_FIS_SATIRI_STRUCT  * CARI_FIS_SATIRI   = new CARI_FIS_SATIRI_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUH_FIS);

    int bordro_donemi_ay  = 0;
    int bordro_donemi_yil = 0;

    SQL_QUERY query(DB);

    query.PREPARE_SELECT( "prs_bordrolar ","bordro_donemi_ay, bordro_donemi_yil",
                          "bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , p_bordro_id);

    QString bordro_tarihi = "";
    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        bordro_donemi_ay  = query.VALUE(0).toInt();
        bordro_donemi_yil = query.VALUE(1).toInt();
    }


    bordro_tarihi = PRS_GET_MONTH_LAST_DATE( bordro_donemi_yil, bordro_donemi_ay );

    MUH_FIS->fis_tarihi       = bordro_tarihi;
    MUH_FIS->aciklama         = QString(QObject::tr("Fiş Personel Modülü tarafından üretilmiştir."));
    MUH_FIS->fis_turu         = ENUM_MAHSUP_FISI;
    MUH_FIS->modul_id         = PERSONEL_MODULU;
    MUH_FIS->program_id       = p_program_id;
    MUH_FIS->base_fis_id      = p_bordro_id;

    int muh_fis_id = MUH_FIS_EKLE(MUH_FIS , USE_DB);

    SQL_QUERY s_query(USE_DB);

    query.PREPARE_UPDATE("prs_bordro_personelleri","bordro_personel_id" ,"muh_fis_id","bordro_id = :bordro_id ");
    query.SET_VALUE( ":muh_fis_id" , muh_fis_id );
    query.SET_VALUE( ":bordro_id", p_bordro_id );
    query.UPDATE();


    double issizlik_sig_isveren_yuzdesi     = 0.00;
    double ssk_primi_isveren_yuzdesi        = 0.00;

    double ssk_isveren_tavan_deger          = 0.00;

    double prs_sgdp_primi_isveren_yuzdesi   = 0.00;

    int    muh_fis_hesaplari_birlestir  = 0;
    int    tehlike_sinifi               = 0;
    double isy_5510_nolu_kanun_yuzdesi  = 0.00;

    query.PREPARE_SELECT( "prs_sabit_degerler ",
                          "issizlik_sig_isveren_yuzdesi, ssk_primi_isveren_yuzdesi , "
                          "ssk_isveren_tavan_deger, muh_fis_hesaplari_birlestir, "
                          "prs_sgdp_primi_isveren_yuzdesi, tehlike_sinifi, isy_5510_nolu_kanun_yuzdesi ",
                          "sabit_degerler_id =:sabit_degerler_id" );
    query.SET_VALUE(":sabit_degerler_id",1);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        issizlik_sig_isveren_yuzdesi        = query.VALUE( 0 ).toDouble();
        ssk_primi_isveren_yuzdesi           = query.VALUE( 1 ).toDouble();
        ssk_isveren_tavan_deger             = query.VALUE( 2 ).toDouble();
        muh_fis_hesaplari_birlestir         = query.VALUE( 3 ).toInt();
        prs_sgdp_primi_isveren_yuzdesi      = query.VALUE( 4 ).toDouble();
        tehlike_sinifi                      = query.VALUE( 5 ).toInt();
        isy_5510_nolu_kanun_yuzdesi         = query.VALUE( 6 ).toDouble();
    }

    isy_5510_nolu_kanun_yuzdesi = isy_5510_nolu_kanun_yuzdesi / 100.0;

    CARI_CLEAR_FIS_STRUCT ( CARI_FIS );

    CARI_FIS->aciklama              = QString(QObject::tr("Fiş Personel Modülünden üretilmiştir."));
    CARI_FIS->fis_tarihi            = bordro_tarihi;
    CARI_FIS->modul_id              = PERSONEL_MODULU;
    CARI_FIS->program_id            = E9_PROGRAMI;
    CARI_FIS->fis_turu              = ENUM_CARI_HAREKET_FISI;
    CARI_FIS->muh_fis_id            = muh_fis_id;
    CARI_FIS->base_fis_id           = p_bordro_id;

    int cari_fis_id = CARI_FIS_EKLE ( CARI_FIS , USE_DB);

    //MUHASEBE FISI VE CARI FISE SATIRLAR EKLENECEK
    int personel_sayisi = 0;

    query.PREPARE_SELECT("prs_bordro_personelleri, prs_puantaj, prs_personeller",
                         "prs_bordro_personelleri.personel_id ,                                                     "
                         "prs_bordro_personelleri.net_ucret ,sigorta_primi ,                                        "
                         "issizlik_primi , gelir_vergisi ,damga_vergisi,                                            "
                         "prs_bordro_personelleri.brut_ucret , asg_gec_indirimi, doviz_kuru, odenekler_toplami,     "
                         "ozel_kesintiler_toplami , ssk_matrahi, sube_id                                            ",
                         "prs_puantaj.puantaj_id = prs_bordro_personelleri.puantaj_id                               "
                         "AND prs_puantaj.personel_id = prs_personeller.personel_id                                 "
                         "AND  prs_bordro_personelleri.bordro_id = :bordro_id                                       ");


    query.SET_VALUE(":bordro_id" , p_bordro_id);

    personel_sayisi = query.SELECT();
    if ( personel_sayisi EQ 0){
        return ;
    }

    int personel_sayaci = 0;
    while ( query.NEXT() EQ true ) {

        personel_sayaci++;
        int     personel_id             = query.VALUE( 0 ).toInt();
        double  net_ucret               = query.VALUE( 1 ).toDouble();
        double  sigorta_primi           = query.VALUE( 2 ).toDouble();
        double  issizlik_primi          = query.VALUE( 3 ).toDouble();
        double  gelir_vergisi           = query.VALUE( 4 ).toDouble();
        double  damga_vergisi           = query.VALUE( 5 ).toDouble();
        double  brut_ucret              = query.VALUE( 6 ).toDouble();
        double  asg_gec_indirimi        = query.VALUE( 7 ).toDouble();
        double  doviz_kuru              = query.VALUE( 8 ).toDouble();
        double  odenekler_toplami       = query.VALUE( 9 ).toDouble(); // odenek toplani brut maasa eklenmesi lazim
        double  ozel_kesintiler_toplami = query.VALUE( 10 ).toDouble(); // odenek toplani brut maasa eklenmesi lazim
        double  ssk_matrahi             = query.VALUE( 11 ).toDouble(); // ssk_matrahi %5 icin lazim
        int     sube_id                 = query.VALUE( 12 ).toInt();

        int     bordro_donemi_yil = 0;
        int     bordro_donemi_ay  = 0;

        PRS_GET_BORDRO_DONEMI_AY_YIL(bordro_donemi_ay , bordro_donemi_yil,p_bordro_id , USE_DB);

        int prs_ssk_isveren_payi_hesap_id   = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "prs_ssk_isveren_payi_hesap_id"   );
        int prs_issizlik_sgk_isv_hsp_id     = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "prs_issizlik_sgk_isv_hsp_id"     );
        int asg_gecim_ind_hesap_id          = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "asg_gecim_ind_hesap_id"          );
        int isveren_payi_giderleri_hps_id   = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "isveren_payi_giderleri_hps_id"   );
        int isy_5510_nolu_kanun_hesap_id    = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "isy_5510_nolu_kanun_hesap_id"    );

        s_query.PREPARE_SELECT("prs_personeller ",
                               "prs_avanslar_hesap_id",
                               "personel_id = :personel_id");
        s_query.SET_VALUE(":personel_id", personel_id);

        if ( s_query.SELECT() EQ 0 ) {
            return ;
        }
        s_query.NEXT();

        int prs_avanslari_hesap_id          = s_query.VALUE("prs_avanslar_hesap_id").toInt();


        int prs_issizlik_primi_isci_hesap_id    = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "prs_issizlik_sgk_isci_hsp_id"    );
        int prs_ssk_primi_hesap_id              = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "prs_ssk_isci_payi_hesap_id"      );
        int prs_gelir_vergisi_hesap_id          = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "personel_vergi_ent_hesap_id"     );
        int prs_damga_vergisi_hesap_id          = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "prs_damga_vergisi_ent_hesap_id"  );
        int cari_hesap_id                       = PRS_GET_PERSONEL_CARI_HESAP_ID( personel_id );
        int prs_doviz_id                        = CARI_GET_PARA_BIRIM_ID( cari_hesap_id );
        int prs_sgdp_prim_yuzde_isc_hsp_id      = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "sgdp_prim_yuzde_isc_hsp_id"      );
        int prs_sgdp_prim_yuzde_isv_hsp_id      = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "sgdp_prim_yuzde_isv_hsp_id"      );
        int prs_giderleri_hesap_id              = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id ,"sube_prs_gdrlr_hsp_id"           );


        if ( muh_fis_hesaplari_birlestir EQ ENUM_BORDRO_DETAYLARI ) {

            MUH_CLEAR_FIS_SATIRI_STRUCT( MUH_FIS_SATIRI );

            MUH_FIS_SATIRI->fis_id              = muh_fis_id;
            MUH_FIS_SATIRI->base_record_id      = 0;
            MUH_FIS_SATIRI->modul_id            = PERSONEL_MODULU;
            MUH_FIS_SATIRI->program_id          = p_program_id;

            // PERSONEL GIDERLERI
            MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Şube Giderleri Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                               .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
            MUH_FIS_SATIRI->hesap_id      = prs_giderleri_hesap_id;
            MUH_FIS_SATIRI->borc_tutari   = brut_ucret;
            MUH_FIS_SATIRI->alacak_tutari = 0.00 ;

            MUH_FIS_SATIRI_EKLE( MUH_FIS ,MUH_FIS_SATIRI , USE_DB);

            // ODENEKLER
            if ( odenekler_toplami > 0.00 ) {
                MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Ödenek Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                                   .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
                MUH_FIS_SATIRI->hesap_id      = prs_giderleri_hesap_id;
                MUH_FIS_SATIRI->borc_tutari   = odenekler_toplami;
                MUH_FIS_SATIRI->alacak_tutari = 0.00 ;

                MUH_FIS_SATIRI_EKLE( MUH_FIS ,MUH_FIS_SATIRI , USE_DB);
            }

            // KESINTILER ( AVANSLAR )
            if ( ozel_kesintiler_toplami > 0.00 ) {
                MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Kesinti Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                                   .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
                MUH_FIS_SATIRI->hesap_id      = prs_avanslari_hesap_id;
                MUH_FIS_SATIRI->borc_tutari   = 0.00;
                MUH_FIS_SATIRI->alacak_tutari = ozel_kesintiler_toplami;

                MUH_FIS_SATIRI_EKLE( MUH_FIS ,MUH_FIS_SATIRI , USE_DB);
            }

            //Issizlik primi hesabi id
            MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel İşsizlik Sigortası İşçi Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                               .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
            MUH_FIS_SATIRI->hesap_id      = prs_issizlik_primi_isci_hesap_id;
            MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
            MUH_FIS_SATIRI->alacak_tutari = issizlik_primi;

            if ( issizlik_primi NE 0.00 ) {
               MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);
            }

            //SSK primi hesabi id
            MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel SSK Primi İşci Payı Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                               .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
            MUH_FIS_SATIRI->hesap_id      = prs_ssk_primi_hesap_id;
            if( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) EQ 1 ) {
                MUH_FIS_SATIRI->hesap_id  = prs_sgdp_prim_yuzde_isc_hsp_id;
                MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel SGDP İşci Payı Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                                   .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
            }
            MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
            MUH_FIS_SATIRI->alacak_tutari = sigorta_primi;

            MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

            //PRS gelir vergisi hesabi id
            MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel Gelir Vergisi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                               .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
            MUH_FIS_SATIRI->hesap_id      = prs_gelir_vergisi_hesap_id;
            MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
            MUH_FIS_SATIRI->alacak_tutari = gelir_vergisi ;

            MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

            //asg_gecim_indirimi_hesap_id
            MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel Asgari Geçim İndirimi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                               .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
            MUH_FIS_SATIRI->hesap_id      = asg_gecim_ind_hesap_id;
            MUH_FIS_SATIRI->borc_tutari   = asg_gec_indirimi;
            MUH_FIS_SATIRI->alacak_tutari = 0.00 ;
            MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB );

            // personel damga vergisi
            MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel Damga Vergisi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                               .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
            MUH_FIS_SATIRI->hesap_id      = prs_damga_vergisi_hesap_id;
            MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
            MUH_FIS_SATIRI->alacak_tutari = damga_vergisi;

            MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);


            double isveren_giderleri_tutari         = 0.00;
            double isveren_ssk_yuzdesi_tutari       = 0.00;
            double isveren_issizlik_yuzdesi_tutari  = 0.00;

            // isveren ssk yuzdesi ve issizlik prim yuzdesi

            double gecerli_prim_yuzdesi = ssk_primi_isveren_yuzdesi;

            // EMEKLI ISE SGDP ORANLARI ALINIR
            if ( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) EQ 1 ) {
                gecerli_prim_yuzdesi    = prs_sgdp_primi_isveren_yuzdesi + ( ( ( tehlike_sinifi + 1 ) / 2.0 )  / 100.0 );
            }

            // EMEKLILER DE ISVEREN ISSIZLIK PRIM YUZDESI OLMAZ
            if ( brut_ucret > ssk_isveren_tavan_deger ) {
                isveren_ssk_yuzdesi_tutari      =  ROUND( ssk_isveren_tavan_deger * gecerli_prim_yuzdesi ) ;
                if ( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) NE 1 ) {
                    isveren_issizlik_yuzdesi_tutari =  ROUND( ssk_isveren_tavan_deger * issizlik_sig_isveren_yuzdesi );
                }
            }
            else {
                    isveren_ssk_yuzdesi_tutari      =  ROUND( brut_ucret * gecerli_prim_yuzdesi ) ;
                    if ( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) NE 1 ) {
                        isveren_issizlik_yuzdesi_tutari =  ROUND( brut_ucret * issizlik_sig_isveren_yuzdesi );
                    }
            }

            // isveren giderleri hesai

            isveren_giderleri_tutari    = isveren_ssk_yuzdesi_tutari + isveren_issizlik_yuzdesi_tutari;

            // isveren giderleri hesai
            MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi İşveren Giderleri Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                               .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
            MUH_FIS_SATIRI->hesap_id      = isveren_payi_giderleri_hps_id;
            MUH_FIS_SATIRI->borc_tutari   = isveren_giderleri_tutari ;
            MUH_FIS_SATIRI->alacak_tutari = 0.00;

            MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI,USE_DB );


             // isveren ssk yuzdesi
            MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi İşveren SSK Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                               .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );

            MUH_FIS_SATIRI->hesap_id      = prs_ssk_isveren_payi_hesap_id;
            if( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) EQ 1 ) {
                MUH_FIS_SATIRI->hesap_id      = prs_sgdp_prim_yuzde_isv_hsp_id;
                MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi İşveren SGDP Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                                   .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
            }
            MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
            MUH_FIS_SATIRI->alacak_tutari = isveren_ssk_yuzdesi_tutari;

            MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

            // isveren issizlik prim yuzdesi
            // EMEKLILER DE ISVEREN ISSIZLIK PRIM YUZDESI OLMAZ
            if ( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) NE 1 ) {
                MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi İşveren İşsizlik Primi Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                                   .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
                MUH_FIS_SATIRI->hesap_id      = prs_issizlik_sgk_isv_hsp_id;
                MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
                MUH_FIS_SATIRI->alacak_tutari = isveren_issizlik_yuzdesi_tutari;

                MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);
            }


            // 5510 KANUN

            if ( PRS_ISYERI_5510_KANUNA_TABI_MI() EQ 1 ) {

                // ssk borc
                MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi 5510 kanun SSK ( %5 ) Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                                   .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
                MUH_FIS_SATIRI->hesap_id      = prs_ssk_primi_hesap_id;
                MUH_FIS_SATIRI->borc_tutari   = ssk_matrahi * isy_5510_nolu_kanun_yuzdesi;
                MUH_FIS_SATIRI->alacak_tutari = 0.00;

                MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

                // ssk alacak
                MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Isyeri %5 SGK prim tesvigi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                                   .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
                MUH_FIS_SATIRI->hesap_id      = isy_5510_nolu_kanun_hesap_id;
                MUH_FIS_SATIRI->borc_tutari   = 0.00;
                MUH_FIS_SATIRI->alacak_tutari = ssk_matrahi * isy_5510_nolu_kanun_yuzdesi;

                MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

            }

        }

        if ( muh_fis_hesaplari_birlestir EQ ENUM_BORDRO_TOPLAMLARI ) {
            if ( personel_sayaci EQ personel_sayisi ) {
                PRS_BORDRO_TOPLAMLARI_MUH_ENT( p_bordro_id, muh_fis_id, sube_id, USE_DB );
            }
        }

        //Personel icin cari fis satirlari ekleniyor.

        CARI_CLEAR_FIS_SATIRI_STRUCT(CARI_FIS_SATIRI);

        CARI_FIS_SATIRI->aciklama                = QString ( " %1  %2 dönemi Personel Bordo bedeli..." ).arg( QString::number( bordro_donemi_yil ) )
                                                   .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
        CARI_FIS_SATIRI->alacak_tutari           = net_ucret;
        CARI_FIS_SATIRI->cari_hesap_id           = cari_hesap_id;
        CARI_FIS_SATIRI->doviz_id                = prs_doviz_id;
        CARI_FIS_SATIRI->doviz_kuru              = ROUND(doviz_kuru,-4);
        CARI_FIS_SATIRI->parite                  = 1.00;
        CARI_FIS_SATIRI->doviz_kodu              = DVZ_GET_DOVIZ_KODU( CARI_GET_PARA_BIRIM_ID( cari_hesap_id ) );
        CARI_FIS_SATIRI->fis_id                  = cari_fis_id;
        CARI_FIS_SATIRI->muh_hesap_id            = 0;           // karsi muhasebe hesabini eklememsi lazim cunku satirdaki tutar ayni degil.
        CARI_FIS_SATIRI->program_id              = E9_PROGRAMI;
        CARI_FIS_SATIRI->modul_id                = PERSONEL_MODULU;
        CARI_FIS_SATIRI->base_record_id          = 0;

        CARI_FIS_SATIRI_EKLE(CARI_FIS , CARI_FIS_SATIRI , USE_DB);

    }

}

/************************************************************************************
                            PRS_GET_PERSONEL_PUANTAJ_ID
*************************************************************************************/

int PRS_GET_PERSONEL_PUANTAJ_ID ( int p_personel_id, int p_bordro_yili, int p_bordro_ay )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "prs_puantaj ","puantaj_id",
                          "bordro_donemi_yil    = :bordro_donemi_yil    "
                          "AND personel_id      = :personel_id          "
                          "AND bordro_donemi_ay = :bordro_donemi_ay     ");

    query.SET_VALUE     ( ":bordro_donemi_ay"  , p_bordro_ay     );
    query.SET_VALUE     ( ":bordro_donemi_yil" , p_bordro_yili   );
    query.SET_VALUE     ( ":personel_id"       , p_personel_id   );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/************************************************************************************
                            PRS_GET_BORDRO_DONEMI_AY_YIL
*************************************************************************************/
void PRS_GET_BORDRO_DONEMI_AY_YIL(int &p_bordro_donemi_ay, int &p_bordro_donemi_yil , int p_bordro_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );

    p_bordro_donemi_ay  = 0;
    p_bordro_donemi_yil = 0;

    query.PREPARE_SELECT("prs_bordrolar","bordro_donemi_ay ,bordro_donemi_yil ",
                         "bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , p_bordro_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        p_bordro_donemi_ay  = query.VALUE(0).toInt();
        p_bordro_donemi_yil = query.VALUE(1).toInt();
    }
}

/***************************************************************************************
                   PRS_GET_MEDENI_HAL_ENUM
 ***************************************************************************************/

int PRS_GET_MEDENI_HAL_ENUM ( QString medeni_hal )
{

    if ( medeni_hal EQ QObject::tr ( "Evli" ) ) {
        return PRS_EVLI;
    }
    else if ( medeni_hal EQ QObject::tr ( "Bekar" )) {
        return PRS_BEKAR;
    }
    else if ( medeni_hal EQ QObject::tr ( "Dul" )) {
        return PRS_DUL;
    }
    else if ( medeni_hal EQ QObject::tr ( "Boşanmış" )) {
        return PRS_BOSANMIS;
    }

    return -1;
}

/***************************************************************************************
                   PRS_GET_PERSONEL_EMEKLI_MI
 ***************************************************************************************/

bool PRS_GET_PERSONEL_EMEKLI_MI ( int   p_personel_id )
{

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT( "prs_personeller","emekli_mi","personel_id = :personel_id" );
    query.SET_VALUE     ( ":personel_id", p_personel_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        int  emekli_durum = query.VALUE( 0 ).toInt();

        if ( emekli_durum EQ 1  ) {
            return true;
        }
        else {
            return false;
        }
    }

    return false;
}



/***************************************************************************************
                   PRS_GET_MONTH_LAST_DATE
 ***************************************************************************************/

QString  PRS_GET_MONTH_LAST_DATE  ( int p_yil, int p_ay )
{
    QString tarih= QVariant ( p_yil ).toString() + "." + QVariant ( p_ay ).toString();

    if ( p_ay < 10 ) {
        tarih = QVariant ( p_yil ).toString() + "." + "0" + QVariant ( p_ay ).toString();
    }

    int last_date =  QDate::fromString( tarih , "yyyy.MM" ).daysInMonth();

    return tarih + "." + QVariant ( last_date ).toString();
}

/***************************************************************************************
                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM
***************************************************************************************/

int PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM(QString odenek_kesinti_yardimi_str)
{
    if ( odenek_kesinti_yardimi_str EQ QObject::tr("Tüm Personele Verilir")) {
        return PRS_TUM_PERSONELE_UYGULANIR;
    }
    else if ( odenek_kesinti_yardimi_str EQ QObject::tr("Yalnızca Belirli Personele Verilir")) {
        return PRS_BELIRLI_PERSONELE_UYGULANIR;
    }
    else if ( odenek_kesinti_yardimi_str EQ QObject::tr("Firmada Uygulanmaz")) {
        return PRS_FIRMADA_UYGULANMAZ;
    }
    return -1;
}

/***************************************************************************************
                   PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING
***************************************************************************************/

QString PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING(int odenek_kesinti_yardimi_enum)
{
    switch (odenek_kesinti_yardimi_enum) {
        case PRS_TUM_PERSONELE_UYGULANIR   :
            return QObject::tr("Tüm Personele Verilir");
        case PRS_BELIRLI_PERSONELE_UYGULANIR:
            return QObject::tr("Yalnızca Belirli Personele Verilir");
        case PRS_FIRMADA_UYGULANMAZ:
            return QObject::tr("Firmada Uygulanmaz");
        default:
            break;
    }
    return "";
}




/***************************************************************************************
                   PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM
***************************************************************************************/

int PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM(QString p_tutar_yuzde_turu_str)
{
    if ( p_tutar_yuzde_turu_str EQ  QObject::tr("TUTAR")) {
        return PRS_TUTAR;
    }
    else if ( p_tutar_yuzde_turu_str EQ QObject::tr("YÜZDE")) {
        return PRS_YUZDE;
    }
    return -1;
}

/***************************************************************************************
                   PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING
***************************************************************************************/

QString PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING(int p_tutar_yuzde_turu_enum)
{
    switch ( p_tutar_yuzde_turu_enum ) {
        case PRS_TUTAR :
            return QObject::tr("TUTAR");
        case PRS_YUZDE:
            return QObject::tr("YÜZDE");
        default:
            break;
    }
    return "";
}

/***************************************************************************************
                   PRS_GET_BORDRO_MUH_HESAPLARI_BIRLESTIR_ENUM
***************************************************************************************/

int PRS_GET_BORDRO_MUH_HESAPLARI_BIRLESTIR_ENUM ( QString p_durum )
{
    //"Her Personel İçin Detaylar" << "Bordro Toplamları"
    if ( p_durum EQ QObject::tr( "Her Personel İçin Detaylar" )) {
        return ENUM_BORDRO_DETAYLARI;
    }
    else {
        return ENUM_BORDRO_TOPLAMLARI;
    }
}

/***************************************************************************************
                   PRS_GET_AKRABALIK_DERECESI_ENUM
***************************************************************************************/

int PRS_GET_AKRABALIK_DERECESI_ENUM(QString akrabalik_derecesi)
{
    if ( akrabalik_derecesi EQ QObject::tr("Eş")) {
        return PRS_ES;
    }
    else if ( akrabalik_derecesi EQ QObject::tr("Çocuk")) {
        return PRS_COCUK;
    }
    else if ( akrabalik_derecesi EQ QObject::tr("Anne")) {
        return PRS_ANNE;
    }
    else if ( akrabalik_derecesi EQ QObject::tr("Baba")) {
        return PRS_BABA;
    }
    return -1;
}

/***************************************************************************************
                   PRS_GET_AKRABALIK_DERECESI_STRING
***************************************************************************************/

QString PRS_GET_AKRABALIK_DERECESI_STRING(int akrabalik_derecesi)
{
    switch ( akrabalik_derecesi ) {
        case PRS_ES     :
            return QObject::tr("Eş");

        case PRS_COCUK  :
            return QObject::tr("Çocuk");

        case PRS_ANNE   :
            return QObject::tr("Anne");

        case PRS_BABA   :
            return QObject::tr("Baba");
        default:
            break;
    }
    return "";
}

/***************************************************************************************
                   PRS_GET_CALISMA_DURUMU_ENUM
***************************************************************************************/

int PRS_GET_CALISMA_DURUMU_ENUM(QString p_calisma_durumu)
{
    if ( p_calisma_durumu EQ QObject::tr("Çalışıyor")) {
        return PRS_CALISIYOR;
    }
    else if ( p_calisma_durumu EQ QObject::tr("Çalışmıyor")) {
        return PRS_CALISMIYOR;
    }
    else if ( p_calisma_durumu EQ QObject::tr("Ögrenci")) {
        return PRS_OGRENCI;
    }
    return -1;
}


/***************************************************************************************
                   PRS_GET_CALISMA_DURUMU_STRING
***************************************************************************************/

QString PRS_GET_CALISMA_DURUMU_STRING(int p_calisma_durumu)
{
    switch (p_calisma_durumu ) {
        case PRS_CALISIYOR  :
            return QObject::tr("Çalışıyor");

        case PRS_CALISMIYOR :
            return QObject::tr("Çalışmıyor");

        case PRS_OGRENCI    :
            return QObject::tr("Ögrenci");

        default:
            break;
    }
    return "";
}

/***************************************************************************************
                   PRS_GET_EVET_HAYIR_DURUMU_ENUM
***************************************************************************************/

int PRS_GET_EVET_HAYIR_DURUMU_ENUM(QString p_durum)
{
    if ( p_durum EQ QObject::tr("Evet")) {
        return EVET;

    }
    else if ( p_durum EQ QObject::tr("Hayır")) {
        return HAYIR;
    }
    return -1;
}

/***************************************************************************************
                   PRS_GET_EVET_HAYIR_DURUMU_STRING
***************************************************************************************/

QString PRS_GET_EVET_HAYIR_DURUMU_STRING(int p_durum)
{
    switch ( p_durum ) {
        case EVET:
            return QObject::tr("Evet");
        case HAYIR:
            return QObject::tr("Hayır");
        default:
            break;
    }
    return "";

}

/***************************************************************************************
                   PRS_BORDRO_TOPLAMLARI_MUH_ENT
***************************************************************************************/

void PRS_BORDRO_TOPLAMLARI_MUH_ENT ( int p_bordro_id, int p_muh_fis_id, int p_sube_id, ADAK_SQL * P_GELEN_DB )
{

    ADAK_SQL * USE_DB = DB;
    if ( P_GELEN_DB NE NULL ) {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query  ( USE_DB );
    SQL_QUERY s_query( USE_DB );

    int prs_ssk_isveren_payi_hesap_id   = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "prs_ssk_isveren_payi_hesap_id"   );
    int prs_issizlik_sgk_isv_hsp_id     = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "prs_issizlik_sgk_isv_hsp_id"     );
    int asg_gecim_ind_hesap_id          = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "asg_gecim_ind_hesap_id"          );
    int isveren_payi_giderleri_hps_id   = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "isveren_payi_giderleri_hps_id"   );
    int prs_issizlik_primi_hesap_id     = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "prs_issizlik_sgk_isci_hsp_id"    );
    int prs_ssk_primi_hesap_id          = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "prs_ssk_isci_payi_hesap_id"      );
    int prs_gelir_vergisi_hesap_id      = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "personel_vergi_ent_hesap_id"     );
    int prs_damga_vergisi_hesap_id      = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "prs_damga_vergisi_ent_hesap_id"  );
    int sgdp_prim_yuzde_isc_hsp_id      = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "sgdp_prim_yuzde_isc_hsp_id"      );
    int sgdp_prim_yuzde_isv_hsp_id      = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "sgdp_prim_yuzde_isv_hsp_id"      );
    int isy_5510_nolu_kanun_hesap_id    = SUBE_GET_SUBE_ENT_HESAP_ID( p_sube_id, "isy_5510_nolu_kanun_hesap_id"    );


    double  toplam_net_ucret                        = 0.00;
    double  toplam_sigorta_primi                    = 0.00;
    double  toplam_issizlik_primi                   = 0.00;
    double  toplam_gelir_vergisi                    = 0.00;
    double  toplam_damga_vergisi                    = 0.00;
    double  toplam_brut_ucret                       = 0.00;
    double  toplam_asg_gec_indirimi                 = 0.00;

    double  toplam_isveren_giderleri_tutari         = 0.00;
    double  toplam_isveren_ssk_yuzdesi_tutari       = 0.00;
    double  toplam_isveren_issizlik_yuzdesi_tutari  = 0.00;

    double  toplam_prs_sgdp_isci_primi              = 0.00;
    double  toplam_prs_sgdp_isveren_primi           = 0.00;

    double  toplam_isy_5510_nolu_kanun_hesap_tutari = 0.00;

    int personel_id = 0;

    query.PREPARE_SELECT("prs_bordro_personelleri, prs_puantaj",
                         "net_ucret ,sigorta_primi ,                         "
                         "issizlik_primi , gelir_vergisi ,damga_vergisi,     "
                         "brut_ucret , asg_gec_indirimi, prs_bordro_personelleri.personel_id ",
                         "prs_puantaj.puantaj_id = prs_bordro_personelleri.puantaj_id "
                         "AND  prs_bordro_personelleri.bordro_id = :bordro_id         ");


    query.SET_VALUE(":bordro_id" , p_bordro_id );

    int personel_sayisi = query.SELECT();
    if ( personel_sayisi EQ 0){
        return ;
    }

    while ( query.NEXT() EQ true ) {
          personel_id                = query.VALUE(7).toInt();
          toplam_net_ucret          += query.VALUE(0).toDouble();
          if ( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) EQ 1 ) {
              toplam_prs_sgdp_isci_primi += query.VALUE(1).toDouble();
          }
          else {
            toplam_sigorta_primi    += query.VALUE(1).toDouble();
          }
          if ( PRS_ISYERI_5510_KANUNA_TABI_MI() EQ 1 ) {
              toplam_isy_5510_nolu_kanun_hesap_tutari += ROUND( query.VALUE( 1 ).toDouble() * 0.05 );
          }
          toplam_issizlik_primi     += query.VALUE(2).toDouble();
          toplam_gelir_vergisi      += query.VALUE(3).toDouble();
          toplam_damga_vergisi      += query.VALUE(4).toDouble();
          toplam_brut_ucret         += query.VALUE(5).toDouble();

          double brut_ucret          = query.VALUE(5).toDouble();

          toplam_asg_gec_indirimi   += query.VALUE(6).toDouble();



          double issizlik_sig_isveren_yuzdesi       = 0.00;
          double ssk_primi_isveren_yuzdesi          = 0.00;
          double ssk_isveren_tavan_deger            = 0.00;
          double prs_sgdp_primi_isveren_yuzdesi     = 0.00;

          int   tehlike_sinifi  = 0;
          s_query.PREPARE_SELECT( "prs_sabit_degerler ","issizlik_sig_isveren_yuzdesi, ssk_primi_isveren_yuzdesi , "
                                  "ssk_isveren_tavan_deger, "
                                  "prs_sgdp_primi_isveren_yuzdesi, tehlike_sinifi ",
                                  "sabit_degerler_id =:sabit_degerler_id" );
          s_query.SET_VALUE(":sabit_degerler_id",1);

          if ( s_query.SELECT() NE 0 ) {
              s_query.NEXT();
              issizlik_sig_isveren_yuzdesi    = s_query.VALUE( 0 ).toDouble();
              ssk_primi_isveren_yuzdesi       = s_query.VALUE( 1 ).toDouble();
              ssk_isveren_tavan_deger         = s_query.VALUE( 2 ).toDouble();
              prs_sgdp_primi_isveren_yuzdesi  = s_query.VALUE( 3 ).toDouble();
              tehlike_sinifi                  = s_query.VALUE( 4 ).toInt();
          }

          double isveren_giderleri_tutari         = 0.00;
          double isveren_ssk_yuzdesi_tutari       = 0.00;
          double isveren_issizlik_yuzdesi_tutari  = 0.00;

          // isveren ssk yuzdesi ve issizlik prim yuzdesi
          double gecerli_isveren_yuzdesi = ssk_primi_isveren_yuzdesi;

          if ( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) EQ 1 ) {
              gecerli_isveren_yuzdesi   = prs_sgdp_primi_isveren_yuzdesi + ( ( ( tehlike_sinifi + 1 ) / 2.0  ) / 100.0 );
          }

          if ( brut_ucret > ssk_isveren_tavan_deger ) {
              isveren_ssk_yuzdesi_tutari      =  ROUND( ssk_isveren_tavan_deger * gecerli_isveren_yuzdesi );
              if ( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) EQ 1 ) {
                isveren_issizlik_yuzdesi_tutari =  ROUND( ssk_isveren_tavan_deger * issizlik_sig_isveren_yuzdesi );
              }
          }
          else {
              isveren_ssk_yuzdesi_tutari      =  ROUND( brut_ucret * gecerli_isveren_yuzdesi ) ;
              if ( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) EQ 1 ) {
                isveren_issizlik_yuzdesi_tutari =  ROUND( brut_ucret * issizlik_sig_isveren_yuzdesi );
              }
          }

          isveren_giderleri_tutari    = isveren_ssk_yuzdesi_tutari + isveren_issizlik_yuzdesi_tutari;

          toplam_isveren_giderleri_tutari        += isveren_giderleri_tutari;
          toplam_isveren_issizlik_yuzdesi_tutari += isveren_issizlik_yuzdesi_tutari;

          if ( PRS_GET_PERSONEL_EMEKLI_MI( personel_id ) EQ 1 ) {
              toplam_prs_sgdp_isveren_primi      += isveren_ssk_yuzdesi_tutari;
          }
          else {
              toplam_isveren_ssk_yuzdesi_tutari      += isveren_ssk_yuzdesi_tutari;
          }

    }

    // 5510 giriyor ise toplam ssk priminde cikarmamiz gerekiyor

    if ( PRS_ISYERI_5510_KANUNA_TABI_MI() EQ 1 ) {
        toplam_sigorta_primi -= toplam_isy_5510_nolu_kanun_hesap_tutari;
    }


    MUH_FIS_STRUCT * MUH_FIS                 = new MUH_FIS_STRUCT;
    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI   = new MUH_FIS_SATIRI_STRUCT;

    MUH_CLEAR_FIS_SATIRI_STRUCT( MUH_FIS_SATIRI );
    MUH_CLEAR_FIS_STRUCT       ( MUH_FIS );

    MUH_FIS_BILGILERINI_OKU( MUH_FIS, p_muh_fis_id, USE_DB );

    MUH_FIS_SATIRI->fis_id              = p_muh_fis_id;
    MUH_FIS_SATIRI->base_record_id      = 0;
    MUH_FIS_SATIRI->modul_id            = PERSONEL_MODULU;
    MUH_FIS_SATIRI->program_id          = E9_PROGRAMI;

    int     bordro_donemi_yil = 0;
    int     bordro_donemi_ay  = 0;

    PRS_GET_BORDRO_DONEMI_AY_YIL(bordro_donemi_ay , bordro_donemi_yil,p_bordro_id , USE_DB);

    // TOPLU SEKILDE GITTIGINDE SUBE PRS_GIDERLERI HESABINA GIDER.
    SUBE_ENTEGRASYON_HESAPLARI_STRUCT * SUBE_ENT_HESAPLARI = ( SUBE_ENTEGRASYON_HESAPLARI_STRUCT * ) SUBE_GET_ENTEGRASYON_HESAPLARI_STRUCT();

    int sube_prs_giderler_hsp_id =   SUBE_ENT_HESAPLARI->prs_giderler_hesap_id;

    MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Şube Giderleri Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                       .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
    MUH_FIS_SATIRI->hesap_id      = sube_prs_giderler_hsp_id;
    MUH_FIS_SATIRI->borc_tutari   = toplam_brut_ucret;
    MUH_FIS_SATIRI->alacak_tutari = 0.00 ;

    MUH_FIS_SATIRI_EKLE( MUH_FIS ,MUH_FIS_SATIRI , USE_DB);

    //Issizlik primi hesabi id
    MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel İşsizlik Sigortası İşçi Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                       .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
    MUH_FIS_SATIRI->hesap_id      = prs_issizlik_primi_hesap_id;
    MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
    MUH_FIS_SATIRI->alacak_tutari = toplam_issizlik_primi;

    if ( toplam_issizlik_primi NE 0.00 ) {
       MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);
    }

    //SSK primi hesabi id
    MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel SSK Primi İşci Payı Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                       .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
    MUH_FIS_SATIRI->hesap_id      = prs_ssk_primi_hesap_id;
    MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
    MUH_FIS_SATIRI->alacak_tutari = toplam_sigorta_primi;

    MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

    if ( toplam_prs_sgdp_isci_primi > 0.00 ) {
        //SSK primi hesabi id ( SGDP ) emekliler icin
        MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel SSK Primi İşci Payı ( SGDP ) Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                           .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
        MUH_FIS_SATIRI->hesap_id      = sgdp_prim_yuzde_isc_hsp_id;
        MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
        MUH_FIS_SATIRI->alacak_tutari = toplam_prs_sgdp_isci_primi;

        MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);
    }

    //PRS gelir vergisi hesabi id
    MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel Gelir Vergisi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                       .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
    MUH_FIS_SATIRI->hesap_id      = prs_gelir_vergisi_hesap_id;
    MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
    MUH_FIS_SATIRI->alacak_tutari = toplam_gelir_vergisi ;

    MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

    //asg_gecim_indirimi_hesap_id
    MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel Asgari Geçim İndirimi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                       .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
    MUH_FIS_SATIRI->hesap_id      = asg_gecim_ind_hesap_id;
    MUH_FIS_SATIRI->borc_tutari   = toplam_asg_gec_indirimi;
    MUH_FIS_SATIRI->alacak_tutari = 0.00 ;
    MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB );

    // personel damga vergisi
    MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Personel Damga Vergisi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                       .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
    MUH_FIS_SATIRI->hesap_id      = prs_damga_vergisi_hesap_id;
    MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
    MUH_FIS_SATIRI->alacak_tutari = toplam_damga_vergisi;

    MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

    // isveren giderleri hesabi
    MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi İşveren Giderleri Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                       .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
    MUH_FIS_SATIRI->hesap_id      = isveren_payi_giderleri_hps_id;
    MUH_FIS_SATIRI->borc_tutari   = toplam_isveren_giderleri_tutari ;
    MUH_FIS_SATIRI->alacak_tutari = 0.00;

    MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI,USE_DB );


    // isveren ssk yuzdesi
    MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi İşveren SSK Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                       .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
    MUH_FIS_SATIRI->hesap_id      = prs_ssk_isveren_payi_hesap_id;
    MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
    MUH_FIS_SATIRI->alacak_tutari = toplam_isveren_ssk_yuzdesi_tutari;

    MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

    if ( toplam_prs_sgdp_isveren_primi > 0.00 ) {
        // isveren ssk yuzdesi ( SGDP ) emeklier icin
        MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi İşveren SSK  ( SGDP ) Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                      .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
        MUH_FIS_SATIRI->hesap_id      = sgdp_prim_yuzde_isv_hsp_id;
        MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
        MUH_FIS_SATIRI->alacak_tutari = toplam_prs_sgdp_isveren_primi;

        MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB );
    }

    // isveren issizlik prim yuzdesi
    MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi İşveren İşsizlik Primi Yüzdesi Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                                                                       .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
    MUH_FIS_SATIRI->hesap_id      = prs_issizlik_sgk_isv_hsp_id;
    MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
    MUH_FIS_SATIRI->alacak_tutari = toplam_isveren_issizlik_yuzdesi_tutari;

    MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

    // 5510 nolu kanun
    if ( PRS_ISYERI_5510_KANUNA_TABI_MI() EQ 1 ) {

        MUH_FIS_SATIRI->aciklama      = QString ( " %1  %2 dönemi Isyeri %5 SGK prim teşviüği Tutarı." ).arg( QString::number( bordro_donemi_yil ) )
                                        .arg( E9_GET_AYLAR_STRING( bordro_donemi_ay ) );
        MUH_FIS_SATIRI->hesap_id      = isy_5510_nolu_kanun_hesap_id;
        MUH_FIS_SATIRI->borc_tutari   = 0.00 ;
        MUH_FIS_SATIRI->alacak_tutari = toplam_isy_5510_nolu_kanun_hesap_tutari;

        MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);

    }

}

/***************************************************************************************
                   PRS_GET_OZEL_DURUM_ENUM
***************************************************************************************/

int PRS_GET_OZEL_DURUM_ENUM(QString p_ozel_durum)
{
    if ( p_ozel_durum EQ QObject::tr("0 - Yok") ) {
        return PRS_YOK;
    }
    else if ( p_ozel_durum EQ QObject::tr("1 - 1. Derece Özürlü") ) {
        return PRS_OZURLU_DERECE_1;
    }
    else if ( p_ozel_durum EQ QObject::tr("2 - 2. Derece Özürlü") ) {
        return PRS_OZURLU_DERECE_2;
    }
    else if ( p_ozel_durum EQ QObject::tr("3 - 3. Derece Özürlü") ) {
        return PRS_OZURLU_DERECE_3;
    }
    else if ( p_ozel_durum EQ QObject::tr("4 - Eski Hükümlü") ) {
        return PRS_ESKI_HUKUMLU;
    }
    else if ( p_ozel_durum EQ QObject::tr("5 - Terör Mağduru") ) {
        return PRS_TEROR_MAGDURU;
    }
    return -1;
}

/***************************************************************************************
                   PRS_GET_OZEL_DURUM_STRING
***************************************************************************************/

QString PRS_GET_OZEL_DURUM_STRING(int p_ozel_durum)
{
    switch ( p_ozel_durum ) {

        case PRS_YOK  :
            return QObject::tr("0 - Yok");

        case PRS_OZURLU_DERECE_1    :
            return QObject::tr("1 - 1. Derece Özürlü");

        case PRS_OZURLU_DERECE_2    :
            return QObject::tr("2 - 2. Derece Özürlü");

        case PRS_OZURLU_DERECE_3    :
            return QObject::tr("3 - 3. Derece Özürlü");

        case PRS_ESKI_HUKUMLU       :
            return QObject::tr("4 - Eski Hükümlü");

        case PRS_TEROR_MAGDURU      :
            return QObject::tr("5 - Terör Mağduru");

        default:
            break;

    }
    return "";
}

/***************************************************************************************
                   PRS_KIDEM_IHBAR_TAZMINATI_ENTEGRASTONU
***************************************************************************************/

void PRS_KIDEM_IHBAR_TAZMINATI_ENTEGRASTONU  ( PRS_KIDEM_IHBAR_ENT_STRUCT  * P_STRUCT , ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    USE_DB = P_GELEN_DB;
    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }

    MUH_FIS_STRUCT          * MUH_FIS          = new MUH_FIS_STRUCT;
    MUH_FIS_SATIRI_STRUCT   * MUH_FIS_SATIRI   = new MUH_FIS_SATIRI_STRUCT;
    CARI_FIS_STRUCT         * CARI_FIS         = new CARI_FIS_STRUCT;
    CARI_FIS_SATIRI_STRUCT  * CARI_FIS_SATIRI  = new CARI_FIS_SATIRI_STRUCT;

    MUH_CLEAR_FIS_STRUCT        ( MUH_FIS );
    MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI );

    MUH_FIS->fis_tarihi       = P_STRUCT->tarih;
    MUH_FIS->aciklama         = QString(QObject::tr("Fiş Personel Modülü tarafından üretilmiştir."));
    MUH_FIS->fis_turu         = ENUM_MAHSUP_FISI;
    MUH_FIS->modul_id         = P_STRUCT->modul_id;
    MUH_FIS->program_id       = P_STRUCT->program_id;
    MUH_FIS->base_fis_id      = P_STRUCT->kidem_ihbar_id;

    int muh_fis_id = MUH_FIS_EKLE(MUH_FIS , USE_DB );

    CARI_CLEAR_FIS_STRUCT ( CARI_FIS );

    CARI_FIS->aciklama              = QString(QObject::tr("Fiş Personel Modülünden üretilmiştir."));
    CARI_FIS->fis_tarihi            = P_STRUCT->tarih;
    CARI_FIS->modul_id              = P_STRUCT->modul_id;
    CARI_FIS->program_id            = P_STRUCT->program_id;
    CARI_FIS->fis_turu              = ENUM_CARI_HAREKET_FISI;
    CARI_FIS->muh_fis_id            = muh_fis_id;
    CARI_FIS->base_fis_id           = P_STRUCT->kidem_ihbar_id;

    int cari_fis_id = CARI_FIS_EKLE ( CARI_FIS , USE_DB);

    SQL_QUERY  query( DB );

    query.PREPARE_SELECT( "prs_personeller", "cari_hesap_id, sube_id ",
                          "personel_id = :personel_id" );
    query.SET_VALUE     ( ":personel_id", P_STRUCT->personel_id );

    int cari_hesap_id  = 0;
    int sube_id        = 0;

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        cari_hesap_id    = query.VALUE( "cari_hesap_id" ).toInt();
        sube_id          = query.VALUE( "sube_id" ).toInt();
    }

    int prs_kidem_tazminati_hesap_id    = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "kidem_tazminati_hesap_id");
    int prs_ihbar_tazminati_hesap_id    = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "ihbar_tazminati_hesap_id");
    int prs_gelir_vergisi_hesap_id      = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "personel_vergi_ent_hesap_id");
    int prs_damga_vergisi_hesap_id      = SUBE_GET_SUBE_ENT_HESAP_ID( sube_id, "prs_damga_vergisi_ent_hesap_id");

    MUH_FIS_SATIRI->fis_id              = muh_fis_id;
    MUH_FIS_SATIRI->base_record_id      = 0;
    MUH_FIS_SATIRI->modul_id            = P_STRUCT->modul_id;
    MUH_FIS_SATIRI->program_id          = P_STRUCT->program_id;

    // Kidem Tazminati

    if ( P_STRUCT->kidem_tazminati_tutari > 0.00 ) {

        MUH_FIS_SATIRI->aciklama      = QString ( " Personel id : %1 Kıdem Tazmınatı Tutarı." ).arg( P_STRUCT->personel_id );
        MUH_FIS_SATIRI->hesap_id      = prs_kidem_tazminati_hesap_id;
        MUH_FIS_SATIRI->borc_tutari   = P_STRUCT->kidem_tazminati_tutari;
        MUH_FIS_SATIRI->alacak_tutari = 0.00 ;

        MUH_FIS_SATIRI_EKLE( MUH_FIS ,MUH_FIS_SATIRI , USE_DB);

    }

    // Ihbar Tazminati

    if( P_STRUCT->ihbar_tazminati_tutari > 0.00 ) {

        MUH_FIS_SATIRI->aciklama      = QString ( " Personel id : %1 Ihbar Tazmınatı Tutarı." ).arg( P_STRUCT->personel_id );
        MUH_FIS_SATIRI->hesap_id      = prs_ihbar_tazminati_hesap_id;
        MUH_FIS_SATIRI->borc_tutari   = P_STRUCT->ihbar_tazminati_tutari;
        MUH_FIS_SATIRI->alacak_tutari = 0.00 ;

        MUH_FIS_SATIRI_EKLE( MUH_FIS ,MUH_FIS_SATIRI , USE_DB);
    }


    // kidem Tazminati Damga vergisi

    if( P_STRUCT->kidem_taz_damga_vergisi_tutari > 0.00 ) {

        MUH_FIS_SATIRI->aciklama      = QString ( " Personel id : %1 Kıdem Tazmınatı Damga Vergisi Tutarı." ).arg( P_STRUCT->personel_id );
        MUH_FIS_SATIRI->hesap_id      = prs_damga_vergisi_hesap_id;
        MUH_FIS_SATIRI->alacak_tutari = P_STRUCT->kidem_taz_damga_vergisi_tutari;
        MUH_FIS_SATIRI->borc_tutari   = 0.00 ;

        MUH_FIS_SATIRI_EKLE( MUH_FIS ,MUH_FIS_SATIRI , USE_DB);
    }


    // Ihbar Tazminati Damga Vergisi

    if ( P_STRUCT->ihbar_taz_damga_vergisi_tutari > 0.00 ) {
        MUH_FIS_SATIRI->aciklama      = QString ( " Personel id : %1 Ihbar Tazmınatı  Damga Vergisi Tutarı." ).arg( P_STRUCT->personel_id );
        MUH_FIS_SATIRI->hesap_id      = prs_damga_vergisi_hesap_id;
        MUH_FIS_SATIRI->alacak_tutari = P_STRUCT->ihbar_taz_damga_vergisi_tutari;
        MUH_FIS_SATIRI->borc_tutari   = 0.00 ;

        MUH_FIS_SATIRI_EKLE( MUH_FIS ,MUH_FIS_SATIRI , USE_DB);
    }

    // Ihbar Tazminati Gelir Vergisi

    if ( P_STRUCT->ihbar_taz_gelir_vergisi_tutari > 0.00 ) {

        MUH_FIS_SATIRI->aciklama      = QString ( " Personel id : %1 Ihbar Tazmınatı Gelir Vergisi Tutarı." ).arg( P_STRUCT->personel_id );
        MUH_FIS_SATIRI->hesap_id      = prs_gelir_vergisi_hesap_id;
        MUH_FIS_SATIRI->alacak_tutari = P_STRUCT->ihbar_taz_gelir_vergisi_tutari;
        MUH_FIS_SATIRI->borc_tutari   = 0.00 ;

        MUH_FIS_SATIRI_EKLE( MUH_FIS ,MUH_FIS_SATIRI , USE_DB);
    }

    double net_ucret =  ( P_STRUCT->kidem_tazminati_tutari + P_STRUCT->ihbar_tazminati_tutari ) -
                        ( P_STRUCT->kidem_taz_damga_vergisi_tutari + P_STRUCT->ihbar_taz_damga_vergisi_tutari + P_STRUCT->ihbar_taz_gelir_vergisi_tutari );

    //Personel icin cari fis satirlari ekleniyor.

    CARI_CLEAR_FIS_SATIRI_STRUCT( CARI_FIS_SATIRI );

    CARI_FIS_SATIRI->aciklama                = QString ( " Kıdem ve Ihbar Tazminati Bedeli..." );
    CARI_FIS_SATIRI->alacak_tutari           = net_ucret;
    CARI_FIS_SATIRI->cari_hesap_id           = cari_hesap_id;
    CARI_FIS_SATIRI->doviz_id                = CARI_GET_PARA_BIRIM_ID( cari_hesap_id );
    CARI_FIS_SATIRI->doviz_kodu              = DVZ_GET_DOVIZ_KODU( CARI_GET_PARA_BIRIM_ID( cari_hesap_id ) );
    CARI_FIS_SATIRI->fis_id                  = cari_fis_id;
    CARI_FIS_SATIRI->muh_hesap_id            = 0;
    CARI_FIS_SATIRI->program_id              = P_STRUCT->program_id;
    CARI_FIS_SATIRI->modul_id                = P_STRUCT->modul_id;
    CARI_FIS_SATIRI->base_record_id          = 0;


    CARI_FIS_SATIRI_EKLE(CARI_FIS , CARI_FIS_SATIRI , USE_DB);

}

/***************************************************************************************
                   PRS_CLEAR_KIDEM_IHBAR_ENT_STRUCT
***************************************************************************************/

void PRS_CLEAR_KIDEM_IHBAR_ENT_STRUCT ( PRS_KIDEM_IHBAR_ENT_STRUCT * P_STRUCT )
{
    P_STRUCT->ihbar_tazminati_tutari         = 0.00;
    P_STRUCT->kidem_tazminati_tutari         = 0.00;
    P_STRUCT->ihbar_taz_damga_vergisi_tutari = 0.00;
    P_STRUCT->ihbar_taz_gelir_vergisi_tutari = 0.00;
    P_STRUCT->kidem_taz_damga_vergisi_tutari = 0.00;
    P_STRUCT->tarih.clear();
    P_STRUCT->personel_id                    = 0;
    P_STRUCT->program_id                     = 0;
    P_STRUCT->modul_id                       = 0;

}

/***************************************************************************************
                   PRS_GET_PERSONEL_CARI_HESAP_ID
***************************************************************************************/

int PRS_GET_PERSONEL_CARI_HESAP_ID( int p_personel_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "prs_personeller", "cari_hesap_id", " personel_id = :personel_id" );
    query.SET_VALUE     ( ":personel_id", p_personel_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        return query.VALUE( 0 ).toInt();
    }

    return 0;
}

/***************************************************************************************
                    PRS_GET_CARI_PERSONEL_ID
***************************************************************************************/

int PRS_GET_CARI_PERSONEL_ID( int p_cari_hesap_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "prs_personeller", "personel_id", " cari_hesap_id = :cari_hesap_id" );
    query.SET_VALUE     ( ":cari_hesap_id", p_cari_hesap_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        return query.VALUE( 0 ).toInt();
    }

    return 0;
}

/***************************************************************************************
                    PRS_GET_KUMULATIF_VERGI_ORANI
***************************************************************************************/

double PRS_GET_KUMULATIF_VERGI_ORANI( double p_kumulatif_vergi_tutari )
{

    double GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI  [ GELIR_VERGISI_DILIMI_SAYISI ];
    double GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI      [ GELIR_VERGISI_DILIMI_SAYISI ];
    double GELIR_VERGISI_YUZDELERI                      [ GELIR_VERGISI_DILIMI_SAYISI ];

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "prs_sabit_degerler",
                          "glr_ver_dilim_bas_tutar_array,"
                          "glr_ver_dilim_bts_tutar_array,"
                          "gelir_vergisi_yuzdeleri_array",
                          "sabit_degerler_id = :sabit_degerler_id" );
    query.SET_VALUE     ( ":sabit_degerler_id", 1 );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        UNPACK_DOUBLE_ARRAY (query.VALUE("glr_ver_dilim_bas_tutar_array").toString() , GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI,
                             GELIR_VERGISI_DILIMI_SAYISI);
        UNPACK_DOUBLE_ARRAY (query.VALUE("glr_ver_dilim_bts_tutar_array").toString() , GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI,
                             GELIR_VERGISI_DILIMI_SAYISI);
        UNPACK_DOUBLE_ARRAY (query.VALUE("gelir_vergisi_yuzdeleri_array").toString() , GELIR_VERGISI_YUZDELERI,GELIR_VERGISI_DILIMI_SAYISI);

        for ( int i = 0; i < GELIR_VERGISI_DILIMI_SAYISI; i++ ) {
            if ( p_kumulatif_vergi_tutari >= GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI[ i ] AND
                 p_kumulatif_vergi_tutari <= GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI[i] ) {
                return GELIR_VERGISI_YUZDELERI[i];
            }
         }
    }
    return 0;
}

/***************************************************************************************
                    PRS_ISYERI_5510_KANUNA_TABI_MI
***************************************************************************************/

int PRS_ISYERI_5510_KANUNA_TABI_MI()
{
    SQL_QUERY query( DB );
    query.PREPARE_SELECT( "prs_sabit_degerler",
                          "isy_5510_nolu_kanuna_tabi_mi",
                          "sabit_degerler_id = :sabit_degerler_id" );
    query.SET_VALUE     ( ":sabit_degerler_id", 1 );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        return query.VALUE( 0 ).toInt();
    }

    return 0;
}

/**************************************************************************************
                   PRS_GET_ONCEKI_AYLARDAN_KALAN_EK_ODENEKLER
***************************************************************************************/

void  PRS_GET_ONCEKI_AYLARDAN_KALAN_EK_ODENEKLER(int p_personel_id, double *  p_bir_ay_once_kalan_ek_odenek, double *  p_iki_ay_once_kalan_ek_odenek )
{
    SQL_QUERY       sql_query ( DB );

    sql_query.PREPARE_SELECT("prs_personeller","bir_ay_once_kalan_ek_odenek, iki_ay_once_kalan_ek_odenek ",
                             "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", p_personel_id );

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();

        *p_bir_ay_once_kalan_ek_odenek = sql_query.VALUE( 0 ).toDouble();
        *p_iki_ay_once_kalan_ek_odenek = sql_query.VALUE( 1 ).toDouble();

    }
}

/**************************************************************************************
                   PRS_GET_CALISMA_SEKLI_ENUM
***************************************************************************************/

int PRS_GET_CALISMA_SEKLI_ENUM(QString p_calisma_sekli)
{
    if ( p_calisma_sekli EQ QObject::tr("AYLIK") ) {
        return ENUM_AYLIK;
    }
    if ( p_calisma_sekli EQ QObject::tr( "GÜNLÜK") ) {
        return ENUM_GUNLUK;
    }

    return -1;
}

/**************************************************************************************
                   PRS_GET_CALISMA_DURUMU_STRING
***************************************************************************************/

QString PRS_GET_CALISMA_SEKLI_STRING(int p_calisma_sekli)
{
    if ( p_calisma_sekli EQ ENUM_AYLIK ) {
        return QObject::tr("AYLIK");
    }
    if ( p_calisma_sekli EQ ENUM_GUNLUK ) {
        return QObject::tr( "GÜNLÜK");
    }

    return "null";
}

/**************************************************************************************
                   PRS_SIRADAKI_ODENEK_KESINTI_NO_AL
***************************************************************************************/

int PRS_SIRADAKI_ODENEK_KESINTI_NO_AL ( QString p_fis_tarihi )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "prs_kesinti_odenek_tanimlar",
                               "kesinti_odenek_no",
                               "kesinti_odenek_tarihi = :kesinti_odenek_tarihi" );
    sql_query.SET_VALUE      ( ":kesinti_odenek_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT( "kesinti_odenek_no DESC", 0, 1 ) EQ 0 ) {
        return 1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt() + 1;
}

/**************************************************************************************
                   PRS_GET_AYLIK_YEMEK_VERILEN_GUN_SAYISI
***************************************************************************************/

double PRS_GET_AYLIK_YEMEK_VERILEN_GUN_SAYISI(int p_personel_id, int p_bordro_ay, int p_bordro_yil)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "prs_puantaj ","yemek_verilen_gun_sayisi",
                          "bordro_donemi_yil    = :bordro_donemi_yil    "
                          "AND personel_id      = :personel_id          "
                          "AND bordro_donemi_ay = :bordro_donemi_ay     ");

    query.SET_VALUE     ( ":bordro_donemi_ay"  , p_bordro_ay     );
    query.SET_VALUE     ( ":bordro_donemi_yil" , p_bordro_yil   );
    query.SET_VALUE     ( ":personel_id"       , p_personel_id   );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toDouble();
}

/**************************************************************************************
                   PRS_GET_EKSIK_GUN_SAYISI
***************************************************************************************/

double PRS_GET_EKSIK_GUN_SAYISI ( int p_personel_id, int p_bordro_ay, int p_bordro_yil )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "prs_eksik_gun_nedenleri","eksik_gun_sayisi",
                          "bordro_donemi_yil    = :bordro_donemi_yil    "
                          "AND personel_id      = :personel_id          "
                          "AND bordro_donemi_ay = :bordro_donemi_ay     ");

    query.SET_VALUE     ( ":bordro_donemi_ay"  , p_bordro_ay     );
    query.SET_VALUE     ( ":bordro_donemi_yil" , p_bordro_yil   );
    query.SET_VALUE     ( ":personel_id"       , p_personel_id   );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toDouble();
}

/**************************************************************************************
                   GET_BORDRO_DONEMI_STRING
***************************************************************************************/

QString PRS_GET_BORDRO_DONEMI_BAS_STRING ( int p_yil, int p_ay )
{
    QString bordro_donemi = QString::number( p_yil ) +"."+QString::number( p_ay );
    if ( p_ay < 10 ) {
        bordro_donemi = QString::number( p_yil ) +".0"+QString::number( p_ay );
    }

    bordro_donemi.append( ".01");

    return bordro_donemi;
}
/**************************************************************************************
                   GET_BORDRO_DONEMI_STRING
***************************************************************************************/

QString PRS_GET_BORDRO_DONEMI_BTS_STRING ( int p_yil, int p_ay )
{
    QString bordro_donemi = QString::number( p_yil ) +"."+QString::number( p_ay );
    if ( p_ay < 10 ) {
        bordro_donemi = QString::number( p_yil ) +".0"+QString::number( p_ay );
    }
    //AYIN KAC GUN CEK
    QDate gecici_date = QDate::fromString( bordro_donemi, "yyyy.MM");

    int kac_gun_cekiyor = gecici_date.daysInMonth();

    bordro_donemi.append( "."+ QVariant( kac_gun_cekiyor ).toString() );

    return bordro_donemi;
}

/**************************************************************************************
                   PRS_GET_GUNCEL_CALISAN_PERSONEL_IDLERI
***************************************************************************************/

QList < int >   PRS_GET_GUNCEL_CALISAN_PERSONEL_IDLERI ( int p_bordro_ay, int p_bordro_yil )
{
    QList < int > calisan_personel_list;
    QString bordro_donemi = PRS_GET_BORDRO_DONEMI_BTS_STRING( p_bordro_yil, p_bordro_ay );

    SQL_QUERY f_query( DB );
    SQL_QUERY s_query( DB );

    f_query.PREPARE_SELECT( "prs_personeller, prs_personel_kisisel_blgler", "prs_personeller.personel_id, ise_giris_tarihi",
                            "prs_personeller.personel_id = prs_personel_kisisel_blgler.personel_id" );

    if ( f_query.SELECT() NE 0 ) {
        while ( f_query.NEXT() EQ true ) {
            int     personel_id      = f_query.VALUE( 0 ).toInt();
            QString ise_giris_tarihi = f_query.VALUE( 1 ).toString();

            s_query.PREPARE_SELECT( "prs_isten_ayrilanlar", "isten_ayrilma_tarihi", "personel_id = :personel_id");
            s_query.SET_VALUE     ( ":personel_id", personel_id );

            if ( s_query.SELECT() NE 0 ) {
                s_query.NEXT();

                //QString isten_ayrilma_tarihi = s_query.VALUE( 0 ).toString();

                if ( ise_giris_tarihi < bordro_donemi ) {
                    calisan_personel_list << personel_id;
                }
            }

        }
    }



    return calisan_personel_list;
}

/**************************************************************************************
                   PRS_GET_BORDRO_DONEMI
***************************************************************************************/

QString PRS_GET_BORDRO_DONEMI(int p_bordro_id)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "prs_bordrolar", "bordro_donemi_ay", "bordro_id = :bordro_id");
    query.SET_VALUE     ( ":bordro_id", p_bordro_id );

    if ( query.SELECT() EQ 0 ) {
        return "NULL";
    }

    query.NEXT();

    return E9_GET_AYLAR_STRING( query.VALUE( 0 ).toInt() );
}
