#ifndef ISLETME_ENUM_H
#define ISLETME_ENUM_H


enum ISLETME_DEFTERI_FIS_TURU {
    ENUM_GELIR_FISI = 0,
    ENUM_GIDER_FISI = 1
};

//- Satılan mal veya hizmet bedeli
//- Faiz gelirleri
//- İşletmeye kayıtlı sabit kıymetlerin satış tutarı

enum ISLETME_DEFTERI_GELIR_TURU {
    ENUM_MAL_SATISI         = 0,
    ENUM_HIZMET_SATISI      = 1,
    ENUM_AMORTISMAN_SATISI  = 2,
    ENUM_FAIZ_GELIRI        = 3,
    ENUM_YILSONU_STOGU      = 4
};

//- Önceki yıldan devreden mal tutarı varsa gider sayfasının aktarılan toplam satırına ilk kayıt olarak kaydedilir.
//- İş yeri kirası
//- Elektrik, su, telefon, ısıtma giderleri
//- Mal alış bedeli, nakliye, komisyon, hamaliye
//- İş yeri sigorta gideri
//- Personele ödenen ücret, sigorta primi işveren payı ve işsizlik sigortası işveren payı
//- Yıl sonlarında sabit kıymetler için amortismanlar
//- İşletme ile ilgili ödenen faiz tutarı

enum ISLETME_DEFTERI_GIDER_TURU {
    ENUM_GENEL_GIDERLER         = 0, // genel giderler
    ENUM_MAL_ALIMI              = 1,
    ENUM_ISYERI_KIRASI          = 2,
    ENUM_ISYERI_SIGORTA_GIDERI  = 3,
    ENUM_ISYERI_PERSONEL_GIDERI = 4,
    ENUM_ISYERI_FAIZ_GIDERI     = 5,
    ENUM_DONEM_BASI_STOGU       = 6,
    ENUM_YILSONU_AMORTISMANLAR  = 7
};

enum ISL_DEFTERI_RAPOR_SEKLI {

    ENUM_GELIRLER_GIDERLER            = 11,
    ENUM_ONCE_GELIRLER_SONRA_GIDERLER = 12
};


#endif // ISLETME_ENUM_H
