#ifndef FAT_IADE_FATURA_SECIMI_H
#define FAT_IADE_FATURA_SECIMI_H

#include "secim_kerneli.h"

class FAT_IADE_FATURA_SECIMI : public SECIM_KERNELI
{

public:

    FAT_IADE_FATURA_SECIMI  ( int p_cari_hesap_id, int p_fatura_alis_satis_iade_turu, int p_fatura_turu, QWidget *p_parent );
    ~FAT_IADE_FATURA_SECIMI () {}

    int *m_secilen_id;

private:

    int  m_cari_hesap_id;
    int  m_fatura_alis_satis_iade_turu;
    int  m_fatura_turu;

    void FILL_TABLE();

    int  SINGLE_LINE_SELECTED ( int selected_row_number );

};

#endif // FAT_IADE_FATURA_SECIMI_H


