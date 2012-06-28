#include <QDate>
#include <QVariant>
#include "adak_sql.h"
#include "QCommaEdit.h"
#include "muh_yevmiye_dokumu_raporu_class.h"
#include "muh_yevmiye_dokumu_raporu_open.h"
#include "adak_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "muh_console_utils.h"
 

extern ADAK_SQL *          DB;

/**************************************************************************************
                   GET_YEVMIYE_DOKUMU_RAPORU
***************************************************************************************/

void GET_YEVMIYE_DOKUMU_RAPORU ( double font_size ,double sayfa_genisligi,double sayfa_yuksekligi,MUH_RAPOR_VARS * rapor_vars)
{
    YEVMIYE_DOKUMU_RAPORU * R = new YEVMIYE_DOKUMU_RAPORU ( font_size , sayfa_genisligi , sayfa_yuksekligi , rapor_vars );
    R->CREATE_HTML_PAGES();
    R->OPEN_PRINTER_PREVIEW(NULL);
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::YEVMIYE_DOKUMU_RAPORU
***************************************************************************************/

YEVMIYE_DOKUMU_RAPORU::YEVMIYE_DOKUMU_RAPORU ( double font_size ,double sayfa_genisligi,double sayfa_yuksekligi,MUH_RAPOR_VARS * rapor_vars) : REPORT_KERNEL ("MUH_YEVMIYE_DOKUMU_RAPORU")
{
    M_MUH_RV = rapor_vars;

    SET_HEIGHTS ( QVariant ( font_size ).toInt() * 3, QVariant ( font_size ).toInt() );

    SET_PAPER_SIZE (sayfa_genisligi,sayfa_yuksekligi);

    m_max_line_count             = GET_MAX_LINE_COUNT ( QVariant ( font_size ).toInt() );
    m_gonderilen_satir_sayisi    = 0;

    m_last_fis_indisi            = 0;
    m_fis_satiri_indisi          = 0;

    m_tum_fisler_basildi         = false;
    m_fis_satirlari_basildi      = true;

    m_borc_satirlari_basildi     = false;
    m_alacak_satirlari_basildi   = false;

    m_madde_toplami_basildi      = false;

    m_footer_toplam_borc         = 0.0;
    m_footer_toplam_alacak       = 0.0;
    m_madde_borc_toplami         = 0.0;
    m_madde_alacak_toplami       = 0.0;

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT (  "e9_sabit_degerler" , "firma_ismi");

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        m_firma_ismi = sql_query.VALUE(0).toString();
    }
    m_font_size = font_size;

    M_MUH_HESAPLAR              = MUH_GET_HESAPLAR ( );
    m_raporlanacak_kayit_var_mi = FILL_STRUCT ();
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::GET_HEADER
***************************************************************************************/

QString YEVMIYE_DOKUMU_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( QString  ( "<TABLE WIDTH=100% style=\"font-size:%1pt;\" BORDER=0>" ).arg ( QVariant ( m_font_size ).toString() ) );
    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=25%"<<"WIDTH=50% ALIGN=CENTER"<<"WIDTH=25% ALIGN=RIGHT" );
    CREATE_TABLE         ( QStringList() << "" << "" << "" , 0 , 0 );

    ADD_ROW_TO_TABLE     ( QStringList() << m_firma_ismi <<""<< "" );

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "" << "" );

    ADD_ROW_TO_TABLE     ( QStringList() << "" <<"<b>YEVMİYE DEFTERİ"<<"" );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::GET_BODY
***************************************************************************************/

QString YEVMIYE_DOKUMU_RAPORU::GET_BODY()
{

    if ( m_raporlanacak_kayit_var_mi EQ false ) {
        return NULL;
    }

    QStringList             STRING_LIST;
    QString                 hesap_kodu_adi;
    int                     parent_id;

    SET_TABLE_TAG_STRING ( QString ( "<TABLE WIDTH = 100% style=\"font-size:%1pt;\" BORDER = 0 >" ).arg ( QVariant ( m_font_size ).toString() ) );

    if ( M_MUH_RV->sadece_ana_hesaplar EQ true ) {
        SET_TD_TAG_STRING ( QStringList() <<"WIDTH = 8%" << "WIDTH = 25% " << "WIDTH = 25% ALIGN=RIGHT"
                                          <<"WIDTH = 21% ALIGN=RIGHT" << "WIDTH = 21% ALIGN=RIGHT");
        STRING_LIST << "<b>Yvm.No" << "<b>Hesap Kodu/Adı" << "<b>Açıklama" << "<b>Borç" << "<b>Alacak";
        CREATE_TABLE ( STRING_LIST, 0, 13 );
    }
    else {
        SET_TD_TAG_STRING ( QStringList() <<"WIDTH = 8%" << "WIDTH = 24% " << "WIDTH = 24% ALIGN=CENTER"
                                          <<"WIDTH = 11% ALIGN=RIGHT"      << "WIDTH = 11% ALIGN=RIGHT"
                                          <<"WIDTH = 11% ALIGN=RIGHT"      << "WIDTH = 11% ALIGN=RIGHT");

        STRING_LIST << "<b>Yvm.No" << "<b>Hesap Kodu/Adı"<< "<b>Açıklama" << "<b>Alt Hsp Borç"
                    << "<b>Alt Hsp Alacak" << "<b>Borç" << "<b>Alacak";
        CREATE_TABLE ( STRING_LIST, 0, 13 );
    }

    SET_TD_TAG_STRING ( QStringList() << "" << "" << "ALIGN=CENTER" << "ALIGN=RIGHT" << "ALIGN=RIGHT"
                                      << "ALIGN=RIGHT"<<"ALIGN=RIGHT");
    parent_id  = 0;

    if ( m_tum_fisler_basildi EQ true ) {
        for ( int i = 0 ; i < m_fis_sayisi ; i++) {
            delete [] M_FISLER[i].fis_satirlari;
        }
        delete [] M_KOK_HESAP_BUFFER;
        delete [] M_FISLER;
        return NULL;
    }

    if ( m_gonderilen_satir_sayisi NE 0  AND m_gonderilen_satir_sayisi >= m_max_line_count ) {
         DEVIR_BAKIYESINI_YAZDIR ();
    }

    if ( m_borc_satirlari_basildi EQ true AND m_alacak_satirlari_basildi EQ true ) {
        m_borc_satirlari_basildi   = false;
        m_alacak_satirlari_basildi = false;
        m_fis_satirlari_basildi    = true;
        M_BASILMIS_UST_HESAPLAR.clear();
    }

    int         fis_id;
    QString     fis_tarihi;
    int         yevmiye_no;

    for ( int i = m_last_fis_indisi; i < m_fis_sayisi; i++ ) {       // Tum fislerin sirayla ele alindigi for dongusu

        m_madde_toplami_basildi = false;

        fis_id     = M_FISLER[i].fis_id;
        fis_tarihi = M_FISLER[i].fis_tarihi;
        yevmiye_no = M_FISLER[i].yevmiye_no;

        if ( m_fis_satirlari_basildi EQ true ) {

            SET_TD_TAG_STRING ( QStringList() << "" << "" << "ALIGN=CENTER" << "ALIGN=RIGHT" <<
                                "ALIGN=RIGHT" << "ALIGN=RIGHT" << "ALIGN=RIGHT");
            YEVMIYE_FISI_BASLIGINI_YAZDIR ( yevmiye_no, fis_tarihi );

            m_fis_satirlari_basildi    = false;
            m_borc_satirlari_basildi   = false;
            m_alacak_satirlari_basildi = false;
        }

        if ( m_gonderilen_satir_sayisi >= m_max_line_count ) {
            return GET_TABLE_HTML_STRING();
        }

        if ( M_MUH_RV->type_hesap_order EQ ENUM_ORDER_BORC_ALACAK ) {    //BORC ALACAGA gore sirali

            /////////////////////   BORC SATIRLARI BASILIYOR       /////////////////////////////////

            if ( m_borc_satirlari_basildi EQ false ) {
                for ( int j = m_fis_satiri_indisi; j < M_FISLER[i].fis_satirlari_sayisi ; j++ ) {
                    if (M_FISLER[i].fis_satirlari[j].borc_tutari EQ 0.00)  {
                        continue;
                    }
                    STRING_LIST.clear();

                    if ( M_MUH_RV->sadece_ana_hesaplar EQ true ) {
                        if ( M_FISLER[i].fis_satirlari[j].parent_id EQ 0 ) {
                            hesap_kodu_adi        = M_FISLER[i].fis_satirlari[j].tam_hesap_kodu;
                            hesap_kodu_adi.append("   ");
                            hesap_kodu_adi.append(M_FISLER[i].fis_satirlari[j].hesap_ismi);
                        }
                        else {
                            hesap_kodu_adi        = M_FISLER[i].fis_satirlari[j].kok_hesap.tam_hesap_kodu;
                            hesap_kodu_adi.append ( "   " );
                            hesap_kodu_adi.append ( M_FISLER[i].fis_satirlari[j].kok_hesap.hesap_ismi );
                        }
                    }
                    else {
                        parent_id          = M_FISLER[i].fis_satirlari[j].parent_id;

                        if ( parent_id NE 0 ) {

                            int parent_index  = FIND_FIS_INDEX ( fis_id );

                            hesap_kodu_adi = ANA_HESAPLARI_YAZDIR_VE_TALI_HESAP_KODUNU_AL( M_FISLER[i].fis_satirlari[j].parent_id,
                                                                                           M_FISLER[i].fis_satirlari[j].hesap_seviyesi,
                                                                                           M_FISLER[i].fis_satirlari[j].tam_hesap_kodu,
                                                                                           M_FISLER[i].fis_satirlari[j].hesap_ismi,
                                                                                           parent_index,true);
                        }
                        if ( m_gonderilen_satir_sayisi >= m_max_line_count) {
                             m_fis_satirlari_basildi = false;
                            return GET_TABLE_HTML_STRING();
                        }
                        if ( parent_id EQ 0 ) {
                            hesap_kodu_adi = M_FISLER[i].fis_satirlari[j].tam_hesap_kodu;
                            hesap_kodu_adi.append("   ");
                            hesap_kodu_adi.append ( M_FISLER[i].fis_satirlari[j].hesap_ismi );
                        }
                    }

                    if ( M_BASILMIS_UST_HESAPLAR.contains ( QString ( "basilan fis index = %1" ).arg ( QVariant(j).toString() ) ) EQ true ) {
                        continue;
                    }


                    double borc_tutari      = M_FISLER[i].fis_satirlari[j].borc_tutari;
                    double alacak_tutari    = M_FISLER[i].fis_satirlari[j].alacak_tutari;

                    m_madde_borc_toplami   += borc_tutari;
                    m_madde_alacak_toplami += alacak_tutari;

                    m_footer_toplam_borc   += borc_tutari;
                    m_footer_toplam_alacak += alacak_tutari;

                    STRING_LIST << "" << hesap_kodu_adi << M_FISLER[i].fis_satirlari[j].aciklama;

                    STRING_LIST << VIRGUL_EKLE ( QVariant(borc_tutari).toString());

                    STRING_LIST << VIRGUL_EKLE ( QVariant(alacak_tutari).toString());

                    STRING_LIST << "" << "";

                    ADD_ROW_TO_TABLE ( STRING_LIST );

                    M_BASILMIS_UST_HESAPLAR.append ( QString ( "basilan fis index = %1" ).arg ( QVariant(j).toString() ) ) ;

                    m_gonderilen_satir_sayisi++;
                    m_fis_satiri_indisi++;

                    if ( m_gonderilen_satir_sayisi >= m_max_line_count ) {
                        m_fis_satirlari_basildi = false;
                        return GET_TABLE_HTML_STRING();
                    }

                }
                m_borc_satirlari_basildi = true;
                m_fis_satiri_indisi      = 0;
                M_BASILMIS_UST_HESAPLAR.clear();

            }

////////////////////////    ALACAK SATIRLARI BASILIYOR   ////////////////////////////////

            if ( m_alacak_satirlari_basildi EQ false ) {
                for ( int j = m_fis_satiri_indisi; j < M_FISLER[i].fis_satirlari_sayisi ; j++ ) {
                    if ( M_FISLER[i].fis_satirlari[j].alacak_tutari EQ 0.00 )  {
                        continue;
                    }
                    STRING_LIST.clear();

                    if ( M_MUH_RV->sadece_ana_hesaplar EQ true ) {
                        if ( M_FISLER[i].fis_satirlari[j].parent_id EQ 0 ) {
                            hesap_kodu_adi = M_FISLER[i].fis_satirlari[j].tam_hesap_kodu;
                            hesap_kodu_adi.append ( "   " );
                            hesap_kodu_adi.append ( M_FISLER[i].fis_satirlari[j].hesap_ismi );

                        }
                        else {
                            hesap_kodu_adi  = M_FISLER[i].fis_satirlari[j].kok_hesap.tam_hesap_kodu;
                            hesap_kodu_adi.append ( "   " );
                            hesap_kodu_adi.append ( M_FISLER[i].fis_satirlari[j].kok_hesap.hesap_ismi );
                        }
                    }

                    else {
                        parent_id          = M_FISLER[i].fis_satirlari[j].parent_id;

                        if ( parent_id NE 0 ) {

                            int parent_index  = FIND_FIS_INDEX ( fis_id ) ;

                            hesap_kodu_adi = ANA_HESAPLARI_YAZDIR_VE_TALI_HESAP_KODUNU_AL ( M_FISLER[i].fis_satirlari[j].parent_id,
                                                                                            M_FISLER[i].fis_satirlari[j].hesap_seviyesi,
                                                                                            M_FISLER[i].fis_satirlari[j].tam_hesap_kodu,
                                                                                            M_FISLER[i].fis_satirlari[j].hesap_ismi,
                                                                                            parent_index,false );
                        }

                        if ( m_gonderilen_satir_sayisi >= m_max_line_count ) {
                            m_fis_satirlari_basildi = false;
                            return GET_TABLE_HTML_STRING();
                        }

                        if ( parent_id EQ 0 ) {

                            hesap_kodu_adi = M_FISLER[i].fis_satirlari[j].tam_hesap_kodu;
                            hesap_kodu_adi.append ( "    " );
                            hesap_kodu_adi.append ( M_FISLER[i].fis_satirlari[j].hesap_ismi );
                        }
                    }

                    if ( M_BASILMIS_UST_HESAPLAR.contains ( QString ( "basilan fis index = %1" ).arg ( QVariant(j).toString() ) ) EQ true ) {
                        continue;
                    }

                    double borc_tutari      = M_FISLER[i].fis_satirlari[j].borc_tutari;
                    double alacak_tutari    = M_FISLER[i].fis_satirlari[j].alacak_tutari;

                    m_madde_borc_toplami   += borc_tutari;
                    m_madde_alacak_toplami += alacak_tutari;

                    m_footer_toplam_borc   += borc_tutari;
                    m_footer_toplam_alacak += alacak_tutari;

                    STRING_LIST << "" << hesap_kodu_adi << M_FISLER[i].fis_satirlari[j].aciklama;

                    STRING_LIST << VIRGUL_EKLE ( QVariant(borc_tutari).toString() );

                    STRING_LIST << VIRGUL_EKLE ( QVariant(alacak_tutari).toString() );

                    STRING_LIST << "" << "";

                    ADD_ROW_TO_TABLE ( STRING_LIST );

                    M_BASILMIS_UST_HESAPLAR.append ( QString ( "basilan fis index = %1" ).arg(QVariant(j).toString()));

                    m_gonderilen_satir_sayisi++;
                    m_fis_satiri_indisi++;

                    if ( m_gonderilen_satir_sayisi >= m_max_line_count ) {
                        m_fis_satirlari_basildi = false;
                        return GET_TABLE_HTML_STRING();
                    }
                }
                m_alacak_satirlari_basildi = true;
                m_fis_satiri_indisi        = 0;
                M_BASILMIS_UST_HESAPLAR.clear();
            }
        }

        STRING_LIST.clear();

        YEVMIYE_FISI_MADDE_TOPLAMINI_YAZDIR ();

        m_madde_toplami_basildi   = true;
        m_fis_satirlari_basildi   = true;
        m_fis_satiri_indisi       = 0;
        m_last_fis_indisi++;
        m_madde_borc_toplami      = 0.0;
        m_madde_alacak_toplami    = 0.0;

        if ( m_gonderilen_satir_sayisi >= m_max_line_count ) {
            if ( m_last_fis_indisi EQ m_fis_sayisi ) {
                m_tum_fisler_basildi = true;
            }
            return GET_TABLE_HTML_STRING();
        }
    }

    if ( m_madde_toplami_basildi EQ false ) {
        YEVMIYE_FISI_MADDE_TOPLAMINI_YAZDIR ();
        m_madde_toplami_basildi = true;
    }

    m_tum_fisler_basildi = true;
    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::FIND_FIS_INDEX
***************************************************************************************/

int YEVMIYE_DOKUMU_RAPORU::FIND_FIS_INDEX ( int fis_id )
{
    for ( int satir_index = 0; satir_index < m_fis_sayisi ; satir_index++  ) {

        if ( M_FISLER [ satir_index ].fis_id EQ fis_id ) {
            return satir_index;
        }
    }
    return -1;
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::GET_FOOTER
***************************************************************************************/

QString YEVMIYE_DOKUMU_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING ( QString ( "<TABLE WIDTH = 100% style=\"font-size:%1pt;\" BORDER = 0>" ).arg ( QVariant ( m_font_size ).toString() ) );

    if ( M_MUH_RV->sadece_ana_hesaplar ) {

        SET_TD_TAG_STRING ( QStringList() << "WIDTH=28%" << "WIDTH=30% ALIGN=RIGHT"<< "WIDTH=21% ALIGN=RIGHT"<<"WIDTH=21% ALIGN=RIGHT" );
    }
    else {
        SET_TD_TAG_STRING ( QStringList() << "WIDTH=50%" << "WIDTH=28% ALIGN=RIGHT"<< "WIDTH=11% ALIGN=RIGHT"<<"WIDTH=11% ALIGN=RIGHT");
    }

    CREATE_TABLE ( QStringList()<< "" << "" <<""<< "",0,0 );

    QStringList          string_list;

    string_list << ""<<"<b>GENEL TOPLAM<b>" << "<b>"+VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_toplam_borc )).toString())
                << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_toplam_alacak  ) ).toString() );

    ADD_ROW_TO_TABLE ( string_list );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::GET_TUM_FISLER
***************************************************************************************/

QStringList YEVMIYE_DOKUMU_RAPORU::GET_TUM_FISLER ()
{
    SQL_QUERY sql_query ( DB );

    QStringList     FISLER;
    QString         fis_bilgileri;

    QString where_str = " fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi  ";

    if ( M_MUH_RV->deneme_baskisi EQ false ) {
        where_str += " AND yevmiye_numarasi >= :bas_yevmiye_num ";
        if ( M_MUH_RV->bitis_yevmiye_no > 0 ) {
            where_str += " AND yevmiye_numarasi <= :bts_yevmiye_num ";
        }
    }

    sql_query.PREPARE_SELECT ( "muh_fisler" , "fis_id, fis_tarihi, yevmiye_numarasi" , where_str);

    sql_query.SET_VALUE ( ":bas_tarihi"      , M_MUH_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );
    sql_query.SET_VALUE ( ":bts_tarihi"      , M_MUH_RV->bitis_tarihi.toString     ( "yyyy.MM.dd" ) );

    if ( M_MUH_RV->deneme_baskisi EQ false ) {
        sql_query.SET_VALUE ( ":bas_yevmiye_num" , M_MUH_RV->baslangic_yevmiye_no );
        if ( M_MUH_RV->bitis_yevmiye_no > 0 ) {
            sql_query.SET_VALUE ( ":bts_yevmiye_num" , M_MUH_RV->bitis_yevmiye_no );
        }
    }

    if ( M_MUH_RV->deneme_baskisi EQ false ) {
        if ( sql_query.SELECT("yevmiye_numarasi ASC") EQ 0 ) {
            return QStringList();
        }
    }
    else {
        if ( sql_query.SELECT("fis_tarihi ASC") EQ 0 ) {
            return QStringList();
        }
    }


    while ( sql_query.NEXT() EQ true ) {

        QString     fis_no          = sql_query.VALUE(0).toString();
        int         yevmiye_no      = sql_query.VALUE(2).toInt();
        QString     fis_tarihi      = sql_query.VALUE(1).toDate().toString("dd MMMM yyyy dddd");

        fis_bilgileri.clear();
        fis_bilgileri.append ( fis_no + "\t" + fis_tarihi + "\t" + QVariant ( yevmiye_no ).toString() );
        FISLER.append ( fis_bilgileri );

    }
    return FISLER;
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::GET_TUM_FIS_SATIRLARI
***************************************************************************************/

QStringList YEVMIYE_DOKUMU_RAPORU::GET_TUM_FIS_SATIRLARI ()
{
    SQL_QUERY       sql_query(DB);
    QStringList     FIS_SATIRLARI;
    QString         fis_satiri;

    sql_query.PREPARE_SELECT("muh_fis_satirlari, muh_hesaplar, muh_fisler" , "muh_fis_satirlari.hesap_id, muh_fis_satirlari.aciklama, borc_tutari, alacak_tutari, "
                             "muh_fis_satirlari.fis_id, hesap_ismi, hesap_kodu, parent_id, hesap_seviyesi, tam_hesap_kodu",
                             "muh_hesaplar.hesap_id = muh_fis_satirlari.hesap_id "
                             "AND muh_fisler.fis_id = muh_fis_satirlari.fis_id AND "
                             "muh_fisler.fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi");

    sql_query.SET_VALUE ( ":bas_tarihi"      , M_MUH_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );
    sql_query.SET_VALUE ( ":bts_tarihi"      , M_MUH_RV->bitis_tarihi.toString     ( "yyyy.MM.dd" ) );

    if ( sql_query.SELECT("muh_fis_satirlari.fis_id ASC") EQ 0 ) {
        return QStringList();
    }

    while ( sql_query.NEXT() EQ true ) {

        QString hesap_id        = sql_query.VALUE(0).toString();
        QString aciklama        = sql_query.VALUE(1).toString();
        QString borc_tutari     = QVariant ( ROUND ( sql_query.VALUE(2).toDouble())).toString();
        QString alacak_tutari   = QVariant ( ROUND ( sql_query.VALUE(3).toDouble())).toString();
        QString fis_id          = sql_query.VALUE(4).toString();

        QString hesap_ismi      = sql_query.VALUE(5).toString();
        QString hesap_kodu      = sql_query.VALUE(6).toString();
        QString parent_id       = sql_query.VALUE(7).toString();
        QString hesap_seviyesi  = sql_query.VALUE(8).toString();
        QString tam_hesap_kodu  = sql_query.VALUE(9).toString();

        fis_satiri.clear();
        fis_satiri.append ( hesap_id + "\t" + aciklama + "\t" + borc_tutari + "\t" + alacak_tutari + "\t" + fis_id  + "\t" +
                            hesap_ismi + "\t" + hesap_kodu + "\t" + parent_id + "\t" + hesap_seviyesi + "\t" + tam_hesap_kodu );
        FIS_SATIRLARI.append ( fis_satiri );

    }
    return FIS_SATIRLARI;
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::FILL_STRUCT
***************************************************************************************/

bool YEVMIYE_DOKUMU_RAPORU::FILL_STRUCT ()
{
    SQL_QUERY       sql_query_1 ( DB );
    SQL_QUERY       sql_query_2 ( DB );
    int             fis_index = 0;
    int             fis_satirlari_index= 0;

    QString where_str = " fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi  "
                        " AND muh_fisler.fis_id = muh_fis_satirlari.fis_id ";

    if ( M_MUH_RV->deneme_baskisi EQ true ) {
        where_str += " AND yevmiye_numarasi >= :bas_yevmiye_num ";

        if ( M_MUH_RV->bitis_yevmiye_no > 0 ) {
            where_str += " AND yevmiye_numarasi <= :bts_yevmiye_num ";
        }
    }

    sql_query_1.PREPARE_SELECT ( "muh_fisler,muh_fis_satirlari" , "muh_fisler.fis_id, fis_tarihi, "
                                 "yevmiye_numarasi,count(muh_fis_satirlari.fis_id)" , where_str,
                                 "muh_fisler.fis_id, muh_fisler.fis_tarihi, muh_fisler.yevmiye_numarasi, muh_fis_satirlari.fis_id");

    sql_query_1.SET_VALUE ( ":bas_tarihi"      , M_MUH_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );
    sql_query_1.SET_VALUE ( ":bts_tarihi"      , M_MUH_RV->bitis_tarihi.toString ("yyyy.MM.dd") );

    if ( M_MUH_RV->deneme_baskisi EQ true ) {
        sql_query_1.SET_VALUE ( ":bas_yevmiye_num" , M_MUH_RV->baslangic_yevmiye_no );

        if ( M_MUH_RV->bitis_yevmiye_no > 0 ) {
            sql_query_1.SET_VALUE ( ":bts_yevmiye_num" , M_MUH_RV->bitis_yevmiye_no );
        }
    }

    if ( sql_query_1.SELECT("muh_fisler.yevmiye_numarasi ASC") EQ 0 ) {
        return false;
    }
    M_FISLER = new FIS_STRUCT [ sql_query_1.NUM_OF_ROWS()];

    while ( sql_query_1.NEXT() EQ true ) {
        M_FISLER [ fis_index ].fis_id                = sql_query_1.VALUE(0).toInt();
        M_FISLER [ fis_index ].fis_tarihi            = sql_query_1.VALUE(1).toDate().toString ( "dd MMMM yyyy dddd");
        M_FISLER [ fis_index ].yevmiye_no            = sql_query_1.VALUE(2).toInt();
        M_FISLER [ fis_index ].fis_satirlari_sayisi  = sql_query_1.VALUE(3).toInt();
        M_FISLER [ fis_index ].fis_satirlari         = new FIS_SATIRI_STRUCT[sql_query_1.VALUE(3).toInt() + 1];
        fis_index++;
    }

    sql_query_2.PREPARE_SELECT("muh_fis_satirlari, muh_hesaplar, muh_fisler" , "muh_fis_satirlari.hesap_id, muh_fis_satirlari.aciklama, borc_tutari, alacak_tutari, "
                               "muh_fis_satirlari.fis_id, hesap_ismi, hesap_kodu, parent_id, hesap_seviyesi, tam_hesap_kodu",
                               "muh_hesaplar.hesap_id = muh_fis_satirlari.hesap_id "
                               "AND muh_fisler.fis_id = muh_fis_satirlari.fis_id AND "
                               "muh_fisler.fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi");

    sql_query_2.SET_VALUE      ( ":bas_tarihi"  , M_MUH_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );
    sql_query_2.SET_VALUE      ( ":bts_tarihi"  , M_MUH_RV->bitis_tarihi.toString ( "yyyy.MM.dd" ) );

    if ( sql_query_2.SELECT("muh_fis_satirlari.fis_id ASC ,muh_hesaplar.tam_hesap_kodu ASC") EQ 0 ) {
        return false;
    }

    int son_fis_id    = -1;
    int son_fis_index = -1;

    m_fis_sayisi = fis_index;

    int buffer_index = 0;
    int fis_satir_counter = 0 ;

    M_KOK_HESAP_BUFFER = new KOK_HESAP_STRUCT[10000];

    while ( sql_query_2.NEXT() EQ true ) {

        fis_satir_counter++;

        for ( int i = 0 ; i < fis_index ; i++ ) {
            if ( son_fis_id EQ sql_query_2.VALUE(4).toInt() ) {
                i = son_fis_index;
            }

            if ( M_FISLER[i].fis_id EQ sql_query_2.VALUE(4).toInt() ) {
                if  ( son_fis_id NE M_FISLER[i].fis_id ) {
                    fis_satirlari_index = 0;
                }
                son_fis_index   = i;
                son_fis_id      = M_FISLER[i].fis_id;

                int hesap_id = sql_query_2.VALUE(0).toInt();

                M_FISLER[i].fis_satirlari [ fis_satirlari_index ].hesap_id        = hesap_id;
                M_FISLER[i].fis_satirlari [ fis_satirlari_index ].aciklama        = sql_query_2.VALUE(1).toString();
                M_FISLER[i].fis_satirlari [ fis_satirlari_index ].borc_tutari     = ROUND ( sql_query_2.VALUE(2).toDouble() );
                M_FISLER[i].fis_satirlari [ fis_satirlari_index ].alacak_tutari   = ROUND ( sql_query_2.VALUE(3).toDouble() );
                M_FISLER[i].fis_satirlari [ fis_satirlari_index ].fis_id          = sql_query_2.VALUE(4).toInt();

                M_FISLER[i].fis_satirlari[fis_satirlari_index].hesap_ismi      = sql_query_2.VALUE(5).toString();
                M_FISLER[i].fis_satirlari[fis_satirlari_index].hesap_kodu      = sql_query_2.VALUE(6).toString();
                M_FISLER[i].fis_satirlari[fis_satirlari_index].parent_id       = sql_query_2.VALUE(7).toInt();
                M_FISLER[i].fis_satirlari[fis_satirlari_index].hesap_seviyesi  = sql_query_2.VALUE(8).toInt();
                M_FISLER[i].fis_satirlari[fis_satirlari_index].tam_hesap_kodu  = sql_query_2.VALUE(9).toString();

                bool buffer_has_record = false;

                for ( int j = 0 ; j < buffer_index ; j++ ) {
                    if ( M_KOK_HESAP_BUFFER[j].child_tam_hesap_kodu EQ sql_query_2.VALUE(9).toString() ) {
                         M_FISLER[i].fis_satirlari[fis_satirlari_index].kok_hesap.hesap_id       = M_KOK_HESAP_BUFFER[j].hesap_id;
                         M_FISLER[i].fis_satirlari[fis_satirlari_index].kok_hesap.hesap_ismi     = M_KOK_HESAP_BUFFER[j].hesap_ismi;
                         M_FISLER[i].fis_satirlari[fis_satirlari_index].kok_hesap.hesap_seviyesi = M_KOK_HESAP_BUFFER[j].hesap_seviyesi;
                         M_FISLER[i].fis_satirlari[fis_satirlari_index].kok_hesap.tam_hesap_kodu = M_KOK_HESAP_BUFFER[j].tam_hesap_kodu;
                         buffer_has_record = true;
                        break;
                    }
                }

                if ( buffer_has_record EQ true ) {
                    fis_satirlari_index++;
                    break;
                }

                QStringList kok_hesap = MUH_FIND_AND_GET_HESAP_BILGILERI( hesap_id, M_MUH_HESAPLAR, true ).split ( "\t" );

                M_KOK_HESAP_BUFFER [ buffer_index ].child_tam_hesap_kodu   = sql_query_2.VALUE(9).toString();
                M_KOK_HESAP_BUFFER [ buffer_index ].hesap_id               = QVariant ( kok_hesap.at(0) ).toInt();
                M_KOK_HESAP_BUFFER [ buffer_index ].hesap_ismi             = kok_hesap.at(2);
                M_KOK_HESAP_BUFFER [ buffer_index ].hesap_seviyesi         = QVariant ( kok_hesap.at(4) ).toInt();
                M_KOK_HESAP_BUFFER [ buffer_index ].tam_hesap_kodu         = kok_hesap.at(1);

                M_FISLER[i].fis_satirlari [ fis_satirlari_index ].kok_hesap.hesap_id         = M_KOK_HESAP_BUFFER[buffer_index].hesap_id;
                M_FISLER[i].fis_satirlari [ fis_satirlari_index ].kok_hesap.hesap_ismi       = M_KOK_HESAP_BUFFER[buffer_index].hesap_ismi;
                M_FISLER[i].fis_satirlari [ fis_satirlari_index ].kok_hesap.hesap_seviyesi   = M_KOK_HESAP_BUFFER[buffer_index].hesap_seviyesi;
                M_FISLER[i].fis_satirlari [ fis_satirlari_index ].kok_hesap.tam_hesap_kodu   = M_KOK_HESAP_BUFFER[buffer_index].tam_hesap_kodu;

                buffer_index++;
                fis_satirlari_index++;
                break;
             }
        }
    }
    return true;
}



/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::DEVIR_BAKIYESINI_YAZDIR
***************************************************************************************/

void YEVMIYE_DOKUMU_RAPORU::DEVIR_BAKIYESINI_YAZDIR ()
{
    QStringList STRING_LIST;

    m_gonderilen_satir_sayisi  = 0;

    STRING_LIST.clear();

    if ( M_MUH_RV->sadece_ana_hesaplar ) {
        STRING_LIST<< "" << "<b> ÖNCEKİ SAYFADAN DEVİR </b>" << "";
    }
    else {
        STRING_LIST<< "" << "<b> ÖNCEKİ SAYFADAN DEVİR </b>" << "" << "" << "";
    }

    STRING_LIST << "<b>" + VIRGUL_EKLE ( QVariant( ROUND ( m_footer_toplam_borc   ) ).toString() );
    STRING_LIST<<  "<b>" + VIRGUL_EKLE ( QVariant( ROUND ( m_footer_toplam_alacak ) ).toString() );

    ADD_ROW_TO_TABLE ( STRING_LIST );

    m_gonderilen_satir_sayisi++;
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::YEVMIYE_FISI_BASLIGINI_YAZDIR
***************************************************************************************/

void YEVMIYE_DOKUMU_RAPORU::YEVMIYE_FISI_BASLIGINI_YAZDIR ( int yevmiye_no, QString fis_tarihi )
{
    QStringList STRING_LIST;

    STRING_LIST << "<b>" + QVariant ( yevmiye_no ).toString();

    STRING_LIST << "<b>--------------" + fis_tarihi +"--------------";
    STRING_LIST << "<b>--------------------";
    STRING_LIST << "<b>--------------------" << "<b>--------------------" << "<b>--------------------" << "<b>--------------------";

    if ( M_MUH_RV->sadece_ana_hesaplar EQ true ) {
        STRING_LIST.clear();
        STRING_LIST << "<b>" + QVariant ( yevmiye_no ).toString();
        STRING_LIST << "<b>--------------" + fis_tarihi +"--------------";
        STRING_LIST << "<b>---------------------------------------";
        STRING_LIST << "<b>---------------------------------------";
        STRING_LIST << "<b>---------------------------------------";
    }
    ADD_ROW_TO_TABLE ( STRING_LIST );

    m_gonderilen_satir_sayisi++;
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::YEVMIYE_FISI_MADDE_TOPLAMINI_YAZDIR
***************************************************************************************/

void YEVMIYE_DOKUMU_RAPORU::YEVMIYE_FISI_MADDE_TOPLAMINI_YAZDIR ()
{
    QStringList STRING_LIST;

    STRING_LIST << "" << "" <<"<u>MADDE TOPLAMI" << "" << "" << VIRGUL_EKLE ( QVariant ( ROUND ( m_madde_borc_toplami )).toString())
                << VIRGUL_EKLE ( QVariant ( ROUND ( m_madde_alacak_toplami ) ).toString());

    if ( M_MUH_RV->sadece_ana_hesaplar EQ true ) {

        STRING_LIST.clear();

        STRING_LIST << "" << ""  << "<u>MADDE TOPLAMI" << VIRGUL_EKLE ( QVariant ( ROUND ( m_madde_borc_toplami)).toString())
                    << VIRGUL_EKLE ( QVariant ( ROUND ( m_madde_alacak_toplami) ).toString());
    }

    ADD_ROW_TO_TABLE ( STRING_LIST );

    m_gonderilen_satir_sayisi++;
}

/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::ANA_HESAPLARI_YAZDIR
***************************************************************************************/

void YEVMIYE_DOKUMU_RAPORU::ANA_HESAPLARI_YAZDIR ( int parent_id , int fis_index , bool borc_satiri_mi )
{
    double          ana_hesap_borc;
    double          ana_hesap_alacak;
    QStringList     UST_HESAPLAR;
    QStringList     STRING_LIST;
    QStringList     MUH_HESAP;
    QString         hesap_kodu_adi;

    for ( ; ; ) {

        if ( parent_id EQ 0 ) {
            break;
        }
        QString muh_hesap_str = MUH_FIND_AND_GET_HESAP_BILGILERI( parent_id, M_MUH_HESAPLAR );
        MUH_HESAP =  muh_hesap_str.split ( "\t" );
        UST_HESAPLAR.prepend ( muh_hesap_str );
        parent_id = QVariant ( MUH_HESAP.at(3)).toInt();
    }

    for ( int i = 0 ; i < UST_HESAPLAR.size() ; i++ ) {

        STRING_LIST.clear();
        MUH_HESAP      = UST_HESAPLAR.at(i).split("\t");
        hesap_kodu_adi = MUH_HESAP.at(1);

        if ( MUH_HESAP.at(0).isEmpty() EQ false ) {
            parent_id          = QVariant ( MUH_HESAP.at(3) ).toInt();
            int hesap_seviyesi = QVariant ( MUH_HESAP.at(4) ).toInt();

            if ( parent_id  NE 0 ) {
                if ( hesap_seviyesi > 1 ) {
                    for ( int i = 1 ; i < hesap_seviyesi ; i++ ) {
                        hesap_kodu_adi.prepend( "&nbsp;&nbsp;" );
                        hesap_kodu_adi.prepend( "&nbsp;&nbsp;" );
                    }
                }
            }
        }

        bool ayni_hesap_bulundu = false;
        for ( int j = 0; j < M_BASILMIS_UST_HESAPLAR.size(); j++ ) {
            if ( M_BASILMIS_UST_HESAPLAR.at(j) EQ hesap_kodu_adi ) {
                ayni_hesap_bulundu = true;
                break;
            }
        }
        if ( ayni_hesap_bulundu EQ true ) {
            continue;
        }

        M_BASILMIS_UST_HESAPLAR.append ( hesap_kodu_adi );
        hesap_kodu_adi.append(" ");

        ana_hesap_borc    = 0.00;
        ana_hesap_alacak  = 0.00;

        QString clear_string = hesap_kodu_adi;

        for (int i = 0; i < M_FISLER [ fis_index ].fis_satirlari_sayisi; i++) {
            if ( M_FISLER [ fis_index ].fis_satirlari[i].tam_hesap_kodu.contains ( clear_string.replace ( "&nbsp;&nbsp;","")) EQ true) {
                if ( borc_satiri_mi EQ true ) {
                    ana_hesap_borc    += M_FISLER [ fis_index ].fis_satirlari[i].borc_tutari;
                    ana_hesap_alacak  = 0.00;
                }
                else {
                    ana_hesap_alacak  += M_FISLER [ fis_index ].fis_satirlari[i].alacak_tutari;
                    ana_hesap_borc    = 0.00;
                }
            }
        }

        if ( MUH_HESAP.at(0).isEmpty() EQ false ) {
            hesap_kodu_adi.append ( MUH_HESAP.at(2) );
        }

        STRING_LIST << "" << hesap_kodu_adi << ""<<""<<"";
        STRING_LIST << VIRGUL_EKLE ( QVariant(ana_hesap_borc).toString() );
        STRING_LIST << VIRGUL_EKLE ( QVariant(ana_hesap_alacak).toString() );

        ADD_ROW_TO_TABLE ( STRING_LIST );

        m_gonderilen_satir_sayisi++;
    }

}
/**************************************************************************************
                   YEVMIYE_DOKUMU_RAPORU::ANA_HESAPLARI_YAZDIR_VE_TALI_HESAP_KODUNU_AL
***************************************************************************************/

QString YEVMIYE_DOKUMU_RAPORU::ANA_HESAPLARI_YAZDIR_VE_TALI_HESAP_KODUNU_AL ( int     parent_id,
                                                                              int     hesap_seviyesi,    QString tam_hesap_kodu,
                                                                              QString hesap_ismi,        int     fis_index,bool borc_satirimi )
{
    QString         hesap_kodu_adi;

    if ( parent_id NE 0 ) {

        ANA_HESAPLARI_YAZDIR ( parent_id,fis_index,borc_satirimi );

        hesap_kodu_adi = tam_hesap_kodu;
        hesap_kodu_adi.append ( "   " );
        hesap_kodu_adi.append ( hesap_ismi );

        if ( hesap_seviyesi > 1 ) {
            for ( int i = 1 ; i < hesap_seviyesi ; i++ ) {
                hesap_kodu_adi.prepend ( "&nbsp;&nbsp;" );
                hesap_kodu_adi.prepend ( "&nbsp;&nbsp;" );
            }
        }
    }

    return hesap_kodu_adi;
}
