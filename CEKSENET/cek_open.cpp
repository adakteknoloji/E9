#include "sekme_menu_kernel.h"

#include "cek_enum.h"
#include "cek_open.h"
#include "e9_gui_utils.h"


/**************************************************************************************
                   CEKSENET_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * CEKSENET_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT CEKSENET_MENU_STRUCT[] = {

            { MENU_CIKIS_BRD_HEADER                 , QObject::tr ( "Çek Bordroları"                        ),""     , "", "", true , 1 , 0 },
            { MENU_MUSTERI_CEK_BORDROSU             , QObject::tr ( "     Müşteri Çek Bordrosu"             ),"CEK1" , "", "", false, 1 , 0 },
            { MENU_FIRMA_CEK_BORDROSU               , QObject::tr ( "     Firma Çek Bordrosu"               ),"CEK2" , "", "", false, 1 , 0 },

            { MENU_GIRIS_BRD_HEADER                 , QObject::tr ( "Senet Bordroları"                      ),""     , "", "", true,  1 , 0 },
            { MENU_MUSTERI_SENET_BORDROSU           , QObject::tr ( "     Müşteri Senet Bordrosu"           ),"CEK3", "", "", false, 1 , 0 },
            { MENU_FIRMA_SENET_BORDROSU             , QObject::tr ( "     Firma Senet Bordrosu"             ),"CEK4", "", "", false, 1 , 0 },

            { MENU_CEK_ISLEMLER_HEADER              , QObject::tr ( "İşlemler"                              ),""     , "", "", true , 1 , 0 },
            { MENU_CEK_KENDI_CEKLERIMIZ             , QObject::tr ( "     Kendi Çeklerimiz"                 ),"CEK5", "", "", false, 1 , 0 },
            { MENU_CEK_KENDI_SENETLERIMIZ           , QObject::tr ( "     Kendi Senetlerimiz"               ),"CEK6", "", "", false, 1 , 0 },
            { MENU_CEK_MUS_CEKLERI_SENETLERI        , QObject::tr ( "     Müşteri Çekleri / Senetleri"      ),"CEK7", "", "", false, 1 , 0 },

            { MENU_CEK_RAPORLAR_HEADER              , QObject::tr ( "Raporlar"                              ),""     , "", "", true , 1 , 0 },
            { MENU_CEK_SENET_LISTESI                , QObject::tr ( "     Çek Senet Listesi"                ),"CEK8", "", "", false, 1 , 0 },
            { MENU_CEK_MUS_CEK_ARAMA                , QObject::tr ( "     Müşteri Çekleri Arama"            ),"CEE9", "", "", false, 1 , 0 },
            { MENU_CEK_MUS_SNT_ARAMA                , QObject::tr ( "     Müşteri Senetleri Arama"          ),"CEK10", "", "", false, 1 , 0 },

            { MENU_CEK_ACILIS_BORDROSU_HEADER       , QObject::tr ( "Açılış Bordrosu"                       ) ,""    , "", "", true,  1 , 0 },
            { MENU_CEK_ACILIS_BORDROSU              , QObject::tr ( "     Açılış Bordrosu"                  ) ,"CEK11", "", "", false, 1 , 0 },

            { MENU_CEK_ENT_FISLERI_HEADER           , QObject::tr ( "Entegrasyon"                           ) ,"", ""  , "", true,1 , 0 },
            { MENU_CEK_CARI_ENT_FISILERI            , QObject::tr ( "     Çek / Senet Enregrasyon Fişleri"  ) ,"CEK12", ""  , "", false,1 , 0 },

            { MENU_END                              , "" , "" ,"", "", false,1,0 }
    };

    return CEKSENET_MENU_STRUCT;
}

/**************************************************************************************
                   CEKSENET_ITEM_CLICKED
***************************************************************************************/

void CEKSENET_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    if ( E9_SUBEYE_UNITE_KAYDI_YAPILDI_MI() EQ false ) {
        return;
    }

    switch ( enum_id ) {

        case MENU_CEK_ACILIS_BORDROSU:
            OPEN_CEK_ACILIS_FISI ( -1, parent );
            break;

        case MENU_MUSTERI_CEK_BORDROSU :
            OPEN_CEK_SENET_BORDROSU_FISI ( -1, CEKSENET_MODULU, ENUM_CEK_SATICIYA_VERILDI , parent, ENUM_MUSTERI_CEK_BORDROSU );
            break;
        case MENU_FIRMA_CEK_BORDROSU :
            OPEN_CEK_SENET_BORDROSU_FISI ( -1, CEKSENET_MODULU,ENUM_CEK_SATICIYA_VERILDI, parent, ENUM_FIRMA_CEK_BORDROSU );
            break;

        case MENU_MUSTERI_SENET_BORDROSU :
            OPEN_CEK_SENET_BORDROSU_FISI ( -1, CEKSENET_MODULU, ENUM_CEK_SATICIYA_VERILDI , parent, ENUM_MUSTERI_SENET_BORDROSU );
            break;

        case MENU_FIRMA_SENET_BORDROSU :
            OPEN_CEK_SENET_BORDROSU_FISI ( -1, CEKSENET_MODULU, ENUM_CEK_SATICIYA_VERILDI , parent, ENUM_FIRMA_SENET_BORDROSU );
            break;

        case MENU_CEK_MUS_SNT_BRD_TAH_EDILEMEYEN :
            OPEN_CEK_SENET_BORDROSU_FISI ( -1, CEKSENET_MODULU, ENUM_CEK_TAHSIL_EDILEMEYEN ,parent, ENUM_MUSTERI_SENET_BORDROSU );
            break;

        case MENU_CEK_CARI_ENT_FISILERI    :
            OPEN_CEK_SENET_BORDROSU_ARAMA ( CARI_MODULU , -1 , ENUM_TUM_BORDROLAR, -1, ENUM_CEK_SENET_ENT_FISI, parent );
            break;

        case MENU_CEK_KENDI_CEKLERIMIZ :
            OPEN_CEK_SENET_FORMU ( ENUM_CEK, -1,parent );
            break;

        case MENU_CEK_KENDI_SENETLERIMIZ :
            OPEN_CEK_SENET_FORMU ( ENUM_SENET,-1, parent );
            break;

        case MENU_CEK_MUS_CEKLERI_SENETLERI :
            ADAK_INFO("Müşteri çeklerini Cari modülünde tahsilat fişinden girebilirsiniz",NULL,NULL);
            break;

        case MENU_CEK_SENET_LISTESI :
            OPEN_CEK_LISTESI_BATCH ( parent );
            break;
        case MENU_CEK_MUS_CEK_ARAMA :
            OPEN_CEK_MUSTERI_CEK_SENET_ARAMA ( ENUM_CEK, parent );
            break;

        case MENU_CEK_MUS_SNT_ARAMA :
            OPEN_CEK_MUSTERI_CEK_SENET_ARAMA ( ENUM_SENET, parent );
            break;

        default :
            break;
    }
}
