#include "adak_utils.h"
#include "adak_sql.h"
#include "stok_fis_utils.h"
#include "muh_fis_utils.h"
#include "muh_enum.h"
#include "sube_console_utils.h"
#include "stok_enum.h"
#include "fat_enum.h"
#include "irs_enum.h"
#include "muh_console_utils.h"
#include "fat_console_utils.h"
#include "irs_console_utils.h"
#include "stok_console_utils.h"
#include "e9_console_utils.h"
#include "fat_fis_utils.h"
#include "cari_console_utils.h"
#include "dvz_console_utils.h"
#include "stok_struct.h"

extern      ADAK_SQL   *   DB;

/***************************************************************************************
                    STK_IRS_FIS_BILGILERINI_OKU
***************************************************************************************/

int STK_IRS_FIS_BILGILERINI_OKU(int p_fis_id, STK_IRS_FIS_STRUCT *P_FIS)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_fisler" , "fis_no,fis_tarihi,depo_id,belge_seri ,                           "
                         "belge_numarasi , islem_saati , aciklama ,irsaliye_mi_stok_fisi_mi,              "
                         "fis_turu , irsaliye_alis_satis_turu , brut_tutar ,isk_oncesi_otv_tutari,        "
                         "isk_oncesi_otv_dahil_tutar , isk_oncesi_kdv_tutari , isk_oncesi_kdv_dahil_tutar,"
                         "ara_toplam , isk_sonrasi_otv_tutari ,isk_sonrasi_otv_dahil_tutar,               "
                         "isk_sonrasi_kdv_tutari ,irsaliye_tutari , iskonto_yuzdesi,                      "
                         "iskonto_tutari , doviz_tutar_bilgileri ,kdv_oran_bilgileri ,                    "
                         "otv_oran_bilgileri ,alan_depo_id , sevk_tarihi ,cari_hesap_id,                  "
                         "cari_hesap_ismi ,vergi_dairesi ,vergi_numarasi ,teslim_adresi,                  "
                         "yazdirildi_mi ,iptal_mi ,irsaliye_faturalastirildi_mi,                          "
                         "base_fis_id ,program_id ,modul_id,             "
                         "iade_irsaliyesi_mi ,iade_edilen_irs_id , kdv_muaf_mi,muh_fis_id ,           "
                         "muh_hesap_id , stk_irs_para_birim_id, otv_muaf_mi, parite, doviz_kuru",
                         "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);


    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    P_FIS->fis_no                       = query.VALUE("fis_no").toInt();
    P_FIS->fis_tarihi                   = query.VALUE("fis_tarihi").toString();
    P_FIS->depo_id                      = query.VALUE("depo_id").toInt();
    P_FIS->belge_seri                   = query.VALUE("belge_seri").toString();
    P_FIS->belge_numarasi               = query.VALUE("belge_numarasi").toString();
    P_FIS->islem_saati                  = query.VALUE("islem_saati").toString();
    P_FIS->aciklama                     = query.VALUE("aciklama").toString();
    P_FIS->irsaliye_mi_stok_fisi_mi     = query.VALUE("irsaliye_mi_stok_fisi_mi").toInt();
    P_FIS->fis_turu                     = query.VALUE("fis_turu").toInt();
    P_FIS->irsaliye_alis_satis_turu     = query.VALUE("irsaliye_alis_satis_turu").toInt();
    P_FIS->brut_tutar                   = query.VALUE("brut_tutar").toDouble();
    P_FIS->isk_oncesi_otv_tutari        = query.VALUE("isk_oncesi_otv_tutari").toDouble();
    P_FIS->isk_oncesi_otv_dahil_tutar   = query.VALUE("isk_oncesi_otv_dahil_tutar").toDouble();
    P_FIS->isk_oncesi_kdv_tutari        = query.VALUE("isk_oncesi_kdv_tutari").toDouble();
    P_FIS->isk_oncesi_kdv_dahil_tutar   = query.VALUE("isk_oncesi_kdv_dahil_tutar").toDouble();
    P_FIS->ara_toplam                   = query.VALUE("ara_toplam").toDouble();
    P_FIS->isk_sonrasi_otv_tutari       = query.VALUE("isk_sonrasi_otv_tutari").toDouble();
    P_FIS->isk_sonrasi_otv_dahil_tutar  = query.VALUE("isk_sonrasi_otv_dahil_tutar").toDouble();
    P_FIS->isk_sonrasi_kdv_tutari       = query.VALUE("isk_sonrasi_kdv_tutari").toDouble();
    P_FIS->irsaliye_tutari              = query.VALUE("irsaliye_tutari").toDouble();
    P_FIS->iskonto_yuzdesi              = query.VALUE("iskonto_yuzdesi").toDouble();
    P_FIS->iskonto_tutari               = query.VALUE("iskonto_tutari").toDouble();
    P_FIS->doviz_tutar_bilgileri        = query.VALUE("doviz_tutar_bilgileri").toString();
    P_FIS->kdv_oran_bilgileri           = query.VALUE("kdv_oran_bilgileri").toString();
    P_FIS->otv_oran_bilgileri           = query.VALUE("otv_oran_bilgileri").toString();
    P_FIS->alan_depo_id                 = query.VALUE("alan_depo_id").toInt();
    P_FIS->sevk_tarihi                  = query.VALUE("sevk_tarihi").toString();
    P_FIS->cari_hesap_id                = query.VALUE("cari_hesap_id").toInt();
    P_FIS->cari_hesap_ismi              = query.VALUE("cari_hesap_ismi").toString();
    P_FIS->vergi_dairesi                = query.VALUE("vergi_dairesi").toString();
    P_FIS->vergi_numarasi               = query.VALUE("vergi_numarasi").toString();
    P_FIS->teslim_adresi                = query.VALUE("teslim_adresi").toString();
    P_FIS->yazdirildi_mi                = query.VALUE("yazdirildi_mi").toInt();
    P_FIS->iptal_mi                     = query.VALUE("iptal_mi").toInt();
    P_FIS->irsaliye_faturalastirildi_mi = query.VALUE("irsaliye_faturalastirildi_mi").toInt();
    P_FIS->base_fis_id                  = query.VALUE("base_fis_id").toInt();
    P_FIS->program_id                   = query.VALUE("program_id").toInt();
    P_FIS->modul_id                     = query.VALUE("modul_id").toInt();
    P_FIS->iade_irsaliyesi_mi           = query.VALUE("iade_irsaliyesi_mi").toInt();
    P_FIS->iade_edilen_irs_id           = query.VALUE("iade_edilen_irs_id").toInt();
    P_FIS->kdv_muaf_mi                  = query.VALUE("kdv_muaf_mi").toInt();
    P_FIS->otv_muaf_mi                  = query.VALUE("otv_muaf_mi").toInt();
    P_FIS->muh_fis_id                   = query.VALUE("muh_fis_id").toInt();
    P_FIS->muh_hesap_id                 = query.VALUE("muh_hesap_id").toInt();
    P_FIS->stk_irs_para_birimi_id       = query.VALUE("stk_irs_para_birim_id").toInt();
    P_FIS->parite                       = query.VALUE("parite").toDouble();
    P_FIS->doviz_kuru                   = query.VALUE("doviz_kuru").toDouble();
    P_FIS->cari_para_birim_id           = CARI_GET_PARA_BIRIM_ID( P_FIS->cari_hesap_id );
    P_FIS->cari_para_birim_kodu         = DVZ_GET_DOVIZ_KODU(P_FIS->cari_para_birim_id );

    return p_fis_id;

}

/***************************************************************************************
                       STK_IRS_SATIR_BILGILERINI_OKU
***************************************************************************************/

int STK_IRS_SATIR_BILGILERINI_OKU(int p_fis_satiri_id, STK_IRS_FIS_SATIR_STRUCT *P_FIS_SATIRI )
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_fis_satirlari", "fis_id , order_number ,urun_id,urun_adi ,"
                         "satirdaki_urun_miktari ,urun_birimi , temel_birim_katsayisi  ,"
                         "isk_oncesi_urun_birim_fiyati , isk_sonrasi_urun_birim_fiyati ,"
                         "satir_iskonto_yuzdesi , satir_iskonto_tutari , satir_tutari  ,"
                         "doviz_id , parti_id , kdv_orani , otv_orani , toplu_iskonto_yuzdesi,"
                         "t_is_son_birim_fiyati , t_is_son_satir_tutari , t_is_son_str_vergi_haric_tutar,"
                         "t_is_son_satir_otv_tutari , t_is_son_satir_otv_dahil_tutar ,"
                         "t_is_son_satir_kdv_tutari , t_is_son_satir_kdv_dahil_tutar , "
                         "satir_vergi_haric_tutar , satir_otv_tutari , satir_otv_dahil_tutar ,"
                         "satir_kdv_tutari ,satir_kdv_dahil_tutar , kdv_dahil_mi , faturalanan_miktar ,"
                         "iade_edilen_miktar , hatali_satir_mi , modul_id , program_id , "
                         "base_record_id"
                         , "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    P_FIS_SATIRI->fis_id                        = query.VALUE("fis_id").toInt();
    P_FIS_SATIRI->order_number                  = query.VALUE("order_number").toInt();
    P_FIS_SATIRI->urun_id                       = query.VALUE("urun_id").toInt();
    P_FIS_SATIRI->urun_adi                      = query.VALUE("urun_adi").toString();
    P_FIS_SATIRI->satirdaki_urun_miktari        = query.VALUE("satirdaki_urun_miktari").toDouble();
    P_FIS_SATIRI->urun_birimi                   = query.VALUE("urun_birimi").toString();
    P_FIS_SATIRI->temel_birim_katsayisi         = query.VALUE("temel_birim_katsayisi").toDouble();
    P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati  = query.VALUE("isk_oncesi_urun_birim_fiyati").toDouble();
    P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati = query.VALUE("isk_sonrasi_urun_birim_fiyati").toDouble();
    P_FIS_SATIRI->iskonto_yuzdesi               = query.VALUE("satir_iskonto_yuzdesi").toDouble();
    P_FIS_SATIRI->iskonto_tutari                = query.VALUE("satir_iskonto_tutari").toDouble();
    P_FIS_SATIRI->satir_tutari                  = query.VALUE("satir_tutari").toDouble();
    P_FIS_SATIRI->doviz_id                      = query.VALUE("doviz_id").toInt();
    P_FIS_SATIRI->parti_id                      = query.VALUE("parti_id").toInt();
    P_FIS_SATIRI->kdv_orani                     = query.VALUE("kdv_orani").toDouble();
    P_FIS_SATIRI->otv_orani                     = query.VALUE("otv_orani").toDouble();
    P_FIS_SATIRI->toplu_iskonto_yuzdesi         = query.VALUE("toplu_iskonto_yuzdesi").toDouble();
    P_FIS_SATIRI->t_is_sonrasi_birim_fiyat      = query.VALUE("t_is_son_birim_fiyati").toDouble();
    P_FIS_SATIRI->t_is_sonrasi_satir_tutari     = query.VALUE("t_is_son_satir_tutari").toDouble();
    P_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar= query.VALUE("t_is_son_str_vergi_haric_tutar").toDouble();
    P_FIS_SATIRI->t_is_sonrasi_otv_tutari       = query.VALUE("t_is_son_satir_otv_tutari").toDouble();
    P_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar  = query.VALUE("t_is_son_satir_otv_dahil_tutar").toDouble();
    P_FIS_SATIRI->t_is_sonrasi_kdv_tutari       = query.VALUE("t_is_son_satir_kdv_tutari").toDouble();
    P_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar  = query.VALUE("t_is_son_satir_kdv_dahil_tutar").toDouble();
    P_FIS_SATIRI->satir_vergi_haric_tutar       = query.VALUE("satir_vergi_haric_tutar").toDouble();
    P_FIS_SATIRI->satir_otv_tutari              = query.VALUE("satir_otv_tutari").toDouble();
    P_FIS_SATIRI->satir_otv_dahil_tutar         = query.VALUE("satir_otv_dahil_tutar").toDouble();
    P_FIS_SATIRI->satir_kdv_tutari              = query.VALUE("satir_kdv_tutari").toDouble();
    P_FIS_SATIRI->satir_kdv_dahil_tutar         = query.VALUE("satir_kdv_dahil_tutar").toDouble();
    P_FIS_SATIRI->kdv_dahil_mi                  = query.VALUE("kdv_dahil_mi").toInt();
    P_FIS_SATIRI->faturalanan_miktar            = query.VALUE("faturalanan_miktar").toDouble();
    P_FIS_SATIRI->iade_edilen_miktar            = query.VALUE("iade_edilen_miktar").toDouble();
    P_FIS_SATIRI->hatali_satir_mi               = query.VALUE("hatali_satir_mi").toInt();
    P_FIS_SATIRI->modul_id                      = query.VALUE("modul_id").toInt();
    P_FIS_SATIRI->program_id                    = query.VALUE("program_id").toInt();
    P_FIS_SATIRI->base_record_id                = query.VALUE("base_record_id").toInt();

    return p_fis_satiri_id;

}

/***************************************************************************************
                  STK_IRS_FIS_UPDATE
***************************************************************************************/

int STK_IRS_FIS_EKLE( STK_IRS_FIS_STRUCT *P_FIS )
{
    SQL_QUERY query( DB );

    //! KONTROLLER

    //! END KONTROL
    if ( P_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_STOK_FISI ) {

        if ( P_FIS->fis_turu EQ ENUM_STK_SARF_FIRE_FISI ) {

            STK_IRS_FIS_STRUCT * STK_FIS    = new STK_IRS_FIS_STRUCT;

            STK_FIS->fis_no                 = P_FIS->fis_no;
            STK_FIS->aciklama               = P_FIS->aciklama;
            STK_FIS->base_fis_id            = 0;
            STK_FIS->fis_tarihi             = P_FIS->fis_tarihi;
            STK_FIS->fis_turu               = P_FIS->fis_turu;
            STK_FIS->program_id             = E9_PROGRAMI;
            STK_FIS->modul_id               = STOK_MODULU;

            P_FIS->muh_fis_id  = STK_ENT_FIS_EKLE ( STK_FIS );
            delete STK_FIS;
        }
    }

    //! INSERT ISLEMI
    if ( P_FIS->modul_id EQ STOK_MODULU OR P_FIS->modul_id EQ IRSALIYE_MODULU ){
        P_FIS->base_fis_id = 0;
    }

    query.PREPARE_INSERT ( "stk_fisler", "fis_id",
                           "fis_no, fis_tarihi,                             "
                           "aciklama, doviz_tutar_bilgileri,                "
                           "irsaliye_mi_stok_fisi_mi, fis_turu,             "
                           "depo_id, alan_depo_id,                          "
                           "sevk_tarihi, belge_seri, belge_numarasi,        "
                           "cari_hesap_id, cari_hesap_ismi,                 "
                           "vergi_dairesi, vergi_numarasi, teslim_adresi,   "
                           "kdv_oran_bilgileri, otv_oran_bilgileri,         "
                           "irsaliye_faturalastirildi_mi,                   "
                           "hatali_irsaliye_mi, iptal_mi,                   "
                           "modul_id, program_id,      "
                           "iade_edilen_irs_id, iade_irsaliyesi_mi,         "
                           "irsaliye_alis_satis_turu, kdv_muaf_mi ,         "
                           "muh_fis_id, base_fis_id, muh_hesap_id ,         "
                           "otv_muaf_mi, stk_irs_para_birim_id, parite, doviz_kuru");

    query.SET_VALUE ( ":fis_no"                         , P_FIS->fis_no                     );
    query.SET_VALUE ( ":fis_tarihi"                     , P_FIS->fis_tarihi                 );
    query.SET_VALUE ( ":aciklama"                       , P_FIS->aciklama                   );
    query.SET_VALUE ( ":doviz_tutar_bilgileri"          , P_FIS->doviz_tutar_bilgileri      );
    query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi"       , P_FIS->irsaliye_mi_stok_fisi_mi   );
    query.SET_VALUE ( ":fis_turu"                       , P_FIS->fis_turu                   );
    query.SET_VALUE ( ":depo_id"                        , P_FIS->depo_id                    );
    query.SET_VALUE ( ":alan_depo_id"                   , P_FIS->alan_depo_id               );
    query.SET_VALUE ( ":sevk_tarihi"                    , P_FIS->sevk_tarihi                );
    query.SET_VALUE ( ":belge_seri"                     , P_FIS->belge_seri                 );
    query.SET_VALUE ( ":belge_numarasi"                 , P_FIS->belge_numarasi             );
    query.SET_VALUE ( ":cari_hesap_id"                  , P_FIS->cari_hesap_id              );
    query.SET_VALUE ( ":cari_hesap_ismi"                , P_FIS->cari_hesap_ismi            );
    query.SET_VALUE ( ":vergi_dairesi"                  , P_FIS->vergi_dairesi              );
    query.SET_VALUE ( ":vergi_numarasi"                 , P_FIS->vergi_numarasi             );
    query.SET_VALUE ( ":teslim_adresi"                  , P_FIS->teslim_adresi              );
    query.SET_VALUE ( ":kdv_oran_bilgileri"             , P_FIS->kdv_oran_bilgileri         );
    query.SET_VALUE ( ":otv_oran_bilgileri"             , P_FIS->otv_oran_bilgileri         );
    query.SET_VALUE ( ":irsaliye_faturalastirildi_mi"   , P_FIS->irsaliye_faturalastirildi_mi);
    query.SET_VALUE ( ":hatali_irsaliye_mi"             , 0                                 );
    query.SET_VALUE ( ":iptal_mi"                       , 0                                 );
    query.SET_VALUE ( ":modul_id"                       , P_FIS->modul_id                   );
    query.SET_VALUE ( ":program_id"                     , P_FIS->program_id                 );
    query.SET_VALUE ( ":iade_edilen_irs_id"             , P_FIS->iade_edilen_irs_id         );
    query.SET_VALUE ( ":iade_irsaliyesi_mi"             , P_FIS->iade_irsaliyesi_mi         );
    query.SET_VALUE ( ":irsaliye_alis_satis_turu"       , P_FIS->irsaliye_alis_satis_turu   );
    query.SET_VALUE ( ":kdv_muaf_mi"                    , P_FIS->kdv_muaf_mi                );
    query.SET_VALUE ( ":muh_fis_id"                     , P_FIS->muh_fis_id                 );
    query.SET_VALUE ( ":base_fis_id"                    , P_FIS->base_fis_id                );
    query.SET_VALUE ( ":muh_hesap_id"                   , P_FIS->muh_hesap_id               );
    query.SET_VALUE ( ":otv_muaf_mi"                    , P_FIS->otv_muaf_mi                );
    query.SET_VALUE ( ":parite"                         , P_FIS->parite                     );
    query.SET_VALUE ( ":doviz_kuru"                     , P_FIS->doviz_kuru                 );
    query.SET_VALUE ( ":stk_irs_para_birim_id"          , P_FIS->stk_irs_para_birimi_id     );

    int fis_id = query.INSERT();

    //! KONTROLLER
    // MUHASEBEDE OLUSAN FISIN BASE FIS ID GUNCELLE
    if ( P_FIS->fis_turu EQ ENUM_STK_SARF_FIRE_FISI ) {
        MUH_UPDATE_FIS_BASE_FIS_ID( P_FIS->muh_fis_id, fis_id );
    }

    return fis_id;
}

/***************************************************************************************
                  STK_IRS_FIS_SATIRI_EKLE
***************************************************************************************/

int STK_IRS_FIS_SATIRI_EKLE ( STK_IRS_FIS_STRUCT * P_FIS, STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI )
{
    SQL_QUERY query( DB );

    //! LINE INSERT ISLEMI

    int new_order_number = 1024;
    query.PREPARE_SELECT ( "stk_fis_satirlari ","order_number", "fis_id = :fis_id");
    query.SET_VALUE      ( ":fis_id", P_FIS_SATIRI->fis_id );

    if ( query.SELECT("order_number DESC") > 0 ) {
        query.NEXT();
        new_order_number  = query.VALUE ( 0 ).toInt();
        new_order_number += 1024;
    }

    if ( P_FIS_SATIRI->modul_id EQ STOK_MODULU OR P_FIS_SATIRI->modul_id EQ IRSALIYE_MODULU ) {
        P_FIS_SATIRI->base_record_id = 0;
    }

    P_FIS_SATIRI->order_number = new_order_number;

    query.PREPARE_INSERT ( "stk_fis_satirlari","fis_satiri_id",
                           "fis_id, order_number, urun_id, "
                           "urun_adi, satirdaki_urun_miktari, urun_birimi, temel_birim_katsayisi, "
                           "isk_oncesi_urun_birim_fiyati, isk_sonrasi_urun_birim_fiyati, "
                           "satir_iskonto_yuzdesi, satir_iskonto_tutari, satir_tutari, doviz_id, "
                           "satir_vergi_haric_tutar,  satir_otv_tutari, satir_otv_dahil_tutar, "
                           "satir_kdv_tutari, satir_kdv_dahil_tutar, kdv_orani, kdv_dahil_mi, otv_orani, "
                           "parti_id, faturalanan_miktar, iade_edilen_miktar, hatali_satir_mi, toplu_iskonto_yuzdesi, "
                           "t_is_son_birim_fiyati, t_is_son_satir_tutari, t_is_son_str_vergi_haric_tutar, "
                           "t_is_son_satir_otv_tutari, t_is_son_satir_otv_dahil_tutar, t_is_son_satir_kdv_tutari, "
                           "t_is_son_satir_kdv_dahil_tutar, modul_id, program_id, base_record_id ");


    query.SET_VALUE ( ":fis_id"                           , P_FIS_SATIRI->fis_id                            );
    query.SET_VALUE ( ":order_number"                     , P_FIS_SATIRI->order_number                      );
    query.SET_VALUE ( ":urun_id"                          , P_FIS_SATIRI->urun_id                           );
    query.SET_VALUE ( ":urun_adi"                         , P_FIS_SATIRI->urun_adi                          );
    query.SET_VALUE ( ":satirdaki_urun_miktari"           , P_FIS_SATIRI->satirdaki_urun_miktari            );
    query.SET_VALUE ( ":urun_birimi"                      , P_FIS_SATIRI->urun_birimi                       );
    query.SET_VALUE ( ":temel_birim_katsayisi"            , P_FIS_SATIRI->temel_birim_katsayisi             );
    query.SET_VALUE ( ":isk_oncesi_urun_birim_fiyati"     , P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati      );
    query.SET_VALUE ( ":isk_sonrasi_urun_birim_fiyati"    , P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati     );
    query.SET_VALUE ( ":satir_iskonto_yuzdesi"            , P_FIS_SATIRI->iskonto_yuzdesi                   );
    query.SET_VALUE ( ":satir_iskonto_tutari"             , P_FIS_SATIRI->iskonto_tutari                    );
    query.SET_VALUE ( ":satir_tutari"                     , P_FIS_SATIRI->satir_tutari                      );
    query.SET_VALUE ( ":doviz_id"                         , P_FIS_SATIRI->doviz_id                          );
    query.SET_VALUE ( ":satir_vergi_haric_tutar"          , P_FIS_SATIRI->satir_vergi_haric_tutar           );
    query.SET_VALUE ( ":satir_otv_tutari"                 , P_FIS_SATIRI->satir_otv_tutari                  );
    query.SET_VALUE ( ":satir_otv_dahil_tutar"            , P_FIS_SATIRI->satir_otv_dahil_tutar             );
    query.SET_VALUE ( ":satir_kdv_tutari"                 , P_FIS_SATIRI->satir_kdv_tutari                  );
    query.SET_VALUE ( ":satir_kdv_dahil_tutar"            , P_FIS_SATIRI->satir_kdv_dahil_tutar             );
    query.SET_VALUE ( ":kdv_orani"                        , P_FIS_SATIRI->kdv_orani                         );
    query.SET_VALUE ( ":kdv_dahil_mi"                     , P_FIS_SATIRI->kdv_dahil_mi                      );
    query.SET_VALUE ( ":otv_orani"                        , P_FIS_SATIRI->otv_orani                         );
    query.SET_VALUE ( ":parti_id"                         , P_FIS_SATIRI->parti_id                          );
    query.SET_VALUE ( ":hatali_satir_mi"                  , P_FIS_SATIRI->hatali_satir_mi                   );
    query.SET_VALUE ( ":toplu_iskonto_yuzdesi"            , P_FIS_SATIRI->toplu_iskonto_yuzdesi             );
    query.SET_VALUE ( ":t_is_son_birim_fiyati"            , P_FIS_SATIRI->t_is_sonrasi_birim_fiyat          );
    query.SET_VALUE ( ":t_is_son_satir_tutari"            , P_FIS_SATIRI->t_is_sonrasi_satir_tutari         );
    query.SET_VALUE ( ":t_is_son_str_vergi_haric_tutar"   , P_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar    );
    query.SET_VALUE ( ":t_is_son_satir_otv_tutari"        , P_FIS_SATIRI->t_is_sonrasi_otv_tutari           );
    query.SET_VALUE ( ":t_is_son_satir_otv_dahil_tutar"   , P_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar      );
    query.SET_VALUE ( ":t_is_son_satir_kdv_tutari"        , P_FIS_SATIRI->t_is_sonrasi_kdv_tutari           );
    query.SET_VALUE ( ":t_is_son_satir_kdv_dahil_tutar"   , P_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar      );
    query.SET_VALUE ( ":faturalanan_miktar"               , P_FIS_SATIRI->faturalanan_miktar                );
    query.SET_VALUE ( ":iade_edilen_miktar"               , 0                                               );
    query.SET_VALUE ( ":modul_id"                         , P_FIS_SATIRI->modul_id                          );
    query.SET_VALUE ( ":program_id"                       , P_FIS_SATIRI->program_id                        );
    query.SET_VALUE ( ":base_record_id"                   , P_FIS_SATIRI->base_record_id                    );

    int irsaliye_satiri_id  = query.INSERT();

    if ( P_FIS->fis_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS OR P_FIS->fis_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS ) {

        int fatura_id = FAT_GET_FATURA_ID( P_FIS_SATIRI->fatura_satiri_id );

        FAT_UPDATE_FATURA_IRSALIYELESTIRILDI_MI( fatura_id, 1);

        query.PREPARE_SELECT( "fat_irs_baglantisi", "fatura_id", "irsaliye_id = :irsaliye_id");
        query.SET_VALUE     ( ":irsaliye_id", P_FIS_SATIRI->fis_id );

        if ( query.SELECT() EQ 0 ) {
            FAT_IRS_BAGLANTISI_EKLE( fatura_id, P_FIS_SATIRI->fis_id );
        }
    }

    return irsaliye_satiri_id;
}

/***************************************************************************************
                  STK_IRS_FIS_HAREKERLERI_OLUSTUR
***************************************************************************************/
int STK_IRS_FIS_SATIR_HAREKETLERI_OLUSTUR ( STK_IRS_FIS_STRUCT * P_FIS, STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI, int p_irsaliye_satiri_id )
{
    //! BURADA FIS SATIRININ OLASI BUTUN DURUMLAR ICIN HAREKET ( ENTEGRASYON ) OLUSTURULACAK
    //! ISLEM BASARILI BIR SEKILDE SONUCLANIRSA ADAK_OK, BASARISIZ OLURSA ADAK_FAIL
    //! DONDRULULECEK

    SQL_QUERY f_query( DB );

    int    stok_takip_sekli     = STK_GET_URUN_TAKIP_SEKLI( P_FIS_SATIRI->urun_id );
    double temel_birim_miktari  = P_FIS_SATIRI->temel_birim_katsayisi * P_FIS_SATIRI->satirdaki_urun_miktari;;

    switch ( P_FIS->fis_turu ) {

        case    ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  :
        case    ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
        case    ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS   :
        case    ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS  :

            f_query.PREPARE_INSERT ("fat_irs_satir_baglantisi", "kayit_id", "fatura_satiri_id, irsaliye_satiri_id" );

            f_query.SET_VALUE      (":fatura_satiri_id"   , P_FIS_SATIRI->fatura_satiri_id   );
            f_query.SET_VALUE      (":irsaliye_satiri_id" , p_irsaliye_satiri_id );
            f_query.INSERT();

            FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE  ( P_FIS_SATIRI->fatura_satiri_id, P_FIS_SATIRI->satirdaki_urun_miktari, ENUM_FAT_IRSALIYELENEN_MIKTAR_ARTIR );
            break;

        default :
            break;
    }

    if ( P_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {

        f_query.PREPARE_INSERT ( "irs_iade_satir_baglantisi","kayit_id", "irs_iade_edilen_satir_id, irs_satir_id ");

        f_query.SET_VALUE      ( ":irs_iade_edilen_satir_id", P_FIS_SATIRI->iade_edilen_irsaliye_satir_id   );
        f_query.SET_VALUE      ( ":irs_satir_id",             p_irsaliye_satiri_id                          );
        f_query.INSERT();
        // SATIRDAKI MIKTAR KADAR IADE EDILIYOR
        IRS_IRSALIYE_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( P_FIS_SATIRI->iade_edilen_irsaliye_satir_id,
                                                         P_FIS_SATIRI->satirdaki_urun_miktari,
                                                         ENUM_IRS_IADE_MIKTARI_ARTIR );
    }


    P_FIS->brut_tutar                     = ROUND ( P_FIS->brut_tutar                  + P_FIS_SATIRI->satir_vergi_haric_tutar    );
    P_FIS->isk_oncesi_otv_tutari          = ROUND ( P_FIS->isk_oncesi_otv_tutari       + P_FIS_SATIRI->satir_otv_tutari           );
    P_FIS->isk_oncesi_otv_dahil_tutar     = ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar  + P_FIS_SATIRI->satir_otv_dahil_tutar      );
    P_FIS->isk_oncesi_kdv_tutari          = ROUND ( P_FIS->isk_oncesi_kdv_tutari       + P_FIS_SATIRI->satir_kdv_tutari           );
    P_FIS->isk_oncesi_kdv_dahil_tutar     = ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar  + P_FIS_SATIRI->satir_kdv_dahil_tutar      );

    P_FIS->ara_toplam                     = ROUND ( P_FIS->brut_tutar                  * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->isk_sonrasi_otv_tutari         = ROUND ( P_FIS->isk_oncesi_otv_tutari       * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->isk_sonrasi_otv_dahil_tutar    = ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar  * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->isk_sonrasi_kdv_tutari         = ROUND ( P_FIS->isk_oncesi_kdv_tutari       * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->irsaliye_tutari                = ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar  * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS-> iskonto_tutari                = ROUND ( P_FIS->brut_tutar - P_FIS->ara_toplam  );

    //! FIS UPDATE EDILDI
    STK_IRS_FIS_UPDATE( P_FIS_SATIRI->fis_id, P_FIS );

    int gun_no = MALI_YIL_ARRAY_INDIS ( QDate::fromString( P_FIS->fis_tarihi, "yyyy.MM.dd") );

    if ( P_FIS->fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
        gun_no = 0;
    }

    int depo_id = 0;

    switch ( P_FIS->fis_turu ) {
       case ENUM_STK_DEPO_TRANSFER_CIKIS :
       default                      :
           depo_id = P_FIS->depo_id;
           break;
       case ENUM_STK_DEPO_TRANSFER_GIRIS :
           depo_id = P_FIS->alan_depo_id;
           break;
    }

    switch ( P_FIS->irsaliye_alis_satis_turu) {

        case    ENUM_ALIS_IRSALIYESI_FATURASI   :
        default                                 : {

            switch ( P_FIS->fis_turu ) {
                case ENUM_STK_DEPO_TRANSFER_CIKIS : // Depo transferlerde urun kartinda gosterilecek olan
                case ENUM_STK_DEPO_TRANSFER_GIRIS : // urun miktari degismez bir depodan digerine transfer
                    break;
                default :
                    // URUN KARTI
                    STK_URUN_KARTINDAKI_STOK_MIKTARINI_ARTIR ( P_FIS_SATIRI->urun_id , gun_no, temel_birim_miktari, 0.00 );
                    break;
            }
            // DEPO MIKTARI
            STK_DEPODAKI_STOK_MIKTARINI_ARTIR  ( depo_id, P_FIS_SATIRI->urun_id, gun_no, temel_birim_miktari, 0.00 );

            switch ( stok_takip_sekli ) {

                case ENUM_SERI_NO_ILE_TAKIP :
                    if ( P_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                        STK_SERI_NO_LIST_SATISTAN_IADE( P_FIS->depo_id, P_FIS_SATIRI->urun_id, p_irsaliye_satiri_id, P_FIS_SATIRI->seri_no_list );
                    }
                    else { // kayit islemi
                        STK_SERI_NO_LIST_ALIS( P_FIS->depo_id, P_FIS_SATIRI->urun_id, p_irsaliye_satiri_id, P_FIS_SATIRI->seri_no_list );
                    }
                    break;
                case ENUM_PARTI_TAKIBI :
                    P_FIS_SATIRI->parti_id= STK_PARTIDEKI_URUN_MIKTARINI_ARTIR ( P_FIS->depo_id, P_FIS_SATIRI->urun_id, gun_no, P_FIS_SATIRI->parti_kodu, temel_birim_miktari, 0.00 );
                    break;

                case ENUM_BASIT_TAKIP :
                default               :
                    break;

            }
            break;
        } //! END ENUM_ALIS_IRSALIYESI_FATURASI
        case ENUM_SATIS_IRSALIYESI_FATURASI : {

            switch ( P_FIS->fis_turu ) {
                case ENUM_STK_DEPO_TRANSFER_CIKIS : // Depo transferlerde urun kartinda gosterilecek olan
                case ENUM_STK_DEPO_TRANSFER_GIRIS : // urun miktari degismez bir depodan digerine transfer
                    break;
                default :
                    // URUN KARTI
                    STK_URUN_KARTINDAKI_STOK_MIKTARINI_ARTIR ( P_FIS_SATIRI->urun_id, gun_no, 0.00, temel_birim_miktari );
                    break;
            }
            // DEPO MIKTARI
            STK_DEPODAKI_STOK_MIKTARINI_ARTIR ( depo_id, P_FIS_SATIRI->urun_id, gun_no, 0.00, temel_birim_miktari );

            switch ( stok_takip_sekli ) {
                case ENUM_SERI_NO_ILE_TAKIP :
                    // IADE ISLEMI ICIN YAPILAN ISLEM AYNI
                    STK_SERI_NO_LIST_SATIS( P_FIS->depo_id, P_FIS_SATIRI->urun_id, p_irsaliye_satiri_id, P_FIS_SATIRI->seri_no_list );
                    break;
                case ENUM_PARTI_TAKIBI :
                    STK_PARTIDEKI_URUN_MIKTARINI_ARTIR ( depo_id, P_FIS_SATIRI->urun_id , gun_no, P_FIS_SATIRI->parti_kodu, 0.00, temel_birim_miktari );
                    break;
                case ENUM_BASIT_TAKIP       :
                default                     :
                    break;
            }
            break;

        //!  END ENUM_SATIS_IRSALIYESI_FATURASI
        }
    }

    f_query.PREPARE_UPDATE("stk_fis_satirlari ", "fis_satiri_id", "parti_id",
                           "fis_satiri_id = :fis_satiri_id");

    f_query.SET_VALUE(":parti_id"         , P_FIS_SATIRI->parti_id );
    f_query.SET_VALUE(":fis_satiri_id"    , p_irsaliye_satiri_id   );

    f_query.UPDATE();

    if ( P_FIS->fis_turu EQ ENUM_STK_SARF_FIRE_FISI ) {

        STK_IRS_FIS_SATIR_STRUCT * STK_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

        STK_FIS_SATIRI->base_record_id      = p_irsaliye_satiri_id;
        STK_FIS_SATIRI->muh_fis_id          = P_FIS->muh_fis_id;
        STK_FIS_SATIRI->irsaliye_tutari     = P_FIS->irsaliye_tutari;
        STK_FIS_SATIRI->modul_id            = STOK_MODULU;
        STK_FIS_SATIRI->program_id          = E9_PROGRAMI;
        STK_FIS_SATIRI->urun_id             = P_FIS_SATIRI->urun_id;
        STK_FIS_SATIRI->muh_hesap_id        = P_FIS->muh_hesap_id;

        STK_ENT_FIS_SATIRI_EKLE( P_FIS, STK_FIS_SATIRI );

        delete STK_FIS_SATIRI;
    }

    return ADAK_OK;
}

/***************************************************************************************
                  STK_IRS_FIS_SATIRI_GUNCELLE
***************************************************************************************/

int STK_IRS_FIS_SATIRI_GUNCELLE( STK_IRS_FIS_STRUCT *P_FIS, STK_IRS_FIS_SATIR_STRUCT *P_NEW_FIS_SATIRI, int p_irsaliye_satiri_id )
{
    SQL_QUERY f_query( DB );

    STK_IRS_FIS_SATIR_STRUCT * OLD_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

    STK_IRS_CLEAR_SATIR_STRUCT   ( OLD_FIS_SATIRI );
    STK_IRS_SATIR_BILGILERINI_OKU( p_irsaliye_satiri_id, OLD_FIS_SATIRI );

    int old_stok_takip_sekli = STK_GET_URUN_TAKIP_SEKLI( OLD_FIS_SATIRI->urun_id );
    int new_stok_takip_sekli = STK_GET_URUN_TAKIP_SEKLI( P_NEW_FIS_SATIRI->urun_id );

    if ( P_NEW_FIS_SATIRI->urun_id > 0 ) {
        if ( P_NEW_FIS_SATIRI->satirdaki_urun_miktari EQ 0 ) {
            P_NEW_FIS_SATIRI->hatali_satir_mi = 1;
        }
    }

    switch ( P_FIS->fis_turu ) {

        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
        {
            f_query.PREPARE_SELECT ( "fat_irs_satir_baglantisi",
                                     "fatura_satiri_id ",
                                     "irsaliye_satiri_id = :irsaliye_satiri_id");

            f_query.SET_VALUE      ( ":irsaliye_satiri_id", p_irsaliye_satiri_id );

            if ( f_query.SELECT() EQ 0 ) {
                break;
            }

            f_query.NEXT();

            OLD_FIS_SATIRI->fatura_satiri_id = f_query.VALUE(0).toInt();

            if ( OLD_FIS_SATIRI->fatura_satiri_id NE P_NEW_FIS_SATIRI->fatura_satiri_id ) {
                f_query.PREPARE_DELETE("fat_irs_satir_baglantisi ",
                                       "irsaliye_satiri_id = :irsaliye_satiri_id");
                f_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );
                f_query.DELETE();
            }
            FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE ( OLD_FIS_SATIRI->fatura_satiri_id, OLD_FIS_SATIRI->satirdaki_urun_miktari, ENUM_FAT_IRSALIYELENEN_MIKTAR_AZALT );
            break;
        }

        default :
            break;

    }

    if ( P_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {

        f_query.PREPARE_SELECT ( "irs_iade_satir_baglantisi",
                                 "irs_iade_edilen_satir_id ",
                                 "irs_satir_id = :irs_satir_id");
        f_query.SET_VALUE      ( ":irs_satir_id", p_irsaliye_satiri_id );

        if ( f_query.SELECT() > 0 ) {
            f_query.NEXT();
            int iade_edilen_irsaliye_satiri_id = f_query.VALUE(0).toInt();
            if ( iade_edilen_irsaliye_satiri_id NE P_NEW_FIS_SATIRI->iade_edilen_irsaliye_satir_id )
            f_query.PREPARE_DELETE ( "irs_iade_satir_baglantisi","irs_satir_id = :irs_satir_id");
            f_query.SET_VALUE      ( ":irs_satir_id", p_irsaliye_satiri_id );
            f_query.DELETE();

            IRS_IRSALIYE_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( iade_edilen_irsaliye_satiri_id , OLD_FIS_SATIRI->satirdaki_urun_miktari, ENUM_IRS_IADE_MIKTARI_AZALT );
        }
    }

    P_FIS->brut_tutar                     =   ROUND ( P_FIS->brut_tutar                    -  OLD_FIS_SATIRI->satir_vergi_haric_tutar  );
    P_FIS->isk_oncesi_otv_tutari          =   ROUND ( P_FIS->isk_oncesi_otv_tutari         -  OLD_FIS_SATIRI->satir_otv_tutari         );
    P_FIS->isk_oncesi_otv_dahil_tutar     =   ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar    -  OLD_FIS_SATIRI->satir_otv_dahil_tutar    );
    P_FIS->isk_oncesi_kdv_tutari          =   ROUND ( P_FIS->isk_oncesi_kdv_tutari         -  OLD_FIS_SATIRI->satir_kdv_tutari         );
    P_FIS->isk_oncesi_kdv_dahil_tutar     =   ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar    -  OLD_FIS_SATIRI->satir_kdv_dahil_tutar    );


    int gun_no = MALI_YIL_ARRAY_INDIS ( QDate::fromString( P_FIS->fis_tarihi, "yyyy.MM.dd") );

    if ( P_FIS->fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
        gun_no = 0;
    }

    int irsaliye_stok_fisi_alis_satis_turu =  P_FIS->irsaliye_alis_satis_turu;;

    if ( P_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
        switch ( P_FIS->irsaliye_alis_satis_turu ) {

            case ENUM_ALIS_IRSALIYESI_FATURASI :
            default :
                irsaliye_stok_fisi_alis_satis_turu = ENUM_SATIS_IRSALIYESI_FATURASI;

            case ENUM_SATIS_IRSALIYESI_FATURASI :
                irsaliye_stok_fisi_alis_satis_turu = ENUM_ALIS_IRSALIYESI_FATURASI;
                break;
        }
    }

    int     old_urun_id                  = OLD_FIS_SATIRI->urun_id;
    double  old_temel_birim_stok_miktari = OLD_FIS_SATIRI->temel_birim_katsayisi * OLD_FIS_SATIRI->satirdaki_urun_miktari;
    double  old_satir_stok_miktari       = OLD_FIS_SATIRI->satirdaki_urun_miktari;
    int     old_parti_kodu_id            = OLD_FIS_SATIRI->parti_id;

    int     new_urun_id                  = P_NEW_FIS_SATIRI->urun_id;
    double  new_temel_birim_stok_miktari = P_NEW_FIS_SATIRI->temel_birim_katsayisi * P_NEW_FIS_SATIRI->satirdaki_urun_miktari;
    double  new_satir_stok_miktari       = P_NEW_FIS_SATIRI->satirdaki_urun_miktari;
    int     new_parti_id                 = P_NEW_FIS_SATIRI->parti_id;

    int     depo_id                      = P_FIS->depo_id;

    switch ( irsaliye_stok_fisi_alis_satis_turu ) {

        case ENUM_ALIS_IRSALIYESI_FATURASI :
        default                            :

            switch ( P_FIS->fis_turu ) {

                case ENUM_STK_DEPO_TRANSFER_CIKIS : // Depo transferlerde urun kartinda gosterilecek olan
                case ENUM_STK_DEPO_TRANSFER_GIRIS : // urun miktari degismez bir depodan digerine transfer
                    break;

                default :
                    STK_URUN_KARTINDAKI_STOK_MIKTARINI_AZALT ( old_urun_id, gun_no, old_temel_birim_stok_miktari, 0.00 );
                    STK_URUN_KARTINDAKI_STOK_MIKTARINI_ARTIR ( new_urun_id, gun_no, new_temel_birim_stok_miktari, 0.00 );
                    break;
            }

            STK_DEPODAKI_STOK_MIKTARINI_AZALT        ( depo_id, old_urun_id, gun_no, old_temel_birim_stok_miktari, 0.00 );
            STK_DEPODAKI_STOK_MIKTARINI_ARTIR        ( depo_id, new_urun_id, gun_no, new_temel_birim_stok_miktari, 0.00 );

            if ( old_urun_id EQ new_urun_id ) { // Sadece ürün miktarı değiştirilmiş
                if ( old_satir_stok_miktari NE new_satir_stok_miktari OR old_parti_kodu_id NE new_parti_id ) {

                    if ( old_stok_takip_sekli EQ ENUM_PARTI_TAKIBI ) {

                        if ( P_FIS->iade_irsaliyesi_mi NE ENUM_IRS_IADE) {
                            if( old_parti_kodu_id NE new_parti_id ) {
                                f_query.PREPARE_SELECT ( "stk_fis_satirlari","fis_satiri_id",
                                                         "parti_id = :parti_id "
                                                         "AND fis_satiri_id != :fis_satiri_id ");

                                f_query.SET_VALUE(":parti_id"      , old_parti_kodu_id    );
                                f_query.SET_VALUE(":fis_satiri_id" , p_irsaliye_satiri_id );

                                if ( f_query.SELECT() EQ 0 ) {
                                    STK_PARTIYI_KAYITLARDAN_SIL ( old_parti_kodu_id );
                                    break;
                                }
                                // Parti sadece silinen irsaliye satirinda varsa o satirla birlikte girmistir.
                                //Satir silinince parti silinebilir.
                            }
                            //else sadece miktar degismistir
                        }

                        STK_PARTIDEKI_URUN_MIKTARINI_AZALT ( old_parti_kodu_id, gun_no, old_temel_birim_stok_miktari, 0.00 );
                        STK_PARTIDEKI_URUN_MIKTARINI_ARTIR ( depo_id, new_urun_id, gun_no, P_NEW_FIS_SATIRI->parti_kodu, new_temel_birim_stok_miktari, 0.00);
                    }
                    else if ( old_stok_takip_sekli EQ ENUM_SERI_NO_ILE_TAKIP ){
                        STK_SERI_NO_LIST_SIL_ALIS( p_irsaliye_satiri_id );
                        STK_SERI_NO_LIST_ALIS( depo_id, new_urun_id, p_irsaliye_satiri_id, P_NEW_FIS_SATIRI->seri_no_list );
                    }
                }
            }
            else  {    //! URUN DEGISTI
                switch ( old_stok_takip_sekli ) {
                    case ENUM_BASIT_TAKIP       :
                    default                     :
                        break;

                    case ENUM_SERI_NO_ILE_TAKIP : {
                        STK_SERI_NO_LIST_SIL_ALIS( p_irsaliye_satiri_id );
                        break;
                    }
                    case ENUM_PARTI_TAKIBI :
                        STK_PARTIDEKI_URUN_MIKTARINI_AZALT ( old_parti_kodu_id, gun_no, old_temel_birim_stok_miktari, 0.00 );
                        break;
                }

                switch ( new_stok_takip_sekli ) {
                    case ENUM_BASIT_TAKIP       :
                    default                     :
                        break;
                    case ENUM_SERI_NO_ILE_TAKIP : {
                        STK_SERI_NO_LIST_ALIS( depo_id, new_urun_id, p_irsaliye_satiri_id, P_NEW_FIS_SATIRI->seri_no_list );
                        break;
                    }
                    case ENUM_PARTI_TAKIBI :
                        STK_PARTIDEKI_URUN_MIKTARINI_ARTIR ( depo_id, new_urun_id, gun_no, P_NEW_FIS_SATIRI->parti_kodu, new_temel_birim_stok_miktari, 0.00 );
                        break;
                }
            }

            break;

        case ENUM_SATIS_IRSALIYESI_FATURASI :

            switch ( P_FIS->fis_turu ) {

                case ENUM_STK_DEPO_TRANSFER_CIKIS : // Depo transferlerde urun kartinda gosterilecek olan
                case ENUM_STK_DEPO_TRANSFER_GIRIS : // urun miktari degismez bir depodan digerine transfer
                    break;

                default :
                    STK_URUN_KARTINDAKI_STOK_MIKTARINI_AZALT ( old_urun_id, gun_no, 0.00, old_temel_birim_stok_miktari);
                    STK_URUN_KARTINDAKI_STOK_MIKTARINI_ARTIR ( new_urun_id, gun_no, 0.00, new_temel_birim_stok_miktari);
                    break;
            }

            STK_DEPODAKI_STOK_MIKTARINI_AZALT ( depo_id, old_urun_id, gun_no, 0.00, old_temel_birim_stok_miktari );
            STK_DEPODAKI_STOK_MIKTARINI_ARTIR ( depo_id, new_urun_id, gun_no, 0.00, new_temel_birim_stok_miktari );

            if ( old_urun_id EQ new_urun_id ) { // Sadece ürün miktarı değiştirilmiş
                if ( old_satir_stok_miktari NE new_satir_stok_miktari ) {

                    if ( old_stok_takip_sekli EQ ENUM_PARTI_TAKIBI ) {
                        STK_PARTIDEKI_URUN_MIKTARINI_AZALT ( old_parti_kodu_id, gun_no,  0.00, old_temel_birim_stok_miktari );
                        STK_PARTIDEKI_URUN_MIKTARINI_ARTIR ( depo_id, new_urun_id, gun_no, P_NEW_FIS_SATIRI->parti_kodu, 0.00, new_temel_birim_stok_miktari);
                    }
                    else if( old_stok_takip_sekli EQ ENUM_SERI_NO_ILE_TAKIP ) {
                        STK_SERI_NO_LIST_IPTAL( p_irsaliye_satiri_id, 1 );
                        STK_SERI_NO_LIST_SATIS( depo_id, new_urun_id, p_irsaliye_satiri_id, P_NEW_FIS_SATIRI->seri_no_list );
                    }
                }
            }
            else  {  //! URUN DEGISTI
                switch ( old_stok_takip_sekli ) {

                    case ENUM_BASIT_TAKIP       :
                    default                     :
                        break;
                    case ENUM_SERI_NO_ILE_TAKIP : {
                        STK_SERI_NO_LIST_IPTAL( p_irsaliye_satiri_id , 1 );
                        break;
                    }
                    case ENUM_PARTI_TAKIBI :
                        STK_PARTIDEKI_URUN_MIKTARINI_AZALT ( old_parti_kodu_id, gun_no, 0.00, old_temel_birim_stok_miktari );
                        break;
                }
                switch ( new_stok_takip_sekli ) {

                    case ENUM_BASIT_TAKIP       :
                    default                     :
                        break;
                    case ENUM_SERI_NO_ILE_TAKIP : {
                        STK_SERI_NO_LIST_SATIS( depo_id, new_urun_id, p_irsaliye_satiri_id, P_NEW_FIS_SATIRI->seri_no_list );
                        break;
                    }
                    case ENUM_PARTI_TAKIBI      :
                        STK_PARTIDEKI_URUN_MIKTARINI_ARTIR ( depo_id, new_urun_id, gun_no, P_NEW_FIS_SATIRI->parti_kodu, 0.00, new_temel_birim_stok_miktari );
                        break;
                }
            }
            break;
    }

    if ( P_FIS->fis_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS OR
         P_FIS->fis_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS ) {

        switch ( new_stok_takip_sekli ) {

            case ENUM_SERI_NO_ILE_TAKIP : {

                int fat_fis_id = FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_SATIRI_ID( p_irsaliye_satiri_id );

                FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;
                FATURA_CLEAR_FIS_SATIRI_STRUCT ( &FAT_FIS_SATIRI );

                int result = FATURA_SATIR_BILGILERINI_OKU( &FAT_FIS_SATIRI, fat_fis_id );

                if ( result EQ 0 ) {
                    break;
                }

                int fatura_satirindaki_seri_no_sayisi = FAT_FIS_SATIRI.satirdaki_urun_miktari; // oncesinde faturalanan_miktar vardi

                f_query.PREPARE_SELECT ("stk_satirdaki_seri_nolari",
                                        "satir_seri_no_id ",
                                        "irsaliye_satiri_id = :irsaliye_satiri_id");
                f_query.SET_VALUE ( ":irsaliye_satiri_id", p_irsaliye_satiri_id );
                if ( f_query.SELECT() EQ fatura_satirindaki_seri_no_sayisi  ) {
                    P_NEW_FIS_SATIRI->hatali_satir_mi = 0;
                }
                else {
                    P_NEW_FIS_SATIRI->hatali_satir_mi = 1;
                }
                break;
            }

            default :
                break;
        }
    }

    f_query.PREPARE_UPDATE ( "stk_fis_satirlari ","fis_satiri_id",
                             "urun_id                           , "
                             "satirdaki_urun_miktari            , "
                             "urun_birimi                       , "
                             "temel_birim_katsayisi             , "
                             "isk_oncesi_urun_birim_fiyati      , "
                             "isk_sonrasi_urun_birim_fiyati     , "
                             "satir_iskonto_yuzdesi             , "
                             "satir_iskonto_tutari              , "
                             "satir_tutari                      , "
                             "doviz_id                          , "
                             "kdv_orani                         , "
                             "kdv_dahil_mi                      , "
                             "otv_orani                         , "
                             "satir_vergi_haric_tutar           , "
                             "satir_otv_tutari                  , "
                             "satir_otv_dahil_tutar             , "
                             "satir_kdv_tutari                  , "
                             "satir_kdv_dahil_tutar             , "
                             "parti_id                          , "
                             "hatali_satir_mi                   , "
                             "toplu_iskonto_yuzdesi             , "
                             "t_is_son_birim_fiyati             , "
                             "t_is_son_satir_tutari             , "
                             "t_is_son_str_vergi_haric_tutar    , "
                             "t_is_son_satir_otv_tutari         , "
                             "t_is_son_satir_otv_dahil_tutar    , "
                             "t_is_son_satir_kdv_tutari         , "
                             "t_is_son_satir_kdv_dahil_tutar      ",
                             "fis_satiri_id = :fis_satiri_id" );

    f_query.SET_VALUE ( ":urun_id"                            , P_NEW_FIS_SATIRI->urun_id                           );
    f_query.SET_VALUE ( ":satirdaki_urun_miktari"             , P_NEW_FIS_SATIRI->satirdaki_urun_miktari            );
    f_query.SET_VALUE ( ":urun_birimi"                        , P_NEW_FIS_SATIRI->urun_birimi                       );
    f_query.SET_VALUE ( ":temel_birim_katsayisi"              , P_NEW_FIS_SATIRI->temel_birim_katsayisi             );
    f_query.SET_VALUE ( ":isk_oncesi_urun_birim_fiyati"       , P_NEW_FIS_SATIRI->isk_oncesi_urun_birim_fiyati      );
    f_query.SET_VALUE ( ":isk_sonrasi_urun_birim_fiyati"      , P_NEW_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati     );
    f_query.SET_VALUE ( ":satir_iskonto_yuzdesi"              , P_NEW_FIS_SATIRI->iskonto_yuzdesi                   );
    f_query.SET_VALUE ( ":satir_iskonto_tutari"               , P_NEW_FIS_SATIRI->iskonto_tutari                    );
    f_query.SET_VALUE ( ":satir_tutari"                       , P_NEW_FIS_SATIRI->satir_tutari                      );
    f_query.SET_VALUE ( ":doviz_id"                           , P_NEW_FIS_SATIRI->doviz_id                          );
    f_query.SET_VALUE ( ":kdv_orani"                          , P_NEW_FIS_SATIRI->kdv_orani                         );
    f_query.SET_VALUE ( ":kdv_dahil_mi"                       , P_NEW_FIS_SATIRI->kdv_dahil_mi                      );
    f_query.SET_VALUE ( ":otv_orani"                          , P_NEW_FIS_SATIRI->otv_orani                         );
    f_query.SET_VALUE ( ":satir_vergi_haric_tutar"            , P_NEW_FIS_SATIRI->satir_vergi_haric_tutar           );
    f_query.SET_VALUE ( ":satir_otv_tutari"                   , P_NEW_FIS_SATIRI->satir_otv_tutari                  );
    f_query.SET_VALUE ( ":satir_otv_dahil_tutar"              , P_NEW_FIS_SATIRI->satir_otv_dahil_tutar             );
    f_query.SET_VALUE ( ":satir_kdv_tutari"                   , P_NEW_FIS_SATIRI->satir_kdv_tutari                  );
    f_query.SET_VALUE ( ":satir_kdv_dahil_tutar"              , P_NEW_FIS_SATIRI->satir_kdv_dahil_tutar             );
    f_query.SET_VALUE ( ":parti_id"                           , P_NEW_FIS_SATIRI->parti_id                          );
    f_query.SET_VALUE ( ":hatali_satir_mi"                    , P_NEW_FIS_SATIRI->hatali_satir_mi                   );
    f_query.SET_VALUE ( ":toplu_iskonto_yuzdesi"              , P_NEW_FIS_SATIRI->toplu_iskonto_yuzdesi             );
    f_query.SET_VALUE ( ":t_is_son_birim_fiyati"              , P_NEW_FIS_SATIRI->t_is_sonrasi_birim_fiyat          );
    f_query.SET_VALUE ( ":t_is_son_satir_tutari"              , P_NEW_FIS_SATIRI->t_is_sonrasi_satir_tutari         );
    f_query.SET_VALUE ( ":t_is_son_str_vergi_haric_tutar"     , P_NEW_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar    );
    f_query.SET_VALUE ( ":t_is_son_satir_otv_tutari"          , P_NEW_FIS_SATIRI->t_is_sonrasi_otv_tutari           );
    f_query.SET_VALUE ( ":t_is_son_satir_otv_dahil_tutar"     , P_NEW_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar      );
    f_query.SET_VALUE ( ":t_is_son_satir_kdv_tutari"          , P_NEW_FIS_SATIRI->t_is_sonrasi_kdv_tutari           );
    f_query.SET_VALUE ( ":t_is_son_satir_kdv_dahil_tutar"     , P_NEW_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar      );
    f_query.SET_VALUE ( ":fis_satiri_id"                      , p_irsaliye_satiri_id                            );

    f_query.UPDATE();

    // FATURA 'DAN GELDIGINDE NORMAL IRSALIYE GELCEK VE FATURA SATIRI GUNCELLEMESI GEREKECEK
    switch ( P_FIS->fis_turu  ) {
        case ENUM_IRS_NORMAL_GIRIS  :
        case ENUM_IRS_NORMAL_CIKIS  :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
        {
            f_query.PREPARE_SELECT ( "fat_irs_satir_baglantisi","fatura_satiri_id ",
                                     "irsaliye_satiri_id = :irsaliye_satiri_id");
            f_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );
            if ( f_query.SELECT() EQ 0 ) {
                break;
            }
            f_query.NEXT();
            int old_fatura_satiri_id = f_query.VALUE(0).toInt();
            if ( old_fatura_satiri_id NE P_NEW_FIS_SATIRI->fatura_satiri_id ) {
                f_query.PREPARE_INSERT ( "fat_irs_satir_baglantisi","kayit_id",
                                         "fatura_satiri_id, irsaliye_satiri_id" );

                f_query.SET_VALUE(":fatura_satiri_id"   , P_NEW_FIS_SATIRI->fatura_satiri_id );
                f_query.SET_VALUE(":irsaliye_satiri_id" , p_irsaliye_satiri_id   );
                f_query.INSERT();
            }
            if( P_FIS->fis_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  OR P_FIS->fis_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  ) {
                FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE ( P_NEW_FIS_SATIRI->fatura_satiri_id, new_satir_stok_miktari, ENUM_FAT_IRSALIYELENEN_MIKTAR_ARTIR );
            }
            else {
                FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE ( P_NEW_FIS_SATIRI->fatura_satiri_id, new_satir_stok_miktari, ENUM_FAT_IRSALIYELENEN_MIKTAR_GUNCELLE );
            }
            break;
        }
        default :
           break;
    }

    if ( P_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE) {
        f_query.PREPARE_SELECT ( "irs_iade_satir_baglantisi",
                                 "irs_iade_edilen_satir_id ",
                                 "irs_satir_id = :irs_satir_id");
        f_query.SET_VALUE      ( ":irs_satir_id", p_irsaliye_satiri_id );

        if ( f_query.SELECT() EQ 0 ) {
           f_query.PREPARE_INSERT ( "irs_iade_satir_baglantisi","kayit_id", "irs_iade_edilen_satir_id,"
                                    "irs_satir_id ");
           f_query.SET_VALUE      ( ":irs_iade_edilen_satir_id", P_NEW_FIS_SATIRI->iade_edilen_irsaliye_satir_id );
           f_query.SET_VALUE      ( ":irs_satir_id",             p_irsaliye_satiri_id                        );
           f_query.INSERT();
        }

        IRS_IRSALIYE_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( P_NEW_FIS_SATIRI->iade_edilen_irsaliye_satir_id,
                                                         P_NEW_FIS_SATIRI->satirdaki_urun_miktari,
                                                         ENUM_IRS_IADE_MIKTARI_ARTIR );
    }

    P_FIS->iskonto_yuzdesi = ROUND( P_FIS->iskonto_yuzdesi );

    //! FISIN YENI SATIR BILGILERI FIS' e EKLENIYOR
    P_FIS->brut_tutar                     = ROUND ( P_FIS->brut_tutar                    + P_NEW_FIS_SATIRI->satir_vergi_haric_tutar    );
    P_FIS->isk_oncesi_otv_tutari          = ROUND ( P_FIS->isk_oncesi_otv_tutari         + P_NEW_FIS_SATIRI->satir_otv_tutari           );
    P_FIS->isk_oncesi_otv_dahil_tutar     = ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar    + P_NEW_FIS_SATIRI->satir_otv_dahil_tutar      );
    P_FIS->isk_oncesi_kdv_tutari          = ROUND ( P_FIS->isk_oncesi_kdv_tutari         + P_NEW_FIS_SATIRI->satir_kdv_tutari           );
    P_FIS->isk_oncesi_kdv_dahil_tutar     = ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar    + P_NEW_FIS_SATIRI->satir_kdv_dahil_tutar      );

    P_FIS->ara_toplam                     = ROUND ( P_FIS->brut_tutar                    * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->isk_sonrasi_otv_tutari         = ROUND ( P_FIS->isk_oncesi_otv_tutari         * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->isk_sonrasi_otv_dahil_tutar    = ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar    * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->isk_sonrasi_kdv_tutari         = ROUND ( P_FIS->isk_oncesi_kdv_tutari         * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->irsaliye_tutari                = ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar    * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->iskonto_tutari                 = ROUND ( P_FIS->brut_tutar                    - P_FIS->ara_toplam                        );


    //! FIS UPDATE EDILIYOR.
    STK_IRS_FIS_UPDATE( P_NEW_FIS_SATIRI->fis_id, P_FIS );

    // EGER SATIR TUTARLARI DEGISMIS ISE MUHASEBE ENT TEKRAR YAPIYORUZ.
    if( OLD_FIS_SATIRI->satir_tutari NE P_NEW_FIS_SATIRI->satir_tutari ) {
        P_NEW_FIS_SATIRI->base_record_id = p_irsaliye_satiri_id;

        if ( P_FIS->fis_turu EQ  ENUM_STK_SARF_FIRE_FISI ) {

            QStringList muh_row_list = MUH_GET_ENT_ROW_ID_LIST( E9_PROGRAMI, STOK_MODULU, p_irsaliye_satiri_id );

            if ( muh_row_list.count() > 0 ) {

                MUH_FIS_STRUCT * MUH_FIS        = new MUH_FIS_STRUCT;

                MUH_FIS->modul_id               = STOK_MODULU;
                MUH_FIS->program_id             = E9_PROGRAMI;
                MUH_FIS->base_fis_id            = P_NEW_FIS_SATIRI->fis_id;

                for ( int i = 0; i< muh_row_list.count(); i++ ) {
                        MUH_FIS_SATIRINI_SIL( MUH_FIS, QVariant ( muh_row_list.at( i ) ).toInt() );
                }
                delete MUH_FIS;
            }
            //! Yeni degerler kaydediliyor.

            STK_ENT_FIS_SATIRI_EKLE( P_FIS, P_NEW_FIS_SATIRI );
        }
    }

    delete OLD_FIS_SATIRI;

    return ADAK_OK;
}

/***************************************************************************************
                  STK_IRS_FIS_SIL
***************************************************************************************/

void STK_IRS_FIS_SIL( int p_irsaliye_id, STK_IRS_FIS_STRUCT *P_FIS )
{
    SQL_QUERY   query         ( DB );

    STK_IRS_FIS_SATIR_STRUCT * FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

    query.PREPARE_SELECT( "stk_fis_satirlari", "fis_satiri_id", " fis_id = :fis_id");
    query.SET_VALUE     ( ":fis_id", p_irsaliye_id );

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {
            int fis_satiri_id = query.VALUE( 0 ).toInt();
            STK_IRS_CLEAR_SATIR_STRUCT( FIS_SATIRI );

            STK_IRS_SATIR_BILGILERINI_OKU( fis_satiri_id, FIS_SATIRI );

            STK_IRS_FIS_SATIRI_SIL( P_FIS, FIS_SATIRI, fis_satiri_id );
        }
    }

    switch( P_FIS->fis_turu ) {
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS:
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS : {

            query.PREPARE_SELECT ( "fat_irs_baglantisi ","fatura_id", "irsaliye_id = :irsaliye_id");
            query.SET_VALUE      (":irsaliye_id", p_irsaliye_id );

            int fatura_id = 0;
            if ( query.SELECT() > 0 ) {

                while ( query.NEXT() EQ true ) {

                    fatura_id = query.VALUE(0).toInt();
                    FAT_UPDATE_FATURA_IRSALIYELESTIRILDI_MI( fatura_id , 0);
                }

                query.PREPARE_DELETE ( "fat_irs_baglantisi", "irsaliye_id = :irsaliye_id" );
                query.SET_VALUE      ( ":irsaliye_id", p_irsaliye_id );
                query.DELETE();
            }

            //! otomatik irsaliye silinginde satirda ki irsaliyelenen miktar 0 lanmali
            if( P_FIS->fis_turu EQ  ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS OR
                P_FIS->fis_turu EQ  ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS ) {

                query.PREPARE_UPDATE( "fat_fatura_satirlari", "fis_satiri_id", "irsaliyelenen_miktar", "fis_id = :fis_id");
                query.SET_VALUE     ( ":irsaliyelenen_miktar", 0 );
                query.SET_VALUE     ( ":fis_id", fatura_id );
                query.UPDATE();
            }

            break;
        }
        default:
            break;
    }

    if ( P_FIS->yazdirildi_mi EQ 1 ) {
        query.PREPARE_UPDATE ( "stk_fisler","fis_id", "iptal_mi","fis_id = :fis_id" );
        query.SET_VALUE      ( ":iptal_mi", 1 );
        query.SET_VALUE      ( ":fis_id"  , p_irsaliye_id );
        query.UPDATE();
        P_FIS->iptal_mi = 1;
    }
    else {

        query.PREPARE_DELETE("stk_fisler","fis_id = :fis_id");
        query.SET_VALUE(":fis_id", p_irsaliye_id );
        query.DELETE();
    }

    //FIXME IPTAL EDILEN BIR STOK FISI ENT SILINIR MI ???
    if ( P_FIS->muh_fis_id EQ ENUM_STK_SARF_FIRE_FISI ) {

        MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;
        MUH_CLEAR_FIS_STRUCT   ( MUH_FIS );
        MUH_FIS_BILGILERINI_OKU( MUH_FIS, P_FIS->muh_fis_id );
        MUH_FIS_SIL( MUH_FIS, P_FIS->muh_fis_id );

        delete MUH_FIS;
    }

    delete FIS_SATIRI;
}

/***************************************************************************************
                  STK_IRS_FIS_SATIRI_SIL
***************************************************************************************/

void STK_IRS_FIS_SATIRI_SIL( STK_IRS_FIS_STRUCT *P_FIS, STK_IRS_FIS_SATIR_STRUCT *P_FIS_SATIRI, int p_irsaliye_satiri_id  )
{

    SQL_QUERY f_query( DB );
    SQL_QUERY s_query( DB );

    int gun_no = MALI_YIL_ARRAY_INDIS ( QDate::fromString ( P_FIS->fis_tarihi, "yyyy.MM.dd" ) );

    int stok_takip_sekli = -1;

    double temel_stok_miktari = ROUND( P_FIS_SATIRI->temel_birim_katsayisi * P_FIS_SATIRI->satirdaki_urun_miktari );

    if ( P_FIS->muh_fis_id > 0 ) {
        // olusturulan muh satirlari siliniyor
        QStringList muh_row_list = MUH_GET_ENT_ROW_ID_LIST( E9_PROGRAMI, STOK_MODULU, p_irsaliye_satiri_id );

        if ( muh_row_list.count() > 0 ) {

            MUH_FIS_STRUCT * MUH_FIS        = new MUH_FIS_STRUCT;

            MUH_FIS->modul_id               = STOK_MODULU;
            MUH_FIS->program_id             = E9_PROGRAMI;
            MUH_FIS->base_fis_id            = P_FIS_SATIRI->fis_id;

            for ( int i = 0; i< muh_row_list.count(); i++ ) {
                    MUH_FIS_SATIRINI_SIL( MUH_FIS, QVariant ( muh_row_list.at( i ) ).toInt() );
            }
            delete MUH_FIS;
        }
    }

    if ( P_FIS_SATIRI->urun_id > 0 ) {
        stok_takip_sekli = STK_GET_URUN_TAKIP_SEKLI( P_FIS_SATIRI->urun_id );
    }

    if ( P_FIS->fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
        gun_no = 0;
    }

    P_FIS->brut_tutar                  = ROUND ( P_FIS->brut_tutar                  - P_FIS_SATIRI->satir_vergi_haric_tutar    );
    P_FIS->isk_oncesi_otv_tutari       = ROUND ( P_FIS->isk_oncesi_otv_tutari       - P_FIS_SATIRI->satir_otv_tutari           );
    P_FIS->isk_oncesi_otv_dahil_tutar  = ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar  - P_FIS_SATIRI->satir_otv_dahil_tutar      );
    P_FIS->isk_oncesi_kdv_tutari       = ROUND ( P_FIS->isk_oncesi_kdv_tutari       - P_FIS_SATIRI->satir_kdv_tutari           );
    P_FIS->isk_oncesi_kdv_dahil_tutar  = ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar  - P_FIS_SATIRI->satir_kdv_dahil_tutar      );

    P_FIS->ara_toplam                  = ROUND ( P_FIS->brut_tutar                  * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->isk_sonrasi_otv_tutari      = ROUND ( P_FIS->isk_oncesi_otv_tutari       * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->isk_sonrasi_otv_dahil_tutar = ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar  * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->isk_sonrasi_kdv_tutari      = ROUND ( P_FIS->isk_oncesi_kdv_tutari       * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->irsaliye_tutari             = ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar  * ( 1 - ( P_FIS->iskonto_yuzdesi / 100 ) ) );
    P_FIS->iskonto_tutari              = ROUND ( P_FIS->brut_tutar                  - P_FIS->ara_toplam                        );

    switch ( P_FIS->fis_turu ) {
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
        {
            f_query.PREPARE_SELECT("fat_irs_satir_baglantisi","fatura_satiri_id ",
                                   "irsaliye_satiri_id = :irsaliye_satiri_id");
            f_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );
            if ( f_query.SELECT() EQ 0 ) {
                break;
            }
            f_query.NEXT();

            int fatura_satiri_id = f_query.VALUE(0).toInt();

            f_query.PREPARE_DELETE("fat_irs_satir_baglantisi ",
                                   "irsaliye_satiri_id = :irsaliye_satiri_id");
            f_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );
            f_query.DELETE();

            FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE ( fatura_satiri_id, P_FIS_SATIRI->satirdaki_urun_miktari, ENUM_FAT_IRSALIYELENEN_MIKTAR_AZALT );
            break;
        }
        default :
            break;
    }

    if ( P_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
        f_query.PREPARE_SELECT ( "irs_iade_satir_baglantisi","irs_iade_edilen_satir_id",
                                 "irs_satir_id = :irs_satir_id");
        f_query.SET_VALUE      ( ":irs_satir_id", p_irsaliye_satiri_id );
        if ( f_query.SELECT() > 0 ) {
            f_query.NEXT();
            int iade_edilen_irsaliye_satiri_id = f_query.VALUE(0).toInt();
            f_query.PREPARE_DELETE ( "irs_iade_satir_baglantisi","irs_satir_id = :irs_satir_id");
            f_query.SET_VALUE      ( ":irs_satir_id", p_irsaliye_satiri_id );
            f_query.DELETE();

            IRS_IRSALIYE_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( iade_edilen_irsaliye_satiri_id, P_FIS_SATIRI->satirdaki_urun_miktari, ENUM_IRS_IADE_MIKTARI_AZALT );
        }
    }

    switch ( P_FIS->irsaliye_alis_satis_turu ) {

        case ENUM_ALIS_IRSALIYESI_FATURASI :
        default                            :

            switch ( stok_takip_sekli ) {

                case ENUM_BASIT_TAKIP       :
                default                     :
                    break;
                case ENUM_SERI_NO_ILE_TAKIP :

                    f_query.PREPARE_SELECT( "stk_satirdaki_seri_nolari","seri_numarasi_id ", "irsaliye_satiri_id = :irsaliye_satiri_id");
                    f_query.SET_VALUE     ( ":irsaliye_satiri_id", p_irsaliye_satiri_id );

                    if ( f_query.SELECT() EQ 0 ) {
                        break;
                    }
                    // IADE DE URUNUN DEPO DURUMU DEGISIR DB' DEN SILINMEZ
                    if ( P_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                        while ( f_query.NEXT() EQ true ) {
                            s_query.PREPARE_UPDATE( "stk_urunun_seri_nolari ", "seri_numarasi_id",
                                                    "depoda_mi", "seri_numarasi_id = :seri_numarasi_id ");
                            s_query.SET_VALUE(":depoda_mi",        0 );
                            s_query.SET_VALUE(":seri_numarasi_id", f_query.VALUE(0).toInt() );
                            s_query.UPDATE();
                        }
                    }
                    else {
                        while ( f_query.NEXT() EQ true ) {
                            s_query.PREPARE_DELETE("stk_urunun_seri_nolari ", "seri_numarasi_id = :seri_numarasi_id");
                            s_query.SET_VALUE(":seri_numarasi_id", f_query.VALUE(0).toInt() );
                            s_query.DELETE();
                        }
                    }

                    f_query.PREPARE_DELETE ( "stk_satirdaki_seri_nolari ", "irsaliye_satiri_id = :irsaliye_satiri_id " );
                    f_query.SET_VALUE      ( ":irsaliye_satiri_id" , p_irsaliye_satiri_id  );
                    f_query.DELETE();

                    break;

                case ENUM_PARTI_TAKIBI :

                    if ( P_FIS->iade_irsaliyesi_mi NE ENUM_IRS_IADE) {
                        f_query.PREPARE_SELECT ( "stk_fis_satirlari","fis_satiri_id", "parti_id = :parti_id "
                                                 "AND fis_satiri_id != :fis_satiri_id ");

                        f_query.SET_VALUE(":parti_id"      , P_FIS_SATIRI->parti_id );
                        f_query.SET_VALUE(":fis_satiri_id" , p_irsaliye_satiri_id );

                        if ( f_query.SELECT() EQ 0 ) {
                            STK_PARTIYI_KAYITLARDAN_SIL ( P_FIS_SATIRI->parti_id );
                            break;
                        }
                        // Parti sadece silinen irsaliye satirinda varsa o satirla birlikte girmistir.
                        //Satir silinince parti silinebilir.
                    }
                    STK_PARTIDEKI_URUN_MIKTARINI_AZALT( P_FIS_SATIRI->parti_id, gun_no, temel_stok_miktari, 0.00 );
                    break;
            }
            // URUN KARTINDAKI VE DEPODAKI URUN MIKTARLARI AZALTILIYOR
            STK_URUN_KARTINDAKI_STOK_MIKTARINI_AZALT ( P_FIS_SATIRI->urun_id, gun_no, temel_stok_miktari, 0.00 );
            STK_DEPODAKI_STOK_MIKTARINI_AZALT        ( P_FIS->depo_id, P_FIS_SATIRI->urun_id, gun_no, temel_stok_miktari, 0.00 );
            break;

        case ENUM_SATIS_IRSALIYESI_FATURASI :

            switch ( stok_takip_sekli ) {

                case ENUM_BASIT_TAKIP       :
                default                     :
                    break;
                case ENUM_SERI_NO_ILE_TAKIP :
                    // SATISTAN IADE SILME
                    STK_SERI_NO_LIST_IPTAL( p_irsaliye_satiri_id, 1 );
                    break;
                case ENUM_PARTI_TAKIBI :
                    STK_PARTIDEKI_URUN_MIKTARINI_AZALT ( P_FIS_SATIRI->parti_id, gun_no,  0.00, temel_stok_miktari );
                    break;
            }
            switch ( P_FIS->fis_turu ) {

                case ENUM_STK_DEPO_TRANSFER_CIKIS : // Depo transferlerde urun kartinda gosterilecek olan
                case ENUM_STK_DEPO_TRANSFER_GIRIS : // urun miktari degismez bir depodan digerine transfer
                    break;

                default :
                    STK_URUN_KARTINDAKI_STOK_MIKTARINI_AZALT ( P_FIS_SATIRI->urun_id, gun_no, 0.00, temel_stok_miktari );
                    break;
            }
            STK_DEPODAKI_STOK_MIKTARINI_AZALT ( P_FIS->depo_id, P_FIS_SATIRI->urun_id, gun_no, 0.00, temel_stok_miktari );
            break;
    }

    //! SATIRI SILINIYOR
    f_query.PREPARE_DELETE  ( "stk_fis_satirlari","fis_satiri_id = :fis_satiri_id" );
    f_query.SET_VALUE       ( ":fis_satiri_id", p_irsaliye_satiri_id  );

    f_query.DELETE();

    //! Fis Update ediliyor..
    STK_IRS_FIS_UPDATE( P_FIS_SATIRI->fis_id, P_FIS );

}

/***************************************************************************************
                  STK_IRS_FIS_HAREKERLERI_OLUSTUR
***************************************************************************************/

int STK_IRS_FIS_HAREKERLERI_OLUSTUR ( STK_IRS_FIS_STRUCT *P_FIS )
{

    Q_UNUSED ( P_FIS );
    //! BURADA FISIN OLASI BUTUN DURUMLAR ICIN HAREKET ( ENTEGRASYON ) OLUSTURULACAK
    //! ISLEM BASARILI BIR SEKILDE SONUCLANIRSA ADAK_OK, BASARISIZ OLURSA ADAK_FAIL
    //! DONDRULULECEK

    return ADAK_OK;
}

/***************************************************************************************
                  STK_IRS_FIS_UPDATE
***************************************************************************************/

void STK_IRS_FIS_UPDATE( int p_fis_id, STK_IRS_FIS_STRUCT * P_FIS )
{
    SQL_QUERY f_query( DB );
    //! kontroller

    //! UPDATE ISLEMI

    f_query.PREPARE_UPDATE ( "stk_fisler ", "fis_id",
                             "brut_tutar                    , "
                             "isk_oncesi_otv_tutari         , "
                             "isk_oncesi_otv_dahil_tutar    , "
                             "isk_oncesi_kdv_tutari         , "
                             "isk_oncesi_kdv_dahil_tutar    , "
                             "ara_toplam                    , "
                             "isk_sonrasi_otv_tutari        , "
                             "isk_sonrasi_otv_dahil_tutar   , "
                             "isk_sonrasi_kdv_tutari        , "
                             "irsaliye_tutari               , "
                             "iskonto_yuzdesi               , "
                             "iskonto_tutari                , "
                             "doviz_tutar_bilgileri         , "
                             "kdv_oran_bilgileri            , "
                             "otv_oran_bilgileri            , "
                             "stk_irs_para_birim_id         , "
                             "parite                        , "
                             "doviz_kuru                      ",
                             "fis_id = :fis_id" );

    f_query.SET_VALUE   ( ":brut_tutar",                        P_FIS->brut_tutar                   );
    f_query.SET_VALUE   ( ":isk_oncesi_otv_tutari",             P_FIS->isk_oncesi_otv_tutari        );
    f_query.SET_VALUE   ( ":isk_oncesi_otv_dahil_tutar",        P_FIS->isk_oncesi_otv_dahil_tutar   );
    f_query.SET_VALUE   ( ":isk_oncesi_kdv_tutari",             P_FIS->isk_oncesi_kdv_tutari        );
    f_query.SET_VALUE   ( ":isk_oncesi_kdv_dahil_tutar",        P_FIS->isk_oncesi_kdv_dahil_tutar   );
    f_query.SET_VALUE   ( ":ara_toplam",                        P_FIS->ara_toplam                   );
    f_query.SET_VALUE   ( ":isk_sonrasi_otv_tutari",            P_FIS->isk_sonrasi_otv_tutari       );
    f_query.SET_VALUE   ( ":isk_sonrasi_otv_dahil_tutar",       P_FIS->isk_sonrasi_otv_dahil_tutar  );
    f_query.SET_VALUE   ( ":isk_sonrasi_kdv_tutari",            P_FIS->isk_sonrasi_kdv_tutari       );
    f_query.SET_VALUE   ( ":irsaliye_tutari",                   P_FIS->irsaliye_tutari              );
    f_query.SET_VALUE   ( ":iskonto_yuzdesi",                   P_FIS->iskonto_yuzdesi              );
    f_query.SET_VALUE   ( ":iskonto_tutari",                    P_FIS->iskonto_tutari               );
    f_query.SET_VALUE   ( ":doviz_tutar_bilgileri",             P_FIS->doviz_tutar_bilgileri        );
    f_query.SET_VALUE   ( ":kdv_oran_bilgileri",                P_FIS->kdv_oran_bilgileri           );
    f_query.SET_VALUE   ( ":otv_oran_bilgileri",                P_FIS->otv_oran_bilgileri           );
    f_query.SET_VALUE   ( ":stk_irs_para_birim_id",             P_FIS->stk_irs_para_birimi_id       );
    f_query.SET_VALUE   ( ":parite",                            P_FIS->parite                       );
    f_query.SET_VALUE   ( ":doviz_kuru",                        P_FIS->doviz_kuru                   );
    f_query.SET_VALUE   ( ":fis_id",                            p_fis_id                            );

    f_query.UPDATE      ();
}

/***************************************************************************************
                  STK_IRS_CLEAR_SATIR_STRUCT
***************************************************************************************/

void STK_IRS_CLEAR_SATIR_STRUCT ( STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI )
{
    P_FIS_SATIRI->row_number                        = 0;
    P_FIS_SATIRI->fis_id                            = 0;
    P_FIS_SATIRI->urun_id                           = 0;
    P_FIS_SATIRI->urun_adi.clear();
    P_FIS_SATIRI->satirdaki_urun_miktari            = 0.00;
    P_FIS_SATIRI->iade_edilen_miktar                = 0.00;
    P_FIS_SATIRI->urun_birimi.clear();
    P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati      = 0.00;
    P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati     = 0.00;
    P_FIS_SATIRI->iskonto_yuzdesi                   = 0.00;
    P_FIS_SATIRI->iskonto_tutari                    = 0.00;
    P_FIS_SATIRI->satir_tutari                      = 0.00;
    P_FIS_SATIRI->doviz_id                          = 0;
    P_FIS_SATIRI->doviz_kodu.clear();;
    P_FIS_SATIRI->satir_vergi_haric_tutar           = 0.00;
    P_FIS_SATIRI->kdv_orani                         = 0.00;
    P_FIS_SATIRI->satir_kdv_tutari                  = 0.00;
    P_FIS_SATIRI->satir_kdv_dahil_tutar             = 0.00;
    P_FIS_SATIRI->kdv_dahil_mi                      = 0;
    P_FIS_SATIRI->order_number                      = 0;
    P_FIS_SATIRI->temel_birim_katsayisi             = 0.00;
    P_FIS_SATIRI->otv_orani                         = 0.00;
    P_FIS_SATIRI->satir_otv_tutari                  = 0.00;
    P_FIS_SATIRI->satir_otv_dahil_tutar             = 0.00;
    P_FIS_SATIRI->fatura_satiri_id                  = 0;
    P_FIS_SATIRI->stok_takip_sekli                  = 0;
    P_FIS_SATIRI->parti_id                          = 0;
    P_FIS_SATIRI->parti_kodu.clear();;
    P_FIS_SATIRI->hatali_satir_mi                   = 0;
    P_FIS_SATIRI->iade_edilen_irsaliye_satir_id     = 0;
    P_FIS_SATIRI->toplu_iskonto_yuzdesi             = 0.00;
    P_FIS_SATIRI->t_is_sonrasi_birim_fiyat          = 0.00;
    P_FIS_SATIRI->t_is_sonrasi_satir_tutari         = 0.00;
    P_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar    = 0.00;
    P_FIS_SATIRI->t_is_sonrasi_otv_tutari           = 0.00;
    P_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar      = 0.00;
    P_FIS_SATIRI->t_is_sonrasi_kdv_tutari           = 0.00;
    P_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar      = 0.00;
    P_FIS_SATIRI->faturalanan_miktar                = 0;
    P_FIS_SATIRI->base_record_id                    = 0;
}

/***************************************************************************************
                   STK_IRS_CLEAR_FIS_STRUCT
***************************************************************************************/
void STK_IRS_CLEAR_FIS_STRUCT(STK_IRS_FIS_STRUCT *P_FIS)
{     
    P_FIS->fis_no = 0;
    P_FIS->aciklama.clear();
    P_FIS->fis_tarihi.clear();
    P_FIS->fis_turu = -1;
    P_FIS->program_id = -1;
    P_FIS->modul_id = -1;
    P_FIS->base_fis_id = -1;
    P_FIS->muh_fis_id = -1;
    P_FIS->depo_id  = -1;
    P_FIS->belge_seri.clear();
    P_FIS->belge_numarasi.clear();
    P_FIS->islem_saati.clear();
    P_FIS->irsaliye_mi_stok_fisi_mi = -1;
    P_FIS->irsaliye_alis_satis_turu = -1;
    P_FIS->brut_tutar = 0.00;
    P_FIS->isk_oncesi_otv_tutari = 0.00;
    P_FIS->isk_oncesi_otv_dahil_tutar= 0.00;
    P_FIS->isk_oncesi_kdv_tutari = 0.00;
    P_FIS->isk_oncesi_kdv_dahil_tutar= 0.00;
    P_FIS->ara_toplam = 0.00;
    P_FIS->isk_sonrasi_otv_tutari = 0.00;
    P_FIS->isk_sonrasi_otv_dahil_tutar  = 0.00;
    P_FIS->isk_sonrasi_kdv_tutari = 0.00;
    P_FIS->irsaliye_tutari = 0.00;
    P_FIS->iskonto_yuzdesi = 0.00;
    P_FIS->iskonto_tutari  = 0.00;
    P_FIS->doviz_tutar_bilgileri = 0.00;
    P_FIS->kdv_oran_bilgileri = 0.00;
    P_FIS->otv_oran_bilgileri = 0.00;
    P_FIS->alan_depo_id = 0.00;
    P_FIS->sevk_tarihi.clear();
    P_FIS->cari_hesap_id = -1;
    P_FIS->cari_para_birim_id = -1;
    P_FIS->cari_hesap_ismi.clear();
    P_FIS->cari_para_birim_kodu.clear();
    P_FIS->vergi_dairesi.clear();
    P_FIS->vergi_numarasi.clear();
    P_FIS->teslim_adresi.clear();
    P_FIS->yazdirildi_mi    = -1;
    P_FIS->iptal_mi = -1;
    P_FIS->irsaliye_faturalastirildi_mi = -1;
    P_FIS->iade_irsaliyesi_mi = -1;
    P_FIS->iade_edilen_irs_id = -1;
    P_FIS->kdv_muaf_mi = -1;
    P_FIS->otv_muaf_mi = -1;
    P_FIS->muh_hesap_id = -1;
    P_FIS->stk_irs_para_birimi_id = -1;

    P_FIS->doviz_kuru = 0.00;
    P_FIS->parite = 0.00;

}



/***************************************************************************************
                       STK_ENT_FIS_EKLE
***************************************************************************************/

int  STK_ENT_FIS_EKLE ( STK_IRS_FIS_STRUCT * P_FIS , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    // Buraya ADD_FIS_RECORD gelecek
    MUH_FIS_STRUCT * MUH_FIS    = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

    MUH_FIS->aciklama               = QString ( QObject::tr ( " Fiş STOK MODÜLÜ tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_FIS->fis_no ).toInt() );
    MUH_FIS->aciklama.append( P_FIS->aciklama );
    MUH_FIS->fis_tarihi             = P_FIS->fis_tarihi;
    MUH_FIS->fis_turu               = ENUM_MAHSUP_FISI;
    MUH_FIS->modul_id               = P_FIS->modul_id;
    MUH_FIS->program_id             = P_FIS->program_id;
    MUH_FIS->base_fis_id            = P_FIS->base_fis_id;

    P_FIS->muh_fis_id = MUH_FIS_EKLE ( MUH_FIS , USE_DB);

    return P_FIS->muh_fis_id;
}

/***************************************************************************************
                       STK_ENT_FIS_SATIRI_EKLE
***************************************************************************************/

int STK_ENT_FIS_SATIRI_EKLE ( STK_IRS_FIS_STRUCT * P_FIS, STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI, ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT ( "stk_urunler","muh_hesap_id", "urun_id = :urun_id");
    query.SET_VALUE      ( ":urun_id", P_FIS_SATIRI->urun_id );

    int urun_muh_hesap_id   =   0;
    if ( query.SELECT() > 0 ) {
          query.NEXT();
          urun_muh_hesap_id = query.VALUE( 0 ).toInt();
    }

    MUH_FIS_STRUCT         * MUH_FIS          = new MUH_FIS_STRUCT;
    MUH_FIS_SATIRI_STRUCT  * MUH_FIS_SATIRI   = new MUH_FIS_SATIRI_STRUCT;

    MUH_CLEAR_FIS_STRUCT        ( MUH_FIS );
    MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI );

    MUH_FIS_BILGILERINI_OKU ( MUH_FIS, P_FIS->muh_fis_id , USE_DB);

    double doviz_kuru = P_FIS->doviz_kuru;

    if( P_FIS->stk_irs_para_birimi_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
        doviz_kuru = 1.000;
    }


    MUH_FIS_SATIRI->aciklama             = P_FIS_SATIRI->aciklama;
    MUH_FIS_SATIRI->borc_tutari          = 0.00;
    MUH_FIS_SATIRI->alacak_tutari        = ROUND( P_FIS->irsaliye_tutari * doviz_kuru );
    MUH_FIS_SATIRI->fis_id               = P_FIS->muh_fis_id;
    MUH_FIS_SATIRI->hesap_id             = urun_muh_hesap_id;
    MUH_FIS_SATIRI->base_record_id       = P_FIS_SATIRI->base_record_id;
    MUH_FIS_SATIRI->program_id           = P_FIS->program_id;
    MUH_FIS_SATIRI->modul_id             = P_FIS->modul_id;

    // alacak
    MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI , USE_DB);


    MUH_FIS_SATIRI->hesap_id             = P_FIS->muh_hesap_id;
    MUH_FIS_SATIRI->borc_tutari          = ROUND( P_FIS->irsaliye_tutari * doviz_kuru );
    MUH_FIS_SATIRI->alacak_tutari        = 0.00;
    // borc
    int fis_satiri_id = MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI , USE_DB);


    delete MUH_FIS;
    delete MUH_FIS_SATIRI;

    return fis_satiri_id;
}

/***************************************************************************************
                        STK_CLEAR_ENT_FIS_STRUCT
***************************************************************************************/

void STK_CLEAR_ENT_FIS_STRUCT  ( STK_IRS_FIS_STRUCT  * P_FIS )
{
    P_FIS->modul_id                         =  0;
    P_FIS->program_id                       =  0;
    P_FIS->base_fis_id                      =  0;
    P_FIS->fis_no                           =  0;
    P_FIS->fis_tarihi.clear();
    P_FIS->aciklama.clear();
}

/***************************************************************************************
                        STK_SERI_NO_LIST_SIL
***************************************************************************************/

void STK_SERI_NO_LIST_SIL_ALIS( int p_irsaliye_satiri_id )
{
    SQL_QUERY query   ( DB );
    SQL_QUERY s_query ( DB );

    s_query.PREPARE_SELECT( "stk_satirdaki_seri_nolari", "seri_numarasi_id", "irsaliye_satiri_id = :irsaliye_satiri_id");
    s_query.SET_VALUE     ( ":irsaliye_satiri_id", p_irsaliye_satiri_id );

    if ( s_query.SELECT() EQ 0 ) {
        return;
    }

    while( s_query.NEXT() EQ true ) {

        int seri_numarasi_id = s_query.VALUE( 0 ).toInt();

        query.PREPARE_DELETE("stk_urunun_seri_nolari","seri_numarasi_id = :seri_numarasi_id");
        query.SET_VALUE(":seri_numarasi_id", seri_numarasi_id );
        query.DELETE();

        query.PREPARE_DELETE("stk_satirdaki_seri_nolari","irsaliye_satiri_id = :irsaliye_satiri_id "
                                 "AND seri_numarasi_id     = :seri_numarasi_id");
        query.SET_VALUE(":irsaliye_satiri_id" , p_irsaliye_satiri_id     );
        query.SET_VALUE(":seri_numarasi_id"   , seri_numarasi_id );
        query.DELETE();

    }
}

/***************************************************************************************
                        STK_SERI_NO_LIST_ALIS
***************************************************************************************/

void STK_SERI_NO_LIST_ALIS( int p_depo_id, int p_urun_id, int p_irsaliye_satiri_id, QStringList p_seri_no_list )
{

    int order_number= 0;
    for (int i = 0; i < p_seri_no_list.size(); i++) {

        SQL_QUERY sql_query ( DB );

        sql_query.PREPARE_INSERT("stk_urunun_seri_nolari","seri_numarasi_id", "depo_id, urun_id, seri_numarasi, depoda_mi, fatura_satiri_id ");

        sql_query.SET_VALUE(":depo_id"       , p_depo_id );
        sql_query.SET_VALUE(":urun_id"       , p_urun_id );
        sql_query.SET_VALUE(":seri_numarasi" , p_seri_no_list.at( i ) );
        sql_query.SET_VALUE(":depoda_mi"     , 1);
        sql_query.SET_VALUE(":fatura_satiri_id", 0);

        int seri_numarasi_id  = sql_query.INSERT();

        sql_query.PREPARE_INSERT("stk_satirdaki_seri_nolari","satir_seri_no_id", "irsaliye_satiri_id, seri_numarasi_id, order_number");

        sql_query.SET_VALUE(":irsaliye_satiri_id" , p_irsaliye_satiri_id );
        sql_query.SET_VALUE(":seri_numarasi_id"   , seri_numarasi_id  );
        sql_query.SET_VALUE(":order_number"       , order_number  );

        sql_query.INSERT();

        order_number += 1024;
    }

}

/***************************************************************************************
                        STK_SERI_NO_LIST_SATIS
***************************************************************************************/

void STK_SERI_NO_LIST_SATIS(int p_depo_id, int p_urun_id, int p_irsaliye_satiri_id, QStringList p_seri_no_list)
{
    SQL_QUERY sql_query ( DB );
    SQL_QUERY s_query ( DB );

    for (int i = 0; i < p_seri_no_list.size(); i++ ) {

        s_query.PREPARE_SELECT( "stk_urunun_seri_nolari", "seri_numarasi_id",
                                "depo_id = :depo_id     "
                                "AND urun_id = :urun_id "
                                "AND depoda_mi = :depoda_mi "
                                "AND seri_numarasi = :seri_numarasi");
        s_query.SET_VALUE     ( ":depo_id",         p_depo_id );
        s_query.SET_VALUE     ( ":urun_id",         p_urun_id );
        s_query.SET_VALUE     ( ":depoda_mi",       1 );
        s_query.SET_VALUE     ( ":seri_numarasi",   p_seri_no_list.at(i) );

        if( s_query.SELECT() EQ 0 ) {
            continue;
        }

        s_query.NEXT();

        int seri_numarasi_id = s_query.VALUE( 0 ).toInt();


        sql_query.PREPARE_INSERT("stk_satirdaki_seri_nolari" , "satir_seri_no_id" ,
                                 "irsaliye_satiri_id, seri_numarasi_id ");

        sql_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );
        sql_query.SET_VALUE(":seri_numarasi_id"  , seri_numarasi_id  );
        sql_query.INSERT();

        sql_query.PREPARE_UPDATE("stk_urunun_seri_nolari","seri_numarasi_id" ,"depoda_mi",
                                 "seri_numarasi_id = :seri_numarasi_id");

        sql_query.SET_VALUE(":depoda_mi"       , 0);
        sql_query.SET_VALUE(":seri_numarasi_id", seri_numarasi_id );

        sql_query.UPDATE();

    }
}

/***************************************************************************************
                        STK_SERI_NO_LIST_IPTAL
***************************************************************************************/

void STK_SERI_NO_LIST_IPTAL(int p_irsaliye_satiri_id, int p_depo_durumu )
{
    SQL_QUERY f_query ( DB );
    SQL_QUERY s_query ( DB );

    f_query.PREPARE_SELECT("stk_satirdaki_seri_nolari ","seri_numarasi_id",
                             "irsaliye_satiri_id = :irsaliye_satiri_id");
    f_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );
    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    while ( f_query.NEXT() EQ true ) {

        int seri_numarasi_id = f_query.VALUE(0).toInt();

        s_query.PREPARE_DELETE("stk_satirdaki_seri_nolari ",
                                 "seri_numarasi_id = :seri_numarasi_id "
                                 "AND irsaliye_satiri_id = :irsaliye_satiri_id ");

        s_query.SET_VALUE (":seri_numarasi_id"   , seri_numarasi_id     );
        s_query.SET_VALUE (":irsaliye_satiri_id" , p_irsaliye_satiri_id );

        s_query.DELETE();

        s_query.PREPARE_UPDATE("stk_urunun_seri_nolari ","seri_numarasi_id",
                                 "depoda_mi" , "seri_numarasi_id = :seri_numarasi_id");

        s_query.SET_VALUE( ":depoda_mi"       , p_depo_durumu );
        s_query.SET_VALUE( ":seri_numarasi_id", seri_numarasi_id );
        s_query.UPDATE();
    }
}

/***************************************************************************************
                        STK_SERI_NO_LIST_SATISTAN_IADE
***************************************************************************************/

void STK_SERI_NO_LIST_SATISTAN_IADE( int p_depo_id, int p_urun_id, int p_irsaliye_satiri_id, QStringList p_seri_no_list)
{
    SQL_QUERY sql_query ( DB );
    SQL_QUERY s_query ( DB );

    for (int i = 0; i < p_seri_no_list.size(); i++ ) {

        s_query.PREPARE_SELECT( "stk_urunun_seri_nolari", "seri_numarasi_id",
                                "depo_id = :depo_id     "
                                "AND urun_id = :urun_id "
                                "AND depoda_mi = :depoda_mi "
                                "AND seri_numarasi = :seri_numarasi");
        s_query.SET_VALUE     ( ":depo_id",         p_depo_id );
        s_query.SET_VALUE     ( ":urun_id",         p_urun_id );
        s_query.SET_VALUE     ( ":depoda_mi",       0 );
        s_query.SET_VALUE     ( ":seri_numarasi",   p_seri_no_list.at(i) );

        if( s_query.SELECT() EQ 0 ) {
            continue;
        }

        s_query.NEXT();

        int seri_numarasi_id = s_query.VALUE( 0 ).toInt();


        sql_query.PREPARE_INSERT("stk_satirdaki_seri_nolari" , "satir_seri_no_id" ,
                                 "irsaliye_satiri_id, seri_numarasi_id ");

        sql_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );
        sql_query.SET_VALUE(":seri_numarasi_id"  , seri_numarasi_id  );
        sql_query.INSERT();

        sql_query.PREPARE_UPDATE("stk_urunun_seri_nolari","seri_numarasi_id" ,"depoda_mi",
                                 "seri_numarasi_id = :seri_numarasi_id");

        sql_query.SET_VALUE(":depoda_mi"       , 1);
        sql_query.SET_VALUE(":seri_numarasi_id", seri_numarasi_id );

        sql_query.UPDATE();

    }
}

/***************************************************************************************
                        STK_FIS_SATIR_TUTARLARINI_HESAPLA
***************************************************************************************/

void STK_FIS_SATIR_TUTARLARINI_HESAPLA(STK_IRS_FIS_STRUCT *P_FIS, STK_IRS_FIS_SATIR_STRUCT *P_FIS_SATIRI)
{
    //! BU HESAPLAMA ISLEMLERI UI KULLAILMADIGI (ARAYUZ) ZAMAN FIS SATIRI EKLENMEDIN ONCE CAGRILMALIDIR.
    //! BENZER HESAPLAMA STOK FISI CHECK_LINE_VAR DA YAPILMAKTADIR.

    if ( P_FIS_SATIRI->urun_id EQ -1 ) {
        return;
    }

    STK_URUNLER_STRUCT * URUN = new STK_URUNLER_STRUCT;
    STK_URUNLER_TABLOSUNU_OKU( P_FIS_SATIRI->urun_id , URUN);

    if ( P_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        if ( P_FIS->fis_turu EQ ENUM_IRS_NORMAL_CIKIS ) {
            P_FIS_SATIRI->kdv_orani = URUN->satis_kdv_orani;
            P_FIS_SATIRI->otv_orani = URUN->satis_otv_orani;
        }
        else {
            P_FIS_SATIRI->kdv_orani = URUN->alis_kdv_orani;
            P_FIS_SATIRI->otv_orani = URUN->alis_otv_orani;
        }
    }
    else {
        if ( P_FIS->fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
            P_FIS_SATIRI->kdv_orani = URUN->satis_kdv_orani;
            P_FIS_SATIRI->otv_orani = URUN->satis_otv_orani;
        }
        else {
            P_FIS_SATIRI->kdv_orani = URUN->alis_kdv_orani;
            P_FIS_SATIRI->otv_orani = URUN->alis_otv_orani;
        }
    }

    switch ( P_FIS->irsaliye_alis_satis_turu ) {

        case     ENUM_ALIS_IRSALIYESI_FATURASI :
        default                                : {
            if ( P_FIS->stk_irs_para_birimi_id EQ URUN->urun_alis_doviz_id ) {
                P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati  = URUN->alis_fiyati;
                P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati = URUN->alis_fiyati;
            }
            else {
                double urun_fiyati = DVZ_PARA_BIRIMLERINI_DONUSTUR ( URUN->urun_alis_doviz_id, URUN->alis_fiyati, P_FIS->stk_irs_para_birimi_id , QDate::fromString( P_FIS->fis_tarihi, "yyyy.MM.dd"), ENUM_ALIS );
                P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati = urun_fiyati;
                P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati= urun_fiyati;
            }
            break;

        }
        case ENUM_SATIS_IRSALIYESI_FATURASI : {
            if ( P_FIS->stk_irs_para_birimi_id EQ URUN->urun_satis_doviz_id )  {
                P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati = URUN->satis_fiyati;
                P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati= URUN->satis_fiyati;
            }
            else {
                double urun_fiyati = DVZ_PARA_BIRIMLERINI_DONUSTUR ( URUN->urun_satis_doviz_id, URUN->satis_fiyati, P_FIS->stk_irs_para_birimi_id, QDate::fromString(P_FIS->fis_tarihi, "yyyy.MM.dd"), ENUM_SATIS );
                P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati = urun_fiyati;
                P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati = urun_fiyati;
            }
            break;
        }

    }

    P_FIS_SATIRI->satir_tutari = ROUND ( P_FIS_SATIRI->satirdaki_urun_miktari * P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati );

    double vergi_haric_tutar = P_FIS_SATIRI->satir_tutari;

    if ( P_FIS_SATIRI->kdv_dahil_mi EQ 1 ) {
        vergi_haric_tutar = ROUND ( P_FIS_SATIRI->satir_tutari / ( 1 + ( P_FIS_SATIRI->kdv_orani / 100 ) ) );
        vergi_haric_tutar = ROUND ( vergi_haric_tutar / ( 1 + ( P_FIS_SATIRI->otv_orani / 100 ) ) );
    }

    P_FIS_SATIRI->satir_tutari = vergi_haric_tutar;

    //**********


    int temel_para_birimi = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

    P_FIS_SATIRI->satir_vergi_haric_tutar = DVZ_PARA_BIRIMLERINI_DONUSTUR ( P_FIS->stk_irs_para_birimi_id, P_FIS_SATIRI->satir_tutari,
                                                               temel_para_birimi, QDate::fromString(P_FIS->fis_tarihi, "yyyy.MM.dd"));

    P_FIS_SATIRI->satir_otv_tutari      = ROUND ( P_FIS_SATIRI->satir_vergi_haric_tutar * ( P_FIS_SATIRI->otv_orani / 100 ) );
    P_FIS_SATIRI->satir_otv_dahil_tutar = ROUND ( P_FIS_SATIRI->satir_vergi_haric_tutar + P_FIS_SATIRI->satir_otv_tutari );
    P_FIS_SATIRI->satir_kdv_tutari      = ROUND ( P_FIS_SATIRI->satir_otv_dahil_tutar * ( P_FIS_SATIRI->kdv_orani / 100 ) );
    P_FIS_SATIRI->satir_kdv_dahil_tutar = ROUND ( P_FIS_SATIRI->satir_otv_dahil_tutar + P_FIS_SATIRI->satir_kdv_tutari );


    double toplu_iskonto_yuzdesi  = P_FIS->iskonto_yuzdesi;

    P_FIS_SATIRI->t_is_sonrasi_birim_fiyat       = P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) );
    P_FIS_SATIRI->t_is_sonrasi_satir_tutari      = P_FIS_SATIRI->satir_tutari * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) );
    P_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar = P_FIS_SATIRI->satir_vergi_haric_tutar * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) );
    P_FIS_SATIRI->t_is_sonrasi_otv_tutari        = P_FIS_SATIRI->satir_otv_tutari * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) );
    P_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar   = P_FIS_SATIRI->satir_otv_dahil_tutar * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) );
    P_FIS_SATIRI->t_is_sonrasi_kdv_tutari        = P_FIS_SATIRI->satir_kdv_tutari * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) );
    P_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar   = P_FIS_SATIRI->satir_kdv_dahil_tutar * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) );

}
