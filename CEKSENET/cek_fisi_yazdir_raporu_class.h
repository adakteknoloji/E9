#ifndef CEK_FISI_YAZDIR_RAPORU_H
#define CEK_FISI_YAZDIR_RAPORU_H

#include "report_kernel.h"

class CEK_FISI_YAZDIR_RAPORU : public REPORT_KERNEL
{


public:

            CEK_FISI_YAZDIR_RAPORU                                             ( int bordro_id, int bordro_islem_turu );
            ~CEK_FISI_YAZDIR_RAPORU                                            () {}


private:

    struct REPORT_HEADER_INFO {
        QString fis_tarihi;
        QString aciklama;
        QString islem_turu;
        QString bordro_no;
        QString cari_hesap_kodu;
        QString cari_hesap_ismi;
        QString banka_ismi;
        QString sube_kodu;
        QString sube_ismi;
        QString hesap_numarasi;
        QString banka_kodu;
    };

    REPORT_HEADER_INFO P_REPORT_HEADER_INFO;


    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    int                         m_bordro_id;
    QStringList                 m_bordro_satirlari_list;
    int                         m_last_satir_indisi;
    double                      m_toplam_cek_tutari;
    int                         m_bordro_islem_turu;

    QString                     GET_HEADER                          ();
    QString                     GET_FOOTER                          ();
    QString                     GET_BODY                            ();


    void                        FILL_HEADER_STRUCT                  ();
    void                        GET_BORDRO_SATIRLARI                ();
};

#endif // CEK_FISI_YAZDIR_RAPORU_H
