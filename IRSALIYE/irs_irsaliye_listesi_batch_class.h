#ifndef IRS_IRSALIYE_LISTESI_BATCH_H
#define IRS_IRSALIYE_LISTESI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class IRS_IRSALIYE_LISTESI_BATCH;
}

class IRS_IRSALIYE_LISTESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           IRS_IRSALIYE_LISTESI_BATCH                            ( QWidget * parent = 0 );
                           ~IRS_IRSALIYE_LISTESI_BATCH                           () {}
private:

    Ui::IRS_IRSALIYE_LISTESI_BATCH *        m_ui;
    int                                     m_cari_hesap_id;
    int                                     m_depo_id;

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);

};

#endif // IRS_IRSALIYE_LISTESI_BATCH_H
