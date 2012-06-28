#ifndef MUH_FIS_ARAMA_BATCH_H
#define MUH_FIS_ARAMA_BATCH_H

#include "arama_kernel.h"
#include "muh_enum.h"

namespace Ui {
    class MUH_FIS_ARAMA;
}

class MUH_FIS_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           MUH_FIS_ARAMA             ( int yvm_no_goster, int fis_kaynagi, int p_fis_turu, MUH_FIS_TURU p_muh_fis_turu, QWidget * parent = 0 );
                           ~MUH_FIS_ARAMA            (){}

    int *                  m_secilen_id;

private:

    Ui::MUH_FIS_ARAMA *        m_ui;

    int                    m_fis_turu;
    int                    m_yvm_no_goster;
    int                    m_modul_id;

    MUH_FIS_TURU           m_muh_fis_turu;

    void                   SETUP_FORM                                 ();
    int                    CHECK_VAR                                  ( QObject * object );
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       ( int selected_row_number );


};

#endif // MUH_FIS_ARAMA_H
