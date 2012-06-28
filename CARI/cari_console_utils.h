#ifndef CARI_CON_UTILS_H
#define CARI_CON_UTILS_H

#include <QList>

class ADAK_SQL;
class QString;

struct CARI_RAPOR_VARS;
struct CARI_HESAP_STRUCT;
struct ADRES_HESAP_STRUCT;
struct BORC_BILGISI;
struct CARI_RAPOR_VARS;

int         CARI_KART_OLUSTUR                                   ( int cari_kart_turu, CARI_HESAP_STRUCT * kayit_bilgileri                      );
bool        CARI_KART_GUNCELLE                                  ( int cari_hesap_id , int cari_kart_turu , CARI_HESAP_STRUCT * kayit_bilgileri );
bool        CARI_KART_SIL                                       ( int cari_hesap_id );
void        CARI_KART_BILGILERINI_OKU                           ( int cari_hesap_id , CARI_HESAP_STRUCT * kayit_bilgileri                      );
void        CARI_SET_HESAP_STRUCT_DEFAULTS                      ( CARI_HESAP_STRUCT * cari_hesap_struct );

int         CARI_FIND_RECORD                                    ( QString cari_hesap_kodu,int kayit_turu,int program_id,int modul_id);
int         CARI_FIND_FIRST_RECORD                              ( int kayit_turu,int program_id,int modul_id);
int         CARI_FIND_LAST_RECORD                               ( int kayit_turu,int program_id,int modul_id);
int         CARI_FIND_PREV_RECORD                               ( QString cari_hesap_kodu,int kayit_turu,int program_id,int modul_id);
int         CARI_FIND_NEXT_RECORD                               ( QString cari_hesap_kodu,int kayit_turu,int program_id,int modul_id);

int         CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID              (  int cari_hesap_id, QString *cari_hesap_kodu = NULL, QString *cari_hesap_ismi = NULL, int *para_birim_id = NULL );
int         CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID              (  QString cari_hesap_kodu,  int *cari_hesap_id = NULL, QString *cari_hesap_ismi = NULL, int *para_birim_id = NULL  );

QString     CARI_GET_FIRMA_SAHIS_ADI                            (int cari_hesap_id);

int         CARI_SIRADAKI_FIS_NO_AL                             ( QString p_fis_tarihi,ADAK_SQL * P_GELEN_DB = NULL );
bool        CARI_FIS_NO_KULLANILDI_MI                           ( int fis_no, QString fis_tarihi );
bool        CARI_HESAP_KODU_KULLANILDI_MI                       ( QString hesap_kodu );

int         CARI_GET_ADRES_RECORD_ID                            ( int cari_hesap_id);

QString     CARI_GET_FIS_TURLERI_STRING                         ( int fis_turu);


int         CARI_FIND_HESAP_ID                                  ( QString cari_hesap_kodu );
QString     CARI_FIND_HESAP_KODU                                ( int cari_hesap_id );
QString     CARI_FIND_HESAP_ISMI                                ( int cari_hesap_id );
int         CARI_FIND_HESAP_MUH_HESAP_ID                        ( int p_cari_hesap_id );

void        CARI_HESABIN_BAKIYESINI_ARTTIR                      ( int gun_no,int cari_hesap_id,double doviz_borc_tutari,double doviz_alacak_tutari, ADAK_SQL * P_GELEN_DB = NULL );

void        CARI_HESABIN_BAKIYESINI_AZALT                       ( int gun_no,int cari_hesap_id,double doviz_borc_tutari,double doviz_alacak_tutari, ADAK_SQL * P_GELEN_DB = NULL);


double      CARI_GET_PARA_BIRIMI_TUTARI                         (int cari_para_birimi_id,int fis_para_birimi_id,
                                                                 double doviz_kuru,double parite,double tutar);

bool        CARI_HESAP_ISLEM_GORDU_MU                           (int cari_hesap_id);

int         CARI_KART_BORC_BILGISINI_GETIR                      ( int cari_hesap_id , BORC_BILGISI * borc_bilgisi);

int         CARI_GET_PARA_BIRIM_ID                              ( int cari_hesap_id );

int         CARI_GET_MUH_HESAP_ID                               ( int cari_hesap_id );

int         CARI_GET_VERGI_NO_VERGI_DAIRESI                     ( int p_cari_hesap_id, QString * p_vergi_no, QString * p_vergi_dairesi );

QString     CARI_GET_ADRES_BILGILERI                            ( int p_cari_hesap_id );

int         CARI_GET_MUHASEBE_ID_AND_PARA_BIRIM_ID              ( int p_cari_hesap_id, int * p_muh_hesap_id, int * p_para_birim_id );

int         CARI_GET_VERGI_NO_KIMLIK_NO_ULKE                    ( int cari_hesap_id , QString * p_vergi_no, QString * p_kimlik_no, QString * p_ulke );

QList < int > CARI_GET_FIS_SATIRLAR_CEK_SENET_ID_LIST           ( int p_fis_id , int cek_veya_senet );

QList < int > CARI_GET_CEK_SENET_ID_LIST                        ( int p_cari_hesap_id , int modul_id );

// Personel Cari modulun disindakiler icin hazirlandi silinen kayitlar 'DELETED' olarak isaretlendiriyor.
void        CARI_HESAP_FILTRE_STR_UPDATE                        ( int p_cari_hesap_id, QString p_filtre_str );

int         CARI_GET_CARI_KART_TIPI_ENUM                        ( QString p_cari_kart_tipi );

QString     CARI_GET_CARI_KART_TIPI_STRING                      ( int   p_cari_kart_tipi   );

QList < int > CARI_GET_CARI_HESAP_ID_LIST                       ( int p_program_id, int p_modul_id, QList < int > p_base_record_id_list );

QString     CARI_GET_SM_SICIL_NUMARASI                          ( int p_cari_hesap_id );

void        CARI_UPDATE_SM_SICIL_NUMARASI                       ( int p_cari_hesap_id, QString p_sm_sicil_numarasi );


int         CARI_GET_MUH_FIS_ID_LIST                            ( int p_program_id, int p_modul_id, int p_base_fis_id );

int         CARI_GET_ENT_FIS_MODUL_ID                           ( QString p_ent_basligi );

void        CARI_COPY_STRUCT_CARI_2_ADRES                       ( CARI_HESAP_STRUCT * P_CARI_STRUCT, ADRES_HESAP_STRUCT *P_ADRES_STRUCT );


#endif // CARI_CON_UTILS_H
