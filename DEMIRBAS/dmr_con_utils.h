#ifndef DMR_CON_UTILS_H
#define DMR_CON_UTILS_H


class QString;
class QStringList;
class ADAK_SQL;

struct  DMR_RAPOR_VARS;
struct  DMR_TEMEL_BILGILER_STRUCT;




void            DMR_GET_DEMIRBAS_KODU_ADI                   ( int     demirbas_id, QString &demirbas_kodu,
                                                             QString &demirbas_ismi);

int             DMR_GET_DEMIRBAS_ID                         ( QString demirbas_kodu );

void            DMR_AMORTISMAN_TUTARINI_ARTTIR              ( int demirbas_id, int gun_no, double amortisman_tutari );

void            DMR_AMORTISMAN_TUTARINI_AZALT               ( int demirbas_id, int gun_no, double amortisman_tutari );

double          DMR_GET_BIRIKMIS_AMORTISMAN_TUTARI          ( int demirbas_id, int gun_no , ADAK_SQL * P_GELEN_DB = NULL);

int             DMR_SIRADAKI_FIS_NO_AL                      ( QString p_fis_tarihi );

bool            DMR_FIS_NO_KULLANILDI_MI                    ( int p_fis_no, QString p_fis_tarihi );

void            DMR_ENTEGRASYON_FISI_OLUSTUR                (int fatura_id , int demirbas_id = 0 , ADAK_SQL * P_GELEN_DB = NULL);

DMR_RAPOR_VARS * DMR_GET_RAPOR_VARS                         ();

QString         DMR_GET_FATURA_BILGILERI                    (int fatura_id);

void            DMR_UPDATE_PERSONEL_DEMIRBAS_DURUM          ( int p_personel_id, int durum );

void            DMR_GET_DEMIRBAS_BILGILERI                  ( DMR_TEMEL_BILGILER_STRUCT * P_DMR , int p_demirbas_id );

double          DMR_GET_AYRILACAK_AMORTISMAN_ORANI          ( int dmr_alis_month);

void            DMR_CREATE_DEMIRBAS_GRUPLARI                ();

int             DMR_GET_AMORTISMAN_SEKLI_ENUM               ( QString amortisman_sekli);

QString         DMR_GET_AMORTISMAN_SEKLI_STRING             ( int amortisman_sekli);

QStringList     DMR_GET_ID_KODU_ADI_LIST                    ( int personel_id );

#endif // DMR_CON_UTILS_H
