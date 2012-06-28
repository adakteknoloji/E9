#ifndef REPORT_KERNEL_H
#define REPORT_KERNEL_H

#include <QString>
#include <QStringList>
#include <QSplashScreen>
#include <QPixmap>
#include <QPrinter>
#include <html_tag.h>
#include "adak_sql.h"


/*
 * Point: ( Turkce karsiligi Punto ), baskı yazılarının genel ölçü birimidir. 1 punto, metrik sistemde yaklaşık 0,376 mm.dir.
 */



 /*
 * Kernel Kullanimi;
 * Rapor kernelda oncelikle SET_HEIGHTS fonksiyonunu kullanarak footer ve header genisliklerini ayarlamaliyiz.
 * Daha sonra bir satirimizin genisligini GET_MAX_LINE_COUNT fonksiyonuna gondererek body'e ekleyebilecegimiz maximum satir sayisini bulmaliyiz.Bunlar
 * hesaplandiktan sonra kernel calisma mantigi geregi once header i cagiracak ve gonderilen degeri header bosluguna eklicek.
 * Burda programci GET_HEADER da headerini duzenleyerek duzenlemis oldugu stringi return etmeli. Sonra kernel GET_BODY i cagiracak
 * burda programci GET_MAX_LINE_COUNT da aldigi deger kadar sayfaya satir eklemeli ve bu satirlari bir string olarak dondurmeli. Burada birden fazla
 * sayfa soz konusu ise programci son gondermis oldugu satiri kayitta tutarak bir dahaki GET_BODY de satirlari eklemeye bu satirdan baslayabilir.
 * Ve son olarak, tum satirlari sayfaya ekleyip son sayfanin body ide dondurdukten sonra bir dahaki GET_BODY'de null dondurmesi donguyu
 * bitirir ve boylelikle rapor hazirlanmis olur. GET_FOOTER da her GET_BODY den sonra cagrilir ve donus degeri footer alanina yazilir.
 */


/* Table Ekleme;
 * Eger standart bir tablo eklenecekse once header olusturulup daha sonra row lar eklenerek, GET_TABLE_HTML_STRING le bu table kodu string olarak alinabilir.
 * QStringList column_header_list << "Column Basligi" << "Column Basligi 2";
 * CREATE_TABLE(column_header_list, 1, 13);
 * for ( int i = 0; i < 9999 ; i++ ) {
 *   QStringList row_datas << "row xyz, column xyz" << "row xyz, column xyz+1";
 *   ADD_ROW_TO_TABLE(row_datas);
 * }
 * QStrign table_string = GET_TABLE_HTML_STRING();
 *
 * Ancak table veya columnlari programci tanimlamak isterse asagidaki sekilde bu tanimlamayi yapabilir.
 * SET_TABLE_TAG_STRING("<table width=200 height=20 border=1>");
 * QStringList column_header_list << "Column Basligi" << "Column Basligi 2";
 * CREATE_TABLE(column_header_list, 0, 0);
 * SET_TD_TAG_STRING(QStringList()<<"WIDTH = 110 align=RIGHT"<<"WIDTH = 110 align=RIGHT");
 * for ( int i = 0; i < 9999 ; i++ ) {
 *   QStringList row_datas << "row xyz, column xyz" << "row xyz, column xyz+1";
 *   ADD_ROW_TO_TABLE(row_datas);
 * }
 * QStrign table_string = GET_TABLE_HTML_STRING();
 *
 *
 *
 * NOT: Fonksiyonlar ile ilgili detayli bilgi asagida bulunmaktadir.
 */

//standard point milimetre katsayısı
#define  POINT_MM_ORANI (float)127/(float)360

enum CSS_POSITION_VALUE_NAME {
    POSITION_ABSOLUTE = 1,
    POSITION_FIXED    = 2,
    POSITION_RELATIVE = 3,
    POSITION_STATIC   = 4,
    POSITION_INHERIT  = 5
};

enum WRAP_TYPE {
    WRAP    = 1,
    NO_WRAP = 2
};

class REPORT_KERNEL : public HTML_TAG
{
public:
    REPORT_KERNEL( QString report_name );
    virtual ~REPORT_KERNEL();
    QString             CREATE_HTML_PAGES                  ();
    void                OPEN_PRINTER_PREVIEW  (QWidget * parent, qreal left_margin = 0.0,qreal top_margin = 0.0,qreal bottom_margin=0.0,qreal right_margin=0.0);

protected:
    virtual QString     GET_HEADER                         () = 0;
    /* Bu fonksiyon kernel tarafindan cagrilir ve  fonksiyonun donus degeri SET_HEIGHTS ile atamis oldugumuz header bosluguna
     * eklenir.Eger header yoksa bu fonksiyon NULL dondurmeli ve SET_HEIGHTS da header_height 0 olmalidir.
     */


    virtual QString     GET_FOOTER                         () = 0;
    /* Bu fonksiyon kernel tarafindan cagrilir ve bu fonksiyon un donus degeri SET_HEIGHTS ile atamis oldugumuz footer bosluguna
     * eklenir.Eger footer yoksa bu fonksiyon NULL dondurmeli ve SET_HEIGHTS da footer_height 0 olmalidir.Bu fonksiyon cagirildiktan
     * sonra kernel donus degerini alip sayfaya ekledikten sonra yeni sayfaya gecer.
     */


    virtual QString     GET_BODY                           () = 0;
    /* Bu fonksiyon kernel tarafindan cagrilir ve bu fonksiyon un donus degeri SET_HEIGHTS ile atamis oldugumuz footer
     * ve header bosluklarinin arasina eklenir.Eger footer yada header yoksa onlardan kalan alanada body eklenir. Kernel GET_BODY
     * den NULL donene kadar loop a devam eder.Eger GET_BODY, NULL dondururse kernel raporun bittigini anlar.
     */


    void                CREATE_TABLE                       (QStringList headers,int border_height,int font_size_in_point = 14);
    /* CREATE_TABLE table imizi olusturan fonksiyondur.Fonksiyon headers QStringList indeki eleman sayisi kadar column yaratir
     * ve bu QStringList deki header isimlerine gore table in header satirini doldurur. Ayni zamanda border_height la table in
     * border kalinligi font_size ile de table in font boyutu ayarlanabilir.Eger programci SET_TABLE_TAG_STRING kullanarak table
     * yaratmissa border_height ve font_size 0 gonderilmelidir. Programci bundan sonra ADD_ROW_TO_TABLE kullanarak ekleyecegi satilarin
     * maksimum column sayisi header sayisi kadar olabilir.
     *
     * Ornek;
     * QStringList column_header_list << "Column Basligi 1" << "Column Basligi 2";
     * CREATE_TABLE(column_header_list, 1, 13);
     *
     * Koduyla olusan table su sekildedir.
     *  --------------------------------------------
     *  ---Column Basligi 1 --- Column Basligi 2 ---
     *  --------------------------------------------
     */

    void                ADD_ROW_TO_TABLE                   (QStringList row_datas);
    /* CREATE_TABLE kullanarak bir table yaratiktan sonra bu table eklemek istedigimiz satirlarin degerlerini bir QStringList e koyup
     * ADD_ROW_TO_TABLE fonksiyonu ile table imiza ekleyebiliriz.Ekleyebilecegimiz satirlarin maksimum degerleri CREATE_TABLE
     * kullanilarak yaratmis oldugumuz column sayisindan fazla olamaz.
     *
     * Ornek;
     * QStringList column_header_list << "Column Basligi" << "Column Basligi 2";
     * CREATE_TABLE(column_header_list, 1, 13);
     * QStringList row_datas << "row 1, column 1" << "row 1, column 2";
     * ADD_ROW_TO_TABLE(row_datas);
     *
     * Koduyla olusan table su sekildedir.
     *  --------------------------------------------
     *  ---Column Basligi 1 --- Column Basligi 2 ---
     *  --------------------------------------------
     *  --------------------------------------------
     *  ---row 1, column 1  --- row 1, column 2  ---
     *  --------------------------------------------
     */

    int                 GET_MAX_LINE_COUNT                 (int line_height);
    /* GET_MAX_LINE_COUNT fonksiyonu gonderilen line_height degerine gore
     * raporun bir sayfasina sigacak olan satir sayisini dondurur.Gonderilecek
     * deger point cinsinden olmalidir. Donen deger kadar satir yazdirilirsa
     * rapora tam bir sayfa olur.
     *
     * Ornek; 16punto genisliginden bir font varsa
     * gonderilmesi gereken deger 16punto olmalidir. Ancak 1.5punto border kalinligi olan
     * bi table icine 16punto buyuklugunde bir font konulursa 16punto font genisligi
     * ust ve alttada 1.5 punto lik border var bu durumda gonderilmesi gereken deger
     * 19punto olmalidir.
     *
     *
     *
     * NOT: Eger sayfaya max line count kadar satir yazildiginda sayfa bir sayfadan fazla olursa SET_HEIGHTS ve
     * GET_MAX_LINE_COUNT a gonderilen degerler ile oynanarak tam bir sayfa olmasi saglanabilir.
     */

    void                SET_HEIGHTS                        (int header_height, int footer_height);
    /* SET_HEIGHTS fonksiyonu raporda ki header ve footer genisliklerini ayarlar. Degerler punto
     * cinsinden gonderilmelidir.Bu deger lere gore kernel sayfanin ust ve altindan genislik ayarlar.
     *
     * NOT: Eger sayfada max line count kadar satir yazildiginda sayfa bir sayfadan fazla olursa SET_HEIGHTS ve
     * GET_MAX_LINE_COUNT a gonderilen degerler ile oynanarak tam bir sayfa olmasi saglanabilir.
     */


    void                SET_PAPER_SIZE                     (int    paper_type_enum);
    void                SET_PAPER_SIZE                     (double paper_width , double paper_height);
    /* SET_PAPER_TYPE fonksiyonu kullanilacak paperin turunu belirlemek icin kullanilir.
     * Kernel default olarak A4 e gore calisir.
     *
     * Ornek;
     * SET_PAPER_TYPE(A3);
     *
     * SET_PAPER_TYPE(210.0,23.50);  //Kullanici tanimli sayfa boyutlarinda
     */

    void                SET_FOOTER_POSITION_PROPERTY       (CSS_POSITION_VALUE_NAME footer_position);
    /* Bu fonksiyon footer davranis seklini tanimlar.Mesela eger FIXED gonderilirse footer daima SET_HEIGHTS
     * tanimlanan degerlere gore sabit bir sekilde sayfanin alt kisminda durur. RELATIVE gonderilirse footer
     * bodynin satirlarinin bittigi yerden baslar yani daima bodyden sonra gelir. Kernelda default olarak
     * RELATIVE tanimlidir.
     *
     * Ornek;
     * SET_FOOTER_POSITION_PROPERT (FIXED);
     */

    /*
     *  Yazilacak sayfanin yatay veya dikey olmasine etkiler
     *
     *
     *
     */

    void                SET_ORIENTATION                    ( QPrinter::Orientation p_paper_orientation );

    void                SET_TABLE_TAG_STRING               (QString table_tag_string);
    /* SET_TABLE_TAG_STRING bu fonksiyon programci ihtiyacina gore tabloyu tanimlar.
     * Programci CREATE_TABLE kullanilarak bir table yaratirsa kernel <table border=%1 style=font-size:%2pt;>
     * tagini kullanarak bir table yaratir. Bu table da degiskenler sadece border kalinligi ve font size oldugundan
     * bazi durumlarda programciya yetersiz gelebilir.Bu durumda ihtiyac duyulursa bu fonksiyon kullanilabilir.
     * Bu fonksiyonla programci istedigi sekilde table tanimlayabilir ve bolece CREATE_TABLE fonksiyonunu cagirildiginda tanimlanan
     * table string e gore yeni table yaratilir.
     *
     * Ornek;
     * SET_TABLE_TAG_STRING("<table width=200 height=20 border=1>");
     * Bu sekilde table string imizi tanimlarsak CREATE_TABLE cagrisi ile yaratacagimiz table 200pt genislige ve 20pt yukselige
     * sahip olan ve border kalinligi 1 pt olan bi table yaratiriz.
     */


    void                SET_TD_TAG_STRING                  (QStringList td_tag_string);
    /* SET_TD_TAG_STRING kullanilarak yaratilacak olan table da bulunacak olan columnlarin tum ozellikleri tanimlanabilir.
     * Eger programci bu fonksiyonu cagirmazsa columnlardaki tum degerler standart olarak sola dayali olarak gelir.
     * Programci SET_TD_TAG_STRING kullanarak table daki columnlari tanimlicaksa eger, sirasiyla her column u tanimlamak zorundadir.
     * Columnlari tanimlarken standart olarak birakmak istedigi column icin "" degerini gondermesi yeterlidir.
     *
     * Ornek;
     * QStringList column_headers_list<<"Hesap"      <<"Hesap Ismi"<<"Aciklama"<<"Borc Tutari"            <<"Alacak Tutari";
     * CREATE_TABLE(column_headers_list, 1, 13);
     * SET_TD_TAG_STRING(QStringList()<<"WIDTH = 110"<<""          <<""        <<"WIDTH = 110 align=RIGHT"<<"WIDTH = 110 align=RIGHT");
     * QStringList row_datas          <<"100"        <<"KASA"      <<"..."     <<"20.00"                  <<"";
     * ADD_ROW_TO_TABLE(row_datas);
     *
     * Once header larimizi tutan bir QStringList olusturduk.CREATE_TABLE a bu QStringList i gondererek header_list deki eleman
     * sayisi kadar column olusturduk.Daha sonra SET_TD_TAG_STRING de her bir columnin ozellikleri belirledik.Mesela ilk columnin
     * genisligini 110 yaptik.Sonraki iki column icin "" gondererek ayni kalmasini sagladik.Ve son 2 borc ve alacak columnida
     * saga dayali ve genisligi 110 olan columnlar olarak ayarladik. Ve bundan sonra bu table imiza ekleyecegimiz tum satirlarin
     * ozelligi bu olacak.
     */

    QString             GET_TABLE_HTML_STRING              ();
    /* CREATE_TABLE kullanilarak yaratilan ve ADD_ROW_TO_TABLE ile doldurulan table in QString olarak html kodunu
     * bize dondurur. Bu sekilde table i istedigimiz gibi kullanabiliriz.Mesela table imizi baska bi Html kodu veya
     * daha once yaratigimiz baska bir table ile append ederek header, body yada footer da kullanabiliriz.
     *
     * Ornek;
     * QStringList column_header_list << "Column Basligi" << "Column Basligi 2";
     * CREATE_TABLE(column_header_list, 1, 13);
     * QStringList row_datas << "row 1, column 1" << "row 1, column 2";
     * ADD_ROW_TO_TABLE(row_datas);
     * QStrign table_string = GET_TABLE_HTML_STRING();
     *
     * table_string de ki deger;
     *
     * <table border=1 style=font-size:13pt;>
     * <tr>
     * <td>row 1, column 1</td>
     * <td>row 1, column 2</td>
     * </tr>
     * </table>
     */

    void                SET_WRAP(WRAP_TYPE wrap_type);
    /*
     * Bu fonksiyonla table satiri icin genis olan text lerin kirpilip kirpilmayacagi belirlenir.
     * Eger WRAP gonderilirse text ler bir alt satira gonderilir.Eger NO_WRAP gonderilirse normalde
     * wraplendigi noktadan itibaren ... konularak geri kalan texti hidelar.Default NO_WRAP.
     */

    void                SET_OVERFLOW_TAG(QString overflow_tag);
    /*
     * Bu tag sayfadaki tasma durumlarinda tarayicinin nasil davranacagi belirtilir."hidden" gonderilirse tasma
     * durumunda tarayaci size lara dokunmaz. "auto" gonderilirse tarayici sizelari ekrana gore ayarlar.
     */

    //Bu fonksiyonla bosluk karakterleri html space karakteri olan nbsp; ile degistirilir.
    QString             ADD_HTML_SPACE(QString html_string);

private:
    QString             GET_HTML                           ();

    bool                first_page_passed;

    void                CALCULATE_MAX_LINE_COUNT           (int line_height_in_point);
    void                SET_HEADER                         (QString value);
    void                SET_BODY                           (QString value);
    void                SET_FOOTER                         (QString value);
    void                CREATE_CSS                         ();
    void                CREATE_META_TAG                    ();

    int                 p_header_height;
    int                 p_footer_height;
    int                 p_body_height;
    int                 p_max_body_line_count;
    double              p_paper_width;
    double              p_paper_height;
    int                 p_paper_type_enum;
    int                 table_header_size;

    QStringList         p_td_tag_string;

    QString             p_table_html_string;
    QString             p_footer_position_name;
    QString             p_html_string;
    QString             p_page_html_string;
    QString             p_css_string;
    QString             p_meta_tag_string;
    QString             p_html_end_string;
    QString             p_table_tag_string;
    QString             p_css_wrap_tag;
    QString             p_overflow_tag;

    QString             m_printer_html_string;

    QPrinter::Orientation m_orientation;
};

#endif // REPORT_KERNEL_H

