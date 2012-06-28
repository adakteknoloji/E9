#ifndef BORDRO_INCELEME_TREE_SECIMI_CLASS_H
#define BORDRO_INCELEME_TREE_SECIMI_CLASS_H

#include <tree_secim_kernel.h>

class BORDRO_INCELEME_TREE_SECIMI : public TREE_SECIM_KERNEL
{

public:

    BORDRO_INCELEME_TREE_SECIMI           (QWidget * parent = 0);
    ~BORDRO_INCELEME_TREE_SECIMI          ();
     int *                               m_secilen_id;

protected:
    void ADD_CHILD_ITEMS        (QStringList column_datas);
    void ADD_ITEM               (QObject * button, int item_id );
    void FILL_TREE_TABLE        ();
    int  SINGLE_LINE_SELECTED   (QStringList column_datas);

public slots:
    void SLOT_URETTIGI_FISLER();

};

#endif // BORDRO_INCELEME_TREE_SECIMI_H
