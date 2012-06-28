#include "adak_utils.h"
#include "stok_ekstresi_raporu_class.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_struct.h"
#include "yonetim.h"
#include "stok_enum.h"
#include "sube_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "fat_fis_utils.h"
#include "stok_console_utils.h"
#include "irs_enum.h"



extern ADAK_SQL *         DB;

/**************************************************************************************
                   GET_STOK_EKSTRESI_RAPORU
***************************************************************************************/

QString GET_STOK_EKSTRESI_RAPORU (STOK_RAPOR_VARS * rapor_vars)
{
    STOK_EKSTRESI_RAPORU  *  F = new STOK_EKSTRESI_RAPORU( "Stok Ekstresi Raporu ",rapor_vars);
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   STOK_EKSTRESI_RAPORU::STOK_EKSTRESI_RAPORU
***************************************************************************************/

STOK_EKSTRESI_RAPORU::STOK_EKSTRESI_RAPORU( QString rapor_ismi, STOK_RAPOR_VARS * rapor_vars ) : REPORT_KERNEL (rapor_ismi)
{
    M_STOK_RV = rapor_vars;

    SET_HEIGHTS ( 30, 30 );

    m_max_line_count              =  GET_MAX_LINE_COUNT ( 14 );
    m_last_satir_indisi           = 0;
    m_sayfaya_yazdirilan_satir_sayisi = 0;

    m_footer_toplam_cikan_miktar  = 0.0;
    m_footer_toplam_giren_miktar  = 0.0;
    m_footer_kalan_miktar         = 0.0;
    m_nakli_yekun_kalan_miktar    = 0.0;

    m_tum_rapor_satirlari_basildi = false;
    m_urun_islemleri_basildi      = false;

    m_baslangic_tarihi           = M_STOK_RV->baslangic_tarihi;
    m_bitis_tarihi               = M_STOK_RV->bitis_tarihi;
    m_depo_id                    = M_STOK_RV->depo_id;


    if ( M_STOK_RV->tek_urun_kodu.isEmpty() EQ true ) {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("stk_urunler",
                             "urun_adi",
                             "urun_kodu = :urun_kodu");
        query.SET_VALUE(":urun_kodu" , M_STOK_RV->bas_urun_kodu);

        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            m_rapor_kriterleri_1.append(M_STOK_RV->bas_urun_kodu + "  " + query.VALUE(0).toString());
        }

        query.PREPARE_SELECT("stk_urunler",
                             "urun_adi",
                             "urun_kodu = :urun_kodu");
        query.SET_VALUE(":urun_kodu" , M_STOK_RV->bts_urun_kodu);

        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            m_rapor_kriterleri_1.append(" - " + M_STOK_RV->bts_urun_kodu + " " + query.VALUE(0).toString());
        }
        m_rapor_kriterleri_1.append(ADD_HTML_SPACE("  ") + "Aralığındaki Ürünler");
    }
    if ( M_STOK_RV->nakli_yekun EQ true ) {
        m_rapor_kriterleri_2.append(" Nakli Yekün - ");
        m_nakli_yekun = true;
    }
    else {
        m_nakli_yekun = false;
    }
    QString depo_kodu;
    QString depo_adi;

    SUBE_GET_DEPO_KODU_ADI(m_depo_id, &depo_kodu , &depo_adi);
    m_rapor_kriterleri_2.append(" Depo Kodu / Adı : " + depo_kodu + "  " + depo_adi);

    FILL_RAPOR_SATIRLARI();
}

/**************************************************************************************
                   STOK_EKSTRESI_RAPORU::GET_HEADER
***************************************************************************************/

QString STOK_EKSTRESI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=LEFT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()<< "" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>STOK EKSTRESİ" << "");

    ADD_ROW_TO_TABLE     ( QStringList() << "" << m_baslangic_tarihi.toString("dd MMMM yyyy dddd") + " - "
                           + m_bitis_tarihi.toString("dd MMMM yyyy dddd") << "");

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "" << m_rapor_kriterleri_1);

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "" << m_rapor_kriterleri_2);


   return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   STOK_EKSTRESI_RAPORU::GET_BODY
***************************************************************************************/

QString STOK_EKSTRESI_RAPORU::GET_BODY()
{
    QString         urun_kodu_adi;
    QStringList     RAPOR_SATIRI;


    int current_urun_id           = 0;
    int urun_id                   = 0;
    double satirdaki_kalan_miktar = 0.0;

    if ( m_tum_rapor_satirlari_basildi EQ true ) {
        return NULL;
    }
    if ( m_urun_islemleri_basildi EQ true ) {
        m_urun_islemleri_basildi = false;
        m_footer_kalan_miktar        = 0.0;
        m_footer_toplam_cikan_miktar = 0.0;
        m_footer_toplam_giren_miktar = 0.0;
        m_nakli_yekun_kalan_miktar   = 0.0;
    }

    m_sayfaya_yazdirilan_satir_sayisi = 0;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER =1 >" );


    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=24%"
                                         << "WIDTH=6%"
                                         << "WIDTH=15%"
                                         << "WIDTH=14%"
                                         << "WIDTH=10%"
                                         << "WIDTH=10%"
                                         << "WIDTH=10%"
                                         << "WIDTH=10%"
                                         << "WIDTH=10%" );

    CREATE_TABLE         ( QStringList()<< "<b>Fiş Tarihi" << "<b>Fiş No" << "<b> Cari Hesap" << "<b>Açıklama"
                           << "<b>G. Miktar" << "<b>Ç. Miktar"<< "<b>K. Miktar" << "<b>A/S Fiyatı" << "<b>Tutar"  ,1,13);

    SET_TD_TAG_STRING(QStringList() << "" << "" << "" << "" << "ALIGN=RIGHT" << "ALIGN=RIGHT" << "ALIGN=RIGHT" <<"ALIGN=RIGHT" <<"ALIGN=RIGHT" );



    for ( ; ; ) {

        if ( m_last_satir_indisi EQ M_RAPOR_SATIRLARI.size() ) {
            m_tum_rapor_satirlari_basildi = true;
            break;
        }

        RAPOR_SATIRI = M_RAPOR_SATIRLARI.at(m_last_satir_indisi).split("\t");

        urun_id = QVariant(RAPOR_SATIRI.at(0)).toInt();

        if ( current_urun_id EQ 0 ) {
            current_urun_id = urun_id;
        }
        if ( urun_id NE current_urun_id ) {
            current_urun_id = urun_id;
            m_urun_islemleri_basildi = true;

            break;
        }
        if ( m_nakli_yekun EQ true ) {
            if ( m_sayfaya_yazdirilan_satir_sayisi EQ 0 ) {
                PRINT_NAKLI_YEKUN(urun_id);
            }
        }

        if ( M_STOK_RV->stok_alis_satis_durumu EQ ENUM_STOK_ALIS_VE_SATIS ) {
            satirdaki_kalan_miktar = QVariant ( RAPOR_SATIRI.at(8) ).toDouble() + m_nakli_yekun_kalan_miktar;
        }
        else {
            satirdaki_kalan_miktar = 0;
        }

        urun_kodu_adi = RAPOR_SATIRI.at(1);

        if ( QVariant(RAPOR_SATIRI.at(3)).toInt() NE 0 ) {

            ADD_ROW_TO_TABLE(QStringList() << RAPOR_SATIRI.at(2) << RAPOR_SATIRI.at(3) << RAPOR_SATIRI.at(4) << RAPOR_SATIRI.at( 5).mid( 0, 15 )
                                 << RAPOR_SATIRI.at(6) << RAPOR_SATIRI.at(7) << QVariant(satirdaki_kalan_miktar).toString()
                                 << RAPOR_SATIRI.at(9) << RAPOR_SATIRI.at(10)    );

            m_sayfaya_yazdirilan_satir_sayisi++;
        }
        m_footer_toplam_giren_miktar += QVariant(RAPOR_SATIRI.at(6)).toDouble();
        m_footer_toplam_cikan_miktar += QVariant(RAPOR_SATIRI.at(7)).toDouble();
        m_footer_kalan_miktar         = satirdaki_kalan_miktar;

        m_last_satir_indisi++;


        if ( m_sayfaya_yazdirilan_satir_sayisi EQ m_max_line_count ) {
            break;
        }
    }
    return urun_kodu_adi.append(GET_TABLE_HTML_STRING());
}

/**************************************************************************************
                   STOK_EKSTRESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString STOK_EKSTRESI_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING("<TABLE style=\"width:100%; font-size:75%; font-weight:bold\" BORDER=0>");
    SET_TD_TAG_STRING(QStringList() << "style=\"width:60%;\""<< "style=\"width:10%;\" ALIGN=RIGHT"
                                    << "style=\"width:10%;\" ALIGN=RIGHT" << "style=\"width:10%;\" ALIGN=RIGHT"
                                    << "style=\"width:10%;\" ALIGN=RIGHT" << "style=\"width:10%;\" ALIGN=RIGHT" );

    CREATE_TABLE (QStringList()<< "" << "" << "" << "" << "" << "" ,0,0);

    ADD_ROW_TO_TABLE(QStringList() << "Toplam" << QVariant(m_footer_toplam_giren_miktar).toString()
                     << QVariant(m_footer_toplam_cikan_miktar).toString() << QVariant( m_footer_kalan_miktar ).toString()  << "" << ""  << "" );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   STOK_EKSTRESI_RAPORU::GET_URUN_LISTESI()
***************************************************************************************/

void STOK_EKSTRESI_RAPORU::FILL_RAPOR_SATIRLARI()
{
    SQL_QUERY       query(DB);
    QString         urun_kodu_adi;
    int             urun_id;

    query.PREPARE_SELECT( "stk_urunler", "urun_id, urun_kodu, urun_adi" );


    if ( M_STOK_RV->tek_urun_kodu.isEmpty() EQ false ) {

        query.AND_EKLE  ( "urun_kodu = :urun_kodu OR barkod_numarasi = :barkod_numarasi " );
        query.SET_VALUE ( ":urun_kodu", M_STOK_RV->tek_urun_kodu );
        query.SET_VALUE ( ":barkod_numarasi", M_STOK_RV->tek_urun_kodu );

    }
    else {

        query.AND_EKLE  ( " (urun_kodu BETWEEN :urun_kodu_1 AND :urun_kodu_2 ) "
                          "OR ( barkod_numarasi BETWEEN  :barkod_numarasi_1 AND :barkod_numarasi_2 ) " );

        query.SET_VALUE( ":urun_kodu_1",        M_STOK_RV->bas_urun_kodu );
        query.SET_VALUE( ":urun_kodu_2",        M_STOK_RV->bts_urun_kodu );
        query.SET_VALUE( ":barkod_numarasi_1",  M_STOK_RV->bas_urun_kodu );
        query.SET_VALUE( ":barkod_numarasi_2",  M_STOK_RV->bts_urun_kodu );

    }

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    while ( query.NEXT() ) {

        urun_id       = query.VALUE(0).toInt();
        urun_kodu_adi = "<p align=center style = \"font-size:85%; font-weight:bold;\">" +
                        query.VALUE(1).toString() + ADD_HTML_SPACE("    ") + query.VALUE(2).toString() + "</p>";

        FILL_URUN_ISLEMLERI(urun_id , urun_kodu_adi);

    }
}

/**************************************************************************************
                   STOK_EKSTRESI_RAPORU::FILL_URUN_ISLEMLERI()
***************************************************************************************/

void STOK_EKSTRESI_RAPORU::FILL_URUN_ISLEMLERI(int urun_id ,QString urun_kodu_adi)
{
    SQL_QUERY       query(DB);
    QString         where_str;
    QStringList     ISLEM;

    double giren_miktar = 0.0;
    double cikan_miktar = 0.0;
    double kalan_miktar = 0.0;
    double satirdaki_urun_miktari;

    int     stok_etkisi = 0;

    where_str  = QString ("stk_fisler.fis_id = stk_fis_satirlari.fis_id AND urun_id = :arg_0 ");
    where_str += QString ("AND fis_tarihi BETWEEN :arg_1 AND :arg_2 ");
    where_str += QString ("AND depo_id = :arg_3 ");

    query.PREPARE_SELECT( "stk_fisler,stk_fis_satirlari",
                          "fis_turu ,fis_tarihi,fis_no,aciklama,satirdaki_urun_miktari ,     "
                          "t_is_son_birim_fiyati, t_is_son_satir_tutari, stk_fisler.fis_id , "
                          "fis_satiri_id , fis_tarihi, iade_irsaliyesi_mi, cari_hesap_ismi   ",
                           where_str );

    query.SET_VALUE     ( ":arg_0", urun_id );
    query.SET_VALUE     ( ":arg_1", m_baslangic_tarihi.toString("yyyy.MM.dd") );
    query.SET_VALUE     ( ":arg_2", m_bitis_tarihi.toString("yyyy.MM.dd") );
    query.SET_VALUE     ( ":arg_3", m_depo_id );

    if ( M_STOK_RV->stok_alis_satis_durumu NE ENUM_STOK_ALIS_VE_SATIS ) {
        query.AND_EKLE  ( "irsaliye_alis_satis_turu = :irsaliye_alis_satis_turu " );
        query.SET_VALUE ( ":irsaliye_alis_satis_turu", M_STOK_RV->stok_alis_satis_durumu );
    }

    if ( query.SELECT( "fis_tarihi,fis_no" ) EQ 0 ) {
        return ;
    }

    while ( query.NEXT() ) {
        int fis_turu = query.VALUE(0).toInt();
        giren_miktar = 0.0;
        cikan_miktar = 0.0;
        satirdaki_urun_miktari = query.VALUE(4).toDouble();



        switch ( fis_turu ) {
            case ENUM_STK_SARF_FIRE_FISI                        :
            case ENUM_STK_IMALATA_SEVK_FISI                     :
            case ENUM_IRS_NORMAL_CIKIS                          :
            case ENUM_STK_DEPO_TRANSFER_CIKIS                   :
            case ENUM_IRS_FASON_BASLANGIC_CIKIS                 :
            case ENUM_IRS_FASON_BITIS_CIKIS                     :
            case ENUM_IRS_KONSINYE_CIKIS                        :
            case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS     :
            case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS    :
            case ENUM_IRS_HAKEDIS_SATIS    :

                stok_etkisi = ENUM_AZALT;
                break;

            case ENUM_STK_IMALATTAN_DONUS_FISI                  :
            case ENUM_IRS_NORMAL_GIRIS                          :
            case ENUM_STK_DEPO_TRANSFER_GIRIS                   :
            case ENUM_IRS_FASON_BASLANGIC_GIRIS                 :
            case ENUM_IRS_FASON_BITIS_GIRIS                     :
            case ENUM_IRS_KONSINYE_GIRIS                        :
            case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS      :
            case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS     :
            case ENUM_IRS_HAKEDIS_ALIS     :

                stok_etkisi = ENUM_ARTTIR;
                break;
            }


        if ( fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
            if ( m_nakli_yekun EQ true ) {
                if ( query.NUM_OF_ROWS() EQ 1 ) {
                    M_RAPOR_SATIRLARI << QVariant(urun_id).toString()+"\t"+ urun_kodu_adi + "\t"+ "" + "\t" + "" + "\t"
                            +"" + "\t" + "" + "\t" + "" + "\t" + "" + "\t" + "" + "\t" + "";
                    return;
                }
            }
            continue;
        }
        if ( satirdaki_urun_miktari EQ 0 ) {
            continue;
        }

        int     irsaliye_satiri_id  = query.VALUE( "fis_satiri_id" ).toInt();
        double  satir_tutari        = ROUND(query.VALUE( "t_is_son_satir_tutari").toDouble());
        QDate   irsaliye_tarihi     = QDate::fromString( query.VALUE( "fis_tarihi").toString(), "yyyy.MM.dd" );
        int     fis_no              = query.VALUE( "fis_no").toInt();
        int     iade_irsaliyesi_mi  = query.VALUE( "iade_irsaliyesi_mi").toInt();
        QString cari_hesap_ismi     = query.VALUE( "cari_hesap_ismi" ).toString();
        QString aciklama            = query.VALUE( "aciklama" ).toString();

        // sag ve soldaki bosluklar siliniyor.
        aciklama = aciklama.trimmed();

        // Fis aciklama bos ise yerine fis turu gosteriliyor...
        if ( aciklama.isEmpty() EQ true ) {

            if( iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                if( fis_turu EQ ENUM_IRS_NORMAL_GIRIS) {
                    aciklama = "SATIŞTAN IADE İRSALİYESİ ";
                }
                else { //ENUM_SATIS_IRSALIYESI
                    aciklama = "ALIŞTAN IADE İRSALİYESİ ";
                }
            }
            else {
                aciklama = STK_GET_STOK_FIS_TURU( fis_turu );
            }
        }

        double  birim_fiyat         = 0.00;
        double  toplam_satir_tutar  = 0.00;

        int     faturalanan_urun_miktari = 0;

        int     toplam_fat_urun_miktari  = 0;
        // irsaliyede fiyat yok ise faturalar bakilacak
        if ( satir_tutari EQ 0.00 ) {
            SQL_QUERY s_query ( DB );

            // [ Otomatik Irsaliye Faturalastirma ]
            // Otomatik Faturalastirmada gerek yok eger satir eklendiginde satir tablasunada kayit ekliyor.
            // [ Elle Irsaliye Faturalastirma ]
            s_query.PREPARE_SELECT( "fat_irs_satir_baglantisi","fatura_satiri_id",
                                    "irsaliye_satiri_id = :irsaliye_satiri_id   ");
            s_query.SET_VALUE     ( ":irsaliye_satiri_id", irsaliye_satiri_id );

            if ( s_query.SELECT() NE 0 ) {

                while ( s_query.NEXT() EQ true ) {
                    faturalanan_urun_miktari    = 0;
                    int fatura_satiri_id  = s_query.VALUE( 0 ).toInt();

                    FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;
                    FATURA_CLEAR_FIS_SATIRI_STRUCT ( &FAT_FIS_SATIRI );

                    int result = FATURA_SATIR_BILGILERINI_OKU( &FAT_FIS_SATIRI, fatura_satiri_id );

                    if ( result NE 0 ) {
                        faturalanan_urun_miktari    = FAT_FIS_SATIRI.satirdaki_urun_miktari;
                        birim_fiyat                 = FAT_FIS_SATIRI.isk_sonrasi_urun_birim_fiyati;
                        toplam_satir_tutar          = FAT_FIS_SATIRI.satir_tutari;

                        if ( stok_etkisi EQ ENUM_ARTTIR ) {
                            giren_miktar = faturalanan_urun_miktari;
                            kalan_miktar += giren_miktar;
                        }
                        else if ( stok_etkisi EQ ENUM_AZALT ) {
                            cikan_miktar = faturalanan_urun_miktari;
                            kalan_miktar -= cikan_miktar;
                        }

                        ISLEM.clear();
                        ISLEM.append(QVariant(urun_id).toString() +
                                     "\t" + urun_kodu_adi +
                                     "\t" + irsaliye_tarihi.toString( "dd MMMM yyyy dddd" ) +
                                     "\t" + QVariant ( fis_no ).toString() +
                                     "\t" + cari_hesap_ismi +
                                     "\t" + aciklama +
                                     "\t" + QVariant(giren_miktar).toString() +
                                     "\t" + QVariant(cikan_miktar).toString() +
                                     "\t" + QVariant(kalan_miktar).toString() +
                                     "\t" + VIRGUL_EKLE( QVariant( birim_fiyat ).toString(), 2 ) +
                                     "\t" + VIRGUL_EKLE( QVariant( toplam_satir_tutar ).toString(), 2 ));

                        M_RAPOR_SATIRLARI << ISLEM;

                        toplam_fat_urun_miktari += faturalanan_urun_miktari;
                    }
                }

                if ( satirdaki_urun_miktari NE toplam_fat_urun_miktari ) {

                    if ( stok_etkisi EQ ENUM_ARTTIR ) {
                        giren_miktar = satirdaki_urun_miktari - toplam_fat_urun_miktari;
                        kalan_miktar += giren_miktar;
                    }
                    else if ( stok_etkisi EQ ENUM_AZALT ) {
                        cikan_miktar = satirdaki_urun_miktari - toplam_fat_urun_miktari;
                        kalan_miktar -= cikan_miktar;
                    }

                    ISLEM.clear();
                    ISLEM.append(QVariant(urun_id).toString() +
                                 "\t" + urun_kodu_adi +
                                 "\t" + irsaliye_tarihi.toString( "dd MMMM yyyy dddd" ) +
                                 "\t" + QVariant ( fis_no ).toString() +
                                 "\t" + cari_hesap_ismi +
                                 "\t" + aciklama +
                                 "\t" + QVariant( giren_miktar ).toString() +
                                 "\t" + QVariant( cikan_miktar ).toString() +
                                 "\t" + QVariant(kalan_miktar).toString() +
                                 "\t" + VIRGUL_EKLE( QVariant( 0 ).toString(), 2 ) +
                                 "\t" + VIRGUL_EKLE( QVariant( 0 ).toString(), 2 ));

                    M_RAPOR_SATIRLARI << ISLEM;
                }

            }
            else { // irsaliyede fiyat girilmese bile ekstrede gorunmeli

                if ( stok_etkisi EQ ENUM_ARTTIR ) {
                    giren_miktar = satirdaki_urun_miktari;
                    kalan_miktar += giren_miktar;
                }
                else if ( stok_etkisi EQ ENUM_AZALT ) {
                    cikan_miktar = satirdaki_urun_miktari;
                    kalan_miktar -= cikan_miktar;
                }

                toplam_satir_tutar  = satir_tutari;
                ISLEM.clear();
                ISLEM.append(QVariant(urun_id).toString() +
                             "\t" + urun_kodu_adi +
                             "\t" + query.VALUE(1).toDate().toString("dd MMMM yyyy dddd") +
                             "\t" + query.VALUE(2).toString() +
                             "\t" + cari_hesap_ismi +
                             "\t" + aciklama +
                             "\t" + QVariant(giren_miktar).toString() +
                             "\t" + QVariant(cikan_miktar).toString() +
                             "\t" + QVariant(kalan_miktar).toString() +
                             "\t" + VIRGUL_EKLE( QVariant(ROUND( query.VALUE("t_is_son_birim_fiyati").toDouble() )).toString(), 2 ) +
                             "\t" + VIRGUL_EKLE( QVariant( toplam_satir_tutar ).toString(), 2 ) );

                M_RAPOR_SATIRLARI << ISLEM;

            }
            // [/Elle Irsaliye Faturalastirma ]
        }
        else {
            if ( stok_etkisi EQ ENUM_ARTTIR ) {
                giren_miktar = satirdaki_urun_miktari;
                kalan_miktar += giren_miktar;
            }
            else if ( stok_etkisi EQ ENUM_AZALT ) {
                cikan_miktar = satirdaki_urun_miktari;
                kalan_miktar -= cikan_miktar;
            }

            toplam_satir_tutar  = satir_tutari;
            ISLEM.clear();
            ISLEM.append(QVariant(urun_id).toString() +
                         "\t" + urun_kodu_adi +
                         "\t" + query.VALUE(1).toDate().toString("dd MMMM yyyy dddd") +
                         "\t" + query.VALUE(2).toString() +
                         "\t" + cari_hesap_ismi +
                         "\t" + aciklama +
                         "\t" + QVariant(giren_miktar).toString() +
                         "\t" + QVariant(cikan_miktar).toString() +
                         "\t" + QVariant(kalan_miktar).toString() +
                         "\t" + VIRGUL_EKLE( QVariant(ROUND( query.VALUE("t_is_son_birim_fiyati").toDouble() )).toString(), 2 )  +
                         "\t" + VIRGUL_EKLE( QVariant( toplam_satir_tutar ).toString(), 2 ) );

            M_RAPOR_SATIRLARI << ISLEM;
        }
    }

}

/**************************************************************************************
                   STOK_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN();
***************************************************************************************/

void STOK_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN(int urun_id)
{
    QString ent_filtre_tables;

    ent_filtre_tables = "toplam_depoya_giren_array,toplam_depodan_cikan_array";

    QString aciklama = "Nakli Yekün";

    int gun_no = MALI_YIL_ARRAY_INDIS(m_baslangic_tarihi);

    if ( gun_no EQ 1 ) {
        int muhasebe_yili = MALI_YIL_FIRST_DATE().year();
        if ( muhasebe_yili EQ m_baslangic_tarihi.year() ) {
            aciklama = QObject::tr ( "AÇILIŞ FİŞİ" );
        }
    }

    SQL_QUERY query(DB);

    query.PREPARE_SELECT( "stk_depodaki_urunler", QString( "%1").arg( ent_filtre_tables ), "depo_id = :depo_id AND urun_id = :urun_id" );
    query.SET_VALUE(":depo_id" , m_depo_id);
    query.SET_VALUE(":urun_id" , urun_id  );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    double depoya_giren_urun_miktari[E9_ARRAY_SIZE];
    double depodan_cikan_urun_miktari[E9_ARRAY_SIZE];
    double kalan_miktar;

    UNPACK_DOUBLE_ARRAY(query.VALUE(0).toString() , depoya_giren_urun_miktari  , E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY(query.VALUE(1).toString() , depodan_cikan_urun_miktari , E9_ARRAY_SIZE);

    kalan_miktar = depoya_giren_urun_miktari[gun_no-1] - depodan_cikan_urun_miktari[gun_no-1];

    ADD_ROW_TO_TABLE(QStringList() << "" << "" << "" << aciklama << QVariant(depoya_giren_urun_miktari[gun_no-1]).toString()
                     << QVariant(depodan_cikan_urun_miktari[gun_no - 1]).toString() << QVariant(kalan_miktar).toString() << "" << "");


    m_sayfaya_yazdirilan_satir_sayisi++;


    m_footer_toplam_giren_miktar = depoya_giren_urun_miktari[gun_no-1];
    m_footer_toplam_cikan_miktar = depodan_cikan_urun_miktari[gun_no-1];
    m_nakli_yekun_kalan_miktar   = kalan_miktar;

}








