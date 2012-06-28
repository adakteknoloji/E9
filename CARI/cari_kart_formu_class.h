#ifndef CARI_KART_FORMU_H
#define CARI_KART_FORMU_H

#include <form_kernel.h>

namespace Ui {
    class CARI_KART_FORMU;
}

class CARI_KART_FORMU : public FORM_KERNEL

{
    Q_OBJECT;

public:

     CARI_KART_FORMU                                                  ( int,int, QWidget * parent = 0 );
    ~CARI_KART_FORMU                                                  (){}

private:

    Ui::CARI_KART_FORMU * m_ui;

    int                     m_cari_kart_turu;
    QStringList             m_ulke_tel_kodlari;
    QStringList             m_list_sehirler;
    QStringList             m_list_ulkeler;
    int                     m_muh_hesap_id;
    int                     m_fis_id;

    QString                 m_fotograf_path;

    QList<int>              m_grup_idleri;

    int                     m_program_id;
    int                     m_modul_id;

    int                     m_para_birimi_id;

    void                    SETUP_FORM                           ();
    void                    NEW_RECORD                           ();
    void                    CLEAR_FORM_MEMBERS			();
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

    void                    ADRES_BILGILERINI_GOSTER                    ();

    void                    SEARCH_EDIT_CLICKED                         ( QWidget * , QLineEdit * );

    void                    ULKEYE_GORE_SABIT_TEL_ALAN_KODLARINI_DOLDUR ( int p_ulke_id, QComboBox * p_comboBox );
    void                    ULKEYE_GORE_CEP_TEL_GSM_KODLARINI_DOLDUR    ( int p_ulke_id, QComboBox * p_comboBox );

    void                    SATIR_TELEFON_ALAN_KODLARINI_DOLDUR         ( int current_row );
    void                    SAVER_BUTTON_CLICKED                        ( QAbstractButton * button,int record_id);
    int                     CHANGER_BUTTON_CLICKED                      ( QAbstractButton * button);

    void                    SET_WIDGETS_MAXIMUM_SIZE                    ();
    void                    EKRANI_DUZENLE                              ();

    int                     FOTOGRAF_SEC                               ();

public:
    void                SHOW_REKLAM();

};


#endif // CARI_KART_FORMU_H
