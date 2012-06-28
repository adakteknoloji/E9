#include "adak_utils.h"
#include "e9_enum.h"
#include "e9_struct.h"
#include "irs_irsaliye_listesi_raporu_class.h"
#include "irs_irsaliye_listesi_raporu_open.h"
#include "irs_struct.h"
#include "yonetim.h"
#include "sube_console_utils.h"
#include "cari_console_utils.h"
#include "irs_console_utils.h"
#include "irs_enum.h"
#include "e9_console_utils.h"
#include "stok_enum.h"

extern ADAK_SQL *         DB;

/**************************************************************************************
                   GET_IRS_IRSALIYE_LISTESI_RAPORU
***************************************************************************************/

QString GET_IRS_IRSALIYE_LISTESI_RAPORU ( IRSALIYE_RAPOR_VARS * P_IRS_RV  )
{
    IRS_IRSALIYE_LISTESI_RAPORU  *  F = new IRS_IRSALIYE_LISTESI_RAPORU( P_IRS_RV, "Irsaliye Listesi Raporu" );
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_RAPORU::IRS_IRSALIYE_LISTESI_RAPORU
***************************************************************************************/

IRS_IRSALIYE_LISTESI_RAPORU::IRS_IRSALIYE_LISTESI_RAPORU( IRSALIYE_RAPOR_VARS * P_IRS_RV , QString rapor_basligi ) : REPORT_KERNEL (rapor_basligi)
{
    SET_HEIGHTS ( 30, 30 );

    M_IRS_RV    = P_IRS_RV;

    m_max_line_count  =  GET_MAX_LINE_COUNT ( 14 );

    m_rapor_kriterleri_1 = M_IRS_RV->baslangic_tarihi.toString("dd MMMM yyyy") + " - " + M_IRS_RV->bitis_tarihi.toString("dd MMMM yyyy");

    if ( M_IRS_RV->cari_hesap_id NE -1 ) {

        QString cari_hesap_kodu, cari_hesap_ismi;
        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_IRS_RV->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

        if ( result NE 0 ) {
            m_rapor_kriterleri_1.append(" - Cari Hesap : " + cari_hesap_kodu+ "  " + cari_hesap_ismi );
        }
    }

    if ( M_IRS_RV->fatura_durumu NE -1 ) {
        m_rapor_kriterleri_1.append(" - " + IRS_GET_FATURA_DURUMU_STRING(M_IRS_RV->fatura_durumu));
    }
    if ( M_IRS_RV->depo_id NE -1 ) {
        QString depo_kodu;
        QString depo_adi;

        SUBE_GET_DEPO_KODU_ADI ( M_IRS_RV->depo_id, &depo_kodu , &depo_adi);
        m_rapor_kriterleri_1.append( " - Depo Kodu / Adı : " + depo_kodu + "  " + depo_adi);

    }

    m_baslangic_tarihi = M_IRS_RV->baslangic_tarihi.toString("dd MMMM yyyy");
    m_bitis_tarihi     = M_IRS_RV->bitis_tarihi.toString("dd MMMM yyyy");

    m_tum_rapor_satirlari_basildi = false;
    m_last_satir_indisi           = 0;
    m_footer_tutar                = 0.00;

    FILL_RAPOR_SATIRLARI();

}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_RAPORU::GET_HEADER
***************************************************************************************/

QString IRS_IRSALIYE_LISTESI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()<< "" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>İRSALİYE  LİSTESİ" << "");

    ADD_ROW_TO_TABLE     ( QStringList() << "" << m_rapor_kriterleri_1 << "");


   return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_RAPORU::GET_BODY
***************************************************************************************/

QString IRS_IRSALIYE_LISTESI_RAPORU::GET_BODY()
{

    if ( m_tum_rapor_satirlari_basildi EQ true ) {
        return NULL;
    }

    int sayfaya_yazdirilan_satir_sayisi = 0;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=13%"
                                         << "WIDTH=5%"
                                         << "WIDTH=10%"
                                         << "WIDTH=10%"
                                         << "WIDTH=25%"
                                         << "WIDTH=15%"
                                         << "WIDTH=12%"
                                         << "WIDTH=10%" );

    CREATE_TABLE         ( QStringList()<< "<b>İrsaliye Tarihi" << "<b>Fiş No" << "<b>B. Seri/No"<<"<b>Açıklama"
                           << "<b>Cari Hesap Kodu/Adı" << "<b>Depo Kodu/Adı" << "<b>Durumu" << "<b>Tutarı",1,13);

    SET_TD_TAG_STRING    ( QStringList()<< "" << "" << "" << "" << "" << "" << ""<<"ALIGN=RIGHT");


    QStringList RAPOR_SATIRI;


    for ( ; ; ) {

        if ( m_last_satir_indisi EQ M_RAPOR_SATIRLARI.size() ) {
            m_tum_rapor_satirlari_basildi =  true;
            break;
        }


        RAPOR_SATIRI = M_RAPOR_SATIRLARI.at(m_last_satir_indisi).split("\t");

        ADD_ROW_TO_TABLE (QStringList() << RAPOR_SATIRI.at(0)
                                        << RAPOR_SATIRI.at(1)
                                        << RAPOR_SATIRI.at(2)
                                        << RAPOR_SATIRI.at(3).mid( 0, 12 )
                                        << RAPOR_SATIRI.at(4).mid( 0, 25 )
                                        << RAPOR_SATIRI.at(5)
                                        << RAPOR_SATIRI.at(6)
                                        << RAPOR_SATIRI.at(7));

        sayfaya_yazdirilan_satir_sayisi++;
        m_last_satir_indisi++;

        if ( sayfaya_yazdirilan_satir_sayisi EQ m_max_line_count ) {
            break;
        }
    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString IRS_IRSALIYE_LISTESI_RAPORU::GET_FOOTER()
{

    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:75%;font-weight:bold;\" BORDER = 0 >");

    SET_TD_TAG_STRING ( QStringList() << "style = \"width:90%;\""
                                      << "style = \"width:10%;\" ALIGN = RIGHT" );


    CREATE_TABLE( QStringList() << "" << "",0,0);

    QStringList footer_line;

    footer_line << "Toplam " << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_tutar )).toString() );
    ADD_ROW_TO_TABLE ( footer_line );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_RAPORU::FILL_RAPOR_SATIRLARI();
***************************************************************************************/


void IRS_IRSALIYE_LISTESI_RAPORU::FILL_RAPOR_SATIRLARI()
{

    QString where_query = "irsaliye_mi_stok_fisi_mi  = :irsaliye_mi_stok_fisi_mi  "
                        "AND fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi";

    QStringList where_list;

    if ( M_IRS_RV->cari_hesap_id NE -1 ) {
        where_query += QString(" AND cari_hesap_id = :%1").arg(where_list.size());
        where_list << QVariant(M_IRS_RV->cari_hesap_id).toString();
    }
    if ( M_IRS_RV->depo_id NE -1 ) {
        where_query += QString (" AND depo_id = :%1").arg(where_list.size());
        where_list << QVariant(M_IRS_RV->depo_id).toString();
    }
    if ( M_IRS_RV->fatura_durumu EQ ENUM_FATURALANMIS ) {
        where_query += QString (" AND irsaliye_faturalastirildi_mi = :%1").arg(where_list.size());
        where_list << "1";

    }
    if ( M_IRS_RV->fatura_durumu EQ ENUM_FATURALANMAMIS ) {
        where_query += QString (" AND irsaliye_faturalastirildi_mi = :%1").arg(where_list.size());
        where_list << "0";
    }

    if ( M_IRS_RV->irsaliye_durumu EQ ENUM_ALIS_IRSALIYESI ) {
        where_query += QString (" AND irsaliye_alis_satis_turu = :%1").arg(where_list.size());
        where_list << "0";

    }
    else if ( M_IRS_RV->irsaliye_durumu EQ ENUM_SATIS_IRSALIYESI ) {
        where_query += QString (" AND irsaliye_alis_satis_turu = :%1").arg(where_list.size());
        where_list << "1";

    }
    else if ( M_IRS_RV->irsaliye_durumu EQ ENUM_IPTAL_IRSALIYELERI ) {
        where_query += QString (" AND irsaliye_faturalastirildi_mi = :%1").arg(where_list.size());
        where_list << "0";
    }

    SQL_QUERY   query(DB);
    //SQL_QUERY   s_query(DB);
    QStringList RAPOR_SATIRI;
    QString     cari_hesap_bilgisi = "";

    QString     depo_kodu ;
    QString     depo_adi ;
    QString     depo_kodu_adi;
    int irsaliye_durumu = -1;

    query.PREPARE_SELECT("stk_fisler" , "fis_tarihi, fis_no, belge_seri, belge_numarasi, aciklama, cari_hesap_id , "
                         "depo_id, irsaliye_faturalastirildi_mi, irsaliye_tutari" ,where_query);
    query.SET_VALUE(":irsaliye_mi_stok_fisi_mi", ENUM_IRSALIYE );
    query.SET_VALUE(":bas_tarihi"             , M_IRS_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    query.SET_VALUE(":bts_tarihi"             , M_IRS_RV->bitis_tarihi.toString("yyyy.MM.dd"));

    for ( int i = 0 ; i < where_list.size() ; i++ ) {
        query.SET_VALUE(QString(":%1").arg(i) , where_list.at(i));
    }

    if ( query.SELECT("fis_tarihi ASC,fis_no ASC") EQ 0 ) {
        return;
    }

    while ( query.NEXT() ) {

        RAPOR_SATIRI.clear();

        int cari_hesap_id = query.VALUE("cari_hesap_id").toInt();

        QString cari_hesap_kodu, cari_hesap_ismi;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

        if ( result NE 0 ) {
            cari_hesap_bilgisi = cari_hesap_kodu + "  " + cari_hesap_ismi;
        }

        int depo_id = query.VALUE("depo_id").toInt();

        SUBE_GET_DEPO_KODU_ADI(depo_id , &depo_kodu , &depo_adi);

        depo_kodu_adi = depo_kodu + "  " + depo_adi;

        int irsaliye_faturalandirildi_mi = query.VALUE("irsaliye_faturalastirildi_mi").toInt();

        if ( irsaliye_faturalandirildi_mi EQ 1 ) {
            irsaliye_durumu = ENUM_FATURALANMIS;
        }
        else {
            irsaliye_durumu = ENUM_FATURALANMAMIS;
        }

        m_footer_tutar     += query.VALUE ( "irsaliye_tutari" ).toDouble();

        RAPOR_SATIRI.append(query.VALUE("fis_tarihi").toDate().toString("dd MMMM yyyy") + "\t" + query.VALUE("fis_no").toString() + "\t"
                            + query.VALUE("belge_seri").toString() + " " + query.VALUE("belge_numarasi").toString()
                            + "\t" + query.VALUE("aciklama").toString() + "\t" + cari_hesap_bilgisi + "\t" +
                            depo_kodu_adi + "\t" + IRS_GET_FATURA_DURUMU_STRING(irsaliye_durumu) + "\t" +
                            VIRGUL_EKLE ( query.VALUE ( "irsaliye_tutari" ).toString(), 2 ) );

        M_RAPOR_SATIRLARI << RAPOR_SATIRI;

    }

}
