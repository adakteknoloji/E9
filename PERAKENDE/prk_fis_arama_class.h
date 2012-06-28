#ifndef PRK_FIS_ARAMA_CLASS_H
#define PRK_FIS_ARAMA_CLASS_H

#include "arama_kernel.h"

namespace Ui {
    class PRK_FIS_ARAMA;
}

class PRK_FIS_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:


    PRK_FIS_ARAMA             ( int p_gider_fisi_mi , QWidget * parent = 0 );
    ~PRK_FIS_ARAMA            () {}

    int           *        m_secilen_id;


private:


    Ui::PRK_FIS_ARAMA *        m_ui;

    int                    m_gider_fisi_mi;

    void                   SETUP_FORM                                 ();
    int                    CHECK_VAR                                  (QObject * object);
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       (int selected_row_number);
    void                   SELECTED_LINE                              ( int selected_row_number );



};

#endif // PRK_FIS_ARAMA_H
