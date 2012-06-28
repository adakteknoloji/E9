#ifndef PRINT_H
#define PRINT_H

#include <QComboBox>
#include <QPrinter>
#include <QString>
#include <QWidget>

enum PRINTER_PAPER_TYPE{
    A3                     = 1,
    A4                     = 2,
    A5                     = 3,
    B3                     = 4,
    B4                     = 5,
    B5                     = 6
};

void DIREKT_YAZDIR              ( QString printer_name,QString printer_document );
void OPEN_REPORT_SHOWER         ( QString string_html, QWidget * parent, QPrinter::Orientation p_orientation = QPrinter::Portrait, qreal left_margin = 0.0,qreal top_margin = 0.0,qreal bottom_margin=0.0,qreal right_margin=0.0,int paper_type = QPrinter::A4,double user_defined_width = -1.0,double user_defined_height = -1.0 );

//////////////////////////////////////////////////////////////

void  FILL_PAPERSIZE_COMBOBOX   ( QComboBox * combo_box  );
int   GET_PAPERSIZE_ENUM        ( QString     paper_size );

#endif // PRINT_H
