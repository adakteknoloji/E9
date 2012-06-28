#ifndef PRK_PERAKENDE_FISI_CLASS_H
#define PRK_PERAKENDE_FISI_CLASS_H

#include <fis_kernel.h>
#include "ui_prk_perakende_fisi.h"


struct      E9_ENT_DETAYLARI_STRUCT;
struct      E9_ISLEM_DETAYLARI;
struct      PRK_FIS_STRUCT;
struct      PRK_FIS_SATIRI_STRUCT;

namespace ui {
    class PRK_PERAKENDE_FISI;
}

class PRK_PERAKENDE_FISI : public FIS_KERNEL

{
    Q_OBJECT;

public:

     PRK_PERAKENDE_FISI                                                  ( int p_sube_id , int p_gider_fisi_mi, QWidget * parent = 0 );
    ~PRK_PERAKENDE_FISI                                                  (){}

private:

    Ui::PRK_PERAKENDE_FISI   *        m_ui;

    int                         m_gider_fisi_mi;


    E9_ENT_DETAYLARI_STRUCT *   M_ENT_DETAYLARI;
    E9_ISLEM_DETAYLARI      *   M_ISLEM_DETAYLARI;

    PRK_FIS_STRUCT          *   M_FIS;
    PRK_FIS_SATIRI_STRUCT   *   M_FIS_SATIRI;


    double                  * M_KDV_ORANLARI_ARRAY;
    int                     * M_INDIRILECEK_KDV_HESABI_ID_ARRAY;
    int                     * M_HESAPLANAN_KDV_HESABI_ID_ARRAY;


    int                     m_sube_id;
    double                  m_ontanimli_kdv_orani;
    int                     m_sube_islem_yetkisi;

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

    void                    SAVER_BUTTON_CLICKED                     (QAbstractButton *button, int record_id);

    int                     CHANGER_BUTTON_CLICKED                   (QAbstractButton *button);

    int                     LINE_CHANGER_BUTTON_CLICKED              (QAbstractButton *button, int row_number);

    void                    SET_KASA_MEVCUDU                         ();
};

#endif // PRK_PERAKENDE_FISI_H
