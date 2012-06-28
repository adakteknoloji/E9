#ifndef TREE_ARAMA_CLASS_H
#define TREE_ARAMA_CLASS_H

#include "tree_arama_kernel.h"

namespace Ui {
    class TREE_ARAMA;
}

class TREE_ARAMA : public TREE_ARAMA_KERNEL
{
    Q_OBJECT

public:
    TREE_ARAMA(QWidget *parent = 0);
    ~TREE_ARAMA();
    
private:
    int *               m_secilen_id;
  
    int                 DOUBLE_CLICKED                       (TREE_ITEM * selected_item);
    void                ADD_CHILD_ITEMS                      (TREE_ITEM * selected_item);
    int                 CHECK_RUN                            ();
    int                 CHECK_VAR                            (QObject *object);
    void                SEARCH                               ();

    Ui::TREE_ARAMA *ui;
};

#endif // TREE_ARAMA_CLASS_H

