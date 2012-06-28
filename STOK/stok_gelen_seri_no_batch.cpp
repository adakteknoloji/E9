#include "stok_gelen_seri_no_batch_open.h"
#include "stok_gelen_seri_no_batch_class.h"
#include "ui_stok_gelen_seri_no_batch.h"
#include "stok_console_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_STOK_GELEN_SERI_NO_BATCH
***************************************************************************************/

void OPEN_STOK_GELEN_SERI_NO_BATCH ( int p_depo_id, int p_urun_id, QStringList *p_seri_no_list, QWidget * parent )
{
    STOK_GELEN_SERI_NO_BATCH * F = new STOK_GELEN_SERI_NO_BATCH ( p_depo_id, p_urun_id, p_seri_no_list, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::STOK_GELEN_SERI_NO_BATCH
***************************************************************************************/

STOK_GELEN_SERI_NO_BATCH::STOK_GELEN_SERI_NO_BATCH ( int p_depo_id, int p_urun_id, QStringList *p_seri_no_list, QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::STOK_GELEN_SERI_NO_BATCH )
{
    m_depo_id      = p_depo_id;
    m_urun_id      = p_urun_id;
    m_seri_no_list = p_seri_no_list;

    m_ui->setupUi      ( this );    
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::SETUP_FORM
***************************************************************************************/

void STOK_GELEN_SERI_NO_BATCH::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Tamam" );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_seri_no );

    SET_AUTO_EXIT_BATCH( true );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    STK_GET_URUN_KODU_ADI( m_urun_id, NULL, &m_urun_adi );

    m_ui->label_urun_adi->setText( m_urun_adi );

    SET_PAGE_TITLE    (tr("SERİ NO GİRİŞ"));
    SET_SETTING_NAME  ("STOK_GELEN_SERI_NO_BATCH");
    SET_HELP_PAGE     ("seri_ni_giris.html");

    m_ui->lineEdit_seri_no->setMaxLength( 30 );

    QStringList header;

    header << tr( "Seri Numarası" ) << tr("");

    m_ui->tableWidget_seri_nolar->setColumnCount( 2 );
    m_ui->tableWidget_seri_nolar->setHorizontalHeaderLabels( header );

    m_ui->tableWidget_seri_nolar->setColumnWidth( 0, 90 );
    m_ui->tableWidget_seri_nolar->setColumnWidth( 1, 10 );

    // gelen liste table widget eklenecek

    for (int i = 0; i < m_seri_no_list->size(); i++ ) {
        YENI_BOS_SATIR_EKLE();
        QLineEdit * line_edit_seri_no = ( QLineEdit * ) m_ui->tableWidget_seri_nolar->cellWidget( i, 0 );
        line_edit_seri_no->setText( m_seri_no_list->at( i ) );
    }
}

/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::CHECK_VAR
***************************************************************************************/

int STOK_GELEN_SERI_NO_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->pushButton_ekle ) {

        if( m_ui->lineEdit_seri_no->text().isEmpty() EQ true ) {
            MSG_WARNING( tr( "Seri numarasını girmelisiniz" ), m_ui->lineEdit_seri_no );
            return ADAK_FAIL;
        }
        if( m_seri_no_list->contains( m_ui->lineEdit_seri_no->text() ) EQ true ) {
            MSG_WARNING( "Seri numarası zaten var.!", m_ui->lineEdit_seri_no );
            return ADAK_FAIL;
        }

        // eger veri tabaninda var ise
        if( VERI_TABANI_KONTROLU( m_ui->lineEdit_seri_no->text() ) EQ false ) {
            return ADAK_FAIL;
        }

        YENI_BOS_SATIR_EKLE();
        int row = m_ui->tableWidget_seri_nolar->rowCount() -1; // 0 dan basliyor
        QLineEdit * line_edit_seri_no = ( QLineEdit * ) m_ui->tableWidget_seri_nolar->cellWidget( row, 0 );
        line_edit_seri_no->setText( m_ui->lineEdit_seri_no->text() );

        m_seri_no_list->append( m_ui->lineEdit_seri_no->text() );
        m_ui->lineEdit_seri_no->clear();
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::CHECK_RUN
***************************************************************************************/

int STOK_GELEN_SERI_NO_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::RUN_BATCH
***************************************************************************************/

void STOK_GELEN_SERI_NO_BATCH::RUN_BATCH ()
{
    close();
}

/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::CHECK_EXIT
***************************************************************************************/

int STOK_GELEN_SERI_NO_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::YENI_BOS_SATIR_EKLE
***************************************************************************************/

void STOK_GELEN_SERI_NO_BATCH::YENI_BOS_SATIR_EKLE()
{
    int p_row = m_ui->tableWidget_seri_nolar->rowCount();

    m_ui->tableWidget_seri_nolar->insertRow( p_row );

    QLineEdit   * line_edit_seri_no = new QLineEdit( this );
    line_edit_seri_no->setObjectName( "seri_no" );
    line_edit_seri_no->setReadOnly( true );
    QToolButton * tool_button_sil   = new QToolButton( this );
    tool_button_sil->setIcon( QPixmap(":kernel_icons/kernel_sil_ikonu.png"));

    connect( tool_button_sil, SIGNAL( clicked()), this, SLOT( SLOT_BUTTON_SIL()) );

    m_ui->tableWidget_seri_nolar->setCellWidget( p_row, 0, line_edit_seri_no );
    m_ui->tableWidget_seri_nolar->setCellWidget( p_row, 1, tool_button_sil  );

    SET_FOCUS( m_ui->lineEdit_seri_no );
}

/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::SLOT_BUTTON_SIL
***************************************************************************************/

void STOK_GELEN_SERI_NO_BATCH::SLOT_BUTTON_SIL()
{
    QToolButton * tool_button = static_cast< QToolButton * >( sender() );

    for (int i = 0; i < m_ui->tableWidget_seri_nolar->rowCount(); i++) {
        if ( m_ui->tableWidget_seri_nolar->cellWidget(i, 1 ) EQ tool_button ) {
            QLineEdit * line_edit_seri_no = ( QLineEdit * )m_ui->tableWidget_seri_nolar->cellWidget( i, 0 );
            m_seri_no_list->removeAt(  m_seri_no_list->indexOf( line_edit_seri_no->text() ) );
            m_ui->tableWidget_seri_nolar->removeRow( i );
        }
    }
}

/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::SERI_NO_EKLE
***************************************************************************************/

void STOK_GELEN_SERI_NO_BATCH::SERI_NO_EKLE()
{
        int current_row = m_ui->tableWidget_seri_nolar->currentRow();

        if ( current_row NE -1 ) {
            QLineEdit * line_edit_satir_seri_no = ( QLineEdit * ) m_ui->tableWidget_seri_nolar->cellWidget( current_row, 0 );

            if( line_edit_satir_seri_no->text().isEmpty() NE true ) {
                if ( m_seri_no_list->contains( line_edit_satir_seri_no->text() ) EQ false ) {
                    m_seri_no_list->append( line_edit_satir_seri_no->text() );
                }
                YENI_BOS_SATIR_EKLE();
            }
            else{
                return;
            }
        }
}


/**************************************************************************************
                   STOK_GELEN_SERI_NO_BATCH::SERI_NO_EKLE
***************************************************************************************/

bool STOK_GELEN_SERI_NO_BATCH::VERI_TABANI_KONTROLU( QString p_seri_no )
{

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_urunun_seri_nolari","seri_numarasi_id ",
                             "depo_id           = :depo_id "
                             "AND urun_id       = :urun_id "
                             "AND seri_numarasi = :seri_numarasi");

    sql_query.SET_VALUE(":depo_id"       , m_depo_id );
    sql_query.SET_VALUE(":urun_id"       , m_urun_id );
    sql_query.SET_VALUE(":seri_numarasi" , p_seri_no );

    if ( sql_query.SELECT() > 0 ) {
        MSG_WARNING(  tr("Bu seri numarası, bu ürün için seçili depoda kullanılmış. \n Lütfen farklı bir seri numarası giriniz."), m_ui->lineEdit_seri_no );

        return false;
    }

    return true;
}
