#ifndef CARI_VIRMAN_FORMU_H
#define CARI_VIRMAN_FORMU_H

#include <form_kernel.h>

struct CARI_FIS_STRUCT;
struct CARI_FIS_SATIRI_STRUCT;
struct E9_ENT_DETAYLARI_STRUCT;


namespace Ui {
    class CARI_VIRMAN_FORMU;
}

class CARI_VIRMAN_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
    CARI_VIRMAN_FORMU( int p_form_id, QWidget *parent = 0);
    ~ CARI_VIRMAN_FORMU(){}



private:
    Ui::CARI_VIRMAN_FORMU  *       m_ui;

    CARI_FIS_STRUCT         *   M_FIS;
    CARI_FIS_SATIRI_STRUCT  *   M_FIS_SATIRI;
    E9_ENT_DETAYLARI_STRUCT *   M_FIS_DETAYLARI;


    int            m_borclu_hesap_id;
    int            m_borclu_hesap_para_birimi_id;
    int            m_alacakli_hesap_id;
    int            m_alacakli_hesap_para_birimi_id;

    int            m_form_id;



    void           SETUP_FORM                           ();
    void           CLEAR_FORM_MEMBERS                   ();
    void           NEW_RECORD                           ();
    int            GET_RECORD                           (int record_id);
    int            SELECT_RECORD                        ();
    int            FIND_RECORD                          ();

    int            CHECK_VAR                            (QObject * object);
    int            CHECK_EMPTY                          ();

    int            CHECK_ADD                            ();
    int            ADD_RECORD                           ();
    int            CHECK_UPDATE                         (int record_id);
    void           UPDATE_RECORD                        (int record_id);
    int            CHECK_DELETE                         (int record_id);
    void           DELETE_RECORD                        (int record_id);

    int            FIND_NEXT_RECORD                     ();
    int            FIND_PREV_RECORD                     ();
    int            FIND_FIRST_RECORD                    ();
    int            FIND_LAST_RECORD                     ();

    int            LOCK_RECORD                          (int record_id);
    void           UNLOCK_RECORD                        (int record_id);
    void           SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);

    int            CHANGER_BUTTON_CLICKED               ( QAbstractButton *button );
    void           SAVER_BUTTON_CLICKED                 ( QAbstractButton *button, int record_id);

};

#endif // CARI_VIRMAN_FORMU_H
