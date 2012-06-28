#include <QStringList>
#include "prs_maas_bordrosu_raporu_class.h"
#include "prs_maas_bordrosu_raporu_open.h"
#include "print.h"
#include "adak_std_utils.h"
#include "prs_con_utils.h"
#include "prs_enum.h"
#include "prs_struct.h"
#include "cari_console_utils.h"
#include "yonetim.h"
#include "cari_struct.h"
#include "adak_utils.h"
#include "e9_gui_utils.h"
#include "sube_console_utils.h"
#include "e9_console_utils.h"

extern ADAK_SQL     *       DB;
extern PRS_RAPOR_VARS        PRS_RV;

/**************************************************************************************
                   SHOW_MAAS_BORDROSU_RAPORU
***************************************************************************************/

QString SHOW_MAAS_BORDROSU_RAPORU ( int bordro_id, QString sicil_no, int ay, int yil )
{
   MAAS_BORDROSU_RAPORU  *  R = new MAAS_BORDROSU_RAPORU( bordro_id, sicil_no, ay, yil );

   return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   MAAS_BORDROSU_RAPORU::MAAS_BORDROSU_RAPORU
**************************************************************************************/

MAAS_BORDROSU_RAPORU::MAAS_BORDROSU_RAPORU( int bordro_id, QString sicil_no, int ay, int yil ) : REPORT_KERNEL ("PRS_MAAS_BORDROSU_RAPORU")
{
    SET_HEIGHTS (60, 60);

    SET_PAPER_SIZE  ( QPrinter::A4 );

    // yatay yazdirmak icin
    SET_ORIENTATION ( QPrinter::Landscape );

    m_max_line_count            =  GET_MAX_LINE_COUNT (8);
    m_son_gonderilen_kayit_id   =  1;
    m_toplam_bordro_sayisi      =  0;
    m_sayfa_no                  =  0;
    m_sicil_no                  =  sicil_no;
    m_rapor_bitti               =  false;
    m_ay                        =  ay;
    m_yil                       =  yil;
    m_bordro_id                 =  bordro_id;
}

/**************************************************************************************
                   MAAS_BORDROSU_RAPORU::GET_HEADER
***************************************************************************************/

QString MAAS_BORDROSU_RAPORU::GET_HEADER()
{
    SQL_QUERY           sql_query_firma ( DB );

    QStringList    header_list;
    QString        firma_adi;
    QString        bordro_donemi;
    QString        vergi_dairesi_adi;
    QString        vergi_no;
    QString        sgk_isyeri_no;


    if ( m_rapor_bitti EQ true ) {
        return NULL;
    }

    SET_TABLE_TAG_STRING("<TABLE WIDTH=100% style=\"font-size:90%;\" BORDER=0 >");

    SET_TD_TAG_STRING(QStringList()<<"WIDTH = 35% ALIGN=LEFT"<<"WIDTH = 35% ALIGN=CENTER"<<"WIDTH = 30% ALIGN=RIGHT");

    header_list << E9_GET_FIRMA_ISMI() << "" << GET_REPORT_TOP_RIGHT_HEADER();

    CREATE_TABLE ( header_list, 0, 0 );

    header_list.clear();

    header_list<<"" <<"<font size=5><b>" + E9_GET_AYLAR_STRING ( m_ay ) +"&nbsp;&nbsp;&nbsp;"+ QVariant(m_yil).toString()+"&nbsp;&nbsp;&nbsp;"
            +"BORDROSU "<<"" ;

    ADD_ROW_TO_TABLE( header_list );

    header_list.clear();

    sql_query_firma.PREPARE_SELECT("e9_sabit_degerler","firma_ismi,firma_vergi_dairesi,firma_vergi_numarasi ",
                                   "sabit_deger_id =:sabit_deger_id");
    sql_query_firma.SET_VALUE(":sabit_deger_id",1);
    if(sql_query_firma.SELECT() > 0) {
        sql_query_firma.NEXT();
        firma_adi         = sql_query_firma.VALUE(0).toString();
        vergi_dairesi_adi = sql_query_firma.VALUE(1).toString();
        vergi_no          = sql_query_firma.VALUE(2).toString();
    }

    bordro_donemi       = E9_GET_AYLAR_STRING ( m_ay )+ "/" + QVariant( m_yil ).toString();

    header_list.clear();

    header_list<<"<font size=3>"+ vergi_dairesi_adi + "&nbsp;&nbsp;&nbsp;"+ vergi_no + "</font>" <<""<<"" ;

    ADD_ROW_TO_TABLE( header_list );

    sgk_isyeri_no = SUBE_GET_SUBE_SGK_NO( SUBE_GET_SUBE_ID() );

    header_list.clear();

    header_list<<"<font size=3> SGK NO  " + sgk_isyeri_no +"</font>" <<""<<"" ;

    ADD_ROW_TO_TABLE( header_list );


    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   MAAS_BORDROSU_RAPORU::GET_BODY
**************************************************************************************/

QString MAAS_BORDROSU_RAPORU::GET_BODY()
{
    SQL_QUERY           sql_query        ( DB );
    SQL_QUERY           sql_query_bordro ( DB );
    SQL_QUERY           query           (DB);

    QStringList         string_list;
    int                 personel_id;
    QString             ad_soyad;
    QString             tc_kimlik_no;
    int                 gun_sayisi              , toplam_gun                        = 0.00;
    double              net_ucret               , toplam_net_ucret                  = 0.00;
    double              brut_ucret              , toplam_brut_ucret                 = 0.00;
    double              issizlik_primi          , toplam_issizlik_primi             = 0.00;
    double              sigorta_primi           , toplam_sigorta_primi              = 0.00;
    double              gelir_vergi_matrahi     , toplam_gelir_vergi_matrahi        = 0.00;
    double              gelir_vergisi           , toplam_gelir_vergisi              = 0.00;
    double              asgari_gecim_indirimi   , toplam_asgari_gecim_indirimi      = 0.00;
    double              damga_vergisi           , toplam_damga_vergisi              = 0.00;
    double              kesintiler_toplami      , toplam_kesintiler_toplami         = 0.00;
    double              odenekler_toplami       , toplam_odenekler_toplami          = 0.00;
    double              kumulatif_vergi_matrahi , toplam_kumulatif_vergi_matrahi    = 0.00;
    double              ozel_kesintiler_toplami , toplam_ozel_kesintiler_toplami    = 0.00;
    double              ssk_matrahi             , toplam_ssk_matrahi                = 0.00;

    m_toplam_bordro_sayisi++;

    if ( m_rapor_bitti EQ true ) {
        return NULL;
    }

    string_list<<"Adı/Soyadı"<<"T.C.No"<<"Gün"<<"Brüt<br>Ücret"<<"Ödenek<br>Toplamı" << "SSK Matrahı" <<"Sigorta<br>Primi"<<"İşsiz.<br>Primi"
                   <<"G.Vergi<br>Matrahı"<<"G.Vergisi"<<"DÖKGV"<<"Damga<br>Vergisi"
                   <<"Kesinti<br>Toplamı" << "Özel Kesinti T." << "Kümülatif <br>Vergi Matrahi"<< "Vergi O."<<"Asg.<br>Geç.İnd."<<"Net<br>Ödeme"<<"İmza";

    SET_TABLE_TAG_STRING("<TABLE WIDTH=100% style=\"font-size:65%;\" BORDER=1 >");

    SET_TD_TAG_STRING(QStringList()<<"WIDTH = 9% ALIGN=LEFT"    <<"WIDTH = 8% ALIGN=LEFT"       <<"WIDTH = 3% ALIGN=RIGHT"
                                   <<"WIDTH = 5.5% ALIGN=RIGHT" <<"WIDTH = 5.5% ALIGN=RIGHT"    <<"WIDTH = 5.5% ALIGN=RIGHT"
                                   <<"WIDTH = 6% ALIGN=RIGHT"   <<"WIDTH = 4.5% ALIGN=RIGHT"    <<"WIDTH = 5% ALIGN=RIGHT"
                                   <<"WIDTH = 5% ALIGN=RIGHT"   <<"WIDTH = 5% ALIGN=RIGHT"      <<"WIDTH = 5% ALIGN=RIGHT"
                                   <<"WIDTH = 5.5% ALIGN=RIGHT" <<"WIDTH = 5.5% ALIGN=RIGHT"    <<"WIDTH = 5.5% ALIGN=RIGHT"
                                   <<"WIDTH = 6.5% ALIGN=RIGHT" <<"WIDTH = 4% ALIGN=RIGHT"      <<"WIDTH = 5% ALIGN=RIGHT"
                                   <<"WIDTH = 5% ALIGN=RIGHT");

    CREATE_TABLE (string_list, 0, 0);

    string_list.clear();

    sql_query_bordro.PREPARE_SELECT("prs_bordro_personelleri , prs_personeller ",
                                    "prs_bordro_personelleri.personel_id, calisilan_gun_sayisi, "
                                    "prs_bordro_personelleri.brut_ucret, prs_bordro_personelleri.net_ucret, "
                                    "sigorta_primi, issizlik_primi, gelir_vergi_matrahi, gelir_vergisi, asg_gec_indirimi, "
                                    "damga_vergisi, kesintiler_toplami, odenekler_toplami, brd_sonrasi_kum_vergi_matrahi, ozel_kesintiler_toplami ,"
                                    "ssk_matrahi",
                                    "prs_personeller.personel_id = prs_bordro_personelleri.personel_id" );

    sql_query_bordro.AND_EKLE ( "bordro_id = :bordro_id");
    sql_query_bordro.SET_VALUE( ":bordro_id"   , m_bordro_id );

    personel_id = PRS_GET_PERSONEL_ID ( m_sicil_no );

    if ( m_sicil_no NE "-1" ) {
        sql_query_bordro.AND_EKLE   ( "prs_personeller.personel_id  = :personel_id");
        sql_query_bordro.SET_VALUE  ( ":personel_id" , personel_id );
    }

    if ( sql_query_bordro.SELECT() EQ 0 ) {
        return NULL;
    }

    while ( sql_query_bordro.NEXT() EQ true ) {

        personel_id = sql_query_bordro.VALUE("personel_id").toInt();
        query.PREPARE_SELECT("prs_puantaj","gun_sayisi","personel_id= :personel_id");
        query.SET_VALUE(":personel_id",personel_id);

        if ( query.SELECT() EQ 0 ) {
            return NULL;
        }
        sql_query.PREPARE_SELECT("prs_personeller",
                                 "cari_hesap_id ",
                                 "personel_id = :personel_id");

        sql_query.SET_VALUE( ":personel_id" , personel_id );

        if ( sql_query.SELECT() EQ 0 ) {
            return NULL;
        }

        sql_query.NEXT();

        int     cari_hesap_id   = sql_query.VALUE( "cari_hesap_id" ).toInt();

        CARI_HESAP_STRUCT hesap_bilgileri;
        CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

        CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri  );

        ad_soyad                        =  hesap_bilgileri.cari_hesap_ismi;
        tc_kimlik_no                    =  hesap_bilgileri.kimlik_no;
        brut_ucret                      =  sql_query_bordro.VALUE("brut_ucret").toDouble();
        toplam_brut_ucret              +=  brut_ucret;
        odenekler_toplami               =  sql_query_bordro.VALUE("odenekler_toplami").toDouble();
        toplam_odenekler_toplami       +=  odenekler_toplami;
        ssk_matrahi                     =  sql_query_bordro.VALUE("ssk_matrahi").toDouble();
        toplam_ssk_matrahi             +=  ssk_matrahi;
        sigorta_primi                   =  sql_query_bordro.VALUE("sigorta_primi").toDouble();
        toplam_sigorta_primi           +=  sigorta_primi;
        gun_sayisi                      =  sql_query_bordro.VALUE("calisilan_gun_sayisi").toInt();
        toplam_gun                     +=  gun_sayisi;
        issizlik_primi                  =  sql_query_bordro.VALUE("issizlik_primi").toDouble();
        toplam_issizlik_primi          +=  issizlik_primi;
        gelir_vergi_matrahi             =  sql_query_bordro.VALUE("gelir_vergi_matrahi").toDouble();
        toplam_gelir_vergi_matrahi     +=  gelir_vergi_matrahi;
        gelir_vergisi                   =  sql_query_bordro.VALUE("gelir_vergisi").toDouble();
        toplam_gelir_vergisi           +=  gelir_vergisi;
        damga_vergisi                   =  sql_query_bordro.VALUE("damga_vergisi").toDouble();
        toplam_damga_vergisi           +=  damga_vergisi;
        kesintiler_toplami              =  sql_query_bordro.VALUE("kesintiler_toplami").toDouble();
        toplam_kesintiler_toplami      +=  kesintiler_toplami;
        ozel_kesintiler_toplami         =  sql_query_bordro.VALUE("ozel_kesintiler_toplami").toDouble();
        toplam_ozel_kesintiler_toplami +=  ozel_kesintiler_toplami;
        asgari_gecim_indirimi           =  sql_query_bordro.VALUE("asg_gec_indirimi").toDouble();
        toplam_asgari_gecim_indirimi   +=  asgari_gecim_indirimi;
        net_ucret                       =  sql_query_bordro.VALUE("net_ucret").toDouble();
        toplam_net_ucret               +=  net_ucret;
        kumulatif_vergi_matrahi         =  sql_query_bordro.VALUE("brd_sonrasi_kum_vergi_matrahi").toDouble(); // her ayin vergi matrahini getirmesi gerekiyor
        toplam_kumulatif_vergi_matrahi +=  kumulatif_vergi_matrahi;

        string_list.clear();

        string_list<<QVariant(ad_soyad).toString()
                   <<tc_kimlik_no
                   <<QVariant(gun_sayisi).toString()
                   <<VIRGUL_EKLE(QVariant(brut_ucret).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(odenekler_toplami).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(ssk_matrahi).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(sigorta_primi).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(issizlik_primi).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(gelir_vergi_matrahi).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(gelir_vergisi).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant((gelir_vergisi - asgari_gecim_indirimi)).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(damga_vergisi).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(kesintiler_toplami).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(ozel_kesintiler_toplami).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(kumulatif_vergi_matrahi ).toString(), 2, false)
                   <<"%"+ QVariant(PRS_GET_KUMULATIF_VERGI_ORANI( kumulatif_vergi_matrahi ) *100 ).toString()
                   <<VIRGUL_EKLE(QVariant(asgari_gecim_indirimi).toString(), 2, false)
                   <<VIRGUL_EKLE(QVariant(net_ucret).toString(), 2, false)<< "";

        ADD_ROW_TO_TABLE(string_list);

        m_rapor_bitti = true;
    }
    string_list.clear();

    string_list<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"-";

    ADD_ROW_TO_TABLE(string_list);

    string_list.clear();

    string_list<<"<font size=3>""Toplam"<< ""<<QVariant(toplam_gun).toString()
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_brut_ucret).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_odenekler_toplami).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_ssk_matrahi).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_sigorta_primi).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_issizlik_primi).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_gelir_vergi_matrahi).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_gelir_vergisi).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant((toplam_gelir_vergisi - toplam_asgari_gecim_indirimi)).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_damga_vergisi).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_kesintiler_toplami).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_ozel_kesintiler_toplami ).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_kumulatif_vergi_matrahi ).toString(), 2, false)
               <<"" // kumulatif vergi matrahi
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_asgari_gecim_indirimi).toString(), 2, false)
               <<"<font size=2>"+VIRGUL_EKLE(QVariant(toplam_net_ucret).toString(), 2, false)<< "";

    ADD_ROW_TO_TABLE(string_list);

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   MAAS_BORDROSU_RAPORU::GET_FOOTER
***************************************************************************************/

QString MAAS_BORDROSU_RAPORU::GET_FOOTER()
{
    return NULL;
}
