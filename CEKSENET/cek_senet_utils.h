#ifndef CEK_SENET_UTILS_H
#define CEK_SENET_UTILS_H

class QString;

struct CEK_SENET_STRUCT {

    int         portfoy_numarasi;
    QString     banka_ismi;
    QString     sube_kodu;
    QString     sube_ismi;
    int         hesap_no_id;
    QString     cek_senet_numarasi;
    QString     vade_tarihi;
    QString     keside_tarihi;
    QString     borclu_kesideci;
    int         musteri_ceki_mi;
    double      cek_senet_tutari;
    QString     musteri_hesap_numarasi;
    int         cek_senet_turu;
    int         doviz_id;
    int         cek_senet_son_durumu;
    int         cek_senet_id;
};

int  CEK_SENET_EKLE         ( CEK_SENET_STRUCT * P_CEK_SENET );
void CEK_SENET_SIL          ( int p_cek_senet_id             );
void CEK_SENET_GUNCELLE     ( CEK_SENET_STRUCT * P_CEK_SENET , int p_cek_senet_id);

int  CEK_GET_TEMEL_BILGILER ( CEK_SENET_STRUCT * P_CEK, int p_cek_id  );


#endif // CEK_SENET_UTILS_H
