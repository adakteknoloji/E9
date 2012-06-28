#ifndef CARI_MAKBUZ_BELGESI_BELGESI_H
#define CARI_MAKBUZ_BELGESI_BELGESI_H

#include "belge_kernel_class.h"


class CARI_MAKBUZ_BELGESI : public BELGE_KERNEL
{

public:
    CARI_MAKBUZ_BELGESI(int cari_fis_id);


private:
    int            m_fis_id;
    int            m_line_index;

    QString        m_doviz_kodu;

    //  HEADER
    double         m_toplam_tutar;
    QString        m_unvan;
    QString        m_tarih;
    QString        m_seri_sira;

    //  LINE
    QStringList    m_odeme_sekli;
    QStringList    m_aciklama;
    QStringList    m_tutar;
    QStringList    m_para_birimi;
    QStringList    m_banka;
    QStringList    m_sube;
    QStringList    m_cek_no;
    QStringList    m_cek_tarihi;


    //FOOTER
    QString        m_tahsil_eden;

    void           GET_HEADER_INFO                 ();
    void           GET_FOOTER_INFO                 ();
    void           GET_LINE_INFO                   ();


};

#endif // CARI_MAKBUZ_BELGESI_BELGESI_H
