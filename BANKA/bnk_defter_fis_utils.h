#ifndef BNK_DEFTER_FISI_H
#define BNK_DEFTER_FISI_H

class ADAK_SQL;
class QString;
class QStringList;


struct BNK_DEFTER_FIS_STRUCT {

    int         fis_no;
    QString     fis_tarihi;
    int         hesap_no_id;
    QString     aciklama;
    int         para_birim_id;
    double      toplam_hesaba_giren;
    double      toplam_hesaptan_cikan;
    int         fis_turu;
    int         last_order_number;
    int         modul_id;
    int         program_id;
    int         base_fis_id;
    int         base_fis_turu;
    int         muh_fis_id;
    int         odm_emr_hesabi_etkilenecek_mi;
};

struct BNK_DEFTER_FIS_SATIRI_STRUCT {
    int         fis_id;
    QString     aciklama;
    double      hesaba_giren;
    double      hesaptan_cikan;
    int         order_number;
    int         hesap_turu;
    int         hesap_id;
    double      temel_tutar;
    int         modul_id;
    int         program_id;
    int         base_record_id;
    double      doviz_kuru;
    double      parite;

};

// Banka  Hareket Islemleri

int         BNK_DEFTER_FIS_EKLE                        ( BNK_DEFTER_FIS_STRUCT * P_FIS , ADAK_SQL * P_GELEN_DB = NULL);
int         BNK_DEFTER_FIS_SATIRI_EKLE                 ( BNK_DEFTER_FIS_STRUCT * P_FIS, BNK_DEFTER_FIS_SATIRI_STRUCT * P_FIS_SATIRI ,
                                                         int fis_satiri_id = 0, bool base_fis_satiri_eklenecek = true,ADAK_SQL * P_GELEN_DB = NULL );

void        BNK_DEFTER_FISI_SIL                        ( BNK_DEFTER_FIS_STRUCT * P_FIS, int p_fis_id         );
void        BNK_DEFTER_FIS_SATIRI_SIL                  ( BNK_DEFTER_FIS_STRUCT * P_FIS, int p_fis_satiri_id ,ADAK_SQL * P_GELEN_DB = NULL );

void        BNK_DEFTER_FIS_BILGILERINI_OKU             ( BNK_DEFTER_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);
void        BNK_DEFTER_FIS_SATIR_BILGILERINI_OKU       ( BNK_DEFTER_FIS_SATIRI_STRUCT * P_FIS_SATIRI  , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);

void        BNK_DEFTER_FISI_GUNCELLE                   ( BNK_DEFTER_FIS_STRUCT * P_FIS, int p_fis_id );
int         BNK_DEFTER_FIS_SATIRINI_GUNCELLE           ( BNK_DEFTER_FIS_STRUCT * P_FIS, BNK_DEFTER_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);

void        CLEAR_BNK_DEFTER_FIS_STRUCT                ( BNK_DEFTER_FIS_STRUCT * P_FIS );
void        CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT         ( BNK_DEFTER_FIS_SATIRI_STRUCT * P_FIS_SATIRI);

int         GET_BNK_DEFTER_FIS_ID                      ( int program_id, int modul_id, int base_fis_id );

int         GET_BNK_DEFTER_MUH_FIS_ID                  ( int p_program_id, int p_modul_id, int p_fis_id );

QStringList GET_BNK_DEFTER_ROW_ID_LIST                 ( int program_id, int modul_id, int base_record_id ,ADAK_SQL * P_GELEN_DB  = NULL);
QStringList BNK_GET_FISE_GORE_BNK_DEFTER_ROW_ID_LIST   ( int p_program_id , int p_modul_id , int p_base_record_id , int p_fis_id);

QStringList GET_BNK_DEFTER_FIS_ROW_ID_LIST             ( int p_program_id, int p_modul_id, int p_fis_id );


#endif // BNK_DEFTER_FISI_H
