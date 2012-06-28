#ifndef MUH_YEVMIYE_DOKUMU_BATCH_H
#define MUH_YEVMIYE_DOKUMU_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"

namespace Ui {
    class MUH_YEVMIYE_DOKUMU_BATCH;
}

class MUH_YEVMIYE_DOKUMU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                                MUH_YEVMIYE_DOKUMU_BATCH                ( QWidget * parent = 0 );
                                ~MUH_YEVMIYE_DOKUMU_BATCH               (){}
private:
    Ui::MUH_YEVMIYE_DOKUMU_BATCH *  m_ui;

    void                        SETUP_FORM                          ();
    int                         CHECK_VAR                           ( QObject * object );
    int                         CHECK_RUN                           ();
    void                        RUN_BATCH                           ();

    void                        SEARCH_EDIT_CLICKED                 (QWidget *, QLineEdit *);

    void                        DATE_FRAME_AYARLA                   (bool default_degerleri_ata = true );
    void                        YEVMIYE_NO_FRAME_AYARLA             (bool default_degerleri_ata = true );

};

#endif // MUH_YEVMIYE_DOKUMU_BATCH_H
