#include <QPushButton>
#include <QLabel>
#include "ui_xyz_ana_menu.h"
#include "xyz_ana_menu_class.h"
#include "xyz_ana_menu_open.h"
#include "adak_sql.h"


extern ADAK_SQL * DB;

enum {
    MENU_COLUMN_1                 = 10,
        MENU_XYZ_FISI             = 11,

    MENU_COLUMN_2                 = 20,
        MENU_XYZ_RAPORU           = 21,
};

/**************************************************************************************
                   OPEN_XYZ_ANA_MENU
****************************************************************************************/

void OPEN_XYZ_ANA_MENU ( QWidget * p_parent )
{
    XYZ_ANA_MENU * F = new XYZ_ANA_MENU ( p_parent );
    F->setWindowTitle ( QObject::tr ("XYZ" ) );
    F->show();
    F->setWindowState ( Qt::WindowMaximized );
}

/**************************************************************************************
                   XYZ_ANA_MENU::XYZ_ANA_MENU
***************************************************************************************/

XYZ_ANA_MENU::XYZ_ANA_MENU ( QWidget * p_parent ) : MENU_KERNEL ( p_parent ), ui ( new Ui::XYZ_ANA_MENU )
{
    ui->setupUi ( this );
    START_KERNEL();
}

/***************************************************************************************
                   XYZ_ANA_MENU::~XYZ_ANA_MENU
****************************************************************************************/

XYZ_ANA_MENU::~XYZ_ANA_MENU()
{

}

/***************************************************************************************
                   XYZ_ANA_MENU::SETUP_FORM
****************************************************************************************/

XYZ_ANA_MENU::SETUP_FORM()
{
    struct MENU_STRUCT MENULER[] = {

       { MENU_COLUMN_1              , ""           , 0             , "" , "" , 0 ,"" , 0 , MENU_SAYFADA },
       { MENU_XYZ_FISI              , "XYZ Fişi"   , MENU_COLUMN_1 , "" , "" , 0 ,"" , 0 , MENU_SAYFADA },
       { MENU_COLUMN_2              , ""           , 0             , "" , "" , 0 ,"" , 0 , MENU_SAYFADA },
       { MENU_XYZ_RAPORU            , "XYZ Raporu" , MENU_COLUMN_2 , "" , "" , 0 ,"" , 0 , MENU_SAYFADA },

       { 0                          , ""           ,  0            , "" , "" , 0 , "", 0, MENU_SAYFADA }

    };


    SET_KERNEL_STRUCT         ( MENULER );

    REGISTER_MENU_WIDGET      ( ui->main_xyz_widget );

    HIDE_BUTTON_COLUMN_HEADER ();

    SET_ICON_SIZE             ( 120, 120 );

    SET_BUTTON_TEXT_POS       ( BUTTON_DISINDA_ALT_TARAFTA );

    SHOW_MENU_ITEM            ( MENU_XYZ_RAPORU );

    KULLANICI_YETKILERINI_AYARLA();     
}

/**************************************************************************************
                   XYZ_ANA_MENU::KULLANICI_YETKILERINI_AYARLA
****************************************************************************************/

void XYZ_ANA_MENU::KULLANICI_YETKILERINI_AYARLA ()
{

    E9_KULLANICI_YETKILERI_STRUCT * XYZ_KULLANICI_YETKILERI = (E9_KULLANICI_YETKILERI_STRUCT *) GET_KULLANICI_YETKILERI();

    if ( XYZ_KULLANICI_YETKILERI->xyz_fisi_girebilir_mi EQ 1 ) {

        SHOW_MENU_ITEM ( MENU_XYZ_FISI        );
    }
}

/**************************************************************************************
                   XYZ_ANA_MENU::MENU_ITEM_CLICKED
****************************************************************************************/

void XYZ_ANA_MENU::MENU_ITEM_CLICKED ( int p_enum_id )
{
    switch ( p_enum_id ) {

        case MENU_XYZ_FISI :
             OPEN_XYZ_FISI(this);
             break;

        default :
            break;
    }
}


