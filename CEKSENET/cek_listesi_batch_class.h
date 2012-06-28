#ifndef CEK_LISTESI_BATCH_H
#define CEK_LISTESI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class CEK_LISTESI_BATCH;
}

class CEK_LISTESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public :
                           CEK_LISTESI_BATCH                            (QWidget * parent = 0);
                           ~CEK_LISTESI_BATCH                           () {}
private :

   Ui::CEK_LISTESI_BATCH *   m_ui;

    int                      m_alinan_cari_hesap_id;
    int                      m_verilen_cari_hesap_id;

    void                     SETUP_FORM                           ();
    int                      CHECK_VAR                            ( QObject * p_object );
    int                      CHECK_RUN                            ();
    void                     RUN_BATCH                            ();
    void                     FILL_COMBOBOX_CEK_SENET_POZISYONU    ();
    void                     FILL_COMBOBOX_CEK_SENET_TURU         ( int cek_senet_pozisyonu );
    void                     SEARCH_EDIT_CLICKED                  ( QWidget * p_widget, QLineEdit * p_lineEdit );

};

#endif // CEK_LISTESI_BATCH_H
