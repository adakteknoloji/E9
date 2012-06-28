#ifndef STOK_URUN_SERI_NO_ARAMA_H
#define STOK_URUN_SERI_NO_ARAMA_H

#include "arama_kernel.h"

namespace Ui {
    class STOK_URUN_SERI_NO_ARAMA;
}

class STOK_URUN_SERI_NO_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           STOK_URUN_SERI_NO_ARAMA             ( int p_depo_id, int p_urun_id, QString p_seri_no,int * secilen_id, QWidget * p_parent);
                           ~STOK_URUN_SERI_NO_ARAMA            (){}



private:
    int *                               m_secilen_id;

    Ui::STOK_URUN_SERI_NO_ARAMA *       m_ui;

    int                                 m_depo_id;
    int                                 m_urun_id;

    QString                             m_seri_no;

    void                                SETUP_FORM            ();
    int                                 CHECK_VAR             ( QObject * p_object );
    int                                 CHECK_RUN             ();
    void                                SEARCH                ();
    int                                 SINGLE_LINE_SELECTED  ( int p_selected_row_number );


};

#endif // STOK_URUN_SERI_NO_ARAMA_H
