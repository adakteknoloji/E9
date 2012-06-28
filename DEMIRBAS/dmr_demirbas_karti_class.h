#ifndef DMR_DEMIRBAS_KARTI_H
#define DMR_DEMIRBAS_KARTI_H

#include <QtGui/QDialog>
#include <form_kernel.h>

namespace Ui {
    class DMR_DEMIRBAS_KARTI;
}

class DMR_DEMIRBAS_KARTI : public FORM_KERNEL
{
    Q_OBJECT
public:
    DMR_DEMIRBAS_KARTI(QWidget *parent = 0);
    ~ DMR_DEMIRBAS_KARTI(){}

private:
    Ui::DMR_DEMIRBAS_KARTI  *       m_ui;
    double                          m_max_amortisman_orani;
    int                             m_muh_hesap_id;
    int                             m_bir_amor_hesap_id;
    int                             m_amor_gider_hesap_id;
    int                             m_cari_hesap_id;
    int                             m_sube_id;
    int                             m_dmr_satis_kar_hesap_id;
    int                             m_dmr_satis_zarar_hesap_id;

    QList<int>                      m_rpr_isl_grubu_idler;


    int                             m_grup_id;
    int                             m_fatura_id;

    void           SETUP_FORM                           ();
    void           CLEAR_FORM_MEMBERS                   ();
    void           NEW_RECORD                           ();
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

    void           SEARCH_EDIT_CLICKED                  (QWidget *,QLineEdit * line_edit);

    int            CHANGER_BUTTON_CLICKED              (QAbstractButton *button);





};

#endif // DMR_DEMIRBAS_KARTI_H
