#include "stok_detaylar_batch_class.h"
#include "ui_stok_detaylar_batch.h"
#include "adak_sql.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_struct.h"
#include "e9_console_utils.h"
#include "muh_console_utils.h"
#include "sube_console_utils.h"
#include "e9_gui_utils.h"
#include "muh_hesap_arama_open.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_STOK_DETAYLAR_BATCH
***************************************************************************************/

bool OPEN_STOK_DETAYLAR_BATCH ( STOK_DETAYLAR_STRUCT * P_STOK_DETAYLARI ,QWidget * parent )
{
    bool degisiklik_yapildi_mi = false;

    STOK_DETAYLAR_BATCH * F = new STOK_DETAYLAR_BATCH ( P_STOK_DETAYLARI , parent );

    F->m_degisiklik_yapildi_mi = &degisiklik_yapildi_mi;

    F->EXEC( NOT_FULL_SCREEN );

    return degisiklik_yapildi_mi;
}

/**************************************************************************************
                   STOK_DETAYLAR_BATCH::STOK_DETAYLAR_BATCH
***************************************************************************************/

STOK_DETAYLAR_BATCH::STOK_DETAYLAR_BATCH ( STOK_DETAYLAR_STRUCT * P_STOK_DETAYLARI , QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::STOK_DETAYLAR_BATCH )
{
    m_ui->setupUi      ( this );

    M_STOK_DETAYLARI = P_STOK_DETAYLARI;

    m_muh_hesap_id  =  M_STOK_DETAYLARI->muh_hesap_id;

    START_BATCH_KERNEL ( this, DB );
}


/**************************************************************************************
                   STOK_DETAYLAR_BATCH::SETUP_FORM
***************************************************************************************/

void STOK_DETAYLAR_BATCH::SETUP_FORM ()
{
    SET_PAGE_TITLE    ( tr ( "STK - DETAYLAR" ) );
    SET_HELP_PAGE     ( "stok-detaylar");

    SET_AUTO_EXIT_BATCH             ( true );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME  ("STOK_DETAYLAR_BATCH");

    if ( E9_KULLANICININ_CALISTIGI_SIRKET() EQ 0 ) {
        m_ui->checkBox_otv_muaf_mi->setChecked(true);
        m_ui->checkBox_kdv_muaf_mi->setChecked(true);
    }

    m_ui->searchEdit_muh_hesabi->SET_TEXT( MUH_GET_HESAP_KODU( M_STOK_DETAYLARI->muh_hesap_id ) );
    m_ui->lineEdit_muh_hesap_adi->setText( MUH_GET_HESAP_ISMI( M_STOK_DETAYLARI->muh_hesap_id ) );

    m_ui->checkBox_kdv_muaf_mi->setChecked(QVariant(M_STOK_DETAYLARI->kdv_muaf_mi).toBool());
    m_ui->checkBox_otv_muaf_mi->setChecked(QVariant(M_STOK_DETAYLARI->otv_muaf_mi).toBool());
}

/**************************************************************************************
                   STOK_DETAYLAR_BATCH::CHECK_VAR
***************************************************************************************/

int STOK_DETAYLAR_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED(object);

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_DETAYLAR_BATCH::CHECK_RUN
***************************************************************************************/

int STOK_DETAYLAR_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_DETAYLAR_BATCH::RUN_BATCH
***************************************************************************************/

void STOK_DETAYLAR_BATCH::RUN_BATCH ()
{
   if ( M_STOK_DETAYLARI->kdv_muaf_mi NE QVariant(m_ui->checkBox_kdv_muaf_mi->isChecked()).toInt() ) {

        if ( m_ui->checkBox_kdv_muaf_mi->isChecked() EQ true ) {
            M_STOK_DETAYLARI->kdv_muaf_mi = 1;
        }
        else {
            M_STOK_DETAYLARI->kdv_muaf_mi = 0;
        }
        *m_degisiklik_yapildi_mi = true;
    }
    if ( M_STOK_DETAYLARI->otv_muaf_mi NE QVariant(m_ui->checkBox_otv_muaf_mi->isChecked()).toInt() ) {

        if ( m_ui->checkBox_otv_muaf_mi->isChecked() EQ true ) {
            M_STOK_DETAYLARI->otv_muaf_mi = 1;
        }
        else {
            M_STOK_DETAYLARI->otv_muaf_mi = 0;
        }
        *m_degisiklik_yapildi_mi = true;
    }

    if ( m_muh_hesap_id NE M_STOK_DETAYLARI->muh_hesap_id ) {
        *m_degisiklik_yapildi_mi    = true;
    }
}

/**************************************************************************************
                   STOK_DETAYLAR_BATCH::CHECK_EXIT
***************************************************************************************/

int STOK_DETAYLAR_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_DETAYLAR_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void STOK_DETAYLAR_BATCH::SEARCH_EDIT_CLICKED( QWidget *widget, QLineEdit *line_edit )
{
    Q_UNUSED( line_edit );

    if ( widget EQ m_ui->searchEdit_muh_hesabi ) {

        int muh_hesap_id = OPEN_MUH_HESAP_ARAMA( m_ui->searchEdit_muh_hesabi->GET_TEXT(),this,1);
        if (muh_hesap_id NE -1) {
            m_ui->lineEdit_muh_hesap_adi->setText( MUH_GET_HESAP_ISMI( muh_hesap_id ));
            m_ui->searchEdit_muh_hesabi->SET_TEXT( MUH_GET_HESAP_KODU( muh_hesap_id ) );
        }

        M_STOK_DETAYLARI->muh_hesap_id  = muh_hesap_id;
    }

}
