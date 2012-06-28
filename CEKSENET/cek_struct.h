#ifndef CEKSENET_STRUCT_H
#define CEKSENET_STRUCT_H

class QDate;
class QString;

struct CEK_RAPOR_VARS {
    int         cek_senet_pozisyonu;
    int         cek_senet_turu;
    QDate       bas_vade_tarihi;
    QDate       bts_vade_tarihi;
    int         alinan_cari_hesap_id;
    int         verilen_cari_hesap_id;
    QString     banka_ismi;
    bool        tum_cari_hesaplar;
    bool        tum_bankalar;
    int         entegre_filtresi;
};



#endif // CEKSENET_STRUCT_H
