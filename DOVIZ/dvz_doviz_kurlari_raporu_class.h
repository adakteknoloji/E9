#ifndef DVZ_DOVIZ_KURLARI_RAPORU_CLASS_H
#define DVZ_DOVIZ_KURLARI_RAPORU_CLASS_H

#include "report_kernel.h"

struct UYE_RAPOR_VARS;

class DVZ_KUR_RAPORU : public REPORT_KERNEL
{


public:
    DVZ_KUR_RAPORU    ( QString p_date );
    ~DVZ_KUR_RAPORU   () {}

private:

    QString             m_date;

    int                 m_max_line_count;
    bool                m_tum_satirlar_yazildimi;

    QStringList         M_SATIR_BILGILERI;

    QString             GET_HEADER              ();
    QString             GET_FOOTER              ();
    QString             GET_BODY                ();
    void                KUR_BILGILERINI_TOPLA   ();

};


#endif // DVZ_DOVIZ_KURLARI_RAPORU_CLASS_H
