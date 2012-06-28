#include "sube_unite_detaylari_formu_class.h"
#include "ui_sube_unite_detaylari_formu.h"
#include "adak_sql.h"
#include "e9_log.h"
 

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_SUBE_UNITE_DETAYLARI_FORMU
***************************************************************************************/

void OPEN_SUBE_UNITE_DETAYLARI_FORMU ( int p_kasa_unite_id, int p_modul_id, QWidget * parent )
{
    SUBE_UNITE_DETAYLARI_FORMU * F = new SUBE_UNITE_DETAYLARI_FORMU ( p_kasa_unite_id, p_modul_id, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::SUBE_UNITE_DETAYLARI_FORMU
***************************************************************************************/

SUBE_UNITE_DETAYLARI_FORMU::SUBE_UNITE_DETAYLARI_FORMU ( int p_kasa_unite_id, int p_modul_id, QWidget * parent )
: FORM_KERNEL ( parent ), m_ui ( new Ui:: SUBE_UNITE_DETAYLARI_FORMU )
{
    m_kasa_unite_id = p_kasa_unite_id;
    m_modul_id      = p_modul_id;

    m_ui->setupUi       ( this );
    START_FORM_KERNEL   ( this, DB );
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::SETUP_FORM
***************************************************************************************/

void SUBE_UNITE_DETAYLARI_FORMU::SETUP_FORM()
{

    if ( m_modul_id EQ CARI_MODULU ) {
        SET_HELP_PAGE( "cari-hesap-islemleri_yazicidaki-belge-numaralari" );
    }
    else if ( m_modul_id EQ FATURA_MODULU ) {
        SET_HELP_PAGE("fatura-islemleri_yazicidaki-belge-numaralari");
    }
    else if ( m_modul_id EQ IRSALIYE_MODULU ) {
        SET_HELP_PAGE("fatura-islemleri_yazicidaki-belge-numaralari");
    }

    SET_PAGE_TITLE           ( tr ( "ŞUBE - YAZICIDAKI BELGELER" ) );

    SET_SETTING_NAME         ( "SUBE_UNITE_DETAY_FORMU" );
    SET_SINGLE_RECORD_ID     ( m_kasa_unite_id );

    REGISTER_BUTTONS_WIDGET  ( m_ui->navigation_buttons_widget  , true  );

    SET_FIRST_FOCUS_WIDGET   ( m_ui->lineEdit_irsaliyeli_fat_belge_seri );

    m_ui->lineEdit_irsaliyeli_fat_belge_numarasi->setMaxLength(15);
    m_ui->lineEdit_irsaliye_belge_numarasi->setMaxLength(15);
    m_ui->lineEdit_fatura_belge_numarasi->setMaxLength(15);
    m_ui->lineEdit_tahsilat_makbuzu_belge_num->setMaxLength(15);
    m_ui->lineEdit_odeme_makbuzu_belge_num->setMaxLength(15);

    m_ui->lineEdit_fatura_belge_seri->setMaxLength( 3 );
    m_ui->lineEdit_irsaliyeli_fat_belge_seri->setMaxLength( 3 );
    m_ui->lineEdit_irsaliye_belge_seri->setMaxLength( 3 );
    m_ui->lineEdit_odeme_makbuzu_belge_seri->setMaxLength( 3 );
    m_ui->lineEdit_tahsilat_makbuzu_belge_seri->setMaxLength( 3 );
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void SUBE_UNITE_DETAYLARI_FORMU::CLEAR_FORM_MEMBERS()
{

}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::NEW_RECORD
***************************************************************************************/

void  SUBE_UNITE_DETAYLARI_FORMU::NEW_RECORD()
{
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::GET_RECORDD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::GET_RECORD ( int unite_id )
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_uniteler","irsaliye_belge_seri, irsaliye_belge_numarasi, "
                               "fatura_belge_seri, fatura_belge_numarasi, "
                               "tahsilat_makbuzu_belge_seri, tahsilat_makbuzu_belge_num, "
                               "odeme_makbuzu_belge_seri, odeme_makbuzu_belge_num, "
                               "irsaliyeli_fat_belge_numarasi, irsaliyeli_fat_belge_seri ",
                               "unite_id = :unite_id" );
    sql_query.SET_VALUE      ( ":unite_id", unite_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }
    sql_query.NEXT();

    m_ui->lineEdit_irsaliye_belge_seri->setText             ( sql_query.VALUE(0).toString() );
    m_ui->lineEdit_irsaliye_belge_numarasi->setText         ( sql_query.VALUE(1).toString() );
    m_ui->lineEdit_fatura_belge_seri->setText               ( sql_query.VALUE(2).toString() );
    m_ui->lineEdit_fatura_belge_numarasi->setText           ( sql_query.VALUE(3).toString() );
    m_ui->lineEdit_tahsilat_makbuzu_belge_seri->setText     ( sql_query.VALUE(4).toString() );
    m_ui->lineEdit_tahsilat_makbuzu_belge_num->setText      ( sql_query.VALUE(5).toString() );
    m_ui->lineEdit_odeme_makbuzu_belge_seri->setText        ( sql_query.VALUE(6).toString() );
    m_ui->lineEdit_odeme_makbuzu_belge_num->setText         ( sql_query.VALUE(7).toString() );
    m_ui->lineEdit_irsaliyeli_fat_belge_numarasi->setText   ( sql_query.VALUE(8).toString() );
    m_ui->lineEdit_irsaliyeli_fat_belge_seri->setText       ( sql_query.VALUE(9).toString() );

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::CHECK_VAR
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::CHECK_VAR ( QObject * p_object )
{
    if ( ( p_object EQ m_ui->lineEdit_irsaliyeli_fat_belge_seri ) OR
         ( p_object EQ m_ui->lineEdit_fatura_belge_seri ) OR
         ( p_object EQ m_ui->lineEdit_irsaliye_belge_seri) OR
         ( p_object EQ m_ui->lineEdit_odeme_makbuzu_belge_seri ) OR
         ( p_object EQ m_ui->lineEdit_tahsilat_makbuzu_belge_seri ) ) {

        QLineEdit * lineEdit = ( QLineEdit * ) p_object;
        lineEdit->setText(lineEdit->text().toUpper());

        //FIXME REGULAR EXP YAZ VE KARAKTER OLDUGUNU KONTROL ET.

    }

   return ADAK_OK;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::CHECK_EMPTY
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::CHECK_EMPTY ()
{

    if ( m_ui->lineEdit_irsaliye_belge_numarasi->text().isEmpty() EQ true OR m_ui->lineEdit_irsaliye_belge_seri->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "İrsaliye seri giriniz." ), m_ui->lineEdit_irsaliye_belge_seri );
        return ADAK_FAIL;
    }

    if ( m_ui->lineEdit_fatura_belge_numarasi->text().isEmpty() EQ true  OR m_ui->lineEdit_fatura_belge_seri->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Fatura seri giriniz." ), m_ui->lineEdit_fatura_belge_seri );
        return ADAK_FAIL;
    }

    if ( m_ui->lineEdit_odeme_makbuzu_belge_num->text().isEmpty() EQ true OR m_ui->lineEdit_odeme_makbuzu_belge_seri->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Ödeme makbuzu seri giriniz." ), m_ui->lineEdit_odeme_makbuzu_belge_seri );
        return ADAK_FAIL;
    }

    if ( m_ui->lineEdit_tahsilat_makbuzu_belge_num->text().isEmpty() EQ true OR m_ui->lineEdit_tahsilat_makbuzu_belge_seri->text().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Tahsilat makbuzu seri giriniz." ), m_ui->lineEdit_tahsilat_makbuzu_belge_seri );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::CHECK_ADD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::ADD_RECORD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::ADD_RECORD ()
{
    return 1;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::CHECK_UPDATE
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::CHECK_UPDATE ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::UPDATE_RECORD
***************************************************************************************/

void SUBE_UNITE_DETAYLARI_FORMU::UPDATE_RECORD ( int p_kasa_unite_id )
{
    SQL_QUERY sql_query (DB);
    sql_query.PREPARE_UPDATE ( "sub_uniteler ","unite_id",
                               "irsaliye_belge_seri             , "
                               "irsaliye_belge_numarasi         , "
                               "fatura_belge_seri               , "
                               "fatura_belge_numarasi           , "
                               "tahsilat_makbuzu_belge_seri     , "
                               "tahsilat_makbuzu_belge_num      , "
                               "odeme_makbuzu_belge_seri        , "
                               "odeme_makbuzu_belge_num         , "
                               "irsaliyeli_fat_belge_seri       , "
                               "irsaliyeli_fat_belge_numarasi     ",
                               "unite_id         = :unite_id");

    sql_query.SET_VALUE     ( ":irsaliye_belge_seri"          , m_ui->lineEdit_irsaliye_belge_seri->text());
    sql_query.SET_VALUE     ( ":irsaliye_belge_numarasi"      , m_ui->lineEdit_irsaliye_belge_numarasi->text());
    sql_query.SET_VALUE     ( ":fatura_belge_seri"            , m_ui->lineEdit_fatura_belge_seri->text());
    sql_query.SET_VALUE     ( ":fatura_belge_numarasi"        , m_ui->lineEdit_fatura_belge_numarasi->text());
    sql_query.SET_VALUE     ( ":tahsilat_makbuzu_belge_seri"  , m_ui->lineEdit_tahsilat_makbuzu_belge_seri->text());
    sql_query.SET_VALUE     ( ":tahsilat_makbuzu_belge_num"   , m_ui->lineEdit_tahsilat_makbuzu_belge_num->text());
    sql_query.SET_VALUE     ( ":odeme_makbuzu_belge_seri"     , m_ui->lineEdit_odeme_makbuzu_belge_seri->text());
    sql_query.SET_VALUE     ( ":odeme_makbuzu_belge_num"      , m_ui->lineEdit_tahsilat_makbuzu_belge_num->text());
    sql_query.SET_VALUE     ( ":irsaliyeli_fat_belge_seri"    , m_ui->lineEdit_irsaliyeli_fat_belge_seri->text());
    sql_query.SET_VALUE     ( ":irsaliyeli_fat_belge_numarasi", m_ui->lineEdit_irsaliyeli_fat_belge_numarasi->text());
    sql_query.SET_VALUE     ( ":unite_id"                     , p_kasa_unite_id );
    sql_query.UPDATE();


    sql_query.PREPARE_SELECT ( "sub_uniteler,sub_subeler","unite_adi, sube_kodu , sube_adi ",
                               "unite_id = :unite_id AND sub_subeler.sube_id = sub_uniteler.sube_id");

    sql_query.SET_VALUE     ( ":unite_id" , p_kasa_unite_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();

    QString log_detaylari = "Kasa Ünite / Adı : " + sql_query.VALUE(0).toString() + " , Şube Kodu / Adı : " +
                            sql_query.VALUE(1).toString() + "  " + sql_query.VALUE(2).toString();

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_UNITELER , LOG_ISLEM_UPDATE , log_detaylari );

}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::CHECK_DELETE
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::DELETE_RECORD
***************************************************************************************/

void SUBE_UNITE_DETAYLARI_FORMU::DELETE_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::FIND_FIRST_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::FIND_LAST_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::FIND_NEXT_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::FIND_PREV_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::SELECT_RECORD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::SELECT_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::LOCK_RECORD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::LOCK_RECORD ( int p_kasa_unite_id )
{
    return DB->LOCK_ROW ( "unite_id","sub_uniteler", QString ( "unite_id = %1").arg ( p_kasa_unite_id ) );
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::UNLOCK_RECORD
***************************************************************************************/

void SUBE_UNITE_DETAYLARI_FORMU::UNLOCK_RECORD ( int p_kasa_unite_id )
{
    DB->UNLOCK_ROW ( "unite_id", "sub_uniteler", QString ( "unite_id = %1").arg ( p_kasa_unite_id ) );
}

/**************************************************************************************
                   SUBE_UNITE_DETAYLARI_FORMU::FIND_RECORD
***************************************************************************************/

int SUBE_UNITE_DETAYLARI_FORMU::FIND_RECORD()
{
    return 0;
}
