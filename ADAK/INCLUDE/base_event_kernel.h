#ifndef BASE_EVENT_KERNEL_H
#define BASE_EVENT_KERNEL_H

#include <QDialog>
#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QToolButton>
#include <QDateEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QTimeEdit>
#include <QTableWidget>
#include <QRadioButton>
#include <QTextEdit>
#include "QSearchEdit.h"
#include "QCommaEdit.h"
#include "QAdakDate.h"
#include "base_kernel.h"

enum CHANGER_RETURN_VALUES {
    ADAK_RECORD_CHANGED   = 10,
    ADAK_RECORD_UNCHANGED = 20
};

enum KERNEL_TYPE {
    ADAK_BATCH_KERNEL = 10,
    ADAK_FORM_KERNEL  = 20,
    ADAK_FIS_KERNEL   = 30,
    ADAK_ARAMA_KERNEL = 40,
};

class BASE_EVENT_KERNEL : public BASE_KERNEL
{
    Q_OBJECT
public:
    BASE_EVENT_KERNEL(QWidget * parent);
    ~BASE_EVENT_KERNEL();


protected:
    bool                   m_widget_changed_active;
    bool                   m_search_clicked_started;

    void                   CLOSE_CHECK_VAR_CONTROL              ();
    void                   OPEN_CHECK_VAR_CONTROL               ();

    //Tum eventlari acar yada kapar. Bu eventlar check var ve widget changed eventlari
    void                   OPEN_ALL_EVENTS                      ();
    void                   CLOSE_ALL_EVENTS                     ();

    //Bu fonksiyonla degisiklikler widget changes aktif eder
    void                   ENABLE_CATCH_WIDGET_CHANGES          ();
    //Bu fonksiyonla degisiklikler widget changes aktif etmez
    void                   DISABLE_CATCH_WIDGET_CHANGES         ();

    bool                   IS_WIDGET_CHANGED                    ();
    bool                   IS_SLOT_WIDGETS                      (QObject * widget);

    void                   UNDO_CHANGES                         ();


    //Bu fonksiyonla ekrandaki tum widgetlar temizlenir.DISABLE_CLEAR_ON_WIDGET la clear istenmeyen
    //widgetlar setlenebilir. Eger parent gonderilmezse tum form aksi takdirde parent child widgetlar
    //clear yapilir.
    void                   CLEAR_ALL_WIDGETS                    (QObject * parent = NULL);
    //Bu fonksiyonla CLEAR_ALL_WIDGETS da clear lanmicak widgetlar setlenir.
    void                   DISABLE_CLEAR_ON_WIDGET              (QObject * widget);

    void                   SET_EVENTS_AND_SIGNALS               (QObject * parent,int kernel_type);
    void                   RESET_CHECK_VAR_VARIABLES            ();
    virtual int            CHECK_VAR                            (QObject * object) = 0;
    virtual void           PROCESS_CHECK_VAR_RETURN_VALUE       (int return_value);
    void                   RESET_KERNEL_VARIABLES               ();
    virtual bool           TRY_LOCK_RECORD                      ();

    virtual void           INCREASE_DEGISEN_WIDGET_COUNT        (QWidget *);
    virtual void           DECREASE_DEGISEN_WIDGET_COUNT        (QWidget *);

    void                   SET_CURRENT_WIDGET                   (QWidget * widget);
    void                   SET_LAST_FOCUSED_WIDGET              (QWidget *);
    void                   FOCUS_LAST_FOCUSED_WIDGET            ();

    //Changer button setlenir.Her turlu(Toolbutton,PushButton,Search edit vs..)
    //button iadak widget setlenebilir.Kernel otomatik olarak
    //button objesini bulup gerekli set leri yapacaktir.
    void                   REGISTER_CHANGER_BUTTON              (QWidget * button_widget);

    //Adak  eventlarini queue ye ekleyerek ve kernel tarafindan uygun zamanda cagrilmasi saglanabilir.
    void                   ADD_ADAK_EVENT                      (int event_type);

    //Focuslanmak istenen widget setlenir.Kernel da eventlar aktif oldugu anda o widgeta focuslanir.
    void                   SET_FOCUS                            (QWidget * focus_widget);

    //Changer buttona tiklanica bu fonksiyona duser.Donus degerine gore ADAK_RECORD_CHANGED ve ADAK_RECORD_UNCHANGED
    //gore kernel gerekli buttonlari enable eder.
    virtual int            CHANGER_BUTTON_CLICKED               (QAbstractButton * button);
    virtual void           PROCESS_ADAK_EVENTS                 ();
    QList<int> *           GET_ADAK_EVENT_QUEUE                ();

    QAbstractButton *      KERNEL_FIND_BUTTON_WIDGET            (QWidget * button_widget);

    virtual bool           eventFilter                          (QObject *, QEvent * );
    void                   SET_COMBOBOX_LENGTH                  (QComboBox * combobox , int length); 

protected slots:
    bool                   CHECK_VAR_CONTROL                    (QObject * object = NULL);
    int                    FOCUS_IN_EVENT                       (QObject * object = NULL);
    int                    FOCUS_OUT_EVENT                      (QObject * object = NULL);

    bool                   CHECK_VAR_FOCUS_EVENT                (QObject * object);
    void                   CHECK_VAR_SLOT_EVENT                 (QObject * object = NULL);

    void                   COMBO_BOX_LINE_EDIT_CHANGED          ();
    void                   SEARCH_EDIT_LINEEDIT_CHANGED         (QWidget * widget,QLineEdit * line_edit,QToolButton * push_button);
    void                   SEARCH_BUTTON_CLICKED                (QWidget * widget,QLineEdit * line_edit,QToolButton * push_button);
    void                   DISABLE_EVENTS_ON_WIDGET             (QWidget * widget);
    void                   WIDGET_TEXT_CHANGED                  ();

    virtual void           WIDGET_CHANGED                       ();
    virtual void           RESET_WIDGET_CHANGED                 ();
    virtual void           SEARCH_EDIT_CLICKED                  (QWidget * widget,QLineEdit * line_edit);
    virtual void           WIDGET_DESTROYED                     (QObject * destroyed_widget);

private slots:
    void                   SLOT_CHANGER_BUTTON_CLICKED          ();
    void                   WIDGET_ITEM_CHANGED                  ();
    void                   WIDGET_ITEM_DOUBLE_CLICKED           (int row,int column);
    void                   WIDGET_ITEM_CLICKED                  (int row,int column);    

private:
    virtual bool           IS_WIDGET_FOCUSABLE                  (QWidget *focus_widget);

    bool                   m_check_var_is_open;
    bool                   m_is_all_events_open;

    QList<QObject *>       m_disable_clear_widget_list;
    QList<int>             m_adak_event_queue;

    QList<QObject *>       m_widget_lists;

    QString                m_widget_undo_value;
    QWidget *              m_current_widget;
    QWidget *              m_event_eklenmiyecek_widget;
    QWidget *              m_last_focused_widget;
    QWidget *              m_previous_widget;
    QWidget *              m_next_focus_widget;
};

#endif // BASE_EVENT_KERNEL_H
