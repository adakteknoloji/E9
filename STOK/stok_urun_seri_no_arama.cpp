#include <adak_utils.h>
#include <adak_sql.h>
#include "stok_urun_seri_no_arama_class.h"
#include "ui_stok_urun_seri_no_arama.h"
#include "sube_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_STOK_URUN_SERI_NO_ARAMA
***************************************************************************************/

int OPEN_STOK_URUN_SERI_NO_ARAMA ( int p_depo_id, int p_urun_id, QString p_seri_no, QWidget * p_parent )
{
    int secilen_id    = -1;
    STOK_URUN_SERI_NO_ARAMA * F = new STOK_URUN_SERI_NO_ARAMA( p_depo_id, p_urun_id, p_seri_no,&secilen_id, p_parent );
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   STOK_URUN_SERI_NO_ARAMA::STOK_URUN_SERI_NO_ARAMA
***************************************************************************************/

STOK_URUN_SERI_NO_ARAMA::STOK_URUN_SERI_NO_ARAMA ( int p_depo_id, int p_urun_id, QString p_seri_no,int * secilen_id, QWidget * p_parent )
: ARAMA_KERNEL ( p_parent ), m_ui ( new Ui::STOK_URUN_SERI_NO_ARAMA )
{
    m_seri_no       = p_seri_no;
    m_depo_id       = p_depo_id;
    m_urun_id       = p_urun_id;
    m_secilen_id    = secilen_id;

    m_ui->setupUi       (this );
    START_ARAMA_KERNEL  (this, DB );
}

/**************************************************************************************
                   STOK_URUN_SERI_NO_ARAMA::SETUP_FORM
***************************************************************************************/

void STOK_URUN_SERI_NO_ARAMA::SETUP_FORM()
{
    SET_PAGE_TITLE ( tr ( "ÜRÜN SERİ NO ARAMA" ) );
    SET_HELP_PAGE ("urun_seri_no_arama.html");
    SET_SETTING_NAME            ( "STOK_URUN_SERI_NO_ARAMA" );

    DISABLE_CLEAR_ON_WIDGET( m_ui->lineEdit_seri_numarasi );

    m_ui->lineEdit_seri_numarasi->setText ( "*" );

    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("stk_urunler","urun_kodu, urun_adi ",
                             "urun_id = :urun_id");
    sql_query.SET_VALUE(":urun_id", m_urun_id );

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        m_ui->label_urun->setText ( sql_query.VALUE(0).toString() + " - " + sql_query.VALUE(1).toString() );
    }

    QString depo_adi, depo_kodu;

    SUBE_GET_DEPO_KODU_ADI( m_depo_id, &depo_kodu, &depo_adi );

    m_ui->label_depo->setText ( depo_kodu + " - " + depo_adi );

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );

    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS(QStringList()<<tr("seri_no_id")<<tr("Seri Numarası"));

    SET_SORTING ( false );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_seri_numarasi );

    m_ui->tableWidget->hideColumn(0);

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    SET_FIND_BUTTON_NAME        ( tr ( "Seri No Bul" ) );
}

/**************************************************************************************
                   STOK_URUN_SERI_NO_ARAMA::CHECK_VAR
***************************************************************************************/

int STOK_URUN_SERI_NO_ARAMA::CHECK_VAR ( QObject * p_object )
{
    Q_UNUSED ( p_object );

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_SERI_NO_ARAMA::CHECK_RUN
***************************************************************************************/

int STOK_URUN_SERI_NO_ARAMA::CHECK_RUN()
{
    if ( m_ui->lineEdit_seri_numarasi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Seri numarası boş bırakılamaz"), m_ui->lineEdit_seri_numarasi );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_SERI_NO_ARAMA::SEARCH
***************************************************************************************/

void STOK_URUN_SERI_NO_ARAMA::SEARCH()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "stk_urunun_seri_nolari","seri_numarasi_id, seri_numarasi"  );

    if ( m_ui->lineEdit_seri_numarasi->text() NE "*" ) {
        sql_query.AND_EKLE( " seri_numarasi LIKE :seri_numarasi " );
        sql_query.SET_LIKE( ":seri_numarasi", m_ui->lineEdit_seri_numarasi->text() );
    }
    sql_query.AND_EKLE( "urun_id = :urun_id AND  depo_id = :depo_id AND depoda_mi = :depoda_mi" );

    sql_query.SET_VALUE(":urun_id"  , m_urun_id );
    sql_query.SET_VALUE(":depo_id"  , m_depo_id );
    sql_query.SET_VALUE(":depoda_mi", 1 );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    int current_row = 0;

    while ( sql_query.NEXT() EQ true ) {
        current_row = ADD_NEW_ROW();
        m_ui->tableWidget->item ( current_row, 0 )->setText ( sql_query.VALUE(0).toString());
        m_ui->tableWidget->item ( current_row, 1 )->setText ( sql_query.VALUE(1).toString());
    }
}

/**************************************************************************************
                   STOK_URUN_SERI_NO_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int STOK_URUN_SERI_NO_ARAMA::SINGLE_LINE_SELECTED ( int p_selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( p_selected_row_number, 0 )->text().toInt();
    return ADAK_EXIT;
}
