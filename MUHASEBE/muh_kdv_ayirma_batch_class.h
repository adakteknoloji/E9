#ifndef MUH_KDV_AYIRMA_BATCH_H
#define MUH_KDV_AYIRMA_BATCH_H


#include "batch_kernel.h"
namespace Ui {
    class MUH_KDV_AYIRMA_BATCH;
}

class MUH_KDV_AYIRMA_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::MUH_KDV_AYIRMA_BATCH *   m_ui;

    int                          m_kdv_hesap_id;

public:
                              MUH_KDV_AYIRMA_BATCH                              ( int       p_fis_satiri_id,
                                                                                  double    p_kdv_dahil_tutar ,
                                                                                  QString   p_aciklama ,
                                                                                  QWidget * p_parent );

                              ~MUH_KDV_AYIRMA_BATCH                             () {}

    bool *                     m_accepted;
    struct                     MUH_KDV_AYIRMA_STRUCT   *                        M_KDV_AYIRMA_STRUCT;

private:
    int                        m_fis_satiri_id;
    double                     m_kdv_dahil_tutar;
    QString                    m_aciklama;

    void                       SETUP_FORM                                       ();
    int                        CHECK_VAR                                        ( QObject * p_object );
    int                        CHECK_RUN                                        ();
    void                       RUN_BATCH                                        ();

    void                       INDIRILECEK_KDV_HESAPLARI_COMBOBOX_DOLDUR        ();
    void                       HESAPLANAN_KDV_HESAPLARI_COMBOBOX_DOLDUR         ();

    void                       KDV_HESAPLA                                      ( double p_kdv_orani );

    void                       TALI_KDV_HESAPLARINI_BUL                         ( int           p_parent_id,
                                                                                QStringList & P_KDV_HESDAP_LISTESI );

private slots:

    void                       FIND_KDV                                         ( double p_kdv_orani );


};

#endif // MUH_KDV_AYIRMA_BATCH_H
