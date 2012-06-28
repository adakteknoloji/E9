#ifndef MUHASEBE_OPEN_H
#define MUHASEBE_OPEN_H

#include "muh_bilanco_raporu_batch_open.h"
#include "muh_defteri_kebir_batch_open.h"
#include "muh_gelir_tablosu_batch_open.h"
#include "muh_hatali_fis_secimi_open.h"
#include "muh_hesap_ekstresi_batch_open.h"
#include "muh_hesap_plani_open.h"
#include "muh_hesap_plani_raporu_batch_open.h"
#include "muh_mizan_raporu_batch_open.h"
#include "muh_muhasebe_fisi_open.h"
#include "muh_toplu_fis_raporu_batch_open.h"
#include "muh_yansitma_hesaplari_fisi_open.h"
#include "muh_yevmiye_dokumu_batch_open.h"
#include "muh_yansitma_islemi_batch_open.h"
#include "muh_yevmiye_no_olustur_batch_open.h"
#include "muh_hesap_plani_kopyala_batch_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_MUHASEBE {
        MENU_MUH_FISLER_HEADER                      =  5010,
            MENU_MUH_MAHSUP_FISI                    =  5011,
            MENU_MUH_TAHSIL_FISI                    =  5012,
            MENU_MUH_TEDIYE_FISI                    =  5013,
            MENU_MUH_ACILIS_FISI                    =  5014,
            MENU_MUH_KAPANIS_FISI                   =  5015,

        MENU_MUH_ENT_FISLERI_HEADER                 =  5020,
            MENU_MUH_ENT_FISLERI                    =  5021,

        MENU_MUH_HESAPLAR_HEADER                    =  5050,
            MENU_MUH_HESAP_PLANI                    =  5051,
            MENU_MUH_HESAP_GRUPLARI                 =  5052,

        MENU_MUH_ISLEMLER_HEADER                    =  5040,
            MENU_MUH_YVM_NO_OLUSTUR                 =  5041,
            MENU_MUH_YANSITMA_HESAPLARI             =  5042,
            MENU_MUH_YANSITMA_FISI_OLUSTUR          =  5043,
            MENU_MUH_HESAP_PLANI_KOPYALA            =  5044,

        MENU_MUH_RAPORLAR_HEADER                    =  5060,
            MENU_MUH_HESAP_EKSTRESI                 =  5061,
            MENU_MUH_MIZAN                          =  5062,
            MENU_MUH_HATALI_FISLER                  =  5063,
            MENU_MUH_HESAP_PLANI_RAPORU             =  5064,
            MENU_MUH_TOPLU_FIS_DOKUMU               =  5065,
            MENU_MUH_YEVMIYE_DOKUMU                 =  5066,
            MENU_MUH_DEFTERI_KEBIR                  =  5067,
            MENU_MUH_BILANCO                        =  5068,
            MENU_MUH_GELIR_TABLOSU                  =  5069
};


void                    MUHASEBE_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           MUHASEBE_GET_MENU_STRUCT();

#endif // MUHASEBE_OPEN_H
