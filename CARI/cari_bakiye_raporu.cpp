#include <QDate>
#include "adak_utils.h"
#include "cari_bakiye_raporu_class.h"
#include "cari_bakiye_raporu_open.h"
#include "adres_console_utils.h"
#include "e9_console_utils.h"
#include "cari_struct.h"
#include "e9_enum.h"
#include "yonetim.h"
#include "adak_gruplar.h"
#include "cari_console_utils.h"
 

extern ADAK_SQL * DB;

/**************************************************************************************
                   GET_CARI_BAKIYE_RAPORU
***************************************************************************************/


QString GET_CARI_BAKIYE_RAPORU ( CARI_RAPOR_VARS * P_CARI_RV)
{
    CARI_BAKIYE_RAPORU  *  R = new CARI_BAKIYE_RAPORU( P_CARI_RV);
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   CARI_BAKIYE_RAPORU::CARI_BAKIYE_RAPORU
***************************************************************************************/

CARI_BAKIYE_RAPORU::CARI_BAKIYE_RAPORU( CARI_RAPOR_VARS * P_CARI_RV) : REPORT_KERNEL ( "CARI_BAKIYE_RAPORU" )

{
    SET_HEIGHTS (30,30);

    m_max_line_count =  GET_MAX_LINE_COUNT (12);

    M_CARI_RV        = P_CARI_RV;

    if ( M_CARI_RV->CARI_ID_LIST.size() NE 0 ) {

        CARI_HESAP_STRUCT   CARI_BILGILER;
        QStringList         cari_hesap;

        QList < int > cari_hesap_id_list = M_CARI_RV->CARI_ID_LIST;
        if( !( M_CARI_RV->program_id EQ E9_PROGRAMI AND M_CARI_RV->modul_id EQ CARI_MODULU ) ) {
            cari_hesap_id_list = CARI_GET_CARI_HESAP_ID_LIST( M_CARI_RV->program_id, M_CARI_RV->modul_id, M_CARI_RV->CARI_ID_LIST );
        }

        for (int i = 0; i < cari_hesap_id_list.size(); ++i) {
            CARI_SET_HESAP_STRUCT_DEFAULTS( &CARI_BILGILER );

            CARI_KART_BILGILERINI_OKU( cari_hesap_id_list.at(i), &CARI_BILGILER );
            cari_hesap.clear();
            cari_hesap.append( QVariant( cari_hesap_id_list.at(i) ).toString() + "\t" + CARI_BILGILER.cari_hesap_kodu + "\t" + CARI_BILGILER.cari_hesap_ismi + "\t");
            m_cari_hesaplar_list << cari_hesap;
        }
    }
    else {
        m_cari_hesaplar_list     =  GET_CARI_HESAPLAR_LIST();
    }

    m_last_cari_hesap_no         = 0;
    m_eklenen_kayit_sayisi       = 0;
    m_tum_hesaplar_basildi       = false;

    m_footer_borc                = 0.0;
    m_footer_alacak              = 0.0;
    m_footer_borc_bakiye         = 0.0;
    m_footer_alacak_bakiye       = 0.0;
}

/**************************************************************************************
                   CARI_BAKIYE_RAPORU::GET_HEADER
***************************************************************************************/

QString CARI_BAKIYE_RAPORU::GET_HEADER()
{

   QString baslangic_tarihi = M_CARI_RV->baslangic_tarihi.toString("dd MMMM yyyy dddd");
   QString bitis_tarihi     = M_CARI_RV->bitis_tarihi.toString("dd MMMM yyyy dddd");

   QString hesap_araligi    = "";

   if ( M_CARI_RV->bas_hesap_kodu NE M_CARI_RV->bts_hesap_kodu) {
       hesap_araligi = "Cari Hesap Aralığı : " + M_CARI_RV->bas_hesap_kodu + " - " + M_CARI_RV->bts_hesap_kodu;
   }
   else {
       hesap_araligi = "Cari Hesap: " + M_CARI_RV->bas_hesap_kodu;
   }


   QString rapor_kriterleri;
   if ( M_CARI_RV->bakiyesi_hesaplar EQ true ) {
        rapor_kriterleri.append ( "Bakiyesiz " );
   }
   if ( M_CARI_RV->calismamis_hesaplar EQ true ) {
       if ( rapor_kriterleri.isEmpty() EQ false ) {
            rapor_kriterleri.append(" - ");
       }
       rapor_kriterleri.append ( "Çalışmamış " );
   }
   if ( rapor_kriterleri.isEmpty() EQ false ) {
        rapor_kriterleri.append ( "Hesaplar " );
   }
   if ( M_CARI_RV->nakli_yekun EQ 1 ) {
        rapor_kriterleri.append ( "Nakli Yekün" );
   }

   SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%;font-size:75%;\" BORDER=0 >" );

   SET_TD_TAG_STRING ( QStringList()<<"style = \"width:30%; font-size:100%;\""
                                    <<"style = \"width:40%; font-size:100%;\" ALIGN=CENTER"
                                    <<"style = \"width:30%; font-size:100%;\"ALIGN=RIGHT");

   CREATE_TABLE ( QStringList()<<""<<""<<"",0,0 );

   ADD_ROW_TO_TABLE ( QStringList() << E9_GET_FIRMA_ISMI() << "<b>CARİ BAKİYE RAPORU</b>" << GET_REPORT_TOP_RIGHT_HEADER() );
   ADD_ROW_TO_TABLE ( QStringList() << "" << baslangic_tarihi + " - " + bitis_tarihi << "");
   ADD_ROW_TO_TABLE ( QStringList() << "" << rapor_kriterleri << "");
   ADD_ROW_TO_TABLE ( QStringList() << "" << hesap_araligi << "");


   return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   CARI_BAKIYE_RAPORU::GET_BODY
***************************************************************************************/

QString CARI_BAKIYE_RAPORU::GET_BODY()
{
    QStringList cari_hesap;
    double      toplam_borc_array[E9_ARRAY_SIZE];
    double      toplam_alacak_array[E9_ARRAY_SIZE];
    double      borc_bakiye;
    double      alacak_bakiye;
    QStringList headers;
    QStringList basilacak_satir;
    double     toplam_borc;
    double     toplam_alacak;

    if ( m_tum_hesaplar_basildi EQ 1 ) {
         return NULL;
    }

    int bas_gun_no = MALI_YIL_ARRAY_INDIS ( M_CARI_RV->baslangic_tarihi );
    int bts_gun_no = MALI_YIL_ARRAY_INDIS ( M_CARI_RV->bitis_tarihi );

    m_eklenen_kayit_sayisi = 0;

    headers << "Cari Hesap Kodu" << "Cari Hesap İsmi" << "Borç" << "Alacak" << "Borç Bakiye" << "Alacak Bakiye";

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%; \" BORDER = 1>");
    SET_TD_TAG_STRING    ( QStringList() << "style = \"width:14%;\"" << "style = \"width:42%;\""
                                         << "style = \"width:11%;\""  << "style = \"width:11%;\""
                                         << "style = \"width:11%;\"" << "style = \"width:11%;\"");

    CREATE_TABLE ( headers,1,14 );

    SET_TD_TAG_STRING ( QStringList()<< "" << "" <<"align = RIGHT "<<"align = RIGHT "
                                               <<"align = RIGHT "<<"align = RIGHT ");
    SQL_QUERY query(DB);

    for ( ; ; ) {

        if ( m_last_cari_hesap_no >= m_cari_hesaplar_list.size() ) {
             m_tum_hesaplar_basildi = true;
            break;
        }

        cari_hesap.clear();
        cari_hesap        = m_cari_hesaplar_list.at ( m_last_cari_hesap_no ).split ( "\t" );
        int cari_hesap_id = QVariant ( cari_hesap.at(0) ).toInt();

        query.PREPARE_SELECT("car_hesaplar",
                             "toplam_borc_array , toplam_alacak_array " ,
                             "cari_hesap_id = :cari_hesap_id");
        query.SET_VALUE(":cari_hesap_id" , cari_hesap_id );

        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            UNPACK_DOUBLE_ARRAY(query.VALUE(0).toString() , toplam_borc_array , E9_ARRAY_SIZE );
            UNPACK_DOUBLE_ARRAY(query.VALUE(1).toString() , toplam_alacak_array , E9_ARRAY_SIZE);
        }

        if ( M_CARI_RV->nakli_yekun EQ 1 ) {
             toplam_borc   = toplam_borc_array [ bts_gun_no ];
             toplam_alacak = toplam_alacak_array [ bts_gun_no ];
        }
        else {
            toplam_borc   = toplam_borc_array[bts_gun_no] - toplam_borc_array[bas_gun_no - 1];
            toplam_alacak = toplam_alacak_array[bts_gun_no] - toplam_alacak_array[bas_gun_no-1];
        }

        if ( M_CARI_RV->bakiyesi_hesaplar EQ false ) {
            if ( toplam_borc EQ toplam_alacak ) {
                if ( toplam_borc NE 0 AND toplam_alacak NE 0 ) {
                    m_last_cari_hesap_no++;
                    continue;
                }
            }
        }
        if ( M_CARI_RV->calismamis_hesaplar EQ false ) {
            if ( toplam_borc_array [ bas_gun_no-1 ] EQ toplam_borc_array [ bts_gun_no ] AND
                 toplam_alacak_array [ bas_gun_no-1 ] EQ toplam_alacak_array [ bts_gun_no ] ) {
                if ( toplam_borc_array [ bts_gun_no ] EQ 0 AND toplam_alacak_array [ bts_gun_no]  EQ 0 ) {
                     m_last_cari_hesap_no++;
                    continue;
                }
            }
        }
        borc_bakiye   = 0.0;
        alacak_bakiye = 0.0;

        if ( toplam_borc > toplam_alacak ) {
             borc_bakiye = toplam_borc - toplam_alacak;
        }
        else {
            alacak_bakiye = toplam_alacak - toplam_borc;
        }
        m_footer_borc          += toplam_borc;
        m_footer_alacak        += toplam_alacak;
        m_footer_borc_bakiye   += borc_bakiye;
        m_footer_alacak_bakiye += alacak_bakiye;

        basilacak_satir.clear();
        basilacak_satir << cari_hesap.at(1) << cari_hesap.at(2)
                << VIRGUL_EKLE ( QVariant ( ROUND ( toplam_borc ) ).toString())
                << VIRGUL_EKLE ( QVariant ( ROUND ( toplam_alacak ) ).toString())
                << VIRGUL_EKLE ( QVariant ( ROUND ( borc_bakiye ) ).toString())
                << VIRGUL_EKLE ( QVariant ( ROUND ( alacak_bakiye ) ).toString());

       ADD_ROW_TO_TABLE ( basilacak_satir );

       m_eklenen_kayit_sayisi++;
       m_last_cari_hesap_no++;

       if  ( m_eklenen_kayit_sayisi EQ m_max_line_count ) {
           return GET_TABLE_HTML_STRING();
       }
    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   CARI_BAKIYE_RAPORU::GET_FOOTER
***************************************************************************************/

QString CARI_BAKIYE_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:75%;font-weight:bold;\" BORDER = 0 >");
    SET_TD_TAG_STRING ( QStringList() << "style = \"width:56%;\"" << "style = \"width:11%;\" ALIGN = RIGHT"
                                      << "style = \"width:11%;\" ALIGN = RIGHT" << "style = \"width:11%;\" ALIGN=RIGHT"
                                      << "style = \"width:11;%\" ALIGN=RIGHT" );
    CREATE_TABLE( QStringList() << "" << "" << "" << "" << "",0,0);

    QStringList footer_line;

    footer_line << "Toplam  " << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_borc ) ).toString() )
                      << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_alacak )).toString() )
                      << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_borc_bakiye )).toString() )
                      << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_alacak_bakiye )).toString());
    ADD_ROW_TO_TABLE ( footer_line );

    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   CARI_BAKIYE_RAPORU::GET_CARI_HESAPLAR_LIST
***************************************************************************************/

QStringList CARI_BAKIYE_RAPORU::GET_CARI_HESAPLAR_LIST()
{
    QStringList cari_hesap;
    QStringList cari_hesaplar_list;

    SQL_QUERY select_query(DB);


    select_query.PREPARE_SELECT ( "car_hesaplar", "cari_hesap_id,cari_hesap_kodu,adres_id" );

    if ( M_CARI_RV->bas_hesap_kodu.isEmpty() NE true OR M_CARI_RV->bts_hesap_kodu.isEmpty() NE true ) {
        select_query.AND_EKLE   ( "cari_hesap_kodu BETWEEN :bas_hesap_kodu AND :bts_hesap_kodu " );
        select_query.SET_VALUE  ( ":bas_hesap_kodu",M_CARI_RV->bas_hesap_kodu );
        select_query.SET_VALUE  ( ":bts_hesap_kodu",M_CARI_RV->bts_hesap_kodu );
    }

    if ( select_query.SELECT( "cari_hesap_kodu ASC" ) EQ 0 ) {
        return QStringList();
    }
    while (select_query.NEXT()) {
        QString cari_hesap_id   = select_query.VALUE(0).toString();
        QString cari_hesap_kodu = select_query.VALUE(1).toString();
        QString cari_hesap_ismi = ADR_GET_FIRMA_SAHIS_ADI(select_query.VALUE(2).toInt());

        cari_hesap.clear();
        cari_hesap.append(cari_hesap_id + "\t" + cari_hesap_kodu + "\t" + cari_hesap_ismi + "\t");
        cari_hesaplar_list << cari_hesap;
    }
    return cari_hesaplar_list;
}
