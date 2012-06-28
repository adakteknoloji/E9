#ifndef CARI_OTO_EKSTRE_ARAMA_CLASS_H
#define CARI_OTO_EKSTRE_ARAMA_CLASS_H

#include "arama_kernel.h"

namespace Ui {
    class CARI_OTO_EKSTRE_ARAMA;
}

class CARI_OTO_EKSTRE_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
    CARI_OTO_EKSTRE_ARAMA( int p_cari_hesap_id, QWidget * parent = 0 );
    ~CARI_OTO_EKSTRE_ARAMA() {}

private:
    Ui::CARI_OTO_EKSTRE_ARAMA  *m_ui;

    int m_cari_hesap_id;
    
private:

    void SETUP_FORM();
    int  CHECK_VAR (QObject * object);
    int  CHECK_RUN ();
    void SEARCH    ();
    int  SINGLE_LINE_SELECTED(int selected_row_number);
    void SELECTED_LINE( int selected_row_number );

};

#endif // CARI_OTO_EKSTRE_ARAMA_CLASS_H
