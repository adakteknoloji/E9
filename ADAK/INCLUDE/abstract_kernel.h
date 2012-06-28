#ifndef ABSTRACT_KERNEL_H
#define ABSTRACT_KERNEL_H

#include "adak_utils.h"


enum SCREEN_SIZE_ENUM {
    FULL_SCREEN      = 1,
    NOT_FULL_SCREEN  = 2
};

class ABSTRACT_KERNEL
{
public:
    ABSTRACT_KERNEL();
    ~ABSTRACT_KERNEL();

protected:
    void                            MSG_ERROR                           (QString message,QObject * focus_widget);
    void                            MSG_WARNING                         (QString message,QObject * focus_widget);
    void                            MSG_INFO                            (QString message,QObject * focus_widget);
    ADAK_MSG_ENUM                   MSG_YES_NO                          (QString message,QObject * focus_widget);
    ADAK_MSG_ENUM                   MSG_YES_NO_CANCEL                   (QString message,QObject * focus_widget);

    virtual void                    SET_FOCUS                           (QWidget * focus_widget);

    virtual bool                    IS_WIDGET_FOCUSABLE                 (QWidget * focus_widget);

    void                            SET_THIS                            (QWidget * parent);

private:
    QWidget *                       m_parent;

};

#endif // ABSTRACT_KERNEL_H
