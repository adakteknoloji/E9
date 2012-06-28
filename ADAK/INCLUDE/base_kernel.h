#ifndef BASE_KERNEL_H
#define BASE_KERNEL_H

#include <QDialog>
#include <QTableWidget>
#include <QTreeWidget>
#include "adak_sql.h"

#include "QLimitedTextEdit.h"
#include <QLineEdit>
#include <QLabel>
#include "adak_defines.h"

#include "abstract_kernel.h"

//Resize olmasi istenmeyen widgetlarin object nameleri NO_EVENT_ON_WIDGET olmali

class BASE_KERNEL : public QDialog , public ABSTRACT_KERNEL
{
    Q_OBJECT
public:
    BASE_KERNEL(QWidget * parent);
    ~BASE_KERNEL();

    int                             EXEC ( SCREEN_SIZE_ENUM p_screen_size );
    void                            SHOW ( SCREEN_SIZE_ENUM p_screen_size );

protected:
    QString     m_kernel_button_text;
    int         m_num_of_transactions;
    ADAK_SQL * m_db_conn;

    //Ekran kapatilmasi yani close durumdalarinda bu fonksiyona duser.
    virtual int                     CHECK_EXIT                          ();

    virtual bool                    eventFilter                         (QObject * obj, QEvent * event);

    //Bu fonksiyon otomatik olarak batch acildiginda hangi widget a focus lanilmasi gerekiyorsa ona focuslanir.
    //Constructor de bu fonksiyonu kullanarak bu widget i setlememiz yeterlidir.INIT den once kullanirsa ilk acilista da otomatik
    //olarak focuslanir.
    void                            SET_FIRST_FOCUS_WIDGET              (QWidget * focus_widget);

    void                            SET_PAGE_TITLE                      (QString page_title);
    void                            SET_HELP_PAGE                       (QString help_page);
    void                            SET_SETTING_NAME                    (QString setting_name);

    QString                         PAGE_TITLE                          ();
    QString                         HELP_PAGE                           ();
    QString                         SETTING_NAME                        ();    

    //Setlenen first focus widget a focuslanir.Normal de kernel ilk giriste otomatik olarak bunu kullanir.
    //Ancak bu durumun haricinde focuslanilmak istenirse bu fonsiyon kullanilabilir.
    void                            FOCUS_FIRST_WIDGET                  ();

    //Bu fonksiyon mdi child yada parent,  penadakin kapanmasi icin gerekli widget i bulur ve kapar.
    void                            ADAK_CLOSE                         ();
    void                            SET_DB_CONN                         (ADAK_SQL * db);

    void                            START_KERNEL_TRANSACTION            ();
    void                            COMMIT_KERNEL_TRANSACTION           ();
    void                            CANCEL_KERNEL_TRANSACTION           ();

    void                            SET_ADAK_WINDOW_DEFAULTS           (QObject * this_ptr);
    bool                            IGNORE_CLOSE_EVENT                  (QEvent * close_event);

    virtual void                    SEND_ADAK_ERROR_EMAIL              ();

    bool                            IS_ADAK_HELP_URL_VALID             (QString url);
    void                            SHOW_NO_HELP_DIALOG                 ();

    void                            OPEN_HELP_PAGE                      (QString yardim_sayfasi = "");

private:
    int                             m_resize_count;

    SCREEN_SIZE_ENUM                m_screen_size_enum;

    QString                         m_setting_name;
    QString                         m_help_page;
    QString                         m_page_title;
    QDialog *                       m_main_this;
    QWidget *                       m_first_focus_widget;

    QList<QTableWidget *         >  m_resized_table_widgets;
    QList<QTreeWidget *          >  m_resized_tree_widgets;
    QList<QAbstractItemView *    >  m_resized_views;
    QList<QAbstractItemModel *   >  m_resized_models;

    QStringList                     m_resized_widgets_default_size;
    QList<bool>                     m_widget_resized;
    bool                            m_close_resize_slot;


    QLineEdit *                     m_line_edit_program_adi;
    QLineEdit *                     m_line_edit_bulunamayan_sayfa;
    QTextEdit *                     m_yetersiz_sayfa_aciklamasi;
    QDialog *                       m_error_dialog;
    QLineEdit *                     m_lineEdit_email_adres;

    int                             exec();
    void                            show();

    void                            FIND_RESIZE_WIDGETS                 (QObject * parent);
    void                            SET_CURSOR_POS_TEXT_WIDGETS         (QObject * parent = NULL);
    void                            RESIZE_WIDGETS                      ();
    void                            UPDATE_DEFAULT_WIDGET_SIZES         ();
    void                            SAVE_WINDOW_SIZES                   ();
    void                            LOAD_WINDOW_SIZES                   ();

    bool                            SHOW_AND_EXEC_COMMON_CODE           ( SCREEN_SIZE_ENUM p_screen_size );

protected slots:
    void                            YARDIM_BUTTON_CLICKED               (QString yardim_sayfasi = "");

private slots:
    void                            SLOT_SEND_ADAK_ERROR_EMAIL         ();
};

#endif // BASE_KERNEL_H
