#ifndef BNK_HESABIN_KREDI_KARTLARI_FISI_CLASS_H
#define BNK_HESABIN_KREDI_KARTLARI_FISI_CLASS_H


#include <fis_kernel.h>

namespace Ui {
    class BNK_HESABIN_KREDI_KARTLARI_FISI;
}


class BNK_HESABIN_KREDI_KARTLARI_FISI : public FIS_KERNEL
{
    Q_OBJECT;

public:

     BNK_HESABIN_KREDI_KARTLARI_FISI                                                  ( int fis_id ,QWidget * parent = 0 );
    ~BNK_HESABIN_KREDI_KARTLARI_FISI                                                  (){}

private:

    Ui::BNK_HESABIN_KREDI_KARTLARI_FISI * m_ui;

    int                     m_num_of_columns;

    bool                    m_bordro_islem_turu_degisti_mi;

    int                     m_start_record_id;

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

    int                     CHECK_EXIT                               ();

    void                    LINE_SEARCH_EDIT_CLICKED(int row_number,QWidget * widget,QLineEdit * line_edit);

};

#endif // BNK_HESABIN_KREDI_KARTLARI_FISI_H
