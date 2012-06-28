#ifndef BNK_VIRMAN_FORMU_CLASS_H
#define BNK_VIRMAN_FORMU_CLASS_H

#include <form_kernel.h>

struct     BNK_DEFTER_FIS_STRUCT;
struct     BNK_DEFTER_FIS_SATIRI_STRUCT;

struct     BNK_VIRMAN_FIS_AND_SATIRI_STRUCT {

    int     fis_id;
    int     fis_no;
    QString aciklama;
    QString fis_tarihi;
    int     gonderen_hesap_no_id;
    int     alici_hesap_no_id;

    int     gonderen_hesap_doviz_id;
    int     alici_hesap_doviz_id;

    double  doviz_kuru;
    double  gonderen_islem_tutari;
    double  alici_islem_tutari;
    int     islem_tutari;

};

void    BNK_VIRMAN_FIS_SATIRI_EKLE ( BNK_VIRMAN_FIS_AND_SATIRI_STRUCT * P_VIRMAN_STRUCT , int p_fis_satiri_id = 0,
                                     bool p_base_fis_satiri_eklenecek = true ,ADAK_SQL * P_GELEN_DB = NULL);
void    CLEAR_BNK_VIRMAN_FIS_SATIRI_STRUCT ( BNK_VIRMAN_FIS_AND_SATIRI_STRUCT * P_VIRMAN_STRUCT  );

namespace Ui {
    class BNK_VIRMAN_FORMU;
}

class BNK_VIRMAN_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
    BNK_VIRMAN_FORMU   ( int p_form_id, QWidget * parent = 0 );
    ~ BNK_VIRMAN_FORMU (){}

private:

    QString m_gonderen_banka_adi;
    QString m_alici_banka_adi;

    int     m_gonderen_hesap_no_id;
    int     m_gon_hesabi_doviz_id;
    int     m_alici_hesap_no_id;
    int     m_alici_hesabi_doviz_id;
    int     m_fis_turu;

    int     m_form_id;

    BNK_DEFTER_FIS_STRUCT              * M_FIS;
    BNK_DEFTER_FIS_SATIRI_STRUCT       * M_FIS_SATIRI;

    int                                 m_ent_filtresi_turu;

private:

    Ui::BNK_VIRMAN_FORMU  *       m_ui;

    void           SETUP_FORM                           ();
    void           NEW_RECORD                           ();
    void           CLEAR_FORM_MEMBERS			();
    int            GET_RECORD                           (int record_id);
    int            SELECT_RECORD                        ();
    int            FIND_RECORD                          ();

    int            CHECK_VAR                            (QObject * object);
    int            CHECK_EMPTY                          ();

    int            CHECK_ADD                            ();
    int            ADD_RECORD                           ();
    int            CHECK_UPDATE                         (int record_id);
    void           UPDATE_RECORD                        (int record_id);
    int            CHECK_DELETE                         (int record_id);
    void           DELETE_RECORD                        (int record_id);

    int            FIND_NEXT_RECORD                     ();
    int            FIND_PREV_RECORD                     ();
    int            FIND_FIRST_RECORD                    ();
    int            FIND_LAST_RECORD                     ();

    int            LOCK_RECORD                          (int record_id);
    void           UNLOCK_RECORD                        (int record_id);

    void           SAVER_BUTTON_CLICKED                 (QAbstractButton *button, int record_id);

};

#endif // BNK_VIRMAN_FORMU_H
