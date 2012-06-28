#ifndef MUH_TOPLU_FIS_RAPORU_BATCH_H
#define MUH_TOPLU_FIS_RAPORU_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"

namespace Ui {
    class MUH_TOPLU_FIS_RAPORU_BATCH;
}

class MUH_TOPLU_FIS_RAPORU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           MUH_TOPLU_FIS_RAPORU_BATCH                            (QWidget *parent = 0);
                           ~MUH_TOPLU_FIS_RAPORU_BATCH                           (){}
private:
    Ui::MUH_TOPLU_FIS_RAPORU_BATCH *   m_ui;

    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

};



#endif // MUH_TOPLU_FIS_RAPORU_BATCH_H
