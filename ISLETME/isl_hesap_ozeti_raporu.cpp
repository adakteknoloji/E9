#include "adak_utils.h"
#include "isl_hesap_ozeti_raporu_open.h"
#include "isl_hesap_ozeti_raporu_class.h"
#include "isl_struct.h"
#include "yonetim.h"
#include "e9_console_utils.h"
#include "isl_enum.h"

extern ADAK_SQL *         DB;


/**************************************************************************************
                   GET_ISL_HESAP_OZETI_RAPORU
***************************************************************************************/

QString GET_ISL_HESAP_OZETI_RAPORU ( ISL_RAPOR_VARS * P_ISL_RV)
{
    ISL_HESAP_OZETI_RAPORU  *  F = new ISL_HESAP_OZETI_RAPORU( "İşletme Hesabı Özeti" , P_ISL_RV);
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   ISL_HESAP_OZETI_RAPORU::ISL_HESAP_OZETI_RAPORU
***************************************************************************************/

ISL_HESAP_OZETI_RAPORU::ISL_HESAP_OZETI_RAPORU( QString rapor_ismi , ISL_RAPOR_VARS * P_ISL_RV ) : REPORT_KERNEL (rapor_ismi)
{
    SET_HEIGHTS ( 30, 30 );

    m_max_line_count  =  GET_MAX_LINE_COUNT ( 14 );

    m_rapor_yazdirildi_mi = false;

    M_ISL_RV  = P_ISL_RV;

    m_genel_gider_tutari                    = 0.00;
    m_gider_mal_alimi_tutari                = 0.00;
    m_gider_donem_basi_stogu_tutari         = 0.00;
    m_gider_isyeri_kirasi_tutari            = 0.00;
    m_gider_isyeri_sigorta_gideri_tutari    = 0.00;
    m_gider_isyeri_personel_gideri_tutari   = 0.00;
    m_gider_isyeri_faiz_gideri_tutari       = 0.00;
    m_gider_mal_alis_gideri_tutari          = 0.00;
    m_gider_yil_sonu_amortismanlar_tutari   = 0.00;
    m_gider_indirilecek_kdv_tutari          = 0.00;
    m_toplam_gider                          = 0.00;

    m_gelir_mal_satisi_tutari               = 0.00;
    m_gelir_hizmet_satisi_tutari            = 0.00;
    m_gelir_amortisman_satisi_tutari        = 0.00;
    m_gelir_faiz_geliri_tutari              = 0.00;
    m_gelir_yil_sonu_stogu_tutari           = 0.00;
    m_gelir_hesaplanan_kdv_tutari           = 0.00;
    m_toplam_gelir                          = 0.00;

    GET_TUTARLAR();
}

/**************************************************************************************
                   ISL_HESAP_OZETI_RAPORU::GET_HEADER
***************************************************************************************/

QString ISL_HESAP_OZETI_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    QString donem_araligi = M_ISL_RV->baslangic_tarihi.toString("dd MMMM yyyy")
            + " - " + M_ISL_RV->bitis_tarihi.toString("dd MMMM yyyy");

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()<< "" << GET_REPORT_TOP_RIGHT_HEADER() );

    ADD_ROW_TO_TABLE     ( QStringList() << "" << "<b>İŞLETME HESABI ÖZETİ" << "");

    ADD_ROW_TO_TABLE     ( QStringList() << "" << donem_araligi << "");

    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   ISL_HESAP_OZETI_RAPORU::GET_BODY
***************************************************************************************/

QString ISL_HESAP_OZETI_RAPORU::GET_BODY()
{

    if ( m_rapor_yazdirildi_mi EQ true ) {
        return NULL;
    }

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% style=\"font-size:75%;\" BORDER = 1 >" );

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH=40% " << "WIDTH=10% " << "WIDTH=40% " << "WIDTH=10%");
    CREATE_TABLE         ( QStringList()<< "<b>GİDER" << "" << "<b>GELİR" << "" ,1,13);
    SET_TD_TAG_STRING    ( QStringList() << "ALIGN=LEFT" << "ALIGN=RIGHT" << "ALIGN=LEFT" << "ALIGN=RIGHT");

    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>Dönem Başı Mal Mevcudu")     << VIRGUL_EKLE( QVariant(m_gider_donem_basi_stogu_tutari).toString() ) << QObject::tr("<b>Dönem İçi Mal Satışı") << VIRGUL_EKLE( QVariant(m_gelir_mal_satisi_tutari).toString() ));
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>Dönem İçi Gider")            << VIRGUL_EKLE( QVariant(m_genel_gider_tutari).toString() ) << QObject::tr("<b>Dönem İçi Hizmet Satışı") << VIRGUL_EKLE( QVariant(m_gelir_hizmet_satisi_tutari).toString() ));
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>Dönem İçi Alınan Emtia")     << VIRGUL_EKLE( QVariant(m_gider_mal_alimi_tutari).toString() ) << QObject::tr("<b>Dönem İçi Amortisman Satışı") << VIRGUL_EKLE( QVariant(m_gelir_amortisman_satisi_tutari).toString() ));
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>İşyeri Kirasi")              << VIRGUL_EKLE( QVariant(m_gider_isyeri_kirasi_tutari).toString() ) << QObject::tr("<b>Dönem İçi Faiz Geliri") << VIRGUL_EKLE( QVariant(m_gelir_faiz_geliri_tutari).toString() ));
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>İşyeri Sigorta Gideri")      << VIRGUL_EKLE( QVariant(m_gider_isyeri_sigorta_gideri_tutari).toString() ) << QObject::tr("<b>Dönem İçi Hesaplanan KDV") << VIRGUL_EKLE( QVariant(m_gelir_hesaplanan_kdv_tutari).toString() ));
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>İşyeri Persosnel Gideri")    << VIRGUL_EKLE( QVariant(m_gider_isyeri_personel_gideri_tutari).toString() ) << QObject::tr("<b>Dönem Sonu Stoğu") << VIRGUL_EKLE( QVariant(m_gelir_yil_sonu_stogu_tutari).toString() ));
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>İşyeri Faiz Gideri")         << VIRGUL_EKLE( QVariant(m_gider_isyeri_faiz_gideri_tutari).toString() ) << QObject::tr("") << "");
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>Dönem Sonu Amortismanlar")   << VIRGUL_EKLE( QVariant(m_gider_yil_sonu_amortismanlar_tutari).toString() ) << QObject::tr("") << "");
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>Dönem İçş İndirilecek KDV")  << VIRGUL_EKLE( QVariant(m_gider_indirilecek_kdv_tutari).toString() ) << QObject::tr("") << "");

    ADD_ROW_TO_TABLE(QStringList() << ADD_HTML_SPACE("    ") << ""  << QObject::tr("") << "");
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>Ara Toplam") << VIRGUL_EKLE( QVariant(m_toplam_gider).toString() ) << QObject::tr("<b>Ara Toplam") << VIRGUL_EKLE( QVariant(m_toplam_gelir).toString() ));


    if ( m_toplam_gelir > m_toplam_gider ) {
        double kar = ROUND( m_toplam_gelir - m_toplam_gider );
        m_toplam_gider = ROUND( m_toplam_gider + kar );
        ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>Dönem Karı") << VIRGUL_EKLE( QVariant(kar).toString() ) << QObject::tr("<b>Dönem Zararı") << "");
    }
    else {
        double zarar = ROUND( m_toplam_gider - m_toplam_gelir );
        m_toplam_gelir = ROUND( m_toplam_gelir + zarar );
        ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>Dönem Karı") << "" << QObject::tr("<b>Dönem Zararı") << VIRGUL_EKLE( QVariant(zarar).toString() ));
    }

    ADD_ROW_TO_TABLE(QStringList() << ADD_HTML_SPACE("    ") << "" << QObject::tr("") << "");
    ADD_ROW_TO_TABLE(QStringList() << QObject::tr("<b>GENEL TOPLAM") << VIRGUL_EKLE( QVariant(m_toplam_gelir).toString() ) << QObject::tr("<b>GENEL TOPLAM") << VIRGUL_EKLE( QVariant(m_toplam_gider).toString() ));

    m_rapor_yazdirildi_mi = true;

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   ISL_HESAP_OZETI_RAPORU::GET_FOOTER
***************************************************************************************/

QString ISL_HESAP_OZETI_RAPORU::GET_FOOTER()
{
    return NULL;
}

/**************************************************************************************
                   ISL_HESAP_OZETI_RAPORU::GET_TUTARLAR
***************************************************************************************/

void ISL_HESAP_OZETI_RAPORU::GET_TUTARLAR()
{
    SQL_QUERY f_query(DB);
    SQL_QUERY s_query(DB);

    f_query.PREPARE_SELECT("isl_fisi", "fis_id, fis_turu", "fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi");
    f_query.SET_VALUE(":bas_tarihi"  , M_ISL_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    f_query.SET_VALUE(":bts_tarihi"  , M_ISL_RV->bitis_tarihi.toString("yyyy.MM.dd"));

    if ( f_query.SELECT() NE 0 ) {

        while( f_query.NEXT() EQ true ) {
            int fis_id   = f_query.VALUE(0).toInt();
            int fis_turu = f_query.VALUE(1).toInt();

            s_query.PREPARE_SELECT("isl_fis_satirlari", "gider_turu, satir_kdv_haric_tutar, satir_kdv_tutari ", "fis_id = :fis_id");
            s_query.SET_VALUE     ( ":fis_id", fis_id );

            if( s_query.SELECT() NE 0 ) {
                while( s_query.NEXT() EQ true ) {

                    int    islem_turu       = s_query.VALUE("gider_turu").toInt();
                    double islem_tutari     = s_query.VALUE("satir_kdv_haric_tutar").toDouble();
                    double islem_kdv_tutari = s_query.VALUE("satir_kdv_tutari").toDouble();

                    if ( fis_turu EQ ENUM_GIDER_FISI ) {

                        m_gider_indirilecek_kdv_tutari += islem_kdv_tutari;

                        switch(islem_turu) {
                            case ENUM_GENEL_GIDERLER         :
                                m_genel_gider_tutari += islem_tutari;
                                break;
                            case ENUM_MAL_ALIMI              :
                                m_gider_mal_alimi_tutari += islem_tutari;
                                break;
                            case ENUM_DONEM_BASI_STOGU       :
                                m_gider_donem_basi_stogu_tutari += islem_tutari;
                                break;
                            case ENUM_ISYERI_KIRASI          :
                                m_gider_isyeri_kirasi_tutari += islem_tutari;
                                break;
                            case ENUM_ISYERI_SIGORTA_GIDERI  :
                                m_gider_isyeri_sigorta_gideri_tutari += islem_tutari;
                                break;
                            case ENUM_ISYERI_PERSONEL_GIDERI :
                                m_gider_isyeri_personel_gideri_tutari += islem_tutari;
                                break;
                            case ENUM_ISYERI_FAIZ_GIDERI     :
                                m_gider_isyeri_faiz_gideri_tutari += islem_tutari;
                                break;
                            case ENUM_YILSONU_AMORTISMANLAR  :
                                m_gider_yil_sonu_amortismanlar_tutari += islem_tutari;
                                break;
                            default:
                                break;
                        }

                        m_toplam_gider += islem_tutari;
                        m_toplam_gider += islem_kdv_tutari;

                    }
                    else { // ENUM_GIDER_FISI

                        m_gelir_hesaplanan_kdv_tutari += islem_kdv_tutari;

                        switch(islem_turu) {
                            case ENUM_MAL_SATISI        :
                                m_gelir_mal_satisi_tutari += islem_tutari;
                                break;
                            case ENUM_HIZMET_SATISI     :
                                m_gelir_hizmet_satisi_tutari += islem_tutari;
                                break;
                            case ENUM_AMORTISMAN_SATISI :
                                m_gelir_amortisman_satisi_tutari += islem_tutari;
                                break;
                            case ENUM_FAIZ_GELIRI       :
                                m_gelir_faiz_geliri_tutari += islem_tutari;
                                break;
                            case ENUM_YILSONU_STOGU     :
                                m_gelir_yil_sonu_stogu_tutari += islem_tutari;
                                break;
                            default:
                                break;
                        }

                        m_toplam_gelir += islem_tutari ;
                        m_toplam_gelir += islem_kdv_tutari ;
                    }
                }
            }
        }
    }
}
