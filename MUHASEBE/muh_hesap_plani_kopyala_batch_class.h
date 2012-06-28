#ifndef MUH_HESAP_PLANI_KOPYALA_BATCH_H
#define MUH_HESAP_PLANI_KOPYALA_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class MUH_HESAP_PLANI_KOPYALA_BATCH;
}

class MUH_HESAP_PLANI_KOPYALA_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           MUH_HESAP_PLANI_KOPYALA_BATCH                            ( QWidget * parent = 0 );
                           ~MUH_HESAP_PLANI_KOPYALA_BATCH                           () {}
private:

    Ui::MUH_HESAP_PLANI_KOPYALA_BATCH *        m_ui;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();
    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);

};

#endif // MUH_HESAP_PLANI_KOPYALA_BATCH_H
