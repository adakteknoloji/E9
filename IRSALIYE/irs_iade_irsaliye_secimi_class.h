#ifndef IRS_IADE_IRSALIYE_SECIMI_H
#define IRS_IADE_IRSALIYE_SECIMI_H

#include "secim_kerneli.h"

class IRS_IADE_IRSALIYE_SECIMI : public SECIM_KERNELI
{

public:
     IRS_IADE_IRSALIYE_SECIMI ( QString p_irsaliye_tarihi, int p_cari_hesap_id, int p_irsaliye_alis_satis_turu, int p_irsaliye_turu, QWidget * p_parent );
     ~IRS_IADE_IRSALIYE_SECIMI       () {}

    int     *m_secilen_id;

private:

    QString m_irsaliye_tarihi_bitis;

    int     m_cari_hesap_id;
    int     m_irsaliye_alis_satis_turu;
    int     m_irsaliye_turu;

    void    FILL_TABLE();
    int     SINGLE_LINE_SELECTED( int selected_row_number );
};

#endif // IRS_IADE_IRSALIYE_SECIMI_H


