#include "muh_hesap_arama_class.h"
#include "muh_hesap_arama_open.h"
#include "ui_muh_hesap_arama.h"
 
 

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_MUH_HESAP_ARAMA
***************************************************************************************/

int OPEN_MUH_HESAP_ARAMA ( const QString& search_value, QWidget * parent, int show_add_button )
{
    int secilen_id    = -1;

    QString value   = search_value;
    MUH_HESAP_ARAMA * F = new MUH_HESAP_ARAMA ( value, show_add_button, parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   MUH_HESAP_ARAMA::MUH_HESAP_ARAMA
***************************************************************************************/

MUH_HESAP_ARAMA::MUH_HESAP_ARAMA ( QString hesap_kodu_baslangici , int show_add_button,  QWidget * parent ) : ARAMA_KERNEL (parent), m_ui ( new Ui::MUH_HESAP_ARAMA )
{
    m_hesap_kodu_baslangici = hesap_kodu_baslangici;
    m_show_add_button       = show_add_button;

    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this,DB );
}

/**************************************************************************************
                   MUH_HESAP_ARAMA::SETUP_FORM
***************************************************************************************/

void MUH_HESAP_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE       ( tr("muhasebe_hesap_arama.html") );
    SET_PAGE_TITLE      ( tr ( "MUH - HESAP ARAMA" ) );
    SET_SETTING_NAME    ( "MUH_HESAP_ARAMA" );
     
    SET_FIRST_FOCUS_WIDGET ( m_ui->line_edit_hesap_kodu_ismi );

    REGISTER_TABLE_WIDGET          ( m_ui->tableWidget ) ;
    REGISTER_ARAMA_BUTTONS_WIDGET  ( m_ui->widget_batch_buttons );


    SET_HEADERS(QStringList()<<tr ( "Hesap Id" )<<tr ( "Tam Hesap Kodu" )<<tr ( "Hesap İsmi" ) );

    m_ui->tableWidget->setColumnWidth (  1, 40 );
    m_ui->tableWidget->setColumnWidth (  2, 60 );

    m_ui->tableWidget->hideColumn(0);

    SET_FIND_BUTTON_NAME ( tr ( "Hesap Bul" ) );

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    if ( m_hesap_kodu_baslangici.isEmpty() EQ true ) {
        m_hesap_kodu_baslangici = "*";
    }

    m_ui->line_edit_hesap_kodu_ismi->setText ( m_hesap_kodu_baslangici );

    if ( m_ui->line_edit_hesap_kodu_ismi->text() NE "*" ) {
        AUTO_START ();
    }
}

/**************************************************************************************
                   MUH_HESAP_ARAMA::CHECK_VAR
***************************************************************************************/

int MUH_HESAP_ARAMA::CHECK_VAR ( QObject * object )
{
    Q_UNUSED(object);
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_ARAMA::CHECK_RUN
***************************************************************************************/

int MUH_HESAP_ARAMA::CHECK_RUN()
{
    if ( m_ui->line_edit_hesap_kodu_ismi->text().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Hesap kodu alanı boş bırakılamaz" ), m_ui->line_edit_hesap_kodu_ismi  );

            return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_ARAMA::SEARCH
***************************************************************************************/

void MUH_HESAP_ARAMA::SEARCH()
{


    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id,tam_hesap_kodu, hesap_ismi" );

    sql_query.AND_EKLE( "tam_hesap_kodu LIKE :tam_hesap_kodu_1 OR hesap_ismi LIKE :tam_hesap_kodu_2" );
    sql_query.SET_LIKE( ":tam_hesap_kodu_1" , m_ui->line_edit_hesap_kodu_ismi->text());
    sql_query.SET_LIKE( ":tam_hesap_kodu_2" , m_ui->line_edit_hesap_kodu_ismi->text());


    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    while ( sql_query.NEXT() EQ true ) {
        int current_row = ADD_NEW_ROW();
        m_ui->tableWidget->item ( current_row,0 )->setText ( sql_query.VALUE(0).toString() );
        m_ui->tableWidget->item ( current_row,1 )->setText ( sql_query.VALUE(1).toString() );
        m_ui->tableWidget->item ( current_row,2 )->setText ( sql_query.VALUE(2).toString() );
    }
}

/**************************************************************************************
                   MUH_HESAP_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int MUH_HESAP_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number,0 )->text().toInt();
    return ADAK_EXIT;
}
