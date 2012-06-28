#ifndef BNK_POS_EKSTRESI_BATCH_CLASS_H
#define BNK_POS_EKSTRESI_BATCH_CLASS_H

#include "batch_kernel.h"

struct BNK_RAPOR_VARS;

namespace Ui {
    class BNK_POS_EKSTRESI_BATCH;
}

class BNK_POS_EKSTRESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           BNK_POS_EKSTRESI_BATCH              ( QWidget * p_parent = 0 );
                           ~BNK_POS_EKSTRESI_BATCH             () {}


private:

    BNK_RAPOR_VARS *        BNK_RV;

    Ui::BNK_POS_EKSTRESI_BATCH *   m_ui;

    int                    m_pos_id;
    int                    m_hesap_no_id;

    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            ( QObject * p_object );
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();
    void                   SEARCH_EDIT_CLICKED                  ( QWidget * p_widget ,QLineEdit * p_line_edit );

};

#endif // BNK_POS_EKSTRESI_BATCH_H
