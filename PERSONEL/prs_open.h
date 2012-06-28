#ifndef PRS_OPEN_H
#define PRS_OPEN_H

#include "prs_ebildirge_olustur_batch_open.h"
#include "prs_ek_odenek_kesinti_arama_open.h"
#include "prs_ek_odenek_ve_kesinti_fisi_open.h"
#include "prs_eksik_gun_nedenleri_batch_open.h"
#include "prs_puantajlari_gor_ve_duzelt_fisi_open.h"
#include "prs_sabit_degerler_formu_open.h"
#include "prs_ucret_pusulasi_batch_open.h"
#include "prs_bordro_inceleme_tree_secimi_open.h"
#include "prs_ihbar_ve_kidem_tazminati_batch_open.h"
#include "prs_isten_ayrilanlar_arama_open.h"
#include "prs_isten_ayrilma_batch_open.h"
#include "prs_isten_ayrilma_raporu_open.h"
#include "prs_maas_bordrosu_batch_open.h"
#include "prs_maas_bordrosu_iptal_batch_open.h"
#include "prs_maas_bordrosu_raporu_open.h"
#include "prs_maas_bordrosu_raporu_batch_open.h"
#include "prs_personel_aile_uyeleri_open.h"
#include "prs_personel_formu_open.h"
#include "prs_personel_kesinti_ekleme_fisi_open.h"
#include "prs_personel_odenek_ekleme_fisi_open.h"
#include "prs_personel_raporu_open.h"
#include "prs_personel_raporu_batch_open.h"
#include "prs_ucret_pusulasi_raporu_open.h"
#include "prs_ibraname_maddeleri_batch_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_PRS {
     MENU_PRS_ISLEMLER_HEADER                      = 5510,
     MENU_PRS_MAAS_BORDROSU                        = 5511,
     MENU_PRS_EBILDIRGE                            = 5512,
     MENU_PRS_EKSIK_GUN_NEDENLERI                  = 5513,
     MENU_PRS_BORDRO_INCELEME                      = 5514,
     MENU_PRS_ISTEN_AYRILMA                        = 5515,
     MENU_PRS_ISTEN_AYRILAN_ARAMA                  = 5516,
     MENU_PRS_MAAS_BORDROSU_IPTAL                  = 5517,

     MENU_PRS_RAPORLAR_HEADER                      = 5520,
     MENU_PRS_MAAS_BORDROSU_RAPORU                 = 5521,
     MENU_PRS_UCRET_PUSULASI                       = 5522,
     MENU_PRS_IHBAR_VE_KIDEM_TAZMINATI_RAPORU      = 5523,
     MENU_PRS_PERSONEL_RAPORU                      = 5524,

     MENU_PRS_TANIMLAR_HEADER                      = 5530,
     MENU_PRS_SICIL_KARTI_TANIMLAMA                = 5531,
     MENU_PRS_EK_ODENEK_TANIMLAMA                  = 5532,
     MENU_PRS_EK_KESINTI_TANIMLAMA                 = 5533,
     MENU_PRS_GRUPLAR                              = 5534,
     MENU_PRS_SABIT_DEGERLER                       = 5535,
     MENU_PRS_KARTI_IMPORT                         = 5536,
     MENU_PRS_KARTI_EXPORT                         = 5537




};

void                    PERSONEL_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           PERSONEL_GET_MENU_STRUCT();

#endif // PRS_OPEN_H
