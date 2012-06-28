#include "sekme_menu_kernel.h"

#include "faiz_enum.h"
#include "faiz_open.h"



/**************************************************************************************
                   FAIZ_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * FAIZ_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT FAIZ_MENU_STRUCT[] = {
        { MENU_FAIZ_ISLEM_HEADER              , "İşlemler"              , "FAIZ"     , "" , "" , true ,1 , 0 },
        { MENU_FAIZ_ANA_PARAYI_BUL            , "   Ana Parayı Bul"        , "FAIZ1"    , "" , "" , false,1 , 0 },
        { MENU_FAIZ_TAKSITLENDIR              , "   Taksitlendir"          , "FAIZ2"    , "" , "" , false,1 , 0 },
        { MENU_FAIZ_AYLIK_FAIZ_ORANI_BUL      , "   Aylık Faiz Oranı Bul"  , "FAIZ3"    , "" , "" , false,1 , 0 },
        { MENU_END                            , "", "","" , "" , false,1,0 }
    };

    return FAIZ_MENU_STRUCT;
}

/**************************************************************************************
                  FAIZ_ITEM_CLICKED
***************************************************************************************/

void FAIZ_ITEM_CLICKED(int enum_id,QWidget * parent)
{
   switch ( enum_id ) {

       case MENU_FAIZ_ANA_PARAYI_BUL :
            OPEN_FAIZ_HESAPLA( ANA_PARAYI_BUL, parent );
            break;
       case MENU_FAIZ_TAKSITLENDIR :
            OPEN_FAIZ_HESAPLA( TAKSITLENDIR, parent );
            break;
       case MENU_FAIZ_AYLIK_FAIZ_ORANI_BUL :
            OPEN_FAIZ_HESAPLA( AYLIK_FAIZI_BUL, parent );
            break;
       default :
           break;
   }
}
