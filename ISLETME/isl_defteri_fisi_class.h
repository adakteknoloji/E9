#ifndef ISL_DEFTERI_FISI_CLASS_H
#define ISL_DEFTERI_FISI_CLASS_H

#include <fis_kernel.h>

struct ISL_FIS_STRUCT;
struct ISL_FIS_SATIRI_STRUCT;
struct ISL_ENT_DETAYLARI_STRUCT;
struct E9_ISLEM_DETAYLARI;

namespace Ui {
    class ISL_DEFTERI_FISI;
}

class ISL_DEFTERI_FISI : public FIS_KERNEL

{
    Q_OBJECT

public:

     ISL_DEFTERI_FISI ( int fis_id,int fis_turu, QWidget * parent = 0 );
    ~ISL_DEFTERI_FISI (){}

private:

    Ui::ISL_DEFTERI_FISI * m_ui;

    ISL_FIS_STRUCT *                M_ISL_FISI;
    ISL_FIS_SATIRI_STRUCT    *      M_ISL_FIS_SATIRI;

    double                   *      M_KDV_ORANLARI_ARRAY;
    int                      *      M_INDIRILECEK_KDV_HESABI_ID_ARRAY;

    int                             m_fis_turu;
    int                             m_fis_id;

    bool                            m_log_kaydi_eklensin_mi;
    bool                            m_kayit_eklendi;
    bool                            m_kayit_silindi;
    QString                         m_log_detaylari;
    double                          m_ontanimli_kdv_orani;

    void                            SETUP_FORM                               ();
    void                            CLEAR_FORM_MEMBERS                       ();
    void                            NEW_FIS_RECORD                           ();
    int                             GET_FIS_RECORD                           (int fis_id);

    int                             CHECK_FIS_FORM_VAR                       (QObject * object);
    int                             CHECK_FIS_FORM_EMPTY                     ();

    int                             CHECK_FIS_RECORD_ADD                     ();
    int                             ADD_FIS_RECORD                           ();
    int                             CHECK_FIS_RECORD_UPDATE                  (int fis_id);
    void                            UPDATE_FIS_RECORD                        (int fis_id);
    int                             CHECK_FIS_RECORD_DELETE                  (int fis_id);
    void                            DELETE_FIS_RECORD                        (int fis_id);

    int                             SELECT_FIS_RECORD                        ();
    int                             FIND_FIS_RECORD                          ();
    int                             FIND_NEXT_FIS_RECORD                     ();
    int                             FIND_PREV_FIS_RECORD                     ();
    int                             FIND_FIRST_FIS_RECORD                    ();
    int                             FIND_LAST_FIS_RECORD                     ();

    int                             LOCK_FIS_RECORD                          (int fis_id);
    void                            UNLOCK_FIS_RECORD                        (int fis_id);

    void                            SET_LINE_DEFAULTS                        ( int row_number );
    int                             CHECK_LINE_VAR                           ( int row_number, QObject * object );
    int                             CHECK_LINE_EMPTY                         ( int row_number );
    int                             CHECK_ADD_LINE                           ( int fis_id, int row_number );
    void                            ADD_LINE                                 ( int fis_id, int row_number );
    int                             CHECK_UPDATE_LINE                        ( int fis_id, int row_number );
    void                            UPDATE_LINE                              ( int fis_id, int row_number );
    int                             CHECK_DELETE_LINE                        ( int fis_id, int row_number );
    void                            DELETE_LINE                              ( int fis_id, int row_number );
    int                             LEAVE_RECORD                             ();
    void                            FIS_EKRANINI_DUZENLE                     ();


    void                            SAVER_BUTTON_CLICKED                     ( QAbstractButton * p_button, int p_record_id );

    int                             CHANGER_BUTTON_CLICKED                   ( QAbstractButton *button);

    int                             LINE_CHANGER_BUTTON_CLICKED              ( QAbstractButton *button , int row_number);

};

#endif // ISL_DEFTERI_FISI_H
