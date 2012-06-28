#ifndef PRS_IBRANAME_MADDELERI_BATCH_CLASS_H
#define PRS_IBRANAME_MADDELERI_BATCH_CLASS_H

#include <QtGui/QDialog>
#include "batch_kernel.h"
#include "ui_prs_ibraname_maddeleri_batch.h"

namespace Ui {
    class PRS_IBRANAME_MADDELERI_BATCH;
}

class PRS_IBRANAME_MADDELERI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public :
                        PRS_IBRANAME_MADDELERI_BATCH               (int firma_id, QWidget *parent = 0);
                        ~PRS_IBRANAME_MADDELERI_BATCH              () {}

private :

    Ui::PRS_IBRANAME_MADDELERI_BATCH *    m_ui;

    int                 m_firma_id;

    void                SETUP_FORM                              ();
    int                 CHECK_VAR                               (QObject * object);
    int                 CHECK_RUN                               ();
    void                RUN_BATCH                               ();
    void                STANDART_IBRANAME_MADDELERI_OLUSTUR      ();
};

#endif // PRS_IBRANAME_MADDELERI_BATCH_H
