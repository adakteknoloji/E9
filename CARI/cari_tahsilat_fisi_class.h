#ifndef CARI_TAHSILAT_FISI_H
#define CARI_TAHSILAT_FISI_H

#include <fis_kernel.h>

struct CARI_FIS_STRUCT;
struct CARI_FIS_SATIRI_STRUCT;
struct E9_ENT_DETAYLARI_STRUCT;
struct E9_ISLEM_DETAYLARI;


namespace Ui {
    class CARI_TAHSILAT_FISI;
}

class CARI_TAHSILAT_FISI : public FIS_KERNEL

{
    Q_OBJECT;

public:

     CARI_TAHSILAT_FISI                                              ( int fis_id, QWidget * parent = 0 );

    ~CARI_TAHSILAT_FISI                                              ();

private:

    Ui::CARI_TAHSILAT_FISI * m_ui;

    CARI_FIS_STRUCT         *   M_FIS;
    CARI_FIS_SATIRI_STRUCT  *   M_FIS_SATIRI;

    E9_ENT_DETAYLARI_STRUCT * M_FIS_DETAYLARI;
    E9_ISLEM_DETAYLARI * M_ISLEM_DETAYLARI;


    int                     m_ent_filtresi_turu;
    int                     m_start_record_id;

    bool                    m_log_kaydi_eklenecek_mi;
    bool                    m_kayit_eklendi;
    bool                    m_kayit_silindi;
    QString                 m_log_detaylari;

    int                     m_makbuz_para_birimi_id;
    int                     m_cari_para_birimi_id;
    int                     m_temel_para_birimi_id;


    bool                    m_satirdaki_islem_turu_degisti;
    int                     m_cari_hesap_id;

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

    void                    ORTALAMA_GUN_YAZ                         (int record_id);
    
    void                    SET_LINE_DEFAULTS                        ( int row_number );
    int                     CHECK_LINE_VAR                           ( int row_number, QObject * object );
    int                     CHECK_LINE_EMPTY                         ( int row_number );
    int                     CHECK_ADD_LINE                           ( int fis_id, int row_number );
    void                    ADD_LINE                                 ( int fis_id, int row_number );
    int                     CHECK_UPDATE_LINE                        ( int fis_id, int row_number );
    void                    UPDATE_LINE                              ( int fis_id, int row_number );
    int                     CHECK_DELETE_LINE                        ( int fis_id, int row_number );
    void                    DELETE_LINE                              ( int fis_id, int row_number );

    void                    SAVER_BUTTON_CLICKED                     (QAbstractButton *button, int record_id);
    int                     CHANGER_BUTTON_CLICKED                   (QAbstractButton *button);

    int                     LEAVE_RECORD                             ();

    bool                    FIS_GUNCELLENEBILIR_MI_SILINEBILIR_MI    ( int fis_id );

    void                    SEARCH_EDIT_CLICKED                      ( QWidget * widget, QLineEdit * lineEdit );

    void                    ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ    ( int row_number );

    int                     LINE_CHANGER_BUTTON_CLICKED              ( QAbstractButton *button, int row_number);

    bool                    SATIR_KAYDI_YAPILABILIR_MI (int row_number , int islem_turu);

    void                    EKRANI_GUNCELLE();

public:
    void                SHOW_REKLAM();

};

#endif // CARI_TAHSILAT_FISI_H
