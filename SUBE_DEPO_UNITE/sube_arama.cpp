#include "adak_sql.h"
#include "sube_arama_class.h"
#include "ui_sube_arama.h"
 

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_SUBE_ARAMA
***************************************************************************************/

int OPEN_SUBE_ARAMA (  QString sube_adi_sube_kodu, QWidget * p_parent )
{
    int secilen_id    = -1;
    SUBE_ARAMA * F = new SUBE_ARAMA( sube_adi_sube_kodu, p_parent);
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   SUBE_ARAMA::SUBE_ARAMA
***************************************************************************************/

SUBE_ARAMA::SUBE_ARAMA ( QString sube_adi_sube_kodu, QWidget  * p_parent ) : ARAMA_KERNEL ( p_parent ),
m_ui ( new Ui::SUBE_ARAMA )
{
    m_sube_adi_sube_kodu = sube_adi_sube_kodu;

    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, DB );

}

/**************************************************************************************
                   SUBE_ARAMA::SETUP_FORM
***************************************************************************************/

void SUBE_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE  ( "sube_arama.html" );
    SET_PAGE_TITLE ( tr( "ŞUBE - ŞUBE ARAMA" ) );

    m_ui->checkBox_sube_adi_sube_kodu->setChecked ( true );
    m_ui->frame_kasa_unite->setEnabled            ( false );

    REGISTER_TABLE_WIDGET           ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET   ( m_ui->widget_batch_buttons );

    if ( m_sube_adi_sube_kodu.isEmpty() EQ true ) {
        m_ui->lineEdit_sube_adi_sube_kodu->setText("*");
    }
    else {
        m_ui->lineEdit_sube_adi_sube_kodu->setText( m_sube_adi_sube_kodu );
    }


    SET_HEADERS            ( QStringList()<<tr("Şube Id")<<tr("Şube Kodu")<<tr("Şube Adı"));
    SET_SORTING            ( false );
    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_sube_adi_sube_kodu );

    m_ui->tableWidget->hideColumn(0);

    m_ui->tableWidget->setColumnWidth   ( 1, 40 );
    m_ui->tableWidget->setColumnWidth   ( 2, 60 );

    SET_SETTING_NAME                    ( "SUBE_ARAMA" );

    SET_ENTER_KEY_FOR_RUN_BATCH         ( true );
    SET_FIND_BUTTON_NAME                ( tr( "Şube Bul" ) );

    if ( m_ui->lineEdit_sube_adi_sube_kodu->text().isEmpty() EQ false ) {
        AUTO_START ();
    }
}

/**************************************************************************************
                   SUBE_ARAMA::CHECK_VAR
***************************************************************************************/

int SUBE_ARAMA::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->checkBox_sube_adi_sube_kodu ) {
        if ( m_ui->checkBox_sube_adi_sube_kodu->isChecked() EQ true ) {
            m_ui->frame_sube->setEnabled(true);
            SET_FOCUS( m_ui->lineEdit_sube_adi_sube_kodu );
        }
        else {
            m_ui->lineEdit_sube_adi_sube_kodu->clear();
            m_ui->frame_sube->setEnabled(false);
        }
    }
    else if ( object EQ m_ui->checkBox_kasa_unite_adi ) {
        if ( m_ui->checkBox_kasa_unite_adi->isChecked() EQ true ) {
            m_ui->frame_kasa_unite->setEnabled(true);
            SET_FOCUS( m_ui->lineEdit_kasa_unite_adi );
        }
        else {
            m_ui->lineEdit_kasa_unite_adi->clear();
            m_ui->frame_kasa_unite->setEnabled(false);
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_ARAMA::CHECK_RUN
***************************************************************************************/

int SUBE_ARAMA::CHECK_RUN()
{
    if ( m_ui->checkBox_sube_adi_sube_kodu->isChecked() EQ true ) {
        if ( m_ui->lineEdit_sube_adi_sube_kodu->text().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Şube adı boş bırakılamaz." ), m_ui->lineEdit_sube_adi_sube_kodu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_kasa_unite_adi->isChecked() EQ true ) {
        if ( m_ui->lineEdit_kasa_unite_adi->text().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Ünite adı boş bırakılamaz." ), m_ui->lineEdit_kasa_unite_adi );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_ARAMA::SEARCH
***************************************************************************************/

void SUBE_ARAMA::SEARCH()
{

    SQL_QUERY sql_query ( DB );

    if ( m_ui->checkBox_sube_adi_sube_kodu->isChecked() EQ false AND
         m_ui->checkBox_kasa_unite_adi->isChecked() EQ false ) {
        return;
    }

    QString tables_name = "sub_subeler";

    if ( m_ui->checkBox_kasa_unite_adi->isChecked() EQ true ) {
            tables_name += ", sub_uniteler";
    }

    sql_query.PREPARE_SELECT( tables_name, "sub_subeler.sube_id, sube_kodu, sube_adi" );

    if ( m_ui->checkBox_sube_adi_sube_kodu->isChecked() EQ true ) {

        sql_query.AND_EKLE( "sube_adi LIKE :sube_adi OR sube_kodu LIKE :sube_kodu" );
        sql_query.SET_LIKE( ":sube_kodu",  m_ui->lineEdit_sube_adi_sube_kodu->text());
        sql_query.SET_LIKE( ":sube_adi", m_ui->lineEdit_sube_adi_sube_kodu->text());
    }

    if ( m_ui->checkBox_kasa_unite_adi->isChecked() EQ true ) {
        sql_query.AND_EKLE( "unite_adi LIKE :unite_adi " );
        sql_query.SET_LIKE( ":unite_adi", m_ui->lineEdit_kasa_unite_adi->text() );
    }

    if ( m_ui->checkBox_kasa_unite_adi->isChecked() EQ true ) {
      sql_query.AND_EKLE( "sub_subeler.sube_id = sub_uniteler.sube_id" );
    }

    if ( sql_query.SELECT("sube_kodu ASC" ) EQ 0 ) {
        return;
    }

    int current_row;

    while ( sql_query.NEXT() EQ true ) {

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item ( current_row,0 )->setText(sql_query.VALUE(0).toString());
        m_ui->tableWidget->item ( current_row,1 )->setText(sql_query.VALUE(1).toString());
        m_ui->tableWidget->item ( current_row,2 )->setText(sql_query.VALUE(2).toString());
    }

}

/**************************************************************************************
                   SUBE_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int SUBE_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number,0 )->text().toInt();
    return ADAK_EXIT;
}
