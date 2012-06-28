#include <QtCore/QCoreApplication>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <stdlib.h>
#include <QStringList>
#include <iostream>
#include "adak_sql_struct.h"
#include "adak_utils.h"
#include "adak_sql.h"
#include "e9_db.h"
#include "e9_onar_utils.h"


extern ADAK_SQL * DB;

using namespace std;


/****************************************************/
/*                    main                          */
/****************************************************/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    a.addLibraryPath( a.applicationDirPath() );
    a.addLibraryPath( a.applicationDirPath() + "/plugins" );

    QString onarilacak_firma_kodu = argv[1];
    QString sunucu                = argv[2];
    QString database_username     = argv[3];
    QString sql_port_number       = argv[4];
    QString sql_driver            = argv[5];
    QString database_user_pass    = "";

    if (argc < 6 ){
        cout << endl << endl << endl ;
        cout << "********************************     EKSİK PARAMETRE GİRİLDİ!!!     ********************************" << endl;
        cout << endl << endl ;
        cout << "Sırasıyla : Onarılacak firmanın kodu | Sunucu | Veritabanı kullanıcısı | Sql Port Numarası | Sql Motoru";
        cout << endl << "şeklindeki parametreleri giriniz." << endl;
        return 0;
    }


    string password;
    cout << database_username.toStdString() <<" için veritabanı kullanıcı parolası : " << endl;
    cin.unsetf(ios::skipws);
    cin >> password;
    if( cin.good()) {
        database_user_pass = QString( password.c_str());
    } else {
        database_user_pass = "";
    }

    SET_SQL_HOSTNAME  ( sunucu             );
    SET_SQL_USERNAME  (database_username);
    SET_SQL_PASSWORD  (database_user_pass);
    SET_SQL_MOTORU    ( sql_driver         );
    SET_SQL_PORT      ( QVariant(sql_port_number).toInt() );

    QList< ADAK_SQL_STRUCT * > db_struct_list;

    db_struct_list << &e9_db;

    DB = CREATE_ADAK_SQL_CONNECTION( db_struct_list , onarilacak_firma_kodu, GET_SQL_DB_DRIVER_ENUM(SQL_MOTORU()));

    cout << "**************************************************************************" << "\n\n";
    cout << "* İşlemin gerçekleştirildiği sunucu : " << sunucu.toStdString() << endl;
    cout << "* Veritabanı kullanıcısı            : " << database_username.toStdString() << endl;
    cout << "* Onarılacak firma kodu             : " << onarilacak_firma_kodu.toStdString() << endl;
    cout << "**************************************************************************" << endl;



    E9_ONAR ( DB, NULL );
    delete DB;
    return 0;

}
