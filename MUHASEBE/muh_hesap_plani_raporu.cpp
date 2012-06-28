#include "muh_hesap_plani_raporu_class.h"
#include "muh_hesap_plani_raporu_open.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "yonetim.h"
#include "e9_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   GET_HESAP_PLANI_RAPORU
***************************************************************************************/

QString GET_HESAP_PLANI_RAPORU (MUH_RAPOR_VARS * rapor_vars)
{
    HESAP_PLANI_RAPORU * R = new HESAP_PLANI_RAPORU(rapor_vars);
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   HESAP_PLANI_RAPORU::HESAP_PLANI_RAPORU
***************************************************************************************/

HESAP_PLANI_RAPORU::HESAP_PLANI_RAPORU(MUH_RAPOR_VARS * rapor_vars) : REPORT_KERNEL ("MUH_HESAP_PLANI_RAPORU")
{
    M_MUH_RV = rapor_vars;

    m_sayfa_no = 0;

    SET_PAPER_SIZE( QPrinter::A4 );

    // yatay cikti almak icin
    //SET_ORIENTATION( QPrinter::Landscape );

    SET_HEIGHTS ( 30, 0 );

    GET_YAZDIRILACAK_MUHASEBE_HESAPLARI();

    m_max_yazdirilabilir_satir_sayisi = GET_MAX_LINE_COUNT (11);

    m_toplam_hesap_sayisi        = 0;
    m_hesap_indisi               = 0;
    m_yazdirilan_kayit_sayisi    = 0;

}

/**************************************************************************************
                   HESAP_PLANI_RAPORU::GET_HEADER
***************************************************************************************/

QString HESAP_PLANI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:75%;\" BORDER=0>" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=25%"<<"WIDTH=50% ALIGN=CENTER"<<"WIDTH=25% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList() << "" << "" << "",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI() << "<b>HESAP PLANI</b>" << GET_REPORT_TOP_RIGHT_HEADER() );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   HESAP_PLANI_RAPORU::GET_BODY
***************************************************************************************/

QString HESAP_PLANI_RAPORU::GET_BODY()
{
    if ( m_hesap_indisi >= M_MUHASEBE_HESAPLARI.size() ) {
        return NULL;
    }

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =0 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=35%" << "WIDTH=65%" );

    CREATE_TABLE         ( QStringList()<< "<b><u>Hesap Kodu" << "<b><u>Hesap İsmi", 1, 13 );

    m_yazdirilan_kayit_sayisi = 0;

    QStringList M_YAZDIRALACAK_HESAP;
    for ( int i = m_hesap_indisi ; i < M_MUHASEBE_HESAPLARI.size() ;i++ ) {
        M_YAZDIRALACAK_HESAP = M_MUHASEBE_HESAPLARI.at(i).split ( "\t" );

        QString hesap_kodu   = M_YAZDIRALACAK_HESAP.at(0);
        QString hesap_ismi   = M_YAZDIRALACAK_HESAP.at(1);
        int hesap_seviyesi   = QVariant ( M_YAZDIRALACAK_HESAP.at(2) ).toInt();

        m_toplam_hesap_sayisi++;

        if ( hesap_seviyesi > 1 ) {
            for ( int i = 1 ; i < hesap_seviyesi ; i++ ) {
                hesap_kodu.prepend ( "&nbsp;&nbsp;" );
                hesap_kodu.prepend ( "&nbsp;&nbsp;" );
            }
        }
        ADD_ROW_TO_TABLE ( QStringList() << hesap_kodu << hesap_ismi );
        m_yazdirilan_kayit_sayisi++;

        m_hesap_indisi++;
        if ( m_yazdirilan_kayit_sayisi EQ m_max_yazdirilabilir_satir_sayisi ) {
            break;
        }
    }
    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   HESAP_PLANI_RAPORU::GET_FOOTER
***************************************************************************************/

QString HESAP_PLANI_RAPORU::GET_FOOTER()
{

    m_sayfa_no++;
    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:75%;font-weight:bold;\" BORDER = 0 >");

    SET_TD_TAG_STRING ( QStringList() << "style = \"width:10%;\"" << "style = \"width:10%;\" ALIGN = RIGHT"
                                      << "style = \"width:30%;\" ALIGN = RIGHT" << "style = \"width:10%;\" ALIGN=CENTER"
                                      << "style = \"width:40;%\" ALIGN=RIGHT" );

    CREATE_TABLE( QStringList() << "" << "" << "" << "" << "",0,0);

    QStringList footer_line;

    footer_line << "Hesap Sayısı : " << QVariant ( m_toplam_hesap_sayisi ).toString()
                << ""
                << QString::number(m_sayfa_no)
                << "";

    ADD_ROW_TO_TABLE ( footer_line );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   HESAP_PLANI_RAPORU::GET_YAZDIRILACAK_MUHASEBE_HESAPLARI();
***************************************************************************************/

void HESAP_PLANI_RAPORU::GET_YAZDIRILACAK_MUHASEBE_HESAPLARI()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi,hesap_seviyesi" ,
                             "tam_hesap_kodu >= :bas_hesap_kodu  AND tam_hesap_kodu <= :bts_hesap_kodu");

    sql_query.SET_VALUE      ( ":bas_hesap_kodu" , M_MUH_RV->bas_hesap_kodu );
    sql_query.SET_VALUE      ( ":bts_hesap_kodu" , M_MUH_RV->bts_hesap_kodu );

    if ( sql_query.SELECT() > 0 ) {

        M_MUHASEBE_HESAPLARI.clear();
        while ( sql_query.NEXT() ) {
            M_MUHASEBE_HESAPLARI.append ( sql_query.VALUE(0).toString() + "\t" + sql_query.VALUE(1).toString() + "\t"
                                        + sql_query.VALUE(2).toString() );
        }
        M_MUHASEBE_HESAPLARI.sort();
    }
}
