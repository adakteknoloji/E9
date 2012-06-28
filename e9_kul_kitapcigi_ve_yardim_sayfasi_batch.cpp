#include "e9_kul_kitapcigi_ve_yardim_sayfasi_batch_open.h"
#include "e9_kul_kitapcigi_ve_yardim_sayfasi_batch_class.h"
#include "ui_e9_kul_kitapcigi_ve_yardim_sayfasi_batch.h"
#include "print.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH
***************************************************************************************/

void OPEN_E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH ( int p_kitapcik_mi, QWidget * parent )
{
    E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH * F = new E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH ( p_kitapcik_mi, parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH
***************************************************************************************/

E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH ( int p_kitapcik_mi, QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH )
{
    m_kitapcik_mi = p_kitapcik_mi;
    m_ui->setupUi      ( this );    
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::SETUP_FORM
***************************************************************************************/

void E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::SETUP_FORM ()
{

    SET_FIRST_FOCUS_WIDGET ( m_ui->webView );
    SET_SETTING_NAME  ("E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH");

    if( m_kitapcik_mi EQ 1 ) {
        SET_PAGE_TITLE(tr("KULLANIM KITAPÇIĞI"));
        m_ui->webView->load( QUrl("http://www.e9ticaripaket.com/kullanim-kitapcigi") );
    }
    else {
        SET_PAGE_TITLE(tr("YARDIM (SIK SORULAN SORULAR)"));
         m_ui->webView->load( QUrl("http://www.e9ticaripaket.com/sik-sorulan-sorular") );
    }

}

/**************************************************************************************
                   E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::CHECK_VAR
***************************************************************************************/

int E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED( object );
    return ADAK_OK;
}

/**************************************************************************************
                   E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::CHECK_RUN
***************************************************************************************/

int E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::RUN_BATCH
***************************************************************************************/

void E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::RUN_BATCH ()
{
}

/**************************************************************************************
                   E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::CHECK_EXIT
***************************************************************************************/

int E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}
