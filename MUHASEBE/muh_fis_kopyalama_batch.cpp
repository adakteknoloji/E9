#include "muh_fis_kopyalama_batch_class.h"
#include "muh_fis_kopyalama_batch_open.h"
#include "ui_muh_fis_kopyalama_batch.h"
#include "adak_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_log.h"
#include "muh_fis_arama_open.h"
#include "muh_muhasebe_fisi_open.h"
#include "muh_enum.h"
#include "muh_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_MUH_FIS_KOPYALAMA_BATCH
***************************************************************************************/

void OPEN_MUH_FIS_KOPYALAMA_BATCH ( int fis_id, QWidget * parent )
{
    MUH_FIS_KOPYALAMA_BATCH * B = new MUH_FIS_KOPYALAMA_BATCH ( fis_id, parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   MUH_FIS_KOPYALAMA_BATCH::MUH_FIS_KOPYALAMA_BATCH
***************************************************************************************/

MUH_FIS_KOPYALAMA_BATCH::MUH_FIS_KOPYALAMA_BATCH ( int fis_id, QWidget * parent ) : BATCH_KERNEL ( parent ),  m_ui ( new Ui::MUH_FIS_KOPYALAMA_BATCH )
{
    m_fis_id   = fis_id;
    m_parent   = parent;

    M_FIS        = new MUH_FIS_STRUCT;
    M_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

    m_ui->setupUi      ( this );

    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   MUH_FIS_KOPYALAMA_BATCH::CHECK_VAR
***************************************************************************************/

void MUH_FIS_KOPYALAMA_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ( tr ( "muh_fis_kopyalama.html" ) );
    SET_PAGE_TITLE      ( tr ( "MUH - MUHASEBE FİŞİ KOPYALAMA" ) );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME    ( "MUH_FIS_KOPYALAMA" );

    MUH_FIS_BILGILERINI_OKU( M_FIS, m_fis_id );

    SET_AUTO_EXIT_BATCH(true);

    m_ui->search_edit_fis_no->SET_TEXT               ( QVariant( M_FIS->fis_no ).toString() );
    m_ui->adakDate_kopyalanacak_fis_tarihi->SET_DATE( M_FIS->fis_tarihi );
    m_ui->adakDate_yeni_fis_tarihi->SET_DATE        ( M_FIS->fis_tarihi );
    m_ui->textedit_aciklama->setPlainText           ( M_FIS->aciklama );
    m_ui->commaedit_top_borc->SET_DOUBLE            ( M_FIS->toplam_borc );
    m_ui->commaedit_top_alacak->SET_DOUBLE          ( M_FIS->toplam_alacak );

    m_fis_turu = M_FIS->fis_turu;

    int siradaki_fis_no = MUH_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_yeni_fis_tarihi->DATE() );
    m_ui->lineEdit_yeni_fis_no->setText ( QVariant ( siradaki_fis_no ).toString() );

    SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Fiş Kopyala" ) );  
    SET_FIRST_FOCUS_WIDGET       ( m_ui->search_edit_fis_no   );

    m_ui->adakDate_kopyalanacak_fis_tarihi->setEnabled( false );
    m_ui->search_edit_fis_no->setEnabled    ( false );
    m_ui->textedit_aciklama->setEnabled     ( false );
    m_ui->commaedit_top_alacak->setEnabled  ( false );
    m_ui->commaedit_top_borc->setEnabled    ( false );

}

/**************************************************************************************
                   MUH_FIS_KOPYALAMA_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_FIS_KOPYALAMA_BATCH::CHECK_VAR ( QObject * p_object )
{
    SQL_QUERY   query ( DB );

    if ( p_object EQ m_ui->adakDate_yeni_fis_tarihi ) {

        if ( m_fis_turu EQ ENUM_ACILIS_FISI OR m_fis_turu EQ ENUM_KAPANIS_FISI ) {
            MSG_WARNING( tr ( "Fiş Tarihini değiştiremezsiniz!.." ), NULL );
            return ADAK_FAIL_UNDO;
        }

        int siradaki_fis_no = MUH_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_yeni_fis_tarihi->DATE() );
        m_ui->lineEdit_yeni_fis_no->setText ( QVariant (  siradaki_fis_no ).toString() );
    }

    else if ( p_object EQ m_ui->search_edit_fis_no ) {

        if ( m_ui->search_edit_fis_no->GET_TEXT().isEmpty() EQ true ) {
            m_fis_id   = 0;
            m_fis_turu = -1;
            m_ui->commaedit_top_borc->clear();
            m_ui->commaedit_top_alacak->clear();
            m_ui->textedit_aciklama->clear();
            return ADAK_OK;
        }

        query.PREPARE_SELECT("muh_fisler" , "fis_id ,fis_tarihi , aciklama , toplam_borc, toplam_alacak, "
                             "kasa_hesabi_id, fis_turu " , "fis_no = :fis_no AND fis_tarihi = :fis_tarihi");


        query.SET_VALUE ( ":fis_no"    , m_ui->search_edit_fis_no->GET_TEXT().toInt() );
        query.SET_VALUE ( ":fis_tarihi", m_ui->adakDate_kopyalanacak_fis_tarihi->DATE() );

        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING( tr ( "Fiş bulunamadı" ), m_ui->search_edit_fis_no );

            return ADAK_FAIL_UNDO;
        }

        query.NEXT();

        m_fis_id = query.VALUE(0).toInt();

        m_ui->adakDate_kopyalanacak_fis_tarihi->SET_DATE(  query.VALUE(1).toString()  );
        m_ui->textedit_aciklama->setPlainText           ( query.VALUE(2).toString() );
        m_ui->commaedit_top_borc->SET_DOUBLE            ( query.VALUE(3).toDouble() );
        m_ui->commaedit_top_alacak->SET_DOUBLE          ( query.VALUE(4).toDouble() ) ;

        int siradaki_fis_no = MUH_SIRADAKI_FIS_NO_AL    ( m_ui->adakDate_yeni_fis_tarihi->DATE() );
        m_ui->lineEdit_yeni_fis_no->setText ( QVariant  ( siradaki_fis_no ).toString() );

        m_fis_turu = query.VALUE(6).toInt();

    }
    else if ( p_object EQ m_ui->lineEdit_yeni_fis_no ) {

        query.PREPARE_SELECT("muh_fisler" , "fis_id" , "fis_no = :fis_no "
                             "AND fis_tarihi = :fis_tarihi");

        query.SET_VALUE ( ":fis_no",       m_ui->lineEdit_yeni_fis_no->text() );
        query.SET_VALUE ( ":fis_tarihi",   m_ui->adakDate_yeni_fis_tarihi->DATE() );

        if ( query.SELECT() NE 0 ) {
            MSG_WARNING( tr ( "Bu fiş numarasına sahip başka bir fiş vardır. Bu numaraya fiş kopyalayamazsınız!.." ), m_ui->lineEdit_yeni_fis_no  );

            return ADAK_FAIL_UNDO;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_FIS_KOPYALAMA_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_FIS_KOPYALAMA_BATCH::CHECK_RUN()
{

    if ( m_ui->search_edit_fis_no->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( "Kopyalanacak fişi seçmelisiniz!..", m_ui->search_edit_fis_no );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_FIS_KOPYALAMA_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_FIS_KOPYALAMA_BATCH::RUN_BATCH()
{

    DB->START_TRANSACTION();
    SQL_QUERY query ( DB );

    MUH_FIS_STRUCT        *  NEW_MUH_FIS        = new MUH_FIS_STRUCT;
    MUH_FIS_SATIRI_STRUCT *  NEW_MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

    MUH_CLEAR_FIS_STRUCT(NEW_MUH_FIS);


    int siradaki_fis_no = MUH_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_yeni_fis_tarihi->DATE() );

    if ( siradaki_fis_no NE m_ui->lineEdit_yeni_fis_no->text().toInt() ) {
        m_ui->lineEdit_yeni_fis_no->setText ( QVariant ( siradaki_fis_no ).toString() );
    }

    NEW_MUH_FIS->fis_no           = m_ui->lineEdit_yeni_fis_no->text().toInt();
    NEW_MUH_FIS->fis_tarihi       = m_ui->adakDate_yeni_fis_tarihi->DATE();
    NEW_MUH_FIS->aciklama         = m_ui->textedit_new_aciklama->toPlainText();
    NEW_MUH_FIS->kasa_hesabi_id   = M_FIS->kasa_hesabi_id;
    NEW_MUH_FIS->fis_turu         = M_FIS->fis_turu;
    NEW_MUH_FIS->modul_id         = MUHASEBE_MODULU;
    NEW_MUH_FIS->program_id       = M_FIS->program_id;
    NEW_MUH_FIS->base_fis_id      = 0;

    int record_id  = MUH_FIS_EKLE( NEW_MUH_FIS );


    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_satiri_id" , "fis_id =:fis_id AND gizli_satir_mi = :gizli_satir_mi ");

    query.SET_VALUE( ":fis_id"         , m_fis_id );
    query.SET_VALUE( ":gizli_satir_mi" , 0);

    if( query.SELECT() EQ  0 ){
        DB->CANCEL_TRANSACTION();
        MSG_WARNING( tr ( "Kopyalanan fiş satırı bulunamadı" ), NULL );
        return;
    }

    while ( query.NEXT() EQ true ) {

        int fis_satiri_id = query.VALUE( 0 ).toInt();

        MUH_CLEAR_FIS_SATIRI_STRUCT( NEW_MUH_FIS_SATIRI);

        MUH_FIS_SATIRINI_OKU( NEW_MUH_FIS_SATIRI, fis_satiri_id );

        NEW_MUH_FIS_SATIRI->fis_id = record_id;

        MUH_FIS_SATIRI_EKLE( NEW_MUH_FIS, NEW_MUH_FIS_SATIRI );

    }

    if ( record_id < 1 ) {
        MSG_WARNING( tr ( "Fiş Kopyalanamadı." ), NULL );
    }

    m_ui->search_edit_fis_no->SET_TEXT ( "" );
    m_ui->adakDate_yeni_fis_tarihi->SET_DATE(MALI_YIL_FIRST_DATE() );
    m_ui->commaedit_top_alacak->clear();
    m_ui->commaedit_top_borc->clear();
    m_ui->textedit_aciklama->clear();
    m_ui->textedit_new_aciklama->clear();

    MSG_INFO("Fiş Kopyalandı.", NULL );

    QString log_detaylari = QObject::tr ( "Log Türü - Fiş Kopyalama ," );
    log_detaylari.append ( MUH_GET_FIS_TURLERI_STRING ( M_FIS->fis_turu ) + "," + M_FIS->fis_tarihi + ",Fiş No : "  +
                           QVariant( M_FIS->fis_no ).toString() + ",Toplam Borç : " +
                           VIRGUL_EKLE ( QVariant ( ROUND ( M_FIS->toplam_borc ) ).toString() ) + ",Toplam Alacak : " +
                           VIRGUL_EKLE ( QVariant ( ROUND ( M_FIS->toplam_alacak ) ).toString() ) );

    log_detaylari.append ( "--Fişi Kopyalandı" );

    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_FIS_KOPYALAMA , LOG_ISLEM_ADD , log_detaylari );

    DB->COMMIT_TRANSACTION  ();

    delete NEW_MUH_FIS;
    delete NEW_MUH_FIS_SATIRI;
}

/**************************************************************************************
                   MUH_FIS_KOPYALAMA_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void MUH_FIS_KOPYALAMA_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget, QLineEdit * lineEdit  )
{
    Q_UNUSED ( widget );

    int fis_id = OPEN_MUH_FIS_ARAMA ( 0, MUHASEBE_MODULU, -1, ENUM_MUHASEBE_FISI, this );

    if ( fis_id < 1 ) {
        return;
    }

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("muh_fisler" , "fis_no" , "fis_id = :fis_id");

    query.SET_VALUE      ( ":fis_id" , fis_id );
    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING( tr ( "Fiş bulunamadı"), NULL );
        SET_FOCUS( m_ui->lineEdit_yeni_fis_no );
        return;
    }

    query.NEXT();
    lineEdit->setText ( query.VALUE(0).toString() );
    m_fis_id = fis_id;
}
