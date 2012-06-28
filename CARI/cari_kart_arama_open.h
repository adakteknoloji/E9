#ifndef CARI_KART_ARAMA_OPEN_H
#define CARI_KART_ARAMA_OPEN_H

#include"cari_enum.h"

class QString;
class QWidget;

int         OPEN_CARI_KART_ARAMA   ( QString cari_hesap_ismi_hesap_kodu, int cari_kart_turu, QWidget * parent,
                                        int show_add_button = 0 , int prog_id =-1,int modul_id = -1,
                                        QString p_ekran_ismi = QObject::tr("CARİ KART ARAMA"), QString filtre_str = "" , int islem_turu = CARI_KART_ARAMA_ISLEMI   );

#endif // CARI_KART_ARAMA_OPEN_H
