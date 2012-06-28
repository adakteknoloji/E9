#ifndef DEMIRBAS_OPEN_H
#define DEMIRBAS_OPEN_H

#include "dmr_amortisman_fisi_open.h"
#include "dmr_demirbas_karti_arama_open.h"
#include "dmr_demirbas_karti_open.h"
#include "dmr_amortisman_ayirma_batch_open.h"
#include "dmr_yeniden_degerleme_batch_open.h"
#include "dmr_yeniden_degerleme_fisi_open.h"
#include "dmr_demirbas_listesi_batch_open.h"
#include "dmr_satis_formu_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_DEMIRBAS {

         MENU_DMR_TANIMLAR_HEADER                          = 3010,
         MENU_DMR_KAYDI                                    = 3011,
         MENU_DMR_GRUPLARI                                 = 3012,
         MENU_DMR_RAPOR_ISLEM_GRUPLARI                     = 3013,

         MENU_DMR_FISLER_HEADER                            = 3020,
         MENU_DMR_AMORTISMAN_FISI                          = 3021,
         MENU_DMR_YEN_DEGERLEME_FISI                       = 3022,

         MENU_DMR_ISLEMLER_HEADER                          = 3030,
         MENU_DMR_TOPLU_AMORTISMAN                         = 3031,
         MENU_DMR_TOPLU_YEN_DEGERLEME                      = 3032,
         MENU_DMR_SATISI                                   = 3034,

         MENU_DMR_RAPORLAR_HEADER                          = 3040,
         MENU_DMR_LISTESI                                  = 3041
};

void                    DEMIRBAS_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           DEMIRBAS_GET_MENU_STRUCT();

#endif // DEMIRBAS_OPEN_H
