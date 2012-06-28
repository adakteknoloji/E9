#ifndef IRS_IRSALIYE_LISTESI_RAPORU_H
#define IRS_IRSALIYE_LISTESI_RAPORU_H

#include "report_kernel.h"

struct IRSALIYE_RAPOR_VARS;

class IRS_IRSALIYE_LISTESI_RAPORU : public REPORT_KERNEL
{


public:
    IRS_IRSALIYE_LISTESI_RAPORU                                          ( IRSALIYE_RAPOR_VARS * P_IRS_RV , QString rapor_ismi );
    ~IRS_IRSALIYE_LISTESI_RAPORU                                         () {}

private:

    IRSALIYE_RAPOR_VARS *       M_IRS_RV ;
    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    QString                     m_rapor_kriterleri_1;
    QStringList                 M_RAPOR_SATIRLARI;
    QString                     m_baslangic_tarihi;
    QString                     m_bitis_tarihi;
    bool                        m_tum_rapor_satirlari_basildi;
    int                         m_last_satir_indisi;
    double                      m_footer_tutar;


    QString                     GET_HEADER              ();
    QString                     GET_FOOTER              ();
    QString                     GET_BODY                ();

    void                        FILL_RAPOR_SATIRLARI();

};

#endif // IRS_IRSALIYE_LISTESI_RAPORU_H
