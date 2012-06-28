#include "fat_hizmet_ekstresi_batch_class.h"
#include "ui_fat_hizmet_ekstresi_batch.h"
#include "stok_enum.h"
#include "print.h"
#include "fat_struct.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "fat_hizmet_ekstresi_raporu_open.h"
#include "stok_urun_arama_open.h"
#include "e9_enum.h"
#include "stok_console_utils.h"
#include "fat_gui_utils.h"
#include "fat_console_utils.h"


extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_FAT_HIZMET_EKSTRESI_BATCH
***************************************************************************************/

void OPEN_FAT_HIZMET_EKSTRESI_BATCH ( QWidget * parent )
{
    FAT_HIZMET_EKSTRESI_BATCH * F = new FAT_HIZMET_EKSTRESI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_BATCH::FAT_HIZMET_EKSTRESI_BATCH
***************************************************************************************/

FAT_HIZMET_EKSTRESI_BATCH::FAT_HIZMET_EKSTRESI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::FAT_HIZMET_EKSTRESI_BATCH )
{
    m_ui->setupUi      ( this );    
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_BATCH::SETUP_FORM
***************************************************************************************/

void FAT_HIZMET_EKSTRESI_BATCH::SETUP_FORM ()
{
    SET_PAGE_TITLE    (tr("FAT - HİZMET EKSTRESİ"));
    SET_HELP_PAGE     ("fatura-islemleri_hizmet-ekstresi");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->frame_sirket_filtresi->setHidden(false);
    }

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );

    SET_FIRST_FOCUS_WIDGET ( m_ui->search_edit_hizmet_kodu );

    SET_ENTER_KEY_FOR_RUN_BATCH(true);

    SET_SETTING_NAME  ("HIZMET_EKSTRESI_BATCH");
    m_ui->adakDatebas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE());
    m_ui->adakDate_bitis_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE());

    m_ui->checkBox_hizmet_kodu->setChecked( true );
    m_ui->frame_hizmet_kodu->setEnabled( true );

    m_ui->frame_hizmet_araligi->setEnabled( false );

    m_ui->frame_fatura_alis_satis_turu->setEnabled( false );

    m_ui->combobox_hizmet_turu->addItems(QStringList() << QObject::tr("Alınan Hizmet") << QObject::tr("Satılan Hizmet") << QObject::tr( "Alınan / Satılan Hizmet") );

    FAT_FILL_ALIS_SATIS_FAT_TURU_COMBO_BOX( m_ui->comboBox_fatura_alis_satis_turu );
    m_ui->comboBox_fatura_alis_satis_turu->setCurrentIndex(-1);
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_BATCH::CHECK_VAR
***************************************************************************************/

int FAT_HIZMET_EKSTRESI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchEdit_bas_hizmet_kodu OR object EQ m_ui->searchEdit_bts_hizmet_kodu OR
         object EQ m_ui->search_edit_hizmet_kodu ) {

        if ( object EQ m_ui->searchEdit_bas_hizmet_kodu ) {
            if ( m_ui->searchEdit_bas_hizmet_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bas_hizmet_ismi->clear();
                return ADAK_OK;
            }
        }
        else if (object EQ m_ui->searchEdit_bts_hizmet_kodu ) {
            if ( m_ui->searchEdit_bts_hizmet_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bts_hizmet_ismi->clear();
                return ADAK_OK;
            }
        }
        else {
            if ( m_ui->search_edit_hizmet_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_hizmet_ismi->clear();
                return ADAK_OK;
            }
        }

        QString urun_kodu;

        if ( object EQ m_ui->searchEdit_bas_hizmet_kodu ) {

            urun_kodu = m_ui->searchEdit_bas_hizmet_kodu->GET_TEXT();
        }
        else if ( object EQ m_ui->searchEdit_bts_hizmet_kodu ) {
            urun_kodu = m_ui->searchEdit_bts_hizmet_kodu->GET_TEXT();
        }
        else {
            urun_kodu = m_ui->search_edit_hizmet_kodu->GET_TEXT();
        }


        int urun_id = STK_GET_URUN_ID(urun_kodu , -1 , 1 , -1);


        if ( urun_id EQ 0 ) {
            if ( object EQ m_ui->searchEdit_bas_hizmet_kodu ) {
                MSG_WARNING(  tr("Girmiş olduğunuz başlangıç hizmet kodu bulunamadı."), m_ui->searchEdit_bas_hizmet_kodu );
                return ADAK_FAIL;

            }
            else if ( object EQ m_ui->searchEdit_bts_hizmet_kodu ) {
                MSG_WARNING(  tr("Girmiş olduğunuz bitiş hizmet kodu bulunamadı."), m_ui->searchEdit_bts_hizmet_kodu );
                return ADAK_FAIL;
            }
            else {
                MSG_WARNING(  tr("Girmiş olduğunuz hizmet kodu bulunamadı."), m_ui->search_edit_hizmet_kodu );

                return ADAK_FAIL;
            }
        }
        QString urun_adi;

        STK_GET_URUN_KODU_ADI(urun_id , &urun_kodu , &urun_adi);

        if ( object EQ m_ui->searchEdit_bas_hizmet_kodu ) {
            m_ui->lineedit_bas_hizmet_ismi->setText(urun_adi);
        }
        else if (object EQ m_ui->searchEdit_bts_hizmet_kodu){
            m_ui->lineedit_bts_hizmet_ismi->setText(urun_adi);
        }
        else {
            m_ui->lineedit_hizmet_ismi->setText(urun_adi);
        }
    }
    else if ( object EQ m_ui->checkBox_hizmet_kodu ) {

        if ( m_ui->checkBox_hizmet_kodu->isChecked() EQ true ) {
            m_ui->frame_hizmet_kodu->setEnabled(true);
            m_ui->checkBox_hizmet_araligi->setCheckState(Qt::Unchecked);
            m_ui->searchEdit_bas_hizmet_kodu->SET_TEXT("");
            m_ui->lineedit_bas_hizmet_ismi->clear();
            m_ui->searchEdit_bts_hizmet_kodu->SET_TEXT("");
            m_ui->lineedit_bts_hizmet_ismi->clear();
            m_ui->frame_hizmet_araligi->setDisabled(true);
            SET_FOCUS( m_ui->search_edit_hizmet_kodu );
        }
        else {
            m_ui->frame_hizmet_kodu->setDisabled(true);
            m_ui->search_edit_hizmet_kodu->SET_TEXT("");
            m_ui->lineedit_hizmet_ismi->clear();
            m_ui->frame_hizmet_araligi->setEnabled(true);
            m_ui->checkBox_hizmet_araligi->setCheckState(Qt::Checked);
        }
    }

    else if ( object EQ m_ui->checkBox_hizmet_araligi ) {

        if ( m_ui->checkBox_hizmet_araligi->isChecked() EQ true ) {
            m_ui->frame_hizmet_araligi->setEnabled(true);
            m_ui->checkBox_hizmet_kodu->setCheckState(Qt::Unchecked);
            m_ui->frame_hizmet_kodu->setDisabled(true);
            m_ui->search_edit_hizmet_kodu->SET_TEXT("");
            m_ui->lineedit_hizmet_ismi->clear();
            SET_FOCUS( m_ui->searchEdit_bas_hizmet_kodu );
        }
        else {
            m_ui->frame_hizmet_kodu->setEnabled(true);
            m_ui->frame_hizmet_araligi->setDisabled(true);
            m_ui->searchEdit_bas_hizmet_kodu->SET_TEXT("");
            m_ui->lineedit_bas_hizmet_ismi->clear();
            m_ui->searchEdit_bts_hizmet_kodu->SET_TEXT("");
            m_ui->lineedit_bts_hizmet_ismi->clear();
            m_ui->checkBox_hizmet_kodu->setCheckState(Qt::Checked);
        }
    }
    else if ( object EQ m_ui->checkBox_tarih_araligi ) {

        if ( m_ui->checkBox_tarih_araligi->isChecked() EQ false ) {
             m_ui->adakDatebas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
             m_ui->adakDate_bitis_tarihi->SET_DATE( MALI_YIL_365_DATE() );
             m_ui->frame_tarih_araligi->setDisabled ( true );
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled ( true );
            SET_FOCUS( m_ui->adakDatebas_tarihi );
        }
    }
    else if ( object EQ m_ui->checkBox_nakli_yekun ) {

        if ( m_ui->checkBox_nakli_yekun->isChecked() EQ false ) {
            m_ui->frame_nakli_yekun->setDisabled ( true );
        }
        else {
            m_ui->frame_nakli_yekun->setEnabled ( true );
        }
    }
    else if ( object EQ m_ui->checkBox_fatura_alis_satis_turu ) {
        if ( m_ui->checkBox_fatura_alis_satis_turu->isChecked() EQ true ) {
            m_ui->frame_fatura_alis_satis_turu->setEnabled(true);
        }
        else {
            m_ui->comboBox_fatura_alis_satis_turu->setCurrentIndex(-1);
            m_ui->frame_fatura_alis_satis_turu->setDisabled(true);
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_BATCH::CHECK_RUN
***************************************************************************************/

int FAT_HIZMET_EKSTRESI_BATCH::CHECK_RUN ()
{

    if ( m_ui->checkBox_hizmet_kodu->isChecked() EQ true ) {
        if ( m_ui->search_edit_hizmet_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Hizmet kodunu seçmelisiniz."), m_ui->search_edit_hizmet_kodu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_hizmet_araligi->isChecked() EQ true ) {
        if ( m_ui->searchEdit_bas_hizmet_kodu->GET_TEXT().isEmpty() EQ true ) {

            MSG_WARNING( tr("Başlangıç hizmet kodunu seçmelisiniz."), m_ui->searchEdit_bas_hizmet_kodu );

            return ADAK_FAIL;
        }
        if ( m_ui->searchEdit_bts_hizmet_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Bitiş hizmet kodunu seçmelisiniz."), m_ui->searchEdit_bts_hizmet_kodu );
            return ADAK_FAIL;
        }
    }
    if ( m_ui->searchEdit_bas_hizmet_kodu->GET_TEXT().isEmpty() EQ false AND
         m_ui->searchEdit_bts_hizmet_kodu->GET_TEXT().isEmpty() EQ false ) {

        if ( m_ui->searchEdit_bas_hizmet_kodu->GET_TEXT() > m_ui->searchEdit_bts_hizmet_kodu->GET_TEXT() ) {
            MSG_WARNING( tr("Başlangıç hizmet kodu , bitiş hizmet kodundan büyük olamaz."), m_ui->searchEdit_bas_hizmet_kodu );
            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_hizmet_araligi->isChecked() EQ false AND m_ui->checkBox_hizmet_kodu->isChecked() EQ false ){
        MSG_WARNING(  tr ("Hizmet kriteri seçmelisiniz."), m_ui->checkBox_hizmet_araligi );
        return ADAK_FAIL;
    }
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDatebas_tarihi, m_ui->adakDate_bitis_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }


    return ADAK_OK;
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_BATCH::RUN_BATCH
***************************************************************************************/

void FAT_HIZMET_EKSTRESI_BATCH::RUN_BATCH ()
{
    FATURA_RAPOR_STRUCT * FAT_RV = new FATURA_RAPOR_STRUCT;

    int fatura_alis_satis_turu    = FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_ENUM(m_ui->comboBox_fatura_alis_satis_turu->currentText());

    FAT_RV->baslangic_tarihi      = m_ui->adakDatebas_tarihi->QDATE();
    FAT_RV->bitis_tarihi          = m_ui->adakDate_bitis_tarihi->QDATE();
    FAT_RV->bas_hizmet_kodu       = m_ui->searchEdit_bas_hizmet_kodu->GET_TEXT();
    FAT_RV->bts_hizmet_kodu       = m_ui->searchEdit_bts_hizmet_kodu->GET_TEXT();
    FAT_RV->tek_hizmet_kodu       = m_ui->search_edit_hizmet_kodu->GET_TEXT();
    FAT_RV->nakli_yekun           = QVariant(m_ui->checkBox_nakli_yekun->isChecked()).toInt();
    FAT_RV->hizmet_turu           = STK_GET_HIZMET_TURU_ENUM(m_ui->combobox_hizmet_turu->currentText());
    FAT_RV->fatura_alis_satis_turu= fatura_alis_satis_turu;

    OPEN_REPORT_SHOWER (GET_FAT_HIZMET_EKSTRESI_RAPORU( QObject::tr("Hizmet Ekstresi") , FAT_RV ), nativeParentWidget() );

    delete FAT_RV;
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_BATCH::CHECK_EXIT
***************************************************************************************/

int FAT_HIZMET_EKSTRESI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_HIZMET_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void FAT_HIZMET_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * , QLineEdit * line_edit)
{
    int hizmet_id = OPEN_STOK_URUN_ARAMA(ENUM_HIZMET_KARTI , -1,"", STK_GET_HIZMET_TURU_ENUM(m_ui->combobox_hizmet_turu->currentText()),this );

    if ( hizmet_id NE -1 ) {

        QString urun_kodu;

        STK_GET_URUN_KODU_ADI(hizmet_id , &urun_kodu);

        line_edit->setText(urun_kodu);
    }
}
