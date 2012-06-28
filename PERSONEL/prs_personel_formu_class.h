#ifndef PRS_PERSONEL_FORMU_CLASS_H
#define PRS_PERSONEL_FORMU_CLASS_H

#include "ui_prs_personel_formu.h"
#include "form_kernel.h"

namespace Ui {
    class PRS_PERSONEL_FORMU;
}

class PRS_PERSONEL_FORMU : public FORM_KERNEL
{
    Q_OBJECT

public :

    PRS_PERSONEL_FORMU       ( int personel_id, QWidget *parent = 0 );
    ~PRS_PERSONEL_FORMU      () {}

private :

    Ui::PRS_PERSONEL_FORMU *    m_ui;

    int                 m_personel_id;


    QList<int>          prs_grup_idleri;
    int                 m_sube_id ;

    QString             old_personel_sicil_kodu;
    QString             m_resim;

    QStringList         m_ulke_tel_kodlari;
    QStringList         m_il_tel_kodlari;
    QString             m_fotograf_path;
    int                 m_doviz_id;

    void                SETUP_FORM                                          ();
    void                CLEAR_FORM_MEMBERS                                  ();
    void                NEW_RECORD                                          ();
    int                 GET_RECORD                                          ( int record_id );
    int                 FIND_RECORD                                         ();
    int                 SELECT_RECORD                                       ();

    int                 CHECK_VAR                                           ( QObject * object );
    int                 CHECK_EMPTY                                         ();

    int                 CHECK_ADD                                           ();
    int                 ADD_RECORD                                          ();
    int                 CHECK_UPDATE                                        ( int record_id );
    void                UPDATE_RECORD                                       ( int record_id );
    int                 CHECK_DELETE                                        ( int record_id );
    void                DELETE_RECORD                                       ( int record_id );

    int                 FIND_NEXT_RECORD                                    ();
    int                 FIND_PREV_RECORD                                    ();
    int                 FIND_FIRST_RECORD                                   ();
    int                 FIND_LAST_RECORD                                    ();

    int                 LOCK_RECORD                                         ( int record_id );
    void                UNLOCK_RECORD                                       ( int record_id );
    void                SAVER_BUTTON_CLICKED                                ( QAbstractButton * button,int record_id) ;

    int                 SICIL_NO_UNIQUE_MI                                  ( int personel_id, QString sicil_no );
    void                FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA   ( int yardim_var_mi_durumu, QCheckBox * check_box );
    void                EVLI_DEGILSE_ES_BILGISINI_DISABLE_ET                ();
    //void                RESIM_SEC_VE_YUKLE                                  ();
    void                SET_WIDGETS_MAXIMUM_SIZE                            ();
    void                ADRES_BILGILERINI_GOSTER();
    int                 RESIM_SEC                                           ();
    int                 CHANGER_BUTTON_CLICKED                              ( QAbstractButton * p_button );
    void                SEARCH_EDIT_CLICKED                                 ( QWidget * p_widget, QLineEdit *);

    // ** kullanilmasinin sebebi QSearchEdit bir pointer
    //    oldugu icin pointer tutan bir pointer olmasi gerekir.
    QSearchEdit  **     M_SEARCH_EDIT_ENT_ARRAY;
    QLineEdit    **     M_LINE_EDIT_ENT_ARRAY;
    int          *      M_ENT_HESAPLARI_ID_ARRAY;

    void                WIDGETLARI_DUZENLE();

public:
    void                SHOW_REKLAM();

};


#endif // PRS_PERSONEL_FORMU_H
