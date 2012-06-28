#ifndef PRS_EBILDIRGE_OLUSTUR_BATCH_CLASS_H
#define PRS_EBILDIRGE_OLUSTUR_BATCH_CLASS_H

#include "batch_kernel.h"

namespace Ui {
    class PRS_EBILDIRGE_OLUSTUR_BATCH;
}

class PRS_EBILDIRGE_OLUSTUR_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           PRS_EBILDIRGE_OLUSTUR_BATCH                            ( QWidget * parent = 0 );
                           ~PRS_EBILDIRGE_OLUSTUR_BATCH                           () {}
private:

    Ui::PRS_EBILDIRGE_OLUSTUR_BATCH *        m_ui;

    QString                m_file_path;

    int                    m_ay;
    int                    m_yil;
    int                    m_sube_id;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();
    void                   E_BILDIRGE_OLUSTUR                   ();

};

#endif // PRS_EBILDIRGE_OLUSTUR_BATCH_H
