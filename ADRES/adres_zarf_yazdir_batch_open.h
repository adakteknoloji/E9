#ifndef ADRES_ZARF_YAZDIR_BATCH_OPEN_H
#define ADRES_ZARF_YAZDIR_BATCH_OPEN_H

#include "etiket_sablonlari.h"

class QString;
class QWidget;

void        OPEN_ADRES_ZARF_YAZDIR_BATCH ( int record_id, QString printer_name , QWidget * parent );

QString     GET_ADRES_ZARF_ETIKETLERI                       ( int record_id,etiket_sablonlari etiket_sablonu );

#endif // ADRES_ZARF_YAZDIR_BATCH_OPEN_H
