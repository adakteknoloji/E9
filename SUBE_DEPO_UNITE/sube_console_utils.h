#ifndef SUBE_CONSOLE_UTILS_H
#define SUBE_CONSOLE_UTILS_H

#include <QList>
#include <QMap>

class QString;
class QWidget;

struct SUBE_UNITE_BILGILERI_STRUCT;

void            SUBE_CREATE_DEFAULT_SUBE_DEPO_UNITE_ENT_HESAPLARI       ();


int             SUBE_GET_DEPO_ISLEM_YETKISI_ENUM      ( QString p_depo_islem_yetkisi );
QString         SUBE_GET_DEPO_ISLEM_YETKISI_STRING    ( int p_depo_islem_yetkisi );

int             SUBE_GET_ISLEM_YETKISI_ENUM           ( QString p_sube_islem_yetkisi );
QString         SUBE_GET_ISLEM_YETKISI_STRING         ( int p_sube_islem_yetkisi );

int             SUBE_GET_DEPO_KODU_ADI                ( int p_depo_id, QString * p_depo_kodu = NULL , QString * p_depo_adi = NULL );
int             SUBE_GET_UNITENIN_DEPOSU              ();

bool            SUBE_SET_CURRENT_SUBE_UNITE           ();

void     *      SUBE_GET_ENTEGRASYON_HESAPLARI_STRUCT ();
void            SUBE_FILL_ENTEGRASYON_HESAPLARI_STRUCT();

int             SUBE_GET_UNITE_MUH_KASA_HESABI_ID     ();


int             SUBE_GET_UNITE_ID                     ();
void            SUBE_SET_UNITE_ID                     ( int p_value );
int             SUBE_GET_SUBE_ID                      ();
void            SUBE_SET_SUBE_ID                      ( int p_value );

int             SUBE_GET_DEPO_ISLEM_YETKISI           ();
int             SUBE_GET_SUBE_ISLEM_YETKISI           ();


int             SUBE_GET_DEPO_ID                      ( QString p_depo_kodu, int p_sube_id = 0 );
QString         SUBE_GET_DEPO_KODU                    ( int     p_depo_id   );

bool            SUBE_DEPO_ISLEM_YETKI_KONTROLU        ( QWidget * p_parent );

int             SUBE_GET_DEPO_ID_ADI                  ( QString p_depo_kodu, int * p_depo_id = NULL, QString * p_depo_adi = NULL );

int             SUBE_GET_SUBE_ID_ADI                  ( QString p_sube_kodu, int * p_sube_id = NULL , QString * p_sube_adi = NULL );
int             SUBE_GET_SUBE_KODU_ADI                ( int p_sube_id, QString * p_sube_kodu = NULL , QString * p_sube_adi = NULL );

QList<int>      SUBE_GET_SUBE_ID_LIST                 ();
// birin indis  keys   = sube_kodu
// ikinci indis values = sube_adi
QMap <QString, QString> SUBE_GET_SUBE_KODU_ADI_MAP    ();


int            SUBE_UNITE_BILGILERI_OKU               ( SUBE_UNITE_BILGILERI_STRUCT * P_SUBE_UNITE , int p_unite_id );

void           SUBE_UPDATE_IRS_BELGE_NO               ( int p_unite_id, QString p_irsaliye_belge_numarasi );
void           SUBE_UPDATE_FAT_BELGE_NO               ( int p_unite_id, QString p_fatura_belge_numarasi );
void           SUBE_UPDATE_IRSALIYELI_FAT_BELGE_NO    ( int p_unite_id, QString p_fatura_belge_numarasi );
void           SUBE_UPDATE_ODM_MKBZ_BELGE_NO          ( int p_unite_id, QString p_odeme_makbuzu_belge_numarasi );
void           SUBE_UPDATE_TAH_MKBZ_BELGE_NO          ( int p_unite_id, QString p_tahsilat_makbuzu_belge_numarasi );

int            SUBE_GET_SUBE_ENT_HESAP_ID             ( int p_sube_id, QString p_column_name , QString * p_muh_hesap_kodu = NULL, QString * p_muh_hesap_adi = NULL );

QString        SUBE_GET_SUBE_SGK_NO                   ( int p_sube_id );

QString        SUBE_GET_SUBE_ADRES_BILGILERI          ( int p_sube_id );

int            SUBE_GET_DEPO_COUNT                    ( int p_sube_id );

#endif // SUBE_CONSOLE_UTILS_H
