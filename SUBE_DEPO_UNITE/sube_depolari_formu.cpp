#include "adak_sql.h"
#include "sube_depolari_formu_class.h"
#include "ui_sube_depolari_formu.h"
#include "sube_console_utils.h"
#include "e9_log.h"
 
#include "sube_depo_secimi_open.h"


extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_SUBE_DEPOLARI_FORMU
***************************************************************************************/

void OPEN_SUBE_DEPOLARI_FORMU ( int p_sube_id, int p_depo_id, QWidget * p_parent )
{
    SUBE_DEPOLARI_FORMU * F = new SUBE_DEPOLARI_FORMU ( p_sube_id, p_depo_id, p_parent );
    F->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::SUBE_DEPOLARI_FORMU
***************************************************************************************/

SUBE_DEPOLARI_FORMU:: SUBE_DEPOLARI_FORMU ( int p_sube_id, int p_depo_id, QWidget * p_parent ) : FORM_KERNEL ( p_parent ),
m_ui ( new Ui:: SUBE_DEPOLARI_FORMU ), m_sube_id ( p_sube_id )
{
    m_sube_id = p_sube_id;
    m_depo_id = p_depo_id;

    m_ui->setupUi       ( this );
    START_FORM_KERNEL   ( this, DB );
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::SETUP_FORM
***************************************************************************************/

void SUBE_DEPOLARI_FORMU::SETUP_FORM()
{
    SET_HELP_PAGE       ( "depolar" );
    SET_SETTING_NAME    ( "SUBE_DEPOLARI_FORMU" );

    REGISTER_BUTTONS_WIDGET  ( m_ui->navigation_buttons_widget , true );

    SET_PAGE_TITLE      ( tr ( "ŞUBE - ŞUBENİN DEPOLARI" ) );

    SET_FIRST_FOCUS_WIDGET   ( m_ui->lineEdit_depo_kodu );
    AUTOSTART_FORM_KERNEL    ( m_depo_id );
     

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "sub_subeler","sube_kodu,sube_adi","sube_id = :sube_id" );
    sql_query.SET_VALUE      ( ":sube_id" , m_sube_id );

    m_sube_kodu_adi = "Şube Kodu / Adı :" ;
    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        m_sube_kodu_adi.append ( sql_query.VALUE(0).toString() + "  " + sql_query.VALUE(1).toString());
    }
    m_ui->lineEdit_depo_kodu->setMaxLength(10);
    m_ui->lineEdit_depo_adi->setMaxLength(20);
    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH(512);
    m_ui->limitedTextEdit_adres->SET_MAX_STRING_LENGTH(512);
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void SUBE_DEPOLARI_FORMU::CLEAR_FORM_MEMBERS()
{

}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::NEW_RECORD
***************************************************************************************/

void  SUBE_DEPOLARI_FORMU::NEW_RECORD()
{

}
/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::GET_RECORD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::GET_RECORD ( int p_depo_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "sub_depolar","depo_kodu, depo_adi, depo_adresi, aciklama ",
                               "sube_id = :sube_id AND depo_id = :depo_id" );


    sql_query.SET_VALUE      ( ":depo_id", p_depo_id );
    sql_query.SET_VALUE      ( ":sube_id", m_sube_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->lineEdit_depo_kodu->setText        ( sql_query.VALUE(0).toString() );
    m_ui->lineEdit_depo_adi->setText         ( sql_query.VALUE(1).toString() );
    m_ui->limitedTextEdit_adres->setPlainText( sql_query.VALUE(2).toString() );
    m_ui->limitedTextEdit_aciklama->setPlainText ( sql_query.VALUE(3).toString() );

    return ADAK_OK;

}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::CHECK_VAR
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->limitedTextEdit_aciklama ) {
        if ( m_ui->limitedTextEdit_aciklama->toPlainText().size() > 512 ) {
            MSG_WARNING( tr ( "Açıklama 512 karakterden daha uzun olamaz" ), m_ui->limitedTextEdit_aciklama );

            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ m_ui->limitedTextEdit_adres ) {

        if ( m_ui->limitedTextEdit_adres->toPlainText().size() > 512 ) {
            MSG_WARNING( tr ( "Adres 512 karakterden daha uzun olamaz" ), m_ui->limitedTextEdit_adres );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::CHECK_EMPTY
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->lineEdit_depo_kodu->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Depo kodu boş bırakılamaz." ), m_ui->lineEdit_depo_kodu );

        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_depo_adi->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Depo adı boş bırakılamaz." ), m_ui->lineEdit_depo_adi );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::CHECK_ADD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::CHECK_ADD ()
{    
    int depo_id = SUBE_GET_DEPO_ID( m_ui->lineEdit_depo_kodu->text() );

    if ( depo_id NE 0 ) {
        MSG_WARNING(  tr ( "Bu depo kodu ile kayıtlı başka bir depo var. "
                                                          "Lütfen depo kodunu değiştiriniz" ), NULL );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::ADD_RECORD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::ADD_RECORD ()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_INSERT ( "sub_depolar","depo_id", "depo_kodu, depo_adi, depo_adresi, "
                               "aciklama, sube_id");

    sql_query.SET_VALUE      ( ":depo_kodu"    , m_ui->lineEdit_depo_kodu->text());
    sql_query.SET_VALUE      ( ":depo_adi"     , m_ui->lineEdit_depo_adi->text());
    sql_query.SET_VALUE      ( ":depo_adresi"  , m_ui->limitedTextEdit_adres->toPlainText());
    sql_query.SET_VALUE      ( ":aciklama"     , m_ui->limitedTextEdit_aciklama->toPlainText());
    sql_query.SET_VALUE      ( ":sube_id"      , m_sube_id );

    int depo_id = sql_query.INSERT();


    QString log_detaylari = m_sube_kodu_adi + " , Depo Kodu / Adı" + m_ui->lineEdit_depo_kodu->text()
                            + "  " + m_ui->lineEdit_depo_adi->text();

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_SUBELER , LOG_ISLEM_ADD , log_detaylari );

    return depo_id;
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::CHECK_UPDATE
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::CHECK_UPDATE ( int p_depo_id )
{
    int depo_id = SUBE_GET_DEPO_ID( m_ui->lineEdit_depo_kodu->text() );
    if ( depo_id NE 0 ) {
        if ( depo_id NE p_depo_id ) {
            MSG_WARNING(  tr ( "Bu depo kodu ile kayıtlı başka bir depo var. "
                                                             "Lütfen depo kodunu değiştiriniz" ), NULL );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::UPDATE_RECORD
***************************************************************************************/

void SUBE_DEPOLARI_FORMU::UPDATE_RECORD ( int p_depo_id )
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_UPDATE ( "sub_depolar","depo_id","depo_kodu, depo_adi , "
                               "depo_adresi , aciklama, sube_id  ",
                               "depo_id = :depo_id" );

    sql_query.SET_VALUE      ( ":depo_kodu"   , m_ui->lineEdit_depo_kodu->text());
    sql_query.SET_VALUE      ( ":depo_adi"    , m_ui->lineEdit_depo_adi->text());
    sql_query.SET_VALUE      ( ":depo_adresi" , m_ui->limitedTextEdit_adres->toPlainText());
    sql_query.SET_VALUE      ( ":aciklama"    , m_ui->limitedTextEdit_aciklama->toPlainText() );
    sql_query.SET_VALUE      ( ":sube_id"     , m_sube_id );
    sql_query.SET_VALUE      ( ":depo_id"     , p_depo_id );

    sql_query.UPDATE();

    QString log_detaylari = m_sube_kodu_adi + " , Depo Kodu / Adı" + m_ui->lineEdit_depo_kodu->text()
                            + "  " + m_ui->lineEdit_depo_adi->text();

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_SUBELER , LOG_ISLEM_UPDATE , log_detaylari );
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::CHECK_DELETE
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::CHECK_DELETE (  int p_depo_id )
{
    if ( p_depo_id EQ 1 ) {
        MSG_WARNING( "Silmek istediğiniz depo merkez depodur.Silemezsiniz.", NULL);
        return ADAK_FAIL;
    }

    SQL_QUERY query (DB);

    query.PREPARE_SELECT("sub_uniteler","depo_id","unite_id=:unite_id");
    query.SET_VALUE     (":unite_id",SUBE_GET_UNITE_ID());

    int user_depo_id = -1;

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        user_depo_id = query.VALUE(0).toInt();
        if ( p_depo_id EQ user_depo_id){
            MSG_WARNING( "Kendi deponuzu silemezsiniz.",NULL);
            return ADAK_FAIL;
        }
    }

    query.PREPARE_SELECT("stk_fisler","depo_id","depo_id=:depo_id");
    query.SET_VALUE     (":depo_id",p_depo_id);

    if (query.SELECT() > 0) {
        MSG_WARNING( "Stok da işlem görmüş depoyu silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("stk_depodaki_urunler","depo_id","depo_id=:depo_id");
    query.SET_VALUE     (":depo_id",p_depo_id);

    if (query.SELECT() > 0) {
        MSG_WARNING( "Stok da işlem görmüş depoyu silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("stk_urunun_partileri","depo_id","depo_id=:depo_id");
    query.SET_VALUE     (":depo_id",p_depo_id);

    if (query.SELECT() > 0) {
        MSG_WARNING( "Stok da işlem görmüş depoyu silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("stk_urunun_seri_nolari","depo_id","depo_id=:depo_id");
    query.SET_VALUE     (":depo_id",p_depo_id);

    if (query.SELECT() > 0) {
        MSG_WARNING( "Stok da işlem görmüş depoyu silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("sub_uniteler","depo_id","depo_id=:depo_id");
    query.SET_VALUE     (":depo_id",p_depo_id);

    if (query.SELECT() > 0) {
        MSG_WARNING( "Depoya kayıtlı üniteler var.Depoyu silmek için önce üniteleri silmelisiniz.",NULL);
        return ADAK_FAIL;
    }

    int msg_secim = MSG_YES_NO( tr ( "Seçili depo kayıtlardan silinecektir. Emin misiniz?" ), NULL );

    if ( msg_secim EQ ADAK_NO ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::DELETE_RECORD
***************************************************************************************/

void SUBE_DEPOLARI_FORMU::DELETE_RECORD ( int p_depo_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "sub_depolar","depo_kodu,depo_adi","depo_id = :depo_id" );
    sql_query.SET_VALUE      ( ":depo_id"  , p_depo_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString depo_kodu = sql_query.VALUE(0).toString();
    QString depo_adi  = sql_query.VALUE(1).toString();

    sql_query.PREPARE_DELETE ( "sub_depolar","depo_id = :depo_id" );
    sql_query.SET_VALUE      ( ":depo_id", p_depo_id );
    sql_query.DELETE();

    QString log_detaylari = m_sube_kodu_adi + " , Depo Kodu / Adı" + depo_kodu
                            + "  " + depo_adi;

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_SUBELER , LOG_ISLEM_DELETE , log_detaylari );
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::SELECT_RECORD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::SELECT_RECORD()
{
    return SUBE_DEPO_SEC ( m_sube_id, NULL );
}


/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::FIND_RECORD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::FIND_RECORD()
{
     return SUBE_GET_DEPO_ID( m_ui->lineEdit_depo_kodu->text(), m_sube_id );
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_depolar ","depo_id ",
                               "sube_id = :sube_id" );
    sql_query.SET_VALUE            ( ":sube_id", m_sube_id );

    if ( sql_query.SELECT("depo_kodu ASC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_depolar","depo_id","sube_id = :sube_id ");

    sql_query.SET_VALUE            ( ":sube_id", m_sube_id );
    if ( sql_query.SELECT("depo_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_depolar","depo_id","depo_kodu > :depo_kodu AND sube_id = :sube_id ");

    sql_query.SET_VALUE            ( ":depo_kodu", m_ui->lineEdit_depo_kodu->text() );
    sql_query.SET_VALUE            ( ":sube_id"  , m_sube_id );

    if ( sql_query.SELECT("depo_kodu ASC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::FIND_PREV_RECORD ()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_depolar","depo_id ",
                                     "depo_kodu < :depo_kodu AND sube_id = :sube_id ");


    sql_query.SET_VALUE            ( ":depo_kodu", m_ui->lineEdit_depo_kodu->text() );
    sql_query.SET_VALUE            ( ":sube_id"  , m_sube_id );

    if ( sql_query.SELECT("depo_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::LOCK_RECORD
***************************************************************************************/

int SUBE_DEPOLARI_FORMU::LOCK_RECORD ( int p_depo_id )
{
    return DB->LOCK_ROW ( "depo_id", "sub_depolar", QString ( "depo_id = %1").arg ( p_depo_id ) );
}

/**************************************************************************************
                   SUBE_DEPOLARI_FORMU::UNLOCK_RECORD
***************************************************************************************/

void SUBE_DEPOLARI_FORMU::UNLOCK_RECORD ( int p_depo_id )
{
    DB->UNLOCK_ROW ( "depo_id", "sub_depolar", QString ( "depo_id = %1" ).arg ( p_depo_id ) );
}

