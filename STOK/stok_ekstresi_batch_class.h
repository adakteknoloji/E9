#ifndef STOK_EKSTRESI_BATCH_H
#define STOK_EKSTRESI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class STOK_EKSTRESI_BATCH;
}

class STOK_EKSTRESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                                     STOK_EKSTRESI_BATCH                  (QWidget * parent = 0);
                                     ~STOK_EKSTRESI_BATCH                 () {}
private:
    Ui::STOK_EKSTRESI_BATCH *        m_ui;

    int                              m_entegrasyon_durumu;
    int                              m_depo_id;
    int                              m_depo_islem_yetkisi;

    void                             SETUP_FORM                           ();
    int                              CHECK_VAR                            ( QObject * object );
    int                              CHECK_RUN                            ();
    int                              CHECK_EXIT                           ();
    void                             RUN_BATCH                            ();

    void                             SEARCH_EDIT_CLICKED                  ( QWidget *widget, QLineEdit *line_edit );

};

#endif // STOK_EKSTRESI_BATCH_H
