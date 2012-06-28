#ifndef ADRES_EMAIL_FISI_H
#define ADRES_EMAIL_FISI_H

#include <fis_kernel.h>

namespace Ui {
    class ADRES_EMAIL_FISI;
}

class ADRES_EMAIL_FISI : public FIS_KERNEL

{
    Q_OBJECT;

public:

     ADRES_EMAIL_FISI                                                ( int p_kayit_id, QWidget * p_parent = 0 );
    ~ADRES_EMAIL_FISI                                                (){}

private:

    Ui::ADRES_EMAIL_FISI *  m_ui;

    int                     m_start_fis_kernel;

    void                    SETUP_FORM                               ();

    void                    CLEAR_FORM_MEMBERS                       ();
    void                    NEW_FIS_RECORD                           ();
    int                     GET_FIS_RECORD                           ( int p_kayit_id );

    int                     CHECK_FIS_FORM_VAR                       ( QObject * p_object );
    int                     CHECK_FIS_FORM_EMPTY                     ();

    int                     CHECK_FIS_RECORD_ADD                     ();
    int                     ADD_FIS_RECORD                           ();
    int                     CHECK_FIS_RECORD_UPDATE                  ( int p_kayit_id );
    void                    UPDATE_FIS_RECORD                        ( int p_kayit_id );
    int                     CHECK_FIS_RECORD_DELETE                  ( int p_kayit_id );
    void                    DELETE_FIS_RECORD                        ( int p_kayit_id );

    int                     SELECT_FIS_RECORD                        ();
    int                     FIND_FIS_RECORD                          ();
    int                     FIND_NEXT_FIS_RECORD                     ();
    int                     FIND_PREV_FIS_RECORD                     ();
    int                     FIND_FIRST_FIS_RECORD                    ();
    int                     FIND_LAST_FIS_RECORD                     ();

    int                     LOCK_FIS_RECORD                          ( int p_kayit_id );
    void                    UNLOCK_FIS_RECORD                        ( int p_kayit_id );

    void                    SET_LINE_DEFAULTS                        ( int p_row_number );
    int                     CHECK_LINE_VAR                           ( int p_row_number, QObject * p_object );
    int                     CHECK_LINE_EMPTY                         ( int p_row_number );
    int                     CHECK_ADD_LINE                           ( int p_kayit_id, int p_row_number );
    void                    ADD_LINE                                 ( int p_kayit_id, int p_row_number );
    int                     CHECK_UPDATE_LINE                        ( int p_kayit_id, int p_row_number );
    void                    UPDATE_LINE                              ( int p_kayit_id, int p_row_number );
    int                     CHECK_DELETE_LINE                        ( int p_kayit_id, int p_row_number );
    void                    DELETE_LINE                              ( int p_kayit_id, int p_row_number );

};

#endif // ADRES_EMAIL_FISI_H
