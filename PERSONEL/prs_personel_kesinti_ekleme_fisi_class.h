#ifndef PRS_PERSONEL_KESINTI_EKLEME_FISI_CLASS_H
#define PRS_PERSONEL_KESINTI_EKLEME_FISI_CLASS_H

#include <fis_kernel.h>
#include "ui_prs_personel_kesinti_ekleme_fisi.h"

class PRS_PERSONEL_KESINTI_EKLEME_FISI : public FIS_KERNEL , public Ui::PRS_PERSONEL_KESINTI_EKLEME_FISI

{
    Q_OBJECT;

public:

     PRS_PERSONEL_KESINTI_EKLEME_FISI               ( int who_called, int record_id, QWidget * parent = 0 );
    ~PRS_PERSONEL_KESINTI_EKLEME_FISI               (){}

private:

    int                     m_who_called;
    int                     m_record_id;

private:
    void                    SETUP_FORM                               ();
    void                    CLEAR_FORM_MEMBERS                       ();
    void                    NEW_FIS_RECORD                           ();
    int                     GET_FIS_RECORD                           ( int record_id );

    int                     CHECK_FIS_FORM_VAR                       ( QObject * object );
    int                     CHECK_FIS_FORM_EMPTY                     ();

    int                     CHECK_FIS_RECORD_ADD                     ();
    int                     ADD_FIS_RECORD                           ();
    int                     CHECK_FIS_RECORD_UPDATE                  ( int record_id );
    void                    UPDATE_FIS_RECORD                        ( int record_id );
    int                     CHECK_FIS_RECORD_DELETE                  ( int record_id );
    void                    DELETE_FIS_RECORD                        ( int record_id );

    int                     SELECT_FIS_RECORD                        ();
    int                     FIND_FIS_RECORD                          ();
    int                     FIND_NEXT_FIS_RECORD                     ();
    int                     FIND_PREV_FIS_RECORD                     ();
    int                     FIND_FIRST_FIS_RECORD                    ();
    int                     FIND_LAST_FIS_RECORD                     ();

    int                     LOCK_FIS_RECORD                          ( int record_id );
    void                    UNLOCK_FIS_RECORD                        ( int record_id );

    void                    SET_LINE_DEFAULTS                        ( int row_number );
    int                     CHECK_LINE_VAR                           ( int row_number, QObject * object );
    int                     CHECK_LINE_EMPTY                         ( int row_number );
    int                     CHECK_ADD_LINE                           ( int record_id, int row_number );
    void                    ADD_LINE                                 ( int record_id, int row_number );
    int                     CHECK_UPDATE_LINE                        ( int record_id, int row_number );
    void                    UPDATE_LINE                              ( int record_id, int row_number );
    int                     CHECK_DELETE_LINE                        ( int record_id, int row_number );
    void                    DELETE_LINE                              ( int record_id, int row_number );

    int                     CHECK_EXIT                               ();
    void                    SET_WIDGETS_MAXIMUM_SIZE                 ();
};

#endif // PRS_PERSONEL_KESINTI_EKLEME_FISI_H
