#include <cmath>
#include "adak_utils.h"
#include "prs_ucret_pusulasi_raporu_class.h"
#include "prs_ucret_pusulasi_raporu_open.h"
#include "yonetim.h"
#include "cari_console_utils.h"
#include "prs_enum.h"
#include "prs_struct.h"
#include "e9_gui_utils.h"
#include "cari_struct.h"
#include "e9_console_utils.h"

extern ADAK_SQL *         DB;

/**************************************************************************************
                   GET_PRS_UCRET_PUSULASI_RAPORU
***************************************************************************************/

QString GET_PRS_UCRET_PUSULASI_RAPORU ( PRS_UCRET_PUSULASI_RAPORU_STRUCT * P_UCRET_RV  )
{
    PRS_UCRET_PUSULASI_RAPORU  *  F = new PRS_UCRET_PUSULASI_RAPORU( P_UCRET_RV, "Ücret Pusulası" );
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_RAPORU::PRS_UCRET_PUSULASI_RAPORU
***************************************************************************************/

PRS_UCRET_PUSULASI_RAPORU::PRS_UCRET_PUSULASI_RAPORU( PRS_UCRET_PUSULASI_RAPORU_STRUCT * P_UCRET_RV , QString rapor_basligi ) : REPORT_KERNEL ( rapor_basligi)
{
    SET_HEIGHTS ( 30, 30 );

    M_UCRET_RV = P_UCRET_RV;

    m_max_line_count  =  GET_MAX_LINE_COUNT ( 14 );

    m_tum_rapor_satirlari_basildi = false;

    toplam_gun              = 0;
    toplam_kazanc           = 0.00;
    toplam_sosyal_yardim    = 0.00;
    toplam_ozel_kesintiler  = 0.00;
    toplam_yasal_kesintiler = 0.00;

    m_son_satir = 0;

    SQL_QUERY query( DB );

    if ( M_UCRET_RV->personel_id NE -1 ) {
        personel_list << M_UCRET_RV->personel_id;
    }
    else if( M_UCRET_RV->personel_list.size() > 0 ) {
        personel_list << M_UCRET_RV->personel_list;
    }
    else {
        query.PREPARE_SELECT("prs_personeller", "personel_id" );

        if ( M_UCRET_RV->sube_id NE 0 ) {
            query.AND_EKLE  ( "sube_id = :sube_id");
            query.SET_VALUE ( ":sube_id", M_UCRET_RV->sube_id );
        }
        if( query.SELECT() NE 0 ) {
            while (  query.NEXT() EQ true  ) {
                personel_list << query.VALUE( 0 ).toInt();
            }
        }
    }
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_RAPORU::GET_HEADER
***************************************************************************************/

QString PRS_UCRET_PUSULASI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList()<< E9_GET_FIRMA_ISMI() << "" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList()<< "" << "<b>ÜCRET PUSULASI" << "");

    ADD_ROW_TO_TABLE     ( QStringList()<< "" <<  E9_GET_AYLAR_STRING( M_UCRET_RV->bordro_ay ) + " - " + QVariant( M_UCRET_RV->bordro_yil ).toString() << "");

    ADD_ROW_TO_TABLE    ( QStringList()<< "" << "" << "");

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_RAPORU::GET_BODY
***************************************************************************************/

QString PRS_UCRET_PUSULASI_RAPORU::GET_BODY()
{
    int  m_last_satir_indisi = 0;

    if ( m_son_satir EQ personel_list.size() ) {
        return NULL;
    }

    M_RAPOR_SATIRLARI.clear();
    M_RAPOR_BILGILER.clear();

    M_SOSYAL_YARDIM_BILGILERI.clear();
    M_OZEL_KESINTILER_BILGILERI.clear();
    M_KAZANC_BILGILERI.clear();
    M_YASAL_KESINTILER_BILGILERI.clear();

    // Satirlar dolduruluyor.
    FILL_RAPOR_BILGILER ( personel_list.at( m_son_satir ) );
    FILL_RAPOR_SATIRLARI( personel_list.at( m_son_satir ) );

    QString tables;

    tables.clear();

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=5%" << "WIDTH=15%"<< "WIDTH=10%" << "WIDTH=10%" << "WIDTH=10%"
                           << "WIDTH=5%" << "WIDTH=15%" );

    /*
    -------------------------------------------------------------------------------------------------------------------------------------
    |  KODU    |      Adı & Soyadı    |T.C. Kim.No|Giriş Trh.|Çıkış Trh.| Görevi        |Üc.Tipi |     Ücreti     | SSK Sicil Nosu      |
    |----------|----------------------|-----------|----------|----------|---------------|--------|----------------|---------------------|
    |0001      |SEDAT GÜRBÜZLER       |11419375528|10.07.2008|          |               |Aylık   |         760,50 |3404199505509        |
    -------------------------------------------------------------------------------------------------------------------------------------
    */

    CREATE_TABLE         ( QStringList()<< "<b>Kodu" << "<b>Adı & Soyadı" << "<b>T.C. Kimlik No" << "<b>Giriş Tarihi"<<"<b>Çıkış Tarihi"
                           << "<b>Görevi" << "<b>Ücret Tipi" << "<b>Ücret" << "<b>SSK Sicil No" ,1,13); // border height and font size

    SET_TD_TAG_STRING    ( QStringList()<< "" << "" << "" << "" << ""<< "" <<"" << "" << "" << "" );

    QStringList RAPOR_SATIRI;
    m_last_satir_indisi = 0;
    forever {

        if ( m_last_satir_indisi EQ M_RAPOR_BILGILER.size() ) {
            break;
        }

        RAPOR_SATIRI = M_RAPOR_BILGILER.at(m_last_satir_indisi).split("\t");

        ADD_ROW_TO_TABLE (QStringList() << RAPOR_SATIRI.at(0) << RAPOR_SATIRI.at(1) << RAPOR_SATIRI.at(2)
                                        << RAPOR_SATIRI.at(3) << RAPOR_SATIRI.at(4) << RAPOR_SATIRI.at(5)
                                        << RAPOR_SATIRI.at(6) << RAPOR_SATIRI.at(7) << RAPOR_SATIRI.at(8));

        m_last_satir_indisi++;

     }

   tables = GET_TABLE_HTML_STRING();
    /*
    -------------------------------------------------------------------------------------------------------------------------------------
    |             KAZANÇ                    |      SOSYAL YARDIMI         |      ÖZEL KESİNTİSİ         |      YASAL KESİNTİSİ          |
    |---------------------------------------|-----------------------------|-----------------------------|-------------------------------|
    |  KAZANÇ ADI  |GÜN/SAAT |  TUTARI      |  YARDIM ADI  |   TUTARI     |  KESİNTİ ADI |   TUTARI     |  İNDİRİM ADI |   TUTARI       |
    |--------------|---------|--------------|--------------|--------------|--------------|--------------|--------------|----------------|
    |Normal        |26/195,0 |       659,10 |Çocuk         |         0,00 |Sendika aidat |         0,00 |SSK günü      |             30 |
    |H.tatil       | 4/ 30,0 |       101,40 |Okul öncesi(0 |         0,00 |Borç taksidi  |         0,00 |Dev. SSK Matr.|           0,00 |

    */

   SET_TABLE_TAG_STRING  ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

   SET_TD_TAG_STRING     ( QStringList() << "WIDTH=25%" << "WIDTH=25%"<< "WIDTH=25%" << "WIDTH=25%"  );

   CREATE_TABLE          ( QStringList()<< "<b>Kazanç" << "<b>Sosyal Yardım" << "<b>Özel Kesinti" << "<b>Yasal Kesinti", 1, 13);

   SET_TD_TAG_STRING     ( QStringList()<< "" << "" << "" << "" );

   tables.append( "<BR>"+ GET_TABLE_HTML_STRING() );

   SET_TABLE_TAG_STRING  ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

   SET_TD_TAG_STRING     ( QStringList() << "WIDTH=10%" << "WIDTH=5%"<< "WIDTH=10%" << "WIDTH=15%" << "WIDTH=10%" << "WIDTH=15%" << "WIDTH=10%"
                                         << "WIDTH=15%" << "WIDTH=10%" );

   CREATE_TABLE          ( QStringList()<< "<b>Kazanç Adı" << "<b>Gün" << "<b>Tutarı" << "<b>Yardım Adı" << "<b>Tutarı" << "<b>Kesinti Adı" << "<b>Tutarı"
                                       << "<b>İndirim Adı"<< "<b>Tutarı", 1, 13);

   SET_TD_TAG_STRING     ( QStringList()<< "" << "" << "ALIGN=RIGHT" << "" << "ALIGN=RIGHT" << "" << "ALIGN=RIGHT" << "" << "ALIGN=RIGHT" );


   QStringList SOSYAL_YARDIM;
   QStringList OZEL_KESINTILER;
   QStringList KAZANC_BILGILERI;
   QStringList YASAL_BILGILERI;

   int max_satir = M_YASAL_KESINTILER_BILGILERI.size();

   if ( M_SOSYAL_YARDIM_BILGILERI.size() > max_satir ) {
       max_satir    = M_SOSYAL_YARDIM_BILGILERI.size();
   }
   if ( M_OZEL_KESINTILER_BILGILERI.size() > max_satir ) {
       max_satir    = M_OZEL_KESINTILER_BILGILERI.size();
   }
   if ( M_KAZANC_BILGILERI.size() > max_satir ) {
       max_satir    = M_KAZANC_BILGILERI.size();
   }

   m_last_satir_indisi = 0;
   for ( ; ; ) {

       if ( m_last_satir_indisi EQ max_satir ) {
           m_tum_rapor_satirlari_basildi =  true;
           break;
       }


       QString kazanc_adi, kazanc_gun, kazanc_tutar;
       QString sosyal_ad, sosyal_tutar;
       QString ozel_adi, ozel_tutar;
       QString yasal_ad, yasal_tutar;


       if ( m_last_satir_indisi < M_KAZANC_BILGILERI.size() ) {
            KAZANC_BILGILERI    = M_KAZANC_BILGILERI.at(m_last_satir_indisi).split("\t");
            kazanc_adi      = KAZANC_BILGILERI.at( 0 );
            kazanc_gun      = KAZANC_BILGILERI.at( 1 );
            kazanc_tutar    = KAZANC_BILGILERI.at( 2 );
       }
       if ( m_last_satir_indisi < M_SOSYAL_YARDIM_BILGILERI.size() ) {
            SOSYAL_YARDIM  = M_SOSYAL_YARDIM_BILGILERI.at(m_last_satir_indisi).split("\t");

            sosyal_ad       = SOSYAL_YARDIM.at(0);
            sosyal_tutar    = SOSYAL_YARDIM.at(1);

       }
       if (  m_last_satir_indisi < M_OZEL_KESINTILER_BILGILERI.size() ) {
            OZEL_KESINTILER = M_OZEL_KESINTILER_BILGILERI.at(m_last_satir_indisi).split("\t");

            ozel_adi        = OZEL_KESINTILER.at(0);
            ozel_tutar      = OZEL_KESINTILER.at(1);
       }
       if (  m_last_satir_indisi < M_YASAL_KESINTILER_BILGILERI.size() ) {
           YASAL_BILGILERI = M_YASAL_KESINTILER_BILGILERI.at(m_last_satir_indisi).split("\t");

           yasal_ad        = YASAL_BILGILERI.at(0);
           yasal_tutar     = YASAL_BILGILERI.at(1);
       }



       ADD_ROW_TO_TABLE (QStringList() << kazanc_adi
                                       << kazanc_gun
                                       << kazanc_tutar
                                       << sosyal_ad
                                       << sosyal_tutar
                                       << ozel_adi
                                       << ozel_tutar
                                       << yasal_ad
                                       << yasal_tutar );

       m_last_satir_indisi++;

  }

   m_son_satir++;

   return tables.append( GET_TABLE_HTML_STRING() );
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_RAPORU::GET_FOOTER
***************************************************************************************/

QString PRS_UCRET_PUSULASI_RAPORU::GET_FOOTER()
{

    SET_TABLE_TAG_STRING  ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

    SET_TD_TAG_STRING     ( QStringList() << "WIDTH=10%" << "WIDTH=5% ALIGN=RIGHT"<< "WIDTH=10% ALIGN=RIGHT" << "WIDTH=15%" << "WIDTH=10% ALIGN=RIGHT" << "WIDTH=15%" << "WIDTH=10% ALIGN=RIGHT"
                                          << "WIDTH=15%" << "WIDTH=10% ALIGN=RIGHT" );

    CREATE_TABLE          ( QStringList() << "<b>Toplam" << VIRGUL_EKLE ( QVariant ( ROUND ( toplam_gun   ) ).toString())  << VIRGUL_EKLE ( QVariant ( ROUND ( toplam_kazanc ) ).toString())
                                          << "<b>Toplam" << VIRGUL_EKLE ( QVariant ( ROUND ( toplam_sosyal_yardim ) ).toString() )
                                          << "<b>Toplam" << VIRGUL_EKLE ( QVariant ( ROUND ( toplam_ozel_kesintiler ) ).toString() )
                                          << "<b>Toplam" << VIRGUL_EKLE ( QVariant ( ROUND ( toplam_yasal_kesintiler ) ).toString() ) , 1, 13);

    SET_TD_TAG_STRING     ( QStringList()<< "" << "" << "ALIGN=RIGHT" << "" << "ALIGN=RIGHT" << "" << "ALIGN=RIGHT" << "" << "ALIGN=RIGHT" );

    QString footer_tables = GET_TABLE_HTML_STRING();



    SET_TABLE_TAG_STRING  ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );

    SET_TD_TAG_STRING     ( QStringList() << "WIDTH=15%" << "WIDTH=15%"<< "WIDTH=15%" << "WIDTH=15%" << "WIDTH=15%" << "WIDTH=25%" );

    CREATE_TABLE          ( QStringList()<< "<b>Toplam Kazanç" << "<b>Toplam Odenekler" << "<b>Toplam Kesintiler"
                                         << "<b>Net Ödenecek" << "<center><b>Ücreti Aldığına Dair <br>Sigortalının İmzası</center>", 1, 13);

    SET_TD_TAG_STRING     ( QStringList()<< "ALIGN=RIGHT"      << "ALIGN=RIGHT"         << "ALIGN=RIGHT"          << "ALIGN=RIGHT"     << "" );


    QStringList toplam_list;

    toplam_list << VIRGUL_EKLE( QVariant ( toplam_kazanc ).toString(), 2 );
    toplam_list << VIRGUL_EKLE( QVariant ( toplam_sosyal_yardim ).toString(), 2 );
    toplam_list << VIRGUL_EKLE( QVariant ( toplam_ozel_kesintiler + toplam_yasal_kesintiler ).toString(), 2 );
    toplam_list << VIRGUL_EKLE( QVariant ( toplam_kazanc + toplam_sosyal_yardim - toplam_yasal_kesintiler - toplam_ozel_kesintiler  ).toString(), 2 );
    toplam_list << "";

    ADD_ROW_TO_TABLE( toplam_list );

    return footer_tables.append( "<br>" + GET_TABLE_HTML_STRING() );
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_RAPORU::FILL_RAPOR_SATIRLARI();
***************************************************************************************/

void PRS_UCRET_PUSULASI_RAPORU::FILL_RAPOR_SATIRLARI(int p_personel_id )
{
    SQL_QUERY query( DB );

    // UCRET BILGILERI
    query.PREPARE_SELECT( "prs_bordro_personelleri, prs_bordrolar", "calisilan_gun_sayisi, brut_ucret",
                          "prs_bordro_personelleri.bordro_id = prs_bordrolar.bordro_id "
                          "AND bordro_donemi_ay  = :bordro_donemi_ay         "
                          "AND bordro_donemi_yil = :bordro_donemi_yil        ");

    query.SET_VALUE     ( ":bordro_donemi_ay"   , M_UCRET_RV->bordro_ay   );
    query.SET_VALUE     ( ":bordro_donemi_yil"  , M_UCRET_RV->bordro_yil  );

    query.AND_EKLE      ( "personel_id       = :personel_id" );
    query.SET_VALUE     ( ":personel_id"    , p_personel_id );


    QStringList SATIR_DATA;

    if ( query.SELECT() NE 0 ) {
       while (query.NEXT() EQ true ) {

           toplam_gun   = query.VALUE( "calisilan_gun_sayisi" ).toInt();
           toplam_kazanc= query.VALUE( "brut_ucret" ).toDouble();

           SATIR_DATA.clear();
           SATIR_DATA.append( QString( "Normal" )
                             + "\t" + query.VALUE( "calisilan_gun_sayisi" ).toString()
                             + "\t" + VIRGUL_EKLE( query.VALUE( "brut_ucret" ).toString(), 2 ) );

           M_KAZANC_BILGILERI << SATIR_DATA;
       }
    }

    // SOSYAL VE OZEL YARDIMLAR
    query.PREPARE_SELECT( "prs_brd_kesinti_odenekleri, prs_bordrolar",
                          "kesinti_odenek_ismi, tutari, kesinti_odenek_turu",
                          "prs_bordrolar.bordro_id = prs_brd_kesinti_odenekleri.bordro_id "
                          "AND bordro_donemi_ay  = :bordro_donemi_ay         "
                          "AND bordro_donemi_yil = :bordro_donemi_yil        ");

    query.SET_VALUE     ( ":bordro_donemi_ay"   , M_UCRET_RV->bordro_ay   );
    query.SET_VALUE     ( ":bordro_donemi_yil"  , M_UCRET_RV->bordro_yil  );

    query.AND_EKLE      ( "bordro_personel_id       = :bordro_personel_id" );
    query.SET_VALUE     ( ":bordro_personel_id" , p_personel_id );


    if ( query.SELECT() NE 0 ) {
       while (query.NEXT() EQ true ) {
           SATIR_DATA.clear();
           SATIR_DATA.append( query.VALUE("kesinti_odenek_ismi").toString()
                             + "\t" + VIRGUL_EKLE( query.VALUE( "tutari" ).toString(), 2 ) );

           if ( query.VALUE("kesinti_odenek_turu") EQ ENUM_PRS_ODENEK ) {
                toplam_sosyal_yardim    += query.VALUE( "tutari" ).toDouble();
                M_SOSYAL_YARDIM_BILGILERI << SATIR_DATA;
           }
           else {
                toplam_ozel_kesintiler  += query.VALUE( "tutari" ).toDouble();
                M_OZEL_KESINTILER_BILGILERI << SATIR_DATA;
           }
       }
    }

    // YASAL KESINTILER

    query.PREPARE_SELECT( "prs_bordro_personelleri, prs_bordrolar",
                          "calisilan_gun_sayisi,brd_oncesi_kum_vergi_matrahi, "
                          "brd_sonrasi_kum_vergi_matrahi, ssk_matrahi, gelir_vergi_matrahi,"
                          "gelir_vergisi, damga_vergisi, asg_gec_indirimi, sigorta_primi, issizlik_primi, "
                          "bir_ay_once_kalan_ek_odenek, iki_ay_once_kalan_ek_odenek, kesintiler_toplami",
                          "prs_bordrolar.bordro_id = prs_bordro_personelleri.bordro_id "
                          "AND bordro_donemi_ay  = :bordro_donemi_ay         "
                          "AND bordro_donemi_yil = :bordro_donemi_yil        ");

    query.SET_VALUE     ( ":bordro_donemi_ay"   , M_UCRET_RV->bordro_ay   );
    query.SET_VALUE     ( ":bordro_donemi_yil"  , M_UCRET_RV->bordro_yil  );

    query.AND_EKLE      ( "personel_id       = :personel_id" );
    query.SET_VALUE     ( ":personel_id"    , p_personel_id );


    if ( query.SELECT() NE 0 ) {
       while (query.NEXT() EQ true ) {
           SATIR_DATA.clear();
           SATIR_DATA << QObject::tr( "SSK Gün" )               + "\t" + query.VALUE("calisilan_gun_sayisi").toString();
           SATIR_DATA << QObject::tr( "2 ay Dev. SSK V.M." )    + "\t" + query.VALUE("iki_ay_once_kalan_ek_odenek").toString();
           SATIR_DATA << QObject::tr( "1 ay Dev. SSK V.M." )    + "\t" + query.VALUE("bir_ay_once_kalan_ek_odenek").toString();
           SATIR_DATA << QObject::tr( "Dev. Kümülatif V.M." )   + "\t" + query.VALUE("brd_oncesi_kum_vergi_matrahi").toString();
           SATIR_DATA << QObject::tr( "Top. Kümülatif V.M." )   + "\t" + query.VALUE("brd_sonrasi_kum_vergi_matrahi").toString();
           SATIR_DATA << QObject::tr( "SSK Matrahı" )           + "\t" + query.VALUE("ssk_matrahi").toString();
           SATIR_DATA << QObject::tr( "SSK Primi" )             + "\t" + query.VALUE("sigorta_primi").toString();
           SATIR_DATA << QObject::tr( "GV matrahı" )            + "\t" + query.VALUE("gelir_vergi_matrahi").toString();
           SATIR_DATA << QObject::tr( "DV matrahı" )            + "\t" + query.VALUE("gelir_vergi_matrahi").toString();
           SATIR_DATA << QObject::tr( "Gelir Vergisi" )         + "\t" + query.VALUE("gelir_vergisi").toString();
           SATIR_DATA << QObject::tr( "Damga Vergisi" )         + "\t" + query.VALUE("damga_vergisi").toString();
           SATIR_DATA << QObject::tr( "İssizlik Sig." )         + "\t" + query.VALUE("issizlik_primi").toString();
           SATIR_DATA << QObject::tr( "A.G.İ." )                + "\t" + query.VALUE("asg_gec_indirimi").toString();

           toplam_yasal_kesintiler = query.VALUE("kesintiler_toplami").toDouble();
           M_YASAL_KESINTILER_BILGILERI << SATIR_DATA;
       }
    }


}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_RAPORU::FILL_RAPOR_BILGILER();
***************************************************************************************/

void PRS_UCRET_PUSULASI_RAPORU::FILL_RAPOR_BILGILER(int p_personel_id )
{
    CARI_HESAP_STRUCT CARI_HESAP;
    QStringList RAPOR_SATIRI;

    SQL_QUERY query     ( DB );
    SQL_QUERY s_query   ( DB );

    query.PREPARE_SELECT( "prs_personeller, prs_personel_kisisel_blgler",
                          "sigorta_no, brut_ucret, calisma_sekli, ise_giris_tarihi, cari_hesap_id " ,
                          "prs_personeller.personel_id = prs_personel_kisisel_blgler.personel_id");


    query.AND_EKLE  ( " prs_personeller.personel_id = :personel_id");
    query.SET_VALUE ( ":personel_id", p_personel_id );

    s_query.PREPARE_SELECT( "prs_isten_ayrilanlar", "isten_ayrilma_tarihi" );

    s_query.AND_EKLE  ( " personel_id = :personel_id");
    s_query.SET_VALUE ( ":personel_id", p_personel_id );


    QString isten_ayrilma_tarihi;
    if ( s_query.SELECT() NE 0 ) {
        s_query.NEXT();
        isten_ayrilma_tarihi = s_query.VALUE( 0 ).toString();
    }

    RAPOR_SATIRI.clear();

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {

            CARI_KART_BILGILERINI_OKU( query.VALUE("cari_hesap_id").toInt() , &CARI_HESAP );

            QString ucret_tipi;
            if ( query.VALUE("calisma_sekli") EQ ENUM_AYLIK ) {
                ucret_tipi  = "AYLIK";
            }
            else {
                ucret_tipi  = "GÜNLÜK";
            }

            RAPOR_SATIRI.append(  CARI_HESAP.cari_hesap_kodu
                            + "\t" + CARI_HESAP.cari_hesap_ismi
                            + "\t" + CARI_HESAP.kimlik_no
                            + "\t" + query.VALUE("ise_giris_tarihi").toString()
                            + "\t" + isten_ayrilma_tarihi
                            + "\t" + "-"
                            + "\t" + ucret_tipi
                            + "\t" + query.VALUE("brut_ucret").toString()
                            + "\t" + query.VALUE("sigorta_no").toString() );

            M_RAPOR_BILGILER << RAPOR_SATIRI;
        }
    }
}











