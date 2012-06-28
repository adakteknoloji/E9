#ifndef DMR_SATIS_FORMU_H
#define DMR_SATIS_FORMU_H

#include <form_kernel.h>

namespace Ui {
    class DMR_SATIS_FORMU;
}

class DMR_SATIS_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
    DMR_SATIS_FORMU                                           ( QWidget * parent = 0 );
    ~ DMR_SATIS_FORMU                                         (){}

private:

    Ui::DMR_SATIS_FORMU  *       m_ui;

    int            m_demirbas_id;
    int            m_fatura_id;

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

    void           SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);

    int            CHANGER_BUTTON_CLICKED               (QAbstractButton *button);
};

#endif // DMR_SATIS_FORMU_H
