#ifndef XYZ_TREE_CLASS_H
#define XYZ_TREE_CLASS_H

#include <tree_kernel.h>

namespace Ui {
    class XYZ_TREE;
}

class XYZ_TREE : public TREE_KERNEL {
    Q_OBJECT

public:

    XYZ_TREE                                ( QWidget * parent = 0 );
    ~XYZ_TREE                               ();

private:

    Ui::XYZ_TREE *  m_ui;

    void                        ADD_CHILD_ITEMS   ( TREE_ITEM * item );
    void                        ADD_PARENT_ITEMS  ();
    void                        ADD_ITEM          ( QObject * button, TREE_ITEM * item );
    void                        UPDATE_ITEM       ( TREE_ITEM * item );
    int                         CHECK_DELETE_ITEM ( TREE_ITEM * item );
    void                        DELETE_ITEM       ( TREE_ITEM * item );
};

#endif // XYZ_TREE_CLASS_H
