#ifndef ISL_HESAP_OZETI_BATCH_CLASS_H
#define ISL_HESAP_OZETI_BATCH_CLASS_H

#include "batch_kernel.h"

namespace Ui {
    class ISL_HESAP_OZETI_BATCH;
}

class ISL_HESAP_OZETI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           ISL_HESAP_OZETI_BATCH                            ( QWidget * parent = 0 );
                           ~ISL_HESAP_OZETI_BATCH                           () {}
private:

    Ui::ISL_HESAP_OZETI_BATCH *        m_ui;

    int                                m_sube_id;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);

};

#endif // ISL_HESAP_OZETI_BATCH_CLASS_H
