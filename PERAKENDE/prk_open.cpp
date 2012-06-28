#include "sekme_menu_kernel.h"
#include "adak_sql.h"
#include "adak_gruplar.h"
#include "yonetim.h"

#include "sube_enum.h"
#include "sube_secimi_open.h"
#include "muh_console_utils.h"
#include "muh_hesap_ekstresi_batch_open.h"
#include "sube_console_utils.h"

#include "prk_enum.h"
#include "prk_open.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   PERAKENDE_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * PERAKENDE_GET_MENU_STRUCT()
{
     static struct MENU_STRUCT PERAKENDE_MENU_STRUCT[] = {
         { MENU_PRK_FISLER_HEADER   , "Fişler"              , ""      , ""    , "" , true,1 , 0 },
         { MENU_PRK_ALIS_FISI       , "     Alış / Gider Fişi"  , "PRK1"  , ""    , "" , false,1 , 0 },
         { MENU_PRK_SATIS_FISI      , "     Satış / Gelir Fişi"  , "PRK2"  , ""          , "" , false,1 , 0 },

         { MENU_PRK_RAPORLAR_HEADER , "Raporlar"             , ""    , ""                    , "" , true,1 , 0 },
         { MENU_PRK_KASA_RAPORU     , "     Kasa Raporu"          , "PRK3", ""               , "" , false,1 , 0 },

         { MENU_PRK_TANIMLAR_HEADER , "Tanımlar"                 , ""     , ""               , "" , true,1 , 0 },
         { MENU_PRK_GIDER_TURLERI   , "     Alış / Gider Türleri " , "PRK4" , ""        , "" , false,1 , 0 },
         { MENU_PRK_GELIR_TURLERI   , "     Satış / Gelir Türleri ", "PRK5" , ""        , "" , false,1 , 0 },

         { MENU_END                 , "" ,""  , "" , "" , false,1,0 }
    };

    return PERAKENDE_MENU_STRUCT;
}

/**************************************************************************************
               PERAKENDE_ITEM_CLICKED
***************************************************************************************/

void PERAKENDE_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    SQL_QUERY query ( DB );

    switch ( enum_id ) {
        case MENU_PRK_ALIS_FISI:
        case MENU_PRK_SATIS_FISI  :
        case MENU_PRK_GIDER_TURLERI         :
        case MENU_PRK_GELIR_TURLERI         :
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
               if ( enum_id EQ MENU_PRK_ALIS_FISI ) {

                  query.PREPARE_SELECT( "prk_gelir_gider_turleri","hesap_id","gider_turu_mu = :gider_turu_mu" );
                  query.SET_VALUE     ( ":gider_turu_mu", 1 );

                  if ( query.SELECT() EQ 0 ) {
                      ADAK_WARNING( QObject::tr( "Tanımlı <b>Alış / Gider Turu</b> yok.  Alış / Gider Turu menüsünden tanimlama yapmanız gerekir." ), NULL, NULL );
                      return;
                  }
                  OPEN_PRK_PERAKENDE_FISI(sube_id ,1,parent);
               }
               else if ( enum_id EQ MENU_PRK_SATIS_FISI){
                  query.PREPARE_SELECT( "prk_gelir_gider_turleri","hesap_id","gider_turu_mu = :gider_turu_mu" );
                  query.SET_VALUE     ( ":gider_turu_mu", 0 );

                  if ( query.SELECT() EQ 0 ) {
                      ADAK_WARNING( QObject::tr( "Tanımlı <b>Satış / Gelir Turu</b> yok.  Satış / Gelir Turu menüsünden tanimlama yapmanız gerekir." ), NULL, NULL  );
                      return;
                  }
                  OPEN_PRK_PERAKENDE_FISI(sube_id ,0,parent);
               }

               else if ( enum_id EQ MENU_PRK_GIDER_TURLERI) {
                  OPEN_PRK_GELIR_GIDER_TURLERI_FISI(sube_id,1,parent);
               }
               else if ( enum_id EQ MENU_PRK_GELIR_TURLERI){
                  OPEN_PRK_GELIR_GIDER_TURLERI_FISI(sube_id,0,parent);
              }
          }

          break;

        case MENU_PRK_KASA_RAPORU:
          OPEN_MUH_HESAP_EKSTRESI_BATCH(MUH_GET_HESAP_KODU(SUBE_GET_UNITE_MUH_KASA_HESABI_ID()),ENUM_PRK_KASA_RAPORU , parent);
          break;

        default :
          break;
    }
}
