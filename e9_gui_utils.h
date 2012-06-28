#ifndef E9_GUI_ITULS_H
#define E9_GUI_ITULS_H

class QString;
class QAdakDate;
class QComboBox;
class QWidget;


void        E9_FILL_COMBOBOX_AYLAR                         ( QComboBox * p_combo_box );

void        E9_FILL_KDV_ORANLARI_COMBOBOX                  ( QComboBox * p_comboBox );
void        E9_FILL_OTV_ORANLARI_COMBOBOX                  ( QComboBox * p_comboBox );

void        E9_FILL_COMBOBOX_ADAK_MODULES                 ( QComboBox * p_comboBox          );

void        E9_SET_WINDOW_ICON                             ( QWidget * p_window );

bool        E9_SUBEYE_UNITE_KAYDI_YAPILDI_MI               ( );

void        E9_FILL_COMBOBOX_RAPOR_SIRKET_FILTRESI         ( QComboBox * combobox_sirket_turu );

int         E9_MALI_YIL_TARIH_ARALIGI_KONTROLU             ( QAdakDate * p_baslangic_tarihi, QAdakDate * p_bitis_tarihi );

void        E9_FILL_KULLANICININ_CALISTIGI_SIRKETLER       ( QComboBox * p_combo_box  );



#endif // E9_GUI_ITULS_H
