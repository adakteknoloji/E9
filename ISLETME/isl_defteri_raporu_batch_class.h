#ifndef ISL_DEFTERI_RAPORU_BATCH_CLASS_H
#define ISL_DEFTERI_RAPORU_BATCH_CLASS_H

#include <QtGui/QDialog>
#include "batch_kernel.h"




namespace Ui {
    class ISL_DEFTERI_RAPORU_BATCH;
}

class ISL_DEFTERI_RAPORU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::ISL_DEFTERI_RAPORU_BATCH *   m_ui;

public:
                           ISL_DEFTERI_RAPORU_BATCH             (QWidget * parent = 0);
                           ~ISL_DEFTERI_RAPORU_BATCH            () {}
private:


    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

    int                    GET_ISL_DEFTERI_RAPOR_SEKLI_ENUM     (QString rapor_sekli_str);

};

#endif // ISL_DEFTERI_RAPORU_BATCH_H
