#ifndef SMM_MAKBUZ_OPEN_H
#define SMM_MAKBUZ_OPEN_H

#include "smm_ent_detaylari_batch_open.h"
#include "smm_hizmet_tanimlama_fisi_open.h"
#include "smm_makbuz_arama_open.h"
#include "smm_makbuz_belgesi_open.h"
#include "smm_makbuz_formu_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_SMM {
    MENU_SMM_FISLER_HEADER                            = 9010,
    MENU_SMM_MAKBUZ_ALIS                              = 9011,
    MENU_SMM_MAKBUZ_SATIS                             = 9012,

    MENU_SMM_TANIMLAR_HEADER                          = 9020,
    MENU_SMM_ALINAN_HIZMETLER                         = 9021,
    MENU_SMM_SATILAN_HIZMETLER                        = 9022,
    MENU_SMM_E9                                       = 9023
};

void                    SMM_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           SMM_GET_MENU_STRUCT();

#endif // SMM_MAKBUZ_OPEN_H
