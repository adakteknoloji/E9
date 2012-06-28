#ifndef MUHASEBE_STRUCT_H
#define MUHASEBE_STRUCT_H

#include <QDate>

struct MUH_RAPOR_VARS {
    QString         bas_hesap_kodu;
    QString         bts_hesap_kodu;
    QDate           baslangic_tarihi;
    QDate           bitis_tarihi;
    int             mizan_seviyesi;
    bool            nakli_yekun;
    int             bas_gun_no;
    int             bts_gun_no;
    int             calismamis_hesaplar;
    bool            bakiyesiz_hesaplar;
    bool            alt_hesaplar;
    bool            sadece_ana_hesaplar;
    YVM_HESAP_ORDER type_hesap_order;
    int             baslangic_yevmiye_no;
    int             bitis_yevmiye_no;
    bool            deneme_baskisi;

};

struct KOK_HESAP_STRUCT {
    QString             tam_hesap_kodu;
    int                 hesap_id;
    int                 hesap_seviyesi;
    QString             hesap_ismi;
    QString             child_tam_hesap_kodu;
};

struct FIS_SATIRI_STRUCT {
    int                 fis_satiri_id;
    int                 fis_id;
    int                 hesap_id;
    int                 order_number;
    double              borc_tutari;
    double              alacak_tutari;
    QString             aciklama;

    int                 parent_id;
    int                 hesap_seviyesi;
    QString             hesap_ismi;
    QString             hesap_kodu;
    QString             tam_hesap_kodu;
    KOK_HESAP_STRUCT    kok_hesap;
};

struct FIS_STRUCT {
    int                 fis_id;
    int                 fis_no;
    int                 kasa_hesabi_id;
    int                 fis_turu;
    int                 yevmiye_no;
    int                 fis_satirlari_sayisi;
    double              toplam_borc;
    double              toplam_alacak;
    QString             fis_tarihi;
    QString             aciklama;
    FIS_SATIRI_STRUCT * fis_satirlari;
};

struct MUH_HESAP_STRUCT {
    int         hesap_id;
    int         parent_id;
    int         hesap_seviyesi;
    int         tali_hesap;
    QString     tam_hesap_kodu;
    QString     hesap_ismi;
    int         alt_hesap_sayisi;
    QString     toplam_borc_array;
    QString     toplam_alacak_array;
};

struct MUH_KDV_AYIRMA_STRUCT {
    QString     aciklama;
    double      kdv_borc_tutari;
    double      kdv_alacak_tutari;
    double      satir_kdv_haric_borc_tutari;
    double      satir_kdv_haric_alacak_tutari;
    int         kdv_hesap_id;
    int         order_number;
};

#endif // MUHASEBE_STRUCT_H
