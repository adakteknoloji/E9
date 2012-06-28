#include "adak_sql.h"
#include "bnk_pos_ekstresi_class.h"
#include "bnk_pos_ekstresi_open.h"
#include "adak_utils.h"
#include "e9_console_utils.h"
#include "e9_enum.h"
#include "yonetim.h"
#include "banka_struct.h"
#include "bnk_console_utils.h"
#include "banka_enum.h"

extern ADAK_SQL * DB;

/*****************************************************************/
/*                  F_BNK_POS_EKSTRESI()                             */
/*****************************************************************/

QString GET_BNK_POS_EKSTRESI ( BNK_RAPOR_VARS * BNK_RAPOR )
{
    BNK_POS_EKSTRESI  *  R = new BNK_POS_EKSTRESI( BNK_RAPOR );
    return R->CREATE_HTML_PAGES();
}

/*****************************************************************/
/*              BNK_POS_EKSTRESI::BNK_POS_EKSTRESI                       */
/*****************************************************************/

BNK_POS_EKSTRESI::BNK_POS_EKSTRESI( BNK_RAPOR_VARS * BNK_RAPOR ) : REPORT_KERNEL ("BNK_POS_EKSTRESI")
{
    SET_HEIGHTS (30,30);


    BNK_RV  = BNK_RAPOR;

    m_max_line_count             =  GET_MAX_LINE_COUNT (12);
    m_tum_satirlar_basildi       = false;
    m_last_rapor_satiri_indisi   = 0;
    m_eklenen_kayit_sayisi       = 0;
    m_footer_toplam_borc         = 0.0;
    m_footer_toplam_alacak       = 0.0;
}

/*****************************************************************/
/*              BNK_POS_EKSTRESI::GET_HEADER                         */
/*****************************************************************/

QString BNK_POS_EKSTRESI::GET_HEADER()
{

    QString baslangic_tarihi = BNK_RV->baslangic_tarihi.toString("dd MMMM yyyy dddd");
    QString bitis_tarihi     = BNK_RV->bitis_tarihi.toString("dd MMMM yyyy dddd");

    QStringList string_list;

    SET_TABLE_TAG_STRING("<TABLE style = \"width:100%; font-size:75%;\" BORDER=0 >");

    SET_TD_TAG_STRING(QStringList()<<"style = \"width:30% font-size:100%;\""<<"style = \"width:40%;font-size:100%;\" ALIGN=center"
                     <<"style = \"width:30%; font-size:100%;\"ALIGN = right");

    CREATE_TABLE (QStringList()<<""<<""<<"",0,0);

    string_list << E9_GET_FIRMA_ISMI() << "<b>POS EKSTRESİ</b>" << GET_REPORT_TOP_RIGHT_HEADER();

    ADD_ROW_TO_TABLE(string_list);

    string_list.clear();
    string_list<<""<< baslangic_tarihi + " - " + bitis_tarihi << "";
    ADD_ROW_TO_TABLE(string_list);

    return GET_TABLE_HTML_STRING();
}

/*****************************************************************/
/*              BNK_POS_EKSTRESI::GET_BODY                           */
/*****************************************************************/

QString BNK_POS_EKSTRESI::GET_BODY()
{
    QString         pos_hesabi;
    QStringList     headers;
    QStringList     rapor_satiri;
    QStringList     basilacak_satir;
    double          toplam_borc   = 0.0;
    double          toplam_alacak = 0.0;
    double          borc_bakiye;
    double          alacak_bakiye;

    if (m_tum_satirlar_basildi EQ 1 ) {
        return NULL;
    }

    pos_hesabi = "<p align = center style = \"font-size:85%; font-weight:bold;\">" + BNK_GET_POS_NUMARASI(BNK_RV->pos_id) + "</p>";

    headers << "Tarih" << "Fiş No " << "Açıklama" << "Borç" << "Alacak" << "Borç Bakiye" << "Alacak Bakiye";

    SET_TABLE_TAG_STRING("<TABLE style=\"width:100%; font-size:75%; \" BORDER = 1>");
    SET_TD_TAG_STRING(QStringList() << "style = \"width:18%;\"" << "style = \"width:7%\"" << "style  = \"width:35%;\""
                      << "style = \"width:9%;\"" << "style = \"width:9%;\""<< "style = \"width:11%;\"" << "style = \"width:11%\"");

    CREATE_TABLE(headers,1,14);
    SET_TD_TAG_STRING(QStringList()<< "" << "" <<""<<"align = RIGHT "<<"align = RIGHT " << "align = RIGHT" << "align=RIGHT");

    if (m_eklenen_kayit_sayisi EQ 0 ) {
        if(BNK_RV->nakli_yekun EQ 1) {
            PRINT_NAKLI_YEKUN();
            toplam_borc   = m_footer_toplam_borc;
            toplam_alacak = m_footer_toplam_alacak;
        }
        m_pos_islemleri_list = GET_POS_ISLEMLERI();
    }

    for ( ; ; ) {
        if (m_last_rapor_satiri_indisi >= m_pos_islemleri_list.size()) {
            m_tum_satirlar_basildi = true;
            break;
        }

        m_eklenen_kayit_sayisi = 0;

        for (int i = m_last_rapor_satiri_indisi ; i < m_pos_islemleri_list.size() ; i++ ) {

            rapor_satiri = m_pos_islemleri_list.at(i).split("\t");

            m_footer_toplam_borc   += QVariant(rapor_satiri.at(3)).toDouble();
            m_footer_toplam_alacak += QVariant(rapor_satiri.at(4)).toDouble();

            toplam_borc += QVariant(rapor_satiri.at(3)).toDouble();
            toplam_alacak += QVariant(rapor_satiri.at(4)).toDouble();

            if (toplam_borc > toplam_alacak ) {
                borc_bakiye = toplam_borc - toplam_alacak;
                alacak_bakiye = 0.0;
            }
            else {
                alacak_bakiye = toplam_alacak - toplam_borc;
                borc_bakiye   = 0.0;
            }


            basilacak_satir.clear();
            basilacak_satir << rapor_satiri.at(0) << rapor_satiri.at(1) << rapor_satiri.at(2)
                            << VIRGUL_EKLE(rapor_satiri.at(3)) << VIRGUL_EKLE(rapor_satiri.at(4))
                            << VIRGUL_EKLE(QVariant(ROUND(borc_bakiye)).toString())
                            << VIRGUL_EKLE(QVariant(ROUND(alacak_bakiye)).toString()) ;

            ADD_ROW_TO_TABLE(basilacak_satir);
            m_eklenen_kayit_sayisi++;
            m_last_rapor_satiri_indisi++;

            if(m_eklenen_kayit_sayisi EQ m_max_line_count ) {
                break;
            }
        }

    }

    pos_hesabi.append(ADD_HTML_SPACE("   "));
    return pos_hesabi.append(GET_TABLE_HTML_STRING());
}

/*****************************************************************/
/*                  BNK_POS_EKSTRESI::GET_FOOTER                     */
/*****************************************************************/

QString BNK_POS_EKSTRESI::GET_FOOTER()
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
            << VIRGUL_EKLE(QVariant(ROUND(m_footer_toplam_alacak)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(footer_borc_bakiye)).toString())
            << VIRGUL_EKLE(QVariant(ROUND(footer_alacak_bakiye)).toString());
    ADD_ROW_TO_TABLE(footer_line);

    return GET_TABLE_HTML_STRING();
}

/*****************************************************************/
/*             BNK_POS_EKSTRESI::GET_POS_ISLEMLERI()                 */
/*****************************************************************/

QStringList BNK_POS_EKSTRESI::GET_POS_ISLEMLERI()
{
    SQL_QUERY    select_query(DB);
    QStringList  fis;
    QStringList  banka_fisleri;
    QStringList  pos_islemleri;

    QString where_str = QString ("hesap_no_id = :hesap_no_id AND fis_turu = :fis_turu AND fis_tarihi "
                                 "BETWEEN :bas_tarihi AND :bts_tarihi");

    select_query.PREPARE_SELECT("bnk_defter_fisler","fis_id,fis_no,fis_tarihi",where_str);

    select_query.SET_VALUE(":hesap_no_id" ,BNK_RV->banka_hesap_no_id );
    select_query.SET_VALUE(":fis_turu"   , ENUM_BANKA_HAREKET_FISI);
    select_query.SET_VALUE(":bas_tarihi" , BNK_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    select_query.SET_VALUE(":bts_tarihi" , BNK_RV->bitis_tarihi.toString("yyyy.MM.dd"));

    if (select_query.SELECT() EQ  0 ) {
        return pos_islemleri;
    }
    while (select_query.NEXT()) {
        QString fis_id       = select_query.VALUE(0).toString();
        QString fis_no       = select_query.VALUE(1).toString();
        QString fis_tarihi   = select_query.VALUE(2).toDate().toString("dd MMMM yyyy dddd");


        fis.clear();
        fis.append(fis_id + "\t" + fis_tarihi + "\t" + fis_no );
        banka_fisleri << fis;
    }

    for (int i = 0 ; i < banka_fisleri.size() ; i++ ) {

        fis = banka_fisleri.at(i).split("\t");

        select_query.PREPARE_SELECT("bnk_defter_fis_satirlari","hesaba_giren,hesaptan_cikan , aciklama,"
                                    "hesap_turu","hesap_id = :hesap_id AND fis_id = :fis_id");
        select_query.SET_VALUE(":hesap_id" , BNK_RV->pos_id);
        select_query.SET_VALUE(":fis_id" , QVariant(fis.at(0)).toInt());

        if (select_query.SELECT() NE 0 ) {

            while (select_query.NEXT()) {
                if (select_query.VALUE(3).toInt() NE ENUM_BNK_POS_HESABI) {
                    continue;
                }
                double hesaba_giren      = select_query.VALUE(0).toDouble();
                double hesaptan_cikan    = select_query.VALUE(1).toDouble();
                QString aciklama         = select_query.VALUE(2).toString();

                pos_islemleri.append(fis.at(1) + "\t" + fis.at(2) + "\t" + aciklama + "\t" +  QVariant(ROUND(hesaba_giren)).toString()+ "\t"
                                     + QVariant(ROUND(hesaptan_cikan)).toString());
            }
        }
    }
    pos_islemleri.sort();

    return pos_islemleri;
}

/***************************************************************/
/*             BNK_POS_EKSTRESI::PRINT_NAKLI_YEKUN();              */
/***************************************************************/

void BNK_POS_EKSTRESI::PRINT_NAKLI_YEKUN()
{
    double toplam_borc_array[E9_ARRAY_SIZE];
    double toplam_alacak_array[E9_ARRAY_SIZE];

    int bas_gun_no = MALI_YIL_ARRAY_INDIS(BNK_RV->baslangic_tarihi);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("bnk_pos_arrayler","toplam_kk_tahsilat_borc_array , toplam_kk_thslat_alacak_array" ,
                         "hesap_no_id = :hesap_no_id");
    query.SET_VALUE(":hesap_no_id" , BNK_RV->banka_hesap_no_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    UNPACK_DOUBLE_ARRAY(query.VALUE("toplam_kk_tahsilat_borc_array").toString() , toplam_borc_array , E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY(query.VALUE("toplam_kk_thslat_alacak_array").toString() , toplam_alacak_array , E9_ARRAY_SIZE );

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
