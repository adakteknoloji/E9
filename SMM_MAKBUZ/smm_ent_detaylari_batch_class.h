#ifndef SMM_ENT_DETAYLARI_BATCH_H
#define SMM_ENT_DETAYLARI_BATCH_H

#include "batch_kernel.h"

struct SMM_ENT_DETAYLARI;

namespace Ui {
    class SMM_ENT_DETAYLARI_BATCH;
}

class SMM_ENT_DETAYLARI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           SMM_ENT_DETAYLARI_BATCH                            ( SMM_ENT_DETAYLARI * P_ENT_DETAYLARI ,
                                                                                      QWidget * parent = 0 );
                           ~SMM_ENT_DETAYLARI_BATCH                           () {}
private:

    Ui::SMM_ENT_DETAYLARI_BATCH *        m_ui;

    SMM_ENT_DETAYLARI * M_ENT_DETAYLARI;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

    void                   SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit);

};

#endif // SMM_ENT_DETAYLARI_BATCH_H
