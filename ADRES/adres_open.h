#ifndef ADRES_OPEN_H
#define ADRES_OPEN_H

#include "adres_arama_open.h"
#include "adres_email_fisi_open.h"
#include "adres_email_raporu_open.h"
#include "adres_fihrist_raporu_open.h"
#include "adres_fihrist_raporu_batch_open.h"
#include "adres_kayitlari_raporu_open.h"
#include "adres_kayit_formu_open.h"
#include "adres_email_log_batch_open.h"
#include "adres_rapor_filtresi_batch_open.h"
#include "adres_etiketleri_batch_open.h"
#include "adres_etiketleri_open.h"
#include "adres_export_open.h"
#include "adres_import_open.h"
#include "adres_not_defteri_fisi_open.h"
#include "adres_telefon_fisi_open.h"
#include "adres_toplu_mail_gonderimi_batch_open.h"
#include "adres_yetkililer_fisi_open.h"
#include "adres_zarf_yazdir_batch_open.h"

struct MENU_STRUCT;

enum ENUM_MENU_ADRES {
     MENU_ADR_ADRES_KARTLARI_HEADER                    =  1010,
     MENU_ADR_SAHIS_KARTI                              =  1011,
     MENU_ADR_FIRMA_KARTI                              =  1012,

     MENU_ADR_ISLEMLER_HEADER                          =  1020,
     MENU_ADR_GRUPLAR                                  =  1021,
     MENU_ADR_ETIKETLERI                               =  1022,
     MENU_ADR_TOPLU_MAIL_GONDERME                      =  1023,
     MENU_ADR_TOPLU_SMS_GONDERME                       =  1024,
     MENU_ADR_IMPORT                                   =  1025,
     MENU_ADR_EXPORT                                   =  1026,

     MENU_ADR_RAPORLAR_HEADER                          =  1030,
     MENU_ADR_KAYITLARI                                =  1031,
     MENU_ADR_TEL_WEB_KAYITLARI                        =  1032,
     MENU_ADR_FIHRIST                                  =  1033

};


void                    ADRES_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           ADRES_GET_MENU_STRUCT();


#endif // ADRES_OPEN_H
