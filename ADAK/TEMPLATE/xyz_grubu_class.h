#ifndef XYZ_GRUBU_CLASS_H
#define XYZ_GRUBU_CLASS_H

#include "grup_kernel.h"

class XYZ_GRUBU : public GRUP_KERNEL {
    Q_OBJECT

public:

    XYZ_GRUBU                                ( QString delimeter, QWidget * parent = 0 );
    ~XYZ_GRUBU                               ();

};

#endif // XYZ_GRUBU_CLASS_H
