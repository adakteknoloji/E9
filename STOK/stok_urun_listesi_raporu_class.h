#ifndef STOK_URUN_LISTESI_RAPORU_H
#define STOK_URUN_LISTESI_RAPORU_H

#include "report_kernel.h"

class STOK_URUN_LISTESI_RAPORU : public REPORT_KERNEL
{


public:
    STOK_URUN_LISTESI_RAPORU                                          ( QString baslik ,struct STOK_RAPOR_VARS *    M_STOK_RV);
    ~STOK_URUN_LISTESI_RAPORU                                         () {}

private:
    struct STOK_RAPOR_VARS *    M_STOK_RV;

    int                         m_max_line_count;
    int                         m_last_urun_indisi;

    QString                     m_urun_araligi;
    QString                     m_grup_bilgisi;
    bool                        m_tum_kayitlar_basildi;

    QList<int>                  M_STOK_GRUPLARI_ID_LIST;
    QStringList                 M_URUN_LISTESI;


    QString             GET_HEADER              ();
    QString             GET_FOOTER              ();
    QString             GET_BODY                ();

    QStringList         GET_URUN_LISTESI ();


};

#endif // STOK_URUN_LISTESI_RAPORU_H
