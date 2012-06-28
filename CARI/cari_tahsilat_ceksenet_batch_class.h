#ifndef CARI_TAHSILAT_CEKSENET_BATCH_H
#define CARI_TAHSILAT_CEKSENET_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"

struct E9_ISLEM_DETAYLARI;

namespace Ui {
    class CARI_TAHSILAT_CEKSENET_BATCH;
}

class CARI_TAHSILAT_CEKSENET_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
     CARI_TAHSILAT_CEKSENET_BATCH( int fis_satiri_id, int islem_turu, E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI ,
                                   bool islem_turu_degisti, QWidget * parent = 0 );

     ~CARI_TAHSILAT_CEKSENET_BATCH() {}
      bool  * m_kayit_degistirildi_mi;

private:

    E9_ISLEM_DETAYLARI * M_ISLEM_DETAYLARI;

    void SETUP_FORM();
    int  CHECK_VAR(QObject * object);
    int  CHECK_RUN();
    int  CHECK_EXIT();
    void RUN_BATCH();


    int  m_islem_turu;
    int  m_islem_turu_degisti;

    int  m_cek_senet_id;
    int  m_fis_satiri_id;

    Ui::CARI_TAHSILAT_CEKSENET_BATCH *   m_ui;
};

#endif // CARI_TAHSILAT_CEKSENET_BATCH_H
