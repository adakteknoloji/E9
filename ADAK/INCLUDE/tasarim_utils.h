#ifndef TASARIM_UTILS
#define TASARIM_UTILS

#include <QDialog>
#include <QComboBox>

void    FILL_DEGISKEN_GRUBU_COMBOBOX    (QComboBox * combo_box);
void    FILL_ALIGN_COMBOBOX             (QComboBox * combo_box);

void    FILL_PRINTER_TYPE_COMBOBOX      (QComboBox * combo_box);
int     GET_PRINTER_TYPE_ENUM           (QString printer_type_str);
QString GET_PRINTER_TYPE_STR            (int     printer_type_enum);

void    FILL_DOTMATRIX_FONT_COMBOBOX    (QComboBox * combo_box);
int     GET_DOTMATRIX_FONT_ENUM         (QString font_type_str);
QString GET_DOTMATRIX_FONT_STR          (int font_type_enum);

void    CREATE_CPP_CODE_SAVED_DESIGNS   ();

QString GET_BELGE_GRUP_STR              (int belge_grup_enum   );
int     GET_BELGE_GRUP_ENUM             (QString belge_grup_str);
int     GET_BELGE_ALIGN_ENUM            (QString belge_align_str);
QString GET_BELGE_ALIGN_STR             (int belge_align_enum);



#endif // TASARIM_UTILS
