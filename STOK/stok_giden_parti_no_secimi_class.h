#ifndef STOK_GIDEN_PARTI_NO_SECIMI_CLASS_H
#define STOK_GIDEN_PARTI_NO_SECIMI_CLASS_H

#include "secim_kerneli.h"

class STOK_GIDEN_PARTI_NO_SECIMI : public SECIM_KERNELI
{
public:
    STOK_GIDEN_PARTI_NO_SECIMI( int p_depo_id, int p_urun_id, int p_secili_parti_no, QWidget *  parent = 0 );
    ~STOK_GIDEN_PARTI_NO_SECIMI() {}

public:
    int        *m_secilen_id;

private:

    int         m_depo_id;
    int         m_urun_id;

    void        FILL_TABLE           ();
    int         SINGLE_LINE_SELECTED ( int selected_row_number );

};

#endif // STOK_GIDEN_PARTI_NO_SECIMI_CLASS_H


