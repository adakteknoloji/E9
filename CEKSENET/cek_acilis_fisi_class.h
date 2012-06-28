#ifndef CEK_ACILIS_FISI_H
#define CEK_ACILIS_FISI_H

#include <fis_kernel.h>

struct CEK_BORDRO_STRUCT;
struct CEK_BORDRO_SATIRI_STRUCT;
struct CEK_SENET_STRUCT;

namespace Ui {
    class CEK_ACILIS_FISI;
}

class CEK_ACILIS_FISI : public FIS_KERNEL

{
    Q_OBJECT;

public:

     CEK_ACILIS_FISI                                                  ( int p_record_id, QWidget * p_parent = 0 );
    ~CEK_ACILIS_FISI                                                  (){}

private:

    Ui::CEK_ACILIS_FISI * m_ui;

    CEK_BORDRO_STRUCT        * M_BORDRO;
    CEK_BORDRO_SATIRI_STRUCT * M_BORDRO_SATIRI;
    CEK_SENET_STRUCT         * M_CEK_SENET;


    int                     m_start_record_id;
    int                     m_bordro_islem_turu;
    int                     m_cek_acilis_bordrosu_islem_turu;
    int                     m_cek_mi_senet_mi;
    int                     m_musteri_cek_senedi_mi;

    bool                    m_log_kaydi_eklenmeli_mi;
    QString                 m_log_detaylari;
    bool                    m_kayit_eklendi;
    bool                    m_kayit_silindi;
    int                     m_hesap_no_id;
    QString                 m_frm_bnk_ismi;
    QString                 m_frm_sube_kodu;
    QString                 m_frm_sube_ismi;
    int                     m_frm_bnk_doviz_id;


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
    void                    SELECTED_LINE                            ( int line_id,int row_save_status);

    void                    LINE_SEARCH_EDIT_CLICKED                  ( int row_number, QWidget * p_widget ,QLineEdit * p_lineEdit );

    int                     LEAVE_RECORD();

    void                    FIS_EKRANINI_DUZENLE                        ();

    void                    EKRANI_GUNCELLE                             ();
};

#endif // CEK_ACILIS_FISI_H
