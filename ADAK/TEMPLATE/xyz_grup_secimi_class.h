#ifndef XYZ_GRUP_SECIMI_CLASS_H
#define XYZ_GRUP_SECIMI_CLASS_H

#include <grup_secim_kernel.h>

class XYZ_GRUP_SECIMI : public GRUP_SECIM_KERNEL {
    Q_OBJECT

public:

    XYZ_GRUP_SECIMI                                ( int cagiran_modul, QWidget * parent = 0 );
    ~XYZ_GRUP_SECIMI                               ();

    //Eger ekrana button eklenmek istenirse tree secim kernel dan gelen fonksiyonlar kullanilabilir.
    // BUTTON_CLICKED gibi
    //void       BUTTON_CLICKED    ( QAbstractButton * button, TREE_ITEM * selected_item );

};

#endif // XYZ_GRUP_SECIMI_CLASS_H
