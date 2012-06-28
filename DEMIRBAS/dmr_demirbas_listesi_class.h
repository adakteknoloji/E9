#ifndef DMR_DEMIRBAS_LISTESI_H
#define DMR_DEMIRBAS_LISTESI_H

#include "report_kernel.h"

struct DMR_RAPOR_VARS;

class DMR_DEMIRBAS_LISTESI : public REPORT_KERNEL
{


public:
    DMR_DEMIRBAS_LISTESI(DMR_RAPOR_VARS * P_DMR_RV);
    ~DMR_DEMIRBAS_LISTESI() {}

protected:
    virtual QString             GET_HEADER                      ();
    virtual QString             GET_FOOTER                      ();
    virtual QString             GET_BODY                        ();

private:
    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    int                         m_sira_no;
    QList<int>                  M_DEMIRBAS_GRUPLARI_ID_LISTESI;
    QList<int>                  M_DEMIRBASLAR_ID_LISTESI;

    int                         m_last_satir_indisi;
    bool                        m_tum_satirlar_basildi;

    QStringList                 M_YAZDIRILACAK_SATIRLAR;

    DMR_RAPOR_VARS    *        M_DMR_RV;

    double                      m_degerleme_oncesi_demirbas_toplami;
    double                      m_degerleme_oncesi_birikmis_amortisman_toplami;
    double                      m_degerleme_oncesi_net_deger_toplami;

    double                      m_degerleme_sonrasi_demirbas_toplami;
    double                      m_degerleme_sonrasi_birikmis_amortisman_toplami;
    double                      m_degerleme_sonrasi_net_deger_toplami;

    double                      m_donem_amortisman_toplami;
    double                      m_birikmis_amortisman_toplami;

    double                      m_ayrilmamis_amortisman_toplami;

    void                        FILL_YAZDIRILACAK_SATIRLAR_LIST();
};

#endif // DMR_DEMIRBAS_LISTESI_H
