#ifndef XYZ_RAPORU_CLASS_H
#define XYZ_RAPORU_CLASS_H

#include "report_kernel.h"

class XYZ_RAPORU : public REPORT_KERNEL
{


public:
    XYZ_RAPORU                                          ( QString rapor_ismi );
    ~XYZ_RAPORU                                         () {}

private:

    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;

    QString             GET_HEADER              ();
    QString             GET_FOOTER              ();
    QString             GET_BODY                ();

};

#endif // XYZ_RAPORU_CLASS_H
