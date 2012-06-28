#ifndef CARI_FISI_YAZDIR_RAPORU_H
#define CARI_FISI_YAZDIR_RAPORU_H

#include "report_kernel.h"

class CARI_FISI_YAZDIR_RAPORU : public REPORT_KERNEL
{


public:
                            CARI_FISI_YAZDIR_RAPORU                 ( int fis_id, int fis_turu );
                            ~CARI_FISI_YAZDIR_RAPORU                () {}


private:

    struct REPORT_HEADER_INFO {
        QString fis_tarihi;
        QString fis_no;
        QString aciklama;
    };

    REPORT_HEADER_INFO          M_REPORT_HEADER_INFO;

    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    int                         m_fis_id;
    int                         m_fis_turu;
    QStringList                 m_fis_satirlari_list;
    int                         m_last_satir_indisi;
    double                      m_footer_toplam_borc;
    double                      m_footer_toplam_alacak;

    QString                     GET_HEADER                      ();
    QString                     GET_FOOTER                      ();
    QString                     GET_BODY                        ();

    void                        GET_CARI_FIS_SATIRLARI          ();
    void                        FILL_HEADER_STRUCT              ();

};

#endif // CARI_FISI_YAZDIR_RAPORU_H
