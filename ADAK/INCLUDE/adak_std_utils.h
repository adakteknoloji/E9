#ifndef ADAK_STD_UTILS_H
#define ADAK_STD_UTILS_H

#include <QStringList>
class QComboBox;

void ADD_ADAK_STD();

//GET fonksiyonlarinda parametre olarak empty("") gonderilirse o degiskene gore filtreleme yapilmaz.
//Tum degerleri dondurur.

QString         GET_ULKE_TELEFON_KODU      ( QString ulke_adi     );
QStringList     GET_ULKELER                ( QComboBox * ComboBox );
QStringList     GET_UYRUKLAR               ( QComboBox * ComboBox );
QStringList     GET_DOVIZLER               ( QComboBox * ComboBox );
QStringList     GET_CINSIYETLER            ( QComboBox * ComboBox );
QStringList     GET_OGRENIM_DURUMLARI      ( QComboBox * ComboBox );
QStringList     GET_KAN_GRUPLARI           ( QComboBox * ComboBox );
QStringList     GET_MEDENI_HALLER          ( QComboBox * ComboBox );

QStringList     GET_VERGI_DAIRELERI        ( QComboBox * ComboBox,QString il_adi   = "" );

QStringList     GET_ULKE_TELEFON_KODLARI   ( QComboBox * ComboBox );
QStringList     GET_GSM_KODLARI            ( QComboBox * ComboBox, QString ulke_telefon_kodu = "90", QString ulke_adi = "" );
QStringList     GET_ALAN_KODLARI           ( QComboBox * ComboBox, QString ulke_telefon_kodu = "90", QString ulke_adi = "" );

QString         GET_IL_ALAN_KODU           ( QString il_adi , QString ulke_adi );
QStringList     GET_ILLER                  ( QComboBox * ComboBox, QString ulke_adi = "Türkiye"  );
QStringList     GET_ILCELER                ( QComboBox * ComboBox, QString il_adi   = ""         );

QStringList     GET_BANKALAR               ( QComboBox * ComboBox );
QStringList     GET_BANKA_SUBE_ADLARI      ( QComboBox * ComboBox, QString banka_adi = "", QString il_adi = "" );
QStringList     GET_BANKA_SUBE_KODLARI     ( QComboBox * ComboBox, QString banka_adi = "", QString il_adi = "" );
QString         GET_BANKA_SUBE_ADI         ( QString banka_adi , QString sube_kodu );
QString         GET_BANKA_SUBE_KODU        ( QString banka_adi , QString sube_adi , QString il_adi = "" );

QStringList     GET_SEKTORLER              ( QComboBox * comboBox );

QStringList     GET_UNVANLAR               ( QComboBox * comboBox );
QStringList     GET_MESLEKLER              ( QComboBox * comboBox );

// Parametre olarak gönderilen comboboxın indexini, gönderilen stringin olduğu indexe e setler.
// Böylece text indexini doğru setlemiş olur.
// Eğer gönderilen text i bulamzasa direkt olarak combobox textini gönderilen stringe setler.

void            FIND_AND_SET_COMBOBOX_TEXT ( QComboBox * p_comboBox, QString p_text );

const char **   GET_ADAK_STD_DOVIZLER_STRUCT();


#endif // ADAK_STD_UTILS_H
