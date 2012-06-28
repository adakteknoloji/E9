#ifndef LOGIN_H
#define LOGIN_H

#include <QList>
#include "login_enum.h"

class QWidget;
class QDialog;
struct ADAK_SQL_STRUCT;
struct USER_LOGIN_INFO_STRUCT;

int     VERITABANI_SEC          ( int kullanici_id , QString * veritabani_kodu , QString * veritabani_tanimi, QDialog * parent );
int     VERITABANLARINI_GUNCELLE( );

int     GET_LOG_TURU            ( QString log_turu );
QString GET_LOG_TURU            ( int log_turu     );
QString GET_LOG_ISLEMI          ( int log_islemi   );
void    YONETIM_007_KAYIT       ( int log_turu, int log_islemi, QString log_detaylari );

QList<ADAK_SQL_STRUCT * > GET_PROGRAM_DB_STRUCTS();
void                       SET_PROGRAM_DB_STRUCTS( ADAK_SQL_STRUCT * P_SQL_STRUCT );

QList<ADAK_SQL_STRUCT * > GET_YONETIM_DB_STRUCTS();
void                       SET_YONETIM_DB_STRUCTS( ADAK_SQL_STRUCT * P_SQL_STRUCT);

// YONETIM KULLANIYOR
void SET_KULLANICI_ID            ( int kullanici_id   );
void SET_KULLANICI_KODU          ( const QString kullanici_kodu );
void SET_KULLANICI_ADI           ( const QString kullanici_adi );
void SET_BILGISAYAR_ID           ( int bilgisayar_id  );
void SET_BILGISAYAR_KODU         ( const QString bilgisayar_kodu );
void SET_BILGISAYAR_ADI          ( const QString bilgisayar_adi );
void SET_VERITABANI_ID           ( int veritabani_id       );
void SET_VERITABANI_ISMI         ( const QString veritabani_ismi );
void SET_VERITABANI_TANIMI       ( const QString veritabani_tanimi );

int  BILGISAYARDA_KAYITLI_KULLANICI_SAYISINI_BUL         ();
int  KULLANICININ_CALISABILECEGI_VERITABANI_SAYISINI_BUL ();

void SET_KULLANICININ_CALISABILECEGI_VERITABANI_SAYISI   ( int p_veritabani_sayisi);
void SET_BILGISAYARDA_KAYITLI_KULLANICI_SAYISI           ( int p_kullanici_sayisi );
// END YONETIM

// db_name adak_000 ise sadece 000 gonderilmelidir. fonksiyon icinde on_ek ve adak_ ekleniyor.
//! [1]
bool CONNECT_TO_SQL_DATABASE    ( QString sunucu,
                                  int port,
                                  QString sql_motoru,
                                  QString veritabani_on_ek,
                                  QString username,
                                  QString password,
                                  QString db_name,
                                  QString * return_error_text = NULL);

//! [/1] END

#endif // LOGIN_H
