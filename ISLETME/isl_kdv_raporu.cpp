#include <QStringList>
#include <adak_sql.h>
#include <adak_utils.h>
#include "isl_struct.h"
#include "isl_kdv_raporu_class.h"
#include "isl_kdv_raporu_open.h"
#include "yonetim.h"
#include "isl_enum.h"
#include "e9_console_utils.h"

extern ADAK_SQL *  DB;

/**************************************************************************************
                   GET_ISL_ISL_KDV_RAPORU
***************************************************************************************/

QString GET_ISL_KDV_RAPORU ( ISL_RAPOR_VARS * P_ISL_RV)
{
    ISL_KDV_RAPORU  * R = new ISL_KDV_RAPORU(P_ISL_RV);
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   ISL_KDV_RAPORU::ISL_KDV_RAPORU
***************************************************************************************/

ISL_KDV_RAPORU::ISL_KDV_RAPORU(ISL_RAPOR_VARS * P_ISL_RV) : REPORT_KERNEL ("ISL_KDV_RAPORU")
{
    SET_HEIGHTS ( 55, 20 );
    m_max_line_count  =  GET_MAX_LINE_COUNT (10);
    M_GELIR_FISI_KDV_LISTESI.clear();
    M_GIDER_FISI_KDV_LISTESI.clear();
    m_gelir_fisi_kdv_orani_sayisi  = 0;
    m_gider_fisi_kdv_orani_sayisi  = 0;
    m_gider_fisi_kdv_haric_toplam  = 0.00;
    m_gider_fisi_kdv_tutari_toplam = 0.00;
    m_gider_fisi_kdv_dahil_toplam  = 0.00;
    m_gelir_fisi_kdv_haric_toplam  = 0.00;
    m_gelir_fisi_kdv_tutari_toplam = 0.00;
    m_gelir_fisi_kdv_dahil_toplam  = 0.00;
    m_rapor_bitti_mi               = false;

    M_ISL_RV  = P_ISL_RV;

    M_GELIR_FISI_TUTARLAR_LISTESI  = new TUTAR_TOPLAMLARI [ 10 ];
    M_GIDER_FISI_TUTARLAR_LISTESI  = new TUTAR_TOPLAMLARI [ 10 ];
    m_kayit_var_mi                 = FIS_SATIRLARINI_KDV_ORANLARINA_GORE_AYIR();
}

/**************************************************************************************
                   ISL_KDV_RAPORU::GET_HEADER
***************************************************************************************/

QString ISL_KDV_RAPORU::GET_HEADER()
{
    if ( m_kayit_var_mi EQ false ) {
        return NULL;
    }
    QString header_string;


    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 0>" );

    SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:75%;\" ALIGN = LEFT"<<"style=\"width:25%;\" ALIGN = RIGHT" );

    CREATE_TABLE         ( QStringList()<< E9_GET_FIRMA_ISMI() << GET_REPORT_TOP_RIGHT_HEADER(), 0, 0 );

    ADD_ROW_TO_TABLE     ( QStringList()<<"&nbsp"<<"" );

    header_string.append ( GET_TABLE_HTML_STRING() );

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 0>" );

    SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:100%;\" ALIGN = CENTER ") ;

    CREATE_TABLE         ( QStringList()<< M_ISL_RV->baslangic_tarihi.toString ( "dd.MMMM.yyyy dddd" ) + " - " +
                                          M_ISL_RV->bitis_tarihi.toString ( "dd.MMMM.yyyy dddd" ),0,0 ) ;

    ADD_ROW_TO_TABLE     (QStringList()<<"&nbsp");

    ADD_ROW_TO_TABLE     ( QStringList()<<"<b>KDV RAPORU </b>" );

    ADD_ROW_TO_TABLE     ( QStringList()<<"&nbsp" );

    header_string.append ( GET_TABLE_HTML_STRING() );

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 1>" );

    SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:50%;\" ALIGN = CENTER"<<"style=\"width:50%;\" ALIGN = CENTER" );

    CREATE_TABLE         ( QStringList()<<"<b>GİDER</b>"<<"<b>GELİR</b>", 0, 0 );

    header_string.append ( GET_TABLE_HTML_STRING() );

    return header_string;
}

/**************************************************************************************
                   ISL_KDV_RAPORU::GET_BODY
***************************************************************************************/

QString ISL_KDV_RAPORU::GET_BODY()
{

    if ( m_kayit_var_mi EQ false ) {
        return NULL;
    }
    if ( m_rapor_bitti_mi EQ true ) {
        return NULL;
    }
    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER=1 >" );
    SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:8%;\"  ALIGN = CENTER"<<"style=\"width:14%;\" ALIGN = RIGHT"
                                        <<"style=\"width:13%;\" ALIGN = RIGHT" <<"style=\"width:15%;\" ALIGN = RIGHT"
                                        <<"style=\"width:8%;\"  ALIGN = CENTER"<<"style=\"width:14%;\" ALIGN = RIGHT"
                                        <<"style=\"width:13%;\" ALIGN = RIGHT" <<"style=\"width:15%;\" ALIGN = RIGHT" );
    CREATE_TABLE ( QStringList()<<"<b>KDV Ornı</b>"
                                <<"<b>KDV Hrç Tutar</b>"<<"<b>KDV Tutarı</b>"<<"<b>KDV Dahil Tutar</b>"
                                <<"<b>KDV Ornı</b>"
                                <<"<b>KDV Hrç Tutar</b>"<<"<b>KDV Tutarı</b>"<<"<b>KDV Dahil Tutar</b>",0,0 );


    int gider_fisi_kdv_listesi_index = 0;
    int gelir_fisi_kdv_listesi_index = 0;

    QString  gider_fisi_kdv_orani;
    QString  gider_fisi_kdv_haric_tutar;
    QString  gider_fisi_kdv_tutari;
    QString  gider_fisi_kdv_dahil_tutar;
    QString  gelir_fisi_kdv_orani;
    QString  gelir_fisi_kdv_haric_tutar;
    QString  gelir_fisi_kdv_tutari;
    QString  gelir_fisi_kdv_dahil_tutar;

    for ( ; ;) {

        if ( m_gider_fisi_kdv_orani_sayisi > gider_fisi_kdv_listesi_index ) {

            gider_fisi_kdv_orani       = "%" + QVariant ( M_GIDER_FISI_TUTARLAR_LISTESI [ gider_fisi_kdv_listesi_index ].kdv_orani ).toString();
            gider_fisi_kdv_haric_tutar = QVariant ( M_GIDER_FISI_TUTARLAR_LISTESI [ gider_fisi_kdv_listesi_index ].kdv_haric_tutar ).toString();
            gider_fisi_kdv_tutari      = QVariant ( M_GIDER_FISI_TUTARLAR_LISTESI [ gider_fisi_kdv_listesi_index ].kdv_tutari ).toString();
            gider_fisi_kdv_dahil_tutar = QVariant ( M_GIDER_FISI_TUTARLAR_LISTESI [ gider_fisi_kdv_listesi_index ].kdv_dahil_tutar ).toString();

            m_gider_fisi_kdv_haric_toplam  += M_GIDER_FISI_TUTARLAR_LISTESI[gider_fisi_kdv_listesi_index].kdv_haric_tutar;
            m_gider_fisi_kdv_tutari_toplam += M_GIDER_FISI_TUTARLAR_LISTESI[gider_fisi_kdv_listesi_index].kdv_tutari;
            m_gider_fisi_kdv_dahil_toplam  += M_GIDER_FISI_TUTARLAR_LISTESI[gider_fisi_kdv_listesi_index].kdv_dahil_tutar;

            gider_fisi_kdv_listesi_index++;
        }
        else {
            gider_fisi_kdv_orani.clear();
            gider_fisi_kdv_haric_tutar.clear();
            gider_fisi_kdv_tutari.clear();
            gider_fisi_kdv_dahil_tutar.clear();
        }
        if ( m_gelir_fisi_kdv_orani_sayisi > gelir_fisi_kdv_listesi_index ) {

             gelir_fisi_kdv_orani       = "%" + QVariant ( M_GELIR_FISI_TUTARLAR_LISTESI [ gelir_fisi_kdv_listesi_index ].kdv_orani ).toString();
             gelir_fisi_kdv_haric_tutar = QVariant ( M_GELIR_FISI_TUTARLAR_LISTESI [ gelir_fisi_kdv_listesi_index ].kdv_haric_tutar ).toString();
             gelir_fisi_kdv_tutari      = QVariant ( M_GELIR_FISI_TUTARLAR_LISTESI [ gelir_fisi_kdv_listesi_index ].kdv_tutari ).toString();
             gelir_fisi_kdv_dahil_tutar = QVariant ( M_GELIR_FISI_TUTARLAR_LISTESI [ gelir_fisi_kdv_listesi_index ].kdv_dahil_tutar ).toString();

             m_gelir_fisi_kdv_haric_toplam  += M_GELIR_FISI_TUTARLAR_LISTESI [ gelir_fisi_kdv_listesi_index ].kdv_haric_tutar;
             m_gelir_fisi_kdv_tutari_toplam += M_GELIR_FISI_TUTARLAR_LISTESI [ gelir_fisi_kdv_listesi_index ].kdv_tutari;
             m_gelir_fisi_kdv_dahil_toplam  += M_GELIR_FISI_TUTARLAR_LISTESI [ gelir_fisi_kdv_listesi_index ].kdv_dahil_tutar;

             gelir_fisi_kdv_listesi_index++;
        }
        else {
            gelir_fisi_kdv_orani.clear();
            gelir_fisi_kdv_haric_tutar.clear();
            gelir_fisi_kdv_tutari.clear();
            gelir_fisi_kdv_dahil_tutar.clear();
        }


        ADD_ROW_TO_TABLE ( QStringList()<< gider_fisi_kdv_orani
                                        <<VIRGUL_EKLE ( gider_fisi_kdv_haric_tutar,2 )
                                        <<VIRGUL_EKLE ( gider_fisi_kdv_tutari,2 )
                                        <<VIRGUL_EKLE ( gider_fisi_kdv_dahil_tutar,2 )
                                        <<gelir_fisi_kdv_orani
                                        <<VIRGUL_EKLE ( gelir_fisi_kdv_haric_tutar,2 )
                                        <<VIRGUL_EKLE ( gelir_fisi_kdv_tutari,2 )
                                        <<VIRGUL_EKLE ( gelir_fisi_kdv_dahil_tutar,2 ) );

        if ( m_gider_fisi_kdv_orani_sayisi > m_gelir_fisi_kdv_orani_sayisi ) {
            if ( gider_fisi_kdv_listesi_index EQ m_gider_fisi_kdv_orani_sayisi )  {
                m_rapor_bitti_mi = true;
                return GET_TABLE_HTML_STRING();
            }
        }
        else {
            if ( gelir_fisi_kdv_listesi_index EQ m_gelir_fisi_kdv_orani_sayisi ) {
                m_rapor_bitti_mi = true;
                return GET_TABLE_HTML_STRING();
            }
        }
    }
    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   ISL_KDV_RAPORU::GET_FOOTER
***************************************************************************************/

QString ISL_KDV_RAPORU::GET_FOOTER()
{
    if ( m_kayit_var_mi EQ false ) {
        return NULL;
    }
    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 1 >" );
    SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:8%;\"  ALIGN = CENTER"<<"style=\"width:14%;\" ALIGN = RIGHT"
                                        <<"style=\"width:13%;\" ALIGN = RIGHT" <<"style=\"width:15%;\" ALIGN = RIGHT"
                                        <<"style=\"width:8%;\"  ALIGN = CENTER"<<"style=\"width:14%;\" ALIGN = RIGHT"
                                        <<"style=\"width:13%;\" ALIGN = RIGHT" <<"style=\"width:15%;\" ALIGN = RIGHT" );

    CREATE_TABLE         ( QStringList()<<"<b>TOPLAM</b>"
                                        <<VIRGUL_EKLE ( QVariant ( m_gider_fisi_kdv_haric_toplam ).toString(),2 )
                                        <<VIRGUL_EKLE ( QVariant ( m_gider_fisi_kdv_tutari_toplam ).toString(),2 )
                                        <<VIRGUL_EKLE ( QVariant ( m_gider_fisi_kdv_dahil_toplam ).toString(),2 )
                                        <<"<b>TOPLAM</b>"
                                        <<VIRGUL_EKLE ( QVariant ( m_gelir_fisi_kdv_haric_toplam ).toString(),2 )
                                        <<VIRGUL_EKLE ( QVariant ( m_gelir_fisi_kdv_tutari_toplam ).toString(),2 )
                                        <<VIRGUL_EKLE ( QVariant ( m_gelir_fisi_kdv_dahil_toplam ).toString(),2),0,0 );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   ISL_KDV_RAPORU::FIS_SATIRLARINI_KDV_ORANLARINA_GORE_AYIR
***************************************************************************************/

bool ISL_KDV_RAPORU::FIS_SATIRLARINI_KDV_ORANLARINA_GORE_AYIR()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT (  "isl_fis_satirlari, isl_fisi",
                                "isl_fis_satirlari.fis_id, kdv_orani, "
                                "satir_kdv_haric_tutar, satir_kdv_tutari, satir_kdv_dahil_tutar, fis_turu ",
                                "isl_fis_satirlari.fis_id = isl_fisi.fis_id "
                                "AND fis_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi ");


    sql_query.SET_VALUE      (  ":baslangic_tarihi", M_ISL_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );
    sql_query.SET_VALUE      (  ":bitis_tarihi"    , M_ISL_RV->bitis_tarihi.toString ( "yyyy.MM.dd" ) );
    if ( sql_query.SELECT( "fis_tarihi ASC" ) EQ 0 ) {
        return false;
    }
    while ( sql_query.NEXT() EQ true ) {
        int    kdv_orani             = sql_query.VALUE(1).toInt();
        double satir_kdv_haric_tutar = sql_query.VALUE(2).toDouble();
        double satir_kdv_tutari      = sql_query.VALUE(3).toDouble();
        double satir_kdv_dahil_tutar = sql_query.VALUE(4).toDouble();
        switch ( sql_query.VALUE(5).toInt() ) {
            case ENUM_GELIR_FISI :
            {
                if ( M_GELIR_FISI_KDV_LISTESI.contains ( kdv_orani ) EQ true ) {
                    int kdv_index = M_GELIR_FISI_KDV_LISTESI.indexOf ( kdv_orani );
                    M_GELIR_FISI_TUTARLAR_LISTESI [ kdv_index ].kdv_haric_tutar += satir_kdv_haric_tutar;
                    M_GELIR_FISI_TUTARLAR_LISTESI [ kdv_index ].kdv_tutari      += satir_kdv_tutari;
                    M_GELIR_FISI_TUTARLAR_LISTESI [ kdv_index ].kdv_dahil_tutar += satir_kdv_dahil_tutar;
                    break;
                }
                M_GELIR_FISI_KDV_LISTESI.append ( kdv_orani );
                M_GELIR_FISI_TUTARLAR_LISTESI [ m_gelir_fisi_kdv_orani_sayisi ].kdv_orani       = kdv_orani;
                M_GELIR_FISI_TUTARLAR_LISTESI [ m_gelir_fisi_kdv_orani_sayisi ].kdv_haric_tutar = satir_kdv_haric_tutar;
                M_GELIR_FISI_TUTARLAR_LISTESI [ m_gelir_fisi_kdv_orani_sayisi ].kdv_tutari      = satir_kdv_tutari;
                M_GELIR_FISI_TUTARLAR_LISTESI [ m_gelir_fisi_kdv_orani_sayisi ].kdv_dahil_tutar = satir_kdv_dahil_tutar;
                m_gelir_fisi_kdv_orani_sayisi++;
                break;
            }
            case ENUM_GIDER_FISI :
            {
                if ( M_GIDER_FISI_KDV_LISTESI.contains ( kdv_orani ) EQ true ) {
                    int kdv_index = M_GIDER_FISI_KDV_LISTESI.indexOf(kdv_orani);
                    M_GIDER_FISI_TUTARLAR_LISTESI [ kdv_index ].kdv_haric_tutar += satir_kdv_haric_tutar;
                    M_GIDER_FISI_TUTARLAR_LISTESI [ kdv_index ].kdv_tutari      += satir_kdv_tutari;
                    M_GIDER_FISI_TUTARLAR_LISTESI [ kdv_index ].kdv_dahil_tutar += satir_kdv_dahil_tutar;
                    break;
                }
                M_GIDER_FISI_KDV_LISTESI.append ( kdv_orani );
                M_GIDER_FISI_TUTARLAR_LISTESI [ m_gider_fisi_kdv_orani_sayisi].kdv_orani       = kdv_orani;
                M_GIDER_FISI_TUTARLAR_LISTESI [ m_gider_fisi_kdv_orani_sayisi].kdv_haric_tutar = satir_kdv_haric_tutar;
                M_GIDER_FISI_TUTARLAR_LISTESI [ m_gider_fisi_kdv_orani_sayisi].kdv_tutari      = satir_kdv_tutari;
                M_GIDER_FISI_TUTARLAR_LISTESI [ m_gider_fisi_kdv_orani_sayisi].kdv_dahil_tutar = satir_kdv_dahil_tutar;
                m_gider_fisi_kdv_orani_sayisi++;
                break;
            }
            default :
                break;
        }
    }
    return true;
}
