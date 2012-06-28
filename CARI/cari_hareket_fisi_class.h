#ifndef CARI_HAREKET_FISI_H
#define CARI_HAREKET_FISI_H

#include <fis_kernel.h>

struct CARI_FIS_STRUCT;
struct CARI_FIS_SATIRI_STRUCT;
struct E9_ENT_DETAYLARI_STRUCT;

namespace Ui {
    class CARI_HAREKET_FISI;
}

class CARI_HAREKET_FISI : public FIS_KERNEL

{
    Q_OBJECT;

public:

     CARI_HAREKET_FISI                                                  ( int fis_id, int fis_turu, int modul_id, QWidget * parent = 0 );
    ~CARI_HAREKET_FISI                                                  (){}

private:

    Ui::CARI_HAREKET_FISI * m_ui;

    int                         m_fis_id;
    int                         m_fis_turu;
    int                         m_modul_id;
    int                         m_ent_filtresi_turu;

    bool                        m_log_kaydi_eklenecek_mi;
    bool                        m_kayit_eklendi;
    bool                        m_kayit_silindi;
    QString                     m_log_detaylari;
    bool                        m_muh_entegrasyonu;

    int                         m_temel_para_birimi_id;

    CARI_FIS_STRUCT         *   M_FIS;
    CARI_FIS_SATIRI_STRUCT  *   M_FIS_SATIRI;

    QStringList                 M_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI;
    QStringList                 M_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI;

    E9_ENT_DETAYLARI_STRUCT  *  M_ENT_DETAYLARI;

    void                        SETUP_FORM                                  ();
    void                        NEW_FIS_RECORD                              ();
    void                        CLEAR_FORM_MEMBERS                          ();
    int                         GET_FIS_RECORD                              ( int fis_id );

    int                         CHECK_FIS_FORM_VAR                          (QObject * object);
    int                         CHECK_FIS_FORM_EMPTY                        ();

    int                         CHECK_FIS_RECORD_ADD                        ();
    int                         ADD_FIS_RECORD                              ();
    int                         CHECK_FIS_RECORD_UPDATE                     (int fis_id);
    void                        UPDATE_FIS_RECORD                           (int fis_id);
    int                         CHECK_FIS_RECORD_DELETE                     (int fis_id);
    void                        DELETE_FIS_RECORD                           (int fis_id);

    int                         SELECT_FIS_RECORD                           ();
    int                         FIND_FIS_RECORD                             ();
    int                         FIND_NEXT_FIS_RECORD                        ();
    int                         FIND_PREV_FIS_RECORD                        ();
    int                         FIND_FIRST_FIS_RECORD                       ();
    int                         FIND_LAST_FIS_RECORD                        ();

    int                         LOCK_FIS_RECORD                             (int fis_id);
    void                        UNLOCK_FIS_RECORD                           (int fis_id);

    void                        SET_LINE_DEFAULTS                           ( int row_number );
    int                         CHECK_LINE_VAR                              ( int row_number, QObject * object );
    int                         CHECK_LINE_EMPTY                            ( int row_number );
    int                         CHECK_ADD_LINE                              ( int fis_id, int row_number );
    void                        ADD_LINE                                    ( int fis_id, int row_number );
    int                         CHECK_UPDATE_LINE                           ( int fis_id, int row_number );
    void                        UPDATE_LINE                                 ( int fis_id, int row_number );
    int                         CHECK_DELETE_LINE                           ( int fis_id, int row_number );
    void                        DELETE_LINE                                 ( int fis_id, int row_number );

    int                         LEAVE_RECORD                                ();

    void                        LINE_SEARCH_EDIT_CLICKED                    ( int row_number, QWidget*,QLineEdit*);

    void                        SAVER_BUTTON_CLICKED                        ( QAbstractButton * p_button, int p_record_id );
    int                         CHANGER_BUTTON_CLICKED                      ( QAbstractButton * p_button );

};

#endif // CARI_HAREKET_FISI_H
