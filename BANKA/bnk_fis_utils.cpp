#include "adak_sql.h"
#include "bnk_fis_utils.h"
#include "e9_console_utils.h"
#include "cari_console_utils.h"
#include "bnk_console_utils.h"
#include "muh_fis_utils.h"
#include "muh_enum.h"
#include "cari_fis_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "adak_utils.h"
#include "bnk_defter_fis_utils.h"
#include <QString>
#include "banka_enum.h"
#include "cari_enum.h"

extern      ADAK_SQL   *   DB;

/***************************************************************************************
                            BNK_HAREKET_FIS_EKLE
***************************************************************************************/

int  BNK_HAREKET_FIS_EKLE( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_id,  bool base_fis_eklenecek ,ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query( USE_DB );


    if ( P_FIS->fis_no EQ 0 ) {
        P_FIS->fis_no  = BNK_SIRADAKI_HAREKET_FIS_NO_AL( P_FIS->fis_tarihi , USE_DB);
    }

    int hrk_fis_id  = p_fis_id;

    if ( base_fis_eklenecek EQ true ) {

        query.PREPARE_INSERT("bnk_hareket_fisler","fis_id" , "fis_no, fis_tarihi, aciklama, "
                             "hesap_no_id, fis_turu, program_id,   "
                             "modul_id, para_birim_id, base_fis_id,  "
                             "toplam_hesaba_giren, toplam_hesaptan_cikan, muh_fis_id, base_fis_turu, def_fis_id,    "
                             "odm_emr_hesabi_etkilenecek_mi,muh_kasa_hesap_id");


        if ( P_FIS->fis_turu EQ 0 ) {
            P_FIS->fis_turu = ENUM_BANKA_HAREKET_FISI;
        }

        query.SET_VALUE( ":fis_no"                          , P_FIS->fis_no                         );
        query.SET_VALUE( ":fis_tarihi"                      , P_FIS->fis_tarihi                     );
        query.SET_VALUE( ":aciklama"                        , P_FIS->aciklama                       );
        query.SET_VALUE( ":hesap_no_id"                     , P_FIS->hesap_no_id                    );
        query.SET_VALUE( ":fis_turu"                        , P_FIS->fis_turu                       );
        query.SET_VALUE( ":program_id"                      , P_FIS->program_id                     );
        query.SET_VALUE( ":modul_id"                        , P_FIS->modul_id                       );
        query.SET_VALUE( ":para_birim_id"                   , P_FIS->para_birim_id                  );
        query.SET_VALUE( ":base_fis_id"                     , P_FIS->base_fis_id                    );
        query.SET_VALUE( ":muh_fis_id"                      , P_FIS->muh_fis_id                     );
        query.SET_VALUE( ":toplam_hesaba_giren"             , 0                                     );
        query.SET_VALUE( ":toplam_hesaptan_cikan"           , 0                                     );
        query.SET_VALUE( ":base_fis_turu"                   , P_FIS->base_fis_turu                  );
        query.SET_VALUE( ":def_fis_id"                      , P_FIS->def_fis_id                     );
        query.SET_VALUE( ":odm_emr_hesabi_etkilenecek_mi"   , P_FIS->odm_emr_hesabi_etkilenecek_mi  );
        query.SET_VALUE( ":para_birim_id"                   , P_FIS->para_birim_id                  );
        query.SET_VALUE( ":muh_kasa_hesap_id"               , P_FIS->muh_kasa_hesap_id);

        hrk_fis_id = query.INSERT();
    }

    if ( P_FIS->modul_id EQ BANKA_MODULU ) {
        if ( P_FIS->base_fis_id EQ 0 ) {
            P_FIS->base_fis_id = hrk_fis_id;
        }
    }

    BNK_DEFTER_FIS_STRUCT * DEF_FIS = new BNK_DEFTER_FIS_STRUCT;

    CLEAR_BNK_DEFTER_FIS_STRUCT ( DEF_FIS );

    if (  P_FIS->def_fis_id EQ 0 ) {

        if ( P_FIS->modul_id EQ BANKA_MODULU ) {
            DEF_FIS->aciklama           = QString ( QObject::tr ( " Fiş BANKA MODÜLÜ ( %2 ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_FIS->fis_no ).toInt() )
                                          .arg(BNK_GET_FIS_TURLERI_STRING(P_FIS->fis_turu));
            DEF_FIS->aciklama.append(P_FIS->aciklama);
        }
        else {
            DEF_FIS->aciklama = P_FIS->aciklama;
        }

        int deftere_gidecek_fis_turu = P_FIS->fis_turu;

        if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {

            deftere_gidecek_fis_turu = ENUM_BANKA_HAREKET_FISI;
        }
        DEF_FIS->fis_tarihi                         = P_FIS->fis_tarihi;
        DEF_FIS->fis_turu                           = deftere_gidecek_fis_turu;
        DEF_FIS->modul_id                           = P_FIS->modul_id;
        DEF_FIS->program_id                         = P_FIS->program_id;
        DEF_FIS->base_fis_id                        = P_FIS->base_fis_id;
        DEF_FIS->hesap_no_id                        = P_FIS->hesap_no_id;
        DEF_FIS->para_birim_id                      = P_FIS->para_birim_id;
        DEF_FIS->muh_fis_id                         = P_FIS->muh_fis_id;
        DEF_FIS->base_fis_turu                      = P_FIS->base_fis_turu;
        DEF_FIS->odm_emr_hesabi_etkilenecek_mi      = P_FIS->odm_emr_hesabi_etkilenecek_mi;

        P_FIS->def_fis_id = BNK_DEFTER_FIS_EKLE ( DEF_FIS ,USE_DB );
    }

    MUH_FIS_STRUCT * MUH_FIS    = new MUH_FIS_STRUCT;
    MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

    if ( P_FIS->muh_fis_id EQ 0 ) {

        if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {
            if ( P_FIS->muh_fis_id EQ 0 ) {

                MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

                if ( P_FIS->modul_id NE BANKA_MODULU ) {
                    MUH_FIS->aciklama = P_FIS->aciklama;
                }
                else{
                    MUH_FIS->aciklama  = QString ( QObject::tr ( " Fiş BANKA MODÜLÜ ( %2 ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_FIS->fis_no ).toInt() )
                                                  .arg(BNK_GET_FIS_TURLERI_STRING(P_FIS->fis_turu));
                    MUH_FIS->aciklama.append (P_FIS->aciklama );
                }

                MUH_FIS->fis_tarihi         = P_FIS->fis_tarihi;
                MUH_FIS->fis_turu           = ENUM_MAHSUP_FISI;
                MUH_FIS->modul_id           = P_FIS->modul_id;
                MUH_FIS->program_id         = P_FIS->program_id;
                MUH_FIS->base_fis_id        = P_FIS->def_fis_id; // defter gitmesi gerekiyor

                P_FIS->muh_fis_id = MUH_FIS_EKLE ( MUH_FIS  , USE_DB);
            }
        }
    }

    query.PREPARE_UPDATE( "bnk_defter_fisler","fis_id","muh_fis_id ","fis_id = :fis_id " );
    query.SET_VALUE     ( ":muh_fis_id",   P_FIS->muh_fis_id );
    query.SET_VALUE     ( ":fis_id",       P_FIS->def_fis_id );
    query.UPDATE();

    query.PREPARE_UPDATE( "bnk_hareket_fisler","fis_id","def_fis_id , muh_fis_id ","fis_id = :fis_id " );
    query.SET_VALUE     ( ":def_fis_id",   P_FIS->def_fis_id );
    query.SET_VALUE     ( ":muh_fis_id",   P_FIS->muh_fis_id );
    query.SET_VALUE     ( ":fis_id",       hrk_fis_id );
    query.UPDATE();

    delete DEF_FIS;
    delete MUH_FIS;

    // Banka modulunde base_fis_id = 0 olmasi gerekir. eger disaridan gelmis ise farklidir.
    if ( P_FIS->modul_id EQ BANKA_MODULU ) {
            P_FIS->base_fis_id = 0;
    }

    return hrk_fis_id;
}

/**************************************************************************************
                        BNK_HAREKET_FIS_SATIRI_EKLE
***************************************************************************************/

int BNK_HAREKET_FIS_SATIRI_EKLE( BNK_HAREKET_FIS_STRUCT * P_FIS, BNK_HAREKET_FIS_SATIRI_STRUCT * P_FIS_SATIRI,
                                 int p_fis_satiri_id, bool base_fis_satiri_eklenecek , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB ;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    if( P_FIS_SATIRI->etkilenecek_hesap_turu EQ 0 ) {
        return 0;
    }

    SQL_QUERY f_query      ( USE_DB);

    f_query.PREPARE_SELECT ( "bnk_hareket_fisler","toplam_hesaba_giren,toplam_hesaptan_cikan, "
                             "fis_tarihi, last_order_number, base_fis_id, aciklama, fis_no, "
                             "modul_id, program_id, para_birim_id","fis_id = :fis_id" );

    f_query.SET_VALUE      ( ":fis_id" , P_FIS_SATIRI->fis_id );

    if ( f_query.SELECT() EQ 0 ) {
        return 0;
    }
    f_query.NEXT();

    double    toplam_hesaba_giren       = f_query.VALUE ( 0 ).toDouble();
    double    toplam_hesaptan_cikan     = f_query.VALUE ( 1 ).toDouble();
    QString   fis_tarihi                = f_query.VALUE ( 2 ).toString();
    int       last_order_number         = f_query.VALUE ( 3 ).toInt();
    int       base_fis_id               = f_query.VALUE ( 4 ).toInt();
    QString   aciklama                  = f_query.VALUE ( 5 ).toString();
    int       fis_no                    = f_query.VALUE ( 6 ).toInt();
    int       modul_id                  = f_query.VALUE ( 7 ).toInt();
    int       program_id                = f_query.VALUE ( 8 ).toInt();
    int       para_birim_id             = f_query.VALUE ( 9 ).toInt();
    int       banka_para_birim_id       = para_birim_id;
    int       karsi_hesap_para_birim_id = 0;


    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR P_FIS->base_fis_id NE base_fis_id ) {
        return 0;
    }

    if ( P_FIS_SATIRI->order_number EQ 0 ) {
        P_FIS_SATIRI->order_number = last_order_number + 1024;
    }


    int satir_id = p_fis_satiri_id;


    if ( base_fis_satiri_eklenecek EQ true ) {

        f_query.PREPARE_INSERT ( "bnk_hareket_fis_satirlari", "fis_satiri_id" ,"fis_id, aciklama, hesaba_giren,hesaptan_cikan, base_record_id, order_number,   "
                                  "etkilenecek_hesap_turu, etkilenecek_hesap_id, karsi_hesap_turu,                  "
                                  "karsi_hesap_id, doviz_kuru, parite, program_id, "
                                  "modul_id,  karsi_hesap_eklenecek_mi ");

        f_query.SET_VALUE ( ":fis_id"                   , P_FIS_SATIRI->fis_id                    );
        f_query.SET_VALUE ( ":aciklama"                 , P_FIS_SATIRI->aciklama                  );
        f_query.SET_VALUE ( ":hesaba_giren"             , ROUND( P_FIS_SATIRI->hesaba_giren  )    );
        f_query.SET_VALUE ( ":hesaptan_cikan"           , ROUND( P_FIS_SATIRI->hesaptan_cikan)    );
        f_query.SET_VALUE ( ":base_record_id"           , P_FIS_SATIRI->base_record_id            );
        f_query.SET_VALUE ( ":order_number"             , P_FIS_SATIRI->order_number              );
        f_query.SET_VALUE ( ":etkilenecek_hesap_turu"   , P_FIS_SATIRI->etkilenecek_hesap_turu    );
        f_query.SET_VALUE ( ":etkilenecek_hesap_id"     , P_FIS_SATIRI->etkilenecek_hesap_id      );
        f_query.SET_VALUE ( ":karsi_hesap_turu"         , P_FIS_SATIRI->karsi_hesap_turu          );
        f_query.SET_VALUE ( ":karsi_hesap_id"           , P_FIS_SATIRI->karsi_hesap_id            );
        f_query.SET_VALUE ( ":doviz_kuru"               , P_FIS_SATIRI->doviz_kuru                );
        f_query.SET_VALUE ( ":parite"                   , P_FIS_SATIRI->parite                    );
        f_query.SET_VALUE ( ":program_id"               , P_FIS_SATIRI->program_id                );
        f_query.SET_VALUE ( ":modul_id"                 , P_FIS_SATIRI->modul_id                  );
        f_query.SET_VALUE ( ":karsi_hesap_eklenecek_mi" , P_FIS_SATIRI->karsi_hesap_eklenecek_mi  );

        satir_id = f_query.INSERT();
    }

    if ( P_FIS_SATIRI->modul_id EQ BANKA_MODULU ) {
        if ( P_FIS_SATIRI->base_record_id EQ 0 ){
            P_FIS_SATIRI->base_record_id = satir_id;
        }
        if ( base_fis_id EQ 0 ) {
            base_fis_id = P_FIS_SATIRI->fis_id;
        }
    }

    double temel_para_hesaba_giren             = 0.0;
    double temel_para_hesaptan_cikan           = 0.0;

    if ( banka_para_birim_id NE DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
        temel_para_hesaba_giren   = P_FIS_SATIRI->hesaba_giren   *  P_FIS_SATIRI->doviz_kuru;
        temel_para_hesaptan_cikan = P_FIS_SATIRI->hesaptan_cikan *  P_FIS_SATIRI->doviz_kuru;
    }
    else {
        temel_para_hesaba_giren   = P_FIS_SATIRI->hesaba_giren;
        temel_para_hesaptan_cikan = P_FIS_SATIRI->hesaptan_cikan;
    }

    BNK_DEFTER_FIS_STRUCT         * DEF_FIS        = new BNK_DEFTER_FIS_STRUCT;
    BNK_DEFTER_FIS_SATIRI_STRUCT  * DEF_FIS_SATIRI = new BNK_DEFTER_FIS_SATIRI_STRUCT;


    CARI_FIS_STRUCT        * CARI_FIS        = new CARI_FIS_STRUCT;
    CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI = new CARI_FIS_SATIRI_STRUCT;

    MUH_FIS_STRUCT         * MUH_FIS         = new MUH_FIS_STRUCT;
    MUH_FIS_SATIRI_STRUCT  * MUH_FIS_SATIRI  = new MUH_FIS_SATIRI_STRUCT;


    if ( temel_para_hesaba_giren > 0.00 ) {

        //Etkilenen hesaba borc yaziliyor
        // KURAL : Muhasebe modulune fis uretilirken once borc,daha sonra alacak satiri eklenir.

        CLEAR_BNK_DEFTER_FIS_STRUCT        ( DEF_FIS );
        CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT ( DEF_FIS_SATIRI );

        BNK_DEFTER_FIS_BILGILERINI_OKU ( DEF_FIS, P_FIS->def_fis_id,USE_DB );

        DEF_FIS_SATIRI->aciklama             = P_FIS_SATIRI->aciklama;
        DEF_FIS_SATIRI->hesaba_giren         = P_FIS_SATIRI->hesaba_giren;
        DEF_FIS_SATIRI->hesaptan_cikan       = P_FIS_SATIRI->hesaptan_cikan;
        DEF_FIS_SATIRI->fis_id               = P_FIS->def_fis_id;
        DEF_FIS_SATIRI->hesap_id             = P_FIS_SATIRI->etkilenecek_hesap_id;
        DEF_FIS_SATIRI->hesap_turu           = P_FIS_SATIRI->etkilenecek_hesap_turu;
        DEF_FIS_SATIRI->base_record_id       = P_FIS_SATIRI->base_record_id;
        DEF_FIS_SATIRI->program_id           = P_FIS_SATIRI->program_id;
        DEF_FIS_SATIRI->modul_id             = P_FIS_SATIRI->modul_id;
        DEF_FIS_SATIRI->doviz_kuru           = P_FIS_SATIRI->doviz_kuru;

        BNK_DEFTER_FIS_SATIRI_EKLE ( DEF_FIS, DEF_FIS_SATIRI, 0, true ,USE_DB );

        if ( P_FIS_SATIRI->karsi_hesap_eklenecek_mi EQ 1 ) {

            if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {

                switch ( P_FIS_SATIRI->karsi_hesap_turu ) {

                    case ENUM_BNK_CARI_HESAP :
                    {

                        karsi_hesap_para_birim_id  = CARI_GET_PARA_BIRIM_ID( P_FIS_SATIRI->karsi_hesap_id );

                        // eger cari modul den geliyor ise cari modulune fis ve satir uretmeyecek
                        if ( P_FIS->program_id NE E9_PROGRAMI OR P_FIS->modul_id NE CARI_MODULU ) {

                            CARI_CLEAR_FIS_STRUCT        ( CARI_FIS );


                            int car_fis_id = CARI_GET_ENT_FIS_ID( P_FIS_SATIRI->program_id, P_FIS_SATIRI->modul_id, P_FIS_SATIRI->base_record_id ,USE_DB);

                            if ( car_fis_id EQ 0 ) {

                                if ( P_FIS->modul_id EQ BANKA_MODULU ) {
                                    CARI_FIS->aciklama = QString ( QObject::tr ( "Fiş BANKA MODÜLÜ ( %1 ) tarafından üretilmiştir. Fiş No = %2\n" ) ).arg(BNK_GET_FIS_TURLERI_STRING(P_FIS->fis_turu)).arg( fis_no)  + aciklama ;
                                }
                                else {
                                    CARI_FIS->aciklama = P_FIS->aciklama;
                                }

                                CARI_FIS->cari_hesap_id         = 0; // banka islem fisi cari hareket fisi olusturur , cari fis satirinda cari hesap id 0 olmalidir
                                CARI_FIS->cari_para_birimi_id   = karsi_hesap_para_birim_id;
                                CARI_FIS->makbuz_para_birimi_id = banka_para_birim_id;
                                CARI_FIS->fis_tarihi            = fis_tarihi;
                                CARI_FIS->modul_id              = P_FIS->modul_id;
                                CARI_FIS->program_id            = P_FIS->program_id;
                                CARI_FIS->fis_turu              = ENUM_CARI_HAREKET_FISI;
                                CARI_FIS->base_fis_id           = P_FIS->def_fis_id;
                                CARI_FIS->muh_fis_id            = P_FIS->muh_fis_id;

                                car_fis_id = CARI_FIS_EKLE      ( CARI_FIS ,USE_DB);
                            }
                            else {
                                CARI_FIS_BILGILERINI_OKU ( CARI_FIS, car_fis_id,USE_DB);
                            }

                            CARI_CLEAR_FIS_SATIRI_STRUCT ( CARI_FIS_SATIRI );


                            CARI_FIS_SATIRI->aciklama                = P_FIS_SATIRI->aciklama;
                            CARI_FIS_SATIRI->borc_tutari             = P_FIS_SATIRI->hesaptan_cikan;
                            CARI_FIS_SATIRI->alacak_tutari           = P_FIS_SATIRI->hesaba_giren;
                            CARI_FIS_SATIRI->cari_hesap_id           = P_FIS_SATIRI->karsi_hesap_id;
                            CARI_FIS_SATIRI->doviz_id                = banka_para_birim_id;
                            CARI_FIS_SATIRI->fis_id                  = car_fis_id;
                            CARI_FIS_SATIRI->program_id              = P_FIS_SATIRI->program_id;
                            CARI_FIS_SATIRI->modul_id                = P_FIS_SATIRI->modul_id;
                            CARI_FIS_SATIRI->base_record_id          = P_FIS_SATIRI->base_record_id;
                            CARI_FIS_SATIRI->doviz_kuru              = P_FIS_SATIRI->doviz_kuru;
                            CARI_FIS_SATIRI->parite                  = P_FIS_SATIRI->parite;

                            CARI_FIS_SATIRI_EKLE ( CARI_FIS, CARI_FIS_SATIRI ,USE_DB);

                        }

                        break;
                    }
                case ENUM_BNK_POS_HESABI:
                case ENUM_BNK_KREDI_KARTI_HESABI: { // Defter

                        CLEAR_BNK_DEFTER_FIS_STRUCT        ( DEF_FIS );
                        CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT ( DEF_FIS_SATIRI );

                        BNK_DEFTER_FIS_BILGILERINI_OKU ( DEF_FIS, P_FIS->def_fis_id ,USE_DB);

                        DEF_FIS_SATIRI->aciklama             = P_FIS_SATIRI->aciklama;
                        DEF_FIS_SATIRI->hesaba_giren         = P_FIS_SATIRI->hesaptan_cikan;
                        DEF_FIS_SATIRI->hesaptan_cikan       = P_FIS_SATIRI->hesaba_giren;
                        DEF_FIS_SATIRI->fis_id               = P_FIS->def_fis_id;
                        DEF_FIS_SATIRI->hesap_id             = P_FIS_SATIRI->karsi_hesap_id;
                        DEF_FIS_SATIRI->hesap_turu           = P_FIS_SATIRI->karsi_hesap_turu;
                        DEF_FIS_SATIRI->base_record_id       = P_FIS_SATIRI->base_record_id;
                        DEF_FIS_SATIRI->program_id           = P_FIS_SATIRI->program_id;
                        DEF_FIS_SATIRI->modul_id             = P_FIS_SATIRI->modul_id;
                        DEF_FIS_SATIRI->doviz_kuru           = P_FIS_SATIRI->doviz_kuru;

                        BNK_DEFTER_FIS_SATIRI_EKLE ( DEF_FIS, DEF_FIS_SATIRI, 0, true ,USE_DB);

                        break;
                    }
                case ENUM_BNK_POS_KOMISYON_HESABI:
                case ENUM_BNK_KASA_HESABI:
                case ENUM_BNK_MUHASEBE_HESABI:
                    {

                        int pos_komisyon_id = 0;

                        if ( P_FIS_SATIRI->karsi_hesap_turu EQ ENUM_BNK_POS_KOMISYON_HESABI ) {
                            SQL_QUERY query( USE_DB );
                            query.PREPARE_SELECT( "bnk_hesabin_poslari","kom_hesap_id","pos_id =:pos_id" );
                            query.SET_VALUE( ":pos_is", P_FIS_SATIRI->karsi_hesap_id );

                            if ( query.SELECT() NE 0 ) {
                                query.NEXT();
                                pos_komisyon_id = query.VALUE( 0 ).toInt();
                            }
                        }

                        MUH_CLEAR_FIS_STRUCT        ( MUH_FIS );
                        MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI );

                        MUH_FIS_BILGILERINI_OKU ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);

                        MUH_FIS_SATIRI->aciklama             = P_FIS_SATIRI->aciklama;
                        MUH_FIS_SATIRI->borc_tutari          = temel_para_hesaptan_cikan;
                        MUH_FIS_SATIRI->alacak_tutari        = temel_para_hesaba_giren;
                        MUH_FIS_SATIRI->fis_id               = P_FIS->muh_fis_id;

                        if ( P_FIS_SATIRI->karsi_hesap_turu EQ ENUM_BNK_KASA_HESABI ) {
                            if ( P_FIS->fis_turu EQ ENUM_BANKA_HAREKET_FISI ) {
                                MUH_FIS_SATIRI->hesap_id = P_FIS_SATIRI->karsi_hesap_id;
                            }
                            else {
                                MUH_FIS_SATIRI->hesap_id = P_FIS->muh_kasa_hesap_id;
                            }
                        }
                        else if ( P_FIS_SATIRI->karsi_hesap_turu EQ ENUM_BNK_POS_KOMISYON_HESABI ) {
                            MUH_FIS_SATIRI->hesap_id         = pos_komisyon_id;
                        }
                        else {
                            MUH_FIS_SATIRI->hesap_id         = P_FIS_SATIRI->karsi_hesap_id;
                        }

                        MUH_FIS_SATIRI->base_record_id       = P_FIS_SATIRI->base_record_id;
                        MUH_FIS_SATIRI->program_id           = P_FIS_SATIRI->program_id;
                        MUH_FIS_SATIRI->modul_id             = P_FIS_SATIRI->modul_id;

                        MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI ,USE_DB);

                        break;
                    }
                    default                :
                        break;
                }
            }
        }
    }
    else {

        if ( P_FIS_SATIRI->karsi_hesap_eklenecek_mi EQ 1 ) {

            if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {

                switch ( P_FIS_SATIRI->karsi_hesap_turu ) {

                case ENUM_BNK_CARI_HESAP:
                {
                        karsi_hesap_para_birim_id  = CARI_GET_PARA_BIRIM_ID( P_FIS_SATIRI->karsi_hesap_id );

                        if ( P_FIS->program_id NE E9_PROGRAMI OR P_FIS->modul_id NE CARI_MODULU ) {

                            CARI_CLEAR_FIS_STRUCT        ( CARI_FIS );

                            int car_fis_id = CARI_GET_ENT_FIS_ID( P_FIS_SATIRI->program_id, P_FIS_SATIRI->modul_id, P_FIS_SATIRI->base_record_id ,USE_DB);

                            if ( car_fis_id EQ 0 ) { // banka hareket fisi inin bir cari hesap yok ise olusturulur

                                if ( P_FIS->modul_id EQ BANKA_MODULU ) {

                                    CARI_FIS->aciklama  = QString ( QObject::tr ( " Fiş BANKA MODÜLÜ ( %2 ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_FIS->fis_no ).toInt() )
                                                                  .arg(BNK_GET_FIS_TURLERI_STRING(P_FIS->fis_turu));
                                    CARI_FIS->aciklama.append (aciklama );
                                }
                                else {
                                    CARI_FIS->aciklama = P_FIS->aciklama;
                                }
                                CARI_FIS->cari_hesap_id         = 0; // banka islem fisi cari hareket fisi olusturur , cari fis satirinda cari hesap id 0 olmalidir
                                CARI_FIS->cari_para_birimi_id   = karsi_hesap_para_birim_id;
                                CARI_FIS->makbuz_para_birimi_id = banka_para_birim_id;
                                CARI_FIS->fis_tarihi            = fis_tarihi;
                                CARI_FIS->modul_id              = P_FIS->modul_id;
                                CARI_FIS->program_id            = P_FIS->program_id;
                                CARI_FIS->fis_turu              = ENUM_CARI_HAREKET_FISI;
                                CARI_FIS->base_fis_id           = P_FIS->def_fis_id;
                                CARI_FIS->muh_fis_id            = P_FIS->muh_fis_id;

                                car_fis_id = CARI_FIS_EKLE      ( CARI_FIS ,USE_DB);
                            }
                            else {
                                CARI_FIS_BILGILERINI_OKU ( CARI_FIS, car_fis_id ,USE_DB);
                            }



                            CARI_CLEAR_FIS_SATIRI_STRUCT ( CARI_FIS_SATIRI );


                            CARI_FIS_SATIRI->aciklama                = P_FIS_SATIRI->aciklama;
                            CARI_FIS_SATIRI->borc_tutari             = P_FIS_SATIRI->hesaptan_cikan;
                            CARI_FIS_SATIRI->alacak_tutari           = P_FIS_SATIRI->hesaba_giren;
                            CARI_FIS_SATIRI->cari_hesap_id           = P_FIS_SATIRI->karsi_hesap_id;
                            CARI_FIS_SATIRI->doviz_id                = banka_para_birim_id;
                            CARI_FIS_SATIRI->doviz_kodu              = DVZ_GET_DOVIZ_KODU( banka_para_birim_id );
                            CARI_FIS_SATIRI->fis_id                  = car_fis_id;
                            CARI_FIS_SATIRI->program_id              = P_FIS_SATIRI->program_id;
                            CARI_FIS_SATIRI->modul_id                = P_FIS_SATIRI->modul_id;
                            CARI_FIS_SATIRI->base_record_id          = P_FIS_SATIRI->base_record_id;
                            CARI_FIS_SATIRI->doviz_kuru              = P_FIS_SATIRI->doviz_kuru;
                            CARI_FIS_SATIRI->parite                  = P_FIS_SATIRI->parite;

                            CARI_FIS_SATIRI_EKLE ( CARI_FIS, CARI_FIS_SATIRI,true ,0, NULL, USE_DB );

                        }

                        break;
                    }
                case ENUM_BNK_POS_HESABI:
                case ENUM_BNK_KREDI_KARTI_HESABI: { // Defter

                        CLEAR_BNK_DEFTER_FIS_STRUCT        ( DEF_FIS );
                        CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT ( DEF_FIS_SATIRI );

                        BNK_DEFTER_FIS_BILGILERINI_OKU ( DEF_FIS, P_FIS->def_fis_id ,USE_DB);


                        DEF_FIS_SATIRI->aciklama             = P_FIS_SATIRI->aciklama;
                        DEF_FIS_SATIRI->hesaba_giren         = P_FIS_SATIRI->hesaptan_cikan;
                        DEF_FIS_SATIRI->hesaptan_cikan       = P_FIS_SATIRI->hesaba_giren;
                        DEF_FIS_SATIRI->fis_id               = P_FIS->def_fis_id;
                        DEF_FIS_SATIRI->hesap_id             = P_FIS_SATIRI->karsi_hesap_id;
                        DEF_FIS_SATIRI->hesap_turu           = P_FIS_SATIRI->karsi_hesap_turu;
                        DEF_FIS_SATIRI->base_record_id       = P_FIS_SATIRI->base_record_id;
                        DEF_FIS_SATIRI->program_id           = P_FIS_SATIRI->program_id;
                        DEF_FIS_SATIRI->modul_id             = P_FIS_SATIRI->modul_id;
                        DEF_FIS_SATIRI->doviz_kuru           = P_FIS_SATIRI->doviz_kuru;

                        BNK_DEFTER_FIS_SATIRI_EKLE ( DEF_FIS, DEF_FIS_SATIRI, 0, true ,USE_DB);

                        break;
                    }
                case ENUM_BNK_POS_KOMISYON_HESABI:
                case ENUM_BNK_KASA_HESABI:
                case ENUM_BNK_MUHASEBE_HESABI: { // Muhhasebe


                        int pos_komisyon_id = 0;

                        if ( P_FIS_SATIRI->karsi_hesap_turu EQ ENUM_BNK_POS_KOMISYON_HESABI ) {
                            SQL_QUERY query( USE_DB );
                            query.PREPARE_SELECT( "bnk_hesabin_poslari","kom_hesap_id"
                                                  ,"pos_id =:pos_id" );
                            query.SET_VALUE( ":pos_is", P_FIS_SATIRI->karsi_hesap_id );

                            if ( query.SELECT() NE 0 ) {
                                query.NEXT();
                                pos_komisyon_id = query.VALUE( 0 ).toInt();
                            }
                        }

                        MUH_CLEAR_FIS_STRUCT        ( MUH_FIS );
                        MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI );

                        MUH_FIS_BILGILERINI_OKU ( MUH_FIS, P_FIS->muh_fis_id ,USE_DB);

                        MUH_FIS_SATIRI->aciklama             = P_FIS_SATIRI->aciklama;
                        MUH_FIS_SATIRI->borc_tutari          = temel_para_hesaptan_cikan;
                        MUH_FIS_SATIRI->alacak_tutari        = temel_para_hesaba_giren;
                        MUH_FIS_SATIRI->fis_id               = P_FIS->muh_fis_id;

                        if ( P_FIS_SATIRI->karsi_hesap_turu EQ ENUM_BNK_KASA_HESABI ) {

                            if ( P_FIS->fis_turu EQ ENUM_BANKA_HAREKET_FISI ) {
                                MUH_FIS_SATIRI->hesap_id = P_FIS_SATIRI->karsi_hesap_id;
                            }
                            else {
                                MUH_FIS_SATIRI->hesap_id = P_FIS->muh_kasa_hesap_id;
                            }
                        }
                        else if ( P_FIS_SATIRI->karsi_hesap_turu EQ ENUM_BNK_POS_KOMISYON_HESABI ) {
                            MUH_FIS_SATIRI->hesap_id         = pos_komisyon_id;
                        }
                        else {
                            MUH_FIS_SATIRI->hesap_id         = P_FIS_SATIRI->karsi_hesap_id;
                        }

                        MUH_FIS_SATIRI->base_record_id       = P_FIS_SATIRI->base_record_id;
                        MUH_FIS_SATIRI->program_id           = P_FIS_SATIRI->program_id;
                        MUH_FIS_SATIRI->modul_id             = P_FIS_SATIRI->modul_id;

                        MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI ,USE_DB);


                        break;
                    }
                    default                :
                        break;
                }
            }
        }

        // Etkilenecek hesaba alacak

        CLEAR_BNK_DEFTER_FIS_STRUCT        ( DEF_FIS );
        CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT ( DEF_FIS_SATIRI );

        BNK_DEFTER_FIS_BILGILERINI_OKU ( DEF_FIS, P_FIS->def_fis_id ,USE_DB);


        DEF_FIS_SATIRI->aciklama             = P_FIS_SATIRI->aciklama;
        DEF_FIS_SATIRI->hesaba_giren         = ROUND(P_FIS_SATIRI->hesaba_giren,-2);
        DEF_FIS_SATIRI->hesaptan_cikan       = ROUND(P_FIS_SATIRI->hesaptan_cikan,-2);
        DEF_FIS_SATIRI->fis_id               = P_FIS->def_fis_id;
        DEF_FIS_SATIRI->hesap_id             = P_FIS_SATIRI->etkilenecek_hesap_id;
        DEF_FIS_SATIRI->hesap_turu           = P_FIS_SATIRI->etkilenecek_hesap_turu;
        DEF_FIS_SATIRI->base_record_id       = P_FIS_SATIRI->base_record_id;
        DEF_FIS_SATIRI->program_id           = P_FIS_SATIRI->program_id;
        DEF_FIS_SATIRI->modul_id             = P_FIS_SATIRI->modul_id;
        DEF_FIS_SATIRI->doviz_kuru           = ROUND( P_FIS_SATIRI->doviz_kuru, -4 );

        BNK_DEFTER_FIS_SATIRI_EKLE ( DEF_FIS, DEF_FIS_SATIRI, 0, true ,USE_DB);
    }

    toplam_hesaba_giren   = ROUND ( toplam_hesaba_giren   + P_FIS_SATIRI->hesaba_giren );
    toplam_hesaptan_cikan = ROUND ( toplam_hesaptan_cikan + P_FIS_SATIRI->hesaptan_cikan);

    if ( base_fis_satiri_eklenecek EQ true ) {
        // toplam degerler guncelleniyor
        f_query.PREPARE_UPDATE (  "bnk_hareket_fisler","fis_id","toplam_hesaba_giren,"
                                  "toplam_hesaptan_cikan ",
                                  "fis_id = :fis_id" );

        f_query.SET_VALUE      ( ":toplam_hesaba_giren"   , toplam_hesaba_giren   );
        f_query.SET_VALUE      ( ":toplam_hesaptan_cikan" , toplam_hesaptan_cikan );
        f_query.SET_VALUE      ( ":fis_id"                , P_FIS_SATIRI->fis_id  );
        f_query.UPDATE         ();

        P_FIS->toplam_hesaba_giren   = toplam_hesaba_giren;
        P_FIS->toplam_hesaptan_cikan = toplam_hesaptan_cikan;
    }

    delete CARI_FIS;
    delete CARI_FIS_SATIRI;

    delete MUH_FIS;
    delete MUH_FIS_SATIRI;

    delete DEF_FIS;
    delete DEF_FIS_SATIRI;

    return satir_id;

}

/**************************************************************************************
                        BNK_HAREKET_FISI_SIL
***************************************************************************************/

void BNK_HAREKET_FISI_SIL( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_id )
{

    SQL_QUERY query ( DB );
    query.PREPARE_SELECT ( "bnk_hareket_fisler","modul_id, program_id, base_fis_id        ",
                           "fis_id = :fis_id  " );
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

    int def_fis_id = P_FIS->def_fis_id;

    if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {


        int car_fis_id =  CARI_GET_ENT_FIS_ID   ( P_FIS->program_id, P_FIS->modul_id, P_FIS->base_fis_id );

        if( ( P_FIS->program_id EQ E9_PROGRAMI ) AND ( P_FIS->modul_id NE CARI_MODULU ) AND
            ( P_FIS->modul_id NE CEKSENET_MODULU ) AND ( P_FIS->modul_id NE SMM_MODULU)) {

            if ( car_fis_id > 0 ) {

                CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;

                CARI_FIS->modul_id      = P_FIS->modul_id;
                CARI_FIS->program_id    = P_FIS->program_id;
                CARI_FIS->base_fis_id   = P_FIS->def_fis_id;
                CARI_FIS->muh_fis_id    = P_FIS->muh_fis_id;

                CARI_FISI_SIL ( CARI_FIS, car_fis_id );

                delete CARI_FIS;
            }
        }
    }
    if ( def_fis_id > 0 ) {

        BNK_DEFTER_FIS_STRUCT * DEF_FIS = new BNK_DEFTER_FIS_STRUCT;

        CLEAR_BNK_DEFTER_FIS_STRUCT(DEF_FIS);

        BNK_DEFTER_FIS_BILGILERINI_OKU(DEF_FIS , def_fis_id);

        BNK_DEFTER_FISI_SIL( DEF_FIS, def_fis_id );

        delete DEF_FIS;
    }


    query.PREPARE_SELECT ( "bnk_hareket_fis_satirlari","fis_satiri_id","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id", p_fis_id );

    if ( query.SELECT() > 0 ) {
        while ( query.NEXT() EQ true ) {
            BNK_HAREKET_FIS_SATIRI_SIL ( P_FIS, query.VALUE ( 0 ).toInt() );
        }
    }

    query.PREPARE_DELETE ( "bnk_hareket_fisler","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id", p_fis_id );
    query.DELETE();
}

/**************************************************************************************
                        BNK_HAREKET_FIS_SATIRI_SIL()
***************************************************************************************/

void BNK_HAREKET_FIS_SATIRI_SIL( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_satiri_id  )
{

    SQL_QUERY query ( DB );
    query.PREPARE_SELECT ( "bnk_hareket_fisler, bnk_hareket_fis_satirlari",
                           "bnk_hareket_fisler.fis_id, fis_tarihi, hesaba_giren, hesaptan_cikan, "
                           "toplam_hesaba_giren, toplam_hesaptan_cikan, "
                           "bnk_hareket_fisler.modul_id, bnk_hareket_fisler.program_id, "
                           "etkilenecek_hesap_turu, karsi_hesap_turu, base_fis_id, "
                           "base_record_id , fis_turu, def_fis_id ",
                           "bnk_hareket_fisler.fis_id = bnk_hareket_fis_satirlari.fis_id "
                           "AND fis_satiri_id  = :fis_satiri_id" );

    query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    int     fis_id                          =  query.VALUE( 0  ).toInt();
    //QString fis_tarihi                      =  query.VALUE( 1  ).toString();
    double  hesaba_giren                    =  query.VALUE( 2  ).toDouble();
    double  hesaptan_cikan                  =  query.VALUE( 3  ).toDouble();
    double  toplam_hesaba_giren             =  query.VALUE( 4  ).toDouble();
    double  toplam_hesaptan_cikan           =  query.VALUE( 5  ).toDouble();
    int     modul_id                        =  query.VALUE( 6  ).toInt();
    int     program_id                      =  query.VALUE( 7  ).toInt(); 
    int     etkilenecek_hesap_turu          =  query.VALUE( 8  ).toInt();
    int     karsi_hesap_turu                =  query.VALUE( 9  ).toInt();
    int     base_fis_id                     =  query.VALUE( 10 ).toInt();
    int     base_record_id                  =  query.VALUE( 11 ).toInt();
    int     fis_turu                        =  query.VALUE( 12 ).toInt();
            P_FIS->def_fis_id               =  query.VALUE( 13 ).toInt();



    if ( modul_id NE P_FIS->modul_id OR program_id NE P_FIS->program_id OR P_FIS->base_fis_id NE base_fis_id) {
        return;
    }

    // eger kendi modulu tarafindan cagrilmis ise

    if ( P_FIS->modul_id EQ BANKA_MODULU ) {
        if ( base_record_id EQ 0 ) {
            base_record_id = p_fis_satiri_id;
        }
    }

    switch ( etkilenecek_hesap_turu )
    {
    case ENUM_BNK_BANKA_HESABI:
    case ENUM_BNK_KREDI_KARTI_HESABI:
    case ENUM_BNK_POS_HESABI: {

                BNK_DEFTER_FIS_STRUCT * DEF_FIS = new BNK_DEFTER_FIS_STRUCT;

                CLEAR_BNK_DEFTER_FIS_STRUCT( DEF_FIS );
                BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, P_FIS->def_fis_id );

                QStringList row_list = BNK_GET_FISE_GORE_BNK_DEFTER_ROW_ID_LIST(P_FIS->program_id, P_FIS->modul_id, base_record_id ,P_FIS->def_fis_id);

                for ( int i =0 ; i< row_list.count(); i++ ) {
                   BNK_DEFTER_FIS_SATIRI_SIL( DEF_FIS, QVariant( row_list.at( i ) ).toInt());
                }
                delete DEF_FIS;
            break;
        }
    } // end switch

    if ( fis_turu NE ENUM_BANKA_ACILIS_FISI ) {
        //  karsi hesaplar eger bankanin hesapalari tekra duzenleniyor
        switch ( karsi_hesap_turu ) {
            case ENUM_BNK_POS_KOMISYON_HESABI:
            case ENUM_BNK_KASA_HESABI:
            case ENUM_BNK_MUHASEBE_HESABI:
            {

                QStringList muh_row_list = MUH_GET_FISE_GORE_ENT_ROW_ID_LIST(P_FIS->program_id, P_FIS->modul_id, base_record_id , P_FIS->muh_fis_id);

                if ( muh_row_list.count() > 0 ) {

                    MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

                    MUH_CLEAR_FIS_STRUCT( MUH_FIS );
                    MUH_FIS_BILGILERINI_OKU( MUH_FIS, P_FIS->muh_fis_id );

                    for ( int i = 0; i< muh_row_list.count(); i++ ) {
                            MUH_FIS_SATIRINI_SIL( MUH_FIS, QVariant ( muh_row_list.at( i ) ).toInt() );
                    }

                    delete MUH_FIS;
                }

                break;
            }
            case ENUM_BNK_CARI_HESAP:
            {
                if( P_FIS->program_id NE E9_PROGRAMI OR P_FIS->modul_id NE CARI_MODULU ) {

                    CARI_FIS_STRUCT  * CARI_FIS = new CARI_FIS_STRUCT;

                    CARI_FIS->program_id = P_FIS->program_id;
                    CARI_FIS->modul_id   = P_FIS->modul_id;
                    CARI_FIS->base_fis_id= P_FIS->def_fis_id;
                    CARI_FIS->muh_fis_id = P_FIS->muh_fis_id;

                    QStringList cari_fis_row_list = CARI_GET_ENT_ROW_ID_LIST( P_FIS->program_id, P_FIS->modul_id, p_fis_satiri_id );

                    for( int i = 0 ; i < cari_fis_row_list.count(); i++ )    {
                        CARI_FIS_SATIRI_SIL ( CARI_FIS,  QVariant (cari_fis_row_list.at( i ) ).toInt()  );
                    }

                    delete CARI_FIS;
                 }
                 break;
            }
            case ENUM_BNK_KREDI_KARTI_HESABI:
            case ENUM_BNK_POS_HESABI:
                    break;
            default                :
                break;
        }
    }

    query.PREPARE_DELETE ( "bnk_hareket_fis_satirlari","fis_satiri_id = :fis_satiri_id" );
    query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );

    query.DELETE();



    P_FIS->toplam_hesaba_giren   = ROUND ( toplam_hesaba_giren   - hesaba_giren  );
    P_FIS->toplam_hesaptan_cikan = ROUND ( toplam_hesaptan_cikan - hesaptan_cikan);


    query.PREPARE_UPDATE ( "bnk_hareket_fisler","fis_id","toplam_hesaba_giren,     "
                           "toplam_hesaptan_cikan","fis_id = :fis_id  ");

    query.SET_VALUE      ( ":toplam_hesaba_giren"   , P_FIS->toplam_hesaba_giren    );
    query.SET_VALUE      ( ":toplam_hesaptan_cikan" , P_FIS->toplam_hesaptan_cikan  );
    query.SET_VALUE      ( ":fis_id"                , fis_id                        ); // P_FIS->fis_id

    query.UPDATE    ();

}

/**************************************************************************************
                        BNK_CLEAR_HAREKET_FIS_STRUCT()
***************************************************************************************/

void BNK_CLEAR_HAREKET_FIS_STRUCT( BNK_HAREKET_FIS_STRUCT *P_FIS )
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
    P_FIS->muh_fis_id                       =  0;
    P_FIS->base_fis_turu                    =  0;
    P_FIS->base_fis_id                      = 0;
    P_FIS->fis_turu                         = -1;
    P_FIS->modul_id                         = 0;
    P_FIS->muh_fis_id                       = 0;
    P_FIS->program_id                       = 0;
    P_FIS->def_fis_id                       = 0;
    P_FIS->odm_emr_hesabi_etkilenecek_mi  = 0;
    P_FIS->para_birim_id                    = 0;
    P_FIS->muh_kasa_hesap_id                = 0;

}

/**************************************************************************************
                        BNK_CLEAR_FIS_SATIRI_STRUCT()
***************************************************************************************/

void BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT( BNK_HAREKET_FIS_SATIRI_STRUCT *P_FIS_SATIRI )
{

    P_FIS_SATIRI->fis_id                    = 0;
    P_FIS_SATIRI->aciklama.clear();
    P_FIS_SATIRI->hesaba_giren              = 0.00;
    P_FIS_SATIRI->hesaptan_cikan            = 0.00;
    P_FIS_SATIRI->order_number              = 0;
    P_FIS_SATIRI->etkilenecek_hesap_turu    = 0;
    P_FIS_SATIRI->etkilenecek_hesap_id      = 0;
    P_FIS_SATIRI->karsi_hesap_turu          = 0;
    P_FIS_SATIRI->karsi_hesap_id            = 0;
    P_FIS_SATIRI->doviz_kuru                = 0.00;
    P_FIS_SATIRI->parite                    = 0.00;
    P_FIS_SATIRI->program_id                = -1;
    P_FIS_SATIRI->modul_id                  = -1;
    P_FIS_SATIRI->base_record_id            = -1;
    P_FIS_SATIRI->karsi_hesap_eklenecek_mi  =  1;

}

/**************************************************************************************
                        BNK_HAREKET_FIS_BILGILERINI_OKU
***************************************************************************************/

void  BNK_HAREKET_FIS_BILGILERINI_OKU ( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY  query( USE_DB );

    query.PREPARE_SELECT( "bnk_hareket_fisler","fis_no, fis_tarihi, program_id, modul_id, hesap_no_id, aciklama,  "
                          "toplam_hesaba_giren, toplam_hesaptan_cikan, fis_turu,                    "
                          "last_order_number, base_fis_id,                                          "
                          "muh_fis_id, base_fis_turu ,                            "
                          "def_fis_id, odm_emr_hesabi_etkilenecek_mi ,muh_kasa_hesap_id",
                          "fis_id = :fis_id                           " );

    query.SET_VALUE( ":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    P_FIS->fis_no                           = query.VALUE( 0 ).toInt();
    P_FIS->fis_tarihi                       = query.VALUE( "fis_tarihi" ).toString();
    P_FIS->program_id                       = query.VALUE( 2 ).toInt();
    P_FIS->modul_id                         = query.VALUE( 3 ).toInt();
    P_FIS->hesap_no_id                      = query.VALUE( 4 ).toInt();
    P_FIS->aciklama                         = query.VALUE("aciklama" ).toString();
    P_FIS->toplam_hesaba_giren              = query.VALUE( 6 ).toDouble();
    P_FIS->toplam_hesaptan_cikan            = query.VALUE( 7 ).toDouble();
    P_FIS->fis_turu                         = query.VALUE( 8 ).toInt();
    P_FIS->last_order_number                = query.VALUE( 9 ).toInt();
    P_FIS->base_fis_id                      = query.VALUE( 10 ).toInt();
    P_FIS->muh_fis_id                       = query.VALUE( 11 ).toInt();
    P_FIS->base_fis_turu                    = query.VALUE( 12 ).toInt();
    P_FIS->def_fis_id                       = query.VALUE( 13 ).toInt();
    P_FIS->odm_emr_hesabi_etkilenecek_mi    = query.VALUE( 14 ).toInt();
    P_FIS->muh_kasa_hesap_id                = query.VALUE("muh_kasa_hesap_id").toInt();


}

/**************************************************************************************
                        BNK_HAREKET_FIS_SATIR_BILGILERINI_OKU()
***************************************************************************************/

void BNK_HAREKET_FIS_SATIR_BILGILERINI_OKU( BNK_HAREKET_FIS_SATIRI_STRUCT * P_FIS_SATIRI  , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY  query( USE_DB );

    query.PREPARE_SELECT( "bnk_hareket_fis_satirlari","fis_id,aciklama,hesaba_giren,hesaptan_cikan, base_record_id, order_number, "
                          "etkilenecek_hesap_turu , etkilenecek_hesap_id, karsi_hesap_turu, karsi_hesap_id,  "
                          "doviz_kuru, parite, program_id, modul_id, "
                          "karsi_hesap_eklenecek_mi ","fis_satiri_id =:fis_satiri_id" );

    query.SET_VALUE( ":fis_satiri_id", p_fis_satiri_id );

    if ( query.SELECT() EQ 0 )
    {
        return;
    }
    query.NEXT();

    P_FIS_SATIRI->fis_id                   = query.VALUE( 0  ).toInt();
    P_FIS_SATIRI->aciklama                 = query.VALUE( 1  ).toString();
    P_FIS_SATIRI->hesaba_giren             = query.VALUE( 2  ).toDouble();
    P_FIS_SATIRI->hesaptan_cikan           = query.VALUE( 3  ).toDouble();
    P_FIS_SATIRI->base_record_id           = query.VALUE( 4  ).toInt();
    P_FIS_SATIRI->order_number             = query.VALUE( 5  ).toInt();
    P_FIS_SATIRI->etkilenecek_hesap_turu   = query.VALUE( 6  ).toInt();
    P_FIS_SATIRI->etkilenecek_hesap_id     = query.VALUE( 7  ).toInt();
    P_FIS_SATIRI->karsi_hesap_turu         = query.VALUE( 8  ).toInt();
    P_FIS_SATIRI->karsi_hesap_id           = query.VALUE( 9  ).toInt();
    P_FIS_SATIRI->doviz_kuru               = query.VALUE( 10 ).toDouble();
    P_FIS_SATIRI->parite                   = query.VALUE( 11 ).toDouble();
    P_FIS_SATIRI->program_id               = query.VALUE( 12 ).toInt();
    P_FIS_SATIRI->modul_id                 = query.VALUE( 13 ).toInt();
    P_FIS_SATIRI->karsi_hesap_eklenecek_mi = query.VALUE( 14 ).toInt();

}

/**************************************************************************************
                        BNK_HAREKET_FISI_GUNCELLE
***************************************************************************************/

void BNK_HAREKET_FISI_GUNCELLE ( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_id )
{

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "bnk_hareket_fisler","modul_id, program_id, hesap_no_id, "
                           "base_fis_id, fis_turu ,muh_kasa_hesap_id","fis_id = :fis_id " );
    query.SET_VALUE      ( ":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int         modul_id          = query.VALUE( 0 ).toInt();
    int         program_id        = query.VALUE( 1 ).toInt();
    int         hesap_no_id       = query.VALUE( 2 ).toInt();
    int         base_fis_id       = query.VALUE( 3 ).toInt();
    int         fis_turu          = query.VALUE( 4 ).toInt();
    int         muh_kasa_hesap_id = query.VALUE( 5 ).toInt();

    int         cari_para_birimi_id   = 0;
    int         banka_para_birimi_id  = 0;
    QString     banka_para_birimi_kodu;

    if ( fis_turu NE ENUM_BANKA_ACILIS_FISI ) {

        query.PREPARE_SELECT ( "bnk_hesaplar","doviz_id","hesap_no_id = :hesap_no_id" );
        query.SET_VALUE      ( ":hesap_no_id", hesap_no_id );

        if ( query.SELECT() > 0 ) {
            query.NEXT();
            banka_para_birimi_id   = query.VALUE ( 0 ).toInt();
            banka_para_birimi_kodu = DVZ_GET_DOVIZ_KODU ( banka_para_birimi_id );
        }

        if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR P_FIS->base_fis_id NE base_fis_id ) {
            return;
        }

        // eger cari olusturmus ise cari guncelleyecek
        int car_fis_id  = CARI_GET_ENT_FIS_ID( P_FIS->program_id, P_FIS->modul_id, P_FIS->base_fis_id );

        int def_fis_id  = P_FIS->def_fis_id;

        if( car_fis_id > 0 ) {

            if( P_FIS->program_id NE E9_PROGRAMI OR P_FIS->modul_id NE CARI_MODULU ) {

                CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;

                if ( P_FIS->modul_id EQ BANKA_MODULU ) {

                    CARI_FIS->aciklama  = QString ( QObject::tr ( "Fiş BANKA MODÜLÜ ( %2 ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_FIS->fis_no ).toInt() )
                                        .arg(BNK_GET_FIS_TURLERI_STRING(P_FIS->fis_turu)) + P_FIS->aciklama;
                }
                else {
                    CARI_FIS->aciklama = P_FIS->aciklama;
                }
                CARI_FIS->cari_hesap_id         = car_fis_id;
                CARI_FIS->cari_para_birimi_id   = cari_para_birimi_id;
                CARI_FIS->makbuz_para_birimi_id = banka_para_birimi_id;
                CARI_FIS->fis_tarihi            = P_FIS->fis_tarihi;
                CARI_FIS->modul_id              = P_FIS->modul_id;
                CARI_FIS->program_id            = P_FIS->program_id;
                CARI_FIS->base_fis_id           = P_FIS->base_fis_id;
                CARI_FIS->fis_turu              = ENUM_CARI_HAREKET_FISI;

                CARI_FISI_GUNCELLE( CARI_FIS, car_fis_id );

                delete CARI_FIS;
            }

        }
        else if ( def_fis_id > 0 ){

            if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {

                BNK_DEFTER_FIS_STRUCT * DEF_FIS  = new BNK_DEFTER_FIS_STRUCT;

                CLEAR_BNK_DEFTER_FIS_STRUCT( DEF_FIS );

                BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, def_fis_id );

                DEF_FIS->fis_tarihi         = P_FIS->fis_tarihi;
                DEF_FIS->hesap_no_id        = P_FIS->hesap_no_id;
                DEF_FIS->aciklama           = P_FIS->aciklama;

                BNK_DEFTER_FISI_GUNCELLE( DEF_FIS, def_fis_id );

                delete DEF_FIS;
            }

        }

        if ( muh_kasa_hesap_id  NE P_FIS->muh_kasa_hesap_id
             OR hesap_no_id     NE P_FIS->hesap_no_id ) {

            query.PREPARE_SELECT("bnk_hareket_fis_satirlari" ,"fis_satiri_id" ,
                                 "fis_id = :fis_id");
            query.SET_VALUE(":fis_id" , p_fis_id);

            if ( query.SELECT() NE 0 ) {

                BNK_HAREKET_FIS_SATIRI_STRUCT * HRK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

                while ( query.NEXT() EQ true ) {

                    int fis_satiri_id = query.VALUE(0).toInt();

                    BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT(HRK_FIS_SATIRI);

                    BNK_HAREKET_FIS_SATIR_BILGILERINI_OKU(HRK_FIS_SATIRI , fis_satiri_id);

                    HRK_FIS_SATIRI->karsi_hesap_id          = P_FIS->muh_kasa_hesap_id;
                    HRK_FIS_SATIRI->etkilenecek_hesap_id    = P_FIS->hesap_no_id;

                    BNK_HAREKET_FIS_SATIRINI_GUNCELLE(P_FIS ,HRK_FIS_SATIRI,fis_satiri_id);
                }
            }

        }
    }

    query.PREPARE_UPDATE("bnk_hareket_fisler","fis_id", "fis_tarihi ,"
                         "aciklama                                  , "
                         "fis_turu                                  , "
                         "program_id                                , "
                         "modul_id                                  , "
                         "toplam_hesaba_giren                       , "
                         "toplam_hesaptan_cikan                     , "
                         "hesap_no_id                               , "
                         "muh_fis_id                                , "
                         "def_fis_id                                , "
                         "odm_emr_hesabi_etkilenecek_mi             , "
                         "muh_kasa_hesap_id                           ",
                         "fis_id = :fis_id  ");

    query.SET_VALUE( ":fis_tarihi"                      , P_FIS->fis_tarihi                         );
    query.SET_VALUE( ":aciklama"                        , P_FIS->aciklama                           );
    query.SET_VALUE( ":fis_turu"                        , P_FIS->fis_turu                           );// ENUM_BANKA_HAREKET_FISI
    query.SET_VALUE( ":program_id"                      , P_FIS->program_id                         );
    query.SET_VALUE( ":modul_id"                        , P_FIS->modul_id                           );// Hangi Modul
    query.SET_VALUE( ":toplam_hesaba_giren"             , P_FIS->toplam_hesaba_giren                );
    query.SET_VALUE( ":toplam_hesaptan_cikan"           , P_FIS->toplam_hesaptan_cikan              );
    query.SET_VALUE( ":hesap_no_id"                     , P_FIS->hesap_no_id                        );
    query.SET_VALUE( ":muh_fis_id"                      , P_FIS->muh_fis_id                         );
    query.SET_VALUE( ":def_fis_id"                      , P_FIS->def_fis_id                         );
    query.SET_VALUE( ":fis_id"                          , p_fis_id                                  );
    query.SET_VALUE( ":odm_emr_hesabi_etkilenecek_mi"   , P_FIS->odm_emr_hesabi_etkilenecek_mi      );
    query.SET_VALUE( ":muh_kasa_hesap_id"               , P_FIS->muh_kasa_hesap_id                  );

    return query.UPDATE();

}

/**************************************************************************************
                        BNK_HAREKET_FIS_SATIRINI_GUNCELLE
***************************************************************************************/

int BNK_HAREKET_FIS_SATIRINI_GUNCELLE( BNK_HAREKET_FIS_STRUCT * P_FIS, BNK_HAREKET_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id)
{

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "bnk_hareket_fisler","modul_id, program_id, toplam_hesaba_giren, "
                           "toplam_hesaptan_cikan, hesap_no_id, base_fis_id ",
                           "fis_id = :fis_id " );
    query.SET_VALUE      ( ":fis_id", P_FIS_SATIRI->fis_id ); // satirdaki fis_id

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    int         modul_id                 = query.VALUE ( 0 ).toInt();
    int         program_id               = query.VALUE ( 1 ).toInt();
    P_FIS->toplam_hesaptan_cikan         = query.VALUE ( 2 ).toDouble();
    P_FIS->toplam_hesaba_giren           = query.VALUE ( 3 ).toDouble();
    int         hesap_no_id              = query.VALUE ( 4 ).toInt();
    int         base_fis_id              = query.VALUE ( 5 ).toInt();


    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR P_FIS->base_fis_id NE base_fis_id ) {
        return 0;
    }

    // Defter Fisine gidecek modul id
    int defter_modul_id = ENUM_BANKA_HAREKET_FISI;

    query.PREPARE_SELECT ( "bnk_hareket_fis_satirlari",
                           "hesaba_giren, hesaptan_cikan, aciklama, "
                           "etkilenecek_hesap_turu, etkilenecek_hesap_id,"
                           "karsi_hesap_turu, karsi_hesap_id",
                           "fis_satiri_id = :fis_satiri_id" );
    query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    double      old_hesaba_giren              = query.VALUE ( 0 ).toDouble();
    double      old_hesaptan_cikan            = query.VALUE ( 1 ).toDouble();
    QString     old_aciklama                  = query.VALUE ( 2 ).toString();
    int         old_etkilenecek_hesap_turu    = query.VALUE ( 3 ).toInt();
    int         old_etkilenecek_hesap_id      = query.VALUE ( 4 ).toInt();
    int         old_karsi_hesap_turu          = query.VALUE ( 5 ).toInt();
    int         old_karsi_hesap_id            = query.VALUE ( 6 ).toInt();

    int         banka_para_birimi_id          = 0;
    QString     banka_para_birimi_kodu;

    Q_UNUSED( old_aciklama );

    query.PREPARE_SELECT ( "bnk_hesaplar","doviz_id","hesap_no_id = :hesap_no_id" );
    query.SET_VALUE      ( ":hesap_no_id", hesap_no_id );

    if ( query.SELECT() > 0 ) {
        query.NEXT();
        banka_para_birimi_id   = query.VALUE ( 0 ).toInt();
        banka_para_birimi_kodu = DVZ_GET_DOVIZ_KODU ( banka_para_birimi_id );
    }

    int new_satir_id = 0;

    if( old_hesaba_giren NE P_FIS_SATIRI->hesaba_giren
        OR old_hesaptan_cikan           NE P_FIS_SATIRI->hesaptan_cikan
        OR old_etkilenecek_hesap_turu   NE P_FIS_SATIRI->etkilenecek_hesap_turu
        OR old_etkilenecek_hesap_id     NE P_FIS_SATIRI->etkilenecek_hesap_id
        OR old_karsi_hesap_turu         NE P_FIS_SATIRI->karsi_hesap_turu
        OR old_karsi_hesap_id           NE P_FIS_SATIRI->karsi_hesap_id )
    {

        BNK_HAREKET_FIS_STRUCT * BNK_FIS = new BNK_HAREKET_FIS_STRUCT;

        BNK_FIS->program_id = P_FIS->program_id;
        BNK_FIS->modul_id   = P_FIS->modul_id;
        BNK_FIS->base_fis_id= P_FIS->base_fis_id;
        BNK_FIS->hesap_no_id= P_FIS->hesap_no_id;
        BNK_FIS->fis_tarihi = P_FIS->fis_tarihi;
        BNK_FIS->def_fis_id = P_FIS->def_fis_id;
        BNK_FIS->muh_fis_id = P_FIS->muh_fis_id;

        BNK_HAREKET_FIS_SATIRI_SIL( BNK_FIS, p_fis_satiri_id );

        new_satir_id = BNK_HAREKET_FIS_SATIRI_EKLE( P_FIS, P_FIS_SATIRI );

        delete BNK_FIS;
    }
    else
    { //sadece aciklama degismistir

        if ( P_FIS->fis_turu NE ENUM_BANKA_ACILIS_FISI ) {

            int car_fis_id  = CARI_GET_ENT_FIS_ID( P_FIS->program_id, P_FIS->modul_id, P_FIS->base_fis_id );
            int def_fis_id  = GET_BNK_DEFTER_FIS_ID ( P_FIS->program_id, defter_modul_id, P_FIS->base_fis_id );

            if( def_fis_id > 0)
            {
                MUH_ENTEGRASYON_FIS_SATIRLARINI_GUNCELLE( P_FIS_SATIRI->aciklama, P_FIS->program_id, P_FIS->modul_id, def_fis_id, P_FIS->base_fis_id, P_FIS_SATIRI->base_record_id );

            }
            if( car_fis_id > 0 )
            {
                if( P_FIS->program_id NE E9_PROGRAMI OR P_FIS->modul_id NE CARI_MODULU ) {

                    CARI_FIS_STRUCT        * CARI_FIS        = new CARI_FIS_STRUCT;
                    CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI = new CARI_FIS_SATIRI_STRUCT;

                    CARI_FIS->program_id        = P_FIS->program_id;
                    CARI_FIS->modul_id          = P_FIS->modul_id;
                    CARI_FIS->base_fis_id       = P_FIS->base_fis_id;

                    CARI_FIS_SATIRI->aciklama              = P_FIS_SATIRI->aciklama;
                    CARI_FIS_SATIRI->borc_tutari           = P_FIS_SATIRI->hesaba_giren;
                    CARI_FIS_SATIRI->alacak_tutari         = P_FIS_SATIRI->hesaptan_cikan;
                    CARI_FIS_SATIRI->doviz_id              = banka_para_birimi_id;
                    CARI_FIS_SATIRI->doviz_kuru            = P_FIS_SATIRI->doviz_kuru;
                    CARI_FIS_SATIRI->parite                = P_FIS_SATIRI->parite;
                    CARI_FIS_SATIRI->doviz_kodu            = banka_para_birimi_kodu;
                    CARI_FIS_SATIRI->fis_id                = car_fis_id;
                    CARI_FIS_SATIRI->program_id            = P_FIS_SATIRI->program_id;
                    CARI_FIS_SATIRI->modul_id              = P_FIS_SATIRI->modul_id;
                    CARI_FIS_SATIRI->base_record_id        = P_FIS_SATIRI->base_record_id;

                    CARI_FIS_SATIRINI_GUNCELLE( CARI_FIS, CARI_FIS_SATIRI, p_fis_satiri_id);

                    delete CARI_FIS;
                    delete CARI_FIS_SATIRI;
                }
            }
        }

        query.PREPARE_UPDATE ( "bnk_hareket_fis_satirlari","fis_satiri_id","aciklama" , "fis_satiri_id     = :fis_satiri_id " );

        query.SET_VALUE     ( ":aciklama"                , P_FIS_SATIRI->aciklama           );
        query.SET_VALUE     ( ":fis_satiri_id"           , p_fis_satiri_id                  );

        query.UPDATE();
    }


    return new_satir_id;
}

/**************************************************************************************
                            BNK_GET_ENT_FIS_ID
***************************************************************************************/

int BNK_GET_ENT_FIS_ID ( int p_program_id, int p_modul_id, int p_base_fis_id ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query( USE_DB );

    query.PREPARE_SELECT( "bnk_hareket_fisler","fis_id","program_id = :program_id "
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
                           BNK_GET_ENT_ROW_ID_LIST
***************************************************************************************/

QStringList BNK_GET_ENT_ROW_ID_LIST ( int p_program_id, int p_modul_id, int p_base_record_id , ADAK_SQL * P_GELEN_DB)
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

    query.PREPARE_SELECT( "bnk_hareket_fis_satirlari","fis_satiri_id",
                          "program_id     =   :program_id "
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
                            BNK_GET_FISE_GORE_ENT_ROW_ID_LIST
***************************************************************************************/
QStringList BNK_GET_FISE_GORE_ENT_ROW_ID_LIST(int p_program_id, int p_modul_id, int p_base_record_id, int p_fis_id)
{
    QStringList row_id_list;

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hareket_fis_satirlari","fis_satiri_id",
                          "program_id     =   :program_id "
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
                            BNK_GET_ENT_FIS_ID
***************************************************************************************/

int BNK_GET_ENT_FIS_ID ( int p_program_id, int p_modul_id, int p_base_fis_id, int p_hesap_no_id ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query( USE_DB );

    query.PREPARE_SELECT( "bnk_hareket_fisler","fis_id",
                          "program_id = :program_id "
                          "AND modul_id     = :modul_id "
                          "AND base_fis_id  = :base_fis_id "
                          "AND hesap_no_id  = :hesap_no_id ");


    query.SET_VALUE( ":program_id",     p_program_id );
    query.SET_VALUE( ":modul_id",       p_modul_id );
    query.SET_VALUE( ":base_fis_id",    p_base_fis_id );
    query.SET_VALUE( ":hesap_no_id",    p_hesap_no_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE( 0 ).toInt();

}
