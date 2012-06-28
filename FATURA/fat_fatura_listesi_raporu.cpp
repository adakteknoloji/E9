#include "adak_utils.h"
#include "fat_fatura_listesi_raporu_class.h"
#include "yonetim.h"
#include "stok_console_utils.h"
#include "fat_console_utils.h"
#include "fat_struct.h"
#include "fat_enum.h"
#include "cari_console_utils.h"
#include "e9_console_utils.h"
#include "e9_enum.h"


extern ADAK_SQL *         DB;

/**************************************************************************************
                   GET_FAT_FATURA_LISTESI_RAPORU
***************************************************************************************/

QString GET_FAT_FATURA_LISTESI_RAPORU ( FATURA_RAPOR_STRUCT * P_FAT_RAPOR )
{
    FAT_FATURA_LISTESI_RAPORU  *  F = new FAT_FATURA_LISTESI_RAPORU( P_FAT_RAPOR, "Fatura Listesi Raporu" );
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   FAT_FATURA_LISTESI_RAPORU::FAT_FATURA_LISTESI_RAPORU
***************************************************************************************/

FAT_FATURA_LISTESI_RAPORU::FAT_FATURA_LISTESI_RAPORU( FATURA_RAPOR_STRUCT * P_FAT_RAPOR, QString rapor_basligi ) : REPORT_KERNEL (rapor_basligi)
{
    SET_HEIGHTS ( 30, 30 );

    SET_ORIENTATION( QPrinter::Landscape );

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

    if ( M_FAT_RV->irsaliye_durumu NE -1 ) {
        m_rapor_kriterleri_1.append(" - " + FAT_GET_FAT_IRSALIYE_DURUMU_STRING( M_FAT_RV->irsaliye_durumu ) );
    }
    if ( M_FAT_RV->min_tutar NE 0.00 OR M_FAT_RV->max_tutar NE 0.00 ) {

        m_rapor_kriterleri_1.append( " - Tutar Aralığı : " +  VIRGUL_EKLE( QVariant ( M_FAT_RV->min_tutar ).toString() ) + " - " + VIRGUL_EKLE( QVariant( M_FAT_RV->max_tutar ).toString() ) );
    }

    double max_tutar = 999999999.99;
    if ( M_FAT_RV->min_tutar EQ 0.00 ) {
        M_FAT_RV->min_tutar = 0.00;
    }
    if ( M_FAT_RV->max_tutar EQ 0.00 ) {
        M_FAT_RV->max_tutar = max_tutar;
    }

    m_baslangic_tarihi = M_FAT_RV->baslangic_tarihi.toString("dd MMMM yyyy");
    m_bitis_tarihi     = M_FAT_RV->bitis_tarihi.toString("dd MMMM yyyy");

    m_tum_rapor_satirlari_basildi = false;
    m_last_satir_indisi           = 0;

    m_footer_vht                = 0.00;
    m_footer_kdv                = 0.00;
    m_footer_otv                = 0.00;
    m_footer_tutari             = 0.00;

    FILL_RAPOR_SATIRLARI();

}

/**************************************************************************************
                   FAT_FATURA_LISTESI_RAPORU::GET_HEADER
***************************************************************************************/

QString FAT_FATURA_LISTESI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:75%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()<< "" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>FATURA  LİSTESİ" << "");

    ADD_ROW_TO_TABLE     ( QStringList() << "" << m_rapor_kriterleri_1 << "");


   return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   FAT_FATURA_LISTESI_RAPORU::GET_BODY
***************************************************************************************/

QString FAT_FATURA_LISTESI_RAPORU::GET_BODY()
{

    if ( m_tum_rapor_satirlari_basildi EQ true ) {
        return NULL;
    }

    int sayfaya_yazdirilan_satir_sayisi = 0;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

    SET_TD_TAG_STRING    ( QStringList()
                           << "WIDTH=9%"                  // fatura tarihi
                           << "WIDTH=5%"                  // Fis no
                           << "WIDTH=9%"                  // belge seri
                           << "WIDTH=15%"                 // Aciklama
                           << "WIDTH=26%"                 // cari hesap kodu
                           << "WIDTH=2% ALIGN=CENTER"     // Alis Satis Ai Si
                           << "WIDTH=6% ALIGN=CENTER"     // Acik kapali
                           << "WIDTH=8%"                  // V.H.T
                           << "WIDTH=6%"                  // KDV
                           << "WIDTH=6%"                  // OTV
                           << "WIDTH=8%" );               // Tutari

    CREATE_TABLE         ( QStringList()<< "<b>Fatura Tarihi" << "<b>Fiş No" << "<b>Belge Seri / No"<<"<b>Açıklama"
                           << "<b>Cari Hesap Kodu/Adı" << "" << "" << "<b> V.H.T" << "<b>KDV" << "<b>ÖTV"  << "<b>Tutarı",1,13);

    SET_TD_TAG_STRING    ( QStringList()<< "" << "" << "" << "" << "" << "ALIGN=CENTER" << "ALIGN=CENTER" << "ALIGN=RIGHT" << "ALIGN=RIGHT" << "ALIGN=RIGHT" <<"ALIGN=RIGHT");


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
                                        << RAPOR_SATIRI.at(3).mid( 0, 25 ) + ".."
                                        << RAPOR_SATIRI.at(4).mid( 0, 40 ) + ".."
                                        << RAPOR_SATIRI.at(5)
                                        << RAPOR_SATIRI.at(6)
                                        << RAPOR_SATIRI.at(7)
                                        << RAPOR_SATIRI.at(8)
                                        << RAPOR_SATIRI.at(9)
                                        << RAPOR_SATIRI.at(10));

        sayfaya_yazdirilan_satir_sayisi++;
        m_last_satir_indisi++;

        if ( sayfaya_yazdirilan_satir_sayisi EQ m_max_line_count ) {
            break;
        }
    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   FAT_FATURA_LISTESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString FAT_FATURA_LISTESI_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:75%;font-weight:bold;\" BORDER = 0 >");

    SET_TD_TAG_STRING ( QStringList() << "style = \"width:72%;\""
                                      << "style = \"width:8%;\" ALIGN = RIGHT"
                                      << "style = \"width:6%;\" ALIGN = RIGHT"
                                      << "style = \"width:6%;\" ALIGN = RIGHT"
                                      << "style = \"width:8%;\" ALIGN = RIGHT" );


    CREATE_TABLE( QStringList() << "" << "" << "" << "" << "",0,0);

    QStringList footer_line;

    footer_line << "Toplam " << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_vht )).toString() )
                             << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_kdv )).toString() )
                             << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_otv )).toString() )
                             << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_tutari )).toString());
    ADD_ROW_TO_TABLE ( footer_line );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   FAT_FATURA_LISTESI_RAPORU::FILL_RAPOR_SATIRLARI();
***************************************************************************************/


void FAT_FATURA_LISTESI_RAPORU::FILL_RAPOR_SATIRLARI()
{

    QString where_str = " fatura_tarihi BETWEEN :bas_tarihi AND :bts_tarihi ";

    SQL_QUERY   query(DB);
    QStringList RAPOR_SATIRI;
    QString     cari_hesap_bilgisi = "";

    query.PREPARE_SELECT("fat_faturalar" , "fatura_tarihi, fis_no, belge_seri, belge_numarasi, aciklama, cari_hesap_id , "
                         "fatura_irsaliyelestirildi_mi, fatura_tutari,fatura_turu, acik_kapali_fatura, "
                         "ara_toplam, isk_sonrasi_kdv_tutari, isk_sonrasi_otv_tutari,iptal_mi, fatura_alis_satis_turu, iade_faturasi_mi" , where_str);

    query.SET_VALUE ( ":bas_tarihi" , M_FAT_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );
    query.SET_VALUE ( ":bts_tarihi" , M_FAT_RV->bitis_tarihi.toString     ( "yyyy.MM.dd" ) );

    if ( M_FAT_RV->cari_hesap_id NE -1 ) {
        query.AND_EKLE  ( "cari_hesap_id = :cari_hesap_id" );
        query.SET_VALUE ( ":cari_hesap_id", M_FAT_RV->cari_hesap_id );
    }
    if ( M_FAT_RV->min_tutar NE 0.00 OR M_FAT_RV->max_tutar NE 0.00 ) {
        query.AND_EKLE  ( "fatura_tutari BETWEEN :min_tutar AND :max_tutar" );
        query.SET_VALUE ( ":min_tutar" , M_FAT_RV->min_tutar );
        query.SET_VALUE ( ":max_tutar" , M_FAT_RV->max_tutar );
    }
    if ( M_FAT_RV->irsaliye_durumu EQ ENUM_IRSALIYELENMIS ) {
        query.AND_EKLE  ( "fatura_irsaliyelestirildi_mi = :fatura_irsaliyelestirildi_mi" );
        query.SET_VALUE ( ":fatura_irsaliyelestirildi_mi" , 1 );

    }
    if ( M_FAT_RV->irsaliye_durumu EQ ENUM_IRSALIYELENMEMIS ) {
        query.AND_EKLE  ( "fatura_irsaliyelestirildi_mi = :fatura_irsaliyelestirildi_mi" );
        query.SET_VALUE ( ":fatura_irsaliyelestirildi_mi" , 0 );
    }

    if ( M_FAT_RV->fatura_alis_satis_turu NE -1 ) {
        query.AND_EKLE  ( "fatura_alis_satis_turu = :fatura_alis_satis_turu" );
        query.SET_VALUE ( ":fatura_alis_satis_turu" , M_FAT_RV->fatura_alis_satis_turu );
    }

    if ( M_FAT_RV->acik_kapali_fatura EQ ENUM_ACIK_FATURA ) {
        query.AND_EKLE  ( "acik_kapali_fatura = :acik_kapali_fatura AND  iptal_mi = :iptal_mi" );
        query.SET_VALUE ( ":acik_kapali_fatura", ENUM_ACIK_FATURA );
        query.SET_VALUE ( ":iptal_mi"          , 0 );
    }
    else if( M_FAT_RV->acik_kapali_fatura EQ ENUM_KAPALI_FATURA ) {
        query.AND_EKLE  ( "acik_kapali_fatura = :acik_kapali_fatura AND  iptal_mi = :iptal_mi" );
        query.SET_VALUE ( ":acik_kapali_fatura", ENUM_KAPALI_FATURA );
        query.SET_VALUE ( ":iptal_mi"          , 0 );
    }

    if ( M_FAT_RV->acik_kapali_fatura EQ ENUM_IPTAL_EDILEN_FATURALAR ) {
        query.AND_EKLE  ( "iptal_mi = :iptal_mi" );
        query.SET_VALUE ( ":iptal_mi", 1 );
    }

    if ( M_FAT_RV->fatura_turu NE -1 ) {
        query.AND_EKLE  ( "fatura_turu = :fatura_turu" );
        query.SET_VALUE ( ":fatura_turu", M_FAT_RV->fatura_turu );
    }

    if ( query.SELECT("fatura_tarihi ASC , fis_no ASC") EQ 0 ) {
        return;
    }

    while ( query.NEXT() ) {

        RAPOR_SATIRI.clear();

        int cari_hesap_id = query.VALUE( "cari_hesap_id").toInt();

        QString cari_hesap_kodu, cari_hesap_ismi;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

        if ( result NE 0 ) {
            cari_hesap_bilgisi = cari_hesap_kodu + "  " + cari_hesap_ismi;
        }

        int fatura_turu = query.VALUE("fatura_turu").toInt();

        int fatura_alis_satis_turu = query.VALUE( "fatura_alis_satis_turu").toInt();
        QString fatura_alis_satis_turu_str = "S";

        if (  fatura_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
            fatura_alis_satis_turu_str = "A";
        }

        int iade_faturasi_mi = query.VALUE( "iade_faturasi_mi" ).toInt();
        if ( iade_faturasi_mi EQ 1 ) {
            fatura_alis_satis_turu_str.append( "İ" );
        }

        QString acik_kapali_fatura = "İptal";

        if  ( query.VALUE( "iptal_mi" ).toInt() EQ 0 ) {
            if ( query.VALUE( "acik_kapali_fatura" ).toInt() EQ 1 ) {
                acik_kapali_fatura = "Kapalı";
            }
            else {
                acik_kapali_fatura = "Açık";
            }
        }

        QString aciklama = query.VALUE("aciklama").toString().trimmed();

        if( aciklama.isEmpty() EQ true ) {
            if( iade_faturasi_mi EQ ENUM_FAT_IADE ) {
                if ( fatura_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
                    aciklama = "ALIŞTAN İADE FATURALARI ";
                }
                else { //ENUM_SATIS_IRSALIYESI_FATURASI
                    aciklama = "SATIŞTAN İADE FATURALARI ";
                }
            }
            else {
                aciklama = FAT_GET_FATURA_TURLERI_STRING( fatura_turu );
            }
        }

        m_footer_vht    += query.VALUE ( "ara_toplam" ).toDouble();
        m_footer_kdv    += query.VALUE ( "isk_sonrasi_kdv_tutari" ).toDouble();
        m_footer_otv    += query.VALUE ( "isk_sonrasi_otv_tutari" ).toDouble();
        m_footer_tutari += query.VALUE ( "fatura_tutari" ).toDouble();

        RAPOR_SATIRI.append( query.VALUE("fatura_tarihi").toDate().toString("dd MMMM yyyy")
                            + "\t" + query.VALUE("fis_no").toString()
                            + "\t" + query.VALUE("belge_seri").toString() + " " + query.VALUE("belge_numarasi").toString()
                            + "\t" + aciklama
                            + "\t" + cari_hesap_bilgisi +
                            + "\t" + fatura_alis_satis_turu_str +
                            + "\t" + acik_kapali_fatura +
                            + "\t" + VIRGUL_EKLE ( query.VALUE ( "ara_toplam" ).toString(), 2 )
                            + "\t" + VIRGUL_EKLE ( query.VALUE ( "isk_sonrasi_kdv_tutari" ).toString(), 2 )
                            + "\t" + VIRGUL_EKLE ( query.VALUE ( "isk_sonrasi_otv_tutari" ).toString(), 2 )
                            + "\t" + VIRGUL_EKLE ( query.VALUE ( "fatura_tutari" ).toString(), 2 ) );

        M_RAPOR_SATIRLARI << RAPOR_SATIRI;
    }

}













