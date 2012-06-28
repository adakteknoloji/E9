#ifndef DVZ_DOVIZLER_BATCH_H
#define DVZ_DOVIZLER_BATCH_H

#include <QGridLayout>
#include "batch_kernel.h"

namespace Ui {
    class DVZ_DOVIZLER_BATCH;
}

class DVZ_DOVIZLER_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::DVZ_DOVIZLER_BATCH *        m_ui;

    QStringList            doviz_kodlari;
    QList<QCommaEdit *>    comma_edit_alis_degerleri;
    QList<QCommaEdit *>    comma_edit_satis_degerleri;

    QList<QLabel *>        label_alis;
    QList<QLabel *>        label_satis;
    QList<QLabel *>        label_efektif_alis;
    QList<QLabel *>        label_efektif_satis;

    bool                   DOVIZLERI_TARIHE_GORE_GOSTER        ();

    void                   CREATE_SCREEN                       ();

    QHBoxLayout *          m_main_layout;
    QFrame *               m_last_created_frame;

public:
                           DVZ_DOVIZLER_BATCH                  ( QWidget * parent = 0 );
                           ~DVZ_DOVIZLER_BATCH                 () {}
protected:
    void                   SETUP_FORM                          ();

    int                    CHECK_VAR                           ( QObject * object );
    int                    CHECK_RUN                           ();
    void                   RUN_BATCH                           ();

};

#endif // DVZ_DOVIZLER_BATCH_H
