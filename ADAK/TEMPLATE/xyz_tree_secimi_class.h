#ifndef XYZ_TREE_SECIMI_CLASS_H
#define XYZ_TREE_SECIMI_CLASS_H

#include <tree_secim_kernel.h>

class XYZ_TREE_SECIMI : public TREE_SECIM_KERNEL {

public:

    XYZ_TREE_SECIMI                               ( QWidget * parent = 0 );
    ~XYZ_TREE_SECIMI                              ();

private

    void                        ADD_CHILD_ITEMS   ( TREE_ITEM * item );
    void                        ADD_ITEM          ( QObject * button, int item_id );
    void                        FILL_TREE_TABLE   ();
    int                         DOUBLE_CLICKED    (TREE_ITEM * item );

    int       *                 m_secilen_id;
};


#endif // XYZ_TREE_SECIMI_CLASS_H
