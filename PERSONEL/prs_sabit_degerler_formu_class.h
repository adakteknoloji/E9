#ifndef PRS_SABIT_DEGERLER_FORMU_CLASS_H
#define PRS_SABIT_DEGERLER_FORMU_CLASS_H

#include <form_kernel.h>
#include "ui_prs_sabit_degerler_formu.h"
#include "prs_enum.h"
#include "prs_struct.h"

namespace Ui {
    class PRS_SABIT_DEGERLER_FORMU;
}

class PRS_SABIT_DEGERLER_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
                        PRS_SABIT_DEGERLER_FORMU(QWidget *parent = 0);
                        ~ PRS_SABIT_DEGERLER_FORMU(){}

private:

    Ui::PRS_SABIT_DEGERLER_FORMU  *       m_ui;

    int             m_sabit_degerler_id;

    int             M_IHBAR_TAZ_BASLANGIC_TARIHI_TURLERI        [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          M_IHBAR_TAZ_BASLANGIC_TARIHI_ORJ_DEGERLERI  [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          M_IHBAR_TAZ_BASLANGIC_TARIHI_AY_DEGERLERI   [IHBAR_TAZMINATI_DILIMI_SAYISI];
    int             M_IHBAR_TAZ_BITIS_TARIHI_TURLERI            [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          M_IHBAR_TAZ_BITIS_TARIHI_ORJ_DEGERLERI      [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          M_IHBAR_TAZ_BITIS_TARIHI_AY_DEGERLERI       [IHBAR_TAZMINATI_DILIMI_SAYISI];
    double          M_IHBAR_TAZ_TAZMINAT_HAFTA_DEGERLERI        [IHBAR_TAZMINATI_DILIMI_SAYISI];

    void            SETUP_FORM                               ();
    void            CLEAR_FORM_MEMBERS                       ();
    void            NEW_RECORD                               ();
    int             GET_RECORD                               ( int record_id );
    int             SELECT_RECORD                            ();
    int             FIND_RECORD                              ();

    int             CHECK_VAR                                ( QObject * object );
    int             CHECK_EMPTY                              ();

    int             CHECK_ADD                                ();
    int             ADD_RECORD                               ();
    int             CHECK_UPDATE                             ( int record_id );
    void            UPDATE_RECORD                            ( int record_id );
    int             CHECK_DELETE                             ( int record_id );
    void            DELETE_RECORD                            ( int record_id );

    int             FIND_NEXT_RECORD                         ();
    int             FIND_PREV_RECORD                         ();
    int             FIND_FIRST_RECORD                        ();
    int             FIND_LAST_RECORD                         ();

    int             LOCK_RECORD                              ( int record_id );
    void            UNLOCK_RECORD                            ( int record_id );
    void            SAVER_BUTTON_CLICKED                     ( QAbstractButton * button, int record_id );

    void            TEHLIKE_SINIFINA_GORE_SSK_PRIMI_BELIRLE  ();
    void            GELIR_VERGISI_DILIMI_ARALIKLARINI_BELIRLE( int index, QLabel * label_gelir_vergisi ,QLineEdit * line_edit_gelir_vergisi_dilimi_bitis );
    void            SHOW_SABIT_DEGERLER_DEFAULTS             ();
    void            HIGHLIGHT_NON_DEFAULT_VALUES             ( QObject * object);
    void            RETURN_SABIT_DEGERLER_DEFAULTS           ();
    int             CHANGER_BUTTON_CLICKED                   ( QAbstractButton * p_button ) ;

    int             SABIT_DEGERLERI_GUNCELLE                 ();

    void            SET_DEFAULT_ODENEK_VE_KESINTIKER         ();

    void            HIDE_OR_SHOW_GROUP_BOX                  ( QComboBox* combo_box_odenek, QGroupBox* group_box_odenek );
    int             ODENEK_KALDIRILSIN_MI                   ( QComboBox* combo_box );

};

#endif // PRS_SABIT_DEGERLER_FORMU_H
