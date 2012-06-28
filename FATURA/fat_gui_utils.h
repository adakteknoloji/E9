#ifndef FAT_GUI_UTILS_H
#define FAT_GUI_UTILS_H

class QComboBox;
class QWidget;

void FAT_FILL_COMBOBOX_FATURA_TURU                          ( QComboBox * p_comboBox, int p_irsaliye_stok_fisi_alis_satis_turu, int p_fatura_turu, int p_iade_faturasi_mi );
bool FAT_IADE_FATURA_SATIR_EKLE_ICIN_MIKTAR_KONTROLU        ( int p_fatura_satiri_id, double p_fatura_satir_miktari, QWidget * p_parent );
bool FAT_IADE_FATURA_SATIR_GUNCELLE_ICIN_MIKTAR_KONTROLU    ( int p_fatura_satiri_id, double p_fatura_satir_miktari, QWidget * p_parent );

void FAT_FILL_ACIK_KAPALI_COMBO_BOX                         ( QComboBox *p_combo_box );

void FAT_FILL_ALIS_SATIS_FAT_TURU_COMBO_BOX                 ( QComboBox *p_combo_box );

#endif // FAT_GUI_UTILS_H
