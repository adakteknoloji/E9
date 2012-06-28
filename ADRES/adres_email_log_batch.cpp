#include "ui_adres_email_log_batch.h"
#include "print.h"
#include "adres_email_raporu_open.h"
#include "adres_email_raporu_class.h"
#include "adres_email_log_batch_class.h"
#include "adres_email_log_batch_open.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_ADRES_EMAIL_LOG_BATCH
***************************************************************************************/

void OPEN_ADRES_EMAIL_LOG_BATCH ( QWidget * parent )
{
    ADRES_EMAIL_LOG_BATCH * F = new ADRES_EMAIL_LOG_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   ADRES_EMAIL_LOG_BATCH::ADRES_EMAIL_LOG_BATCH
***************************************************************************************/

ADRES_EMAIL_LOG_BATCH::ADRES_EMAIL_LOG_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::ADRES_EMAIL_LOG_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   ADRES_EMAIL_LOG_BATCH::SETUP_FORM
***************************************************************************************/

void ADRES_EMAIL_LOG_BATCH::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );

    REGISTER_BUTTON_WIDGET ( m_ui->search_widget);
    SET_PAGE_TITLE    ( tr ( "ADR - E-MAIL LOG KAYITLARI" ) );
    SET_HELP_PAGE     ("email-log-kayitlari");
    SET_FIRST_FOCUS_WIDGET ( m_ui->adak_date_bas );

    SET_SETTING_NAME  ("EMAIL_LOG_KAYITLARI");    

    m_ui->adak_date_bts->SET_DATE (QDate::currentDate().addDays(1));

    m_ui->line_edit_email_adresi->setMaxLength(50);
}

/**************************************************************************************
                   ADRES_EMAIL_LOG_BATCH::CHECK_VAR
***************************************************************************************/

int ADRES_EMAIL_LOG_BATCH::CHECK_VAR ( QObject *  object )
{
    if ( object EQ m_ui->check_box_email_adresi ) {
        if ( m_ui->check_box_email_adresi->isChecked() EQ true ) {
            m_ui->frame_email_adresi->setEnabled(true);
        }
        else {
            m_ui->frame_email_adresi->setDisabled(true);
            m_ui->line_edit_email_adresi->clear();
        }
    }

    else if ( object EQ m_ui->check_box_kullanici_adi ) {
        if ( m_ui->check_box_kullanici_adi->isChecked() EQ true ) {
            m_ui->frame_kullanici_adi->setEnabled(true);
        }
        else {
            m_ui->frame_kullanici_adi->setDisabled(true);
            m_ui->line_edit_kullanici_adi->clear();
        }
    }

    else if ( object EQ m_ui->check_box_tarih_araligi ) {
        if ( m_ui->check_box_tarih_araligi->isChecked() EQ true ) {
            m_ui->frame_tarih_araligi->setEnabled(true);
        }
        else {
            m_ui->frame_tarih_araligi->setDisabled(true);

            m_ui->adak_date_bas->SET_DATE(QDate::currentDate());
            m_ui->adak_date_bts->SET_DATE(QDate::currentDate());

            m_ui->adak_date_bts->SET_DATE (QDate::currentDate().addDays(1));
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_EMAIL_LOG_BATCH::CHECK_RUN
***************************************************************************************/

int ADRES_EMAIL_LOG_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_EMAIL_LOG_BATCH::RUN_BATCH
***************************************************************************************/

void ADRES_EMAIL_LOG_BATCH::RUN_BATCH ()
{
    EMAIL_RAPOR_STRUCT email_report;

    email_report.bas_tarihi     = m_ui->adak_date_bas->DATE();
    email_report.bts_tarihi     = m_ui->adak_date_bts->DATE();
    email_report.kullanici_adi  = m_ui->line_edit_kullanici_adi->text();
    email_report.email_adresi   = m_ui->line_edit_email_adresi->text();

    OPEN_REPORT_SHOWER ( GET_EMAIL_LOG_RAPORU(&email_report) , nativeParentWidget() );
}
