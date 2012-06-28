#include <QStringList>
#include <QString>
#include "e9_console_utils.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "muh_fis_utils.h"
#include "muh_enum.h"
#include "muh_console_utils.h"
 

extern ADAK_SQL *    DB;

/**************************************************************************************
                                    MUH_FIS_EKLE
***************************************************************************************/


int  MUH_FIS_EKLE( MUH_FIS_STRUCT * P_FIS , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB NE NULL ) {
       USE_DB = P_GELEN_DB;
    }
    else {
       USE_DB = DB;
    }

    int fis_no = P_FIS->fis_no;

    if ( P_FIS->fis_no EQ 0 ) {
        fis_no =  MUH_SIRADAKI_FIS_NO_AL ( P_FIS->fis_tarihi , USE_DB);
    }

    SQL_QUERY query( USE_DB );

    query.PREPARE_INSERT("muh_fisler" , "fis_id" ,
                         "fis_no , fis_tarihi , aciklama , toplam_borc , "
                         "toplam_alacak , kasa_hesabi_id , fis_turu , yevmiye_numarasi , yvm_defterine_basildi,"
                         "last_order_number , modul_id , program_id , base_fis_id");


    query.SET_VALUE(":fis_no"                   , fis_no                                   );
    query.SET_VALUE(":fis_tarihi"               , P_FIS->fis_tarihi                        );
    query.SET_VALUE(":aciklama"                 , P_FIS->aciklama                          );
    query.SET_VALUE(":toplam_borc"              , 0                                        );
    query.SET_VALUE(":toplam_alacak"            , 0                                        );
    query.SET_VALUE(":kasa_hesabi_id"           , P_FIS->kasa_hesabi_id                    );
    query.SET_VALUE(":fis_turu"                 , P_FIS->fis_turu                          );
    query.SET_VALUE(":yevmiye_numarasi"         , 0                                        );
    query.SET_VALUE(":yvm_defterine_basildi"    , 0                                        );
    query.SET_VALUE(":last_order_number"        , 0                                        );
    query.SET_VALUE(":modul_id"                 , P_FIS->modul_id                          );
    query.SET_VALUE(":program_id"               , P_FIS->program_id                        );
    query.SET_VALUE(":base_fis_id"              , P_FIS->base_fis_id                       );

    int fis_id = query.INSERT();


    return fis_id;
}

/**************************************************************************************
                                    MUH_FIS_SATIRI_EKLE
***************************************************************************************/

int  MUH_FIS_SATIRI_EKLE ( MUH_FIS_STRUCT * P_FIS , MUH_FIS_SATIRI_STRUCT * P_FIS_SATIRI , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("muh_fisler" , "toplam_borc,toplam_alacak,last_order_number,fis_turu,kasa_hesabi_id,fis_tarihi, "
                         "modul_id , program_id" , "fis_id = :fis_id");


    query.SET_VALUE(":fis_id" , P_FIS_SATIRI->fis_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    double toplam_borc       = query.VALUE ( 0 ).toDouble();
    double toplam_alacak     = query.VALUE ( 1 ).toDouble();
    int    last_order_number = query.VALUE ( 2 ).toInt();
    int    fis_turu          = query.VALUE ( 3 ).toInt();
    int    kasa_hesabi_id    = query.VALUE ( 4 ).toInt();
    QDate  fis_tarihi        = QDate::fromString(query.VALUE(5).toString() , "yyyy.MM.dd");
    int    modul_id          = query.VALUE ( 6 ).toInt();
    int    program_id        = query.VALUE ( 7 ).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id ) {
        return 0;
    }

    int order_number = 0;
    if ( P_FIS_SATIRI->order_number EQ 0 ) {
        order_number = last_order_number + 1024;
    }
    else {
        order_number = P_FIS_SATIRI->order_number;
    }

    int gun_no = 0;

    if ( fis_turu NE ENUM_ACILIS_FISI ) {
        gun_no = MALI_YIL_ARRAY_INDIS(fis_tarihi);
    }

    int gizli_satir_id = 0;

    if ( fis_turu EQ ENUM_TAHSIL_FISI OR fis_turu EQ ENUM_TEDIYE_FISI ) {
        //Gizli kasa hesabi satiri eklenecek.

        query.PREPARE_INSERT("muh_fis_satirlari" , "fis_satiri_id" , "fis_id , hesap_id , aciklama , "
                             "borc_tutari , alacak_tutari , order_number , gizli_satir_mi , gizli_satir_id,"
                             "base_record_id, modul_id, program_id");


        query.SET_VALUE(":fis_id"           , P_FIS_SATIRI->fis_id                      );
        query.SET_VALUE(":hesap_id"         , kasa_hesabi_id                            );
        query.SET_VALUE(":aciklama"         , P_FIS_SATIRI->aciklama                    );
        query.SET_VALUE(":borc_tutari"      , ROUND ( P_FIS_SATIRI->alacak_tutari ) );
        query.SET_VALUE(":alacak_tutari"    , ROUND ( P_FIS_SATIRI->borc_tutari   ) );
        query.SET_VALUE(":order_number"     , order_number                              );
        query.SET_VALUE(":gizli_satir_mi"   , 1                                         );
        query.SET_VALUE(":gizli_satir_id"   , 0                                         );
        query.SET_VALUE(":base_record_id"   , P_FIS_SATIRI->base_record_id              );
        query.SET_VALUE(":modul_id"         , P_FIS_SATIRI->modul_id                    );
        query.SET_VALUE(":program_id"       , P_FIS_SATIRI->program_id                  );

        gizli_satir_id = query.INSERT();

        MUH_HESABININ_BAKIYELERINI_ARTTIR ( kasa_hesabi_id , gun_no , P_FIS_SATIRI->alacak_tutari , P_FIS_SATIRI->borc_tutari, USE_DB);

    }

    if ( fis_turu EQ ENUM_TAHSIL_FISI OR fis_turu EQ ENUM_TEDIYE_FISI ) {
        order_number += 1024;
    }

    query.PREPARE_INSERT("muh_fis_satirlari" , "fis_satiri_id" , "fis_id , hesap_id , aciklama ,"
                         "borc_tutari , alacak_tutari , order_number , gizli_satir_mi , gizli_satir_id,"
                         "base_record_id , modul_id , program_id");


    query.SET_VALUE(":fis_id"               , P_FIS_SATIRI->fis_id                  );
    query.SET_VALUE(":hesap_id"             , P_FIS_SATIRI->hesap_id                );
    query.SET_VALUE(":aciklama"             , P_FIS_SATIRI->aciklama                );
    query.SET_VALUE(":borc_tutari"          , ROUND( P_FIS_SATIRI->borc_tutari   )  );
    query.SET_VALUE(":alacak_tutari"        , ROUND( P_FIS_SATIRI->alacak_tutari )  );
    query.SET_VALUE(":order_number"         , order_number                          );
    query.SET_VALUE(":gizli_satir_mi"       , 0                                     );
    query.SET_VALUE(":gizli_satir_id"       , gizli_satir_id                        );
    query.SET_VALUE(":base_record_id"       , P_FIS_SATIRI->base_record_id          );
    query.SET_VALUE(":modul_id"             , P_FIS_SATIRI->modul_id                );
    query.SET_VALUE(":program_id"           , P_FIS_SATIRI->program_id              );

    int fis_satiri_id = query.INSERT();

    MUH_HESABININ_BAKIYELERINI_ARTTIR ( P_FIS_SATIRI->hesap_id , gun_no ,
                                           P_FIS_SATIRI->borc_tutari , P_FIS_SATIRI->alacak_tutari, USE_DB);


    toplam_borc   = ROUND ( toplam_borc   + P_FIS_SATIRI->borc_tutari );
    toplam_alacak = ROUND ( toplam_alacak + P_FIS_SATIRI->alacak_tutari );

    if ( fis_turu EQ ENUM_TAHSIL_FISI ) {
        toplam_borc = toplam_alacak;
    }
    else if ( fis_turu EQ ENUM_TEDIYE_FISI ) {
        toplam_alacak = toplam_borc;
    }



    query.PREPARE_UPDATE("muh_fisler","fis_id","toplam_borc, toplam_alacak , "
                         "last_order_number ","fis_id = :fis_id");

    query.SET_VALUE ( ":toplam_borc"       , toplam_borc          );
    query.SET_VALUE ( ":toplam_alacak"     , toplam_alacak        );
    query.SET_VALUE ( ":last_order_number" , order_number         );
    query.SET_VALUE ( ":fis_id"            , P_FIS_SATIRI->fis_id );

    query.UPDATE();


    P_FIS->toplam_borc   = toplam_borc;
    P_FIS->toplam_alacak = toplam_alacak;

    return fis_satiri_id;
}

/**************************************************************************************
                                    MUH_FIS_SATIRINI_SIL
***************************************************************************************/

int MUH_FIS_SATIRINI_SIL ( MUH_FIS_STRUCT * P_FIS, int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query (USE_DB);

    query.PREPARE_SELECT("muh_fisler,muh_fis_satirlari" , "toplam_borc , toplam_alacak , fis_turu , fis_tarihi , kasa_hesabi_id,  "
                         "borc_tutari , alacak_tutari , gizli_satir_id , hesap_id , muh_fis_satirlari.fis_id , "
                         "muh_fis_satirlari.modul_id , muh_fis_satirlari.program_id, " ,
                         "muh_fisler.fis_id = muh_fis_satirlari.fis_id  AND fis_satiri_id = :fis_satiri_id");


    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    double toplam_borc      = query.VALUE(0).toDouble();
    double toplam_alacak    = query.VALUE(1).toDouble();
    int    fis_turu         = query.VALUE(2).toInt();
    QDate  fis_tarihi       = QDate::fromString(query.VALUE(3).toString() , "yyyy.MM.dd");
    int    kasa_hesabi_id   = query.VALUE(4).toInt();
    double borc_tutari      = query.VALUE(5).toDouble();
    double alacak_tutari    = query.VALUE(6).toDouble();
    int    gizli_satir_id   = query.VALUE(7).toInt();
    int    hesap_id         = query.VALUE(8).toInt();
    int    fis_id           = query.VALUE(9).toInt();
    int    modul_id         = query.VALUE(10).toInt();
    int    program_id       = query.VALUE(11).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id ) {
        return fis_id;
    }

    int gun_no = MALI_YIL_ARRAY_INDIS ( fis_tarihi );

    if ( fis_turu EQ ENUM_ACILIS_FISI ) {
        gun_no = 0;
    }


    if ( fis_turu EQ ENUM_TAHSIL_FISI OR fis_turu EQ ENUM_TEDIYE_FISI ) {

        if ( gizli_satir_id > 0 ) {

            query.PREPARE_DELETE("muh_fis_satirlari" , "fis_satiri_id = :fis_satiri_id");

            query.SET_VALUE(":fis_satiri_id" , gizli_satir_id);
            query.DELETE();

            MUH_HESABININ_BAKIYELERINI_AZALT (kasa_hesabi_id , gun_no , alacak_tutari , borc_tutari, USE_DB);
        }
    }

    query.PREPARE_DELETE("muh_fis_satirlari", "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);
    query.DELETE();

    MUH_HESABININ_BAKIYELERINI_AZALT ( hesap_id , gun_no , borc_tutari , alacak_tutari, USE_DB);

    toplam_borc   = ROUND ( toplam_borc   - borc_tutari  );
    toplam_alacak = ROUND ( toplam_alacak - alacak_tutari );

    if ( fis_turu EQ ENUM_TAHSIL_FISI ) {
        toplam_borc  = ROUND ( toplam_borc - alacak_tutari );
    }
    else if ( fis_turu EQ ENUM_TEDIYE_FISI ) {
        toplam_alacak = ROUND ( toplam_alacak - borc_tutari );
    }

    query.PREPARE_UPDATE( "muh_fisler","fis_id",
                          "toplam_borc ,toplam_alacak ",
                          "fis_id = :fis_id" );

    query.SET_VALUE ( ":toplam_borc",    ROUND ( toplam_borc )   );
    query.SET_VALUE ( ":toplam_alacak",  ROUND ( toplam_alacak ) );
    query.SET_VALUE ( ":fis_id",         fis_id        );
    query.UPDATE();

    P_FIS->toplam_borc   = toplam_borc;
    P_FIS->toplam_alacak = toplam_alacak;

    return fis_id;

}
/**************************************************************************************
                                    MUH_FIS_SATIRINI_GUNCELLE
***************************************************************************************/

void MUH_FIS_SATIRINI_GUNCELLE ( MUH_FIS_STRUCT * P_FIS , MUH_FIS_SATIRI_STRUCT * P_FIS_SATIRI , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("muh_fis_satirlari,muh_fisler" , "hesap_id , borc_tutari , alacak_tutari,toplam_borc , toplam_alacak , kasa_hesabi_id,"
                         "fis_tarihi,fis_turu ,gizli_satir_id , muh_fis_satirlari.fis_id , "
                         "muh_fis_satirlari.modul_id , muh_fis_satirlari.program_id",
                         "fis_satiri_id = :fis_satiri_id AND "
                         "muh_fisler.fis_id = muh_fis_satirlari.fis_id");

    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int hesap_id                = query.VALUE ( 0 ).toInt();
    double borc_tutari          = ROUND( query.VALUE ( 1 ).toDouble() );
    double alacak_tutari        = query.VALUE ( 2 ).toDouble();
    double toplam_borc          = query.VALUE ( 3 ).toDouble();
    double toplam_alacak        = query.VALUE ( 4 ).toDouble();
    int    kasa_hesabi_id       = query.VALUE ( 5 ).toInt();
    QDate  fis_tarihi           = QDate::fromString(query.VALUE(6).toString() , "yyyy.MM.dd");
    int    fis_turu             = query.VALUE ( 7  ).toInt();
    int    gizli_satir_id       = query.VALUE ( 8  ).toInt();
    int    fis_id               = query.VALUE ( 9  ).toInt();
    int    modul_id             = query.VALUE ( 10 ).toInt();
    int    program_id           = query.VALUE ( 11 ).toInt();

    if ( P_FIS->program_id NE program_id OR P_FIS->modul_id NE modul_id ) {
        return;
    }



    bool hesap_bakiyeleri_guncellenecek = false;
    bool alacak_tutari_degisti          = false;
    bool borc_tutari_degisti            = false;

    if ( P_FIS_SATIRI->hesap_id NE hesap_id ) {
        hesap_bakiyeleri_guncellenecek = true;
    }

    if ( ( P_FIS_SATIRI->borc_tutari NE borc_tutari) AND ( P_FIS_SATIRI->alacak_tutari NE alacak_tutari) ) {
        hesap_bakiyeleri_guncellenecek = true;
    }
    else if ( P_FIS_SATIRI->alacak_tutari NE alacak_tutari ) {
       alacak_tutari_degisti = true;
    }
    else if ( P_FIS_SATIRI->borc_tutari NE borc_tutari ) {
        borc_tutari_degisti = true;
    }

    if ( hesap_bakiyeleri_guncellenecek EQ true ) {

        int gun_no = MALI_YIL_ARRAY_INDIS(fis_tarihi);

        if ( fis_turu EQ ENUM_ACILIS_FISI ) {
            gun_no = 0;
        }

        MUH_HESABININ_BAKIYELERINI_AZALT ( hesap_id , gun_no , borc_tutari , alacak_tutari , USE_DB);

        toplam_borc   = ROUND ( toplam_borc - borc_tutari     );
        toplam_alacak = ROUND ( toplam_alacak - alacak_tutari );

        if ( fis_turu EQ ENUM_TAHSIL_FISI ) {
            MUH_HESABININ_BAKIYELERINI_AZALT ( kasa_hesabi_id , gun_no , borc_tutari , 0.0, USE_DB);
        }
        else if ( fis_turu EQ ENUM_TEDIYE_FISI ) {
            MUH_HESABININ_BAKIYELERINI_AZALT ( kasa_hesabi_id , gun_no , 0.0 , alacak_tutari, USE_DB );
        }

        MUH_HESABININ_BAKIYELERINI_ARTTIR ( P_FIS_SATIRI->hesap_id , gun_no , P_FIS_SATIRI->borc_tutari ,
                                                 P_FIS_SATIRI->alacak_tutari, USE_DB);

        toplam_borc   = ROUND ( toplam_borc + P_FIS_SATIRI->borc_tutari     );
        toplam_alacak = ROUND ( toplam_alacak + P_FIS_SATIRI->alacak_tutari  );

        if ( fis_turu EQ ENUM_TAHSIL_FISI ) {
            MUH_HESABININ_BAKIYELERINI_ARTTIR ( kasa_hesabi_id , gun_no , P_FIS_SATIRI->borc_tutari , 0.0, USE_DB);
        }
        else if ( fis_turu EQ ENUM_TEDIYE_FISI ) {
            MUH_HESABININ_BAKIYELERINI_ARTTIR ( kasa_hesabi_id , gun_no , 0.0 , P_FIS_SATIRI->alacak_tutari , USE_DB );
        }

    }
    else {
        int gun_no = MALI_YIL_ARRAY_INDIS(fis_tarihi);

        if ( fis_turu EQ ENUM_ACILIS_FISI ) {
            gun_no = 0;
        }

        if ( borc_tutari_degisti EQ true ) {

            double borc_tutari_farki = ROUND ( P_FIS_SATIRI->borc_tutari - borc_tutari );

            MUH_HESABININ_BAKIYELERINI_ARTTIR ( hesap_id , gun_no , borc_tutari_farki , 0.0, USE_DB);

            toplam_borc = ROUND ( toplam_borc + borc_tutari_farki );

            if ( fis_turu EQ ENUM_TEDIYE_FISI ) {
                MUH_HESABININ_BAKIYELERINI_ARTTIR(kasa_hesabi_id , gun_no , 0.0 , borc_tutari_farki, USE_DB);
            }

        }
        else if ( alacak_tutari_degisti EQ true ) {

            double alacak_tutari_farki = ROUND ( P_FIS_SATIRI->alacak_tutari - alacak_tutari );

            MUH_HESABININ_BAKIYELERINI_ARTTIR ( hesap_id , gun_no , 0.0 , alacak_tutari_farki, USE_DB);

            toplam_alacak = ROUND ( toplam_alacak + alacak_tutari_farki );

            if ( fis_turu EQ ENUM_TAHSIL_FISI ) {
                MUH_HESABININ_BAKIYELERINI_ARTTIR ( kasa_hesabi_id , gun_no , alacak_tutari_farki , 0.0, USE_DB );
            }
        }
    }

    query.PREPARE_UPDATE("muh_fis_satirlari ","fis_satiri_id",
                         "hesap_id , aciklama ,borc_tutari  , "
                         "alacak_tutari , base_record_id ",
                         "fis_satiri_id   = :fis_satiri_id" );

    query.SET_VALUE( ":hesap_id"        , P_FIS_SATIRI->hesap_id                    );
    query.SET_VALUE( ":aciklama"        , P_FIS_SATIRI->aciklama                    );
    query.SET_VALUE( ":borc_tutari"     , ROUND ( P_FIS_SATIRI->borc_tutari )       );
    query.SET_VALUE( ":alacak_tutari"   , ROUND ( P_FIS_SATIRI->alacak_tutari )     );
    query.SET_VALUE( ":fis_satiri_id"   , p_fis_satiri_id                           );
    query.SET_VALUE( ":base_record_id"  , P_FIS_SATIRI->base_record_id);

    query.UPDATE();

    if ( fis_turu EQ ENUM_TAHSIL_FISI OR fis_turu EQ ENUM_TEDIYE_FISI ) {

        query.PREPARE_UPDATE ( "muh_fis_satirlari ","fis_satiri_id",
                               "aciklama , borc_tutari , "
                               "alacak_tutari ",
                               "fis_satiri_id   = :fis_satiri_id" );

        query.SET_VALUE( ":aciklama"        , P_FIS_SATIRI->aciklama);
        query.SET_VALUE( ":borc_tutari"     , ROUND ( P_FIS_SATIRI->alacak_tutari ));
        query.SET_VALUE( ":alacak_tutari"   , ROUND ( P_FIS_SATIRI->borc_tutari   ));
        query.SET_VALUE( ":fis_satiri_id"   , gizli_satir_id);

        query.UPDATE();
    }

    query.PREPARE_UPDATE ( "muh_fisler ","fis_id",
                           "toplam_borc , toplam_alacak " ,
                           "fis_id = :fis_id" );

    query.SET_VALUE ( ":toplam_borc"      , ROUND(toplam_borc)               );
    query.SET_VALUE ( ":toplam_alacak"    , ROUND(toplam_alacak)             );
    query.SET_VALUE ( ":fis_id"           , fis_id                    );

    query.UPDATE();

    P_FIS->toplam_borc   = toplam_borc;
    P_FIS->toplam_alacak = toplam_alacak;

}

/**************************************************************************************
                                    MUH_FIS_SIL
***************************************************************************************/

void MUH_FIS_SIL ( MUH_FIS_STRUCT * P_FIS , int p_fis_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_fisler" , "program_id,modul_id" ,"fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int program_id = query.VALUE(0).toInt();
    int modul_id   = query.VALUE(1).toInt();

    if ( P_FIS->program_id NE program_id OR P_FIS->modul_id NE modul_id ) {
        return;
    }

    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_satiri_id" , "fis_id = :fis_id AND gizli_satir_mi = :gizli_satir_mi");

    query.SET_VALUE      ( ":fis_id"         , p_fis_id );
    query.SET_VALUE      ( ":gizli_satir_mi" , 0 );

    if ( query.SELECT() > 0 ) {

        while ( query.NEXT() ) {
            int fis_satiri_id = query.VALUE(0).toInt();
            MUH_FIS_SATIRINI_SIL ( P_FIS , fis_satiri_id );
        }
    }

    query.PREPARE_DELETE ( "muh_fisler ","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id" , p_fis_id );
    query.DELETE();
}

/**************************************************************************************
                                    MUH_FISI_GUNCELLE
***************************************************************************************/

void MUH_FISI_GUNCELLE ( MUH_FIS_STRUCT *P_FIS , int p_fis_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("muh_fisler" , "fis_tarihi , kasa_hesabi_id , program_id , modul_id , fis_turu , toplam_borc , "
                         "toplam_alacak" , "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    QString     fis_tarihi              = query.VALUE ( 0 ).toString();
    int         kasa_hesabi_id          = query.VALUE ( 1 ).toInt();
    int         program_id              = query.VALUE ( 2 ).toInt();
    int         modul_id                = query.VALUE ( 3 ).toInt();
    int         fis_turu                = query.VALUE ( 4 ).toInt();
    double      toplam_borc             = query.VALUE ( 5 ).toDouble();
    double      toplam_alacak           = query.VALUE ( 6 ).toDouble();

    if ( P_FIS->program_id NE program_id OR P_FIS->modul_id NE modul_id ) {
        return;
    }

    bool kasa_hesabi_degisti_mi = false;
    bool fis_tarihi_degisti_mi  = false;
    bool entegrasyon_degisti_mi = false;

    if ( fis_tarihi NE P_FIS->fis_tarihi ) {
        fis_tarihi_degisti_mi = true;
    }
    if ( kasa_hesabi_id NE P_FIS->kasa_hesabi_id ) {
        kasa_hesabi_degisti_mi = true;
    }

    int old_gun_no = MALI_YIL_ARRAY_INDIS(QDate::fromString(fis_tarihi ,"yyyy.MM.dd"));

    int gun_no     = MALI_YIL_ARRAY_INDIS( QDate::fromString ( P_FIS->fis_tarihi , "yyyy.MM.dd" ));

    if ( fis_tarihi_degisti_mi EQ true OR entegrasyon_degisti_mi EQ true ) {

        query.PREPARE_SELECT("muh_fis_satirlari" , "hesap_id, borc_tutari, alacak_tutari",
                             "fis_id = :fis_id");

        query.SET_VALUE      ( ":fis_id", p_fis_id );

        if ( query.SELECT() NE 0 ) {

            while ( query.NEXT() EQ true ) {

                int     hesap_id      = query.VALUE ( 0 ).toInt();
                double  borc_tutari   = query.VALUE ( 1 ).toDouble();
                double  alacak_tutari = query.VALUE ( 2 ).toDouble();

                MUH_HESABININ_BAKIYELERINI_AZALT ( hesap_id , old_gun_no , borc_tutari , alacak_tutari, USE_DB);

                MUH_HESABININ_BAKIYELERINI_ARTTIR ( hesap_id , gun_no    , borc_tutari , alacak_tutari, USE_DB);
            }
        }

    }
    if ( kasa_hesabi_degisti_mi EQ true ) {

        if ( fis_turu EQ ENUM_TAHSIL_FISI ) {
            MUH_HESABININ_BAKIYELERINI_AZALT  ( kasa_hesabi_id        , old_gun_no , toplam_borc , 0.0 , USE_DB);
            MUH_HESABININ_BAKIYELERINI_ARTTIR ( P_FIS->kasa_hesabi_id , gun_no     , toplam_borc , 0.0 , USE_DB);

        }
        else if ( fis_turu EQ ENUM_TEDIYE_FISI ) {
            MUH_HESABININ_BAKIYELERINI_AZALT  ( kasa_hesabi_id        , old_gun_no , 0.0 , toplam_alacak , USE_DB);
            MUH_HESABININ_BAKIYELERINI_ARTTIR ( P_FIS->kasa_hesabi_id , gun_no     , 0.0 , toplam_borc   , USE_DB);
        }

        if ( fis_turu EQ ENUM_TAHSIL_FISI OR fis_turu EQ ENUM_TEDIYE_FISI ) {

            query.PREPARE_SELECT("muh_fis_satirlari" , "fis_satiri_id" , "fis_id = :fis_id "
                                 "AND gizli_satir_mi = :gizli_satir_mi");

            query.SET_VALUE(":fis_id"         , p_fis_id);
            query.SET_VALUE(":gizli_satir_mi" , 1);

            if ( query.SELECT() NE 0 ) {

                SQL_QUERY update_query(USE_DB);

                while ( query.NEXT() ) {

                    int fis_satiri_id = query.VALUE(0).toInt();

                    update_query.PREPARE_UPDATE("muh_fis_satirlari","fis_satiri_id", "hesap_id",
                                                "fis_satiri_id = :fis_satiri_id");
                    update_query.SET_VALUE(":hesap_id"      , P_FIS->kasa_hesabi_id);
                    update_query.SET_VALUE(":fis_satiri_id" , fis_satiri_id);
                    update_query.UPDATE();
                }
            }
        }
    }

    query.PREPARE_UPDATE ( "muh_fisler ","fis_id",
                           "fis_tarihi , fis_no ,"
                           "aciklama , kasa_hesabi_id , base_fis_id ",
                           "fis_id = :fis_id " );

    query.SET_VALUE ( ":fis_tarihi"       , P_FIS->fis_tarihi       );
    query.SET_VALUE ( ":fis_no"           , P_FIS->fis_no           );
    query.SET_VALUE ( ":aciklama"         , P_FIS->aciklama         );
    query.SET_VALUE ( ":kasa_hesabi_id"   , P_FIS->kasa_hesabi_id   );
    query.SET_VALUE ( ":base_fis_id"      , P_FIS->base_fis_id      );
    query.SET_VALUE ( ":fis_id"           , p_fis_id                );

    query.UPDATE();

}

/**************************************************************************************
                                    MUH_FIS_BILGILERINI_OKU
***************************************************************************************/

void MUH_FIS_BILGILERINI_OKU ( MUH_FIS_STRUCT * P_FIS , int p_fis_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("muh_fisler" , "fis_no, fis_tarihi, aciklama, toplam_borc, toplam_alacak, "
                         "kasa_hesabi_id, fis_turu, modul_id, program_id, base_fis_id, "
                         "yevmiye_numarasi",
                         "fis_id = :fis_id");

    query.SET_VALUE     ( ":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    P_FIS->fis_no           = query.VALUE(0).toInt();
    P_FIS->fis_tarihi       = query.VALUE(1).toString();
    P_FIS->aciklama         = query.VALUE(2).toString();
    P_FIS->toplam_borc      = query.VALUE(3).toDouble();
    P_FIS->toplam_alacak    = query.VALUE(4).toDouble();
    P_FIS->kasa_hesabi_id   = query.VALUE(5).toInt();
    P_FIS->fis_turu         = query.VALUE(6).toInt();
    P_FIS->modul_id         = query.VALUE(7).toInt();
    P_FIS->program_id       = query.VALUE(8).toInt();
    P_FIS->base_fis_id      = query.VALUE(9).toInt();
    P_FIS->yevmiye_no       = query.VALUE(10).toInt();
}

/**************************************************************************************
                                    MUH_FIS_SATIRINI_OKU
***************************************************************************************/

void MUH_FIS_SATIRINI_OKU (MUH_FIS_SATIRI_STRUCT * P_FIS_SATIRI , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB);

    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_id, hesap_id, aciklama, borc_tutari, alacak_tutari, "
                         "order_number, base_record_id, modul_id, program_id",
                         "fis_satiri_id = :fis_satiri_id");

    query.SET_VALUE      ( ":fis_satiri_id" , p_fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    P_FIS_SATIRI->fis_id               = query.VALUE(0).toInt();
    P_FIS_SATIRI->hesap_id             = query.VALUE(1).toInt();
    P_FIS_SATIRI->aciklama             = query.VALUE(2).toString();
    P_FIS_SATIRI->borc_tutari          = query.VALUE(3).toDouble();
    P_FIS_SATIRI->alacak_tutari        = query.VALUE(4).toDouble();
    P_FIS_SATIRI->order_number         = query.VALUE(5).toInt();
    P_FIS_SATIRI->base_record_id       = query.VALUE(6).toInt();
    P_FIS_SATIRI->modul_id             = query.VALUE(7).toInt();
    P_FIS_SATIRI->program_id           = query.VALUE(8).toInt();
}

/**************************************************************************************
                      MUH_FIS_TUM_SATIRLARINI_SIL
***************************************************************************************/

void MUH_FIS_TUM_SATIRLARINI_SIL ( MUH_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query         ( USE_DB );

    query.PREPARE_SELECT("muh_fisler" , "modul_id,program_id" , "fis_id = :fis_id");

    query.SET_VALUE         ( ":fis_id", p_fis_id );
    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();
    int modul_id    = query.VALUE(0).toInt();
    int program_id  = query.VALUE(1).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id ) {
        return;
    }

    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_satiri_id" , "fis_id = :fis_id");

    query.SET_VALUE      ( ":fis_id", p_fis_id );
    if ( query.SELECT() EQ 0 ) {
        return;
    }
    while ( query.NEXT() EQ true ) {
        MUH_FIS_SATIRINI_SIL ( P_FIS, query.VALUE(0).toInt() , USE_DB);
    }
}

/**************************************************************************************
                      MUH_ENTEGRASYON_FIS_SATIRLARINI_SIL
***************************************************************************************/

int MUH_ENTEGRASYON_FIS_SATIRLARINI_SIL ( MUH_FIS_STRUCT * P_FIS, int p_muh_fis_id, int p_base_record_id )
{
    SQL_QUERY query      ( DB );

    query.PREPARE_SELECT("muh_fisler" , "modul_id,program_id" , "fis_id = :fis_id");
    query.SET_VALUE      ( ":fis_id", p_muh_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    int     modul_id    =   query.VALUE ( 0 ).toInt();
    int     program_id  =   query.VALUE ( 1 ).toInt();

    if ( modul_id NE P_FIS->modul_id OR program_id NE P_FIS->program_id ) {
        return p_muh_fis_id;
    }

    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_satiri_id" , "base_record_id = :base_record_id "
                         "AND fis_id           = :fis_id         "
                         "AND modul_id         = :modul_id       "
                         "AND program_id       = :program_id");

    query.SET_VALUE      ( ":base_record_id", p_base_record_id       );
    query.SET_VALUE      ( ":fis_id",         p_muh_fis_id           );
    query.SET_VALUE      ( ":modul_id",       P_FIS->modul_id        );
    query.SET_VALUE      ( ":program_id",     P_FIS->program_id      );


    if ( query.SELECT() EQ 0 ) {
        return p_muh_fis_id;
    }

    int fis_id = p_muh_fis_id;

    // MUH FIS SATIRINI SIL SON SATIRLA BERABER FISI DE SILERSE 0 DEGERINI DONDURUR.
    // WHILE DONGUSU SONUNDA fis_id degiskenimiz 0 olmussa fis silinmis demektir.
    // GERI DONDURDUGUMUZ DEGER DIREKT OLARAK 0 VEYA fis_id OLMUS OLUYOR
    while ( query.NEXT() EQ true ) {
        fis_id = MUH_FIS_SATIRINI_SIL ( P_FIS, query.VALUE ( 0 ).toInt() );
    }
    return fis_id;
}

/**************************************************************************************
                      MUH_FISINI_HATALI_FISE_DONUSTUR
***************************************************************************************/

void MUH_FISINI_HATALI_FISE_DONUSTUR ( MUH_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query         ( USE_DB );

    query.PREPARE_SELECT("muh_fisler" , "modul_id,program_id" , "fis_id = :fis_id");
    query.SET_VALUE         ( ":fis_id", p_fis_id );
    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();
    int modul_id    = query.VALUE(0).toInt();
    int program_id  = query.VALUE(1).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id ) {
        return;
    }

    MUH_FIS_TUM_SATIRLARINI_SIL ( P_FIS, p_fis_id ,USE_DB);

    query.PREPARE_UPDATE ( "muh_fisler","fis_id","aciklama", "fis_id = :fis_id" );
    query.SET_VALUE      ( ":aciklama",  P_FIS->aciklama );
    query.SET_VALUE      ( ":fis_id",    p_fis_id        );
    query.UPDATE         ();

}

/**************************************************************************************
                      MUH_CLEAR_FIS_STRUCT
***************************************************************************************/

void MUH_CLEAR_FIS_STRUCT ( MUH_FIS_STRUCT * P_FIS )
{
    P_FIS->fis_tarihi.clear();
    P_FIS->aciklama.clear();
    P_FIS->fis_no               =  0;
    P_FIS->kasa_hesabi_id       =  0;
    P_FIS->fis_turu             = -1;
    P_FIS->modul_id             = -1;
    P_FIS->program_id           = -1;
    P_FIS->toplam_borc          =  0.0;
    P_FIS->toplam_alacak        =  0.0;
    P_FIS->base_fis_id          =  0;
    P_FIS->yevmiye_no           = 0;
}

/**************************************************************************************
                      MUH_CLEAR_FIS_SATIRI_STRUCT
***************************************************************************************/

void MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI_STRUCT * P_FIS_SATIRI )
{
    P_FIS_SATIRI->aciklama.clear();
    P_FIS_SATIRI->fis_id                =  0;
    P_FIS_SATIRI->hesap_id              =  0;
    P_FIS_SATIRI->borc_tutari           =  0.0;
    P_FIS_SATIRI->alacak_tutari         =  0.0;
    P_FIS_SATIRI->order_number          =  0;
    P_FIS_SATIRI->base_record_id        =  0;
    P_FIS_SATIRI->modul_id              = -1;
    P_FIS_SATIRI->program_id            = -1;
}

/**************************************************************************************
                      MUH_ENTEGRASYON_FIS_SATIRLARINI_GUNCELLE
***************************************************************************************/

void MUH_ENTEGRASYON_FIS_SATIRLARINI_GUNCELLE ( QString p_aciklama, int p_program_id, int p_modul_id,
                                                int p_muh_fis_id, int p_base_fis_id, int p_base_record_id,
                                                ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query         ( USE_DB);

    query.PREPARE_SELECT("muh_fisler" , "modul_id, program_id, base_fis_id" , "fis_id = :fis_id");

    query.SET_VALUE         ( ":fis_id", p_muh_fis_id );
    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();
    int modul_id    = query.VALUE(0).toInt();
    int program_id  = query.VALUE(1).toInt();
    int base_fis_id = query.VALUE(2).toInt();

    if ( modul_id NE p_modul_id OR program_id NE p_program_id  OR  p_base_fis_id  NE base_fis_id ) {
        return;
    }

    query.PREPARE_UPDATE( "muh_fis_satirlari ","fis_satiri_id",
                          "aciklama",
                          "fis_id =:fis_id AND "
                          "base_record_id =:base_record_id AND "
                          "program_id =:program_id AND "
                          "modul_id =:modul_id ");

    query.SET_VALUE     ( ":aciklama",       p_aciklama         );
    query.SET_VALUE     ( ":fis_id",         p_muh_fis_id       );
    query.SET_VALUE     ( ":base_record_id", p_base_record_id   );
    query.SET_VALUE     ( ":program_id",     p_program_id       );
    query.SET_VALUE     ( ":modul_id",       p_modul_id         );


    query.UPDATE();

}

/**************************************************************************************
                            MUH_GET_ENT_FIS_ID
***************************************************************************************/

int MUH_GET_ENT_FIS_ID ( int p_program_id, int p_modul_id, int p_base_fis_id , ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query( USE_DB );

    query.PREPARE_SELECT("muh_fisler" , "fis_id" , "program_id = :program_id "
                         "AND modul_id     = :modul_id "
                         "AND base_fis_id  = :base_fis_id");

    query.SET_VALUE( ":program_id",     p_program_id );
    query.SET_VALUE( ":modul_id",       p_modul_id );
    query.SET_VALUE( ":base_fis_id",    p_base_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE( 0 ).toInt();

}

/**************************************************************************************
                            MUH_GET_ENT_ROW_ID_LIST
***************************************************************************************/

QStringList MUH_GET_ENT_ROW_ID_LIST ( int p_program_id, int p_modul_id,
                                      int p_base_record_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }



    QStringList row_id_list;

    SQL_QUERY query( USE_DB);

    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_satiri_id" ,
                         "program_id           = :program_id "
                         "AND modul_id         = :modul_id "
                         "AND base_record_id   = :base_record_id");

    query.SET_VALUE ( ":program_id",     p_program_id      );
    query.SET_VALUE ( ":modul_id",       p_modul_id        );
    query.SET_VALUE ( ":base_record_id", p_base_record_id  );


    if ( query.SELECT() EQ 0 ) {
        return row_id_list;
    }

    while ( query.NEXT() EQ true ) {
        row_id_list << query.VALUE( 0 ).toString();
    }

    return row_id_list;
}
/**************************************************************************************
                            MUH_GET_FISE_GORE_ENT_ROW_ID_LIST
***************************************************************************************/

QStringList MUH_GET_FISE_GORE_ENT_ROW_ID_LIST(int p_program_id, int p_modul_id,
                                              int p_base_record_id, int p_fis_id,
                                              ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }


    QStringList row_id_list;

    SQL_QUERY query( USE_DB);

    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_satiri_id" ,
                         "program_id           = :program_id "
                         "AND modul_id         = :modul_id "
                         "AND base_record_id   = :base_record_id "
                         "AND fis_id = :fis_id");

    query.SET_VALUE ( ":program_id",     p_program_id      );
    query.SET_VALUE ( ":modul_id",       p_modul_id        );
    query.SET_VALUE ( ":base_record_id", p_base_record_id  );
    query.SET_VALUE ( ":fis_id"        , p_fis_id);


    if ( query.SELECT() EQ 0 ) {
        return row_id_list;
    }

    while ( query.NEXT() EQ true ) {
        row_id_list << query.VALUE( 0 ).toString();
    }

    return row_id_list;
}

/**************************************************************************************
                            MUH_GET_ENT_FIS_ID_FOR_BASE_RECORD_ID
***************************************************************************************/

int MUH_GET_ENT_FIS_ID_FOR_BASE_RECORD_ID ( int p_program_id, int p_modul_id, int p_base_record_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_id" ,
                         "program_id        = :program_id "
                         "AND   modul_id          = :modul_id "
                         "AND   base_record_id    = :base_record_id");

    query.SET_VALUE ( ":program_id",        p_program_id      );
    query.SET_VALUE ( ":modul_id",          p_modul_id        );
    query.SET_VALUE ( ":base_record_id",    p_base_record_id  );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();
    return query.VALUE ( 0 ).toInt();
}

