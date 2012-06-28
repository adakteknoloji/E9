#ifndef BNK_MAKRO_SECIMI_CLASS_H
#define BNK_MAKRO_SECIMI_CLASS_H

#include <QTableWidget>
#include "secim_kerneli.h"

class BNK_MAKRO_SECIMI : public SECIM_KERNELI
{
public:
                         BNK_MAKRO_SECIMI(QWidget* parent = 0);
                         ~BNK_MAKRO_SECIMI() {}
    int *                m_secilen_id;

private:
    void                 FILL_TABLE              ();


    int                  SINGLE_LINE_SELECTED    (int selected_row_number);
};

#endif // BNK_MAKRO_SECIMI_H


