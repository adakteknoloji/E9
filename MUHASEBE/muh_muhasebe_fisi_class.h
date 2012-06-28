#ifndef MUH_MUHASEBE_FISI_H
#define MUH_MUHASEBE_FISI_H

#include <fis_kernel.h>

struct      MUH_FIS_STRUCT;
struct      MUH_FIS_SATIRI_STRUCT;


namespace Ui {
    class MUH_MUHASEBE_FISI;
}

class MUH_MUHASEBE_FISI : public FIS_KERNEL
{
    Q_OBJECT;

public:

     MUH_MUHASEBE_FISI                                                  ( int, int ,int, QWidget *parent = 0 );
    ~MUH_MUHASEBE_FISI                                                  (){}

     void                    SHOW_REKLAM                              ();

private:

    Ui::MUH_MUHASEBE_FISI * m_ui;

    MUH_FIS_STRUCT        * M_FIS;
    MUH_FIS_SATIRI_STRUCT * M_FIS_SATIRI;

    int                     m_fis_id;
    int                     m_fis_turu;
    int                     m_modul_id;
    bool                    m_is_entegrasyon_fisi;

    QString                 m_log_detaylari;
    bool                    m_log_kaydi_eklensin_mi;
    bool                    m_kayit_eklendi;
    bool                    m_kayit_silindi;

    int                     m_kasa_hesabi_id;

    QString                 m_muh_fis_tarihi; // db teki db tutar update sirasinda degismis ise kontrol edilir.


    double          *       M_HESABIN_BORC_BAKIYESI;
    double          *       M_HESABIN_ALACAK_BAKIYESI;

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

    void                    TALI_HESAP_MI                            ();
    bool                    KASA_ALACAK_BAKIYE_VERIYOR_MU            (int);
    void                    FIS_EKRANINI_DUZENLE                     ( int fis_turu );


    void                    SELECTED_LINE                            (int line_id,int row_save_status);
    void                    LINE_SEARCH_EDIT_CLICKED                 (int current_row,QWidget *,QLineEdit *);
    void                    SEARCH_EDIT_CLICKED                      (QWidget *, QLineEdit *);

    void                    SAVER_BUTTON_CLICKED                     ( QAbstractButton * p_button, int p_record_id );

    void                    LINE_SAVER_BUTTON_CLICKED                ( QAbstractButton * p_button, int p_row_id, int p_record_id, int p_row_number );

    int                     LINE_CHANGER_BUTTON_CLICKED              ( QAbstractButton * p_button, int p_row_number );

    int                     SATIR_TAMAMLAMA                          ( int p_row_number );

    void                    KDV_AYIRMA                               ( int p_current_row, int p_row_id, int p_fis_id );

    int                     CHECK_ALARMLAR                           (int row_number);

    int                     CHECK_EXIT                               ();
};

#endif // MUH_MUHASEBE_FISI_H
