#ifndef DVZ_KUR_GUNCELLE_BATCH_H
#define DVZ_KUR_GUNCELLE_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class DVZ_KUR_GUNCELLE_BATCH;
}

class DVZ_KUR_GUNCELLE_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::DVZ_KUR_GUNCELLE_BATCH *        m_ui;

public:
                           DVZ_KUR_GUNCELLE_BATCH                            ( QWidget * parent = 0 );
                           ~DVZ_KUR_GUNCELLE_BATCH                           () {}
protected:
    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();


};

#endif // DVZ_KUR_GUNCELLE_BATCH_H
