#ifndef MUH_FIS_UTILS_H
#define MUH_FIS_UTILS_H

class ADAK_SQL;
//class QString;
//class QStringList;
#include <QString>
#include <QStringList>

struct MUH_FIS_STRUCT {
    int        fis_no;
    QString    fis_tarihi;
    QString    aciklama;
    int        kasa_hesabi_id;
    int        fis_turu;
    int        modul_id;
    int        program_id;
    double     toplam_borc;
    double     toplam_alacak;
    int        base_fis_id;
    int        yevmiye_no;
};

struct MUH_FIS_SATIRI_STRUCT {
    int        fis_id;
    int        hesap_id;
    QString    aciklama;
    double     borc_tutari;
    double     alacak_tutari;
    int        order_number;
    int        base_record_id;
    int        program_id;
    int        modul_id;
};


int             MUH_FIS_EKLE                                ( MUH_FIS_STRUCT * P_FIS , ADAK_SQL * P_GELEN_DB = NULL);

int             MUH_FIS_SATIRI_EKLE                         ( MUH_FIS_STRUCT * P_FIS , MUH_FIS_SATIRI_STRUCT * P_FIS_SATIRI , ADAK_SQL * P_GELEN_DB = NULL);

int             MUH_FIS_SATIRINI_SIL                        ( MUH_FIS_STRUCT * P_FIS , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);

void            MUH_FIS_SATIRINI_GUNCELLE                   ( MUH_FIS_STRUCT * P_FIS , MUH_FIS_SATIRI_STRUCT * P_FIS_SATIRI , int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB = NULL);

void            MUH_FIS_SIL                                 ( MUH_FIS_STRUCT * P_FIS , int p_fis_id);

void            MUH_FISI_GUNCELLE                           ( MUH_FIS_STRUCT * P_FIS , int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);

void            MUH_FIS_BILGILERINI_OKU                     ( MUH_FIS_STRUCT * P_FIS , int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);

void            MUH_FIS_SATIRINI_OKU                        ( MUH_FIS_SATIRI_STRUCT * P_FIS_SATIRI , int p_fis_satiri_id ,ADAK_SQL * P_GELEN_DB = NULL );

void            MUH_FIS_TUM_SATIRLARINI_SIL                 ( MUH_FIS_STRUCT * P_FIS, int p_fis_id ,ADAK_SQL * P_GELEN_DB = NULL);

void            MUH_CLEAR_FIS_STRUCT                        ( MUH_FIS_STRUCT * P_FIS );

void            MUH_CLEAR_FIS_SATIRI_STRUCT                 ( MUH_FIS_SATIRI_STRUCT * P_FIS_SATIRI );

void            MUH_FISINI_HATALI_FISE_DONUSTUR             ( MUH_FIS_STRUCT * P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB = NULL);

int             MUH_ENTEGRASYON_FIS_SATIRLARINI_SIL         ( MUH_FIS_STRUCT * P_FIS, int p_muh_fis_id, int p_sender_modul_line_id );

void            MUH_ENTEGRASYON_FIS_SATIRLARINI_GUNCELLE    ( QString p_aciklama, int p_program_id, int p_modul_id, int p_muh_fis_id, int p_base_fis_id, int p_base_record_id , ADAK_SQL * P_GELEN_DB = NULL);

int             MUH_GET_ENT_FIS_ID                          ( int program_id, int modul_id, int base_fis_id ,ADAK_SQL * P_GELEN_DB = NULL);

QStringList     MUH_GET_ENT_ROW_ID_LIST                     ( int program_id, int modul_id, int base_record_id , ADAK_SQL * P_GELEN_DB = NULL);


QStringList     MUH_GET_FISE_GORE_ENT_ROW_ID_LIST           ( int p_program_id , int p_modul_id , int p_base_record_id , int p_fis_id, ADAK_SQL * P_GELEN_DB = NULL);
int             MUH_GET_ENT_FIS_ID_FOR_BASE_RECORD_ID       ( int p_program_id, int p_modul_id, int p_base_record_id );

int             MUH_GET_ENT_FIS_ID_FOR_BASE_RECORD_ID       ( int p_program_id, int p_modul_id, int p_base_record_id );



#endif // MUH_FIS_UTILS_H
