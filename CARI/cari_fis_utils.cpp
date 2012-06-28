#include "adak_sql.h"
#include "cari_fis_utils.h"
#include "cari_console_utils.h"
#include "cek_enum.h"
#include "cek_senet_utils.h"
#include "muh_fis_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "muh_enum.h"
#include "e9_console_utils.h"
#include "cek_fis_utils.h"
#include "banka_enum.h"
#include "bnk_fis_utils.h"
#include "e9_enum.h"
#include "cari_enum.h"
#include "cek_console_utils.h"

extern      ADAK_SQL   *   DB;

/***************************************************************************************
                   CARI_FIS_EKLE
***************************************************************************************/

int CARI_FIS_EKLE ( CARI_FIS_STRUCT * P_FIS , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB ;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    if ( P_FIS->fis_no EQ 0 ) {
        P_FIS->fis_no = CARI_SIRADAKI_FIS_NO_AL ( P_FIS->fis_tarihi , USE_DB);
    }

    SQL_QUERY query ( USE_DB );

    query.PREPARE_INSERT ("car_fisler", "fis_id", "fis_no, fis_tarihi, cari_hesap_id, aciklama, "
                           "toplam_borc, toplam_alacak, belge_seri, belge_numarasi, fis_turu, makbuz_para_birimi_id, "
                           "doviz_kuru, parite, muh_kasa_hesap_id, modul_id, program_id, base_fis_id, "
                           "last_order_number, muh_fis_id, yazdirildi_mi, iptal_mi " );

    query.SET_VALUE ( ":fis_no",                    P_FIS->fis_no                   );
    query.SET_VALUE ( ":fis_tarihi",                P_FIS->fis_tarihi               );
    query.SET_VALUE ( ":cari_hesap_id",             P_FIS->cari_hesap_id            );
    query.SET_VALUE ( ":aciklama",                  P_FIS->aciklama                 );
    query.SET_VALUE ( ":toplam_borc",               0                               );
    query.SET_VALUE ( ":toplam_alacak",             0                               );
    query.SET_VALUE ( ":belge_seri",                P_FIS->belge_seri               );
    query.SET_VALUE ( ":belge_numarasi",            P_FIS->belge_numarasi           );
    query.SET_VALUE ( ":fis_turu",                  P_FIS->fis_turu                 );
    query.SET_VALUE ( ":makbuz_para_birimi_id",     P_FIS->makbuz_para_birimi_id    );
    query.SET_VALUE ( ":doviz_kuru",                ROUND ( P_FIS->doviz_kuru , -4) );
    query.SET_VALUE ( ":parite",                    ROUND ( P_FIS->parite     , -4) );
    query.SET_VALUE ( ":muh_kasa_hesap_id",         P_FIS->muh_kasa_hesap_id        );
    query.SET_VALUE ( ":modul_id",                  P_FIS->modul_id                 );
    query.SET_VALUE ( ":program_id",                P_FIS->program_id               );
    query.SET_VALUE ( ":base_fis_id",               P_FIS->base_fis_id              );
    query.SET_VALUE ( ":muh_fis_id" ,               P_FIS->muh_fis_id               );
    query.SET_VALUE ( ":yazdirildi_mi" ,            0                               );
    query.SET_VALUE ( ":iptal_mi" ,                 0                               );
    query.SET_VALUE ( ":last_order_number" ,        0                               );

    int fis_id = query.INSERT();

    if ( P_FIS->fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
        return fis_id;
    }

    if ( P_FIS->muh_fis_id EQ 0 AND P_FIS->modul_id EQ CARI_MODULU ) {

        MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT(MUH_FIS);

        QString aciklama = QObject::tr("Fiş CARİ modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
        aciklama += P_FIS->aciklama;

        MUH_FIS->aciklama           = aciklama;
        MUH_FIS->fis_tarihi         = P_FIS->fis_tarihi;
        MUH_FIS->fis_turu           = ENUM_MAHSUP_FISI;
        MUH_FIS->modul_id           = P_FIS->modul_id;
        MUH_FIS->program_id         = P_FIS->program_id;
        MUH_FIS->base_fis_id        = fis_id;

        P_FIS->muh_fis_id = MUH_FIS_EKLE ( MUH_FIS , USE_DB);

        delete MUH_FIS;

        query.PREPARE_UPDATE("car_fisler","fis_id", "muh_fis_id", "fis_id = :fis_id");
        query.SET_VALUE     (":muh_fis_id" , P_FIS->muh_fis_id );
        query.SET_VALUE     (":fis_id"     , fis_id);
        query.UPDATE();

    }
    return fis_id;
}

/***************************************************************************************
                   CARI_FIS_SATIRI_EKLE
***************************************************************************************/

int CARI_FIS_SATIRI_EKLE ( CARI_FIS_STRUCT * P_FIS, CARI_FIS_SATIRI_STRUCT * P_FIS_SATIRI, bool base_fis_satiri_eklenecek ,
                           int p_fis_satiri_id , int *p_muh_olusan_fis_satiri_id, ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query (USE_DB );

    query.PREPARE_SELECT ( "car_fisler", "toplam_borc, toplam_alacak, last_order_number, fis_tarihi, cari_hesap_id, "
                           "fis_turu, makbuz_para_birimi_id, doviz_kuru, parite, modul_id, program_id, "
                           "aciklama, muh_kasa_hesap_id, base_fis_id  ",
                           "fis_id = :fis_id" );

    query.SET_VALUE      ( ":fis_id", P_FIS_SATIRI->fis_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    int     modul_id                =   query.VALUE         ( "modul_id"    ).toInt();
    int     program_id              =   query.VALUE         ( "program_id"  ).toInt();
    int     base_fis_id             =   query.VALUE         ( "base_fis_id" ).toInt();

    if ( modul_id NE P_FIS->modul_id OR program_id NE P_FIS->program_id
         OR P_FIS->base_fis_id NE base_fis_id ) {
         return 0;
    }

    double  toplam_borc             =   query.VALUE         ( "toplam_borc"         ).toDouble  ();
    double  toplam_alacak           =   query.VALUE         ( "toplam_alacak"       ).toDouble  ();
    int     last_order_number       =   query.VALUE         ( "last_order_number"   ).toInt     ();
    int     cari_hesap_id           =   query.VALUE         ( "cari_hesap_id"       ).toInt();
    int     fis_turu                =   query.VALUE         ( "fis_turu"            ).toInt();
    QDate   fis_tarihi              =   QDate::fromString   ( query.VALUE ( "fis_tarihi" ).toString(), "yyyy.MM.dd" );
    int     gun_no                  =   MALI_YIL_ARRAY_INDIS( fis_tarihi );
    int     para_birimi_id          =   query.VALUE         ( "makbuz_para_birimi_id" ).toInt();
    double  doviz_kuru              =   query.VALUE         ( "doviz_kuru"            ).toDouble();
    double  parite                  =   query.VALUE         ( "parite"                ).toDouble();
    int     muh_kasa_hesap_id       =   query.VALUE         ( "muh_kasa_hesap_id"     ).toInt();
    int     temel_para_birimi_id    =   DVZ_GET_TEMEL_PARA_BIRIMI_ID();


    if ( P_FIS_SATIRI->order_number EQ 0 ) {
        P_FIS_SATIRI->order_number = last_order_number + 1024;
    }

    switch ( fis_turu ) {

        case ENUM_CARI_HAREKET_FISI  :
        case ENUM_CARI_ACILIS_FISI   :
        case ENUM_CARI_VIRMAN_FORMU  :

            cari_hesap_id  = P_FIS_SATIRI->cari_hesap_id;

            if ( P_FIS_SATIRI->doviz_id NE 0 ) {
                para_birimi_id      = P_FIS_SATIRI->doviz_id;
            }
            doviz_kuru          = P_FIS_SATIRI->doviz_kuru;
            parite              = P_FIS_SATIRI->parite;

            break;
        default :
            break;
    }

    //! CARI DOVIZ KURU
    if( P_FIS->makbuz_para_birimi_id EQ P_FIS->cari_para_birimi_id  ) {
        if( P_FIS->makbuz_para_birimi_id NE temel_para_birimi_id ) {
            doviz_kuru = P_FIS->doviz_kuru;
        }
        else {
            doviz_kuru = 1.000;
        }
    }
    else {
        if( P_FIS->cari_para_birimi_id EQ temel_para_birimi_id ) {
            doviz_kuru = 1.000;
        }
        else {
            if( P_FIS->makbuz_para_birimi_id EQ temel_para_birimi_id ){
                doviz_kuru = P_FIS->doviz_kuru;
            }
            else {
                doviz_kuru = ROUND( P_FIS->doviz_kuru *  ( 1 / P_FIS->parite ) , 4 ); // TL / E * 1 / ( E / $ )= TL / $
            }
        }
    }
    //! END CARI DOVIZ KURU

    double      satir_tutari = 0.0;

    if ( P_FIS_SATIRI->borc_tutari > 0 ) {
        satir_tutari = P_FIS_SATIRI->borc_tutari;
    }
    else {
        satir_tutari = P_FIS_SATIRI->alacak_tutari;
    }

    int     cari_para_birimi_id     =   CARI_GET_PARA_BIRIM_ID ( cari_hesap_id );
    double  cari_para_birimi_tutari =   CARI_GET_PARA_BIRIMI_TUTARI ( cari_para_birimi_id, para_birimi_id, doviz_kuru, parite, satir_tutari );

    double      cari_para_birimi_borc_tutari    = 0.0;
    double      cari_para_birimi_alacak_tutari  = 0.0;
    double      temel_para_birimi_tutari        = 0.0;

    switch ( fis_turu ) {
        case ENUM_CARI_ACILIS_FISI :
            cari_para_birimi_borc_tutari    =  P_FIS_SATIRI->borc_tutari;
            cari_para_birimi_alacak_tutari  =  P_FIS_SATIRI->alacak_tutari;
            if ( P_FIS_SATIRI->borc_tutari > 0 ) {
                cari_para_birimi_tutari  = cari_para_birimi_borc_tutari;
                temel_para_birimi_tutari = cari_para_birimi_borc_tutari;
            }
            else {
                cari_para_birimi_tutari  = cari_para_birimi_alacak_tutari;
                temel_para_birimi_tutari = cari_para_birimi_alacak_tutari;
            }
            break;

        default :

            if ( P_FIS_SATIRI->borc_tutari > 0 ) {
                cari_para_birimi_borc_tutari = cari_para_birimi_tutari;
            }
            else {
                cari_para_birimi_alacak_tutari = cari_para_birimi_tutari;
            }

            if ( P_FIS_SATIRI->borc_tutari > 0 ) {
                if (  para_birimi_id EQ temel_para_birimi_id ) {
                    temel_para_birimi_tutari = P_FIS_SATIRI->borc_tutari;
                }
                else {
                    temel_para_birimi_tutari = P_FIS_SATIRI->borc_tutari * doviz_kuru;
                }
            }
            else {
                if (  para_birimi_id EQ temel_para_birimi_id ) {
                    temel_para_birimi_tutari = P_FIS_SATIRI->alacak_tutari;
                }
                else {
                    temel_para_birimi_tutari = P_FIS_SATIRI->alacak_tutari * doviz_kuru;
                }
            }
            break;
    }

    int fis_satiri_id = p_fis_satiri_id;

    //! e9 onardan gelmis ise hareket eklemesi lazim
    bool cek_senet_hareket_ekle = false;

    if ( base_fis_satiri_eklenecek EQ true ) {

        cek_senet_hareket_ekle = true;

        if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
            //! MUSTERI CEKI VEYA SENEDI ISE CEK SENET MODULUNE FIS URETMESI LAZIM.
            if ( P_FIS_SATIRI->islem_turu EQ ENUM_MUSTERI_CEKI OR
                 P_FIS_SATIRI->islem_turu EQ ENUM_MUSTERI_SENEDI) {

                int cek_senet_turu = ENUM_CEK;
                if ( P_FIS_SATIRI->islem_turu EQ ENUM_MUSTERI_SENEDI ) {
                    cek_senet_turu = ENUM_SENET;
                }

                CEK_SENET_STRUCT * CEK_SENET = new CEK_SENET_STRUCT;

                CEK_SENET->portfoy_numarasi         =   0;
                CEK_SENET->banka_ismi               =   P_FIS_SATIRI->musteri_banka_ismi;
                CEK_SENET->sube_kodu                =   P_FIS_SATIRI->musteri_sube_kodu;
                CEK_SENET->sube_ismi                =   P_FIS_SATIRI->musteri_sube_ismi;
                CEK_SENET->hesap_no_id              =   P_FIS_SATIRI->hesap_no_id;
                CEK_SENET->cek_senet_numarasi       =   P_FIS_SATIRI->cek_senet_no;
                CEK_SENET->vade_tarihi              =   P_FIS_SATIRI->cek_senet_vade_tarihi;
                CEK_SENET->keside_tarihi            =   P_FIS_SATIRI->cek_senet_keside_tarihi;
                CEK_SENET->borclu_kesideci          =   P_FIS_SATIRI->kesideci;
                CEK_SENET->musteri_ceki_mi          =   1;
                CEK_SENET->cek_senet_tutari         =   P_FIS_SATIRI->alacak_tutari;
                CEK_SENET->musteri_hesap_numarasi   =   P_FIS_SATIRI->musteri_hesap_no;
                CEK_SENET->cek_senet_turu           =   cek_senet_turu;
                CEK_SENET->doviz_id                 =   P_FIS_SATIRI->doviz_id;

                P_FIS_SATIRI->cek_senet_id = CEK_SENET_EKLE ( CEK_SENET );
                P_FIS_SATIRI->portfoy_no   = CEK_SENET->portfoy_numarasi;

                delete CEK_SENET;
            }
        }


        query.PREPARE_INSERT ( "car_fis_satirlari", "fis_satiri_id", "fis_id, aciklama, borc_tutari, alacak_tutari, "
                               "kredi_karti_numarasi, kredi_karti_sahibi, musteri_banka_ismi, musteri_sube_kodu, "
                               "musteri_sube_ismi, musteri_hesap_no, cari_hesap_id, cek_senet_id, islem_turu, order_number, "
                               "temel_para_birimi_tutari, muh_hesap_id, hesap_no_id, pos_id, kredi_karti_id, "
                               "cari_para_birimi_tutari, doviz_id, doviz_kuru, parite, program_id, modul_id, "
                               "base_record_id" );

        query.SET_VALUE ( ":fis_id",                    P_FIS_SATIRI->fis_id                    );
        query.SET_VALUE ( ":aciklama",                  P_FIS_SATIRI->aciklama                  );
        query.SET_VALUE ( ":borc_tutari",               ROUND ( P_FIS_SATIRI->borc_tutari   )   );
        query.SET_VALUE ( ":alacak_tutari",             ROUND ( P_FIS_SATIRI->alacak_tutari )   );
        query.SET_VALUE ( ":kredi_karti_numarasi",      P_FIS_SATIRI->kredi_karti_numarasi      );
        query.SET_VALUE ( ":kredi_karti_sahibi",        P_FIS_SATIRI->kredi_karti_sahibi        );
        query.SET_VALUE ( ":musteri_banka_ismi",        P_FIS_SATIRI->musteri_banka_ismi        );
        query.SET_VALUE ( ":musteri_sube_kodu",         P_FIS_SATIRI->musteri_sube_kodu         );
        query.SET_VALUE ( ":musteri_sube_ismi",         P_FIS_SATIRI->musteri_sube_ismi         );
        query.SET_VALUE ( ":musteri_hesap_no",          P_FIS_SATIRI->musteri_hesap_no          );
        query.SET_VALUE ( ":cari_hesap_id",             P_FIS_SATIRI->cari_hesap_id             );
        query.SET_VALUE ( ":cek_senet_id",              P_FIS_SATIRI->cek_senet_id              );
        query.SET_VALUE ( ":islem_turu",                P_FIS_SATIRI->islem_turu                );
        query.SET_VALUE ( ":order_number",              P_FIS_SATIRI->order_number              );
        query.SET_VALUE ( ":temel_para_birimi_tutari",  ROUND(temel_para_birimi_tutari     )    );
        query.SET_VALUE ( ":muh_hesap_id",              P_FIS_SATIRI->muh_hesap_id              );
        query.SET_VALUE ( ":hesap_no_id",               P_FIS_SATIRI->hesap_no_id               );
        query.SET_VALUE ( ":pos_id",                    P_FIS_SATIRI->pos_id                    );
        query.SET_VALUE ( ":kredi_karti_id",            P_FIS_SATIRI->kredi_karti_id            );
        query.SET_VALUE ( ":cari_para_birimi_tutari",   cari_para_birimi_tutari                 );
        query.SET_VALUE ( ":doviz_id",                  P_FIS_SATIRI->doviz_id                  );
        query.SET_VALUE ( ":doviz_kuru",                ROUND(P_FIS_SATIRI->doviz_kuru , -4)    );
        query.SET_VALUE ( ":parite",                    ROUND(P_FIS_SATIRI->parite     , -4)    );
        query.SET_VALUE ( ":program_id",                P_FIS_SATIRI->program_id                );
        query.SET_VALUE ( ":modul_id",                  P_FIS_SATIRI->modul_id                  );
        query.SET_VALUE ( ":base_record_id",            P_FIS_SATIRI->base_record_id            );

        fis_satiri_id = query.INSERT();

        toplam_borc         = ROUND ( toplam_borc   + P_FIS_SATIRI->borc_tutari );
        toplam_alacak       = ROUND ( toplam_alacak + P_FIS_SATIRI->alacak_tutari );


        if ( P_FIS_SATIRI->borc_tutari EQ 0.0 ) {
            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_ARTIR  ( P_FIS_SATIRI->doviz_kodu, P_FIS_SATIRI->alacak_tutari,
                                                               0, P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                               P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
        }
        else if ( P_FIS_SATIRI->alacak_tutari EQ 0.0 ) {
            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_ARTIR  ( P_FIS_SATIRI->doviz_kodu, P_FIS_SATIRI->borc_tutari,
                                                               1, P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                               P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
        }


        P_FIS->borc_doviz_tutar_bilgileri   = CARI_GET_HAREKET_FISI_SATIR_DOVIZ_BORC_TUTARLARI   ( P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI     );
        P_FIS->alacak_doviz_tutar_bilgileri = CARI_GET_HAREKET_FISI_SATIR_DOVIZ_ALACAK_TUTARLARI ( P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI   );
    }

    if ( modul_id EQ CARI_MODULU OR program_id NE E9_PROGRAMI ) {
        //SM MAKBUZ modulunden de base record id 0 gonderiliyor.O yuzden module bakiliyor.

        if ( P_FIS_SATIRI->base_record_id EQ 0 ) {
            P_FIS_SATIRI->base_record_id = fis_satiri_id;
        }
        if ( base_fis_id EQ 0 ) {
            base_fis_id = P_FIS_SATIRI->fis_id;
        }
    }


    //ENTEGRASYON

    bool cari_entegrasyon_fisi_olusturulacak   = true;
    bool karsi_hesap_muhasebe_satiri_eklenecek = false;

    if ( fis_turu EQ ENUM_CARI_HAREKET_FISI ) {
        karsi_hesap_muhasebe_satiri_eklenecek = true;
    }

    switch ( P_FIS_SATIRI->islem_turu ) {
        case ENUM_MUSTERI_CEKI    :
        case ENUM_MUSTERI_SENEDI  :
        case ENUM_KENDI_CEKIMIZ   :
        case ENUM_KENDI_SENEDIMIZ :
        {
            if ( P_FIS->modul_id EQ CARI_MODULU ) {
                //! cek senet muhasebeye hem borc hem alacak gonderiyor burdan tekrar
                //! cari gondermemeli
                cari_entegrasyon_fisi_olusturulacak = false;

                if ( P_FIS_SATIRI->alacak_tutari > 0 ) {
                    CARI_CEKSENET_MODULUNE_FIS_SATIR_EKLE( P_FIS , P_FIS_SATIRI,temel_para_birimi_tutari,
                                                     cari_para_birimi_tutari , base_fis_id , cek_senet_hareket_ekle, USE_DB);
                }
            }
            break;
        }
        case ENUM_HAVALE :
        case ENUM_EFT:
        case ENUM_KREDI_KARTI:
        {
            cari_entegrasyon_fisi_olusturulacak = true ;

            if ( P_FIS_SATIRI->alacak_tutari > 0 ) {
                CARI_BANKA_MODULUNE_FIS_SATIR_EKLE(P_FIS , P_FIS_SATIRI , cari_hesap_id , base_fis_id ,USE_DB);
            }
            break;
        }
        case ENUM_NAKIT :
            karsi_hesap_muhasebe_satiri_eklenecek = true;
            break;

        default :
            break;
    }

    if ( fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
        cari_entegrasyon_fisi_olusturulacak = false;
        gun_no = 0;
    }

    if ( cari_entegrasyon_fisi_olusturulacak EQ true ) {

        int cari_muh_hesap_id = CARI_FIND_HESAP_MUH_HESAP_ID ( cari_hesap_id );

        MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

        MUH_FIS_BILGILERINI_OKU ( MUH_FIS, P_FIS->muh_fis_id,USE_DB );


        if ( cari_muh_hesap_id EQ 0 ) {
            MUH_FIS_TUM_SATIRLARINI_SIL ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);
            MUH_FIS->aciklama   = QObject::tr("Cari hesabın muhasebe hesabı bulunamadı. \n" ) + P_FIS->aciklama;
            MUH_FISI_GUNCELLE ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);
        }
        else {
            //MUHASEBE FISI BORC SATIRI EKLENIYOR
            //Muhasebe modulune fis satiri eklenirken once borc daha sonra alacak satiri eklenmelidir.

            double      muh_fis_satiri_alacak_tutari    =   0.0;
            double      muh_fis_satiri_borc_tutari      =   0.0;


            if ( P_FIS_SATIRI->borc_tutari > 0 ) {
                muh_fis_satiri_borc_tutari      = temel_para_birimi_tutari;
            }
            else {
                muh_fis_satiri_alacak_tutari    = temel_para_birimi_tutari;
            }


            MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

            MUH_CLEAR_FIS_SATIRI_STRUCT     ( MUH_FIS_SATIRI );

            MUH_FIS_SATIRI->aciklama                = P_FIS_SATIRI->aciklama;
            MUH_FIS_SATIRI->borc_tutari             = muh_fis_satiri_borc_tutari;
            MUH_FIS_SATIRI->alacak_tutari           = muh_fis_satiri_alacak_tutari;
            MUH_FIS_SATIRI->fis_id                  = P_FIS->muh_fis_id;
            MUH_FIS_SATIRI->hesap_id                = cari_muh_hesap_id;
            MUH_FIS_SATIRI->program_id              = P_FIS_SATIRI->program_id;
            MUH_FIS_SATIRI->modul_id                = P_FIS_SATIRI->modul_id;
            MUH_FIS_SATIRI->base_record_id          = P_FIS_SATIRI->base_record_id;

            if ( karsi_hesap_muhasebe_satiri_eklenecek EQ true ) {

                if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                    MUH_FIS_SATIRI->hesap_id      = muh_kasa_hesap_id;
                    MUH_FIS_SATIRI->borc_tutari   = temel_para_birimi_tutari;
                    MUH_FIS_SATIRI->alacak_tutari = 0.0;
                }
                else if ( fis_turu EQ ENUM_CARI_HAREKET_FISI ) {

                    if ( P_FIS_SATIRI->muh_hesap_id > 0 ) {
                        if ( P_FIS_SATIRI->alacak_tutari > 0 ) {
                            MUH_FIS_SATIRI->hesap_id      = P_FIS_SATIRI->muh_hesap_id;
                            MUH_FIS_SATIRI->borc_tutari   = temel_para_birimi_tutari;
                            MUH_FIS_SATIRI->alacak_tutari = 0.0;
                        }
                    }
                }
            }

            int muh_fis_satiri_id = MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI  ,USE_DB );
            if( p_muh_olusan_fis_satiri_id NE NULL ) {
                *p_muh_olusan_fis_satiri_id = muh_fis_satiri_id;
            }

            if ( karsi_hesap_muhasebe_satiri_eklenecek EQ true ) {

                if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                    MUH_FIS_SATIRI->hesap_id      = cari_muh_hesap_id;
                    MUH_FIS_SATIRI->borc_tutari   = 0.0;
                    MUH_FIS_SATIRI->alacak_tutari = temel_para_birimi_tutari;

                    MUH_FIS_SATIRI_EKLE(MUH_FIS,MUH_FIS_SATIRI , USE_DB);
                }
                else  if ( fis_turu EQ ENUM_CARI_ODEME_FISI ) { //Odeme Fisi
                    MUH_FIS_SATIRI->hesap_id      = muh_kasa_hesap_id;
                    MUH_FIS_SATIRI->borc_tutari   = 0.0;
                    MUH_FIS_SATIRI->alacak_tutari = temel_para_birimi_tutari;

                    MUH_FIS_SATIRI_EKLE(MUH_FIS,MUH_FIS_SATIRI , USE_DB);
                }

                if ( fis_turu EQ ENUM_CARI_HAREKET_FISI ) {

                    if ( P_FIS_SATIRI->muh_hesap_id > 0 ) {
                        if ( P_FIS_SATIRI->borc_tutari > 0 ) {
                            MUH_FIS_SATIRI->hesap_id = P_FIS_SATIRI->muh_hesap_id;
                        }
                        else {
                            MUH_FIS_SATIRI->hesap_id = cari_muh_hesap_id;
                        }
                        MUH_FIS_SATIRI->borc_tutari   = 0.0;
                        MUH_FIS_SATIRI->alacak_tutari = temel_para_birimi_tutari;

                        MUH_FIS_SATIRI_EKLE(MUH_FIS,MUH_FIS_SATIRI , USE_DB);
                    }
                }
            }
            delete MUH_FIS_SATIRI;
        }
        delete MUH_FIS;

    }

    switch ( P_FIS_SATIRI->islem_turu ) {
        case ENUM_MUSTERI_CEKI    :
        case ENUM_MUSTERI_SENEDI  :
        case ENUM_KENDI_CEKIMIZ   :
        case ENUM_KENDI_SENEDIMIZ :
        {
            if ( P_FIS->modul_id EQ CARI_MODULU ) {
                if ( P_FIS_SATIRI->borc_tutari > 0 ) {
                    CARI_CEKSENET_MODULUNE_FIS_SATIR_EKLE(P_FIS,P_FIS_SATIRI,temel_para_birimi_tutari,
                                                     cari_para_birimi_tutari,base_fis_id , cek_senet_hareket_ekle, USE_DB);
                }
            }

            break;
        }
        case ENUM_HAVALE     :
        case ENUM_EFT        :
        case ENUM_KREDI_KARTI:
        {
            if ( P_FIS_SATIRI->borc_tutari > 0 ) {
                CARI_BANKA_MODULUNE_FIS_SATIR_EKLE(P_FIS , P_FIS_SATIRI,cari_hesap_id ,base_fis_id , USE_DB);
            }
            break;
        }

        default :
            break;
    }


    if ( base_fis_satiri_eklenecek EQ true ) {

        query.PREPARE_UPDATE ( "car_fisler ","fis_id",
                               "toplam_borc         , "
                               "toplam_alacak           , "
                               "last_order_number       , "
                               "doviz_tutar_bilgileri     ",
                               "fis_id     = :fis_id" );

        query.SET_VALUE      ( ":toplam_borc",              ROUND ( toplam_borc   )      );
        query.SET_VALUE      ( ":toplam_alacak",            ROUND ( toplam_alacak )     );
        query.SET_VALUE      ( ":last_order_number",        P_FIS_SATIRI->order_number      );
        query.SET_VALUE      ( ":doviz_tutar_bilgileri",    P_FIS->borc_doviz_tutar_bilgileri + "#" +
                                                            P_FIS->alacak_doviz_tutar_bilgileri );
        query.SET_VALUE      ( ":fis_id",                   P_FIS_SATIRI->fis_id    );

        query.UPDATE         ();

        P_FIS->toplam_borc      = toplam_borc;
        P_FIS->toplam_alacak    = toplam_alacak;

        CARI_HESABIN_BAKIYESINI_ARTTIR  ( gun_no, cari_hesap_id, cari_para_birimi_borc_tutari, cari_para_birimi_alacak_tutari, USE_DB );
    }

    return fis_satiri_id;
}

/***************************************************************************************
                   CARI_FIS_SATIRI_SIL
***************************************************************************************/

int CARI_FIS_SATIRI_SIL ( CARI_FIS_STRUCT * P_FIS, int p_fis_satiri_id ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT ( "car_fisler, car_fis_satirlari ",
                           "car_fisler.fis_id, fis_tarihi, car_fisler.cari_hesap_id, toplam_borc, toplam_alacak, "
                           "cari_para_birimi_tutari, borc_tutari, alacak_tutari, car_fis_satirlari.cari_hesap_id, "
                           "fis_turu, car_fis_satirlari.modul_id, car_fis_satirlari.program_id, doviz_id, islem_turu, "
                           "cek_senet_id,  base_record_id, "
                           "base_fis_id, car_fis_satirlari.hesap_no_id,muh_fis_id ",
                           "car_fisler.fis_id = car_fis_satirlari.fis_id "
                           "AND fis_satiri_id = :fis_satiri_id" );

    query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT  ();

    int     fis_id                  =   query.VALUE ( 0  ).toInt();
    int     modul_id                =   query.VALUE ( 10 ).toInt();
    int     program_id              =   query.VALUE ( 11 ).toInt();
    int     base_fis_id             =   query.VALUE ( "base_fis_id" ).toInt();


    if ( modul_id NE P_FIS->modul_id OR program_id NE P_FIS->program_id OR
        P_FIS->base_fis_id NE base_fis_id ) {
        return fis_id;
    }
    QDate   fis_tarihi                  = QDate::fromString ( query.VALUE ( "fis_tarihi" ).toString(), "yyyy.MM.dd" );
    int     fis_cari_hesap_id           = query.VALUE ( 2 ).toInt();
    int     fis_satiri_cari_hesap_id    = query.VALUE ( 8 ).toInt();
    double  toplam_borc                 = query.VALUE ( "toplam_borc"               ).toDouble();
    double  toplam_alacak               = query.VALUE ( "toplam_alacak"             ).toDouble();
    double  satir_borc_tutari           = query.VALUE ( "borc_tutari"               ).toDouble();
    double  satir_alacak_tutari         = query.VALUE ( "alacak_tutari"             ).toDouble();
    double  cari_para_birimi_tutari     = query.VALUE ( "cari_para_birimi_tutari"   ).toDouble();
    double  cari_para_birimi_borc       = 0.0;
    double  cari_para_birimi_alacak     = 0.0;
    int     fis_turu                    = query.VALUE ( "fis_turu"                  ).toDouble();
    int     cari_hesap_id               = 0;
    int     gun_no                      = MALI_YIL_ARRAY_INDIS ( fis_tarihi );
    QString doviz_kodu                  = DVZ_GET_DOVIZ_KODU ( query.VALUE ( "doviz_id" ).toInt() );
    int     islem_turu                  = query.VALUE ( "islem_turu"           ).toInt();
    int     cek_senet_id                = query.VALUE ( "cek_senet_id"         ).toInt();
    int     muh_fis_id                  = MUH_GET_ENT_FIS_ID ( P_FIS->program_id, P_FIS->modul_id, fis_id );
    int     base_record_id              = query.VALUE ( "base_record_id" ).toInt();
    int     hesap_no_id                 = query.VALUE ( "hesap_no_id" ).toInt();
    P_FIS->muh_fis_id                   = query.VALUE("muh_fis_id").toInt();

    // E9 programi sys den geldiginde kapora iptal edilmemesi icin 0 geliyor. onun  icin if girmesi gerekiyor
    if ( P_FIS->modul_id EQ CARI_MODULU OR P_FIS->program_id NE E9_PROGRAMI ) {
        if ( base_fis_id EQ 0 ) {
            base_fis_id = fis_id;
        }

        if ( base_record_id EQ 0 ) {
            base_record_id = p_fis_satiri_id;
        }
    }


    switch ( fis_turu ) {

        case ENUM_CARI_HAREKET_FISI  :
        case ENUM_CARI_ACILIS_FISI   :
        case ENUM_CARI_VIRMAN_FORMU  :
            cari_hesap_id = fis_satiri_cari_hesap_id;
            break;

        default :
            cari_hesap_id = fis_cari_hesap_id;
            break;
    }

    if ( satir_borc_tutari EQ 0.0 ) {
        CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_AZALT  ( doviz_kodu, satir_alacak_tutari, 0,
                                                           P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                           P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
    }
    else if ( satir_alacak_tutari EQ 0.0 ) {
        CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_AZALT  ( doviz_kodu, satir_borc_tutari, 1,
                                                           P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                           P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
    }

    P_FIS->borc_doviz_tutar_bilgileri       = CARI_GET_HAREKET_FISI_SATIR_DOVIZ_BORC_TUTARLARI      ( P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI   );
    P_FIS->alacak_doviz_tutar_bilgileri     = CARI_GET_HAREKET_FISI_SATIR_DOVIZ_ALACAK_TUTARLARI    ( P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );

    bool cari_entegrasyon_fisi_silinecek = true;

    switch ( islem_turu ) {

        //Asagidaki fonksiyonlara USE_DB gonderilmedi cunku sadece nakit islem icin
     //Fatura modulunden cari fis_satiri sil fonksiyonu devir sirasinda cagiriliyor,

        case ENUM_MUSTERI_CEKI    :
        case ENUM_MUSTERI_SENEDI  :
        case ENUM_KENDI_CEKIMIZ   :
        case ENUM_KENDI_SENEDIMIZ :

            if ( P_FIS->modul_id EQ CARI_MODULU ) {

                cari_entegrasyon_fisi_silinecek = false;

                if ( cek_senet_id > 0 ) {

                    CEK_BORDRO_STRUCT * CEK_BORDROSU = new CEK_BORDRO_STRUCT;

                    CEK_BORDROSU->program_id  = P_FIS->program_id;
                    CEK_BORDROSU->modul_id    = P_FIS->modul_id;
                    CEK_BORDROSU->base_fis_id = base_fis_id;
                    CEK_BORDROSU->muh_fis_id  = muh_fis_id;

                    QStringList BORDRO_SATIRI_ID_LIST = CEK_GET_ENT_ROW_ID_LIST ( P_FIS->program_id, P_FIS->modul_id, base_record_id );

                    if ( BORDRO_SATIRI_ID_LIST.size() > 0 ) {
                        CEK_BORDRO_SATIRINI_SIL ( CEK_BORDROSU , QVariant ( BORDRO_SATIRI_ID_LIST.at ( 0 ) ).toInt() );
                    }

                    if ( P_FIS->fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                        CEK_SENET_SIL ( cek_senet_id );
                    }
                }
            }
            break;

        case ENUM_HAVALE :
        case ENUM_EFT:
        case ENUM_KREDI_KARTI:
            {

                QStringList BNK_FIS_SATIRI_ID_LIST  =  BNK_GET_ENT_ROW_ID_LIST( P_FIS->program_id, P_FIS->modul_id, base_record_id );

                if ( BNK_FIS_SATIRI_ID_LIST.size() > 0 ) {


                    BNK_HAREKET_FIS_STRUCT  * BNK_FIS = new BNK_HAREKET_FIS_STRUCT;

                    BNK_FIS->base_fis_id        = base_fis_id;
                    BNK_FIS->program_id         = P_FIS->program_id;
                    BNK_FIS->modul_id           = P_FIS->modul_id;
                    BNK_FIS->hesap_no_id        = hesap_no_id;
                    BNK_FIS->muh_fis_id         = muh_fis_id;

                    for ( int i = 0; i < BNK_FIS_SATIRI_ID_LIST.size(); i++ ) {
                        BNK_HAREKET_FIS_SATIRI_SIL( BNK_FIS, QVariant ( BNK_FIS_SATIRI_ID_LIST.at(i) ).toInt() );
                    }

                    delete BNK_FIS;
                }
            }

        default :
            break;
    }

    if ( cari_entegrasyon_fisi_silinecek EQ true ) {

        MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

        MUH_FIS->modul_id    = P_FIS->modul_id;
        MUH_FIS->program_id  = P_FIS->program_id;
        MUH_FIS->base_fis_id = base_fis_id;

        switch ( fis_turu ) {

            case ENUM_CARI_TAHSILAT_FISI :
            case ENUM_CARI_ODEME_FISI    :

                MUH_ENTEGRASYON_FIS_SATIRLARINI_SIL ( MUH_FIS, muh_fis_id, p_fis_satiri_id );
                break;
            default :
                QStringList MUH_FIS_SATIRI_ID_LIST = MUH_GET_FISE_GORE_ENT_ROW_ID_LIST( P_FIS->program_id, P_FIS->modul_id, base_record_id,P_FIS->muh_fis_id ,USE_DB);
                for ( int i = 0; i < MUH_FIS_SATIRI_ID_LIST.size(); i++ ) {
                    int muh_fis_satiri_id = QVariant(MUH_FIS_SATIRI_ID_LIST.at(i)).toInt();

                    MUH_FIS_SATIRINI_SIL ( MUH_FIS, muh_fis_satiri_id ,USE_DB);
                }
                break;
        }

        delete MUH_FIS;
    }
    // FIS YAZILMIS ISE SATIRLARI DURMAK ZORUNDA AMA HAREKETLERI SILINMELI
    if ( P_FIS->yazdirildi_mi EQ ENUM_FIS_YAZILMADI ) {
        query.PREPARE_DELETE ( "car_fis_satirlari","fis_satiri_id = :fis_satiri_id" );
        query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );

        query.DELETE();
    }

    if ( satir_borc_tutari > 0 ) {
        cari_para_birimi_borc = cari_para_birimi_tutari;
    }
    else {
        cari_para_birimi_alacak = cari_para_birimi_tutari;
    }

    if ( fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
        gun_no = 0;
    }

    CARI_HESABIN_BAKIYESINI_AZALT ( gun_no, cari_hesap_id, cari_para_birimi_borc, cari_para_birimi_alacak, USE_DB);

    toplam_borc     = ROUND ( toplam_borc   - satir_borc_tutari   );
    toplam_alacak   = ROUND ( toplam_alacak - satir_alacak_tutari );

    query.PREPARE_UPDATE ( "car_fisler ","fis_id",
                           "toplam_borc             , "
                           "toplam_alacak           , "
                           "doviz_tutar_bilgileri     ",
                           "fis_id    = :fis_id" );

    query.SET_VALUE      ( ":toplam_borc"          , toplam_borc    );
    query.SET_VALUE      ( ":toplam_alacak"        , toplam_alacak  );
    query.SET_VALUE      ( ":doviz_tutar_bilgileri", P_FIS->borc_doviz_tutar_bilgileri + "#" +
                           P_FIS->alacak_doviz_tutar_bilgileri      );
    query.SET_VALUE      ( ":fis_id"               , fis_id         );
    query.UPDATE         ();

    P_FIS->toplam_borc      = toplam_borc;
    P_FIS->toplam_alacak    = toplam_alacak;

    return fis_id;

}

/***************************************************************************************
                   CARI_FISI_SIL
***************************************************************************************/

void CARI_FISI_SIL ( CARI_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT ( "car_fisler", "modul_id, program_id , base_fis_id,muh_fis_id ",
                           "fis_id = :fis_id " );
    query.SET_VALUE      ( ":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int     modul_id           =   query.VALUE ( 0 ).toInt();
    int     program_id         =   query.VALUE ( 1 ).toInt();
    int     base_fis_id        =   query.VALUE ( 2 ).toInt();
    int     muh_fis_id         =   query.VALUE ( 3 ).toInt();


    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR
         P_FIS->base_fis_id NE base_fis_id) {
        return;
    }

    if ( P_FIS->modul_id EQ CARI_MODULU ) {
        if ( base_fis_id EQ 0 ) {
            base_fis_id = p_fis_id;
        }
    }



    if ( modul_id NE BANKA_MODULU ) {
        int bnk_fis_id  =  BNK_GET_ENT_FIS_ID( P_FIS->program_id, P_FIS->modul_id, P_FIS->base_fis_id );

        if( bnk_fis_id NE 0 ) {

            BNK_HAREKET_FIS_STRUCT  * BNK_FIS = new BNK_HAREKET_FIS_STRUCT;

            BNK_FIS->base_fis_id        = base_fis_id;
            BNK_FIS->program_id         = P_FIS->program_id;
            BNK_FIS->modul_id           = P_FIS->modul_id;
            BNK_FIS->muh_fis_id         = muh_fis_id;

            BNK_HAREKET_FISI_SIL( BNK_FIS, bnk_fis_id );

            delete BNK_FIS;
        }
    }

    query.PREPARE_SELECT ( "car_fis_satirlari","fis_satiri_id", "fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id", p_fis_id );

    if ( query.SELECT() > 0 ) {
        while ( query.NEXT() EQ true ) {

            int         fis_satiri_id           = query.VALUE ( 0 ).toInt();
            QStringList BORDRO_SATIRI_ID_LIST   = CEK_GET_ENT_ROW_ID_LIST ( P_FIS->program_id, P_FIS->modul_id, fis_satiri_id ,USE_DB);

            for ( int i = 0 ; i < BORDRO_SATIRI_ID_LIST.size() ;i++ ) {

                CEK_BORDRO_STRUCT * CEK_BORDROSU = new CEK_BORDRO_STRUCT;

                CEK_BORDROSU->modul_id   = P_FIS->modul_id;
                CEK_BORDROSU->program_id = P_FIS->program_id;
                CEK_BORDROSU->base_fis_id= base_fis_id;
                CEK_BORDROSU->muh_fis_id = muh_fis_id;

                int bordro_satiri_id = QVariant(BORDRO_SATIRI_ID_LIST.at(i)).toInt();


                CEK_BORDRO_SATIRINI_SIL ( CEK_BORDROSU ,bordro_satiri_id );
            }

            CARI_FIS_SATIRI_SIL ( P_FIS, fis_satiri_id ,USE_DB);
        }
    }
    if (P_FIS->modul_id EQ CARI_MODULU AND  muh_fis_id > 0 ) {

        MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

        MUH_FIS->modul_id   = P_FIS->modul_id;
        MUH_FIS->program_id = P_FIS->program_id;
        MUH_FIS->base_fis_id= base_fis_id;

        MUH_FIS_SIL ( MUH_FIS, muh_fis_id );
        delete MUH_FIS;

    }
    //YAZDIRILMIS ISE FIS SILINEMEZ
    if ( P_FIS->yazdirildi_mi EQ ENUM_FIS_YAZILMADI ) {
        query.PREPARE_DELETE ( "car_fisler","fis_id = :fis_id" );
        query.SET_VALUE      ( ":fis_id", p_fis_id );
        query.DELETE         ();
    }
    else {
        query.PREPARE_UPDATE ( "car_fisler", "fis_id", "iptal_mi", "fis_id = :fis_id");
        query.SET_VALUE      ( ":iptal_mi", 1 );
        query.SET_VALUE      ( ":fis_id", p_fis_id );
        query.UPDATE();
    }
}

/***************************************************************************************
                   CARI_FISI_GUNCELLE
***************************************************************************************/

void CARI_FISI_GUNCELLE ( CARI_FIS_STRUCT * P_FIS, int p_fis_id  )
{
    SQL_QUERY f_query  ( DB);
    SQL_QUERY s_query  ( DB );

    f_query.PREPARE_SELECT ( "car_fisler",
                             "fis_tarihi, program_id, modul_id, cari_hesap_id, fis_turu, "
                             "makbuz_para_birimi_id, doviz_kuru, parite, muh_fis_id, "
                             "base_fis_id,muh_kasa_hesap_id ",
                             "fis_id = :fis_id " );

    f_query.SET_VALUE      ( ":fis_id", p_fis_id );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    f_query.NEXT();

    int     program_id  =   f_query.VALUE ( "program_id" ).toInt();
    int     modul_id    =   f_query.VALUE ( "modul_id"   ).toInt();
    int     base_fis_id =   f_query.VALUE ( "base_fis_id").toInt();


    if ( program_id NE P_FIS->program_id OR modul_id NE P_FIS->modul_id OR
         base_fis_id NE P_FIS->base_fis_id ) {
        return;
    }

    QDate   old_fis_tarihi              =   QDate::fromString ( f_query.VALUE("fis_tarihi").toString(), "yyyy.MM.dd" );
    int     old_gun_no                  =   MALI_YIL_ARRAY_INDIS ( old_fis_tarihi );


    int     old_cari_hesap_id           =   f_query.VALUE ( "cari_hesap_id" ).toInt();


    QDate   new_fis_tarihi              =   QDate::fromString ( P_FIS->fis_tarihi, "yyyy.MM.dd" );
    int     new_gun_no                  =   MALI_YIL_ARRAY_INDIS ( new_fis_tarihi );
    int     new_cari_hesap_id           =   P_FIS->cari_hesap_id;
    int     fis_turu                    =   f_query.VALUE ( "fis_turu" ).toInt();
    int     new_cari_para_birimi_id     =   P_FIS->cari_para_birimi_id;

    int     old_makbuz_para_birimi_id   =   f_query.VALUE ( "makbuz_para_birimi_id" ).toInt();
    double  old_doviz_kuru              =   f_query.VALUE ( "doviz_kuru"            ).toDouble();
    double  old_parite                  =   f_query.VALUE ( "parite"                ).toDouble();

    double  new_doviz_kuru              =   P_FIS->doviz_kuru;
    double  new_parite                  =   P_FIS->parite;
    double  new_makbuz_para_birimi_id   =   P_FIS->makbuz_para_birimi_id;

    int     muh_fis_id                  =   f_query.VALUE("muh_fis_id").toInt();
    int     muh_kasa_hesap_id           =   f_query.VALUE("muh_kasa_hesap_id").toInt();

    bool    bakiyeler_guncellenecek     =   false;
    bool    satirlari_tekrar_hesapla    =   false;

    if ( old_fis_tarihi NE new_fis_tarihi ) {
        bakiyeler_guncellenecek = true;
    }

    switch ( fis_turu ) {

        case ENUM_CARI_TAHSILAT_FISI :
        case ENUM_CARI_ODEME_FISI    :

            if ( old_cari_hesap_id NE new_cari_hesap_id ) {
                bakiyeler_guncellenecek = true;
            }
            if ( old_doviz_kuru             NE new_doviz_kuru OR
                 old_makbuz_para_birimi_id  NE new_makbuz_para_birimi_id OR
                 old_parite                 NE new_parite ) {
                bakiyeler_guncellenecek  = true;
                satirlari_tekrar_hesapla = true;
            }
            break;
        default :
            break;
    }

    if ( bakiyeler_guncellenecek EQ true ) {

        switch ( fis_turu ) {

            case ENUM_CARI_HAREKET_FISI :
            case ENUM_CARI_ACILIS_FISI  :
            case ENUM_CARI_VIRMAN_FORMU :
            default                     :

                f_query.PREPARE_SELECT ( "car_fis_satirlari", "cari_hesap_id, alacak_tutari, borc_tutari, "
                                         "cari_para_birimi_tutari ",
                                         "fis_id = :fis_id" );
                f_query.SET_VALUE ( ":fis_id", p_fis_id );

                if ( f_query.SELECT( "order_number" ) > 0 ) {
                    while ( f_query.NEXT() EQ true ) {

                        double      cari_para_birimi_borc_tutari     =   0.0;
                        double      cari_para_birimi_alacak_tutari   =   0.0;
                        int         satir_cari_hesap_id              =   f_query.VALUE ( "cari_hesap_id" ).toInt();

                        if ( f_query.VALUE ( "borc_tutari" ).toDouble() NE 0 ) {
                            cari_para_birimi_borc_tutari = f_query.VALUE ( "cari_para_birimi_tutari" ).toDouble();
                        }
                        else {
                            cari_para_birimi_alacak_tutari = f_query.VALUE ( "cari_para_birimi_tutari" ).toDouble();
                        }

                        if ( fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
                            old_gun_no = 0;
                            new_gun_no = 0;
                        }

                        CARI_HESABIN_BAKIYESINI_AZALT  ( old_gun_no, satir_cari_hesap_id, cari_para_birimi_borc_tutari, cari_para_birimi_alacak_tutari);
                        CARI_HESABIN_BAKIYESINI_ARTTIR ( new_gun_no, satir_cari_hesap_id, cari_para_birimi_borc_tutari, cari_para_birimi_alacak_tutari);
                    }
                }
                break;

            case ENUM_CARI_TAHSILAT_FISI :
            case ENUM_CARI_ODEME_FISI    :

                if ( satirlari_tekrar_hesapla EQ true ) {
                    f_query.PREPARE_SELECT ( "car_fis_satirlari","alacak_tutari, borc_tutari, cari_para_birimi_tutari, "
                                             "fis_satiri_id ",
                                             "fis_id = :fis_id " );

                    f_query.SET_VALUE      ( ":fis_id", p_fis_id );
                    if ( f_query.SELECT() > 0 ) {
                        while ( f_query.NEXT() EQ true ) {

                            int     fis_satiri_id                       = f_query.VALUE ( "fis_satiri_id" ).toInt();
                            double  satir_alacak_tutari                 = f_query.VALUE ( "alacak_tutari" ).toDouble();
                            double  satir_borc_tutari                   = f_query.VALUE ( "borc_tutari"   ).toDouble();
                            double  old_cari_para_birimi_borc_tutari    = 0.0;
                            double  old_cari_para_birimi_alacak_tutari  = 0.0;
                            double  new_cari_para_birimi_borc_tutari    = 0.0;
                            double  new_cari_para_birimi_alacak_tutari  = 0.0;
                            double  new_cari_para_birimi_tutari         = 0.0;
                            double  new_temel_para_birimi_tutari        = 0.0;

                            if ( satir_alacak_tutari > 0 ) {
                                old_cari_para_birimi_alacak_tutari = f_query.VALUE ( "cari_para_birimi_tutari" ).toDouble();
                                new_cari_para_birimi_alacak_tutari = CARI_GET_PARA_BIRIMI_TUTARI ( new_cari_para_birimi_id, new_makbuz_para_birimi_id, new_doviz_kuru, new_parite, satir_alacak_tutari );
                                new_cari_para_birimi_tutari        = new_cari_para_birimi_alacak_tutari;
                                new_temel_para_birimi_tutari       = ROUND ( new_doviz_kuru * satir_alacak_tutari);
                            }
                            else {
                                old_cari_para_birimi_borc_tutari = f_query.VALUE ( "cari_para_birimi_tutari" ).toDouble();
                                new_cari_para_birimi_borc_tutari = CARI_GET_PARA_BIRIMI_TUTARI ( new_cari_para_birimi_id, new_makbuz_para_birimi_id, new_doviz_kuru, new_parite, satir_borc_tutari );
                                new_cari_para_birimi_tutari        = new_cari_para_birimi_borc_tutari;
                                new_temel_para_birimi_tutari       = ROUND ( new_doviz_kuru * satir_borc_tutari);
                            }

                            CARI_HESABIN_BAKIYESINI_AZALT  ( old_gun_no, old_cari_hesap_id, old_cari_para_birimi_borc_tutari, old_cari_para_birimi_alacak_tutari);
                            CARI_HESABIN_BAKIYESINI_ARTTIR ( new_gun_no, new_cari_hesap_id, new_cari_para_birimi_borc_tutari, new_cari_para_birimi_alacak_tutari);

                            s_query.PREPARE_UPDATE ( "car_fis_satirlari ","fis_satiri_id",
                                                     "cari_para_birimi_tutari , "
                                                     "temel_para_birimi_tutari      ",
                                                     "fis_satiri_id    = :fis_satiri_id" );

                            s_query.SET_VALUE      ( ":cari_para_birimi_tutari",    new_cari_para_birimi_tutari  );
                            s_query.SET_VALUE      ( ":temel_para_birimi_tutari",   new_temel_para_birimi_tutari );
                            s_query.SET_VALUE      ( ":fis_satiri_id",              fis_satiri_id                );
                            s_query.UPDATE         ();
                        }
                    }
                }
                else {
                    f_query.PREPARE_SELECT ( "car_fis_satirlari", "alacak_tutari, cari_para_birimi_tutari ",
                                             "fis_id = :fis_id " );
                    f_query.SET_VALUE      ( ":fis_id", p_fis_id );
                    if ( f_query.SELECT() > 0 ) {
                        while ( f_query.NEXT() EQ true ) {
                            double  satir_alacak_tutari                 = f_query.VALUE ( "alacak_tutari" ).toDouble();
                            double  cari_para_birimi_borc_tutari        = 0.0;
                            double  cari_para_birimi_alacak_tutari      = 0.0;

                            if ( satir_alacak_tutari > 0 ) {
                                cari_para_birimi_alacak_tutari = f_query.VALUE ( "cari_para_birimi_tutari" ).toDouble();
                            }
                            else {
                                cari_para_birimi_borc_tutari   = f_query.VALUE ( "cari_para_birimi_tutari" ).toDouble();
                            }

                            CARI_HESABIN_BAKIYESINI_AZALT  ( old_gun_no, old_cari_hesap_id, cari_para_birimi_borc_tutari, cari_para_birimi_alacak_tutari );
                            CARI_HESABIN_BAKIYESINI_ARTTIR ( new_gun_no, new_cari_hesap_id, cari_para_birimi_borc_tutari, cari_para_birimi_alacak_tutari );

                        }
                    }
                }
        }
    }


    f_query.PREPARE_UPDATE ( "car_fisler ", "fis_id",
                             "fis_no            , "
                             "fis_tarihi            , "
                             "cari_hesap_id         , "
                             "aciklama              , "
                             "belge_seri            , "
                             "belge_numarasi              , "
                             "doviz_tutar_bilgileri , "
                             "makbuz_para_birimi_id , "
                             "doviz_kuru            , "
                             "parite                , "
                             "muh_kasa_hesap_id       ",
                             "fis_id   = :fis_id" );

    f_query.SET_VALUE ( ":fis_no",                  P_FIS->fis_no                               );
    f_query.SET_VALUE ( ":fis_tarihi",              P_FIS->fis_tarihi                           );
    f_query.SET_VALUE ( ":cari_hesap_id",           P_FIS->cari_hesap_id                        );
    f_query.SET_VALUE ( ":aciklama",                P_FIS->aciklama                             );
    f_query.SET_VALUE ( ":belge_seri",              P_FIS->belge_seri                           );
    f_query.SET_VALUE ( ":belge_numarasi",          P_FIS->belge_numarasi                             );
    f_query.SET_VALUE ( ":doviz_tutar_bilgileri",   P_FIS->borc_doviz_tutar_bilgileri + "#" +
                                                    P_FIS->alacak_doviz_tutar_bilgileri         );
    f_query.SET_VALUE ( ":makbuz_para_birimi_id",   P_FIS->makbuz_para_birimi_id                );
    f_query.SET_VALUE ( ":doviz_kuru",              ROUND ( P_FIS->doviz_kuru , -4)             );
    f_query.SET_VALUE ( ":parite",                  ROUND ( P_FIS->parite , -4)                 );
    f_query.SET_VALUE ( ":muh_kasa_hesap_id"      , P_FIS->muh_kasa_hesap_id );
    f_query.SET_VALUE ( ":fis_id",                  p_fis_id                                    );

    f_query.UPDATE    ();

    if ( P_FIS->modul_id EQ CARI_MODULU ) {
        if ( P_FIS->base_fis_id EQ 0 ) {
            P_FIS->base_fis_id = p_fis_id;
        }
    }



    QStringList BORDRO_ID_LIST = CEK_GET_ENT_FIS_ID_LIST ( P_FIS->program_id, P_FIS->modul_id,P_FIS->base_fis_id );

    CEK_BORDRO_STRUCT * CEK_BORDROSU  = new CEK_BORDRO_STRUCT;

    for ( int i = 0; i < BORDRO_ID_LIST.size(); i++ ) {

        CEK_BORDROSU->CLEAR_BORDRO_STRUCT();

        CEK_BORDRO_BILGILERI_OKU ( CEK_BORDROSU , QVariant ( BORDRO_ID_LIST.at ( 0 ) ).toInt() );

        QString aciklama = QObject::tr("Fiş CARİ modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
        aciklama        += P_FIS->aciklama;

        CEK_BORDROSU->modul_id              = P_FIS->modul_id;
        CEK_BORDROSU->program_id            = P_FIS->program_id;
        CEK_BORDROSU->bordro_tarihi         = P_FIS->fis_tarihi;
        CEK_BORDROSU->cari_hesap_id         = P_FIS->cari_hesap_id;
        CEK_BORDROSU->aciklama              = aciklama;
        CEK_BORDROSU->bordro_para_birimi_id = P_FIS->makbuz_para_birimi_id;
        CEK_BORDROSU->doviz_kuru            = P_FIS->doviz_kuru;
        CEK_BORDROSU->parite                = P_FIS->parite;
        CEK_BORDROSU->cari_para_birimi_id   = P_FIS->cari_para_birimi_id;
        CEK_BORDROSU->muh_fis_id            = muh_fis_id;


        CEK_BORDRO_GUNCELLE ( CEK_BORDROSU , QVariant ( BORDRO_ID_LIST.at ( 0 ) ).toInt() );
    }

    delete CEK_BORDROSU;

    // CARI TAHSILAT VE ODEME FISLERINDE BIR CARI FISIN SATIRLARINDA ISLEM TURUNE GORE BIRDEN FAZLA
    // BORDRO OLUSABILIR. BU DURUMDA O BORDRO VEYA BORDROLAR AYNI BASE_FIS_ID ILE MUHASEBE FISI OLUSTURACAKLAR
    // NAKIT ISLEM SATIRI ICIN DE AYRICA MUHASEBE FISI OLUSTURULUR VE ONUN DA BASE_FIS_ID SI AYNI CARI_FIS_ID OLUR.
    // BU DURUMDA NAKIT SATIRA AIT CARIDEN DIREKT ENTEGRE OLAN MUHASEBE FISINI YAKALAMAK ICIN
    // NAKIT ISLEM FIS SATIRI CARI FISTEN BULUNUR. BU SATIRA ENTEGRE OLAN MUH_FIS_ID YI BULMAK ICIN SADECE
    // BU OZEL DURUM ICIN YAZILMIS OLAN GET_MUH_ENT_FIS_ID_FOR_BASE_RECORD_ID() FONKSIYONU CAGRILARAK
    // ISTENILEN MUH_FIS_ID ELDE EDILMIS OLUR.


    if ( muh_fis_id > 0 ) {

        MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

        MUH_FIS_BILGILERINI_OKU ( MUHASEBE_FISI , muh_fis_id );

        if ( P_FIS->modul_id EQ CARI_MODULU ) {
            QString aciklama        = QObject::tr("Fiş CARİ modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
            aciklama               += P_FIS->aciklama;
            MUHASEBE_FISI->aciklama = aciklama;
        }
        else {
            MUHASEBE_FISI->aciklama = P_FIS->aciklama;
        }
        MUHASEBE_FISI->fis_tarihi       = P_FIS->fis_tarihi;

        MUH_FISI_GUNCELLE ( MUHASEBE_FISI , muh_fis_id );

        if ( muh_kasa_hesap_id NE P_FIS->muh_kasa_hesap_id OR
             old_cari_hesap_id NE new_cari_hesap_id) {

            int nakit_islem_fis_satiri_id = 0;

            f_query.PREPARE_SELECT ( "car_fis_satirlari","fis_satiri_id,borc_tutari,temel_para_birimi_tutari,"
                                     "aciklama",
                                     "islem_turu = :islem_turu AND fis_id = :fis_id" );

            f_query.SET_VALUE      ( ":islem_turu", ENUM_NAKIT );
            f_query.SET_VALUE      ( ":fis_id"    , p_fis_id       );

            if ( f_query.SELECT() > 0 ) {

                int cari_muh_hesap_id = CARI_FIND_HESAP_MUH_HESAP_ID(P_FIS->cari_hesap_id);

                while ( f_query.NEXT()) {
                    nakit_islem_fis_satiri_id       = f_query.VALUE ( 0 ).toInt();
                    double borc_tutari              = f_query.VALUE(1).toDouble();
                    double temel_para_birimi_tutari = f_query.VALUE(2).toDouble();
                    QString satir_aciklama          = f_query.VALUE(3).toString();

                    MUH_ENTEGRASYON_FIS_SATIRLARINI_SIL(MUHASEBE_FISI,muh_fis_id,nakit_islem_fis_satiri_id);

                    double      muh_fis_satiri_alacak_tutari    =   0.0;
                    double      muh_fis_satiri_borc_tutari      =   0.0;

                    if ( borc_tutari > 0 ) {
                        muh_fis_satiri_borc_tutari      = temel_para_birimi_tutari;
                    }
                    else {
                        muh_fis_satiri_alacak_tutari    = temel_para_birimi_tutari;
                    }


                    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

                    MUH_CLEAR_FIS_SATIRI_STRUCT     ( MUH_FIS_SATIRI );

                    MUH_FIS_SATIRI->aciklama                = satir_aciklama;
                    MUH_FIS_SATIRI->borc_tutari             = muh_fis_satiri_borc_tutari;
                    MUH_FIS_SATIRI->alacak_tutari           = muh_fis_satiri_alacak_tutari;
                    MUH_FIS_SATIRI->fis_id                  = muh_fis_id;
                    MUH_FIS_SATIRI->hesap_id                = cari_muh_hesap_id;
                    MUH_FIS_SATIRI->program_id              = P_FIS->program_id;
                    MUH_FIS_SATIRI->modul_id                = P_FIS->modul_id;
                    MUH_FIS_SATIRI->base_record_id          = nakit_islem_fis_satiri_id;

                    if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                        MUH_FIS_SATIRI->hesap_id      = P_FIS->muh_kasa_hesap_id;
                        MUH_FIS_SATIRI->borc_tutari   = temel_para_birimi_tutari;
                        MUH_FIS_SATIRI->alacak_tutari = 0.0;
                    }

                    MUH_FIS_SATIRI_EKLE ( MUHASEBE_FISI, MUH_FIS_SATIRI   );

                    if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                        MUH_FIS_SATIRI->hesap_id      = cari_muh_hesap_id;
                        MUH_FIS_SATIRI->borc_tutari   = 0.0;
                        MUH_FIS_SATIRI->alacak_tutari = temel_para_birimi_tutari;

                    }
                    else  if ( fis_turu EQ ENUM_CARI_ODEME_FISI ) { //Odeme Fisi
                        MUH_FIS_SATIRI->hesap_id      = P_FIS->muh_kasa_hesap_id;
                        MUH_FIS_SATIRI->borc_tutari   = 0.0;
                        MUH_FIS_SATIRI->alacak_tutari = temel_para_birimi_tutari;
                    }
                    MUH_FIS_SATIRI_EKLE(MUHASEBE_FISI,MUH_FIS_SATIRI);

                }
            }

        }

        delete MUHASEBE_FISI;
    }



    if ( P_FIS->modul_id NE SMM_MODULU ) {
        int bnk_fis_id = BNK_GET_ENT_FIS_ID(P_FIS->program_id, P_FIS->modul_id, P_FIS->base_fis_id );


        if( bnk_fis_id NE 0 ) {

            BNK_HAREKET_FIS_STRUCT * BNK_FIS = new BNK_HAREKET_FIS_STRUCT;

            BNK_HAREKET_FIS_BILGILERINI_OKU(BNK_FIS,bnk_fis_id);

            QString aciklama = QObject::tr("Fiş CARİ modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
            aciklama.append(P_FIS->aciklama);

            if ( aciklama.size() > 512 ) {
                aciklama.resize(512);
            }

            BNK_FIS->aciklama         = aciklama;
            BNK_FIS->fis_tarihi       = P_FIS->fis_tarihi;
            BNK_FIS->muh_fis_id       = muh_fis_id;

            BNK_HAREKET_FISI_GUNCELLE( BNK_FIS, bnk_fis_id );

        }
    }

}

/***************************************************************************************
                   CARI_FIS_SATIRINI_GUNCELLE
***************************************************************************************/

int CARI_FIS_SATIRINI_GUNCELLE ( CARI_FIS_STRUCT * P_FIS, CARI_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id )
{

    // guncellemede satir silip eklendigi icin yeni satir is return edilmesi gerekir
    int new_record_id = 0;

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "car_fis_satirlari, car_fisler",
                           "toplam_borc, toplam_alacak, borc_tutari, alacak_tutari, "
                           "cari_para_birimi_tutari, car_fis_satirlari.cari_hesap_id,"
                           "car_fis_satirlari.doviz_id, "
                           "car_fis_satirlari.doviz_kuru, car_fis_satirlari.parite, "
                           "car_fisler.doviz_kuru,  fis_tarihi,  car_fisler.cari_hesap_id, "
                           "islem_turu, cek_senet_id, car_fis_satirlari.modul_id, "
                           "car_fis_satirlari.program_id, fis_turu, "
                           "car_fisler.fis_id, base_record_id, "
                           "makbuz_para_birimi_id, car_fisler.parite ,muh_fis_id , base_fis_id ",

                           "car_fisler.fis_id   = car_fis_satirlari.fis_id "
                           "AND   fis_satiri_id       = :fis_satiri_id" );

    query.SET_VALUE       ( ":fis_satiri_id", p_fis_satiri_id );
    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    double  toplam_borc                             = query.VALUE ( "toplam_borc"               ).toDouble();
    double  toplam_alacak                           = query.VALUE ( "toplam_alacak"             ).toDouble();

    double  old_satir_borc_tutari                   = query.VALUE ( "borc_tutari"               ).toDouble();
    double  old_satir_alacak_tutari                 = query.VALUE ( "alacak_tutari"             ).toDouble();

    double  old_cari_para_birimi_tutari             = query.VALUE ( "cari_para_birimi_tutari"   ).toDouble();

    int     old_satir_cari_hesap_id                 = query.VALUE ( 5 ).toInt();

    int     old_satir_doviz_id                      = query.VALUE ( 6 ).toInt();
    QString old_doviz_kodu                          = DVZ_GET_DOVIZ_KODU ( old_satir_doviz_id );
    double  old_satir_doviz_kuru                    = query.VALUE ( 7 ).toDouble();
    double  old_satir_parite                        = query.VALUE ( 8 ).toDouble();

    double  fis_doviz_kuru                          = query.VALUE ( 9 ).toDouble();
    double  fis_parite                              = query.VALUE ( 20 ).toDouble();
    int     fis_cari_hesap_id                       = query.VALUE ( 11 ).toInt();

    QDate   fis_tarihi                              = QDate::fromString ( query.VALUE("fis_tarihi").toString(), "yyyy.MM.dd" );
    int     muh_fis_id                              = query.VALUE("muh_fis_id").toInt();

    int     makbuz_para_birimi_id                   = query.VALUE ( "makbuz_para_birimi_id" ).toInt();
    int     modul_id                                = query.VALUE ( "modul_id"          ).toInt();
    int     program_id                              = query.VALUE ( "program_id"        ).toInt();
    int     old_islem_turu                          = query.VALUE ( "islem_turu"        ).toInt();
    int     old_cek_senet_id                        = query.VALUE ( "cek_senet_id"      ).toInt();
    int     new_islem_turu                          = P_FIS_SATIRI->islem_turu;
    int     new_cek_senet_id                        = P_FIS_SATIRI->cek_senet_id;
    int     fis_turu                                = query.VALUE ( "fis_turu"          ).toInt();
    int     fis_id                                  = query.VALUE("fis_id").toInt();
    int     base_fis_id                             = query.VALUE("base_fis_id").toInt();

    int     gun_no                                  = MALI_YIL_ARRAY_INDIS ( fis_tarihi );

    int     new_satir_doviz_id                      = P_FIS_SATIRI->doviz_id;
    double  new_satir_doviz_kuru                    = P_FIS_SATIRI->doviz_kuru;
    double  new_satir_parite                        = P_FIS_SATIRI->parite;

    double  new_satir_borc_tutari                   = P_FIS_SATIRI->borc_tutari;
    double  new_satir_alacak_tutari                 = P_FIS_SATIRI->alacak_tutari;

    double  satir_tutari                            = 0.0;

    if ( new_satir_borc_tutari > 0 ) {
        satir_tutari = new_satir_borc_tutari;
    }
    else {
        satir_tutari = new_satir_alacak_tutari;
    }

    if ( P_FIS->modul_id EQ CARI_MODULU ) {
        if ( base_fis_id EQ 0 ) {
            base_fis_id = P_FIS_SATIRI->fis_id;
        }
    }



    if ( program_id NE P_FIS->program_id OR modul_id NE P_FIS->modul_id OR base_fis_id NE P_FIS->base_fis_id) {
        return 0;
    }

    if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
        if ( P_FIS_SATIRI->cek_senet_id EQ 0 ) {
            P_FIS_SATIRI->cek_senet_id = old_cek_senet_id;
        }
    }

    if ( fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
        gun_no = 0;
    }

    bool    cari_bakiye_guncellenecek   = false;

    if ( ( old_satir_borc_tutari NE new_satir_borc_tutari ) OR
         ( old_satir_alacak_tutari NE new_satir_alacak_tutari ) ) {
        cari_bakiye_guncellenecek = true;
    }

    int     old_cari_hesap_id    = 0;
    int     new_cari_hesap_id    = 0;

    switch ( fis_turu ) {

        case ENUM_CARI_TAHSILAT_FISI :
        case ENUM_CARI_ODEME_FISI    :
            old_cari_hesap_id = fis_cari_hesap_id;
            new_cari_hesap_id = fis_cari_hesap_id;
            break;

        case ENUM_CARI_HAREKET_FISI :
        case ENUM_CARI_ACILIS_FISI  :
        case ENUM_CARI_VIRMAN_FORMU :
        default                     :

            old_cari_hesap_id = old_satir_cari_hesap_id;
            new_cari_hesap_id = P_FIS_SATIRI->cari_hesap_id;

            if ( old_satir_doviz_id NE new_satir_doviz_id OR old_satir_doviz_kuru NE new_satir_doviz_kuru OR
                 old_satir_parite NE new_satir_parite OR
                 new_cari_hesap_id NE old_cari_hesap_id ) {
                cari_bakiye_guncellenecek = true;
            }
            break;
    }

    int     cari_para_birimi_id                     = CARI_GET_PARA_BIRIM_ID ( new_cari_hesap_id );
    double  new_cari_para_birimi_tutari             = 0.0;
    double  new_temel_para_birimi_tutari            = 0.0;


    switch ( fis_turu ) {

        case ENUM_CARI_TAHSILAT_FISI :
        case ENUM_CARI_ODEME_FISI    :
            new_cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI ( cari_para_birimi_id, makbuz_para_birimi_id, fis_doviz_kuru, fis_parite, satir_tutari );
            if ( makbuz_para_birimi_id NE DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
                new_temel_para_birimi_tutari = ROUND ( fis_doviz_kuru * satir_tutari);
            }
            else {
                new_temel_para_birimi_tutari = satir_tutari;
            }
            break;

        case ENUM_CARI_HAREKET_FISI :
        case ENUM_CARI_ACILIS_FISI  :
        case ENUM_CARI_VIRMAN_FORMU :
        default                     :
            new_cari_para_birimi_tutari     = CARI_GET_PARA_BIRIMI_TUTARI ( cari_para_birimi_id, new_satir_doviz_id, new_satir_doviz_kuru, new_satir_parite, satir_tutari );
            if ( new_satir_doviz_id NE DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
                new_temel_para_birimi_tutari    = ROUND ( new_satir_doviz_kuru * satir_tutari);
            }
            else {
                new_temel_para_birimi_tutari = satir_tutari;
            }
            break;
    }

    if ( cari_bakiye_guncellenecek EQ true ) {

        double  old_cari_para_birimi_borc_tutari        = 0.0;
        double  old_cari_para_birimi_alacak_tutari      = 0.0;

        double  new_cari_para_birimi_borc_tutari        = 0.0;
        double  new_cari_para_birimi_alacak_tutari      = 0.0;

        if ( old_satir_borc_tutari > 0 ) {
            old_cari_para_birimi_borc_tutari = old_cari_para_birimi_tutari;
        }
        else {
            old_cari_para_birimi_alacak_tutari = old_cari_para_birimi_tutari;
        }

        if ( new_satir_borc_tutari > 0 ) {
            new_cari_para_birimi_borc_tutari = new_cari_para_birimi_tutari;
        }
        else {
            new_cari_para_birimi_alacak_tutari = new_cari_para_birimi_tutari;
        }



        CARI_HESABIN_BAKIYESINI_AZALT  ( gun_no, old_cari_hesap_id, old_cari_para_birimi_borc_tutari, old_cari_para_birimi_alacak_tutari);
        CARI_HESABIN_BAKIYESINI_ARTTIR ( gun_no, new_cari_hesap_id, new_cari_para_birimi_borc_tutari, new_cari_para_birimi_alacak_tutari);


        if ( old_satir_borc_tutari EQ 0.0 ) {
            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_AZALT  ( old_doviz_kodu,
                                                               old_satir_alacak_tutari,
                                                               0,
                                                               P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                               P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
        }
        else if ( old_satir_alacak_tutari EQ 0.0 ) {
            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_AZALT  ( old_doviz_kodu,
                                                               old_satir_borc_tutari,
                                                               1,
                                                               P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                               P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
        }

        if ( P_FIS_SATIRI->borc_tutari EQ 0.0 ) {
            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_ARTIR  ( P_FIS_SATIRI->doviz_kodu,
                                                               P_FIS_SATIRI->alacak_tutari,
                                                               0,
                                                               P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                               P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
        }
        else if ( P_FIS_SATIRI->alacak_tutari EQ 0.0 ) {
            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_ARTIR  ( P_FIS_SATIRI->doviz_kodu,
                                                               P_FIS_SATIRI->borc_tutari,
                                                               1,
                                                               P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                               P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
        }
    }

    if ( old_islem_turu NE new_islem_turu ) {

        switch ( old_islem_turu ) {

            case ENUM_MUSTERI_CEKI    :
            case ENUM_MUSTERI_SENEDI  :
            case ENUM_KENDI_CEKIMIZ   :
            case ENUM_KENDI_SENEDIMIZ :
                if ( P_FIS->modul_id EQ CARI_MODULU ) {
                    if ( old_cek_senet_id > 0 ) {

                        CEK_BORDRO_STRUCT * CEK_BORDROSU = new CEK_BORDRO_STRUCT;

                        CEK_BORDROSU->modul_id         = P_FIS->modul_id;
                        CEK_BORDROSU->program_id       = P_FIS->program_id;
                        CEK_BORDROSU->muh_fis_id       = P_FIS->muh_fis_id;

                        int base_record_id = P_FIS_SATIRI->base_record_id;

                        if ( P_FIS->modul_id EQ CARI_MODULU ) {
                            base_record_id = p_fis_satiri_id;
                        }

                        QStringList BORDRO_ROW_ID_LIST = CEK_GET_ENT_ROW_ID_LIST(P_FIS->program_id , P_FIS->modul_id ,
                                                                                 base_record_id);

                        for ( int i = 0 ; i < BORDRO_ROW_ID_LIST.size() ; i++ ) {

                            int bordro_satiri_id = QVariant(BORDRO_ROW_ID_LIST.at(i)).toInt();

                            CEK_BORDRO_SATIRINI_SIL ( CEK_BORDROSU , bordro_satiri_id);
                        }

                        if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                            CEK_SENET_SIL ( old_cek_senet_id );
                            P_FIS_SATIRI->cek_senet_id = 0;
                        }
                    }
                }
                break;


            default :
                break;
        }

        switch ( new_islem_turu ) {

            case ENUM_KENDI_CEKIMIZ     :
            case ENUM_KENDI_SENEDIMIZ   :
            case ENUM_MUSTERI_CEKI      :
            case ENUM_MUSTERI_SENEDI    :

                if ( new_cek_senet_id EQ 0 ) {

                    if ( P_FIS->modul_id EQ CARI_MODULU ){

                        if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                            int cek_senet_turu = ENUM_CEK;
                            if ( new_islem_turu EQ ENUM_MUSTERI_SENEDI ) {
                                cek_senet_turu = ENUM_SENET;
                            }

                            CEK_SENET_STRUCT * CEK_SENET = new CEK_SENET_STRUCT;

                            CEK_SENET->portfoy_numarasi         =   0;
                            CEK_SENET->banka_ismi               =   P_FIS_SATIRI->musteri_banka_ismi;
                            CEK_SENET->sube_kodu                =   P_FIS_SATIRI->musteri_sube_kodu;
                            CEK_SENET->sube_ismi                =   P_FIS_SATIRI->musteri_sube_ismi;
                            CEK_SENET->hesap_no_id              =   P_FIS_SATIRI->hesap_no_id;
                            CEK_SENET->cek_senet_numarasi       =   P_FIS_SATIRI->cek_senet_no;
                            CEK_SENET->vade_tarihi              =   P_FIS_SATIRI->cek_senet_vade_tarihi;
                            CEK_SENET->keside_tarihi            =   P_FIS_SATIRI->cek_senet_keside_tarihi;
                            CEK_SENET->borclu_kesideci          =   P_FIS_SATIRI->kesideci;
                            CEK_SENET->musteri_ceki_mi          =   1;
                            CEK_SENET->cek_senet_tutari         =   P_FIS_SATIRI->alacak_tutari;
                            CEK_SENET->musteri_hesap_numarasi   =   P_FIS_SATIRI->musteri_hesap_no;
                            CEK_SENET->cek_senet_turu           =   cek_senet_turu;
                            CEK_SENET->doviz_id                 =   P_FIS_SATIRI->doviz_id;

                            P_FIS_SATIRI->cek_senet_id          = CEK_SENET_EKLE ( CEK_SENET );
                            P_FIS_SATIRI->portfoy_no            = CEK_SENET->portfoy_numarasi;

                            delete CEK_SENET;
                        }
                    }
                }

                if ( P_FIS_SATIRI->cek_senet_id > 0 ) {

                    CEK_BORDRO_STRUCT * CEK_BORDROSU = new CEK_BORDRO_STRUCT;

                    int base_fis_id    = P_FIS->base_fis_id;
                    int base_record_id = P_FIS_SATIRI->base_record_id;

                    if ( P_FIS->modul_id EQ CARI_MODULU ) {
                        base_fis_id    = fis_id;
                        base_record_id = p_fis_satiri_id;
                    }

                    int bordro_id = CEK_GET_BORDRO_ID( program_id, modul_id, base_fis_id, base_record_id );

                    if ( bordro_id EQ 0 ) {

                        CEK_BORDROSU->CLEAR_BORDRO_STRUCT();

                        CEK_BORDROSU->program_id            = P_FIS->program_id;
                        CEK_BORDROSU->modul_id              = P_FIS->modul_id;
                        CEK_BORDROSU->bordro_tarihi         = P_FIS->fis_tarihi;
                        CEK_BORDROSU->cari_hesap_id         = P_FIS->cari_hesap_id;
                        CEK_BORDROSU->cari_para_birimi_id   = P_FIS->cari_para_birimi_id;
                        CEK_BORDROSU->bordro_para_birimi_id = P_FIS->makbuz_para_birimi_id;
                        CEK_BORDROSU->doviz_kuru            = P_FIS->doviz_kuru;
                        CEK_BORDROSU->parite                = P_FIS->parite;
                        CEK_BORDROSU->bordro_no             = 0;


                        if ( P_FIS_SATIRI->islem_turu EQ ENUM_MUSTERI_CEKI ) {
                            CEK_BORDROSU->bordro_turu       = ENUM_MUSTERI_CEK_BORDROSU;
                            CEK_BORDROSU->cek_bordrosu_mu   = 1;
                        }
                        else if ( P_FIS_SATIRI->islem_turu EQ ENUM_MUSTERI_SENEDI ){
                            CEK_BORDROSU->bordro_turu       = ENUM_MUSTERI_SENET_BORDROSU;
                            CEK_BORDROSU->cek_bordrosu_mu   = 0;
                        }
                        else if ( P_FIS_SATIRI->islem_turu EQ ENUM_KENDI_CEKIMIZ ) {
                            CEK_BORDROSU->bordro_turu       = ENUM_FIRMA_CEK_BORDROSU;
                            CEK_BORDROSU->cek_bordrosu_mu   = 1;
                        }
                        else if ( P_FIS_SATIRI->islem_turu EQ ENUM_KENDI_SENEDIMIZ ) {
                            CEK_BORDROSU->bordro_turu       = ENUM_FIRMA_SENET_BORDROSU;
                            CEK_BORDROSU->cek_bordrosu_mu   = 0;
                        }
                        if ( P_FIS->fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                            CEK_BORDROSU->bordro_islem_turu = ENUM_CEK_MUSTERIDEN_ALINDI;
                        }
                        else {
                            CEK_BORDROSU->bordro_islem_turu = ENUM_CEK_SATICIYA_VERILDI;
                        }
                        CEK_BORDROSU->base_fis_id = base_fis_id;

                        QString aciklama = QObject::tr("Fiş CARİ modülü tarafından oluşturulmuştur. Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
                        aciklama += P_FIS->aciklama;

                        CEK_BORDROSU->aciklama         = aciklama;
                        CEK_BORDROSU->muh_fis_id       = muh_fis_id;

                        bordro_id = CEK_BORDROSU_EKLE(CEK_BORDROSU );
                    }
                    else {
                        CEK_BORDRO_BILGILERI_OKU ( CEK_BORDROSU , bordro_id );
                    }
                    CEK_BORDRO_SATIRI_STRUCT * BORDRO_SATIRI = new CEK_BORDRO_SATIRI_STRUCT;

                    CEK_CLEAR_BORDRO_SATIRI_STRUCT ( BORDRO_SATIRI );

                    BORDRO_SATIRI->bordro_id                = bordro_id;
                    BORDRO_SATIRI->cek_senet_id             = P_FIS_SATIRI->cek_senet_id;
                    BORDRO_SATIRI->aciklama                 = P_FIS_SATIRI->aciklama;
                    BORDRO_SATIRI->temel_para_birimi_tutari = new_temel_para_birimi_tutari;
                    BORDRO_SATIRI->cari_para_birimi_tutari  = new_cari_para_birimi_tutari;
                    BORDRO_SATIRI->modul_id                 = P_FIS_SATIRI->modul_id;
                    BORDRO_SATIRI->program_id               = P_FIS_SATIRI->program_id;
                    BORDRO_SATIRI->base_record_id           = base_record_id;

                    if ( P_FIS->fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                        BORDRO_SATIRI->cek_tutari = P_FIS_SATIRI->alacak_tutari;
                    }
                    else {
                        BORDRO_SATIRI->cek_tutari = P_FIS_SATIRI->borc_tutari;
                    }

                    CEK_BORDRO_SATIRI_EKLE(CEK_BORDROSU, BORDRO_SATIRI, true, 0 );
                }

            case ENUM_HAVALE :
            case ENUM_EFT:
            case ENUM_KREDI_KARTI:
                {

                    BNK_HAREKET_FIS_STRUCT  * BNK_FIS = new BNK_HAREKET_FIS_STRUCT;
                    BNK_HAREKET_FIS_SATIRI_STRUCT * BNK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

                    int bnk_hareket_fisi_id = BNK_GET_ENT_FIS_ID( P_FIS->program_id, P_FIS->modul_id, base_fis_id );

                    BNK_HAREKET_FIS_BILGILERINI_OKU( BNK_FIS, bnk_hareket_fisi_id );

                    BNK_FIS_SATIRI->fis_id                 = bnk_hareket_fisi_id;
                    BNK_FIS_SATIRI->program_id             = P_FIS_SATIRI->program_id;
                    BNK_FIS_SATIRI->modul_id               = P_FIS_SATIRI->modul_id;
                    BNK_FIS_SATIRI->base_record_id         = P_FIS_SATIRI->base_record_id;
                    BNK_FIS_SATIRI->karsi_hesap_turu       = ENUM_BNK_CARI_HESAP;
                    BNK_FIS_SATIRI->karsi_hesap_id         = new_cari_hesap_id;

                    BNK_FIS_SATIRI->hesaba_giren           = P_FIS_SATIRI->alacak_tutari;
                    BNK_FIS_SATIRI->hesaptan_cikan         = P_FIS_SATIRI->borc_tutari;

                    BNK_FIS_SATIRI->doviz_kuru             = P_FIS_SATIRI->doviz_kuru;
                    BNK_FIS_SATIRI->parite                 = P_FIS_SATIRI->parite;

                    switch ( P_FIS_SATIRI->islem_turu ) {

                    case ENUM_HAVALE :
                        {
                            BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
                            BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->hesap_no_id; // banaka hesabi
                            break;
                        }
                    case ENUM_EFT:
                        {
                            BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
                            BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->hesap_no_id; // banka hesabi
                            break;
                        }
                    case ENUM_KREDI_KARTI:
                        {
                            BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_POS_HESABI;
                            BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->pos_id;
                            break;
                        }
                    default:
                        break;
                    }

                    new_record_id = BNK_HAREKET_FIS_SATIRINI_GUNCELLE( BNK_FIS, BNK_FIS_SATIRI, p_fis_satiri_id );

                    delete BNK_FIS;
                    delete BNK_FIS_SATIRI;

                    break;
                }
            break;
            default :
                break;
        }
    }

    query.PREPARE_UPDATE ( "car_fis_satirlari ", "fis_satiri_id",
                           "aciklama                 , "
                           "borc_tutari              , "
                           "alacak_tutari            , "
                           "kredi_karti_numarasi     , "
                           "kredi_karti_sahibi       , "
                           "musteri_banka_ismi       , "
                           "musteri_sube_kodu        , "
                           "musteri_sube_ismi        , "
                           "musteri_hesap_no         , "
                           "cari_hesap_id            , "
                           "cek_senet_id             , "
                           "islem_turu               , "
                           "temel_para_birimi_tutari , "
                           "muh_hesap_id             , "
                           "hesap_no_id              , "
                           "pos_id                   , "
                           "kredi_karti_id           , "
                           "cari_para_birimi_tutari  , "
                           "doviz_id                 , "
                           "doviz_kuru               , "
                           "parite                     ",
                           "fis_satiri_id = :fis_satiri_id" );

    query.SET_VALUE      ( ":aciklama"                  , P_FIS_SATIRI->aciklama                );
    query.SET_VALUE      ( ":borc_tutari"               , ROUND(new_satir_borc_tutari,-2)                 );
    query.SET_VALUE      ( ":alacak_tutari"             , ROUND(new_satir_alacak_tutari,-2)               );
    query.SET_VALUE      ( ":kredi_karti_numarasi"      , P_FIS_SATIRI->kredi_karti_numarasi    );
    query.SET_VALUE      ( ":kredi_karti_sahibi"        , P_FIS_SATIRI->kredi_karti_sahibi      );
    query.SET_VALUE      ( ":musteri_banka_ismi"        , P_FIS_SATIRI->musteri_banka_ismi      );
    query.SET_VALUE      ( ":musteri_sube_kodu"         , P_FIS_SATIRI->musteri_sube_kodu       );
    query.SET_VALUE      ( ":musteri_sube_ismi"         , P_FIS_SATIRI->musteri_sube_ismi       );
    query.SET_VALUE      ( ":musteri_hesap_no"          , P_FIS_SATIRI->musteri_hesap_no        );
    query.SET_VALUE      ( ":cari_hesap_id"             , P_FIS_SATIRI->cari_hesap_id           );
    query.SET_VALUE      ( ":cek_senet_id"              , P_FIS_SATIRI->cek_senet_id            );
    query.SET_VALUE      ( ":islem_turu"                , P_FIS_SATIRI->islem_turu              );
    query.SET_VALUE      ( ":temel_para_birimi_tutari"  , new_temel_para_birimi_tutari          );
    query.SET_VALUE      ( ":muh_hesap_id"              , P_FIS_SATIRI->muh_hesap_id            );
    query.SET_VALUE      ( ":hesap_no_id"               , P_FIS_SATIRI->hesap_no_id             );
    query.SET_VALUE      ( ":pos_id"                    , P_FIS_SATIRI->pos_id                  );
    query.SET_VALUE      ( ":kredi_karti_id"            , P_FIS_SATIRI->kredi_karti_id          );
    query.SET_VALUE      ( ":cari_para_birimi_tutari"   , ROUND(new_cari_para_birimi_tutari ,-2)          );
    query.SET_VALUE      ( ":doviz_id"                  , P_FIS_SATIRI->doviz_id                );
    query.SET_VALUE      ( ":doviz_kuru"                , P_FIS_SATIRI->doviz_kuru              );
    query.SET_VALUE      ( ":parite"                    , P_FIS_SATIRI->parite                  );
    query.SET_VALUE      ( ":fis_satiri_id"             , p_fis_satiri_id                       );

    query.UPDATE         ();

    toplam_borc     = ROUND ( toplam_borc       - old_satir_borc_tutari);
    toplam_alacak   = ROUND ( toplam_alacak     - old_satir_alacak_tutari);

    toplam_borc     = ROUND ( toplam_borc       + new_satir_borc_tutari);
    toplam_alacak   = ROUND ( toplam_alacak     + new_satir_alacak_tutari);

    query.PREPARE_UPDATE ( "car_fisler", "fis_id", "toplam_borc, toplam_alacak     ",
                           " fis_id    = :fis_id" );
    query.SET_VALUE      ( ":toplam_borc",      toplam_borc             );
    query.SET_VALUE      ( ":toplam_alacak",    toplam_alacak           );
    query.SET_VALUE      ( ":fis_id",           P_FIS_SATIRI->fis_id    );
    query.UPDATE         ();

    P_FIS->toplam_borc      = toplam_borc;
    P_FIS->toplam_alacak    = toplam_alacak;

    bool cari_entegrasyon_fisi_olusturulacak = true;

    if ( old_islem_turu EQ new_islem_turu ) {

        switch ( new_islem_turu ) {
            case ENUM_MUSTERI_CEKI    :
            case ENUM_MUSTERI_SENEDI  :
            case ENUM_KENDI_CEKIMIZ   :
            case ENUM_KENDI_SENEDIMIZ :
                {
                if ( P_FIS->modul_id EQ CARI_MODULU ) {
                    cari_entegrasyon_fisi_olusturulacak = false;

                    if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                        CEK_SENET_STRUCT * CEK_SENET = new CEK_SENET_STRUCT;

                        CEK_SENET->banka_ismi         = P_FIS_SATIRI->musteri_banka_ismi;
                        CEK_SENET->borclu_kesideci    = P_FIS_SATIRI->kesideci;
                        CEK_SENET->cek_senet_numarasi = P_FIS_SATIRI->cek_senet_no;

                        int cek_senet_turu = ENUM_SENET;

                        if ( new_islem_turu EQ ENUM_MUSTERI_CEKI ) {
                            cek_senet_turu = ENUM_CEK;
                        }

                        CEK_SENET->cek_senet_turu   = cek_senet_turu;
                        CEK_SENET->cek_senet_tutari = P_FIS_SATIRI->borc_tutari;

                        if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                            CEK_SENET->cek_senet_tutari = P_FIS_SATIRI->alacak_tutari;
                        }
                        CEK_SENET->doviz_id               = P_FIS->makbuz_para_birimi_id;
                        CEK_SENET->hesap_no_id            = 0;
                        CEK_SENET->keside_tarihi          = P_FIS_SATIRI->cek_senet_keside_tarihi;
                        CEK_SENET->vade_tarihi            = P_FIS_SATIRI->cek_senet_vade_tarihi;
                        CEK_SENET->musteri_ceki_mi        = 1;
                        CEK_SENET->musteri_hesap_numarasi = P_FIS_SATIRI->musteri_hesap_no;
                        CEK_SENET->portfoy_numarasi       = P_FIS_SATIRI->portfoy_no;
                        CEK_SENET->sube_ismi              = P_FIS_SATIRI->musteri_sube_ismi;
                        CEK_SENET->sube_kodu              = P_FIS_SATIRI->musteri_sube_kodu;

                        CEK_SENET_GUNCELLE(CEK_SENET , new_cek_senet_id );
                    }


                    CEK_BORDRO_STRUCT * CEK_BORDROSU = new CEK_BORDRO_STRUCT;

                    CEK_BORDROSU->modul_id         = P_FIS->modul_id;
                    CEK_BORDROSU->program_id       = P_FIS->program_id;
                    CEK_BORDROSU->base_fis_id      = base_fis_id;
                    CEK_BORDROSU->muh_fis_id       = muh_fis_id;

                    CEK_BORDRO_SATIRI_STRUCT * BORDRO_SATIRI = new CEK_BORDRO_SATIRI_STRUCT;

                    QStringList BORDRO_ROW_ID_LIST = CEK_GET_ENT_ROW_ID_LIST(P_FIS->program_id,
                                                                             P_FIS->modul_id , p_fis_satiri_id);

                    for ( int i = 0 ; i < BORDRO_ROW_ID_LIST.size() ; i++ ) {

                        int bordro_satiri_id = QVariant(BORDRO_ROW_ID_LIST.at(i)).toInt();

                        CEK_BORDRO_SATIRINI_OKU(BORDRO_SATIRI , bordro_satiri_id );

                        BORDRO_SATIRI->aciklama         = P_FIS_SATIRI->aciklama;
                        BORDRO_SATIRI->cek_senet_id     = P_FIS_SATIRI->cek_senet_id;
                        BORDRO_SATIRI->modul_id         = P_FIS_SATIRI->modul_id;
                        BORDRO_SATIRI->program_id       = P_FIS_SATIRI->program_id;

                        BORDRO_SATIRI->cari_para_birimi_tutari  = new_cari_para_birimi_tutari;
                        BORDRO_SATIRI->temel_para_birimi_tutari = new_temel_para_birimi_tutari;

                        if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                            BORDRO_SATIRI->cek_tutari = P_FIS_SATIRI->alacak_tutari;
                        }
                        else {
                            BORDRO_SATIRI->cek_tutari = P_FIS_SATIRI->borc_tutari;
                        }

                        CEK_BORDRO_SATIRINI_GUNCELLE ( CEK_BORDROSU, BORDRO_SATIRI, bordro_satiri_id );
                    }

                    delete CEK_BORDROSU;
                    delete BORDRO_SATIRI;
                    return 0;
                }
            }
            break;
        case ENUM_HAVALE :
        case ENUM_EFT:
        case ENUM_KREDI_KARTI:
            {

                cari_entegrasyon_fisi_olusturulacak = false;

                BNK_HAREKET_FIS_STRUCT  * BNK_FIS = new BNK_HAREKET_FIS_STRUCT;
                BNK_HAREKET_FIS_SATIRI_STRUCT * BNK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

                int bnk_hareket_fisi_id = BNK_GET_ENT_FIS_ID( P_FIS->program_id, P_FIS->modul_id, base_fis_id );

                if( bnk_hareket_fisi_id EQ  0 ) {
                    BNK_FIS->base_fis_id        = base_fis_id;
                    BNK_FIS->program_id         = P_FIS->program_id;
                    BNK_FIS->modul_id           = P_FIS->modul_id;

                    BNK_FIS->aciklama           = P_FIS->aciklama;
                    BNK_FIS->fis_tarihi         = P_FIS->fis_tarihi;
                    BNK_FIS->muh_fis_id         = muh_fis_id;

                    bnk_hareket_fisi_id =  BNK_HAREKET_FIS_EKLE( BNK_FIS  );
                }
                else {
                  BNK_HAREKET_FIS_BILGILERINI_OKU( BNK_FIS, bnk_hareket_fisi_id );
                }

                BNK_FIS_SATIRI->fis_id                 = bnk_hareket_fisi_id;

                BNK_FIS_SATIRI->program_id             = P_FIS_SATIRI->program_id;
                BNK_FIS_SATIRI->modul_id               = P_FIS_SATIRI->modul_id;
                BNK_FIS_SATIRI->base_record_id         = p_fis_satiri_id;
                BNK_FIS_SATIRI->aciklama               = P_FIS_SATIRI->aciklama;
                BNK_FIS_SATIRI->karsi_hesap_turu       = ENUM_BNK_CARI_HESAP;
                BNK_FIS_SATIRI->karsi_hesap_id         = new_cari_hesap_id;

                BNK_FIS_SATIRI->hesaba_giren           = P_FIS_SATIRI->alacak_tutari;
                BNK_FIS_SATIRI->hesaptan_cikan         = P_FIS_SATIRI->borc_tutari;

                BNK_FIS_SATIRI->doviz_kuru             = P_FIS_SATIRI->doviz_kuru;
                BNK_FIS_SATIRI->parite                 = P_FIS_SATIRI->parite;


                switch ( P_FIS_SATIRI->islem_turu ) {

                case ENUM_HAVALE :
                    {
                        BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
                        BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->hesap_no_id; // banaka hesabi
                        break;
                    }
                case ENUM_EFT:
                    {
                        BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
                        BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->hesap_no_id; // banka hesabi
                        break;
                    }
                case ENUM_KREDI_KARTI:
                    {
                        BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_POS_HESABI;
                        BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->pos_id;
                        break;
                    }
                default:
                    break;
                }


                QStringList bnk_hareket_row_list =  BNK_GET_ENT_ROW_ID_LIST( P_FIS_SATIRI->program_id, P_FIS_SATIRI->modul_id, p_fis_satiri_id );

                if( bnk_hareket_row_list.count() > 0 ) {

                    for( int i = 0; i < bnk_hareket_row_list.count(); i++ ) {
                      new_record_id =  BNK_HAREKET_FIS_SATIRINI_GUNCELLE( BNK_FIS, BNK_FIS_SATIRI, QVariant ( bnk_hareket_row_list.at(i) ).toInt() );
                    }

                }

                delete BNK_FIS;
                delete BNK_FIS_SATIRI;

                break;
            }
        default:
            break;
        }
    }

    if ( cari_entegrasyon_fisi_olusturulacak EQ true ) {

        int base_record_id = P_FIS_SATIRI->base_record_id;

        if ( P_FIS->modul_id EQ CARI_MODULU ) {
            base_record_id = p_fis_satiri_id;
        }

        if ( muh_fis_id > 0 ) {

            int cari_muh_hesap_id = CARI_FIND_HESAP_MUH_HESAP_ID ( new_cari_hesap_id );

            MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;
            MUH_CLEAR_FIS_STRUCT ( MUH_FIS );
            MUH_FIS_BILGILERINI_OKU ( MUH_FIS, muh_fis_id );


            if ( cari_muh_hesap_id EQ 0 ) {
                MUH_FIS->aciklama   = QObject::tr("Cari hesabın muhasebe hesabı bulunamadı. \n" ) + P_FIS->aciklama;
                MUH_FISI_GUNCELLE    ( MUH_FIS, muh_fis_id );

                QStringList MUH_ROW_ID_LIST = MUH_GET_ENT_ROW_ID_LIST(P_FIS->program_id,P_FIS->modul_id ,
                                                                      base_record_id );

                for ( int i = 0 ; i < MUH_ROW_ID_LIST.size() ; i++ ) {
                    int muh_fis_satiri_id = QVariant(MUH_ROW_ID_LIST.at(i)).toInt();

                    MUH_FIS_SATIRINI_SIL ( MUH_FIS, muh_fis_satiri_id );
                }

            }
            else {
                double      muh_fis_satiri_alacak_tutari    =   0.0;
                double      muh_fis_satiri_borc_tutari      =   0.0;

                if ( P_FIS_SATIRI->borc_tutari > 0 ) {
                    muh_fis_satiri_borc_tutari      = new_temel_para_birimi_tutari;
                }
                else {
                    muh_fis_satiri_alacak_tutari = new_temel_para_birimi_tutari;
                }



                MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;
                MUH_CLEAR_FIS_SATIRI_STRUCT     ( MUH_FIS_SATIRI );
                MUH_FIS_SATIRI->aciklama                = P_FIS_SATIRI->aciklama;
                MUH_FIS_SATIRI->borc_tutari             = ROUND(muh_fis_satiri_borc_tutari,-2);
                MUH_FIS_SATIRI->alacak_tutari           = ROUND(muh_fis_satiri_alacak_tutari,2);
                MUH_FIS_SATIRI->fis_id                  = muh_fis_id;
                MUH_FIS_SATIRI->hesap_id                = cari_muh_hesap_id;
                MUH_FIS_SATIRI->modul_id                = P_FIS_SATIRI->modul_id;
                MUH_FIS_SATIRI->program_id              = P_FIS_SATIRI->program_id;
                MUH_FIS_SATIRI->base_record_id          = P_FIS_SATIRI->base_record_id;

                QStringList MUH_ROW_ID_LIST = MUH_GET_ENT_ROW_ID_LIST(P_FIS->program_id,P_FIS->modul_id ,
                                                                      base_record_id );

                for ( int i = 0 ; i < MUH_ROW_ID_LIST.size() ; i++ ) {
                    int muh_fis_satiri_id = QVariant(MUH_ROW_ID_LIST.at(i)).toInt();

                    MUH_FIS_SATIRINI_GUNCELLE ( MUH_FIS, MUH_FIS_SATIRI, muh_fis_satiri_id );
                }

                delete MUH_FIS_SATIRI;
            }
            delete MUH_FIS;
        }
    }

    return new_record_id;
}

/**************************************************************************************
                        CARI_FIS_BILGILERINI_OKU
***************************************************************************************/

void CARI_FIS_BILGILERINI_OKU ( CARI_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT ( "car_fisler",
                           "fis_no, fis_tarihi, cari_hesap_id, aciklama, toplam_borc, toplam_alacak, "
                           "belge_seri, belge_numarasi, fis_turu, makbuz_para_birimi_id, doviz_kuru, parite, "
                           "muh_kasa_hesap_id, modul_id, program_id ,"
                           "base_fis_id,muh_fis_id, yazdirildi_mi, iptal_mi ",
                           "fis_id = :fis_id" );

    query.SET_VALUE     ( ":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    P_FIS->fis_no                   = query.VALUE ( "fis_no"                ).toInt();
    P_FIS->fis_tarihi               = query.VALUE ( "fis_tarihi"            ).toString();
    P_FIS->cari_hesap_id            = query.VALUE ( "cari_hesap_id"         ).toInt();
    P_FIS->aciklama                 = query.VALUE ( "aciklama"              ).toString();
    P_FIS->toplam_borc              = query.VALUE ( "toplam_borc"           ).toDouble();
    P_FIS->toplam_alacak            = query.VALUE ( "toplam_alacak"         ).toDouble();
    P_FIS->belge_seri               = query.VALUE ( "belge_seri"            ).toString();
    P_FIS->belge_numarasi           = query.VALUE ( "belge_numarasi"        ).toString();
    P_FIS->fis_turu                 = query.VALUE ( "fis_turu"              ).toInt();
    P_FIS->makbuz_para_birimi_id    = query.VALUE ( "makbuz_para_birimi_id" ).toInt();
    P_FIS->doviz_kuru               = query.VALUE ( "doviz_kuru"            ).toDouble();
    P_FIS->parite                   = query.VALUE ( "parite"                ).toDouble();
    P_FIS->muh_kasa_hesap_id        = query.VALUE ( "muh_kasa_hesap_id"     ).toInt();
    P_FIS->modul_id                 = query.VALUE ( "modul_id"              ).toInt();
    P_FIS->program_id               = query.VALUE ( "program_id"            ).toInt();
    P_FIS->base_fis_id              = query.VALUE ( "base_fis_id"           ).toInt();
    P_FIS->muh_fis_id               = query.VALUE ( "muh_fis_id"            ).toInt();
    P_FIS->yazdirildi_mi            = query.VALUE ( "yazdirildi_mi"         ).toInt();
    P_FIS->iptal_mi                 = query.VALUE ( "iptal_mi"              ).toInt();

}

/**************************************************************************************
                   CARI_FIS_SATIRINI_OKU
***************************************************************************************/

void CARI_FIS_SATIRINI_OKU(CARI_FIS_SATIRI_STRUCT *P_FIS_SATIRI, int p_fis_satiri_id ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("car_fis_satirlari","fis_id, aciklama, borc_tutari, alacak_tutari, kredi_karti_numarasi,"
                         "kredi_karti_sahibi, musteri_banka_ismi, musteri_sube_kodu, musteri_sube_ismi,"
                         "musteri_hesap_no, cari_hesap_id, cek_senet_id, islem_turu, order_number,"
                         "muh_hesap_id, hesap_no_id, pos_id, kredi_karti_id, "
                         "doviz_id, doviz_kuru, parite ,"
                         "base_record_id , modul_id , program_id, cari_para_birimi_tutari ",
                         "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    P_FIS_SATIRI->fis_id                   = query.VALUE ( "fis_id"                   ).toInt();
    P_FIS_SATIRI->aciklama                 = query.VALUE ( "aciklama"                 ).toString();
    P_FIS_SATIRI->borc_tutari              = query.VALUE ( "borc_tutari"              ).toDouble();
    P_FIS_SATIRI->alacak_tutari            = query.VALUE ( "alacak_tutari"            ).toDouble();
    P_FIS_SATIRI->kredi_karti_numarasi     = query.VALUE ( "kredi_karti_numarasi"     ).toString();
    P_FIS_SATIRI->kredi_karti_sahibi       = query.VALUE ( "kredi_karti_sahibi"       ).toString();
    P_FIS_SATIRI->musteri_banka_ismi       = query.VALUE ( "musteri_banka_ismi"       ).toString();
    P_FIS_SATIRI->musteri_sube_kodu        = query.VALUE ( "musteri_sube_kodu"        ).toString();
    P_FIS_SATIRI->musteri_sube_ismi        = query.VALUE ( "musteri_sube_ismi"        ).toString();
    P_FIS_SATIRI->musteri_hesap_no         = query.VALUE ( "musteri_hesap_no"         ).toString();
    P_FIS_SATIRI->cari_hesap_id            = query.VALUE ( "cari_hesap_id"            ).toInt();
    P_FIS_SATIRI->cek_senet_id             = query.VALUE ( "cek_senet_id"             ).toInt();
    P_FIS_SATIRI->islem_turu               = query.VALUE ( "islem_turu"               ).toInt();
    P_FIS_SATIRI->order_number             = query.VALUE ( "order_number"             ).toInt();
    P_FIS_SATIRI->muh_hesap_id             = query.VALUE ( "muh_hesap_id"             ).toInt();
    P_FIS_SATIRI->hesap_no_id              = query.VALUE ( "hesap_no_id"              ).toInt();
    P_FIS_SATIRI->pos_id                   = query.VALUE ( "pos_id"                   ).toInt();
    P_FIS_SATIRI->kredi_karti_id           = query.VALUE ( "kredi_karti_id"           ).toInt();
    P_FIS_SATIRI->doviz_id                 = query.VALUE ( "doviz_id"                 ).toInt();
    P_FIS_SATIRI->doviz_kuru               = query.VALUE ( "doviz_kuru"               ).toDouble();
    P_FIS_SATIRI->parite                   = query.VALUE ( "parite"                   ).toDouble();
    P_FIS_SATIRI->base_record_id           = query.VALUE ( "base_record_id"           ).toInt();
    P_FIS_SATIRI->modul_id                 = query.VALUE ( "modul_id"                 ).toInt();
    P_FIS_SATIRI->program_id               = query.VALUE ( "program_id"               ).toInt();
    P_FIS_SATIRI->cari_para_birimi_tutari  = query.VALUE ( "cari_para_birimi_tutari"  ).toDouble();
    P_FIS_SATIRI->doviz_kodu               = DVZ_GET_DOVIZ_KODU ( P_FIS_SATIRI->doviz_id );
}



/**************************************************************************************
                   CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_ARTIR
***************************************************************************************/

void CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_ARTIR ( QString p_para_birimi_kodu, double p_satir_tutari, int borc_satiri, QStringList & P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,  QStringList & P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI )
{
    QStringList TOPLAM_BILGILERI;

    double      para_birimi_toplami = p_satir_tutari;

    if ( borc_satiri EQ 1 ) {
        QStringList TOPLAM_BILGILERI;

        double      para_birimi_toplami = p_satir_tutari;

        for ( int i = 0; i < P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.size(); i++ ) {
            TOPLAM_BILGILERI = P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.at(i).split("\t");
            if ( TOPLAM_BILGILERI.at(1) EQ p_para_birimi_kodu ) {
                para_birimi_toplami += QVariant ( TOPLAM_BILGILERI.at(0)).toDouble();
                P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI[i] = QVariant ( para_birimi_toplami ).toString() + "\t" + p_para_birimi_kodu;
                return;
            }
        }
        QString satir_bilgileri = QVariant ( p_satir_tutari ).toString()  + "\t" + p_para_birimi_kodu;
        P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.append ( satir_bilgileri );
        return;
    }

    for ( int i = 0; i < P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.size(); i++ ) {
        TOPLAM_BILGILERI = P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.at(i).split("\t");
        if ( TOPLAM_BILGILERI.at(1) EQ p_para_birimi_kodu ) {
            para_birimi_toplami += QVariant ( TOPLAM_BILGILERI.at(0)).toDouble();
            P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI[i] = QVariant ( para_birimi_toplami).toString() + "\t" + p_para_birimi_kodu;
            return;
        }
    }
    QString satir_bilgileri = QVariant ( p_satir_tutari ).toString()  + "\t" + p_para_birimi_kodu;
    P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.append ( satir_bilgileri );

}

/**************************************************************************************
                   CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_AZALT
***************************************************************************************/

void CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_AZALT ( QString p_para_birimi_kodu, double p_satir_tutari, int p_borc_satiri, QStringList & P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,  QStringList & P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI )
{
    QStringList TOPLAM_BILGILERI;

    double      para_birimi_toplami;

    if ( p_borc_satiri EQ 1 ) {

        for ( int i = 0; i < P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.size(); i++ ) {
            TOPLAM_BILGILERI = P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.at(i).split("\t");
            if ( TOPLAM_BILGILERI.at(1) EQ p_para_birimi_kodu ) {
                para_birimi_toplami = QVariant ( TOPLAM_BILGILERI.at(0)).toDouble() - p_satir_tutari;
                if ( para_birimi_toplami EQ 0 ) {
                    P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.removeAt(i);
                    return;
                }
                P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI[i] = QVariant ( para_birimi_toplami ).toString() + "\t" + TOPLAM_BILGILERI.at(1);
                return;
            }
        }
        return;
    }


    for ( int i = 0; i < P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.size(); i++ ) {
        TOPLAM_BILGILERI = P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.at(i).split ( "\t" ) ;
        if ( TOPLAM_BILGILERI.at(1) EQ p_para_birimi_kodu ) {
            para_birimi_toplami = QVariant ( TOPLAM_BILGILERI.at(0)).toDouble() - p_satir_tutari;
            if ( para_birimi_toplami EQ 0 ) {
                P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.removeAt(i);
                return;
            }
            P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI[i] = QVariant ( para_birimi_toplami ).toString() + "\t" + TOPLAM_BILGILERI.at(1);
            return;
        }
    }
}


/**************************************************************************************
                   CARI_GET_HAREKET_FISI_SATIR_DOVIZ_BORC_TUTARLARI
***************************************************************************************/

QString CARI_GET_HAREKET_FISI_SATIR_DOVIZ_BORC_TUTARLARI ( QStringList & P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI )
{
    QStringList SATIR_BILGILERI;
    QString     satir_doviz_borc_tutarlari;

    for ( int i = 0; i < P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.size(); i++ ) {
         SATIR_BILGILERI = P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.at(i).split("\t");
         satir_doviz_borc_tutarlari.append ( VIRGUL_EKLE ( QVariant ( ROUND ( SATIR_BILGILERI.at(0).toDouble())).toString(), 2, false )
                                                            + "\t" + SATIR_BILGILERI.at(1) + "\n" );
    }
    return satir_doviz_borc_tutarlari;
}

/**************************************************************************************
                   CARI_GET_HAREKET_FISI_SATIR_DOVIZ_ALACAK_TUTARLARI
***************************************************************************************/

QString CARI_GET_HAREKET_FISI_SATIR_DOVIZ_ALACAK_TUTARLARI ( QStringList & P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI )
{
    QStringList SATIR_BILGILERI;
    QString     satir_doviz_alacak_tutarlari;

    for ( int i = 0; i < P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.size(); i++ ) {
         SATIR_BILGILERI = P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.at(i).split("\t");
         satir_doviz_alacak_tutarlari.append ( VIRGUL_EKLE ( QVariant ( ROUND ( SATIR_BILGILERI.at(0).toDouble() )).toString(), 2, false )
                                                            + "\t" + SATIR_BILGILERI.at(1) + "\n" );
    }
    return satir_doviz_alacak_tutarlari;
}

/**************************************************************************************
                   CARI_CLEAR_FIS_STRUCT()
***************************************************************************************/

void CARI_CLEAR_FIS_STRUCT(CARI_FIS_STRUCT *P_FIS)
{
    P_FIS->modul_id              = -1;
    P_FIS->program_id            = -1;
    P_FIS->fis_no                =  0;
    P_FIS->fis_turu              = -1;
    P_FIS->cari_hesap_id         =  0;
    P_FIS->toplam_borc           =  0;
    P_FIS->toplam_alacak         =  0;
    P_FIS->makbuz_para_birimi_id =  0;
    P_FIS->cari_para_birimi_id   =  0;
    P_FIS->doviz_kuru            =  0;
    P_FIS->parite                =  0;
    P_FIS->muh_kasa_hesap_id     =  0;
    P_FIS->belge_numarasi.clear();
    P_FIS->base_fis_id           =  0;
    P_FIS->muh_fis_id            =  0;
    P_FIS->yazdirildi_mi         =  0;
    P_FIS->iptal_mi              =  0;

    P_FIS->fis_tarihi.clear();
    P_FIS->aciklama.clear();
    P_FIS->belge_seri.clear();
    P_FIS->borc_doviz_tutar_bilgileri.clear();
    P_FIS->alacak_doviz_tutar_bilgileri.clear();
    P_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.clear();
    P_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.clear();

}

/**************************************************************************************
                   CARI_CLEAR_FIS_SATIRI_STRUCT()
***************************************************************************************/

void CARI_CLEAR_FIS_SATIRI_STRUCT(CARI_FIS_SATIRI_STRUCT *P_FIS_SATIRI)
{
    P_FIS_SATIRI->fis_id                    =  0;
    P_FIS_SATIRI->borc_tutari               =  0;
    P_FIS_SATIRI->alacak_tutari             =  0;
    P_FIS_SATIRI->cari_hesap_id             =  0;
    P_FIS_SATIRI->cek_senet_id              =  0;
    P_FIS_SATIRI->islem_turu                = -1;
    P_FIS_SATIRI->order_number              =  0;
    P_FIS_SATIRI->muh_hesap_id              =  0;
    P_FIS_SATIRI->hesap_no_id               =  0;
    P_FIS_SATIRI->pos_id                    =  0;
    P_FIS_SATIRI->kredi_karti_id            =  0;
    P_FIS_SATIRI->doviz_id                  =  0;
    P_FIS_SATIRI->doviz_kuru                =  0;
    P_FIS_SATIRI->parite                    =  0;
    P_FIS_SATIRI->portfoy_no                =  0;
    P_FIS_SATIRI->program_id                = -1;
    P_FIS_SATIRI->modul_id                  = -1;
    P_FIS_SATIRI->base_record_id            =  0;
    P_FIS_SATIRI->cari_para_birimi_tutari   = 0.00;


    P_FIS_SATIRI->aciklama.clear();
    P_FIS_SATIRI->kredi_karti_numarasi.clear();
    P_FIS_SATIRI->kredi_karti_sahibi.clear();
    P_FIS_SATIRI->musteri_banka_ismi.clear();
    P_FIS_SATIRI->musteri_hesap_no.clear();
    P_FIS_SATIRI->musteri_sube_ismi.clear();
    P_FIS_SATIRI->musteri_sube_kodu.clear();
    P_FIS_SATIRI->cek_senet_vade_tarihi.clear();
    P_FIS_SATIRI->cek_senet_keside_tarihi.clear();
    P_FIS_SATIRI->kesideci.clear();
    P_FIS_SATIRI->cek_senet_no.clear();
    P_FIS_SATIRI->doviz_kodu.clear();
}

/**************************************************************************************
                            CARI_GET_ENT_FIS_ID
***************************************************************************************/

int CARI_GET_ENT_FIS_ID( int program_id, int modul_id, int base_fis_id , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query( USE_DB );

    query.PREPARE_SELECT( "car_fisler", "fis_id",
                          "program_id =:program_id AND modul_id =:modul_id "
                          "AND base_fis_id =:base_fis_id");

    query.SET_VALUE( ":program_id",     program_id );
    query.SET_VALUE( ":modul_id",       modul_id );
    query.SET_VALUE( ":base_fis_id",    base_fis_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE( 0 ).toInt();
}

/**************************************************************************************
                            CARI_GET_ENT_ROW_ID_LIST
***************************************************************************************/

QStringList CARI_GET_ENT_ROW_ID_LIST ( int program_id, int modul_id, int base_record_id ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    QStringList row_id_list;

    SQL_QUERY query( USE_DB );

    query.PREPARE_SELECT( "car_fis_satirlari", "fis_satiri_id",
                          "program_id =:program_id AND "
                          "modul_id =:modul_id AND "
                          "base_record_id =:base_record_id");

    query.SET_VALUE( ":program_id",     program_id      );
    query.SET_VALUE( ":modul_id",       modul_id        );
    query.SET_VALUE( ":base_record_id", base_record_id  );

    if( query.SELECT() EQ 0 )
    {
        return row_id_list;
    }

    while( query.NEXT() EQ true )
    {
        row_id_list << query.VALUE( 0 ).toString();
    }

    return row_id_list;
}

/**************************************************************************************
                            CARI_BANKA_MODULUNE_FIS_SATIR_EKLE
***************************************************************************************/

void CARI_BANKA_MODULUNE_FIS_SATIR_EKLE(CARI_FIS_STRUCT *P_FIS, CARI_FIS_SATIRI_STRUCT *P_FIS_SATIRI,
                                   int cari_hesap_id, int base_fis_id ,ADAK_SQL * P_GELEN_DB)
{
    //Fonksiyon banka modulunde fis yoksa fis ve bu fise banka satiri ekler
    //Entegrasyon fislerinde once borc satiri eklenmesi gerektigi icin fonksiyona cekilmistir.

    BNK_HAREKET_FIS_STRUCT  * BNK_FIS = new BNK_HAREKET_FIS_STRUCT;
    BNK_HAREKET_FIS_SATIRI_STRUCT * BNK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

    int bnk_hareket_fisi_id = BNK_GET_ENT_FIS_ID( P_FIS->program_id, P_FIS->modul_id, base_fis_id, P_FIS_SATIRI->hesap_no_id ,P_GELEN_DB);

    QString aciklama = QObject::tr("Fiş CARİ modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
    aciklama += P_FIS->aciklama;


    if ( bnk_hareket_fisi_id EQ  0 ) {

        BNK_CLEAR_HAREKET_FIS_STRUCT ( BNK_FIS );
        BNK_FIS->base_fis_id                        = base_fis_id;
        BNK_FIS->program_id                         = P_FIS->program_id;
        BNK_FIS->modul_id                           = P_FIS->modul_id;
        BNK_FIS->hesap_no_id                        = P_FIS_SATIRI->hesap_no_id;
        BNK_FIS->aciklama                           = aciklama;
        BNK_FIS->fis_tarihi                         = P_FIS->fis_tarihi;
        BNK_FIS->fis_turu                           = ENUM_BANKA_HAREKET_FISI;
        BNK_FIS->muh_fis_id                         = P_FIS->muh_fis_id;
        BNK_FIS->odm_emr_hesabi_etkilenecek_mi      = 1;
        BNK_FIS->para_birim_id                      = P_FIS->makbuz_para_birimi_id;


        bnk_hareket_fisi_id =  BNK_HAREKET_FIS_EKLE( BNK_FIS ,0 , true,P_GELEN_DB);

    }
    else {

      BNK_HAREKET_FIS_BILGILERINI_OKU( BNK_FIS, bnk_hareket_fisi_id ,P_GELEN_DB);
      BNK_FIS->hesap_no_id = P_FIS_SATIRI->hesap_no_id;
    }

    BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT(BNK_FIS_SATIRI);

    BNK_FIS_SATIRI->fis_id                 = bnk_hareket_fisi_id;
    BNK_FIS_SATIRI->program_id             = P_FIS_SATIRI->program_id;
    BNK_FIS_SATIRI->modul_id               = P_FIS_SATIRI->modul_id;
    BNK_FIS_SATIRI->base_record_id         = P_FIS_SATIRI->base_record_id;
    BNK_FIS_SATIRI->karsi_hesap_turu       = ENUM_BNK_CARI_HESAP;
    BNK_FIS_SATIRI->karsi_hesap_id         = cari_hesap_id;

    BNK_FIS_SATIRI->hesaba_giren           = P_FIS_SATIRI->alacak_tutari;
    BNK_FIS_SATIRI->hesaptan_cikan         = P_FIS_SATIRI->borc_tutari;

    BNK_FIS_SATIRI->doviz_kuru             = P_FIS_SATIRI->doviz_kuru;
    BNK_FIS_SATIRI->parite                 = P_FIS_SATIRI->parite;
    BNK_FIS_SATIRI->aciklama               = P_FIS_SATIRI->aciklama;

    switch ( P_FIS_SATIRI->islem_turu ) {

        case ENUM_HAVALE :
            {
                BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
                BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->hesap_no_id; // banaka hesabi
                break;
            }
        case ENUM_EFT:
            {
                BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
                BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->hesap_no_id; // banka hesabi
                break;
            }
        case ENUM_KREDI_KARTI:
            {
                if ( P_FIS->fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                    BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_POS_HESABI;
                    BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->pos_id;
                }
                else {
                    BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_KREDI_KARTI_HESABI;
                    BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->kredi_karti_id;
                }
                break;
            }
        default:
            break;
    }

    BNK_HAREKET_FIS_SATIRI_EKLE( BNK_FIS, BNK_FIS_SATIRI,0,true,P_GELEN_DB );

    delete BNK_FIS;
    delete BNK_FIS_SATIRI;

}

/**************************************************************************************
                            CARI_CEKSENET_MODULUNE_FIS_SATIR_EKLE
***************************************************************************************/

void CARI_CEKSENET_MODULUNE_FIS_SATIR_EKLE(CARI_FIS_STRUCT *P_FIS, CARI_FIS_SATIRI_STRUCT *P_FIS_SATIRI,
                                      double temel_para_birimi_tutari, double cari_para_birimi_tutari,int base_fis_id , bool p_cek_hareketi_ekle,
                                      ADAK_SQL * P_GELEN_DB)
{
    //Ceksenet modulune fis yoksa fis ve satir ekler..
    //Muhasebe fisine once borc satiri eklenmesi gerektigi icin fonjksiyona cekilmistir.

    CEK_BORDRO_STRUCT * CEK_BORDROSU = new CEK_BORDRO_STRUCT;

    QString aciklama = QObject::tr("Fiş CARİ modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(P_FIS->fis_no).toString());
    aciklama += P_FIS->aciklama;

    CEK_BORDROSU->aciklama            = aciklama;
    CEK_BORDROSU->muh_fis_id          = P_FIS->muh_fis_id;

    QStringList BORDRO_ID_LIST;

    BORDRO_ID_LIST = CEK_GET_ENT_FIS_ID_LIST ( P_FIS->program_id, P_FIS->modul_id, P_FIS_SATIRI->fis_id, P_GELEN_DB );

    if ( BORDRO_ID_LIST.size() EQ 0 ) {

        CEK_BORDROSU->program_id            = P_FIS->program_id;
        CEK_BORDROSU->modul_id              = P_FIS->modul_id;
        CEK_BORDROSU->bordro_tarihi         = P_FIS->fis_tarihi;
        CEK_BORDROSU->cari_hesap_id         = P_FIS->cari_hesap_id;
        CEK_BORDROSU->cari_para_birimi_id   = P_FIS->cari_para_birimi_id;
        CEK_BORDROSU->bordro_para_birimi_id = P_FIS->makbuz_para_birimi_id;
        CEK_BORDROSU->doviz_kuru            = P_FIS->doviz_kuru;
        CEK_BORDROSU->parite                = P_FIS->parite;
        CEK_BORDROSU->bordro_no             = 0;
        CEK_BORDROSU->musteri_cek_senedi_mi = EVET;

        if ( P_FIS_SATIRI->islem_turu EQ ENUM_MUSTERI_CEKI ) {
            CEK_BORDROSU->bordro_turu       = ENUM_MUSTERI_CEK_BORDROSU;
            CEK_BORDROSU->cek_bordrosu_mu   = 1;
        }
        else if ( P_FIS_SATIRI->islem_turu EQ ENUM_MUSTERI_SENEDI ){
            CEK_BORDROSU->bordro_turu       = ENUM_MUSTERI_SENET_BORDROSU;
            CEK_BORDROSU->cek_bordrosu_mu   = 0;
        }
        else if ( P_FIS_SATIRI->islem_turu EQ ENUM_KENDI_CEKIMIZ ) {
            CEK_BORDROSU->bordro_turu       = ENUM_FIRMA_CEK_BORDROSU;
            CEK_BORDROSU->cek_bordrosu_mu   = 1;
        }
        else if ( P_FIS_SATIRI->islem_turu EQ ENUM_KENDI_SENEDIMIZ ) {
            CEK_BORDROSU->bordro_turu       = ENUM_FIRMA_SENET_BORDROSU;
            CEK_BORDROSU->cek_bordrosu_mu   = 0;
        }

        if ( P_FIS->fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
            CEK_BORDROSU->bordro_islem_turu = ENUM_CEK_MUSTERIDEN_ALINDI;
        }
        else {
            CEK_BORDROSU->bordro_islem_turu = ENUM_CEK_SATICIYA_VERILDI;
        }

        CEK_BORDROSU->base_fis_id = base_fis_id;
        CEK_BORDROSU->hesap_no_id = P_FIS_SATIRI->hesap_no_id;

        BORDRO_ID_LIST.append ( QVariant ( CEK_BORDROSU_EKLE ( CEK_BORDROSU , P_GELEN_DB) ).toString() );

    }
    else {
        CEK_BORDRO_BILGILERI_OKU ( CEK_BORDROSU, QVariant ( BORDRO_ID_LIST.at ( 0 ) ).toInt(), P_GELEN_DB);
    }

    CEK_BORDRO_SATIRI_STRUCT * BORDRO_SATIRI = new CEK_BORDRO_SATIRI_STRUCT;

    CEK_CLEAR_BORDRO_SATIRI_STRUCT ( BORDRO_SATIRI );

    BORDRO_SATIRI->bordro_id                = QVariant ( BORDRO_ID_LIST.at ( 0 ) ).toInt();
    BORDRO_SATIRI->cek_senet_id             = P_FIS_SATIRI->cek_senet_id;
    BORDRO_SATIRI->aciklama                 = P_FIS_SATIRI->aciklama;
    BORDRO_SATIRI->temel_para_birimi_tutari = temel_para_birimi_tutari;
    BORDRO_SATIRI->cari_para_birimi_tutari  = cari_para_birimi_tutari;
    BORDRO_SATIRI->base_record_id           = P_FIS_SATIRI->base_record_id;
    BORDRO_SATIRI->modul_id                 = P_FIS_SATIRI->modul_id;
    BORDRO_SATIRI->program_id               = P_FIS_SATIRI->program_id;

    if ( P_FIS->fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
        BORDRO_SATIRI->cek_tutari = P_FIS_SATIRI->alacak_tutari;
    }
    else {
        BORDRO_SATIRI->cek_tutari = P_FIS_SATIRI->borc_tutari;
    }

    CEK_BORDRO_SATIRI_EKLE( CEK_BORDROSU, BORDRO_SATIRI, p_cek_hareketi_ekle, true,0, P_GELEN_DB );


    delete BORDRO_SATIRI;
    delete CEK_BORDROSU;
}

/**************************************************************************************
                            CARI_GET_MUH_FIS_SATIRLARI_ID_LIST
***************************************************************************************/

QStringList   CARI_GET_FIS_SATIRLARI_ID_LIST ( int p_program_id, int p_modul_id, int p_fis_id )
{
    QStringList row_id_list;

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT( "car_fis_satirlari", "fis_satiri_id", "program_id = :program_id AND modul_id = :modul_id AND fis_id = :fis_id" );

    query.SET_VALUE     ( ":program_id" , p_program_id );
    query.SET_VALUE     ( ":modul_id"   , p_modul_id );
    query.SET_VALUE     ( ":fis_id"     , p_fis_id );

    if ( query.SELECT() NE 0 ) {
        while (query.NEXT() EQ true ) {
            row_id_list << query.VALUE( 0 ).toString();
        }
    }

    return row_id_list;
}

/**************************************************************************************
                            CARI_FISE_GORE_GET_ENT_ROW_ID_LIST
***************************************************************************************/

QStringList CARI_FISE_GORE_GET_ENT_ROW_ID_LIST(int p_program_id, int p_modul_id,
                                              int p_base_record_id, int p_fis_id)
{
    QStringList row_id_list;

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_fis_satirlari", "fis_satiri_id",
                          "program_id =:program_id AND "
                          "modul_id =:modul_id AND "
                          "base_record_id =:base_record_id AND fis_id = :fis_id");

    query.SET_VALUE( ":program_id",     p_program_id      );
    query.SET_VALUE( ":modul_id",       p_modul_id        );
    query.SET_VALUE( ":base_record_id", p_base_record_id  );
    query.SET_VALUE( ":fis_id"        , p_fis_id          );

    if( query.SELECT() EQ 0 )
    {
        return row_id_list;
    }

    while( query.NEXT() EQ true )
    {
        row_id_list << query.VALUE( 0 ).toString();
    }

    return row_id_list;
}

/**************************************************************************************
                            CARI_FISE_GORE_GET_ENT_ROW_ID_LIST
***************************************************************************************/

FIS_YAZICI_DURMU CARI_GET_FIS_YAZDIRILDIMI ( int p_fis_id )
{

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT( "car_fisler", "yazdirildi_mi", "fis_id = :fis_id" );
    query.SET_VALUE     ( ":fis_id"     , p_fis_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        if ( query.VALUE( 0 ).toInt() EQ 1 ) {
            return ENUM_FIS_YAZILDI;
        }
    }

    return ENUM_FIS_YAZILMADI;
}


