#ifndef CARI_ODEME_FISI_H
#define CARI_ODEME_FISI_H

#include <fis_kernel.h>

struct CARI_FIS_STRUCT;
struct CARI_FIS_SATIRI_STRUCT;
struct E9_ENT_DETAYLARI_STRUCT;
struct E9_ISLEM_DETAYLARI;

namespace Ui {
    class CARI_ODEME_FISI;
}

class CARI_ODEME_FISI : public FIS_KERNEL

{
    Q_OBJECT

public:

    CARI_ODEME_FISI  ( int fis_id, int p_modul_id, QWidget * parent = 0 );
    ~CARI_ODEME_FISI ();

private:

    Ui::CARI_ODEME_FISI * m_ui;

    CARI_FIS_STRUCT         *   M_FIS;
    CARI_FIS_SATIRI_STRUCT  *   M_FIS_SATIRI;

    int                     m_fis_id;
    E9_ENT_DETAYLARI_STRUCT * M_FIS_DETAYLARI;
    E9_ISLEM_DETAYLARI * M_ISLEM_DETAYLARI;

    int                     m_ent_filtresi_turu;

    int                     m_modul_id;
    bool                    m_log_kaydi_eklenecek_mi;
    bool                    m_kayit_eklendi;
    bool                    m_kayit_silindi;
    QString                 m_log_detaylari;
    int                     m_cari_hesap_id;
    int                     m_doviz_cinsi_id;

    QString                 m_temel_para_birimi_kodu;
    int                     m_temel_para_birimi_id;
    int                     m_cari_para_birimi_id;
    int                     m_makbuz_para_birimi_id;

    void                    SETUP_FORM                               ();

    void                    CLEAR_FORM_MEMBERS                       ();
    void                    NEW_FIS_RECORD                           ();
    int                     GET_FIS_RECORD                           ( int fis_id );

    int                     CHECK_FIS_FORM_VAR                       ( QObject * object );
    int                     CHECK_FIS_FORM_EMPTY                     ();

    int                     CHECK_FIS_RECORD_ADD                     ();
    int                     ADD_FIS_RECORD                           ();
    int                     CHECK_FIS_RECORD_UPDATE                  ( int fis_id );
    void                    UPDATE_FIS_RECORD                        ( int fis_id );
    int                     CHECK_FIS_RECORD_DELETE                  ( int fis_id );
    void                    DELETE_FIS_RECORD                        ( int fis_id );

    int                     SELECT_FIS_RECORD                        ();
    int                     FIND_FIS_RECORD                          ();
    int                     FIND_NEXT_FIS_RECORD                     ();
    int                     FIND_PREV_FIS_RECORD                     ();
    int                     FIND_FIRST_FIS_RECORD                    ();
    int                     FIND_LAST_FIS_RECORD                     ();

    int                     LOCK_FIS_RECORD                          ( int fis_id );
    void                    UNLOCK_FIS_RECORD                        ( int fis_id );

    void                    SET_LINE_DEFAULTS                        ( int row_number );
    int                     CHECK_LINE_VAR                           ( int row_number, QObject * object );
    int                     CHECK_LINE_EMPTY                         ( int row_number );
    int                     CHECK_ADD_LINE                           ( int fis_id, int row_number );
    void                    ADD_LINE                                 ( int fis_id, int row_number );
    int                     CHECK_UPDATE_LINE                        ( int fis_id, int row_number );
    void                    UPDATE_LINE                              ( int fis_id, int row_number );
    int                     CHECK_DELETE_LINE                        ( int fis_id, int row_number );
    void                    DELETE_LINE                              ( int fis_id, int row_number );

    void                    SELECTED_LINE                            ( int row_number, int row_save_status );
    void                    LINE_SEARCH_EDIT_CLICKED                 ( int row_number, QWidget * widget ,QLineEdit * lineEdit);

    bool                    FIS_GUNCELLENEBILIR_MI_SILINEBILIR_MI    ( int fis_id );
    
    void                    SEARCH_EDIT_CLICKED                      ( QWidget * widget , QLineEdit * lineEdit );

    int                     LEAVE_RECORD();

    void                    SAVER_BUTTON_CLICKED                     ( QAbstractButton *button, int record_id);
    int                     CHANGER_BUTTON_CLICKED                   ( QAbstractButton *button );

    int                     KENDI_CEKLERIMIZ                         ();
    int                     KENDI_SENETLERIMIZ                       ();

    int                     LINE_CHANGER_BUTTON_CLICKED              ( QAbstractButton *button, int row_number);

    void                    EKRANI_GUNCELLE ();

public:
    void                SHOW_REKLAM();

};

#endif // CARI_ODEME_FISI_H
