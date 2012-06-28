#include "sekme_menu_kernel.h"

#include "adak_sql.h"
#include "adak_gruplar.h"
#include "adak_utils.h"
#include "yonetim.h"
#include "print.h"

#include "prs_personel_raporu_open.h"
#include "cari_kart_arama_open.h"
#include "sube_console_utils.h"
#include "prs_karti_import_open.h"

#include "prs_enum.h"
#include "prs_open.h"
#include "prs_con_utils.h"

extern ADAK_SQL * DB;




/**************************************************************************************
                   PERSONEL_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * PERSONEL_GET_MENU_STRUCT()
{


    static struct MENU_STRUCT PERSONEL_MENU_STRUCT[] = {
            { MENU_PRS_ISLEMLER_HEADER              ,QObject::tr ("İşlemler")                 , ""      ,"","",true,1, 0 },
            { MENU_PRS_MAAS_BORDROSU                ,QObject::tr ("     Maaş Bordrosu")       , "PRS1"  ,"","",false,1, 0 },
            { MENU_PRS_EBILDIRGE                    ,QObject::tr ("     E-Bildirge")          , "PRS2"  ,"","",false,1, 0 },
            { MENU_PRS_EKSIK_GUN_NEDENLERI          ,QObject::tr ("     Eksik Gün Nedenleri") , "PRS3"  ,"","",false,1, 0 },
            { MENU_PRS_BORDRO_INCELEME              ,QObject::tr ("     Bordro İncele")       , "PRS4"  ,"","",false,1, 0 },
            { MENU_PRS_ISTEN_AYRILMA                ,QObject::tr ("     İşten Ayrılma İşlemi"), "PRS5"  ,"","",false,1, 0 },
            { MENU_PRS_MAAS_BORDROSU_IPTAL          ,QObject::tr ("     Maaş Bordrosu İptal") , "PRS6"  ,"","",false,1, 0 },
            { MENU_PRS_KARTI_IMPORT                 ,QObject::tr ("     Kayıtlarınızı İçeriye Alın ") , "PRS7"  ,"","",false,1, 0 },
            { MENU_PRS_KARTI_EXPORT                 ,QObject::tr ("     Kayıtlarınızı Dışarıya Aktarın") , "PRS9"  ,"","",false,1, 0 },

            { MENU_PRS_RAPORLAR_HEADER                 ,QObject::tr ("Raporlar")                            , ""  ,"","",true,1, 0 },
            { MENU_PRS_MAAS_BORDROSU_RAPORU            ,QObject::tr ("     Maaş Bordrosu Raporu")           , "PRS10"  ,"","",false,1, 0 },
            { MENU_PRS_UCRET_PUSULASI                  ,QObject::tr ("     Ücret Pusulası")                 , "PRS11"      ,"","",false,1, 0 },
            { MENU_PRS_IHBAR_VE_KIDEM_TAZMINATI_RAPORU ,QObject::tr ("     İhbar ve Kıdem Tazminatı Raporu"), "PRS12" ,"","",false,1, 0 },
            { MENU_PRS_PERSONEL_RAPORU                 ,QObject::tr ("     Personel Raporu")                , "PRS13"     ,"","",false,1, 0 },
            { MENU_PRS_ISTEN_AYRILAN_ARAMA             ,QObject::tr ("     İşten Ayrılanlar")               , "PRS14"  ,"","",false,1, 0 },


            { MENU_PRS_TANIMLAR_HEADER              ,QObject::tr ("Tanımlar")                    , "","","",true,1, 0 },
            { MENU_PRS_SICIL_KARTI_TANIMLAMA        ,QObject::tr ("     Personel Sicil Kartı")   , "PRS15"  ,"","",false,1, 0 },
            { MENU_PRS_EK_ODENEK_TANIMLAMA          ,QObject::tr ("     Ek Ödenek Tanımlama")    , "PRS16"      ,"","",false,1, 0 },
            { MENU_PRS_EK_KESINTI_TANIMLAMA         ,QObject::tr ("     Ek Kesinti Tanımlama")   , "PRS17"     ,"","",false,1, 0 },
            { MENU_PRS_GRUPLAR                      ,QObject::tr ("     Personel Grupları")      , "PRS18"     ,"","",false,1, 0 },
            { MENU_PRS_SABIT_DEGERLER               ,QObject::tr ("     Sabit Değerler")         , "PRS19"       ,"","",false,1, 0 },

            { MENU_END                              , "","","","",false,1,0}
    };

    return PERSONEL_MENU_STRUCT;
}

/**************************************************************************************
                   PERSONEL_ITEM_CLICKED
***************************************************************************************/

void PERSONEL_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    switch ( enum_id ) {
        case MENU_PRS_SICIL_KARTI_TANIMLAMA :
            SHOW_PRS_PERSONEL_FORMU (-1 , parent);
            break;

        case MENU_PRS_MAAS_BORDROSU :
            SHOW_PRS_MAAS_BORDROSU_BATCH ( parent );
            break;

        case MENU_PRS_MAAS_BORDROSU_IPTAL : {
            SQL_QUERY  sql_query  (DB);

            sql_query.PREPARE_SELECT ( "prs_bordrolar","bordro_id");

            if ( sql_query.SELECT() EQ 0 ) {

                ADAK_WARNING(  QObject::tr ("Henüz Hiç Bordro Hazırlanmamıştır !.."), NULL , NULL);
                return;
            }
            SHOW_PRS_MAAS_BORDROSU_IPTAL_BATCH ( parent );
            break;
        }
        case MENU_PRS_EK_ODENEK_TANIMLAMA :
            OPEN_PRS_EK_ODENEK_VE_KESINTI_FISI( ENUM_PRS_ODENEK, parent );
            break;
        case MENU_PRS_EK_KESINTI_TANIMLAMA :
            OPEN_PRS_EK_ODENEK_VE_KESINTI_FISI( ENUM_PRS_KESINTI, parent );
            break;
        case MENU_PRS_SABIT_DEGERLER :
            OPEN_PRS_SABIT_DEGERLER_FORMU ( parent );
            break;
        case MENU_PRS_MAAS_BORDROSU_RAPORU :
            SHOW_PRS_MAAS_BORDROSU_RAPORU_BATCH ( SUBE_GET_SUBE_ID(), parent);
            break;
        case MENU_PRS_IHBAR_VE_KIDEM_TAZMINATI_RAPORU :
            SHOW_PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH (parent);
            break;
        case MENU_PRS_ISTEN_AYRILMA :
            SHOW_PRS_ISTEN_AYRILMA_BATCH ( parent );
            break;
        case MENU_PRS_GRUPLAR :
            OPEN_GRUPLAR( E9_PROGRAMI, PERSONEL_MODULU, QObject::tr( "PRS - PERSONEL GRUPLAR" ), "personel-bordrosu_personel-gruplari", DB , parent );
            break;
        case MENU_PRS_EBILDIRGE :
            OPEN_PRS_EBILDIRGE_OLUSTUR_BATCH( parent );
            break;
        case MENU_PRS_EKSIK_GUN_NEDENLERI :
            OPEN_PRS_EKSIK_GUN_NEDENLERI_BATCH( parent );
            break;

        case MENU_PRS_BORDRO_INCELEME : {
            SQL_QUERY  sql_query  (DB);

            sql_query.PREPARE_SELECT ( "prs_bordrolar","bordro_id");

            if ( sql_query.SELECT() EQ 0 ) {

            ADAK_WARNING(  QObject::tr ("Henüz Hiç Bordro Hazırlanmamıştır !.."), NULL ,NULL);
            return;
            }

            SHOW_BORDRO_INCELEME_TREE_SECIMI ( parent );
            break;
        }
        case MENU_PRS_PERSONEL_RAPORU:
        SHOW_PRS_PERSONEL_RAPORU_BATCH ( parent);
            //OPEN_REPORT_SHOWER( SHOW_PERSONEL_RAPORU(), parent );
            break;
        case MENU_PRS_ISTEN_AYRILAN_ARAMA :
            SHOW_PRS_ISTEN_AYRILANLAR_ARAMA ( parent );
            break;
        case MENU_PRS_UCRET_PUSULASI :
            OPEN_PRS_UCRET_PUSULASI_BATCH ( parent );
            break;
        case MENU_PRS_KARTI_IMPORT :
            OPEN_PRS_KARTI_IMPORT( parent );
            break;
        case MENU_PRS_KARTI_EXPORT :
            OPEN_CARI_KART_ARAMA( "", SAHIS_KAYDI , parent, 0, E9_PROGRAMI, PERSONEL_MODULU, "Personel Kartı Dışarıya Aktarma", "", PRS_KARTI_EXPORT_ISLEMI );
            break;
        default :
            break ;
    }
}
