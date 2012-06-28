#ifndef DMR_YENIDEN_DEGERLEME_FISI_H
#define DMR_YENIDEN_DEGERLEME_FISI_H

#include <fis_kernel.h>

struct DMR_FIS_STRUCT ;
struct DMR_FIS_SATIRI_STRUCT;

namespace Ui {
    class DMR_YENIDEN_DEGERLEME_FISI;
}


class DMR_YENIDEN_DEGERLEME_FISI : public FIS_KERNEL

{
    Q_OBJECT;

public:

     DMR_YENIDEN_DEGERLEME_FISI                                                  (int, QWidget * parent = 0 );
    ~DMR_YENIDEN_DEGERLEME_FISI                                                  (){}

private:

    Ui::DMR_YENIDEN_DEGERLEME_FISI * m_ui;

    DMR_FIS_STRUCT *        M_FIS;
    DMR_FIS_SATIRI_STRUCT * M_FIS_SATIRI;

    bool                    m_log_kaydi_eklenecek_mi;
    bool                    m_kayit_eklendi;
    bool                    m_kayit_silindi;
    QString                 m_log_detaylari;

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

    void                    LINE_SEARCH_EDIT_CLICKED(int row_number,QWidget *,QLineEdit * );

    int                     LEAVE_RECORD                             ();
};

#endif // DMR_ALT_GRUP_EKLE_H
