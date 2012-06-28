#ifndef SUBE_DEPOLARI_FORMU_H
#define SUBE_DEPOLARI_FORMU_H

#include <form_kernel.h>

namespace Ui {
    class SUBE_DEPOLARI_FORMU;
}

class SUBE_DEPOLARI_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
                  SUBE_DEPOLARI_FORMU                                    ( int p_sube_id, int p_depo_id, QWidget * p_parent = 0 );
                  ~SUBE_DEPOLARI_FORMU                                   () {}

private :

    Ui::SUBE_DEPOLARI_FORMU     *   m_ui;
    int                     m_depo_id;
    int                     m_sube_id;
    QString                 m_sube_kodu_adi;

    void                    SETUP_FORM                           ();
    void                    CLEAR_FORM_MEMBERS                   ();
    void                    NEW_RECORD                           ();
    int                     GET_RECORD                           ( int p_depo_id );
    int                     SELECT_RECORD                        ();
    int                     FIND_RECORD                          ();

    int                     CHECK_VAR                            ( QObject * p_object );
    int                     CHECK_EMPTY                          ();

    int                     CHECK_ADD                            ();
    int                     ADD_RECORD                           ();
    int                     CHECK_UPDATE                         ( int p_depo_id );
    void                    UPDATE_RECORD                        ( int p_depo_id );
    int                     CHECK_DELETE                         ( int p_depo_id );
    void                    DELETE_RECORD                        ( int p_depo_id );

    int                     FIND_NEXT_RECORD                     ();
    int                     FIND_PREV_RECORD                     ();
    int                     FIND_FIRST_RECORD                    ();
    int                     FIND_LAST_RECORD                     ();

    int                     LOCK_RECORD                          ( int p_depo_id );
    void                    UNLOCK_RECORD                        ( int p_depo_id );

};

#endif // SUBE_DEPOLARI_FORMU_H
