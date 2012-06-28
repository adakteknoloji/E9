#ifndef MUH_GUI_UTILS_H
#define MUH_GUI_UTILS_H

class QString;
class QComboBox;

int     MUH_CHECK_MUHASEBE_ALARMLARI   ( QString tam_hesap_kodu,double eklenecek_borc,double eklenecek_alacak );

void    MUH_FILL_ENT_FISLERI_COMBO_BOX ( QComboBox * p_combo_box );

void    MUH_FILL_FIS_TURLERI_COMBO_BOX ( QComboBox * p_combo_box );

#endif // MUH_GUI_UTILS_H
