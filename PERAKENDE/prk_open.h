#ifndef PERAKENDE_OPEN_H
#define PERAKENDE_OPEN_H

#include "prk_fis_arama_open.h"
#include "prk_gelir_gider_turleri_fisi_open.h"
#include "prk_perakende_fisi_open.h"
#include "prk_fisi_yazdir_raporu_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_PERAKENDE {
    MENU_PRK_FISLER_HEADER                            = 8010,
    MENU_PRK_ALIS_FISI                                = 8011,
    MENU_PRK_SATIS_FISI                               = 8012,

    MENU_PRK_RAPORLAR_HEADER                          = 8020,
    MENU_PRK_KASA_RAPORU                              = 8021,

    MENU_PRK_TANIMLAR_HEADER                          = 8030,
    MENU_PRK_GIDER_TURLERI                            = 8031,
    MENU_PRK_GELIR_TURLERI                            = 8032


};

void                    PERAKENDE_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           PERAKENDE_GET_MENU_STRUCT();

#endif // PERAKENDE_OPEN_H
