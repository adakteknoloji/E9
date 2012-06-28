#include "fat_fatura_listesi_batch_class.h"
#include "fat_fatura_listesi_raporu_class.h"
#include "ui_fat_fatura_listesi_batch.h"
#include "print.h"
#include "e9_enum.h"
#include "cari_console_utils.h"
#include "sube_console_utils.h"
#include "adak_std_utils.h"
#include "fat_struct.h"
#include "fat_console_utils.h"
#include "fat_gui_utils.h"
#include "fat_enum.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "cari_kart_arama_open.h"
#include "sube_depo_secimi_open.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_FAT_FATURA_LISTESI_BATCH
***************************************************************************************/

void OPEN_FAT_FATURA_LISTESI_BATCH ( QWidget * parent )
{
    FAT_FATURA_LISTESI_BATCH * F = new FAT_FATURA_LISTESI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   FAT_FATURA_LISTESI_BATCH::FAT_FATURA_LISTESI_BATCH
***************************************************************************************/

FAT_FATURA_LISTESI_BATCH::FAT_FATURA_LISTESI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::FAT_FATURA_LISTESI_BATCH )
{
    m_ui->setupUi       (this );
    START_BATCH_KERNEL  (this, DB );
}

/**************************************************************************************
                   FAT_FATURA_LISTESI_BATCH::SETUP_FORM
***************************************************************************************/

void FAT_FATURA_LISTESI_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "FAT - FATURA LİSTESİ" ) );
    SET_HELP_PAGE     ("fatura-islemleri_fatura-listesi");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );
    m_ui->frame_sirket_filtresi->setVisible( false );
    m_ui->checkBox_sirket_filtresi->setVisible( false );

    FAT_FILL_ACIK_KAPALI_COMBO_BOX( m_ui->comboBox_acik_kapali_fatura );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_acik_kapali_fatura, FAT_GET_ACIK_KAPALI_FATURA_STRING( ENUM_TUM_FATURALAR ));

    m_ui->checkBox_tarih_araligi->setChecked( true );
    m_ui->frame_tarih_araligi->setEnabled( true );

    SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_bas_tarihi );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->frame_sirket_filtresi->setVisible( true );
        m_ui->checkBox_sirket_filtresi->setVisible( true );

        m_ui->frame_sirket_filtresi->setEnabled( true );
        m_ui->checkBox_sirket_filtresi->setChecked( true );
    }

    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_sirket_filtresi );

    SET_SETTING_NAME  ("FAT_FATURA_LISTESI_BATCH");
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );

    SET_ENTER_KEY_FOR_RUN_BATCH( true );

    m_ui->commaEdit_min_tutar->SET_PRECISION( 2 );
    m_ui->commaEdit_min_tutar->SET_RETURN_EMPTY( false );

    m_ui->commaEdit_min_tutar->SET_PRECISION( 2 );
    m_ui->commaEdit_max_tutar->SET_RETURN_EMPTY( false );

    m_ui->adakDate_bas_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
    m_ui->adakDate_bts_tarihi->SET_DATE(MALI_ARRAY_LAST_DATE());

    m_ui->frame_fatura_durumu->setDisabled(true);
    m_ui->comboBox_irsaliye_durumu->addItems(QStringList() << tr("İrsaliyelenmiş") << tr("İrsaliyelenmemiş"));
    m_ui->comboBox_irsaliye_durumu->setCurrentIndex(-1);
    m_ui->frame_cari_hesap->setDisabled(true);
    m_ui->frame_tutar_araligi->setDisabled(true);
    m_ui->frame_fatura_alis_satis_turu->setDisabled(true);
    m_ui->label_sirket->setDisabled(true);
    m_ui->comboBox_sirket_filtresi->setDisabled(true);

    m_ui->frame_fatura_turu->setEnabled( false );

    FAT_FILL_ALIS_SATIS_FAT_TURU_COMBO_BOX( m_ui->comboBox_fatura_alis_satis_turu  );

    m_ui->comboBox_fatura_alis_satis_turu->setCurrentIndex(-1);

    m_cari_hesap_id = -1;
    m_depo_id       = -1;

}


/**************************************************************************************
                   FAT_FATURA_LISTESI_BATCH::CHECK_VAR
***************************************************************************************/

int FAT_FATURA_LISTESI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchEdit_cari_hesap_kodu ) {
        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_cari_hesap_ismi->clear();
            return ADAK_OK;
        }

        QString cari_hesap_ismi;
        int cari_hesap_id;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi );
        if ( result EQ 0 ) {
            MSG_WARNING( tr("Aradığınız cari hesap kodu bulunamadı."), m_ui->searchEdit_cari_hesap_kodu );

            return ADAK_FAIL;
        }
        m_ui->lineEdit_cari_hesap_ismi->setText(cari_hesap_ismi);
        m_cari_hesap_id = cari_hesap_id;
    }
    else if (object EQ m_ui->checkBox_tarih_araligi) {
        if ( m_ui->checkBox_tarih_araligi->isChecked() EQ false ) {
            m_ui->frame_tarih_araligi->setDisabled(true);
            m_ui->adakDate_bas_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
            m_ui->adakDate_bts_tarihi->SET_DATE(MALI_YIL_365_DATE());
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled(true);
            SET_FOCUS( m_ui->adakDate_bas_tarihi);
            SET_FOCUS( m_ui->adakDate_bas_tarihi );

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
            SET_FOCUS( m_ui->comboBox_irsaliye_durumu );
        }
        else {
            m_ui->frame_fatura_durumu->setDisabled(true);
            m_ui->comboBox_irsaliye_durumu->setCurrentIndex(-1);
        }
    }

    else if ( object EQ m_ui->checkBox_tutar_araligi ) {
        if ( m_ui->checkBox_tutar_araligi->isChecked() EQ false ) {
            m_ui->frame_tutar_araligi->setDisabled(true);
            m_ui->commaEdit_min_tutar->clear();
            m_ui->commaEdit_max_tutar->clear();
        }
        else {
            m_ui->frame_tutar_araligi->setEnabled(true);
            SET_FOCUS( m_ui->commaEdit_min_tutar );
            m_ui->commaEdit_max_tutar->SET_DOUBLE(999999999.99);
            m_ui->commaEdit_min_tutar->SET_DOUBLE(0);
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
    else if ( object EQ m_ui->checkBox_sirket_filtresi ) {
        if ( m_ui->checkBox_sirket_filtresi->isChecked() EQ true ) {
            m_ui->frame_sirket_filtresi->setEnabled(true);
            m_ui->comboBox_sirket_filtresi->setEnabled(true);
            m_ui->label_sirket->setEnabled(true);
        }
        else {
            m_ui->frame_sirket_filtresi->setEnabled(false);
            m_ui->comboBox_sirket_filtresi->setEnabled(false);
            m_ui->label_sirket->setEnabled(false);
        }
    }
    else if ( object EQ m_ui->checkBox_fatura_turu ) {

        if ( m_ui->checkBox_fatura_alis_satis_turu->isChecked() EQ true AND m_ui->comboBox_fatura_alis_satis_turu->currentIndex() NE -1 ) {
            m_ui->comboBox_fatura_turu->clear();

            FAT_FILL_COMBOBOX_FATURA_TURU( m_ui->comboBox_fatura_turu, FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_ENUM(m_ui->comboBox_fatura_alis_satis_turu->currentText()), -1 , ENUM_FAT_NORMAL );
            m_ui->comboBox_fatura_turu->setCurrentIndex( -1 );

            if ( m_ui->checkBox_fatura_turu->isChecked() EQ true ) {
                m_ui->frame_fatura_turu->setEnabled( true );
            }
            else {
                m_ui->frame_fatura_turu->setEnabled( false );
            }
        }
        else {
            MSG_WARNING( tr("Aliş / Satış Türünü seçmelisiniz"), m_ui->comboBox_fatura_alis_satis_turu );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->comboBox_fatura_alis_satis_turu ) {
        m_ui->comboBox_fatura_turu->clear();
        FAT_FILL_COMBOBOX_FATURA_TURU( m_ui->comboBox_fatura_turu, FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_ENUM(m_ui->comboBox_fatura_alis_satis_turu->currentText()), -1 , ENUM_FAT_NORMAL );
        m_ui->comboBox_fatura_turu->setCurrentIndex( -1 );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_LISTESI_BATCH::CHECK_RUN
***************************************************************************************/

int FAT_FATURA_LISTESI_BATCH::CHECK_RUN ()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarihi, m_ui->adakDate_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    if ( m_ui->checkBox_cari_hesap->isChecked() EQ true ) {
        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Cari hesap kodunu boş bırakamazsınız."), m_ui->searchEdit_cari_hesap_kodu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_fatura_durumu->isChecked()  EQ true ) {
        if ( m_ui->comboBox_irsaliye_durumu->currentIndex() EQ -1 ) {
            MSG_WARNING(  tr("İrsaliye durumunu seçmelisiniz."), m_ui->comboBox_irsaliye_durumu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_tutar_araligi->isChecked() EQ true ) {
        if ( m_ui->commaEdit_min_tutar->GET_TEXT().isEmpty() EQ true OR m_ui->commaEdit_max_tutar->GET_TEXT().isEmpty() EQ true  ) {

            MSG_INFO( tr( "Tutarlar boş bırakılamaz!!!" ), m_ui->commaEdit_min_tutar );
            return ADAK_FAIL;
        }
    }

    if ( m_ui->checkBox_fatura_alis_satis_turu->isChecked() EQ true ) {
        if ( m_ui->comboBox_fatura_alis_satis_turu->currentIndex() EQ -1 ) {
            MSG_INFO( tr( "Fatura alış / satış türünü seçmelisiniz." ), m_ui->comboBox_fatura_alis_satis_turu );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_LISTESI_BATCH::RUN_BATCH
***************************************************************************************/

void FAT_FATURA_LISTESI_BATCH::RUN_BATCH ()
{

     FATURA_RAPOR_STRUCT * FAT_RAPOR = new FATURA_RAPOR_STRUCT;

     int fatura_alis_satis_turu         = FAT_GET_IRSALIYE_FATURA_ALIS_SATIS_TURU_ENUM(m_ui->comboBox_fatura_alis_satis_turu->currentText());

     FAT_RAPOR->baslangic_tarihi        = m_ui->adakDate_bas_tarihi->QDATE();
     FAT_RAPOR->bitis_tarihi            = m_ui->adakDate_bts_tarihi->QDATE();
     FAT_RAPOR->cari_hesap_id           = m_cari_hesap_id;
     FAT_RAPOR->irsaliye_durumu         = FAT_GET_IRSALIYE_DURUMU_ENUM( m_ui->comboBox_irsaliye_durumu->currentText() );
     FAT_RAPOR->min_tutar               = m_ui->commaEdit_min_tutar->GET_DOUBLE();
     FAT_RAPOR->max_tutar               = m_ui->commaEdit_max_tutar->GET_DOUBLE();
     FAT_RAPOR->fatura_alis_satis_turu  = fatura_alis_satis_turu;
     FAT_RAPOR->entegre_filtresi_turu   = m_ent_filtresi_turu;
     FAT_RAPOR->acik_kapali_fatura      = FAT_GET_ACIK_KAPALI_FATURA_ENUM( m_ui->comboBox_acik_kapali_fatura->currentText() );

     FAT_RAPOR->fatura_turu             = -1;
     if ( m_ui->checkBox_fatura_turu->isChecked() EQ true ) {
         QString fatura_turu = m_ui->comboBox_fatura_turu->currentText();
         switch ( fatura_alis_satis_turu ) {
             case ENUM_ALIS_IRSALIYESI_FATURASI :
             default :
                 fatura_turu.append( tr(" ( ALIŞ )")  );
                 break;
             case ENUM_SATIS_IRSALIYESI_FATURASI :
                 fatura_turu.append ( tr(" ( SATIŞ )") );
                 break;
         }
         FAT_RAPOR->fatura_turu         =  FAT_GET_FATURA_TURLERI_ENUM( fatura_turu );
     }

     OPEN_REPORT_SHOWER( GET_FAT_FATURA_LISTESI_RAPORU( FAT_RAPOR ), nativeParentWidget(), QPrinter::Landscape );

}

/**************************************************************************************
                   FAT_FATURA_LISTESI_BATCH::CHECK_EXIT
***************************************************************************************/

int FAT_FATURA_LISTESI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}


/**************************************************************************************
                   FAT_FATURA_LISTESI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void FAT_FATURA_LISTESI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget , QLineEdit * line_edit)
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
