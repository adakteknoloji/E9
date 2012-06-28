#include <QDate>
#include "adak_utils.h"
#include "faiz_taksit_raporu_class.h"
#include "yonetim.h"
#include "faiz_enum.h"
#include "e9_console_utils.h"

extern ADAK_SQL * DB;

/*****************************************************************/
/*                  F_FAIZ_TAKSIT_RAPORU()                       */
/*****************************************************************/

QString F_FAIZ_TAKSIT_RAPORU (double anapara,double aylik_faiz_orani,double toplam_tutar,QList<int> odeme_gunleri,QList<double> odeme_tutarlari, QList< QString > odeme_tarihleri, QString page_title , int p_gunluk_aylik)
{
    FAIZ_TAKSIT_RAPORU  *  R = new FAIZ_TAKSIT_RAPORU(anapara,aylik_faiz_orani,toplam_tutar,odeme_gunleri,odeme_tutarlari, odeme_tarihleri, page_title , p_gunluk_aylik );
    return R->CREATE_HTML_PAGES();
}

/*****************************************************************/
/*     FAIZ_TAKSIT_RAPORU::FAIZ_TAKSIT_RAPORU                    */
/*****************************************************************/

FAIZ_TAKSIT_RAPORU::FAIZ_TAKSIT_RAPORU(double anapara,double aylik_faiz_orani,double toplam_tutar,QList<int> odeme_gunleri,QList<double> odeme_tutarlari,QList< QString > odeme_tarihleri, QString page_title , int p_gunluk_aylik) : REPORT_KERNEL ("FAIZ_TAKSIT_RAPORU")
{
    SET_HEIGHTS (30,0);


    text_gunluk_aylik = "";
    if( p_gunluk_aylik EQ  AYLIK_FAIZ )
    {
        text_gunluk_aylik = " Ay Sonra";
    }
    else if( p_gunluk_aylik EQ GUNLUK_FAIZ )
    {
        text_gunluk_aylik = " Gün Sonra";
    }

    p_page_title            = page_title;
    p_anapara               = anapara;
    p_aylik_faiz_orani      = aylik_faiz_orani;
    p_toplam_tutar          = toplam_tutar;
    p_taksit_sayisi         = odeme_gunleri.size();
    p_odeme_gunleri         = odeme_gunleri;
    p_odeme_tutarlari       = odeme_tutarlari;

    p_odeme_tarihleri       = odeme_tarihleri;

    p_max_line_count        = GET_MAX_LINE_COUNT (15);
    p_eklenen_kayit_sayisi  = 0;
    p_raporu_bitir          = false;


}

/*****************************************************************/
/*              FAIZ_TAKSIT_RAPORU::GET_HEADER                   */
/*****************************************************************/

QString FAIZ_TAKSIT_RAPORU::GET_HEADER()
{
    QStringList row;
    QString header = "";

    SET_TABLE_TAG_STRING("<table style=\"width:100%; font-size:75%;\">");
    SET_TD_TAG_STRING(QStringList()<<"style=\" width:30%; text-align:left;\""<<"style=\"text-align:centered;font-size:100%; font-weight:bold;\""<<"style=\" width:30%; text-align:right;\"");
    CREATE_TABLE (QStringList()<< E9_GET_FIRMA_ISMI() <<"<center>"+ p_page_title +"</center>"<< GET_REPORT_TOP_RIGHT_HEADER() ,0,19);

    ADD_ROW_TO_TABLE(QStringList() << "" << "" << "");
    ADD_ROW_TO_TABLE(QStringList() << "" << "" << "");

    header = GET_TABLE_HTML_STRING();

    SET_TABLE_TAG_STRING("<table style=\"width:100%; font-size:75%;\">");
    SET_TD_TAG_STRING(QStringList()<<"style=\"color:black; font-weight:bold;\""<<"style=\"color:black; font-weight:bold;\""
                                   <<"style=\"color:black;font-weight:bold;\""<<"style=\" text-align:right;color:black; font-weight:bold;\"");
    
    CREATE_TABLE (QStringList()<<"Taksit Sayısı"<<"Anapara"<<"Aylık Faiz Oranı"<<"Toplam Tutar",0,0);


    row << QVariant(p_taksit_sayisi).toString() << VIRGUL_EKLE(QVariant(ROUND(p_anapara)).toString())
        << VIRGUL_EKLE(QVariant(ROUND(p_aylik_faiz_orani)).toString()) << VIRGUL_EKLE(QVariant(ROUND(p_toplam_tutar)).toString());

    ADD_ROW_TO_TABLE(row);

    header.append(GET_TABLE_HTML_STRING());

    return header;
}

/*****************************************************************/
/*              FAIZ_TAKSIT_RAPORU::GET_BODY                     */
/*****************************************************************/

QString FAIZ_TAKSIT_RAPORU::GET_BODY()
{
    if (p_raporu_bitir EQ true) {
        return NULL;
    }

    int column_two_added = 0;
    int column_three_added = 0;
    int column_four_added = 0;

    SET_TABLE_TAG_STRING("<table border=1 style=\"width:100%; font-size:85%;\">");
    SET_TD_TAG_STRING(QStringList()<<"style=\"width:15%; color:black;\""<<"style=\"color:black; text-align:left;\""<<"style=\"width:15%; color:black; text-align:right;\""<<"style=\"width:15%; color:black;\""<<"style=\"color:black; text-align:left;\""<<"style=\"width:5%; color:black; text-align:right;\""<<"style=\"width:15%; color:black;\""<<"style=\"color:black; text-align:left;\""<<"style=\"width:15%; color:black; text-align:right;\""<<"style=\"width:15%; color:black;\""<<"style=\"color:black; text-align:left;\""<<"style=\"width:15%; color:black; text-align:right;\"");

    QStringList create_table_string;
    create_table_string <<"Taksit No"<< "Ödeme Tarihi"<<"Ödeme Tutarı";


    if (p_eklenen_kayit_sayisi + p_max_line_count < p_taksit_sayisi) {
        create_table_string << " "<<"Taksit No"<< "Ödeme Tarihi"<<"Ödeme Tutarı";
    }
    if (p_eklenen_kayit_sayisi + (p_max_line_count * 2) < p_taksit_sayisi) {
        create_table_string << " "<<"Taksit No"<< "Ödeme Tarihi"<<"Ödeme Tutarı";
    }

    if (p_eklenen_kayit_sayisi + (p_max_line_count * 3) < p_taksit_sayisi) {
        create_table_string << " "<<"Taksit No"<< "Ödeme Tarihi"<<"Ödeme Tutarı";
    }

    CREATE_TABLE (create_table_string,0,16);

    for (int i = 0 ; i < p_max_line_count ; i++) {
        QStringList row;

        if ( p_odeme_gunleri.size() <= p_eklenen_kayit_sayisi) {
            break;
        }

        row << QString::number( i + 1 ) + " : ( " + QVariant(p_odeme_gunleri.at(p_eklenen_kayit_sayisi)).toString() + text_gunluk_aylik + " )" << p_odeme_tarihleri.at( p_eklenen_kayit_sayisi ) << VIRGUL_EKLE( QVariant(ROUND(p_odeme_tutarlari.at(p_eklenen_kayit_sayisi))).toString() );
        row << "" ;

        if (p_eklenen_kayit_sayisi + p_max_line_count < p_taksit_sayisi) {
            int index = p_eklenen_kayit_sayisi + p_max_line_count;
            row << QString::number( i + 1 ) + " : ( " + QVariant(p_odeme_gunleri.at(index)).toString() + text_gunluk_aylik + " )"<< p_odeme_tarihleri.at( index ) << VIRGUL_EKLE( QVariant(ROUND(p_odeme_tutarlari.at(p_eklenen_kayit_sayisi))).toString() );
            column_two_added ++;
            row << "" ;
        }
        if (p_eklenen_kayit_sayisi + (p_max_line_count * 2) < p_taksit_sayisi) {
            int index = p_eklenen_kayit_sayisi + (p_max_line_count * 2);
            row << QString::number( i + 1 ) + " : ( " + QVariant(p_odeme_gunleri.at(index)).toString() + text_gunluk_aylik + " )" << p_odeme_tarihleri.at( index ) << VIRGUL_EKLE(QVariant(ROUND(p_odeme_tutarlari.at(p_eklenen_kayit_sayisi))).toString() );
            column_three_added ++;
            row << "" ;
        }

        if (p_eklenen_kayit_sayisi + (p_max_line_count * 3) < p_taksit_sayisi) {
            int index = p_eklenen_kayit_sayisi + (p_max_line_count * 3);
            row << QString::number( i + 1 ) + " : ( " + QVariant(p_odeme_gunleri.at(index)).toString() + text_gunluk_aylik + " )" << p_odeme_tarihleri.at( index ) << VIRGUL_EKLE(QVariant(ROUND(p_odeme_tutarlari.at(p_eklenen_kayit_sayisi))).toString() );
            column_four_added ++;
            row << "" ;
        }

        row << "" << "" << ""<< "" << "" << ""<< "" << "" << "";

        ADD_ROW_TO_TABLE(row);

        p_eklenen_kayit_sayisi++;
    }

    p_eklenen_kayit_sayisi += (column_two_added + column_three_added + column_four_added);

    if (p_eklenen_kayit_sayisi EQ p_taksit_sayisi ) {
        p_raporu_bitir = true;
    }

    return GET_TABLE_HTML_STRING();
}

/*****************************************************************/
/*                  FAIZ_TAKSIT_RAPORU::GET_FOOTER               */
/*****************************************************************/

QString FAIZ_TAKSIT_RAPORU::GET_FOOTER()
{
    return "";
}
