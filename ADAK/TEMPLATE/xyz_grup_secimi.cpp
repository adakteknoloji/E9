#include "xyz_grup_secimi_class.h"
#include "xyz_grup_secimi_open.h"
#include "adak_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_XYZ_GRUP_SECIMI
***************************************************************************************/

int OPEN_XYZ_GRUP_SECIMI (  int p_cagiran_modul, QWidget * p_parent )
{
    XYZ_GRUP_SECIMI * F = new XYZ_GRUP_SECIMI ( p_cagiran_modul, p_parent );
    F->exec();
    return GET_GRUP_KERNEL_SECILEN_ID ();
}

/**************************************************************************************
                   XYZ_GRUP_SECIMI::XYZ_GRUP_SECIMI
***************************************************************************************/

XYZ_GRUP_SECIMI::XYZ_GRUP_SECIMI ( int p_cagiran_modul, QWidget * p_parent ) : GRUP_SECIM_KERNEL ( p_parent )
{
    GRUP_KERNEL::SET_GRUPLAR_TABLE_NAME ( "xyz_gruplari" );
    GRUP_KERNEL::SET_GRUPLAR_UYE_SAYISI_COLUMN_NAME ( "grupta_kayitli_uye_sayisi" );

    SET_HELP_PAGE           ( tr("xyz_grubu.html") );
    SET_PAGE_TITLE          ( tr ( "XYZ GRUPLARI" ) );
    SET_SETTING_NAME        ( "XYZ_GRUBU" );

    if ( p_cagiran_modul EQ XYZ_MODULU ) {
        GRP_SET_MODUL_ID ( XYZ_MODULU );
    }
    else {
        GRP_SET_MODUL_ID        ( ABC_MODULU );
        GRP_SET_VISIBLE_MODULES ( ABC_MODULU );
    }

    GRP_SET_VISIBLE_MODULES ( XYZ_MODULU );

    INIT_GRUP_SECIM_KERNEL  ( this, DB );
}

/**************************************************************************************
                   XYZ_GRUP_SECIMI::~XYZ_GRUP_SECIMI
***************************************************************************************/

XYZ_GRUP_SECIMI::~XYZ_GRUP_SECIMI()
{
}
