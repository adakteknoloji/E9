#ifndef SUBE_TREE_BATCH_CLASS_H
#define SUBE_TREE_BATCH_CLASS_H

#include "batch_kernel.h"

namespace Ui {
    class SUBE_TREE_BATCH;
}

class SUBE_TREE_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           SUBE_TREE_BATCH                            ( QWidget * parent = 0 );
                           ~SUBE_TREE_BATCH                           () {}
private:

    Ui::SUBE_TREE_BATCH *        m_ui;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

    void                   REFRESH_SUBE_SCHEMA                  ();


};

#endif // SUBE_TREE_BATCH_CLASS_H
