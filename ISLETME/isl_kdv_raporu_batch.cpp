#include "adak_utils.h"
#include "adak_sql.h"
#include "print.h"
#include "isl_kdv_raporu_batch_class.h"
#include "isl_kdv_raporu_batch_open.h"
#include "ui_isl_kdv_raporu_batch.h"
#include "isl_struct.h"
#include "e9_console_utils.h"
#include "e9_log.h"
#include "isl_kdv_raporu_open.h"
#include "e9_gui_utils.h"

extern ADAK_SQL         * DB;

/**************************************************************************************
                   OPEN_ISL_KDV_RAPORU_BATCH
**************************************************************************************/

void OPEN_ISL_KDV_RAPORU_BATCH ( QWidget * parent )
{
    ISL_KDV_RAPORU_BATCH * R = new ISL_KDV_RAPORU_BATCH ( parent );
    R->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   ISL_KDV_RAPORU_BATCH::ISL_KDV_RAPORU_BATCH
**************************************************************************************/

ISL_KDV_RAPORU_BATCH::ISL_KDV_RAPORU_BATCH ( QWidget * p_parent ) :  BATCH_KERNEL ( p_parent ), m_ui ( new Ui::ISL_KDV_RAPORU_BATCH )
{
    m_ui->setupUi                       ( this );
    START_BATCH_KERNEL                  ( this, DB );
}

/**************************************************************************************
                   ISL_KDV_RAPORU_BATCH::CHECK_VAR
***************************************************************************************/

void ISL_KDV_RAPORU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE                       ("isletme-defteri_kdv-raporu");

    m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bts_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE() );

    SET_NAME_OF_RUN_BATCH_BUTTON        ( tr ( "Rapor Al" ) );

    SET_PAGE_TITLE                      ( tr ( "İŞL - KDV RAPORU" ) );

    SET_ENTER_KEY_FOR_RUN_BATCH         ( true );
    REGISTER_BUTTON_WIDGET              ( m_ui->widget_batch_buttons );

    SET_FIRST_FOCUS_WIDGET              ( m_ui->adakDate_bas_tarihi);

    SET_SETTING_NAME                    ( "ISL_KDV_RAPORU_BATCH" );
}

/**************************************************************************************
                   ISL_KDV_RAPORU_BATCH::CHECK_VAR
***************************************************************************************/

int ISL_KDV_RAPORU_BATCH::CHECK_VAR ( QObject * p_object )
{
    Q_UNUSED( p_object );
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_KDV_RAPORU_BATCH::CHECK_RUN
***************************************************************************************/

int ISL_KDV_RAPORU_BATCH::CHECK_RUN()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarihi, m_ui->adakDate_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_KDV_RAPORU_BATCH::RUN_BATCH
***************************************************************************************/

void ISL_KDV_RAPORU_BATCH::RUN_BATCH()
{
    ISL_RAPOR_VARS * ISL_RV = new ISL_RAPOR_VARS;

    ISL_RV->baslangic_tarihi = m_ui->adakDate_bas_tarihi->QDATE();
    ISL_RV->bitis_tarihi     = m_ui->adakDate_bts_tarihi->QDATE();

    DB->START_TRANSACTION();

    QString log_detaylari = QObject::tr("KDV Oranları Raporu , Tarih Aralığı : ") +
                            m_ui->adakDate_bas_tarihi->QDATE().toString("dd MMMM yyyy") + " - "
                            + m_ui->adakDate_bts_tarihi->QDATE().toString("dd MMMM yyyy");
    E9_LOG_KAYDI_EKLE (ISLETME_MODULU , LOG_ISL_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari);

    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER      ( GET_ISL_KDV_RAPORU(ISL_RV), nativeParentWidget() );

    delete ISL_RV;
}

