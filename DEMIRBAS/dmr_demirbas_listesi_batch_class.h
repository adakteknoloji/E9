#ifndef DMR_DEMIRBAS_LISTESI_BATCH_H
#define DMR_DEMIRBAS_LISTESI_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"

namespace Ui {
    class DMR_DEMIRBAS_LISTESI_BATCH;
}

class DMR_DEMIRBAS_LISTESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::DMR_DEMIRBAS_LISTESI_BATCH *   m_ui;
    QList<int>                         m_grup_idleri;
    int                                m_sube_id;

public:
                           DMR_DEMIRBAS_LISTESI_BATCH                            (QWidget * parent = 0);
                           ~DMR_DEMIRBAS_LISTESI_BATCH                           () {}
protected:
    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);


};

#endif // DMR_DEMIRBAS_LISTESI_BATCH_H
