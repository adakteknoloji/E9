#ifndef FAT_FATURA_BELGESI_H
#define FAT_FATURA_BELGESI_H

#include "belge_kernel_class.h"


class FAT_FATURA_BELGESI : public BELGE_KERNEL
{

public:
    FAT_FATURA_BELGESI(int fat_fis_id);


private:
    int            m_fis_id;
    int            m_line_index;

    //HEADER
    QString M_FAT_ADR_STR;
    QString M_FAT_VERGI_DAIRE_STR;
    QString M_FAT_VERGI_NO_STR;
    QString M_FAT_SERISIRA_STR;
    QString M_FAT_TARIHI_STR;
    QString M_FAT_IRS_TARIHI_STR;
    QString M_FAT_IRSALIYE_NO_STR;
    QString M_FAT_DUZEN_TAR_STR;
    QString M_FAT_DUZEN_SAAT_STR;
    QString M_FAT_SEVK_ADR_STR;
    QString M_FAT_CARI_HESAP_ISMI_STR;
    QString M_FAT_ACIKLAMA_STR;
    QString M_FAT_FIS_NO_STR;
    QString M_FAT_ACIK_KAPALI_FATURA_STR;
    //
    QStringList M_FAT_IRS_TARIHI_LIST;
    QStringList M_FAT_IRS_NO_LIST;

    //FOOTER
    QString M_FAT_BRUT_TUTAR_STR;
    QString M_FAT_ISKONTO_YUZDESI_STR;
    QString M_FAT_ISKONTO_TUTARI_STR;
    QString M_FAT_ARA_TOPLAM_STR;
    QString M_FAT_OTV_TUTARI_STR;
    QString M_FAT_OTV_DAHIL_TUTAR_STR;
    QString M_FAT_KDV_TUTARI_STR;
    QString M_FAT_TUTARI_RAKAMLA_STR;
    QString M_FAT_TUTARI_YAZIYLA_STR;
    QString M_FAT_SON_CAR_BAK_STR;
    QString M_FAT_KDV_1_STR;
    QString M_FAT_KDV_2_STR;
    QString M_FAT_KDV_3_STR;
    QString M_FAT_KDV_4_STR;
    QString M_FAT_KDV_5_STR;


    //LINE
    QStringList M_FAT_URUN_KODU;
    QStringList M_FAT_URUN_CINSI;
    QStringList M_FAT_URUN_BIRIMI;
    QStringList M_FAT_URUN_MIKTARI;
    QStringList M_FAT_SAT_ISK_ONCESI_BIRIM_FIYAT;
    QStringList M_FAT_SAT_ISK_SONRASI_BIRIM_FIYAT;
    QStringList M_FAT_SAT_ISKONTO_YUZDESI;
    QStringList M_FAT_SAT_ISKONTO_TUTARI;
    QStringList M_FAT_SAT_SATIR_TUTARI;
    QStringList M_FAT_SAT_DOVIZ_KODU;
    QStringList M_FAT_KDV_DAHIL_MI;
    QStringList M_FAT_SAT_VERGI_HARIC_TUTAR;
    QStringList M_FAT_SAT_OTV_TUTARI;
    QStringList M_FAT_SAT_OTV_DAHIL_TUTAR;
    QStringList M_FAT_SAT_KDV_TUTARI;
    QStringList M_FAT_SAT_KDV_DAHIL_TUTAR;
    QStringList M_FAT_KDV_ORANI;
    QStringList M_FAT_OTV_ORANI;

    void           GET_HEADER_INFO                 ();
    void           GET_FOOTER_INFO                 ();
    void           GET_LINE_INFO                   ();


};

#endif // FAT_FATURA_BELGESI_H
