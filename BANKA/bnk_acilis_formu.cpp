#include "bnk_acilis_formu_class.h"
#include "bnk_acilis_formu_open.h"
#include "bnk_hareket_fisi_open.h"
#include "bnk_hareket_fisi_arama_open.h"
#include "ui_bnk_acilis_formu.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_log.h"
#include "bnk_fis_utils.h"
#include "banka_enum.h"
#include "adak_std_utils.h"
#include "bnk_hesaplari_arama_open.h"
#include "bnk_console_utils.h"

extern ADAK_SQL  * DB;

/**************************************************************************************
                   OPEN_BNK_ACILIS_FORMU
***************************************************************************************/

void OPEN_BNK_ACILIS_FORMU ( QWidget * parent )
{
    BNK_ACILIS_FORMU * F = new BNK_ACILIS_FORMU ( parent );
    F->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::BNK_ACILIS_FORMU
***************************************************************************************/

BNK_ACILIS_FORMU:: BNK_ACILIS_FORMU ( QWidget * parent ) : FORM_KERNEL ( parent ), m_ui(new Ui:: BNK_ACILIS_FORMU)
{
    m_ui->setupUi     ( this );
    START_FORM_KERNEL ( this , DB );

    m_fis_turu  = ENUM_BANKA_ACILIS_FISI;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::SETUP_FORM
***************************************************************************************/

void  BNK_ACILIS_FORMU::SETUP_FORM ()
{
    SET_PAGE_TITLE    ( tr ( "BNK - BANKA AÇILIŞ FORMU" ) );
    SET_HELP_PAGE     ("banka-islemleri_banka-acilis-formu" );

    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget );

    M_FIS         = new BNK_HAREKET_FIS_STRUCT;
    M_FIS_SATIRI  = new BNK_HAREKET_FIS_SATIRI_STRUCT;

    m_ui->adakDate_tarih->setEnabled( false );


    m_ui->adakDate_tarih->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->comboBox_fis_filtresi->setVisible( false );
    m_ui->label_fis_filtresi->setVisible(false);

    m_ui->comboBox_sirket->setVisible(false);
    m_ui->label_sirket->setVisible(false);

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ 1 ) {
        m_ui->comboBox_fis_filtresi->setVisible( true );
        m_ui->label_fis_filtresi->setVisible(true);

        m_ui->comboBox_sirket->setVisible(true);
        m_ui->label_sirket->setVisible(true);
    }

    SET_FIRST_FOCUS_WIDGET ( m_ui->searchEdit_banka_hesabi );
    SET_SETTING_NAME  ("BANAKA_ACILIS_FORMU");

    m_ui->commaEdit_toplam_alacak->SET_PRECISION( 2 );
    m_ui->commaEdit_toplam_borc->SET_PRECISION( 2 );
    m_ui->commaEdit_kk_odeme_borc->SET_PRECISION( 2 );
    m_ui->commaEdit_kk_tahsilat_borc->SET_PRECISION( 2 );


    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH( 512 );
    m_ui->lineEdit_banka_adi->setMaxLength( 50 );
    m_ui->commaEdit_kk_odeme_borc->setMaxLength( 15 );
    m_ui->commaEdit_kk_tahsilat_borc->setMaxLength( 15 );
    m_ui->commaEdit_toplam_alacak->setMaxLength( 15 );
    m_ui->commaEdit_toplam_borc->setMaxLength( 15 );

    DISABLE_CLEAR_ON_WIDGET( m_ui->adakDate_tarih );

}


/**************************************************************************************
                   BNK_ACILIS_FORMU::NEW_RECORD
***************************************************************************************/

void  BNK_ACILIS_FORMU::NEW_RECORD ()
{
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void BNK_ACILIS_FORMU::CLEAR_FORM_MEMBERS()
{

}

/**************************************************************************************
                   BNK_ACILIS_FORMU::GET_RECORD
***************************************************************************************/

int BNK_ACILIS_FORMU::GET_RECORD ( int record_id )
{
    BNK_CLEAR_HAREKET_FIS_STRUCT( M_FIS );

    BNK_HAREKET_FIS_BILGILERINI_OKU( M_FIS, record_id );

    int fis_satiri_id = 0;
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hareket_fis_satirlari","fis_satiri_id","fis_id =:fis_id" );
    query.SET_VALUE     ( ":fis_id", record_id );

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {
            fis_satiri_id = query.VALUE( 0 ).toInt();
            BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );
            BNK_HAREKET_FIS_SATIR_BILGILERINI_OKU( M_FIS_SATIRI, fis_satiri_id );

            switch ( M_FIS_SATIRI->etkilenecek_hesap_turu ) {

            case ENUM_BNK_BANKA_HESABI: {
                    m_ui->commaEdit_toplam_borc->SET_DOUBLE( M_FIS_SATIRI->hesaba_giren );
                    m_ui->commaEdit_toplam_alacak->SET_DOUBLE( M_FIS_SATIRI->hesaptan_cikan );
                    break;
                }
            case ENUM_BNK_KREDI_KARTI_HESABI: {
                    m_ui->commaEdit_kk_odeme_borc->SET_DOUBLE( M_FIS_SATIRI->hesaba_giren );
                    m_ui->commaEdit_kk_odeme_alacak->SET_DOUBLE( M_FIS_SATIRI->hesaptan_cikan );
                    break;
                }
            case ENUM_BNK_POS_HESABI: {
                    m_ui->commaEdit_kk_tahsilat_borc->SET_DOUBLE( M_FIS_SATIRI->hesaba_giren );
                    break;
                }
            default : break;
            }
        }

    }

    m_ui->searchEdit_banka_hesabi->SET_TEXT( BNK_GET_BANKA_KODU ( M_FIS->hesap_no_id ));
    m_ui->lineEdit_banka_doviz_kodu->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( M_FIS->hesap_no_id ) ) );
    m_ui->lineEdit_hesap_bakiyesi_alacak_birimi->setText    ( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( M_FIS->hesap_no_id ) ) );
    m_ui->lineEdit_hesap_bakiyesi_borc_birimi->setText      ( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( M_FIS->hesap_no_id ) ) );
    m_ui->lineEdit_kk_odeme_borc_birimi->setText            ( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( M_FIS->hesap_no_id ) ) );
    m_ui->lineEdit_kk_taksilat_borc_birimi->setText         ( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( M_FIS->hesap_no_id ) ) );
    m_ui->lineEdit_kk_odeme_alacak_birimi->setText          ( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( M_FIS->hesap_no_id ) ) );



    m_ui->lineEdit_banka_adi->setText( BNK_GET_BANKA_ADI_SUBE_KODU_ADI_HESABI_NO( M_FIS->hesap_no_id ));
    m_ui->adakDate_tarih->SET_DATE(  M_FIS->fis_tarihi );
    m_ui->limitedTextEdit_aciklama->setText( M_FIS->aciklama );

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::CHECK_VAR
***************************************************************************************/

int BNK_ACILIS_FORMU::CHECK_VAR (QObject * object)
{

    if( object EQ m_ui->commaEdit_toplam_alacak ) {
        if( m_ui->commaEdit_toplam_borc->GET_DOUBLE() NE 0.00 ) {
            MSG_WARNING( tr( "Borç veya Alacak girebilirsiniz." ), m_ui->commaEdit_toplam_alacak );
            return ADAK_FAIL_UNDO;
        }
    }
    else if ( object EQ m_ui->commaEdit_toplam_borc) {
        if( m_ui->commaEdit_toplam_alacak->GET_DOUBLE() NE 0.00 ) {
            MSG_WARNING( tr( "Borç veya Alacak girebilirsiniz." ), m_ui->commaEdit_toplam_borc );
            return ADAK_FAIL_UNDO;
        }
    }
    else if( object EQ m_ui->commaEdit_kk_odeme_alacak ) {
        if( m_ui->commaEdit_kk_odeme_borc->GET_DOUBLE() NE 0.00 ) {
            MSG_WARNING( tr( "Borç veya Alacak girebilirsiniz." ), m_ui->commaEdit_kk_odeme_alacak );
            return ADAK_FAIL_UNDO;
        }
    }
    else if ( object EQ m_ui->commaEdit_kk_odeme_borc) {
        if( m_ui->commaEdit_kk_odeme_alacak->GET_DOUBLE() NE 0.00 ) {
            MSG_WARNING( tr( "Borç veya Alacak girebilirsiniz." ), m_ui->commaEdit_kk_odeme_borc);
            return ADAK_FAIL_UNDO;
        }
    }
    else if ( object EQ m_ui->searchEdit_banka_hesabi ) {
        SQL_QUERY query ( DB );

        query.PREPARE_SELECT( "bnk_hesaplar", "hesap_no_id","banka_hesap_kodu = :banka_hesap_kodu" );
        query.SET_VALUE     ( ":banka_hesap_kodu", m_ui->searchEdit_banka_hesabi->GET_TEXT() );

        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING( tr( "Böyle bir Banka Hesabı yok." ), m_ui->searchEdit_banka_hesabi );
            return ADAK_FAIL_UNDO;
        }

        query.NEXT();

        m_bnk_hesap_no_id = query.VALUE( 0 ).toInt();

        m_banka_hesabi_doviz_id = BNK_GET_PARA_BIRIM_ID( m_bnk_hesap_no_id );

        m_ui->searchEdit_banka_hesabi->SET_TEXT( BNK_GET_BANKA_KODU( m_bnk_hesap_no_id ) );
        m_ui->lineEdit_banka_adi->setText( BNK_GET_BANKA_ADI_SUBE_KODU_ADI_HESABI_NO( m_bnk_hesap_no_id ));
        m_ui->lineEdit_banka_doviz_kodu->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( m_bnk_hesap_no_id ) ));

        m_ui->lineEdit_hesap_bakiyesi_alacak_birimi->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( m_bnk_hesap_no_id ) ));
        m_ui->lineEdit_hesap_bakiyesi_borc_birimi->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( m_bnk_hesap_no_id ) ));
        m_ui->lineEdit_kk_odeme_borc_birimi->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( m_bnk_hesap_no_id ) ));
        m_ui->lineEdit_kk_taksilat_borc_birimi->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( m_bnk_hesap_no_id ) ));

    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::CHECK_EMPTY
***************************************************************************************/

int BNK_ACILIS_FORMU::CHECK_EMPTY ()
{

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::CHECK_ADD
***************************************************************************************/

int BNK_ACILIS_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::ADD_RECORD
***************************************************************************************/

int BNK_ACILIS_FORMU::ADD_RECORD ()
{  

    BNK_HAREKET_FIS_STRUCT         * BNK_HRK_FIS        = new BNK_HAREKET_FIS_STRUCT;
    BNK_HAREKET_FIS_SATIRI_STRUCT  * BNK_HRK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

    BNK_CLEAR_HAREKET_FIS_STRUCT        ( BNK_HRK_FIS );
    BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT ( BNK_HRK_FIS_SATIRI );

    BNK_HRK_FIS->program_id                       = E9_PROGRAMI;
    BNK_HRK_FIS->modul_id                         = BANKA_MODULU;
    BNK_HRK_FIS->base_fis_id                      = 0;
    BNK_HRK_FIS->fis_tarihi                       = m_ui->adakDate_tarih->DATE();
    BNK_HRK_FIS->aciklama                         = m_ui->limitedTextEdit_aciklama->toPlainText();
    BNK_HRK_FIS->fis_turu                         = ENUM_BANKA_ACILIS_FISI;
    BNK_HRK_FIS->base_fis_turu                    = ENUM_BANKA_ACILIS_FISI;
    BNK_HRK_FIS->muh_fis_id                       = -1; // hareket 0 oldugunda muh fis uretiyor rasgele deger gonderdim
    BNK_HRK_FIS->odm_emr_hesabi_etkilenecek_mi    = 1;
    BNK_HRK_FIS->hesap_no_id                      = m_bnk_hesap_no_id;
    BNK_HRK_FIS->def_fis_id                       = 0;
    BNK_HRK_FIS->para_birim_id                    = m_banka_hesabi_doviz_id;

    int bnk_fis_id = BNK_HAREKET_FIS_EKLE ( BNK_HRK_FIS );

    BNK_HRK_FIS_SATIRI->fis_id            = bnk_fis_id;
    BNK_HRK_FIS_SATIRI->program_id        = E9_PROGRAMI;
    BNK_HRK_FIS_SATIRI->modul_id          = BANKA_MODULU;
    BNK_HRK_FIS_SATIRI->base_record_id    = 0;

    if ( m_ui->commaEdit_toplam_borc->GET_DOUBLE() NE 0.00 OR m_ui->commaEdit_toplam_alacak->GET_DOUBLE() NE 0.00 ) {

        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_id   = 0;

        if ( m_ui->commaEdit_toplam_borc->GET_DOUBLE() NE 0.00 ) {
            BNK_HRK_FIS_SATIRI->hesaba_giren  = m_ui->commaEdit_toplam_borc->GET_DOUBLE();
            BNK_HRK_FIS_SATIRI->hesaptan_cikan= 0.00;
        }
        else {
            BNK_HRK_FIS_SATIRI->hesaba_giren  = 0.00;
            BNK_HRK_FIS_SATIRI->hesaptan_cikan= m_ui->commaEdit_toplam_alacak->GET_DOUBLE();

        }

        BNK_HAREKET_FIS_SATIRI_EKLE ( BNK_HRK_FIS, BNK_HRK_FIS_SATIRI );

    }
    if ( m_ui->commaEdit_kk_odeme_borc->GET_DOUBLE() NE 0.00  OR m_ui->commaEdit_kk_odeme_alacak->GET_DOUBLE() NE 0.00 ) {

        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_turu    = ENUM_BNK_KREDI_KARTI_HESABI;
        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_id      = 0;
        BNK_HRK_FIS_SATIRI->base_record_id            = 0;

        if ( m_ui->commaEdit_kk_odeme_borc->GET_DOUBLE() > 0.00 ) {
            BNK_HRK_FIS_SATIRI->hesaba_giren              = m_ui->commaEdit_kk_odeme_borc->GET_DOUBLE();
            BNK_HRK_FIS_SATIRI->hesaptan_cikan            = 0.00;
        }
        else {
            BNK_HRK_FIS_SATIRI->hesaba_giren              = 0.00;
            BNK_HRK_FIS_SATIRI->hesaptan_cikan            = m_ui->commaEdit_kk_odeme_alacak->GET_DOUBLE();
        }

        BNK_HAREKET_FIS_SATIRI_EKLE ( BNK_HRK_FIS, BNK_HRK_FIS_SATIRI );

    }
    if ( m_ui->commaEdit_kk_tahsilat_borc->GET_DOUBLE() NE 0.00 ) {

        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_turu    = ENUM_BNK_POS_HESABI;
        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_id      = 0;
        BNK_HRK_FIS_SATIRI->base_record_id            = 0;

        BNK_HRK_FIS_SATIRI->hesaba_giren              = m_ui->commaEdit_kk_tahsilat_borc->GET_DOUBLE();
        BNK_HRK_FIS_SATIRI->hesaptan_cikan            = 0.00;

        BNK_HAREKET_FIS_SATIRI_EKLE ( BNK_HRK_FIS, BNK_HRK_FIS_SATIRI );

    }

    delete BNK_HRK_FIS;
    delete BNK_HRK_FIS_SATIRI;

    return bnk_fis_id;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::CHECK_UPDATE
***************************************************************************************/

int BNK_ACILIS_FORMU::CHECK_UPDATE ( int record_id )
{
    Q_UNUSED( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::UPDATE_RECORD
***************************************************************************************/

void BNK_ACILIS_FORMU::UPDATE_RECORD ( int record_id )
{

    BNK_HAREKET_FIS_STRUCT         * BNK_HRK_FIS        = new BNK_HAREKET_FIS_STRUCT;
    BNK_HAREKET_FIS_SATIRI_STRUCT  * BNK_HRK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

    BNK_HRK_FIS->fis_tarihi       = m_ui->adakDate_tarih->DATE();
    BNK_HRK_FIS->aciklama         = m_ui->limitedTextEdit_aciklama->toPlainText();

    BNK_HRK_FIS->base_fis_id  =0;

    BNK_HAREKET_FIS_BILGILERINI_OKU( BNK_HRK_FIS, record_id );

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hareket_fis_satirlari", "fis_satiri_id","fis_id = :fis_id" );
    query.SET_VALUE     ( ":fis_id", record_id );

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {

            BNK_HRK_FIS->base_fis_id = 0; // donen m_fis ' te bas_fis_ degisiyor 0 lamak gerekiyor
            BNK_HAREKET_FIS_SATIRI_SIL( BNK_HRK_FIS, query.VALUE( 0 ).toInt() );
        }
    }

    BNK_HAREKET_FISI_GUNCELLE( BNK_HRK_FIS, record_id );

    BNK_HRK_FIS_SATIRI->fis_id            = record_id;
    BNK_HRK_FIS_SATIRI->program_id        = E9_PROGRAMI;
    BNK_HRK_FIS_SATIRI->modul_id          = BANKA_MODULU;
    BNK_HRK_FIS_SATIRI->base_record_id    = 0;


    if ( m_ui->commaEdit_toplam_borc->GET_DOUBLE() NE 0.00 OR m_ui->commaEdit_toplam_alacak->GET_DOUBLE() NE 0.00 ) {

        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_turu    = ENUM_BNK_BANKA_HESABI;
        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_id      = 0;

        if ( m_ui->commaEdit_toplam_borc->GET_DOUBLE() NE 0.00 ) {
            BNK_HRK_FIS_SATIRI->hesaba_giren  = m_ui->commaEdit_toplam_borc->GET_DOUBLE();
            BNK_HRK_FIS_SATIRI->hesaptan_cikan= 0.00;
        }
        else {
            BNK_HRK_FIS_SATIRI->hesaba_giren  = 0.00;
            BNK_HRK_FIS_SATIRI->hesaptan_cikan= m_ui->commaEdit_toplam_alacak->GET_DOUBLE();

        }
        BNK_HAREKET_FIS_SATIRI_EKLE ( BNK_HRK_FIS, BNK_HRK_FIS_SATIRI );
    }

    BNK_HRK_FIS_SATIRI->base_record_id= 0;
    if ( m_ui->commaEdit_kk_odeme_borc->GET_DOUBLE() NE 0.00 OR m_ui->commaEdit_kk_odeme_alacak->GET_DOUBLE() NE 0.00 ) {

        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_turu      = ENUM_BNK_KREDI_KARTI_HESABI;
        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_id        = 0;

        if ( m_ui->commaEdit_kk_odeme_borc->GET_DOUBLE() NE 0.00 ) {
            BNK_HRK_FIS_SATIRI->hesaba_giren    = m_ui->commaEdit_kk_odeme_borc->GET_DOUBLE();
            BNK_HRK_FIS_SATIRI->hesaptan_cikan  = 0.00;
        }
        else {
            BNK_HRK_FIS_SATIRI->hesaba_giren    = 0.00;
            BNK_HRK_FIS_SATIRI->hesaptan_cikan  = m_ui->commaEdit_kk_odeme_alacak->GET_DOUBLE();
        }
        BNK_HAREKET_FIS_SATIRI_EKLE ( BNK_HRK_FIS, BNK_HRK_FIS_SATIRI );
    }

    BNK_HRK_FIS_SATIRI->base_record_id= 0;
    if ( m_ui->commaEdit_kk_tahsilat_borc->GET_DOUBLE() NE 0.00 ) {

        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_turu    = ENUM_BNK_POS_HESABI;
        BNK_HRK_FIS_SATIRI->etkilenecek_hesap_id      = 0;

        BNK_HRK_FIS_SATIRI->hesaba_giren              = m_ui->commaEdit_kk_tahsilat_borc->GET_DOUBLE();
        BNK_HRK_FIS_SATIRI->hesaptan_cikan            = 0.00;

        BNK_HAREKET_FIS_SATIRI_EKLE ( BNK_HRK_FIS, BNK_HRK_FIS_SATIRI );

    }
    BNK_HRK_FIS->base_fis_id = 0;
    BNK_HRK_FIS_SATIRI->base_record_id= 0; // new ile uretildigi icin update donerken degerler degisiyor bundan dolayi sifirlaniyor

    delete BNK_HRK_FIS;
    delete BNK_HRK_FIS_SATIRI;

}

/**************************************************************************************
                   BNK_ACILIS_FORMU::CHECK_DELETE
***************************************************************************************/

int BNK_ACILIS_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::DELETE_RECORD
***************************************************************************************/

void BNK_ACILIS_FORMU::DELETE_RECORD ( int record_id )
{
    BNK_HAREKET_FIS_STRUCT         * BNK_HRK_FIS        = new BNK_HAREKET_FIS_STRUCT;

    BNK_CLEAR_HAREKET_FIS_STRUCT    ( BNK_HRK_FIS );

    BNK_HAREKET_FIS_BILGILERINI_OKU( BNK_HRK_FIS, record_id );


    BNK_HAREKET_FISI_SIL( BNK_HRK_FIS, record_id );

    delete BNK_HRK_FIS;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int BNK_ACILIS_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY select_query(DB);

    QString where_str = "fis_turu = :fis_turu AND modul_id = :modul_id";

    select_query.PREPARE_SELECT ( "bnk_hareket_fisler","fis_id",where_str);

    select_query.SET_VALUE(":fis_turu" , m_fis_turu   );
    select_query.SET_VALUE(":modul_id" , BANKA_MODULU );

    if ( select_query.SELECT(" fis_tarihi, fis_no" , 0 ,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int BNK_ACILIS_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY select_query(DB);

    QString where_str = "fis_turu  = :fis_turu AND modul_id = :modul_id";

    select_query.PREPARE_SELECT ( "bnk_hareket_fisler","fis_id", where_str );

    select_query.SET_VALUE(":fis_turu" , m_fis_turu        );
    select_query.SET_VALUE(":modul_id" , BANKA_MODULU     );


    if ( select_query.SELECT(" fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int BNK_ACILIS_FORMU::FIND_NEXT_RECORD ()
{

    SQL_QUERY select_query(DB);

    QString where_str = "fis_no   > :fis_no                 "
                        "AND   fis_turu       = :fis_turu         "
                        "AND   modul_id       = :modul_id         "
                        "AND   fis_tarihi     = :fis_tarihi       ";

    select_query.PREPARE_SELECT( "bnk_hareket_fisler" ,"fis_id",where_str );

    select_query.SET_VALUE(":fis_no"       , M_FIS->fis_no  );
    select_query.SET_VALUE(":fis_turu"     , m_fis_turu     );
    select_query.SET_VALUE(":modul_id"     , BANKA_MODULU   );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_tarih->DATE());

    if ( select_query.SELECT("fis_no",0,1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    where_str = "fis_turu       = :fis_turu       "
                "AND   modul_id       = :modul_id       "
                "AND   fis_tarihi     > :fis_tarihi     ";

    select_query.PREPARE_SELECT( "bnk_hareket_fisler","fis_id",where_str );

    select_query.SET_VALUE(":fis_turu"     , m_fis_turu    );
    select_query.SET_VALUE(":modul_id"     , BANKA_MODULU  );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_tarih->DATE());

    if ( select_query.SELECT("fis_tarihi ASC, fis_no ASC",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   BNK_ACILIS_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int BNK_ACILIS_FORMU::FIND_PREV_RECORD ()
{

    SQL_QUERY select_query(DB);

    QString where_str = "fis_no   < :fis_no              "
                        "AND   fis_turu       = :fis_turu      "
                        "AND   modul_id       = :modul_id      "
                        "AND   fis_tarihi     = :fis_tarihi    ";

    select_query.PREPARE_SELECT( "bnk_hareket_fisler","fis_id",where_str );

    select_query.SET_VALUE(":fis_no"       , M_FIS->fis_no );
    select_query.SET_VALUE(":fis_turu"     , m_fis_turu    );
    select_query.SET_VALUE(":modul_id"     , BANKA_MODULU  );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_tarih->DATE());

    if ( select_query.SELECT("fis_no DESC",0,1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    where_str = "fis_turu       = :fis_turu      "
                "AND   modul_id       = :modul_id      "
                "AND   fis_tarihi     < :fis_tarihi    ";

    select_query.PREPARE_SELECT( "bnk_hareket_fisler","fis_id", where_str );

    select_query.SET_VALUE(":fis_turu"     , m_fis_turu    );
    select_query.SET_VALUE(":modul_id"     , BANKA_MODULU  );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_tarih->DATE());

    if ( select_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::SELECT_RECORD
***************************************************************************************/

int BNK_ACILIS_FORMU::SELECT_RECORD()
{
    return OPEN_BNK_HAREKET_FISI_ARAMA ( ENUM_BANKA_ACILIS_FISI, BANKA_MODULU, ENUM_BANKA_FISI, this );
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::LOCK_RECORD
***************************************************************************************/

int BNK_ACILIS_FORMU::LOCK_RECORD( int record_id )
{
    return DB->LOCK_ROW ( "fis_id", "bnk_hareket_fisler", QString ( "fis_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::UNLOCK_RECORD
***************************************************************************************/

void BNK_ACILIS_FORMU::UNLOCK_RECORD( int record_id )
{
    DB->UNLOCK_ROW ( "fis_id", "bnk_hareket_fisler", QString ( "fis_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::FIND_ENTERED_RECORD
***************************************************************************************/

int BNK_ACILIS_FORMU::FIND_RECORD()
{
    return 0;
}

/**************************************************************************************
                   BNK_ACILIS_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void BNK_ACILIS_FORMU::SEARCH_EDIT_CLICKED ( QWidget * p_widget, QLineEdit * line_edit)
{
    Q_UNUSED ( line_edit );

    if ( p_widget EQ m_ui->searchEdit_banka_hesabi ){
        int banka_hesap_id = OPEN_BNK_HESAPLARI_ARAMA ( this );

        m_bnk_hesap_no_id = banka_hesap_id;

        m_ui->searchEdit_banka_hesabi->SET_TEXT( BNK_GET_BANKA_KODU( banka_hesap_id ) );
        m_ui->lineEdit_banka_adi->setText( BNK_GET_BANKA_ADI_SUBE_KODU_ADI_HESABI_NO( banka_hesap_id ));
        m_ui->lineEdit_banka_doviz_kodu->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( banka_hesap_id ) ));

        m_ui->lineEdit_hesap_bakiyesi_alacak_birimi->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( banka_hesap_id ) ));
        m_ui->lineEdit_hesap_bakiyesi_borc_birimi->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( banka_hesap_id ) ));
        m_ui->lineEdit_kk_odeme_borc_birimi->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( banka_hesap_id ) ));
        m_ui->lineEdit_kk_taksilat_borc_birimi->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( banka_hesap_id ) ));

    }

}
