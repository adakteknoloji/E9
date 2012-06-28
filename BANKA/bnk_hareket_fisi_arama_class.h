#ifndef BNK_HAREKET_FISI_ARAMA_CLASS_H
#define BNK_HAREKET_FISI_ARAMA_CLASS_H

#include "arama_kernel.h"
#include "banka_enum.h"


namespace Ui {
    class BNK_HAREKET_FISI_ARAMA;
}

class BNK_HAREKET_FISI_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           BNK_HAREKET_FISI_ARAMA         ( int banka_modulu_fis_turu, int fis_kaynagi, BANKA_FIS_TURU p_fis_turu, QWidget * parent = 0 );
                           ~BNK_HAREKET_FISI_ARAMA                            (){}
    int *                  m_secilen_id;


private:
    Ui::BNK_HAREKET_FISI_ARAMA *    m_ui;
    int                           m_base_fisi_turu;
    int                           m_modul_id;
    int                           m_entegre_filtresi_turu;
    BANKA_FIS_TURU                m_fis_turu;
    
    void                          SETUP_FORM                                 ();

    int                           CHECK_VAR                                  ( QObject * object );
    int                           CHECK_RUN                                  ();
    void                          SEARCH                                     ();
    int                           SINGLE_LINE_SELECTED                       ( int selected_row_number );

    QStringList                   GET_UYGUN_BANKA_HESAPLARI_ID_LIST          (QString banka_ismi);


};

#endif // BNK_FISI_ARAMA_H
