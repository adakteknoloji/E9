#include <QDate>
#include <QStringList>
#include "adak_sql.h"
#include "adak_utils.h"
#include "isl_enum.h"
#include "isl_fis_yazdir_class.h"
#include "isl_fis_yazdir_open.h"
#include "yonetim.h"
#include "isl_console_utils.h"
#include "isl_enum.h"
#include "e9_console_utils.h"
#include "isl_gui_utils.h"

extern ADAK_SQL   *    DB;

/**************************************************************************************
                   GET_ISL_FISI_RAPORU
***************************************************************************************/

QString GET_ISL_FISI_RAPORU ( int fis_id, int fis_turu )
{
    ISL_FIS_YAZDIR  *  R = new ISL_FIS_YAZDIR ( fis_id, fis_turu );
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   ISL_FIS_YAZDIR::ISL_FIS_YAZDIR
***************************************************************************************/

ISL_FIS_YAZDIR::ISL_FIS_YAZDIR ( int fis_id, int fis_turu ) : REPORT_KERNEL ("ISL_FIS_YAZDIR")
{
    SET_HEIGHTS ( 40, 30 );
    p_fis_id                  =  fis_id;
    p_fis_turu                =  fis_turu;
    if ( FIS_BILGILERINI_AL() EQ false ) {
        return;
    }
    if ( FIS_SATIRLARINI_AL() EQ false ) {
        return;
    }
    p_max_line_count            =  GET_MAX_LINE_COUNT (14);
    p_yazdirilan_satir_numarasi = 0;
    p_yazdirilan_sayfa_sayisi   = 1;
    p_kdv_haric_tutar           = 0.00;
    p_kdv_tutari                = 0.00;
    p_toplam_tutar              = 0.00;
    p_rapor_bitti_mi            = false;
}

/**************************************************************************************
                   ISL_FIS_YAZDIR::GET_HEADER
***************************************************************************************/

QString ISL_FIS_YAZDIR::GET_HEADER()
{

    QString header_html_string;

    QStringList FIS_BILGILERI      =  p_fis_bilgileri.split ( "\t" );
    QString     fis_no             = "Fiş No       : "      + QVariant ( p_fis_id ).toString();
    QString     fis_tarihi         = "Fiş Tarihi   : "      + FIS_BILGILERI.at(0);
    QString     fis_turu           = "Fiş Türü     : "      + FIS_BILGILERI.at(1);
    QString     aciklama           =  FIS_BILGILERI.at(2);

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% BORDER=0 style=\"font-size:75%;\">" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 50%"<<"WIDTH = 50% ALIGN = RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<"", 0, 0 );

    ADD_ROW_TO_TABLE     ( QStringList()<< E9_GET_FIRMA_ISMI() << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList()<<fis_no<<fis_tarihi ) ;

    ADD_ROW_TO_TABLE     ( QStringList()<<fis_turu<<"" );

    header_html_string.append ( GET_TABLE_HTML_STRING() );

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% BORDER=0 style = font-size:75%;>" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 8%"<<"WIDTH = 90%" );

    CREATE_TABLE         ( QStringList()<<""<<"", 0, 0 );

    ADD_ROW_TO_TABLE     ( QStringList() << "Açıklama     :" << aciklama );

    header_html_string.append ( GET_TABLE_HTML_STRING() );

    return header_html_string;
}

/**************************************************************************************
                    ISL_FIS_YAZDIR::GET_BODY
***************************************************************************************/

QString ISL_FIS_YAZDIR::GET_BODY()
{
    if ( p_yazdirilan_satir_numarasi EQ P_FIS_SATIRLARI.size() ) {
        return NULL;
    }
    QString body_html_string;
    QStringList SATIR_BILGILERI;
    p_yazdirilan_satir_sayisi   = 0;

    body_html_string.clear();

    if ( p_yazdirilan_sayfa_sayisi > 1 ) {
        SET_TABLE_TAG_STRING ( "<table WIDTH = 100% style=\"font-size:75%;\" BORDER = 0" );
        SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 56%"<<"WIDTH = 18% align = RIGHT"<<"WIDTH = 13% align = RIGHT"
                                         <<"WIDTH = 13% align = RIGHT" );

        CREATE_TABLE         ( QStringList()<<"<b><div ALIGN = RIGHT> DEVREDEN TOPLAM </div></b>"
                                            <<"<b><div ALIGN = RIGHT>" + VIRGUL_EKLE ( QVariant ( p_kdv_haric_tutar ).toString(),2 ) + "</div></b>"
                                            <<"<b><div ALIGN = RIGHT>" + VIRGUL_EKLE ( QVariant ( p_kdv_tutari ).toString(),2 ) + "</div></b>"
                                            <<"<b><div ALIGN = RIGHT>" + VIRGUL_EKLE ( QVariant ( p_toplam_tutar ).toString(),2 ) + "</div></b>",0,0 );
        body_html_string.append ( GET_TABLE_HTML_STRING() ) ;
    }

    switch ( p_fis_turu ) {
        case ENUM_GELIR_FISI :
        {
            SET_TABLE_TAG_STRING ( "<table WIDTH = 100% style=\"font-size:75%;\" BORDER = 1" );
            SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 10%"<<"WIDTH = 32%"<<"WIDTH = 13% align = RIGHT"
                                                <<"WIDTH = 18% ALIGN = RIGHT"<<"WIDTH = 13% ALIGN = RIGHT"<<"WIDTH = 13% ALIGN = RIGHT");
            CREATE_TABLE         ( QStringList()<<"<b>Belge No</b>"<<"<b>Açıklama</b>"<<"<b><div ALIGN = RIGHT>KDV Oranı</div></b>"
                                                <<"<b><div ALIGN = RIGHT>KDV Hariç Tutar</div></b>"<<"<b><div ALIGN = RIGHT>KDV Tutarı</div></b>"
                                                <<"<b><div ALIGN = RIGHT>Toplam Tutar</div></b>", 0, 0 );
            break;
        }
        case ENUM_GIDER_FISI :
        default         :
        {
            SET_TABLE_TAG_STRING ( "<table WIDTH = 100% style=\"font-size:75%;\" BORDER = 1" );
            SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 10%"<<"WIDTH = 30%"<<"WIDTH = 8%"<<"WIDTH = 8% align = RIGHT"
                                                <<"WIDTH = 18% ALIGN = RIGHT"<<"WIDTH = 13% ALIGN = RIGHT"<<"WIDTH = 13% ALIGN = RIGHT" );
            CREATE_TABLE         ( QStringList () <<"<b>Belge No</b>"<<"<b>Açıklama</b>"<<"<b>Gider Türü</b>"<<"<b><div ALIGN = RIGHT>KDV Oranı</div></b>"
                                                  <<"<b><div ALIGN = RIGHT>KDV Hariç Tutar</div></b>"<<"<b><div ALIGN = RIGHT>KDV Tutarı</div></b>"
                                                  <<"<b><div ALIGN = RIGHT>Toplam Tutar</div></b>", 0, 0  );
            break;
        }
    }


    for ( ; ; ) {

        SATIR_BILGILERI               = QVariant ( P_FIS_SATIRLARI.at ( p_yazdirilan_satir_numarasi ) ).toString().split ( "\t" );
        QString belge_numarasi        = SATIR_BILGILERI.at(0);
        QString aciklama              = SATIR_BILGILERI.at(1);
        QString gider_turu            = SATIR_BILGILERI.at(2);
        QString kdv_orani             = SATIR_BILGILERI.at(3);
        QString satir_kdv_haric_tutar = VIRGUL_EKLE(SATIR_BILGILERI.at(4),2);
        QString satir_kdv_tutari      = VIRGUL_EKLE(SATIR_BILGILERI.at(5),2);
        QString satir_kdv_dahil_tutar    = VIRGUL_EKLE(SATIR_BILGILERI.at(6),2);

        p_kdv_haric_tutar             += ROUND(QVariant(SATIR_BILGILERI.at(4)).toDouble());
        p_kdv_tutari                  += ROUND(QVariant(SATIR_BILGILERI.at(5)).toDouble());
        p_toplam_tutar                += ROUND(QVariant(SATIR_BILGILERI.at(6)).toDouble());

        switch ( p_fis_turu ) {
            case ENUM_GELIR_FISI :
            {
                ADD_ROW_TO_TABLE ( QStringList()<<belge_numarasi<<aciklama<<kdv_orani<<satir_kdv_haric_tutar
                                                <<satir_kdv_tutari<<satir_kdv_dahil_tutar );
                break;
            }
            case ENUM_GIDER_FISI :
            default         :
            {
                ADD_ROW_TO_TABLE ( QStringList()<<belge_numarasi<<aciklama<<gider_turu<<kdv_orani<<satir_kdv_haric_tutar
                                                <<satir_kdv_tutari<<satir_kdv_dahil_tutar );
                break;
            }
        }


        p_yazdirilan_satir_sayisi++;
        p_yazdirilan_satir_numarasi++;
        if ( p_yazdirilan_satir_sayisi EQ p_max_line_count ) {
            p_yazdirilan_satir_sayisi = 0;
            p_yazdirilan_sayfa_sayisi++;
            break;
        }
        if ( p_yazdirilan_satir_numarasi EQ P_FIS_SATIRLARI.size() ) {
            body_html_string.append ( GET_TABLE_HTML_STRING() );
            p_rapor_bitti_mi = true;
            return body_html_string;
        }
    }
    body_html_string.append ( GET_TABLE_HTML_STRING() );
    return body_html_string;
}

/**************************************************************************************
                    ISL_FIS_YAZDIR::GET_FOOTER
***************************************************************************************/

QString ISL_FIS_YAZDIR::GET_FOOTER()
{
    QString footer_html_string;
    QString sayfa_sonu_basligi =  "SAYFA TOPLAMI";
    if ( p_rapor_bitti_mi EQ true ) {
            sayfa_sonu_basligi =  "TOPLAMLAR";
    }

    SET_TABLE_TAG_STRING ( "<table WIDTH = 100% style=\"font-size:75%;\" BORDER = 0" );

    switch(p_fis_turu ) {
        case ENUM_GELIR_FISI :
        {
            SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 56%"<<"WIDTH = 18% align = RIGHT"<<"WIDTH = 13% align = RIGHT"
                                             <<"WIDTH = 13% align = RIGHT" );
            break;
        }
        case ENUM_GIDER_FISI :
        default         :
        {
            SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 56%"<<"WIDTH = 18% align = RIGHT"<<"WIDTH = 13% align = RIGHT"
                                             <<"WIDTH = 13% align = RIGHT" );
            break;
        }
    }

    CREATE_TABLE ( QStringList()<<"<b><div ALIGN = RIGHT> " + sayfa_sonu_basligi +"</div></b>"
                                <<"<b><div ALIGN = RIGHT>" + VIRGUL_EKLE ( QVariant ( p_kdv_haric_tutar ).toString(),2 ) + "</div></b>"
                                <<"<b><div ALIGN = RIGHT>" + VIRGUL_EKLE ( QVariant ( p_kdv_tutari ).toString(),2 ) + "</div></b>"
                                <<"<b><div ALIGN = RIGHT>" + VIRGUL_EKLE ( QVariant ( p_toplam_tutar ).toString(),2 ) + "</div></b>",0,0);


    footer_html_string.append ( GET_TABLE_HTML_STRING() );

    return footer_html_string;
}

/**************************************************************************************
                    ISL_FIS_YAZDIR::FIS_BILGILERINI_AL
***************************************************************************************/

bool ISL_FIS_YAZDIR::FIS_BILGILERINI_AL ()
{
    p_fis_bilgileri.clear();

    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "isl_fisi","fis_tarihi, fis_turu, aciklama, kdv_haric_toplam, kdv_toplam, kdv_dahil_toplam ",
                               "fis_id = :fis_id" );
    sql_query.SET_VALUE      ( ":fis_id", p_fis_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return false;
    }
    sql_query.NEXT();
    QString fis_tarihi = sql_query.VALUE(0).toDate().toString ( "dd MMMM yyyy dddd" );
    p_fis_turu         = sql_query.VALUE(1).toInt();
    QString fis_turu   = ISL_GET_FIS_TURU_STRING( p_fis_turu );
    QString aciklama   = sql_query.VALUE(2).toString();
    p_kdv_haric_tutar  = ROUND ( sql_query.VALUE(3).toDouble() );
    p_kdv_tutari       = ROUND ( sql_query.VALUE(4).toDouble() );
    p_toplam_tutar     = ROUND ( sql_query.VALUE(5).toDouble() );;

    p_fis_bilgileri.append ( fis_tarihi + "\t" + fis_turu + "\t" + aciklama + "\t" + QVariant ( p_kdv_haric_tutar ).toString() +
                            "\t" + QVariant ( p_kdv_tutari ).toString() + "\t" + QVariant ( p_toplam_tutar ).toString() );
    return true;
}

/**************************************************************************************
                    ISL_FIS_YAZDIR::FIS_SATIRLARINI_AL
***************************************************************************************/

bool ISL_FIS_YAZDIR::FIS_SATIRLARINI_AL ()
{
    SQL_QUERY sql_query ( DB );
    QString   fis_satiri;
    P_FIS_SATIRLARI.clear();
    sql_query.PREPARE_SELECT ( "isl_fis_satirlari","belge_no, aciklama, gider_turu, kdv_orani, satir_kdv_haric_tutar, satir_kdv_tutari, "
                               "satir_kdv_dahil_tutar","fis_id = :fis_id" );
    sql_query.SET_VALUE      (  ":fis_id", p_fis_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return false;
    }
    while ( sql_query.NEXT() EQ true ) {
        QString belge_no              = sql_query.VALUE(0).toString();
        QString aciklama              = sql_query.VALUE(1).toString();
        QString gider_turu            = ISL_GET_GIDER_STRING(sql_query.VALUE(2).toInt());
        QString kdv_orani             = "%" + sql_query.VALUE(3).toString();
        QString satir_kdv_haric_tutar = sql_query.VALUE(4).toString();
        QString satir_kdv_tutari      = sql_query.VALUE(5).toString();
        QString satir_kdv_dahil_tutar    = sql_query.VALUE(6).toString();
        fis_satiri.clear();
        fis_satiri.append ( belge_no + "\t" + aciklama + "\t" + gider_turu + "\t" + kdv_orani + "\t" + satir_kdv_haric_tutar +
                            "\t" + satir_kdv_tutari + "\t" + satir_kdv_dahil_tutar );
        P_FIS_SATIRLARI.append ( fis_satiri );
    }
    return true;
}
