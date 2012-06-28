#ifndef STOK_FIS_UTILS_H
#define STOK_FIS_UTILS_H

#include <QMap>

class QString;
class QStringList;
class ADAK_SQL;

struct STK_IRS_FIS_SATIR_STRUCT {

    int         fis_id;
    int         row_number;
    int         urun_id;
    QString     urun_adi;
    double      satirdaki_urun_miktari;
    double      iade_edilen_miktar;
    QString     urun_birimi;
    double      isk_oncesi_urun_birim_fiyati;
    double      isk_sonrasi_urun_birim_fiyati;
    double      iskonto_yuzdesi;
    double      iskonto_tutari;
    double      satir_tutari;
    int         doviz_id;
    QString     doviz_kodu;
    double      satir_vergi_haric_tutar;
    double      kdv_orani;
    double      satir_kdv_tutari;
    double      satir_kdv_dahil_tutar;
    int         kdv_dahil_mi;
    int         order_number;
    double      temel_birim_katsayisi;
    double      otv_orani;
    double      satir_otv_tutari;
    double      satir_otv_dahil_tutar;
    int         fatura_satiri_id;
    int         stok_takip_sekli;
    int         parti_id;
    QString     parti_kodu;
    int         hatali_satir_mi;
    int         iade_edilen_irsaliye_satir_id;
    double      toplu_iskonto_yuzdesi;
    double      t_is_sonrasi_birim_fiyat;
    double      t_is_sonrasi_satir_tutari;
    double      t_is_sonrasi_vergi_haric_tutar;
    double      t_is_sonrasi_otv_tutari;
    double      t_is_sonrasi_otv_dahil_tutar;
    double      t_is_sonrasi_kdv_tutari;
    double      t_is_sonrasi_kdv_dahil_tutar;
    QString     aciklama;
    double      irsaliye_tutari;
    int         base_record_id;
    int         program_id;
    int         modul_id;
    int         muh_fis_id;
    int         muh_hesap_id;
    int         faturalanan_miktar;

    // satir_id // kodu
    QStringList seri_no_list;
};

struct STK_IRS_FIS_STRUCT {

    int     fis_no;
    QString aciklama;
    QString fis_tarihi;
    int     fis_turu;
    int     program_id;
    int     modul_id;
    int     base_fis_id;
    int     muh_fis_id;
    int     depo_id;
    QString belge_seri;
    QString belge_numarasi;
    QString islem_saati;
    int     irsaliye_mi_stok_fisi_mi;
    int     irsaliye_alis_satis_turu;
    double  brut_tutar;
    double  isk_oncesi_otv_tutari;
    double  isk_oncesi_otv_dahil_tutar;
    double  isk_oncesi_kdv_tutari;
    double  isk_oncesi_kdv_dahil_tutar;
    double  ara_toplam;
    double  isk_sonrasi_otv_tutari;
    double  isk_sonrasi_otv_dahil_tutar;
    double  isk_sonrasi_kdv_tutari;
    double  irsaliye_tutari;
    double  iskonto_yuzdesi;
    double  iskonto_tutari;
    QString doviz_tutar_bilgileri;
    QString kdv_oran_bilgileri;
    QString otv_oran_bilgileri;
    int     alan_depo_id;
    QString sevk_tarihi;
    int     cari_hesap_id;
    int     cari_para_birim_id;
    QString cari_hesap_ismi;
    QString cari_para_birim_kodu;
    QString vergi_dairesi;
    QString vergi_numarasi;
    QString teslim_adresi;
    int     yazdirildi_mi;
    int     iptal_mi;
    int     irsaliye_faturalastirildi_mi;
    int     iade_irsaliyesi_mi;
    int     iade_edilen_irs_id;
    int     kdv_muaf_mi;
    int     otv_muaf_mi;
    int     muh_hesap_id;
    int     stk_irs_para_birimi_id;

    double  doviz_kuru;
    double  parite;

};

//! Fis
int        STK_IRS_FIS_EKLE                     ( STK_IRS_FIS_STRUCT       * P_FIS           );
int        STK_IRS_FIS_HAREKETLERI_OLUSTUR      ( STK_IRS_FIS_STRUCT       * P_FIS           );
int        STK_IRS_FIS_BILGILERINI_OKU          ( int p_fis_id , STK_IRS_FIS_STRUCT * P_FIS  );
void       STK_IRS_FIS_UPDATE                   ( int p_fis_id , STK_IRS_FIS_STRUCT * P_FIS  );
void       STK_IRS_CLEAR_FIS_STRUCT             ( STK_IRS_FIS_STRUCT       * P_FIS           );
void       STK_IRS_FIS_SIL                      ( int p_irsaliye_id, STK_IRS_FIS_STRUCT * P_FIS );

//! Satir
int        STK_IRS_SATIR_BILGILERINI_OKU        ( int p_fis_satiri_id , STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI);
int        STK_IRS_FIS_SATIRI_EKLE              ( STK_IRS_FIS_STRUCT       * P_FIS, STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI    );
int        STK_IRS_FIS_SATIR_HAREKETLERI_OLUSTUR( STK_IRS_FIS_STRUCT       * P_FIS, STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI, int p_irsaliye_satiri_id );
int        STK_IRS_FIS_SATIRI_GUNCELLE          ( STK_IRS_FIS_STRUCT       * P_FIS, STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI, int p_irsaliye_satiri_id );
void       STK_IRS_CLEAR_SATIR_STRUCT           ( STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI    );
void       STK_IRS_FIS_SATIRI_SIL               ( STK_IRS_FIS_STRUCT       * P_FIS, STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI, int p_irsaliye_satiri_id );


int        STK_ENT_FIS_EKLE                     ( STK_IRS_FIS_STRUCT       * P_FIS         , ADAK_SQL * P_GELEN_DB = NULL);
int        STK_ENT_FIS_SATIRI_EKLE              ( STK_IRS_FIS_STRUCT * P_FIS, STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI, ADAK_SQL * P_GELEN_DB = NULL);
void       STK_CLEAR_ENT_FIS_STRUCT             ( STK_IRS_FIS_STRUCT       * P_FIS                                 );

void       STK_SERI_NO_LIST_SIL_ALIS            ( int p_irsaliye_satiri_id );
void       STK_SERI_NO_LIST_ALIS                ( int p_depo_id, int p_urun_id, int p_irsaliye_satiri_id, QStringList p_seri_no_list );


void       STK_SERI_NO_LIST_IPTAL               ( int p_irsaliye_satiri_id, int p_depo_durumu  );
void       STK_SERI_NO_LIST_SATIS               ( int p_depo_id, int p_urun_id, int p_irsaliye_satiri_id, QStringList p_seri_no_list );

void       STK_SERI_NO_LIST_SATISTAN_IADE       ( int p_depo_id, int p_urun_id, int p_irsaliye_satiri_id, QStringList p_seri_no_list);

void       STK_FIS_SATIR_TUTARLARINI_HESAPLA    ( STK_IRS_FIS_STRUCT *P_FIS, STK_IRS_FIS_SATIR_STRUCT *P_FIS_SATIRI );

#endif // STOK_FIS_UTILS_H
