#include <QDate>
#include "adak_sql.h"
#include "adak_utils.h"
#include "fat_enum.h"
#include "fat_struct.h"
#include "fat_console_utils.h"
#include <QComboBox>
#include "fat_fis_utils.h"
#include "stok_fis_utils.h"
#include "e9_enum.h"
#include "stok_console_utils.h"

extern ADAK_SQL * DB;

static FATURA_DETAYLARI_STRUCT * G_FATURA_DETAYLARI = new FATURA_DETAYLARI_STRUCT;

/**************************************************************************************
                   FAT_GET_FATURA_TURLERI_ENUM
***************************************************************************************/

int FAT_GET_FATURA_TURLERI_ENUM ( QString p_fatura_turu )
{

    if ( p_fatura_turu EQ QObject::tr("FATURA ( İRSALİYESİZ ) ( ALIŞ )") ) {
        return ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("İRSALİYELİ FATURA ( ALIŞ )") ) {
        return ENUM_FAT_IRSALIYELI_ALIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("HİZMET FATURASI ( ALIŞ )") ) {
        return ENUM_FAT_HIZMET_ALIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("VADE FARKI FATURASI ( ALIŞ )") ) {
        return ENUM_FAT_VADE_FARKI_ALIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("HAKEDİŞ FATURASI ( ALIŞ )") ) {
        return ENUM_FAT_HAKEDIS_ALIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("FATURA ( İRSALİYESİZ ) ( SATIŞ )") ) {
        return ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("İRSALİYELİ FATURA ( SATIŞ )") ) {
        return ENUM_FAT_IRSALIYELI_SATIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("HİZMET FATURASI ( SATIŞ )") ) {
        return ENUM_FAT_HIZMET_SATIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("PROFORMA FATURA ( SATIŞ )") ) {
        return ENUM_FAT_PROFORMA_SATIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("VADE FARKI FATURASI ( SATIŞ )") ) {
        return ENUM_FAT_VADE_FARKI_SATIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("HAKEDİŞ FATURASI ( SATIŞ )") ) {
        return ENUM_FAT_HAKEDIS_SATIS_FATURASI;
    }
    else if ( p_fatura_turu EQ QObject::tr("ELLE İRSALİYE FATURALAŞTIRMA ( ALIŞ )") ) {
        return ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS;
    }
    else if ( p_fatura_turu EQ QObject::tr("ELLE İRSALİYE FATURALAŞTIRMA ( SATIŞ )") ) {
        return ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS;
    }
    else if ( p_fatura_turu EQ QObject::tr("OTOMATIK İRSALİYE FATURALAŞTIRMA ( ALIŞ )") ) {
        return ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS;
    }
    else if ( p_fatura_turu EQ QObject::tr("OTOMATIK İRSALİYE FATURALAŞTIRMA ( SATIŞ )") ) {
        return ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS;
    }
    else if ( p_fatura_turu EQ QObject::tr("ELLE İRSALİYE FATURALAŞTIRMA KONSİNYE ( ALIŞ )") ) {
        return ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS;
    }
    else if ( p_fatura_turu EQ QObject::tr("ELLE İRSALİYE FATURALAŞTIRMA KONSİNYE ( SATIŞ )") ) {
        return ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS;
    }
    else if ( p_fatura_turu EQ QObject::tr("FATURA + İRSALİYE ( SATIŞ )") ) {
        return ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS;
    }
    else if ( p_fatura_turu EQ QObject::tr("FATURA + İRSALİYE ( ALIŞ )") ) {
        return ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS;
    }

    return -1;
}

/**************************************************************************************
                   FAT_GET_FATURA_TURLERI_STRING
***************************************************************************************/

QString FAT_GET_FATURA_TURLERI_STRING ( int p_fatura_turu )
{

    switch ( p_fatura_turu ) {

        case ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI :
            return QObject::tr("FATURA ( İRSALİYESİZ ) ( ALIŞ )");

        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI :
            return QObject::tr("İRSALİYELİ FATURA ( ALIŞ )");

        case ENUM_FAT_HIZMET_ALIS_FATURASI :
            return QObject::tr("HİZMET FATURASI ( ALIŞ )");

        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI :
            return QObject::tr("VADE FARKI FATURASI ( ALIŞ )");

        case ENUM_FAT_HAKEDIS_ALIS_FATURASI :
            return QObject::tr("HAKEDİŞ FATURASI ( ALIŞ )");

        case ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI :
            return QObject::tr("FATURA ( İRSALİYESİZ ) ( SATIŞ )");

        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
            return QObject::tr ("İRSALİYELİ FATURA ( SATIŞ )");

        case ENUM_FAT_HIZMET_SATIS_FATURASI :
            return QObject::tr("HİZMET FATURASI ( SATIŞ )");

        case ENUM_FAT_PROFORMA_SATIS_FATURASI :
            return QObject::tr("PROFORMA FATURA ( SATIŞ )");

        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI :
            return QObject::tr("VADE FARKI FATURASI ( SATIŞ )");

        case ENUM_FAT_HAKEDIS_SATIS_FATURASI :
            return QObject::tr("HAKEDİŞ FATURASI ( SATIŞ )");

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS :
            return QObject::tr("ELLE İRSALİYE FATURALAŞTIRMA ( ALIŞ )");

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS :
            return QObject::tr("ELLE İRSALİYE FATURALAŞTIRMA ( SATIŞ )");

        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS :
            return QObject::tr("OTOMATIK İRSALİYE FATURALAŞTIRMA ( ALIŞ )");

        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS :
            return QObject::tr("OTOMATIK İRSALİYE FATURALAŞTIRMA ( SATIŞ )");

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS :
            return QObject::tr("ELLE İRSALİYE FATURALAŞTIRMA KONSİNYE ( ALIŞ )");

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :
            return QObject::tr("ELLE İRSALİYE FATURALAŞTIRMA KONSİNYE ( SATIŞ )");

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
            return QObject::tr("FATURA + İRSALİYE ( SATIŞ )");

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS:
            return QObject::tr("FATURA + İRSALİYE ( ALIŞ )");

        default :
            return "";
    }
}

/**************************************************************************************
                   FAT_GET_ACIK_KAPALI_FATURA_STRING
***************************************************************************************/

QString FAT_GET_ACIK_KAPALI_FATURA_STRING ( int fatura_cinsi )
{
    if ( fatura_cinsi EQ ENUM_ACIK_FATURA ) {
        return QObject::tr("Açık Fatura");
    }
    else if( fatura_cinsi EQ ENUM_KAPALI_FATURA ) {
        return QObject::tr("Kapalı Fatura");
    }

    return QObject::tr( "Tüm Faturalar" );
}

/**************************************************************************************
                   FAT_GET_ACIK_KAPALI_FATURA_ENUM
***************************************************************************************/

int FAT_GET_ACIK_KAPALI_FATURA_ENUM ( QString fatura_cinsi )
{
    if ( fatura_cinsi EQ QObject::tr("Açık Fatura") ) {
        return ENUM_ACIK_FATURA;
    }
    else if ( fatura_cinsi EQ QObject::tr( "Kapalı Fatura") ) {
        return ENUM_KAPALI_FATURA;
    }
    else if ( fatura_cinsi EQ QObject::tr( "İptal Edilen Faturalar") ){
        return ENUM_IPTAL_EDILEN_FATURALAR;
    }
    return ENUM_TUM_FATURALAR;
}


/**************************************************************************************
                   FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE
***************************************************************************************/

void FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE ( int p_fis_satiri_id, double p_irsaliyelenen_miktar, int p_artir_azalt )
{
    SQL_QUERY sql_query ( DB );

    double irsaliyelenen_miktar = 0.00;

    sql_query.PREPARE_SELECT("fat_fatura_satirlari ","irsaliyelenen_miktar, fis_id",
                             "fis_satiri_id = :fis_satiri_id");

    sql_query.SET_VALUE(":fis_satiri_id", p_fis_satiri_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    int fatura_id = sql_query.VALUE( 0 ).toInt();

    if ( p_artir_azalt EQ ENUM_FAT_IRSALIYELENEN_MIKTAR_AZALT ) {
        irsaliyelenen_miktar = ROUND ( sql_query.VALUE(0).toDouble() - p_irsaliyelenen_miktar );
    }
    else if( p_artir_azalt EQ ENUM_FAT_IRSALIYELENEN_MIKTAR_ARTIR ) {
        irsaliyelenen_miktar = ROUND ( sql_query.VALUE(0).toDouble() + p_irsaliyelenen_miktar );
    }
    else if( p_artir_azalt EQ ENUM_FAT_IRSALIYELENEN_MIKTAR_GUNCELLE ) {
        irsaliyelenen_miktar = p_irsaliyelenen_miktar;
    }

    sql_query.PREPARE_UPDATE("fat_fatura_satirlari ","fis_satiri_id",
                             "irsaliyelenen_miktar ",
                             "fis_satiri_id   = :fis_satiri_id ");

    sql_query.SET_VALUE ( ":irsaliyelenen_miktar" , irsaliyelenen_miktar );
    sql_query.SET_VALUE ( ":fis_satiri_id"     , p_fis_satiri_id   );

    sql_query.UPDATE();

    if ( irsaliyelenen_miktar EQ 0.00 ) {
        FAT_IRSALIYENDIMI_KONTROL(  fatura_id );
    }
}

/**************************************************************************************
                   FAT_SIRADAKI_FIS_NO_AL
***************************************************************************************/

int FAT_SIRADAKI_FIS_NO_AL ( QString p_fatura_tarihi)
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("fat_faturalar" , "fis_no" , "fatura_tarihi = :fatura_tarihi");

    sql_query.SET_VALUE ( ":fatura_tarihi", p_fatura_tarihi );

    if ( sql_query.SELECT("fis_no DESC" , 0,1) EQ 0 ) {
        return 1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt() + 1;
}

/**************************************************************************************
                   FAT_GET_FATURA_DETAYLARI_STRUCT
***************************************************************************************/

void * FAT_GET_FATURA_DETAYLARI_STRUCT ()
{
    return G_FATURA_DETAYLARI;
}

/**************************************************************************************
                FAT_FATURA_SATIRINDA_IADE_MIKTARINI_GUNCELLE
**************************************************************************************/

void FAT_FATURA_SATIRINDA_IADE_MIKTARINI_GUNCELLE ( int p_fatura_satiri_id, double p_iade_edilen_miktar,
                                                    int p_artir_azalt , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query ( USE_DB );

    double iade_edilen_miktar = 0.00;

    query.PREPARE_SELECT ( "fat_fatura_satirlari ","iade_edilen_miktar",
                           "fis_satiri_id = :fis_satiri_id");

    query.SET_VALUE ( ":fis_satiri_id", p_fatura_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    if ( p_artir_azalt EQ ENUM_FAT_IADE_MIKTARI_AZALT ) {
        iade_edilen_miktar = ROUND ( query.VALUE(0).toDouble() - p_iade_edilen_miktar  );
    }
    else {
        iade_edilen_miktar = ROUND ( query.VALUE(0).toDouble() + p_iade_edilen_miktar  );
    }

    query.PREPARE_UPDATE ( "fat_fatura_satirlari ","fis_satiri_id",
                           "iade_edilen_miktar",
                           "fis_satiri_id    = :fis_satiri_id");

    query.SET_VALUE ( ":iade_edilen_miktar" , iade_edilen_miktar   );
    query.SET_VALUE ( ":fis_satiri_id"      , p_fatura_satiri_id );

    query.UPDATE();

}

/**************************************************************************************
                    FAT_GET_IRS_DURUMU_ENUM
***************************************************************************************/

int FAT_GET_IRSALIYE_DURUMU_ENUM ( QString irsaliye_durumu )
{
    if ( irsaliye_durumu EQ QObject::tr("İrsaliyelenmiş")) {
        return ENUM_IRSALIYELENMIS;
    }
    else if ( irsaliye_durumu EQ QObject::tr("İrsaliyelenmemiş")) {
        return ENUM_IRSALIYELENMEMIS;
    }
    else {
        return -1;
    }
}

/**************************************************************************************
                    FAT_GET_IRS_FATURA_DURUMU_STRING
***************************************************************************************/

QString FAT_GET_FAT_IRSALIYE_DURUMU_STRING ( int irsaliye_durumu )
{
    if ( irsaliye_durumu EQ ENUM_IRSALIYELENMIS ) {
        return QObject::tr("İrsaliyelenmiş");
    }
    else if ( irsaliye_durumu EQ ENUM_IRSALIYELENMEMIS ) {
        return QObject::tr("İrsaliyelenmemiş");
    }
    else {
        return "";
    }
}

/**************************************************************************************
                    FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_ENUM
***************************************************************************************/

int FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_ENUM(QString alis_satis_turu)
{
    if ( alis_satis_turu EQ QObject::tr("Alış Faturaları")) {
        return ENUM_ALIS_IRSALIYESI_FATURASI;
    }
    else if ( alis_satis_turu EQ QObject::tr("Satış Faturaları")) {
        return ENUM_SATIS_IRSALIYESI_FATURASI;
    }
    return -1;
}

/**************************************************************************************
                    FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_STRING
***************************************************************************************/

QString FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_STRING(int alis_satis_turu)
{
    if ( alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
        return QObject::tr("Alış Faturaları");
    }
    else if ( alis_satis_turu EQ ENUM_SATIS_IRSALIYESI_FATURASI ) {
        return QObject::tr("Satış Faturaları");
    }
    return "";
}

/**************************************************************************************
                    FAT_GET_FAT_BA_BS_FORMU_TURU_ENUM
***************************************************************************************/

int FAT_GET_FAT_BA_BS_FORMU_TURU_ENUM( QString ba_bs_turu )
{
    if ( ba_bs_turu EQ QObject::tr("BA Formu")) {
        return ENUM_BA_FORMU;
    }
    else if ( ba_bs_turu EQ QObject::tr("BS Formu")) {
        return ENUM_BS_FORMU;
    }
    return -1;
}

/**************************************************************************************
                    FAT_GET_FAT_BA_BS_FORMU_TURU_STRING
***************************************************************************************/

QString FAT_GET_FAT_BA_BS_FORMU_TURU_STRING(int ba_bs_turu)
{
    if ( ba_bs_turu EQ ENUM_BA_FORMU ) {
        return QObject::tr("BA Formu");
    }
    else if ( ba_bs_turu EQ ENUM_BS_FORMU ) {
        return QObject::tr("BS Formu");
    }
    return "";
}



/*******************************************************************************/
//            FAT_GET_FATURA_CARI_HESAP_ID
/*******************************************************************************/

int FAT_GET_FATURA_CARI_HESAP_ID( int p_fatura_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("fat_faturalar" , "cari_hesap_id" , "fis_id = :fis_id");

    sql_query.SET_VALUE ( ":fis_id", p_fatura_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return 1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/*******************************************************************************/
//            FAT_GET_FAT_TEMEL_BILGILER
/*******************************************************************************/

void FAT_GET_FAT_TEMEL_BILGILER( FATURA_TEMEL_BILGILER_STRUCT *P_FAT, int p_fatura_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("fat_faturalar" , "fatura_tutari, isk_sonrasi_kdv_tutari, muh_fis_id" ,
                         "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fatura_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        P_FAT->fatura_tutari             = query.VALUE(0).toDouble();
        P_FAT->isk_sonrasi_kdv_tutari    = query.VALUE(1).toDouble();
        P_FAT->muh_fis_id                = query.VALUE(2).toInt();
    }

}

/**************************************************************************************
                   FAT_GET_FATURA_FIS_SATIRI_ID
***************************************************************************************/

int  FAT_GET_FATURA_FIS_SATIRI_ID ( int p_urun_id )
{
    SQL_QUERY select_query( DB );

    select_query.PREPARE_SELECT("fat_fatura_satirlari","fis_satiri_id","urun_id = :urun_id");
    select_query.SET_VALUE(":urun_id" , p_urun_id);

    if ( select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE( 0 ).toInt();
    }

    return 0;
}

/*****************************************************************************************************
                 FAT_GET_FATURA_FIS_ID_LIST
*******************************************************************************************************/

QList <int > FAT_GET_FATURA_FIS_ID_LIST(int p_cari_hesap_id,
                                    int p_fatura_turu,
                                    int p_fatura_irsaliyelestirildi_mi,
                                    int p_hatali_fatura_mi,
                                    QString p_fatura_tarihi_baslangic,
                                    QString p_fatura_tarihi_bitis )
{

    QList <int > fat_fis_list;

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("fat_faturalar" , "fis_id");

    if ( p_cari_hesap_id NE -1 ) {
        sql_query.AND_EKLE  ( "cari_hesap_id = :cari_hesap_id " );
        sql_query.SET_VALUE ( ":cari_hesap_id" , p_cari_hesap_id );
    }
    if ( p_fatura_turu NE -1 ) {
        sql_query.AND_EKLE  ( "fatura_turu = :fatura_turu" );
        sql_query.SET_VALUE ( ":fatura_turu", p_fatura_turu             );
    }
    if ( p_fatura_irsaliyelestirildi_mi NE -1 ) {
        sql_query.AND_EKLE  ( "fatura_irsaliyelestirildi_mi = :fatura_irsaliyelestirildi_mi" );
        sql_query.SET_VALUE ( ":fatura_irsaliyelestirildi_mi", p_fatura_irsaliyelestirildi_mi );
    }
    if ( p_hatali_fatura_mi NE -1 ) {
        sql_query.AND_EKLE  ( "hatali_fatura_mi = :hatali_fatura_mi" );
        sql_query.SET_VALUE ( ":hatali_fatura_mi", p_hatali_fatura_mi );
    }

    if ( p_fatura_tarihi_baslangic.isEmpty() NE true AND p_fatura_tarihi_bitis.isEmpty() NE true ) {
        sql_query.AND_EKLE  ( "fatura_tarihi BETWEEN :fatura_tarihi_baslangic AND :fatura_tarihi_bitis" );
        sql_query.SET_VALUE ( ":fatura_tarihi_baslangic", p_fatura_tarihi_baslangic );
        sql_query.SET_VALUE ( ":fatura_tarihi_bitis", p_fatura_tarihi_bitis );
    }

    if ( sql_query.SELECT("fatura_tarihi ASC ,fis_no ASC") NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {
            fat_fis_list << sql_query.VALUE( "fis_id").toInt();
        }
    }

    return fat_fis_list;
}

/*****************************************************************************************************
                 FAT_GET_FATURA_FIS_ID_LIST
*******************************************************************************************************/

QMap <int, int > FAT_GET_FATURA_FIS_AND_FIS_SATIRI_ID_MAP( int p_cari_hesap_id, int p_fatura_turu, int p_hatali_fatura_mi,
                                                           QString p_fatura_tarihi_baslangic, QString p_fatura_tarihi_bitis )
{

    QMap < int, int > fat_fis_map;

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("fat_faturalar, fat_fatura_satirlari",
                             "fat_faturalar.fis_id, fis_satiri_id, urun_id",
                             "fat_faturalar.fis_id = fat_fatura_satirlari.fis_id ");

    if ( p_cari_hesap_id NE -1 ) {
        sql_query.AND_EKLE  ( "cari_hesap_id = :cari_hesap_id " );
        sql_query.SET_VALUE ( ":cari_hesap_id" , p_cari_hesap_id );
    }
    if ( p_fatura_turu NE -1 ) {
        sql_query.AND_EKLE  ( "fatura_turu = :fatura_turu" );
        sql_query.SET_VALUE ( ":fatura_turu", p_fatura_turu );
    }
    if ( p_hatali_fatura_mi NE -1 ) {
        sql_query.AND_EKLE  ( "hatali_fatura_mi = :hatali_fatura_mi" );
        sql_query.SET_VALUE ( ":hatali_fatura_mi", p_hatali_fatura_mi );
    }
    if ( p_fatura_tarihi_baslangic.isEmpty() NE true AND p_fatura_tarihi_bitis.isEmpty() NE true ) {
        sql_query.AND_EKLE  ( "fatura_tarihi BETWEEN :fatura_tarihi_baslangic AND :fatura_tarihi_bitis" );
        sql_query.SET_VALUE ( ":fatura_tarihi_baslangic", p_fatura_tarihi_baslangic );
        sql_query.SET_VALUE ( ":fatura_tarihi_bitis", p_fatura_tarihi_bitis );
    }

    if ( sql_query.SELECT("fatura_tarihi ASC ,fis_no ASC") NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {
            int urun_id = sql_query.VALUE( 2 ).toInt();
            //! HIZMET IRSALIYENLENMECEK
            if( STK_GET_HIZMET_MI( urun_id ) EQ EVET ) {
                continue;
            }
            fat_fis_map.insert( sql_query.VALUE( "fis_id").toInt(), sql_query.VALUE( "fis_satiri_id").toInt() );
        }
    }

    return fat_fis_map;
}



/*****************************************************************************************************
                 FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_ID
*******************************************************************************************************/

int FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_ID( int p_irsaliye_satiri_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "fat_irs_satir_baglantisi, fat_fatura_satirlari",
                          "fat_fatura_satirlari.fis_id",
                          "fat_fatura_satirlari.fis_satiri_id = fat_irs_satir_baglantisi.irsaliye_satiri_id "
                          "AND irsaliye_satiri_id = :irsaliye_satiri_id" );
    query.SET_VALUE     ( ":irsaliye_satiri_id" , p_irsaliye_satiri_id );

    if ( query.SELECT() NE 0 ) {
        return query.VALUE( 0 ).toInt();
    }
    return 0;
}

/*****************************************************************************************************
                 FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_SATIRI_ID
*******************************************************************************************************/

int FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_SATIRI_ID(int p_irsaliye_satiri_id)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "fat_irs_satir_baglantisi", "fatura_satiri_id", "irsaliye_satiri_id = :irsaliye_satiri_id" );
    query.SET_VALUE     ( ":irsaliye_satiri_id" , p_irsaliye_satiri_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        return query.VALUE( 0 ).toInt();
    }

    return 0;
}

/*****************************************************************************************************
                 FAT_GET_FAT_IRS_BAGLANTISI_FATURA_ID
*******************************************************************************************************/

QList <int> FAT_GET_FAT_IRS_BAGLANTISI_FATURA_ID(int p_irsaliye_id)
{
    QList <int> fat_fis_id_list;
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "fat_irs_baglantisi", "fatura_id", "irsaliye_id = :irsaliye_id" );
    query.SET_VALUE     ( ":irsaliye_id" , p_irsaliye_id );

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            fat_fis_id_list << query.VALUE( 0 ).toInt();
        }
    }

    return fat_fis_id_list;
}

/**************************************************************************************
                   FAT_UPDATE_FATURA_SATIRI_URUN_MIKTARI
***************************************************************************************/

void FAT_UPDATE_FATURA_SATIRI_URUN_MIKTARI ( int p_fatura_satiri_id, int p_satirdaki_urun_miktari )
{
    SQL_QUERY query(DB);

    query.PREPARE_UPDATE ( "fat_fatura_satirlari ","fis_satiri_id",
                           "irsaliyelenen_miktar",
                           "fis_satiri_id        = :fis_satiri_id");

    query.SET_VALUE ( ":irsaliyelenen_miktar",  p_satirdaki_urun_miktari );
    query.SET_VALUE ( ":fis_satiri_id",         p_fatura_satiri_id     );

    query.UPDATE();

}

/**************************************************************************************
                    FAT_GET_FATURA_FIS_SATIRI_LIST
***************************************************************************************/

QList <int> FAT_GET_FATURA_FIS_SATIRI_ID_LIST  ( int  p_fis_id , ADAK_SQL * P_GELEN_DB )
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    QList <int> fis_satiri_id_list;

    SQL_QUERY query      ( USE_DB );

    query.PREPARE_SELECT ( "fat_fatura_satirlari",
                           "fis_satiri_id",
                               "fis_id = :fis_id" );
    query.SET_VALUE ( ":fis_id", p_fis_id );

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            fis_satiri_id_list << query.VALUE( 0 ).toInt();
        }
    }
    return fis_satiri_id_list;
}

/**************************************************************************************
                    FAT_FAT_GET_FAT_IRS_STR_IRSALIYE_STR_ID_LIST
***************************************************************************************/

QList < int> FAT_GET_FAT_IRS_STR_IRSALIYE_STR_ID_LIST(int p_fis_id) {

    QList <int> irs_fis_satiri_id_list;

    SQL_QUERY query      ( DB );

    query.PREPARE_SELECT     ( "fat_fatura_satirlari, fat_irs_satir_baglantisi",
                               "irsaliye_satiri_id",
                               "fat_fatura_satirlari.fis_satiri_id = fat_irs_satir_baglantisi.fatura_satiri_id "
                               "AND fat_fatura_satirlari.fis_id = :fis_id " );
    query.SET_VALUE ( ":fis_id", p_fis_id );

    if ( query.SELECT() NE 0 ) {
        irs_fis_satiri_id_list << query.VALUE( 0 ).toInt();
    }
    return irs_fis_satiri_id_list;
}

/**************************************************************************************
                    FAT_UPDATE_FATURA_IRSALIYELESTI_MI
***************************************************************************************/

void FAT_UPDATE_FATURA_IRSALIYELESTIRILDI_MI( int p_fatura_id, int p_fatura_irsaliyelestirildi_mi )
{
    SQL_QUERY query( DB );

    query.PREPARE_UPDATE ( "fat_faturalar","fis_id", "fatura_irsaliyelestirildi_mi ",
                           "fis_id = :fis_id");
    query.SET_VALUE      ( ":fis_id"                      , p_fatura_id );
    query.SET_VALUE      ( ":fatura_irsaliyelestirildi_mi", p_fatura_irsaliyelestirildi_mi );
    query.UPDATE         ();

}

/**************************************************************************************
                    FAT_GET_FATURA_TURU
***************************************************************************************/

int FAT_GET_FATURA_TURU ( int p_fatura_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "fat_faturalar", "fatura_turu", "fis_id = :fis_id");
    query.SET_VALUE     ( ":fis_id", p_fatura_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE( 0 ).toInt();

}

/**************************************************************************************
                    FAT_IRSALIYENDIMI_KONTROL
***************************************************************************************/

void FAT_IRSALIYENDIMI_KONTROL( int p_fis_id )
{
    SQL_QUERY f_query ( DB );

    bool fatura_durumunu_degistir = false;

    f_query.PREPARE_SELECT("fat_fatura_satirlari ","irsaliyelenen_miktar",
                             "fis_id = :fis_id");

    f_query.SET_VALUE(":fis_id", p_fis_id );

    if ( f_query.SELECT() NE 0 ) {

        while ( f_query.NEXT() EQ true ) {
            double irsaliyelenen_miktar = f_query.VALUE( 0 ).toDouble();
            // EGER HERHANGI BIR SATIR FATURALANMIS ISE FIS DURMUNU DEGISTIREMEYIZ
            if ( irsaliyelenen_miktar > 0.00 ) {
                return;
            }
            fatura_durumunu_degistir = true;
        }
    }

    if ( fatura_durumunu_degistir EQ true ) {
        f_query.PREPARE_UPDATE( "fat_faturalar", "fis_id", "fatura_irsaliyelestirildi_mi", "fis_id = :fis_id");
        f_query.SET_VALUE     ( ":fatura_irsaliyelestirildi_mi", 0        );
        f_query.SET_VALUE     ( ":fis_id"                      , p_fis_id );
        f_query.UPDATE();
    }
}

/**************************************************************************************
                    FAT_IRSALIYENDIMI_KONTROL
***************************************************************************************/

int FAT_GET_FATURA_ID ( int p_fatura_satir_id )
{
    SQL_QUERY f_query ( DB );

    f_query.PREPARE_SELECT("fat_fatura_satirlari ","fis_id",
                             "fis_satiri_id = :fis_satiri_id");

    f_query.SET_VALUE(":fis_satiri_id", p_fatura_satir_id );

    if ( f_query.SELECT() NE 0 ) {
        f_query.NEXT();
        return f_query.VALUE( 0 ).toInt();
    }

    return 0;
}

/**************************************************************************************
                    FAT_IRS_BAGLANTISI_EKLE
***************************************************************************************/

int FAT_IRS_BAGLANTISI_EKLE( int p_fatura_id, int p_irsaliye_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_INSERT( "fat_irs_baglantisi", "kayit_id", "fatura_id, irsaliye_id" );
    query.SET_VALUE     ( ":fatura_id"   ,  p_fatura_id  );
    query.SET_VALUE     ( ":irsaliye_id" , p_irsaliye_id );

    return query.INSERT();
}

/**************************************************************************************
                    FAT_IRS_BAGLANTISI_EKLE
***************************************************************************************/

QString FAT_IRS_BAGLANTISI_BILGISI( int p_fatura_id, int p_irsaliye_id )
{
    SQL_QUERY query( DB );

    FATURA_FISI_STRUCT FAT_FIS;
    STK_IRS_FIS_STRUCT IRS_FIS;

    FAT_FIS.KDV_ORANLARI_ARRAY                     =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_HESAPLARI_ID_ARRAY                 =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY        =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.OTV_ORANLARI_ARRAY                     =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_HESAPLARI_ID_ARRAY                 =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY        =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY      =   new int     [KDV_OTV_ORANLARI_SAYISI];

    FATURA_CLEAR_FIS_STRUCT  ( &FAT_FIS );
    STK_IRS_CLEAR_FIS_STRUCT ( &IRS_FIS );

    QString baglanti_bilgisi = "";

    query.PREPARE_SELECT( "fat_irs_baglantisi", "fatura_id, irsaliye_id" );

    if ( p_fatura_id NE -1 ) {
        query.AND_EKLE  ( "fatura_id = :fatura_id");
        query.SET_VALUE ( ":fatura_id"   ,  p_fatura_id  );
    }
    else if ( p_irsaliye_id NE -1 ) {
        query.AND_EKLE  ( "irsaliye_id = :irsaliye_id" );
        query.SET_VALUE ( ":irsaliye_id" , p_irsaliye_id );
    }

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            int fatura_id   = query.VALUE( 0 ).toInt();
            int irsaliye_id = query.VALUE( 1).toInt();

            if ( p_fatura_id NE -1 ) {
                STK_IRS_FIS_BILGILERINI_OKU( irsaliye_id, &IRS_FIS );
                baglanti_bilgisi.append( QDate::fromString( IRS_FIS.fis_tarihi, "yyyy.MM.dd").toString( "dd/MM/yyyy" ) + "  İrsaliye No : " + QVariant ( IRS_FIS.fis_no ).toString() + "\n" );
            }
            else if ( p_irsaliye_id NE -1 ) {
                FATURA_FIS_BILGILERINI_OKU( &FAT_FIS, fatura_id );
                baglanti_bilgisi.append( QDate::fromString( FAT_FIS.fatura_tarihi, "yyyy.MM.dd").toString( "dd/MM/yyyy" ) + "  Fatura No:" +  QVariant ( FAT_FIS.fis_no ).toString() + "\n" );
            }
        }
    }

    return baglanti_bilgisi;
}

/**************************************************************************************
                    BELGE_SERI_KONTROLU
***************************************************************************************/

bool BELGE_SERI_KONTROLU ( QString p_belge_seri, QString p_belge_numarasi, int p_modul_id, int p_cari_hesap_id )
{

    if( p_belge_seri.isEmpty() EQ true OR p_belge_numarasi.isEmpty() EQ true ) {
        return true;
    }

    SQL_QUERY query( DB );

    QString table_name;

    if( p_modul_id EQ FATURA_MODULU ) {
        table_name = "fat_faturalar";
    }
    else if ( p_modul_id EQ IRSALIYE_MODULU ) { // IRSALIYE_MODULU
        table_name = "stk_fisler";
    }
    else { // CARI MODULU
        table_name = "car_fisler";
    }

    query.PREPARE_SELECT( table_name, "fis_id", "belge_seri = :belge_seri AND belge_numarasi = :belge_numarasi AND cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE     ( ":belge_seri", p_belge_seri );
    query.SET_VALUE     ( ":belge_numarasi", p_belge_numarasi );
    query.SET_VALUE     ( ":cari_hesap_id", p_cari_hesap_id );

    if( query.SELECT() EQ 0 ) {
        return true;
    }

    return false;
}

/**************************************************************************************
                    FAT_GET_FIS_ID_LIST
***************************************************************************************/

QList<int> FAT_GET_FIS_ID_LIST(int p_urun_id)
{
    SQL_QUERY query( DB );

    QList<int> fis_id_list;

    query.PREPARE_SELECT( "fat_faturalar, fat_fatura_satirlari",
                          "fat_faturalar.fis_id ", "fat_faturalar.fis_id = fat_fatura_satirlari.fis_id AND urun_id = :urun_id");
    query.SET_VALUE     ( ":urun_id", p_urun_id );

    if( query.SELECT() NE 0 ) {
        while( query.NEXT() EQ true ) {
            int fis_id        = query.VALUE(0).toInt();
            if( fis_id_list.contains( fis_id) EQ false ) {
                fis_id_list << fis_id;
            }
        }
    }

    return fis_id_list;
}
