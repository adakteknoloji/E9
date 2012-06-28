#ifndef BNK_MAKRO_FISI_ARAMA_CLASS_H
#define BNK_MAKRO_FISI_ARAMA_CLASS_H

#include "arama_kernel.h"
#include "ui_bnk_makro_fisi_arama.h"

namespace Ui {
    class BNK_MAKRO_FISI_ARAMA;
}

class BNK_MAKRO_FISI_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           BNK_MAKRO_FISI_ARAMA                             ( int       banka_modulu_fis_turu,
                                                                              int       fis_kaynagi,
                                                                              QWidget * parent = 0 );
                           ~BNK_MAKRO_FISI_ARAMA                            (){}
    int *                  m_secilen_id;

private:

    int                           m_entegre_filtresi_turu;

private:
    Ui::BNK_MAKRO_FISI_ARAMA *    m_ui;
    int                           m_base_fisi_turu;
    int                           m_modul_id;
    
    void                          SETUP_FORM                                 ();
    int                           CHECK_VAR                                  ( QObject * object );
    int                           CHECK_RUN                                  ();
    void                          SEARCH                                     ();
    int                           SINGLE_LINE_SELECTED                       ( int selected_row_number );

    QStringList                   GET_UYGUN_BANKA_HESAPLARI_ID_LIST          (QString banka_ismi);


};

#endif // BNK_FISI_ARAMA_H
