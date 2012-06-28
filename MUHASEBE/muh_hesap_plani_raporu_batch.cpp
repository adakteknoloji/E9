#include "muh_hesap_plani_raporu_batch_class.h"
#include "muh_hesap_plani_raporu_batch_open.h"
#include "ui_muh_hesap_plani_raporu_batch.h"
#include "print.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_log.h"
#include "muh_hesap_arama_open.h"
#include "muh_hesap_plani_raporu_open.h"

extern ADAK_SQL * DB;

/**************************************************************************************
              OPEN_MUH_HESAP_PLANI_RAPORU_BATCH
***************************************************************************************/

void OPEN_MUH_HESAP_PLANI_RAPORU_BATCH ( QWidget * parent )
{
    MUH_HESAP_PLANI_RAPORU_BATCH * B = new MUH_HESAP_PLANI_RAPORU_BATCH ( parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
      MUH_HESAP_PLANI_RAPORU_BATCH::MUH_HESAP_PLANI_RAPORU_BATCH
***************************************************************************************/

MUH_HESAP_PLANI_RAPORU_BATCH::MUH_HESAP_PLANI_RAPORU_BATCH ( QWidget *parent ) : BATCH_KERNEL ( parent ), m_ui ( new Ui::MUH_HESAP_PLANI_RAPORU_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   MUH_HESAP_PLANI_RAPORU_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_HESAP_PLANI_RAPORU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ( "muhasebe-raporlari" );
    SET_PAGE_TITLE      ( tr ( "MUH - HESAP PLANI" ) );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME             ( "MUH_HESAP_PLANI_RAPORU_BATCH" );
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->search_edit_bas_hesap_kodu );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" );

    if ( sql_query.SELECT("tam_hesap_kodu ASC" , 0, 1) > 0 ) {

         sql_query.NEXT();
        m_ui->search_edit_bas_hesap_kodu->SET_TEXT ( sql_query.VALUE(0).toString() );
        m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(1).toString() );

    }

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" );

    if ( sql_query.SELECT("tam_hesap_kodu DESC" , 0, 1) NE 0 ) {
         sql_query.NEXT();

        m_ui->search_edit_bts_hesap_kodu->SET_TEXT ( sql_query.VALUE(0).toString() );
        m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(1).toString() );
    }
}

/**************************************************************************************
                   MUH_HESAP_PLANI_RAPORU_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_HESAP_PLANI_RAPORU_BATCH::CHECK_VAR ( QObject * object )
{
    SQL_QUERY       sql_query ( DB ) ;

    if ( object EQ m_ui->search_edit_bas_hesap_kodu ) {

        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi" , "tam_hesap_kodu = :tam_hesap_kodu");

        sql_query.SET_VALUE ( ":tam_hesap_kodu" , m_ui->search_edit_bas_hesap_kodu->GET_TEXT() );

        if ( sql_query.SELECT() EQ 0 ) {
             MSG_WARNING( tr ( "Hesap Kodu Bulunamadı!.." ), m_ui->search_edit_bas_hesap_kodu );

            return ADAK_FAIL;
        }
        sql_query.NEXT();
        m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
    }
    if ( object EQ m_ui->search_edit_bts_hesap_kodu ) {

        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi" , "tam_hesap_kodu = :tam_hesap_kodu");
        sql_query.SET_VALUE      ( ":tam_hesap_kodu" , m_ui->search_edit_bts_hesap_kodu->GET_TEXT() );

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING( tr ( "Hesap Kodu Bulunamadı!.." ), m_ui->search_edit_bts_hesap_kodu );

            return ADAK_FAIL;
        }
        sql_query.NEXT();
        m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_PLANI_RAPORU_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_HESAP_PLANI_RAPORU_BATCH::CHECK_RUN()
{
    if ( QString::compare ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT() , m_ui->search_edit_bts_hesap_kodu->GET_TEXT() ) > 0  ) {
        MSG_WARNING( tr ( "Başlangıç hesap kodu ,bitiş hesap kodundan büyük olamaz!..." ), m_ui->search_edit_bas_hesap_kodu );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_PLANI_RAPORU_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_HESAP_PLANI_RAPORU_BATCH::RUN_BATCH()
{
    MUH_RAPOR_VARS * MUH_RV   = new MUH_RAPOR_VARS;

    MUH_RV->bas_hesap_kodu  =  m_ui->search_edit_bas_hesap_kodu->GET_TEXT();
    MUH_RV->bts_hesap_kodu  =  m_ui->search_edit_bts_hesap_kodu->GET_TEXT();

    QString log_detaylari = QObject::tr ( "Hesap Planı Raporu , \nHesap Aralığı : " ) +
                            m_ui->search_edit_bas_hesap_kodu->GET_TEXT() + " " + m_ui->lineedit_bas_hesap_ismi->text()
                            + " - " + m_ui->search_edit_bts_hesap_kodu->GET_TEXT() + " " +
                            m_ui->lineedit_bts_hesap_ismi->text() ;

    DB->START_TRANSACTION();
    E9_LOG_KAYDI_EKLE  (  MUHASEBE_MODULU ,LOG_MUH_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI, log_detaylari );
    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER ( GET_HESAP_PLANI_RAPORU(MUH_RV), nativeParentWidget());

    delete MUH_RV;
}

/**************************************************************************************
                   MUH_HESAP_PLANI_RAPORU_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void MUH_HESAP_PLANI_RAPORU_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget, QLineEdit * )
{    
    SQL_QUERY sql_query ( DB );

    if ( widget EQ m_ui->search_edit_bts_hesap_kodu ) {
        int hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->search_edit_bts_hesap_kodu->GET_TEXT(),this );

        if ( hesap_id EQ -1 ) {
            return;
        }

        sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" , "hesap_id = :hesap_id");

        sql_query.SET_VALUE      ( ":hesap_id" , hesap_id );
        if (sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
            m_ui->search_edit_bts_hesap_kodu->SET_TEXT ( sql_query.VALUE(0).toString() );
            m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(1).toString() );
        }
    }
    else {
        int hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT(),this );

        if ( hesap_id EQ -1 ) {
            return;
        }

        sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" , "hesap_id = :hesap_id");

        sql_query.SET_VALUE      ( ":hesap_id" , hesap_id );
        if (sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
            m_ui->search_edit_bas_hesap_kodu->SET_TEXT ( sql_query.VALUE(0).toString() );
            m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(1).toString() );
        }
    }


}







