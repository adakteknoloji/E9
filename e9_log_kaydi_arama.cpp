#include "e9_log_kaydi_arama_class.h"
#include "ui_e9_log_kaydi_arama.h"
#include "yonetim.h"
#include "e9_console_utils.h"
#include "e9_log.h"
#include "e9_gui_utils.h"

extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

#define     LOG_ID_COLUMN               0
#define     ISLEM_TARIHI_COLUMN         1
#define     ISLEM_ZAMANI_COLUMN         2
#define     KULLANICI_COLUMN            3
#define     BILGISAYAR_COLUMN           4
#define     LOG_TURU_COLUMN             5
#define     ISLEM_TURU_COLUMN           6


/**************************************************************************************
                              OPEN_E9_LOG_KAYDI_BATCH
***************************************************************************************/

void OPEN_E9_LOG_KAYDI_ARAMA ( QWidget * p_parent )
{
    E9_LOG_KAYDI_ARAMA *  F  = new E9_LOG_KAYDI_ARAMA ( p_parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                            E9_LOG_KAYDI_ARAMA::E9_LOG_KAYDI_ARAMA
***************************************************************************************/

E9_LOG_KAYDI_ARAMA::E9_LOG_KAYDI_ARAMA ( QWidget * p_parent ) : ARAMA_KERNEL ( p_parent ), m_ui ( new Ui::E9_LOG_KAYDI_ARAMA )
{
    m_ui->setupUi     ( this );
    START_ARAMA_KERNEL( this, DB );
}

/**************************************************************************************
                                E9_LOG_KAYDI_ARAMA::SETUP_FORM
***************************************************************************************/

void E9_LOG_KAYDI_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE  ( "yonetim-islemleri_e9-log-bilgileri" );

    QString page_title = tr("E9 Log Bilgisi Arama");

    m_ui->frame_kullanici_secimi->setEnabled ( false );
    m_ui->frame_log_turu->setEnabled         ( false );
    m_ui->textEdit_yapilan_islem->setVisible ( false );

    m_ui->adakDate_first_date->SET_DATE( MALI_YIL_FIRST_DATE() );

    m_ui->adakDate_second_date->SET_DATE( QDate::currentDate()     );

    E9_FILL_COMBOBOX_ADAK_MODULES   ( m_ui->comboBox_modul_secimi );

    SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_first_date );

#ifdef ADRES_BAGIMSIZ
    m_ui->adakDate_first_date->SET_DATE( QDate::currentDate().addDays(-30) );
    m_ui->comboBox_modul_secimi->setCurrentIndex ( m_ui->comboBox_modul_secimi->findText ( E9_GET_ADAK_MODULES_STRING ( ADRES_MODULU ) ));
    m_ui->comboBox_modul_secimi->setEnabled ( false );
    page_title = tr( "ADRES 007 LOG ARAMA" );

#endif

    FILL_LOG_TURLERI_COMBOBOX     ();

    m_kullanici_id = -1;

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );

    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_PAGE_TITLE ( page_title );

    SET_HEADERS ( QStringList() << tr ("log_id")       << tr ("İşlem Tarihi") << tr ("İşlem Zamanı")
                                << tr ("Kullanıcı")    << tr ("Bilgisayar")   << tr ("Log Türü")
                                << tr ("İşlem Türü"));

    SET_ENTER_KEY_FOR_RUN_BATCH   ( true );

    SET_SORTING                   ( false );

    SET_FIND_BUTTON_NAME          ( tr("Log Kaydı Ara") );

    m_ui->tableWidget->hideColumn     ( LOG_ID_COLUMN );

    m_ui->tableWidget->setColumnWidth ( ISLEM_TARIHI_COLUMN, 144 );
    m_ui->tableWidget->setColumnWidth ( ISLEM_ZAMANI_COLUMN, 98 );
    m_ui->tableWidget->setColumnWidth ( KULLANICI_COLUMN,    147 );
    m_ui->tableWidget->setColumnWidth ( BILGISAYAR_COLUMN,   149 );
    m_ui->tableWidget->setColumnWidth ( LOG_TURU_COLUMN,     155 );
    m_ui->tableWidget->setColumnWidth ( ISLEM_TURU_COLUMN,   142 );

    SET_SETTING_NAME                  ( "E9_LOG_KAYDI_ARAMA" );
}

/**************************************************************************************
                                E9_LOG_KAYDI_ARAMA::CHECK_VAR
***************************************************************************************/

int E9_LOG_KAYDI_ARAMA::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->checkBox_log_turu ) {
        if ( m_ui->checkBox_log_turu->isChecked() EQ true ) {
            m_ui->frame_log_turu->setEnabled(true);
        }
        else {
            m_ui->frame_log_turu->setEnabled(false);
            m_ui->comboBox_log_turu->setCurrentIndex(-1);
        }
    }
    else if ( p_object EQ m_ui->checkBox_kullanici_secimi ) {

        if ( m_ui->checkBox_kullanici_secimi->isChecked() EQ true ) {
            m_ui->frame_kullanici_secimi->setEnabled(true);
        }
        else {
            m_ui->frame_kullanici_secimi->setEnabled(false);
            m_ui->searchEdit_kullanici_kodu->SET_TEXT("");
            m_ui->lineEdit_kullanici_adi->clear();
            m_kullanici_id = -1;
        }
    }
    else if ( p_object EQ m_ui->searchEdit_kullanici_kodu ) {

        if ( m_ui->searchEdit_kullanici_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_kullanici_adi->clear();
            m_kullanici_id = -1;
            return ADAK_OK;
        }

        SQL_QUERY yonetim_query ( G_YONETIM_DB );

        yonetim_query.PREPARE_SELECT("ynt_kullanicilar ",
                                     "kullanici_id, kullanici_adi ",
                                     "kullanici_kodu = :kullanici_kodu AND silinmis_mi = 0 ");

        yonetim_query.SET_VALUE(":kullanici_kodu", m_ui->searchEdit_kullanici_kodu->GET_TEXT() );

        if ( yonetim_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr("Bu kodla kullanıcı bulunamadı."), NULL );
            return ADAK_FAIL_UNDO;
        }
        yonetim_query.NEXT();
        m_kullanici_id = yonetim_query.VALUE(0).toInt();
        m_ui->lineEdit_kullanici_adi->setText ( yonetim_query.VALUE(1).toString());
    }
    else if ( p_object EQ m_ui->comboBox_modul_secimi ) {
        FILL_LOG_TURLERI_COMBOBOX();
    }

    return ADAK_OK;
}

/**************************************************************************************
                               E9_LOG_KAYDI_ARAMA::CHECK_RUN
***************************************************************************************/

int E9_LOG_KAYDI_ARAMA::CHECK_RUN ()
{
    if ( m_ui->checkBox_kullanici_secimi->isChecked() EQ true ) {
        if ( m_ui->searchEdit_kullanici_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING(  tr("Kullanıcı seçimi yapmalısınız."), m_ui->searchEdit_kullanici_kodu );
            return ADAK_FAIL;
        }
        if ( m_kullanici_id < 1 ) {
            MSG_WARNING(  tr("Geçerli bir kullanıcı seçimi yapılmamış"), NULL );
            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_log_turu->isChecked() EQ true ) {
        if ( m_ui->comboBox_log_turu->currentIndex() EQ -1 ) {
            MSG_WARNING(  tr("Log türünü seçmelisiniz."), m_ui->comboBox_log_turu );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                             E9_LOG_KAYDI_ARAMA::SEARCH
***************************************************************************************/

void E9_LOG_KAYDI_ARAMA::SEARCH ()
{
    m_ui->textEdit_yapilan_islem->clear();

    int     selected_modul   = E9_GET_ADAK_MODULES_ENUM ( m_ui->comboBox_modul_secimi->currentText() );

    QString baslangic_tarihi = m_ui->adakDate_first_date->DATE();
    QString bitis_tarihi     = m_ui->adakDate_second_date->DATE();

    int     log_turu = -1;
    QString log_turu_str;

    QString kullanici_kodu_adi;
    QString bilgisayar_adi;

    if ( m_ui->checkBox_log_turu->isChecked() EQ true ) {
        log_turu     = E9_GET_LOG_TURLERI_ENUM ( m_ui->comboBox_log_turu->currentText() );
    }


    SQL_QUERY   sql_query     ( DB );
    SQL_QUERY   query_yonetim ( G_YONETIM_DB );

    QString sql_where_string = QString("islem_tarihi BETWEEN :bas_tarihi AND :bts_tarihi ");


    if ( m_ui->checkBox_log_turu->isChecked() EQ true  ) {
        sql_where_string += QString(" AND log_turu = %1 ").arg ( log_turu );
    }

    if ( m_ui->checkBox_kullanici_secimi->isChecked() EQ true ) {
        sql_where_string += QString("AND kullanici_id = %1").arg ( m_kullanici_id );
    }

    sql_where_string += QObject::tr(" AND modul_id = :modul_id ");

    sql_query.PREPARE_SELECT ( "e9_log_bilgileri",
                               "log_id, islem_tarihi, islem_zamani, kullanici_id, bilgisayar_id, log_turu, yapilan_islem ",
                               sql_where_string);

    sql_query.SET_VALUE ( ":bas_tarihi", baslangic_tarihi );
    sql_query.SET_VALUE ( ":bts_tarihi", bitis_tarihi     );
    sql_query.SET_VALUE ( ":modul_id",   selected_modul   );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    while ( sql_query.NEXT() EQ true ) {

        kullanici_kodu_adi = QObject::tr("Tanımlanmamış Kullanıcı");

        query_yonetim.PREPARE_SELECT("ynt_kullanicilar",
                                     "kullanici_kodu, kullanici_adi",
                                     "kullanici_id = :kullanici_id");
        query_yonetim.SET_VALUE(":kullanici_id",    sql_query.VALUE("kullanici_id").toInt());

        if ( query_yonetim.SELECT() > 0 ) {

            query_yonetim.NEXT();

            kullanici_kodu_adi = query_yonetim.VALUE(0).toString() + "    " + query_yonetim.VALUE(1).toString();
        }

        bilgisayar_adi = QObject::tr("Tanımlanmamış Bilgisayar");

        query_yonetim.PREPARE_SELECT("ynt_bilgisayarlar",
                                     "bilgisayar_adi",
                                     "bilgisayar_id = :bilgisayar_id" );
        query_yonetim.SET_VALUE(":bilgisayar_id", sql_query.VALUE("bilgisayar_id").toInt());

        if ( query_yonetim.SELECT() > 0 ) {
            query_yonetim.NEXT();
            bilgisayar_adi = query_yonetim.VALUE(0).toString();
        }
        log_turu_str  = E9_GET_LOG_TURLERI_STRING ( sql_query.VALUE("log_turu").toInt() );

        int current_row = 0;

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item ( current_row, LOG_ID_COLUMN       )->setText ( sql_query.VALUE(0).toString() );
        m_ui->tableWidget->item ( current_row, ISLEM_TARIHI_COLUMN )->setText ( QDate::fromString ( sql_query.VALUE(1).toString(), "yyyy.MM.dd" ).toString("dd MMMM yyyy") );
        m_ui->tableWidget->item ( current_row, ISLEM_ZAMANI_COLUMN )->setText ( sql_query.VALUE(2).toString() );
        m_ui->tableWidget->item ( current_row, KULLANICI_COLUMN    )->setText ( kullanici_kodu_adi );
        m_ui->tableWidget->item ( current_row, BILGISAYAR_COLUMN   )->setText ( bilgisayar_adi     );
        m_ui->tableWidget->item ( current_row, LOG_TURU_COLUMN     )->setText ( log_turu_str );
        m_ui->tableWidget->item ( current_row, ISLEM_TURU_COLUMN   )->setText ( E9_GET_LOG_ISLEMLERI_STRING ( sql_query.VALUE(6).toInt() ) );
    }

}

/**************************************************************************************
                         E9_LOG_KAYDI_ARAMA::SELECTED_LINE
***************************************************************************************/

void E9_LOG_KAYDI_ARAMA::SELECTED_LINE ( int p_selected_row_number )
{
    SQL_QUERY sql_query ( DB );

    m_ui->textEdit_yapilan_islem->clear();
    m_ui->textEdit_yapilan_islem->setVisible ( true );

    if ( p_selected_row_number EQ -1 ) {
        return;
    }

    int     log_id        = m_ui->tableWidget->item ( p_selected_row_number, LOG_ID_COLUMN )->text().toInt();

    sql_query.PREPARE_SELECT ("e9_log_bilgileri",
                              "log_detaylari",
                              "log_id = :log_id");
    sql_query.SET_VALUE(":log_id" , log_id );

    if ( sql_query.SELECT() EQ 0 ) {
        m_ui->textEdit_yapilan_islem->setVisible(false);
        return;
    }
    sql_query.NEXT();
    m_ui->textEdit_yapilan_islem->setText ( sql_query.VALUE(0).toString() );
}

/**************************************************************************************
                        E9_LOG_KAYDI_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int E9_LOG_KAYDI_ARAMA::SINGLE_LINE_SELECTED ( int p_selected_row_number )
{
    Q_UNUSED ( p_selected_row_number );
    return ADAK_CONTINUE;
}

/**************************************************************************************
                        E9_LOG_KAYDI_ARAMA::SEARCH_EDIT_CLICKED
***************************************************************************************/

void E9_LOG_KAYDI_ARAMA::SEARCH_EDIT_CLICKED ( QWidget * p_widget, QLineEdit * p_line_edit )
{
    Q_UNUSED ( p_widget );

    SQL_QUERY   yonetim_query ( G_YONETIM_DB );

    int kullanici_id = KULLANICI_SEC ( false, this );

    if ( kullanici_id > 0 ) {

        yonetim_query.PREPARE_SELECT ( "ynt_kullanicilar",
                                       "kullanici_kodu, kullanici_adi",
                                       "kullanici_id = :kullanici_id" );

        yonetim_query.SET_VALUE ( ":kullanici_id", kullanici_id );

        if ( yonetim_query.SELECT() EQ 0 ) {
            return;
        }
        yonetim_query.NEXT();
        p_line_edit->setText                  ( yonetim_query.VALUE(0).toString() );
        m_ui->lineEdit_kullanici_adi->setText ( yonetim_query.VALUE(1).toString() );
        m_kullanici_id = kullanici_id;
    }
}

/**************************************************************************************
                    E9_LOG_KAYDI_ARAMA::FILL_LOG_TURLERI_COMBOBOX
***************************************************************************************/

void E9_LOG_KAYDI_ARAMA::FILL_LOG_TURLERI_COMBOBOX ()
{
    m_ui->comboBox_log_turu->clear();

    int secilen_modul = E9_GET_ADAK_MODULES_ENUM ( m_ui->comboBox_modul_secimi->currentText() );

    switch ( secilen_modul ) {

        case MUHASEBE_MODULU :

            m_ui->comboBox_log_turu->addItems ( QStringList()<< QObject::tr ("Muhasebe Fişleri")
                                                << QObject::tr ("Muhasebe Raporlar")
                                                << QObject::tr ("Hesap Planı")
                                                << QObject::tr ("Hesap Grupları")
                                                << QObject::tr ("Entegrasyon Ana Hesapları")
                                                << QObject::tr ("Fiş Kopyalama")
                                                << QObject::tr ("Yevmiye No Oluştur") );
            break;
        case ADRES_MODULU :
            m_ui->comboBox_log_turu->addItems ( QStringList()<< QObject::tr ( "Şahıs Kaydı" )
                                                << QObject::tr ( "Firma Kaydı" )
                                                << QObject::tr ( "Adres Grup Ekleme" )
                                                << QObject::tr ( "Adres Grupları")
                                                << QObject::tr ( "Firma Yetkili Kaydı" )
                                                << QObject::tr ( "Yakın Bilgileri Kaydı" )
                                                << QObject::tr ( "Kullanıcı Yetkileri" )
                                                << QObject::tr ( "Etiket Yazdırma" )
                                                << QObject::tr ( "Mail Gönderimi"));
            break;

        case CARI_MODULU :
            m_ui->comboBox_log_turu->addItems(QStringList() <<  QObject::tr("Cari Fişler")
                                              << QObject::tr("Cari Hesaplar")
                                              << QObject::tr("Cari Hesap Grupları")
                                              << QObject::tr("Cari Raporlar"));

            break;
        case BANKA_MODULU :
            m_ui->comboBox_log_turu->addItems(QStringList() << QObject::tr("Banka Fişleri")
                                              << QObject::tr("Banka Hesapları")
                                              << QObject::tr("Banka İşlemleri")
                                              << QObject::tr("Pos İşlemleri")
                                              << QObject::tr("Kredİ Kartları")
                                              << QObject::tr("Banka Raporları"));
            break;

        case CEKSENET_MODULU :
            m_ui->comboBox_log_turu->addItems(QStringList() << QObject::tr("Çek-Senet Bordrosu")
                                              << QObject::tr("Kendi Çek/Senetlerimiz")
                                              << QObject::tr("Çek Raporları"));
            break;

        case ISLETME_MODULU :
            m_ui->comboBox_log_turu->addItems(QStringList () << QObject::tr("İşletme Fişleri")
                                              << QObject::tr("İşletme Fiş Kopyalama")
                                              << QObject::tr("Raporlar"));
            break;
        case DEMIRBAS_MODULU :
            m_ui->comboBox_log_turu->addItems(QStringList () << QObject::tr("Amortisman Ayırma")
                                              << QObject::tr("Demirbaş Modülü Fişleri")
                                              << QObject::tr("Demirbaşlar")
                                              << QObject::tr("Yeniden Değerleme")
                                              << QObject::tr("İşletme Raporları"));
            break;

        case FATURA_MODULU   :
            m_ui->comboBox_log_turu->addItems(QStringList () << QObject::tr("Faturalar")
                                              << QObject::tr("Hizmetler")
                                              << QObject::tr("Toplu Faturalaştırma"));
            break;

        case STOK_MODULU:
            m_ui->comboBox_log_turu->addItems(QStringList () << QObject::tr("Stok Fişleri")
                                              << QObject::tr("Ürünler")
                                              << QObject::tr("Ürün Birimleri"));
            break;

        case IRSALIYE_MODULU:
            m_ui->comboBox_log_turu->addItems(QStringList() << QObject::tr("İrsaliyeler"));

            break;
        case SUBE_MODULU:
            m_ui->comboBox_log_turu->addItems(QStringList() << QObject::tr("Şubeler")
                                              << QObject::tr("Depolar")
                                              << QObject::tr("Üniteler")
                                              << QObject::tr("Şube Entegrasyon Hesapları"));
            break;
        case PERSONEL_MODULU:
            m_ui->comboBox_log_turu->addItems(QStringList() << QObject::tr("Personeller")
                                              << QObject::tr("Personel Bordroları")
                                              << QObject::tr("Personel Sabit Değerler")
                                              << QObject::tr("İbraname Maddeleri")
                                              << QObject::tr("Personel Raporları"));
            break;

        case E9_YONETIM:
            m_ui->comboBox_log_turu->addItems(QStringList () << QObject::tr("E9 Onar")
                                              << QObject::tr("Devir İşlemi")
                                              << QObject::tr("Sabit Değerler")
                                              << QObject::tr("Kdv Ötv Tanımları")
                                              << QObject::tr("Yetkilendirme"));
            break;

        default :
            break;
    }
    m_ui->comboBox_log_turu->setCurrentIndex(-1);
}












