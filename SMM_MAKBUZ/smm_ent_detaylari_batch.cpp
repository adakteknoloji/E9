#include "smm_ent_detaylari_batch_class.h"
#include "ui_smm_ent_detaylari_batch.h"
#include "smm_struct.h"
#include "e9_console_utils.h"
#include "muh_console_utils.h"
#include "sube_console_utils.h"
#include "muh_hesap_arama_open.h"
#include "smm_enum.h"
#include "e9_enum.h"
#include "e9_gui_utils.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_SMM_ENT_DETAYLARI_BATCH
***************************************************************************************/

void OPEN_SMM_ENT_DETAYLARI_BATCH ( SMM_ENT_DETAYLARI * P_ENT_DETAYLARI,QWidget * parent )
{
    SMM_ENT_DETAYLARI_BATCH * F = new SMM_ENT_DETAYLARI_BATCH ( P_ENT_DETAYLARI , parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   SMM_ENT_DETAYLARI_BATCH::SMM_ENT_DETAYLARI_BATCH
***************************************************************************************/

SMM_ENT_DETAYLARI_BATCH::SMM_ENT_DETAYLARI_BATCH ( SMM_ENT_DETAYLARI * P_ENT_DETAYLARI , QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::SMM_ENT_DETAYLARI_BATCH )
{
    m_ui->setupUi      ( this );

    M_ENT_DETAYLARI = P_ENT_DETAYLARI;

    START_BATCH_KERNEL ( this, DB );
}


/**************************************************************************************
                   SMM_ENT_DETAYLARI_BATCH::SETUP_FORM
***************************************************************************************/

void SMM_ENT_DETAYLARI_BATCH::SETUP_FORM ()
{
    SET_PAGE_TITLE    ( tr ( "SMM - ENTEGRASYON DETAYLARI" ) );
    SET_HELP_PAGE     (      "sm_makbuz_ent_detaylari.html" );

    SET_AUTO_EXIT_BATCH             ( true );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Kaydet" );

    SET_FIRST_FOCUS_WIDGET (m_ui->comboBox_sirket );

    SET_SETTING_NAME  ("SMM_ENT_DETAYLARI_BATCH");

    m_ui->commaEdit_muh_fis_no->SET_INTEGER(M_ENT_DETAYLARI->muh_fis_no);
    m_ui->commaEdit_bnk_fis_no->SET_INTEGER(M_ENT_DETAYLARI->bnk_fis_no);
    m_ui->commaEdit_cari_fis_no->SET_INTEGER(M_ENT_DETAYLARI->cari_fis_no);

    m_ui->commaEdit_bnk_fis_no->setDisabled(true);
    m_ui->commaEdit_cari_fis_no->setDisabled(true);
    m_ui->commaEdit_muh_fis_no->setDisabled(true);

    m_ui->lineEdit_muh_hesap_ismi->setReadOnly(true);


    if ( M_ENT_DETAYLARI->tevkifatli_mi EQ 1 ) {
        m_ui->checkBox_tevkifatli_mi->setChecked(true);
    }
    else {
        m_ui->checkBox_tevkifatli_mi->setChecked(false);
        m_ui->frame_tevkifat_detaylari->setHidden(true);
    }

    if ( M_ENT_DETAYLARI->tevkifat_yuzdesi > 0 ) {
        m_ui->commaEdit_tevkifat_yuzdesi->SET_DOUBLE(M_ENT_DETAYLARI->tevkifat_yuzdesi);
    }

    if ( M_ENT_DETAYLARI->muh_kasa_hesap_id EQ 0 ) {
        M_ENT_DETAYLARI->muh_kasa_hesap_id = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();
    }

    if ( M_ENT_DETAYLARI->tahsil_edildi_odendi EQ 1 ) {
        m_ui->checkBox_tahsil_edildi_odendi->setChecked(true);
    }
    else {
        m_ui->checkBox_tahsil_edildi_odendi->setChecked(false);
    }

    if ( M_ENT_DETAYLARI->fis_turu EQ SMM_ALIS_MAKBUZU ) {
        m_ui->checkBox_tahsil_edildi_odendi->setText(tr("Ödendi"));
    }
    else {
        m_ui->checkBox_tahsil_edildi_odendi->setText(tr("Tahsil Edildi"));
    }

    QString hesap_kodu;
    QString hesap_ismi;

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_muh_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_muh_hesap_ismi);

    MUH_GET_HESAP_KODU_HESAP_ISMI(M_ENT_DETAYLARI->muh_kasa_hesap_id , hesap_kodu , hesap_ismi);

    m_ui->searchEdit_muh_hesap_kodu->SET_TEXT(hesap_kodu);
    m_ui->lineEdit_muh_hesap_ismi->setText(hesap_ismi);

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ) {
        m_ui->comboBox_sirket->setHidden ( true );
        m_ui->label_sirket->setHidden(true);
    }

    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_sirket);

    if ( M_ENT_DETAYLARI->muh_fis_no EQ 0 ) {
        m_ui->label_muh_fis_no->setHidden(true);
        m_ui->commaEdit_muh_fis_no->setHidden(true);
    }
    if ( M_ENT_DETAYLARI->bnk_fis_no EQ 0 ) {
        m_ui->label_banka_fis_no->setHidden(true);
        m_ui->commaEdit_bnk_fis_no->setHidden(true);
    }
    if ( M_ENT_DETAYLARI->cari_fis_no EQ 0 ) {
        m_ui->label_cari_fis_no->setHidden(true);
        m_ui->commaEdit_cari_fis_no->setHidden(true);
    }


}

/**************************************************************************************
                   SMM_ENT_DETAYLARI_BATCH::CHECK_VAR
***************************************************************************************/

int SMM_ENT_DETAYLARI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->checkBox_tevkifatli_mi ) {

        if ( m_ui->checkBox_tevkifatli_mi->isChecked() EQ true ) {
            m_ui->frame_tevkifat_detaylari->setVisible(true);
            m_ui->commaEdit_tevkifat_yuzdesi->SET_DOUBLE(50);
        }
        else {
            m_ui->frame_tevkifat_detaylari->setVisible(false);
            m_ui->commaEdit_tevkifat_yuzdesi->clear();
        }
    }

    else if ( object EQ m_ui->searchEdit_muh_hesap_kodu ) {

        if ( m_ui->searchEdit_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_muh_hesap_ismi->clear();
            M_ENT_DETAYLARI->muh_kasa_hesap_id = 0;
            return ADAK_OK;
        }

        M_ENT_DETAYLARI->muh_kasa_hesap_id = MUH_GET_HESAP_ID(m_ui->searchEdit_muh_hesap_kodu->GET_TEXT());

        if ( M_ENT_DETAYLARI->muh_kasa_hesap_id EQ 0 ) {
            MSG_WARNING(QObject::tr("Aradığınız muhasebe hesabı bulunamadı."),m_ui->searchEdit_muh_hesap_kodu);
            return ADAK_FAIL;
        }

        m_ui->lineEdit_muh_hesap_ismi->setText(MUH_GET_HESAP_ISMI(m_ui->searchEdit_muh_hesap_kodu->GET_TEXT()));
    }

    else if ( object EQ m_ui->commaEdit_tevkifat_yuzdesi ) {
        if (m_ui->commaEdit_tevkifat_yuzdesi->GET_DOUBLE() > 100 ) {
            MSG_WARNING(QObject::tr("Tevkifat oranı 100 den büyük olamaz."),m_ui->commaEdit_tevkifat_yuzdesi);
            return ADAK_FAIL;
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   SMM_ENT_DETAYLARI_BATCH::CHECK_RUN
***************************************************************************************/

int SMM_ENT_DETAYLARI_BATCH::CHECK_RUN ()
{
    if ( m_ui->checkBox_tevkifatli_mi->isChecked() EQ true ) {

        if ( m_ui->commaEdit_tevkifat_yuzdesi->GET_DOUBLE() EQ 0 ) {
            MSG_WARNING(QObject::tr("Tevkifat yüzdesini boş bırakamazsınız.") , m_ui->commaEdit_tevkifat_yuzdesi);
            return ADAK_FAIL;
        }

    }

    if ( m_ui->searchEdit_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(QObject::tr("Muhasebe Hesabını boş bırakamazsınız.") , m_ui->searchEdit_muh_hesap_kodu);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMM_ENT_DETAYLARI_BATCH::RUN_BATCH
***************************************************************************************/

void SMM_ENT_DETAYLARI_BATCH::RUN_BATCH ()
{
    M_ENT_DETAYLARI->tevkifatli_mi        = QVariant(m_ui->checkBox_tevkifatli_mi->isChecked()).toInt();
    M_ENT_DETAYLARI->tevkifat_yuzdesi     = m_ui->commaEdit_tevkifat_yuzdesi->GET_DOUBLE();
    M_ENT_DETAYLARI->tahsil_edildi_odendi = QVariant(m_ui->checkBox_tahsil_edildi_odendi->isChecked()).toInt();
}

/**************************************************************************************
                   SMM_ENT_DETAYLARI_BATCH::CHECK_EXIT
***************************************************************************************/

int SMM_ENT_DETAYLARI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_ENT_DETAYLARI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void SMM_ENT_DETAYLARI_BATCH::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    QSearchEdit * searchedit = ( QSearchEdit * ) widget;

    int muh_hesap_id = OPEN_MUH_HESAP_ARAMA(searchedit->GET_TEXT(),this,true);

    if ( muh_hesap_id <= 0 ) {
        return;
    }

    QString hesap_kodu;
    QString hesap_ismi;

    MUH_GET_HESAP_KODU_HESAP_ISMI(muh_hesap_id , hesap_kodu , hesap_ismi);

    line_edit->setText(hesap_ismi);
}









