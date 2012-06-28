#include "sekme_menu_kernel.h"

#include "adak_sql.h"
#include "adak_gruplar.h"
#include "yonetim.h"

#include "muh_enum.h"
#include "muh_open.h"
#include "muh_fis_arama_open.h"

extern ADAK_SQL * DB;



/**************************************************************************************
                   MUHASEBE_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * MUHASEBE_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT MUHASEBE_MENU_STRUCT[] = {
            { MENU_MUH_FISLER_HEADER        , QObject::tr ("Fişler")                                     , ""         , ""       , "", true,1, 0 },
            { MENU_MUH_MAHSUP_FISI          , QObject::tr ("    Mahsup Fişi")                            , "MUH1"     , ""       , "", false,1, 0 },
            { MENU_MUH_TAHSIL_FISI          , QObject::tr ("    Tahsil Fişi")                            , "MUH2"     , ""       , "", false,1, 0 },
            { MENU_MUH_TEDIYE_FISI          , QObject::tr ("    Tediye Fişi")                            , "MUH3"     , ""       , "", false,1, 0 },
            { MENU_MUH_ACILIS_FISI          , QObject::tr ("    Açılış Fişi")                            , "MUH4"     , ""      , "", false,1, 0 },
            { MENU_MUH_KAPANIS_FISI         , QObject::tr ("    Kapanış Fişi")                           , "MUH5"     , ""      , "", false,1, 0 },


            { MENU_MUH_ISLEMLER_HEADER      , QObject::tr ("İşlemler")                                   , ""         , ""   , "", true,2, 0 },
            { MENU_MUH_YVM_NO_OLUSTUR       , QObject::tr ("     Yevmiye Numaralarını Oluştur")          , "MUH40"    , ""            , "", false,2, 0 },
            { MENU_MUH_YANSITMA_HESAPLARI   , QObject::tr ("     Yansıtma Hesapları")                    , "MUH41"    , ""            , "", false,2, 0 },
            { MENU_MUH_YANSITMA_FISI_OLUSTUR, QObject::tr ("     Yansıtma Fişi Oluştur")                 , "MUH42"    , ""            , "", false,2, 0 },
            { MENU_MUH_HESAP_PLANI_KOPYALA  , QObject::tr ("     Hesap Planı Kopyala")                   , "MUH43"    , ""            , "", false,2, 0 },


            { MENU_MUH_ENT_FISLERI_HEADER   , QObject::tr ("Entegrasyon")                               , ""         , "" , "", true,2, 0 },
            { MENU_MUH_ENT_FISLERI          , QObject::tr ("    Muhasebe Entegrasyon Fişleri")          , "MUH10"    , ""      , "", false,2, 0 },


            { MENU_MUH_HESAPLAR_HEADER      , QObject::tr ("Hesaplar")                                   , ""         , ""     , "", true,1, 0 },
            { MENU_MUH_HESAP_PLANI          , QObject::tr ("     Hesap Planı")                           , "MUH30"    , ""     , "", false,1, 0 },
            { MENU_MUH_HESAP_GRUPLARI       , QObject::tr ("     Gruplar")                               , "MUH31"    , ""             , "", false,1, 0 },


            { MENU_MUH_RAPORLAR_HEADER      , QObject::tr ("Raporlar")                                   , ""         , ""   , "", true,1, 0 },
            { MENU_MUH_HESAP_EKSTRESI       , QObject::tr ("     Hesap Ekstresi ( Muavin )")                        , "MUH50"    , ""   , "", false,1, 0 },
            { MENU_MUH_MIZAN                , QObject::tr ("     Mizan")                                 , "MUH51"    , ""           , "", false,1, 0 },
            { MENU_MUH_HATALI_FISLER        , QObject::tr ("     Hatalı Fişler")                         , "MUH52"    , ""            , "", false,1, 0 },
            { MENU_MUH_HESAP_PLANI_RAPORU   , QObject::tr ("     Hesap Planı Raporu")                    , "MUH53"    , ""            , "", false,1, 0 },
            { MENU_MUH_TOPLU_FIS_DOKUMU     , QObject::tr ("     Toplu Fiş Dökümü")                      , "MUH54"    , ""            , "", false,1, 0 },
            { MENU_MUH_YEVMIYE_DOKUMU       , QObject::tr ("     Yevmiye Dökümü")                        , "MUH55"    , ""            , "", false,1, 0 },
            { MENU_MUH_DEFTERI_KEBIR        , QObject::tr ("     Defteri Kebir")                         , "MUH56"    , ""            , "", false,1, 0 },
            { MENU_MUH_BILANCO              , QObject::tr ("     Bilanço")                               , "MUH57"    , ""            , "", false,1, 0 },
            { MENU_MUH_GELIR_TABLOSU        , QObject::tr ("     Gelir Tablosu")                         , "MUH58"    , ""            , "", false,1, 0 },

            { MENU_END                      , "" , "" ,  "", "", false,1,0}

    };

    return MUHASEBE_MENU_STRUCT;
}

/**************************************************************************************
                   MUHASEBE_ITEM_CLICKED
***************************************************************************************/

void MUHASEBE_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    switch ( enum_id ) {
        case MENU_MUH_YVM_NO_OLUSTUR :
            OPEN_MUH_YEVMIYE_NO_OLUSTUR_BATCH ( parent );
            break;

        case MENU_MUH_MAHSUP_FISI :
            OPEN_MUHASEBE_FISI ( -1, ENUM_MAHSUP_FISI, MUHASEBE_MODULU, parent );
            break;
        case MENU_MUH_TAHSIL_FISI :
            OPEN_MUHASEBE_FISI ( -1, ENUM_TAHSIL_FISI, MUHASEBE_MODULU, parent );
            break;
        case MENU_MUH_TEDIYE_FISI :
            OPEN_MUHASEBE_FISI ( -1, ENUM_TEDIYE_FISI, MUHASEBE_MODULU, parent );
            break;
        case MENU_MUH_ACILIS_FISI :
            OPEN_MUHASEBE_FISI ( -1, ENUM_ACILIS_FISI, MUHASEBE_MODULU, parent );
            break;
        case MENU_MUH_KAPANIS_FISI:
            OPEN_MUHASEBE_FISI ( -1, ENUM_KAPANIS_FISI, MUHASEBE_MODULU, parent );
            break;
        case MENU_MUH_HESAP_PLANI :
            OPEN_MUH_HESAP_PLANI ( parent );
            break;

        case MENU_MUH_HESAP_GRUPLARI :
        OPEN_GRUPLAR( E9_PROGRAMI, MUHASEBE_MODULU, QObject::tr( "MUH - MUHASEBE GRUPLAR" ), "genel-muhasebe_gruplar", DB, parent );
            break;

        case MENU_MUH_HESAP_EKSTRESI :
            OPEN_MUH_HESAP_EKSTRESI_BATCH ( "",ENUM_MUH_HESAP_EKSTRESI , parent );
            break;

        case MENU_MUH_MIZAN :
            OPEN_MUH_MIZAN_RAPORU_BATCH ( parent );
            break;

        case MENU_MUH_HATALI_FISLER:
            OPEN_HATALI_FIS_SEC                 ( parent );
            break;
        case MENU_MUH_HESAP_PLANI_RAPORU :
            OPEN_MUH_HESAP_PLANI_RAPORU_BATCH   ( parent );
            break;
        case MENU_MUH_TOPLU_FIS_DOKUMU :
            OPEN_MUH_TOPLU_FIS_RAPORU_BATCH     ( parent );
            break;
        case MENU_MUH_YEVMIYE_DOKUMU :
            OPEN_MUH_YEVMIYE_DOKUMU_BATCH       ( parent );
            break;
        case MENU_MUH_DEFTERI_KEBIR :
            OPEN_MUH_DEFTERI_KEBIR_BATCH        ( parent );
            break;
        case MENU_MUH_BILANCO :
            OPEN_MUH_BILANCO_RAPORU_BATCH       ( parent );
            break;
        case MENU_MUH_GELIR_TABLOSU :
            OPEN_MUH_GELIR_TABLOSU_BATCH        ( parent );
            break;
        case MENU_MUH_YANSITMA_FISI_OLUSTUR  :
            OPEN_MUH_YANSITMA_ISLEMI_BATCH      ( parent );
            break;
        case MENU_MUH_YANSITMA_HESAPLARI :
            OPEN_MUH_YANSITMA_HESAPLARI_FISI    ( parent );
            break;
        case MENU_MUH_ENT_FISLERI :
            OPEN_MUH_FIS_ARAMA ( 0, -1 , ENUM_MAHSUP_FISI , ENUM_MUHASEBE_ENT_FISI , parent );
            break;
        case MENU_MUH_HESAP_PLANI_KOPYALA :
            OPEN_MUH_HESAP_PLANI_KOPYALA_BATCH  ( parent );
            break;
        default :
            break;
    }
}
