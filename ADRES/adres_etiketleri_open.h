#ifndef ADRES_ETIKETLERI_OPEN_H
#define ADRES_ETIKETLERI_OPEN_H

#include "etiket_sablonlari.h"

class QStringList;
class QString;

QString GET_ADRES_ETIKETLERI ( QStringList P_ADRES_ETIKET_BILGILERI, etiket_sablonlari etiket_sablonu, float yazi_tipi_boyutu );

#endif // ADRES_ETIKETLERI_OPEN_H
