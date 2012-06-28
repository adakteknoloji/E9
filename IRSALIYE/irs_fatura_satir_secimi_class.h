#ifndef IRS_FATURA_SATIR_SECIMI_H
#define IRS_FATURA_SATIR_SECIMI_H

#include "secim_kerneli.h"

class IRS_FATURA_SATIR_SECIMI : public SECIM_KERNELI
{

public:

    IRS_FATURA_SATIR_SECIMI( int p_cari_hesap_id, int p_irsaliye_turu, QWidget * p_parent = 0 );
    ~IRS_FATURA_SATIR_SECIMI() {}

    int  *  m_secilen_id;

private:

    int m_cari_hesap_id;
    int m_fatura_turu;

    void FILL_TABLE();
    int  SINGLE_LINE_SELECTED( int p_selected_row_number );
};

#endif // IRS_FATURA_SATIR_SECIMI_H


