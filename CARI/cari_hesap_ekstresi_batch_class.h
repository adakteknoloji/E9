#ifndef CARI_HESAP_EKSTRESI_BATCH_H
#define CARI_HESAP_EKSTRESI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class CARI_HESAP_EKSTRESI_BATCH;
}

class CARI_HESAP_EKSTRESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           CARI_HESAP_EKSTRESI_BATCH                        ( int p_program_id, int p_modul_id, int cari_hesap_id  , QWidget * p_parent = 0 );
                           ~CARI_HESAP_EKSTRESI_BATCH                       () {}
private :

    Ui::CARI_HESAP_EKSTRESI_BATCH   *       m_ui;
    int                                     m_cari_hesap_id;

    int                                     m_program_id;
    int                                     m_modul_id;

    void                                    SETUP_FORM                           ();
    int                                     CHECK_VAR                            ( QObject * p_object );
    int                                     CHECK_RUN                            ();
    void                                    RUN_BATCH                            ();
    void                                    SEARCH_EDIT_CLICKED                  ( QWidget * p_widget, QLineEdit * p_line_edit );

};

#endif // CARI_HESAP_EKSTRESI_BATCH_H
