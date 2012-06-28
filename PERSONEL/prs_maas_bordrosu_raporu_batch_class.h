#ifndef PRS_MAAS_BORDROSU_RAPORU_BATCH_CLASS_H
#define PRS_MAAS_BORDROSU_RAPORU_BATCH_CLASS_H

#include <batch_kernel.h>
#include "ui_prs_maas_bordrosu_raporu_batch.h"


namespace Ui {
    class PRS_MAAS_BORDROSU_RAPORU_BATCH;
}

class PRS_MAAS_BORDROSU_RAPORU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::PRS_MAAS_BORDROSU_RAPORU_BATCH *   m_ui;

public:
                           PRS_MAAS_BORDROSU_RAPORU_BATCH                  ( int p_sube_id, QWidget * parent = 0 );
                           ~PRS_MAAS_BORDROSU_RAPORU_BATCH                 () {}

private :
    int                    m_personel_id;
    QString                m_sicil_no;
    int                    m_ay;
    int                    m_yil;
    int                    m_sube_id;


protected:
    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();
    void                   SEARCH_EDIT_CLICKED                  ( QWidget *, QLineEdit * line_edit);

private:
    void                   CHECK_BOX_CHANGED                    ();
    void                   COMBO_BOX_BORDRO_YILI_DOLDUR         ();
    void                   PERSONELI_SEC                        ();

};

#endif // PRS_MAAS_BORDROSU_RAPORU_BATCH_H
