#ifndef IRSALIYE_STRUCT_H
#define IRSALIYE_STRUCT_H

class QDate;

struct IRSALIYE_RAPOR_VARS {
    int       cari_hesap_id;
    int       fatura_durumu;
    QDate     baslangic_tarihi;
    QDate     bitis_tarihi;
    int       depo_id;
    int       irsaliye_durumu;
};

#endif // IRSALIYE_STRUCT_H
