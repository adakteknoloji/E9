#ifndef MAAS_BORDROSU_RAPORU_CLASS_H
#define MAAS_BORDROSU_RAPORU_CLASS_H

#include "report_kernel.h"

class MAAS_BORDROSU_RAPORU : public REPORT_KERNEL
{

public:
    MAAS_BORDROSU_RAPORU ( int bordro_id, QString sicil_no, int ay, int yil );
    ~MAAS_BORDROSU_RAPORU() {}

protected:
    virtual QString             GET_HEADER                      ();
    virtual QString             GET_FOOTER                      ();
    virtual QString             GET_BODY                        ();

private:
    int                         m_max_line_count;
    int                         m_sayfa_no;
    int                         m_son_gonderilen_kayit_id;
    int                         m_toplam_bordro_sayisi;
    QString                     m_sicil_no;
    bool                        m_rapor_bitti;
    int                         m_ay;
    int                         m_yil;
    int                         m_bordro_id;

};

#endif // MAAS_BORDROSU_RAPORU_H
