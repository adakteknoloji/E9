#ifndef E9_ANA_MENU_H
#define E9_ANA_MENU_H

#include "sekme_menu_kernel.h"

//Array indexleri ve ekrandaki siralama buna gore yapilir
//Ekrandaki siralama ise su sekilde;
//           0    1    2    3
//           4    5    6    7
//           8    9    .    .

enum E9_MODULES_INDEX {
    GENEL_MUHASEBE_INDEX  = 0,
    ISLETME_DEFTERI_INDEX = 1,
    DOVIZ_INDEX           = 2,
    ADRES_INDEX           = 3,
    CARI_INDEX            = 4,
    CEK_INDEX             = 5,
    BANKA_INDEX           = 6,
    SUBE_INDEX            = 7,
    FATURA_INDEX          = 8,
    IRSALIYE_INDEX        = 9,
    STOK_INDEX            = 10,
    PERAKENDE_SATIS_INDEX = 11,
    PERSONEL_INDEX        = 12,
    DEMIRBAS_INDEX        = 13,
    FAIZ_INDEX            = 14
};

class E9_ANA_MENU : public SEKME_MENU_KERNEL
{
    Q_OBJECT

public:
    E9_ANA_MENU(QWidget *parent = 0);
    ~E9_ANA_MENU();

    void                SET_ANA_EKRAN_TEXT                   (QString text,int font_size_pt = 10,int widget_height = 70);

private:
    void                MENU_ITEM_CLICKED                    (int parent_item_enum, int menu_item_enum);

    void                PREPARE_PROGRAM_FOR_USER             ();

    MENU_STRUCT *       GET_MENU_STRUCT                      (int parent_enum_id);

    void                KULLANICI_YETKILERINI_AYARLA         ();

};

#endif // E9_ANA_MENU_H
