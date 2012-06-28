#ifndef CARI_OPEN_H
#define CARI_OPEN_H

#include "cari_odeme_ceksenet_batch_open.h"
#include "cari_hareket_fisi_open.h"
#include "cari_virman_formu_open.h"
#include "cari_hesap_ekstresi_batch_open.h"
#include "cari_hesap_ekstresi_raporu_open.h"
#include "cari_tahsilat_ceksenet_batch_open.h"
#include "cari_kart_arama_open.h"
#include "cari_odeme_ceksenet_batch_open.h"
#include "cari_bakiye_raporu_batch_open.h"
#include "cari_odeme_fisi_open.h"
#include "cari_hesap_ekstresi_batch_open.h"
#include "cari_tahsilat_fisi_open.h"
#include "cari_kart_formu_open.h"
#include "cari_hesap_ekstresi_batch_open.h"


struct MENU_STRUCT;

enum ENUM_MENU_CARI {
        MENU_CARI_FISLER_HEADER                     = 2010,
        MENU_CARI_TAHSILAT_FISI                     = 2011,
        MENU_CARI_ODEME_FISI                        = 2012,
        MENU_CARI_HAREKET_FISI                      = 2013,
        MENU_CARI_VIRMAN_FISI                       = 2014,
        MENU_CARI_ACILIS_FISI                       = 2015,

        MENU_CARI_ENT_FISLERI_HEADER                = 2020,
        MENU_CARI_ENT_FISLERI                       = 2021,

        MENU_CARI_KARTLAR_HEADER                    = 2030,
        MENU_CARI_FIRMA_KARTI                       = 2031,
        MENU_CARI_SAHIS_KARTI                       = 2032,
        MENU_CARI_GRUPLAR                           = 2033,

        MENU_CARI_RAPORLAR_HEADER                   = 2040,
        MENU_CARI_HESAP_EKSTRESI                    = 2041,
        MENU_CARI_BAKIYE_RAPORU                     = 2042,

        MENU_CARI_ISLEMLER_HEADER                   = 2050,
        MENU_CARI_YAZICIDAKI_BLG_NOLARI             = 2051,

        MENU_CARI_KART_EXPORT                       = 2052,
        MENU_CARI_KART_IMPORT                       = 2053
};

void                    CARI_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           CARI_GET_MENU_STRUCT();

#endif // CARI_OPEN_H
