#ifndef STOK_OPEN_H
#define STOK_OPEN_H

#include "stok_detaylar_batch_open.h"
#include "stok_ekstresi_batch_open.h"
#include "stok_ekstresi_raporu_open.h"
#include "stok_fisi_arama_open.h"
#include "stok_hizmet_karti_formu_open.h"
#include "stok_miktarlari_batch_open.h"
#include "stok_miktarlari_raporu_open.h"
#include "stok_urun_arama_open.h"
#include "stok_urun_birimleri_batch_open.h"
#include "stok_urun_karti_formu_open.h"
#include "stok_urun_listesi_batch_open.h"
#include "stok_urun_listesi_raporu_open.h"
#include "stok_urun_seri_no_arama_open.h"
#include "stok_fisi_open.h"

enum ENUM_MENU_STOK {
         MENU_STOK_FISLER_HEADER                           = 6011,
         MENU_STOK_SARF_FISI                               = 6012,
         MENU_STOK_IMALATA_SEVK_FISI                       = 6013,
         MENU_STOK_IMALATTAN_DONUS_FISI                    = 6014,
         MENU_STOK_DEPO_TRANSFER_GIRIS_FISI                = 6015,
         MENU_STOK_DEPO_TRANSFER_CIKIS_FISI                = 6016,
         MENU_STOK_ACILIS_FISI                             = 6017,

         MENU_STOK_TANIMLAR_HEADER                         = 6020,
         MENU_STOK_URUN_KARTI                              = 6021,
         MENU_STOK_HIZMET_KARTI                            = 6022,
         MENU_STOK_GRUPLARI                                = 6023,

         MENU_STOK_RAPORLAR_HEADER                         = 6030,
         MENU_STOK_URUN_LISTESI                            = 6031,
         MENU_STOK_HIZMET_LISTESI                          = 6032,
         MENU_STOK_MIKTARLARI_RAPORU                       = 6033,
         MENU_STOK_EKSTRESI                                = 6034,

         MENU_STOK_ISLEMLER_HEADER                         = 6040,
         MENU_STOK_URUN_IMPORT                             = 6041,
         MENU_STOK_URUN_EXPORT                             = 6042
};


#endif // STOK_OPEN_H
