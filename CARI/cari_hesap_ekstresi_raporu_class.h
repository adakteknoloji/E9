#ifndef CARI_HESAP_EKSTRESI_RAPORU_H
#define CARI_HESAP_EKSTRESI_RAPORU_H

#include "report_kernel.h"

struct CARI_RAPOR_VARS;

class CARI_HESAP_EKSTRESI_RAPORU : public REPORT_KERNEL
{


public:
    CARI_HESAP_EKSTRESI_RAPORU( CARI_RAPOR_VARS * P_CARI_RV);
    ~CARI_HESAP_EKSTRESI_RAPORU() {}


private:

    CARI_RAPOR_VARS   *         M_CARI_RV;

    int                         m_max_line_count;
    int                         m_gonderilen_kayit_sayisi;
    int                         m_last_satir_indisi;
    QStringList                 m_cari_hesaplar_list;
    int                         m_last_hesap_indisi;
    int                         m_satirlar_basildi;

    double                      m_footer_toplam_borc;
    double                      m_footer_toplam_alacak;

    double                      m_nakli_yekun_borc;
    double                      m_nakli_yekun_alacak;

    QStringList                 m_fis_satirlari;    

    QString                     GET_HEADER                          ();
    QString                     GET_FOOTER                          ();
    QString                     GET_BODY                            ();


    QStringList                 GET_CARI_HESAPLAR_LIST              ();
    QStringList                 GET_CARI_HESAP_ISLEMLERI            ( int cari_hesap_id );
    void                        PRINT_NAKLI_YEKUN                   ( double toplam_borc, double toplam_alacak );
};

#endif // CARI_HESAP_EKSTRESI_RAPORU_H
