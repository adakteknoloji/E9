#include "muh_hesap_plani_kopyala_batch_class.h"
#include "muh_hesap_plani_kopyala_batch_open.h"
#include "ui_muh_hesap_plani_kopyala_batch.h"
#include "login.h"
#include "yonetim.h"
#include "e9_console_utils.h"

extern ADAK_SQL *DB;
ADAK_SQL        *TARGET_DB;


/**************************************************************************************
                   OPEN_MUH_HESAP_PLANI_KOPYALA_BATCH
***************************************************************************************/

void OPEN_MUH_HESAP_PLANI_KOPYALA_BATCH ( QWidget * parent )
{
    MUH_HESAP_PLANI_KOPYALA_BATCH * F = new MUH_HESAP_PLANI_KOPYALA_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   MUH_HESAP_PLANI_KOPYALA_BATCH::MUH_HESAP_PLANI_KOPYALA_BATCH
***************************************************************************************/

MUH_HESAP_PLANI_KOPYALA_BATCH::MUH_HESAP_PLANI_KOPYALA_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::MUH_HESAP_PLANI_KOPYALA_BATCH )
{
    m_ui->setupUi      ( this );    
    START_BATCH_KERNEL ( this, DB );
}


/**************************************************************************************
                   MUH_HESAP_PLANI_KOPYALA_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_HESAP_PLANI_KOPYALA_BATCH::SETUP_FORM ()
{
    SET_PAGE_TITLE    ( tr ( "MUH - HESAP PLANI KOPYALA" ) );
    SET_HELP_PAGE     ("genel-muhasebe_hesap-plani-kopyala");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Kopyala" );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->searchEdit_veritabani_ismi );
    SET_SETTING_NAME             ( "MUH_HESAP_PLANI_KOPYALA_BATCH" );
    SET_ENTER_KEY_FOR_RUN_BATCH  ( true );
}

/**************************************************************************************
                   MUH_HESAP_PLANI_KOPYALA_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_HESAP_PLANI_KOPYALA_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchEdit_veritabani_ismi ) {

        if ( m_ui->searchEdit_veritabani_ismi->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_veritabani_tanimi->clear();
            return ADAK_FAIL;
        }

        QString veritabani_tanimi = VERITABANI_TANIMI(m_ui->searchEdit_veritabani_ismi->GET_TEXT());

        if ( veritabani_tanimi EQ VERITABANI_TANIMI() ) {
            MSG_WARNING(tr("Kopyalanacak veritabanı olarak içerisinde bulunduğunuz veritabanını seçemezsiniz."),m_ui->searchEdit_veritabani_ismi);
            return ADAK_FAIL_UNDO;
        }

        if ( veritabani_tanimi.isEmpty() EQ true ) {
            MSG_WARNING(tr("Aradığınız isimle kayıtlı veritabanı bulunmamaktadır."),
                        m_ui->searchEdit_veritabani_ismi);
            return ADAK_FAIL;
        }
        m_ui->lineEdit_veritabani_tanimi->setText(veritabani_tanimi);
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_PLANI_KOPYALA_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_HESAP_PLANI_KOPYALA_BATCH::CHECK_RUN ()
{
    if ( m_ui->searchEdit_veritabani_ismi->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(tr("Hesap Planını kopyalamak istediğiniz veritabanını seçmelisiniz."),
                    m_ui->searchEdit_veritabani_ismi);
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_PLANI_KOPYALA_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_HESAP_PLANI_KOPYALA_BATCH::RUN_BATCH ()
{

    QString db_name = SQL_DBNAME_ONEK() + "adak_" + m_ui->searchEdit_veritabani_ismi->GET_TEXT();

    TARGET_DB = CREATE_ADAK_SQL_CONNECTION( GET_PROGRAM_DB_STRUCTS() , db_name , GET_SQL_DB_DRIVER_ENUM(SQL_MOTORU()));

    if ( TARGET_DB->CONNECT_TO_DATABASE(true ) EQ ADAK_FAIL ) {

        MSG_WARNING(  tr("Seçilen veritabanı ile bağlantı kurulamadı"), NULL );
        return;
    }

    SQL_QUERY query_source_db(DB);

    SQL_QUERY query_target_db(TARGET_DB);

    query_target_db.PREPARE_SELECT("muh_hesaplar" , "hesap_id", "hesap_seviyesi = :hesap_seviyesi");
    query_target_db.SET_VALUE(":hesap_seviyesi" , 2);

    if ( query_target_db.SELECT() NE 0 ) {
        //Hesaplara alt hesap acilmis ise ,baska bir hesap palnindan kopyalama yapilmayacak
        MSG_WARNING(tr("Veritabanınızdaki hesaplara alt hesap açılmış durumda."
                       "Hesap Planı kopyalayamazsınız.") , NULL);
        return;
    }
    else {

        query_target_db.PREPARE_SELECT("muh_fis_satirlari" ,"fis_satiri_id");

        if ( query_target_db.SELECT() NE 0 ) {
            MSG_WARNING(tr("Veritabanınızdaki hesaplar işlem görmüş,"
                           "Hesap Planı kopyalayamazsınız.") , NULL);
            return;
        }

        TARGET_DB->START_TRANSACTION();

        query_source_db.PREPARE_SELECT("muh_hesaplar" , "hesap_id, parent_id, hesap_seviyesi,"
                                "hesap_kodu,hesap_ismi,tam_hesap_kodu,tali_hesap,"
                                "alt_hesap_sayisi");

        if ( query_source_db.SELECT() NE 0 ) {

            query_target_db.PREPARE_DELETE("muh_hesaplar" , "");
            query_target_db.DELETE();

            int hesap_id = 0;

            while ( query_source_db.NEXT() ) {

                query_target_db.PREPARE_INSERT("muh_hesaplar" , "hesap_id" ,
                                        "hesap_id,parent_id, hesap_seviyesi,"
                                        "hesap_kodu, hesap_ismi,tam_hesap_kodu,"
                                        "tali_hesap, alt_hesap_sayisi");
                query_target_db.SET_VALUE(":hesap_id"         , query_source_db.VALUE(0).toInt());
                query_target_db.SET_VALUE(":parent_id"        , query_source_db.VALUE(1).toInt());
                query_target_db.SET_VALUE(":hesap_seviyesi"   , query_source_db.VALUE(2).toInt());
                query_target_db.SET_VALUE(":hesap_kodu"       , query_source_db.VALUE(3).toString());
                query_target_db.SET_VALUE(":hesap_ismi"       , query_source_db.VALUE(4).toString());
                query_target_db.SET_VALUE(":tam_hesap_kodu"   , query_source_db.VALUE(5).toString());
                query_target_db.SET_VALUE(":tali_hesap"       , query_source_db.VALUE(6).toInt());
                query_target_db.SET_VALUE(":alt_hesap_sayisi" , query_source_db.VALUE(7).toInt());

                hesap_id = query_target_db.INSERT();
            }

            if ( hesap_id > 0 ) {
                DB->SET_TABLE_AUTO_INCREMENT_ID_VALUE("muh_hesaplar" , "hesap_id" ,hesap_id);
            }
        }

        TARGET_DB->COMMIT_TRANSACTION();

        MSG_WARNING(tr("İçerisinde bulunduğunuz veritabanı %1 Veritabanındaki hesap planına kopyalandı.").arg(m_ui->searchEdit_veritabani_ismi->GET_TEXT()),NULL);

    }
}

/**************************************************************************************
                   MUH_HESAP_PLANI_KOPYALA_BATCH::CHECK_EXIT
***************************************************************************************/

int MUH_HESAP_PLANI_KOPYALA_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_PLANI_KOPYALA_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/
void MUH_HESAP_PLANI_KOPYALA_BATCH::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    Q_UNUSED(widget);

    QString veritabani_ismi;
    QString veritabani_tanimi;

    int veritabani_id = VERITABANI_SEC(KULLANICI_ID() , &veritabani_ismi , &veritabani_tanimi , this);

    if ( veritabani_id > 0 ) {
        line_edit->setText(veritabani_ismi);
    }
}

