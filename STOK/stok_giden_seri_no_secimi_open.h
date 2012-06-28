#ifndef STOK_GIDEN_SERI_NO_SECIMI_OPEN_H
#define STOK_GIDEN_SERI_NO_SECIMI_OPEN_H

#include "stok_enum.h"

class QWidget;
class QStringList;



void STOK_GIDEN_SERI_NO_SEC ( int p_depo_id, int p_urun_id, QStringList *p_seri_no_list, URUN_DEPO_DURMU p_urun_durumu,
                              bool p_sadece_liste, int p_fatura_satiri_id, QWidget * parent );

#endif // STOK_GIDEN_SERI_NO_SECIMI_OPEN_H


