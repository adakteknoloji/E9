#ifndef PERSONEL_RAPORU_CLASS_H
#define PERSONEL_RAPORU_CLASS_H

#include "report_kernel.h"

class PERSONEL_RAPORU : public REPORT_KERNEL
{


public:
    PERSONEL_RAPORU( int sube_id );
    ~PERSONEL_RAPORU() {}

protected:
    virtual QString             GET_HEADER                      ();
    virtual QString             GET_FOOTER                      ();
    virtual QString             GET_BODY                        ();

private:
    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    bool                        m_rapor_bitti;
    int                         m_sube_id;

};

#endif // PERSONEL_RAPORU_H
