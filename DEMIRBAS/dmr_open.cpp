#include "sekme_menu_kernel.h"

#include "adak_sql.h"
#include "adak_gruplar.h"
#include "yonetim.h"

#include "adres_enum.h"
#include "dmr_open.h"
#include "dmr_con_utils.h"

extern ADAK_SQL * DB;




/**************************************************************************************
                   DEMIRBAS_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * DEMIRBAS_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT DEMIRBAS_MENU_STRUCT[] = {


            { MENU_DMR_TANIMLAR_HEADER      , QObject::tr ("Tanımlar")                    , ""      ,"", "",true,   1, 0 },
            { MENU_DMR_KAYDI                , QObject::tr ("    Sabit Kıymet Kartları")   , "DMR4"  ,"", "",false,  1, 0 },
            { MENU_DMR_GRUPLARI             , QObject::tr ("    Sabit Kıymet Grupları")   , "DMR8"  ,"", "",false,  1, 0 },
            { MENU_DMR_RAPOR_ISLEM_GRUPLARI , QObject::tr ("    Rapor / İşlem Grupları")  , "DMR9"  ,"", "",false,  1, 0 },

            { MENU_DMR_FISLER_HEADER        , QObject::tr ("Fişler")                      , ""      ,"", "",true,   1, 0 },
            { MENU_DMR_AMORTISMAN_FISI      , QObject::tr ("     Amortisman Fişi")        , "DMR1"  ,"", "",false,  1, 0 },
            { MENU_DMR_YEN_DEGERLEME_FISI   , QObject::tr ("     Yeniden Değerleme Fişi") , "DMR2"  ,"", "",false,  1, 0 },

            { MENU_DMR_ISLEMLER_HEADER      , QObject::tr ("İşlemler")                    , ""      ,"", "",true,   1, 0 },
            { MENU_DMR_SATISI               , QObject::tr ("     Sabit Kıymet Satışları") , "DMR5"  ,"", "",false,  1, 0 },
            { MENU_DMR_TOPLU_AMORTISMAN     , QObject::tr ("     Toplu Amortisman Ayırma"), "DMR6"  ,"", "",false,  1, 0 },
            { MENU_DMR_TOPLU_YEN_DEGERLEME  , QObject::tr ("     Toplu Yeniden Değerleme"), "DMR7"  ,"", "",false,  1, 0 },

            { MENU_DMR_RAPORLAR_HEADER      , QObject::tr ("Raporlar")                     , ""     ,"", "",true,   1, 0 },
            { MENU_DMR_LISTESI              , QObject::tr ("     Sabit Kıymetler Listesi") , "DMR3" ,"", "",false,  1, 0 },

            { MENU_SEPERATOR,                   QObject::tr (""),"","","",false,1 , 0 },


        { MENU_END                      , "","", "", "",false,1,0}
    };

    DMR_CREATE_DEMIRBAS_GRUPLARI();

    return DEMIRBAS_MENU_STRUCT;
}

/**************************************************************************************
                   DEMIRBAS_ITEM_CLICKED
***************************************************************************************/

void DEMIRBAS_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    switch ( enum_id ) {
        case MENU_DMR_KAYDI:
            OPEN_DMR_DEMIRBAS_KARTI ( parent );
            break;
        case MENU_DMR_SATISI :
            OPEN_DMR_SATIS_FORMU (parent);
            break;
        case MENU_DMR_GRUPLARI                  :
        OPEN_GRUPLAR( E9_PROGRAMI , DEMIRBAS_MODULU , QObject::tr( "DMR - SABİT KIYMET GRUPLAR" ), "sabit-kiymet-demirbaslar_sabit-kiymet-gruplari", DB ,parent );
            break;
        case MENU_DMR_RAPOR_ISLEM_GRUPLARI:
        OPEN_GRUPLAR( E9_PROGRAMI , DEMIRBAS_GRUP_MODULU, QObject::tr( "DMR - SABİT KIYMET İŞLEM GRUPLAR" ), "sabit-kiymetler-demirbaslar_rapor-islem-gruplari", DB ,parent );
            break;
        case MENU_DMR_AMORTISMAN_FISI                    :
            OPEN_DMR_AMORTISMAN_FISI ( -1,parent );
            break;
        case MENU_DMR_TOPLU_AMORTISMAN                   :
            OPEN_DMR_AMORTISMAN_AYIRMA_BATCH ( parent );
            break;
        case MENU_DMR_YEN_DEGERLEME_FISI                 :
            OPEN_DMR_YENIDEN_DEGERLEME_FISI ( -1,parent );
            break;
        case MENU_DMR_TOPLU_YEN_DEGERLEME                :
            OPEN_DMR_YENIDEN_DEGERLEME_BATCH ( parent );
            break;
        case MENU_DMR_LISTESI                   :
            OPEN_DMR_DEMIRBAS_LISTESI_BATCH ( parent );
            break;
        default:
            break;
      }
}
