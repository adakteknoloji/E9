#ifndef DMR_FIS_ARAMA_H
#define DMR_FIS_ARAMA_H

#include <QtGui/QDialog>
#include "arama_kernel.h"

namespace Ui {
    class DMR_FIS_ARAMA;
}

class DMR_FIS_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           DMR_FIS_ARAMA             ( int fis_turu, QWidget * parent = 0 );
                           ~DMR_FIS_ARAMA            (){}

    int         *          m_secilen_id;
private:
    Ui::DMR_FIS_ARAMA *        m_ui;

    int                    m_fis_turu;

    void                   SETUP_FORM                                 ();

    int                    CHECK_VAR                                  (QObject * object);
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       (int selected_row_number);


};

#endif // DMR_FIS_ARAMA_H
