#ifndef E9_OPEN_H
#define E9_OPEN_H

#include "e9_devir_islemi_batch_open.h"
#include "e9_eft_batch_open.h"
#include "e9_ent_detaylari_batch_open.h"
#include "e9_kk_odeme_batch_open.h"
#include "e9_kk_tahsilat_batch_open.h"
#include "e9_kullanici_yetkileri_formu_open.h"
#include "e9_log_kaydi_arama_open.h"
#include "e9_sabit_degerler_formu_open.h"
#include "e9_onar_batch_open.h"

enum ENUM_MENU_E9 {
    MENU_ADRES                           = 9910,
    MENU_BANKA                           = 9915,
    MENU_CARI                            = 9920,
    MENU_CEK                             = 9925,
    MENU_DEMIRBAS                        = 9930,
    MENU_DOVIZ                           = 9935,
    MENU_FAIZ                            = 9940,
    MENU_ISLETME                         = 9945,
    MENU_MUHASEBE                        = 9950,
    MENU_PERSONEL                        = 9955,
    MENU_STOK                            = 9960,
    MENU_IRSALIYE                        = 9965,
    MENU_FATURA                          = 9970,
    MENU_SUBE                            = 9975,
    MENU_PERAKENDE                       = 9980,
    MENU_SMM_MAKBUZ                      = 9990,
    MENU_YONETIM                         = 99100,
    MENU_KULLANIM_KITAPCIGI              = 99200,
    MENU_SIK_SORUNLAN_SORUNLAR           = 99300,
    MENU_HATA_ISTEK                      = 99400,
    MENU_CRM                             = 99500

};


////////////////////////////////YONETIM///////////////////////////////////////////////////////////////

enum ENUM_MENU_YONETIM {
    MENU_YNT_E9_KULLANICI_YETKILERI                   = 10010,
    MENU_YNT_E9_DEVIR_ISLEMI                          = 10011,
    MENU_YNT_E9_ONAR                                  = 10012,
    MENU_YNT_E9_BELGE_TASARIMCISI                     = 10013,
    MENU_YNT_E9_LOG_BILGILERI                         = 10014,
    MENU_YNT_E9_SABIT_DEGERLER                        = 10527,
    MENU_YNT_E9_EMAIL_LOGLARI                         = 10031,
    MENU_YNT_E9_PROFIL_SECME                          = 10032
};

class  QWidget;
class  QString;
struct MENU_STRUCT;

void                    E9_ITEM_CLICKED                             ( int menu_item,QWidget * parent);
MENU_STRUCT *           E9_GET_MENU_STRUCT                          ();

void                    YONETIM_ITEM_CLICKED                        ( int menu_item,QWidget * parent);
MENU_STRUCT *           YONETIM_GET_MENU_STRUCT                     ();

#endif // E9_OPEN_H
