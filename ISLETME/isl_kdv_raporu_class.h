#ifndef ISL_ISL_KDV_RAPORU_CLASS_H
#define ISL_ISL_KDV_RAPORU_CLASS_H

#include "report_kernel.h"

struct ISL_RAPOR_VARS;

class ISL_KDV_RAPORU : public REPORT_KERNEL
{


public:
    ISL_KDV_RAPORU(ISL_RAPOR_VARS * P_ISL_RV);
    ~ISL_KDV_RAPORU() {}

protected:

   QString          GET_HEADER                              ();
   QString          GET_FOOTER                              ();
   QString          GET_BODY                                ();


private:

    ISL_RAPOR_VARS * M_ISL_RV;

    int             m_max_line_count;
    bool            m_rapor_bitti_mi;
    bool            m_kayit_var_mi;
    int             m_gelir_fisi_kdv_orani_sayisi;
    int             m_gider_fisi_kdv_orani_sayisi;
    double          m_gider_fisi_kdv_haric_toplam;
    double          m_gider_fisi_kdv_tutari_toplam;
    double          m_gider_fisi_kdv_dahil_toplam;
    double          m_gelir_fisi_kdv_haric_toplam;
    double          m_gelir_fisi_kdv_tutari_toplam;
    double          m_gelir_fisi_kdv_dahil_toplam;
    QList  <int>    M_GELIR_FISI_KDV_LISTESI;
    QList  <int>    M_GIDER_FISI_KDV_LISTESI;

    struct TUTAR_TOPLAMLARI {
        int    kdv_orani;
        double kdv_haric_tutar;
        double kdv_tutari;
        double kdv_dahil_tutar;
    };

    TUTAR_TOPLAMLARI  * M_GIDER_FISI_TUTARLAR_LISTESI;
    TUTAR_TOPLAMLARI  * M_GELIR_FISI_TUTARLAR_LISTESI;

    bool            FIS_SATIRLARINI_KDV_ORANLARINA_GORE_AYIR ();

};

#endif // ISL_ISL_KDV_RAPORU_H
