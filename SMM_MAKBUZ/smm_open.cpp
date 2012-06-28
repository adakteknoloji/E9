#include "sekme_menu_kernel.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "adak_gruplar.h"
#include "yonetim.h"
#include "sube_enum.h"
#include "sube_console_utils.h"
#include "sube_secimi_open.h"
#include "yonetim.h"
#include "e9_open.h"
#include "smm_enum.h"
#include "smm_open.h"
#include "e9_ana_menu_open.h"
#include "e9_gui_utils.h"

extern ADAK_SQL * DB;



/**************************************************************************************
                   SMM_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * SMM_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT SMM_MENU_STRUCT[] = {
            { MENU_SMM_FISLER_HEADER      , "Satış" ,"" , "" , "" , true,1 , 0 },
            { MENU_SMM_MAKBUZ_SATIS       , "     Serbest Meslek Makbuzu (Satış)","SMM1" , "" , "" , false,1 , 0 },
            { MENU_SMM_SATILAN_HIZMETLER  , "     Satılan Hizmetler"             ,"SMM2" , "" , "" , false,1 , 0 },

            { MENU_SMM_TANIMLAR_HEADER    , "Alış"                   ,""       , "" , "" , true ,1 , 0 },
            { MENU_SMM_MAKBUZ_ALIS        , "     Serbest Meslek Makbuzu (Alış)" ,"SMM3" , "" , "" , false,1 , 0 },
            { MENU_SMM_ALINAN_HIZMETLER   , "     Alınan Hizmetler"              ,"SMM4" , "" , "" , false,1 , 0 },

#ifdef SM_MAKBUZU_BAGIMSIZ
            { MENU_SEPERATOR              , QObject::tr ( "")           ,""        , "" , "" , false,1, 0 },
            { MENU_YONETIM                , QObject::tr ( "Yönetim")    ,""        , "" , "" , false,1, 0 },
            { MENU_SISTEM                 , QObject::tr ( "Sistem")     ,""        , "" , "" , false,1, 0 },
            { MENU_SEPERATOR              , QObject::tr ( "")           ,""        , "" , "" , false,1, 0 },
            { MENU_SEPERATOR              , QObject::tr ( "")           ,""        , "" , "" , false,1, 0 },
            { MENU_HATA_ISTEK             , QObject::tr ( "Hata/İstek Bildirimi")  , "" , "" ,"",false,1 , 0 },
            { MENU_SEPERATOR              , QObject::tr ( "")           ,""        , "" , "" , false,1, 0 },
            { MENU_SMM_E9                 , QObject::tr ( "E9" )        ,""        , "" , "" , false,1, 0 },
#endif
            { MENU_END                    , ""                          , ""       , "" , "" , false,1 ,0 }
    };

    return SMM_MENU_STRUCT;
}

/**************************************************************************************
                   E9_YENI_ANA_MENU::MUHASEBE_ITEM_CLICKED
***************************************************************************************/

void SMM_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    if ( E9_SUBEYE_UNITE_KAYDI_YAPILDI_MI() EQ false ) {
        return;
    }

    switch ( enum_id ) {
        case MENU_SMM_MAKBUZ_ALIS :
             OPEN_SMM_MAKBUZ_FORMU( -1, SMM_ALIS_MAKBUZU , parent);
             break;
        case MENU_SMM_MAKBUZ_SATIS :
             OPEN_SMM_MAKBUZ_FORMU( -1, SMM_SATIS_MAKBUZU , parent);
             break;
        case MENU_SMM_E9 :
            OPEN_E9_ANA_MENU( NULL );
            break;
        case MENU_SMM_SATILAN_HIZMETLER :
        case MENU_SMM_ALINAN_HIZMETLER:
        {
             int sube_yetkisi = SUBE_GET_SUBE_ISLEM_YETKISI();
             int sube_id = SUBE_GET_SUBE_ID();

             if ( sube_id <= 0 ) {
                 ADAK_WARNING(QObject::tr("Bilgisayarınızın kayıtlı olduğu bir şube bulunamadı."
                                "Lütfen bilgisayarınızı bir şubeye kaydediniz."),NULL,NULL);
                 return;
             }

             if ( sube_yetkisi EQ TUM_SUBELERDE ) {
                 QList < int > sube_id_list = SUBE_GET_SUBE_ID_LIST();
                 if ( sube_id_list.size() > 1 ) {
                     sube_id = SUBE_SEC( NULL );
                 }
             }
             if ( sube_id EQ -1 ) {
                 return;
             }
             if ( enum_id EQ MENU_SMM_ALINAN_HIZMETLER ) {
                 OPEN_SMM_HIZMET_TANIMLAMA_FISI(sube_id , SMM_ALINAN_HIZMET , parent);
             }
             else {
                 OPEN_SMM_HIZMET_TANIMLAMA_FISI(sube_id , SMM_SATILAN_HIZMET , parent);
             }
         }
        break;

    case MENU_HATA_ISTEK:
        OPEN_HATA_ISTEK_BATCH ( parent );
        break;
        default :
            break;
    }
}
