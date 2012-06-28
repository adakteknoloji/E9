#ifndef STOK_DETAYLAR_BATCH_H
#define STOK_DETAYLAR_BATCH_H

#include "batch_kernel.h"

struct STOK_DETAYLAR_STRUCT;

namespace Ui {
    class STOK_DETAYLAR_BATCH;
}

class STOK_DETAYLAR_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           STOK_DETAYLAR_BATCH                            ( STOK_DETAYLAR_STRUCT * P_STOK_DETAYLARI ,QWidget * parent = 0 );
                           ~STOK_DETAYLAR_BATCH                           () {}

    bool          *         m_degisiklik_yapildi_mi;
    int                     m_muh_hesap_id;
private:

    Ui::STOK_DETAYLAR_BATCH *        m_ui;

    STOK_DETAYLAR_STRUCT * M_STOK_DETAYLARI;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();
    void                   SEARCH_EDIT_CLICKED                  ( QWidget *widget, QLineEdit *line_edit );

};

#endif // STOK_DETAYLAR_BATCH_H
