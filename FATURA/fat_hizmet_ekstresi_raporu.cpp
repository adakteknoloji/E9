#include "fat_hizmet_ekstresi_raporu_class.h"
#include "fat_struct.h"
#include "fat_enum.h"
#include "adak_utils.h"
#include "yonetim.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_console_utils.h"
#include "e9_console_utils.h"


extern ADAK_SQL *         DB;


/**************************************************************************************
                   GET_FAT_HIZMET_EKSTRESI_RAPORU
***************************************************************************************/

QString GET_FAT_HIZMET_EKSTRESI_RAPORU (QString rapor_ismi ,FATURA_RAPOR_STRUCT * P_FAT_RV)
{
    FAT_HIZMET_EKSTRESI_RAPORU  *  F = new FAT_HIZMET_EKSTRESI_RAPORU( rapor_ismi , P_FAT_RV);
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_RAPORU::FAT_HIZMET_EKSTRESI_RAPORU
***************************************************************************************/

FAT_HIZMET_EKSTRESI_RAPORU::FAT_HIZMET_EKSTRESI_RAPORU( QString rapor_ismi , FATURA_RAPOR_STRUCT * P_FAT_RV) : REPORT_KERNEL (rapor_ismi)
{
    SET_HEIGHTS ( 30, 30 );

    M_FAT_RV = P_FAT_RV;

    m_max_line_count  =  GET_MAX_LINE_COUNT ( 14 );

    m_yazdirilacak_satir_bulundu   = true;

    FILL_RAPOR_SATIRLARI();

    m_tum_rapor_satirlari_basildi  = false;
    m_hizmete_ait_islemler_basildi = false;


    m_last_satir_indisi       = 0;
    m_yazdirilan_satir_sayisi = 0;

    m_footer_toplam = 0;



    if ( M_FAT_RV->tek_hizmet_kodu.isEmpty() EQ true ) {

        QString urun_adi;

        STK_GET_URUN_ID_ADI(M_FAT_RV->bas_hizmet_kodu ,NULL , &urun_adi);

        m_rapor_kriterleri_1.append(M_FAT_RV->bas_hizmet_kodu + "  " + urun_adi);

        STK_GET_URUN_ID_ADI(M_FAT_RV->bts_hizmet_kodu ,NULL , &urun_adi);

        m_rapor_kriterleri_1.append(" - " + M_FAT_RV->bts_hizmet_kodu + " " + urun_adi);

        m_rapor_kriterleri_1.append(ADD_HTML_SPACE("  ") + "Aralığındaki Hizmetler");
    }
    if ( M_FAT_RV->nakli_yekun EQ true ) {
        m_rapor_kriterleri_2.append(" Nakli Yekün - ");
        m_rapor_kriterleri_2.append(STK_GET_HIZMET_TURU_STRING(M_FAT_RV->hizmet_turu));
    }
    else {
        m_rapor_kriterleri_2.append(STK_GET_HIZMET_TURU_STRING(M_FAT_RV->hizmet_turu));
    }
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_RAPORU::GET_HEADER
***************************************************************************************/

QString FAT_HIZMET_EKSTRESI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=LEFT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()<< "" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>HİZMET EKSTRESİ" << "");

    ADD_ROW_TO_TABLE     ( QStringList() << "" << M_FAT_RV->baslangic_tarihi.toString("dd MMMM yyyy dddd") + " - "
                           + M_FAT_RV->bitis_tarihi.toString("dd MMMM yyyy dddd") << "");

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "" << m_rapor_kriterleri_1);

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "" << m_rapor_kriterleri_2);

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_RAPORU::GET_BODY
***************************************************************************************/

QString FAT_HIZMET_EKSTRESI_RAPORU::GET_BODY()
{
    QStringList RAPOR_SATIRI;
    QString hizmet_kodu_adi;
    QStringList HIZMET;

    if ( m_yazdirilacak_satir_bulundu EQ false ) {

        if ( M_FAT_RV->nakli_yekun EQ true ) {

            SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

            SET_TD_TAG_STRING    ( QStringList() << "WIDTH=15%" << "WIDTH=8%"<< "WIDTH=67%" << "WIDTH=10%" );

            if ( M_FAT_RV->hizmet_turu EQ ENUM_ALINAN_HIZMET ) {
                CREATE_TABLE         ( QStringList()<< "<b>Fiş Tarihi" << "<b>Fiş No" << "<b>Açıklama"
                                   << "<b>Giren Miktar" ,1,13);
            }
            else {
                CREATE_TABLE         ( QStringList()<< "<b>Fiş Tarihi" << "<b>Fiş No" << "<b>Açıklama"
                                   << "<b>Çıkan Miktar" ,1,13);
            }

            SET_TD_TAG_STRING(QStringList() << "" << "" << "" << "ALIGN=RIGHT" );

            for ( ; ; ) {

                m_footer_toplam = 0;

                if ( m_last_satir_indisi EQ M_HIZMETLER.size() ) {
                    M_FAT_RV->nakli_yekun = false;
                    break;
                }

                HIZMET = M_HIZMETLER.at(m_last_satir_indisi).split("\t");

                PRINT_NAKLI_YEKUN(QVariant(HIZMET.at(0)).toInt());

                m_last_satir_indisi++;

                hizmet_kodu_adi = HIZMET.at(1);

                return hizmet_kodu_adi.append(GET_TABLE_HTML_STRING());

            }
        }
        return NULL;
    }

    int hizmet_id         = 0;
    int current_hizmet_id = 0;

    if ( m_tum_rapor_satirlari_basildi EQ true ) {
        return NULL;
    }

    if ( m_hizmete_ait_islemler_basildi EQ true ) {
        m_hizmete_ait_islemler_basildi = false;
        m_footer_toplam = 0;
        hizmet_kodu_adi.clear();
    }

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=15%" << "WIDTH=8%"<< "WIDTH=67%" << "WIDTH=10%" );

    if ( M_FAT_RV->hizmet_turu EQ ENUM_ALINAN_HIZMET ) {
        CREATE_TABLE         ( QStringList()<< "<b>Fiş Tarihi" << "<b>Fiş No" << "<b>Açıklama"
                           << "<b>Giren Miktar" ,1,13);
    }
    else {
        CREATE_TABLE         ( QStringList()<< "<b>Fiş Tarihi" << "<b>Fiş No" << "<b>Açıklama"
                           << "<b>Çıkan Miktar" ,1,13);
    }

    SET_TD_TAG_STRING(QStringList() << "" << "" << "" << "ALIGN=RIGHT" );

    m_yazdirilan_satir_sayisi = 0;

    for ( ; ; ) {


        if ( m_last_satir_indisi EQ M_YAZDIRILACAK_SATIRLAR.size() ) {
            m_tum_rapor_satirlari_basildi = true;
            break;
        }

        RAPOR_SATIRI = M_YAZDIRILACAK_SATIRLAR.at(m_last_satir_indisi).split("\t");

        hizmet_id       = QVariant(RAPOR_SATIRI.at(0)).toInt();


        if ( current_hizmet_id EQ 0 ) {
            current_hizmet_id = hizmet_id;
        }

        if ( current_hizmet_id NE hizmet_id ) {
            m_hizmete_ait_islemler_basildi = true;
            break;
        }

        if ( M_FAT_RV->nakli_yekun EQ true ) {

            if ( m_yazdirilan_satir_sayisi EQ 0 ) {
                PRINT_NAKLI_YEKUN(hizmet_id);
            }
        }

        hizmet_kodu_adi = RAPOR_SATIRI.at(1);

        if ( QVariant(RAPOR_SATIRI.at(5)).toInt() NE 0 ) {
            //Satirdaki urun miktari sifir degilse

            ADD_ROW_TO_TABLE(QStringList() << RAPOR_SATIRI.at(2) << RAPOR_SATIRI.at(3)
                             <<RAPOR_SATIRI.at(4) << RAPOR_SATIRI.at(5));

            m_yazdirilan_satir_sayisi++;
        }

        m_last_satir_indisi++;

        m_footer_toplam += QVariant(RAPOR_SATIRI.at(5)).toInt();

        if ( m_yazdirilan_satir_sayisi EQ m_max_line_count ) {
            break;
        }
    }

    return hizmet_kodu_adi.append(GET_TABLE_HTML_STRING());
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString FAT_HIZMET_EKSTRESI_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING("<TABLE style=\"width:100%; font-size:75%; font-weight:bold\" BORDER=0>");
    SET_TD_TAG_STRING(QStringList() << "style=\"width:90%;\""<< "style=\"width:10%;\" ALIGN=RIGHT");

    CREATE_TABLE (QStringList()<< "" << "" ,0,0);

    ADD_ROW_TO_TABLE(QStringList() << "Toplam" << QVariant(m_footer_toplam).toString());


    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_RAPORU::FILL_RAPOR_SATIRLARI
***************************************************************************************/

void FAT_HIZMET_EKSTRESI_RAPORU::FILL_RAPOR_SATIRLARI()
{
    QString         hizmet_kodu_adi;
    int             hizmet_id;

    int hizmet_turu = M_FAT_RV->hizmet_turu;

    QList < int > HIZMET_ID_LIST;

    if ( M_FAT_RV->tek_hizmet_kodu.isEmpty() EQ false ) {

        HIZMET_ID_LIST = STK_GET_URUN_HIZMET_ID_LIST(M_FAT_RV->tek_hizmet_kodu ,"",hizmet_turu);
    }
    else {

        HIZMET_ID_LIST = STK_GET_URUN_HIZMET_ID_LIST(M_FAT_RV->bas_hizmet_kodu , M_FAT_RV->bts_hizmet_kodu,hizmet_turu);
    }


    QStringList HIZMET;

    for ( int i = 0 ; i < HIZMET_ID_LIST.size() ; i++ )
    {

        hizmet_id       = HIZMET_ID_LIST.at(i);

        QString hizmet_kodu,hizmet_adi;

        STK_GET_URUN_KODU_ADI(hizmet_id ,&hizmet_kodu , &hizmet_adi);

        hizmet_kodu_adi = "<p align=center style = \"font-size:85%; font-weight:bold;\">" +
                        hizmet_kodu + ADD_HTML_SPACE("    ") + hizmet_adi + "</p>";
        HIZMET.clear();

        HIZMET.append(QVariant(hizmet_id).toString() + "\t" + hizmet_kodu_adi);

        GET_HIZMET_ISLEMLERI(hizmet_id , hizmet_kodu_adi);

        M_HIZMETLER << HIZMET;

    }

    if ( M_YAZDIRILACAK_SATIRLAR.size() EQ 0 ) {
        m_yazdirilacak_satir_bulundu = false;
    }
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_RAPORU::GET_HIZMET_ISLEMLERI
***************************************************************************************/

void FAT_HIZMET_EKSTRESI_RAPORU::GET_HIZMET_ISLEMLERI(int hizmet_id, QString hizmet_kodu_adi)
{
    SQL_QUERY query(DB);

    QList< int > hizmet_id_list = STK_GET_URUN_HIZMET_ID_LIST( "","", M_FAT_RV->hizmet_turu );

    QStringList YAZDIRILACAK_SATIR;

    QString where_str = "fat_faturalar.fis_id = fat_fatura_satirlari.fis_id "
                        "AND urun_id = :urun_id  "
                        "AND fatura_tarihi BETWEEN :fat_bas_tarihi AND :fat_bts_tarihi ";

    query.PREPARE_SELECT("fat_faturalar,fat_fatura_satirlari" , "fatura_tarihi,fis_no , aciklama ,"
                         "satirdaki_urun_miktari, urun_id",
                         where_str);

    query.SET_VALUE(":fat_bas_tarihi" , M_FAT_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    query.SET_VALUE(":fat_bts_tarihi" , M_FAT_RV->bitis_tarihi.toString("yyyy.MM.dd"));

    if ( M_FAT_RV->fatura_alis_satis_turu NE -1 ) {
        query.AND_EKLE  ( " fatura_alis_satis_turu = :fatura_alis_satis_turu");
        query.SET_VALUE ( ":fatura_alis_satis_turu", M_FAT_RV->fatura_alis_satis_turu );
    }

    query.SET_VALUE(":urun_id" , hizmet_id);

    if ( query.SELECT("fat_faturalar.fatura_tarihi ASC , fat_faturalar.fis_no ASC") EQ 0 ) {
        return;
    }

    while ( query.NEXT() ) {

        if ( hizmet_id_list.contains( query.VALUE( "urun_id").toInt()) EQ false ) {
            continue;
        }
        YAZDIRILACAK_SATIR.clear();

        YAZDIRILACAK_SATIR.append(QVariant(hizmet_id).toString() + "\t" +
                hizmet_kodu_adi + "\t" + query.VALUE(0).toDate().toString("dd MMMM yyyy")
                +"\t" + query.VALUE(1).toString() + "\t" + query.VALUE(2).toString()
                +"\t" + query.VALUE(3).toString());

        M_YAZDIRILACAK_SATIRLAR << YAZDIRILACAK_SATIR;
    }
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN
***************************************************************************************/

void FAT_HIZMET_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN(int hizmet_id)
{
    SQL_QUERY query(DB);

    QString where_str = " fatura_turu = :fatura_turu "
                        "AND fat_faturalar.fis_id = fat_fatura_satirlari.fis_id "
                        "AND urun_id = :urun_id AND fatura_tarihi < :fatura_tarihi ";

    query.PREPARE_SELECT("fat_faturalar , fat_fatura_satirlari" , "satirdaki_urun_miktari" , where_str);

    if ( M_FAT_RV->hizmet_turu EQ ENUM_ALINAN_HIZMET ) {
        query.SET_VALUE(":fatura_turu" , ENUM_FAT_HIZMET_ALIS_FATURASI);
    }
    else {
        query.SET_VALUE(":fatura_turu" , ENUM_FAT_HIZMET_SATIS_FATURASI);
    }
    query.SET_VALUE(":urun_id"       , hizmet_id );
    query.SET_VALUE(":fatura_tarihi" , M_FAT_RV->baslangic_tarihi.toString("yyyy.MM.dd"));

    double nakli_yekun_miktari = 0;

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() ) {

            nakli_yekun_miktari += query.VALUE(0).toInt();
        }
    }

    ADD_ROW_TO_TABLE(QStringList() << "" << "" << "Nakli Yekün" << QVariant(nakli_yekun_miktari).toString() );


    m_yazdirilan_satir_sayisi++;

    m_footer_toplam = nakli_yekun_miktari;

}






