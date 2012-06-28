#ifndef ADAK_DEFINES_H
#define ADAK_DEFINES_H
#include <QString>


#define EQ  ==

/*
 *  C/C++ programciligi esnasinda cogu zaman en zor tespit edilen hata "==" yerine "=" yazilmis olmasidir.
 *
 *  En iyi onlemlerden biri == yerine daima EQ kulanmaktir.
 *
 *  if (a == b) yerine if (a EQ b) risklerin cogunu engeller.
 */


#define NE  !=

/*
 *  EQ gibi != yerine de NE kullanmak daha iyi bir yontem gibi gorunuyor
 */

#define AND &&

/*
 *  EQ gibi && yerine de AND kullanmak daha iyi bir yontem gibi gorunuyor
 */

#define OR   ||
/*
 *  AND gibi || yerine de OR kullanmak daha iyi bir yontem gibi gorunuyor
 */



#define MOD  %
/*
 *  % yerine MOD kullanmak daha iyi bir yontem gibi gorunuyor
 */


enum ADAK_MSG_ENUM {
    ADAK_CANCEL = 0,
    ADAK_YES    = 1,
    ADAK_NO     = 2,
};

enum ADAK_PROGRAM_ENUM {
    YONETIM_libADAK    =  0,
    ADAK_DESIGNER      =  1,
    E9_PROGRAMI        =  2,
    TEST_PROGRAMI      = 99
};

enum ADAK_ADRES_CARI_ENUM {
    SAHIS_KAYDI         = 1,
    FIRMA_KAYDI         = 2,
    SAHIS_ARTI_FIRMA    = 3
};

enum ADAK_MODULES {
    MUHASEBE_MODULU        = 50,
    ISLETME_MODULU         = 45,
    DOVIZ_MODULU           = 35,
    ADRES_MODULU           = 10,

    CARI_MODULU            = 20,
    CEKSENET_MODULU        = 25,
    BANKA_MODULU           = 15,
    SUBE_MODULU            = 75,

    FATURA_MODULU          = 70,
    IRSALIYE_MODULU        = 65,
    STOK_MODULU            = 60,
    PERAKENDE_MODULU       = 80,

    DEMIRBAS_MODULU        = 30,
    DEMIRBAS_GRUP_MODULU   = 33,
    PERSONEL_MODULU        = 55,
    FAIZ_MODULU            = 40,

    SMM_MODULU             = 90,

    E9_YONETIM             = 200
};

enum SSL_PROTOCOL {
    NO_SSL   = 0,
    SSLV2    = 10,
    SSLV3    = 20,
    TLSV1    = 30,
    STARTTLS = 40
};

enum RETURN_VALUE {
    ADAK_OK         = -1,
    ADAK_FAIL       = -2,
    ADAK_CONTINUE   = -3,
    ADAK_EXIT       = -4,
    ADAK_NEW_RECORD = -5,
    ADAK_FAIL_UNDO  = -6
};

enum ADAK_EVENTS {
    ADAK_REFRESH    = -101
};

#endif // ADAK_DEFINES_H
