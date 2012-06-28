#ifndef BNK_HESAPLARI_FISI_CLASS_H
#define BNK_HESAPLARI_FISI_CLASS_H

#include <fis_kernel.h>

namespace Ui {
    class BNK_HESAPLARI_FISI;
}

class BNK_HESAPLARI_FISI : public FIS_KERNEL
{
    Q_OBJECT;

public:
     BNK_HESAPLARI_FISI                                        ( int, QWidget * parent = 0 );
    ~BNK_HESAPLARI_FISI                                        (){}

private:
    Ui::BNK_HESAPLARI_FISI * m_ui;

    int                     m_bnk_muh_hesap_id;
    int                     m_bnk_verilen_cekler_hesap_id;
    int                     m_bnk_odeme_emirleri_hesap_id;

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
    void                    SEARCH_EDIT_CLICKED                      (QWidget *widget, QLineEdit *line_edit);
    void                    LINE_SEARCH_EDIT_CLICKED                 ( int row_number, QWidget *, QLineEdit *);
    void                    SAVER_BUTTON_CLICKED                     (QAbstractButton *button, int record_id);

    bool                    BANKA_HESABI_ISLEM_GORDU_MU              (int hesap_no_id);

    //int                     CHECK_EXIT                               ();
};

#endif // BNK_HESAPLARI_FISI_H
