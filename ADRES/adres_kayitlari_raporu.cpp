#include <QDate>
#include "adak_sql.h"
#include "adak_utils.h"
#include "adres_kayitlari_raporu_class.h"
#include "adres_kayitlari_raporu_open.h"
#include "adres_enum.h"
#include "yonetim.h"
#include "e9_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_ADRES_KAYITLARI_RAPORU
***************************************************************************************/

QString OPEN_ADRES_KAYITLARI_RAPORU ( int rapor_turu,int p_adres_kayit_turu, QStringList P_RAPOR_VERILERI )
{
    ADRES_KAYITLARI_RAPORU  *  R = new ADRES_KAYITLARI_RAPORU ( rapor_turu,p_adres_kayit_turu, P_RAPOR_VERILERI );
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                    ADRES_KAYITLARI_RAPORU::ADRES_KAYITLARI_RAPORU
***************************************************************************************/

ADRES_KAYITLARI_RAPORU::ADRES_KAYITLARI_RAPORU ( int rapor_turu , int p_adres_kayit_turu, QStringList P_RAPOR_VERILERI ) : REPORT_KERNEL ("ADRES_KAYITLARI_RAPORU")
{
    SET_HEIGHTS              ( 30, 30 );
    m_max_line_count       = GET_MAX_LINE_COUNT (12);
    m_rapor_turu           = rapor_turu;
    m_sayfa_numarasi       = 0;
    m_satir_index          = 0;
    M_RAPOR_VERILERI       = P_RAPOR_VERILERI;
    m_adres_kayit_turu     = p_adres_kayit_turu;
}


/**************************************************************************************
                   ADRES_KAYITLARI_RAPORU::GET_HEADER
***************************************************************************************/

QString ADRES_KAYITLARI_RAPORU::GET_HEADER()
{
    QString header_string;

    QStringList string_list;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER = 0 >" );

    SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 25%"<<"WIDTH = 50% ALIGN = CENTER"<<"WIDTH = 25% ALIGN = RIGHT" );

    CREATE_TABLE ( QStringList()<<""<<""<<"",0,0) ;

    QString baslik;

    switch ( m_adres_kayit_turu ) {

        case SAHIS_KAYDI :
            baslik = QObject::tr ( "ŞAHIS KAYITLARI" );
            break;
        case FIRMA_KAYDI :
            baslik = QObject::tr ( "FİRMA KAYITLARI" );
            break;
        case SAHIS_ARTI_FIRMA :
            baslik = QObject::tr ( "ŞAHIS VE FİRMA KAYITLARI" );
            break;
        default :
            break;
    }

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

    if ( m_rapor_turu EQ ADRES_BILGILERI_RAPORU ) {
        SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 25% ALIGN=CENTER"<<"WIDTH = 75% ALIGN=CENTER" );

        CREATE_TABLE ( QStringList()<<"Kayıt Adı"<<"Adresi",0,0 );
    }
    else {
        SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 20% ALIGN=CENTER"
                                         <<"WIDTH = 20% ALIGN=CENTER"
                                         <<"WIDTH = 15% ALIGN=CENTER"
                                         <<"WIDTH = 15% ALIGN=CENTER"
                                         <<"WIDTH = 15% ALIGN=CENTER"
                                         <<"WIDTH = 15% ALIGN=CENTER");

        CREATE_TABLE ( QStringList()<<"Kayıt Adı"<<"Web Adresi"<<"E-Mail"<<"Telefon - 1"
                      << "Telefon - 2" << "Cep Telefonu",0,0 );
    }

    header_string.append ( GET_TABLE_HTML_STRING() ) ;

    return header_string;
}

/**************************************************************************************
                   ADRES_KAYITLARI_RAPORU::GET_BODY
***************************************************************************************/

QString ADRES_KAYITLARI_RAPORU::GET_BODY()
{
    if ( M_RAPOR_VERILERI.size() EQ 0 ) {
        return NULL;
    }

    int eklenen_satir_sayisi = 0;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:75%;\" BORDER=0 >" );

    if ( m_rapor_turu EQ ADRES_BILGILERI_RAPORU ) {
        SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 25% ALIGN=CENTER"<<"WIDTH = 75% ALIGN=CENTER" );

        CREATE_TABLE ( QStringList()<<""<<"", 0, 0 );

        for ( int i = m_satir_index; i < M_RAPOR_VERILERI.size(); i++ ) {

            QStringList SATIR_BILGILERI;

            QStringList sahis_bilgileri = M_RAPOR_VERILERI.at(i).split ( "\t" );

            SATIR_BILGILERI << sahis_bilgileri.at(0);
            SATIR_BILGILERI << sahis_bilgileri.at(1) + " " + sahis_bilgileri.at(2) + " " + sahis_bilgileri.at(3) + "   " +
                               sahis_bilgileri.at(4) + " " + sahis_bilgileri.at(5) + "-" + sahis_bilgileri.at(6);

            ADD_ROW_TO_TABLE ( SATIR_BILGILERI );

            eklenen_satir_sayisi++;
            m_satir_index++;

            if ( eklenen_satir_sayisi > m_max_line_count - 1 ) {
                return GET_TABLE_HTML_STRING();
            }

            if ( m_satir_index EQ M_RAPOR_VERILERI.size() ) {
                return GET_TABLE_HTML_STRING();
            }
        }
    }
    else {
        SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 20% ALIGN=CENTER"
                                         <<"WIDTH = 20% ALIGN=CENTER"
                                         <<"WIDTH = 15% ALIGN=CENTER"
                                         <<"WIDTH = 15% ALIGN=CENTER"
                                         <<"WIDTH = 15% ALIGN=CENTER"
                                         <<"WIDTH = 15% ALIGN=CENTER");

        CREATE_TABLE ( QStringList()<<""<<""<<""<<""<<""<<"", 0, 0 );

        for ( int i = m_satir_index; i < M_RAPOR_VERILERI.size(); i++ ) {

            QStringList SATIR_BILGILERI = M_RAPOR_VERILERI.at(i).split ( "\t" );

            ADD_ROW_TO_TABLE ( SATIR_BILGILERI );

            eklenen_satir_sayisi++;
            m_satir_index++;

            if ( eklenen_satir_sayisi > m_max_line_count - 1 ) {
                return GET_TABLE_HTML_STRING();
            }

            if ( m_satir_index EQ M_RAPOR_VERILERI.size() ) {
                return GET_TABLE_HTML_STRING();
            }
        }
    }



    return NULL;

}

/**************************************************************************************
                   ADRES_KAYITLARI_RAPORU::GET_FOOTER
***************************************************************************************/

QString ADRES_KAYITLARI_RAPORU::GET_FOOTER()
{
    return NULL;
}
