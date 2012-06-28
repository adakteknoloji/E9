#ifndef E9_SABIT_DEGERLER_FORMU_H
#define E9_SABIT_DEGERLER_FORMU_H

#include <form_kernel.h>

class   QSearchEdit;

namespace Ui {
    class E9_SABIT_DEGERLER_FORMU;
}

class E9_SABIT_DEGERLER_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:

    E9_SABIT_DEGERLER_FORMU(QWidget *parent = 0);
    ~ E9_SABIT_DEGERLER_FORMU(){}
    

private :

#define     ONTANIMLI_ENTEGRASYON_HESAPLARI_SAYISI 34
#define     AYRAC_SAYISI                11

    Ui::E9_SABIT_DEGERLER_FORMU  *       m_ui;

    int                                 m_e9_ayraci_guncellenecek;
    QString                             m_old_e9_ayraci;
    int                                 m_mali_yil_degisti;
    QString                             m_logo_path;
    int                                 m_personel_avanslari_muh_id;
    int                                 m_personel_sgk_muh_id;
    int                                 m_personel_borclari_muh_id;
    int                                 m_personel_vergi_muh_id;
    int                                 m_kdv_orani_sayisi;
    int                                 m_otv_orani_sayisi;
    int                                 kdv_otv_tanimlarina_girebilir_mi;

    int                                 m_onceki_tab_index;

    QCommaEdit **                       M_KDV_ORANLARI_COMMAEDIT_ARRAY;
    QCommaEdit **                       M_OTV_ORANLARI_COMMAEDIT_ARRAY;

    double     *                        M_KDV_ORANLARI_ARRAY;
    double     *                        M_OTV_ORANLARI_ARRAY;

    QLineEdit   **  M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY;
    QLineEdit   **  M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY;
    QLineEdit   **  M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY;
    QLineEdit   **  M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY;


    QLineEdit   **  M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY;
    QLineEdit   **  M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY;

    QLineEdit   **  M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY;

    QSearchEdit **  M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY;
    QSearchEdit **  M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY;
    QSearchEdit **  M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY;
    QSearchEdit **  M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY;


    QSearchEdit **  M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY;
    QSearchEdit **  M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY;

    QSearchEdit **  M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY;

    QSearchEdit **  M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY;
    QLineEdit   **  M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY;


    QLabel      **  M_IND_KDV_HESAPLARI_LABEL_ARRAY;
    QLabel      **  M_HES_KDV_HESAPLARI_LABEL_ARRAY;
    QLabel      **  M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY;
    QLabel      **  M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY;

    QLabel      **  M_OTV_HESAPLARI_ALIS_LABEL_ARRAY;
    QLabel      **  M_OTV_HESAPLARI_SATIS_LABEL_ARRAY;

    QLabel      **  M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY;

    int         *   M_IND_KDV_HESAPLARI_ID_ARRAY;
    int         *   M_HES_KDV_HESAPLARI_ID_ARRAY;
    int         *   M_IND_KDV_IADE_HESAPLARI_ID_ARRAY;
    int         *   M_HES_KDV_IADE_HESAPLARI_ID_ARRAY;


    int         *   M_OTV_HESAPLARI_ALIS_ID_ARRAY;
    int         *   M_OTV_HESAPLARI_SATIS_ID_ARRAY;

    int         *   M_TEVKIFAT_KDV_HESAPLARI_ID_ARRAY;




    void           SETUP_FORM                           ();
    void           CLEAR_FORM_MEMBERS                   ();
    void           NEW_RECORD                           ();
    int            GET_RECORD                           ( int record_id );
    int            SELECT_RECORD                        ();
    int            FIND_RECORD                          ();


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

    void           SET_WIDGET_DEFAULT_LENGTHS           ();


    void           ADRES_BILGILERINI_OLUSTUR            ();

    int            CHANGER_BUTTON_CLICKED               (QAbstractButton *button);

    int            LOGO_SEC                             ();

    void           WIDGET_DIZILERINI_DUZENLE            ();

    void           SEARCH_EDIT_CLICKED                  ( QWidget *widget , QLineEdit *line_edit);

    void           KDV_OTV_HESAP_SAYISINA_GORE_EKRANI_DUZENLE ();

    void           KDV_OTV_HESAPLARI_GUNCELLE();

};

#endif // E9_SABIT_DEGERLER_FORMU_H
