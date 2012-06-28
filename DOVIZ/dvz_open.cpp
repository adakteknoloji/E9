#include "sekme_menu_kernel.h"

#include "dvz_open.h"



/**************************************************************************************
                   DOVIZ_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * DOVIZ_GET_MENU_STRUCT ()
{
    static struct MENU_STRUCT DOVIZ_MENU_STRUCT[] = {
            { MENU_DVZ_ISLEM_HEADER          , "İşlemler"        , "DVZ1" , "" , "" ,true  ,1 , 0 },
            { MENU_DVZ_DOVIZLER              , "    Döviz Kurları"   , "DVZ2" , "" , "" ,false,1 , 0 },
            { MENU_DVZ_TANIMLARI             , "    Döviz Tanımları" , "DVZ3" , "" , "" ,false,1 , 0 },
            { MENU_DVZ_KURLARI_GUNCELLE      , "    Kurları Güncelle", "DVZ4" , "" , "" ,false,1 , 0 },
            { MENU_END                       , "","",  "" , "" ,false,1,0 }
    };

    return DOVIZ_MENU_STRUCT;

}

/**************************************************************************************
                   DOVIZ_ITEM_CLICKED
***************************************************************************************/

void DOVIZ_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    switch ( enum_id ) {
        case MENU_DVZ_DOVIZLER :
            OPEN_DOVIZLER_BATCH        (parent);
            break;
        case MENU_DVZ_TANIMLARI:
            OPEN_DVZ_TANIMLARI_BATCH   (parent);
            break;
        case MENU_DVZ_KURLARI_GUNCELLE:
            OPEN_DVZ_KUR_GUNCELLE_BATCH(parent);
            break;
        default :
            break;
    }
}

