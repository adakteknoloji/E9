#ifndef STOK_STRUCT_H
#define STOK_STRUCT_H

#include <QList>
#include <QDate>
#include "e9_enum.h"

class QString;
class QDate;

struct IRS_SATIRI_BILGILERI_STRUCT {

    int     urun_id;
    QString urun_kodu;
    QString urun_adi;
    int     stok_takip_sekli;
    int     parti_id;
    double  silinecek_urun_miktari;
    int     fis_satiri_id;
};


struct STOK_RAPOR_VARS {
    QString         bas_urun_kodu;
    QString         bts_urun_kodu;
    QString         tek_urun_kodu;
    bool            nakli_yekun;
    QList<int>      grup_idleri;
    int             bts_grup_id;
    int             depo_id;
    QDate           baslangic_tarihi;
    QDate           bitis_tarihi;
    int             ent_filtresi;
    int             stok_alis_satis_durumu;
    int             urun_mu_hizmet_mi;

};

struct STOK_DETAYLAR_STRUCT {
    int           kdv_muaf_mi;
    int           otv_muaf_mi;
    int           muh_hesap_id;
};

struct STK_URUNLER_STRUCT {
    QString     urun_kodu;
    QString     urun_adi;
    QString     barkod_numarasi;
    double      alis_fiyati;
    double      satis_fiyati;
    int         urun_alis_doviz_id;
    int         urun_satis_doviz_id;
    int         stok_takip_sekli;
    QString     urun_temel_birimi;
    double      min_stok_seviyesi;
    int         kritik_stok_seviyesi;
    QString     aciklama;
    double      alis_otv_orani;
    double      alis_kdv_orani;
    double      satis_otv_orani;
    double      satis_kdv_orani;
    int         hizmet_mi;
    int         muh_hesap_id;
    int         hizmet_turu;

    ////////

    double     toplam_stoga_giren_array         [E9_ARRAY_SIZE];
    double     toplam_stoktan_cikan_array       [E9_ARRAY_SIZE];

    QString    urun_birimleri_array             [URUN_BIRIMLERI_SAYISI];
    double     urun_birim_katsayilari_array     [URUN_BIRIMLERI_SAYISI];
    int        urun_capraz_birimler_array       [URUN_BIRIMLERI_SAYISI];
    double     urun_birim_alis_fiyatlar_array   [URUN_BIRIMLERI_SAYISI];
    double     urun_birim_sts_fiyatlar_array    [URUN_BIRIMLERI_SAYISI];
    int        gorunecek_urn_birimleri_array    [URUN_BIRIMLERI_SAYISI];

};


#endif // STOK_STRUCT_H
