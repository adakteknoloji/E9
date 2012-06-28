#ifndef ISL_FIS_UTILS_H
#define ISL_FIS_UTILS_H
#include <QString>

class ADAK_SQL;

struct ISL_FIS_STRUCT {
    int             fis_no;
    QString         fis_tarihi;
    int             fis_turu;
    QString         aciklama;

    double          kdv_haric_toplam;
    double          kdv_dahil_toplam;
    double          kdv_toplam;
    int             modul_id;
    int             program_id;
    int             base_fis_id;
    int             last_order_number;
};

struct ISL_FIS_SATIRI_STRUCT {
    int             fis_id;
    int             order_number;
    QString         belge_no;
    QString         aciklama;
    int             gider_turu;
    double          kdv_orani;
    double          satir_kdv_haric_tutar;
    double          satir_kdv_dahil_tutar;
    double          satir_kdv_tutari;

    int             modul_id;
    int             program_id;
    int             base_record_id;
};

int  ISL_FISI_EKLE                  ( ISL_FIS_STRUCT * P_FIS, ADAK_SQL * P_GELEN_DB = NULL);
int  ISL_FIS_SATIRI_EKLE            ( ISL_FIS_STRUCT * P_FIS, ISL_FIS_SATIRI_STRUCT * P_FIS_SATIRI , ADAK_SQL * P_GELEN_DB = NULL);
void ISL_FISI_GUNCELLE              ( ISL_FIS_STRUCT * P_FIS, int p_fis_id);
void ISL_FIS_SATIRI_GUNCELLE        ( ISL_FIS_STRUCT * P_FIS, ISL_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id);
void ISL_FISI_SIL                   ( ISL_FIS_STRUCT * P_FIS, int p_fis_id);
void ISL_FIS_SATIRI_SIL             ( ISL_FIS_STRUCT * P_FIS, int p_fis_satiri_id );
void ISL_FIS_BILGILERINI_OKU        ( ISL_FIS_STRUCT * P_FIS, int p_fis_id );
void ISL_FIS_SATIRI_BILGILERINI_OKU ( ISL_FIS_SATIRI_STRUCT * P_FIS_SATIRI , int p_fis_satiri_id);
void ISL_CLEAR_FIS_STRUCT           ( ISL_FIS_STRUCT * P_FIS);
void ISL_CLEAR_FIS_SATIRI_STRUCT    ( ISL_FIS_SATIRI_STRUCT * P_FIS_SATIRI);
int  ISL_GET_ENT_FIS_ID             ( int p_modul_id, int p_program_id, int p_base_fis_id, ADAK_SQL * P_GELEN_DB = NULL);
int  ISL_GET_ENT_FIS_SATIRI_ID      ( int p_modul_id, int p_program_id, int p_base_record_id);
void ISL_ENTEGRASYON_SATIRI_EKLE    ( ISL_FIS_STRUCT * P_FIS , ISL_FIS_SATIRI_STRUCT * P_FIS_SATIRI,int p_fis_satiri_id,ADAK_SQL * P_GELEN_DB = NULL);

#endif // ISL_FIS_UTILS_H















