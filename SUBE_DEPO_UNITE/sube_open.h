#ifndef SUBE_OPEN_H
#define SUBE_OPEN_H

#include "sube_fisi_open.h"

struct MENU_STRUCT;
class QWidget;

enum ENUM_MENU_SUBELER {
         MENU_SUBE_SUBELER                                 = 7511,
         MENU_SUBE_ORGANIZASYON_SEMASI                     = 7512
};


void                    SUBE_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *             SUBE_GET_MENU_STRUCT();

#endif // SUBE_OPEN_H
