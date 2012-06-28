#include "adak_sql.h"
#include "cek_fis_utils.h"
#include "cek_enum.h"
#include "cek_console_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
#include "cek_senet_utils.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "bnk_fis_utils.h"
#include "muh_enum.h"
#include "muh_console_utils.h"
#include "sube_console_utils.h"
#include "e9_enum.h"
#include "banka_struct.h"
#include "banka_enum.h"
#include "bnk_console_utils.h"
#include "cari_enum.h"
#include "sube_struct.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                                    CEK_BORDROSU_EKLE()
***************************************************************************************/

int CEK_BORDROSU_EKLE ( CEK_BORDRO_STRUCT  *  P_BORDRO , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    int bordro_no = P_BORDRO->bordro_no;

    if ( P_BORDRO->bordro_no EQ 0 ) {
        bordro_no = CEK_SIRADAKI_BORDRO_NO_AL ( P_BORDRO->bordro_tarihi , USE_DB);
    }

    SQL_QUERY query ( USE_DB );

    query.PREPARE_INSERT ( "cek_bordrolar", "bordro_id",
                           "bordro_no,bordro_tarihi,bordro_islem_turu,aciklama,"
                           "cari_hesap_id , hesap_no_id , toplam_tutar , "
                           "modul_id , bordro_turu , cek_bordrosu_mu , program_id,"
                           "last_order_number , bordro_para_birimi_id , doviz_kuru , parite ,"
                           "musteri_cek_senedi_mi ,base_fis_id , muh_fis_id" );

    query.SET_VALUE      ( ":bordro_no"             , bordro_no         );
    query.SET_VALUE      ( ":bordro_tarihi"         , P_BORDRO->bordro_tarihi     );
    query.SET_VALUE      ( ":bordro_islem_turu"     , P_BORDRO->bordro_islem_turu );
    query.SET_VALUE      ( ":aciklama"              , P_BORDRO->aciklama          );
    query.SET_VALUE      ( ":cari_hesap_id"         , P_BORDRO->cari_hesap_id     );
    query.SET_VALUE      ( ":hesap_no_id"           , P_BORDRO->hesap_no_id       );
    query.SET_VALUE      ( ":modul_id"              , P_BORDRO->modul_id          );
    query.SET_VALUE      ( ":bordro_turu"           , P_BORDRO->bordro_turu       );
    query.SET_VALUE      ( ":cek_bordrosu_mu"       , P_BORDRO->cek_bordrosu_mu   );
    query.SET_VALUE      ( ":program_id"            , P_BORDRO->program_id        );
    query.SET_VALUE      ( ":toplam_tutar"          , 0 );
    query.SET_VALUE      ( ":last_order_number"     , 0 );
    query.SET_VALUE      ( ":bordro_para_birimi_id" , P_BORDRO->bordro_para_birimi_id );
    query.SET_VALUE      ( ":doviz_kuru"            , ROUND(P_BORDRO->doviz_kuru , -4));
    query.SET_VALUE      ( ":parite"                , ROUND(P_BORDRO->parite     , -4));
    query.SET_VALUE      ( ":musteri_cek_senedi_mi" , P_BORDRO->musteri_cek_senedi_mi );
    query.SET_VALUE      ( ":base_fis_id"           , P_BORDRO->base_fis_id           );
    query.SET_VALUE      ( ":muh_fis_id"            , P_BORDRO->muh_fis_id          );

    int bordro_id = query.INSERT();

    if ( P_BORDRO->bordro_turu EQ ENUM_CEK_SENET_ACILIS_BORDROSU ) {
        return bordro_id;
    }

    if ( P_BORDRO->muh_fis_id EQ 0 AND P_BORDRO->modul_id EQ CEKSENET_MODULU ) {

        MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        if ( P_BORDRO->modul_id EQ CEKSENET_MODULU ) {
            QString aciklama = QObject::tr("Fiş ÇEK SENET modülü tarafından üretilmiştir.Bordro No : %1\n").arg(QVariant(P_BORDRO->bordro_no).toString());
            aciklama.append(P_BORDRO->aciklama);

            MUHASEBE_FISI->aciklama = aciklama;
            MUHASEBE_FISI->fis_turu = ENUM_MAHSUP_FISI;
        }
        else {
            MUHASEBE_FISI->aciklama = P_BORDRO->aciklama;
            if ( P_BORDRO->bordro_islem_turu EQ ENUM_CEK_SATICIYA_VERILDI ) {
                MUHASEBE_FISI->fis_turu = ENUM_TEDIYE_FISI;
            }
            else {
                MUHASEBE_FISI->fis_turu = ENUM_MAHSUP_FISI;
            }
        }

        MUHASEBE_FISI->fis_tarihi       = P_BORDRO->bordro_tarihi;
        MUHASEBE_FISI->modul_id         = P_BORDRO->modul_id;
        MUHASEBE_FISI->program_id       = P_BORDRO->program_id;
        MUHASEBE_FISI->base_fis_id      = bordro_id;

        P_BORDRO->muh_fis_id = MUH_FIS_EKLE ( MUHASEBE_FISI ,USE_DB);

        query.PREPARE_UPDATE("cek_bordrolar", "bordro_id", "muh_fis_id ", "bordro_id = :bordro_id");
        query.SET_VALUE(":muh_fis_id" , P_BORDRO->muh_fis_id);
        query.SET_VALUE(":bordro_id"  , bordro_id);
        query.UPDATE();
    }

    return bordro_id;
}


/**************************************************************************************
                                    CEK_BORDRO_SATIRI_EKLE()
***************************************************************************************/

int CEK_BORDRO_SATIRI_EKLE ( CEK_BORDRO_STRUCT * P_BORDRO , CEK_BORDRO_SATIRI_STRUCT * P_BORDRO_SATIRI, bool p_cek_hareketi_ekle,
                             bool p_base_fis_satiri_eklenecek , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("cek_bordrolar",
                         "toplam_tutar , modul_id , program_id , "
                         "last_order_number , cari_hesap_id , bordro_tarihi ,"
                         "bordro_islem_turu , doviz_kuru , parite , "
                         "bordro_para_birimi_id , base_fis_id , bordro_id  ",
                         "bordro_id = :bordro_id");

    query.SET_VALUE(":bordro_id" , P_BORDRO_SATIRI->bordro_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    double toplam_tutar          = query.VALUE(0).toDouble();
    int    modul_id              = query.VALUE(1).toInt();
    int    program_id            = query.VALUE(2).toInt();
    int    last_order_number     = query.VALUE(3).toInt();
    int    cari_hesap_id         = query.VALUE(4).toInt();
    QDate  bordro_tarihi         = QDate::fromString(query.VALUE(5).toString() , "yyyy.MM.dd");
    int    bordro_islem_turu     = query.VALUE(6).toInt();
    double doviz_kuru            = query.VALUE(7).toDouble();
    double parite                = query.VALUE(8).toDouble();
    int    bordro_para_birimi_id = query.VALUE(9).toInt();
    int    base_fis_id           = query.VALUE(10).toInt();
    int    bordro_id             = query.VALUE(11).toInt();

    if ( P_BORDRO->modul_id NE modul_id OR P_BORDRO->program_id NE program_id
         OR P_BORDRO->base_fis_id NE base_fis_id ) {
        return 0;
    }

    if ( base_fis_id EQ 0 ) {
        base_fis_id = bordro_id;
    }

    double cari_para_birimi_tutari = 0.0;

    if ( cari_hesap_id > 0 ) {
        cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI(P_BORDRO->cari_para_birimi_id , bordro_para_birimi_id,
                                                              doviz_kuru , parite , P_BORDRO_SATIRI->cek_tutari);
    }

    double temel_para_birimi_tutari = 0.0;

    if ( bordro_para_birimi_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
        temel_para_birimi_tutari = P_BORDRO_SATIRI->cek_tutari;
    }
    else {
        temel_para_birimi_tutari = P_BORDRO_SATIRI->cek_tutari * doviz_kuru;
    }

    int order_number = 0;

    if ( P_BORDRO_SATIRI->order_number EQ 0 ) {
        order_number = last_order_number;
        order_number += 1024;
    }
    else {
        order_number = P_BORDRO_SATIRI->order_number;
    }

    int bordro_satiri_id = p_fis_satiri_id;

    if ( p_base_fis_satiri_eklenecek EQ true ) {

        query.PREPARE_INSERT ( "cek_bordro_satirlari", "bordro_satiri_id",
                               "bordro_id, cek_senet_id, aciklama, order_number,cari_para_birimi_tutari , "
                               "temel_para_birimi_tutari , alinan_cari_hesap_id , verilen_cari_hesap_id,"
                               "satir_tutari , program_id , modul_id , base_record_id ");

        query.SET_VALUE     ( ":bordro_id"               , P_BORDRO_SATIRI->bordro_id             );
        query.SET_VALUE     ( ":cek_senet_id"            , P_BORDRO_SATIRI->cek_senet_id          );
        query.SET_VALUE     ( ":aciklama"                , P_BORDRO_SATIRI->aciklama              );
        query.SET_VALUE     ( ":order_number"            , order_number                           );
        query.SET_VALUE     ( ":cari_para_birimi_tutari" , ROUND ( cari_para_birimi_tutari     )  );
        query.SET_VALUE     ( ":temel_para_birimi_tutari", ROUND ( temel_para_birimi_tutari    )  );
        query.SET_VALUE     ( ":alinan_cari_hesap_id"    , P_BORDRO_SATIRI->alinan_cari_hesap_id  );
        query.SET_VALUE     ( ":verilen_cari_hesap_id"   , P_BORDRO_SATIRI->verilen_cari_hesap_id );
        query.SET_VALUE     ( ":satir_tutari"            , P_BORDRO_SATIRI->cek_tutari            );
        query.SET_VALUE     ( ":program_id"              , P_BORDRO_SATIRI->program_id            );
        query.SET_VALUE     ( ":modul_id"                , P_BORDRO_SATIRI->modul_id              );
        query.SET_VALUE     ( ":base_record_id"          , P_BORDRO_SATIRI->base_record_id        );

        bordro_satiri_id = query.INSERT();

        toplam_tutar += P_BORDRO_SATIRI->cek_tutari;

        query.PREPARE_UPDATE("cek_bordrolar","bordro_id","toplam_tutar , last_order_number ","bordro_id = :bordro_id");
        query.SET_VALUE(":toplam_tutar"          , ROUND ( toplam_tutar ));
        query.SET_VALUE(":last_order_number"     , order_number);
        query.SET_VALUE(":bordro_id"             , P_BORDRO_SATIRI->bordro_id);

        query.UPDATE();

        //! E9 ONARDAN GELDIGIN DE EKLEMEMESI GEREKIR.
        //! CARI BURAYA GONDERDIGINDE KENDI SATIRINI EKLEMEDI ISE
        //! CEKIN HAREKETINDE DEGISIKLIK YAPMAYCAK
        if( p_cek_hareketi_ekle EQ true ) {
            int cekin_durumu = CEK_ISLEM_TURUNE_GORE_CEKIN_DURUMUNU_AL ( bordro_islem_turu );

            CEK_SENET_DURUMUNU_GUNCELLE ( P_BORDRO_SATIRI->cek_senet_id , cekin_durumu , USE_DB);

            CEK_SENET_HAREKETINI_KAYDET ( P_BORDRO_SATIRI->cek_senet_id, bordro_islem_turu, bordro_tarihi , USE_DB);
        }
    }

    P_BORDRO->toplam_tutar                    = toplam_tutar;
    P_BORDRO_SATIRI->temel_para_birimi_tutari = temel_para_birimi_tutari;
    P_BORDRO_SATIRI->cari_para_birimi_tutari  = cari_para_birimi_tutari;

    //! e9 0nardan gelmis ise iceride hareketler geriye alinacak
    CEK_SENET_ENT_FISLERINI_OLUSTUR ( P_BORDRO , P_BORDRO_SATIRI , bordro_satiri_id, !p_cek_hareketi_ekle,  USE_DB);

    return bordro_satiri_id;
}

/**************************************************************************************
                                    CEK_BORDRO_SATIRINI_SIL()
***************************************************************************************/

int CEK_BORDRO_SATIRINI_SIL ( CEK_BORDRO_STRUCT * P_BORDRO , int p_bordro_satiri_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("cek_bordrolar,cek_bordro_satirlari",
                         "toplam_tutar , cek_bordro_satirlari.modul_id , "
                         "cek_bordro_satirlari.program_id  , cek_senet_id,"
                         "cek_bordro_satirlari.bordro_id , bordro_turu , satir_tutari,base_fis_id  ",
                         "bordro_satiri_id = :bordro_satiri_id AND "
                         "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id");
    query.SET_VALUE(":bordro_satiri_id" , p_bordro_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    double toplam_tutar = query.VALUE(0).toDouble();
    int    modul_id     = query.VALUE(1).toInt();
    int    program_id   = query.VALUE(2).toInt();
    int    cek_senet_id = query.VALUE(3).toInt();
    int    bordro_id    = query.VALUE(4).toInt();
    int    bordro_turu  = query.VALUE(5).toInt();
    double satir_tutari = query.VALUE(6).toDouble();
    int    base_fis_id  = query.VALUE(7).toInt();


    if ( P_BORDRO->modul_id NE modul_id OR P_BORDRO->program_id NE program_id OR
         P_BORDRO->base_fis_id NE base_fis_id) {
        return bordro_id;
    }
    if ( base_fis_id EQ 0 ) {
        base_fis_id = bordro_id;
    }

    toplam_tutar = ROUND ( toplam_tutar - satir_tutari);

    CEK_SENET_ENT_FIS_SATIRLARINI_SIL( P_BORDRO , p_bordro_satiri_id);

    if ( bordro_turu EQ ENUM_CEK_SENET_ACILIS_BORDROSU ) {
        CEK_SENET_SIL ( cek_senet_id );
    }
    else {
        //! ONCEKI DURUMUNU DONULMESI GEREKIYOR.
        int cekin_onceki_durumu = CEK_SONDAN_BIR_ONCEKI_CEK_SENET_HAREKETINI_BUL( cek_senet_id );
        CEK_SON_CEK_SENET_HAREKETINI_SIL ( cek_senet_id );

        CEK_SENET_DURUMUNU_GUNCELLE ( cek_senet_id , cekin_onceki_durumu );
    }

    query.PREPARE_DELETE("cek_bordro_satirlari","bordro_satiri_id = :bordro_satiri_id");
    query.SET_VALUE(":bordro_satiri_id" , p_bordro_satiri_id);
    query.DELETE();


    if ( P_BORDRO->modul_id NE CEKSENET_MODULU ) {
        query.PREPARE_SELECT("cek_bordro_satirlari","bordro_satiri_id","bordro_id = :bordro_id");
        query.SET_VALUE(":bordro_id" , bordro_id);

        if ( query.SELECT() EQ 0 ) {

            query.PREPARE_DELETE("cek_bordrolar", "bordro_id = :bordro_id");
            query.SET_VALUE(":bordro_id" , bordro_id);
            query.DELETE();

            return 0;
        }
    }

    query.PREPARE_UPDATE("cek_bordrolar", "bordro_id", "toplam_tutar", "bordro_id = :bordro_id");
    query.SET_VALUE(":toplam_tutar" , toplam_tutar);
    query.SET_VALUE(":bordro_id"    , bordro_id);
    query.UPDATE();

    P_BORDRO->toplam_tutar = toplam_tutar;

    return bordro_id;

}

/**************************************************************************************
                                    CEK_BORDRO_SATIRINI_GUNCELLE()
***************************************************************************************/

void CEK_BORDRO_SATIRINI_GUNCELLE ( CEK_BORDRO_STRUCT *P_BORDRO , CEK_BORDRO_SATIRI_STRUCT *P_BORDRO_SATIRI,
                                  int p_bordro_satiri_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("cek_bordrolar , cek_bordro_satirlari,cek_cekler_senetler",
                         "toplam_tutar , cek_bordro_satirlari.cek_senet_id , "
                         "cek_bordro_satirlari.modul_id , cek_bordro_satirlari.program_id , "
                         "cari_hesap_id , bordro_tarihi, doviz_kuru , parite , bordro_para_birimi_id,"
                         "cek_senet_tutari,cek_bordro_satirlari.bordro_id, bordro_islem_turu , "
                         "cari_para_birimi_tutari , temel_para_birimi_tutari , "
                         "satir_tutari ,base_fis_id,muh_fis_id     ",
                         "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                         "bordro_satiri_id = :bordro_satiri_id AND "
                         "cek_cekler_senetler.cek_senet_id = cek_bordro_satirlari.cek_senet_id");

    query.SET_VALUE(":bordro_satiri_id" , p_bordro_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    double toplam_tutar              = query.VALUE(0).toDouble();
    int    cek_senet_id              = query.VALUE(1).toInt();
    int    modul_id                  = query.VALUE(2).toInt();
    int    program_id                = query.VALUE(3).toInt();
    int    cari_hesap_id             = query.VALUE(4).toInt();
    QDate  bordro_tarihi             = QDate::fromString(query.VALUE(5).toString() , "yyyy.MM.dd");
    double doviz_kuru                = query.VALUE(6).toDouble();
    double parite                    = query.VALUE(7).toDouble();
    int    bordro_para_birimi_id     = query.VALUE(8).toInt();
    double cek_senet_tutari          = query.VALUE(9).toDouble();
    int   bordro_id                  = query.VALUE(10).toInt();
    int   bordro_islem_turu          = query.VALUE(11).toInt();
    double cari_para_birimi_tutari   = query.VALUE(12).toDouble();
    double temel_para_birimi_tutari  = query.VALUE(13).toDouble();
    double satir_tutari              = query.VALUE(14).toDouble();
    int    base_fis_id               = query.VALUE(15).toInt();
    int    muh_fis_id                = query.VALUE(16).toInt();


    if ( P_BORDRO->program_id NE program_id OR P_BORDRO->modul_id NE modul_id OR
         P_BORDRO->base_fis_id NE base_fis_id ) {
        return;
    }

    if ( base_fis_id EQ 0 ) {
        base_fis_id = bordro_id;
    }

    bool entegrasyon_fis_satiri_guncellenecek = true;


    if ( P_BORDRO_SATIRI->cek_senet_id NE cek_senet_id OR cek_senet_tutari NE satir_tutari) {

        if ( P_BORDRO_SATIRI->cek_senet_id NE cek_senet_id ) {

            entegrasyon_fis_satiri_guncellenecek = false;

            CEK_SENET_ENT_FIS_SATIRLARINI_SIL(P_BORDRO , p_bordro_satiri_id);

            CEK_SON_CEK_SENET_HAREKETINI_SIL(cek_senet_id);

            int cekin_durumu = CEK_SON_CEK_SENET_HAREKETINI_BUL (cek_senet_id);

            CEK_SENET_DURUMUNU_GUNCELLE ( cek_senet_id , cekin_durumu );

            cekin_durumu = CEK_ISLEM_TURUNE_GORE_CEKIN_DURUMUNU_AL ( bordro_islem_turu );

            CEK_SENET_DURUMUNU_GUNCELLE ( P_BORDRO_SATIRI->cek_senet_id , cekin_durumu );

            CEK_SENET_HAREKETINI_KAYDET ( P_BORDRO_SATIRI->cek_senet_id , bordro_islem_turu, bordro_tarihi );

        }

        //Entegrasyon fis satiri silinir ve yeniden olusturulur.

        if ( cari_hesap_id > 0 ) {
            cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI(P_BORDRO->cari_para_birimi_id , bordro_para_birimi_id,
                                                                  doviz_kuru , parite , P_BORDRO_SATIRI->cek_tutari);
        }
        if ( bordro_para_birimi_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
            temel_para_birimi_tutari = P_BORDRO_SATIRI->cek_tutari;
        }
        else {
            temel_para_birimi_tutari = P_BORDRO_SATIRI->cek_tutari * doviz_kuru;
        }

        toplam_tutar = ROUND ( toplam_tutar - satir_tutari );

        toplam_tutar = ROUND ( toplam_tutar + P_BORDRO_SATIRI->cek_tutari );

        query.PREPARE_UPDATE("cek_bordrolar","bordro_id","toplam_tutar = :toplam_tutar","bordro_id = :bordro_id");
        query.SET_VALUE(":toplam_tutar" , toplam_tutar);
        query.SET_VALUE(":bordro_id"    , bordro_id);
        query.UPDATE();
    }


    query.PREPARE_UPDATE("cek_bordro_satirlari", "bordro_satiri_id" ,
                         "cek_senet_id             ,"
                         "aciklama                 ,"
                         "cari_para_birimi_tutari  ,"
                         "temel_para_birimi_tutari ,"
                         "alinan_cari_hesap_id     ,"
                         "verilen_cari_hesap_id    ,"
                         "satir_tutari              ",
                         "bordro_satiri_id = :bordro_satiri_id");

    query.SET_VALUE(":cek_senet_id"            , P_BORDRO_SATIRI->cek_senet_id );
    query.SET_VALUE(":aciklama"                , P_BORDRO_SATIRI->aciklama     );
    query.SET_VALUE(":cari_para_birimi_tutari" , ROUND ( cari_para_birimi_tutari ));
    query.SET_VALUE(":temel_para_birimi_tutari", ROUND ( temel_para_birimi_tutari ));
    query.SET_VALUE(":alinan_cari_hesap_id"    , P_BORDRO_SATIRI->alinan_cari_hesap_id );
    query.SET_VALUE(":verilen_cari_hesap_id"   , P_BORDRO_SATIRI->verilen_cari_hesap_id);
    query.SET_VALUE(":satir_tutari"            , P_BORDRO_SATIRI->cek_tutari             );
    query.SET_VALUE(":bordro_satiri_id"        , p_bordro_satiri_id            );

    query.UPDATE();

    P_BORDRO->toplam_tutar                    = toplam_tutar;
    P_BORDRO_SATIRI->temel_para_birimi_tutari = temel_para_birimi_tutari;
    P_BORDRO_SATIRI->cari_para_birimi_tutari  = cari_para_birimi_tutari;

    if ( entegrasyon_fis_satiri_guncellenecek EQ true ) {

        int cek_senet_entegrasyon_turu = CEK_GET_SENET_ENTEGRASYON_TURU ( cek_senet_id , P_BORDRO->modul_id);

        int base_record_id = P_BORDRO_SATIRI->base_record_id;

        if ( base_record_id EQ 0 ) {
            base_record_id = p_bordro_satiri_id;
        }

        if ( cek_senet_entegrasyon_turu EQ ENUM_MUHASEBE_ENTEGRASYONU ) {
            MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;
            MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

            MUHASEBE_FISI->program_id       = P_BORDRO->program_id;
            MUHASEBE_FISI->modul_id         = P_BORDRO->modul_id;
            MUHASEBE_FISI->base_fis_id      = base_fis_id;

            MUH_FIS_SATIRI_STRUCT * MUHASEBE_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

            QStringList muh_satirlar_list = MUH_GET_ENT_ROW_ID_LIST(P_BORDRO->program_id, P_BORDRO->modul_id, base_record_id);

            for ( int i = 0 ; i < muh_satirlar_list.size() ; i++ ) {

                int muh_fis_satiri_id = QVariant(muh_satirlar_list.at(i)).toInt();

                MUH_FIS_SATIRINI_OKU(MUHASEBE_FIS_SATIRI , muh_fis_satiri_id );

                MUHASEBE_FIS_SATIRI->aciklama         = P_BORDRO_SATIRI->aciklama;

                if ( MUHASEBE_FIS_SATIRI->borc_tutari > 0 ) {
                    MUHASEBE_FIS_SATIRI->borc_tutari   = temel_para_birimi_tutari;
                }
                else {
                    MUHASEBE_FIS_SATIRI->alacak_tutari = temel_para_birimi_tutari;
                }

                MUH_FIS_SATIRINI_GUNCELLE(MUHASEBE_FISI , MUHASEBE_FIS_SATIRI , muh_fis_satiri_id );
            }
        }
        else if ( cek_senet_entegrasyon_turu EQ ENUM_CARI_ENTEGRASYON ) {
            CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;
            CARI_CLEAR_FIS_STRUCT ( CARI_FIS);

            CARI_FIS->program_id       = P_BORDRO->program_id;
            CARI_FIS->modul_id         = P_BORDRO->modul_id;
            CARI_FIS->base_fis_id      = base_fis_id;
            CARI_FIS->muh_fis_id       = muh_fis_id;

            CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI = new CARI_FIS_SATIRI_STRUCT;

            QStringList car_satirlar_list = CARI_GET_ENT_ROW_ID_LIST( P_BORDRO->program_id, P_BORDRO->modul_id, base_record_id );

            for ( int i = 0 ; i < car_satirlar_list.size() ; i++ ) {

                int car_fis_satiri_id = QVariant(car_satirlar_list.at(i)).toInt();

                CARI_FIS_SATIRINI_OKU (CARI_FIS_SATIRI , car_fis_satiri_id);

                CARI_FIS_SATIRI->aciklama         = P_BORDRO_SATIRI->aciklama;

                CARI_FIS_SATIRINI_GUNCELLE(CARI_FIS , CARI_FIS_SATIRI , car_fis_satiri_id );
            }

            delete CARI_FIS;
            delete CARI_FIS_SATIRI;

            MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;
            MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

            MUHASEBE_FISI->program_id       = P_BORDRO->program_id;
            MUHASEBE_FISI->modul_id         = P_BORDRO->modul_id;
            MUHASEBE_FISI->base_fis_id      = base_fis_id;

            MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

            QStringList MUH_ROW_ID_LIST = MUH_GET_ENT_ROW_ID_LIST(P_BORDRO->program_id,
                                                                  P_BORDRO->modul_id , base_record_id);
            for ( int i = 0 ; i < MUH_ROW_ID_LIST.size() ; i++ ) {
                int muh_fis_satiri_id = QVariant(MUH_ROW_ID_LIST.at(i)).toInt();

                MUH_FIS_SATIRINI_OKU     ( MUH_FIS_SATIRI , muh_fis_satiri_id );

                MUH_FIS_SATIRI->aciklama         = P_BORDRO_SATIRI->aciklama;

                MUH_FIS_SATIRINI_GUNCELLE(MUHASEBE_FISI , MUH_FIS_SATIRI ,muh_fis_satiri_id);

            }
            delete MUHASEBE_FISI;
            delete MUH_FIS_SATIRI;
        }
        else if ( cek_senet_entegrasyon_turu EQ ENUM_BANKA_ENTEGRASYONU ) {
            BNK_HAREKET_FIS_STRUCT * BANKA_FISI = new BNK_HAREKET_FIS_STRUCT;

            BANKA_FISI->modul_id         = P_BORDRO->modul_id;
            BANKA_FISI->program_id       = P_BORDRO->program_id;
            BANKA_FISI->base_fis_id      = base_fis_id;
            BANKA_FISI->muh_fis_id       = muh_fis_id;

            BNK_HAREKET_FIS_SATIRI_STRUCT * BNK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

            QStringList BNK_ROW_ID_LIST = BNK_GET_ENT_ROW_ID_LIST( P_BORDRO->program_id, P_BORDRO->modul_id , base_record_id);

            for ( int i = 0 ; i < BNK_ROW_ID_LIST.size() ; i++ ) {

                int bnk_fis_satiri_id = QVariant(BNK_ROW_ID_LIST.at(i)).toInt();

                BNK_HAREKET_FIS_SATIR_BILGILERINI_OKU(BNK_FIS_SATIRI , bnk_fis_satiri_id);

                BNK_FIS_SATIRI->aciklama         = P_BORDRO_SATIRI->aciklama;

                BNK_HAREKET_FIS_SATIRINI_GUNCELLE(BANKA_FISI , BNK_FIS_SATIRI , bnk_fis_satiri_id);

            }
        }
    }
    else {
        CEK_SENET_ENT_FISLERINI_OLUSTUR(P_BORDRO, P_BORDRO_SATIRI, false, p_bordro_satiri_id);
    }
}

/**************************************************************************************
                                    CEK_BORDRO_SIL()
***************************************************************************************/

void CEK_BORDRO_SIL(CEK_BORDRO_STRUCT *P_BORDRO, int p_bordro_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("cek_bordrolar", "program_id , modul_id , base_fis_id , muh_fis_id ",
                         "bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , p_bordro_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    int program_id = query.VALUE(0).toInt();
    int modul_id   = query.VALUE(1).toInt();
    int base_fis_id= query.VALUE(2).toInt();
    int muh_fis_id = query.VALUE(3).toInt();

    if ( P_BORDRO->program_id NE program_id OR P_BORDRO->modul_id NE modul_id OR
         P_BORDRO->base_fis_id NE base_fis_id) {
         return;
    }

    if ( base_fis_id EQ 0 ) {
        base_fis_id = p_bordro_id;
    }

    query.PREPARE_SELECT("cek_bordro_satirlari",
                         "bordro_satiri_id,cek_senet_id,base_record_id ",
                         "bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , p_bordro_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    while ( query.NEXT() ) {

        int bordro_satiri_id  = query.VALUE(0).toInt();
        int cek_senet_id      = query.VALUE(1).toInt();
        int base_record_id    = query.VALUE(2).toInt();

        if ( P_BORDRO->modul_id EQ CEKSENET_MODULU ) {
            base_record_id = bordro_satiri_id;
        }

        int cek_senet_entegrasyon_turu = CEK_GET_SENET_ENTEGRASYON_TURU(cek_senet_id , P_BORDRO->modul_id);

        if ( cek_senet_entegrasyon_turu EQ ENUM_MUHASEBE_ENTEGRASYONU ) {

            if ( muh_fis_id > 0 ) {
                MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

                MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI , muh_fis_id);

                MUH_FIS_SIL(MUHASEBE_FISI , muh_fis_id);

                delete MUHASEBE_FISI;
            }
        }
        else if ( cek_senet_entegrasyon_turu EQ ENUM_CARI_ENTEGRASYON ) {

            int car_fis_id = MUH_GET_ENT_FIS_ID(P_BORDRO->program_id ,
                                                P_BORDRO->modul_id ,base_fis_id);

            if ( car_fis_id > 0 ) {

                CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;
                CARI_CLEAR_FIS_STRUCT(CARI_FIS);

                CARI_FIS->modul_id         = P_BORDRO->modul_id;
                CARI_FIS->program_id       = P_BORDRO->program_id;
                CARI_FIS->base_fis_id      = base_fis_id;
                CARI_FIS->muh_fis_id       = muh_fis_id;

                CARI_FISI_SIL( CARI_FIS , car_fis_id);

                delete CARI_FIS;

                MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;
                MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

                MUHASEBE_FISI->modul_id         = P_BORDRO->modul_id;
                MUHASEBE_FISI->program_id       = P_BORDRO->program_id;
                MUHASEBE_FISI->base_fis_id      = base_fis_id;

                QStringList MUH_ROW_ID_LIST = MUH_GET_ENT_ROW_ID_LIST(P_BORDRO->program_id,
                                                                      P_BORDRO->modul_id ,base_record_id );

                for ( int i = 0 ; i < MUH_ROW_ID_LIST.size() ; i++ ) {

                    int muh_fis_satiri_id = QVariant(MUH_ROW_ID_LIST.at(i)).toInt();

                    MUH_FIS_SATIRINI_SIL(MUHASEBE_FISI , muh_fis_satiri_id );
                }

                delete MUHASEBE_FISI;
            }
        }
        else if ( cek_senet_entegrasyon_turu EQ ENUM_BANKA_ENTEGRASYONU ) {
            int bnk_fis_id = BNK_GET_ENT_FIS_ID ( P_BORDRO->program_id , P_BORDRO->modul_id,
                                                base_fis_id);
            if ( bnk_fis_id > 0 ) {

                BNK_HAREKET_FIS_STRUCT * BANKA_FISI = new BNK_HAREKET_FIS_STRUCT;

                BNK_HAREKET_FIS_BILGILERINI_OKU(BANKA_FISI , bnk_fis_id);

                BNK_HAREKET_FISI_SIL(BANKA_FISI , bnk_fis_id);

                delete BANKA_FISI;
            }
        }

        CEK_BORDRO_SATIRINI_SIL ( P_BORDRO , bordro_satiri_id );
    }

    query.PREPARE_DELETE("cek_bordrolar","bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , p_bordro_id);
    query.DELETE();

}

/**************************************************************************************
                                    CEK_BORDRO_GUNCELLE()
***************************************************************************************/

void CEK_BORDRO_GUNCELLE ( CEK_BORDRO_STRUCT * P_BORDRO , int p_bordro_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("cek_bordrolar",
                         "cari_hesap_id , bordro_tarihi , hesap_no_id , "
                         "bordro_para_birimi_id , doviz_kuru , parite , base_fis_id, "
                         "program_id , modul_id,muh_fis_id",
                         "bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , p_bordro_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int cari_hesap_id         = query.VALUE(0).toInt();
    QDate bordro_tarihi       = QDate::fromString(query.VALUE(1).toString() , "yyyy.MM.dd");
    int hesap_no_id           = query.VALUE(2).toInt();
    int bordro_para_birimi_id = query.VALUE(3).toInt();
    double doviz_kuru         = query.VALUE(4).toDouble();
    double parite             = query.VALUE(5).toDouble();
    int base_fis_id           = query.VALUE(6).toInt();
    int program_id            = query.VALUE(7).toInt();
    int modul_id              = query.VALUE(8).toInt();
    int muh_fis_id            = query.VALUE(9).toInt();


    if (P_BORDRO->program_id NE program_id OR P_BORDRO->modul_id NE modul_id OR
        P_BORDRO->base_fis_id NE base_fis_id ) {
        return;
    }

    if ( base_fis_id EQ 0 ) {
        base_fis_id = p_bordro_id;
    }


    bool ent_fisleri_silinecek       = false;
    bool fis_satirlari_guncellenecek = false;

    if ( cari_hesap_id NE P_BORDRO->cari_hesap_id ) {
        ent_fisleri_silinecek = true;
        fis_satirlari_guncellenecek = true;
    }
    if ( bordro_tarihi.toString("yyyy.MM.dd") NE P_BORDRO->bordro_tarihi ) {
        ent_fisleri_silinecek = true;
    }
    if ( hesap_no_id NE P_BORDRO->hesap_no_id ) {
        ent_fisleri_silinecek = true;
    }
    if ( bordro_para_birimi_id NE P_BORDRO->bordro_para_birimi_id ) {
        ent_fisleri_silinecek       = true;
        fis_satirlari_guncellenecek = true;
    }
    if ( doviz_kuru NE P_BORDRO->doviz_kuru ) {
        ent_fisleri_silinecek       = true;
        fis_satirlari_guncellenecek = true;
    }
    if ( parite NE P_BORDRO->parite ) {
        ent_fisleri_silinecek       = true;
        fis_satirlari_guncellenecek = true;
    }

    if ( ent_fisleri_silinecek EQ true ) {

        if ( muh_fis_id > 0 ) {

            MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

            MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI,muh_fis_id);

            MUH_FIS_TUM_SATIRLARINI_SIL(MUHASEBE_FISI , muh_fis_id);
        }

        int entegrasyon_turu = CEK_GET_BORDRO_ISLEM_TURUNE_GORE_ENTEGRASYON_TURU(P_BORDRO->bordro_islem_turu,
                                                          P_BORDRO->modul_id);


        if ( entegrasyon_turu EQ ENUM_CARI_ENTEGRASYON ) {
            int car_fis_id = CARI_GET_ENT_FIS_ID(P_BORDRO->program_id ,
                                                 P_BORDRO->modul_id ,base_fis_id);
            if ( car_fis_id > 0 ) {

                CARI_FIS_STRUCT * CARI_FISI = new CARI_FIS_STRUCT;
                CARI_CLEAR_FIS_STRUCT(CARI_FISI);

                CARI_FISI->modul_id         = P_BORDRO->modul_id;
                CARI_FISI->program_id       = P_BORDRO->program_id;
                CARI_FISI->base_fis_id      = base_fis_id;
                CARI_FISI->muh_fis_id       = muh_fis_id;

                CARI_FISI_SIL ( CARI_FISI , car_fis_id);

                delete CARI_FISI;

            }
        }
        else if ( entegrasyon_turu EQ ENUM_BANKA_ENTEGRASYONU ) {

            int bnk_fis_id = BNK_GET_ENT_FIS_ID ( P_BORDRO->program_id , P_BORDRO->modul_id,
                                                base_fis_id );

            if ( bnk_fis_id > 0 ) {
                BNK_HAREKET_FIS_STRUCT * BANKA_FISI = new BNK_HAREKET_FIS_STRUCT;

                BNK_CLEAR_HAREKET_FIS_STRUCT(BANKA_FISI);

                BANKA_FISI->modul_id         = P_BORDRO->modul_id;
                BANKA_FISI->program_id       = P_BORDRO->program_id;
                BANKA_FISI->base_fis_id      = base_fis_id;
                BANKA_FISI->hesap_no_id      = P_BORDRO->hesap_no_id;
                BANKA_FISI->muh_fis_id       = muh_fis_id;

                BNK_HAREKET_FISI_SIL(BANKA_FISI , bnk_fis_id);

                delete BANKA_FISI;
            }
        }

    }



    query.PREPARE_UPDATE("cek_bordrolar","bordro_id",
                         "bordro_tarihi          ,"
                         "aciklama               ,"
                         "cari_hesap_id          ,"
                         "hesap_no_id            ,"
                         "bordro_para_birimi_id  ,"
                         "doviz_kuru             ,"
                         "parite                 ,"
                         "bordro_no               ",
                         "bordro_id       = :bordro_id");

    query.SET_VALUE(":bordro_tarihi"         , P_BORDRO->bordro_tarihi        );
    query.SET_VALUE(":aciklama"              , P_BORDRO->aciklama             );
    query.SET_VALUE(":cari_hesap_id"         , P_BORDRO->cari_hesap_id        );
    query.SET_VALUE(":hesap_no_id"           , P_BORDRO->hesap_no_id          );
    query.SET_VALUE(":bordro_para_birimi_id" , P_BORDRO->bordro_para_birimi_id);
    query.SET_VALUE(":doviz_kuru"            , ROUND ( P_BORDRO->doviz_kuru , -4) );
    query.SET_VALUE(":parite"                , ROUND ( P_BORDRO->parite     , -4) );
    query.SET_VALUE(":bordro_no"             , P_BORDRO->bordro_no            );
    query.SET_VALUE(":bordro_id"             , p_bordro_id                    );

    query.UPDATE();

    if ( fis_satirlari_guncellenecek EQ true) {

        SQL_QUERY s_query(DB);

        //cari para birimi tutari guncellenecek.

        query.PREPARE_SELECT("cek_bordro_satirlari,cek_cekler_senetler",
                             "bordro_satiri_id , satir_tutari",
                             "bordro_id = :bordro_id AND "
                             "cek_bordro_satirlari.cek_senet_id = cek_cekler_senetler.cek_senet_id");

        query.SET_VALUE(":bordro_id" , p_bordro_id);

        if ( query.SELECT() NE 0 ) {
            while ( query.NEXT() ) {

                int bordro_satiri_id    = query.VALUE(0).toInt();
                double satir_tutari     = query.VALUE(1).toDouble();

                double cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI(P_BORDRO->cari_para_birimi_id ,
                                                                             P_BORDRO->bordro_para_birimi_id,
                                                                      P_BORDRO->doviz_kuru , P_BORDRO->parite ,
                                                                      satir_tutari);
                double temel_para_birimi_tutari = 0.0;

                if ( P_BORDRO->bordro_para_birimi_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
                    temel_para_birimi_tutari = satir_tutari;
                }
                else {
                    temel_para_birimi_tutari = satir_tutari * P_BORDRO->doviz_kuru;
                }

                s_query.PREPARE_UPDATE("cek_bordro_satirlari ", "bordro_satiri_id",
                                       "cari_para_birimi_tutari     , "
                                       "temel_para_birimi_tutari     ",
                                       "bordro_satiri_id  = :bordro_satiri_id");
                s_query.SET_VALUE(":cari_para_birimi_tutari"  , ROUND ( cari_para_birimi_tutari  ));
                s_query.SET_VALUE(":temel_para_birimi_tutari" , ROUND ( temel_para_birimi_tutari ));
                s_query.SET_VALUE(":bordro_satiri_id"         , bordro_satiri_id );
                s_query.UPDATE();

            }
        }

    }

    if ( ent_fisleri_silinecek EQ true ) {
        query.PREPARE_SELECT("cek_bordro_satirlari", "bordro_satiri_id", "bordro_id = :bordro_id");
        query.SET_VALUE(":bordro_id" , p_bordro_id);

        if ( query.SELECT() EQ 0 ) {
            return;
        }

        while ( query.NEXT() ) {
            int bordro_satiri_id = query.VALUE(0).toInt();

            CEK_BORDRO_SATIRI_STRUCT * BORDRO_SATIRI = new CEK_BORDRO_SATIRI_STRUCT;

            CEK_BORDRO_SATIRINI_OKU(BORDRO_SATIRI , bordro_satiri_id);

            CEK_SENET_ENT_FISLERINI_OLUSTUR(P_BORDRO, BORDRO_SATIRI, false, bordro_satiri_id );
        }
    }
    else {

        QString aciklama;

        if ( P_BORDRO->modul_id EQ CEKSENET_MODULU ) {
            aciklama= QObject::tr("Fiş ÇEK SENET modülü tarafından üretilmiştir.Bordro No : %1\n").arg(QVariant(P_BORDRO->bordro_no).toString());
            aciklama.append(P_BORDRO->aciklama);
        }
        else {
            aciklama = P_BORDRO->aciklama;
        }

        int car_fis_id = CARI_GET_ENT_FIS_ID(P_BORDRO->program_id , P_BORDRO->modul_id,
                                             base_fis_id );
        if ( car_fis_id > 0 ) {
            CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;
            CARI_FIS_BILGILERINI_OKU(CARI_FIS , car_fis_id);

            CARI_FIS->aciklama         = aciklama;
            CARI_FIS->fis_tarihi       = P_BORDRO->bordro_tarihi;
            CARI_FIS->muh_fis_id       = muh_fis_id;

            CARI_FISI_GUNCELLE(CARI_FIS , car_fis_id);

            delete CARI_FIS;
        }

        if ( muh_fis_id > 0 ) {
            MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

            MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI , muh_fis_id);

            MUHASEBE_FISI->fis_tarihi       = P_BORDRO->bordro_tarihi;
            MUHASEBE_FISI->aciklama         = aciklama;

            MUH_FISI_GUNCELLE(MUHASEBE_FISI , muh_fis_id);
            delete MUHASEBE_FISI;
        }

        int bnk_fis_id = BNK_GET_ENT_FIS_ID(P_BORDRO->program_id , P_BORDRO->modul_id,
                                            base_fis_id);

        if ( bnk_fis_id > 0 ) {
            BNK_HAREKET_FIS_STRUCT * BANKA_FISI = new BNK_HAREKET_FIS_STRUCT;

            BNK_HAREKET_FIS_BILGILERINI_OKU(BANKA_FISI , bnk_fis_id);

            BANKA_FISI->aciklama         = aciklama;
            BANKA_FISI->fis_tarihi       = P_BORDRO->bordro_tarihi;

            BNK_HAREKET_FISI_GUNCELLE(BANKA_FISI , bnk_fis_id);
        }
    }
}

/**************************************************************************************
                                    CEK_BORDRO_BILGILERI_OKU()
***************************************************************************************/

void CEK_BORDRO_BILGILERI_OKU(CEK_BORDRO_STRUCT *P_BORDRO, int p_bordro_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("cek_bordrolar",
                         "bordro_tarihi , bordro_no , bordro_islem_turu , aciklama ,"
                         "cari_hesap_id , hesap_no_id , toplam_tutar , modul_id , bordro_turu , "
                         "musteri_cek_senedi_mi , cek_bordrosu_mu , program_id , "
                         "bordro_para_birimi_id , doviz_kuru , parite , base_fis_id, "
                         "muh_fis_id ",
                         "bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , p_bordro_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    P_BORDRO->bordro_tarihi         = query.VALUE(0).toString();
    P_BORDRO->bordro_no             = query.VALUE(1).toInt();
    P_BORDRO->bordro_islem_turu     = query.VALUE(2).toInt();
    P_BORDRO->aciklama              = query.VALUE(3).toString();
    P_BORDRO->cari_hesap_id         = query.VALUE(4).toInt();
    P_BORDRO->hesap_no_id           = query.VALUE(5).toInt();
    P_BORDRO->toplam_tutar          = query.VALUE(6).toDouble();
    P_BORDRO->modul_id              = query.VALUE(7).toInt();
    P_BORDRO->bordro_turu           = query.VALUE(8).toInt();
    P_BORDRO->musteri_cek_senedi_mi = query.VALUE(9).toInt();
    P_BORDRO->cek_bordrosu_mu       = query.VALUE(10).toInt();
    P_BORDRO->program_id            = query.VALUE(11).toInt();
    P_BORDRO->bordro_para_birimi_id = query.VALUE(12).toInt();
    P_BORDRO->doviz_kuru            = query.VALUE(13).toDouble();
    P_BORDRO->parite                = query.VALUE(14).toDouble();
    P_BORDRO->base_fis_id           = query.VALUE(15).toInt();
    P_BORDRO->muh_fis_id            = query.VALUE(16).toInt();
}
/**************************************************************************************
              CEK_BORDRO_SATIRINI_OKU()
***************************************************************************************/

void CEK_BORDRO_SATIRINI_OKU ( CEK_BORDRO_SATIRI_STRUCT *P_BORDRO_SATIRI,
                             int p_bordro_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("cek_bordro_satirlari",
                        "bordro_id , cek_senet_id , aciklama , order_number ,"
                        "alinan_cari_hesap_id , verilen_cari_hesap_id , temel_para_birimi_tutari , "
                        "cari_para_birimi_tutari , satir_tutari , base_record_id ,"
                        "modul_id , program_id",
                        "bordro_satiri_id = :bordro_satiri_id");

    query.SET_VALUE(":bordro_satiri_id" ,p_bordro_satiri_id );

    if ( query.SELECT() EQ 0 ) {
       return;
    }
    query.NEXT();

    P_BORDRO_SATIRI->bordro_id                = query.VALUE(0).toInt();
    P_BORDRO_SATIRI->cek_senet_id             = query.VALUE(1).toInt();
    P_BORDRO_SATIRI->aciklama                 = query.VALUE(2).toString();
    P_BORDRO_SATIRI->order_number             = query.VALUE(3).toInt();
    P_BORDRO_SATIRI->alinan_cari_hesap_id     = query.VALUE(4).toInt();
    P_BORDRO_SATIRI->verilen_cari_hesap_id    = query.VALUE(5).toInt();
    P_BORDRO_SATIRI->temel_para_birimi_tutari = query.VALUE(6).toDouble();
    P_BORDRO_SATIRI->cari_para_birimi_tutari  = query.VALUE(7).toDouble();
    P_BORDRO_SATIRI->cek_tutari               = query.VALUE(8).toDouble();
    P_BORDRO_SATIRI->base_record_id           = query.VALUE(9).toInt();
    P_BORDRO_SATIRI->modul_id                 = query.VALUE(10).toInt();
    P_BORDRO_SATIRI->program_id               = query.VALUE(11).toInt();
}

/**************************************************************************************
              CEK_SENET_ENT_FISLERINI_OLUSTUR()
***************************************************************************************/


void CEK_SENET_ENT_FISLERINI_OLUSTUR( CEK_BORDRO_STRUCT *P_BORDRO, CEK_BORDRO_SATIRI_STRUCT *P_BORDRO_SATIRI,
                                           int p_bordro_satiri_id, bool p_e9_onar_cagirdi, ADAK_SQL * P_GELEN_DB )

{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    int borclu_hesap_id   = 0;
    int alacakli_hesap_id = 0;

    if ( CEK_SENET_ENT_ICIN_MUH_HESAPLARINI_BELIRLE ( p_bordro_satiri_id, &borclu_hesap_id, &alacakli_hesap_id, p_e9_onar_cagirdi ,USE_DB ) EQ false ) {
        return;
    }

    SQL_QUERY sql_query ( USE_DB );

    sql_query.PREPARE_SELECT("cek_cekler_senetler","cek_senet_turu, musteri_ceki_mi",
                             "cek_senet_id = :cek_senet_id");

    sql_query.SET_VALUE(":cek_senet_id", P_BORDRO_SATIRI->cek_senet_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    int cek_senet_turu  = sql_query.VALUE(0).toInt();
    int musteri_ceki_mi = sql_query.VALUE(1).toInt();
    int base_fis_id     = P_BORDRO->base_fis_id;
    int base_record_id  = P_BORDRO_SATIRI->base_record_id;

    if ( P_BORDRO->modul_id EQ CEKSENET_MODULU ) {
        base_fis_id    = P_BORDRO_SATIRI->bordro_id;
        base_record_id = p_bordro_satiri_id;
    }

    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT( MUHASEBE_FISI );

    MUH_FIS_BILGILERINI_OKU( MUHASEBE_FISI, P_BORDRO->muh_fis_id, USE_DB);

    MUHASEBE_FISI->aciklama = P_BORDRO->aciklama;
    MUHASEBE_FISI->fis_tarihi = P_BORDRO->bordro_tarihi;

    MUH_FISI_GUNCELLE(MUHASEBE_FISI , P_BORDRO->muh_fis_id ,USE_DB);

    int cek_senet_entegrasyon_turu = CEK_GET_SENET_ENTEGRASYON_TURU( P_BORDRO_SATIRI->cek_senet_id, P_BORDRO->modul_id, p_e9_onar_cagirdi );

    if ( cek_senet_entegrasyon_turu EQ ENUM_MUHASEBE_ENTEGRASYONU ) {

        MUH_FIS_SATIRI_STRUCT * MUHASEBE_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

        MUH_CLEAR_FIS_SATIRI_STRUCT  ( MUHASEBE_FIS_SATIRI );

        MUHASEBE_FIS_SATIRI->hesap_id             = borclu_hesap_id;
        MUHASEBE_FIS_SATIRI->borc_tutari          = P_BORDRO_SATIRI->temel_para_birimi_tutari;
        MUHASEBE_FIS_SATIRI->fis_id               = P_BORDRO->muh_fis_id;
        MUHASEBE_FIS_SATIRI->aciklama             = P_BORDRO_SATIRI->aciklama;
        MUHASEBE_FIS_SATIRI->modul_id             = P_BORDRO_SATIRI->modul_id;
        MUHASEBE_FIS_SATIRI->program_id           = P_BORDRO_SATIRI->program_id;
        MUHASEBE_FIS_SATIRI->base_record_id       = base_record_id;

        MUH_FIS_SATIRI_EKLE ( MUHASEBE_FISI , MUHASEBE_FIS_SATIRI ,USE_DB);

        MUHASEBE_FIS_SATIRI->hesap_id         = alacakli_hesap_id;
        MUHASEBE_FIS_SATIRI->borc_tutari      = 0;
        MUHASEBE_FIS_SATIRI->alacak_tutari    = P_BORDRO_SATIRI->temel_para_birimi_tutari;
        MUHASEBE_FIS_SATIRI->order_number     = 0;

        MUH_FIS_SATIRI_EKLE ( MUHASEBE_FISI , MUHASEBE_FIS_SATIRI ,USE_DB);

        delete MUHASEBE_FISI;
        delete MUHASEBE_FIS_SATIRI;

    }
    else if ( cek_senet_entegrasyon_turu EQ ENUM_CARI_ENTEGRASYON ) {
        //! CARI MODULU GELDI ISE TEKRAR KENDINI CAGIRMAMALI
        if ( P_BORDRO->modul_id NE CARI_MODULU ) {
            CARI_FIS_STRUCT * CARI_FIS               = new CARI_FIS_STRUCT;
            CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI = new CARI_FIS_SATIRI_STRUCT;

            CARI_CLEAR_FIS_STRUCT        ( CARI_FIS );
            CARI_CLEAR_FIS_SATIRI_STRUCT ( CARI_FIS_SATIRI );

            int car_fis_id = CARI_GET_ENT_FIS_ID ( P_BORDRO->program_id, P_BORDRO->modul_id, base_fis_id, USE_DB );

            if ( car_fis_id EQ 0 ) {

                CARI_FIS->program_id            = P_BORDRO->program_id;
                CARI_FIS->modul_id              = P_BORDRO->modul_id;
                CARI_FIS->fis_tarihi            = P_BORDRO->bordro_tarihi;
                CARI_FIS->base_fis_id           = base_fis_id;
                CARI_FIS->muh_fis_id            = P_BORDRO->muh_fis_id;
                CARI_FIS->cari_hesap_id         = P_BORDRO->cari_hesap_id;
                CARI_FIS->cari_para_birimi_id   = P_BORDRO->cari_para_birimi_id;
                CARI_FIS->makbuz_para_birimi_id = P_BORDRO->bordro_para_birimi_id;
                CARI_FIS->doviz_kuru            = P_BORDRO->doviz_kuru;
                CARI_FIS->parite                = P_BORDRO->parite;

                if ( P_BORDRO->bordro_islem_turu EQ ENUM_CEK_IADE_ALINDI_SATICIDAN ) {
                    CARI_FIS->fis_turu = ENUM_CARI_HAREKET_FISI;
                }
                else {
                    CARI_FIS->fis_turu = ENUM_CARI_ODEME_FISI;
                }

                QString aciklama = QObject::tr("Fiş ÇEK SENET modülü tarafından üretilmiştir.Bordro No : %1\n").arg(QVariant( P_BORDRO->bordro_no ).toString());
                aciklama.append(P_BORDRO->aciklama);

                CARI_FIS->aciklama   = aciklama;

                car_fis_id = CARI_FIS_EKLE ( CARI_FIS ,USE_DB);
            }
            else {
                CARI_FIS_BILGILERINI_OKU ( CARI_FIS ,car_fis_id , USE_DB);
            }

            double borc_tutari   = 0;
            double alacak_tutari = 0;
            int    muh_hesap_id  = 0;

            CARI_FIS_SATIRI->cari_hesap_id = P_BORDRO->cari_hesap_id;
            CARI_FIS_SATIRI->doviz_id      = P_BORDRO->bordro_para_birimi_id;
            CARI_FIS_SATIRI->doviz_kuru    = P_BORDRO->doviz_kuru;
            CARI_FIS_SATIRI->parite        = P_BORDRO->parite;

            if ( P_BORDRO->bordro_islem_turu EQ ENUM_CEK_IADE_ALINDI_SATICIDAN ) {
                alacak_tutari = P_BORDRO_SATIRI->cek_tutari;
                muh_hesap_id  = borclu_hesap_id;
            }
            else {
                borc_tutari  = P_BORDRO_SATIRI->cek_tutari;
                muh_hesap_id = alacakli_hesap_id;
            }

            if ( cek_senet_turu EQ ENUM_CEK ) {
                if (  musteri_ceki_mi EQ EVET ) {
                    CARI_FIS_SATIRI->islem_turu = ENUM_MUSTERI_CEKI;
                }
                else {
                    CARI_FIS_SATIRI->islem_turu = ENUM_KENDI_CEKIMIZ;
                }
            }
            else {
                if ( musteri_ceki_mi EQ EVET ) {
                    CARI_FIS_SATIRI->islem_turu = ENUM_MUSTERI_SENEDI;
                }
                else {
                    CARI_FIS_SATIRI->islem_turu = ENUM_KENDI_SENEDIMIZ;
                }
            }

            CARI_FIS_SATIRI->aciklama            = P_BORDRO_SATIRI->aciklama;
            CARI_FIS_SATIRI->borc_tutari         = borc_tutari;
            CARI_FIS_SATIRI->alacak_tutari       = alacak_tutari;
            CARI_FIS_SATIRI->cek_senet_id        = P_BORDRO_SATIRI->cek_senet_id;
            CARI_FIS_SATIRI->fis_id              = car_fis_id;
            CARI_FIS_SATIRI->program_id          = P_BORDRO_SATIRI->program_id;
            CARI_FIS_SATIRI->modul_id            = P_BORDRO_SATIRI->modul_id;
            CARI_FIS_SATIRI->base_record_id      = base_record_id;

            CARI_FIS_SATIRI_EKLE ( CARI_FIS, CARI_FIS_SATIRI, USE_DB);

            //! CEK SENET KENDI MUAHSEBE SATIRI EKLIYOR
            if (P_BORDRO->muh_fis_id > 0 ) {

                MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

                MUH_CLEAR_FIS_SATIRI_STRUCT (MUH_FIS_SATIRI);

                int last_order_number = MUH_GET_FIS_LAST_ORDER_NUMBER( P_BORDRO->muh_fis_id );

                MUH_FIS_SATIRI->program_id           = P_BORDRO_SATIRI->program_id;
                MUH_FIS_SATIRI->modul_id             = P_BORDRO_SATIRI->modul_id;
                MUH_FIS_SATIRI->aciklama             = P_BORDRO_SATIRI->aciklama;
                MUH_FIS_SATIRI->borc_tutari          = alacak_tutari;
                MUH_FIS_SATIRI->alacak_tutari        = borc_tutari;
                MUH_FIS_SATIRI->fis_id               = P_BORDRO->muh_fis_id;
                MUH_FIS_SATIRI->hesap_id             = muh_hesap_id;
                MUH_FIS_SATIRI->order_number         = 0;
                MUH_FIS_SATIRI->base_record_id       = base_record_id;

                if ( P_BORDRO->bordro_islem_turu EQ ENUM_CEK_IADE_ALINDI_SATICIDAN ) {
                    MUH_FIS_SATIRI->order_number = last_order_number -1;
                }
                else {
                    MUH_FIS_SATIRI->order_number = last_order_number + 1;
                }

                MUH_FIS_SATIRI_EKLE(MUHASEBE_FISI , MUH_FIS_SATIRI , USE_DB);

                delete MUH_FIS_SATIRI;
                delete MUHASEBE_FISI;
            }

            delete CARI_FIS;
            delete CARI_FIS_SATIRI;
        }
    }
    else if ( cek_senet_entegrasyon_turu EQ ENUM_BANKA_ENTEGRASYONU ) {
        BNK_HAREKET_FIS_STRUCT * BANKA_FISI = new BNK_HAREKET_FIS_STRUCT;
        BNK_CLEAR_HAREKET_FIS_STRUCT(BANKA_FISI);

        int bnk_fis_id = BNK_GET_ENT_FIS_ID(P_BORDRO->program_id, P_BORDRO->modul_id, base_fis_id, USE_DB);

        if ( bnk_fis_id EQ 0 ) {

            BANKA_FISI->base_fis_id                   = base_fis_id;
            BANKA_FISI->modul_id                      = P_BORDRO->modul_id;
            BANKA_FISI->program_id                    = P_BORDRO->program_id;
            BANKA_FISI->fis_tarihi                    = P_BORDRO->bordro_tarihi;
            BANKA_FISI->fis_turu                      = ENUM_BANKA_HAREKET_FISI;
            BANKA_FISI->hesap_no_id                   = P_BORDRO->hesap_no_id;
            BANKA_FISI->muh_fis_id                    = P_BORDRO->muh_fis_id;
            BANKA_FISI->odm_emr_hesabi_etkilenecek_mi = 0; // etkilenmeyecek zaten kendisi ( cekler ve odeme emirleri hesabi)
            BANKA_FISI->para_birim_id                 = P_BORDRO->bordro_para_birimi_id;

            QString aciklama = QObject::tr("Fiş ÇEK SENET modülü tarafından üretilmiştir.Bordro No : %1\n").arg(QVariant(P_BORDRO->bordro_no).toString());
            aciklama.append(P_BORDRO->aciklama);

            BANKA_FISI->aciklama = aciklama;

            bnk_fis_id = BNK_HAREKET_FIS_EKLE(BANKA_FISI , 0,true,USE_DB);
        }
        else {
            BNK_HAREKET_FIS_BILGILERINI_OKU(BANKA_FISI , bnk_fis_id , USE_DB);
        }

        BNK_HAREKET_FIS_SATIRI_STRUCT * BNK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;
        BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT ( BNK_FIS_SATIRI );

        BNK_FIS_SATIRI->modul_id               = P_BORDRO->modul_id;
        BNK_FIS_SATIRI->program_id             = P_BORDRO->program_id;
        BNK_FIS_SATIRI->fis_id                 = bnk_fis_id;
        BNK_FIS_SATIRI->aciklama               = P_BORDRO_SATIRI->aciklama;
        BNK_FIS_SATIRI->base_record_id         = base_record_id;
        BNK_FIS_SATIRI->doviz_kuru             = P_BORDRO->doviz_kuru;
        BNK_FIS_SATIRI->parite                 = P_BORDRO->parite;
        BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_BORDRO->hesap_no_id;
        BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;

        if (P_BORDRO->bordro_islem_turu EQ ENUM_CEK_TAHSIL_EDILDI_HESABA ) {
            BNK_FIS_SATIRI->hesaba_giren   = P_BORDRO_SATIRI->cek_tutari;
            BNK_FIS_SATIRI->karsi_hesap_id = alacakli_hesap_id;
        }
        else {//Odendi hesaptan ise
            BNK_FIS_SATIRI->hesaptan_cikan = P_BORDRO_SATIRI->cek_tutari;
            BNK_FIS_SATIRI->karsi_hesap_id = borclu_hesap_id;
        }

        BNK_FIS_SATIRI->karsi_hesap_turu = ENUM_BNK_MUHASEBE_HESABI;

        BNK_HAREKET_FIS_SATIRI_EKLE( BANKA_FISI, BNK_FIS_SATIRI, 0, true, USE_DB );
    }
}

/**************************************************************************************
              CEK_SENET_ENT_FIS_SATIRLARINI_SIL();
***************************************************************************************/

void CEK_SENET_ENT_FIS_SATIRLARINI_SIL( CEK_BORDRO_STRUCT *P_BORDRO, int p_bordro_satiri_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("cek_bordro_satirlari,cek_bordrolar",
                         "cek_bordrolar.bordro_id,cek_senet_id , base_record_id,muh_fis_id ",
                         "bordro_satiri_id = :bordro_satiri_id "
                         "AND cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id");
    query.SET_VALUE(":bordro_satiri_id" , p_bordro_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    int bordro_id         = query.VALUE(0).toInt();
    int cek_senet_id      = query.VALUE(1).toInt();
    int base_record_id    = query.VALUE(2).toInt();
    int base_fis_id       = P_BORDRO->base_fis_id;
    int muh_fis_id        = query.VALUE(3).toInt();

    if ( P_BORDRO->modul_id EQ CEKSENET_MODULU ) {
        base_fis_id    = bordro_id;
        base_record_id = p_bordro_satiri_id;
    }

    int cek_senet_entegrasyon_turu = CEK_GET_SENET_ENTEGRASYON_TURU( cek_senet_id , P_BORDRO->modul_id);

    if ( cek_senet_entegrasyon_turu EQ ENUM_MUHASEBE_ENTEGRASYONU ) {

        MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;
        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        MUHASEBE_FISI->modul_id             = P_BORDRO->modul_id;
        MUHASEBE_FISI->program_id           = P_BORDRO->program_id;
        MUHASEBE_FISI->base_fis_id          = base_fis_id;

        MUH_ENTEGRASYON_FIS_SATIRLARINI_SIL( MUHASEBE_FISI, muh_fis_id, base_record_id );

        delete MUHASEBE_FISI;
    }
    else if ( cek_senet_entegrasyon_turu EQ ENUM_CARI_ENTEGRASYON ) {
        CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;
        CARI_CLEAR_FIS_STRUCT(CARI_FIS);

        CARI_FIS->modul_id              = P_BORDRO->modul_id;
        CARI_FIS->program_id            = P_BORDRO->program_id;
        CARI_FIS->base_fis_id           = base_fis_id;
        CARI_FIS->muh_fis_id            = muh_fis_id;

        QStringList CARI_ROW_ID_LIST = CARI_GET_ENT_ROW_ID_LIST(P_BORDRO->program_id, P_BORDRO->modul_id, base_record_id);

        for ( int i = 0 ; i < CARI_ROW_ID_LIST.size() ; i++ ) {
            int car_fis_satiri_id = QVariant(CARI_ROW_ID_LIST.at(i)).toInt();
            CARI_FIS_SATIRI_SIL(CARI_FIS , car_fis_satiri_id);
        }

        MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;
        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        MUHASEBE_FISI->modul_id             = P_BORDRO->modul_id;
        MUHASEBE_FISI->program_id           = P_BORDRO->program_id;
        MUHASEBE_FISI->base_fis_id          = base_fis_id;

        QStringList MUH_ROW_ID_LIST = MUH_GET_ENT_ROW_ID_LIST( P_BORDRO->program_id, P_BORDRO->modul_id, base_record_id);

        for ( int i = 0 ; i < MUH_ROW_ID_LIST.size() ; i++ ) {
            int muh_fis_satiri_id = QVariant ( MUH_ROW_ID_LIST.at(i)).toInt();
            MUH_FIS_SATIRINI_SIL ( MUHASEBE_FISI , muh_fis_satiri_id);
        }
        delete CARI_FIS;
    }
    else if ( cek_senet_entegrasyon_turu EQ ENUM_BANKA_ENTEGRASYONU ) {

        BNK_HAREKET_FIS_STRUCT * BANKA_FISI = new BNK_HAREKET_FIS_STRUCT;

        int bnk_fis_id = BNK_GET_ENT_FIS_ID(P_BORDRO->program_id , P_BORDRO->modul_id ,
                                            base_fis_id , P_BORDRO->hesap_no_id);
        BNK_HAREKET_FIS_BILGILERINI_OKU(BANKA_FISI , bnk_fis_id );

        BANKA_FISI->modul_id            = P_BORDRO->modul_id;
        BANKA_FISI->program_id          = P_BORDRO->program_id;
        BANKA_FISI->base_fis_id         = base_fis_id;
        BANKA_FISI->muh_fis_id          = muh_fis_id;

        QStringList BNK_ROW_ID_LIST = BNK_GET_ENT_ROW_ID_LIST(P_BORDRO->program_id, P_BORDRO->modul_id , base_record_id );

        for ( int i = 0 ; i < BNK_ROW_ID_LIST.size() ; i++ ) {

            int bnk_fis_satiri_id = QVariant(BNK_ROW_ID_LIST.at(i)).toInt();

            BNK_HAREKET_FIS_SATIRI_SIL(BANKA_FISI , bnk_fis_satiri_id );
        }
    }
}


/**************************************************************************************
                                    CEK_GET_SENET_ENTEGRASYON_TURU()
***************************************************************************************/

int CEK_GET_SENET_ENTEGRASYON_TURU ( int cek_senet_id , int modul_id , bool p_e9_onar_cagirdi)
{
    int cek_senet_son_durumu    = -1;
    int cek_senet_onceki_durumu = -1;

    CEK_GET_CEK_SENET_SON_DURUMLARI( cek_senet_id, &cek_senet_son_durumu, &cek_senet_onceki_durumu );

    //! ONAR ICIN EKLENMISTIR
    if( p_e9_onar_cagirdi EQ true ) {
        if( modul_id EQ CARI_MODULU ) {
            cek_senet_son_durumu    = ENUM_PORTFOYDE;
            cek_senet_onceki_durumu = ENUM_PORTFOYDE;
        }
        else {
            CEK_ONAR_ICIN_CEK_DURUMLARINI_BUL( cek_senet_id, &cek_senet_son_durumu, &cek_senet_onceki_durumu );
        }
    }
    //! END ONAR

    switch ( cek_senet_onceki_durumu ) {

        case ENUM_PORTFOYDE :

            switch ( cek_senet_son_durumu ) {

                case ENUM_BNK_TAHSILAT_VERILDI  :
                case ENUM_BNK_TEMINAT_VERILDI   :
                case ENUM_KARSILIKSIZ           :
                case ENUM_PROTESTOLU            :
                case ENUM_PORTFOYDE             :

                    return ENUM_MUHASEBE_ENTEGRASYONU;

                case ENUM_SATICIYA_VERILDI         :
                    if ( modul_id EQ CEKSENET_MODULU ) {
                        return ENUM_CARI_ENTEGRASYON;
                    }
                    return ENUM_MUHASEBE_ENTEGRASYONU;

                case ENUM_MUS_IADE_EDILDI     :

                    return ENUM_CARI_ENTEGRASYON;
                case ENUM_TAHSIL_EDILDI_NKT   :

                    return ENUM_MUHASEBE_ENTEGRASYONU;

                default :
                    break;
            }
            break;

        case ENUM_BNK_TEMINAT_VERILDI :

            switch ( cek_senet_son_durumu ) {

                case ENUM_PORTFOYDE :
                    return ENUM_MUHASEBE_ENTEGRASYONU;

                case ENUM_TAHSIL_EDILDI_HESABA :
                    return ENUM_BANKA_ENTEGRASYONU;
                default        :
                    break;
            }
            break;

        case ENUM_BNK_TAHSILAT_VERILDI :

            switch ( cek_senet_son_durumu ) {

                case ENUM_TAHSIL_EDILDI_HESABA :
                    return ENUM_BANKA_ENTEGRASYONU;

                case ENUM_PORTFOYDE :
                    return ENUM_MUHASEBE_ENTEGRASYONU;
                default        :
                    break;
            }
            break;

        case ENUM_SATICIYA_VERILDI :

            switch ( cek_senet_son_durumu ) {

                case ENUM_PORTFOYDE :
                    return ENUM_CARI_ENTEGRASYON;

                case ENUM_ODENDI_NKT :
                   return ENUM_MUHASEBE_ENTEGRASYONU;

                case ENUM_ODENDI_HESAPTAN :
                   return ENUM_BANKA_ENTEGRASYONU;

                default        :
                    break;
            }

            break;

        case ENUM_KARSILIKSIZ :
        case ENUM_PROTESTOLU  :

            switch ( cek_senet_son_durumu ) {

                case ENUM_TAHSIL_EDILDI_NKT :
                case ENUM_SUPHELI_ALACAK    :
                    return ENUM_MUHASEBE_ENTEGRASYONU;

                case ENUM_MUS_IADE_EDILDI :
                    return ENUM_CARI_ENTEGRASYON;
                default :
                    break;
            }

            break;

        case ENUM_SUPHELI_ALACAK :

            switch ( cek_senet_son_durumu ) {

                case ENUM_TAHSIL_EDILDI_NKT :
                case ENUM_TAHSIL_EDILEMEYEN :

                    return ENUM_MUHASEBE_ENTEGRASYONU;

                case ENUM_MUS_IADE_EDILDI :
                    return ENUM_CARI_ENTEGRASYON;

                default :
                    break;
            }

            break;

        case ENUM_TAHSIL_EDILDI_NKT          :
        case ENUM_TAHSIL_EDILDI_HESABA       :
        case ENUM_MUS_IADE_EDILDI            :
        case ENUM_ODENDI_NKT                 :
        case ENUM_ODENDI_HESAPTAN            :
        case ENUM_TAHSIL_EDILEMEYEN          :
        default                              :
            if( p_e9_onar_cagirdi EQ true ) {
                return ENUM_MUHASEBE_ENTEGRASYONU;
            }
            break;
    }
    return -1;
}


/**************************************************************************************
                  CEK_SENET_ENT_ICIN_MUH_HESAPLARINI_BELIRLE()
***************************************************************************************/

bool CEK_SENET_ENT_ICIN_MUH_HESAPLARINI_BELIRLE ( int bordro_satiri_id, int *borclu_hesap_id, int *alacakli_hesap_id, bool p_e9_onar_cagirdi, ADAK_SQL * P_GELEN_DB )
{

    SUBE_ENTEGRASYON_HESAPLARI_STRUCT * G_SB_ENT_HESAPLARI = (SUBE_ENTEGRASYON_HESAPLARI_STRUCT * ) SUBE_GET_ENTEGRASYON_HESAPLARI_STRUCT();

    int portfoydeki_cekler_hesap_id                    = G_SB_ENT_HESAPLARI->port_cekler_hsp_id;
    int tahsile_verilen_cekler_hesap_id                = G_SB_ENT_HESAPLARI->tah_verilen_cekler_hsp_id;
    int teminata_verilen_cekler_hesap_id               = G_SB_ENT_HESAPLARI->tem_verilen_cekler_hsp_id;
    int karsiliksiz_cekler_hesap_id                    = G_SB_ENT_HESAPLARI->kar_cekler_hsp_id;
    int portfoydeki_senetler_hesap_id                  = G_SB_ENT_HESAPLARI->port_senetler_hsp_id;
    int tahsile_verilen_senetler_hesap_id              = G_SB_ENT_HESAPLARI->tah_senetler_hsp_id;
    int teminata_verilen_senetler_hesap_id             = G_SB_ENT_HESAPLARI->tem_senetler_hsp_id;
    int protestolu_senetler_hesap_id                   = G_SB_ENT_HESAPLARI->pro_senetler_hsp_id;
    int ileri_tarih_portfoydeki_senetler_hesap_id      = G_SB_ENT_HESAPLARI->port_senetler_ileri_trh_hsp_id;
    int ileri_tarih_tahsile_verilen_senetler_hesap_id  = G_SB_ENT_HESAPLARI->tah_senetler_ileri_trh_hsp_id;
    int ileri_tarih_teminata_verilen_senetler_hesap_id = G_SB_ENT_HESAPLARI->tem_senetler_ileri_trh_hsp_id;
    int borc_senetleri_hesap_id                        = G_SB_ENT_HESAPLARI->borc_senetler_hsp_id;
    int ileri_tarih_borc_senetleri_hesap_id            = G_SB_ENT_HESAPLARI->borc_senetleri_ileri_trh_hsp_id;
    int supheli_alacaklar_hesap_id                     = G_SB_ENT_HESAPLARI->sup_alacaklar_hsp_id;
    int tahsil_edilemeyen_cekler_hesap_id              = G_SB_ENT_HESAPLARI->tahsil_edilemeyen_cekler_hsp_id;
    int tahsil_edilemeyen_senetler_hesap_id            = G_SB_ENT_HESAPLARI->tahsil_edilemeyen_senetler_hsp_id;
    int unite_muhasebe_kasa_hesap_id                   = G_SB_ENT_HESAPLARI->unite_muh_kasa_hsp_id;

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT("cek_bordrolar, cek_bordro_satirlari, cek_cekler_senetler",
                         "bordro_tarihi, cari_hesap_id , cek_bordrolar.aciklama,"
                         "cek_cekler_senetler.hesap_no_id, bordro_islem_turu, "
                         "cek_senet_turu, vade_tarihi, "
                         "musteri_ceki_mi, cek_cekler_senetler.cek_senet_id, "
                         "cek_bordrolar.modul_id , cek_bordrolar.program_id , base_fis_id, cek_bordrolar.bordro_id ",
                         "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id "
                         "AND cek_bordro_satirlari.cek_senet_id  = cek_cekler_senetler.cek_senet_id "
                         "AND bordro_satiri_id = :bordro_satiri_id");

    query.SET_VALUE(":bordro_satiri_id", bordro_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return false;
    }

    query.NEXT();

    QDate   bordro_tarihi                    = QDate::fromString(query.VALUE(0).toString(), "yyyy.MM.dd");
    int     cari_hesap_id                    = query.VALUE(1).toInt();
    QString aciklama                         = query.VALUE(2).toString();
    int     banka_hesap_id                   = query.VALUE(3).toInt();
    int     bordro_islem_turu                = query.VALUE(4).toInt();
    int     cek_senet_turu                   = query.VALUE(5).toInt();
    QDate   vade_tarihi                      = QDate::fromString(query.VALUE(6).toString(), "yyyy.MM.dd");
    int     vade_tarihine_kalan_gun_sayisi   = CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI(vade_tarihi);
    int     musteri_ceki_veya_seneti_mi      = query.VALUE(7).toInt();
    int     cek_senet_id                     = query.VALUE(8).toInt();
    int     modul_id                         = query.VALUE(9).toInt();
    int     program_id                       = query.VALUE(10).toInt();
    int     base_fid_id                      = query.VALUE(11).toInt();
    int     bordro_id                        = query.VALUE(12).toInt();

    int     cek_senet_son_durum    = -1;
    int     cek_senet_onceki_durum = -1;

    CEK_GET_CEK_SENET_SON_DURUMLARI( cek_senet_id, &cek_senet_son_durum, &cek_senet_onceki_durum );

    if( base_fid_id EQ 0 ) {
        base_fid_id = bordro_id;
    }

    //! ONAR ICIN EKLENMISTIR
    if( p_e9_onar_cagirdi EQ true ) {
        if( modul_id EQ CARI_MODULU ) {
            cek_senet_son_durum    = ENUM_PORTFOYDE;
            cek_senet_onceki_durum = ENUM_PORTFOYDE;
        }
        else {
            CEK_ONAR_ICIN_CEK_DURUMLARINI_BUL( cek_senet_id, &cek_senet_son_durum, &cek_senet_onceki_durum );
        }
    }
    //! END ONAR

    int muh_fis_id = MUH_GET_ENT_FIS_ID(program_id , modul_id , base_fid_id , USE_DB);


    int banka_muh_hesap_id                = 0;
    int banka_verilen_cekler_muh_hesap_id = 0;
    int cari_muh_hesap_id                 = 0;

    cari_muh_hesap_id  = CARI_GET_MUH_HESAP_ID ( cari_hesap_id );
    banka_muh_hesap_id = BNK_GET_BANKA_MUH_HESAP_ID( banka_hesap_id );

    if ( cek_senet_id > 0 ) {

        BNK_MUH_HESAPLARI_STRUCT BNK_MUH_HESAPLAR;

        BNK_GET_BANKA_MUH_HESAPLARI( &BNK_MUH_HESAPLAR, banka_hesap_id );
        banka_verilen_cekler_muh_hesap_id = BNK_MUH_HESAPLAR.verilen_cekler_hesap_id;

    }

    bool    senet_vadesi_bir_yildan_fazla_mi = false;

    if ( vade_tarihine_kalan_gun_sayisi > bordro_tarihi.daysInYear() ) {
        senet_vadesi_bir_yildan_fazla_mi = true;
    }

    switch ( cek_senet_onceki_durum ) {
        case ENUM_PORTFOYDE :
        // Önceki durumu portföyde olan çeklerin/senetlerin son durumuna göre entegrasyon kontrolü
        {
            if ( musteri_ceki_veya_seneti_mi EQ 1 ) {   // Müşteri çek/senetleri
                if ( cek_senet_turu EQ ENUM_CEK ) {
                    if ( portfoydeki_cekler_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Entegrasyon hesaplarında portföydeki çekler hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    if(modul_id EQ CARI_MODULU) {
                        if(bordro_islem_turu EQ ENUM_CEK_SATICIYA_VERILDI) {
                            *alacakli_hesap_id = portfoydeki_cekler_hesap_id;
                        }
                        if (bordro_islem_turu EQ ENUM_CEK_MUSTERIDEN_ALINDI) {
                            *borclu_hesap_id = portfoydeki_cekler_hesap_id;
                        }

                    }
                    else {
                        *alacakli_hesap_id = portfoydeki_cekler_hesap_id;
                    }
                }
                else {  // Çek değilse senettir
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true  ) {
                        if ( ileri_tarih_portfoydeki_senetler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                        "portföydeki senetler hesabı tanımlı değil."));

                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        if(modul_id EQ CARI_MODULU ) {

                            if(bordro_islem_turu EQ ENUM_CEK_MUSTERIDEN_ALINDI) {
                                *borclu_hesap_id = ileri_tarih_portfoydeki_senetler_hesap_id;
                            }
                            if(bordro_islem_turu EQ ENUM_CEK_SATICIYA_VERILDI) {
                                *alacakli_hesap_id = ileri_tarih_portfoydeki_senetler_hesap_id;
                            }
                        }
                        else {
                            *alacakli_hesap_id = ileri_tarih_portfoydeki_senetler_hesap_id;
                        }
                    }
                    else {
                        if ( portfoydeki_senetler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                        "portföydeki senetler hesabı tanımlı değil."));

                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        if(modul_id EQ CARI_MODULU) {
                            if(bordro_islem_turu EQ ENUM_CEK_MUSTERIDEN_ALINDI) {
                                *borclu_hesap_id = portfoydeki_senetler_hesap_id;
                            }
                            if(bordro_islem_turu EQ ENUM_CEK_SATICIYA_VERILDI) {
                                *alacakli_hesap_id = portfoydeki_senetler_hesap_id;
                            }
                        }
                        else {
                            *alacakli_hesap_id = portfoydeki_senetler_hesap_id;
                        }
                    }
                }
            }
            else {      // Kendi çeklerimiz/senetlerimiz
                if ( cek_senet_turu EQ  ENUM_CEK ) {
                    if ( banka_verilen_cekler_muh_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Banka hesabında verilen çekler muhasebe hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *alacakli_hesap_id = banka_verilen_cekler_muh_hesap_id;
                }
                else {
                    if (senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                        if ( ileri_tarih_borc_senetleri_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                            " borç senetleri hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *alacakli_hesap_id = ileri_tarih_borc_senetleri_hesap_id;
                    }
                    else {
                        if ( borc_senetleri_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                            "borc senetleri hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *alacakli_hesap_id = borc_senetleri_hesap_id;
                    }
                }
            }

            switch ( cek_senet_son_durum ) {

                case ENUM_BNK_TAHSILAT_VERILDI :

                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( tahsile_verilen_cekler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında tahsile verilen çekler hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id   = tahsile_verilen_cekler_hesap_id;
                        return true;
                    }
                     // Çek değilse senettir
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                        if ( ileri_tarih_tahsile_verilen_senetler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                        "tahsile verilen senetler hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id   = ileri_tarih_tahsile_verilen_senetler_hesap_id;
                        return true;
                    }
                    // Vade tarihi 1 yıldan az olan senet
                    if ( tahsile_verilen_senetler_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                    "tahsile verilen senetler hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id   = tahsile_verilen_senetler_hesap_id;
                    return true;

                case ENUM_BNK_TEMINAT_VERILDI :

                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( teminata_verilen_cekler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında teminata verilen çekler hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id = teminata_verilen_cekler_hesap_id;
                        return true;
                    }
                     // Çek değilse senettir
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                        if ( ileri_tarih_teminata_verilen_senetler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                            "teminata verilen senetler hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id   = ileri_tarih_teminata_verilen_senetler_hesap_id;
                        return true;
                    }
                    // Vade tarihi 1 yıldan az olan senet
                    if ( teminata_verilen_senetler_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                        "teminata verilen senetler hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id   = teminata_verilen_senetler_hesap_id;
                    return true;

                case ENUM_KARSILIKSIZ :
                case ENUM_PROTESTOLU  :

                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( karsiliksiz_cekler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında karşılıksız çekler hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id   =  karsiliksiz_cekler_hesap_id;
                        return true;
                    }
                    // Çek değilse senettir
                    if ( protestolu_senetler_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Entegrasyon hesaplarında protestolu senetler hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id   = protestolu_senetler_hesap_id;
                    return true;

                case ENUM_SATICIYA_VERILDI     :
                case ENUM_MUS_IADE_EDILDI :
                case ENUM_PORTFOYDE       :

                    cari_muh_hesap_id = CARI_GET_MUH_HESAP_ID( cari_hesap_id );

                    if ( cari_muh_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Cari hesabın muhasebe hesap kodu cari kartta tanımlı değil.  "));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        //return false;
                    }
                    if(cek_senet_son_durum EQ ENUM_PORTFOYDE) {
                        *alacakli_hesap_id = cari_muh_hesap_id;
                    }
                    else {
                        *borclu_hesap_id   = cari_muh_hesap_id;
                    }
                    return true;
                case ENUM_TAHSIL_EDILDI_NKT :            // Elden tahsil edilebilir
                    if ( unite_muhasebe_kasa_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Ünitede entegrasyon için gerekli muhasebe kasa hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id = unite_muhasebe_kasa_hesap_id;
                    return true;

                default :
                    return false;
            }
                break;
        }
        // **********  Önceki durumu portföyde olan çek/senet entegrasyon kontrolü sonu  ***********************
        case ENUM_BNK_TEMINAT_VERILDI :        {
           // Önceki durumu Bankaya Teminata verildi olan çek/senedin sonraki durumuna göre entegrasyon kontrolü
            if ( cek_senet_turu EQ ENUM_CEK ) {
                if ( teminata_verilen_cekler_hesap_id < 1 ) {
                    aciklama.append(QObject ::tr("Entegrasyon hesaplarında teminata verilen çekler hesabı tanımlı değil."));
                    MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                    return false;
                }
                *alacakli_hesap_id =  teminata_verilen_cekler_hesap_id;
            }
            else {  // Çek değilse senettir
                if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                    if ( ileri_tarih_teminata_verilen_senetler_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                        "teminata verilen senetler hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *alacakli_hesap_id = ileri_tarih_teminata_verilen_senetler_hesap_id;
                }
                else { // Vadesi bir yıldan az olan teminata verilen senetler
                    if ( teminata_verilen_senetler_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                        "teminata verilen senetler hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *alacakli_hesap_id =  teminata_verilen_senetler_hesap_id;
                }
            }
            switch ( cek_senet_son_durum ) {
               case ENUM_PORTFOYDE :  // Teminata verilen çek iade alınırsa portföye düşer -- sadece müşteri çekleri
                        if ( cek_senet_turu EQ ENUM_CEK ) {
                            if ( portfoydeki_cekler_hesap_id < 1 ) {
                                aciklama.append(QObject::tr("Entegrasyon hesaplarında portföydeki çekler hesabı tanımlı değil."));
                                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                return false;
                            }
                            *borclu_hesap_id = portfoydeki_cekler_hesap_id;
                            return true;
                        } // çek değilse senettir ( müşteri senedi )
                        if ( senet_vadesi_bir_yildan_fazla_mi EQ true  ) {
                            if ( ileri_tarih_portfoydeki_senetler_hesap_id < 1 ) {
                                aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                            "portföydeki senetler hesabı tanımlı değil."));
                                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                return false;
                            }
                            *borclu_hesap_id = ileri_tarih_portfoydeki_senetler_hesap_id;
                            return true;
                        }
                        if ( portfoydeki_senetler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                        "portföydeki senetler hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id =  portfoydeki_senetler_hesap_id;
                        return true;
                    case ENUM_TAHSIL_EDILDI_HESABA :
                        banka_muh_hesap_id = BNK_GET_BANKA_MUH_HESAP_ID( banka_hesap_id );

                        if ( banka_muh_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Bordroda tanımlı banka hesabının karşılığı olan muhasebe "
                                                        "hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            //return false;
                        }
                        *borclu_hesap_id = banka_muh_hesap_id;
                        return true;
                default :
                    return false;
                }
            break;
        } // **********  Önceki durumu bankaya teminata verildi olan çek/senet entegrasyon kontrolü sonu  ***********************

        case ENUM_BNK_TAHSILAT_VERILDI :
        // Önceki durumu Bankaya tahsile verildi olan çek/senedin sonraki durumuna göre entegrasyon kontrolü
        {
            if ( cek_senet_turu EQ ENUM_CEK ) {
                if ( tahsile_verilen_cekler_hesap_id < 1 ) {
                    aciklama.append(QObject::tr("Entegrasyon hesaplarında tahsile verilen çekler hesabı tanımlı değil."));
                    MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                    return false;
                }
                *alacakli_hesap_id =   tahsile_verilen_cekler_hesap_id;

            }
             // Çek değilse senettir
            if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                if ( ileri_tarih_tahsile_verilen_senetler_hesap_id < 1 ) {
                    aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                "tahsile verilen senetler hesabı tanımlı değil."));
                    MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                    return false;
                }
                *alacakli_hesap_id = ileri_tarih_tahsile_verilen_senetler_hesap_id;

            }
            // Vade tarihi 1 yıldan az olan senet
            if ( tahsile_verilen_senetler_hesap_id < 1 ) {
                aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                            "tahsile verilen senetler hesabı tanımlı değil."));
                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                return false;
            }
            *alacakli_hesap_id =  tahsile_verilen_senetler_hesap_id;


            switch ( cek_senet_son_durum ) {
                case ENUM_TAHSIL_EDILDI_HESABA :        // Bankaya tahsile verilen müşteri/çeki senedi
                                                        //tahsil edilirse banka hesabına borç yazılır.

                    if ( banka_muh_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Bordroda tanımlı banka hesabının karşılığı olan muhasebe "
                                                    "hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id = banka_muh_hesap_id;
                    return true;
                                    return true;
                case ENUM_PORTFOYDE :    // Bankaya tahsile verilen müşteri çeki/senedi iade alınırsa portföye geri döner
                default        :

                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( portfoydeki_cekler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında portföydeki çekler hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id = portfoydeki_cekler_hesap_id;
                        return true;
                    } // çek değilse senettir ( müşteri senedi )
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true  ) {
                        if ( ileri_tarih_portfoydeki_senetler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                        "portföydeki senetler hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id = ileri_tarih_portfoydeki_senetler_hesap_id;
                        return true;
                    }
                    if ( portfoydeki_senetler_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                    "portföydeki senetler hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id = portfoydeki_senetler_hesap_id;
                    return true;
            }
            break;
        } // **********  Önceki durumu bankaya tahsile verildi olan çek/senet entegrasyon kontrolü sonu  ***********************

        case ENUM_SATICIYA_VERILDI :

        {       // Önceki durumu müşteriye verildi olan çeklerin/ senetlerin sonraki durumuna göre entegrasyon kontrolü
                // Müşteriye verilen çek/senet iade alındığında portföye düşerler.

            switch ( cek_senet_son_durum ) {
                case ENUM_PORTFOYDE :
                default        :

                    if ( cari_muh_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Cari hesabın muhasebe hesap kodu cari kartta tanımlı değil.  "));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *alacakli_hesap_id = cari_muh_hesap_id;

                    if ( musteri_ceki_veya_seneti_mi EQ 1 ) {   // müşteri çeki veya senedi
                        if ( cek_senet_turu EQ ENUM_CEK ) {
                            if ( portfoydeki_cekler_hesap_id < 1 ) {
                                aciklama.append(QObject::tr("Entegrasyon hesaplarında portföydeki çekler hesabı tanımlı değil."));
                                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                return false;
                            }
                            *borclu_hesap_id   = portfoydeki_cekler_hesap_id;
                            return true;
                        } // çek değilse senettir ( müşteri senedi )
                        if ( senet_vadesi_bir_yildan_fazla_mi EQ true  ) {
                            if ( ileri_tarih_portfoydeki_senetler_hesap_id < 1 ) {
                                aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                            "portföydeki senetler hesabı tanımlı değil."));
                                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                return false;
                            }
                            *borclu_hesap_id = ileri_tarih_portfoydeki_senetler_hesap_id;
                            return true;
                        }
                        else {
                            if ( portfoydeki_senetler_hesap_id < 1 ) {
                                aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                            "portföydeki senetler hesabı tanımlı değil."));
                                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                return false;
                            }
                            *borclu_hesap_id = portfoydeki_senetler_hesap_id;
                            return true;
                        }
                    }
                    else { // Müşteriye verdiğimiz kendi çekimiz/senedimiz iade alınırsa
                           // verilen çekler/ borç senetleri hesabı etkilenir
                        if ( cek_senet_turu EQ  ENUM_CEK ) {
                            if ( banka_verilen_cekler_muh_hesap_id < 1 ) {
                                aciklama.append(QObject::tr("Banka hesabında verilen çekler muhasebe hesabı tanımlı değil."));
                                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                return false;
                            }
                            *borclu_hesap_id =  banka_verilen_cekler_muh_hesap_id;
                            return true;
                        }
                        else { // Borç senetleri hesapları
                            if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                                if ( ileri_tarih_borc_senetleri_hesap_id < 1 ) {
                                    aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                    " borç senetleri hesabı tanımlı değil."));
                                    MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                    return false;
                                }
                                *borclu_hesap_id = ileri_tarih_borc_senetleri_hesap_id;
                                return true;
                            }
                            else {
                                if ( borc_senetleri_hesap_id < 1 ) {
                                    aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                    "borc senetleri hesabı tanımlı değil."));
                                    MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                    return false;
                                }
                                *borclu_hesap_id = borc_senetleri_hesap_id;
                                return true;
                            }
                        }
                    }
                    break;

                case ENUM_ODENDI_NKT      :       // Müşteriye verilmiş bir çek/senet ancak kendi çekimiz veya senedimizse
                case ENUM_ODENDI_HESAPTAN :       // elden veya hesaptan ödendi durumuna düşebilir.

                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( banka_verilen_cekler_muh_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Banka hesabında verilen çekler muhasebe hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id = banka_verilen_cekler_muh_hesap_id;
                    }
                    else {      // Çek değilse senettir
                        if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                                if ( ileri_tarih_borc_senetleri_hesap_id < 1 ) {
                                    aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                    " borç senetleri hesabı tanımlı değil."));
                                    MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                    return false;
                                }
                                *borclu_hesap_id = ileri_tarih_borc_senetleri_hesap_id;
                            }
                            else {
                                if ( borc_senetleri_hesap_id < 1 ) {
                                    aciklama.append(QObject::tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                                " borç senetleri hesabı tanımlı değil."));
                                    MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                    return false;
                                }
                                *borclu_hesap_id = borc_senetleri_hesap_id;
                            }
                    }
                    switch ( bordro_islem_turu ) {
                        case ENUM_CEK_ODENDI_NKT :
                            if ( unite_muhasebe_kasa_hesap_id < 1 ) {
                                aciklama.append(QObject::tr("Ünitede entegrasyon için gerekli muhasebe kasa hesabı tanımlı değil."));
                                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                return false;
                            }
                            *alacakli_hesap_id = unite_muhasebe_kasa_hesap_id;
                            return true;
                        case ENUM_CEK_ODENDI_HESAPTAN :
                            if ( banka_muh_hesap_id < 1 ) {
                                aciklama.append(QObject::tr("Bordroda tanımlı banka hesabının karşılığı olan muhasebe "
                                                            "hesabı tanımlı değil."));
                                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                                return false;
                            }
                            *alacakli_hesap_id = banka_muh_hesap_id;
                            return true;
                        default :
                            break;
                    }
            }
            break;
        }
        // **********  Önceki durumu müşteriye verildi olan çek/senet entegrasyon kontrolü sonu  ***********************

        case ENUM_KARSILIKSIZ :
        case ENUM_PROTESTOLU  :

        {  // Önceki durumu karşılıksız/protestolu olan çeklerin/senetlerin sonraki durumuna göre entegrasyon kontrolü
            if ( cek_senet_turu EQ ENUM_CEK ) {
                if ( karsiliksiz_cekler_hesap_id < 1 ) {
                    aciklama.append(QObject::tr("Entegrasyon hesaplarında karşılıksız çekler hesabı tanımlı değil."));
                    MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                    return false;
                }
                *alacakli_hesap_id = karsiliksiz_cekler_hesap_id;
            }
            else {
            // Çek değilse senettir
                if ( protestolu_senetler_hesap_id < 1 ) {
                    aciklama.append(QObject::tr("Entegrasyon hesaplarında protestolu senetler hesabı tanımlı değil."));
                    MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                    return false;
                }
                *alacakli_hesap_id =  protestolu_senetler_hesap_id;
            }
            switch ( cek_senet_son_durum ) {

                case ENUM_TAHSIL_EDILDI_NKT :    // Karşılıksız/protestolu durumuna düşmüş bir çek/senet
                                           // sadece elden tahsil edilebilir
                    if ( unite_muhasebe_kasa_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Ünitede entegrasyon için gerekli muhasebe kasa hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id = unite_muhasebe_kasa_hesap_id;
                    return true;

                case ENUM_SUPHELI_ALACAK :

                    if ( supheli_alacaklar_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Entegrasyon hesaplarında şüpheli alacaklar hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id = supheli_alacaklar_hesap_id;
                    return true;

                case ENUM_MUS_IADE_EDILDI :

                    if ( cari_muh_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Cari hesabın muhasebe hesap kodu cari kartta tanımlı değil.  "));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id = cari_muh_hesap_id;
                    return true;
                default :
                    return false;
            }

        }
        // ********** Önceki durumu karşılıksız/protestolu olan çek/senet entegrasyon kontrolü sonu  ***********************

        case ENUM_SUPHELI_ALACAK :

        { // Önceki durumu şüpheli alacaklar olan çeklerin/senetlerin sonraki durumuna göre entegrasyon kontrolü
            if ( supheli_alacaklar_hesap_id < 1 ) {
                aciklama.append(QObject::tr("Entegrasyon hesaplarında şüpheli alacaklar hesabı tanımlı değil."));
                MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                return false;
            }
            *alacakli_hesap_id = supheli_alacaklar_hesap_id;
            switch ( cek_senet_son_durum ) {
                case ENUM_TAHSIL_EDILDI_NKT : // Şüpheli alacaklara düşmüş bir çek elden tahsil edilebilir.
                    if ( unite_muhasebe_kasa_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Ünitede entegrasyon için gerekli muhasebe kasa hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id = unite_muhasebe_kasa_hesap_id;
                    return true;
                case ENUM_MUS_IADE_EDILDI :

                    if ( cari_muh_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Cari hesabın muhasebe hesap kodu cari kartta tanımlı değil.  "));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id = cari_muh_hesap_id;
                    return true;

                case ENUM_TAHSIL_EDILEMEYEN :
                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( tahsil_edilemeyen_cekler_hesap_id < 1 ) {
                            aciklama.append(QObject::tr("Entegrasyon hesaplarında tahsil edilmeyen çekler hesabı tanımlı değil."));
                            MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                            return false;
                        }
                        *borclu_hesap_id = tahsil_edilemeyen_cekler_hesap_id;
                        return true;
                    }
                    // Çek değilse senettir
                    if ( tahsil_edilemeyen_senetler_hesap_id < 1 ) {
                        aciklama.append(QObject::tr("Entegrasyon hesaplarında tahsil edilmeyen senetler hesabı tanımlı değil."));
                        MUH_UPDATE_FIS_ACIKLAMA( muh_fis_id, aciklama );
                        return false;
                    }
                    *borclu_hesap_id =  tahsil_edilemeyen_senetler_hesap_id;
                    return true;
                default :
                    return false;
            }
        }  // ********** Önceki durumu şüpheli alacak olan çek/senet entegrasyon kontrolü sonu  ***********************

        case ENUM_TAHSIL_EDILDI_NKT          :
        case ENUM_TAHSIL_EDILDI_HESABA       :
        case ENUM_MUS_IADE_EDILDI            :
        case ENUM_ODENDI_NKT                 :
        case ENUM_ODENDI_HESAPTAN            :
        case ENUM_TAHSIL_EDILEMEYEN          :
        default                              :
        // Önceki durumu Tahsil edildi, mus iade edildi, ödendi veya tahsil edilemeyen olan çeklerin sonraki
        // durumları yoktur. Herhangi bit muhasebe entegrasyonu söz konusu olamaz.
            return false;
    }
    return false;
}

CEK_BORDRO_STRUCT::CEK_BORDRO_STRUCT()
{
    CLEAR_BORDRO_STRUCT();
}

/**************************************************************************************
                             CEK_CLEAR_BORDRO_STRUCT()
***************************************************************************************/

void CEK_BORDRO_STRUCT::CLEAR_BORDRO_STRUCT()
{
    this->modul_id              = -1;
    this->program_id            = -1;
    this->bordro_no             =  0;
    this->bordro_islem_turu     = -1;
    this->bordro_turu           = -1;
    this->cari_hesap_id         =  0;
    this->cari_para_birimi_id   =  0;
    this->hesap_no_id           =  0;
    this->toplam_tutar          =  0;
    this->base_fis_id           =  0;
    this->cek_bordrosu_mu       = -1;
    this->bordro_para_birimi_id =  0;
    this->doviz_kuru            =  0;
    this->parite                =  0;
    this->musteri_cek_senedi_mi = -1;
    this->muh_fis_id            =  0;

    this->bordro_tarihi.clear();
    this->aciklama.clear();
}

/**************************************************************************************
                             CEK_CLEAR_BORDRO_SATIRI_STRUCT()
***************************************************************************************/

void CEK_CLEAR_BORDRO_SATIRI_STRUCT(CEK_BORDRO_SATIRI_STRUCT *P_BORDRO_SATIRI)
{
    P_BORDRO_SATIRI->bordro_id                = 0;
    P_BORDRO_SATIRI->cek_senet_id             = 0;
    P_BORDRO_SATIRI->order_number             = 0;
    P_BORDRO_SATIRI->alinan_cari_hesap_id     = 0;
    P_BORDRO_SATIRI->verilen_cari_hesap_id    = 0;
    P_BORDRO_SATIRI->cek_tutari               = 0;
    P_BORDRO_SATIRI->temel_para_birimi_tutari = 0;
    P_BORDRO_SATIRI->cari_para_birimi_tutari  = 0;
    P_BORDRO_SATIRI->base_record_id           = 0;
    P_BORDRO_SATIRI->modul_id                 = -1;
    P_BORDRO_SATIRI->program_id               = -1;

    P_BORDRO_SATIRI->aciklama.clear();
}

/**************************************************************************************
                                    CEK_GET_ENT_FIS_ID_LIST
***************************************************************************************/

QStringList CEK_GET_ENT_FIS_ID_LIST ( int p_program_id, int p_modul_id, int p_base_fis_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    QStringList BORDRO_ID_LIST;

    SQL_QUERY query (USE_DB );

    query.PREPARE_SELECT ( "cek_bordrolar","bordro_id",
                           "program_id      = :program_id   "
                           "AND modul_id    = :modul_id     "
                           "AND base_fis_id = :base_fis_id  " );

    query.SET_VALUE ( ":program_id"  , p_program_id  );
    query.SET_VALUE ( ":modul_id"    , p_modul_id    );
    query.SET_VALUE ( ":base_fis_id" , p_base_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return BORDRO_ID_LIST;
    }

    while ( query.NEXT() EQ true ) {
        BORDRO_ID_LIST.append ( query.VALUE ( 0 ).toString() );
    }

    return BORDRO_ID_LIST;
}

/**************************************************************************************
                                    CEK_GET_ENT_ROW_ID_LIST()
***************************************************************************************/

QStringList CEK_GET_ENT_ROW_ID_LIST(int p_program_id, int p_modul_id, int p_base_record_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    QStringList ROW_ID_LIST;

    query.PREPARE_SELECT("cek_bordro_satirlari", "bordro_satiri_id ",
                         "program_id = :program_id AND modul_id = :modul_id AND "
                         "base_record_id = :base_record_id");

    query.SET_VALUE(":program_id"     , p_program_id);
    query.SET_VALUE(":modul_id"       , p_modul_id  );
    query.SET_VALUE(":base_record_id" , p_base_record_id);

    if ( query.SELECT() EQ 0 ) {
        return QStringList();
    }
    while ( query.NEXT() ) {
        ROW_ID_LIST << query.VALUE(0).toString();
    }

    return ROW_ID_LIST;
}

/**************************************************************************************
                     CEK_GET_BORDRO_ISLEM_TURUNE_GORE_ENTEGRASYON_TURU
***************************************************************************************/

int CEK_GET_BORDRO_ISLEM_TURUNE_GORE_ENTEGRASYON_TURU(int p_bordro_islem_turu , int p_modul_id)
{
    switch ( p_bordro_islem_turu ) {
        case ENUM_CEK_BNK_TAHSILATA_VERILDI :
        case ENUM_CEK_BNK_TEMINATA_VERILDI  :
        case ENUM_CEK_TAHSIL_EDILDI_NKT     :
        case ENUM_CEK_KARSILIKSIZ           :
        case ENUM_CEK_PROTESTOLU            :
        case ENUM_CEK_ODENDI_NKT            :
        case ENUM_CEK_TAHSIL_EDILEMEYEN     :
        case ENUM_CEK_SUPHELI_ALACAK        :
        case ENUM_CEK_MUSTERIDEN_ALINDI     :
        case ENUM_CEK_IADE_ALINDI_BANKADAN  :

            return ENUM_MUHASEBE_ENTEGRASYONU;

        case ENUM_CEK_TAHSIL_EDILDI_HESABA  :
        case ENUM_CEK_ODENDI_HESAPTAN       :

            return ENUM_BANKA_ENTEGRASYONU;

        case ENUM_CEK_SATICIYA_VERILDI           :
            if ( p_modul_id NE CEKSENET_MODULU ) {
                return ENUM_MUHASEBE_ENTEGRASYONU;
            }

            return ENUM_CARI_ENTEGRASYON;

        case ENUM_CEK_MUS_IADE_EDILDI       :
        case ENUM_CEK_IADE_ALINDI_SATICIDAN:

            return ENUM_CARI_ENTEGRASYON;
        default:
           break;
    }
    return -1;
}

/**************************************************************************************
                     CEK_ONAR_ICIN_CEK_DURUMLARINI_BUL
***************************************************************************************/

void CEK_ONAR_ICIN_CEK_DURUMLARINI_BUL(int p_cek_senet_id, int *p_son_durum, int *p_onceki_durum)
{
    SQL_QUERY query( DB );
    QMap<int, int > cek_hareketleri_map;
    query.PREPARE_SELECT( "cek_hareketleri", "cek_son_hareketi, cek_onceki_hareketi, cek_hareketi_id", "cek_senet_id = :cek_senet_id");
    query.SET_VALUE     ( ":cek_senet_id", p_cek_senet_id );

    int select_size = query.SELECT( "cek_hareketi_id DESC", 0, 2 );
    if( select_size NE 0 ) {

        while( query.NEXT() EQ true ) {
            cek_hareketleri_map.insert( query.VALUE(0).toInt(), query.VALUE(1).toInt());
        }
    }

    if( cek_hareketleri_map.size() EQ 1 ) {
        *p_son_durum = cek_hareketleri_map.keys().at(0);
        *p_onceki_durum = cek_hareketleri_map.values().at(0);
    }
    else {
        *p_son_durum = cek_hareketleri_map.keys().at(1);
        *p_onceki_durum = cek_hareketleri_map.values().at(1);
    }

    if ( *p_onceki_durum EQ -1 ) {
        *p_onceki_durum = ENUM_PORTFOYDE;
    }

}

/**************************************************************************************
                     CEK_GET_CEK_SENET_SON_DURUMLARI
***************************************************************************************/

void CEK_GET_CEK_SENET_SON_DURUMLARI(int p_cek_senet_id, int *p_cek_senet_son_durum, int *p_cek_senet_onceki_durum)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "cek_hareketleri", "cek_onceki_hareketi, cek_son_hareketi, cek_hareketi_id", "cek_senet_id = :cek_senet_id" );
    query.SET_VALUE     ( ":cek_senet_id", p_cek_senet_id );

    if( query.SELECT( "cek_hareketi_id DESC") NE 0 ) {
        query.NEXT();

        *p_cek_senet_onceki_durum = query.VALUE( 0 ).toInt();
        *p_cek_senet_son_durum    = query.VALUE( 1 ).toInt();

        if (*p_cek_senet_onceki_durum EQ -1 ) {
            *p_cek_senet_onceki_durum = ENUM_PORTFOYDE;
        }
    }
}
