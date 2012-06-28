#include <QStringList>
#include "adak_utils.h"
#include "cari_hesap_ekstresi_raporu_class.h"
#include "QCommaEdit.h"
#include "e9_enum.h"
#include "cari_struct.h" 
#include "e9_console_utils.h"
#include "yonetim.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "adres_console_utils.h"
#include "cari_enum.h"

extern ADAK_SQL * DB;


/**************************************************************************************
                  GET_CARI_HESAP_EKSTRESI_RAPORU
***************************************************************************************/

//Not - 2 : Font genislikleri icin html de kullanilan <font size=X> tagi yerine css de kullanilan style="font-size:18pt" kullanilmalidir. Boylelikle sizelarimiz bir birime(pt) gore atanir. Ayni zamanda "size" ekran duzenine gore bir katsayi gibi davranirken "pt" ile belirtigimiz bir boyut daima sabittir.

QString GET_CARI_HESAP_EKSTRESI_RAPORU ( CARI_RAPOR_VARS * P_CARI_RV)
{
    CARI_HESAP_EKSTRESI_RAPORU  *  R = new CARI_HESAP_EKSTRESI_RAPORU( P_CARI_RV);
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
                  CARI_HESAP_EKSTRESI_RAPORU::CARI_HESAP_EKSTRESI_RAPORU
***************************************************************************************/

CARI_HESAP_EKSTRESI_RAPORU::CARI_HESAP_EKSTRESI_RAPORU( CARI_RAPOR_VARS * P_CARI_RV) : REPORT_KERNEL ("CARI_HESAP_EKSTRESI_RAPORU")
{

    M_CARI_RV  = P_CARI_RV;
    SET_HEIGHTS (30,30);
    m_cari_hesaplar_list         =  GET_CARI_HESAPLAR_LIST();
    m_last_hesap_indisi          =  0;
    m_max_line_count             =  GET_MAX_LINE_COUNT (12);
    m_gonderilen_kayit_sayisi    =  0;
    m_last_satir_indisi          =  0;
    m_satirlar_basildi           =  true;
}

/**************************************************************************************
                  CARI_HESAP_EKSTRESI_RAPORU::GET_HEADER
***************************************************************************************/

QString CARI_HESAP_EKSTRESI_RAPORU::GET_HEADER()
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
        rapor_kriterleri.append("Bakiyesiz ");
    }
    if ( M_CARI_RV->calismamis_hesaplar EQ true ) {
        if (rapor_kriterleri.isEmpty() EQ false ) {
            rapor_kriterleri.append(" - ");
        }
        rapor_kriterleri.append("Çalışmamış ");
    }
    if (rapor_kriterleri.isEmpty() EQ false ) {
        rapor_kriterleri.append("Hesaplar ");
    }


    SET_TABLE_TAG_STRING("<TABLE style= \"width:100%;font-size:75%;\" BORDER = 0>");

    SET_TD_TAG_STRING(QStringList()<<"style= \"width:30%;font-size:100%;\" "<<"style= \"width:40%; font-size:100%;\"ALIGN=CENTER"
                      <<"style = \"width:30%; font-size:100%;\" ALIGN=RIGHT");

    CREATE_TABLE(QStringList()<<""<<""<<"",0,0);

    ADD_ROW_TO_TABLE(QStringList()<< E9_GET_FIRMA_ISMI()<<"<b>CARİ HESAP EKSTRESİ"<<GET_REPORT_TOP_RIGHT_HEADER());
    ADD_ROW_TO_TABLE(QStringList()<<""<< baslangic_tarihi + "-" + bitis_tarihi <<"");
    ADD_ROW_TO_TABLE(QStringList()<<""<< rapor_kriterleri << "");
    ADD_ROW_TO_TABLE(QStringList()<<""<< hesap_araligi    <<"");

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                  CARI_HESAP_EKSTRESI_RAPORU::GET_BODY
***************************************************************************************/

QString CARI_HESAP_EKSTRESI_RAPORU::GET_BODY()
{
    QStringList         headers;
    QString             cari_hesap_kodu_adi;
    QStringList         cari_hesap;
    double              toplam_borc;
    double              toplam_alacak;

    QStringList         fis_satiri;

    double toplam_borc_array  [E9_ARRAY_SIZE];
    double toplam_alacak_array[E9_ARRAY_SIZE];


    headers << "Tarih" << "Fiş No" << "Açıklama" << "Borç" << "Alacak" << "Borç Bakiye" << "Alacak Bakiye";

    for ( ; ; ) {

        if ( m_satirlar_basildi EQ 1 ) {
            m_gonderilen_kayit_sayisi = 0;
            m_last_satir_indisi       = 0;
            m_satirlar_basildi        = false;
            m_footer_toplam_borc      = 0.0;
            m_footer_toplam_alacak    = 0.0;
        }

        if (m_last_hesap_indisi >= m_cari_hesaplar_list.size()) {
            return NULL;
        }
        SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%; \" BORDER = 1>");
        SET_TD_TAG_STRING ( QStringList() << "style = \"width:18%;\"" << "style = \"width:7%\"" << "style  = \"width:35%;\""
                            << "style = \"width:9%;\"" << "style = \"width:9%;\"" << "style = \"width:11%;\""
                            << "style = \"width:11%;\"");

        CREATE_TABLE (  headers,1,14);
        SET_TD_TAG_STRING(QStringList()<< "" << "" <<""<<"align = RIGHT "
                                       <<"align = RIGHT "<<"align = RIGHT "<<"align = RIGHT ");

        cari_hesap.clear();
        cari_hesap = m_cari_hesaplar_list.at(m_last_hesap_indisi).split("\t");

        cari_hesap_kodu_adi = "<p align=center style = \"font-size:85%; font-weight:bold;\">"
                              + cari_hesap.at(1) + "  " + cari_hesap.at(2) + " ( " + cari_hesap.at(3)+ " )</p>";



        int cari_hesap_id = QVariant(cari_hesap.at(0)).toInt();

        UNPACK_DOUBLE_ARRAY(cari_hesap.at(4) , toplam_borc_array   , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(cari_hesap.at(5) , toplam_alacak_array , E9_ARRAY_SIZE);

        int bas_gun_no = MALI_YIL_ARRAY_INDIS ( M_CARI_RV->baslangic_tarihi);
        int bts_gun_no = MALI_YIL_ARRAY_INDIS ( M_CARI_RV->bitis_tarihi);

        if ( M_CARI_RV->nakli_yekun EQ 1 ) {
            toplam_borc   = toplam_borc_array[bts_gun_no];
            toplam_alacak = toplam_alacak_array[bts_gun_no];
        }
        else {
            toplam_borc  = toplam_borc_array[bts_gun_no] - toplam_borc_array[bas_gun_no -1];
            toplam_alacak = toplam_alacak_array[bts_gun_no] - toplam_alacak_array[bas_gun_no-1];
        }

        if ( M_CARI_RV->bakiyesi_hesaplar EQ false ) {
            if (toplam_borc EQ toplam_alacak ) {
                if (toplam_borc NE 0 AND toplam_alacak NE 0 ) {
                    m_last_hesap_indisi++;
                    continue;
                }
            }
        }
        if ( M_CARI_RV->calismamis_hesaplar EQ false ) {
            if (toplam_borc_array[bas_gun_no-1] EQ toplam_borc_array[bts_gun_no] AND
                toplam_alacak_array[bas_gun_no-1] EQ toplam_alacak_array[bts_gun_no]) {
                if (toplam_borc EQ 0 AND toplam_alacak EQ 0 ) {
                    m_last_hesap_indisi++;
                    continue;
                }
            }
        }
        if ( m_last_satir_indisi EQ 0 ) {
            m_fis_satirlari.clear();
            m_fis_satirlari = GET_CARI_HESAP_ISLEMLERI(cari_hesap_id);
        }
        if ( M_CARI_RV->nakli_yekun EQ 1 ) {
            if (m_gonderilen_kayit_sayisi EQ 0 ) {
                PRINT_NAKLI_YEKUN( toplam_borc_array[bas_gun_no-1],toplam_alacak_array[bas_gun_no-1]);
                m_footer_toplam_borc   = toplam_borc_array  [bas_gun_no-1];
                m_footer_toplam_alacak = toplam_alacak_array[bas_gun_no-1];
                m_nakli_yekun_borc     = toplam_borc_array  [bas_gun_no-1];
                m_nakli_yekun_alacak   = toplam_alacak_array[bas_gun_no-1];
            }
        }
        m_gonderilen_kayit_sayisi = 0;
        QStringList  basilacak_satir;

        for (int i = m_last_satir_indisi ; i < m_fis_satirlari.size(); i++ ) {
            fis_satiri.clear();

            fis_satiri  = m_fis_satirlari.at(i).split("\t");

            basilacak_satir.clear();

            m_footer_toplam_borc      += QVariant(fis_satiri.at(3)).toDouble();
            m_footer_toplam_alacak    += QVariant(fis_satiri.at(4)).toDouble();

            double alacak_bakiye = 0.0;
            double borc_bakiye   = 0.0;

            if ( M_CARI_RV->nakli_yekun EQ 1 ) {
                if ( m_nakli_yekun_alacak  NE 0.0 ) {
                    m_nakli_yekun_alacak -= QVariant(fis_satiri.at(3)).toDouble();
                    m_nakli_yekun_alacak += QVariant(fis_satiri.at(4)).toDouble();

                    alacak_bakiye = m_nakli_yekun_alacak - m_nakli_yekun_borc;

                    if (alacak_bakiye < 0) {
                        borc_bakiye          = -1 * alacak_bakiye;
                        alacak_bakiye        = 0.0;
                        m_nakli_yekun_borc   = borc_bakiye;
                        m_nakli_yekun_alacak = alacak_bakiye;
                    }
                }
                else if ( m_nakli_yekun_borc NE 0.0 ) {
                    m_nakli_yekun_borc -= QVariant(fis_satiri.at(4)).toDouble();
                    m_nakli_yekun_borc += QVariant(fis_satiri.at(3)).toDouble();

                    borc_bakiye = m_nakli_yekun_borc - m_nakli_yekun_alacak;

                    if (borc_bakiye < 0) {
                        alacak_bakiye        = -1 * borc_bakiye;
                        borc_bakiye          = 0.0;
                        m_nakli_yekun_alacak = alacak_bakiye;
                        m_nakli_yekun_borc   = borc_bakiye;
                    }
                }
                else {
                    borc_bakiye   = QVariant(fis_satiri.at(5)).toDouble();
                    alacak_bakiye = QVariant(fis_satiri.at(6)).toDouble();
                }
            }
            else {
                borc_bakiye   = QVariant(fis_satiri.at(5)).toDouble();
                alacak_bakiye = QVariant(fis_satiri.at(6)).toDouble();
            }


            basilacak_satir << QVariant(fis_satiri.at(0)).toDate().toString("dd MMMM yyyy")
                            << fis_satiri.at(1)
                            << fis_satiri.at(2)
                            << VIRGUL_EKLE(fis_satiri.at(3))
                            << VIRGUL_EKLE(fis_satiri.at(4))
                            << VIRGUL_EKLE(QVariant(ROUND(borc_bakiye)).toString())
                            << VIRGUL_EKLE(QVariant(ROUND(alacak_bakiye)).toString());

            ADD_ROW_TO_TABLE(basilacak_satir);

            m_gonderilen_kayit_sayisi++;
            m_last_satir_indisi++;

            if (m_gonderilen_kayit_sayisi EQ m_max_line_count ) {
                m_satirlar_basildi = false;
                cari_hesap_kodu_adi.append(ADD_HTML_SPACE(" "));
                return cari_hesap_kodu_adi.append(GET_TABLE_HTML_STRING());
            }
        }
        m_satirlar_basildi = true;
        m_last_hesap_indisi++;
        m_last_satir_indisi = 0;
        break;
    }
    cari_hesap_kodu_adi.append(ADD_HTML_SPACE(" "));
    return cari_hesap_kodu_adi.append(GET_TABLE_HTML_STRING());
}

/**************************************************************************************
                  CARI_HESAP_EKSTRESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString CARI_HESAP_EKSTRESI_RAPORU::GET_FOOTER()
{
    double footer_borc_bakiye   = 0.0;
    double footer_alacak_bakiye = 0.0;

    SET_TABLE_TAG_STRING("<TABLE style=\"width:100%; font-size:75%; font-weight:bold\" BORDER=0>");
    SET_TD_TAG_STRING(QStringList() << "style=\"width:60%;\""<< "style=\"width:9%;\" ALIGN=RIGHT"
                                    << "style=\"width:9%;\" ALIGN=RIGHT" << "style=\"width:11%;\" ALIGN=RIGHT"
                                    << "style=\"width:11%;\" ALIGN=RIGHT");

    CREATE_TABLE (QStringList()<< "" << "" << "" << "" << "",0,0);

    QStringList footer_line;

    if(m_footer_toplam_borc > m_footer_toplam_alacak ) {
       footer_borc_bakiye  = m_footer_toplam_borc - m_footer_toplam_alacak;
       footer_alacak_bakiye = 0.0;
    }
    else {
       footer_alacak_bakiye = m_footer_toplam_alacak - m_footer_toplam_borc;
       footer_borc_bakiye   = 0.0;
    }

    footer_line << "Toplam" << VIRGUL_EKLE(QVariant(ROUND(m_footer_toplam_borc)).toString())
            << VIRGUL_EKLE (QVariant (ROUND (m_footer_toplam_alacak)).toString())
            << VIRGUL_EKLE (QVariant (ROUND (footer_borc_bakiye    )).toString())
            << VIRGUL_EKLE (QVariant (ROUND (footer_alacak_bakiye  )).toString());
    ADD_ROW_TO_TABLE(footer_line);

    return GET_TABLE_HTML_STRING();


}

/**************************************************************************************
                  CARI_HESAP_EKSTRESI_RAPORU::GET_CARI_HESAPLAR_LIST
***************************************************************************************/

QStringList CARI_HESAP_EKSTRESI_RAPORU::GET_CARI_HESAPLAR_LIST()
{
    SQL_QUERY select_query(DB);
    QStringList cari_hesap;
    QStringList cari_hesaplar;
    QString toplam_borc;
    QString toplam_alacak;

    select_query.PREPARE_SELECT ("car_hesaplar","cari_hesap_id,cari_hesap_kodu,adres_id,para_birimi_id,"
                                 "toplam_borc_array , toplam_alacak_array" ,
                                 "cari_hesap_kodu BETWEEN :bas_hesap_kodu AND :bts_hesap_kodu ");
    select_query.SET_VALUE      (":bas_hesap_kodu" , M_CARI_RV->bas_hesap_kodu);
    select_query.SET_VALUE      (":bts_hesap_kodu" , M_CARI_RV->bts_hesap_kodu);

    if (select_query.SELECT( "cari_hesap_kodu ASC") EQ 0 ) {
        return QStringList();
    }
    while (select_query.NEXT()) {
        QString cari_hesap_id       = select_query.VALUE(0).toString();
        QString cari_hesap_kodu     = select_query.VALUE(1).toString();
        QString cari_hesap_ismi     = ADR_GET_FIRMA_SAHIS_ADI(select_query.VALUE(2).toInt());
        int     cari_para_birimi_id = select_query.VALUE(3).toInt();

        toplam_borc   = select_query.VALUE(4).toString();
        toplam_alacak = select_query.VALUE(5).toString();

        cari_hesap.clear();

        cari_hesap.append( cari_hesap_id + "\t" + cari_hesap_kodu + "\t" + cari_hesap_ismi + "\t" +
                          DVZ_GET_DOVIZ_KODU( cari_para_birimi_id ) + "\t" + toplam_borc +
                          "\t" + toplam_alacak);
        cari_hesaplar.append(cari_hesap);
    }

    return cari_hesaplar;
}

/**************************************************************************************
                  CARI_HESAP_EKSTRESI_RAPORU::GET_CARI_HESAP_ISLEMLERI
***************************************************************************************/

QStringList CARI_HESAP_EKSTRESI_RAPORU::GET_CARI_HESAP_ISLEMLERI  ( int cari_hesap_id )
{
    //Fonksiyon Cari Hesabin islem gordugu fis satirlarini,fis tarihine gore sirali olarak dondurur.

    SQL_QUERY   select_query( DB );
    QStringList fisler;
    QStringList fis;
    QStringList fis_satiri;
    QStringList fis_satirlari_list;
    double      borc_bakiye;
    double      alacak_bakiye;
    double      toplam_borc   = m_footer_toplam_borc;
    double      toplam_alacak = m_footer_toplam_alacak;


    select_query.PREPARE_SELECT ( "car_fisler", "fis_id , fis_tarihi , fis_turu , fis_no, modul_id",
                                  "cari_hesap_id = :cari_hesap_id "
                                  "AND fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi " );

    select_query.SET_VALUE      ( ":cari_hesap_id"  , cari_hesap_id);
    select_query.SET_VALUE      ( ":bas_tarihi"     , M_CARI_RV->baslangic_tarihi.toString ( "yyyy.MM.dd"));
    select_query.SET_VALUE      ( ":bts_tarihi"     , M_CARI_RV->bitis_tarihi.toString ( "yyyy.MM.dd"));

    if ( select_query.SELECT( "fis_tarihi , fis_no") NE 0 ) {
        while (select_query.NEXT()) {

            QString fis_bilgisi = select_query.VALUE(1).toString() +"\t" +
                                  select_query.VALUE(0).toString() +"\t" +
                                  select_query.VALUE(2).toString() +"\t" +
                                  select_query.VALUE(3).toString() +"\t" +
                                  select_query.VALUE(4).toString();

            if( fisler.contains( fis_bilgisi ) EQ false ) {
                fisler.append( fis_bilgisi );
            }
        }
    }

    select_query.PREPARE_SELECT( "car_fisler,car_fis_satirlari","car_fisler.fis_id,fis_tarihi,fis_turu,fis_no, car_fisler.modul_id",
                                 "car_fis_satirlari.cari_hesap_id = :cari_hesap_id "
                                 "AND car_fisler.fis_id = car_fis_satirlari.fis_id "
                                 "AND car_fisler.fis_turu != :fis_turu "
                                 "AND fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi ");


    select_query.SET_VALUE(":cari_hesap_id"  , cari_hesap_id);
    select_query.SET_VALUE(":fis_turu"       , ENUM_CARI_ACILIS_FISI);
    select_query.SET_VALUE(":bas_tarihi"     , M_CARI_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    select_query.SET_VALUE(":bts_tarihi"     , M_CARI_RV->bitis_tarihi.toString("yyyy.MM.dd"));

    if ( select_query.SELECT( "fis_tarihi, fis_no" ) EQ 0 ) {
        if ( fisler.size() EQ 0 ) {
            return QStringList();
        }
    }

    while (select_query.NEXT()) {

        QString fis_bilgisi = select_query.VALUE(1).toString() +"\t" +
                              select_query.VALUE(0).toString() +"\t" +
                              select_query.VALUE(2).toString() +"\t" +
                              select_query.VALUE(3).toString() +"\t" +
                              select_query.VALUE(4).toString();

        if( fisler.contains( fis_bilgisi ) EQ false ) {
            fisler.append( fis_bilgisi );
        }
    }

    fisler.sort();

    for ( int i = 0 ; i < fisler.size() ; i++ ) {
         fis.clear();
         fis  = fisler.at(i).split("\t");

        QString sql_query = QString ( "fis_id = %1 " ).arg ( QVariant(fis.at(1)).toInt());

        if ( QVariant(fis.at(2)).toInt() EQ ENUM_CARI_HAREKET_FISI OR QVariant (fis.at (2)).toInt() EQ ENUM_CARI_VIRMAN_FORMU) {
           sql_query += QString  (" AND cari_hesap_id = %2").arg(cari_hesap_id );
        }

        select_query.PREPARE_SELECT ( "car_fis_satirlari", "aciklama,borc_tutari,alacak_tutari,cari_para_birimi_tutari", sql_query );

        double cari_borc_tutari   = 0.0;
        double cari_alacak_tutari = 0.0;

        if  ( select_query.SELECT() NE  0 ) {
              while ( select_query.NEXT()) {
               fis_satiri.clear();
               double borc_tutari             = select_query.VALUE(1).toDouble();
               double alacak_tutari           = select_query.VALUE(2).toDouble();
               double cari_para_birimi_tutari = select_query.VALUE(3).toDouble();

               cari_borc_tutari   = 0.0;
               cari_alacak_tutari = 0.0;

               if ( borc_tutari NE 0.0 ) {
                   cari_borc_tutari = cari_para_birimi_tutari;
               }
               if ( alacak_tutari NE 0.0 ) {
                   cari_alacak_tutari = cari_para_birimi_tutari;
               }

               toplam_borc   += cari_borc_tutari;
               toplam_alacak += cari_alacak_tutari;

               if ( toplam_borc > toplam_alacak ) {
                   borc_bakiye   = toplam_borc - toplam_alacak;
                   alacak_bakiye = 0.0;

               }
               else {
                   alacak_bakiye = toplam_alacak - toplam_borc;
                   borc_bakiye   = 0.0;
               }

               QString aciklama = select_query.VALUE(0).toString();

               switch( fis.at( 4 ).toInt() ) {

               case CARI_MODULU     : {
                   int cari_fis_turu = fis.at( 2 ).toInt();
                   if ( cari_fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                       aciklama.prepend( "TAHSILAT - " );
                   }
                   else if( cari_fis_turu EQ ENUM_CARI_ODEME_FISI ) {
                       aciklama.prepend( "ÖDEME - " );
                   }
                   else if ( cari_fis_turu EQ ENUM_CARI_VIRMAN_FORMU ) {
                       aciklama.prepend( "VİRMAN - " );
                   }
                   else if ( cari_fis_turu EQ ENUM_CARI_HAREKET_FISI ) {
                       aciklama.prepend( "HAREKET - " );
                   }
                   break;
               }
               case BANKA_MODULU    :
                   aciklama.prepend( "BANKA - " );
                   break;
               case FATURA_MODULU   :
                   aciklama.prepend( "FATURA - " );
                   break;
               case CEKSENET_MODULU :
                   aciklama.prepend( "CEK-SENET - " );
                   break;
               default:
                   break;
               }

               fis_satiri.append(fis.at(0) + "\t" + fis.at(3)
                          +"\t"+ aciklama
                          +"\t"+ QVariant(ROUND(cari_borc_tutari)).toString()
                          +"\t"+ QVariant(ROUND(cari_alacak_tutari)).toString()
                          +"\t"+ QVariant(ROUND(borc_bakiye)).toString()
                          +"\t"+ QVariant(ROUND(alacak_bakiye)).toString());


               fis_satirlari_list.append(fis_satiri);
            }
        }
    }

    return fis_satirlari_list;
}

/**************************************************************************************
                   CARI_HESAP_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN
***************************************************************************************/

void CARI_HESAP_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN(double toplam_borc,double toplam_alacak)
{
    double borc_bakiye   = 0.0;
    double alacak_bakiye = 0.0;

    if (toplam_borc > toplam_alacak ) {
        borc_bakiye = toplam_borc - toplam_alacak;
    }
    else {
        alacak_bakiye = toplam_alacak - toplam_borc;
    }

    QStringList nakli_yekun_satiri;

    nakli_yekun_satiri << "" << "" << "Nakli Yekün "
            << VIRGUL_EKLE(QVariant(ROUND(toplam_borc)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(toplam_alacak)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(borc_bakiye)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(alacak_bakiye)).toString());

    m_gonderilen_kayit_sayisi++;

    ADD_ROW_TO_TABLE(nakli_yekun_satiri);
}
