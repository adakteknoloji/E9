#ifndef FAT_IADE_SATIR_SECIMI_H
#define FAT_IADE_SATIR_SECIMI_H

#include "secim_kerneli.h"

class FAT_IADE_SATIR_SECIMI : public SECIM_KERNELI
{
public:

    int         *        m_secilen_id;

    FAT_IADE_SATIR_SECIMI                                ( int p_fatura_id, QWidget *  parent = 0 );
    ~FAT_IADE_SATIR_SECIMI                               () {}

private:

    int                     m_fatura_id;

    void                    FILL_TABLE           ();

    int                     SINGLE_LINE_SELECTED ( int selected_row_number );
};

#endif // FAT_IADE_SATIR_SECIMI_H


