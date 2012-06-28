#include <QMessageBox>
#include "adak_sql.h"
#include "adres_email_fisi_class.h"
#include "adres_email_fisi_open.h"
#include "ui_adres_email_fisi.h"
 
 

extern ADAK_SQL *              DB;

#define     EMAIL_ID_COLUMN             0
#define     KAYIT_ID_COLUMN             1
#define     EMAIL_ORDER_NUM             2
#define     EMAIL_ADRESI_COLUMN         3
#define     ACIKLAMA                    4


/**************************************************************************************
                   OPEN_ADRES_EMAIL_FISI
***************************************************************************************/

void OPEN_ADRES_EMAIL_FISI ( int p_adres_id, QWidget * p_parent )
{
    ADRES_EMAIL_FISI * F = new ADRES_EMAIL_FISI ( p_adres_id, p_parent );
    F->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::ADRES_EMAIL_FISI
***************************************************************************************/

ADRES_EMAIL_FISI::ADRES_EMAIL_FISI ( int p_adres_id, QWidget * p_parent ) :FIS_KERNEL ( p_parent ), m_ui ( new Ui::ADRES_EMAIL_FISI )
{
    m_start_fis_kernel   = p_adres_id;

    m_ui->setupUi        ( this );
    START_FIS_KERNEL     ( this, DB );
 }

/**************************************************************************************
                     ADRES_EMAIL_FISI::SETUP_FORM
***************************************************************************************/

void ADRES_EMAIL_FISI::SETUP_FORM()
{
    SET_HELP_PAGE                  ( tr("toplu-mail-gonderimi") );
    SET_PAGE_TITLE                 ( tr ( "ADR - EMAILLER" ) );

    SET_SINGLE_RECORD_ID           ( m_start_fis_kernel );

    REGISTER_BUTTONS_WIDGET        ( m_ui->kernel_buttons_widget, true );
    REGISTER_TABLE_WIDGET          ( m_ui->tableWidget_emailler, 5 );

    SET_FIS_ORDER_COLUMN_NUMBER    ( EMAIL_ORDER_NUM );
    SET_FORM_ID_COLUMN_POSITION    ( KAYIT_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION     ( EMAIL_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN    ( 3 );

    SET_FIS_ORDER_DATABASE         ( "adr_emailler", "order_number", "email_id" );

    SET_TABLE_ROW_WIDGETS          ( EMAIL_ADRESI_COLUMN, WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS          ( ACIKLAMA           , WIDGET_LINE_EDIT );

    m_ui->tableWidget_emailler->setColumnWidth ( EMAIL_ADRESI_COLUMN, 250 );
    m_ui->tableWidget_emailler->setColumnWidth ( ACIKLAMA           , 300 );

    SET_SETTING_NAME                ( "ADRES_EMAIL_FISI" );

    m_ui->tableWidget_emailler->setHorizontalHeaderLabels ( QStringList()<<"email_id"<<"adres_id" <<"emil_order_num"
                                                                         <<tr ( "E-Mail Adresi" ) <<tr ( "Açıklama" ) <<tr ( "" )<<tr( "" ) );
}

/**************************************************************************************
                     ADRES_EMAIL_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void ADRES_EMAIL_FISI::CLEAR_FORM_MEMBERS ()
{
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::NEW_FIS_RECORD
***************************************************************************************/

void ADRES_EMAIL_FISI::NEW_FIS_RECORD()
{
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::GET_FIS_RECORD
***************************************************************************************/

int ADRES_EMAIL_FISI::GET_FIS_RECORD ( int p_adres_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi","adres_id = :adres_id" );
    sql_query.SET_VALUE      ( ":adres_id", p_adres_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->label_adres_kaydi->setText ( sql_query.VALUE(0).toString() );

    sql_query.PREPARE_SELECT( "adr_emailler","email_id, order_number, email_adresi,aciklama","adres_kayit_id = :adres_kayit_id AND gizli_kayit = :gizli_kayit" );
    sql_query.SET_VALUE     ( ":gizli_kayit" , 0 );
    sql_query.SET_VALUE     ( ":adres_kayit_id", p_adres_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_OK;
    }

    int current_row = 0;

    while ( sql_query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE();

        QLineEdit        * lineEdit_email     = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( current_row, EMAIL_ADRESI_COLUMN );
        QLineEdit        * lineEdit_aciklama  = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( current_row, ACIKLAMA );

        QTableWidgetItem * email_id_item      = new QTableWidgetItem ( sql_query.VALUE(0).toString()      );
        QTableWidgetItem * adres_id_item      = new QTableWidgetItem ( QVariant ( p_adres_id ).toString() );
        QTableWidgetItem * email_order_item   = new QTableWidgetItem ( sql_query.VALUE(1).toString()      );

        m_ui->tableWidget_emailler->setItem ( current_row, EMAIL_ID_COLUMN, email_id_item    );
        m_ui->tableWidget_emailler->setItem ( current_row, KAYIT_ID_COLUMN, adres_id_item    );
        m_ui->tableWidget_emailler->setItem ( current_row, EMAIL_ORDER_NUM, email_order_item );

        lineEdit_email->setText ( sql_query.VALUE(2).toString() );
        lineEdit_aciklama->setText ( sql_query.VALUE(3).toString() );

    }

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void ADRES_EMAIL_FISI::SET_LINE_DEFAULTS ( int p_row_number )
{
    QLineEdit        * lineEdit_aciklama  = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( p_row_number, ACIKLAMA );
    lineEdit_aciklama->setMaxLength(100);

    QLineEdit        * lineEdit_email  = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( p_row_number, EMAIL_ADRESI_COLUMN );
    lineEdit_email->setMaxLength( 50 );

}

/**************************************************************************************
                   ADRES_EMAIL_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_FIS_FORM_VAR ( QObject * p_object )
{
    Q_UNUSED ( p_object );
    return ADAK_OK;    
}


/**************************************************************************************
                   ADRES_EMAIL_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}


/**************************************************************************************
                   ADRES_EMAIL_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_FIS_RECORD_ADD()
{
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::ADD_FIS_RECORD
***************************************************************************************/

int ADRES_EMAIL_FISI::ADD_FIS_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_FIS_RECORD_UPDATE ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void ADRES_EMAIL_FISI::UPDATE_FIS_RECORD ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
}

/**************************************************************************************
                    ADRES_EMAIL_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_FIS_RECORD_DELETE ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
    return ADAK_OK;
}


/**************************************************************************************
                    ADRES_EMAIL_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void ADRES_EMAIL_FISI::DELETE_FIS_RECORD ( int p_adres_id )
{
    Q_UNUSED ( p_adres_id );
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int ADRES_EMAIL_FISI::SELECT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::FIND_FIS_RECORD
***************************************************************************************/

int ADRES_EMAIL_FISI::FIND_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int ADRES_EMAIL_FISI::FIND_FIRST_FIS_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int ADRES_EMAIL_FISI::FIND_LAST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int ADRES_EMAIL_FISI::FIND_PREV_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int ADRES_EMAIL_FISI::FIND_NEXT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::CHECK_LINE_VAR
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_LINE_VAR ( int p_row_number, QObject * p_object )
{
    QLineEdit        * lineEdit_email     = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( p_row_number, EMAIL_ADRESI_COLUMN );

    if ( p_object EQ lineEdit_email ) {
        if ( lineEdit_email->text().size() > 50 ) {
            MSG_WARNING( tr("Email adresi 50 karakterden daha uzun olamaz"), lineEdit_email );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_LINE_EMPTY ( int p_row_number )
{
    QLineEdit * lineEdit_email = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( p_row_number, EMAIL_ADRESI_COLUMN );

    if ( lineEdit_email->text().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Email adresi boş bırakılamaz." ), lineEdit_email );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}


/**************************************************************************************
                   ADRES_EMAIL_FISI::CHECK_ADD_LINE
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_ADD_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id   );
    Q_UNUSED ( p_row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::ADD_LINE
***************************************************************************************/

void ADRES_EMAIL_FISI::ADD_LINE ( int p_adres_id, int p_row_number )
{
    int         order_number = m_ui->tableWidget_emailler->item ( p_row_number, EMAIL_ORDER_NUM )->text().toInt();
    QLineEdit * lineEdit_email  = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( p_row_number, EMAIL_ADRESI_COLUMN );
    QLineEdit * lineEdit_aciklama = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( p_row_number, ACIKLAMA );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_INSERT ( "adr_emailler","email_id", "order_number, adres_kayit_id, "
                               "email_adresi, gizli_kayit,aciklama");

    sql_query.SET_VALUE ( ":order_number",   order_number              );
    sql_query.SET_VALUE ( ":adres_kayit_id", p_adres_id                );
    sql_query.SET_VALUE ( ":email_adresi",   lineEdit_email->text()    );
    sql_query.SET_VALUE ( ":gizli_kayit",    0                         );
    sql_query.SET_VALUE ( ":aciklama",       lineEdit_aciklama->text() );

    int email_id = sql_query.INSERT();

    m_ui->tableWidget_emailler->setItem ( p_row_number, EMAIL_ID_COLUMN, new QTableWidgetItem(QVariant(email_id).toString()));

}

/**************************************************************************************
                   ADRES_EMAIL_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_UPDATE_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id   );
    Q_UNUSED ( p_row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::UPDATE_LINE
***************************************************************************************/

void ADRES_EMAIL_FISI::UPDATE_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id   );

    int         email_id        = m_ui->tableWidget_emailler->item ( p_row_number, EMAIL_ID_COLUMN )->text().toInt();
    QLineEdit * lineEdit_email  = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( p_row_number, EMAIL_ADRESI_COLUMN );
    QLineEdit * lineEdit_aciklama = ( QLineEdit * ) m_ui->tableWidget_emailler->cellWidget ( p_row_number, ACIKLAMA );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_UPDATE ( "adr_emailler","email_id","email_adresi = :email_adresi,aciklama=:aciklama","email_id = :email_id " );

    sql_query.SET_VALUE      ( ":email_adresi", lineEdit_email->text() );
    sql_query.SET_VALUE      ( ":aciklama", lineEdit_aciklama->text()  );
    sql_query.SET_VALUE      ( ":email_id", email_id                   );
    sql_query.SET_VALUE      ( ":aciklama", lineEdit_aciklama->text() );

    sql_query.UPDATE();

}

/**************************************************************************************
                   ADRES_EMAIL_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int ADRES_EMAIL_FISI::CHECK_DELETE_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id   );
    Q_UNUSED ( p_row_number );


    int clicked_button = MSG_YES_NO( tr( "Bu email adresi silinsin mi" ) , NULL );

    if ( clicked_button EQ ADAK_NO ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::DELETE_LINE
***************************************************************************************/

void ADRES_EMAIL_FISI::DELETE_LINE ( int p_adres_id, int p_row_number )
{
    Q_UNUSED ( p_adres_id );

    int email_id = m_ui->tableWidget_emailler->item ( p_row_number, EMAIL_ID_COLUMN )->text().toInt();

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_DELETE ( "adr_emailler","email_id = :email_id" );
    sql_query.SET_VALUE ( ":email_id", email_id );

    sql_query.DELETE();

}

/**************************************************************************************
                   ADRES_EMAIL_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int ADRES_EMAIL_FISI::LOCK_FIS_RECORD ( int p_adres_id )
{
    return DB->LOCK_ROW (  "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( p_adres_id ) );
}

/**************************************************************************************
                   ADRES_EMAIL_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void ADRES_EMAIL_FISI::UNLOCK_FIS_RECORD ( int p_adres_id )
{
    DB->UNLOCK_ROW (  "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( p_adres_id ) );
}
