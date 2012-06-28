#include "sekme_menu_kernel.h"

#include "isl_open.h"
#include "isl_enum.h"


/**************************************************************************************
                  ISLETME_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * ISLETME_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT ISLETME_MENU_STRUCT[] = {
        { MENU_ISL_FISLER_HEADER        , QObject::tr ( "Fişler"  )                      , ""     , ""  , "", true,1, 0 },
        { MENU_ISL_GELIR_FISI           , QObject::tr ( "     Gelir Fişi"  )             , "ISL1" , ""  , "", false,1, 0 },
        { MENU_ISL_GIDER_FISI           , QObject::tr ( "     Gider Fişi"  )             , "ISL2" , ""  , "", false,1, 0 },
        { MENU_ISL_RAPORLAR_HEADER      , QObject::tr ( "Raporlar"  )                    , ""     , ""  , "", true,1, 0 },
        { MENU_ISL_KDV_RAPORU           , QObject::tr ( "     KDV Raporu"  )             , "ISL3" , ""  , "", false,1, 0 },
        { MENU_ISL_DEFTERI_RAPORU       , QObject::tr ( "     İşletme Defteri Raporu"  ) , "ISL4" , ""  , "", false,1, 0 },
        { MENU_ISL_HESAP_OZETI          , QObject::tr ( "     İşletme Hesap Özeti"  )    , "ISL5" , ""  , "", false,1, 0 },
        { MENU_END,   "" , "" ,"", "" ,false,1,0}
    };

    return ISLETME_MENU_STRUCT;
}

/**************************************************************************************
                   ISLETME_ITEM_CLICKED
***************************************************************************************/

 void ISLETME_ITEM_CLICKED(int enum_id,QWidget * parent)
 {
     switch ( enum_id ) {
        case MENU_ISL_GELIR_FISI :
            OPEN_ISL_ISLETME_FISI(-1 , ENUM_GELIR_FISI, parent);
            break;
        case MENU_ISL_GIDER_FISI :
            OPEN_ISL_ISLETME_FISI(-1 , ENUM_GIDER_FISI, parent);
            break;
        case MENU_ISL_KDV_RAPORU :
            OPEN_ISL_KDV_RAPORU_BATCH(parent);
            break;
        case MENU_ISL_DEFTERI_RAPORU :
            OPEN_ISL_DEFTERI_RAPORU_BATCH(parent);
            break;
        case MENU_ISL_HESAP_OZETI :
            OPEN_ISL_HESAP_OZETI_BATCH(parent);
            break;
        default :
            break;
     }


 }
