#ifndef PRK_FISI_YAZDIR_RAPORU_CLASS_H
#define PRK_FISI_YAZDIR_RAPORU_CLASS_H

#include "report_kernel.h"

class PRK_FISI_YAZDIR_RAPORU : public REPORT_KERNEL
{


public:
    PRK_FISI_YAZDIR_RAPORU                                          (QString rapor_ismi , int fis_id);
    ~PRK_FISI_YAZDIR_RAPORU                                         () {}

private:

    int                         m_max_line_count;


    int                         m_fis_id;
    int                         m_last_satir_indisi;
    bool                        m_fis_yazdirildi;

    double                      m_kdv_haric_toplam;
    double                      m_kdv_dahil_toplam;
    double                      m_kdv_toplam;

    QStringList                 m_fis_header_bilgileri;
    QStringList                 M_FIS_SATIRLARI_LIST;

    QString             GET_HEADER              ();
    QString             GET_FOOTER              ();
    QString             GET_BODY                ();

    void                FILL_FIS_SATIRLARI_LIST ();

};

#endif // PRK_FISI_YAZDIR_RAPORU_H
