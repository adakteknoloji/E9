#include <QHeaderView>
#include <QMessageBox>
#include "adak_utils.h"
#include "ui_adres_not_defteri_fisi.h"
#include "adres_not_defteri_fisi_class.h"
#include "adres_not_defteri_fisi_open.h"
#include "yonetim.h"
 
 

#define ROW_ID_COLUMN               0
#define FIS_ID_COLUMN               1
#define ORDER_COLUMN                2
#define NOT_TARIHI_COLUMN           3
#define NOT_COLUMN                  4

extern ADAK_SQL   *            DB;

/**************************************************************************************
                   OPEN_ADRES_NOT_DEFTERI_FISI
***************************************************************************************/

void OPEN_ADRES_NOT_DEFTERI_FISI ( int record_id , int modul_id, int program_id, QWidget * parent)
{
    ADRES_NOT_DEFTERI_FISI * F = new ADRES_NOT_DEFTERI_FISI ( record_id,modul_id,program_id, parent );
    F->EXEC( NOT_FULL_SCREEN );
}
/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::ADRES_NOT_DEFTERI_FISI
***************************************************************************************/

ADRES_NOT_DEFTERI_FISI::ADRES_NOT_DEFTERI_FISI (int record_id, int modul_id, int program_id, QWidget * parent) : FIS_KERNEL ( parent ), m_ui ( new Ui::ADRES_NOT_DEFTERI_FISI )
{
    m_modul_id           = modul_id;
    m_program_id         = program_id;
    m_start_record_id    = record_id;

    m_ui->setupUi        ( this );
    START_FIS_KERNEL     ( this, DB );
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::SETUP_FORM
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::SETUP_FORM()
{
    SET_HELP_PAGE           ( tr("not-defteri-fisi") );

    SET_SINGLE_RECORD_ID    ( m_start_record_id );

    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_buttons_widget );

    int num_of_columns   = 5;

    REGISTER_TABLE_WIDGET             ( m_ui->tableWidget_not_defteri, num_of_columns );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN  );
    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN       ( 3 );

    SET_FIS_ORDER_DATABASE ( "adr_not_defteri", "order_number", "not_id" );

    SET_TABLE_ROW_WIDGETS ( NOT_TARIHI_COLUMN,  WIDGET_ADAK_DATE );
    SET_TABLE_ROW_WIDGETS ( NOT_COLUMN,         WIDGET_LINE_EDIT  );

    SET_PAGE_TITLE ( tr ("ADR - NOT DEFTERİ") );

    m_ui->tableWidget_not_defteri->setHorizontalHeaderLabels ( QStringList ()<<"id"<<"fis_id"<<"Column Order"
                                                                             <<tr ( "Eklenme Tarihi" )
                                                                             <<tr ( "Not" )<<tr( "" )<<tr( "" ) );

    m_ui->tableWidget_not_defteri->setColumnWidth ( NOT_TARIHI_COLUMN,  200 );
    m_ui->tableWidget_not_defteri->setColumnWidth ( NOT_COLUMN,         603 );

    SET_SETTING_NAME     ( "ADRES_NOT_DEFTERI_FISI" );

    m_ui->tableWidget_not_defteri->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tableWidget_not_defteri->setFocus( Qt::OtherFocusReason );
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::CLEAR_FORM_MEMBERS ()
{
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::NEW_FIS_RECORD
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::NEW_FIS_RECORD()
{
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::GET_FIS_RECORD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::GET_FIS_RECORD ( int record_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi","adres_id = :adres_id" );

    query.SET_VALUE ( ":adres_id", record_id );

    if ( query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    query.NEXT();

    m_ui->label_sahis_firma_name->setText ( query.VALUE(0).toString() );


    query.PREPARE_SELECT ( "adr_not_defteri","not_id, tarih, order_number, not_bilgisi","adres_kayit_id = :adres_kayit_id AND program_id = :program_id "
                           "AND modul_id = :modul_id" );

    query.SET_VALUE ( ":adres_kayit_id", record_id    );
    query.SET_VALUE ( ":program_id"    , m_program_id );
    query.SET_VALUE ( ":modul_id"      , m_modul_id   );

    if ( query.SELECT("order_number" ) EQ 0 ) {
        return ADAK_OK;
    }

    int current_row  = 0;

    while ( query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE ();

        QLineEdit        * lineEdit_not        = ( QLineEdit * ) m_ui->tableWidget_not_defteri->cellWidget ( current_row, NOT_COLUMN );
        QAdakDate *     dateEdit_not_tarihi   = ( QAdakDate * ) m_ui->tableWidget_not_defteri->cellWidget ( current_row, NOT_TARIHI_COLUMN );

        QTableWidgetItem * not_id_item         = new QTableWidgetItem ( query.VALUE(0).toString() );
        QTableWidgetItem * kayit_id_item       = new QTableWidgetItem ( QVariant(record_id).toString() );
        QTableWidgetItem * order_item          = new QTableWidgetItem ( query.VALUE(2).toString() );

        lineEdit_not->setText ( query.VALUE(3).toString() );
        dateEdit_not_tarihi->SET_DATE(query.VALUE(1).toString());

        m_ui->tableWidget_not_defteri->setItem ( current_row, ROW_ID_COLUMN, not_id_item );
        m_ui->tableWidget_not_defteri->setItem ( current_row, FIS_ID_COLUMN, kayit_id_item );
        m_ui->tableWidget_not_defteri->setItem ( current_row, ORDER_COLUMN, order_item );

    }
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QAdakDate * dateEdit_not_tarihi = ( QAdakDate * ) m_ui->tableWidget_not_defteri->cellWidget ( row_number, NOT_TARIHI_COLUMN );
    dateEdit_not_tarihi->SET_DATE(QDate::currentDate() );

    QLineEdit        * lineEdit_not        = ( QLineEdit * ) m_ui->tableWidget_not_defteri->cellWidget ( row_number, NOT_COLUMN );
    lineEdit_not->setMaxLength(128);
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_FIS_FORM_VAR ( QObject * p_object )
{
    Q_UNUSED(p_object);

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_FIS_RECORD_ADD()
{
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::ADD_FIS_RECORD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::ADD_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::UPDATE_FIS_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_FIS_RECORD_DELETE ( int record_id )
{
    Q_UNUSED ( record_id )

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_NOT_DEFTERI_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::DELETE_FIS_RECORD ( int record_id )
{

Q_UNUSED ( record_id )

}

/**************************************************************************************
                    ADRES_NOT_DEFTERI_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::SELECT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                    ADRES_NOT_DEFTERI_FISI::FIND_FIS_RECORD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::FIND_FIS_RECORD()
{
    return -1;
}

/**************************************************************************************
                    ADRES_NOT_DEFTERI_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::FIND_FIRST_FIS_RECORD()
{
    return -1;
}

/**************************************************************************************
                    ADRES_NOT_DEFTERI_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::FIND_LAST_FIS_RECORD()
{
    return -1;
}

/**************************************************************************************
                    ADRES_NOT_DEFTERI_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::FIND_PREV_FIS_RECORD()
{
    return -1;
}

/**************************************************************************************
                    ADRES_NOT_DEFTERI_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::FIND_NEXT_FIS_RECORD()
{
    return -1;
}

/**************************************************************************************
                    ADRES_NOT_DEFTERI_FISI::CHECK_LINE_VAR
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QLineEdit * lineEdit_not = ( QLineEdit * ) m_ui->tableWidget_not_defteri->cellWidget( row_number, NOT_COLUMN );

    if ( object EQ lineEdit_not ) {
        if ( lineEdit_not->text().size() > 128 ) {
            MSG_WARNING( tr("Not bilgisi uzunluğu 128 karakteri geçemez."), lineEdit_not );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                    ADRES_NOT_DEFTERI_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_LINE_EMPTY ( int row_number )
{

    QLineEdit * lineEdit_not = ( QLineEdit * ) m_ui->tableWidget_not_defteri->cellWidget( row_number, NOT_COLUMN );

    if ( lineEdit_not->text().isEmpty() EQ true ) {
         MSG_WARNING( tr ( "Not bilgisi boş bırakılamaz" ), lineEdit_not );

         return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                    ADRES_NOT_DEFTERI_FISI::CHECK_ADD_LINE
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id  );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_NOT_DEFTERI_FISI::ADD_LINE
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::ADD_LINE ( int record_id, int row_number )
{

    QLineEdit * lineEdit_not          = ( QLineEdit * ) m_ui->tableWidget_not_defteri->cellWidget ( row_number, NOT_COLUMN        );
    QAdakDate * dateEdit_not_tarihi  = ( QAdakDate * ) m_ui->tableWidget_not_defteri->cellWidget ( row_number, NOT_TARIHI_COLUMN );

    int         order_number = m_ui->tableWidget_not_defteri->item ( row_number,ORDER_COLUMN )->text().toInt();

    SQL_QUERY query ( DB );

    query.PREPARE_INSERT ( "adr_not_defteri","not_id","tarih, order_number, adres_kayit_id, ekleyen_kisi_id, not_bilgisi , program_id , modul_id");

    query.SET_VALUE ( ":tarih",                 dateEdit_not_tarihi->DATE()                        );
    query.SET_VALUE ( ":order_number",          order_number                                       );
    query.SET_VALUE ( ":adres_kayit_id",        record_id                                          );
    query.SET_VALUE ( ":ekleyen_kisi_id",       KULLANICI_ID()                                     );
    query.SET_VALUE ( ":not_bilgisi",           lineEdit_not->text()                               );
    query.SET_VALUE ( ":program_id",            m_program_id                                       );
    query.SET_VALUE ( ":modul_id",              m_modul_id                                         );

    int not_id = query.INSERT();

    QTableWidgetItem * not_id_item   = new QTableWidgetItem ( QVariant ( not_id ).toString() );
    QTableWidgetItem * kayit_id_item = new QTableWidgetItem ( QVariant ( record_id ).toString() );

    m_ui->tableWidget_not_defteri->setItem ( row_number, ROW_ID_COLUMN, not_id_item   );
    m_ui->tableWidget_not_defteri->setItem ( row_number, FIS_ID_COLUMN, kayit_id_item );
}

/**************************************************************************************
                     ADRES_NOT_DEFTERI_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id  );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_NOT_DEFTERI_FISI::UPDATE_LINE
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );

    SQL_QUERY query ( DB );

    query.PREPARE_UPDATE ( "adr_not_defteri","not_id","tarih,not_bilgisi","not_id = :not_id");


    QLineEdit * lineEdit_not        = ( QLineEdit * ) m_ui->tableWidget_not_defteri->cellWidget ( row_number,NOT_COLUMN         );
    QAdakDate * dateEdit_not_tarihi = ( QAdakDate * ) m_ui->tableWidget_not_defteri->cellWidget ( row_number, NOT_TARIHI_COLUMN );

    query.SET_VALUE ( ":tarih",          dateEdit_not_tarihi->DATE());
    query.SET_VALUE ( ":not_bilgisi",    lineEdit_not->text() );
    query.SET_VALUE ( ":not_id",         m_ui->tableWidget_not_defteri->item ( row_number, ROW_ID_COLUMN )->text());

    query.UPDATE();
}

/**************************************************************************************
                     ADRES_NOT_DEFTERI_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )

{   Q_UNUSED ( record_id  );
    Q_UNUSED ( row_number );

    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_NOT_DEFTERI_FISI::DELETE_LINE
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::DELETE_LINE (int record_id, int row_number )
{
    Q_UNUSED ( record_id );

    int not_id = m_ui->tableWidget_not_defteri->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    SQL_QUERY query ( DB );

    query.PREPARE_DELETE ( "adr_not_defteri","not_id = :not_id" );

    query.SET_VALUE(":not_id", not_id );

    query.DELETE();
}

/**************************************************************************************
                     ADRES_NOT_DEFTERI_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int ADRES_NOT_DEFTERI_FISI::LOCK_FIS_RECORD ( int record_id )
{
    return DB->LOCK_ROW ( "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                     ADRES_NOT_DEFTERI_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void ADRES_NOT_DEFTERI_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
    DB->UNLOCK_ROW ( "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( record_id ));
}
