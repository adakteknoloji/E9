#include "irs_irsaliye_listesi_batch_class.h"
#include "irs_irsaliye_listesi_batch_open.h"
#include "ui_irs_irsaliye_listesi_batch.h"
#include "print.h"
#include "cari_console_utils.h"
#include "irs_struct.h"
#include "sube_console_utils.h"
#include "e9_console_utils.h"
#include "cari_kart_arama_open.h"
#include "sube_depo_secimi_open.h"
#include "sube_depo_unite_open.h"
#include "irs_irsaliye_listesi_raporu_open.h"
#include "irs_console_utils.h"
#include "sube_enum.h"
#include "irs_gui_utils.h"
#include "e9_gui_utils.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_IRS_IRSALIYE_LISTESI_BATCH
***************************************************************************************/

void OPEN_IRS_IRSALIYE_LISTESI_BATCH ( QWidget * parent )
{
    IRS_IRSALIYE_LISTESI_BATCH * F = new IRS_IRSALIYE_LISTESI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_BATCH::IRS_IRSALIYE_LISTESI_BATCH
***************************************************************************************/

IRS_IRSALIYE_LISTESI_BATCH::IRS_IRSALIYE_LISTESI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::IRS_IRSALIYE_LISTESI_BATCH )
{
    m_ui->setupUi       (this );
    START_BATCH_KERNEL  (this, DB );
}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_BATCH::SETUP_FORM
***************************************************************************************/

void IRS_IRSALIYE_LISTESI_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "İRS - İRSALİYE LİSTESİ" ) );
    SET_HELP_PAGE     ("irsaliye-listesi");

    REGISTER_BUTTON_WIDGET       ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME             ("IRS_IRSALIYE_LISTESI_BATCH");
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->adak_date_bas_tarihi );



    IRS_FILL_ALIS_SATIS_COMBO_BOX   ( m_ui->comboBox_irsaliye_durumu);

    IRS_FILL_FATURA_DURUMU_COMBO_BOX( m_ui->comboBox_fatura_durumu);

    m_ui->checkBox_tarih_araligi->setChecked( true );
    m_ui->frame_tarih_araligi->setEnabled( true );
    m_ui->adak_date_bas_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
    m_ui->adak_date_bts_tarihi->SET_DATE(MALI_ARRAY_LAST_DATE());
    m_ui->frame_fatura_durumu->setDisabled(true);
    m_ui->frame_irsaliye_durumu->setDisabled(true);
    m_ui->frame_cari_hesap->setDisabled(true);
    m_ui->frame_depo_kodu->setDisabled(true);

    m_cari_hesap_id = -1;

    m_depo_id  = SUBE_GET_UNITENIN_DEPOSU();

    if ( SUBE_GET_DEPO_ISLEM_YETKISI() EQ SADECE_UNITE_DEPOSUNDA ) {
        m_ui->frame_depo_kodu->setHidden(true);
        m_ui->checkBox_depo_kodu->setHidden(true);
    }
    else {
        QString depo_kodu;
        QString depo_adi;

        SUBE_GET_DEPO_KODU_ADI(m_depo_id , &depo_kodu , &depo_adi);

        m_ui->searchEdit_depo_kodu->SET_TEXT(depo_kodu);
        m_ui->lineEdit_depo_ismi->setText(depo_adi);
        m_ui->checkBox_depo_kodu->setCheckState(Qt::Checked);
        m_ui->checkBox_depo_kodu->setDisabled(true);
    }
}


/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_BATCH::CHECK_VAR
***************************************************************************************/

int IRS_IRSALIYE_LISTESI_BATCH::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->searchEdit_cari_hesap_kodu ) {

        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_cari_hesap_ismi->clear();
            return ADAK_OK;
        }

        QString cari_hesap_ismi;
        int     cari_hesap_id;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi );

        if ( result EQ 0 ) {
            MSG_WARNING( tr("Aradığınız cari hesap kodu bulunamadı."), m_ui->searchEdit_cari_hesap_kodu );
            return ADAK_FAIL;
        }
        m_ui->lineEdit_cari_hesap_ismi->setText( cari_hesap_ismi );
        m_cari_hesap_id = cari_hesap_id;
    }
    else if (object EQ m_ui->checkBox_tarih_araligi) {
        if ( m_ui->checkBox_tarih_araligi->isChecked() EQ false ) {
            m_ui->frame_tarih_araligi->setDisabled(true);
            m_ui->adak_date_bas_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
            m_ui->adak_date_bts_tarihi->SET_DATE(MALI_YIL_365_DATE());
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled(true);
            SET_FOCUS( m_ui->adak_date_bas_tarihi );
            SET_FOCUS( m_ui->adak_date_bas_tarihi );

        }
    }
    else if (object EQ m_ui->checkBox_cari_hesap ) {
        if ( m_ui->checkBox_cari_hesap->isChecked() EQ true ) {
            m_ui->frame_cari_hesap->setEnabled(true);
            SET_FOCUS( m_ui->searchEdit_cari_hesap_kodu );
        }
        else {
            m_ui->frame_cari_hesap->setDisabled(true);
            m_ui->searchEdit_cari_hesap_kodu->SET_TEXT("");
            m_ui->lineEdit_cari_hesap_ismi->clear();
            m_cari_hesap_id = -1;
        }
    }
    if ( object EQ m_ui->checkBox_fatura_durumu ) {
        if ( m_ui->checkBox_fatura_durumu->isChecked() EQ true ) {
            m_ui->frame_fatura_durumu->setEnabled(true);
            SET_FOCUS( m_ui->comboBox_fatura_durumu );
        }
        else {
            m_ui->frame_fatura_durumu->setDisabled(true);
            m_ui->comboBox_fatura_durumu->setCurrentIndex(-1);
        }
    }
    else if ( object EQ m_ui->searchEdit_depo_kodu ) {
        if ( m_ui->searchEdit_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_depo_ismi->clear();
            m_depo_id = -1;
            return ADAK_OK;
        }

        QString depo_adi;
        int     depo_id;

        int result = SUBE_GET_DEPO_ID_ADI( m_ui->searchEdit_depo_kodu->GET_TEXT(), &depo_id, &depo_adi );

        if ( result EQ 0 ) {
            MSG_WARNING( tr("Aradığınız depo kodu bulunamadı."), m_ui->searchEdit_depo_kodu );
            return ADAK_FAIL;
        }
        m_ui->lineEdit_depo_ismi->setText( depo_adi );
        m_depo_id = depo_id;
    }
    else if ( object EQ m_ui->checkBox_depo_kodu ) {
        if ( m_ui->checkBox_depo_kodu->isChecked() EQ false ) {
            m_ui->frame_depo_kodu->setDisabled(true);
            m_ui->searchEdit_depo_kodu->SET_TEXT(" ");
            m_ui->lineEdit_depo_ismi->clear();
        }
        else {
            m_ui->frame_depo_kodu->setEnabled(true);
            SET_FOCUS( m_ui->searchEdit_depo_kodu );

        }
    }

    if ( object EQ m_ui->checkBox_irsaliye_durumu ) {
        if ( m_ui->checkBox_irsaliye_durumu->isChecked() EQ true ) {
            m_ui->frame_irsaliye_durumu->setEnabled( true );
            SET_FOCUS( m_ui->comboBox_irsaliye_durumu );
        }
        else {
            m_ui->frame_irsaliye_durumu->setDisabled( true );
            m_ui->comboBox_irsaliye_durumu->setCurrentIndex( -1 );
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_BATCH::CHECK_RUN
***************************************************************************************/

int IRS_IRSALIYE_LISTESI_BATCH::CHECK_RUN ()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adak_date_bas_tarihi, m_ui->adak_date_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    if ( m_ui->checkBox_cari_hesap->isChecked() EQ true ) {
        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Cari hesap kodunu boş bırakamazsınız."), m_ui->searchEdit_cari_hesap_kodu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_fatura_durumu->isChecked()  EQ true ) {
        if ( m_ui->comboBox_fatura_durumu->currentIndex() EQ -1 ) {
            MSG_WARNING(  tr("Fatura durumunu seçmelisiniz."), m_ui->comboBox_fatura_durumu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_depo_kodu->isChecked() EQ true ) {
        if ( m_ui->searchEdit_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr ("Depo kodunu boş bırakamazsınız."), m_ui->searchEdit_depo_kodu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_irsaliye_durumu->isChecked() EQ true ) {
        if ( m_ui->comboBox_irsaliye_durumu->currentIndex() EQ -1 ) {
            MSG_WARNING( tr("İrsaliye durumunu seçmelisiniz."), m_ui->comboBox_irsaliye_durumu);

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_BATCH::RUN_BATCH
***************************************************************************************/

void IRS_IRSALIYE_LISTESI_BATCH::RUN_BATCH ()
{
    IRSALIYE_RAPOR_VARS * IRS_RV = new IRSALIYE_RAPOR_VARS;

    IRS_RV->baslangic_tarihi = m_ui->adak_date_bas_tarihi->QDATE();
    IRS_RV->bitis_tarihi     = m_ui->adak_date_bts_tarihi->QDATE();
    IRS_RV->cari_hesap_id    = m_cari_hesap_id;
    IRS_RV->fatura_durumu    = IRS_GET_FATURA_DURUMU_ENUM( m_ui->comboBox_fatura_durumu->currentText() );
    IRS_RV->irsaliye_durumu  = IRS_GET_ALIS_SATIS_ENUM   ( m_ui->comboBox_irsaliye_durumu->currentText() );
    IRS_RV->depo_id          = m_depo_id;

    OPEN_REPORT_SHOWER ( GET_IRS_IRSALIYE_LISTESI_RAPORU( IRS_RV ) , nativeParentWidget() );

    delete IRS_RV;
}

/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_BATCH::CHECK_EXIT
***************************************************************************************/

int IRS_IRSALIYE_LISTESI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}


/**************************************************************************************
                   IRS_IRSALIYE_LISTESI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void IRS_IRSALIYE_LISTESI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget , QLineEdit * line_edit)
{

    if ( widget EQ m_ui->searchEdit_cari_hesap_kodu ) {
        int cari_hesap_id = OPEN_CARI_KART_ARAMA( line_edit->text(), SAHIS_ARTI_FIRMA , this,true, E9_PROGRAMI, CARI_MODULU );

        if ( cari_hesap_id NE -1 ) {
            line_edit->setText(CARI_FIND_HESAP_KODU(cari_hesap_id));
            m_cari_hesap_id = cari_hesap_id;
        }
    }
    else {
        int depo_id = SUBE_DEPO_SEC ( SUBE_GET_SUBE_ID(),this );

        if ( depo_id NE -1 ) {
            line_edit->setText(SUBE_GET_DEPO_KODU(depo_id));
            m_depo_id = depo_id;
        }
    }
}
