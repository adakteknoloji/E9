#ifndef FAT_BA_BS_FORMU_LISTESI_RAPORU_H
#define FAT_BA_BS_FORMU_LISTESI_RAPORU_H

#include "report_kernel.h"
#include <QDate>

struct FATURA_RAPOR_STRUCT;

class FAT_BA_BS_FORMU_LISTESI_RAPORU : public REPORT_KERNEL
{


public:
    FAT_BA_BS_FORMU_LISTESI_RAPORU                                          ( FATURA_RAPOR_STRUCT * P_FAT_RAPOR, QString rapor_ismi );
    ~FAT_BA_BS_FORMU_LISTESI_RAPORU                                         () {}

private:

    FATURA_RAPOR_STRUCT *       M_FAT_RV;
    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    QString                     m_rapor_kriterleri_1;
    QStringList                 M_RAPOR_SATIRLARI;
    QString                     m_baslangic_tarihi;
    QString                     m_bitis_tarihi;
    bool                        m_tum_rapor_satirlari_basildi;
    int                         m_last_satir_indisi;



    QString                     GET_HEADER              ();
    QString                     GET_FOOTER              ();
    QString                     GET_BODY                ();

    void                        FILL_RAPOR_SATIRLARI();

};

QString                         GET_FAT_BA_BS_FORMU_LISTESI_RAPORU ( FATURA_RAPOR_STRUCT * P_FAT_RV );

#endif // FAT_BA_BS_FORMU_LISTESI_RAPORU_H
