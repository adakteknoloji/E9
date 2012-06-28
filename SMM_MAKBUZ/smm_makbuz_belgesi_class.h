#ifndef SMM_MAKBUZ_BELGESI_BELGESI_H
#define SMM_MAKBUZ_BELGESI_BELGESI_H

#include "belge_kernel_class.h"


class SMM_MAKBUZ_BELGESI : public BELGE_KERNEL
{

public:
    SMM_MAKBUZ_BELGESI(int cari_fis_id);


private:

    //  HEADER

    QString        m_unvan;
    QString        m_tarih;
    QString        m_belge_seri_sira;
    QString        m_adresi;
    QString        m_vergi_dairesi;
    QString        m_vergi_no;

    //  LINE
    QString        m_yapilan_hizmet;

    //FOOTER

    QString       m_brut_ucret;
    QString       m_stopaj;
    QString       m_net_ucret;
    QString       m_kdv;
    QString       m_hizmet_tutari;

    void           GET_HEADER_INFO                 ();
    void           GET_FOOTER_INFO                 ();
    void           GET_LINE_INFO                   ();


};

#endif // SMM_MAKBUZ_BELGESI_BELGESI_H
