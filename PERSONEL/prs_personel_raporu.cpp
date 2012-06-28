#include "prs_personel_raporu_class.h"
#include "prs_personel_raporu_open.h"
#include "prs_con_utils.h"
#include "adak_utils.h"
#include "cari_struct.h"
#include "yonetim.h"
#include "cari_console_utils.h"
#include "e9_console_utils.h"


extern ADAK_SQL          * DB;

/***************************************************************************************
                   SHOW_PERSONEL_RAPORU
***************************************************************************************/


QString SHOW_PERSONEL_RAPORU (int sube_id )
{
    PERSONEL_RAPORU  *  R = new PERSONEL_RAPORU( sube_id );

    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
    PERSONEL_RAPORU::PERSONEL_RAPORU
***************************************************************************************/

PERSONEL_RAPORU::PERSONEL_RAPORU( int sube_id) : REPORT_KERNEL ("PRS_PERSONEL_RAPORU")
{
    SET_HEIGHTS (30,30);

    m_max_line_count           =  GET_MAX_LINE_COUNT (12);
    m_rapor_bitti              =  false;
    m_sube_id                  =  sube_id;

}


/**************************************************************************************
                   PERSONEL_RAPORU::GET_HEADER
***************************************************************************************/

QString PERSONEL_RAPORU::GET_HEADER()
{
    QStringList    header_list;

    if ( m_rapor_bitti EQ true ) {
        return NULL;
    }


    SET_TABLE_TAG_STRING("<TABLE WIDTH=100% style=\"font-size:75%;\" BORDER=0 >");

    SET_TD_TAG_STRING(QStringList()<<"WIDTH = 31% ALIGN=CENTER"<<"WIDTH = 23% ALIGN=CENTER"<<"WIDTH = 23% ALIGN=CENTER"<<"WIDTH = 23% ALIGN=CENTER");


    header_list<<"<b>""<font size=3>" +  E9_GET_FIRMA_ISMI() << "" << "" << "<b>""<font size=3>"+ GET_REPORT_TOP_RIGHT_HEADER() + "<br><br>""</b>";

    CREATE_TABLE( header_list, 0, 14 );



    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   PERSONEL_RAPORU::GET_BODY
***************************************************************************************/

QString PERSONEL_RAPORU::GET_BODY()
{
    SQL_QUERY     sql_query     (DB);
    QStringList   column_header_list;
    QStringList   row_datas;
    int           personel_id;

    if ( m_rapor_bitti EQ true ) {
        return NULL;
    }

    column_header_list <<"Adı Soyadı"<<"Sicil No"<<"T.C. Kimlik No"<< "Cep Telefonu";

    SET_TABLE_TAG_STRING( "<table style=\"width:100%;\" border=\"2\">");
    CREATE_TABLE( column_header_list, 0, 15 );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id , cari_hesap_id , sube_id" , "");

    if (m_sube_id > 0 ) {
        sql_query.AND_EKLE  ( "sube_id = :sube_id"   );
        sql_query.SET_VALUE ( ":sube_id" , m_sube_id );

    }

    if (sql_query.SELECT() EQ 0) {
            return NULL;
        }

    while ( sql_query.NEXT() EQ true ) {

        row_datas.clear();

        personel_id = sql_query.VALUE(0).toInt();
        int cari_hesap_id = sql_query.VALUE( "cari_hesap_id").toInt();

        CARI_HESAP_STRUCT hesap_bilgileri;
        CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

        CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri  );

        row_datas << hesap_bilgileri.cari_hesap_ismi
                  << PRS_GET_PERSONEL_SICIL_NO ( personel_id ) << hesap_bilgileri.kimlik_no
                  << PRS_GET_PERSONEL_CEP_TELEFONU ( personel_id );
        ADD_ROW_TO_TABLE(row_datas);

        m_eklenen_kayit_sayisi += m_max_line_count;
    }

    m_rapor_bitti = true;

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   PERSONEL_RAPORU::GET_FOOTER
***************************************************************************************/

QString PERSONEL_RAPORU::GET_FOOTER()
{
    QString    string_list = " ";

    return string_list;
}
