#ifndef FIS_KERNEL_H
#define FIS_KERNEL_H

#include "form_kernel.h"
#include "adak_sql.h"

#define   ORDER_NUMBER_RANGE_VALUE 1024

enum ROW_WIDGET_TYPE {
    NO_TYPE                         = -1,
    WIDGET_LINE_EDIT                =  0,
    WIDGET_COMBO_BOX                =  1,
    WIDGET_COMMA_EDIT               =  2,
    WIDGET_PUSH_BUTTON              =  3,
    WIDGET_CHECK_BOX                =  4,
    WIDGET_SEARCH_EDIT              =  5,
    WIDGET_DATE_EDIT                =  6,
    WIDGET_COMBO_BOX_EDITABLE       =  7,
    WIDGET_ADAK_DATE               =  8,
};

enum {
    CHANGER_LINE_SLOT      = 10,
    SAVER_LINE_SLOT        = 20
};

class FIS_KERNEL : public FORM_KERNEL
{
    Q_OBJECT
public:
    FIS_KERNEL(QWidget * parent = 0);
    virtual ~FIS_KERNEL();

    void                   AUTOSTART_FIS_KERNEL                 (int record_id);

public slots:
    void                   TABLE_WIDGETS_CHANGED                ();

protected:
    virtual void           NEW_RECORD                           ();
    virtual int            GET_RECORD                           (int fis_id);
    virtual int            FIND_RECORD                          ();
    virtual int            CHECK_VAR                            (QObject * object);
    virtual int            CHECK_EMPTY                          ();
    virtual int            CHECK_ADD                            ();
    virtual int            ADD_RECORD                           ();
    virtual int            CHECK_UPDATE                         (int fis_id);
    virtual void           UPDATE_RECORD                        (int fis_id);
    virtual int            CHECK_DELETE                         (int fis_id);
    virtual void           DELETE_RECORD                        (int fis_id);
    virtual int            FIND_NEXT_RECORD                     ();
    virtual int            FIND_PREV_RECORD                     ();
    virtual int            FIND_FIRST_RECORD                    ();
    virtual int            FIND_LAST_RECORD                     ();
    virtual int            LOCK_RECORD                          (int fis_id);
    virtual void           UNLOCK_RECORD                        (int fis_id);
    virtual int            SELECT_RECORD                        ();

    virtual void           START_FIS_KERNEL                     (QObject * parent,ADAK_SQL * database);
    virtual void           NEW_FIS_RECORD                       () = 0;
    virtual int            GET_FIS_RECORD                       (int fis_id) = 0;
    virtual int            FIND_FIS_RECORD                      () = 0;
    virtual int            CHECK_FIS_FORM_VAR                   (QObject * object) = 0;
    virtual int            CHECK_FIS_FORM_EMPTY                 () = 0;
    virtual int            CHECK_FIS_RECORD_ADD                 () = 0;
    virtual int            ADD_FIS_RECORD                       () = 0;
    virtual int            CHECK_FIS_RECORD_UPDATE              (int fis_id) = 0;
    virtual void           UPDATE_FIS_RECORD                    (int fis_id) = 0;
    virtual int            CHECK_FIS_RECORD_DELETE              (int fis_id) = 0;
    virtual void           DELETE_FIS_RECORD                    (int fis_id) = 0;
    virtual int            FIND_NEXT_FIS_RECORD                 () = 0;
    virtual int            FIND_PREV_FIS_RECORD                 () = 0;
    virtual int            FIND_FIRST_FIS_RECORD                () = 0;
    virtual int            FIND_LAST_FIS_RECORD                 () = 0;
    virtual int            LOCK_FIS_RECORD                      (int fis_id) = 0;
    virtual void           UNLOCK_FIS_RECORD                    (int fis_id) = 0;
    virtual int            SELECT_FIS_RECORD                    () = 0;

    virtual void           SET_LINE_DEFAULTS                    (int row_number) = 0;
    virtual int            CHECK_LINE_EMPTY                     (int row_number) = 0;

    // CHECK_LINE_VAR fonksiyonunda widget icerisine girilen veri neticesinde veritabaninda bir id tespit edilmisse
    // bu id ( mecburi degil ama genelde boyle bir kullanim vardir ) gizli bir fis satiri kolonunda saklanarak
    // ADD_LINE veya UPDATE_LINE gibi fonksiyonlarda direkt veritabanina yazilmak icin saklanmis olur.
    // Boylece kayit ani geldiginde CHECK_LINE_VAR neticesinde elde edilen id nin kesin olarak elde edilmis olmasi
    // garanti altina alinmis olur. ADD_LINE veya UPDATE_LINE gibi fonksiyonlarda ikinci bir sorguyla id
    // tespit etmek istenilen bir durum degildir. GET_FIS_RECORD fonksiyonunda satirlar doldurulurken gizli satir id
    // alaninin doldurulmasi unutulmamalidir.
    virtual int            CHECK_LINE_VAR                       (int row_number, QObject * object) = 0;

    virtual int            CHECK_ADD_LINE                       (int fis_id, int row_number) = 0;
    virtual void           ADD_LINE                             (int fis_id, int row_number) = 0;
    virtual int            CHECK_UPDATE_LINE                    (int fis_id, int row_number) = 0;
    virtual void           UPDATE_LINE                          (int fis_id, int row_number) = 0;
    virtual int            CHECK_DELETE_LINE                    (int fis_id, int row_number) = 0;
    virtual void           DELETE_LINE                          (int fis_id, int row_number) = 0;

    //SET_FIS_ORDER_DATABASE de atanan degerlere gore database in order number column ini update eder
    //Virtual bir fonksiyondur.Istenirse tekrar tanimlanabilir.
    virtual void           REORGANIZE_ORDER_NUMBER              (int row_number,int new_order_number);

    virtual void           SELECTED_LINE                        (int line_id,int row_save_status);

    //satira eklenen search_editler icin gereklidir.Sadece search editteki buttona tiklanildiginda buraya duser.
    //line edite yazilan textler de focus out olundugunda check var a duser.
    virtual void           LINE_SEARCH_EDIT_CLICKED             (int row_number,QWidget *,QLineEdit *);

    int                    ADD_NEW_LINE                         (int current_row = -1, bool close_event = true);

    //Line eklenen buttonlara isim atamak icin kullanilir.Buttonlarin eklenme sirasina gore isimler gonderilmelidir.
    //INIT_FIS_KERNEL dan once kullanilmali
    void                   SET_LINE_BUTTON_ICON                 (QStringList icon);    

    void                   SET_LINE_CHECK_BOX_TEXT              (QStringList check_box_names);

    //Table Widget da focus atanmak istendiginde kullanilabilir.
    void                   SET_TABLE_WIDGET_FOCUS               (int row , int column );

    //Satirdaki cell icin shortcut atar. Bunun icin hangi column da shortcut kullanmak istiyorsak bunu keysequence le birlikte
    //set lemek yeterli olacaktir.
    //
    //Ornek;SET_BUTTON_SHORT_CUT(2, QKeySequence("Ctrl+B"));
    void                   REGISTER_BUTTON_SHORT_CUT            (int column,QKeySequence qt_key);
    void                   REMOVE_BUTTON_SHORT_CUT              (int column);

    //Bu fonksiyonla table widget daki comma editlerin Precision degerleri atanabilir.Eger precision deger 0 olursa
    //Comma edit int deger kabul eder ve ondalik kismi eklemez.
    void                   SET_COMMA_EDIT_PRECISION             (int column,int precision);

    void                   REGISTER_TABLE_WIDGET                (QTableWidget *table_widget, int column_number);

    //columnlari scroll barlar geldiginde otomatik olarak resize etmek icin kullanilir.
    void                   SET_FIS_ORDER_COLUMN_NUMBER          (int fis_order_column_number);
    void                   SET_NUMBER_OF_HIDDEN_COLUMN          (int number_of_hidden_column);
    // FIS KERNEL duzgun calisabilmek icin ROW_ID ve FORM_ID colonlarinda kayitli bir satir icin her zaman
    // dogru degerlerin setlenmis olmasini bekler. Bunun icin GET_FIS_RECORD da herbir satir verileri alindiginda,
    // ADD_LINE da yeni bir satir kaydedildikten sonra ve UPDATE_LINE da satir silme ekleme islemi yapildiysa
    // bunun islem sonunda, satir_id degistiginden gerekli kolonlarda ROW_ID ve FIS_ID her zaman setlenmelidir.
    void                   SET_FORM_ID_COLUMN_POSITION          (int form_id);
    void                   SET_ROW_ID_COLUMN_POSITION           (int row_id_column_number);
    void                   SET_TABLE_ROW_WIDGETS                (int column_number,ROW_WIDGET_TYPE type);

    //Cell deki widgetlari dinamik olarak setlemek icin kullanilir. Widget turu,column ve row gonderilir.
    //Eger row gonderilmezse current row u kullanir.
    void                   SET_CELL_WIDGET                      (int widget_type,int column,int row = -1);

    //Eger REORGANIZE_ORDER_NUMBER yeniden tanimlanmazsa kernel SET_FIS_ORDER_DATABASE atanan database bilgilerine gore
    //SADECE fis satirlarinin order numberlarini update edecek sekilde calisir.
    void                   SET_FIS_ORDER_DATABASE               (QString table_name, QString fis_order_column_name,QString line_id_column_name);

    //Bu fonksiyonla table widget disinda bulunan widgetlarin, line widget olarak kullanilmasini saglayabiliriz.
    //Boylelikle bu widgetlarda degisiklik oldugunda save durumunda SAVE_LINE a duser.
    //Bu widgetlarin degerlerini GET_FORM yerine SELECTED_LINE da doldurmaliyiz.
    //
    //Ayrica bu widgetlarin SET_DEFAULT_LINE da temizlenmesi lazim.Ve widgetlarin hangi sira ile focus
    //olmasini istiyorsak o sira ile register etmemiz lazim.Readonly ve disabled olan widgetlar register
    //edilse bile readonly ve disabled diye kernel tarafindan focuslanmaz.
    void                   REGISTER_LINE_WIDGET                 (QWidget * widget);

    //gonderilen id ye gore fis i yeniler.Eger id gonderilmezse acik olan fis yenilenir
    void                   REFRESH_FIS_RECORD                   (int record_id = -1);

    bool                   eventFilter                          (QObject *, QEvent * );


    virtual void           INCREASE_DEGISEN_WIDGET_COUNT        (QWidget *);
    virtual void           DECREASE_DEGISEN_WIDGET_COUNT        (QWidget *);

    //Changer button setlenir.Her turlu(Toolbutton,PushButton,Search edit vs..)
    //button iadak widget setlenebilir.Kernel otomatik olarak
    //button objesini bulup gerekli set leri yapacaktir.
    void                   REGISTER_LINE_CHANGER_BUTTON         (int row,int column);
    void                   REGISTER_LINE_SAVER_BUTTON           (int row,int column);

    //Changer buttona tiklanica bu fonksiyona duser.Donus degerine gore ADAK_RECORD_CHANGED ve ADAK_RECORD_UNCHANGED
    //gore kernel gerekli butonlari enable eder.
    virtual int            LINE_CHANGER_BUTTON_CLICKED          (QAbstractButton * button,int row_number);

    //row_id satir kaydinin veritabanindaki idsidir, row_number kaydin satir numarasidir.
    virtual void           LINE_SAVER_BUTTON_CLICKED            (QAbstractButton * button,int row_id,int record_id,int row_number);


protected slots:
    virtual void           WIDGET_CHANGED                       ();

private:
    QTableWidget *         p_table_widget;
    QList<int>             p_widget_types;
    QList<int>             p_push_button_columns;
    QList<int>             p_search_edit_columns;
    QList<int>             p_comma_edit_precision;
    // QList<int>             p_resize_column_numbers;
    // QList<int>             p_resize_column_width;
    QList<int>             m_changer_buttons_column;
    QList<int>             m_saver_buttons_column;
    QList<QWidget *>       p_registered_line_widget;
    QList<QAction *>       p_table_widget_actions;
    QStringList            p_line_buttons_icon_name;
    QStringList            p_line_check_box_names;
    int                    p_row_id_column_number;
    int                    p_number_of_column;
    int                    p_fis_order_column_number;
    int                    p_row_status_column_number;
    int                    p_first_visible_column;
    int                    p_number_of_hidden_column;
    int                    p_form_id_column_number;
    int                    m_satir_degisen_widget_count;
    int                    p_current_row;
    int                    p_current_column;
    int                    p_previous_row;
    int                    p_previous_column;
    int                    p_is_table_filling;
    int                    p_insert_button_column;
    int                    p_delete_button_column;
    int                    p_last_added_row;
    bool                   p_block_signals;
    bool                   p_focus_in_event_status;
    QString                p_db_table_name;
    QString                p_db_fis_order_column_name;
    QString                p_db_line_id_column_name;
    QString                p_this_object_name;

    void                   INIT_KERNEL                          ();

    int                    GET_LAST_COLUMN_FOR_NEW_LINE         ();

    int                    FIND_FOCUSED_ROW                     (int column_number);
    int                    CALL_CHECK_LINE_VAR                  (int row_number , QObject * obj);
    bool                   IS_FORM_CHANGED                      ();
    bool                   CHECK_ADD_NEW_ROW                    ();
    void                   ADD_INSERT_BUTTON                    (int current_row);
    void                   ADD_DELETE_BUTTON                    (int current_row);
    void                   ADD_PUSH_BUTTON                      (int row, int column);
    void                   UPDATE_ROW_STATUS                    (int row_number,RECORD_SAVE_STATUS type);
    void                   ENABLE_DELETE_BUTTON                 (int button_row);
    void                   DISABLE_DELETE_BUTTON                (int button_row);
    void                   CHECK_ADD_EMPTY_ROW                  ();
    void                   ADD_ORDER_NUMBER                     (int row_number);
    void                   CHECK_UNSAVED_LINES                  ();
    void                   CLEAR_TABLE_WIDGET                   ();
    void                   SEND_TAB_KEY_EVENT                   (int current_row = -1,int current_column = -1);
    void                   SET_TABLE_WIDGET_DEFAULTS            (QString save_name = "");
    void                   SAVE_TABLE_WIDGET_DEFAULTS           (QString save_name = "");
    void                   LINE_BUTTONS_SLOT_FUNCTION           (QObject * obj,QList<int> * button_columns,int slot_type);
    void                   LINE_BUTTONS_REGISTER_FUNCTION       (int row,int column,QList<int> * button_columns,int slot_type);

private slots:
    void                   INSERT_ROW                           ();
    void                   DELETE_ROW                           ();
    void                   CURRENT_CELL_CHANGED                 (int, int, int, int);
    void                   CELL_BUTTON_CLICKED                  ();
    void                   LINE_SEARCH_BUTTON_CLICKED           (QWidget *,QLineEdit *,QToolButton *);
    void                   LINE_SEARCH_EDIT_LINEEDIT_CHANGED    (QWidget *,QLineEdit *,QToolButton *);
    void                   COPY_PREVIOUS_CELL_CURRENT           ();

    void                   SLOT_LINE_CHANGER_BUTTON_CLICKED     ();
    void                   SLOT_LINE_SAVER_BUTTON_CLICKED       ();

    void                   SLOT_SHORT_CUT_PRESSED               ();

};


#endif // FIS_KERNEL_H

