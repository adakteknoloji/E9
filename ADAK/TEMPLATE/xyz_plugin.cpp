#include <QtGui>
#include "adak_sql.h"
#include "xyz_plugin.h"
#include "xyz_db.h"
#include "xyz.h"

// Burada fonksiyonlari tanimliyoruz.Yonetim tarafindan cagrildiginda neler yapacagimiz belirtiyoruz.
// Mesela GET_PLUGIN_NAME i cagrirsa yonetim ona plugin in ismini donduruyoruz veya SHOW_GUI derse gui mizi gosteriyoruz.

// Modul ismi burda modul e koymus oldugumuz ismi kullanmaliyiz
// Mesela isletme_defteri gibi.
QString module_name = "xyz";

// Burada buton uzerinde ve menulerde gorulecek yani kullanicinin gorecegi isimdir.Bu yuzden uygun bir isim olmalidir.
// Mesela plugin_name degiskeni isletme_defteri olan bir pluginin, button text i İşletme Defteri olabilir.
QString button_text = "XYZ YYZ";

// Burda ise icon path in veriyoruz.Bu icon menulerde ve buttonlarda gosterilecektir.
QString icon        = ":/icons/xyz.png";

// Kullanilacak database bize yonetim tarafindan SHOW_GUI esnasinda mevcutsa parametre olarak gonderilecektir.
// Biz burda global bir ADAK_SQL objesi yaratarak (DB gibi) buna gonderilen parametreyi atarsak tum formlarimizda
// bunu extern edip kullanabiliriz.
extern ADAK_SQL *           DB;

// Burada,fonksiyon eger cagrilirsa plugin in gui sini gostermemiz lazim.Ayrica yonetim tarafindan gonderilen database global bir degiskene(DB)
// esitleyip diger formlarda bunu extern ederek kullanabiliriz.

/**************************************************************************************
                   XYZ_PLUGIN::SHOW_GUI
***************************************************************************************/

void XYZ_PLUGIN::SHOW_GUI ( ADAK_SQL * db, QWidget * parent )
{
    if (db EQ NULL) {
      return;
    }
    DB = db;
    SHOW_XYZ ( parent );
}

// Bu fonksiyonda modulun database header inda bulunan struct in adresini dondurmeliyiz.Boylelikle modulun database leri olusturulabilir.

/**************************************************************************************
                   XYZ_PLUGIN::GET_DB_STRUCT
***************************************************************************************/

ADAK_SQL_STRUCT * XYZ_PLUGIN::GET_DB_STRUCT()
{
    return &xyz_db;
}

// Burada programin bagli oldugu modulun idsini dondurmeliyiz. E9_MODULU gibi.

/**************************************************************************************
                   XYZ_PLUGIN::GET_PROGRAM_ID
***************************************************************************************/

int XYZ_PLUGIN::GET_PROGRAM_ID()
{
    return XYZ_PROGRAMI;
}

// module vermis oldugumuz ismi dondurmeliyiz

/**************************************************************************************
                   XYZ_PLUGIN::GET_MODULE_NAME
***************************************************************************************/

QString XYZ_PLUGIN::GET_MODULE_NAME()
{
    return module_name;
}

// butonda set lenecek text i dondurmeliyiz.

/**************************************************************************************
                   XYZ_PLUGIN::GET_BUTTON_TEXT
***************************************************************************************/

QString XYZ_PLUGIN::GET_BUTTON_TEXT()
{
    return button_text;
}

// QIcon olarak buton iconunu dondurmeliyiz.

/**************************************************************************************
                   XYZ_PLUGIN::GET_ICON
***************************************************************************************/

QIcon XYZ_PLUGIN::GET_ICON()
{
    return QIcon ( icon );
}

// Burda ilk deger pluginName diger deger ise class ismidir.
// Ilk deger(plugin_name) projedeki pro dosyasindaki TARGETle ayni olmalidir.
Q_EXPORT_PLUGIN2 ( XYZ, XYZ_PLUGIN );

