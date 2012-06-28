#ifndef E9_ONAR_BATCH_H
#define E9_ONAR_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class E9_ONAR_BATCH;
}

class E9_ONAR_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::E9_ONAR_BATCH *   m_ui;

    bool                  m_tum_sistemi_onar;

public:
                           E9_ONAR_BATCH                            (QWidget * parent = 0);
                           ~E9_ONAR_BATCH                           () {}
protected:
    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

    void                   TUM_SISTEMI_ONAR                                ( );

    void                   BILGILENDIR                          (QString message);

};

#endif // E9_ONAR_BATCH_H
