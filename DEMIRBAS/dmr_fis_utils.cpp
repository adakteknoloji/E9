#include <QDate>
#include "dmr_fis_utils.h"
#include "adak_defines.h"
#include "adak_sql.h"
#include "dmr_con_utils.h"
#include "muh_fis_utils.h"
#include "muh_enum.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "fat_fis_utils.h"
#include "adak_utils.h"
#include "isl_fis_utils.h"
#include "isl_enum.h"
#include "dmr_enum.h"

extern ADAK_SQL * DB;



/***************************************************************************************
                   DMR_FIS_EKLE
***************************************************************************************/

int DMR_FIS_EKLE(DMR_FIS_STRUCT *P_FIS, ADAK_SQL *P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    if ( P_FIS->fis_no EQ 0 ) {
        P_FIS->fis_no = DMR_SIRADAKI_FIS_NO_AL(P_FIS->fis_tarihi);
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_INSERT("dmr_fisler" , "fis_id" , "fis_no , fis_tarihi , aciklama , fis_turu,"
                         "toplam_tutar , toplam_birikmis_amor_tutari , degerleme_orani,"
                         "dusulecek_amortisman_yuzdesi , modul_id , program_id , "
                         "base_fis_id , muh_fis_id , last_order_number");

    query.SET_VALUE(":fis_no"                        , P_FIS->fis_no );
    query.SET_VALUE(":fis_tarihi"                    , P_FIS->fis_tarihi);
    query.SET_VALUE(":aciklama"                      , P_FIS->aciklama  );
    query.SET_VALUE(":fis_turu"                      , P_FIS->fis_turu  );
    query.SET_VALUE(":toplam_tutar"                  , 0);
    query.SET_VALUE(":toplam_birikmis_amor_tutari"   , 0);
    query.SET_VALUE(":degerleme_orani"               , P_FIS->degerleme_orani );
    query.SET_VALUE(":dusulecek_amortisman_yuzdesi"  , P_FIS->dusulecek_amor_yuzdesi);
    query.SET_VALUE(":modul_id"                      , P_FIS->modul_id);
    query.SET_VALUE(":program_id"                    , P_FIS->program_id);
    query.SET_VALUE(":base_fis_id"                   , P_FIS->base_fis_id);
    query.SET_VALUE(":muh_fis_id"                    , P_FIS->muh_fis_id );
    query.SET_VALUE(":last_order_number"             , 0);

    int fis_id = query.INSERT();

    if ( P_FIS->modul_id EQ DEMIRBAS_MODULU ) {
        MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT(MUH_FIS);

        QString aciklama = QObject::tr("Fiş SABİT KIYMET modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
        aciklama += P_FIS->aciklama;

        MUH_FIS->aciklama           = aciklama;
        MUH_FIS->fis_tarihi         = P_FIS->fis_tarihi;
        MUH_FIS->fis_turu           = ENUM_MAHSUP_FISI;
        MUH_FIS->modul_id           = P_FIS->modul_id;
        MUH_FIS->program_id         = P_FIS->program_id;
        MUH_FIS->base_fis_id        = fis_id;

        P_FIS->muh_fis_id = MUH_FIS_EKLE ( MUH_FIS , USE_DB);

        delete MUH_FIS;

        query.PREPARE_UPDATE("dmr_fisler","fis_id", "muh_fis_id", "fis_id = :fis_id");
        query.SET_VALUE     (":muh_fis_id" , P_FIS->muh_fis_id );
        query.SET_VALUE     (":fis_id"     , fis_id);
        query.UPDATE();


    }

    return fis_id;
}

/***************************************************************************************
                   DMR_FIS_SATIRI_EKLE
***************************************************************************************/

int DMR_FIS_SATIRI_EKLE(DMR_FIS_STRUCT *P_FIS, DMR_FIS_SATIRI_STRUCT *P_FIS_SATIRI, ADAK_SQL *P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("dmr_fisler" , "modul_id , program_id,base_fis_id,"
                         "last_order_number , toplam_tutar , fis_tarihi , "
                         "toplam_birikmis_amor_tutari","fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , P_FIS_SATIRI->fis_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    int modul_id    = query.VALUE(0).toInt();
    int program_id  = query.VALUE(1).toInt();
    int base_fis_id = query.VALUE(2).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id
         OR P_FIS->base_fis_id NE base_fis_id ) {
        return 0;
    }
    int    last_order_number            = query.VALUE(3).toInt();
    double toplam_tutar                 = query.VALUE(4).toDouble();
    QDate  fis_tarihi                   = QDate::fromString(query.VALUE(5).toString(),"yyyy.MM.dd");
    double toplam_bir_amortisman_tutari = query.VALUE(6).toDouble();

    if ( P_FIS_SATIRI->order_number EQ 0 ) {
        P_FIS_SATIRI->order_number = last_order_number + 1024;
    }

    query.PREPARE_SELECT ( "dmr_demirbaslar","yillik_ayrilan_amor_yuzdesi,toplamda_ayrilan_amor_yuzdesi, "
                           "amortisman_orani,demirbasin_yeni_degeri ,"
                           "yeni_brkms_amortisman_tutari , yeniden_degerleme_yapildi ,"
                           "muh_yili_ayrilacak_amor_tutari , amortisman_yontemi,"
                           "kist_amortisman , alis_tarihi , amortisman_suresi,"
                           "demirbasin_degeri , amortisman_ayrilabilir_mi,bir_amor_hesap_id,"
                           "amor_gider_hesap_id , fatura_id ,muh_devir_yili_ayrilacak_amor",
                           "demirbas_id = :demirbas_id");
    query.SET_VALUE      ( ":demirbas_id" , P_FIS_SATIRI->demirbas_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }


    query.NEXT();

    double yillik_ayrilan_amortisman_yuzdesi   = query.VALUE(0).toDouble();
    double toplamda_ayrilan_amortisman_yuzdesi = query.VALUE(1).toDouble();
    double amortisman_orani                    = query.VALUE(2).toDouble();
    double demirbasin_yeni_degeri              = query.VALUE(3).toDouble();
    double yeni_brkms_amortisman_tutari        = query.VALUE(4).toDouble();
    int    yeniden_degerleme_yapildi           = query.VALUE(5).toInt();
    double muh_yili_ayrilacak_amortisman_tutari= query.VALUE(6).toDouble();
    int    amortisman_yontemi                  = query.VALUE(7).toInt();
    int    kist_amortisman_uygulanacak_mi      = query.VALUE(8).toInt();
    QDate  alis_tarihi                         = QDate::fromString(query.VALUE(9).toString() , "yyyy.MM.dd");
    int    amortisman_suresi                   = query.VALUE(10).toInt();
    double demirbasin_degeri                   = query.VALUE(11).toDouble();
    int    amortisman_ayrilabilir_mi           = query.VALUE(12).toInt();
    int    bir_amortismanlar_hesap_id          = query.VALUE(13).toInt();
    int    amor_gider_hesap_id                 = query.VALUE(14).toInt();
    int    fatura_id                           = query.VALUE(15).toInt();
    double muh_devir_yili_ayrilacak_amor_tutari= query.VALUE("muh_devir_yili_ayrilacak_amor").toDouble();

    int gun_no = MALI_YIL_ARRAY_INDIS ( fis_tarihi );

    if ( P_FIS_SATIRI->birikmis_amor_tutari EQ 0 ) {
        P_FIS_SATIRI->birikmis_amor_tutari = DMR_GET_BIRIKMIS_AMORTISMAN_TUTARI(P_FIS_SATIRI->demirbas_id , gun_no);
    }

    if ( P_FIS->fis_turu EQ YENIDEN_DEGERLEME_FISI ) {
        if ( P_FIS_SATIRI->satir_tutari EQ 0 ) {
            demirbasin_yeni_degeri = demirbasin_degeri * ( 1 + ( P_FIS->degerleme_orani / 100));
            P_FIS_SATIRI->satir_tutari = demirbasin_yeni_degeri;
        }
        else {
            demirbasin_yeni_degeri       = P_FIS_SATIRI->satir_tutari;
        }

        if ( P_FIS_SATIRI->yeni_birikmis_amor_tutari EQ 0 ) {
            yeni_brkms_amortisman_tutari = P_FIS_SATIRI->birikmis_amor_tutari
                                           * ( 1 + ( P_FIS->degerleme_orani / 100));
        }
        else {
            yeni_brkms_amortisman_tutari = P_FIS_SATIRI->yeni_birikmis_amor_tutari;
        }
        yeniden_degerleme_yapildi    = 1;
    }
    else { //AMORTISMAN FISI

        yillik_ayrilan_amortisman_yuzdesi += P_FIS->dusulecek_amor_yuzdesi;

        if ( yillik_ayrilan_amortisman_yuzdesi EQ 100 ) { //Eger yillik amortismanin tamami ayrilmissa\ayni yil icinde bir daha amortisman ayrilamaz.

            toplamda_ayrilan_amortisman_yuzdesi += amortisman_orani;

           if ( toplamda_ayrilan_amortisman_yuzdesi EQ 100) {
              amortisman_ayrilabilir_mi = 0; // Eger demirbasin tum amortismani ayrilmmissa,demirbastan bir daha amortisman ayrilamaz.
           }

        }

        if ( muh_yili_ayrilacak_amortisman_tutari EQ 0 OR muh_devir_yili_ayrilacak_amor_tutari EQ 0) {
            //Muhasebe yilinda ayrilacak olan amortisman tutari henuz hesaplanmamissa

            if ( fatura_id > 0 ) {

                FATURA_FISI_STRUCT * FAT_FIS = new FATURA_FISI_STRUCT;



                FAT_FIS->KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
                FAT_FIS->KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
                FAT_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

                FAT_FIS->OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
                FAT_FIS->OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
                FAT_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

                FAT_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];

                FATURA_CLEAR_FIS_STRUCT ( FAT_FIS );

                FATURA_FIS_BILGILERINI_OKU( FAT_FIS, fatura_id );

                double fatura_tutari = FAT_FIS->fatura_tutari;
                double kdv_tutari    = FAT_FIS->isk_sonrasi_kdv_tutari;

                double kdv_haric_tutar = fatura_tutari - kdv_tutari;

                if (  amortisman_yontemi EQ NORMAL_BAKIYE ) {
                    muh_yili_ayrilacak_amortisman_tutari = kdv_haric_tutar * ( amortisman_orani/100);
                }
                else {
                    muh_yili_ayrilacak_amortisman_tutari = ( fatura_tutari - (demirbasin_yeni_degeri - yeni_brkms_amortisman_tutari))
                                                                       * ( ( amortisman_orani * 2)/100);
                }
            }
            else {
                if ( amortisman_yontemi EQ NORMAL_BAKIYE ) {
                    muh_yili_ayrilacak_amortisman_tutari = demirbasin_yeni_degeri * ( amortisman_orani / 100);
                }
                else {
                    muh_yili_ayrilacak_amortisman_tutari = (demirbasin_yeni_degeri - yeni_brkms_amortisman_tutari) * ((amortisman_orani * 2)/100);
                }

            }

            muh_devir_yili_ayrilacak_amor_tutari = muh_yili_ayrilacak_amortisman_tutari;


            //KIST AMORTISMAN VAR ISE,ILK YIL BINEK OTOMOBILIN FIRMAYA GIRDIGI AY DAHIL OLMAK UZERE YIL SONUNA KADAR KALAN TUM AYLAR ICIN,
            //SON YIL ISE ILK YILDAN AMORTISMAN AYRILMAYAN AYLAR VE SON YILIN TAMAMI ICIN AMORTISMAN AYRILIR.



            if ( kist_amortisman_uygulanacak_mi EQ 1 ) {


                int demirbasin_alindigi_ay = alis_tarihi.month();

                if ( alis_tarihi.year() EQ fis_tarihi.year() ) {
                     muh_yili_ayrilacak_amortisman_tutari = ( muh_yili_ayrilacak_amortisman_tutari/12) * ((12-demirbasin_alindigi_ay) + 1);
                }

                if ( ( alis_tarihi.year() + amortisman_suresi) EQ fis_tarihi.year() ) {
                     double ayrilacak_amortisman_tutari = (muh_yili_ayrilacak_amortisman_tutari/12) * (demirbasin_alindigi_ay-1);

                     muh_yili_ayrilacak_amortisman_tutari += ayrilacak_amortisman_tutari;

                }

                if ( ( alis_tarihi.year() + amortisman_suresi) EQ fis_tarihi.year() + 1 ) {
                    double ayrilacak_amortisman_tutari = (muh_yili_ayrilacak_amortisman_tutari/12) * (demirbasin_alindigi_ay-1);

                    muh_yili_ayrilacak_amortisman_tutari += ayrilacak_amortisman_tutari;
                }

            }

        }

        if ( P_FIS_SATIRI->satir_tutari EQ 0 ) {

            if ( fis_tarihi.year() EQ MALI_YIL_FIRST_DATE().year() ) {

                P_FIS_SATIRI->satir_tutari = muh_yili_ayrilacak_amortisman_tutari *  ( P_FIS->dusulecek_amor_yuzdesi / 100);
            }
            else {
                P_FIS_SATIRI->satir_tutari = muh_devir_yili_ayrilacak_amor_tutari *  ( P_FIS->dusulecek_amor_yuzdesi / 100);
            }
        }

        DMR_AMORTISMAN_TUTARINI_ARTTIR ( P_FIS_SATIRI->demirbas_id,gun_no,
                                     P_FIS_SATIRI->satir_tutari );
    }


    query.PREPARE_UPDATE ( "dmr_demirbaslar","demirbas_id",
                                  "yillik_ayrilan_amor_yuzdesi  , "
                                  "toplamda_ayrilan_amor_yuzdesi ,"
                                  "amortisman_ayrilabilir_mi,  "
                                  "yeni_brkms_amortisman_tutari,"
                                  "demirbasin_yeni_degeri,"
                                  "yeniden_degerleme_yapildi,"
                                  "muh_yili_ayrilacak_amor_tutari,"
                                  "muh_devir_yili_ayrilacak_amor",
                                  "demirbas_id = :demirbas_id" );

    query.SET_VALUE      ( ":yillik_ayrilan_amor_yuzdesi"       , yillik_ayrilan_amortisman_yuzdesi );
    query.SET_VALUE      ( ":toplamda_ayrilan_amor_yuzdesi"     , toplamda_ayrilan_amortisman_yuzdesi ) ;
    query.SET_VALUE      ( ":amortisman_ayrilabilir_mi"         , amortisman_ayrilabilir_mi );
    query.SET_VALUE      ( ":yeni_brkms_amortisman_tutari"      , yeni_brkms_amortisman_tutari);
    query.SET_VALUE      ( ":demirbasin_yeni_degeri"            , demirbasin_yeni_degeri);
    query.SET_VALUE      ( ":yeniden_degerleme_yapildi"         , yeniden_degerleme_yapildi);
    query.SET_VALUE      ( ":muh_yili_ayrilacak_amor_tutari"    , ROUND(muh_yili_ayrilacak_amortisman_tutari,-2));
    query.SET_VALUE      ( ":muh_devir_yili_ayrilacak_amor"     , ROUND(muh_devir_yili_ayrilacak_amor_tutari,-2));
    query.SET_VALUE      ( ":demirbas_id"                       , P_FIS_SATIRI->demirbas_id );
    query.UPDATE();



    toplam_tutar                 += P_FIS_SATIRI->satir_tutari;
    toplam_bir_amortisman_tutari += P_FIS_SATIRI->yeni_birikmis_amor_tutari;

    P_FIS->toplam_tutar             = toplam_tutar;
    P_FIS->top_birikmis_amor_tutari = toplam_bir_amortisman_tutari;





    query.PREPARE_INSERT("dmr_fis_satirlari" , "fis_satiri_id" , "fis_id , demirbas_id,"
                         "aciklama ,satir_tutari , birikmis_amortisman_tutari , order_number,"
                         "base_record_id , modul_id , program_id");
    query.SET_VALUE(":fis_id"                     , P_FIS_SATIRI->fis_id);
    query.SET_VALUE(":demirbas_id"                , P_FIS_SATIRI->demirbas_id);
    query.SET_VALUE(":aciklama"                   , P_FIS_SATIRI->aciklama);
    query.SET_VALUE(":satir_tutari"               , ROUND(P_FIS_SATIRI->satir_tutari,-2));
    query.SET_VALUE(":birikmis_amortisman_tutari" , ROUND(P_FIS_SATIRI->birikmis_amor_tutari,-2));
    query.SET_VALUE(":order_number"               , P_FIS_SATIRI->order_number);
    query.SET_VALUE(":base_record_id"             , P_FIS_SATIRI->base_record_id);
    query.SET_VALUE(":modul_id"                   , P_FIS_SATIRI->modul_id);
    query.SET_VALUE(":program_id"                 , P_FIS_SATIRI->program_id);

    int fis_satiri_id = query.INSERT();


    query.PREPARE_UPDATE("dmr_fisler" , "fis_id" , "last_order_number,toplam_tutar,"
                         "toplam_birikmis_amor_tutari", "fis_id = :fis_id");
    query.SET_VALUE(":last_order_number"           , P_FIS_SATIRI->order_number);
    query.SET_VALUE(":toplam_tutar"                , ROUND(P_FIS->toplam_tutar ,-2));
    query.SET_VALUE(":toplam_birikmis_amor_tutari" , ROUND(P_FIS->top_birikmis_amor_tutari,-2));
    query.SET_VALUE(":fis_id"                      , P_FIS_SATIRI->fis_id);
    query.UPDATE();

    if ( P_FIS_SATIRI->base_record_id EQ 0 ) {
        P_FIS_SATIRI->base_record_id = fis_satiri_id;
    }


    //ISLETME ENTEGRASYONU

    //Amortisman tutari kadar isletme modulune gelir fisi satiri eklenir.
    //Kdv yoktur.

    ISL_FIS_STRUCT * ISL_FISI = new ISL_FIS_STRUCT;

    ISL_CLEAR_FIS_STRUCT(ISL_FISI);

    int isl_fis_id = ISL_GET_ENT_FIS_ID(P_FIS->modul_id , P_FIS->program_id , P_FIS->base_fis_id);

    if ( isl_fis_id EQ 0 ) {

        ISL_FISI->aciklama    = QObject::tr("Fiş Sabit Kıymet Modülü tarafından üretilmiştir.Fiş No : %1").arg(P_FIS->fis_no) + P_FIS->aciklama;
        ISL_FISI->base_fis_id = P_FIS_SATIRI->fis_id;
        ISL_FISI->fis_tarihi  = P_FIS->fis_tarihi;
        ISL_FISI->fis_turu    = ENUM_GIDER_FISI;
        ISL_FISI->modul_id    = P_FIS->modul_id;
        ISL_FISI->program_id  = P_FIS->program_id;

        isl_fis_id = ISL_FISI_EKLE(ISL_FISI);
    }
    else {
        ISL_FIS_BILGILERINI_OKU(ISL_FISI , isl_fis_id);
    }

    ISL_FIS_SATIRI_STRUCT * ISL_FIS_SATIRI = new ISL_FIS_SATIRI_STRUCT;

    ISL_CLEAR_FIS_SATIRI_STRUCT(ISL_FIS_SATIRI);

    ISL_FIS_SATIRI->modul_id              = P_FIS->modul_id;
    ISL_FIS_SATIRI->program_id            = P_FIS->program_id;
    ISL_FIS_SATIRI->base_record_id        = P_FIS_SATIRI->base_record_id;
    ISL_FIS_SATIRI->aciklama              = P_FIS_SATIRI->aciklama;
    ISL_FIS_SATIRI->fis_id                = isl_fis_id;
    ISL_FIS_SATIRI->satir_kdv_haric_tutar = P_FIS_SATIRI->satir_tutari;
    ISL_FIS_SATIRI->satir_kdv_dahil_tutar = P_FIS_SATIRI->satir_tutari;
    ISL_FIS_SATIRI->gider_turu            = ENUM_GENEL_GIDERLER;

    ISL_FIS_SATIRI_EKLE(ISL_FISI , ISL_FIS_SATIRI);

    delete ISL_FISI;
    delete ISL_FIS_SATIRI;

    //MUHASEBE ENTEGRASYONU


    MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

    MUH_FIS_BILGILERINI_OKU ( MUH_FIS, P_FIS->muh_fis_id,USE_DB );

    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

    MUH_CLEAR_FIS_SATIRI_STRUCT     ( MUH_FIS_SATIRI );

    MUH_FIS_SATIRI->aciklama                = P_FIS_SATIRI->aciklama;
    MUH_FIS_SATIRI->borc_tutari             = P_FIS_SATIRI->satir_tutari;
    MUH_FIS_SATIRI->alacak_tutari           = 0;
    MUH_FIS_SATIRI->fis_id                  = P_FIS->muh_fis_id;
    MUH_FIS_SATIRI->hesap_id                = amor_gider_hesap_id;
    MUH_FIS_SATIRI->program_id              = P_FIS_SATIRI->program_id;
    MUH_FIS_SATIRI->modul_id                = P_FIS_SATIRI->modul_id;
    MUH_FIS_SATIRI->base_record_id          = P_FIS_SATIRI->base_record_id;

    MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);

    MUH_FIS_SATIRI->borc_tutari             = 0;
    MUH_FIS_SATIRI->alacak_tutari           = P_FIS_SATIRI->satir_tutari;
    MUH_FIS_SATIRI->hesap_id                = bir_amortismanlar_hesap_id;

    MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);

    delete MUH_FIS;
    delete MUH_FIS_SATIRI;

    return fis_satiri_id;

}

/***************************************************************************************
                   DMR_FIS_GUNCELLE
***************************************************************************************/

void DMR_FIS_GUNCELLE(DMR_FIS_STRUCT *P_FIS, int p_fis_id)
{
    SQL_QUERY query (DB);

    query.PREPARE_SELECT("dmr_fisler" , "fis_tarihi , modul_id , program_id , base_fis_id",
                         "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    QString fis_tarihi = query.VALUE(0).toString();
    int modul_id       = query.VALUE(1).toInt();
    int program_id     = query.VALUE(2).toInt();
    int base_fis_id    = query.VALUE(3).toInt();


    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id
         OR P_FIS->base_fis_id NE base_fis_id ) {
        return;
    }

    if ( P_FIS->fis_turu EQ AMORTISMAN_AYIRMA_FISI ) {

        if ( fis_tarihi NE P_FIS->fis_tarihi ) {
            //fis tarihi degistiginde eski tarihten baslayarak amortisman tutari azaltilir\yeni tarihe gore tekrardan arttirilir

            int old_gun_no = MALI_YIL_ARRAY_INDIS ( QDate::fromString(fis_tarihi , "yyyy.MM.dd") );
            int gun_no     = MALI_YIL_ARRAY_INDIS ( QDate::fromString(P_FIS->fis_tarihi , "yyyy.MM.dd"));

            query.PREPARE_SELECT ( "dmr_fisler,dmr_fis_satirlari","demirbas_id,satir_tutari  ",
                                   "dmr_fisler.fis_id = dmr_fis_satirlari.fis_id AND "
                                   "dmr_fisler.fis_id = :fis_id" );
            query.SET_VALUE      ( ":fis_id" , p_fis_id );

            if ( query.SELECT() NE 0 ) {
                while ( query.NEXT() ) {
                    int demirbas_id       = query.VALUE(0).toInt();
                    double satir_tutari   = query.VALUE(1).toDouble();

                    DMR_AMORTISMAN_TUTARINI_AZALT  ( demirbas_id , old_gun_no , satir_tutari );
                    DMR_AMORTISMAN_TUTARINI_ARTTIR ( demirbas_id , gun_no     , satir_tutari );
                }
            }
        }
    }

    query.PREPARE_UPDATE("dmr_fisler" , "fis_id" , "fis_no,aciklama,fis_tarihi,"
                         "dusulecek_amortisman_yuzdesi,degerleme_orani" , "fis_id = :fis_id");
    query.SET_VALUE(":fis_no"                       , P_FIS->fis_no );
    query.SET_VALUE(":aciklama"                     , P_FIS->aciklama);
    query.SET_VALUE(":fis_tarihi"                   , P_FIS->fis_tarihi);
    query.SET_VALUE(":dusulecek_amortisman_yuzdesi" , P_FIS->dusulecek_amor_yuzdesi);
    query.SET_VALUE(":degerleme_orani"              , P_FIS->degerleme_orani);
    query.SET_VALUE(":fis_id"                       , p_fis_id);

    query.UPDATE();

    if ( P_FIS->base_fis_id EQ 0 ) {
        P_FIS->base_fis_id = p_fis_id;
    }

    //ISLETME ENTEGRASYONU

    int isl_fis_id = ISL_GET_ENT_FIS_ID(P_FIS->modul_id , P_FIS->program_id , P_FIS->base_fis_id);

    if ( isl_fis_id > 0 ) {

        ISL_FIS_STRUCT * ISL_FISI = new ISL_FIS_STRUCT;

        ISL_CLEAR_FIS_STRUCT(ISL_FISI);

        ISL_FIS_BILGILERINI_OKU(ISL_FISI , isl_fis_id);

        QString aciklama     = QObject::tr("Fiş SABİT KIYMET modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
        aciklama             += P_FIS->aciklama;
        ISL_FISI->aciklama   = aciklama;
        ISL_FISI->fis_tarihi = P_FIS->fis_tarihi;

        ISL_FISI_GUNCELLE(ISL_FISI , isl_fis_id);

        delete ISL_FISI;

    }


    //MUHASEBE ENTEGRASYONU

    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

    MUH_FIS_BILGILERINI_OKU ( MUHASEBE_FISI , P_FIS->muh_fis_id );

    if ( P_FIS->modul_id EQ DEMIRBAS_MODULU ) {
        QString aciklama        = QObject::tr("Fiş SABİT KIYMET modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
        aciklama               += P_FIS->aciklama;
        MUHASEBE_FISI->aciklama = aciklama;
    }
    else {
        MUHASEBE_FISI->aciklama = P_FIS->aciklama;
    }
    MUHASEBE_FISI->fis_tarihi   = P_FIS->fis_tarihi;

    MUH_FISI_GUNCELLE ( MUHASEBE_FISI , P_FIS->muh_fis_id );

    delete MUHASEBE_FISI;

}
/***************************************************************************************
                   DMR_FIS_SATIRI_GUNCELLE
***************************************************************************************/

void DMR_FIS_SATIRI_GUNCELLE(DMR_FIS_STRUCT *P_FIS, DMR_FIS_SATIRI_STRUCT *P_FIS_SATIRI, int p_fis_satiri_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("dmr_fis_satirlari,dmr_fisler" , "demirbas_id,satir_tutari,toplam_tutar,"
                         "toplam_birikmis_amor_tutari,birikmis_amortisman_tutari,fis_tarihi",
                         "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int    demirbas_id                 = query.VALUE(0).toInt();
    double satir_tutari                = query.VALUE(1).toDouble();
    double toplam_tutar                = query.VALUE(2).toDouble();
    double toplam_birikmis_amor_tutari = query.VALUE(3).toDouble();
    double birikmis_amortisman_tutari  = query.VALUE(4).toDouble();
    QDate  fis_tarihi                  = QDate::fromString(query.VALUE(5).toString() , "yyyy.MM.dd");

    if ( demirbas_id NE P_FIS_SATIRI->demirbas_id ) {
        //Eski demirbas bilgileri guncellenecek

        query.PREPARE_SELECT("dmr_demirbaslar" , "yillik_ayrilan_amor_yuzdesi , toplamda_ayrilan_amor_yuzdesi,"
                             "amortisman_orani , demirbasin_yeni_degeri ,"
                             "yeni_brkms_amortisman_tutari ,yeniden_degerleme_yapildi,"
                             "amortisman_ayrilabilir_mi",
                             "demirbas_id = :demirbas_id");
        query.SET_VALUE(":demirbas_id" , demirbas_id);

        if ( query.SELECT() EQ 0 ) {
            return;
        }


        query.NEXT();

        double yillik_ayrilan_amor_yuzdesi   = query.VALUE(0).toDouble();
        double toplamda_ayrilan_amor_yuzdesi = query.VALUE(1).toDouble();
        double amortisman_orani              = query.VALUE(2).toDouble();
        double demirbasin_yeni_degeri        = query.VALUE(3).toDouble();
        double yeni_brkms_amortisman_tutari  = query.VALUE(4).toDouble();
        int    yeniden_degerleme_yapildi     = query.VALUE(5).toInt();
        int    amortisman_ayrilabilir_mi     = query.VALUE(6).toInt();



        if ( P_FIS->fis_turu EQ YENIDEN_DEGERLEME_FISI ) {
            demirbasin_yeni_degeri       = 0;
            yeni_brkms_amortisman_tutari = 0;
            yeniden_degerleme_yapildi    = 0;
        }
        else {
            yillik_ayrilan_amor_yuzdesi   -= P_FIS->dusulecek_amor_yuzdesi;
            toplamda_ayrilan_amor_yuzdesi -= amortisman_orani;

            if ( toplamda_ayrilan_amor_yuzdesi < 100 ) {
                amortisman_ayrilabilir_mi = 1;
            }

        }

        query.PREPARE_UPDATE("dmr_demirbaslar" , "demirbas_id" , "yillik_ayrilan_amor_yuzdesi,"
                             "toplamda_ayrilan_amor_yuzdesi , amortisman_ayrilabilir_mi,"
                             "demirbasin_yeni_degeri,"
                             "yeni_brkms_amortisman_tutari , yeniden_degerleme_yapildi",
                             "demirbas_id = :demirbas_id");

        query.SET_VALUE(":yillik_ayrilan_amor_yuzdesi"   , yillik_ayrilan_amor_yuzdesi);
        query.SET_VALUE(":toplamda_ayrilan_amor_yuzdesi" , toplamda_ayrilan_amor_yuzdesi);
        query.SET_VALUE(":amortisman_ayrilabilir_mi"     , amortisman_ayrilabilir_mi);
        query.SET_VALUE(":demirbasin_yeni_degeri"        , demirbasin_yeni_degeri);
        query.SET_VALUE(":yeni_brkms_amortisman_tutari"  , yeni_brkms_amortisman_tutari);
        query.SET_VALUE(":yeniden_degerleme_yapildi"     , yeniden_degerleme_yapildi);
        query.SET_VALUE(":demirbas_id"                   , demirbas_id);

        query.UPDATE();

        int gun_no = MALI_YIL_ARRAY_INDIS(fis_tarihi);

        if ( P_FIS->fis_turu EQ AMORTISMAN_AYIRMA_FISI ) {

            DMR_AMORTISMAN_TUTARINI_AZALT ( demirbas_id,gun_no,satir_tutari );
        }

        toplam_tutar                -= satir_tutari;
        toplam_birikmis_amor_tutari -= birikmis_amortisman_tutari;

        //Eski demirbas bilgileri guncellendi


        query.PREPARE_SELECT("dmr_demirbaslar" , "yillik_ayrilan_amor_yuzdesi , toplamda_ayrilan_amor_yuzdesi,"
                             "amortisman_orani , amortisman_ayrilabilir_mi ,"
                             "demirbasin_yeni_degeri,"
                             "yeniden_degerleme_yapildi , yeni_brkms_amortisman_tutari",
                             "demirbas_id = :demirbas_id");

        query.SET_VALUE(":demirbas_id" , P_FIS_SATIRI->demirbas_id);


        if ( query.SELECT() EQ 0 ) {
            return;
        }

        query.NEXT();

        yillik_ayrilan_amor_yuzdesi      = query.VALUE(0).toDouble();
        toplamda_ayrilan_amor_yuzdesi    = query.VALUE(1).toDouble();
        amortisman_orani                 = query.VALUE(2).toDouble();
        amortisman_ayrilabilir_mi        = query.VALUE(3).toInt();
        demirbasin_yeni_degeri           = query.VALUE(4).toDouble();
        yeniden_degerleme_yapildi        = query.VALUE(5).toInt();
        yeni_brkms_amortisman_tutari     = query.VALUE(6).toDouble();



        if ( P_FIS->fis_turu EQ AMORTISMAN_AYIRMA_FISI ) {

            yillik_ayrilan_amor_yuzdesi += P_FIS->dusulecek_amor_yuzdesi;

            if ( yillik_ayrilan_amor_yuzdesi EQ 100 ) {

                toplamda_ayrilan_amor_yuzdesi += amortisman_orani;

                if ( toplamda_ayrilan_amor_yuzdesi EQ 100 ) {
                    amortisman_ayrilabilir_mi = 0;
                }
            }


            DMR_AMORTISMAN_TUTARINI_ARTTIR ( P_FIS_SATIRI->demirbas_id,gun_no,P_FIS_SATIRI->satir_tutari);
        }
        else {

            demirbasin_yeni_degeri       = P_FIS_SATIRI->satir_tutari;
            yeniden_degerleme_yapildi    = 1;
            yeni_brkms_amortisman_tutari = P_FIS_SATIRI->yeni_birikmis_amor_tutari;
        }


        query.PREPARE_UPDATE("dmr_demirbaslar" , "demirbas_id" , "yillik_ayrilan_amor_yuzdesi,"
                             "toplamda_ayrilan_amor_yuzdesi , amortisman_ayrilabilir_mi,"
                             "demirbasin_yeni_degeri,"
                             "yeniden_degerleme_yapildi , yeni_brkms_amortisman_tutari",
                             "demirbas_id = :demirbas_id");

        query.SET_VALUE(":yillik_ayrilan_amor_yuzdesi"   , yillik_ayrilan_amor_yuzdesi);
        query.SET_VALUE(":toplamda_ayrilan_amor_yuzdesi" , toplamda_ayrilan_amor_yuzdesi);
        query.SET_VALUE(":amortisman_ayrilabilir_mi"     , amortisman_ayrilabilir_mi);
        query.SET_VALUE(":demirbasin_yeni_degeri"        , demirbasin_yeni_degeri);
        query.SET_VALUE(":yeniden_degerleme_yapildi"     , yeniden_degerleme_yapildi);
        query.SET_VALUE(":yeni_brkms_amortisman_tutari"  , yeni_brkms_amortisman_tutari);
        query.SET_VALUE(":demirbas_id"                   , P_FIS_SATIRI->demirbas_id );

        query.UPDATE();

        toplam_tutar                += P_FIS_SATIRI->satir_tutari;
        toplam_birikmis_amor_tutari += P_FIS_SATIRI->birikmis_amor_tutari;
    }


    query.PREPARE_UPDATE("dmr_fis_satirlari" , "fis_satiri_id" , "demirbas_id , aciklama,"
                         "satir_tutari , birikmis_amortisman_tutari ",
                         "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":demirbas_id"                , P_FIS_SATIRI->demirbas_id);
    query.SET_VALUE(":aciklama"                   , P_FIS_SATIRI->aciklama   );
    query.SET_VALUE(":satir_tutari"               , P_FIS_SATIRI->satir_tutari);
    query.SET_VALUE(":birikmis_amortisman_tutari" , P_FIS_SATIRI->birikmis_amor_tutari);
    query.SET_VALUE(":fis_satiri_id"              , p_fis_satiri_id);

    query.UPDATE();

    P_FIS->toplam_tutar             = toplam_tutar;
    P_FIS->top_birikmis_amor_tutari = toplam_birikmis_amor_tutari;

    query.PREPARE_UPDATE("dmr_fisler" , "fis_id" , "toplam_tutar , toplam_birikmis_amor_tutari" ,
                         "fis_id = :fis_id");
    query.SET_VALUE(":toplam_tutar"                , P_FIS->toplam_tutar  );
    query.SET_VALUE(":toplam_birikmis_amor_tutari" , P_FIS->top_birikmis_amor_tutari);
    query.SET_VALUE(":fis_id"                      , P_FIS_SATIRI->fis_id );
    query.UPDATE();



    if ( P_FIS_SATIRI->base_record_id EQ 0 ) {
        P_FIS_SATIRI->base_record_id = p_fis_satiri_id;
    }

    if ( P_FIS->base_fis_id EQ 0 ) {
        P_FIS->base_fis_id = P_FIS_SATIRI->fis_id;
    }

    //ISLETME ENTEGRASYONU
    int isl_fis_id = ISL_GET_ENT_FIS_ID(P_FIS->modul_id , P_FIS->program_id , P_FIS->base_fis_id);

    if ( isl_fis_id > 0 ) {

        ISL_FIS_STRUCT * ISL_FISI = new ISL_FIS_STRUCT;

        ISL_CLEAR_FIS_STRUCT(ISL_FISI);

        ISL_FIS_BILGILERINI_OKU(ISL_FISI ,  isl_fis_id);

        ISL_FIS_SATIRI_STRUCT * ISL_FIS_SATIRI = new ISL_FIS_SATIRI_STRUCT;

        ISL_CLEAR_FIS_SATIRI_STRUCT(ISL_FIS_SATIRI);

        int isl_satir_id = ISL_GET_ENT_FIS_SATIRI_ID(P_FIS->modul_id , P_FIS->program_id , P_FIS_SATIRI->base_record_id);

        ISL_FIS_SATIRI_BILGILERINI_OKU(ISL_FIS_SATIRI ,isl_satir_id);

        ISL_FIS_SATIRI->aciklama = P_FIS_SATIRI->aciklama;
        ISL_FIS_SATIRI->satir_kdv_dahil_tutar = P_FIS_SATIRI->satir_tutari;
        ISL_FIS_SATIRI->satir_kdv_haric_tutar = P_FIS_SATIRI->satir_tutari;

        if ( isl_satir_id > 0 ) {
            ISL_FIS_SATIRI_GUNCELLE(ISL_FISI , ISL_FIS_SATIRI , isl_satir_id);
        }

        delete ISL_FISI;
        delete ISL_FIS_SATIRI;
    }


    //MUHASEBE ENTEGRASYONU

    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

    MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI , P_FIS->muh_fis_id);

    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;


    QStringList MUH_ROW_ID_LIST = MUH_GET_ENT_ROW_ID_LIST(P_FIS->program_id,P_FIS->modul_id ,
                                                          P_FIS_SATIRI->base_record_id);

    for ( int i = 0 ; i < MUH_ROW_ID_LIST.size() ; i++ ) {

        int muh_fis_satiri_id = QVariant(MUH_ROW_ID_LIST.at(i)).toInt();

        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRINI_OKU(MUH_FIS_SATIRI , muh_fis_satiri_id );

        MUH_FIS_SATIRI->aciklama                = P_FIS_SATIRI->aciklama;

        if ( MUH_FIS_SATIRI->borc_tutari > 0 ) {
            MUH_FIS_SATIRI->borc_tutari   = P_FIS_SATIRI->satir_tutari;
            MUH_FIS_SATIRI->alacak_tutari = 0;
        }
        else {
            MUH_FIS_SATIRI->borc_tutari   = 0 ;
            MUH_FIS_SATIRI->alacak_tutari = P_FIS_SATIRI->satir_tutari;
        }

        MUH_FIS_SATIRINI_GUNCELLE ( MUHASEBE_FISI, MUH_FIS_SATIRI, muh_fis_satiri_id );
    }

    delete MUHASEBE_FISI;
    delete MUH_FIS_SATIRI;

}

/***************************************************************************************
                   DMR_FIS_SIL
***************************************************************************************/
void DMR_FIS_SIL(DMR_FIS_STRUCT * P_FIS, int p_fis_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("dmr_fisler" , "program_id,modul_id,base_fis_id" , "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT( ) EQ 0 ) {
        return;
    }

    query.NEXT();

    int program_id     = query.VALUE(0).toInt();
    int modul_id       = query.VALUE(1).toInt();
    int base_fis_id    = query.VALUE(2).toInt();

    if ( P_FIS->program_id NE program_id OR P_FIS->modul_id NE modul_id OR
         P_FIS->base_fis_id NE base_fis_id ) {
        return;
    }
    if ( base_fis_id EQ 0 ) {
        base_fis_id = p_fis_id;
    }

    if ( P_FIS->muh_fis_id > 0  AND P_FIS->modul_id EQ DEMIRBAS_MODULU ) {
        MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

        MUH_FIS->modul_id   = P_FIS->modul_id;
        MUH_FIS->program_id = P_FIS->program_id;
        MUH_FIS->base_fis_id= base_fis_id;

        MUH_FIS_SIL ( MUH_FIS, P_FIS->muh_fis_id );
        delete MUH_FIS;
    }

    int isl_fis_id = ISL_GET_ENT_FIS_ID(P_FIS->modul_id, P_FIS->program_id , base_fis_id);

    if ( isl_fis_id > 0 ) {

        ISL_FIS_STRUCT * ISL_FISI = new ISL_FIS_STRUCT;

        ISL_CLEAR_FIS_STRUCT(ISL_FISI);

        ISL_FIS_BILGILERINI_OKU(ISL_FISI , isl_fis_id);

        ISL_FISI_SIL(ISL_FISI , isl_fis_id);

        delete ISL_FISI;
    }


    query.PREPARE_SELECT ( "dmr_fis_satirlari","fis_satiri_id","fis_id = :fis_id");
    query.SET_VALUE      ( ":fis_id" , p_fis_id );

    if ( query.SELECT() NE 0 ) {
        while  ( query.NEXT() ) {
            int fis_satiri_id  = query.VALUE(0).toInt();

            DMR_FIS_SATIRI_SIL(P_FIS , fis_satiri_id);
        }
    }


    query.PREPARE_DELETE ( "dmr_fisler","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id" , p_fis_id );
    query.DELETE();

}
/***************************************************************************************
                   DMR_FIS_SATIRI_SIL
***************************************************************************************/

void DMR_FIS_SATIRI_SIL(DMR_FIS_STRUCT *P_FIS, int p_fis_satiri_id)
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dmr_fis_satirlari , dmr_fisler" , "dmr_fisler.program_id,"
                         "dmr_fisler.modul_id,base_fis_id ,fis_tarihi , "
                         "demirbas_id , toplam_tutar , satir_tutari , "
                         "dmr_fis_satirlari.fis_id , toplam_birikmis_amor_tutari,"
                         "birikmis_amortisman_tutari,base_record_id",
                         "dmr_fisler.fis_id = dmr_fis_satirlari.fis_id AND "
                         "fis_satiri_id = :fis_satiri_id");

    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    int program_id  = query.VALUE(0).toInt();
    int modul_id    = query.VALUE(1).toInt();
    int base_fis_id = query.VALUE(2).toInt();

    if ( P_FIS->program_id NE program_id OR P_FIS->modul_id NE modul_id OR
         P_FIS->base_fis_id NE base_fis_id ) {
        return;
    }

    QDate  fis_tarihi                  = QDate::fromString(query.VALUE(3).toString() , "yyyy.MM.dd");
    int    demirbas_id                 = query.VALUE(4).toInt();
    double toplam_tutar                = query.VALUE(5).toDouble();
    double satir_tutari                = query.VALUE(6).toDouble();
    int    fis_id                      = query.VALUE(7).toInt();
    double toplam_birikmis_amor_tutari = query.VALUE(8).toDouble();
    double birikmis_amor_tutari        = query.VALUE(9).toDouble();
    int    base_record_id              = query.VALUE(10).toInt();

    query.PREPARE_SELECT ( "dmr_demirbaslar","yillik_ayrilan_amor_yuzdesi,"
                           "toplamda_ayrilan_amor_yuzdesi,"
                           "amortisman_orani,"
                           "amortisman_ayrilabilir_mi , yeniden_degerleme_yapildi ,"
                           "yeni_brkms_amortisman_tutari , demirbasin_yeni_degeri",
                           "demirbas_id = :demirbas_id" );
    query.SET_VALUE      (":demirbas_id" , demirbas_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    double yillik_ayrilan_amortisman_yuzdesi   = query.VALUE(0).toDouble();
    double toplamda_ayrilan_amortisman_yuzdesi = query.VALUE(1).toDouble();
    double amortisman_orani                    = query.VALUE(2).toDouble();
    int    amortisman_ayrilabilir_mi           = query.VALUE(3).toInt();
    int    yeniden_degerleme_yapildi           = query.VALUE(4).toInt();
    double yeni_birikmis_amortisman_tutari     = query.VALUE(5).toDouble();
    double demirbasin_yeni_degeri              = query.VALUE(6).toDouble();



    if ( P_FIS->fis_turu EQ YENIDEN_DEGERLEME_FISI ) {
        yeniden_degerleme_yapildi       = 0;
        yeni_birikmis_amortisman_tutari = 0;
        demirbasin_yeni_degeri          = 0;
    }
    else {
        if ( yillik_ayrilan_amortisman_yuzdesi EQ 100 ) {
            toplamda_ayrilan_amortisman_yuzdesi -= amortisman_orani;
        }

        yillik_ayrilan_amortisman_yuzdesi -= P_FIS->dusulecek_amor_yuzdesi;
        if ( toplamda_ayrilan_amortisman_yuzdesi < 100 ) {
            amortisman_ayrilabilir_mi = 1;
        }


    }

    query.PREPARE_UPDATE ( "dmr_demirbaslar","demirbas_id","yillik_ayrilan_amor_yuzdesi ,"
                            "toplamda_ayrilan_amor_yuzdesi  , "
                            "amortisman_ayrilabilir_mi ,"
                            "yeniden_degerleme_yapildi , yeni_brkms_amortisman_tutari,"
                            "demirbasin_yeni_degeri",
                            "demirbas_id = :demirbas_id" );
    query.SET_VALUE      ( ":yillik_ayrilan_amor_yuzdesi"      , yillik_ayrilan_amortisman_yuzdesi );
    query.SET_VALUE      ( ":toplamda_ayrilan_amor_yuzdesi"    , toplamda_ayrilan_amortisman_yuzdesi );
    query.SET_VALUE      ( ":amortisman_ayrilabilir_mi"        , amortisman_ayrilabilir_mi );
    query.SET_VALUE      ( ":yeniden_degerleme_yapildi"        , yeniden_degerleme_yapildi );
    query.SET_VALUE      ( ":yeni_brkms_amortisman_tutari"     , yeni_birikmis_amortisman_tutari);
    query.SET_VALUE      ( ":demirbasin_yeni_degeri"           , demirbasin_yeni_degeri);
    query.SET_VALUE      ( ":demirbas_id"                      , demirbas_id );
    query.UPDATE();

    toplam_tutar -= satir_tutari;
    toplam_birikmis_amor_tutari -= birikmis_amor_tutari;

    P_FIS->toplam_tutar = toplam_tutar;
    P_FIS->top_birikmis_amor_tutari = toplam_birikmis_amor_tutari;

    query.PREPARE_UPDATE ( "dmr_fisler","fis_id","toplam_tutar ,toplam_birikmis_amor_tutari",
                           "fis_id = :fis_id");
    query.SET_VALUE      ( ":toplam_tutar" , P_FIS->toplam_tutar );
    query.SET_VALUE      ( ":toplam_birikmis_amor_tutari" , P_FIS->top_birikmis_amor_tutari);
    query.SET_VALUE      ( ":fis_id"       , fis_id );
    query.UPDATE();

    query.PREPARE_DELETE ( "dmr_fis_satirlari","fis_satiri_id = :fis_satiri_id" );
    query.SET_VALUE      ( ":fis_satiri_id" , p_fis_satiri_id);
    query.DELETE();

    if ( P_FIS->fis_turu EQ AMORTISMAN_AYIRMA_FISI ) {

        DMR_AMORTISMAN_TUTARINI_AZALT ( demirbas_id, MALI_YIL_ARRAY_INDIS ( fis_tarihi),satir_tutari);
    }

    if ( base_record_id EQ 0 ) {
        base_record_id = p_fis_satiri_id;
    }

    if ( P_FIS->base_fis_id EQ 0 ) {
        P_FIS->base_fis_id = fis_id;
    }

    //ISLETME_ENTEGRASYONU

    int isl_fis_id = ISL_GET_ENT_FIS_ID(P_FIS->modul_id , P_FIS->program_id , P_FIS->base_fis_id);

    if ( isl_fis_id > 0 ) {

        ISL_FIS_STRUCT * ISL_FISI = new ISL_FIS_STRUCT;

        ISL_CLEAR_FIS_STRUCT(ISL_FISI);

        ISL_FIS_BILGILERINI_OKU(ISL_FISI , isl_fis_id);

        int isl_fis_satiri_id = ISL_GET_ENT_FIS_SATIRI_ID(P_FIS->modul_id , P_FIS->program_id , base_record_id);

        ISL_FIS_SATIRI_SIL(ISL_FISI , isl_fis_satiri_id);

        delete ISL_FISI;

    }



    //MUHASEBE ENTEGRASYONU

    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

    MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI , P_FIS->muh_fis_id);


    QStringList MUH_ROW_ID_LIST = MUH_GET_ENT_ROW_ID_LIST(P_FIS->program_id,P_FIS->modul_id ,
                                                          base_record_id);

    for ( int i = 0 ; i < MUH_ROW_ID_LIST.size() ; i++ ) {

        int muh_fis_satiri_id = QVariant(MUH_ROW_ID_LIST.at(i)).toInt();

        MUH_FIS_SATIRINI_SIL(MUHASEBE_FISI , muh_fis_satiri_id);
    }

    delete MUHASEBE_FISI;

}

/***************************************************************************************
                  DMR_CLEAR_FIS_STRUCT
***************************************************************************************/

void DMR_CLEAR_FIS_STRUCT(DMR_FIS_STRUCT *P_FIS)
{
    P_FIS->fis_no                   = 0;
    P_FIS->fis_turu                 = -1;
    P_FIS->modul_id                 = -1;
    P_FIS->program_id               = -1;
    P_FIS->base_fis_id              = 0;
    P_FIS->muh_fis_id               = 0;
    P_FIS->degerleme_orani          = 0.0;
    P_FIS->dusulecek_amor_yuzdesi   = 0.0;
    P_FIS->toplam_tutar             = 0.0;
    P_FIS->top_birikmis_amor_tutari = 0.0;
    P_FIS->muh_fis_id               = 0;

    P_FIS->fis_tarihi.clear();
    P_FIS->aciklama.clear();
}

/***************************************************************************************
                   DMR_CLEAR_FIS_SATIRI_STRUCT
***************************************************************************************/

void DMR_CLEAR_FIS_SATIRI_STRUCT(DMR_FIS_SATIRI_STRUCT *P_FIS_SATIRI)
{
    P_FIS_SATIRI->fis_id                = 0;
    P_FIS_SATIRI->demirbas_id           = 0;
    P_FIS_SATIRI->order_number          = 0;
    P_FIS_SATIRI->base_record_id        = 0;
    P_FIS_SATIRI->modul_id              = -1;
    P_FIS_SATIRI->program_id            = -1;
    P_FIS_SATIRI->birikmis_amor_tutari  = 0.0;
    P_FIS_SATIRI->satir_tutari          = 0.0;
    P_FIS_SATIRI->aciklama.clear();
    P_FIS_SATIRI->yeni_birikmis_amor_tutari = 0;

}
/***************************************************************************************
                   DMR_FIS_BILGILERINI_OKU
***************************************************************************************/
void DMR_FIS_BILGILERINI_OKU(DMR_FIS_STRUCT *P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("dmr_fisler" , "fis_no,fis_tarihi,aciklama,fis_turu,"
                         "toplam_tutar,toplam_birikmis_amor_tutari,degerleme_orani,"
                         "dusulecek_amortisman_yuzdesi,modul_id,program_id,base_fis_id,"
                         "muh_fis_id" , "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    P_FIS->fis_no                   = query.VALUE("fis_no").toInt();
    P_FIS->fis_tarihi               = query.VALUE("fis_tarihi").toString();
    P_FIS->aciklama                 = query.VALUE("aciklama").toString();
    P_FIS->fis_turu                 = query.VALUE("fis_turu").toInt();
    P_FIS->toplam_tutar             = query.VALUE("toplam_tutar").toDouble();
    P_FIS->top_birikmis_amor_tutari = query.VALUE("toplam_birikmis_amor_tutari").toDouble();
    P_FIS->degerleme_orani          = query.VALUE("degerleme_orani").toDouble();
    P_FIS->dusulecek_amor_yuzdesi   = query.VALUE("dusulecek_amortisman_yuzdesi").toDouble();
    P_FIS->modul_id                 = query.VALUE("modul_id").toInt();
    P_FIS->program_id               = query.VALUE("program_id").toInt();
    P_FIS->base_fis_id              = query.VALUE("base_fis_id").toInt();
    P_FIS->muh_fis_id               = query.VALUE("muh_fis_id").toInt();
}

/***************************************************************************************
                   DMR_FIS_SATIRINI_OKU
***************************************************************************************/

void DMR_FIS_SATIRINI_OKU(DMR_FIS_SATIRI_STRUCT *P_FIS_SATIRI, int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("dmr_fis_satirlari" , "fis_id,demirbas_id,aciklama,satir_tutari,"
                         "birikmis_amortisman_tutari,order_number,"
                         "base_record_id,modul_id,program_id" , "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    P_FIS_SATIRI->fis_id               = query.VALUE("fis_id").toInt();
    P_FIS_SATIRI->demirbas_id          = query.VALUE("demirbas_id").toInt();
    P_FIS_SATIRI->aciklama             = query.VALUE("aciklama").toString();
    P_FIS_SATIRI->satir_tutari         = query.VALUE("satir_tutari").toDouble();
    P_FIS_SATIRI->birikmis_amor_tutari = query.VALUE("birikmis_amortisman_tutari").toDouble();
    P_FIS_SATIRI->order_number         = query.VALUE("order_number").toInt();
    P_FIS_SATIRI->base_record_id       = query.VALUE("base_record_id").toInt();
    P_FIS_SATIRI->modul_id             = query.VALUE("modul_id").toInt();
    P_FIS_SATIRI->program_id           = query.VALUE("program_id").toInt();
}

















