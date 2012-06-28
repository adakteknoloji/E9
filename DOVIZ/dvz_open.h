#ifndef DOVIZ_OPEN_H
#define DOVIZ_OPEN_H

#include "dvz_dovizler_batch_open.h"
#include "dvz_kur_guncelle_batch_open.h"
#include "dvz_tanimlari_batch_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_DOVIZ {
         MENU_DVZ_ISLEM_HEADER                             = 3510,
         MENU_DVZ_DOVIZLER                                 = 3511,
         MENU_DVZ_TANIMLARI                                = 3512,
         MENU_DVZ_KURLARI_GUNCELLE                         = 3513
};

void                    DOVIZ_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           DOVIZ_GET_MENU_STRUCT();

#endif // DOVIZ_OPEN_H
