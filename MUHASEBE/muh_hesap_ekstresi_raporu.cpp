#include "adak_utils.h"
#include "muh_hesap_ekstresi_raporu_class.h"
#include "muh_hesap_ekstresi_raporu_open.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "yonetim.h"

extern ADAK_SQL *          DB;

/**************************************************************************************
                   GET_HESAP_EKSTRESI_RAPORU
***************************************************************************************/

QString GET_HESAP_EKSTRESI_RAPORU (MUH_RAPOR_VARS * rapor_vars)
{
    HESAP_EKSTRESI_RAPORU  *  R = new HESAP_EKSTRESI_RAPORU(rapor_vars);
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   HESAP_EKSTRESI_RAPORU::HESAP_EKSTRESI_RAPORU
***************************************************************************************/

HESAP_EKSTRESI_RAPORU::HESAP_EKSTRESI_RAPORU(MUH_RAPOR_VARS * rapor_vars) : REPORT_KERNEL ("MUH_HESAP_EKSTRESI_RAPORU")
{
    M_MUH_RV = rapor_vars;

    SET_HEIGHTS ( 30,30 );

    m_max_yazdirilabilir_satir_sayisi = GET_MAX_LINE_COUNT (11);

    m_yazdirilan_kayit_sayisi    = 0;
    m_satirlar_basildi           = true;
    m_last_satir_indisi          = 0;
    m_last_hesap_index           = 0;

    if ( M_MUH_RV->bts_hesap_kodu EQ M_MUH_RV->bas_hesap_kodu ) {
        m_rapor_kriterleri_1.append ( QString ( "Tek Hesap: %1" ).arg ( M_MUH_RV->bas_hesap_kodu ) );
    }
    else {
        m_rapor_kriterleri_1.append ( M_MUH_RV->bas_hesap_kodu );
        m_rapor_kriterleri_1.append ( "-" + M_MUH_RV->bts_hesap_kodu + " Arası Hesaplar" );
    }
    if ( M_MUH_RV->bakiyesiz_hesaplar EQ 0) {
         m_rapor_kriterleri_1.append ( "- Bakiyeli Hesaplar" );
    }
    else if ( M_MUH_RV->bakiyesiz_hesaplar EQ 1) {
        m_rapor_kriterleri_1.append ( "- Bakiyesiz Hesaplar" );
    }

    if ( M_MUH_RV->calismamis_hesaplar EQ 0) {
         m_rapor_kriterleri_2.append ( "- Çalışmış Hesaplar" );
    }
    else if  ( M_MUH_RV->calismamis_hesaplar EQ 1 OR M_MUH_RV->calismamis_hesaplar EQ 2) {
         m_rapor_kriterleri_2.append ( "- Çalışmamış Hesaplar" );
         FILL_TUM_HESAPLAR_LIST();
    }

    if ( M_MUH_RV->nakli_yekun EQ 1 ) {
        m_rapor_kriterleri_2.append ( "- Nakli Yekün" );
    }

    if ( M_MUH_RV->sadece_ana_hesaplar EQ true) {
        m_rapor_kriterleri_2.append ( "- Sadece Ana Hesaplar" );
    }
    m_raporlanacak_kayit_var_mi  = FILL_STRUCT();
}

/**************************************************************************************
                   HESAP_EKSTRESI_RAPORU::~HESAP_EKSTRESI_RAPORU
***************************************************************************************/

HESAP_EKSTRESI_RAPORU::~HESAP_EKSTRESI_RAPORU()
{

}

/**************************************************************************************
                   HESAP_EKSTRESI_RAPORU::GET_HEADER
***************************************************************************************/

QString HESAP_EKSTRESI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING  ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER=0 >" );

    SET_TD_TAG_STRING     ( QStringList()<<"style=\"width:30%; font-size:100%;\""<<"style=\"width:40%; font-size:100%;\" ALIGN=CENTER"<<"style=\"width:30%; font-size:100%;\" ALIGN=RIGHT");

    CREATE_TABLE          ( QStringList() << "" << "" << "" , 0 , 0 );

    ADD_ROW_TO_TABLE      ( QStringList() << E9_GET_FIRMA_ISMI() << "<b>HESAP EKSTRESİ</b>" << GET_REPORT_TOP_RIGHT_HEADER() );


    ADD_ROW_TO_TABLE      ( QStringList() << "" << M_MUH_RV->baslangic_tarihi.toString ( "dd MMMM yyyy dddd" ) + " - "+
                      M_MUH_RV->bitis_tarihi.toString ( "dd MMMM yyyy dddd" ) << "" );

    ADD_ROW_TO_TABLE      ( QStringList() << "" << m_rapor_kriterleri_1 + " " + m_rapor_kriterleri_2 << "" );

    return GET_TABLE_HTML_STRING();
}

/**********************************************************************************
                   HESAP_EKSTRESI_RAPORU::GET_BODY
***************************************************************************************/

QString HESAP_EKSTRESI_RAPORU::GET_BODY()
{
    double          borc_bakiyesi [ E9_ARRAY_SIZE ];
    double          alacak_bakiyesi [ E9_ARRAY_SIZE ];

    int             bas_gun_no = M_MUH_RV->bas_gun_no;
    int             bts_gun_no = M_MUH_RV->bts_gun_no;
    QString         hesap_kodu_adi;

    if ( m_raporlanacak_kayit_var_mi EQ false ) {

        if ( M_MUH_RV->nakli_yekun ) {

            SQL_QUERY sql_query ( DB );

            sql_query.PREPARE_SELECT("muh_hesaplar" ,
                                     "toplam_borc_array,toplam_alacak_array,"
                                     "tam_hesap_kodu,hesap_ismi" ,
                                     "tam_hesap_kodu = :tam_hesap_kodu" );

            sql_query.SET_VALUE      ( ":tam_hesap_kodu" , M_MUH_RV->bas_hesap_kodu );

            if ( sql_query.SELECT() > 0) {
                sql_query.NEXT();

                UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(0).toString() , borc_bakiyesi   , E9_ARRAY_SIZE );
                UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(1).toString() , alacak_bakiyesi , E9_ARRAY_SIZE );

                hesap_kodu_adi = "<p align=center style=\"font-size:85%; font-weight:bold \">" +
                                 sql_query.VALUE(2).toString() + " " +
                                 sql_query.VALUE(3).toString() + "</p>";
            }
            SET_TABLE_TAG_STRING  ( "<TABLE style=\"width:100%; font-size:75%; \" BORDER = 1>" );
            SET_TD_TAG_STRING ( QStringList() << "style=\"width:18%;\""<< "style=\"width:7%;\""<< "style=\"width:35%;\""
                                              << "style=\"width:9%;\"" << "style=\"width:9%;\"" << "style=\"width:11%;\""
                                              << "style=\"width:11%;\"" );

            CREATE_TABLE ( QStringList()<< "Tarih" << "Fiş No" << "Açıklama"<< "Borç"  << "Alacak" << "Borç Bakiye"
                                        << "Alacak Bakiye",1,14 );

            PRINT_NAKLI_YEKUN ( borc_bakiyesi [ bas_gun_no-1 ] , alacak_bakiyesi [ bas_gun_no-1 ] );

            m_footer_borc   = borc_bakiyesi   [ bas_gun_no-1 ];
            m_footer_alacak = alacak_bakiyesi [ bas_gun_no-1 ];

            M_MUH_RV->nakli_yekun = 0;

            if ( M_MUH_RV->bas_hesap_kodu EQ M_MUH_RV->bts_hesap_kodu ) {

                return hesap_kodu_adi.append(GET_TABLE_HTML_STRING());
            }
            return GET_TABLE_HTML_STRING();
        }
        return NULL;
    }

    double          toplam_borc   = 0.00;
    double          toplam_alacak = 0.00;

    QStringList     string_list;

    int array_size = 0;

    if ( M_MUH_RV->calismamis_hesaplar EQ 2 OR M_MUH_RV->calismamis_hesaplar EQ 1) {
        array_size = M_TUM_HESAPLAR_LIST.size();
    }
    else {
        array_size = m_num_of_muh_hesaplar;
    }

    for ( ; ; ) {

        if ( m_last_hesap_index >= array_size ) {
            for ( int i = 0; i < array_size ; i++ ) {
                delete [] M_MUH_HESAPLAR[i].fis_satirlari;
            }
            delete [] M_MUH_HESAPLAR;

            return NULL;
        }

        if ( m_satirlar_basildi EQ true ) {
             m_yazdirilan_kayit_sayisi = 0;
             m_footer_borc             = 0.00;
             m_footer_alacak           = 0.00;
             m_footer_borc_bakiye      = 0.00;
             m_footer_alacak_bakiye    = 0.00;
             m_last_satir_indisi       = 0;
             m_satirlar_basildi        = false;
        }

        SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%; \" BORDER = 1>" );

        SET_TD_TAG_STRING ( QStringList() << "style=\"width:18%;\""<< "style=\"width:7%;\""<< "style=\"width:35%;\""
                                          << "style=\"width:9%;\"" << "style=\"width:9%;\"" << "style=\"width:11%;\""
                                          << "style=\"width:11%;\"" );

        int hesap_id                = M_MUH_HESAPLAR [ m_last_hesap_index ].hesap_id;
        int parent_id               = M_MUH_HESAPLAR [ m_last_hesap_index ].parent_id;
        int tali_hesap_mi           = M_MUH_HESAPLAR [ m_last_hesap_index ].tali_hesap;

        if ( M_MUH_RV->sadece_ana_hesaplar EQ true ) {
            if ( parent_id NE 0 ) {
                m_last_hesap_index++;
                continue;
            }
        }
        else {
            if(tali_hesap_mi EQ false) {
               m_last_hesap_index++;
               continue;
            }
        }

        hesap_kodu_adi = "<p align=center style=\"font-size:85%; font-weight:bold \">" +
                         M_MUH_HESAPLAR [ m_last_hesap_index ].tam_hesap_kodu + " " +
                         M_MUH_HESAPLAR [ m_last_hesap_index ].hesap_ismi + "</p>";

        CREATE_TABLE ( QStringList()<< "Tarih" << "Fiş No" << "Açıklama"<< "Borç"  << "Alacak" << "Borç Bakiye"
                                    << "Alacak Bakiye",1,14 );

        SET_TD_TAG_STRING ( QStringList()<< "" << "" << "" << "align = RIGHT "
                                         <<"align = RIGHT "<<"align = RIGHT "<<"align = RIGHT ");
        UNPACK_DOUBLE_ARRAY ( M_MUH_HESAPLAR [ m_last_hesap_index ].borc_bakiyesi  ,borc_bakiyesi  , E9_ARRAY_SIZE );
        UNPACK_DOUBLE_ARRAY ( M_MUH_HESAPLAR [ m_last_hesap_index ].alacak_bakiyesi,alacak_bakiyesi, E9_ARRAY_SIZE );

        toplam_borc   = 0.00;
        toplam_alacak = 0.00;

        if ( M_MUH_RV->nakli_yekun EQ true ) {
            toplam_borc   = borc_bakiyesi [ bts_gun_no ];
            toplam_alacak = alacak_bakiyesi [ bts_gun_no ];

        }
        else {
            toplam_borc   = borc_bakiyesi [ bts_gun_no ] - borc_bakiyesi [ bas_gun_no-1 ];
            toplam_alacak = alacak_bakiyesi [ bts_gun_no ] - alacak_bakiyesi [ bas_gun_no-1 ];
        }

        if ( M_MUH_RV->bakiyesiz_hesaplar EQ 0 ) {
            if ( toplam_borc EQ toplam_alacak ) {
                if ( toplam_borc NE 0 AND toplam_alacak NE 0 ) {
                    m_last_hesap_index++;
                    continue;
                }
            }
        }
        else if ( M_MUH_RV->bakiyesiz_hesaplar EQ 1 ) {
            if ( toplam_borc NE toplam_alacak ) {
                    m_last_hesap_index++;
                    continue;
            }
        }

        if ( M_MUH_RV->calismamis_hesaplar EQ 0 ) {
            if ( alacak_bakiyesi  [ bas_gun_no-1 ]   EQ alacak_bakiyesi [ bts_gun_no ]
                AND borc_bakiyesi [ bas_gun_no-1 ] EQ borc_bakiyesi [ bts_gun_no ] ) {

                if ( ( alacak_bakiyesi [ bts_gun_no ] EQ 0 ) AND ( borc_bakiyesi [ bts_gun_no ] EQ 0 ) ) {
                   m_last_hesap_index++;
                   continue;
                }
            }
        }
        else if ( M_MUH_RV->calismamis_hesaplar EQ 1 ) {
            if ( alacak_bakiyesi  [ bas_gun_no-1 ] NE alacak_bakiyesi [ bts_gun_no ]
                AND borc_bakiyesi [ bas_gun_no-1 ] NE borc_bakiyesi [ bts_gun_no ] ) {
                   m_last_hesap_index++;
                   continue;
            }
        }

        if  ( M_MUH_RV->nakli_yekun ) {
            if  ( m_yazdirilan_kayit_sayisi EQ 0 ) {
                  PRINT_NAKLI_YEKUN ( borc_bakiyesi [ bas_gun_no-1 ],alacak_bakiyesi [ bas_gun_no-1 ] );

                m_footer_borc   = borc_bakiyesi   [ bas_gun_no-1 ];
                m_footer_alacak = alacak_bakiyesi [ bas_gun_no-1 ];

            }
        }
        m_yazdirilan_kayit_sayisi = 0;

        toplam_borc   = 0.00;
        toplam_alacak = 0.00;

        for ( int i = m_last_satir_indisi ; i < M_MUH_HESAPLAR [ m_last_hesap_index ].fis_satirlari_sayisi ; i++ ) {

            double          satir_borc_bakiye = 0.00;
            double          satir_alacak_bakiye = 0.00;

            toplam_borc   = m_footer_borc;
            toplam_alacak = m_footer_alacak;

            double satir_borc_tutari   = 0.00;
            double satir_alacak_tutari = 0.00;

            if ( tali_hesap_mi EQ false ) {
                int tali_hesap_id  = M_MUH_HESAPLAR [ m_last_hesap_index ].hesap_id;
                int parent_hesap_id = FIND_PARENT_HESAP ( tali_hesap_id );

                if ( parent_hesap_id EQ hesap_id ) {
                     satir_borc_tutari   = M_MUH_HESAPLAR [ m_last_hesap_index ].fis_satirlari[i].borc_tutari;
                     satir_alacak_tutari = M_MUH_HESAPLAR [ m_last_hesap_index ].fis_satirlari [i].alacak_tutari;
                }
            }
            else {
                satir_borc_tutari   = M_MUH_HESAPLAR [ m_last_hesap_index ].fis_satirlari [i].borc_tutari;
                satir_alacak_tutari = M_MUH_HESAPLAR [ m_last_hesap_index ].fis_satirlari [i].alacak_tutari;
            }

            if ( satir_borc_tutari EQ 0.00 AND satir_alacak_tutari EQ 0.00 ) {
                continue;
            }
            toplam_borc    += satir_borc_tutari;
            toplam_alacak  += satir_alacak_tutari;

            if (toplam_borc > toplam_alacak ) {

                satir_borc_bakiye      = toplam_borc - toplam_alacak;
                satir_alacak_bakiye    = 0.00;
            }
            else {
                satir_alacak_bakiye = toplam_alacak - toplam_borc;
                satir_borc_bakiye = 0.00;
            }

            string_list.clear();
            string_list << QDate::fromString ( M_MUH_HESAPLAR [ m_last_hesap_index ].fis_satirlari [i].fis_tarihi,"yyyy.MM.dd").toString("dd MMMM yyyy dddd")
                        << M_MUH_HESAPLAR [ m_last_hesap_index ].fis_satirlari [i].fis_no
                        << M_MUH_HESAPLAR [ m_last_hesap_index ].fis_satirlari [i].aciklama
                        << VIRGUL_EKLE ( QVariant ( ROUND ( satir_borc_tutari    ) ).toString() )
                        << VIRGUL_EKLE ( QVariant ( ROUND ( satir_alacak_tutari  ) ).toString() )
                        << VIRGUL_EKLE ( QVariant ( ROUND ( satir_borc_bakiye    ) ).toString() )
                        << VIRGUL_EKLE ( QVariant ( ROUND ( satir_alacak_bakiye  ) ).toString() );

            m_footer_borc   += ROUND ( satir_borc_tutari );
            m_footer_alacak += ROUND ( satir_alacak_tutari);

            ADD_ROW_TO_TABLE ( string_list );

            m_yazdirilan_kayit_sayisi++;
            m_last_satir_indisi++;

            if ( m_yazdirilan_kayit_sayisi >= m_max_yazdirilabilir_satir_sayisi ){
                m_satirlar_basildi = false;
                return hesap_kodu_adi.append ( GET_TABLE_HTML_STRING() );
            }
        }

        m_satirlar_basildi  = true;
        m_last_satir_indisi = 0;
        m_last_hesap_index++;
        break;

    }

    return hesap_kodu_adi.append ( GET_TABLE_HTML_STRING() );
}

/**************************************************************************************
                   HESAP_EKSTRESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString HESAP_EKSTRESI_RAPORU::GET_FOOTER()
{

    QStringList string_list;

    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER=0>" );
    SET_TD_TAG_STRING ( QStringList() << "style=\"width:60%;\""            << "style=\"width:9%;\" ALIGN=RIGHT"
                                    << "style=\"width:9%;\" ALIGN=RIGHT" << "style=\"width:11%;\" ALIGN=RIGHT"
                                    << "style=\"width:11%;\" ALIGN=RIGHT" );

    CREATE_TABLE ( QStringList()<< "" << "" << "" << "" << "",0,0 );


    if ( m_footer_borc > m_footer_alacak ) {
         m_footer_borc_bakiye = m_footer_borc - m_footer_alacak;
         m_footer_alacak_bakiye = 0.0;

    }
    else {
        m_footer_alacak_bakiye = m_footer_alacak - m_footer_borc;
        m_footer_borc_bakiye   = 0.0;
    }

    string_list.clear();
    string_list << "<b>Toplam";
    string_list << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_borc          ) ).toString() );
    string_list << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_alacak        ) ).toString() );
    string_list << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_borc_bakiye   ) ).toString() );
    string_list << VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_alacak_bakiye ) ).toString() );

    ADD_ROW_TO_TABLE ( string_list );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   HESAP_EKSTRESI_RAPORU::FIND_PARENT_HESAP
***************************************************************************************/

int HESAP_EKSTRESI_RAPORU::FIND_PARENT_HESAP ( int hesap_id )
{
    int         parent_id;
    int         hesap_index;

    for ( ; ; ) {
        hesap_index = -1;

        for ( int i = 0 ; i < m_num_of_muh_hesaplar ; i++ ) {
            if ( M_MUH_HESAPLAR [i].hesap_id EQ hesap_id ) {
                hesap_index = i;
                break;
            }
        }

        if ( hesap_index EQ -1 ) {
            return 0;
        }

        parent_id          = M_MUH_HESAPLAR [ hesap_index ].parent_id;
        int hesap_seviyesi = M_MUH_HESAPLAR [ hesap_index ].hesap_seviyesi;

        if (hesap_seviyesi EQ 1 ) {
            break;
        }
        hesap_id = parent_id;

    }
    return parent_id;
}

/**************************************************************************************
            HESAP_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN
***************************************************************************************/

void HESAP_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN ( double p_toplam_borc,double p_toplam_alacak )
{
    QStringList     string_list;
    double          borc_bakiye;
    double          alacak_bakiye;

    QString aciklama     = "Nakli Yekun";

    if ( p_toplam_borc NE 0 OR p_toplam_alacak NE 0 ) {
        if ( MALI_YIL_ARRAY_INDIS ( M_MUH_RV->baslangic_tarihi ) EQ 1 ) {
            int muhasebe_yili = MALI_YIL_FIRST_DATE().year();
            if ( muhasebe_yili EQ M_MUH_RV->baslangic_tarihi.year() ) {
                aciklama = QObject::tr ( "Nakli Yekun" );
            }
        }
    }

    if  ( p_toplam_borc > p_toplam_alacak ) {
        borc_bakiye = p_toplam_borc - p_toplam_alacak;
        alacak_bakiye = 0.0;
    }
    else {
        alacak_bakiye = p_toplam_alacak - p_toplam_borc;
        borc_bakiye   = 0.0;
    }

    SET_TD_TAG_STRING ( QStringList() << "" <<  "" << "" << "ALIGN = RIGHT"
                                    << "ALIGN = RIGHT" << "ALIGN=RIGHT" << "ALIGN = RIGHT" );
    string_list.clear();

    string_list << "" << "" << aciklama << VIRGUL_EKLE ( QVariant(ROUND ( p_toplam_borc    ) ).toString() )
                                        << VIRGUL_EKLE ( QVariant(ROUND ( p_toplam_alacak  ) ).toString() )
                                        << VIRGUL_EKLE ( QVariant(ROUND ( borc_bakiye      ) ).toString() )
                                        << VIRGUL_EKLE ( QVariant(ROUND ( alacak_bakiye    ) ).toString() );

    m_yazdirilan_kayit_sayisi++;

    ADD_ROW_TO_TABLE(string_list);

}


/**************************************************************************************
                   HESAP_EKSTRESI_RAPORU::FILL_STRUCT
***************************************************************************************/

bool HESAP_EKSTRESI_RAPORU::FILL_STRUCT()
{
    SQL_QUERY       sql_query ( DB );

    QString fields1 = "muh_hesaplar.hesap_id, tam_hesap_kodu, hesap_ismi, parent_id, "
                          "hesap_seviyesi,tali_hesap, ";

    QString fields2 = " toplam_borc_array, toplam_alacak_array";

    QString column_names = fields1 + "count(muh_fis_satirlari.hesap_id) ," + fields2;

    QString where_str = "muh_fis_satirlari.fis_id   = muh_fisler.fis_id "
                        "AND muh_fis_satirlari.hesap_id = muh_hesaplar.hesap_id "
                        "AND muh_fisler.fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi ";

    sql_query.PREPARE_SELECT ("muh_fisler ,muh_fis_satirlari,muh_hesaplar" ,
                              column_names ,
                              where_str ,fields1 + fields2);


    if ( M_MUH_RV->bas_hesap_kodu NE M_MUH_RV->bts_hesap_kodu ) {
        sql_query.AND_EKLE  ( "tam_hesap_kodu BETWEEN :bas_hesap_kodu AND :bts_hesap_kodu " );
        sql_query.SET_VALUE ( ":bas_hesap_kodu", M_MUH_RV->bas_hesap_kodu );
        sql_query.SET_VALUE ( ":bts_hesap_kodu", M_MUH_RV->bts_hesap_kodu );
    }
    else {
        sql_query.AND_EKLE("tam_hesap_kodu LIKE :tam_hesap_kodu");
        sql_query.SET_LIKE(":tam_hesap_kodu" , M_MUH_RV->bas_hesap_kodu);
    }

    sql_query.SET_VALUE ( ":bas_tarihi"    , M_MUH_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );
    sql_query.SET_VALUE ( ":bts_tarihi"    , M_MUH_RV->bitis_tarihi.toString ( "yyyy.MM.dd" ) );

    sql_query.AND_EKLE ( "fis_turu != :fis_turu ");
    sql_query.SET_VALUE ( ":fis_turu" , ENUM_ACILIS_FISI );


    if ( sql_query.SELECT("tam_hesap_kodu ASC") EQ 0 ) {

        if ( M_MUH_RV->calismamis_hesaplar EQ 2 OR M_MUH_RV->calismamis_hesaplar EQ 1) {

            if (M_TUM_HESAPLAR_LIST.isEmpty() EQ true ) {
                return false;
            }


            M_MUH_HESAPLAR = new HESAPLAR_STRUCT[M_TUM_HESAPLAR_LIST.size()];

            QStringList MUHASEBE_HESABI;

            for ( int i = 0 ; i < M_TUM_HESAPLAR_LIST.size() ; i++ ) {

                MUHASEBE_HESABI = M_TUM_HESAPLAR_LIST.at(i).split("\t");

                M_MUH_HESAPLAR [ i ].hesap_id                = QVariant(MUHASEBE_HESABI.at(0)).toInt();
                M_MUH_HESAPLAR [ i ].tam_hesap_kodu          = MUHASEBE_HESABI.at(1);
                M_MUH_HESAPLAR [ i ].hesap_ismi              = MUHASEBE_HESABI.at(2);
                M_MUH_HESAPLAR [ i ].parent_id               = QVariant(MUHASEBE_HESABI.at(3)).toInt();
                M_MUH_HESAPLAR [ i ].hesap_seviyesi          = QVariant(MUHASEBE_HESABI.at(4)).toInt();
                M_MUH_HESAPLAR [ i ].tali_hesap              = QVariant(MUHASEBE_HESABI.at(5)).toInt();
                M_MUH_HESAPLAR [ i ].fis_satirlari_sayisi    = 0;
                M_MUH_HESAPLAR [ i ].fis_satirlari           = 0;
                M_MUH_HESAPLAR [ i ].borc_bakiyesi           = MUHASEBE_HESABI.at(6);
                M_MUH_HESAPLAR [ i ].alacak_bakiyesi         = MUHASEBE_HESABI.at(7);

            }
            return true;
        }
        else {
           return false;
        }
    }

    m_num_of_muh_hesaplar = sql_query.NUM_OF_ROWS();

    int list_size         = M_TUM_HESAPLAR_LIST.size();

    if ( M_MUH_RV->calismamis_hesaplar EQ 2 OR M_MUH_RV->calismamis_hesaplar EQ 1) {
        M_MUH_HESAPLAR         = new HESAPLAR_STRUCT [ list_size ];
    }
    else {
        M_MUH_HESAPLAR         = new HESAPLAR_STRUCT [ m_num_of_muh_hesaplar ];
    }

    int muh_hesap_index = 0;

    if ( M_MUH_RV->calismamis_hesaplar EQ 2 OR  M_MUH_RV->calismamis_hesaplar EQ 1) {

        QStringList MUHASEBE_HESABI;

        int         liste_indis_no = 0;

        while ( sql_query.NEXT() ) {
            //Sorgudan islem goren hesaplar,tam hesap koduna gore siralanmis halde gelirler.

            QString sorgu_hesap_kodu = sql_query.VALUE(1).toString();

            for ( int i = liste_indis_no ; i < M_TUM_HESAPLAR_LIST.size() ; i++ ) {

                MUHASEBE_HESABI = M_TUM_HESAPLAR_LIST.at(i).split("\t");

                QString liste_hesap_kodu = MUHASEBE_HESABI.at(1);

                if ( sorgu_hesap_kodu EQ liste_hesap_kodu ) {

                    QString borc_bakiyesi;
                    QString alacak_bakiyesi;

                    borc_bakiyesi   = sql_query.VALUE(7).toString();
                    alacak_bakiyesi = sql_query.VALUE(8).toString();

                    //Hesap islem gormustur.O yuzden sorgudaki degerler alinir.
                    M_MUH_HESAPLAR [ muh_hesap_index ].hesap_id                = sql_query.VALUE(0).toInt();
                    M_MUH_HESAPLAR [ muh_hesap_index ].tam_hesap_kodu          = sql_query.VALUE(1).toString();
                    M_MUH_HESAPLAR [ muh_hesap_index ].hesap_ismi              = sql_query.VALUE(2).toString();
                    M_MUH_HESAPLAR [ muh_hesap_index ].parent_id               = sql_query.VALUE(3).toInt();
                    M_MUH_HESAPLAR [ muh_hesap_index ].hesap_seviyesi          = sql_query.VALUE(4).toInt();
                    M_MUH_HESAPLAR [ muh_hesap_index ].tali_hesap              = sql_query.VALUE(5).toInt();
                    M_MUH_HESAPLAR [ muh_hesap_index ].fis_satirlari_sayisi    = 0;
                    M_MUH_HESAPLAR [ muh_hesap_index ].fis_satirlari           = new HESAP_FIS_SATIRI_STRUCT [ sql_query.VALUE(6).toInt() + 1];
                    M_MUH_HESAPLAR [ muh_hesap_index ].borc_bakiyesi           = borc_bakiyesi;
                    M_MUH_HESAPLAR [ muh_hesap_index ].alacak_bakiyesi         = alacak_bakiyesi;

                    muh_hesap_index++;
                    liste_indis_no++;
                    break;
                }
                else {
                    //Hesap calismamistir.
                    M_MUH_HESAPLAR [ muh_hesap_index ].hesap_id                = QVariant(MUHASEBE_HESABI.at(0)).toInt();
                    M_MUH_HESAPLAR [ muh_hesap_index ].tam_hesap_kodu          = MUHASEBE_HESABI.at(1);
                    M_MUH_HESAPLAR [ muh_hesap_index ].hesap_ismi              = MUHASEBE_HESABI.at(2);
                    M_MUH_HESAPLAR [ muh_hesap_index ].parent_id               = QVariant(MUHASEBE_HESABI.at(3)).toInt();
                    M_MUH_HESAPLAR [ muh_hesap_index ].hesap_seviyesi          = QVariant(MUHASEBE_HESABI.at(4)).toInt();
                    M_MUH_HESAPLAR [ muh_hesap_index ].tali_hesap              = QVariant(MUHASEBE_HESABI.at(5)).toInt();
                    M_MUH_HESAPLAR [ muh_hesap_index ].fis_satirlari_sayisi    = 0;
                    M_MUH_HESAPLAR [ muh_hesap_index ].fis_satirlari           = 0;
                    M_MUH_HESAPLAR [ muh_hesap_index ].borc_bakiyesi           = MUHASEBE_HESABI.at(6);
                    M_MUH_HESAPLAR [ muh_hesap_index ].alacak_bakiyesi         = MUHASEBE_HESABI.at(7);

                    muh_hesap_index++;
                    liste_indis_no++;
                    continue;

                }
            }
        }
        if ( list_size NE liste_indis_no) {
            //LIstedeki tum degerler okunmamis ise

            for ( int i = liste_indis_no ; i < list_size ; i++ ) {
                MUHASEBE_HESABI = M_TUM_HESAPLAR_LIST.at(i).split("\t");

                M_MUH_HESAPLAR [ muh_hesap_index ].hesap_id                = QVariant(MUHASEBE_HESABI.at(0)).toInt();
                M_MUH_HESAPLAR [ muh_hesap_index ].tam_hesap_kodu          = MUHASEBE_HESABI.at(1);
                M_MUH_HESAPLAR [ muh_hesap_index ].hesap_ismi              = MUHASEBE_HESABI.at(2);
                M_MUH_HESAPLAR [ muh_hesap_index ].parent_id               = QVariant(MUHASEBE_HESABI.at(3)).toInt();
                M_MUH_HESAPLAR [ muh_hesap_index ].hesap_seviyesi          = QVariant(MUHASEBE_HESABI.at(4)).toInt();
                M_MUH_HESAPLAR [ muh_hesap_index ].tali_hesap              = QVariant(MUHASEBE_HESABI.at(5)).toInt();
                M_MUH_HESAPLAR [ muh_hesap_index ].fis_satirlari_sayisi    = 0;
                M_MUH_HESAPLAR [ muh_hesap_index ].fis_satirlari           = 0;
                M_MUH_HESAPLAR [ muh_hesap_index ].borc_bakiyesi           = MUHASEBE_HESABI.at(6);
                M_MUH_HESAPLAR [ muh_hesap_index ].alacak_bakiyesi         = MUHASEBE_HESABI.at(7);

                muh_hesap_index++;

            }
        }
    }
    else {
        while ( sql_query.NEXT() EQ true ) {

            QString borc_bakiyesi;
            QString alacak_bakiyesi;

            borc_bakiyesi   = sql_query.VALUE(7).toString();
            alacak_bakiyesi = sql_query.VALUE(8).toString();

            M_MUH_HESAPLAR [ muh_hesap_index ].hesap_id                = sql_query.VALUE(0).toInt();
            M_MUH_HESAPLAR [ muh_hesap_index ].tam_hesap_kodu          = sql_query.VALUE(1).toString();
            M_MUH_HESAPLAR [ muh_hesap_index ].hesap_ismi              = sql_query.VALUE(2).toString();
            M_MUH_HESAPLAR [ muh_hesap_index ].parent_id               = sql_query.VALUE(3).toInt();
            M_MUH_HESAPLAR [ muh_hesap_index ].hesap_seviyesi          = sql_query.VALUE(4).toInt();
            M_MUH_HESAPLAR [ muh_hesap_index ].tali_hesap              = sql_query.VALUE(5).toInt();
            M_MUH_HESAPLAR [ muh_hesap_index ].fis_satirlari_sayisi    = 0;
            M_MUH_HESAPLAR [ muh_hesap_index ].fis_satirlari           = new HESAP_FIS_SATIRI_STRUCT [ sql_query.VALUE(6).toInt() + 1];
            M_MUH_HESAPLAR [ muh_hesap_index ].borc_bakiyesi           = borc_bakiyesi;
            M_MUH_HESAPLAR [ muh_hesap_index ].alacak_bakiyesi         = alacak_bakiyesi;

            muh_hesap_index++;
        }
    }

    where_str = "muh_fisler.fis_id = muh_fis_satirlari.fis_id "
                "AND muh_hesaplar.hesap_id = muh_fis_satirlari.hesap_id";


    sql_query.PREPARE_SELECT ("muh_fis_satirlari,muh_fisler,muh_hesaplar",
                              "muh_fis_satirlari.hesap_id, borc_tutari, alacak_tutari,"
                              "muh_fis_satirlari.aciklama, muh_fis_satirlari.fis_id, "
                              "fis_tarihi,fis_no " ,where_str );


    if ( M_MUH_RV->bas_hesap_kodu NE M_MUH_RV->bts_hesap_kodu ) {
        sql_query.AND_EKLE( "tam_hesap_kodu BETWEEN :bas_hesap_kodu AND :bts_hesap_kodu " );

        sql_query.SET_VALUE ( ":bas_hesap_kodu", M_MUH_RV->bas_hesap_kodu );
        sql_query.SET_VALUE ( ":bts_hesap_kodu", M_MUH_RV->bts_hesap_kodu );

    }
    else {
        sql_query.AND_EKLE( "tam_hesap_kodu LIKE :tam_hesap_kodu  " );

        sql_query.SET_LIKE(":tam_hesap_kodu" , M_MUH_RV->bas_hesap_kodu);
    }

    sql_query.AND_EKLE( "muh_fisler.fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi "
                        "AND fis_turu != :fis_turu " );


    sql_query.SET_VALUE ( ":bas_tarihi"   , M_MUH_RV->baslangic_tarihi.toString ( "yyyy.MM.dd" ) );
    sql_query.SET_VALUE ( ":bts_tarihi"   , M_MUH_RV->bitis_tarihi.toString ( "yyyy.MM.dd" ) );
    sql_query.SET_VALUE ( ":fis_turu"     , ENUM_ACILIS_FISI );

    if ( sql_query.SELECT("muh_fis_satirlari.hesap_id ASC, muh_fisler.fis_tarihi ASC, muh_fisler.fis_no ASC, order_number ASC") EQ 0 ) {
        return false;
    }

    int son_hesap_id    = -1;
    int son_hesap_index = -1;

    int fis_satir_counter = 0 ;

    int array_size = 0;

    if ( M_MUH_RV->calismamis_hesaplar EQ 2 OR  M_MUH_RV->calismamis_hesaplar EQ 1) {
        array_size = M_TUM_HESAPLAR_LIST.size();
    }
    else {
        array_size = m_num_of_muh_hesaplar;
    }

    while ( sql_query.NEXT() EQ true ) {

        fis_satir_counter++;
        for ( int i = 0 ; i < array_size ; i++ ) {
            if ( son_hesap_id EQ sql_query.VALUE(0).toInt() ) {
                i = son_hesap_index;
            }

            if ( M_MUH_HESAPLAR[i].hesap_id EQ sql_query.VALUE(0).toInt() ) {
                son_hesap_index   = i;
                son_hesap_id      = sql_query.VALUE(0).toInt();

                M_MUH_HESAPLAR [ i ].fis_satirlari [ M_MUH_HESAPLAR[i].fis_satirlari_sayisi ].borc_tutari     = ROUND ( sql_query.VALUE(1).toDouble() );
                M_MUH_HESAPLAR [ i ].fis_satirlari [ M_MUH_HESAPLAR[i].fis_satirlari_sayisi ].alacak_tutari   = ROUND ( sql_query.VALUE(2).toDouble() );
                M_MUH_HESAPLAR [ i ].fis_satirlari [ M_MUH_HESAPLAR[i].fis_satirlari_sayisi ].aciklama        = sql_query.VALUE(3).toString();
                M_MUH_HESAPLAR [ i ].fis_satirlari [ M_MUH_HESAPLAR[i].fis_satirlari_sayisi ].fis_id          = sql_query.VALUE(4).toInt();
                M_MUH_HESAPLAR [ i ].fis_satirlari [ M_MUH_HESAPLAR[i].fis_satirlari_sayisi ].fis_tarihi      = sql_query.VALUE(5).toString();
                M_MUH_HESAPLAR [ i ].fis_satirlari [ M_MUH_HESAPLAR[i].fis_satirlari_sayisi ].fis_no          = sql_query.VALUE(6).toString();
                M_MUH_HESAPLAR[i].fis_satirlari_sayisi ++;
                break;
             }
        }
    }

    return true;
}


/**************************************************************************************
                   HESAP_EKSTRESI_RAPORU::FILL_TUM_HESAPLAR_LIST();
***************************************************************************************/

void HESAP_EKSTRESI_RAPORU::FILL_TUM_HESAPLAR_LIST()
{
    QString where_str;


    if ( M_MUH_RV->bas_hesap_kodu NE M_MUH_RV->bts_hesap_kodu ) {
        where_str += QString (" tam_hesap_kodu BETWEEN :bas_hesap_kodu AND :bts_hesap_kodu ");
    }
    else {
        where_str += QString ( "tam_hesap_kodu LIKE :tam_hesap_kodu ");
    }


    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_hesaplar" , "hesap_id,tam_hesap_kodu,hesap_ismi,parent_id,hesap_seviyesi,tali_hesap, "
                         "toplam_borc_array , toplam_alacak_array " , where_str);

    if ( M_MUH_RV->bas_hesap_kodu NE M_MUH_RV->bts_hesap_kodu ) {
        query.SET_VALUE(":bas_hesap_kodu" , M_MUH_RV->bas_hesap_kodu);
        query.SET_VALUE(":bts_hesap_kodu" , M_MUH_RV->bts_hesap_kodu);
    }
    else {
        query.SET_LIKE(":tam_hesap_kodu" , M_MUH_RV->bas_hesap_kodu);
    }

    if ( query.SELECT("tam_hesap_kodu ASC") EQ 0 ) {
        return;
    }

    QStringList muhasebe_hesabi;

    while ( query.NEXT() ) {

        muhasebe_hesabi.clear();

        int tali_hesap_mi = query.VALUE(5).toInt();

        if ( tali_hesap_mi EQ 0 ) {
            continue;
        }

        QString borc_bakiyesi_array;
        QString alacak_bakiyesi_array;

        borc_bakiyesi_array     = query.VALUE(6).toString();
        alacak_bakiyesi_array   = query.VALUE(7).toString();

        muhasebe_hesabi.append(query.VALUE(0).toString() + "\t" + query.VALUE(1).toString() + "\t" +
                               query.VALUE(2).toString() + "\t" + query.VALUE(3).toString() + "\t" +
                               query.VALUE(4).toString() + "\t" + query.VALUE(5).toString() + "\t" +
                               borc_bakiyesi_array       + "\t" + alacak_bakiyesi_array );

        M_TUM_HESAPLAR_LIST << muhasebe_hesabi;
    }
}















