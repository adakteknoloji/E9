#include <QMessageBox>
#include "adak_sql.h"
#include "adak_std_utils.h"
#include "adres_console_utils.h"
#include "adres_yetkililer_fisi_class.h"
#include "adres_yetkililer_fisi_open.h"
 
#include "adres_kayit_formu_open.h"

#include "ui_adres_yetkililer_fisi.h"

extern ADAK_SQL *              DB;

#define YETKILI_ID_COLUMN          0
#define FIRMA_ID_COLUMN            1
#define ORDER_COLUMN               2
#define SAHIS_ID_COLUMN            3
#define YETKILI_ADI_SOYADI_COLUMN  4
#define YETKILI_GOREVI             5
#define YETKILI_ULKE_KODU          6
#define YETKILI_EV_TEL_ALAN_KODU   7
#define YETKILI_EV_TELEFONU        8
#define YETKILI_GSM_KODU           9
#define YETKILI_CEP_TELEFONU       10
#define YETKILI_IS_TEL_ALAN_KODU   11
#define YETKILI_IS_TELEFONU        12
#define YETKILI_IS_TELEFONU_DAHILI 13
#define DETAYLAR_BUTTON_COLUMN     14

/**************************************************************************************
                   OPEN_ADRES_YETKILILER_FISI
***************************************************************************************/

void OPEN_ADRES_YETKILILER_FISI ( int p_adres_id, QWidget * p_parent )
{
    ADRES_YETKILILER_FISI * F = new ADRES_YETKILILER_FISI ( p_adres_id, p_parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::ADRES_YETKILILER_FISI
***************************************************************************************/

ADRES_YETKILILER_FISI::ADRES_YETKILILER_FISI ( int p_adres_id, QWidget * p_parent ) :FIS_KERNEL ( p_parent ), m_ui ( new Ui::ADRES_YETKILILER_FISI )
{
    m_start_fis_kernel   = p_adres_id;

    m_ui->setupUi        ( this );
    START_FIS_KERNEL     ( this, DB );
 }

/**************************************************************************************
                     ADRES_YETKILILER_FISI::SETUP_FORM
***************************************************************************************/

void ADRES_YETKILILER_FISI::SETUP_FORM()
{
    SET_HELP_PAGE    ( tr("firma_yetkilileri_fisi.html") );
    SET_PAGE_TITLE   ( tr ( "FİRMA YETKİLİLERİ" ) );
    SET_SETTING_NAME ( "ADRES_YETKILILER_FISI" );

    SET_SINGLE_RECORD_ID(m_start_fis_kernel);

    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget );

    int num_of_columns   = 15;

    REGISTER_TABLE_WIDGET             ( m_ui->table_widget_yetkililer, num_of_columns );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION       ( FIRMA_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( YETKILI_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN       ( 4 );    

    SET_FIS_ORDER_DATABASE ( "adr_yetkili_bilgileri", "order_number", "yetkili_id" );

    SET_TABLE_ROW_WIDGETS ( SAHIS_ID_COLUMN,            WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_ADI_SOYADI_COLUMN,  WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_GOREVI,             WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_EV_TELEFONU,        WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_IS_TELEFONU,        WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_CEP_TELEFONU,       WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_ULKE_KODU,          WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_EV_TEL_ALAN_KODU,   WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_GSM_KODU,           WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_IS_TEL_ALAN_KODU,   WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( YETKILI_IS_TELEFONU_DAHILI, WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( DETAYLAR_BUTTON_COLUMN,     WIDGET_PUSH_BUTTON );

    SET_FIRST_FOCUS_WIDGET  ( m_ui->table_widget_yetkililer );

    SET_LINE_BUTTON_ICON                 ( QStringList()<<"" );

    SET_LINE_BUTTON_ICON ( QStringList()<<"" );

    m_ui->table_widget_yetkililer->setHorizontalHeaderLabels ( QStringList()<<"id"<<"fis_id"<<"Column Order"
                                                                                   <<tr ( "adres_id" )<<tr ( "Yetkili Adı Soyadı" )
                                                                                   <<tr ( "Görevi")<<tr ( "Ülke Kodu" )
                                                                                   <<tr ( "Alan Kodu")<<tr ( "Ev Telefonu" )
                                                                                   <<tr ( "Gsm Kodu" )<<tr("Cep Telefonu")
                                                                                   <<tr ( "Alan Kodu" )
                                                                                   <<tr ( "İş Telefonu" )
                                                                                   <<tr ( "İş Tel Dahili" )
                                                                                   <<tr ( "" ) <<tr ( "" ) <<tr ( "" ) );

    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_ADI_SOYADI_COLUMN      , 275 );
    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_GOREVI                 , 145 );
    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_ULKE_KODU              , 85  );
    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_EV_TEL_ALAN_KODU       , 78  );
    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_EV_TELEFONU            , 87  );
    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_GSM_KODU               , 78  );
    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_CEP_TELEFONU           , 95  );
    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_IS_TEL_ALAN_KODU       , 72  );
    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_IS_TELEFONU            , 91  );
    m_ui->table_widget_yetkililer->setColumnWidth ( YETKILI_IS_TELEFONU_DAHILI     , 87  );
    m_ui->table_widget_yetkililer->setColumnWidth ( DETAYLAR_BUTTON_COLUMN         , 90 );


    m_ui->table_widget_yetkililer->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->table_widget_yetkililer->setFocus         ( Qt::OtherFocusReason );

   FOCUS_FIRST_WIDGET ();

   //AUTOSTART_FIS_KERNEL    ( m_start_fis_kernel );
}

/**************************************************************************************
                     ADRES_YETKILILER_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void ADRES_YETKILILER_FISI::CLEAR_FORM_MEMBERS ()
{
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::NEW_FIS_RECORD
***************************************************************************************/

void ADRES_YETKILILER_FISI::NEW_FIS_RECORD()
{
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::GET_FIS_RECORD
***************************************************************************************/

int ADRES_YETKILILER_FISI::GET_FIS_RECORD ( int p_adres_id )
{
    SQL_QUERY   sql_query     ( DB );

    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi","adres_id=:adres_id");
    sql_query.SET_VALUE      ( ":adres", p_adres_id );

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();

        m_ui->label_adres_kaydi->setText(sql_query.VALUE(0).toString());
    }

    sql_query.PREPARE_SELECT ( "adr_yetkili_bilgileri, adr_adresler","yetkili_id, order_number, gorevi, "
                               "firma_sahis_adi, tel_ulke_kodu, tel_alan_kodu, telefon,"
                               "is_tel_alan_kodu, is_telefonu, cep_tel_gsm_kodu, cep_telefonu, "
                               "adr_yetkili_bilgileri.adres_id, is_telefonu_dahili",
                               "adr_yetkili_bilgileri.adres_id = adr_adresler.adres_id "
                               "AND adr_yetkili_bilgileri.firma_id = :firma_id ");

    sql_query.SET_VALUE      ( ":firma_id", p_adres_id );

    if ( sql_query.SELECT("order_number") EQ 0 ) {
        return ADAK_OK;
    }

    int current_row = 0;

    while ( sql_query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE ();

        QTableWidgetItem * yetkili_id_item = new QTableWidgetItem ( sql_query.VALUE(0).toString() );
        m_ui->table_widget_yetkililer->setItem ( current_row, YETKILI_ID_COLUMN, yetkili_id_item );

        QTableWidgetItem * firma_id_item = new QTableWidgetItem ( QVariant ( p_adres_id ).toString() );
        m_ui->table_widget_yetkililer->setItem ( current_row, FIRMA_ID_COLUMN, firma_id_item );

        QTableWidgetItem * yetki_order_item = new QTableWidgetItem ( sql_query.VALUE(1).toString() );
        m_ui->table_widget_yetkililer->setItem ( current_row, ORDER_COLUMN, yetki_order_item );

        QLineEdit  * lineEdit_sahis_id            = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, SAHIS_ID_COLUMN            );
        QLineEdit  * lineEdit_yetkili_adi_soyadi  = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_ADI_SOYADI_COLUMN  );
        QLineEdit  * lineEdit_yetkili_gorevi      = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_GOREVI             );
        QLineEdit  * lineEdit_yetkili_is_telefonu = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_IS_TELEFONU        );
        QLineEdit  * lineEdit_yetkili_gsm         = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_CEP_TELEFONU       );
        QLineEdit  * lineEdit_yetkili_ev_telefonu = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_EV_TELEFONU        );
        QComboBox  * comboBox_ulke_kodu           = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_ULKE_KODU          );
        QComboBox  * comboBox_ev_tel_alan_kodu    = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_EV_TEL_ALAN_KODU   );
        QComboBox  * comboBox_is_tel_alan_kodu    = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_IS_TEL_ALAN_KODU   );
        QComboBox  * comboBox_gsm_kodu            = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_GSM_KODU           );
        QCommaEdit * commaEdit_is_tel_dahili      = ( QCommaEdit * ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_IS_TELEFONU_DAHILI );

        lineEdit_yetkili_gorevi->setText      ( sql_query.VALUE(2).toString() );
        lineEdit_yetkili_adi_soyadi->setText  ( sql_query.VALUE(3).toString() );

        FIND_AND_SET_COMBOBOX_TEXT ( comboBox_ulke_kodu,        sql_query.VALUE(4).toString() );

        SATIR_TELEFON_ALAN_KODLARINI_DOLDUR   ( current_row );

        FIND_AND_SET_COMBOBOX_TEXT ( comboBox_ev_tel_alan_kodu, sql_query.VALUE(5).toString() );
        lineEdit_yetkili_ev_telefonu->setText ( sql_query.VALUE(6).toString() );

        FIND_AND_SET_COMBOBOX_TEXT ( comboBox_is_tel_alan_kodu, sql_query.VALUE(7).toString() );
        lineEdit_yetkili_is_telefonu->setText ( sql_query.VALUE(8).toString() );

        FIND_AND_SET_COMBOBOX_TEXT ( comboBox_gsm_kodu, sql_query.VALUE(9).toString() );
        lineEdit_yetkili_gsm->setText ( sql_query.VALUE(10).toString() );

        lineEdit_sahis_id->setText ( sql_query.VALUE(11).toString() );

        commaEdit_is_tel_dahili->SET_INTEGER ( sql_query.VALUE(12).toInt() );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void ADRES_YETKILILER_FISI::SET_LINE_DEFAULTS ( int p_row_number )
{
    QComboBox   * comboBox_ulke_kodu            = ( QComboBox   * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_ULKE_KODU          );
    QComboBox   * comboBox_ev_alan              = ( QComboBox   * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_EV_TEL_ALAN_KODU   );
    QComboBox   * comboBox_gsm_kodu             = ( QComboBox   * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_GSM_KODU           );
    QComboBox   * comboBox_is_tel_kodu          = ( QComboBox   * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_IS_TEL_ALAN_KODU   );
    QLineEdit   * lineEdit_yetkili_ev_telefonu  = ( QLineEdit   * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_EV_TELEFONU        );
    QLineEdit   * lineEdit_yetkili_is_telefonu  = ( QLineEdit   * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_IS_TELEFONU        );
    QLineEdit   * lineEdit_yetkili_cep_telefonu = ( QLineEdit   * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_CEP_TELEFONU       );
    QLineEdit   * line_edit_ad_soyad            = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget  ( p_row_number, YETKILI_ADI_SOYADI_COLUMN  );
    QLineEdit   * line_edit_gorevi              = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget  ( p_row_number, YETKILI_GOREVI             );

    QPushButton * button_detaylar               = ( QPushButton * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, DETAYLAR_BUTTON_COLUMN     );
    QCommaEdit  * commaEdit_is_tel_dahili       = ( QCommaEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_IS_TELEFONU_DAHILI );

    SET_COMBOBOX_LENGTH(comboBox_ulke_kodu     , 10 );
    SET_COMBOBOX_LENGTH(comboBox_ev_alan       , 10 );
    SET_COMBOBOX_LENGTH(comboBox_gsm_kodu      , 10 );
    SET_COMBOBOX_LENGTH(comboBox_is_tel_kodu   , 10 );

    lineEdit_yetkili_ev_telefonu->setMaxLength   ( 10 );
    lineEdit_yetkili_is_telefonu->setMaxLength   ( 10 );
    lineEdit_yetkili_cep_telefonu->setMaxLength  ( 10 );
    line_edit_gorevi->setMaxLength               ( 30 );
    line_edit_ad_soyad->setMaxLength             ( 100 );
    commaEdit_is_tel_dahili->setMaxLength        ( 5 );

    GET_ULKE_TELEFON_KODLARI( comboBox_ulke_kodu );

    QString turkiye_ulke_kodu = GET_ULKE_TELEFON_KODU( "Türkiye" );

    FIND_AND_SET_COMBOBOX_TEXT ( comboBox_ulke_kodu, turkiye_ulke_kodu );

    SATIR_TELEFON_ALAN_KODLARINI_DOLDUR ( p_row_number );

    lineEdit_yetkili_ev_telefonu->setInputMask  ( "999-99-99" );
    lineEdit_yetkili_is_telefonu->setInputMask  ( "999-99-99" );
    lineEdit_yetkili_cep_telefonu->setInputMask ( "999-99-99" );

    button_detaylar->setText       ( tr ( "Detaylar" ) );
    button_detaylar->setToolTip    ( tr ( "Yetkili detayları için tıklayınız." ) );

    commaEdit_is_tel_dahili->VIRGULSUZ_CALIS ();
    commaEdit_is_tel_dahili->SET_PRECISION   ( 0 );
    commaEdit_is_tel_dahili->SET_RETURN_EMPTY(true);

    REGISTER_LINE_SAVER_BUTTON     ( p_row_number, DETAYLAR_BUTTON_COLUMN );
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_FIS_FORM_VAR ( QObject * p_object )
{

    Q_UNUSED(p_object);
    return ADAK_OK;
}


/**************************************************************************************
                   ADRES_YETKILILER_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}


/**************************************************************************************
                   ADRES_YETKILILER_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_FIS_RECORD_ADD()
{
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::ADD_FIS_RECORD
***************************************************************************************/

int ADRES_YETKILILER_FISI::ADD_FIS_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_FIS_RECORD_UPDATE ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void ADRES_YETKILILER_FISI::UPDATE_FIS_RECORD ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
}

/**************************************************************************************
                    ADRES_YETKILILER_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_FIS_RECORD_DELETE ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
    return ADAK_OK;
}


/**************************************************************************************
                    ADRES_YETKILILER_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void ADRES_YETKILILER_FISI::DELETE_FIS_RECORD ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int ADRES_YETKILILER_FISI::SELECT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::FIND_FIS_RECORD
***************************************************************************************/

int ADRES_YETKILILER_FISI::FIND_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int ADRES_YETKILILER_FISI::FIND_FIRST_FIS_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int ADRES_YETKILILER_FISI::FIND_LAST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int ADRES_YETKILILER_FISI::FIND_PREV_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int ADRES_YETKILILER_FISI::FIND_NEXT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::CHECK_LINE_VAR
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QComboBox   * comboBox_ulke_kodu        = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget   ( row_number, YETKILI_ULKE_KODU          );
    QComboBox   * comboBox_ev_tel_alan_kodu = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget   ( row_number, YETKILI_EV_TEL_ALAN_KODU   );
    QComboBox   * comboBox_is_tel_alan_kodu = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget   ( row_number, YETKILI_IS_TEL_ALAN_KODU   );
    QComboBox   * comboBox_gsm_kodu         = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget   ( row_number, YETKILI_GSM_KODU           );
    QCommaEdit  * commaEdit_is_tel_dahili   = ( QCommaEdit * ) m_ui->table_widget_yetkililer->cellWidget   ( row_number, YETKILI_IS_TELEFONU_DAHILI );


    if ( object EQ comboBox_ulke_kodu ) {
        if ( comboBox_ulke_kodu->currentText().size() > 10 ) {
            QMessageBox::warning ( this, tr ( "HATA" ), tr ( "Ülke kodu 10 karakterden daha uzun olamaz." ) );
            SET_FOCUS( comboBox_ulke_kodu );
            return ADAK_FAIL;
        }
        SATIR_TELEFON_ALAN_KODLARINI_DOLDUR ( row_number );
    }
    else if ( object EQ comboBox_ev_tel_alan_kodu OR object EQ comboBox_gsm_kodu OR object EQ comboBox_is_tel_alan_kodu ) {
        QComboBox * comboBox = ( QComboBox * ) object;
        if ( comboBox->currentText().size() > 10 ) {
           QMessageBox::warning ( this, tr ( "HATA" ), tr ( "Alan kodu 10 karakterden daha uzun olamaz." ) );
           SET_FOCUS( comboBox_ev_tel_alan_kodu );
           return ADAK_FAIL;
        }
    }
    else if ( object EQ commaEdit_is_tel_dahili ) {
        if ( commaEdit_is_tel_dahili->GET_TEXT().size() > 5 ) {
            QMessageBox::warning ( this, tr("HATA"), tr ( "Dahili no 5 karakterden daha uzun olamaz" ) );
            SET_FOCUS( commaEdit_is_tel_dahili );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QLineEdit  * line_edit_ad_soyad      = ( QLineEdit * ) m_ui->table_widget_yetkililer->cellWidget  ( row_number, YETKILI_ADI_SOYADI_COLUMN  );
    QLineEdit  * line_edit_gorevi        = ( QLineEdit * ) m_ui->table_widget_yetkililer->cellWidget  ( row_number, YETKILI_GOREVI             );

    if ( line_edit_ad_soyad->text().isEmpty() EQ true ) {
        QMessageBox::warning( this, tr ( "HATA" ), tr ( "Firma yetkili adı soyadı boş bırakılamaz" ) );
        SET_FOCUS( line_edit_ad_soyad );
        return ADAK_FAIL;
    }
    else if ( line_edit_gorevi->text().isEmpty() EQ true ) {
        QMessageBox::warning ( this, tr ( "HATA" ), tr ( "Firma yetkili görevi boş bırakılamaz" ) );
        SET_FOCUS( line_edit_gorevi );
        return ADAK_FAIL;
    }


    return ADAK_OK;
}


/**************************************************************************************
                   ADRES_YETKILILER_FISI::CHECK_ADD_LINE
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id  );
    Q_UNUSED ( row_number );

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::ADD_LINE
***************************************************************************************/

void ADRES_YETKILILER_FISI::ADD_LINE ( int p_record_id, int p_row_number )
{
    SQL_QUERY sql_query ( DB );

    QLineEdit  * lineEdit_sahis_id             = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, SAHIS_ID_COLUMN            );
    QLineEdit  * lineEdit_yetkili_adi_soyadi   = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_ADI_SOYADI_COLUMN  );
    QLineEdit  * lineEdi_yetkili_gorevi        = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_GOREVI             );
    QLineEdit  * lineEdit_yetkili_cep_tel      = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_CEP_TELEFONU       );
    QLineEdit  * lineEdit_yetkili_is_tel       = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_IS_TELEFONU        );
    QLineEdit  * lineEdit_yetkili_ev_tel       = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_EV_TELEFONU        );
    QComboBox  * comboBox_yetkili_ulke_kodu    = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_ULKE_KODU          );
    QComboBox  * comboBox_yetkili_ev_alan_kodu = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_EV_TEL_ALAN_KODU   );
    QComboBox  * comboBox_yetkili_is_alan_kodu = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_IS_TEL_ALAN_KODU   );
    QComboBox  * comboBox_yetkili_gsm_kodu     = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_GSM_KODU           );
    QCommaEdit * commaEdit_is_tel_dahili       = ( QCommaEdit * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_IS_TELEFONU_DAHILI );

    int     order_number    = m_ui->table_widget_yetkililer->item ( p_row_number , ORDER_COLUMN )->text().toInt();

    QString yetkili_cep_telefonu = lineEdit_yetkili_cep_tel->text();
    QString yetkili_is_telefonu  = lineEdit_yetkili_is_tel->text();
    QString yetkili_ev_telefonu  = lineEdit_yetkili_ev_tel->text();

    yetkili_cep_telefonu.replace ( "-", "" );
    yetkili_is_telefonu.replace  ( "-", "" );
    yetkili_ev_telefonu.replace  ( "-", "" );

    QString     yetkili_ulke_kodu   = comboBox_yetkili_ulke_kodu->currentText();
    QString     telefon_alan_kodu   = comboBox_yetkili_ev_alan_kodu->currentText();
    QString     is_tel_alan_kodu    = comboBox_yetkili_is_alan_kodu->currentText();
    QString     cep_tel_gsm_kodu    = comboBox_yetkili_gsm_kodu->currentText();
    QString     is_telefonu_dahili  = commaEdit_is_tel_dahili->GET_TEXT();

    sql_query.PREPARE_INSERT ( "adr_adresler" ,"adres_id",
                               "firma_sahis_adi, firma_id, tel_ulke_kodu, tel_alan_kodu, "
                               "telefon, is_tel_ulke_kodu, is_tel_alan_kodu, is_telefonu, is_telefonu_dahili, "
                               "cep_tel_ulke_kodu, cep_tel_gsm_kodu, cep_telefonu, modul_id,program_id, "
                               "adres_kart_turu");

    sql_query.SET_VALUE ( ":firma_sahis_adi"    ,    lineEdit_yetkili_adi_soyadi->text() );
    sql_query.SET_VALUE ( ":firma_id"           ,    p_record_id                         );
    sql_query.SET_VALUE ( ":tel_ulke_kodu"      ,    yetkili_ulke_kodu                   );
    sql_query.SET_VALUE ( ":tel_alan_kodu"      ,    telefon_alan_kodu                   );
    sql_query.SET_VALUE ( ":telefon"            ,    yetkili_ev_telefonu                 );
    sql_query.SET_VALUE ( ":is_tel_ulke_kodu"   ,    yetkili_ulke_kodu                   );
    sql_query.SET_VALUE ( ":is_tel_alan_kodu"   ,    is_tel_alan_kodu                    );
    sql_query.SET_VALUE ( ":is_telefonu"        ,    yetkili_is_telefonu                 );
    sql_query.SET_VALUE ( ":is_telefonu_dahili" ,    is_telefonu_dahili                  );
    sql_query.SET_VALUE ( ":cep_tel_ulke_kodu"  ,    yetkili_ulke_kodu                   );
    sql_query.SET_VALUE ( ":cep_tel_gsm_kodu"   ,    cep_tel_gsm_kodu                    );
    sql_query.SET_VALUE ( ":cep_telefonu"       ,    yetkili_cep_telefonu                );
    sql_query.SET_VALUE ( ":modul_id"           ,    ADRES_MODULU                        );
    sql_query.SET_VALUE ( ":program_id"         ,    E9_PROGRAMI                         );
    sql_query.SET_VALUE ( ":adres_kart_turu"  ,    SAHIS_KAYDI                         );

    int adres_id = sql_query.INSERT();

    int tel_gizli_kayit_id     = ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR ( yetkili_ulke_kodu, telefon_alan_kodu, yetkili_ev_telefonu, adres_id                     );
    int cep_tel_gizli_kayit_id = ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR ( yetkili_ulke_kodu, cep_tel_gsm_kodu, yetkili_cep_telefonu, adres_id                     );
    int is_tel_gizli_kayit_id  = ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR ( yetkili_ulke_kodu, is_tel_alan_kodu, yetkili_cep_telefonu, adres_id, is_telefonu_dahili );

    sql_query.PREPARE_UPDATE ( "adr_adresler ","adres_id","telefon_gizli_kayit_id, "
                               "cep_tel_gizli_kayit_id, is_tel_gizli_kayit_id",
                               "adres_id = :adres_id" );

    sql_query.SET_VALUE ( ":telefon_gizli_kayit_id",      tel_gizli_kayit_id     );
    sql_query.SET_VALUE ( ":cep_tel_gizli_kayit_id",      cep_tel_gizli_kayit_id );
    sql_query.SET_VALUE ( ":is_tel_gizli_kayit_id",       is_tel_gizli_kayit_id  );
    sql_query.SET_VALUE ( ":adres_id",                    adres_id               );

    sql_query.UPDATE();

    sql_query.PREPARE_INSERT ( "adr_yetkili_bilgileri","yetkili_id","order_number, "
                               "firma_id, adres_id, gorevi" );

    sql_query.SET_VALUE ( ":order_number" , order_number              );
    sql_query.SET_VALUE ( ":firma_id"          , p_record_id                    );
    sql_query.SET_VALUE ( ":adres_id"          , adres_id                       );
    sql_query.SET_VALUE ( ":gorevi"            , lineEdi_yetkili_gorevi->text() );

    int yetkili_id = sql_query.INSERT();

    QTableWidgetItem * yetkili_id_item = new QTableWidgetItem ( QVariant ( yetkili_id ).toString() );
    QTableWidgetItem * firma_id_item   = new QTableWidgetItem ( QVariant ( p_record_id  ).toString() );

    m_ui->table_widget_yetkililer->setItem ( p_row_number, YETKILI_ID_COLUMN, yetkili_id_item );
    m_ui->table_widget_yetkililer->setItem ( p_row_number, FIRMA_ID_COLUMN, firma_id_item     );

    lineEdit_sahis_id->setText ( QVariant (adres_id ).toString() );


//    int firma_id = 0;
//    QString log_detaylari;

//    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi, firma_id","adres_id = :adres_id" );
//    sql_query.SET_VALUE ( ":adres_id", adres_id );

//    if ( sql_query.SELECT() > 0 ) {
//        sql_query.NEXT();
//        log_detaylari.append ( " # Yetkili Adı Soyadı: " );
//        log_detaylari.append ( sql_query.VALUE(0).toString() );
//        firma_id = sql_query.VALUE(1).toInt();
//    }

//    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi","adres_id = :adres_id" );
//    sql_query.SET_VALUE      ( ":adres_id", firma_id );

//    if ( sql_query.SELECT() > 0 ) {
//        sql_query.NEXT();
//        log_detaylari.append ( " # Firma Adı: " );
//        log_detaylari.append ( sql_query.VALUE(0).toString() );
//    }

//    log_detaylari.append ( " # Yetkili Görevi: " );
//    log_detaylari.append ( lineEdi_yetkili_gorevi->text()      );

//    E9_LOG_KAYDI_EKLE(ADRES_MODULU,LOG_ADR_FIRMA_YETKILI_KAYDI,LOG_ISLEM_ADD,log_detaylari);

}

/**************************************************************************************
                    ADRES_YETKILILER_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id  );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                    ADRES_YETKILILER_FISI::UPDATE_LINE
***************************************************************************************/

void ADRES_YETKILILER_FISI::UPDATE_LINE ( int p_record_id, int p_row_number )
{
    SQL_QUERY sql_query ( DB );

    QLineEdit  * lineEdit_sahis_id             = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, SAHIS_ID_COLUMN           );
    QLineEdit  * lineEdit_yetkili_adi_soyadi   = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_ADI_SOYADI_COLUMN );
    QLineEdit  * lineEdi_yetkili_gorevi        = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_GOREVI            );
    QLineEdit  * lineEdit_yetkili_cep_tel      = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_CEP_TELEFONU      );
    QLineEdit  * lineEdit_yetkili_is_tel       = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_IS_TELEFONU       );
    QLineEdit  * lineEdit_yetkili_ev_tel       = ( QLineEdit  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_EV_TELEFONU       );
    QComboBox  * comboBox_yetkili_ulke_kodu    = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_ULKE_KODU         );
    QComboBox  * comboBox_yetkili_ev_alan_kodu = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_EV_TEL_ALAN_KODU  );
    QComboBox  * comboBox_yetkili_is_alan_kodu = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_IS_TEL_ALAN_KODU  );
    QComboBox  * comboBox_yetkili_gsm_kodu     = ( QComboBox  * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_GSM_KODU          );
    QCommaEdit * commaEdit_is_tel_dahili       = ( QCommaEdit * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, YETKILI_IS_TELEFONU_DAHILI );

    int     yetkili_id           = m_ui->table_widget_yetkililer->item ( p_row_number,YETKILI_ID_COLUMN)->text().toInt();

    QString yetkili_cep_telefonu = lineEdit_yetkili_cep_tel->text();
    QString yetkili_is_telefonu  = lineEdit_yetkili_is_tel->text();
    QString yetkili_ev_telefonu  = lineEdit_yetkili_ev_tel->text();

    yetkili_cep_telefonu.replace (  "-", ""  );
    yetkili_is_telefonu.replace  (  "-", ""  );
    yetkili_ev_telefonu.replace  (  "-", ""  );

    QString yetkili_ulke_kodu        = comboBox_yetkili_ulke_kodu->currentText();
    QString yetkili_ev_tel_alan_kodu = comboBox_yetkili_ev_alan_kodu->currentText();

    QString is_tel_alan_kodu         = comboBox_yetkili_is_alan_kodu->currentText();
    QString cep_tel_gsm_kodu         = comboBox_yetkili_gsm_kodu->currentText();

    QString is_tel_dahili            = commaEdit_is_tel_dahili->GET_TEXT();

    sql_query.PREPARE_SELECT ( "adr_adresler","telefon_gizli_kayit_id, cep_tel_gizli_kayit_id, "
                               "fax_gizli_kayit_id, is_tel_gizli_kayit_id, email_gizli_kayit_id ",
                                "adres_id = :adres_id" );

    sql_query.SET_VALUE ( ":adres_id", lineEdit_sahis_id->text().toInt() );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();

    int     tel_gizli_kayit_id      =   sql_query.VALUE(0).toInt();
    int     cep_tel_gizli_kayit_id  =   sql_query.VALUE(1).toInt();
    int     is_tel_gizli_kayit_id   =   sql_query.VALUE(3).toInt();

    sql_query.PREPARE_UPDATE ( "adr_adresler","adres_id","firma_sahis_adi, "
                               "tel_ulke_kodu, "
                               "tel_alan_kodu, "
                               "telefon      , "
                               "is_tel_ulke_kodu   , "
                               "is_tel_alan_kodu   , "
                               "is_telefonu        , "
                               "is_telefonu_dahili , "
                               "cep_tel_ulke_kodu  , "
                               "cep_tel_gsm_kodu   , "
                               "cep_telefonu       ","adres_id      = :adres_id");

    sql_query.SET_VALUE ( ":firma_sahis_adi"    ,  lineEdit_yetkili_adi_soyadi->text()  );
    sql_query.SET_VALUE ( ":tel_ulke_kodu"      ,  yetkili_ulke_kodu                    );
    sql_query.SET_VALUE ( ":tel_alan_kodu"      ,  yetkili_ev_tel_alan_kodu             );
    sql_query.SET_VALUE ( ":telefon"            ,  yetkili_ev_telefonu                  );
    sql_query.SET_VALUE ( ":is_tel_ulke_kodu"   ,  yetkili_ulke_kodu                    );
    sql_query.SET_VALUE ( ":is_tel_alan_kodu"   ,  is_tel_alan_kodu                     );
    sql_query.SET_VALUE ( ":is_telefonu"        ,  yetkili_is_telefonu                  );
    sql_query.SET_VALUE ( ":is_telefonu_dahili" ,  is_tel_dahili                        );
    sql_query.SET_VALUE ( ":cep_tel_ulke_kodu"  ,  yetkili_ulke_kodu                    );
    sql_query.SET_VALUE ( ":cep_tel_gsm_kodu"   ,  cep_tel_gsm_kodu                     );
    sql_query.SET_VALUE ( ":cep_telefonu"       ,  yetkili_cep_telefonu                 );
    sql_query.SET_VALUE ( ":adres_id"           ,  lineEdit_sahis_id->text().toInt()    );

    sql_query.UPDATE();

    ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE ( yetkili_ulke_kodu, yetkili_ev_tel_alan_kodu, yetkili_ev_telefonu, tel_gizli_kayit_id                         );
    ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE ( yetkili_ulke_kodu, cep_tel_gsm_kodu, yetkili_cep_telefonu, cep_tel_gizli_kayit_id             );
    ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE ( yetkili_ulke_kodu, is_tel_alan_kodu, yetkili_is_telefonu, is_tel_gizli_kayit_id, is_tel_dahili );

    sql_query.PREPARE_UPDATE ( "adr_yetkili_bilgileri","yetkili_id","firma_id,gorevi","yetkili_id = :yetkili_id" );

    sql_query.SET_VALUE ( ":firma_id"   , p_record_id    );
    sql_query.SET_VALUE ( ":gorevi"     , lineEdi_yetkili_gorevi->text() );
    sql_query.SET_VALUE ( ":yetkili_id" , yetkili_id     );

    sql_query.UPDATE();

//    QString log_detaylari;
//    int firma_id = 0;

//    log_detaylari      = "Log Türü: FİRMA YETKİLİ KAYDI ";

//    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi, firma_id","adres_id = :adres_id" );
//    sql_query.SET_VALUE ( ":adres_id", lineEdit_sahis_id->text().toInt() );

//    if ( sql_query.SELECT() > 0 ) {
//        sql_query.NEXT();
//        log_detaylari.append ( " # Yetkili Adı Soyadı: " );
//        log_detaylari.append ( sql_query.VALUE(0).toString() );
//        firma_id = sql_query.VALUE(1).toInt();
//    }
//    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi","adres_id = :adres_id" );
//    sql_query.SET_VALUE      ( ":adres_id", firma_id );

//    if ( sql_query.SELECT() > 0 ) {
//        sql_query.NEXT();
//        log_detaylari.append ( " # Firma Adı: " );
//        log_detaylari.append ( sql_query.VALUE(0).toString() );
//    }

//    log_detaylari.append ( " # Yetkili Görevi: " );
//    log_detaylari.append ( lineEdi_yetkili_gorevi->text()        );

//    E9_LOG_KAYDI_EKLE(ADRES_MODULU,LOG_ADR_FIRMA_YETKILI_KAYDI,LOG_ISLEM_UPDATE,log_detaylari);

}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int ADRES_YETKILILER_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{

    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );

    int clicked_button = MSG_YES_NO( tr ( "Bu kaydı silmek yetkilinin şahıslar tablosundan da silinmesine sebep olacaktır. "
                                                                              "Emin misiniz ? " ), NULL );

    if ( clicked_button EQ ADAK_NO ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::DELETE_LINE
***************************************************************************************/

void ADRES_YETKILILER_FISI::DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );

    SQL_QUERY sql_query ( DB );

    int yetkili_id = m_ui->table_widget_yetkililer->item ( row_number,YETKILI_ID_COLUMN )->text().toInt();

    sql_query.PREPARE_SELECT ( "adr_yetkili_bilgileri","adres_id, gorevi","yetkili_id = :yetkili_id" );

    sql_query.SET_VALUE      ( ":yetkili_id", yetkili_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    int     adres_id           = sql_query.VALUE(0).toInt();
//    QString yetkili_gorevi     = sql_query.VALUE(1).toString();

//    QString log_detaylari;

//    int firma_id = 0;

//    log_detaylari      = "Log Türü: FİRMA YETKİLİ KAYDI ";

//    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi, firma_id","adres_id = :adres_id" );
//    sql_query.SET_VALUE ( ":adres_id", adres_id );

//    if ( sql_query.SELECT() > 0 ) {
//        sql_query.NEXT();
//        log_detaylari.append ( " # Yetkili Adı Soyadı: " );
//        log_detaylari.append ( sql_query.VALUE(0).toString() );
//        firma_id = sql_query.VALUE(1).toInt();
//    }

//    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi","adres_id = :adres_id" );
//    sql_query.SET_VALUE      ( ":adres_id", firma_id );

//    if ( sql_query.SELECT() > 0 ) {
//        sql_query.NEXT();
//        log_detaylari.append ( " # Firma Adı: " );
//        log_detaylari.append ( sql_query.VALUE(0).toString() );
//    }

//    log_detaylari.append ( " # Yetkili Görevi: " );
//    log_detaylari.append ( yetkili_gorevi        );


//    E9_LOG_KAYDI_EKLE(ADRES_MODULU,LOG_ADR_FIRMA_YETKILI_KAYDI,LOG_ISLEM_DELETE,log_detaylari);

    sql_query.PREPARE_DELETE ( "adr_telefonlar","adres_kayit_id = :adres_kayit_id");
    sql_query.SET_VALUE      ( ":adres_kayit_id", adres_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE ( "adr_adresler","adres_id = :adres_id" );
    sql_query.SET_VALUE      ( ":adres_id", adres_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE ( "adr_yetkili_bilgileri","yetkili_id=:yetkili_id" );
    sql_query.SET_VALUE      ( ":yetkili_id",yetkili_id  );
    sql_query.DELETE();


}

/**************************************************************************************
                    ADRES_YETKILILER_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int ADRES_YETKILILER_FISI::LOCK_FIS_RECORD ( int record_id )
{
    return DB->LOCK_ROW ( "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( record_id ) );
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void ADRES_YETKILILER_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
   DB->UNLOCK_ROW( "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( record_id ) );
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::LINE_SAVER_BUTTON_CLICKED
***************************************************************************************/

void ADRES_YETKILILER_FISI::LINE_SAVER_BUTTON_CLICKED ( QAbstractButton *p_button, int p_row_id, int p_record_id, int p_row_number )
{
    Q_UNUSED ( p_button     );
    Q_UNUSED ( p_record_id  );
    Q_UNUSED ( p_row_id     );

    int id = (( QLineEdit * ) m_ui->table_widget_yetkililer->cellWidget ( p_row_number, SAHIS_ID_COLUMN ))->text().toInt();

    OPEN_ADRES_KAYIT_FORMU( id ,SAHIS_KAYDI, this );
}

/**************************************************************************************
                   ADRES_YETKILILER_FISI::SATIR_TELEFON_ALAN_KODLARINI_DOLDUR
***************************************************************************************/

void ADRES_YETKILILER_FISI::SATIR_TELEFON_ALAN_KODLARINI_DOLDUR ( int current_row )
{
    QComboBox * comboBox_ulke_kodu        = ( QComboBox *  ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_ULKE_KODU );
    QComboBox * comboBox_ev_tel_alan_kodu = ( QComboBox *  ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_EV_TEL_ALAN_KODU );
    QComboBox * comboBox_gsm_kodu         = ( QComboBox *  ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_GSM_KODU );
    QComboBox * comboBox_is_tel_alan_kodu = ( QComboBox *  ) m_ui->table_widget_yetkililer->cellWidget ( current_row, YETKILI_IS_TEL_ALAN_KODU );

    comboBox_gsm_kodu->clear();
    comboBox_ev_tel_alan_kodu->clear();
    comboBox_is_tel_alan_kodu->clear();

    if ( comboBox_ulke_kodu->currentText().isEmpty() EQ true ) {
        return;
    }

    GET_GSM_KODLARI  ( comboBox_gsm_kodu        , comboBox_ulke_kodu->currentText() );
    GET_ALAN_KODLARI ( comboBox_ev_tel_alan_kodu, comboBox_ulke_kodu->currentText() );
    GET_ALAN_KODLARI ( comboBox_is_tel_alan_kodu, comboBox_ulke_kodu->currentText() );
}
