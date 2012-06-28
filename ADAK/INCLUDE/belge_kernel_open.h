#ifndef BELGE_H
#define BELGE_H

#include <QWidget>
#include <belge_struct.h>

enum BELGE_PRINT_MOD {
    PRINT_NORMAL     = 0,
    PRINT_TEST_VARS  = 1,
    PRINT_TEST_XXX   = 2
};

int    TASARIM_SEC   (int belge_id,QWidget * parent);

void   PRINT_BELGE   (int belge_turu, int tasarim_id, ADAK_BELGELER_STRUCT * belgeler_struct,
                      QString printer_name = "", BELGE_PRINT_MOD p_mode = PRINT_NORMAL );

void   OPEN_TASARIM_BELGE_SECIMI( ADAK_BELGELER_STRUCT * belge_struct, int belge_count, QWidget * parent );
void   OPEN_TASARIM_MENU_BATCH  ( ADAK_BELGELER_STRUCT * p_belge_struct, int p_belge_count, QWidget * parent );

#endif // BELGE_H
