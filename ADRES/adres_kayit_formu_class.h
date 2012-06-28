#ifndef ADRES_KAYIT_FORMU_H
#define ADRES_KAYIT_FORMU_H

#include <form_kernel.h>

struct ADRES_HESAP_STRUCT;

namespace Ui {
    class ADRES_KAYIT_FORMU;
}

class ADRES_KAYIT_FORMU : public FORM_KERNEL
{
    Q_OBJECT

public:
     ADRES_KAYIT_FORMU                                                  ( int id, int kart_turu,QWidget * parent = 0 );
    ~ADRES_KAYIT_FORMU                                                  (){}

private:

    Ui::ADRES_KAYIT_FORMU * m_ui;

    ADRES_HESAP_STRUCT      *M_ADR_STRUCT;

    int                     m_kart_turu;
    int                     m_modul_id;
    int                     m_program_id;
    int                     m_start_record_id;

    QString                 m_fotograf_path;
    QList<int>              m_grup_idleri;

    void                    SETUP_FORM                           ();
    void                    NEW_RECORD                           ();
    void                    CLEAR_FORM_MEMBERS                   ();
    int                     GET_RECORD                           (int record_id);
    int                     SELECT_RECORD                        ();
    int                     FIND_RECORD                          ();

    int                     CHECK_VAR                            (QObject * object);
    int                     CHECK_EMPTY                          ();

    int                     CHECK_ADD                            ();
    int                     ADD_RECORD                           ();
    int                     CHECK_UPDATE                         (int record_id);
    void                    UPDATE_RECORD                        (int record_id);
    int                     CHECK_DELETE                         (int record_id);
    void                    DELETE_RECORD                        (int record_id);

    int                     FIND_NEXT_RECORD                     ();
    int                     FIND_PREV_RECORD                     ();
    int                     FIND_FIRST_RECORD                    ();
    int                     FIND_LAST_RECORD                     ();

    int                     LOCK_RECORD                          (int record_id);
    void                    UNLOCK_RECORD                        (int record_id);

    void                    SAVER_BUTTON_CLICKED                 (QAbstractButton * button,int record_id);
    int                     CHANGER_BUTTON_CLICKED               ( QAbstractButton * p_button );

    void                    ADRES_BILGILERINI_OLUSTUR            ();

    int                     FOTOGRAF_SEC                         ();

    void                    SET_WIDGETS_MAXIMUM_SIZE             ();
    void                    EKRANI_DUZENLE                       ();
    void                    KULLANICI_YETKILERINI_AYARLA         ();

public:
    void                SHOW_REKLAM();
};

#endif // ADRES_KAYIT_FORMU_H
