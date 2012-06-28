#ifndef E9_ONAR_UTILS_H
#define E9_ONAR_UTILS_H
#include <stdio.h>

class ADAK_SQL;
class QString;



void    MUH_ENT_FISLERINI_SIL  ( int p_program_id , int p_modul_id  ,ADAK_SQL * P_GELEN_DB = NULL);

void    CARI_ENT_FISLERINI_SIL ( int p_program_id , int p_modul_id  ,ADAK_SQL * P_GELEN_DB = NULL);

void    CEK_ENT_FISLERINI_SIL  ( int p_prograam_id , int p_modul_id ,ADAK_SQL * P_GELEN_DB = NULL);

void    FAT_ENT_FISLERINI_SIL  ( int p_program_id  , int p_modul_id ,ADAK_SQL * P_GELEN_DB = NULL);

void    PRK_ENT_FISLERINI_SIL  ( int p_program_id  , int p_modul_id ,ADAK_SQL * P_GELEN_DB = NULL);

void    BNK_ENT_FISLERINI_SIL  ( int p_program_id , int p_modul_id  ,ADAK_SQL * P_GELEN_DB = NULL);

void    STOK_ENT_FISLERINI_SIL ( int p_program_id , int p_modul_id  ,ADAK_SQL * P_GELEN_DB = NULL);

void    ISL_ENT_FISLERINI_SIL  ( int p_program_id , int p_modul_id  ,ADAK_SQL * P_GELEN_DB = NULL);


void    CARI_ENT_FISLERINI_OLUSTUR                      (int p_program_id , ADAK_SQL * P_GELEN_DB = NULL);

void    CEK_ENT_FISLERINI_OLUSTUR                       (int p_program_id , ADAK_SQL * P_GELEN_DB = NULL);

void    FAT_ENT_FISLERINI_OLUSTUR                       (int p_program_id , ADAK_SQL * P_GELEN_DB = NULL );

void    PRK_ENT_FISLERINI_OLUSTUR                       (int p_program_id , ADAK_SQL * P_GELEN_DB = NULL);

void    BNK_ENT_FISLERINI_OLUSTUR                       ( int p_program_id , ADAK_SQL * P_GELEN_DB = NULL);

void    PRS_ENT_FISLERINI_OLUSTUR                       ( int p_program_id , ADAK_SQL * P_GELEN_DB = NULL);

void    STK_ENT_FISLERINI_OLUSTUR                       ( int p_program_id , ADAK_SQL * P_GELEN_DB = NULL);

void    DMR_ENT_FISLERINI_OLUSTUR                       ( int p_program_id , ADAK_SQL * P_GELEN_DB = NULL );

void    SM_MAKBUZ_ENT_FISLERINI_OLUSTUR                 ( int p_program_id , ADAK_SQL * P_GELEN_DB = NULL );


QString GET_MUH_ENT_KALAN_FIS_VE_SATIR_BILGISI          ( int p_program_id );

QString GET_CARI_ENT_KALAN_FIS_VE_SATIR_BILGISI         ( int p_program_id );

QString GET_CEK_ENT_KALAN_FIS_VE_SATIR_BILGISI          ( int p_program_id );

QString GET_FAT_ENT_KALAN_FIS_VE_SATIR_BILGISI          ( int p_program_id );

QString GET_PRK_ENT_KALAN_FIS_VE_SATIR_BILGISI          ( int p_program_id );

QString GET_BNK_DEFTER_ENT_KALAN_FIS_VE_SATIR_BILGISI   ( int p_program_id );

QString GET_BNK_MAKRO_ENT_KALAN_FIS_VE_SATIR_BILGISI    ( int p_program_id );

QString GET_BNK_HAREKET_ENT_KALAN_FIS_VE_SATIR_BILGISI  ( int p_program_id );

QString GET_STOK_ENT_KALAN_FIS_VE_SATIR_BILGISI         ( int p_program_id );


void    MUHASEBE_MODULUNU_ONAR                          ( ADAK_SQL * P_GELEN_DB = NULL);

void    CARI_MODULUNU_ONAR                              ( ADAK_SQL * P_GELEN_DB = NULL);

void    BANKA_MODULUNU_ONAR                             ( ADAK_SQL * P_GELEN_DB = NULL);

void    STOK_MODULUNU_ONAR                              ( ADAK_SQL * P_GELEN_DB = NULL);

void    ISLETME_MODULUNU_ONAR                           ( ADAK_SQL * P_GELEN_DB = NULL);



void                   MUHASEBE_BAKIYELERINI_SIFIRLA        ();
void                   CARI_BAKIYELERINI_SIFIRLA            ();
void                   BANKA_BAKIYELERINI_SIFIRLA           ();
void                   STOK_BAKIYELERINI_SIFIRLA            ();


void    FIS_NULL_COLUMNLARI_GUNCELLE   (QString p_table_name , QString p_id_column_name,
                                        int p_modul_id,int p_program_id,ADAK_SQL * P_GELEN_DB = NULL);

void    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE   (QString p_table_name , QString p_id_column_name,
                                               int p_modul_id,int p_program_id ,ADAK_SQL * P_GELEN_DB = NULL);


void    CEK_SENETIN_YANLIS_URETTIGI_FISLERI_SIL();

void    FATURA_VE_IRSALIYE_FISLERINI_ONAR      ();

void    HATALI_FISLERIN_ENT_FISLERINI_YOK_ET   ();

void    TUM_CEK_DURUMLARINI_ONAR               ();

void    CEK_SON_DURUMLARI_DUZELT               ();



#endif // E9_ONAR_UTILS_H
