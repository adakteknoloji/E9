#ifndef SMM_MAKBUZ_ARAMA_H
#define SMM_MAKBUZ_ARAMA_H

#include "arama_kernel.h"

namespace Ui {
    class SMM_MAKBUZ_ARAMA;
}

class SMM_MAKBUZ_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           SMM_MAKBUZ_ARAMA             ( int p_fis_turu ,QWidget * parent = 0 );
                           ~SMM_MAKBUZ_ARAMA            () {}

                           int           *        m_secilen_id;
private:
    Ui::SMM_MAKBUZ_ARAMA *        m_ui;

    int                    m_makbuz_turu;
    
    void   		   SETUP_FORM                          	      ();
    int                    CHECK_VAR                                  (QObject * object);
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       (int selected_row_number);

};

#endif // SMM_MAKBUZ_ARAMA_H
