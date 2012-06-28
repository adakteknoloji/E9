#ifndef YEVMIYE_DOKUMU_RAPORU_H
#define YEVMIYE_DOKUMU_RAPORU_H

#include "report_kernel.h"

class YEVMIYE_DOKUMU_RAPORU : public REPORT_KERNEL
{
public:
                                YEVMIYE_DOKUMU_RAPORU                       (double font_size ,double sayfa_genisligi,double sayfa_yuksekligi,struct MUH_RAPOR_VARS * MUH_RV);
                                ~YEVMIYE_DOKUMU_RAPORU                      (){}
private:
    struct FIS_STRUCT *         M_FISLER;
    struct KOK_HESAP_STRUCT *   M_KOK_HESAP_BUFFER;
    struct MUH_RAPOR_VARS *     M_MUH_RV;

    QString                     m_firma_ismi;

    QStringList                 M_BASILMIS_UST_HESAPLAR;
    QStringList                 M_MUH_HESAPLAR;

    int                         m_fis_sayisi;
    int                         m_max_line_count;
    int                         m_last_fis_indisi;
    int                         m_gonderilen_satir_sayisi;
    bool                        m_fis_satirlari_basildi;
    int                         m_fis_satiri_indisi;
    int                         m_temp_parent_id;

    double                      m_madde_borc_toplami;
    double                      m_madde_alacak_toplami;
    double                      m_footer_toplam_borc;
    double                      m_footer_toplam_alacak;
    double                      m_font_size;

    bool                        m_borc_satirlari_basildi;
    bool                        m_alacak_satirlari_basildi;
    bool                        m_tum_fisler_basildi;
    bool                        m_madde_toplami_basildi;
    bool                        m_raporlanacak_kayit_var_mi;

    QString                     GET_HEADER                                       ();
    QString                     GET_FOOTER                                       ();
    QString                     GET_BODY                                         ();

    bool                        FILL_STRUCT                                      ();
    int                         FIND_FIS_INDEX                                   ( int fis_id);

    QStringList                 GET_TUM_FISLER                                   ();
    QStringList                 GET_TUM_FIS_SATIRLARI                            ();

    void                        DEVIR_BAKIYESINI_YAZDIR                          ();
    void                        YEVMIYE_FISI_BASLIGINI_YAZDIR                    ( int yevmiye_no, QString fis_tarihi );
    void                        YEVMIYE_FISI_MADDE_TOPLAMINI_YAZDIR              ();

    void                        ANA_HESAPLARI_YAZDIR                             ( int parent_id,int fis_index,bool borc_satiri_mi );

    QString                     ANA_HESAPLARI_YAZDIR_VE_TALI_HESAP_KODUNU_AL     ( int     parent_id,
                                                                                   int     hesap_seviyesi,
                                                                                   QString tam_hesap_kodu,
                                                                                   QString hesap_ismi,
                                                                                   int     fis_index,
                                                                                   bool borc_satirimi);

};

#endif // YEVMIYE_DOKUMU_RAPORU_H
