#ifndef XYZ_PLUGIN_H
#define XYZ_PLUGIN_H

#include <QObject>
#include "adak_plugin_interface.h"

//Burda adak plugin interface i miras alarak, interface de tanimlamis oldugumuz fonksiyonlari burda tanimliyoruz.
//Amacimiz yonetim arayuzu ile plugin arasinda ki iletisimi saglicak standart fonksiyonlari olusturmak. Yonetim arayuzu pluginle iletisimini bu
//fonksiyonlar araciligi ile yapar. Bu yuzden ilgili fonksiyonlari tanimlamak lazim.

class XYZ_PLUGIN : public QObject, ADAK_PLUGIN_INTERFACE
{
    Q_OBJECT
    Q_INTERFACES(ADAK_PLUGIN_INTERFACE)

public:
    void               SHOW_GUI          ( ADAK_SQL * db , QWidget * parent );
    QIcon              GET_ICON          ();
    int                GET_PROGRAM_ID    ();
    QString            GET_MODULE_NAME   ();
    QString            GET_BUTTON_TEXT   ();
    ADAK_SQL_STRUCT * GET_DB_STRUCT     ();
};

#endif // XYZ_PLUGIN_H
