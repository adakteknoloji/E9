#ifndef E9_ENT_DETAYLARI_BATCH_H
#define E9_ENT_DETAYLARI_BATCH_H

#include "batch_kernel.h"

struct E9_ENT_DETAYLARI_STRUCT;

namespace Ui {
    class E9_ENT_DETAYLARI_BATCH;
}
class QLineEdit;

class E9_ENT_DETAYLARI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           E9_ENT_DETAYLARI_BATCH                            ( E9_ENT_DETAYLARI_STRUCT * P_ENT_DETAYLARI, bool kasa_hesabi_gosterilecek ,QWidget * parent = 0 );
                           ~E9_ENT_DETAYLARI_BATCH                           () {}
private:

    Ui::E9_ENT_DETAYLARI_BATCH *        m_ui;

    bool                       m_kasa_hesabi_gosterilecek;

    E9_ENT_DETAYLARI_STRUCT * M_ENT_DETAYLARI;

    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);

};

#endif // E9_ENT_DETAYLARI_BATCH_H
