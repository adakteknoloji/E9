#ifndef IRS_IRSALIYE_BELGESI_H
#define IRS_IRSALIYE_BELGESI_H

#include "belge_kernel_class.h"


class IRS_IRSALIYE_BELGESI : public BELGE_KERNEL
{

public:
    IRS_IRSALIYE_BELGESI ( int p_irsaliye_id );


private:
    int            m_fis_id;
    int            m_line_index;

    // HEADER

    QString         M_IRS_SEVK_ADRESI;
    QString         M_IRS_FATURA_ADRESI;
    QString         M_IRS_VERGI_DAIRESI;
    QString         M_IRS_VERGI_NO;
    QString         M_IRS_SERI_SIRA_NO;
    QString         M_IRS_DUZENLEME_TARIHI;
    QString         M_IRS_DUZENLEME_SAATI;
    QString         M_IRS_TARIHI;
    QString         M_IRS_FAT_TARIHI;
    QString         M_IRS_FAT_NO;
    QString         M_IRS_FAT_FIILI_SEVK_TARIHI;

    QStringList     M_IRS_FAT_TARIH_LIST;
    QStringList     M_IRS_FAT_NO_LIST;


    // FOOTER

    QString         M_IRS_BRUT_TUTAR;
    QString         M_IRS_ISKONTO_TUTARI;
    QString         M_IRS_ARA_TOPLAM;
    QString         M_IRS_TOPLAM_RAKAMLA;
    QString         M_IRS_TOPLAM_YAZIYLA;
    QString         M_IRS_SON_CAR_BAKIYE;
    QString         M_IRS_KDV_1;
    QString         M_IRS_KDV_2;
    QString         M_IRS_KDV_3;
    QString         M_IRS_KDV_4;
    QString         M_IRS_KDV_5;
    QString         M_IRS_OTV;
    QString         M_IRS_TESLIM_ALAN;
    QString         M_IRS_TESLIM_EDEN;
    QString         M_IRS_DIP_NOT;

    //LINES

    QStringList     M_IRS_URUN_KODU;
    QStringList     M_IRS_URUN_CINSI;
    QStringList     M_IRS_BIRIM;
    QStringList     M_IRS_MIKTAR;
    QStringList     M_IRS_SATIR_TUTAR;
    QStringList     M_IRS_KDV_ORANI;
    QStringList     M_IRS_ISKONTO;
    QStringList     M_IRS_SATIS_TUTARI;
    QStringList     M_IRS_IS_SON_BIRIM_FIYAT;
    QStringList     M_IRS_SATIR_DOVIZ_KODU;
    QStringList     M_IRS_SATIR_OTV_TUTARI;
    QStringList     M_IRS_SATIR_KDV_TUTARI;
    QStringList     M_IRS_SATIR_OTV_DAHIL_TUTAR;
    QStringList     M_IRS_SATIR_KDV_DAHIL_TUTAR;
    QStringList     M_IRS_SATIR_KDV_ORANI;
    QStringList     M_IRS_SATIR_VERGI_HRC_TUTAR;
    QStringList     M_IRS_SATIR_OTV_ORANI;
    QStringList     M_IRS_SATIR_KDV_DAHIL_MI;
    QStringList     M_IRS_IS_ON_BIRIM_FIYAT;


    void           GET_HEADER_INFO                 ();
    void           GET_FOOTER_INFO                 ();
    void           GET_LINE_INFO                   ();


};

#endif // IRS_IRSALIYE_BELGESI_H
