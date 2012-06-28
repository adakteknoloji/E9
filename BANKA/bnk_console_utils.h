#ifndef BNK_CONSOLE_UTILS_H
#define BNK_CONSOLE_UTILS_H


class ADAK_SQL;
class QString;
class QStringList;
class QByteArray;

struct      BNK_BANKA_BILGILERI_STRUCT;
struct      BNK_MUH_HESAPLARI_STRUCT;
struct      BNK_RAPOR_VARS;



int         BNK_SIRADAKI_MAKRO_FIS_NO_AL                ( QString p_fis_tarihi );
int         BNK_SIRADAKI_HAREKET_FIS_NO_AL              ( QString p_fis_tarihi , ADAK_SQL * P_GELEN_DB = NULL);
int         BNK_SIRADAKI_DEFTER_FIS_NO_AL               ( QString p_fis_tarihi , ADAK_SQL * P_GELEN_DB = NULL);
int         BNK_SIRADAKI_VIRMAN_FIS_NO_AL               ( QString p_fis_tarihi , ADAK_SQL * P_GELEN_DB = NULL);

void        BNK_CREATE_BANKA_MAKROLARI                  ();

int         BNK_GET_POS_ID                              ( QString pos_numarasi );
QString     BNK_GET_POS_NUMARASI                        ( int pos_id );

int         BNK_GET_HESAP_NUMARASI_ID                   ( QString hesap_numarasi );
QString     BNK_GET_HESAP_NUMARASI                      ( int hesap_no_id );

QString     BNK_GET_BANKA_KODU                          ( int p_banka_id );
QString     BNK_GET_BANKA_ADI_SUBE_KODU_ADI_HESABI_NO   ( int p_banka_id );


QString     BNK_GET_KREDI_KARTI_NUMARASI                ( int kredi_karti_id );

void        BANKA_HESAP_BAKIYELERINI_ARTTIR             ( int gun_no , int hesap_no_id, double borc_tutari ,
                                                          double alacak_tutari,int bakiye_turu, ADAK_SQL * P_GELEN_DB = NULL);

void        BANKA_HESAP_BAKIYELERINI_AZALT              ( int gun_no , int hesap_no_id, double borc_tutari ,
                                                          double alacak_tutari,int bakiye_turu, ADAK_SQL * P_GELEN_DB = NULL);

int         BNK_GET_PARA_BIRIM_ID                       ( int p_banka_id );

int         BNK_GET_HESABA_ETKISI_ENUM                  ( QString hesaba_etkisi_str );
QString     BNK_GET_HESAP_ETKISI_STRING                 ( int hesaba_etkisi_enum    );

int         BNK_GET_HESAP_TURU_ENUM                     ( QString hesap_tur_adi     );
QString     BNK_GET_HESAP_TURU_STRING                   ( int hesap_tur_enum        );

int         BNK_GET_ROUND_SEKLI_ENUM                    ( QString p_round_sekli     );
QString     BNK_GET_ROUND_SEKLI_STRING                  ( int p_round_sekli        );

int         BNK_GET_BANKA_MUH_HESAP_ID                  ( int p_hesap_no_id );

QStringList BNK_GET_BANKA_ISIMLERI                      ( int p_banka_hesap_id = 0);

void        BNK_GET_BANKA_TEMEL_BILGILER                ( BNK_BANKA_BILGILERI_STRUCT * P_BNK , int p_hesap_no_id );

void        BNK_GET_BANKA_MUH_HESAPLARI                 ( BNK_MUH_HESAPLARI_STRUCT * P_BNK_MUH , int p_hesap_no_id );

QString     BNK_GET_FIS_TURLERI_STRING                  ( int bnk_fis_turu);

int         BNK_GET_ENT_FIS_MODUL_ID                    ( QString p_combo_box_text );

#endif // BNK_CONSOLE_UTILS_H
