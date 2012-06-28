#include "prs_personel_odenek_ekleme_fisi_class.h"
#include "prs_personel_odenek_ekleme_fisi_open.h"
#include "prs_enum.h"
#include "e9_log.h"
#include "sube_console_utils.h"
#include "cari_struct.h"
 
#include "cari_kart_arama_open.h"
#include "cari_console_utils.h"
#include "prs_gui_utils.h"
#include "prs_con_utils.h"

extern ADAK_SQL * DB;

#define ROW_ID_COLUMN                               0
#define RECORD_ID_COLUMN                            1
#define ORDER_COLUMN                                2
#define BORDRO_DONEMI_AY_COLUMN                     3
#define BORDRO_DONEMI_YIL_COLUMN                    4
#define ODENEK_ADI_COLUMN                           5
#define ODENEK_SSK_KESINTISI_ICERIYOR_MU_COLUMN     6
#define ODENEK_GELIR_VERGISI_ICERIYOR_MU_COLUMN     7
#define ODENEK_DAMGA_VERGISI_ICERIYOR_MU_COLUMN     8
#define ODENEK_YUZDE_MI_COLUMN                      9
#define YUZDE_COLUMN                                10
#define TUTAR_COLUMN                                11
#define ACIKLAMA_COLUMN                             12

/**************************************************************************************
                   SHOW_PRS_PERSONEL_ODENEK_EKLEME_FISI
***************************************************************************************/

void SHOW_PRS_PERSONEL_ODENEK_EKLEME_FISI( int who_called, int record_id , QWidget * parent )
{
    PRS_PERSONEL_ODENEK_EKLEME_FISI * F = new PRS_PERSONEL_ODENEK_EKLEME_FISI( who_called, record_id, parent);

    F->SHOW( NOT_FULL_SCREEN );
}
/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::PRS_PERSONEL_ODENEK_EKLEME_FISI
**************************************************************************************/

PRS_PERSONEL_ODENEK_EKLEME_FISI::PRS_PERSONEL_ODENEK_EKLEME_FISI(int who_called, int record_id, QWidget * parent) :FIS_KERNEL(parent)
{
    m_who_called = who_called;
    m_record_id  = record_id;

    setupUi             (this );
    START_FIS_KERNEL    (this, DB );
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::SETUP_FORM
***************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::SETUP_FORM()
{
    SET_HELP_PAGE  (      "personel-bordrosu_ek-odenek-tanimlama" );
    SET_PAGE_TITLE ( tr ( "PRS - EK ÖDENEK EKLEME" ) );

    SET_SETTING_NAME        ( "PRS_PERSONEL_ODENEK_EKLEME_FISI" );

    int num_of_columns   = 13;

    if ( m_who_called EQ PRS_PERSONEL_SICIL_KARTI ) {
        SET_SINGLE_RECORD_ID (m_record_id);
    }

    REGISTER_BUTTONS_WIDGET          ( navigation_button_widget);

    SET_WIDGETS_MAXIMUM_SIZE ();


    REGISTER_TABLE_WIDGET             ( table_widget_odenek_satirlari , num_of_columns );

    SET_FORM_ID_COLUMN_POSITION       ( RECORD_ID_COLUMN );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN  );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN       ( 3 );

    SET_FIS_ORDER_DATABASE ( "prs_ek_kesintiler_odenekler", "order_number", "kesinti_odenek_id" );


    SET_TABLE_ROW_WIDGETS ( BORDRO_DONEMI_AY_COLUMN, WIDGET_COMBO_BOX );
    SET_TABLE_ROW_WIDGETS ( BORDRO_DONEMI_YIL_COLUMN, WIDGET_COMBO_BOX );
    SET_TABLE_ROW_WIDGETS ( ODENEK_ADI_COLUMN, WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS ( ODENEK_SSK_KESINTISI_ICERIYOR_MU_COLUMN, WIDGET_CHECK_BOX);
    SET_TABLE_ROW_WIDGETS ( ODENEK_GELIR_VERGISI_ICERIYOR_MU_COLUMN, WIDGET_CHECK_BOX);
    SET_TABLE_ROW_WIDGETS ( ODENEK_DAMGA_VERGISI_ICERIYOR_MU_COLUMN, WIDGET_CHECK_BOX);
    SET_TABLE_ROW_WIDGETS ( ODENEK_YUZDE_MI_COLUMN, WIDGET_COMBO_BOX);
    SET_TABLE_ROW_WIDGETS ( YUZDE_COLUMN, WIDGET_COMMA_EDIT);
    SET_TABLE_ROW_WIDGETS ( TUTAR_COLUMN, WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN, WIDGET_LINE_EDIT );

    table_widget_odenek_satirlari->setHorizontalHeaderLabels(QStringList()<<"id"<<"record_id"<<"satir_order"
                                                           <<tr("Bordro \nDönemi Ay") <<tr("Bordro \nDönemi Yıl")
                                                           <<tr("Ödenek Adı")
                                                           <<tr("SSK Kesintisi")<<tr("Gelir Vergisi")<<tr("Damga Vergisi")
                                                           <<("Yüzde Mi")<<("Yüzde")<<tr("Tutar")
                                                           <<tr("Açıklama")<<tr("")<<tr("") );

    table_widget_odenek_satirlari->setColumnWidth ( BORDRO_DONEMI_AY_COLUMN   , 120  );
    table_widget_odenek_satirlari->setColumnWidth ( BORDRO_DONEMI_YIL_COLUMN  , 120  );
    table_widget_odenek_satirlari->setColumnWidth ( ODENEK_ADI_COLUMN         , 120  );
    table_widget_odenek_satirlari->setColumnWidth ( ODENEK_SSK_KESINTISI_ICERIYOR_MU_COLUMN     , 180  );
    table_widget_odenek_satirlari->setColumnWidth ( ODENEK_GELIR_VERGISI_ICERIYOR_MU_COLUMN     , 180  );
    table_widget_odenek_satirlari->setColumnWidth ( ODENEK_DAMGA_VERGISI_ICERIYOR_MU_COLUMN     , 180  );
    table_widget_odenek_satirlari->setColumnWidth ( ODENEK_YUZDE_MI_COLUMN    , 120  );
    table_widget_odenek_satirlari->setColumnWidth ( YUZDE_COLUMN              , 80  );
    table_widget_odenek_satirlari->setColumnWidth ( TUTAR_COLUMN              , 80  );
    table_widget_odenek_satirlari->setColumnWidth ( ACIKLAMA_COLUMN           , 150 );

    SET_FIRST_FOCUS_WIDGET ( table_widget_odenek_satirlari );

    FOCUS_FIRST_WIDGET();

    line_edit_personel_sicil_no->setReadOnly(true);
    line_edit_personel_adi_soyadi->setReadOnly(true);
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::NEW_FIS_RECORD
***************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::NEW_FIS_RECORD()
{
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::GET_FIS_RECORD
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::GET_FIS_RECORD ( int record_id )
{
    SQL_QUERY   sql_query(DB);
    SQL_QUERY   query    (DB);

    sql_query.PREPARE_SELECT( "prs_personeller ",
                              "sicil_no, cari_hesap_id",
                              "personel_id = :personel_id" );
    sql_query.SET_VALUE( ":personel_id", record_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    line_edit_personel_sicil_no->setText ( sql_query.VALUE(0).toString() );

    int cari_hesap_id   = sql_query.VALUE( 1 ).toInt();

    CARI_HESAP_STRUCT hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri  );

    line_edit_personel_adi_soyadi->setText( hesap_bilgileri.cari_hesap_ismi );


    query.PREPARE_SELECT ( "prs_ek_kesintiler_odenekler",
                           "kesinti_odenek_id, personel_id, order_number,  "
                           "odenek_kesinti_adi, bordro_donemi_ay, bordro_donemi_yil, ssk_kesintisi_iceriyor_mu, "
                           "gelir_vergisi_iceriyor_mu, damga_vergisi_iceriyor_mu, "
                           "yuzde_mi, yuzde_orani, tutar, aciklama, bordro_id ",
                           "personel_id = :personel_id "
                           "AND kesinti_odenek_turu = :kesinti_odenek_turu ");


    query.SET_VALUE      ( ":personel_id",          record_id );
    query.SET_VALUE      ( ":kesinti_odenek_turu"  , ENUM_PRS_ODENEK);

    query.SELECT("order_number ASC" );

    while ( query.NEXT() EQ true ) {

        int p_current_row;

        p_current_row = ADD_NEW_LINE ();

        QWidget     *  cell_widget = table_widget_odenek_satirlari->cellWidget( p_current_row, BORDRO_DONEMI_AY_COLUMN );
        QLineEdit   *   line_edit_odenek_adi                        = ( QLineEdit  * ) table_widget_odenek_satirlari->cellWidget( p_current_row, ODENEK_ADI_COLUMN );
        QComboBox   *   combo_box_bordro_donemi_ay                  = ( QComboBox  * ) table_widget_odenek_satirlari->cellWidget( p_current_row, BORDRO_DONEMI_AY_COLUMN );
        QComboBox   *   combo_box_bordro_donemi_yil                 = ( QComboBox  * ) table_widget_odenek_satirlari->cellWidget( p_current_row, BORDRO_DONEMI_YIL_COLUMN );
        QCheckBox   *   check_box_odenek_ssk_kesintisi_iceriyor_mu  = ( QCheckBox  * ) table_widget_odenek_satirlari->cellWidget( p_current_row, ODENEK_SSK_KESINTISI_ICERIYOR_MU_COLUMN );
        QCheckBox   *   check_box_odenek_gelir_vergisi_iceriyor_mu  = ( QCheckBox  * ) table_widget_odenek_satirlari->cellWidget( p_current_row, ODENEK_GELIR_VERGISI_ICERIYOR_MU_COLUMN );
        QCheckBox   *   check_box_odenek_damga_vergisi_iceriyor_mu  = ( QCheckBox  * ) table_widget_odenek_satirlari->cellWidget( p_current_row, ODENEK_DAMGA_VERGISI_ICERIYOR_MU_COLUMN );
        QComboBox   *   combo_box_odenek_yuzde_mi                   = ( QComboBox  * ) table_widget_odenek_satirlari->cellWidget( p_current_row, ODENEK_YUZDE_MI_COLUMN );
        QCommaEdit  *   comma_edit_yuzde_orani                      = ( QCommaEdit * ) table_widget_odenek_satirlari->cellWidget( p_current_row, YUZDE_COLUMN );
        QCommaEdit  *   comma_edit_tutar                            = ( QCommaEdit * ) table_widget_odenek_satirlari->cellWidget( p_current_row, TUTAR_COLUMN );

        line_edit_odenek_adi->setText( query.VALUE("odenek_kesinti_adi").toString() );
        combo_box_bordro_donemi_ay->setCurrentIndex( query.VALUE("bordro_donemi_ay").toInt() );
        combo_box_bordro_donemi_yil->setCurrentIndex( combo_box_bordro_donemi_yil->findText( query.VALUE("bordro_donemi_yil").toString()) );
        check_box_odenek_ssk_kesintisi_iceriyor_mu->setChecked( query.VALUE("ssk_kesintisi_iceriyor_mu").toInt() );
        check_box_odenek_gelir_vergisi_iceriyor_mu->setChecked( query.VALUE("gelir_vergisi_iceriyor_mu").toInt() );
        check_box_odenek_damga_vergisi_iceriyor_mu->setChecked( query.VALUE("damga_vergisi_iceriyor_mu").toInt() );
        combo_box_odenek_yuzde_mi->setCurrentIndex( query.VALUE("yuzde_mi").toInt() );
        comma_edit_yuzde_orani->SET_DOUBLE ( query.VALUE("yuzde_orani").toDouble() );
        comma_edit_tutar->SET_DOUBLE ( query.VALUE("tutar").toDouble() );

        if ( combo_box_odenek_yuzde_mi->currentText() EQ tr("TUTAR") ) {
            comma_edit_tutar->setEnabled( true );
        }
        else {
            comma_edit_yuzde_orani->setEnabled( true );
        }

        cell_widget = table_widget_odenek_satirlari->cellWidget( p_current_row, ACIKLAMA_COLUMN );
        QLineEdit  *   line_edit_aciklama    = static_cast < QLineEdit * > ( cell_widget );
        line_edit_aciklama->setText( query.VALUE("aciklama").toString() );


        QString id                                  = query.VALUE("kesinti_odenek_id").toString();
        QString personel_id                         = query.VALUE("personel_id").toString();
        QString order_number                        = query.VALUE("order_number").toString();

        QTableWidgetItem * new_item;

        new_item = new QTableWidgetItem(tr("%1").arg(id));
        table_widget_odenek_satirlari->setItem(p_current_row, ROW_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(personel_id));
        table_widget_odenek_satirlari->setItem(p_current_row, RECORD_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(order_number));
        table_widget_odenek_satirlari->setItem(p_current_row, ORDER_COLUMN, new_item);

    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::SET_LINE_DEFAULTS ( int row_number ) {

    QWidget   * cell_widget;

    QComboBox * combo_box_bordro_donemi_ay;
    QComboBox * combo_box_bordro_donemi_yil;
    QCheckBox * check_box_odenek_ssk_kesintisi_iceriyor_mu;
    QCheckBox * check_box_odenek_gelir_vergisi_iceriyor_mu;
    QCheckBox * check_box_odenek_damga_vergisi_iceriyor_mu;
    QComboBox * combobox_odenek_yuzde_mi;

    QStringList bordro_donemi_ay;
    QString     odenek_ssk_kesintisi_iceriyor_mu;
    QString     odenek_gelir_vergisi_iceriyor_mu;
    QString     odenek_damga_vergisi_iceriyor_mu;
    QStringList odenek_yuzde_mi;


    QLineEdit * line_edit_aciklama     = ((QLineEdit *) table_widget_odenek_satirlari->cellWidget(row_number, ACIKLAMA_COLUMN ));
    QLineEdit *line_edit_odenek_adi    = ((QLineEdit*)table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_ADI_COLUMN ));

    line_edit_odenek_adi->setMaxLength   (128) ;

    line_edit_aciklama->setMaxLength    ( 512 );


    bordro_donemi_ay <<("")<< tr("OCAK")<< tr("ŞUBAT")<< tr("MART") << tr("NİSAN")
                     << tr("MAYIS")<< tr("HAZİRAN")<< tr("TEMMUZ") << tr("AĞUSTOS")
                     << tr("EYLÜL")<< tr("EKİM")<< tr("KASIM") << tr("ARALIK");
    cell_widget                  = table_widget_odenek_satirlari->cellWidget(row_number, BORDRO_DONEMI_AY_COLUMN);
    combo_box_bordro_donemi_ay   = static_cast <QComboBox*> (cell_widget);
    combo_box_bordro_donemi_ay->addItems(bordro_donemi_ay);
    combo_box_bordro_donemi_ay->setCurrentIndex( QDate::currentDate().month());


    cell_widget                   = table_widget_odenek_satirlari->cellWidget(row_number, BORDRO_DONEMI_YIL_COLUMN);
    combo_box_bordro_donemi_yil   = static_cast <QComboBox*> (cell_widget);
    PRS_FILL_COMBO_BOX_BORDRO_YILLARI( combo_box_bordro_donemi_yil );

    odenek_ssk_kesintisi_iceriyor_mu = tr("SSK Kesintisi");
    cell_widget                   = table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_SSK_KESINTISI_ICERIYOR_MU_COLUMN);
    check_box_odenek_ssk_kesintisi_iceriyor_mu      = static_cast <QCheckBox*> (cell_widget);
    check_box_odenek_ssk_kesintisi_iceriyor_mu->setText(odenek_ssk_kesintisi_iceriyor_mu);

    odenek_gelir_vergisi_iceriyor_mu = tr("Gelir Vergisi");
    cell_widget                   = table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_GELIR_VERGISI_ICERIYOR_MU_COLUMN);
    check_box_odenek_gelir_vergisi_iceriyor_mu      = static_cast <QCheckBox*> (cell_widget);
    check_box_odenek_gelir_vergisi_iceriyor_mu->setText(odenek_gelir_vergisi_iceriyor_mu);

    odenek_damga_vergisi_iceriyor_mu = tr("Damga Vergisi");
    cell_widget                   = table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_DAMGA_VERGISI_ICERIYOR_MU_COLUMN);
    check_box_odenek_damga_vergisi_iceriyor_mu      = static_cast <QCheckBox*> (cell_widget);
    check_box_odenek_damga_vergisi_iceriyor_mu->setText(odenek_damga_vergisi_iceriyor_mu);

    odenek_yuzde_mi << tr("TUTAR")<< tr("YÜZDE");
    cell_widget                   = table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_YUZDE_MI_COLUMN);
    combobox_odenek_yuzde_mi     = static_cast <QComboBox*> (cell_widget);
    combobox_odenek_yuzde_mi->addItems(odenek_yuzde_mi);


    QCommaEdit * comma_edit_tutar          =  ( QCommaEdit * ) table_widget_odenek_satirlari->cellWidget(row_number, TUTAR_COLUMN );
    QCommaEdit * comma_edit_yuzde_orani    =  ( QCommaEdit * ) table_widget_odenek_satirlari->cellWidget(row_number, YUZDE_COLUMN );

    comma_edit_tutar->setEnabled( true );
    comma_edit_yuzde_orani->setEnabled( false );
}

/**************************************************************************************
         PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_FORM_VAR
 **************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    Q_UNUSED ( object );
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_FORM_EMPTY
**********************************************************8****************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_RECORD_ADD()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::ADD_FIS_RECORD
**************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::ADD_FIS_RECORD()
{
    return 1;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_RECORD_UPDATE
****************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_RECORD_UPDATE(int record_id)
{
    Q_UNUSED(record_id);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::UPDATE_FIS_RECORD
**************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::UPDATE_FIS_RECORD(int record_id)
{
    Q_UNUSED(record_id);
}

/**************************************************************************************
                  PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    Q_UNUSED(record_id);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::DELETE_FIS_RECORD
**************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::DELETE_FIS_RECORD ( int record_id )
{
    Q_UNUSED(record_id);
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::SELECT_FIS_RECORD()
{
    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA, this, 0, E9_PROGRAMI, PERSONEL_MODULU , "Personel Arama", "DELETED" );

    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id ",
                             "cari_hesap_id = :cari_hesap_id");

    sql_query.SET_VALUE( ":cari_hesap_id"   , cari_hesap_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_FIS_RECORD
**************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id",
                             "sicil_no = :sicil_no ");

    sql_query.SET_VALUE( ":sicil_no"   ,line_edit_personel_sicil_no->text() );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
              PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personel_nufus_blgler","personel_id ");


    if ( sql_query.SELECT("personel_id ASC",0,1) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personel_nufus_blgler","personel_id ");


    if ( sql_query.SELECT("personel_id DESC",0,1) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id ",
                              "sicil_no < :sicil_no ");


    sql_query.SET_VALUE( ":sicil_no", line_edit_personel_sicil_no->text() );

    if ( sql_query.SELECT("sicil_no DESC",0,1) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id ",
                              "sicil_no > :sicil_no");

    sql_query.SET_VALUE( ":sicil_no", line_edit_personel_sicil_no->text() );

    if ( sql_query.SELECT("sicil_no ASC",0,1) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_LINE_VAR
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{
    QComboBox  * combo_box_odenek_yuzde_mi =  ( QComboBox  * ) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_YUZDE_MI_COLUMN );
    QCommaEdit * comma_edit_tutar          =  ( QCommaEdit * ) table_widget_odenek_satirlari->cellWidget(row_number, TUTAR_COLUMN );
    QCommaEdit * comma_edit_yuzde_orani    =  ( QCommaEdit * ) table_widget_odenek_satirlari->cellWidget(row_number, YUZDE_COLUMN );

    if( object EQ combo_box_odenek_yuzde_mi  ) {

        if ( combo_box_odenek_yuzde_mi->currentText() EQ tr("TUTAR") ) {
            comma_edit_yuzde_orani->setEnabled( false );
            comma_edit_tutar->setEnabled( true );
        }
        else {
            comma_edit_tutar->setEnabled( false );
            comma_edit_yuzde_orani->setEnabled( true );
        }
        comma_edit_tutar->clear();
        comma_edit_yuzde_orani->clear();
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_LINE_EMPTY(int row_number)
{
    QComboBox  * combo_box_odenek_yuzde_mi =  ( QComboBox  * ) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_YUZDE_MI_COLUMN );
    QCommaEdit * comma_edit_tutar          =  ( QCommaEdit * ) table_widget_odenek_satirlari->cellWidget(row_number, TUTAR_COLUMN );
    QCommaEdit * comma_edit_yuzde_orani    =  ( QCommaEdit * ) table_widget_odenek_satirlari->cellWidget(row_number, YUZDE_COLUMN );
    QLineEdit  * line_edit_odenek_adi      =  ( QLineEdit  * ) table_widget_odenek_satirlari->cellWidget( row_number, ODENEK_ADI_COLUMN );

    if ( line_edit_odenek_adi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr( "Ödenek adını girmek zorundasınız." ), line_edit_odenek_adi );
        return ADAK_FAIL;
    }
    if ( combo_box_odenek_yuzde_mi->currentText() EQ  tr("TUTAR") ) {
        if ( comma_edit_tutar->GET_DOUBLE() EQ  0.00 ) {
            MSG_WARNING( tr( "Tutarı girmek zorundasınız." ), comma_edit_tutar );
            return ADAK_FAIL;
        }
    }
    else { // yuzde
        if ( comma_edit_yuzde_orani->GET_DOUBLE() EQ  0.00 ) {
            MSG_WARNING( tr( "Yüzdeyi girmek zorundasınız." ), comma_edit_yuzde_orani );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}


/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_ADD_LINE
**************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_ADD_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                  PRS_PERSONEL_ODENEK_EKLEME_FISI::ADD_LINE
***************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::ADD_LINE(int record_id, int row_number)
{
    SQL_QUERY   sql_query(DB);

    double    tutar               = 0;
    double    yuzde_orani         = 0 ;
    int       order_number                          = table_widget_odenek_satirlari->item ( row_number, ORDER_COLUMN )->text().toInt();

    QString   odenek_adi                            = ((QLineEdit  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_ADI_COLUMN))->text();
    int       bordro_donemi_ay                      = ((QComboBox  *) table_widget_odenek_satirlari->cellWidget(row_number, BORDRO_DONEMI_AY_COLUMN ))->currentIndex();
    int       bordro_donemi_yil                     = ((QComboBox  *) table_widget_odenek_satirlari->cellWidget(row_number, BORDRO_DONEMI_YIL_COLUMN ))->currentText().toInt();
    int       odenek_ssk_kesintisi_iceriyor_mu      = ((QCheckBox  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_SSK_KESINTISI_ICERIYOR_MU_COLUMN))->isChecked();
    int       odenek_gelir_vergisi_iceriyor_mu      = ((QCheckBox  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_GELIR_VERGISI_ICERIYOR_MU_COLUMN))->isChecked();
    int       odenek_damga_vergisi_iceriyor_mu      = ((QCheckBox  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_DAMGA_VERGISI_ICERIYOR_MU_COLUMN))->isChecked();
    int       odenek_yuzde_mi                       = ((QComboBox  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_YUZDE_MI_COLUMN))->currentIndex();
    yuzde_orani                                     = ((QCommaEdit *) table_widget_odenek_satirlari->cellWidget(row_number, YUZDE_COLUMN ))->GET_DOUBLE();
    tutar                                           = ((QCommaEdit *) table_widget_odenek_satirlari->cellWidget(row_number, TUTAR_COLUMN ))->GET_DOUBLE();
    QString   aciklama                              = ((QLineEdit  *) table_widget_odenek_satirlari->cellWidget(row_number, ACIKLAMA_COLUMN))->text();


    int       bordro_id = PRS_GET_BORDRO_ID( bordro_donemi_ay, bordro_donemi_yil,  SUBE_GET_SUBE_ID() ) ;

    sql_query.PREPARE_INSERT("prs_ek_kesintiler_odenekler","kesinti_odenek_id",
                             "personel_id, order_number,  "
                             "odenek_kesinti_adi, bordro_donemi_ay, bordro_donemi_yil, "
                             "ssk_kesintisi_iceriyor_mu, gelir_vergisi_iceriyor_mu, "
                             "damga_vergisi_iceriyor_mu, yuzde_mi, yuzde_orani, tutar, "
                             "aciklama, bordrosu_kesildi_mi , kesinti_odenek_turu , bordro_id ");


    sql_query.SET_VALUE(":personel_id"                              , record_id                                 );
    sql_query.SET_VALUE(":order_number"                             , order_number                              );
    sql_query.SET_VALUE(":odenek_kesinti_adi"                       , odenek_adi                                );
    sql_query.SET_VALUE(":bordro_donemi_ay"                         , bordro_donemi_ay                         );
    sql_query.SET_VALUE(":bordro_donemi_yil"                        , bordro_donemi_yil                        );
    sql_query.SET_VALUE(":ssk_kesintisi_iceriyor_mu"                , odenek_ssk_kesintisi_iceriyor_mu          );
    sql_query.SET_VALUE(":gelir_vergisi_iceriyor_mu"                , odenek_gelir_vergisi_iceriyor_mu          );
    sql_query.SET_VALUE(":damga_vergisi_iceriyor_mu"                , odenek_damga_vergisi_iceriyor_mu          );
    sql_query.SET_VALUE(":yuzde_mi"                                 , odenek_yuzde_mi                           );
    sql_query.SET_VALUE(":yuzde_orani"                              , yuzde_orani                               );
    sql_query.SET_VALUE(":tutar"                                    , tutar                                     );
    sql_query.SET_VALUE(":aciklama"                                 , aciklama                                  );
    sql_query.SET_VALUE(":bordrosu_kesildi_mi"                      , 0                                         );
    sql_query.SET_VALUE(":kesinti_odenek_turu"                      , ENUM_PRS_ODENEK                           );
    sql_query.SET_VALUE(":bordro_id"                                , bordro_id                                 );


    int satir_id = sql_query.INSERT();

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( satir_id ).toString() );
    table_widget_odenek_satirlari->setItem ( row_number, ROW_ID_COLUMN, new_item );

    QString personel_adi_soyadi = line_edit_personel_adi_soyadi->text();
    QString personel_sicil_no   = line_edit_personel_sicil_no->text();

    QString log_detaylari = "Personel Ödenek Ekleme";

    log_detaylari.append("\nPersonel Adı / Soyadı : " + personel_adi_soyadi +
                         "Sicil No : " + personel_sicil_no);

    E9_LOG_KAYDI_EKLE ( PERSONEL_MODULU , -1 , LOG_ISLEM_ADD , log_detaylari);

}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::UPDATE_LINE
**************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    SQL_QUERY   sql_query(DB);

    int       row_id                        = table_widget_odenek_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();
    int       bordro_donemi_ay              = ((QComboBox  *) table_widget_odenek_satirlari->cellWidget(row_number, BORDRO_DONEMI_AY_COLUMN ))->currentIndex();
    int       bordro_donemi_yil             = ((QComboBox  *) table_widget_odenek_satirlari->cellWidget(row_number, BORDRO_DONEMI_YIL_COLUMN ))->currentText().toInt();
    QString   odenek_adi                    = ((QLineEdit  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_ADI_COLUMN))->text();
    int       ssk_kesintisi_iceriyor_mu     = ((QCheckBox  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_SSK_KESINTISI_ICERIYOR_MU_COLUMN))->isChecked();
    int       gelir_vergisi_iceriyor_mu     = ((QCheckBox  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_GELIR_VERGISI_ICERIYOR_MU_COLUMN))->isChecked();
    int       damga_vergisi_iceriyor_mu     = ((QCheckBox  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_DAMGA_VERGISI_ICERIYOR_MU_COLUMN))->isChecked();
    int       odenek_yuzde_mi               = ((QComboBox  *) table_widget_odenek_satirlari->cellWidget(row_number, ODENEK_YUZDE_MI_COLUMN))->currentIndex();
    double    yuzde_orani                   = ((QCommaEdit *) table_widget_odenek_satirlari->cellWidget(row_number, YUZDE_COLUMN ))->GET_DOUBLE();
    double    tutar                         = ((QCommaEdit *) table_widget_odenek_satirlari->cellWidget(row_number, TUTAR_COLUMN ))->GET_DOUBLE();
    QString   aciklama                      = ((QLineEdit  *) table_widget_odenek_satirlari->cellWidget(row_number, ACIKLAMA_COLUMN))->text();


    sql_query.PREPARE_UPDATE("prs_ek_kesintiler_odenekler","prs_ek_kesintiler_odenekler ",
                             "personel_id , "
                             "odenek_kesinti_adi, "
                             "bordro_donemi_ay , "
                             "bordro_donemi_yil , "
                             "ssk_kesintisi_iceriyor_mu , "
                             "gelir_vergisi_iceriyor_mu , "
                             "damga_vergisi_iceriyor_mu , "
                             "yuzde_orani , tutar , "
                             "yuzde_mi , "
                             "aciklama  , bordro_id  ",
                             "kesinti_odenek_id = :kesinti_odenek_id");

    sql_query.SET_VALUE(":personel_id"                              , record_id                         );
    sql_query.SET_VALUE(":odenek_kesinti_adi"                       , odenek_adi                        );
    sql_query.SET_VALUE(":bordro_donemi_ay"                         , bordro_donemi_ay                  );
    sql_query.SET_VALUE(":bordro_donemi_yil"                        , bordro_donemi_yil                 );
    sql_query.SET_VALUE(":ssk_kesintisi_iceriyor_mu"                , ssk_kesintisi_iceriyor_mu         );
    sql_query.SET_VALUE(":gelir_vergisi_iceriyor_mu"                , gelir_vergisi_iceriyor_mu         );
    sql_query.SET_VALUE(":damga_vergisi_iceriyor_mu"                , damga_vergisi_iceriyor_mu         );
    sql_query.SET_VALUE(":yuzde_mi"                                 , odenek_yuzde_mi                   );
    sql_query.SET_VALUE(":yuzde_orani"                              , yuzde_orani                       );
    sql_query.SET_VALUE(":tutar"                                    , tutar                             );
    sql_query.SET_VALUE(":aciklama"                                 , aciklama                          );
    sql_query.SET_VALUE(":kesinti_odenek_id"                        , row_id                      );
    sql_query.SET_VALUE(":bordro_id"                                , PRS_GET_BORDRO_ID(bordro_donemi_ay , bordro_donemi_yil, SUBE_GET_SUBE_ID()));

    sql_query.UPDATE();

    QString personel_adi_soyadi = line_edit_personel_adi_soyadi->text();
    QString personel_sicil_no   = line_edit_personel_sicil_no->text();

    QString log_detaylari = "Personel Ödenek Ekleme";

    log_detaylari.append("\nPersonel Adı / Soyadı : " + personel_adi_soyadi +
                         "Sicil No : " + personel_sicil_no);

    E9_LOG_KAYDI_EKLE ( PERSONEL_MODULU , -1 , LOG_ISLEM_UPDATE , log_detaylari);
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_DELETE_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::DELETE_LINE
***************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::DELETE_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);
    SQL_QUERY sql_query (DB);

    int row_id = table_widget_odenek_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    sql_query.PREPARE_DELETE("prs_ek_kesintiler_odenekler", "kesinti_odenek_id = :kesinti_odenek_id");
    sql_query.SET_VALUE(":kesinti_odenek_id", row_id );
    sql_query.DELETE();

    QString personel_adi_soyadi = line_edit_personel_adi_soyadi->text();
    QString personel_sicil_no   = line_edit_personel_sicil_no->text();

    QString log_detaylari = "Personel Ödenek Ekleme";

    log_detaylari.append("\nPersonel Adı / Soyadı : " + personel_adi_soyadi +
                         "Sicil No : " + personel_sicil_no);

    E9_LOG_KAYDI_EKLE ( PERSONEL_MODULU , -1 , LOG_ISLEM_DELETE , log_detaylari);
}

/**************************************************************************************
                 PRS_PERSONEL_ODENEK_EKLEME_FISI::LOCK_FIS_RECORD
**************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::LOCK_FIS_RECORD(int record_id)
{
    return DB->LOCK_ROW( "personel_id","prs_personeller", QString ( "personel_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::UNLOCK_FIS_RECORD(int record_id)
{
    DB->UNLOCK_ROW( "personel_id","prs_personeller", QString ( "personel_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_EXIT
***************************************************************************************/

int PRS_PERSONEL_ODENEK_EKLEME_FISI::CHECK_EXIT()
{
    return ADAK_OK;
}

/***************************************************************************************
                   PRS_PERSONEL_ODENEK_EKLEME_FISI::SET_WIDGETS_MAXIMUM_SIZE
 ***************************************************************************************/

void PRS_PERSONEL_ODENEK_EKLEME_FISI::SET_WIDGETS_MAXIMUM_SIZE ()
{


    line_edit_personel_adi_soyadi->setMaxLength (100);

    line_edit_personel_sicil_no->setMaxLength    (20);
}


