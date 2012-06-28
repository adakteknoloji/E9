#ifndef QAdakDate_H
#define QAdakDate_H

#include <QtDesigner/QDesignerExportWidget>
#include <QWidget>
#include <QDate>

#include <QDate>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLineEdit>
#include <QKeyEvent>
#include <QCalendarWidget>

#ifdef WIN32
class QAdakDate : public QWidget
#else
class QDESIGNER_WIDGET_EXPORT QAdakDate : public QWidget
#endif
{
    Q_OBJECT
public:
    QAdakDate(QWidget *parent = 0);
    ~QAdakDate();

    // Default format dd MMMMMMMMM yyyy GGGGGGGGGGGG
    void                    SET_DATE_FORMAT                 (const QString& date_format);

    void                    SET_DATE                        (const QString& date);
    void                    SET_DATE                        (const QDate& date);


    void                    MALI_YIL_KISITI_YOK             ( bool p_disable );

    int                     MALI_YIL_ARRAY_INDIS            ();

    QString                 DATE                            ();
    QDate                   QDATE                           ();

protected:
    bool                    eventFilter                     (QObject *, QEvent *);

private:

    QDate                   m_current_date;
    bool                    m_mali_yil_disable;
    bool                    m_date_changed;

    // tarihi silip bir sey yazmadiginda eski tarihi getirmek icin
    QDate                   m_old_date;
    QString                 m_date_format;
    QString                 m_date_str;

    QHBoxLayout *           h_box_layout;
    QToolButton *           tool_button;
    QLineEdit *             line_edit;
    QCalendarWidget *       calendar_widget;

    void                    REFRESH_DATE                    ();
    void                    TRY_TO_SET_DATE                 (const QString& date_str);

private slots:
    void                    SLOT_POPUP_BUTTON_CLICKED       ();
    void                    SLOT_POPUP_DATE_SELECTED        (const QDate&);
    void                    SLOT_SET_DATE                   ();

signals:
    void                    SIGNAL_DATE_CHANGED             ();

};

#endif // QAdakDate_H



