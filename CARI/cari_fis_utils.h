#ifndef CARI_FIS_UTILS_H
#define CARI_FIS_UTILS_H

#include <QStringList>
#include "adak_utils.h"
#include "e9_enum.h"

struct CEK_BORDRO_STRUCT;
struct CEK_BORDRO_SATIRI_STRUCT;
class ADAK_SQL;

struct CARI_FIS_STRUCT {

    int             modul_id;
    int             program_id;
    int             base_fis_id;

    int             fis_no;
    QString         fis_tarihi;
    int             fis_turu;
    int             cari_hesap_id;
    QString         aciklama;
    double          toplam_borc;
    double          toplam_alacak;

    int             makbuz_para_birimi_id;
    int             cari_para_birimi_id;
    double          doviz_kuru;
    double          parite;

    int             muh_kasa_hesap_id;

    QString         belge_seri;
    QString         belge_numarasi;

    QString         borc_doviz_tutar_bilgileri;
    QString         alacak_doviz_tutar_bilgileri;

    QStringList     DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI;
    QStringList     DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI;

    int             muh_fis_id;
    int             yazdirildi_mi;
    int             iptal_mi;

};

struct CARI_FIS_SATIRI_STRUCT {
    int             fis_id;
    QString         aciklama;
    double          borc_tutari;
    double          alacak_tutari;
    QString         kredi_karti_numarasi;
    QString         kredi_karti_sahibi;
    QString         musteri_banka_ismi;
    QString         musteri_sube_kodu;
    QString         musteri_sube_ismi;
    QString         musteri_hesap_no;

    QString         cek_senet_vade_tarihi;
    QString         cek_senet_keside_tarihi;
    QString         kesideci;
    QString         cek_senet_no;

    int             cari_hesap_id;
    int             cek_senet_id;
    int             islem_turu;
    int             order_number;
    int             muh_hesap_id;
    int             hesap_no_id;
    int             pos_id;
    int             kredi_karti_id;
    int             doviz_id;
    double          doviz_kuru;
    double          parite;
    QString         doviz_kodu;
    int             portfoy_no;

    int             base_record_id;
    int             program_id;
    int             modul_id;

    double          cari_para_birimi_tutari;
};


int             CARI_FIS_EKLE               ( CARI_FIS_STRUCT * P_FIS , ADAK_SQL * P_GELEN_DB = NULL);
int             CARI_FIS_SATIRI_EKLE        ( CARI_FIS_STRUCT * P_FIS, CARI_FIS_SATIRI_STRUCT * P_FIS_SATIRI ,
                                              bool base_fis_satiri_eklenecek = true,int p_fis_satiri_id = 0 , int *p_muh_olusan_fis_satiri_id = NULL, ADAK_SQL * P_GELEN_DB = NULL);

int             CARI_FIS_SATIRI_SIL         ( CARI_FIS_STRUCT * P_FIS, int p_fis_satiri_id ,ADAK_SQL * P_GELEN_DB = NULL);
void            CARI_FISI_SIL               ( CARI_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);

void            CARI_FISI_GUNCELLE          ( CARI_FIS_STRUCT * P_FIS, int p_fis_id );

void            CARI_FIS_BILGILERINI_OKU    ( CARI_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);
void            CARI_FIS_SATIRINI_OKU       ( CARI_FIS_SATIRI_STRUCT * P_FIS_SATIRI  , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);

int             CARI_FIS_SATIRINI_GUNCELLE  ( CARI_FIS_STRUCT * P_FIS, CARI_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id );

void            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_ARTIR     ( QString p_para_birimi_kodu, double p_satir_tutari, int borc_satiri, QStringList &P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,  QStringList &P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
void            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_AZALT     ( QString p_para_birimi_kodu, double p_satir_tutari, int borc_satiri, QStringList &P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,  QStringList &P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );

QString         CARI_GET_HAREKET_FISI_SATIR_DOVIZ_BORC_TUTARLARI    ( QStringList & P_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI );
QString         CARI_GET_HAREKET_FISI_SATIR_DOVIZ_ALACAK_TUTARLARI  ( QStringList & P_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );


void            CARI_CLEAR_FIS_STRUCT        ( CARI_FIS_STRUCT * P_FIS );
void            CARI_CLEAR_FIS_SATIRI_STRUCT ( CARI_FIS_SATIRI_STRUCT * P_FIS_SATIRI);

int             CARI_GET_ENT_FIS_ID           ( int program_id, int modul_id, int base_fis_id    , ADAK_SQL * P_GELEN_DB = NULL);

QStringList     CARI_GET_ENT_ROW_ID_LIST      ( int program_id, int modul_id, int base_record_id , ADAK_SQL * P_GELEN_DB = NULL);

void            CARI_BANKA_MODULUNE_FIS_SATIR_EKLE ( CARI_FIS_STRUCT * P_FIS , CARI_FIS_SATIRI_STRUCT * P_FIS_SATIRI,
                                                int cari_hesap_id , int base_fis_id , ADAK_SQL * P_GELEN_DB);
void            CARI_CEKSENET_MODULUNE_FIS_SATIR_EKLE ( CARI_FIS_STRUCT * P_FIS,CARI_FIS_SATIRI_STRUCT * P_FIS_SATIRI,
                                                   double temel_para_birimi_tutari,double cari_para_birimi_tutari,int base_fis_id , bool p_cek_hareketi_ekle,ADAK_SQL * P_GELEN_DB);

QStringList     CARI_GET_FIS_SATIRLARI_ID_LIST      ( int p_program_id, int p_modul_id, int p_fis_id );
QStringList     CARI_FISE_GORE_GET_ENT_ROW_ID_LIST  ( int p_program_id, int p_modul_id, int p_base_record_id , int p_fis_id);

FIS_YAZICI_DURMU CARI_GET_FIS_YAZDIRILDIMI          ( int p_fis_id );
#endif // CARI_FIS_UTILS_H
