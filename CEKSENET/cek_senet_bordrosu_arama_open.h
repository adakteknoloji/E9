#ifndef CEK_SENET_BORDROSU_ARAMA_OPEN_H
#define CEK_SENET_BORDROSU_ARAMA_OPEN_H

#include "cek_enum.h"

class QWidget;
int     OPEN_CEK_SENET_BORDROSU_ARAMA               ( int modul_id, int bordro_turu , int bordro_islem_turu,
                                                      int cek_bordrosu_mu, CEK_SENET_TURU p_cek_senet_turu = ENUM_CEK_SENET_FISI ,QWidget * parent = NULL );


#endif // CEK_SENET_BORDROSU_ARAMA_OPEN_H
