#ifndef HESAP_PLANI_PAGE_H
#define HESAP_PLANI_PAGE_H

#include "report_kernel.h"

class HESAP_PLANI_RAPORU : public REPORT_KERNEL
{

public :

    HESAP_PLANI_RAPORU(struct MUH_RAPOR_VARS *  M_MUH_RV);
    ~HESAP_PLANI_RAPORU(){}


private :
    struct MUH_RAPOR_VARS *  M_MUH_RV;

    int         m_max_yazdirilabilir_satir_sayisi;
    int         m_toplam_hesap_sayisi;
    int         m_hesap_indisi;
    int         m_yazdirilan_kayit_sayisi;
    int         m_sayfa_no;


    QStringList M_MUHASEBE_HESAPLARI;

    void        GET_YAZDIRILACAK_MUHASEBE_HESAPLARI();

    QString     GET_HEADER                      ();
    QString     GET_FOOTER                      ();
    QString     GET_BODY                        ();

};

#endif // HESAP_PLANI_PAGE_H
