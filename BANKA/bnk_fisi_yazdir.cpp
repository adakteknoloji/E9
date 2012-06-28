#include <QDate>
#include "adak_utils.h"
#include "bnk_fisi_yazdir_class.h"
#include "bnk_fisi_yazdir_open.h"
#include "dvz_enum.h"
#include "yonetim.h"
#include "dvz_console_utils.h"
#include "e9_console_utils.h"

extern ADAK_SQL * DB;

/*****************************************************************/
/*                  F_BNK_FISI_YAZDIR()                          */
/*****************************************************************/

QString GET_BNK_FISI_YAZDIR (int fis_id,int fis_turu)
{
    BNK_FISI_YAZDIR  *  R = new BNK_FISI_YAZDIR(fis_id,fis_turu);
    return R->CREATE_HTML_PAGES();
}

/*****************************************************************/
/*     BNK_FISI_YAZDIR::BNK_FISI_YAZDIR                          */
/*****************************************************************/

BNK_FISI_YAZDIR::BNK_FISI_YAZDIR(int fis_id,int fis_turu) : REPORT_KERNEL ("BNK_FISI_YAZDIR")
{
    SET_HEIGHTS (40,30);

    m_fis_id                     =  fis_id;
    m_fis_turu                   =  fis_turu;

    SQL_QUERY sql_query (DB);
    sql_query.PREPARE_SELECT("bnk_makrolar","makro_id,makro_adi");
    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    while (sql_query.NEXT() EQ true) {
        m_islem_ids << sql_query.VALUE(0).toInt();
        m_islem_str << sql_query.VALUE(1).toString();
    }

    FILL_HEADER_STRUCT();
    GET_BANKA_FIS_SATIRLARI();

    m_max_line_count             =  GET_MAX_LINE_COUNT (12);

    m_eklenen_kayit_sayisi       = 0;
    m_last_satir_indisi          = 0;
    m_toplam_hesaba_giren        = 0.0;
    m_toplam_hesaptan_cikan      = 0.0;
    m_toplam_pos_blokaj_tutari   = 0.0;
    m_toplam_komisyon_gideri     = 0.0;

}

/*****************************************************************/
/*              BNK_FISI_YAZDIR::GET_HEADER                      */
/*****************************************************************/

QString BNK_FISI_YAZDIR::GET_HEADER()
{

    SET_TABLE_TAG_STRING("<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >");

    SET_TD_TAG_STRING(QStringList()<<"style = \"width:40%; font-size:88%;\""<<"style = \"width:20%;font-size:120%;\" ALIGN=CENTER"
                      << "style = \"width:40%;style = \"font-size:88%;\" ALIGN = RIGHT");

    CREATE_TABLE (QStringList()<<""<<""<<"",0,0);


    QString rapor_basligi = "";


    ADD_ROW_TO_TABLE(QStringList() << E9_GET_FIRMA_ISMI() << "<b>" + rapor_basligi+ "</b>"<< GET_REPORT_TOP_RIGHT_HEADER() );
    ADD_ROW_TO_TABLE(QStringList() << "<b> Banka İsmi" + ADD_HTML_SPACE(" ") + ":</b>" + ADD_HTML_SPACE("   ") + M_HEADER_INFO.banka_ismi
                                   + " / "  + M_HEADER_INFO.sube_kodu + ADD_HTML_SPACE("  ") + M_HEADER_INFO.sube_ismi <<""<< "");
    ADD_ROW_TO_TABLE(QStringList() << "<b>Hesap No " + ADD_HTML_SPACE("   ") + ":</b>" + ADD_HTML_SPACE("   ") + M_HEADER_INFO.hesap_numarasi + ADD_HTML_SPACE("  ") + "<b>" + M_HEADER_INFO.doviz_kodu << ""<<"");

    ADD_ROW_TO_TABLE(QStringList() << "<b>Fiş Tarihi" + ADD_HTML_SPACE("    ") + ":</b>" + ADD_HTML_SPACE("   ") + M_HEADER_INFO.fis_tarihi <<   "<p style = \"font-size:80%;\"><b>Fiş No : </b> " + M_HEADER_INFO.fis_no + ADD_HTML_SPACE("  ")+"</p>"  <<"");
    ADD_ROW_TO_TABLE(QStringList() << "<b>Açıklama   " + ADD_HTML_SPACE("   ") + ":</b>" + ADD_HTML_SPACE("   ") + M_HEADER_INFO.aciklama << "" << "");

    return GET_TABLE_HTML_STRING();
}

/*****************************************************************/
/*              BNK_FISI_YAZDIR::GET_BODY                        */
/*****************************************************************/

QString BNK_FISI_YAZDIR::GET_BODY()
{
    QStringList headers;
    QStringList basilacak_satir;
    QStringList string_list;


    if ( m_last_satir_indisi >= m_banka_fis_satirlari.size() ) {
        return NULL;
    }

    headers << "<b> Fiş No" << "<b>Açıklama" << "<b>Hesaba Giren"<< "<b> Hesaptan Çıkan";

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 1>");

    SET_TD_TAG_STRING ( QStringList() << "style = \"width:17%;\"" << "style = \"width:23%;\""
                                      << "style = \"width:10%;\"" << "style = \"width:10%;\"");

    CREATE_TABLE ( headers,1,14 );

    SET_TD_TAG_STRING ( QStringList() << "" << "" << "ALIGN = RIGHT" << "ALIGN = RIGHT");


    m_eklenen_kayit_sayisi = 0;

    for ( int i = m_last_satir_indisi ; i < m_banka_fis_satirlari.size() ; i++ ) {

        basilacak_satir = m_banka_fis_satirlari.at(i).split ( "\t" );

        string_list.clear();

        string_list << basilacak_satir.at(0) << basilacak_satir.at(1) << basilacak_satir.at(2)
                    << basilacak_satir.at(3) ;


        ADD_ROW_TO_TABLE ( string_list );
        m_last_satir_indisi++;
        m_eklenen_kayit_sayisi++;

        if ( m_eklenen_kayit_sayisi EQ m_max_line_count ) {
             break;
        }
    }
    return GET_TABLE_HTML_STRING();
}

/*****************************************************************/
/*                  BNK_FISI_YAZDIR::GET_FOOTER                  */
/*****************************************************************/

QString BNK_FISI_YAZDIR::GET_FOOTER()
{
    return NULL;
}

/*****************************************************************/
/*            BNK_FISI_YAZDIR::GET_BANKA_FIS_SATIRLARI();        */
/*****************************************************************/

void BNK_FISI_YAZDIR::GET_BANKA_FIS_SATIRLARI()
{

    QStringList         fis_satiri;

    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "bnk_hareket_fis_satirlari","fis_id , aciklama, hesaba_giren, hesaptan_cikan ",
                                  "fis_id = :fis_id");
    select_query.SET_VALUE      ( ":fis_id" , m_fis_id );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }
    while ( select_query.NEXT() EQ true ) {

        int     fis_no              = select_query.VALUE(0).toInt();
        QString aciklama            = select_query.VALUE(1).toString();
        double  hesaba_giren        = select_query.VALUE(2).toDouble();
        double  hesaptan_cikan      = select_query.VALUE(3).toDouble();


        fis_satiri.clear();
        fis_satiri.append ( QString::number( fis_no )
                            + "\t" + aciklama
                            + "\t" + VIRGUL_EKLE( QVariant( ROUND( hesaba_giren   ) ).toString(), 2 )
                            + "\t" + VIRGUL_EKLE( QVariant( ROUND( hesaptan_cikan ) ).toString(), 2 ) );
        m_banka_fis_satirlari << fis_satiri;
    }

}

/**********************************************************************/
/*              BNK_FISI_YAZDIR::FILL_HEADER_STRUCT                   */
/**********************************************************************/

void BNK_FISI_YAZDIR::FILL_HEADER_STRUCT()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hareket_fisler","fis_tarihi,fis_no,hesap_no_id,aciklama ",
                                "fis_id = :fis_id");
    select_query.SET_VALUE(":fis_id",m_fis_id);

    if(select_query.SELECT() NE 0 ) {
        select_query.NEXT();

        M_HEADER_INFO.fis_tarihi     = select_query.VALUE(0).toDate().toString("dd MMMM yyyy dddd");
        M_HEADER_INFO.fis_no         = select_query.VALUE(1).toString();
        int hesap_no_id              = select_query.VALUE(2).toInt();
        M_HEADER_INFO.aciklama       = select_query.VALUE(3).toString();

        SQL_QUERY sql_query(DB);

        sql_query.PREPARE_SELECT("bnk_hesaplar","hesap_numarasi,doviz_id,banka_ismi,sube_kodu,sube_ismi",
                                 "hesap_no_id = :hesap_no_id");
        sql_query.SET_VALUE(":hesap_no_id" , hesap_no_id);
        if(sql_query.SELECT() NE 0 ) {
            sql_query.NEXT();
            M_HEADER_INFO.hesap_numarasi = sql_query.VALUE(0).toString();
            M_HEADER_INFO.doviz_kodu     = DVZ_GET_DOVIZ_KODU(sql_query.VALUE(1).toInt());
            M_HEADER_INFO.banka_ismi     = sql_query.VALUE(2).toString();
            M_HEADER_INFO.sube_kodu      = sql_query.VALUE(3).toString();
            M_HEADER_INFO.sube_ismi      = sql_query.VALUE(4).toString();
        }
    }
}
