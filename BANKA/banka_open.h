#ifndef BANKA_OPEN_H
#define BANKA_OPEN_H

#include "bnk_acilis_formu_open.h"
#include "bnk_banka_defteri_batch_open.h"
#include "bnk_hareket_fisi_open.h"
#include "bnk_hareket_fisi_arama_open.h"
#include "bnk_hesabin_kredi_kartlari_fisi_open.h"
#include "bnk_hesaplari_arama_open.h"
#include "bnk_hesaplari_fisi_open.h"
#include "bnk_kk_ekstresi_batch_open.h"
#include "bnk_kredi_karti_secimi_open.h"
#include "bnk_makro_fisi_open.h"
#include "bnk_makro_fisi_arama_open.h"
#include "bnk_makro_secimi_open.h"
#include "bnk_makro_tanimlama_fisi_open.h"
#include "bnk_pos_ekstresi_batch_open.h"
#include "bnk_pos_secimi_open.h"
#include "bnk_virman_formu_open.h"
#include "bnk_banka_defteri_raporu_open.h"
#include "bnk_fisi_yazdir_open.h"
#include "bnk_kk_ekstresi_raporu_open.h"
#include "bnk_pos_ekstresi_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_BANKA {
         MENU_BNK_FISLER_HEADER                            =  1510,
         MENU_BNK_MAKRO_FISI                               =  1511,
         MENU_BNK_HAREKET_FISI                             =  1512,
         MENU_BNK_PARA_YAT_CEKME_FISI                      =  1513,
         MENU_BNK_VIRMAN_FORMU                             =  1514,
         MENU_BNK_ACILIS_FISI                              =  1515,

         MENU_BNK_ENT_FISLER_HEADER                        =  1520,
         MENU_BNK_ENT_FISLERI                              =  1521,


         MENU_BNK_ISLEMLER_HEADER                          =  1530,
         MENU_BNK_HESAPLARI                                =  1531,
         MENU_BNK_MAKRO_TANIMLAMA                          =  1532,

         MENU_BNK_RAPORLAR_HEADER                          =  1540,
         MENU_BNK_DEFTERI                                  =  1541,
         MENU_BNK_POS_EKSTRESI                             =  1542,
         MENU_BNK_KK_EKSTRESI                              =  1543,

};

void                    BANKA_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           BANKA_GET_MENU_STRUCT();

#endif // BANKA_OPEN_H
