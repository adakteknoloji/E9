#include <QDate>
#include <QString>
#include "adak_sql.h"
#include "adak_utils.h"
#include "e9_console_utils.h"
#include "fat_enum.h"
#include "stok_enum.h"
#include "stok_console_utils.h"
#include "e9_enum.h"
#include "stok_struct.h"
#include "irs_enum.h"

extern ADAK_SQL    *   DB;

/**************************************************************************************
                   STK_GET_STOK_TAKIP_SEKLI
***************************************************************************************/

QString STK_GET_STOK_TAKIP_SEKLI ( int p_stok_takip_sekli )
{
    switch ( p_stok_takip_sekli ) {
        case ENUM_BASIT_TAKIP :
            return QObject::tr("BASİT TAKİP");
        case ENUM_PARTI_TAKIBI :
            return QObject::tr("PARTİ KODU İLE TAKİP");
        case ENUM_SERI_NO_ILE_TAKIP :
            return QObject::tr("SERİ NO İLE TAKİP");
    }
    return "";
}


/**************************************************************************************
                   STK_GET_STOK_TAKIP_SEKLI_ENUM
***************************************************************************************/

int STK_GET_STOK_TAKIP_SEKLI_ENUM ( QString p_stok_takip_sekli )
{
    if ( p_stok_takip_sekli EQ QObject::tr("BASİT TAKİP") ) {
        return ENUM_BASIT_TAKIP;
    }
    else if ( p_stok_takip_sekli EQ QObject::tr("PARTİ KODU İLE TAKİP") ) {
        return ENUM_PARTI_TAKIBI;
    }
    else if ( p_stok_takip_sekli EQ QObject::tr("SERİ NO İLE TAKİP") ) {
        return ENUM_SERI_NO_ILE_TAKIP;
    }
    return -1;
}

/**************************************************************************************
                   STK_GET_STOK_FIS_TURU
***************************************************************************************/

QString STK_GET_STOK_FIS_TURU ( int p_stok_fis_turu )
{
    switch ( p_stok_fis_turu ) {

        case ENUM_STK_SARF_FIRE_FISI :
            return QObject::tr("SARF/FİRE FİŞİ");

        case ENUM_STK_IMALATA_SEVK_FISI :
            return QObject::tr("İMALATA SEVK FİŞİ");

        case ENUM_STK_IMALATTAN_DONUS_FISI :
            return QObject::tr("İMALATTAN DÖNÜŞ FİŞİ");

        case ENUM_STK_STOK_ACILIS_FISI :
            return QObject::tr("STOK AÇILIŞ FİŞİ");

        case ENUM_IRS_NORMAL_GIRIS :
            return QObject::tr("ALIŞ İRSALİYESİ - NORMAL");

        case ENUM_STK_DEPO_TRANSFER_GIRIS :
            return QObject::tr("DEPO TRANSFER GİRİŞ");

        case ENUM_IRS_FASON_BASLANGIC_GIRIS :
            return QObject::tr("ALIŞ İRSALİYESİ - FASON BAŞLANGIÇ");

        case ENUM_IRS_FASON_BITIS_GIRIS :
            return QObject::tr("ALIŞ İRSALİYESİ - FASON BİTİŞ");

        case ENUM_IRS_KONSINYE_GIRIS :
            return QObject::tr("ALIŞ İRSALİYESİ - KONSİNYE");

        case ENUM_IRS_NORMAL_CIKIS :
            return QObject::tr("SATIŞ İRSALİYESİ - NORMAL");

        case ENUM_STK_DEPO_TRANSFER_CIKIS :
            return QObject::tr("DEPO TRANSFER ÇIKIŞ");

        case ENUM_IRS_FASON_BASLANGIC_CIKIS :
            return QObject::tr("SATIŞ İRSALİYESİ - FASON BAŞLANGIÇ");

        case ENUM_IRS_FASON_BITIS_CIKIS :
            return QObject::tr("SATIŞ İRSALİYESİ - FASON BİTİŞ");

        case ENUM_IRS_KONSINYE_CIKIS :
            return QObject::tr("SATIŞ İRSALİYESİ - KONSİNYE");

        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS :
            return QObject::tr("ELLE FATURA İRSALİYELEŞTİRME - ALIŞ");

        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
            return QObject::tr("ELLE FATURA İRSALİYELEŞTİRME - SATIŞ");

        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS :
            return QObject::tr("OTOMATİK FATURA İRSALİYELEŞTİRME - ALIŞ");

        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS :
            return QObject::tr("OTOMATİK FATURA İRSALİYELEŞTİRME - SATIŞ");

        case ENUM_IRS_HAKEDIS_SATIS :
            return QObject::tr("SATIŞ İRSALİYESİ - HAKEDİŞ");

        case ENUM_IRS_HAKEDIS_ALIS :
            return QObject::tr("ALIŞ İRSALİYESİ - HAKEDİŞ");

        default :
            break;
    }
    return "";

}


/**************************************************************************************
                   STK_GET_STOK_FIS_TURU_ENUM
***************************************************************************************/

int STK_GET_STOK_FIS_TURU_ENUM ( QString p_stok_fis_turu )
{
    if ( p_stok_fis_turu EQ QObject::tr("SARF/FİRE FİŞİ") ) {
        return ENUM_STK_SARF_FIRE_FISI;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("İMALATA SEVK FİŞİ") ) {
        return ENUM_STK_IMALATA_SEVK_FISI;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("İMALATTAN DÖNÜŞ FİŞİ")) {
        return ENUM_STK_IMALATTAN_DONUS_FISI;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("STOK AÇILIŞ FİŞİ") ) {
        return ENUM_STK_STOK_ACILIS_FISI;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("ALIŞ İRSALİYESİ - NORMAL") ) {
        return ENUM_IRS_NORMAL_GIRIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("DEPO TRANSFER GİRİŞ") ) {
        return ENUM_STK_DEPO_TRANSFER_GIRIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("ALIŞ İRSALİYESİ - FASON BAŞLANGIÇ")) {
        return ENUM_IRS_FASON_BASLANGIC_GIRIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("ALIŞ İRSALİYESİ - FASON BİTİŞ") ) {
        return ENUM_IRS_FASON_BITIS_GIRIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("ALIŞ İRSALİYESİ - KONSİNYE") ) {
        return ENUM_IRS_KONSINYE_GIRIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("SATIŞ İRSALİYESİ - NORMAL") ) {
        return ENUM_IRS_NORMAL_CIKIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("DEPO TRANSFER ÇIKIŞ") ) {
        return ENUM_STK_DEPO_TRANSFER_CIKIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("SATIŞ İRSALİYESİ - FASON BAŞLANGIÇ")) {
        return ENUM_IRS_FASON_BASLANGIC_CIKIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("SATIŞ İRSALİYESİ - FASON BİTİŞ") ) {
        return ENUM_IRS_FASON_BITIS_CIKIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("SATIŞ İRSALİYESİ - KONSİNYE") ) {
        return ENUM_IRS_KONSINYE_CIKIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("ELLE FATURA İRSALİYELEŞTİRME - ALIŞ") ) {
        return ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("ELLE FATURA İRSALİYELEŞTİRME - SATIŞ") ) {
        return ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("OTOMATİK FATURA İRSALİYELEŞTİRME - ALIŞ") ) {
        return ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr("OTOMATİK FATURA İRSALİYELEŞTİRME - SATIŞ") ) {
        return ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr( "SATIŞ İRSALİYESİ - HAKEDİŞ" ) ) {
        return ENUM_IRS_HAKEDIS_SATIS;
    }
    else if ( p_stok_fis_turu EQ QObject::tr( "ALIŞ İRSALİYESİ - HAKEDİŞ" ) ) {
        return ENUM_IRS_HAKEDIS_ALIS;
    }

    return -1;
}



/**************************************************************************************
                   STK_DEPODAKI_URUN_MIKTARINI_BUL
***************************************************************************************/

double STK_DEPODAKI_URUN_MIKTARINI_BUL ( int p_depo_id, int p_urun_id )
{

    double TOPLAM_DEPOYA_GIREN_ARRAY        [ E9_ARRAY_SIZE ];
    double TOPLAM_DEPODAN_CIKAN_ARRAY       [ E9_ARRAY_SIZE ];


    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_depodaki_urunler",
                             "toplam_depoya_giren_array, toplam_depodan_cikan_array ",
                             "urun_id = :urun_id AND depo_id = :depo_id");

    sql_query.SET_VALUE(":urun_id", p_urun_id );
    sql_query.SET_VALUE(":depo_id", p_depo_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return 0.00;
    }
    sql_query.NEXT();

    QString toplam_depoya_giren_array           = sql_query.VALUE(4).toString();
    QString toplam_depodan_cikan_array          = sql_query.VALUE(5).toString();



    UNPACK_DOUBLE_ARRAY ( toplam_depoya_giren_array,          TOPLAM_DEPOYA_GIREN_ARRAY,          E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( toplam_depodan_cikan_array,         TOPLAM_DEPODAN_CIKAN_ARRAY,         E9_ARRAY_SIZE );


    return ROUND ( TOPLAM_DEPOYA_GIREN_ARRAY [E9_SON_BAKIYE] - TOPLAM_DEPODAN_CIKAN_ARRAY[E9_SON_BAKIYE] );
}

/**************************************************************************************
                   STK_PARTIDEKI_URUN_MIKTARINI_BUL
***************************************************************************************/

double STK_PARTIDEKI_URUN_MIKTARINI_BUL ( int p_parti_id )
{
    double TOPLAM_PARTIYE_GIREN_ARRAY               [ E9_ARRAY_SIZE ];
    double TOPLAM_PARTIDEN_CIKAN_ARRAY              [ E9_ARRAY_SIZE ];


    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "stk_urunun_partileri",
                           "toplam_partiye_giren_array, toplam_partiden_cikan_array ",
                           "parti_id = :parti_id " );

    query.SET_VALUE ( ":parti_id", p_parti_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    QString toplam_partiye_giren_array          = query.VALUE(4).toString();
    QString toplam_partiden_cikan_array         = query.VALUE(5).toString();



    UNPACK_DOUBLE_ARRAY ( toplam_partiye_giren_array,       TOPLAM_PARTIYE_GIREN_ARRAY,         E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( toplam_partiden_cikan_array,      TOPLAM_PARTIDEN_CIKAN_ARRAY,        E9_ARRAY_SIZE );
    return ROUND ( TOPLAM_PARTIYE_GIREN_ARRAY[E9_SON_BAKIYE] - TOPLAM_PARTIDEN_CIKAN_ARRAY[E9_SON_BAKIYE] );
}

/**************************************************************************************
                   STK_URUN_KARTINDAKI_STOK_MIKTARINI_ARTIR
***************************************************************************************/

void STK_URUN_KARTINDAKI_STOK_MIKTARINI_ARTIR ( int p_urun_id, int p_gun_no, double p_stoga_giren_miktar, double p_stoktan_cikan_miktar,
                                               ADAK_SQL * P_GELEN_DB )
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    if ( p_urun_id < 1 ) {
        return;
    }

    double TOPLAM_STOK_GIRIS_ARRAY          [ E9_ARRAY_SIZE ];
    double TOPLAM_STOK_CIKIS_ARRAY          [ E9_ARRAY_SIZE ];

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT ( "stk_urunler",
                           "toplam_stoga_giren_array, toplam_stoktan_cikan_array ",
                           "urun_id = :urun_id" );

    query.SET_VALUE ( ":urun_id", p_urun_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    QString toplam_stoga_giren_array             = query.VALUE ( 0 ).toString();
    QString toplam_stoktan_cikan_array           = query.VALUE ( 1 ).toString();


    UNPACK_DOUBLE_ARRAY ( toplam_stoga_giren_array,                 TOPLAM_STOK_GIRIS_ARRAY,       E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( toplam_stoktan_cikan_array,               TOPLAM_STOK_CIKIS_ARRAY,       E9_ARRAY_SIZE );


    for ( int i = p_gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
        if ( p_stoga_giren_miktar NE 0.00 ) {
            TOPLAM_STOK_GIRIS_ARRAY[i]     = ROUND ( TOPLAM_STOK_GIRIS_ARRAY[i] + p_stoga_giren_miktar );
        }
        if ( p_stoktan_cikan_miktar NE 0.00 ) {
            TOPLAM_STOK_CIKIS_ARRAY[i] = ROUND ( TOPLAM_STOK_CIKIS_ARRAY[i] + p_stoktan_cikan_miktar );
        }
    }

    toplam_stoga_giren_array            = PACK_DOUBLE_ARRAY ( TOPLAM_STOK_GIRIS_ARRAY,          E9_ARRAY_SIZE );
    toplam_stoktan_cikan_array          = PACK_DOUBLE_ARRAY ( TOPLAM_STOK_CIKIS_ARRAY,          E9_ARRAY_SIZE );



    query.PREPARE_UPDATE ( "stk_urunler ", "urun_id",
                           "toplam_stoga_giren_array             , "
                           "toplam_stoktan_cikan_array             ",
                           "urun_id   = :urun_id" );

    query.SET_VALUE ( ":toplam_stoga_giren_array",          toplam_stoga_giren_array            );
    query.SET_VALUE ( ":toplam_stoktan_cikan_array",        toplam_stoktan_cikan_array          );


    query.SET_VALUE ( ":urun_id",  p_urun_id  );

    query.UPDATE();
}

/**************************************************************************************
                   STK_URUN_KARTINDAKI_STOK_MIKTARINI_AZALT
***************************************************************************************/

void STK_URUN_KARTINDAKI_STOK_MIKTARINI_AZALT ( int p_urun_id, int p_gun_no, double p_stoga_giren_miktar, double p_stoktan_cikan_miktar)
{
    if ( p_urun_id < 1 ) {
        return;
    }

    double TOPLAM_STOK_GIRIS_ARRAY              [ E9_ARRAY_SIZE ];
    double TOPLAM_STOK_CIKIS_ARRAY              [ E9_ARRAY_SIZE ];


    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "stk_urunler", "toplam_stoga_giren_array, toplam_stoktan_cikan_array ",
                           "urun_id = :urun_id" );

    query.SET_VALUE ( ":urun_id", p_urun_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    QString toplam_stoga_giren_array             = query.VALUE ( 0 ).toString();
    QString toplam_stoktan_cikan_array           = query.VALUE ( 1 ).toString();


    UNPACK_DOUBLE_ARRAY ( toplam_stoga_giren_array,             TOPLAM_STOK_GIRIS_ARRAY,            E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( toplam_stoktan_cikan_array,           TOPLAM_STOK_CIKIS_ARRAY,            E9_ARRAY_SIZE );


    for ( int i = p_gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
        if ( p_stoga_giren_miktar NE 0.00 ) {
            TOPLAM_STOK_GIRIS_ARRAY[i] = ROUND ( TOPLAM_STOK_GIRIS_ARRAY[i] - p_stoga_giren_miktar );
        }
        if ( p_stoktan_cikan_miktar NE 0.00 ) {
            TOPLAM_STOK_CIKIS_ARRAY[i] = ROUND ( TOPLAM_STOK_CIKIS_ARRAY[i] - p_stoktan_cikan_miktar );
        }
    }

    toplam_stoga_giren_array            = PACK_DOUBLE_ARRAY ( TOPLAM_STOK_GIRIS_ARRAY,          E9_ARRAY_SIZE );
    toplam_stoktan_cikan_array          = PACK_DOUBLE_ARRAY ( TOPLAM_STOK_CIKIS_ARRAY,          E9_ARRAY_SIZE );



    query.PREPARE_UPDATE ( "stk_urunler ", "urun_id",
                           "toplam_stoga_giren_array             , "
                           "toplam_stoktan_cikan_array             ",
                           "urun_id = :urun_id" );

    query.SET_VALUE ( ":toplam_stoga_giren_array",          toplam_stoga_giren_array            );
    query.SET_VALUE ( ":toplam_stoktan_cikan_array",        toplam_stoktan_cikan_array          );


    query.SET_VALUE ( ":urun_id",  p_urun_id  );

    query.UPDATE();


}

/**************************************************************************************
                   STK_DEPODAKI_STOK_MIKTARINI_ARTIR
***************************************************************************************/

void STK_DEPODAKI_STOK_MIKTARINI_ARTIR ( int p_depo_id, int p_urun_id, int p_gun_no, double p_depoya_giren_miktar,
                                        double p_depodan_cikan_miktar, ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    if ( p_urun_id < 1 ) {
        return;
    }

    int    kayit_id         =       0;

    double TOPLAM_STOK_GIRIS_ARRAY          [ E9_ARRAY_SIZE ];
    double TOPLAM_STOK_CIKIS_ARRAY          [ E9_ARRAY_SIZE ];


    //! DEPO ICIN URUN ARRAYI OLUSTURULUYOR
    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT ( "stk_depodaki_urunler","kayit_id",
                           "depo_id = :depo_id AND urun_id = :urun_id");

    query.SET_VALUE ( ":depo_id", p_depo_id );
    query.SET_VALUE ( ":urun_id", p_urun_id );

    if ( query.SELECT() EQ 0 ) {
        query.PREPARE_INSERT ( "stk_depodaki_urunler", "kayit_id", "depo_id, urun_id ");

        query.SET_VALUE ( ":depo_id", p_depo_id );
        query.SET_VALUE ( ":urun_id", p_urun_id );

        kayit_id = query.INSERT();
    }
    else {
        query.NEXT();
        kayit_id = query.VALUE(0).toInt();
    }

    query.PREPARE_SELECT ( "stk_depodaki_urunler",
                           "toplam_depoya_giren_array, toplam_depodan_cikan_array ",
                           "kayit_id = :kayit_id " );

    query.SET_VALUE      ( ":kayit_id", kayit_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    QString toplam_depoya_giren_array               = query.VALUE ( 0 ).toString();
    QString toplam_depodan_cikan_array              = query.VALUE ( 1 ).toString();



    UNPACK_DOUBLE_ARRAY ( toplam_depoya_giren_array,            TOPLAM_STOK_GIRIS_ARRAY,         E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( toplam_depodan_cikan_array,           TOPLAM_STOK_CIKIS_ARRAY,         E9_ARRAY_SIZE );


    for ( int i = p_gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
        if ( p_depoya_giren_miktar NE 0.00 ) {
            TOPLAM_STOK_GIRIS_ARRAY[i]   = ROUND ( TOPLAM_STOK_GIRIS_ARRAY[i] + p_depoya_giren_miktar );
        }
        if ( p_depodan_cikan_miktar NE 0.00 ) {
            TOPLAM_STOK_CIKIS_ARRAY[i] = ROUND ( TOPLAM_STOK_CIKIS_ARRAY[i] + p_depodan_cikan_miktar );
        }
    }

    toplam_depoya_giren_array          = PACK_DOUBLE_ARRAY ( TOPLAM_STOK_GIRIS_ARRAY,          E9_ARRAY_SIZE );
    toplam_depodan_cikan_array         = PACK_DOUBLE_ARRAY ( TOPLAM_STOK_CIKIS_ARRAY,          E9_ARRAY_SIZE );


    query.PREPARE_UPDATE ( "stk_depodaki_urunler ", "kayit_id",
                           "toplam_depoya_giren_array               , "
                           "toplam_depodan_cikan_array                ",
                           "kayit_id = :kayit_id" );


    query.SET_VALUE ( ":toplam_depoya_giren_array",         toplam_depoya_giren_array       );
    query.SET_VALUE ( ":toplam_depodan_cikan_array",        toplam_depodan_cikan_array      );
    query.SET_VALUE ( ":kayit_id",                          kayit_id                        );

    query.UPDATE    ();

}

/**************************************************************************************
                   STK_DEPODAKI_STOK_MIKTARINI_AZALT
***************************************************************************************/

void STK_DEPODAKI_STOK_MIKTARINI_AZALT ( int p_depo_id, int p_urun_id, int p_gun_no, double p_depoya_giren_miktar, double p_depodan_cikan_miktar )
{
    if ( p_urun_id < 1 ) {
        return;
    }

    int    kayit_id         =       0;

    double TOPLAM_STOK_GIRIS_ARRAY      [ E9_ARRAY_SIZE ];
    double TOPLAM_STOK_CIKIS_ARRAY      [ E9_ARRAY_SIZE ];


    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "stk_depodaki_urunler","kayit_id",
                           "depo_id = :depo_id AND urun_id = :urun_id");

    query.SET_VALUE ( ":depo_id", p_depo_id );
    query.SET_VALUE ( ":urun_id", p_urun_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();
    kayit_id = query.VALUE(0).toInt();

    query.PREPARE_SELECT ( "stk_depodaki_urunler",
                           "toplam_depoya_giren_array, toplam_depodan_cikan_array ",
                           "kayit_id = :kayit_id " );

    query.SET_VALUE      ( ":kayit_id", kayit_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    QString toplam_depoya_giren_array       = query.VALUE ( 0 ).toString();
    QString toplam_depodan_cikan_array      = query.VALUE ( 1 ).toString();



    UNPACK_DOUBLE_ARRAY ( toplam_depoya_giren_array,            TOPLAM_STOK_GIRIS_ARRAY,            E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( toplam_depodan_cikan_array,           TOPLAM_STOK_CIKIS_ARRAY,            E9_ARRAY_SIZE );


    for ( int i = p_gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
        if ( p_depoya_giren_miktar NE 0.00 ) {
            TOPLAM_STOK_GIRIS_ARRAY[i] = ROUND ( TOPLAM_STOK_GIRIS_ARRAY[i] - p_depoya_giren_miktar );
        }
        if ( p_depodan_cikan_miktar NE 0.00 ) {
            TOPLAM_STOK_CIKIS_ARRAY[i] = ROUND ( TOPLAM_STOK_CIKIS_ARRAY[i] - p_depodan_cikan_miktar );
        }
    }

    toplam_depoya_giren_array              = PACK_DOUBLE_ARRAY ( TOPLAM_STOK_GIRIS_ARRAY,      E9_ARRAY_SIZE );
    toplam_depodan_cikan_array             = PACK_DOUBLE_ARRAY ( TOPLAM_STOK_CIKIS_ARRAY,      E9_ARRAY_SIZE );


    query.PREPARE_UPDATE ( "stk_depodaki_urunler ", "kayit_id",
                           "toplam_depoya_giren_array               ,"
                           "toplam_depodan_cikan_array               ",
                           "kayit_id  = :kayit_id" );


    query.SET_VALUE ( ":toplam_depoya_giren_array",         toplam_depoya_giren_array           );
    query.SET_VALUE ( ":toplam_depodan_cikan_array",        toplam_depodan_cikan_array          );
    query.SET_VALUE ( ":kayit_id",                          kayit_id                            );

    query.UPDATE    ();


}

/**************************************************************************************
                   STK_PARTIDEKI_URUN_MIKTARINI_ARTIR
***************************************************************************************/

int STK_PARTIDEKI_URUN_MIKTARINI_ARTIR ( int p_depo_id, int p_urun_id, int p_gun_no, QString p_parti_kodu,
                                         double p_giren_urun_miktari, double p_cikan_urun_miktari,
                                         QString  p_uretim_tarihi, QString p_son_kul_tarihi,ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }


    if ( p_urun_id < 1 ) {
        return 0;
    }

    SQL_QUERY query    ( USE_DB );

    double TOPLAM_PARTIYE_GIREN_ARRAY           [ E9_ARRAY_SIZE ];
    double TOPLAM_PARTIDEN_CIKAN_ARRAY          [ E9_ARRAY_SIZE ];


    int parti_id = 0;

    query.PREPARE_SELECT ( "stk_urunun_partileri", "parti_id", "parti_kodu = :parti_kodu "
                           "AND urun_id = :urun_id AND depo_id = :depo_id ");
    query.SET_VALUE      ( ":parti_kodu", p_parti_kodu );
    query.SET_VALUE      ( ":urun_id"   , p_urun_id    );
    query.SET_VALUE      ( ":depo_id"   , p_depo_id    );

    if ( query.SELECT() EQ 0 ) {

        query.PREPARE_INSERT ( "stk_urunun_partileri", "parti_id", "depo_id, urun_id, "
                               "parti_kodu, urun_uretim_tarihi, urun_son_kul_tarihi ");

        query.SET_VALUE(":depo_id"               , p_depo_id        );
        query.SET_VALUE(":urun_id"               , p_urun_id        );
        query.SET_VALUE(":parti_kodu"            , p_parti_kodu     );
        query.SET_VALUE(":urun_uretim_tarihi"    , p_uretim_tarihi  );
        query.SET_VALUE(":urun_son_kul_tarihi"   , p_son_kul_tarihi );
        parti_id = query.INSERT();

    }
    else {
        query.NEXT();
        parti_id = query.VALUE(0).toInt();
    }

    query.PREPARE_SELECT ( "stk_urunun_partileri",
                           "toplam_partiye_giren_array, toplam_partiden_cikan_array ",
                           "parti_id = :parti_id" );

    query.SET_VALUE ( ":parti_id", parti_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    QString toplam_partiye_giren_array                  = query.VALUE ( 4 ).toString();
    QString toplam_partiden_cikan_array                 = query.VALUE ( 5 ).toString();

    UNPACK_DOUBLE_ARRAY ( toplam_partiye_giren_array,           TOPLAM_PARTIYE_GIREN_ARRAY,         E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( toplam_partiden_cikan_array,          TOPLAM_PARTIDEN_CIKAN_ARRAY,        E9_ARRAY_SIZE );


    for ( int i = p_gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
        if ( p_giren_urun_miktari NE 0.00 ) {
            TOPLAM_PARTIYE_GIREN_ARRAY[i] = ROUND ( TOPLAM_PARTIYE_GIREN_ARRAY[i] + p_giren_urun_miktari );
        }
        if ( p_cikan_urun_miktari NE 0.00 ) {
            TOPLAM_PARTIDEN_CIKAN_ARRAY[i] = ROUND ( TOPLAM_PARTIDEN_CIKAN_ARRAY[i] + p_cikan_urun_miktari );
        }
    }

    toplam_partiye_giren_array        = PACK_DOUBLE_ARRAY ( TOPLAM_PARTIYE_GIREN_ARRAY,           E9_ARRAY_SIZE );
    toplam_partiden_cikan_array       = PACK_DOUBLE_ARRAY ( TOPLAM_PARTIDEN_CIKAN_ARRAY,          E9_ARRAY_SIZE );


    query.PREPARE_UPDATE ( "stk_urunun_partileri ", "parti_id",
                           "toplam_partiye_giren_array        , "
                           "toplam_partiden_cikan_array         ",
                           "parti_id  = :parti_id " );


    query.SET_VALUE ( ":toplam_partiye_giren_array",        toplam_partiye_giren_array              );
    query.SET_VALUE ( ":toplam_partiden_cikan_array",       toplam_partiden_cikan_array             );

    query.SET_VALUE ( ":parti_id", parti_id   );

    query.UPDATE();

    return parti_id;
}

/**************************************************************************************
                   STK_PARTIDEKI_URUN_MIKTARINI_AZALT
***************************************************************************************/

void STK_PARTIDEKI_URUN_MIKTARINI_AZALT ( int p_parti_id, int p_gun_no, double p_giren_urun_miktari, double p_cikan_urun_miktari )
{
    if ( p_parti_id < 1 ) {
        return;
    }

    SQL_QUERY query ( DB );

    double TOPLAM_PARTIYE_GIREN_ARRAY           [ E9_ARRAY_SIZE ];
    double TOPLAM_PARTIDEN_CIKAN_ARRAY          [ E9_ARRAY_SIZE ];

    query.PREPARE_SELECT ( "stk_urunun_partileri",
                           "toplam_partiye_giren_array, toplam_partiden_cikan_array ",
                           "parti_id = :parti_id" );

    query.SET_VALUE ( ":parti_id", p_parti_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    QString toplam_partiye_giren_array                  = query.VALUE ( 4 ).toString();
    QString toplam_partiden_cikan_array                 = query.VALUE ( 5 ).toString();

    UNPACK_DOUBLE_ARRAY ( toplam_partiye_giren_array,           TOPLAM_PARTIYE_GIREN_ARRAY,         E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( toplam_partiden_cikan_array,          TOPLAM_PARTIDEN_CIKAN_ARRAY,        E9_ARRAY_SIZE );


    for ( int i = p_gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
        if ( p_giren_urun_miktari NE 0.00 ) {
            TOPLAM_PARTIYE_GIREN_ARRAY[i] = ROUND ( TOPLAM_PARTIYE_GIREN_ARRAY[i] - p_giren_urun_miktari );
        }
        if ( p_cikan_urun_miktari NE 0.00 ) {
            TOPLAM_PARTIDEN_CIKAN_ARRAY[i] = ROUND ( TOPLAM_PARTIDEN_CIKAN_ARRAY[i] - p_cikan_urun_miktari );
        }
    }

    toplam_partiye_giren_array          = PACK_DOUBLE_ARRAY ( TOPLAM_PARTIYE_GIREN_ARRAY,           E9_ARRAY_SIZE );
    toplam_partiden_cikan_array         = PACK_DOUBLE_ARRAY ( TOPLAM_PARTIDEN_CIKAN_ARRAY,          E9_ARRAY_SIZE );

    query.PREPARE_UPDATE ( "stk_urunun_partileri ", "parti_id",
                           "toplam_partiye_giren_array         , "
                           "toplam_partiden_cikan_array          ",
                           "parti_id = :parti_id " );


    query.SET_VALUE ( ":toplam_partiye_giren_array",        toplam_partiye_giren_array              );
    query.SET_VALUE ( ":toplam_partiden_cikan_array",       toplam_partiden_cikan_array             );

    query.SET_VALUE ( ":parti_id", p_parti_id   );

    query.UPDATE();
}

/**************************************************************************************
                   STK_IRS_SATIRINDAN_TUM_SERI_NOLARI_SIL
***************************************************************************************/

void STK_IRS_SATIRINDAN_TUM_SERI_NOLARI_SIL ( int p_irsaliye_satiri_id )
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_DELETE("stk_satirdaki_seri_nolari",
                             "irsaliye_satiri_id = :irsaliye_satiri_id");
    sql_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );
    sql_query.DELETE();
}



/**************************************************************************************
                   STK_PARTIYI_KAYITLARDAN_SIL
***************************************************************************************/

void STK_PARTIYI_KAYITLARDAN_SIL ( int p_parti_id )
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_DELETE("stk_urunun_partileri","parti_id = :parti_id");
    sql_query.SET_VALUE(":parti_id", p_parti_id );
    sql_query.DELETE();
}

/**************************************************************************************
                   STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_GIRIS_CIKISLARINI_DUZENLE
***************************************************************************************/

void STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_GIRIS_CIKISLARINI_DUZENLE ( int p_irsaliye_id,
                                                                            QDate p_old_fis_tarihi,
                                                                            QDate p_new_fis_tarihi,
                                                                            int p_depo_id,
                                                                            int p_alis_satis_turu)
{

    SQL_QUERY sql_query  ( DB );
    SQL_QUERY depo_query ( DB );
    SQL_QUERY query(DB);

    double TOPLAM_STOK_GIRIS_ARRAY          [ E9_ARRAY_SIZE ];
    double TOPLAM_STOK_CIKIS_ARRAY          [ E9_ARRAY_SIZE ];


    bool    fis_tarihi_degisti_mi   =   false;

    int     old_gun_no              =   MALI_YIL_ARRAY_INDIS ( p_old_fis_tarihi );
    int     new_gun_no              =   MALI_YIL_ARRAY_INDIS ( p_new_fis_tarihi );

    if ( p_old_fis_tarihi NE p_new_fis_tarihi ) {
        fis_tarihi_degisti_mi = true;
    }

    sql_query.PREPARE_SELECT("stk_fis_satirlari, stk_urunler",
                             "satirdaki_urun_miktari, stk_fis_satirlari.urun_id, "
                             "stok_takip_sekli, parti_id, temel_birim_katsayisi ",
                             "stk_fis_satirlari.urun_id = stk_urunler.urun_id "
                             "AND stk_fis_satirlari.urun_id > 0 "
                             "AND fis_id = :fis_id " );
    sql_query.SET_VALUE(":fis_id", p_irsaliye_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    while ( sql_query.NEXT() EQ true ) {

        double  satirdaki_urun_miktari    = sql_query.VALUE ( 0 ).toDouble();
        int     urun_id                   = sql_query.VALUE ( 1 ).toInt();
        int     stok_takip_sekli          = sql_query.VALUE ( 2 ).toInt();
        int     parti_id                  = sql_query.VALUE ( 3 ).toInt();
        double  temel_birim_katsayisi     = sql_query.VALUE ( 4 ).toDouble();
        double  temel_birim_urun_miktari  = ROUND ( satirdaki_urun_miktari * temel_birim_katsayisi );

        depo_query.PREPARE_SELECT ( "stk_depodaki_urunler",
                                    "kayit_id, "
                                    "toplam_depoya_giren_array, toplam_depodan_cikan_array    ",
                                    "urun_id = :urun_id AND depo_id = :depo_id    ");

        depo_query.SET_VALUE ( ":urun_id", urun_id   );
        depo_query.SET_VALUE ( ":depo_id", p_depo_id );

        if ( depo_query.SELECT() EQ 0 ) {
            return;
        }
        depo_query.NEXT();

        int  kayit_id = depo_query.VALUE(0).toInt();

        QString toplam_depoya_giren_array               = depo_query.VALUE ( 1 ).toString();
        QString toplam_depodan_cikan_array              = depo_query.VALUE ( 2 ).toString();

        UNPACK_DOUBLE_ARRAY ( toplam_depoya_giren_array,        TOPLAM_STOK_GIRIS_ARRAY,        E9_ARRAY_SIZE );
        UNPACK_DOUBLE_ARRAY ( toplam_depodan_cikan_array,       TOPLAM_STOK_CIKIS_ARRAY,        E9_ARRAY_SIZE );

        switch ( p_alis_satis_turu ) {

            case ENUM_ALIS_IRSALIYESI_FATURASI :

                if ( fis_tarihi_degisti_mi EQ true ) {
                    for ( int i = 0; i < old_gun_no; i++ ) {
                        TOPLAM_STOK_GIRIS_ARRAY[i] = ROUND ( TOPLAM_STOK_GIRIS_ARRAY[i] - temel_birim_urun_miktari );
                    }

                    for ( int i = 0; i < new_gun_no; i++ ) {
                        TOPLAM_STOK_GIRIS_ARRAY[i] = ROUND ( TOPLAM_STOK_GIRIS_ARRAY[i] + temel_birim_urun_miktari );
                    }

                    query.PREPARE_UPDATE("stk_depodaki_urunler ", "kayit_id",
                                         "toplam_depoya_giren_array       ",
                                         "kayit_id = :kayit_id");
                    query.SET_VALUE(":toplam_depoya_giren_array"      , PACK_DOUBLE_ARRAY(TOPLAM_STOK_GIRIS_ARRAY      , E9_ARRAY_SIZE));
                    query.SET_VALUE(":kayit_id"                       , kayit_id);
                    query.UPDATE();

                }

                switch ( stok_takip_sekli ) {

                    case ENUM_BASIT_TAKIP       :
                    case ENUM_SERI_NO_ILE_TAKIP :
                    default                :
                        break;

                    case ENUM_PARTI_TAKIBI :

                        if ( fis_tarihi_degisti_mi EQ true ) {
                            for ( int i = 0; i < old_gun_no; i++ ) {
                                TOPLAM_STOK_GIRIS_ARRAY[i] = ROUND ( TOPLAM_STOK_GIRIS_ARRAY[i] - temel_birim_urun_miktari );
                            }

                            for ( int i = 0; i < new_gun_no; i++ ) {
                                TOPLAM_STOK_GIRIS_ARRAY[i] = ROUND ( TOPLAM_STOK_GIRIS_ARRAY[i] + temel_birim_urun_miktari );
                            }
                        }

                        query.PREPARE_UPDATE("stk_urunun_partileri", "parti_id",
                                             "toplam_partiye_giren_array   ",
                                             "parti_id = :parti_id");
                        query.SET_VALUE(":toplam_partiye_giren_array"  , PACK_DOUBLE_ARRAY(TOPLAM_STOK_GIRIS_ARRAY      , E9_ARRAY_SIZE));
                        query.SET_VALUE(":parti_id"                    , parti_id);
                        query.UPDATE();

                        break;
                    }
                    break;

            case ENUM_SATIS_IRSALIYESI_FATURASI :

                if ( fis_tarihi_degisti_mi EQ true ) {
                    for ( int i = 0; i < old_gun_no; i++ ) {
                        TOPLAM_STOK_CIKIS_ARRAY[i] = ROUND ( TOPLAM_STOK_CIKIS_ARRAY[i] - temel_birim_urun_miktari );
                    }
                    for ( int i = 0; i < new_gun_no; i++ ) {
                        TOPLAM_STOK_CIKIS_ARRAY[i] = ROUND ( TOPLAM_STOK_CIKIS_ARRAY[i] + temel_birim_urun_miktari );
                    }
                }

                query.PREPARE_UPDATE("stk_depodaki_urunler", "kayit_id",
                                     "toplam_depodan_cikan_array     ",
                                     "kayit_id = :kayit_id");
                query.SET_VALUE(":toplam_depodan_cikan_array"   , PACK_DOUBLE_ARRAY(TOPLAM_STOK_CIKIS_ARRAY      , E9_ARRAY_SIZE));
                query.SET_VALUE(":kayit_id"                     , kayit_id);
                query.UPDATE();

                switch ( stok_takip_sekli ) {

                    case ENUM_BASIT_TAKIP       :
                    case ENUM_SERI_NO_ILE_TAKIP :
                    default                :
                        break;

                    case ENUM_PARTI_TAKIBI :
                    {
                        SQL_QUERY query(DB);

                        query.PREPARE_SELECT("stk_urunun_partileri", "toplam_partiden_cikan_array ", "parti_id = :parti_id");
                        query.SET_VALUE(":parti_id" , parti_id);

                        if ( query.SELECT() NE 0 ) {
                            query.NEXT();

                            UNPACK_DOUBLE_ARRAY(query.VALUE("toplam_partiden_cikan_array").toString(), TOPLAM_STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);

                        }

                        if ( fis_tarihi_degisti_mi EQ true ) {
                            for ( int i = 0; i < old_gun_no; i++ ) {
                                TOPLAM_STOK_CIKIS_ARRAY[i] = ROUND ( TOPLAM_STOK_CIKIS_ARRAY[i] - temel_birim_urun_miktari );
                            }
                            for ( int i = 0; i < new_gun_no; i++ ) {
                                TOPLAM_STOK_CIKIS_ARRAY[i] = ROUND ( TOPLAM_STOK_CIKIS_ARRAY[i] + temel_birim_urun_miktari );
                            }
                        }

                        query.PREPARE_UPDATE("stk_urunun_partileri", "parti_id",
                                             "toplam_partiden_cikan_array   ",
                                             "parti_id = :parti_id");
                        query.SET_VALUE(":toplam_partiden_cikan_array"  , PACK_DOUBLE_ARRAY(TOPLAM_STOK_CIKIS_ARRAY , E9_ARRAY_SIZE));
                        query.SET_VALUE(":parti_id" , parti_id);
                        query.UPDATE();

                        break;
                    }
                    break;
                }
                default :
                    break;
        }
    }

    if ( fis_tarihi_degisti_mi EQ true ) {
        sql_query.PREPARE_UPDATE ( "stk_fisler ", "fis_id",
                                   "fis_tarihi      ",
                                   "fis_id   = :fis_id"  );

        sql_query.SET_VALUE      ( ":fis_tarihi", p_new_fis_tarihi.toString ( "yyyy.MM.dd" ) );
        sql_query.SET_VALUE      ( ":fis_id",     p_irsaliye_id );
        sql_query.UPDATE();
    }
}


/**************************************************************************************
                   STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_MIKTARLARI_KONTROLU
***************************************************************************************/


bool STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_MIKTARLARI_KONTROLU ( int       p_irsaliye_id,
                                                                      QDate     p_old_fis_tarihi,
                                                                      QDate     p_new_fis_tarihi,
                                                                      int       p_depo_id,
                                                                      int       p_alis_satis_turu,
                                                                      QWidget * p_parent )
{

    // Fis tarihi ileriye alındıysa sadece giriş fişleri için eski fiş tarihi ile yeni fiş tarihi arasında
    //kalan günlerde stoğun eksiye düşmesi söz konusu olabilir

    // Fiş tarihi geriye alındıysa sadece çıkış fişleri eski tarih ile yeni tarih
    // arasında kalan günlerde stoğun eksiye düşmesi söz konusu olabilir

    SQL_QUERY sql_query  ( DB );
    SQL_QUERY depo_query ( DB );

    int old_gun_no = MALI_YIL_ARRAY_INDIS ( p_old_fis_tarihi );
    int new_gun_no = MALI_YIL_ARRAY_INDIS ( p_new_fis_tarihi );

    double STOK_GIRIS_ARRAY [ E9_ARRAY_SIZE ];
    double STOK_CIKIS_ARRAY [ E9_ARRAY_SIZE ];

    sql_query.PREPARE_SELECT("stk_fis_satirlari, stk_urunler",
                             "satirdaki_urun_miktari, stk_fis_satirlari.urun_id, "
                             "urun_kodu, stk_urunler.urun_adi, stok_takip_sekli, parti_id, temel_birim_katsayisi ",
                             "stk_fis_satirlari.urun_id = stk_urunler.urun_id "
                             "AND stk_fis_satirlari.urun_id > 0 "
                             "AND fis_id = :fis_id " );
    sql_query.SET_VALUE(":fis_id", p_irsaliye_id );

    if ( sql_query.SELECT() EQ 0 ) {
        // UYARI VERMEYE GEREK YOK CUNKU SATIR  YOK ISE URUN DE YOKTUR
        // STOK KONTROLU YAPMAYA GEREK YOK
        return true;
    }

    while ( sql_query.NEXT() EQ true ) {

        double  satirdaki_urun_miktari = sql_query.VALUE ( 0 ).toDouble();
        int     urun_id                = sql_query.VALUE ( 1 ).toInt();
        QString urun_kodu              = sql_query.VALUE ( 2 ).toString();
        QString urun_adi               = sql_query.VALUE ( 3 ).toString();
        int     stok_takip_sekli       = sql_query.VALUE ( 4 ).toInt();
        int     parti_id               = sql_query.VALUE ( 5 ).toInt();
        double  temel_birim_katsayisi  = sql_query.VALUE ( 6 ).toDouble();
        double  temel_birim_miktari    = ROUND ( satirdaki_urun_miktari * temel_birim_katsayisi );

        switch ( p_alis_satis_turu ) {

            case ENUM_ALIS_IRSALIYESI_FATURASI :
            default                                   :

                if ( p_new_fis_tarihi > p_old_fis_tarihi ) {

                    switch ( stok_takip_sekli ) {
                        case ENUM_BASIT_TAKIP       :
                        case ENUM_SERI_NO_ILE_TAKIP :
                        default                     :
                        {
                            depo_query.PREPARE_SELECT("stk_depodaki_urunler",
                                                      "ent_depoya_giren_array,ent_depodan_cikan_array,"
                                                      "toplam_depoya_giren_array,toplam_depodan_cikan_array ",
                                                      "urun_id = :urun_id AND depo_id = :depo_id");
                            depo_query.SET_VALUE(":urun_id", urun_id   );
                            depo_query.SET_VALUE(":depo_id", p_depo_id );
                            if ( depo_query.SELECT() EQ 0 ) {
                                ADAK_WARNING( QObject::tr("Ürün ve depo bilgileri alınamadı"), NULL, p_parent );
                                return false;
                            }
                            depo_query.NEXT();

                            if ( E9_KULLANICININ_CALISTIGI_SIRKET() EQ 1 ) {
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_depoya_giren_array").toString(),STOK_GIRIS_ARRAY,E9_ARRAY_SIZE);
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_depodan_cikan_array").toString(),STOK_CIKIS_ARRAY,E9_ARRAY_SIZE);
                            }
                            else {
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_depoya_giren_array").toString(),STOK_GIRIS_ARRAY,E9_ARRAY_SIZE);
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_depodan_cikan_array").toString(),STOK_CIKIS_ARRAY,E9_ARRAY_SIZE);
                            }

                            for ( int i = old_gun_no; i < new_gun_no; i++ ) {
                                double depodaki_urun_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY [i];
                                if ( temel_birim_miktari > depodaki_urun_miktari ) {
                                    ADAK_WARNING( QObject::tr( "Fiş tarihi güncellenirse "
                                                                                 + urun_kodu.toLatin1() + " - " + urun_adi.toLatin1()
                                                                                 + " ürünü için depodaki miktar eksiye düşüyor." +
                                                                                 "\nGüncelleme işlemi gerçekleştirilemez."), NULL, p_parent );
                                    return false;
                                }
                            }
                            break;
                        }
                        case ENUM_PARTI_TAKIBI :

                            depo_query.PREPARE_SELECT("stk_urunun_partileri",
                                                      "ent_partiye_giren_array , ent_partiden_cikan_array,"
                                                      "toplam_partiye_giren_array,toplam_partiden_cikan_array ",
                                                      "parti_id = :parti_id");
                            depo_query.SET_VALUE("parti_id" , parti_id);

                            if ( depo_query.SELECT() EQ 0 ) {
                                break;
                            }

                            depo_query.NEXT();

                            if ( E9_KULLANICININ_CALISTIGI_SIRKET() EQ 1 ) {
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_partiye_giren_array").toString(),
                                                    STOK_GIRIS_ARRAY,E9_ARRAY_SIZE);
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_partiden_cikan_array").toString(),
                                                    STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);
                            }
                            else {
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_partiye_giren_array").toString(),
                                                    STOK_GIRIS_ARRAY,E9_ARRAY_SIZE);
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_partiden_cikan_array").toString(),
                                                    STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);
                            }

                            for ( int i = old_gun_no; i < new_gun_no; i ++ ) {
                                double partideki_urun_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY[i];
                                if ( temel_birim_miktari > partideki_urun_miktari ) {
                                    ADAK_WARNING( QObject::tr( "Fiş tarihi güncellenirse "
                                                                                 + urun_kodu.toLatin1() + " - " + urun_adi.toLatin1()
                                                                                 + " ürünü için partideki miktar eksiye düşüyor." +
                                                                                 "\nGüncelleme işlemi gerçekleştirilemez."), NULL, p_parent );
                                    return false;
                            }
                        }
                        break;
                    }

                }
                break;


            case ENUM_SATIS_IRSALIYESI_FATURASI :

                if ( p_old_fis_tarihi > p_new_fis_tarihi ) {

                    switch ( stok_takip_sekli ) {
                        case ENUM_BASIT_TAKIP       :
                        case ENUM_SERI_NO_ILE_TAKIP :
                        default                :
                        {
                            depo_query.PREPARE_SELECT("stk_depodaki_urunler",
                                                      "ent_depoya_giren_array,ent_depodan_cikan_array,"
                                                      "toplam_depoya_giren_array,toplam_depodan_cikan_array ",
                                                      "urun_id = :urun_id AND depo_id = :depo_id");
                            depo_query.SET_VALUE(":urun_id", urun_id );
                            depo_query.SET_VALUE(":depo_id", p_depo_id );
                            if ( depo_query.SELECT() EQ 0 ) {
                                ADAK_WARNING( QObject::tr("Ürün ve depo bilgileri alınamadı"), NULL, p_parent );
                                return false;
                            }
                            depo_query.NEXT();

                            if ( E9_KULLANICININ_CALISTIGI_SIRKET() EQ 1 ) {
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_depoya_giren_array").toString(),
                                                    STOK_GIRIS_ARRAY , E9_ARRAY_SIZE);
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_depodan_cikan_array").toString(),
                                                    STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);
                            }
                            else {
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_depoya_giren_array").toString(),
                                                    STOK_GIRIS_ARRAY , E9_ARRAY_SIZE);
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_depodan_cikan_array").toString(),
                                                    STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);
                            }

                            for ( int i = new_gun_no; i < old_gun_no; i++ ) {
                                double depodaki_urun_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY [i];
                                if ( temel_birim_miktari > depodaki_urun_miktari ) {
                                    ADAK_WARNING( QObject::tr( "Fiş tarihi güncellenirse "
                                                                                 + urun_kodu.toLatin1() + " - " + urun_adi.toLatin1()
                                                                                 + " ürünü için depodaki miktar eksiye düşüyor." +
                                                                                 "\nGüncelleme işlemi gerçekleştirilemez."), NULL, p_parent );
                                    return false;
                                }
                            }
                            break;
                        }

                        case ENUM_PARTI_TAKIBI :

                            depo_query.PREPARE_SELECT("stk_urunun_partileri",
                                                      "ent_partiye_giren_array , ent_partiden_cikan_array,"
                                                      "toplam_partiye_giren_array , toplam_partiden_cikan_array ",
                                                      "parti_id = :parti_id");

                            depo_query.SET_VALUE(":parti_id" , parti_id);

                            if ( depo_query.SELECT() EQ 0 ) {
                                break;
                            }
                            depo_query.NEXT();

                            if ( E9_KULLANICININ_CALISTIGI_SIRKET() EQ 1 ) {
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_partiye_giren_array").toString(),
                                                    STOK_GIRIS_ARRAY , E9_ARRAY_SIZE);
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_partiden_cikan_array").toString(),
                                                    STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);
                            }
                            else {
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_partiye_giren_array").toString(),
                                                    STOK_GIRIS_ARRAY , E9_ARRAY_SIZE);
                                UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_partiden_cikan_array").toString(),
                                                    STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);
                            }

                            for ( int i = new_gun_no; i < old_gun_no; i ++ ) {
                                double partideki_urun_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY[i];
                                if ( temel_birim_miktari > partideki_urun_miktari ) {
                                    ADAK_WARNING( QObject::tr( "Fiş tarihi güncellenirse "
                                                                                 + urun_kodu.toLatin1() + " - " + urun_adi.toLatin1()
                                                                                 + " ürünü için partideki miktar eksiye düşüyor." +
                                                                                 "\nGüncelleme işlemi gerçekleştirilemez."), NULL, p_parent );
                                    return false;
                                }
                            }
                            break;
                    }
                }
                break;
        }
    }
    return true;
}

/**************************************************************************************
                   STK_IRSALIYE_SILINMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU
***************************************************************************************/

bool STK_IRSALIYE_SILINMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( int p_irsaliye_id, QWidget * p_parent )
{
    SQL_QUERY sql_query      ( DB );
    SQL_QUERY depo_query     ( DB );

    double STOK_GIRIS_ARRAY [ E9_ARRAY_SIZE ];
    double STOK_CIKIS_ARRAY [ E9_ARRAY_SIZE ];

    sql_query.PREPARE_SELECT("stk_fisler","fis_tarihi, depo_id, alan_depo_id, fis_turu, iade_irsaliyesi_mi ",
                             "fis_id = :fis_id");
    sql_query.SET_VALUE(":fis_id", p_irsaliye_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return false;
    }
    sql_query.NEXT();

    int fis_turu = sql_query.VALUE(3).toInt();
    int iade_irsaliyesi_mi = sql_query.VALUE(4).toInt();
    int depo_id  = 0;

    switch ( fis_turu ) {

        case ENUM_STK_DEPO_TRANSFER_CIKIS :
        default                      :
            depo_id = sql_query.VALUE(1).toInt();
            break;

        case ENUM_STK_DEPO_TRANSFER_GIRIS :
            depo_id = sql_query.VALUE(2).toInt();
            break;
    }

    QDate fis_tarihi = QDate::fromString ( sql_query.VALUE(0).toString(), "yyyy.MM.dd" );
    int   gun_no     = MALI_YIL_ARRAY_INDIS ( fis_tarihi );

    if ( fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
        gun_no = 0;
    }


    sql_query.PREPARE_SELECT("stk_fis_satirlari, stk_urunler",
                             "satirdaki_urun_miktari, stk_fis_satirlari.urun_id, "
                             "urun_kodu, stk_urunler.urun_adi, stok_takip_sekli, parti_id, "
                             "temel_birim_katsayisi, fis_satiri_id ",
                             "stk_fis_satirlari.urun_id = stk_urunler.urun_id "
                             "AND stk_fis_satirlari.urun_id > :urun_id "
                             "AND fis_id = :fis_id ");

    sql_query.SET_VALUE(":urun_id", 0 );
    sql_query.SET_VALUE(":fis_id" , p_irsaliye_id );
    if ( sql_query.SELECT( "stk_urunler.urun_id" ) EQ 0 ) {
        return true;
    }

    int irs_satir_sayisi =  sql_query.NUM_OF_ROWS();
    int irs_satir_index  = -1;
    int last_urun_id     =  0;
    int last_parti_id    =  0;

    IRS_SATIRI_BILGILERI_STRUCT * IRS_SATIR_BILGILERI = new IRS_SATIRI_BILGILERI_STRUCT [ irs_satir_sayisi ];

    while ( sql_query.NEXT() EQ true ) {


        int         urun_id                 =   sql_query.VALUE(1).toInt();
        QString     urun_kodu               =   sql_query.VALUE(2).toString();
        QString     urun_adi                =   sql_query.VALUE(3).toString();
        int         stok_takip_sekli        =   sql_query.VALUE(4).toInt();
        int         parti_id                =   sql_query.VALUE(5).toInt();
        double      silinecek_urun_miktari  =   ROUND ( sql_query.VALUE(0).toDouble() * sql_query.VALUE(6).toDouble() );
        int         fis_satiri_id           =   sql_query.VALUE(7).toInt();



        switch ( stok_takip_sekli ) {

            case ENUM_BASIT_TAKIP :

                if ( urun_id NE last_urun_id ) {

                    irs_satir_index++;

                    IRS_SATIR_BILGILERI[irs_satir_index].urun_id                = urun_id;
                    IRS_SATIR_BILGILERI[irs_satir_index].urun_kodu              = urun_kodu;
                    IRS_SATIR_BILGILERI[irs_satir_index].urun_adi               = urun_adi;
                    IRS_SATIR_BILGILERI[irs_satir_index].stok_takip_sekli       = stok_takip_sekli;
                    IRS_SATIR_BILGILERI[irs_satir_index].parti_id               = parti_id;
                    IRS_SATIR_BILGILERI[irs_satir_index].silinecek_urun_miktari = silinecek_urun_miktari;

                    last_urun_id = urun_id;
                }
                else {
                    IRS_SATIR_BILGILERI[irs_satir_index].silinecek_urun_miktari += silinecek_urun_miktari;
                }

                break;

            case ENUM_SERI_NO_ILE_TAKIP :

                irs_satir_index++;

                IRS_SATIR_BILGILERI[irs_satir_index].urun_id                = urun_id;
                IRS_SATIR_BILGILERI[irs_satir_index].urun_kodu              = urun_kodu;
                IRS_SATIR_BILGILERI[irs_satir_index].urun_adi               = urun_adi;
                IRS_SATIR_BILGILERI[irs_satir_index].stok_takip_sekli       = stok_takip_sekli;
                IRS_SATIR_BILGILERI[irs_satir_index].parti_id               = parti_id;
                IRS_SATIR_BILGILERI[irs_satir_index].silinecek_urun_miktari = silinecek_urun_miktari;
                IRS_SATIR_BILGILERI[irs_satir_index].fis_satiri_id          = fis_satiri_id;

                break;

            case ENUM_PARTI_TAKIBI :

                if ( parti_id NE last_parti_id ) {

                    irs_satir_index++;

                    IRS_SATIR_BILGILERI[irs_satir_index].urun_id                = urun_id;
                    IRS_SATIR_BILGILERI[irs_satir_index].urun_kodu              = urun_kodu;
                    IRS_SATIR_BILGILERI[irs_satir_index].urun_adi               = urun_adi;
                    IRS_SATIR_BILGILERI[irs_satir_index].stok_takip_sekli       = stok_takip_sekli;
                    IRS_SATIR_BILGILERI[irs_satir_index].parti_id               = parti_id;
                    IRS_SATIR_BILGILERI[irs_satir_index].silinecek_urun_miktari = silinecek_urun_miktari;

                    last_parti_id = parti_id;
                }
                else {
                    IRS_SATIR_BILGILERI[irs_satir_index].silinecek_urun_miktari += silinecek_urun_miktari;
                }

                break;
        }
    }

    for ( int i = 0 ; i < irs_satir_index + 1; i++ ) {

        switch ( IRS_SATIR_BILGILERI[i].stok_takip_sekli ) {

            case ENUM_BASIT_TAKIP       :
            default                :
            {
                depo_query.PREPARE_SELECT("stk_depodaki_urunler",
                                          "ent_depoya_giren_array , ent_depodan_cikan_array ,"
                                          "toplam_depoya_giren_array , toplam_depodan_cikan_array ",
                                          "urun_id = :urun_id AND depo_id = :depo_id");
                depo_query.SET_VALUE(":urun_id", IRS_SATIR_BILGILERI[i].urun_id );
                depo_query.SET_VALUE(":depo_id", depo_id );
                if ( depo_query.SELECT() EQ 0 ) {
                    ADAK_WARNING( QObject::tr("Ürün ve depo bilgileri alınamadı"), NULL, p_parent );
                    return false;
                }
                depo_query.NEXT();

                if ( E9_KULLANICININ_CALISTIGI_SIRKET() EQ 1 ) {
                    UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_depoya_giren_array").toString() ,
                                        STOK_GIRIS_ARRAY , E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(depo_query.VALUE("ent_depodan_cikan_array").toString(),
                                        STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);

                }
                else {
                    UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_depoya_giren_array").toString() ,
                                        STOK_GIRIS_ARRAY , E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(depo_query.VALUE("toplam_depodan_cikan_array").toString(),
                                        STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);
                }
                // HIZMETLERDE STOK KONTROLU YOKTUR SONSUZ ALINIP SONZUZ SATILABILIR
                if ( STK_GET_HIZMET_MI( IRS_SATIR_BILGILERI[i].urun_id ) NE 1 ) {
                    if( iade_irsaliyesi_mi NE ENUM_IRS_IADE ) {
                        for ( int j = gun_no; j < E9_ARRAY_SIZE; j++ ) {
                            double depodaki_urun_miktari = STOK_GIRIS_ARRAY[j] - STOK_CIKIS_ARRAY [j];
                            if ( IRS_SATIR_BILGILERI[i].silinecek_urun_miktari > depodaki_urun_miktari ) {
                                ADAK_WARNING( QObject::tr( "Silme işlemi gerçekleşirse "
                                                                             + IRS_SATIR_BILGILERI[i].urun_kodu.toUtf8() + " - " + IRS_SATIR_BILGILERI[i].urun_adi.toUtf8()
                                                                             + " ürünü için depodaki miktar eksiye düşüyor." +
                                                                             "\nSilme işlemi gerçekleştirilemez."), NULL, p_parent );
                                return false;
                            }
                        }
                    }
                }
                break;
            }

            case ENUM_SERI_NO_ILE_TAKIP :

// GOTO KONTROL
//                sql_query.PREPARE_SELECT("stk_satirdaki_seri_nolari",
//                                         "seri_numarasi_id ",
//                                         "irsaliye_satiri_id = :irsaliye_satiri_id");
//                sql_query.SET_VALUE(":irsaliye_satiri_id", IRS_SATIR_BILGILERI[i].fis_satiri_id );

//                if ( sql_query.SELECT() EQ 0 ) {
//                    break;
//                }

//                while ( sql_query.NEXT() EQ true ) {

//                    second_query.PREPARE_SELECT("stk_satirdaki_seri_nolari, stk_urunun_seri_nolari",
//                                                "satir_seri_no_id, seri_numarasi  ",
//                                                "stk_satirdaki_seri_nolari.seri_numarasi_id "
//                                                " = stk_urunun_seri_nolari.seri_numarasi_id "
//                                                "AND irsaliye_satiri_id                        != :irsaliye_satiri_id "
//                                                "AND stk_satirdaki_seri_nolari.seri_numarasi_id = :seri_numarasi_id");

//                    second_query.SET_VALUE(":irsaliye_satiri_id", IRS_SATIR_BILGILERI[i].fis_satiri_id );
//                    second_query.SET_VALUE(":seri_numarasi_id"  , sql_query.VALUE(0).toInt());

//                    if ( second_query.SELECT() > 0 ) {
//                        second_query.NEXT();
//                        ADAK_WARNING( QObject::tr( second_query.VALUE(1).toString().toUtf8() + " seri numarası başka bir irsaliye satırında kullanılmış durumda"
//                                                                                           "\nSilme işlemi gerçekleştirilemez."), NULL, p_parent );
//                        return false;
//                    }
//                }

                break;

            case ENUM_PARTI_TAKIBI :

                sql_query.PREPARE_SELECT("stk_urunun_partileri",
                                         "ent_partiye_giren_array,ent_partiden_cikan_array,"
                                         "toplam_partiye_giren_array,toplam_partiden_cikan_array ",
                                         "parti_id = :parti_id");
                sql_query.SET_VALUE(":parti_id" , IRS_SATIR_BILGILERI[i].parti_id );

                if ( sql_query.SELECT() EQ 0 ) {
                    break;
                }
                sql_query.NEXT();

                if ( E9_KULLANICININ_CALISTIGI_SIRKET() EQ 1 ) {
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("ent_partiye_giren_array").toString(),STOK_GIRIS_ARRAY,
                                        E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("ent_partiden_cikan_array").toString() , STOK_CIKIS_ARRAY,
                                        E9_ARRAY_SIZE);
                }
                else {
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_partiye_giren_array").toString(),STOK_GIRIS_ARRAY,
                                        E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_partiden_cikan_array").toString() , STOK_CIKIS_ARRAY,
                                        E9_ARRAY_SIZE);

                }

                for ( int j = gun_no; j < E9_ARRAY_SIZE; j++ ) {
                    double partideki_urun_miktari = STOK_GIRIS_ARRAY[j] - STOK_CIKIS_ARRAY[j];
                    if ( IRS_SATIR_BILGILERI[i].silinecek_urun_miktari > partideki_urun_miktari ) {
                        ADAK_WARNING( QObject::tr( "Silme işlemi gerçekleşirse "
                                                                     + IRS_SATIR_BILGILERI[i].urun_kodu.toUtf8() + " - " + IRS_SATIR_BILGILERI[i].urun_adi.toUtf8()
                                                                     + " ürünü için partideki miktar eksiye düşüyor." +
                                                                     "\nSilme işlemi gerçekleştirilemez."), NULL, p_parent );
                        return false;
                    }
                }
                break;
            }
    }

    return true;
}

/**************************************************************************************
                   STK_IRSALIYEYE_SATIR_EKLENMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU
***************************************************************************************/

bool STK_IRSALIYEYE_SATIR_EKLENMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( QDate        p_tarih ,
                                                                           int          p_irsaliye_stok_fisi_alis_satis_turu,
                                                                           int          p_stok_takip_sekli,
                                                                           int          p_urun_id,
                                                                           int          p_depo_id,
                                                                           int          p_parti_id,
                                                                           double       p_girilen_stok_miktari,
                                                                           QString      p_birim,
                                                                           QWidget *    p_parent )
{

    int p_gun_no = MALI_YIL_ARRAY_INDIS( p_tarih );

    SQL_QUERY sql_query ( DB );

    switch ( p_irsaliye_stok_fisi_alis_satis_turu ) {

        case ENUM_ALIS_IRSALIYESI_FATURASI :
        default                            :

            break;

        case ENUM_SATIS_IRSALIYESI_FATURASI :

            switch ( p_stok_takip_sekli ) {
                case ENUM_BASIT_TAKIP       :
                default                :

                    double STOK_GIRIS_ARRAY [ E9_ARRAY_SIZE ];
                    double STOK_CIKIS_ARRAY [ E9_ARRAY_SIZE ];

                    sql_query.PREPARE_SELECT("stk_depodaki_urunler",
                                             "toplam_depoya_giren_array, toplam_depodan_cikan_array " ,
                                             "depo_id = :depo_id AND urun_id = :urun_id" );
                    sql_query.SET_VALUE(":depo_id", p_depo_id );
                    sql_query.SET_VALUE(":urun_id", p_urun_id );

                    if ( sql_query.SELECT() EQ 0 ) {
                        break;
                    }

                    sql_query.NEXT();
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_depoya_giren_array").toString() , STOK_GIRIS_ARRAY , E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_depodan_cikan_array").toString() , STOK_CIKIS_ARRAY , E9_ARRAY_SIZE);

                    if ( STK_GET_HIZMET_MI( p_urun_id ) NE 1 ) {
                        for ( int i = p_gun_no; i < E9_ARRAY_SIZE; i++ ) {
                            double depodaki_urun_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY[i];
                            if ( p_girilen_stok_miktari > depodaki_urun_miktari ) {
                                ADAK_WARNING( QObject::tr( "<b>%1</b> Tarihinde Stoktaki Ürün Mikarı : <b>%2</b> %3 <br> "
                                                          "Depodan fazla Ürün Çıkamazsınız!").arg( p_tarih.toString("dd/MM/yyyy") ).arg( QString::number( depodaki_urun_miktari )).arg( p_birim ), NULL, p_parent );
                                return false;
                            }
                        }
                    }
                    break;

                case ENUM_SERI_NO_ILE_TAKIP :
                    break;

                case ENUM_PARTI_TAKIBI :

                    double PARTIYE_GIREN_URUN_ARRAY  [ E9_ARRAY_SIZE ];
                    double PARTIDEN_CIKAN_URUN_ARRAY [ E9_ARRAY_SIZE ];

                    sql_query.PREPARE_SELECT("stk_urunun_partileri",
                                             "toplam_partiye_giren_array , toplam_partiden_cikan_array ",
                                             "parti_id = :parti_id");

                    sql_query.SET_VALUE(":parti_id" , p_parti_id);

                    if ( sql_query.SELECT() EQ 0 ) {
                        break;
                    }

                    sql_query.NEXT();

                    UNPACK_DOUBLE_ARRAY( sql_query.VALUE("toplam_partiye_giren_array").toString() , PARTIYE_GIREN_URUN_ARRAY , E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY( sql_query.VALUE("toplam_partiden_cikan_array").toString() , PARTIDEN_CIKAN_URUN_ARRAY , E9_ARRAY_SIZE);

                    for ( int i = p_gun_no; i < E9_ARRAY_SIZE; i++ ) {
                        double partideki_urun_miktari = PARTIYE_GIREN_URUN_ARRAY[i] - PARTIDEN_CIKAN_URUN_ARRAY[i];
                        if ( p_girilen_stok_miktari > partideki_urun_miktari ) {
                            ADAK_WARNING( QObject::tr("Partide girilen miktarda ürün yok!"), NULL, p_parent );
                            return false;
                        }
                    }
                    break;
            }
            break;
    }

    return true;

}

/**************************************************************************************
                   STK_IRSALIYE_SATIR_GUNCELLEME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU
***************************************************************************************/

bool STK_IRSALIYE_SATIR_GUNCELLEME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU   ( int       p_irsaliye_alis_satis_turu,
                                                                            int       p_new_urun_id,
                                                                            int       p_new_stok_takip_sekli,
                                                                            int       p_depo_id,
                                                                            double    p_new_stok_miktari,
                                                                            int       p_new_seri_no_parti_kodu_id,
                                                                            int       p_old_urun_id,
                                                                            double    p_old_stok_miktari,
                                                                            int       p_old_parti_kodu_id,
                                                                            int       p_gun_no,
                                                                            QWidget * p_parent )
{

    QString old_urun_kodu;
    QString old_urun_adi;
    int     old_stok_takip_sekli = -1;

    SQL_QUERY sql_query ( DB );

    if ( p_old_urun_id > 0 ) {

        sql_query.PREPARE_SELECT("stk_urunler",
                                 "urun_kodu, urun_adi, stok_takip_sekli ",
                                 "urun_id = :urun_id");
        sql_query.SET_VALUE(":urun_id", p_old_urun_id );

        if ( sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
            old_urun_kodu        = sql_query.VALUE(0).toString();
            old_urun_adi         = sql_query.VALUE(1).toString();
            old_stok_takip_sekli = sql_query.VALUE(2).toInt();
        }
    }

    QString new_urun_kodu;
    QString new_urun_adi;

    if ( p_new_urun_id > 0 ) {

        sql_query.PREPARE_SELECT("stk_urunler","urun_kodu, urun_adi ",
                                 "urun_id = :urun_id");
        sql_query.SET_VALUE(":urun_id", p_new_urun_id );

        if ( sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
            new_urun_kodu          = sql_query.VALUE(0).toString();
            new_urun_adi           = sql_query.VALUE(1).toString();
        }
    }

    double STOK_GIRIS_ARRAY [ E9_ARRAY_SIZE ];
    double STOK_CIKIS_ARRAY [ E9_ARRAY_SIZE ];

    switch ( p_irsaliye_alis_satis_turu ) {

        case ENUM_ALIS_IRSALIYESI_FATURASI :
        default                         :


            switch ( old_stok_takip_sekli ) {

                case ENUM_BASIT_TAKIP       :
                default                :
                {
                    if ( p_old_urun_id < 1 ) {
                        break;
                    }

                    sql_query.PREPARE_SELECT("stk_depodaki_urunler",
                                             "toplam_depoya_giren_array ,toplam_depodan_cikan_array ",
                                             "depo_id = :depo_id AND urun_id = :urun_id");
                    sql_query.SET_VALUE(":depo_id", p_depo_id );
                    sql_query.SET_VALUE(":urun_id", p_old_urun_id );
                    if ( sql_query.SELECT() EQ 0 ) {
                        ADAK_WARNING( QObject::tr("Eski ürün bilgileri alınamadı."), NULL, p_parent );
                        return false;
                    }
                    sql_query.NEXT();

                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_depoya_giren_array").toString(),STOK_GIRIS_ARRAY,E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_depodan_cikan_array").toString() , STOK_CIKIS_ARRAY,E9_ARRAY_SIZE);

                    for ( int i = p_gun_no; i < E9_ARRAY_SIZE; i++ ) {
                        double depodaki_stok_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY[i];
                        if ( p_old_urun_id EQ p_new_urun_id ) {
                            p_old_stok_miktari -= p_new_stok_miktari;
                        }
                        if ( p_old_stok_miktari > depodaki_stok_miktari ) {
                            ADAK_WARNING(  QObject::tr( "Güncelleme işlemi gerçekleşirse "
                                                                         + old_urun_kodu.toLatin1() + " - " + old_urun_adi.toLatin1()
                                                                         + " ürünü için depodaki miktar eksiye düşüyor." +
                                                                         "\nGüncelleme işlemi gerçekleştirilemez."), NULL, p_parent );
                            return false;
                        }

                    }
                    break;
                }

                case ENUM_SERI_NO_ILE_TAKIP :
                    break;

                case ENUM_PARTI_TAKIBI :

                    if ( p_old_urun_id < 1 ) {
                        break;
                    }
                    sql_query.PREPARE_SELECT("stk_urunun_partileri",
                                             "toplam_partiye_giren_array , toplam_partiden_cikan_array ",
                                             "parti_id = :parti_id");
                    sql_query.SET_VALUE(":parti_id" , p_old_parti_kodu_id);

                    if ( sql_query.SELECT() EQ 0 ) {
                        break;
                    }
                    sql_query.NEXT();

                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_partiye_giren_array").toString(),STOK_GIRIS_ARRAY,E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_partiden_cikan_array").toString() , STOK_CIKIS_ARRAY,E9_ARRAY_SIZE);

                    for ( int i = p_gun_no; i < E9_ARRAY_SIZE; i++ ) {
                        double partideki_stok_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY[i];
                        if ( p_old_urun_id EQ p_new_urun_id ) {
                            p_old_stok_miktari -= p_new_stok_miktari;
                        }
                        if ( p_old_stok_miktari > partideki_stok_miktari ) {
                            ADAK_WARNING( QObject::tr( "Güncelleme işlemi gerçekleşirse "
                                                                         + old_urun_kodu.toLatin1() + " - " + old_urun_adi.toLatin1()
                                                                         + " ürünü için partideki miktar eksiye düşüyor." +
                                                                         "\nGüncelleme işlemi gerçekleştirilemez."), NULL, p_parent );
                            return false;
                        }
                    }
                    break;
            }

            break;

        case ENUM_SATIS_IRSALIYESI_FATURASI :

            switch ( p_new_stok_takip_sekli ) {
                case ENUM_BASIT_TAKIP       :
                case ENUM_SERI_NO_ILE_TAKIP :
                default                :
                {
                    if ( p_new_urun_id < 1 ) {
                        break;
                    }

                    sql_query.PREPARE_SELECT("stk_depodaki_urunler",
                                             "toplam_depoya_giren_array , toplam_depodan_cikan_array ",
                                             "depo_id = :depo_id AND urun_id = :urun_id");
                    sql_query.SET_VALUE(":depo_id", p_depo_id   );
                    sql_query.SET_VALUE(":urun_id", p_new_urun_id );
                    if ( sql_query.SELECT() EQ 0 ) {
                        ADAK_WARNING( QObject::tr("Yeni ürün bilgileri alınamadı."), NULL, p_parent );
                        return false;
                    }
                    sql_query.NEXT();

                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_depoya_giren_array").toString() , STOK_GIRIS_ARRAY, E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_depodan_cikan_array").toString() , STOK_CIKIS_ARRAY, E9_ARRAY_SIZE);

                    for ( int i = p_gun_no; i < E9_ARRAY_SIZE; i++ ) {
                        double depodaki_stok_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY[i];
                        if ( p_old_urun_id EQ p_new_urun_id ) {
                            p_new_stok_miktari -= p_old_stok_miktari;
                        }
                        if ( p_new_stok_miktari > depodaki_stok_miktari ) {
                            ADAK_WARNING( QObject::tr( "Güncelleme işlemi gerçekleşirse "
                                                                         + new_urun_kodu.toUtf8() + " - " + new_urun_adi.toLatin1()
                                                                         + " ürünü için depodaki miktar eksiye düşüyor." +
                                                                         "\nGüncelleme işlemi gerçekleştirilemez."), NULL, p_parent );
                            return false;
                        }
                    }
                    break;
                }

                case ENUM_PARTI_TAKIBI :

                    if ( p_new_urun_id < 1 ) {
                        break;
                    }

                    sql_query.PREPARE_SELECT("stk_urunun_partileri",
                                             "toplam_partiye_giren_array , toplam_partiden_cikan_array ",
                                             "parti_id = :parti_id");

                    sql_query.SET_VALUE(":parti_id" , p_new_seri_no_parti_kodu_id);

                    if ( sql_query.SELECT() EQ 0 ) {
                        break;
                    }

                    sql_query.NEXT();

                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_partiye_giren_array").toString() , STOK_GIRIS_ARRAY , E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_partiden_cikan_array").toString(), STOK_CIKIS_ARRAY, E9_ARRAY_SIZE);

                    for ( int i = p_gun_no; i < E9_ARRAY_SIZE; i++ ) {
                        double partideki_stok_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY[i];
                        if ( p_old_urun_id EQ p_new_urun_id ) {
                            p_new_stok_miktari -= p_old_stok_miktari;
                        }
                        if ( p_new_stok_miktari > partideki_stok_miktari ) {
                            ADAK_WARNING( QObject::tr( "Güncelleme işlemi gerçekleşirse "
                                                                         + new_urun_kodu.toUtf8() + " - " + new_urun_adi.toUtf8()
                                                                         + " ürünü için partideki miktar eksiye düşüyor." +
                                                                         "\nGüncelleme işlemi gerçekleştirilemez."), NULL, p_parent );
                            return false;
                        }
                    }
                    break;
            }
            break;
    }
    return true;
}

/**************************************************************************************
                   STK_IRSALIYE_SATIR_SILME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU
***************************************************************************************/

bool STK_IRSALIYE_SATIR_SILME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( int p_irsaliye_satiri_id, int p_irsaliye_alis_satis_turu, QWidget * p_parent )
{

    SQL_QUERY   f_query         ( DB );

    double STOK_GIRIS_ARRAY [ E9_ARRAY_SIZE ];
    double STOK_CIKIS_ARRAY [ E9_ARRAY_SIZE ];


    switch ( p_irsaliye_alis_satis_turu ) {

        case ENUM_ALIS_IRSALIYESI_FATURASI :
        {

            f_query.PREPARE_SELECT ( "stk_fisler, stk_fis_satirlari",
                                     "stk_fisler.fis_tarihi, depo_id, satirdaki_urun_miktari, "
                                     "urun_id, parti_id, fis_turu ",
                                     "stk_fisler.fis_id =  stk_fis_satirlari.fis_id "
                                     "AND fis_satiri_id = :fis_satiri_id" );

            f_query.SET_VALUE(":fis_satiri_id", p_irsaliye_satiri_id );

            if ( f_query.SELECT() EQ 0 ) {
                ADAK_WARNING( QObject::tr("Silinecek satır bilgileri alınamadı."), NULL, p_parent );
                return false;
            }
            f_query.NEXT();

            QDate   fis_tarihi              = QDate::fromString ( f_query.VALUE( 0 ).toString() , "yyyy.MM.dd" );
            int     depo_id                 = f_query.VALUE(1).toInt();
            int     gun_no                  = MALI_YIL_ARRAY_INDIS ( fis_tarihi );
            double  silinecek_urun_miktari  = f_query.VALUE(2).toDouble();
            int     urun_id                 = f_query.VALUE(3).toInt();
            int     parti_id                = f_query.VALUE(4).toInt();
            int     fis_turu                = f_query.VALUE(5).toInt();

            if ( fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
                gun_no = 0;
            }

            if ( urun_id < 1 ) {
                return true;
            }

            QString urun_kodu;
            QString urun_adi;
            int     stok_takip_sekli = -1;

            f_query.PREPARE_SELECT ( "stk_urunler",
                                     "urun_kodu, urun_adi, stok_takip_sekli ",
                                     "urun_id = :urun_id");

            f_query.SET_VALUE      ( ":urun_id", urun_id );

            if ( f_query.SELECT() > 0 ) {
                f_query.NEXT();
                urun_kodu        = f_query.VALUE(0).toString();
                urun_adi         = f_query.VALUE(1).toString();
                stok_takip_sekli = f_query.VALUE(2).toInt();
            }

            switch ( stok_takip_sekli ) {

                case ENUM_BASIT_TAKIP       :
                default                :
                {

                    f_query.PREPARE_SELECT ( "stk_depodaki_urunler", "toplam_depoya_giren_array, toplam_depodan_cikan_array ",
                                             "urun_id = :urun_id AND depo_id = :depo_id");

                    f_query.SET_VALUE ( ":urun_id", urun_id );
                    f_query.SET_VALUE ( ":depo_id", depo_id );

                    if ( f_query.SELECT() EQ 0 ) {
                        ADAK_WARNING( QObject::tr("Ürün ve depo bilgileri alınamadı"), NULL, p_parent );
                        return false;
                    }

                    f_query.NEXT();

                    UNPACK_DOUBLE_ARRAY(f_query.VALUE("toplam_depoya_giren_array").toString(),STOK_GIRIS_ARRAY,E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(f_query.VALUE("toplam_depodan_cikan_array").toString(),STOK_CIKIS_ARRAY,E9_ARRAY_SIZE);

                    for ( int i = gun_no; i < E9_ARRAY_SIZE; i++ ) {
                        double depodaki_urun_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY [i];

                        if ( silinecek_urun_miktari > depodaki_urun_miktari ) {
                            ADAK_WARNING( QObject::tr( "Silme işlemi gerçekleşirse "
                                                                         + urun_kodu.toLatin1() + " - " + urun_adi.toLatin1()
                                                                         + " ürünü için depodaki miktar eksiye düşüyor." +
                                                                         "\nSilme işlemi gerçekleştirilemez."), NULL, p_parent );
                            return false;
                        }
                    }
                    break;
                }

                case ENUM_SERI_NO_ILE_TAKIP :
// GOTO KONTROL
//                    f_query.PREPARE_SELECT("stk_satirdaki_seri_nolari","seri_numarasi_id ",
//                                           "irsaliye_satiri_id = :irsaliye_satiri_id");
//                    f_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );

//                    if ( f_query.SELECT() EQ 0 ) {
//                        break;
//                    }

//                    while ( f_query.NEXT() EQ true ) {

//                        s_query.PREPARE_SELECT("stk_satirdaki_seri_nolari","satir_seri_no_id",
//                                               "irsaliye_satiri_id   != :irsaliye_satiri_id "
//                                               "AND seri_numarasi_id  = :seri_numarasi_id");
//                        s_query.SET_VALUE(":irsaliye_satiri_id", p_irsaliye_satiri_id );
//                        s_query.SET_VALUE(":seri_numarasi_id"  , f_query.VALUE(0).toInt());

//                        if ( s_query.SELECT() > 0 ) {
//                            ADAK_WARNING( QObject::tr( "Bu seri numarası başka bir irsaliye satırında kullanılmış durumda"
//                                                                                               "\nSilme işlemi gerçekleştirilemez."), NULL, p_parent );
//                            return false;
//                        }
//                    }

                    break;

                case ENUM_PARTI_TAKIBI :

                    f_query.PREPARE_SELECT("stk_urunun_partileri",
                                           "toplam_partiye_giren_array,toplam_partiden_cikan_array ",
                                           "parti_id = :parti_id");
                    f_query.SET_VALUE(":parti_id" , parti_id);

                    if ( f_query.SELECT() EQ 0 ) {
                        break;
                    }

                    f_query.NEXT();

                    UNPACK_DOUBLE_ARRAY(f_query.VALUE("toplam_partiye_giren_array").toString() , STOK_GIRIS_ARRAY,E9_ARRAY_SIZE);
                    UNPACK_DOUBLE_ARRAY(f_query.VALUE("toplam_partiden_cikan_array").toString() , STOK_CIKIS_ARRAY,E9_ARRAY_SIZE);

                    for ( int i = gun_no; i < E9_ARRAY_SIZE; i++ ) {
                        double partideki_stok_miktari = STOK_GIRIS_ARRAY[i] - STOK_CIKIS_ARRAY[i];
                        if ( silinecek_urun_miktari > partideki_stok_miktari ) {
                            ADAK_WARNING( QObject::tr( "Silme işlemi gerçekleşirse "
                                                                         + urun_kodu.toLatin1() + " - " + urun_adi.toLatin1()
                                                                         + " ürünü için partideki miktar eksiye düşüyor." +
                                                                         "\nGüncelleme işlemi gerçekleştirilemez."), NULL, p_parent );
                            return false;
                        }
                    }

                    break;
            }
            break;
        }

        default :
            break;

    }

    return true;
}

/**************************************************************************************
                   STK_SIRADAKI_FIS_NO_AL
***************************************************************************************/

int STK_SIRADAKI_FIS_NO_AL ( QString p_fis_tarihi )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "stk_fisler","fis_no",
                                     "fis_tarihi = :fis_tarihi" );
    sql_query.SET_VALUE(":fis_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT( "fis_no DESC", 0, 1) EQ 0 ) {
        return 1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt() + 1;
}
/**************************************************************************************
                   STK_GET_HIZMET_TURU_ENUM()
***************************************************************************************/

int STK_GET_HIZMET_TURU_ENUM ( QString hizmet_turu )
{
    if ( hizmet_turu EQ QObject::tr("Alınan Hizmet")) {
        return ENUM_ALINAN_HIZMET;
    }
    else if ( hizmet_turu EQ QObject::tr("Satılan Hizmet")) {
        return ENUM_SATILAN_HIZMET;
    }
    else if ( hizmet_turu EQ QObject::tr( "Alınan / Satılan Hizmet") ) {
        return ENUM_ALINAN_SATILAN_HIZMET;
    }
    return -1;
}

/**************************************************************************************
                   STK_GET_HIZMET_TURU_STRING()
***************************************************************************************/

QString STK_GET_HIZMET_TURU_STRING(int hizmet_turu)
{
    if ( hizmet_turu EQ ENUM_ALINAN_HIZMET ) {
        return QObject::tr("Alınan Hizmet");
    }
    else if ( hizmet_turu EQ ENUM_SATILAN_HIZMET ) {
        return QObject::tr("Satılan Hizmet");
    }
    else if  ( hizmet_turu EQ ENUM_ALINAN_SATILAN_HIZMET ) {
        return QObject::tr( "Alınan / Satılan Hizmet");
    }
    return "";
}

/**************************************************************************************
                   STK_GET_IRS_ID
***************************************************************************************/

int STK_GET_IRS_ID(int stk_irs_satir_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_fis_satirlari" , "fis_id" , "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id" , stk_irs_satir_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   STK_UPDATE_IRSALIYE_FATURALASTIRILDI_MI
***************************************************************************************/

void STK_UPDATE_IRSALIYE_FATURALASTIRILDI_MI( int stk_irs_fis_id, int irsaliye_faturalastirildi_mi )
{
    SQL_QUERY query(DB);

    query.PREPARE_UPDATE ( "stk_fisler","fis_id", "irsaliye_faturalastirildi_mi  ",
                           "fis_id = :fis_id");
    query.SET_VALUE      (":irsaliye_faturalastirildi_mi" , irsaliye_faturalastirildi_mi);
    query.SET_VALUE      (":fis_id"                       , stk_irs_fis_id );
    query.UPDATE         ();
}

/**************************************************************************************
                   STK_GET_PARTI_KODU
***************************************************************************************/

QString STK_GET_PARTI_KODU(int parti_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_urunun_partileri ","parti_kodu",
                         "parti_id = :parti_id");
    query.SET_VALUE(":parti_id" , parti_id);

    if ( query.SELECT() EQ 0 ) {
        return QString();
    }
    query.NEXT();
    return query.VALUE(0).toString();
}

/**************************************************************************************
                   STK_GET_PARTI_ID
***************************************************************************************/

int STK_GET_PARTI_ID(QString parti_kodu)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_urunun_partileri ","parti_id",
                         "parti_kodu = :parti_kodu");
    query.SET_VALUE(":parti_kodu" , parti_kodu);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   STK_GET_FIS_SATIRI_ID_LIST
***************************************************************************************/

QList<int> STK_GET_FIS_SATIRI_ID_LIST(int stk_irs_fis_id)
{
    QList<int> fis_satiri_id_list;

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_fis_satirlari" , "fis_satiri_id" , "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , stk_irs_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return fis_satiri_id_list;
    }
    while (query.NEXT()) {
        fis_satiri_id_list << query.VALUE(0).toInt();
    }
    return fis_satiri_id_list;
}
/**************************************************************************************
                   STK_FATURALANAN_MIKTARI_GUNCELLE
***************************************************************************************/

void STK_FATURALANAN_MIKTARI_GUNCELLE(int stk_irs_satir_id, double faturalanan_miktar)
{
    SQL_QUERY query(DB);

    query.PREPARE_UPDATE ( "stk_fis_satirlari ","fis_satiri_id",
                             "faturalanan_miktar","fis_satiri_id    = :fis_satiri_id" );

    query.SET_VALUE ( ":faturalanan_miktar"    , faturalanan_miktar );
    query.SET_VALUE ( ":fis_satiri_id"         , stk_irs_satir_id );
    query.UPDATE();
}


/*****************************************************************************************************
                 STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID
*******************************************************************************************************/

int STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID(int p_fatura_id)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "fat_irs_baglantisi", "irsaliye_id", "fatura_id = :fatura_id" );
    query.SET_VALUE     ( ":fatura_id" , p_fatura_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        return query.VALUE( 0 ).toInt();
    }

    return 0;
}
/*****************************************************************************************************
                 STK_GET_FAT_IRS_STR_BAGLANTISI_IRSALIYE_ID
*******************************************************************************************************/

int STK_GET_FAT_IRS_STR_BAGLANTISI_IRSALIYE_ID( int p_fatura_satiri_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "fat_irs_satir_baglantisi, stk_fis_satirlari",
                          "stk_fis_satirlari.fis_id",
                          "stk_fis_satirlari.fis_satiri_id = fat_irs_satir_baglantisi.irsaliye_satiri_id "
                          "AND fatura_satiri_id = :fatura_satiri_id" );
    query.SET_VALUE     ( ":fatura_satiri_id" , p_fatura_satiri_id );

    if ( query.SELECT() NE 0 ) {
        return query.VALUE( 0 ).toInt();
    }
    return 0;
}
/*****************************************************************************************************
                 STK_GET_FAT_IRS_STR_BAGLANTISI_IRSALIYE_SATIRI_ID
*******************************************************************************************************/

int STK_GET_FAT_IRS_STR_BAGLANTISI_IRSALIYE_SATIRI_ID ( int p_fatura_satiri_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "fat_irs_satir_baglantisi", "irsaliye_satiri_id", "fatura_satiri_id = :fatura_satiri_id" );
    query.SET_VALUE     ( ":fatura_satiri_id" , p_fatura_satiri_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        return query.VALUE( 0 ).toInt();
    }

    return 0;
}

/*****************************************************************************************************
                 STK_GET_STK_IRS_FIS_AND_FIS_SATIRI_ID_MAP
*******************************************************************************************************/

QMap < int, int >   STK_GET_STK_IRS_FIS_AND_FIS_SATIRI_ID_MAP(int p_cari_hesap_id, int p_irsaliye_turu,
                                                          int p_hatali_irsaliye_mi, QString p_fis_tarihi_baslangic,
                                                          QString p_fis_tarihi_bitis,
                                                          int p_irsaliye_faturalastirildi_mi)
{
    QMap < int, int > irs_fis_map;

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_fisler, stk_fis_satirlari" , "stk_fisler.fis_id, fis_satiri_id", " stk_fisler.fis_id = stk_fis_satirlari.fis_id");

    if ( p_cari_hesap_id NE -1 ) {
        sql_query.AND_EKLE  ( "cari_hesap_id = :cari_hesap_id " );
        sql_query.SET_VALUE ( ":cari_hesap_id" , p_cari_hesap_id );
    }
    if ( p_irsaliye_turu NE -1 ) {
        sql_query.AND_EKLE  ( "fis_turu = :fis_turu" );
        sql_query.SET_VALUE ( ":fis_turu", p_irsaliye_turu );
    }
    if ( p_hatali_irsaliye_mi NE -1 ) {
        sql_query.AND_EKLE  ( "hatali_irsaliye_mi = :hatali_irsaliye_mi" );
        sql_query.SET_VALUE ( ":hatali_irsaliye_mi", p_hatali_irsaliye_mi );
    }
    if ( p_fis_tarihi_baslangic.isEmpty() NE true AND p_fis_tarihi_bitis.isEmpty() NE true ) {
        sql_query.AND_EKLE  ( "fis_tarihi BETWEEN :fis_tarihi_baslangic AND :fis_tarihi_bitis" );
        sql_query.SET_VALUE ( ":fis_tarihi_baslangic", p_fis_tarihi_baslangic );
        sql_query.SET_VALUE ( ":fis_tarihi_bitis", p_fis_tarihi_bitis );
    }

    if ( p_irsaliye_faturalastirildi_mi NE -1 ) {
        sql_query.AND_EKLE(" irsaliye_faturalastirildi_mi = :irsaliye_faturalastirildi_mi " );
        sql_query.SET_VALUE(":irsaliye_faturalastirildi_mi" , p_irsaliye_faturalastirildi_mi);
    }

    if ( sql_query.SELECT("fis_tarihi ASC ,fis_no ASC") NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {
            irs_fis_map.insertMulti( sql_query.VALUE( "fis_id").toInt(), sql_query.VALUE( "fis_satiri_id").toInt() );
        }
    }

    return irs_fis_map;

}

/*****************************************************************************************************
                 STK_GET_IRSALIYE_FIS_ID_LIST
*******************************************************************************************************/

QList<int> STK_GET_IRSALIYE_FIS_ID_LIST(int p_cari_hesap_id, int p_irsaliye_alis_satis_turu,
                                    int p_irsaliye_faturalastirildi_mi,
                                    int p_hatali_irsaliye_mi,
                                    QString p_fis_tarihi_baslangic, QString p_fis_tarihi_bitis)
{
    QList <int > IRS_FIS_ID_LIST;

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_fisler" , "fis_id");

    if ( p_cari_hesap_id NE -1 ) {
        sql_query.AND_EKLE  ( "cari_hesap_id = :cari_hesap_id " );
        sql_query.SET_VALUE ( ":cari_hesap_id" , p_cari_hesap_id );
    }
    if ( p_irsaliye_alis_satis_turu NE -1 ) {
        sql_query.AND_EKLE  ( "irsaliye_alis_satis_turu = :irsaliye_alis_satis_turu" );
        sql_query.SET_VALUE ( ":irsaliye_alis_satis_turu", p_irsaliye_alis_satis_turu             );
    }
    if ( p_irsaliye_faturalastirildi_mi NE -1 ) {
        sql_query.AND_EKLE  ( "irsaliye_faturalastirildi_mi = :irsaliye_faturalastirildi_mi" );
        sql_query.SET_VALUE ( ":irsaliye_faturalastirildi_mi", p_irsaliye_faturalastirildi_mi );
    }
    if ( p_hatali_irsaliye_mi NE -1 ) {
        sql_query.AND_EKLE  ( "hatali_irsaliye_mi = :hatali_irsaliye_mi" );
        sql_query.SET_VALUE ( ":hatali_irsaliye_mi", p_hatali_irsaliye_mi );
    }

    if ( p_fis_tarihi_baslangic.isEmpty() NE true AND p_fis_tarihi_bitis.isEmpty() NE true ) {
        sql_query.AND_EKLE  ( "fis_tarihi BETWEEN :fis_tarihi_baslangic AND :fis_tarihi_bitis" );
        sql_query.SET_VALUE ( ":fis_tarihi_baslangic", p_fis_tarihi_baslangic );
        sql_query.SET_VALUE ( ":fis_tarihi_bitis"    , p_fis_tarihi_bitis );
    }

    if ( sql_query.SELECT("fis_tarihi ASC ,fis_no ASC") NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {
            IRS_FIS_ID_LIST << sql_query.VALUE( "fis_id").toInt();
        }
    }

    return IRS_FIS_ID_LIST;
}

/*****************************************************************************************************
                 STK_GET_URUN_KODU_ADI
*******************************************************************************************************/

int STK_GET_URUN_KODU_ADI(int p_urun_id, QString *p_urun_kodu, QString *p_urun_adi)
{

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_urunler" , "urun_kodu,urun_adi" , "urun_id = :urun_id");
    query.SET_VALUE(":urun_id" , p_urun_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    if ( p_urun_kodu NE NULL ) {
        *p_urun_kodu = query.VALUE(0).toString();
    }
    if ( p_urun_adi NE NULL ) {
        *p_urun_adi = query.VALUE(1).toString();
    }
    return 1;
}

/*****************************************************************************************************
                 STK_GET_URUN_ID_ADI
*******************************************************************************************************/

int STK_GET_URUN_ID_ADI(QString p_urun_kodu, int *p_urun_id, QString *p_urun_adi)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_urunler" , "urun_id,urun_adi" , "urun_kodu = :urun_kodu");
    query.SET_VALUE(":urun_kodu" , p_urun_kodu );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    if ( p_urun_id NE NULL ) {
        *p_urun_id  = query.VALUE(0).toInt();
    }
    if ( p_urun_adi NE NULL ) {
        *p_urun_adi = query.VALUE(1).toString();
    }
    return 1;
}

/*****************************************************************************************************
                 STK_URUNLER_TABLOSUNU_OKU
*******************************************************************************************************/

int STK_URUNLER_TABLOSUNU_OKU(int urun_id, STK_URUNLER_STRUCT *P_URUN ,
                              bool arrayler_okunacak , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("stk_urunler" , "urun_kodu , urun_adi ,barkod_numarasi,"
                         "alis_fiyati , satis_fiyati , urun_alis_doviz_id , "
                         "urun_satis_doviz_id , stok_takip_sekli , urun_temel_birimi ,"
                         "min_stok_seviyesi , kritik_stok_seviyesi , aciklama , alis_otv_orani,"
                         "alis_kdv_orani , hizmet_mi , muh_hesap_id , hizmet_turu ,"
                         "toplam_stoga_giren_array,toplam_stoktan_cikan_array,"
                         "urun_birimleri_array , urun_birim_katsayilari_array,"
                         "urun_capraz_birimler_array ,urun_birim_alis_fiyatlar_array,"
                         "urun_birim_sts_fiyatlar_array ,gorunecek_urn_birimleri_array, satis_otv_orani, satis_kdv_orani "
                         , "urun_id = :urun_id");
    query.SET_VALUE(":urun_id" , urun_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    P_URUN->urun_kodu            = query.VALUE("urun_kodu").toString();
    P_URUN->urun_adi             = query.VALUE("urun_adi").toString();
    P_URUN->barkod_numarasi      = query.VALUE("barkod_numarasi").toString();
    P_URUN->alis_fiyati          = query.VALUE("alis_fiyati").toDouble();
    P_URUN->satis_fiyati         = query.VALUE("satis_fiyati").toDouble();
    P_URUN->urun_alis_doviz_id   = query.VALUE("urun_alis_doviz_id").toInt();
    P_URUN->urun_satis_doviz_id  = query.VALUE("urun_satis_doviz_id").toInt();
    P_URUN->stok_takip_sekli     = query.VALUE("stok_takip_sekli").toInt();
    P_URUN->urun_temel_birimi    = query.VALUE("urun_temel_birimi").toString();
    P_URUN->min_stok_seviyesi    = query.VALUE("min_stok_seviyesi").toDouble();
    P_URUN->kritik_stok_seviyesi = query.VALUE("kritik_stok_seviyesi").toDouble();
    P_URUN->aciklama             = query.VALUE("aciklama").toString();
    P_URUN->alis_otv_orani       = query.VALUE("alis_otv_orani").toDouble();
    P_URUN->alis_kdv_orani       = query.VALUE("alis_kdv_orani").toDouble();
    P_URUN->satis_otv_orani      = query.VALUE("satis_otv_orani").toDouble();
    P_URUN->satis_kdv_orani      = query.VALUE("satis_kdv_orani").toDouble();
    P_URUN->hizmet_mi            = query.VALUE("hizmet_mi").toInt();
    P_URUN->muh_hesap_id         = query.VALUE("muh_hesap_id").toInt();
    P_URUN->hizmet_turu          = query.VALUE("hizmet_turu").toInt();

    if ( arrayler_okunacak EQ true ) {

        UNPACK_DOUBLE_ARRAY( query.VALUE("toplam_stoga_giren_array").toString() ,
                             P_URUN->toplam_stoga_giren_array , E9_ARRAY_SIZE);

        UNPACK_DOUBLE_ARRAY( query.VALUE("toplam_stoktan_cikan_array").toString(),
                             P_URUN->toplam_stoktan_cikan_array , E9_ARRAY_SIZE );


        UNPACK_STRING_ARRAY ( query.VALUE("urun_birimleri_array").toString() ,
                             P_URUN->urun_birimleri_array , URUN_BIRIMLERI_SAYISI );

        UNPACK_DOUBLE_ARRAY( query.VALUE("urun_birim_katsayilari_array").toString(),
                             P_URUN->urun_birim_katsayilari_array , URUN_BIRIMLERI_SAYISI);

        UNPACK_INT_ARRAY   ( query.VALUE("urun_capraz_birimler_array").toString() ,
                             P_URUN->urun_capraz_birimler_array , URUN_BIRIMLERI_SAYISI);

        UNPACK_DOUBLE_ARRAY( query.VALUE("urun_birim_alis_fiyatlar_array").toString(),
                             P_URUN->urun_birim_alis_fiyatlar_array , URUN_BIRIMLERI_SAYISI);


        UNPACK_DOUBLE_ARRAY( query.VALUE("urun_birim_sts_fiyatlar_array").toString() ,
                             P_URUN->urun_birim_sts_fiyatlar_array , URUN_BIRIMLERI_SAYISI );

        UNPACK_INT_ARRAY   ( query.VALUE("gorunecek_urn_birimleri_array").toString() ,
                             P_URUN->gorunecek_urn_birimleri_array , URUN_BIRIMLERI_SAYISI );
    }

    return 1;
}


/*****************************************************************************************************
                 STK_GET_SERI_NUMARASI_ID
*******************************************************************************************************/

int STK_GET_SERI_NUMARASI_ID(int irsaliye_satir_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_satirdaki_seri_nolari" , "seri_numarasi_id" ,
                         "irsaliye_satiri_id = :irsaliye_satiri_id");

    query.SET_VALUE(":irsaliye_satiri_id" , irsaliye_satir_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/*****************************************************************************************************
                 STK_GET_SERI_NUMARASI
*******************************************************************************************************/

QString STK_GET_SERI_NUMARASI(int seri_numarasi_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_urunun_seri_nolari" , "seri_numarasi" , "seri_numarasi_id = :seri_numarasi_id");
    query.SET_VALUE(":seri_numarasi_id" , seri_numarasi_id);

    if ( query.SELECT() EQ 0 ) {
        return QString();
    }
    query.NEXT();

    return query.VALUE(0).toString();
}

/*****************************************************************************************************
                 STK_SATIRDAKI_SERI_NO_TABLOSU_IRS_SATIR_ID_GUNCELLE
*******************************************************************************************************/

void STK_SATIRDAKI_SERI_NO_TABLOSU_IRS_SATIR_ID_GUNCELLE(int irsaliye_satiri_id ,int old_irs_satir_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_UPDATE ( "stk_satirdaki_seri_nolari ","satir_seri_no_id",
                           "irsaliye_satiri_id ",
                           "irsaliye_satiri_id = :old_irsaliye_satiri_id");
    query.SET_VALUE ( ":irsaliye_satiri_id"    , irsaliye_satiri_id );
    query.SET_VALUE ( ":old_irsaliye_satiri_id", old_irs_satir_id );
    query.UPDATE();
}


/*****************************************************************************************************
                 STK_GET_URUN_ID
*******************************************************************************************************/

int STK_GET_URUN_ID(QString urun_barkod_kodu, int depo_id, int hizmet_mi, int hizmet_turu)
{
    SQL_QUERY sql_query(DB);

    if ( depo_id > 0 ) {
        sql_query.PREPARE_SELECT("stk_depodaki_urunler, stk_urunler" ,
                                 "stk_depodaki_urunler.urun_id",
                                 "stk_depodaki_urunler.urun_id = stk_urunler.urun_id "
                                 "AND (urun_kodu     = :urun_kodu "
                                 "OR  barkod_numarasi = :barkod_numarasi) "
                                 "AND depo_id = :depo_id");
        sql_query.SET_VALUE(":urun_kodu",       urun_barkod_kodu );
        sql_query.SET_VALUE(":barkod_numarasi", urun_barkod_kodu );
        sql_query.SET_VALUE(":depo_id"        , depo_id);

    }
    else {
        sql_query.PREPARE_SELECT("stk_urunler" ,"urun_id",
                                 "(urun_kodu     = :urun_kodu "
                                 "OR  barkod_numarasi = :barkod_numarasi)");
        sql_query.SET_VALUE(":urun_kodu",       urun_barkod_kodu );
        sql_query.SET_VALUE(":barkod_numarasi", urun_barkod_kodu );
    }

    if ( hizmet_mi > 0 ) {
        sql_query.AND_EKLE(" hizmet_mi = :hizmet_mi");
        sql_query.SET_VALUE(":hizmet_mi" , hizmet_mi);
    }
    if ( hizmet_turu NE ENUM_ALINAN_SATILAN_HIZMET AND hizmet_turu > 0 ) {
        sql_query.AND_EKLE(" hizmet_turu = :hizmet_turu ");
        sql_query.SET_VALUE(":hizmet_turu" , hizmet_turu);
    }

    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/*****************************************************************************************************
                 STK_GET_URUN_ID_LIST
*******************************************************************************************************/

QList <int> STK_GET_URUN_HIZMET_ID_LIST(QString bas_urun_kodu, QString bts_urun_kodu ,int hizmet_turu)
{
    SQL_QUERY query(DB);

    QList <int> URUN_ID_LIST;

    query.PREPARE_SELECT("stk_urunler" , "urun_id" , "");

    if ( hizmet_turu > 0 ) {
        query.AND_EKLE("hizmet_turu = :hizmet_turu");
        query.SET_VALUE(":hizmet_turu" , hizmet_turu);
    }

    if ( bas_urun_kodu.isEmpty() EQ false AND bts_urun_kodu.isEmpty() EQ false ) {

        query.AND_EKLE("urun_kodu BETWEEN :bas_urun_kodu AND :bts_urun_kodu");
        query.SET_VALUE(":bas_urun_kodu" , bas_urun_kodu );
        query.SET_VALUE(":bts_urun_kodu" , bts_urun_kodu );
    }
    else if ( bas_urun_kodu.isEmpty() EQ false ) {
        query.AND_EKLE("urun_kodu = :urun_kodu");
        query.SET_VALUE(":urun_kodu" , bas_urun_kodu);
    }

    if ( query.SELECT() EQ 0 ) {
        return URUN_ID_LIST;
    }

    while ( query.NEXT() ) {

        URUN_ID_LIST << query.VALUE(0).toInt();
    }
    return URUN_ID_LIST;

}

/*****************************************************************************************************
                 STK_GET_URUN_SAYI_AND_SERI_NUMARASI
*******************************************************************************************************/

int STK_GET_URUN_SAYI_AND_SERI_NUMARASI(int p_irsaliye_satir_id, QString *p_urun_seri_numarasi)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT ( "stk_urunun_seri_nolari, stk_satirdaki_seri_nolari",
                           "seri_numarasi ",
                           "stk_urunun_seri_nolari.seri_numarasi_id = stk_satirdaki_seri_nolari.seri_numarasi_id "
                           "AND stk_satirdaki_seri_nolari.irsaliye_satiri_id = :irsaliye_satiri_id ");
    query.SET_VALUE     ( ":irsaliye_satiri_id", p_irsaliye_satir_id );

    if ( query.SELECT( "order_number ASC ") EQ 0 ) {
        if ( p_urun_seri_numarasi NE NULL ) {
            *p_urun_seri_numarasi = "";
        }
        return 0;
    }

    query.NEXT();

    if ( p_urun_seri_numarasi NE NULL ) {
        *p_urun_seri_numarasi = query.VALUE(0).toString();
    }
    return query.NUM_OF_ROWS();
}


/*****************************************************************************************************
                 STK_URUN_TAKIP_SEKLI
*******************************************************************************************************/

int STK_GET_URUN_TAKIP_SEKLI( int p_urun_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("stk_urunler", "stok_takip_sekli", "urun_id = :urun_id");
    query.SET_VALUE     (":urun_id", p_urun_id );

    if ( query.SELECT() EQ 0 ) {
        return -1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();

}

/*****************************************************************************************************
                 STK_GET_BIRIM_HASSASIYETI
*******************************************************************************************************/

int STK_GET_BIRIM_HASSASIYETI()
{
    // [ 1 ]
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "e9_sabit_degerler", "stok_birim_fiyat_hassasiyeti");
    query.SELECT();
    query.NEXT();

    return query.VALUE( 0 ).toInt();
    // [/1]
}

/*****************************************************************************************************
                 STK_GET_BIRIM_HASSASIYETI
*******************************************************************************************************/

int STK_GET_MIKTAR_HASSASIYETI()
{
    // [ 1 ]
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "e9_sabit_degerler", "stok_birim_miktar_hassasiyeti");
    query.SELECT();
    query.NEXT();

    return query.VALUE( 0 ).toInt();
    // [/1]
}

/*****************************************************************************************************
                 STK_GET_URUN_MU_HIZMET_MI
*******************************************************************************************************/

int STK_GET_HIZMET_MI ( QString p_urun_or_hizmet_kodu, int * p_urun_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("stk_urunler", "hizmet_mi, urun_id", "urun_kodu = :urun_kodu");
    query.SET_VALUE     (":urun_kodu", p_urun_or_hizmet_kodu );

    if ( query.SELECT() EQ 0 ) {
        return -1;
    }

    query.NEXT();

    if ( p_urun_id NE NULL ) {
        *p_urun_id = query.VALUE( 1 ).toInt();
    }

    return query.VALUE(0).toInt();
}

/*****************************************************************************************************
                 STK_GET_URUN_MU_HIZMET_MI
*******************************************************************************************************/

int STK_GET_HIZMET_MI ( int  p_urun_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("stk_urunler", "hizmet_mi, urun_id", "urun_id = :urun_id");
    query.SET_VALUE     (":urun_id", p_urun_id );

    if ( query.SELECT() EQ 0 ) {
        return -1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/*****************************************************************************************************
                 STK_GET_SERI_NO_BILGILERI
*******************************************************************************************************/

QString STK_GET_SERI_NO_BILGILERI(int p_irs_satir_id, URUN_DEPO_DURMU p_depoda_durumu, int p_fatura_satiri_id )
{

    QString seri_numaralari = "";
    SQL_QUERY t_query( DB );

    t_query.PREPARE_SELECT("stk_satirdaki_seri_nolari, stk_urunun_seri_nolari",
                           "seri_numarasi, order_number ",
                           "stk_satirdaki_seri_nolari.seri_numarasi_id = stk_urunun_seri_nolari.seri_numarasi_id "
                           "AND irsaliye_satiri_id = :irsaliye_satiri_id " );
    t_query.SET_VALUE(":irsaliye_satiri_id", p_irs_satir_id  );

    if ( p_depoda_durumu EQ ENUM_URUN_DEPODA ) {
        t_query.AND_EKLE( "depoda_mi = :depoda_mi");
        t_query.SET_VALUE(":depoda_mi" , 1  );
    }
    else if ( p_depoda_durumu EQ ENUM_URUN_DEPODA_DEGIL ) {
        t_query.AND_EKLE( "depoda_mi = :depoda_mi");
        t_query.SET_VALUE(":depoda_mi", 0  );
    }
    // -2 sadece faturalanmamislari geri donderir.
    if ( p_fatura_satiri_id EQ -2 ) {
        t_query.AND_EKLE( "fatura_satiri_id = :fatura_satiri_id");
        t_query.SET_VALUE(":fatura_satiri_id", 0  );
    }
    else if ( p_fatura_satiri_id NE -1 ) {
        t_query.AND_EKLE( "fatura_satiri_id = :fatura_satiri_id");
        t_query.SET_VALUE(":fatura_satiri_id" , p_fatura_satiri_id  );
    }

    int seri_no_count = t_query.SELECT("order_number");
    int i = 0;
    if (  seri_no_count NE 0 ) {
        while ( t_query.NEXT() EQ true ) {
            i++;
            seri_numaralari.append( t_query.VALUE(0).toString() );
            if( i NE seri_no_count ) {
                seri_numaralari.append( ",");
            }
        }
    }

    return seri_numaralari;
}

/*****************************************************************************************************
                 STK_SERI_NOLARI_FATURA_SATIRI_ID_GUNCELLE
*******************************************************************************************************/

void STK_SERI_NOLARI_FATURA_SATIRI_ID_GUNCELLE ( QStringList p_seri_no_list, int p_urun_id, int p_fat_satiri_id )
{
    SQL_QUERY query(DB);

    for (int i = 0; i < p_seri_no_list.size(); ++i) {
        query.PREPARE_UPDATE( "stk_urunun_seri_nolari", "seri_numarasi_id", "fatura_satiri_id",
                              "urun_id = :urun_id AND seri_numarasi =:seri_numarasi");
        query.SET_VALUE     ( ":urun_id", p_urun_id );
        query.SET_VALUE     ( ":seri_numarasi", p_seri_no_list.at( i ) );
        query.SET_VALUE     ( ":fatura_satiri_id", p_fat_satiri_id );
        query.UPDATE();
    }
}

/*****************************************************************************************************
                 STK_URUN_KARTI_EKLE
*******************************************************************************************************/

int STK_URUN_KARTI_EKLE( STK_URUNLER_STRUCT *P_URUN_STRUCT )
{
    SQL_QUERY insert_query(DB);

    insert_query.PREPARE_INSERT("stk_urunler" , "urun_id" , "urun_kodu,urun_adi,barkod_numarasi,alis_fiyati,satis_fiyati,"
                                "urun_alis_doviz_id,urun_satis_doviz_id,aciklama,"
                                "alis_kdv_orani,hizmet_mi,muh_hesap_id,urun_temel_birimi,hizmet_turu, satis_kdv_orani, alis_otv_orani, satis_otv_orani ");

    insert_query.SET_VALUE(":urun_kodu"           , P_URUN_STRUCT->urun_kodu                               );
    insert_query.SET_VALUE(":urun_adi"            , P_URUN_STRUCT->urun_adi                                );
    insert_query.SET_VALUE(":barkod_numarasi"     , P_URUN_STRUCT->barkod_numarasi                         );
    insert_query.SET_VALUE(":alis_fiyati"         , P_URUN_STRUCT->alis_fiyati                             );
    insert_query.SET_VALUE(":satis_fiyati"        , P_URUN_STRUCT->satis_fiyati                            );
    insert_query.SET_VALUE(":urun_alis_doviz_id"  , P_URUN_STRUCT->urun_alis_doviz_id                      );
    insert_query.SET_VALUE(":urun_satis_doviz_id" , P_URUN_STRUCT->urun_satis_doviz_id                     );
    insert_query.SET_VALUE(":aciklama"            , P_URUN_STRUCT->aciklama                                );
    insert_query.SET_VALUE(":alis_kdv_orani"      , P_URUN_STRUCT->alis_kdv_orani                          );
    insert_query.SET_VALUE(":satis_kdv_orani"     , P_URUN_STRUCT->satis_kdv_orani                         );
    insert_query.SET_VALUE(":hizmet_mi"           , P_URUN_STRUCT->hizmet_mi                               );
    insert_query.SET_VALUE(":muh_hesap_id"        , P_URUN_STRUCT->muh_hesap_id                            );
    insert_query.SET_VALUE(":urun_temel_birimi"   , P_URUN_STRUCT->urun_temel_birimi                       );
    insert_query.SET_VALUE(":alis_otv_orani"      , P_URUN_STRUCT->alis_otv_orani                          );
    insert_query.SET_VALUE(":satis_otv_orani"     , P_URUN_STRUCT->satis_otv_orani                         );
    insert_query.SET_VALUE(":hizmet_turu"         , STK_GET_HIZMET_TURU_ENUM( QString::number( P_URUN_STRUCT->hizmet_turu ) ) );

    int urun_id = insert_query.INSERT();

    return urun_id;
}
