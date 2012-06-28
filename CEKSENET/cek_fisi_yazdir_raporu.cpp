#include "adak_utils.h"
#include "cek_fisi_yazdir_raporu_class.h"
#include "cari_console_utils.h"
#include "yonetim.h"
#include "banka_struct.h"
#include "bnk_console_utils.h"
#include "cek_enum.h"
#include "cek_console_utils.h"
#include "e9_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   GET_CEK_FISI_YAZDIR_RAPORU
***************************************************************************************/


QString GET_CEK_FISI_YAZDIR_RAPORU ( int bordro_id,int bordro_islem_turu )
{
    CEK_FISI_YAZDIR_RAPORU  *  R = new CEK_FISI_YAZDIR_RAPORU ( bordro_id,bordro_islem_turu );
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   CEK_FISI_YAZDIR_RAPORU::CEK_FISI_YAZDIR_RAPORU
***************************************************************************************/

CEK_FISI_YAZDIR_RAPORU::CEK_FISI_YAZDIR_RAPORU ( int bordro_id,int bordro_islem_turu) : REPORT_KERNEL ("CEK_FISI_YAZDIR_RAPORU")
{
    SET_HEIGHTS( 45,30 );

    m_bordro_id          = bordro_id;
    m_bordro_islem_turu  = bordro_islem_turu;

    FILL_HEADER_STRUCT();
    GET_BORDRO_SATIRLARI();

    m_max_line_count             =  GET_MAX_LINE_COUNT (12);
    m_last_satir_indisi          = 0;
    m_eklenen_kayit_sayisi       = 0;
    m_toplam_cek_tutari          = 0.0;
}


/**************************************************************************************
                   CEK_FISI_YAZDIR_RAPORU::GET_HEADER
***************************************************************************************/

QString CEK_FISI_YAZDIR_RAPORU::GET_HEADER()
{
    QString header;

    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >" ) ;

    SET_TD_TAG_STRING ( QStringList()<<"style = \"width:37%; font-size:88%;\""<<"style = \"width:26%;font-size:110%;\" ALIGN=CENTER"
                                     << "style = \"width:%;style = \"font-size:88%;\" ALIGN = RIGHT" );


    CREATE_TABLE     ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE ( QStringList() << E9_GET_FIRMA_ISMI() << "<b>ÇEK SENET BORDROSU</b>" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE ( QStringList() << "" << "" << "" );

    header.append ( GET_TABLE_HTML_STRING() );

    switch ( m_bordro_islem_turu ) {
        case ENUM_CEK_IADE_ALINDI_SATICIDAN :
        case ENUM_CEK_MUS_IADE_EDILDI        :
        case ENUM_CEK_SATICIYA_VERILDI            :

            SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >");
            SET_TD_TAG_STRING ( QStringList() << "style = \"width:10%;\"" << "style = \"width:80%; font-size:88%;\"ALIGN = CENTER" << " td style = \"width:10%;\"");

            CREATE_TABLE(QStringList() << "" << "" << "",0,0);
            ADD_ROW_TO_TABLE(QStringList() << "" << "<u><b>Cari Hesap" + ADD_HTML_SPACE(" ")+ ":</b>"+ ADD_HTML_SPACE("   ")+ P_REPORT_HEADER_INFO.cari_hesap_kodu
                         + ADD_HTML_SPACE("   ") + P_REPORT_HEADER_INFO.cari_hesap_ismi + "</u>"<< "");
            header.append(GET_TABLE_HTML_STRING());
            
            break;
        case ENUM_CEK_TAHSIL_EDILDI_HESABA   :
        case ENUM_CEK_ODENDI_HESAPTAN        :
        case ENUM_CEK_BNK_TAHSILATA_VERILDI  :
        case ENUM_CEK_BNK_TEMINATA_VERILDI   :
        case ENUM_CEK_IADE_ALINDI_BANKADAN   :
            SET_TABLE_TAG_STRING("<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >");
            SET_TD_TAG_STRING(QStringList() << "style = \"width:10%;\"" << "style = \"width:80%; font-size:88%;\"ALIGN = CENTER" << " td style = \"width:10%;\"" );

            CREATE_TABLE(QStringList() << "" << "" << "",0,0);
            ADD_ROW_TO_TABLE ( QStringList() << ""<< "<b><u> Banka Bilgisi" + ADD_HTML_SPACE(" ") + ":</b>" + ADD_HTML_SPACE("   ") + P_REPORT_HEADER_INFO.banka_kodu + ADD_HTML_SPACE("  ") + P_REPORT_HEADER_INFO.banka_ismi
                                             + " / "  + P_REPORT_HEADER_INFO.sube_kodu + ADD_HTML_SPACE("  ") + P_REPORT_HEADER_INFO.sube_ismi + " / " + P_REPORT_HEADER_INFO.hesap_numarasi + "</u>"<< "" );

            header.append ( GET_TABLE_HTML_STRING() );
            break;
        default :
                break;

    };
    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >" );
    SET_TD_TAG_STRING ( QStringList()<<"style = \"width:42%; font-size:88%;\""<<"style = \"width:16%;font-size:110%;\" ALIGN=CENTER"
                                     << "style = \"width:42%;font-size:88%;\" ALIGN = RIGHT" );
    CREATE_TABLE     ( QStringList() << "" << "" << "",0,0  );

    ADD_ROW_TO_TABLE ( QStringList() << "<b> İşlem Türü" + ADD_HTML_SPACE("  ") + ":</b>" + ADD_HTML_SPACE("   ") + P_REPORT_HEADER_INFO.islem_turu << "" << "");
    ADD_ROW_TO_TABLE ( QStringList() << "<b>Fiş Tarihi" + ADD_HTML_SPACE("    ") + ":</b>" + ADD_HTML_SPACE("   ") + P_REPORT_HEADER_INFO.fis_tarihi <<   "<p style = \"font-size:80%;\"><b>Bordro No : </b> "
                                    + P_REPORT_HEADER_INFO.bordro_no + ADD_HTML_SPACE("  ")+"</p>"  <<"");

    ADD_ROW_TO_TABLE ( QStringList() << "<b>Açıklama   " + ADD_HTML_SPACE("   ") + ":</b>" + ADD_HTML_SPACE("   ") + P_REPORT_HEADER_INFO.aciklama.mid(0,40) << "" << "");

    if  ( P_REPORT_HEADER_INFO.aciklama.size() > 40 ) {
        ADD_ROW_TO_TABLE ( QStringList() << P_REPORT_HEADER_INFO.aciklama.mid(40) << "" << "" );
    }
    return header.append ( GET_TABLE_HTML_STRING() );

}

/**************************************************************************************
                   CEK_FISI_YAZDIR_RAPORU::GET_BODY
***************************************************************************************/

QString CEK_FISI_YAZDIR_RAPORU::GET_BODY()
{
    QStringList headers;
    QStringList basilacak_satir;
    QStringList string_list;


    if ( m_last_satir_indisi >= m_bordro_satirlari_list.size() ) {
        return NULL;
    }

    headers << "<b>Portföy No" << "<b>Çek Numarası"<<"<b>Vade Tarihi"<<"<b>Valör"<<"<b>Banka İsmi"
            <<"<b>Şubesi"<<"<b>Açıklama"<<"<b>Tutar";

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 1>" );

    SET_TD_TAG_STRING ( QStringList() << "style = \"width:9%;\"" << "style = \"width:11%;\"" << "style = \"width:13%;\""
                                      << "style = \"width:5%;\"" << "style = \"width:10%;\"" << "style = \"width:17%;\""
                                      << "style = \"width:25%;\""<< "style = \"width:10%;\"" );
    CREATE_TABLE ( headers,1,14 );

    SET_TD_TAG_STRING ( QStringList() << "" << "" << "" << "" << "" << ""<<""<<"ALIGN = RIGHT" );


    m_eklenen_kayit_sayisi = 0;

    for  ( int i = 0 ; i < m_bordro_satirlari_list.size() ; i++ ) {
           basilacak_satir = m_bordro_satirlari_list.at(i).split("\t");

        string_list.clear();

        m_toplam_cek_tutari += QVariant(basilacak_satir.at(8)).toDouble();

        string_list << basilacak_satir.at(0) << basilacak_satir.at(1) << basilacak_satir.at(2) << basilacak_satir.at(3)
                    << basilacak_satir.at(4) << basilacak_satir.at(5)+ADD_HTML_SPACE("  ") + basilacak_satir.at(6)
                    << basilacak_satir.at(7) << VIRGUL_EKLE(basilacak_satir.at(8));

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
                   CEK_FISI_YAZDIR_RAPORU::GET_FOOTER
***************************************************************************************/

QString CEK_FISI_YAZDIR_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:85%;\">" );

    SET_TD_TAG_STRING ( QStringList() << "style = \"width:87%;\""<< "style = \"width:13%;\" ALIGN=RIGHT");
    CREATE_TABLE ( QStringList() << "" << "",0,14 );

    ADD_ROW_TO_TABLE ( QStringList() << "" << VIRGUL_EKLE ( QVariant ( m_toplam_cek_tutari ).toString() ) );

    if ( m_bordro_islem_turu EQ ENUM_CEK_BNK_TAHSILATA_VERILDI ) {
        SET_TD_TAG_STRING ( QStringList() << "style = \"width:87%;\""<< "style = \"width:13%;\"");

        ADD_ROW_TO_TABLE(QStringList () << "" << "");
        QString aciklama = "<b>İşbu bordro ekinde bulunan çeklerin takas odası aracılığıyla ibraz edilmesi, "
                           "karşılığı yok ise karşılıksız  işlemine tabi tutulması ve karşılıksız şerhinin <b>altının 5941 sayılı";
        ADD_ROW_TO_TABLE(QStringList() << aciklama << "");
        ADD_ROW_TO_TABLE(QStringList() << "<b>Çek Kanunu'nun 3. maddesinin  4. bendi gereğince adıma Bankanız tarafından imzalanmasına muvafakat ederim." << "");
    }

    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   CEK_FISI_YAZDIR_RAPORU::GET_BORDRO_SATIRLARI
***************************************************************************************/

void CEK_FISI_YAZDIR_RAPORU::GET_BORDRO_SATIRLARI()
{
    SQL_QUERY select_query ( DB );
    QStringList bordro_satiri;

    select_query.PREPARE_SELECT ( "cek_cekler_senetler, cek_bordro_satirlari",
                                  "cek_bordro_satirlari.cek_senet_id,aciklama,cek_senet_numarasi,vade_tarihi,"
                                  "banka_ismi,sube_kodu,sube_ismi,cek_senet_tutari,portfoy_numarasi ",
                                  "cek_bordro_satirlari.cek_senet_id = cek_cekler_senetler.cek_senet_id "
                                  "AND bordro_id = :bordro_id" );
    select_query.SET_VALUE      ( ":bordro_id" , m_bordro_id );

    if  ( select_query.SELECT() NE 0 ) {
        while ( select_query.NEXT() ) {
            QString aciklama           = select_query.VALUE(1).toString();
            QString cek_senet_numarasi = select_query.VALUE(2).toString();
            QDate vade_tarihi          = QDate::fromString(select_query.VALUE(3).toString(),"yyyy.MM.dd" );
            QString valor              = QVariant ( CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI( vade_tarihi ) ).toString();
            QString banka_ismi         = select_query.VALUE(4).toString();
            QString sube_kodu          = select_query.VALUE(5).toString();
            QString sube_ismi          = select_query.VALUE(6).toString();
            double cek_senet_tutari    = select_query.VALUE(7).toDouble();
            QString portfoy_no         = select_query.VALUE(8).toString();

            bordro_satiri.clear();
            bordro_satiri.append ( portfoy_no + "\t" + cek_senet_numarasi + "\t" +
                                  select_query.VALUE(3).toDate().toString("dd MMMM yyyy") + "\t" + valor + "\t"
                                  + banka_ismi + "\t" + sube_kodu + "\t" + sube_ismi + "\t" + aciklama + "\t"
                                  + QVariant ( ROUND ( cek_senet_tutari ) ).toString() );
            m_bordro_satirlari_list << bordro_satiri;

        }
    }
}

/**************************************************************************************
                   CEK_FISI_YAZDIR_RAPORU::FILL_HEADER_STRUCT
***************************************************************************************/

void CEK_FISI_YAZDIR_RAPORU::FILL_HEADER_STRUCT()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "cek_bordrolar",
                                  "bordro_tarihi,bordro_islem_turu,aciklama,bordro_no,cari_hesap_id, "
                                  "hesap_no_id ",
                                  "bordro_id = :bordro_id" );
    select_query.SET_VALUE      ( ":bordro_id" , m_bordro_id );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }



    select_query.NEXT();

    P_REPORT_HEADER_INFO.fis_tarihi      = select_query.VALUE(0).toDate().toString("dd MMMM yyyy");
    int islem_turu                       = select_query.VALUE(1).toInt();
    P_REPORT_HEADER_INFO.islem_turu      = CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(islem_turu);

    P_REPORT_HEADER_INFO.aciklama        = select_query.VALUE(2).toString();
    P_REPORT_HEADER_INFO.bordro_no = select_query.VALUE(3).toString();

    int cari_hesap_id                      = select_query.VALUE(4).toInt();
    P_REPORT_HEADER_INFO.cari_hesap_kodu = CARI_FIND_HESAP_KODU(cari_hesap_id);
    P_REPORT_HEADER_INFO.cari_hesap_ismi = CARI_FIND_HESAP_ISMI(cari_hesap_id);

    int hesap_no_id = select_query.VALUE(5).toInt();

    BNK_BANKA_BILGILERI_STRUCT BNK_BILGILER;

    BNK_GET_BANKA_TEMEL_BILGILER( &BNK_BILGILER, hesap_no_id );

    P_REPORT_HEADER_INFO.banka_kodu     = BNK_BILGILER.banka_hesap_kodu;
    P_REPORT_HEADER_INFO.banka_ismi     = BNK_BILGILER.banka_ismi;
    P_REPORT_HEADER_INFO.sube_kodu      = BNK_BILGILER.sube_kodu;
    P_REPORT_HEADER_INFO.sube_ismi      = BNK_BILGILER.sube_ismi;
    P_REPORT_HEADER_INFO.hesap_numarasi = BNK_BILGILER.hesap_numarasi;

}












