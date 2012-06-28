#ifndef PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH_CLASS_H
#define PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH_CLASS_H

#include "batch_kernel.h"
#include "ui_prs_ihbar_ve_kidem_tazminati_batch.h"

namespace Ui {
    class PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH;
}

class PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH *   m_ui;

public:
                           PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH       ( QWidget * parent = 0 );
                           ~PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH      () {}
protected:
    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

};

#endif // PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH_H
