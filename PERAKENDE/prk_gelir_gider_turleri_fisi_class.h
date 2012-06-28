#ifndef PRK_GELIR_GIDER_TURLERI_FISI_CLASS_H
#define PRK_GELIR_GIDER_TURLERI_FISI_CLASS_H

#include <fis_kernel.h>
#include "ui_prk_gelir_gider_turleri_fisi.h"

namespace ui {
    class PRK_GELIR_GIDER_TURLERI_FISI;
}

class PRK_GELIR_GIDER_TURLERI_FISI : public FIS_KERNEL

{
    Q_OBJECT;

public:

     PRK_GELIR_GIDER_TURLERI_FISI                                                  ( int sube_id,int gider_turu_mu,QWidget * parent = 0 );
    ~PRK_GELIR_GIDER_TURLERI_FISI                                                  (){}

private:

    Ui::PRK_GELIR_GIDER_TURLERI_FISI   *        m_ui;
    int                     m_sube_id;
    int                     m_gider_turu_mu;

    void                    SETUP_FORM                                 ();
    void                    CLEAR_FORM_MEMBERS                       ();
    void                    NEW_FIS_RECORD                           ();
    int                     GET_FIS_RECORD                           (int fis_id);

    int                     CHECK_FIS_FORM_VAR                       (QObject * object);
    int                     CHECK_FIS_FORM_EMPTY                     ();

    int                     CHECK_FIS_RECORD_ADD                     ();
    int                     ADD_FIS_RECORD                           ();
    int                     CHECK_FIS_RECORD_UPDATE                  (int fis_id);
    void                    UPDATE_FIS_RECORD                        (int fis_id);
    int                     CHECK_FIS_RECORD_DELETE                  (int fis_id);
    void                    DELETE_FIS_RECORD                        (int fis_id);

    int                     SELECT_FIS_RECORD                        ();
    int                     FIND_FIS_RECORD                          ();
    int                     FIND_NEXT_FIS_RECORD                     ();
    int                     FIND_PREV_FIS_RECORD                     ();
    int                     FIND_FIRST_FIS_RECORD                    ();
    int                     FIND_LAST_FIS_RECORD                     ();

    int                     LOCK_FIS_RECORD                          (int fis_id);
    void                    UNLOCK_FIS_RECORD                        (int fis_id);

    void                    SET_LINE_DEFAULTS                        ( int row_number );
    int                     CHECK_LINE_VAR                           ( int row_number, QObject * object );
    int                     CHECK_LINE_EMPTY                         ( int row_number );
    int                     CHECK_ADD_LINE                           ( int fis_id, int row_number );
    void                    ADD_LINE                                 ( int fis_id, int row_number );
    int                     CHECK_UPDATE_LINE                        ( int fis_id, int row_number );
    void                    UPDATE_LINE                              ( int fis_id, int row_number );
    int                     CHECK_DELETE_LINE                        ( int fis_id, int row_number );
    void                    DELETE_LINE                              ( int fis_id, int row_number );

    void                   LINE_SEARCH_EDIT_CLICKED                  ( int row_number , QWidget *, QLineEdit *);

};

#endif // PRK_GELIR_GIDER_TURLERI_FISI_H
