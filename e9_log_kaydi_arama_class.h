#ifndef E9_LOG_KAYDI_ARAMA_H
#define E9_LOG_KAYDI_ARAMA_H

#include "arama_kernel.h"

namespace Ui {
    class E9_LOG_KAYDI_ARAMA;
}

class E9_LOG_KAYDI_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                                    E9_LOG_KAYDI_ARAMA                         ( QWidget * p_parent = 0 );
                                    ~E9_LOG_KAYDI_ARAMA                        () {}
private:

    Ui::E9_LOG_KAYDI_ARAMA *        m_ui;

    int                             m_kullanici_id;

    void                            SETUP_FORM                                 ();
    int                             CHECK_VAR                                  ( QObject * p_object );
    int                             CHECK_RUN                                  ();
    void                            SEARCH                                     ();
    int                             SINGLE_LINE_SELECTED                       ( int p_selected_row_number );
    void                            SELECTED_LINE                              ( int p_selected_row_number );

    void                            SEARCH_EDIT_CLICKED                        ( QWidget * p_widget, QLineEdit * p_line_edit );

    void                            FILL_LOG_TURLERI_COMBOBOX                  ();



};

#endif // E9_LOG_KAYDI_ARAMA_H
