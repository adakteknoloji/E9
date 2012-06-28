#ifndef BNK_MAKRO_FISI_CLASS_H
#define BNK_MAKRO_FISI_CLASS_H

#include <QtGui/QDialog>
#include <fis_kernel.h>

enum {
    ENUM_ADD_LINE       = 10,
    ENUM_UPDATE_LINE    = 20,
    ENUM_DELETE_LINE    = 30
};

struct E9_ENT_DETAYLARI_STRUCT;

namespace Ui {
    class BNK_MAKRO_FISI;
}

struct E9_ENT_DETAYLARI_STRUCT;

class BNK_MAKRO_FISI : public FIS_KERNEL

{
    Q_OBJECT

public:
     BNK_MAKRO_FISI( int p_fis_id ,int p_fis_kaynagi , QWidget * p_parent = 0 );
    ~BNK_MAKRO_FISI(){}

private:
    E9_ENT_DETAYLARI_STRUCT *M_ENT_DETAYLARI;

    int m_ent_filtresi_turu;
    int m_muh_fis_id;

private:
    Ui::BNK_MAKRO_FISI  *m_ui;

    int m_record_id;
    int m_num_of_columns;
    int m_temel_para_birim_id;
    int m_banka_hesabi_doviz_id;

    QString m_temel_para_birim_kodu;

    int m_fis_kaynagi;

    QStringList m_makro_turleri;
    int m_bakiyeler_arttirilacak;

    bool m_log_kaydi_eklensin_mi;
    bool m_kayit_eklendi;
    bool m_kayit_silindi;
    QString m_log_detaylari;
    int  m_bnk_hesap_no_id;
    bool m_muh_entegrasyonu;

    void SETUP_FORM                 ();
    void CLEAR_FORM_MEMBERS         ();
    void NEW_FIS_RECORD             ();
    int  GET_FIS_RECORD             ( int p_fis_id );

    int  CHECK_FIS_FORM_VAR         (QObject * p_object);
    int  CHECK_FIS_FORM_EMPTY       ();

    int  CHECK_FIS_RECORD_ADD       ();
    int  ADD_FIS_RECORD             ();
    int  CHECK_FIS_RECORD_UPDATE    ( int p_fis_id );
    void UPDATE_FIS_RECORD          ( int p_fis_id );
    int  CHECK_FIS_RECORD_DELETE    ( int p_fis_id );
    void DELETE_FIS_RECORD          ( int p_fis_id );
    int  SELECT_FIS_RECORD          ();
    int  FIND_FIS_RECORD            ();
    int  FIND_NEXT_FIS_RECORD       ();
    int  FIND_PREV_FIS_RECORD       ();
    int  FIND_FIRST_FIS_RECORD      ();
    int  FIND_LAST_FIS_RECORD       ();

    int  LOCK_FIS_RECORD            ( int p_fis_id );
    void UNLOCK_FIS_RECORD          ( int p_fis_id );

    void SEARCH_EDIT_CLICKED        ( QWidget *widget, QLineEdit *line_edit);

    void SET_LINE_DEFAULTS          ( int p_row_number );
    int  CHECK_LINE_VAR             ( int p_row_number, QObject * p_object );
    int  CHECK_LINE_EMPTY           ( int p_row_number );
    int  CHECK_ADD_LINE             ( int p_fis_id, int p_row_number );
    void ADD_LINE                   ( int p_fis_id, int p_row_number );
    int  CHECK_UPDATE_LINE          ( int p_fis_id, int p_row_number );
    void UPDATE_LINE                ( int p_fis_id, int p_row_number );
    int  CHECK_DELETE_LINE          ( int p_fis_id, int p_row_number );
    void DELETE_LINE                ( int p_fis_id, int p_row_number );

    int  LEAVE_RECORD               ();

    bool ISLEME_CARI_HESAP_GEREKLIMI(int makro_id);
    void GET_MAKRO_TURLERI          ();
    int  GET_MAKRO_ID               (QString makro_adi);

    void LINE_SEARCH_EDIT_CLICKED   ( int p_row_number,QWidget * p_widget, QLineEdit * p_line_edit);

    void FIS_EKRANINI_DUZENLE       ( int p_row_number );
    int  GET_CARI_MODULU_FIS_NO     ( int p_cari_hesap_id, int p_fis_id );
    void SAVER_BUTTON_CLICKED       ( QAbstractButton * p_button, int p_record_id );
    int  CHANGER_BUTTON_CLICKED     ( QAbstractButton * p_button );

    void ADD_ENT_LINE               ( int p_fis_id, int p_row_number, int base_method );
    void DELETE_ENT_LINE            ( int p_fis_id, int p_row_number, int base_method );

    void SATIR_DUZENLE              ( bool p_kur, bool p_parite );
};


struct MAKRO_SATIR_STRUCT {

    int     fis_id;
    double  islem_tutari;
    double  otomatik_odeme_tutari;
    int     order_number;
    int     makro_id;

    QString search_edit_hesap;
    double  doviz_kuru;
    double  parite;
    int     base_record_id;
    QString fis_tarihi;
    QString satir_aciklama;
    int     banka_doviz_id;
    int     hesap_no_id;

    int     hesap_turu;
    QString fis_aciklama;

    double  satir_tutari;
};

struct MAKRO_FIS_STRUCT {
    int fis_no;
    int program_id;
    int modul_id;
    int base_fis_id;
    int muh_fis_id;
    int def_fis_id;
    QString fis_tarihi;
    QString aciklama;
    int hesap_no_id;
    int para_birim_id;

};


int     MAKRO_ADD_LINE            ( MAKRO_SATIR_STRUCT  * P_FIS, int fis_satiri_id = 0, bool base_fis_satiri_eklenecek = true , ADAK_SQL * P_GELEN_DB = NULL);
void    CLEAR_MAKRO_FIS_SATIRI    ( MAKRO_SATIR_STRUCT  * );
int     MAKRO_ADD_FIS             ( MAKRO_FIS_STRUCT    * P_FIS , int p_fis_id = 0,  bool base_fis_eklenecek = true ,ADAK_SQL * P_GELEN_DB = NULL);
void    CLEAR_MAKRO_FIS           ( MAKRO_FIS_STRUCT    * );
double  ROUND_DEGER               ( int round_sekli, double p_deger );

#endif // BNK_MAKRO_FISI_H


















