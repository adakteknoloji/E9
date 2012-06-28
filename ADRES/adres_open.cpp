#include "sekme_menu_kernel.h"

#include "adak_sql.h"
#include "adak_gruplar.h"
#include "yonetim.h"
#include "e9_open.h"
#include "adres_enum.h"
#include "adres_open.h"

extern ADAK_SQL * DB;


/**************************************************************************************
                   ADRES_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * ADRES_GET_MENU_STRUCT()
{

    static struct MENU_STRUCT ADRES_MENU_STRUCT[] = {
            { MENU_ADR_ADRES_KARTLARI_HEADER,  QObject::tr ( "Adres Kartları" )           , ""     ,  "", "",true,1 , 0 },
            { MENU_ADR_SAHIS_KARTI,            QObject::tr ( "     Şahıs Kartı" )         , "ADR1" ,  "", "",false,1 , 0 },
            { MENU_ADR_FIRMA_KARTI,            QObject::tr ( "     Firma Kartı" )         , "ADR2" ,  "", "",false,1 , 0 },

            { MENU_ADR_ISLEMLER_HEADER,        QObject::tr ( "İşlemler" )                 , ""     ,  "", "",  true,1 , 0 },
            { MENU_ADR_GRUPLAR,                QObject::tr ( "     Gruplar" )             , "ADR10" ,  "", "",  false,1 , 0 },
            { MENU_ADR_ETIKETLERI,             QObject::tr ( "     Adres Etiketleri Yazdır" )       , "ADR11" ,  "",   ""  ,false,1 , 0 },
            { MENU_ADR_TOPLU_MAIL_GONDERME,    QObject::tr ( "     Toplu Mail Gönderimi" ), "ADR12" ,  "", "", false,1, 0 },
            { MENU_ADR_TOPLU_SMS_GONDERME,     QObject::tr ( "     Toplu SMS Gönderimi" ) , "ADR13" ,  "", "", false,1, 0 },
            { MENU_ADR_IMPORT,                 QObject::tr ( "     İçeriye Kayıt Al" )  , "ADR14" ,           "","",  false,1  , 0 },
            { MENU_ADR_EXPORT,                 QObject::tr ( "     Dışarıya Kayıt İhraç et" )            , "ADR15",    "","",  false,1  , 0 },

            { MENU_ADR_RAPORLAR_HEADER,        QObject::tr ( "Raporlar" )                 , ""     ,  "" , "" ,   true,1  , 0 },
            { MENU_ADR_KAYITLARI,              QObject::tr ( "     Adres Kayıtları" )     , "ADR20" ,  "" , "" ,   false,1  , 0 },
            { MENU_ADR_TEL_WEB_KAYITLARI,      QObject::tr ( "     Telefon / Web / E-posta Kayıtları" ), "ADR21",   "" , "" ,   false,1  , 0 },
            { MENU_ADR_FIHRIST,                QObject::tr ( "     Fihrist Raporu" )      ,   "ADR22",      "" , "" ,   false,1  , 0 },
#ifdef ADRES_BAGIMSIZ
            { MENU_YONETIM,                    QObject::tr ( "Yönetim")                   ,""         ,"","",false,1 , 0 },
            { MENU_SISTEM,                     QObject::tr ( "Sistem")                    ,""         ,"","",false,1 , 0 },
            { MENU_SEPERATOR,                  QObject::tr ( "")               ,""  , ""                                   , "",false,1, 0 },
            { MENU_SEPERATOR,                  QObject::tr ( "")               ,""  , ""                                   , "",false,1, 0 },
            { MENU_HATA_ISTEK,                 QObject::tr ( "Hata/İstek Bildirimi")      ,""         ,"","",false,1 , 0 },
#endif
            { MENU_END,                        QObject::tr ( "" ),    "",    "","",  false,1, 0  }

    };

    return ADRES_MENU_STRUCT;
}

/**************************************************************************************
                   ADRES_ITEM_CLICKED
***************************************************************************************/

void ADRES_ITEM_CLICKED(int enum_id,QWidget * parent)
{
    switch ( enum_id ) {

        case MENU_ADR_FIRMA_KARTI :
            OPEN_ADRES_KAYIT_FORMU ( -1 ,FIRMA_KAYDI, parent );
            break;
        case MENU_ADR_SAHIS_KARTI :
            OPEN_ADRES_KAYIT_FORMU ( -1 ,SAHIS_KAYDI, parent );
            break;
        case MENU_ADR_TOPLU_MAIL_GONDERME :
            OPEN_ADRES_RAPOR_FILTRESI_BATCH( TOPLU_MAIL_GONDERIMI, parent );
            break;

        case MENU_ADR_TOPLU_SMS_GONDERME :
            OPEN_ADRES_RAPOR_FILTRESI_BATCH(TOPLU_SMS_GONDERIMI,parent ) ;
            break;

        case MENU_ADR_TEL_WEB_KAYITLARI:
            OPEN_ADRES_RAPOR_FILTRESI_BATCH ( ADRES_ILETISIM_RAPORU, parent );
            break;

        case MENU_ADR_KAYITLARI :
            OPEN_ADRES_RAPOR_FILTRESI_BATCH ( ADRES_BILGILERI_RAPORU, parent );
            break;
        case MENU_ADR_FIHRIST :
            OPEN_ADRES_FIHRIST_RAPORU_BATCH ( parent );
            break;

        case MENU_ADR_ETIKETLERI :
            OPEN_ADRES_RAPOR_FILTRESI_BATCH ( ADRES_ETIKETLERI_RAPORU, parent );
            break;

        case MENU_ADR_GRUPLAR :
        OPEN_GRUPLAR( E9_PROGRAMI,ADRES_MODULU,QObject::tr( "ADRES GRUPLAR" ), "adres-toplu-mail-sms_gruplar", DB, parent);
            break;

        case MENU_ADR_EXPORT:
            OPEN_ADRES_ARAMA(SAHIS_ARTI_FIRMA,ADRES_EXPORT_ISLEMI ,parent);
            break;
        case MENU_ADR_IMPORT:
            OPEN_ADRES_IMPORT( parent );
            break;

        default :
            break;
    }
}
