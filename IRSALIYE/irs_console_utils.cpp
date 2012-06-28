#include <QDate>
#include "adak_sql.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "irs_console_utils.h"
#include "adak_utils.h"
#include "fat_console_utils.h"
#include "e9_console_utils.h"
#include "fat_enum.h"
#include "stok_console_utils.h"
#include "sube_struct.h"
#include "fat_fis_utils.h"
#include "irs_enum.h"
#include "irs_struct.h"
#include "sube_console_utils.h"
#include "stok_fis_utils.h"

extern ADAK_SQL   *   DB;

static IRSALIYE_RAPOR_VARS * IRS_RV = new IRSALIYE_RAPOR_VARS;


/**************************************************************************************
                    IRSALIYEDE_TOPLU_ISKONTO_SONRASI_TUTARLARI_GUNCELE
***************************************************************************************/

void IRS_IRSALIYEDE_TOPLU_ISKONTO_SONRASI_TUTARLARI_GUNCELE ( FATURA_FISI_STRUCT * FAT_FIS, int p_fatura_id )
{
    SQL_QUERY query ( DB );

//    FATURA_FISI_STRUCT FAT_FIS;

//    FAT_FIS.KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
//    FAT_FIS.KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
//    FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

//    FAT_FIS.OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
//    FAT_FIS.OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
//    FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

//    FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];



//    FATURA_CLEAR_FIS_STRUCT ( &FAT_FIS );

//    int result = FATURA_FIS_BILGILERINI_OKU( &FAT_FIS, p_fatura_id );

//    if ( result EQ 0 ) {
//        return;
//    }

    int     irsaliye_id                 = STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID( p_fatura_id );
    double  ara_toplam                  = FAT_FIS->ara_toplam;                   //query.VALUE ( "ara_toplam"                    ).toDouble();
    double  isk_sonrasi_otv_tutari      = FAT_FIS->isk_sonrasi_otv_tutari;       //query.VALUE ( "isk_sonrasi_otv_tutari"        ).toDouble();
    double  isk_sonrasi_otv_dahil_tutar = FAT_FIS->isk_sonrasi_otv_dahil_tutar;  //query.VALUE ( "isk_sonrasi_otv_dahil_tutar"   ).toDouble();
    double  isk_sonrasi_kdv_tutari      = FAT_FIS->isk_oncesi_kdv_tutari;        //query.VALUE ( "isk_sonrasi_kdv_tutari"        ).toDouble();
    double  irsaliye_tutari             = FAT_FIS->fatura_tutari;                //query.VALUE ( "fatura_tutari"                 ).toDouble();
    double  iskonto_yuzdesi             = FAT_FIS->iskonto_yuzdesi;              //query.VALUE ( "iskonto_yuzdesi"               ).toDouble();
    double  iskonto_tutari              = FAT_FIS->iskonto_tutari;               //query.VALUE ( "iskonto_tutari"                ).toDouble();

    query.PREPARE_UPDATE ( "stk_fisler ","fis_id",
                           "ara_toplam ,                     "
                           "isk_sonrasi_otv_tutari ,         "
                           "isk_sonrasi_otv_dahil_tutar ,    "
                           "isk_sonrasi_kdv_tutari ,         "
                           "irsaliye_tutari ,                "
                           "iskonto_yuzdesi ,                "
                           "iskonto_tutari                   ",
                           "fis_id  = :fis_id" );

    query.SET_VALUE ( ":ara_toplam"                       , ara_toplam                  );
    query.SET_VALUE ( ":isk_sonrasi_otv_tutari"           , isk_sonrasi_otv_tutari      );
    query.SET_VALUE ( ":isk_sonrasi_otv_dahil_tutar"      , isk_sonrasi_otv_dahil_tutar );
    query.SET_VALUE ( ":isk_sonrasi_kdv_tutari"           , isk_sonrasi_kdv_tutari      );
    query.SET_VALUE ( ":irsaliye_tutari"                  , irsaliye_tutari             );
    query.SET_VALUE ( ":iskonto_yuzdesi"                  , iskonto_yuzdesi             );
    query.SET_VALUE ( ":iskonto_tutari"                   , iskonto_tutari              );
    query.SET_VALUE ( ":fis_id"                           , irsaliye_id                 );

    query.UPDATE    ();

}


/**************************************************************************************
                    IRS_SATIRINDA_TOPLU_ISKONTO_SONRASI_TUTARLARI_GUNCELE
***************************************************************************************/

void IRS_SATIRINDA_TOPLU_ISKONTO_SONRASI_TUTARLARI_GUNCELE ( int p_fatura_satiri_id )
{
    SQL_QUERY query ( DB );

    FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;

    FATURA_CLEAR_FIS_SATIRI_STRUCT ( &FAT_FIS_SATIRI );

    int result = FATURA_SATIR_BILGILERINI_OKU( &FAT_FIS_SATIRI , p_fatura_satiri_id );

    if ( result EQ 0 ) {
        return;
    }

    int     irsaliye_satiri_id                  = STK_GET_FAT_IRS_STR_BAGLANTISI_IRSALIYE_SATIRI_ID( p_fatura_satiri_id );
    double  toplu_iskonto_yuzdesi               = FAT_FIS_SATIRI.toplu_iskonto_yuzdesi;
    double  t_is_son_birim_fiyati               = FAT_FIS_SATIRI.t_is_son_birim_fiyati;
    double  t_is_son_satir_tutari               = FAT_FIS_SATIRI.t_is_son_satir_tutari;
    double  t_is_son_str_vergi_haric_tutar      = FAT_FIS_SATIRI.t_is_son_str_vergi_haric_tutar;
    double  t_is_son_satir_otv_tutari           = FAT_FIS_SATIRI.t_is_son_satir_otv_tutari;
    double  t_is_son_satir_otv_dahil_tutar      = FAT_FIS_SATIRI.t_is_son_satir_otv_dahil_tutar;
    double  t_is_son_satir_kdv_tutari           = FAT_FIS_SATIRI.t_is_son_satir_kdv_tutari;
    double  t_is_son_satir_kdv_dahil_tutar      = FAT_FIS_SATIRI.t_is_son_satir_kdv_dahil_tutar;

    query.PREPARE_UPDATE ( "stk_fis_satirlari ","fis_satiri_id",
                           "toplu_iskonto_yuzdesi,             "
                           "t_is_son_birim_fiyati,             "
                           "t_is_son_satir_tutari,             "
                           "t_is_son_str_vergi_haric_tutar  ,  "
                           "t_is_son_satir_otv_tutari,         "
                           "t_is_son_satir_otv_dahil_tutar,    "
                           "t_is_son_satir_kdv_tutari,         "
                           "t_is_son_satir_kdv_dahil_tutar     ",
                           "fis_satiri_id = :fis_satiri_id" );

    query.SET_VALUE ( ":toplu_iskonto_yuzdesi",             toplu_iskonto_yuzdesi               );
    query.SET_VALUE ( ":t_is_son_birim_fiyati",             t_is_son_birim_fiyati               );
    query.SET_VALUE ( ":t_is_son_satir_tutari",             t_is_son_satir_tutari               );
    query.SET_VALUE ( ":t_is_son_str_vergi_haric_tutar",    t_is_son_str_vergi_haric_tutar      );
    query.SET_VALUE ( ":t_is_son_satir_otv_tutari",         t_is_son_satir_otv_tutari           );
    query.SET_VALUE ( ":t_is_son_satir_otv_dahil_tutar",    t_is_son_satir_otv_dahil_tutar      );
    query.SET_VALUE ( ":t_is_son_satir_kdv_tutari",         t_is_son_satir_kdv_tutari           );
    query.SET_VALUE ( ":t_is_son_satir_kdv_dahil_tutar",    t_is_son_satir_kdv_dahil_tutar      );
    query.SET_VALUE ( ":fis_satiri_id",                     irsaliye_satiri_id                  );

    query.UPDATE    ();    
}


/**************************************************************************************
                    IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE
***************************************************************************************/

void IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_GUNCELLE ( int p_irsaliye_satiri_id, double p_faturalanan_miktar,
                                                           int p_artir_azalt , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY sql_query ( USE_DB );

    double faturalanan_miktar = 0.00;
    int    fis_id             = 0;

    sql_query.PREPARE_SELECT("stk_fis_satirlari ","faturalanan_miktar, fis_id ",
                             "fis_satiri_id = :fis_satiri_id");

    sql_query.SET_VALUE(":fis_satiri_id", p_irsaliye_satiri_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    fis_id = sql_query.VALUE( 1 ).toInt();

    if ( p_artir_azalt EQ ENUM_IRS_FATURALANAN_MIKTAR_AZALT ) {
        faturalanan_miktar = ROUND ( sql_query.VALUE(0).toDouble() - p_faturalanan_miktar );
    }
    else  if ( p_artir_azalt EQ ENUM_IRS_FATURALANAN_MIKTAR_ARTIR ){
        faturalanan_miktar = ROUND ( sql_query.VALUE(0).toDouble() + p_faturalanan_miktar );
    }
    else if ( p_artir_azalt EQ ENUM_IRS_FATURALANAN_MIKTAR_GUNCELLE ) {
        faturalanan_miktar = p_faturalanan_miktar;
    }


    sql_query.PREPARE_UPDATE("stk_fis_satirlari ","fis_satiri_id",
                             "faturalanan_miktar",
                             "fis_satiri_id = :fis_satiri_id");

    sql_query.SET_VALUE ( ":faturalanan_miktar" , faturalanan_miktar   );
    sql_query.SET_VALUE ( ":fis_satiri_id"      , p_irsaliye_satiri_id );

    sql_query.UPDATE();

    // EGER FATURALAN MIKTAR 0.00 ISE FIS DURUMU TEKRAR KONTROL EDILMELI
    if ( faturalanan_miktar EQ 0.00 ) {
        IRS_FATURALANDIMI_KONTROL( fis_id );
    }
}

/**************************************************************************************
                    IRS_IRSALIYE_FATURALANMIS_DURUMUNU_GUNCELLE
***************************************************************************************/

void IRS_IRSALIYE_FATURALANMIS_DURUMUNU_GUNCELLE ( int p_irsaliye_id, int p_durum )
{
    SQL_QUERY query ( DB );

    query.PREPARE_UPDATE ( "stk_fisler","fis_id",
                           "irsaliye_faturalastirildi_mi",
                           "fis_id    = :fis_id");

    query.SET_VALUE ( ":irsaliye_faturalastirildi_mi" , p_durum       );
    query.SET_VALUE ( ":fis_id"                       , p_irsaliye_id );

    query.UPDATE();

}

/**************************************************************************************
                    IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_SIFIRLA
***************************************************************************************/

void IRS_IRSALIYE_SATIRINDA_FATURALANAN_MIKTARI_SIFIRLA ( int p_irsaliye_satiri_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_UPDATE ( "stk_fis_satirlari ","fis_satiri_id",
                           "faturalanan_miktar",
                           "fis_satiri_id    = :fis_satiri_id");

    query.SET_VALUE ( ":faturalanan_miktar" , 0                    );
    query.SET_VALUE ( ":fis_satiri_id"      , p_irsaliye_satiri_id );

    query.UPDATE();
}


/**************************************************************************************
                    IRS_IRSALIYE_SATIRINDA_IADE_MIKTARINI_GUNCELLE
***************************************************************************************/

void IRS_IRSALIYE_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( int p_irsaliye_satiri_id, double p_iade_edilen_miktar, int p_artir_azalt )
{
    SQL_QUERY sql_query ( DB );

    double iade_edilen_miktar = 0.00;

    sql_query.PREPARE_SELECT("stk_fis_satirlari ","iade_edilen_miktar",
                             "fis_satiri_id = :fis_satiri_id");

    sql_query.SET_VALUE(":fis_satiri_id", p_irsaliye_satiri_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    if ( p_artir_azalt EQ ENUM_IRS_IADE_MIKTARI_AZALT ) {
        iade_edilen_miktar = ROUND ( sql_query.VALUE(0).toDouble() - p_iade_edilen_miktar );
    }
    else {
        iade_edilen_miktar = ROUND ( sql_query.VALUE(0).toDouble() + p_iade_edilen_miktar );
    }



    sql_query.PREPARE_UPDATE("stk_fis_satirlari ","fis_satiri_id",
                             "iade_edilen_miktar",
                             "fis_satiri_id    = :fis_satiri_id");

    sql_query.SET_VALUE ( ":iade_edilen_miktar" , iade_edilen_miktar   );
    sql_query.SET_VALUE ( ":fis_satiri_id"      , p_irsaliye_satiri_id );

    sql_query.UPDATE();

}

/**************************************************************************************
                    GET_STRUCT_IRS_RAPOR_VARS
***************************************************************************************/

IRSALIYE_RAPOR_VARS * GET_STRUCT_IRS_RAPOR_VARS()
{
    return IRS_RV;
}

/**************************************************************************************
                    IRS_GET_FATURA_DURUMU_ENUM
***************************************************************************************/

int IRS_GET_FATURA_DURUMU_ENUM ( QString fatura_durumu )
{
    if ( fatura_durumu EQ QObject::tr("Faturalanmış")) {
        return ENUM_FATURALANMIS;
    }
    else if ( fatura_durumu EQ QObject::tr("Faturalanmamış")) {
        return ENUM_FATURALANMAMIS;
    }
    else {
        return -1;
    }
}

/**************************************************************************************
                    IRS_GET_FATURA_DURUMU_STRING
***************************************************************************************/

QString IRS_GET_FATURA_DURUMU_STRING ( int fatura_durumu )
{
    if ( fatura_durumu EQ ENUM_FATURALANMIS ) {
        return QObject::tr("Faturalanmış");
    }
    else if ( fatura_durumu EQ ENUM_FATURALANMAMIS ) {
        return QObject::tr("Faturalanmamış");
    }
    else {
        return "";
    }
}

/**************************************************************************************
                    IRS_GET_IRSALIYE_YAZDIRILDIMI
***************************************************************************************/

bool IRS_GET_IRSALIYE_YAZDIRILDIMI  ( int p_irsaliye_id )
{
    SQL_QUERY sql_query( DB );

    sql_query.PREPARE_SELECT("stk_fisler","yazdirildi_mi",
                             "fis_id = :fis_id");

    sql_query.SET_VALUE(":fis_id", p_irsaliye_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return false;
    }

    sql_query.NEXT();

    return sql_query.VALUE( 0 ).toBool();
}

/**************************************************************************************
                    IRS_UPDATE_BELGE_SERI_VE_NUMARASI
***************************************************************************************/

void IRS_UPDATE_BELGE_SERI_VE_NUMARASI ( int p_irsaliye_id, QString p_belge_seri, QString p_belge_numarasi )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_UPDATE("stk_fisler","fis_id",
                             "belge_seri, belge_numarasi",
                             "fis_id    = :fis_id");

    sql_query.SET_VALUE ( ":belge_seri" ,       p_belge_seri    );
    sql_query.SET_VALUE ( ":belge_numarasi" ,   p_belge_numarasi);
    sql_query.SET_VALUE ( ":fis_id",            p_irsaliye_id   );

    sql_query.UPDATE();

}

/**************************************************************************************
                    IRS_FATURALANDIMI_KONTROL
***************************************************************************************/

void IRS_FATURALANDIMI_KONTROL( int p_fis_id )
{
    SQL_QUERY f_query ( DB );

    bool irsaliye_durumunu_degistir = false;

    f_query.PREPARE_SELECT("stk_fis_satirlari ","faturalanan_miktar",
                             "fis_id = :fis_id");

    f_query.SET_VALUE(":fis_id", p_fis_id );

    if ( f_query.SELECT() NE 0 ) {

        while ( f_query.NEXT() EQ true ) {
            double faturalanan_miktar = f_query.VALUE( 0 ).toDouble();
            // EGER HERHANGI BIR SATIR FATURALANMIS ISE FIS DURMUNU DEGISTIREMEYIZ
            if ( faturalanan_miktar > 0.00 ) {
                return;
            }
            irsaliye_durumunu_degistir = true;
        }
    }

    if ( irsaliye_durumunu_degistir EQ true ) {
        f_query.PREPARE_UPDATE( "stk_fisler", "fis_id", "irsaliye_faturalastirildi_mi", "fis_id = :fis_id");
        f_query.SET_VALUE     ( ":irsaliye_faturalastirildi_mi", 0        );
        f_query.SET_VALUE     ( ":fis_id"                      , p_fis_id );
        f_query.UPDATE();
    }
}

/**************************************************************************************
                    IRS_GET_IRSALIYE_ID_LIST
***************************************************************************************/

QList<int> IRS_GET_IRSALIYE_ID_LIST( int p_cari_hesap_id )
{
    SQL_QUERY query( DB );

    QList<int> fis_id_list;

    query.PREPARE_SELECT( "stk_fisler, stk_fis_satirlari",
                          "stk_fisler.fis_id ",
                          "stk_fisler.fis_id = stk_fis_satirlari.fis_id AND cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE     ( ":cari_hesap_id", p_cari_hesap_id );

    if( query.SELECT() NE 0 ) {
        while( query.NEXT() EQ true ) {
            int fis_id = query.VALUE(0).toInt();

            if( fis_id_list.contains( fis_id) EQ false ) {
                fis_id_list << fis_id;
            }
        }
    }

    return fis_id_list;
}
