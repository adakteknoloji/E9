#include <QDate>
#include "adak_utils.h"
#include "cek_listesi_raporu_class.h"
#include "e9_enum.h"
#include "yonetim.h"
#include "cari_console_utils.h"
#include "cek_struct.h"
#include "cek_enum.h"
#include "cek_console_utils.h"
#include "e9_console_utils.h"
#include "dvz_console_utils.h"

extern ADAK_SQL * DB;


/**************************************************************************************
                   GET_CEK_LISTESI_RAPORU()
***************************************************************************************/

QString GET_CEK_LISTESI_RAPORU ( CEK_RAPOR_VARS * P_M_CEK_RV)
{
    CEK_LISTESI_RAPORU  *  R = new CEK_LISTESI_RAPORU ( P_M_CEK_RV);
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   CEK_LISTESI_RAPORU::CEK_LISTESI_RAPORU
***************************************************************************************/

CEK_LISTESI_RAPORU::CEK_LISTESI_RAPORU( CEK_RAPOR_VARS * P_M_CEK_RV) : REPORT_KERNEL ("CEK_LISTESI_RAPORU")
{
    SET_HEIGHTS (45,30);
    m_max_line_count             =  GET_MAX_LINE_COUNT (12);
    m_eklenen_kayit_sayisi       = 0;
    m_last_cek_indisi            = 0;
    m_toplam_tutar               = 0;
    m_cek_senet_column_header    = "";

    M_CEK_RV = P_M_CEK_RV;

    FILL_CEKLER_LISTESI();
}


/**************************************************************************************
                   CEK_LISTESI_RAPORU::GET_HEADER
***************************************************************************************/


QString CEK_LISTESI_RAPORU::GET_HEADER()
{

    QString report_header;

    switch ( M_CEK_RV->cek_senet_pozisyonu ) {
        case ENUM_PORTFOYDE :
            report_header = "Portföydeki ";
            break;
        case ENUM_BNK_TEMINAT_VERILDI :
            report_header = "Teminata Verilen ";
            break;
        case ENUM_BNK_TAHSILAT_VERILDI :
            report_header = "Tahsilata Verilen ";
            break;
        case ENUM_TAHSIL_EDILDI_NKT :
            report_header = "Nakit Tahsil Edilen ";
            break;
        case ENUM_TAHSIL_EDILDI_HESABA :
            report_header = "Hesaptan Tahsil Edilen ";
            break;
        case ENUM_SATICIYA_VERILDI  :
            report_header = "Satıcıya Verilen / Ciro Edilen ";
            break;
        case ENUM_KARSILIKSIZ :
            report_header = "Karşılıksız ";
            break;
        case ENUM_PROTESTOLU :
            report_header = "Protestolu ";
            break;
        case ENUM_MUS_IADE_EDILDI :
            report_header = "Müşteriye İade Edilen ";
            break;
        case ENUM_ODENDI_NKT :
            report_header = "Nakit Ödenen ";
            break;
        case ENUM_ODENDI_HESAPTAN :
            report_header = "Hesaptan Ödenen ";
            break;
        case ENUM_SUPHELI_ALACAK :
            report_header = "Şüpheli Alacaklardaki ";
            break;
        case ENUM_TAHSIL_EDILEMEYEN :
            report_header = "Tahsil Edilemeyen ";
            break;
        case ENUM_IPTAL :
            report_header = "İptal Edilen ";
            break;
        case ENUM_TUM_POZISYONLAR :
            report_header = "Tümü";
            break;
        default :
                report_header = " ";
            break;
    };

    switch ( M_CEK_RV->cek_senet_turu ) {
        case ENUM_MUSTERI_CEK_SENEDI :
            report_header.append ( "Müşteri Çek ve Senetleri" );
            break;
        case ENUM_KENDI_CEK_SENEDIMIZ :
            report_header.append ( "Kendi Çek ve Senetlerimiz" );
            break;
        default : //TUM CEK_SENETLER
                report_header.append ( "Çek ve Senetler" );
        break;
    };

    QString report_header_str;

    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >" );

    SET_TD_TAG_STRING ( QStringList()<<"style = \"width:40%; font-size:88%;\""<<"style = \"width:20%;font-size:110%;\" ALIGN=CENTER"
                                     << "style = \"width:40%;style = \"font-size:88%;\" ALIGN = RIGHT" );


    CREATE_TABLE     ( QStringList()<<""<<""<<"",0,0 ) ;

    ADD_ROW_TO_TABLE ( QStringList() << E9_GET_FIRMA_ISMI() << "" << GET_REPORT_TOP_RIGHT_HEADER() );

    report_header_str.append ( GET_TABLE_HTML_STRING() );

    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"style = \"width:10%; font-size:88%;\""<<"style = \"width:80%;font-size:110%;\" ALIGN=CENTER"
                                        << "style = \"width:10%;style = \"font-size:88%;\" ALIGN = RIGHT" );


    CREATE_TABLE     ( QStringList()<<""<<""<<"",0,0 );
    ADD_ROW_TO_TABLE ( QStringList() << "" << "<b>" + report_header + "</b>" << "" );
    ADD_ROW_TO_TABLE ( QStringList() << "" <<  "<p style = \"font-size:90%;\">Vade Tarihi : " + M_CEK_RV->bas_vade_tarihi.toString ( "dd MMMM yyyy dddd" ) + " - "
                     + M_CEK_RV->bts_vade_tarihi.toString ( "dd MMMM yyyy dddd" ) + "</p>"<< "" ) ;
    report_header_str.append ( GET_TABLE_HTML_STRING() );

    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:100%;\" BORDER=0 >" );

    SET_TD_TAG_STRING   ( QStringList()<<"style = \"width:40%; font-size:88%;\""<<"style = \"width:20%;font-size:110%;\" ALIGN=CENTER"
                                       << "style = \"width:40%;style = \"font-size:88%;\" ALIGN = RIGHT" );


    CREATE_TABLE  ( QStringList()<<""<<""<<"",0,0 ) ;


    QString cekin_pozisyonu = CEK_GET_CEK_SENET_POZISYONU_STRING ( M_CEK_RV->cek_senet_pozisyonu );


    if ( M_CEK_RV->tum_bankalar EQ false ) {
        ADD_ROW_TO_TABLE ( QStringList() << "<b>Pozisyonu"   + ADD_HTML_SPACE ( "  " ) + ":</b>" + ADD_HTML_SPACE("  ") + cekin_pozisyonu << "" << ""   );
        ADD_ROW_TO_TABLE ( QStringList() << "<b>Banka İsmi " + ADD_HTML_SPACE ( "  " ) + ":</b>" + ADD_HTML_SPACE("  ") + M_CEK_RV->banka_ismi << "" << "" );
    }

    if ( M_CEK_RV->alinan_cari_hesap_id NE 0 ) {
        ADD_ROW_TO_TABLE ( QStringList() << "<b>Pozisyonu" + ADD_HTML_SPACE("                     ") + ":</b>" + ADD_HTML_SPACE("  ") + cekin_pozisyonu << "" << "" );
        ADD_ROW_TO_TABLE ( QStringList() << "<b> Alınan Cari Hesap " + ADD_HTML_SPACE("     ") + ":</b>" + ADD_HTML_SPACE("  ") + CARI_FIND_HESAP_KODU(M_CEK_RV->alinan_cari_hesap_id ) +
                           ADD_HTML_SPACE ( "  " ) + CARI_FIND_HESAP_ISMI ( M_CEK_RV->alinan_cari_hesap_id ) << "" << "" );
    }

    if ( M_CEK_RV->verilen_cari_hesap_id NE 0  ) {
        if ( M_CEK_RV->alinan_cari_hesap_id EQ 0 ) {
            ADD_ROW_TO_TABLE ( QStringList() << "<b>Pozisyonu" + ADD_HTML_SPACE("  ") + ":</b>" + ADD_HTML_SPACE("  ") + cekin_pozisyonu << "" << "" );
        }
        ADD_ROW_TO_TABLE(QStringList() << "<b>Verilen Cari Hesap " + ADD_HTML_SPACE("    ") + ":</b>" + ADD_HTML_SPACE("  ") + CARI_FIND_HESAP_KODU ( M_CEK_RV->        verilen_cari_hesap_id ) +
                         ADD_HTML_SPACE("  " ) + CARI_FIND_HESAP_ISMI ( M_CEK_RV->verilen_cari_hesap_id ) << "" << "" );
    }

    if ( M_CEK_RV->tum_bankalar EQ true AND M_CEK_RV->tum_cari_hesaplar EQ true ) {
         ADD_ROW_TO_TABLE ( QStringList() << "<b>Pozisyonu" + ADD_HTML_SPACE("  ") + ":</b>" + ADD_HTML_SPACE("  ") + cekin_pozisyonu << "" << "");
    }


    return report_header_str.append ( GET_TABLE_HTML_STRING() );

}

/**************************************************************************************
                   CEK_LISTESI_RAPORU::GET_BODY
***************************************************************************************/

QString CEK_LISTESI_RAPORU::GET_BODY()
{
    QStringList headers;
    QStringList basilacak_satir;
    QStringList cek_bilgileri;

    if ( m_last_cek_indisi >= m_cekler_listesi.size() ) {
        return NULL;
    }

    if ( M_CEK_RV->cek_senet_pozisyonu EQ ENUM_TUM_POZISYONLAR ) {
        headers << "<b>Portföy No" << "<b>Çek/Senet Num."<<"<b>Vade Tarihi"<<"<b>Banka İsmi"
                <<"<b>Şubesi"<<"<b>Türü" << "<b>Pozisyonu"<<"<b>Tutar" << "<b>P. Birimi";
    }
    else {
        headers << "<b>Portföy No" << "<b>Çek/Senet Numarası"<<"<b>Vade Tarihi"<<"<b>Banka İsmi"
                <<"<b>Şubesi"<<"<b>Türü"<<"<b>Tutar" << "<b>P. Birimi";
    }



    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 1>" );

    if ( M_CEK_RV->cek_senet_pozisyonu EQ ENUM_TUM_POZISYONLAR ) {
        SET_TD_TAG_STRING    ( QStringList() << "style = \"width:9%;\"" << "style = \"width:12%;\"" << "style = \"width:14%;\""
                                             << "style = \"width:12%;\"" << "style = \"width:13%;\"" << "style = \"width:12%;\""
                                             << "style = \"width:13%;\""<< "style = \"width:10%;\"" << "style = \"width:5%;\"" );
    }
    else {
        SET_TD_TAG_STRING    ( QStringList() << "style = \"width:9%;\"" << "style = \"width:14%;\"" << "style = \"width:18%;\""
                                             << "style = \"width:16%;\"" << "style = \"width:15%;\""
                                             << "style = \"width:13%;\""<< "style = \"width:10%;\"" << "style = \"width:5%;\"" );
    }


    CREATE_TABLE ( headers,1,14 ) ;

    if ( M_CEK_RV->cek_senet_pozisyonu EQ ENUM_TUM_POZISYONLAR ) {
       SET_TD_TAG_STRING ( QStringList() << "" << "" << "" << "" << "" << "" << ""<<"ALIGN = RIGHT" << "" );
    }
    else {
       SET_TD_TAG_STRING ( QStringList() << "" << "" << "" << "" << "" << ""<<"ALIGN = RIGHT" << "" );
    }

    m_eklenen_kayit_sayisi = 0;

    for ( int i = m_last_cek_indisi ; i < m_cekler_listesi.size() ; i++ ) {
         cek_bilgileri = m_cekler_listesi.at(i).split ( "\t" );

        m_toplam_tutar += QVariant ( cek_bilgileri.at(6)).toDouble();

        basilacak_satir.clear();
        if ( M_CEK_RV->cek_senet_pozisyonu EQ ENUM_TUM_POZISYONLAR ) {
            basilacak_satir << cek_bilgileri.at(0) << cek_bilgileri.at(1) << cek_bilgileri.at(2) << cek_bilgileri.at(3)
                            << cek_bilgileri.at(4) << cek_bilgileri.at(5) <<  cek_bilgileri.at(7) << VIRGUL_EKLE(cek_bilgileri.at(6) ) << cek_bilgileri.at(8);
        }
        else {
            basilacak_satir << cek_bilgileri.at(0) << cek_bilgileri.at(1) << cek_bilgileri.at(2) << cek_bilgileri.at(3)
                            << cek_bilgileri.at(4) << cek_bilgileri.at(5) << VIRGUL_EKLE(cek_bilgileri.at(6) ) << cek_bilgileri.at(8);
        }

        ADD_ROW_TO_TABLE ( basilacak_satir );

        m_eklenen_kayit_sayisi++;
        m_last_cek_indisi++;

        if ( m_eklenen_kayit_sayisi EQ m_max_line_count) {
            break;
        }
    }
    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   CEK_LISTESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString CEK_LISTESI_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER = 0>" );
    SET_TD_TAG_STRING ( QStringList() << "style = \"width:85%;\"" << "style = \"width:10%;\"ALIGN=RIGHT" << "style = \"width:5%;\"" );
    CREATE_TABLE ( QStringList() << "" << "" << "",0,14 );

    ADD_ROW_TO_TABLE ( QStringList() << "" <<  VIRGUL_EKLE ( QVariant ( m_toplam_tutar ).toString() ) << "" );
    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   CEK_LISTESI_RAPORU::FILL_CEKLER_LISTESI
***************************************************************************************/

void CEK_LISTESI_RAPORU::FILL_CEKLER_LISTESI()
{
    SQL_QUERY select_query ( DB );
    QStringList uygun_cekler;
    QStringList uygun_cekler_alinan_cari_hesap;
    QStringList uygun_cekler_verilen_cari_hesap;



    if ( M_CEK_RV->tum_bankalar EQ false ) {

       select_query.PREPARE_SELECT ( "cek_bordrolar, cek_bordro_satirlari, cek_cekler_senetler ",
                                     "cek_bordro_satirlari.cek_senet_id",
                                     "banka_ismi = :banka_ismi AND  "
                                     "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                                     "cek_cekler_senetler.cek_senet_id = cek_bordro_satirlari.cek_senet_id AND "
                                     "cek_bordrolar.modul_id = :modul_id " );

       select_query.SET_VALUE        ( ":banka_ismi"  , M_CEK_RV->banka_ismi );
       select_query.SET_VALUE        ( ":modul_id"    , CEKSENET_MODULU );

       if ( select_query.SELECT() EQ 0 ) {
            return;
       }

       while ( select_query.NEXT() ) {
           uygun_cekler << select_query.VALUE(0).toString();
       }
    }
    if ( M_CEK_RV->tum_cari_hesaplar EQ false ) {
        if ( M_CEK_RV->verilen_cari_hesap_id NE 0 ) {
            select_query.PREPARE_SELECT ( "cek_bordrolar,cek_bordro_satirlari","cek_senet_id ",
                                          "cari_hesap_id = :cari_hesap_id AND "
                                          "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                                          "cek_bordrolar.modul_id = :modul_id" );
            select_query.SET_VALUE      ( ":cari_hesap_id" , M_CEK_RV->verilen_cari_hesap_id );
            select_query.SET_VALUE      ( ":modul_id"      , CEKSENET_MODULU );

            if ( select_query.SELECT() NE 0 ) {
                while ( select_query.NEXT()) {
                       uygun_cekler_verilen_cari_hesap << select_query.VALUE(0).toString();
                }
            }


            select_query.PREPARE_SELECT ( "cek_bordro_satirlari", "cek_senet_id",
                                          "verilen_cari_hesap_id = :verilen_cari_hesap_id ");
            select_query.SET_VALUE      ( ":verilen_cari_hesap_id" , M_CEK_RV->verilen_cari_hesap_id );

            if ( select_query.SELECT() NE 0 ) {
                while ( select_query.NEXT() ) {
                    uygun_cekler_verilen_cari_hesap << select_query.VALUE(0).toString();
                }
            }

        }
        if ( M_CEK_RV->alinan_cari_hesap_id NE 0 )  {

            QList <int > cek_senet_id_list = CARI_GET_CEK_SENET_ID_LIST( M_CEK_RV->alinan_cari_hesap_id, CARI_MODULU );

            if ( cek_senet_id_list.size() NE 0 ) {
                for ( int i = 0; i < cek_senet_id_list.size(); i++ ) {
                    uygun_cekler_alinan_cari_hesap << QVariant( cek_senet_id_list.at( i )).toString();
                }
            }
        }
    }

    select_query.PREPARE_SELECT ( "cek_cekler_senetler",
                                  "cek_senet_id , portfoy_numarasi , cek_senet_numarasi , vade_tarihi, "
                                  "banka_ismi , sube_kodu , sube_ismi , cek_senet_turu , cek_senet_tutari , "
                                  "musteri_ceki_mi , cek_senet_son_durumu,doviz_id " );

    if ( M_CEK_RV->cek_senet_pozisyonu NE ENUM_TUM_POZISYONLAR ) {
        select_query.AND_EKLE( "cek_senet_son_durumu = :cek_senet_son_durumu");
        select_query.SET_VALUE( ":cek_senet_son_durumu", M_CEK_RV->cek_senet_pozisyonu );
    }

    int cek_senet_turu = M_CEK_RV->cek_senet_turu;

    switch ( cek_senet_turu ) {
        case ENUM_MUSTERI_CEK_SENEDI :
            select_query.AND_EKLE( "musteri_ceki_mi = :musteri_ceki_mi");
            select_query.SET_VALUE( ":musteri_ceki_mi", 1 );
            break;
        case ENUM_KENDI_CEK_SENEDIMIZ:
            select_query.AND_EKLE( "musteri_ceki_mi = :musteri_ceki_mi");
            select_query.SET_VALUE( ":musteri_ceki_mi", 0 );
            break;
        default :  //TUM CEK VE SENETLER
            break;
    };

    select_query.AND_EKLE( "vade_tarihi BETWEEN :vade_tarihi_1 AND :vade_tarihi_2");
    select_query.SET_VALUE( ":vade_tarihi_1", M_CEK_RV->bas_vade_tarihi.toString ( "yyyy.MM.dd" ) );
    select_query.SET_VALUE( ":vade_tarihi_2", M_CEK_RV->bts_vade_tarihi.toString ( "yyyy.MM.dd" ) );

    if ( select_query.SELECT() EQ  0 ) {
         return;
    }

    QStringList cek_bilgileri;
    QString cek_senet_turu_str;

    while ( select_query.NEXT() ) {
       QString cek_senet_id        = select_query.VALUE(0).toString();
       if ( M_CEK_RV->tum_bankalar EQ false ) {
           if ( uygun_cekler.indexOf(cek_senet_id) EQ -1 ) {
               continue;
           }
       }
       if ( M_CEK_RV->tum_cari_hesaplar EQ false ) {
           if ( M_CEK_RV->alinan_cari_hesap_id NE 0 ) {
               if ( uygun_cekler_alinan_cari_hesap.indexOf ( cek_senet_id ) EQ -1 )  {
                   continue;
               }
           }
           if ( M_CEK_RV->verilen_cari_hesap_id NE 0 ) {
               if ( uygun_cekler_verilen_cari_hesap.indexOf ( cek_senet_id ) EQ -1 ) {
                   continue;
               }
           }
       }

       QString portfoy_no      = select_query.VALUE(1).toString();
       QString cek_senet_no    = select_query.VALUE(2).toString();
       QString vade_tarihi     = select_query.VALUE(3).toDate().toString ( "dd MMMM yyyy dddd" );
       QString banka_ismi      = select_query.VALUE(4).toString();
       QString sube_kodu       = select_query.VALUE(5).toString();
       QString sube_ismi       = select_query.VALUE(6).toString();
       int     cek_senet_turu  = select_query.VALUE(7).toInt();
       double cek_senet_tutari = select_query.VALUE(8).toDouble();
       int musteri_ceki_mi     = select_query.VALUE(9).toInt();
       int cek_senet_pozisyonu = select_query.VALUE(10).toInt();
       int doviz_id            = select_query.VALUE(11).toInt();

       if ( cek_senet_turu EQ ENUM_CEK ) {
           if ( musteri_ceki_mi EQ 1 ) {
                cek_senet_turu_str = QObject::tr ( "MÜŞTERİ ÇEKİ" );
           }
           else {
               cek_senet_turu_str = QObject::tr ( "KENDİ ÇEKİMİZ" );
           }
       }
       else {
           if ( musteri_ceki_mi EQ 1 ) {
                cek_senet_turu_str = QObject::tr ( "MÜŞTERİ SENEDİ" );
           }
           else {
               cek_senet_turu_str = QObject::tr ( "KENDİ SENEDİMİZ" );
           }
       }
       cek_bilgileri.clear();
       cek_bilgileri.append ( portfoy_no    + "\t" +
                              cek_senet_no  + "\t" +
                              vade_tarihi   + "\t" +
                              banka_ismi    + "\t" +
                              sube_kodu + "  " + sube_ismi + "\t" +
                              cek_senet_turu_str + "\t" +
                              QVariant ( ROUND ( cek_senet_tutari ) ).toString() + "\t" +
                              CEK_GET_CEK_SENET_POZISYONU_STRING(cek_senet_pozisyonu) + "\t" +
                              DVZ_GET_DOVIZ_KODU( doviz_id ));

       m_cekler_listesi << cek_bilgileri;

    }
}











