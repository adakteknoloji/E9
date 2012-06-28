#ifndef FATURA_OPEN_H
#define FATURA_OPEN_H

#include "fat_ba_bs_formu_listesi_batch_open.h"
#include "fat_ba_bs_formu_listesi_raporu_open.h"
#include "fat_fatura_arama_open.h"
#include "fat_fatura_belgesi_open.h"
#include "fat_fatura_detaylari_batch_open.h"
#include "fat_fatura_listesi_batch_open.h"
#include "fat_fatura_listesi_raporu_open.h"
#include "fat_hizmet_ekstresi_batch_open.h"
#include "fat_hizmet_ekstresi_raporu_open.h"
#include "fat_iade_fatura_secimi_open.h"
#include "fat_iade_satir_secimi_open.h"
#include "fat_irsaliye_satir_secimi_open.h"
#include "fat_toplu_faturalastir_arama_open.h"
#include "fat_fatura_fisi_open.h"


struct MENU_STRUCT;

enum ENUM_MENU_FATURA {
         MENU_FAT_SATIS_FAT_HEADER                              = 7011,
         MENU_FAT_SATIS_FATURALARI                              = 7012,
         MENU_FAT_ALISTAN_IADE                                  = 7013,
         MENU_FAT_TOPLU_IRS_FATURALASTIRMA                      = 7014,

         MENU_FAT_ALIS_FAT_HEADER                               = 7030,
         MENU_FAT_ALIS_FATURALARI                               = 7031,
         MENU_FAT_SATISTAN_IADE                                 = 7032,

         MENU_FAT_ISLEM_HEADER                                  = 7020,

         MENU_FAT_SATIS_FAT_ISLEM_HEADER                        = 7021,
         MENU_FAT_SATIS_ELLE_IRS_FATURALASTIRMA                 = 7022,
         MENU_FAT_SATIS_OTOMATIK_IRS_FATURALASTIRMA             = 7023,
         MENU_FAT_SATIS_ELLE_IRS_FATURALASTIRMA_KONSINYE        = 7024,
         MENU_FAT_YAZICIDAKI_BELGE_NOLARI                       = 7054,

         MENU_FAT_ALIS_FAT_ISLEM_HEADER                         = 7041,
         MENU_FAT_ALIS_ELLE_IRS_FATURALASTIRMA                  = 7042,
         MENU_FAT_ALIS_OTOMATIK_IRS_FATURALASTIRMA              = 7043,
         MENU_FAT_ALIS_ELLE_IRS_FATURALASTIRMA_KONSINYE         = 7044,

         MENU_FAT_RAPORLAR_HEADER                               = 7050,
         MENU_FAT_HIZMET_EKSTRESI                               = 7051,
         MENU_FAT_BA_BS_FORMU                                   = 7052,
         MENU_FAT_LISTESI                                       = 7053

};

void                    FATURA_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           FATURA_GET_MENU_STRUCT();

#endif // FATURA_OPEN_H
