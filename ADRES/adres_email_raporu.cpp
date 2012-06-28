#include "adak_utils.h"
#include "adres_email_raporu_class.h"
#include <QDate>
#include "yonetim.h"
#include "e9_console_utils.h"

extern ADAK_SQL *         DB;
extern ADAK_SQL *         G_YONETIM_DB;

/**************************************************************************************
                   GET_ADRES_EMAIL_RAPORU
***************************************************************************************/

QString GET_EMAIL_LOG_RAPORU (EMAIL_RAPOR_STRUCT * report_struct)
{
    ADRES_EMAIL_RAPORU  *  F = new ADRES_EMAIL_RAPORU( report_struct , "EMAIL_LOG_RAPORU" );
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   ADRES_EMAIL_RAPORU::ADRES_EMAIL_RAPORU
***************************************************************************************/

ADRES_EMAIL_RAPORU::ADRES_EMAIL_RAPORU( EMAIL_RAPOR_STRUCT * report_struct , QString rapor_ismi ) : REPORT_KERNEL (rapor_ismi)
{
    SET_HEIGHTS ( 30, 30 );

    m_sayfa_numarasi        = 0;
    m_email_struct          = report_struct;
    m_max_line_count        = GET_MAX_LINE_COUNT ( 14 );

    CREATE_REPORT_BODY();
}

/**************************************************************************************
                   ADRES_EMAIL_RAPORU::GET_HEADER
***************************************************************************************/

QString ADRES_EMAIL_RAPORU::GET_HEADER()
{
    QString header_string;

    QStringList string_list;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER = 0 >" );

    SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 25%"<<"WIDTH = 50% ALIGN = CENTER"<<"WIDTH = 25% ALIGN = RIGHT" );

    CREATE_TABLE ( QStringList()<<""<<""<<"",0,0) ;

    QString baslik = m_email_struct->bas_tarihi + "-" + m_email_struct->bts_tarihi;

    string_list << E9_GET_FIRMA_ISMI();
    string_list << "<b>" + baslik  + "<b>";
    string_list << GET_REPORT_TOP_RIGHT_HEADER();

    ADD_ROW_TO_TABLE( string_list );

    m_sayfa_numarasi++;

    QString sayfa_numarasi = "Sayfa No : ";

    sayfa_numarasi.append ( QVariant ( m_sayfa_numarasi ).toString() );

    string_list.clear();

    string_list <<""<<""<<sayfa_numarasi;

    ADD_ROW_TO_TABLE ( string_list );

    header_string = GET_TABLE_HTML_STRING();

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER = 1 >" );

    SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 15% ALIGN=LEFT"<<"WIDTH = 50% ALIGN=LEFT"<<"WIDTH = 35% ALIGN=LEFT" );

    CREATE_TABLE ( QStringList()<<"Tarih - Saat"<<"Kullanıcı Kodu - Kullanıcı Adı ( E-Mail Adresi )"<<"Gönderilen E-Mail Adresi",0,0 );

    header_string.append ( GET_TABLE_HTML_STRING() ) ;

    return header_string;
}

/**************************************************************************************
                   ADRES_EMAIL_RAPORU::GET_BODY
***************************************************************************************/

QString ADRES_EMAIL_RAPORU::GET_BODY()
{
    if ( m_report_lines.size() EQ 0 ) {
        return NULL;
    }

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER=0>" );

    SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 15% ALIGN=LEFT"<<"WIDTH = 50% ALIGN=LEFT"<<"WIDTH = 35% ALIGN=LEFT" );

    CREATE_TABLE ( QStringList()<< "" << "" << "" ,0,0 );

    for ( int i = 0 ; i < m_report_lines.size() ; i++ ) {
        QStringList row_datas = m_report_lines.at(i).split("\t");

        ADD_ROW_TO_TABLE(row_datas);

        m_report_lines[i] = "-1";

        if ( i EQ m_max_line_count) {
            break;
        }
    }

    m_report_lines.removeAll("-1");

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   ADRES_EMAIL_RAPORU::GET_FOOTER
***************************************************************************************/

QString ADRES_EMAIL_RAPORU::GET_FOOTER()
{
    return "";
}

/**************************************************************************************
                   ADRES_EMAIL_RAPORU::CREATE_REPORT_BODY
***************************************************************************************/

void ADRES_EMAIL_RAPORU::CREATE_REPORT_BODY()
{
    SQL_QUERY query (G_YONETIM_DB);

    QList<int>   kullanici_idleri;
    QStringList  kullanici_adlari;

    query.PREPARE_SELECT("ynt_kullanicilar","kullanici_id,kullanici_kodu,kullanici_adi,email_adresi");

    if ( m_email_struct->kullanici_adi.isEmpty() EQ false ) {
        query.AND_EKLE("kullanici_adi like :kullanici_adi");
        query.SET_LIKE(":kullanici_adi",m_email_struct->kullanici_adi);
    }

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            kullanici_idleri    << query.VALUE(0).toInt();
            kullanici_adlari    << query.VALUE(1).toString() + " - " + query.VALUE(2).toString() + " ( " + query.VALUE(3).toString() + " ) ";
        }
    }
    else {
        return;
    }

    query.PREPARE_SELECT("adak_email_log","tarih,saat,gonderilen_email,kullanici_id","tarih>=:bas_tarihi AND tarih<=:bts_tarihi AND gonderilen_email LIKE :email");
    query.SET_VALUE     (":bas_tarihi",m_email_struct->bas_tarihi  );
    query.SET_VALUE     (":bts_tarihi",m_email_struct->bts_tarihi  );
    query.SET_LIKE      (":email"     ,m_email_struct->email_adresi);

    if ( kullanici_idleri.size() > 0 ) {
        query.AND_MULTI_EKLE("kullanici_id",query.TO_QVARIANT(kullanici_idleri),ADAK_OR);
    }

    if ( query.SELECT("tarih,saat") EQ 0 ) {
        return;
    }

    while ( query.NEXT() EQ true ) {
        QString satir = QDate::fromString(query.VALUE(0).toString(),"yyyy.MM.dd").toString("dd MMMM yyyy") + " - " + query.VALUE(1).toString();

        if ( kullanici_idleri.isEmpty() EQ false ) {
            satir.append("\t");
            int index = kullanici_idleri.indexOf(query.VALUE(3).toInt());
            satir.append(kullanici_adlari.at(index));
        }
        else {
            satir.append("\t ");
        }

        satir.append("\t" + query.VALUE(2).toString());

        m_report_lines << satir;
    }
}
