#ifndef BNK_FIS_UTILS_H
#define BNK_FIS_UTILS_H

class ADAK_SQL;
class QString;
class QStringList;


struct BNK_HAREKET_FIS_STRUCT {

    int         modul_id;
    int         program_id;
    int         base_fis_id;
    int         fis_no;
    QString     fis_tarihi;
    int         hesap_no_id;
    QString     aciklama;
    double      toplam_hesaba_giren;
    double      toplam_hesaptan_cikan;
    int         fis_turu;
    int         last_order_number;
    int         muh_fis_id;     // muhasebede olusan fis id
    int         base_fis_turu;
    int         def_fis_id;
    int         odm_emr_hesabi_etkilenecek_mi;
    int         para_birim_id;
    int         muh_kasa_hesap_id;
};

struct BNK_HAREKET_FIS_SATIRI_STRUCT {

    int         modul_id;
    int         program_id;
    int         base_record_id;
    int         fis_id;
    QString     aciklama;
    double      hesaba_giren;
    double      hesaptan_cikan;
    int         order_number;
    int         etkilenecek_hesap_turu; // *
    int         etkilenecek_hesap_id;   // *
    int         karsi_hesap_turu;       // *
    int         karsi_hesap_id;         // *
    double      doviz_kuru;
    double      parite;
    int         karsi_hesap_eklenecek_mi;
};

// Banka  Hareket Islemleri

int         BNK_HAREKET_FIS_EKLE                        ( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_id = 0,  bool base_fis_eklenecek = true,ADAK_SQL * P_GELEN_DB = NULL );
int         BNK_HAREKET_FIS_SATIRI_EKLE                 ( BNK_HAREKET_FIS_STRUCT * P_FIS, BNK_HAREKET_FIS_SATIRI_STRUCT * P_FIS_SATIRI,
                                                          int fis_satiri_id = 0, bool base_fis_satiri_eklenecek = true,ADAK_SQL * P_GELEN_DB = NULL );

void        BNK_HAREKET_FIS_SATIRI_SIL                  ( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_satiri_id  );
void        BNK_HAREKET_FISI_SIL                        ( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_id         );

void        BNK_HAREKET_FIS_BILGILERINI_OKU             ( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);
void        BNK_HAREKET_FIS_SATIR_BILGILERINI_OKU       ( BNK_HAREKET_FIS_SATIRI_STRUCT * P_FIS_SATIRI  , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);

void        BNK_HAREKET_FISI_GUNCELLE                   ( BNK_HAREKET_FIS_STRUCT * P_FIS, int p_fis_id );
int         BNK_HAREKET_FIS_SATIRINI_GUNCELLE           ( BNK_HAREKET_FIS_STRUCT * P_FIS, BNK_HAREKET_FIS_SATIRI_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id );

void        BNK_CLEAR_HAREKET_FIS_STRUCT                ( BNK_HAREKET_FIS_STRUCT * P_FIS );
void        BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT         ( BNK_HAREKET_FIS_SATIRI_STRUCT * P_FIS_SATIRI);


int         BNK_GET_ENT_FIS_ID                          ( int program_id, int modul_id, int base_fis_id , ADAK_SQL * P_GELEN_DB = NULL);

int         BNK_GET_ENT_FIS_ID                          ( int program_id, int modul_id, int base_fis_id, int p_hesap_no_id , ADAK_SQL * P_GELEN_DB = NULL);

QStringList BNK_GET_ENT_ROW_ID_LIST                     ( int program_id, int modul_id, int base_record_id , ADAK_SQL * P_GELEN_DB = NULL);

QStringList BNK_GET_FISE_GORE_ENT_ROW_ID_LIST           ( int p_program_id , int p_modul_id , int p_base_record_id,int p_fis_id );


#endif // BNK_FIS_UTILS_H
