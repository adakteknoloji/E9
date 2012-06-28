#ifndef ADRES_EMAIL_LOG_BATCH_H
#define ADRES_EMAIL_LOG_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class ADRES_EMAIL_LOG_BATCH;
}

class ADRES_EMAIL_LOG_BATCH : public BATCH_KERNEL
{
    Q_OBJECT
public:
                           ADRES_EMAIL_LOG_BATCH                ( QWidget * parent = 0 );
                           ~ADRES_EMAIL_LOG_BATCH               () {}
private:

    Ui::ADRES_EMAIL_LOG_BATCH *        m_ui;

    void    		   SETUP_FORM                           ();

    int                    CHECK_VAR                            ( QObject * object );

    int                    CHECK_RUN                            ();

    void                   RUN_BATCH                            ();

};

#endif // ADRES_EMAIL_LOG_BATCH_H
