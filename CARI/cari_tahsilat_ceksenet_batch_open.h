#ifndef CARI_TAHSILAT_CEKSENET_BATCH_OPEN_H
#define CARI_TAHSILAT_CEKSENET_BATCH_OPEN_H


class QString ;
class QWidget;
struct E9_ISLEM_DETAYLARI;

bool        OPEN_CARI_TAHSILAT_CEKSENET_BATCH                  ( int fis_satiri_id , int islem_turu ,
                                                                 E9_ISLEM_DETAYLARI * islem_detaylari,
                                                                 bool islem_turu_degisti = false ,QWidget * parent = 0);

#endif // CARI_TAHSILAT_CEKSENET_BATCH_OPEN_H
