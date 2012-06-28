#include <QDate>
#include "adak_utils.h"
#include "bnk_banka_defteri_raporu_class.h"
#include "bnk_banka_defteri_raporu_open.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_console_utils.h"
#include "yonetim.h"
#include "e9_enum.h"
#include "banka_struct.h"
#include "banka_enum.h"

extern ADAK_SQL * DB;

/*****************************************************************/
/*                  F_BNK_BANKA_DEFTERI_RAPORU()                            */
/*****************************************************************/

QString GET_BNK_BANKA_DEFTERI_RAPORU ( BNK_RAPOR_VARS * BNK_RAPOR )
{
    BNK_BANKA_DEFTERI_RAPORU  *  R = new BNK_BANKA_DEFTERI_RAPORU( BNK_RAPOR );
    return R->CREATE_HTML_PAGES();
}

/*****************************************************************/
/*     BNK_BANKA_DEFTERI_RAPORU::BNK_BANKA_DEFTERI_RAPORU                                  */
/*****************************************************************/

BNK_BANKA_DEFTERI_RAPORU::BNK_BANKA_DEFTERI_RAPORU( BNK_RAPOR_VARS * BNK_RAPOR ) : REPORT_KERNEL ("BNK_BANKA_DEFTERI_RAPORU")
{
    SET_HEIGHTS (30,30);

    BNK_RV = BNK_RAPOR;

    m_max_line_count             =  GET_MAX_LINE_COUNT (11);

    m_last_satir_indisi          = 0;
    m_tum_satirlar_basildi       = false;
    m_eklenen_kayit_sayisi       = 0;
    m_footer_toplam_borc         = 0.0;
    m_footer_toplam_alacak       = 0.0;
}

/*****************************************************************/
/*              BNK_BANKA_DEFTERI_RAPORU::GET_HEADER                        */
/*****************************************************************/

QString BNK_BANKA_DEFTERI_RAPORU::GET_HEADER()
{

   QString baslangic_tarihi = BNK_RV->baslangic_tarihi.toString("dd MMMM yyyy dddd");
   QString bitis_tarihi     = BNK_RV->bitis_tarihi.toString("dd MMMM yyyy dddd");

   QStringList string_list;

   SET_TABLE_TAG_STRING("<TABLE style = \"width:100%; font-size:75%;\" BORDER=0 >");

   SET_TD_TAG_STRING(QStringList()<<"style = \"width:30% font-size:100%;\""<<"style = \"width:40%;font-size:100%;\" ALIGN=center"
                     <<"style = \"width:30%; font-size:100%;\"ALIGN = right");

   CREATE_TABLE (QStringList()<<""<<""<<"",0,0);

   string_list << E9_GET_FIRMA_ISMI() << "<b>BANKA DEFTERİ<b>" << GET_REPORT_TOP_RIGHT_HEADER();

   ADD_ROW_TO_TABLE(string_list);

   string_list.clear();
   string_list<<""<< baslangic_tarihi + " - " + bitis_tarihi << "";
   ADD_ROW_TO_TABLE(string_list);

   return GET_TABLE_HTML_STRING();
}

/*****************************************************************/
/*              BNK_BANKA_DEFTERI_RAPORU::GET_BODY                          */
/*****************************************************************/

QString BNK_BANKA_DEFTERI_RAPORU::GET_BODY()
{
    QStringList     headers;
    QStringList     fis_satiri;
    QStringList     basilacak_satir;
    QString         banka_hesabi;

    if (m_tum_satirlar_basildi EQ 1 ) {
        return NULL;
    }

    banka_hesabi = "<p align=center style = \"font-size:85%; font-weight:bold;\">";

    SQL_QUERY select_query(DB);
    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_numarasi,banka_ismi,sube_kodu,sube_ismi,doviz_id","hesap_no_id = :hesap_no_id");
    select_query.SET_VALUE(":hesap_no_id" , BNK_RV->banka_hesap_no_id);
    if (select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        banka_hesabi.append( select_query.VALUE(1).toString() + ADD_HTML_SPACE("   ") + select_query.VALUE(2).toString() + " - "+
                             select_query.VALUE(3).toString() + ADD_HTML_SPACE("   ") + select_query.VALUE(0).toString() + ADD_HTML_SPACE("   ")
                             + DVZ_GET_DOVIZ_KODU(select_query.VALUE(4).toInt()));
    }
    banka_hesabi.append("</p>");

    headers << "Tarih" << "Fiş No " << "Açıklama" << "Borç" << "Alacak" << "Borç Bakiye" << "Alacak Bakiye";

    SET_TABLE_TAG_STRING("<TABLE style=\"width:100%; font-size:75%; \" BORDER = 1>");
    SET_TD_TAG_STRING(QStringList() << "style = \"width:18%;\"" << "style = \"width:7%\"" << "style  = \"width:35%;\""
                      << "style = \"width:9%;\"" << "style = \"width:9%;\""<< "style = \"width:11%;\"" << "style = \"width:11%\"");

    CREATE_TABLE(headers,1,14);
    SET_TD_TAG_STRING(QStringList()<< "" << "" <<""<<"align = RIGHT "<<"align = RIGHT " << "align = RIGHT" << "align=RIGHT");

    if (m_eklenen_kayit_sayisi EQ 0 ) {
        if (BNK_RV->nakli_yekun EQ 1 ) {
            PRINT_NAKLI_YEKUN();
        }
        m_fis_satirlari_list = GET_BANKA_ISLEMLERI();
    }

    for ( ; ; ) {

        if (m_last_satir_indisi >= m_fis_satirlari_list.size()) {
            m_tum_satirlar_basildi = true;
            break;
        }

        m_eklenen_kayit_sayisi = 0;

        for (int i = m_last_satir_indisi ; i < m_fis_satirlari_list.size() ; i++ ) {

            fis_satiri = m_fis_satirlari_list.at(i).split("\t");

            m_footer_toplam_borc   += QVariant(fis_satiri.at(3)).toDouble();
            m_footer_toplam_alacak += QVariant(fis_satiri.at(4)).toDouble();

            basilacak_satir.clear();

            basilacak_satir << QVariant(fis_satiri.at(0)).toDate().toString("dd MMMM yyyy dddd") << fis_satiri.at(1) << fis_satiri.at(2)
                            << VIRGUL_EKLE(fis_satiri.at(3)) << VIRGUL_EKLE(fis_satiri.at(4))
                            << VIRGUL_EKLE(fis_satiri.at(5)) << VIRGUL_EKLE(fis_satiri.at(6));
            ADD_ROW_TO_TABLE(basilacak_satir);

            m_eklenen_kayit_sayisi++;
            m_last_satir_indisi++;

            if (m_eklenen_kayit_sayisi EQ m_max_line_count ) {
                banka_hesabi.append(ADD_HTML_SPACE(" "));
                return banka_hesabi.append(GET_TABLE_HTML_STRING());;
            }
        }
    }
    banka_hesabi.append(ADD_HTML_SPACE(" "));
    return banka_hesabi.append(GET_TABLE_HTML_STRING());
}

/*****************************************************************/
/*                  BNK_BANKA_DEFTERI_RAPORU::GET_FOOTER                    */
/*****************************************************************/

QString BNK_BANKA_DEFTERI_RAPORU::GET_FOOTER()
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

/********************************************************************/
/*                BNK_BANKA_DEFTERI_RAPORU::GET_BANKA_ISLEMLERI()             */
/********************************************************************/

QStringList BNK_BANKA_DEFTERI_RAPORU::GET_BANKA_ISLEMLERI()
{
    QStringList fisler;
    QStringList fis;
    QStringList fis_satiri;
    QStringList fis_satirlari_list;
    double      toplam_hesaba_giren   = m_footer_toplam_borc;
    double      toplam_hesaptan_cikan = m_footer_toplam_alacak;
    double      borc_bakiye;
    double      alacak_bakiye;

    SQL_QUERY select_query(DB);

    //Banka defteri raporu banka defter fisinden verilir.Banka islem fisinden rapor verilmez.ama banka islem
    //fisindeki fis numaralari gosterilir.

    QString where_str = QString("hesap_no_id = :hesap_no_id "
                                "AND fis_turu = :fis_turu AND fis_tarihi BETWEEN "
                                ":bas_tarihi AND :bts_tarihi ");

    select_query.PREPARE_SELECT("bnk_defter_fisler","fis_id,fis_tarihi,fis_no",where_str);

    select_query.SET_VALUE(":hesap_no_id" , BNK_RV->banka_hesap_no_id);
    select_query.SET_VALUE(":fis_turu"    , ENUM_BANKA_HAREKET_FISI  );
    select_query.SET_VALUE(":bas_tarihi"  , BNK_RV->baslangic_tarihi.toString("yyyy.MM.dd"));
    select_query.SET_VALUE(":bts_tarihi"  , BNK_RV->bitis_tarihi.toString("yyyy.MM.dd"));

    if ( select_query.SELECT("fis_tarihi ASC , fis_no ASC") EQ 0 ) {
        return QStringList();
    }

    while (select_query.NEXT()) {
        fisler.append(select_query.VALUE(0).toString() + "\t" +
                      select_query.VALUE(1).toString() +
                      "\t" + select_query.VALUE(2).toString() );
                     
    }

    int fis_no = 0;

    for ( int i = 0 ; i < fisler.size() ; i++ ) {

        fis = fisler.at(i).split("\t");

        fis_no = QVariant(fis.at(2)).toInt();

        if ( QVariant(fis.at(2)).toInt() EQ 0 ) {
           fis_no = QVariant(fis.at(3)).toInt();
        }


        select_query.PREPARE_SELECT("bnk_defter_fis_satirlari","aciklama,hesaptan_cikan,hesaba_giren,hesap_id, hesap_turu","fis_id = :fis_id");
        select_query.SET_VALUE(":fis_id" , QVariant(fis.at(0)).toInt());

        if ( select_query.SELECT() EQ 0 ) {
            continue;
        }

        while ( select_query.NEXT() EQ true ) {

            QString     aciklama                 = select_query.VALUE(0).toString();
            double      hesaba_giren             = select_query.VALUE(2).toDouble();
            double      hesaptan_cikan           = select_query.VALUE(1).toDouble();
            //int         hesap_id                 = select_query.VALUE(3).toInt();
            int         hesap_turu               = select_query.VALUE(4).toInt();

            if ( hesap_turu NE ENUM_BNK_BANKA_HESABI ) {
                continue;
            }

            toplam_hesaba_giren   += hesaba_giren;
            toplam_hesaptan_cikan += hesaptan_cikan;

            if ( toplam_hesaba_giren > toplam_hesaptan_cikan ) {
                borc_bakiye = toplam_hesaba_giren - toplam_hesaptan_cikan;
                alacak_bakiye = 0.0;
            }
            else {
                alacak_bakiye = toplam_hesaptan_cikan - toplam_hesaba_giren;
                borc_bakiye   = 0.0;
            }

            if (hesaba_giren > hesaptan_cikan) {
                hesaba_giren   -= hesaptan_cikan;
                hesaptan_cikan  = 0.0;
            }
            else {
                hesaptan_cikan -= hesaba_giren;
                hesaba_giren    = 0.0;
            }

            fis_satiri.clear();
            fis_satiri.append(fis.at(1) + "\t" + QVariant(fis_no).toString() + "\t" + aciklama + "\t"
                              + QVariant(ROUND(hesaba_giren)).toString() + "\t"
                              + QVariant(ROUND(hesaptan_cikan)).toString() + "\t"
                              + QVariant(ROUND(borc_bakiye)).toString() + "\t"
                              + QVariant(ROUND(alacak_bakiye)).toString());

            fis_satirlari_list << fis_satiri;
        }

    }

    return fis_satirlari_list;

}

/***************************************************************/
/*            BNK_BANKA_DEFTERI_RAPORU::PRINT_NAKLI_YEKUN()    */
/***************************************************************/

void BNK_BANKA_DEFTERI_RAPORU::PRINT_NAKLI_YEKUN()
{
    double toplam_borc_array   [E9_ARRAY_SIZE];
    double toplam_alacak_array [E9_ARRAY_SIZE];

    int bas_gun_no = MALI_YIL_ARRAY_INDIS ( BNK_RV->baslangic_tarihi);

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","toplam_borc_array,toplam_alacak_array,"
                                "hesap_no_id = :hesap_no_id");

    select_query.SET_VALUE(":hesap_no_id", BNK_RV->banka_hesap_no_id);

    if(select_query.SELECT() EQ 0) {
        return;
    }
    select_query.NEXT();

    UNPACK_DOUBLE_ARRAY(select_query.VALUE("toplam_borc_array").toString(),toplam_borc_array,E9_ARRAY_SIZE);
    UNPACK_DOUBLE_ARRAY(select_query.VALUE("toplam_alacak_array").toString(),toplam_alacak_array,E9_ARRAY_SIZE);

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
