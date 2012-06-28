#ifndef FAT_HIZMET_EKSTRESI_RAPORU_H
#define FAT_HIZMET_EKSTRESI_RAPORU_H

#include "report_kernel.h"

struct FATURA_RAPOR_STRUCT;

class FAT_HIZMET_EKSTRESI_RAPORU : public REPORT_KERNEL
{


public:
    FAT_HIZMET_EKSTRESI_RAPORU                                          ( QString rapor_ismi ,
                                                                          FATURA_RAPOR_STRUCT * P_FAT_RV);
    ~FAT_HIZMET_EKSTRESI_RAPORU                                         () {}

private:

    int                         m_max_line_count;
    int                         m_yazdirilan_satir_sayisi;
    int                         m_last_satir_indisi;
    QString                     m_rapor_kriterleri_1;
    QString                     m_rapor_kriterleri_2;
    bool                        m_tum_rapor_satirlari_basildi;
    int                         m_hizmete_ait_islemler_basildi;
    int                         m_footer_toplam;
    bool                        m_yazdirilacak_satir_bulundu;


    QStringList                 M_YAZDIRILACAK_SATIRLAR;
    QStringList                 M_HIZMETLER;

    FATURA_RAPOR_STRUCT   *     M_FAT_RV;

    QString             GET_HEADER              ();
    QString             GET_FOOTER              ();
    QString             GET_BODY                ();

    void                FILL_RAPOR_SATIRLARI();
    void                GET_HIZMET_ISLEMLERI( int hizmet_id , QString hizmet_kodu_adi);
    void                PRINT_NAKLI_YEKUN (int hizmet_id);

};

#endif // FAT_HIZMET_EKSTRESI_RAPORU_H
