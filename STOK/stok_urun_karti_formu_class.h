#ifndef STOK_URUN_KARTI_FORMU_H
#define STOK_URUN_KARTI_FORMU_H

#include <form_kernel.h>

class QLabel;

namespace Ui {
    class STOK_URUN_KARTI_FORMU;
}

class STOK_URUN_KARTI_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
                    STOK_URUN_KARTI_FORMU                      ( int p_urun_id, QWidget * parent = 0 );
                    ~STOK_URUN_KARTI_FORMU                     (){}

private:
                    Ui::STOK_URUN_KARTI_FORMU           *       m_ui;

    QCommaEdit  **  M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI;
    QLabel      **  M_LABEL_DIGER_BIRIM_ADLARI;

    QString         m_temel_para_birimi_kodu;

    int             m_stok_takip_sekli;
    int             m_muh_hesap_id;
    int             m_urun_id;

    QList<int>      m_grup_idleri;

    QString         m_fotograf_path;

    void            SETUP_FORM                           ();

    void            CLEAR_FORM_MEMBERS                   ();
    void            NEW_RECORD                           ();
    int             GET_RECORD                           ( int p_urun_id );
    int             SELECT_RECORD                        ();
    int             FIND_RECORD                          ();

    int             CHECK_VAR                            ( QObject * p_object );
    int             CHECK_EMPTY                          ();

    int             CHECK_ADD                            ();
    int             ADD_RECORD                           ();
    int             CHECK_UPDATE                         ( int p_urun_id );
    void            UPDATE_RECORD                        ( int p_urun_id );
    int             CHECK_DELETE                         ( int p_urun_id );
    void            DELETE_RECORD                        ( int p_urun_id );

    int             FIND_NEXT_RECORD                     ();
    int             FIND_PREV_RECORD                     ();
    int             FIND_FIRST_RECORD                    ();
    int             FIND_LAST_RECORD                     ();

    int             LOCK_RECORD                          ( int p_urun_id );
    void            UNLOCK_RECORD                        ( int p_urun_id );

    void            SAVER_BUTTON_CLICKED                 ( QAbstractButton * button, int p_urun_id );

    void            SEARCH_EDIT_CLICKED                  ( QWidget * p_widget, QLineEdit * p_lineEdit );

    void            SET_WIDGET_ARRAYS                    ();

    void            URUN_BIRIMLERINI_GORUNTULE           ( int p_urun_id );

    int             CHANGER_BUTTON_CLICKED               ( QAbstractButton *button );

    void            FILL_URUN_TEMEL_BIRIMLERI            ();

    int             FOTOGRAF_SEC                         ();

    void            FOTOGRAF_CLEAR                       ();


public:
    void                SHOW_REKLAM();

};

#endif // STOK_URUN_KARTI_FORMU_H
