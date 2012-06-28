#ifndef MUH_MIZAN_RAPORU_BATCH_H
#define MUH_MIZAN_RAPORU_BATCH_H

#include "batch_kernel.h"

class QAdakDate;
class QComboBox;

namespace Ui {
    class MUH_MIZAN_RAPORU_BATCH;
}

class MUH_MIZAN_RAPORU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public :
                           MUH_MIZAN_RAPORU_BATCH       ( QWidget * p_parent = 0 );
                           ~MUH_MIZAN_RAPORU_BATCH      (){}
private :
    QWidget *              m_parent;

    Ui::MUH_MIZAN_RAPORU_BATCH *   m_ui;

    QList<int>             m_grup_idleri;

    QString                m_current_mizan_donemi;

    void                   SETUP_FORM                   ();

    int                    CHECK_VAR                    ( QObject * object );
    int                    CHECK_RUN                    ();
    void                   RUN_BATCH                    ();
    void                   SEARCH_EDIT_CLICKED          ( QWidget * p_widget, QLineEdit * p_lineEdit );

    void                   GET_MZN_DONEMINE_GORE_BAS_BTS_TARIHI ( QString mizan_donemi, QAdakDate * bas_tarihi ,QAdakDate * bts_tarihi);

    QString                GET_MZN_DONEMI_AY_STR ( int month );

    void                   FILL_COMBOBOX_MIZAN_DONEMI ( QComboBox * combobox_mizan_donemi);
};

#endif // MUH_MIZAN_RAPORU_BATCH_H
