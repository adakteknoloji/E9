#include "adak_sql.h"
#include "prs_ek_odenek_ve_kesinti_fisi_class.h"
#include "prs_ek_odenek_ve_kesinti_fisi_open.h"
#include "prs_enum.h"
#include "e9_gui_utils.h"
#include "adak_std_utils.h"
#include "cari_console_utils.h"
#include "cari_kart_arama_open.h"
#include "prs_ek_odenek_kesinti_arama_open.h"
#include "cari_struct.h"
#include "prs_gui_utils.h"
#include "prs_con_utils.h"
#include "e9_console_utils.h"

extern ADAK_SQL *           DB;

#define ROW_ID_COLUMN                  0  // Satirin idsini tutacak column
#define RECORD_ID_COLUMN               1
#define ORDER_NUMBER_COLUMN            2  // Satirlari siralamak amacli kullanilacak column
#define PERSONEL_ID                    3
#define PERSONEL_KODU                  4
#define PERSONEL_AD_SOYAD              5


/**************************************************************************************
                   OPEN_PRS_EK_ODENEK_VE_KESINTI_FISI
***************************************************************************************/

void OPEN_PRS_EK_ODENEK_VE_KESINTI_FISI ( int p_fis_turu , QWidget * parent )
{
    PRS_EK_ODENEK_VE_KESINTI_FISI * F = new PRS_EK_ODENEK_VE_KESINTI_FISI ( p_fis_turu, parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::PRS_EK_ODENEK_VE_KESINTI_FISI
***************************************************************************************/

PRS_EK_ODENEK_VE_KESINTI_FISI::PRS_EK_ODENEK_VE_KESINTI_FISI( int p_fis_turu, QWidget * parent) :FIS_KERNEL(parent), m_ui ( new Ui::PRS_EK_ODENEK_VE_KESINTI_FISI )
{
    m_fis_turu            = p_fis_turu;
    m_bordrosu_kesildi_mi = 0;


    m_ui->setupUi    ( this );        
    START_FIS_KERNEL ( this, DB );

 }
 
 /**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::SETUP_FORM()
{

    SET_SETTING_NAME  ("EK_ODENEK_VE_KESINTI_FISI");
    SET_HELP_PAGE     ("personel-bordrosu_ek-odenek-tanimlama");

    if ( m_fis_turu EQ ENUM_PRS_ODENEK ) {
        SET_PAGE_TITLE    ( tr ( "PRS - EK ÖDENEK TANIMLAMA" ) );
    }
    else {
        SET_PAGE_TITLE    ( tr ( "PRS - EK KESİNTİ TANIMLAMA" ) );
    }

    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget );

    SET_FIRST_FOCUS_WIDGET( m_ui->lineEdit_odenek_kesinti_adi );    

    REGISTER_TABLE_WIDGET ( m_ui->table_widget, 6 );

    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_NUMBER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION ( RECORD_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION  ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN ( 4 );

    // Bu fonksiyon ise kernelin otomatik olarak satir order numlarin reorganize edilmesi saglar.
    // Burda kernela database bilgilerini setleyerek bu isi yapabiliriz.

    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_yuzde_mi );

    QStringList kesinti_yuzde_mi;
    kesinti_yuzde_mi << tr("TUTAR")<< tr("YÜZDE");

    m_ui->comboBox_yuzde_mi->addItems( kesinti_yuzde_mi );

    SET_FIS_ORDER_DATABASE ("prs_ek_kesintiler_odenekler", "order_number", "kesinti_odenek_id" );

    SET_TABLE_ROW_WIDGETS ( PERSONEL_KODU      , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( PERSONEL_AD_SOYAD  , WIDGET_LINE_EDIT );


    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_bordro_donemi_ay );
    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_bordro_donemi_yil );

    E9_FILL_COMBOBOX_AYLAR( m_ui->comboBox_bordro_donemi_ay );

    PRS_FILL_COMBO_BOX_BORDRO_YILLARI( m_ui->comboBox_bordro_donemi_yil );

    int current_month = QDate::currentDate().month();
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_bordro_donemi_ay, E9_GET_AYLAR_STRING( current_month) );

    m_ui->lineEdit_odenek_kesinti_adi->setMaxLength( 50 );

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH( 512 );

    m_ui->table_widget->setHorizontalHeaderLabels (QStringList() << "row_id" << "fis_id" << "order_number_column" << "personel_id"
                                                   << tr("Personel Kodu") << tr("Personel Adı Soyadı") << tr("") << tr("") );

    m_ui->table_widget->setColumnWidth( PERSONEL_KODU      , 100 );
    m_ui->table_widget->setColumnWidth( PERSONEL_AD_SOYAD  , 550 );

    FOCUS_FIRST_WIDGET ();

    if ( m_fis_turu EQ ENUM_PRS_KESINTI ) {

        m_ui->checkBox_ssk_kesintisi->setVisible( false );
        m_ui->checkBox_gelir_vergisi_kesintisi->setVisible( false );
        m_ui->checkBox_damga_vergisi_kesintisi->setVisible( false );
    }
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::NEW_FIS_RECORD
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::NEW_FIS_RECORD()
{
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::GET_FIS_RECORD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::GET_FIS_RECORD ( int record_id )
{

    SQL_QUERY   sql_query(DB);
    SQL_QUERY   query    (DB);

    sql_query.PREPARE_SELECT( "prs_kesinti_odenek_tanimlar ",
                              "kes_ode_tanim_id, kesinti_odenek_adi, aciklama, "
                              "bordro_donemi_ay, bordro_donemi_yil, "
                              "ssk_kesintisi_iceriyor_mu, damga_vergisi_iceriyor_mu,"
                              "gelir_vergisi_iceriyor_mu, kesinti_odenek_tarihi,"
                              "yuzde_mi, tutar, kesinti_odenek_no, bordrosu_kesildi_mi ",
                              "kes_ode_tanim_id = :kes_ode_tanim_id   "
                              "AND kesinti_odenek_turu = :kesinti_odenek_turu ");

    sql_query.SET_VALUE( ":kes_ode_tanim_id", record_id );
    sql_query.SET_VALUE( ":kesinti_odenek_turu", m_fis_turu );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_odenek_kesinti_no     = sql_query.VALUE("kesinti_odenek_no").toInt();
    m_bordrosu_kesildi_mi   = sql_query.VALUE("bordrosu_kesildi_mi").toInt();

    m_ui->lineEdit_odenek_kesinti_adi->setText( sql_query.VALUE("kesinti_odenek_adi").toString() );

    m_ui->adakDate_odenek_kesinti_tarihi->SET_DATE( sql_query.VALUE("kesinti_odenek_tarihi").toString() );
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_bordro_donemi_ay, E9_GET_AYLAR_STRING( sql_query.VALUE("bordro_donemi_ay").toInt()));
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_bordro_donemi_yil, sql_query.VALUE("bordro_donemi_yil").toString() );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_yuzde_mi, tr("TUTAR"));
    if ( sql_query.VALUE( "yuzde_mi").toInt() EQ PRS_YUZDE ) {
        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_yuzde_mi, tr("YÜZDE"));
    }

    m_ui->commaEdit_tutar->SET_DOUBLE( sql_query.VALUE("tutar").toDouble());

    m_ui->checkBox_ssk_kesintisi->setChecked( sql_query.VALUE("ssk_kesintisi_iceriyor_mu").toInt() );
    m_ui->checkBox_gelir_vergisi_kesintisi->setChecked( sql_query.VALUE("gelir_vergisi_iceriyor_mu").toInt() );
    m_ui->checkBox_damga_vergisi_kesintisi->setChecked( sql_query.VALUE("damga_vergisi_iceriyor_mu").toInt() );


    query.PREPARE_SELECT ( "prs_ek_kesintiler_odenekler, prs_personeller",
                           "kesinti_odenek_id, prs_ek_kesintiler_odenekler.personel_id, cari_hesap_id, order_number ",
                           "prs_ek_kesintiler_odenekler.personel_id = prs_personeller.personel_id "
                           "AND kes_ode_tanim_id = :kes_ode_tanim_id ");

    query.SET_VALUE      ( ":kes_ode_tanim_id" , record_id );

    query.SELECT("order_number ASC" );

    while ( query.NEXT() EQ true ) {

        int p_current_row;

        p_current_row = ADD_NEW_LINE ();

        QSearchEdit * search_edit_persosnel_kodu     = ( QSearchEdit  * ) m_ui->table_widget->cellWidget ( p_current_row, PERSONEL_KODU );
        QLineEdit   * lineedit_personel_adi_soyadi   = ( QLineEdit    * ) m_ui->table_widget->cellWidget ( p_current_row, PERSONEL_AD_SOYAD );

        search_edit_persosnel_kodu->SET_TEXT( CARI_FIND_HESAP_KODU( query.VALUE("cari_hesap_id").toInt()));
        lineedit_personel_adi_soyadi->setText( CARI_FIND_HESAP_ISMI( query.VALUE("cari_hesap_id").toInt() ));

        QString id                                  = query.VALUE("kesinti_odenek_id").toString();
        QString order_number                        = query.VALUE("order_number").toString();

        QTableWidgetItem * new_item;

        new_item = new QTableWidgetItem(tr("%1").arg(id));
        m_ui->table_widget->setItem(p_current_row, ROW_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(record_id));
        m_ui->table_widget->setItem(p_current_row, RECORD_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(order_number));
        m_ui->table_widget->setItem(p_current_row, ORDER_NUMBER_COLUMN, new_item);

    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QSearchEdit * search_edit_persosnel_kodu     = ( QSearchEdit  * ) m_ui->table_widget->cellWidget ( row_number, PERSONEL_KODU );
    QLineEdit   * lineedit_personel_adi_soyadi   = ( QLineEdit    * ) m_ui->table_widget->cellWidget ( row_number, PERSONEL_AD_SOYAD );

    lineedit_personel_adi_soyadi->setReadOnly( true );
    lineedit_personel_adi_soyadi->setMaxLength( 60 );

    search_edit_persosnel_kodu->SET_MAX_LENGTH( 15 );

    SET_LAST_FOCUSED_WIDGET(lineedit_personel_adi_soyadi);

}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    Q_UNUSED( object );

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_FORM_EMPTY()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prs_bordrolar", "bordro_id", "bordro_donemi_ay = :bordro_donemi_ay AND bordro_donemi_yil =:bordro_donemi_yil ");
    query.SET_VALUE     (":bordro_donemi_ay", E9_GET_AYLAR_ENUM( m_ui->comboBox_bordro_donemi_ay->currentText()) );
    query.SET_VALUE     (":bordro_donemi_yil", m_ui->comboBox_bordro_donemi_yil->currentText().toInt() );

    if ( query.SELECT() NE 0 ) {
        MSG_WARNING("Bu Ödenek / Kesinde Ödenmiş değiklik yapamazsınız", NULL );
        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_odenek_kesinti_adi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr( "Lütfen Ödenek / Kesinti Adı giriniz." ), m_ui->lineEdit_odenek_kesinti_adi );
        return ADAK_FAIL;
    }
    if( m_ui->commaEdit_tutar->GET_DOUBLE() < 0.00 ) {
        MSG_WARNING( tr("Lütfen Yüzde / Tutar giriniz."), m_ui->commaEdit_tutar );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_RECORD_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::ADD_FIS_RECORD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::ADD_FIS_RECORD ()
{

    int yuzde_mi = PRS_TUTAR;
    if ( m_ui->comboBox_yuzde_mi->currentText() EQ "YÜZDE") {
        yuzde_mi = PRS_YUZDE;
    }
    double tutar  = m_ui->commaEdit_tutar->GET_DOUBLE();

    SQL_QUERY query( DB );

    query.PREPARE_INSERT( "prs_kesinti_odenek_tanimlar","kes_ode_tanim_id",
                          "kesinti_odenek_no, kesinti_odenek_tarihi,"
                          "kesinti_odenek_adi, aciklama, bordro_donemi_ay, bordro_donemi_yil,"
                          "ssk_kesintisi_iceriyor_mu, damga_vergisi_iceriyor_mu, "
                          "gelir_vergisi_iceriyor_mu, yuzde_mi, tutar, "
                          "kesinti_odenek_turu, bordrosu_kesildi_mi ");

    query.SET_VALUE(":kesinti_odenek_no"            , PRS_SIRADAKI_ODENEK_KESINTI_NO_AL( m_ui->adakDate_odenek_kesinti_tarihi->DATE() ) );
    query.SET_VALUE(":kesinti_odenek_tarihi"        , m_ui->adakDate_odenek_kesinti_tarihi->DATE() );
    query.SET_VALUE(":kesinti_odenek_adi"           , m_ui->lineEdit_odenek_kesinti_adi->text()     );
    query.SET_VALUE(":aciklama"                     , m_ui->limitedTextEdit_aciklama->toPlainText()     );
    query.SET_VALUE(":bordro_donemi_ay"             , E9_GET_AYLAR_ENUM( m_ui->comboBox_bordro_donemi_ay->currentText() ));
    query.SET_VALUE(":bordro_donemi_yil"            , m_ui->comboBox_bordro_donemi_yil->currentText().toInt());
    query.SET_VALUE(":ssk_kesintisi_iceriyor_mu"    , QVariant (m_ui->checkBox_ssk_kesintisi->isChecked()).toInt() );
    query.SET_VALUE(":damga_vergisi_iceriyor_mu"    , QVariant (m_ui->checkBox_damga_vergisi_kesintisi->isChecked()).toInt() );
    query.SET_VALUE(":gelir_vergisi_iceriyor_mu"    , QVariant (m_ui->checkBox_gelir_vergisi_kesintisi->isChecked()).toInt() );
    query.SET_VALUE(":yuzde_mi"                     , yuzde_mi      );
    query.SET_VALUE(":tutar"                        , tutar         );
    query.SET_VALUE(":kesinti_odenek_turu"          , m_fis_turu    );
    query.SET_VALUE(":bordrosu_kesildi_mi"          , 0             );

    int record_id = query.INSERT();

    return record_id;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::UPDATE_FIS_RECORD ( int record_id )
{

    int yuzde_mi = PRS_TUTAR;
    if ( m_ui->comboBox_yuzde_mi->currentText() EQ "YÜZDE") {
        yuzde_mi = PRS_YUZDE;
    }
    double tutar  = m_ui->commaEdit_tutar->GET_DOUBLE();

    SQL_QUERY query( DB );

    query.PREPARE_UPDATE( "prs_kesinti_odenek_tanimlar","kes_ode_tanim_id",
                          "kesinti_odenek_tarihi, kesinti_odenek_adi, aciklama, bordro_donemi_ay, bordro_donemi_yil,"
                          "ssk_kesintisi_iceriyor_mu, damga_vergisi_iceriyor_mu, gelir_vergisi_iceriyor_mu,"
                          "yuzde_mi, tutar",
                          "kes_ode_tanim_id = :kes_ode_tanim_id");

    query.SET_VALUE(":kesinti_odenek_tarihi"        , m_ui->adakDate_odenek_kesinti_tarihi->DATE()                             );
    query.SET_VALUE(":kesinti_odenek_adi"           , m_ui->lineEdit_odenek_kesinti_adi->text()                                 );
    query.SET_VALUE(":aciklama"                     , m_ui->limitedTextEdit_aciklama->toPlainText()                             );
    query.SET_VALUE(":bordro_donemi_ay"             , E9_GET_AYLAR_ENUM( m_ui->comboBox_bordro_donemi_ay->currentText() )       );
    query.SET_VALUE(":bordro_donemi_yil"            , m_ui->comboBox_bordro_donemi_yil->currentText().toInt()                   );
    query.SET_VALUE(":ssk_kesintisi_iceriyor_mu"    , QVariant (m_ui->checkBox_ssk_kesintisi->isChecked()).toInt()              );
    query.SET_VALUE(":damga_vergisi_iceriyor_mu"    , QVariant (m_ui->checkBox_damga_vergisi_kesintisi->isChecked()).toInt()    );
    query.SET_VALUE(":gelir_vergisi_iceriyor_mu"    , QVariant (m_ui->checkBox_gelir_vergisi_kesintisi->isChecked()).toInt()    );
    query.SET_VALUE(":yuzde_mi"                     , yuzde_mi                                                                  );
    query.SET_VALUE(":tutar"                        , tutar                                                                     );
    query.SET_VALUE(":kes_ode_tanim_id"            , record_id                                                                 );

    query.UPDATE();;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    Q_UNUSED ( record_id );

    if ( m_bordrosu_kesildi_mi EQ 1 ) {
        MSG_WARNING( "Ödenek / Kesintinin Bordrosu alınmış Değişiklik yapamazssınız.", NULL );
        return ADAK_FAIL_UNDO;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::DELETE_FIS_RECORD ( int record_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_DELETE( "prs_ek_kesintiler_odenekler","kes_ode_tanim_id = :kes_ode_tanim_id" );
    query.SET_VALUE     ( ":kes_ode_tanim_id", record_id );
    query.DELETE();

    query.PREPARE_DELETE( "prs_kesinti_odenek_tanimlar","kes_ode_tanim_id = :kes_ode_tanim_id" );
    query.SET_VALUE     ( ":kes_ode_tanim_id", record_id );
    query.DELETE();

}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::SELECT_FIS_RECORD()
{
    return OPEN_PRS_EK_ODENEK_KESINTI_ARAMA( m_fis_turu, this );
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_FIS_RECORD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "prs_kesinti_odenek_tanimlar","kes_ode_tanim_id","kesinti_odenek_no = :kesinti_odenek_no "
                               "AND kesinti_odenek_turu = :kesinti_odenek_turu AND kesinti_odenek_tarihi = :kesinti_odenek_tarihi" );

    sql_query.SET_VALUE      ( ":kesinti_odenek_no"     , m_odenek_kesinti_no );
    sql_query.SET_VALUE      ( ":kesinti_odenek_turu"   , m_fis_turu );
    sql_query.SET_VALUE      ( ":kesinti_odenek_tarihi" , m_ui->adakDate_odenek_kesinti_tarihi->DATE() );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr ( "Ödenek / Kesiti bulunamadı" ), NULL );

        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT( "prs_kesinti_odenek_tanimlar","kes_ode_tanim_id");

    if ( sql_query.SELECT("kesinti_odenek_no ASC",0,1) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT( "prs_kesinti_odenek_tanimlar","kes_ode_tanim_id");

    if ( sql_query.SELECT("kesinti_odenek_no DESC",0,1) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
             PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_kesinti_odenek_tanimlar","kes_ode_tanim_id ",
                              " kesinti_odenek_no < :kesinti_odenek_no ");

    sql_query.SET_VALUE( ":kesinti_odenek_no"        , m_odenek_kesinti_no );

    if ( sql_query.SELECT("kesinti_odenek_no DESC",0,1 ) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }
    return -1;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_kesinti_odenek_tanimlar","kes_ode_tanim_id ",
                              " kesinti_odenek_no > :kesinti_odenek_no ");

    sql_query.SET_VALUE( ":kesinti_odenek_no"        , m_odenek_kesinti_no );

    if ( sql_query.SELECT("kesinti_odenek_no ASC",0,1 ) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }
    return -1;
}



/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_LINE_VAR
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QSearchEdit * searchedit_personel_kodu      = ( QSearchEdit * ) m_ui->table_widget->cellWidget(row_number, PERSONEL_KODU );
    QLineEdit *   line_edit_personel_adi_soyad  = ( QLineEdit   * ) m_ui->table_widget->cellWidget(row_number, PERSONEL_AD_SOYAD );

    if ( m_bordrosu_kesildi_mi EQ 1 ) {
        MSG_WARNING( "Ödenek / Kesintinin Bordrosu alınmış Değişiklik yapamazssınız.", NULL );
        return ADAK_FAIL_UNDO;
    }
    if ( object EQ searchedit_personel_kodu ) {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT ( "prs_personeller","personel_id, cari_hesap_id ",
                               "sicil_no = :sicil_no" );
        query.SET_VALUE      ( ":sicil_no" , searchedit_personel_kodu->GET_TEXT() );

        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING( tr("Aradığınız Personel kodu bulunamadı!.."), searchedit_personel_kodu  );
            return ADAK_FAIL_UNDO;
        }

        query.NEXT();

        int personel_id = query.VALUE(0).toInt();
        line_edit_personel_adi_soyad->setText(  CARI_FIND_HESAP_ISMI(query.VALUE(1).toInt()));

        m_ui->table_widget->item( row_number, PERSONEL_ID )->setText(QString::number(personel_id));
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_LINE_EMPTY ( int row_number )
{    
    Q_UNUSED ( row_number );

    if ( m_bordrosu_kesildi_mi EQ 1 ) {
        MSG_WARNING( "Ödenek / Kesintinin Bordrosu alınmış Değişiklik yapamazssınız.", NULL );
        return ADAK_FAIL_UNDO;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_ADD_LINE
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{

    SQL_QUERY sql_query( DB );

    sql_query.PREPARE_SELECT("prs_ek_kesintiler_odenekler", "personel_id ",
                             "kes_ode_tanim_id = :kes_ode_tanim_id AND personel_id = :personel_id");

    sql_query.SET_VALUE(":personel_id" , m_ui->table_widget->item( row_number, PERSONEL_ID )->text().toInt() );
    sql_query.SET_VALUE(":kes_ode_tanim_id" ,record_id );

    if ( sql_query.SELECT() NE 0 ) {
        MSG_WARNING("Personel Zaten tanimli!!!", NULL );
        return ADAK_FAIL_UNDO;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::ADD_LINE
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::ADD_LINE ( int record_id, int row_number )
{

    int ssk_kesintisi_var_mi = QVariant (m_ui->checkBox_ssk_kesintisi->isChecked()).toInt();
    int gelir_vergisi_var_mi = QVariant (m_ui->checkBox_gelir_vergisi_kesintisi->isChecked()).toInt();
    int damga_vergisi_var_mi = QVariant (m_ui->checkBox_damga_vergisi_kesintisi->isChecked()).toInt();

    int yuzde_mi_tutar_mi = PRS_TUTAR;
    if ( m_ui->comboBox_yuzde_mi->currentText() EQ "YÜZDE") {
        yuzde_mi_tutar_mi = PRS_YUZDE;
    }
    double tutar          = m_ui->commaEdit_tutar->GET_DOUBLE();
    QString odenek_kesinti_adi = m_ui->lineEdit_odenek_kesinti_adi->text();

    int bordro_donemi_ay    = E9_GET_AYLAR_ENUM( m_ui->comboBox_bordro_donemi_ay->currentText() );
    int bordro_donemi_yil   = m_ui->comboBox_bordro_donemi_yil->currentText().toInt();


    SQL_QUERY sql_query( DB );

    sql_query.PREPARE_INSERT("prs_ek_kesintiler_odenekler","kesinti_odenek_id",
                             "personel_id,  "
                             "odenek_kesinti_adi, bordro_donemi_ay, bordro_donemi_yil, "
                             "ssk_kesintisi_iceriyor_mu, gelir_vergisi_iceriyor_mu, "
                             "damga_vergisi_iceriyor_mu, yuzde_mi, yuzde_orani, tutar, "
                             "aciklama, bordrosu_kesildi_mi , kesinti_odenek_turu ,"
                             "bordro_id, kes_ode_tanim_id, order_number ");


    sql_query.SET_VALUE(":personel_id"              , m_ui->table_widget->item( row_number, PERSONEL_ID )->text().toInt() );
    sql_query.SET_VALUE(":odenek_kesinti_adi"       , odenek_kesinti_adi                            );
    sql_query.SET_VALUE(":bordro_donemi_ay"         , bordro_donemi_ay                              );
    sql_query.SET_VALUE(":bordro_donemi_yil"        , bordro_donemi_yil                             );
    sql_query.SET_VALUE(":ssk_kesintisi_iceriyor_mu", ssk_kesintisi_var_mi                          );
    sql_query.SET_VALUE(":gelir_vergisi_iceriyor_mu", gelir_vergisi_var_mi                          );
    sql_query.SET_VALUE(":damga_vergisi_iceriyor_mu", damga_vergisi_var_mi                          );
    sql_query.SET_VALUE(":yuzde_mi"                 , yuzde_mi_tutar_mi                             );
    sql_query.SET_VALUE(":tutar"                    , tutar                                         );
    sql_query.SET_VALUE(":aciklama"                 , m_ui->limitedTextEdit_aciklama->toPlainText() );
    sql_query.SET_VALUE(":bordrosu_kesildi_mi"      , 0                                             );
    sql_query.SET_VALUE(":kesinti_odenek_turu"      , m_fis_turu                                    ); // odenek or kesinti
    sql_query.SET_VALUE(":bordro_id"                , 0                                             );
    sql_query.SET_VALUE(":kes_ode_tanim_id"         , record_id                                     );
    sql_query.SET_VALUE(":order_number"             , m_ui->table_widget->item( row_number, ORDER_NUMBER_COLUMN )->text().toInt() );
    if ( yuzde_mi_tutar_mi EQ PRS_YUZDE ) {
        sql_query.SET_VALUE(":yuzde_orani"          , tutar );
        sql_query.SET_VALUE(":tutar"                , 0     );
    }
    else {
        sql_query.SET_VALUE(":yuzde_orani"          , 0     );
        sql_query.SET_VALUE(":tutar"                , tutar );
    }

    int fis_satiri_id = sql_query.INSERT();

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( fis_satiri_id ).toString() );
    m_ui->table_widget->setItem ( row_number, ROW_ID_COLUMN, new_item );

    new_item = new QTableWidgetItem ( QVariant ( record_id ).toString() );
    m_ui->table_widget->setItem ( row_number, RECORD_ID_COLUMN, new_item );

}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::UPDATE_LINE
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::UPDATE_LINE ( int record_id, int row_number )
{

   int kesinti_odenek_id    = m_ui->table_widget->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    int ssk_kesintisi_var_mi = QVariant (m_ui->checkBox_ssk_kesintisi->isChecked()).toInt();
    int gelir_vergisi_var_mi = QVariant (m_ui->checkBox_gelir_vergisi_kesintisi->isChecked()).toInt();
    int damga_vergisi_var_mi = QVariant (m_ui->checkBox_damga_vergisi_kesintisi->isChecked()).toInt();

    int yuzde_mi_tutar_mi = PRS_TUTAR;
    if ( m_ui->comboBox_yuzde_mi->currentText() EQ "YÜZDE") {
        yuzde_mi_tutar_mi = PRS_YUZDE;
    }
    double tutar          = m_ui->commaEdit_tutar->GET_DOUBLE();
    QString kesinti_odenek_adi = m_ui->lineEdit_odenek_kesinti_adi->text();

    int bordro_donemi_ay    = E9_GET_AYLAR_ENUM( m_ui->comboBox_bordro_donemi_ay->currentText() );
    int bordro_donemi_yil   = m_ui->comboBox_bordro_donemi_yil->currentText().toInt();

    SQL_QUERY sql_query( DB );

    sql_query.PREPARE_UPDATE("prs_ek_kesintiler_odenekler","prs_ek_kesintiler_odenekler ",
                             "personel_id , "
                             "odenek_kesinti_adi, "
                             "bordro_donemi_ay , "
                             "bordro_donemi_yil , "
                             "ssk_kesintisi_iceriyor_mu , "
                             "gelir_vergisi_iceriyor_mu , "
                             "damga_vergisi_iceriyor_mu , "
                             "yuzde_orani , tutar       , "
                             "yuzde_mi , "
                             "aciklama  , bordro_id  ",
                             "kesinti_odenek_id = :kesinti_odenek_id");

    sql_query.SET_VALUE(":personel_id"               , record_id            );
    sql_query.SET_VALUE(":odenek_kesinti_adi"        , kesinti_odenek_adi   );
    sql_query.SET_VALUE(":bordro_donemi_ay"          , bordro_donemi_ay     );
    sql_query.SET_VALUE(":bordro_donemi_yil"         , bordro_donemi_yil    );
    sql_query.SET_VALUE(":ssk_kesintisi_iceriyor_mu" , ssk_kesintisi_var_mi );
    sql_query.SET_VALUE(":gelir_vergisi_iceriyor_mu" , gelir_vergisi_var_mi );
    sql_query.SET_VALUE(":damga_vergisi_iceriyor_mu" , damga_vergisi_var_mi );
    sql_query.SET_VALUE(":yuzde_mi"                  , yuzde_mi_tutar_mi    );
    sql_query.SET_VALUE(":aciklama"                  , m_ui->limitedTextEdit_aciklama->toPlainText() );
    sql_query.SET_VALUE(":bordro_id"                 , 0                    );
    sql_query.SET_VALUE(":kesinti_odenek_id"         , kesinti_odenek_id    );
    if ( yuzde_mi_tutar_mi EQ PRS_YUZDE ) {
        sql_query.SET_VALUE(":yuzde_orani"           , tutar                );
        sql_query.SET_VALUE(":tutar"                 , 0                    );
    }
    else {
        sql_query.SET_VALUE(":yuzde_orani"           , 0                    );
        sql_query.SET_VALUE(":tutar"                 , tutar                );
    }
    sql_query.UPDATE();

}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );

    if ( m_bordrosu_kesildi_mi EQ 1 ) {
        MSG_WARNING( "Ödenek / Kesintinin Bordrosu alınmış Değişiklik yapamazssınız.", NULL );
        return ADAK_FAIL_UNDO;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::DELETE_LINE
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int PRS_EK_ODENEK_VE_KESINTI_FISI::LOCK_FIS_RECORD ( int record_id )
{
    return DB->LOCK_ROW ( "kes_ode_tanim_id", "prs_kesinti_odenek_tanimlar", QString ( "kes_ode_tanim_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
    DB->UNLOCK_ROW ( "kes_ode_tanim_id", "prs_kesinti_odenek_tanimlar", QString ( "kes_ode_tanim_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRS_EK_ODENEK_VE_KESINTI_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void PRS_EK_ODENEK_VE_KESINTI_FISI::LINE_SEARCH_EDIT_CLICKED(int row_number, QWidget * widget, QLineEdit * lineedit )
{

    Q_UNUSED ( widget );
    QSearchEdit * searchedit_personel_kodu = ( QSearchEdit * ) m_ui->table_widget->cellWidget(row_number, PERSONEL_KODU );

    if ( widget EQ searchedit_personel_kodu ) {

        int cari_hesap_id = OPEN_CARI_KART_ARAMA ( lineedit->text(), SAHIS_KAYDI, this, 1, E9_PROGRAMI, PERSONEL_MODULU, "PERSONEL ARAMA", "DELETED" );

        if ( cari_hesap_id < 1 ) {
            return;
        }

        SQL_QUERY       sql_query( DB );
        sql_query.PREPARE_SELECT("prs_personeller","personel_id",
                                 "cari_hesap_id = :cari_hesap_id");

        sql_query.SET_VALUE( ":cari_hesap_id"   , cari_hesap_id );

        if ( sql_query.SELECT() NE 0 ) {
            sql_query.NEXT();
            int personel_id   = sql_query.VALUE(0).toInt();

            QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( personel_id ).toString() );
            m_ui->table_widget->setItem ( row_number, PERSONEL_ID, new_item );
        }

        lineedit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id) );
        SET_FOCUS(lineedit);
    }

}
