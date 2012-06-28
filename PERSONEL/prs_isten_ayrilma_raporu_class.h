#ifndef ISTEN_AYRILMA_RAPORU_CLASS_H
#define ISTEN_AYRILMA_RAPORU_CLASS_H

#include "report_kernel.h"

class ISTEN_AYRILMA_RAPORU : public REPORT_KERNEL
{
public:
                        ISTEN_AYRILMA_RAPORU        ( int personel_id, int isten_ayrilan_id );
                        ~ISTEN_AYRILMA_RAPORU       () {}

private:
    int                 m_personel_id;
    int                 m_isten_ayrilan_id;
    bool                m_rapor_bitti_mi;


protected:
    QString             GET_HEADER                  ();
    QString             GET_BODY                    ();
    QString             GET_FOOTER                  ();


};


#endif // ISTEN_AYRILMA_RAPORU_H
