#include "prs_personel_raporu_batch_class.h"
#include "prs_personel_raporu_batch_open.h"
#include "print.h"
#include "prs_enum.h"
#include "prs_struct.h"
#include "e9_log.h"
#include "prs_personel_raporu_open.h"
#include "prs_gui_utils.h"
#include "sube_console_utils.h"
#include <QDebug>
extern ADAK_SQL       *     DB;
extern PRS_SABITLER          G_PRS_SABITLER;



/**************************************************************************************
                   SHOW_PRS_PERSONEL_RAPORU_BATCH
***************************************************************************************/
void SHOW_PRS_PERSONEL_RAPORU_BATCH ( QWidget * parent )
{
    PRS_PERSONEL_RAPORU_BATCH * B = new PRS_PERSONEL_RAPORU_BATCH(  parent );
    B->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRS_PERSONEL_RAPORU_BATCH::PRS_PERSONEL_RAPORU_BATCH
***************************************************************************************/

PRS_PERSONEL_RAPORU_BATCH::PRS_PERSONEL_RAPORU_BATCH(  QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::PRS_PERSONEL_RAPORU_BATCH)
{
    m_ui->setupUi               ( this );
    START_BATCH_KERNEL          ( this, DB );
}

/**************************************************************************************
                   PRS_PERSONEL_RAPORU_BATCH::SETUP_FORM
***************************************************************************************/

void PRS_PERSONEL_RAPORU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE                   ( "personel-bordrosu_personel-raporu");
    SET_PAGE_TITLE                  ( tr ( "PRS - PERSONEL RAPORU" )    );

    REGISTER_BUTTON_WIDGET          ( m_ui->widget_batch_buttons  );

    SET_SETTING_NAME                ( "PRS_PERSONEL_RAPORU_BATCH" );
    SET_NAME_OF_RUN_BATCH_BUTTON    ( "PERSONEL RAPORU AL"        );
    SET_FIRST_FOCUS_WIDGET          ( m_ui->comboBox_subeler      );

    m_ui->comboBox_subeler->addItem ( "Hepsi" , 0                 );

    PRS_FILL_COMBO_BOX_SUBE_SECIMI  ( m_ui->comboBox_subeler      ) ;

    m_sube_id = -1;
}

/**************************************************************************************
                   PRS_PERSONEL_RAPORU_BATCH::CHECK_VAR
***************************************************************************************/

int PRS_PERSONEL_RAPORU_BATCH::CHECK_VAR(QObject * object)
{
    if ( object EQ m_ui->comboBox_subeler ) {
            int result = SUBE_GET_SUBE_ID_ADI( m_ui->comboBox_subeler->currentText().split("-").at(0) , &m_sube_id );

            if ( result EQ 0 ) {
                return 0;
            }
        }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_RAPORU_BATCH::CHECK_RUN
***************************************************************************************/

int PRS_PERSONEL_RAPORU_BATCH::CHECK_RUN()
{
    if ( m_ui->comboBox_subeler->currentIndex() < 0 ){
        MSG_WARNING("Şube seçimi yapmalısınız." , m_ui->comboBox_subeler);
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_RAPORU_BATCH::RUN_BATCH
***************************************************************************************/

void PRS_PERSONEL_RAPORU_BATCH::RUN_BATCH()
{


    if (m_ui->comboBox_subeler->currentText() EQ "Hepsi") {
        m_sube_id = -1;
    }

     OPEN_REPORT_SHOWER( SHOW_PERSONEL_RAPORU( m_sube_id ), nativeParentWidget() );

     DB->START_TRANSACTION();

     E9_LOG_KAYDI_EKLE( PERSONEL_MODULU , LOG_PRS_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , "Personeller Raporu" );

     DB->COMMIT_TRANSACTION();

}

/**************************************************************************************
                   PRS_PERSONEL_RAPORU_BATCH::CHECK_EXIT
***************************************************************************************/

int PRS_PERSONEL_RAPORU_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}

