#ifndef HESAP_EKSTRESI_RAPORU_H
#define HESAP_EKSTRESI_RAPORU_H

#include "report_kernel.h"

struct HESAP_FIS_SATIRI_STRUCT {
    int                 fis_satiri_id;
    int                 fis_id;
    int                 order_number;
    double              borc_tutari;
    double              alacak_tutari;
    QString             fis_no;
    QString             aciklama;
    QString             fis_tarihi;
};

struct HESAPLAR_STRUCT {
    int                        hesap_id;
    int                        parent_id;
    int                        hesap_seviyesi;
    int                        tali_hesap;
    int                        fis_satirlari_sayisi;
    QString                    tam_hesap_kodu;
    QString                    hesap_ismi;
    QString                    borc_bakiyesi;
    QString                    alacak_bakiyesi;
    HESAP_FIS_SATIRI_STRUCT *  fis_satirlari;
};


class HESAP_EKSTRESI_RAPORU : public REPORT_KERNEL
{

public:

    HESAP_EKSTRESI_RAPORU(struct MUH_RAPOR_VARS *  M_MUH_RV);
    ~HESAP_EKSTRESI_RAPORU();


private:
    struct MUH_RAPOR_VARS *     M_MUH_RV;
    HESAPLAR_STRUCT *           M_MUH_HESAPLAR;
    bool                        m_raporlanacak_kayit_var_mi;
    QString                     m_rapor_kriterleri_1;
    QString                     m_rapor_kriterleri_2;

    QStringList                 M_TUM_HESAPLAR_LIST;

    int                         m_max_yazdirilabilir_satir_sayisi;
    int                         m_yazdirilan_kayit_sayisi;
    int                         m_num_of_muh_hesaplar;
    int                         m_last_hesap_index;
    int                         m_last_satir_indisi;
    bool                        m_satirlar_basildi;

    double                      m_footer_borc;
    double                      m_footer_alacak;
    double                      m_footer_borc_bakiye;
    double                      m_footer_alacak_bakiye;

    bool                        FILL_STRUCT                       ();
    int                         FIND_PARENT_HESAP                 ( int hesap_id);
    void                        PRINT_NAKLI_YEKUN                 ( double toplam_borc , double toplam_alacak);
    QString                     GET_HESAP_KODU_ADI                ( int hesap_id );
    void                        ORDER_MUHASEBE_HESAPLARI          ();
    int                         FIND_CURRENT_HESAP_INDEX          ( QString tam_hesap_kodu);

    QStringList                 FIS_SATIRLARI                     ( int hesap_id);

    void                        FILL_TUM_HESAPLAR_LIST  ();

    QString             GET_HEADER                      ();
    QString             GET_FOOTER                      ();
    QString             GET_BODY                        ();


};

#endif // HESAP_EKSTRESI_RAPORU_H














