#ifndef CEK_FIS_UTILS_H
#define CEK_FIS_UTILS_H

class ADAK_SQL;
class QString;
class QStringList;

struct SUBE_ENTEGRASYON_HESAPLARI_STRUCT;


struct CEK_BORDRO_STRUCT {
    int     program_id;
    int     modul_id;
    QString bordro_tarihi;
    int     bordro_no;
    int     bordro_islem_turu;
    int     bordro_turu;
    QString aciklama;
    int     cari_hesap_id;
    int     cari_para_birimi_id;
    int     hesap_no_id;
    double  toplam_tutar;
    int     cek_bordrosu_mu;
    int     bordro_para_birimi_id;
    double  doviz_kuru;
    double  parite;
    int     musteri_cek_senedi_mi;//Sadece cek acilis bordrosunda kullanilir
    int     base_fis_id;
    int     muh_fis_id;

    //! consructor
    CEK_BORDRO_STRUCT();
    void    CLEAR_BORDRO_STRUCT();
};


struct CEK_BORDRO_SATIRI_STRUCT {
    int            bordro_id;
    int            cek_senet_id;
    QString        aciklama;
    int            order_number;
    int            alinan_cari_hesap_id;
    int            verilen_cari_hesap_id;
    double         cek_tutari;
    double         temel_para_birimi_tutari;
    double         cari_para_birimi_tutari;
    int            modul_id;
    int            program_id;
    int            base_record_id;
};

enum CEK_SENET_ENTEGRASYON_TURU {
    ENUM_CARI_ENTEGRASYON      = 100,
    ENUM_BANKA_ENTEGRASYONU    = 200,
    ENUM_MUHASEBE_ENTEGRASYONU = 300
};


int     CEK_BORDROSU_EKLE            ( CEK_BORDRO_STRUCT * P_BORDRO  , ADAK_SQL * P_GELEN_DB = NULL);

int     CEK_BORDRO_SATIRI_EKLE       ( CEK_BORDRO_STRUCT * P_BORDRO , CEK_BORDRO_SATIRI_STRUCT * P_BORDRO_SATIRI , bool p_cek_hareketi_ekle,
                                                  bool p_base_fis_satiri_eklenecek = true,int p_fis_satiri_id = 0 , ADAK_SQL * P_GELEN_DB = NULL);

int     CEK_BORDRO_SATIRINI_SIL      ( CEK_BORDRO_STRUCT * P_BORDRO , int p_bordro_satiri_id );

void    CEK_BORDRO_SATIRINI_GUNCELLE ( CEK_BORDRO_STRUCT * P_BORDRO , CEK_BORDRO_SATIRI_STRUCT * P_BORDRO_SATIRI,
                                                  int p_bordro_satiri_id);

void    CEK_BORDRO_SIL               ( CEK_BORDRO_STRUCT * P_BORDRO , int p_bordro_id);
void    CEK_BORDRO_GUNCELLE          ( CEK_BORDRO_STRUCT * P_BORDRO , int p_bordro_id);
void    CEK_BORDRO_BILGILERI_OKU     ( CEK_BORDRO_STRUCT * P_BORDRO , int p_bordro_id , ADAK_SQL * P_GELEN_DB = NULL);
void    CEK_BORDRO_SATIRINI_OKU      ( CEK_BORDRO_SATIRI_STRUCT * P_BORDRO_SATIRI , int p_bordro_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);

//! ENTEGRASYON
bool    CEK_SENET_ENT_ICIN_MUH_HESAPLARINI_BELIRLE ( int bordro_satiri_id, int *borclu_hesap_id, int *alacakli_hesap_id, bool p_e9_onar_cagirdi, ADAK_SQL * P_GELEN_DB = NULL);
void    CEK_SENET_ENT_FISLERINI_OLUSTUR( CEK_BORDRO_STRUCT * P_BORDRO, CEK_BORDRO_SATIRI_STRUCT * P_BORDRO_SATIRI, int p_bordro_satiri_id , bool p_e9_onar_cagirdi, ADAK_SQL * P_GELEN_DB = NULL );
void    CEK_SENET_ENT_FIS_SATIRLARINI_SIL( CEK_BORDRO_STRUCT * P_BORDRO , int p_bordro_satiri_id);


int     CEK_GET_SENET_ENTEGRASYON_TURU ( int cek_senet_id, int modul_id, bool p_e9_onar_cagirdi = false);

void    CEK_CLEAR_BORDRO_SATIRI_STRUCT ( CEK_BORDRO_SATIRI_STRUCT * P_BORDRO_SATIRI);

QStringList CEK_GET_ENT_FIS_ID_LIST        ( int p_program_id, int p_modul_id, int p_base_fis_id , ADAK_SQL * P_GELEN_DB = NULL);

QStringList CEK_GET_ENT_ROW_ID_LIST        ( int p_program_id , int p_modul_id , int p_base_record_id , ADAK_SQL * P_GELEN_DB = NULL);

int         CEK_GET_BORDRO_ISLEM_TURUNE_GORE_ENTEGRASYON_TURU (int p_bordro_islem_turu , int p_modul_id);

void        CEK_ONAR_ICIN_CEK_DURUMLARINI_BUL( int p_cek_senet_id, int *p_son_durum, int *p_onceki_durum );

void        CEK_GET_CEK_SENET_SON_DURUMLARI ( int p_cek_senet_id, int *p_cek_senet_son_durum, int *p_cek_senet_onceki_durum );

#endif // CEK_FIS_UTILS_H
