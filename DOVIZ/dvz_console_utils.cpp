#include <QMessageBox>
#include <QUrl>
#include <QDate>
#include <QNetworkRequest>
#include <QTimer>
#include <QDebug>
#include <QDialog>
#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QStringList>
#include "adak_defines.h"
#include "adak_std_utils.h"
#include "adak_sql.h"
#include "kernel_utils.h"
#include "dvz_struct.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"

extern ADAK_SQL *  DB;

static kur          KURLAR[KUR_ARRAY];
static doviz *      DOVIZLER;

static  int         g_dovizler_count            = 0;
static  int         g_buffer_item_count         = 0;

static  bool        g_doviz_transaction_is_on   = false;

/**************************************************************************************
                   DVZ_DOVIZLERI_GUNCELLE
***************************************************************************************/

void DVZ_DOVIZLERI_GUNCELLE (int host , QDate date)
{
    if (date.isNull() EQ true) {
        date = QDate::currentDate();
    }

    for ( int i = 0 ; i < g_buffer_item_count ; i++ ) {
        if ( KURLAR[g_buffer_item_count].tarih EQ date ) {
            return;
        }
    }

    SQL_QUERY       query(DB);
    QList<QDate>    guncellenecek_tarihler;
    QDate           kullanilacak_tarih = QDate();

    //365 burda herhangi bir hata durumunda sonsuz loop a girmemesi icin onlem olarak verilmistir.
    QDate tarih = date;

    QSplashScreen * splash = CREATE_SPLASH_SCREEN();

    DVZ_DOVIZ_SPLASH_ON(splash , QString("Kur Bilgileri Güncelleştiriliyor..."));

    for ( int i = 0 ; i < 365 ; i++ ) {
        // qDebug("Kur bilgileri icin database kontrol ediliyor.");

        query.PREPARE_SELECT("dvz_kurlar" , "kur_id" , "kur_tarihi = :kur_tarihi");

        query.SET_VALUE(":kur_tarihi" , tarih.toString("yyyy.MM.dd"));

        if ( query.SELECT() NE 0 ) {
            // qDebug("Database de kur bulundu.");
            kullanilacak_tarih   = tarih;
            break;
        }

        // qDebug("Databasede kur bulunamadi.Internet kontrol ediliyor.");
        if ( DVZ_GET_DOVIZ_BILGILERI_INTERNET(tarih,host) EQ true ) {
            // qDebug("Internet de kur bulundu.");
            kullanilacak_tarih = tarih;
            break;
        }

        // qDebug("Internet ve database de kur bulunamadi. Onceki gun e bakiliyor.");
        guncellenecek_tarihler << tarih;
        tarih = tarih.addDays(-1);
    }

    if ( kullanilacak_tarih.isNull() EQ true ) {
        QMessageBox::warning(NULL,QObject::tr("Uyarı"),QObject::tr("Döviz bilgileri güncellemek için internet veya database de gerekli bilgiler bulunamadı.\n"
                                                 "İnternet bağlantısını kontrol ediniz."));

        return;
    }

    // qDebug("Kurlar ekleniyor.");
    for ( int i = 0 ; i < guncellenecek_tarihler.size() ; i++ ) {
        DVZ_GET_DOVIZ_BILGILERI_DATABASE(kullanilacak_tarih,guncellenecek_tarihler.at(i));
    }
    // qDebug("Kurlar eklendi.");

    DVZ_DOVIZ_SPLASH_OFF(splash);

}

/**************************************************************************************
                   DVZ_GET_DOVIZ_BILGILERI_INTERNET
***************************************************************************************/

bool DVZ_GET_DOVIZ_BILGILERI_INTERNET(QDate date , int host)
{
    //Tek kaynak kullaniyor artik.
    host = 0;

    QStringList kur_hosts;
    kur_hosts << "http://www.tcmb.gov.tr";

    DVZ_DOVIZLERI_EKLE ();

    QString xml_path = "";

    if (date EQ QDate::currentDate()) {
        xml_path = QString("%1/kurlar/today.xml").arg(kur_hosts.at(host));
    }
    else {
        xml_path = QString("%1/kurlar/%2/%3.xml").arg(kur_hosts.at(host))
                                                 .arg(date.toString("yyyyMM"))
                                                 .arg(date.toString("ddMMyyyy"));
    }

    const QString html = QVariant ( xml_path ).toByteArray();

    QNetworkAccessManager * network_access_manager = new QNetworkAccessManager();
    QUrl url(html);
    QNetworkReply * network_reply = network_access_manager->get ( QNetworkRequest ( url ) );
    QEventLoop answer_loop;
    answer_loop.connect ( network_reply, SIGNAL ( finished() ), &answer_loop, SLOT ( quit() ) );
    QTimer bekleme_suresi;
    answer_loop.connect ( &bekleme_suresi, SIGNAL ( timeout() ), &answer_loop, SLOT ( quit() ) );
    bekleme_suresi.start ( 10000 );
    answer_loop.exec();
    QByteArray result = network_reply->readAll();

    if ( network_reply->error() != QNetworkReply::NoError ) {
        // qDebug() << "Hata:" << network_reply->errorString();
        return false;
    }

    QStringList dovizler = DVZ_FIND_DOVIZLER_FROM_XML ( result , date );

    DVZ_CREATE_DOVIZ_KURLAR(dovizler);

    if ( dovizler.size() EQ 0 ) {
        return false;
    }

    DVZ_SET_XML_TEMEL_PARA_BIRIMI_ID ( host );

    return true;
}

/**************************************************************************************
                   DVZ_GET_DOVIZ_BILGILERI_INTERNET
***************************************************************************************/

bool DVZ_GET_DOVIZ_BILGILERI_DATABASE(QDate date_orj_kullanilacak, QDate date_kopyalanacak)
{

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dvz_kurlar" , "kur_bilgileri" , "kur_tarihi = :kur_tarihi");

    query.SET_VALUE(":kur_tarihi",date_orj_kullanilacak.toString("yyyy.MM.dd"));

    if ( query.SELECT() EQ 0 ) {
        return false;
    }

    query.NEXT();

    QString kur_bilgileri = query.VALUE(0).toString();

    DVZ_START_DOVIZ_TRANSACTION();

    query.PREPARE_INSERT("dvz_kurlar" , "kur_id" , "kur_bilgileri,kur_tarihi");

    query.SET_VALUE(":kur_bilgileri",QVariant(kur_bilgileri).toByteArray());
    query.SET_VALUE(":kur_tarihi"   ,date_kopyalanacak.toString("yyyy.MM.dd"));
    query.INSERT();

    DVZ_COMMIT_DOVIZ_TRANSACTION();

    return true;
}

/**************************************************************************************
                   DVZ_FIND_DOVIZLER_FROM_XML
***************************************************************************************/

QStringList DVZ_FIND_DOVIZLER_FROM_XML ( QByteArray xml , QDate date )
{
    QStringList dovizler;
    QStringList doviz_kodlari;
    QStringList kur_bilgileri;

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dvz_dovizler" , "doviz_kodu,doviz_id,alis_eklenecek_yuzde,satis_eklenecek_yuzde");

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() ) {
            doviz_kodlari   << query.VALUE(0).toString();
            kur_bilgileri   << QString ( query.VALUE(1).toString() + "\t" + date.toString("yyyy.MM.dd") + "\t" + query.VALUE(2).toString() + "\t" + query.VALUE(3).toString() );
        }
    }

    QDomDocument doviz_xml ( "doviz_xml" );
    doviz_xml.setContent ( xml );

    QDomElement doviz_xml_element = doviz_xml.documentElement();

    QDomNode parent_node = doviz_xml_element.firstChild();

    while ( !parent_node.isNull() ) {
        QString doviz;
        int     data_accured =  0;

        QDomElement parent_element = parent_node.toElement();

        if ( !parent_element.isNull() ) {
            if ( parent_element.tagName() EQ "Currency" ) {
                QString doviz_kodu = parent_element.attribute ( "Kod" );
                doviz.append( doviz_kodu );
                for ( int i = 0 ; i < doviz_kodlari.size() ; i++ ) {
                    if ( doviz_kodlari.at(i) EQ doviz_kodu) {
                        doviz.append( QString ( "\t" + kur_bilgileri.at(i) ) );
                    }
                }
            }

            QDomNode child_node = parent_element.firstChild();
            while ( !child_node.isNull() ){
                QDomElement child_element = child_node.toElement();
                if ( !child_element.isNull() ) {
                    if ( child_element.tagName() EQ "ForexBuying" OR
                         child_element.tagName() EQ "ForexSelling" OR
                         child_element.tagName() EQ "BanknoteBuying" OR
                         child_element.tagName() EQ "BanknoteSelling") {
                        if ( child_element.text().isEmpty() EQ false ) {
                            doviz.append ( QString ( "\t" + child_element.text() ) );
                            data_accured++;
                        }
                    }
                }
                child_node = child_node.nextSibling();
            }

        }
        if ( data_accured EQ 4 ) {
             dovizler << doviz;
        }
        parent_node = parent_node.nextSibling();
    }

    //Temel Para Birimi
    QString temel_para_birimi = "TL";
    for ( int i = 0 ; i < doviz_kodlari.size() ; i++ ) {
        if ( doviz_kodlari.at(i) EQ temel_para_birimi) {
            temel_para_birimi.append( QString ( "\t" + kur_bilgileri.at(i) ) );
            break;
        }
    }
    temel_para_birimi.append("\t1.0\t1.0\t1.0\t1.0\t1.0\t1.0");

    dovizler << temel_para_birimi;

    return dovizler;
}

/**************************************************************************************
                   DVZ_CREATE_UPDATE_KURLAR
***************************************************************************************/

void DVZ_CREATE_DOVIZ_KURLAR(QStringList dovizler)
{
    SQL_QUERY query (DB);

    DVZ_START_DOVIZ_TRANSACTION();

    QString eklenecek_dovizler = "";
    QString kur_tarihi         = "";

    for ( int i = 0 ; i < dovizler.size() ; i++ ) {
        QStringList kur_bilgileri = dovizler.at(i).split("\t");

        if (eklenecek_dovizler.isEmpty() EQ false) {
            eklenecek_dovizler.append("\t");
        }
        else {
            kur_tarihi = kur_bilgileri.at(2);
        }

        double alis_eklenecek_yuzde  = kur_bilgileri.at(3).toDouble();
        double satis_eklenecek_yuzde = kur_bilgileri.at(4).toDouble();
        double yeni_alis_fiyati      = kur_bilgileri.at(5).toDouble() - ( kur_bilgileri.at(5).toDouble() * ( alis_eklenecek_yuzde/100 ) );
        double yeni_satis_fiyati     = kur_bilgileri.at(6).toDouble() * ( 1 + ( satis_eklenecek_yuzde/100 ) );

        eklenecek_dovizler.append(QString ( kur_bilgileri.at(0)                 + "+" + kur_bilgileri.at(1) + "+" +
                                            kur_bilgileri.at(5)                 + "+" + kur_bilgileri.at(6) + "+" +
                                            kur_bilgileri.at(7)                 + "+" + kur_bilgileri.at(8) + "+" +
                                            QVariant(yeni_alis_fiyati).toString() + "+" + QVariant(yeni_satis_fiyati).toString() ) );
    }

    query.PREPARE_SELECT ( "dvz_kurlar","kur_id" ,"kur_tarihi=:kur_tarihi");

    query.SET_VALUE ( ":kur_tarihi", kur_tarihi );

    if ( query.SELECT() EQ 0 ) {

        query.PREPARE_INSERT("dvz_kurlar" , "kur_id" , "kur_tarihi,kur_bilgileri");

        query.SET_VALUE ( ":kur_tarihi"   , kur_tarihi);
        query.SET_VALUE ( ":kur_bilgileri", QVariant(eklenecek_dovizler).toByteArray());

        query.INSERT();
    }
    else {
        query.NEXT();
        int kur_id = query.VALUE(0).toInt();

        query.PREPARE_UPDATE ( "dvz_kurlar","kur_id", "kur_bilgileri", "kur_id=:kur_id" );

        query.SET_VALUE      ( ":kur_bilgileri", QVariant(eklenecek_dovizler).toByteArray() );
        query.SET_VALUE      ( ":kur_id"       , kur_id );
        query.UPDATE();
    }

    DVZ_COMMIT_DOVIZ_TRANSACTION();
}

/**************************************************************************************
                   DVZ_UPDATE_DOVIZ_KURLAR
***************************************************************************************/

void DVZ_UPDATE_DOVIZ_KURLAR(QString doviz_kodu, QDate tarih, double kullanici_alis_fiyati, double kullanici_satis_fiyati)
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dvz_kurlar" , "kur_bilgileri" , "kur_tarihi = :kur_tarihi");

    query.SET_VALUE(":kur_tarihi" , tarih.toString("yyyy.MM.dd"));

    QStringList kurlar;

    if (query.SELECT() NE 0) {
        query.NEXT();
        kurlar = query.VALUE(0).toString().split("\t");
    }

    QString eklenecek_dovizler = "";
    bool    kur_bulundu        = false;

    DVZ_START_DOVIZ_TRANSACTION();

    for ( int i = 0 ; i < kurlar.size() ; i++ ) {
        if (eklenecek_dovizler.isEmpty() EQ false) {
            eklenecek_dovizler.append("\t");
        }

        QStringList kur_bilgileri = kurlar.at(i).split("+");

        if (kur_bilgileri.at(0) NE doviz_kodu) {
            eklenecek_dovizler.append(kurlar.at(i));
            continue;
        }

        query.PREPARE_SELECT("dvz_dovizler" , "alis_eklenecek_yuzde, satis_eklenecek_yuzde" ,
                             "doviz_kodu = :doviz_kodu");

        query.SET_VALUE(":doviz_kodu" , doviz_kodu);

        double alis_eklenecek_yuzde  = kur_bilgileri.at(3).toDouble();
        double satis_eklenecek_yuzde = kur_bilgileri.at(4).toDouble();

        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            alis_eklenecek_yuzde  = query.VALUE(0).toDouble();
            satis_eklenecek_yuzde = query.VALUE(1).toDouble();
        }

        double yeni_alis_fiyati      = kur_bilgileri.at(5).toDouble() - ( kur_bilgileri.at(5).toDouble() * ( alis_eklenecek_yuzde/100 ) );
        double yeni_satis_fiyati     = kur_bilgileri.at(6).toDouble() * ( 1 + ( satis_eklenecek_yuzde/100 ) );

        if ( kullanici_alis_fiyati NE -1 ) {
            yeni_alis_fiyati = kullanici_alis_fiyati;
        }

        if ( kullanici_satis_fiyati NE -1 ) {
            yeni_satis_fiyati = kullanici_satis_fiyati;
        }

        eklenecek_dovizler.append(QString ( kur_bilgileri.at(0)                   + "+" + kur_bilgileri.at(1) + "+" +
                                            kur_bilgileri.at(2)                   + "+" + kur_bilgileri.at(3) + "+" +
                                            kur_bilgileri.at(4)                   + "+" + kur_bilgileri.at(5) + "+" +
                                            QVariant(yeni_alis_fiyati).toString() + "+" + QVariant(yeni_satis_fiyati).toString() ) );

        kur_bulundu = true;
    }

    if (kur_bulundu EQ false) {
        if (eklenecek_dovizler.isEmpty() EQ false) {
            eklenecek_dovizler.append("\t");
        }
        eklenecek_dovizler.append(QString ( doviz_kodu + "+-1+-+-+-+-+" + QVariant(kullanici_alis_fiyati).toString() + "+" + QVariant(kullanici_satis_fiyati).toString() ) );
    }

    query.PREPARE_SELECT("dvz_kurlar" , "kur_id" , "kur_tarihi=:kur_tarihi");

    query.SET_VALUE ( ":kur_tarihi", tarih.toString("yyyy.MM.dd") );

    if ( query.SELECT() EQ 0 ) {

        query.PREPARE_INSERT("dvz_kurlar" , "kur_id" , "kur_tarihi,kur_bilgileri");

        query.SET_VALUE ( ":kur_tarihi"   , tarih.toString("yyyy.MM.dd"));
        query.SET_VALUE ( ":kur_bilgileri", QVariant(eklenecek_dovizler).toByteArray());

        query.INSERT();
    }
    else {
        query.NEXT();
        int kur_id = query.VALUE(0).toInt();

        query.PREPARE_UPDATE ( "dvz_kurlar","kur_id", "kur_bilgileri", "kur_id=:kur_id" );

        query.SET_VALUE      ( ":kur_bilgileri", QVariant(eklenecek_dovizler).toByteArray() );
        query.SET_VALUE      ( ":kur_id"       , kur_id );
        query.UPDATE();
    }

    DVZ_COMMIT_DOVIZ_TRANSACTION();
}

/**************************************************************************************
                    DVZ_PARA_BIRIMLERINI_DONUSTUR
***************************************************************************************/

double DVZ_PARA_BIRIMLERINI_DONUSTUR ( int cevrilen_para_birimi, double cevrilecek_tutar, int cevrilecek_para_birimi,  QDate kur_tarihi, int kur_turu )
{

    if ( cevrilen_para_birimi EQ cevrilecek_para_birimi ) {
        return cevrilecek_tutar;
    }

    SQL_QUERY query ( DB );


    query.PREPARE_SELECT ( "dvz_kurlar","kur_id" ,"kur_tarihi = :kur_tarihi" );
    query.SET_VALUE      ( ":kur_tarihi",kur_tarihi.toString ( "yyyy.MM.dd" ) );

    if ( query.SELECT() EQ 0 ) {
        QString mesaj = QObject::tr("Kur Bilgileri Güncelleştiriliyor...");
        QSplashScreen * splash = CREATE_SPLASH_SCREEN();

        DVZ_DOVIZ_SPLASH_ON(splash,mesaj);
        DVZ_DOVIZLERI_GUNCELLE(ENUM_TCMB_XML,kur_tarihi);
        DVZ_DOVIZ_SPLASH_OFF(splash);
    }

    double cevrilen_doviz_birim_fiyati   = 1.00;
    double cevrilecek_doviz_birim_fiyati = 1.00;
    int    kur_index   = -1;

    for (int i = 0 ; i < g_buffer_item_count ; i++ ) {
        if ( KURLAR[i].tarih EQ kur_tarihi ) {
             kur_index = i;
            break;
        }
    }

    if ( kur_index EQ -1 ) {
        SQL_QUERY query ( DB );

        QStringList doviz_idleri;
        QStringList doviz_kodlari;
        QStringList doviz_adlari;

        query.PREPARE_SELECT ( "dvz_dovizler","doviz_id,doviz_adi,doviz_kodu");

        if (query.SELECT() NE 0) {
            while (query.NEXT() EQ true) {
                doviz_idleri  << query.VALUE(0).toString();
                doviz_adlari  << query.VALUE(1).toString();
                doviz_kodlari << query.VALUE(2).toString();
            }
        }

        query.PREPARE_SELECT("dvz_kurlar" , "kur_bilgileri" , "kur_tarihi=:kur_tarihi");

        query.SET_VALUE      ( ":kur_tarihi",kur_tarihi.toString ( "yyyy.MM.dd" ) );

        if ( query.SELECT() NE 0 ) {
            KURLAR [ g_buffer_item_count ].tarih = kur_tarihi;

            query.NEXT();

            QStringList kurlar = query.VALUE(0).toString().split("\t");

            int doviz_index = 0;

            for ( doviz_index = 0 ; doviz_index < kurlar.size() ; doviz_index++ ) {
                int i;

                QStringList kur_bilgileri = kurlar.at(doviz_index).split("+");

                for ( i = 0 ; i < doviz_adlari.size() ; i++ ) {
                    if (doviz_kodlari.at(i) EQ kur_bilgileri.at(0)) {
                        break;
                    }
                }

                KURLAR [ g_buffer_item_count ].dovizler [ doviz_index ].doviz_id               = doviz_idleri.at(i).toInt();
                KURLAR [ g_buffer_item_count ].dovizler [ doviz_index ].doviz_adi              = doviz_adlari.at(i);
                KURLAR [ g_buffer_item_count ].dovizler [ doviz_index ].doviz_kodu             = kur_bilgileri.at(0);
                KURLAR [ g_buffer_item_count ].dovizler [ doviz_index ].alis_tutari            = kur_bilgileri.at(2).toDouble();
                KURLAR [ g_buffer_item_count ].dovizler [ doviz_index ].satis_tutari           = kur_bilgileri.at(3).toDouble();
                KURLAR [ g_buffer_item_count ].dovizler [ doviz_index ].efektif_alis_tutari    = kur_bilgileri.at(4).toDouble();
                KURLAR [ g_buffer_item_count ].dovizler [ doviz_index ].efektif_satis_tutari   = kur_bilgileri.at(5).toDouble();
                KURLAR [ g_buffer_item_count ].dovizler [ doviz_index ].kullanici_alis_tutari  = kur_bilgileri.at(6).toDouble();
                KURLAR [ g_buffer_item_count ].dovizler [ doviz_index ].kullanici_satis_tutari = kur_bilgileri.at(7).toDouble();
                doviz_index++;
            }
            KURLAR[g_buffer_item_count].doviz_sayisi = doviz_index + 1;
            kur_index = g_buffer_item_count;
            g_buffer_item_count++;
        }
    }

    for ( int i = 0; i < KURLAR [ kur_index ].doviz_sayisi; i++ ) {
        double fiyat = 1.00;

        if ( kur_turu EQ ENUM_ALIS ) {
            fiyat = KURLAR [ kur_index ].dovizler[i].kullanici_alis_tutari;
            if ( fiyat EQ 0.00 ) {
                 fiyat = KURLAR [ kur_index ].dovizler[i].alis_tutari;
            }
        }
        else if  ( kur_turu EQ ENUM_SATIS ) {
            fiyat = KURLAR [ kur_index].dovizler [i].kullanici_satis_tutari;
            if ( fiyat EQ 0 ) {
                 fiyat = KURLAR [ kur_index ].dovizler[i].satis_tutari;
            }
        }
        else if  ( kur_turu EQ ENUM_EFEKTIF_ALIS ) {
            fiyat = KURLAR [ kur_index ].dovizler[i].efektif_alis_tutari;
        }
        else if ( kur_turu EQ ENUM_EFEKTIF_SATIS ) {
            fiyat = KURLAR [ kur_index ].dovizler [i].efektif_satis_tutari;
        }

        //////////////////////////////////////////////////////////////////////////////////

        if ( fiyat EQ 0.00 ) {
             fiyat = 1.00;
        }

        if  ( KURLAR [ kur_index ].dovizler[i].doviz_id EQ cevrilecek_para_birimi ) {
             cevrilecek_doviz_birim_fiyati = fiyat;
        }
        else if ( KURLAR [ kur_index ].dovizler[i].doviz_id EQ cevrilen_para_birimi ) {
            cevrilen_doviz_birim_fiyati   = fiyat;
        }
    }

    double tutar        = cevrilecek_tutar * cevrilen_doviz_birim_fiyati;
    double yeni_tutar   = tutar / cevrilecek_doviz_birim_fiyati;

    return yeni_tutar;
}

/**************************************************************************************
                   DOVIZ_KURUNU_AL
***************************************************************************************/

double DVZ_DOVIZ_KURUNU_AL ( int p_doviz_id, QDate p_kur_tarihi, int p_kur_turu )
{
    return DVZ_PARA_BIRIMLERINI_DONUSTUR ( p_doviz_id, 1.00, DVZ_GET_TEMEL_PARA_BIRIMI_ID(), p_kur_tarihi, p_kur_turu );
}

/**************************************************************************************
                   DVZ_PARITEYI_AL
***************************************************************************************/

double DVZ_PARITEYI_AL ( int p_donusturulecek_para_birimi_id, int p_baz_alinacak_para_birimi_id, QDate p_kur_tarihi, int p_kur_turu )
{
    return DVZ_PARA_BIRIMLERINI_DONUSTUR ( p_baz_alinacak_para_birimi_id, 1.00, p_donusturulecek_para_birimi_id, p_kur_tarihi, p_kur_turu );
}


/**************************************************************************************
                   DVZ_GET_DOVIZ_KODU
***************************************************************************************/

QString DVZ_GET_DOVIZ_KODU ( int doviz_id )
{
    DVZ_DOVIZLER_STRUCT_DOLDUR();

    for ( int i = 0; i < g_dovizler_count ; i++ ) {
        if ( DOVIZLER[i].doviz_id EQ doviz_id ) {
            return DOVIZLER[i].doviz_kodu;
        }
    }

    return "";
}

/**************************************************************************************
                   DVZ_GET_DOVIZ_ADI
**************************************************************************************/

QString DVZ_GET_DOVIZ_ADI ( int doviz_id )
{
    DVZ_DOVIZLER_STRUCT_DOLDUR ();

    for ( int i = 0; i < g_dovizler_count ; i++ ) {
        if ( DOVIZLER [i].doviz_id EQ doviz_id ) {
            return DOVIZLER[i].doviz_adi;
        }
    }

    return "";
}


/**************************************************************************************
                   DVZ_GET_DOVIZ_ID
***************************************************************************************/

int DVZ_GET_DOVIZ_ID ( QString doviz_kodu , bool init )
{
    if ( init EQ true ) {
        DVZ_DOVIZLERI_EKLE ();

        SQL_QUERY query (DB);

        query.PREPARE_SELECT ( "dvz_dovizler","doviz_id", "doviz_kodu=:doviz_kodu" );
        query.SET_VALUE(":doviz_kodu", doviz_kodu);
        if ( query.SELECT() EQ 0 ) {
            return -1;
        }

        query.NEXT();

        return query.VALUE(0).toInt();
    }

    DVZ_DOVIZLER_STRUCT_DOLDUR ();

    for (  int i = 0; i < g_dovizler_count; i++ ) {
        if ( DOVIZLER[i].doviz_kodu EQ doviz_kodu ) {
            return DOVIZLER[i].doviz_id;
        }
    }

    return -1;
}

/**************************************************************************************
                           DVZ_DOVIZLER_STRUCT_DOLDUR
***************************************************************************************/

void DVZ_DOVIZLER_STRUCT_DOLDUR ()
{
    if ( g_dovizler_count NE 0 ) {
        return;
    }

    SQL_QUERY query ( DB );

    //query.PREPARE_LIMIT_SELECT ( "SELECT kur_id FROM dvz_kurlar",0,1,"kur_id" );

    //if ( query.SELECT() EQ 0 ) {
    //    DOVIZLERI_GUNCELLE();
    //}

    query.PREPARE_SELECT ( "dvz_dovizler","doviz_id,doviz_adi,doviz_kodu,dovizi_kullan");

    if ( query.SELECT() NE 0 ) {

        g_dovizler_count = query.NUM_OF_ROWS();
        DOVIZLER = new doviz [ g_dovizler_count ];
        int index = 0;

        while ( query.NEXT() ) {
            DOVIZLER [ index ].doviz_id             = query.VALUE(0).toInt();
            DOVIZLER [ index ].doviz_adi            = query.VALUE(1).toString();
            DOVIZLER [ index ].doviz_kodu           = query.VALUE(2).toString();
            DOVIZLER [ index ].dovizi_kullan        = false;

            if ( query.VALUE(3).toInt() EQ 1 ) {
                DOVIZLER [ index ].dovizi_kullan    = true;
            }
            index++;
        }
    }
}

/**************************************************************************************
                   DVZ_DOVIZLERI_EKLE
***************************************************************************************/

bool DVZ_DOVIZLERI_EKLE()
{
    SQL_QUERY query ( DB );

    int i = 0;

    query.PREPARE_SELECT ( "dvz_dovizler" , "doviz_kodu" );

    if ( query.SELECT() NE 0 ) {
        return true;
    }

    DVZ_START_DOVIZ_TRANSACTION();

    const char ** dovizler_ptr = GET_ADAK_STD_DOVIZLER_STRUCT();

    while ( strcmp( dovizler_ptr[i],"" ) ) {
        query.PREPARE_INSERT("dvz_dovizler" , "doviz_id" ,
                             "doviz_kodu,doviz_adi,alis_eklenecek_yuzde,satis_eklenecek_yuzde,dovizi_kullan");
        query.SET_VALUE(":doviz_kodu"            , QString::fromUtf8( dovizler_ptr[i]));
        query.SET_VALUE(":doviz_adi"             , QString::fromUtf8( dovizler_ptr[i+1]));
        query.SET_VALUE(":alis_eklenecek_yuzde"  , 0);
        query.SET_VALUE(":satis_eklenecek_yuzde" , 0);
        query.SET_VALUE(":dovizi_kullan"         , 0);

        query.INSERT();

        i+=2;
    }

    DVZ_COMMIT_DOVIZ_TRANSACTION();

    return true;
}

/**************************************************************************************
                   DVZ_SET_TEMEL_PARA_BIRIMI_ID
***************************************************************************************/

void DVZ_SET_TEMEL_PARA_BIRIMI_ID ( int para_birimi_id )
{
    DVZ_DOVIZLERI_EKLE();

    DVZ_START_DOVIZ_TRANSACTION();

    SQL_QUERY query ( DB );

    query.PREPARE_UPDATE ( "dvz_degiskenler","degisken_id" ,"temel_para_birimi_id" ,"degisken_id = :degisken_id");
    query.SET_VALUE      ( ":temel_para_birimi_id",para_birimi_id );
    query.SET_VALUE      ( ":degisken_id"      , 1);
    query.UPDATE();

    DVZ_CLEAR_DOVIZLER_STRUCT();

    query.PREPARE_UPDATE ( "dvz_dovizler","doviz_id", "dovizi_kullan", "doviz_kodu = :doviz_kodu");
    query.SET_VALUE(":dovizi_kullan" , 1);
    query.SET_VALUE(":doviz_kodu"    , "TL");
    query.UPDATE();

    query.PREPARE_UPDATE ( "dvz_dovizler","doviz_id", "dovizi_kullan ", "doviz_kodu = :doviz_kodu");
    query.SET_VALUE(":dovizi_kullan"  , 1);
    query.SET_VALUE(":doviz_kodu"     ,"USD");
    query.UPDATE();

    query.PREPARE_UPDATE ( "dvz_dovizler","doviz_id", "dovizi_kullan ", "doviz_kodu = :doviz_kodu");
    query.SET_VALUE(":dovizi_kullan"  , 1);
    query.SET_VALUE(":doviz_kodu"     ,"EUR");
    query.UPDATE();


    DVZ_COMMIT_DOVIZ_TRANSACTION();
}

/**************************************************************************************
                   DVZ_GET_TEMEL_PARA_BIRIMI_ID
***************************************************************************************/

int DVZ_GET_TEMEL_PARA_BIRIMI_ID ( )
{

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dvz_degiskenler" , "temel_para_birimi_id" , "degisken_id = :degisken_id");
    query.SET_VALUE(":degisken_id" , 1);

    if (query.SELECT() EQ 0 ) {
        return -1;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   DVZ_SET_GET_TEMEL_PARA_BIRIMI_KODU
***************************************************************************************/

QString DVZ_GET_TEMEL_PARA_BIRIMI_KODU ()
{
    return DVZ_GET_DOVIZ_KODU ( DVZ_GET_TEMEL_PARA_BIRIMI_ID() );
}

/**************************************************************************************
                   DVZ_SET_XML_TEMEL_PARA_BIRIMI_ID
***************************************************************************************/

void DVZ_SET_XML_TEMEL_PARA_BIRIMI_ID ( int host )
{

    SQL_QUERY query(DB);

    int para_birimi_id = 0;

    if ( host EQ ENUM_TCMB_XML ) {

        query.PREPARE_SELECT("dvz_dovizler" , "doviz_id" , "doviz_kodu = :doviz_kodu" );
        query.SET_VALUE(":doviz_kodu" , "TL");

        if ( query.SELECT() > 0 ) {
            query.NEXT();
            para_birimi_id = query.VALUE(0).toInt();
        }
    }

    DVZ_START_DOVIZ_TRANSACTION();

    query.PREPARE_UPDATE ( "dvz_degiskenler","degisken_id", "xml_temel_para_birimi_id" ,
                           "degisken_id = :degisken_id");
    query.SET_VALUE ( ":xml_temel_para_birimi_id",para_birimi_id );
    query.SET_VALUE ( ":degisken_id"             , 1);
    query.UPDATE();

    DVZ_COMMIT_DOVIZ_TRANSACTION();

}

/**************************************************************************************
                   DVZ_GET_XML_TEMEL_PARA_BIRIMI_ID
**************************************************************************************/

int GET_XML_TEMEL_PARA_BIRIMI_ID()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dvz_degiskenler" , "xml_temel_para_birimi_id" , "degisken_id = :degisken_id");
    query.SET_VALUE(":degisken_id" , 1);

    query.SELECT();

    if (query.SELECT() EQ 0 ) {
        return -1;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   DVZ_CLEAR_DOVIZLER_STRUCT
***************************************************************************************/

void DVZ_CLEAR_DOVIZLER_STRUCT()
{
    if ( g_dovizler_count > 0 ) {
        delete [] DOVIZLER;
    }

    g_dovizler_count = 0;
}

/**************************************************************************************
                   DVZ_START_DOVIZ_TRANSACTION
***************************************************************************************/

void DVZ_START_DOVIZ_TRANSACTION ()
{
    if ( DB->TRANSACTION_IS_ON() EQ false ) {
        DB->START_TRANSACTION();
        g_doviz_transaction_is_on = true;
    }
}

/**************************************************************************************
                           DVZ_COMMIT_DOVIZ_TRANSACTION
***************************************************************************************/

void DVZ_COMMIT_DOVIZ_TRANSACTION ()
{
    if ( g_doviz_transaction_is_on EQ false ) {
        return;
    }

    DB->COMMIT_TRANSACTION();
    g_doviz_transaction_is_on = false;
}

/**************************************************************************************
                   DVZ_CANCEL_DOVIZ_TRANSACTION
***************************************************************************************/

void DVZ_CANCEL_DOVIZ_TRANSACTION ()
{
    if ( g_doviz_transaction_is_on EQ true ) {
        return;
    }

    DB->CANCEL_TRANSACTION();
    g_doviz_transaction_is_on = false;
}

/**************************************************************************************
                   DVZ_DOVIZ_SPLASH_ON
***************************************************************************************/

void DVZ_DOVIZ_SPLASH_ON(QSplashScreen * splash , QString mesaj)
{
    splash->show();

    mesaj.append("Lütfen bekleyiniz...");
    splash->showMessage(mesaj,Qt::AlignCenter , Qt::white);
}

/**************************************************************************************
                   DVZ_DOVIZ_SPLASH_OFF
***************************************************************************************/

void DVZ_DOVIZ_SPLASH_OFF(QSplashScreen *splash)
{
    splash->finish(NULL);
}

/**************************************************************************************
                   DVZ_GET_DOVIZ_BILGILERI
***************************************************************************************/

void DVZ_GET_DOVIZ_BILGILERI( DOVIZ_BILGILERI_STRUCT *P_DOVIZ, int p_doviz_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("dvz_dovizler",
                         "doviz_adi, doviz_kodu, doviz_sembolu, alis_eklenecek_yuzde, satis_eklenecek_yuzde",
                         "doviz_id = :doviz_id");
    query.SET_VALUE(":doviz_id" , p_doviz_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        P_DOVIZ->doviz_adi              = query.VALUE( "doviz_adi" ).toString();
        P_DOVIZ->doviz_kodu             = query.VALUE( "doviz_kodu" ).toString();
        P_DOVIZ->doviz_sembolu          = query.VALUE( "doviz_sembolu" ).toString();
        P_DOVIZ->alis_eklenecek_yuzde   = query.VALUE( "alis_eklenecek_yuzde" ).toDouble();
        P_DOVIZ->satis_eklenecek_yuzde  = query.VALUE( "satis_eklenecek_yuzde" ).toDouble();
    }

}

