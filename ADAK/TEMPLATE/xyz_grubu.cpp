#include "xyz_grubu_open.h"
#include "xyz_grubu_class.h"

#include "adak_sql.h"
#include "adak_utils.h"
#include "adak_defines.h"

extern ADAK_SQL *           DB;

/* Grup Kernel kullanılacak formlarda database isimleri aşagıdaki şekilde olmalıdır

    gruplar_tablo_ismi                     = "tablo ismini setlenmesi gereklidir.Ornek;SET_GRUPLAR_TABLE_NAME("tablo_ismi");

    grup_id_column_name                    = "grup_id";
    grup_parent_id_column_name             = "grup_parent_id";
    grup_seviyesi_column_name              = "grup_seviyesi";
    grup_kodu_column_name                  = "grup_kodu";
    grup_adi_column_name                   = "grup_adi";
    tam_grup_kodu_column_name              = "tam_grup_kodu";
    grup_son_seviyemi_column_name          = "grup_son_seviye_mi";
    grupta_kayitli_uye_sayisi_column_name  = "gruptaki_kayit_sayisi";
    alt_grup_sayisi_column_name            = "alt_grup_sayisi";
    program_id_column_name                 = "program_id";
    modul_id_column_name                   = "modul_id";


    --Diger Farkli column isimleri varsa da onlarda ayri olarak setlenmelidir.Mesela
      UYE_SAYISI_COLUMN yukardakinden farkli ise asagidaki gibi setlenebilir

    SET_GRUPLAR_UYE_SAYISI_COLUMN_NAME("gruptaki_kayit_sayisi");

*/

/**************************************************************************************
                   OPEN_XYZ_GRUPLARI
***************************************************************************************/

void OPEN_XYZ_GRUPLARI (QString delimeter, QWidget * parent )
{
    XYZ_GRUBU * F = new XYZ_GRUBU ( delimeter , parent );
    F->exec();
}

/**************************************************************************************
                   XYZ_GRUBU::XYZ_GRUBU
***************************************************************************************/

XYZ_GRUBU::XYZ_GRUBU ( QString delimeter, QWidget * parent ) : GRUP_KERNEL(parent)
{
    // SQL tablo ismini setleriz
    SET_GRUPLAR_TABLE_NAME ("xyz_gruplari");

    SET_HELP_PAGE           ( tr("xyz_grubu.html") );
    SET_PAGE_TITLE          ( tr ( "XYZ GRUPLARI" ) );
    SET_SETTING_NAME        ( "XYZ_GRUBU" );

    // Delimeterimizi setleriz
    SET_DELIMETER ( delimeter );

    // Hangi modulden calistiriliyorsa o modul id setlenir.Bu idler adak_defines.h da bulunur.
    GRP_SET_MODUL_ID ( MUHASEBE_MODULU );

    // Ekranda hangi modulerin kayitlarini gormek istiyorsak onlarin idlerini de setleriz.Hic setlenmesse tum kayitlar gosterilir.
    GRP_SET_VISIBLE_MODULES ( MUHASEBE_MODULU );
    GRP_SET_VISIBLE_MODULES ( CARI_MODULU );
    GRP_SET_VISIBLE_MODULES ( ADRES_MODULU );

    // Ve initleriz
    INIT_GRUP_KERNEL ( parent, DB );
}

/**************************************************************************************
                   XYZ_GRUBU::~XYZ_GRUBU
***************************************************************************************/

XYZ_GRUBU::~XYZ_GRUBU()
{

}
