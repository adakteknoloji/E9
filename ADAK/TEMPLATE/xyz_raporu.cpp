#include "adak_utils.h"
#include "xyz_raporu_open.h"
#include "xyz_raporu_class.h"
#include "e9.h"
#include "e9_defines.h"

extern ADAK_SQL *         DB;

extern RAPOR_VARS          RV;

// Not - 1 : Tum fonksiyonlar ve kullanim detaylari ayni zamanda report_kernel.h dosyasindada mevcuttur.

// Not - 2 : Font genislikleri icin html de kullanilan <font size=X> tagi yerine css de kullanilan style="font-size:18pt" kullanilmalidir.
// Boylelikle sizelarimiz bir birime(pt) gore atanir. Ayni zamanda "size" ekran duzenine gore bir katsayi gibi davranirken "pt" ile
// belirtigimiz bir boyut daima sabittir.

/**************************************************************************************
                   GET_XYZ_RAPORU
***************************************************************************************/

QString GET_XYZ_RAPORU ()
{
    XYZ_RAPORU  *  F = new XYZ_RAPORU( " ????????_raporu ");
    return F->CREATE_HTML_PAGES();
}

/**************************************************************************************
                   XYZ_RAPORU::XYZ_RAPORU
***************************************************************************************/

XYZ_RAPORU::XYZ_RAPORU( QString rapor_ismi ) : REPORT_KERNEL (rapor_ismi)
{
    SET_HEIGHTS ( 30, 30 );
    // Line genişliğini parametre olarak gonderiyoruz.

    // Maxlinecount bir sayfaya sigacak satir sayisidir.Burada gonderilen deger satirdaki fontlarin size ini etkilemez. Buradaki deger satirlardaki
    // textlerin font size i kadar olmalidir.Ancak eger sayfada fazla satir varsa veya tam tersi fazla bosluk varsa degeri 1-2 pt civari oynatarak
    // tam olarak sigmasi saglanabilir.
    m_max_line_count  =  GET_MAX_LINE_COUNT ( 14 );

}

/**************************************************************************************
                   XYZ_RAPORU::~XYZ_RAPORU
***************************************************************************************/

XYZ_RAPORU::~XYZ_RAPORU()
{

}

// Kernel headeri cagirir sonrasinda body ve son olarak footeri cagirir.Bu dongu body den NULL gonderilene kadar devam eder.

/**************************************************************************************
                   XYZ_RAPORU::GET_HEADER
***************************************************************************************/

QString XYZ_RAPORU::GET_HEADER()
{

// Bu kod standart bir rapor headeridir.
//    SQL_QUERY       query(DB);
//    QString         firma_ismi;
//
//    query.PREPARE_SELECT( "e9_sabit_degerler","firma_ismi");
//    firma_ismi = "";
//    if (query.SELECT() NE 0 ) {
//        query.NEXT();
//        firma_ismi = query.VALUE("firma_ismi").toString();
//    }
//
//
//    QDate sistem_tarihi       = QDate::currentDate();
//    QString sistem_tarihi_str = sistem_tarihi.toString("dd MMMM yyyy dddd");
//
//
//    QTime sistem_saati        = QTime::currentTime();
//    QString sistem_saati_str  = sistem_saati.toString("hh:mm");
//
//    QString versiyon          = GV.versiyon;
//
//
//    QString tarih_saat_versiyon = sistem_tarihi_str;
//    tarih_saat_versiyon.append(" , ");
//    tarih_saat_versiyon.append(sistem_saati_str);
//    tarih_saat_versiyon.append(" , ");
//    tarih_saat_versiyon.append(versiyon);
//
//
//   QString baslangic_tarihi = RV.baslangic_tarihi.toString("dd MMMM yyyy dddd");
//   QString bitis_tarihi     = RV.bitis_tarihi.toString("dd MMMM yyyy dddd");
//
//
//   QStringList string_list;
//
//   SET_TABLE_TAG_STRING("<TABLE WIDTH=1200 BORDER=0 >");
//
//   SET_TD_TAG_STRING(QStringList()<<"WIDTH = 350 style=font-size:14pt;"<<"WIDTH=500 ALIGN=CENTER style=font-size:15pt;"<<"WIDTH=350 ALIGN=RIGHT style=font-size:13pt;");
//
//   CREATE_TABLE (QStringList()<<""<<""<<"",0,0);
//
//   string_list << firma_ismi;
//   string_list << "";
//   string_list <<tarih_saat_versiyon;
//   ADD_ROW_TO_TABLE(string_list);
//
//
//   p_sayfa_numarasi++;
//
//   QString sayfa_numarasi = "Sayfa No : ";
//   sayfa_numarasi.append(QVariant(p_sayfa_numarasi).toString());
//
//   string_list.clear();
//   string_list <<""<<""<<sayfa_numarasi;
//   ADD_ROW_TO_TABLE(string_list);
//
//   QString filters_1 = "";
//   QString filters_2 = "";
//
//   if (filters_1.isEmpty() EQ true) {
//       if (RV.bakiyesiz_hesaplar EQ 1) {
//            filters_1.append( "Bakiyesiz" );
//       }
//       if (RV.calismamis_hesaplar EQ 1) {
//          if (filters_1.isEmpty() EQ false) {
//               filters_1.append( "-");
//          }
//          filters_1.append( "Çalışmamış");
//       }
//       if (filters_1.isEmpty() EQ false) {
//           filters_1.append( " Hesaplar");
//       }
//
//       if (RV.nakli_yekun EQ 1) {
//            filters_1.append( " Nakli Yekün");
//       }
//
//       filters_2.append(QString("Mizan Seviyesi:%1").arg(RV.mizan_seviyesi));
//       filters_2.append(QString(" Hesap Aralığı:%1-%2").arg(RV.bas_hesap_kodu,RV.bts_hesap_kodu));
//       SQL_QUERY query(DB);
//       query.PREPARE_SELECT("muh_hesap_gruplari","grup_adi","grup_id=:grup_id");
//       query.SET_VALUE(":grup_adi",p_grup_id);
//       if(query.SELECT() NE 0) {
//            query.NEXT();
//            filters_2.append(QString(" %1").arg(query.VALUE(0).toString()));
//       }
//   }
//
//   string_list.clear();
//   string_list<<""<<"<b>MİZAN RAPORU<b>"<<filters_1;
//   ADD_ROW_TO_TABLE(string_list);
//
//   string_list.clear();
//   string_list<<""<<baslangic_tarihi+" - "+bitis_tarihi<<filters_2;
//   ADD_ROW_TO_TABLE(string_list);
//
//
//   return GET_TABLE_HTML_STRING();

    // Basit bir header ornegi
   CREATE_TABLE (QStringList() << "Hesap Kodu" << "Hesap Ismi" << "", 0, 14 );

   // header table imizi olusturduktan sonra return ederiz.Header da birden fazla table kullanmamiz gerekliyse
   // Birden fazla table yaratip her bir table in string ini GET_TABLE_HTML_STRING le alip.Tumunu append edip
   // return edebiliriz.
   return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   XYZ_RAPORU::GET_BODY
***************************************************************************************/

QString XYZ_RAPORU::GET_BODY()
{
    SQL_QUERY query(DB);

    QStringList column_header_list << "" << "";
    CREATE_TABLE(column_header_list, 0, 14);

    query.PREPARE_SELECT ( "muh_hesaplar","hesap_kodu,hesap_ismi","tam_hesap_kodu = :tam_hesap_kodu" );
    query.SET_VALUE ( ":tam_hesap_kodu",         tam_hesap_kodu );

    if (query.SELECT("hesap_kodu",p_eklenen_kayit_sayisi,p_max_line_count) EQ 0) {
        return NULL;
    }

    while(query.NEXT()) {
        QStringList row_datas << sql_query.VALUE(0).toString() << sql_query.VALUE(1).toString();
        ADD_ROW_TO_TABLE ( row_datas );
    }

    m_eklenen_kayit_sayisi += m_max_line_count;

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   XYZ_RAPORU::GET_FOOTER
***************************************************************************************/

QString XYZ_RAPORU::GET_FOOTER()
{
    QStringList string_list;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH =1200 BORDER=0 style=font-size:15pt;>" );
    SET_TD_TAG_STRING(QStringList() << "WIDTH=690" << "WIDTH=130 ALIGN=RIGHT"
                      << "WIDTH=130 ALIGN =RIGHT" << "WIDTH =130 ALIGN=RIGHT"
                      << "WIDTH=130 ALIGN=RIGHT");

    CREATE_TABLE (QStringList()<< "" << "" << "" << "" << "", 0, 0 );


    if (m_footer_borc > p_footer_alacak ) {
        m_footer_borc_bakiye   = m_footer_borc - m_footer_alacak;
        m_footer_alacak_bakiye = 0.0;

    }
    else {
        m_footer_alacak_bakiye = m_footer_alacak - m_footer_borc;
        m_footer_borc_bakiye   = 0.0;
    }

    string_list.clear();
    string_list << "";
    string_list << "<b>" + VIRGUL_EKLE(QVariant(ROUND(m_footer_borc         )).toString());
    string_list << "<b>" + VIRGUL_EKLE(QVariant(ROUND(m_footer_alacak       )).toString());
    string_list << "<b>" + VIRGUL_EKLE(QVariant(ROUND(m_footer_borc_bakiye  )).toString());
    string_list << "<b>" + VIRGUL_EKLE(QVariant(ROUND(m_footer_alacak_bakiye)).toString());

    ADD_ROW_TO_TABLE ( string_list );

    return GET_TABLE_HTML_STRING();
}
