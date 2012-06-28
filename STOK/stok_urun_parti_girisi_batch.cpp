#include "stok_urun_parti_girisi_batch_class.h"
#include "stok_urun_parti_girisi_batch_open.h"
#include "ui_stok_urun_parti_girisi_batch.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_STOK_URUN_PARTI_GIRISI_BATCH
***************************************************************************************/

void OPEN_STOK_URUN_PARTI_GIRISI_BATCH ( int * p_parti_kodu_id, int p_urun_id, int p_depo_id, QWidget * parent )
{
    STOK_URUN_PARTI_GIRISI_BATCH * F = new STOK_URUN_PARTI_GIRISI_BATCH ( p_parti_kodu_id, p_urun_id, p_depo_id, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   STOK_URUN_PARTI_GIRISI_BATCH::STOK_URUN_PARTI_GIRISI_BATCH
***************************************************************************************/

STOK_URUN_PARTI_GIRISI_BATCH::STOK_URUN_PARTI_GIRISI_BATCH ( int *p_parti_kodu_id, int p_urun_id, int p_depo_id,  QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui( new Ui::STOK_URUN_PARTI_GIRISI_BATCH )
{
    m_gelen_parti_id   = p_parti_kodu_id;
    m_urun_id          = p_urun_id;
    m_depo_id          = p_depo_id;

    m_ui->setupUi      ( this );    
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   STOK_URUN_PARTI_GIRISI_BATCH::SETUP_FORM
***************************************************************************************/

void STOK_URUN_PARTI_GIRISI_BATCH::SETUP_FORM ()
{

    SET_PAGE_TITLE    (tr("PARTİ ÜRÜN GİRİŞİ"));
    SET_SETTING_NAME  ("STOK_URUN_PARTI_GIRISI_BATCH");
    SET_HELP_PAGE     ("stok_urun_girisi.html");

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Kaydet" );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    m_ui->adakDate_son_kullanma_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_uretim_tarihi->MALI_YIL_KISITI_YOK( true );

    m_ui->adakDate_son_kullanma_tarihi->SET_DATE( QDate::currentDate().addYears( 1 ) );

    SET_AUTO_EXIT_BATCH( true );

    m_ui->lineEdit_parti_kodu->setMaxLength( 30 );



    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "stk_urunun_partileri",
                           "parti_kodu, urun_uretim_tarihi, urun_son_kul_tarihi",
                           "parti_id = :parti_id ");
    query.SET_VALUE      ( ":parti_id", *m_gelen_parti_id );


    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        m_ui->lineEdit_parti_kodu->setText( query.VALUE( 0 ).toString() );
        m_ui->adakDate_uretim_tarihi->SET_DATE( QDate::fromString( query.VALUE( 1 ).toString(), "yyyy.MM.dd") );
        m_ui->adakDate_son_kullanma_tarihi->SET_DATE( QDate::fromString( query.VALUE( 2 ).toString(), "yyyy.MM.dd") );
    }
    else {
        m_ui->lineEdit_parti_kodu->setText(m_ui->adakDate_uretim_tarihi->QDATE().toString("dd.MM.yyyy"));
        *m_gelen_parti_id = -1;
    }

     SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_parti_kodu );
}

/**************************************************************************************
                   STOK_URUN_PARTI_GIRISI_BATCH::CHECK_VAR
***************************************************************************************/

int STOK_URUN_PARTI_GIRISI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->lineEdit_parti_kodu ) {

        SQL_QUERY query ( DB );

        query.PREPARE_SELECT ( "stk_urunun_partileri", "parti_id", "parti_kodu = :parti_kodu "
                               "AND depo_id = :depo_id "
                               "AND urun_id = :urun_id");
        query.SET_VALUE      ( ":parti_kodu", m_ui->lineEdit_parti_kodu->text() );
        query.SET_VALUE      ( ":depo_id"   , m_depo_id    );
        query.SET_VALUE      ( ":urun_id"   , m_urun_id    );

        if ( query.SELECT() NE 0 ) {
            MSG_WARNING( "Bu Parti kodu zaten var.!", m_ui->lineEdit_parti_kodu );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_PARTI_GIRISI_BATCH::CHECK_RUN
***************************************************************************************/

int STOK_URUN_PARTI_GIRISI_BATCH::CHECK_RUN ()
{
    if ( m_ui->lineEdit_parti_kodu->text().isEmpty() EQ true ) {
        MSG_WARNING( "Parti kodu Boş bırakılamaz.!", m_ui->lineEdit_parti_kodu );
        return ADAK_FAIL;
    }
    if ( m_ui->adakDate_uretim_tarihi->QDATE() >= m_ui->adakDate_son_kullanma_tarihi->QDATE() ) {
        MSG_WARNING( "Üretim Tarihi, Son Kullanma Tarihinden büyük veya eşit olamaz.!", m_ui->adakDate_uretim_tarihi );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_PARTI_GIRISI_BATCH::RUN_BATCH
***************************************************************************************/

void STOK_URUN_PARTI_GIRISI_BATCH::RUN_BATCH ()
{
    DB->START_TRANSACTION();

    SQL_QUERY query( DB );

    if( *m_gelen_parti_id EQ -1 ) {
        query.PREPARE_INSERT ( "stk_urunun_partileri", "parti_id", "depo_id, urun_id, "
                               "parti_kodu, urun_uretim_tarihi, urun_son_kul_tarihi ");

        query.SET_VALUE(":depo_id"             , m_depo_id );
        query.SET_VALUE(":urun_id"             , m_urun_id );
        query.SET_VALUE(":parti_kodu"          , m_ui->lineEdit_parti_kodu->text() );
        query.SET_VALUE(":urun_uretim_tarihi"  , m_ui->adakDate_uretim_tarihi->DATE() );
        query.SET_VALUE(":urun_son_kul_tarihi" , m_ui->adakDate_son_kullanma_tarihi->DATE() );

        *m_gelen_parti_id = query.INSERT();
    }
    else {
        // update
        query.PREPARE_UPDATE( "stk_urunun_partileri", "parti_id",
                              "parti_kodu, urun_uretim_tarihi, urun_son_kul_tarihi ", " parti_id = :parti_id");

        query.SET_VALUE(":parti_kodu"          , m_ui->lineEdit_parti_kodu->text() );
        query.SET_VALUE(":urun_uretim_tarihi"  , m_ui->adakDate_uretim_tarihi->DATE() );
        query.SET_VALUE(":urun_son_kul_tarihi" , m_ui->adakDate_son_kullanma_tarihi->DATE() );

        query.SET_VALUE(":parti_id", *m_gelen_parti_id );

        query.UPDATE();
    }

    DB->COMMIT_TRANSACTION();

    close();
}

/**************************************************************************************
                   STOK_URUN_PARTI_GIRISI_BATCH::CHECK_EXIT
***************************************************************************************/

int STOK_URUN_PARTI_GIRISI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}
