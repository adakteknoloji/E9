#ifndef STOK_GUI_UTILS_H
#define STOK_GUI_UTILS_H

class QComboBox;
class QString;

void                STK_FILL_URUN_BIRIMLERI_COMBOBOX        ( int         p_urun_id,
                                                              QComboBox * p_comboBox_urun_birimleri,
                                                              QString *   P_STOK_BIRIM_ADLARI );

void                STK_FILL_STOK_FIS_TURU_COMBOBOX         ( QComboBox * p_comboBox );

void                STK_FILL_STOK_KART_TURLERI_COMBOBOX     ( QComboBox * p_comboBox );

int                 STK_GET_STOK_KART_TURLERI_ENUM          ( QString p_stok_kart_turu );

void                STK_FILL_ALIS_SATIS_DURUMU_COMBO_BOX    ( QComboBox *p_combo_box );

int                 STK_GET_ALIS_SATIS_ENUM                 ( QString irsaliye_cinsi );

#endif // STOK_GUI_UTILS_H
