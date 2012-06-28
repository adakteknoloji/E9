#include <QMessageBox>
#include <QDate>
#include "dvz_kur_guncelle_batch_class.h"
#include "ui_dvz_kur_guncelle_batch.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "kernel_utils.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_DVZ_KUR_GUNCELLE_BATCH
***************************************************************************************/

void OPEN_DVZ_KUR_GUNCELLE_BATCH ( QWidget * parent )
{
    DVZ_KUR_GUNCELLE_BATCH * F = new DVZ_KUR_GUNCELLE_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   DVZ_KUR_GUNCELLE_BATCH::DVZ_KUR_GUNCELLE_BATCH
***************************************************************************************/

DVZ_KUR_GUNCELLE_BATCH::DVZ_KUR_GUNCELLE_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::DVZ_KUR_GUNCELLE_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   DVZ_KUR_GUNCELLE_BATCH::SETUP_FORM
***************************************************************************************/

void DVZ_KUR_GUNCELLE_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "DVZ - KURLARI GÜNCELLE" ) );
    SET_SETTING_NAME  ("DVZ_KUR_GUNCELLE_BATCH");
    SET_HELP_PAGE     ("doviz-islemleri_kurlari-guncelle");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_bas_tarihi );
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Kur Güncelle" );

    m_ui->adakDate_bas_tarihi->SET_DATE(QDate::currentDate());
    m_ui->adakDate_bitis_tarihi->SET_DATE(QDate::currentDate());
}

/**************************************************************************************
                   DVZ_KUR_GUNCELLE_BATCH::CHECK_VAR
***************************************************************************************/

int DVZ_KUR_GUNCELLE_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED( object );
    return ADAK_OK;
}

/**************************************************************************************
                   DVZ_KUR_GUNCELLE_BATCH::CHECK_RUN
***************************************************************************************/

int DVZ_KUR_GUNCELLE_BATCH::CHECK_RUN ()
{
    if( m_ui->adakDate_bas_tarihi->QDATE() > m_ui->adakDate_bitis_tarihi->QDATE() ) {
        MSG_WARNING( tr("Kur Başlangıç Tarihi , Kur Bitiş Tarihinden büyük olamaz."), m_ui->adakDate_bas_tarihi );
        return ADAK_FAIL;
    }
    if( m_ui->adakDate_bitis_tarihi->QDATE() < m_ui->adakDate_bas_tarihi->QDATE() ) {
        MSG_WARNING( tr("Kur Bitiş Tarihi , Kur Başlangıç Tarihinden Küçük Olamaz."), m_ui->adakDate_bitis_tarihi );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DVZ_KUR_GUNCELLE_BATCH::RUN_BATCH
***************************************************************************************/

void DVZ_KUR_GUNCELLE_BATCH::RUN_BATCH ()
{
    SQL_QUERY sql_query ( DB );

    QSplashScreen * splash = CREATE_SPLASH_SCREEN();

    QDate kur_tarihi = m_ui->adakDate_bas_tarihi->QDATE();

    while ( kur_tarihi <= m_ui->adakDate_bitis_tarihi->QDATE() ) {

        sql_query.PREPARE_SELECT("dvz_kurlar" , "kur_id" , "kur_tarihi = :kur_tarihi");

        sql_query.SET_VALUE      ( ":kur_tarihi", kur_tarihi.toString ("yyyy.MM.dd" ) );

        if ( sql_query.SELECT("kur_id ASC" , 0, 1 ) EQ 0 ) {
            QString mesaj = QObject::tr("%1 Tarihinin Kur Bilgileri Güncelleştiriliyor...").arg(kur_tarihi.toString("dd MMMM yyyy"));

            DVZ_DOVIZ_SPLASH_ON(splash,mesaj);
            qApp->processEvents();

            DVZ_DOVIZLERI_GUNCELLE( ENUM_TCMB_XML, kur_tarihi);

        }
        kur_tarihi = kur_tarihi.addDays(1);
    }

    DVZ_DOVIZ_SPLASH_OFF( splash );

    QMessageBox::information(this, "Bilgi" , QObject::tr("Döviz Kurları Güncellendi."));
}

/**************************************************************************************
                   DVZ_KUR_GUNCELLE_BATCH::CHECK_EXIT
***************************************************************************************/

int DVZ_KUR_GUNCELLE_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}


