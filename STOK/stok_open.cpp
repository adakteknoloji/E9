#include "sekme_menu_kernel.h"
#include "adak_sql.h"
#include "adak_gruplar.h"
#include "yonetim.h"
#include "stok_enum.h"
#include "stok_open.h"
#include "e9_gui_utils.h"
#include "stok_urun_karti_import_open.h"


extern ADAK_SQL * DB;


/**************************************************************************************
                   STOK_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * STOK_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT STOK_MENU_STRUCT[] = {
            { MENU_STOK_FISLER_HEADER           , QObject::tr("Fişler")                         , ""     , "", "", true,  1, 0 },
            { MENU_STOK_SARF_FISI               , QObject::tr("     Sarf / Fire Fişi")          , "STK1" , "", "", false, 1, 0 },
            { MENU_STOK_IMALATA_SEVK_FISI       , QObject::tr("     İmalata Sevk Fişi")         , "STK2" , "", "", false, 1, 0 },
            { MENU_STOK_IMALATTAN_DONUS_FISI    , QObject::tr("     İmalattan Dönüş Fişi")      , "STK3" , "", "", false, 1, 0 },
            { MENU_STOK_DEPO_TRANSFER_GIRIS_FISI, QObject::tr("     Depo Transfer Giriş Fişi")  , "STK3" , "", "", false, 1, 0 },
            { MENU_STOK_DEPO_TRANSFER_CIKIS_FISI, QObject::tr("     Depo Transfer Çıkış Fişi")  , "STK5" , "", "", false, 1, 0 },
            { MENU_STOK_ACILIS_FISI             , QObject::tr("     Açılış Fişi")               , "STK6" , "", "", false, 1, 0 },

            { MENU_STOK_RAPORLAR_HEADER         , QObject::tr("Raporlar")                       , ""    ,  "", "", true,  1, 0 },
            { MENU_STOK_URUN_LISTESI            , QObject::tr("     Ürün Listesi")              , "STK10", "", "", false, 1, 0 },
            { MENU_STOK_HIZMET_LISTESI          , QObject::tr("     Hizmet Listesi")            , "STK11", "", "", false, 1, 0 },
            { MENU_STOK_MIKTARLARI_RAPORU       , QObject::tr("     Stok Miktarları Raporu")    , "STK12", "", "", false, 1, 0 },
            { MENU_STOK_EKSTRESI                , QObject::tr("     Stok Ekstresi")             , "STK13", "", "", false, 1, 0 },

            { MENU_STOK_TANIMLAR_HEADER         , QObject::tr("Tanımlar")                       , ""      , "", "", true,  1, 0 },
            { MENU_STOK_URUN_KARTI              , QObject::tr("     Ürün Kartı")                , "STK14" , "", "", false, 1, 0 },
            { MENU_STOK_HIZMET_KARTI            , QObject::tr("     Hizmet Kartı")              , "STK15" , "", "", false, 1, 0 },
            { MENU_STOK_GRUPLARI                , QObject::tr("     Stok Grupları")             , "STK16" , "", "", false, 1, 0 },

            { MENU_STOK_ISLEMLER_HEADER         , QObject::tr("İşlemler")                       , ""      , "", "", true,  1, 0 },
            { MENU_STOK_URUN_IMPORT             , QObject::tr("     Ürünlerinizi İçeri Alın")   , "STK17" , "", "", false, 1, 0 },
            { MENU_STOK_URUN_EXPORT             , QObject::tr("     Ürünlerinizi Dışarı Kaydedin")   , "STK18" , "", "", false, 1, 0 },

            { MENU_END                      , ""  ,"" ,  "" ,"",false,1,0}
    };

    return STOK_MENU_STRUCT;
}

/**************************************************************************************
               STOK_ITEM_CLICKED
***************************************************************************************/

void STOK_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    if ( E9_SUBEYE_UNITE_KAYDI_YAPILDI_MI() EQ false ) {
        return;
    }

    switch ( enum_id ) {

        case MENU_STOK_URUN_KARTI            :
            OPEN_STOK_URUN_KARTI_FORMU ( -1, parent );
            break;

        case MENU_STOK_HIZMET_KARTI :
            OPEN_STOK_HIZMET_KARTI_FORMU ( parent );
            break;

        case MENU_STOK_SARF_FISI:
            OPEN_STOK_FISI ( -1, ENUM_STOK_FISI, ENUM_STK_SARF_FIRE_FISI, -1 , parent );
            break;
        case MENU_STOK_IMALATA_SEVK_FISI:
            OPEN_STOK_FISI ( -1, ENUM_STOK_FISI, ENUM_STK_IMALATA_SEVK_FISI, -1 ,  parent );
            break;
        case MENU_STOK_IMALATTAN_DONUS_FISI:
            OPEN_STOK_FISI ( -1, ENUM_STOK_FISI, ENUM_STK_IMALATTAN_DONUS_FISI, -1, parent );
            break;
        case MENU_STOK_DEPO_TRANSFER_GIRIS_FISI:
            OPEN_STOK_FISI ( -1, ENUM_STOK_FISI, ENUM_STK_DEPO_TRANSFER_GIRIS, -1, parent );
            break;

        case MENU_STOK_DEPO_TRANSFER_CIKIS_FISI:
            OPEN_STOK_FISI ( -1, ENUM_STOK_FISI, ENUM_STK_DEPO_TRANSFER_CIKIS, -1, parent );
            break;

        case MENU_STOK_ACILIS_FISI :
            OPEN_STOK_FISI ( -1, ENUM_STOK_FISI, ENUM_STK_STOK_ACILIS_FISI, -1, parent );
            break;

        case MENU_STOK_GRUPLARI        :
            OPEN_GRUPLAR ( E9_PROGRAMI , STOK_MODULU , QObject::tr( "STK - STOK GRUPLAR" ),"stok-islemleri_stok-gruplari", DB , parent );
            break;

        case MENU_STOK_URUN_LISTESI :
            OPEN_STOK_URUN_LISTESI_BATCH( ENUM_URUN_KARTI, parent);
            break;

        case MENU_STOK_HIZMET_LISTESI :
            OPEN_STOK_URUN_LISTESI_BATCH( ENUM_HIZMET_KARTI, parent);
            break;

        case MENU_STOK_MIKTARLARI_RAPORU :
            OPEN_STOK_MIKTARLARI_BATCH(parent);
            break;

        case MENU_STOK_EKSTRESI  :
            OPEN_STOK_EKSTRESI_BATCH(parent);
            break;
            break;

        case MENU_STOK_URUN_IMPORT  :
            OPEN_STOK_URUN_KARTI_IMPORT( parent );
            break;

        case MENU_STOK_URUN_EXPORT :
            OPEN_STOK_URUN_ARAMA( ENUM_URUN_KARTI , -1 , "" , -1 , parent , STOK_URUN_KARTI_EXPORT_ISLEMI );
            break ;
        default:
            break;
    }
}
