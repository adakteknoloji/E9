#include <QMessageBox>
#include "bnk_pos_ekstresi_batch_class.h"
#include "bnk_pos_ekstresi_batch_open.h"
#include "ui_bnk_pos_ekstresi_batch.h"
#include "print.h"
#include "e9_console_utils.h"
#include "banka_struct.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "e9_log.h"
#include  "bnk_pos_secimi_open.h"
#include "bnk_pos_ekstresi_open.h"
#include "bnk_console_utils.h"

extern ADAK_SQL * DB;

/*******************************************/
/*         OPEN_BNK_POS_EKSTRESI_BATCH        */
/*******************************************/

void OPEN_BNK_POS_EKSTRESI_BATCH ( QWidget * parent)
{
    BNK_POS_EKSTRESI_BATCH * B = new BNK_POS_EKSTRESI_BATCH( parent);
    B->EXEC( NOT_FULL_SCREEN );
}

/*****************************************************************/
/*          BNK_POS_EKSTRESI_BATCH::BNK_POS_EKSTRESI_BATCH       */
/*****************************************************************/

BNK_POS_EKSTRESI_BATCH::BNK_POS_EKSTRESI_BATCH( QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::BNK_POS_EKSTRESI_BATCH)
{
    m_ui->setupUi      ( this);
    START_BATCH_KERNEL ( this, DB );
}

/*****************************************************/
/*         BNK_POS_EKSTRESI_BATCH::SETUP_FORM         */
/*****************************************************/

void BNK_POS_EKSTRESI_BATCH::SETUP_FORM()
{
    m_pos_id        =   0;
    m_hesap_no_id   =   0;

    SET_HELP_PAGE                           ( "banka-islemleri_pos-ekstresi" );
    SET_PAGE_TITLE                          ( tr ( "BNK - POS EKSTRESİ" ) );

    REGISTER_BUTTON_WIDGET                  ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON            ( tr ( "Rapor Al" ) );
    SET_ENTER_KEY_FOR_RUN_BATCH             ( true );

    m_ui->adakDate_bas_tarih->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bts_tarih->SET_DATE( MALI_ARRAY_LAST_DATE() );
    m_ui->frame_tarih_araligi->setDisabled  ( true );

    SET_FIRST_FOCUS_WIDGET                  ( m_ui->searchedit_pos_no );
    FOCUS_FIRST_WIDGET                      ();

    SET_SETTING_NAME                        ( "BANKA_POS_EKSTRESI_BATCH" );
}

/*****************************************************/
/*         BNK_POS_EKSTRESI_BATCH::CHECK_VAR         */
/*****************************************************/

int BNK_POS_EKSTRESI_BATCH::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->searchedit_pos_no ) {

        SQL_QUERY sql_query(DB);

        sql_query.PREPARE_SELECT("bnk_hesabin_poslari", "pos_id,hesap_no_id","pos_numarasi = :pos_numarasi");
        sql_query.SET_VALUE(":pos_numarasi" , m_ui->searchedit_pos_no->GET_TEXT());

        if ( sql_query.SELECT()  EQ 0 ) {
            MSG_WARNING( "Aradığınız pos numarası bulunamadı!.." , m_ui->searchedit_pos_no );

            return ADAK_FAIL;
        }
        sql_query.NEXT();
        m_pos_id      = sql_query.VALUE(0).toInt();
        m_hesap_no_id = sql_query.VALUE(1).toInt();
    }
    else if ( p_object EQ m_ui->checkbox_pos_no ) {

        if ( m_ui->checkbox_pos_no->isChecked() EQ false ) {
            m_ui->searchedit_pos_no->SET_TEXT( "" );
            m_ui->frame_pos_no->setDisabled(true);
        }
        else {
            m_ui->frame_pos_no->setEnabled(true);
            SET_FOCUS( m_ui->searchedit_pos_no );
        }
    }
    else if ( p_object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ false ) {
            m_ui->adakDate_bas_tarih->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_bts_tarih->SET_DATE( MALI_YIL_365_DATE() );
            m_ui->frame_tarih_araligi->setDisabled(true);
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled(true);
            SET_FOCUS( m_ui->adakDate_bas_tarih );
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

/*****************************************************/
/*          BNK_POS_EKSTRESI_BATCH::CHECK_RUN      */
/*****************************************************/

int BNK_POS_EKSTRESI_BATCH::CHECK_RUN()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarih, m_ui->adakDate_bts_tarih ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/*****************************************************/
/*          BNK_POS_EKSTRESI_BATCH::RUN_BATCH        */
/*****************************************************/

void BNK_POS_EKSTRESI_BATCH::RUN_BATCH()
{

    BNK_RV =  new BNK_RAPOR_VARS;

    BNK_RV->pos_id               =   m_pos_id;
    BNK_RV->banka_hesap_no_id    =   m_hesap_no_id;
    BNK_RV->baslangic_tarihi     =   m_ui->adakDate_bas_tarih->QDATE();
    BNK_RV->bitis_tarihi         =   m_ui->adakDate_bts_tarih->QDATE();
    BNK_RV->nakli_yekun          =   m_ui->checkbox_nakli_yekun->isChecked();

    QString log_detaylari = ( "Log Türü - Raporlar , Pos Ekstresi\nPos Üye İşyeri No  : " +
                              m_ui->searchedit_pos_no->GET_TEXT() + "\nTarih Aralığı  : " +
                              m_ui->adakDate_bas_tarih->QDATE().toString("dd MMMM yyyy") + " - " +
                              m_ui->adakDate_bts_tarih->QDATE().toString("dd MMMM yyyy"));

    if ( BNK_RV->nakli_yekun EQ 1 ) {
        log_detaylari.append("\nNakli Yekün");
    }

    DB->START_TRANSACTION   ();

    E9_LOG_KAYDI_EKLE       ( BANKA_MODULU , LOG_BNK_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari );

    DB->COMMIT_TRANSACTION  ();

    OPEN_REPORT_SHOWER      ( GET_BNK_POS_EKSTRESI( BNK_RV ), nativeParentWidget() );

    delete BNK_RV;
}

/***************************************************************/
/*          BNK_POS_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED        */
/***************************************************************/

void BNK_POS_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED(QWidget *,QLineEdit * line_edit)
{
    int pos_id = OPEN_BNK_POS_SEC(-1,this);
    if ( pos_id NE -1 ) {
        line_edit->setText ( BNK_GET_POS_NUMARASI ( pos_id ) );
    }
}
