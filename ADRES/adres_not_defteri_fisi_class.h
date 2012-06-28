#ifndef ADRES_NOT_DEFTERI_FISI_H
#define ADRES_NOT_DEFTERI_FISI_H

#include <fis_kernel.h>

namespace Ui {
    class ADRES_NOT_DEFTERI_FISI;
}


class ADRES_NOT_DEFTERI_FISI : public FIS_KERNEL
{
    Q_OBJECT;

public:
     ADRES_NOT_DEFTERI_FISI                                     ( int record_id , int modul_id, int program_id, QWidget * parent = 0 );
    ~ADRES_NOT_DEFTERI_FISI                                     (){}

private:

    Ui::ADRES_NOT_DEFTERI_FISI * m_ui;

    int                     m_start_record_id;

    int                     m_modul_id;
    int                     m_program_id;

    void                    SETUP_FORM                             ();
    void                    CLEAR_FORM_MEMBERS                     ();
    void                    NEW_FIS_RECORD                         ();
    int                     GET_FIS_RECORD                         (int record_id);

    int                     CHECK_FIS_FORM_VAR                     (QObject * object);
    int                     CHECK_FIS_FORM_EMPTY                   ();

    int                     CHECK_FIS_RECORD_ADD                   ();
    int                     ADD_FIS_RECORD                         ();
    int                     CHECK_FIS_RECORD_UPDATE                (int record_id);
    void                    UPDATE_FIS_RECORD                      (int record_id);
    int                     CHECK_FIS_RECORD_DELETE                (int record_id);
    void                    DELETE_FIS_RECORD                      (int record_id);

    int                     SELECT_FIS_RECORD                      ();
    int                     FIND_FIS_RECORD                        ();
    int                     FIND_NEXT_FIS_RECORD                   ();
    int                     FIND_PREV_FIS_RECORD                   ();
    int                     FIND_FIRST_FIS_RECORD                  ();
    int                     FIND_LAST_FIS_RECORD                   ();

    int                     LOCK_FIS_RECORD                        (int record_id);
    void                    UNLOCK_FIS_RECORD                      (int record_id);

    void                    SET_LINE_DEFAULTS                      ( int row_number );
    int                     CHECK_LINE_VAR                         ( int row_number, QObject * object );
    int                     CHECK_LINE_EMPTY                       ( int row_number );
    int                     CHECK_ADD_LINE                         ( int record_id, int row_number );
    void                    ADD_LINE                               ( int record_id, int row_number );
    int                     CHECK_UPDATE_LINE                      ( int record_id, int row_number );
    void                    UPDATE_LINE                            ( int record_id, int row_number );
    int                     CHECK_DELETE_LINE                      ( int record_id, int row_number );
    void                    DELETE_LINE                            ( int record_id, int row_number );
};

#endif // ADRES_NOT_DEFTERI_FISI_H
