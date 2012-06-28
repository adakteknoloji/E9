#ifndef YONETIM_H
#define YONETIM_H

#include <QMainWindow>
#include "adak_sql.h"
#include "adak_sql_struct.h"
#include "adak_defines.h"

struct MENU_STRUCT;

void            OPEN_HATA_ISTEK_BATCH                         ( QWidget * parent );

void            OPEN_SIFRE_DEGISTIRME_BATCH                   ( QWidget * parent          );
bool            OPEN_KULLANICILAR_FORMU                       ( void (*F_PROGRAM_KULLANICI_YETKILERI_FONK) ( int,bool *, QWidget *), QWidget * parent );
void            OPEN_BILGISAYARLAR_FORMU                      ( QWidget * parent          );
void            OPEN_VERITABANLARI_FORMU                      ( QWidget * parent          );
void            OPEN_POSTA_SUNUCUSU_FORMU                     ( QWidget * parent          );
void            OPEN_SMS_SUNUCUSU_FORMU                       ( QWidget * parent          );
void            OPEN_YONETIM_007_ARAMA                        ( QWidget * parent          );

void            INIT_PROGRAM                                  (ADAK_SQL_STRUCT * p_program_struct , void F_KULLANICI_TAM_YETKI_FONK(void));
int             PROGRAMA_GIRIS                                (int argc, char *argv[] , int display_id , int program_id, QWidget * parent);

void            CHECK_VERSION_UPGRADES                        (QString program_version_column_name ,
                                                               QString program_version_db_name,
                                                               void UPGRADE_TO_VERSION_FUNC(QString) );

int             KULLANICI_SEC                                 ( bool yonetici_goster, QDialog * parent, QStringList * p_varolan_kullanici_id_list = NULL );
int             BILGISAYAR_SEC                                ( QDialog * parent );
int             PROGRAM_SEC                                   ( QDialog * parent , bool check_exe = false);
void            YONETIM_007_CIKIS_KAYDI                       ();

int             KULLANICI_ID                                  ( void );
QString         KULLANICI_KODU                                ( void );
QString         KULLANICI_ADI                                 ( void );
int             BILGISAYAR_ID                                 ( void );
QString         BILGISAYAR_KODU                               ( void );
QString         BILGISAYAR_ADI                                ( void );
int             VERITABANI_ID                                 ( void );
QString         VERITABANI_ISMI                               ( void );
QString         VERITABANI_TANIMI                             ( void );
QString         VERITABANI_TANIMI                             ( QString veritabani_kodu );

int             GET_KULLANICININ_CALISABILECEGI_VERITABANI_SAYISI  ();
int             GET_BILGISAYARDA_KAYITLI_KULLANICI_SAYISI          ();

int             VERITABANI_DEGISTIR                           ( ADAK_SQL ** p_program_db, QWidget * p_parent );
int             KULLANICI_DEGISTIR                            ( QWidget * p_parent );

enum ENUM_MENU_SISTEM {
    MENU_SIS_KULLANICILAR                             = 20015,
    MENU_SIS_BILGISAYARLAR                            = 20016,
    MENU_SIS_VERITABANLARI                            = 20017,
    MENU_SIS_YONETIM_LOG_BILGILERI                    = 20018,
    MENU_SIS_KULLANICI_DEGISTIR                       = 20019,
    MENU_SIS_VERITABANI_DEGISTIR                      = 20020,
    MENU_SIS_SIFRE_DEGISTIR                           = 20022,
    MENU_SIS_POSTA_SUNUCUSU                           = 20023,
    MENU_SIS_SMS_SUNUCUSU                             = 20024,
    MENU_SIS_OTOMATIK_GIRIS_KAPAT                     = 20025,
    MENU_SIS_OTOMATIK_SIFRE_URET                      = 20026,
    MENU_SIS_HAKKIMIZDA                               = 20511,
    MENU_SIS_MENU_AYARLARI                            = 20512
};

struct SYSTEM_MENU_ITEMS {

    int        sis_blg_formuna_girebilir_mi;
    int        sis_kul_formuna_girebilir_mi;
    int        sis_veritabani_formuna_girebilir_mi;
    int        sis_yonetim_log_inceleme_yetkisi;
    int        sis_mail_srv_tanimlayabilirmi;
    int        sis_sms_srv_tanimlayabilirmi;
    int        sis_otomatik_giris_gorunsun_mu;
};



void            SISTEM_ITEM_CLICKED                           ( int menu_item,QWidget * parent, void (*F_PROGRAM_KULLANICI_YETKILERI_FONK) ( int,bool *, QWidget *) );
MENU_STRUCT *   SISTEM_GET_MENU_STRUCT                        ();


#endif // YONETIM_H
