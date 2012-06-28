#include "sekme_menu_kernel.h"
#include "sube_open.h"
#include "sube_tree_batch_open.h"

/**************************************************************************************
                   SUBE_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * SUBE_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT SUBE_MENU_STRUCT[] = {
            { MENU_SUBE_SUBELER               , QObject::tr("Şube/Depo/Ünite/Kasa")   , "SUB1" , ""           ,"",false,1, 0 },
            { MENU_SUBE_ORGANIZASYON_SEMASI   , QObject::tr("Organizasyon Şeması")    , "SUB2" , ""           ,"",false,1, 0 },
            { MENU_END                        , ""  ,"" ,  "" ,"",false,1,0}
    };

    return SUBE_MENU_STRUCT;
}

/**************************************************************************************
                   SUBE_ITEM_CLICKED
***************************************************************************************/

void SUBE_ITEM_CLICKED(int p_item, QWidget * parent )
{
    switch ( p_item ) {
        case MENU_SUBE_SUBELER:
            OPEN_SUBE_FISI( parent );
            break;

        case MENU_SUBE_ORGANIZASYON_SEMASI:
            OPEN_SUBE_TREE_BATCH( parent );
            break;

        default:
            break;
    }
}


