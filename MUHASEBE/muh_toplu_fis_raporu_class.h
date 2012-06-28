#ifndef TOPLU_FIS_RAPORU_H
#define TOPLU_FIS_RAPORU_H

#include <QDate>
#include "report_kernel.h"


class TOPLU_FIS_RAPORU : public REPORT_KERNEL
{


public:
    TOPLU_FIS_RAPORU                                        ( int fis_id ,struct MUH_RAPOR_VARS *  M_MUH_RV);
    virtual ~TOPLU_FIS_RAPORU                               (){}


private:

    struct MUH_RAPOR_VARS *  M_MUH_RV;

    struct FIS_SATIRI {
        int     hesap_kodu_id;
        int     fis_satiri_id;
        bool    basilacak_satir;
        double  borc_tutari;
        double  alacak_tutari;
    };

    QDate                       m_baslangic_tarihi;
    QDate                       m_bitis_tarihi;
    int                         m_baslangic_fisi;
    int                         m_bitis_fisi;


    QStringList                 M_MUH_HESAPLAR;
    QStringList                 M_TUM_FIS_SATIRLARI;
    QStringList                 M_BORC_FIS_SATIRLARI;
    QStringList                 M_ALACAK_FIS_SATIRLARI;
    QStringList                 M_TUM_FISLER;
    QStringList                 M_FIS_BILGILERI;
    QStringList                 M_FIS_BILGI_SUTUNLARI;

    QString                     m_fis_tarihi_str;
    QString                     m_body_string;
    QString                     m_fis_satirlari_string;
    QString                     m_firma_ismi;

    int                         m_fis_satir_indisi;
    int                         m_alacaklar_yazilacak;
    int                         m_max_line_count;
    int                         m_son_gonderilen_kayit_id;
    int                         m_fis_sayisi;

    double                      m_borc;
    double                      m_alacak;
    double                      m_toplam_borc;
    double                      m_toplam_alacak;
    double                      m_sayfadaki_toplam_borc;
    double                      m_sayfadaki_toplam_alacak;


    bool                        m_rapor_bitti_mi;
    bool                        m_fis_satiri_bulundu;
    bool                        m_fis_bitti_mi;

    QString                     GET_HEADER                         ();
    QString                     GET_FOOTER                         ();
    QString                     GET_BODY                           ();

    void                        SET_REPORT_VARIABLES               ();
    void                        BORC_VE_ALACAK_SATIRLARINI_OLUSTUR ( int fis_id );
};

#endif // TOPLU_FIS_RAPORU_H
