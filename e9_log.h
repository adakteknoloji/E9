#ifndef E9_LOG_H
#define E9_LOG_H

class QString;

enum LOG_ISLEMLERI {
    LOG_ISLEM_ADD                = 100,
    LOG_ISLEM_DELETE             = 200,
    LOG_ISLEM_UPDATE             = 300,
    LOG_ISLEM_RAPOR_ALINDI       = 400,
    LOG_ISLEM_KDV_AYIRMA         = 500,

};



enum LOG_TURLERI {
    LOG_E9_YETKILENDIRME            = 1,
    LOG_E9_ONAR                     = 2,
    LOG_E9_DEVIR                    = 3,
    LOG_E9_SABIT_DEGERLER           = 4,
    LOG_E9_KDV_OTV_ORANLARI         = 5,


    LOG_MUH_FISLER                  = 10,
    LOG_MUH_HESAP_PLANI             = 11,
    LOG_MUH_HESAP_GRUPLARI          = 12,
    LOG_MUH_ENT_HESAPLARI           = 13,
    LOG_MUH_FIS_KOPYALAMA           = 14,
    LOG_MUH_YVM_NO_OLUSTUR          = 15,
    LOG_MUH_RAPORLAR                = 16,


    LOG_ADR_SAHIS_KAYDI             = 20,
    LOG_ADR_FIRMA_KAYDI             = 21,
    LOG_ADR_ADRES_GRUP_EKLEME       = 22,
    LOG_ADR_ADRES_GRUPLARI          = 23,
    LOG_ADR_FIRMA_YETKILI_KAYDI     = 24,
    LOG_ADR_YAKIN_BILGILERI_KAYDI   = 25,
    LOG_ADR_KULLANICI_YETKILERI     = 26,
    LOG_ADR_ETIKET_YAZDIRMA         = 27,
    LOG_ADR_MAIL_GONDERIMI          = 28,

    LOG_CARI_FISLER                 = 30,
    LOG_CARI_HESAPLAR               = 31,
    LOG_CARI_GRUPLAR                = 32,
    LOG_CARI_RAPORLAR               = 33,

    LOG_BNK_FISLER                  = 40,
    LOG_BNK_HESAPLAR                = 41,
    LOG_BNK_ISLEMLER                = 42,
    LOG_BNK_POS_ISLEMLERI           = 43,
    LOG_BNK_KREDI_KARTLARI          = 44,
    LOG_BNK_RAPORLAR                = 45,

    LOG_CEK_BORDROSU                = 50,
    LOG_CEK_SENETLER                = 51,
    LOG_CEK_RAPORLAR                = 52,

    LOG_ISL_FISLER                  = 60,
    LOG_ISL_FIS_KOPYALAMA           = 61,
    LOG_ISL_RAPORLAR                = 62,

    LOG_DMR_AMORTISMAN_AYIRMA       = 70,
    LOG_DMR_FISLER                  = 71,
    LOG_DMR_DEMIRBASLAR             = 72,
    LOG_DMR_YENIDEN_DEGERLEME       = 73,
    LOG_DMR_RAPORLAR                = 74,

    LOG_FAT_FATURALAR               = 80,
    LOG_FAT_HIZMETLER               = 81,
    LOG_FAT_TOPLU_FATURALASTIRMA    = 82,

    LOG_STK_FISLER                  = 90,
    LOG_STK_URUNLER                 = 91,
    LOG_STK_URUN_BIRIMLERI          = 92,

    LOG_IRS_FISLER                  = 100,

    LOG_SUBE_DEPOLAR                = 110,
    LOG_SUBE_SUBELER                = 111,
    LOG_SUBE_ENT_HESAPLARI          = 112,
    LOG_SUBE_UNITELER               = 113,

    LOG_PRS_PERSONEL                = 121,
    LOG_PRS_SABIT_DEGERLER          = 122,
    LOG_PRS_BORDROLAR               = 123,
    LOG_PRS_IBRANAME_MADDELERI      = 124,
    LOG_PRS_RAPORLAR                = 125,

};

void        E9_LOG_KAYDI_EKLE                           ( int modul_id ,int log_turu , int log_islemi , QString log_detaylari);

QString     E9_GET_LOG_TURLERI_STRING                   ( int p_log_turu_enum );

int         E9_GET_LOG_TURLERI_ENUM                     ( QString log_turu );

QString     E9_GET_LOG_ISLEMLERI_STRING                 ( int p_e9_log_islemi_enum );


#endif // E9_LOG_H
