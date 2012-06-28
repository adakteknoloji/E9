#ifndef PRS_STRUCT_H
#define PRS_STRUCT_H

#include <QDate>

struct PRS_RAPOR_VARS {
    QDate         baslangic_tarihi;
    QDate         bitis_tarihi;
};

struct PRS_CALISMA_SURESI {
    int         year;
    int         month;
    int         day;
};

struct IHBAR_TAZMINATI {
    int           baslangic_tarihi_turu;
    double        baslangic_tarihi_orj_degeri;
    double        baslangic_tarihi_ay_degeri;
    int           bitis_tarihi_turu;
    double        bitis_tarihi_orj_degeri;
    double        bitis_tarihi_ay_degeri;
    double        tazminat_hafta_degeri;
};

struct PRS_SABITLER {
    QDate           sgk_aciklama_tarihi;
    double          issizlik_sig_isci_yuzdesi;
    double          issizlik_sig_isveren_yuzdesi;
    double          sakatlik_ind_derece_1_tutari;
    double          sakatlik_ind_derece_2_tutari;
    double          sakatlik_ind_derece_3_tutari;
    int             tehlike_sinifi;
    double          ssk_primi_isci_yuzdesi;
    double          ssk_primi_isveren_yuzdesi;
    double          ssk_taban_degeri;
    double          ssk_tavan_degeri;
    double          ssk_16_yas_alti_asgari_ucret;
    double          ssk_16_yas_ustu_asgari_ucret;
    double          damga_vergisi;
    double          GELIR_VERGISI_DILIMLERI_BASLANGIC_TUTARLARI [GELIR_VERGISI_DILIMI_SAYISI];
    double          GELIR_VERGISI_DILIMLERI_BITIS_TUTARLARI     [GELIR_VERGISI_DILIMI_SAYISI];
    double          GELIR_VERGISI_YUZDELERI                     [GELIR_VERGISI_DILIMI_SAYISI];
    int             gecerli_gelir_vergisi_dilimi_sayisi;
    double          EVLI_ES_CALISMIYOR_ASG_GEC_INDIRIMLERI  [MAX_COCUK_SAYISI];
    double          EVLI_ES_CALISIYOR_ASG_GEC_INDIRIMLERI   [MAX_COCUK_SAYISI];
    double          BEKAR_ASG_GEC_INDIRIMLERI               [MAX_COCUK_SAYISI];
    IHBAR_TAZMINATI IHBAR_TAZMINATI_TUTARLARI               [IHBAR_TAZMINATI_DILIMI_SAYISI];

    double          tehlike_sinifi_isci_yuzdesi;
    double          tehlike_sinifi_isveren_yuzdesi; // bu tehlike sinifi % yuzdesi eklenmemelidir tehlike sinifi secildiginde otomatik hesaplanacak

    double          ssk_isveren_taban_deger;
    double          ssk_isveren_tavan_deger;

    double          prs_sgdp_primi_isci_yuzdesi;
    double          prs_sgdp_primi_isveren_yuzdesi;
    int             muh_fis_hesaplari_birlestir;

    int             isy_5510_nolu_kanuna_tabi_mi;
    double          isy_5510_nolu_kanun_yuzdesi;

    double          haftalik_calisilan_gun_sayisi;
    double          gunluk_yemek_yardimi_yuzdesi;


};

struct PRS_ODENEK_STRUCT {
    int         odenek_id;
    QString     odenek_adi;
    QString     aciklama;
    int         var_mi_durumu;        // Dikkat !! Yalnizca firma odenekleri icin firmada odenegin uygulanip uygulanmadigini yada kimlere uygulandigini belirtmek icin kullanilir
    int         ek_odenek_mi;
    int         ssk_kesintisi_iceriyor_mu;
    int         gelir_vergisi_iceriyor_mu;
    int         damga_vergisi_iceriyor_mu;
    int         yuzde_mi;
    double      yuzde_orani;
    double      brut_tutari;
    double      ssk_kesinti_matrahi;
    double      ssk_kesinti_tutari;
    double      gelir_vergisi_matrahi;
    double      gelir_vergisi_tutari;
    double      damga_vergisi_matrahi;
    double      damga_vergisi_tutari;
    double      net_tutari;
};

struct PRS_KESINTI_STRUCT {
    int         kesinti_id;
    int         ek_kesinti_mi;
    QString     kesinti_adi;
    int         yuzde_mi;
    double      yuzde_orani;
    double      tutari;
    QString     aciklama;
};

struct PRS_BORDRO_STRUCT {
    int         bordro_id;
    int         bordro_donemi_ay;
    int         bordro_donemi_yil;
    double      toplam_brut_ucret;
    double      toplam_net_ucret;
    double      toplam_brut_odenek_tutari;
    double      toplam_net_odenek_tutari;
    double      toplam_kesinti_tutari;
    double      toplam_ssk_kesintisi_tutari;
    double      toplam_gelir_vergisi_tutari;
    double      toplam_damga_vergisi_tutari;
    int         personel_sayisi;
    struct      PRS_PERSONEL_STRUCT *  PERSONEL;
};

struct PRS_PERSONEL_STRUCT {
    int         personel_id;
    QString     tc_kimlik_no;
    QString     sicil_no;
    QString     personel_adi_soyadi;
    int         calisilan_gun_sayisi;
    double      brut_ucret;
    double      net_ucret;
    double      bordro_oncesi_kumulatif_vergi_matrahi;
    double      bordro_sonrasi_kumulatif_vergi_matrahi;
    double      gelir_vergi_matrahi;
    double      ssk_matrahi;
    double      ssk_primi_tutari;
    double      issizlik_primi_tutari;
    double *    gelir_vergisi_tutari;
    double      asg_gec_indirimi_tutari;
    double      damga_vergisi_tutari;
    int         odenek_sayisi;
    int         kesinti_sayisi;
    struct      PRS_ODENEK_STRUCT  * ODENEKLER;
    struct      PRS_KESINTI_STRUCT * KESINTILER;
    double      toplam_brut_odenek_tutari;
    double      toplam_net_odenek_tutari;
    double      toplam_kesinti_tutari;
    double      toplam_ozel_kesintiler_tutari;
    double      bordro_bir_ay_once_kalan_ek_odenek;// bordroda tutulan
    double      bordro_iki_ay_once_kalan_ek_odenek;
    double      bir_ay_once_kalan_ek_odenek;       // kartta tutulan
    double      iki_ay_once_kalan_ek_odenek;
    int         calisma_sekli; // Aylik Gunluk

    double      yemek_verilen_gun_sayisi; // bir ayda calistigi gun sayisi yemek parsini hesaplamak icin
    double      yemek_yardimi_tutari;
};

struct PRS_FIRMA_ODENEKLERI {
    PRS_ODENEK_STRUCT  DOGUM_YARDIMI;
    PRS_ODENEK_STRUCT  OLUM_YARDIMI;
    PRS_ODENEK_STRUCT  COCUK_YARDIMI;
    PRS_ODENEK_STRUCT  YAKACAK_YARDIMI;
    PRS_ODENEK_STRUCT  AILE_YARDIMI;
    PRS_ODENEK_STRUCT  YEMEK_PARASI;
    PRS_ODENEK_STRUCT  EVLENME_YARDIMI;
    PRS_ODENEK_STRUCT  GOREV_HARCIHARI;
    PRS_ODENEK_STRUCT  IHBAR_TAZMINATI;
    PRS_ODENEK_STRUCT  KIDEM_TAZMINATI;
};


struct PRS_KIDEM_IHBAR_ENT_STRUCT {

    int     kidem_ihbar_id;// isten_ayrilan_id  | base_fis id icin kullanilacak
    int     personel_id;
    QString tarih;
    double  kidem_tazminati_tutari;
    double  ihbar_tazminati_tutari;
    double  ihbar_taz_gelir_vergisi_tutari;
    double  ihbar_taz_damga_vergisi_tutari;
    double  kidem_taz_damga_vergisi_tutari;

    int     program_id;
    int     modul_id;
};


struct PRS_UCRET_PUSULASI_RAPORU_STRUCT {

    int     personel_id;
    QString cari_hesap_kodu;
    int     bordro_ay;
    int     bordro_yil;

    QList < int > personel_list;

    int     sube_id;
};

#endif // PRS_STRUCT_H
