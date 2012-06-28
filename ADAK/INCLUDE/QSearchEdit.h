#ifndef QSearchEdit_H
#define QSearchEdit_H

#include <QToolButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLocale>
#include <QtDesigner/QDesignerExportWidget>

//NOT:: Bu widget kullanilmak istendiginde slot olarak SEARCH_BUTTON_CLICKED(QWidget *,QLineEdit *,QPushButton *) tanimlanmali.
//Bu sekilde widgettaki butona tiklandiginda otomatik olarak bu slot a duser.

#ifdef WIN32
class QSearchEdit : public QWidget
#else
class QDESIGNER_WIDGET_EXPORT QSearchEdit : public QWidget
#endif
{
    Q_OBJECT

public:
    QSearchEdit(QWidget *parent = 0);
    ~QSearchEdit();

    void            SET_BUTTON_TEXT        (QString text);
    void            SET_BUTTON_ICON        (QString icon_path);
    void            SET_BUTTON_SIZE        (int height,int width);

    //Search Edit de bulunan buton a shortcut atar
    //
    //SET_SHORTCUT(Qt::Key_F8);
    void            SET_SHORTCUT           (int qt_key);

    //Search Edit de bulunan line edit e text atar.SET_TEXT ile atanan degerler de
    //check var dusmez.
    void            SET_TEXT               (QString text);

    //Search Edit de bulunan line edit ki text i okur.
    QString         GET_TEXT               ();

    //Search Edit de bulunan line edit i limitler
    void            SET_MAX_LENGTH         (int length);

    void            CLEAR                  ();


private:
    QHBoxLayout *   h_box_layout;
    QToolButton *   tool_button;
    QLineEdit *     line_edit;

    int             qt_key;

    void            keyPressEvent(QKeyEvent *);
    bool            line_edit_changed;
    QString         m_last_entered_value;

signals:
    void            SIGNAL_BUTTON_CLICKED   (QWidget *,QLineEdit *,QToolButton *);
    void            SIGNAL_LINE_EDIT_CHANGED(QWidget *,QLineEdit *,QToolButton *);

private slots:
    void            SLOT_BUTTON_CLICKED();
    void            SLOT_LINE_EDIT_CHANGED();
    void            SLOT_SET_LINE_EDIT_CHANGED_FLAG();

};

#endif // QSearchEdit_H



