#include "prs_ucret_pusulasi_batch_class.h"
#include "prs_ucret_pusulasi_batch_open.h"
#include "ui_prs_ucret_pusulasi_batch.h"
#include "print.h"
#include "cari_console_utils.h"
#include "e9_console_utils.h"
#include "adak_std_utils.h"
#include "e9_gui_utils.h"
#include "prs_ucret_pusulasi_raporu_open.h"
#include "prs_enum.h"
#include "prs_struct.h"
#include "prs_con_utils.h"
#include "prs_gui_utils.h"
#include "sube_console_utils.h"
#include "adak_gruplar.h"
#include "cari_kart_arama_open.h"
#include "prs_ucret_pusulasi_raporu_open.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_PRS_UCRET_PUSULASI_BATCH
***************************************************************************************/

void OPEN_PRS_UCRET_PUSULASI_BATCH ( QWidget * parent )
{
    PRS_UCRET_PUSULASI_BATCH * F = new PRS_UCRET_PUSULASI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_BATCH::PRS_UCRET_PUSULASI_BATCH
***************************************************************************************/

PRS_UCRET_PUSULASI_BATCH::PRS_UCRET_PUSULASI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::PRS_UCRET_PUSULASI_BATCH )
{
    m_ui->setupUi       (this );
    START_BATCH_KERNEL  (this, DB );
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_BATCH::SETUP_FORM
***************************************************************************************/

void PRS_UCRET_PUSULASI_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "PRS - ÜCRET PUSULASI" ) );
    SET_HELP_PAGE     ("personel-bordrosu_ucret-pusulasi");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME  ("PRS_UCRET_PUSULASI_BATCH");

    m_sube_id = 0;
    M_UCRET_PUSULASI_VAR = new PRS_UCRET_PUSULASI_RAPORU_STRUCT;


    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );

    SET_FIRST_FOCUS_WIDGET ( m_ui->searchEdit_personel_kodu );

    REGISTER_CHANGER_BUTTON( m_ui->pushButton_gruplar );

    m_ui->checkBox_personel->setChecked( true );

    m_ui->frame_subeler->setEnabled( false );

    m_ui->frame_gruplar->setEnabled( false );

    E9_FILL_COMBOBOX_AYLAR( m_ui->comboBox_bordro_ay );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_bordro_ay, E9_GET_AYLAR_STRING( QDate::currentDate().month() ) );

    PRS_FILL_COMBO_BOX_BORDRO_YILLARI( m_ui->comboBox_bordro_yil );

    m_ui->combo_box_subeler->addItem( NULL,"Şube Seçiniz...");
    PRS_FILL_COMBO_BOX_SUBE_SECIMI( m_ui->combo_box_subeler );

    m_ui->limitedTextEdit_gruplar->SET_MAX_STRING_LENGTH( 125 );
}


/**************************************************************************************
                   PRS_UCRET_PUSULASI_BATCH::CHECK_VAR
***************************************************************************************/

int PRS_UCRET_PUSULASI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->checkBox_subeler ) {
        if ( m_ui->checkBox_subeler->isChecked() EQ true ) {
            m_ui->frame_subeler->setEnabled( true );

            m_ui->checkBox_personel->setChecked( false );
            m_ui->checkBox_gruplar->setChecked( false );

            m_ui->searchEdit_personel_kodu->CLEAR();
            m_ui->lineEdit_personel_adi->clear();
            m_grup_idleri.clear();
            m_ui->frame_gruplar->setEnabled( false );
            m_ui->frame_personel->setEnabled( false );
        }
    }
    else if( object EQ m_ui->checkBox_gruplar ) {
        if ( m_ui->checkBox_gruplar->isChecked() EQ true ) {
            m_ui->frame_gruplar->setEnabled( true );

            m_ui->checkBox_personel->setChecked( false );
            m_ui->checkBox_subeler->setChecked( false );

            m_ui->combo_box_subeler->setCurrentIndex( -1 );
            m_ui->searchEdit_personel_kodu->CLEAR();
            m_ui->lineEdit_personel_adi->clear();
            m_ui->frame_personel->setEnabled( false );
            m_ui->frame_subeler->setEnabled( false );
        }
    }
    else if ( object EQ m_ui->checkBox_personel ) {
        if ( m_ui->checkBox_personel->isChecked() EQ true ) {
            m_ui->frame_personel->setEnabled( true );

            m_ui->checkBox_gruplar->setChecked( false );
            m_ui->checkBox_subeler->setChecked( false );

            m_ui->combo_box_subeler->setCurrentIndex( -1 );
            m_grup_idleri.clear();
            m_ui->frame_gruplar->setEnabled( false );
            m_ui->frame_subeler->setEnabled( false );
        }
    }
    else if( object EQ m_ui->searchEdit_personel_kodu ) {
        QString cari_hesap_ismi;
        CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_personel_kodu->GET_TEXT(), NULL, &cari_hesap_ismi );
        m_ui->lineEdit_personel_adi->setText( cari_hesap_ismi );
    }
    else if ( object EQ m_ui->combo_box_subeler ) {
        SUBE_GET_SUBE_ID_ADI( m_ui->combo_box_subeler->currentText().split("-").at(0) , &m_sube_id );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_BATCH::CHECK_RUN
***************************************************************************************/

int PRS_UCRET_PUSULASI_BATCH::CHECK_RUN ()
{
    if ( m_ui->checkBox_personel->isChecked() EQ true ) {
        if ( m_ui->searchEdit_personel_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Personel Aralığı Seçin."), m_ui->searchEdit_personel_kodu );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_BATCH::RUN_BATCH
***************************************************************************************/

void PRS_UCRET_PUSULASI_BATCH::RUN_BATCH ()
{

    M_UCRET_PUSULASI_VAR->bordro_ay         = E9_GET_AYLAR_ENUM( m_ui->comboBox_bordro_ay->currentText() );
    M_UCRET_PUSULASI_VAR->bordro_yil        = m_ui->comboBox_bordro_yil->currentText().toInt();
    M_UCRET_PUSULASI_VAR->personel_list     = GRP_GRUBA_EKLI_KAYITLARI_BUL ( E9_PROGRAMI, PERSONEL_MODULU, m_grup_idleri );
    M_UCRET_PUSULASI_VAR->personel_id       = PRS_GET_PERSONEL_ID( m_ui->searchEdit_personel_kodu->GET_TEXT() );
    M_UCRET_PUSULASI_VAR->cari_hesap_kodu   = m_ui->searchEdit_personel_kodu->GET_TEXT();
    M_UCRET_PUSULASI_VAR->sube_id           = m_sube_id;

    OPEN_REPORT_SHOWER( GET_PRS_UCRET_PUSULASI_RAPORU( M_UCRET_PUSULASI_VAR ), nativeParentWidget() );
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_BATCH::CHECK_EXIT
***************************************************************************************/

int PRS_UCRET_PUSULASI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void PRS_UCRET_PUSULASI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget , QLineEdit * line_edit)
{

    if ( widget EQ m_ui->searchEdit_personel_kodu ) {
        int cari_hesap_id = OPEN_CARI_KART_ARAMA("", SAHIS_KAYDI, this, 0, E9_PROGRAMI, PERSONEL_MODULU, "Personel Arama", "DELETED" );

        if ( cari_hesap_id > 0 ) {
            SQL_QUERY query( DB );

            query.PREPARE_SELECT( "prs_personeller", "personel_id"," cari_hesap_id = :cari_hesap_id");
            query.SET_VALUE     ( ":cari_hesap_id", cari_hesap_id );

            if ( query.SELECT() NE 0 ) {
                query.NEXT();

                 M_UCRET_PUSULASI_VAR->personel_id   = query.VALUE( 0 ).toInt();
                 M_UCRET_PUSULASI_VAR->cari_hesap_kodu     = CARI_FIND_HESAP_KODU( cari_hesap_id );
                 line_edit->setText( M_UCRET_PUSULASI_VAR->cari_hesap_kodu );
            }
        }
    }

}

/**************************************************************************************
                   PRS_UCRET_PUSULASI_BATCH::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int PRS_UCRET_PUSULASI_BATCH::CHANGER_BUTTON_CLICKED(QAbstractButton *p_button)
{
    if ( p_button EQ m_ui->pushButton_gruplar ) {
        if ( OPEN_GRUP_SECIM ( E9_PROGRAMI,PERSONEL_MODULU , &m_grup_idleri, DB, this ) EQ true ) {
            QString grup_str = GRP_GRUP_ADLARINI_BUL ( &m_grup_idleri, new QStringList() , new QStringList() );
            m_ui->limitedTextEdit_gruplar->setPlainText( grup_str );
            return ADAK_RECORD_CHANGED;
        }
    }

    return ADAK_RECORD_UNCHANGED;
}
