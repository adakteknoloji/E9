#ifndef SUBE_DEPO_SECIMI_H
#define SUBE_DEPO_SECIMI_H

#include "secim_kerneli.h"

class SUBE_DEPO_SECIMI : public SECIM_KERNELI
{
public  :
    SUBE_DEPO_SECIMI                                ( int sube_id , QWidget *  parent = 0 );
    ~SUBE_DEPO_SECIMI                               () {}

     int         *        m_secilen_id;

private :

    void                 FILL_TABLE           ();

    QToolButton *        m_button_yeni_depo;

    int                  m_sube_id;

    int                  SINGLE_LINE_SELECTED ( int selected_row_number );
};

#endif // SUBE_DEPO_SECIMI_H


