#include <QDate>
#include "adak_utils.h"
#include "muh_toplu_fis_raporu_class.h"
#include "muh_toplu_fis_raporu_open.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "adak_sql.h"
#include "e9_enum.h"
#include "muh_console_utils.h"

extern ADAK_SQL *       DB;

/**************************************************************************************
                   GET_TOPLU_FIS_RAPORU
***************************************************************************************/

QString GET_TOPLU_FIS_RAPORU (int fis_id,MUH_RAPOR_VARS * rapor_vars)
{
    TOPLU_FIS_RAPORU  *  R = new TOPLU_FIS_RAPORU ( fis_id ,rapor_vars );
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   TOPLU_FIS_RAPORU::TOPLU_FIS_RAPORU
***************************************************************************************/

TOPLU_FIS_RAPORU::TOPLU_FIS_RAPORU ( int fis_id , MUH_RAPOR_VARS * rapor_vars ) : REPORT_KERNEL ("MUH_TOPLU_FIS_RAPORU")
{
    M_MUH_RV = rapor_vars;

    SET_HEIGHTS (30, 20);

    m_baslangic_tarihi              = M_MUH_RV->baslangic_tarihi;
    m_bitis_tarihi                  = M_MUH_RV->bitis_tarihi;
    m_baslangic_fisi                = fis_id;
    m_bitis_fisi                    = fis_id;

    m_rapor_bitti_mi                = false;
    m_fis_bitti_mi                  = true;

    m_max_line_count                = GET_MAX_LINE_COUNT ( 11 );
    m_son_gonderilen_kayit_id       = 0;
    m_toplam_borc                   = 0.00;
    m_toplam_alacak                 = 0.00;
    m_fis_sayisi                    = -1;

    m_sayfadaki_toplam_borc         = 0.00;
    m_sayfadaki_toplam_alacak       = 0.00;

    SQL_QUERY       sql_query ( DB );

    sql_query.PREPARE_SELECT ( "e9_sabit_degerler","firma_ismi" );

    if ( sql_query.SELECT() > 0 ) {
         sql_query.NEXT();
        m_firma_ismi = sql_query.VALUE(0).toString();
    }
    M_MUH_HESAPLAR   = MUH_GET_HESAPLAR ( );

    SET_REPORT_VARIABLES();
}

/**************************************************************************************
                   OPLU_FIS_RAPORU::GET_HEADER
***************************************************************************************/

QString TOPLU_FIS_RAPORU::GET_HEADER()
{
    if ( m_fis_bitti_mi EQ true ) {

        for ( ; ; ) {

            m_fis_sayisi++;

            if ( m_fis_sayisi + 1 > M_TUM_FISLER.size() ) {
                return NULL;
            }

            m_body_string             = M_TUM_FISLER.at(m_fis_sayisi);
            m_son_gonderilen_kayit_id = 0;
            m_fis_bitti_mi            = false;
            M_FIS_BILGI_SUTUNLARI     = M_FIS_BILGILERI.at(m_fis_sayisi).split("\t");

            if ( m_bitis_fisi EQ -1 OR m_baslangic_fisi EQ -1 ) {
                break;
            }
            if ( M_FIS_BILGI_SUTUNLARI.at(0).toInt() <= m_bitis_fisi AND M_FIS_BILGI_SUTUNLARI.at(0).toInt() >= m_baslangic_fisi ) {
                break;
            }
        }
    }

    if ( m_fis_sayisi  EQ M_TUM_FISLER.size() ) {
        return NULL;
    }

    QString         header_html_string;
    QString         header_bilgileri[4];

    header_bilgileri[0]         = "Fiş Tarihi :   " + M_FIS_BILGI_SUTUNLARI.at(1);
    header_bilgileri[1]         = "Fiş No : " + M_FIS_BILGI_SUTUNLARI.at(4);
    header_bilgileri[2]         = "Yevmiye No :   " + M_FIS_BILGI_SUTUNLARI.at(2);
    header_bilgileri[3]         = "Açıklama   :   " + M_FIS_BILGI_SUTUNLARI.at(3);

    SET_TABLE_TAG_STRING("<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0>");

    SET_TD_TAG_STRING ( QStringList () << "WIDTH=50%" << "WIDTH=25% align=right" << "WIDTH=25% align=right" );

    CREATE_TABLE      ( QStringList () << m_firma_ismi << "" << "", 0, 0);

    ADD_ROW_TO_TABLE  ( QStringList () << header_bilgileri[0] << header_bilgileri[1] << header_bilgileri[2] );

    header_html_string = GET_TABLE_HTML_STRING();

    CREATE_TABLE ( QStringList() << header_bilgileri[3] << "" << "" , 0, 12);

    header_html_string.append ( GET_TABLE_HTML_STRING() );

    return header_html_string;
}

/**************************************************************************************
                   TOPLU_FIS_RAPORU::GET_BODY
***************************************************************************************/

QString TOPLU_FIS_RAPORU::GET_BODY()
{

    QStringList         STRING_LIST;
    QStringList         TABLE_ROWS;
    QStringList         TABLE_COLUMNS;

    if ( m_rapor_bitti_mi EQ true ) {
        return NULL;
    }

    if( m_fis_sayisi + 1 > M_TUM_FISLER.size() ) {
        return NULL;
    }

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:75%;\" BORDER=1" );

    STRING_LIST <<"Hesap Kodu"<<"Hesap İsmi"<<"Açıklama"<<"<div align=right>Borç Tutarı</div>"
                <<"<div align=right>Alacak Tutarı</div>";

    SET_TD_TAG_STRING ( QStringList() << "width=15%" << "width=30%" << "width=25%" << "WIDTH=15% align=RIGHT"
                                      << "WIDTH=15% align=RIGHT");

    CREATE_TABLE      ( STRING_LIST, 0, 0);

    TABLE_ROWS = m_body_string.split ( "<br>" );

    int max_line_counter = 0;

    for ( int i = m_son_gonderilen_kayit_id; i < TABLE_ROWS.size() ; i++ ) {

        TABLE_COLUMNS = TABLE_ROWS.at(i).split ( "\t" );

        if ( TABLE_COLUMNS.size() EQ 2 ) {
             TABLE_COLUMNS << "" << "" << "";
        }

        if ( TABLE_COLUMNS.size() EQ 5 ) {

            m_sayfadaki_toplam_borc   += TABLE_COLUMNS.at(3).toDouble();
            m_sayfadaki_toplam_alacak += TABLE_COLUMNS.at(4).toDouble();

            STRING_LIST.clear();
            STRING_LIST << TABLE_COLUMNS.at(0);
            STRING_LIST << TABLE_COLUMNS.at(1);
            STRING_LIST << TABLE_COLUMNS.at(2);

            STRING_LIST << VIRGUL_EKLE ( QVariant ( ROUND ( QVariant ( TABLE_COLUMNS.at(3)).toDouble())).toString());

            STRING_LIST << VIRGUL_EKLE ( QVariant ( ROUND ( QVariant ( TABLE_COLUMNS.at(4)).toDouble())).toString());

            ADD_ROW_TO_TABLE ( STRING_LIST );
        }

        max_line_counter++;

        if ( max_line_counter EQ m_max_line_count ) {
            break;
        }
        m_son_gonderilen_kayit_id = i+1;
    }
    m_borc              = m_sayfadaki_toplam_borc;
    m_alacak            = m_sayfadaki_toplam_alacak;
    m_toplam_borc      += m_sayfadaki_toplam_borc;
    m_toplam_alacak    += m_sayfadaki_toplam_alacak;

    if ( m_son_gonderilen_kayit_id EQ TABLE_ROWS.size() ) {
        m_fis_bitti_mi = true;
    }

    if ( m_fis_sayisi + 1  EQ M_TUM_FISLER.size() ) {
        m_toplam_borc   = 0.00;
        m_toplam_alacak = 0.00;
    }

    m_son_gonderilen_kayit_id++;
    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   TOPLU_FIS_RAPORU::GET_FOOTER
***************************************************************************************/

QString TOPLU_FIS_RAPORU::GET_FOOTER()
{

    QString         borc_toplami_str    = QVariant(ROUND(m_borc)).toString();
    QString         alacak_toplami_str  = QVariant(ROUND(m_alacak)).toString();
    QStringList     string_list;

    borc_toplami_str            = VIRGUL_EKLE(borc_toplami_str);
    alacak_toplami_str          = VIRGUL_EKLE(alacak_toplami_str);

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:75%;\" BORDER=0>" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 70%"<<"WIDTH = 15% align=RIGHT"<<"WIDTH = 15% align=RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"", 0, 0 );

    string_list << "" << "<b>" + QString("%1").arg(borc_toplami_str)<<"<b>" + QString("%1").arg ( alacak_toplami_str );

    ADD_ROW_TO_TABLE     ( string_list );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   TOPLU_FIS_RAPORU::SET_REPORT_VARIABLES
***************************************************************************************/

void TOPLU_FIS_RAPORU::SET_REPORT_VARIABLES()
{
    SQL_QUERY sql_query ( DB );

    QString where_str = " ";

    if ( m_baslangic_fisi EQ -1 ) { //RAPOR EKRANINDAN CAGIRILMISTIR

        where_str = "fis_tarihi BETWEEN "
                    ":bas_tarihi AND :bts_tarihi";

        sql_query.PREPARE_SELECT("muh_fisler" , "fis_id , fis_tarihi , yevmiye_numarasi,aciklama,"
                                 "fis_no" , where_str);
        sql_query.SET_VALUE(":bas_tarihi" , m_baslangic_tarihi.toString("yyyy.MM.dd"));
        sql_query.SET_VALUE(":bts_tarihi" , m_bitis_tarihi.toString("yyyy.MM.dd"));

    }

    else {
        sql_query.PREPARE_SELECT("muh_fisler" , "fis_id, fis_tarihi, yevmiye_numarasi, "
                                 "aciklama, fis_no" , "fis_id = :fis_id");

        sql_query.SET_VALUE      ( ":fis_id"           , m_baslangic_fisi );
    }
    if ( sql_query.SELECT( "fis_tarihi, fis_no" ) > 0 ) {

        while ( sql_query.NEXT() EQ true ) {

            m_toplam_borc         = 0.00;
            m_toplam_alacak       = 0.00;

            int fis_id            = sql_query.VALUE(0).toInt();

            QString fis_bilgileri = QVariant(fis_id).toString() + "\t" +
                                    QDate::fromString ( sql_query.VALUE(1).toString(), "yyyy.MM.dd" ).toString ( "dd MMMM yyyy dddd" )
                                    + "\t" + sql_query.VALUE(2).toString() + "\t" + sql_query.VALUE(3).toString()
                                    + "\t" + sql_query.VALUE(4).toString();

            M_FIS_BILGILERI.append ( fis_bilgileri );

            BORC_VE_ALACAK_SATIRLARINI_OLUSTUR ( fis_id );

            if ( m_baslangic_fisi NE -1 ) {

                for ( int i = 0; i < M_TUM_FIS_SATIRLARI.size() ; i++ ) {
                    m_body_string.append(M_TUM_FIS_SATIRLARI.at(i) );
                }
                M_TUM_FISLER << m_body_string;
                m_body_string.clear();
            }
            else {

                for ( int i = 0; i < M_BORC_FIS_SATIRLARI.size() ; i++ ) {
                    m_body_string.append ( M_BORC_FIS_SATIRLARI.at(i) );
                }

                for ( int i = 0; i < M_ALACAK_FIS_SATIRLARI.size() ; i++ ) {
                    m_body_string.append ( M_ALACAK_FIS_SATIRLARI.at(i) );
                }
                M_TUM_FISLER << m_body_string;
                m_body_string.clear();
                M_BORC_FIS_SATIRLARI.clear();
                M_ALACAK_FIS_SATIRLARI.clear();
            }
        }
    }
}

/**************************************************************************************
                   TOPLU_FIS_RAPORU::BORC_VE_ALACAK_SATIRLARINI_OLUSTUR
***************************************************************************************/

void TOPLU_FIS_RAPORU::BORC_VE_ALACAK_SATIRLARINI_OLUSTUR ( int fis_id )
{
    SQL_QUERY sql_query ( DB );

    QString         fis_satiri;
    QStringList     MUH_HESAP;
    QStringList     BORC_SATIRLARI_BASILMIS_UST_HESAPLAR;
    QStringList     ALACAK_SATIRLARI_BASILMIS_UST_HESAPLAR;


    sql_query.PREPARE_SELECT ( "muh_fis_satirlari,muh_hesaplar" , "aciklama, borc_tutari, alacak_tutari, parent_id, "
                               "tam_hesap_kodu, hesap_ismi" ,
                               " muh_fis_satirlari.hesap_id = muh_hesaplar.hesap_id AND fis_id = :fis_id" );
    sql_query.SET_VALUE      ( ":fis_id" , fis_id );

    if ( m_baslangic_fisi NE -1 ) {
        if ( sql_query.SELECT("muh_fis_satirlari.order_number ASC") EQ 0 ) {
            return;
        }
    }
    else {
        if ( sql_query.SELECT("muh_hesaplar.tam_hesap_kodu ASC") EQ 0 ) {
            return;
        }
    }



    while ( sql_query.NEXT() EQ true ) {

        QString     aciklama                =   sql_query.VALUE(0).toString();
        double      borc_tutari             =   sql_query.VALUE(1).toDouble();
        double      alacak_tutari           =   sql_query.VALUE(2).toDouble();
        int         parent_id               =   sql_query.VALUE(3).toInt();
        QString     tam_hesap_kodu          =   sql_query.VALUE(4).toString();
        QString     hesap_ismi              =   sql_query.VALUE(5).toString();

        if ( m_baslangic_fisi NE -1 ) {

            fis_satiri.clear();
            fis_satiri.append ( QString ( tam_hesap_kodu  +"\t"+ hesap_ismi + "\t" + aciklama + "\t" +
                                         QVariant ( borc_tutari ).toString() + "\t" + QVariant ( alacak_tutari ).toString()  ) );
            fis_satiri.insert ( 0,"<br>" );

            M_TUM_FIS_SATIRLARI << fis_satiri;
        }
        else {
            if ( alacak_tutari EQ 0 ) {

                fis_satiri.clear();
                fis_satiri.append (  QString ( tam_hesap_kodu  +"\t"+ hesap_ismi + "\t" + aciklama + "\t" +
                                              QVariant  ( borc_tutari ).toString() + "\t" + QVariant ( alacak_tutari).toString()  )  );

                for ( ; ; ) {

                    QString tab_alt_hesap;
                    QString tab_ana_hesap;

                    if ( m_baslangic_fisi NE -1 ) {
                        fis_satiri.insert ( 0,"<br>" );
                        break;
                    }

                    if ( parent_id  EQ 0 ) {
                        fis_satiri.insert ( 0,"<br>" );
                        break;
                    }

                    QString parent_hesap      = MUH_FIND_AND_GET_HESAP_BILGILERI( parent_id, M_MUH_HESAPLAR, false );

                    MUH_HESAP                 = parent_hesap.split ( "\t" );

                    int parent_hesap_seviyesi =  QVariant ( MUH_HESAP.at(4) ).toInt();

                    if ( BORC_SATIRLARI_BASILMIS_UST_HESAPLAR.contains ( QVariant ( parent_id ).toString() ) EQ true ) {

                        for ( int i = 0; i < parent_hesap_seviyesi; i++ ) {
                            tab_alt_hesap.append ( "&nbsp;&nbsp;" );
                        }

                        fis_satiri.insert ( 0,"<br>" + tab_alt_hesap );
                        break;
                    }

                    BORC_SATIRLARI_BASILMIS_UST_HESAPLAR << QVariant ( parent_id ).toString();
                    
                    parent_id =  QVariant( MUH_HESAP.at(3) ).toInt();

                    for ( int i = 0; i < parent_hesap_seviyesi; i++ ) {
                        tab_alt_hesap.append ( "&nbsp;&nbsp;" );
                    }

                    for ( int i = 0; i < parent_hesap_seviyesi - 1; i++ ) {
                        tab_ana_hesap.append ( "&nbsp;&nbsp;" );
                    }

                    fis_satiri.insert ( 0, "<br>" + tab_ana_hesap + MUH_HESAP.at(1) + "\t" + MUH_HESAP.at(2) + "<br>"
                                           + tab_alt_hesap );
                    if ( parent_id EQ 0 ) {
                        break;
                    }
                }
                M_BORC_FIS_SATIRLARI << fis_satiri;
            }
            else {

                fis_satiri.clear();
                fis_satiri.append ( QString ( tam_hesap_kodu  +"\t"+ hesap_ismi + "\t" + aciklama + "\t" +
                                             QVariant ( borc_tutari ).toString()+ "\t" + QVariant ( alacak_tutari ).toString()  ) );

                for ( ; ; ) {

                    QString tab_alt_hesap;
                    QString tab_ana_hesap;

                    if ( m_baslangic_fisi NE -1 ) {
                        fis_satiri.insert ( 0,"<br>" );
                        break;
                    }

                    if ( parent_id  EQ 0 ) {
                        fis_satiri.insert ( 0,"<br>" );
                        break;
                    }

                    QString parent_hesap      = MUH_FIND_AND_GET_HESAP_BILGILERI( parent_id, M_MUH_HESAPLAR, false );

                    MUH_HESAP                 = parent_hesap.split ( "\t" );

                    int parent_hesap_seviyesi =  QVariant ( MUH_HESAP.at(4) ).toInt();

                    if ( ALACAK_SATIRLARI_BASILMIS_UST_HESAPLAR.contains ( QVariant ( parent_id ).toString() ) EQ true ) {
                        for ( int i = 0; i < parent_hesap_seviyesi; i++ ) {
                            tab_alt_hesap.append ( "&nbsp;&nbsp;" );
                        }
                        fis_satiri.insert ( 0,"<br>" + tab_alt_hesap );
                        break;
                    }

                    ALACAK_SATIRLARI_BASILMIS_UST_HESAPLAR << QVariant ( parent_id ).toString();

                    parent_id =  QVariant ( MUH_HESAP.at(3) ).toInt();

                    for ( int i = 0; i < parent_hesap_seviyesi; i++ ) {
                        tab_alt_hesap.append ( "&nbsp;&nbsp;" );
                    }

                    for ( int i = 0; i < parent_hesap_seviyesi - 1; i++ ) {
                        tab_ana_hesap.append ( "&nbsp;&nbsp;" );
                    }

                    fis_satiri.insert ( 0, "<br>" + tab_ana_hesap + MUH_HESAP.at(1) + "\t" + MUH_HESAP.at(2) + "<br>"
                                           + tab_alt_hesap );
                    if ( parent_id EQ 0 ) {
                        break;
                    }
                }
                M_ALACAK_FIS_SATIRLARI << fis_satiri;
            }
        }
    }
}
