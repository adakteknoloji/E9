#include "adak_sql.h"
#include "adak_utils.h"
#include "sube_enum.h"
#include "sube_struct.h"
#include "yonetim.h"
#include "muh_console_utils.h"
#include "e9_yetki.h"
#include "sube_console_utils.h"
 

extern ADAK_SQL *   DB;

SUBE_ENTEGRASYON_HESAPLARI_STRUCT   * G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT = new SUBE_ENTEGRASYON_HESAPLARI_STRUCT;

static int         g_sube_id             =   -1;
static int         g_kasa_unite_id       =   -1;


/**************************************************************************************
                   SUBE_CREATE_DEFAULT_SUBE_DEPO_ENT_HESAPLARI
***************************************************************************************/

void SUBE_CREATE_DEFAULT_SUBE_DEPO_UNITE_ENT_HESAPLARI()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "sub_subeler","sube_id" );
    if ( query.SELECT() > 0 ) {
        return;
    }

    query.PREPARE_INSERT   (   " sub_subeler","sube_id","sube_kodu, sube_adi,sube_port_cekler_hsp_id, "
                               " sube_tah_cekler_hsp_id,           sube_tem_cekler_hsp_id,           sube_kar_cekler_hsp_id, "
                               " sube_port_snt_hsp_id,             sube_tah_snt_hsp_id,              sube_tem_snt_hsp_id, "
                               " sube_pro_snt_hsp_id,              sube_port_snt_ileri_trh_hsp_id,   sube_tah_snt_ileri_trh_hsp_id, "
                               " sube_tem_snt_ileri_trh_hsp_id,    sube_borc_snt_hsp_id,             sube_borc_snt_ileri_trh_hsp_id, "
                               " sube_sup_alacaklar_hsp_id,        sube_tah_edlmyn_cekler_hsp_id,    sube_tah_edlmyn_snt_hsp_id, "
                               " sube_yurtici_satis_hsp_id,        sube_hizmet_urt_maliyet_hsp_id, "
                               " sube_prs_gdrlr_hsp_id         , sube_sarf_malzemeleri_hsp_id ,"
                               " sube_imalattan_giris_hsp_id    , "
                               " kasa_ent_hesap_id              ,                                    "
                               " cari_alici_ent_hesap_id        , cari_satici_ent_hesap_id ,         "
                               " banka_ent_hesap_id             , ver_cek_ent_hesap_id             , "
                               " odeme_emirleri_ent_hesap_id    ,                                    "
                               " pos_ent_hesap_id               ,                                    "
                               " kk_ent_hesap_id                , prk_gider_ent_hesap_id           , "
                               " urunler_ent_hesap_id           ,                                    "
                               " hizmetler_ent_hesap_id         , demirbaslar_ent_hesap_id,          "
                               " pos_kom_gideri_ent_hesap_id    ,                                    "
                               " prs_avanslari_ent_hesap_id     , personel_borclari_ent_hesap_id   , "
                               " prs_damga_vergisi_ent_hesap_id , personel_vergi_ent_hesap_id      , "
                               " prk_gelir_ent_hesap_id         ,                                    "
                               " prs_ssk_isci_payi_hesap_id     ,                                    "
                               " prs_ssk_isveren_payi_hesap_id  ,                                    "
                               " prs_issizlik_sgk_isci_hsp_id   ,                                    "
                               " prs_issizlik_sgk_isv_hsp_id    ,                                    "
                               " asg_gecim_ind_hesap_id         ,                                    "
                               " isveren_payi_giderleri_hps_id  ,                                    "
                               " bir_amortismanlar_hsp_id       ,                                    "
                               " amor_giderleri_hsp_id          ,                                    "
                               " sgdp_prim_yuzde_isc_hsp_id     , sgdp_prim_yuzde_isv_hsp_id        ,"
                               " kidem_tazminati_hesap_id       , ihbar_tazminati_hesap_id          ,"
                               " dmr_satis_kar_ent_hesap_id     , dmr_satis_zarar_ent_hesap_id      ,"
                               " sendika_odeme_hesap_id         , smmm_alinan_hizmet_hesap_id       ,"
                               " smmm_gelir_vergisi_hesap_id    , smmm_satilan_hizmet_hesap_id      ,"
                               " isy_5510_nolu_kanun_hesap_id   , satistan_iade_hesap_id            ,"
                               " ortaklara_borclar_hesap_id   ");

    query.SET_VALUE ( ":sube_kodu"                          , "MRK"                   );
    query.SET_VALUE ( ":sube_adi"                           , "MERKEZ"                );
    query.SET_VALUE ( ":sube_port_cekler_hsp_id"            , MUH_GET_HESAP_ID("101") );
    query.SET_VALUE ( ":sube_tah_cekler_hsp_id"             , MUH_GET_HESAP_ID("101") );
    query.SET_VALUE ( ":sube_tem_cekler_hsp_id"             , MUH_GET_HESAP_ID("101") );
    query.SET_VALUE ( ":sube_kar_cekler_hsp_id"             , MUH_GET_HESAP_ID("101") );
    query.SET_VALUE ( ":sube_port_snt_hsp_id"               , MUH_GET_HESAP_ID("121") );
    query.SET_VALUE ( ":sube_tah_snt_hsp_id"                , MUH_GET_HESAP_ID("121") );
    query.SET_VALUE ( ":sube_tem_snt_hsp_id"                , MUH_GET_HESAP_ID("121") );
    query.SET_VALUE ( ":sube_pro_snt_hsp_id"                , MUH_GET_HESAP_ID("121") );
    query.SET_VALUE ( ":sube_port_snt_ileri_trh_hsp_id"     , MUH_GET_HESAP_ID("221") );
    query.SET_VALUE ( ":sube_tah_snt_ileri_trh_hsp_id"      , MUH_GET_HESAP_ID("221") );
    query.SET_VALUE ( ":sube_tem_snt_ileri_trh_hsp_id"      , MUH_GET_HESAP_ID("221") );
    query.SET_VALUE ( ":sube_borc_snt_hsp_id"               , MUH_GET_HESAP_ID("321") );
    query.SET_VALUE ( ":sube_borc_snt_ileri_trh_hsp_id"     , MUH_GET_HESAP_ID("421") );
    query.SET_VALUE ( ":sube_sup_alacaklar_hsp_id"          , MUH_GET_HESAP_ID("128") );
    query.SET_VALUE ( ":sube_tah_edlmyn_cekler_hsp_id"      , MUH_GET_HESAP_ID("101") );
    query.SET_VALUE ( ":sube_tah_edlmyn_snt_hsp_id"         , MUH_GET_HESAP_ID("121") );
    query.SET_VALUE ( ":sube_yurtici_satis_hsp_id"          , MUH_GET_HESAP_ID("600") );
    query.SET_VALUE ( ":sube_hizmet_urt_maliyet_hsp_id"     , MUH_GET_HESAP_ID("740") );
    query.SET_VALUE ( ":sube_prs_gdrlr_hsp_id"              , MUH_GET_HESAP_ID("770") );
    query.SET_VALUE ( ":sube_sarf_malzemeleri_hsp_id"       , MUH_GET_HESAP_ID("710") );
    query.SET_VALUE ( ":sube_imalattan_giris_hsp_id"        , MUH_GET_HESAP_ID("152") );
    query.SET_VALUE ( ":kasa_ent_hesap_id"                  , MUH_GET_HESAP_ID("100") );
    query.SET_VALUE ( ":cari_alici_ent_hesap_id"            , MUH_GET_HESAP_ID("120") );
    query.SET_VALUE ( ":cari_satici_ent_hesap_id"           , MUH_GET_HESAP_ID("320") );
    query.SET_VALUE ( ":banka_ent_hesap_id"                 , MUH_GET_HESAP_ID("102") );
    query.SET_VALUE ( ":ver_cek_ent_hesap_id"               , MUH_GET_HESAP_ID("103") );
    query.SET_VALUE ( ":odeme_emirleri_ent_hesap_id"        , MUH_GET_HESAP_ID("103") );
    query.SET_VALUE ( ":pos_ent_hesap_id"                   , MUH_GET_HESAP_ID("108") );
    query.SET_VALUE ( ":kk_ent_hesap_id"                    , MUH_GET_HESAP_ID("300") );
    query.SET_VALUE ( ":prk_gider_ent_hesap_id"             , MUH_GET_HESAP_ID("770") );
    query.SET_VALUE ( ":urunler_ent_hesap_id"               , MUH_GET_HESAP_ID("153") );
    query.SET_VALUE ( ":hizmetler_ent_hesap_id"             , MUH_GET_HESAP_ID("770") );
    query.SET_VALUE ( ":demirbaslar_ent_hesap_id"           , MUH_GET_HESAP_ID("255") );
    query.SET_VALUE ( ":pos_kom_gideri_ent_hesap_id"        , MUH_GET_HESAP_ID("780") );
    query.SET_VALUE ( ":prs_avanslari_ent_hesap_id"         , MUH_GET_HESAP_ID("196") );
    query.SET_VALUE ( ":personel_borclari_ent_hesap_id"     , MUH_GET_HESAP_ID("335") );
    query.SET_VALUE ( ":prs_damga_vergisi_ent_hesap_id"     , MUH_GET_HESAP_ID("360") );
    query.SET_VALUE ( ":personel_vergi_ent_hesap_id"        , MUH_GET_HESAP_ID("360") );
    query.SET_VALUE ( ":prk_gelir_ent_hesap_id"             , MUH_GET_HESAP_ID("600") );
    query.SET_VALUE ( ":prs_ssk_isci_payi_hesap_id"         , MUH_GET_HESAP_ID("361") );
    query.SET_VALUE ( ":prs_ssk_isveren_payi_hesap_id"      , MUH_GET_HESAP_ID("361") );
    query.SET_VALUE ( ":prs_issizlik_sgk_isci_hsp_id"       , MUH_GET_HESAP_ID("361") );
    query.SET_VALUE ( ":prs_issizlik_sgk_isv_hsp_id"        , MUH_GET_HESAP_ID("361") );
    query.SET_VALUE ( ":asg_gecim_ind_hesap_id"             , MUH_GET_HESAP_ID("360") );
    query.SET_VALUE ( ":isveren_payi_giderleri_hps_id"      , MUH_GET_HESAP_ID("770") );
    query.SET_VALUE ( ":bir_amortismanlar_hsp_id"           , MUH_GET_HESAP_ID("257") );
    query.SET_VALUE ( ":amor_giderleri_hsp_id"              , MUH_GET_HESAP_ID("770") );
    query.SET_VALUE ( ":sgdp_prim_yuzde_isc_hsp_id"         , MUH_GET_HESAP_ID("361") );
    query.SET_VALUE ( ":sgdp_prim_yuzde_isv_hsp_id"         , MUH_GET_HESAP_ID("361") );
    query.SET_VALUE ( ":kidem_tazminati_hesap_id"           , MUH_GET_HESAP_ID("770") );
    query.SET_VALUE ( ":ihbar_tazminati_hesap_id"           , MUH_GET_HESAP_ID("770") );
    query.SET_VALUE ( ":dmr_satis_kar_ent_hesap_id"         , MUH_GET_HESAP_ID("679") );
    query.SET_VALUE ( ":dmr_satis_zarar_ent_hesap_id"       , MUH_GET_HESAP_ID("689") );
    query.SET_VALUE ( ":sendika_odeme_hesap_id"             , MUH_GET_HESAP_ID("770") );
    query.SET_VALUE ( ":smmm_alinan_hizmet_hesap_id"        , MUH_GET_HESAP_ID("770") );
    query.SET_VALUE ( ":smmm_gelir_vergisi_hesap_id"        , MUH_GET_HESAP_ID("360") );
    query.SET_VALUE ( ":smmm_satilan_hizmet_hesap_id"       , MUH_GET_HESAP_ID("600") );
    query.SET_VALUE ( ":isy_5510_nolu_kanun_hesap_id"       , MUH_GET_HESAP_ID("602") );
    query.SET_VALUE ( ":satistan_iade_hesap_id"             , MUH_GET_HESAP_ID("610") );
    query.SET_VALUE ( ":ortaklara_borclar_hesap_id"         , MUH_GET_HESAP_ID("331") );

    int sube_id = query.INSERT();

    query.PREPARE_INSERT ( "sub_depolar ","depo_id", "depo_kodu, depo_adi, sube_id ");

    query.SET_VALUE      ( ":sube_id", sube_id );
    query.SET_VALUE      (":depo_kodu","001");
    query.SET_VALUE      (":depo_adi","MERKEZ DEPO");

    int depo_id = query.INSERT();


    query.PREPARE_INSERT ( "sub_uniteler ","unite_id", "sube_id, unite_adi, order_number, "
                             "bilgisayar_id, depo_id, irsaliye_belge_seri, fatura_belge_seri, "
                             "tahsilat_makbuzu_belge_seri, odeme_makbuzu_belge_seri, "
                             "irsaliye_belge_numarasi, fatura_belge_numarasi, "
                             "tahsilat_makbuzu_belge_num, odeme_makbuzu_belge_num , "
                             "muh_kasa_hesap_id, irsaliyeli_fat_belge_seri , "
                             "irsaliyeli_fat_belge_numarasi ");

    query.SET_VALUE      ( ":unite_adi"                     , "MERKEZ KASA");
    query.SET_VALUE      ( ":order_number"                  , 1024);
    query.SET_VALUE      ( ":irsaliyeli_fat_belge_seri"     , "A");
    query.SET_VALUE      ( ":irsaliye_belge_seri"           , "A");
    query.SET_VALUE      ( ":fatura_belge_seri"             , "A");
    query.SET_VALUE      ( ":tahsilat_makbuzu_belge_seri"   , "A");
    query.SET_VALUE      ( ":odeme_makbuzu_belge_seri"      , "A");
    query.SET_VALUE      ( ":irsaliyeli_fat_belge_numarasi" , QVariant ( 1 ).toString() );
    query.SET_VALUE      ( ":irsaliye_belge_numarasi"       , QVariant ( 1 ).toString() );
    query.SET_VALUE      ( ":fatura_belge_numarasi"         , QVariant ( 1 ).toString() );
    query.SET_VALUE      ( ":tahsilat_makbuzu_belge_num"    , QVariant ( 1 ).toString());
    query.SET_VALUE      ( ":odeme_makbuzu_belge_num"       , QVariant ( 1 ).toString() );
    query.SET_VALUE      ( ":sube_id"                       , sube_id );
    query.SET_VALUE      ( ":bilgisayar_id"                 , BILGISAYAR_ID() );
    query.SET_VALUE      ( ":depo_id"                       , depo_id );
    query.SET_VALUE      ( ":muh_kasa_hesap_id"             , MUH_GET_HESAP_ID("100"));

    query.INSERT();
}

/**************************************************************************************
                   SUBE_GET_DEPO_ISLEM_YETKISI_ENUM
***************************************************************************************/

int SUBE_GET_DEPO_ISLEM_YETKISI_ENUM ( QString p_depo_islem_yetkisi )
{

    if ( p_depo_islem_yetkisi EQ QObject::tr ( "SADECE ÜNİTE DEPOSUNDA İŞLEM YAPABİLİR" ) ) {
        return SADECE_UNITE_DEPOSUNDA;
    }
    else if ( p_depo_islem_yetkisi EQ QObject::tr ( "TÜM DEPOLARDA İŞLEM YAPABİLİR" ) ) {
        return TUM_DEPOLARDA;
    }

    return -1;
}


/**************************************************************************************
                   SUBE_GET_DEPO_ISLEM_YETKISI_STRING
***************************************************************************************/

QString SUBE_GET_DEPO_ISLEM_YETKISI_STRING ( int p_depo_islem_yetkisi )
{

    switch ( p_depo_islem_yetkisi ) {

        case SADECE_UNITE_DEPOSUNDA :
            return QObject::tr ( "SADECE ÜNİTE DEPOSUNDA İŞLEM YAPABİLİR" );

        case TUM_DEPOLARDA :
            return QObject::tr ( "TÜM DEPOLARDA İŞLEM YAPABİLİR" );

        default:
            break;
    }
    return "";
}

/**************************************************************************************
                   SUBE_GET_ISLEM_YETKISI_ENUM
***************************************************************************************/

int SUBE_GET_ISLEM_YETKISI_ENUM ( QString p_sube_islem_yetkisi )
{
    if ( p_sube_islem_yetkisi EQ QObject::tr ( "SADECE ÜNİTENİN BULUNDUĞU ŞUBEDE İŞLEM YAPABİLİR") ) {
        return SADECE_UNITE_SUBESINDE;
    }
    else if ( p_sube_islem_yetkisi EQ QObject::tr ( "TÜM ŞUBELERDE İŞLEM YAPABİLİR" )) {
        return TUM_SUBELERDE;
    }
    return -1;
}


/**************************************************************************************
                   GET_SUBE_SUBE_ISLEM_YETKISI_STRING
***************************************************************************************/

QString SUBE_GET_ISLEM_YETKISI_STRING ( int p_sube_islem_yetkisi )
{

    switch ( p_sube_islem_yetkisi ) {

        case SADECE_UNITE_SUBESINDE  :
            return QObject::tr ( "SADECE ÜNİTENİN BULUNDUĞU ŞUBEDE İŞLEM YAPABİLİR");

        case TUM_SUBELERDE:
            return QObject::tr ( "TÜM ŞUBELERDE İŞLEM YAPABİLİR");

        default:
            break;
    }
    return "";
}





/**************************************************************************************
                   SUBE_GET_DEPO_KODU_ADI
***************************************************************************************/

int SUBE_GET_DEPO_KODU_ADI ( int p_depo_id, QString * p_depo_kodu, QString * p_depo_adi )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "sub_depolar","depo_kodu, depo_adi","depo_id = :depo_id");
    sql_query.SET_VALUE      ( ":depo_id", p_depo_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    if( p_depo_kodu NE NULL ){
        *p_depo_kodu = sql_query.VALUE(0).toString();
    }
    if ( p_depo_adi NE NULL ) {
        *p_depo_adi  = sql_query.VALUE(1).toString();
    }

    return 1;
}

/**************************************************************************************
                   SUBE_GET_UNITENIN_DEPOSU
***************************************************************************************/

int SUBE_GET_UNITENIN_DEPOSU ()
{

    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_uniteler ","depo_id","unite_id = :unite_id");
    sql_query.SET_VALUE      ( ":unite_id", SUBE_GET_UNITE_ID() );

    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_SET_CURRENT_SUBE_UNITE
***************************************************************************************/

bool SUBE_SET_CURRENT_SUBE_UNITE()
{
    int bilgisayar_id = BILGISAYAR_ID();
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_uniteler","unite_id, sube_id ",
                               "bilgisayar_id = :bilgisayar_id");
    sql_query.SET_VALUE      ( ":bilgisayar_id", bilgisayar_id  );
    if ( sql_query.SELECT() EQ 0 ) {
        SUBE_SET_SUBE_ID        ( -1 );
        SUBE_SET_UNITE_ID       ( -1 );
        return false;
    }
    sql_query.NEXT();
    SUBE_SET_UNITE_ID      ( sql_query.VALUE(0).toInt() );
    SUBE_SET_SUBE_ID       ( sql_query.VALUE(1).toInt() );
    return true;
}

/**************************************************************************************
                   SUBE_GET_ENTEGRAGRASYON_HESAPLARI
***************************************************************************************/

void * SUBE_GET_ENTEGRASYON_HESAPLARI_STRUCT()
{
    return G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT;
}


/**************************************************************************************
                  SUBE_FILL_ENTEGRASYON_HESAPLARI_STRUCT();
***************************************************************************************/

void SUBE_FILL_ENTEGRASYON_HESAPLARI_STRUCT()
{
    SQL_QUERY sql_query  ( DB );
    sql_query.PREPARE_SELECT ( "sub_subeler","sube_port_cekler_hsp_id, sube_tah_cekler_hsp_id, sube_tem_cekler_hsp_id, "
                               "sube_kar_cekler_hsp_id, sube_port_snt_hsp_id, "
                               "sube_tah_snt_hsp_id, sube_tem_snt_hsp_id, sube_pro_snt_hsp_id, "
                               "sube_port_snt_ileri_trh_hsp_id, sube_tah_snt_ileri_trh_hsp_id, "
                               "sube_tem_snt_ileri_trh_hsp_id, sube_borc_snt_hsp_id, "
                               "sube_borc_snt_ileri_trh_hsp_id, sube_sup_alacaklar_hsp_id, "
                               "sube_tah_edlmyn_cekler_hsp_id, sube_tah_edlmyn_snt_hsp_id, "
                               "sube_yurtici_satis_hsp_id, sube_hizmet_urt_maliyet_hsp_id, "
                               "sube_imalattan_giris_hsp_id,   "
                               "sube_prs_gdrlr_hsp_id , sube_sarf_malzemeleri_hsp_id, satistan_iade_hesap_id ",
                               "sube_id = :sube_id" );
    sql_query.SET_VALUE      ( ":sube_id", g_sube_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();

    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->port_cekler_hsp_id                = sql_query.VALUE ( "sube_port_cekler_hsp_id"          ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->tah_verilen_cekler_hsp_id         = sql_query.VALUE ( "sube_tah_cekler_hsp_id"           ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->tem_verilen_cekler_hsp_id         = sql_query.VALUE ( "sube_tem_cekler_hsp_id"           ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->kar_cekler_hsp_id                 = sql_query.VALUE ( "sube_kar_cekler_hsp_id"           ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->port_senetler_hsp_id              = sql_query.VALUE ( "sube_port_snt_hsp_id"             ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->tah_senetler_hsp_id               = sql_query.VALUE ( "sube_tah_snt_hsp_id"              ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->tem_senetler_hsp_id               = sql_query.VALUE ( "sube_tem_snt_hsp_id"              ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->pro_senetler_hsp_id               = sql_query.VALUE ( "sube_pro_snt_hsp_id"              ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->port_senetler_ileri_trh_hsp_id    = sql_query.VALUE ( "sube_port_snt_ileri_trh_hsp_id"   ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->tah_senetler_ileri_trh_hsp_id     = sql_query.VALUE ( "sube_tah_snt_ileri_trh_hsp_id"    ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->tem_senetler_ileri_trh_hsp_id     = sql_query.VALUE ( "sube_tem_snt_ileri_trh_hsp_id"    ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->borc_senetler_hsp_id              = sql_query.VALUE ( "sube_borc_snt_hsp_id"             ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->borc_senetleri_ileri_trh_hsp_id   = sql_query.VALUE ( "sube_borc_snt_ileri_trh_hsp_id"   ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->sup_alacaklar_hsp_id              = sql_query.VALUE ( "sube_sup_alacaklar_hsp_id"        ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->tahsil_edilemeyen_cekler_hsp_id   = sql_query.VALUE ( "sube_tah_edlmyn_cekler_hsp_id"    ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->tahsil_edilemeyen_senetler_hsp_id = sql_query.VALUE ( "sube_tah_edlmyn_snt_hsp_id"       ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->yurtici_sts_hsp_id                = sql_query.VALUE ( "sube_yurtici_satis_hsp_id"        ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->hizmet_uretim_maliyeti_hsp_id     = sql_query.VALUE ( "sube_hizmet_urt_maliyet_hsp_id" ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->imalattan_giris_hesap_id          = sql_query.VALUE ( "sube_imalattan_giris_hsp_id"      ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->prs_giderler_hesap_id             = sql_query.VALUE ( "sube_prs_gdrlr_hsp_id"            ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->sarf_malzemeleri_hesap_id         = sql_query.VALUE ( "sube_sarf_malzemeleri_hsp_id"     ).toInt();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->satistan_iade_hesap_id            = sql_query.VALUE ( "satistan_iade_hesap_id"           ).toInt();

    sql_query.PREPARE_SELECT ( "sub_uniteler","muh_kasa_hesap_id","unite_id = :unite_id");
    sql_query.SET_VALUE      ( ":unite_id", g_kasa_unite_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    G_SUBE_ENTEGRASYON_HESAPLARI_STRUCT->unite_muh_kasa_hsp_id = sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_GET_UNITE_MUH_KASA_HESABI_ID
***************************************************************************************/

int SUBE_GET_UNITE_MUH_KASA_HESABI_ID()
{
   SQL_QUERY select_query ( DB );

   select_query.PREPARE_SELECT ( "sub_uniteler","muh_kasa_hesap_id ",
                                 "unite_id = :unite_id" );

   select_query.SET_VALUE ( ":unite_id",g_kasa_unite_id );

   if ( select_query.SELECT() EQ 0 ) {
       return 0;
   }
   select_query.NEXT();
   int kasa_unite_muh_kasa_hesabi_id = select_query.VALUE(0).toInt();

   if ( kasa_unite_muh_kasa_hesabi_id EQ 0 ) {
       return 0;
   }
   return kasa_unite_muh_kasa_hesabi_id;
}

/**************************************************************************************
                   SUBE_GET_UNITE_ID
***************************************************************************************/

int SUBE_GET_UNITE_ID ()
{
    return g_kasa_unite_id;
}

/**************************************************************************************
                   SUBE_SET_UNITE_ID
***************************************************************************************/

void SUBE_SET_UNITE_ID ( int p_value )
{
    g_kasa_unite_id = p_value;
}

/**************************************************************************************
                   SUBE_GET_SUBE_ID
***************************************************************************************/

int SUBE_GET_SUBE_ID ()
{
    return g_sube_id;
}

/**************************************************************************************
                   SUBE_SET_SUBE_ID
***************************************************************************************/

void SUBE_SET_SUBE_ID ( int p_value )
{
    g_sube_id = p_value;
}


/**************************************************************************************
                   SUBE_GET_DEPO_ISLEM_YETKISI
***************************************************************************************/

int SUBE_GET_DEPO_ISLEM_YETKISI ()
{
    E9_KULLANICI_YETKILERI_STRUCT * KULLANICI_YETKILERI =  ( E9_KULLANICI_YETKILERI_STRUCT *) E9_GET_KULLANICI_YETKILERI();
    return KULLANICI_YETKILERI->sub_depo_islem_yetkisi;
}

/**************************************************************************************
                   SUBE_GET_SUBE_ISLEM_YETKISI
***************************************************************************************/

int SUBE_GET_SUBE_ISLEM_YETKISI ()
{
    E9_KULLANICI_YETKILERI_STRUCT * KULLANICI_YETKILERI =  ( E9_KULLANICI_YETKILERI_STRUCT *) E9_GET_KULLANICI_YETKILERI();
    return KULLANICI_YETKILERI->sub_sube_islem_yetkisi;
}

/**************************************************************************************
                   SUBE_GET_DEPO_ID
***************************************************************************************/

int SUBE_GET_DEPO_ID ( QString p_depo_kodu , int p_sube_id  )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "sub_depolar","depo_id ","depo_kodu = :depo_kodu" );
    select_query.SET_VALUE      ( ":depo_kodu" , p_depo_kodu );

    if ( p_sube_id NE 0 ) {
        select_query.AND_EKLE   ( "sube_id = :sube_id" );
        select_query.SET_VALUE  ( ":sube_id", p_sube_id );
    }

    if ( select_query.SELECT() EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_GET_DEPO_KODU
***************************************************************************************/

QString SUBE_GET_DEPO_KODU ( int p_depo_id )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "sub_depolar","depo_kodu","depo_id = :depo_id" );
    select_query.SET_VALUE      ( ":depo_id" , p_depo_id );

    if ( select_query.SELECT() EQ 0 ) {
        return "";
    }
    select_query.NEXT();
    return select_query.VALUE(0).toString();
}

/**************************************************************************************
                   SUBE_DEPO_ISLEM_YETKI_KONTROLU
***************************************************************************************/

bool SUBE_DEPO_ISLEM_YETKI_KONTROLU ( QWidget * p_parent )
{
    switch ( SUBE_GET_DEPO_ISLEM_YETKISI() ) {


        case SADECE_UNITE_DEPOSUNDA :

            if ( SUBE_GET_UNITENIN_DEPOSU() EQ 0 ) {
                ADAK_WARNING( QObject::tr ( "Kullanıcı yetkisi sadece ünite deposunda işlem yapmaya izin veriyor. Bu ünite herhangi bir depoya kaydedilmemiş. "
                                                                                                  "\n Stok->İşlem->Depo kartında bir depoya bu üniteyi kaydetmelisiniz."), NULL, p_parent );

                return false;
            }
            break;

        case TUM_DEPOLARDA :
        default            :

            SQL_QUERY sql_query ( DB );
            sql_query.PREPARE_SELECT ( "sub_depolar","depo_id" );
            if ( sql_query.SELECT() EQ 0 ) {
                ADAK_WARNING( QObject::tr ( "Kayıtlı bir depo bulunamadı. Herhangi bir işlem yapabilmek için önce depoyu kaydetmelisiniz" ), NULL, p_parent );
                return false;
            }
            break;
    }

    return true;
}

/**************************************************************************************
                   SUBE_GET_DEPO_ID_ADI
***************************************************************************************/

int SUBE_GET_DEPO_ID_ADI(QString p_depo_kodu, int *p_depo_id, QString *p_depo_adi )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("sub_depolar",
                         "depo_id, depo_adi",
                         "depo_kodu = :depo_kodu");
    query.SET_VALUE     (":depo_kodu", p_depo_kodu );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    if ( p_depo_id NE NULL ) {
        *p_depo_id = query.VALUE( "depo_id" ).toInt();
    }
    if ( p_depo_adi NE NULL) {
        *p_depo_adi = query.VALUE( "depo_adi" ).toString();
    }
    return 1;
}

/**************************************************************************************
                   SUBE_GET_SUBE_ID_ADI
***************************************************************************************/

int SUBE_GET_SUBE_ID_ADI(QString p_sube_kodu, int *p_sube_id, QString *p_sube_adi )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("sub_subeler",
                         "sube_id, sube_adi",
                         "sube_kodu = :sube_kodu");
    query.SET_VALUE     (":sube_kodu", p_sube_kodu );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    if ( p_sube_id NE NULL ) {
        *p_sube_id = query.VALUE( "sube_id" ).toInt();
    }
    if ( p_sube_adi NE NULL) {
        *p_sube_adi = query.VALUE( "sube_adi" ).toString();
    }
    return 1;
}

/**************************************************************************************
                   SUBE_GET_SUBE_KODU_ADI
***************************************************************************************/

int SUBE_GET_SUBE_KODU_ADI( int p_sube_id, QString* p_sube_kodu, QString *p_sube_adi )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("sub_subeler",
                         "sube_kodu, sube_adi",
                         "sube_id = :sube_id");
    query.SET_VALUE     (":sube_id", p_sube_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    if ( p_sube_kodu NE NULL ) {
        *p_sube_kodu = query.VALUE( "sube_kodu" ).toString();
    }
    if ( p_sube_adi NE NULL) {
        *p_sube_adi = query.VALUE( "sube_adi" ).toString();
    }
    return 1;
}


/**************************************************************************************
                   SUBE_GET_SUBE_ID_LIST
***************************************************************************************/

QList < int > SUBE_GET_SUBE_ID_LIST()
{
    QList <int> sube_id_list;
    SQL_QUERY query( DB );
    query.PREPARE_SELECT("sub_subeler" ,"sube_id");
    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            sube_id_list << query.VALUE( "sube_id" ).toInt();
        }
    }
    return sube_id_list;
}

/**************************************************************************************
                   SUBE_GET_SUBE_KODU_ADI_MAP
***************************************************************************************/

QMap <QString, QString > SUBE_GET_SUBE_KODU_ADI_MAP()
{
    QMap < QString, QString > sube_kodu_adi_map;

    SQL_QUERY query( DB );
    query.PREPARE_SELECT("sub_subeler" ,"sube_kodu, sube_adi");
    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            sube_kodu_adi_map.insert( query.VALUE("sube_kodu").toString(), query.VALUE( "sube_adi" ).toString() );
        }
    }
    return sube_kodu_adi_map;

}

/**************************************************************************************
                   SUBE_UNITE_BILGILERI_OKU
***************************************************************************************/

int SUBE_UNITE_BILGILERI_OKU ( SUBE_UNITE_BILGILERI_STRUCT * P_SUBE_UNITE , int p_unite_id )
{

    SQL_QUERY query( DB );
    query.PREPARE_SELECT ( "sub_uniteler",
                           "unite_adi ,                     "
                           "sube_id   ,                     "
                           "bilgisayar_id,                  "
                           "muh_kasa_hesap_id,              "
                           "cari_hesap_id,                  "
                           "irsaliye_belge_seri,            "
                           "fatura_belge_seri,              "
                           "tahsilat_makbuzu_belge_seri,    "
                           "odeme_makbuzu_belge_seri,       "
                           "depo_id,                        "
                           "tahsilat_makbuzu_belge_num,     "
                           "odeme_makbuzu_belge_num,        "
                           "fatura_belge_numarasi,          "
                           "irsaliye_belge_numarasi,        "
                           "irsaliyeli_fat_belge_numarasi,  "
                           "irsaliyeli_fat_belge_seri       ",
                           "unite_id = :unite_id");
    query.SET_VALUE      ( ":unite_id", p_unite_id );

    if ( query.SELECT() NE 0 ) {
          query.NEXT();
          P_SUBE_UNITE->unite_adi                       = query.VALUE( "unite_adi" ).toString();
          P_SUBE_UNITE->sube_id                         = query.VALUE( "sube_id" ).toInt();
          P_SUBE_UNITE->bilgisayar_id                   = query.VALUE( "bilgisayar_id" ).toInt();
          P_SUBE_UNITE->muh_kasa_hesap_id               = query.VALUE( "muh_kasa_hesap_id" ).toInt();
          P_SUBE_UNITE->cari_hesap_id                   = query.VALUE( "cari_hesap_id" ).toInt();
          P_SUBE_UNITE->irsaliye_belge_seri             = query.VALUE( "irsaliye_belge_seri" ).toString();
          P_SUBE_UNITE->fatura_belge_seri               = query.VALUE( "fatura_belge_seri" ).toString();
          P_SUBE_UNITE->tahsilat_makbuzu_belge_seri     = query.VALUE( "tahsilat_makbuzu_belge_seri" ).toString();
          P_SUBE_UNITE->odeme_makbuzu_belge_seri        = query.VALUE( "odeme_makbuzu_belge_seri" ).toString();
          P_SUBE_UNITE->depo_id                         = query.VALUE( "depo_id" ).toInt();
          P_SUBE_UNITE->tahsilat_makbuzu_belge_num      = query.VALUE( "tahsilat_makbuzu_belge_num" ).toString();
          P_SUBE_UNITE->odeme_makbuzu_belge_num         = query.VALUE( "odeme_makbuzu_belge_num" ).toString();
          P_SUBE_UNITE->fatura_belge_numarasi           = query.VALUE( "fatura_belge_numarasi" ).toString();
          P_SUBE_UNITE->irsaliye_belge_numarasi         = query.VALUE( "irsaliye_belge_numarasi" ).toString();
          P_SUBE_UNITE->irsaliyeli_fat_belge_numarasi   = query.VALUE( "irsaliyeli_fat_belge_numarasi" ).toString();
          P_SUBE_UNITE->irsaliyeli_fat_belge_seri       = query.VALUE( "irsaliyeli_fat_belge_seri" ).toString();

          return 1;
    }
    return 0;
}

/**************************************************************************************
                   SUBE_UPDATE_IRS_BELGE_NO
***************************************************************************************/

void SUBE_UPDATE_IRS_BELGE_NO ( int p_unite_id, QString p_irsaliye_belge_numarasi )
{
    SQL_QUERY query( DB );
    query.PREPARE_UPDATE ( "sub_uniteler","unite_id","irsaliye_belge_numarasi ",
                           "unite_id = :unite_id");

    query.SET_VALUE ( ":irsaliye_belge_numarasi", p_irsaliye_belge_numarasi );
    query.SET_VALUE ( ":unite_id",  p_unite_id );
    query.UPDATE();
}

/**************************************************************************************
                   SUBE_UPDATE_FAT_BELGE_NO
***************************************************************************************/

void SUBE_UPDATE_FAT_BELGE_NO ( int p_unite_id, QString p_fatura_belge_numarasi )
{
    SQL_QUERY query( DB );
    query.PREPARE_UPDATE ( "sub_uniteler","unite_id", "fatura_belge_numarasi",
                           "unite_id = :unite_id");

    query.SET_VALUE ( ":fatura_belge_numarasi", p_fatura_belge_numarasi );
    query.SET_VALUE ( ":unite_id" ,  p_unite_id );
    query.UPDATE();
}

/**************************************************************************************
                   SUBE_UPDATE_IRSALIYELI_FAT_BELGE_NO
***************************************************************************************/

void SUBE_UPDATE_IRSALIYELI_FAT_BELGE_NO ( int p_unite_id, QString p_fatura_belge_numarasi )
{
    SQL_QUERY query( DB );
    query.PREPARE_UPDATE ( "sub_uniteler","unite_id", "irsaliyeli_fat_belge_numarasi",
                          "unite_id = :unite_id");

    query.SET_VALUE ( ":irsaliyeli_fat_belge_numarasi", p_fatura_belge_numarasi );
    query.SET_VALUE ( ":unite_id" ,  p_unite_id );
    query.UPDATE();
}

/**************************************************************************************
                   SUBE_UPDATE_ODM_MKBZ_BELGE_NO
***************************************************************************************/

void SUBE_UPDATE_ODM_MKBZ_BELGE_NO ( int p_unite_id, QString p_odeme_makbuzu_belge_numarasi )
{
    SQL_QUERY query( DB );
    query.PREPARE_UPDATE ( "sub_uniteler", "unite_id", "odeme_makbuzu_belge_num", "unite_id = :unite_id");
    query.SET_VALUE( ":odeme_makbuzu_belge_num",  p_odeme_makbuzu_belge_numarasi );
    query.SET_VALUE( ":unite_id",                 p_unite_id );
    query.UPDATE();
}

/**************************************************************************************
                   SUBE_UPDATE_TAH_MKBZ_BELGE_NO
***************************************************************************************/

void SUBE_UPDATE_TAH_MKBZ_BELGE_NO ( int p_unite_id, QString p_tahsilat_makbuzu_belge_numarasi )
{
    SQL_QUERY query( DB );
    query.PREPARE_UPDATE("sub_uniteler","unite_id","tahsilat_makbuzu_belge_num",
                             "unite_id = :unite_id");

    query.SET_VALUE ( ":tahsilat_makbuzu_belge_num" , p_tahsilat_makbuzu_belge_numarasi );
    query.SET_VALUE ( ":unite_id"                   , p_unite_id );
    query.UPDATE();
}

/**************************************************************************************
                   SUBE_GET_SUBE_ENT_HESAP_ID
***************************************************************************************/

int SUBE_GET_SUBE_ENT_HESAP_ID(int p_sube_id, QString p_column_name, QString *p_muh_hesap_kodu, QString *p_muh_hesap_adi)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("sub_subeler", p_column_name , "sube_id = :sube_id");
    query.SET_VALUE     (":sube_id", p_sube_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    int sube_ent_hesap_id = query.VALUE(0).toInt();

    QString tam_hesap_kodu;
    QString hesap_ismi;

    MUH_GET_HESAP_KODU_HESAP_ISMI(sube_ent_hesap_id ,tam_hesap_kodu,hesap_ismi);

    if ( p_muh_hesap_adi NE NULL ) {
        *p_muh_hesap_adi = hesap_ismi;
    }
    if ( p_muh_hesap_kodu NE NULL ) {
        *p_muh_hesap_kodu = tam_hesap_kodu;
    }

    return sube_ent_hesap_id;
}

/**************************************************************************************
                   SUBE_GET_SUBE_SGK_NO
***************************************************************************************/

QString SUBE_GET_SUBE_SGK_NO(int p_sube_id)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("sub_subeler ", "sgk_isyeri_no",
                         "sube_id = :sube_id");
    query.SET_VALUE     (":sube_id", p_sube_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE( 0 ).toString();
}

/**************************************************************************************
                   SUBE_GET_SUBE_SGK_NO
***************************************************************************************/

QString SUBE_GET_SUBE_ADRES_BILGILERI(int p_sube_id)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("sub_subeler ", "sube_adresi",
                         "sube_id = :sube_id");
    query.SET_VALUE     (":sube_id", p_sube_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE( 0 ).toString();
}

/**************************************************************************************
                   SUBE_GET_DEPO_COUNT
***************************************************************************************/

int SUBE_GET_DEPO_COUNT( int p_sube_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("sub_depolar ", "depo_id",
                         "sube_id = :sube_id");
    query.SET_VALUE     (":sube_id", p_sube_id );

    return query.SELECT();
}
