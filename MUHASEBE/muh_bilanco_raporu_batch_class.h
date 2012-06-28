#ifndef MUH_BILANCO_RAPORU_BATCH_H
#define MUH_BILANCO_RAPORU_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"

namespace Ui {
    class MUH_BILANCO_RAPORU_BATCH;
}

class MUH_BILANCO_RAPORU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::MUH_BILANCO_RAPORU_BATCH *   m_ui;

public:
                             MUH_BILANCO_RAPORU_BATCH                            (QWidget *parent = 0);
                             ~MUH_BILANCO_RAPORU_BATCH                           (){}
private:
    void                     SETUP_FORM                           ();
    int                      CHECK_VAR                            (QObject * object);
    int                      CHECK_RUN                            ();
    void                     RUN_BATCH                            ();
};

#endif // MUH_BILANCO_RAPORU_BATCH_H
