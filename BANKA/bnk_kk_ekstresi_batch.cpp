#include <QMessageBox>
#include "bnk_kk_ekstresi_batch_class.h"
#include "bnk_kk_ekstresi_batch_open.h"
#include "ui_bnk_kk_ekstresi_batch.h"
#include "print.h"
#include "e9_enum.h"
#include "banka_struct.h"
#include "bnk_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_log.h"
#include "bnk_kredi_karti_secimi_open.h"
#include "bnk_kk_ekstresi_raporu_open.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_BNK_KK_EKSTRESI_BATCH
***************************************************************************************/

void OPEN_BNK_KK_EKSTRESI_BATCH ( QWidget * parent )
{
    BNK_KK_EKSTRESI_BATCH * F = new BNK_KK_EKSTRESI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   BNK_KK_EKSTRESI_BATCH::BNK_KK_EKSTRESI_BATCH
***************************************************************************************/

BNK_KK_EKSTRESI_BATCH::BNK_KK_EKSTRESI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::BNK_KK_EKSTRESI_BATCH )
{
    m_ui->setupUi      (this );

    START_BATCH_KERNEL (this, DB );
}

/**************************************************************************************
                   BNK_KK_EKSTRESI_BATCH::SETUP_FORM
***************************************************************************************/

void BNK_KK_EKSTRESI_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE               ( tr ( "BNK - KREDİ KARTLARI EKSTRESİ" ) );
    SET_SETTING_NAME             ( "BNK_KK_EKSTRESI_BATCH");
    SET_HELP_PAGE                ( "banka-islemleri_kredi-karti-ekstresi");
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->searchedit_kk_numarasi );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bits_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE() );
    m_ui->frame_tarih_araligi->setDisabled ( true );

    m_kredi_karti_id = 0;
    m_hesap_no_id    = 0;

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->frame_entegre_filtresi->setVisible(false);
    }
}

/**************************************************************************************
                   BNK_KK_EKSTRESI_BATCH::CHECK_VAR
***************************************************************************************/

int BNK_KK_EKSTRESI_BATCH::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->searchedit_kk_numarasi ) {

        SQL_QUERY sql_query(DB);

        sql_query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari","kredi_karti_id,hesap_no_id","kredi_karti_numarasi = :kredi_karti_numarasi");
        sql_query.SET_VALUE(":kredi_karti_numarasi" , m_ui->searchedit_kk_numarasi->GET_TEXT());

        if (sql_query.SELECT() EQ 0 ) {
            MSG_WARNING( tr("Aradığınız kredi kartı numarası bulunamadı."), m_ui->searchedit_kk_numarasi );

            return ADAK_FAIL;
        }
        sql_query.NEXT();
        m_kredi_karti_id = sql_query.VALUE(0).toInt();
        m_hesap_no_id    = sql_query.VALUE(1).toInt();

    }
    else if ( p_object EQ m_ui->checkbox_pos_no ) {

        if ( m_ui->checkbox_pos_no->isChecked() EQ false ) {
            m_ui->searchedit_kk_numarasi->SET_TEXT( "" );
            m_ui->frame_pos_no->setDisabled(true);
        }
        else {
            m_ui->frame_pos_no->setEnabled(true);
            SET_FOCUS( m_ui->searchedit_kk_numarasi );
        }
    }
    else if ( p_object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ false ) {
            m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_bits_tarihi->SET_DATE( MALI_YIL_365_DATE() );
            m_ui->frame_tarih_araligi->setDisabled(true);
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled(true);
            SET_FOCUS( m_ui->adakDate_bas_tarihi );
        }
    }
    else if ( p_object EQ m_ui->checkbox_nakli_yekun ) {

        if ( m_ui->checkbox_nakli_yekun->isChecked() EQ false ) {
            m_ui->frame_nakli_yekun->setDisabled ( true );
        }
        else {
            m_ui->frame_nakli_yekun->setEnabled ( true );
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_KK_EKSTRESI_BATCH::CHECK_RUN
***************************************************************************************/

int BNK_KK_EKSTRESI_BATCH::CHECK_RUN ()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarihi, m_ui->adakDate_bits_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_KK_EKSTRESI_BATCH::RUN_BATCH
***************************************************************************************/

void BNK_KK_EKSTRESI_BATCH::RUN_BATCH ()
{

    BNK_RAPOR_VARS * BNK_RV = new BNK_RAPOR_VARS;

    BNK_RV->kredi_karti_id   = m_kredi_karti_id;
    BNK_RV->banka_hesap_no_id= m_hesap_no_id;
    BNK_RV->baslangic_tarihi = m_ui->adakDate_bas_tarihi->QDATE();
    BNK_RV->bitis_tarihi     = m_ui->adakDate_bits_tarihi->QDATE();
    BNK_RV->nakli_yekun      = QVariant ( m_ui->checkbox_nakli_yekun->isChecked()).toInt();

    QString log_detaylari = ("Log Türü - Raporlar , Kredi Kartı Ekstresi\nKredi Kartı Numarası  : " +
                            m_ui->searchedit_kk_numarasi->GET_TEXT() + "\nTarih Aralığı  : " +
                             m_ui->adakDate_bas_tarihi->QDATE().toString("dd MMMM yyyy") + " - " +
                             m_ui->adakDate_bits_tarihi->QDATE().toString("dd MMMM yyyy"));
    if(BNK_RV->nakli_yekun EQ 1 ) {
        log_detaylari.append("\nNakli Yekün");
    }

    DB->START_TRANSACTION();

    E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari);

    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER ( GET_BNK_KK_EKSTRESI_RAPORU( BNK_RV ), nativeParentWidget() );

    delete BNK_RV;
}


/**************************************************************************************
                   BNK_KK_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void BNK_KK_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * , QLineEdit * line_edit)
{
    int kredi_karti_id = OPEN_BNK_KREDI_KARTI_SEC(-1,this);

    if (kredi_karti_id NE -1 ) {
        line_edit->setText(BNK_GET_KREDI_KARTI_NUMARASI(kredi_karti_id));
    }

}
