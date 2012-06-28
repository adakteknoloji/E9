#ifndef SUBE_FISI_H
#define SUBE_FISI_H

#include <fis_kernel.h>

namespace Ui {
    class SUBE_FISI;
}

class SUBE_FISI : public FIS_KERNEL

{
    Q_OBJECT;

public:

                            SUBE_FISI                         ( QWidget * p_parent );
                            ~SUBE_FISI                        (){}

private:

    Ui::SUBE_FISI *       m_ui;

    int                     m_sube_id;

    void                    SETUP_FORM                                  ();
    void                    CLEAR_FORM_MEMBERS                          ();
    void                    NEW_FIS_RECORD                              ();
    int                     GET_FIS_RECORD                              ( int fis_id );

    int                     CHECK_FIS_FORM_VAR                          ( QObject * object );
    int                     CHECK_FIS_FORM_EMPTY                        ();

    int                     CHECK_FIS_RECORD_ADD                        ();
    int                     ADD_FIS_RECORD                              ();
    int                     CHECK_FIS_RECORD_UPDATE                     ( int fis_id );
    void                    UPDATE_FIS_RECORD                           ( int fis_id );
    int                     CHECK_FIS_RECORD_DELETE                     ( int fis_id );
    void                    DELETE_FIS_RECORD                           ( int fis_id );

    int                     SELECT_FIS_RECORD                           ();
    int                     FIND_FIS_RECORD                             ();
    int                     FIND_NEXT_FIS_RECORD                        ();
    int                     FIND_PREV_FIS_RECORD                        ();
    int                     FIND_FIRST_FIS_RECORD                       ();
    int                     FIND_LAST_FIS_RECORD                        ();

    int                     LOCK_FIS_RECORD                             ( int fis_id );
    void                    UNLOCK_FIS_RECORD                           ( int fis_id );

    void                    SET_LINE_DEFAULTS                           ( int row_number );
    int                     CHECK_LINE_VAR                              ( int row_number, QObject * object );
    int                     CHECK_LINE_EMPTY                            ( int row_number );
    int                     CHECK_ADD_LINE                              ( int fis_id, int row_number );
    void                    ADD_LINE                                    ( int fis_id, int row_number );
    int                     CHECK_UPDATE_LINE                           ( int fis_id, int row_number );
    void                    UPDATE_LINE                                 ( int fis_id, int row_number );
    int                     CHECK_DELETE_LINE                           ( int fis_id, int row_number );
    void                    DELETE_LINE                                 ( int fis_id, int row_number );

    void                    LINE_SEARCH_EDIT_CLICKED                    ( int current_row, QWidget * searchEdit,
                                                                          QLineEdit * lineEdit );

    bool                    SECILI_BILGISAYAR_BASKA_UNITEYE_KAYITLI_MI  ( int p_bilgisayar_id, int p_kasa_unite_id = -1 );


    void                    SAVER_BUTTON_CLICKED     (QAbstractButton *button, int record_id);
    void                    LINE_SAVER_BUTTON_CLICKED(QAbstractButton *button, int row_id, int record_id, int row_number);

public:
    void                    SHOW_REKLAM();


};

#endif // SUBE_FISI_H
