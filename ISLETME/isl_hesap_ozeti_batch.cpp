#include "isl_hesap_ozeti_batch_open.h"
#include "isl_hesap_ozeti_batch_class.h"
#include "isl_hesap_ozeti_raporu_open.h"
#include "ui_isl_hesap_ozeti_batch.h"
#include "print.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "isl_struct.h"
#include "e9_log.h"
#include "sube_console_utils.h"
#include "sube_arama_open.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_ISL_HESAP_OZETI_BATCH
***************************************************************************************/

void OPEN_ISL_HESAP_OZETI_BATCH ( QWidget * parent )
{
    ISL_HESAP_OZETI_BATCH * F = new ISL_HESAP_OZETI_BATCH ( parent );
    F->EXEC(NOT_FULL_SCREEN);
}

/**************************************************************************************
                   ISL_HESAP_OZETI_BATCH::ISL_HESAP_OZETI_BATCH
***************************************************************************************/

ISL_HESAP_OZETI_BATCH::ISL_HESAP_OZETI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::ISL_HESAP_OZETI_BATCH )
{
    m_ui->setupUi      ( this );
    m_sube_id = -1;
    START_BATCH_KERNEL ( this, DB );
}


/**************************************************************************************
                   ISL_HESAP_OZETI_BATCH::SETUP_FORM
***************************************************************************************/

void ISL_HESAP_OZETI_BATCH::SETUP_FORM ()
{
    SET_PAGE_TITLE    ( tr( "İŞL - İŞLETME HESAP ÖZETİ" ) );
    SET_HELP_PAGE     ( "isletme-defteri_isletme-hesap-ozeti"  );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->adakDate_bas_tarihi );
    SET_ENTER_KEY_FOR_RUN_BATCH  ( true );
    SET_SETTING_NAME             ( "ISL_HESAP_OZETI_BATCH" );

    m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bts_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE() );

    int sube_id = SUBE_GET_SUBE_ID();
    QString sube_kodu, sube_adi;
    SUBE_GET_SUBE_KODU_ADI( sube_id , &sube_kodu, &sube_adi );
    m_ui->searchEdit_sube_kodu->SET_TEXT(sube_kodu);
    m_ui->lineEdit_sube_ismi->setText(sube_adi);
}

/**************************************************************************************
                   ISL_HESAP_OZETI_BATCH::CHECK_VAR
***************************************************************************************/

int ISL_HESAP_OZETI_BATCH::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->searchEdit_sube_kodu ) {

        if ( m_ui->searchEdit_sube_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_sube_id = -1;
            m_ui->lineEdit_sube_ismi->clear();
            return ADAK_OK;
        }

        int     sube_id;
        QString sube_adi;

        int result = SUBE_GET_SUBE_ID_ADI(m_ui->searchEdit_sube_kodu->GET_TEXT() , &sube_id,&sube_adi);

        if ( result EQ 0 ) {

            MSG_WARNING(tr("Aradığınız şube kodu bulunamadı.") , m_ui->searchEdit_sube_kodu);
            return ADAK_FAIL;
        }
        m_sube_id = sube_id;
        m_ui->lineEdit_sube_ismi->setText(sube_adi);
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_HESAP_OZETI_BATCH::CHECK_RUN
***************************************************************************************/

int ISL_HESAP_OZETI_BATCH::CHECK_RUN ()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarihi, m_ui->adakDate_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_sube_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Bir Şube seçiniz.!", m_ui->searchEdit_sube_kodu );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_HESAP_OZETI_BATCH::RUN_BATCH
***************************************************************************************/

void ISL_HESAP_OZETI_BATCH::RUN_BATCH ()
{
    ISL_RAPOR_VARS * ISL_RV = new ISL_RAPOR_VARS;

    ISL_RV->baslangic_tarihi = m_ui->adakDate_bas_tarihi->QDATE();
    ISL_RV->bitis_tarihi     = m_ui->adakDate_bts_tarihi->QDATE();
    ISL_RV->sube_id          = m_sube_id;

    DB->START_TRANSACTION();

    QString log_detaylari = QObject::tr("İşletme Hesap Özeti , Tarih Aralığı : ") +
                            m_ui->adakDate_bas_tarihi->QDATE().toString("dd MMMM yyyy") + " - "
                            + m_ui->adakDate_bts_tarihi->QDATE().toString("dd MMMM yyyy");
    E9_LOG_KAYDI_EKLE (ISLETME_MODULU , LOG_ISL_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari);

    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER ( GET_ISL_HESAP_OZETI_RAPORU(ISL_RV), nativeParentWidget() );
}

/**************************************************************************************
                   ISL_HESAP_OZETI_BATCH::CHECK_EXIT
***************************************************************************************/

int ISL_HESAP_OZETI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_HESAP_OZETI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void ISL_HESAP_OZETI_BATCH::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    Q_UNUSED ( widget );

    int sube_id = OPEN_SUBE_ARAMA("" , this);

    if ( sube_id > 0 ) {
        QString sube_kodu;
        SUBE_GET_SUBE_KODU_ADI(sube_id , &sube_kodu);
        line_edit->setText(sube_kodu);
    }

}













