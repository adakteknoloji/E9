#ifndef ISLETME_STRUCT_H
#define ISLETME_STRUCT_H

#include <QDate>

class QString;

struct ISL_RAPOR_VARS {
    QDate           baslangic_tarihi;
    QDate           bitis_tarihi;
    bool            nakli_yekun;
    int             bas_gun_no;
    int             bts_gun_no;
    bool            ana_hesaplar;
    bool            alt_hesaplar;
    bool            ozet;
    int             rapor_sekli;
    int             sube_id;
};

#endif // ISLETME_STRUCT_H
