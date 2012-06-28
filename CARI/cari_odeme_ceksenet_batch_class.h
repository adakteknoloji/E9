#ifndef CARI_ODEME_CEKSENET_BATCH_H
#define CARI_ODEME_CEKSENET_BATCH_H

#include <QtGui/QDialog>
 
#include "batch_kernel.h"

namespace Ui {
    class CARI_ODEME_CEKSENET_BATCH;
}

class CARI_ODEME_CEKSENET_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           CARI_ODEME_CEKSENET_BATCH              (int fis_satiri_id,int cek_senet_id,int islem_turu,QWidget * parent = 0);
                           ~CARI_ODEME_CEKSENET_BATCH             () {}

    bool             *     m_kayit_degisti_mi;
protected:
    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

private:
    int                    m_islem_turu;
    int                    m_fis_satiri_id;
    int                    m_cek_senet_id;
    int                    m_cek_senet_turu;
    int                    m_cek_senet_durumu;

    void                   CEK_SENET_BILGILERINI_GOSTER();

    Ui::CARI_ODEME_CEKSENET_BATCH *   m_ui;
};

#endif // CARI_ODEME_CEKSENET_BATCH_H
