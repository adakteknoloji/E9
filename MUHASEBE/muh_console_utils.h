#ifndef MUH_CONSOLE_UTILS_H
#define MUH_CONSOLE_UTILS_H

class ADAK_SQL;
class QString;
class QStringList;

struct MUH_HESAP_STRUCT;

void            MUH_HESABININ_BAKIYELERINI_ARTTIR           ( int hesap_id , int gun_no , double borc_tutari ,
                                                              double alacak_tutari, ADAK_SQL * P_DB = NULL );
void            MUH_HESABININ_BAKIYELERINI_AZALT            ( int hesap_id , int gun_no , double borc_tutari ,
                                                              double alacak_tutari, ADAK_SQL * P_DB = NULL );

int             MUH_HESABI_TALI_HESAP_MI                    ( int hesap_id );

QString         MUH_GET_HESAP_ISMI                          ( int p_hesap_id );
QString         MUH_GET_HESAP_ISMI                          ( QString tam_hesap_kodu );
QString         MUH_GET_HESAP_KODU                          ( int p_hesap_id );
int             MUH_GET_HESAP_ID                            ( QString hesap_kodu );
void            MUH_GET_HESAP_KODU_HESAP_ISMI               ( int hesap_id, QString &tam_hesap_kodu, QString &hesap_ismi );
int             MUH_GET_HESAP_ID_HESAP_ISMI                 ( QString tam_hesap_kodu, int &hesap_id , QString &hesap_ismi);

int             MUH_GET_KOK_HESAP_ID                        ( int hesap_id );

int             MUH_SIRADAKI_FIS_NO_AL                      ( QString p_fis_tarihi ,ADAK_SQL * P_GELEN_DB = NULL);

int             MUH_GET_PARENT_HESAP_ID                     ( int hesap_id );

QStringList     MUH_GET_HESAPLAR                            ( const QString p_baslangic_hesabi = "", const QString p_bitis_hesabi = "" );
QString         MUH_FIND_AND_GET_HESAP_BILGILERI            ( int     hesap_id      ,QStringList muh_hesaplar,bool idnin_kok_hesabini_bul = false);
QString         MUH_FIND_AND_GET_HESAP_BILGILERI            ( QString tam_hesap_kodu,QStringList muh_hesaplar,bool idnin_kok_hesabini_bul = false);

void            MUH_CREATE_MUHASEBE_HESAPLARI               ();

void            MUH_CREATE_YANSITMA_HESAPLARI               ();

int             MUH_GET_FIS_TURU                             ( int fis_id );

QString         MUH_GET_BORC_ARRAY                           ( int hesap_id );
QString         MUH_GET_ALACAK_ARRAY                         ( int hesap_id );

void            MUH_ALT_HESAPLARIN_TAM_HESAP_KODUNU_GUNCELLE ( int hesap_id, QString tam_hesap_kodu, QString delimeter);

QString         MUH_GET_FIS_TURLERI_STRING                   ( int fis_turu     );
int             MUH_GET_FIS_TURLERI_ENUM                     ( QString fis_turu );

int             MUH_GET_ENTEGRASYON_FIS_KAYNAK_MODUL_ENUM    ( QString fis_turu );
QString         MUH_GET_ENTEGRASYON_FIS_KAYNAK_MODUL_STRING  ( int fis_kaynagi );

int             MUH_GET_ENTEGRASYON_FIS_TURLERI_ENUM         ( QString fis_turu );
QString         MUH_GET_ENTEGRASYON_FIS_TURLERI_STRING       ( int fis_turu     );

void            MUH_GET_FROM_GELIR_TABLOSU_DONEM_NET_KARI_ZARARI( double &donem_kari,double & donem_zarari , int gun_no);
void            MUH_GET_FROM_GIDER_HESABI_ISLENMEMIS_BAKIYE     ( int gelir_tablosu_hesabi_id , double &borc_bakiye , double &alacak_bakiye , int gun_no);

int             MUH_GET_TALI_HESAP_ID                       ( QString tam_hesap_kodu );

void            MUH_UPDATE_FIS_ACIKLAMA                     ( int p_muh_fis_id, QString p_aciklama ) ;

int             MUH_GET_FIS_LAST_ORDER_NUMBER               ( int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);

void            MUH_UPDATE_FIS_BASE_FIS_ID                  ( int p_muh_fis_id, int p_base_fis_id );

int             MUH_HESAPLAR_TABLOSUNU_OKU                  ( MUH_HESAP_STRUCT * P_HESAP , int p_hesap_id);

int             MUH_HESAPLAR_TABLOSUNU_OKU                  ( MUH_HESAP_STRUCT * P_HESAP , QString p_tam_hesap_kodu);

void            MUH_CLEAR_HESAP_STRUCT                      ( MUH_HESAP_STRUCT * P_HESAP );

#endif // MUH_CONSOLE_UTILS_H
