#ifndef STOK_HIZMET_KARTI_FORMU_H
#define STOK_HIZMET_KARTI_FORMU_H

#include <form_kernel.h>

namespace Ui {
    class STOK_HIZMET_KARTI_FORMU;
}

class STOK_HIZMET_KARTI_FORMU : public FORM_KERNEL
{
    Q_OBJECT

public:

    STOK_HIZMET_KARTI_FORMU(QWidget *parent = 0);
    ~ STOK_HIZMET_KARTI_FORMU(){}


private:

    Ui::STOK_HIZMET_KARTI_FORMU  *       m_ui;
    QString                       m_temel_para_birimi_kodu;
    int                           m_muh_hesap_id;

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

    void           SAVER_BUTTON_CLICKED                 (QAbstractButton *button, int record_id);

public:
    void                SHOW_REKLAM();

};

#endif // STOK_HIZMET_KARTI_FORMU_H
