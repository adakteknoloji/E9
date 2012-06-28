#include "muh_yansitma_islemi_batch_class.h"
#include "muh_yansitma_islemi_batch_open.h"
#include "ui_muh_yansitma_islemi_batch.h"
#include "print.h"
#include "e9_enum.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "muh_enum.h"
#include "muh_console_utils.h"
#include "muh_fis_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_MUH_YANSITMA_ISLEMI_BATCH
***************************************************************************************/

void OPEN_MUH_YANSITMA_ISLEMI_BATCH ( QWidget * parent )
{
    MUH_YANSITMA_ISLEMI_BATCH * F = new MUH_YANSITMA_ISLEMI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   MUH_YANSITMA_ISLEMI_BATCH::MUH_YANSITMA_ISLEMI_BATCH
***************************************************************************************/

MUH_YANSITMA_ISLEMI_BATCH::MUH_YANSITMA_ISLEMI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::MUH_YANSITMA_ISLEMI_BATCH )
{
    m_ui->setupUi       ( this );
    START_BATCH_KERNEL  ( this, DB );


}

/**************************************************************************************
                   MUH_YANSITMA_ISLEMI_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_YANSITMA_ISLEMI_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE          ( tr ( "MUH - YANSITMA FİŞİ OLUŞTUR"));
    SET_HELP_PAGE           ( "genel-muhasebe_yansitma-fisi-olustur");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    m_donem_bas_tarihi = QDate ( QDate::currentDate().year(), QDate::currentDate().month(), 01 );
    m_donem_bts_tarihi = m_donem_bas_tarihi.addDays ( QDate::currentDate().daysInMonth() -1 );

    m_ui->adakDate_bas_tarihi->SET_DATE( m_donem_bas_tarihi );

    m_ui->adakDate_bitis_tarihi->SET_DATE( m_donem_bts_tarihi     );

    m_ui->adakDate_fis_tarihi->SET_DATE( m_donem_bts_tarihi     );

    m_ui->lineEdit_fis_no->setText("*");

    m_ui->limitedTextEdit_aciklama->setText("Fiş, Yansıtma Fişi Oluştur işlemi sonucunda oluşturulmuştur.");
    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH(512);
    m_ui->lineEdit_satir_aciklama->setText("Yansıtma İşlemi");

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Oluştur" );    
    SET_SETTING_NAME             ( "YANSITMA_ISLEMI_BATCH" );
    SET_SETTING_NAME             ( "MUH_YANSITMA_ISLEMI_BATCH" );

    SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_fis_tarihi );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ) {
        m_ui->comboBox_sirket_turu->setHidden(true);
        m_ui->label_sirket_turu->setHidden(true);
    }

}

/**************************************************************************************
                   MUH_YANSITMA_ISLEMI_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_YANSITMA_ISLEMI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->lineEdit_fis_no ) {
        if ( QString ( m_ui->lineEdit_fis_no->text().at(0)) NE "*" ) {
            int fis_no = m_ui->lineEdit_fis_no->text().toInt();

            SQL_QUERY query ( DB );

            query.PREPARE_SELECT("muh_fisler" , "fis_id" , "fis_no = :fis_no "
                                 "AND fis_tarihi = :fis_tarihi");

            query.SET_VALUE      ( ":fis_no"       ,fis_no );
            query.SET_VALUE      ( ":fis_tarihi"   ,m_ui->adakDate_fis_tarihi->DATE() );

            if ( query.SELECT() NE 0 ) {
                MSG_WARNING( tr ( "Aynı fiş numarası ile kayıtlı başka bir fiş bulunmaktadır.Lütfen fiş numarasını değiştiriniz. "), m_ui->lineEdit_fis_no );

                return ADAK_FAIL;

            }
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_ISLEMI_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_YANSITMA_ISLEMI_BATCH::CHECK_RUN ()
{
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarihi, m_ui->adakDate_bitis_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_ISLEMI_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_YANSITMA_ISLEMI_BATCH::RUN_BATCH ()
{
    //Transfer hesaplarinin verilen tarih araligindaki , toplam borc ve alacaklari okunur.

    SQL_QUERY sql_query(DB);

    int fis_no            = 0;

    if ( QString ( m_ui->lineEdit_fis_no->text().at(0) ) EQ "*") {
        fis_no = MUH_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE() );
    }
    else {
        fis_no = m_ui->lineEdit_fis_no->text().toInt();
    }

    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

    MUHASEBE_FISI->fis_no           = fis_no;
    MUHASEBE_FISI->program_id       = E9_PROGRAMI;
    MUHASEBE_FISI->modul_id         = MUHASEBE_MODULU;
    MUHASEBE_FISI->fis_tarihi       = m_ui->adakDate_fis_tarihi->DATE();
    MUHASEBE_FISI->fis_turu         = ENUM_MAHSUP_FISI;
    MUHASEBE_FISI->aciklama         = m_ui->limitedTextEdit_aciklama->toPlainText();

    sql_query.PREPARE_SELECT("muh_yansitma_hesaplari" , "gider_hesabi_id, yansitma_hesabi_id, bilanco_gelir_hesabi_id");


    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr("Yansıtma hesaplarında kayıt bulunmamaktadır."), m_ui->lineEdit_fis_no );

        return;
    }

    double      BORC_BAKIYESI   [ E9_ARRAY_SIZE ];
    double      ALACAK_BAKIYESI [ E9_ARRAY_SIZE ];


    int         bas_gun_no =  m_ui->adakDate_bas_tarihi->MALI_YIL_ARRAY_INDIS();
    int         bts_gun_no =  m_ui->adakDate_bitis_tarihi->MALI_YIL_ARRAY_INDIS();


    double      borc_bakiye   = 0.0;
    double      alacak_bakiye = 0.0;

    double      hesap_bakiye        = 0.00;

    //Transfer islemi sonucunda verilen donem araligi icin bir mahsup fisi olusturulur.

    DB->START_TRANSACTION   ();

    SQL_QUERY query   ( DB );

    bool yansitma_fisi_eklendi  = false;
    int fis_id                  = 0;

    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;


    while ( sql_query.NEXT() EQ true ) {

        int transfer_hesabi_id = sql_query.VALUE(0).toInt();

        query.PREPARE_SELECT("muh_hesaplar" , "toplam_borc_array,toplam_alacak_array" ,
                             "hesap_id = :hesap_id");

        query.SET_VALUE(":hesap_id" , transfer_hesabi_id);

        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            UNPACK_DOUBLE_ARRAY(query.VALUE(0).toString() , BORC_BAKIYESI , E9_ARRAY_SIZE);
            UNPACK_DOUBLE_ARRAY(query.VALUE(1).toString() , ALACAK_BAKIYESI , E9_ARRAY_SIZE);
        }

        borc_bakiye     =  ROUND ( ( BORC_BAKIYESI   [ bts_gun_no ] - BORC_BAKIYESI   [ bas_gun_no ] ) );
        alacak_bakiye   =  ROUND ( ( ALACAK_BAKIYESI [ bts_gun_no ] - ALACAK_BAKIYESI [ bas_gun_no ] ) );

        if ( borc_bakiye EQ 0  AND alacak_bakiye EQ 0 ) {
            continue;
        }

        hesap_bakiye = 0.0;

        if ( borc_bakiye > alacak_bakiye ) {
            hesap_bakiye = ROUND ( borc_bakiye - alacak_bakiye );
        }
        else {
            hesap_bakiye = ROUND ( alacak_bakiye - borc_bakiye  );
        }

        if ( yansitma_fisi_eklendi EQ false ) {
            fis_id = MUH_FIS_EKLE ( MUHASEBE_FISI );
            yansitma_fisi_eklendi = true;
        }

        //Muhasebe fisine satir eklenecek.
        //Gider hesabinin yansitma ve bilanco-gelir tablosu hesaplari muhasebe fisine eklenir,gider hesabi eklenmez.
        //Ynasitma ve bilanco_gelir hesaplarina giderr hesabinin bakiyesi yazilir.

        int yansitma_hesabi_id      = sql_query.VALUE(1).toInt();
        int bilanco_gelir_hesabi_id = sql_query.VALUE(2).toInt();

        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRI->aciklama         = m_ui->lineEdit_satir_aciklama->text();
        MUH_FIS_SATIRI->fis_id           = fis_id;
        MUH_FIS_SATIRI->borc_tutari      = hesap_bakiye;
        MUH_FIS_SATIRI->hesap_id         = bilanco_gelir_hesabi_id;
        MUH_FIS_SATIRI->modul_id         = MUHASEBE_FISI->modul_id;
        MUH_FIS_SATIRI->program_id       = MUHASEBE_FISI->program_id;

        MUH_FIS_SATIRI_EKLE(MUHASEBE_FISI , MUH_FIS_SATIRI);


        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRI->aciklama         = m_ui->lineEdit_satir_aciklama->text();
        MUH_FIS_SATIRI->fis_id           = fis_id;
        MUH_FIS_SATIRI->alacak_tutari    = hesap_bakiye;
        MUH_FIS_SATIRI->hesap_id         = yansitma_hesabi_id;
        MUH_FIS_SATIRI->modul_id         = MUHASEBE_FISI->modul_id;
        MUH_FIS_SATIRI->program_id       = MUHASEBE_FISI->program_id;

        MUH_FIS_SATIRI_EKLE(MUHASEBE_FISI , MUH_FIS_SATIRI);


    }

    DB->COMMIT_TRANSACTION();

    if ( fis_id EQ 0 ) {
        MSG_WARNING( tr("Yansıtma hesaplarının verilen tarih aralığında bakiyesi bulunmamaktadır."), m_ui->adakDate_bas_tarihi );
        return;
    }
    MSG_WARNING( tr("%1 Numaralı Yansıtma Fişi Oluşturuldu.").arg(fis_no), NULL );

}

/**************************************************************************************
                   MUH_YANSITMA_ISLEMI_BATCH::CHECK_EXIT
***************************************************************************************/

int MUH_YANSITMA_ISLEMI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}


