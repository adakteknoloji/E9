#include "adak_utils.h"
#include "stok_urun_listesi_raporu_class.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_struct.h"
#include "yonetim.h"
#include "adak_gruplar.h"
#include "e9_console_utils.h"

extern ADAK_SQL *         DB;

/**************************************************************************************
                   GET_STOK_URUN_LISTESI_RAPORU
***************************************************************************************/

QString GET_STOK_URUN_LISTESI_RAPORU (STOK_RAPOR_VARS * rapor_vars )
{
    STOK_URUN_LISTESI_RAPORU  *  F = new STOK_URUN_LISTESI_RAPORU( "Stok Urun Listesi",rapor_vars);
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   STOK_URUN_LISTESI_RAPORU::STOK_URUN_LISTESI_RAPORU
***************************************************************************************/

STOK_URUN_LISTESI_RAPORU::STOK_URUN_LISTESI_RAPORU( QString baslik,STOK_RAPOR_VARS * rapor_vars ) : REPORT_KERNEL (baslik)
{
    M_STOK_RV = rapor_vars;

    SET_HEIGHTS ( 30, 0);

    m_max_line_count       =  GET_MAX_LINE_COUNT ( 14 );

    m_tum_kayitlar_basildi = false;
    m_last_urun_indisi     = 0;

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_urunler","urun_adi","urun_kodu = :urun_kodu AND hizmet_mi = :hizmet_mi");
    query.SET_VALUE(":hizmet_mi" , M_STOK_RV->urun_mu_hizmet_mi );
    query.SET_VALUE(":urun_kodu" , M_STOK_RV->bas_urun_kodu);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        m_urun_araligi.append("<b>" + M_STOK_RV->bas_urun_kodu + " " + query.VALUE(0).toString() + "  - ");
    }

    query.PREPARE_SELECT("stk_urunler","urun_adi","urun_kodu = :urun_kodu AND hizmet_mi = :hizmet_mi");
    query.SET_VALUE(":hizmet_mi" , M_STOK_RV->urun_mu_hizmet_mi );
    query.SET_VALUE(":urun_kodu" , M_STOK_RV->bts_urun_kodu);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        m_urun_araligi.append(M_STOK_RV->bts_urun_kodu + " " + query.VALUE(0).toString());
    }

    if ( M_STOK_RV->grup_idleri.size() NE 0 ) {
        GRP_GRUBUN_ALT_GRUPLARINI_BUL(M_STOK_RV->grup_idleri,&M_STOK_GRUPLARI_ID_LIST);
    }
    M_URUN_LISTESI = GET_URUN_LISTESI();
}


/**************************************************************************************
                   STOK_URUN_LISTESI_RAPORU::GET_HEADER
***************************************************************************************/

QString STOK_URUN_LISTESI_RAPORU::GET_HEADER()
{

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()<< "" << GET_REPORT_TOP_RIGHT_HEADER() );

    if ( M_STOK_RV->urun_mu_hizmet_mi EQ ENUM_URUN_KARTI ) {
        ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>ÜRÜN  LİSTESİ" << "");
    }
    else {
        ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>HİZMET  LİSTESİ" << "");
    }

    ADD_ROW_TO_TABLE     ( QStringList() << "" << m_urun_araligi << "");

    ADD_ROW_TO_TABLE     ( QStringList() << "" << m_grup_bilgisi << "");

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   STOK_URUN_LISTESI_RAPORU::GET_BODY
***************************************************************************************/

QString STOK_URUN_LISTESI_RAPORU::GET_BODY()
{
    QStringList     basilacak_satir;
    QStringList     URUN;

    if ( m_tum_kayitlar_basildi EQ true ) {
        return NULL;
    }

    int yazdirilan_kayit_sayisi = 0;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );
    SET_TD_TAG_STRING( QStringList() << "WIDTH=20%" << "WIDTH=35%"<< "WIDTH=15%" << "WIDTH=30%" );
    CREATE_TABLE( QStringList()<< "<b>Ürün Kodu" << "<b>Ürün Adı" << "<b>Temel Birimi" << "<b>Barkod Numarası",1,13);

    for ( ; ;  ) {

        if ( m_last_urun_indisi >= M_URUN_LISTESI.size() ) {
            m_tum_kayitlar_basildi = true;
            break;
        }

        URUN = M_URUN_LISTESI.at(m_last_urun_indisi).split("\t");

        basilacak_satir.clear();
        basilacak_satir << URUN.at(0) << URUN.at(1) << URUN.at(2) << URUN.at(3) ;


        ADD_ROW_TO_TABLE(basilacak_satir);

        yazdirilan_kayit_sayisi++;
        m_last_urun_indisi++;

        if ( yazdirilan_kayit_sayisi EQ m_max_line_count ) {
            break;
        }
    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   STOK_URUN_LISTESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString STOK_URUN_LISTESI_RAPORU::GET_FOOTER()
{
    return NULL;
}

/**************************************************************************************
                   STOK_URUN_LISTESI_RAPORU::GET_URUN_LISTESI()
***************************************************************************************/

QStringList STOK_URUN_LISTESI_RAPORU::GET_URUN_LISTESI()
{
    SQL_QUERY       query(DB);
    QStringList     URUN_LISTESI;
    QStringList     URUN;

    query.PREPARE_SELECT("stk_urunler","urun_kodu, urun_adi, urun_temel_birimi, barkod_numarasi" );

    query.AND_EKLE  ( " ( ( urun_kodu BETWEEN :bas_urun_kodu AND :bts_urun_kodu ) OR ( barkod_numarasi BETWEEN :bas_barcode AND :bts_barcode ) ) " );
    query.SET_VALUE ( ":bas_urun_kodu", M_STOK_RV->bas_urun_kodu );
    query.SET_VALUE ( ":bts_urun_kodu", M_STOK_RV->bts_urun_kodu );

    query.SET_VALUE ( ":bas_barcode", M_STOK_RV->bas_urun_kodu );
    query.SET_VALUE ( ":bts_barcode", M_STOK_RV->bts_urun_kodu );

    query.AND_EKLE  ( "( hizmet_mi = :hizmet_mi )" );
    query.SET_VALUE ( ":hizmet_mi", M_STOK_RV->urun_mu_hizmet_mi );

    QList<int> kayit_idleri;

    kayit_idleri = GRP_GRUBA_EKLI_KAYITLARI_BUL( E9_PROGRAMI, STOK_MODULU, M_STOK_GRUPLARI_ID_LIST );

    if ( kayit_idleri.isEmpty() EQ false ) {

        query.AND_MULTI_EKLE("urun_id" , query.TO_QVARIANT( kayit_idleri ), ADAK_OR );
    }

    if ( query.SELECT( "urun_kodu" ) EQ 0 ) {
        return QStringList();
    }

    while ( query.NEXT() ) {
        URUN.clear();
        URUN.append ( query.VALUE(0).toString() + "\t" +
                      query.VALUE(1).toString() + "\t" +
                      query.VALUE(2).toString() + "\t" +
                      query.VALUE(3).toString() + "\t" +
                      QVariant(" ").toString() );

        URUN_LISTESI << URUN;
    }
    return URUN_LISTESI;
}












