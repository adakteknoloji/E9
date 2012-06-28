#ifndef BANKA_STRUCT_H
#define BANKA_STRUCT_H

#include <QDate>

struct BNK_RAPOR_VARS {
    int         banka_hesap_no_id;
    int         pos_id;
    int         nakli_yekun;
    QDate       baslangic_tarihi;
    QDate       bitis_tarihi;
    int         kredi_karti_id;
    int         entegre_filtresi;
};

struct BNK_BANKA_BILGILERI_STRUCT {

    QString banka_hesap_kodu;
    QString banka_ismi;
    QString sube_kodu;
    QString sube_ismi;
    QString hesap_numarasi;
    int     doviz_id;

};
struct BNK_MUH_HESAPLARI_STRUCT {
    int verilen_cekler_hesap_id;
};

#endif // BANKA_STRUCT_H
