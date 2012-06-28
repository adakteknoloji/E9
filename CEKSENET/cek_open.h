#ifndef CEKSENET_OPEN_H
#define CEKSENET_OPEN_H

#include "cek_acilis_fisi_open.h"
#include "cek_fisi_yazdir_raporu_open.h"
#include "cek_listesi_batch_open.h"
#include "cek_listesi_raporu_open.h"
#include "cek_musteri_cek_senet_arama_open.h"
#include "cek_senet_arama_open.h"
#include "cek_senet_bordrosu_arama_open.h"
#include "cek_senet_bordrosu_fisi_open.h"
#include "cek_senet_formu_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_CEKSENET {
     MENU_CEK_ISLEMLER_HEADER                    = 2501,
     MENU_CEK_KENDI_CEKLERIMIZ                   = 2502,
     MENU_CEK_KENDI_SENETLERIMIZ                 = 2503,
     MENU_CEK_MUS_CEKLERI_SENETLERI              = 2504,

     MENU_CIKIS_BRD_HEADER                       = 2530,
     MENU_MUSTERI_CEK_BORDROSU                   = 2532,
     MENU_FIRMA_CEK_BORDROSU                     = 2533,

     MENU_GIRIS_BRD_HEADER                       = 2520,
     MENU_MUSTERI_SENET_BORDROSU                 = 2522,
     MENU_FIRMA_SENET_BORDROSU                   = 2523,

     MENU_CEK_MUS_SNT_BRD_TAH_EDILEMEYEN         = 2561,

     MENU_CEK_ACILIS_BORDROSU_HEADER             = 2570,
     MENU_CEK_ACILIS_BORDROSU                    = 2571,

     MENU_CEK_ENT_FISLERI_HEADER                 = 2590,
     MENU_CEK_CARI_ENT_FISILERI                  = 2591,


     MENU_CEK_RAPORLAR_HEADER                    = 25100,
     MENU_CEK_SENET_LISTESI                      = 25101,
     MENU_CEK_MUS_CEK_ARAMA                      = 25102,
     MENU_CEK_MUS_SNT_ARAMA                      = 25103
};


void                    CEKSENET_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           CEKSENET_GET_MENU_STRUCT();

#endif // CEKSENET_OPEN_H
