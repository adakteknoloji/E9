#include <QtGui/QApplication>
#include <QIcon>
#include "xyz.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "yonetim.h"
#include "xyz_db.h"

extern ADAK_SQL *       G_YONETIM_DB;
extern ADAK_SQL *       DB;
extern ADAK_SQL_STRUCT  xyz_db;

static void UPGRADE_TO_VERSION (double version);

/**************************************************************************************
                   main
***************************************************************************************/

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/xyz_icons/xyz_logo.jpg")), QIcon::Normal, QIcon::Off);
    a.setWindowIcon(icon);

    if ( PROGRAMA_LOGIN( argc, argv, XYZ_PROGRAMI , XYZ_PROGRAMI, &xyz_db, &F_KULLANICI_TAM_YETKILENDIRME_FUNC , NULL ) EQ ADAK_OK ) {
        CHECK_VERSION_UPGRADES ("program_version","xyz_sabit_degerler",&UPGRADE_TO_VERSION);
        OPEN_XYZ_ANA_MENU();
        a.exec();
    }

    if ( DB NE NULL ) {
        DB->~ADAK_SQL();
    }

    if ( G_YONETIM_DB NE NULL ) {
        G_YONETIM_DB->~ADAK_SQL();
    }

    // Programin main ' e donerek DATABASE kapatmasi en ideal cikis yontemi

    return 0;
}

/**************************************************************************************
                   UPGRADE_TO_VERSION
***************************************************************************************/

static void UPGRADE_TO_VERSION (double version)
{
    if (version EQ QVariant(ADAK_PROGRAM_VERSION()).toDouble()) {
        // ADD_DEFAULT_BELGELER();
    }
}
