#ifndef MUH_YEVMIYE_NO_OLUSTUR_BATCH_H
#define MUH_YEVMIYE_NO_OLUSTUR_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"



namespace Ui {
    class MUH_YEVMIYE_NO_OLUSTUR_BATCH;
}

class MUH_YEVMIYE_NO_OLUSTUR_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           MUH_YEVMIYE_NO_OLUSTUR_BATCH                            (QWidget *parent = 0);
                           ~MUH_YEVMIYE_NO_OLUSTUR_BATCH                           (){}
private:
    Ui::MUH_YEVMIYE_NO_OLUSTUR_BATCH *   m_ui;
    int muhasebe_yili;
    int son_yevmiye_numarasi;

    QDate                  m_bas_date;

    void                   SETUP_FORM                                    ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

    int                    ACILIS_FISINE_NUMARA_VER             ();
    void                   KAPANIS_FISINE_NUMARA_VER            ();
    QDate                  SON_YEVMIYE_TARIHI                   ();


};


#endif // MUH_YEVMIYE_NO_OLUSTUR_BATCH_H
