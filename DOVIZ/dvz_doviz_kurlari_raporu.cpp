#include "dvz_doviz_kurlari_raporu_class.h"
#include "adak_utils.h"
#include "yonetim.h"
#include "e9_console_utils.h"

extern ADAK_SQL *         DB;

/**************************************************************************************
                   OPEN_DVZ_KUR_RAPORU
***************************************************************************************/

QString OPEN_DVZ_KUR_RAPORU ( QString p_date )
{
    DVZ_KUR_RAPORU *  F = new DVZ_KUR_RAPORU( p_date );

    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   DVZ_KUR_RAPORU::DVZ_KUR_RAPORU
***************************************************************************************/

 DVZ_KUR_RAPORU::DVZ_KUR_RAPORU( QString p_date ) : REPORT_KERNEL ("DVZ_KUR_RAPORU")
{
    m_date = p_date;

    SET_HEIGHTS ( 30, 30 );

    m_max_line_count = GET_MAX_LINE_COUNT ( 14 );

    m_tum_satirlar_yazildimi = false;

    KUR_BILGILERINI_TOPLA();
}

/**************************************************************************************
                   UYE_LISTESI_RAPORU::GET_HEADER
***************************************************************************************/

QString DVZ_KUR_RAPORU::GET_HEADER()
{

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList() <<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList() <<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()  << "" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>DÖVİZ KURLARI" << "");

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   UYE_LISTESI_RAPORU::GET_BODY
***************************************************************************************/

QString DVZ_KUR_RAPORU::GET_BODY()
{
    if( m_tum_satirlar_yazildimi EQ true ) {
        return NULL;
    }

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=10%" << "WIDTH=30%"<< "WIDTH=10%" << "WIDTH=10%" << "WIDTH=10%"
                                         << "WIDTH=10%" << "WIDTH=10%" << "WIDTH=10%" );
    CREATE_TABLE         ( QStringList()<< "<b>Döviz kodu" << "<b>Döviz Adı" << "<b>Döviz Alış" << "<b>Döviz Satış"<<"<b>Efektif Alış"
                                        <<"<b>Efektif Satış" <<"<b>Alış Değeri" <<"<b>Satış Değeri", 1, 13);

    SET_TD_TAG_STRING    ( QStringList()<< "" << "" << "ALIGN=RIGHT" << "ALIGN=RIGHT" << "ALIGN=RIGHT" << "ALIGN=RIGHT" << "ALIGN=RIGHT" << "ALIGN=RIGHT" );

    QStringList RAPOR_SATIRI;
    int         last_satir_indisi = 0;

    forever {

        if ( last_satir_indisi EQ M_SATIR_BILGILERI.size() ) {
            m_tum_satirlar_yazildimi = true;
            break;
        }

        RAPOR_SATIRI = M_SATIR_BILGILERI.at( last_satir_indisi ).split("\t");

        ADD_ROW_TO_TABLE (QStringList() << RAPOR_SATIRI.at(0) << RAPOR_SATIRI.at(1) << RAPOR_SATIRI.at(2)
                                        << RAPOR_SATIRI.at(3) << RAPOR_SATIRI.at(4) << RAPOR_SATIRI.at(5)
                                        << RAPOR_SATIRI.at(6) << RAPOR_SATIRI.at(7) );

        last_satir_indisi++;
     }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   UYE_LISTESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString DVZ_KUR_RAPORU::GET_FOOTER()
{
    return NULL;
}

/**************************************************************************************
                   UYE_LISTESI_RAPORU::GET_UYE_LISTESI
***************************************************************************************/
void DVZ_KUR_RAPORU::KUR_BILGILERINI_TOPLA()
{

    SQL_QUERY f_query ( DB );
    SQL_QUERY s_query ( DB );

    QStringList kurlar;

    f_query.PREPARE_SELECT("dvz_dovizler" , "doviz_kodu, doviz_adi, dovizi_kullan, doviz_id" ,
                           "dovizi_kullan = :dovizi_kullan");
    f_query.SET_VALUE(":dovizi_kullan" , 1);

    if ( f_query.SELECT("doviz_adi ASC") EQ 0 ) {
        return;
    }

    s_query.PREPARE_SELECT( "dvz_kurlar","kur_bilgileri", "kur_tarihi = :kur_tarihi" );
    s_query.SET_VALUE(":kur_tarihi" , m_date );
    if ( s_query.SELECT() NE 0 ) {
        kurlar.clear();
        while ( s_query.NEXT() EQ true ) {
            kurlar << s_query.VALUE(0).toString().split("\t");
        }
    }

    QStringList SATIR_DATA;

    while( f_query.NEXT() ) {
        QStringList kur_bilgileri;

        QString alis_fiyati       = "0.0";
        QString satis_fiyati      = "0.0";
        QString efk_alis_fiyati   = "0.0";
        QString efk_satis_fiyati  = "0.0";
        QString kull_alis_fiyati  = "0.0";
        QString kull_satis_fiyati = "0.0";

        for ( int i = 0 ; i < kurlar.size() ; i++ ) {
            kur_bilgileri = kurlar.at(i).split("+");
            if ( kur_bilgileri.at(0) EQ f_query.VALUE(0).toString() ) {
                alis_fiyati       = kur_bilgileri.at(2);
                satis_fiyati      = kur_bilgileri.at(3);
                efk_alis_fiyati   = kur_bilgileri.at(4);
                efk_satis_fiyati  = kur_bilgileri.at(5);
                kull_alis_fiyati  = kur_bilgileri.at(6);
                kull_satis_fiyati = kur_bilgileri.at(7);
                break;
            }
        }

        QString doviz_kodu = f_query.VALUE(0).toString();
        QString doviz_adi  = f_query.VALUE(1).toString();

        SATIR_DATA.clear();

        SATIR_DATA.append( doviz_kodu
                          + "\t" + doviz_adi
                          + "\t" + VIRGUL_EKLE( alis_fiyati             , 4 )
                          + "\t" + VIRGUL_EKLE( satis_fiyati            , 4 )
                          + "\t" + VIRGUL_EKLE( efk_alis_fiyati         , 4 )
                          + "\t" + VIRGUL_EKLE( efk_satis_fiyati        , 4 )
                          + "\t" + VIRGUL_EKLE( kull_alis_fiyati        , 4 )
                          + "\t" + VIRGUL_EKLE( kull_satis_fiyati       , 4 ) );

        M_SATIR_BILGILERI << SATIR_DATA;

    }
}

