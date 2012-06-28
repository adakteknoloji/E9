#ifndef BNK_POS_SECIMI_CLASS_H
#define BNK_POS_SECIMI_CLASS_H

#include <QtGui/QDialog>
#include <QTableWidget>
#include "secim_kerneli.h"

class BNK_POS_SECIMI : public SECIM_KERNELI
{
public:
                         BNK_POS_SECIMI(QWidget* parent = 0);
                         ~BNK_POS_SECIMI() {}
    int *                m_secilen_id;

private:
    void                 FILL_TABLE              ();

    QToolButton *        m_button_yeni_pos_kaydi;

    int                  SINGLE_LINE_SELECTED    (int selected_row_number);
};

#endif // BNK_POS_SECIMI_H


