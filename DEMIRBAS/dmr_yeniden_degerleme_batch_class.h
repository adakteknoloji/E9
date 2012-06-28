#ifndef DMR_YENIDEN_DEGERLEME_BATCH_H
#define DMR_YENIDEN_DEGERLEME_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"

namespace Ui {
    class DMR_YENIDEN_DEGERLEME_BATCH;
}

class DMR_YENIDEN_DEGERLEME_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::DMR_YENIDEN_DEGERLEME_BATCH *   m_ui;
    QList<int>             m_grup_idleri;

    int                    m_sube_id;

public:
                           DMR_YENIDEN_DEGERLEME_BATCH                            (QWidget * parent = 0);
                           ~DMR_YENIDEN_DEGERLEME_BATCH                           () {}
protected:
    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();
    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);


    void                   FILL_DEMIRBAS_GRUPLARI_ID_LISTESI(int parent_grup_id);

};

#endif // DMR_YENIDEN_DEGERLEME_BATCH_H
