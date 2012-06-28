#ifndef DEFTERI_KEBIR_RAPORU_H
#define DEFTERI_KEBIR_RAPORU_H

#include "report_kernel.h"

struct MUH_RAPOR_VARS;

class DEFTERI_KEBIR_RAPORU : public REPORT_KERNEL
{

public:
                                        DEFTERI_KEBIR_RAPORU                (double font_size,double sayfa_genisligi,double sayfa_yuksekligi,struct MUH_RAPOR_VARS * );
                                        ~DEFTERI_KEBIR_RAPORU               () {}

private:
    struct MUH_RAPOR_VARS *     M_MUH_RV;

    struct DEFTERI_KEBIR_FIS_SATIRLARI {

        int         satir_hesap_id;
        int         yevmiye_no;
        double      borc_tutari;
        double      alacak_tutari;
        QString     satir_tam_hesap_kodu;
        QString     satir_hesap_ismi;
        QString     aciklama;
        QString     fis_tarihi;
    };


    struct DEFTERI_KEBIR_MUH_ANA_HESAPLAR {
        int         ana_hesap_id;
        QString     ana_hesap_kodu;
        QString     ana_hesap_ismi;
        QString     borc_bakiyesi;
        QString     alacak_bakiyesi;
    };


    QString                             m_firma_ismi;
    int                                 m_max_yazdirilacak_satir_sayisi;
    int                                 m_yazdirilan_satir_sayisi;
    bool                                m_tum_satirlar_yazdirildi;

    int                                 m_son_hesap_index;
    int                                 m_current_hesap_index;

    double                              m_footer_borc;
    double                              m_footer_alacak;
    double                              m_font_size;

    bool                                m_borc_satirlari_bitti;
    bool                                m_alacak_satirlari_bitti;

    int                                 m_toplam_hesap_sayisi;
    bool                                m_raporlanacak_kayit_var_mi;
    bool                                m_ana_hesabin_fis_satiri_var_mi;

    int                                 m_borc_satiri_sayisi;
    int                                 m_alacak_satiri_sayisi;

    int                                 m_borc_satirlari_index;
    int                                 m_alacak_satirlari_index;

    DEFTERI_KEBIR_FIS_SATIRLARI    *    M_DEFTERI_KEBIR_BORC_SATIRLARI;
    DEFTERI_KEBIR_FIS_SATIRLARI    *    M_DEFTERI_KEBIR_ALACAK_SATIRLARI;
    DEFTERI_KEBIR_MUH_ANA_HESAPLAR *    M_MUH_ANA_HESAPLAR;

    QString                             GET_HEADER                              ();
    QString                             GET_FOOTER                              ();
    QString                             GET_BODY                                ();

    bool                                RAPOR_VERILERINI_AL                     ();
    void                                DEVIR_BAKIYESINI_YAZDIR                 ();

};

#endif // DEFTERI_KEBIR_RAPORU_H
