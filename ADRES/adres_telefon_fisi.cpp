#include <QMessageBox>
#include "adak_sql.h"
#include "ui_adres_telefon_fisi.h"
#include "adres_telefon_fisi_class.h"
#include "adres_telefon_fisi_open.h"
#include "adak_std_utils.h"
 
 


extern ADAK_SQL *              DB;

#define     TELEFON_ID_COLUMN           0
#define     KAYIT_ID_COLUMN             1
#define     TELEFON_ORDER_NUM           2
#define     TEL_ULKE_KODU_COLUMN        3
#define     TEL_ALAN_KODU_COLUMN        4
#define     TELEFON_COLUMN              5
#define     IS_DAHILI_TELEFON_COLUMN    6
#define     ACIKLAMA_COLUMN             7


/**************************************************************************************
                     OPEN_ADRES_TELEFON_FISI
***************************************************************************************/

void OPEN_ADRES_TELEFON_FISI ( int p_adres_id, QWidget * p_parent )
{
    ADRES_TELEFON_FISI * F = new ADRES_TELEFON_FISI ( p_adres_id, p_parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::ADRES_TELEFON_FISI
***************************************************************************************/

ADRES_TELEFON_FISI::ADRES_TELEFON_FISI ( int p_adres_id, QWidget * p_parent ) :FIS_KERNEL ( p_parent ),
m_ui ( new Ui::ADRES_TELEFON_FISI )
{
    m_adres_id          = p_adres_id;

    m_ui->setupUi    (this );
    START_FIS_KERNEL (this, DB );
 }

/**************************************************************************************
                     ADRES_TELEFON_FISI::SETUP_FORM
***************************************************************************************/

void ADRES_TELEFON_FISI::SETUP_FORM()
{
    SET_HELP_PAGE       ( tr("telefon-fisi") );
    SET_PAGE_TITLE      ( tr ( "ADR - TELEFONLAR" ) );
    SET_SETTING_NAME    ( "ADRES_TELEFON_FISI" );

    SET_SINGLE_RECORD_ID         ( m_adres_id );
    REGISTER_BUTTONS_WIDGET      ( m_ui->kernel_buttons_widget, true );
    REGISTER_TABLE_WIDGET        ( m_ui->tableWidget_telefonlar, 8 );
    SET_FIS_ORDER_COLUMN_NUMBER  ( TELEFON_ORDER_NUM );
    SET_FORM_ID_COLUMN_POSITION  ( KAYIT_ID_COLUMN   );
    SET_ROW_ID_COLUMN_POSITION   ( TELEFON_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN  ( 3 );

    SET_FIS_ORDER_DATABASE ("adr_telefonlar", "order_number", "telefon_id" );

    SET_TABLE_ROW_WIDGETS ( TEL_ULKE_KODU_COLUMN,       WIDGET_COMBO_BOX    );
    SET_TABLE_ROW_WIDGETS ( TEL_ALAN_KODU_COLUMN,       WIDGET_COMBO_BOX    );
    SET_TABLE_ROW_WIDGETS ( TELEFON_COLUMN,             WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( IS_DAHILI_TELEFON_COLUMN,   WIDGET_COMMA_EDIT   );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN,            WIDGET_LINE_EDIT    );

    m_ui->tableWidget_telefonlar->setHorizontalHeaderLabels(QStringList()<<"telefon_id"<<"adres_id"<<"order_column"
                                                                         <<tr ( "Ülke kodu" )<<tr ( "Alan Kodu" )
                                                                         <<tr ( "Telefon" )<<tr("Dahili")<<tr("Açıklama")<<tr ( "" )<<tr("") );

    m_ui->tableWidget_telefonlar->setColumnWidth ( TEL_ULKE_KODU_COLUMN,        90  );
    m_ui->tableWidget_telefonlar->setColumnWidth ( TEL_ALAN_KODU_COLUMN,        90  );
    m_ui->tableWidget_telefonlar->setColumnWidth ( TELEFON_COLUMN,              110 );
    m_ui->tableWidget_telefonlar->setColumnWidth ( IS_DAHILI_TELEFON_COLUMN,    90  );
    m_ui->tableWidget_telefonlar->setColumnWidth ( ACIKLAMA_COLUMN,             170 );

    m_ui->tableWidget_telefonlar->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tableWidget_telefonlar->setFocus         ( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET ();
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void ADRES_TELEFON_FISI::CLEAR_FORM_MEMBERS ()
{
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::NEW_FIS_RECORD
***************************************************************************************/

void ADRES_TELEFON_FISI::NEW_FIS_RECORD ()
{
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::GET_FIS_RECORD
***************************************************************************************/

int ADRES_TELEFON_FISI::GET_FIS_RECORD ( int p_adres_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi,adres_kart_turu","adres_id = :adres_id" );
    sql_query.SET_VALUE      ( ":adres_id", p_adres_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->label_firma_sahis_adi->setText ( sql_query.VALUE(0).toString());

    if (sql_query.VALUE(1) EQ FIRMA_KAYDI ) {
        m_ui->tableWidget_telefonlar->hideColumn ( IS_DAHILI_TELEFON_COLUMN );
    }

    sql_query.PREPARE_SELECT ( "adr_telefonlar","telefon_id, order_number, telefon_ulke_kodu, telefon_alan_kodu, "
                               "telefon, telefon_dahili,aciklama ","adres_kayit_id = :adres_kayit_id AND gizli_kayit = :gizli_kayit ");

    sql_query.SET_VALUE ( ":adres_kayit_id", p_adres_id );
    sql_query.SET_VALUE ( ":gizli_kayit"   , 0);

    if ( sql_query.SELECT("order_number") EQ 0 ) {
        return ADAK_OK;
    }

    int current_row = 0;

    while ( sql_query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE ();

        QComboBox        * comboBox_ulke_kodu = ( QComboBox  * ) m_ui->tableWidget_telefonlar->cellWidget ( current_row, TEL_ULKE_KODU_COLUMN     );
        QComboBox        * comboBox_alan_kodu = ( QComboBox  * ) m_ui->tableWidget_telefonlar->cellWidget ( current_row, TEL_ALAN_KODU_COLUMN     );
        QLineEdit        * lineEdit_telefon   = ( QLineEdit  * ) m_ui->tableWidget_telefonlar->cellWidget ( current_row, TELEFON_COLUMN           );
        QCommaEdit       * commaEdit_dahili   = ( QCommaEdit * ) m_ui->tableWidget_telefonlar->cellWidget ( current_row, IS_DAHILI_TELEFON_COLUMN );
        QLineEdit        * lineEdit_aciklama  = ( QLineEdit  * ) m_ui->tableWidget_telefonlar->cellWidget ( current_row, ACIKLAMA_COLUMN          );

        QTableWidgetItem * telefon_id_item    = new QTableWidgetItem ( sql_query.VALUE(0).toString()      );
        QTableWidgetItem * adres_id_item      = new QTableWidgetItem ( QVariant ( p_adres_id ).toString() );
        QTableWidgetItem * telefon_order_item = new QTableWidgetItem ( sql_query.VALUE(1).toString()      );

        FIND_AND_SET_COMBOBOX_TEXT ( comboBox_ulke_kodu, sql_query.VALUE(2).toString() );

        GET_ALAN_KODLARI           ( comboBox_alan_kodu, comboBox_ulke_kodu->currentText() );
        FIND_AND_SET_COMBOBOX_TEXT ( comboBox_alan_kodu, sql_query.VALUE(3).toString() );

        lineEdit_telefon->setText ( sql_query.VALUE(4).toString() );

        commaEdit_dahili->SET_INTEGER ( sql_query.VALUE(5).toInt() );
        lineEdit_aciklama->setText(sql_query.VALUE(6).toString());

        m_ui->tableWidget_telefonlar->setItem ( current_row, TELEFON_ID_COLUMN, telefon_id_item    );
        m_ui->tableWidget_telefonlar->setItem ( current_row, KAYIT_ID_COLUMN,   adres_id_item      );
        m_ui->tableWidget_telefonlar->setItem ( current_row, TELEFON_ORDER_NUM, telefon_order_item );
    }

    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void ADRES_TELEFON_FISI::SET_LINE_DEFAULTS ( int p_row_number )
{
    QComboBox  * comboBox_ulke_kodu = ( QComboBox  * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, TEL_ULKE_KODU_COLUMN     );
    QComboBox  * comboBox_alan_kodu = ( QComboBox  * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, TEL_ALAN_KODU_COLUMN     );
    QLineEdit  * lineEdit_telefon   = ( QLineEdit  * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, TELEFON_COLUMN           );
    QCommaEdit * commaEdit_dahili   = ( QCommaEdit * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, IS_DAHILI_TELEFON_COLUMN );
    QLineEdit  * lineEdit_aciklama  = ( QLineEdit  * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, ACIKLAMA_COLUMN          );

    lineEdit_aciklama->setMaxLength(100);

    GET_ULKE_TELEFON_KODLARI( comboBox_ulke_kodu );

    QString turkiye_ulke_kodu = GET_ULKE_TELEFON_KODU( "Türkiye" );

    FIND_AND_SET_COMBOBOX_TEXT ( comboBox_ulke_kodu, turkiye_ulke_kodu );

    GET_ALAN_KODLARI           ( comboBox_alan_kodu );

    lineEdit_telefon->setInputMask  ("999-99-99");
    lineEdit_telefon->setCursorPosition(0);

    commaEdit_dahili->setMaxLength(5);
    commaEdit_dahili->SET_PRECISION(0);
    commaEdit_dahili->VIRGULSUZ_CALIS(true);
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    Q_UNUSED ( object );
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_FIS_RECORD_ADD()
{
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::ADD_FIS_RECORD
***************************************************************************************/

int ADRES_TELEFON_FISI::ADD_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_FIS_RECORD_UPDATE ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void ADRES_TELEFON_FISI::UPDATE_FIS_RECORD ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_FIS_RECORD_DELETE ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void ADRES_TELEFON_FISI::DELETE_FIS_RECORD ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int ADRES_TELEFON_FISI::SELECT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::FIND_FIS_RECORD
***************************************************************************************/

int ADRES_TELEFON_FISI::FIND_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int ADRES_TELEFON_FISI::FIND_FIRST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int ADRES_TELEFON_FISI::FIND_LAST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int ADRES_TELEFON_FISI::FIND_PREV_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int ADRES_TELEFON_FISI::FIND_NEXT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_LINE_VAR
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_LINE_VAR ( int p_row_number, QObject * p_object )
{
    QComboBox  * comboBox_ulke_kodu = ( QComboBox  * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, TEL_ULKE_KODU_COLUMN      );
    QComboBox  * comboBox_alan_kodu = ( QComboBox  * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, TEL_ALAN_KODU_COLUMN      );
    QCommaEdit * commaEdit_dahili   = ( QCommaEdit * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, IS_DAHILI_TELEFON_COLUMN  );


    if ( p_object EQ comboBox_ulke_kodu ) {

        if ( comboBox_ulke_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr("Ülke kodu 10 karakterden fazla olamaz"), comboBox_ulke_kodu );

            return ADAK_FAIL;
        }
        GET_ALAN_KODLARI ( comboBox_alan_kodu, comboBox_ulke_kodu->currentText() );
    }
    else if ( p_object EQ comboBox_alan_kodu ) {
        if ( comboBox_alan_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr("Alan kodu 10 karakterden fazla olamaz"), comboBox_alan_kodu );

            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ commaEdit_dahili ) {
        if ( commaEdit_dahili->GET_TEXT().size() > 5 ) {
            MSG_WARNING( tr("Dahili numara 5 karakterden fazla olamaz"), commaEdit_dahili );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_LINE_EMPTY ( int p_row_number )
{
    QComboBox  * comboBox_ulke_kodu = ( QComboBox * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, TEL_ULKE_KODU_COLUMN );
    QComboBox  * comboBox_alan_kodu = ( QComboBox * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, TEL_ALAN_KODU_COLUMN );
    QLineEdit  * lineEdit_telefon   = ( QLineEdit * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, TELEFON_COLUMN       );

    if ( comboBox_ulke_kodu->currentText().isEmpty() EQ true ) {
        MSG_WARNING( tr("Ülke kodu boş bırakılamaz."), comboBox_ulke_kodu );

        return ADAK_FAIL;
    }
    if ( comboBox_alan_kodu->currentText().isEmpty() EQ true ) {
        MSG_WARNING( tr("Alan kodu boş bırakılamaz."), comboBox_alan_kodu );

        return ADAK_FAIL;
    }

    if ( lineEdit_telefon->text().replace( "-", "" ).size() < 7 ) {
        MSG_WARNING( tr("Telefon numarası eksik girilmiş"), lineEdit_telefon );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}


/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_ADD_LINE
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_ADD_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id   );
    Q_UNUSED ( p_row_number );
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::ADD_LINE
***************************************************************************************/

void ADRES_TELEFON_FISI::ADD_LINE ( int p_adres_id, int p_row_number )
{
    QComboBox  * comboBox_ulke_kodu = ( QComboBox * ) m_ui->tableWidget_telefonlar->cellWidget  ( p_row_number, TEL_ULKE_KODU_COLUMN      );
    QComboBox  * comboBox_alan_kodu = ( QComboBox * ) m_ui->tableWidget_telefonlar->cellWidget  ( p_row_number, TEL_ALAN_KODU_COLUMN      );
    QLineEdit  * lineEdit_telefon   = ( QLineEdit * ) m_ui->tableWidget_telefonlar->cellWidget  ( p_row_number, TELEFON_COLUMN            );
    QCommaEdit * commaEdit_dahili   = ( QCommaEdit * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, IS_DAHILI_TELEFON_COLUMN  );
    QLineEdit  * lineEdit_aciklama  = ( QLineEdit * ) m_ui->tableWidget_telefonlar->cellWidget  ( p_row_number, ACIKLAMA_COLUMN           );

    int tel_order_num = m_ui->tableWidget_telefonlar->item ( p_row_number, TELEFON_ORDER_NUM )->text().toInt();

    QString telefon = lineEdit_telefon->text();
    telefon.replace ( "-", "");

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_INSERT  ( "adr_telefonlar","telefon_id", "adres_kayit_id, order_number, "
                                "telefon_ulke_kodu, telefon_alan_kodu, telefon, telefon_dahili, gizli_kayit, aciklama");

    sql_query.SET_VALUE ( ":adres_kayit_id",    p_adres_id                                             );
    sql_query.SET_VALUE ( ":order_number",      tel_order_num                                          );
    sql_query.SET_VALUE ( ":telefon_ulke_kodu", comboBox_ulke_kodu->currentText()                      );
    sql_query.SET_VALUE ( ":telefon_alan_kodu", comboBox_alan_kodu->currentText()                      );
    sql_query.SET_VALUE ( ":telefon",           telefon                                                );
    sql_query.SET_VALUE ( ":telefon_dahili",    commaEdit_dahili->GET_TEXT()                           );
    sql_query.SET_VALUE ( ":gizli_kayit",       0                                                      );
    sql_query.SET_VALUE ( ":aciklama",          lineEdit_aciklama->text()                              );

    int telefon_id = sql_query.INSERT();

    QTableWidgetItem * telefon_id_item  = new QTableWidgetItem ( QVariant ( telefon_id ).toString() );
    QTableWidgetItem * adres_id_item    = new QTableWidgetItem ( QVariant ( p_adres_id ).toString() );

    m_ui->tableWidget_telefonlar->setItem ( p_row_number, TELEFON_ID_COLUMN, telefon_id_item );
    m_ui->tableWidget_telefonlar->setItem ( p_row_number, KAYIT_ID_COLUMN,   adres_id_item   );

}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_UPDATE_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id  );
    Q_UNUSED ( p_row_number );
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::UPDATE_LINE
***************************************************************************************/

void ADRES_TELEFON_FISI::UPDATE_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id );

    QComboBox  * comboBox_ulke_kodu = ( QComboBox * ) m_ui->tableWidget_telefonlar->cellWidget  ( p_row_number, TEL_ULKE_KODU_COLUMN     );
    QComboBox  * comboBox_alan_kodu = ( QComboBox * ) m_ui->tableWidget_telefonlar->cellWidget  ( p_row_number, TEL_ALAN_KODU_COLUMN     );
    QLineEdit  * lineEdit_telefon   = ( QLineEdit * ) m_ui->tableWidget_telefonlar->cellWidget  ( p_row_number, TELEFON_COLUMN           );
    QCommaEdit * commaEdit_dahili   = ( QCommaEdit * ) m_ui->tableWidget_telefonlar->cellWidget ( p_row_number, IS_DAHILI_TELEFON_COLUMN );
    QLineEdit  * lineEdit_aciklama  = ( QLineEdit * ) m_ui->tableWidget_telefonlar->cellWidget  ( p_row_number, ACIKLAMA_COLUMN          );

    int telefon_id = m_ui->tableWidget_telefonlar->item ( p_row_number, TELEFON_ID_COLUMN )->text().toInt();

    QString telefon = lineEdit_telefon->text();
    telefon.replace ( "-", "");

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_UPDATE("adr_telefonlar","telefon_id","telefon_ulke_kodu, "
                             "telefon_alan_kodu,telefon,telefon_dahili, "
                             "aciklama","telefon_id = :telefon_id" );

    sql_query.SET_VALUE ( ":telefon_ulke_kodu", comboBox_ulke_kodu->currentText() );
    sql_query.SET_VALUE ( ":telefon_alan_kodu", comboBox_alan_kodu->currentText() );
    sql_query.SET_VALUE ( ":telefon",           telefon                           );
    sql_query.SET_VALUE ( ":telefon_dahili",    commaEdit_dahili->GET_TEXT()      );
    sql_query.SET_VALUE ( ":aciklama",          lineEdit_aciklama->text()         );
    sql_query.SET_VALUE ( ":telefon_id",        telefon_id                        );    

    sql_query.UPDATE();
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int ADRES_TELEFON_FISI::CHECK_DELETE_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id  );
    Q_UNUSED ( p_row_number );

    int clicked_button = MSG_YES_NO( tr("Seçili telefon kaydı silinsin mi?"), NULL );

    if ( clicked_button EQ ADAK_NO ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::DELETE_LINE
***************************************************************************************/

void ADRES_TELEFON_FISI::DELETE_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id );

    int telefon_id = m_ui->tableWidget_telefonlar->item ( p_row_number, TELEFON_ID_COLUMN )->text().toInt();

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_DELETE( "adr_telefonlar","telefon_id = :telefon_id");
    sql_query.SET_VALUE     ( ":telefon_id", telefon_id );
    sql_query.DELETE();
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int ADRES_TELEFON_FISI::LOCK_FIS_RECORD ( int p_adres_id )
{
    return DB->LOCK_ROW ( "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( p_adres_id ));
}

/**************************************************************************************
                     ADRES_TELEFON_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void ADRES_TELEFON_FISI::UNLOCK_FIS_RECORD ( int p_adres_id )
{
    DB->UNLOCK_ROW ( "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( p_adres_id ));
}
