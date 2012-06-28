#ifndef FAIZ_OPEN_H
#define FAIZ_OPEN_H

#include "faiz_hesapla_batch_open.h"
#include "faiz_taksit_raporu_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_FAIZ {
        MENU_FAIZ_ISLEM_HEADER                            = 4010,
        MENU_FAIZ_ANA_PARAYI_BUL                          = 4011,
        MENU_FAIZ_TAKSITLENDIR                            = 4012,
        MENU_FAIZ_AYLIK_FAIZ_ORANI_BUL                    = 4013
};

void                    FAIZ_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           FAIZ_GET_MENU_STRUCT();

#endif // FAIZ_OPEN_H
