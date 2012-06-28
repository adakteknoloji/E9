#ifndef BNK_HESAPLARI_ARAMA_CLASS_H
#define BNK_HESAPLARI_ARAMA_CLASS_H

#include "arama_kernel.h"

namespace Ui {
    class BNK_HESAPLARI_ARAMA;
}

class BNK_HESAPLARI_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                                   BNK_HESAPLARI_ARAMA             (  QWidget * parent = 0 );
                                   ~BNK_HESAPLARI_ARAMA            (){}
    int *                          m_secilen_id;
private:
    Ui::BNK_HESAPLARI_ARAMA *      m_ui;

    void                           SETUP_FORM                      ();
    int                            CHECK_VAR                       (QObject * object);
    int                            CHECK_RUN                       ();
    void                           SEARCH                          ();
    int                            SINGLE_LINE_SELECTED            (int selected_row_number);

};

#endif // CSB_CAR_BANKA_HESAPLARI_ARAMA_H
