#ifndef MUH_HESAP_EKSTRESI_BATCH_H
#define MUH_HESAP_EKSTRESI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class MUH_HESAP_EKSTRESI_BATCH;
}

class MUH_HESAP_EKSTRESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           MUH_HESAP_EKSTRESI_BATCH                            (QString p_secili_hesap_kodu,int p_rapor_turu , QWidget *p_parent = 0);
                           ~MUH_HESAP_EKSTRESI_BATCH                           (){}
private:
    Ui::MUH_HESAP_EKSTRESI_BATCH *   m_ui;

    QString                m_secili_hesap;

    int                    m_rapor_turu;

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();
    void                   SEARCH_EDIT_CLICKED                  (QWidget *, QLineEdit *);
};

#endif // MUH_HESAP_EKSTRESI_BATCH_H
