#ifndef STOK_MIKTARLARI_BATCH_H
#define STOK_MIKTARLARI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class STOK_MIKTARLARI_BATCH;
}

class STOK_MIKTARLARI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           STOK_MIKTARLARI_BATCH                            ( QWidget * parent = 0 );
                           ~STOK_MIKTARLARI_BATCH                           () {}
private:
    Ui::STOK_MIKTARLARI_BATCH *     m_ui;

    int                             m_depo_id;
    int                             m_entegrasyon_durumu;
    QList<int>                      m_grup_idleri;

    void                            SETUP_FORM                           ();
    int                             CHECK_VAR                            ( QObject * object );
    int                             CHECK_RUN                            ();
    int                             CHECK_EXIT                           ();
    void                            RUN_BATCH                            ();

    void                            SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);

    void                            GRUP_FRAME_AYARLA                    ( bool p_state );
    void                            URUN_FRAME_AYARLA                    ( bool p_state );
};

#endif // STOK_MIKTARLARI_BATCH_H
