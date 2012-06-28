#ifndef ADRES_ARAMA_OPEN_H
#define ADRES_ARAMA_OPEN_H

#include <QList>
class QWidget;


int  OPEN_ADRES_ARAMA               ( int record_type ,int islem_turu, QWidget * parent = NULL );
QList<int>  OPEN_ADRES_ARAMA_LIST   ( int record_type , bool is_multi_line = false, QWidget * parent = NULL );
#endif // ADRES_ARAMA_OPEN_H
