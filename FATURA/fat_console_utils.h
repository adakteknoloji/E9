#ifndef FAT_CONSOLE_UTILS_H
#define FAT_CONSOLE_UTILS_H

#include <QMap>
#include <QList>

class   QString;
class   QDate;
class   ADAK_SQL;

struct  FATURA_TEMEL_BILGILER_STRUCT;



QString     FAT_GET_ACIK_KAPALI_FATURA_STRING                           ( int     fatura_cinsi  );
int         FAT_GET_ACIK_KAPALI_FATURA_ENUM                             ( QString fatura_cinsi  );

int         FAT_GET_FATURA_TURLERI_ENUM                                 ( QString p_fatura_turu );
QString     FAT_GET_FATURA_TURLERI_STRING                               ( int     p_fatura_turu );

void        FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE         ( int p_fatura_satiri_id, double p_irsaliyelenen_miktar, int p_artir_azalt );

int         FAT_SIRADAKI_FIS_NO_AL                                      ( QString p_fatura_tarihi );

void    *   FAT_GET_FATURA_DETAYLARI_STRUCT                             ();

void        FAT_FATURA_SATIRINDA_IADE_MIKTARINI_GUNCELLE                ( int p_fatura_satiri_id, double p_iade_edilen_miktar, int p_artir_azalt , ADAK_SQL * P_GELEN_DB = NULL);

int         FAT_GET_IRSALIYE_DURUMU_ENUM                                ( QString irsaliye_durumu );

QString     FAT_GET_FAT_IRSALIYE_DURUMU_STRING                          ( int irsaliye_durumu );

int         FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_ENUM                ( QString alis_satis_turu);

QString     FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_STRING              ( int alis_satis_turu);

int         FAT_GET_FAT_BA_BS_FORMU_TURU_ENUM                           ( QString ba_bs_turu );

QString     FAT_GET_FAT_BA_BS_FORMU_TURU_STRING                         ( int ba_bs_turu );

int         FAT_GET_FATURA_CARI_HESAP_ID                                ( int p_fatura_id );

void        FAT_GET_FAT_TEMEL_BILGILER                                  ( FATURA_TEMEL_BILGILER_STRUCT * P_FAT, int p_fatura_id ,ADAK_SQL * P_GELEN_DB = NULL);

int         FAT_GET_FATURA_FIS_SATIRI_ID                                ( int p_urun_id );

QList < int > FAT_GET_FATURA_FIS_ID_LIST                                ( int       p_cari_hesap_id                 = -1,
                                                                          int       p_fatura_turu                   = -1,
                                                                          int       p_fatura_irsaliyelestirildi_mi  = -1,
                                                                          int       p_hatali_fatura_mi              = -1,
                                                                         QString   p_fatura_tarihi_baslangic       = "",
                                                                         QString   p_fatura_tarihi_bitis           = "" );
// QMap < fis_id, fis_satiri_id >

QMap < int, int > FAT_GET_FATURA_FIS_AND_FIS_SATIRI_ID_MAP             ( int       p_cari_hesap_id             = -1,
                                                                         int       p_fatura_turu               = -1,
                                                                         int       p_hatali_fatura_mi          = -1,
                                                                         QString   p_fatura_tarihi_baslangic   = "",
                                                                         QString   p_fatura_tarihi_bitis       = "" );

void    FAT_UPDATE_FATURA_SATIRI_URUN_MIKTARI                         ( int p_fatura_satiri_id, int p_satirdaki_urun_miktari );

int     FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_ID                      ( int  p_irsaliye_satiri_id );

int     FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_SATIRI_ID               ( int  p_irsaliye_satiri_id );

QList <int>  FAT_GET_FAT_IRS_BAGLANTISI_FATURA_ID                     ( int  p_irsaliye_id );

QList <int> FAT_GET_FATURA_FIS_SATIRI_ID_LIST                         ( int  p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);

QList <int> FAT_GET_FAT_IRS_STR_IRSALIYE_STR_ID_LIST                  ( int  p_fis_id );

void    FAT_UPDATE_FATURA_IRSALIYELESTIRILDI_MI                       ( int p_fatura_id, int p_fatura_irsaliyelestirildi_mi );

int     FAT_GET_FATURA_TURU                                           ( int p_fatura_id );

void    FAT_IRSALIYENDIMI_KONTROL                                     ( int p_fis_id );

int     FAT_GET_FATURA_ID                                             ( int p_fatura_satir_id );

int     FAT_IRS_BAGLANTISI_EKLE                                       ( int p_fatura_id, int p_irsaliye_id );

QString FAT_IRS_BAGLANTISI_BILGISI                                    ( int p_fatura_id = -1, int p_irsaliye_id = -1 );

bool    BELGE_SERI_KONTROLU                                           ( QString p_belge_seri, QString p_belge_numarasi , int p_modul_id, int p_cari_hesap_id );

QList<int> FAT_GET_FIS_ID_LIST                                        ( int p_urun_id );

#endif // FAT_CONSOLE_UTILS_H
