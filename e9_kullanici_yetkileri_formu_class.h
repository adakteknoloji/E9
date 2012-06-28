#ifndef E9_KULLANICI_YETKILERI_FORMU_H
#define E9_KULLANICI_YETKILERI_FORMU_H

#include <form_kernel.h>

struct E9_KULLANICI_YETKILERI_STRUCT;

namespace Ui {
    class E9_KULLANICI_YETKILERI_FORMU;
}

class E9_KULLANICI_YETKILERI_FORMU : public FORM_KERNEL
{
    Q_OBJECT

public:

    E9_KULLANICI_YETKILERI_FORMU                                 ( int p_kullanici_id, bool * yetkileri_tazele, QWidget *parent = 0 );
    ~E9_KULLANICI_YETKILERI_FORMU                                () {}

private:

    Ui::E9_KULLANICI_YETKILERI_FORMU  *       m_ui;
    int            m_yetki_id;
    bool *         m_yetkileri_tazele;

    int            m_kullanici_id;



    void           SETUP_FORM                           ();
    void           CLEAR_FORM_MEMBERS                   ();
    void           NEW_RECORD                           ();
    int            GET_RECORD                           (int record_id);
    int            FIND_RECORD                          ();
    int            SELECT_RECORD                        ();

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

    void           TUM_COMBOXLARI_DEGISTIR              ( int value );

    void           EKRANI_DUZENLE                       ();

    int            CHANGER_BUTTON_CLICKED               ( QAbstractButton * p_button );

};


#endif // E9_KULLANICI_YETKILERI_FORMU_H
