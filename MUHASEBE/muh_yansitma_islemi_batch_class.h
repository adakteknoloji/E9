#ifndef MUH_YANSITMA_ISLEMI_BATCH_H
#define MUH_YANSITMA_ISLEMI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class MUH_YANSITMA_ISLEMI_BATCH;
}

class MUH_YANSITMA_ISLEMI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

    public:
                           MUH_YANSITMA_ISLEMI_BATCH                            ( QWidget * parent = 0 );
                           ~MUH_YANSITMA_ISLEMI_BATCH                           () {}

    private:
    Ui::MUH_YANSITMA_ISLEMI_BATCH *        m_ui;
    QDate              m_donem_bas_tarihi;
    QDate              m_donem_bts_tarihi;


    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

};

#endif // MUH_YANSITMA_ISLEMI_BATCH_H
