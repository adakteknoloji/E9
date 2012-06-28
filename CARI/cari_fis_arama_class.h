#ifndef CARI_FIS_ARAMA_H
#define CARI_FIS_ARAMA_H

#include "arama_kernel.h"
#include "cari_enum.h"

namespace Ui {
    class CARI_FIS_ARAMA;
}

class CARI_FIS_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT

public:
                                CARI_FIS_ARAMA                  ( int p_cari_modulu_fis_turu, int p_fis_kaynagi, CARI_FIS_TURU p_cari_fis_turu, QWidget * p_parent = 0);
                                ~CARI_FIS_ARAMA                 () {}
    int *                       m_secilen_id;

private:

    Ui::CARI_FIS_ARAMA *        m_ui;
    int                         m_cari_modulu_fis_turu;
    int                         m_modul_id;
    int                         m_cari_hesap_id;
    CARI_FIS_TURU               m_cari_fis_turu;

    void                        SETUP_FORM                           ();

    int                         CHECK_VAR                            ( QObject * p_object );
    int                         CHECK_RUN                            ();
    void                        SEARCH                               ();
    int                         SINGLE_LINE_SELECTED                 ( int p_selected_row_number );
    void                        SEARCH_EDIT_CLICKED                  ( QWidget * p_widget, QLineEdit * p_line_edit );



};

#endif // CARI_FIS_ARAMA_H
