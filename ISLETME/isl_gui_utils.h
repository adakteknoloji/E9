#ifndef ISL_GUI_UTILS_H
#define ISL_GUI_UTILS_H

class QComboBox;
class QString;

void FILL_ISL_GIDERLERI( QComboBox *p_combobox );
void FILL_ISL_GELIRLERI( QComboBox *p_combobox );

int ISL_GET_GIDER_ENUM( QString p_str );
int ISL_GET_GELIR_ENUM( QString p_str );

QString ISL_GET_GIDER_STRING( int p_enum );
QString ISL_GET_GELIR_STRING( int p_enum );

#endif // ISL_GUI_UTILS_H
