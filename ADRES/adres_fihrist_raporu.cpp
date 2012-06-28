#include <QDate>
#include "adak_utils.h"
#include "adres_fihrist_raporu_class.h"
#include "e9_console_utils.h"
#include "yonetim.h"


extern ADAK_SQL *     DB;

#define HARF_SAYISI     38

QString HARF_DIZISI [ 38 ] = {  "0","1","2","3","4","5","6","7","8","9",
                                "a","b","c","Ç","d","e","f","g","h","ı","İ",
                                "j","k","l","m","n","o","Ö","p","r",
                                 "s","Ş","t","u","Ü","v","y","z" };

/**************************************************************************************
                    OPEN_ADRES_FIHRIST_RAPORU
***************************************************************************************/

QString OPEN_ADRES_FIHRIST_RAPORU ( int baslangic_harf_index,int bitis_harf_index , QList<int> gruba_gore_kayit_idleri )
{
    ADRES_FIHRIST_RAPORU  *  R = new ADRES_FIHRIST_RAPORU ( baslangic_harf_index,bitis_harf_index , gruba_gore_kayit_idleri );
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU::ADRES_FIHRIST_RAPORU
***************************************************************************************/

ADRES_FIHRIST_RAPORU::ADRES_FIHRIST_RAPORU ( int baslangic_harf_index,int bitis_harf_index , QList<int> gruba_gore_kayit_idleri) : REPORT_KERNEL ("ADRES_FIHRIST_RAPORU")
{
    SET_HEIGHTS (30,30);
    m_max_line_count                 =  GET_MAX_LINE_COUNT (8);
    m_sayfa_numarasi                 = 0;
    m_harf_indexi                    = 0;
    m_satir_numarasi                 = 0;
    m_body_basligi_olusturuldu_mu    = false;
    m_indexteki_harf_yazimi_bitti_mi = true;
    m_baslangic_harf_index           = baslangic_harf_index;
    m_bitis_harf_index               = bitis_harf_index;
    m_gruba_gore_kayit_idleri        = gruba_gore_kayit_idleri;
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU::GET_HEADER
***************************************************************************************/

QString ADRES_FIHRIST_RAPORU::GET_HEADER()
{

    SET_TABLE_TAG_STRING("<TABLE style=\"width:100%; font-size:75%;\" BORDER = 0 >");

    QStringList string_list;

    CREATE_TABLE ( QStringList()<<""<<""<<"",0,0 );

    SET_TD_TAG_STRING(QStringList()<<"style=\"width:25%;\""<<"style=\"width:50%;\" ALIGN = CENTER"
                                   <<"style=\"width:25%;\" ALIGN = RIGHT ");

    string_list << E9_GET_FIRMA_ISMI();
    string_list << "<b>FİHRİST<b>";
    string_list << GET_REPORT_TOP_RIGHT_HEADER();

    ADD_ROW_TO_TABLE ( string_list );

    m_sayfa_numarasi++;

    QString sayfa_numarasi = "Sayfa No : ";
    sayfa_numarasi.append ( QVariant ( m_sayfa_numarasi ).toString() );

    string_list.clear();

    string_list <<""<<""<<sayfa_numarasi;

    ADD_ROW_TO_TABLE ( string_list );

    m_body_basligi_olusturuldu_mu = false;
    m_satir_numarasi              = 0;

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU::GET_BODY
***************************************************************************************/

QString ADRES_FIHRIST_RAPORU::GET_BODY()
{
    QString     body_string;
    QStringList satir_bilgileri;

    for ( int i = m_baslangic_harf_index; i < m_bitis_harf_index; i++ ) {

        if ( m_indexteki_harf_yazimi_bitti_mi EQ true ) {
            if ( ADRES_KAYIT_BILGILERINI_AL ( HARF_DIZISI [ i ] ) EQ false ) {
                m_baslangic_harf_index++;
                continue;
            }
        }

        for ( int j =  m_adres_kayitlari_index; j < M_ADRES_KAYITLARI.size(); j++ ) {
            satir_bilgileri             = M_ADRES_KAYITLARI.at(j).split("\t");
            int     adres_id            = QVariant ( satir_bilgileri.at(0) ).toInt();
            QString firma_sahis_adi     = satir_bilgileri.at(1);
            QString telefon             = satir_bilgileri.at(2);
            QString fax                 = satir_bilgileri.at(3);
            int     adres_kart_turu   = QVariant ( satir_bilgileri.at(4) ).toInt();

            if ( m_body_basligi_olusturuldu_mu EQ false ) {
                body_string.append( BODY_BASLIGI_OLUSTUR() );
                BODY_TABLOSU_OLUSTUR();
            }

            ADD_ROW_TO_TABLE ( QStringList()<<"<b>"+ firma_sahis_adi + "</b>"<< telefon << fax );
            m_satir_numarasi++;
            m_adres_kayitlari_index++;
            if ( m_satir_numarasi EQ m_max_line_count ) {
                body_string.append ( GET_TABLE_HTML_STRING() );
                return body_string;
            }

            if ( adres_kart_turu EQ FIRMA_KAYDI ) {

                if ( FIRMA_YETKILI_BILGILERINI_AL ( adres_id ) EQ true )  {

                    for ( int k = 0; k < M_FIRMA_YETKILILERI.size(); k++ ) {

                        satir_bilgileri            = M_FIRMA_YETKILILERI.at(k).split("\t");
                        QString yetkili_adi_soyadi = satir_bilgileri.at(0);
                        QString yetkili_telefonu   = satir_bilgileri.at(1);
                        QString yetkili_fax        = satir_bilgileri.at(2);

                        ADD_ROW_TO_TABLE ( QStringList()<< "&nbsp;&nbsp;&nbsp;" + yetkili_adi_soyadi<<yetkili_telefonu<<yetkili_fax);

                        m_satir_numarasi++;

                        if ( m_satir_numarasi EQ m_max_line_count ) {
                            body_string.append(GET_TABLE_HTML_STRING());
                            return body_string;
                        }
                    }
                }
            }
        }

        if ( m_adres_kayitlari_index > 0 ) {
            body_string.append ( GET_TABLE_HTML_STRING() );
        }

        m_indexteki_harf_yazimi_bitti_mi = true;
        m_baslangic_harf_index++;
        return body_string;
    }
    return NULL;
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU::GET_FOOTER
***************************************************************************************/

QString ADRES_FIHRIST_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% BORDER=0 style=\"font-size:75%;\">" );
    SET_TD_TAG_STRING    ( QStringList() << "WIDTH= 100%" );

    CREATE_TABLE ( QStringList()<< "" << "" << "" << "" << "",0,0 );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU::BODY_BASLIGI_OLUSTUR
***************************************************************************************/

QString ADRES_FIHRIST_RAPORU::BODY_BASLIGI_OLUSTUR()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:75%;\" BORDER=1 >" );

    SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 33%"
                                     <<"WIDTH = 33% ALIGN=CENTER"
                                     <<"WIDTH = 34% ALIGN=CENTER" );

    CREATE_TABLE ( QStringList() <<""<<"<b>"+QObject::tr(HARF_DIZISI [ m_baslangic_harf_index ].toUpper().toLatin1() )+"</b>"<<"", 0, 0 );
    ADD_ROW_TO_TABLE ( QStringList()<<"<b>İSİM</b>"<<"<b>TELEFONLAR</b>"<<"<b>FAX</b>" );
    m_satir_numarasi+=2;

    m_body_basligi_olusturuldu_mu = true;
    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU::BODY_TABLOSU_OLUSTUR
***************************************************************************************/

void ADRES_FIHRIST_RAPORU::BODY_TABLOSU_OLUSTUR()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:75%;\" BORDER = 0 >" );

    SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 33%"
                                   <<"WIDTH = 33% ALIGN=CENTER"
                                   <<"WIDTH = 34% ALIGN=CENTER" );

    CREATE_TABLE ( QStringList() <<""<<""<<"", 0, 0 );
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU::ADRES_KAYIT_BILGILERINI_AL
***************************************************************************************/

bool ADRES_FIHRIST_RAPORU::ADRES_KAYIT_BILGILERINI_AL ( QString fihrist_harfi )
{
    SQL_QUERY sql_query    ( DB );

    bool fihrist_bilgisi_var         = false;
    m_indexteki_harf_yazimi_bitti_mi = false;

    QString satir_bilgisi;
    M_ADRES_KAYITLARI.clear();
    M_FIRMA_YETKILILERI.clear();
    
    QString where_str = "firma_id = :firma_id ";

    if (m_gruba_gore_kayit_idleri.isEmpty() EQ false) {
        where_str.append("AND (");
        for ( int i = 0 ; i < m_gruba_gore_kayit_idleri.size() ; i++ ) {
            if ( i NE 0 ) {
                where_str.append(" OR ");
            }

            where_str.append(QString(" adres_id = %1 ").arg(m_gruba_gore_kayit_idleri.at(i)));
        }
        where_str.append(") ");
    }

    where_str.append ( QString ( "AND firma_sahis_adi LIKE :firma_sahis_adi " ) );

    sql_query.PREPARE_SELECT("adr_adresler","adres_id, firma_sahis_adi, tel_ulke_kodu, tel_alan_kodu, telefon, "
                             "fax_ulke_kodu, fax_alan_kodu, fax, adres_kart_turu",where_str);

    sql_query.SET_VALUE(":firma_id",0);
    sql_query.SET_VALUE(":firma_sahis_adi",QString( fihrist_harfi + "%"));

    if ( sql_query.SELECT("firma_sahis_adi") EQ 0 ) {

        m_indexteki_harf_yazimi_bitti_mi = true;
        return fihrist_bilgisi_var;
    }

    while ( sql_query.NEXT() EQ true ) {

        fihrist_bilgisi_var     = true;
        m_adres_kayitlari_index = 0;

        satir_bilgisi.clear();

        QString telefon_str     = "";
        QString fax_telefon_str = "";

        if ( sql_query.VALUE(4).toString().isEmpty() EQ false ) {
            telefon_str = sql_query.VALUE(2).toString() + "-" + sql_query.VALUE(3).toString() + "-" + sql_query.VALUE(4).toString();
        }

        if ( sql_query.VALUE(7).toString().isEmpty() EQ false ) {
            fax_telefon_str = sql_query.VALUE(5).toString() +   "-" + sql_query.VALUE(6).toString() + "-" + sql_query.VALUE(7).toString();
        }

        satir_bilgisi.append ( sql_query.VALUE(0).toString() + "\t" + sql_query.VALUE(1).toString()  + "\t" +
                               telefon_str + "\t" + fax_telefon_str + "\t" + sql_query.VALUE(8).toString() ) ;

        M_ADRES_KAYITLARI.append ( satir_bilgisi );
    }

    if ( fihrist_bilgisi_var EQ false ) {
        m_indexteki_harf_yazimi_bitti_mi = true;
    }

    return fihrist_bilgisi_var;
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU::FIRMA_YETKILI_BILGILERINI_AL
***************************************************************************************/

bool ADRES_FIHRIST_RAPORU::FIRMA_YETKILI_BILGILERINI_AL ( int p_firma_id )
{
    M_FIRMA_YETKILILERI.clear();

    if (m_gruba_gore_kayit_idleri.isEmpty() EQ false) {
        if ( m_gruba_gore_kayit_idleri.contains(p_firma_id) EQ false ) {
            return false;
        }
    }

    SQL_QUERY sql_query ( DB );

    QString satir_bilgisi;

    QString cep_telefonu;
    QString fax;

    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi, cep_tel_ulke_kodu, "
                               "cep_tel_gsm_kodu, cep_telefonu, fax_ulke_kodu, fax_alan_kodu, fax",
                               "firma_id = :firma_id" );

    sql_query.SET_VALUE      ( ":firma_id", p_firma_id );

    if ( sql_query.SELECT("firma_sahis_adi") EQ 0 ) {
        return false;
    }

    while ( sql_query.NEXT() EQ true ) {

        satir_bilgisi.clear();

        cep_telefonu.clear();
        fax.clear();

        if ( sql_query.VALUE(3).toString().isEmpty() EQ false ) {
            cep_telefonu.append ( sql_query.VALUE(1).toString() + "-" + sql_query.VALUE(2).toString() + "-" +
            sql_query.VALUE(3).toString() );
        }

        if ( sql_query.VALUE(6).toString().isEmpty() EQ false ) {
            fax.append ( sql_query.VALUE(4).toString() + "-" + sql_query.VALUE(5).toString() + "-" + sql_query.VALUE(6).toString() );
        }

        satir_bilgisi.append ( sql_query.VALUE(0).toString()  + "\t" + cep_telefonu + "\t" + fax );

        M_FIRMA_YETKILILERI.append ( satir_bilgisi );
    }
    return true;
}

