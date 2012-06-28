#ifndef BNK_HAREKET_FISI_CLASS_H
#define BNK_HAREKET_FISI_CLASS_H

#include <fis_kernel.h>

struct BNK_HAREKET_FIS_STRUCT;
struct BNK_HAREKET_FIS_SATIRI_STRUCT;
struct E9_ENT_DETAYLARI_STRUCT;

namespace Ui {
    class BNK_HAREKET_FISI;
}

class BNK_HAREKET_FISI : public FIS_KERNEL

{
    Q_OBJECT;

public:

     BNK_HAREKET_FISI                                                  ( int, int , int fis_kaynagi,QWidget * parent = 0 );
    ~BNK_HAREKET_FISI                                                  (){}


private:
    BNK_HAREKET_FIS_STRUCT        * M_FIS;
    BNK_HAREKET_FIS_SATIRI_STRUCT * M_FIS_SATIRI;

    E9_ENT_DETAYLARI_STRUCT * M_ENT_DETAYLARI;

    int     etkilenecek_hesap_turu;
    int     karsi_hesap_turu;
    int     m_bnk_hesap_no_id;
    int     m_banka_hesabi_doviz_id;
    int     m_ent_filtresi_turu;

    int     m_fis_turu;
private:

    Ui::BNK_HAREKET_FISI *          m_ui;

    int                             m_start_record_id;
    int                             m_entegrasyon_hesaplarini_girebilir_mi;
    int                             m_modul_id;
    bool                            m_banka_islem_fisi_tarafindan_olusturuldu;

    void                            SETUP_FORM                               ();
    void                            CLEAR_FORM_MEMBERS                       ();
    void                            NEW_FIS_RECORD                           ();
    int                             GET_FIS_RECORD                           ( int p_fis_id );

    int                             CHECK_FIS_FORM_VAR                       ( QObject * p_object );
    int                             CHECK_FIS_FORM_EMPTY                     ();

    int                             CHECK_FIS_RECORD_ADD                     ();
    int                             ADD_FIS_RECORD                           ();
    int                             CHECK_FIS_RECORD_UPDATE                  ( int p_fis_id );
    void                            UPDATE_FIS_RECORD                        ( int p_fis_id );
    int                             CHECK_FIS_RECORD_DELETE                  ( int p_fis_id );
    void                            DELETE_FIS_RECORD                        ( int p_fis_id );

    int                             SELECT_FIS_RECORD                        ();
    int                             FIND_FIS_RECORD                          ();
    int                             FIND_NEXT_FIS_RECORD                     ();
    int                             FIND_PREV_FIS_RECORD                     ();
    int                             FIND_FIRST_FIS_RECORD                    ();
    int                             FIND_LAST_FIS_RECORD                     ();

    int                             LOCK_FIS_RECORD                          ( int p_fis_id );
    void                            UNLOCK_FIS_RECORD                        ( int p_fis_id );

    void                            SET_LINE_DEFAULTS                        ( int p_row_number );
    int                             CHECK_LINE_VAR                           ( int p_row_number, QObject * object );
    int                             CHECK_LINE_EMPTY                         ( int p_row_number );
    int                             CHECK_ADD_LINE                           ( int p_fis_id, int p_row_number );
    void                            ADD_LINE                                 ( int p_fis_id, int p_row_number );
    int                             CHECK_UPDATE_LINE                        ( int p_fis_id, int p_row_number );
    void                            UPDATE_LINE                              ( int p_fis_id, int p_row_number );
    int                             CHECK_DELETE_LINE                        ( int p_fis_id, int p_row_number );
    void                            DELETE_LINE                              ( int p_fis_id, int p_row_number );

    void                            LINE_SEARCH_EDIT_CLICKED                 ( int p_row_number, QWidget * p_widget , QLineEdit * p_lineEdit );
    void                            SAVER_BUTTON_CLICKED                     ( QAbstractButton * p_button, int p_record_id );
    int                             CHANGER_BUTTON_CLICKED                   ( QAbstractButton * p_button );
};

#endif // BNK_HAREKET_FISI_H
