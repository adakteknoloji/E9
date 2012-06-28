#ifndef ISL_FIS_KOPYALAMA_BATCH_CLASS_H
#define ISL_FIS_KOPYALAMA_BATCH_CLASS_H

#include <QtGui/QDialog>
#include "batch_kernel.h"

class QMdiArea;

namespace Ui {
    class ISL_FIS_KOPYALAMA_BATCH;
}

class ISL_FIS_KOPYALAMA_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                                       ISL_FIS_KOPYALAMA_BATCH              ( int fis_id, QWidget * parent = 0 );
                                       ~ISL_FIS_KOPYALAMA_BATCH             () {}

private:

       Ui::ISL_FIS_KOPYALAMA_BATCH *   m_ui;

       QWidget           *             m_parent;
       int                             m_fis_id;
       int                             m_fis_turu;

       void                            SETUP_FORM                           ();
       int                             CHECK_VAR                            ( QObject * object );
       int                             CHECK_RUN                            ();
       void                            RUN_BATCH                            ();
       void                            SEARCH_EDIT_CLICKED                  ( QWidget     * widget,
                                                                              QLineEdit   * lineEdit);

};

#endif // ISL_FIS_KOPYALAMA_BATCH_H
