#include "adak_utils.h"
#include "bnk_kk_ekstresi_raporu_class.h"
#include "bnk_kk_ekstresi_raporu_open.h"
#include "e9_console_utils.h"
#include "e9_enum.h"
#include "yonetim.h"
#include "banka_struct.h"
#include "banka_enum.h"
#include "bnk_console_utils.h"

extern ADAK_SQL *         DB;

/**************************************************************************************
                   GET_BNK_KK_EKSTRESI_RAPORU
***************************************************************************************/

QString GET_BNK_KK_EKSTRESI_RAPORU ( BNK_RAPOR_VARS * BNK_RV )
{
    BNK_KK_EKSTRESI_RAPORU  *  F = new BNK_KK_EKSTRESI_RAPORU( BNK_RV, "Banka Kredi Karti Ekstresi");
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   BNK_KK_EKSTRESI_RAPORU::BNK_KK_EKSTRESI_RAPORU
***************************************************************************************/

BNK_KK_EKSTRESI_RAPORU::BNK_KK_EKSTRESI_RAPORU( BNK_RAPOR_VARS * BNK_RAPOR, QString baslik ) : REPORT_KERNEL (baslik)
{
    SET_HEIGHTS ( 30, 30 );

    BNK_RV = BNK_RAPOR;

    m_max_line_count       =  GET_MAX_LINE_COUNT ( 14 );
    m_tum_satirlar_basildi = false;
    m_last_islem_indisi    = 0;
    m_eklenen_kayit_sayisi = 0;
    m_footer_toplam_borc   = 0.0;
    m_footer_toplam_alacak = 0.0;
}



/**************************************************************************************
                   BNK_KK_EKSTRESI_RAPORU::GET_HEADER
***************************************************************************************/

QString BNK_KK_EKSTRESI_RAPORU::GET_HEADER()
{

    QString baslangic_tarihi = BNK_RV->baslangic_tarihi.toString("dd MMMM yyyy dddd");
    QString bitis_tarihi     = BNK_RV->bitis_tarihi.toString("dd MMMM yyyy dddd");

    SET_TABLE_TAG_STRING("<TABLE style = \"width:100%; font-size:75%;\" BORDER=0 >");

    SET_TD_TAG_STRING(QStringList()<<"style = \"width:30% font-size:100%;\""<<"style = \"width:40%;font-size:100%;\" ALIGN=center"
                     <<"style = \"width:30%; font-size:100%;\"ALIGN = right");

    CREATE_TABLE (QStringList()<<""<<""<<"",0,0);

    ADD_ROW_TO_TABLE(QStringList() << E9_GET_FIRMA_ISMI() << "<b>KREDİ KARTI EKSTRESİ</b>"
                     << GET_REPORT_TOP_RIGHT_HEADER());

    ADD_ROW_TO_TABLE (QStringList() <<""<< baslangic_tarihi + " - " + bitis_tarihi << "");



    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   BNK_KK_EKSTRESI_RAPORU::GET_BODY
***************************************************************************************/

QString BNK_KK_EKSTRESI_RAPORU::GET_BODY()
{
    if ( m_tum_satirlar_basildi EQ 1 ) {
        return NULL;
    }
    QStringList headers;
    QStringList islem_satiri;

    double toplam_borc   = 0.0;
    double toplam_alacak = 0.0;
    double borc_bakiye   = 0.0;
    double alacak_bakiye = 0.0;

    QStringList basilacak_satir;

    QString kredi_karti_hesabi = "<p align = center style = \"font-size:85%; font-weight:bold;\">" + BNK_GET_KREDI_KARTI_NUMARASI(BNK_RV->kredi_karti_id) + "</p>";

    headers << "Tarih" << "Fiş No " << "Açıklama" << "Borç" << "Alacak" << "Borç Bakiye" << "Alacak Bakiye";

    SET_TABLE_TAG_STRING("<TABLE style=\"width:100%; font-size:75%; \" BORDER = 1>");
    SET_TD_TAG_STRING(QStringList() << "style = \"width:18%;\"" << "style = \"width:7%\"" << "style  = \"width:35%;\""
                      << "style = \"width:9%;\"" << "style = \"width:9%;\""<< "style = \"width:11%;\"" << "style = \"width:11%\"");

    CREATE_TABLE(headers,1,14);

    SET_TD_TAG_STRING(QStringList()<< "" << "" <<""<<"align = RIGHT "<<"align = RIGHT " << "align = RIGHT" << "align=RIGHT");

    QStringList KREDI_KARTI_ISLEMLERI;

    if (m_eklenen_kayit_sayisi EQ 0 ) {
        if (BNK_RV->nakli_yekun EQ 1 ) {
            PRINT_NAKLI_YEKUN();
            toplam_borc   = m_footer_toplam_borc;
            toplam_alacak = m_footer_toplam_alacak;
        }
        KREDI_KARTI_ISLEMLERI = GET_KREDI_KARTI_ISLEMLERI();
    }

    m_eklenen_kayit_sayisi = 0;

    for ( int i = m_last_islem_indisi; i < KREDI_KARTI_ISLEMLERI.size() ; i++) {

        islem_satiri = KREDI_KARTI_ISLEMLERI.at(i).split("\t");

        m_footer_toplam_borc   += QVariant(islem_satiri.at(3)).toDouble();
        m_footer_toplam_alacak += QVariant(islem_satiri.at(4)).toDouble();;


        toplam_borc   += QVariant(islem_satiri.at(3)).toDouble();
        toplam_alacak += QVariant(islem_satiri.at(4)).toDouble();


        if (toplam_borc > toplam_alacak ) {
            borc_bakiye   = ROUND (toplam_borc - toplam_alacak);
            alacak_bakiye = 0.0;
        }
        else {
            borc_bakiye = 0.0;
            alacak_bakiye = ROUND(toplam_alacak - toplam_borc);
        }
        basilacak_satir.clear();
        basilacak_satir << islem_satiri.at(0) << islem_satiri.at(1) << islem_satiri.at(2)
                        << VIRGUL_EKLE(islem_satiri.at(3)) << VIRGUL_EKLE(islem_satiri.at(4))
                        << VIRGUL_EKLE(QVariant(borc_bakiye).toString(),2)
                        << VIRGUL_EKLE(QVariant(alacak_bakiye).toString() ,2);

        ADD_ROW_TO_TABLE(basilacak_satir);

        m_eklenen_kayit_sayisi++;
        m_last_islem_indisi++;

        if( m_eklenen_kayit_sayisi EQ m_max_line_count ) {
            break;
        }


    }
    if (m_last_islem_indisi >= KREDI_KARTI_ISLEMLERI.size()) {
        m_tum_satirlar_basildi = true;
    }

    kredi_karti_hesabi.append(ADD_HTML_SPACE("     "));


    return kredi_karti_hesabi.append(GET_TABLE_HTML_STRING());
}

/**************************************************************************************
                   BNK_KK_EKSTRESI_RAPORU::GET_FOOTER
***************************************************************************************/

QString BNK_KK_EKSTRESI_RAPORU::GET_FOOTER()
{
    SET_TABLE_TAG_STRING("<TABLE style=\"width:100%; font-size:75%; font-weight:bold\" BORDER=0>");
    SET_TD_TAG_STRING(QStringList() << "style=\"width:60%;\""<< "style=\"width:9%;\" ALIGN=RIGHT"
                                    << "style=\"width:9%;\" ALIGN=RIGHT" << "style=\"width:11%;\" ALIGN=RIGHT"
                                    << "style=\"width:11%;\" ALIGN=RIGHT");

    CREATE_TABLE (QStringList()<< "" << "" << "" << "" << "",0,0);

    double footer_borc_bakiye   = 0.0;
    double footer_alacak_bakiye = 0.0;
    QStringList footer_satiri;

    if ( m_footer_toplam_borc > m_footer_toplam_alacak ) {
        footer_borc_bakiye   = m_footer_toplam_borc - m_footer_toplam_alacak;
    }
    else {
        footer_alacak_bakiye = m_footer_toplam_alacak - m_footer_toplam_borc;
    }

    footer_satiri << "Toplam" << VIRGUL_EKLE(QVariant(ROUND(m_footer_toplam_borc)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(m_footer_toplam_alacak)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(footer_borc_bakiye)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(footer_alacak_bakiye)).toString());
    ADD_ROW_TO_TABLE(footer_satiri);

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   BNK_KK_EKSTRESI_RAPORU::GET_KREDI_KARTI_ISLEMLERI()
***************************************************************************************/

QStringList BNK_KK_EKSTRESI_RAPORU::GET_KREDI_KARTI_ISLEMLERI()
{
    SQL_QUERY select_query(DB);

    QStringList kredi_karti_islemleri;

    QString where_str = QString("fis_turu = :fis_turu "
                                "AND bnk_defter_fisler.fis_id = bnk_defter_fis_satirlari.fis_id AND "
                                "hesap_id = :hesap_id AND "
                                "hesap_turu = :hesap_turu "
                                "AND fis_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi ");

    select_query.PREPARE_SELECT("bnk_defter_fisler,bnk_defter_fis_satirlari","fis_tarihi,fis_no,"
                                "hesaba_giren, hesaptan_cikan, bnk_defter_fis_satirlari.aciklama",where_str);

    select_query.SET_VALUE(":fis_turu"         , ENUM_BANKA_HAREKET_FISI );
    select_query.SET_VALUE(":hesap_id"         , BNK_RV->kredi_karti_id);
    select_query.SET_VALUE(":hesap_turu"       , ENUM_BNK_KREDI_KARTI_HESABI);
    select_query.SET_VALUE(":baslangic_tarihi" , BNK_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    select_query.SET_VALUE(":bitis_tarihi"     , BNK_RV->bitis_tarihi.toString("yyyy.MM.dd"));

    if ( select_query.SELECT("fis_tarihi ASC , fis_no ASC") EQ 0 ) {
         return QStringList();
    }

   QString fis_no;

    while (select_query.NEXT()) {

        fis_no = select_query.VALUE(1).toString();
        kredi_karti_islemleri.append(select_query.VALUE(0).toDate().toString("dd MMMM yyyy dddd") + "\t" +
                                     fis_no + "\t" +
                                     select_query.VALUE(4).toString() + "\t" +
                                     QVariant ( ROUND(select_query.VALUE(2).toDouble())).toString() + "\t" +
                                     QVariant ( ROUND(select_query.VALUE(3).toDouble())).toString() );

    }

    return kredi_karti_islemleri;
}
/**************************************************************************************
                   BNK_KK_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN();
***************************************************************************************/

void BNK_KK_EKSTRESI_RAPORU::PRINT_NAKLI_YEKUN()
{
    double toplam_borc_array[E9_ARRAY_SIZE];
    double toplam_alacak_array[E9_ARRAY_SIZE];

    int bas_gun_no = MALI_YIL_ARRAY_INDIS(BNK_RV->baslangic_tarihi);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("bnk_kk_arrayler","toplam_kk_odeme_borc_array , toplam_kk_odeme_alacak_array" ,
                          "hesap_no_id = :hesap_no_id");
    query.SET_VALUE(":hesap_no_id" , BNK_RV->banka_hesap_no_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    UNPACK_DOUBLE_ARRAY(query.VALUE("toplam_kk_odeme_borc_array").toString() , toplam_borc_array , E9_ARRAY_SIZE);
    UNPACK_DOUBLE_ARRAY(query.VALUE("toplam_kk_odeme_alacak_array").toString() , toplam_alacak_array , E9_ARRAY_SIZE);

    double toplam_borc   = toplam_borc_array[bas_gun_no - 1];
    double toplam_alacak = toplam_alacak_array[bas_gun_no - 1];

    double borc_bakiye   = 0.0;
    double alacak_bakiye = 0.0;
    if (toplam_borc > toplam_alacak ) {
        borc_bakiye = toplam_borc - toplam_alacak;
    }
    else {
        alacak_bakiye = toplam_alacak - toplam_borc;
    }

    m_footer_toplam_borc   = toplam_borc;
    m_footer_toplam_alacak = toplam_alacak;

    QStringList nakli_yekun_satiri;

    nakli_yekun_satiri << "" << "" << "Nakli Yekün "
            << VIRGUL_EKLE(QVariant(ROUND(toplam_borc)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(toplam_alacak)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(borc_bakiye)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(alacak_bakiye)).toString());


    m_eklenen_kayit_sayisi++;

    ADD_ROW_TO_TABLE(nakli_yekun_satiri);
}









