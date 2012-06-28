#ifndef FAT_HIZMET_EKSTRESI_BATCH_H
#define FAT_HIZMET_EKSTRESI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class FAT_HIZMET_EKSTRESI_BATCH;
}

class FAT_HIZMET_EKSTRESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           FAT_HIZMET_EKSTRESI_BATCH                            ( QWidget * parent = 0 );
                           ~FAT_HIZMET_EKSTRESI_BATCH                           () {}
private:

    Ui::FAT_HIZMET_EKSTRESI_BATCH *        m_ui;

    void    		       SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();
    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);

};

#endif // FAT_HIZMET_EKSTRESI_BATCH_H
