#include <QDate>
#include "adak_utils.h"
#include "cari_fisi_yazdir_raporu_class.h"
#include "cari_enum.h"
#include "yonetim.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
#include "e9_console_utils.h"

extern ADAK_SQL * DB;


/**************************************************************************************
                   GET_CARI_FISI_YAZDIR_RAPORU
***************************************************************************************/


QString GET_CARI_FISI_YAZDIR_RAPORU ( int fis_id,int fis_turu )
{
    CARI_FISI_YAZDIR_RAPORU  *  R = new CARI_FISI_YAZDIR_RAPORU ( fis_id,fis_turu );
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   CARI_FISI_YAZDIR_RAPORU::CARI_FISI_YAZDIR_RAPORU
***************************************************************************************/

CARI_FISI_YAZDIR_RAPORU::CARI_FISI_YAZDIR_RAPORU(int fis_id,int fis_turu) : REPORT_KERNEL ("CARI_FISI_YAZDIR_RAPORU")
{
    m_fis_id                     =  fis_id;
    m_fis_turu                   =  fis_turu;
    m_last_satir_indisi          =  0;
    m_max_line_count             =  GET_MAX_LINE_COUNT (12);
    m_footer_toplam_borc         =  0.0;
    m_footer_toplam_alacak       =  0.0;

    SET_HEIGHTS             ( 50, 30 );
    FILL_HEADER_STRUCT      ();
    GET_CARI_FIS_SATIRLARI  ();
    SET_WRAP                ( NO_WRAP );
}

/**************************************************************************************
                   CARI_FISI_YAZDIR_RAPORU::GET_HEADER
***************************************************************************************/

QString CARI_FISI_YAZDIR_RAPORU::GET_HEADER()
{

    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >");

    SET_TD_TAG_STRING    ( QStringList()<<"style = \"width:35%; font-size:88%;\""<<"style = \"width:30%;font-size:100%;\" ALIGN=CENTER"
                                   << "style = \"width:35%;style = \"font-size:88%;\" ALIGN = RIGHT");

    CREATE_TABLE ( QStringList()<<""<<""<<"",0,0 );

    QString rapor_basligi = "";

    if ( m_fis_turu EQ ENUM_CARI_HAREKET_FISI ) {
        rapor_basligi = QObject::tr("HAREKET FİŞİ");
    }
    else if ( m_fis_turu EQ ENUM_CARI_VIRMAN_FORMU ) {
        rapor_basligi = QObject::tr("VİRMAN FİŞİ");
    }


    ADD_ROW_TO_TABLE ( QStringList() << E9_GET_FIRMA_ISMI() << "<b>" + rapor_basligi+ "</b>"<< GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE ( QStringList() << "<b>Fiş Tarihi" + ADD_HTML_SPACE("       ") + ":</b>" + ADD_HTML_SPACE("   ") +
                       M_REPORT_HEADER_INFO.fis_tarihi <<   "<p style = \"font-size:80%;\"><b>Fiş No : </b> " +
                       M_REPORT_HEADER_INFO.fis_no + ADD_HTML_SPACE("  ")+"</p>"  <<"");

    QString header;
    header.append ( GET_TABLE_HTML_STRING() );

    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >" );
    SET_TD_TAG_STRING ( QStringList() << "style = width:\"90%;\"" << "style = \"widht:10%;\"" );
    CREATE_TABLE ( QStringList()<<""<<"",0,0);
    ADD_ROW_TO_TABLE ( QStringList() << "<b>Açıklama   " + ADD_HTML_SPACE("   ") + ":</b>" + ADD_HTML_SPACE("   ") + M_REPORT_HEADER_INFO.aciklama  << "" );

    return header.append ( GET_TABLE_HTML_STRING() );

}

/**************************************************************************************
                   CARI_FISI_YAZDIR_RAPORU::GET_BODY
***************************************************************************************/

QString CARI_FISI_YAZDIR_RAPORU::GET_BODY()
{
    QStringList headers;
    QStringList basilacak_satir;
    QStringList string_list;


    if ( m_last_satir_indisi >= m_fis_satirlari_list.size() ) {
        return NULL;
    }

    headers << "<b> Cari Hesap <br>Kodu/Adı" << "<b>Açıklama" << "<b>Borç"<< "<b> Alacak"
            << "<b>Para <br>Birimi" << "<b>Döviz Kuru" << "<b>Parite" << "<b>Cari Para <br>Birimi Tutarı";

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 1>");

    SET_TD_TAG_STRING ( QStringList() << "style = \"width:17%;\"" << "style = \"width:23%;\""
                                      << "style = \"width:10%;\"" << "style = \"width:10%;\""
                                      << "style = \"width:7%;\""  << "style = \"width:10%;\""
                                      << "style = \"width:10%;\"" << "style = \"width:13%;\"");

    CREATE_TABLE ( headers,1,14 );

    SET_TD_TAG_STRING ( QStringList() << "" << "" << "ALIGN = RIGHT" << "ALIGN = RIGHT" << ""
                        <<  "ALIGN = RIGHT" <<  "ALIGN = RIGHT" <<  "ALIGN = RIGHT");


    m_eklenen_kayit_sayisi = 0;

    for ( int i = m_last_satir_indisi ; i < m_fis_satirlari_list.size() ; i++ ) {

        basilacak_satir = m_fis_satirlari_list.at(i).split ( "\t" );

        double borc_tutari   = QVariant ( basilacak_satir.at(2) ).toDouble();
        double alacak_tutari = QVariant ( basilacak_satir.at(3) ).toDouble();

        if ( borc_tutari > alacak_tutari ) {
            m_footer_toplam_borc += QVariant(basilacak_satir.at(8)).toDouble();
        }
        else {
            m_footer_toplam_alacak += QVariant(basilacak_satir.at(8)).toDouble();
        }

        string_list.clear();

        string_list << basilacak_satir.at(0) << basilacak_satir.at(1) << basilacak_satir.at(2)
                    << basilacak_satir.at(3) << basilacak_satir.at(4) << basilacak_satir.at(5)
                    << basilacak_satir.at(6) << basilacak_satir.at(7);


        ADD_ROW_TO_TABLE ( string_list );
        m_last_satir_indisi++;
        m_eklenen_kayit_sayisi++;

        if ( m_eklenen_kayit_sayisi EQ m_max_line_count ) {
             break;
        }
    }
    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   CARI_FISI_YAZDIR_RAPORU::GET_FOOTER
***************************************************************************************/

QString CARI_FISI_YAZDIR_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:90%;\">" );

    SET_TD_TAG_STRING ( QStringList() << "style = \"width:40%;\""
                                      << "style = \"width:10%;font-size:90%;\" ALIGN=RIGHT"
                                      << "style = \"width:10%;font-size:90%;\" ALIGN=RIGHT"
                                      << "style= \"width:40%;\"");
    CREATE_TABLE ( QStringList() << "" << "" << "" << "",0,14 );

    ADD_ROW_TO_TABLE ( QStringList() << " " << "<b>" + VIRGUL_EKLE ( QVariant ( m_footer_toplam_borc ).toString() )
                                     << "<b>" + VIRGUL_EKLE( QVariant ( m_footer_toplam_alacak ).toString() ) << "");


    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   CARI_FISI_YAZDIR_RAPORU::GET_CARI_FIS_SATIRLARI
***************************************************************************************/

void CARI_FISI_YAZDIR_RAPORU::GET_CARI_FIS_SATIRLARI()
{
    QStringList         fis_satiri;

    SQL_QUERY select_query ( DB );

    SQL_QUERY query(DB);

    select_query.PREPARE_SELECT ( "car_fis_satirlari", "cari_hesap_id , aciklama,borc_tutari,alacak_tutari , "
                                  "doviz_id,doviz_kuru,parite,cari_para_birimi_tutari,temel_para_birimi_tutari ",
                                  "fis_id = :fis_id");
    select_query.SET_VALUE      ( ":fis_id" , m_fis_id );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }
    while ( select_query.NEXT() EQ true ) {

        int cari_hesap_id              = select_query.VALUE(0).toInt();
        QString aciklama               = select_query.VALUE(1).toString();
        double borc_tutari             = select_query.VALUE(2).toDouble();
        double alacak_tutari           = select_query.VALUE(3).toDouble();
        int doviz_id                   = select_query.VALUE(4).toInt();
        double doviz_kuru              = select_query.VALUE(5).toDouble();
        double parite                  = select_query.VALUE(6).toDouble();
        double cari_para_birimi_tutari = select_query.VALUE(7).toDouble();
        double temel_para_birimi_tutari= select_query.VALUE(8).toDouble();

        QString cari_hesap_kodu, cari_hesap_ismi;
        int para_birim_id;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi, &para_birim_id );

        QString cari_hesap_kodu_adi;
        if ( result NE 0 ) {
            cari_hesap_kodu_adi = cari_hesap_kodu + ADD_HTML_SPACE("  ") +
                                  cari_hesap_ismi + ADD_HTML_SPACE("  ") +
                                  DVZ_GET_DOVIZ_KODU( para_birim_id );
        }

        fis_satiri.clear();
        fis_satiri.append (cari_hesap_kodu_adi + "\t" + aciklama + "\t"+ VIRGUL_EKLE( QVariant(ROUND(borc_tutari )).toString(),2)
                           + "\t" + VIRGUL_EKLE( QVariant(ROUND(alacak_tutari)).toString(),2) + "\t"
                           + DVZ_GET_DOVIZ_KODU(doviz_id) + "\t" + VIRGUL_EKLE( QVariant(ROUND(doviz_kuru,-4)).toString(),2)
                           + "\t" + VIRGUL_EKLE( QVariant(ROUND(parite,-4)).toString(),2) + "\t"
                           + VIRGUL_EKLE( QVariant(ROUND(cari_para_birimi_tutari)).toString(),2)
                           + "\t" + VIRGUL_EKLE( QVariant(ROUND(temel_para_birimi_tutari)).toString(),2));
        m_fis_satirlari_list << fis_satiri;
    }
}

/**************************************************************************************
                   CARI_FISI_YAZDIR_RAPORU::FILL_HEADER_STRUCT
***************************************************************************************/

void CARI_FISI_YAZDIR_RAPORU::FILL_HEADER_STRUCT()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "car_fisler", "fis_tarihi,fis_no,aciklama",
                                  "fis_id = :fis_id" );
    select_query.SET_VALUE      ( ":fis_id" , m_fis_id );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }
    select_query.NEXT();

    M_REPORT_HEADER_INFO.fis_tarihi      = select_query.VALUE(0).toDate().toString("dd MMMM yyyy dddd");
    M_REPORT_HEADER_INFO.fis_no          = select_query.VALUE(1).toString();
    M_REPORT_HEADER_INFO.aciklama        = select_query.VALUE(2).toString();

}
