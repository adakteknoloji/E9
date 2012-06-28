#ifndef CARI_GUI_UTILS_H
#define CARI_GUI_UTILS_H

class QWidget;
class QComboBox;
class QString;

void    OPEN_CARI_NOT_DEFTERI       ( int cari_hesap_id , int modul_id, int program_id , QWidget * parent = 0);
void    OPEN_CARI_ZARF_YAZDIR       ( int cari_hesap_id , QString printer_name         , QWidget * parent = 0);
void    OPEN_CARI_EMAIL_FISI        ( int cari_hesap_id , QWidget * parent = 0                               );
void    OPEN_CARI_TELEFON_FISI      ( int cari_hesap_id , QWidget * parent = 0                               );
void    OPEN_CARI_FIRMA_YETKILILERI ( int cari_hesap_id , QWidget * parent                                   );

//Siradaki cari kodu alir.Parametre olarak da programa ait kod gonderilmeli.
//Mesela SM00005 ise fonksiyon CARI_GET_NEXT_CARI_KODU("SM") cagrildigindan
//SM00006 dondurur.Doner deger empty ise kaydetme islemi durdurulmali.
QString CARI_GET_NEXT_CARI_KODU     ( QString program_kod_oneki, int program_id ,int module_id , bool start_transaction = true );

void    CARI_FILL_CARI_KART_TIPI_COMBO_BOX    ( QComboBox * p_combo_box  );

void    CARI_FILL_CARI_ENT_FIS_TURU_COMBO_BOX ( QComboBox * p_combo_box  );

#endif // CARI_GUI_UTILS_H
