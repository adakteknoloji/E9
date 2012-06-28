#ifndef E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH_CLASS_H
#define E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH_CLASS_H

#include "batch_kernel.h"

namespace Ui {
    class E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH;
}

class E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
     E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH ( int p_kitapcik_mi, QWidget * parent = 0 );
     ~E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH() {}
private:

    Ui::E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH *m_ui;

    int  m_kitapcik_mi;

    void SETUP_FORM();
    int  CHECK_VAR( QObject * object );
    int  CHECK_RUN();
    int  CHECK_EXIT();
    void RUN_BATCH();

};

#endif // E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH_CLASS_H
