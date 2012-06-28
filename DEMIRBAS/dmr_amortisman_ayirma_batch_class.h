#ifndef DMR_AMORTISMAN_AYIRMA_BATCH_H
#define DMR_AMORTISMAN_AYIRMA_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"

namespace Ui {
    class DMR_AMORTISMAN_AYIRMA_BATCH;
}

class DMR_AMORTISMAN_AYIRMA_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::DMR_AMORTISMAN_AYIRMA_BATCH *   m_ui;

      QList<int>           m_grup_idleri;
      int                  m_sube_id;

public:
                           DMR_AMORTISMAN_AYIRMA_BATCH                            (QWidget * parent = 0);
                           ~DMR_AMORTISMAN_AYIRMA_BATCH                           () {}
protected:
    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);
};

#endif // DMR_AMORTISMAN_AYIRMA_BATCH_H
