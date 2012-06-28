#ifndef MIZAN_RAPORU_H
#define MIZAN_RAPORU_H

#include "report_kernel.h"




class MIZAN_RAPORU : public REPORT_KERNEL
{


public:
                                MIZAN_RAPORU                                ( QList<int> grup_idler,struct MUH_RAPOR_VARS *  M_MUH_RV );
                                ~MIZAN_RAPORU                               (){}

protected:

    QString                     GET_HEADER                                  ();
    QString                     GET_FOOTER                                  ();
    QString                     GET_BODY                                    ();

private:
    struct MUH_RAPOR_VARS *     M_MUH_RV;

    int                         m_max_line_count;
    int                         m_gonderilen_kayit_sayisi;
    int                         m_hesap_indisi;
    QList<int>                  m_hesap_idleri;


    double                      m_footer_toplam_borc;
    double                      m_footer_toplam_alacak;
    double                      m_footer_borc_bakiye;
    double                      m_footer_alacak_bakiye;
    int                         m_sayfa_numarasi;

    QString                     m_grup_adi;
    bool                        m_hesaplar_bitti_mi;

    QStringList                 M_MUH_HESAPLAR;
    QStringList                 M_YAZDIRILACAK_HESAPLAR;
    QStringList                 M_BASILMIS_UST_HESAPLAR;
    QStringList                 M_HESAP_GRUP_ID_LISTESI;


    void                        ORDER_MUHASEBE_HESAPLARI                        ();
    void                        YAZDIRILACAK_MUHASEBE_HESAPLARINI_BELIRLE       ();
    void                        UST_HESAPLARIN_BORC_ALACAGINI_HESAPLA           (QString tam_hesap_kodu,
                                                                                 double & toplam_borc, double &toplam_alacak);

    bool                        ILK_HESAP_IKINCI_HESABIN_ALT_HESABI_MI          ( QString       p_alt_hesap_tam_hesap_kodu,
                                                                                  const QString p_parent_hesap_tam_hesap_kodu );

};

#endif // MIZAN_RAPORU_H
