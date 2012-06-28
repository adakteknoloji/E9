#ifndef FAIZ_CONSOLE_UTILS_H
#define FAIZ_CONSOLE_UTILS_H


class QStringList;
class QString;
class QDate;

QStringList  FAIZ_GET_HESAPLAMA_TURU_LIST      ();
QString      FAIZ_GET_HESAPLAMA_TURU_STRING    ( int faiz_sekli );
int          FAIZ_GET_HESAPLAMA_TURU_ENUM      ( QString faiz_sekli );

QDate        FAIZ_ADD_MONTH                    ( QDate p_date , int month_size );


#endif // FAIZ_CONSOLE_UTILS_H
