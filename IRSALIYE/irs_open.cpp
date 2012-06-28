#include "sekme_menu_kernel.h"
#include "stok_fisi_open.h"
#include "irs_enum.h"
#include "stok_enum.h"
#include "irs_open.h"
#include "sube_unite_detaylari_formu_open.h"
#include "sube_console_utils.h"


/**************************************************************************************
                   ADRES_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * IRSALIYE_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT IRSALIYE_MENU_STRUCT[] = {
            { MENU_IRS_SATIS_IRS_HEADER                               , QObject::tr ( "Giden İrsaliyeler"  )                     , ""       , ""  , "", true, 1, 0 },
            { MENU_IRS_SATIS_IRSALIYELERI                             , QObject::tr ( "     Satış İrsaliyeleri"  )               , "IRS1"   , ""  , "", false,1, 0 },
            { MENU_IRS_ALISTAN_IADE                                   , QObject::tr ( "     Alıştan İade İrsaliyeleri"  )        , "IRS2"   , ""  , "", false,1, 0 },
            { MENU_IRS_FASON_BASLANGIC_CIKIS                          , QObject::tr ( "     Fason Başlangıç İrsaliyeleri"  )     , "IRS3"   , ""  , "", false,1, 0 },
            { MENU_IRS_FASON_BITIS_CIKIS                              , QObject::tr ( "     Fason Bitiş İrsaliyeleri"  )         , "IRS3"   , ""  , "", false,1, 0 },

            { MENU_IRS_ALIS_IRS_HEADER                                , QObject::tr ( "Gelen İrsaliyeler"  )                     , ""       , ""  , "", true, 1, 0 },
            { MENU_IRS_ALIS_IRSALIYELERI                              , QObject::tr ( "     Alış İrsaliyeleri"  )                , "IRS4"  , ""  , "", false,1, 0 },
            { MENU_IRS_SATISTAN_IADE                                  , QObject::tr ( "     Satıştan İade İrsaliyeleri"  )       , "IRS5"  , ""  , "", false,1, 0 },
            { MENU_IRS_FASON_BASLANGIC_GIRIS                          , QObject::tr ( "     Fason Başlangıç İrsaliyeleri"  )     , "IRS6"   , ""  , "", false,1, 0 },
            { MENU_IRS_FASON_BITIS_GIRIS                              , QObject::tr ( "     Fason Bitiş İrsaliyeleri"  )         , "IRS6"   , ""  , "", false,1, 0 },

            { MENU_IRS_ISLEM_HEADER                                   , QObject::tr ( "İşlemler"  )                              , ""       , ""  , "", true, 1, 0 },
            { MENU_IRS_SATIS_IRS_ISLEM_HEADER                         , QObject::tr ( "   Satış"  )                              , ""       , ""  , "", true, 1, 0 },
            { MENU_IRS_SATIS_OTOMATIK_FAT_IRSALIYELESTIRME            , QObject::tr ( "       Fatura İrsaliyeleştirme")          , "IRS7"  , ""  , "", false,1, 0 },
            { MENU_IRS_SATIS_ELLE_FAT_IRSALIYELESTIRME                , QObject::tr ( "       Fatura Kısmen İrsaliyeleştirme")   , "IRS8"  , ""  , "", false,1, 0 },
            { MENU_IRS_YAZICIDAKI_BELGE_NOLARI                        , QObject::tr ( "       Yazıcıdaki Belge Numaraları"  )    , "IRS9"  , ""  , "", false,1, 0 },

            { MENU_IRS_ALIS_IRS_ISLEM_HEADER                          , QObject::tr ( "   Alış"  )                               , ""        , "" , "", true, 1, 0 },
            { MENU_IRS_ALIS_OTOMATIK_FAT_IRSALIYELESTIRME             , QObject::tr ( "       Fatura İrsaliyeleştirme")          , "IRS10"   , "" , "", false,1, 0 },
            { MENU_IRS_ALIS_ELLE_FAT_IRSALIYELESTIRME                 , QObject::tr ( "       Fatura Kısmen İrsaliyeleştirme")   , "IRS11"   , "" , "", false,1, 0 },

            { MENU_IRS_RAPORLAR_HEADER                                , QObject::tr ( "Raporlar"  )                              , ""        , "" , "", true, 1, 0 },
            { MENU_IRS_LISTESI                                        , QObject::tr ( "     İrsaliye Listesi"  )                 , "IRS12"   , "" , "", false,1, 0 },
            { MENU_END                                                , "" , "" , "", "", false,1,0}
    };

    return IRSALIYE_MENU_STRUCT;
}

/**************************************************************************************
                   IRSALIYE_ITEM_CLICKED
***************************************************************************************/

void IRSALIYE_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    switch ( enum_id ) {

        case MENU_IRS_SATIS_IRSALIYELERI:
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_NORMAL_CIKIS, ENUM_IRS_NORMAL, parent );
            break;
        case MENU_IRS_ALISTAN_IADE:
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_NORMAL_CIKIS, ENUM_IRS_IADE, parent );
            break;
        case MENU_IRS_FASON_BASLANGIC_CIKIS:
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_FASON_BASLANGIC_CIKIS, ENUM_IRS_NORMAL, parent );
            break;
        case MENU_IRS_FASON_BITIS_CIKIS:
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_FASON_BITIS_CIKIS, ENUM_IRS_NORMAL, parent );
            break;
        case MENU_IRS_ALIS_IRSALIYELERI :
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_NORMAL_GIRIS, ENUM_IRS_NORMAL, parent );
            break;
        case MENU_IRS_SATISTAN_IADE :
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_NORMAL_GIRIS, ENUM_IRS_IADE, parent );
            break;
        case MENU_IRS_FASON_BASLANGIC_GIRIS :
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_FASON_BASLANGIC_GIRIS, ENUM_IRS_NORMAL, parent );
            break;
        case MENU_IRS_FASON_BITIS_GIRIS :
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_FASON_BITIS_GIRIS, ENUM_IRS_NORMAL, parent );
            break;
        case MENU_IRS_SATIS_OTOMATIK_FAT_IRSALIYELESTIRME :
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS, ENUM_IRS_NORMAL, parent );
            break;
        case MENU_IRS_SATIS_ELLE_FAT_IRSALIYELESTIRME :
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS, ENUM_IRS_NORMAL, parent );
            break;
        case MENU_IRS_ALIS_OTOMATIK_FAT_IRSALIYELESTIRME :
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS, ENUM_IRS_NORMAL, parent );
            break;
        case MENU_IRS_ALIS_ELLE_FAT_IRSALIYELESTIRME :
            OPEN_STOK_FISI ( -1, ENUM_IRSALIYE, ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS, ENUM_IRS_NORMAL, parent );
            break;


        case MENU_IRS_LISTESI:
            OPEN_IRS_IRSALIYE_LISTESI_BATCH(parent);
            break;
        case MENU_IRS_YAZICIDAKI_BELGE_NOLARI:
            OPEN_SUBE_UNITE_DETAYLARI_FORMU(SUBE_GET_UNITENIN_DEPOSU(), IRSALIYE_MODULU, parent);
            break;
        default :
            break;
    }
}
