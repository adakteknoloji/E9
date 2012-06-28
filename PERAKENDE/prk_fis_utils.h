#ifndef PRK_FIS_UTILS_H
#define PRK_FIS_UTILS_H

#include <QString>
#include <QList>

class ADAK_SQL;

struct PRK_FIS_STRUCT {

    int         modul_id;
    int         program_id;
    int         base_fis_id;
    int         muh_fis_id;
    int         fis_no;
    QString     fis_tarihi;
    QString     aciklama;
    int         sube_id;
    double      kdv_haric_toplam;
    double      kdv_toplam;
    double      kdv_dahil_toplam;
    int         gider_fisi_mi;
    int         muh_kasa_hesap_id;

};

struct PRK_FIS_SATIRI_STRUCT {

    int         modul_id;
    int         program_id;
    int         base_record_id;
    int         fis_id;
    QString     belge_no;
    QString     aciklama;
    double      kdv_orani;
    QList<double> kdv_tutari;
    double      kdv_haric_tutar;
    double      kdv_dahil_tutar;
    int         gelir_gider_turu_id;
    QList<int>  kdv_hesap_id;
    int         tah_odm_sekli;
    int         hesap_no_id;
    int         kredi_karti_id;
    int         pos_id;
    QString     kredi_karti_numarasi;
    QString     kredi_karti_sahibi;
    int         order_number;
    int         yeni_kdv_satiri_eklenecek;

    QList <double> kdv_oranlari_list;

    int         muh_gelir_gider_satiri_id;
    int         muh_kdv_satiri_id;
    int         muh_kasa_satiri_id;

};


int              PRK_FIS_EKLE                   ( PRK_FIS_STRUCT * P_FIS);
int              PRK_FIS_SATIRI_EKLE            ( PRK_FIS_STRUCT * P_FIS ,
                                                  PRK_FIS_SATIRI_STRUCT * P_FIS_SATIRI,QList<int>ayni_belge_numarali_satir_id_list,int fis_satiri_id = 0,
                                                  bool p_base_fis_satiri_eklenecek = true , ADAK_SQL * P_GELEN_DB = NULL
                                                  );

int              PRK_FIS_SATIRINI_SIL           ( PRK_FIS_STRUCT * P_FIS , int p_fis_satiri_id );

void             PRK_FIS_SIL                    ( PRK_FIS_STRUCT * P_FIS , int p_fis_id );

void             PRK_FIS_SATIRINI_GUNCELLE      ( PRK_FIS_STRUCT * P_FIS , PRK_FIS_SATIRI_STRUCT * P_FIS_SATIRI , int p_fis_satiri_id);

void             PRK_FIS_GUNCELLE               ( PRK_FIS_STRUCT * P_FIS , int p_fis_id );

void             PRK_FIS_BILGILERINI_OKU        ( PRK_FIS_STRUCT * P_FIS , int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);
void             PRK_FIS_SATIRI_BILGILERINI_OKU ( PRK_FIS_SATIRI_STRUCT * P_FIS_SATIRI ,
                                                  int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);

void             PRK_CLEAR_FIS_STRUCT           ( PRK_FIS_STRUCT * P_FIS );
void             PRK_CLEAR_FIS_SATIRI_STRUCT    ( PRK_FIS_SATIRI_STRUCT * P_FIS_SATIRI );


void             PRK_ENT_FISLERI_OLUSTUR        ( PRK_FIS_STRUCT * P_FIS ,
                                                  PRK_FIS_SATIRI_STRUCT * P_FIS_SATIRI,int fis_satiri_id ,
                                                  QList<int>ayni_belge_numarali_satir_id_list,ADAK_SQL * P_GELEN_DB = NULL);


QList <int>      PRK_GET_FIS_SATIRI_ID_LIST     ( int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);

QList <int>      PRK_GET_AYNI_BELGE_NUMARALI_SATIR_LIST_AND_ENTEGRE_ET (int fis_satiri_id , ADAK_SQL * P_GELEN_DB);






#endif // PRK_FIS_UTILS_H
