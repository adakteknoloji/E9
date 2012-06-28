#ifndef BNK_ACILIS_FORMU_CLASS_H
#define BNK_ACILIS_FORMU_CLASS_H

#include <form_kernel.h>

struct     BNK_HAREKET_FIS_STRUCT;
struct     BNK_HAREKET_FIS_SATIRI_STRUCT;

namespace Ui {
    class BNK_ACILIS_FORMU;
}

class BNK_ACILIS_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
    BNK_ACILIS_FORMU                                           ( QWidget * parent = 0 );
    ~ BNK_ACILIS_FORMU                                         (){}

private:
    int  m_bnk_hesap_no_id;
    int  m_banka_hesabi_doviz_id;
    int  m_fis_turu;
    int  m_ent_filtresi_turu;


private:
    BNK_HAREKET_FIS_STRUCT         * M_FIS;
    BNK_HAREKET_FIS_SATIRI_STRUCT  * M_FIS_SATIRI;

private:

    Ui::BNK_ACILIS_FORMU  *       m_ui;

    void           SETUP_FORM                           ();
    void           NEW_RECORD                           ();
    void           CLEAR_FORM_MEMBERS			();
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
    void           SEARCH_EDIT_CLICKED                  ( QWidget *, QLineEdit * line_edit);


    void           DESIGNER_ONAR                        ();
};

#endif // BNK_ACILIS_FORMU_H
