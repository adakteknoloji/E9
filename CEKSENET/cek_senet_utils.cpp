#include <QDate>
#include "adak_sql.h"
#include "cek_senet_utils.h"
#include "cek_enum.h"
#include "cek_console_utils.h"

extern  ADAK_SQL       *       DB;

/**************************************************************************************
                                CEK_SENET_EKLE
***************************************************************************************/

int CEK_SENET_EKLE ( CEK_SENET_STRUCT * P_CEK_SENET )
{
    SQL_QUERY query ( DB );

    //! DIKKAT
    //! BURDA LOCK KULLANILMASININ SEBENI CEK VE SENEDE VERILEN PORTFOY NUMARASININ
    //! BIRINCIL OLMASIDIR. BUNDAN DOLAYI SISTEME BIR CEK VEYA SENET GRILDIRINDE
    //! DIGER KULLANICILARIN AYNI PORTFOY NUMARASINI ALMAMALARI ICIN LOCK EDILIYOR.
    //! PORTFOY NUMARASI SON INDIS E9_SABIT DEGERLERDE TUTUYOR.

    int lock_return_value = DB->LOCK_ROW ( "sabit_deger_id", "e9_sabit_degerler", QString ( "sabit_deger_id = 1" ) );

    if ( lock_return_value NE ADAK_OK ) {
        return 0;
    }

    if ( P_CEK_SENET->portfoy_numarasi EQ 0 ) {

        query.PREPARE_SELECT ( "e9_sabit_degerler","cek_portfoy_numarasi_sayaci",
                               "sabit_deger_id = 1" );

        if ( query.SELECT() EQ 0 ) {
            DB->UNLOCK_ROW ( "sabit_deger_id", "e9_sabit_degerler", QString ( "sabit_deger_id = 1" ) );
            return 0;
        }
        query.NEXT();

        P_CEK_SENET->portfoy_numarasi = query.VALUE(0).toInt() + 1;

        query.PREPARE_UPDATE   ( "e9_sabit_degerler", "sabit_deger_id", "cek_portfoy_numarasi_sayaci ",
                                 "sabit_deger_id = :sabit_deger_id" );

        query.SET_VALUE ( ":cek_portfoy_numarasi_sayaci" , P_CEK_SENET->portfoy_numarasi );
        query.SET_VALUE ( ":sabit_deger_id"              , 1 );

        query.UPDATE    ();

    }
    else {

        query.PREPARE_SELECT ( "cek_cekler_senetler", "cek_senet_id ", "portfoy_numarasi = :portfoy_numarasi" );
        query.SET_VALUE      ( ":portfoy_numarasi", P_CEK_SENET->portfoy_numarasi );

        if ( query.SELECT() > 0 ) {
            DB->UNLOCK_ROW ( "sabit_deger_id", "e9_sabit_degerler", QString ( "sabit_deger_id = 1" ) );
            return 0;
        }

        query.PREPARE_SELECT ( "e9_sabit_degerler", "cek_portfoy_numarasi_sayaci ", "sabit_deger_id = :sabit_deger_id" );
        query.SET_VALUE      (":sabit_deger_id", 0 );

        if ( query.SELECT() EQ 0 ) {
            DB->UNLOCK_ROW (  "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = 1" ) );
            return 0;
        }
        query.NEXT();

        if ( P_CEK_SENET->portfoy_numarasi > query.VALUE(0).toInt() ) {
            query.PREPARE_UPDATE ( "e9_sabit_degerler ", "sabit_deger_id",
                                   "cek_portfoy_numarasi_sayaci ",
                                   "sabit_deger_id = :sabit_deger_id" );
            query.SET_VALUE      ( ":cek_portfoy_numarasi_sayaci", P_CEK_SENET->portfoy_numarasi );
            query.SET_VALUE      ( ":sabit_deger_id"             , 1 );
            query.UPDATE();
        }
    }

    DB->UNLOCK_ROW ( "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = 1" ) );

    query.PREPARE_INSERT ( "cek_cekler_senetler","cek_senet_id" , "portfoy_numarasi, banka_ismi, "
                           "sube_kodu, "
                           "sube_ismi, cek_senet_numarasi, cek_senet_son_durumu, vade_tarihi, keside_tarihi, "
                           "borclu_kesideci, musteri_ceki_mi, cek_senet_tutari, musteri_hesap_numarasi, "
                           "cek_senet_turu, doviz_id ,hesap_no_id");

    query.SET_VALUE ( ":portfoy_numarasi"          , P_CEK_SENET->portfoy_numarasi          );
    query.SET_VALUE ( ":banka_ismi"                , P_CEK_SENET->banka_ismi                );
    query.SET_VALUE ( ":sube_kodu"                 , P_CEK_SENET->sube_kodu                 );
    query.SET_VALUE ( ":sube_ismi"                 , P_CEK_SENET->sube_ismi                 );
    query.SET_VALUE ( ":cek_senet_numarasi"        , P_CEK_SENET->cek_senet_numarasi        );
    query.SET_VALUE ( ":cek_senet_son_durumu"      , ENUM_PORTFOYDE                         );
    query.SET_VALUE ( ":vade_tarihi"               , P_CEK_SENET->vade_tarihi               );
    query.SET_VALUE ( ":keside_tarihi"             , P_CEK_SENET->keside_tarihi             );
    query.SET_VALUE ( ":borclu_kesideci"           , P_CEK_SENET->borclu_kesideci           );
    query.SET_VALUE ( ":musteri_ceki_mi"           , P_CEK_SENET->musteri_ceki_mi           );
    query.SET_VALUE ( ":cek_senet_tutari"          , P_CEK_SENET->cek_senet_tutari          );
    query.SET_VALUE ( ":musteri_hesap_numarasi"    , P_CEK_SENET->musteri_hesap_numarasi    );
    query.SET_VALUE ( ":cek_senet_turu"            , P_CEK_SENET->cek_senet_turu            );
    query.SET_VALUE ( ":doviz_id"                  , P_CEK_SENET->doviz_id                  );
    query.SET_VALUE ( ":hesap_no_id"               , P_CEK_SENET->hesap_no_id               );

    return query.INSERT();
}

/**************************************************************************************
                                CEK_SENET_SIL
***************************************************************************************/

void CEK_SENET_SIL ( int p_cek_senet_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_DELETE ( "cek_hareketleri","cek_senet_id = :cek_senet_id");
    query.SET_VALUE      ( ":cek_senet_id" , p_cek_senet_id );
    query.DELETE();

    query.PREPARE_DELETE ( "cek_cekler_senetler", "cek_senet_id = :cek_senet_id");
    query.SET_VALUE      ( ":cek_senet_id" , p_cek_senet_id );
    query.DELETE();
}

/**************************************************************************************
                                CEK_SENET_GUNCELLE()
***************************************************************************************/

void CEK_SENET_GUNCELLE( CEK_SENET_STRUCT *P_CEK_SENET, int p_cek_senet_id)
{
    SQL_QUERY query(DB);

    if ( P_CEK_SENET->portfoy_numarasi > 0 ) {

        query.PREPARE_SELECT ( "e9_sabit_degerler", "cek_portfoy_numarasi_sayaci", "sabit_deger_id = :sabit_deger_id");
        query.SET_VALUE      ( ":sabit_deger_id", 1 );

        if ( query.SELECT() EQ 0 ) {
            DB->UNLOCK_ROW (  "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = 1" ));
            return;
        }
        query.NEXT();

        int cek_portfoy_numarasi_sayaci = query.VALUE(0).toInt();

        if ( P_CEK_SENET->portfoy_numarasi > cek_portfoy_numarasi_sayaci ) {
            query.PREPARE_UPDATE ( "e9_sabit_degerler ", "sabit_deger_id",
                                   "cek_portfoy_numarasi_sayaci ",
                                   "sabit_deger_id = :sabit_deger_id" );
            query.SET_VALUE      ( ":cek_portfoy_numarasi_sayaci" , P_CEK_SENET->portfoy_numarasi );
            query.SET_VALUE      ( ":sabit_deger_id"              , 1 );
            query.UPDATE();
        }
        DB->UNLOCK_ROW ( "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = 1" ) );
    }


    query.PREPARE_UPDATE ( "cek_cekler_senetler","cek_senet_id" ,
                           "portfoy_numarasi        , "
                           "banka_ismi              , "
                           "sube_ismi               , "
                           "sube_kodu               , "
                           "hesap_no_id             , "
                           "cek_senet_numarasi      , "
                           "vade_tarihi             , "
                           "keside_tarihi           , "
                           "cek_senet_tutari        , "
                           "doviz_id                , "
                           "musteri_hesap_numarasi    ",
                           "cek_senet_id      = :cek_senet_id" );

    query.SET_VALUE      ( ":portfoy_numarasi"          , P_CEK_SENET->portfoy_numarasi);
    query.SET_VALUE      ( ":banka_ismi"                , P_CEK_SENET->banka_ismi);
    query.SET_VALUE      ( ":sube_ismi"                 , P_CEK_SENET->sube_ismi);
    query.SET_VALUE      ( ":sube_kodu"                 , P_CEK_SENET->sube_kodu);
    query.SET_VALUE      ( ":hesap_no_id"               , P_CEK_SENET->hesap_no_id );
    query.SET_VALUE      ( ":cek_senet_numarasi"        , P_CEK_SENET->cek_senet_numarasi);
    query.SET_VALUE      ( ":vade_tarihi"               , P_CEK_SENET->vade_tarihi);
    query.SET_VALUE      ( ":keside_tarihi"             , P_CEK_SENET->keside_tarihi);
    query.SET_VALUE      ( ":cek_senet_tutari"          , P_CEK_SENET->cek_senet_tutari);
    query.SET_VALUE      ( ":doviz_id"                  , P_CEK_SENET->doviz_id);
    query.SET_VALUE      ( ":musteri_hesap_numarasi"    , P_CEK_SENET->musteri_hesap_numarasi);
    query.SET_VALUE      ( ":cek_senet_id"              , p_cek_senet_id );

    query.UPDATE();

}

/**************************************************************************************
                            CEK_GET_TEMEL_BILGILER
***************************************************************************************/

int CEK_GET_TEMEL_BILGILER( CEK_SENET_STRUCT *P_CEK, int p_cek_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("cek_cekler_senetler",
                           "banka_ismi, sube_kodu, sube_ismi, musteri_hesap_numarasi,"
                           "cek_senet_numarasi, vade_tarihi, cek_senet_tutari, cek_senet_turu, "
                           "keside_tarihi, borclu_kesideci, doviz_id, hesap_no_id,"
                           "musteri_ceki_mi, cek_senet_son_durumu, "
                           "portfoy_numarasi ,cek_senet_id",
                           "cek_senet_id = :cek_senet_id");
    query.SET_VALUE ( ":cek_senet_id", p_cek_id  );


    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        P_CEK->banka_ismi               = query.VALUE( "banka_ismi" ).toString();
        P_CEK->sube_ismi                = query.VALUE( "sube_ismi" ).toString();
        P_CEK->musteri_hesap_numarasi   = query.VALUE( "musteri_hesap_numarasi" ).toString();
        P_CEK->cek_senet_numarasi       = query.VALUE( "cek_senet_numarasi" ).toString();
        P_CEK->vade_tarihi              = query.VALUE( "vade_tarihi" ).toString();
        P_CEK->cek_senet_tutari         = query.VALUE( "cek_senet_tutari" ).toDouble();
        P_CEK->cek_senet_turu           = query.VALUE( "cek_senet_turu" ).toInt();
        P_CEK->portfoy_numarasi         = query.VALUE( "portfoy_numarasi" ).toInt();
        P_CEK->sube_kodu                = query.VALUE( "sube_kodu" ).toString();
        P_CEK->keside_tarihi            = query.VALUE( "keside_tarihi" ).toString();
        P_CEK->borclu_kesideci          = query.VALUE( "borclu_kesideci" ).toString();
        P_CEK->doviz_id                 = query.VALUE( "doviz_id" ).toInt();
        P_CEK->musteri_ceki_mi          = query.VALUE( "musteri_ceki_mi" ).toInt();
        P_CEK->cek_senet_son_durumu     = query.VALUE( "cek_senet_son_durumu" ).toInt();
        P_CEK->cek_senet_id             = query.VALUE( "cek_senet_id").toInt();

        return 1;
    }
   return 0;
}













