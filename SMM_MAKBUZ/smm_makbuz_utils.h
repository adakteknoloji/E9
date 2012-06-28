#ifndef SM_MAKBUZ_FIS_UTILS_H
#define SM_MAKBUZ_FIS_UTILS_H

#include <QString>

class ADAK_SQL;

enum ENTEGRASYON_SATIRLARI {

    CARI_ENT_SATIRI      = -1,
    MUH_ENT_SATIRI       = -2,
    BNK_ENT_SATIRI       = -3,
    CARI_TAH_ODN_SATIRI  = -4
};


struct SMM_MAKBUZ_STRUCT {
    int         makbuz_no;
    QString     makbuz_tarihi;
    int         cari_hesap_id;
    QString     cari_hesap_adresi;
    QString     vergi_dairesi;
    QString     vergi_numarasi;
    QString     sm_sicil_numarasi;
    int         modul_id;
    int         program_id;
    int         base_record_id;
    double      hizmet_tutari;
    int         makbuz_turu;
    int         muh_kasa_hesap_id;
    int         tevkifatli_mi;
    double      tevkifat_yuzdesi;
    QString     aciklama;
    int         muh_fis_id;
    QString     belge_seri;
    QString     belge_numarasi;
    int         tahsil_edildi_odendi;
    double      kdv_tutari;
    double      tevkifat_tutari;
    double      brut_tutar;
    int         kdv_tevkifat_muh_hesap_id;
    double      kdv_orani;
    double      vergi_orani;
    int         kdv_hesap_id;
    double      vergi_stopaj_tutari;
    double      net_tutar;
    int         hizmet_id;
    int         tah_odm_sekli;
    int         hesap_no_id;
    int         kredi_karti_id;
    int         pos_id;
};




int         SMM_MAKBUZ_EKLE                                        ( SMM_MAKBUZ_STRUCT * P_MAKBUZ);

void        SMM_MAKBUZ_GUNCELLE                                    ( SMM_MAKBUZ_STRUCT * P_MAKBUZ , int p_makbuz_id);

void        SMM_MAKBUZ_SIL                                         ( SMM_MAKBUZ_STRUCT * P_MAKBUZ , int p_makbuz_id);


void        SMM_MAKBUZ_BILGILERINI_OKU                             ( SMM_MAKBUZ_STRUCT * P_MAKBUZ , int p_makbuz_id,ADAK_SQL * P_GELEN_DB = NULL);

void        SMM_CLEAR_MAKBUZ_STRUCT                                ( SMM_MAKBUZ_STRUCT * P_MAKBUZ);

void        SMM_ODENDI_TAHSIL_EDILDI_ENT_FISLERI_OLUSTUR           ( SMM_MAKBUZ_STRUCT * P_MAKBUZ ,
                                                                     int p_makbuz_id);

void        SMM_ENT_FISLERI_OLUSTUR                                ( SMM_MAKBUZ_STRUCT * P_MAKBUZ ,
                                                                     int p_makbuz_id , ADAK_SQL * P_GELEN_DB = NULL);
void        SMM_ENT_FISLERI_SIL                                    ( SMM_MAKBUZ_STRUCT * P_MAKBUZ ,
                                                                   int p_makbuz_id , int hesap_no_id );



#endif // SM_MAKBUZ_FIS_UTILS_H
