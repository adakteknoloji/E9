#ifndef ADRES_CONSOLE_UTILS_H
#define ADRES_CONSOLE_UTILS_H

#include <QMap>
#include "adak_defines.h"

class QString;
class QStringList;
class QByteArray;

struct ADRES_HESAP_STRUCT;


int         ADR_KART_OLUSTUR                              ( ADRES_HESAP_STRUCT * P_ADR_STRUCT );
void        ADR_KART_GUNCELLE                             ( ADRES_HESAP_STRUCT * P_ADR_STRUCT , int p_adres_id );
void        ADR_KART_BILGILERINI_OKU                      ( ADRES_HESAP_STRUCT * P_ADR_STRUCT , int p_adres_id );
void        ADR_KARTI_SIL                                 ( int p_adres_id );
void        ADR_CLEAR_ADRES_STRUCT                        ( ADRES_HESAP_STRUCT * P_ADR_STRUCT );

void        ADR_KART_FILTRE_STR_UPDATE                    ( int p_adres_id, QString p_filtre_str );

QStringList ADR_GET_ADRES_SEHIR_ILCE_GORE_ID_LIST         ( QString p_sehir = NULL , QString p_ilce = NULL );

QStringList ADR_GET_ADRES_ID_LIST                         ( int p_kart_turu = -1, int p_modul_id = -1 ,int p_program_id = -1 );

int         ADR_GET_ADRES_KART_TURU (int p_adres_id);
/*
    QStringList personel_sehir_bilg;
    personel_sehir_bilg.at(0); // sehir
    personel_sehir_bilg.at(1); // ilce
    personel_sehir_bilg.at(2); // semt

 */
QStringList ADR_GET_ADRES_SEHIR_ILCE_SEMT_LIST              ( int p_adres_id );


int         ADR_GET_ADRES_VERGI_NO_VERGI_DAIRESI            ( int p_adres_id, QString *p_vergi_no, QString *p_vergi_dairesi );

// Filtre uyan degerleri liste olarak geri donderir.
QStringList ADR_GET_ADRES_FILTRE_STR                        ( QString p_filtre_str );


QString     ADR_GET_USER_MAIL                               ();

int         ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR         ( QString p_tel_ulke_kodu, QString p_tel_alan_kodu, QString p_telefon,  int p_kayit_id ,  QString p_tel_dahili = "" );
void        ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE      ( QString p_tel_ulke_kodu, QString p_tel_alan_kodu, QString p_telefon,  int p_telefon_id, QString p_tel_dahili = "" );

void        ADR_EMAIL_FISINDEKI_GIZLI_KAYDI_GUNCELLE        ( QString p_email_adresi, int p_email_id );
int         ADR_EMAIL_FISINDE_GIZLI_KAYIT_OLUSTUR           ( QString p_email_adresi, int p_kayit_id );


int         ADR_GET_KART_TURU_ENUM                              ( QString kart_turu );
QString     ADR_GET_KART_TURU_STRING                            ( int kart_turu );


QString     ADR_GET_ADRES_BILGILERI                             ( int p_adres_id );

int         ADR_GET_ADRES_VERGI_NO_KIMLIK_NO_ULKE               ( int p_adres_id, QString *p_vergi_no = NULL, QString * p_kimlik_no = NULL, QString * p_ulke = NULL );

QString     ADR_GET_FIRMA_SAHIS_ADI                             ( int p_adres_id);

QStringList ADR_GET_ADRES_ID_LIST                               ( QString p_firma_sahis_adi);

QStringList ADR_GET_ADRES_TEL_FILTRE_LIST                       ( QString p_telefon );

QStringList ADR_GET_ADRES_SEKTOR_OR_MESLEK_FILTRE_LIST          ( QString p_sektor_or_meslek , ADAK_ADRES_CARI_ENUM p_kart_turu );

#endif // ADRES_CONSOLE_UTILS_H
