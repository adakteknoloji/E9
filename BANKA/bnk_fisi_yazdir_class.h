#ifndef BNK_FISI_YAZDIR_CLASS_H
#define BNK_FISI_YAZDIR_CLASS_H

#include "report_kernel.h"

class BNK_FISI_YAZDIR : public REPORT_KERNEL
{


public:
    BNK_FISI_YAZDIR (int fis_id,int fis_turu);
    ~BNK_FISI_YAZDIR() {}


private:

    struct HEADER_INFO {
        QString     fis_tarihi;
        QString     fis_no;
        QString     banka_ismi;
        QString     sube_kodu;
        QString     sube_ismi;
        QString     hesap_numarasi;
        QString     doviz_kodu;
        QString     aciklama;
    };

    HEADER_INFO                 M_HEADER_INFO;


    int                         m_max_line_count;
    int                         m_eklenen_kayit_sayisi;
    int                         m_fis_id;
    int                         m_fis_turu;
    QStringList                 m_banka_fis_satirlari;
    int                         m_last_satir_indisi;
    double                      m_toplam_hesaba_giren;
    double                      m_toplam_hesaptan_cikan;
    double                      m_toplam_pos_blokaj_tutari;
    double                      m_toplam_komisyon_gideri;

    QList<int>                  m_islem_ids;
    QList<QString>              m_islem_str;

    QString                     GET_HEADER                      ();
    QString                     GET_FOOTER                      ();
    QString                     GET_BODY                        ();


    void                        GET_BANKA_FIS_SATIRLARI         ();
    void                        FILL_HEADER_STRUCT              ();
};

#endif // BNK_FISI_YAZDIR_H
