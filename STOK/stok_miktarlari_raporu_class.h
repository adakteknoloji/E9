#ifndef STOK_MIKTARLARI_RAPORU_H
#define STOK_MIKTARLARI_RAPORU_H

#include "report_kernel.h"

class STOK_MIKTARLARI_RAPORU : public REPORT_KERNEL
{


public:
    STOK_MIKTARLARI_RAPORU                                          ( QString baslik,struct STOK_RAPOR_VARS *    M_STOK_RV );
    ~STOK_MIKTARLARI_RAPORU                                         () {}

private:
    struct STOK_RAPOR_VARS *    M_STOK_RV;

    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    bool                        m_tum_kayitlar_basildi;
    int                         m_last_urun_indisi;
    double                         m_footer_depodaki_miktar;
    QString                     m_rapor_kriterleri_1;
    QString                     m_rapor_kriterleri_2;
    QString                     m_rapor_kriterleri_3;

    QList<int>                  M_STOK_GRUPLARI_ID_LIST;
    QStringList                 M_URUN_LISTESI;

    QString                     GET_HEADER              ();
    QString                     GET_FOOTER              ();
    QString                     GET_BODY                ();

    QStringList                 GET_URUN_LISTESI        ();

};

#endif // STOK_MIKTARLARI_RAPORU_H
