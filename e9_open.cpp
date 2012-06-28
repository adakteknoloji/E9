#include "yonetim.h"
#include "e9_kullanici_yetkileri_formu_open.h"
#include "sekme_menu_kernel.h"
#include "belge_kernel_open.h"
#include "e9_open.h"
#include "e9_belge.h"
#include "kernel_utils.h"
#include "adres_email_log_batch_open.h"
#include "e9_profil_secme_batch.h"

/**************************************************************************************
                   E9_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * E9_GET_MENU_STRUCT()
{
    static struct MENU_STRUCT E9_MENU_STRUCT[] = {
        { MENU_CARI                 , QObject::tr ( "Cari Hesap İşlemleri" )        , "" , "" , "" , false , 1 , 0 },
        { MENU_CEK                  , QObject::tr ( "Çek/Senet İşlemleri" )         , "" , "" , "" , false , 1 , 0 },
        { MENU_BANKA                , QObject::tr ( "Banka İşlemleri" )             , "" , "" , "" , false , 1 , 0 },
        { MENU_DOVIZ                , QObject::tr ( "Döviz İşlemleri" )             , "" , "" , "" , false , 1 , 0 },
        { MENU_SEPERATOR            , QObject::tr ( "" )                            , "" , "" , "" , false , 1 , 0 },

        { MENU_FATURA               , QObject::tr ( "Fatura İşlemleri" )            , "" , "" , "" , false , 1 , 0 },
        { MENU_IRSALIYE             , QObject::tr ( "İrsaliye İşlemleri" )          , "" , "" , "" , false , 1 , 0 },
        { MENU_STOK                 , QObject::tr ( "Stok İşlemleri" )              , "" , "" , "" , false , 1 , 0 },
        { MENU_SUBE                 , QObject::tr ( "Şube/Depo/Kasa/Ünite" )        , "" , "" , "" , false , 1 , 0 },
        { MENU_SEPERATOR            , QObject::tr ( "" )                            , "" , "" , "" , false , 1 , 0 },

        { MENU_MUHASEBE             , QObject::tr ( "Genel Muhasebe" )              , "" , "" , "" , false , 1 , 0 },
        { MENU_ISLETME              , QObject::tr ( "İşletme Defteri" )             , "" , "" , "" , false , 1 , 0 },
        { MENU_PERAKENDE            , QObject::tr ( "Kasa/Perakende" )              , "" , "" , "" , false , 1 , 0 },
        { MENU_PERSONEL             , QObject::tr ( "Personel Bordrosu" )           , "" , "" , "" , false , 1 , 0 },
        { MENU_DEMIRBAS             , QObject::tr ( "Sabit Kıymetler/Demirbaşlar" ) , "" , "" , "" , false , 1 , 0 },
        { MENU_SEPERATOR            , QObject::tr ( "" )                            , "" , "" , "" , false , 1 , 0 },

        { MENU_ADRES                , QObject::tr ( "Adres/Toplu Mail/Toplu SMS" )  , "" , "" , "" , false , 1 , 0 },
        { MENU_FAIZ                 , QObject::tr ( "Faiz/Taksit Hesaplama" )       , "" , "" , "" , false , 1 , 0 },
        { MENU_SMM_MAKBUZ           , QObject::tr ( "Serbest Meslek Makbuzu" )      , "" , "" , "" , false , 1 , 0 },
        { MENU_SEPERATOR            , QObject::tr ( "" )                            , "" , "" , "" , false , 1 , 0 },

        { MENU_YONETIM              , QObject::tr ( "Yönetim İşlemleri" )               , "" , "" , "" , false , 1 , 0 },
        { MENU_KULLANIM_KITAPCIGI   , QObject::tr ( "Kullanım Kitapçığı" )              , "" , "" , "" , false , 1 , 0 },
        { MENU_SIK_SORUNLAN_SORUNLAR, QObject::tr ( "YARDIM ( Sık Sorulan Sorular )" )  , "" , "" , "" , false , 1 , 0 },
        { MENU_HATA_ISTEK           , QObject::tr ( "Hata / İstek Bildirimi" )          , "" , "" , "" , false , 1 , 0 },
        { MENU_SEPERATOR            , QObject::tr ( "" )                            , "" , "" , "" , false , 1 , 0 },

        { MENU_CRM                  , QObject::tr ( "Crm" )                             , "" , "" , "" , false , 1 , 0 },
        { MENU_END                  , ""                                                , "" , "" , "" , false , 1 , 0 }
    };

    return E9_MENU_STRUCT;
}

/**************************************************************************************
                   E9_ITEM_CLICKED
***************************************************************************************/

void E9_ITEM_CLICKED(int )
{
    return;
}

/**************************************************************************************
                   YONETIM_GET_MENU_STRUCT
***************************************************************************************/

MENU_STRUCT * YONETIM_GET_MENU_STRUCT()
{
    //
    // Not eger herhangi menu eklenir ise menu sayilari duzeltilmeli
    //

    int yonetim_menu_sayisi        =  8;
    //  sistem_menu_sayisi         = 18;
    //                           + _____
    //                               26

    static struct MENU_STRUCT YONETIM_MENU_STRUCT[ 26 ] = {
            { MENU_YNT_E9_SABIT_DEGERLER      , QObject::tr ("Parametreler")                   , "SIS4" , "" , "" , false , 1 , 0 },
            { MENU_YNT_E9_BELGE_TASARIMCISI   , QObject::tr ("Belge Tasarımcısı")              , "SIS5" , "" , "" , false , 1 , 0 },
            { MENU_YNT_E9_LOG_BILGILERI       , QObject::tr ("E9 Log Bilgisi Arama")           , "SIS6" , "" , "" , false , 1 , 0 },
            { MENU_YNT_E9_EMAIL_LOGLARI       , QObject::tr ("E-Mail Log Bilgileri" )          , "SIS7" , "" , "" , false , 1 , 0 },
            { MENU_YNT_E9_DEVIR_ISLEMI        , QObject::tr ("Devir İşlemi")                   , "SIS8" , "" , "" , false , 1 , 0 },
            { MENU_YNT_E9_ONAR                , QObject::tr ("E9 Onar")                        , "SIS9" , "" , "" , false , 1 , 0 },
            { MENU_YNT_E9_PROFIL_SECME        , QObject::tr ("E9 Profil Seçme")                , "SIS10", "" , "" , false , 1 , 0 },
            { MENU_SEPERATOR                  , QObject::tr ("")                               , ""     , "" , "" , false , 1 , 0 }

            // MENU_END sistem menusunden geliyor
    };

    struct MENU_STRUCT * SISTEM_MENU_STRUCT = SISTEM_GET_MENU_STRUCT();

    for (int i=0; ; i++) {
        YONETIM_MENU_STRUCT[ yonetim_menu_sayisi + i ].enum_id              = SISTEM_MENU_STRUCT[ i ].enum_id;
        YONETIM_MENU_STRUCT[ yonetim_menu_sayisi + i ].text_to_display      = SISTEM_MENU_STRUCT[ i ].text_to_display;
        YONETIM_MENU_STRUCT[ yonetim_menu_sayisi + i ].menu_code            = SISTEM_MENU_STRUCT[ i ].menu_code;
        YONETIM_MENU_STRUCT[ yonetim_menu_sayisi + i ].icon_path            = SISTEM_MENU_STRUCT[ i ].icon_path;
        YONETIM_MENU_STRUCT[ yonetim_menu_sayisi + i ].shortcut             = SISTEM_MENU_STRUCT[ i ].shortcut;
        YONETIM_MENU_STRUCT[ yonetim_menu_sayisi + i ].is_header            = SISTEM_MENU_STRUCT[ i ].is_header;
        YONETIM_MENU_STRUCT[ yonetim_menu_sayisi + i ].menu_column          = SISTEM_MENU_STRUCT[ i ].menu_column;
        YONETIM_MENU_STRUCT[ yonetim_menu_sayisi + i ].font_size_increment  = SISTEM_MENU_STRUCT[ i ].font_size_increment;
        if ( SISTEM_MENU_STRUCT[ i ].enum_id EQ MENU_END ) {
            break;
        }
    }
    return YONETIM_MENU_STRUCT;
}

/**************************************************************************************
                   YONETIM_ITEM_CLICKED
***************************************************************************************/

void YONETIM_ITEM_CLICKED (int enum_id, QWidget * parent)
{
    switch ( enum_id ) {
        case MENU_YNT_E9_DEVIR_ISLEMI :
            OPEN_E9_DEVIR_ISLEMI_BATCH ( parent );
            break;

        case MENU_YNT_E9_ONAR :
            OPEN_E9_ONAR_BATCH ( parent );
            break;

        case MENU_YNT_E9_BELGE_TASARIMCISI :
            //ADAK_INFO( "Belge Tasarımcısı bu versiyonda kapalıdır. \nBir sonraki versiyonda yeni hali ile devreye girecektir...", NULL, NULL );
            OPEN_TASARIM_MENU_BATCH (  e9_belge_struct, ADAK_BELGE_COUNT, parent );
            break;

        case MENU_YNT_E9_SABIT_DEGERLER :
            OPEN_E9_SABIT_DEGERLER_FORMU ( parent );
            break;

        case MENU_YNT_E9_LOG_BILGILERI  :
            OPEN_E9_LOG_KAYDI_ARAMA ( parent );
            break;

        case MENU_YNT_E9_EMAIL_LOGLARI :
            OPEN_ADRES_EMAIL_LOG_BATCH ( parent );
            break;

        case MENU_YNT_E9_PROFIL_SECME : 
	    {
                bool degisiklik_yapildimi = false;
                OPEN_E9_PROFIL_SECME( ENUM_E9_BASLANGIC, parent, &degisiklik_yapildimi );
            }
            break;

        default:
            SISTEM_ITEM_CLICKED  ( enum_id , parent, &OPEN_E9_KULLANICI_YETKILERI_FORMU );
            break;
    }
}
