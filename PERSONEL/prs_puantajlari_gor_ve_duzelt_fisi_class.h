#ifndef PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI_CLASS_H
#define PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI_CLASS_H

#include "ui_prs_puantajlari_gor_ve_duzelt_fisi.h"
#include <fis_kernel.h>

namespace ui {
    class PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI;
}

class PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI : public FIS_KERNEL

{
    Q_OBJECT

public:

     PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI(int,int,int,int, QWidget * parent = 0 );
    ~PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI (){}

private:

    Ui::PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI * m_ui;

    int                     m_record_id  ;
    int                     m_sube_id    ;
    int                     m_bordro_ayi ;
    int                     m_bordro_yili;
    int                     m_kur;
    int                     m_doviz_id;

    void   		    SETUP_FORM                               ();
    void                    NEW_FIS_RECORD                           ();
    void    		    CLEAR_FORM_MEMBERS		             ();
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
    void                    SAVER_BUTTON_CLICKED                     ( QAbstractButton *button, int record_id );

    //int                     CHECK_EXIT                               ();
};

#endif // PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI_H
