#ifndef CEK_SENET_FORMU_H
#define CEK_SENET_FORMU_H

#include <form_kernel.h>

struct CEK_SENET_STRUCT;

namespace Ui {
    class CEK_SENET_FORMU;
}

class CEK_SENET_FORMU : public FORM_KERNEL
{
    Q_OBJECT


public:

    CEK_SENET_FORMU(int cek_senet_turu,int cek_senet_id,QWidget *parent = 0);
    ~CEK_SENET_FORMU() {}



private:

    Ui::CEK_SENET_FORMU * m_ui;

    CEK_SENET_STRUCT * M_CEK_SENET;

    int             m_cek_senet_turu;
    int             m_hesap_no_id;

    QString         m_banka_ismi;
    QString         m_sube_kodu;
    QString         m_sube_ismi;
    int             m_doviz_id;
    int             m_entegre_olsun_mu;
    int             m_cek_senet_id;

    void           SETUP_FORM                           ();
    void           CLEAR_FORM_MEMBERS                   ();
    void           NEW_RECORD                           ();
    int            GET_RECORD                           ( int record_id );
    int            FIND_RECORD                          ();
    int            SELECT_RECORD                        ();

    int            CHECK_VAR                            ( QObject * object );
    int            CHECK_EMPTY                          ();
    int            CHECK_ADD                            ();
    int            ADD_RECORD                           ();
    int            CHECK_UPDATE                         ( int record_id );
    void           UPDATE_RECORD                        ( int record_id );
    int            CHECK_DELETE                         ( int record_id );
    void           DELETE_RECORD                        ( int record_id );

    int            FIND_NEXT_RECORD                     ();
    int            FIND_PREV_RECORD                     ();
    int            FIND_FIRST_RECORD                    ();
    int            FIND_LAST_RECORD                     ();

    int            LOCK_RECORD                          ( int record_id );
    void           UNLOCK_RECORD                        ( int record_id );



};

#endif // CEK_SENET_FORMU_H
