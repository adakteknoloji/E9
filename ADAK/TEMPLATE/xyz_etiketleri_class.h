#ifndef XYZ_ETIKETLERI_CLASS_H
#define XYZ_ETIKETLERI_CLASS_H

#include "etiket_kernel.h"

//       #define GRUP_SAYISI 20 ????????????????/

class XYZ_ETIKETLERI : public ETIKET_KERNEL
{


public:
    XYZ_ETIKETLERI ();

    QString ETIKET_TABLOLARINI_OLUSTUR ( int grup_id );

};

QString GET_XYZ_ETIKETLERI ( int grup_id );

#endif // XYZ_ETIKETLERI_CLASS_H
