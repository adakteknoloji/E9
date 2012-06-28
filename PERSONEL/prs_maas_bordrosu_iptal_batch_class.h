#ifndef PRS_MAAS_BORDROSU_IPTAL_BATCH_CLASS_H
#define PRS_MAAS_BORDROSU_IPTAL_BATCH_CLASS_H

#include <batch_kernel.h>
#include "ui_prs_maas_bordrosu_iptal_batch.h"


namespace Ui {
    class PRS_MAAS_BORDROSU_IPTAL_BATCH;
}

class PRS_MAAS_BORDROSU_IPTAL_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::PRS_MAAS_BORDROSU_IPTAL_BATCH *   m_ui;

public:
                           PRS_MAAS_BORDROSU_IPTAL_BATCH                  ( QWidget * parent = 0 );
                           ~PRS_MAAS_BORDROSU_IPTAL_BATCH                 () {}

private :
    int                    m_personel_id;
    int                    m_ay;
    int                    m_yil;
    int                    m_sube_id;


protected:
    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

private:
    void                   BORDRO_IPTAL_ET                      ( int p_bordro_id );
    void                   COMBO_BOX_IPTAL_EDILECEK_BORDRO_AY_VE_YILI_DOLDUR ();

    int                    BORDRO_IPTAL_EDILEBILIR_MI           ();
};

#endif // PRS_MAAS_BORDROSU_IPTAL_BATCH_H
