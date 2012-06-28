#ifndef STOK_FISI_OPEN_H
#define STOK_FISI_OPEN_H

class QWidget;

void  OPEN_STOK_FISI ( int p_fis_id , int p_irsaliye_mi_stok_fisi_mi, int p_fis_turu, int p_iade_irsaliyesi_mi = -1 , QWidget * p_parent = 0 );

struct MENU_STRUCT;

void                    STOK_ITEM_CLICKED   (int menu_item,QWidget * parent);

MENU_STRUCT *           STOK_GET_MENU_STRUCT();


#endif // STOK_FISI_OPEN_H
