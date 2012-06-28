#ifndef MUH_HESAP_EKLEME_FISI_H
#define MUH_HESAP_EKLEME_FISI_H

#include <fis_kernel.h>
#include "muh_hesap_plani_class.h"

namespace Ui {
    class MUH_HESAP_EKLEME_FISI;
}

class MUH_HESAP_EKLEME_FISI : public FIS_KERNEL
{
    Q_OBJECT

public:

     MUH_HESAP_EKLEME_FISI   ( int ana_grup_id, QString delimeter,bool * hesap_eklendi,MUH_HESAP_PLANI * hesap_plani );
    ~MUH_HESAP_EKLEME_FISI   (){}

private:

    Ui::MUH_HESAP_EKLEME_FISI * m_ui;

    int                     m_num_of_columns;
    int                     m_ana_hesap_id;
    int                     m_ana_hesap_seviyesi;
    bool                    m_ana_hesap_aktarilmalimi;
    QString                 m_delimeter;
    QString                 m_ana_hesap_kodu;
    int                     m_alt_hesap_sayisi;
    bool                    m_hesabin_parentini_aktar;


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
    void                    REORGANIZE_ORDER_NUMBER                  (int row_number,int new_order_number);

    int                     LINE_CHANGER_BUTTON_CLICKED              (QAbstractButton *button, int row_number);
    void                    LINE_SAVER_BUTTON_CLICKED                (QAbstractButton *button, int row_id, int record_id, int row_number);

    bool *                  m_hesap_eklendi;
    MUH_HESAP_PLANI *       m_muh_hesap_plani;
};

#endif // MUH_HESAP_EKLEME_FISI_H
