#include <cmath>
#include "adak_utils.h"
#include "fat_ba_bs_formu_listesi_raporu_class.h"
#include "yonetim.h"
#include "e9_console_utils.h"
 
#include "fat_console_utils.h"
#include "fat_struct.h"
#include "fat_enum.h"
#include "cari_console_utils.h"
#include "prk_console_utils.h"


extern ADAK_SQL *         DB;

/**************************************************************************************
                   GET_FAT_BA_BS_FORMU_LISTESI_RAPORU
***************************************************************************************/

QString GET_FAT_BA_BS_FORMU_LISTESI_RAPORU ( FATURA_RAPOR_STRUCT * P_FAT_RAPOR )
{
    FAT_BA_BS_FORMU_LISTESI_RAPORU  *  F = new FAT_BA_BS_FORMU_LISTESI_RAPORU( P_FAT_RAPOR, "BA / BS Formu Listesi Raporu" );
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_RAPORU::FAT_BA_BS_FORMU_LISTESI_RAPORU
***************************************************************************************/

FAT_BA_BS_FORMU_LISTESI_RAPORU::FAT_BA_BS_FORMU_LISTESI_RAPORU( FATURA_RAPOR_STRUCT * P_FAT_RAPOR, QString rapor_basligi ) : REPORT_KERNEL (rapor_basligi)
{
    SET_HEIGHTS ( 30, 30 );

    M_FAT_RV = P_FAT_RAPOR;

    m_max_line_count  =  GET_MAX_LINE_COUNT ( 14 );


    m_rapor_kriterleri_1 = M_FAT_RV->baslangic_tarihi.toString("dd MMMM yyyy") + " - " + M_FAT_RV->bitis_tarihi.toString("dd MMMM yyyy");

    if ( M_FAT_RV->cari_hesap_id NE -1 ) {

        QString cari_hesap_kodu, cari_hesap_ismi;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FAT_RV->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

        if ( result NE 0 ) {
            m_rapor_kriterleri_1.append(" - Cari Hesap : " + cari_hesap_kodu + "  " + cari_hesap_ismi );
        }
    }

    if ( M_FAT_RV->max_tutar NE 0.00 ) {
        m_rapor_kriterleri_1.append( "  - Tutar : " +  QVariant( M_FAT_RV->max_tutar ).toString() );
    }
    if ( M_FAT_RV->max_tutar EQ 0.00 ) {
        M_FAT_RV->max_tutar = 5000.00;
    }

    m_baslangic_tarihi = M_FAT_RV->baslangic_tarihi.toString("dd MMMM yyyy");
    m_bitis_tarihi     = M_FAT_RV->bitis_tarihi.toString("dd MMMM yyyy");

    m_tum_rapor_satirlari_basildi = false;
    m_last_satir_indisi           = 0;

    FILL_RAPOR_SATIRLARI();

}

/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_RAPORU::GET_HEADER
***************************************************************************************/

QString FAT_BA_BS_FORMU_LISTESI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()<< "" << GET_REPORT_TOP_RIGHT_HEADER() );

    if ( M_FAT_RV->fatura_alis_satis_turu EQ ENUM_BA_FORMU ) {
        ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>BA Formu Listesi" << "");
    }
    else {
        ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>BS Formu Listesi" << "");
    }

    ADD_ROW_TO_TABLE     ( QStringList() << "" << m_rapor_kriterleri_1 << "");


   return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_RAPORU::GET_BODY
***************************************************************************************/

QString FAT_BA_BS_FORMU_LISTESI_RAPORU::GET_BODY()
{

    if ( m_tum_rapor_satirlari_basildi EQ true ) {
        return NULL;
    }

    int sayfaya_yazdirilan_satir_sayisi = 0;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=5%" << "WIDTH=15%"<< "WIDTH=10%" << "WIDTH=10%" << "WIDTH=10%"
                           << "WIDTH=5%" << "WIDTH=15%" );

    CREATE_TABLE         ( QStringList()<< "Sıra No" << "<b>Şahıs / Firma" << "<b>Ülke" << "<b>Vergi No"<<"<b>TC No"
                           << "<b>Belge Sayısı" << "<b>Mal ve Hizmetlerin \n Toplam Bedeli ",1,13);

    SET_TD_TAG_STRING    ( QStringList()<< "" << "" << "" << "" << ""<< "" <<"ALIGN=RIGHT");


    QStringList RAPOR_SATIRI;


    for ( ; ; ) {

        if ( m_last_satir_indisi EQ M_RAPOR_SATIRLARI.size() ) {
            m_tum_rapor_satirlari_basildi =  true;
            break;
        }

        RAPOR_SATIRI = M_RAPOR_SATIRLARI.at(m_last_satir_indisi).split("\t");

        ADD_ROW_TO_TABLE (QStringList() << RAPOR_SATIRI.at(0) << RAPOR_SATIRI.at(1) << RAPOR_SATIRI.at(2)
                           << RAPOR_SATIRI.at(3) << RAPOR_SATIRI.at(4) << RAPOR_SATIRI.at(5)
                           << RAPOR_SATIRI.at(6));

        sayfaya_yazdirilan_satir_sayisi++;
        m_last_satir_indisi++;

        if ( sayfaya_yazdirilan_satir_sayisi EQ m_max_line_count ) {
            break;
        }
    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString FAT_BA_BS_FORMU_LISTESI_RAPORU::GET_FOOTER()
{
    return NULL;
}

/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_RAPORU::FILL_RAPOR_SATIRLARI();
***************************************************************************************/


void FAT_BA_BS_FORMU_LISTESI_RAPORU::FILL_RAPOR_SATIRLARI()
{

    SQL_QUERY   query(DB);

    QString where_str = "fatura_tarihi BETWEEN :bas_tarihi AND :bts_tarihi ";

    query.PREPARE_SELECT("fat_faturalar", "cari_hesap_id, SUM( brut_tutar ) , "
                         "count( cari_hesap_id ) " , where_str, "cari_hesap_id" );

    query.SET_VALUE(":bas_tarihi"       , M_FAT_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    query.SET_VALUE(":bts_tarihi"       , M_FAT_RV->bitis_tarihi.toString("yyyy.MM.dd"));

    if ( M_FAT_RV->cari_hesap_id NE -1 ) {
        query.AND_EKLE  ( "cari_hesap_id = :cari_hesap_id" );
        query.SET_VALUE(":cari_hesap_id" , M_FAT_RV->cari_hesap_id);
    }
    if ( M_FAT_RV->fatura_alis_satis_turu NE -1 ) {
        query.AND_EKLE  ( "fatura_alis_satis_turu = :fatura_alis_satis_turu" );
        query.SET_VALUE(":fatura_alis_satis_turu" , M_FAT_RV->fatura_alis_satis_turu);
    }

    query.AND_EKLE  ( "iptal_mi = :iptal_mi");
    query.SET_VALUE ( ":iptal_mi",  0 );

    QStringList RAPOR_SATIRI;
    QString     cari_hesap_bilgisi = "";

    int sira_no = 0;

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {

            double brut_tutar = query.VALUE(1).toDouble();

            if ( brut_tutar < M_FAT_RV->max_tutar ) {
                continue;
            }

            RAPOR_SATIRI.clear();
            int cari_hesap_id = query.VALUE("cari_hesap_id").toInt();

            QString cari_hesap_kodu, cari_hesap_ismi, vergi_no, kimlik_no, ulke;

            CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

            int result = CARI_GET_VERGI_NO_KIMLIK_NO_ULKE( cari_hesap_id, &vergi_no, &kimlik_no, &ulke );

            if ( result NE 0 ) {
                cari_hesap_bilgisi = cari_hesap_kodu + "  " + cari_hesap_ismi;
                vergi_no           = vergi_no;
                kimlik_no          = kimlik_no;
                ulke               = ulke;
            }

            if ( M_FAT_RV->fatura_alis_satis_turu EQ ENUM_BA_FORMU ) {
                if ( ulke NE QObject::tr( "Türkiye" ) ) {
                    vergi_no    = "1111111111";
                }
            }
            else {
                if ( ulke NE QObject::tr( "Türkiye" ) ) {
                    vergi_no    = "2222222222";
                }
            }

            sira_no++;

            double toplam_tutar = ROUND_DOWN( query.VALUE ( 1 ).toDouble() );

            RAPOR_SATIRI.append( QString::number( sira_no )
                                + "\t" + cari_hesap_bilgisi
                                + "\t" + ulke
                                + "\t" + vergi_no
                                + "\t" + kimlik_no
                                + "\t" + query.VALUE ( 2 ).toString() +
                                + "\t" + VIRGUL_EKLE ( QVariant ( floor( toplam_tutar ) ).toString(), 0 ) );

            M_RAPOR_SATIRLARI << RAPOR_SATIRI;
        }

    }
    // perakende modulu icin kesilen fatura
    // Fatura alis satis eklendikten sonra yapilacak

    QList < double > kdv_haric_toplam_tutarlar = PRK_GET_FIS_KDV_HARIC_TOPLAM_TUTAR( M_FAT_RV->baslangic_tarihi.toString("yyyy.MM.dd"), M_FAT_RV->bitis_tarihi.toString("yyyy.MM.dd"),
                                                                                     M_FAT_RV->max_tutar, M_FAT_RV->fatura_alis_satis_turu );

    if ( kdv_haric_toplam_tutarlar.size() NE 0 ) {

        for ( int i = 0; i < kdv_haric_toplam_tutarlar.size(); i++ ) {

            RAPOR_SATIRI.clear();
            sira_no++;
            RAPOR_SATIRI.append( QString::number( sira_no )
                                + "\t" + "Perakende"
                                + "\t" + ""
                                + "\t" + ""
                                + "\t" + ""
                                + "\t" + "1" +
                                + "\t" + VIRGUL_EKLE ( QVariant( kdv_haric_toplam_tutarlar.at(i) ).toString(), 2 ) );

            M_RAPOR_SATIRLARI << RAPOR_SATIRI;

        }

    }
}













