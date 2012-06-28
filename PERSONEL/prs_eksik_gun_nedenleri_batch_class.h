#ifndef PRS_EKSIK_GUN_NEDENLERI_BATCH_CLASS_H
#define PRS_EKSIK_GUN_NEDENLERI_BATCH_CLASS_H

#include "batch_kernel.h"

namespace Ui {
    class PRS_EKSIK_GUN_NEDENLERI_BATCH;
}

class PRS_EKSIK_GUN_NEDENLERI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           PRS_EKSIK_GUN_NEDENLERI_BATCH                            ( QWidget * parent = 0 );
                           ~PRS_EKSIK_GUN_NEDENLERI_BATCH                           () {}
private:

    Ui::PRS_EKSIK_GUN_NEDENLERI_BATCH *        m_ui;

    QString                m_sicil_no;
    int                    m_personel_id;
    int                    m_cari_hesap_id;

    int                    m_ay;
    int                    m_yil;
    int                    m_sube_id;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();
    void                   SEARCH_EDIT_CLICKED                  ( QWidget *widget, QLineEdit *line_edit );

};

#endif // PRS_EKSIK_GUN_NEDENLERI_BATCH_H
