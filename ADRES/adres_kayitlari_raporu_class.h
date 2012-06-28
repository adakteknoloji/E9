#ifndef ADRES_KAYITLARI_RAPORU_H
#define ADRES_KAYITLARI_RAPORU_H

#include "report_kernel.h"

class ADRES_KAYITLARI_RAPORU : public REPORT_KERNEL
{

public:

                        ADRES_KAYITLARI_RAPORU          ( int rapor_turu ,int adres_kayit_turu, QStringList P_RAPOR_VERILERI );
                        ~ADRES_KAYITLARI_RAPORU         () {}

private:

    int                 m_max_line_count;
    int                 m_sayfa_numarasi;
    int                 m_satir_index;
    int                 m_adres_kayit_turu;
    int                 m_rapor_turu;

    QStringList         M_RAPOR_VERILERI;

    QString             GET_HEADER                      ();
    QString             GET_FOOTER                      ();
    QString             GET_BODY                        ();

};

#endif // ADRES_KAYITLARI_RAPORU_H
