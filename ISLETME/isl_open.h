#ifndef ISLETME_OPEN_H
#define ISLETME_OPEN_H

#include "isl_defteri_fisi_open.h"
#include "isl_defteri_raporu_batch_open.h"
#include "isl_fis_arama_open.h"
#include "isl_fis_kopyalama_batch_open.h"
#include "isl_kdv_raporu_batch_open.h"
#include "isl_defteri_raporu_open.h"
#include "isl_fis_yazdir_open.h"
#include "isl_kdv_raporu_open.h"
#include "isl_hesap_ozeti_batch_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_ISLETME {

    MENU_ISL_FISLER_HEADER       = 4510,
    MENU_ISL_GELIR_FISI          = 4511,
    MENU_ISL_GIDER_FISI          = 4512,
    MENU_ISL_RAPORLAR_HEADER     = 4520,
    MENU_ISL_KDV_RAPORU          = 4521,
    MENU_ISL_DEFTERI_RAPORU      = 4522,
    MENU_ISL_HESAP_OZETI         = 4523
};


void                    ISLETME_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           ISLETME_GET_MENU_STRUCT();

#endif // ISLETME_OPEN_H
