#ifndef STOK_CONSOLE_UTILS_H
#define STOK_CONSOLE_UTILS_H

#include <QList>
#include <QMap>
#include "stok_enum.h"

class   QString;
class   QStringList;
class   QDate;
class   ADAK_SQL;
class   QWidget;
struct  STK_URUNLER_STRUCT;


QString             STK_GET_STOK_TAKIP_SEKLI                 ( int p_stok_takip_sekli );
int                 STK_GET_STOK_TAKIP_SEKLI_ENUM            ( QString p_stok_takip_sekli );

QString             STK_GET_STOK_FIS_TURU                    ( int p_stok_fis_turu );
int                 STK_GET_STOK_FIS_TURU_ENUM               ( QString p_stok_fis_turu );

double              STK_DEPODAKI_URUN_MIKTARINI_BUL          ( int p_depo_id, int p_urun_id );

double              STK_PARTIDEKI_URUN_MIKTARINI_BUL         ( int p_parti_id );



void                STK_URUN_KARTINDAKI_STOK_MIKTARINI_ARTIR ( int    p_urun_id, int p_gun_no, double p_stoga_giren_miktar,
                                                              double p_stoktan_cikan_miktar, ADAK_SQL * P_GELEN_DB = NULL );

void                STK_URUN_KARTINDAKI_STOK_MIKTARINI_AZALT ( int    p_urun_id, int p_gun_no, double p_stoga_giren_miktar,
                                                              double p_stoktan_cikan_miktar);


void                STK_DEPODAKI_STOK_MIKTARINI_ARTIR        ( int p_depo_id, int p_urun_id, int p_gun_no,
                                                              double p_depoya_giren_miktar,
                                                              double p_depodan_cikan_miktar, ADAK_SQL * P_GELEN_DB = NULL );

void                STK_DEPODAKI_STOK_MIKTARINI_AZALT        ( int p_depo_id, int p_urun_id, int p_gun_no,
                                                              double p_depoya_giren_miktar,
                                                              double p_depodan_cikan_miktar);

void                STK_PARTIYI_KAYITLARDAN_SIL              ( int p_parti_id );

void                STK_IRS_SATIRINDAN_TUM_SERI_NOLARI_SIL   ( int p_irsaliye_satiri_id );

int                 STK_PARTIDEKI_URUN_MIKTARINI_ARTIR       ( int p_depo_id, int p_urun_id, int p_gun_no, QString p_parti_kodu,
                                                               double p_giren_urun_miktari, double p_cikan_urun_miktari,
                                                               QString p_uretim_tarihi = "", QString p_son_kul_tarihi = "",ADAK_SQL * P_GELEN_DB = NULL );

void                STK_PARTIDEKI_URUN_MIKTARINI_AZALT       ( int p_parti_id, int p_gun_no,
                                                               double p_giren_urun_miktari, double p_cikan_urun_miktari);

void                STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_GIRIS_CIKISLARINI_DUZENLE ( int       p_irsaliye_id,
                                                                                           QDate     p_old_fis_tarihi,
                                                                                           QDate     p_new_fis_tarihi,
                                                                                           int       p_depo_id,
                                                                                           int       p_alis_satis_turu);

bool                STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_MIKTARLARI_KONTROLU       ( int       p_irsaliye_id,
                                                                                           QDate     p_old_fis_tarihi,
                                                                                           QDate     p_new_fis_tarihi,
                                                                                           int       p_depo_id,
                                                                                           int       p_alis_satis_turu,
                                                                                           QWidget * p_parent );

bool                STK_IRSALIYE_SILINMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU           ( int p_irsaliye_id, QWidget * p_parent );

bool                STK_IRSALIYEYE_SATIR_EKLENMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU   ( QDate       p_tarih ,
                                                                                            int         p_irsaliye_stok_fisi_alis_satis_turu,
                                                                                            int         p_stok_takip_sekli,
                                                                                            int         p_urun_id,
                                                                                            int         p_depo_id,
                                                                                            int         p_parti_id,
                                                                                            double      p_girilen_stok_miktari,
                                                                                            QString      p_birim,
                                                                                            QWidget   * p_parent );

bool                STK_IRSALIYE_SATIR_GUNCELLEME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU   ( int       p_irsaliye_alis_satis_turu,
                                                                                           int       p_new_urun_id,
                                                                                           int       p_new_stok_takip_sekli,
                                                                                           int       p_depo_id,
                                                                                           double    p_new_stok_miktari,
                                                                                           int       p_new_seri_no_parti_kodu_id,
                                                                                           int       p_old_urun_id,
                                                                                           double    p_old_stok_miktari,
                                                                                           int       p_old_parti_kodu_id,
                                                                                           int       p_gun_no,
                                                                                           QWidget * p_parent );

bool                STK_IRSALIYE_SATIR_SILME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU        ( int       p_irsaliye_satiri_id,
                                                                                           int       p_irsaliye_alis_satis_turu,
                                                                                           QWidget * p_parent );

void                STK_URUN_SERI_NO_IRS_SATIRNA_KAYDET                                  ( int p_irsaliye_satiri_id, int p_seri_no_id );
void                STK_URUN_SERI_NO_IRS_SATIRINDAN_SIL                                  ( int p_seri_no_id );
void                STK_IRS_SATIRINDAN_TUM_SERI_NOLARI_SIL                               ( int p_irsaliye_satiri_id );

int                 STK_SIRADAKI_FIS_NO_AL                                               ( QString fis_tarihi );

int                 STK_GET_HIZMET_TURU_ENUM                                             ( QString hizmet_turu);
QString             STK_GET_HIZMET_TURU_STRING                                           ( int hizmet_turu);

int                 STK_GET_IRS_ID                                              ( int stk_irs_satir_id);

void                STK_UPDATE_IRSALIYE_FATURALASTIRILDI_MI                     ( int stk_irs_fis_id , int irsaliye_faturalastirildi_mi);


QString             STK_GET_PARTI_KODU                                          ( int parti_id);

int                 STK_GET_PARTI_ID                                            ( QString parti_kodu);

QList<int>          STK_GET_FIS_SATIRI_ID_LIST                                  ( int stk_irs_fis_id);

void                STK_FATURALANAN_MIKTARI_GUNCELLE                            ( int stk_irs_satir_id , double faturalanan_miktar);

int                 STK_GET_FAT_IRS_STR_BAGLANTISI_IRSALIYE_SATIRI_ID           ( int  p_fatura_satiri_id   );

int                 STK_GET_FAT_IRS_STR_BAGLANTISI_IRSALIYE_ID                  ( int  p_fatura_satiri_id   );

int                 STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID                      ( int  p_fatura_id );


QMap < int, int >   STK_GET_STK_IRS_FIS_AND_FIS_SATIRI_ID_MAP                   ( int       p_cari_hesap_id             = -1,
                                                                                  int       p_irsaliye_turu             = -1,
                                                                                  int       p_hatali_irsaliye_mi        = -1,
                                                                                  QString   p_fis_tarihi_baslangic   = "",
                                                                                  QString   p_fis_tarihi_bitis       = "" ,
                                                                                  int       p_irsaliye_faturalastirildi_mi = -1);


QList < int >        STK_GET_IRSALIYE_FIS_ID_LIST                               ( int       p_cari_hesap_id                 = -1,
                                                                                  int       p_irsaliye_turu                   = -1,
                                                                                  int       p_irsaliye_faturalastirildi_mi    = -1,
                                                                                  int       p_hatali_irsaliye_mi              = -1,
                                                                                  QString   p_fis_tarihi_baslangic            = "",
                                                                                  QString   p_fis_tarihi_bitis                = "" );





int                  STK_GET_URUN_KODU_ADI                                      ( int p_urun_id ,
                                                                                  QString * p_urun_kodu = NULL, QString * p_urun_adi = NULL);

int                  STK_GET_URUN_ID_ADI                                        ( QString p_urun_kodu ,
                                                                                  int * p_urun_id = NULL, QString * p_urun_adi = NULL);

int                  STK_URUNLER_TABLOSUNU_OKU                                  ( int urun_id , STK_URUNLER_STRUCT * P_URUN ,
                                                                                 bool arrayler_okunacak = false , ADAK_SQL * P_GELEN_DB = NULL);


int                  STK_GET_SERI_NUMARASI_ID                                   ( int irsaliye_satir_id);


QString              STK_GET_SERI_NUMARASI                                      ( int seri_numarasi_id);

void                 STK_SATIRDAKI_SERI_NO_TABLOSU_IRS_SATIR_ID_GUNCELLE        (int irsaliye_satiri_id ,int old_irs_satir_id);

int                  STK_GET_URUN_ID                                            ( QString urun_barkod_kodu ,int depo_id = -1 ,
                                                                                 int hizmet_mi = -1 , int hizmet_turu = -1);

QList <int>          STK_GET_URUN_HIZMET_ID_LIST                                ( QString bas_urun_kodu, QString bts_urun_kodu = "" , int hizmet_turu = -1);

int                  STK_GET_URUN_SAYI_AND_SERI_NUMARASI                        ( int p_irsaliye_satir_id, QString * p_urun_seri_numarasi = NULL );

int                  STK_GET_URUN_TAKIP_SEKLI                                   ( int p_urun_id );

int                  STK_GET_BIRIM_HASSASIYETI                                  ();

int                  STK_GET_MIKTAR_HASSASIYETI                                 ();

int                  STK_GET_HIZMET_MI                                  ( QString p_urun_or_hizmet_kodu, int * p_urun_id  );

int                  STK_GET_HIZMET_MI                                  ( int p_urun_id  );

QString              STK_GET_SERI_NO_BILGILERI                                  ( int p_irs_satir_id, URUN_DEPO_DURMU p_depoda_durumu, int p_fatura_satiri_id = -1 );

void                 STK_SERI_NOLARI_FATURA_SATIRI_ID_GUNCELLE                  ( QStringList p_seri_no_list, int p_urun_id, int p_fat_satiri_id );

int                  STK_URUN_KARTI_EKLE                                        ( STK_URUNLER_STRUCT *P_URUN_STRUCT );

#endif // STOK_CONSOLE_UTILS_H
