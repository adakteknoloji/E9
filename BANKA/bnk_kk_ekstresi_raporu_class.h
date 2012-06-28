#ifndef BNK_KK_EKSTRESI_RAPORU_CLASS_H
#define BNK_KK_EKSTRESI_RAPORU_CLASS_H

#include "report_kernel.h"

struct BNK_RAPOR_VARS;

class BNK_KK_EKSTRESI_RAPORU : public REPORT_KERNEL
{


public:
    BNK_KK_EKSTRESI_RAPORU                                          ( BNK_RAPOR_VARS *, QString baslik );
    ~BNK_KK_EKSTRESI_RAPORU                                         () {}

protected:
    virtual QString             GET_HEADER              ();
    virtual QString             GET_FOOTER              ();
    virtual QString             GET_BODY                ();

private:
    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    int                         m_tum_satirlar_basildi;
    int                         m_last_islem_indisi;

    double                      m_footer_toplam_borc;
    double                      m_footer_toplam_alacak;

    BNK_RAPOR_VARS              * BNK_RV ;

    QStringList                 GET_KREDI_KARTI_ISLEMLERI ();
    void                        PRINT_NAKLI_YEKUN();
};

#endif // BNK_KK_EKSTRESI_RAPORU_H
