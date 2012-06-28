#ifndef PRS_ISTEN_AYRILANLAR_ARAMA_BATCH_CLASS_H
#define PRS_ISTEN_AYRILANLAR_ARAMA_BATCH_CLASS_H

#include "arama_kernel.h"
#include "ui_prs_isten_ayrilanlar_arama.h"

namespace Ui {
    class PRS_ISTEN_AYRILANLAR_ARAMA;
}

class PRS_ISTEN_AYRILANLAR_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           PRS_ISTEN_AYRILANLAR_ARAMA     ( QWidget * parent = 0 );
                           ~PRS_ISTEN_AYRILANLAR_ARAMA    (){}
  int *                     m_secilen_id;

private:
    Ui::PRS_ISTEN_AYRILANLAR_ARAMA *        m_ui;

protected:
    void                   SETUP_FORM                 ();
    int                    CHECK_VAR                  ( QObject * object );
    int                    CHECK_RUN                  ();
    void                   SEARCH                     ();
    int                    SINGLE_LINE_SELECTED       ( int selected_row_number );
    void                   CHECK_BOX_CHANGED          ();

};

#endif // PRS_ISTEN_AYRILANLAR_ARAMA_H
