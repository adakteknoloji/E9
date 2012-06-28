#ifndef XYZ_ARAMA_CLASS_H
#define XYZ_ARAMA_CLASS_H

#include "arama_kernel.h"

namespace Ui {
    class XYZ_ARAMA;
}

class XYZ_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           XYZ_ARAMA             ( int xxxx, QWidget * parent = 0 );
                           ~XYZ_ARAMA            () {}

                           int           *       m_secilen_id;
private:
    Ui::XYZ_ARAMA *        m_ui;
    
    void   		   SETUP_FORM                          	      ();
    int                    CHECK_VAR                                  ( QObject * object );
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       ( int selected_row_number );
    void                   SELECTED_LINE                              ( int selected_row_number );



};

#endif // XYZ_ARAMA_CLASS_H
