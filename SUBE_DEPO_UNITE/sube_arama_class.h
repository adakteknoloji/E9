#ifndef SUBE_ARAMA_H
#define SUBE_ARAMA_H

#include "arama_kernel.h"

namespace Ui {
    class SUBE_ARAMA;
}

class SUBE_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           SUBE_ARAMA             (  QString sube_adi_sube_kodu, QWidget * parent = 0 );
                           ~SUBE_ARAMA            (){}
    int *                  m_secilen_id;

private:

    QString                m_sube_adi_sube_kodu;

                           Ui::SUBE_ARAMA *       m_ui;

    void                   SETUP_FORM                                 ();
    int                    CHECK_VAR                                  (QObject * object);
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       ( int selected_row_number );

};

#endif // SUBE_ARAMA_H
