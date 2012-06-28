#ifndef CEK_SENET_BORDROSU_ARAMA_H
#define CEK_SENET_BORDROSU_ARAMA_H

#include "arama_kernel.h"
#include "cek_enum.h"

namespace Ui {
    class CEK_SENET_BORDROSU_ARAMA;
}

class CEK_SENET_BORDROSU_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT


public:
                           CEK_SENET_BORDROSU_ARAMA                         ( int fis_kaynagi,int bordro_turu ,
                                                                              int bordro_islem_turu,
                                                                              int cek_bordrosu_mu, CEK_SENET_TURU p_cek_senet_turu, QWidget * parent = 0);
                           ~CEK_SENET_BORDROSU_ARAMA                        () {}
    int *                               m_secilen_id;
private:

    Ui::CEK_SENET_BORDROSU_ARAMA    *   m_ui;

    int                                 m_modul_id;
    int                                 m_bordro_turu;
    int                                 m_bordro_islem_turu;
    int                                 m_cek_bordrosu_mu;

    CEK_SENET_TURU                      m_cek_senet_turu;

    void                                SETUP_FORM                           ();

    int                                 CHECK_VAR                            ( QObject * object );
    int                                 CHECK_RUN                            ();
    void                                SEARCH                               ();
    int                                 SINGLE_LINE_SELECTED                 ( int selected_row_number );

};

#endif // CEK_SENET_BORDROSU_ARAMA_H
