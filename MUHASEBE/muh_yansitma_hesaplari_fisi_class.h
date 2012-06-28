#ifndef MUH_YANSITMA_HESAPLARI_FISI_H
#define MUH_YANSITMA_HESAPLARI_FISI_H

#include <fis_kernel.h>
#include "ui_muh_yansitma_hesaplari_fisi.h"

class MUH_YANSITMA_HESAPLARI_FISI : public FIS_KERNEL , public Ui::MUH_YANSITMA_HESAPLARI_FISI

{
    Q_OBJECT;

public:

     MUH_YANSITMA_HESAPLARI_FISI                                                  ( QWidget * parent = 0 );
    ~MUH_YANSITMA_HESAPLARI_FISI                                                  (){}

private:
    void                    SETUP_FORM                               ();
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
    void                    LINE_SEARCH_EDIT_CLICKED                 ( int row_number, QWidget *, QLineEdit *);

    //int                     CHECK_EXIT                               ();
};

#endif // MUH_YANSITMA_HESAPLARI_FISI_H
