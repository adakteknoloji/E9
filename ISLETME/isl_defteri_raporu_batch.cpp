#include "adak_sql.h"
#include "print.h"
#include "adak_utils.h"
#include "isl_defteri_raporu_batch_class.h"
#include "isl_defteri_raporu_batch_open.h"
#include "ui_isl_defteri_raporu_batch.h"
#include "isl_struct.h"
#include "isl_enum.h"
#include "e9_console_utils.h"
#include "e9_log.h"
#include "isl_defteri_raporu_open.h"
#include "e9_gui_utils.h"

extern ADAK_SQL         * DB;

/**************************************************************************************
                   OPEN_ISL_DEFTERI_RAPORU_BATCH
***************************************************************************************/

void OPEN_ISL_DEFTERI_RAPORU_BATCH ( QWidget * p_parent )
{
    ISL_DEFTERI_RAPORU_BATCH * F = new ISL_DEFTERI_RAPORU_BATCH ( p_parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU_BATCH::ISL_DEFTERI_RAPORU_BATCH
***************************************************************************************/

ISL_DEFTERI_RAPORU_BATCH::ISL_DEFTERI_RAPORU_BATCH ( QWidget * p_parent ) :BATCH_KERNEL ( p_parent ), m_ui ( new Ui::ISL_DEFTERI_RAPORU_BATCH )
{
    m_ui->setupUi                       ( this );
    START_BATCH_KERNEL                  ( this, DB);
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU_BATCH::SETUP_FORM
***************************************************************************************/

void ISL_DEFTERI_RAPORU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE                       ("isletme-defteri_isletme-defteri-raporu") ;
    SET_PAGE_TITLE                      ( tr ( "İŞL - İŞLETME DEFTERİ RAPORU" ) );

    REGISTER_BUTTON_WIDGET              ( m_ui->widget_batch_buttons      );

    m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE()        );
    m_ui->adakDate_bts_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE()       );

    SET_NAME_OF_RUN_BATCH_BUTTON        ( tr ( "Rapor Al" )               );
    SET_ENTER_KEY_FOR_RUN_BATCH         ( true                            );    
    SET_SETTING_NAME                    ( "ISL_DEFTERI_RAPORU"            );
    SET_FIRST_FOCUS_WIDGET              ( m_ui->adakDate_bas_tarihi      );

    QStringList rapor_sekli;

    rapor_sekli << QObject::tr("Gelirler/Giderler")<< QObject::tr("Önce Gelirler/Sonra Giderler");

    m_ui->comboBox_rapor_sekli->addItems(rapor_sekli);


}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU_BATCH::CHECK_VAR
***************************************************************************************/

int ISL_DEFTERI_RAPORU_BATCH::CHECK_VAR ( QObject * p_object )
{
    Q_UNUSED( p_object );
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU_BATCH::CHECK_RUN
***************************************************************************************/

int ISL_DEFTERI_RAPORU_BATCH::CHECK_RUN()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarihi, m_ui->adakDate_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU_BATCH::RUN_BATCH
***************************************************************************************/

void ISL_DEFTERI_RAPORU_BATCH::RUN_BATCH()
{
    ISL_RAPOR_VARS * ISL_RV = new ISL_RAPOR_VARS;

    ISL_RV->baslangic_tarihi = m_ui->adakDate_bas_tarihi->QDATE();
    ISL_RV->bitis_tarihi     = m_ui->adakDate_bts_tarihi->QDATE();
    ISL_RV->rapor_sekli      = GET_ISL_DEFTERI_RAPOR_SEKLI_ENUM(m_ui->comboBox_rapor_sekli->currentText());

    DB->START_TRANSACTION();

    QString log_detaylari = QObject::tr("İşletme Defteri Raporu , Tarih Aralığı : ") +
                            m_ui->adakDate_bas_tarihi->QDATE().toString("dd MMMM yyyy") + " - "
                            + m_ui->adakDate_bts_tarihi->QDATE().toString("dd MMMM yyyy");
    E9_LOG_KAYDI_EKLE (ISLETME_MODULU , LOG_ISL_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari);

    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER ( GET_ISL_DEFTERI_RAPORU (ISL_RV), nativeParentWidget() );

    delete ISL_RV;
}

/**************************************************************************************
                   ISL_DEFTERI_RAPORU_BATCH::GET_ISL_DEFTERI_RAPOR_SEKLI_ENUM
***************************************************************************************/

int ISL_DEFTERI_RAPORU_BATCH::GET_ISL_DEFTERI_RAPOR_SEKLI_ENUM(QString rapor_sekli_str)
{
    if ( rapor_sekli_str EQ QObject::tr("Gelirler/Giderler")) {
        return ENUM_GELIRLER_GIDERLER;
    }
    else if ( rapor_sekli_str EQ QObject::tr("Önce Gelirler/Sonra Giderler")) {
        return ENUM_ONCE_GELIRLER_SONRA_GIDERLER;
    }
    return -1;
}








