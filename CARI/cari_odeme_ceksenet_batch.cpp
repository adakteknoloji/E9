#include <QMessageBox>
 
#include "adak_std_utils.h"
#include "cari_odeme_ceksenet_batch_class.h"
#include "cari_odeme_ceksenet_batch_open.h"
#include "ui_cari_odeme_ceksenet_batch.h"
#include "cek_enum.h"
#include "cek_console_utils.h"
#include "bnk_console_utils.h"
#include "cek_senet_utils.h"
#include "e9_enum.h"
#include "cek_senet_formu_open.h"

extern ADAK_SQL * DB;


bool OPEN_CARI_ODEME_CEKSENET_BATCH ( int fis_satiri_id,int cek_senet_id,int islem_turu,QWidget * parent )
{
    bool kayit_degistirildimi = false;

    CARI_ODEME_CEKSENET_BATCH * B = new CARI_ODEME_CEKSENET_BATCH ( fis_satiri_id,cek_senet_id,islem_turu,parent );

    B->m_kayit_degisti_mi = &kayit_degistirildimi;

    B->EXEC( NOT_FULL_SCREEN );

    return kayit_degistirildimi;
}

/**************************************************************************************
                   CARI_ODEME_CEKSENET_BATCH::CARI_ODEME_CEKSENET_BATCH
***************************************************************************************/

CARI_ODEME_CEKSENET_BATCH::CARI_ODEME_CEKSENET_BATCH ( int fis_satiri_id,int cek_senet_id,int islem_turu,QWidget * parent) :  BATCH_KERNEL (parent), m_ui(new Ui::CARI_ODEME_CEKSENET_BATCH)
{
    m_fis_satiri_id = fis_satiri_id;
    m_cek_senet_id  = cek_senet_id;
    m_islem_turu    = islem_turu;

    if ( m_islem_turu EQ ENUM_KENDI_CEKIMIZ ) {
        m_cek_senet_turu = ENUM_CEK;
    }
    else if ( m_islem_turu EQ ENUM_KENDI_SENEDIMIZ){
        m_cek_senet_turu = ENUM_SENET;
    }

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   CARI_ODEME_CEKSENET_BATCH::SETUP_FORM
***************************************************************************************/

void CARI_ODEME_CEKSENET_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE     ( "cari_odeme_cek_senet_batch.html" );
    if ( m_islem_turu EQ ENUM_MUSTERI_SENEDI OR m_islem_turu EQ ENUM_KENDI_SENEDIMIZ ) {
         m_ui->groupBox_musteri_banka_bilgisi->hide();
         SET_PAGE_TITLE ( tr ( "CARİ - SENET BİLGİLERİ" ) );
         m_ui->label_cek_senet_no->setText ( "Senet Numarası" );
         m_ui->groupBox_cek_senet_bilgileri->setTitle ( "Senet Bilgileri" );

    }
    else {
        SET_PAGE_TITLE ( "CARİ - ÇEK BİLGİLERİ" );
        m_ui->label_cek_senet_no->setText ( "Çek Numarası" );
        m_ui->groupBox_cek_senet_bilgileri->setTitle ( "Çek Bilgileri" );
    }
    REGISTER_BUTTON_WIDGET  ( m_ui->run_batch_widget );

    m_ui->adakDate_vade_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_keside_tarihi->MALI_YIL_KISITI_YOK( true );

    SET_AUTO_EXIT_BATCH ( true );

    m_ui->adakDate_keside_tarihi->SET_DATE( QDate::currentDate() );
    m_ui->adakDate_vade_tarihi->SET_DATE( QDate::currentDate() );

    if ( m_fis_satiri_id NE -1 ) {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("car_fis_satirlari" , "cek_senet_id" ,"fis_satiri_id = :fis_satiri_id");
        query.SET_VALUE(":fis_satiri_id" , m_fis_satiri_id);

        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            m_cek_senet_id = query.VALUE(0).toInt();
        }

        CEK_SENET_STRUCT CEK_STRUCT;

        if ( m_cek_senet_id > 0 ) {

            CEK_GET_TEMEL_BILGILER( &CEK_STRUCT , m_cek_senet_id);

            m_cek_senet_turu   = CEK_STRUCT.cek_senet_turu;
            m_cek_senet_durumu = CEK_STRUCT.cek_senet_son_durumu;
        }

    }

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Tamam" );
    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_vade_tarihi );

    if ( m_cek_senet_id NE 0 ) {
        CEK_SENET_BILGILERINI_GOSTER();
    }
    if ( m_islem_turu EQ ENUM_MUSTERI_CEKI OR m_islem_turu EQ ENUM_MUSTERI_SENEDI ) {
        m_ui->pushButton_duzenle->setHidden(true);
    }
    SET_ENTER_KEY_FOR_RUN_BATCH(true);
    SET_SETTING_NAME        ( "CARI_ODEME_CEKSENET_BATCH" );
}

/**************************************************************************************
                   CARI_ODEME_CEKSENET_BATCH::CHECK_VAR
***************************************************************************************/

int CARI_ODEME_CEKSENET_BATCH::CHECK_VAR (  QObject * object )
{
    if ( object EQ m_ui->pushButton_duzenle ) {
        if ( m_cek_senet_durumu EQ ENUM_SATICIYA_VERILDI OR m_cek_senet_durumu EQ ENUM_PORTFOYDE ) {
            OPEN_CEK_SENET_FORMU( m_cek_senet_turu , m_cek_senet_id , this);
            CEK_SENET_BILGILERINI_GOSTER();
            *m_kayit_degisti_mi = true;
        }
        else {
            MSG_INFO(QObject::tr("Çek - Senedin son durumu müşteriye verildi olmadığı için çek-senet bilgilerinde değişiklik yapamazsınız."),NULL);
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_CEKSENET_BATCH::CHECK_RUN
***************************************************************************************/

int CARI_ODEME_CEKSENET_BATCH::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_CEKSENET_BATCH::RUN_BATCH
***************************************************************************************/

void CARI_ODEME_CEKSENET_BATCH::RUN_BATCH()
{
}

/**************************************************************************************
                   CARI_ODEME_CEKSENET_BATCH::CHECK_EXIT
***************************************************************************************/

int CARI_ODEME_CEKSENET_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}
/**************************************************************************************
                   CARI_ODEME_CEKSENET_BATCH::CEK_SENET_BILGILERINI_GOSTER()
***************************************************************************************/

void CARI_ODEME_CEKSENET_BATCH::CEK_SENET_BILGILERINI_GOSTER()
{

    CEK_SENET_STRUCT CEK_STRUCT;

    int result = CEK_GET_TEMEL_BILGILER( &CEK_STRUCT, m_cek_senet_id );

    if ( result NE 0  ) {

        m_ui->line_edit_portfoy_no->setText     ( QVariant( CEK_STRUCT.portfoy_numarasi ).toString() );
        m_ui->adakDate_vade_tarihi->SET_DATE   ( CEK_STRUCT.vade_tarihi );
        m_ui->line_edit_valor->setText          ( QVariant ( CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI( m_ui->adakDate_vade_tarihi->QDATE()) ).toString() );
        m_ui->line_edit_cek_senet_no->setText   ( QVariant ( CEK_STRUCT.cek_senet_numarasi ).toString() );
        m_ui->lineEdit_banka_ismi->setText      ( CEK_STRUCT.banka_ismi );
        QString sube_kodu = CEK_STRUCT.sube_kodu;
        m_ui->lineEdit_sube_kodu->setText       ( sube_kodu );
        m_ui->lineedit_sube_ismi->setText       ( CEK_STRUCT.sube_ismi );
        if ( CEK_STRUCT.musteri_ceki_mi EQ 1 ) {
            m_ui->lineedit_hesap_no->setText    ( CEK_STRUCT.musteri_hesap_numarasi );

            m_ui->label_kesideci->setHidden     ( false );
            m_ui->lineedit_kesideci->setHidden  ( false );
            QString kesideci =CEK_STRUCT.borclu_kesideci;
            m_ui->lineedit_kesideci->setText    ( kesideci );
        }
        else {
            m_ui->lineedit_hesap_no->setText ( BNK_GET_HESAP_NUMARASI ( CEK_STRUCT.hesap_no_id ));
            m_ui->label_kesideci->setHidden     ( true );
            m_ui->lineedit_kesideci->setHidden  ( true ) ;
        }
        m_ui->adakDate_keside_tarihi->SET_DATE ( CEK_STRUCT.keside_tarihi );
        m_cek_senet_durumu = CEK_STRUCT.cek_senet_son_durumu;
   }
}
