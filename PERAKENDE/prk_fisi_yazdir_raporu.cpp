#include "adak_utils.h"
#include <QDate>
#include "prk_fisi_yazdir_raporu_class.h"
#include "prk_fisi_yazdir_raporu_open.h"
#include "yonetim.h"
#include "e9_console_utils.h"

extern ADAK_SQL *         DB;


/**************************************************************************************
                   GET_PRK_FISI_YAZDIR_RAPORU
***************************************************************************************/

QString GET_PRK_FISI_YAZDIR_RAPORU (int fis_id)
{
    PRK_FISI_YAZDIR_RAPORU  *  F = new PRK_FISI_YAZDIR_RAPORU("Perakende Fisi Raporu" , fis_id);
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   PRK_FISI_YAZDIR_RAPORU::PRK_FISI_YAZDIR_RAPORU
***************************************************************************************/

PRK_FISI_YAZDIR_RAPORU::PRK_FISI_YAZDIR_RAPORU ( QString rapor_ismi, int fis_id) : REPORT_KERNEL (rapor_ismi)
{
    SET_HEIGHTS ( 30, 30 );

    m_max_line_count    =  GET_MAX_LINE_COUNT ( 14 );
    m_fis_id            = fis_id;
    m_last_satir_indisi = 0;
    m_fis_yazdirildi    = false;

    m_kdv_haric_toplam  = 0.0;
    m_kdv_dahil_toplam  = 0.0;
    m_kdv_toplam        = 0.0;

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler" , "fis_tarihi,fis_no,aciklama" , "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , m_fis_id);

    if ( query.SELECT() NE 0 ) {

        query.NEXT();

        m_fis_header_bilgileri << QObject::tr("Fiş Tarihi") + ADD_HTML_SPACE("  " )+ " : " + ADD_HTML_SPACE("  ")+ query.VALUE(0).toDate().toString("dd MMMM yyyy dddd")
                << QObject::tr("Fiş No") + ADD_HTML_SPACE("  ") + ":" + ADD_HTML_SPACE(" ") + query.VALUE(1).toString()
                << QObject::tr("Açıklama") + ADD_HTML_SPACE("   ") + ":" + ADD_HTML_SPACE("   ") + query.VALUE(2).toString();

    }

    FILL_FIS_SATIRLARI_LIST();

}

/**************************************************************************************
                   PRK_FISI_YAZDIR_RAPORU::GET_HEADER
***************************************************************************************/

QString PRK_FISI_YAZDIR_RAPORU::GET_HEADER()
{

    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:90%; font-size:90%;\" BORDER=0 >");

    SET_TD_TAG_STRING    ( QStringList()<<"style = \"width:37%; font-size:90%;\""<<"style = \"width:26%;font-size:100%;\" ALIGN=CENTER"
                                   << "style = \"width:37%;style = \"font-size:90%;\" ALIGN = RIGHT");

    CREATE_TABLE ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE ( QStringList() << E9_GET_FIRMA_ISMI() << QObject::tr("Perakende Fişi") << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE ( QStringList() << m_fis_header_bilgileri.at(0) << m_fis_header_bilgileri.at(1) << "");

    ADD_ROW_TO_TABLE ( QStringList() << m_fis_header_bilgileri.at(2) << "" << "");


    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   PRK_FISI_YAZDIR_RAPORU::GET_BODY
***************************************************************************************/

QString PRK_FISI_YAZDIR_RAPORU::GET_BODY()
{
    if ( m_fis_yazdirildi EQ true ) {
        return NULL;
    }

    int sayfaya_yazdirilan_satir_sayisi = 0;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:80%;\" BORDER =1 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=18%" << "WIDTH=10%"<< "WIDTH=23%" << "WIDTH=10%"
                                         << "WIDTH=13%" << "WIDTH=13%" << "WIDTH=13%" );

    CREATE_TABLE         ( QStringList()<< "<b>Gider Türü" << "<b>Belge No" <<"<b>Açıklama"
                           << "<b>Kdv Oranı" << "<b>Kdv Hariç Tutar" << "<b>Kdv Tutarı" << "<b>Kdv Dahil Tutar",1,13);

    SET_TD_TAG_STRING    ( QStringList() << "" << "" << "" << "" << "ALIGN=RIGHT" << "ALIGN=RIGHT"<<"ALIGN=RIGHT");


    QStringList RAPOR_SATIRI;


    for ( ; ; ) {

        if ( m_last_satir_indisi EQ M_FIS_SATIRLARI_LIST.size() ) {
            m_fis_yazdirildi = true;
            break;
        }

        RAPOR_SATIRI = M_FIS_SATIRLARI_LIST.at(m_last_satir_indisi).split("\t");

        ADD_ROW_TO_TABLE(QStringList () << RAPOR_SATIRI.at(0) << RAPOR_SATIRI.at(1) << RAPOR_SATIRI.at(2)
                                        << RAPOR_SATIRI.at(3) << RAPOR_SATIRI.at(4) << RAPOR_SATIRI.at(5)
                                        << RAPOR_SATIRI.at(6));

        m_kdv_haric_toplam += QVariant(RAPOR_SATIRI.at(4)).toDouble();
        m_kdv_toplam       += QVariant(RAPOR_SATIRI.at(5)).toDouble();
        m_kdv_dahil_toplam += QVariant(RAPOR_SATIRI.at(6)).toDouble();

        sayfaya_yazdirilan_satir_sayisi++;
        m_last_satir_indisi++;

        if ( sayfaya_yazdirilan_satir_sayisi EQ m_max_line_count ) {
            break;
        }
    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   PRK_FISI_YAZDIR_RAPORU::GET_FOOTER
***************************************************************************************/

QString PRK_FISI_YAZDIR_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:80%;\" BORDER =0 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=61%" << "WIDTH=13% ALIGN=RIGHT" << "WIDTH=13% ALIGN=RIGHT"
                           << "WIDTH=13% ALIGN=RIGHT" );

    CREATE_TABLE(QStringList() << "" << "" << "" << "" ,1,14);

    ADD_ROW_TO_TABLE(QStringList() << "" << "<b>" + VIRGUL_EKLE(QVariant(m_kdv_haric_toplam).toString(),2)
                     << "<b>" + VIRGUL_EKLE( QVariant(m_kdv_toplam).toString(),2)
                     << "<b>" + VIRGUL_EKLE( QVariant( m_kdv_dahil_toplam).toString(),2));

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   PRK_FISI_YAZDIR_RAPORU::FILL_FIS_SATIRLARI_LIST();
***************************************************************************************/

void PRK_FISI_YAZDIR_RAPORU::FILL_FIS_SATIRLARI_LIST()
{
    QStringList FIS_SATIRI;

    SQL_QUERY   query(DB);
    SQL_QUERY   s_query(DB);

    query.PREPARE_SELECT("prk_fis_satirlari" , "gider_turu_id,belge_no,aciklama,kdv_orani,kdv_haric_tutar,kdv_tutari,"
                         "kdv_dahil_tutar" , "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , m_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    while ( query.NEXT() ) {

        int gider_turu_id = query.VALUE(0).toInt();

        s_query.PREPARE_SELECT("prk_gelir_gider_turleri" , "gelir_gider_adi" , "tur_id = :tur_id");

        s_query.SET_VALUE(":tur_id" , gider_turu_id);

        QString gider_turu = "";
        if ( s_query.SELECT() NE 0 ) {
            s_query.NEXT();
            gider_turu = s_query.VALUE(0).toString();
        }
        QString belge_no        = query.VALUE(1).toString();
        QString aciklama        = query.VALUE(2).toString();
        QString kdv_orani       = "%" + query.VALUE(3).toString();
        QString kdv_haric_tutar = VIRGUL_EKLE(query.VALUE(4).toString(),2);
        QString kdv_tutari      = VIRGUL_EKLE(query.VALUE(5).toString(),2);
        QString kdv_dahil_tutar = VIRGUL_EKLE(query.VALUE(6).toString(),2);

        FIS_SATIRI.clear();

        FIS_SATIRI.append(gider_turu + "\t" + belge_no + "\t" + aciklama + "\t" + kdv_orani + "\t" + kdv_haric_tutar
                          + "\t" + kdv_tutari + "\t" + kdv_dahil_tutar );

        M_FIS_SATIRLARI_LIST << FIS_SATIRI;
    }
}














