#ifndef PRS_ENUM_H
#define PRS_ENUM_H

#define         BELGE_TURLERI_SAYISI            17
#define         INDIRIM_KANUNU_SAYISI           4
#define         ISTEN_CIKMA_NEDENLERI_SAYISI    34
#define         EKSIK_GUN_NEDENLERI_SAYISI      19
#define         OZEL_DURUM_SAYISI               10
#define         GELIR_VERGISI_DILIMI_SAYISI     5
#define         MAX_COCUK_SAYISI                5
#define         IHBAR_TAZMINATI_DILIMI_SAYISI   4
#define         MAX_TEHLIKE_SINIFI_SAYISI       5

enum ZAMAN_TURU {
    PRS_HAFTA  = 0,
    PRS_AY     = 1,
    PRS_YIL    = 2
};

enum TUTAR_YUZDE_TURU {
    PRS_TUTAR  = 0,
    PRS_YUZDE  = 1
};


enum PERSONEL_FIS_ISLEM_TURU {
    PRS_MENU                 = 0,
    PRS_PERSONEL_SICIL_KARTI = 1
};

enum FIRMADA_ODENEK_VAR_MI_DURUMU {
    PRS_TUM_PERSONELE_UYGULANIR     = 0,
    PRS_BELIRLI_PERSONELE_UYGULANIR = 1,
    PRS_FIRMADA_UYGULANMAZ          = 2
};

enum UCRET_SEKLI {
    PRS_BRUT = 0,
    PRS_NET  = 1
};
enum CALISMA_SEKLI {
    ENUM_AYLIK  = 0,
    ENUM_GUNLUK = 1
};

enum TABI_OLDUGU_SIGORTA_TURU {
    PRS_SSK            = 0,
    PRS_EMEKLI_SANDIGI = 1,
    PRS_BAGKUR         = 2
};

enum MEDENI_HAL {
    PRS_EVLI     = 0,
    PRS_BEKAR    = 1,
    PRS_DUL      = 2,
    PRS_BOSANMIS = 3
};

enum OZEL_DURUM {
    PRS_OZURLU_DERECE_1 = 1,
    PRS_OZURLU_DERECE_2 = 2,
    PRS_OZURLU_DERECE_3 = 3,
    PRS_ESKI_HUKUMLU    = 4,
    PRS_TEROR_MAGDURU   = 5
};

enum AKRABALIK_DERECESI {
    PRS_ES    = 0,
    PRS_COCUK = 1,
    PRS_ANNE  = 2,
    PRS_BABA  = 3
};

enum CINSIYET {
    PRS_KADIN = 0,
    PRS_ERKEK = 1
};

enum EGITIM_DURUMU {
    PRS_YOK           = 0,
    PRS_ILKOKUL       = 1,
    PRS_ORTAOKUL      = 2,
    PRS_LISE          = 3,
    PRS_ONLISANS      = 4,
    PRS_LISANS        = 5,
    PRS_YUKSEK_LISANS = 6
};

enum COCUK_SAYISI {
    PRS_COCUK_YOK  = 0,
    PRS_TEK_COCUK  = 1,
    PRS_IKI_COCUK  = 2,
    PRS_UC_COCUK   = 3,
    PRS_DORT_COCUK = 4
};

enum CALISMA_DURUMU {
    PRS_CALISMIYOR = 0,
    PRS_CALISIYOR  = 1,
    PRS_OGRENCI    = 2
};



enum KESINTI_ODENEK_TURU {

    ENUM_PRS_KESINTI = 10,
    ENUM_PRS_ODENEK  = 11
};


enum EKSIK_GUN_NEDENLERI {
    PRS_ISTIRAHAT                           = 1, // 01  eger tek hane ise basina sifir ekliyoru
    PRS_DISIPLIN_CEZASI                     = 3,
    PRS_GOZALTINA_ALINMA                    = 4,
    PRS_TUTUKLULUK                          = 5,
    PRS_KISMI_ISTIHDAM                      = 6,
    PRS_PUANTAJ_KAYITLARI                   = 7,
    PRS_GREV                                = 8,
    PRS_LOKAVT                              = 9,
    PRS_GENEL_HAYATI_ETK_OLAYLAR            = 10,
    PRS_DOGAL_AFET                          = 11,
    PRS_BIRDEN_FAZLA                        = 12,
    PRS_DIGER                               = 13,
    PRS_DEVAMSIZLIK                         = 15,
    PRS_FESIH_TRHNDE_CALISMAMIS             = 16,
    PRS_EV_HZMTLRNDE_30_GUNDEN_AZ_CALISMA   = 17,
    PRS_KISA_CALISMA_ODENEGI                = 18,
    PRS_UCRETSIZ_DOGUM_IZNI                 = 19,
    PRS_UCRETSIZ_YOL_IZNI                   = 20,
    PRS_DIGER_UCRETSIZ_IZIN                 = 21

};

enum ISTEN_CIKMA_NEDENLERI {
    PRS_DENEME_SURELI_IS_SOZLESMESININ_ISVERENCE_FESHI                                  = 1,
    PRS_DENEME_SURELI_IS_SOZLESMESININ_ISCI_TARAFINDAN_FESHI                            = 2,
    PRS_BELIRSIZ_SURELI_IS_SOZLESMESININ_ISCI_TARAFINDAN_FESHI                          = 3,
    PRS_BELIRSIZ_SURELI_IS_SOZLESMESININ_ISVERENCE_FESHI                                = 4,
    PRS_BELIRLI_SURELI_IS_SOZLESMESININ_SONA_ERMESI                                     = 5,
    PRS_EMEKLILIK_VEYA_TOPTAN_ODEME                                                     = 8,
    PRS_MALULEN_EMEKLILIK                                                               = 9,
    PRS_OLUM                                                                            = 10,
    PRS_IS_KAZASI_SONUCU_OLUM                                                           = 11,
    PRS_ASKERLK                                                                         = 12,
    PRS_KADIN_ISCININ_EVLENMESI                                                         = 13,
    PRS_EMEKLILIK_ICIN_YAS_DISINDA_DGR_SARTLARIN_TAMAMLANMASI                           = 14,
    PRS_TOPLU_ISCI_CIKARMA                                                              = 15,
    PRS_SOZLESME_SONA_ERMEDEN_AYNI_ISVEREN_AIT_DIGER_ISYERINE_NAKIL                     = 16,
    PRS_ISYERININ_KAPANMASI                                                             = 17,
    PRS_ISIN_SONA_ERMESI                                                                = 18,
    PRS_MEVSIM_BITIMI                                                                   = 19,
    PRS_KAMPANYA_BITIMI                                                                 = 20,
    PRS_STATU_DEGISIKLIGI                                                               = 21,
    PRS_DIGER_NEDENLER                                                                  = 22,
    PRS_ISCI_TARAFINDAN_ZORUNLU_NEDENLE_FESIH                                           = 23,
    PRS_ISCI_TARAFINDAN_SAGLIK_NEDENIYLE_FESIH                                          = 24,
    PRS_ISCI_TARAFINDAN_ISVERENIN_AHLAK_IYINIYET_KURALLARINA_UYMAMASI_NEDENIYLE_FESIH   = 25,
    PRS_DISIPLIN_KURULU_KARARI_ILE_FESIH                                                = 26,
    PRS_ISVEREN_TARAFINDAN_ZORUNLU_NEDENLERLE_VE_TUTUKLULUK_NEDENIYLE_FESIH             = 27,
    PRS_ISVEREN_TARAFINDAN_SAGLIK_NEDENIYLE_FESIH                                       = 28,
    PRS_ISVEREN_TARAFINDAN_ISCININ_AHLAK_IYINIYET_KURALLARINA_UYMAMASI_NEDENIYLE_FESIH  = 29,
    PRS_VIZE_SURESININ_BITIMI                                                           = 30,
    PRS_BORCLAR_KANUNU_SENDIKALAR_KANUNU_GREV_LOKAVT_KANUNU_KAPSAMINDA_KENDI_ISTEK_KUSURU_DISINDA_FESIH = 31,
    PRS_OZELLESTIRME_NEDENI_ILE_4096_SAYILI_KANUNUN_21_MADDESINE_GORE_FESIH             = 32,
    PRS_GAZETECI_TARAFINDAN_SOZLESMENIN_FESHI                                           = 33,
    PRS_ISYERININ_DEVRI_ISIN_VEYA_ISYERININ_NITELIGININ_DEGISMESI_NEDENIYLE_FESIH       = 34
};

enum MUHASEBE_ENT_NASIL_YAPILSIN {
    ENUM_BORDRO_DETAYLARI   = 0,
    ENUM_BORDRO_TOPLAMLARI  = 1
};

#endif // PRS_ENUM_H
