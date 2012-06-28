#include "sekme_menu_kernel.h"

#include "adak_sql.h"
#include "adak_gruplar.h"
#include "yonetim.h"

#include "cari_enum.h"
#include "cari_open.h"
#include "e9_gui_utils.h"
#include "cari_fis_arama_open.h"
#include "sube_unite_detaylari_formu_open.h"
#include "sube_console_utils.h"
#include "cari_kart_import_open.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   CARI_GET_MENU_STRUCT
***************************************************************************************/
MENU_STRUCT * CARI_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT CARI_MENU_STRUCT[] = {
            { MENU_CARI_FISLER_HEADER               , QObject::tr("Fişler")                             ,""        ,""   , "",true ,1 , 0 },
            { MENU_CARI_TAHSILAT_FISI               , QObject::tr("     Tahsilat Fişi")                 ,"CAR1"    ,""   , "",false,1 , 0 },
            { MENU_CARI_ODEME_FISI                  , QObject::tr("     Ödeme Fişi")                    ,"CAR2"    ,""   , "",false,1 , 0 },
            { MENU_CARI_HAREKET_FISI                , QObject::tr("     Cari Hareket Fişi")             ,"CAR3"    ,""   , "",false,1 , 0 },
            { MENU_CARI_VIRMAN_FISI                 , QObject::tr("     Virman Fişi")                   ,"CAR4"    ,""   , "",false,1 , 0 },
            { MENU_CARI_ACILIS_FISI                 , QObject::tr("     Açılış Fişi")                   ,"CAR5"    ,""   , "",false,1 , 0 },

            { MENU_CARI_RAPORLAR_HEADER             , QObject::tr("Raporlar")                           ,""        ,""   , "",true ,1 , 0 },
            { MENU_CARI_HESAP_EKSTRESI              , QObject::tr("     Cari Hesap Ekstresi")           ,"CAR30"   ,""   , "",false,1 , 0 },
            { MENU_CARI_BAKIYE_RAPORU               , QObject::tr("     Cari Bakiye Raporu")            ,"CAR31"   ,""   , "",false,1 , 0 },

            { MENU_CARI_KARTLAR_HEADER              , QObject::tr("Cari Kartlar / Gruplar")             ,""        ,""   , "",true ,1 , 0 },
            { MENU_CARI_FIRMA_KARTI                 , QObject::tr("     Firma Cari Kartı")              ,"CAR20"   ,""   , "",false,1 , 0 },
            { MENU_CARI_SAHIS_KARTI                 , QObject::tr("     Şahıs Cari Kartı")              ,"CAR21"   ,""   , "",false,1 , 0 },
            { MENU_CARI_GRUPLAR                     , QObject::tr("     Cari Gruplar")                  ,"CAR22"   ,""   , "",false,1 , 0 },

            { MENU_CARI_ENT_FISLERI_HEADER          , QObject::tr("Entegrasyon")                        ,""        ,""   , "",true ,1 , 0 },
            { MENU_CARI_ENT_FISLERI                 , QObject::tr("     Cari Entegrasyon Fişleri")      ,"CAR10"   ,""   , "",false,1 , 0 },

            { MENU_CARI_ISLEMLER_HEADER             , QObject::tr("İşlemler")                           ,""        ,""   , "",true ,1 , 0 },
            { MENU_CARI_YAZICIDAKI_BLG_NOLARI       , QObject::tr("     Yazıcıdaki Belge Numaraları")   ,"CAR41"   ,""   , "",false,1 , 0 },
            { MENU_CARI_KART_IMPORT                 , QObject::tr("     İçeriye Kayıt Alın")            ,""        ,""   , "",false,1 , 0 },
            { MENU_CARI_KART_EXPORT                 , QObject::tr("     Dışarıya Kayıt İhraç Edin")       ,""        ,""   , "",false,1 , 0 },

            { MENU_END                              , "" , ""  ,"", "",false,1 ,0}
    };

    return CARI_MENU_STRUCT;
}

/**************************************************************************************
                   CARI_ITEM_CLICKED
***************************************************************************************/

void CARI_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    if ( E9_SUBEYE_UNITE_KAYDI_YAPILDI_MI() EQ false ) {
        return;
    }
    switch ( enum_id ) {

        case MENU_CARI_TAHSILAT_FISI :
            OPEN_CARI_TAHSILAT_FISI ( -1, parent );
            break;

        case MENU_CARI_ODEME_FISI :
            OPEN_CARI_ODEME_FISI (-1, CARI_MODULU, parent );
            break;

        case MENU_CARI_HAREKET_FISI                 :
            OPEN_CARI_HAREKET_FISI (-1 , ENUM_CARI_HAREKET_FISI , CARI_MODULU, parent);
            ADAK_INFO( "Uzmanlık gerektiren alan dikkat ediniz !",NULL,parent);
            break;

        case MENU_CARI_VIRMAN_FISI                       :
            OPEN_CARI_VIRMAN_FORMU ( -1, parent );
            break;

        case MENU_CARI_ACILIS_FISI                       :
            OPEN_CARI_HAREKET_FISI ( -1,ENUM_CARI_ACILIS_FISI, CARI_MODULU, parent );
            break;

        case MENU_CARI_GRUPLAR               :
            OPEN_GRUPLAR( E9_PROGRAMI,CARI_MODULU, QObject::tr( "CARİ - CARİ GRUPLAR" ), QObject::tr("cari-hesap-islemleri_cari-gruplar"), DB, parent );
            break;

        case MENU_CARI_FIRMA_KARTI              :
            OPEN_CARI_KART_FORMU ( -1,FIRMA_KAYDI,parent);
            break;

        case MENU_CARI_SAHIS_KARTI :
            OPEN_CARI_KART_FORMU ( -1,SAHIS_KAYDI,parent);
            break;

        case MENU_CARI_ENT_FISLERI   :
            OPEN_CARI_FIS_ARAMA ( ENUM_CARI_HAREKET_FISI ,-1 , ENUM_CARI_ENT_FISI , parent );
            break;

        case MENU_CARI_HESAP_EKSTRESI               :
            OPEN_CARI_HESAP_EKSTRESI_BATCH ( E9_PROGRAMI, CARI_MODULU, -1, parent );
            break;

        case MENU_CARI_BAKIYE_RAPORU                :
            OPEN_CARI_BAKIYE_RAPORU_BATCH ( E9_PROGRAMI, CARI_MODULU, parent );
            break;
        case MENU_CARI_YAZICIDAKI_BLG_NOLARI:
            OPEN_SUBE_UNITE_DETAYLARI_FORMU(SUBE_GET_UNITENIN_DEPOSU(), CARI_MODULU, parent);
            break;

        case MENU_CARI_KART_EXPORT :
            OPEN_CARI_KART_ARAMA( "" , SAHIS_ARTI_FIRMA , parent , 1 , E9_PROGRAMI, CARI_MODULU , QObject::tr( "Dışarıya Kayıt İhraç Etme" ) ,
                                  "" , CARI_KART_EXPORT_ISLEMI );
            break ;

        case MENU_CARI_KART_IMPORT :
            OPEN_CARI_KART_IMPORT( parent ) ;
            break ;

        default:
            break;
      }
}
