#include "isl_fis_utils.h"
#include "adak_sql.h"
#include "isl_console_utils.h"
#include "adak_utils.h"
#include "muh_fis_utils.h"
#include "muh_enum.h"
#include "bnk_fis_utils.h"
#include "e9_enum.h"
#include "banka_enum.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "isl_enum.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                                    ISL_FISI_EKLE
***************************************************************************************/

int ISL_FISI_EKLE(ISL_FIS_STRUCT * P_FIS , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    if ( P_FIS->fis_no EQ 0 ) {
        P_FIS->fis_no = ISL_SIRADAKI_FIS_NO_AL(P_FIS->fis_tarihi , USE_DB);
    }

    query.PREPARE_INSERT("isl_fisi" , "fis_id" , "fis_no,fis_tarihi,fis_turu,aciklama,"
                         "kdv_haric_toplam,kdv_dahil_toplam,kdv_toplam,modul_id,"
                         "program_id,base_fis_id, last_order_number");

    query.SET_VALUE(":fis_no"              , P_FIS->fis_no);
    query.SET_VALUE(":fis_tarihi"          , P_FIS->fis_tarihi);
    query.SET_VALUE(":fis_turu"            , P_FIS->fis_turu);
    query.SET_VALUE(":aciklama"            , P_FIS->aciklama );
    query.SET_VALUE(":kdv_haric_toplam"    , 0);
    query.SET_VALUE(":kdv_dahil_toplam"    , 0);
    query.SET_VALUE(":kdv_toplam"          , 0);
    query.SET_VALUE(":modul_id"            , P_FIS->modul_id);
    query.SET_VALUE(":program_id"          , P_FIS->program_id);
    query.SET_VALUE(":base_fis_id"         , P_FIS->base_fis_id);
    query.SET_VALUE(":last_order_number"   , 0);

    return query.INSERT();
}
/**************************************************************************************
                                    ISL_FIS_SATIRI_EKLE
***************************************************************************************/

int ISL_FIS_SATIRI_EKLE( ISL_FIS_STRUCT * P_FIS, ISL_FIS_SATIRI_STRUCT * P_FIS_SATIRI, ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("isl_fisi" , "last_order_number,kdv_haric_toplam,kdv_toplam,"
                         "kdv_dahil_toplam,modul_id,program_id,base_fis_id" ,
                         "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , P_FIS_SATIRI->fis_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    int    last_order_number = query.VALUE(0).toInt();
    double kdv_haric_toplam  = query.VALUE(1).toDouble();
    double kdv_toplam        = query.VALUE(2).toDouble();
    double kdv_dahil_toplam  = query.VALUE(3).toDouble();
    int    modul_id          = query.VALUE(4).toInt();
    int    program_id        = query.VALUE(5).toInt();
    int    base_fis_id       = query.VALUE(6).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR
         P_FIS->base_fis_id NE base_fis_id ) {
        return 0;
    }

    if ( P_FIS_SATIRI->order_number EQ 0 ) {
        P_FIS_SATIRI->order_number = last_order_number + 1024;
    }


    query.PREPARE_INSERT("isl_fis_satirlari" , "fis_satiri_id" , "fis_id,order_number,"
                         "belge_no,aciklama , gider_turu , kdv_orani , satir_kdv_haric_tutar,"
                         "satir_kdv_tutari , satir_kdv_dahil_tutar , program_id,modul_id,"
                         "base_record_id ");
    query.SET_VALUE(":fis_id"                , P_FIS_SATIRI->fis_id);
    query.SET_VALUE(":order_number"          , P_FIS_SATIRI->order_number);
    query.SET_VALUE(":belge_no"              , P_FIS_SATIRI->belge_no);
    query.SET_VALUE(":aciklama"              , P_FIS_SATIRI->aciklama);
    query.SET_VALUE(":gider_turu"            , P_FIS_SATIRI->gider_turu);
    query.SET_VALUE(":kdv_orani"             , P_FIS_SATIRI->kdv_orani);
    query.SET_VALUE(":satir_kdv_haric_tutar" , P_FIS_SATIRI->satir_kdv_haric_tutar);
    query.SET_VALUE(":satir_kdv_tutari"      , P_FIS_SATIRI->satir_kdv_tutari);
    query.SET_VALUE(":satir_kdv_dahil_tutar" , P_FIS_SATIRI->satir_kdv_dahil_tutar);
    query.SET_VALUE(":program_id"            , P_FIS_SATIRI->program_id);
    query.SET_VALUE(":modul_id"              , P_FIS_SATIRI->modul_id);
    query.SET_VALUE(":base_record_id"        , P_FIS_SATIRI->base_record_id);

    int fis_satiri_id = query.INSERT();

    kdv_haric_toplam += P_FIS_SATIRI->satir_kdv_haric_tutar;
    kdv_toplam       += P_FIS_SATIRI->satir_kdv_tutari;
    kdv_dahil_toplam += P_FIS_SATIRI->satir_kdv_dahil_tutar;

    query.PREPARE_UPDATE("isl_fisi" , "fis_id" , "last_order_number,kdv_haric_toplam,"
                         "kdv_toplam,kdv_dahil_toplam" ,"fis_id = :fis_id");

    query.SET_VALUE(":last_order_number", P_FIS_SATIRI->order_number);
    query.SET_VALUE(":kdv_haric_toplam" , ROUND(kdv_haric_toplam ,-2));
    query.SET_VALUE(":kdv_toplam"       , ROUND(kdv_toplam       ,-2));
    query.SET_VALUE(":kdv_dahil_toplam" , ROUND(kdv_dahil_toplam ,-2));
    query.SET_VALUE(":fis_id"           , P_FIS_SATIRI->fis_id);

    query.UPDATE();

    P_FIS->kdv_dahil_toplam = kdv_dahil_toplam;
    P_FIS->kdv_haric_toplam = kdv_haric_toplam;
    P_FIS->kdv_toplam       = kdv_toplam;

    //ISL_ENTEGRASYON_SATIRI_EKLE(P_FIS , P_FIS_SATIRI , fis_satiri_id , USE_DB);

    return fis_satiri_id;

}
/**************************************************************************************
                                    ISL_FISI_GUNCELLE
***************************************************************************************/

void ISL_FISI_GUNCELLE(ISL_FIS_STRUCT *P_FIS, int p_fis_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("isl_fisi" , "modul_id,program_id,base_fis_id ", "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    int modul_id          = query.VALUE(0).toInt();
    int program_id        = query.VALUE(1).toInt();
    int base_fis_id       = query.VALUE(2).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR
         P_FIS->base_fis_id NE base_fis_id ) {
        return;
    }

    query.PREPARE_UPDATE("isl_fisi" , "fis_id" , "fis_no , fis_tarihi , aciklama ",
                         "fis_id = :fis_id");
    query.SET_VALUE(":fis_no"              , P_FIS->fis_no);
    query.SET_VALUE(":fis_tarihi"          , P_FIS->fis_tarihi);
    query.SET_VALUE(":aciklama"            , P_FIS->aciklama);
    query.SET_VALUE(":fis_id"              , p_fis_id);

    query.UPDATE();
}

/**************************************************************************************
                                    ISL_FIS_SATIRI_GUNCELLE
***************************************************************************************/

void ISL_FIS_SATIRI_GUNCELLE( ISL_FIS_STRUCT *P_FIS, ISL_FIS_SATIRI_STRUCT *P_FIS_SATIRI,
                             int p_fis_satiri_id)
{

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("isl_fisi,isl_fis_satirlari" ,"isl_fisi.modul_id,isl_fisi.program_id,"
                         "base_fis_id,kdv_haric_toplam,kdv_toplam,kdv_dahil_toplam,"
                         "satir_kdv_haric_tutar,satir_kdv_tutari , satir_kdv_dahil_tutar",
                         "isl_fisi.fis_id = isl_fis_satirlari.fis_id AND "
                         "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int modul_id    = query.VALUE(0).toInt();
    int program_id  = query.VALUE(1).toInt();
    int base_fis_id = query.VALUE(2).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR
         P_FIS->base_fis_id NE base_fis_id ) {
        return;
    }

    double kdv_haric_toplam             = query.VALUE(3).toDouble();
    double kdv_toplam                   = query.VALUE(4).toDouble();
    double kdv_dahil_toplam             = query.VALUE(5).toDouble();
    double kdv_haric_tutar              = query.VALUE(6).toDouble();
    double kdv_tutari                   = query.VALUE(7).toDouble();
    double kdv_dahil_tutar              = query.VALUE(8).toDouble();



    kdv_haric_toplam -= kdv_haric_tutar;
    kdv_toplam       -= kdv_tutari;
    kdv_dahil_toplam -= kdv_dahil_tutar;

    query.PREPARE_UPDATE("isl_fis_satirlari" , "fis_satiri_id" , "belge_no , aciklama,"
                         "gider_turu ,kdv_orani , satir_kdv_haric_tutar ,satir_kdv_tutari,"
                         "satir_kdv_dahil_tutar ",
                         "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":belge_no"                     , P_FIS_SATIRI->belge_no);
    query.SET_VALUE(":aciklama"                     , P_FIS_SATIRI->aciklama);
    query.SET_VALUE(":gider_turu"                   , P_FIS_SATIRI->gider_turu);
    query.SET_VALUE(":kdv_orani"                    , P_FIS_SATIRI->kdv_orani);
    query.SET_VALUE(":satir_kdv_haric_tutar"        , P_FIS_SATIRI->satir_kdv_haric_tutar);
    query.SET_VALUE(":satir_kdv_tutari"             , P_FIS_SATIRI->satir_kdv_tutari);
    query.SET_VALUE(":satir_kdv_dahil_tutar"        , P_FIS_SATIRI->satir_kdv_dahil_tutar);
    query.SET_VALUE(":fis_satiri_id"                , p_fis_satiri_id);

    query.UPDATE();

    kdv_haric_toplam += P_FIS_SATIRI->satir_kdv_haric_tutar;
    kdv_toplam       += P_FIS_SATIRI->satir_kdv_tutari;
    kdv_dahil_toplam += P_FIS_SATIRI->satir_kdv_dahil_tutar;

    query.PREPARE_UPDATE("isl_fisi" , "fis_id","kdv_haric_toplam , kdv_toplam , kdv_dahil_toplam" ,
                         "fis_id = :fis_id");
    query.SET_VALUE(":kdv_haric_toplam" , ROUND(kdv_haric_toplam , -2));
    query.SET_VALUE(":kdv_toplam"       , ROUND(kdv_toplam       , -2));
    query.SET_VALUE(":kdv_dahil_toplam" , ROUND(kdv_dahil_toplam , -2));
    query.SET_VALUE(":fis_id"           , P_FIS_SATIRI->fis_id        );

    query.UPDATE();

    P_FIS->kdv_haric_toplam = kdv_haric_toplam;
    P_FIS->kdv_toplam       = kdv_toplam;
    P_FIS->kdv_dahil_toplam = kdv_dahil_toplam;

    //ISL_ENTEGRASYON_SATIRI_EKLE(P_FIS, P_FIS_SATIRI, p_fis_satiri_id );

}
/**************************************************************************************
                                    ISL_FISI_SIL
***************************************************************************************/

void ISL_FISI_SIL(ISL_FIS_STRUCT *P_FIS, int p_fis_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("isl_fisi" , "modul_id ,program_id , base_fis_id" , "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int modul_id    = query.VALUE(0).toInt();
    int program_id  = query.VALUE(1).toInt();
    int base_fis_id = query.VALUE(2).toInt();

    if ( P_FIS->program_id NE program_id OR P_FIS->modul_id NE modul_id OR
         P_FIS->base_fis_id NE base_fis_id ) {
        return;
    }

    query.PREPARE_SELECT("isl_fis_satirlari" ,"fis_satiri_id" , "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() ) {

            int fis_satiri_id = query.VALUE(0).toInt();

            ISL_FIS_SATIRI_SIL(P_FIS , fis_satiri_id);
        }
    }

    query.PREPARE_DELETE("isl_fisi" ,"fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);
    query.DELETE();
}
/**************************************************************************************
                                    ISL_FIS_SATIRI_SIL
***************************************************************************************/

void ISL_FIS_SATIRI_SIL(ISL_FIS_STRUCT *P_FIS, int p_fis_satiri_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("isl_fisi,isl_fis_satirlari" , "isl_fisi.modul_id,isl_fisi.program_id,"
                         "isl_fisi.base_fis_id,kdv_haric_toplam,kdv_toplam,kdv_dahil_toplam,"
                         "satir_kdv_haric_tutar,satir_kdv_tutari,satir_kdv_dahil_tutar, isl_fisi.fis_id ",
                         "isl_fisi.fis_id = isl_fis_satirlari.fis_id AND fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int modul_id    = query.VALUE(0).toInt();
    int program_id  = query.VALUE(1).toInt();
    int base_fis_id = query.VALUE(2).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR
         P_FIS->base_fis_id NE base_fis_id ) {
        return;
    }

    double kdv_haric_toplam = query.VALUE(3).toDouble();
    double kdv_toplam       = query.VALUE(4).toDouble();
    double kdv_dahil_toplam = query.VALUE(5).toDouble();

    double kdv_haric_tutar  = query.VALUE(6).toDouble();
    double kdv_tutari       = query.VALUE(7).toDouble();
    double kdv_dahil_tutar  = query.VALUE(8).toDouble();

    int    fis_id           = query.VALUE(9).toInt();

    kdv_haric_toplam -= kdv_haric_tutar;
    kdv_dahil_toplam -= kdv_dahil_tutar;
    kdv_toplam       -= kdv_tutari;

    query.PREPARE_DELETE("isl_fis_satirlari" ,"fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);
    query.DELETE();

    query.PREPARE_UPDATE("isl_fisi" ,"fis_id" , "kdv_haric_toplam,kdv_toplam,"
                         "kdv_dahil_toplam" ,"fis_id = :fis_id");
    query.SET_VALUE(":kdv_haric_toplam" , ROUND(kdv_haric_toplam ,-2));
    query.SET_VALUE(":kdv_toplam"       , ROUND(kdv_toplam ,-2)      );
    query.SET_VALUE(":kdv_dahil_toplam" , ROUND(kdv_dahil_toplam , -2));
    query.SET_VALUE(":fis_id"           , fis_id);
    query.UPDATE();

    P_FIS->kdv_haric_toplam = kdv_haric_toplam;
    P_FIS->kdv_dahil_toplam = kdv_dahil_toplam;
    P_FIS->kdv_toplam       = kdv_toplam;

}
/**************************************************************************************
                                    ISL_FIS_BILGILERINI_OKU
***************************************************************************************/

void ISL_FIS_BILGILERINI_OKU(ISL_FIS_STRUCT *P_FIS, int p_fis_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("isl_fisi" , "fis_no , fis_tarihi , fis_turu , aciklama,"
                         "kdv_haric_toplam,kdv_dahil_toplam,kdv_toplam,"
                         "modul_id,program_id,base_fis_id",
                         "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() EQ  0 ) {
        return;
    }

    query.NEXT();

    P_FIS->fis_no              = query.VALUE("fis_no").toInt();
    P_FIS->fis_tarihi          = query.VALUE("fis_tarihi").toString();
    P_FIS->fis_turu            = query.VALUE("fis_turu").toInt();
    P_FIS->aciklama            = query.VALUE("aciklama").toString();
    P_FIS->kdv_haric_toplam    = query.VALUE("kdv_haric_toplam").toDouble();
    P_FIS->kdv_toplam          = query.VALUE("kdv_toplam").toDouble();
    P_FIS->kdv_dahil_toplam    = query.VALUE("kdv_dahil_toplam").toDouble();
    P_FIS->modul_id            = query.VALUE("modul_id").toInt();
    P_FIS->program_id          = query.VALUE("program_id").toInt();
    P_FIS->base_fis_id         = query.VALUE("base_fis_id").toInt();

}

/**************************************************************************************
                                    ISL_FIS_SATIRI_BILGILERINI_OKU
***************************************************************************************/

void ISL_FIS_SATIRI_BILGILERINI_OKU(ISL_FIS_SATIRI_STRUCT *P_FIS_SATIRI, int p_fis_satiri_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("isl_fis_satirlari" , "fis_id , order_number,belge_no,aciklama,"
                         "gider_turu , kdv_orani , satir_kdv_haric_tutar,satir_kdv_tutari,"
                         "satir_kdv_dahil_tutar , modul_id , program_id , base_record_id ",
                         "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    P_FIS_SATIRI->fis_id                = query.VALUE("fis_id").toInt();
    P_FIS_SATIRI->order_number          = query.VALUE("order_number").toInt();
    P_FIS_SATIRI->belge_no              = query.VALUE("belge_no").toString();
    P_FIS_SATIRI->aciklama              = query.VALUE("aciklama").toString();
    P_FIS_SATIRI->gider_turu            = query.VALUE("gider_turu").toInt();
    P_FIS_SATIRI->kdv_orani             = query.VALUE("kdv_orani").toDouble();
    P_FIS_SATIRI->satir_kdv_haric_tutar = query.VALUE("satir_kdv_haric_tutar").toDouble();
    P_FIS_SATIRI->satir_kdv_tutari      = query.VALUE("satir_kdv_tutari").toDouble();
    P_FIS_SATIRI->satir_kdv_dahil_tutar = query.VALUE("satir_kdv_dahil_tutar").toDouble();
    P_FIS_SATIRI->modul_id              = query.VALUE("modul_id").toInt();
    P_FIS_SATIRI->program_id            = query.VALUE("program_id").toInt();
    P_FIS_SATIRI->base_record_id        = query.VALUE("base_record_id").toInt();
}

/**************************************************************************************
                                    ISL_CLEAR_FIS_STRUCT
***************************************************************************************/

void ISL_CLEAR_FIS_STRUCT(ISL_FIS_STRUCT *P_FIS)
{
    P_FIS->aciklama.clear();
    P_FIS->fis_tarihi.clear();
    P_FIS->base_fis_id      = 0;
    P_FIS->fis_no           = 0;
    P_FIS->fis_turu         = -1;
    P_FIS->kdv_dahil_toplam = 0;
    P_FIS->kdv_haric_toplam = 0;
    P_FIS->kdv_toplam       = 0;
    P_FIS->modul_id         = -1;
    P_FIS->program_id       = -1;
    P_FIS->last_order_number   = 0;
}

/**************************************************************************************
                                    ISL_CLEAR_FIS_SATIRI_STRUCT
***************************************************************************************/

void ISL_CLEAR_FIS_SATIRI_STRUCT(ISL_FIS_SATIRI_STRUCT *P_FIS_SATIRI)
{
    P_FIS_SATIRI->aciklama.clear();
    P_FIS_SATIRI->belge_no.clear();
    P_FIS_SATIRI->base_record_id        = 0;
    P_FIS_SATIRI->fis_id                = 0;
    P_FIS_SATIRI->gider_turu            = -1;
    P_FIS_SATIRI->kdv_orani             = 0;
    P_FIS_SATIRI->modul_id              = -1;
    P_FIS_SATIRI->program_id            = -1;
    P_FIS_SATIRI->order_number          = 0;
    P_FIS_SATIRI->satir_kdv_dahil_tutar = 0;
    P_FIS_SATIRI->satir_kdv_haric_tutar = 0;
    P_FIS_SATIRI->satir_kdv_tutari      = 0;

}

/**************************************************************************************
                          ISL_GET_ENT_FIS_ID
***************************************************************************************/

int ISL_GET_ENT_FIS_ID(int p_modul_id, int p_program_id, int p_base_fis_id , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("isl_fisi" , "fis_id" , "modul_id = :modul_id AND program_id = :program_id "
                         "AND base_fis_id = :base_fis_id");
    query.SET_VALUE(":modul_id"    , p_modul_id);
    query.SET_VALUE(":program_id"  , p_program_id);
    query.SET_VALUE(":base_fis_id" , p_base_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}


/**************************************************************************************
                          ISL_GET_ENT_FIS_SATIRI_ID
***************************************************************************************/

int ISL_GET_ENT_FIS_SATIRI_ID(int p_modul_id, int p_program_id, int p_base_record_id)
{
    SQL_QUERY query( DB);

    query.PREPARE_SELECT("isl_fis_satirlari" , "fis_satiri_id" , "modul_id = :modul_id "
                         "AND program_id = :program_id AND base_record_id = :base_record_id");

    query.SET_VALUE(":modul_id"       , p_modul_id);
    query.SET_VALUE(":program_id"     , p_program_id);
    query.SET_VALUE(":base_record_id" , p_base_record_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();

}

/**************************************************************************************
                          ISL_ENTEGRASYON_SATIRI_EKLE
***************************************************************************************/

void ISL_ENTEGRASYON_SATIRI_EKLE(ISL_FIS_STRUCT *P_FIS, ISL_FIS_SATIRI_STRUCT *P_FIS_SATIRI,
                                 int p_fis_satiri_id, ADAK_SQL * P_GELEN_DB)
{
    Q_UNUSED(P_FIS);
    Q_UNUSED(P_FIS_SATIRI);
    Q_UNUSED(p_fis_satiri_id);
    Q_UNUSED(P_GELEN_DB);
//    ADAK_SQL * USE_DB;

//    if ( P_GELEN_DB EQ NULL ) {
//        USE_DB = DB;
//    }
//    else {
//        USE_DB = P_GELEN_DB;
//    }

//    BNK_HAREKET_FIS_STRUCT              * BNK_FISI       = new BNK_HAREKET_FIS_STRUCT;
//    BNK_HAREKET_FIS_SATIRI_STRUCT       * BNK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

//    BNK_CLEAR_HAREKET_FIS_STRUCT        ( BNK_FISI );
//    BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT ( BNK_FIS_SATIRI );


//    MUH_FIS_STRUCT          * MUHASEBE_FISI  = new MUH_FIS_STRUCT;
//    MUH_FIS_SATIRI_STRUCT   * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;


//    MUH_CLEAR_FIS_STRUCT            ( MUHASEBE_FISI );
//    MUH_CLEAR_FIS_SATIRI_STRUCT     ( MUH_FIS_SATIRI );

//    MUH_FIS_BILGILERINI_OKU ( MUHASEBE_FISI , P_FIS->muh_fis_id , USE_DB);

//    int muh_kasa_satiri_id        = 0;
//    int muh_kdv_satiri_id         = 0;
//    int muh_gelir_gider_satiri_id = 0;

//    int base_record_id = P_FIS_SATIRI->base_record_id;

//    if ( base_record_id EQ 0 ) {
//        base_record_id = p_fis_satiri_id;
//    }

//    SQL_QUERY query( USE_DB );

//    if ( P_FIS_SATIRI->tah_odm_sekli NE ENUM_NAKIT ) {

//        int bnk_fis_id = BNK_GET_ENT_FIS_ID ( P_FIS->program_id , P_FIS->modul_id , P_FIS_SATIRI->fis_id , P_FIS_SATIRI->hesap_no_id, USE_DB);

//        if ( bnk_fis_id EQ 0 ) {

//            BNK_FISI->base_fis_id      = P_FIS_SATIRI->fis_id;
//            BNK_FISI->program_id       = P_FIS->program_id;
//            BNK_FISI->modul_id         = P_FIS->modul_id;
//            BNK_FISI->hesap_no_id      = P_FIS_SATIRI->hesap_no_id;

//            QString fis_aciklama         = QObject::tr("Fiş İşletme Modülü Tarafından Üretilmiştir.Fiş No : %1\n").arg(P_FIS->fis_no);
//            fis_aciklama.append(P_FIS->aciklama);

//            BNK_FISI->aciklama       = fis_aciklama;
//            BNK_FISI->fis_tarihi     = P_FIS->fis_tarihi;
//            BNK_FISI->fis_turu       = ENUM_BANKA_HAREKET_FISI;
//            BNK_FISI->muh_fis_id     = P_FIS->muh_fis_id;
//            BNK_FISI->odm_emr_hesabi_etkilenecek_mi = 1;
//            BNK_FISI->para_birim_id  = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

//            bnk_fis_id = BNK_HAREKET_FIS_EKLE(BNK_FISI ,0,true , USE_DB);
//        }
//        else {
//            BNK_HAREKET_FIS_BILGILERINI_OKU ( BNK_FISI , bnk_fis_id , USE_DB);
//            BNK_FISI->hesap_no_id = P_FIS_SATIRI->hesap_no_id;
//        }


//        BNK_FIS_SATIRI->fis_id                      = bnk_fis_id;
//        BNK_FIS_SATIRI->program_id                  = P_FIS_SATIRI->program_id;
//        BNK_FIS_SATIRI->modul_id                    = P_FIS_SATIRI->modul_id;
//        BNK_FIS_SATIRI->base_record_id              = base_record_id;
//        BNK_FIS_SATIRI->aciklama                    = P_FIS_SATIRI->aciklama;
//        BNK_FIS_SATIRI->karsi_hesap_eklenecek_mi    = 0;



//        switch ( P_FIS_SATIRI->tah_odm_sekli ) {
//        case ENUM_KREDI_KARTI : {
//                if ( P_FIS->fis_turu EQ ENUM_GIDER_FISI ) {
//                    BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_KREDI_KARTI_HESABI;
//                    BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->kredi_karti_id;
//                }
//                else {
//                    BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_POS_HESABI;
//                    BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->pos_id;
//                }
//            }
//            break;
//        case ENUM_EFT    :
//        case ENUM_HAVALE : {
//                BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
//                BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->hesap_no_id;
//            }
//            break;
//        default:
//            break;
//        }
//        if ( P_FIS->fis_turu EQ ENUM_GIDER_FISI ) {
//            BNK_FIS_SATIRI->hesaba_giren   = 0.0;
//            BNK_FIS_SATIRI->hesaptan_cikan = P_FIS_SATIRI->satir_kdv_dahil_tutar;
//        }
//        else {
//            BNK_FIS_SATIRI->hesaba_giren   = P_FIS_SATIRI->satir_kdv_dahil_tutar;
//            BNK_FIS_SATIRI->hesaptan_cikan = 0.0;
//        }
//        if ( P_FIS->fis_turu EQ ENUM_GELIR_FISI ) {
//            BNK_HAREKET_FIS_SATIRI_EKLE(BNK_FISI , BNK_FIS_SATIRI , 0 , true , USE_DB);
//        }
//    }
//    else {
//        //NAKIT ISE

//        MUH_FIS_SATIRI->hesap_id            = P_FIS->muh_kasa_hesap_id;
//        MUH_FIS_SATIRI->aciklama            = P_FIS_SATIRI->aciklama;
//        MUH_FIS_SATIRI->base_record_id      = base_record_id;
//        MUH_FIS_SATIRI->fis_id              = P_FIS->muh_fis_id;
//        MUH_FIS_SATIRI->modul_id            = MUHASEBE_FISI->modul_id;
//        MUH_FIS_SATIRI->program_id          = MUHASEBE_FISI->program_id;

//        if ( P_FIS->fis_turu EQ ENUM_GIDER_FISI ) {
//            MUH_FIS_SATIRI->borc_tutari      = 0.0;
//            MUH_FIS_SATIRI->alacak_tutari    = P_FIS_SATIRI->satir_kdv_dahil_tutar;
//        }
//        else {
//            MUH_FIS_SATIRI->borc_tutari      = P_FIS_SATIRI->satir_kdv_dahil_tutar;
//            MUH_FIS_SATIRI->alacak_tutari    = 0.0;
//            muh_kasa_satiri_id = MUH_FIS_SATIRI_EKLE ( MUHASEBE_FISI , MUH_FIS_SATIRI , USE_DB);
//        }

//    }

//    MUH_FIS_SATIRI->aciklama         = P_FIS_SATIRI->aciklama;
//    MUH_FIS_SATIRI->base_record_id   = base_record_id;

//    if ( P_FIS->fis_turu EQ ENUM_GIDER_FISI ) {
//        MUH_FIS_SATIRI->borc_tutari      = P_FIS_SATIRI->satir_kdv_haric_tutar;
//        MUH_FIS_SATIRI->alacak_tutari    = 0;
//    }
//    else {
//        MUH_FIS_SATIRI->alacak_tutari    = P_FIS_SATIRI->satir_kdv_haric_tutar;
//        MUH_FIS_SATIRI->borc_tutari      = 0;
//    }
//    MUH_FIS_SATIRI->fis_id                    = P_FIS->muh_fis_id;
//    MUH_FIS_SATIRI->hesap_id                  = P_FIS->muh_hesap_id;
//    MUH_FIS_SATIRI->modul_id                  = MUHASEBE_FISI->modul_id;
//    MUH_FIS_SATIRI->program_id                = MUHASEBE_FISI->program_id;

//    muh_gelir_gider_satiri_id = MUH_FIS_SATIRI_EKLE ( MUHASEBE_FISI , MUH_FIS_SATIRI ,USE_DB);

//    if ( P_FIS_SATIRI->satir_kdv_tutari > 0 ) {

//        if ( P_FIS->fis_turu EQ ENUM_GIDER_FISI ) {
//            MUH_FIS_SATIRI->borc_tutari      = P_FIS_SATIRI->satir_kdv_tutari;
//            MUH_FIS_SATIRI->alacak_tutari    = 0;
//        }
//        else {
//            MUH_FIS_SATIRI->alacak_tutari    = P_FIS_SATIRI->satir_kdv_tutari;
//            MUH_FIS_SATIRI->borc_tutari      = 0;
//        }
//        MUH_FIS_SATIRI->hesap_id             = P_FIS_SATIRI->kdv_hesap_id;


//        muh_kdv_satiri_id = MUH_FIS_SATIRI_EKLE ( MUHASEBE_FISI , MUH_FIS_SATIRI , USE_DB);
//    }


//    if ( P_FIS->fis_turu EQ ENUM_GIDER_FISI) {
//        if ( P_FIS_SATIRI->tah_odm_sekli NE ENUM_NAKIT ) {
//            BNK_HAREKET_FIS_SATIRI_EKLE(BNK_FISI , BNK_FIS_SATIRI , 0 , true ,USE_DB);
//        }
//        else {
//            MUH_FIS_SATIRI->borc_tutari      = 0.0;
//            MUH_FIS_SATIRI->alacak_tutari    = P_FIS_SATIRI->satir_kdv_dahil_tutar;
//            MUH_FIS_SATIRI->hesap_id         = P_FIS->muh_kasa_hesap_id;

//            muh_kasa_satiri_id = MUH_FIS_SATIRI_EKLE(MUHASEBE_FISI , MUH_FIS_SATIRI , USE_DB);
//        }
//    }

//    query.PREPARE_UPDATE("isl_fis_satirlari" , "fis_satiri_id" , "muh_kdv_satiri_id,"
//                         "muh_gelir_gider_satiri_id,muh_kasa_satiri_id","fis_satiri_id = :fis_satiri_id");
//    query.SET_VALUE(":muh_kdv_satiri_id"         , muh_kdv_satiri_id);
//    query.SET_VALUE(":muh_gelir_gider_satiri_id" , muh_gelir_gider_satiri_id);
//    query.SET_VALUE(":muh_kasa_satiri_id"        , muh_kasa_satiri_id);
//    query.SET_VALUE(":fis_satiri_id"             , p_fis_satiri_id);
//    query.UPDATE();
}
















