#ifndef ISL_FIS_YAZDIR_CLASS_H
#define ISL_FIS_YAZDIR_CLASS_H

#include "report_kernel.h"

class ISL_FIS_YAZDIR : public REPORT_KERNEL
{


public:
    ISL_FIS_YAZDIR(int fis_id,int fis_turu);
    ~ISL_FIS_YAZDIR() {}

protected:

    QString             GET_HEADER                      ();
    QString             GET_FOOTER                      ();
    QString             GET_BODY                        ();

private:

    bool                p_rapor_bitti_mi;
    int                 p_max_line_count;
    int                 p_fis_id;
    int                 p_yazdirilan_sayfa_sayisi;
    int                 p_yazdirilan_satir_sayisi;
    int                 p_yazdirilan_satir_numarasi;
    int                 p_fis_turu;
    double              p_kdv_haric_tutar;
    double              p_kdv_tutari;
    double              p_toplam_tutar;
    QString             p_fis_bilgileri;
    QStringList         P_FIS_SATIRLARI;

    bool                FIS_BILGILERINI_AL              ();
    bool                FIS_SATIRLARINI_AL              ();

};

#endif // ISL_FIS_YAZDIR_H
