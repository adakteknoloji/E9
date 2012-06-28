#ifndef SUBE_UNITE_DETAYLARI_FORMU_H
#define SUBE_UNITE_DETAYLARI_FORMU_H

#include <form_kernel.h>

namespace Ui {
    class SUBE_UNITE_DETAYLARI_FORMU;
}

class SUBE_UNITE_DETAYLARI_FORMU : public FORM_KERNEL
{
    Q_OBJECT

public :

    SUBE_UNITE_DETAYLARI_FORMU  ( int unite_id, int p_modul_id, QWidget * parent = 0 );
    ~SUBE_UNITE_DETAYLARI_FORMU () {}

private :

    Ui::SUBE_UNITE_DETAYLARI_FORMU  *       m_ui;

    int            m_kasa_unite_id;
    int            m_modul_id;

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


};

#endif // SUBE_UNITE_DETAYLARI_FORMU_H
