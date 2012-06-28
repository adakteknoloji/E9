#ifndef BNK_KREDI_KARTI_SECIMI_CLASS_H
#define BNK_KREDI_KARTI_SECIMI_CLASS_H

#include "secim_kerneli.h"

class BNK_KREDI_KARTI_SECIMI : public SECIM_KERNELI
{
public:
                         BNK_KREDI_KARTI_SECIMI(QWidget* parent = 0);
                         ~BNK_KREDI_KARTI_SECIMI() {}
    int *                m_secilen_id;

private:
    void                 FILL_TABLE              ();

    QToolButton *        m_button_yeni_kk;
    int                  SINGLE_LINE_SELECTED    (int selected_row_number);
};

#endif // BNK_KREDI_KARTI_SECIMI_H


