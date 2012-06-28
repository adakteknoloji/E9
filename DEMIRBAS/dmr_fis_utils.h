#ifndef DMR_FIS_UTILS_H
#define DMR_FIS_UTILS_H
#include <QString>

class ADAK_SQL;


struct DMR_FIS_STRUCT {

    int      fis_no;
    QString  fis_tarihi;
    QString  aciklama;
    int      fis_turu;
    int      modul_id;
    int      program_id;
    int      base_fis_id;
    double   degerleme_orani;
    double   dusulecek_amor_yuzdesi;
    double   toplam_tutar;
    double   top_birikmis_amor_tutari;
    int      muh_fis_id;
};

struct DMR_FIS_SATIRI_STRUCT {
    int      fis_id;
    int      demirbas_id;
    QString  aciklama;
    double   satir_tutari;
    double   birikmis_amor_tutari;
    double   yeni_birikmis_amor_tutari;
    int      order_number;
    int      base_record_id;
    int      modul_id;
    int      program_id;

};


int           DMR_FIS_EKLE ( DMR_FIS_STRUCT * P_FIS , ADAK_SQL * P_GELEN_DB = NULL);

int           DMR_FIS_SATIRI_EKLE ( DMR_FIS_STRUCT * P_FIS , DMR_FIS_SATIRI_STRUCT * P_FIS_SATIRI ,
                                    ADAK_SQL * P_GELEN_DB = NULL);

void          DMR_FIS_GUNCELLE ( DMR_FIS_STRUCT * P_FIS , int p_fis_id);

void          DMR_FIS_SATIRI_GUNCELLE ( DMR_FIS_STRUCT * P_FIS , DMR_FIS_SATIRI_STRUCT * P_FIS_SATIRI , int p_fis_satiri_id);

void          DMR_FIS_SIL (DMR_FIS_STRUCT * P_FIS , int p_fis);

void          DMR_FIS_SATIRI_SIL ( DMR_FIS_STRUCT * P_FIS , int p_fis_satiri_id);


void          DMR_CLEAR_FIS_STRUCT        ( DMR_FIS_STRUCT * P_FIS);
void          DMR_CLEAR_FIS_SATIRI_STRUCT ( DMR_FIS_SATIRI_STRUCT * P_FIS_SATIRI );

void          DMR_FIS_BILGILERINI_OKU     ( DMR_FIS_STRUCT * P_FIS , int p_fis_id ,ADAK_SQL * P_GELEN_DB = NULL);
void          DMR_FIS_SATIRINI_OKU        ( DMR_FIS_SATIRI_STRUCT * P_FIS_SATIRI , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);



#endif // DMR_FIS_UTILS_H
