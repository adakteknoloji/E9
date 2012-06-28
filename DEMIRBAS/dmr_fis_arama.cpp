#include "dmr_fis_arama_class.h"
#include "ui_dmr_fis_arama.h"
#include "e9_console_utils.h"
#include "dmr_enum.h"
#include "dmr_con_utils.h"
 


#define     FIS_ID_COLUMN           0
#define     FIS_TARIHI_COLUMN       1
#define     FIS_NO_COLUMN           2
#define     ACIKLAMA_COLUMN         3
#define     TOPLAM_TUTAR_COLUMN     4

extern ADAK_SQL * DB;

/**************************************************************************************
                   F_DMR_FIS_ARAMA
***************************************************************************************/


int OPEN_DMR_FIS_ARAMA ( int p_fis_turu, QWidget * parent )
{
    int secilen_id    = -1;

    DMR_FIS_ARAMA * F = new DMR_FIS_ARAMA ( p_fis_turu,parent );

    F->m_secilen_id = &secilen_id;

    F->EXEC( NOT_FULL_SCREEN );

    return secilen_id;
}

/**************************************************************************************
                   DMR_FIS_ARAMA::DMR_FIS_ARAMA
***************************************************************************************/

DMR_FIS_ARAMA::DMR_FIS_ARAMA ( int fis_turu,QWidget *parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::DMR_FIS_ARAMA)
{
    m_fis_turu = fis_turu;

    m_ui->setupUi      (this );
    START_ARAMA_KERNEL (this, DB );
}

/**************************************************************************************
                   DMR_FIS_ARAMA::SETUP_FORM
***************************************************************************************/

void DMR_FIS_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE ( tr("demirbas_fis_arama.html") );
    if ( m_fis_turu EQ AMORTISMAN_AYIRMA_FISI ) {
        SET_PAGE_TITLE ( tr ( "DMR - AMORTİSMAN FİŞLERİ ARAMA" ) );
    }
    else {
        SET_PAGE_TITLE ( tr ( "DMR - YENİDEN DEĞERLENDİRME FİŞLERİ ARAMA" ) );
    }

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );


    SET_HEADERS ( QStringList()<<tr ( "Fis Id" )<<tr ( "Fiş Tarihi" )<<tr ( "Fiş No" )<<tr ( "Açıklama" ) <<tr ( "Toplam Tutar" ) );

    SET_SORTING ( false );

    m_ui->tableWidget->setColumnWidth       ( FIS_TARIHI_COLUMN,      240 );
    m_ui->tableWidget->setColumnWidth       ( FIS_NO_COLUMN,          70  );
    m_ui->tableWidget->setColumnWidth       ( ACIKLAMA_COLUMN,        300 );
    m_ui->tableWidget->setColumnWidth       ( TOPLAM_TUTAR_COLUMN,    120 );

    SET_SETTING_NAME                        ( "DMR_FIS_ARAMA" );

    m_ui->date_frame->setEnabled            ( false );

    m_ui->tableWidget->hideColumn           ( FIS_ID_COLUMN );

    SET_ENTER_KEY_FOR_RUN_BATCH             ( true );

    SET_FIND_BUTTON_NAME                    ( tr ( "Fişleri Bul" ) );

    m_ui->adakDate_first_date->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_second_date->SET_DATE( MALI_YIL_365_DATE() );
}

/**************************************************************************************
                   DMR_FIS_ARAMA::CHECK_VAR
***************************************************************************************/

int DMR_FIS_ARAMA::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->date_check_box ) {

        if ( m_ui->date_check_box->isChecked() EQ true ) {
            m_ui->date_frame->setEnabled ( true );
        }
        else {
            m_ui->date_frame->setDisabled ( true );
            m_ui->adakDate_first_date->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_second_date->SET_DATE( MALI_YIL_365_DATE() );
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_FIS_ARAMA::CHECK_RUN
***************************************************************************************/

int DMR_FIS_ARAMA::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_FIS_ARAMA::SEARCH
***************************************************************************************/

void DMR_FIS_ARAMA::SEARCH()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "dmr_fisler", "fis_id, fis_no, fis_tarihi, aciklama, toplam_tutar", "fis_turu = :fis_turu" );
    sql_query.SET_VALUE(":fis_turu",m_fis_turu);

    if ( m_ui->date_check_box->isChecked() EQ true ) {   
        sql_query.AND_EKLE ( "fis_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi" );
        sql_query.SET_VALUE( ":baslangic_tarihi", m_ui->adakDate_first_date->DATE()  );
        sql_query.SET_VALUE( ":bitis_tarihi"    , m_ui->adakDate_second_date->DATE()     );
    }
    if ( sql_query.SELECT( "fis_tarihi ASC, fis_no ASC", 0, 1 ) EQ 0 ) {
        return;
    }

    while ( sql_query.NEXT() EQ true ) {

        int current_row = ADD_NEW_ROW ();

        m_ui->tableWidget->item ( current_row, TOPLAM_TUTAR_COLUMN  )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item ( current_row, FIS_ID_COLUMN        )->setText ( sql_query.VALUE(0).toString());
        m_ui->tableWidget->item ( current_row, FIS_NO_COLUMN        )->setText ( sql_query.VALUE(1).toString());
        m_ui->tableWidget->item ( current_row, FIS_TARIHI_COLUMN    )->setText ( sql_query.VALUE(2).toDate().toString("dd MMMM yyyy dddd"));
        m_ui->tableWidget->item ( current_row, ACIKLAMA_COLUMN      )->setText ( sql_query.VALUE(3).toString());
        m_ui->tableWidget->item ( current_row, TOPLAM_TUTAR_COLUMN  )->setText ( VIRGUL_EKLE(QVariant(ROUND(sql_query.VALUE(4).toDouble())).toString()));

    }
}

/**************************************************************************************
                   DMR_FIS_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int DMR_FIS_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{

    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number,FIS_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}
