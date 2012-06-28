#ifndef BASE_TREE_KERNEL_H
#define BASE_TREE_KERNEL_H

#include <QTreeWidget>
#include <QDialog>
#include <QAbstractButton>
#include <QEvent>
#include <QCloseEvent>
#include <QTreeWidgetItem>
#include <QCheckBox>
#include "adak_sql.h"
#include "base_kernel.h"

class BASE_TREE_KERNEL : public BASE_KERNEL
{
        Q_OBJECT
public:
    BASE_TREE_KERNEL                                                    (QWidget * parent);
    ~BASE_TREE_KERNEL                                                   ();

protected:
    int                             m_column_info_start_index;
    QStringList                     m_column_headers;
    QTreeWidget *                   m_tree_widget;    

    void                            REGISTER_TREE_WIDGET                ( QTreeWidget * tree_widget );

    void                            SET_COLUMN_HEADERS                  (const QStringList column_headers);

    void                            SET_RIGHT_ALIGN_COLUMNS             (QList<int> list);

    virtual void                    ADD_CHILD_ITEMS                     (QStringList column_datas)                    = 0;
    virtual void                    ADD_PARENT_ITEMS                    ()                                            = 0;

    virtual void                    ADD_PARENT_ITEM                     (QStringList column_datas,int number_of_child );
    virtual void                    ADD_CHILD_ITEM                      (QStringList column_datas,int number_of_child );
    virtual void                    UPDATE_SELECTED_ITEM                (QStringList column_datas);

    //istenilen column i hide lar
    void                            HIDE_COLUMN                         (int column_name);

    void                            REFRESH_TREE_SELECTED_ITEM          ();
    void                            REFRESH_TREE_WIDGET                 ();

    QStringList                     GET_COLUMN_DATAS_FROM_ITEM          (QTreeWidgetItem * item);

    void                            SET_ANA_GRUPLAR_SELECTABLE          (bool value);
    void                            SET_TREE_WIDGET_ITEM_ALIGN          (QTreeWidgetItem * item);

    //ADD_CHILD_ITEM veya ADD_PARENT_ITEM yapilarak eklenen son itemi tutar.
    //Fonksiyonla item alindiktan sonra kernel NULL yapar.
    QTreeWidgetItem *               GET_LAST_ADDED_ITEM                 ();

    int                             GET_ANA_GRUPLAR_SELECTABLE          ();

private:

    QTreeWidgetItem *               m_last_expanded_item;

    QTreeWidgetItem *               m_last_added_item;

    QList<int>                      m_right_align_columns;

    bool                            m_ana_gruplar_selectable;

    void                            TREE_KERNEL_ADD_CHECK_BOX           (QTreeWidgetItem * item , int column,int child_count, bool select_item_if_multi_row_enable = false);

private slots:
    void                            ITEM_EXPANDED                       (QTreeWidgetItem * item);
    void                            ITEM_COLLAPSED                      (QTreeWidgetItem * item);
    void                            UPDATE_TREE_VIEW                    ();

};

#endif // BASE_TREE_KERNEL_H
