#ifndef IRS_OPEN_H
#define IRS_OPEN_H

#include "irs_irsaliye_listesi_batch_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_IRSALIYE {

        MENU_IRS_SATIS_IRS_HEADER                               = 6511,
        MENU_IRS_SATIS_IRSALIYELERI                             = 6512,
        MENU_IRS_ALISTAN_IADE                                   = 6513,
        MENU_IRS_FASON_BASLANGIC_CIKIS                          = 6514,
        MENU_IRS_FASON_BITIS_CIKIS                              = 6515,

        MENU_IRS_ALIS_IRS_HEADER                                = 6520,
        MENU_IRS_ALIS_IRSALIYELERI                              = 6521,
        MENU_IRS_SATISTAN_IADE                                  = 6522,
        MENU_IRS_FASON_BASLANGIC_GIRIS                          = 6523,
        MENU_IRS_FASON_BITIS_GIRIS                              = 6524,

        MENU_IRS_ISLEM_HEADER                                   = 6530,
        MENU_IRS_SATIS_IRS_ISLEM_HEADER                         = 6531,
        MENU_IRS_SATIS_ELLE_FAT_IRSALIYELESTIRME                = 6532,
        MENU_IRS_SATIS_OTOMATIK_FAT_IRSALIYELESTIRME            = 6533,
        MENU_IRS_YAZICIDAKI_BELGE_NOLARI                        = 6552,

        MENU_IRS_ALIS_IRS_ISLEM_HEADER                          = 6541,
        MENU_IRS_ALIS_ELLE_FAT_IRSALIYELESTIRME                 = 6542,
        MENU_IRS_ALIS_OTOMATIK_FAT_IRSALIYELESTIRME             = 6543,


        MENU_IRS_RAPORLAR_HEADER                                = 6550,
        MENU_IRS_LISTESI                                        = 6551

    //MENU_CARI_YAZICIDAKI_BLG_NOLARI
};

void                    IRSALIYE_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           IRSALIYE_GET_MENU_STRUCT();

#endif // IRS_OPEN_H
