#ifndef DMR_GRUP_TREE_SECIMI_H
#define DMR_GRUP_TREE_SECIMI_H

#include <tree_secim_kernel.h>

class DMR_GRUP_TREE_SECIMI : public TREE_SECIM_KERNEL
{

public:

    DMR_GRUP_TREE_SECIMI           (QWidget * parent = 0);
    ~DMR_GRUP_TREE_SECIMI          ();
     int *                               m_secilen_id;

protected:

    void         ADD_CHILD_ITEMS        (QStringList column_datas);
    void         ADD_ITEM               (QObject * button, int item_id );
    void         FILL_TREE_TABLE        ();
    int          SINGLE_LINE_SELECTED   (QStringList column_datas);

private:
    QToolButton * m_tool_button;

    QTreeWidget * m_tree_widget;
};

#endif // DMR_GRUP_TREE_SECIMI_H
