#include "muh_bilanco_raporu_batch_class.h"
#include "muh_bilanco_raporu_batch_open.h"
#include "ui_muh_bilanco_raporu_batch.h"
#include "print.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_enum.h"
#include "e9_log.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "muh_bilanco_raporu_open.h"

extern ADAK_SQL *    DB;

/**************************************************************************************
                   OPEN_MUH_BILANCO_RAPORU_BATCH
***************************************************************************************/

void OPEN_MUH_BILANCO_RAPORU_BATCH ( QWidget * parent )
{
    MUH_BILANCO_RAPORU_BATCH * B = new MUH_BILANCO_RAPORU_BATCH ( parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   MUH_BILANCO_RAPORU_BATCH::MUH_BILANCO_RAPORU_BATCH
***************************************************************************************/

MUH_BILANCO_RAPORU_BATCH::MUH_BILANCO_RAPORU_BATCH ( QWidget *parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::MUH_BILANCO_RAPORU_BATCH )
{
    m_ui->setupUi       ( this );
    START_BATCH_KERNEL  ( this, DB );
}

/**************************************************************************************
                   MUH_BILANCO_RAPORU_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_BILANCO_RAPORU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE   ( "genel-muhasebe_bilanco" );
    SET_PAGE_TITLE  ( tr ( "MUH - BİLANÇO RAPORU" ) );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );

    m_ui->adakDate_bas_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
    m_ui->adakDate_bitis_tarihi->SET_DATE(MALI_ARRAY_LAST_DATE());

    SET_SETTING_NAME       ( "MUH_BILANCO_RAPORU_BATCH" );
    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_bas_tarihi );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->comboBox_sirket->setVisible(true);
        m_ui->label_sirket->setVisible(true);
    }


    m_ui->checkBox_bakiyesiz_hesaplar->setChecked(true);
}

/**************************************************************************************
                   MUH_BILANCO_RAPORU_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_BILANCO_RAPORU_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED( object );
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_BILANCO_RAPORU_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_BILANCO_RAPORU_BATCH::CHECK_RUN()
{

    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarihi, m_ui->adakDate_bitis_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_BILANCO_RAPORU_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_BILANCO_RAPORU_BATCH::RUN_BATCH()
{
    MUH_RAPOR_VARS * MUH_RV   = new MUH_RAPOR_VARS;

    MUH_RV->baslangic_tarihi   = m_ui->adakDate_bas_tarihi->QDATE();
    MUH_RV->bitis_tarihi       = m_ui->adakDate_bitis_tarihi->QDATE();
    MUH_RV->bakiyesiz_hesaplar = QVariant(m_ui->checkBox_bakiyesiz_hesaplar->isChecked()).toInt();

    QString log_detaylari = QObject::tr ( "Bilanço Raporu , Tarih Aralığı : " ) +  m_ui->adakDate_bas_tarihi->QDATE().toString("dd MMMM yyyy") + " - " +
                            m_ui->adakDate_bitis_tarihi->QDATE().toString("dd MMMM yyyy");

    DB->START_TRANSACTION();
    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_RAPORLAR ,LOG_ISLEM_RAPOR_ALINDI, log_detaylari );
    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER ( GET_BILANCO_RAPORU(MUH_RV),nativeParentWidget() );

    delete MUH_RV;
}
