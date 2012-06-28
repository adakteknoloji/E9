#ifndef SUBE_ENTEGRASYON_HESAPLARI_FORMU_H
#define SUBE_ENTEGRASYON_HESAPLARI_FORMU_H

#include <form_kernel.h>

namespace Ui {
    class SUBE_ENTEGRASYON_HESAPLARI_FORMU;
}

class SUBE_ENTEGRASYON_HESAPLARI_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:

    SUBE_ENTEGRASYON_HESAPLARI_FORMU                          ( int sube_id, QWidget * parent = 0 );
    ~ SUBE_ENTEGRASYON_HESAPLARI_FORMU                        (){}

private :

    Ui::SUBE_ENTEGRASYON_HESAPLARI_FORMU  *       m_ui;
    int                                           m_sube_id;

    QSearchEdit **  M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY;
    QLineEdit   **  M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY;


    void           SETUP_FORM                           ();
    void           CLEAR_FORM_MEMBERS                   ();
    void           NEW_RECORD                           ();
    int            GET_RECORD                           ( int record_id );
    int            SELECT_RECORD                        ();
    int            FIND_RECORD                          ();

    int            CHECK_VAR                            ( QObject * object );
    int            CHECK_EMPTY                          ();

    int            CHECK_ADD                            ();
    int            ADD_RECORD                           ();
    int            CHECK_UPDATE                         ( int record_id );
    void           UPDATE_RECORD                        ( int record_id );
    int            CHECK_DELETE                         ( int record_id );
    void           DELETE_RECORD                        ( int record_id );

    int            FIND_NEXT_RECORD                     ();
    int            FIND_PREV_RECORD                     ();
    int            FIND_FIRST_RECORD                    ();
    int            FIND_LAST_RECORD                     ();

    int            LOCK_RECORD                          ( int record_id );
    void           UNLOCK_RECORD                        ( int record_id );

    void           SEARCH_EDIT_CLICKED                  ( QWidget * searchEdit, QLineEdit * lineEdit );

    void           WIDGET_DIZILERINI_DUZENLE            ();

};

#endif // SUBE_ENTEGRASYON_HESAPLARI_FORMU_H
