#ifndef BNK_HAREKET_FISI_ARAMA_OPEN_H
#define BNK_HAREKET_FISI_ARAMA_OPEN_H

// enum un calisabilmesi icin eklenmistir
#include "banka_enum.h"

class QWidget;

int         OPEN_BNK_HAREKET_FISI_ARAMA ( int banka_modulu_fis_turu, int fis_kaynagi, BANKA_FIS_TURU p_fis_turu = ENUM_BANKA_FISI, QWidget * parent = NULL );

#endif // BNK_HAREKET_FISI_ARAMA_OPEN_H
