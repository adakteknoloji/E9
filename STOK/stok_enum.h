#ifndef STOK_ENUM_H
#define STOK_ENUM_H


#define  URUN_BIRIMLERI_SAYISI      4   // Temel birim hariç 4 tane stok birimi tutulur.

enum STK_STOK_TAKIP_SEKLI {

    ENUM_BASIT_TAKIP         = 0,
    ENUM_PARTI_TAKIBI        = 1,
    ENUM_SERI_NO_ILE_TAKIP   = 2
};

enum STK_IRSALIYE_MI_STOK_FISI_MI {

    ENUM_IRSALIYE        =   0,
    ENUM_STOK_FISI       =   1
};

enum STK_STOK_FIS_TURLERI {

    ENUM_STK_SARF_FIRE_FISI                             =   0,
    ENUM_STK_IMALATA_SEVK_FISI                          =   1,
    ENUM_STK_IMALATTAN_DONUS_FISI                       =   2,
    ENUM_STK_STOK_ACILIS_FISI                           =   3,
    ENUM_IRS_NORMAL_GIRIS                               =   4,
    ENUM_STK_DEPO_TRANSFER_GIRIS                        =   5,
    ENUM_IRS_FASON_BASLANGIC_GIRIS                      =   6,
    ENUM_IRS_FASON_BITIS_GIRIS                          =   7,
    ENUM_IRS_KONSINYE_GIRIS                             =   8,
    ENUM_IRS_NORMAL_CIKIS                               =   9,
    ENUM_STK_DEPO_TRANSFER_CIKIS                        =   10,
    ENUM_IRS_FASON_BASLANGIC_CIKIS                      =   11,
    ENUM_IRS_FASON_BITIS_CIKIS                          =   12,
    ENUM_IRS_KONSINYE_CIKIS                             =   13,
    ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS          =   14,
    ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS         =   15,
    ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS           =   16,
    ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS          =   17,
    ENUM_IRS_HAKEDIS_SATIS                              =   18,
    ENUM_IRS_HAKEDIS_ALIS                               =   19

};



enum URUN_HIZMET_KART_TURU { //Urun aramada aranacak urunun turu icin kullanilir.
    ENUM_URUN_KARTI        = 0,
    ENUM_HIZMET_KARTI      = 1,
    ENUM_URUN_HIZMET_KARTI = 2
};

enum HIZMET_TURU {
    ENUM_ALINAN_HIZMET          = 11,
    ENUM_SATILAN_HIZMET         = 12,
    ENUM_ALINAN_SATILAN_HIZMET  = 13 // HEM ALINAN HEMDE SATILAN HIZMET
};

enum STOK_ALIS_SATIS_DURUMU {
    ENUM_STOK_ALIS             = 0,
    ENUM_STOK_SATIS            = 1,
    ENUM_STOK_ALIS_VE_SATIS    = 2
};


enum URUN_DEPO_DURMU {
    ENUM_URUN_DEPODA_DEGIL = 0,
    ENUM_URUN_DEPODA       = 1,
    ENUM_URUN_HER_IKISI    = 2
};

enum STOK_ARAMA_ISLEM_TURU {
    STOK_URUN_KARTI_EXPORT_ISLEMI = 0 ,
    STOK_URUN_ARAMA_ISLEMI  = 1
};

#endif // STOK_ENUM_H
