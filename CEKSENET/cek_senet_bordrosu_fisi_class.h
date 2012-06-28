#ifndef CEK_SENET_BORDROSU_FISI_H
#define CEK_SENET_BORDROSU_FISI_H

#include <fis_kernel.h>

struct      CEK_BORDRO_STRUCT;
struct      CEK_BORDRO_SATIRI_STRUCT;
struct      E9_ENT_DETAYLARI_STRUCT;

namespace Ui {
    class CEK_SENET_BORDROSU_FISI;
}


class CEK_SENET_BORDROSU_FISI : public FIS_KERNEL

{
    Q_OBJECT

public:

     CEK_SENET_BORDROSU_FISI( int bordro_id, int modul_id, int bordro_islem_turu ,QWidget * parent = 0, int bordro_turu = -1 );
    ~CEK_SENET_BORDROSU_FISI() {}

     void                   SHOW_REKLAM();

private:

    CEK_BORDRO_STRUCT         *  M_BORDRO;
    CEK_BORDRO_SATIRI_STRUCT  *  M_BORDRO_SATIRI;
    E9_ENT_DETAYLARI_STRUCT   *  M_ENTEGRASYON_DETAYLARI;

    Ui::CEK_SENET_BORDROSU_FISI * m_ui;

    bool                    m_log_kaydi_eklenmeli_mi;
    bool                    m_kayit_eklendi;
    bool                    m_kayit_silindi;
    QString                 m_log_detaylari;

    int                     m_modul_id;
    int                     m_bordro_id;
    int                     m_bordro_islem_turu;
    int                     m_ent_bordrosu_islem_turu;
    int                     m_bordro_turu;
    int                     m_cek_mi_senet_mi;
    bool                    m_musteri_cek_senedi_mi;

    QStringList             m_doviz_cinsine_gore_toplam_tutar_list;
    int                     m_cari_hesap_id;
    int                     m_hesap_no_id;
    int                     m_cari_para_birimi_id;
    int                     m_bordro_para_birimi_id;
    int                     m_bnk_hesabi_para_birimi_id;
    int                     m_temel_para_birimi_id;

    QString                 m_banka_ismi;
    QString                 m_sube_kodu;
    QString                 m_sube_ismi;

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

    int                     LEAVE_RECORD                             ();



    void                    BORDRO_EKRANINI_DUZENLE                  ();

    bool                    SATIR_SILINEBILIR_MI_GUNCELLENEBILIR_MI  ( int fis_id , int cek_senet_id);




    void                    CEK_SENET_ENTEGRASYON_HESAPLARI_KONTROLU  ( int bordro_id, int row_number, int p_cek_snt_id = 0 );

    void                    LINE_SEARCH_EDIT_CLICKED                  ( int row_number,QWidget * widget,QLineEdit * line_edit);

    void                    SEARCH_EDIT_CLICKED                       ( QWidget * widget, QLineEdit * lineEdit);


    QStringList             GET_UYGUN_CEKLER_LIST                     ( int hesap_id );

    void                    SET_WINDOW_TITLE();

    void                    SAVER_BUTTON_CLICKED                      ( QAbstractButton * p_button, int p_record_id );

    int                     CHANGER_BUTTON_CLICKED                    ( QAbstractButton *button);

    int                     KENDI_CEKLERIMIZ                          ();
    int                     KENDI_SENETLERIMIZ                        ();




};

#endif // CEK_SENET_BORDROSU_FISI_H
