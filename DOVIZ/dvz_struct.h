#ifndef DOVIZ_STRUCT_H
#define DOVIZ_STRUCT_H

#define DOVIZ_SAYISI 300

class QString;
class QDate;

struct DOVIZ_BILGILERI_STRUCT {
    QString doviz_kodu;
    QString doviz_adi;
    QString doviz_sembolu;
    double  alis_eklenecek_yuzde;
    double  satis_eklenecek_yuzde;
};


struct DOVIZ_LIST {
    int     doviz_id;
    QString doviz_kodu;
    QString doviz_adi;
};

struct DOVIZLER {
    int doviz_sayisi;
    DOVIZ_LIST  * DOVIZ_LISTESI;
};

struct doviz {
    int     doviz_id;
    bool    dovizi_kullan;
    QString doviz_kodu;
    QString doviz_adi;
    QString doviz_sembolu;
    double  alis_tutari;
    double  satis_tutari;
    double  efektif_alis_tutari;
    double  efektif_satis_tutari;
    double  kullanici_alis_tutari;
    double  kullanici_satis_tutari;
};

struct kur {
    QDate   tarih;
    int     doviz_sayisi;
    doviz   dovizler[DOVIZ_SAYISI];
};

#endif // DOVIZ_STRUCT_H
