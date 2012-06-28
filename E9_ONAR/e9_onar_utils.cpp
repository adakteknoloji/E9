#include "e9_onar_utils.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "cek_fis_utils.h"
#include "fat_fis_utils.h"
#include "muh_enum.h"
#include "cek_enum.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "fat_enum.h"
#include "bnk_fis_utils.h"
#include "bnk_defter_fis_utils.h"
#include "bnk_makro_fisi_class.h"
#include "bnk_makro_fisi_open.h"
#include "bnk_console_utils.h"
#include "banka_enum.h"
#include "cari_enum.h"
#include "prs_con_utils.h"
#include "BANKA/bnk_virman_formu_class.h"
#include "BANKA/bnk_virman_formu_open.h"
#include "stok_fis_utils.h"
#include "prk_fis_utils.h"
#include "dmr_fis_utils.h"
#include "dmr_con_utils.h"
#include "isl_fis_utils.h"
#include "isl_enum.h"
#include "smm_makbuz_utils.h"
#include "dmr_enum.h"
#include "dmr_con_utils.h"
#include "e9_console_utils.h"
#include "muh_console_utils.h"
#include "stok_console_utils.h"
#include "cari_console_utils.h"
#include "adak_gruplar.h"
#include "fat_console_utils.h"
#include "fat_fis_utils.h"
#include "irs_console_utils.h"
#include "irs_enum.h"
#include "cek_console_utils.h"


extern ADAK_SQL * DB;


/**************************************************************************************
                   MUH_ENT_FISLERINI_SIL
***************************************************************************************/

void MUH_ENT_FISLERINI_SIL(int p_program_id , int p_modul_id ,ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    FIS_NULL_COLUMNLARI_GUNCELLE       ("muh_fisler"        ,"fis_id"         , p_modul_id , p_program_id , USE_DB);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("muh_fis_satirlari" , "fis_satiri_id" , p_modul_id , p_program_id , USE_DB);

    SQL_QUERY query(USE_DB);

    query.PREPARE_DELETE("muh_fisler","program_id = :program_id AND "
                         "modul_id != :modul_id");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

    query.PREPARE_DELETE("muh_fis_satirlari","program_id = :program_id AND "
                         "modul_id != :modul_id");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

}

/**************************************************************************************
                   CARI_ENT_FISLERINI_SIL
***************************************************************************************/

void CARI_ENT_FISLERINI_SIL(int p_program_id , int p_modul_id ,ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    FIS_NULL_COLUMNLARI_GUNCELLE       ("car_fisler"         , "fis_id"         , p_modul_id , p_program_id , USE_DB);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("car_fis_satirlari"  , "fis_satiri_id"  , p_modul_id , p_program_id , USE_DB);


    SQL_QUERY query(USE_DB);

    query.PREPARE_DELETE("car_fisler","program_id = :program_id AND "
                         "modul_id != :modul_id");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

    query.PREPARE_DELETE("car_fis_satirlari","program_id = :program_id AND "
                         "modul_id != :modul_id");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

}

/**************************************************************************************
                   CEK_ENT_FISLERINI_SIL
***************************************************************************************/

void CEK_ENT_FISLERINI_SIL(int p_program_id , int p_modul_id ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    FIS_NULL_COLUMNLARI_GUNCELLE       ("cek_bordrolar"        , "bordro_id"        , p_modul_id , p_program_id , USE_DB);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("cek_bordro_satirlari" , "bordro_satiri_id" , p_modul_id , p_program_id , USE_DB);

    SQL_QUERY query(USE_DB);

    query.PREPARE_DELETE("cek_bordrolar","program_id = :program_id AND "
                         "modul_id != :modul_id ");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

    query.PREPARE_DELETE("cek_bordro_satirlari", "program_id = :program_id "
                         "AND modul_id != :modul_id ");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

}

/**************************************************************************************
                   FAT_ENT_FISLERINI_SIL
***************************************************************************************/

void FAT_ENT_FISLERINI_SIL(int p_program_id , int p_modul_id ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    FIS_NULL_COLUMNLARI_GUNCELLE       ("fat_faturalar"        , "fis_id"         , p_modul_id , p_program_id , USE_DB);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("fat_fatura_satirlari" , "fis_satiri_id"  , p_modul_id , p_program_id , USE_DB);

    SQL_QUERY query(USE_DB);

    query.PREPARE_DELETE("fat_faturalar","program_id = :program_id AND "
                         "modul_id != :modul_id ");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

    query.PREPARE_DELETE("fat_fatura_satirlari", "program_id = :program_id "
                         "AND modul_id != :modul_id ");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();
}

/**************************************************************************************
                   PRK_ENT_FISLERINI_SIL
***************************************************************************************/

void PRK_ENT_FISLERINI_SIL(int p_program_id , int p_modul_id ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    FIS_NULL_COLUMNLARI_GUNCELLE       ("prk_fisler"        , "fis_id"         , p_modul_id , p_program_id , USE_DB);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("prk_fis_satirlari" , "fis_satiri_id" , p_modul_id , p_program_id , USE_DB);


    SQL_QUERY query(USE_DB);

    query.PREPARE_DELETE("prk_fisler", "program_id = :program_id AND "
                         "modul_id != :modul_id ");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

    query.PREPARE_DELETE("prk_fis_satirlari","program_id = :program_id "
                         "AND modul_id != :modul_id ");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

}
/**************************************************************************************
                   ISL_ENT_FISLERINI_SIL
***************************************************************************************/

void ISL_ENT_FISLERINI_SIL(int p_program_id , int p_modul_id ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    FIS_NULL_COLUMNLARI_GUNCELLE       ("isl_fisi" ,"fis_id" , p_modul_id      , p_program_id , USE_DB);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("isl_fis_satirlari"  , "fis_satiri_id" , p_modul_id , p_program_id , USE_DB);

    query.PREPARE_DELETE("isl_fisi", "program_id = :program_id AND "
                         "modul_id != :modul_id ");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id  );
    query.DELETE();

    query.PREPARE_DELETE("isl_fis_satirlari","program_id = :program_id "
                         "AND modul_id != :modul_id ");
    query.SET_VALUE(":program_id" , p_program_id);
    query.SET_VALUE(":modul_id"   , p_modul_id);
    query.DELETE();

}

/**************************************************************************************
                   GET_MUH_ENT_KALAN_FIS_VE_SATIR_BILGISI
***************************************************************************************/

QString GET_MUH_ENT_KALAN_FIS_VE_SATIR_BILGISI(int p_program_id)
{
    SQL_QUERY query(DB);

    QString fis_ve_satir_bilgisi = "";

    query.PREPARE_SELECT("muh_fisler","fis_id","program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_sayisi = query.SELECT();

    if ( fis_sayisi > 0 ) {
        fis_ve_satir_bilgisi = QString(" %1 Fiş Bulundu. ( ").arg(fis_sayisi);
        while ( query.NEXT() ) {
            fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
        }
        fis_ve_satir_bilgisi.append(" ) ");
    }

    query.PREPARE_SELECT("muh_fis_satirlari", "fis_satiri_id", "program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_satir_sayisi = query.SELECT();

    if ( fis_satir_sayisi EQ 0 ) {
        return fis_ve_satir_bilgisi;
    }
    fis_ve_satir_bilgisi.append(QString("\n %1 Satır bulundu. ( ").arg(fis_satir_sayisi));

    while ( query.NEXT()) {
        fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
    }
    fis_ve_satir_bilgisi.append(" ) ");

    return fis_ve_satir_bilgisi;
}


/**************************************************************************************
                   GET_CARI_ENT_KALAN_FIS_VE_SATIR_BILGISI
***************************************************************************************/

QString GET_CARI_ENT_KALAN_FIS_VE_SATIR_BILGISI(int p_program_id)
{
    SQL_QUERY query(DB);

    QString fis_ve_satir_bilgisi = "";

    query.PREPARE_SELECT("car_fisler", "fis_id", "program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_sayisi = query.SELECT();

    if ( fis_sayisi > 0 ) {
        fis_ve_satir_bilgisi = QString(" %1 Fiş Bulundu. ( ").arg(fis_sayisi);
        while ( query.NEXT() ) {
            fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
        }
        fis_ve_satir_bilgisi.append(" ) ");
    }

    query.PREPARE_SELECT("car_fis_satirlari", "fis_satiri_id", "program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_satir_sayisi = query.SELECT();

    if ( fis_satir_sayisi EQ 0 ) {
        return fis_ve_satir_bilgisi;
    }
    fis_ve_satir_bilgisi.append(QString("\n %1 Satır bulundu. ( ").arg(fis_satir_sayisi));

    while ( query.NEXT()) {
        fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
    }
    fis_ve_satir_bilgisi.append(" ) ");

    return fis_ve_satir_bilgisi;
}

/**************************************************************************************
                   GET_CEK_ENT_KALAN_FIS_VE_SATIR_BILGISI
***************************************************************************************/

QString GET_CEK_ENT_KALAN_FIS_VE_SATIR_BILGISI(int p_program_id)
{
    SQL_QUERY query(DB);

    QString fis_ve_satir_bilgisi = "";

    query.PREPARE_SELECT("cek_bordrolar", "bordro_id","program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_sayisi = query.SELECT();

    if ( fis_sayisi > 0 ) {
        fis_ve_satir_bilgisi = QString(" %1 Fiş Bulundu. ( ").arg(fis_sayisi);
        while ( query.NEXT() ) {
            fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
        }
        fis_ve_satir_bilgisi.append(" ) ");
    }

    query.PREPARE_SELECT("cek_bordro_satirlari", "bordro_satiri_id", "program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_satir_sayisi = query.SELECT();

    if ( fis_satir_sayisi EQ 0 ) {
        return fis_ve_satir_bilgisi;
    }
    fis_ve_satir_bilgisi.append(QString("\n %1 Satır bulundu. ( ").arg(fis_satir_sayisi));

    while ( query.NEXT()) {
        fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
    }
    fis_ve_satir_bilgisi.append(" ) ");

    return fis_ve_satir_bilgisi;
}

/**************************************************************************************
                   GET_FAT_ENT_KALAN_FIS_VE_SATIR_BILGISI
***************************************************************************************/

QString GET_FAT_ENT_KALAN_FIS_VE_SATIR_BILGISI(int p_program_id)
{
    SQL_QUERY query(DB);

    QString fis_ve_satir_bilgisi;

    query.PREPARE_SELECT("fat_faturalar","fis_id","program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_sayisi = query.SELECT();

    if ( fis_sayisi > 0 ) {
        fis_ve_satir_bilgisi = QString(" %1 Fiş Bulundu. ( ").arg(fis_sayisi);
        while ( query.NEXT() ) {
            fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
        }
        fis_ve_satir_bilgisi.append(" ) ");
    }

    query.PREPARE_SELECT("fat_fatura_satirlari", "fis_satiri_id", "program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_satir_sayisi = query.SELECT();

    if ( fis_satir_sayisi EQ 0 ) {
        return fis_ve_satir_bilgisi;
    }
    fis_ve_satir_bilgisi.append(QString("\n %1 Satır bulundu. ( ").arg(fis_satir_sayisi));

    while ( query.NEXT()) {
        fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
    }
    fis_ve_satir_bilgisi.append(" ) ");

    return fis_ve_satir_bilgisi;
}

/**************************************************************************************
                   GET_PRK_ENT_KALAN_FIS_VE_SATIR_BILGISI
***************************************************************************************/

QString GET_PRK_ENT_KALAN_FIS_VE_SATIR_BILGISI(int p_program_id)
{
    SQL_QUERY query(DB);

    QString fis_ve_satir_bilgisi;

    query.PREPARE_SELECT("prk_fisler", "fis_id", "program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_sayisi = query.SELECT();

    if ( fis_sayisi > 0 ) {
        fis_ve_satir_bilgisi = QString(" %1 Fiş Bulundu. ( ").arg(fis_sayisi);
        while ( query.NEXT() ) {
            fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
        }
        fis_ve_satir_bilgisi.append(" ) ");
    }

    query.PREPARE_SELECT("prk_fis_satirlari", "fis_satiri_id","program_id != :program_id");
    query.SET_VALUE(":program_id" , p_program_id);

    int fis_satir_sayisi = query.SELECT();

    if ( fis_satir_sayisi EQ 0 ) {
        return fis_ve_satir_bilgisi;
    }
    fis_ve_satir_bilgisi.append(QString("\n %1 Satır bulundu. ( ").arg(fis_satir_sayisi));

    while ( query.NEXT()) {
        fis_ve_satir_bilgisi.append(query.VALUE(0).toString() + " , ");
    }
    fis_ve_satir_bilgisi.append(" ) ");

    return fis_ve_satir_bilgisi;
}

/**************************************************************************************
                   CARI_ENT_FISLERINI_OLUSTUR
***************************************************************************************/

void CARI_ENT_FISLERINI_OLUSTUR(int p_program_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY f_query( USE_DB );
    SQL_QUERY s_query( USE_DB );
    SQL_QUERY query  ( USE_DB );


    CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;
    MUH_FIS_STRUCT * MUH_FIS   = new MUH_FIS_STRUCT;

    CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI = new CARI_FIS_SATIRI_STRUCT;



    f_query.PREPARE_SELECT("car_fisler","fis_id,fis_turu", "program_id = :program_id "
                         "AND base_fis_id = :base_fis_id");
    f_query.SET_VALUE(":program_id"     , p_program_id);
    f_query.SET_VALUE(":base_fis_id"    , 0);

    f_query.AND_EKLE ("iptal_mi != :iptal_mi");
    f_query.SET_VALUE(":iptal_mi", 1 );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    while ( f_query.NEXT() ) {

        int fis_id = f_query.VALUE(0).toInt();
        int fis_turu = f_query.VALUE(1).toInt();

        CARI_CLEAR_FIS_STRUCT(CARI_FIS);

        CARI_FIS_BILGILERINI_OKU ( CARI_FIS , fis_id , USE_DB);

        CARI_FIS->muh_fis_id = 0;

        if ( fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
            continue;
        }

        MUH_CLEAR_FIS_STRUCT(MUH_FIS);

        QString aciklama = QObject::tr("Fiş CARİ modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(CARI_FIS->fis_no).toString());
        aciklama += CARI_FIS->aciklama;

        MUH_FIS->aciklama           = aciklama;
        MUH_FIS->fis_tarihi         = CARI_FIS->fis_tarihi;
        MUH_FIS->fis_turu           = ENUM_MAHSUP_FISI;
        MUH_FIS->modul_id           = CARI_FIS->modul_id;
        MUH_FIS->program_id         = CARI_FIS->program_id;
        MUH_FIS->base_fis_id        = fis_id;

        CARI_FIS->muh_fis_id = MUH_FIS_EKLE ( MUH_FIS ,USE_DB);


        query.PREPARE_UPDATE("car_fisler", "fis_id", "muh_fis_id", "fis_id = :fis_id");
        query.SET_VALUE(":muh_fis_id" , CARI_FIS->muh_fis_id );
        query.SET_VALUE(":fis_id"     , fis_id);
        query.UPDATE();

        s_query.PREPARE_SELECT("car_fis_satirlari","fis_satiri_id", "fis_id = :fis_id");
        s_query.SET_VALUE(":fis_id" , fis_id);

        if ( s_query.SELECT() EQ 0 ) {
            continue;
        }
        while ( s_query.NEXT()) {

            int fis_satiri_id = s_query.VALUE(0).toInt();

            CARI_CLEAR_FIS_SATIRI_STRUCT ( CARI_FIS_SATIRI);

            CARI_FIS_SATIRINI_OKU( CARI_FIS_SATIRI , fis_satiri_id , USE_DB);

            CARI_FIS_SATIRI_EKLE( CARI_FIS, CARI_FIS_SATIRI, false, fis_satiri_id, NULL, USE_DB);
        }
    }

    delete CARI_FIS;
    delete CARI_FIS_SATIRI;
    delete MUH_FIS;

}

/**************************************************************************************
                   CEK_ENT_FISLERINI_OLUSTUR
***************************************************************************************/

void CEK_ENT_FISLERINI_OLUSTUR(int p_program_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY f_query(USE_DB);
    SQL_QUERY s_query(USE_DB);
    SQL_QUERY query(USE_DB);

    CEK_BORDRO_STRUCT   * CEK_BORDROSU  = new CEK_BORDRO_STRUCT;
    MUH_FIS_STRUCT      * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    CEK_BORDRO_SATIRI_STRUCT * BORDRO_SATIRI = new CEK_BORDRO_SATIRI_STRUCT;


    f_query.PREPARE_SELECT("cek_bordrolar", "bordro_id , bordro_turu,bordro_islem_turu", "program_id = :program_id "
                         "AND base_fis_id = :base_fis_id");
    f_query.SET_VALUE(":program_id"  , p_program_id);
    f_query.SET_VALUE(":base_fis_id" , 0 );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    while ( f_query.NEXT() ) {

        int bordro_id         = f_query.VALUE(0).toInt();
        int bordro_turu       = f_query.VALUE(1).toInt();
        int bordro_islem_turu = f_query.VALUE(2).toInt();

        CEK_BORDROSU->CLEAR_BORDRO_STRUCT();

        CEK_BORDRO_BILGILERI_OKU(CEK_BORDROSU , bordro_id , USE_DB);
        CEK_BORDROSU->muh_fis_id = 0;

        switch ( bordro_islem_turu) {
            case ENUM_TAHSIL_EDILDI_NKT      :
            case ENUM_TAHSIL_EDILDI_HESABA   :
            case ENUM_MUS_IADE_EDILDI        :
            case ENUM_ODENDI_NKT             :
            case ENUM_ODENDI_HESAPTAN        :
            default                          :
                {
                    if ( bordro_turu NE ENUM_CEK_SENET_ACILIS_BORDROSU ) {
                        MUH_CLEAR_FIS_STRUCT (MUHASEBE_FISI);

                        if ( CEK_BORDROSU->modul_id EQ CEKSENET_MODULU ) {
                            QString aciklama = QObject::tr("Fiş ÇEK SENET modülü tarafından üretilmiştir.Bordro No : %1\n").arg(QVariant(CEK_BORDROSU->bordro_no).toString());
                            aciklama.append(CEK_BORDROSU->aciklama);

                            MUHASEBE_FISI->aciklama         = aciklama;
                            MUHASEBE_FISI->fis_turu         = ENUM_MAHSUP_FISI;
                        }
                        else {
                            MUHASEBE_FISI->aciklama = CEK_BORDROSU->aciklama;
                            if ( CEK_BORDROSU->bordro_islem_turu EQ ENUM_CEK_SATICIYA_VERILDI ) {
                                MUHASEBE_FISI->fis_turu = ENUM_TEDIYE_FISI;
                            }
                            else {
                                MUHASEBE_FISI->fis_turu = ENUM_MAHSUP_FISI;
                            }
                        }

                        MUHASEBE_FISI->fis_tarihi       = CEK_BORDROSU->bordro_tarihi;
                        MUHASEBE_FISI->modul_id         = CEK_BORDROSU->modul_id;
                        MUHASEBE_FISI->program_id       = CEK_BORDROSU->program_id;
                        MUHASEBE_FISI->base_fis_id      = bordro_id;

                        CEK_BORDROSU->muh_fis_id = MUH_FIS_EKLE ( MUHASEBE_FISI , USE_DB );
                    }
                }
                break;
        }

        query.PREPARE_UPDATE("cek_bordrolar", "bordro_id", "muh_fis_id ", "bordro_id = :bordro_id");
        query.SET_VALUE(":muh_fis_id" , CEK_BORDROSU->muh_fis_id);
        query.SET_VALUE(":bordro_id"  , bordro_id);
        query.UPDATE();


        s_query.PREPARE_SELECT("cek_bordro_satirlari", "bordro_satiri_id", "bordro_id = :bordro_id");
        s_query.SET_VALUE(":bordro_id" , bordro_id);

        if ( s_query.SELECT() EQ 0 ) {
            continue;
        }
        while ( s_query.NEXT()) {

            int bordro_satiri_id = s_query.VALUE(0).toInt();

            CEK_CLEAR_BORDRO_SATIRI_STRUCT (BORDRO_SATIRI);

            CEK_BORDRO_SATIRINI_OKU( BORDRO_SATIRI, bordro_satiri_id, USE_DB);
            CEK_BORDRO_SATIRI_EKLE( CEK_BORDROSU, BORDRO_SATIRI, false, false, bordro_satiri_id, USE_DB);
        }
    }

    delete CEK_BORDROSU;
    delete BORDRO_SATIRI;
    delete MUHASEBE_FISI;

}

/**************************************************************************************
                   FAT_ENT_FISLERINI_OLUSTUR
***************************************************************************************/

void FAT_ENT_FISLERINI_OLUSTUR(int p_program_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY f_query(USE_DB);
    SQL_QUERY s_query(USE_DB);
    SQL_QUERY query(USE_DB);


    FATURA_FISI_STRUCT * FATURA              = new FATURA_FISI_STRUCT ;

    MUH_FIS_STRUCT * MUHASEBE_FISI           = new MUH_FIS_STRUCT;

    FATURA_FIS_SATIRI_STRUCT * FATURA_SATIRI = new FATURA_FIS_SATIRI_STRUCT;

    FATURA->KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FATURA->KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FATURA->KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FATURA->OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FATURA->OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FATURA->OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FATURA->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];

    f_query.PREPARE_SELECT("fat_faturalar", "fis_id", "program_id = :program_id "
                         "AND base_fis_id = :base_fis_id");
    f_query.SET_VALUE(":program_id"  , p_program_id);
    f_query.SET_VALUE(":base_fis_id" , 0 );

    f_query.AND_EKLE ("iptal_mi != :iptal_mi");
    f_query.SET_VALUE(":iptal_mi", 1 );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    while ( f_query.NEXT() ) {

        int fis_id = f_query.VALUE(0).toInt();

        FATURA_CLEAR_FIS_STRUCT(FATURA);

        FATURA_FIS_BILGILERINI_OKU(FATURA , fis_id , USE_DB);

        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        MUHASEBE_FISI->modul_id              = FATURA->modul_id;
        MUHASEBE_FISI->program_id            = FATURA->program_id;
        MUHASEBE_FISI->fis_tarihi            = FATURA->fatura_tarihi;
        MUHASEBE_FISI->aciklama              = QObject::tr ( "Fiş FATURA MODÜLÜ tarafından üretilmiştir. Fiş No = %1 \n" ) .arg ( QVariant ( FATURA->fis_no ).toString() ) + FATURA->aciklama;
        MUHASEBE_FISI->fis_turu              = ENUM_MAHSUP_FISI;
        MUHASEBE_FISI->base_fis_id           = fis_id;

        FATURA->muh_fis_id = MUH_FIS_EKLE(MUHASEBE_FISI , USE_DB);

        query.PREPARE_UPDATE("fat_faturalar","fis_id", "muh_fis_id ", "fis_id = :fis_id");
        query.SET_VALUE(":muh_fis_id" , FATURA->muh_fis_id);
        query.SET_VALUE(":fis_id"     , fis_id);
        query.UPDATE();

        s_query.PREPARE_SELECT("fat_fatura_satirlari", "fis_satiri_id", "fis_id = :fis_id");
        s_query.SET_VALUE(":fis_id" , fis_id);

        if ( s_query.SELECT() EQ 0 ) {
            continue;
        }
        while ( s_query.NEXT()) {

            int fis_satiri_id = s_query.VALUE(0).toInt();

            FATURA_CLEAR_FIS_SATIRI_STRUCT  ( FATURA_SATIRI );

            FATURA_SATIR_BILGILERINI_OKU    ( FATURA_SATIRI , fis_satiri_id , USE_DB);

            FATURA_FIS_SATIRI_EKLE          ( FATURA , FATURA_SATIRI , false , fis_satiri_id , USE_DB);
        }
    }


    delete FATURA;
    delete FATURA_SATIRI;
    delete MUHASEBE_FISI;

}

/**************************************************************************************
                   PRK_ENT_FISLERINI_OLUSTUR
***************************************************************************************/

void PRK_ENT_FISLERINI_OLUSTUR(int p_program_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY f_query(USE_DB);
    SQL_QUERY s_query(USE_DB);
    SQL_QUERY query(USE_DB);

    MUH_FIS_STRUCT * MUHASEBE_FISI          = new MUH_FIS_STRUCT;

    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;


    f_query.PREPARE_SELECT("prk_fisler", "fis_id,aciklama,fis_no", "program_id = :program_id "
                           "AND base_fis_id = 0");
    f_query.SET_VALUE(":program_id" , p_program_id);

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    PRK_FIS_STRUCT * PRK_FISI              = new PRK_FIS_STRUCT;
    PRK_FIS_SATIRI_STRUCT * PRK_FIS_SATIRI = new PRK_FIS_SATIRI_STRUCT;

    while ( f_query.NEXT() ) {

        int fis_id = f_query.VALUE(0).toInt();


        PRK_CLEAR_FIS_STRUCT(PRK_FISI);

        PRK_FIS_BILGILERINI_OKU(PRK_FISI , fis_id , USE_DB);

        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        QString aciklama = QString("Fiş PERAKENDE modülü tarafından üretilmiştir.Fiş No : %1 \n").arg(f_query.VALUE("fis_no").toInt());
        aciklama.append(f_query.VALUE("aciklama").toString());

        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        MUHASEBE_FISI->aciklama         = PRK_FISI->aciklama;
        MUHASEBE_FISI->base_fis_id      = fis_id;
        MUHASEBE_FISI->fis_tarihi       = PRK_FISI->fis_tarihi;
        MUHASEBE_FISI->fis_turu         = ENUM_MAHSUP_FISI;
        MUHASEBE_FISI->modul_id         = PRK_FISI->modul_id;
        MUHASEBE_FISI->program_id       = PRK_FISI->program_id;


        PRK_FISI->muh_fis_id = MUH_FIS_EKLE (MUHASEBE_FISI , USE_DB);

        query.PREPARE_UPDATE("prk_fisler", "fis_id", "muh_fis_id", "fis_id = :fis_id" );
        query.SET_VALUE     (":muh_fis_id" , PRK_FISI->muh_fis_id);
        query.SET_VALUE     (":fis_id"     , fis_id);
        query.UPDATE();

        QList<int> prk_fis_satiri_id_list = PRK_GET_FIS_SATIRI_ID_LIST(fis_id , USE_DB);

        if ( prk_fis_satiri_id_list.isEmpty() EQ true ) {
            continue;
        }

        for ( int i = 0 ; i < prk_fis_satiri_id_list.size() ; i++ ) {

            int fis_satiri_id = prk_fis_satiri_id_list.at(i);

            //ayni belge numarasina ait satir varmi diye kontrol edilir.

            QList<int> uygun_satir_id_list = PRK_GET_AYNI_BELGE_NUMARALI_SATIR_LIST_AND_ENTEGRE_ET(fis_satiri_id ,USE_DB);

            PRK_CLEAR_FIS_SATIRI_STRUCT(PRK_FIS_SATIRI);

            if ( uygun_satir_id_list.isEmpty() EQ false ) {
                //ayni belge numarali satirlar okunur ve tutarlari bulunur.Entegre edilir.
                //Fis satirlari listesinden cikarilir.

                for ( int i = 0 ; i < uygun_satir_id_list.size() ; i++ ) {
                    prk_fis_satiri_id_list.removeAt(prk_fis_satiri_id_list.indexOf(uygun_satir_id_list.at(i)));
                }

            }
            else {
                QList<int> uygun_satir_id_list;

                PRK_FIS_SATIRI_BILGILERINI_OKU(PRK_FIS_SATIRI , fis_satiri_id , USE_DB);
                PRK_FIS_SATIRI_EKLE(PRK_FISI , PRK_FIS_SATIRI , uygun_satir_id_list ,fis_satiri_id , false ,USE_DB);
            }
        }
    }
    delete PRK_FISI;
    delete PRK_FIS_SATIRI;

    delete MUHASEBE_FISI;
    delete MUH_FIS_SATIRI;

}

/**************************************************************************************
                   BNK_ENT_FISLERINI_SIL
***************************************************************************************/

void BNK_ENT_FISLERINI_SIL ( int p_program_id , int p_modul_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    FIS_NULL_COLUMNLARI_GUNCELLE       ("bnk_defter_fisler"        , "fis_id"        , p_modul_id , p_program_id);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("bnk_defter_fis_satirlari" , "fis_satiri_id" , p_modul_id , p_program_id);

    SQL_QUERY query( USE_DB);

    // bnk_defter_fisler
    query.PREPARE_DELETE( "bnk_defter_fisler",
                          "program_id = :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.DELETE();

    query.PREPARE_DELETE( "bnk_defter_fis_satirlari","program_id = :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.DELETE();

    FIS_NULL_COLUMNLARI_GUNCELLE       ("bnk_hareket_fisler"        , "fis_id"        , p_modul_id , p_program_id);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("bnk_hareket_fis_satirlari" , "fis_satiri_id" , p_modul_id , p_program_id);

    // bnk_hareket_fisler
    query.PREPARE_DELETE( "bnk_hareket_fisler", "program_id = :program_id AND modul_id != :modul_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.SET_VALUE     ( ":modul_id"  , p_modul_id);
    query.DELETE();

    query.PREPARE_DELETE( "bnk_hareket_fis_satirlari ",
                          "program_id = :program_id AND modul_id != :modul_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.SET_VALUE     ( ":modul_id"  , p_modul_id);
    query.DELETE();

    // bnk_makro_fisler
    FIS_NULL_COLUMNLARI_GUNCELLE       ("bnk_makro_fisler"        , "fis_id"        , p_modul_id , p_program_id);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("bnk_makro_fis_satirlari" , "fis_satiri_id" , p_modul_id , p_program_id);

    query.PREPARE_DELETE( "bnk_makro_fisler",
                          "program_id = :program_id AND modul_id != :modul_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.SET_VALUE     ( ":modul_id"  , p_modul_id);
    query.DELETE();

    query.PREPARE_DELETE( "bnk_makro_fis_satirlari ",
                          "program_id = :program_id AND "
                          "modul_id != :modul_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.SET_VALUE     ( ":modul_id"  , p_modul_id);
    query.DELETE();

    FIS_NULL_COLUMNLARI_GUNCELLE       ("bnk_virman_fisler"        , "fis_id"        , p_modul_id , p_program_id);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("bnk_virman_fis_satirlari" , "fis_satiri_id" , p_modul_id , p_program_id);

    query.PREPARE_DELETE( "bnk_virman_fisler",
                          "program_id = :program_id AND modul_id != :modul_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.SET_VALUE     ( ":modul_id"  , p_modul_id);
    query.DELETE();

    query.PREPARE_DELETE( "bnk_virman_fis_satirlari ",
                          "program_id = :program_id AND "
                          "modul_id != :modul_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.SET_VALUE     ( ":modul_id"  , p_modul_id);
    query.DELETE();

}

/**************************************************************************************
                   STOK_ENT_FISLERINI_SIL
***************************************************************************************/

void STOK_ENT_FISLERINI_SIL ( int p_program_id , int p_modul_id)
{
    SQL_QUERY query( DB );

    FIS_NULL_COLUMNLARI_GUNCELLE       ("stok_fisler"        , "fis_id"        , p_modul_id , p_program_id);
    FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE("stok_fis_satirlari" , "fis_satiri_id" , p_modul_id , p_program_id);


    query.PREPARE_DELETE( "stok_fisler",
                          "program_id = :program_id AND modul_id != :modul_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.SET_VALUE     ( ":modul_id"  , p_modul_id);
    query.DELETE();

    query.PREPARE_DELETE( "stok_fis_satirlari",
                          "program_id = :program_id AND modul_id != :modul_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.SET_VALUE     ( ":modul_id"  , p_modul_id);
    query.DELETE();

}

/**************************************************************************************
                   GET_BNK_HAREKET_ENT_KALAN_FIS_VE_SATIR_BILGISI
***************************************************************************************/

QString GET_BNK_HAREKET_ENT_KALAN_FIS_VE_SATIR_BILGISI( int p_program_id )
{
    QString fis_ve_satiri_bilgisi = QString( "" );

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hareket_fisler", "fis_id", "program_id != :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    int fis_sayisi =  query.SELECT();

    if ( fis_sayisi > 0 ) {
        fis_ve_satiri_bilgisi.append(  QString( "%1 Fiş Bulundu. (bnk_hareket_fisler)( " ).arg( fis_sayisi) );
        while ( query.NEXT() EQ true ) {
            int fis_id  = query.VALUE( 0 ).toInt();
            fis_ve_satiri_bilgisi.append( " %1 ," ).arg( fis_id );
        }
         fis_ve_satiri_bilgisi.append(  QString( " ) " ) );
    }

    query.PREPARE_SELECT( "bnk_hareket_fis_satirlari", "fis_satiri_id", "program_id != :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    int satir_sayisi =  query.SELECT();

       if ( satir_sayisi > 0 ) {
        fis_ve_satiri_bilgisi.append(  QString( " \n  %1 Satir bulundu.(bnk_hareket_fis_satirlari) ( " ).arg( satir_sayisi) );
        while ( query.NEXT() EQ true ) {
            int fis_satiri_id   = query.VALUE( 0 ).toInt();
            fis_ve_satiri_bilgisi.append( QString( " %1 ," ).arg( fis_satiri_id ) );
        }
        fis_ve_satiri_bilgisi.append(  QString( " )" ) );
    }

    return fis_ve_satiri_bilgisi;

}


/**************************************************************************************
                   GET_BNK_MAKRO_ENT_KALAN_FIS_VE_SATIR_BILGISI
***************************************************************************************/

QString GET_BNK_MAKRO_ENT_KALAN_FIS_VE_SATIR_BILGISI( int p_program_id )
{
    QString fis_ve_satiri_bilgisi = QString( "" );

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_makro_fisler","fis_id","program_id != :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    int fis_sayisi =  query.SELECT();

    if ( fis_sayisi > 0 ) {
        fis_ve_satiri_bilgisi.append(  QString( "%1 Fiş Bulundu.(bnk_makro_fisler) ( " ).arg( fis_sayisi) );
        while ( query.NEXT() EQ true ) {
            int fis_id  = query.VALUE( 0 ).toInt();
            fis_ve_satiri_bilgisi.append( " %1 ," ).arg( fis_id );
        }
         fis_ve_satiri_bilgisi.append(  QString( " ) " ) );
    }

    query.PREPARE_SELECT( "bnk_makro_fis_satirlari", "fis_satiri_id", "program_id != :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    int satir_sayisi =  query.SELECT();

       if ( satir_sayisi > 0 ) {
        fis_ve_satiri_bilgisi.append(  QString( " \n  %1 Satir bulundu.(bnk_makro_fisler) ( " ).arg( satir_sayisi) );
        while ( query.NEXT() EQ true ) {
            int fis_satiri_id   = query.VALUE( 0 ).toInt();
            fis_ve_satiri_bilgisi.append( QString( " %1 ," ).arg( fis_satiri_id ) );
        }
        fis_ve_satiri_bilgisi.append(  QString( " )" ) );
    }

    return fis_ve_satiri_bilgisi;

}


/**************************************************************************************
                   GET_BNK_DEFTER_ENT_KALAN_FIS_VE_SATIR_BILGISI
***************************************************************************************/

QString GET_BNK_DEFTER_ENT_KALAN_FIS_VE_SATIR_BILGISI( int p_program_id )
{
    QString fis_ve_satiri_bilgisi = QString( "" );

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_defter_fisler", "fis_id", "program_id != :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    int fis_sayisi =  query.SELECT();

    if ( fis_sayisi > 0 ) {
        fis_ve_satiri_bilgisi.append(  QString( "%1 Fiş Bulundu.(bnk_defter_fisler) ( " ).arg( fis_sayisi) );
        while ( query.NEXT() EQ true ) {
            fis_ve_satiri_bilgisi.append( query.VALUE(0).toString() + ",");
        }
         fis_ve_satiri_bilgisi.append(  QString( " ) " ) );
    }

    query.PREPARE_SELECT( "bnk_defter_fis_satirlari", "fis_satiri_id", "program_id != :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    int satir_sayisi =  query.SELECT();

       if ( satir_sayisi > 0 ) {
        fis_ve_satiri_bilgisi.append(  QString( " \n  %1 Satir bulundu.(bnk_defter_fis_satirlari) ( " ).arg( satir_sayisi) );
        while ( query.NEXT() EQ true ) {
            int fis_satiri_id   = query.VALUE( 0 ).toInt();
            fis_ve_satiri_bilgisi.append( QString( " %1 ," ).arg( fis_satiri_id ) );
        }
        fis_ve_satiri_bilgisi.append(  QString( " )" ) );
    }

    return fis_ve_satiri_bilgisi;

}

/**************************************************************************************
                   GET_STOK_ENT_KALAN_FIS_VE_SATIR_BILGISI
***************************************************************************************/

QString GET_STOK_ENT_KALAN_FIS_VE_SATIR_BILGISI( int p_program_id )
{
    QString fis_ve_satiri_bilgisi = QString( "" );

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "stk_fisler", "fis_id", "program_id != :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    int fis_sayisi =  query.SELECT();

    if ( fis_sayisi > 0 ) {
        fis_ve_satiri_bilgisi.append(  QString( "%1 Fiş Bulundu. ( " ).arg( fis_sayisi) );
        while ( query.NEXT() EQ true ) {
            int fis_id  = query.VALUE( 0 ).toInt();
            fis_ve_satiri_bilgisi.append( " %1 ," ).arg( fis_id );
        }
         fis_ve_satiri_bilgisi.append(  QString( " ) " ) );
    }

    query.PREPARE_SELECT( "stk_fis_satirlari", "fis_satiri_id", "program_id != :program_id " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    int satir_sayisi =  query.SELECT();

       if ( satir_sayisi > 0 ) {
        fis_ve_satiri_bilgisi.append(  QString( " \n  %1 Satir bulundu. ( " ).arg( fis_sayisi) );
        while ( query.NEXT() EQ true ) {
            int fis_satiri_id   = query.VALUE( 0 ).toInt();
            fis_ve_satiri_bilgisi.append( QString( " %1 ," ).arg( fis_satiri_id ) );
        }
        fis_ve_satiri_bilgisi.append(  QString( " )" ) );
    }

    return fis_ve_satiri_bilgisi;

}

/**************************************************************************************
                   BNK_ENT_FISLERINI_OLUSTUR
***************************************************************************************/

void    BNK_ENT_FISLERINI_OLUSTUR( int p_program_id , ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB ;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query  ( USE_DB );
    SQL_QUERY s_query( USE_DB );
    SQL_QUERY update_query ( USE_DB);

    BNK_HAREKET_FIS_STRUCT        * HRK_FIS        = new BNK_HAREKET_FIS_STRUCT;
    BNK_HAREKET_FIS_SATIRI_STRUCT * HRK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;
    MUH_FIS_STRUCT                * MUH_FIS        = new MUH_FIS_STRUCT;

    query.PREPARE_SELECT( "bnk_hareket_fisler", "fis_id", "program_id = :program_id AND base_fis_id = 0 " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {

               int fis_id = query.VALUE( 0 ).toInt();

               BNK_CLEAR_HAREKET_FIS_STRUCT   ( HRK_FIS );
               BNK_HAREKET_FIS_BILGILERINI_OKU( HRK_FIS, fis_id ,USE_DB);

               HRK_FIS->def_fis_id  = 0;
               HRK_FIS->muh_fis_id  = 0;

               // muhasebe fis olustur.

               BNK_HAREKET_FIS_EKLE( HRK_FIS , fis_id, false , USE_DB );

               s_query.PREPARE_UPDATE( "bnk_hareket_fisler","fis_id", "muh_fis_id, def_fis_id ", "fis_id = :fis_id " );
               s_query.SET_VALUE     ( ":muh_fis_id", HRK_FIS->muh_fis_id );
               s_query.SET_VALUE     ( ":def_fis_id", HRK_FIS->def_fis_id );
               s_query.SET_VALUE     ( ":fis_id",     fis_id );
               s_query.UPDATE();


               s_query.PREPARE_SELECT( "bnk_hareket_fis_satirlari", "fis_satiri_id", "fis_id = :fis_id " );
               s_query.SET_VALUE( ":fis_id", fis_id );

               if ( s_query.SELECT() NE 0 ) {
                   while ( s_query.NEXT() EQ true ) {
                        int fis_satiri_id   = s_query.VALUE( 0 ).toInt();
                        BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT( HRK_FIS_SATIRI );
                        BNK_HAREKET_FIS_SATIR_BILGILERINI_OKU( HRK_FIS_SATIRI, fis_satiri_id ,USE_DB);
                        BNK_HAREKET_FIS_SATIRI_EKLE( HRK_FIS, HRK_FIS_SATIRI, fis_satiri_id, false ,USE_DB); // kendisi icin olusturmayacak
                   }
               }
        }
    }

    // makro fisi
    MAKRO_SATIR_STRUCT * M_FIS_SATIRI  = new MAKRO_SATIR_STRUCT;
    // BANKA_MAKRO_FISI

    SQL_QUERY t_query( USE_DB );
    query.PREPARE_SELECT( "bnk_makro_fisler","fis_id, fis_tarihi, hesap_no_id, aciklama, "
                          "fis_no",
                          "program_id = :program_id AND base_fis_id = :base_fis_id " );
    query.SET_VALUE     ( ":program_id" , p_program_id );
    query.SET_VALUE     ( ":base_fis_id", 0 );

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {
            int     fis_id              = query.VALUE( 0 ).toInt();
            QString fis_tarihi          = query.VALUE( 1 ).toString();
            int     hesap_no_id         = query.VALUE( 2 ).toInt();
            QString fis_aciklama        = query.VALUE( 3 ).toString();
            int     fis_no              = query.VALUE( 4 ).toInt();

            MAKRO_FIS_STRUCT * M_FIS    = new MAKRO_FIS_STRUCT;
            CLEAR_MAKRO_FIS( M_FIS );

            M_FIS->fis_tarihi       = fis_tarihi;
            M_FIS->fis_no           = fis_no;
            M_FIS->def_fis_id       = 0;
            M_FIS->program_id       = E9_PROGRAMI;
            M_FIS->modul_id         = BANKA_MODULU;
            M_FIS->base_fis_id      = 0;
            M_FIS->hesap_no_id      = hesap_no_id;
            M_FIS->para_birim_id    = 0;
            M_FIS->muh_fis_id       = 0;
            M_FIS->aciklama         = fis_aciklama;

            MAKRO_ADD_FIS( M_FIS , fis_id, false , USE_DB);

            s_query.PREPARE_UPDATE( "bnk_makro_fisler", "fis_id", "muh_fis_id , def_fis_id", "fis_id = :fis_id " );
            s_query.SET_VALUE     ( ":muh_fis_id", M_FIS->muh_fis_id );
            s_query.SET_VALUE     ( ":def_fis_id", M_FIS->def_fis_id );
            s_query.SET_VALUE     ( ":fis_id",     fis_id );
            s_query.UPDATE();


            s_query.PREPARE_SELECT( "bnk_makro_fis_satirlari",
                                    "fis_satiri_id, aciklama, doviz_kuru, parite, hesap_turu, makro_tutari, "
                                    "otomatik_odeme_tutari, makro_id, order_number, hesap_id ",
                                    "fis_id = :fis_id " );
            s_query.SET_VALUE( ":fis_id", fis_id );

            if ( s_query.SELECT() NE 0 ) {

                while ( s_query.NEXT() EQ true ) {
                     int fis_satiri_id              = s_query.VALUE( 0 ).toInt();
                     QString satir_aciklama         = s_query.VALUE( 1 ).toString();
                     double  doviz_kuru             = s_query.VALUE( 2 ).toDouble();
                     double  parite                 = s_query.VALUE( 3 ).toDouble();
                     int     hesap_turu             = s_query.VALUE( 4 ).toInt();
                     double  makro_tutari           = s_query.VALUE( 5 ).toDouble();
                     double  otomatik_odeme_tutari  = s_query.VALUE( 6 ).toDouble();
                     int     makro_id               = s_query.VALUE( 7 ).toInt();
                     int     order_number           = s_query.VALUE( 8 ).toInt();
                     int     hesap_id               = s_query.VALUE( 9 ).toInt();

                     CLEAR_MAKRO_FIS_SATIRI( M_FIS_SATIRI );

                     M_FIS_SATIRI->fis_id                   = fis_id;
                     M_FIS_SATIRI->fis_tarihi               = fis_tarihi;
                     M_FIS_SATIRI->fis_aciklama             = fis_aciklama;
                     M_FIS_SATIRI->hesap_no_id              = hesap_no_id;
                     M_FIS_SATIRI->banka_doviz_id           = BNK_GET_PARA_BIRIM_ID( hesap_no_id );
                     M_FIS_SATIRI->satir_aciklama           = satir_aciklama;
                     M_FIS_SATIRI->base_record_id           = 0;
                     M_FIS_SATIRI->doviz_kuru               = doviz_kuru;
                     M_FIS_SATIRI->parite                   = parite;
                     M_FIS_SATIRI->hesap_turu               = hesap_turu;
                     M_FIS_SATIRI->islem_tutari             = makro_tutari;
                     M_FIS_SATIRI->otomatik_odeme_tutari    = otomatik_odeme_tutari;
                     M_FIS_SATIRI->makro_id                 = makro_id;
                     M_FIS_SATIRI->order_number             = order_number;

                     switch ( hesap_turu ) {

                         case ENUM_BNK_CARI_HESAP : {
                                 t_query.PREPARE_SELECT("car_hesaplar","cari_hesap_kodu","cari_hesap_id = :cari_hesap_id");
                                 t_query.SET_VALUE(":cari_hesap_id" , hesap_id );

                                 if( t_query.SELECT() NE 0 ) {
                                     t_query.NEXT();
                                     M_FIS_SATIRI->search_edit_hesap = t_query.VALUE( 0 ).toString();
                                 }
                             }
                             break;
                         case ENUM_BNK_POS_HESABI:
                                 M_FIS_SATIRI->search_edit_hesap = BNK_GET_POS_NUMARASI( hesap_id );
                             break;

                         case ENUM_BNK_KREDI_KARTI_HESABI:
                             t_query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari", "kredi_karti_numarasi", "kredi_karti_id = :kredi_karti_id  "
                                                         "AND hesap_no_id = :hesap_no_id");
                             t_query.SET_VALUE(":kredi_karti_id" , hesap_id );
                             t_query.SET_VALUE(":hesap_no_id"    , hesap_no_id);
                             if(t_query.SELECT() NE 0 ) {
                                 t_query.NEXT();
                                 M_FIS_SATIRI->search_edit_hesap = t_query.VALUE(0).toString();
                             }
                             break;
                     }

                     MAKRO_ADD_LINE( M_FIS_SATIRI, fis_satiri_id, false,USE_DB );
                }
            }
        }
    }

    delete HRK_FIS;
    delete HRK_FIS_SATIRI;
    delete MUH_FIS;

    // banka virman fisleri olusturuluyor


    BNK_VIRMAN_FIS_AND_SATIRI_STRUCT * VIRMAN_STRUCT = new BNK_VIRMAN_FIS_AND_SATIRI_STRUCT;

    query.PREPARE_SELECT( "bnk_virman_fisler, bnk_virman_fis_satirlari",
                          "bnk_virman_fisler.fis_id, fis_satiri_id, fis_no, fis_tarihi, bnk_virman_fisler.aciklama,   "
                          "bnk_virman_fisler, gonderen_hesap_no_id, alici_hesap_no_id, doviz_kuru, "
                          "islem_tutari, gonderen_islem_tutari, alici_islem_tutari ",
                          "bnk_virman_fisler.fis_id = bnk_virman_fis_satirlari.fis_id "
                          "AND  bnk_virman_fisler.program_id = :program_id AND base_fis_id = 0 " );
    query.SET_VALUE     ( ":program_id", p_program_id );

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {

            CLEAR_BNK_VIRMAN_FIS_SATIRI_STRUCT( VIRMAN_STRUCT );

            int    fis_satiri_id                        = query.VALUE( 1 ).toInt();


            VIRMAN_STRUCT->fis_id                       = query.VALUE( 0 ).toInt();
            VIRMAN_STRUCT->fis_no                       = query.VALUE( 2 ).toInt();
            VIRMAN_STRUCT->fis_tarihi                   = query.VALUE( 3 ).toString();
            VIRMAN_STRUCT->aciklama                     = query.VALUE( 4 ).toString();
            VIRMAN_STRUCT->gonderen_hesap_no_id        = query.VALUE( 6 ).toInt();
            VIRMAN_STRUCT->alici_hesap_no_id           = query.VALUE( 7 ).toInt();
            VIRMAN_STRUCT->doviz_kuru                   = query.VALUE( 8 ).toDouble();
            VIRMAN_STRUCT->islem_tutari                 = query.VALUE( 9 ).toDouble();
            VIRMAN_STRUCT->gonderen_islem_tutari        = query.VALUE( 10 ).toDouble();
            VIRMAN_STRUCT->alici_islem_tutari           = query.VALUE( 11 ).toDouble();
            VIRMAN_STRUCT->gonderen_hesap_doviz_id      = BNK_GET_PARA_BIRIM_ID( VIRMAN_STRUCT->gonderen_hesap_no_id );
            VIRMAN_STRUCT->alici_hesap_doviz_id         = BNK_GET_PARA_BIRIM_ID( VIRMAN_STRUCT->alici_hesap_doviz_id );

            // satir id gondermen gerekiyor bve bool
            BNK_VIRMAN_FIS_SATIRI_EKLE( VIRMAN_STRUCT, fis_satiri_id, false ,USE_DB );

        }
    }

}

/**************************************************************************************
                   PRS_ENT_FISLERINI_OLUSTUR
***************************************************************************************/
void PRS_ENT_FISLERINI_OLUSTUR(int p_program_id , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query( USE_DB );

    QStringList BORDROLAR;

    query.PREPARE_SELECT("prs_bordrolar", "bordro_id");

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    while ( query.NEXT() ) {
       BORDROLAR << query.VALUE(0).toString();
    }

    for ( int i = 0 ; i < BORDROLAR.size() ; i++ ) {

        int bordro_id = QVariant(BORDROLAR.at(i)).toInt();

        PRS_ENTEGRASYON_FISLERINI_OLUSTUR(bordro_id , p_program_id , USE_DB);
    }
}


/**************************************************************************************
                   STK_ENT_FISLERINI_OLUSTUR
***************************************************************************************/

void STK_ENT_FISLERINI_OLUSTUR ( int p_program_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    // FISLERI TESPIT ET
    SQL_QUERY query  ( USE_DB );

    STK_IRS_FIS_STRUCT       * STK_FIS         = new STK_IRS_FIS_STRUCT;
    STK_IRS_FIS_SATIR_STRUCT * STK_FIS_SATIRI  = new STK_IRS_FIS_SATIR_STRUCT;


    query.PREPARE_SELECT( "stk_fisler, stk_fis_satirlari ",
                          "stk_fisler.fis_id ,                 "
                          "fis_no, fis_tarihi,                 "
                          "stk_fisler.aciklama,                "
                          "irsaliye_tutari,                    "
                          "stk_fisler.fis_turu,                "
                          "fis_satiri_id, urun_id, muh_hesap_id, doviz_kuru",
                          "stk_fisler.fis_id   = stk_fis_satirlari.fis_id   "
                          "AND stk_fisler.program_id = :program_id          "
                          "AND base_fis_id = 0                              "
                          "AND stk_fisler.modul_id     = :modul_id          " );
    query.SET_VALUE     ( ":program_id", p_program_id );
    query.SET_VALUE     ( ":modul_id", STOK_MODULU );

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {


            int fis_turu    = query.VALUE( "fis_turu" ).toInt();
            int muh_fis_id  = 0;
            int fis_id      = query.VALUE( "fis_id").toInt();

            if ( fis_turu EQ ENUM_STK_SARF_FIRE_FISI ) {

                STK_FIS->aciklama                   = query.VALUE( "aciklama" ).toString();
                STK_FIS->base_fis_id                = query.VALUE( "fis_id").toInt();
                STK_FIS->modul_id                   = STOK_MODULU;
                STK_FIS->program_id                 = E9_PROGRAMI;
                STK_FIS->fis_no                     = query.VALUE( "fis_no").toInt();
                STK_FIS->fis_tarihi                 = query.VALUE( "fis_tarihi").toString();
                STK_FIS->fis_turu                   = fis_turu;
                STK_FIS->doviz_kuru                 = query.VALUE( "doviz_kuru").toDouble();



                muh_fis_id  = STK_ENT_FIS_EKLE( STK_FIS , USE_DB );


                STK_FIS_SATIRI->base_record_id      = query.VALUE( "fis_satiri_id" ).toInt();
                STK_FIS_SATIRI->muh_fis_id          = muh_fis_id;
                STK_FIS_SATIRI->irsaliye_tutari     = query.VALUE( "irsaliye_tutari").toDouble();
                STK_FIS_SATIRI->modul_id            = STOK_MODULU;
                STK_FIS_SATIRI->program_id          = E9_PROGRAMI;
                STK_FIS_SATIRI->urun_id             = query.VALUE( "urun_id" ).toInt();
                STK_FIS_SATIRI->muh_hesap_id        = query.VALUE( "muh_hesap_id").toInt();

                STK_ENT_FIS_SATIRI_EKLE( STK_FIS, STK_FIS_SATIRI, USE_DB);

            }

            SQL_QUERY s_query( USE_DB );

            s_query.PREPARE_UPDATE( "stk_fisler", "fis_id", "muh_fis_id", "fis_id =:fis_id " );
            s_query.SET_VALUE     ( ":muh_fis_id", muh_fis_id );
            s_query.SET_VALUE     ( ":fis_id",     fis_id );

            s_query.UPDATE();
        }
    }
}

/**************************************************************************************
                   DMR_ENT_FISLERINI_OLUSTUR
***************************************************************************************/

void DMR_ENT_FISLERINI_OLUSTUR(int p_program_id, ADAK_SQL *P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }


    SQL_QUERY query(USE_DB);
    SQL_QUERY s_query(USE_DB);

    //AMORTISMAN VE YENIDEN DEGERLEME FISI MUH ENTEGRASYONU


    query.PREPARE_SELECT("dmr_fisler" , "fis_id" ,"program_id = :program_id "
                         "AND base_fis_id = :base_fis_id");
    query.SET_VALUE(":program_id"  , p_program_id);
    query.SET_VALUE(":base_fis_id" , 0);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    DMR_FIS_STRUCT * DMR_FISI = new DMR_FIS_STRUCT;

    DMR_FIS_SATIRI_STRUCT * DMR_FIS_SATIRI = new DMR_FIS_SATIRI_STRUCT;

    MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

    ISL_FIS_STRUCT * ISL_FISI = new ISL_FIS_STRUCT;

    ISL_FIS_SATIRI_STRUCT * ISL_FIS_SATIRI = new ISL_FIS_SATIRI_STRUCT;


    SQL_QUERY t_query(USE_DB);

    while ( query.NEXT() ) {

        int fis_id = query.VALUE("fis_id").toInt();

        DMR_CLEAR_FIS_STRUCT(DMR_FISI);

        DMR_FIS_BILGILERINI_OKU(DMR_FISI ,fis_id , USE_DB);

        QString aciklama = QObject::tr("Fiş SABİT KIYMET modülü tarafından üretilmiştir.Fiş No : %1\n").arg(QVariant(DMR_FISI->fis_no).toString());
        aciklama += DMR_FISI->aciklama;

        int isl_fis_id = ISL_GET_ENT_FIS_ID(DMR_FISI->modul_id , DMR_FISI->program_id,fis_id , USE_DB);

        if ( DMR_FISI->fis_turu EQ AMORTISMAN_AYIRMA_FISI ) {

            if ( isl_fis_id EQ 0 ) {
                ISL_CLEAR_FIS_STRUCT(ISL_FISI);

                ISL_FISI->program_id = DMR_FISI->program_id;
                ISL_FISI->modul_id   = DMR_FISI->modul_id;
                ISL_FISI->base_fis_id= fis_id;
                ISL_FISI->aciklama   = aciklama;
                ISL_FISI->fis_tarihi = DMR_FISI->fis_tarihi;
                ISL_FISI->fis_turu   = ENUM_GIDER_FISI;

                isl_fis_id = ISL_FISI_EKLE(ISL_FISI , USE_DB);
            }

        }

        MUH_CLEAR_FIS_STRUCT(MUH_FIS);

        MUH_FIS->aciklama           = aciklama;
        MUH_FIS->fis_tarihi         = DMR_FISI->fis_tarihi;
        MUH_FIS->fis_turu           = ENUM_MAHSUP_FISI;
        MUH_FIS->modul_id           = DMR_FISI->modul_id;
        MUH_FIS->program_id         = DMR_FISI->program_id;
        MUH_FIS->base_fis_id        = fis_id;

        DMR_FISI->muh_fis_id        = MUH_FIS_EKLE ( MUH_FIS ,USE_DB);


        s_query.PREPARE_UPDATE("dmr_fisler", "fis_id", "muh_fis_id", "fis_id = :fis_id");
        s_query.SET_VALUE(":muh_fis_id" , DMR_FISI->muh_fis_id );
        s_query.SET_VALUE(":fis_id"     , fis_id);
        s_query.UPDATE();


        s_query.PREPARE_SELECT("dmr_fis_satirlari" , "fis_satiri_id,demirbas_id" , "fis_id = :fis_id");
        s_query.SET_VALUE(":fis_id" , fis_id );

        if ( s_query.SELECT() EQ 0 ) {
            continue;
        }
        while ( s_query.NEXT() ) {

            int fis_satiri_id = s_query.VALUE(0).toInt();
            int demirbas_id   = s_query.VALUE(1).toInt();

            t_query.PREPARE_SELECT("dmr_demirbaslar" , "bir_amor_hesap_id,amor_gider_hesap_id",
                                   "demirbas_id = :demirbas_id");
            t_query.SET_VALUE(":demirbas_id" , demirbas_id);

            if ( t_query.SELECT() EQ 0 ) {
                continue;
            }
            t_query.NEXT();


            DMR_CLEAR_FIS_SATIRI_STRUCT(DMR_FIS_SATIRI );

            DMR_FIS_SATIRINI_OKU(DMR_FIS_SATIRI , fis_satiri_id , USE_DB);

            if ( DMR_FISI->fis_turu EQ AMORTISMAN_AYIRMA_FISI ) {

                if ( isl_fis_id > 0 ) {

                    ISL_CLEAR_FIS_SATIRI_STRUCT(ISL_FIS_SATIRI);

                    ISL_FIS_SATIRI->modul_id              = DMR_FISI->modul_id;
                    ISL_FIS_SATIRI->program_id            = DMR_FISI->program_id;
                    ISL_FIS_SATIRI->base_record_id        = fis_satiri_id;
                    ISL_FIS_SATIRI->aciklama              = DMR_FIS_SATIRI->aciklama;
                    ISL_FIS_SATIRI->fis_id                = isl_fis_id;
                    ISL_FIS_SATIRI->gider_turu            = ENUM_GENEL_GIDERLER;
                    ISL_FIS_SATIRI->satir_kdv_dahil_tutar = DMR_FIS_SATIRI->satir_tutari;
                    ISL_FIS_SATIRI->satir_kdv_haric_tutar = DMR_FIS_SATIRI->satir_tutari;

                    ISL_FIS_SATIRI_EKLE(ISL_FISI , ISL_FIS_SATIRI , USE_DB);
                }
            }


            MUH_CLEAR_FIS_SATIRI_STRUCT     ( MUH_FIS_SATIRI );

            MUH_FIS_SATIRI->aciklama                = DMR_FIS_SATIRI->aciklama;
            MUH_FIS_SATIRI->borc_tutari             = DMR_FIS_SATIRI->satir_tutari;
            MUH_FIS_SATIRI->alacak_tutari           = 0;
            MUH_FIS_SATIRI->fis_id                  = DMR_FISI->muh_fis_id;
            MUH_FIS_SATIRI->hesap_id                = t_query.VALUE(1).toInt();
            MUH_FIS_SATIRI->program_id              = DMR_FIS_SATIRI->program_id;
            MUH_FIS_SATIRI->modul_id                = DMR_FIS_SATIRI->modul_id;
            MUH_FIS_SATIRI->base_record_id          = DMR_FIS_SATIRI->base_record_id;

            MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);

            MUH_FIS_SATIRI->borc_tutari             = 0;
            MUH_FIS_SATIRI->alacak_tutari           = DMR_FIS_SATIRI->satir_tutari;
            MUH_FIS_SATIRI->hesap_id                = t_query.VALUE(0).toInt();

            MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);


        }
    }
    //DEMIRBAS FATURA ENTEGRASYONU

    query.PREPARE_SELECT("dmr_demirbaslar" , "fatura_id,demirbas_id");

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    while ( query.NEXT() ) {

        int fatura_id    = query.VALUE(0).toInt();
        int demirbas_id  = query.VALUE(1).toInt();

        if ( fatura_id > 0 ) {
            DMR_ENTEGRASYON_FISI_OLUSTUR(fatura_id , demirbas_id,USE_DB);
        }
    }
}

/**************************************************************************************
                   SM_MAKBUZ_ENT_FISLERINI_OLUSTUR
***************************************************************************************/

void SM_MAKBUZ_ENT_FISLERINI_OLUSTUR(int p_program_id, ADAK_SQL *P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY f_query(USE_DB);
    SQL_QUERY query(USE_DB);

    MUH_FIS_STRUCT * MUHASEBE_FISI          = new MUH_FIS_STRUCT;


    f_query.PREPARE_SELECT("smm_makbuzlar", "makbuz_id,aciklama,makbuz_no", "program_id = :program_id "
                           "AND base_record_id = 0");
    f_query.SET_VALUE(":program_id" , p_program_id);

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    SMM_MAKBUZ_STRUCT   * SM_MAKBUZU       = new SMM_MAKBUZ_STRUCT;

    while ( f_query.NEXT() ) {

        int fis_id = f_query.VALUE(0).toInt();

        SMM_CLEAR_MAKBUZ_STRUCT(SM_MAKBUZU);

        SMM_MAKBUZ_BILGILERINI_OKU(SM_MAKBUZU , fis_id ,USE_DB);

        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        QString aciklama = QString("Fiş SM MAKBUZ modülü tarafından üretilmiştir.Fiş No : %1 \n").arg(f_query.VALUE("makbuz_no").toInt());
        aciklama.append(f_query.VALUE("aciklama").toString());

        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        MUHASEBE_FISI->aciklama         = SM_MAKBUZU->aciklama;
        MUHASEBE_FISI->base_fis_id      = fis_id;
        MUHASEBE_FISI->fis_tarihi       = SM_MAKBUZU->makbuz_tarihi;
        MUHASEBE_FISI->fis_turu         = ENUM_MAHSUP_FISI;
        MUHASEBE_FISI->modul_id         = SM_MAKBUZU->modul_id;
        MUHASEBE_FISI->program_id       = SM_MAKBUZU->program_id;

        SM_MAKBUZU->muh_fis_id = MUH_FIS_EKLE (MUHASEBE_FISI , USE_DB);

        query.PREPARE_UPDATE("smm_makbuzlar", "makbuz_id", "muh_fis_id", "makbuz_id = :makbuz_id" );
        query.SET_VALUE     (":muh_fis_id"  , SM_MAKBUZU->muh_fis_id);
        query.SET_VALUE     (":makbuz_id"   , fis_id);
        query.UPDATE();

        SMM_ENT_FISLERI_OLUSTUR(SM_MAKBUZU , fis_id,USE_DB);
    }
    delete SM_MAKBUZU;
    delete MUHASEBE_FISI;
}

/**************************************************************************************
                   FIS_NULL_COLUMNLARI_GUNCELLE
***************************************************************************************/

void FIS_NULL_COLUMNLARI_GUNCELLE(QString p_table_name, QString p_id_column_name,
                                  int p_modul_id, int p_program_id,ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY select_query(USE_DB);
    SQL_QUERY query(USE_DB);


    select_query.PREPARE_SELECT(p_table_name, p_id_column_name + ",program_id,modul_id,base_fis_id" ,"");


    if ( select_query.SELECT() NE 0 ) {

        while ( select_query.NEXT() ) {

            int fis_id              = select_query.VALUE(0).toInt();
            int program_id          = select_query.VALUE(1).toInt();
            int modul_id            = select_query.VALUE(2).toInt();
            int base_fis_id         = select_query.VALUE(3).toInt();


            if ( program_id EQ 0) {
                program_id = p_program_id;
            }
            if ( modul_id EQ 0 ) {
                modul_id   = p_modul_id;
            }
            if ( base_fis_id EQ 0) {
                base_fis_id = 0;
            }

            query.PREPARE_UPDATE(p_table_name ,p_id_column_name , "program_id,modul_id,base_fis_id",
                                 QString("%1 = :%1").arg(p_id_column_name));

            query.SET_VALUE(":program_id"                        , program_id);
            query.SET_VALUE(":modul_id"                          , modul_id);
            query.SET_VALUE(":base_fis_id"                       , base_fis_id);
            query.SET_VALUE(QString(":%1").arg(p_id_column_name) , fis_id);

            query.UPDATE();
        }
    }
}


/**************************************************************************************
                   FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE
***************************************************************************************/

void FIS_SATIRI_NULL_COLUMNLARI_GUNCELLE(QString p_table_name, QString p_id_column_name,
                                         int p_modul_id, int p_program_id,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }


    SQL_QUERY select_query(USE_DB);
    SQL_QUERY query(USE_DB);


    select_query.PREPARE_SELECT(p_table_name, p_id_column_name + ",program_id,modul_id,base_record_id" ,"");


    if ( select_query.SELECT() NE 0 ) {

        while ( select_query.NEXT() ) {

            int fis_satiri_id          = select_query.VALUE(0).toInt();
            int program_id             = select_query.VALUE(1).toInt();
            int modul_id               = select_query.VALUE(2).toInt();
            int base_record_id         = select_query.VALUE(3).toInt();


            if ( program_id EQ 0 ) {
                program_id = p_program_id;
            }
            if ( modul_id EQ 0 ) {
                modul_id   = p_modul_id;
            }
            if ( base_record_id EQ 0 ) {
                base_record_id = 0;
            }

            query.PREPARE_UPDATE(p_table_name ,p_id_column_name , "program_id,modul_id,base_record_id",
                                 QString("%1 = :%1").arg(p_id_column_name));
            query.SET_VALUE(":program_id"                        , program_id);
            query.SET_VALUE(":modul_id"                          , modul_id);
            query.SET_VALUE(":base_record_id"                    , base_record_id);
            query.SET_VALUE(QString(":%1").arg(p_id_column_name) , fis_satiri_id);

            query.UPDATE();
        }
    }
}

/**************************************************************************************
                   MUHASEBE_MODULUNU_ONAR();
***************************************************************************************/

void MUHASEBE_MODULUNU_ONAR(ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY       query(USE_DB);

    query.PREPARE_SELECT("muh_fisler","fis_id, fis_turu, fis_tarihi"  );

    if ( query.SELECT( "fis_id ASC ") EQ 0 ) {
        return;
    }

    SQL_QUERY       select_query(USE_DB);
    SQL_QUERY       update_query(USE_DB);

    double          borc_tutari   = 0.0;
    double          alacak_tutari = 0.0;


    while ( query.NEXT() EQ true ) {

        int     fis_id              = query.VALUE(0).toInt();
        int     fis_turu            = query.VALUE(1).toInt();
        QDate   fis_tarihi          = QDate::fromString(query.VALUE(2).toString(),"yyyy.MM.dd");

        select_query .PREPARE_SELECT("muh_fis_satirlari", "fis_satiri_id, hesap_id, borc_tutari, alacak_tutari" ,
                                     "fis_id = :fis_id" );
        select_query.SET_VALUE(":fis_id",fis_id);

        if ( select_query.SELECT() EQ 0 ) {
            continue;
        }

        double toplam_borc   = 0.0;
        double toplam_alacak = 0.0;

        while ( select_query.NEXT() EQ true ) {

            int fis_satiri_id    = select_query.VALUE(0).toInt();
            int hesap_id         = select_query.VALUE(1).toInt();

            borc_tutari   = ROUND(select_query.VALUE(2).toDouble());
            alacak_tutari = ROUND(select_query.VALUE(3).toDouble());

            update_query.PREPARE_UPDATE("muh_fis_satirlari","fis_satiri_id",
                                        "borc_tutari    , "
                                        "alacak_tutari    ",
                                        "fis_satiri_id = :fis_satiri_id ");

            update_query.SET_VALUE(":borc_tutari"  ,borc_tutari);
            update_query.SET_VALUE(":alacak_tutari",alacak_tutari);
            update_query.SET_VALUE(":fis_satiri_id",fis_satiri_id);

            update_query.UPDATE();

            int gun_no = 0;
            switch (fis_turu ) {

                case ENUM_KAPANIS_FISI :
                case ENUM_MAHSUP_FISI  :
                case ENUM_TAHSIL_FISI  :
                case ENUM_TEDIYE_FISI  :
                    gun_no = MALI_YIL_ARRAY_INDIS( fis_tarihi );
                    break;
                default :
                    break;

            }

            MUH_HESABININ_BAKIYELERINI_ARTTIR( hesap_id, gun_no, borc_tutari, alacak_tutari, USE_DB );

            toplam_borc   += borc_tutari;
            toplam_alacak += alacak_tutari;
        }

        update_query.PREPARE_UPDATE("muh_fisler","fis_id",
                                    "toplam_borc  , "
                                    "toplam_alacak  ",
                                    "fis_id = :fis_id");
        update_query.SET_VALUE(":toplam_borc"  ,ROUND(toplam_borc));
        update_query.SET_VALUE(":toplam_alacak",ROUND(toplam_alacak));
        update_query.SET_VALUE(":fis_id"       ,fis_id);

        update_query.UPDATE();

    }
}

/**************************************************************************************
                   CARI_MODULUNU_ONAR
***************************************************************************************/

void CARI_MODULUNU_ONAR( ADAK_SQL * P_GELEN_DB )
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY select_query ( USE_DB );

    select_query.PREPARE_SELECT("car_fisler","fis_id, fis_tarihi, fis_turu, cari_hesap_id  ");

    if ( select_query.SELECT( "fis_no" ) EQ 0 ) {
        return;
    }

    SQL_QUERY update_query(USE_DB);
    SQL_QUERY select_query_satirlar(USE_DB);

    double borc_tutari   = 0.0;
    double alacak_tutari = 0.0;

    while (select_query.NEXT()) {
        int fis_id        = select_query.VALUE(0).toInt();
        QDate fis_tarihi  = QDate::fromString(select_query.VALUE(1).toString(),"yyyy.MM.dd");
        int fis_turu      = select_query.VALUE(2).toInt();
        int cari_hesap_id = select_query.VALUE(3).toInt();

        select_query_satirlar.PREPARE_SELECT("car_fis_satirlari",
                                             "fis_satiri_id,cari_hesap_id,borc_tutari,alacak_tutari,"
                                             "cari_para_birimi_tutari ",
                                             "fis_id = :fis_id");
        select_query_satirlar.SET_VALUE(":fis_id" , fis_id);

        if(select_query_satirlar.SELECT() EQ 0 ) {
            continue;
        }
        double fis_toplam_borc   = 0;
        double fis_toplam_alacak = 0;
        double cari_para_birimi_tutari = 0.0;

        while (select_query_satirlar.NEXT()) {
            int fis_satiri_id = select_query_satirlar.VALUE(0).toInt();
            int line_cari_hesap_id = select_query_satirlar.VALUE(1).toInt();

            borc_tutari              = ROUND( select_query_satirlar.VALUE(2).toDouble() );
            alacak_tutari            = ROUND( select_query_satirlar.VALUE(3).toDouble() );
            cari_para_birimi_tutari  = ROUND( select_query_satirlar.VALUE(4).toDouble() );

            update_query.PREPARE_UPDATE("car_fis_satirlari","fis_satiri_id",
                                        "borc_tutari    ,"
                                        "alacak_tutari   ",
                                        "fis_satiri_id = :fis_satiri_id");
            update_query.SET_VALUE(":borc_tutari"   , borc_tutari);
            update_query.SET_VALUE(":alacak_tutari" , alacak_tutari);
            update_query.SET_VALUE(":fis_satiri_id" , fis_satiri_id);
            update_query.UPDATE();

            int gun_no = 0;

            switch (fis_turu) {
                case ENUM_CARI_HAREKET_FISI :
                case ENUM_CARI_VIRMAN_FORMU :
                gun_no = MALI_YIL_ARRAY_INDIS( fis_tarihi );

                    if ( borc_tutari > alacak_tutari ) {
                        CARI_HESABIN_BAKIYESINI_ARTTIR(gun_no,line_cari_hesap_id, cari_para_birimi_tutari, 0.0 ,USE_DB);
                        fis_toplam_borc += borc_tutari;
                    }
                    else {
                        CARI_HESABIN_BAKIYESINI_ARTTIR(gun_no,line_cari_hesap_id, 0.0 ,cari_para_birimi_tutari,USE_DB);
                        fis_toplam_alacak += alacak_tutari;
                    }
                    break;
                case ENUM_CARI_ACILIS_FISI  :
                    if ( borc_tutari > alacak_tutari ) {
                        CARI_HESABIN_BAKIYESINI_ARTTIR(gun_no,line_cari_hesap_id, cari_para_birimi_tutari, 0.0,USE_DB);
                        fis_toplam_borc += borc_tutari;
                    }
                    else {
                        CARI_HESABIN_BAKIYESINI_ARTTIR(gun_no,line_cari_hesap_id, 0.0, cari_para_birimi_tutari, USE_DB);
                        fis_toplam_alacak += alacak_tutari;
                    }
                    break;


                default :
                    gun_no = MALI_YIL_ARRAY_INDIS  ( fis_tarihi );

                    if ( borc_tutari > alacak_tutari ) {
                        CARI_HESABIN_BAKIYESINI_ARTTIR(gun_no,cari_hesap_id, cari_para_birimi_tutari,0.0, USE_DB);
                        fis_toplam_borc += borc_tutari;
                    }
                    else {
                        CARI_HESABIN_BAKIYESINI_ARTTIR(gun_no,cari_hesap_id ,0.0,cari_para_birimi_tutari,USE_DB);
                        fis_toplam_alacak += alacak_tutari;
                    }
                    break;
            }

        }

        update_query.PREPARE_UPDATE("car_fisler","fis_id","toplam_borc ,toplam_alacak  ",
                                    "fis_id = :fis_id");
        update_query.SET_VALUE(":toplam_borc"   , ROUND(fis_toplam_borc  ));
        update_query.SET_VALUE(":toplam_alacak" , ROUND(fis_toplam_alacak));
        update_query.SET_VALUE(":fis_id"        , fis_id);
        update_query.UPDATE();

    }
}

/**************************************************************************************
                   BANKA_MODULUNU_ONAR
***************************************************************************************/

void BANKA_MODULUNU_ONAR( ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY select_query(USE_DB);
    SQL_QUERY update_query(USE_DB);
    SQL_QUERY select_fis_satirlari(USE_DB);

    double hesaba_giren   = 0.0;
    double hesaptan_cikan = 0.0;

    select_query.PREPARE_SELECT("bnk_defter_fisler","fis_id,fis_tarihi, fis_turu,hesap_no_id " );

    if ( select_query.SELECT( "fis_no ASC " ) EQ 0 ) {
        return;
    }

    while (select_query.NEXT()) {
        int fis_id       = select_query.VALUE(0).toInt();
        QDate fis_tarihi = QDate::fromString(select_query.VALUE(1).toString(),"yyyy.MM.dd");
        int fis_turu     = select_query.VALUE(2).toInt();
        int hesap_no_id  = select_query.VALUE(3).toInt();


        int gun_no = -1;

        if ( fis_turu EQ ENUM_BANKA_ACILIS_FISI) {
           gun_no = 0;
        }
        else {
            gun_no = MALI_YIL_ARRAY_INDIS ( fis_tarihi );
        }

        select_fis_satirlari.PREPARE_SELECT("bnk_defter_fis_satirlari",
                                            "fis_satiri_id,hesaba_giren,      "
                                            "hesaptan_cikan,      "
                                            "hesap_turu",
                                            "fis_id = :fis_id");
        select_fis_satirlari.SET_VALUE(":fis_id" , fis_id);

        if ( select_fis_satirlari.SELECT() EQ 0 ) {
            continue;
        }
        double fis_toplam_borc   = 0.0;
        double fis_toplam_alacak = 0.0;


        while (select_fis_satirlari.NEXT()) {
            int fis_satiri_id           = select_fis_satirlari.VALUE(0).toInt();
            hesaba_giren                = ROUND(select_fis_satirlari.VALUE(1).toDouble());
            hesaptan_cikan              = ROUND(select_fis_satirlari.VALUE(2).toDouble());
            int etkilenecek_hesap_turu  = select_fis_satirlari.VALUE(3).toInt();

            update_query.PREPARE_UPDATE("bnk_defter_fis_satirlari","fis_satiri_id",
                                        "hesaba_giren ,"
                                        "hesaptan_cikan   ",
                                        "fis_satiri_id = :fis_satiri_id");
            update_query.SET_VALUE(":hesaba_giren"     , hesaba_giren);
            update_query.SET_VALUE(":hesaptan_cikan"   , hesaptan_cikan);
            update_query.SET_VALUE(":fis_satiri_id"    , fis_satiri_id);
            update_query.UPDATE();

            fis_toplam_borc   += hesaba_giren;
            fis_toplam_alacak += hesaptan_cikan;


            if ( etkilenecek_hesap_turu EQ ENUM_BNK_POS_HESABI ) {
                BANKA_HESAP_BAKIYELERINI_ARTTIR(gun_no,hesap_no_id,hesaba_giren,hesaptan_cikan,ENUM_BNK_POS_HESABI ,USE_DB);
                continue;
            }
            else if ( etkilenecek_hesap_turu EQ ENUM_BNK_KREDI_KARTI_HESABI ) {
                BANKA_HESAP_BAKIYELERINI_ARTTIR(gun_no,hesap_no_id,hesaba_giren,hesaptan_cikan,ENUM_BNK_KREDI_KARTI_HESABI, USE_DB);
                continue;
            }
            else if ( etkilenecek_hesap_turu EQ ENUM_BNK_BANKA_HESABI ) {
                if ( fis_turu EQ ENUM_BANKA_ACILIS_FISI) {
                    BANKA_HESAP_BAKIYELERINI_ARTTIR(gun_no,hesap_no_id,hesaba_giren,hesaptan_cikan,ENUM_BNK_BANKA_HESABI, USE_DB);
                }
                else {
                    BANKA_HESAP_BAKIYELERINI_ARTTIR(gun_no,hesap_no_id,hesaba_giren,hesaptan_cikan,ENUM_BNK_BANKA_HESABI, USE_DB);
                }
            }
        }
        update_query.PREPARE_UPDATE("bnk_defter_fisler", "fis_id",
                                    "toplam_hesaba_giren        ,"
                                    "toplam_hesaptan_cikan       ",
                                    "fis_id = :fis_id");
        update_query.SET_VALUE(":toplam_hesaba_giren"     , ROUND(fis_toplam_borc));
        update_query.SET_VALUE(":toplam_hesaptan_cikan"   , ROUND(fis_toplam_alacak));
        update_query.SET_VALUE(":fis_id"                  , fis_id);
        update_query.UPDATE();

    }
}

/**************************************************************************************
                   STOK_MODULUNU_ONAR
***************************************************************************************/

void STOK_MODULUNU_ONAR( ADAK_SQL * P_GELEN_DB )
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY select_query(USE_DB);
    SQL_QUERY select_fis_satirlari(USE_DB);

    select_query.PREPARE_SELECT("stk_fisler","fis_id,fis_tarihi,fis_turu,depo_id,alan_depo_id " );

    if(select_query.SELECT( "fis_no ASC " ) EQ 0 ) {
        return;
    }


    while (select_query.NEXT()) {
        int fis_id                   = select_query.VALUE(0).toInt();
        QDate fis_tarihi             = QDate::fromString(select_query.VALUE(1).toString(),"yyyy.MM.dd");
        int fis_turu                 = select_query.VALUE(2).toInt();
        int depo_id                  = select_query.VALUE(3).toInt();
        int alan_depo_id             = select_query.VALUE(4).toInt();

        int etkilenen_depo_id = depo_id;

        if ( fis_turu EQ ENUM_STK_DEPO_TRANSFER_GIRIS ) {
            etkilenen_depo_id = alan_depo_id;
        }

        select_fis_satirlari.PREPARE_SELECT("stk_fis_satirlari",
                                            "urun_id,satirdaki_urun_miktari,"
                                            "parti_id , temel_birim_katsayisi",
                                            "fis_id = :fis_id" );

        select_fis_satirlari.SET_VALUE(":fis_id" , fis_id);

        if ( select_fis_satirlari.SELECT() EQ 0 ) {
            continue;
        }

        while ( select_fis_satirlari.NEXT() ) {

            int    urun_id                   = select_fis_satirlari.VALUE(0).toInt();
            double satirdaki_urun_miktari    = select_fis_satirlari.VALUE(1).toDouble();
            int    parti_id                  = select_fis_satirlari.VALUE(2).toInt();
            double temel_birim_katsayisi     = select_fis_satirlari.VALUE(3).toInt();

            if ( urun_id EQ 0 ) {
                continue;
            }

            //urunler tablosundaki urun miktari ayarlanacak.

            double stoga_giren_miktar   = 0.0;
            double stoktan_cikan_miktar = 0.0;

            switch (fis_turu) {
               case ENUM_STK_IMALATTAN_DONUS_FISI :
               case ENUM_STK_STOK_ACILIS_FISI     :
               case ENUM_IRS_NORMAL_GIRIS:
               case ENUM_STK_DEPO_TRANSFER_GIRIS:
               case ENUM_IRS_FASON_BASLANGIC_GIRIS:
               case ENUM_IRS_FASON_BITIS_GIRIS:
               case ENUM_IRS_KONSINYE_GIRIS:
               case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS:
               case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS:
                   stoga_giren_miktar = satirdaki_urun_miktari * temel_birim_katsayisi;
                   break;
               default :
                   stoktan_cikan_miktar = satirdaki_urun_miktari * temel_birim_katsayisi;
                   break;
           }

            int gun_no = 0;

            if (fis_turu NE ENUM_STK_STOK_ACILIS_FISI) {
                gun_no = MALI_YIL_ARRAY_INDIS ( fis_tarihi );
            }

            if ( fis_turu NE ENUM_STK_DEPO_TRANSFER_CIKIS AND fis_turu NE ENUM_STK_DEPO_TRANSFER_GIRIS ) {
                STK_URUN_KARTINDAKI_STOK_MIKTARINI_ARTIR(urun_id,gun_no,stoga_giren_miktar,stoktan_cikan_miktar ,USE_DB);
            }

            STK_DEPODAKI_STOK_MIKTARINI_ARTIR (etkilenen_depo_id, urun_id,gun_no,stoga_giren_miktar,stoktan_cikan_miktar, USE_DB );

            if ( parti_id EQ 0 ) {
                continue;
            }

            QString parti_kodu = STK_GET_PARTI_KODU(parti_id);
            STK_PARTIDEKI_URUN_MIKTARINI_ARTIR(etkilenen_depo_id, urun_id,gun_no,parti_kodu,stoga_giren_miktar,stoktan_cikan_miktar, "","",USE_DB);

        }
    }
}

/**************************************************************************************
                   ISLETME_MODULUNU_ONAR
***************************************************************************************/

void ISLETME_MODULUNU_ONAR(ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query (USE_DB);
    SQL_QUERY s_query(USE_DB);
    SQL_QUERY update_query(USE_DB);

    query.PREPARE_SELECT("isl_fisi","fis_id,modul_id,program_id");

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    while ( query.NEXT() ) {

        int fis_id = query.VALUE(0).toInt();

        double kdv_haric_toplam = 0.0;
        double kdv_toplam       = 0.0;
        double kdv_dahil_toplam = 0.0;

        s_query.PREPARE_SELECT("isl_fis_satirlari",
                               "satir_kdv_haric_tutar, satir_kdv_tutari, satir_kdv_dahil_tutar,modul_id,program_id,fis_satiri_id",
                               "fis_id = :fis_id");
        s_query.SET_VALUE(":fis_id" , fis_id);

        if ( s_query.SELECT() NE 0 ) {

            while ( s_query.NEXT() ) {

                double kdv_haric_tutar = s_query.VALUE(0).toDouble();

                double kdv_tutari      = s_query.VALUE(1).toDouble();

                double kdv_dahil_tutar = s_query.VALUE(2).toDouble();

                kdv_haric_toplam += kdv_haric_tutar;
                kdv_toplam       += kdv_tutari;
                kdv_dahil_toplam += kdv_dahil_tutar;

                SQL_QUERY t_query (DB);

                t_query.PREPARE_UPDATE("isl_fis_satirlari","fis_satiri_id","modul_id,program_id","fis_satiri_id=:fis_satiri_id");
                t_query.SET_VALUE(":fis_satiri_id",s_query.VALUE("fis_satiri_id").toString());

                if (s_query.VALUE("modul_id").toInt() EQ 0 ) {
                    t_query.SET_VALUE(":modul_id",ISLETME_MODULU);
                }
                else {
                    t_query.SET_VALUE(":modul_id",s_query.VALUE("modul_id"));
                }

                if (s_query.VALUE("program_id").toInt() EQ 0 ) {
                    t_query.SET_VALUE(":program_id", E9_PROGRAMI);
                }
                else {
                    t_query.SET_VALUE(":program_id", s_query.VALUE("program_id"));
                }

                t_query.UPDATE();
            }
        }
        update_query.PREPARE_UPDATE("isl_fisi", "fis_id",
                                    "kdv_haric_toplam ,"
                                    "kdv_toplam       ,"
                                    "kdv_dahil_toplam,modul_id,program_id  ",
                                    "fis_id = :fis_id");
        update_query.SET_VALUE(":kdv_haric_toplam" , ROUND(kdv_haric_toplam ));
        update_query.SET_VALUE(":kdv_toplam"       , ROUND(kdv_toplam       ));
        update_query.SET_VALUE(":kdv_dahil_toplam" , ROUND(kdv_dahil_toplam ));

        if (query.VALUE(1).toInt() EQ 0 ) {
            update_query.SET_VALUE(":modul_id",ISLETME_MODULU);
        }
        else {
            update_query.SET_VALUE(":modul_id",query.VALUE(1));
        }

        if (query.VALUE(2).toInt() EQ 0) {
            update_query.SET_VALUE(":program_id", E9_PROGRAMI);
        }
        else {
            update_query.SET_VALUE(":program_id", query.VALUE(2));
        }

        update_query.SET_VALUE(":fis_id"           , fis_id);
        update_query.UPDATE();

    }

}


/**************************************************************************************
                   MUHASEBE_BAKIYELERINI_SIFIRLA
***************************************************************************************/


void MUHASEBE_BAKIYELERINI_SIFIRLA()
{
    SQL_QUERY       select_query(DB);

    select_query.PREPARE_SELECT("muh_hesaplar", "hesap_id, parent_id");

    if ( select_query.SELECT( "tam_hesap_kodu ASC" ) EQ 0 ) {
        //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Kayıtlı muhasebe hesabı bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    SQL_QUERY       update_query(DB);
    SQL_QUERY       query_parent_id(DB);

    double          borc_bakiyesi[E9_ARRAY_SIZE];
    double          alacak_bakiyesi[E9_ARRAY_SIZE];

    //BILGILENDIR (QString("<font color = \"red\"> %1</font> - Muhasebe hesapları düzeltiliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe hesaplarının bakiyeleri düzeltiliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    while ( select_query.NEXT()  EQ true ) {
        int hesap_id  = select_query.VALUE(0).toInt();
        int parent_id = select_query.VALUE(1).toInt();

        update_query.PREPARE_UPDATE("muh_hesaplar", "hesap_id", "tali_hesap ,hesap_seviyesi ",
                                    "hesap_id = :hesap_id");

        update_query.SET_VALUE     ( ":tali_hesap",     1   );
        update_query.SET_VALUE     ( ":hesap_seviyesi", 1   );
        update_query.SET_VALUE     ( ":hesap_id",       hesap_id    );

        update_query.UPDATE();

        if ( parent_id NE 0 ) {
           update_query.PREPARE_UPDATE("muh_hesaplar","hesap_id","tali_hesap",
                                       "hesap_id = :hesap_id");

           update_query.SET_VALUE     ( ":tali_hesap",     0   );
           update_query.SET_VALUE     ( ":hesap_id",parent_id);
           update_query.UPDATE();

           int hesap_seviyesi = 1;
           for ( ; ; ) {
               query_parent_id.PREPARE_SELECT("muh_hesaplar", "parent_id", "hesap_id = :hesap_id");
               query_parent_id.SET_VALUE(":hesap_id",parent_id);
               if( query_parent_id.SELECT() EQ 0 ) {
                   break;
               }
               query_parent_id.NEXT();
               parent_id = query_parent_id.VALUE("parent_id").toInt();
               hesap_seviyesi++;
               if ( parent_id EQ 0 ) {
                   break;
               }
           }
           update_query.PREPARE_UPDATE("muh_hesaplar", "hesap_id", "hesap_seviyesi",
                                       "hesap_id = :hesap_id");
           update_query.SET_VALUE(":hesap_seviyesi" , hesap_seviyesi);
           update_query.SET_VALUE(":hesap_id"       , hesap_id);
           update_query.UPDATE();
        }
        for (int i = 0 ; i < E9_ARRAY_SIZE ; i++ )  {
            borc_bakiyesi[i]   = 0.00;
            alacak_bakiyesi[i] = 0.00;
        }


        update_query.PREPARE_UPDATE("muh_hesaplar",
                                    "hesap_id",
                                    "toplam_borc_array ,"
                                    "toplam_alacak_array",
                                    "hesap_id = :hesap_id");

        update_query.SET_VALUE(":toplam_borc_array"        , PACK_DOUBLE_ARRAY(borc_bakiyesi , E9_ARRAY_SIZE));
        update_query.SET_VALUE(":toplam_alacak_array"      , PACK_DOUBLE_ARRAY(alacak_bakiyesi , E9_ARRAY_SIZE));
        update_query.SET_VALUE(":hesap_id"                 , hesap_id);
        update_query.UPDATE();
    }
    //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe hesapları düzeltildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}


/**************************************************************************************
                   CARI_BAKIYELERINI_SIFIRLA();
***************************************************************************************/

void CARI_BAKIYELERINI_SIFIRLA()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("car_hesaplar","cari_hesap_id ");

    if ( select_query.SELECT() EQ 0 ) {
        //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Kayıtlı cari hesap bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    SQL_QUERY query(DB);

    double    toplam_borc   [E9_ARRAY_SIZE];
    double    toplam_alacak [E9_ARRAY_SIZE];

    for (int i = 0 ; i < E9_ARRAY_SIZE ; i++) {
        toplam_borc[i]         = 0;
        toplam_alacak[i]       = 0;
    }
    //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Hesap Bakiyeleri Onarılıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    while (select_query.NEXT()) {
        int cari_hesap_id = select_query.VALUE(0).toInt();


        query.PREPARE_UPDATE("car_hesaplar", "cari_hesap_id",
                             "toplam_borc_array ,"
                             "toplam_alacak_array" ,
                             "cari_hesap_id = :cari_hesap_id");

        query.SET_VALUE(":toplam_borc_array"         , PACK_DOUBLE_ARRAY(toplam_borc   , E9_ARRAY_SIZE));
        query.SET_VALUE(":toplam_alacak_array"       , PACK_DOUBLE_ARRAY(toplam_alacak , E9_ARRAY_SIZE));
        query.SET_VALUE(":cari_hesap_id"             , cari_hesap_id);

        query.UPDATE();

    }
    //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Hesap Bakiyeleri Sıfırlandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/**************************************************************************************
                   BANKA_BAKIYELERINI_SIFIRLA();
***************************************************************************************/

void BANKA_BAKIYELERINI_SIFIRLA()
{
    SQL_QUERY select_query(DB);
    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id");

    if ( select_query.SELECT() EQ 0 ) {
        //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Kayıtlı Banka Hesabı Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }


    SQL_QUERY update_query(DB);

    double toplam_borc[E9_ARRAY_SIZE];
    double toplam_alacak[E9_ARRAY_SIZE];

    //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Hesap Bakiyeleri Onarılıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    while (select_query.NEXT()) {
        int hesap_no_id = select_query.VALUE(0).toInt();

        for (int i = 0 ; i < E9_ARRAY_SIZE ; i++) {
            toplam_borc[i]   = 0;
            toplam_alacak[i] = 0;
        }

        QString sql_query = QString ("toplam_borc_array         ,toplam_alacak_array ");

        QString where_condition = QString( "hesap_no_id = :hesap_no_id" );

        update_query.PREPARE_UPDATE( "bnk_hesaplar", "hesap_no_id", sql_query, where_condition );

        update_query.SET_VALUE     ( ":toplam_borc_array"           , PACK_DOUBLE_ARRAY(toplam_borc,E9_ARRAY_SIZE));
        update_query.SET_VALUE     ( ":toplam_alacak_array"         , PACK_DOUBLE_ARRAY(toplam_alacak, E9_ARRAY_SIZE));
        update_query.SET_VALUE     ( ":hesap_no_id"                 , hesap_no_id);

        update_query.UPDATE();

        sql_query = QString("toplam_kk_tahsilat_borc_array ,"
                            "toplam_kk_thslat_alacak_array ");

        where_condition = QString( "hesap_no_id = :hesap_no_id" );

        update_query.PREPARE_UPDATE( "bnk_pos_arrayler", "kayit_id", sql_query, where_condition );

        update_query.SET_VALUE     ( ":toplam_kk_tahsilat_borc_array"   , PACK_DOUBLE_ARRAY(toplam_borc,E9_ARRAY_SIZE));
        update_query.SET_VALUE     ( ":toplam_kk_thslat_alacak_array"   , PACK_DOUBLE_ARRAY(toplam_alacak , E9_ARRAY_SIZE));
        update_query.SET_VALUE     ( ":hesap_no_id"                 , hesap_no_id);

        update_query.UPDATE();


       sql_query = QString(" toplam_kk_odeme_borc_array     ,"
                           " toplam_kk_odeme_alacak_array   ");
        where_condition = QString( "hesap_no_id = :hesap_no_id" );

        update_query.PREPARE_UPDATE( "bnk_kk_arrayler", "kayit_id", sql_query, where_condition );

        update_query.SET_VALUE     ( ":toplam_kk_odeme_borc_array"       , PACK_DOUBLE_ARRAY(toplam_borc,E9_ARRAY_SIZE));
        update_query.SET_VALUE     ( ":toplam_kk_odeme_alacak_array"     , PACK_DOUBLE_ARRAY(toplam_alacak , E9_ARRAY_SIZE));
        update_query.SET_VALUE     ( ":hesap_no_id"                 , hesap_no_id);

        update_query.UPDATE();

    }
    //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Hesaplarının bakiyeleri sıfırlandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}


/**************************************************************************************
                   STOK_BAKIYELERINI_SIFIRLA();
***************************************************************************************/


void STOK_BAKIYELERINI_SIFIRLA ()
{
    SQL_QUERY select_query(DB);
    SQL_QUERY update_query(DB);
    SQL_QUERY select_fis_satirlari(DB);
    SQL_QUERY query(DB);

    double stok_giris_miktari[E9_ARRAY_SIZE];
    double stok_cikis_miktari[E9_ARRAY_SIZE];

    for (int i = 0 ; i < E9_ARRAY_SIZE ; i++) {
        stok_giris_miktari[i] = 0.0;
        stok_cikis_miktari[i] = 0.0;
    }

    select_query.PREPARE_SELECT("stk_urunler","urun_id" );

    if(select_query.SELECT() EQ 0 ) {
        //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Stok modülünde kayıtlı ürün bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }
    //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Ürünler tablosundaki stok miktarları onarılıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    while (select_query.NEXT()) {
        int urun_id = select_query.VALUE(0).toInt();

        QString sql_query = QString ("toplam_stoga_giren_array        ");

        update_query.PREPARE_UPDATE( "stk_urunler", "urun_id", sql_query, "urun_id = :urun_id");

        update_query.SET_VALUE     ( ":toplam_stoga_giren_array"        , PACK_DOUBLE_ARRAY(stok_giris_miktari , E9_ARRAY_SIZE) );
        update_query.SET_VALUE     ( ":urun_id"                         , urun_id );

        update_query.UPDATE();

        sql_query = QString ("toplam_stoktan_cikan_array      " );

        update_query.PREPARE_UPDATE( "stk_urunler", "urun_id", sql_query, "urun_id = :urun_id");

        update_query.SET_VALUE     ( ":toplam_stoktan_cikan_array"      , PACK_DOUBLE_ARRAY(stok_giris_miktari , E9_ARRAY_SIZE) );
        update_query.SET_VALUE     ( ":urun_id"                         , urun_id );

        update_query.UPDATE();

    }
    //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Ürünler tablosundaki stok miktarları sıfırlandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    select_query.PREPARE_SELECT("stk_depodaki_urunler","kayit_id ");

    if ( select_query.SELECT() NE 0 ) {

        //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Depodaki ürünler tablosundaki , ürün miktarları onarılıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while (select_query.NEXT()) {
            int kayit_id = select_query.VALUE(0).toInt();

            QString sql_query = QString("toplam_depoya_giren_array ,toplam_depodan_cikan_array ");


            update_query.PREPARE_UPDATE( "stk_depodaki_urunler", "kayit_id", sql_query, "kayit_id = :kayit_id");

            update_query.SET_VALUE     ( ":toplam_depoya_giren_array"       , PACK_DOUBLE_ARRAY(stok_giris_miktari, E9_ARRAY_SIZE) );
            update_query.SET_VALUE     ( ":toplam_depodan_cikan_array"      , PACK_DOUBLE_ARRAY(stok_cikis_miktari,E9_ARRAY_SIZE) );
            update_query.SET_VALUE     ( ":kayit_id"                        , kayit_id );


            update_query.UPDATE();

        }
        //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Depodaki ürünler tablosundaki , ürün miktarları sıfırlandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    }


    select_query.PREPARE_SELECT("stk_urunun_partileri","parti_id");


    if ( select_query.SELECT() NE 0 ) {

        //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Ürünlere ait parti miktarları onarılıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while (select_query.NEXT()) {
            int parti_id = select_query.VALUE(0).toInt();

            QString sql_query = QString("toplam_partiye_giren_array     , "
                                        "toplam_partiden_cikan_array      ");

            update_query.PREPARE_UPDATE( "stk_urunun_partileri", "parti_id", sql_query, "parti_id = :parti_id");

            update_query.SET_VALUE     ( ":toplam_partiye_giren_array"   , PACK_DOUBLE_ARRAY(stok_giris_miktari,E9_ARRAY_SIZE) );
            update_query.SET_VALUE     ( ":toplam_partiden_cikan_array"  , PACK_DOUBLE_ARRAY(stok_cikis_miktari,E9_ARRAY_SIZE) );
            update_query.SET_VALUE     ( ":parti_id"                     , parti_id );

            update_query.UPDATE();
        }
        //BILGILENDIR (QString(" <font color = \"red\">%1</font> - Ürünlere ait parti miktarları sıfırlandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    }
}

/**************************************************************************************
                   CEK_SENETIN_YANLIS_URETTIGI_FISLERI_SIL
***************************************************************************************/

void  CEK_SENETIN_YANLIS_URETTIGI_FISLERI_SIL()
{

    SQL_QUERY f_query( DB );
    SQL_QUERY s_query( DB );

    f_query.PREPARE_SELECT  ( "cek_bordrolar", "base_fis_id, modul_id", " program_id = :program_id AND modul_id != :modul_id");
    f_query.SET_VALUE       ( ":program_id", E9_PROGRAMI       );
    f_query.SET_VALUE       ( ":modul_id"  , CEKSENET_MODULU   );

    if ( f_query.SELECT() NE 0 ) {

        while( f_query.NEXT() EQ true ) {
            int base_fis_id = f_query.VALUE( "base_fis_id" ).toInt();
            int modul_id    = f_query.VALUE( "modul_id" ).toInt();

            if ( modul_id EQ CARI_MODULU ) {
                s_query.PREPARE_SELECT( "car_fisler" , "fis_id", " modul_id = :modul_id AND base_fis_id = :base_fis_id");
                s_query.SET_VALUE     ( ":modul_id"   , modul_id );
                s_query.SET_VALUE     ( ":base_fis_id", base_fis_id );

                if ( s_query.SELECT() NE 0 ) {
                   s_query.NEXT();
                   int car_fis_id = s_query.VALUE( 0 ).toInt();

                   CARI_FIS_STRUCT CARI_FIS;
                   CARI_FIS_BILGILERINI_OKU( &CARI_FIS, car_fis_id );

                   CARI_FISI_SIL( &CARI_FIS, car_fis_id );
                }
            }
            else if ( modul_id EQ BANKA_MODULU ) {

                s_query.PREPARE_SELECT( "bnk_hareket_fisler" , "fis_id", " modul_id = :modul_id AND base_fis_id = :base_fis_id");
                s_query.SET_VALUE     ( ":modul_id"   , modul_id );
                s_query.SET_VALUE     ( ":base_fis_id", base_fis_id );

                if ( s_query.SELECT() NE 0 ) {
                   s_query.NEXT();
                   int bnk_fis_id = s_query.VALUE( 0 ).toInt();

                   BNK_HAREKET_FIS_STRUCT BNK_FIS;

                   BNK_HAREKET_FIS_BILGILERINI_OKU( &BNK_FIS, bnk_fis_id );

                   BNK_HAREKET_FISI_SIL( &BNK_FIS, bnk_fis_id );

                }
            }
        }
    }
}

/**************************************************************************************
                   FATURA_VE_IRSALIYE_FISLERINI_ONAR
***************************************************************************************/

void FATURA_VE_IRSALIYE_FISLERINI_ONAR()
{

    SQL_QUERY f_query( DB );
    SQL_QUERY s_query( DB );
    SQL_QUERY t_query( DB );


    FATURA_FISI_STRUCT       FAT_FIS;
    FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;

    FAT_FIS.KDV_ORANLARI_ARRAY                     =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_HESAPLARI_ID_ARRAY                 =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY        =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.OTV_ORANLARI_ARRAY                     =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_HESAPLARI_ID_ARRAY                 =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY        =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY      =   new int     [KDV_OTV_ORANLARI_SAYISI];

    FATURA_CLEAR_FIS_STRUCT       ( &FAT_FIS );
    FATURA_CLEAR_FIS_SATIRI_STRUCT( &FAT_FIS_SATIRI);

    STK_IRS_FIS_STRUCT       IRS_FIS;
    STK_IRS_FIS_SATIR_STRUCT IRS_FIS_SATIRI;

    STK_IRS_CLEAR_FIS_STRUCT   ( &IRS_FIS );
    STK_IRS_CLEAR_SATIR_STRUCT ( &IRS_FIS_SATIRI );


    f_query.PREPARE_SELECT( "fat_irs_baglantisi", "fatura_id, irsaliye_id", "" );

    // BAGLANTI IRSALIYELENEN  VEYA FATURALANAN FIS VAR
    if ( f_query.SELECT() NE 0 ) {

        while ( f_query.NEXT() EQ true ) {
            int fatura_id   = f_query.VALUE( 0 ).toInt();
            int irsaliye_id = f_query.VALUE( 1 ).toInt();

            FATURA_CLEAR_FIS_STRUCT   ( &FAT_FIS );
            FATURA_FIS_BILGILERINI_OKU( &FAT_FIS, fatura_id );

            STK_IRS_CLEAR_FIS_STRUCT   ( &IRS_FIS );
            STK_IRS_FIS_BILGILERINI_OKU( irsaliye_id, &IRS_FIS );

            switch ( FAT_FIS.fatura_turu ) {

                case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS         :
                case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS        :
                case ENUM_FAT_IRSALIYELI_ALIS_FATURASI          :
                case ENUM_FAT_IRSALIYELI_SATIS_FATURASI         :
                case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS  :
                case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS : {
                // fat_irs_baglantisi TABLOSUNDA 1-1 BAGLANTI VARDIR

                // BU FATURARALAR TAMEN IRSALIYENLENMISTIR.
                // AYNI SEKILDE BUNLARIN OLUSTURDUGU IRSALIYELER ISE
                // TAMAMEN FATURALANMISTIR

                // FATURADAKI SATIR BAGLATINLARINI BUL

                s_query.PREPARE_SELECT( "fat_fatura_satirlari", "fis_satiri_id", "fis_id = :fis_id" );
                s_query.SET_VALUE     ( ":fis_id", fatura_id );

                if ( s_query.SELECT() NE 0 ) {
                    while ( s_query.NEXT() EQ true ) {
                        int  fatura_fis_satiri_id   = s_query.VALUE( 0 ).toInt();
                        int  irsaliye_fis_satiri_id = STK_GET_FAT_IRS_STR_BAGLANTISI_IRSALIYE_SATIRI_ID( fatura_fis_satiri_id );
                        FATURA_CLEAR_FIS_SATIRI_STRUCT  ( &FAT_FIS_SATIRI );
                        FATURA_SATIR_BILGILERINI_OKU    ( &FAT_FIS_SATIRI, fatura_fis_satiri_id );

                        FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE( fatura_fis_satiri_id, FAT_FIS_SATIRI.satirdaki_urun_miktari, ENUM_FAT_IRSALIYELENEN_MIKTAR_GUNCELLE );

                        IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE( irsaliye_fis_satiri_id, FAT_FIS_SATIRI.satirdaki_urun_miktari, ENUM_IRS_FATURALANAN_MIKTAR_GUNCELLE );
                    }
                }

                // FATURA DURMUNU IRSALIYENLENMIS OLARAK GUNCELLE
                // IRSALIYE DURUMUNU FATURALANMIS OLARAK GUNCELLE

                FAT_UPDATE_FATURA_IRSALIYELESTIRILDI_MI    ( fatura_id  , 1 );
                IRS_IRSALIYE_FATURALANMIS_DURUMUNU_GUNCELLE( irsaliye_id, 1 );

                break;
                }
                case ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI         :
                case ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI        : {
                // fat_irs_baglantisi TABLOSUNDA 1-N BAGLANTI VARDIR

                s_query.PREPARE_SELECT( "stk_fis_satirlari", "fis_satiri_id", "fis_id = :fis_id" );
                s_query.SET_VALUE     ( ":fis_id", irsaliye_id );

                if ( s_query.SELECT() NE 0 ) {
                    while ( s_query.NEXT() EQ true ) {
                        int  irs_fis_satiri_id   = s_query.VALUE( 0 ).toInt();
                        int  fat_fis_satiri_id   = FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_SATIRI_ID( irs_fis_satiri_id );
                        t_query.PREPARE_SELECT( "fat_irs_satir_baglantisi", "irsaliye_satiri_id", "fatura_satiri_id = :fatura_satiri_id");
                        t_query.SET_VALUE     ( ":fatura_satiri_id", fat_fis_satiri_id );

                        double fat_str_toplam_irs_miktar = 0.00;
                        if ( t_query.SELECT() NE 0 ) {
                            while ( t_query.NEXT() EQ true ) {
                                int  irsaliye_fis_satiri_id = t_query.VALUE( 0 ).toInt();
                                STK_IRS_CLEAR_SATIR_STRUCT( &IRS_FIS_SATIRI );
                                STK_IRS_SATIR_BILGILERINI_OKU( irsaliye_fis_satiri_id, &IRS_FIS_SATIRI );

                                IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE( irsaliye_fis_satiri_id, IRS_FIS_SATIRI.satirdaki_urun_miktari, ENUM_IRS_FATURALANAN_MIKTAR_GUNCELLE );
                                fat_str_toplam_irs_miktar += IRS_FIS_SATIRI.satirdaki_urun_miktari;

                                IRS_IRSALIYE_FATURALANMIS_DURUMUNU_GUNCELLE( STK_GET_IRS_ID( irsaliye_fis_satiri_id ), 1 );

                            }
                        }
                        FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE( fat_fis_satiri_id, fat_str_toplam_irs_miktar, ENUM_FAT_IRSALIYELENEN_MIKTAR_GUNCELLE );
                        IRS_IRSALIYE_FATURALANMIS_DURUMUNU_GUNCELLE( FAT_GET_FATURA_ID( fat_fis_satiri_id ), 1 );
                    }
                }

                break;
                }
                case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS :
                case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS:  {
                // fat_irs_baglantisi TABLOSUNDA N-1 BAGLANTI VARDIR
                s_query.PREPARE_SELECT( "stk_fis_satirlari", "fis_satiri_id", "fis_id = :fis_id" );
                s_query.SET_VALUE     ( ":fis_id", irsaliye_id );

                if ( s_query.SELECT() NE 0 ) {
                    while ( s_query.NEXT() EQ true ) {
                        int  irs_fis_satiri_id   = s_query.VALUE( 0 ).toInt();
                        t_query.PREPARE_SELECT( "fat_irs_satir_baglantisi", "fatura_satiri_id", "irsaliye_satiri_id = :irsaliye_satiri_id");
                        t_query.SET_VALUE     ( ":irsaliye_satiri_id", irs_fis_satiri_id );

                        double irs_str_toplam_fat_miktar = 0.00;
                        if ( t_query.SELECT() NE 0 ) {
                            while ( t_query.NEXT() EQ true ) {
                                int  fat_fis_satiri_id = t_query.VALUE( 0 ).toInt();
                                FATURA_CLEAR_FIS_SATIRI_STRUCT  ( &FAT_FIS_SATIRI );
                                FATURA_SATIR_BILGILERINI_OKU    ( &FAT_FIS_SATIRI, fat_fis_satiri_id );

                                FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE( fat_fis_satiri_id, FAT_FIS_SATIRI.satirdaki_urun_miktari, ENUM_FAT_IRSALIYELENEN_MIKTAR_GUNCELLE );
                                irs_str_toplam_fat_miktar += FAT_FIS_SATIRI.satirdaki_urun_miktari;
                                IRS_IRSALIYE_FATURALANMIS_DURUMUNU_GUNCELLE( FAT_GET_FATURA_ID( fat_fis_satiri_id ), 1 );
                            }
                        }

                        IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE( irs_fis_satiri_id, irs_str_toplam_fat_miktar , ENUM_IRS_FATURALANAN_MIKTAR_GUNCELLE );
                        IRS_IRSALIYE_FATURALANMIS_DURUMUNU_GUNCELLE( STK_GET_IRS_ID( irs_fis_satiri_id ), 1 );
                    }
                }
                break;
                }

                default:
                break;
            }
        }
    }
}

/**************************************************************************************
                   HATALI_FISLERIN_ENT_FISLERINI_YOK_ET
***************************************************************************************/

void HATALI_FISLERIN_ENT_FISLERINI_YOK_ET()
{
    SQL_QUERY query( DB );

    FATURA_FISI_STRUCT FAT_FIS;

    FAT_FIS.KDV_ORANLARI_ARRAY                     =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_HESAPLARI_ID_ARRAY                 =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY        =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.OTV_ORANLARI_ARRAY                     =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_HESAPLARI_ID_ARRAY                 =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY        =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY      =   new int     [KDV_OTV_ORANLARI_SAYISI];

    // IPTAL FATURALARI
    query.PREPARE_SELECT( "fat_faturalar", "fis_id", " hatali_fatura_mi = :hatali_fatura_mi" );
    query.SET_VALUE     ( ":hatali_fatura_mi", 1 );

    if ( query.SELECT() NE 0 ) {
        while( query.NEXT() EQ true ){

            int fat_fis_id = query.VALUE( 0 ).toInt();

            FATURA_CLEAR_FIS_STRUCT       ( &FAT_FIS );
            FATURA_FIS_BILGILERINI_OKU( &FAT_FIS, fat_fis_id );

            FATURA_FISI_SIL( &FAT_FIS, fat_fis_id );
        }
    }

    STK_IRS_FIS_STRUCT IRS_FIS;
    // IPTAL IRSALIYELERI
    query.PREPARE_SELECT( "stk_fisler", "fis_id", " hatali_irsaliye_mi = :hatali_irsaliye_mi AND irsaliye_mi_stok_fisi_mi = :irsaliye_mi_stok_fisi_mi" );
    query.SET_VALUE     ( ":hatali_irsaliye_mi"      , 1 );
    query.SET_VALUE     ( ":irsaliye_mi_stok_fisi_mi", ENUM_IRSALIYE );

    if ( query.SELECT() NE 0 ) {
        while( query.NEXT() EQ true ){

            int irs_fis_id = query.VALUE( 0 ).toInt();

            STK_IRS_CLEAR_FIS_STRUCT( &IRS_FIS );
            STK_IRS_FIS_BILGILERINI_OKU( irs_fis_id, &IRS_FIS );

            STK_IRS_FIS_SIL( irs_fis_id, &IRS_FIS );
        }
    }

    CARI_FIS_STRUCT CAR_FIS;
    // IPTAL TAHSILAT
    // IPTAL ODEME

    query.PREPARE_SELECT( "car_fisler", "fis_id", "iptal_mi = :iptal_mi AND ( fis_turu = :fis_turu_1 OR fis_turu = :fis_turu_2 ) ");
    query.SET_VALUE     ( ":iptal_mi", 1 );
    query.SET_VALUE     ( ":fis_turu_1", ENUM_CARI_TAHSILAT_FISI );
    query.SET_VALUE     ( ":fis_turu_2", ENUM_CARI_ODEME_FISI    );

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            int fis_id = query.VALUE( 0 ).toInt();

            CARI_CLEAR_FIS_STRUCT( &CAR_FIS );
            CARI_FIS_BILGILERINI_OKU( &CAR_FIS, fis_id );

            CARI_FISI_SIL( &CAR_FIS, fis_id );
        }
    }
}

/**************************************************************************************
                   CEK_SON_DURUMLARI_DUZELT
***************************************************************************************/

void CEK_SON_DURUMLARI_DUZELT()
{
    SQL_QUERY f_query(DB);
    SQL_QUERY s_query(DB);

    f_query.PREPARE_SELECT("cek_cekler_senetler", "cek_senet_id, cek_senet_son_durumu", "");
    if ( f_query.SELECT() NE 0 ) {
        while( f_query.NEXT() EQ true ) {
            int cek_senet_id         = f_query.VALUE(0).toInt();
            int cek_senet_son_durumu = f_query.VALUE(1).toInt();

            int hareketteki_durum = CEK_SON_CEK_SENET_HAREKETINI_BUL(cek_senet_id);

            if ( hareketteki_durum EQ cek_senet_son_durumu ) {
                continue;
            }

            s_query.PREPARE_UPDATE("cek_cekler_senetler", "cek_senet_id", "cek_senet_son_durumu", "cek_senet_id = :cek_senet_id");
            s_query.SET_VALUE     (":cek_senet_son_durumu", hareketteki_durum );
            s_query.SET_VALUE     (":cek_senet_id", cek_senet_id );
            s_query.UPDATE();

        }
    }
}

/**************************************************************************************
                   TUM_CEK_DURUMLARINI_ONAR
***************************************************************************************/

void TUM_CEK_DURUMLARINI_ONAR()
{
    /*
      ONCE TUM HAREKETLER SILINIYOR.
      CEK BORDROLAR UZERINDE DOLASILARAK TARIHE GORE CEKLERIN DURUMLARI
      EKLENIYOR.
      ONAR SIRASINDA ISE DURUMLARA DOKUNULMUYOR.
     */

    SQL_QUERY query  ( DB );
    SQL_QUERY s_query( DB );
    SQL_QUERY t_query( DB );

    //! TUM HAREKETLER SILINIYOR
    query.PREPARE_DELETE( "cek_hareketleri", "" );
    query.DELETE();

    query.PREPARE_SELECT( "cek_cekler_senetler", "cek_senet_id, keside_tarihi ", "" );
    if( query.SELECT() NE 0 ) {
        while( query.NEXT() EQ true ) {
            //! ILK CEK HAREKETLERI EKLENIYOR.
            int cek_senet_id   = query.VALUE(0).toInt();

            //! CEK E ISLEM YAPILMIS MI ?

            s_query.PREPARE_SELECT( "cek_bordrolar, cek_bordro_satirlari", "cek_bordrolar.bordro_id, bordro_satiri_id, bordro_tarihi",
                                    "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND cek_senet_id = :cek_senet_id");
            s_query.SET_VALUE     ( ":cek_senet_id", cek_senet_id );

            if ( s_query.SELECT("bordro_tarihi") NE 0 ) {
                while( s_query.NEXT() EQ true ) {
                    int bordro_id = s_query.VALUE(0).toInt();

                    t_query.PREPARE_SELECT( "cek_bordrolar", "bordro_islem_turu, bordro_tarihi", "bordro_id = :bordro_id" );
                    t_query.SET_VALUE     ( ":bordro_id", bordro_id );

                    if ( t_query.SELECT() NE 0 ) {
                        t_query.NEXT();

                        int bordro_islem_turu = t_query.VALUE(0).toInt();
                        QString bordro_tarihi = t_query.VALUE(1).toString();

                        int cekin_durumu = CEK_ISLEM_TURUNE_GORE_CEKIN_DURUMUNU_AL ( bordro_islem_turu );

                        CEK_SENET_HAREKETINI_KAYDET ( cek_senet_id, bordro_islem_turu, QDate::fromString( bordro_tarihi, "yyyy.MM.dd" ));

                        CEK_SENET_DURUMUNU_GUNCELLE ( cek_senet_id , cekin_durumu );
                    }
                }
            }
        }
    }
}
