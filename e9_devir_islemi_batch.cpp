#include <QDate>
#include "e9_devir_islemi_batch_class.h"
#include "ui_e9_devir_islemi_batch.h"
#include "adak_sql.h"
#include "sql_plugins.h"
#include "muh_enum.h"
#include "banka_enum.h"
#include "cari_enum.h"
#include "cek_enum.h"
#include "login.h"
#include "yonetim.h"
#include "e9_console_utils.h"
#include "e9_log.h"
#include "cari_fis_utils.h"
#include "cek_fis_utils.h"
#include "muh_fis_utils.h"
#include "bnk_fis_utils.h"
#include "e9_onar_utils.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "sube_console_utils.h"

extern ADAK_SQL         * DB;
extern ADAK_SQL         * G_YONETIM_DB;
ADAK_SQL                * ARSIV_DB = NULL;


/**************************************************************************************
                   OPEN_E9_DEVIR_ISLEMI_BATCH
***************************************************************************************/

void OPEN_E9_DEVIR_ISLEMI_BATCH (QWidget * parent)
{
    E9_DEVIR_ISLEMI_BATCH * B = new E9_DEVIR_ISLEMI_BATCH(parent);
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   E9_DEVIR_ISLEMI_BATCH::E9_DEVIR_ISLEMI_BATCH
***************************************************************************************/

E9_DEVIR_ISLEMI_BATCH::E9_DEVIR_ISLEMI_BATCH(QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::E9_DEVIR_ISLEMI_BATCH)
{
    m_ui->setupUi      (this);
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   E9_DEVIR_ISLEMI_BATCH::SETUP_FORM
***************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE      ( tr ( "DEVİR İŞLEMİ" ) );
    SET_HELP_PAGE       ( "yonetim-islemleri_devir-islemi" );

    SET_AUTO_EXIT_BATCH ( true );

    REGISTER_BUTTON_WIDGET(m_ui->widget_batch_buttons);

    SET_SETTING_NAME    ( "E9_DEVIR_ISLEMI_BATCH" );

    m_muhasebe_yili = MALI_YIL();

    m_ui->line_edit_calisan_firma_kodu->setText(VERITABANI_ISMI());
    m_ui->lineedit_calisan_firma_adi->setText(VERITABANI_TANIMI());


    SET_NAME_OF_RUN_BATCH_BUTTON(tr("Devret"));
    SET_ENTER_KEY_FOR_RUN_BATCH(true);

    SET_FIRST_FOCUS_WIDGET(m_ui->searchEdit_arsiv_firma_kodu );

    m_calisan_firma_last_cari_fis_no = 0;
    m_calisan_firma_last_muh_fis_no  = 0;
    m_calisan_firma_last_stk_fis_no  = 0;
    m_calisan_firma_last_cek_fis_no  = 0;
    m_calisan_firma_last_bnk_hareket_fis_no  = 0;

    //! default tarih setleniyor.
    m_ui->adakDate_devir_tarihi->SET_DATE( MALI_YIL_365_DATE().addDays(1));
    m_ui->adakDate_devir_tarihi->setDisabled(true);

    QDate previous_date = m_ui->adakDate_devir_tarihi->QDATE().addDays( -1 );

    m_ui->label_aciklama->setText( m_ui->label_aciklama->text().arg( previous_date.toString( "dd MMMM yyyy dddd" )) );
}

/**************************************************************************************
                   E9_DEVIR_ISLEMI_BATCH::CHECK_VAR
***************************************************************************************/

int E9_DEVIR_ISLEMI_BATCH::CHECK_VAR(QObject * object)
{

    if ( object EQ m_ui->searchEdit_arsiv_firma_kodu ) {

        if ( m_ui->line_edit_calisan_firma_kodu->text() EQ m_ui->searchEdit_arsiv_firma_kodu->GET_TEXT() ) {
            MSG_WARNING( tr( "Aynı Veritabanına yedek alınmaz..!" ), m_ui->searchEdit_arsiv_firma_kodu );
            m_ui->searchEdit_arsiv_firma_kodu->CLEAR();
            m_ui->lineedit_arsiv_firma_adi->clear();
            return ADAK_FAIL;
        }
        if (m_ui->searchEdit_arsiv_firma_kodu->GET_TEXT().isEmpty() EQ true) {
            m_ui->lineedit_arsiv_firma_adi->clear();
            return ADAK_OK;
        }
        m_ui->searchEdit_arsiv_firma_kodu->SET_TEXT(m_ui->searchEdit_arsiv_firma_kodu->GET_TEXT().toLower());


        QString  veritabani_tanimi = VERITABANI_TANIMI(m_ui->searchEdit_arsiv_firma_kodu->GET_TEXT());


        if ( veritabani_tanimi.isEmpty() EQ true ) {
            MSG_WARNING(  tr("Bu kodla kayıtlı firma bulunamadı."), m_ui->searchEdit_arsiv_firma_kodu );
            return ADAK_FAIL;
        }

        m_ui->lineedit_arsiv_firma_adi->setText(veritabani_tanimi);
    }
    else if ( object EQ m_ui->adakDate_devir_tarihi ) {
        QDate previous_date = m_ui->adakDate_devir_tarihi->QDATE().addDays( -1 );
        m_ui->label_aciklama->setText( m_ui->label_aciklama->text().arg( previous_date.toString( "dd MMMMM yyyy" )) );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   E9_DEVIR_ISLEMI_BATCH::CHECK_RUN
***************************************************************************************/

int E9_DEVIR_ISLEMI_BATCH::CHECK_RUN()
{
    if ( m_ui->searchEdit_arsiv_firma_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(  tr("Arşiv firma kodunu girmelisiniz."), m_ui->searchEdit_arsiv_firma_kodu );
        return ADAK_FAIL;
    }
    QString db_name = SQL_DBNAME_ONEK() + "adak_" + m_ui->searchEdit_arsiv_firma_kodu->GET_TEXT();

    ARSIV_DB = CREATE_ADAK_SQL_CONNECTION( GET_PROGRAM_DB_STRUCTS(), db_name, GET_SQL_DB_DRIVER_ENUM( SQL_MOTORU() ));

    if ( ARSIV_DB->CONNECT_TO_DATABASE(true ) EQ ADAK_FAIL ) {

        MSG_WARNING(  tr("Arşiv firmasına bağlanılamadı"), NULL );
        return ADAK_FAIL;
    }


    if ( VERITABANI_BOS_MU( ARSIV_DB ) EQ false ) {
        MSG_WARNING( tr( "Arşiv alacağınız veritabanı boş olmalıdır.!" ), NULL );
        ARSIV_DB->DISCONNECT_TO_DATABASE();
        return ADAK_FAIL;
    }

    int msg_secim = MSG_YES_NO( tr("Eğer Kar-Zarar Fişini oluşturmadıysanız , mizan tutmayabilir. \n Emin misiniz? "), NULL );

    if ( msg_secim EQ ADAK_NO ) {
        return ADAK_FAIL;
    }

    msg_secim =  MSG_YES_NO( tr("Arşivde Devir Yılını içeren fişler silinecek.Yeni Firmaya aktarılacak. \n Onaylıyor musunuz? "), NULL );

    if ( msg_secim EQ ADAK_NO ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   E9_DEVIR_ISLEMI_BATCH::RUN_BATCH
***************************************************************************************/
void E9_DEVIR_ISLEMI_BATCH::RUN_BATCH()
{
    m_muhasebe_yili = m_ui->adakDate_devir_tarihi->QDATE().year()-1;

    m_arsiv_firma_kapanis_tarihi  = m_ui->adakDate_devir_tarihi->QDATE().addDays( -1 );

    m_calisan_firma_acilis_tarihi = m_ui->adakDate_devir_tarihi->QDATE();

    m_ui->plainTextEdit_bilgilendirme->clear();

    DB->START_TRANSACTION();
    ARSIV_DB->START_TRANSACTION();

    ESKI_YILA_AIT_BILGILERI_ARSIV_FIRMAYA_KOPYALA();

    CALISAN_FIRMANIN_MUHASEBE_YILINI_GUNCELLE();

    CALISAN_FIRMADAN_ESKI_YILIN_FISLERINI_SIL();

    CALISAN_FIRMANIN_FIS_NUMARALARINI_GUNCELLE();

    QString log_detaylari = "Çalışan Firma : " + m_ui->line_edit_calisan_firma_kodu->text() + "  "
                            + m_ui->lineedit_calisan_firma_adi->text() + "\nArşiv Firma  : " +
                            m_ui->searchEdit_arsiv_firma_kodu->GET_TEXT() + "  " + m_ui->lineedit_arsiv_firma_adi->text()
                            + "\nDevir Yılı : " + m_ui->adakDate_devir_tarihi->QDATE().year();



    E9_LOG_KAYDI_EKLE (E9_YONETIM , LOG_E9_DEVIR , -1 , log_detaylari);

    DB->COMMIT_TRANSACTION();
    ARSIV_DB->COMMIT_TRANSACTION();

    MSG_INFO( tr("Çalışan Firma , Arşiv Firmaya Devredildi."), NULL );
}

/**************************************************************************************
                  E9_DEVIR_ISLEMI_BATCH::BILGILENDIR
***************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::BILGILENDIR (QString mesaj)
{
    m_ui->plainTextEdit_bilgilendirme->appendHtml(mesaj);
    QCoreApplication::processEvents();
}

/**************************************************************************************
      E9_DEVIR_ISLEMI_BATCH::ESKI_YILA_AIT_BILGILERI_ARSIV_FIRMAYA_KOPYALA
***************************************************************************************/


void E9_DEVIR_ISLEMI_BATCH::ESKI_YILA_AIT_BILGILERI_ARSIV_FIRMAYA_KOPYALA()
{
    /***********************        ADAK_GRUPLAR  *******************/

    E9_COPY_TABLES ( DB , ARSIV_DB , "adak_gruplar" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Gruplar tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES ( DB , ARSIV_DB , "adak_kaydin_gruplari" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Kaydın Grupları,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    /***********************        E9_KULLANICI_YETKILERI  *******************/

    E9_COPY_TABLES(DB , ARSIV_DB , "e9_kullanici_yetkileri","");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - E9 Kullanıcı Yetkileri Tablosu , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));



    /***********************        E9_SABIT_DEGERLER     *******************/

    E9_COPY_TABLES(DB ,ARSIV_DB , "e9_sabit_degerler","");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - E9 Sabit Değerler Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    SUBE_DEPO_UNITE_DEVRET                   ();

    DOVIZ_MODULUNU_DEVRET                    ();

    ADRES_MODULUNU_DEVRET                    ();


    MUHASEBE_MODULUNU_DEVRET                 ();

    CARI_MODULUNU_DEVRET                     ();

    BANKA_MODULUNU_DEVRET                    ();

    CEK_SENET_MODULUNU_DEVRET                ();


    ISLETME_MODULUNU_DEVRET                  ();

    STOK_MODULUNU_DEVRET                     ();

    FATURA_MODULUNU_DEVRET                   ();


    PERAKENDE_MODULUNU_DEVRET                ();

    PERSONEL_MODULUNU_DEVRET                 ();

    DEMIRBAS_MODULUNU_DEVRET                 ();

    SMM_MODULUNU_DEVRET                      ();

    E9_UPDATE_TANIMLANMAMIS_ENT_HESAPLARI    ( ARSIV_DB );

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Eksik entegrasyon hesapları kontrol edildi , düzeltildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_SUBE_ENT_HESAPLARINI_GUNCELLE         ( SUBE_GET_SUBE_ID(),ARSIV_DB );

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Eksik şube entegrasyon hesapları kontrol edildi , düzeltildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    MUHASEBE_MODULUNU_ONAR                   (ARSIV_DB);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada muhasebe hesap bakiyeleri arttırıldı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    CARI_MODULUNU_ONAR                       (ARSIV_DB);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada cari hesap bakiyeleri arttırıldı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BANKA_MODULUNU_ONAR                      (ARSIV_DB);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada banka hesap bakiyeleri arttırıldı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    ISLETME_MODULUNU_ONAR                    (ARSIV_DB);


    /***************************      ENT FISLERI OLUSTUR       ****************/

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Cari Modülü Entegrasyon Fişleri Oluşturuluyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    CARI_ENT_FISLERINI_OLUSTUR      ( E9_PROGRAMI , ARSIV_DB );
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Cari Modülü Entegrasyon Fişleri Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Banka Modülü Entegrasyon Fişleri Oluşturuluyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    BNK_ENT_FISLERINI_OLUSTUR       ( E9_PROGRAMI , ARSIV_DB );
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Banka Modülü Entegrasyon Fişleri Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Çek Senet Modülü Entegrasyon Fişleri Oluşturuluyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    CEK_ENT_FISLERINI_OLUSTUR       ( E9_PROGRAMI , ARSIV_DB );
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Çek Senet Modülü Entegrasyon Fişleri Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Stok Modülü Entegrasyon Fişleri Oluşturuluyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    STK_ENT_FISLERINI_OLUSTUR       ( E9_PROGRAMI , ARSIV_DB );
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Stok Modülü Entegrasyon Fişleri Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Fatura Modülü Entegrasyon Fişleri Oluşturuluyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    FAT_ENT_FISLERINI_OLUSTUR       ( E9_PROGRAMI , ARSIV_DB );
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Fatura Modülü Entegrasyon Fişleri Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Perakende Modülü Entegrasyon Fişleri Oluşturuluyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    PRK_ENT_FISLERINI_OLUSTUR       ( E9_PROGRAMI , ARSIV_DB );
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Perakende Modülü Entegrasyon Fişleri Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Personel Modülü Entegrasyon Fişleri Oluşturuluyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    PRS_ENT_FISLERINI_OLUSTUR       ( E9_PROGRAMI , ARSIV_DB );
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Personel Modülü Entegrasyon Fişleri Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Sabit Kıymet Modülü Entegrasyon Fişleri Oluşturuluyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    DMR_ENT_FISLERINI_OLUSTUR       ( E9_PROGRAMI , ARSIV_DB );
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Sabit Kıymet Modülü Entegrasyon Fişleri Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Sm Makbuz Modülü Entegrasyon Fişleri Oluşturuluyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    SM_MAKBUZ_ENT_FISLERINI_OLUSTUR ( E9_PROGRAMI , ARSIV_DB);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Sm Makbuz Modülü Entegrasyon Fişleri Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    /****************************     ACILIS - KAPANIS FISLERI   ***************/

    CARI_ACILIS_FISI_OLUSTUR                  ();

    BNK_ACILIS_FISI_OLUSTUR                   ();

    CEK_SENET_ACILIS_BORDROSU_OLUSTUR         ();

    STOK_ACILIS_FISI_OLUSTUR                  ();

    MUH_ACILIS_FISI_OLUSTUR                   ();

    MUH_KAPANIS_FISI_OLUSTUR                  ();
}

/**************************************************************************************
          E9_DEVIR_ISLEMI_BATCH::CALISAN_FIRMANIN_MUHASEBE_YILINI_GUNCELLE
***************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CALISAN_FIRMANIN_MUHASEBE_YILINI_GUNCELLE()
{
    SQL_QUERY query(DB);

    QDate e9_array_son_tarih = QDate (m_muhasebe_yili + 1 , 01,01).addDays(E9_ARRAY_SIZE-2);

    query.PREPARE_UPDATE("e9_sabit_degerler",
                         "sabit_deger_id",
                         "mali_yil_ilk_tarih    , "
                         "mali_yil_son_tarih    , "
                         "e9_array_son_tarih     ",
                         "" );

    query.SET_VALUE(":mali_yil_ilk_tarih"       , QVariant (m_muhasebe_yili + 1).toString() + ".01.01");
    query.SET_VALUE(":mali_yil_son_tarih"       , QVariant (m_muhasebe_yili + 1).toString() + ".12.31" );
    query.SET_VALUE(":e9_array_son_tarih"       , e9_array_son_tarih.toString("yyyy.MM.dd"));

    query.UPDATE();

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Çalışan Firmanın mali yılı güncellendi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}


/**************************************************************************************
         E9_DEVIR_ISLEMI_BATCH::CALISAN_FIRMADAN_ESKI_YILIN_FISLERINI_SIL
***************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CALISAN_FIRMADAN_ESKI_YILIN_FISLERINI_SIL()
{
    SQL_QUERY query        ( DB );
    SQL_QUERY query_delete ( DB );
    SQL_QUERY select_query ( DB);

    QString fis_tarihi = m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

    //MUHASEBE MODULU FISLERI

    query.PREPARE_SELECT( "muh_fisler","fis_id ","fis_tarihi < :fis_tarihi");
    query.SET_VALUE(":fis_tarihi"   ,   fis_tarihi);

    if ( query.SELECT() NE 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Muhasebe Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while ( query.NEXT() EQ true ) {
            int fis_id = query.VALUE(0).toInt();

            query_delete.PREPARE_DELETE("muh_fis_satirlari", "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id",fis_id);
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("muh_fisler", "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , fis_id);
            query_delete.DELETE();
        }
    }

    //PERAKENDE FISLERI

    query.PREPARE_SELECT("prk_fisler", "fis_id", "fis_tarihi < :fis_tarihi");
    query.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query.SELECT() NE 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Perakende Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while ( query.NEXT() ) {

            int fis_id = query.VALUE(0).toInt();

            query_delete.PREPARE_DELETE("prk_fis_satirlari", "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , fis_id);
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("prk_fisler","fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , fis_id);
            query_delete.DELETE();
        }
    }



    //ISLETME FISLERI

    query.PREPARE_SELECT("isl_fisi", "fis_id", "fis_tarihi < :fis_tarihi " );
    query.SET_VALUE(":fis_tarihi", fis_tarihi );

    if ( query.SELECT() > 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait İşletme Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while ( query.NEXT() EQ  true ) {

            query_delete.PREPARE_DELETE("isl_fis_satirlari", "fis_id = :fis_id" );
            query_delete.SET_VALUE(":fis_id", query.VALUE(0).toInt());
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("isl_fisi", "fis_id = :fis_id" );
            query_delete.SET_VALUE(":fis_id", query.VALUE(0).toInt());
            query_delete.DELETE();
        }

    }


    //CARI FISLER

    query.PREPARE_SELECT( "car_fisler", "fis_id", "fis_tarihi < :fis_tarihi ");
    query.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query.SELECT() NE 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Cari Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        while (query.NEXT()) {

            query_delete.PREPARE_DELETE("car_fis_satirlari", "fis_id = :fis_id" );
            query_delete.SET_VALUE(":fis_id", query.VALUE(0).toInt());
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("car_fisler", "fis_id = :fis_id" );
            query_delete.SET_VALUE(":fis_id", query.VALUE(0).toInt());
            query_delete.DELETE();

        }
    }

    //BANKA MODULU

    query.PREPARE_SELECT("bnk_defter_fisler",
                         "fis_id   ",
                         "fis_tarihi < :fis_tarihi");
    query.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query.SELECT() NE 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Banka Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while ( query.NEXT() ) {

            query_delete.PREPARE_DELETE("bnk_defter_fis_satirlari ",
                                        "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("bnk_defter_fisler ",
                                        "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

        }
    }

    query.PREPARE_SELECT("bnk_hareket_fisler",
                         "fis_id",
                         "fis_tarihi < :fis_tarihi");
    query.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query.SELECT() NE 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Banka Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while ( query.NEXT() ) {

            query_delete.PREPARE_DELETE("bnk_hareket_fis_satirlari ",
                                        "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("bnk_hareket_fisler ",
                                        "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

        }
    }

    query.PREPARE_SELECT( "bnk_makro_fisler",
                          "fis_id",
                          "fis_tarihi < :fis_tarihi");
    query.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query.SELECT() NE 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Banka Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while ( query.NEXT() ) {

            query_delete.PREPARE_DELETE("bnk_makro_fis_satirlari ",
                                        "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("bnk_makro_fisler ",
                                        " fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

        }
    }

    query.PREPARE_SELECT("bnk_virman_fisler",
                         "fis_id",
                         "fis_tarihi < :fis_tarihi");
    query.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query.SELECT() NE 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Banka Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while ( query.NEXT() ) {

            query_delete.PREPARE_DELETE("bnk_virman_fis_satirlari ",
                                        "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("bnk_virman_fisler ",
                                        "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

        }
    }

    //CEK-SENETLER


    query.PREPARE_SELECT( "cek_cekler_senetler",
                          "cek_senet_id,cek_senet_son_durumu,musteri_ceki_mi    ",
                          "cek_senet_son_durumu != :cek_senet_son_durumu_1      "
                          "AND cek_senet_son_durumu != :cek_senet_son_durumu_2  "
                          "AND cek_senet_son_durumu != :cek_senet_son_durumu_3  "
                          "AND cek_senet_son_durumu != :cek_senet_son_durumu_4  "
                          "AND cek_senet_son_durumu != :cek_senet_son_durumu_5  "
                          "AND cek_senet_son_durumu != :cek_senet_son_durumu_6  " );


    query.SET_VALUE     ( ":cek_senet_son_durumu_1", ENUM_PORTFOYDE);
    query.SET_VALUE     ( ":cek_senet_son_durumu_2", ENUM_BNK_TEMINAT_VERILDI);
    query.SET_VALUE     ( ":cek_senet_son_durumu_3", ENUM_BNK_TAHSILAT_VERILDI);
    query.SET_VALUE     ( ":cek_senet_son_durumu_4", ENUM_KARSILIKSIZ);
    query.SET_VALUE     ( ":cek_senet_son_durumu_5", ENUM_PROTESTOLU);
    query.SET_VALUE     ( ":cek_senet_son_durumu_6", ENUM_SUPHELI_ALACAK);

    if(query.SELECT() NE 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Çek Senet Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while (query.NEXT()) {
            int cek_senet_id         = query.VALUE(0).toInt();
            int cek_senet_son_durumu = query.VALUE(1).toInt();
            int musteri_ceki_mi      = query.VALUE(2).toInt();

            if(CEK_SENET_CALISAN_FIRMADA_ISLEM_GORDU_MU(cek_senet_id) EQ true) {
                continue;
            }

            if(cek_senet_son_durumu EQ ENUM_SATICIYA_VERILDI) {
                if(musteri_ceki_mi EQ 1) {
                    if(CIRO_EDILEN_CEK_ODENDI_KABUL_EDILECEK_MI(cek_senet_id,m_arsiv_firma_kapanis_tarihi) EQ false) {
                        continue;
                    }
                }
                else {
                    continue;
                }
            }


            query_delete.PREPARE_DELETE("cek_cekler_senetler", "cek_senet_id = :cek_senet_id");
            query_delete.SET_VALUE(":cek_senet_id" , cek_senet_id);
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("cek_hareketleri", "cek_senet_id = :cek_senet_id" );
            query_delete.SET_VALUE(":cek_senet_id" , cek_senet_id);
            query_delete.DELETE();
        }
    }

    //CEK BORDROLAR

    query.PREPARE_SELECT("cek_bordrolar",
                         "bordro_id",
                         "bordro_tarihi < :bordro_tarihi ");
    query.SET_VALUE(":bordro_tarihi" , fis_tarihi);

    if(query.SELECT() NE 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Çek Senet Modülü Bordroları,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        while (query.NEXT()) {

            query_delete.PREPARE_DELETE("cek_bordro_satirlari", "bordro_id = :bordro_id" );
            query_delete.SET_VALUE(":bordro_id", query.VALUE(0).toInt());
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("cek_bordrolar", "bordro_id = :bordro_id" );
            query_delete.SET_VALUE(":bordro_id", query.VALUE(0).toInt());
            query_delete.DELETE();

        }
    }

    //STOK FISLER

    query.PREPARE_SELECT("stk_fisler",
                         "fis_id , irsaliye_faturalastirildi_mi,irsaliye_mi_stok_fisi_mi",
                         "fis_tarihi < :fis_tarihi");
    query.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if(query.SELECT() NE 0) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Stok Modülü Fişleri,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while (query.NEXT()) {

            int irsaliye_mi_stok_fisi_mi = query.VALUE(2).toInt();

            if ( irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {

                int irs_faturalastirildi_mi = query.VALUE(1).toInt();

                if ( irs_faturalastirildi_mi EQ 0 ) {
                    continue;
                }
            }

            query_delete.PREPARE_DELETE("stk_fis_satirlari", "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("stk_fisler", "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , query.VALUE(0).toInt());
            query_delete.DELETE();

        }
    }

    //FATURALAR

    query.PREPARE_SELECT("fat_faturalar",
                         "fis_id",
                         "fatura_tarihi < :fatura_tarihi");
    query.SET_VALUE(":fatura_tarihi" , fis_tarihi);

    if(query.SELECT() NE 0) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Arşiv Firmaya Ait Faturalar,Çalışan Firmadan Siliniyor....<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while (query.NEXT()) {

            int fatura_id = query.VALUE(0).toInt();

            select_query.PREPARE_SELECT("fat_fatura_satirlari",
                                        "fis_satiri_id",
                                        "fis_id = :fis_id");
            select_query.SET_VALUE(":fis_id" , fatura_id);

            if(select_query.SELECT() NE 0 ) {
                while (select_query.NEXT()) {
                    query_delete.PREPARE_DELETE("fat_irs_satir_baglantisi", "fatura_satiri_id = :fatura_satiri_id");
                    query_delete.SET_VALUE(":fatura_satiri_id" , select_query.VALUE(0).toInt());
                    query_delete.DELETE();
                }
            }

            query_delete.PREPARE_DELETE("fat_fatura_satirlari", "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , fatura_id);
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("fat_irs_baglantisi", "fatura_id = :fatura_id");
            query_delete.SET_VALUE(":fatura_id" , fatura_id);

            query_delete.PREPARE_DELETE("fat_faturalar", "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , fatura_id);
            query_delete.DELETE();
        }
    }


    //İRSALİYE SERI NUMARALARI

    QStringList islem_gormemis_seri_no_id_list = GET_YILA_GORE_SATIRDAKI_SERI_NUMARALARI_ID_LIST(m_arsiv_firma_kapanis_tarihi);

    for (int i = 0 ; i < islem_gormemis_seri_no_id_list.size() ; i++ ) {
        query_delete.PREPARE_DELETE("stk_satirdaki_seri_nolari", "seri_numarasi_id = :seri_numarasi_id");
        query_delete.SET_VALUE(":seri_numarasi_id" , QVariant(islem_gormemis_seri_no_id_list.at(i)).toString());
        query_delete.DELETE();
    }

    SQL_QUERY query_select(DB);


    //Eger parti calisan firmada islem gormemisse ve 31 aralikta partideki urun miktari 0 ise calisan firmadan silinecek.

    double partiye_giren_urun_miktari[E9_ARRAY_SIZE];
    double partiden_cikan_urun_miktari[E9_ARRAY_SIZE];

    int gun_no = MALI_YIL_ARRAY_INDIS(m_arsiv_firma_kapanis_tarihi);

    query.PREPARE_SELECT( "stk_urunun_partileri", "parti_id");
    if (query.SELECT() NE 0 ) {
        while (query.NEXT()) {
            int parti_id = query.VALUE(0).toInt();

            query_select.PREPARE_SELECT("stk_fis_satirlari,stk_fisler ",
                                        "fis_satiri_id",
                                        "stk_fisler.fis_id = stk_fis_satirlari.fis_id AND "
                                        "parti_id = :parti_id AND fis_tarihi > :fis_tarihi");
            query_select.SET_VALUE(":parti_id"   , parti_id);
            query_select.SET_VALUE(":fis_tarihi" , m_arsiv_firma_kapanis_tarihi.toString("yyyy.MM.dd"));

            if(query_select.SELECT() NE 0 ) {
                continue;
            }
            //31 aralikta partide bulunan urun miktarina bakilacak eger sifir ise parti id calisan firmadan silinecek

            query_select.PREPARE_SELECT("stk_urunun_partileri",
                                        "toplam_partiye_giren_array ,"
                                        "toplam_partiden_cikan_array ",
                                        "parti_id = :parti_id");
            query_select.SET_VALUE(":parti_id" , parti_id);

            if ( query_select.SELECT() NE 0 ) {

                query_select.NEXT();

                UNPACK_DOUBLE_ARRAY(query_select.VALUE(0).toString() , partiye_giren_urun_miktari  , E9_ARRAY_SIZE);
                UNPACK_DOUBLE_ARRAY(query_select.VALUE(1).toString() , partiden_cikan_urun_miktari , E9_ARRAY_SIZE);

                double yil_sonu_partiye_giren_miktar  = partiye_giren_urun_miktari[gun_no];
                double yil_sonu_partiden_cikan_miktar = partiden_cikan_urun_miktari[gun_no];

                double yil_sonu_partideki_miktar      = yil_sonu_partiye_giren_miktar - yil_sonu_partiden_cikan_miktar;

                if(yil_sonu_partideki_miktar NE 0 ) {
                    continue;
                }
                query_delete.PREPARE_DELETE("stk_urunun_partileri", "parti_id = :parti_id");
                query_delete.SET_VALUE(":parti_id" , parti_id);
                query_delete.DELETE();
            }
        }
    }
    //Eger seri_numarasi calisan firmada islem gormemis ise ve depoda degilse calisan firmadan silinecek

    QStringList seri_numaralari_id_list = GET_YILA_GORE_SATIRDAKI_SERI_NUMARALARI_ID_LIST(m_calisan_firma_acilis_tarihi);



    select_query.PREPARE_SELECT( "stk_urunun_seri_nolari", "seri_numarasi_id", "depoda_mi = :depoda_mi");
    select_query.SET_VALUE     ( ":depoda_mi", 0 );

    if ( select_query.SELECT() NE 0 ) {
        while (select_query.NEXT()) {
            int seri_numarasi_id = select_query.VALUE(0).toInt();

            //seri numarasi calisan firmada kullanilmamis ise silinecek.
            if(seri_numaralari_id_list.contains(QVariant(seri_numarasi_id).toString()) EQ true) {
                continue;
            }
            query_delete.PREPARE_DELETE("stk_urunun_seri_nolari", "seri_numarasi_id = :seri_numarasi_id");
            query_delete.SET_VALUE(":seri_numarasi_id" , seri_numarasi_id);
            query_delete.DELETE();
        }
    }

    CALISAN_FIRMADAN_ESKI_YILA_AIT_PRS_BORDROLARI_SIL();

    //DEMIRBAS

    query.PREPARE_SELECT("dmr_fisler" , "fis_id" , "fis_tarihi < :fis_tarihi");
    query.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() ) {
            int fis_id = query.VALUE(0).toInt();

            query_delete.PREPARE_DELETE("dmr_fis_satirlari" , "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , fis_id);
            query_delete.DELETE();

            query_delete.PREPARE_DELETE("dmr_fisler" , "fis_id = :fis_id");
            query_delete.SET_VALUE(":fis_id" , fis_id);
            query_delete.DELETE();
        }
    }

    //SM_MAKBUZ

    query_delete.PREPARE_DELETE("smm_makbuzlar" , "makbuz_tarihi < :makbuz_tarihi");
    query_delete.SET_VALUE(":makbuz_tarihi" , fis_tarihi);
    query_delete.DELETE();

}

/************************************************************************************************/
/*                  E9_DEVIR_ISLEMI_BATCH::CALISAN_FIRMADAN_ESKI_YILA_AIT_PRS_BORDROLARI_SIL();    */
/************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CALISAN_FIRMADAN_ESKI_YILA_AIT_PRS_BORDROLARI_SIL()
{

    SQL_QUERY select_query(DB);
    SQL_QUERY delete_query(DB);

    QString bordro_tarihi = m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

    //PRS BORDROLAR

    select_query.PREPARE_SELECT("prs_bordrolar",
                                "bordro_id , bordro_donemi_ay, bordro_donemi_yil",
                                "bordro_kesim_tarihi < :bordro_kesim_tarihi");
    select_query.SET_VALUE(":bordro_kesim_tarihi" , bordro_tarihi);

    if ( select_query.SELECT() NE 0 ) {

        while ( select_query.NEXT() ) {

            int bordro_id = select_query.VALUE(0).toInt();

            delete_query.PREPARE_DELETE("prs_bordrolar",
                                        "bordro_id = :bordro_id");
            delete_query.SET_VALUE(":bordro_id" , bordro_id);

            delete_query.DELETE();

            delete_query.PREPARE_DELETE("prs_bordro_personelleri ",
                                        "bordro_id = :bordro_id");
            delete_query.SET_VALUE(":bordro_id" , bordro_id);

            delete_query.DELETE();

            delete_query.PREPARE_DELETE("prs_brd_kesinti_odenekleri ",
                                        "bordro_id = :bordro_id");
            delete_query.SET_VALUE(":bordro_id" , bordro_id);

            delete_query.DELETE();

            delete_query.PREPARE_DELETE("prs_ek_kesintiler_odenekler",
                                        "bordro_id = :bordro_id");
            delete_query.SET_VALUE(":bordro_id" , bordro_id);

            delete_query.DELETE();

            delete_query.PREPARE_DELETE("prs_puantaj ",
                                        "bordro_donemi_ay = :bordro_donemi_ay AND bordro_donemi_yil = :bordro_donemi_yil ");
            delete_query.SET_VALUE(":bordro_donemi_ay" , select_query.VALUE( 1 ).toInt() );
            delete_query.SET_VALUE(":bordro_donemi_yil", select_query.VALUE( 2 ).toInt());

            delete_query.DELETE();
        }
    }
}


/************************************************************************************************/
/*                  E9_DEVIR_ISLEMI_BATCH::CALISAN_FIRMANIN_FIS_NUMARALARINI_GUNCELLE();              */
/************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CALISAN_FIRMANIN_FIS_NUMARALARINI_GUNCELLE()
{

    SQL_QUERY query_update(DB);
    SQL_QUERY query_select(DB);

    BILGILENDIR (QString(" <font color = \"red\">%1</font> Çalışan Firmanın , Fiş Numaraları Güncelleniyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    QString fis_tarihi = m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

    //MUHASEBE FISLERI

    query_select.PREPARE_SELECT("muh_fisler", "fis_id,fis_turu","fis_tarihi  >= :fis_tarihi ");
    query_select.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query_select.SELECT( "fis_tarihi,fis_no" ) NE 0 ) {
        while ( query_select.NEXT()) {

            int fis_id         = query_select.VALUE(0).toInt();
            int fis_turu       = query_select.VALUE(1).toInt();



            if ( fis_turu EQ ENUM_ACILIS_FISI ) {
                continue;
            }
            m_calisan_firma_last_muh_fis_no++;

            query_update.PREPARE_UPDATE("muh_fisler", "fis_id", "fis_no",
                                        "fis_id = :fis_id");

            query_update.SET_VALUE(":fis_no"  ,m_calisan_firma_last_muh_fis_no);
            query_update.SET_VALUE(":fis_id"        ,fis_id);
            query_update.UPDATE();
        }
    }
    //Muhasebe modulu fislerinin fis_numaralari guncellendi

    int last_fis_no = 0;

    query_select.PREPARE_SELECT("prk_fisler", "fis_id","fis_tarihi  >= :fis_tarihi " );
    query_select.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query_select.SELECT( "fis_tarihi,fis_no" ) NE 0 ) {
        while ( query_select.NEXT()) {

            int fis_id         = query_select.VALUE(0).toInt();

            last_fis_no++;

            query_update.PREPARE_UPDATE("prk_fisler", "fis_id",  "fis_no",
                                        "fis_id = :fis_id");

            query_update.SET_VALUE(":fis_no"        ,last_fis_no);
            query_update.SET_VALUE(":fis_id"        ,fis_id);
            query_update.UPDATE();
        }
    }
    //Perakende Modulu fislerinin fis_numaralari guncellendi



    query_select.PREPARE_SELECT("car_fisler","fis_id,fis_turu","fis_tarihi  >= :fis_tarihi " );
    query_select.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query_select.SELECT( "fis_tarihi,fis_no" ) NE 0 ) {
        while ( query_select.NEXT()) {

            int fis_id         = query_select.VALUE(0).toInt();
            int fis_turu       = query_select.VALUE(1).toInt();


            if ( fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
                continue;
            }
            m_calisan_firma_last_cari_fis_no++;

            query_update.PREPARE_UPDATE("car_fisler", "fis_id",  "fis_no",
                                        "fis_id = :fis_id");

            query_update.SET_VALUE(":fis_no"        ,m_calisan_firma_last_cari_fis_no);
            query_update.SET_VALUE(":fis_id"        ,fis_id);
            query_update.UPDATE();
        }
    }

    //Cari modulu fislerinin fis numaralari guncellendi

    query_select.PREPARE_SELECT("bnk_hareket_fisler","fis_id,fis_turu","fis_tarihi  >= :fis_tarihi ");
    query_select.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query_select.SELECT( "fis_tarihi,fis_no" ) NE 0 ) {
        while ( query_select.NEXT()) {

            int fis_id   = query_select.VALUE(0).toInt();
            int fis_turu = query_select.VALUE(1).toInt();


            if ( fis_turu EQ ENUM_BANKA_ACILIS_FISI ) {
                continue;
            }

            m_calisan_firma_last_bnk_hareket_fis_no++;

            query_update.PREPARE_UPDATE("bnk_hareket_fisler", "fis_id",  "fis_no",
                                        "fis_id = :fis_id");

            query_update.SET_VALUE(":fis_no"  ,m_calisan_firma_last_bnk_hareket_fis_no);
            query_update.SET_VALUE(":fis_id"  ,fis_id);
            query_update.UPDATE();

        }

    }

    last_fis_no = 0;

    query_select.PREPARE_SELECT("bnk_makro_fisler","fis_id","fis_tarihi  >= :fis_tarihi " );
    query_select.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query_select.SELECT( "fis_tarihi,fis_no" ) NE 0 ) {
        while ( query_select.NEXT()) {

            int fis_id   = query_select.VALUE(0).toInt();


            last_fis_no++;

            query_update.PREPARE_UPDATE("bnk_makro_fisler", "fis_id",  "fis_no",
                                        "fis_id = :fis_id");

            query_update.SET_VALUE(":fis_no"  ,last_fis_no);
            query_update.SET_VALUE(":fis_id"  ,fis_id);
            query_update.UPDATE();

        }

    }

    last_fis_no = 0;

    query_select.PREPARE_SELECT("bnk_virman_fisler","fis_id","fis_tarihi  >= :fis_tarihi ");
    query_select.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query_select.SELECT( "fis_tarihi,fis_no" ) NE 0 ) {
        while ( query_select.NEXT()) {

            int fis_id   = query_select.VALUE(0).toInt();

            last_fis_no++;

            query_update.PREPARE_UPDATE("bnk_virman_fisler", "fis_id",  "fis_no",
                                        "fis_id = :fis_id");

            query_update.SET_VALUE(":fis_no"  ,last_fis_no);
            query_update.SET_VALUE(":fis_id"  ,fis_id);
            query_update.UPDATE();

        }

    }


    //Banka modulu fislerinin fis numaralari guncellendi


    query_select.PREPARE_SELECT("cek_bordrolar", "bordro_id,bordro_turu","bordro_tarihi  >= :bordro_tarihi ");
    query_select.SET_VALUE(":bordro_tarihi" , fis_tarihi);


    if(query_select.SELECT( "bordro_tarihi,bordro_no" ) NE 0 ) {
        while ( query_select.NEXT()) {

            int bordro_id   = query_select.VALUE(0).toInt();
            int bordro_turu = query_select.VALUE(1).toInt();


            if(bordro_turu EQ ENUM_CEK_SENET_ACILIS_BORDROSU) {
                continue;
            }

            m_calisan_firma_last_cek_fis_no++;


            query_update.PREPARE_UPDATE("cek_bordrolar", "bordro_id",  "bordro_no",
                                        "bordro_id = :bordro_id");

            query_update.SET_VALUE(":bordro_no"  ,m_calisan_firma_last_cek_fis_no);
            query_update.SET_VALUE(":bordro_id"        ,bordro_id);
            query_update.UPDATE();
        }

    }
    //Cek senet Modulu fislerinin bordro numaralari guncellendi


    int calisan_firma_last_isl_fis_no = 0;

    query_select.PREPARE_SELECT("isl_fisi","fis_id","fis_tarihi  >= :fis_tarihi ");
    query_select.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if (query_select.SELECT( "fis_tarihi,fis_no" ) NE 0 ) {

        while ( query_select.NEXT() EQ true ) {

            int fis_id         = query_select.VALUE(0).toInt();


            calisan_firma_last_isl_fis_no++;

            query_update.PREPARE_UPDATE("isl_fisi", "fis_id",  "fis_no",
                                        "fis_id = :fis_id");

            query_update.SET_VALUE(":fis_no"    ,calisan_firma_last_isl_fis_no);
            query_update.SET_VALUE(":fis_id"    ,fis_id);
            query_update.UPDATE();

        }
    }

    //isletme modulu fislerinin fis_numaralari guncellendi



    query_select.PREPARE_SELECT("stk_fisler","fis_id","fis_tarihi > :fis_tarihi ");
    query_select.SET_VALUE(":fis_tarihi" ,  fis_tarihi);

    if(query_select.SELECT( "fis_tarihi,fis_no" ) NE 0 ) {
        while (query_select.NEXT()) {
            int fis_id         = query_select.VALUE(0).toInt();

            m_calisan_firma_last_stk_fis_no++;

            query_update.PREPARE_UPDATE("stk_fisler", "fis_id",  "fis_no",
                                        "fis_id = :fis_id");


            query_update.SET_VALUE(":fis_no"    ,m_calisan_firma_last_stk_fis_no);
            query_update.SET_VALUE(":fis_id"          ,fis_id);
            query_update.UPDATE();


        }
    }

    last_fis_no = 0;

    query_select.PREPARE_SELECT("dmr_fisler" , "fis_id" , "fis_tarihi > :fis_tarihi");
    query_select.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query_select.SELECT("fis_tarihi , fis_no") NE 0 ) {

        while ( query_select.NEXT() ) {
            int fis_id = query_select.VALUE(0).toInt();

            last_fis_no++;

            query_update.PREPARE_UPDATE("dmr_fisler" , "fis_id" , "fis_no" , "fis_id = :fis_id");

            query_update.SET_VALUE(":fis_no" , last_fis_no);
            query_update.SET_VALUE(":fis_id" , fis_id);

            query_update.UPDATE();
        }
    }

    last_fis_no = 0;

    query_select.PREPARE_SELECT("smm_makbuzlar" , "makbuz_id" , "makbuz_tarihi >:makbuz_tarihi");
    query_select.SET_VALUE(":makbuz_tarihi" , fis_tarihi);

    if ( query_select.SELECT("makbuz_tarihi, makbuz_no") NE 0 ) {

        while ( query_select.NEXT() ) {

            int makbuz_id = query_select.VALUE(0).toInt();

            last_fis_no++;

            query_update.PREPARE_UPDATE("smm_makbuzlar" , "makbuz_id" , "makbuz_no" ,
                                        "makbuz_id = :makbuz_id");

            query_update.SET_VALUE(":makbuz_no" , last_fis_no);
            query_update.SET_VALUE(":makbuz_id" , makbuz_id);

            query_update.UPDATE();
        }
    }

    last_fis_no = 0;

    query_select.PREPARE_SELECT("fat_faturalar","fis_id","fatura_tarihi > :fatura_tarihi" );
    query_select.SET_VALUE(":fatura_tarihi" , fis_tarihi);

    if(query_select.SELECT( "fatura_tarihi,fis_no" ) EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> Çalışan Firmanın Fiş Numaraları Güncellendi.Devir İşlemi Tamamlandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }
    while (query_select.NEXT()) {
        int fatura_id = query_select.VALUE(0).toInt();


        last_fis_no++;

        query_update.PREPARE_UPDATE("fat_faturalar", "fis_id",  "fis_no",
                                    "fis_id = :fis_id");

        query_update.SET_VALUE(":fis_no" , last_fis_no);
        query_update.SET_VALUE(":fis_id" , fatura_id);

        query_update.UPDATE();
    }
    BILGILENDIR (QString(" <font color = \"red\">%1</font> Çalışan Firmanın Fiş Numaraları Güncellendi.Devir İşlemi Tamamlandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/**********************************************************************************************/
/*                 E9_DEVIR_ISLEMI_BATCH::ADRES_MODULUNU_DEVRET();                             */
/**********************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::ADRES_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** ADRES MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Adres Modülü,Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB , ARSIV_DB , "adr_adresler" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Adres Modülündeki Adresler Tablosu , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB , ARSIV_DB , "adr_telefonlar" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Adres Modülü Telefonlar Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB , ARSIV_DB , "adr_emailler" ,"");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Adres Modülü Emailler Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB ,ARSIV_DB , "adr_yetkili_bilgileri" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Adres Modülü Yetkili Bilgileri Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB, ARSIV_DB , "adr_not_defteri" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Adres Modülü Not Defteri Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Adres Modülü , Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}


/**************************************************************************************************/
/*                 E9_DEVIR_ISLEMI_BATCH::BANKA_MODULUNU_DEVRET();                              */
/**************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::BANKA_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** BANKA MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülü,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_hesabin_kredi_kartlari" , "");
    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_kk_arrayler"            , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Kredi Kartları , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));



    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_hesabin_poslari" , "");
    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_pos_arrayler"    , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Posları , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_hesaplar" , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Hesapları , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_makrolar" , "");
    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_makro_satirlari" , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülü Makroları , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Hesap Bakiyeleri Sıfırlanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    QStringList column_names;

    column_names << "toplam_borc_array" << "toplam_alacak_array";


    HESAP_BAKIYELERINI_SIFIRLA("bnk_hesaplar" ,"hesap_no_id" ,column_names ,6 , ARSIV_DB );


    column_names.clear();

    column_names << "toplam_kk_tahsilat_borc_array" << "toplam_kk_thslat_alacak_array";

    HESAP_BAKIYELERINI_SIFIRLA("bnk_pos_arrayler" ,"kayit_id" , column_names , 6 , ARSIV_DB);


    column_names.clear();

    column_names << "toplam_kk_odeme_borc_array" << "toplam_kk_odeme_alacak_array";

    HESAP_BAKIYELERINI_SIFIRLA("bnk_kk_arrayler" ,"kayit_id" , column_names , 6 , ARSIV_DB);


    BNK_FISLERINI_KOPYALA                     ();


    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülü,Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/************************************************************************************************/
/*             E9_DEVIR_ISLEMI_BATCH::BNK_FISLERINI_KOPYALA();                               */
/************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::BNK_FISLERINI_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);

    QStringList where_list;
    QStringList line_where_list;

    QString where_str = QString("fis_tarihi < :arg_%1 AND ( modul_id = :arg_%2 AND program_id = :arg_%3 )"
                                " OR ( program_id != :arg_%4 )").arg(where_list.size())
                                                          .arg(where_list.size() + 1)
                                                          .arg(where_list.size() + 2)
                                                          .arg(where_list.size() + 3);

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");
    where_list << QString::number(BANKA_MODULU)<< QString::number(E9_PROGRAMI) << QString::number(E9_PROGRAMI);


    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_hareket_fisler" , where_str , where_list);

    select_devredilecek_firma.PREPARE_SELECT("bnk_hareket_fisler",
                                             "fis_id",
                                             "fis_tarihi < :fis_tarihi AND "
                                             "(modul_id = :modul_id AND program_id = :program_id_1) OR "
                                             "(program_id != :program_id_2)");

    select_devredilecek_firma.SET_VALUE(":fis_tarihi"   , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd") );
    select_devredilecek_firma.SET_VALUE(":modul_id"     , BANKA_MODULU );
    select_devredilecek_firma.SET_VALUE(":program_id_1" , E9_PROGRAMI);
    select_devredilecek_firma.SET_VALUE(":program_id_2" , E9_PROGRAMI);

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülünde Kayıtlı Hareket Fişi Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    }
    else {

        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülü Hareket Fişleri,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while (select_devredilecek_firma.NEXT()) {

            line_where_list.clear();

            line_where_list << select_devredilecek_firma.VALUE(0).toString();

            E9_COPY_TABLES ( DB , ARSIV_DB , "bnk_hareket_fis_satirlari" , QString("fis_id = :arg_%1").arg(0),line_where_list);

        }

        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülü Hareket Fişleri , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    }




    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_makro_fisler" , where_str , where_list);

    select_devredilecek_firma.PREPARE_SELECT("bnk_makro_fisler",
                                             "fis_id ",
                                             "fis_tarihi < :fis_tarihi AND "
                                             "(modul_id = :modul_id AND program_id = :program_id_1) OR "
                                             "(program_id != :program_id_2)");
    select_devredilecek_firma.SET_VALUE(":fis_tarihi"   , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd") );
    select_devredilecek_firma.SET_VALUE(":modul_id"     , BANKA_MODULU );
    select_devredilecek_firma.SET_VALUE(":program_id_1" , E9_PROGRAMI);
    select_devredilecek_firma.SET_VALUE(":program_id_2" , E9_PROGRAMI);

    if ( select_devredilecek_firma.SELECT() NE 0 ) {

        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülü Makro Fişleri,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        while ( select_devredilecek_firma.NEXT() ) {

            line_where_list.clear();

            line_where_list << select_devredilecek_firma.VALUE(0).toString();

            E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_makro_fis_satirlari" , QString("fis_id = :arg_%1").arg(0) , line_where_list);

        }
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülü Makro Fişleri,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    }
    else {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülünde Kayıtlı Makro Fişi Bulunmamaktadır<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    }



    E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_virman_fisler" , where_str , where_list);


    select_devredilecek_firma.PREPARE_SELECT("bnk_virman_fisler","fis_id",
                                             "fis_tarihi < :fis_tarihi AND "
                                             "(modul_id = :modul_id AND program_id = :program_id_1) OR "
                                             "(program_id != :program_id_2)");

    select_devredilecek_firma.SET_VALUE(":fis_tarihi"   , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd") );
    select_devredilecek_firma.SET_VALUE(":modul_id"     , BANKA_MODULU );
    select_devredilecek_firma.SET_VALUE(":program_id_1" , E9_PROGRAMI);
    select_devredilecek_firma.SET_VALUE(":program_id_2" , E9_PROGRAMI);

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {

        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülünde Kayıtlı Virman Fişi Bulunmamaktadır<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }
    while ( select_devredilecek_firma.NEXT() ) {

        line_where_list.clear();

        line_where_list << select_devredilecek_firma.VALUE(0).toString();

        E9_COPY_TABLES(DB ,ARSIV_DB , "bnk_virman_fis_satirlari" , QString("fis_id = :arg_%1").arg(0) , line_where_list);

    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülü Virman Fişleri,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}


/***************************************************************************************/
/*                 E9_DEVIR_ISLEMI_BATCH::BNK_ACILIS_FISI_OLUSTUR();                  */
/***************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::BNK_ACILIS_FISI_OLUSTUR ()
{

    SQL_QUERY query_arsiv_db(ARSIV_DB);
    SQL_QUERY s_query_arsiv_db (ARSIV_DB);


    query_arsiv_db.PREPARE_SELECT("bnk_hesaplar" ,
                                   "hesap_no_id,doviz_id,toplam_borc_array,"
                                  "toplam_alacak_array ", "");

    if ( query_arsiv_db.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülünde , Açılış Fişi Oluşturulamadı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülünde , Açılış Fişi Oluşturuluyor.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));



    double borc_array   [E9_ARRAY_SIZE];
    double alacak_array [E9_ARRAY_SIZE];

    double pos_borc_array[E9_ARRAY_SIZE];
    double pos_alacak_array[E9_ARRAY_SIZE];

    double kk_borc_array[E9_ARRAY_SIZE];
    double kk_alacak_array[E9_ARRAY_SIZE];

    BNK_HAREKET_FIS_STRUCT         * BNK_HRK_FIS        = new BNK_HAREKET_FIS_STRUCT;
    BNK_HAREKET_FIS_SATIRI_STRUCT  * BNK_HRK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

    int gun_no = MALI_YIL_ARRAY_INDIS( m_arsiv_firma_kapanis_tarihi );

    double borc_tutari       = 0.0;
    double alacak_tutari     = 0.0;

    double pos_borc_tutari   = 0.0;
    double pos_alacak_tutari = 0.0;

    double kk_borc_tutari    = 0.0;
    double kk_alacak_tutari  = 0.0;

    while ( query_arsiv_db.NEXT() ) {

        int hesap_no_id = query_arsiv_db.VALUE("hesap_no_id").toInt();

        UNPACK_DOUBLE_ARRAY(query_arsiv_db.VALUE("toplam_borc_array").toString(),borc_array , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(query_arsiv_db.VALUE("toplam_alacak_array").toString() , alacak_array , E9_ARRAY_SIZE);

        s_query_arsiv_db.PREPARE_SELECT("bnk_pos_arrayler" , "toplam_kk_tahsilat_borc_array,"
                                        "toplam_kk_tahsilat_alacak_array" , "hesap_no_id = :hesap_no_id");
        s_query_arsiv_db.SET_VALUE(":hesap_no_id" , hesap_no_id);

        if ( s_query_arsiv_db.SELECT() > 0 ) {
            s_query_arsiv_db.NEXT();

            UNPACK_DOUBLE_ARRAY(s_query_arsiv_db.VALUE("toplam_kk_tahsilat_borc_array").toString(),pos_borc_array , E9_ARRAY_SIZE);
            UNPACK_DOUBLE_ARRAY(s_query_arsiv_db.VALUE("toplam_kk_tahsilat_alacak_array").toString() , pos_alacak_array , E9_ARRAY_SIZE);

        }
        s_query_arsiv_db.PREPARE_SELECT("bnk_kk_arrayler" , "toplam_kk_odeme_borc_array,"
                                        "toplam_kk_odeme_alacak_array" , "hesap_no_id = :hesap_no_id");
        s_query_arsiv_db.SET_VALUE(":hesap_no_id" , hesap_no_id);

        if ( s_query_arsiv_db.SELECT() > 0 ) {

            s_query_arsiv_db.NEXT();

            UNPACK_DOUBLE_ARRAY(s_query_arsiv_db.VALUE("toplam_kk_odeme_borc_array").toString() , kk_borc_array , E9_ARRAY_SIZE);
            UNPACK_DOUBLE_ARRAY(s_query_arsiv_db.VALUE("toplam_kk_odeme_alacak_array").toString() , kk_alacak_array , E9_ARRAY_SIZE);

        }

        borc_tutari       = 0;
        alacak_tutari     = 0;

        pos_borc_tutari   = 0;
        pos_alacak_tutari = 0;

        kk_borc_tutari    = 0;
        kk_alacak_tutari  = 0;

        if ( borc_array[gun_no] > alacak_array[gun_no] ) {
            borc_tutari       = ROUND(borc_array[gun_no]) - ROUND(alacak_array[gun_no]);
        }
        else {
            alacak_tutari     = ROUND(alacak_array[gun_no]) - ROUND(borc_array[gun_no]);
        }

        if ( pos_borc_array[gun_no] > pos_alacak_array[gun_no] ) {
            pos_borc_tutari   = ROUND(pos_borc_array [gun_no]) - ROUND(pos_alacak_array[gun_no]);
        }
        else {
            pos_alacak_tutari = ROUND(pos_alacak_array[gun_no]) - ROUND(pos_borc_array[gun_no]);
        }

        if ( kk_borc_array[gun_no] > kk_alacak_array[gun_no] ) {
            kk_borc_tutari    = ROUND(kk_borc_array[gun_no]) - ROUND(kk_alacak_array[gun_no]);
        }
        else {
            kk_alacak_tutari  = ROUND(kk_alacak_array[gun_no]) - ROUND(kk_borc_array[gun_no]);
        }

        if ( (borc_tutari EQ 0 AND alacak_tutari EQ 0) AND
             (pos_borc_tutari EQ 0 AND pos_alacak_tutari EQ 0 ) AND
             (kk_borc_tutari EQ 0 AND kk_alacak_tutari EQ 0) ) {
            continue;
        }

        if ( ( borc_tutari EQ alacak_tutari ) AND (pos_borc_tutari EQ pos_alacak_tutari)
                AND (kk_borc_tutari EQ kk_alacak_tutari) ) {
            continue;
        }


        BNK_CLEAR_HAREKET_FIS_STRUCT( BNK_HRK_FIS );
        BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT ( BNK_HRK_FIS_SATIRI );

        m_calisan_firma_last_bnk_hareket_fis_no++;

        BNK_HRK_FIS->program_id                       = E9_PROGRAMI;
        BNK_HRK_FIS->modul_id                         = BANKA_MODULU;
        BNK_HRK_FIS->base_fis_id                      = 0;
        BNK_HRK_FIS->fis_tarihi                       = m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");
        BNK_HRK_FIS->aciklama                         = QObject::tr("BANKA AÇILIŞ FİŞİ - Devir İşlemi Esnasında Otomatik Olarak Üretilmiştir.");
        BNK_HRK_FIS->fis_turu                         = ENUM_BANKA_ACILIS_FISI;
        BNK_HRK_FIS->base_fis_turu                    = ENUM_BANKA_ACILIS_FISI;
        BNK_HRK_FIS->muh_fis_id                       = 999; // hareket 0 oldugunda muh fis uretiyor rasgele deger gonderdim
        BNK_HRK_FIS->odm_emr_hesabi_etkilenecek_mi    = 1;
        BNK_HRK_FIS->hesap_no_id                      = hesap_no_id;
        BNK_HRK_FIS->def_fis_id                       = 0;
        BNK_HRK_FIS->para_birim_id                    = query_arsiv_db.VALUE("doviz_id").toInt();

        int bnk_fis_id = BNK_HAREKET_FIS_EKLE(BNK_HRK_FIS ,0,true);

        BNK_HRK_FIS_SATIRI->fis_id            = bnk_fis_id;
        BNK_HRK_FIS_SATIRI->program_id        = E9_PROGRAMI;
        BNK_HRK_FIS_SATIRI->modul_id          = BANKA_MODULU;
        BNK_HRK_FIS_SATIRI->base_record_id    = 0;

        if ( borc_tutari NE 0 OR alacak_tutari NE 0 ) {

            BNK_HRK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
            BNK_HRK_FIS_SATIRI->etkilenecek_hesap_id   = 0;

            BNK_HRK_FIS_SATIRI->hesaba_giren   = borc_tutari;
            BNK_HRK_FIS_SATIRI->hesaptan_cikan = alacak_tutari;

            BNK_HAREKET_FIS_SATIRI_EKLE ( BNK_HRK_FIS, BNK_HRK_FIS_SATIRI ,0,true );

        }

        if ( pos_borc_tutari NE 0 OR pos_alacak_tutari NE 0 ) {

            BNK_HRK_FIS_SATIRI->etkilenecek_hesap_turu    = ENUM_BNK_POS_HESABI;
            BNK_HRK_FIS_SATIRI->etkilenecek_hesap_id      = 0;
            BNK_HRK_FIS_SATIRI->base_record_id            = 0;

            BNK_HRK_FIS_SATIRI->hesaba_giren              = pos_borc_tutari;
            BNK_HRK_FIS_SATIRI->hesaptan_cikan            = pos_alacak_tutari;

            BNK_HAREKET_FIS_SATIRI_EKLE ( BNK_HRK_FIS, BNK_HRK_FIS_SATIRI ,0,true);
        }

        if ( kk_borc_tutari NE 0 OR kk_alacak_tutari NE 0 ) {

            BNK_HRK_FIS_SATIRI->etkilenecek_hesap_turu    = ENUM_BNK_KREDI_KARTI_HESABI;
            BNK_HRK_FIS_SATIRI->etkilenecek_hesap_id      = 0;
            BNK_HRK_FIS_SATIRI->base_record_id            = 0;

            BNK_HRK_FIS_SATIRI->hesaba_giren              = kk_borc_tutari;
            BNK_HRK_FIS_SATIRI->hesaptan_cikan            = kk_alacak_tutari;


            BNK_HAREKET_FIS_SATIRI_EKLE ( BNK_HRK_FIS, BNK_HRK_FIS_SATIRI ,0,true);
        }


    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Banka Modülünde , Açılış Fişi Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/*************************************************************************************************/
/*                   E9_DEVIR_ISLEMI_BATCH::CARI_MODULUNU_DEVRET();                             */
/*************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CARI_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** CARİ MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Modülü , Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB ,ARSIV_DB , "car_hesaplar" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Hesaplar Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    //Cari Hesaplar tablosu sifirlanmalidir.

    QStringList column_names;

    column_names << "toplam_borc_array" << "toplam_alacak_array";


    HESAP_BAKIYELERINI_SIFIRLA("car_hesaplar" , "cari_hesap_id" , column_names ,  6 , ARSIV_DB);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Hesap Bakiyeleri Sıfırlandı<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    CARI_FISLERI_KOPYALA();

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Modülü , Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}
/*************************************************************************************************/
/*                 E9_DEVIR_ISLEMI_BATCH::CARI_FISLERI_KOPYALA();                               */
/*************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CARI_FISLERI_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);

    //E9 programina ait olan cari modulu entegrasyon fisleri devredilmeyecek

    QStringList where_list;

    QString where_str = QString("fis_tarihi < :arg_%1 AND ( modul_id = :arg_%2 AND program_id = :arg_%3 )"
                                "OR ( program_id != :arg_%4 )").arg(where_list.size())
                                                          .arg(where_list.size() + 1)
                                                          .arg(where_list.size() + 2)
                                                          .arg(where_list.size() + 3);

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");
    where_list << QString::number(CARI_MODULU)<< QString::number(E9_PROGRAMI) << QString::number(E9_PROGRAMI);


    E9_COPY_TABLES(DB ,ARSIV_DB , "car_fisler" , where_str , where_list);


    select_devredilecek_firma.PREPARE_SELECT("car_fisler","fis_id",
                                             "fis_tarihi < :fis_tarihi AND "
                                             "(modul_id = :modul_id AND program_id = :program_id_1) OR "
                                             "(program_id != :program_id_2)");

    select_devredilecek_firma.SET_VALUE(":fis_tarihi"   , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd") );
    select_devredilecek_firma.SET_VALUE(":modul_id"     , CARI_MODULU );
    select_devredilecek_firma.SET_VALUE(":program_id_1" , E9_PROGRAMI);
    select_devredilecek_firma.SET_VALUE(":program_id_2" , E9_PROGRAMI);


    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Fişler Tablosunda Kayıt Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return ;
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Modülü Fişleri,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    QStringList where_line_list;

    while ( select_devredilecek_firma.NEXT()) {

        where_line_list.clear();

        where_line_list << select_devredilecek_firma.VALUE(0).toString();

        E9_COPY_TABLES(DB , ARSIV_DB , "car_fis_satirlari" , QString("fis_id = :arg_%1").arg(0) , where_line_list);

    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Modülü Fişleri , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/********************************************************************/
/*       E9_DEVIR_ISLEMI_BATCH::CARI_ACILIS_FISI_OLUSTUR();         */
/********************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CARI_ACILIS_FISI_OLUSTUR( )
{

    SQL_QUERY select_arsiv_firma(ARSIV_DB);

    select_arsiv_firma.PREPARE_SELECT("car_hesaplar " , "cari_hesap_id,toplam_borc_array , toplam_alacak_array");

    if (select_arsiv_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Modülünde , Açılış Fişi Oluşturulamadı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    m_calisan_firma_last_cari_fis_no++;

    CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;

    CARI_CLEAR_FIS_STRUCT(CARI_FIS);

    CARI_FIS->fis_no      = m_calisan_firma_last_cari_fis_no;
    CARI_FIS->fis_tarihi  = m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");
    CARI_FIS->aciklama    = tr("CARİ AÇILIŞ FİŞİ - Devir İşlemi Esnasında Otomatik Olarak Üretilmiştir.");
    CARI_FIS->fis_turu    = ENUM_CARI_ACILIS_FISI;
    CARI_FIS->modul_id    = CARI_MODULU;
    CARI_FIS->program_id  = E9_PROGRAMI;

    int cari_acilis_fisi_id = CARI_FIS_EKLE( CARI_FIS );

    int gun_no = MALI_YIL_ARRAY_INDIS( m_arsiv_firma_kapanis_tarihi );

    double      toplam_borc[E9_ARRAY_SIZE];
    double      toplam_alacak[E9_ARRAY_SIZE];

    CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI = new CARI_FIS_SATIRI_STRUCT;
    CARI_CLEAR_FIS_SATIRI_STRUCT(CARI_FIS_SATIRI);

    CARI_FIS_SATIRI->fis_id           = cari_acilis_fisi_id;
    CARI_FIS_SATIRI->program_id       = E9_PROGRAMI;
    CARI_FIS_SATIRI->modul_id         = CARI_MODULU;

    bool fis_satiri_eklendi = false;

    while (select_arsiv_firma.NEXT() EQ true) {

        int cari_hesap_id = select_arsiv_firma.VALUE(0).toInt();

        UNPACK_DOUBLE_ARRAY ( select_arsiv_firma.VALUE("toplam_borc_array").toString() , toplam_borc   , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY ( select_arsiv_firma.VALUE("toplam_alacak_array").toString() , toplam_alacak , E9_ARRAY_SIZE);

        if ( toplam_borc[gun_no] EQ toplam_alacak[gun_no]) {
            continue;
        }
        double borc_tutari   = 0.0;
        double alacak_tutari = 0.0;


        if ( toplam_borc[gun_no] > toplam_alacak[gun_no]) {
            borc_tutari = ROUND(toplam_borc[gun_no]) - ROUND(toplam_alacak[gun_no]);
        }
        else {
            alacak_tutari = ROUND(toplam_alacak[gun_no]) - ROUND(toplam_borc[gun_no]);
        }

        if ( borc_tutari EQ 0 AND alacak_tutari EQ 0 ) {
            continue;
        }

        CARI_FIS_SATIRI->borc_tutari      = borc_tutari;
        CARI_FIS_SATIRI->alacak_tutari    = alacak_tutari;
        CARI_FIS_SATIRI->cari_hesap_id    = cari_hesap_id;

        CARI_FIS_SATIRI_EKLE(CARI_FIS,CARI_FIS_SATIRI);

        fis_satiri_eklendi = true;
    }

    if ( fis_satiri_eklendi EQ false ) {

        SQL_QUERY sql_query(DB);

        sql_query.PREPARE_DELETE("car_fisler", "fis_id = :fis_id");
        sql_query.SET_VALUE(":fis_id" , cari_acilis_fisi_id);
        sql_query.DELETE();
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Modülünde , Açılış Fişi Oluşturulamadı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Cari Modülünde , Açılış Fişi Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/*************************************************************************************************/
/*              E9_DEVIR_ISLEMI_BATCH::CEK_SENET_MODULUNU_DEVRET();                             */
/*************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CEK_SENET_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** ÇEK SENET MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Çek Senet Modülü,Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    CEKLER_TABLOSUNU_KOPYALA (  );

    CEK_BORDROLARINI_KOPYALA (  );

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Çek Senet Modülü, Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/*************************************************************************************************/
/*               E9_DEVIR_ISLEMI_BATCH::CEKLER_TABLOSUNU_KOPYALA();                             */
/*************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CEKLER_TABLOSUNU_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);

    select_devredilecek_firma.PREPARE_SELECT("cek_cekler_senetler" , "cek_senet_id");

    if (select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Çekler Tablosunda , Çek Senet Kaydı Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }
    int cek_senet_id = 0;

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Çekler ve Senetler,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    QStringList where_line_list;

    while (select_devredilecek_firma.NEXT()) {

        cek_senet_id              = select_devredilecek_firma.VALUE(0).toInt();

        //CEk veya senet arsiv firmada islem gormus ise arsiv firmaya kopyalanmalidir.
        if ( CEK_SENET_ARSIV_FIRMADA_ISLEM_GORDU_MU ( cek_senet_id ) EQ false ) {
            continue;
        }

        where_line_list.clear();

        where_line_list << QVariant(cek_senet_id).toString();

        E9_COPY_TABLES(DB , ARSIV_DB ,"cek_cekler_senetler" ,QString("cek_senet_id = :arg_%1").arg(0) , where_line_list);

        where_line_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

        E9_COPY_TABLES(DB , ARSIV_DB ,"cek_hareketleri" , QString("cek_senet_id = :arg_%1 AND "
                                             "hareket_tarihi < :arg_%2").arg(0).arg(1) , where_line_list);

    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Uygun Çek Ve Senetler Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}
/************************************************************************************************/
/*              E9_DEVIR_ISLEMI_BATCH::CEK_BORDROLARINI_KOPYALA();                             */
/************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CEK_BORDROLARINI_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);


    QStringList where_list;

    QString where_str = QString("bordro_tarihi < :arg_%1 AND ( modul_id = :arg_%2 AND program_id = :arg_%3 ) "
                                "OR ( program_id != :arg_%4 )").arg(where_list.size())
                                                          .arg(where_list.size() + 1)
                                                          .arg(where_list.size() + 2)
                                                          .arg(where_list.size() + 3);

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");
    where_list << QString::number(CEKSENET_MODULU)<< QString::number(E9_PROGRAMI) << QString::number(E9_PROGRAMI);

    E9_COPY_TABLES(DB ,ARSIV_DB , "cek_bordrolar" , where_str , where_list);



    select_devredilecek_firma.PREPARE_SELECT("cek_bordrolar" , "bordro_id" , "bordro_tarihi < :bordro_tarihi AND "
                                             "(modul_id = :modul_id AND program_id = :program_id_1) OR "
                                             "(program_id != :program_id_2)");

    select_devredilecek_firma.SET_VALUE(":bordro_tarihi" , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));
    select_devredilecek_firma.SET_VALUE(":modul_id"      , CEKSENET_MODULU );
    select_devredilecek_firma.SET_VALUE(":program_id_1"  , E9_PROGRAMI);
    select_devredilecek_firma.SET_VALUE(":program_id_2"  , E9_PROGRAMI);

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmaya Kopyalanacak,Uygun Bordro Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Çek - Senet Bordroları Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    QStringList line_where_list;


    while (select_devredilecek_firma.NEXT()) {

        line_where_list.clear();

        line_where_list << select_devredilecek_firma.VALUE(0).toString();

        E9_COPY_TABLES(DB ,ARSIV_DB , "cek_bordro_satirlari" , QString("bordro_id = :arg_%1").arg(0) , line_where_list );
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Uygun Çek ve Senet Bordroları , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}



/*********************************************************************/
/*    E9_DEVIR_ISLEMI_BATCH::CEK_SENET_ACILIS_BORDROSU_OLUSTUR();   */
/*********************************************************************/

void E9_DEVIR_ISLEMI_BATCH::CEK_SENET_ACILIS_BORDROSU_OLUSTUR( )
{
    SQL_QUERY select_devredilecek_firma(DB);
    SQL_QUERY select_query(DB);

    //Cek veye senedin acilis bordrosunun olusturulabilmesi icin,31 aralikta cek veya senedin durumu asagidaki durumlardan biri olmalidir.

    QString where_str = "(cek_senet_son_durumu    = :durum_1) "
                        "OR (cek_senet_son_durumu = :durum_2) "
                        "OR (cek_senet_son_durumu = :durum_3) "
                        "OR (cek_senet_son_durumu = :durum_4) "
                        "OR (cek_senet_son_durumu = :durum_5) "
                        "OR (cek_senet_son_durumu = :durum_6) ";

    select_devredilecek_firma.PREPARE_SELECT("cek_cekler_senetler" , "cek_senet_id,cek_senet_son_durumu,musteri_ceki_mi,"
                                             "cek_senet_turu,cek_senet_tutari",where_str);

    select_devredilecek_firma.SET_VALUE(":durum_1"          , ENUM_BNK_TEMINAT_VERILDI );
    select_devredilecek_firma.SET_VALUE(":durum_2"          , ENUM_BNK_TAHSILAT_VERILDI );
    select_devredilecek_firma.SET_VALUE(":durum_3"          , ENUM_KARSILIKSIZ );
    select_devredilecek_firma.SET_VALUE(":durum_4"          , ENUM_PROTESTOLU);
    select_devredilecek_firma.SET_VALUE(":durum_5"          , ENUM_SUPHELI_ALACAK );
    select_devredilecek_firma.SET_VALUE(":durum_6"          , ENUM_SATICIYA_VERILDI );

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Çek - Senet Modülünde , Açılış Fişi Oluşturulamadı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }


    int bordro_islem_turu = -1;

    CEK_BORDRO_STRUCT * CEK_BORDROSU = new CEK_BORDRO_STRUCT;

    CEK_BORDRO_SATIRI_STRUCT * CEK_BORDRO_SATIRI = new CEK_BORDRO_SATIRI_STRUCT;

    while (select_devredilecek_firma.NEXT()) {

        int cek_senet_id          = select_devredilecek_firma.VALUE(0).toInt();
        int cek_senet_son_durumu  = select_devredilecek_firma.VALUE(1).toInt();
        int musteri_cek_senedi_mi = select_devredilecek_firma.VALUE(2).toInt();
        int cek_senet_turu        = select_devredilecek_firma.VALUE(3).toInt();
        double cek_senet_tutari   = select_devredilecek_firma.VALUE(4).toDouble();

        if ( cek_senet_son_durumu EQ ENUM_SATICIYA_VERILDI) {
            if ( musteri_cek_senedi_mi EQ 1 ) {
                if ( CIRO_EDILEN_CEK_ODENDI_KABUL_EDILECEK_MI(cek_senet_id,m_arsiv_firma_kapanis_tarihi) EQ true) {
                    continue;
                }
            }
        }

        select_query.PREPARE_SELECT("cek_hareketleri" , "cek_son_hareketi" ,"cek_senet_id = :cek_senet_id "
                                    "AND hareket_tarihi <= :hareket_tarihi" );

        select_query.SET_VALUE(":cek_senet_id"     , cek_senet_id);
        select_query.SET_VALUE(":hareket_tarihi"   , m_arsiv_firma_kapanis_tarihi.toString("yyyy.MM.dd"));

        if ( select_query.SELECT("hareket_tarihi DESC" , 0 , 1) NE 0 ) {
            select_query.NEXT();
            int cekin_pozisyonu = select_query.VALUE(0).toInt();

            if ( cek_senet_son_durumu NE cekin_pozisyonu) {
                //cek senedin son durumu 31 araliktaki pozisyonundan farkli ise acilis bordrosu olusturulmayacak.
                continue;
            }
        }
        switch (cek_senet_son_durumu) {
            case ENUM_BNK_TEMINAT_VERILDI :
                bordro_islem_turu = ENUM_CEK_BNK_TEMINATA_VERILDI;
                break;
            case ENUM_BNK_TAHSILAT_VERILDI :
                bordro_islem_turu = ENUM_CEK_BNK_TAHSILATA_VERILDI;
                break;
            case ENUM_SATICIYA_VERILDI:
                bordro_islem_turu = ENUM_CEK_SATICIYA_VERILDI;
                break;
            case ENUM_KARSILIKSIZ :
                bordro_islem_turu = ENUM_CEK_KARSILIKSIZ;
                break;
            case ENUM_PROTESTOLU :
                bordro_islem_turu = ENUM_CEK_PROTESTOLU;
                break;
            case ENUM_SUPHELI_ALACAK :
                bordro_islem_turu = ENUM_CEK_SUPHELI_ALACAK;
                break;

        }

        int hesap_no_id = 0;
        if ( bordro_islem_turu EQ ENUM_BNK_TEMINAT_VERILDI OR bordro_islem_turu EQ ENUM_BNK_TAHSILAT_VERILDI) {

            //cek senedin 31 araliktan once islem gorup,son pozisyonuna gectigi bordrodan hangi bankaya teminata
            //verildigi bilgisi alinir.

            select_query.PREPARE_SELECT("cek_bordrolar,cek_bordro_satirlari" ,"cek_bordrolar.hesap_no_id",
                                        "bordro_islem_turu = :bordro_islem_turu "
                                        "AND cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                                        "cek_senet_id = :cek_senet_id");

            select_query.SET_VALUE(":bordro_islem_turu"   , bordro_islem_turu);
            select_query.SET_VALUE(":cek_senet_id"        , cek_senet_id);

            if ( select_query.SELECT() NE 0) {
                select_query.NEXT();

                hesap_no_id = select_query.VALUE(0).toInt();
            }
        }
        //Cek veya senet musteri cek senedi ise,hangi musteriden alindigi bilgisi cekin firmaya ilk girisinin
        //yapildigi bordrodan alinir.
        int cek_senedin_alindigi_cari_hesap  = 0;
        int cek_senedin_verildigi_cari_hesap = 0;

        if ( musteri_cek_senedi_mi NE 0) {

            select_query.PREPARE_SELECT("cek_bordrolar ,cek_bordro_satirlari" , "cek_bordrolar.cari_hesap_id",
                                        "bordro_islem_turu = :bordro_islem_turu "
                                        "AND cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id "
                                        "AND cek_senet_id = :cek_senet_id");

            select_query.SET_VALUE(":bordro_islem_turu"   , ENUM_CEK_MUSTERIDEN_ALINDI);
            select_query.SET_VALUE(":cek_senet_id"        , cek_senet_id);

            if ( select_query.SELECT() NE 0 ) {
                select_query.NEXT();
                cek_senedin_alindigi_cari_hesap = select_query.VALUE(0).toInt();
            }

            if ( bordro_islem_turu EQ ENUM_SATICIYA_VERILDI) {

                select_query.PREPARE_SELECT("cek_bordrolar ,cek_bordro_satirlari" , "cek_bordrolar.cari_hesap_id",
                                            "bordro_islem_turu = :bordro_islem_turu "
                                            "AND cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id "
                                            "AND cek_senet_id = :cek_senet_id");

                select_query.SET_VALUE(":bordro_islem_turu"   , ENUM_CEK_SATICIYA_VERILDI);
                select_query.SET_VALUE(":cek_senet_id"        , cek_senet_id);

                if(select_query.SELECT() NE 0 ) {
                    select_query.NEXT();
                    cek_senedin_verildigi_cari_hesap = select_query.VALUE(0).toInt();
                }
            }
        }

        int uygun_bordro_id = FIND_UYGUN_ACILIS_BORDROSU ( bordro_islem_turu,cek_senet_turu,hesap_no_id );

        if ( uygun_bordro_id EQ -1) {
            //cekin son pozisyonuna hangi bordro ile gectigi bulunur.O bordrodan gerekli bilgiler alinir.

            m_calisan_firma_last_cek_fis_no++;

            CEK_BORDROSU->CLEAR_BORDRO_STRUCT();

            CEK_BORDROSU->bordro_tarihi         = m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");
            CEK_BORDROSU->bordro_no             = m_calisan_firma_last_cek_fis_no;
            CEK_BORDROSU->bordro_islem_turu     = bordro_islem_turu;
            CEK_BORDROSU->aciklama              = tr("AÇILIŞ BORDROSU-Devir işlemi sırasında otomatik olarak oluşturulmuştur.");
            CEK_BORDROSU->hesap_no_id           = hesap_no_id;
            CEK_BORDROSU->modul_id              = CEKSENET_MODULU;
            CEK_BORDROSU->program_id            = E9_PROGRAMI;
            CEK_BORDROSU->bordro_turu           = ENUM_CEK_SENET_ACILIS_BORDROSU;
            CEK_BORDROSU->musteri_cek_senedi_mi = musteri_cek_senedi_mi;

            if ( cek_senet_turu EQ ENUM_CEK ) {
                CEK_BORDROSU->cek_bordrosu_mu = 1;
            }
            else {
                CEK_BORDROSU->cek_bordrosu_mu = 0;
            }
            int acilis_bordrosu_id = CEK_BORDROSU_EKLE(CEK_BORDROSU);

            uygun_bordro_id = acilis_bordrosu_id;
        }
        else {
            CEK_BORDRO_BILGILERI_OKU(CEK_BORDROSU , uygun_bordro_id);
        }
        CEK_CLEAR_BORDRO_SATIRI_STRUCT(CEK_BORDRO_SATIRI);

        CEK_BORDRO_SATIRI->bordro_id             = uygun_bordro_id;
        CEK_BORDRO_SATIRI->cek_senet_id          = cek_senet_id;
        CEK_BORDRO_SATIRI->alinan_cari_hesap_id  = cek_senedin_alindigi_cari_hesap;
        CEK_BORDRO_SATIRI->verilen_cari_hesap_id = cek_senedin_verildigi_cari_hesap;
        CEK_BORDRO_SATIRI->modul_id              = CEKSENET_MODULU;
        CEK_BORDRO_SATIRI->program_id            = E9_PROGRAMI;
        CEK_BORDRO_SATIRI->cek_tutari            = cek_senet_tutari;

        CEK_BORDRO_SATIRI_EKLE ( CEK_BORDROSU , CEK_BORDRO_SATIRI, false );
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Çek - Senet Modülünde , Açılış Fişi Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/*************************************************************************************************/
/*                   E9_DEVIR_ISLEMI_BATCH::MUHASEBE_MODULUNU_DEVRET();                         */
/*************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::MUHASEBE_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** MUHASEBE MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Modülü Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB , ARSIV_DB , "muh_hesaplar" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Hesapları , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB ,ARSIV_DB , "muh_yansitma_hesaplari" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Yansıtma Hesapları , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    QStringList column_names;

    column_names << "toplam_borc_array" << "toplam_alacak_array";


    HESAP_BAKIYELERINI_SIFIRLA("muh_hesaplar" , "hesap_id" , column_names ,  6 , ARSIV_DB);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Hesap Bakiyeleri Sıfırlandı<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    MUHASEBE_FISLERINI_KOPYALA  ( );

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Modülü Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/*******************************************************************************/
/*                  E9_DEVIR_ISLEMI_BATCH::MUHASEBE_FISLERINI_KOPYALA         */
/*******************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::MUHASEBE_FISLERINI_KOPYALA (  )
{
    SQL_QUERY select_db_query         ( DB );

    //E9_PROGRAMINA ait olan entegrasyon fisleri aktarilmayacak.E9 ONAR fonksiyonu kullanilarak ent fisleri tekrardan olusturulacak

    QStringList where_list;

    QString where_str = QString("fis_tarihi < :arg_%1 AND ( modul_id = :arg_%2 AND program_id = :arg_%3 )"
                                "OR ( program_id != :arg_%4 )").arg(where_list.size())
                                                          .arg(where_list.size() + 1)
                                                          .arg(where_list.size() + 2)
                                                          .arg(where_list.size() + 3);

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");
    where_list << QString::number(MUHASEBE_MODULU)<< QString::number(E9_PROGRAMI) << QString::number(E9_PROGRAMI);

    E9_COPY_TABLES(DB ,ARSIV_DB , "muh_fisler" , where_str , where_list);



    select_db_query.PREPARE_SELECT("muh_fisler" , "fis_id" ,
                                   "fis_tarihi < :fis_tarihi AND "
                                   "(modul_id = :modul_id AND program_id =:program_id_1 ) OR "
                                   "(program_id != :program_id_2)");

    select_db_query.SET_VALUE(":fis_tarihi"   , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));
    select_db_query.SET_VALUE(":modul_id"     , MUHASEBE_MODULU );
    select_db_query.SET_VALUE(":program_id_1" , E9_PROGRAMI);
    select_db_query.SET_VALUE(":program_id_2" , E9_PROGRAMI);

    if ( select_db_query.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Fişleri Tablosunda Kayıt Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Fişleri,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    QStringList line_where_list;


    while ( select_db_query.NEXT() EQ true ) {

        line_where_list.clear();

        line_where_list << select_db_query.VALUE(0).toString();

        E9_COPY_TABLES(DB ,ARSIV_DB , "muh_fis_satirlari" , QString("fis_id = :arg_%1").arg(0) , line_where_list);
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Fişleri,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}


/****************************************************************/
/*           E9_DEVIR_ISLEMI_BATCH::MUH_ACILIS_FISI_OLUSTUR        */
/****************************************************************/

void E9_DEVIR_ISLEMI_BATCH::MUH_ACILIS_FISI_OLUSTUR ()
{
    SQL_QUERY arsiv_query(ARSIV_DB);

    arsiv_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id,toplam_borc_array,toplam_alacak_array" ,
                               "tali_hesap = :tali_hesap AND tam_hesap_kodu < :tam_hesap_kodu");

    arsiv_query.SET_VALUE(":tali_hesap"     , 1 );
    arsiv_query.SET_VALUE(":tam_hesap_kodu" , "600");


    if ( arsiv_query.SELECT("tam_hesap_kodu ASC") EQ 0 ) {
       BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Modülünde , Açılış Fişi Oluşturulamadı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

       return;
    }
    m_calisan_firma_last_muh_fis_no++;

    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

    MUHASEBE_FISI->fis_no           = m_calisan_firma_last_muh_fis_no;
    MUHASEBE_FISI->fis_tarihi       = m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");
    MUHASEBE_FISI->aciklama         = tr("AÇILIŞ FİŞİ - Devir İşlemi Esnasında Otomatik Olarak Üretilmiştir.");
    MUHASEBE_FISI->fis_turu         = ENUM_ACILIS_FISI;
    MUHASEBE_FISI->modul_id         = MUHASEBE_MODULU;
    MUHASEBE_FISI->program_id       = E9_PROGRAMI;


    int acilis_fisi_id = MUH_FIS_EKLE(MUHASEBE_FISI , DB);

    int gun_no = MALI_YIL_ARRAY_INDIS( m_arsiv_firma_kapanis_tarihi );

    double borc_bakiyesi  [E9_ARRAY_SIZE];
    double alacak_bakiyesi [E9_ARRAY_SIZE];

    bool muh_fis_satiri_eklendi = false;

    while ( arsiv_query.NEXT() EQ true ) {

        int     hesap_id   = arsiv_query.VALUE(0).toInt();

        UNPACK_DOUBLE_ARRAY ( arsiv_query.VALUE("toplam_borc_array").toString() , borc_bakiyesi   , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY ( arsiv_query.VALUE("toplam_alacak_array").toString() , alacak_bakiyesi , E9_ARRAY_SIZE);

        if ( borc_bakiyesi[gun_no] EQ alacak_bakiyesi[gun_no]) {
            continue;
        }
        double borc_tutari   = 0.0;
        double alacak_tutari = 0.0;

        if (alacak_bakiyesi[gun_no] > borc_bakiyesi[gun_no] ) {
           alacak_tutari = ROUND(alacak_bakiyesi[gun_no]) - ROUND(borc_bakiyesi[gun_no]);
           ROUND (alacak_tutari);
        }
        else  {
           borc_tutari   = ROUND(borc_bakiyesi[gun_no]) - ROUND(alacak_bakiyesi[gun_no]);
           ROUND (borc_tutari );
        }

        MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRI->fis_id           = acilis_fisi_id;
        MUH_FIS_SATIRI->hesap_id         = hesap_id;
        MUH_FIS_SATIRI->borc_tutari      = borc_tutari;
        MUH_FIS_SATIRI->alacak_tutari    = alacak_tutari;
        MUH_FIS_SATIRI->program_id       = E9_PROGRAMI;
        MUH_FIS_SATIRI->modul_id         = MUHASEBE_MODULU;
        MUH_FIS_SATIRI->aciklama         = QObject::tr("Açılış Bilançosu");

        MUH_FIS_SATIRI_EKLE(MUHASEBE_FISI , MUH_FIS_SATIRI , DB);

        muh_fis_satiri_eklendi = true;
    }

    if ( muh_fis_satiri_eklendi EQ false ) {
        SQL_QUERY query(DB);

        query.PREPARE_DELETE("muh_fisler", "fis_id = :fis_id");
        query.SET_VALUE(":fis_id" , acilis_fisi_id);
        query.DELETE();
        return;
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Modülünde , Açılış Fişi Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/********************************************************************/
/*            E9_DEVIR_ISLEMI_BATCH::KAPANIS_FISI_OLUSTUR          */
/********************************************************************/

void E9_DEVIR_ISLEMI_BATCH::MUH_KAPANIS_FISI_OLUSTUR( )
{

    SQL_QUERY arsiv_query(ARSIV_DB);


    arsiv_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id,toplam_borc_array,toplam_alacak_array,"
                               "tali_hesap = :tali_hesap AND tam_hesap_kodu < :tam_hesap_kodu");
    arsiv_query.SET_VALUE(":tali_hesap"     , 1 );
    arsiv_query.SET_VALUE(":tam_hesap_kodu" , "600");


    if ( arsiv_query.SELECT("tam_hesap_kodu ASC") EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Muhasebe Modülünde , Kapanış Fişi Oluşturulamadı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

    MUHASEBE_FISI->fis_tarihi       = m_arsiv_firma_kapanis_tarihi.toString("yyyy.MM.dd");
    MUHASEBE_FISI->fis_turu         = ENUM_KAPANIS_FISI;
    MUHASEBE_FISI->aciklama         = tr("KAPANIŞ FİŞİ - Devir İşlemi Esnasında Otomatik Olarak Üretilmiştir.");
    MUHASEBE_FISI->modul_id         = MUHASEBE_MODULU;
    MUHASEBE_FISI->program_id       = E9_PROGRAMI;

    int kapanis_fisi_id = MUH_FIS_EKLE(MUHASEBE_FISI , ARSIV_DB);

    int gun_no          = MALI_YIL_ARRAY_INDIS( m_arsiv_firma_kapanis_tarihi );

    double borc_bakiyesi[E9_ARRAY_SIZE];
    double alacak_bakiyesi[E9_ARRAY_SIZE];

    bool fis_satiri_eklendi = false;

    while ( arsiv_query.NEXT() EQ true ) {

        int     hesap_id            = arsiv_query.VALUE(0).toInt();

        UNPACK_DOUBLE_ARRAY(arsiv_query.VALUE("toplam_borc_array").toString() , borc_bakiyesi , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(arsiv_query.VALUE("toplam_alacak_array").toString() , alacak_bakiyesi , E9_ARRAY_SIZE);

        if ( borc_bakiyesi[gun_no] EQ alacak_bakiyesi[gun_no] ) {
            continue;
        }


        double borc_tutari   = 0.00;
        double alacak_tutari = 0.00;

        if ( alacak_bakiyesi[gun_no] > borc_bakiyesi[gun_no] ) {
           borc_tutari = ROUND(alacak_bakiyesi[gun_no]) - ROUND(borc_bakiyesi[gun_no]);
        }
        else  {
           alacak_tutari = ROUND(borc_bakiyesi[gun_no]) - ROUND(alacak_bakiyesi[gun_no]);
        }

        MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRI->fis_id           = kapanis_fisi_id;
        MUH_FIS_SATIRI->hesap_id         = hesap_id;
        MUH_FIS_SATIRI->aciklama         = QObject::tr("Kapanış Bilançosu");
        MUH_FIS_SATIRI->borc_tutari      = borc_tutari;
        MUH_FIS_SATIRI->alacak_tutari    = alacak_tutari;
        MUH_FIS_SATIRI->modul_id         = MUHASEBE_MODULU;
        MUH_FIS_SATIRI->program_id       = E9_PROGRAMI;

        MUH_FIS_SATIRI_EKLE(MUHASEBE_FISI , MUH_FIS_SATIRI , ARSIV_DB);

        fis_satiri_eklendi = true;
    }

    if ( fis_satiri_eklendi EQ false ) {
        SQL_QUERY query(ARSIV_DB);

        query.PREPARE_DELETE("muh_fisler" ,"fis_id = :fis_id");
        query.SET_VALUE(":fis_id" , kapanis_fisi_id);
        query.DELETE();
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmada Muhasebe Modülünde , Kapanış Fişi Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/********************************************************************************************/
/*           E9_DEVIR_ISLEMI_BATCH::ISLETME_DEFTERI_MODULUNU_DEVRET();                     */
/*********************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::ISLETME_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** İŞLETME MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - İşletme Modülü , Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    ISLETME_DEFTERI_FISLERINI_KOPYALA();

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - İşletme Modülü , Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/***********************************************************************************/
/*              E9_DEVIR_ISLEMI_BATCH::ISLETME_DEFTERI_FISLERINI_KOPYALA          */
/***********************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::ISLETME_DEFTERI_FISLERINI_KOPYALA ( )
{
    SQL_QUERY select_db_query(DB);

    QStringList where_list;

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

    E9_COPY_TABLES(DB ,ARSIV_DB , "isl_fisi" , QString("fis_tarihi < :arg_%1").arg(0) , where_list);

    select_db_query.PREPARE_SELECT("isl_fisi" , "fis_id" , "fis_tarihi < :fis_tarihi");

    select_db_query.SET_VALUE(":fis_tarihi" , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd") );

    if ( select_db_query.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - İşletme Fişleri Tablosunda Kayıt Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - İşletme Modülü Fişleri,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    while ( select_db_query.NEXT() EQ true ) {

        where_list.clear();

        where_list << select_db_query.VALUE(0).toString();

        E9_COPY_TABLES(DB ,ARSIV_DB , "isl_fis_satirlari" , QString("fis_id = :arg_%1").arg(0) , where_list);

    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - İşletme Modülü Fişleri,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/***********************************************************************************/
/*                 E9_DEVIR_ISLEMI_BATCH::DOVIZ_MODULUNU_DEVRET();                 */
/***********************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::DOVIZ_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** DÖVİZ MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Döviz Modülü,Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB , ARSIV_DB , "dvz_degiskenler" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Döviz Modülü Değişkenler Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB ,ARSIV_DB , "dvz_dovizler" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Dövizler Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB , ARSIV_DB , "dvz_kurlar" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Döviz Kurları Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Döviz Modülü , Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}



/******************************************************************************/
/*               E9_DEVIR_ISLEMI_BATCH::SUBE_DEPO_UNITE_DEVRET();             */
/******************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::SUBE_DEPO_UNITE_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">***  ŞUBE DEPO ÜNİTE  ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Şube Modülü,Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB ,ARSIV_DB , "sub_depolar" , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Depolar , Arşiv Firmaya Kopyalandı<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES ( DB ,ARSIV_DB , "sub_uniteler" , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Şubelerin Kasa ve Ünite Bilgileri,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB ,ARSIV_DB , "sub_subeler" , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Şubeler,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Şube Modülü arşiv firmaya devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/************************************************************************************/
/*            E9_DEVIR_ISLEMI_BATCH::STOK_MODULUNU_DEVRET();                        */
/************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::STOK_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** STOK MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Stok Modülü , Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES ( DB ,ARSIV_DB , "stk_urunler" , "");

    STK_URUNUN_PARTILERI_TABLOSUNU_KOPYALA();

    STK_URUNUN_SERI_NUMARALARI_TABLOSUNU_KOPYALA();

    STK_DEPODAKI_URUNLER_TABLOSUNU_KOPYALA();

    STK_SATIRDAKI_SERI_NUMARALARI_TABLOSUNU_KOPYALA();

    STOK_FISLERINI_KOPYALA();


    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Stok Modülü Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/***********************************************************************************/
/*                  E9_DEVIR_ISLEMI_BATCH::STOK_FISLERINI_KOPYALA();               */
/***********************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::STOK_FISLERINI_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);
    SQL_QUERY select_db_satirlar (DB);

    QStringList where_list;

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

    E9_COPY_TABLES(DB ,ARSIV_DB , "stk_fisler" , QString("fis_tarihi < :arg_%1").arg(0) ,where_list );


    select_devredilecek_firma.PREPARE_SELECT("stk_fisler" ,"fis_id,fis_turu",
                                             "fis_tarihi < :fis_tarihi");

    select_devredilecek_firma.SET_VALUE(":fis_tarihi" , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Stok Modülünde Kayıtlı Fiş Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Stok Modülü Fişleri , Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    while (select_devredilecek_firma.NEXT()) {
        int fis_turu = select_devredilecek_firma.VALUE("fis_turu").toInt();

        where_list.clear();
        where_list << select_devredilecek_firma.VALUE(0).toString();

        E9_COPY_TABLES(DB ,ARSIV_DB , "stk_fis_satirlari" , QString("fis_id = :arg_%1").arg(0) , where_list);

        if ( fis_turu NE ENUM_IRSALIYE ) {
            continue;
        }

        select_db_satirlar.PREPARE_SELECT("stk_fis_satirlari" , "fis_satiri_id" ,  "fis_id = :fis_id");
        select_db_satirlar.SET_VALUE(":fis_id" , select_devredilecek_firma.VALUE(0).toInt());

        if ( select_db_satirlar.SELECT() EQ 0 ) {
            continue;
        }
        while ( select_db_satirlar.NEXT()) {
            IRS_IADE_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA(select_db_satirlar.VALUE(0).toInt());
        }
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Stok Modülü Fişleri,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}


/****************************************************************************************************/
/*            E9_DEVIR_ISLEMI_BATCH::STK_DEPODAKI_URUNLER_TABLOSUNU_KOPYALA();                      */
/****************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::STK_DEPODAKI_URUNLER_TABLOSUNU_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);
    SQL_QUERY insert_arsiv_firma(ARSIV_DB);
    SQL_QUERY sql_query(DB);

    double toplam_depoya_giren[E9_ARRAY_SIZE];
    double toplam_depodan_cikan[E9_ARRAY_SIZE];

    double calisan_firma_toplam_depoya_giren[E9_ARRAY_SIZE];
    double calisan_firma_toplam_depodan_cikan[E9_ARRAY_SIZE];

    select_devredilecek_firma.PREPARE_SELECT("stk_depodaki_urunler" , "depo_id, urun_id, kayit_id,"
                                             "toplam_depoya_giren_array,toplam_depodan_cikan_array");

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Depodaki Ürünler Tablosunda Kayıt Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }
    int gun_no = MALI_YIL_ARRAY_INDIS( m_arsiv_firma_kapanis_tarihi );

    while (select_devredilecek_firma.NEXT()) {

        int kayit_id = select_devredilecek_firma.VALUE("kayit_id").toInt();

        UNPACK_DOUBLE_ARRAY(select_devredilecek_firma.VALUE("toplam_depoya_giren_array").toString() , toplam_depoya_giren  , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(select_devredilecek_firma.VALUE("toplam_depodan_cikan_array").toString(), toplam_depodan_cikan , E9_ARRAY_SIZE);


        double yil_sonu_toplam_depoya_giren = toplam_depoya_giren[gun_no];
        double yil_sonu_toplam_depodan_cikan= toplam_depodan_cikan[gun_no];


        for ( int i = 0 ; i < E9_ARRAY_SIZE ; i++) {
            calisan_firma_toplam_depoya_giren[i]  = 0;
            calisan_firma_toplam_depodan_cikan[i] = 0;

        }
        //31 araliktaki urun miktari,calisan firmanin 0.indisine yazilir.

        if ( yil_sonu_toplam_depoya_giren > yil_sonu_toplam_depodan_cikan ) {
            calisan_firma_toplam_depoya_giren[0] = yil_sonu_toplam_depoya_giren - yil_sonu_toplam_depodan_cikan;
        }
        else {
            calisan_firma_toplam_depodan_cikan[0] = yil_sonu_toplam_depodan_cikan - yil_sonu_toplam_depoya_giren;
        }


        int gun_indis_no = 0;

        //gun no yilin son gunudur. i = devredilecek firmanin,yeni muhasebe yilina ait olan indis degerleridir.
        for ( int i = gun_no + 1 ; i < E9_ARRAY_SIZE ; i++) {

            gun_indis_no++;

            calisan_firma_toplam_depoya_giren[gun_indis_no]        = (toplam_depoya_giren[i]       - toplam_depoya_giren[i-1]        ) + calisan_firma_toplam_depoya_giren        [gun_indis_no-1];
            calisan_firma_toplam_depodan_cikan[gun_indis_no]       = (toplam_depodan_cikan[i]      - toplam_depodan_cikan[i-1]       ) + calisan_firma_toplam_depodan_cikan       [gun_indis_no-1];

        }
        //Devredilecek firmanin,calisan firmaya ait bakiyeleri duzeltiliyor.DEvredilecek firmanin yeni firma yilina
        //ait olan bakiyeleri bulunup,1.gun indisinden itibaren kaydedilmis gun indisine kadar calisan firmaya kopyalaniyor.


        //Calisan firmanin bos olan indislerine dolu olan son indisin bakiyeleri kopyalaniyor.
        for (int i = gun_indis_no+1 ; i < E9_ARRAY_SIZE ; i++) {
            calisan_firma_toplam_depoya_giren[i]       = calisan_firma_toplam_depoya_giren[gun_indis_no];
            calisan_firma_toplam_depodan_cikan[i]      = calisan_firma_toplam_depodan_cikan[gun_indis_no];
        }

        sql_query.PREPARE_UPDATE("stk_depodaki_urunler","kayit_id",
                                 "toplam_depoya_giren_array ,"
                                 "toplam_depodan_cikan_array ",
                                 "kayit_id = :kayit_id");
        sql_query.SET_VALUE(":toplam_depoya_giren_array"       , PACK_DOUBLE_ARRAY(toplam_depoya_giren       , E9_ARRAY_SIZE));
        sql_query.SET_VALUE(":toplam_depodan_cikan_array"      , PACK_DOUBLE_ARRAY(toplam_depodan_cikan      , E9_ARRAY_SIZE));
        sql_query.SET_VALUE(":kayit_id" , kayit_id);

        sql_query.UPDATE();

        for (int i = gun_no + 1 ; i < E9_ARRAY_SIZE ; i++) {
            toplam_depoya_giren[i]       = yil_sonu_toplam_depoya_giren;
            toplam_depodan_cikan[i]      = yil_sonu_toplam_depodan_cikan;
        }
        insert_arsiv_firma.PREPARE_INSERT("stk_depodaki_urunler" , "kayit_id" , "depo_id,urun_id,"
                                          "toplam_depoya_giren_array , toplam_depodan_cikan_array");


        insert_arsiv_firma.SET_VALUE(":depo_id"                         , select_devredilecek_firma.VALUE(0).toInt());
        insert_arsiv_firma.SET_VALUE(":urun_id"                         , select_devredilecek_firma.VALUE(1).toInt());
        insert_arsiv_firma.SET_VALUE(":toplam_depoya_giren_array"       , PACK_DOUBLE_ARRAY(toplam_depoya_giren,E9_ARRAY_SIZE));
        insert_arsiv_firma.SET_VALUE(":toplam_depodan_cikan_array"      , PACK_DOUBLE_ARRAY(toplam_depodan_cikan,E9_ARRAY_SIZE));


        insert_arsiv_firma.INSERT();


    }
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Depodaki Ürünler Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/************************************************************************************/
/*     E9_DEVIR_ISLEMI_BATCH::STK_SATIRDAKI_SERI_NUMARALARI_TABLOSUNU_KOPYALA();    */
/************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::STK_SATIRDAKI_SERI_NUMARALARI_TABLOSUNU_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);
    SQL_QUERY insert_arsiv_firma(DB);
    QStringList seri_numaralari_id_list;

    //ARSiv firmada islem goren irsaliye satirlarindaki seri numaralar tablosu arsiv firmaya kopyalanir.
    seri_numaralari_id_list = GET_YILA_GORE_SATIRDAKI_SERI_NUMARALARI_ID_LIST(m_arsiv_firma_kapanis_tarihi);

    for (int i = 0 ; i < seri_numaralari_id_list.size() ; i++) {
        int seri_numarasi_id = QVariant(seri_numaralari_id_list.at(i)).toInt();

        select_devredilecek_firma.PREPARE_SELECT("stk_satirdaki_seri_nolari","irsaliye_satiri_id,seri_numarasi_id,order_number",
                                                 "seri_numarasi_id = :seri_numarasi_id");
        select_devredilecek_firma.SET_VALUE(":seri_numarasi_id" , seri_numarasi_id);

        if(select_devredilecek_firma.SELECT() EQ 0) {
            return;
        }
        while (select_devredilecek_firma.NEXT()) {

            insert_arsiv_firma.PREPARE_INSERT("stk_satirdaki_seri_nolari" , "satir_seri_no_id" ,
                                              "irsaliye_satiri_id, seri_numarasi_id, order_number");

            insert_arsiv_firma.SET_VALUE(":irsaliye_satiri_id" , select_devredilecek_firma.VALUE(0).toInt());
            insert_arsiv_firma.SET_VALUE(":seri_numarasi_id"   , select_devredilecek_firma.VALUE(1).toInt());
            insert_arsiv_firma.SET_VALUE(":order_number"  , select_devredilecek_firma.VALUE(2).toInt());

            insert_arsiv_firma.INSERT();
        }
    }

}

/**************************************************************************************/
/*          E9_DEVIR_ISLEMI_BATCH::STK_URUNUN_PARTILERI_TABLOSUNU_KOPYALA();          */
/**************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::STK_URUNUN_PARTILERI_TABLOSUNU_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);
    SQL_QUERY insert_arsiv_firma(ARSIV_DB);
    SQL_QUERY sql_query(DB);

    double  toplam_partiye_giren[E9_ARRAY_SIZE];
    double  toplam_partiden_cikan[E9_ARRAY_SIZE];

    double  calisan_firma_toplam_partiye_giren[E9_ARRAY_SIZE];
    double  calisan_firma_toplam_partiden_cikan[E9_ARRAY_SIZE];

    //Arsiv firmada islem gormus olan partiler kopyalanir.
    QStringList islem_gormus_parti_id_list = GET_YILA_GORE_SATIRDAKI_PARTI_ID_LIST(m_arsiv_firma_kapanis_tarihi);


    select_devredilecek_firma.PREPARE_SELECT("stk_urunun_partileri ","parti_id,depo_id,urun_id,parti_kodu,urun_uretim_tarihi,"
                                             "urun_son_kul_tarihi,toplam_partiye_giren_array,"
                                             "toplam_partiden_cikan_array");

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        return;
    }
    int parti_id = 0;
    int last_parti_id = 0;
    while (select_devredilecek_firma.NEXT()) {

        parti_id = select_devredilecek_firma.VALUE(0).toInt();


        UNPACK_DOUBLE_ARRAY(select_devredilecek_firma.VALUE("toplam_partiye_giren_array").toString()  , toplam_partiye_giren    , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(select_devredilecek_firma.VALUE("toplam_partiden_cikan_array").toString() , toplam_partiden_cikan   , E9_ARRAY_SIZE);

        int gun_no = MALI_YIL_ARRAY_INDIS( m_arsiv_firma_kapanis_tarihi );

        double yil_sonu_toplam_partiye_giren  = toplam_partiye_giren[gun_no];
        double yil_sonu_toplam_partiden_cikan = toplam_partiden_cikan[gun_no];

        for (int i = 0 ; i < E9_ARRAY_SIZE ; i++) {
            calisan_firma_toplam_partiye_giren[i]   = 0;
            calisan_firma_toplam_partiden_cikan[i]  = 0;
        }
        //31 araliktaki urun miktari,calisan firmanin 0.indisine yazilir.

        if ( yil_sonu_toplam_partiye_giren > yil_sonu_toplam_partiden_cikan) {
            calisan_firma_toplam_partiye_giren[0] = yil_sonu_toplam_partiye_giren - yil_sonu_toplam_partiden_cikan;
        }
        else {
            calisan_firma_toplam_partiden_cikan[0] = yil_sonu_toplam_partiden_cikan - yil_sonu_toplam_partiye_giren;
        }

        int gun_indis_no = 0;

        //gun no yilin son gunudur. i = devredilecek firmanin,yeni muhasebe yilina ait olan indis degerleridir.
        for (int i = gun_no + 1 ; i < E9_ARRAY_SIZE ; i++) {

            gun_indis_no++;

            calisan_firma_toplam_partiye_giren[gun_indis_no]   = (toplam_partiye_giren[i] -
                toplam_partiye_giren[i-1]) + calisan_firma_toplam_partiye_giren[gun_indis_no-1];

            calisan_firma_toplam_partiden_cikan[gun_indis_no]  = (toplam_partiden_cikan[i] -
                toplam_partiden_cikan[i-1]) + calisan_firma_toplam_partiden_cikan[gun_indis_no-1];
        }


        //Calisan firmanin bos olan indislerine dolu olan son indisin bakiyeleri kopyalaniyor.
        for (int i = gun_indis_no+1 ; i < E9_ARRAY_SIZE ; i++) {
            calisan_firma_toplam_partiye_giren[i]    = calisan_firma_toplam_partiye_giren[gun_indis_no];
            calisan_firma_toplam_partiden_cikan[i]   = calisan_firma_toplam_partiden_cikan[gun_indis_no];
        }

        sql_query.PREPARE_UPDATE("stk_urunun_partileri" , "parti_id",
                                 "toplam_partiye_giren_array ,"
                                 "toplam_partiden_cikan_array",
                                 "parti_id = :parti_id");
        sql_query.SET_VALUE(":toplam_partiye_giren_array"   , PACK_DOUBLE_ARRAY(toplam_partiye_giren  , E9_ARRAY_SIZE));
        sql_query.SET_VALUE(":toplam_partiden_cikan_array"  , PACK_DOUBLE_ARRAY(toplam_partiden_cikan , E9_ARRAY_SIZE));
        sql_query.SET_VALUE(":parti_id" , parti_id);

        sql_query.UPDATE();

        if ( islem_gormus_parti_id_list.contains(QVariant(parti_id).toString()) EQ true ) {

            for (int i = gun_no + 1 ; i < E9_ARRAY_SIZE ; i++) {
                toplam_partiye_giren[i]   = yil_sonu_toplam_partiye_giren;
                toplam_partiden_cikan[i]  = yil_sonu_toplam_partiden_cikan;
            }

            insert_arsiv_firma.PREPARE_INSERT("stk_urunun_partileri" , "parti_id" , "parti_id,depo_id,"
                                              "urun_id,parti_kodu,urun_uretim_tarihi,urun_son_kul_tarihi ,"
                                              "toplam_partiye_giren_array,toplam_partiden_cikan_array");

            insert_arsiv_firma.SET_VALUE(":parti_id"                    , parti_id);
            insert_arsiv_firma.SET_VALUE(":depo_id"                     , select_devredilecek_firma.VALUE(1).toInt());
            insert_arsiv_firma.SET_VALUE(":urun_id"                     , select_devredilecek_firma.VALUE(2).toInt());
            insert_arsiv_firma.SET_VALUE(":parti_kodu"                  , select_devredilecek_firma.VALUE(3).toString());
            insert_arsiv_firma.SET_VALUE(":urun_uretim_tarihi"          , select_devredilecek_firma.VALUE(4).toString());
            insert_arsiv_firma.SET_VALUE(":urun_son_kul_tarihi"         , select_devredilecek_firma.VALUE(5).toString());
            insert_arsiv_firma.SET_VALUE(":toplam_partiye_giren_array"  , PACK_DOUBLE_ARRAY ( toplam_partiye_giren    , E9_ARRAY_SIZE));
            insert_arsiv_firma.SET_VALUE(":toplam_partiden_cikan_array" , PACK_DOUBLE_ARRAY ( toplam_partiden_cikan   , E9_ARRAY_SIZE));

            insert_arsiv_firma.INSERT();

            last_parti_id = parti_id;

        }
    }


    if ( last_parti_id > 0 ) {
        ARSIV_DB->SET_TABLE_AUTO_INCREMENT_ID_VALUE ( "stk_urunun_partileri" , "parti_id", last_parti_id );
    }
}

/*************************************************************************************/
/*          E9_DEVIR_ISLEMI_BATCH::STK_URUNUN_SERI_NUMARALARI_TABLOSUNU_KOPYALA();   */
/*************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::STK_URUNUN_SERI_NUMARALARI_TABLOSUNU_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);
    SQL_QUERY insert_arsiv_firma(ARSIV_DB);

    QStringList islem_goren_seri_no_id_list = GET_YILA_GORE_SATIRDAKI_SERI_NUMARALARI_ID_LIST(m_arsiv_firma_kapanis_tarihi);

    select_devredilecek_firma.PREPARE_SELECT("stk_urunun_seri_nolari","seri_numarasi_id,depo_id,urun_id,seri_numarasi,"
                                             "depoda_mi");

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        return;
    }
    int seri_numarasi_id = 0;

    while (select_devredilecek_firma.NEXT()) {

        seri_numarasi_id = select_devredilecek_firma.VALUE(0).toInt();

        if (islem_goren_seri_no_id_list.contains(QVariant(seri_numarasi_id).toString()) EQ false) {
            continue;
        }

        insert_arsiv_firma.PREPARE_INSERT("stk_urunun_seri_nolari" , "seri_numarasi_id" ,
                                          "seri_numarasi_id,depo_id,urun_id,seri_numarasi,depoda_mi");

        insert_arsiv_firma.SET_VALUE(":seri_numarasi_id" , select_devredilecek_firma.VALUE(0).toInt());
        insert_arsiv_firma.SET_VALUE(":depo_id"          , select_devredilecek_firma.VALUE(1).toInt());
        insert_arsiv_firma.SET_VALUE(":urun_id"          , select_devredilecek_firma.VALUE(2).toInt());
        insert_arsiv_firma.SET_VALUE(":seri_numarasi"    , select_devredilecek_firma.VALUE(3).toString());
        insert_arsiv_firma.SET_VALUE(":depoda_mi"        , select_devredilecek_firma.VALUE(4).toInt());

        insert_arsiv_firma.INSERT();

    }
    if ( seri_numarasi_id > 0 ) {
        ARSIV_DB->SET_TABLE_AUTO_INCREMENT_ID_VALUE ( "stk_urunun_seri_nolari" , "seri_numarasi_id", seri_numarasi_id);
    }
}
/*****************************************************************************************/
/*             E9_DEVIR_ISLEMI_BATCH::IRS_IADE_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA();                        */
/*****************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::IRS_IADE_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA(int irs_satir_id)
{
    SQL_QUERY select_devredilecek_firma(DB);

    select_devredilecek_firma.PREPARE_SELECT("irs_iade_satir_baglantisi ","irs_iade_edilen_satir_id",
                                             "irs_satir_id = :irs_satir_id");
    select_devredilecek_firma.SET_VALUE(":irs_satir_id" , irs_satir_id);

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        return;
    }

    SQL_QUERY insert_arsiv_firma(ARSIV_DB);

    while ( select_devredilecek_firma.NEXT() ){

        insert_arsiv_firma.PREPARE_INSERT("irs_iade_satir_baglantisi" , "kayit_id" , "irs_satir_id,"
                                          "irs_iade_edilen_satir_id");

        insert_arsiv_firma.SET_VALUE(":irs_satir_id"             , irs_satir_id);
        insert_arsiv_firma.SET_VALUE(":irs_iade_edilen_satir_id" , select_devredilecek_firma.VALUE(0).toInt());

        insert_arsiv_firma.INSERT();
    }
}



/*****************************************************************************************/
/*             E9_DEVIR_ISLEMI_BATCH::STOK_ACILIS_FISI_OLUSTUR();                        */
/*****************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::STOK_ACILIS_FISI_OLUSTUR ( )
{

    SQL_QUERY       query_arsiv_db(ARSIV_DB);
    SQL_QUERY       select_query(DB);
    SQL_QUERY       insert_devredilecek_firma(DB);
    QStringList     depolar_list;


    double      depoya_giren_urun_miktari[E9_ARRAY_SIZE];
    double      depodan_cikan_urun_miktari[E9_ARRAY_SIZE];

    //Firmanin subelerine ait olan tum depolar bulunur.
    depolar_list = GET_DEPOLAR_LIST();

    if ( depolar_list.isEmpty() EQ true) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Stok Modülünde , Açılış Fişi Oluşturulamadı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }


    int old_depo_id = 0;

    for (int i = 0 ; i < depolar_list.size() ; i++) {

        int depo_id = QVariant(depolar_list.at(i)).toInt();

        //31 aralikta depodaki urun miktarina bakilir.Eger depoda urun varsa,acilis fisi olusturulur.
        //Her depo icin ayri acilis fisi olusturulur.

        int gun_no = MALI_YIL_ARRAY_INDIS( m_arsiv_firma_kapanis_tarihi );

        //depodaki urunler tablosunun kayit idsi bulunur.

        query_arsiv_db.PREPARE_SELECT("stk_depodaki_urunler" , "urun_id, toplam_depoya_giren_array,"
                             "toplam_depodan_cikan_array" , "depo_id = :depo_id");

        query_arsiv_db.SET_VALUE(":depo_id" , depo_id);


        int order_number       = 0;
        int stk_acilis_fisi_id = 0;

        if ( query_arsiv_db.SELECT() NE 0 ) {
            while ( query_arsiv_db.NEXT()) {

                int urun_id     = query_arsiv_db.VALUE(0).toInt();

                UNPACK_DOUBLE_ARRAY(query_arsiv_db.VALUE("toplam_depoya_giren_array").toString() , depoya_giren_urun_miktari , E9_ARRAY_SIZE);
                UNPACK_DOUBLE_ARRAY(query_arsiv_db.VALUE("toplam_depodan_cikan_array").toString() , depodan_cikan_urun_miktari , E9_ARRAY_SIZE);

                double giren_urun_miktari = depoya_giren_urun_miktari[gun_no];
                double cikan_urun_miktari = depodan_cikan_urun_miktari[gun_no];

                double depodaki_urun_miktari = ROUND(giren_urun_miktari) - ROUND(cikan_urun_miktari);

                if ( depodaki_urun_miktari EQ 0 ) {
                    continue;
                }
                 //Depoda urun varsa o depo icin acilis fisi olusturulur.


                if (old_depo_id NE depo_id) {
                    old_depo_id = depo_id;

                    //Depo id ayni oldugu surece tek bir acilis fisi olusturulacak.
                     //HEr yeni acilis fisinde fis numarasi arttirilacak.
                    m_calisan_firma_last_stk_fis_no++;

                    insert_devredilecek_firma.PREPARE_INSERT("stk_fisler" , "fis_id" , "fis_no,fis_tarihi,aciklama,"
                                                             "irsaliye_mi_stok_fisi_mi,fis_turu,depo_id,modul_id , program_id,"
                                                             "base_fis_id,iade_irsaliyesi_mi");

                    insert_devredilecek_firma.SET_VALUE(":fis_no"       , m_calisan_firma_last_stk_fis_no);
                    insert_devredilecek_firma.SET_VALUE(":fis_tarihi"   , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));
                    insert_devredilecek_firma.SET_VALUE(":aciklama"     , tr("Açılış Fişi- Devir işlemi sırasında otomatik olarak oluşturulmuştur."));
                    insert_devredilecek_firma.SET_VALUE(":irsaliye_mi_stok_fisi_mi" , ENUM_STOK_FISI);
                    insert_devredilecek_firma.SET_VALUE(":fis_turu"     , ENUM_STK_STOK_ACILIS_FISI);
                    insert_devredilecek_firma.SET_VALUE(":depo_id"      , depo_id);
                    insert_devredilecek_firma.SET_VALUE(":modul_id"     , STOK_MODULU);
                    insert_devredilecek_firma.SET_VALUE(":program_id"   , E9_PROGRAMI);
                    insert_devredilecek_firma.SET_VALUE(":base_fis_id"  , 0);
                    insert_devredilecek_firma.SET_VALUE(":iade_irsaliyesi_mi" , 0);

                    stk_acilis_fisi_id = insert_devredilecek_firma.INSERT();
                }
                //Stok acilis fisinde satirlar ekleniyor.

                select_query.PREPARE_SELECT("stk_urunler","urun_temel_birimi,stok_takip_sekli" ,"urun_id = :urun_id");
                select_query.SET_VALUE(":urun_id" , urun_id);

                QString urun_temel_birimi;
                int stok_takip_sekli = -1;
                if (select_query.SELECT() NE 0 ) {
                    select_query.NEXT();
                    urun_temel_birimi = select_query.VALUE(0).toString();
                    stok_takip_sekli  = select_query.VALUE(1).toInt();
                }
                if ( stok_takip_sekli EQ ENUM_PARTI_TAKIBI) {
                    //urune ait olan partiler bulunur.

                    SQL_QUERY sql_query(ARSIV_DB);

                    //Depodaki urun birden fazla partide bulunabilir.her parti icin ayri satir eklenmelidir.
                    //urune ait partiler bulunur.
                    sql_query.PREPARE_SELECT("stk_urunun_partileri ","parti_id , toplam_partiye_giren_array,"
                                             "toplam_partiden_cikan_array" ,
                                             "urun_id = :urun_id AND depo_id = :depo_id");
                    sql_query.SET_VALUE(":urun_id" , urun_id);
                    sql_query.SET_VALUE(":depo_id" , depo_id);

                    if ( sql_query.SELECT() NE 0 ) {

                        int gun_no = MALI_YIL_ARRAY_INDIS ( m_arsiv_firma_kapanis_tarihi );

                        double partiye_giren_urun_miktari[E9_ARRAY_SIZE];
                        double partiden_cikan_urun_miktari[E9_ARRAY_SIZE];

                        while (sql_query.NEXT()) {

                             int parti_id = sql_query.VALUE(0).toInt();

                             UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_partiye_giren_array").toString() , partiye_giren_urun_miktari ,E9_ARRAY_SIZE);
                             UNPACK_DOUBLE_ARRAY(sql_query.VALUE("toplam_partiden_cikan_array").toString() , partiden_cikan_urun_miktari , E9_ARRAY_SIZE);

                            //partinin 31 aralikta depodaki miktari bulunacak ve parti id turu kadar acilis fisi satiri eklenecek.

                            double partideki_urun_miktari = partiye_giren_urun_miktari[gun_no] - partiden_cikan_urun_miktari[gun_no];

                            if ( partideki_urun_miktari EQ 0 ) {
                                continue;
                            }
                            order_number += 1024;

                            insert_devredilecek_firma.PREPARE_INSERT("stk_fis_satirlari" , "fis_satiri_id",
                                                                     "fis_id,order_number,urun_id,satirdaki_urun_miktari,"
                                                                     "urun_birimi,parti_id,temel_birim_katsayisi,modul_id,"
                                                                     "program_id , base_record_id");


                            insert_devredilecek_firma.SET_VALUE(":fis_id"                 , stk_acilis_fisi_id);
                            insert_devredilecek_firma.SET_VALUE(":order_number"           , order_number);
                            insert_devredilecek_firma.SET_VALUE(":urun_id"                , urun_id);
                            insert_devredilecek_firma.SET_VALUE(":satirdaki_urun_miktari" , partideki_urun_miktari);
                            insert_devredilecek_firma.SET_VALUE(":urun_birimi"            , urun_temel_birimi);
                            insert_devredilecek_firma.SET_VALUE(":parti_id"               , parti_id );
                            insert_devredilecek_firma.SET_VALUE(":temel_birim_katsayisi"  , 1);
                            insert_devredilecek_firma.SET_VALUE(":modul_id"               , STOK_MODULU);
                            insert_devredilecek_firma.SET_VALUE(":program_id"             , E9_PROGRAMI);
                            insert_devredilecek_firma.SET_VALUE(":base_record_id"         , 0);

                            insert_devredilecek_firma.INSERT();
                        }
                    }
                }
                else if(stok_takip_sekli EQ ENUM_BASIT_TAKIP){
                    order_number += 1024;

                    insert_devredilecek_firma.PREPARE_INSERT("stk_fis_satirlari" , "fis_satiri_id" ,
                                                             "fis_id,order_number,urun_id,satirdaki_urun_miktari,"
                                                             "urun_birimi,parti_id,temel_birim_katsayisi ,modul_id,"
                                                             "program_id , base_record_id");

                    insert_devredilecek_firma.SET_VALUE(":fis_id"                 , stk_acilis_fisi_id);
                    insert_devredilecek_firma.SET_VALUE(":order_number"           , order_number);
                    insert_devredilecek_firma.SET_VALUE(":urun_id"                , urun_id);
                    insert_devredilecek_firma.SET_VALUE(":satirdaki_urun_miktari" , depodaki_urun_miktari);
                    insert_devredilecek_firma.SET_VALUE(":urun_birimi"            , urun_temel_birimi);
                    insert_devredilecek_firma.SET_VALUE(":parti_id"               , 0);
                    insert_devredilecek_firma.SET_VALUE(":temel_birim_katsayisi"  , 1);
                    insert_devredilecek_firma.SET_VALUE(":modul_id"               , STOK_MODULU);
                    insert_devredilecek_firma.SET_VALUE(":program_id"             , E9_PROGRAMI);
                    insert_devredilecek_firma.SET_VALUE(":base_record_id"         , 0);

                    insert_devredilecek_firma.INSERT();
                }
                else {
                    order_number += 1024;

                    insert_devredilecek_firma.PREPARE_INSERT("stk_fis_satirlari" , "fis_satiri_id" ,
                                                             "fis_id,order_number,urun_id,satirdaki_urun_miktari,"
                                                             "urun_birimi,parti_id,temel_birim_katsayisi,modul_id,"
                                                             "program_id , base_record_id");

                    insert_devredilecek_firma.SET_VALUE(":fis_id"                 , stk_acilis_fisi_id);
                    insert_devredilecek_firma.SET_VALUE(":order_number"        , order_number);
                    insert_devredilecek_firma.SET_VALUE(":urun_id"                , urun_id);
                    insert_devredilecek_firma.SET_VALUE(":satirdaki_urun_miktari" , 0);
                    insert_devredilecek_firma.SET_VALUE(":urun_birimi"            , urun_temel_birimi);
                    insert_devredilecek_firma.SET_VALUE(":parti_id"               , 0);
                    insert_devredilecek_firma.SET_VALUE(":temel_birim_katsayisi"  , 1);
                    insert_devredilecek_firma.SET_VALUE(":modul_id"               , STOK_MODULU);
                    insert_devredilecek_firma.SET_VALUE(":program_id"             , E9_PROGRAMI);
                    insert_devredilecek_firma.SET_VALUE(":base_record_id"         , 0);

                    int satir_id = insert_devredilecek_firma.INSERT();
                    //seri numarasi ile takip ise
                    select_query.PREPARE_SELECT("stk_urunun_seri_nolari ","seri_numarasi_id",
                                                "depoda_mi = :depoda_mi AND depo_id = :depo_id AND urun_id = :urun_id");
                    select_query.SET_VALUE(":depoda_mi" , 1);
                    select_query.SET_VALUE(":depo_id" , depo_id);
                    select_query.SET_VALUE(":urun_id" , urun_id);
                    int seri_numarasi_id = 0;

                    if(select_query.SELECT() EQ 0 ) {
                       continue;
                    }
                    int order_number = 0;
                    double urun_miktari = 0.0;
                    while (select_query.NEXT()) {
                        seri_numarasi_id = select_query.VALUE(0).toInt();

                        order_number += 1024;

                        insert_devredilecek_firma.PREPARE_INSERT("stk_satirdaki_seri_nolari" , "satir_seri_no_id",
                                                                 "irsaliye_satiri_id,seri_numarasi_id,order_number");

                        insert_devredilecek_firma.SET_VALUE(":irsaliye_satiri_id" , satir_id);
                        insert_devredilecek_firma.SET_VALUE(":seri_numarasi_id" , seri_numarasi_id);
                        insert_devredilecek_firma.SET_VALUE(":order_number" , order_number);
                        insert_devredilecek_firma.INSERT();
                        urun_miktari += 1;
                    }
                    SQL_QUERY update_devredilecek_firma(DB);

                    update_devredilecek_firma.PREPARE_UPDATE("stk_fis_satirlari","fis_satiri_id", "satirdaki_urun_miktari",
                                                             "fis_satiri_id = :fis_satiri_id");
                    update_devredilecek_firma.SET_VALUE(":satirdaki_urun_miktari" , urun_miktari);
                    update_devredilecek_firma.SET_VALUE(":fis_satiri_id" , satir_id);
                    update_devredilecek_firma.UPDATE();
                }
            }
        }
    }



    //Acilis fisleri olusturulduktan sonra sabit degerlerdeki fis_no guncellenir.

    //Stok acilis fisinde fiyat bilgisi olmayacak.

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Stok Modülünde , Açılış Fişi Oluşturuldu.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/***************************************************************************************/
/*            E9_DEVIR_ISLEMI_BATCH::FATURA_MODULUNU_DEVRET();                         */
/***************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::FATURA_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** FATURA MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Fatura Modülü , Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    FATURALARI_KOPYALA();

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Fatura Modülü , Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/***************************************************************************************/
/*                   E9_DEVIR_ISLEMI_BATCH::FATURALARI_KOPYALA();                      */
/***************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::FATURALARI_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma (DB);
    SQL_QUERY select_devredilecek_firma_satirlar(DB);

    QStringList where_list;

    QString where_str = QString("fatura_tarihi < :arg_%1 AND ( modul_id = :arg_%2 AND program_id = :arg_%3 )"
                                "OR ( program_id != :arg_%4 )").arg(where_list.size())
                                                          .arg(where_list.size() + 1)
                                                          .arg(where_list.size() + 2)
                                                          .arg(where_list.size() + 3);

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");
    where_list << QString::number(FATURA_MODULU)<< QString::number(E9_PROGRAMI) << QString::number(E9_PROGRAMI);


    E9_COPY_TABLES(DB ,ARSIV_DB , "fat_faturalar" , where_str , where_list);

    select_devredilecek_firma.PREPARE_SELECT("fat_faturalar" , "fis_id" ,
                                             "fatura_tarihi < :fatura_tarihi AND ( modul_id = :modul_id_1 "
                                             "AND program_id = :program_id_1 )"
                                             "OR ( program_id != :program_id_2 )" );
    select_devredilecek_firma.SET_VALUE(":fatura_tarihi"   , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));
    select_devredilecek_firma.SET_VALUE(":modul_id"     , FATURA_MODULU);
    select_devredilecek_firma.SET_VALUE(":program_id_1" , E9_PROGRAMI);
    select_devredilecek_firma.SET_VALUE(":program_id"   , E9_PROGRAMI);

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Arşiv Firmaya Kopyalanacak Fatura Kaydı Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Faturalar , Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    while (select_devredilecek_firma.NEXT()) {

        int fatura_id = select_devredilecek_firma.VALUE(0).toInt();


        FAT_IRS_BAGLANTISI_TABLOSUNU_KOPYALA(fatura_id);

        select_devredilecek_firma_satirlar.PREPARE_SELECT("fat_fatura_satirlari" , "fis_satiri_id" ,"fis_id = :fis_id");

        select_devredilecek_firma_satirlar.SET_VALUE(":fis_id" , fatura_id);

        if ( select_devredilecek_firma_satirlar.SELECT() EQ 0 ) {
            continue;
        }
        while (select_devredilecek_firma_satirlar.NEXT()) {

            int fatura_satiri_id = select_devredilecek_firma_satirlar.VALUE(0).toInt();

            FAT_IRS_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA(fatura_satiri_id);
            FAT_IADE_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA(fatura_satiri_id);
        }

        where_list.clear();

        where_list << QVariant(fatura_id).toString();

        E9_COPY_TABLES ( DB ,ARSIV_DB , "fat_fatura_satirlari" , QString("fis_id = :arg_%1").arg(0) ,
                              where_list);

    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Faturalar,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/*******************************************************************************************/
/*          E9_DEVIR_ISLEMI_BATCH::FAT_IRS_BAGLANTISI_TABLOSUNU_KOPYALA();                */
/******************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::FAT_IRS_BAGLANTISI_TABLOSUNU_KOPYALA(int fatura_id)
{
    SQL_QUERY select_devredilecek_firma(DB);
    SQL_QUERY insert_arsiv_firma(ARSIV_DB);


    select_devredilecek_firma.PREPARE_SELECT("fat_irs_baglantisi","irsaliye_id" ,"fatura_id = :fatura_id ");
    select_devredilecek_firma.SET_VALUE(":fatura_id" , fatura_id);

    if(select_devredilecek_firma.SELECT() EQ 0 ) {
        return;
    }
    while (select_devredilecek_firma.NEXT()) {

        insert_arsiv_firma.PREPARE_INSERT("fat_irs_baglantisi" , "kayit_id" , "fatura_id,irsaliye_id");

        insert_arsiv_firma.SET_VALUE(":fatura_id"   , fatura_id);
        insert_arsiv_firma.SET_VALUE(":irsaliye_id" , select_devredilecek_firma.VALUE(0).toInt());

        insert_arsiv_firma.INSERT();
    }

}

/*********************************************************************************************/
/*          E9_DEVIR_ISLEMI_BATCH::FAT_IRS_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA();             */
/*********************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::FAT_IRS_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA(int fatura_satiri_id)
{
    SQL_QUERY select_devredilecek_firma(DB);
    SQL_QUERY insert_arsiv_firma(ARSIV_DB);


    select_devredilecek_firma.PREPARE_SELECT("fat_irs_satir_baglantisi","irsaliye_satiri_id",
                                             "fatura_satiri_id = :fatura_satiri_id");
    select_devredilecek_firma.SET_VALUE(":fatura_satiri_id" , fatura_satiri_id);

    if(select_devredilecek_firma.SELECT() EQ 0 ) {
        return;
    }
    while (select_devredilecek_firma.NEXT()) {

        insert_arsiv_firma.PREPARE_INSERT("fat_irs_satir_baglantisi" , "kayit_id" , "fatura_satiri_id,"
                                          "irsaliye_satiri_id");

        insert_arsiv_firma.SET_VALUE(":fatura_satiri_id"   , fatura_satiri_id);
        insert_arsiv_firma.SET_VALUE(":irsaliye_satiri_id" , select_devredilecek_firma.VALUE(0).toInt());

        insert_arsiv_firma.INSERT();

    }
}
/*********************************************************************************************/
/*          E9_DEVIR_ISLEMI_BATCH::FAT_IADE_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA();             */
/*********************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::FAT_IADE_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA(int fatura_satiri_id)
{
    SQL_QUERY select_devredilecek_firma (DB);

    select_devredilecek_firma.PREPARE_SELECT("fat_iade_satir_baglantisi ","fat_iade_edilen_satir_id",
                                             "fat_satir_id = :fat_satir_id");
    select_devredilecek_firma.SET_VALUE(":fat_satir_id" , fatura_satiri_id);

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        return;
    }
    SQL_QUERY insert_arsiv_firma(ARSIV_DB);

    while ( select_devredilecek_firma.NEXT() ) {

        insert_arsiv_firma.PREPARE_INSERT("fat_iade_satir_baglantisi" , "kayit_id" , "fat_iade_edilen_satir_id,"
                                          "fat_satir_id");

        insert_arsiv_firma.SET_VALUE(":fat_iade_edilen_satir_id" , select_devredilecek_firma.VALUE(0).toInt());
        insert_arsiv_firma.SET_VALUE(":fat_satir_id"             , fatura_satiri_id);

        insert_arsiv_firma.INSERT();
    }
}



/*******************************************************************************************/
/*          E9_DEVIR_ISLEMI_BATCH::PERAKENDE_MODULUNU_DEVRET();                            */
/******************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::PERAKENDE_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** PERAKENDE MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Perakende Modülü , Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Perakende Gelir/Gider Türleri Tablosu,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
    E9_COPY_TABLES(DB ,ARSIV_DB , "prk_gelir_gider_turleri" ,"");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Perakende Gelir/Gider Türleri,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    PERAKENDE_MODULU_FISLERINI_KOPYALA();

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Perakende Modülü , Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}


/*******************************************************************************************/
/*          E9_DEVIR_ISLEMI_BATCH::PERAKENDE_MODULU_FISLERINI_KOPYALA();                   */
/******************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::PERAKENDE_MODULU_FISLERINI_KOPYALA()
{
    SQL_QUERY select_devredilecek_firma(DB);

    QStringList where_list;

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

    E9_COPY_TABLES(DB ,ARSIV_DB , "prk_fisler" , QString("fis_tarihi < :arg_%1").arg(0) , where_list);



    select_devredilecek_firma.PREPARE_SELECT("prk_fisler" , "fis_id" , "fis_tarihi < :fis_tarihi");

    select_devredilecek_firma.SET_VALUE(":fis_tarihi" , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Perakende Fişleri Tablosunda Kayıt Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Perakende Fişleri,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    while ( select_devredilecek_firma.NEXT() ) {

        where_list.clear();

        where_list << select_devredilecek_firma.VALUE(0).toString();

        E9_COPY_TABLES(DB ,ARSIV_DB , "prk_fis_satirlari" , QString("fis_id = :arg_%1").arg(0) , where_list);

    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Perakende Fişleri,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/*******************************************************************************************/
/*          E9_DEVIR_ISLEMI_BATCH::PERSONEL_MODULUNU_DEVRET();                           */
/******************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::PERSONEL_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** PERSONEL MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Personel Modülü,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB , ARSIV_DB , "prs_sabit_degerler" , "");
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Personel Sabit Değerler Tablosu , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    E9_COPY_TABLES(DB , ARSIV_DB , "prs_isten_ayrilanlar" ,
                          QString("isten_ayrilma_tarihi <= '%1'").arg(m_arsiv_firma_kapanis_tarihi.toString("yyyy.MM.dd")));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Personel İşten Ayrılanlar Tablosu , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB , ARSIV_DB , "prs_firma_odenekleri" , "");

    E9_COPY_TABLES(DB , ARSIV_DB , "prs_kesinti_odenek_tanimlar" , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Personel Firma Kesinti / Ödenekleri Tablosu , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    PRS_PERSONELLER_TABLOSUNU_KOPYALA();

    PRS_BORDROLAR_TABLOSUNU_KOPYALA();

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Personel Modülü,Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/******************************************************************************************/
/*                E9_DEVIR_ISLEMI_BATCH::PRS_BORDROLAR_TABLOSUNU_KOPYALA();                    */
/******************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::PRS_BORDROLAR_TABLOSUNU_KOPYALA()
{
    SQL_QUERY query_db(DB);
    SQL_QUERY s_query_db(DB);

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Personel Modülü Bordrolari arşiv firmaya kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB , ARSIV_DB , "prs_bordrolar" , QString("bordro_kesim_tarihi < '%1'").arg(m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd")));


    query_db.PREPARE_SELECT("prs_bordrolar" , "bordro_id" , "bordro_kesim_tarihi < :bordro_kesim_tarihi");

    query_db.SET_VALUE(":bordro_kesim_tarihi" , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));

    if ( query_db.SELECT() EQ 0 ) {
        return;
    }

    int bordro_id = 0;

    while ( query_db.NEXT() ) {

        bordro_id = query_db.VALUE("bordro_id").toInt();

        E9_COPY_TABLES(DB , ARSIV_DB , "prs_bordro_personelleri" , QString("bordro_id = %1").arg(bordro_id));

        E9_COPY_TABLES(DB , ARSIV_DB , "prs_brd_kesinti_odenekleri" , QString("bordro_id = %1").arg(bordro_id));

        s_query_db.PREPARE_SELECT("prs_bordro_personelleri" , "puantaj_id,personel_id" , "bordro_id = :bordro_id");
        s_query_db.SET_VALUE(":bordro_id" , bordro_id);

        if ( s_query_db.SELECT() NE 0 ) {
            s_query_db.NEXT();

            E9_COPY_TABLES(DB ,ARSIV_DB , "prs_puantaj" , QString("puantaj_id = %1").arg(s_query_db.VALUE(0).toInt()));

            E9_COPY_TABLES(DB ,ARSIV_DB , "prs_eksik_gun_nedenleri" , QString("personel_id = %1").arg(s_query_db.VALUE(1).toInt()));
        }
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Personel Modülü Bordrolari arşiv firmaya kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}

/*******************************************************************************************/
/*          E9_DEVIR_ISLEMI_BATCH::PRS_PERSONELLER_TABLOSUNU_KOPYALA();                    */
/******************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::PRS_PERSONELLER_TABLOSUNU_KOPYALA()
{

    E9_COPY_TABLES(DB ,ARSIV_DB , "prs_personeller" , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Personel Modülü,Personeller Tablosu Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    //PERSONEL KISISEL BILGILERI
    E9_COPY_TABLES(DB , ARSIV_DB , "prs_personel_kisisel_blgler" ,"");

    //PERSONEL NOT BILGILERI
    E9_COPY_TABLES(DB ,ARSIV_DB , "prs_personel_notlari" ,"" );

    //PERSONEL NUFUS BILGILERI

    E9_COPY_TABLES(DB ,ARSIV_DB , "prs_personel_nufus_blgler" ,"" );

    //PERSONEL AILE UYELERI

    E9_COPY_TABLES(DB ,ARSIV_DB , "prs_personel_aile_uyeleri" ,"" );

    //PERSONEL EK ODENEKLER/KESINTILER

    E9_COPY_TABLES(DB ,ARSIV_DB , "prs_ek_kesintiler_odenekler" ,"" );

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Personel Modülü,Personeller Tablosu Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

}


/******************************************************************************************/
/*                E9_DEVIR_ISLEMI_BATCH::FIND_UYGUN_ACILIS_BORDROSU()                    */
/******************************************************************************************/

int E9_DEVIR_ISLEMI_BATCH::FIND_UYGUN_ACILIS_BORDROSU ( int bordro_islem_turu, int cek_senet_turu, int hesap_no_id )
{
    SQL_QUERY select_devredilecek_firma(DB);

    int cek_bordrosu_mu = -1;

    if ( cek_senet_turu EQ ENUM_CEK) {
        cek_bordrosu_mu = 1;
    }
    else {
        cek_bordrosu_mu = 0;
    }

    QString where_str = "bordro_turu =:bordro_turu "
                        "AND cek_bordrosu_mu = :cek_bordrosu_mu AND bordro_tarihi = :bordro_tarihi AND "
                        "bordro_islem_turu = :bordro_islem_turu";


    if ( hesap_no_id NE -1) {
       where_str += QString (" AND hesap_no_id = :hesap_no_id").arg(hesap_no_id);
    }

    select_devredilecek_firma.PREPARE_SELECT("cek_bordrolar" , "bordro_id" , where_str);

    select_devredilecek_firma.SET_VALUE(":bordro_turu"       , ENUM_CEK_SENET_ACILIS_BORDROSU);
    select_devredilecek_firma.SET_VALUE(":cek_bordrosu_mu"   , cek_bordrosu_mu);
    select_devredilecek_firma.SET_VALUE(":bordro_tarihi"     , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));
    select_devredilecek_firma.SET_VALUE(":bordro_islem_turu" , bordro_islem_turu);

    if ( hesap_no_id NE -1 ) {
        select_devredilecek_firma.SET_VALUE(":hesap_no_id" , hesap_no_id);
    }

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        return -1;
    }
    select_devredilecek_firma.NEXT();

    return select_devredilecek_firma.VALUE(0).toInt();
}


/********************************************************************************************/
/*             E9_DEVIR_ISLEMI_BATCH::CIRO_EDILEN_CEK_ODENDI_KABUL_EDILECEK_MI()           */
/********************************************************************************************/

bool E9_DEVIR_ISLEMI_BATCH::CIRO_EDILEN_CEK_ODENDI_KABUL_EDILECEK_MI(int cek_senet_id , QDate islem_tarihi)
{
    SQL_QUERY select_devredilecek_firma(DB);

    select_devredilecek_firma.PREPARE_SELECT("e9_sabit_degerler " , "cek_ciro_sonrasi_gun_sayisi");

    int ciro_sonrasi_gun_sayisi = 15;

    if(select_devredilecek_firma.SELECT() NE 0 ) {
        select_devredilecek_firma.NEXT();
        ciro_sonrasi_gun_sayisi = select_devredilecek_firma.VALUE(0).toInt();
    }

    select_devredilecek_firma.PREPARE_SELECT("cek_bordrolar,cek_bordro_satirlari" , "bordro_tarihi" ,
                                             "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id "
                                             "AND bordro_islem_turu = :bordro_islem_turu AND cek_senet_id = :cek_senet_id");

    select_devredilecek_firma.SET_VALUE(":bordro_islem_turu" , ENUM_CEK_SATICIYA_VERILDI);
    select_devredilecek_firma.SET_VALUE(":cek_senet_id"      , cek_senet_id);

    if(select_devredilecek_firma.SELECT("bordro_tarihi DESC" , 0,1) EQ 0 ) {
         return false;
    }
    select_devredilecek_firma.NEXT();

    QDate bordro_tarihi = QDate::fromString(select_devredilecek_firma.VALUE(0).toString(),"yyyy.MM.dd");



    int gun_sayisi = bordro_tarihi.daysTo(islem_tarihi);
    if(gun_sayisi < ciro_sonrasi_gun_sayisi) {
        return false;
    }
    return true;
}

/******************************************************************************/
/*       E9_DEVIR_ISLEMI_BATCH::CEK_SENET_ARSIV_FIRMADA_ISLEM_GORDU_MU()      */
/******************************************************************************/

bool E9_DEVIR_ISLEMI_BATCH::CEK_SENET_ARSIV_FIRMADA_ISLEM_GORDU_MU(int cek_senet_id)
{
    SQL_QUERY select_devredilecek_firma(DB);

    select_devredilecek_firma.PREPARE_SELECT("cek_bordrolar,cek_bordro_satirlari ","cek_bordrolar.bordro_id ",
                                             "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id "
                                             "AND cek_senet_id = :cek_senet_id AND "
                                             "bordro_tarihi < :bordro_tarihi");

    select_devredilecek_firma.SET_VALUE(":cek_senet_id" , cek_senet_id);
    select_devredilecek_firma.SET_VALUE(":bordro_tarihi" , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));

    if (select_devredilecek_firma.SELECT() EQ 0 ) {
        return false;
    }
    return true;
}
/*********************************************************************************/
/*        E9_DEVIR_ISLEMI_BATCH::CEK_SENET_CALISAN_FIRMADA_ISLEM_GORDU_MU()      */
/*********************************************************************************/

bool E9_DEVIR_ISLEMI_BATCH::CEK_SENET_CALISAN_FIRMADA_ISLEM_GORDU_MU(int cek_senet_id)
{
    SQL_QUERY select_devredilecek_firma(DB);

    QString bordro_tarihi = m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

    select_devredilecek_firma.PREPARE_SELECT("cek_bordrolar,cek_bordro_satirlari ","cek_bordrolar.bordro_id",
                                             "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id "
                                             "AND cek_senet_id = :cek_senet_id AND "
                                             "bordro_tarihi >= :bordro_tarihi");
    select_devredilecek_firma.SET_VALUE(":cek_senet_id"    , cek_senet_id);
    select_devredilecek_firma.SET_VALUE(":bordro_tarihi" , bordro_tarihi);

    if (select_devredilecek_firma.SELECT() EQ 0 ) {
        return false;
    }
    return true;

}

/***********************************************************************************/
/*           E9_DEVIR_ISLEMI_BATCH::GET_SUBELER_VE_SUBENIN_DEPOLARI_LIST()         */
/***********************************************************************************/

QStringList E9_DEVIR_ISLEMI_BATCH::GET_DEPOLAR_LIST()
{
    SQL_QUERY select_devredileck_firma(DB);
    SQL_QUERY select_query(DB);
    QStringList depolar_list;

    select_devredileck_firma.PREPARE_SELECT("sub_subeler","sube_id");

    if(select_devredileck_firma.SELECT() EQ 0 ) {
        return QStringList();
    }
    while (select_devredileck_firma.NEXT()) {
        int sube_id = select_devredileck_firma.VALUE(0).toInt();



        select_query.PREPARE_SELECT("sub_depolar","depo_id" ,"sube_id = :sube_id");
        select_query.SET_VALUE(":sube_id" , sube_id);

        if(select_query.SELECT() EQ 0) {
            continue;
        }
        while (select_query.NEXT()) {

            depolar_list << select_query.VALUE(0).toString();
        }
    }
    if(depolar_list.size() EQ 0 ) {
        return QStringList();
    }
    return depolar_list;
}
/**********************************************************************************/
/*           E9_DEVIR_ISLEMI_BATCH::GET_YILA_GORE_SERI_NUMARALARI_ID_LIST()       */
/**********************************************************************************/

QStringList E9_DEVIR_ISLEMI_BATCH::GET_YILA_GORE_SATIRDAKI_SERI_NUMARALARI_ID_LIST(QDate fis_tarihi)
{
    SQL_QUERY query(DB);
    SQL_QUERY select_query(DB);
    QStringList seri_numaralari_id_list;


    QString sql_query = QString("stk_fis_satirlari.fis_id = stk_fisler.fis_id ");

    if(fis_tarihi.year() EQ m_muhasebe_yili) {//Arsiv firmaya ait irsaliye satirlari
        sql_query += QString(" AND fis_tarihi <= :fis_tarihi");
    }
    else {//calisan firmaya ait irsaliye satirlari
        sql_query += QString(" AND fis_tarihi >= :fis_tarihi");
    }
    query.PREPARE_SELECT("stk_fis_satirlari,stk_fisler","fis_satiri_id", sql_query);
    query.SET_VALUE(":fis_tarihi" , fis_tarihi.toString("yyyy.MM.dd"));

    if(query.SELECT() EQ 0 ) {
        return QStringList();
    }
    while (query.NEXT()) {
        int fis_satiri_id = query.VALUE(0).toInt();

        select_query.PREPARE_SELECT("stk_satirdaki_seri_nolari","seri_numarasi_id",
                                    "irsaliye_satiri_id = :irsaliye_satiri_id");
        select_query.SET_VALUE(":irsaliye_satiri_id" , fis_satiri_id);

        if(select_query.SELECT() EQ 0 ) {
            continue;
        }
        while (select_query.NEXT()) {
            seri_numaralari_id_list << select_query.VALUE(0).toString();
        }
    }
    if(seri_numaralari_id_list.size() EQ 0 ) {
        return QStringList();
    }
    return seri_numaralari_id_list;
}

/*************************************************************************************************
                                 GET_YILA_GORE_SATIRDAKI_PARTI_ID_LIST
*************************************************************************************************/

QStringList E9_DEVIR_ISLEMI_BATCH::GET_YILA_GORE_SATIRDAKI_PARTI_ID_LIST(QDate fis_tarihi)
{
    SQL_QUERY query(DB);
    QStringList parti_id_list;

    QString sql_query = QString("stk_fis_satirlari.fis_id = stk_fisler.fis_id ");

    if ( fis_tarihi.year() EQ m_muhasebe_yili) {//Arsiv firmaya ait irsaliye satirlari
        sql_query += QString(" AND fis_tarihi <= :fis_tarihi");
    }
    else {//calisan firmaya ait irsaliye satirlari
        sql_query += QString(" AND fis_tarihi >= :fis_tarihi");
    }
    query.PREPARE_SELECT("stk_fis_satirlari,stk_fisler" , "parti_id" , sql_query);
    query.SET_VALUE(":fis_tarihi" , fis_tarihi.toString("yyyy.MM.dd"));

    if(query.SELECT() EQ 0 ) {
        return QStringList();
    }
    while (query.NEXT()) {
        parti_id_list << query.VALUE(0).toString();
    }
    if(parti_id_list.size() EQ 0 ) {
        return QStringList();
    }
    return parti_id_list;
}



/*************************************************************************************************
                                 HESAP_BAKIYELERINI_SIFIRLA
*************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::HESAP_BAKIYELERINI_SIFIRLA(QString p_table_name, QString id_column_name,
                                                       QStringList column_names,
                                                       int values_size , ADAK_SQL * P_GELEN_DB)
{
    SQL_QUERY query(P_GELEN_DB);

    double    toplam_borc_alacak[E9_ARRAY_SIZE];

    for (int i = 0 ; i < E9_ARRAY_SIZE ; i++) {
        toplam_borc_alacak[i]         = 0;
    }
    QString columns;

    for ( int i = 0 ; i < column_names.size() ; i++ ) {
        if ( i NE 0 ) {
            columns.append(",");
        }
        columns.append(column_names.at(i));
    }

    query.PREPARE_UPDATE(p_table_name, id_column_name,
                         columns , "");

    for ( int i = 0 ; i < values_size ; i++ ) {

        query.SET_VALUE(QString(":%1").arg(column_names.at(i)) , PACK_DOUBLE_ARRAY(toplam_borc_alacak   , E9_ARRAY_SIZE));

    }

    query.UPDATE();

}

/*************************************************************************************************
                                 DEMIRBAS_MODULUNU_DEVRET
*************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::DEMIRBAS_MODULUNU_DEVRET ()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** SABİT KIYMET MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sabit Kıymet Modülü , Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB ,ARSIV_DB , "dmr_demirbaslar" , "");
    E9_COPY_TABLES(DB ,ARSIV_DB , "dmr_satilan_demirbaslar" , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sabit Kıymetler Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    DEMIRBAS_MODULU_FISLERINI_KOPYALA();

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sabit Kıymet Modülü , Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/*************************************************************************************************
                                 DEMIRBAS_MODULU_FISLERINI_KOPYALA
*************************************************************************************************/
void  E9_DEVIR_ISLEMI_BATCH::DEMIRBAS_MODULU_FISLERINI_KOPYALA ()
{
    SQL_QUERY select_devredilecek_firma(DB);

    QStringList where_list;

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

    E9_COPY_TABLES(DB ,ARSIV_DB , "dmr_fisler" , QString("fis_tarihi < :arg_%1").arg(0) , where_list);



    select_devredilecek_firma.PREPARE_SELECT("dmr_fisler" , "fis_id" , "fis_tarihi < :fis_tarihi");

    select_devredilecek_firma.SET_VALUE(":fis_tarihi" , m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd"));

    if ( select_devredilecek_firma.SELECT() EQ 0 ) {
        BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sabit Kıymet Modülü Fişleri Tablosunda Kayıt Bulunmamaktadır.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
        return;
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sabit Kıymet Modülü Fişleri,Arşiv Firmaya Kopyalanıyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    while ( select_devredilecek_firma.NEXT() ) {

        where_list.clear();

        where_list << select_devredilecek_firma.VALUE(0).toString();

        E9_COPY_TABLES(DB ,ARSIV_DB , "dmr_fis_satirlari" , QString("fis_id = :arg_%1").arg(0) , where_list);

    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sabit Kıymet Modülü Fişleri,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}


/*************************************************************************************************
                                 SMM_MODULUNU_DEVRET
*************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::SMM_MODULUNU_DEVRET()
{
    BILGILENDIR (QString(" <font color = \"blue\">*** SM MAKBUZ MODÜLÜ ***</font>  <br>"));

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sm Makbuz Modülü , Arşiv Firmaya Devrediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    E9_COPY_TABLES(DB ,ARSIV_DB , "smm_hizmetler" , "");

    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sm Makbuz Hizmetler Tablosu,Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    QStringList where_list;

    where_list << m_calisan_firma_acilis_tarihi.toString("yyyy.MM.dd");

    E9_COPY_TABLES(DB ,ARSIV_DB , "smm_makbuzlar" , QString("makbuz_tarihi < :arg_%1").arg(0) , where_list);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sm Makbuzları , Arşiv Firmaya Kopyalandı.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    BILGILENDIR (QString(" <font color = \"red\">%1</font> - Sm Makbuz Modülü , Arşiv Firmaya Devredildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}

/*************************************************************************************************
                             E9_DEVIR_ISLEMI_BATCH::VERITABANI_BOS_MU
*************************************************************************************************/

bool E9_DEVIR_ISLEMI_BATCH::VERITABANI_BOS_MU( ADAK_SQL *P_DB )
{
    //! Veritabani bos mu ?
    SQL_QUERY query( P_DB );

    //! VERI TABANINDA KAYIT OLUP OLMADIGINA BAKILIYOR.
    query.PREPARE_SELECT( "e9_sabit_degerler", "sabit_deger_id");
    if ( query.SELECT() NE 0 ) {
        return false;
    }
    query.PREPARE_SELECT( "muh_hesaplar", "hesap_id");
    if ( query.SELECT() NE 0 ) {
        return false;
    }
    query.PREPARE_SELECT( "adak_gruplar", "grup_id");
    if ( query.SELECT() NE 0 ) {
        return false;
    }
    query.PREPARE_SELECT( "e9_kullanici_yetkileri", "kullanici_id");

    if ( query.SELECT() NE 0 ) {
        return false;
    }

    return true;
}

/*************************************************************************************************
                             E9_DEVIR_ISLEMI_BATCH::SEARCH_EDIT_CLICKED
*************************************************************************************************/

void E9_DEVIR_ISLEMI_BATCH::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    Q_UNUSED( line_edit );
    if ( widget EQ m_ui->searchEdit_arsiv_firma_kodu ) {
        QString veritabani_kodu, db_tanimi;
        int veritabani_id = VERITABANI_SEC( KULLANICI_ID(), &veritabani_kodu, &db_tanimi, this );

        Q_UNUSED( veritabani_id );
        m_ui->searchEdit_arsiv_firma_kodu->SET_TEXT( veritabani_kodu );
    }
}



