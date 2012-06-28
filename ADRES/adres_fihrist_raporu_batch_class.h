#ifndef ADRES_FIHRIST_RAPORU_BATCH_H
#define ADRES_FIHRIST_RAPORU_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class ADRES_FIHRIST_RAPORU_BATCH;
}

class ADRES_FIHRIST_RAPORU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                                        ADRES_FIHRIST_RAPORU_BATCH            ( QWidget * p_parent = 0 );
                                        ~ADRES_FIHRIST_RAPORU_BATCH           () {}
private :

    Ui::ADRES_FIHRIST_RAPORU_BATCH *      m_ui;

    QList<int>                          m_grup_idleri;

    void                                SETUP_FORM                           ();

    int                                 CHECK_VAR                            ( QObject * p_object );
    int                                 CHECK_RUN                            ();
    void                                RUN_BATCH                            ();

};

#endif // ADRES_FIHRIST_RAPORU_BATCH_H
