#ifndef IRS_IADE_SATIR_SECIMI_H
#define IRS_IADE_SATIR_SECIMI_H

#include "secim_kerneli.h"

class IRS_IADE_SATIR_SECIMI : public SECIM_KERNELI
{
public:

    int         *        m_secilen_id;

    IRS_IADE_SATIR_SECIMI                                ( int p_irsaliye_id, QWidget *  parent = 0 );
    ~IRS_IADE_SATIR_SECIMI                               () {}


private:

    int                 m_irsaliye_id;

    void                FILL_TABLE           ();
    int                 SINGLE_LINE_SELECTED ( int selected_row_number );
};

#endif // IRS_IADE_SATIR_SECIMI_H


