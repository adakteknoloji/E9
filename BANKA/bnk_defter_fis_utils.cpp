#include "adak_sql.h"
#include "bnk_defter_fis_utils.h"
#include "e9_console_utils.h"
#include "banka_enum.h"
#include "bnk_console_utils.h"
#include "sube_console_utils.h"
#include "muh_fis_utils.h"
#include "cari_fis_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"

extern      ADAK_SQL   *   DB;

/***************************************************************************************
                            BNK_DEFTER_FIS_EKLE
***************************************************************************************/

int  BNK_DEFTER_FIS_EKLE( BNK_DEFTER_FIS_STRUCT * P_FIS , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    if ( P_FIS->fis_no EQ 0 ) {
        P_FIS->fis_no  = BNK_SIRADAKI_DEFTER_FIS_NO_AL( P_FIS->fis_tarihi , USE_DB);
    }

    QString aciklama = P_FIS->aciklama;

    if ( P_FIS->aciklama.size() > 128 ) {
        aciklama.resize( 128 );
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_INSERT("bnk_defter_fisler","fis_id" ," fis_no, fis_tarihi, aciklama,           "
                         "hesap_no_id, fis_turu, program_id,                                     "
                         "modul_id, para_birim_id, base_fis_id,                                  "
                         "toplam_hesaba_giren, toplam_hesaptan_cikan, base_fis_turu, muh_fis_id ,"
                         "odm_emr_hesabi_etkilenecek_mi                                         ");

    int para_birim_id   = BNK_GET_PARA_BIRIM_ID( P_FIS->hesap_no_id );

    query.SET_VALUE( ":fis_no"                , P_FIS->fis_no            );
    query.SET_VALUE( ":fis_tarihi"            , P_FIS->fis_tarihi        );
    query.SET_VALUE( ":aciklama"              , aciklama                 );
    query.SET_VALUE( ":hesap_no_id"           , P_FIS->hesap_no_id       );
    query.SET_VALUE( ":fis_turu"              , P_FIS->fis_turu          );
    query.SET_VALUE( ":program_id"            , P_FIS->program_id        );
    query.SET_VALUE( ":modul_id"              , P_FIS->modul_id          );
    query.SET_VALUE( ":para_birim_id"         , para_birim_id            );
    query.SET_VALUE( ":base_fis_id"           , P_FIS->base_fis_id       );
    query.SET_VALUE( ":toplam_hesaba_giren"   , 0 );
    query.SET_VALUE( ":toplam_hesaptan_cikan" , 0 );
    query.SET_VALUE( ":base_fis_turu"         , P_FIS->base_fis_turu     );
    query.SET_VALUE( ":muh_fis_id"            , P_FIS->muh_fis_id        );
    query.SET_VALUE( ":odm_emr_hesabi_etkilenecek_mi", P_FIS->odm_emr_hesabi_etkilenecek_mi );

    int def_fis_id = query.INSERT();

    return def_fis_id;
}

/***************************************************************************************
                            BNK_DEFTER_FIS_SATIRI_EKLE
***************************************************************************************/

int BNK_DEFTER_FIS_SATIRI_EKLE ( BNK_DEFTER_FIS_STRUCT * P_FIS, BNK_DEFTER_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id , bool base_fis_satiri_eklenecek ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query      ( USE_DB );

    query.PREPARE_SELECT ( "bnk_defter_fisler ","toplam_hesaba_giren,toplam_hesaptan_cikan,   "
                           "fis_tarihi, last_order_number, base_fis_id,         "
                           "modul_id, program_id, para_birim_id                 ",
                           "fis_id = :fis_id       " );

    query.SET_VALUE      ( ":fis_id" , P_FIS_SATIRI->fis_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    double    toplam_hesaba_giren       = query.VALUE ( 0 ).toDouble();
    double    toplam_hesaptan_cikan     = query.VALUE ( 1 ).toDouble();
    QString   tarih                     = query.VALUE ( 2 ).toString();
    int       last_order_number         = query.VALUE ( 3 ).toInt();
    int       base_fis_id               = query.VALUE ( 4 ).toInt();
    int       modul_id                  = query.VALUE ( 5 ).toInt();
    int       program_id                = query.VALUE ( 6 ).toInt();
    int       para_birim_id             = query.VALUE ( 7 ).toInt();
    int       gun_no                    = MALI_YIL_ARRAY_INDIS ( QDate::fromString( tarih, "yyyy.MM.dd" ) );


    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR P_FIS->base_fis_id NE base_fis_id ) {
        return 0;
    }

    if ( P_FIS_SATIRI->order_number EQ 0 ) {
        P_FIS_SATIRI->order_number = last_order_number + 1024;
    }


    double hesaba_giren     = 0.00;
    double hesaptan_cikan   = 0.00;


    hesaba_giren    = P_FIS_SATIRI->hesaba_giren;
    hesaptan_cikan  = P_FIS_SATIRI->hesaptan_cikan;

    int satir_id = p_fis_satiri_id;

    if ( base_fis_satiri_eklenecek EQ true ) {

        query.PREPARE_INSERT ( "bnk_defter_fis_satirlari","fis_satiri_id" ,
                               "fis_id, aciklama,                         "
                               "hesaba_giren,hesaptan_cikan,              "
                               "base_record_id,                           "
                               "order_number, hesap_turu, hesap_id,       "
                               "program_id, modul_id                     ");

        query.SET_VALUE ( ":fis_id"                  , P_FIS_SATIRI->fis_id                       );
        query.SET_VALUE ( ":aciklama"                , P_FIS_SATIRI->aciklama                     );
        query.SET_VALUE ( ":hesaba_giren"            , ROUND( hesaba_giren )                      );
        query.SET_VALUE ( ":hesaptan_cikan"          , ROUND( hesaptan_cikan )                    );
        query.SET_VALUE ( ":base_record_id"          , P_FIS_SATIRI->base_record_id               );
        query.SET_VALUE ( ":order_number"            , P_FIS_SATIRI->order_number                 );
        query.SET_VALUE ( ":hesap_turu"              , P_FIS_SATIRI->hesap_turu                   );
        query.SET_VALUE ( ":hesap_id"                , P_FIS_SATIRI->hesap_id                     );
        query.SET_VALUE ( ":program_id"              , P_FIS_SATIRI->program_id                   );
        query.SET_VALUE ( ":modul_id"                , P_FIS_SATIRI->modul_id                     );

        satir_id = query.INSERT();
    }

    if ( P_FIS->modul_id EQ BANKA_MODULU ) {
        if ( base_fis_id EQ 0 ) {
            base_fis_id = P_FIS_SATIRI->fis_id;
        }
        if ( P_FIS_SATIRI->base_record_id EQ 0 ) {
            P_FIS_SATIRI->base_record_id = satir_id;
        }
    }

    if ( P_FIS->fis_turu EQ ENUM_BANKA_ACILIS_FISI ) {
        gun_no = 0;
    }

    int hesap_no_id = P_FIS->hesap_no_id;

    if ( P_FIS->fis_turu EQ ENUM_BANKA_VIRMAN_FISI ) {
        hesap_no_id = P_FIS_SATIRI->hesap_id;
    }


    switch ( P_FIS_SATIRI->hesap_turu ) {

        case ENUM_BNK_KREDI_KARTI_HESABI:
                BANKA_HESAP_BAKIYELERINI_ARTTIR ( gun_no, hesap_no_id, hesaba_giren, hesaptan_cikan,
                                                  ENUM_BNK_KREDI_KARTI_HESABI, USE_DB);
            break;
        case ENUM_BNK_POS_HESABI:
                BANKA_HESAP_BAKIYELERINI_ARTTIR ( gun_no, hesap_no_id, hesaba_giren, hesaptan_cikan,
                                                  ENUM_BNK_POS_HESABI, USE_DB);

           break;
        case ENUM_BNK_BANKA_HESABI:
                BANKA_HESAP_BAKIYELERINI_ARTTIR ( gun_no, hesap_no_id, hesaba_giren, hesaptan_cikan,
                                                  ENUM_BNK_BANKA_HESABI , USE_DB);
           break;
        default:
            break;
    }

    // Muhasebeye temel para birimi cinsinden gitmesi gerekir...
    if (  para_birim_id NE DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {

        if ( P_FIS_SATIRI->hesaba_giren > 0.00 ) {
            hesaba_giren    = ROUND( P_FIS_SATIRI->hesaba_giren * P_FIS_SATIRI->doviz_kuru );
            hesaptan_cikan  = 0.00;
        }
        else {
            hesaba_giren    = 0.00;
            hesaptan_cikan  = ROUND( P_FIS_SATIRI->hesaptan_cikan * P_FIS_SATIRI->doviz_kuru );
        }

    }
    else {
        hesaba_giren    = P_FIS_SATIRI->hesaba_giren;
        hesaptan_cikan  = P_FIS_SATIRI->hesaptan_cikan;
    }

    if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {

        // muhasebe kodu tespit ediliyor
        int muh_id = 0;
        switch ( P_FIS_SATIRI->hesap_turu ) {

            case    ENUM_BNK_BANKA_HESABI :
                query.PREPARE_SELECT ( "bnk_hesaplar","muh_hesap_id","hesap_no_id = :hesap_no_id");
                query.SET_VALUE      ( ":hesap_no_id" , P_FIS_SATIRI->hesap_id );
                if ( query.SELECT() EQ  0 ) {
                    break;
                }
                query.NEXT();
                muh_id = query.VALUE(0).toInt();
                break;
            case    ENUM_BNK_POS_HESABI :
                query.PREPARE_SELECT ( "bnk_hesabin_poslari","muh_hesap_id","pos_id = :pos_id AND hesap_no_id = :hesap_no_id");
                query.SET_VALUE      ( ":pos_id"      , P_FIS_SATIRI->hesap_id );
                query.SET_VALUE      ( ":hesap_no_id" , P_FIS->hesap_no_id );
                if ( query.SELECT() EQ 0 ) {
                    break;
                }
                query.NEXT();
                muh_id = query.VALUE(0).toInt();
                break;
            case    ENUM_BNK_KREDI_KARTI_HESABI :
                query.PREPARE_SELECT ( "bnk_hesabin_kredi_kartlari","muh_hesap_id","kredi_karti_id = :kredi_karti_id AND hesap_no_id = :hesap_no_id ");
                query.SET_VALUE      ( ":kredi_karti_id"  , P_FIS_SATIRI->hesap_id );
                query.SET_VALUE      ( ":hesap_no_id"     , P_FIS->hesap_no_id );
                if ( query.SELECT() EQ  0 ) {
                    break;
                }
                query.NEXT();
                muh_id = query.VALUE(0).toInt();
                break;

            case ENUM_BNK_KASA_HESABI:
                muh_id  = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();
                break;

            case ENUM_BNK_MUHASEBE_HESABI:

                muh_id = P_FIS_SATIRI->hesap_id;
                break;
            case    ENUM_BNK_POS_KOMISYON_HESABI :
                query.PREPARE_SELECT ( "bnk_hesabin_poslari ","kom_hesap_id","pos_id = :pos_id AND hesap_no_id = :hesap_no_id");
                query.SET_VALUE      ( ":pos_id"      , P_FIS_SATIRI->hesap_id );
                query.SET_VALUE      ( ":hesap_no_id" , P_FIS->hesap_no_id );
                if ( query.SELECT() EQ 0 ) {
                    break;
                }
                query.NEXT();
                muh_id = query.VALUE(0).toInt();
                break;

            default :
                break;
        }


        double borc_tutari      = 0.00;
        double alacak_tutari    = 0.00;
        alacak_tutari           = hesaptan_cikan;
        borc_tutari             = hesaba_giren;

        MUH_FIS_STRUCT         * MUH_FIS          = new MUH_FIS_STRUCT;
        MUH_FIS_SATIRI_STRUCT  * MUH_FIS_SATIRI   = new MUH_FIS_SATIRI_STRUCT;

        // [ 1 ]

        if ( P_FIS->odm_emr_hesabi_etkilenecek_mi EQ 1 ) {
            if ( P_FIS_SATIRI->hesap_turu EQ ENUM_BNK_BANKA_HESABI ) {


                if ( alacak_tutari > 0.00 ) { // bankadan para cikiyor

                    // [ 2 ] Odeme emirleri ne alacak sonra  borc

                    SQL_QUERY f_query( DB );

                    int odeme_emirleri_hesap_id = 0;

                    f_query.PREPARE_SELECT( "bnk_hesaplar","odeme_emirleri_hesap_id","hesap_no_id = :hesap_no_id" );
                    f_query.SET_VALUE     ( ":hesap_no_id", P_FIS->hesap_no_id );

                    if ( f_query.SELECT() NE 0 ) {

                        f_query.NEXT();

                        odeme_emirleri_hesap_id = f_query.VALUE( 0 ).toInt();

                        MUH_CLEAR_FIS_STRUCT        ( MUH_FIS );
                        MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI );


                        MUH_FIS_BILGILERINI_OKU ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);

                        MUH_FIS_SATIRI->aciklama             = P_FIS_SATIRI->aciklama;
                        MUH_FIS_SATIRI->borc_tutari          = borc_tutari;
                        MUH_FIS_SATIRI->alacak_tutari        = alacak_tutari;
                        MUH_FIS_SATIRI->fis_id               = P_FIS->muh_fis_id;
                        MUH_FIS_SATIRI->hesap_id             = odeme_emirleri_hesap_id;
                        MUH_FIS_SATIRI->base_record_id       = P_FIS_SATIRI->base_record_id;
                        MUH_FIS_SATIRI->program_id           = P_FIS_SATIRI->program_id;
                        MUH_FIS_SATIRI->modul_id             = P_FIS_SATIRI->modul_id;

                        // hesaba giren
                        MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI ,USE_DB);

                        MUH_FIS_SATIRI->borc_tutari          = alacak_tutari;
                        MUH_FIS_SATIRI->alacak_tutari        = borc_tutari;
                        // hesaptan cikan
                        MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI ,USE_DB);

                    }
                    // [ 2 ]
                }
            }
        }
        // [ 1 ]

        //Muhasebe modulune fis_satiri uretiliyor

        MUH_CLEAR_FIS_STRUCT        ( MUH_FIS );
        MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI );

        MUH_FIS_BILGILERINI_OKU ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);

        MUH_FIS_SATIRI->aciklama             = P_FIS_SATIRI->aciklama;
        MUH_FIS_SATIRI->borc_tutari          = borc_tutari;
        MUH_FIS_SATIRI->alacak_tutari        = alacak_tutari;
        MUH_FIS_SATIRI->fis_id               = P_FIS->muh_fis_id;
        MUH_FIS_SATIRI->hesap_id             = muh_id;
        MUH_FIS_SATIRI->base_record_id       = P_FIS_SATIRI->base_record_id;
        MUH_FIS_SATIRI->program_id           = P_FIS_SATIRI->program_id;
        MUH_FIS_SATIRI->modul_id             = P_FIS_SATIRI->modul_id;

        MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI ,USE_DB);

        delete MUH_FIS;
        delete MUH_FIS_SATIRI;

    }

    if ( base_fis_satiri_eklenecek EQ true ) {

        toplam_hesaba_giren   = ROUND ( toplam_hesaba_giren   + P_FIS_SATIRI->hesaba_giren  );
        toplam_hesaptan_cikan = ROUND ( toplam_hesaptan_cikan + hesaptan_cikan );
        // toplam degerler guncelleniyor
        query.PREPARE_UPDATE (  "bnk_defter_fisler","fis_id","toplam_hesaba_giren, toplam_hesaptan_cikan ",
                                "fis_id = :fis_id" );

        query.SET_VALUE( ":toplam_hesaba_giren"   , toplam_hesaba_giren   );
        query.SET_VALUE( ":toplam_hesaptan_cikan" , toplam_hesaptan_cikan );
        query.SET_VALUE( ":fis_id"                , P_FIS_SATIRI->fis_id  );
        query.UPDATE   ();

        P_FIS->toplam_hesaba_giren   = toplam_hesaba_giren;
        P_FIS->toplam_hesaptan_cikan = toplam_hesaptan_cikan;
    }
    return satir_id;

}

/**************************************************************************************
                        BNK_DEFTER_FISI_SIL
***************************************************************************************/

void BNK_DEFTER_FISI_SIL( BNK_DEFTER_FIS_STRUCT * P_FIS, int p_fis_id )
{
    SQL_QUERY query ( DB );
    query.PREPARE_SELECT ( "bnk_defter_fisler","modul_id, program_id,base_fis_id ",
                           "fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id" , p_fis_id );

   if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int         modul_id           =   query.VALUE ( 0 ).toInt();
    int         program_id         =   query.VALUE ( 1 ).toInt();
    int         base_fis_id        =   query.VALUE ( 2 ).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR P_FIS->base_fis_id NE base_fis_id ) {
        return;
    }

    query.PREPARE_SELECT ( "bnk_defter_fis_satirlari","fis_satiri_id","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id", p_fis_id );

    // Fis'e ait satirlar siliniyor
    if ( query.SELECT() > 0 ) {
        while ( query.NEXT() EQ true ) {
            BNK_DEFTER_FIS_SATIRI_SIL ( P_FIS, query.VALUE ( 0 ).toInt() );
        }
    }

    query.PREPARE_DELETE ( "bnk_defter_fisler","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id", p_fis_id );
    query.DELETE();
}

/**************************************************************************************
                        BNK_DEFTER_FIS_SATIRI_SIL
***************************************************************************************/

void BNK_DEFTER_FIS_SATIRI_SIL ( BNK_DEFTER_FIS_STRUCT * P_FIS, int p_fis_satiri_id  ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB ;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );
    query.PREPARE_SELECT ( "bnk_defter_fisler, bnk_defter_fis_satirlari",
                           "bnk_defter_fisler.fis_id, fis_tarihi, hesaba_giren, hesaptan_cikan,             "
                           "toplam_hesaba_giren, toplam_hesaptan_cikan, bnk_defter_fis_satirlari.modul_id,  "
                           "bnk_defter_fis_satirlari.program_id, hesap_turu,                                "
                           "base_fis_id , fis_turu , bnk_defter_fis_satirlari.hesap_id,base_record_id",
                           "bnk_defter_fisler.fis_id = bnk_defter_fis_satirlari.fis_id                      "
                           "AND fis_satiri_id  = :fis_satiri_id                                             " );

    query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int     fis_id                          =  query.VALUE( 0  ).toInt();
    QString fis_tarihi                      =  query.VALUE( 1  ).toString();
    double  hesaba_giren                    =  query.VALUE( 2  ).toDouble();
    double  hesaptan_cikan                  =  query.VALUE( 3  ).toDouble();
    double  toplam_hesaba_giren             =  query.VALUE( 4  ).toDouble();
    double  toplam_hesaptan_cikan           =  query.VALUE( 5  ).toDouble();
    int     modul_id                        =  query.VALUE( 6  ).toInt();
    int     program_id                      =  query.VALUE( 7  ).toInt();
    int     hesap_turu                      =  query.VALUE( 8  ).toInt();
    int     base_fis_id                     =  query.VALUE( 9  ).toInt();
    int     fis_turu                        =  query.VALUE( 10 ).toInt();
    int     hesap_id                        =  query.VALUE( 11 ).toInt();
    int     base_record_id                  =  query.VALUE(12).toInt();

    if ( modul_id NE P_FIS->modul_id OR program_id NE P_FIS->program_id OR P_FIS->base_fis_id NE base_fis_id) {
        return;
    }
    int gun_no =  MALI_YIL_ARRAY_INDIS ( QDate::fromString ( fis_tarihi, "yyyy.MM.dd" ) );

    if ( fis_turu EQ ENUM_BANKA_ACILIS_FISI ) {
        gun_no = 0;
    }

    int hesap_no_id = P_FIS->hesap_no_id;
    if ( P_FIS->fis_turu EQ ENUM_BANKA_VIRMAN_FISI ) {
        hesap_no_id = hesap_id;
    }
    // bakiyeler azaltma islemi yapiliyor
    switch ( hesap_turu ) {
        case ENUM_BNK_KREDI_KARTI_HESABI:
                BANKA_HESAP_BAKIYELERINI_AZALT ( gun_no, hesap_no_id ,hesaba_giren, hesaptan_cikan,ENUM_BNK_KREDI_KARTI_HESABI, USE_DB);
            break;
        case ENUM_BNK_POS_HESABI :
                BANKA_HESAP_BAKIYELERINI_AZALT ( gun_no, hesap_no_id, hesaba_giren, hesaptan_cikan,ENUM_BNK_POS_HESABI, USE_DB);
           break;
        case ENUM_BNK_BANKA_HESABI:
                BANKA_HESAP_BAKIYELERINI_AZALT ( gun_no, hesap_no_id, hesaba_giren, hesaptan_cikan,ENUM_BNK_BANKA_HESABI, USE_DB) ;
           break;
    default: break;
    }

    if ( fis_turu NE ENUM_BANKA_ACILIS_FISI ) {

        switch ( hesap_turu )
        {
        case ENUM_BNK_BANKA_HESABI:
        case ENUM_BNK_KREDI_KARTI_HESABI:
        case ENUM_BNK_POS_HESABI:
            {    
                 QStringList muh_row_list = MUH_GET_FISE_GORE_ENT_ROW_ID_LIST( P_FIS->program_id, P_FIS->modul_id, base_record_id ,P_FIS->muh_fis_id,USE_DB);

                 if ( muh_row_list.count() > 0 ) {

                     MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;
                     MUH_FIS->modul_id         = P_FIS->modul_id;
                     MUH_FIS->program_id       = P_FIS->program_id;
                     MUH_FIS->base_fis_id      = P_FIS->base_fis_id;

                     for ( int i = 0; i< muh_row_list.count(); i++ ) {
                             MUH_FIS_SATIRINI_SIL( MUH_FIS, QVariant ( muh_row_list.at( i ) ).toInt() ,USE_DB);
                     }

                     delete MUH_FIS;
                 }
            }
            break;
         }
    }

    query.PREPARE_DELETE ( "bnk_defter_fis_satirlari","fis_satiri_id = :fis_satiri_id" );
    query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );
    query.DELETE();

    P_FIS->toplam_hesaba_giren   = ROUND ( toplam_hesaba_giren   - hesaba_giren  );
    P_FIS->toplam_hesaptan_cikan = ROUND ( toplam_hesaptan_cikan - hesaptan_cikan);

    query.PREPARE_UPDATE ( "bnk_defter_fisler","fis_id","toplam_hesaba_giren ,     "
                           "toplam_hesaptan_cikan  ",
                           "fis_id = :fis_id");

    query.SET_VALUE      ( ":toplam_hesaba_giren"   , P_FIS->toplam_hesaba_giren    );
    query.SET_VALUE      ( ":toplam_hesaptan_cikan" , P_FIS->toplam_hesaptan_cikan  );
    query.SET_VALUE      ( ":fis_id"                , fis_id                        );
    query.UPDATE();

}

/**************************************************************************************
                        CLEAR_BNK_DEFTER_FIS_STRUCT
***************************************************************************************/

void CLEAR_BNK_DEFTER_FIS_STRUCT( BNK_DEFTER_FIS_STRUCT *P_FIS )
{
    P_FIS->modul_id                         = -1;
    P_FIS->program_id                       = -1;
    P_FIS->base_fis_id                      = -1;
    P_FIS->fis_no                           =  0;
    P_FIS->toplam_hesaba_giren              =  0.00;
    P_FIS->toplam_hesaptan_cikan            =  0.00;
    P_FIS->fis_tarihi.clear();
    P_FIS->hesap_no_id                      =  0;
    P_FIS->aciklama.clear();
    P_FIS->last_order_number                =  0;
    P_FIS->base_fis_turu                    =  0;
    P_FIS->muh_fis_id                       =  0;
    P_FIS->odm_emr_hesabi_etkilenecek_mi  = 0;
}

/**************************************************************************************
                        CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT
***************************************************************************************/

void CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT( BNK_DEFTER_FIS_SATIRI_STRUCT *P_FIS_SATIRI )
{
    P_FIS_SATIRI->fis_id                = 0;
    P_FIS_SATIRI->aciklama.clear();
    P_FIS_SATIRI->hesaba_giren          = 0.00;
    P_FIS_SATIRI->hesaptan_cikan        = 0.00;
    P_FIS_SATIRI->order_number          = 0;
    P_FIS_SATIRI->hesap_turu            = 0;
    P_FIS_SATIRI->hesap_id              = 0;
    P_FIS_SATIRI->program_id            = -1;
    P_FIS_SATIRI->modul_id              = -1;
    P_FIS_SATIRI->base_record_id        = -1;
}

/**************************************************************************************
                        BNK_DEFTER_FIS_BILGILERINI_OKU
***************************************************************************************/

void  BNK_DEFTER_FIS_BILGILERINI_OKU ( BNK_DEFTER_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB)
{
    /*
        Banka Defter Fisine  makro_fisi ve hareket_fisinden iki ayri fis gelmektedir
        defter fisi bunlari kendi icinde modul_id olarak ayirir

        modul_id = enum_banka_hareket_fisi;
        modul_id = enum_banka_makr0_fisi;

        diye bunlar alt module iletmez sadece alt module  BANKA_MODULU olarak iletir
     */

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY  query( USE_DB );

    query.PREPARE_SELECT( "bnk_defter_fisler","fis_no, fis_tarihi, program_id, modul_id, hesap_no_id, aciklama, "
                          "toplam_hesaba_giren, toplam_hesaptan_cikan, fis_turu, "
                          "last_order_number, base_fis_id, base_fis_turu, muh_fis_id, odm_emr_hesabi_etkilenecek_mi ",
                          "fis_id = :fis_id" );

    query.SET_VALUE( ":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    P_FIS->fis_no                           = query.VALUE( 0  ).toInt();
    P_FIS->fis_tarihi                       = query.VALUE( 1  ).toString();
    P_FIS->program_id                       = query.VALUE( 2  ).toInt();
    P_FIS->modul_id                         = query.VALUE( 3  ).toInt();
    P_FIS->hesap_no_id                      = query.VALUE( 4  ).toInt();
    P_FIS->aciklama                         = query.VALUE( 5  ).toString();
    P_FIS->toplam_hesaba_giren              = query.VALUE( 6  ).toDouble();
    P_FIS->toplam_hesaptan_cikan            = query.VALUE( 7  ).toDouble();
    P_FIS->fis_turu                         = query.VALUE( 8  ).toInt();
    P_FIS->last_order_number                = query.VALUE( 9  ).toInt();
    P_FIS->base_fis_id                      = query.VALUE( 10 ).toInt();
    P_FIS->base_fis_turu                    = query.VALUE( 11 ).toInt();
    P_FIS->muh_fis_id                       = query.VALUE( 12 ).toInt();
    P_FIS->odm_emr_hesabi_etkilenecek_mi    = query.VALUE( 13 ).toInt();
}

/**************************************************************************************
                       BNK_DEFTER_FIS_SATIR_BILGILERINI_OKU
***************************************************************************************/

void BNK_DEFTER_FIS_SATIR_BILGILERINI_OKU( BNK_DEFTER_FIS_SATIRI_STRUCT * P_FIS_SATIRI  , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB  =P_GELEN_DB;
    }

    SQL_QUERY  query( USE_DB );
    query.PREPARE_SELECT( "bnk_defter_fis_satirlari",
                          "fis_id, aciklama, hesaba_giren, hesaptan_cikan, base_record_id, "
                          "order_number, hesap_turu, hesap_id,                             "
                          "program_id, modul_id                                            ",
                          "fis_satiri_id =:fis_satiri_id      " );
    query.SET_VALUE( ":fis_satiri_id", p_fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    P_FIS_SATIRI->fis_id                   = query.VALUE( 0  ).toInt();
    P_FIS_SATIRI->aciklama                 = query.VALUE( 1  ).toString();
    P_FIS_SATIRI->hesaba_giren             = query.VALUE( 2  ).toDouble();
    P_FIS_SATIRI->hesaptan_cikan           = query.VALUE( 3  ).toDouble();
    P_FIS_SATIRI->base_record_id           = query.VALUE( 4  ).toInt();
    P_FIS_SATIRI->order_number             = query.VALUE( 5  ).toInt();
    P_FIS_SATIRI->hesap_turu               = query.VALUE( 6  ).toInt();
    P_FIS_SATIRI->hesap_id                 = query.VALUE( 7  ).toInt();
    P_FIS_SATIRI->program_id               = query.VALUE( 8  ).toInt();
    P_FIS_SATIRI->modul_id                 = query.VALUE( 9  ).toInt();
}

/**************************************************************************************
                        BNK_DEFTER_FISI_GUNCELLE
***************************************************************************************/

void BNK_DEFTER_FISI_GUNCELLE ( BNK_DEFTER_FIS_STRUCT * P_FIS, int p_fis_id )
{
    SQL_QUERY query ( DB );
    query.PREPARE_SELECT ( "bnk_defter_fisler","modul_id, program_id, hesap_no_id, base_fis_id",
                           "fis_id = :fis_id " );
    query.SET_VALUE      ( ":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int         modul_id     = query.VALUE( 0 ).toInt();
    int         program_id   = query.VALUE( 1 ).toInt();
    int         hesap_no_id  = query.VALUE( 2 ).toInt();
    int         base_fis_id  = query.VALUE( 3 ).toInt();

    int         banka_para_birimi_id  = 0;
    QString     banka_para_birimi_kodu;

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR P_FIS->base_fis_id NE base_fis_id ) {
        return;
    }

    query.PREPARE_SELECT ( "bnk_hesaplar","doviz_id","hesap_no_id = :hesap_no_id" );
    query.SET_VALUE      ( ":hesap_no_id", hesap_no_id );

    if ( query.SELECT() > 0 ) {
        query.NEXT();
        banka_para_birimi_id   = query.VALUE ( 0 ).toInt();
        banka_para_birimi_kodu = DVZ_GET_DOVIZ_KODU ( banka_para_birimi_id );
    }

    query.PREPARE_SELECT( "bnk_defter_fisler","hesap_no_id , fis_tarihi","fis_id = :fis_id" );
    query.SET_VALUE     ( ":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int old_hesap_no_id         = query.VALUE( 0 ).toInt();
    QString old_fis_tarihi      = query.VALUE( 1 ).toString();


    if ( old_hesap_no_id      NE P_FIS->hesap_no_id OR
         old_fis_tarihi       NE P_FIS->fis_tarihi )
    {

        if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {


            BNK_DEFTER_FIS_STRUCT        * DEF_FIS        = new BNK_DEFTER_FIS_STRUCT;
            BNK_DEFTER_FIS_SATIRI_STRUCT * DEF_FIS_SATIRI = new BNK_DEFTER_FIS_SATIRI_STRUCT;

            CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT( DEF_FIS_SATIRI );
            CLEAR_BNK_DEFTER_FIS_STRUCT       ( DEF_FIS );

            BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, p_fis_id );

            query.PREPARE_UPDATE( "bnk_defter_fisler","fis_id","fis_tarihi,"
                                  "hesap_no_id,                      "
                                  "odm_emr_hesabi_etkilenecek_mi",
                                  "fis_id = :fis_id " );

            query.SET_VALUE     ( ":fis_tarihi",                        P_FIS->fis_tarihi                       );
            query.SET_VALUE     ( ":hesap_no_id",                       P_FIS->hesap_no_id                      );
            query.SET_VALUE     ( ":odm_emr_hesabi_etkilenecek_mi",     P_FIS->odm_emr_hesabi_etkilenecek_mi    );
            query.SET_VALUE     ( ":fis_id",                            p_fis_id                                );
            query.UPDATE();

            MUH_FIS_STRUCT * MUH_FIS    = new MUH_FIS_STRUCT;
            MUH_CLEAR_FIS_STRUCT( MUH_FIS );

            MUH_FIS_BILGILERINI_OKU( MUH_FIS, P_FIS->muh_fis_id );

            MUH_FIS->fis_tarihi         = P_FIS->fis_tarihi;

            if ( P_FIS->modul_id EQ BANKA_MODULU ) {
                MUH_FIS->aciklama           = QString ( QObject::tr ( "Fiş BANKA MODÜLÜ ( %2 ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_FIS->fis_no ).toInt() )
                                              .arg(BNK_GET_FIS_TURLERI_STRING(P_FIS->fis_turu)) + P_FIS->aciklama;
            }
            else {
                MUH_FIS->aciklama = P_FIS->aciklama;
            }

            MUH_FISI_GUNCELLE( MUH_FIS, P_FIS->muh_fis_id );


            query.PREPARE_SELECT( "bnk_defter_fis_satirlari","fis_satiri_id","fis_id = :fis_id ");
            query.SET_VALUE     ( ":fis_id", p_fis_id );

            int def_fis_satir_id    = 0;
            if ( query.SELECT() NE 0 ) {

                while ( query.NEXT() EQ true ) {
                    def_fis_satir_id = query.VALUE( 0 ).toInt();
                    BNK_DEFTER_FIS_SATIR_BILGILERINI_OKU( DEF_FIS_SATIRI, def_fis_satir_id );
                    BNK_DEFTER_FIS_SATIRI_SIL ( DEF_FIS, def_fis_satir_id );

                    if ( DEF_FIS_SATIRI->hesap_turu EQ ENUM_BNK_BANKA_HESABI ) {
                        DEF_FIS_SATIRI->hesap_id = P_FIS->hesap_no_id;
                    }

                    BNK_DEFTER_FIS_SATIRI_EKLE( P_FIS, DEF_FIS_SATIRI );
                }
            }
            delete DEF_FIS_SATIRI;
        }
        else {

        }

        query.PREPARE_UPDATE("bnk_defter_fisler","fis_id","aciklama,                    "
                             "hesap_no_id   ,   "
                             "fis_tarihi     ,        "
                             "odm_emr_hesabi_etkilenecek_mi  ",
                             "fis_id              = :fis_id                       ");

        query.SET_VALUE( ":aciklama"                        , P_FIS->aciklama                   );
        query.SET_VALUE( ":hesap_no_id"                     , P_FIS->hesap_no_id                );
        query.SET_VALUE( ":fis_tarihi"                      , P_FIS->fis_tarihi                 );
        query.SET_VALUE( ":odm_emr_hesabi_etkilenecek_mi"   , P_FIS->odm_emr_hesabi_etkilenecek_mi);
        query.SET_VALUE( ":fis_id"                          , p_fis_id                          );

        query.UPDATE();

    }
    else {
        // sadece aciklama degismistir

        query.PREPARE_UPDATE("bnk_defter_fisler","fis_id",
                             "aciklama",
                             "fis_id = :fis_id" );

        query.SET_VALUE( ":aciklama"                        , P_FIS->aciklama  );
        query.SET_VALUE( ":fis_id"                          , p_fis_id         );


        MUH_FIS_STRUCT         * MUH_FIS          = new MUH_FIS_STRUCT;
        MUH_FIS_SATIRI_STRUCT  * MUH_FIS_SATIRI   = new MUH_FIS_SATIRI_STRUCT;

        MUH_CLEAR_FIS_STRUCT        ( MUH_FIS );
        MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI );

        MUH_FIS_BILGILERINI_OKU ( MUH_FIS, P_FIS->muh_fis_id );

        if ( P_FIS->modul_id EQ BANKA_MODULU ) {
            MUH_FIS->aciklama           = QString ( QObject::tr ( " Fiş BANKA MODÜLÜ ( %2 ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_FIS->fis_no ).toInt() ).arg(BNK_GET_FIS_TURLERI_STRING(P_FIS->fis_turu));
            MUH_FIS->aciklama.append( P_FIS->aciklama );
        }
        else {
            MUH_FIS->aciklama = P_FIS->aciklama;
        }

        MUH_FISI_GUNCELLE( MUH_FIS, P_FIS->muh_fis_id );

        if ( P_FIS->aciklama.size() > 512 ) {
            P_FIS->aciklama.resize(512);
        }

        query.PREPARE_UPDATE("bnk_defter_fisler","fis_id","aciklama","fis_id = :fis_id  ");

        query.SET_VALUE( ":aciklama"                , P_FIS->aciklama                   );
        query.SET_VALUE( ":fis_id"                  , p_fis_id                          );

        query.UPDATE();

    }

}

/**************************************************************************************
                        BNK_DEFTER_FIS_SATIRINI_GUNCELLE
***************************************************************************************/

int BNK_DEFTER_FIS_SATIRINI_GUNCELLE( BNK_DEFTER_FIS_STRUCT * P_FIS, BNK_DEFTER_FIS_SATIRI_STRUCT * P_FIS_SATIRI,
                                      int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );
    query.PREPARE_SELECT ( "bnk_defter_fisler","modul_id, program_id, toplam_hesaba_giren, "
                           "toplam_hesaptan_cikan, base_fis_id                ",
                           "fis_id = :fis_id                            " );
    query.SET_VALUE      ( ":fis_id", P_FIS_SATIRI->fis_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    int         modul_id                     = query.VALUE ( 0 ).toInt();
    int         program_id                   = query.VALUE ( 1 ).toInt();
    double      old_toplam_hesaptan_cikan    = query.VALUE ( 2 ).toDouble();
    double      old_toplam_hesaba_giren      = query.VALUE ( 3 ).toDouble();
    int         base_fis_id                  = query.VALUE ( 4 ).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR P_FIS->base_fis_id NE base_fis_id ) {
        return 0;
    }

    query.PREPARE_SELECT ( "bnk_defter_fis_satirlari",
                           "hesaba_giren, hesaptan_cikan,  "
                           "hesap_turu, hesap_id",
                           "fis_satiri_id = :fis_satiri_id" );
    query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    double      old_hesaba_giren              = query.VALUE ( 0 ).toDouble();
    double      old_hesaptan_cikan            = query.VALUE ( 1 ).toDouble();
    int         old_hesap_turu                = query.VALUE ( 2 ).toInt();
    int         old_hesap_id                  = query.VALUE ( 3 ).toInt();

    int new_satir_id = 0;

    if ( old_hesaba_giren       NE P_FIS_SATIRI->hesaba_giren   OR
         old_hesaptan_cikan     NE P_FIS_SATIRI->hesaptan_cikan OR
         old_hesap_turu         NE P_FIS_SATIRI->hesap_turu     OR
         old_hesap_id           NE P_FIS_SATIRI->hesap_id ) {

        BNK_DEFTER_FIS_STRUCT * DEF_FIS = new BNK_DEFTER_FIS_STRUCT;

        DEF_FIS->program_id         = P_FIS->program_id;
        DEF_FIS->modul_id           = P_FIS->modul_id;
        DEF_FIS->base_fis_id        = P_FIS->base_fis_id;
        DEF_FIS->hesap_no_id        = P_FIS->hesap_no_id;
        DEF_FIS->muh_fis_id         = P_FIS->muh_fis_id;
        DEF_FIS->base_fis_turu      = P_FIS->base_fis_turu;

        QStringList def_row_list = GET_BNK_DEFTER_ROW_ID_LIST( P_FIS->program_id, P_FIS->modul_id, P_FIS_SATIRI->base_record_id ,USE_DB);

        if ( def_row_list.count() > 0 ) {

            for ( int i =0; i < def_row_list.count(); i++ ) {
                 BNK_DEFTER_FIS_SATIRI_SIL( DEF_FIS, QVariant ( def_row_list.at( i ) ).toInt(), USE_DB );
                 new_satir_id = BNK_DEFTER_FIS_SATIRI_EKLE( P_FIS, P_FIS_SATIRI ,0,true,USE_DB);
            }
        }



        delete DEF_FIS;

        // bakiyeler guncelleniyor
        P_FIS->toplam_hesaba_giren   = ROUND ( old_toplam_hesaba_giren  +  P_FIS_SATIRI->hesaba_giren   - old_hesaba_giren   );
        P_FIS->toplam_hesaptan_cikan = ROUND ( old_toplam_hesaptan_cikan+  P_FIS_SATIRI->hesaptan_cikan - old_hesaptan_cikan );

        query.PREPARE_UPDATE ( "bnk_defter_fisler","fis_id","toplam_hesaba_giren ,     "
                               "toplam_hesaptan_cikan  ","fis_id = :fis_id");

        query.SET_VALUE      ( ":toplam_hesaba_giren"   , P_FIS->toplam_hesaba_giren    );
        query.SET_VALUE      ( ":toplam_hesaptan_cikan" , P_FIS->toplam_hesaptan_cikan  );
        query.SET_VALUE      ( ":fis_id"                , P_FIS_SATIRI->fis_id          );
        query.UPDATE();

    }
    else { // sadece aciklama degismis

        if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {
            int muh_fis_id  = MUH_GET_ENT_FIS_ID ( P_FIS->program_id, P_FIS->modul_id, P_FIS->base_fis_id ,USE_DB);
            if ( muh_fis_id > 0) {
                MUH_ENTEGRASYON_FIS_SATIRLARINI_GUNCELLE( P_FIS_SATIRI->aciklama, P_FIS->program_id, P_FIS->modul_id, muh_fis_id,
                                                          P_FIS->base_fis_id, P_FIS_SATIRI->base_record_id ,USE_DB);
            }
        }
        query.PREPARE_UPDATE ( "bnk_defter_fis_satirlari","fis_satiri_id","aciklama","fis_satiri_id     = :fis_satiri_id " );

        query.SET_VALUE     ( ":aciklama"                , P_FIS_SATIRI->aciklama           );
        query.SET_VALUE     ( ":fis_satiri_id"           , p_fis_satiri_id                  );
        query.UPDATE();
    }

    return new_satir_id;
}

/**************************************************************************************
                            GET_BNK_DEFTER_FIS_ID
***************************************************************************************/

int GET_BNK_DEFTER_FIS_ID ( int p_program_id, int p_modul_id, int p_base_fis_id )
{

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_defter_fisler","fis_id","program_id = :program_id "
                          "AND modul_id     = :modul_id "
                          "AND base_fis_id  = :base_fis_id" );

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
                            GET_BNK_DEFTER_MUH_FIS_ID
***************************************************************************************/

int GET_BNK_DEFTER_MUH_FIS_ID ( int p_program_id, int p_modul_id, int p_fis_id )
{

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_defter_fisler","muh_fis_id","program_id = :program_id "
                          "AND modul_id     = :modul_id "
                          "AND fis_id       = :fis_id" );

    query.SET_VALUE( ":program_id",     p_program_id    );
    query.SET_VALUE( ":modul_id",       p_modul_id      );
    query.SET_VALUE( ":fis_id",         p_fis_id        );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE( 0 ).toInt();

}

/**************************************************************************************
                            GET_BNK_DEFTER_ROW_ID_LIST
***************************************************************************************/

QStringList GET_BNK_DEFTER_ROW_ID_LIST ( int p_program_id, int p_modul_id, int p_base_record_id , ADAK_SQL * P_GELEN_DB )
{
    QStringList row_id_list;

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query( USE_DB );
    query.PREPARE_SELECT( "bnk_defter_fis_satirlari","fis_satiri_id",
                          "program_id           =   :program_id "
                          "AND modul_id         =   :modul_id "
                          "AND base_record_id   =   :base_record_id ");

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
                            BNK_GET_FISE_GORE_BNK_DEFTER_ROW_ID_LIST
***************************************************************************************/
QStringList BNK_GET_FISE_GORE_BNK_DEFTER_ROW_ID_LIST(int p_program_id, int p_modul_id, int p_base_record_id, int p_fis_id)
{
    QStringList row_id_list;


    SQL_QUERY query( DB );
    query.PREPARE_SELECT( "bnk_defter_fis_satirlari","fis_satiri_id",
                          "program_id           =   :program_id "
                          "AND modul_id         =   :modul_id "
                          "AND base_record_id   =   :base_record_id "
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
                            GET_BNK_DEFTER_FIS_ROW_ID_LIST
***************************************************************************************/

QStringList GET_BNK_DEFTER_FIS_ROW_ID_LIST ( int p_program_id, int p_modul_id, int p_fis_id )
{
    QStringList row_id_list;

    SQL_QUERY query( DB );
    query.PREPARE_SELECT( "bnk_defter_fis_satirlari","fis_satiri_id",
                          "program_id   = :program_id   "
                          "AND modul_id = :modul_id     "
                          "AND fis_id   = :fis_id       ");

    query.SET_VALUE ( ":program_id",     p_program_id      );
    query.SET_VALUE ( ":modul_id",       p_modul_id        );
    query.SET_VALUE ( ":fis_id",         p_fis_id          );

    if ( query.SELECT() EQ 0 ) {
        return row_id_list;
    }
    while ( query.NEXT() EQ true ) {
        row_id_list << query.VALUE( 0 ).toString();
    }
    return row_id_list;
}

