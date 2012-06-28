#ifndef CARI_BAKIYE_RAPORU_H
#define CARI_BAKIYE_RAPORU_H

#include "report_kernel.h"
#include <QStringList>

struct CARI_RAPOR_VARS;

class CARI_BAKIYE_RAPORU : public REPORT_KERNEL
{

public:
                                CARI_BAKIYE_RAPORU              (CARI_RAPOR_VARS * P_CARI_RV);
                                ~CARI_BAKIYE_RAPORU             () {}
private:

    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    QStringList                 m_cari_hesaplar_list;
    int                         m_last_cari_hesap_no;
    int                         m_tum_hesaplar_basildi;

    QList<int>                  M_CARI_HESAPLAR_ID_LIST;

    CARI_RAPOR_VARS    *       M_CARI_RV;

    double                      m_footer_borc;
    double                      m_footer_alacak;
    double                      m_footer_borc_bakiye;
    double                      m_footer_alacak_bakiye;

    QString                     GET_HEADER                      ();
    QString                     GET_FOOTER                      ();
    QString                     GET_BODY                        ();

    QStringList                 GET_CARI_HESAPLAR_LIST          ();

    void                        FILL_CARI_GRUPLAR_ID_LIST        ();

};

#endif // CARI_BAKIYE_RAPORU_H
