#ifndef CARI_ENUM_H
#define CARI_ENUM_H

enum CARI_MODULU_FIS_TURLERI {
     ENUM_CARI_TAHSILAT_FISI    = 11,
     ENUM_CARI_ODEME_FISI       = 44,
     ENUM_CARI_HAREKET_FISI     = 77,
     ENUM_CARI_ACILIS_FISI      = 88,
     ENUM_CARI_VIRMAN_FORMU     = 100
 };


enum    CARI_KART_TIPI {
    ENUM_CARI_ALICI             = 0,
    ENUM_CARI_SATICI            = 1,
    ENUM_CARI_PERSONEL          = 2,
    ENUM_FIRMA_ORTAGI           = 3
};

enum CARI_ENT_FIS_TURLERI {

    ENUM_CARI_BANKA_ENT_FISI            = 10,
    ENUM_CARI_CEK_SENET_ODEME_ENT_FISI  = 20,
    ENUM_CARI_CEK_SENET_ENT_FISI        = 30,
    ENUM_CARI_FATURA_ENT_FISI           = 40,
    ENUM_CARI_PERSONEL_ENT_FISI         = 50,
    ENUM_CARI_SMM_ENT_FISI              = 60,
    ENUM_CARI_DIGER_ENT_FISI            = 70

};

enum CARI_FIS_TURU {
    ENUM_CARI_FISI      = 10,
    ENUM_CARI_ENT_FISI  = 20
};

enum CARI_KART_ARAMA_ISLEM_TURU {
    CARI_KART_EXPORT_ISLEMI = 20,
    CARI_KART_ARAMA_ISLEMI  = 21,
    PRS_KARTI_EXPORT_ISLEMI = 22
};

#endif // CARI_ENUM_H
