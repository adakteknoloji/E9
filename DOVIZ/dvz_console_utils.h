#ifndef DVZ_CON_UTILS_H
#define DVZ_CON_UTILS_H

#include <QStringList>
#include <QDate>
#include <QSplashScreen>
#include "dvz_enum.h"

struct DOVIZ_BILGILERI_STRUCT ;



void                            DVZ_SET_TEMEL_PARA_BIRIMI_ID                            ( int para_birimi_id );

int                             DVZ_GET_DOVIZ_ID                                        ( QString doviz_kodu , bool init = false );
QString                         DVZ_GET_DOVIZ_KODU                                      ( int doviz_id       );
QString                         DVZ_GET_DOVIZ_ADI                                       ( int doviz_id       );

int                             DVZ_GET_TEMEL_PARA_BIRIMI_ID                            ();
QString                         DVZ_GET_TEMEL_PARA_BIRIMI_KODU                          ();

double                          DVZ_PARA_BIRIMLERINI_DONUSTUR                           ( int paranin_birimi, double cevrilecek_tutar,int cevrilecek_para_birimi, QDate kur_tarihi, int kur_turu = ENUM_SATIS );

double                          DVZ_DOVIZ_KURUNU_AL                                     ( int p_doviz_id, QDate p_kur_tarihi, int p_kur_turu = ENUM_SATIS );
double                          DVZ_PARITEYI_AL                                         ( int p_donusturulecek_para_birimi_id, int p_baz_alinacak_para_birimi_id, QDate p_kur_tarihi, int p_kur_turu );

//Dovizleri gunceller.Ve bir listesini return eder. Burda update_db true ise
//databasede guncellenir.Aksi takdirde sadece stringlist doner.

void                            DVZ_DOVIZLERI_GUNCELLE                                  ( int host = ENUM_TCMB_XML,QDate date = QDate());
bool                            DVZ_DOVIZLERI_EKLE                                      ();

bool                            DVZ_GET_DOVIZ_BILGILERI_INTERNET                        (QDate date , int host);
bool                            DVZ_GET_DOVIZ_BILGILERI_DATABASE                        (QDate date_orj_kullanilacak, QDate date_kopyalanacak);

void                            DVZ_CREATE_DOVIZ_KURLAR                                 (QStringList dovizler);
void                            DVZ_UPDATE_DOVIZ_KURLAR                                 ( QString doviz_kodu, QDate tarih , double kullanici_alis_fiyati = -1.0 , double kullanici_satis_fiyati = -1.0 );

void                            DVZ_DOVIZLER_STRUCT_DOLDUR                              ();

void                            DVZ_SET_XML_TEMEL_PARA_BIRIMI_ID                        (int host);
int                             DVZ_GET_XML_TEMEL_PARA_BIRIMI_ID                        ();

QStringList                     DVZ_FIND_DOVIZLER_FROM_XML                              (QByteArray xml,QDate date);

void                            DVZ_CLEAR_DOVIZLER_STRUCT                               ();

void                            DVZ_START_DOVIZ_TRANSACTION                             ();
void                            DVZ_COMMIT_DOVIZ_TRANSACTION                            ();
void                            DVZ_CANCEL_DOVIZ_TRANSACTION                            ();

void                            DVZ_DOVIZ_SPLASH_ON(QSplashScreen * splash , QString mesaj);
void                            DVZ_DOVIZ_SPLASH_OFF(QSplashScreen * splash);

void                            DVZ_GET_DOVIZ_BILGILERI                           ( DOVIZ_BILGILERI_STRUCT * P_DOVIZ, int p_doviz_id );


#endif // DVZ_CON_UTILS_H
