#include "fat_ba_bs_formu_listesi_batch_class.h"
#include "ui_fat_ba_bs_formu_listesi_batch.h"
#include "print.h"
#include "cari_console_utils.h"
#include "sube_console_utils.h"
#include "fat_console_utils.h"
#include "adak_std_utils.h"
#include "fat_ba_bs_formu_listesi_raporu_class.h"
#include "faiz_console_utils.h"
#include "fat_struct.h"
#include "e9_gui_utils.h"
#include "cari_kart_arama_open.h"
#include "sube_depo_secimi_open.h"
#include "e9_console_utils.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_FAT_BA_BS_FORMU_LISTESI_BATCH
***************************************************************************************/

void OPEN_FAT_BA_BS_FORMU_LISTESI_BATCH ( QWidget * parent )
{
    FAT_BA_BS_FORMU_LISTESI_BATCH * F = new FAT_BA_BS_FORMU_LISTESI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_BATCH::FAT_BA_BS_FORMU_LISTESI_BATCH
***************************************************************************************/

FAT_BA_BS_FORMU_LISTESI_BATCH::FAT_BA_BS_FORMU_LISTESI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::FAT_BA_BS_FORMU_LISTESI_BATCH )
{
    m_ui->setupUi       (this );
    START_BATCH_KERNEL  (this, DB );
}

/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_BATCH::SETUP_FORM
***************************************************************************************/

void FAT_BA_BS_FORMU_LISTESI_BATCH::SETUP_FORM()
{


    SET_PAGE_TITLE    (tr("FAT - BA-BS FORMU LİSTESİ"));
    SET_SETTING_NAME  ("FAT_BA_BS_FORMU_LISTESI_BATCH");
    SET_HELP_PAGE     ("fatura-islemleri_ba-bs-formlari-listesi");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );

    SET_FIRST_FOCUS_WIDGET ( m_ui->comboBox_ay  );


    m_ui->checkBox_tarih_araligi->setChecked( true );
    m_ui->frame_tarih_araligi->setEnabled( true );

    m_ui->checkBox_fatura_alis_satis_turu->setChecked( true );
    m_ui->frame_fatura_alis_satis_turu->setEnabled( true );

    E9_FILL_COMBOBOX_AYLAR( m_ui->comboBox_ay );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_ay, E9_GET_AYLAR_STRING( QDate::currentDate().month() ) );

    m_ui->commaEdit_yil->SET_PRECISION( 0 );
    m_ui->commaEdit_yil->VIRGULSUZ_CALIS();
    m_ui->commaEdit_yil->SET_INTEGER( MALI_YIL() );

    m_ui->frame_cari_hesap->setDisabled( true );

    m_ui->frame_tutar_araligi->setDisabled(true);

    m_ui->comboBox_fatura_alis_satis_turu->addItems(QStringList() << QObject::tr("BA Formu")
                                                    << QObject::tr("BS Formu"));
    m_ui->comboBox_fatura_alis_satis_turu->setCurrentIndex( 0 );


    m_cari_hesap_id = -1;
    m_depo_id       = -1;
}


/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_BATCH::CHECK_VAR
***************************************************************************************/

int FAT_BA_BS_FORMU_LISTESI_BATCH::CHECK_VAR ( QObject * object )
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

        m_ui->lineEdit_cari_hesap_ismi->setText( cari_hesap_ismi);
        m_cari_hesap_id = cari_hesap_id;
    }
    else if (object EQ m_ui->checkBox_tarih_araligi ) {
        if ( m_ui->checkBox_tarih_araligi->isChecked() EQ false ) {
            m_ui->frame_tarih_araligi->setDisabled(true);

        }
        else {
            m_ui->frame_tarih_araligi->setEnabled(true);

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

    else if ( object EQ m_ui->checkBox_tutar_araligi ) {
        if ( m_ui->checkBox_tutar_araligi->isChecked() EQ false ) {
            m_ui->frame_tutar_araligi->setDisabled(true);
            m_ui->commaEdit_max_tutar->clear();
            m_ui->commaEdit_max_tutar->SET_DOUBLE( 5000.00 );

        }
        else {
            m_ui->frame_tutar_araligi->setEnabled(true);
            SET_FOCUS( m_ui->commaEdit_max_tutar );
            m_ui->commaEdit_max_tutar->clear();
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
                   FAT_BA_BS_FORMU_LISTESI_BATCH::CHECK_RUN
***************************************************************************************/

int FAT_BA_BS_FORMU_LISTESI_BATCH::CHECK_RUN ()
{

    if ( m_ui->checkBox_cari_hesap->isChecked() EQ true ) {
        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Cari hesap kodunu boş bırakamazsınız."), m_ui->searchEdit_cari_hesap_kodu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->comboBox_fatura_alis_satis_turu->currentIndex() EQ -1 ) {

        m_ui->checkBox_fatura_alis_satis_turu->setChecked   ( true );
        m_ui->frame_fatura_alis_satis_turu->setEnabled      ( true );
        MSG_WARNING( tr ( "BA - BS Türünü seçin." ), m_ui->comboBox_fatura_alis_satis_turu );
        return ADAK_FAIL;
    }

    if ( m_ui->checkBox_tutar_araligi->isChecked() EQ true ) {
        if ( m_ui->commaEdit_max_tutar->GET_TEXT().isEmpty() EQ true  ) {

            MSG_INFO( tr( "Tutar boş bırakılamaz!!!" ), m_ui->commaEdit_max_tutar );
            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_fatura_alis_satis_turu->isChecked() EQ true ) {
        if ( m_ui->comboBox_fatura_alis_satis_turu->currentIndex() EQ -1 ) {
            MSG_INFO( tr( "Form türünü seçmelisiniz." ), m_ui->comboBox_fatura_alis_satis_turu );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_BATCH::RUN_BATCH
***************************************************************************************/

void FAT_BA_BS_FORMU_LISTESI_BATCH::RUN_BATCH ()
{

     FATURA_RAPOR_STRUCT * FAT_RAPOR = new FATURA_RAPOR_STRUCT;

     QDate baslama_tarihi;
     int ay = E9_GET_AYLAR_ENUM( m_ui->comboBox_ay->currentText() );
     if ( ay < 10  ) {
         baslama_tarihi   = QDate::fromString( QString::number( m_ui->commaEdit_yil->GET_INTEGER() ) + ".0" + QString::number( ay ) + ".01", "yyyy.MM.dd" );
     }
     else {
         baslama_tarihi   = QDate::fromString( QString::number( m_ui->commaEdit_yil->GET_INTEGER() ) + "." + QString::number( ay ) + ".01", "yyyy.MM.dd" );
     }

     int ay_kac_gun_cekiyor = baslama_tarihi.daysInMonth() -1;

     FAT_RAPOR->baslangic_tarihi        = baslama_tarihi;
     FAT_RAPOR->bitis_tarihi            = baslama_tarihi.addDays( ay_kac_gun_cekiyor );
     FAT_RAPOR->cari_hesap_id           = m_cari_hesap_id;
     FAT_RAPOR->max_tutar            = m_ui->commaEdit_max_tutar->GET_DOUBLE();

     FAT_RAPOR->fatura_alis_satis_turu  = FAT_GET_FAT_BA_BS_FORMU_TURU_ENUM( m_ui->comboBox_fatura_alis_satis_turu->currentText() );

     OPEN_REPORT_SHOWER( GET_FAT_BA_BS_FORMU_LISTESI_RAPORU( FAT_RAPOR ), nativeParentWidget() );

}

/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_BATCH::CHECK_EXIT
***************************************************************************************/

int FAT_BA_BS_FORMU_LISTESI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}


/**************************************************************************************
                   FAT_BA_BS_FORMU_LISTESI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void FAT_BA_BS_FORMU_LISTESI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget , QLineEdit * line_edit)
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
