#ifndef BNK_BANKA_DEFTERI_RAPORU_CLASS_H
#define BNK_BANKA_DEFTERI_RAPORU_CLASS_H

#include "report_kernel.h"

struct BNK_RAPOR_VARS;

class BNK_BANKA_DEFTERI_RAPORU : public REPORT_KERNEL
{


public:
        BNK_BANKA_DEFTERI_RAPORU( BNK_RAPOR_VARS * BNK_RAPOR );
    ~BNK_BANKA_DEFTERI_RAPORU() {  }


private:

    BNK_RAPOR_VARS              * BNK_RV ;
    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    QStringList                 m_fis_satirlari_list;
    int                         m_last_satir_indisi;
    int                         m_tum_satirlar_basildi;

    double                      m_footer_toplam_borc;
    double                      m_footer_toplam_alacak;

    QString                     GET_HEADER                      ();
    QString                     GET_FOOTER                      ();
    QString                     GET_BODY                        ();

    QStringList                 GET_BANKA_ISLEMLERI             ();

    void                        PRINT_NAKLI_YEKUN               ();
};

#endif // BNK_BANKA_DEFTERI_RAPORU_H
