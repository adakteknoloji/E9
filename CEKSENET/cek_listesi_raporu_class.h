#ifndef CEK_LISTESI_RAPORU_H
#define CEK_LISTESI_RAPORU_H

#include "report_kernel.h"

struct CEK_RAPOR_VARS;

class CEK_LISTESI_RAPORU : public REPORT_KERNEL
{


public:
    CEK_LISTESI_RAPORU( CEK_RAPOR_VARS * P_CEK_RV);
    ~CEK_LISTESI_RAPORU() {}


private:

    CEK_RAPOR_VARS    *         M_CEK_RV;

    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    int                         m_last_cek_indisi;
    QStringList                 m_cekler_listesi;
    double                      m_toplam_tutar;

    QString                     m_cek_senet_column_header;

    QString                     GET_HEADER                      ();
    QString                     GET_FOOTER                      ();
    QString                     GET_BODY                        ();

    void                        FILL_CEKLER_LISTESI             ();
};

#endif // CEK_LISTESI_RAPORU_H
