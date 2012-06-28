#ifndef E9_PROFIL_SECME_H
#define E9_PROFIL_SECME_H

#include <QDialog>
#include "batch_kernel.h"

class QListWidget;
class QListWidgetItem;
class QString;

namespace Ui {
    class E9_PROFIL_SECME;
}

// 0 ATANMAMISTIR
enum PROFIL_TIPLERI {
    KOMPLE_TICARI_PAKET_PROGRAM                = 1,
    FATURA_CARI_STOK_TAKIP_PROGRAMI            = 2,
    MALI_MUSAVIR_PROGRAMI                      = 3,
    ADRES_ETIKET_TOPLU_MAIL_TOPLU_SMS_PROGRAMI = 4,
    SERBEST_MESLEK_PROGRAMI                    = 5
};

enum PROFIL_CAGRILAN_YER {
    ENUM_E9_KULLANICI_YETKILERI = 0,
    ENUM_E9_BASLANGIC           = 1
};

class E9_PROFIL_SECME : public BATCH_KERNEL
{
    Q_OBJECT

public:
    explicit E9_PROFIL_SECME( PROFIL_CAGRILAN_YER p_cagrilan_yer, QWidget *parent = 0, bool * p_degisiklik_yapildimi = false );
    ~E9_PROFIL_SECME() {}

private:
    Ui::E9_PROFIL_SECME *m_ui;

    int                 m_profil_id;
    PROFIL_CAGRILAN_YER m_cagrilan_yer;
    bool               *m_degisiklik_yapildimi;

    int                 CHECK_VAR     ( QObject * p_object );
    int                 CHECK_RUN   ();
    void                RUN_BATCH     ();

    void                SETUP_FORM    ();

private slots:

    void    SLOT_CHANGE_PROFILE( QListWidgetItem * current_item );
    void    SLOT_DOUBLE_CLICK_PROFILE( QListWidgetItem * current_item );
    void    SLOT_CHANGE_CURRENT_ROW( int );

private:

    int     GET_PROFIL_TIPI ( QString p_profil_name );
    void    FILL_LIST_WIDGET( QListWidget * p_list_widget );
    QString GET_PROFIL_BILGILERI( int p_profil_id );

    void    E9_KULLANICI_STRUCT_GUNCELLE();
    void    E9_PROFIL_BILGILERINI_DB_YAZ();

};

void  OPEN_E9_PROFIL_SECME ( PROFIL_CAGRILAN_YER p_cagrilan_yer, QWidget * parent , bool * p_degisiklik_yapildimi = false );

#endif // E9_PROFIL_SECME_H
