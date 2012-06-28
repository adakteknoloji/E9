#ifndef MUH_HESAP_ARAMA_H
#define MUH_HESAP_ARAMA_H

#include <QtGui/QDialog>
#include "arama_kernel.h"



namespace Ui {
    class MUH_HESAP_ARAMA;
}

class MUH_HESAP_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           MUH_HESAP_ARAMA                            ( QString hesap_kodu_baslangici, int show_add_button, QWidget * parent = 0);
                           ~MUH_HESAP_ARAMA                           () {}

    int *                   m_secilen_id;
private:

    Ui::MUH_HESAP_ARAMA *      m_ui;

    QString                m_hesap_kodu_baslangici;
    int                    m_show_add_button;

    void                   SETUP_FORM                                 ();
    int                    CHECK_VAR                                  (QObject * object);
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       (int selected_row_number);


};

#endif // MUH_HESAP_ARAMA_H
