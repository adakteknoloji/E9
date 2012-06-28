#ifndef STOK_URUN_LISTESI_BATCH_H
#define STOK_URUN_LISTESI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class STOK_URUN_LISTESI_BATCH;
}

class STOK_URUN_LISTESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
    STOK_URUN_LISTESI_BATCH ( int p_urun_mu_hizmet_mi, QWidget * parent = 0 );
    ~STOK_URUN_LISTESI_BATCH() {}

private:
    Ui::STOK_URUN_LISTESI_BATCH *m_ui;

    int         m_urun_mu_hizmet_mi;
    QList<int>  m_grup_idleri;

    void SETUP_FORM();
    int  CHECK_VAR( QObject * object );
    int  CHECK_RUN();
    void RUN_BATCH();
    void SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit);

};

#endif // STOK_URUN_LISTESI_BATCH_H
