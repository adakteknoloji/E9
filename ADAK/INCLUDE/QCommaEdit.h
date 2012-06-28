#ifndef QCommaEdit_H
#define QCommaEdit_H

#include <QLineEdit>
#include <QLocale>
#include <QtDesigner/QDesignerExportWidget>

//Deger girelen tum formlarda lineedit yerine commaedit kullanilmalidir
//Boylikle -(eksi) deger girilmesi durumunuda kontrol altina aliriz.

#ifdef WIN32
class QCommaEdit : public QLineEdit
#else
class QDESIGNER_WIDGET_EXPORT QCommaEdit : public QLineEdit
#endif
{
    Q_OBJECT

public:
    QCommaEdit(QWidget *parent = 0);
    QString COMMA_FLOAT_CEVIR(const QString &);

    void    SET_PRECISION    (int precision);
    void    SET_DOUBLE       (const double deger);
    void    SET_INTEGER      (const int    deger);

    //SQL icin gerekli degeri dondurur. 0 veya 0.00 degerlerinde empty dondurmez!
    double  GET_DOUBLE       ();
    int     GET_INTEGER      ();

    //Commaedit deki degeri okur.
    QString GET_TEXT         ();
    void    UNDO_TEXT         (const QString &);  // Sadece UNDO icin KERNEL ' lar kullaniyor. KULLANMAYIN

    //SET_RETURN_EMPTY i eger true sa gonderilen deger 0.00 veya 0 ise ""(empty) dondurur.
    //false olursa eger bu islevi yapmaz.Default true dur.
    void    SET_RETURN_EMPTY (bool value);

    //comma editin eksi degerleri kabul edip etmeyecegini setler.Eger true ysa eksi deger kabul eder.
    //false sa kabul etmez.Default olarak false gelir.
    void    EKSI_DEGER_GIRILSIN(bool value = true);

    //Integer degerler kabul eder araya virgul ve nokta gibi isaretleri koymaz.
    void    VIRGULSUZ_CALIS(bool value = true);

    //Line editten textin hangi tarafa align olacagini setler.Default saga dayalidir
    //Eger sol a dayali olmasi istenirse SET_ALIGNMENT(Qt::AlignLeft); cagrilmali.
    void    SET_ALIGNMENT(Qt::Alignment flag);

    ~QCommaEdit();

private slots:
    void SAYIYA_VIRGUL_EKLE (const QString &);
    void setText            (const QString &);

private:    
   QString         NUMERIC_OLMAYAN_KARAKTERLERI_SIL(const QString &cevrilecek_sayi);
   void            SET_LINE_EDIT_TEXT(const QString &);
   void            focusOutEvent(QFocusEvent *);
   void            focusInEvent (QFocusEvent *);
   QChar           p_minus_sign;
   bool            p_return_empty;
   bool            p_eksi_deger_girilsin;
   bool            p_virgulsuz_calis;
   int             p_precision;

   const QString   text();
   QString         REMOVE_COMMA();
};

#endif // QCommaEdit_H



