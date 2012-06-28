#ifndef STOK_URUN_HAREKETLERI_ARAMA_CLASS_H
#define STOK_URUN_HAREKETLERI_ARAMA_CLASS_H

#include "arama_kernel.h"

namespace Ui {
    class STOK_URUN_HAREKETLERI_ARAMA;
}

class STOK_URUN_HAREKETLERI_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
     STOK_URUN_HAREKETLERI_ARAMA( int xxxx, QWidget * parent = 0 );
     ~STOK_URUN_HAREKETLERI_ARAMA() {}

private:
    Ui::STOK_URUN_HAREKETLERI_ARAMA *m_ui;

    int  m_urun_id;
    
    void SETUP_FORM           ();
    int  CHECK_VAR            (QObject * object);
    int  CHECK_RUN            ();
    void SEARCH               ();
    int  SINGLE_LINE_SELECTED (int selected_row_number);
    void SELECTED_LINE        ( int selected_row_number );

};

#endif // STOK_URUN_HAREKETLERI_ARAMA_CLASS_H
