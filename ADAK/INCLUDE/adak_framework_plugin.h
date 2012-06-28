#ifndef ADAK_PLUGIN_INTERFACE_H
#define ADAK_PLUGIN_INTERFACE_H

#include "adak_sql.h"

/*
 *  Plugin interface: Pluginlerin,plugin yonetim arabirimi ile haberlesmesini saglayan fonksiyonlari tutar.
 *                    Boylelikle ana program ihtiyac duydugu bilgileri pluginden isteyebilir.Veya bu pluginde
 *                    calistirilmasi gereken kodlari calistirabilir.Bu header dosyasini tum plugin ler de include edip gerekli
 *                    fonksiyonlari tanimlamak gerekir.
 */

class QString;

class ADAK_PLUGIN_INTERFACE
{
public:
    virtual ~ADAK_PLUGIN_INTERFACE() {}

public slots:
    //SHOW_GUI de pluginlere yaratilan database baglantisi,parametre olarak gonderilir.
    //Boylece pluginlerde bu baglanti kullanilarak database erisimi saglanabilir.
    virtual void SHOW_GUI(ADAK_SQL * db,QWidget * parent) = 0;
    virtual QIcon   GET_ICON() = 0;
    virtual int     GET_PROGRAM_ID() = 0;
    virtual QString GET_MODULE_NAME() = 0;
    virtual QString GET_BUTTON_TEXT() = 0;
    virtual ADAK_SQL_STRUCT * GET_DB_STRUCT() = 0;

};

//Bu da interface class ini unique bir isimle tanimlariz.
Q_DECLARE_INTERFACE(ADAK_PLUGIN_INTERFACE,"ADAKBILGISAYAR.PLUGIN.INTERFACE/1.0");

#endif // ADAK_PLUGIN_INTERFACE_H
