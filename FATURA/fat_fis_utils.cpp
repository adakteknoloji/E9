#include <QComboBox>
#include <QTableWidget>
#include "QCommaEdit.h"
#include "adak_sql.h"
#include "fat_fis_utils.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "stok_enum.h"
#include "stok_struct.h"
#include "cari_console_utils.h"
#include "sube_console_utils.h"
#include "fat_console_utils.h"
#include "fat_enum.h"
#include "muh_fis_utils.h"
#include "cari_fis_utils.h"
#include "cari_enum.h"
#include "irs_console_utils.h"
#include "muh_enum.h"
#include "muh_console_utils.h"
#include "dmr_struct.h"
#include "dmr_con_utils.h"
#include "irs_enum.h"
#include "stok_console_utils.h"
#include "sube_struct.h"
#include "stok_fis_utils.h"


extern      ADAK_SQL    *       DB;


/***************************************************************************************
                                FATURA_FISI_EKLE
***************************************************************************************/

int FATURA_FISI_EKLE ( FATURA_FISI_STRUCT * P_FIS )
{
    SQL_QUERY query ( DB );

    if (  P_FIS->fis_no EQ 0 ) {
       P_FIS->fis_no = FAT_SIRADAKI_FIS_NO_AL ( P_FIS->fatura_tarihi );
    }

    SUBE_ENTEGRASYON_HESAPLARI_STRUCT * SB_ENT_HESAPLARI = ( SUBE_ENTEGRASYON_HESAPLARI_STRUCT * ) SUBE_GET_ENTEGRASYON_HESAPLARI_STRUCT();

    P_FIS->yurtici_satislar_hesap_id          = SB_ENT_HESAPLARI->yurtici_sts_hsp_id;
    P_FIS->hizmet_urt_maliyeti_hesap_id       = SB_ENT_HESAPLARI->hizmet_uretim_maliyeti_hsp_id;
    P_FIS->satistan_iade_hesap_id             = SB_ENT_HESAPLARI->satistan_iade_hesap_id;


    QString kdv_oranlari_array                = PACK_DOUBLE_ARRAY ( P_FIS->KDV_ORANLARI_ARRAY,                 KDV_OTV_ORANLARI_SAYISI );
    QString kdv_hesaplari_id_array            = PACK_INT_ARRAY    ( P_FIS->KDV_HESAPLARI_ID_ARRAY,             KDV_OTV_ORANLARI_SAYISI );
    QString kdv_oran_gore_tutarlar_array      = PACK_DOUBLE_ARRAY ( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY,    KDV_OTV_ORANLARI_SAYISI );

    QString otv_oranlari_array                = PACK_DOUBLE_ARRAY ( P_FIS->OTV_ORANLARI_ARRAY,                 KDV_OTV_ORANLARI_SAYISI );
    QString otv_hesaplari_id_array            = PACK_INT_ARRAY    ( P_FIS->OTV_HESAPLARI_ID_ARRAY,             KDV_OTV_ORANLARI_SAYISI );
    QString otv_oran_gore_tutarlar_array      = PACK_DOUBLE_ARRAY ( P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY,    KDV_OTV_ORANLARI_SAYISI );
    QString tevkifatli_kdv_hsplar_id_array    = PACK_INT_ARRAY    ( P_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY,  KDV_OTV_ORANLARI_SAYISI );

    query.PREPARE_INSERT("fat_faturalar" , "fis_id" , "fis_no, fatura_tarihi, "
                         "fatura_turu, fatura_alis_satis_turu, belge_seri, belge_numarasi, acik_kapali_fatura, "
                         "cari_hesap_id, cari_hesap_ismi, aciklama, cari_hesap_adresi, vergi_dairesi, "
                         "vergi_numarasi, islem_saati, yazdirildi_mi, iptal_mi, "
                         "fatura_irsaliyelestirildi_mi, hatali_fatura_mi, iade_edilen_fatura_id, "
                         "iade_faturasi_mi, tevkifatli_fatura_mi, tevkifat_yuzdesi, tevkifatli_kdv_tutari, "
                         "yurtici_satislar_hesap_id, hizmet_urt_maliyeti_hesap_id,  "
                         "satistan_iade_hesap_id, "
                         "iskonto_yuzdesi, iskonto_tutari, cari_para_birimi_tutari, "
                         "kdv_oranlari_array, kdv_hesaplari_id_array, kdv_oran_gore_tutarlar_array, "
                         "otv_oranlari_array, otv_hesaplari_id_array, otv_oran_gore_tutarlar_array, "
                         "kdv_orani_sayisi, otv_orani_sayisi, tevkifatli_kdv_hsplar_id_array,"
                         "program_id, modul_id, base_fis_id, fatura_para_birimi_id, doviz_kuru, parite");


    query.SET_VALUE ( ":fis_no"                             , P_FIS->fis_no                             );
    query.SET_VALUE ( ":fatura_tarihi"                      , P_FIS->fatura_tarihi                      );
    query.SET_VALUE ( ":fatura_turu"                        , P_FIS->fatura_turu                        );
    query.SET_VALUE ( ":fatura_alis_satis_turu"             , P_FIS->fatura_alis_satis_turu             );
    query.SET_VALUE ( ":belge_seri"                         , P_FIS->belge_seri                         );
    query.SET_VALUE ( ":belge_numarasi"                     , P_FIS->belge_numarasi                     );
    query.SET_VALUE ( ":acik_kapali_fatura"                 , P_FIS->acik_kapali_fatura                 );
    query.SET_VALUE ( ":cari_hesap_id"                      , P_FIS->cari_hesap_id                      );
    query.SET_VALUE ( ":cari_hesap_ismi"                    , P_FIS->cari_hesap_ismi                    );
    query.SET_VALUE ( ":aciklama"                           , P_FIS->aciklama                           );
    query.SET_VALUE ( ":cari_hesap_adresi"                  , P_FIS->cari_hesap_adresi                  );
    query.SET_VALUE ( ":vergi_dairesi"                      , P_FIS->vergi_dairesi                      );
    query.SET_VALUE ( ":vergi_numarasi"                     , P_FIS->vergi_numarasi                     );
    query.SET_VALUE ( ":islem_saati"                        , P_FIS->islem_saati                        );
    query.SET_VALUE ( ":yazdirildi_mi"                      , 0                                         );
    query.SET_VALUE ( ":iptal_mi"                           , 0                                         );
    query.SET_VALUE ( ":fatura_irsaliyelestirildi_mi"       , P_FIS->fatura_irsaliyelestirildi_mi       );
    query.SET_VALUE ( ":hatali_fatura_mi"                   , 0                                         );
    query.SET_VALUE ( ":iade_edilen_fatura_id"              , P_FIS->iade_edilen_fatura_id              );
    query.SET_VALUE ( ":iade_faturasi_mi"                   , P_FIS->iade_faturasi_mi                   );
    query.SET_VALUE ( ":tevkifatli_fatura_mi"               , 0                                         );
    query.SET_VALUE ( ":tevkifat_yuzdesi"                   , 0                                         );
    query.SET_VALUE ( ":tevkifatli_kdv_tutari"              , 0                                         );
    query.SET_VALUE ( ":yurtici_satislar_hesap_id"          , P_FIS->yurtici_satislar_hesap_id          );
    query.SET_VALUE ( ":hizmet_urt_maliyeti_hesap_id"       , P_FIS->hizmet_urt_maliyeti_hesap_id       );
    query.SET_VALUE ( ":satistan_iade_hesap_id"             , P_FIS->satistan_iade_hesap_id             );
    query.SET_VALUE ( ":iskonto_yuzdesi"                    , P_FIS->iskonto_yuzdesi                    );
    query.SET_VALUE ( ":iskonto_tutari"                     , P_FIS->iskonto_tutari                     );
    query.SET_VALUE ( ":cari_para_birimi_tutari"            , P_FIS->cari_para_birimi_tutari            );
    query.SET_VALUE ( ":kdv_oranlari_array"                 , kdv_oranlari_array                        );
    query.SET_VALUE ( ":kdv_hesaplari_id_array"             , kdv_hesaplari_id_array                    );
    query.SET_VALUE ( ":kdv_oran_gore_tutarlar_array"       , kdv_oran_gore_tutarlar_array              );
    query.SET_VALUE ( ":otv_oranlari_array"                 , otv_oranlari_array                        );
    query.SET_VALUE ( ":otv_hesaplari_id_array"             , otv_hesaplari_id_array                    );
    query.SET_VALUE ( ":otv_oran_gore_tutarlar_array"       , otv_oran_gore_tutarlar_array              );
    query.SET_VALUE ( ":kdv_orani_sayisi"                   , P_FIS->kdv_orani_sayisi                   );
    query.SET_VALUE ( ":otv_orani_sayisi"                   , P_FIS->otv_orani_sayisi                   );
    query.SET_VALUE ( ":tevkifatli_kdv_hsplar_id_array"     , tevkifatli_kdv_hsplar_id_array            );
    query.SET_VALUE ( ":program_id"                         , P_FIS->program_id                         );
    query.SET_VALUE ( ":modul_id"                           , P_FIS->modul_id                           );
    query.SET_VALUE ( ":base_fis_id"                        , P_FIS->base_fis_id                        );
    query.SET_VALUE ( ":fatura_para_birimi_id"              , P_FIS->fatura_para_birimi_id              );
    query.SET_VALUE ( ":doviz_kuru"                         , P_FIS->doviz_kuru                         );
    query.SET_VALUE ( ":parite"                             , P_FIS->parite                             );

    int fis_id = query.INSERT();

    if ( P_FIS->muh_fis_id EQ 0 ) {

        MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        MUHASEBE_FISI->modul_id              = P_FIS->modul_id;
        MUHASEBE_FISI->program_id            = P_FIS->program_id;
        MUHASEBE_FISI->fis_tarihi            = P_FIS->fatura_tarihi;
        MUHASEBE_FISI->aciklama              = QObject::tr ( "Fiş FATURA MODÜLÜ tarafından üretilmiştir. Fiş No = %1 \n" ).arg ( QVariant ( P_FIS->fis_no ).toString() ) + P_FIS->aciklama;
        MUHASEBE_FISI->fis_turu              = ENUM_MAHSUP_FISI;
        MUHASEBE_FISI->base_fis_id           = fis_id;

        P_FIS->muh_fis_id = MUH_FIS_EKLE(MUHASEBE_FISI);

        delete MUHASEBE_FISI;

        query.PREPARE_UPDATE("fat_faturalar","fis_id", "muh_fis_id", "fis_id = :fis_id");

        query.SET_VALUE(":muh_fis_id" , P_FIS->muh_fis_id);
        query.SET_VALUE(":fis_id"     , fis_id);

        query.UPDATE();
    }

    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
        FAT_FATURA_IRSALIYELESTIR ( fis_id, P_FIS );
            break;
        default :
            break;
    }

    return fis_id;
}

/***************************************************************************************
                                FATURA_FISI_SIL
***************************************************************************************/

void FATURA_FISI_SIL ( FATURA_FISI_STRUCT *P_FIS, int p_fis_id )
{
    SQL_QUERY query ( DB );

    FATURA_FIS_SATIRI_STRUCT * FIS_SATIRI = new FATURA_FIS_SATIRI_STRUCT;

    query.PREPARE_SELECT( "fat_fatura_satirlari", "fis_satiri_id, satirdaki_urun_miktari", "fis_id = :fis_id");
    query.SET_VALUE     ( ":fis_id" , p_fis_id );

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {
            int fis_satiri_id = query.VALUE(0).toInt();

            FATURA_CLEAR_FIS_SATIRI_STRUCT( FIS_SATIRI );
            FATURA_SATIR_BILGILERINI_OKU( FIS_SATIRI, fis_satiri_id );

            FATURA_FIS_SATIRI_SIL( P_FIS, FIS_SATIRI, fis_satiri_id );
        }
    }

    //! IRSALIYE SILINIYOR.
    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS :
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS  :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI   :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI  :
            FAT_FATURA_IRSALIYESINI_SIL ( p_fis_id );
            break;
        default:
            break;
    }

    //! FATURA IRSALIYELI VEYA IRSALITE FATURALASMISSA
    //! 'fat_irs_baglantisi' TABLOSUNDA KAYDI OLMALIDIR.
    //! EGER KAYIT VAR ISE BU IRSALIYE DURUMU FATURALANMADI
    //! OLARAK GUNCELLENMELIDIR.

    query.PREPARE_SELECT ( "fat_irs_baglantisi", "irsaliye_id", "fatura_id = :fatura_id");
    query.SET_VALUE      (":fatura_id", p_fis_id );

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            int irsaliye_id = query.VALUE(0).toInt();
            STK_UPDATE_IRSALIYE_FATURALASTIRILDI_MI( irsaliye_id, 0 );
        }

        query.PREPARE_DELETE ( "fat_irs_baglantisi", "fatura_id = :fatura_id" );
        query.SET_VALUE      ( ":fatura_id", p_fis_id );
        query.DELETE();
    }

    //! [X] ENTEGRASYON FISLERI SILINIYOR.
    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_PROFORMA_SATIS_FATURASI :
            break;
        default  :
            CARI_FIS_STRUCT         * CARI_FIS          = new CARI_FIS_STRUCT;
            MUH_FIS_STRUCT          * MUH_FIS           = new MUH_FIS_STRUCT;

            CARI_FIS->modul_id      = FATURA_MODULU;
            CARI_FIS->program_id    = E9_PROGRAMI;
            CARI_FIS->base_fis_id   = p_fis_id;

            int cari_fis_id         = CARI_GET_ENT_FIS_ID ( E9_PROGRAMI, FATURA_MODULU, p_fis_id );

            CARI_CLEAR_FIS_STRUCT( CARI_FIS );

            CARI_FIS_BILGILERINI_OKU( CARI_FIS, cari_fis_id );

            CARI_FISI_SIL ( CARI_FIS, cari_fis_id );

            MUH_FIS->modul_id       = FATURA_MODULU;
            MUH_FIS->program_id     = E9_PROGRAMI;
            MUH_FIS->base_fis_id    = p_fis_id;

            MUH_FIS_SIL   ( MUH_FIS,  P_FIS->muh_fis_id );

            delete CARI_FIS;
            delete MUH_FIS;
            break;
    }
    //! [X]

    if ( P_FIS->yazdirildi_mi EQ 1 ) {
        query.PREPARE_UPDATE ( "fat_faturalar", "fis_id" ,"iptal_mi", "fis_id = :fis_id" );
        query.SET_VALUE      ( ":iptal_mi" , 1);
        query.SET_VALUE      ( ":fis_id"   , p_fis_id );
        query.UPDATE();
        P_FIS->iptal_mi = 1;
    }
    else {
        query.PREPARE_DELETE ( "fat_faturalar", "fis_id = :fis_id" );
        query.SET_VALUE      ( ":fis_id", p_fis_id );
        query.DELETE();
    }

}

/**************************************************************************************
                   FATURA_FIS_SATIRI_EKLE
***************************************************************************************/

int FATURA_FIS_SATIRI_EKLE ( FATURA_FISI_STRUCT * P_FIS, FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI , bool base_fis_satiri_eklenecek,
                            int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB,int demirbas_id)
{

    Q_UNUSED( demirbas_id );

    ADAK_SQL * USE_DB =  DB;

    if ( P_GELEN_DB NE NULL ) {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );

    //FATURA_FIS_BILGILERINI_OKU ( P_FIS, P_FIS_SATIRI->fis_id ,USE_DB);

    QString cari_hesap_kodu;

    if ( P_FIS->cari_hesap_id NE 0 ) {
        CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( P_FIS->cari_hesap_id, &cari_hesap_kodu, &P_FIS->cari_hesap_ismi );
     }

    // Fatura satirinda hizmet islemi varsa alinan ve verilen hizmet sayisi artirilmali ve azaltilmali.

    int fis_satiri_id = p_fis_satiri_id;

    if ( base_fis_satiri_eklenecek EQ true ) {

        query.PREPARE_INSERT("fat_fatura_satirlari" , "fis_satiri_id" , "fis_id, urun_id,"
                             "urun_adi, satirdaki_urun_miktari, irsaliyelenen_miktar, urun_birimi, "
                             "isk_oncesi_urun_birim_fiyati, isk_sonrasi_urun_birim_fiyati, satir_iskonto_yuzdesi, "
                             "satir_iskonto_tutari, satir_tutari, doviz_id, order_number, temel_birim_katsayisi, "
                             "otv_orani, satir_vergi_haric_tutar, satir_otv_tutari, satir_otv_dahil_tutar, kdv_dahil_mi,  "
                             "kdv_orani, satir_kdv_tutari, satir_kdv_dahil_tutar, fiyat_irsaliyeden_alindi, "
                             "hatali_satir_mi, iade_edilen_miktar, toplu_iskonto_yuzdesi, t_is_son_birim_fiyati, "
                             "t_is_son_satir_tutari, t_is_son_str_vergi_haric_tutar, t_is_son_satir_otv_tutari, "
                             "t_is_son_satir_otv_dahil_tutar, t_is_son_satir_kdv_tutari, "
                             "t_is_son_satir_kdv_dahil_tutar, program_id, modul_id, base_record_id");


        query.SET_VALUE ( ":fis_id"                             , P_FIS_SATIRI->fis_id                           );
        query.SET_VALUE ( ":urun_id"                            , P_FIS_SATIRI->urun_id                          );
        query.SET_VALUE ( ":urun_adi"                           , P_FIS_SATIRI->urun_adi                         );
        query.SET_VALUE ( ":satirdaki_urun_miktari"             , P_FIS_SATIRI->satirdaki_urun_miktari           );
        query.SET_VALUE ( ":irsaliyelenen_miktar"               , P_FIS_SATIRI->irsaliyelenen_miktar             );
        query.SET_VALUE ( ":urun_birimi"                        , P_FIS_SATIRI->urun_birimi                      );
        query.SET_VALUE ( ":isk_oncesi_urun_birim_fiyati"       , P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati     );
        query.SET_VALUE ( ":isk_sonrasi_urun_birim_fiyati"      , P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati    );
        query.SET_VALUE ( ":satir_iskonto_yuzdesi"              , P_FIS_SATIRI->satir_iskonto_yuzdesi            );
        query.SET_VALUE ( ":satir_iskonto_tutari"               , P_FIS_SATIRI->satir_iskonto_tutari             );
        query.SET_VALUE ( ":satir_tutari"                       , P_FIS_SATIRI->satir_tutari                     );
        query.SET_VALUE ( ":doviz_id"                           , P_FIS_SATIRI->doviz_id                         );
        query.SET_VALUE ( ":order_number"                       , P_FIS_SATIRI->order_number                     );
        query.SET_VALUE ( ":temel_birim_katsayisi"              , P_FIS_SATIRI->temel_birim_katsayisi            );
        query.SET_VALUE ( ":otv_orani"                          , P_FIS_SATIRI->otv_orani                        );
        query.SET_VALUE ( ":satir_vergi_haric_tutar"            , P_FIS_SATIRI->satir_vergi_haric_tutar          );
        query.SET_VALUE ( ":satir_otv_tutari"                   , P_FIS_SATIRI->satir_otv_tutari                 );
        query.SET_VALUE ( ":satir_otv_dahil_tutar"              , P_FIS_SATIRI->satir_otv_dahil_tutar            );
        query.SET_VALUE ( ":kdv_dahil_mi"                       , P_FIS_SATIRI->kdv_dahil_mi                     );
        query.SET_VALUE ( ":kdv_orani"                          , P_FIS_SATIRI->kdv_orani                        );
        query.SET_VALUE ( ":satir_kdv_tutari"                   , P_FIS_SATIRI->satir_kdv_tutari                 );
        query.SET_VALUE ( ":satir_kdv_dahil_tutar"              , P_FIS_SATIRI->satir_kdv_dahil_tutar            );
        query.SET_VALUE ( ":fiyat_irsaliyeden_alindi"           , 0                                              );
        query.SET_VALUE ( ":hatali_satir_mi"                    , P_FIS_SATIRI->hatali_satir_mi                  );
        query.SET_VALUE ( ":iade_edilen_miktar"                 , 0                                              );
        query.SET_VALUE ( ":toplu_iskonto_yuzdesi"              , P_FIS_SATIRI->toplu_iskonto_yuzdesi            );
        query.SET_VALUE ( ":t_is_son_birim_fiyati"              , P_FIS_SATIRI->t_is_son_birim_fiyati            );
        query.SET_VALUE ( ":t_is_son_satir_tutari"              , P_FIS_SATIRI->t_is_son_satir_tutari            );
        query.SET_VALUE ( ":t_is_son_str_vergi_haric_tutar"     , P_FIS_SATIRI->t_is_son_str_vergi_haric_tutar   );
        query.SET_VALUE ( ":t_is_son_satir_otv_tutari"          , P_FIS_SATIRI->t_is_son_satir_otv_tutari        );
        query.SET_VALUE ( ":t_is_son_satir_otv_dahil_tutar"     , P_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar   );
        query.SET_VALUE ( ":t_is_son_satir_kdv_tutari"          , P_FIS_SATIRI->t_is_son_satir_kdv_tutari        );
        query.SET_VALUE ( ":t_is_son_satir_kdv_dahil_tutar"     , P_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar   );
        query.SET_VALUE ( ":program_id"                         , P_FIS_SATIRI->program_id                       );
        query.SET_VALUE ( ":modul_id"                           , P_FIS_SATIRI->modul_id                         );
        query.SET_VALUE ( ":base_record_id"                     , P_FIS_SATIRI->base_record_id                   );

        fis_satiri_id   = query.INSERT();

        switch ( P_FIS->fatura_turu ) {

            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS          :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS         :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS           :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS  :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :
            {

                if ( P_FIS->fatura_turu EQ ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS OR
                     P_FIS->fatura_turu EQ ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS OR
                     P_FIS->fatura_turu EQ ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS OR
                     P_FIS->fatura_turu EQ ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS ) {

                    int irsaliye_id = STK_GET_IRS_ID(P_FIS_SATIRI->irsaliye_satiri_id);

                    query.PREPARE_SELECT( "fat_irs_baglantisi", "fatura_id",
                                          "fatura_id = :fatura_id AND irsaliye_id = :irsaliye_id ");
                    query.SET_VALUE     ( ":fatura_id", P_FIS_SATIRI->fis_id );
                    query.SET_VALUE     ( ":irsaliye_id", irsaliye_id );

                    if ( query.SELECT() EQ 0 ) {
                        FAT_IRS_BAGLANTISI_EKLE( P_FIS_SATIRI->fis_id, irsaliye_id );
                    }
                }

                query.PREPARE_INSERT("fat_irs_satir_baglantisi" , "kayit_id" , "fatura_satiri_id, irsaliye_satiri_id");

                query.SET_VALUE ( ":fatura_satiri_id"   , fis_satiri_id                     );
                query.SET_VALUE ( ":irsaliye_satiri_id" , P_FIS_SATIRI->irsaliye_satiri_id  );
                query.INSERT();

                IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE ( P_FIS_SATIRI->irsaliye_satiri_id, P_FIS_SATIRI->satirdaki_urun_miktari, ENUM_IRS_FATURALANAN_MIKTAR_ARTIR ,USE_DB);

                break;
            }
            default :
                break;
        }

        if ( P_FIS->iade_faturasi_mi EQ 1 ) {

            query.PREPARE_INSERT("fat_iade_satir_baglantisi", "kayit_id", "fat_iade_edilen_satir_id, fat_satir_id");
            query.SET_VALUE      ( ":fat_iade_edilen_satir_id", P_FIS_SATIRI->iade_edilen_fatura_satiri_id );
            query.SET_VALUE      ( ":fat_satir_id",             fis_satiri_id                              );
            query.INSERT();

            FAT_FATURA_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( P_FIS_SATIRI->iade_edilen_fatura_satiri_id, P_FIS_SATIRI->iade_edilen_miktar, \
                                                           ENUM_FAT_IADE_MIKTARI_ARTIR , USE_DB);
        }

        // Aşağıdaki işlemin bu şekilde gerçekleştirilme sebebi satır bazında iskonto uygulandığında
        // herbir satırdaki iskontoda ROUND sebebiyle kuruş kaybı yaşayacak olmamızdır.
        // Satır tutarlarını faturada tuttuğumuz iskonto öncesi tutarlara ekleyip iskonto uygulayarak
        // oluşan iskontolu değerleri faturaya yansıtıyoruz.
        // Böylece iskonto uygulamasını toplamda yaparak kuruş kaybı durumunu en aza indirgiyoruz.

        FATURA_KDV_ORANINA_GORE_TUTARLARI_GUNCELLE ( P_FIS, P_FIS_SATIRI->kdv_orani, P_FIS_SATIRI->t_is_son_satir_kdv_tutari, VERGI_ORANI_EKLE );
        FATURA_OTV_ORANINA_GORE_TUTARLARI_GUNCELLE ( P_FIS, P_FIS_SATIRI->otv_orani, P_FIS_SATIRI->t_is_son_satir_otv_tutari, VERGI_ORANI_EKLE );

        FATURA_SATIR_DOVIZ_TUTARLARINI_GUNCELLE    ( P_FIS_SATIRI->doviz_kodu, P_FIS_SATIRI->satir_tutari, DOVIZ_EKLE, P_FIS->DOVIZ_CINSINDEN_SATIR_TOPLAMLARI );

        P_FIS->brut_tutar                 = ROUND ( P_FIS->brut_tutar                 + P_FIS_SATIRI->satir_vergi_haric_tutar );
        P_FIS->isk_oncesi_otv_tutari      = ROUND ( P_FIS->isk_oncesi_otv_tutari      + P_FIS_SATIRI->satir_otv_tutari        );
        P_FIS->isk_oncesi_otv_dahil_tutar = ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar + P_FIS_SATIRI->satir_otv_dahil_tutar   );
        P_FIS->isk_oncesi_kdv_tutari      = ROUND ( P_FIS->isk_oncesi_kdv_tutari      + P_FIS_SATIRI->satir_kdv_tutari        );
        P_FIS->isk_oncesi_kdv_dahil_tutar = ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar + P_FIS_SATIRI->satir_kdv_dahil_tutar   );

        FATURA_ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA ( P_FIS );
    }

    if ( P_FIS->fatura_turu NE ENUM_FAT_PROFORMA_SATIS_FATURASI ) {

        if ( base_fis_satiri_eklenecek EQ true ) {
            if ( ( P_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI ) OR ( P_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) OR
                 ( P_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS) OR ( P_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS) ) {
                // irsaliye fis satiri ekle
                P_FIS_SATIRI->irsaliye_satiri_id = FAT_FATURA_IRSALIYE_STR_EKLE ( fis_satiri_id, P_FIS_SATIRI );
            }
        }

        QList <int> fis_no_list = FAT_FATURA_ENT_FISLERINI_OLUSTUR( P_FIS, P_FIS_SATIRI->fis_id, USE_DB );

        P_FIS->muh_fis_no  = fis_no_list.at(1);
        P_FIS->cari_fis_no = fis_no_list.at(0);

    }
    if ( base_fis_satiri_eklenecek EQ true ) {

        //! UPDATE ISLEMI
        FAT_FATURA_FIS_STRUCT_UPDATE_DB( P_FIS, P_FIS_SATIRI->fis_id );

        switch ( P_FIS->fatura_turu ) {

            case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS         :
            case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS          :
            case ENUM_FAT_IRSALIYELI_ALIS_FATURASI           :
            case ENUM_FAT_IRSALIYELI_SATIS_FATURASI          :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS  :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS :

                query.PREPARE_UPDATE ( "fat_faturalar","fis_id", "fatura_irsaliyelestirildi_mi",
                                       "fis_id = :fis_id");
                query.SET_VALUE(":fatura_irsaliyelestirildi_mi" , 1);
                query.SET_VALUE(":fis_id"                       , P_FIS_SATIRI->fis_id );
                query.UPDATE();
                break;

            default :
                break;
        }

        switch ( P_FIS->fatura_turu ) {
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS            :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS             :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS   :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS             :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS              :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS    : {
                STK_SERI_NOLARI_FATURA_SATIRI_ID_GUNCELLE( P_FIS_SATIRI->seri_no_list, P_FIS_SATIRI->urun_id, fis_satiri_id );
            break;
            }
            default:
                break;
        }
    }

    return fis_satiri_id;
}

/***************************************************************************************
                                FATURA_FIS_SATIRI_SIL
***************************************************************************************/

void FATURA_FIS_SATIRI_SIL ( FATURA_FISI_STRUCT * P_FIS, FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id )
{
    SQL_QUERY query ( DB );

    P_FIS->brut_tutar                   = ROUND ( P_FIS->brut_tutar - P_FIS_SATIRI->satir_vergi_haric_tutar                 );
    P_FIS->isk_oncesi_otv_tutari        = ROUND ( P_FIS->isk_oncesi_otv_tutari - P_FIS_SATIRI->satir_otv_tutari             );
    P_FIS->isk_oncesi_otv_dahil_tutar   = ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar- P_FIS_SATIRI->satir_otv_dahil_tutar    );
    P_FIS->isk_oncesi_kdv_tutari        = ROUND ( P_FIS->isk_oncesi_kdv_tutari - P_FIS_SATIRI->satir_kdv_tutari             );
    P_FIS->isk_oncesi_kdv_dahil_tutar   = ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar - P_FIS_SATIRI->satir_kdv_dahil_tutar   );

    FATURA_ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA ( P_FIS );

    QString doviz_kodu = DVZ_GET_DOVIZ_KODU( P_FIS_SATIRI->doviz_id );

    FATURA_SATIR_DOVIZ_TUTARLARINI_GUNCELLE     ( doviz_kodu, P_FIS_SATIRI->satir_tutari, DOVIZ_SIL, P_FIS->DOVIZ_CINSINDEN_SATIR_TOPLAMLARI  );
    FATURA_KDV_ORANINA_GORE_TUTARLARI_GUNCELLE  ( P_FIS, P_FIS_SATIRI->kdv_orani, P_FIS_SATIRI->t_is_son_satir_kdv_tutari, VERGI_ORANI_SIL    );
    FATURA_OTV_ORANINA_GORE_TUTARLARI_GUNCELLE  ( P_FIS, P_FIS_SATIRI->otv_orani, P_FIS_SATIRI->t_is_son_satir_otv_tutari, VERGI_ORANI_SIL    );

    CARI_FIS_STRUCT         * CARI_FIS          = new CARI_FIS_STRUCT;
    MUH_FIS_STRUCT          * MUH_FIS           = new MUH_FIS_STRUCT;

    query.PREPARE_SELECT ( "fat_fatura_satirlari","fis_satiri_id","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id", P_FIS_SATIRI->fis_id );

    int faturadaki_satir_sayisi = query.SELECT();

    //! DIKKAT SATIR SAYISI OKUNDUKTAN SONRA SILINMESI GEREKIR.
    //! FATURA SATIRI SILINIYOR.
    query.PREPARE_DELETE ( "fat_fatura_satirlari", "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );
    query.DELETE();

    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_PROFORMA_SATIS_FATURASI :
            break;
        default  :
            if (( P_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI ) OR ( P_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) OR
                ( P_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS) OR ( P_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS) ) {
                FAT_FATURA_IRSALIYE_STR_SIL ( p_fis_satiri_id  );
            }

            if ( faturadaki_satir_sayisi EQ 1 ) {
                CARI_FIS->modul_id      = FATURA_MODULU;
                CARI_FIS->program_id    = E9_PROGRAMI;
                CARI_FIS->base_fis_id   = P_FIS_SATIRI->fis_id;
                CARI_FIS->muh_fis_id    = P_FIS->muh_fis_id;

                MUH_FIS->modul_id       = FATURA_MODULU;
                MUH_FIS->program_id     = E9_PROGRAMI;
                MUH_FIS->base_fis_id    = P_FIS_SATIRI->fis_id;

                int cari_fis_id = CARI_GET_ENT_FIS_ID ( E9_PROGRAMI, FATURA_MODULU, P_FIS_SATIRI->fis_id );

                CARI_FISI_SIL ( CARI_FIS, cari_fis_id );

                MUH_FIS_TUM_SATIRLARINI_SIL(MUH_FIS , P_FIS->muh_fis_id );

                break;
            }

            QList<int> fis_no_list = FAT_FATURA_ENT_FISLERINI_OLUSTUR( P_FIS, P_FIS_SATIRI->fis_id );

            P_FIS->muh_fis_no  = fis_no_list.at(1);
            P_FIS->cari_fis_no = fis_no_list.at(0);
            break;
    }

    delete CARI_FIS;
    delete MUH_FIS;

    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI                  : // sadece irsaliye satirlarini fat mik guncellemel icin
        case ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI                 : // sadece irsaliye satirlarini fat mik guncellemel icin
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS         :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS  :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :

            query.PREPARE_SELECT ( "fat_irs_satir_baglantisi ", "irsaliye_satiri_id ", "fatura_satiri_id = :fatura_satiri_id");
            query.SET_VALUE(":fatura_satiri_id", p_fis_satiri_id );
            if ( query.SELECT() > 0 ) {

                query.NEXT();
                int irsaliye_satiri_id = query.VALUE(0).toInt();

                query.PREPARE_DELETE ( "fat_irs_satir_baglantisi ", "fatura_satiri_id = :fatura_satiri_id");
                query.SET_VALUE  ( ":fatura_satiri_id", p_fis_satiri_id );
                query.DELETE();

                IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE ( irsaliye_satiri_id, P_FIS_SATIRI->satirdaki_urun_miktari, ENUM_IRS_FATURALANAN_MIKTAR_AZALT );

                //! Seri nolu urunler fatura durumu guncelleniyor
                if( STK_GET_URUN_TAKIP_SEKLI( P_FIS_SATIRI->urun_id ) EQ ENUM_SERI_NO_ILE_TAKIP ) {
                    QString seri_numaralari = STK_GET_SERI_NO_BILGILERI( irsaliye_satiri_id , ENUM_URUN_HER_IKISI, p_fis_satiri_id  );

                    QStringList seri_no_list = seri_numaralari.split( ",");

                    STK_SERI_NOLARI_FATURA_SATIRI_ID_GUNCELLE( seri_no_list, P_FIS_SATIRI->urun_id, 0 );
                }
            }
            break;
        default :
            break;
    }

    if ( P_FIS->iade_faturasi_mi EQ 1 ) {
        query.PREPARE_SELECT ( "fat_iade_satir_baglantisi ","fat_iade_edilen_satir_id", "fat_satir_id = :fat_satir_id");
        query.SET_VALUE      ( ":fat_satir_id", p_fis_satiri_id );
        if ( query.SELECT() > 0 ) {
            query.NEXT();
            int iade_edilen_fatura_satiri_id = query.VALUE(0).toInt();
            query.PREPARE_DELETE ( "fat_iade_satir_baglantisi", "fat_satir_id = :fat_satir_id");
            query.SET_VALUE      ( ":fat_satir_id", p_fis_satiri_id );
            query.DELETE();

            FAT_FATURA_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( iade_edilen_fatura_satiri_id, P_FIS_SATIRI->satirdaki_urun_miktari, ENUM_FAT_IADE_MIKTARI_AZALT );
        }
    }

    FAT_FATURA_FIS_STRUCT_UPDATE_DB( P_FIS, P_FIS_SATIRI->fis_id );

}

/**************************************************************************************
                   FATURA_FISI_GUNCELLE
***************************************************************************************/

void FATURA_FISI_GUNCELLE ( FATURA_FISI_STRUCT * P_FIS, int p_fis_id )
{
    bool satirlardaki_stoklar_degistirilecek = false;
    bool ent_fisleri_silinecek               = false;

    SQL_QUERY query  ( DB );

    query.PREPARE_SELECT ( "fat_faturalar","fatura_tarihi, cari_hesap_id, iskonto_yuzdesi, "
                           "acik_kapali_fatura,belge_seri,belge_numarasi,aciklama" ,"fis_id = :fis_id");

    query.SET_VALUE(":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    QDate   old_fatura_tarihi       = QDate::fromString ( query.VALUE("fatura_tarihi").toString(), "yyyy.MM.dd" );
    int     old_cari_hesap_id       = query.VALUE ( "cari_hesap_id"      ).toInt();
    double  old_iskonto_yuzdesi     = query.VALUE ( "iskonto_yuzdesi"    ).toDouble();
    double  new_iskonto_yuzdesi     = P_FIS->iskonto_yuzdesi;
    int     acik_kapali_fatura      = query.VALUE("acik_kapali_fatura").toInt();
    QString old_belge_seri          = query.VALUE("belge_seri").toString();
    QString old_belge_no            = query.VALUE("belge_numarasi").toString();
    QString old_aciklama            = query.VALUE("aciklama").toString();


    bool    iskonto_yuzdesi_degisti = false;
    if ( old_iskonto_yuzdesi NE new_iskonto_yuzdesi ) {
        iskonto_yuzdesi_degisti = true;
    }

    query.PREPARE_SELECT ( "fat_irs_baglantisi", "irsaliye_id", "fatura_id = :fatura_id" );
    query.SET_VALUE      ( ":fatura_id" , p_fis_id );

    int irsaliye_id  = 0;
    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        irsaliye_id = query.VALUE(0).toInt();
    }

    STK_IRS_FIS_STRUCT * IRS_FISI = new STK_IRS_FIS_STRUCT;

    STK_IRS_CLEAR_FIS_STRUCT(IRS_FISI);

    STK_IRS_FIS_BILGILERINI_OKU(irsaliye_id , IRS_FISI);

    QDate   new_fatura_tarihi     =  QDate::fromString ( P_FIS->fatura_tarihi, "yyyy.MM.dd" );
    int     new_cari_hesap_id     =  P_FIS->cari_hesap_id;

    if ( old_fatura_tarihi NE new_fatura_tarihi ) {
        satirlardaki_stoklar_degistirilecek = true;
        ent_fisleri_silinecek               = true;
    }

    if ( old_cari_hesap_id NE new_cari_hesap_id ) {
        ent_fisleri_silinecek = true;
    }

    if ( old_iskonto_yuzdesi NE new_iskonto_yuzdesi ) {
        ent_fisleri_silinecek = true;
    }

    if ( acik_kapali_fatura NE P_FIS->acik_kapali_fatura ) {
        ent_fisleri_silinecek = true;
    }
    if ( old_belge_no NE P_FIS->belge_numarasi OR old_belge_seri NE P_FIS->belge_seri ) {
        ent_fisleri_silinecek = true;
    }
    if ( IRS_FISI->belge_seri NE P_FIS->irs_belge_seri OR IRS_FISI->belge_numarasi NE P_FIS->irs_belge_numarasi ) {
        IRS_UPDATE_BELGE_SERI_VE_NUMARASI( irsaliye_id, P_FIS->irs_belge_seri, P_FIS->irs_belge_numarasi );
    }
    if ( old_aciklama NE P_FIS->aciklama) {
        ent_fisleri_silinecek = true;
    }

    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_PROFORMA_SATIS_FATURASI :
            break;
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI:
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
            if ( satirlardaki_stoklar_degistirilecek EQ true ) {

                STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_GIRIS_CIKISLARINI_DUZENLE ( irsaliye_id, old_fatura_tarihi, new_fatura_tarihi,
                                                                                       P_FIS->depo_id, P_FIS->fatura_alis_satis_turu);
            }
            break;
        default :
        break;
    }

    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
            IRS_IRSALIYEDE_TOPLU_ISKONTO_SONRASI_TUTARLARI_GUNCELE ( P_FIS, p_fis_id );
            break;
        default:
            break;
    }

    /*
      DIKKAT :

      FISTE ISKONTO VAR ISE FIS SATIRLARINDA BULUNAN T_ISK_..... DEGISLENLERININ DEGERLERININ GUNCELLENMESI GEREKIR.
      ORDAKIDEGERLER SATIRDAKI ISKONTOYA AIT DEGIL FIS 'E AIT. SATIRDAKI ISKONTO DIREK SATIR OLARAK ISKONTO SONRSI FIYATI
      VE DOLAYISI ILE SATIR TUTARINI ETKILER. ONUN DISINDA SATIR ISKONTOSU BIR ISLEVI YOKTUR. AMA FIS 'TEKI ISKONTO MUHASEBEYE
      ENTEGRE OLUR. HER SATIRDAKI KDV VE OTV FARKLI OLABILECEGI ICIN HER SATIRA AYRI AYRI ISKONTO UYGULANIR
     */

    if ( iskonto_yuzdesi_degisti EQ true ) {

        query.PREPARE_SELECT ( "fat_fatura_satirlari","t_is_son_satir_kdv_tutari, t_is_son_satir_otv_tutari, kdv_orani, otv_orani, fis_satiri_id", "fis_id = :fis_id" );
        query.SET_VALUE      ( ":fis_id", p_fis_id );
        if ( query.SELECT() > 0 ) {
            while ( query.NEXT() EQ true ) {

                int fis_satiri_id = query.VALUE( "fis_satiri_id" ).toInt();

                FATURA_FIS_SATIRI_STRUCT FATURA_SATIRI;

                FATURA_CLEAR_FIS_SATIRI_STRUCT( &FATURA_SATIRI );
                FATURA_SATIR_BILGILERINI_OKU( &FATURA_SATIRI, fis_satiri_id );


                FATURA_KDV_ORANINA_GORE_TUTARLARI_GUNCELLE ( P_FIS, query.VALUE ( "kdv_orani" ).toDouble(), FATURA_SATIRI.t_is_son_satir_kdv_tutari, VERGI_ORANI_SIL );
                FATURA_OTV_ORANINA_GORE_TUTARLARI_GUNCELLE ( P_FIS, query.VALUE ( "otv_orani" ).toDouble(), FATURA_SATIRI.t_is_son_satir_otv_tutari, VERGI_ORANI_SIL );


                double toplu_iskonto_yuzdesi  = P_FIS->iskonto_yuzdesi;

                FATURA_SATIRI.toplu_iskonto_yuzdesi             = toplu_iskonto_yuzdesi ;
                FATURA_SATIRI.t_is_son_birim_fiyati             = FATURA_SATIRI.t_is_son_birim_fiyati * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 )  );
                FATURA_SATIRI.t_is_son_satir_tutari             = FATURA_SATIRI.satir_tutari * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 )  );
                FATURA_SATIRI.t_is_son_str_vergi_haric_tutar    = FATURA_SATIRI.satir_vergi_haric_tutar * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 )  );
                FATURA_SATIRI.t_is_son_satir_otv_tutari         = FATURA_SATIRI.satir_otv_tutari * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 )  );
                FATURA_SATIRI.t_is_son_satir_otv_dahil_tutar    = FATURA_SATIRI.satir_otv_dahil_tutar * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 )  );
                FATURA_SATIRI.t_is_son_satir_kdv_tutari         = FATURA_SATIRI.satir_kdv_tutari * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 )  );
                FATURA_SATIRI.t_is_son_satir_kdv_dahil_tutar    = FATURA_SATIRI.satir_kdv_dahil_tutar * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 )  );

                SQL_QUERY s_query( DB );
                s_query.PREPARE_UPDATE ( "fat_fatura_satirlari ","fis_satiri_id",
                                       "toplu_iskonto_yuzdesi ,               "
                                       "t_is_son_birim_fiyati ,               "
                                       "t_is_son_satir_tutari,                "
                                       "t_is_son_str_vergi_haric_tutar ,      "
                                       "t_is_son_satir_otv_tutari    ,        "
                                       "t_is_son_satir_otv_dahil_tutar ,      "
                                       "t_is_son_satir_kdv_tutari     ,       "
                                       "t_is_son_satir_kdv_dahil_tutar        ",
                                       "fis_satiri_id                 = :fis_satiri_id " );

                s_query.SET_VALUE ( ":toplu_iskonto_yuzdesi",             FATURA_SATIRI.toplu_iskonto_yuzdesi          );
                s_query.SET_VALUE ( ":t_is_son_birim_fiyati",             FATURA_SATIRI.t_is_son_birim_fiyati      );
                s_query.SET_VALUE ( ":t_is_son_satir_tutari",             FATURA_SATIRI.t_is_son_satir_tutari     );
                s_query.SET_VALUE ( ":t_is_son_str_vergi_haric_tutar",    FATURA_SATIRI.t_is_son_str_vergi_haric_tutar);
                s_query.SET_VALUE ( ":t_is_son_satir_otv_tutari",         FATURA_SATIRI.t_is_son_satir_otv_tutari      );
                s_query.SET_VALUE ( ":t_is_son_satir_otv_dahil_tutar",    FATURA_SATIRI.t_is_son_satir_otv_dahil_tutar   );
                s_query.SET_VALUE ( ":t_is_son_satir_kdv_tutari",         FATURA_SATIRI.t_is_son_satir_kdv_tutari       );
                s_query.SET_VALUE ( ":t_is_son_satir_kdv_dahil_tutar",    FATURA_SATIRI.t_is_son_satir_kdv_dahil_tutar  );
                s_query.SET_VALUE ( ":fis_satiri_id",                     fis_satiri_id );

                s_query.UPDATE();

                FATURA_KDV_ORANINA_GORE_TUTARLARI_GUNCELLE ( P_FIS, FATURA_SATIRI.kdv_orani, FATURA_SATIRI.t_is_son_satir_kdv_tutari, VERGI_ORANI_EKLE );
                FATURA_OTV_ORANINA_GORE_TUTARLARI_GUNCELLE ( P_FIS, FATURA_SATIRI.otv_orani, FATURA_SATIRI.t_is_son_satir_otv_tutari, VERGI_ORANI_EKLE );


                switch ( P_FIS->fatura_turu ) {

                    case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
                    case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
                    case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
                    case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
                        IRS_SATIRINDA_TOPLU_ISKONTO_SONRASI_TUTARLARI_GUNCELE ( fis_satiri_id );
                        break;

                    default:
                        break;
                }
            }
        }
    }

    //! FATURA GUNCELLENIYOR.
    FAT_FATURA_FIS_STRUCT_UPDATE_DB( P_FIS, p_fis_id );

    if ( ent_fisleri_silinecek EQ true ) {

        switch ( P_FIS->fatura_turu ) {
            case ENUM_FAT_PROFORMA_SATIS_FATURASI :
                break;
            default  :
                QList<int> fis_no_list = FAT_FATURA_ENT_FISLERINI_OLUSTUR( P_FIS, p_fis_id );

                P_FIS->muh_fis_no   = fis_no_list.at(1);
                P_FIS->cari_fis_no  = fis_no_list.at(0);
                break;
        }
    }
}


/**************************************************************************************
                   FATURA_FIS_SATIRI_GUNCELLE
***************************************************************************************/

void FATURA_FIS_SATIRI_GUNCELLE ( FATURA_FISI_STRUCT * P_FIS, FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id )
{

    SQL_QUERY query ( DB );

    FATURA_FISI_STRUCT * OLD_FIS = new FATURA_FISI_STRUCT;
    FATURA_FIS_SATIRI_STRUCT * OLD_FIS_SATIRI = new FATURA_FIS_SATIRI_STRUCT;

    OLD_FIS->KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    OLD_FIS->KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    OLD_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];
    OLD_FIS->OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    OLD_FIS->OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    OLD_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];
    OLD_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];

    FATURA_CLEAR_FIS_STRUCT( OLD_FIS );
    FATURA_CLEAR_FIS_SATIRI_STRUCT( OLD_FIS_SATIRI );

    FATURA_SATIR_BILGILERINI_OKU( OLD_FIS_SATIRI, p_fis_satiri_id );
    FATURA_FIS_BILGILERINI_OKU( OLD_FIS, OLD_FIS_SATIRI->fis_id );


    P_FIS->fis_no             = OLD_FIS->fis_no;
    P_FIS->fatura_tarihi      = OLD_FIS->fatura_tarihi;
    P_FIS->belge_seri         = OLD_FIS->belge_seri;
    P_FIS->belge_numarasi     = OLD_FIS->belge_numarasi;
    P_FIS->acik_kapali_fatura = OLD_FIS->acik_kapali_fatura;
    P_FIS->cari_hesap_id      = OLD_FIS->cari_hesap_id;

    int         hatali_satir_mi                     = 0;

    if ( P_FIS_SATIRI->urun_id > 0 ) {
        if ( P_FIS_SATIRI->satirdaki_urun_miktari EQ 0 ) {
            hatali_satir_mi = 1;
        }
    }

    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_PROFORMA_SATIS_FATURASI :
            break;
        default  :
            if ( ( OLD_FIS_SATIRI->satirdaki_urun_miktari NE P_FIS_SATIRI->satirdaki_urun_miktari ) OR
                 ( OLD_FIS_SATIRI->urun_birimi NE P_FIS_SATIRI->urun_birimi ) OR
                 ( OLD_FIS_SATIRI->satir_kdv_tutari NE P_FIS_SATIRI->satir_kdv_tutari ) OR
                 ( OLD_FIS_SATIRI->satir_otv_tutari NE P_FIS_SATIRI->satir_otv_tutari ) )  {
                // IN IF
                if ( ( P_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI ) OR ( P_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) OR
                     ( P_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS) OR ( P_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS) ) {

                      FAT_FATURA_IRSALIYE_STR_UPDATE( P_FIS_SATIRI, p_fis_satiri_id );
                }
                //END IF
            }
            if ( P_FIS->iade_faturasi_mi EQ 1 ) {

                query.PREPARE_SELECT ( "fat_iade_satir_baglantisi ","fat_iade_edilen_satir_id",
                                       "fat_satir_id = :fat_satir_id");
                query.SET_VALUE      ( ":fat_satir_id", p_fis_satiri_id );
                if ( query.SELECT() > 0 ) {
                    query.NEXT();
                    int iade_edilen_fatura_satiri_id = query.VALUE(0).toInt();
                    if ( iade_edilen_fatura_satiri_id NE P_FIS_SATIRI->iade_edilen_fatura_satiri_id )
                    query.PREPARE_DELETE ( "fat_iade_satir_baglantisi ",
                                           "fat_satir_id = :fat_satir_id");
                    query.SET_VALUE      ( ":fat_satir_id", p_fis_satiri_id );
                    query.DELETE();

                    FAT_FATURA_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( iade_edilen_fatura_satiri_id, OLD_FIS_SATIRI->satirdaki_urun_miktari, ENUM_FAT_IADE_MIKTARI_AZALT );
                }
            }
            break;
    }

    OLD_FIS->brut_tutar                     =   ROUND ( OLD_FIS->brut_tutar                    -  OLD_FIS_SATIRI->satir_vergi_haric_tutar  );
    OLD_FIS->isk_oncesi_otv_tutari          =   ROUND ( OLD_FIS->isk_oncesi_otv_tutari         -  OLD_FIS_SATIRI->satir_otv_tutari         );
    OLD_FIS->isk_oncesi_otv_dahil_tutar     =   ROUND ( OLD_FIS->isk_oncesi_otv_dahil_tutar    -  OLD_FIS_SATIRI->satir_otv_dahil_tutar    );
    OLD_FIS->isk_oncesi_kdv_tutari          =   ROUND ( OLD_FIS->isk_oncesi_kdv_tutari         -  OLD_FIS_SATIRI->satir_kdv_tutari         );
    OLD_FIS->isk_oncesi_kdv_dahil_tutar     =   ROUND ( OLD_FIS->isk_oncesi_kdv_dahil_tutar    -  OLD_FIS_SATIRI->satir_kdv_dahil_tutar    );


    FATURA_SATIR_DOVIZ_TUTARLARINI_GUNCELLE( DVZ_GET_DOVIZ_KODU ( OLD_FIS_SATIRI->doviz_id ), OLD_FIS_SATIRI->satir_tutari, DOVIZ_SIL, P_FIS->DOVIZ_CINSINDEN_SATIR_TOPLAMLARI );
    FATURA_KDV_ORANINA_GORE_TUTARLARI_GUNCELLE( P_FIS, OLD_FIS_SATIRI->kdv_orani, OLD_FIS_SATIRI->t_is_son_satir_kdv_tutari, VERGI_ORANI_SIL );
    FATURA_OTV_ORANINA_GORE_TUTARLARI_GUNCELLE( P_FIS, OLD_FIS_SATIRI->otv_orani, OLD_FIS_SATIRI->t_is_son_satir_otv_tutari, VERGI_ORANI_SIL );

    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS :

            query.PREPARE_SELECT ( "fat_irs_satir_baglantisi ","irsaliye_satiri_id",
                                   "fatura_satiri_id = :fatura_satiri_id");
            query.SET_VALUE      ( ":fatura_satiri_id", p_fis_satiri_id );
            if ( query.SELECT() > 0 ) {

                query.NEXT();
                int old_irsaliye_satiri_id = query.VALUE(0).toInt();

                if ( ( old_irsaliye_satiri_id NE P_FIS_SATIRI->irsaliye_satiri_id ) OR ( OLD_FIS_SATIRI->satirdaki_urun_miktari NE P_FIS_SATIRI->satirdaki_urun_miktari ) ) {
                    if ( old_irsaliye_satiri_id NE P_FIS_SATIRI->irsaliye_satiri_id ) {
                        query.PREPARE_DELETE ( "fat_irs_satir_baglantisi ",
                                               "fatura_satiri_id = :fatura_satiri_id");
                        query.SET_VALUE      ( ":fatura_satiri_id", p_fis_satiri_id );
                        query.DELETE();

                    }
                    IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE ( old_irsaliye_satiri_id, OLD_FIS_SATIRI->satirdaki_urun_miktari, ENUM_IRS_FATURALANAN_MIKTAR_AZALT );
                }
            }
            break;

        default :
            break;
    }


    query.PREPARE_UPDATE ( "fat_fatura_satirlari ","fis_satiri_id" ,
                           "urun_id                                 ,"
                           "urun_adi                                ,"
                           "satirdaki_urun_miktari                  ,"
                           "urun_birimi                             ,"
                           "isk_oncesi_urun_birim_fiyati            ,"
                           "isk_sonrasi_urun_birim_fiyati           ,"
                           "satir_iskonto_yuzdesi                   ,"
                           "satir_iskonto_tutari                    ,"
                           "satir_tutari                            ,"
                           "doviz_id                                ,"
                           "satir_kdv_tutari                        ,"
                           "satir_kdv_dahil_tutar                   ,"
                           "kdv_orani                               ,"
                           "kdv_dahil_mi                            ,"
                           "satir_vergi_haric_tutar                 ,"
                           "otv_orani                               ,"
                           "satir_otv_tutari                        ,"
                           "satir_otv_dahil_tutar                   ,"
                           "temel_birim_katsayisi                   ,"
                           "hatali_satir_mi                         ,"
                           "toplu_iskonto_yuzdesi                   ,"
                           "t_is_son_birim_fiyati                   ,"
                           "t_is_son_satir_tutari                   ,"
                           "t_is_son_str_vergi_haric_tutar          ,"
                           "t_is_son_satir_otv_tutari               ,"
                           "t_is_son_satir_otv_dahil_tutar          ,"
                           "t_is_son_satir_kdv_tutari               ,"
                           "t_is_son_satir_kdv_dahil_tutar          ,"
                           "base_record_id                           ",
                           "fis_satiri_id                 = :fis_satiri_id " );

    query.SET_VALUE ( ":urun_id"                            , P_FIS_SATIRI->urun_id                          );
    query.SET_VALUE ( ":urun_adi"                           , P_FIS_SATIRI->urun_adi                         );
    query.SET_VALUE ( ":satirdaki_urun_miktari"             , P_FIS_SATIRI->satirdaki_urun_miktari           );
    query.SET_VALUE ( ":urun_birimi"                        , P_FIS_SATIRI->urun_birimi                      );
    query.SET_VALUE ( ":isk_oncesi_urun_birim_fiyati"       , P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati     );
    query.SET_VALUE ( ":isk_sonrasi_urun_birim_fiyati"      , P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati    );
    query.SET_VALUE ( ":satir_iskonto_yuzdesi"              , P_FIS_SATIRI->satir_iskonto_yuzdesi            );
    query.SET_VALUE ( ":satir_iskonto_tutari"               , P_FIS_SATIRI->satir_iskonto_tutari             );
    query.SET_VALUE ( ":satir_tutari"                       , P_FIS_SATIRI->satir_tutari                     );
    query.SET_VALUE ( ":doviz_id"                           , P_FIS_SATIRI->doviz_id                         );
    query.SET_VALUE ( ":satir_kdv_tutari"                   , P_FIS_SATIRI->satir_kdv_tutari                 );
    query.SET_VALUE ( ":satir_kdv_dahil_tutar"              , P_FIS_SATIRI->satir_kdv_dahil_tutar            );
    query.SET_VALUE ( ":kdv_orani"                          , P_FIS_SATIRI->kdv_orani                        );
    query.SET_VALUE ( ":kdv_dahil_mi"                       , P_FIS_SATIRI->kdv_dahil_mi                     );
    query.SET_VALUE ( ":satir_vergi_haric_tutar"            , P_FIS_SATIRI->satir_vergi_haric_tutar          );
    query.SET_VALUE ( ":otv_orani"                          , P_FIS_SATIRI->otv_orani                        );
    query.SET_VALUE ( ":satir_otv_tutari"                   , P_FIS_SATIRI->satir_otv_tutari                 );
    query.SET_VALUE ( ":satir_otv_dahil_tutar"              , P_FIS_SATIRI->satir_otv_dahil_tutar            );
    query.SET_VALUE ( ":temel_birim_katsayisi"              , P_FIS_SATIRI->temel_birim_katsayisi            );
    query.SET_VALUE ( ":hatali_satir_mi"                    , hatali_satir_mi                                );
    query.SET_VALUE ( ":toplu_iskonto_yuzdesi"              , P_FIS_SATIRI->toplu_iskonto_yuzdesi            );
    query.SET_VALUE ( ":t_is_son_birim_fiyati"              , P_FIS_SATIRI->t_is_son_birim_fiyati            );
    query.SET_VALUE ( ":t_is_son_satir_tutari"              , P_FIS_SATIRI->t_is_son_satir_tutari            );
    query.SET_VALUE ( ":t_is_son_str_vergi_haric_tutar"     , P_FIS_SATIRI->t_is_son_str_vergi_haric_tutar   );
    query.SET_VALUE ( ":t_is_son_satir_otv_tutari"          , P_FIS_SATIRI->t_is_son_satir_otv_tutari        );
    query.SET_VALUE ( ":t_is_son_satir_otv_dahil_tutar"     , P_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar   );
    query.SET_VALUE ( ":t_is_son_satir_kdv_tutari"          , P_FIS_SATIRI->t_is_son_satir_kdv_tutari        );
    query.SET_VALUE ( ":t_is_son_satir_kdv_dahil_tutar"     , P_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar   );
    query.SET_VALUE ( ":base_record_id"                     , P_FIS_SATIRI->base_record_id                   );
    query.SET_VALUE ( ":fis_satiri_id"                      , p_fis_satiri_id                                );

    query.UPDATE();

    FATURA_SATIR_DOVIZ_TUTARLARINI_GUNCELLE     ( P_FIS_SATIRI->doviz_kodu, P_FIS_SATIRI->satir_tutari, DOVIZ_EKLE, P_FIS->DOVIZ_CINSINDEN_SATIR_TOPLAMLARI );
    FATURA_KDV_ORANINA_GORE_TUTARLARI_GUNCELLE  ( P_FIS, P_FIS_SATIRI->kdv_orani, P_FIS_SATIRI->t_is_son_satir_kdv_tutari, VERGI_ORANI_EKLE  );
    FATURA_OTV_ORANINA_GORE_TUTARLARI_GUNCELLE  ( P_FIS, P_FIS_SATIRI->otv_orani, P_FIS_SATIRI->t_is_son_satir_otv_tutari, VERGI_ORANI_EKLE  );

    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS  :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS :

            query.PREPARE_SELECT ( "fat_irs_satir_baglantisi ","irsaliye_satiri_id",
                                   "fatura_satiri_id = :fatura_satiri_id");
            query.SET_VALUE(":fatura_satiri_id", p_fis_satiri_id );
            if ( query.SELECT() > 0 ) {

                query.NEXT();
                int old_irsaliye_satiri_id = query.VALUE(0).toInt();

                if ( ( old_irsaliye_satiri_id NE P_FIS_SATIRI->irsaliye_satiri_id ) OR ( OLD_FIS_SATIRI->satirdaki_urun_miktari NE P_FIS_SATIRI->satirdaki_urun_miktari ) ) {
                    if ( old_irsaliye_satiri_id NE P_FIS_SATIRI->irsaliye_satiri_id ) {

                        query.PREPARE_INSERT("fat_irs_satir_baglantisi" , "kayit_id" ,
                                             "fatura_satiri_id, irsaliye_satiri_id");

                        query.SET_VALUE ( ":fatura_satiri_id"   , p_fis_satiri_id         );
                        query.SET_VALUE ( ":irsaliye_satiri_id" , P_FIS_SATIRI->irsaliye_satiri_id  );
                        query.INSERT();

                    }
                    IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE ( P_FIS_SATIRI->irsaliye_satiri_id, P_FIS_SATIRI->satirdaki_urun_miktari, ENUM_IRS_FATURALANAN_MIKTAR_ARTIR );
                }
            }
            break;

        default :
            break;
    }

    if ( P_FIS->iade_faturasi_mi EQ 1 ) {
        query.PREPARE_SELECT ( "fat_iade_satir_baglantisi ","fat_iade_edilen_satir_id",
                                "fat_satir_id = :fat_satir_id");
        query.SET_VALUE      ( ":fat_satir_id", p_fis_satiri_id );

        if ( query.SELECT() EQ 0 ) {

           query.PREPARE_INSERT("fat_iade_satir_baglantisi" , "kayit_id" , "fat_iade_edilen_satir_id,fat_satir_id");
           query.SET_VALUE     ( ":fat_iade_edilen_satir_id", P_FIS_SATIRI->iade_edilen_fatura_satiri_id );
           query.SET_VALUE     ( ":fat_satir_id",             p_fis_satiri_id                    );
           query.INSERT();
        }

        FAT_FATURA_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( P_FIS_SATIRI->iade_edilen_fatura_satiri_id,
                                                       P_FIS_SATIRI->satirdaki_urun_miktari,
                                                       ENUM_IRS_IADE_MIKTARI_ARTIR );
    }

    P_FIS->brut_tutar                     =  ROUND ( OLD_FIS->brut_tutar                    + P_FIS_SATIRI->satir_vergi_haric_tutar  );
    P_FIS->isk_oncesi_otv_tutari          =  ROUND ( OLD_FIS->isk_oncesi_otv_tutari         + P_FIS_SATIRI->satir_otv_tutari         );
    P_FIS->isk_oncesi_otv_dahil_tutar     =  ROUND ( OLD_FIS->isk_oncesi_otv_dahil_tutar    + P_FIS_SATIRI->satir_otv_dahil_tutar    );
    P_FIS->isk_oncesi_kdv_tutari          =  ROUND ( OLD_FIS->isk_oncesi_kdv_tutari         + P_FIS_SATIRI->satir_kdv_tutari         );
    P_FIS->isk_oncesi_kdv_dahil_tutar     =  ROUND ( OLD_FIS->isk_oncesi_kdv_dahil_tutar    + P_FIS_SATIRI->satir_kdv_dahil_tutar    );

    FATURA_ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA ( P_FIS );

    switch ( P_FIS->fatura_turu ) {

        case ENUM_FAT_PROFORMA_SATIS_FATURASI :
            break;
        default  :
            QList<int> fis_no_list = FAT_FATURA_ENT_FISLERINI_OLUSTUR( P_FIS, P_FIS_SATIRI->fis_id );
            P_FIS->muh_fis_no  = fis_no_list.at(1);
            P_FIS->cari_fis_no = fis_no_list.at(0);
            break;
    }

    FAT_FATURA_FIS_STRUCT_UPDATE_DB( P_FIS, P_FIS_SATIRI->fis_id );

}

/***************************************************************************************
                            FATURA_FIS_BILGILERINI_OKU
***************************************************************************************/

int FATURA_FIS_BILGILERINI_OKU ( FATURA_FISI_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT("fat_faturalar" , "fis_no, fatura_tarihi, fatura_turu, fatura_alis_satis_turu, belge_seri, "
                         "belge_numarasi, acik_kapali_fatura, cari_hesap_id, cari_hesap_ismi, aciklama, "
                         "cari_hesap_adresi, vergi_dairesi, vergi_numarasi, islem_saati, "
                         "isk_oncesi_kdv_tutari, isk_oncesi_kdv_dahil_tutar, brut_tutar, isk_oncesi_otv_tutari, "
                         "isk_oncesi_otv_dahil_tutar, yazdirildi_mi, iptal_mi, modul_id, ara_toplam, "
                         "isk_sonrasi_kdv_tutari, fatura_tutari, isk_sonrasi_otv_tutari, "
                         "isk_sonrasi_otv_dahil_tutar, tevkifatli_fatura_mi, tevkifat_yuzdesi, "
                         "tevkifatli_kdv_tutari , muh_fis_id,program_id,"
                         "kdv_orani_sayisi,otv_orani_sayisi,"
                         "kdv_oranlari_array ,kdv_hesaplari_id_array,"
                         "kdv_oran_gore_tutarlar_array,otv_oranlari_array,"
                         "otv_hesaplari_id_array,otv_oran_gore_tutarlar_array,"
                         "tevkifatli_kdv_hsplar_id_array ,hizmet_urt_maliyeti_hesap_id,"
                         "yurtici_satislar_hesap_id , base_fis_id "
                         "fatura_turu , iade_faturasi_mi ,"
                         "fatura_irsaliyelestirildi_mi,"
                         "iade_edilen_fatura_id, iskonto_yuzdesi,"
                         "iskonto_tutari , cari_para_birimi_tutari, doviz_kuru, "
                         "parite, kdv_muaf_mi,otv_muaf_mi, "
                         "fatura_para_birimi_id, satistan_iade_hesap_id",
                         "fis_id = :fis_id");



    query.SET_VALUE       ( ":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    P_FIS->fis_no                           = query.VALUE ( "fis_no"                        ).toInt();
    P_FIS->fatura_tarihi                    = query.VALUE ( "fatura_tarihi"                 ).toString();
    P_FIS->fatura_alis_satis_turu           = query.VALUE ( "fatura_alis_satis_turu"        ).toInt();
    P_FIS->belge_seri                       = query.VALUE ( "belge_seri"                    ).toString();
    P_FIS->belge_numarasi                   = query.VALUE ( "belge_numarasi"                ).toString();
    P_FIS->acik_kapali_fatura               = query.VALUE ( "acik_kapali_fatura"            ).toInt();
    P_FIS->cari_hesap_id                    = query.VALUE ( "cari_hesap_id"                 ).toInt();
    P_FIS->cari_hesap_ismi                  = query.VALUE ( "cari_hesap_ismi"               ).toString();
    P_FIS->aciklama                         = query.VALUE ( "aciklama"                      ).toString();
    P_FIS->cari_hesap_adresi                = query.VALUE ( "cari_hesap_adresi"             ).toString();
    P_FIS->vergi_dairesi                    = query.VALUE ( "vergi_dairesi"                 ).toString();
    P_FIS->vergi_numarasi                   = query.VALUE ( "vergi_numarasi"                ).toString();
    P_FIS->islem_saati                      = query.VALUE ( "islem_saati"                   ).toString();
    P_FIS->isk_oncesi_kdv_tutari            = query.VALUE ( "isk_oncesi_kdv_tutari"         ).toDouble();
    P_FIS->isk_oncesi_kdv_dahil_tutar       = query.VALUE ( "isk_oncesi_kdv_dahil_tutar"    ).toDouble();
    P_FIS->brut_tutar                       = query.VALUE ( "brut_tutar"                    ).toDouble();
    P_FIS->isk_oncesi_otv_tutari            = query.VALUE ( "isk_oncesi_otv_tutari"         ).toDouble();
    P_FIS->isk_oncesi_otv_dahil_tutar       = query.VALUE ( "isk_oncesi_otv_dahil_tutar"    ).toDouble();
    P_FIS->ara_toplam                       = query.VALUE ( "ara_toplam"                    ).toDouble();
    P_FIS->isk_sonrasi_kdv_tutari           = query.VALUE ( "isk_sonrasi_kdv_tutari"        ).toDouble();
    P_FIS->fatura_tutari                    = query.VALUE ( "fatura_tutari"                 ).toDouble();
    P_FIS->isk_sonrasi_otv_tutari           = query.VALUE ( "isk_sonrasi_otv_tutari"        ).toDouble();
    P_FIS->isk_sonrasi_otv_dahil_tutar      = query.VALUE ( "isk_sonrasi_otv_dahil_tutar"   ).toDouble();
    P_FIS->yazdirildi_mi                    = query.VALUE ( "yazdirildi_mi"                 ).toInt();
    P_FIS->iptal_mi                         = query.VALUE ( "iptal_mi"                      ).toInt();
    P_FIS->modul_id                         = query.VALUE ( "modul_id"                      ).toInt();
    P_FIS->tevkifatli_fatura_mi             = query.VALUE ( "tevkifatli_fatura_mi"          ).toInt();
    P_FIS->tevkifat_yuzdesi                 = query.VALUE ( "tevkifat_yuzdesi"              ).toDouble();
    P_FIS->tevkifatli_kdv_tutari            = query.VALUE ( "tevkifatli_kdv_tutari"         ).toDouble();
    P_FIS->muh_fis_id                       = query.VALUE ( "muh_fis_id"                    ).toInt();
    P_FIS->program_id                       = query.VALUE ( "program_id"                    ).toInt();
    P_FIS->kdv_orani_sayisi                 = query.VALUE ( "kdv_orani_sayisi"              ).toInt();
    P_FIS->otv_orani_sayisi                 = query.VALUE ( "otv_orani_sayisi"              ).toInt();
    P_FIS->hizmet_urt_maliyeti_hesap_id     = query.VALUE ( "hizmet_urt_maliyeti_hesap_id"  ).toInt();
    P_FIS->yurtici_satislar_hesap_id        = query.VALUE ( "yurtici_satislar_hesap_id"     ).toInt();
    P_FIS->base_fis_id                      = query.VALUE ( "base_fis_id"                   ).toInt();
    P_FIS->fatura_turu                      = query.VALUE ( "fatura_turu"                   ).toInt();
    P_FIS->iade_faturasi_mi                 = query.VALUE ( "iade_faturasi_mi"              ).toInt();
    P_FIS->fatura_irsaliyelestirildi_mi     = query.VALUE ( "fatura_irsaliyelestirildi_mi"  ).toInt();
    P_FIS->iade_edilen_fatura_id            = query.VALUE ( "iade_edilen_fatura_id"         ).toInt();
    P_FIS->iskonto_yuzdesi                  = query.VALUE ( "iskonto_yuzdesi"               ).toDouble();
    P_FIS->iskonto_tutari                   = query.VALUE ( "iskonto_tutari"                ).toDouble();
    P_FIS->cari_para_birimi_tutari          = query.VALUE ( "cari_para_birimi_tutari"       ).toDouble();
    P_FIS->doviz_kuru                       = query.VALUE ( "doviz_kuru"                    ).toDouble();
    P_FIS->parite                           = query.VALUE ( "parite"                        ).toDouble();
    P_FIS->kdv_muaf_mi                      = query.VALUE ( "kdv_muaf_mi"                   ).toInt();
    P_FIS->otv_muaf_mi                      = query.VALUE ( "otv_muaf_mi"                   ).toInt();
    P_FIS->fatura_para_birimi_id            = query.VALUE ( "fatura_para_birimi_id"         ).toInt();
    P_FIS->satistan_iade_hesap_id           = query.VALUE ( "satistan_iade_hesap_id"         ).toInt();
    P_FIS->cari_para_birimi_id              = CARI_GET_PARA_BIRIM_ID( P_FIS->cari_hesap_id );
    P_FIS->fatura_para_birimi_kodu          = DVZ_GET_DOVIZ_KODU( P_FIS->fatura_para_birimi_id );

    UNPACK_DOUBLE_ARRAY ( query.VALUE ( "kdv_oranlari_array"               ).toString(), P_FIS->KDV_ORANLARI_ARRAY,                KDV_OTV_ORANLARI_SAYISI );
    UNPACK_INT_ARRAY    ( query.VALUE ( "kdv_hesaplari_id_array"           ).toString(), P_FIS->KDV_HESAPLARI_ID_ARRAY,            KDV_OTV_ORANLARI_SAYISI );
    UNPACK_DOUBLE_ARRAY ( query.VALUE ( "kdv_oran_gore_tutarlar_array"     ).toString(), P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY,   KDV_OTV_ORANLARI_SAYISI );

    UNPACK_DOUBLE_ARRAY ( query.VALUE ( "otv_oranlari_array"               ).toString(), P_FIS->OTV_ORANLARI_ARRAY,                KDV_OTV_ORANLARI_SAYISI );
    UNPACK_INT_ARRAY    ( query.VALUE ( "otv_hesaplari_id_array"           ).toString(), P_FIS->OTV_HESAPLARI_ID_ARRAY,            KDV_OTV_ORANLARI_SAYISI );
    UNPACK_DOUBLE_ARRAY ( query.VALUE ( "otv_oran_gore_tutarlar_array"     ).toString(), P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY,   KDV_OTV_ORANLARI_SAYISI );
    UNPACK_INT_ARRAY    ( query.VALUE ( "tevkifatli_kdv_hsplar_id_array"   ).toString(), P_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
    return 1; // eger kayit okursa 1 doner
}


/***************************************************************************************
                            FATURA_SATIR_BILGILERINI_OKU
***************************************************************************************/

int FATURA_SATIR_BILGILERINI_OKU(FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("fat_fatura_satirlari" , "fis_id , order_number , urun_id ,urun_adi,satirdaki_urun_miktari,"
                         "irsaliyelenen_miktar , urun_birimi , isk_oncesi_urun_birim_fiyati,"
                         "isk_sonrasi_urun_birim_fiyati , satir_iskonto_yuzdesi , satir_iskonto_tutari,"
                         "t_is_son_birim_fiyati , satir_tutari , t_is_son_satir_tutari,"
                         "doviz_id , kdv_orani , satir_kdv_tutari , satir_kdv_dahil_tutar ,"
                         "otv_orani , satir_vergi_haric_tutar , satir_otv_tutari , satir_otv_dahil_tutar,"
                         "t_is_son_satir_kdv_tutari , t_is_son_satir_kdv_dahil_tutar,"
                         "t_is_son_str_vergi_haric_tutar , t_is_son_satir_otv_tutari,"
                         "t_is_son_satir_otv_dahil_tutar , toplu_iskonto_yuzdesi , kdv_dahil_mi,"
                         "temel_birim_katsayisi , fiyat_irsaliyeden_alindi,hatali_satir_mi,"
                         "iade_edilen_miktar , program_id , modul_id , base_record_id " ,
                         "fis_satiri_id = :fis_satiri_id");

    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    P_FIS_SATIRI->fis_id                           = query.VALUE("fis_id").toInt();
    P_FIS_SATIRI->order_number                     = query.VALUE("order_number").toInt();
    P_FIS_SATIRI->urun_id                          = query.VALUE("urun_id").toInt();
    P_FIS_SATIRI->urun_adi                         = query.VALUE("urun_adi").toString();
    P_FIS_SATIRI->satirdaki_urun_miktari           = query.VALUE("satirdaki_urun_miktari").toDouble();
    P_FIS_SATIRI->irsaliyelenen_miktar             = query.VALUE("irsaliyelenen_miktar").toDouble();
    P_FIS_SATIRI->urun_birimi                      = query.VALUE("urun_birimi").toString();
    P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati     = query.VALUE("isk_oncesi_urun_birim_fiyati").toDouble();
    P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati    = query.VALUE("isk_sonrasi_urun_birim_fiyati").toDouble();
    P_FIS_SATIRI->satir_iskonto_yuzdesi            = query.VALUE("satir_iskonto_yuzdesi").toDouble();
    P_FIS_SATIRI->satir_iskonto_tutari             = query.VALUE("satir_iskonto_tutari").toDouble();
    P_FIS_SATIRI->t_is_son_birim_fiyati            = query.VALUE("t_is_son_birim_fiyati").toDouble();
    P_FIS_SATIRI->satir_tutari                     = query.VALUE("satir_tutari").toDouble();
    P_FIS_SATIRI->t_is_son_satir_tutari            = query.VALUE("t_is_son_satir_tutari").toDouble();
    P_FIS_SATIRI->doviz_id                         = query.VALUE("doviz_id").toInt();
    P_FIS_SATIRI->kdv_orani                        = query.VALUE("kdv_orani").toDouble();
    P_FIS_SATIRI->satir_kdv_tutari                 = query.VALUE("satir_kdv_tutari").toDouble();
    P_FIS_SATIRI->satir_kdv_dahil_tutar            = query.VALUE("satir_kdv_dahil_tutar").toDouble();
    P_FIS_SATIRI->otv_orani                        = query.VALUE("otv_orani").toDouble();
    P_FIS_SATIRI->satir_vergi_haric_tutar          = query.VALUE("satir_vergi_haric_tutar").toDouble();
    P_FIS_SATIRI->satir_otv_tutari                 = query.VALUE("satir_otv_tutari").toDouble();
    P_FIS_SATIRI->satir_otv_dahil_tutar            = query.VALUE("satir_otv_dahil_tutar").toDouble();
    P_FIS_SATIRI->t_is_son_satir_kdv_tutari        = query.VALUE("t_is_son_satir_kdv_tutari").toDouble();
    P_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar   = query.VALUE("t_is_son_satir_kdv_dahil_tutar").toDouble();
    P_FIS_SATIRI->t_is_son_str_vergi_haric_tutar   = query.VALUE("t_is_son_str_vergi_haric_tutar").toDouble();
    P_FIS_SATIRI->t_is_son_satir_otv_tutari        = query.VALUE("t_is_son_satir_otv_tutari").toDouble();
    P_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar   = query.VALUE("t_is_son_satir_otv_dahil_tutar").toDouble();
    P_FIS_SATIRI->toplu_iskonto_yuzdesi            = query.VALUE("toplu_iskonto_yuzdesi").toDouble();
    P_FIS_SATIRI->kdv_dahil_mi                     = query.VALUE("kdv_dahil_mi").toInt();
    P_FIS_SATIRI->temel_birim_katsayisi            = query.VALUE("temel_birim_katsayisi").toDouble();
    P_FIS_SATIRI->fiyat_irsaliyeden_alindi         = query.VALUE("fiyat_irsaliyeden_alindi").toInt();
    P_FIS_SATIRI->hatali_satir_mi                  = query.VALUE("hatali_satir_mi").toInt();
    P_FIS_SATIRI->iade_edilen_miktar               = query.VALUE("iade_edilen_miktar").toDouble();
    P_FIS_SATIRI->program_id                       = query.VALUE("program_id").toInt();
    P_FIS_SATIRI->modul_id                         = query.VALUE("modul_id").toInt();
    P_FIS_SATIRI->base_record_id                   = query.VALUE("base_record_id").toInt();

    return 1;
}


/***************************************************************************************
                                FATURA_CLEAR_FIS_STRUCT
***************************************************************************************/

void FATURA_CLEAR_FIS_STRUCT ( FATURA_FISI_STRUCT * P_FIS )
{
    P_FIS->fatura_tarihi.clear();
    P_FIS->belge_seri.clear();
    P_FIS->cari_hesap_ismi.clear();
    P_FIS->aciklama.clear();
    P_FIS->cari_hesap_adresi.clear();
    P_FIS->vergi_dairesi.clear();
    P_FIS->vergi_numarasi.clear();
    P_FIS->islem_saati.clear();
    P_FIS->doviz_tutar_bilgileri.clear();
    P_FIS->kdv_oran_bilgileri.clear();
    P_FIS->otv_oran_bilgileri.clear();
    P_FIS->DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.clear();

    P_FIS->fis_no                               =  0;
    P_FIS->fatura_turu                          =  0;
    P_FIS->fatura_alis_satis_turu               =  0;
    P_FIS->belge_numarasi.clear();
    P_FIS->acik_kapali_fatura                   = -1;
    P_FIS->cari_hesap_id                        =  0;
    P_FIS->isk_oncesi_kdv_tutari                =  0.0;
    P_FIS->isk_oncesi_kdv_dahil_tutar           =  0.0;
    P_FIS->brut_tutar                           =  0.0;
    P_FIS->isk_oncesi_otv_tutari                =  0.0;
    P_FIS->isk_oncesi_otv_dahil_tutar           =  0.0;
    P_FIS->yazdirildi_mi                        =  0;
    P_FIS->iptal_mi                             =  0;
    P_FIS->modul_id                             = -1;
    P_FIS->program_id                           = -1;
    P_FIS->base_fis_id                          =  0;
    P_FIS->fatura_irsaliyelestirildi_mi         =  0;
    P_FIS->yurtici_satislar_hesap_id            =  0;
    P_FIS->hizmet_urt_maliyeti_hesap_id         =  0;
    P_FIS->iade_faturasi_mi                     =  0;
    P_FIS->iade_edilen_fatura_id                =  0;
    P_FIS->tevkifatli_fatura_mi                 =  0;
    P_FIS->tevkifat_yuzdesi                     =  0.0;
    P_FIS->tevkifatli_kdv_tutari                =  0.0;
    P_FIS->iskonto_yuzdesi                      =  0.0;
    P_FIS->iskonto_tutari                       =  0.0;
    P_FIS->ara_toplam                           =  0.0;
    P_FIS->fatura_tutari                        =  0.0;
    P_FIS->isk_sonrasi_otv_tutari               =  0.0;
    P_FIS->isk_sonrasi_otv_dahil_tutar          =  0.0;
    P_FIS->cari_para_birimi_tutari              =  0.0;
    P_FIS->cari_para_birimi_id                  =  -1;
    P_FIS->fatura_para_birimi_id                =  -1;
    P_FIS->fatura_para_birimi_kodu.clear();
    P_FIS->doviz_kuru                           =  0.0;
    P_FIS->parite                               =  0.0;
    P_FIS->kdv_orani_sayisi                     =  0;
    P_FIS->otv_orani_sayisi                     =  0;
    P_FIS->isk_sonrasi_kdv_tutari               =  0;
    P_FIS->cari_fis_no                          =  0;
    P_FIS->muh_fis_no                           =  0;
    P_FIS->depo_id                              =  0;
    P_FIS->muh_fis_id                           =  0;
    P_FIS->irs_belge_seri.clear();
    P_FIS->irs_belge_numarasi.clear();

    for ( int i = 0; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {
        P_FIS->KDV_HESAPLARI_ID_ARRAY[i]             = 0;
        P_FIS->OTV_HESAPLARI_ID_ARRAY[i]             = 0;
        P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i]    = 0.0;
        P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i]    = 0.0;
        P_FIS->KDV_ORANLARI_ARRAY[i]                 = 0.0;
        P_FIS->OTV_ORANLARI_ARRAY[i]                 = 0.0;
        P_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY[i]  = 0;
    }
}

/***************************************************************************************
                                FATURA_CLEAR_FIS_SATIRI_STRUCT
***************************************************************************************/

void FATURA_CLEAR_FIS_SATIRI_STRUCT ( FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI )
{
    P_FIS_SATIRI->urun_adi.clear();
    P_FIS_SATIRI->urun_birimi.clear();
    P_FIS_SATIRI->doviz_kodu.clear();
    P_FIS_SATIRI->parti_kodu.clear();

    P_FIS_SATIRI->fis_id                            =   0;
    P_FIS_SATIRI->order_number                      =   0;
    P_FIS_SATIRI->urun_id                           =   0;
    P_FIS_SATIRI->satirdaki_urun_miktari            =   0.0;
    P_FIS_SATIRI->irsaliyelenen_miktar              =   0.0;
    P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati      =   0.0;
    P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati     =   0.0;
    P_FIS_SATIRI->satir_iskonto_yuzdesi             =   0.0;
    P_FIS_SATIRI->satir_iskonto_tutari              =   0.0;
    P_FIS_SATIRI->t_is_son_birim_fiyati             =   0.0;
    P_FIS_SATIRI->satir_tutari                      =   0.0;
    P_FIS_SATIRI->t_is_son_satir_tutari             =   0.0;
    P_FIS_SATIRI->doviz_id                          =   0;
    P_FIS_SATIRI->kdv_orani                         =   0;
    P_FIS_SATIRI->satir_kdv_tutari                  =   0.0;
    P_FIS_SATIRI->satir_kdv_dahil_tutar             =   0.0;
    P_FIS_SATIRI->otv_orani                         =   0.0;
    P_FIS_SATIRI->satir_vergi_haric_tutar           =   0.0;
    P_FIS_SATIRI->satir_otv_tutari                  =   0.0;
    P_FIS_SATIRI->satir_otv_dahil_tutar             =   0.0;
    P_FIS_SATIRI->t_is_son_satir_kdv_tutari         =   0.0;
    P_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar    =   0.0;
    P_FIS_SATIRI->t_is_son_str_vergi_haric_tutar    =   0.0;
    P_FIS_SATIRI->t_is_son_satir_otv_tutari         =   0.0;
    P_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar    =   0.0;
    P_FIS_SATIRI->toplu_iskonto_yuzdesi             =   0.0;
    P_FIS_SATIRI->kdv_dahil_mi                      =   0;
    P_FIS_SATIRI->temel_birim_katsayisi             =   0.0;
    P_FIS_SATIRI->fiyat_irsaliyeden_alindi          =   0;
    P_FIS_SATIRI->hatali_satir_mi                   =   0;
    P_FIS_SATIRI->iade_edilen_miktar                =   0.0;
    P_FIS_SATIRI->irsaliye_satiri_id                =   0;
    P_FIS_SATIRI->iade_edilen_fatura_satiri_id      =   0;
    P_FIS_SATIRI->parti_id                          =   0;
    P_FIS_SATIRI->program_id                        =   0;
    P_FIS_SATIRI->modul_id                          =   0;
    P_FIS_SATIRI->base_record_id                    =   0;
    P_FIS_SATIRI->seri_no_list.clear();

}


/*****************************************************************************************************
                 FATURA_KDV_OTV_DEGISKENLERINE_ILK_DEGERLERINI_YAZ
*******************************************************************************************************/

void FATURA_KDV_OTV_DEGISKENLERINE_ILK_DEGERLERINI_YAZ ( FATURA_FISI_STRUCT * P_FIS )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ("e9_sabit_degerler" ,"kdv_orani_sayisi, kdv_oranlari_array, otv_orani_sayisi, otv_oranlari_array , "
                           "ind_kdv_hesap_id_array, ind_kdv_iade_hesap_id_array, hes_kdv_hesap_id_array, "
                           "hes_kdv_iade_hesap_id_array, otv_alis_hesap_id_array, otv_satis_hesap_id_array, "
                           "tevkifat_kdv_hesap_id_array ");

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    P_FIS->kdv_orani_sayisi = query.VALUE ( "kdv_orani_sayisi" ).toInt();
    P_FIS->otv_orani_sayisi = query.VALUE ( "otv_orani_sayisi" ).toInt();

    UNPACK_DOUBLE_ARRAY ( query.VALUE ( "kdv_oranlari_array" ).toString(), P_FIS->KDV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI );
    UNPACK_DOUBLE_ARRAY ( query.VALUE ( "otv_oranlari_array" ).toString(), P_FIS->OTV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI );

    QString ind_kdv_hesap_id_array      = query.VALUE ( "ind_kdv_hesap_id_array"      ).toString();
    QString ind_kdv_iade_hesap_id_array = query.VALUE ( "ind_kdv_iade_hesap_id_array" ).toString();
    QString hes_kdv_hesap_id_array      = query.VALUE ( "hes_kdv_hesap_id_array"      ).toString();
    QString hes_kdv_iade_hesap_id_array = query.VALUE ( "hes_kdv_iade_hesap_id_array" ).toString();
    QString otv_alis_hesap_id_array     = query.VALUE ( "otv_alis_hesap_id_array"     ).toString();
    QString otv_satis_hesap_id_array    = query.VALUE ( "otv_satis_hesap_id_array"    ).toString();
    QString tevkifat_kdv_hesap_id_array = query.VALUE ( "tevkifat_kdv_hesap_id_array" ).toString();

    switch ( P_FIS->fatura_alis_satis_turu ) {

        case ENUM_ALIS_IRSALIYESI_FATURASI :
        default                            :

            if ( P_FIS->iade_faturasi_mi EQ 0 ) {
                UNPACK_INT_ARRAY ( ind_kdv_hesap_id_array, P_FIS->KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
            }
            else {
                UNPACK_INT_ARRAY ( ind_kdv_iade_hesap_id_array, P_FIS->KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
            }
            UNPACK_INT_ARRAY ( otv_alis_hesap_id_array,     P_FIS->OTV_HESAPLARI_ID_ARRAY,            KDV_OTV_ORANLARI_SAYISI );
            UNPACK_INT_ARRAY ( tevkifat_kdv_hesap_id_array, P_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
            break;

        case ENUM_SATIS_IRSALIYESI_FATURASI :

            if ( P_FIS->iade_faturasi_mi EQ 0 ) {
                UNPACK_INT_ARRAY ( hes_kdv_hesap_id_array, P_FIS->KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
            }
            else {
                UNPACK_INT_ARRAY ( hes_kdv_iade_hesap_id_array, P_FIS->KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
            }
            UNPACK_INT_ARRAY ( otv_satis_hesap_id_array,    P_FIS->OTV_HESAPLARI_ID_ARRAY,            KDV_OTV_ORANLARI_SAYISI );
            UNPACK_INT_ARRAY ( tevkifat_kdv_hesap_id_array, P_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
            break;
    }
}

/*****************************************************************************************************
                 FATURA_KDV_ORANINA_GORE_TUTARLARI_GUNCELLE
*******************************************************************************************************/

void FATURA_KDV_ORANINA_GORE_TUTARLARI_GUNCELLE ( FATURA_FISI_STRUCT * P_FIS, double p_kdv_orani, double p_kdv_tutari, int p_kdv_artir_azalt )
{
    for ( int i = 0; i < P_FIS->kdv_orani_sayisi; i++ ) {

        if ( P_FIS->KDV_ORANLARI_ARRAY[i] NE p_kdv_orani ) {
            continue;
        }

        if ( p_kdv_artir_azalt EQ VERGI_ORANI_EKLE ) {
            P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] = ROUND ( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] + p_kdv_tutari );
            break;
        }
        else {
            P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] = ROUND ( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] - p_kdv_tutari );
            break;
        }
    }
}

/*****************************************************************************************************
                 FATURA_OTV_ORANINA_GORE_TUTARLARI_GUNCELLE
*******************************************************************************************************/

void FATURA_OTV_ORANINA_GORE_TUTARLARI_GUNCELLE ( FATURA_FISI_STRUCT * P_FIS, double p_otv_orani, double p_otv_tutari, int p_otv_artir_azalt )
{
    for ( int i = 0; i < P_FIS->otv_orani_sayisi; i++ ) {

        if ( P_FIS->OTV_ORANLARI_ARRAY[i] NE p_otv_orani ) {
            continue;
        }

        if ( p_otv_artir_azalt EQ VERGI_ORANI_EKLE ) {
            P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] = ROUND ( P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] + p_otv_tutari );
            break;
        }
        else {
            P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] = ROUND ( P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] - p_otv_tutari );
            break;
        }
    }
}

/**************************************************************************************
                   FATURA_SATIR_DOVIZ_TUTARLARINI_GUNCELLE
***************************************************************************************/

void FATURA_SATIR_DOVIZ_TUTARLARINI_GUNCELLE ( QString p_satir_doviz_kodu, double p_satir_doviz_tutari, int p_ekle_sil, QStringList & P_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI )
{
    QStringList SATIR_BILGILERI;

    double      doviz_toplami      = 0.00;
    bool        doviz_kodu_bulundu = false;

    if ( p_satir_doviz_tutari EQ 0 ) {
        return;
    }

    for ( int i = 0; i < P_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.size(); i++ ) {

        SATIR_BILGILERI = P_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.at(i).split("\t");

        doviz_toplami      = SATIR_BILGILERI.at(0).toDouble();
        QString doviz_kodu = SATIR_BILGILERI.at(1);
        if ( doviz_kodu EQ p_satir_doviz_kodu ) {
            doviz_kodu_bulundu = true;

            switch ( p_ekle_sil ) {

                case DOVIZ_EKLE :
                    doviz_toplami += p_satir_doviz_tutari;
                    P_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI[i] = QVariant(doviz_toplami).toString() + "\t" + p_satir_doviz_kodu;
                    return;

                case DOVIZ_SIL :
                    doviz_toplami -= p_satir_doviz_tutari;
                    if ( doviz_toplami <= 0 ) {
                        P_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.removeAt(i);
                    }
                    else {
                        P_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI[i] = QVariant(doviz_toplami).toString() + "\t" + p_satir_doviz_kodu;
                    }
                    return;

                default :
                    return;
            }
        }
    }
    if ( doviz_kodu_bulundu EQ false ) {
        if ( p_ekle_sil EQ DOVIZ_EKLE ) {
            QString satir_bilgileri = QVariant( p_satir_doviz_tutari ).toString() + "\t" + p_satir_doviz_kodu;
            P_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.append(satir_bilgileri);
        }
    }
}

/**************************************************************************************
                   FATURA_ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA
***************************************************************************************/

void FATURA_ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA ( FATURA_FISI_STRUCT * P_FIS )
{

    double  iskonto_yuzdesi                 = P_FIS->iskonto_yuzdesi;
    double  ara_toplam                      = ROUND ( P_FIS->brut_tutar * ( 1 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  isk_sonrasi_otv_tutari          = ROUND ( P_FIS->isk_oncesi_otv_tutari      * ( 1 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  isk_sonrasi_otv_dahil_tutar     = ROUND ( P_FIS->isk_oncesi_otv_dahil_tutar * ( 1 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  isk_sonrasi_kdv_tutari          = ROUND ( P_FIS->isk_oncesi_kdv_tutari      * ( 1 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  fatura_tutari                   = ROUND ( P_FIS->isk_oncesi_kdv_dahil_tutar * ( 1 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  iskonto_tutari                  = ROUND ( P_FIS->brut_tutar - ara_toplam                                           );


    P_FIS->tevkifatli_kdv_tutari = 0.0;

    if ( P_FIS->tevkifatli_fatura_mi EQ 1 ) {

        double tevkifat_yuzdesi = P_FIS->tevkifat_yuzdesi;

        P_FIS->tevkifatli_kdv_tutari = ROUND ( ( isk_sonrasi_kdv_tutari * tevkifat_yuzdesi ) / 100.0     );
        fatura_tutari                = ROUND ( fatura_tutari            - isk_sonrasi_kdv_tutari       );
        isk_sonrasi_kdv_tutari       = ROUND ( isk_sonrasi_kdv_tutari   - P_FIS->tevkifatli_kdv_tutari );
        fatura_tutari                = ROUND ( fatura_tutari            + isk_sonrasi_kdv_tutari       );
    }

    P_FIS->ara_toplam                   = ara_toplam;
    P_FIS->isk_sonrasi_kdv_tutari       = isk_sonrasi_kdv_tutari;
    P_FIS->fatura_tutari                = fatura_tutari;
    P_FIS->isk_sonrasi_otv_tutari       = isk_sonrasi_otv_tutari;
    P_FIS->isk_sonrasi_otv_dahil_tutar  = isk_sonrasi_otv_dahil_tutar;
    P_FIS->iskonto_tutari               = iskonto_tutari;

    int temel_para_birimi_id = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

    double carpim_katsayisi = P_FIS->doviz_kuru;

    if( P_FIS->cari_para_birimi_id NE temel_para_birimi_id AND P_FIS->fatura_para_birimi_id NE temel_para_birimi_id ) {
        if( P_FIS->cari_para_birimi_id NE P_FIS->fatura_para_birimi_id ) {
            carpim_katsayisi = P_FIS->parite;
        }
        else {
            carpim_katsayisi = 1.000; // ayni ise cari direk gider
        }
    }

    P_FIS->cari_para_birimi_tutari      = ROUND( P_FIS->fatura_tutari * carpim_katsayisi ) ;
}

/**************************************************************************************
                    FAT_FATURA_IRSALIYELESTIR
***************************************************************************************/

void FAT_FATURA_IRSALIYELESTIR (  int p_fatura_id, FATURA_FISI_STRUCT * P_FAT_FIS  )
{

    int fis_no  = STK_SIRADAKI_FIS_NO_AL ( P_FAT_FIS->fatura_tarihi );

    STK_IRS_FIS_STRUCT * STK_IRS_FIS = new STK_IRS_FIS_STRUCT;

    STK_IRS_FIS->fis_tarihi             = P_FAT_FIS->fatura_tarihi;
    STK_IRS_FIS->cari_hesap_id          = P_FAT_FIS->cari_hesap_id;
    STK_IRS_FIS->cari_hesap_ismi        = P_FAT_FIS->cari_hesap_ismi;
    STK_IRS_FIS->aciklama               = P_FAT_FIS->aciklama;
    STK_IRS_FIS->teslim_adresi          = P_FAT_FIS->cari_hesap_adresi;
    STK_IRS_FIS->vergi_dairesi          = P_FAT_FIS->vergi_dairesi;
    STK_IRS_FIS->vergi_numarasi         = P_FAT_FIS->vergi_numarasi;
    STK_IRS_FIS->islem_saati            = P_FAT_FIS->islem_saati;
    STK_IRS_FIS->iade_irsaliyesi_mi     = P_FAT_FIS->iade_faturasi_mi;

    QString     irsaliye_belge_numarasi;
    QString     irsaliye_belge_seri;

    int     irsaliye_turu            = ENUM_IRS_NORMAL_GIRIS;
    int     irsaliye_alis_satis_turu = ENUM_ALIS_IRSALIYESI_FATURASI;

    // irsaliyeye gonderirken iadeler icin ter islem yapilip gonderilmelidir
    switch ( P_FAT_FIS->fatura_turu ) {

        case     ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case     ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        default                                     :

            if( STK_IRS_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                irsaliye_turu            = ENUM_IRS_NORMAL_CIKIS;
                irsaliye_alis_satis_turu = ENUM_SATIS_IRSALIYESI_FATURASI;
            }
            else {
                irsaliye_turu            = ENUM_IRS_NORMAL_GIRIS;
                irsaliye_alis_satis_turu = ENUM_ALIS_IRSALIYESI_FATURASI;
            }
            break;

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS    :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI     :
            if( STK_IRS_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                irsaliye_turu            = ENUM_IRS_NORMAL_GIRIS;
                irsaliye_alis_satis_turu = ENUM_ALIS_IRSALIYESI_FATURASI;
            }
            else {
                irsaliye_turu            = ENUM_IRS_NORMAL_CIKIS;
                irsaliye_alis_satis_turu = ENUM_SATIS_IRSALIYESI_FATURASI;
            }
            break;
    }

    STK_IRS_FIS->irsaliye_alis_satis_turu = irsaliye_alis_satis_turu;
    STK_IRS_FIS->fis_turu                 = irsaliye_turu;

    // IRSALIYELI FATURADA FARKLI BELGE_SERI VE BELGE NUMARSI ALIR.
    if ( P_FAT_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) {
        irsaliye_belge_seri       = P_FAT_FIS->belge_seri;
        irsaliye_belge_numarasi   = P_FAT_FIS->belge_numarasi;
    }
    else if ( P_FAT_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS ) {
        irsaliye_belge_seri     = P_FAT_FIS->irs_belge_seri;
        irsaliye_belge_numarasi = P_FAT_FIS->irs_belge_numarasi;
    }
    else {
        irsaliye_belge_seri     = P_FAT_FIS->belge_seri;
        irsaliye_belge_numarasi = P_FAT_FIS->belge_numarasi;
    }

    STK_IRS_FIS->belge_seri                     = irsaliye_belge_seri;
    STK_IRS_FIS->belge_numarasi                 = irsaliye_belge_numarasi;
    STK_IRS_FIS->irsaliye_mi_stok_fisi_mi       = ENUM_IRSALIYE;
    STK_IRS_FIS->yazdirildi_mi                  = 0;
    STK_IRS_FIS->iptal_mi                       = 0;
    STK_IRS_FIS->modul_id                       = FATURA_MODULU;
    STK_IRS_FIS->program_id                     = E9_PROGRAMI;
    STK_IRS_FIS->base_fis_id                    = p_fatura_id;
    STK_IRS_FIS->fis_no                         = fis_no;
    STK_IRS_FIS->depo_id                        = P_FAT_FIS->depo_id;
    STK_IRS_FIS->irsaliye_faturalastirildi_mi   = 1;
    STK_IRS_FIS->stk_irs_para_birimi_id         = P_FAT_FIS->fatura_para_birimi_id;
    STK_IRS_FIS->parite                         = P_FAT_FIS->parite;
    STK_IRS_FIS->doviz_kuru                     = P_FAT_FIS->doviz_kuru;

    int irsaliye_id = STK_IRS_FIS_EKLE( STK_IRS_FIS );

    FAT_IRS_BAGLANTISI_EKLE( p_fatura_id, irsaliye_id );

    delete STK_IRS_FIS;
}

/**************************************************************************************
                    FAT_FATURA_IRSALIYE_STR_EKLE
***************************************************************************************/

int FAT_FATURA_IRSALIYE_STR_EKLE ( int p_fatura_satiri_id,  FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI )
{

    SQL_QUERY query ( DB );

    STK_IRS_FIS_SATIR_STRUCT * IRS_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

    STK_IRS_CLEAR_SATIR_STRUCT ( IRS_FIS_SATIRI );

    IRS_FIS_SATIRI->fis_id                              = STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID( P_FIS_SATIRI->fis_id );
    IRS_FIS_SATIRI->urun_id                             = P_FIS_SATIRI->urun_id;
    IRS_FIS_SATIRI->urun_adi                            = P_FIS_SATIRI->urun_adi;
    IRS_FIS_SATIRI->satirdaki_urun_miktari              = P_FIS_SATIRI->satirdaki_urun_miktari;
    IRS_FIS_SATIRI->urun_birimi                         = P_FIS_SATIRI->urun_birimi;
    IRS_FIS_SATIRI->isk_oncesi_urun_birim_fiyati        = P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati;
    IRS_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati       = P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati;
    IRS_FIS_SATIRI->iskonto_yuzdesi                     = P_FIS_SATIRI->satir_iskonto_yuzdesi;
    IRS_FIS_SATIRI->iskonto_tutari                      = P_FIS_SATIRI->satir_iskonto_tutari;
    IRS_FIS_SATIRI->satir_tutari                        = P_FIS_SATIRI->satir_tutari;
    IRS_FIS_SATIRI->doviz_id                            = P_FIS_SATIRI->doviz_id;
    IRS_FIS_SATIRI->kdv_orani                           = P_FIS_SATIRI->kdv_orani;
    IRS_FIS_SATIRI->otv_orani                           = P_FIS_SATIRI->otv_orani;
    IRS_FIS_SATIRI->kdv_dahil_mi                        = P_FIS_SATIRI->kdv_dahil_mi;
    IRS_FIS_SATIRI->satir_vergi_haric_tutar             = P_FIS_SATIRI->satir_vergi_haric_tutar;
    IRS_FIS_SATIRI->satir_otv_tutari                    = P_FIS_SATIRI->satir_otv_tutari;
    IRS_FIS_SATIRI->satir_otv_dahil_tutar               = P_FIS_SATIRI->satir_otv_dahil_tutar;
    IRS_FIS_SATIRI->satir_kdv_tutari                    = P_FIS_SATIRI->satir_kdv_tutari;
    IRS_FIS_SATIRI->satir_kdv_dahil_tutar               = P_FIS_SATIRI->satir_kdv_dahil_tutar;
    IRS_FIS_SATIRI->t_is_sonrasi_birim_fiyat            = P_FIS_SATIRI->t_is_son_birim_fiyati;
    IRS_FIS_SATIRI->t_is_sonrasi_satir_tutari           = P_FIS_SATIRI->t_is_son_satir_tutari;
    IRS_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar      = P_FIS_SATIRI->t_is_son_str_vergi_haric_tutar;
    IRS_FIS_SATIRI->t_is_sonrasi_otv_tutari             = P_FIS_SATIRI->t_is_son_satir_otv_tutari;
    IRS_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar        = P_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar;
    IRS_FIS_SATIRI->t_is_sonrasi_kdv_tutari             = P_FIS_SATIRI->t_is_son_satir_kdv_tutari;
    IRS_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar        = P_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar;
    IRS_FIS_SATIRI->toplu_iskonto_yuzdesi               = P_FIS_SATIRI->toplu_iskonto_yuzdesi;
    IRS_FIS_SATIRI->temel_birim_katsayisi               = P_FIS_SATIRI->temel_birim_katsayisi;
    IRS_FIS_SATIRI->hatali_satir_mi                     = P_FIS_SATIRI->hatali_satir_mi;
    IRS_FIS_SATIRI->iade_edilen_miktar                  = P_FIS_SATIRI->iade_edilen_miktar;
    IRS_FIS_SATIRI->temel_birim_katsayisi               = P_FIS_SATIRI->temel_birim_katsayisi;
    IRS_FIS_SATIRI->faturalanan_miktar                  = P_FIS_SATIRI->satirdaki_urun_miktari;
    IRS_FIS_SATIRI->modul_id                            = FATURA_MODULU;
    IRS_FIS_SATIRI->program_id                          = E9_PROGRAMI;
    IRS_FIS_SATIRI->seri_no_list                        = P_FIS_SATIRI->seri_no_list;
    IRS_FIS_SATIRI->parti_id                            = P_FIS_SATIRI->parti_id;
    IRS_FIS_SATIRI->parti_kodu                          = P_FIS_SATIRI->parti_kodu;
    IRS_FIS_SATIRI->base_record_id                      = p_fatura_satiri_id;


    STK_IRS_FIS_STRUCT * IRS_FIS = new STK_IRS_FIS_STRUCT;

    STK_IRS_CLEAR_FIS_STRUCT   ( IRS_FIS );
    STK_IRS_FIS_BILGILERINI_OKU( IRS_FIS_SATIRI->fis_id, IRS_FIS );

    int irs_satir_id = STK_IRS_FIS_SATIRI_EKLE( IRS_FIS, IRS_FIS_SATIRI );

    //! HAREKETLERI OLUSTURULUYOR
    STK_IRS_FIS_SATIR_HAREKETLERI_OLUSTUR( IRS_FIS, IRS_FIS_SATIRI, irs_satir_id );

    FAT_UPDATE_FATURA_SATIRI_URUN_MIKTARI( p_fatura_satiri_id, P_FIS_SATIRI->satirdaki_urun_miktari );

    query.PREPARE_INSERT( "fat_irs_satir_baglantisi", "kayit_id", "fatura_satiri_id, irsaliye_satiri_id" );
    query.SET_VALUE( ":fatura_satiri_id"    , p_fatura_satiri_id );
    query.SET_VALUE( ":irsaliye_satiri_id"  , irs_satir_id       );
    query.INSERT();

    delete IRS_FIS_SATIRI;
    delete IRS_FIS;

    return irs_satir_id;
}


/**************************************************************************************
                    FAT_FATURA_IRSALIYE_STR_SIL
***************************************************************************************/

void FAT_FATURA_IRSALIYE_STR_SIL ( int p_fatura_satiri_id )
{
    SQL_QUERY f_query   ( DB );

    f_query.PREPARE_SELECT ( "fat_irs_satir_baglantisi ","irsaliye_satiri_id",
                             "fatura_satiri_id = :fatura_satiri_id" );
    f_query.SET_VALUE      ( ":fatura_satiri_id", p_fatura_satiri_id );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    f_query.NEXT();

    int irsaliye_satiri_id = f_query.VALUE(0).toInt();


    STK_IRS_FIS_STRUCT       * IRS_FIS        = new STK_IRS_FIS_STRUCT;
    STK_IRS_FIS_SATIR_STRUCT * IRS_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

    STK_IRS_CLEAR_SATIR_STRUCT( IRS_FIS_SATIRI );
    STK_IRS_CLEAR_FIS_STRUCT  ( IRS_FIS );

    STK_IRS_SATIR_BILGILERINI_OKU( irsaliye_satiri_id, IRS_FIS_SATIRI );
    STK_IRS_FIS_BILGILERINI_OKU  ( IRS_FIS_SATIRI->fis_id, IRS_FIS );

    STK_IRS_FIS_SATIRI_SIL( IRS_FIS, IRS_FIS_SATIRI, irsaliye_satiri_id );

     // SATIR BAGLANTISI SILINIYOR.
    f_query.PREPARE_DELETE ( "fat_irs_satir_baglantisi ",
                             "irsaliye_satiri_id = :irsaliye_satiri_id");
    f_query.SET_VALUE      ( ":irsaliye_satiri_id", irsaliye_satiri_id );
    f_query.DELETE();

    delete IRS_FIS;
    delete IRS_FIS_SATIRI;
}

/**************************************************************************************
                        FAT_FATURA_IRSALIYE_STR_UPDATE
***************************************************************************************/

void FAT_FATURA_IRSALIYE_STR_UPDATE( FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fatura_satiri_id )
{

    SQL_QUERY f_query   ( DB );

    STK_IRS_FIS_SATIR_STRUCT * IRS_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

    IRS_FIS_SATIRI->fis_id                              = STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID( P_FIS_SATIRI->fis_id );
    IRS_FIS_SATIRI->urun_id                             = P_FIS_SATIRI->urun_id;
    IRS_FIS_SATIRI->urun_adi                            = P_FIS_SATIRI->urun_adi;
    IRS_FIS_SATIRI->satirdaki_urun_miktari              = P_FIS_SATIRI->satirdaki_urun_miktari;
    IRS_FIS_SATIRI->urun_birimi                         = P_FIS_SATIRI->urun_birimi;
    IRS_FIS_SATIRI->isk_oncesi_urun_birim_fiyati        = P_FIS_SATIRI->isk_oncesi_urun_birim_fiyati;
    IRS_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati       = P_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati;
    IRS_FIS_SATIRI->iskonto_yuzdesi                     = P_FIS_SATIRI->satir_iskonto_yuzdesi;
    IRS_FIS_SATIRI->iskonto_tutari                      = P_FIS_SATIRI->satir_iskonto_tutari;
    IRS_FIS_SATIRI->satir_tutari                        = P_FIS_SATIRI->satir_tutari;
    IRS_FIS_SATIRI->doviz_id                            = P_FIS_SATIRI->doviz_id;
    IRS_FIS_SATIRI->kdv_orani                           = P_FIS_SATIRI->kdv_orani;
    IRS_FIS_SATIRI->otv_orani                           = P_FIS_SATIRI->otv_orani;
    IRS_FIS_SATIRI->kdv_dahil_mi                        = P_FIS_SATIRI->kdv_dahil_mi;
    IRS_FIS_SATIRI->satir_vergi_haric_tutar             = P_FIS_SATIRI->satir_vergi_haric_tutar;
    IRS_FIS_SATIRI->satir_otv_tutari                    = P_FIS_SATIRI->satir_otv_tutari;
    IRS_FIS_SATIRI->satir_otv_dahil_tutar               = P_FIS_SATIRI->satir_otv_dahil_tutar;
    IRS_FIS_SATIRI->satir_kdv_tutari                    = P_FIS_SATIRI->satir_kdv_tutari;
    IRS_FIS_SATIRI->satir_kdv_dahil_tutar               = P_FIS_SATIRI->satir_kdv_dahil_tutar;
    IRS_FIS_SATIRI->t_is_sonrasi_birim_fiyat            = P_FIS_SATIRI->t_is_son_birim_fiyati;
    IRS_FIS_SATIRI->t_is_sonrasi_satir_tutari           = P_FIS_SATIRI->t_is_son_satir_tutari;
    IRS_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar      = P_FIS_SATIRI->t_is_son_str_vergi_haric_tutar;
    IRS_FIS_SATIRI->t_is_sonrasi_otv_tutari             = P_FIS_SATIRI->t_is_son_satir_otv_tutari;
    IRS_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar        = P_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar;
    IRS_FIS_SATIRI->t_is_sonrasi_kdv_tutari             = P_FIS_SATIRI->t_is_son_satir_kdv_tutari;
    IRS_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar        = P_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar;
    IRS_FIS_SATIRI->toplu_iskonto_yuzdesi               = P_FIS_SATIRI->toplu_iskonto_yuzdesi;
    IRS_FIS_SATIRI->temel_birim_katsayisi               = P_FIS_SATIRI->temel_birim_katsayisi;
    IRS_FIS_SATIRI->hatali_satir_mi                     = P_FIS_SATIRI->hatali_satir_mi;
    IRS_FIS_SATIRI->iade_edilen_miktar                  = P_FIS_SATIRI->iade_edilen_miktar;
    IRS_FIS_SATIRI->temel_birim_katsayisi               = P_FIS_SATIRI->temel_birim_katsayisi;
    IRS_FIS_SATIRI->modul_id                            = FATURA_MODULU;
    IRS_FIS_SATIRI->program_id                          = E9_PROGRAMI;
    IRS_FIS_SATIRI->seri_no_list                        = P_FIS_SATIRI->seri_no_list;
    IRS_FIS_SATIRI->fatura_satiri_id                    = p_fatura_satiri_id;
    IRS_FIS_SATIRI->parti_id                            = P_FIS_SATIRI->parti_id;
    IRS_FIS_SATIRI->parti_kodu                          = P_FIS_SATIRI->parti_kodu;



    f_query.PREPARE_SELECT ( "fat_irs_satir_baglantisi ","irsaliye_satiri_id",
                             "fatura_satiri_id = :fatura_satiri_id" );
    f_query.SET_VALUE      ( ":fatura_satiri_id", p_fatura_satiri_id );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    f_query.NEXT();

    int irsaliye_satiri_id = f_query.VALUE(0).toInt();

    STK_IRS_FIS_STRUCT       * IRS_FIS        = new STK_IRS_FIS_STRUCT;

    STK_IRS_CLEAR_FIS_STRUCT  ( IRS_FIS );

    STK_IRS_FIS_BILGILERINI_OKU  ( IRS_FIS_SATIRI->fis_id, IRS_FIS );

    STK_IRS_FIS_SATIRI_GUNCELLE( IRS_FIS, IRS_FIS_SATIRI, irsaliye_satiri_id );

    delete IRS_FIS;
    delete IRS_FIS_SATIRI;
    //! end
}

/**************************************************************************************
                    FAT_FATURA_IRSALIYESINI_SIL
***************************************************************************************/

void FAT_FATURA_IRSALIYESINI_SIL ( int p_fatura_id  )
{
    SQL_QUERY f_query    ( DB );

    f_query.PREPARE_SELECT("fat_irs_baglantisi","irsaliye_id" ,"fatura_id = :fatura_id");
    f_query.SET_VALUE(":fatura_id", p_fatura_id );
    if ( f_query.SELECT() EQ 0 ) {
        return;
    }
    f_query.NEXT();

    int irsaliye_id = f_query.VALUE(0).toInt();

    STK_IRS_FIS_STRUCT       * IRS_FIS        = new STK_IRS_FIS_STRUCT;

    STK_IRS_CLEAR_FIS_STRUCT  ( IRS_FIS );

    STK_IRS_FIS_BILGILERINI_OKU  ( irsaliye_id, IRS_FIS );

    STK_IRS_FIS_SIL( irsaliye_id, IRS_FIS );

    f_query.PREPARE_DELETE  ( "fat_irs_baglantisi" ,"irsaliye_id = :irsaliye_id" );
    f_query.SET_VALUE       ( ":irsaliye_id", irsaliye_id );
    f_query.DELETE          ();

}

/**************************************************************************************
                    FAT_FATURA_FIS_STRUCT_UPDATE_DB
***************************************************************************************/

void   FAT_FATURA_FIS_STRUCT_UPDATE_DB(FATURA_FISI_STRUCT *P_FIS, int p_fis_id )
{
    /*  DIKKAT
        SADECE FATURA STRUCTINI FISE UPDATE ISLEMI YAPAR
        KONTROL YAPILMAZ, KONTROLLERI DIGER YERDE YAPIP EN SON SAGLIKLI
        OLAN STRUCT GONDERILMELIDIR.
     */

    SQL_QUERY query( DB );

    QString kdv_oran_gore_tutarlar_array = PACK_DOUBLE_ARRAY ( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY,    KDV_OTV_ORANLARI_SAYISI );
    QString otv_oran_gore_tutarlar_array = PACK_DOUBLE_ARRAY ( P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY,    KDV_OTV_ORANLARI_SAYISI );

    query.PREPARE_UPDATE ( "fat_faturalar ","fis_id",
                           "fis_no ,                          "
                           "fatura_tarihi ,                   "
                           "belge_seri ,                      "
                           "belge_numarasi ,                  "
                           "acik_kapali_fatura ,              "
                           "cari_hesap_id  ,                  "
                           "cari_hesap_ismi ,                 "
                           "aciklama ,                        "
                           "cari_hesap_adresi ,               "
                           "vergi_dairesi ,                   "
                           "vergi_numarasi ,                  "
                           "ara_toplam ,                      "
                           "isk_sonrasi_otv_tutari ,          "
                           "isk_sonrasi_otv_dahil_tutar ,     "
                           "isk_sonrasi_kdv_tutari      ,     "
                           "fatura_tutari ,                   "
                           "iskonto_yuzdesi ,                 "
                           "iskonto_tutari ,                  "
                           "doviz_kuru ,                      "
                           "parite ,                          "
                           "cari_para_birimi_tutari ,         "
                           "kdv_oran_gore_tutarlar_array,     "
                           "otv_oran_gore_tutarlar_array ,    "
                           "base_fis_id,                      "
                           "fatura_para_birimi_id,            "
                           "brut_tutar,                       "
                           "isk_oncesi_otv_tutari,            "
                           "isk_oncesi_otv_dahil_tutar,       "
                           "isk_oncesi_kdv_tutari,            "
                           "isk_oncesi_kdv_dahil_tutar,       "
                           "kdv_oran_bilgileri,               "
                           "otv_oran_bilgileri                ",
                           "fis_id  = :fis_id ");

    query.SET_VALUE ( ":fis_no"                           , P_FIS->fis_no                       );
    query.SET_VALUE ( ":fatura_tarihi"                    , P_FIS->fatura_tarihi                );
    query.SET_VALUE ( ":belge_seri"                       , P_FIS->belge_seri                   );
    query.SET_VALUE ( ":belge_numarasi"                   , P_FIS->belge_numarasi               );
    query.SET_VALUE ( ":acik_kapali_fatura"               , P_FIS->acik_kapali_fatura           );
    query.SET_VALUE ( ":cari_hesap_id"                    , P_FIS->cari_hesap_id                );
    query.SET_VALUE ( ":cari_hesap_ismi"                  , P_FIS->cari_hesap_ismi              );
    query.SET_VALUE ( ":aciklama"                         , P_FIS->aciklama                     );
    query.SET_VALUE ( ":cari_hesap_adresi"                , P_FIS->cari_hesap_adresi            );
    query.SET_VALUE ( ":vergi_dairesi"                    , P_FIS->vergi_dairesi                );
    query.SET_VALUE ( ":vergi_numarasi"                   , P_FIS->vergi_numarasi               );
    query.SET_VALUE ( ":ara_toplam"                       , P_FIS->ara_toplam                   );
    query.SET_VALUE ( ":isk_sonrasi_otv_tutari"           , P_FIS->isk_sonrasi_otv_tutari       );
    query.SET_VALUE ( ":isk_sonrasi_otv_dahil_tutar"      , P_FIS->isk_sonrasi_otv_dahil_tutar  );
    query.SET_VALUE ( ":isk_sonrasi_kdv_tutari"           , P_FIS->isk_sonrasi_kdv_tutari       );
    query.SET_VALUE ( ":fatura_tutari"                    , P_FIS->fatura_tutari                );
    query.SET_VALUE ( ":iskonto_yuzdesi"                  , P_FIS->iskonto_yuzdesi              );
    query.SET_VALUE ( ":iskonto_tutari"                   , P_FIS->iskonto_tutari               );
    query.SET_VALUE ( ":doviz_kuru"                       , P_FIS->doviz_kuru                   );
    query.SET_VALUE ( ":parite"                           , P_FIS->parite                       );
    query.SET_VALUE ( ":cari_para_birimi_tutari"          , P_FIS->cari_para_birimi_tutari      );
    query.SET_VALUE ( ":kdv_oran_gore_tutarlar_array"     , kdv_oran_gore_tutarlar_array        );
    query.SET_VALUE ( ":otv_oran_gore_tutarlar_array"     , otv_oran_gore_tutarlar_array        );
    query.SET_VALUE ( ":base_fis_id"                      , P_FIS->base_fis_id                  );
    query.SET_VALUE ( ":fatura_para_birimi_id"            , P_FIS->fatura_para_birimi_id        );
    query.SET_VALUE ( ":brut_tutar",                        P_FIS->brut_tutar                   );
    query.SET_VALUE ( ":isk_oncesi_otv_tutari",             P_FIS->isk_oncesi_otv_tutari        );
    query.SET_VALUE ( ":isk_oncesi_otv_dahil_tutar",        P_FIS->isk_oncesi_otv_dahil_tutar   );
    query.SET_VALUE ( ":isk_oncesi_kdv_tutari",             P_FIS->isk_oncesi_kdv_tutari        );
    query.SET_VALUE ( ":isk_oncesi_kdv_dahil_tutar",        P_FIS->isk_oncesi_kdv_dahil_tutar   );
    query.SET_VALUE ( ":kdv_oran_bilgileri",                P_FIS->kdv_oran_bilgileri           );
    query.SET_VALUE ( ":otv_oran_bilgileri",                P_FIS->otv_oran_bilgileri           );
    query.SET_VALUE ( ":fis_id"                           , p_fis_id                            );

    query.UPDATE();

}

/**************************************************************************************
                    FAT_FATURA_ENT_FISLERINI_OLUSTUR
***************************************************************************************/

QList<int> FAT_FATURA_ENT_FISLERINI_OLUSTUR( FATURA_FISI_STRUCT *P_FIS, int p_fis_id, ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB = DB;
    if ( P_GELEN_DB NE NULL ) {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query( USE_DB );

    QList<int> fis_no_list;

    MUH_FIS_STRUCT        * MUH_FIS = new MUH_FIS_STRUCT;
    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

    MUH_CLEAR_FIS_STRUCT( MUH_FIS );
    MUH_CLEAR_FIS_SATIRI_STRUCT( MUH_FIS_SATIRI );

    CARI_FIS_STRUCT        * CARI_FIS = new CARI_FIS_STRUCT;
    CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI = new CARI_FIS_SATIRI_STRUCT;

    CARI_CLEAR_FIS_STRUCT ( CARI_FIS );
    CARI_CLEAR_FIS_SATIRI_STRUCT ( CARI_FIS_SATIRI );

    //! [1] ONCEKI MUHASEBE ENT FISLERI SILINIYOR
    if ( P_FIS->muh_fis_id > 0 ) {
        MUH_FIS_BILGILERINI_OKU ( MUH_FIS, P_FIS->muh_fis_id, USE_DB );
        MUH_FIS_TUM_SATIRLARINI_SIL ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);
    }
    //! [1]

    int temel_para_birimi_id = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

    //! [2] CARI DOVIZ KURU
    double cari_doviz_kuru      = 0.00;
    double fat_icin_doviz_kuru  = P_FIS->doviz_kuru;

    if( P_FIS->fatura_para_birimi_id EQ P_FIS->cari_para_birimi_id  ) {
        if( P_FIS->fatura_para_birimi_id NE temel_para_birimi_id ) {
            cari_doviz_kuru = P_FIS->doviz_kuru;
        }
        else {
            cari_doviz_kuru = 1.000;
        }
    }
    else {
        if( P_FIS->cari_para_birimi_id EQ temel_para_birimi_id ) {
            cari_doviz_kuru = 1.000;
        }
        else {
            if( P_FIS->cari_para_birimi_id NE temel_para_birimi_id AND P_FIS->fatura_para_birimi_id EQ temel_para_birimi_id ) {
                cari_doviz_kuru      = ROUND( 1 / P_FIS->doviz_kuru , 4 ); // ex : 1TL -> 0.6 $  oldugu icin ters cevrildi
                fat_icin_doviz_kuru  = 1.0000; // fatura TL ise kur 1.000 dir
            }
            else if( P_FIS->fatura_para_birimi_id EQ temel_para_birimi_id ){
                cari_doviz_kuru = P_FIS->doviz_kuru;
            }
            else {
                cari_doviz_kuru = ROUND( P_FIS->doviz_kuru *  ( 1 / P_FIS->parite ) , 4 ); // TL / E * 1 / ( E / $ )= TL / $
            }
        }
    }
    //! [2] END CARI DOVIZ KURU

    //! MUHASEBE FIS BILGISI OKUNUYOR
    MUH_FIS_BILGILERINI_OKU( MUH_FIS, P_FIS->muh_fis_id, USE_DB );

    //! [3] CARI FIS KONTROL EDILIYOR VAR ISE OKUNUYOR YOK ISE OLUSTURULUYOR
    int cari_fis_id = CARI_GET_ENT_FIS_ID( P_FIS->program_id, P_FIS->modul_id, p_fis_id, USE_DB );

    if ( cari_fis_id EQ 0 ) {

        CARI_FIS->cari_hesap_id         = P_FIS->cari_hesap_id;
        CARI_FIS->modul_id              = P_FIS->modul_id;
        CARI_FIS->modul_id              = P_FIS->modul_id;
        CARI_FIS->modul_id              = P_FIS->modul_id;
        CARI_FIS->program_id            = P_FIS->program_id;
        CARI_FIS->fis_tarihi            = P_FIS->fatura_tarihi;
        CARI_FIS->aciklama              = QObject::tr ( "Fiş FATURA MODÜLÜ tarafından üretilmiştir. Fiş No = %1 \n" ) .arg ( QVariant ( P_FIS->fis_no ).toString() ) + P_FIS->aciklama;
        CARI_FIS->fis_turu              = ENUM_CARI_HAREKET_FISI;
        CARI_FIS->makbuz_para_birimi_id = P_FIS->cari_para_birimi_id;
        CARI_FIS->doviz_kuru            = cari_doviz_kuru;
        CARI_FIS->parite                = 1.000; // carinin para birimi cinsinden gittigi icin 1.000 olmali
        CARI_FIS->base_fis_id           = p_fis_id;
        CARI_FIS->muh_fis_id            = P_FIS->muh_fis_id;

        cari_fis_id = CARI_FIS_EKLE ( CARI_FIS ,USE_DB);
    }
    else {
        CARI_FIS_BILGILERINI_OKU ( CARI_FIS, cari_fis_id ,USE_DB);
        CARI_FIS->muh_fis_id = P_FIS->muh_fis_id;
    }
    fis_no_list.append( CARI_FIS->fis_no );
    fis_no_list.append( MUH_FIS->fis_no  );
    //! [3]


    //! [4]FATURA FIS BAZINDA ENTEGRE OLDUGU ICIN ONCEKI ENT FIS SATIRLARI SILINECEK
    QStringList CARI_FIS_SATIRI_ID_LIST = CARI_GET_ENT_ROW_ID_LIST  ( P_FIS->program_id, P_FIS->modul_id, p_fis_id, USE_DB);
    if ( CARI_FIS_SATIRI_ID_LIST.size() > 0 ) {
        for ( int i = 0; i < CARI_FIS_SATIRI_ID_LIST.size(); i++ ) {
            CARI_FIS_SATIRI_SIL ( CARI_FIS, QVariant ( CARI_FIS_SATIRI_ID_LIST.at (i) ).toInt(), USE_DB );
        }
    }
    //! [4]

    double cari_fis_satiri_alacak_tutari    = 0.0;
    double cari_fis_satiri_borc_tutari      = 0.0;

    if ( P_FIS->fatura_alis_satis_turu EQ ENUM_SATIS_IRSALIYESI_FATURASI ) {
        cari_fis_satiri_borc_tutari = P_FIS->cari_para_birimi_tutari;
        cari_fis_satiri_alacak_tutari = 0.00;
    }
    else { //! ENUM_ALIS_IRSALIYESI_FATURASI
        cari_fis_satiri_alacak_tutari = P_FIS->cari_para_birimi_tutari;
        cari_fis_satiri_borc_tutari = 0.00;
    }

    //! IADE FATURASI ISE TUTARLAR TERSLENECEK
    if ( P_FIS->iade_faturasi_mi EQ 1 ) {
        double gecici_degisken = cari_fis_satiri_borc_tutari;
        cari_fis_satiri_borc_tutari   = cari_fis_satiri_alacak_tutari;
        cari_fis_satiri_alacak_tutari = gecici_degisken;
    }

    //! [5]CARI ENT FIS SATIRI OLUSTURULUYOR
    QString aciklama = P_FIS->fatura_tarihi + " / " + QVariant ( P_FIS->fis_no ).toString() + " / " +
                       P_FIS->belge_seri + "  " + QVariant ( P_FIS->belge_numarasi ).toString() + " " + P_FIS->aciklama.mid(0, 30 ) + "...";

    CARI_FIS_SATIRI->aciklama            =  aciklama;
    CARI_FIS_SATIRI->doviz_kodu          =  DVZ_GET_DOVIZ_KODU( P_FIS->cari_para_birimi_id );
    CARI_FIS_SATIRI->fis_id              =  cari_fis_id;
    CARI_FIS_SATIRI->borc_tutari         =  cari_fis_satiri_borc_tutari;
    CARI_FIS_SATIRI->alacak_tutari       =  cari_fis_satiri_alacak_tutari;
    CARI_FIS_SATIRI->cari_hesap_id       =  P_FIS->cari_hesap_id;
    CARI_FIS_SATIRI->islem_turu          = -1;
    CARI_FIS_SATIRI->doviz_id            =  P_FIS->cari_para_birimi_id;
    CARI_FIS_SATIRI->doviz_kuru          =  cari_doviz_kuru;
    CARI_FIS_SATIRI->parite              =  1.000;
    CARI_FIS_SATIRI->modul_id            =  P_FIS->modul_id;
    CARI_FIS_SATIRI->program_id          =  P_FIS->program_id;
    CARI_FIS_SATIRI->base_record_id      =  p_fis_id;

    int cari_olusturdugu_muh_fis_satiri_id = 0;
    CARI_FIS_SATIRI_EKLE ( CARI_FIS, CARI_FIS_SATIRI , true, 0, &cari_olusturdugu_muh_fis_satiri_id, USE_DB);

    //! [5]

    //! [6] MUHASEBE ENT FIS SATIRLARI OLUSTURULACAK

    int ticari_mallar_hesap_id = 0;

    int last_order_number =  MUH_GET_FIS_LAST_ORDER_NUMBER( P_FIS->muh_fis_id ,USE_DB);

    aciklama = "Fatura : " + QVariant ( P_FIS->fis_no ).toString() + " / " + P_FIS->belge_seri + " " +
               QVariant ( P_FIS->belge_numarasi ).toString() + " / " + P_FIS->cari_hesap_ismi;

    MUH_FIS_SATIRI->aciklama              = aciklama;
    MUH_FIS_SATIRI->base_record_id        = p_fis_id;
    MUH_FIS_SATIRI->modul_id              = FATURA_MODULU;
    MUH_FIS_SATIRI->program_id            = E9_PROGRAMI;

    switch ( P_FIS->fatura_alis_satis_turu ) {

        case ENUM_ALIS_IRSALIYESI_FATURASI :

            //! HER URUNUN KENDISINE OZGU MUHASEBE HESABI OLDUGU ICIN BUTUN SATIRLADAKI URUNLER TARANIYOR.
            query.PREPARE_SELECT("fat_fatura_satirlari", "urun_id, satir_tutari", "fis_id = :fis_id" );
            query.SET_VALUE     (":fis_id", p_fis_id );

            if ( query.SELECT() EQ 0 ) {
                return fis_no_list;
            }

            while ( query.NEXT() EQ true ) {

                int    urun_id      = query.VALUE ( 0 ).toInt();
                double satir_tutari = query.VALUE ( 1 ).toDouble();

                if ( urun_id > 0 ) {

                    STK_URUNLER_STRUCT * URUN = new STK_URUNLER_STRUCT;

                    STK_URUNLER_TABLOSUNU_OKU( urun_id, URUN, false, USE_DB);

                    ticari_mallar_hesap_id = URUN->muh_hesap_id;

                    MUH_FIS_SATIRI->hesap_id = ticari_mallar_hesap_id;
                    if ( ticari_mallar_hesap_id < 1 ) {
                        MUH_FIS->aciklama.append ( URUN->urun_kodu + " " + URUN->urun_adi + QObject::tr ( " ürünü için, \n stok kartında ticari mallar muhasebe hesabı tanımlı olmadığından fiş oluşturulmadı.") );
                        MUH_FIS->aciklama.append ( "Fatura : " + P_FIS->belge_seri + " " + QVariant ( P_FIS->belge_numarasi ).toString() + " / Fiş No : " + QVariant (P_FIS->fis_no ).toString() + " / " + P_FIS->cari_hesap_ismi );
                        MUH_FISINI_HATALI_FISE_DONUSTUR ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);
                        return fis_no_list;
                    }
                    delete URUN;
                }
                else {
                    MUH_FIS_SATIRI->hesap_id = P_FIS->hizmet_urt_maliyeti_hesap_id;
                }

                MUH_FIS_SATIRI->aciklama      = aciklama;

                if ( P_FIS->iade_faturasi_mi EQ 0 ) {
                    MUH_FIS_SATIRI->alacak_tutari = 0.0;
                    MUH_FIS_SATIRI->borc_tutari   = ROUND( satir_tutari * fat_icin_doviz_kuru );
                }
                else {
                    MUH_FIS_SATIRI->alacak_tutari = ROUND( satir_tutari * fat_icin_doviz_kuru );
                    MUH_FIS_SATIRI->borc_tutari   = 0.0;
                }
                MUH_FIS_SATIRI->fis_id       = P_FIS->muh_fis_id;
                MUH_FIS_SATIRI->order_number = last_order_number - 1;

                MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI, USE_DB );
            }
            break;

        case ENUM_SATIS_IRSALIYESI_FATURASI : {

            if ( P_FIS->iade_faturasi_mi EQ 1 ) {
                MUH_FIS_SATIRI->hesap_id      = P_FIS->satistan_iade_hesap_id;
                MUH_FIS_SATIRI->alacak_tutari = 0.0;
                MUH_FIS_SATIRI->borc_tutari   = ROUND( P_FIS->brut_tutar * fat_icin_doviz_kuru );
            }
            else {
                MUH_FIS_SATIRI->hesap_id      = P_FIS->yurtici_satislar_hesap_id;
                MUH_FIS_SATIRI->alacak_tutari = ROUND( P_FIS->brut_tutar * fat_icin_doviz_kuru );
                MUH_FIS_SATIRI->borc_tutari   = 0.0;
            }

            MUH_FIS_SATIRI->fis_id = P_FIS->muh_fis_id;
            MUH_FIS_SATIRI->order_number = last_order_number + 1;

            MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI, USE_DB );

            break;
        }
        default :
            break;
    }
    //! [6]

    //! KAPALI FATURA ISE CARI 2. BIR SATIR EKLIYORUZ.
    if ( P_FIS->acik_kapali_fatura EQ ENUM_KAPALI_FATURA ) {
        CARI_FIS_SATIRI->muh_hesap_id  = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();
        CARI_FIS_SATIRI->borc_tutari   =  cari_fis_satiri_alacak_tutari;
        CARI_FIS_SATIRI->alacak_tutari =  cari_fis_satiri_borc_tutari;

        CARI_FIS_SATIRI_EKLE( CARI_FIS, CARI_FIS_SATIRI, true, 0, NULL, USE_DB);
    }

    //! [7] MUHASEBEYE KDV, OTV VE TEVKIFAT ICIN ENTEGRASYON EKLER
    MUH_FIS_SATIRI->aciklama = aciklama;

    int KDV_HES_ID_ARRAY[KDV_OTV_ORANLARI_SAYISI];
    int KDV_IND_ID_ARRAY[KDV_OTV_ORANLARI_SAYISI];

    SQL_QUERY s_query( DB );

    s_query.PREPARE_SELECT( "e9_sabit_degerler", "ind_kdv_hesap_id_array, hes_kdv_hesap_id_array");

    if ( s_query.SELECT() NE 0 ) {
        s_query.NEXT();
        UNPACK_INT_ARRAY( s_query.VALUE( 0 ).toString(), KDV_IND_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI  );
        UNPACK_INT_ARRAY( s_query.VALUE( 1 ).toString(), KDV_HES_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI  );

    }

    for ( int i = 0; i < P_FIS->kdv_orani_sayisi; i++ ) {

        if ( P_FIS->KDV_ORANLARI_ARRAY[i] EQ 0 OR P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] EQ 0 ) {
            continue;
        }

        MUH_FIS_SATIRI->hesap_id = P_FIS->KDV_HESAPLARI_ID_ARRAY[i];

        // IADE FATURALARINDA ISLEM FARKLI CALISIYOR.
        // hesap id farkli calisiyor. BIRINDE 191 - 391
        if( P_FIS->iade_faturasi_mi EQ ENUM_FAT_IADE ) {
            if ( P_FIS->fatura_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
                MUH_FIS_SATIRI->hesap_id = KDV_HES_ID_ARRAY[i];
            }
            else {
                MUH_FIS_SATIRI->hesap_id = KDV_IND_ID_ARRAY[i];
            }
        }

        if ( P_FIS->KDV_HESAPLARI_ID_ARRAY[i] < 1 ) {
            MUH_FIS->aciklama.append ( QString ( "\n% %1 KDV muhasebe hesabı bulunmadı " ).arg ( P_FIS->KDV_ORANLARI_ARRAY[i] ) );
            MUH_FIS->aciklama.append ( "\nFatura : " + P_FIS->belge_seri + " " + QVariant ( P_FIS->belge_numarasi ).toString() +
                                       " / Fiş No : " + QVariant ( P_FIS->fis_no ).toString() + " / " + P_FIS->cari_hesap_ismi );
            MUH_FISINI_HATALI_FISE_DONUSTUR ( MUH_FIS, P_FIS->muh_fis_id , USE_DB);
            return fis_no_list;
        }

        switch ( P_FIS->fatura_alis_satis_turu ) {

            case ENUM_ALIS_IRSALIYESI_FATURASI :
                if ( P_FIS->iade_faturasi_mi EQ 0 ) {
                    MUH_FIS_SATIRI->alacak_tutari = 0.0;
                    MUH_FIS_SATIRI->borc_tutari   = ROUND( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] * fat_icin_doviz_kuru );
                }
                else {
                    MUH_FIS_SATIRI->alacak_tutari = ROUND( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] * fat_icin_doviz_kuru );
                    MUH_FIS_SATIRI->borc_tutari   = 0.0;
                }
                break;
            case ENUM_SATIS_IRSALIYESI_FATURASI :
                if ( P_FIS->iade_faturasi_mi EQ 0 ) {
                    MUH_FIS_SATIRI->alacak_tutari = ROUND ( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] * ( 1 - P_FIS->tevkifat_yuzdesi / 100 ) * fat_icin_doviz_kuru );
                    MUH_FIS_SATIRI->borc_tutari   = 0.0;
                }
                else {
                    MUH_FIS_SATIRI->alacak_tutari = 0.0;
                    MUH_FIS_SATIRI->borc_tutari   = ROUND ( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] * ( 1 - P_FIS->tevkifat_yuzdesi / 100 ) * fat_icin_doviz_kuru );
                }
                break;
            default :
                break;
        }

        MUH_FIS_SATIRI->order_number = 0;

        MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI ,USE_DB);

        if ( P_FIS->tevkifatli_fatura_mi EQ 1 ) {

            switch ( P_FIS->fatura_alis_satis_turu ) {
                case ENUM_ALIS_IRSALIYESI_FATURASI :
                    MUH_FIS_SATIRI->hesap_id = P_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY[i];

                    if ( P_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY[i] < 1 ) {
                        MUH_FIS->aciklama.append ( QString ( "\n % %1 Tevkifat KDV muhasebe hesabı bulunmadı " ).arg ( P_FIS->KDV_ORANLARI_ARRAY[i] ) );
                        MUH_FIS->aciklama.append ( "\nFatura : " + P_FIS->belge_seri + " " + QVariant ( P_FIS->belge_numarasi ).toString() + " / Fiş No : "
                                                   + QVariant ( P_FIS->fis_no ).toString() + " / " + P_FIS->cari_hesap_ismi );
                        MUH_FISINI_HATALI_FISE_DONUSTUR ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);
                        return fis_no_list;
                    }
                    if ( P_FIS->iade_faturasi_mi EQ 0 ) {
                        MUH_FIS_SATIRI->borc_tutari     = 0.0;
                        MUH_FIS_SATIRI->alacak_tutari   = ROUND ( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] * ( P_FIS->tevkifat_yuzdesi / 100 ) * fat_icin_doviz_kuru );
                    }
                    else {
                        MUH_FIS_SATIRI->borc_tutari     = ROUND ( P_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] * ( P_FIS->tevkifat_yuzdesi / 100 ) * fat_icin_doviz_kuru);
                        MUH_FIS_SATIRI->alacak_tutari   = 0.0;
                    }
                    MUH_FIS_SATIRI->order_number = 0;
                    MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI ,USE_DB);
                    break;

                case ENUM_SATIS_IRSALIYESI_FATURASI :
                    break;

                default :
                    break;
            }
        }
    }

    for ( int i = 0; i < P_FIS->otv_orani_sayisi; i++ ) {

        if ( P_FIS->OTV_ORANLARI_ARRAY[i] EQ 0 OR P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] EQ 0 ) {
            continue;
        }
        MUH_FIS_SATIRI->hesap_id = P_FIS->OTV_HESAPLARI_ID_ARRAY[i];

        if ( P_FIS->OTV_HESAPLARI_ID_ARRAY[i] < 1 ) {
            MUH_FIS->aciklama.append ( QString ( "\n% %1 ÖTV muhasebe hesabı bulunmadı " ).arg ( P_FIS->OTV_ORANLARI_ARRAY[i] ) );
            MUH_FIS->aciklama.append ( "\nFatura : " + P_FIS->belge_seri + " " + QVariant ( P_FIS->belge_numarasi ).toString() +
                                       " / Fiş No : " + QVariant ( P_FIS->fis_no ).toString() + " / " + P_FIS->cari_hesap_ismi );
            MUH_FISINI_HATALI_FISE_DONUSTUR ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);
            return fis_no_list;
        }

        switch ( P_FIS->fatura_alis_satis_turu ) {

            case ENUM_ALIS_IRSALIYESI_FATURASI :
                if ( P_FIS->iade_faturasi_mi EQ 0 ) {
                    MUH_FIS_SATIRI->alacak_tutari = 0.0;
                    MUH_FIS_SATIRI->borc_tutari   = ROUND( P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] * fat_icin_doviz_kuru );
                }
                else {
                    MUH_FIS_SATIRI->alacak_tutari = ROUND( P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] * fat_icin_doviz_kuru );
                    MUH_FIS_SATIRI->borc_tutari   = 0.0;
                }

                break;

            case ENUM_SATIS_IRSALIYESI_FATURASI :
                if ( P_FIS->iade_faturasi_mi EQ 0 ) {
                    MUH_FIS_SATIRI->alacak_tutari = ROUND( P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] * fat_icin_doviz_kuru );
                    MUH_FIS_SATIRI->borc_tutari   = 0.0;
                }
                else {
                    MUH_FIS_SATIRI->alacak_tutari = 0.0;
                    MUH_FIS_SATIRI->borc_tutari   = ROUND( P_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] * fat_icin_doviz_kuru );
                }
                break;
            default :
                break;
        }
        MUH_FIS_SATIRI->order_number = 0;
        MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI ,USE_DB);
    }
    //! [7]

    //! EGER MUHASEBEDE KURUS KAYBI YASANDI ISE KURUS KAYBI KADAR CARIDEN ARTIRIP VEYA AZALTACAK
    //! KUSURAT KAYBI VAR
    //! [8]
    if ( MUH_FIS->toplam_borc NE MUH_FIS->toplam_alacak ) {

        MUH_FIS_SATIRINI_OKU( MUH_FIS_SATIRI, cari_olusturdugu_muh_fis_satiri_id, USE_DB );

        double fis_sapmasi = ROUND( MUH_FIS->toplam_alacak - MUH_FIS->toplam_borc );

        if( P_FIS->fatura_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
            MUH_FIS_SATIRI->alacak_tutari -= fis_sapmasi;
        }
        else {
            MUH_FIS_SATIRI->borc_tutari += fis_sapmasi;
        }

        MUH_FIS_SATIRINI_GUNCELLE( MUH_FIS, MUH_FIS_SATIRI, cari_olusturdugu_muh_fis_satiri_id, USE_DB );
    }
    //! [8]

    return fis_no_list; // 1 - > cari_fis_no  2 - > muhasebe fis no
}
