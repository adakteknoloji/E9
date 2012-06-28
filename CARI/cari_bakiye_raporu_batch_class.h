#ifndef CARI_BAKIYE_RAPORU_BATCH_H
#define CARI_BAKIYE_RAPORU_BATCH_H

#include "batch_kernel.h"


namespace Ui {
    class CARI_BAKIYE_RAPORU_BATCH;
}

class CARI_BAKIYE_RAPORU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

    public :
                           CARI_BAKIYE_RAPORU_BATCH                            ( int p_program_id, int p_modul_id, QWidget * parent = 0);
                           ~CARI_BAKIYE_RAPORU_BATCH                           () {}
    private :

    Ui::CARI_BAKIYE_RAPORU_BATCH *   m_ui;

    QList<int>             m_grup_idleri;

    int                    m_program_id;
    int                    m_modul_id;

    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

    void                   SEARCH_EDIT_CLICKED                  ( QWidget * p_widget, QLineEdit * p_line_edit );

};

#endif // CARI_BAKIYE_RAPORU_BATCH_H
