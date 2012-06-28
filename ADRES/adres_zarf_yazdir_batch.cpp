#include <QMessageBox>
#include <QPrintDialog>
#include <QSettings>
#include <QFontMetrics>
#include "adres_zarf_yazdir_batch_class.h"
#include "adres_zarf_yazdir_batch_open.h"
#include "ui_adres_zarf_yazdir_batch.h"
#include "print.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_ADRES_ZARF_YAZDIR_BATCH
***************************************************************************************/

void OPEN_ADRES_ZARF_YAZDIR_BATCH ( int record_id, QString printer_name , QWidget * parent )
{
    ADRES_ZARF_YAZDIR_BATCH * B = new ADRES_ZARF_YAZDIR_BATCH ( record_id,printer_name,parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   ADRES_ZARF_YAZDIR_BATCH::ADRES_ZARF_YAZDIR_BATCH_BATCH
***************************************************************************************/

ADRES_ZARF_YAZDIR_BATCH::ADRES_ZARF_YAZDIR_BATCH (int record_id, QString printer_name, QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::ADRES_ZARF_YAZDIR_BATCH)
{
    m_record_id         = record_id;
    m_printer_name      = printer_name;

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this,DB);
}

/**************************************************************************************
                   ADRES_ZARF_YAZDIR_BATCH::SETUP_FORM
***************************************************************************************/

void ADRES_ZARF_YAZDIR_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ( tr("zarf-yazdir") );
    SET_PAGE_TITLE      ( tr ( "ADR - ZARF YAZDIR" ) );
    SET_SETTING_NAME    ( "ADRES_ZARF_YAZDIR_BATCH" );
    REGISTER_BUTTON_WIDGET(m_ui->widget_batch_button);

    m_ui->comma_edit_sol_bosluk->SET_PRECISION           ( 0 );
    m_ui->comma_edit_sol_bosluk->VIRGULSUZ_CALIS         ( true );
    m_ui->comma_edit_sol_bosluk->SET_RETURN_EMPTY        ( false );

    m_ui->comma_edit_ust_bosluk->SET_PRECISION           ( 0 );
    m_ui->comma_edit_ust_bosluk->VIRGULSUZ_CALIS         ( true );
    m_ui->comma_edit_ust_bosluk->SET_RETURN_EMPTY        ( false );

    m_ui->comma_edit_etiket_genisligi->SET_PRECISION     ( 0 );
    m_ui->comma_edit_etiket_genisligi->VIRGULSUZ_CALIS   ( true );
    m_ui->comma_edit_etiket_genisligi->SET_RETURN_EMPTY  ( false );

    m_ui->comma_edit_etiket_yuksekligi->SET_PRECISION    ( 0 );
    m_ui->comma_edit_etiket_yuksekligi->VIRGULSUZ_CALIS  ( true );
    m_ui->comma_edit_etiket_yuksekligi->SET_RETURN_EMPTY ( false );

    m_ui->comma_edit_font_size->SET_PRECISION            ( 0 );
    m_ui->comma_edit_font_size->VIRGULSUZ_CALIS          ( true );
    m_ui->comma_edit_font_size->SET_RETURN_EMPTY         ( false );

    m_ui->comma_edit_sol_bosluk->SET_INTEGER        ( 140 );
    m_ui->comma_edit_ust_bosluk->SET_INTEGER        ( 60  );
    m_ui->comma_edit_etiket_genisligi->SET_INTEGER  ( 100 );
    m_ui->comma_edit_etiket_yuksekligi->SET_INTEGER ( 50  );
    m_ui->comma_edit_font_size->SET_INTEGER         ( 12  );

    SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Yazdır" ) );

    SET_FIRST_FOCUS_WIDGET(m_ui->comma_edit_etiket_genisligi);

    QSettings settings ("ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME( ADAK_DISPLAY_ID()));

    m_etiket_sol_bosluk = settings.value ( "zarf_sol_bosluk"         ,-1 ).toInt();
    m_etiket_ust_bosluk = settings.value ( "zarf_ust_bosluk"         ,-1 ).toInt();
    m_etiket_genislik   = settings.value ( "zarf_etiket_genislik"    ,-1 ).toInt();
    m_etiket_yukseklik  = settings.value ( "zarf_etiket_yukseklik"   ,-1 ).toInt();
    m_font_size         = settings.value ( "zarf_font_size"          ,-1 ).toInt();
    m_row_space         = 2;

    if ( m_etiket_sol_bosluk > 0 ) {
        m_ui->comma_edit_sol_bosluk->SET_INTEGER( m_etiket_sol_bosluk );
    }

    if ( m_etiket_ust_bosluk > 0 ) {
        m_ui->comma_edit_ust_bosluk->SET_INTEGER ( m_etiket_ust_bosluk );
    }

    if ( m_etiket_genislik > 0 ) {
        m_ui->comma_edit_etiket_genisligi->SET_INTEGER ( m_etiket_genislik );
    }

    if ( m_etiket_yukseklik > 0 ) {
        m_ui->comma_edit_etiket_yuksekligi->SET_INTEGER ( m_etiket_yukseklik );
    }

    if ( m_font_size > 0 ) {
        m_ui->comma_edit_font_size->SET_INTEGER ( m_font_size );
    }

    m_ui->check_box_dikey->setChecked ( settings.value ( "zarf_dikey_bastir" ).toBool() );
}

/**************************************************************************************
                   ADRES_ZARF_YAZDIR_BATCH::CHECK_VAR
***************************************************************************************/

int ADRES_ZARF_YAZDIR_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED ( object );
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_ZARF_YAZDIR_BATCH::CHECK_RUN
***************************************************************************************/

int ADRES_ZARF_YAZDIR_BATCH::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_ZARF_YAZDIR_BATCH::RUN_BATCH
***************************************************************************************/

void ADRES_ZARF_YAZDIR_BATCH::RUN_BATCH()
{
    GET_VARIABLES();

    m_etiket_sol_bosluk = m_ui->comma_edit_sol_bosluk->GET_INTEGER();
    m_etiket_ust_bosluk = m_ui->comma_edit_ust_bosluk->GET_INTEGER();
    m_etiket_genislik   = m_ui->comma_edit_etiket_genisligi->GET_INTEGER();
    m_etiket_yukseklik  = m_ui->comma_edit_etiket_yuksekligi->GET_INTEGER();
    m_font_size         = m_ui->comma_edit_font_size->GET_INTEGER();

    START_PRINT();

    QSettings settings ("ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()));

    settings.setValue ( "zarf_sol_bosluk"         ,m_ui->comma_edit_sol_bosluk->GET_INTEGER());
    settings.setValue ( "zarf_ust_bosluk"         ,m_ui->comma_edit_ust_bosluk->GET_INTEGER());
    settings.setValue ( "zarf_etiket_genislik"    ,m_ui->comma_edit_etiket_genisligi->GET_INTEGER());
    settings.setValue ( "zarf_etiket_yukseklik"   ,m_ui->comma_edit_etiket_yuksekligi->GET_INTEGER());
    settings.setValue ( "zarf_font_size"          ,m_ui->comma_edit_font_size->GET_INTEGER());
    settings.setValue ( "zarf_dikey_bastir"       ,m_ui->check_box_dikey->isChecked());
}

/**************************************************************************************
                   ADRES_ZARF_YAZDIR_BATCH::START_PRINT
***************************************************************************************/

void ADRES_ZARF_YAZDIR_BATCH::START_PRINT()
{
    m_printer.setPaperSize ( QPrinter::Comm10E );
    m_printer.setFullPage ( true );

    if ( m_ui->check_box_dikey->isChecked() EQ true ) {
         m_printer.setOrientation ( QPrinter::Portrait );
    }
    else {
        m_printer.setOrientation ( QPrinter::Landscape );
    }

    if ( m_printer_name.isEmpty() EQ false ) {
        m_printer.setPrinterName ( m_printer_name );
    }
    else {
        QPrintDialog printDialog(&m_printer);
        if ( printDialog.exec() NE QDialog::Accepted ) {
            return;
        }
    }

    m_painter.begin(&m_printer);
    CALCULATE_DEVICE_PIXEL();


    CREATE_PAGE();

    m_painter.end();

    QMessageBox::warning ( this,QObject::tr ( "Yazdırma İşlemi" ),QObject::tr ( "Belge Yazdırıldı.\t" ) );
}

/**************************************************************************************
                   ADRES_ZARF_YAZDIR_BATCH::GET_VARIABLES
***************************************************************************************/

void ADRES_ZARF_YAZDIR_BATCH::GET_VARIABLES()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "adr_adresler","adres_kart_turu,unvan,firma_sahis_adi,adres_satiri_1,adres_satiri_2,adres_satiri_3,semt,ilce,sehir,ulke,posta_kodu","adres_id=:adres_id");
    sql_query.SET_VALUE      ( ":adres_id",m_record_id );

    if (sql_query.SELECT() EQ 0) {
        return;
    }

    sql_query.NEXT();

    m_adres_string.clear();

    if ( sql_query.VALUE(1).toInt() EQ SAHIS_KAYDI ) {
        m_adres_string.append ( QString( sql_query.VALUE(1).toString() + "." ) );
    }

    m_adres_string.append ( QString ( sql_query.VALUE(2).toString() ) );


    if ( sql_query.VALUE(3).toString().isEmpty() EQ false ) {
        m_adres_string.append ( QString ( "\n" + sql_query.VALUE(3).toString() ) );
    }

    if ( sql_query.VALUE(4).toString().isEmpty() EQ false ) {
        m_adres_string.append(QString("\n" + sql_query.VALUE(4).toString() ) );
    }

    if (sql_query.VALUE(5).toString().isEmpty() EQ false) {
        m_adres_string.append(QString("\n" + sql_query.VALUE(5).toString()));
    }

    QString ilce_semt_ulke_bilgiler;

    ilce_semt_ulke_bilgiler.append ( sql_query.VALUE(10).toString() );

    if ( sql_query.VALUE(6).toString().isEmpty() EQ false ) {
        if ( ilce_semt_ulke_bilgiler.isEmpty() EQ false ) {
            ilce_semt_ulke_bilgiler.append ( ", " );
        }
        ilce_semt_ulke_bilgiler.append ( QString ( sql_query.VALUE(6).toString() ) );
    }

    if ( sql_query.VALUE(7).toString().isEmpty() EQ false ) {
        if ( ilce_semt_ulke_bilgiler.isEmpty() EQ false ) {
             ilce_semt_ulke_bilgiler.append ( ", " );
        }
        ilce_semt_ulke_bilgiler.append ( QString ( sql_query.VALUE(7).toString() + " " ) );
    }

    if (sql_query.VALUE(8).toString().isEmpty() EQ false ) {
        if ( ilce_semt_ulke_bilgiler.isEmpty() EQ  false ) {
            ilce_semt_ulke_bilgiler.append ( ", " );
        }
         ilce_semt_ulke_bilgiler.append ( QString(sql_query.VALUE(8).toString () ) );
    }

    if ( sql_query.VALUE(9).toString().isEmpty() EQ false ) {
         ilce_semt_ulke_bilgiler.append ( "\n" + QString(sql_query.VALUE(9).toString() ) );
    }

    if ( ilce_semt_ulke_bilgiler.isEmpty() EQ false ) {
         m_adres_string.append("\n" + ilce_semt_ulke_bilgiler );
    }
}

/**************************************************************************************
                   ADRES_ZARF_YAZDIR_BATCH::CREATE_PAGE
***************************************************************************************/

void ADRES_ZARF_YAZDIR_BATCH::CREATE_PAGE()
{
    double carpan = 0.0;
    QRectF  device_pixel = m_printer.paperRect();

    if (m_ui->check_box_dikey->isChecked() EQ true) {
        m_painter.translate ( device_pixel.width()/2.0,device_pixel.height()/2.0 );
        m_painter.rotate ( 90 );
        carpan = 1.0;
    }

    m_painter.setPen( Qt::black );
    m_painter.setFont ( QFont("Arial", m_font_size ) );

    QStringList lines = m_adres_string.split ( "\n" );

    double rect_width  = m_device_pixel_rate_x * QVariant( m_etiket_genislik).toDouble();
    double rect_height = m_device_pixel_rate_y * QVariant(m_etiket_yukseklik).toDouble();

    if (lines.size() > 0) {
        double mmx = m_device_pixel_rate_x * QVariant(m_etiket_sol_bosluk).toDouble() - ((device_pixel.height()/2.0) * carpan);
        m_last_add_line_y_pos = m_etiket_ust_bosluk;

        int space = (QVariant(m_font_size).toDouble() * 0.336 ) + m_row_space;

        for (int i = 0 ; i < lines.size();i++) {
            double mmy = m_device_pixel_rate_y * QVariant(m_last_add_line_y_pos).toDouble() - ((device_pixel.width()/2.0) * carpan);

            m_painter.drawText(QRectF(mmx,mmy,rect_width,rect_height),Qt::AlignLeft,lines.at(i));
            m_last_add_line_y_pos += space;

        }
    }
    else {
        double mmx = m_device_pixel_rate_x * m_etiket_sol_bosluk;
        double mmy = m_device_pixel_rate_y * m_etiket_ust_bosluk;
        m_painter.drawText(QRectF(mmx,mmy,m_etiket_genislik,m_etiket_yukseklik),Qt::AlignLeft,m_adres_string);
    }
}

/**************************************************************************************
                   ADRES_ZARF_YAZDIR_BATCH::CALCULATE_DEVICE_PIXEL
***************************************************************************************/

void ADRES_ZARF_YAZDIR_BATCH::CALCULATE_DEVICE_PIXEL()
{
    QRectF  milimiter    = m_printer.paperRect(QPrinter::Millimeter);
    QRectF  device_pixel = m_printer.paperRect();

    m_device_pixel_rate_x = device_pixel.width()  / milimiter.width();
    m_device_pixel_rate_y = device_pixel.height() / milimiter.height();
}
