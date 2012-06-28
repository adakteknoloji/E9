#ifndef SUBE_SECIMI_H
#define SUBE_SECIMI_H

#include "secim_kerneli.h"

class SUBE_SECIMI : public SECIM_KERNELI
{
public:
    SUBE_SECIMI                                ( QWidget *  parent = 0 );
    ~SUBE_SECIMI                               () {}

    int         *        m_secilen_id;

private:

    void                 FILL_TABLE           ();

    int                  SINGLE_LINE_SELECTED ( int selected_row_number );

};

#endif // SUBE_SECIMI_H


