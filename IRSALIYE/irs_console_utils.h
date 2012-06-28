#ifndef IRS_CONSOLE_UTILS_H
#define IRS_CONSOLE_UTILS_H

class    QString;
class    QDate;
class    ADAK_SQL;

struct   IRSALIYE_RAPOR_VARS;
struct   FATURA_FISI_STRUCT;


void        IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE     ( int    p_irsaliye_satiri_id,
                                                                    double   p_faturalanan_miktar,
                                                                    int      p_artir_azalt , ADAK_SQL * P_GELEN_DB = NULL);

void        IRS_IRSALIYE_SATIRINDA_IADE_MIKTARINI_GUNCELLE          ( int p_irsaliye_satiri_id, double p_iade_edilen_miktar, int p_artir_azalt );

void        IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_SIFIRLA      ( int p_irsaliye_satiri_id );

int         IRS_GET_FATURA_DURUMU_ENUM                              ( QString fatura_durumu );

QString     IRS_GET_FATURA_DURUMU_STRING                            ( int fatura_durumu );

void        IRS_SATIRINDA_TOPLU_ISKONTO_SONRASI_TUTARLARI_GUNCELE   ( int p_fatura_satiri_id );

void        IRS_IRSALIYEDE_TOPLU_ISKONTO_SONRASI_TUTARLARI_GUNCELE  ( FATURA_FISI_STRUCT * P_FIS, int p_fatura_id );

IRSALIYE_RAPOR_VARS * GET_STRUCT_IRS_RAPOR_VARS                     ();

bool        IRS_GET_IRSALIYE_YAZDIRILDIMI                           ( int p_irsaliye_id );

void        IRS_UPDATE_BELGE_SERI_VE_NUMARASI                       ( int p_irsaliye_id, QString p_belge_seri, QString p_belge_numarasi );

void        IRS_FATURALANDIMI_KONTROL                               ( int p_fis_id );

void        IRS_IRSALIYE_FATURALANMIS_DURUMUNU_GUNCELLE             ( int p_irsaliye_id,  int p_durum = 0  );

QList<int>  IRS_GET_IRSALIYE_ID_LIST                                ( int p_cari_hesap_id);

#endif // IRS_CONSOLE_UTILS_H
