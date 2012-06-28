#ifndef FORM_KERNEL_H
#define FORM_KERNEL_H

#include <QDialog>
#include <adak_sql.h>
#include "kernel_utils.h"
#include "base_event_kernel.h"

#define MULTI_RECORD_MODE_BUTTON_COUNT  24


enum RECORD_SAVE_STATUS {
    CANT_LOCKED = 0,
    SAVED       = 1,
    NEED_UPDATE = 2,
    NEED_ADD    = 3,
    EMPTY       = 4
};

enum LOCK_RECORD_STATUS {
    LOCKED               = 1,
    LOCK_NOT_REQUESTED   = 2,
    CANT_LOCK            = 3
};

enum RECORD_MODE {
    SINGLE_RECORD_MODE = 0,
    MULTI_RECORD_MODE  = 1
};

enum KERNEL_SAVE_RETURN_VALUES {
    RECORD_SAVED         = 10,
    RECORD_CANT_SAVED    = 20,
    RECORD_ALREADY_SAVED = 30,
    RECORD_EMPTY         = 40
};

//Single record mode sadece tek kayitli bir form varsa kullanilabilir.Bu durumda sadece yenile ve update
//buttonlari eklenir.Ve Kernel tarafindan record id daima 1 olarak gonderilir.Eger mevcut bir record yoksa
//yaratilmalidir.

class FORM_KERNEL : public BASE_EVENT_KERNEL
{
    Q_OBJECT

public:
    FORM_KERNEL (QWidget * parent = 0 );
    virtual ~FORM_KERNEL();

protected :
    int                    m_degisen_widget_counter;
    bool                   m_is_readonly_mode_on;
    bool                   is_fis_kernel;
    bool                   lock_flag;

    // REGISTER_BUTTONS_WIDGET metodunun cagrildigini kontrol eder
    bool                   m_register_button_widget_called;

    void                   REGISTER_BUTTONS_WIDGET              (QWidget         * kernel_buttons_widget,bool remove_buttons_text = false, bool readonly_mode = false);
    void                   REGISTER_SAVER_BUTTON                (QAbstractButton * button);
    void                   REGISTER_DATABASE                    (ADAK_SQL * database);

    void                   START_FORM_KERNEL                    (QObject * parent, ADAK_SQL * database);

    //Bu fonksiyon  sağlıklı çalışabilmesi için REGISTER_BUTTONS_WIDGETS'dan önce çağrılmalıdır.
    //Ayrica gonderilen id databaseden cagrilacak olan kaydin id si olmalidir.
    void                   SET_SINGLE_RECORD_ID                 (int single_record_id );


    void                   SET_RECORD_STATUS                    (int record_status);
    void                   SET_MDI_WINDOW                       ();
    int                    UNSAVED_RECORD_WARNING               ();
    void                   UPDATE_BUTTON_STATUS                 ();
    int                    SAVE_RECORD                          ();
    int                    SAVE_FIS_RECORD                      ();

    //gonderilen idye gore kaydi yeniler.Eger id gonderilmezse o anki formu yeniler
    void                   REFRESH_FORM_RECORD                  (int record_id = -1);

    //Bu fonksiyonla programci FORM un save durumunu alabilir.
    int                    GET_RECORD_SAVE_STATUS               ();

    int                    GET_LOCK_STATUS                      ();

    int                    GET_RECORD_ID                        ();
    void                   SET_CURRENT_RECORD_ID                (int record_id);

    void                   AUTOSTART_FORM_KERNEL                (int record_id);

    virtual void           SETUP_FORM                           ()                                 = 0;

    //NEW_RECORD yeni bir kayit acmak icin kullanilacak fonksiyondur.
    virtual void           NEW_RECORD                           ()                                 = 0;
    virtual void           CLEAR_FORM_MEMBERS                   ()                                 = 0;


    //Bu fonksiyon cagirildiginda programci record_id ye gore widget lari dolduracak.
    virtual int            GET_RECORD                           (int record_id)                    = 0;

    //Bu fonksiyonla kullanici tarafindan elle girilen record id degeri gecerli olup olmadigi kontrol edilip
    //return degeri olarak kernela gonderilmeli.Kernel otomatik olarak widgetlari bu id ye gore doldurur.
    virtual int            FIND_RECORD                          ()                                 = 0;

    virtual int            CHECK_EMPTY                          ()                                 = 0;

    //Bu fonksiyon record eklemek icin kullanilacak.Donus degeri eklenenen kaydin id si olmali.
    virtual int            CHECK_ADD                            ()                                 = 0;

    virtual int            ADD_RECORD                           ()                                 = 0;

    virtual int            CHECK_UPDATE                         (int record_id)                    = 0;
    virtual void           UPDATE_RECORD                        (int record_id)                    = 0;
    virtual int            CHECK_DELETE                         (int record_id)                    = 0;
    virtual void           DELETE_RECORD                        (int record_id)                    = 0;
    //Bu fonksiyonlar yapilacak tum islemler icin kernel o anda aktif olan kaydin id sini gonderecektir.
    //Programci bu id ye gore tum islemleri yapabilir.

    //Bu fonksiyon secilen id yi dondurmeli.Kernel burdan gelen id numarasina gore GET_RECORD u cagirir.
    virtual int            SELECT_RECORD                        ()                                 = 0;

    virtual int            FIND_NEXT_RECORD                     ()                                 = 0;
    virtual int            FIND_PREV_RECORD                     ()                                 = 0;
    virtual int            FIND_FIRST_RECORD                    ()                                 = 0;
    virtual int            FIND_LAST_RECORD                     ()                                 = 0;
    //Bu fonksiyonla bulunan recordlarin idleri dondurulmeli kernel bu idlerle widgetlari otomatik olarak
    //doldurur.

    //LOCK_RECORD ve UNLOCK_RECORD formda yada fiste kullanilan record id ye gore yapilmali.
    virtual int            LOCK_RECORD                          (int record_id)                    = 0;
    virtual void           UNLOCK_RECORD                        (int record_id)                    = 0;

    //Ekrandaki kaydin temizlenmesi yeni kayit vs gibi durumlarda duser.
    virtual int            LEAVE_RECORD                         ();

    //Kernelin kontrol etmesi icin programci tarafindan SET_BUTTON_FOR_AUTO_SAVE ile eklenen buttonlarin click eventi ile
    //cagiracagi fonksiyondur.Burda QAbstractButton o anda uzerine tiklanan butonu tasir.
    virtual void           SAVER_BUTTON_CLICKED                 (QAbstractButton * button, int record_id);
    virtual void           INIT_KERNEL                          ();

    virtual void           INCREASE_DEGISEN_WIDGET_COUNT        (QWidget *);
    virtual void           DECREASE_DEGISEN_WIDGET_COUNT        (QWidget *);

    virtual bool           eventFilter                          (QObject *, QEvent * );

protected slots:
    //WIDGET_CHANGED fonksiyonu kernelda, herhangi bir widget ta degisiklik oldugunu
    //form status u degistirip butonlarin ona gore aktif olmasini saglar.
    //SLOT oldugundan dolayi SLOT a baglanilarak kullanilmali ama gerekirse fonksiyon olarak
    //da cagrilabilir.
    virtual void           WIDGET_CHANGED                       ();
    void                   RESET_WIDGET_CHANGED                 ();

private :
    int                    p_current_record_id;
    int                    p_previous_record_id;
    int                    p_record_status;
    RECORD_MODE            p_record_mode;
    QList<QAbstractButton *>   p_template_buttons;//programcinin eklemis oldugu butonlar ama kernel enable veya disable ediyor.
    bool                   p_widget_changed;
    QPushButton *          FORM_BUTTONS[MULTI_RECORD_MODE_BUTTON_COUNT];
    bool                   close_event_started;
    bool                   p_table_is_filling;
    int                    p_lock_status;
    bool                   p_event_started;
    bool                   TRY_LOCK_RECORD                      ();
    void                   TRY_UNLOCK_RECORD                    (int record_id = -1);
    bool                   CHECK_WIDGET_SAVE_STATUS             ();
    bool                   FILL_FORM                            (int record_id);
    void                   READONLY_BUTTONS_CLICKED             (int button_enum);
    QPushButton *          CREATE_KERNEL_BUTTON                 (QString button_text,QString icon_path,QWidget * parent,bool remove_button_text);
    void                   YENI_KAYIT                           ();

    void                   BUTTON_BUL_CLICKED                   ();
    void                   BUTTON_YENI_KAYIT_CLICKED            ();
    void                   BUTTON_KAYDI_SIL_CLICKED             ();
    void                   BUTTON_KAYDET_CLICKED                ();
    void                   BUTTON_SON_KAYIT_CLICKED             ();
    void                   BUTTON_ILK_KAYIT_CLICKED             ();
    void                   BUTTON_ONCEKI_KAYIT_CLICKED          ();
    void                   BUTTON_SONRAKI_KAYIT_CLICKED         ();
    void                   BUTTON_YENILE_CLICKED                ();
    void                   BUTTON_GETIR_CLICKED                 ();
    void                   BUTTON_KAYDET_TEMPLATE_BUTTONS       (QObject *);

    bool                   BLOCK_BUTTON_EVENTS                  (QObject *);
    void                   UNBLOCK_BUTTON_EVENTS                (QObject *);

    void                   PROCESS_CHECK_VAR_RETURN_VALUE       (int return_value);

    void                   SET_RECORD_MODE                      (RECORD_MODE record_mode);
    void                   SET_READONLY_MODE                    ();
    void                   PROCESS_ADAK_EVENTS                 ();

private slots:
    void                   SLOT_BUTTON_BUL_CLICKED              ();
    void                   SLOT_BUTTON_YENI_KAYIT_CLICKED       ();
    void                   SLOT_BUTTON_KAYDI_SIL_CLICKED        ();
    void                   SLOT_BUTTON_KAYDET_CLICKED           ();
    void                   SLOT_BUTTON_SON_KAYIT_CLICKED        ();
    void                   SLOT_BUTTON_ILK_KAYIT_CLICKED        ();
    void                   SLOT_BUTTON_ONCEKI_KAYIT_CLICKED     ();
    void                   SLOT_BUTTON_SONRAKI_KAYIT_CLICKED    ();
    void                   SLOT_BUTTON_YENILE_CLICKED           ();
    void                   SLOT_BUTTON_GETIR_CLICKED            ();
    void                   SLOT_BUTTON_KAYDET_TEMPLATE_BUTTONS  ();
};

#endif // FORM_KERNEL_H
