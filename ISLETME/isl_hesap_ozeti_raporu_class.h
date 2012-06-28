#ifndef ISL_HESAP_OZETI_RAPORU_CLASS_H
#define ISL_HESAP_OZETI_RAPORU_CLASS_H

#include "report_kernel.h"

struct ISL_RAPOR_VARS;

class ISL_HESAP_OZETI_RAPORU : public REPORT_KERNEL
{


public:
    ISL_HESAP_OZETI_RAPORU  ( QString rapor_ismi ,ISL_RAPOR_VARS * P_ISL_RV);
    ~ISL_HESAP_OZETI_RAPORU () {}

private:

    int  m_max_line_count;
    bool m_rapor_yazdirildi_mi;

    ISL_RAPOR_VARS *M_ISL_RV;

    double m_genel_gider_tutari;
    double m_gider_mal_alimi_tutari;
    double m_gider_donem_basi_stogu_tutari;
    double m_gider_isyeri_kirasi_tutari;
    double m_gider_isyeri_sigorta_gideri_tutari;
    double m_gider_isyeri_personel_gideri_tutari;
    double m_gider_isyeri_faiz_gideri_tutari;
    double m_gider_mal_alis_gideri_tutari;
    double m_gider_yil_sonu_amortismanlar_tutari;
    double m_gider_indirilecek_kdv_tutari;
    double m_toplam_gider;

    double m_gelir_mal_satisi_tutari;
    double m_gelir_hizmet_satisi_tutari;
    double m_gelir_amortisman_satisi_tutari;
    double m_gelir_faiz_geliri_tutari;
    double m_gelir_yil_sonu_stogu_tutari;
    double m_gelir_hesaplanan_kdv_tutari;
    double m_toplam_gelir;


    QString GET_HEADER ();
    QString GET_FOOTER ();
    QString GET_BODY   ();

    void    GET_TUTARLAR();

};

#endif // ISL_HESAP_OZETI_RAPORU_CLASS_H
