#include "sekme_menu_kernel.h"

#include "adak_sql.h"
#include "adak_gruplar.h"
#include "yonetim.h"

#include "fat_enum.h"
#include "fat_struct.h"
#include "fat_open.h"
#include "sube_unite_detaylari_formu_open.h"
#include "sube_console_utils.h"
extern ADAK_SQL * DB;

/**************************************************************************************
                   FATURA_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * FATURA_GET_MENU_STRUCT()
{

      static struct MENU_STRUCT FATURA_MENU_STRUCT[] = {

          { MENU_FAT_SATIS_FAT_HEADER                               , QObject::tr ( "Giden Faturalar"  )                           , ""        , ""    , "", true, 1, 0 },
            { MENU_FAT_SATIS_FATURALARI                             , QObject::tr ( "   Satış Faturaları" )                        , "FAT1"    , ""    , "", false,1, 0 },
            { MENU_FAT_ALISTAN_IADE                                 , QObject::tr ( "   Alıştan İade Faturaları"  )                , "FAT2"    , ""    , "", false,1, 0 },

            { MENU_FAT_ALIS_FAT_HEADER                              , QObject::tr ( "Gelen Faturalar"  )                           , ""        , ""    , "", true, 1, 0 },
            { MENU_FAT_ALIS_FATURALARI                              , QObject::tr ( "   Alış Faturaları" )                         , "FAT3"    , ""    , "", false,1, 0 },
            { MENU_FAT_SATISTAN_IADE                                , QObject::tr ( "   Satıştan İade Faturaları"  )               , "FAT4"    , ""    , "", false,1, 0 },

            { MENU_FAT_ISLEM_HEADER                                 , QObject::tr ( "İşlemler"  )                                  , ""        , ""    , "", true, 1, 0 },
            { MENU_FAT_SATIS_FAT_ISLEM_HEADER                       , QObject::tr ( "   Satış"  )                                  , ""        , ""    , "", true, 1, 0 },
            { MENU_FAT_SATIS_OTOMATIK_IRS_FATURALASTIRMA            , QObject::tr ( "       İrsaliye Faturalaştırma")              , "FAT4"    , ""    , "", false,1, 0 },
            { MENU_FAT_SATIS_ELLE_IRS_FATURALASTIRMA                , QObject::tr ( "       İrsaliyeyi Kısmen Faturalaştırma")     , "FAT5"    , ""    , "", false,1, 0 },
            { MENU_FAT_SATIS_ELLE_IRS_FATURALASTIRMA_KONSINYE       , QObject::tr ( "       Konsinye İrsaliyesini Faturalaştırma") , "FAT6"    , ""    , "", false,1, 0 },
            { MENU_FAT_TOPLU_IRS_FATURALASTIRMA                     , QObject::tr ( "       Toplu İrsaliye Faturalaştırma")        , "FAT7"    , ""    , "", false,1, 0 },
            { MENU_FAT_YAZICIDAKI_BELGE_NOLARI                      , QObject::tr ( "       Yazıcıdaki Belge Numaraları" )         , "FAT8"    , ""    , "", false,1, 0 },

            { MENU_FAT_ALIS_FAT_ISLEM_HEADER                        , QObject::tr ( "   Alış"  )                                   , ""        , ""    , "", true, 1, 0 },
            { MENU_FAT_ALIS_OTOMATIK_IRS_FATURALASTIRMA             , QObject::tr ( "       İrsaliye Faturalaştırma")              , "FAT9"    , ""    , "", false,1, 0 },
            { MENU_FAT_ALIS_ELLE_IRS_FATURALASTIRMA                 , QObject::tr ( "       İrsaliyeyi Kısmen Faturalaştırma")     , "FAT10"    , ""    , "", false,1, 0 },
            { MENU_FAT_ALIS_ELLE_IRS_FATURALASTIRMA_KONSINYE        , QObject::tr ( "       Konsinye İrsaliyesini Faturalaştırma "), "FAT11"   , ""    , "", false,1, 0 },

            { MENU_FAT_RAPORLAR_HEADER                              , QObject::tr ( "Raporlar"  )                                  , ""        , ""    , "", true, 1, 0 },
            { MENU_FAT_LISTESI                                      , QObject::tr ( "   Fatura Listesi")                           , "FAT12"   , ""    , "", false,1, 0 },
            { MENU_FAT_HIZMET_EKSTRESI                              , QObject::tr ( "   Hizmet Ekstresi"  )                        , "FAT13"   , ""    , "", false,1, 0 },
            { MENU_FAT_BA_BS_FORMU                                  , QObject::tr ( "   BA-BS Formlari Listesi" )                  , "FAT14"   , ""    , "", false,1, 0 },

            { MENU_END                  , "" , "" , "", "", false,1,0}
    };

    return FATURA_MENU_STRUCT;
}

/**************************************************************************************
                   FATURA_ITEM_CLICKED
***************************************************************************************/

void FATURA_ITEM_CLICKED(int enum_id,QWidget * parent){
    switch ( enum_id ) {
        case MENU_FAT_SATIS_FATURALARI:
            OPEN_FATURA_FISI ( ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS, -1, ENUM_FAT_NORMAL,    parent );
            break;
        case MENU_FAT_ALISTAN_IADE :
            OPEN_FATURA_FISI ( ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS,  -1, ENUM_FAT_IADE,      parent );
            break;
        case MENU_FAT_ALIS_FATURALARI:
            OPEN_FATURA_FISI ( ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS,  -1, ENUM_FAT_NORMAL,    parent );
            break;
        case MENU_FAT_SATISTAN_IADE :
            OPEN_FATURA_FISI ( ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS, -1, ENUM_FAT_IADE,      parent );
            break;

        case MENU_FAT_SATIS_ELLE_IRS_FATURALASTIRMA:
            OPEN_FATURA_FISI ( ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS , -1, ENUM_FAT_NORMAL, parent );
            break;

        case MENU_FAT_SATIS_OTOMATIK_IRS_FATURALASTIRMA:
            OPEN_FATURA_FISI ( ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS, -1, ENUM_FAT_NORMAL, parent );
            break;

        case MENU_FAT_SATIS_ELLE_IRS_FATURALASTIRMA_KONSINYE:
            OPEN_FATURA_FISI ( ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS, -1, ENUM_FAT_NORMAL, parent );
            break;

        case MENU_FAT_ALIS_ELLE_IRS_FATURALASTIRMA:
            OPEN_FATURA_FISI ( ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS, -1, ENUM_FAT_NORMAL, parent );
            break;

        case MENU_FAT_ALIS_OTOMATIK_IRS_FATURALASTIRMA:
            OPEN_FATURA_FISI ( ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS, -1, ENUM_FAT_NORMAL, parent );
            break;

        case MENU_FAT_ALIS_ELLE_IRS_FATURALASTIRMA_KONSINYE:
            OPEN_FATURA_FISI ( ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS, -1, ENUM_FAT_NORMAL, parent );
            break;

        case MENU_FAT_TOPLU_IRS_FATURALASTIRMA :
            OPEN_FAT_TOPLU_FATURALASTIR_ARAMA( parent );
            break;
        case MENU_FAT_LISTESI :
            OPEN_FAT_FATURA_LISTESI_BATCH( parent );
            break;
        case MENU_FAT_BA_BS_FORMU :
            OPEN_FAT_BA_BS_FORMU_LISTESI_BATCH( parent );
            break;
        case MENU_FAT_HIZMET_EKSTRESI :
            OPEN_FAT_HIZMET_EKSTRESI_BATCH(parent);
            break;
        case MENU_FAT_YAZICIDAKI_BELGE_NOLARI:
            OPEN_SUBE_UNITE_DETAYLARI_FORMU(SUBE_GET_UNITENIN_DEPOSU(), FATURA_MODULU, parent);
            break;
        default :
            break;
    }
}
