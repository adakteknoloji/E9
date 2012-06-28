#include <QStringList>
#include <adak_sql.h>
#include <adak_utils.h>
#include "isl_defteri_raporu_class.h"
#include "isl_defteri_raporu_open.h"
#include "isl_struct.h"
#include "yonetim.h"
#include "isl_enum.h"
#include "isl_console_utils.h"
#include "e9_console_utils.h"
#include "isl_gui_utils.h"


extern ADAK_SQL *  DB;

/**************************************************************************************
                   GET_ISL_DEFTERI_RAPORU
***************************************************************************************/

QString GET_ISL_DEFTERI_RAPORU( ISL_RAPOR_VARS * P_ISL_RV)
{
    ISL_DEFTERI_RAPORU  *  R = new ISL_DEFTERI_RAPORU(P_ISL_RV);
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU::ISL_DEFTERI_RAPORU
***************************************************************************************/

ISL_DEFTERI_RAPORU::ISL_DEFTERI_RAPORU( ISL_RAPOR_VARS * P_ISL_RV) : REPORT_KERNEL ("ISL_DEFTERI_RAPORU")
{
    SET_HEIGHTS ( 48, 20 );

    m_max_line_count           =  GET_MAX_LINE_COUNT ( 10 );
    m_gider_fisi_index         = 0;
    m_gelir_fisi_index         = 0;
    m_sayfa_numarasi           = 1;
    m_rapor_bitti_mi           = false;
    m_gider_fisi_satiri_var_mi = false;
    m_gelir_fisi_satiri_var_mi = false;
    m_gider_kdv_haric_toplam   = 0.00;
    m_gider_kdv_tutar_toplam   = 0.00;
    m_gider_toplam             = 0.00;
    m_gelir_kdv_haric_toplam   = 0.00;
    m_gelir_kdv_tutar_toplam   = 0.00;
    m_gelir_toplam             = 0.00;
    m_gelirler_yazdirildi      = false;
    m_giderler_yazdirildi      = false;

    M_ISL_RV = P_ISL_RV;

    FIS_SATIRLARINI_AL();

    m_devreden_toplam_var_mi = DEVREDEN_TOPLAM_HESAPLA();
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU::GET_HEADER
***************************************************************************************/

QString ISL_DEFTERI_RAPORU::GET_HEADER()
{
    QString header_string;


    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 0>" ) ;

    SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:75%;\" ALIGN = LEFT"<<"style=\"width:25%;\" ALIGN = RIGHT" );

    CREATE_TABLE         ( QStringList()<<E9_GET_FIRMA_ISMI()<<GET_REPORT_TOP_RIGHT_HEADER(),0,0 );

    ADD_ROW_TO_TABLE     ( QStringList()<<"&nbsp"<<"" );

    header_string.append ( GET_TABLE_HTML_STRING() );

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 0>" );

    SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:100%;\" ALIGN = CENTER " );

    CREATE_TABLE         ( QStringList()<<"<b>İŞLETME HESABI DEFTERİ </b>",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList()<<"&nbsp" );

    ADD_ROW_TO_TABLE     ( QStringList()<<"&nbsp" );
    header_string.append ( GET_TABLE_HTML_STRING() );



    if ( M_ISL_RV->rapor_sekli EQ ENUM_GELIRLER_GIDERLER ) {

        SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 1 >" );

        SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:53%;\" ALIGN = CENTER"<<"style=\"width:47%;\" ALIGN = CENTER" );

        CREATE_TABLE         ( QStringList()<<"<b>GİDER</b>"<<"<b>GELİR</b>", 0, 0 ) ;

        header_string.append ( GET_TABLE_HTML_STRING() );
    }

    return header_string;
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU::GET_BODY
***************************************************************************************/

QString ISL_DEFTERI_RAPORU::GET_BODY()
{

    if ( M_GELIR_FIS_SATIRLARI.size() EQ 0 AND M_GIDER_FIS_SATIRLARI.size() EQ 0 ) {
        return NULL;
    }

    QString body_string;

    if ( M_GELIR_FIS_SATIRLARI.size() > M_GIDER_FIS_SATIRLARI.size() ) {
        if ( M_GELIR_FIS_SATIRLARI.size() EQ m_gelir_fisi_index ) {
            m_rapor_bitti_mi = true;
            return NULL;
        }
    }
    else {
        if ( M_GIDER_FIS_SATIRLARI.size() EQ m_gider_fisi_index ) {
            return NULL;
        }
    }

    if ( m_gider_fisi_satiri_var_mi EQ false AND m_gelir_fisi_satiri_var_mi EQ false ) {
        return NULL;
    }

    QStringList GIDER_FIS_SATIRI;
    QStringList GELIR_FIS_SATIRI;

    QString gider_fis_tarihi;
    QString gider_belge_no;
    QString gider_aciklama;
    QString gider_gider_turu;
    QString gider_kdv_orani;
    QString gider_kdv_haric_tutar;
    QString gider_kdv_tutari;
    QString gider_toplam_tutar;

    QString gelir_fis_tarihi;
    QString gelir_belge_no;
    QString gelir_aciklama;
    QString gelir_kdv_orani;
    QString gelir_kdv_haric_tutar;
    QString gelir_kdv_tutari;
    QString gelir_toplam_tutar;

    int yazdirilan_gider_fisi_satiri = 0;
    int yazdirilan_gelir_fisi_satiri = 0;
    int yazdirilan_satir_sayisi      = 0;

    body_string.clear();

    if ( m_devreden_toplam_var_mi EQ true ) {

        SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 1>" );

        if ( M_ISL_RV->rapor_sekli EQ ENUM_GELIRLER_GIDERLER ) {


            SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 27% ALIGN = RIGHT"<<"WIDTH = 10% ALIGN = RIGHT"
                                                <<"WIDTH = 9% ALIGN  = RIGHT"<<"WIDTH = 10% ALIGN = RIGHT"
                                                <<"WIDTH = 21% ALIGN = RIGHT"<<"WIDTH = 7% ALIGN = RIGHT"
                                                <<"WIDTH = 8% ALIGN  = RIGHT"<<"WIDTH = 8% ALIGN = RIGHT" );

            CREATE_TABLE ( QStringList()<<"<b> DEVREDEN TOPLAM </b>"
                                        <<VIRGUL_EKLE ( QVariant ( m_gider_kdv_haric_toplam).toString(),2 )
                                        <<VIRGUL_EKLE ( QVariant ( m_gider_kdv_tutar_toplam).toString(),2)
                                        <<VIRGUL_EKLE ( QVariant ( m_gider_toplam ).toString(),2 )
                                        <<"<b>DEVREDEN TOPLAM</b>"
                                        <<VIRGUL_EKLE ( QVariant ( m_gelir_kdv_haric_toplam ).toString(),2 )
                                        <<VIRGUL_EKLE ( QVariant ( m_gelir_kdv_tutar_toplam ).toString(),2 )
                                        <<VIRGUL_EKLE ( QVariant ( m_gelir_toplam).toString(),2 ),0,0 );
        }
        else {
            SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 48% ALIGN = RIGHT"<<"WIDTH = 17% ALIGN = RIGHT"
                                                <<"WIDTH = 17% ALIGN  = RIGHT"<<"WIDTH = 18% ALIGN = RIGHT" );


            CREATE_TABLE ( QStringList()<<"<b>DEVREDEN GELİR TOPLAMI</b>"
                                        <<VIRGUL_EKLE ( QVariant ( m_gelir_kdv_haric_toplam ).toString(),2 )
                                        <<VIRGUL_EKLE ( QVariant ( m_gelir_kdv_tutar_toplam ).toString(),2 )
                                        <<VIRGUL_EKLE ( QVariant ( m_gelir_toplam).toString(),2 ),0,0 );

            if ( m_gelirler_yazdirildi EQ true ) {
                CREATE_TABLE ( QStringList()<<"<b> DEVREDEN GİDER TOPLAMI </b>"
                                            <<VIRGUL_EKLE ( QVariant ( m_gider_kdv_haric_toplam).toString(),2 )
                                            <<VIRGUL_EKLE ( QVariant ( m_gider_kdv_tutar_toplam).toString(),2)
                                            <<VIRGUL_EKLE ( QVariant ( m_gider_toplam ).toString(),2 )
                                            ,0,0 );
            }
        }
        body_string.append ( GET_TABLE_HTML_STRING() );
    }





    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% BORDER=1 style=font-size:75%;>" );

    if ( M_ISL_RV->rapor_sekli EQ ENUM_GELIRLER_GIDERLER ) {

        SET_TD_TAG_STRING    ( QStringList()<<"WIDTH=7%"

                                            <<"WIDTH=6%"              <<"WIDTH=8%"              <<"WIDTH=6%"
                                            <<"WIDTH=6%"              <<"WIDTH=6% ALIGN = RIGHT"<<"WIDTH=6% ALIGN = RIGHT"
                                            <<"WIDTH=8% ALIGN = RIGHT"

                                            <<"WIDTH=7%"              <<"WIDTH=6%"
                                            <<"WIDTH=8%"              <<"WIDTH=6%"              <<"WIDTH=6% ALIGN = RIGHT"
                                            <<"WIDTH=6% ALIGN = RIGHT"<<"WIDTH=8% ALIGN = RIGHT" );

        CREATE_TABLE ( QStringList()<<"Tarih"<<"Blg No"<<"Açıklama"<<"Cinsi"
                                    <<"KDV %"<<"KDV Hrç Tutar"<<"KDV Tutarı"
                                    <<"Toplam Tutar"<<"Tarih"<<"Blg No"<<"Açıklama"
                                    <<"KDV %"<<"KDV Hrç Tutar"<<"KDV Tutarı"<<"Toplam Tutar",0,0 );
    }
    else {
        SET_TD_TAG_STRING    ( QStringList()<<"WIDTH=14%"
                                            <<"WIDTH=12%"              <<"WIDTH=16%"              <<"WIDTH=12%"
                                            <<"WIDTH=12%"              <<"WIDTH=12% ALIGN = RIGHT"<<"WIDTH=12% ALIGN = RIGHT"
                                            <<"WIDTH=10% ALIGN = RIGHT");

        CREATE_TABLE ( QStringList()<<"Tarih"<<"Blg No"<<"Açıklama"<<"Cinsi"
                                    <<"KDV %"<<"KDV Hrç Tutar"<<"KDV Tutarı"
                                    <<"Toplam Tutar",0,0 );

    }

    for ( ; ; ) {

        gider_fis_tarihi.clear();
        gider_belge_no.clear();
        gider_aciklama.clear();
        gider_gider_turu.clear();
        gider_kdv_orani.clear();
        gider_kdv_haric_tutar.clear();
        gider_kdv_tutari.clear();
        gider_toplam_tutar.clear();
        gelir_fis_tarihi.clear();
        gelir_belge_no.clear();
        gelir_aciklama.clear();
        gelir_kdv_orani.clear();
        gelir_kdv_haric_tutar.clear();
        gelir_kdv_tutari.clear();
        gelir_toplam_tutar.clear();

        //GELIRLER/GIDERLER ..........

        if ( M_ISL_RV->rapor_sekli EQ ENUM_GELIRLER_GIDERLER ) {
            if ( m_gider_fisi_satiri_var_mi EQ true ) {
                GIDER_FIS_SATIRI              = QVariant ( M_GIDER_FIS_SATIRLARI.at(m_gider_fisi_index)
                                                     ).toString().split ( "\t" );

                gider_fis_tarihi      = GIDER_FIS_SATIRI.at(1);
                gider_belge_no        = GIDER_FIS_SATIRI.at(2);
                gider_aciklama        = GIDER_FIS_SATIRI.at(3);
                gider_gider_turu      = GIDER_FIS_SATIRI.at(4);
                gider_kdv_orani       = GIDER_FIS_SATIRI.at(5);
                gider_kdv_haric_tutar = VIRGUL_EKLE(GIDER_FIS_SATIRI.at(6),2);
                gider_kdv_tutari      = VIRGUL_EKLE(GIDER_FIS_SATIRI.at(7),2);
                gider_toplam_tutar    = VIRGUL_EKLE(GIDER_FIS_SATIRI.at(8),2);

                m_gider_kdv_haric_toplam += ROUND(QVariant(GIDER_FIS_SATIRI.at(6)).toDouble());
                m_gider_kdv_tutar_toplam += ROUND(QVariant(GIDER_FIS_SATIRI.at(7)).toDouble());
                m_gider_toplam           += ROUND(QVariant(GIDER_FIS_SATIRI.at(8)).toDouble());

            }
            if ( m_gelir_fisi_satiri_var_mi EQ true ) {
                GELIR_FIS_SATIRI              = QVariant ( M_GELIR_FIS_SATIRLARI.at(m_gelir_fisi_index )
                                                     ).toString().split( "\t" );
                gelir_fis_tarihi      = GELIR_FIS_SATIRI.at(1);
                gelir_belge_no        = GELIR_FIS_SATIRI.at(2);
                gelir_aciklama        = GELIR_FIS_SATIRI.at(3);
                gelir_kdv_orani       = GELIR_FIS_SATIRI.at(4);
                gelir_kdv_haric_tutar = VIRGUL_EKLE ( GELIR_FIS_SATIRI.at(5),2 );
                gelir_kdv_tutari      = VIRGUL_EKLE ( GELIR_FIS_SATIRI.at(6),2 );
                gelir_toplam_tutar    = VIRGUL_EKLE ( GELIR_FIS_SATIRI.at(7),2 );

                m_gelir_kdv_haric_toplam += ROUND ( QVariant ( GELIR_FIS_SATIRI.at(5) ).toDouble() );
                m_gelir_kdv_tutar_toplam += ROUND ( QVariant   ( GELIR_FIS_SATIRI.at(6)).toDouble() );
                m_gelir_toplam           += ROUND ( QVariant   ( GELIR_FIS_SATIRI.at(7)).toDouble() );
            }

            ADD_ROW_TO_TABLE ( QStringList() << gider_fis_tarihi      << gider_belge_no        << gider_aciklama
                                             << gider_gider_turu      << gider_kdv_orani       << gider_kdv_haric_tutar
                                             << gider_kdv_tutari      << gider_toplam_tutar    << gelir_fis_tarihi
                                             << gelir_belge_no        << gelir_aciklama        << gelir_kdv_orani
                                             << gelir_kdv_haric_tutar << gelir_kdv_tutari      << gelir_toplam_tutar);


            m_gider_fisi_index++;
            m_gelir_fisi_index++;

            if ( m_gider_fisi_index EQ M_GIDER_FIS_SATIRLARI.size() ) {
                m_gider_fisi_satiri_var_mi = false;
            }
            if ( m_gelir_fisi_index EQ M_GELIR_FIS_SATIRLARI.size() ) {
                m_gelir_fisi_satiri_var_mi = false;
            }

            if ( m_gider_fisi_satiri_var_mi EQ false AND m_gelir_fisi_satiri_var_mi EQ false ) {
                body_string.append ( GET_TABLE_HTML_STRING() );
                return body_string;
            }

            yazdirilan_gelir_fisi_satiri++;
            yazdirilan_gider_fisi_satiri++;

            if ( yazdirilan_gider_fisi_satiri EQ m_max_line_count ) {
                m_sayfa_numarasi++;
                body_string.append ( GET_TABLE_HTML_STRING() );
                return body_string;
            }
            if ( yazdirilan_gelir_fisi_satiri EQ m_max_line_count ) {
                m_sayfa_numarasi++;
                body_string.append ( GET_TABLE_HTML_STRING() );
                return body_string;
            }

            if ( M_GELIR_FIS_SATIRLARI.size() > M_GIDER_FIS_SATIRLARI.size() ) {
                if ( M_GELIR_FIS_SATIRLARI.size() EQ m_gelir_fisi_index ) {
                    m_rapor_bitti_mi = true;
                    body_string.append ( GET_TABLE_HTML_STRING() );
                    return body_string;
                }
            }
            else {
                if ( M_GIDER_FIS_SATIRLARI.size() EQ m_gider_fisi_index ) {
                    m_rapor_bitti_mi = true;
                    body_string.append ( GET_TABLE_HTML_STRING() );
                    return body_string;
                }
            }

        }
        //ONCE GELIRLER SONRA GIDERLER  ...........

        else {

            if ( m_gelir_fisi_satiri_var_mi EQ true ) {
                GELIR_FIS_SATIRI              = QVariant ( M_GELIR_FIS_SATIRLARI.at(m_gelir_fisi_index )
                                                     ).toString().split( "\t" );
                gelir_fis_tarihi      = GELIR_FIS_SATIRI.at(1);
                gelir_belge_no        = GELIR_FIS_SATIRI.at(2);
                gelir_aciklama        = GELIR_FIS_SATIRI.at(3);
                gelir_kdv_orani       = GELIR_FIS_SATIRI.at(4);
                gelir_kdv_haric_tutar = VIRGUL_EKLE ( GELIR_FIS_SATIRI.at(5),2 );
                gelir_kdv_tutari      = VIRGUL_EKLE ( GELIR_FIS_SATIRI.at(6),2 );
                gelir_toplam_tutar    = VIRGUL_EKLE ( GELIR_FIS_SATIRI.at(7),2 );

                m_gelir_kdv_haric_toplam += ROUND ( QVariant ( GELIR_FIS_SATIRI.at(5) ).toDouble() );
                m_gelir_kdv_tutar_toplam += ROUND ( QVariant   ( GELIR_FIS_SATIRI.at(6)).toDouble() );
                m_gelir_toplam           += ROUND ( QVariant   ( GELIR_FIS_SATIRI.at(7)).toDouble() );

                ADD_ROW_TO_TABLE ( QStringList() << gelir_fis_tarihi << gelir_belge_no        << gelir_aciklama
                                   << QObject::tr("GELİR") << gelir_kdv_orani
                                                 << gelir_kdv_haric_tutar << gelir_kdv_tutari      << gelir_toplam_tutar);
                m_gelir_fisi_index++;

                if ( m_gelir_fisi_index EQ M_GELIR_FIS_SATIRLARI.size() ) {
                    m_gelir_fisi_satiri_var_mi = false;
                    m_gelirler_yazdirildi      = true;
                }
            }

            if ( m_gelirler_yazdirildi EQ true ) {

                if ( m_gider_fisi_satiri_var_mi EQ true ) {
                    GIDER_FIS_SATIRI              = QVariant ( M_GIDER_FIS_SATIRLARI.at(m_gider_fisi_index)
                                                         ).toString().split ( "\t" );

                    gider_fis_tarihi      = GIDER_FIS_SATIRI.at(1);
                    gider_belge_no        = GIDER_FIS_SATIRI.at(2);
                    gider_aciklama        = GIDER_FIS_SATIRI.at(3);
                    gider_gider_turu      = GIDER_FIS_SATIRI.at(4);
                    gider_kdv_orani       = GIDER_FIS_SATIRI.at(5);
                    gider_kdv_haric_tutar = VIRGUL_EKLE(GIDER_FIS_SATIRI.at(6),2);
                    gider_kdv_tutari      = VIRGUL_EKLE(GIDER_FIS_SATIRI.at(7),2);
                    gider_toplam_tutar    = VIRGUL_EKLE(GIDER_FIS_SATIRI.at(8),2);

                    m_gider_kdv_haric_toplam += ROUND(QVariant(GIDER_FIS_SATIRI.at(6)).toDouble());
                    m_gider_kdv_tutar_toplam += ROUND(QVariant(GIDER_FIS_SATIRI.at(7)).toDouble());
                    m_gider_toplam           += ROUND(QVariant(GIDER_FIS_SATIRI.at(8)).toDouble());

                    ADD_ROW_TO_TABLE ( QStringList() << gider_fis_tarihi      << gider_belge_no        << gider_aciklama
                                                     << gider_gider_turu      << gider_kdv_orani       << gider_kdv_haric_tutar
                                                     << gider_kdv_tutari      << gider_toplam_tutar    );
                    m_gider_fisi_index++;

                    if ( m_gider_fisi_index EQ M_GIDER_FIS_SATIRLARI.size() ) {
                        m_gider_fisi_satiri_var_mi = false;
                        m_giderler_yazdirildi      = true;
                    }

                }

            }
            if ( m_gider_fisi_satiri_var_mi EQ false AND m_gelir_fisi_satiri_var_mi EQ false ) {
                body_string.append ( GET_TABLE_HTML_STRING() );
                return body_string;
            }
            yazdirilan_satir_sayisi++;

            if ( yazdirilan_satir_sayisi EQ m_max_line_count ) {
                m_sayfa_numarasi++;
                body_string.append ( GET_TABLE_HTML_STRING() );
                return body_string;
            }

            if ( m_gelirler_yazdirildi EQ true ) {

                if ( m_gider_fisi_index EQ M_GIDER_FIS_SATIRLARI.size() ) {
                    m_rapor_bitti_mi = true;
                    body_string.append ( GET_TABLE_HTML_STRING() );
                    return body_string;
                }
            }
        }
    }
    return body_string;
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU::GET_FOOTER
***************************************************************************************/

QString ISL_DEFTERI_RAPORU::GET_FOOTER()
{
    QString gider_toplam_basligi = "SAYFA TOPLAMI";
    QString gelir_toplam_basligi = "SAYFA TOPLAMI";

    if (m_rapor_bitti_mi EQ true ) {
        gider_toplam_basligi = "TOPLAMLAR";
        gelir_toplam_basligi = "TOPLAMLAR";
    }

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 1>" );

    if ( M_ISL_RV->rapor_sekli EQ ENUM_GELIRLER_GIDERLER ) {

        SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 33% ALIGN = RIGHT"<<"WIDTH = 6% ALIGN = RIGHT"
                                            <<"WIDTH = 6% ALIGN  = RIGHT"<<"WIDTH = 8% ALIGN = RIGHT"
                                            <<"WIDTH = 27% ALIGN = RIGHT"<<"WIDTH = 6% ALIGN = RIGHT"
                                            <<"WIDTH = 6% ALIGN  = RIGHT"<<"WIDTH = 8% ALIGN = RIGHT" );

        CREATE_TABLE ( QStringList() <<"<b>" +gider_toplam_basligi +"</b>"
                                  <<VIRGUL_EKLE ( QVariant(m_gider_kdv_haric_toplam).toString(),2 )
                                  <<VIRGUL_EKLE ( QVariant(m_gider_kdv_tutar_toplam).toString(),2 )
                                  <<VIRGUL_EKLE ( QVariant(m_gider_toplam).toString(),2)
                                  <<"<b>"+ gelir_toplam_basligi + "</b>"
                                  <<VIRGUL_EKLE ( QVariant(m_gelir_kdv_haric_toplam).toString(),2 )
                                  <<VIRGUL_EKLE ( QVariant(m_gelir_kdv_tutar_toplam).toString(),2 )
                                  <<VIRGUL_EKLE ( QVariant(m_gelir_toplam).toString(),2 ),0,0 );

    }
    else {
        SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 66% ALIGN = RIGHT"<<"WIDTH = 12% ALIGN = RIGHT"
                                            <<"WIDTH = 12% ALIGN  = RIGHT"<<"WIDTH = 10% ALIGN = RIGHT");

        CREATE_TABLE ( QStringList() <<"<b>" + gider_toplam_basligi +"</b>"
                                  <<VIRGUL_EKLE ( QVariant(m_gider_kdv_haric_toplam + m_gelir_kdv_haric_toplam).toString(),2 )
                                  <<VIRGUL_EKLE ( QVariant(m_gider_kdv_tutar_toplam + m_gelir_kdv_tutar_toplam).toString(),2 )
                                  <<VIRGUL_EKLE ( QVariant(m_gider_toplam           + m_gelir_toplam).toString(),2),0,0 );



    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU::FIS_SATIRLARINI_AL
***************************************************************************************/

bool ISL_DEFTERI_RAPORU::FIS_SATIRLARINI_AL()
{
    QString fis_satiri;
    QString fis_id;
    QString fis_tarihi;
    QString belge_no;
    QString aciklama;
    QString kdv_orani;
    QString satir_kdv_haric_tutar;
    QString satir_kdv_tutari;
    QString satir_kdv_dahil_tutar;
    QString gider_turu;

    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "isl_fis_satirlari, isl_fisi","isl_fis_satirlari.fis_id, fis_tarihi, belge_no, "
                               "isl_fis_satirlari.aciklama, kdv_orani, "
                               "satir_kdv_haric_tutar, satir_kdv_tutari, satir_kdv_dahil_tutar, fis_turu, gider_turu ",
                               "isl_fis_satirlari.fis_id = isl_fisi.fis_id "
                               "AND fis_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi ");


    sql_query.SET_VALUE      ( ":baslangic_tarihi", M_ISL_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );
    sql_query.SET_VALUE      ( ":bitis_tarihi"    , M_ISL_RV->bitis_tarihi.toString ( "yyyy.MM.dd" ) );
    if ( sql_query.SELECT("fis_tarihi ASC " ) EQ 0 ) {
        return false;
    }
    while ( sql_query.NEXT() EQ true ) {

        switch ( sql_query.VALUE(8).toInt() ) {
            case ENUM_GELIR_FISI :
            {
                m_gelir_fisi_satiri_var_mi = true;
                fis_id                     = sql_query.VALUE(0).toString();
                fis_tarihi                 = sql_query.VALUE(1).toDate().toString( "dd.MM.yyyy" );
                belge_no                   = sql_query.VALUE(2).toString();
                aciklama                   = sql_query.VALUE(3).toString();
                kdv_orani                  = "%" + sql_query.VALUE(4).toString();
                satir_kdv_haric_tutar      = sql_query.VALUE(5).toString();
                satir_kdv_tutari           = sql_query.VALUE(6).toString();
                satir_kdv_dahil_tutar         = sql_query.VALUE(7).toString();

                fis_satiri.clear();

                fis_satiri.append ( fis_id                  + "\t" +
                                    fis_tarihi              + "\t" +
                                    belge_no                + "\t" +
                                    aciklama                + "\t" +
                                    kdv_orani               + "\t" +
                                    satir_kdv_haric_tutar   + "\t" +
                                    satir_kdv_tutari        + "\t" +
                                    satir_kdv_dahil_tutar );

                M_GELIR_FIS_SATIRLARI.append ( fis_satiri );
                break;
            }
            case ENUM_GIDER_FISI :
            {
                m_gider_fisi_satiri_var_mi  = true;
                fis_id                      = sql_query.VALUE(0).toString();
                fis_tarihi                  = sql_query.VALUE(1).toDate().toString ( "dd.MM.yyyy" );
                belge_no                    = sql_query.VALUE(2).toString();
                aciklama                    = sql_query.VALUE(3).toString();
                kdv_orani                   = "%" + sql_query.VALUE(4).toString();
                satir_kdv_haric_tutar       = sql_query.VALUE(5).toString();
                satir_kdv_tutari            = sql_query.VALUE(6).toString();
                satir_kdv_dahil_tutar       = sql_query.VALUE(7).toString();
                gider_turu                  = ISL_GET_GIDER_STRING( sql_query.VALUE(9).toInt() );

                fis_satiri.clear();

                fis_satiri.append ( fis_id                  + "\t" +
                                    fis_tarihi              + "\t" +
                                    belge_no                + "\t" +
                                    aciklama                + "\t" +
                                    gider_turu              + "\t" +
                                    kdv_orani               + "\t" +
                                    satir_kdv_haric_tutar   + "\t" +
                                    satir_kdv_tutari        + "\t" +
                                    satir_kdv_dahil_tutar );

                M_GIDER_FIS_SATIRLARI.append ( fis_satiri );
                break;
            }
            default :
                break;
        }
    }

    if ( m_gelir_fisi_satiri_var_mi EQ false ) {
        m_gelirler_yazdirildi = true;
    }
    if ( m_gider_fisi_satiri_var_mi EQ false ) {
        m_giderler_yazdirildi = true;
    }

    return true;
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU::DEVREDEN_TOPLAM_HESAPLA
***************************************************************************************/

bool ISL_DEFTERI_RAPORU::DEVREDEN_TOPLAM_HESAPLA()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "isl_fis_satirlari, isl_fisi","satir_kdv_haric_tutar, satir_kdv_tutari, satir_kdv_dahil_tutar, fis_turu ",
                               "isl_fis_satirlari.fis_id = isl_fisi.fis_id "
                               "AND fis_tarihi < :baslangic_tarihi" );
    sql_query.SET_VALUE      ( ":baslangic_tarihi", M_ISL_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );

    if ( sql_query.SELECT() EQ 0 ) {
        return false;
    }
    while ( sql_query.NEXT() EQ true ) {
        switch ( sql_query.VALUE(3).toInt() ) {

            case ENUM_GELIR_FISI :
            default         :
                m_gelir_kdv_haric_toplam += sql_query.VALUE(0).toDouble();
                m_gelir_kdv_tutar_toplam += sql_query.VALUE(1).toDouble();
                m_gelir_toplam           += sql_query.VALUE(2).toDouble();
                break;

            case ENUM_GIDER_FISI :

                m_gider_kdv_haric_toplam += sql_query.VALUE(0).toDouble();
                m_gider_kdv_tutar_toplam += sql_query.VALUE(1).toDouble();
                m_gider_toplam           += sql_query.VALUE(2).toDouble();
                break;

        }
    }
    return true;
}
