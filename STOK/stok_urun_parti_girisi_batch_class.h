#ifndef STOK_URUN_PARTI_GIRISI_BATCH_CLASS_H
#define STOK_URUN_PARTI_GIRISI_BATCH_CLASS_H

#include "batch_kernel.h"

namespace Ui {
    class STOK_URUN_PARTI_GIRISI_BATCH;
}

class STOK_URUN_PARTI_GIRISI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
          STOK_URUN_PARTI_GIRISI_BATCH( int *p_parti_kodu_id, int p_urun_id, int p_depo_id, QWidget * parent = 0 );
          ~STOK_URUN_PARTI_GIRISI_BATCH() {}

private:

    Ui::STOK_URUN_PARTI_GIRISI_BATCH *  m_ui;

    int   m_urun_id;
    int   m_depo_id;
    int   *m_gelen_parti_id;

    void  SETUP_FORM();
    int   CHECK_VAR( QObject * object );
    int   CHECK_RUN();
    int   CHECK_EXIT();
    void  RUN_BATCH();

};

#endif // STOK_URUN_PARTI_GIRISI_BATCH_CLASS_H
