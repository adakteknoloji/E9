#ifndef BELGE_KERNEL_UTILS_H
#define BELGE_KERNEL_UTILS_H

#include <QList>

struct ADAK_BELGELER_STRUCT;
struct ADAK_BELGE_DEFAULTS_STRUCT;
class TASARIM_DEGISKENI;
class QString;

enum PRINTER_TYPES {
    DOT_MATRIX_PRINTER = 10,
    LAZER_PRINTERS     = 20
};

enum DOTMATRIX_FONT {
    DOTMATRIX_CONDENSED_FONT = 10,
    DOTMATRIX_NORMAL_FONT    = 20,
    DOTMATRIX_LARGE_FONT     = 30
};

//0 belge_adi ni tutar,1 belge id yi tutar.
void SET_BELGE_STRUCTURES (int belge_turu,ADAK_BELGELER_STRUCT * belgeler_struct);
void SET_BELGE_TASARIM    (int belge_turu, int tasarim_id,ADAK_BELGELER_STRUCT * belgeler_struct);

void SET_BELGE_STRUCTURES (ADAK_BELGE_DEFAULTS_STRUCT * header_struct, int header_struct_size,
                           ADAK_BELGE_DEFAULTS_STRUCT * footer_struct, int footer_struct_size,
                           ADAK_BELGE_DEFAULTS_STRUCT * line_struct, int line_struct_size );

ADAK_BELGE_DEFAULTS_STRUCT * GET_BELGE_HEADER_STRUCT             ();
ADAK_BELGE_DEFAULTS_STRUCT * GET_BELGE_FOOTER_STRUCT             ();
ADAK_BELGE_DEFAULTS_STRUCT * GET_BELGE_LINE_STRUCT               ();


int     GET_BELGE_HEADER_STRUCT_SIZE     ();
int     GET_BELGE_FOOTER_STRUCT_SIZE     ();
int     GET_BELGE_LINE_STRUCT_SIZE       ();
void    GET_KAYITLI_DEGISKENLER       ( QList< TASARIM_DEGISKENI *> *P_USER_VARIABLE );
int     GET_BELGE_PRINTER_TYPE           ();
int     GET_BELGE_PRINT_ROW_COUNT        ();
int     GET_BELGE_ROW_COUNT              (); // belge satir sayisi
double  GET_BELGE_ROW_SPACE_MM           ();
int     GET_BELGE_ROW_SPACE_SATIR        ();
void    GET_BELGE_BOYUT_BILGILERI_SATIR  (int * belge_satir_sayisi,int * belge_toplam_karakter_sayisi);
void    GET_SAVED_VARIABLE_INFO          (int tasarim_id);
int     GET_BELGE_ID                     ();
QString GET_BELGE_ADI                    ();
int     GET_TASARIM_ID                   ();
QString GET_TASARIM_ADI                  ();
bool    GET_IS_TASARIM_READONLY          ();
void    SET_BELGE_ID                     (int belge_id);
void    SET_BELGE_ADI                    (QString belge_adi);
void    SET_TASARIM_ID                   (int tasarim_id);
void    SET_TASARIM_ADI                  (QString tasarim_adi);
void    SET_TASARIM_READONLY_VALUE       (int value);

void    DEGISLENLERI_SIRALA              ( QList<TASARIM_DEGISKENI *> *p_list );

#endif // BELGE_KERNEL_UTILS_H
