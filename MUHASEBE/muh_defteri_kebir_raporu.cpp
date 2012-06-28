#include <QStringList>
#include <QString>
#include <QVariant>
#include "adak_sql.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "adak_utils.h"
#include "e9_enum.h"
#include "muh_defteri_kebir_raporu_class.h"
#include "muh_defteri_kebir_raporu_open.h"


extern ADAK_SQL * DB;

/*****************************************************************
              GET_DEFTERI_KEBIR_RAPORU
*****************************************************************/

void GET_DEFTERI_KEBIR_RAPORU (double font_size,double sayfa_genisligi,double sayfa_yuksekligi,MUH_RAPOR_VARS * rapor_vars)
{
    DEFTERI_KEBIR_RAPORU    * R = new DEFTERI_KEBIR_RAPORU(font_size,sayfa_genisligi,sayfa_yuksekligi,rapor_vars);
    R->CREATE_HTML_PAGES();
    R->OPEN_PRINTER_PREVIEW(NULL);
}

/*****************************************************************
       DEFTERI_KEBIR_RAPORU::DEFTERI_KEBIR_RAPORU
*****************************************************************/

DEFTERI_KEBIR_RAPORU::DEFTERI_KEBIR_RAPORU(double font_size,double sayfa_genisligi,double sayfa_yuksekligi,MUH_RAPOR_VARS * rapor_vars) : REPORT_KERNEL ("MUH_DEFTERI_KEBIR_RAPORU")
{
    M_MUH_RV = rapor_vars;

    SET_HEIGHTS     ( QVariant(font_size).toInt() * 3, QVariant(font_size).toInt() );

    SET_PAPER_SIZE  ( sayfa_genisligi,sayfa_yuksekligi );

    m_max_yazdirilacak_satir_sayisi = GET_MAX_LINE_COUNT (QVariant(font_size + 2.00).toInt());

    m_son_hesap_index          = 0;
    m_current_hesap_index      = -1;
    m_tum_satirlar_yazdirildi  = false;
    m_footer_borc              = 0.0;
    m_footer_alacak            = 0.0;
    m_borc_satirlari_bitti     = false;
    m_alacak_satirlari_bitti   = false;
    m_borc_satiri_sayisi       = 0;
    m_alacak_satiri_sayisi     = 0;
    m_borc_satirlari_index     = 0;
    m_alacak_satirlari_index   = 0;
    m_yazdirilan_satir_sayisi  = 0;
    m_font_size                = font_size;

    m_raporlanacak_kayit_var_mi = RAPOR_VERILERINI_AL();


    SQL_QUERY sql_query(DB);
    sql_query.PREPARE_SELECT("e9_sabit_degerler" , "firma_ismi");

    if (sql_query.SELECT() > 0) {
        sql_query.NEXT();
        m_firma_ismi = sql_query.VALUE(0).toString();
    }
}

/**************************************************************************************
                   DEFTERI_KEBIR_RAPORU::GET_HEADER
***************************************************************************************/

QString DEFTERI_KEBIR_RAPORU::GET_HEADER()
{

    SET_TABLE_TAG_STRING (QString("<TABLE WIDTH = 100% style=\"font-size:%1pt;\" border = 0 >").arg(m_font_size));

    SET_TD_TAG_STRING    (QStringList() << "WIDTH = 30%" << "WIDTH = 50% ALIGN=CENTER" << "WIDTH = 20% ALIGN=RIGHT");

    CREATE_TABLE         (QStringList()<<""<<""<<"",0,0);

    ADD_ROW_TO_TABLE     (QStringList() << m_firma_ismi << "" << "");

    ADD_ROW_TO_TABLE     (QStringList() << "" << "" << "");

    ADD_ROW_TO_TABLE     (QStringList() <<""<<"<b>DEFTER-İ KEBİR RAPORU"<<"");


    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   DEFTERI_KEBIR_RAPORU::GET_BODY
***************************************************************************************/

QString DEFTERI_KEBIR_RAPORU::GET_BODY()
{
    if ( m_raporlanacak_kayit_var_mi EQ false ) {
        return NULL;
    }

    if ( m_current_hesap_index NE m_son_hesap_index ) {
        m_current_hesap_index     = m_son_hesap_index;
        m_footer_borc             = 0.0;
        m_footer_alacak           = 0.0;
        m_yazdirilan_satir_sayisi = 0;
    }

    QStringList         SATIRLAR;
    QString             hesap_kodu_adi;
    QStringList         HEADERS;

    double              BORC_BAKIYESI   [E9_ARRAY_SIZE];
    double              ALACAK_BAKIYESI [E9_ARRAY_SIZE];

    bool borc_satiri_yazildi   = false;
    bool alacak_satiri_yazildi = false;

    bool calismamis_hesap_bulundu      = false;

    int baslangic_gun_no = M_MUH_RV->bas_gun_no;
    int bitis_gun_no     = M_MUH_RV->bts_gun_no;

    HEADERS << "Tarih"<< "Hesap Kodu"<<"Açıklama"<<"Yvm No"<<"Borç"<< "Tarih"<< "Hesap Kodu"
            <<"Açıklama"<<"Yvm No"<<"Alacak";

    for ( ;  ; ) {

        m_ana_hesabin_fis_satiri_var_mi = false;

        if ( m_tum_satirlar_yazdirildi EQ true ) {

            if ( M_MUH_RV->calismamis_hesaplar EQ false ) {
                return NULL;
            }
        }

        if ( m_son_hesap_index >= m_toplam_hesap_sayisi ) {
            return NULL;
        }

        hesap_kodu_adi  = QString ( "<h3 style=font-size:%1pt; align=center>" ).arg ( m_font_size );
        hesap_kodu_adi.append ( M_MUH_ANA_HESAPLAR[m_son_hesap_index].ana_hesap_kodu );
        hesap_kodu_adi.append (  "  "  );
        hesap_kodu_adi.append ( M_MUH_ANA_HESAPLAR [ m_son_hesap_index ].ana_hesap_ismi ) ;
        hesap_kodu_adi.append ( "</h3>" );

        SET_TABLE_TAG_STRING ( QString ( "<TABLE  WIDTH = 100%  style=\"font-size:%1pt;\"  border = 0 >").arg ( m_font_size ) );

        SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 9%"<<"WIDTH = 8%"<<"WIDTH = 18%"<<"WIDTH = 6%"<<"WIDTH = 9%"
                                         <<"WIDTH = 9%"<<"WIDTH = 8%"<<"WIDTH = 18%"<<"WIDTH = 6%"<<"WIDTH = 9%" );
        CREATE_TABLE ( HEADERS, 1, 13 );

        if ( m_yazdirilan_satir_sayisi > 0 AND
             m_yazdirilan_satir_sayisi EQ m_max_yazdirilacak_satir_sayisi ) {
            m_yazdirilan_satir_sayisi = 0;
            DEVIR_BAKIYESINI_YAZDIR();
        }

        UNPACK_DOUBLE_ARRAY ( M_MUH_ANA_HESAPLAR [ m_son_hesap_index ].borc_bakiyesi   , BORC_BAKIYESI   , E9_ARRAY_SIZE );
        UNPACK_DOUBLE_ARRAY ( M_MUH_ANA_HESAPLAR [ m_son_hesap_index ].alacak_bakiyesi , ALACAK_BAKIYESI , E9_ARRAY_SIZE );

        if ( M_MUH_RV->bakiyesiz_hesaplar EQ false ) {
            if ( BORC_BAKIYESI [ bitis_gun_no ] EQ ALACAK_BAKIYESI [ bitis_gun_no ] ) {
                if ( BORC_BAKIYESI[bitis_gun_no ] NE 0 AND ALACAK_BAKIYESI [ bitis_gun_no ]  NE 0 ) {
                    m_son_hesap_index++;
                    continue;
                }
            }
        }

        if ( ALACAK_BAKIYESI [ baslangic_gun_no-1 ] EQ ALACAK_BAKIYESI [ bitis_gun_no ]
           AND BORC_BAKIYESI [ baslangic_gun_no-1 ] EQ BORC_BAKIYESI [ bitis_gun_no ] ) {
            calismamis_hesap_bulundu = true;
        }

        if ( M_MUH_RV->calismamis_hesaplar EQ false ) {

            if ( calismamis_hesap_bulundu EQ true ) {
                 calismamis_hesap_bulundu = false;
                 m_son_hesap_index++;
                continue;
            }
        }

        SET_TD_TAG_STRING ( QStringList()<<"WIDTH = 9%"<<"WIDTH = 8%"<<"WIDTH = 18%"<<"WIDTH = 6%"<<"WIDTH = 9% ALIGN=RIGHT"
                                         <<"WIDTH = 9%"<<"WIDTH = 8%"<<"WIDTH = 18%"<<"WIDTH = 6%"<<"WIDTH = 9% ALIGN=RIGHT");
        if ( calismamis_hesap_bulundu EQ true ) {
            m_son_hesap_index++;
            return hesap_kodu_adi.append ( GET_TABLE_HTML_STRING() );
        }

        for ( ; ; ) {

            borc_satiri_yazildi   = false;
            alacak_satiri_yazildi = false;

            if ( m_borc_satirlari_bitti EQ false ) {

                QString  ana_hesap_kodu =  M_MUH_ANA_HESAPLAR [ m_son_hesap_index ].ana_hesap_kodu;
                QString  hesap_kodu     =  M_DEFTERI_KEBIR_BORC_SATIRLARI [ m_borc_satirlari_index ].satir_tam_hesap_kodu.mid ( 0, ana_hesap_kodu.size() );

                if ( hesap_kodu > ana_hesap_kodu ) {
                    borc_satiri_yazildi = false;
                }
                else {
                    if ( hesap_kodu < ana_hesap_kodu ) {

                        for ( ; ; ) {
                            m_borc_satirlari_index++;

                            if ( m_borc_satiri_sayisi EQ m_borc_satirlari_index ) {
                                m_borc_satirlari_bitti = true;
                                break;
                            }

                            QString satir_tam_hesap_kodu = M_DEFTERI_KEBIR_BORC_SATIRLARI [ m_borc_satirlari_index ].satir_tam_hesap_kodu;

                            hesap_kodu     =  satir_tam_hesap_kodu.mid ( 0, ana_hesap_kodu.size());

                            if ( hesap_kodu > ana_hesap_kodu ) {
                                borc_satiri_yazildi = false;
                                break;
                            }
                            if ( ana_hesap_kodu EQ hesap_kodu ) {
                                break;
                            }
                        }
                    }
                }

                double   borc_tutari    =  M_DEFTERI_KEBIR_BORC_SATIRLARI[m_borc_satirlari_index].borc_tutari;

                if ( hesap_kodu EQ ana_hesap_kodu ) {
                    SATIRLAR << M_DEFTERI_KEBIR_BORC_SATIRLARI [ m_borc_satirlari_index ].fis_tarihi
                             << M_DEFTERI_KEBIR_BORC_SATIRLARI [ m_borc_satirlari_index ].satir_tam_hesap_kodu
                             << M_DEFTERI_KEBIR_BORC_SATIRLARI [ m_borc_satirlari_index ].aciklama
                             << QVariant ( M_DEFTERI_KEBIR_BORC_SATIRLARI [ m_borc_satirlari_index ].yevmiye_no ).toString()
                             << VIRGUL_EKLE ( QVariant ( borc_tutari ).toString(), 2 );
                    m_footer_borc += M_DEFTERI_KEBIR_BORC_SATIRLARI [ m_borc_satirlari_index ].borc_tutari;
                    borc_satiri_yazildi             = true;
                    m_ana_hesabin_fis_satiri_var_mi = true;
                    if ( m_borc_satirlari_index EQ m_borc_satiri_sayisi -1 ) {
                        m_borc_satirlari_bitti = true;
                    }
                    m_borc_satirlari_index++;
                }
            }

            if ( m_alacak_satirlari_bitti EQ false ) {

                QString  ana_hesap_kodu =  M_MUH_ANA_HESAPLAR [ m_son_hesap_index ].ana_hesap_kodu;
                QString  hesap_kodu     =  M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ m_alacak_satirlari_index ].satir_tam_hesap_kodu.mid ( 0, ana_hesap_kodu.size() );

                if ( hesap_kodu > ana_hesap_kodu ) {
                    alacak_satiri_yazildi = false;
                }
                else {
                    if ( hesap_kodu < ana_hesap_kodu ) {

                        for ( ; ; ) {
                            m_alacak_satirlari_index++;

                            if ( m_alacak_satirlari_index EQ m_alacak_satiri_sayisi ) {
                                m_alacak_satirlari_bitti = true;
                                break;
                            }

                            hesap_kodu     =  M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ m_alacak_satirlari_index ].satir_tam_hesap_kodu.mid ( 0, ana_hesap_kodu.size());

                            if ( hesap_kodu > ana_hesap_kodu ) {
                                alacak_satiri_yazildi = false;
                                break;
                            }
                            if ( ana_hesap_kodu EQ hesap_kodu ) {
                                break;
                            }
                        }
                    }
                }

                double   alacak_tutari  =  M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ m_alacak_satirlari_index ].alacak_tutari;

                if ( hesap_kodu EQ ana_hesap_kodu ) {
                    if ( borc_satiri_yazildi EQ false ) {
                        SATIRLAR << "" << "" << "" << "" << "";
                    }
                    SATIRLAR << M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ m_alacak_satirlari_index ].fis_tarihi
                             << M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ m_alacak_satirlari_index ].satir_tam_hesap_kodu
                             << M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ m_alacak_satirlari_index ].aciklama
                             << QVariant ( M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ m_alacak_satirlari_index ].yevmiye_no ).toString()
                             << VIRGUL_EKLE ( QVariant ( alacak_tutari ).toString(), 2 );
                    m_footer_alacak += M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ m_alacak_satirlari_index ].alacak_tutari;
                    alacak_satiri_yazildi           = true;
                    m_ana_hesabin_fis_satiri_var_mi = true;

                    if ( m_alacak_satirlari_index EQ m_alacak_satiri_sayisi -1 ) {
                         m_alacak_satirlari_bitti = true;
                    }
                    m_alacak_satirlari_index++;
                }
            }

            if ( m_borc_satirlari_bitti EQ true AND m_alacak_satirlari_bitti EQ true ) {
                 m_tum_satirlar_yazdirildi = true;
            }

            if ( borc_satiri_yazildi EQ false AND alacak_satiri_yazildi EQ false ) {
                 m_son_hesap_index++;
                break;
            }

            if ( borc_satiri_yazildi EQ true AND alacak_satiri_yazildi EQ false ) {
                SATIRLAR << "" << "" << "" << "" << "";
            }

            ADD_ROW_TO_TABLE(SATIRLAR);
            SATIRLAR.clear();
            m_yazdirilan_satir_sayisi++;

            if ( m_yazdirilan_satir_sayisi EQ m_max_yazdirilacak_satir_sayisi ) {
                break;
            }
        }

        if ( m_ana_hesabin_fis_satiri_var_mi EQ false ) {
            continue;
        }
        return hesap_kodu_adi.append ( GET_TABLE_HTML_STRING() );
    }
    return NULL;
}

/**************************************************************************************
                   DEFTERI_KEBIR_RAPORU::GET_FOOTER
***************************************************************************************/

QString DEFTERI_KEBIR_RAPORU::GET_FOOTER()
{

  QStringList string_list;

  SET_TABLE_TAG_STRING ( QString ( "<TABLE  WIDTH = 100%  border=0 style=\"font-size:%1pt;\">").arg ( m_font_size ) );
  SET_TD_TAG_STRING    ( QStringList() << "WIDTH = 50% ALIGN=RIGHT" << "WIDTH = 50% ALIGN=RIGHT" );

  CREATE_TABLE         ( QStringList() << "" << "" , 0 , 0 );

  string_list.clear();
  string_list << "<b>Ara Toplam : " + VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_borc ) ).toString() ) ;
  string_list << "<b>Ara Toplam : " + VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_alacak ) ).toString() );

  ADD_ROW_TO_TABLE (string_list);

  return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   DEFTERI_KEBIR_RAPORU::DEVIR_BAKIYESINI_YAZDIR()
***************************************************************************************/

void DEFTERI_KEBIR_RAPORU::DEVIR_BAKIYESINI_YAZDIR()
{
    QStringList string_list;

    string_list.clear();
    string_list << "<b>ÖNCEKİ SAYFADAN DEVİR :"<<""<<""<<"";
    string_list << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_borc    ) ).toString() );
    string_list << ""<<""<<""<<"";
    string_list << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_alacak  ) ).toString() );

    ADD_ROW_TO_TABLE ( string_list );
    m_yazdirilan_satir_sayisi++;
}

/**************************************************************************************
                   DEFTERI_KEBIR_RAPORU::RAPOR_VERILERINI_AL
***************************************************************************************/

bool DEFTERI_KEBIR_RAPORU::RAPOR_VERILERINI_AL ()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" ,
                             "hesap_id, tam_hesap_kodu, hesap_ismi,toplam_borc_array, toplam_alacak_array" ,
                             "( (tam_hesap_kodu >= :bas_hesap_kodu "
                             "AND tam_hesap_kodu <=  :bts_hesap_kodu ) OR tam_hesap_kodu LIKE :bas_hesap_kodu_ "
                             "OR tam_hesap_kodu LIKE  :bts_hesap_kodu_) "
                             "AND parent_id = :parent_id ");
    sql_query.SET_VALUE(":bas_hesap_kodu"      , QString(M_MUH_RV->bas_hesap_kodu + "%"));
    sql_query.SET_VALUE(":bts_hesap_kodu"      , QString(M_MUH_RV->bts_hesap_kodu + "%"));
    sql_query.SET_VALUE (":bas_hesap_kodu_" , QString(M_MUH_RV->bas_hesap_kodu + "%"));
    sql_query.SET_VALUE(":bts_hesap_kodu_"  , QString(M_MUH_RV->bts_hesap_kodu + "%"));


    sql_query.SET_VALUE(":parent_id"      , 0);

    if ( sql_query.SELECT() EQ 0 ) {
        return false;
    }
    m_toplam_hesap_sayisi = sql_query.NUM_OF_ROWS();
    M_MUH_ANA_HESAPLAR    = new DEFTERI_KEBIR_MUH_ANA_HESAPLAR [ m_toplam_hesap_sayisi ];

    int hesap_index = 0;

    while ( sql_query.NEXT() EQ true ) {
        M_MUH_ANA_HESAPLAR [ hesap_index ].ana_hesap_id   = sql_query.VALUE(0).toInt();
        M_MUH_ANA_HESAPLAR [ hesap_index ].ana_hesap_kodu = sql_query.VALUE(1).toString();
        M_MUH_ANA_HESAPLAR [ hesap_index ].ana_hesap_ismi = sql_query.VALUE(2).toString();
        M_MUH_ANA_HESAPLAR [ hesap_index ].borc_bakiyesi  = sql_query.VALUE(3).toString();
        M_MUH_ANA_HESAPLAR [ hesap_index ].alacak_bakiyesi= sql_query.VALUE(4).toString();
        hesap_index++;
    }

    sql_query.PREPARE_SELECT("muh_hesaplar,muh_fisler,muh_fis_satirlari" ,
                             "muh_fis_satirlari.hesap_id, yevmiye_numarasi, "
                             "borc_tutari, alacak_tutari, tam_hesap_kodu, hesap_ismi, "
                             "muh_fis_satirlari.aciklama, fis_tarihi",
                             "muh_hesaplar.hesap_id = muh_fis_satirlari.hesap_id "
                             "AND muh_fisler.fis_id = muh_fis_satirlari.fis_id "
                             "AND ( (tam_hesap_kodu >= :bas_hesap_kodu "
                             "AND tam_hesap_kodu <=  :bts_hesap_kodu) OR tam_hesap_kodu LIKE :bas_hesap_kodu_ "
                             "OR tam_hesap_kodu LIKE  :bts_hesap_kodu_) "
                             "AND fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi "
                             "AND alacak_tutari = :alacak_tutari AND fis_turu != :fis_turu ");
    sql_query.SET_VALUE(":bas_hesap_kodu" , M_MUH_RV->bas_hesap_kodu);
    sql_query.SET_VALUE(":bts_hesap_kodu" , M_MUH_RV->bts_hesap_kodu);
    sql_query.SET_VALUE(":bas_hesap_kodu_" , M_MUH_RV->bas_hesap_kodu);
    sql_query.SET_VALUE(":bts_hesap_kodu_" , M_MUH_RV->bts_hesap_kodu);
    sql_query.SET_VALUE(":bas_tarihi"     , M_MUH_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    sql_query.SET_VALUE(":bts_tarihi"     , M_MUH_RV->bitis_tarihi.toString("yyyy.MM.dd"));
    sql_query.SET_VALUE(":alacak_tutari"  , 0);
    sql_query.SET_VALUE(":fis_turu"       , ENUM_ACILIS_FISI);


    if ( sql_query.SELECT("muh_hesaplar.tam_hesap_kodu ASC, "
                          "muh_fisler.fis_tarihi ASC, "
                          "muh_fisler.yevmiye_numarasi ASC") > 0 ) {


        m_borc_satiri_sayisi = sql_query.NUM_OF_ROWS();
        M_DEFTERI_KEBIR_BORC_SATIRLARI = new DEFTERI_KEBIR_FIS_SATIRLARI [ m_borc_satiri_sayisi ];
        int satir_index = 0;
        while ( sql_query.NEXT() EQ true ) {

            M_DEFTERI_KEBIR_BORC_SATIRLARI [ satir_index ].satir_hesap_id           = sql_query.VALUE(0).toInt();
            M_DEFTERI_KEBIR_BORC_SATIRLARI [ satir_index ].yevmiye_no               = sql_query.VALUE(1).toInt();
            M_DEFTERI_KEBIR_BORC_SATIRLARI [ satir_index ].borc_tutari              = sql_query.VALUE(2).toDouble();
            M_DEFTERI_KEBIR_BORC_SATIRLARI [ satir_index ].alacak_tutari            = sql_query.VALUE(3).toDouble();
            M_DEFTERI_KEBIR_BORC_SATIRLARI [ satir_index ].satir_tam_hesap_kodu     = sql_query.VALUE(4).toString();
            M_DEFTERI_KEBIR_BORC_SATIRLARI [ satir_index ].satir_hesap_ismi         = sql_query.VALUE(5).toString();
            M_DEFTERI_KEBIR_BORC_SATIRLARI [ satir_index ].aciklama                 = sql_query.VALUE(6).toString();
            M_DEFTERI_KEBIR_BORC_SATIRLARI [ satir_index ].fis_tarihi               = QDate::fromString ( sql_query.VALUE(7).toString(), "yyyy.MM.dd").toString("dd/MM/yyyy");
            satir_index++;
        }
    }
    else {
        m_borc_satirlari_bitti = true;
    }

    sql_query.PREPARE_SELECT("muh_hesaplar, muh_fisler, muh_fis_satirlari" , "muh_fis_satirlari.hesap_id, yevmiye_numarasi, "
                             "borc_tutari, alacak_tutari, tam_hesap_kodu, hesap_ismi, "
                             "muh_fis_satirlari.aciklama, fis_tarihi" , "muh_hesaplar.hesap_id = muh_fis_satirlari.hesap_id "
                             "AND muh_fisler.fis_id = muh_fis_satirlari.fis_id "
                             "AND ( (tam_hesap_kodu >= :bas_hesap_kodu "
                             "AND tam_hesap_kodu <=  :bts_hesap_kodu) OR tam_hesap_kodu LIKE :bas_hesap_kodu_ "
                             "OR tam_hesap_kodu LIKE  :bts_hesap_kodu_) "
                             "AND fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi "
                             "AND borc_tutari = :borc_tutari AND fis_turu != :fis_turu ");

    sql_query.SET_VALUE(":bas_hesap_kodu"  , M_MUH_RV->bas_hesap_kodu);
    sql_query.SET_VALUE(":bts_hesap_kodu"  , M_MUH_RV->bts_hesap_kodu);
    sql_query.SET_VALUE(":bas_hesap_kodu_" , M_MUH_RV->bas_hesap_kodu);
    sql_query.SET_VALUE(":bts_hesap_kodu_" , M_MUH_RV->bts_hesap_kodu);
    sql_query.SET_VALUE(":bas_tarihi"      , M_MUH_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    sql_query.SET_VALUE(":bts_tarihi"      , M_MUH_RV->bitis_tarihi.toString("yyyy.MM.dd"));
    sql_query.SET_VALUE(":borc_tutari"     , 0);
    sql_query.SET_VALUE(":fis_turu"        , ENUM_ACILIS_FISI);

    if ( sql_query.SELECT("muh_hesaplar.tam_hesap_kodu ASC, "
                          "muh_fisler.fis_tarihi ASC, "
                          "muh_fisler.yevmiye_numarasi ASC") > 0 ) {

        m_alacak_satiri_sayisi = sql_query.NUM_OF_ROWS();

        M_DEFTERI_KEBIR_ALACAK_SATIRLARI = new DEFTERI_KEBIR_FIS_SATIRLARI [ m_alacak_satiri_sayisi ];

        int satir_index = 0;

        while ( sql_query.NEXT() EQ true ) {

            M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ satir_index].satir_hesap_id           = sql_query.VALUE(0).toInt();
            M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ satir_index].yevmiye_no               = sql_query.VALUE(1).toInt();
            M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ satir_index].borc_tutari              = sql_query.VALUE(2).toDouble();
            M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ satir_index].alacak_tutari            = sql_query.VALUE(3).toDouble();
            M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ satir_index].satir_tam_hesap_kodu     = sql_query.VALUE(4).toString();
            M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ satir_index].satir_hesap_ismi         = sql_query.VALUE(5).toString();
            M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ satir_index].aciklama                 = sql_query.VALUE(6).toString();
            M_DEFTERI_KEBIR_ALACAK_SATIRLARI [ satir_index].fis_tarihi               = QDate::fromString(sql_query.VALUE(7).toString(), "yyyy.MM.dd").toString ( "dd/MM/yyyy" );
            satir_index++;
        }
    }
    else {
        m_alacak_satirlari_bitti = true;
    }
    return true;
}
