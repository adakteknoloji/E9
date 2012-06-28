#ifndef DEMIRBAS_STRUCT_H
#define DEMIRBAS_STRUCT_H

#include <QString>

struct DMR_TEMEL_BILGILER_STRUCT {
    int     muh_hesap_id;
    int     bir_amor_hesap_id;
    double  demirbasin_yeni_degeri;
    int     dmr_satis_kar_hesap_id;
    int     dmr_satis_zarar_hesap_id;
};

struct DMR_RAPOR_VARS {

    QList <int> grup_id_list;
    int         sube_id;
    QString     bas_dmr_kodu;
    QString     bts_dmr_kodu;
};

#endif // DEMIRBAS_STRUCT_H
