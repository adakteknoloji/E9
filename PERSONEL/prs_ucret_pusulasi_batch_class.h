#ifndef PRS_UCRET_PUSULASI_BATCH_CLASS_H
#define PRS_UCRET_PUSULASI_BATCH_CLASS_H

#include "batch_kernel.h"

struct  PRS_UCRET_PUSULASI_RAPORU_STRUCT;

namespace Ui {
    class PRS_UCRET_PUSULASI_BATCH;
}

class PRS_UCRET_PUSULASI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           PRS_UCRET_PUSULASI_BATCH                            ( QWidget * parent = 0 );
                           ~PRS_UCRET_PUSULASI_BATCH                           () {}
private:

    Ui::PRS_UCRET_PUSULASI_BATCH *           m_ui;

    PRS_UCRET_PUSULASI_RAPORU_STRUCT *       M_UCRET_PUSULASI_VAR;
    QList<int>                               m_grup_idleri;

    int                                     m_sube_id;

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);
    int                    CHANGER_BUTTON_CLICKED               (QAbstractButton * p_button );

};

#endif // PRS_UCRET_PUSULASI_BATCH_H
