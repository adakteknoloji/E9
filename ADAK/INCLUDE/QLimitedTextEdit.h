#ifndef QLimitedTextEdit_H
#define QLimitedTextEdit_H

#include <QTextEdit>
#include <QLocale>
#include <QtDesigner/QDesignerExportWidget>

//Deger girelen tum formlarda lineedit yerine commaedit kullanilmalidir
//Boylikle -(eksi) deger girilmesi durumunuda kontrol altina aliriz.

#ifdef WIN32
class QLimitedTextEdit : public QTextEdit
#else
class QDESIGNER_WIDGET_EXPORT QLimitedTextEdit : public QTextEdit
#endif
{
    Q_OBJECT

public:
    QLimitedTextEdit                            (QWidget *parent = 0);
    ~QLimitedTextEdit                           ();

    //Karakter limitlemek icin kullanilacak fonksiyon
    void SET_MAX_STRING_LENGTH                  (int max_string_length);

    //Satir olarak limitlemek istersek kullanilacak fonksiyon
    void SET_LINE_LIMIT                         (int line_limit);

    //Bu fonksiyon kalan karakter sayisini dondurur.
    int  GET_REMAINING_LENGTH                   ();

private slots:
    void TEXT_EDIT_CHANGED                      ();

private:
    int  m_line_limit;
    int  m_max_str_length;
    int  m_remaing_length;

    void SET_TEXT                               (QString str);
    void keyPressEvent                          (QKeyEvent * e);
};

#endif // QLimitedTextEdit_H



