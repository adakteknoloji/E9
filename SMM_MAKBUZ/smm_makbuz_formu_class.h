#ifndef SMM_MAKBUZ_FORMU_H
#define SMM_MAKBUZ_FORMU_H

#include <form_kernel.h>

struct       SMM_ENT_DETAYLARI;
struct       SMM_MAKBUZ_STRUCT;

namespace Ui {
    class SMM_MAKBUZ_FORMU;
}

class SMM_MAKBUZ_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
    SMM_MAKBUZ_FORMU    ( int p_form_id, int p_makbuz_turu ,QWidget * parent = 0 );
    ~ SMM_MAKBUZ_FORMU  (){}

private:

    Ui::SMM_MAKBUZ_FORMU  *m_ui;


    double                *M_KDV_ORANLARI_ARRAY;
    int                   *M_INDIRILECEK_KDV_HESABI_ID_ARRAY;
    int                   *M_TEVKIFAT_HESAPLARI_ID_ARRAY;
    SMM_ENT_DETAYLARI     *M_ENT_DETAYLARI;
    SMM_MAKBUZ_STRUCT     *M_MAKBUZ;

    int         m_makbuz_turu;
    int         m_cari_hesap_id;
    double      m_ontanimli_kdv_orani;
    int         m_sube_kasa_hesap_id;
    QString     m_sm_sicil_numarasi;

    int         m_hesap_no_id;
    int         m_kredi_karti_id;
    int         m_pos_id;
    int         m_form_id;


private:

    void SETUP_FORM             ();
    void NEW_RECORD             ();
    void CLEAR_FORM_MEMBERS	();
    int  GET_RECORD             (int record_id);
    int  SELECT_RECORD          ();
    int  FIND_RECORD            ();

    int  CHECK_VAR              (QObject * object);
    int  CHECK_EMPTY            ();

    int  CHECK_ADD              ();
    int  ADD_RECORD             ();
    int  CHECK_UPDATE           (int record_id);
    void UPDATE_RECORD          (int record_id);
    int  CHECK_DELETE           (int record_id);
    void DELETE_RECORD          (int record_id);

    int  FIND_NEXT_RECORD       ();
    int  FIND_PREV_RECORD       ();
    int  FIND_FIRST_RECORD      ();
    int  FIND_LAST_RECORD       ();

    int  LOCK_RECORD            (int record_id);
    void UNLOCK_RECORD          (int record_id);

    void SEARCH_EDIT_CLICKED    (QWidget *widget, QLineEdit *line_edit);
    int  CHANGER_BUTTON_CLICKED (QAbstractButton *button);
    void SAVER_BUTTON_CLICKED   (QAbstractButton *button, int record_id);

public:
    void                SHOW_REKLAM();

};

#endif // SMM_MAKBUZ_FORMU_H
