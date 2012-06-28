#include <QMessageBox>
#include "bnk_banka_defteri_batch_class.h"
#include "bnk_banka_defteri_batch_open.h"
#include "ui_bnk_banka_defteri_batch.h"
#include "print.h"
#include "e9_console_utils.h"
#include "bnk_gui_utils.h"
#include "banka_struct.h"
#include "bnk_banka_defteri_batch_open.h"
#include "e9_gui_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_log.h"
#include "e9_enum.h"
#include "bnk_banka_defteri_raporu_open.h"

extern ADAK_SQL * DB;

/***************************************************/
/*           F_BNK_BANKA_DEFTERI_BATCH             */
/***************************************************/

void OPEN_BNK_BANKA_DEFTERI_BATCH ( QWidget * parent)
{
    BNK_BANKA_DEFTERI_BATCH * B = new BNK_BANKA_DEFTERI_BATCH( parent);
    B->EXEC( NOT_FULL_SCREEN );
}

/************************************************************************/
/*              BNK_BANKA_DEFTERI_BATCH::BNK_BANKA_DEFTERI_BATCH        */
/************************************************************************/

BNK_BANKA_DEFTERI_BATCH::BNK_BANKA_DEFTERI_BATCH( QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::BNK_BANKA_DEFTERI_BATCH)
{
    m_ui->setupUi      (this);

    START_BATCH_KERNEL (this,DB);
}

/*****************************************************/
/*      BNK_BANKA_DEFTERI_BATCH::SETUP_FORM          */
/*****************************************************/

void BNK_BANKA_DEFTERI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE  ("banka-islemleri_banka-defteri" );
    SET_PAGE_TITLE ( tr ( "BNK - BANKA DEFTERİ" ) );

    REGISTER_BUTTON_WIDGET(m_ui->widget_batch_buttons);

    m_ui->adakDate_bsl_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
    m_ui->adakDate_bts_tarihi->SET_DATE(MALI_ARRAY_LAST_DATE());
    m_ui->combo_box_banka_hesap_kodu->setEditable(false);

    SET_NAME_OF_RUN_BATCH_BUTTON("Rapor Al");
    SET_ENTER_KEY_FOR_RUN_BATCH(true);

    SET_FIRST_FOCUS_WIDGET(m_ui->combo_box_banka_hesap_kodu);
    FOCUS_FIRST_WIDGET();

    SET_ENTER_KEY_FOR_RUN_BATCH(true);    

    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI(m_ui->combo_box_banka_hesap_kodu);

    m_bnk_hesap_no_id = 0;    

    SET_SETTING_NAME    ( "BANKA_DEFTERI_BATCH" );

    m_ui->frame_entegre_filtresi->setVisible(false);

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 1 ) {
        m_ui->frame_entegre_filtresi->setVisible(true);
    }

}

/*****************************************************/
/*      BNK_BANKA_DEFTERI_BATCH::CHECK_VAR           */
/*****************************************************/

int BNK_BANKA_DEFTERI_BATCH::CHECK_VAR(QObject * object)
{
    if ( object EQ m_ui->combo_box_banka_hesap_kodu ) {

        int row = m_ui->combo_box_banka_hesap_kodu->view()->currentIndex().row();

        if ( row EQ -1 ) {
            return ADAK_FAIL;
        }

        m_bnk_hesap_no_id         =BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combo_box_banka_hesap_kodu,row,0).toInt();
        m_banka_hesabi_doviz_id   = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combo_box_banka_hesap_kodu,row,1).toInt();
        m_ui->lineEdit_doviz_cinsi->setText ( DVZ_GET_DOVIZ_KODU ( m_banka_hesabi_doviz_id ) );
        QString banka_bilgisi     =BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combo_box_banka_hesap_kodu,row,3).toString()
                                + " / " + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL
                                (m_ui->combo_box_banka_hesap_kodu,row,4).toString() + " "
                                + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combo_box_banka_hesap_kodu,row,5).toString()
                                + "  " + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combo_box_banka_hesap_kodu,row,6).toString();
        m_ui->lineEdit_banka_sube_bilgisi->setText(banka_bilgisi);

        m_ui->combo_box_banka_hesap_kodu->setCurrentIndex(row);

    }

    else if ( object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ false ) {
            m_ui->adakDate_bsl_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
            m_ui->adakDate_bts_tarihi->SET_DATE(MALI_YIL_365_DATE());
            m_ui->frame_tarih_araligi->setDisabled(true);
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled(true);
        }
    }
    else if ( object EQ m_ui->checkbox_nakli_yekun ) {

        if ( m_ui->checkbox_nakli_yekun->isChecked() EQ false ) {
            m_ui->frame_nakli_yekun->setDisabled(true);
        }
        else {
            m_ui->frame_nakli_yekun->setEnabled(true);
        }
    }
    return ADAK_OK;
}

/*****************************************************/
/*       BNK_BANKA_DEFTERI_BATCH::CHECK_RUN        */
/*****************************************************/

int BNK_BANKA_DEFTERI_BATCH::CHECK_RUN()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bsl_tarihi, m_ui->adakDate_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/*****************************************************/
/*          BNK_BANKA_DEFTERI_BATCH::RUN_BATCH      */
/*****************************************************/

void BNK_BANKA_DEFTERI_BATCH::RUN_BATCH()
{

    BNK_RV = new BNK_RAPOR_VARS;

    BNK_RV->baslangic_tarihi     = m_ui->adakDate_bsl_tarihi->QDATE();
    BNK_RV->bitis_tarihi         = m_ui->adakDate_bts_tarihi->QDATE();
    BNK_RV->nakli_yekun          = m_ui->checkbox_nakli_yekun->isChecked();
    BNK_RV->banka_hesap_no_id    = m_bnk_hesap_no_id;

    QString log_detaylari = ("Log Türü - Raporlar , Banka Defteri\n" +
                                        m_ui->combo_box_banka_hesap_kodu->currentText() + "-" + m_ui->lineEdit_banka_sube_bilgisi->text()
                                       +" " + m_ui->lineEdit_doviz_cinsi->text() + " " +
                                       m_ui->combo_box_banka_hesap_kodu->currentText() + "\nTarih Aralığı  : " +
                                       m_ui->adakDate_bsl_tarihi->QDATE().toString("dd MMMM yyyy") + " - " +
                                       m_ui->adakDate_bts_tarihi->QDATE().toString("dd MMMM yyyy"));
    if ( BNK_RV->nakli_yekun EQ 1 ) {
        log_detaylari.append("\nNakli Yekün");
    }

    DB->START_TRANSACTION  ();
    E9_LOG_KAYDI_EKLE (BANKA_MODULU , LOG_BNK_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari);
    DB->COMMIT_TRANSACTION ();

    OPEN_REPORT_SHOWER ( GET_BNK_BANKA_DEFTERI_RAPORU( BNK_RV ), nativeParentWidget() );

    delete BNK_RV;
}
