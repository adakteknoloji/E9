#ifndef MENU_KERNEL_H
#define MENU_KERNEL_H

#include <QMainWindow>
#include <QFrame>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

#include "abstract_kernel.h"

struct MENU_STRUCT {
    int      enum_id;
    QString  text_to_display;
    int      parent_enum_id;
    QString  icon_path;
    QString  shortcut;
    int      column_stretch;
    QString  tab_str;
    int      row_stretch;
    int      menu_column_visibility;
};

struct KERNEL_MENU_SECENEGI {
    int           enum_id;
    QString       text_to_display;
    int           parent_enum_id;
    QString       icon_path;
    QString       shortcut;
    QString       tab_str;
    int           column_stretch;
    int           row_stretch;
    bool          is_visible;
    bool          is_enable;
    int           num_of_childs;
    int           menu_level;
    int           menu_column_visibility;
};

enum {
    MENU_HEADER_ITEM = 10,
    MENU_BODY_ITEM   = 20
};

enum BUTTON_TEXT_POSITION {
    BUTTON_USTUNDE_SAG_TARAFTA = 10,
    BUTTON_USTUNDE_ALT_TARAFTA = 20,
    BUTTON_DISINDA_SAG_TARAFTA = 30,
    BUTTON_DISINDA_ALT_TARAFTA = 40,
    BUTTON_TEXT_YOK            = 50,
    BUTTON_ICON_YOK            = 60
};
enum MENU_COLUMN_VISIBILITY {
    MENU_GOSTERME        = 0 ,
    MENU_SAYFADA         = 10,
    MENU_TOPMENUDE       = 20,
    MENU_HERYERDE        = 30
};

class MENU_KERNEL : public QMainWindow, public ABSTRACT_KERNEL {
    Q_OBJECT

public:
                                    MENU_KERNEL                             (QWidget * parent = NULL);

protected:
    //Setlenmis olan menuleri yaratir bundan once setlenen hersey bu komutla yaratilir. Bu komuttan sonra menude
    //Degisikliklerin aktif olabilmesi REFRESH_MENU kodu cagirilmalidir.
    void                            START_KERNEL                            ();

    //Bu fonksiyonla menu struct setlenir.
    void                            SET_KERNEL_STRUCT                       ( MENU_STRUCT * menu_struct );

    //Bu fonksiyonla menulerin yaratilacagi widget setlenmeli
    void                            REGISTER_MENU_WIDGET                    ( QWidget * widget );

    //Bu fonksiyonlarla icon_size,style_sheet,button text position setlenebilir. Eger column_enum_id -1 olursa
    //Default olarak kullanilan degerler setlenebilir.
    void                            SET_ICON_SIZE                           ( int width,int height, int column_enum_num = -1);
    void                            SET_STYLE_SHEET                         ( QString style_sheet,  int column_enum_num = -1);
    void                            SET_BUTTON_TEXT_POS                     ( int button_pos_enum,  int column_enum_num = -1);

    //Bu fonksiyon enum_id gonderilen menu item dan sonra seperator ekler. Ayrica buttonlarin arasina bosluk koyar.
    void                            ADD_SEPERATOR                           ( int enum_id );

    //Bu fonsiyonlarla enum_id gonderilen menu_item i show,hide,enable veya disable yapabiliriz.
    void                            SHOW_MENU_ITEM                          (int enum_id);
    void                            HIDE_MENU_ITEM                          (int enum_id);
    void                            ENABLE_MENU_ITEM                        (int enum_id);
    void                            DISABLE_MENU_ITEM                       (int enum_id);

    //Menu yu tamamem clear edip bastan yaratir.
    void                            REFRESH_MENU                            ();

    //Menuye eklenen actionlari enable veya disable eder
    void                            HIDE_MENU_ACTIONS                       ();
    void                            SHOW_MENU_ACTIONS                       ();

    //Buton columnlarinin header bolumunu gizler.
    void                            HIDE_BUTTON_COLUMN_HEADER               ();
    void                            SHOW_BUTTON_COLUMN_HEADER               ();

    //Tum butonlari hidelar. Buton yetkilendirmesini resetlemek icin kullanilabilir.
    void                            HIDE_ALL                                ();


    virtual void                    SETUP_FORM                              () = 0;

    //Bu fonksiyona ise tiklanan button yada action un enum_id gonderilir.
    //Gelen enum_id ye gore gerekli fonksiyonlar cagrilmalidir.
    virtual void                    MENU_ITEM_CLICKED                       ( int enum_id ) = 0;


private:
    QWidget *                       m_main_widget;
    QWidget *                       m_menu_widget;

    int                             m_num_of_secenek;
    int                             m_secenek_no;
    KERNEL_MENU_SECENEGI *          M_KERNEL_MENU_SECENEGI;

    bool                            m_menu_actions_is_visible;
    bool                            m_menu_headers_is_visible;

    int                             m_max_item_count_on_column;
    int                             m_default_button_pos;
    QSize                           m_default_icon_size;
    QString                         m_default_style_sheet;

    QList<int>                      m_seperator_list;

    QList<QString>                  m_icon_sizes;
    QList<QString>                  m_style_sheets;
    QList<QString>                  m_button_positions;

    void                            FIND_CHILDS                             ( MENU_STRUCT * menu_struct ,int parent_secenek_no, int parent_enum_id, int menu_level);

    void                            SET_WIDGET_DEFAULTS                     (QObject * obj,int index);

    void                            ADD_MENU_KERNEL_BUTTON                  (QLayout * layout,int button_type,QString button_name,int index);
    void                            ADD_MENU_KERNEL_LABEL                   (QLayout * layout,int label_type ,QString label_name ,int index);

    void                            ADD_BOS_BUTTON_PROPERTY                 ();

    void                            CREATE_MENU                             ();
    void                            CLEAR_MENU                              ();

    void                            ADD_ITEMS_TO_MENU_COLUMN                (QLayout * layout,QList<QMenu *> * action_menu,int * number_of_child,int index);

    void                            ADD_SPACER_ITEM                         (QLayout * layout);
    void                            ADD_STATUS_BAR_INFO                     ();
    void                            INIT_KERNEL                             ();

    bool                            eventFilter                             (QObject * obj, QEvent * event);

    QVBoxLayout *                   ADD_FRAME                               (QLayout * layout,QWidget * parent,int frame_type,int index);
    QVBoxLayout *                   CREATE_NEW_MENU_COLUMN                  (QLayout * layout,QList<QMenu *> * action_menu,QLayout * parent_layout,int index);

    QLabel *                        CREATE_STATUS_BAR_LABEL                 ();

private slots:
    void                            SLOT_MENU_ITEM_CLICKED                  ();


};

#endif // MENU_KERNEL_H
