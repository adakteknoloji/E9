#ifndef MUH_GELIR_TABLOSU_BATCH_H
#define MUH_GELIR_TABLOSU_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"



namespace Ui {
    class MUH_GELIR_TABLOSU_BATCH;
}

class MUH_GELIR_TABLOSU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::MUH_GELIR_TABLOSU_BATCH *   m_ui;

public:
                           MUH_GELIR_TABLOSU_BATCH                            (QWidget *parent = 0);
                           ~MUH_GELIR_TABLOSU_BATCH                           () {}
private:
    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

};

#endif // MUH_GELIR_TABLOSU_BATCH_H
