#include "adak_utils.h"
#include "stok_miktarlari_raporu_class.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_struct.h"
#include "stok_enum.h"
#include "sube_console_utils.h"
#include "yonetim.h"
#include "adak_gruplar.h"
#include "e9_console_utils.h"


extern ADAK_SQL *         DB;

/**************************************************************************************
                   GET_STOK_MIKTARLARI_RAPORU
***************************************************************************************/

QString GET_STOK_MIKTARLARI_RAPORU (STOK_RAPOR_VARS * rapor_vars)
{
    STOK_MIKTARLARI_RAPORU  *  F = new STOK_MIKTARLARI_RAPORU( "Stok Miktarlari" ,rapor_vars);
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   STOK_MIKTARLARI_RAPORU::STOK_MIKTARLARI_RAPORU
***************************************************************************************/

STOK_MIKTARLARI_RAPORU::STOK_MIKTARLARI_RAPORU( QString baslik,STOK_RAPOR_VARS * rapor_vars) : REPORT_KERNEL (baslik)
{

    M_STOK_RV = rapor_vars;

    SET_HEIGHTS ( 30, 30 );

    m_max_line_count                =  GET_MAX_LINE_COUNT ( 14 );
    m_tum_kayitlar_basildi          = false;
    m_last_urun_indisi              = 0;
    m_footer_depodaki_miktar        = 0.00;

    if ( M_STOK_RV->grup_idleri.size() NE 0 ) {
        GRP_GRUBUN_ALT_GRUPLARINI_BUL(M_STOK_RV->grup_idleri , &M_STOK_GRUPLARI_ID_LIST );
    }

    M_URUN_LISTESI = GET_URUN_LISTESI();

    m_rapor_kriterleri_1 = "Ürün Aralığı " + ADD_HTML_SPACE("      ") + " : ";

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_urunler","urun_adi" ,"urun_kodu = :urun_kodu");
    query.SET_VALUE(":urun_kodu" , M_STOK_RV->bas_urun_kodu);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        m_rapor_kriterleri_1.append(M_STOK_RV->bas_urun_kodu + "  " + query.VALUE(0).toString());
    }

    query.PREPARE_SELECT("stk_urunler","urun_adi" ,"urun_kodu = :urun_kodu");
    query.SET_VALUE(":urun_kodu" , M_STOK_RV->bts_urun_kodu);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        m_rapor_kriterleri_1.append(" - " + M_STOK_RV->bts_urun_kodu + " " + query.VALUE(0).toString());
    }

    if ( M_STOK_RV->depo_id NE -1 ) {
        QString depo_kodu;
        QString depo_adi;

        SUBE_GET_DEPO_KODU_ADI(M_STOK_RV->depo_id, &depo_kodu , &depo_adi);
        m_rapor_kriterleri_3 = "Depo Kodu / Adı : " + depo_kodu + "  " + depo_adi;
    }

}


/**************************************************************************************
                   STOK_MIKTARLARI_RAPORU::GET_HEADER
***************************************************************************************/

QString STOK_MIKTARLARI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()<< "" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>STOK MİKTARLARI" << "");

    if ( m_rapor_kriterleri_1.isEmpty() EQ false ) {
        ADD_ROW_TO_TABLE     ( QStringList() << ""  << "" << m_rapor_kriterleri_1);
    }

    if ( m_rapor_kriterleri_2.isEmpty() EQ false ) {
        ADD_ROW_TO_TABLE     ( QStringList() << " " << "" << m_rapor_kriterleri_2);
    }

    if ( m_rapor_kriterleri_3.isEmpty() EQ false ) {
        ADD_ROW_TO_TABLE     ( QStringList() << ""  << "" << m_rapor_kriterleri_3);
    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   STOK_MIKTARLARI_RAPORU::GET_BODY
***************************************************************************************/

QString STOK_MIKTARLARI_RAPORU::GET_BODY()
{

    QStringList URUN;
    QStringList basilacak_satir;

    if ( m_tum_kayitlar_basildi EQ true ) {
        return NULL;
    }

    int yazdirilan_kayit_sayisi = 0;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=15%"
                                         << "WIDTH=30%"
                                         << "WIDTH=25%"
                                         << "WIDTH=15% ALIGN = RIGHT"
                                         << "WIDTH=15% ALIGN = CENTER" );

    CREATE_TABLE         ( QStringList()<< "<b>Ürün Kodu" << "<b>Ürün Adı" << "<b>Depo Kodu / Adı"
                                        << "<b>Depodaki Miktar" << "<b>Birimi",1,13);



    for ( ; ; ) {

        if ( m_last_urun_indisi >= M_URUN_LISTESI.size()) {
            m_tum_kayitlar_basildi = true;
            break;
        }

        URUN = M_URUN_LISTESI.at(m_last_urun_indisi).split("\t");

        basilacak_satir.clear();
        basilacak_satir << URUN.at(0) << URUN.at(1) << URUN.at(2) << URUN.at(3) << URUN.at(4);


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
                   STOK_MIKTARLARI_RAPORU::GET_FOOTER
***************************************************************************************/

QString STOK_MIKTARLARI_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%; font-size:75%;font-weight:bold;\" BORDER = 0 >");


    SET_TD_TAG_STRING ( QStringList() << " style = \"width:70%;\" "
                                      << " style = \"width:15%;\" ALIGN = RIGHT"
                                      << " style = \"width:15%;\" ALIGN = RIGHT");

    CREATE_TABLE ( QStringList() << "" << "" << "",0,0 );

    QStringList footer_line;

    footer_line << "Toplam " <<  QVariant( m_footer_depodaki_miktar).toString()  << "";

    ADD_ROW_TO_TABLE ( footer_line );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   STOK_MIKTARLARI_RAPORU::GET_URUN_LISTESI()
***************************************************************************************/

QStringList STOK_MIKTARLARI_RAPORU::GET_URUN_LISTESI()
{
    QString ent_filtre_tables;

    SQL_QUERY       query(DB);
    QStringList     URUN_LISTESI;
    QStringList     URUN;


    ent_filtre_tables = "toplam_depoya_giren_array, toplam_depodan_cikan_array ,   ";

    query.PREPARE_SELECT("stk_depodaki_urunler , stk_urunler" , ent_filtre_tables + "depo_id, "
                         " urun_kodu, urun_adi, urun_temel_birimi, stk_urunler.urun_id" ,
                         "stk_depodaki_urunler.urun_id = stk_urunler.urun_id ");


    if ( M_STOK_RV->depo_id NE -1 ) {
        query.AND_EKLE  ( " depo_id = :depo_id " );
        query.SET_VALUE (":depo_id", M_STOK_RV->depo_id );
    }
    if ( M_STOK_RV->bas_urun_kodu.isEmpty() NE true OR M_STOK_RV->bts_urun_kodu.isEmpty() NE true ) {
        query.AND_EKLE( " ( urun_kodu BETWEEN :bas_urun_kodu AND :bts_urun_kodu )" );

        query.SET_VALUE(":bas_urun_kodu" , QVariant ( M_STOK_RV->bas_urun_kodu ).toString() );
        query.SET_VALUE(":bts_urun_kodu" , QVariant ( M_STOK_RV->bts_urun_kodu ).toString() );
    }

    double depoya_giren_urun_miktari[E9_ARRAY_SIZE];
    double depodan_cikan_urun_miktari[E9_ARRAY_SIZE];

    QList<int> kayit_idleri;

    kayit_idleri = GRP_GRUBA_EKLI_KAYITLARI_BUL( E9_PROGRAMI, STOK_MODULU, M_STOK_GRUPLARI_ID_LIST );

    if ( kayit_idleri.isEmpty() EQ false ) {
        query.AND_MULTI_EKLE("stk_urunler.urun_id" , query.TO_QVARIANT( kayit_idleri ), ADAK_OR );
    }

    if ( query.SELECT("urun_kodu ASC, urun_adi ASC ") EQ 0 ) {
        return QStringList();
    }

    int gun_no = MALI_YIL_ARRAY_INDIS( M_STOK_RV->baslangic_tarihi );

    while ( query.NEXT() EQ true ) {

        UNPACK_DOUBLE_ARRAY(query.VALUE(0).toString() , depoya_giren_urun_miktari  , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(query.VALUE(1).toString() , depodan_cikan_urun_miktari , E9_ARRAY_SIZE);

        double depodaki_miktar = depoya_giren_urun_miktari[gun_no] - depodan_cikan_urun_miktari[gun_no];

        int depo_id = query.VALUE(2).toInt();
        QString depo_kodu ;
        QString depo_adi;

        SUBE_GET_DEPO_KODU_ADI(depo_id , &depo_kodu , &depo_adi);
        QString depo_bilgisi = depo_kodu + "  " + depo_adi;

        URUN.clear();
        m_footer_depodaki_miktar    += QVariant(depodaki_miktar).toDouble();

        URUN.append ( query.VALUE("urun_kodu").toString() + "\t" + query.VALUE("urun_adi").toString() + "\t" +
                      depo_bilgisi + "\t" + QVariant(depodaki_miktar).toString()
                      +"\t" + query.VALUE("urun_temel_birimi").toString() );

        URUN_LISTESI << URUN;
    }

    return URUN_LISTESI;
}
