#include "sekme_menu_kernel.h"

#include "banka_enum.h"
#include "banka_open.h"
#include "e9_gui_utils.h"



/**************************************************************************************
                   BANKA_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * BANKA_GET_MENU_STRUCT()
{
     static struct MENU_STRUCT BANKA_MENU_STRUCT[] = {
            { MENU_BNK_FISLER_HEADER        , QObject::tr ( "Fişler" )                              , ""       , ""       , "",  true,1 , 0 },
            { MENU_BNK_MAKRO_FISI           , QObject::tr ( "     Makro Fişi" )                     , "BNK1"   , ""       , "",  false,1 , 0 },
            { MENU_BNK_HAREKET_FISI         , QObject::tr ( "     Hareket Fişi" )                   , "BNK2"   , ""      , "",  false,1 , 0 },
            { MENU_BNK_PARA_YAT_CEKME_FISI  , QObject::tr ( "     Para Yatirma / Çekme Fişi" )      , "BNK3"   , ""             , "",  false,1 , 0 },
            { MENU_BNK_VIRMAN_FORMU         , QObject::tr ( "     Banka Virman Formu" )             , "BNK4"   , ""           , "",  false,1 , 0 },
            { MENU_BNK_ACILIS_FISI          , QObject::tr ( "     Banka Açılış Formu" )             , "BNK5"   , ""       , "",  false,1 , 0 },

            { MENU_BNK_RAPORLAR_HEADER      , QObject::tr ( "Raporlar" )                            , ""     , ""         , "",  true,1 , 0 },
            { MENU_BNK_DEFTERI              , QObject::tr ( "     Banka Defteri" )                  , "BNK30"    , ""           , "",  false,1 , 0 },
            { MENU_BNK_POS_EKSTRESI         , QObject::tr ( "     Pos Ekstresi" )                   , "BNK31"    , ""              , "",  false,1 , 0 },
            { MENU_BNK_KK_EKSTRESI          , QObject::tr ( "     Kredi Kartı Ekstresi" )           , "BNK32"   , ""       , "",  false,1 , 0 },


            { MENU_BNK_ISLEMLER_HEADER      , QObject::tr ( "İşlemler" )                            , ""      , ""  , "",  true,1 , 0 },
            { MENU_BNK_HESAPLARI            , QObject::tr ( "     Banka Hesapları" )                , "BNK20"     , ""         , "",  false,1 , 0 },
            { MENU_BNK_MAKRO_TANIMLAMA      , QObject::tr ( "     Makro Tanımlama" )                , "BNK21"      , ""  , "",  false,1 , 0 },

            { MENU_BNK_ENT_FISLER_HEADER    , QObject::tr ( "Entegrasyon" )                         , ""   , ""       , "",  true,1 , 0 },
            { MENU_BNK_ENT_FISLERI          , QObject::tr ( "     Banka Entegrasyon Fişleri" )          , "BNK10"   , ""       , "",  false,1 , 0 },
            { MENU_END                      , "",    "" , ""  , "",  false,1,0 }
    };

    return BANKA_MENU_STRUCT;
}

/**************************************************************************************
                   BANKA_ITEM_CLICKED
***************************************************************************************/

void BANKA_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    if ( E9_SUBEYE_UNITE_KAYDI_YAPILDI_MI() EQ false ) {
        return;
    }

    switch ( enum_id ) {

        case    MENU_BNK_ACILIS_FISI:
            OPEN_BNK_ACILIS_FORMU ( parent );
            break;

        case    MENU_BNK_MAKRO_FISI             :
            OPEN_BNK_MAKRO_FISI ( -1, BANKA_MODULU, parent );
            break;

        case    MENU_BNK_HAREKET_FISI           :
            OPEN_BNK_HAREKET_FISI ( ENUM_BANKA_HAREKET_FISI , -1, BANKA_MODULU, parent );
            ADAK_INFO( "Uzmanlık gerektiren alan dikkat ediniz !",NULL,parent);
            break;

        case    MENU_BNK_PARA_YAT_CEKME_FISI   :
            OPEN_BNK_HAREKET_FISI ( ENUM_BANKA_PARA_YAT_CEKME_FISI, -1 , BANKA_MODULU, parent );
            break;

        case    MENU_BNK_ENT_FISLERI  :
            // DEFAULT CARI SETLENDI ICERIDE COMBOBOX ILE DEGISIKLIK YAPABILIYOR
            OPEN_BNK_HAREKET_FISI_ARAMA ( ENUM_BANKA_HAREKET_FISI, -1 , ENUM_BANKA_ENT_FISI , parent );
            break;

        case    MENU_BNK_HESAPLARI              :
            OPEN_BNK_HESAPLARI_FISI ( -1, parent );
            break;

        case    MENU_BNK_MAKRO_TANIMLAMA            :
            OPEN_BNK_MAKRO_TANIMLA ( -1, parent );
            break;

        case    MENU_BNK_DEFTERI                :
            OPEN_BNK_BANKA_DEFTERI_BATCH ( parent );
            break;

        case    MENU_BNK_POS_EKSTRESI               :
            OPEN_BNK_POS_EKSTRESI_BATCH ( parent );
            break;

        case    MENU_BNK_KK_EKSTRESI                :
            OPEN_BNK_KK_EKSTRESI_BATCH ( parent );
            break;

        case    MENU_BNK_VIRMAN_FORMU               :
            OPEN_BNK_VIRMAN_FORMU( -1, parent );
            break;

        default :
            break;
    }
}
