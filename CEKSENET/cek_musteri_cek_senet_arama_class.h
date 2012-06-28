#ifndef CEK_MUSTERI_CEK_SENET_ARAMA_H
#define CEK_MUSTERI_CEK_SENET_ARAMA_H

#include "arama_kernel.h"

namespace Ui {
    class CEK_MUSTERI_CEK_SENET_ARAMA;
}

class CEK_MUSTERI_CEK_SENET_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           CEK_MUSTERI_CEK_SENET_ARAMA         ( int cek_senet_turu,QWidget * parent = 0);
                           ~CEK_MUSTERI_CEK_SENET_ARAMA         () {}

private:

    Ui::CEK_MUSTERI_CEK_SENET_ARAMA *   m_ui;

    int                    m_cek_senet_turu;
    int                    m_cari_hesap_id;

    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   SEARCH                               ();
    int                    SINGLE_LINE_SELECTED                 ( int selected_row_number );
    void                   FILL_COMBOBOX_CEKIN_DURUMU();
    void                   SEARCH_EDIT_CLICKED                  ( QWidget * widget, QLineEdit * lineEdit);


};

#endif // CEK_MUSTERI_CEK_SENET_ARAMA_H
