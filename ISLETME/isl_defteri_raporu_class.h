#ifndef ISL_DEFTERI_RAPORU_CLASS_H
#define ISL_DEFTERI_RAPORU_CLASS_H

#include "report_kernel.h"

struct ISL_RAPOR_VARS;

class ISL_DEFTERI_RAPORU : public REPORT_KERNEL
{

public:
    ISL_DEFTERI_RAPORU( ISL_RAPOR_VARS * P_ISL_RV);
    ~ISL_DEFTERI_RAPORU() {}

protected:
    QString                     GET_HEADER                         ();
    QString                     GET_FOOTER                         ();
    QString                     GET_BODY                           ();

private:

    ISL_RAPOR_VARS *           M_ISL_RV;

    int                         m_max_line_count;
    int                         m_gider_fisi_index;
    int                         m_gelir_fisi_index;
    int                         m_sayfa_numarasi;
    bool                        m_gider_fisi_satiri_var_mi;
    bool                        m_gelir_fisi_satiri_var_mi;
    bool                        m_devreden_toplam_var_mi;
    bool                        m_rapor_bitti_mi;
    double                      m_gider_kdv_haric_toplam;
    double                      m_gider_kdv_tutar_toplam;
    double                      m_gider_toplam;
    double                      m_gelir_kdv_haric_toplam;
    double                      m_gelir_kdv_tutar_toplam;
    double                      m_gelir_toplam;

    bool                        m_gelirler_yazdirildi;
    bool                        m_giderler_yazdirildi;

    QStringList                 M_GIDER_FIS_SATIRLARI;
    QStringList                 M_GELIR_FIS_SATIRLARI;

    bool                        FIS_SATIRLARINI_AL                  ();
    bool                        DEVREDEN_TOPLAM_HESAPLA             ();

};

#endif // ISL_DEFTERI_RAPORU_H
