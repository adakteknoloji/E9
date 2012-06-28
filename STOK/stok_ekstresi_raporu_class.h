#ifndef STOK_EKSTRESI_RAPORU_H
#define STOK_EKSTRESI_RAPORU_H

#include "report_kernel.h"
#include <QDate>


enum STOK_ETKISI {
    ENUM_ARTTIR = 10,
    ENUM_AZALT  = 20,
};

class STOK_EKSTRESI_RAPORU : public REPORT_KERNEL
{


public:
    STOK_EKSTRESI_RAPORU                                          ( QString rapor_ismi,struct STOK_RAPOR_VARS *    M_STOK_RV );
    ~STOK_EKSTRESI_RAPORU                                         () {}

private:

    struct STOK_RAPOR_VARS *    M_STOK_RV;

    int                         m_max_line_count;
    int                         m_depo_id;
    int                         m_last_satir_indisi;
    int                         m_sayfaya_yazdirilan_satir_sayisi;

    bool                        m_tum_rapor_satirlari_basildi;
    bool                        m_nakli_yekun;
    bool                        m_urun_islemleri_basildi;

    QString                     m_rapor_kriterleri_1;
    QString                     m_rapor_kriterleri_2;

    QDate                       m_baslangic_tarihi;
    QDate                       m_bitis_tarihi;

    double                      m_footer_toplam_giren_miktar;
    double                      m_footer_toplam_cikan_miktar;
    double                      m_footer_kalan_miktar;
    double                      m_nakli_yekun_kalan_miktar;

    QStringList                 M_RAPOR_SATIRLARI;

    QString                     GET_HEADER              ();
    QString                     GET_FOOTER              ();
    QString                     GET_BODY                ();

    void                        FILL_RAPOR_SATIRLARI    ();
    void                        FILL_URUN_ISLEMLERI     (int urun_id , QString urun_kodu_adi);
    void                        PRINT_NAKLI_YEKUN       (int urun_id);


};

#endif // STOK_EKSTRESI_RAPORU_H
