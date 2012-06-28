#ifndef STOK_FISI_ARAMA_H
#define STOK_FISI_ARAMA_H

#include "arama_kernel.h"

namespace Ui {
    class STOK_FISI_ARAMA;
}

class STOK_FISI_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           STOK_FISI_ARAMA  ( int p_fis_alis_satis_turu, int p_irsaliye_mi_stok_fisi_mi,int * secilen_id,
                                              int p_iade_irsaliyesi_mi, int p_fis_turu, QWidget * p_parent );
                           ~STOK_FISI_ARAMA (){}
private:
    int *                  m_secilen_id;

    Ui::STOK_FISI_ARAMA *  m_ui;

    int                    m_irsaliye_stok_fisi_alis_satis_turu;
    int                    m_irsaliye_mi_stok_fisi_mi;
    int                    m_iade_irsaliyesi_mi;
    int                    m_fis_turu;
    int                    m_cari_hesap_id;

    void                   SETUP_FORM                                 ();
    int                    CHECK_VAR                                  ( QObject * p_object );
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       ( int p_selected_row_number );

    void                   SEARCH_EDIT_CLICKED                        ( QWidget *widget, QLineEdit *line_edit );

};

#endif // STOK_FISI_ARAMA_H
