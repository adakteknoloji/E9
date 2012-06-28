#ifndef FATURA_FIS_UTILS_H
#define FATURA_FIS_UTILS_H

#include <QList>

#define     VERGI_ORANI_EKLE                      100
#define     VERGI_ORANI_SIL                       200


class QString;
class QStringList;
class QTableWidget;

struct CARI_FIS_STRUCT;
struct CARI_FIS_SATIRI_STRUCT;
struct MUH_FIS_STRUCT;
struct MUH_FIS_SATIRI_STRUCT;

class ADAK_SQL;

struct FATURA_FISI_STRUCT {

    int             fis_no;
    QString         fatura_tarihi;
    int             fatura_turu;
    int             fatura_alis_satis_turu;
    QString         belge_seri;
    QString         belge_numarasi;
    int             acik_kapali_fatura;
    int             cari_hesap_id;
    QString         cari_hesap_ismi;
    QString         aciklama;
    QString         cari_hesap_adresi;
    QString         vergi_dairesi;
    QString         vergi_numarasi;
    QString         islem_saati;
    double          isk_oncesi_kdv_tutari;
    double          isk_oncesi_kdv_dahil_tutar;
    double          brut_tutar;
    double          isk_oncesi_otv_tutari;
    double          isk_oncesi_otv_dahil_tutar;
    int             yazdirildi_mi;
    int             iptal_mi;
    int             modul_id;
    int             program_id;
    int             base_fis_id;
    QString         doviz_tutar_bilgileri;
    QString         kdv_oran_bilgileri;
    QString         otv_oran_bilgileri;
    QString         iskonto_oran_bilgileri;
    int             fatura_irsaliyelestirildi_mi;
    int             yurtici_satislar_hesap_id;
    int             hizmet_urt_maliyeti_hesap_id;
    int             satistan_iade_hesap_id;
    int             iade_faturasi_mi;
    int             iade_edilen_fatura_id;
    int             tevkifatli_fatura_mi;
    double          tevkifat_yuzdesi;
    double          tevkifatli_kdv_tutari;
    double          iskonto_yuzdesi;
    double          iskonto_tutari;
    double          ara_toplam;
    double          fatura_tutari;
    double          isk_sonrasi_otv_tutari;
    double          isk_sonrasi_otv_dahil_tutar;
    double          isk_sonrasi_kdv_tutari;
    double          cari_para_birimi_tutari;
    int             cari_para_birimi_id;
    int             fatura_para_birimi_id;
    QString         fatura_para_birimi_kodu;
    double          doviz_kuru;
    double          parite;
    double      *   KDV_ORANLARI_ARRAY;
    int         *   KDV_HESAPLARI_ID_ARRAY;
    double      *   KDV_ORANINA_GORE_TUTARLAR_ARRAY;
    double      *   OTV_ORANLARI_ARRAY;
    int         *   OTV_HESAPLARI_ID_ARRAY;
    double      *   OTV_ORANINA_GORE_TUTARLAR_ARRAY;
    int             kdv_orani_sayisi;
    int             otv_orani_sayisi;
    int         *   TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY;
    QStringList     DOVIZ_CINSINDEN_SATIR_TOPLAMLARI;
    int             cari_fis_no;
    int             muh_fis_no;
    int             depo_id;
    int             muh_fis_id;
    int             kdv_muaf_mi;
    int             otv_muaf_mi;

    // fatura + irsaliyede ekrandan alinan degerleri
    // irsaliyeye gondrmek icin kullanilmisitir
    QString         irs_belge_seri;
    QString         irs_belge_numarasi;

};

struct FATURA_FIS_SATIRI_STRUCT {

    int             fis_id;
    int             order_number;
    int             urun_id;
    QString         urun_adi;
    double          satirdaki_urun_miktari;
    double          irsaliyelenen_miktar;
    QString         urun_birimi;
    double          isk_oncesi_urun_birim_fiyati;
    double          isk_sonrasi_urun_birim_fiyati;
    double          satir_iskonto_yuzdesi;
    double          satir_iskonto_tutari;
    double          t_is_son_birim_fiyati;
    double          satir_tutari;
    double          t_is_son_satir_tutari;
    int             doviz_id;
    double          kdv_orani;
    double          satir_kdv_tutari;
    double          satir_kdv_dahil_tutar;
    double          otv_orani;
    double          satir_vergi_haric_tutar;
    double          satir_otv_tutari;
    double          satir_otv_dahil_tutar;

    //! DIKKAT ENT ICIN ONEMLI
    //! BU DEGERLER FIS BAZINDA ISKONTO UYANDIGINDA
    //! UYGULANAN YENI ISKONTOLU DEGERLER TUTULUR
    //! SATIR BAZINDA ISKONTO DIREK SATIRA ETKI EDER
    //! ONUN ICIN EXTRA BIR SEY YAPMAYA GEREK YOTUR.
    double          t_is_son_satir_kdv_tutari;
    double          t_is_son_satir_kdv_dahil_tutar;
    double          t_is_son_str_vergi_haric_tutar;
    double          t_is_son_satir_otv_tutari;
    double          t_is_son_satir_otv_dahil_tutar;
    double          toplu_iskonto_yuzdesi;
    //! END

    int             kdv_dahil_mi;
    double          temel_birim_katsayisi;
    int             fiyat_irsaliyeden_alindi;
    int             hatali_satir_mi;
    double          iade_edilen_miktar;
    int             irsaliye_satiri_id;
    int             iade_edilen_fatura_satiri_id;
    QString         doviz_kodu;
    QString         parti_kodu;
    int             parti_id;
    int             modul_id;
    int             program_id;
    int             base_record_id;
    QStringList     seri_no_list;

};


int     FATURA_FISI_EKLE                                        ( FATURA_FISI_STRUCT * P_FIS                                            );
void    FATURA_FISI_SIL                                         ( FATURA_FISI_STRUCT * P_FIS, int p_fis_id                              );
int     FATURA_FIS_SATIRI_EKLE                                  ( FATURA_FISI_STRUCT * P_FIS, FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI,bool base_fis_satiri_eklenecek = true ,
                                                                  int fis_satiri_id = 0 , ADAK_SQL * P_GELEN_DB = NULL , int demirbas_id = 0);
void    FATURA_FIS_SATIRI_SIL                                   ( FATURA_FISI_STRUCT * P_FIS, FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id                       );
void    FATURA_FISI_GUNCELLE                                    ( FATURA_FISI_STRUCT * P_FIS, int p_fis_id );
void    FATURA_FIS_SATIRI_GUNCELLE                              ( FATURA_FISI_STRUCT * P_FIS, FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id );

int     FATURA_FIS_BILGILERINI_OKU                              ( FATURA_FISI_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);
int     FATURA_SATIR_BILGILERINI_OKU                            ( FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);

void    FATURA_KDV_OTV_DEGISKENLERINE_ILK_DEGERLERINI_YAZ       ( FATURA_FISI_STRUCT         * P_FIS        );
void    FATURA_CLEAR_FIS_STRUCT                                 ( FATURA_FISI_STRUCT         * P_FIS        );
void    FATURA_CLEAR_FIS_SATIRI_STRUCT                          ( FATURA_FIS_SATIRI_STRUCT   * P_FIS_SATIRI );

void    FATURA_KDV_ORANINA_GORE_TUTARLARI_GUNCELLE              ( FATURA_FISI_STRUCT * P_FIS, double p_kdv_orani, double p_kdv_tutari, int p_kdv_artir_azalt );
void    FATURA_OTV_ORANINA_GORE_TUTARLARI_GUNCELLE              ( FATURA_FISI_STRUCT * P_FIS, double p_otv_orani, double p_otv_tutari, int p_otv_artir_azalt );

void    FATURA_SATIR_DOVIZ_TUTARLARINI_GUNCELLE                 ( QString p_satir_doviz_kodu, double p_satir_doviz_tutari, int p_ekle_sil, QStringList & P_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI );

void    FATURA_ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA          ( FATURA_FISI_STRUCT * P_FIS );


//! FATURA IRSALIYELESTIRME FONKSIYONLARI

struct   FATURA_FISI_STRUCT;
struct   FATURA_FIS_SATIRI_STRUCT;

void     FAT_FATURA_IRSALIYELESTIR                               ( int p_fatura_id, FATURA_FISI_STRUCT * P_FIS );

int      FAT_FATURA_IRSALIYE_STR_EKLE                            ( int p_fatura_satiri_id, FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI );

void     FAT_FATURA_IRSALIYE_STR_UPDATE                          ( FATURA_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fatura_satiri_id );

void     FAT_FATURA_IRSALIYE_STR_SIL                             ( int p_fatura_satiri_id );

void     FAT_FATURA_IRSALIYESINI_SIL                             ( int p_fatura_id );


//! FATURA STRUCT UPDATE SADECE STRUCT"A UPDATE EDER HERHANGI BIR ISLEM YAPMAZ
void     FAT_FATURA_FIS_STRUCT_UPDATE_DB                         ( FATURA_FISI_STRUCT * P_FIS, int p_fis_id );



QList<int>FAT_FATURA_ENT_FISLERINI_OLUSTUR                         ( FATURA_FISI_STRUCT *P_FIS, int p_fis_id, ADAK_SQL * P_GELEN_DB = NULL );


#endif // FATURA_FIS_UTILS_H
