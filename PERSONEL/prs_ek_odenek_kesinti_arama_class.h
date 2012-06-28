#ifndef PRS_EK_ODENEK_KESINTI_ARAMA_CLASS_H
#define PRS_EK_ODENEK_KESINTI_ARAMA_CLASS_H

#include <QtGui/QDialog>
#include "arama_kernel.h"
#include "ui_prs_ek_odenek_kesinti_arama.h"

class PRS_EK_ODENEK_KESINTI_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           PRS_EK_ODENEK_KESINTI_ARAMA                        ( int kesinti_odenek_turu, QWidget * parent = 0);
                           ~PRS_EK_ODENEK_KESINTI_ARAMA                       () {}

    int *                  m_secilen_id;
private:
    Ui::PRS_EK_ODENEK_KESINTI_ARAMA *    m_ui;
    int                    m_kesinti_odenek_turu;

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   SEARCH                               ();
    int                    SINGLE_LINE_SELECTED                 (int selected_row_number);

};

#endif // PRS_EK_ODENEK_KESINTI_ARAMA_H
