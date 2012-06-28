#ifndef PRS_UCRET_PUSULASI_RAPORU_CLASS_H
#define PRS_UCRET_PUSULASI_RAPORU_CLASS_H

#include "report_kernel.h"
#include <QDate>

struct PRS_UCRET_PUSULASI_RAPORU_STRUCT;

class PRS_UCRET_PUSULASI_RAPORU : public REPORT_KERNEL
{


public:
    PRS_UCRET_PUSULASI_RAPORU                                          ( PRS_UCRET_PUSULASI_RAPORU_STRUCT * P_UCRET_RV , QString rapor_ismi );
    ~PRS_UCRET_PUSULASI_RAPORU                                         () {}

private:

    PRS_UCRET_PUSULASI_RAPORU_STRUCT *  M_UCRET_RV;
    int                                 m_max_line_count;
    int                                 m_eklenen_kayit_sayisi;
    QString                             m_rapor_kriterleri_1;
    QStringList                         M_RAPOR_SATIRLARI;
    QStringList                         M_RAPOR_BILGILER;
    QString                             m_baslangic_tarihi;
    QString                             m_bitis_tarihi;
    int                                 m_tum_rapor_satirlari_basildi;

    int                                 m_son_satir;
    QList < int >                       personel_list;


    QStringList                         M_KAZANC_BILGILERI;
    QStringList                         M_SOSYAL_YARDIM_BILGILERI;
    QStringList                         M_OZEL_KESINTILER_BILGILERI;
    QStringList                         M_YASAL_KESINTILER_BILGILERI;

    double                              toplam_gun;
    double                              toplam_kazanc;
    double                              toplam_sosyal_yardim;
    double                              toplam_ozel_kesintiler;
    double                              toplam_yasal_kesintiler;

    QString                             GET_HEADER              ();
    QString                             GET_FOOTER              ();
    QString                             GET_BODY                ();

    void                                FILL_RAPOR_SATIRLARI( int p_personel_id );
    void                                FILL_RAPOR_BILGILER ( int p_personel_id );

};



#endif // PRS_UCRET_PUSULASI_RAPORU_H
