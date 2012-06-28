#ifndef SEKME_MENU_KERNEL_H
#define SEKME_MENU_KERNEL_H

#include <QMainWindow>
#include <QMap>
#include <QLabel>

#include "abstract_kernel.h"

class QFrame;
class QScrollArea;
class QLayout;
class QLabel;
class QMdiArea;
class QReklamViewer;
class QMenuBar;
class QAdakTabWidget;

enum MENU_ITEM_TYPES {
    MENU_ROOT      = -1,
    MENU_END       = -2,
    MENU_SEPERATOR = -3,
};

enum MENU_SCHEMA_WIDGET_TYPES {
     MENU_SCHEMA_SCROLLABLE = 0,
     MENU_SCHEMA_NO_SCROLL  = 1
};

struct MENU_STRUCT {
    int      enum_id;
    QString  text_to_display;    
    QString  menu_code;
    QString  icon_path;
    QString  shortcut;
    bool     is_header;
    int      menu_column;
    int      font_size_increment;
};

struct KERNEL_MENU_ITEM {
    int           enum_id;
    QString       text_to_display;
    int           parent_enum_id;
    QString       icon_path;
    QString       shortcut;
    bool          is_visible;
    bool          is_enable;
    bool          add_seperator_after;
    bool          is_header;
    int           num_of_childs;
    int           menu_column;
    int           font_size_increment; //label font size eklenecek deger
    int           font_dec_multiplier; //font azalma orani
};

struct MENU_PROPERTIES {
    int     column_font_diff;
    int     mouse_over_font_inc;
    int     label_heigth;
    int     label_width;

    QFont   font;

    QColor  parent_label_color;
    QColor  parent_label_mouse_over_color;
    QColor  child_label_color;
    QColor  child_label_mouse_over_color;
    QColor  header_color;
    QColor  select_label_color;
    QColor  background_color;

    QString default_font_weight;
    QString mouse_over_font_weight;
    QString background_img_path;
};

struct SYSTEM_MENU_ITEMS;


class SEKME_MENU_KERNEL : public QMainWindow , public ABSTRACT_KERNEL
{
    Q_OBJECT

public:
                                         SEKME_MENU_KERNEL              ( int program_id, QWidget * parent );
                                        ~SEKME_MENU_KERNEL              ();

    void                                 ADD_PARENT_TAB                 ( QWidget * widget , SCREEN_SIZE_ENUM p_screen_size);
    void                                 ADD_CHILD_DIALOG                  ( QWidget * widget , QWidget * parent , SCREEN_SIZE_ENUM p_screen_size);

    //Herseyi refresh eder(Yetkiler, widgetlar ...)
    void                                 REFRESH_MENU_KERNEL            ();

    void                                 SET_MENU_BOTTOM_TEXT           ( QString text );

    void                                 SET_NEW_MENU_PROPERTIES        ( MENU_PROPERTIES menu_properties );

    MENU_PROPERTIES                      GET_MENU_PROPERTIES            ( );

    void                                 CLEAR_MENU_PROPERTIES_STRUCT   ( MENU_PROPERTIES * menu_properties );

    void                                 SISTEM_MENU_SHOWS              ( SYSTEM_MENU_ITEMS &SYSTEM_STRUCT );

public slots:
    void                                 OPEN_EKRANI_AC_DIALOG          ();

protected:
    //Gosterilmesi istenen menu itemlerini gosterir.
    void                                 SHOW_MENU_ITEM                 ( int menu_item_enum );
    //Gosterilmesi istenmeyen menu itemlerini kapatir.
    void                                 HIDE_MENU_ITEM                 ( int menu_item_enum );

    //Ekranda menulerin yerlestirilecegi widget setlenir.
    void                                 REGISTER_WIDGET                ( QWidget * widget );

    //scroll lu ekran
    void                                 ENABLE_SCROLLABLE_MENUS        ();

    //scroll suz ekran
    void                                 DISABLE_SCROLLABLE_MENUS       ();

    //Bu fonksiyonda tiklanan item icin gerekli dialog olusturulup return edilmeli.
    virtual void                         MENU_ITEM_CLICKED              ( int parent_item_enum , int menu_item_enum ) = 0;

    virtual MENU_STRUCT *                GET_MENU_STRUCT                ( int parent_enum_id )                        = 0;

    virtual void                         PREPARE_PROGRAM_FOR_USER       () = 0;

    //Menu start kernel.
    void                                 START_MENU_KERNEL              ();

    //Menu gosterilir.
    void                                 SHOW_MENU                      ();

    //Ekrandaki tum itemlar gizlenir.
    void                                 HIDE_ALL                       ();

private:
    QWidget *                            m_main_widget;
    QLabel *                             m_background;
    QAdakTabWidget *                    m_tab_widget;
    QLabel *                             m_bottom_label;
    QString                              m_bottom_text;
    QMenu *                              m_menu;
    QList<int>                           m_column_lenghts;
    MENU_PROPERTIES                      m_menu_properties;

    bool                                 m_close_events;

    int                                  m_sub_window_width_inc;
    int                                  m_sub_window_height_inc;

    int                                  m_program_id;
    int                                  m_num_of_columns;
    int                                  m_container_type;

    QMap<QString,int>                    m_menu_indexes;
    std::vector<KERNEL_MENU_ITEM *>      m_menu_items;

    // 0 da daima main container olacak.
    QList<QLayout *>                     m_containers;
    QList<QWidget *>                     m_container_widgets;
    QList<QMdiArea *>                    m_mdi_areas;
    QList<QWidget *>                     m_tab_widgets;
    QList<QPushButton *>                 m_selected_buttons;
    QStringList                          m_menu_codes;

    QLabel                               m_sb_label_firma;
    QLabel                               m_sb_label_user;
    QLabel                               m_sb_label_pc;
    QLabel                               m_sb_label_version;

    int                                  ADD_TAB                        (QWidget * tab_widget , QString name);

    void                                 SET_WINDOW_DEFAULTS            ();
    void                                 SET_MAIN_MENU_DEFAULTS         ();

    void                                 ADD_MENU_BAR                   ( QAdakTabWidget * tab_widget       );
    void                                 ADD_MENU_BAR_CHILDS            ( QMenu * p_menu , int parent_id = 0 );

    void                                 DESTROY_ALL                    ();

    void                                 CREATE_MENU                    ( int parent_enum_id = MENU_ROOT, bool recursive = false );

    //Recursive true ise tum child menuler eklenir.
    QWidget *                            CREATE_MENU_COLUMN             ( int column_index , KERNEL_MENU_ITEM * menu_items , bool recursive = false );
    QWidget *                            CREATE_MENU_COLUMN             ( int column_index ,QList<int> * enum_ids         , bool recursive = false );

    void                                 CLEAR_SELECTED_FOCUS           ( int menu_column  );
    void                                 SET_STRUCT_DEFAULTS            ( KERNEL_MENU_ITEM * menu_item                           );
    void                                 FILL_MENU_STRUCT               ( int parent_index = -1 , int parent_enum_id = MENU_ROOT );

    QWidget *                            CREATE_TAB_WIDGET              ();

    QWidget *                            GET_CONTAINER_WIDGET           ( QWidget * widget    );

    int                                  MENU_INDEX_OF_WIDGET           ( QWidget * widget    );
    void                                 MENU_SET_OPENED_WIDGET         ( int index           );
    void                                 MENU_DELETE_WIDGET             ( QWidget * widget    );

    void                                 SET_LABEL_DEFAULT_PROPERTIES   ( QPushButton * label      );
    void                                 SET_LABEL_MOUSE_OVER_PROPERTIES( QPushButton * label      );
    void                                 SET_LABEL_SELECTED_PROPERTIES  ( QPushButton * label  , int menu_column  );

    int                                  GET_LABEL_WIDTH                ( int column_index    );
    int                                  GET_LABEL_HEIGHT               ( int column_index    );

    void                                 REPAINT                        ();

    bool                                 CLOSE_PROGRAM                  (bool no_message = false);

    bool                                 CLOSE_TAB                      (int index);

    bool                                 eventFilter                    ( QObject *, QEvent * );

public:
    int                                  GET_TAB_COUNT                  ();

private slots:    
    void                                 SLOT_MENU_CLICKED              ();
    void                                 SLOT_ITEM_CLICKED              ( QString str = "", QObject * obj = NULL );
    void                                 SLOT_CLOSE_WIDGET              ( int index                              );
    void                                 SLOT_CLOSE_CHILD_WIDGET        ();
    // Son tab kapatilgidan programi kapatir
    void                                 SLOT_CLOSE_LAST_TAB            ( QString tab_name );

    void                                 SLOT_MENU_TOOL_BUTTON_CLICK    ();

public slots:
    void                                 SLOT_SET_CURRENT_TAB_INDEX     ( int index );
};

#endif // SEKME_MENU_KERNEL_H
