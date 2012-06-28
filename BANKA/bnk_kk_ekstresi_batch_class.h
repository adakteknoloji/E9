#ifndef BNK_KK_EKSTRESI_BATCH_CLASS_H
#define BNK_KK_EKSTRESI_BATCH_CLASS_H

#include "batch_kernel.h"

struct BNK_RAPOR_VARS;

namespace Ui {
    class BNK_KK_EKSTRESI_BATCH;
}

class BNK_KK_EKSTRESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

    public:
                           BNK_KK_EKSTRESI_BATCH                            ( QWidget * parent = 0 );
                           ~BNK_KK_EKSTRESI_BATCH                           () {}

    private:

    Ui::BNK_KK_EKSTRESI_BATCH *        m_ui;
    int                                m_kredi_karti_id;
    int                                m_hesap_no_id;

    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();
    void                   SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit);

};

#endif // BNK_KK_EKSTRESI_BATCH_H
