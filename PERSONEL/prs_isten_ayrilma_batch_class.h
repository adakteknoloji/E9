#ifndef PRS_ISTEN_AYRILMA_BATCH_CLASS_H
#define PRS_ISTEN_AYRILMA_BATCH_CLASS_H

#include "batch_kernel.h"
#include "ui_prs_isten_ayrilma_batch.h"
#include "prs_enum.h"
#include "prs_struct.h"


namespace Ui {
    class PRS_ISTEN_AYRILMA_BATCH;
}


class PRS_ISTEN_AYRILMA_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::PRS_ISTEN_AYRILMA_BATCH *   m_ui;

public:
                           PRS_ISTEN_AYRILMA_BATCH                  ( QWidget * parent = 0 );
                           ~PRS_ISTEN_AYRILMA_BATCH                 () {}

private :
    int                    m_personel_id;
    QDate                  m_ise_giris_tarihi;

private:
    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();
    void                   BUTTON_CLICKED                       ( QAbstractButton *, int );
    void                   SEARCH_EDIT_CLICKED                  ( QWidget * widget, QLineEdit * lineEdit);

};

#endif // PRS_ISTEN_AYRILMA_BATCH_H
