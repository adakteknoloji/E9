#include "prs_ihbar_ve_kidem_tazminati_batch_class.h"
#include "prs_ihbar_ve_kidem_tazminati_batch_open.h"
#include "print.h"
#include "prs_isten_ayrilma_raporu_open.h"
#include "e9_gui_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                  SHOW_PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH
***************************************************************************************/
void SHOW_PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH (QWidget * parent)
{
    PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH * B = new PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH(parent);
    B->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
            PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH
***************************************************************************************/

PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH(QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH)
{
    m_ui->setupUi       (this);
    START_BATCH_KERNEL  (this, DB );
}

/**************************************************************************************
           PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::SETUP_FORM
**************************************************************************************/

void PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ("personel-bordrosu_ihbar-ve-kidem-tazminati-raporu");
    SET_PAGE_TITLE      ( tr ( "PRS - İHBAR VE KIDEM TAZMİNATI RAPORU" ) );

    REGISTER_BUTTON_WIDGET  ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME             ( "PRS_IHB_VE_KDM_TAZMINATI_BATCH" );
    SET_NAME_OF_RUN_BATCH_BUTTON ( "TAZMİNAT RAPORU AL" );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->adakDate_bas_tarihi );

    m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bts_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE());

}

/**************************************************************************************
           PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::CHECK_VAR
***************************************************************************************/

int PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::CHECK_VAR(QObject * object)
{
    Q_UNUSED(object);
    return ADAK_OK;
}

/**************************************************************************************
          PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::CHECK_RUN
***************************************************************************************/

int PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::CHECK_RUN()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarihi, m_ui->adakDate_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
          PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::RUN_BATCH
***************************************************************************************/

void PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::RUN_BATCH()
{
    SQL_QUERY query( DB );
    query.PREPARE_SELECT( "prs_isten_ayrilanlar", "personel_id" );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING( tr( "İşten Ayrılan Personel Bulunmamaktadır..." ), NULL );
        return;
    }

    OPEN_REPORT_SHOWER( SHOW_ISTEN_AYRILMA_RAPORU (  1,  1 ),nativeParentWidget());
}

/**************************************************************************************
         PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::CHECK_EXIT
**************************************************************************************/

int PRS_IHBAR_VE_KIDEM_TAZMINATI_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}
