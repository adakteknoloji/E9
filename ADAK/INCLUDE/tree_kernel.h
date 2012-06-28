#ifndef TREE_KERNEL_H
#define TREE_KERNEL_H

#include <QTreeWidget>
#include <QDialog>
#include <QAbstractButton>
#include <QEvent>
#include <QCloseEvent>
#include <QTreeWidgetItem>
#include "adak_sql.h"
#include "base_tree_kernel.h"

class TREE_KERNEL : public BASE_TREE_KERNEL
{
        Q_OBJECT
public:
    TREE_KERNEL                                                         (QWidget * parent);
    ~TREE_KERNEL                                                        ();

protected:
    void                            INIT_KERNEL                         (QObject * parent,ADAK_SQL * database);

    void                            REGISTER_ADD_BUTTON                 (QAbstractButton * add_button);
    void                            REGISTER_UPDATE_BUTTON              (QAbstractButton * update_button);
    void                            REGISTER_DELETE_BUTTON              (QAbstractButton * delete_button);
    void                            REGISTER_HELP_BUTTON                (QAbstractButton * help_button);

    void                            SET_ACTION_MENU_TEXT                (QString add_button,QString update_button,QString delete_button);

    void                            REGISTER_BUTTON                     (QAbstractButton * button);

    void                            ADD_VALUE_TO_DELETED_ITEM_PARENT    (int added_column , int value);
    void                            ADD_VALUE_TO_ADDED_ITEM_PARENT      (int added_column , int value);

    virtual void                    BUTTON_CLICKED                      (QAbstractButton * button,QStringList column_datas);

    //Eger column datas bos gelirse bu secili bir item olmadigini gosterir.
    virtual void                    ADD_ITEM                            (QObject * button,QStringList column_datas)   = 0;
    virtual void                    UPDATE_ITEM                         (QStringList column_datas)                    = 0;

    virtual int                     CHECK_DELETE_ITEM                   (QStringList column_datas)                    = 0;
    virtual void                    DELETE_ITEM                         (QStringList column_datas)                    = 0;



private:
    QAction *                       item_add;
    QAction *                       item_delete;
    QAction *                       item_update;

    void                            SET_TREE_WIDGET_DEFAULTS            ();

    QTreeWidgetItem *               m_last_deleted_item_parent;
    QTreeWidgetItem *               m_last_added_item_parent;

private slots:
    void                            SLOT_ADD_ITEM                       ();
    void                            SLOT_UPDATE_ITEM                    ();
    void                            SLOT_DELETE_ITEM                    ();
    void                            SLOT_BUTTON_CLICKED                 ();
};

#endif // TREE_KERNEL_H
