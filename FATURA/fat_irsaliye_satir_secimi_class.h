#ifndef FAT_IRSALIYE_SATIR_SECIMI_H
#define FAT_IRSALIYE_SATIR_SECIMI_H

#include "secim_kerneli.h"

class FAT_IRSALIYE_SATIR_SECIMI : public SECIM_KERNELI
{
public:
             FAT_IRSALIYE_SATIR_SECIMI       ( QString p_fatura_tarihi, int p_cari_hesap_id, int p_fatura_turu, QWidget * p_parent = 0 );
             ~FAT_IRSALIYE_SATIR_SECIMI      () {}

    int  *   m_secilen_id;

private:

    QString  m_irsaliye_tarihi_bitis;
    QString  m_irsaliye_tarihi_baslangic;
    int      m_cari_hesap_id;
    int      m_irsaliye_turu;

    void     FILL_TABLE              ();
    int      SINGLE_LINE_SELECTED    ( int p_selected_row_number );
};

#endif // FAT_IRSALIYE_SATIR_SECIMI_H
