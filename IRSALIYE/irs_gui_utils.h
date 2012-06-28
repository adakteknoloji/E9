#ifndef IRS_GUI_UTILS_H
#define IRS_GUI_UTILS_H

class QComboBox;
class QWidget;

void    IRS_FILL_IRSALIYE_TURU_COMBOBOX                         ( QComboBox *   p_comboBox,
                                                                  int           p_irsaliye_stok_fisi_alis_satis_turu, int p_irsaliye_turu );

bool    IRS_IADE_IRSALIYE_SATIR_EKLE_ICIN_MIKTAR_KONTROLU       ( int           p_irsaliye_satiri_id,
                                                                  double        p_irsaliye_satir_miktari,
                                                                  QWidget *     p_parent );

bool    IRS_IADE_IRSALIYE_SATIR_GUNCELLE_ICIN_MIKTAR_KONTROLU    ( int          p_irsaliye_satiri_id,
                                                                   double       p_irsaliye_satir_miktari,
                                                                   QWidget *    p_parent );

void    IRS_FILL_ALIS_SATIS_COMBO_BOX                            ( QComboBox *p_combo_box );
void    IRS_FILL_FATURA_DURUMU_COMBO_BOX                         ( QComboBox *p_combo_box );
int     IRS_GET_ALIS_SATIS_ENUM                                  ( QString irsaliye_cinsi );


#endif // IRS_GUI_UTILS_H
