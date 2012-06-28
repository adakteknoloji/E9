#ifndef STOK_GELEN_SERI_NO_BATCH_CLASS_H
#define STOK_GELEN_SERI_NO_BATCH_CLASS_H

#include "batch_kernel.h"

namespace Ui {
    class STOK_GELEN_SERI_NO_BATCH;
}

class STOK_GELEN_SERI_NO_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
    STOK_GELEN_SERI_NO_BATCH( int p_depo_id, int p_urun_id, QStringList *p_seri_no_list, QWidget * parent = 0 );
    ~STOK_GELEN_SERI_NO_BATCH() {}
private:

    Ui::STOK_GELEN_SERI_NO_BATCH *        m_ui;

    QString     m_urun_adi;
    QStringList *m_seri_no_list;

    int         m_depo_id;
    int         m_urun_id;

    void    	SETUP_FORM();
    int         CHECK_VAR( QObject * object );
    int         CHECK_RUN();
    int         CHECK_EXIT();
    void        RUN_BATCH();

    void        YENI_BOS_SATIR_EKLE();
    void        SERI_NO_EKLE();

    bool        VERI_TABANI_KONTROLU( QString p_seri_no );

public slots:
    void        SLOT_BUTTON_SIL();

};

#endif // STOK_GELEN_SERI_NO_BATCH_CLASS_H
