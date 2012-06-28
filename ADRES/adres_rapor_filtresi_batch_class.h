#ifndef ADRES_RAPOR_FILTRESI_BATCH_H
#define ADRES_RAPOR_FILTRESI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class ADRES_RAPOR_FILTRESI_BATCH;
}

class ADRES_RAPOR_FILTRESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           ADRES_RAPOR_FILTRESI_BATCH                            ( int p_rapor_turu, QWidget * parent = 0 );
                           ~ADRES_RAPOR_FILTRESI_BATCH                           () {}
private :

    Ui::ADRES_RAPOR_FILTRESI_BATCH      *    m_ui;

    QStringList                             M_ADRES_KAYIT_RAPORU_BILGILERI;
    QStringList                             M_ADRES_ETIKET_BILGILERI;
    QStringList                             M_TOPLU_MAIL_BILGILERI;
    QString                                 M_SMS_BILGILERI;
    QList<int>                              m_grup_idleri;

    int                                     m_adres_kayit_turu;
    int                                     m_adres_rapor_turu;

    void                                    SETUP_FORM                           ();

    int                                     CHECK_VAR                            ( QObject * object );
    int                                     CHECK_RUN                            ();
    void                                    RUN_BATCH                            ();

    bool                                    RAPOR_VERILERINI_OLUSTUR             ();

};

#endif // ADRES_RAPOR_FILTRESI_BATCH_H
