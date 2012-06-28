#include "muh_bilanco_raporu_class.h"
#include "muh_bilanco_raporu_open.h"
#include "adak_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_enum.h"
#include "yonetim.h"
#include "e9_console_utils.h"
#include "muh_console_utils.h"

extern ADAK_SQL *          DB;

/**************************************************************************************
                   GET_BILANCO_RAPORU
***************************************************************************************/

QString GET_BILANCO_RAPORU (MUH_RAPOR_VARS * rapor_vars)
{
    BILANCO_RAPORU    * R = new BILANCO_RAPORU(rapor_vars);
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   BILANCO_RAPORU::BILANCO_RAPORU
***************************************************************************************/

BILANCO_RAPORU::BILANCO_RAPORU(MUH_RAPOR_VARS * rapor_vars) : REPORT_KERNEL ("MUH_BILANCO_RAPORU")
{
    SET_HEIGHTS ( 30 , 0 );

    M_MUH_RV = rapor_vars;

    m_max_yazdirilacak_satir_sayisi = GET_MAX_LINE_COUNT (8);

    m_yazdirilan_kayit_sayisi       = 0;
    m_bilanco_hesaplari_sayisi      = 0;
    m_bilanco_hesaplari_indis_no    = 0;
    m_last_hesap_indisi             = 0;

    m_aktif_finished                = false;
    m_pasif_finished                = false;

    BILANCO_HESAPLARI = new MUH_BILANCO_HESAPLARI_STRUCT [ MAX_BILANCO_HESAPLARI_SAYISI ];
    FILL_BILANCO_TABLOSU();
}

/**************************************************************************************
                   BILANCO_RAPORU::GET_HEADER
***************************************************************************************/

QString BILANCO_RAPORU::GET_HEADER()
{
    QString baslangic_tarihi = M_MUH_RV->baslangic_tarihi.toString ( "dd MMMM yyyy dddd" );
    QString bitis_tarihi     = M_MUH_RV->bitis_tarihi.toString ( "dd MMMM yyyy dddd" );

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%;\" BORDER=\"0\">" );

    SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:25%;\""<<"style=\"width:45%;\" ALIGN=CENTER"<<"style=\"width:30%;\" ALIGN=RIGHT");

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,14 );

    ADD_ROW_TO_TABLE     ( QStringList()  << E9_GET_FIRMA_ISMI() << "<b>BİLANÇO TABLOSU</b>" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList()  << "" << baslangic_tarihi + "-" + bitis_tarihi << "" );

    ADD_ROW_TO_TABLE     ( QStringList()  << " " << " " << " " );
    ADD_ROW_TO_TABLE     ( QStringList()  << " " << " " << " " );

    SET_TD_TAG_STRING    ( QStringList()<<"ALIGN=CENTER"<<"ALIGN=CENTER"<<"ALIGN=CENTER");

    ADD_ROW_TO_TABLE     ( QStringList()  << "<b><u>Aktif Kaynaklar" << " " << "<b><u>Pasif Kaynaklar" );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   BILANCO_RAPORU::GET_BODY
***************************************************************************************/

QString BILANCO_RAPORU::GET_BODY()
{
    QStringList     headers;
    QStringList     string_list;
    double          donem_baslangic_bakiyesi;
    double          donem_bitis_bakiyesi;


    if ( m_aktif_finished EQ true AND m_pasif_finished EQ true ) {
        return NULL;
    }

    headers.clear();

    headers << "<b><u>Hesap/Açıklama"
            << "<b><u>31/12/ "  + QVariant ( MALI_YIL_FIRST_DATE().year() -1 ).toString()
            << "<b><u>31/12/"   + QVariant ( MALI_YIL_FIRST_DATE().year()    ).toString()
            << " "
            << "<b><u>Hesap/Açıklama"
            << "<b><u>31/12/ "  + QVariant ( MALI_YIL_FIRST_DATE().year() -1 ).toString()
            << "<b><u>31/12/"   + QVariant ( MALI_YIL_FIRST_DATE().year()    ).toString();


    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:74%;\" BORDER = 0>" );

    SET_TD_TAG_STRING    ( QStringList()<<"style=\"width:35%;\""<<"style=\"width:7%;\" ALIGN=RIGHT"<<"style=\"width:7%;\" ALIGN=RIGHT" <<"style=\"width:2%;\" ALIGN=RIGHT"
                                        <<"style=\"width:35%;\""<<"style=\"width:7%;\" ALIGN=RIGHT"<<"style=\"width:7%;\" ALIGN=RIGHT" );

    CREATE_TABLE ( headers , 1 , 14 );

    SET_TD_TAG_STRING ( QStringList() << "" << "ALIGN=RIGHT" << "ALIGN=RIGHT" << " " << "" << "ALIGN=RIGHT" << "ALIGN=RIGHT"  );

    m_yazdirilan_kayit_sayisi = 0;


    for ( ; ; ) {
        donem_baslangic_bakiyesi = 0.0;
        donem_bitis_bakiyesi     = 0.0;

        string_list.clear();

        int index = 0 ;

        for ( int i = 0 ; i < 2 ; i++ ) {


            if ( i EQ 0 ) {
                index = m_aktif_index;
                if ( m_aktif_index > m_pasif_baslangic_index ) {
                    string_list << " " << " " << " ";
                    m_aktif_finished = true;
                    continue;
                }
            }

            if ( i EQ 1 ) {
                string_list << " ";
                index = m_pasif_index;
                if ( m_pasif_index > m_bilanco_hesaplari_sayisi ) {
                    string_list << " " << " " << " ";
                    m_pasif_finished = true;
                    continue;
                }
            }

            if ( m_aktif_finished EQ true AND m_pasif_finished EQ true ) {
                break;
            }

            QString hesap_ismi = BILANCO_HESAPLARI [ index ].hesap_ismi;

            int hesap_id       = BILANCO_HESAPLARI [ index ].hesap_id;

            if ( hesap_id > 0 ) {
                hesap_ismi.prepend ( "-----------" );
            }
            if ( hesap_id EQ -1 ) {
                hesap_ismi.prepend ( "----" );
            }

            if ( BILANCO_HESAPLARI[index].baslangic_borc_bakiyesi > BILANCO_HESAPLARI[index].baslangic_alacak_bakiyesi ) {
                donem_baslangic_bakiyesi = BILANCO_HESAPLARI [ index ].baslangic_borc_bakiyesi -
                                           BILANCO_HESAPLARI [ index ].baslangic_alacak_bakiyesi;
            }
            else {
                donem_baslangic_bakiyesi = BILANCO_HESAPLARI [ index ].baslangic_alacak_bakiyesi -
                                           BILANCO_HESAPLARI [ index ].baslangic_borc_bakiyesi;
            }


            if ( BILANCO_HESAPLARI[index].bitis_borc_bakiyesi > BILANCO_HESAPLARI[index].bitis_alacak_bakiyesi) {
                donem_bitis_bakiyesi     = BILANCO_HESAPLARI [ index ].bitis_borc_bakiyesi     -
                                           BILANCO_HESAPLARI [ index ].bitis_alacak_bakiyesi;
            }
            else {
                donem_bitis_bakiyesi     = BILANCO_HESAPLARI [ index ].bitis_alacak_bakiyesi     -
                                           BILANCO_HESAPLARI [ index ].bitis_borc_bakiyesi;
            }


            if ( hesap_id EQ TOPLAM_SATIRI  OR hesap_id EQ HESAP_GRUP_SATIRI OR hesap_id EQ HESAP_SINIF_SATIRI ) {
                string_list << "<b>" + hesap_ismi;

                if (donem_baslangic_bakiyesi NE 0 ) {
                    string_list << "<b>" + VIRGUL_EKLE ( QVariant (ROUND (donem_baslangic_bakiyesi  ) ).toString() );
                }
                else {
                    string_list << "";
                }
                if ( donem_bitis_bakiyesi NE 0 ) {
                    string_list << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( donem_bitis_bakiyesi  ) ).toString() );
                }
                else {
                    string_list << "";
                }
            }
            else {

                string_list << hesap_ismi;

                if ( donem_baslangic_bakiyesi NE 0 ) {
                    string_list << VIRGUL_EKLE ( QVariant ( ROUND (donem_baslangic_bakiyesi  ) ).toString() );
                }
                else {
                    string_list << "";
                }
                if ( donem_bitis_bakiyesi NE 0 ) {
                     string_list << VIRGUL_EKLE ( QVariant ( ROUND ( donem_bitis_bakiyesi  ) ).toString() );
                }
                else {
                    string_list << "";
                }

            }
        }



        if ( m_aktif_finished EQ true AND m_pasif_finished EQ true ) {
            break;
        }

        ADD_ROW_TO_TABLE ( string_list );

        m_yazdirilan_kayit_sayisi++;

        m_aktif_index++;
        m_pasif_index++;

        if ( m_aktif_index > m_pasif_baslangic_index ) {
            m_aktif_finished = true;
        }

        if ( m_pasif_index > m_bilanco_hesaplari_sayisi ) {
            m_pasif_finished = true;
        }

        if ( m_yazdirilan_kayit_sayisi EQ m_max_yazdirilacak_satir_sayisi ) {
            break;
        }
    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   BILANCO_RAPORU::GET_FOOTER
***************************************************************************************/

QString BILANCO_RAPORU::GET_FOOTER()
{
    return NULL;
}
/**************************************************************************************
                   BILANCO_RAPORU::BILANCO_TABLOSU()
***************************************************************************************/

void BILANCO_RAPORU::FILL_BILANCO_TABLOSU()
{

    MUH_HESAP_SINIFLARI_STRUCT MUH_HESAP_SINIFLARI [] = {
        { "1","DÖNEN VARLIKLAR" },
        { "2","DURAN VARLIKLAR" },
        { "3","KISA VADELİ YABANCI KAYNAKLAR"  } ,
        { "4","UZUN VADELİ YABANCI KAYNAKLAR" },
        { "5","ÖZKAYNAKLAR" }
    };

    MUH_HESAP_GRUPLARI_STRUCT MUH_HESAP_GRUPLARI [] = {
        { "10", "HAZIR DEĞERLER" },
        { "11", "MENKUL KIYMETLER" },
        { "12", "TİCARİ ALACAKLAR" },
        { "13", "DİĞER ALACAKLAR" },
        { "15", "STOKLAR" },
        { "17", "YILLARA YAYGIN İNŞAAT VE ONARIM MALİYETLERİ " },
        { "18", "GELECEK AYLARA AİT GİDERLER VE GELİR TAHAKKUKLARI" },
        { "19", "DİĞER DÖNEN VARLIKLAR"},
        { "22", "TİCARİ ALACAKLAR"},
        { "23", "DİĞER ALACAKLAR"},
        { "24", "MALİ DURAN VARLIKLAR"},
        { "25", "MADDİ DURAN VARLIKLAR"},
        { "26", "MADDİ OLMAYAN DURAN VARLIKLAR"},
        { "27", "ÖZEL TÜKENMEYE TABİ VARLIKLAR"},
        { "28", "GELECEK YILLARA AİT GİDERLER VE GELİR TAHAKKUKLARI"},
        { "29", "DİĞER DURAN VARLIKLAR"},
        { "30", "MALİ BORÇLAR"},
        { "32", "TİCARİ BORÇLAR"},
        { "33", "DİĞER BORÇLAR"},
        { "34", "ALINAN AVANSLAR"},
        { "35", "YILLARA YAYGIN İNŞAAT VE ONARIM HAKEDİŞLERİ "},
        { "36", "ÖDENECEK VERGİ VE DİĞER YÜKÜMLÜLÜKLER"},
        { "37", "BORÇ VE GİDER KARŞILIKLARI"},
        { "38", "GELECEK AYLARA AİT GELİRLER VE GİDER TAHAKKUKLARI"},
        { "39", "DİĞER KISA VADELİ YABANCI KAYNAKLAR" },
        { "40", "MALİ BORÇLAR" },
        { "42", "TİCARİ BORÇLAR" },
        { "43", "DİĞER BORÇLAR" },
        { "44", "ALINAN AVANSLAR" },
        { "47", "BORÇ VE DİĞER KARŞILIKLARI" },
        { "48", "GELECEK YILLARA AİT GELİRLER VE GİDER TAHUKKUKLARI" },
        { "49", "DİĞER UZUN VADELİ YABANCI KAYNAKLAR" },
        { "50", "ÖDENMİŞ SERMAYE" },
        { "52", "SERMAYE YEDEKLERİ" },
        { "54", "KAR YEDEKLERİ" },
        { "57", "GEÇMİŞ YILLAR KARLARI" },
        { "58", "GEÇMİŞ YILLAR ZARARLARI" },
        { "59", "DÖNEM NET KARI (ZARARI)" }
    };


    int grup_indis_no  = 0;
    m_bilanco_hesaplari_indis_no = 0;

    for (int i = 0 ; i < BILANCO_SINIFI_SAYISI ; i++ ) {

        //Bilanco sinifi , bilanco hesaplarina ekleniyor.

        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_kodu  = MUH_HESAP_SINIFLARI [i].hesap_kodu;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi.clear();
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi.append(MUH_HESAP_SINIFLARI[i].hesap_kodu);
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi.append("-");
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi.append(MUH_HESAP_SINIFLARI[i].hesap_ismi);

        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_borc_bakiyesi       = 0.00;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_alacak_bakiyesi = 0.00;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_alacak_bakiyesi     = 0.00;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_borc_bakiyesi   = 0.00;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_id = HESAP_SINIF_SATIRI;//SINIF_SATIRI


        for ( int j = 0 ; j < BILANCO_HESAPLARI_GRUP_SAYISI ; j++ ) {

            //Sinifin gruplari bilanco hesaplarina ekleniyor.

            if ( MUH_HESAP_GRUPLARI [j].hesap_kodu[0] NE MUH_HESAP_SINIFLARI [i].hesap_kodu[0] ) {
                continue;
            }
            m_bilanco_hesaplari_indis_no++;
            grup_indis_no = m_bilanco_hesaplari_indis_no;

            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_kodu = MUH_HESAP_GRUPLARI [j].hesap_kodu;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi.clear();
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi.append( MUH_HESAP_GRUPLARI [j].hesap_kodu );
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi.append( "-" );
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi.append( MUH_HESAP_GRUPLARI [j].hesap_ismi );

            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_borc_bakiyesi       = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_alacak_bakiyesi = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_alacak_bakiyesi     = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_borc_bakiyesi   = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_id = HESAP_GRUP_SATIRI;//GRUP SATIRI

            GRUBUN_HESAPLARINI_BILANCO_HESAPLARINA_EKLE ( grup_indis_no , MUH_HESAP_GRUPLARI [j].hesap_kodu );
            //Gruba ait muhasebe ana hesaplari , bilanco siniflarina ekleniyor.

        }
        m_bilanco_hesaplari_indis_no++;

        //Grubun toplam satiri ekleniyor.

        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_kodu = MUH_HESAP_SINIFLARI [i].hesap_kodu;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi = MUH_HESAP_SINIFLARI [i].hesap_ismi;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi.append( "  TOPLAMI" );
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_id = TOPLAM_SATIRI;//GRUP TOPLAM SATIRI

        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_borc_bakiyesi   = 0.00;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_borc_bakiyesi       = 0.00;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_alacak_bakiyesi = 0.00;
        BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_alacak_bakiyesi     = 0.00;

        SINIF_BAKIYESINI_BILANCO_HESAPLARINA_EKLE ( m_bilanco_hesaplari_indis_no , MUH_HESAP_SINIFLARI [i].hesap_kodu) ;

        if ( i EQ 1 ) {//DURAN VARLIKLAR

            m_bilanco_hesaplari_indis_no++;

            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no].hesap_kodu  = "2__" ;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi = "<b>AKTİF VARLIKLAR TOPLAMI....................." ;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_id = TOPLAM_SATIRI;

            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no].baslangic_borc_bakiyesi   = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no].bitis_borc_bakiyesi       = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_alacak_bakiyesi = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_alacak_bakiyesi     = 0.00;

            SINIF_BAKIYESINI_BILANCO_HESAPLARINA_EKLE ( m_bilanco_hesaplari_indis_no , "1" );
            SINIF_BAKIYESINI_BILANCO_HESAPLARINA_EKLE ( m_bilanco_hesaplari_indis_no , "2" );

            //BILANCOYA_FARK_BAKIYESINI_EKLE(m_bilanco_hesaplari_indis_no , true);

            m_pasif_baslangic_index = m_bilanco_hesaplari_indis_no;
        }

        if (i EQ 4 ) {

            m_bilanco_hesaplari_indis_no++;

            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no].hesap_kodu  = "5__";
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_ismi = "<b>PASİF  KAYNAKLAR TOPLAMI..................";
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_id = TOPLAM_SATIRI;

            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_borc_bakiyesi   = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_borc_bakiyesi       = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_alacak_bakiyesi = 0.00;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_alacak_bakiyesi     = 0.00;

            SINIF_BAKIYESINI_BILANCO_HESAPLARINA_EKLE ( m_bilanco_hesaplari_indis_no , "3" );
            SINIF_BAKIYESINI_BILANCO_HESAPLARINA_EKLE ( m_bilanco_hesaplari_indis_no , "4" );
            SINIF_BAKIYESINI_BILANCO_HESAPLARINA_EKLE ( m_bilanco_hesaplari_indis_no , "5" ) ;


        }
        m_bilanco_hesaplari_indis_no++;

    }
    m_bilanco_hesaplari_sayisi = m_bilanco_hesaplari_indis_no-1;

    m_aktif_index = 0;
    m_pasif_index = m_pasif_baslangic_index + 1;
}

/**************************************************************************************
                   BILANCO_RAPORU::HESAP_BAKIYELERI
***************************************************************************************/

void BILANCO_RAPORU::GRUBUN_HESAPLARINI_BILANCO_HESAPLARINA_EKLE ( int grup_indisi,QString grup_kodu )
{
    SQL_QUERY             sql_query ( DB );

    double borc_bakiyesi   [ E9_ARRAY_SIZE ];
    double alacak_bakiyesi [ E9_ARRAY_SIZE ];

    int bas_gun_no = MALI_YIL_ARRAY_INDIS ( M_MUH_RV->baslangic_tarihi );
    int bts_gun_no = MALI_YIL_ARRAY_INDIS ( M_MUH_RV->bitis_tarihi );

    double donem_kari   = 0.0;
    double donem_zarari = 0.0;

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id,tam_hesap_kodu,hesap_ismi,toplam_borc_array,"
                             "toplam_alacak_array " , "hesap_seviyesi = :hesap_seviyesi");

    sql_query.SET_VALUE(":hesap_seviyesi" , 1);

    if ( sql_query.SELECT() > 0 ) {

        while ( sql_query.NEXT() ) {

            QString tam_hesap_kodu = sql_query.VALUE(1).toString();


            if ( tam_hesap_kodu.startsWith(grup_kodu) EQ false ) {
                continue;
            }

            int hesap_id       = sql_query.VALUE(0).toInt();
            QString hesap_ismi = sql_query.VALUE(2).toString();

            UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(3).toString() , borc_bakiyesi   , E9_ARRAY_SIZE );
            UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(4).toString() , alacak_bakiyesi , E9_ARRAY_SIZE );

            if ( M_MUH_RV->bakiyesiz_hesaplar EQ 0 ) {
                if ( grup_kodu NE "59") {
                    if ( borc_bakiyesi[bts_gun_no] EQ 0 AND alacak_bakiyesi[bts_gun_no] EQ 0 ) {
                        continue;
                    }
                }
            }


            m_bilanco_hesaplari_indis_no++;

            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].hesap_id = hesap_id;

            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no].hesap_kodu = tam_hesap_kodu;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no].hesap_ismi = "       " ;
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no].hesap_ismi.append( tam_hesap_kodu );
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no].hesap_ismi.append( "-" );
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no].hesap_ismi.append( hesap_ismi );


            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_borc_bakiyesi  = borc_bakiyesi [ bas_gun_no-1 ];
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].baslangic_alacak_bakiyesi= alacak_bakiyesi [ bas_gun_no-1 ];
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_borc_bakiyesi      = borc_bakiyesi [ bts_gun_no ];
            BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_alacak_bakiyesi    = alacak_bakiyesi [ bts_gun_no ];

            if ( grup_kodu EQ "59") {

                MUH_GET_FROM_GELIR_TABLOSU_DONEM_NET_KARI_ZARARI(donem_kari , donem_zarari, bts_gun_no);
                if ( tam_hesap_kodu EQ "590") {

                    //692 donem net_kari/ zarari hesabindan aktarilan bakiye kar ise ,590 hesabina alacak_yazilir.
                    BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_alacak_bakiyesi    += donem_kari;

                }
                else if ( tam_hesap_kodu EQ "591") {
                    //692 donem net kari/zarari hesabindan aktarilan bakiye zarar ise , 591 hesabinin borcuna yazilir.
                    BILANCO_HESAPLARI [ m_bilanco_hesaplari_indis_no ].bitis_borc_bakiyesi    += donem_zarari;
                }
            }
            //Grup bakiyeleri arttiriliyor

            BILANCO_HESAPLARI [ grup_indisi ].baslangic_borc_bakiyesi     += borc_bakiyesi [ bas_gun_no-1 ];
            BILANCO_HESAPLARI [ grup_indisi ].baslangic_alacak_bakiyesi   += alacak_bakiyesi [ bas_gun_no-1 ];
            BILANCO_HESAPLARI [ grup_indisi ].bitis_borc_bakiyesi         += borc_bakiyesi [ bts_gun_no ];
            BILANCO_HESAPLARI [ grup_indisi ].bitis_alacak_bakiyesi       += alacak_bakiyesi [ bts_gun_no ];

            if ( grup_kodu EQ "59") {

                if ( tam_hesap_kodu EQ "590") {

                    //692 donem net_kari/ zarari hesabindan aktarilan bakiye kar ise ,590 hesabina alacak_yazilir.
                    BILANCO_HESAPLARI [ grup_indisi ].bitis_alacak_bakiyesi         += donem_kari;
                }
                else if ( tam_hesap_kodu EQ "591") {
                    //692 donem net kari/zarari hesabindan aktarilan bakiye zarar ise , 591 hesabinin borcuna yazilir.
                    BILANCO_HESAPLARI [ grup_indisi ].bitis_borc_bakiyesi       += donem_zarari;
                }
            }
        }
    }

}


/**************************************************************************************
                   BILANCO_RAPORU::SINIF_BAKIYESINI_BILANCO_HESAPLARINA_EKLE
***************************************************************************************/

void BILANCO_RAPORU::SINIF_BAKIYESINI_BILANCO_HESAPLARINA_EKLE ( int p_sinif_indisi, QString p_sinif_kodu )
{
    if ( p_sinif_kodu.isEmpty() EQ true ) {
        return;
    }

    //Hesap sinifina ait olan hesap gruplarinin bakiyeleri toplanip toplam_satirina yaziliyor.

    for ( int i = 0 ; i < m_bilanco_hesaplari_indis_no ; i++ ) {

        if (BILANCO_HESAPLARI [ i ].hesap_kodu.size() NE 2 ) {
            //HESAP GRUBU DEGILSE
            continue;
        }

        if ( BILANCO_HESAPLARI [ i ].hesap_kodu.startsWith(p_sinif_kodu) EQ false ) {
            //HESAP GRUBU SINIFA AIT DEGILSE
            continue;
        }

        BILANCO_HESAPLARI[p_sinif_indisi].baslangic_borc_bakiyesi   += BILANCO_HESAPLARI [ i ].baslangic_borc_bakiyesi;
        BILANCO_HESAPLARI[p_sinif_indisi].baslangic_alacak_bakiyesi += BILANCO_HESAPLARI [ i ].baslangic_alacak_bakiyesi;
        BILANCO_HESAPLARI[p_sinif_indisi].bitis_borc_bakiyesi       += BILANCO_HESAPLARI [ i ].bitis_borc_bakiyesi;
        BILANCO_HESAPLARI[p_sinif_indisi].bitis_alacak_bakiyesi     += BILANCO_HESAPLARI [ i ].bitis_alacak_bakiyesi;
    }
}

/**************************************************************************************
                   BILANCO_RAPORU::BILANCOYA_FARK_BAKIYESINI_EKLE
***************************************************************************************/

void BILANCO_RAPORU::BILANCOYA_FARK_BAKIYESINI_EKLE(int indis_no , bool aktif_varlik_satiri_mi)
{
    //Yil sonu yansitma ve gelir tablosu islemleri yapilmadan once bilancoda aktifler ile pasifler
    //birbirini tutuyor.

    //Aradaki fark bakiyesi 600 ile 799 arasindaki hesaplarin verdigi borc veya alacak bakiye farkina gore bilancodaki
    //akti veya pasif toplamlara yansitiliyor.

    //fark bakiyesi borc bakiye ise aktif , alacak bakiye ise pasif toplamlara yansitiliyor.

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("muh_hesaplar" , "toplam_borc_array , toplam_alacak_array ",
                         "tam_hesap_kodu >= :tam_hesap_kodu_1 AND "
                         "tam_hesap_kodu <= :tam_hesap_kodu_2 AND hesap_seviyesi = :hesap_seviyesi");


    query.SET_VALUE(":tam_hesap_kodu_1" , "600");
    query.SET_VALUE(":tam_hesap_kodu_2" , "799");
    query.SET_VALUE(":hesap_seviyesi"   , 1);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    double borc_bakiyesi [E9_ARRAY_SIZE];
    double alacak_bakiyesi[E9_ARRAY_SIZE];

    int bts_gun_no = MALI_YIL_ARRAY_INDIS ( M_MUH_RV->bitis_tarihi );

    double toplam_borc_bakiye = 0.0;
    double toplam_alacak_bakiye = 0.0;
    double fark_bakiye = 0.0;

    while ( query.NEXT() ) {

        UNPACK_DOUBLE_ARRAY ( query.VALUE("toplam_borc_array").toString() , borc_bakiyesi   , E9_ARRAY_SIZE );
        UNPACK_DOUBLE_ARRAY ( query.VALUE("toplam_alacak_array").toString() , alacak_bakiyesi , E9_ARRAY_SIZE );

        if ( borc_bakiyesi[bts_gun_no] EQ alacak_bakiyesi[bts_gun_no] ) {
            continue;
        }

        double toplam_borc = borc_bakiyesi[bts_gun_no];
        double toplam_alacak = alacak_bakiyesi[bts_gun_no];

        if ( toplam_borc > toplam_alacak ) {
            toplam_borc_bakiye += ROUND(toplam_borc - toplam_alacak );
        }
        else {
            toplam_alacak_bakiye += ROUND(toplam_alacak - toplam_borc );
        }
    }

    ROUND(toplam_borc_bakiye   );
    ROUND(toplam_alacak_bakiye );

    fark_bakiye = 0;

    if ( toplam_borc_bakiye > toplam_alacak_bakiye ) {
        fark_bakiye = ROUND(toplam_borc_bakiye - toplam_alacak_bakiye);

        if ( aktif_varlik_satiri_mi EQ true ) {
            BILANCO_HESAPLARI[indis_no].bitis_borc_bakiyesi += fark_bakiye;
        }
    }
    else {
        fark_bakiye = ROUND(toplam_alacak_bakiye - toplam_borc_bakiye);

        if ( aktif_varlik_satiri_mi EQ false ) {
            BILANCO_HESAPLARI[indis_no].bitis_alacak_bakiyesi += fark_bakiye;
        }
    }
}












