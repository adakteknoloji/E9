#ifndef PRS_PERSONEL_RAPORU_BATCH_CLASS_H
#define PRS_PERSONEL_RAPORU_BATCH_CLASS_H

#include "batch_kernel.h"
#include "ui_prs_personel_raporu_batch.h"

namespace Ui {
    class PRS_PERSONEL_RAPORU_BATCH;
}

class PRS_PERSONEL_RAPORU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::PRS_PERSONEL_RAPORU_BATCH *   m_ui;

public:
                           PRS_PERSONEL_RAPORU_BATCH                  (  QWidget * parent = 0 );
                           ~PRS_PERSONEL_RAPORU_BATCH                 () {}


protected:
    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();


 private:
    int                    m_sube_id;

};

#endif // PRS_PERSONEL_RAPORU_BATCH_H
