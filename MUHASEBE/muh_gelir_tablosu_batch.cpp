#include <adak_utils.h>
#include "muh_gelir_tablosu_batch_class.h"
#include "muh_gelir_tablosu_batch_open.h"
#include "ui_muh_gelir_tablosu_batch.h"
#include "print.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_enum.h"
#include "e9_log.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "muh_gelir_tablosu_raporu_open.h"

extern ADAK_SQL *          DB;

/**************************************************************************************
                   OPEN_MUH_GELIR_TABLOSU_BATCH
***************************************************************************************/

void OPEN_MUH_GELIR_TABLOSU_BATCH ( QWidget * parent  )
{
    MUH_GELIR_TABLOSU_BATCH * B = new MUH_GELIR_TABLOSU_BATCH ( parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   MUH_GELIR_TABLOSU_BATCH::MUH_GELIR_TABLOSU_BATCH
***************************************************************************************/

MUH_GELIR_TABLOSU_BATCH::MUH_GELIR_TABLOSU_BATCH ( QWidget *parent ) :  BATCH_KERNEL ( parent ),
m_ui ( new Ui::MUH_GELIR_TABLOSU_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this,DB );
}

/**************************************************************************************
                   MUH_GELIR_TABLOSU_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_GELIR_TABLOSU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ("genel-muhasebe_gelir-tablosu" );
    SET_PAGE_TITLE      ( tr ( "MUH - GELİR TABLOSU" ) );
    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME    ( "MUH_GELIR_TABLOSU_BATCH" );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );

    SET_ENTER_KEY_FOR_RUN_BATCH(true);

    SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_baslangic_tarihi );

    m_ui->adakDate_baslangic_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bitis_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE() );


    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->comboBox_sirket_turu->setVisible(true);
        m_ui->label_sirket->setVisible(true);
    }

    m_ui->checkBox_bakiyesiz_hesaplar->setChecked(true);

}

/**************************************************************************************
                   MUH_GELIR_TABLOSU_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_GELIR_TABLOSU_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED( object );
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_GELIR_TABLOSU_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_GELIR_TABLOSU_BATCH::CHECK_RUN()
{

    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_baslangic_tarihi, m_ui->adakDate_bitis_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_GELIR_TABLOSU_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_GELIR_TABLOSU_BATCH::RUN_BATCH()
{
    MUH_RAPOR_VARS * MUH_RV   = new MUH_RAPOR_VARS;

    MUH_RV->baslangic_tarihi  = m_ui->adakDate_baslangic_tarihi->QDATE();
    MUH_RV->bitis_tarihi      = m_ui->adakDate_bitis_tarihi->QDATE();
    MUH_RV->bakiyesiz_hesaplar= QVariant(m_ui->checkBox_bakiyesiz_hesaplar->isChecked()).toInt();

    QString log_detaylari = QObject::tr ( "Gelir Tablosu , \n" ) +
                            QObject::tr ( "Tarih Aralığı    : " ) +
                            m_ui->adakDate_baslangic_tarihi->QDATE().toString("dd MMMM yyyy") + " - " +
                            m_ui->adakDate_bitis_tarihi->QDATE().toString("dd MMMM yyyy") ;

    DB->START_TRANSACTION();
    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU, LOG_MUH_RAPORLAR, LOG_ISLEM_RAPOR_ALINDI  , log_detaylari );
    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER ( GET_GELIR_TABLOSU_RAPORU(MUH_RV),nativeParentWidget() );

    delete MUH_RV;
}
