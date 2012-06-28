#ifndef ADRES_TOPLU_MAIL_GONDERIMI_BATCH_H
#define ADRES_TOPLU_MAIL_GONDERIMI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class ADRES_TOPLU_MAIL_GONDERIMI_BATCH;
}

class ADRES_TOPLU_MAIL_GONDERIMI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT



public:
                           ADRES_TOPLU_MAIL_GONDERIMI_BATCH                 ( QStringList P_TOPLU_MAIL_BILGILERI,
                                                                              QWidget *   parent = 0 );
                           ~ADRES_TOPLU_MAIL_GONDERIMI_BATCH                () {}
private:

    Ui::ADRES_TOPLU_MAIL_GONDERIMI_BATCH    *   m_ui;

    QStringList                                 M_TOPLU_MAIL_BILGILERI;

    QString                                     m_to_email_address;
    QString                                     m_to_email_address_with_record_id;

    void                                        SETUP_FORM                 ();

    int                                         CHECK_VAR                  ( QObject * object );
    int                                         CHECK_RUN                  ();
    void                                        RUN_BATCH                  ();

};

#endif // ADRES_TOPLU_MAIL_GONDERIMI_BATCH_H
