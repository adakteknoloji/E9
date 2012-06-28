#ifndef BNK_POS_EKSTRESI_CLASS_H
#define BNK_POS_EKSTRESI_CLASS_H

#include "report_kernel.h"

struct BNK_RAPOR_VARS;

class BNK_POS_EKSTRESI : public REPORT_KERNEL
{


public:
    BNK_POS_EKSTRESI( BNK_RAPOR_VARS * BNK_RAPOR );
    ~BNK_POS_EKSTRESI() {}

protected:
    virtual QString             GET_HEADER                      ();
    virtual QString             GET_FOOTER                      ();
    virtual QString             GET_BODY                        ();

private:
    BNK_RAPOR_VARS *            BNK_RV;
    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    int                         m_tum_satirlar_basildi;
    QStringList                 m_pos_islemleri_list;
    int                         m_last_rapor_satiri_indisi;

    double                      m_footer_toplam_borc;
    double                      m_footer_toplam_alacak;

    QStringList                 GET_POS_ISLEMLERI();

    void                        PRINT_NAKLI_YEKUN();
};

#endif // BNK_POS_EKSTRESI_H
