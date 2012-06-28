#ifndef FATURA_STRUCT_H
#define FATURA_STRUCT_H

#include <QDate>
#include <QString>

struct FATURA_DETAYLARI_STRUCT {
    int     yurtici_satislar_hesap_id;
    int     hizmet_urt_maliyeti_hesap_id;
    int     satistan_iade_hesap_id;
    int     tevkifatli_fatura_mi;
    double  tevkifat_yuzdesi;
    int     muh_fis_no;
    int     cari_fis_no;
    int     kdv_muaf_mi;
    int     otv_muaf_mi;

};


struct FATURA_RAPOR_STRUCT {
    int       cari_hesap_id;
    int       irsaliye_durumu;
    QDate     baslangic_tarihi;
    QDate     bitis_tarihi;
    double    min_tutar;
    double    max_tutar;
    int       fatura_alis_satis_turu;
    int       entegre_filtresi_turu;
    QString   bas_hizmet_kodu;
    QString   bts_hizmet_kodu;
    QString   tek_hizmet_kodu;
    int       hizmet_turu;
    bool      nakli_yekun;
    int       acik_kapali_fatura;
    int       fatura_turu;
};

struct FATURA_TEMEL_BILGILER_STRUCT {
    int     muh_fis_id;
    double  fatura_tutari;
    double  isk_sonrasi_kdv_tutari;
};

#endif // FATURA_STRUCT_H
