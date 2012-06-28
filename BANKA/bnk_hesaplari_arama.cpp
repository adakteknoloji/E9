#include <QMessageBox>
#include "adak_utils.h"
#include "adak_sql.h"
#include "bnk_hesaplari_arama_class.h"
#include "bnk_hesaplari_arama_open.h"
#include "ui_bnk_hesaplari_arama.h"
#include "adak_std_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "bnk_gui_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_BNK_HESAPLARI_ARAMA
***************************************************************************************/

int OPEN_BNK_HESAPLARI_ARAMA ( QWidget * parent )
{
    int secilen_id    = -1;

    BNK_HESAPLARI_ARAMA * F = new BNK_HESAPLARI_ARAMA( parent);
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   BNK_HESAPLARI_ARAMA::BNK_HESAPLARI_ARAMA
***************************************************************************************/

BNK_HESAPLARI_ARAMA::BNK_HESAPLARI_ARAMA ( QWidget *parent) : ARAMA_KERNEL (parent), m_ui(new Ui::BNK_HESAPLARI_ARAMA)
{
    m_ui->setupUi      (this);
    START_ARAMA_KERNEL (this, DB );
}

/**************************************************************************************
                   BNK_HESAPLARI_ARAMA::SETUP_FORM
***************************************************************************************/

void BNK_HESAPLARI_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE     ( "banka_hesaplari_arama.html" );
    SET_PAGE_TITLE    ( tr ( "BNK - BANKA HESAP ARAMA" ) );
    SET_SETTING_NAME  ( "BANKA_HESAP_ARAMA" );

    REGISTER_TABLE_WIDGET         (m_ui->tableWidget);
    REGISTER_ARAMA_BUTTONS_WIDGET (m_ui->widget_batch_buttons);

    SET_HEADERS(QStringList()<<tr("Hesap_no Id")<<tr("Hesap Kodu")<<tr("Hesap Ünvanı")<<tr("Banka İsmi")<<tr("Şube Kodu")
                             <<tr("Şube İsmi")<<tr("Hesap \n Numarası")  << tr("Döviz Cinsi"));

    m_ui->sube_bilgisi_frame->setDisabled(true);
    m_ui->banka_bilgisi_frame->setDisabled(true);

    SET_SORTING(false);
    SET_ENTER_KEY_FOR_RUN_BATCH(false);

    SQL_QUERY select_query(DB);
    select_query.PREPARE_SELECT("bnk_hesaplar","banka_ismi","","banka_ismi");
    if(select_query.SELECT() NE 0 ) {
        while (select_query.NEXT()) {
            m_ui->combobox_banka_ismi->addItem(select_query.VALUE(0).toString());
        }
    }
    m_ui->combobox_banka_ismi->setCurrentIndex(-1);

    m_ui->tableWidget->hideColumn(0);

    m_ui->tableWidget->setColumnWidth ( 1, 100 );
    m_ui->tableWidget->setColumnWidth ( 2, 100 );
    m_ui->tableWidget->setColumnWidth ( 3, 110 );
    m_ui->tableWidget->setColumnWidth ( 4, 100 );
    m_ui->tableWidget->setColumnWidth ( 5, 100 );
    m_ui->tableWidget->setColumnWidth ( 6, 100 );
    m_ui->tableWidget->setColumnWidth ( 7, 100 );

    SET_FIND_BUTTON_NAME ( tr ( "Hesapları Bul" ) );
}

/**************************************************************************************
                   BNK_HESAPLARI_ARAMA::CHECK_VAR
***************************************************************************************/

int BNK_HESAPLARI_ARAMA::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->combobox_banka_ismi ) {

        m_ui->combobox_sube_ismi->clear();

        BNK_GET_FIRMANIN_BANKA_SUBELERI ( m_ui->combobox_banka_ismi->currentText(), m_ui->combobox_sube_ismi );

        m_ui->combobox_sube_ismi->setCurrentIndex ( -1 );
    }
    else if ( object EQ m_ui->checkbox_sube_bilgisi ) {

        if ( m_ui->checkbox_sube_bilgisi->isChecked() EQ true ) {
            m_ui->sube_bilgisi_frame->setEnabled(true);
            SET_FOCUS( m_ui->combobox_sube_ismi );
        }
        else {
            m_ui->sube_bilgisi_frame->setEnabled(false);
            m_ui->combobox_sube_ismi->setCurrentIndex(-1);
        }
    }
    else if ( object EQ m_ui->checkbox_banka_bilgisi ) {

        if ( m_ui->checkbox_banka_bilgisi->isChecked() EQ true ) {
            m_ui->banka_bilgisi_frame->setEnabled(true);
            SET_FOCUS( m_ui->combobox_banka_ismi );
        }
        else {
            m_ui->banka_bilgisi_frame->setDisabled(true);
            m_ui->combobox_banka_ismi->setCurrentIndex(-1);
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_ARAMA::CHECK_RUN
***************************************************************************************/

int BNK_HESAPLARI_ARAMA::CHECK_RUN()
{

    if ( m_ui->checkbox_banka_bilgisi->isChecked() EQ true ) {
        if ( m_ui->combobox_banka_ismi->currentIndex() EQ -1 ) {
            MSG_WARNING( tr("Banka ismini seçmelisiniz!.."), m_ui->combobox_banka_ismi );
            return ADAK_FAIL;
        }
    }

    if ( m_ui->checkbox_sube_bilgisi->isChecked() EQ true ) {
        if ( m_ui->combobox_sube_ismi->currentIndex() EQ -1 ) {
            MSG_WARNING( tr("Şube seçmelisiniz!.."), m_ui->combobox_sube_ismi );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_ARAMA::SEARCH
***************************************************************************************/

void BNK_HESAPLARI_ARAMA::SEARCH()
{
    SQL_QUERY f_query ( DB );

    f_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id, banka_ismi, sube_kodu, sube_ismi, hesap_numarasi,"
                                "doviz_id, banka_hesap_kodu, banka_hesap_unvani","");

    if ( m_ui->checkbox_banka_bilgisi->isChecked() EQ true ) {
        f_query.AND_EKLE ("banka_ismi = :banka_ismi");
        f_query.SET_VALUE(":banka_ismi",m_ui->combobox_banka_ismi->currentText());
    }

    if ( m_ui->checkbox_sube_bilgisi->isChecked() EQ true ) {
        f_query.AND_EKLE ("sube_ismi = :sube_ismi");
        f_query.SET_VALUE(":sube_ismi",m_ui->combobox_sube_ismi->currentText());
    }

    int current_row;

    if ( f_query.SELECT() > 0 ) {

        while ( f_query.NEXT() EQ true ) {

            current_row = ADD_NEW_ROW();

            m_ui->tableWidget->item ( current_row, 0 )->setText ( f_query.VALUE(0).toString());
            m_ui->tableWidget->item ( current_row, 1 )->setText ( f_query.VALUE("banka_hesap_kodu").toString());
            m_ui->tableWidget->item ( current_row, 2 )->setText ( f_query.VALUE("banka_hesap_unvani").toString());
            m_ui->tableWidget->item ( current_row, 3 )->setText ( f_query.VALUE(1).toString());
            m_ui->tableWidget->item ( current_row, 4 )->setText ( f_query.VALUE(2).toString());
            m_ui->tableWidget->item ( current_row, 5 )->setText ( f_query.VALUE(3).toString());
            m_ui->tableWidget->item ( current_row, 6 )->setText ( f_query.VALUE(4).toString());
            m_ui->tableWidget->item ( current_row, 7 )->setText ( DVZ_GET_DOVIZ_KODU(f_query.VALUE(5).toInt()));
        }
    }
}

/**************************************************************************************
                   BNK_HESAPLARI_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int BNK_HESAPLARI_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item(selected_row_number,0)->text().toInt();
    return ADAK_EXIT;
}
