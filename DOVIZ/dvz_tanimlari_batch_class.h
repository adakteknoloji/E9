#ifndef DVZ_TANIMLARI_BATCH_H
#define DVZ_TANIMLARI_BATCH_H

#include <QtGui/QDialog>
#include "QCommaEdit.h"
#include "batch_kernel.h"

namespace Ui {
    class DVZ_TANIMLARI_BATCH;
}

class DVZ_TANIMLARI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::DVZ_TANIMLARI_BATCH *        m_ui;

    QStringList                            doviz_kodlari;
    QList<QCommaEdit *>                    comma_edit_alis_yuzdesi;
    QList<QCommaEdit *>                    comma_edit_satis_yuzdesi;
    QList<QCheckBox  *>                    m_check_box_dovizi_kullan;
    QList<int         >                    m_dovizi_kullan_default_values;

public:
                                           DVZ_TANIMLARI_BATCH       ( QWidget * parent = 0 );
                                           ~DVZ_TANIMLARI_BATCH      () {}
protected:
    void                                   SETUP_FORM                      ();

    int                                    CHECK_VAR                       ( QObject * object );
    int                                    CHECK_RUN                       ();
    void                                   RUN_BATCH                       ();

};


#endif // DVZ_TANIMLARI_BATCH_H
