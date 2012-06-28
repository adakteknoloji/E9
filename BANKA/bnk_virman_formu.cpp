#include "bnk_virman_formu_class.h"
#include "ui_bnk_virman_formu.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_log.h"
#include "bnk_fis_utils.h"
#include "banka_enum.h"
#include "adak_std_utils.h"
#include "bnk_defter_fis_utils.h"
#include "muh_fis_utils.h"
#include "muh_enum.h"
#include "bnk_hareket_fisi_arama_open.h"
#include "bnk_virman_formu_open.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "bnk_gui_utils.h"
#include "bnk_console_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_BNK_VIRMAN_FORMU
***************************************************************************************/

void OPEN_BNK_VIRMAN_FORMU ( int p_form_id, QWidget * parent )
{
    BNK_VIRMAN_FORMU * F = new BNK_VIRMAN_FORMU ( p_form_id, parent );
    F->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::BNK_VIRMAN_FORMU
***************************************************************************************/

BNK_VIRMAN_FORMU:: BNK_VIRMAN_FORMU ( int p_form_id, QWidget * parent ) : FORM_KERNEL ( parent ), m_ui(new Ui:: BNK_VIRMAN_FORMU)
{
    m_form_id = p_form_id;
    m_fis_turu  = ENUM_BANKA_VIRMAN_FISI;

    m_ui->setupUi     ( this );
    START_FORM_KERNEL ( this , DB );
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::SETUP_FORM
***************************************************************************************/

void  BNK_VIRMAN_FORMU::SETUP_FORM ()
{
    SET_PAGE_TITLE    (tr ( "BNK - BANKA VİRMAN FORMU" ) );
    SET_HELP_PAGE     ("banka-islemleri_banka-virman-formu");

    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget );

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );

    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    m_ui->commaEdit_kur->setVisible( false );
    m_ui->label_kur->setVisible( false );
     
    M_FIS           = new BNK_DEFTER_FIS_STRUCT;
    M_FIS_SATIRI    = new BNK_DEFTER_FIS_SATIRI_STRUCT;

    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_fis_filtresi );

    m_ui->adakDate_tarih->SET_DATE( QDate::currentDate() );
    m_ui->comboBox_fis_filtresi->setVisible( false );
    m_ui->label_fis_filtresi->setVisible(false);

    m_ui->comboBox_sirket_turu->setVisible(false);
    m_ui->label_sirket->setVisible(false);

    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI ( m_ui->comboBox_gonderen_hesap );
    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI ( m_ui->comboBox_alici_hesap    );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->comboBox_fis_filtresi->setVisible( true );
        m_ui->label_fis_filtresi->setVisible(true);

        m_ui->label_sirket->setVisible(true);
        m_ui->comboBox_sirket_turu->setVisible( true );
    }

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_fis_no );
    SET_SETTING_NAME       ( "BANAKA_VIRMAN_FORMU" );

    m_ui->commaEdit_gonderen_tutar->SET_PRECISION      ( 2 );
    m_ui->commaEdit_alan_tutar->SET_PRECISION          ( 2 );
    m_ui->commaEdit_kur->SET_PRECISION                 ( 4 );


    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH       ( 512);
    m_ui->lineEdit_gonderen_hesap_adi->setMaxLength             ( 50 );
    m_ui->lineEdit_alici_hesap_adi->setMaxLength                ( 50 );
    m_ui->commaEdit_gonderen_tutar->setMaxLength                ( 15 );
    m_ui->commaEdit_alan_tutar->setMaxLength                    ( 15 );
    m_ui->commaEdit_kur->setMaxLength                           ( 8  );
    m_ui->lineEdit_fis_no->setMaxLength                         ( 10 );

    DISABLE_CLEAR_ON_WIDGET( m_ui->adakDate_tarih );

    AUTOSTART_FORM_KERNEL( m_form_id );

}


/**************************************************************************************
                   BNK_VIRMAN_FORMU::NEW_RECORD
***************************************************************************************/

void  BNK_VIRMAN_FORMU::NEW_RECORD ()
{
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void BNK_VIRMAN_FORMU::CLEAR_FORM_MEMBERS()
{
    m_gonderen_banka_adi.clear();
    m_alici_banka_adi.clear();

    m_gonderen_hesap_no_id    = 0;
    m_alici_hesap_no_id       = 0;

    m_gon_hesabi_doviz_id     = 0;
    m_alici_hesabi_doviz_id   = 0;

    CLEAR_BNK_DEFTER_FIS_STRUCT        ( M_FIS );
    CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::GET_RECORD
***************************************************************************************/

int BNK_VIRMAN_FORMU::GET_RECORD ( int record_id )
{

    SQL_QUERY query( DB );
    query.PREPARE_SELECT( "bnk_virman_fisler","fis_no, fis_tarihi , aciklama","fis_id =:fis_id" );
    query.SET_VALUE     ( ":fis_id", record_id );

    if ( query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    query.NEXT();

    m_ui->lineEdit_fis_no->setText  ( QVariant ( query.VALUE( 0 ).toInt() ).toString() );
    m_ui->adakDate_tarih->SET_DATE(  query.VALUE( 1 ).toString());
    m_ui->limitedTextEdit_aciklama->insertPlainText( query.VALUE( 2 ).toString() );

    query.PREPARE_SELECT( "bnk_virman_fis_satirlari","gonderen_hesap_no_id, alici_hesap_no_id, "
                          "doviz_kuru, gonderen_islem_tutari, alici_islem_tutari, doviz_kuru",
                          "fis_id =:fis_id" );
    query.SET_VALUE     ( ":fis_id", record_id );

    if ( query.SELECT() NE 0 ) {

        query.NEXT();

        m_gonderen_hesap_no_id  = query.VALUE( 0 ).toInt();
        m_alici_hesap_no_id     = query.VALUE( 1 ).toInt();

        m_ui->commaEdit_kur->SET_DOUBLE           ( query.VALUE( 2 ).toDouble() );
        m_ui->commaEdit_gonderen_tutar->SET_DOUBLE( query.VALUE( 3 ).toDouble() );
        m_ui->commaEdit_alan_tutar->SET_DOUBLE    ( query.VALUE( 4 ).toDouble() );
        m_ui->commaEdit_kur->SET_DOUBLE           ( query.VALUE( 5 ).toDouble() );
    }

    // gonderen banka
    int row = BNK_HESAPLARI_MODEL_INDEX_BUL ( m_gonderen_hesap_no_id, m_ui->comboBox_gonderen_hesap );

    if ( row > -1 ) {

        m_ui->comboBox_gonderen_hesap->setCurrentIndex ( row );

        m_gonderen_hesap_no_id    = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 0 ).toInt();
        m_gon_hesabi_doviz_id   = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 1 ).toInt();

        m_ui->lineEdit_gonderen_hesap_doviz_kodu->setText ( DVZ_GET_DOVIZ_KODU ( m_gon_hesabi_doviz_id ) );
        m_ui->lineEdit_gonderenin_birim->setText          ( DVZ_GET_DOVIZ_KODU ( m_gon_hesabi_doviz_id ) );

        QString banka_bilgisi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 3 ).toString() + " / " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 4 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 5 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 6 ).toString();

        m_ui->lineEdit_gonderen_hesap_adi->setText ( banka_bilgisi );

    }

    // alici banka
    row = BNK_HESAPLARI_MODEL_INDEX_BUL ( m_alici_hesap_no_id, m_ui->comboBox_alici_hesap );

    if ( row > -1 ) {

        m_ui->comboBox_alici_hesap->setCurrentIndex ( row );

        m_alici_hesap_no_id       = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->comboBox_alici_hesap, row, 0 ).toInt();
        m_alici_hesabi_doviz_id   = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->comboBox_alici_hesap, row, 1 ).toInt();

        m_ui->lineEdit_alici_hesap_doviz_kodu->setText ( DVZ_GET_DOVIZ_KODU ( m_alici_hesabi_doviz_id ) );
        m_ui->lineEdit_alanin_birimi->setText( DVZ_GET_DOVIZ_KODU ( m_alici_hesabi_doviz_id ) );

        QString banka_bilgisi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_alici_hesap, row, 3 ).toString() + " / " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_alici_hesap, row, 4 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_alici_hesap, row, 5 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_alici_hesap, row, 6 ).toString();

        m_ui->lineEdit_alici_hesap_adi->setText ( banka_bilgisi );
    }

    if ( m_gon_hesabi_doviz_id NE m_alici_hesabi_doviz_id ) {
        if( m_gon_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
        }
        else if( m_alici_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
        }
        else {
            m_ui->commaEdit_kur->setVisible( true );
        }
    }
    else {
        m_ui->commaEdit_alan_tutar->setEnabled( false );
        if ( m_gon_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
        }
        else {
            m_ui->commaEdit_kur->setVisible( true );
        }
    }
    return ADAK_OK;

}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::CHECK_VAR
***************************************************************************************/

int BNK_VIRMAN_FORMU::CHECK_VAR (QObject * object)
{
    if ( object EQ m_ui->comboBox_gonderen_hesap ) {
        int row = -1;

        row = m_ui->comboBox_gonderen_hesap->view()->currentIndex().row();

        if ( row EQ -1 ) {
            return ADAK_FAIL;
        }

        m_gonderen_hesap_no_id    = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->comboBox_gonderen_hesap, row, 0 ).toInt();
        m_gon_hesabi_doviz_id   = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 1 ).toInt();

        m_ui->lineEdit_gonderen_hesap_doviz_kodu->setText   ( DVZ_GET_DOVIZ_KODU ( m_gon_hesabi_doviz_id ) );
        m_ui->lineEdit_gonderenin_birim->setText            ( DVZ_GET_DOVIZ_KODU ( m_gon_hesabi_doviz_id ) );

        m_gonderen_banka_adi  = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 4 ).toString();

        QString banka_bilgisi =BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 3 ).toString() + " / " +
                               BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 4 ).toString() + " " +
                               BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 5 ).toString() + " " +
                               BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_gonderen_hesap, row, 6 ).toString();

        m_ui->lineEdit_gonderen_hesap_adi->setText ( banka_bilgisi );
        m_ui->comboBox_gonderen_hesap->setCurrentIndex(row);

    }
    else if ( object EQ m_ui->comboBox_alici_hesap ) {

         int row = -1;
         row = m_ui->comboBox_alici_hesap->view()->currentIndex().row();

         if ( row EQ -1 ) {
             return ADAK_FAIL;
         }

         m_alici_banka_adi  = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_alici_hesap, row, 4 ).toString();

         if ( m_alici_banka_adi NE m_gonderen_banka_adi ) {
             MSG_WARNING( tr("Farklı Bankalar Arasında Virman Yapamzsınız"), m_ui->comboBox_alici_hesap );
             return ADAK_FAIL_UNDO;
         }

         m_alici_hesap_no_id         = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_alici_hesap, row, 0 ).toInt();
         m_alici_hesabi_doviz_id   = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->comboBox_alici_hesap, row, 1 ).toInt();

         m_ui->lineEdit_alici_hesap_doviz_kodu->setText ( DVZ_GET_DOVIZ_KODU ( m_alici_hesabi_doviz_id ) );
         m_ui->lineEdit_alanin_birimi->setText          ( DVZ_GET_DOVIZ_KODU ( m_alici_hesabi_doviz_id ) );

         QString banka_bilgisi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_alici_hesap, row, 3 ).toString() + " / " +
                                 BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->comboBox_alici_hesap, row, 4 ).toString() + " " +
                                 BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->comboBox_alici_hesap, row, 5 ).toString() + " " +
                                 BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_alici_hesap, row, 6 ).toString();

         m_ui->lineEdit_alici_hesap_adi->setText ( banka_bilgisi );
         m_ui->comboBox_alici_hesap->setCurrentIndex(row);


         m_ui->commaEdit_kur->setVisible    ( false );
         m_ui->label_kur->setVisible        ( false );

         if ( m_gon_hesabi_doviz_id NE m_alici_hesabi_doviz_id ) {
             if( m_gon_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
             }
             else if( m_alici_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
             }
             else {
                 m_ui->commaEdit_kur->setVisible    ( true );
                 m_ui->label_kur->setVisible        ( true );
                 m_ui->commaEdit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( m_gon_hesabi_doviz_id, m_ui->adakDate_tarih->QDATE(), ENUM_SATIS ));
             }
         }
         else {
             m_ui->commaEdit_alan_tutar->setEnabled( false );

             if ( m_gon_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
             }
             else {
                 m_ui->commaEdit_kur->setVisible    ( true );
                 m_ui->label_kur->setVisible        ( true );
                 m_ui->commaEdit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( m_gon_hesabi_doviz_id, m_ui->adakDate_tarih->QDATE(), ENUM_SATIS ));
             }
         }

         if ( m_gonderen_hesap_no_id NE 0 OR m_alici_hesap_no_id NE 0 ) {
             if ( m_gonderen_hesap_no_id EQ m_alici_hesap_no_id ) {
                 MSG_WARNING( tr( "Aynı Bankaya Virman İşlemi yapamazsınız." ), m_ui->comboBox_gonderen_hesap );
                 return ADAK_FAIL_UNDO;
             }
         }
     }

     else if ( object EQ m_ui->commaEdit_gonderen_tutar ) {
         if ( m_gon_hesabi_doviz_id EQ m_alici_hesabi_doviz_id ) {
             m_ui->commaEdit_alan_tutar->SET_DOUBLE( m_ui->commaEdit_gonderen_tutar->GET_DOUBLE() );
         }
     }

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::CHECK_EMPTY
***************************************************************************************/

int BNK_VIRMAN_FORMU::CHECK_EMPTY ()
{

    if ( m_ui->lineEdit_fis_no->text().isEmpty() EQ true ) {
        MSG_INFO( tr("Yeni Banka Fişi kaydı için (*) girmelisiniz"), m_ui->lineEdit_fis_no );
        return ADAK_FAIL;
    }
    if ( m_gonderen_hesap_no_id EQ 0 ) {
        MSG_WARNING( tr( "Gönderen Banka hesabını girmek zorundasınız." ), m_ui->comboBox_gonderen_hesap );
        return ADAK_FAIL;
    }
    if ( m_alici_hesap_no_id EQ 0 ) {
        MSG_WARNING( tr( "Alıcı Banka hesabını girmek zorundasınız." ), m_ui->comboBox_gonderen_hesap );
        return ADAK_FAIL;
    }
    if ( m_ui->commaEdit_gonderen_tutar->GET_DOUBLE() EQ 0.00 ) {
        MSG_WARNING( tr( "Tutarı girmek zorundasınız." ), m_ui->commaEdit_gonderen_tutar );
        return ADAK_FAIL;
    }
    if ( m_gon_hesabi_doviz_id NE m_alici_hesabi_doviz_id ) {
        if ( m_ui->commaEdit_alan_tutar->GET_DOUBLE() EQ 0.00 ) {
            MSG_WARNING( tr( "Tutarı girmek zorundasınız." ), m_ui->commaEdit_alan_tutar );
            return ADAK_FAIL;
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::CHECK_ADD
***************************************************************************************/

int BNK_VIRMAN_FORMU::CHECK_ADD ()
{

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::ADD_RECORD
***************************************************************************************/

int BNK_VIRMAN_FORMU::ADD_RECORD ()
{
    BNK_VIRMAN_FIS_AND_SATIRI_STRUCT * VIRMAN_STRUCT = new BNK_VIRMAN_FIS_AND_SATIRI_STRUCT;

    CLEAR_BNK_VIRMAN_FIS_SATIRI_STRUCT( VIRMAN_STRUCT );

    VIRMAN_STRUCT->alici_islem_tutari           = m_ui->commaEdit_alan_tutar->GET_DOUBLE();
    if ( m_gon_hesabi_doviz_id NE m_alici_hesabi_doviz_id ) {
         VIRMAN_STRUCT->alici_islem_tutari           = m_ui->commaEdit_gonderen_tutar->GET_DOUBLE();
    }

    VIRMAN_STRUCT->fis_id                       = 0; // fis olusturulacak ise
    VIRMAN_STRUCT->fis_no                       = 0;
    VIRMAN_STRUCT->aciklama                     = m_ui->limitedTextEdit_aciklama->toPlainText();
    VIRMAN_STRUCT->gonderen_hesap_no_id         = m_gonderen_hesap_no_id;
    VIRMAN_STRUCT->gonderen_islem_tutari        = m_ui->commaEdit_gonderen_tutar->GET_DOUBLE();
    VIRMAN_STRUCT->alici_hesap_no_id            = m_alici_hesap_no_id;

    VIRMAN_STRUCT->doviz_kuru                   = m_ui->commaEdit_kur->GET_DOUBLE();
    VIRMAN_STRUCT->gonderen_hesap_doviz_id      = m_gon_hesabi_doviz_id;
    VIRMAN_STRUCT->alici_hesap_doviz_id         = m_alici_hesabi_doviz_id;
    VIRMAN_STRUCT->fis_tarihi                   = m_ui->adakDate_tarih->DATE();


    BNK_VIRMAN_FIS_SATIRI_EKLE( VIRMAN_STRUCT );

    return 0;
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::CHECK_UPDATE
***************************************************************************************/

int BNK_VIRMAN_FORMU::CHECK_UPDATE ( int record_id )
{
    Q_UNUSED( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::UPDATE_RECORD
***************************************************************************************/

void BNK_VIRMAN_FORMU::UPDATE_RECORD ( int record_id )
{
    // hesaplama
    double islem_tutari     = 0.00;

    if ( m_gon_hesabi_doviz_id NE m_alici_hesabi_doviz_id) {
        if(m_gon_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
            islem_tutari    = m_ui->commaEdit_gonderen_tutar->GET_DOUBLE();
        }
        else if( m_alici_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
            islem_tutari    = m_ui->commaEdit_alan_tutar->GET_DOUBLE();
        }
        else {
            islem_tutari    = ROUND( m_ui->commaEdit_gonderen_tutar->GET_DOUBLE() * m_ui->commaEdit_kur->GET_DOUBLE() );
        }
    }
    else {
        if ( m_gon_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
            islem_tutari    = m_ui->commaEdit_gonderen_tutar->GET_DOUBLE();
        }
        else {
            islem_tutari    = ROUND( m_ui->commaEdit_gonderen_tutar->GET_DOUBLE() * m_ui->commaEdit_kur->GET_DOUBLE() );
        }
    }

    SQL_QUERY query( DB );

    query.PREPARE_SELECT ( "bnk_virman_fisler","def_fis_id, alici_def_fis_id","fis_id = :fis_id  " );
    query.SET_VALUE      ( ":fis_id" , record_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int         def_fis_id          =   query.VALUE ( 0 ).toInt();
    int         alici_def_fis_id    =   query.VALUE ( 1 ).toInt();

    QStringList def_row_list    =   GET_BNK_DEFTER_FIS_ROW_ID_LIST( E9_PROGRAMI, BANKA_MODULU, def_fis_id );

    BNK_DEFTER_FIS_STRUCT * DEF_FIS     = new BNK_DEFTER_FIS_STRUCT;
    BNK_DEFTER_FIS_STRUCT * DEF_FIS_2   = new BNK_DEFTER_FIS_STRUCT;
    CLEAR_BNK_DEFTER_FIS_STRUCT( DEF_FIS );

    BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, def_fis_id );

    for ( int i = 0; i < def_row_list.count(); i++ ) {
        BNK_DEFTER_FIS_SATIRI_SIL( DEF_FIS, QVariant ( def_row_list.at( i ) ).toInt() );
    }

    CLEAR_BNK_DEFTER_FIS_STRUCT( DEF_FIS_2 );
    BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS_2, alici_def_fis_id );

    for ( int i = 0; i < def_row_list.count(); i++ ) {
        BNK_DEFTER_FIS_SATIRI_SIL( DEF_FIS_2, QVariant ( def_row_list.at( i ) ).toInt() );
    }

    query.PREPARE_UPDATE ( "bnk_virman_fisler","fis_id",
                           "aciklama, fis_tarihi, toplam_hesaba_giren, toplam_hesaptan_cikan ",
                           "fis_id = :fis_id " );

    query.SET_VALUE     ( ":aciklama"               , m_ui->limitedTextEdit_aciklama->toPlainText()       );
    query.SET_VALUE     ( ":fis_tarihi"             , m_ui->adakDate_tarih->DATE()                       );
    query.SET_VALUE     ( ":toplam_hesaba_giren"    , m_ui->commaEdit_gonderen_tutar->GET_DOUBLE()        );
    query.SET_VALUE     ( ":toplam_hesaptan_cikan"  , m_ui->commaEdit_alan_tutar->GET_DOUBLE()            );
    query.SET_VALUE     ( ":fis_id"                 , record_id                                           );

    query.UPDATE();

    //satirlar silindi
    query.PREPARE_DELETE ( "bnk_virman_fis_satirlari","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id", record_id );
    query.DELETE();


    // virman satir ekliyecegiz.

    query.PREPARE_INSERT (  "bnk_virman_fis_satirlari","fis_satiri_id" ,"fis_id, aciklama, gonderen_islem_tutari, alici_islem_tutari,   "
                            "base_record_id, order_number, hesap_turu,                       "
                            "program_id, modul_id, gonderen_hesap_no_id, alici_hesap_no_id,     "
                            "doviz_kuru, islem_tutari ");

    query.SET_VALUE ( ":fis_id"                         , record_id                                 );
    query.SET_VALUE ( ":aciklama"                       , m_ui->limitedTextEdit_aciklama->toPlainText() );
    query.SET_VALUE ( ":gonderen_islem_tutari"          , m_ui->commaEdit_gonderen_tutar->GET_DOUBLE()  );
    query.SET_VALUE ( ":alici_islem_tutari"             , m_ui->commaEdit_alan_tutar->GET_DOUBLE()      );
    query.SET_VALUE ( ":base_record_id"                 , 0                                             );
    query.SET_VALUE ( ":order_number"                   , 1204                                          );
    query.SET_VALUE ( ":hesap_turu"                     , ENUM_BNK_BANKA_HESABI                         );
    query.SET_VALUE ( ":program_id"                     , E9_PROGRAMI                                   );
    query.SET_VALUE ( ":modul_id"                       , BANKA_MODULU                                  );
    query.SET_VALUE ( ":gonderen_hesap_no_id"           , m_gonderen_hesap_no_id                        );
    query.SET_VALUE ( ":alici_hesap_no_id"              , m_alici_hesap_no_id                           );
    query.SET_VALUE ( ":doviz_kuru"                     , m_ui->commaEdit_kur->GET_DOUBLE()             );
    query.SET_VALUE ( ":islem_tutari"                   , islem_tutari                                  );

    int satir_id = query.INSERT();


    M_FIS_SATIRI->aciklama              = QString( "Virman Islemi Banka Hesap id : %1" ).arg( m_gonderen_hesap_no_id );
    M_FIS_SATIRI->fis_id                = def_fis_id;
    M_FIS_SATIRI->program_id            = E9_PROGRAMI;
    M_FIS_SATIRI->modul_id              = BANKA_MODULU;
    M_FIS_SATIRI->base_record_id        = satir_id;
    M_FIS_SATIRI->hesap_turu            = ENUM_BNK_BANKA_HESABI;
    M_FIS_SATIRI->hesap_id              = m_gonderen_hesap_no_id;
    M_FIS_SATIRI->hesaba_giren          = 0.00;
    M_FIS_SATIRI->hesaptan_cikan        = islem_tutari; // alacak

    BNK_DEFTER_FIS_SATIRI_EKLE ( DEF_FIS, M_FIS_SATIRI );

    M_FIS_SATIRI->fis_id            = alici_def_fis_id;
    M_FIS_SATIRI->aciklama          = QString( "Virman Islemi Banka Hesap id : %1" ).arg( m_alici_hesap_no_id );
    M_FIS_SATIRI->hesap_id          = m_alici_hesap_no_id;
    M_FIS_SATIRI->hesaba_giren      = islem_tutari;
    M_FIS_SATIRI->hesaptan_cikan    = 0.00;

    BNK_DEFTER_FIS_SATIRI_EKLE ( DEF_FIS_2, M_FIS_SATIRI );


    delete DEF_FIS_2;
    delete DEF_FIS;
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::CHECK_DELETE
***************************************************************************************/

int BNK_VIRMAN_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::DELETE_RECORD
***************************************************************************************/

void BNK_VIRMAN_FORMU::DELETE_RECORD ( int record_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT ( "bnk_virman_fisler","def_fis_id, muh_fis_id, alici_def_fis_id","fis_id = :fis_id  " );
    query.SET_VALUE      ( ":fis_id" , record_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int         def_fis_id          =   query.VALUE ( 0 ).toInt();
    int         muh_fis_id          =   query.VALUE ( 1 ).toInt();
    int         alici_def_fis_id    = query.VALUE( 2 ).toInt();

    BNK_DEFTER_FIS_STRUCT * DEF_FIS = new BNK_DEFTER_FIS_STRUCT;

    // gonderen bankanin defter fisi siliniyor.
    CLEAR_BNK_DEFTER_FIS_STRUCT ( DEF_FIS );
    BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, def_fis_id );
    BNK_DEFTER_FISI_SIL( DEF_FIS, def_fis_id );

    // alicinin olusturdugu fis siliniyor...
    BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, alici_def_fis_id );
    BNK_DEFTER_FISI_SIL( DEF_FIS, alici_def_fis_id );

    delete DEF_FIS;

    MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

    // Muhasebe fisleri siliniyor
    MUH_CLEAR_FIS_STRUCT( MUH_FIS );
    MUH_FIS_BILGILERINI_OKU( MUH_FIS, muh_fis_id );
    MUH_FIS_SIL( MUH_FIS, muh_fis_id );

    delete MUH_FIS;

    query.PREPARE_DELETE ( "bnk_virman_fis_satirlari","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id", record_id );
    query.DELETE();

    query.PREPARE_DELETE ( "bnk_virman_fisler","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id", record_id );
    query.DELETE();

}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int BNK_VIRMAN_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY select_query(DB);

    QString query_str = "modul_id  = :modul_id";

    select_query.PREPARE_SELECT ( "bnk_virman_fisler","fis_id", query_str );

    select_query.SET_VALUE(":modul_id" , BANKA_MODULU );

    if ( select_query.SELECT(" fis_tarihi ASC, fis_no ASC",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int BNK_VIRMAN_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY select_query(DB);

    QString where_str = "modul_id  = :modul_id";

    select_query.PREPARE_SELECT ( "bnk_virman_fisler","fis_id",where_str );

    select_query.SET_VALUE(":modul_id" , BANKA_MODULU     );
    if ( select_query.SELECT("fis_tarihi DESC, fis_no DESC") EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int BNK_VIRMAN_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY select_query(DB);

    QString where_str = "fis_no   > :fis_no                 "
                        "AND   modul_id       = :modul_id         "
                        "AND   fis_tarihi     = :fis_tarihi       ";

    select_query.PREPARE_SELECT( "bnk_virman_fisler","fis_id",where_str );

    select_query.SET_VALUE(":fis_no"       , QVariant ( m_ui->lineEdit_fis_no->text()).toInt() );
    select_query.SET_VALUE(":modul_id"     , BANKA_MODULU  );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_tarih->DATE());

    if ( select_query.SELECT("fis_no ASC",0,1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    where_str = "modul_id       = :modul_id       "
                "AND   fis_tarihi     > :fis_tarihi     ";

    select_query.PREPARE_SELECT( "bnk_virman_fisler","fis_id",where_str );

    select_query.SET_VALUE(":modul_id"     , BANKA_MODULU  );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_tarih->DATE());

    if ( select_query.SELECT("fis_tarihi ASC, fis_no ASC") EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int BNK_VIRMAN_FORMU::FIND_PREV_RECORD ()
{

    SQL_QUERY select_query(DB);

    QString where_str = "fis_no   < :fis_no              "
                        "AND   modul_id       = :modul_id      "
                        "AND   fis_tarihi     = :fis_tarihi    ";

    select_query.PREPARE_SELECT( "bnk_virman_fisler","fis_id", where_str);

    select_query.SET_VALUE(":fis_no"       , QVariant ( m_ui->lineEdit_fis_no->text()).toInt() );
    select_query.SET_VALUE(":modul_id"     , BANKA_MODULU  );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_tarih->DATE());

    if ( select_query.SELECT("fis_no DESC",0,1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    where_str = "modul_id       = :modul_id      "
                "AND   fis_tarihi     < :fis_tarihi    ";

    select_query.PREPARE_SELECT( "bnk_virman_fisler","fis_id", where_str );

    select_query.SET_VALUE(":modul_id"     , BANKA_MODULU  );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_tarih->DATE());

    if ( select_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::SELECT_RECORD
***************************************************************************************/

int BNK_VIRMAN_FORMU::SELECT_RECORD()
{
    return OPEN_BNK_HAREKET_FISI_ARAMA ( ENUM_BANKA_VIRMAN_FISI, BANKA_MODULU, ENUM_BANKA_FISI, this );
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::LOCK_RECORD
***************************************************************************************/

int BNK_VIRMAN_FORMU::LOCK_RECORD( int record_id )
{
    return DB->LOCK_ROW ( "fis_id", "bnk_virman_fisler", QString ( "fis_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::UNLOCK_RECORD
***************************************************************************************/

void BNK_VIRMAN_FORMU::UNLOCK_RECORD( int record_id )
{
    DB->UNLOCK_ROW ( "fis_id", "bnk_virman_fisler", QString ( "fis_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::FIND_ENTERED_RECORD
***************************************************************************************/

int BNK_VIRMAN_FORMU::FIND_RECORD()
{
    return 0;
}

/**************************************************************************************
                   BNK_VIRMAN_FORMU::SAVER_BUTTON_CLICKED
***************************************************************************************/

void BNK_VIRMAN_FORMU::SAVER_BUTTON_CLICKED(QAbstractButton *button, int record_id)
{
    if ( button EQ m_ui->pushButton_urettigi_fisler ) {
        int def_fis_id = GET_BNK_DEFTER_FIS_ID( E9_PROGRAMI, BANKA_MODULU, record_id );
        E9_URETILEN_FIS_SEC( E9_PROGRAMI, BANKA_MODULU, def_fis_id, this );
    }
}

/**************************************************************************************
                   BNK_VIRMAN_FIS_SATIRI_EKLE
***************************************************************************************/

void   BNK_VIRMAN_FIS_SATIRI_EKLE( BNK_VIRMAN_FIS_AND_SATIRI_STRUCT * P_VIRMAN_STRUCT , int p_fis_satiri_id ,
                                   bool p_base_fis_satiri_eklenecek , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    // hesaplama
    int    para_birim_id   = 0;
    double islem_tutari     = 0.00;

    if ( P_VIRMAN_STRUCT->gonderen_hesap_doviz_id NE P_VIRMAN_STRUCT->alici_hesap_doviz_id ) {
        if( P_VIRMAN_STRUCT->gonderen_hesap_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
            islem_tutari    = P_VIRMAN_STRUCT->gonderen_islem_tutari;
            para_birim_id   = P_VIRMAN_STRUCT->gonderen_hesap_doviz_id;
        }
        else if( P_VIRMAN_STRUCT->alici_hesap_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
            islem_tutari    = P_VIRMAN_STRUCT->alici_islem_tutari;
            para_birim_id   = P_VIRMAN_STRUCT->alici_hesap_doviz_id;
        }
        else {
            islem_tutari    = P_VIRMAN_STRUCT->gonderen_islem_tutari ;
            para_birim_id   = P_VIRMAN_STRUCT->gonderen_hesap_doviz_id;
        }
    }
    else {
        if ( P_VIRMAN_STRUCT->gonderen_hesap_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
            islem_tutari    = P_VIRMAN_STRUCT->gonderen_islem_tutari;
            para_birim_id   = P_VIRMAN_STRUCT->gonderen_hesap_doviz_id;
        }
        else {
            islem_tutari    = P_VIRMAN_STRUCT->gonderen_islem_tutari;
            para_birim_id   = P_VIRMAN_STRUCT->gonderen_hesap_doviz_id;
        }
    }


    P_VIRMAN_STRUCT->fis_no  = 0;
    if ( P_VIRMAN_STRUCT->fis_no EQ 0 ) {
        P_VIRMAN_STRUCT->fis_no  = BNK_SIRADAKI_VIRMAN_FIS_NO_AL( P_VIRMAN_STRUCT->fis_tarihi ,USE_DB);
    }

    MUH_FIS_STRUCT * MUH_FIS    = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

    MUH_FIS->aciklama           = QString ( QObject::tr ( " Fiş BANKA MODÜLÜ ( VİRMAN FİŞİ ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_VIRMAN_STRUCT->fis_no ).toInt() );
    MUH_FIS->aciklama.append( P_VIRMAN_STRUCT->aciklama );
    MUH_FIS->fis_tarihi         = P_VIRMAN_STRUCT->fis_tarihi;
    MUH_FIS->fis_turu           = ENUM_MAHSUP_FISI;
    MUH_FIS->modul_id           = BANKA_MODULU;
    MUH_FIS->program_id         = E9_PROGRAMI;
    MUH_FIS->base_fis_id        = 0;

    int muh_fis_id = MUH_FIS_EKLE ( MUH_FIS ,USE_DB);

    SQL_QUERY query( USE_DB );
    int virman_fis_id   = P_VIRMAN_STRUCT->fis_id;
    if ( virman_fis_id EQ 0 ) {
        query.PREPARE_INSERT("bnk_virman_fisler","fis_id",
                             "fis_no, fis_tarihi, aciklama,                                 "
                             "hesap_no_id, program_id,                                      "
                             "modul_id, base_fis_id,                                        "
                             "toplam_hesaba_giren, toplam_hesaptan_cikan, muh_fis_id,       "
                             "odm_emr_hesabi_etkilenecek_mi, def_fis_id                     ");

        query.SET_VALUE( ":fis_no"                          , P_VIRMAN_STRUCT->fis_no               );
        query.SET_VALUE( ":fis_tarihi"                      , P_VIRMAN_STRUCT->fis_tarihi           );
        query.SET_VALUE( ":aciklama"                        , P_VIRMAN_STRUCT->aciklama             );
        query.SET_VALUE( ":hesap_no_id"                     , 0                                     );
        query.SET_VALUE( ":program_id"                      , E9_PROGRAMI                           );
        query.SET_VALUE( ":modul_id"                        , BANKA_MODULU                          );
        query.SET_VALUE( ":base_fis_id"                     , 0                                     );
        query.SET_VALUE( ":toplam_hesaba_giren"             , P_VIRMAN_STRUCT->gonderen_islem_tutari);
        query.SET_VALUE( ":toplam_hesaptan_cikan"           , P_VIRMAN_STRUCT->alici_islem_tutari   );
        query.SET_VALUE( ":muh_fis_id"                      , muh_fis_id                            );
        query.SET_VALUE( ":odm_emr_hesabi_etkilenecek_mi"   , 0                                     );
        query.SET_VALUE( ":def_fis_id"                      , 0                                     );

        virman_fis_id = query.INSERT();
    }


    // GONDEREN BANKA ICIN OLUSAN DEFTER FISI
    BNK_DEFTER_FIS_STRUCT * DEF_FIS = new BNK_DEFTER_FIS_STRUCT;
    CLEAR_BNK_DEFTER_FIS_STRUCT ( DEF_FIS );


    DEF_FIS->aciklama           = QString ( QObject::tr ( " Fiş BANKA MODÜLÜ ( VİRMAN FİŞİ ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_VIRMAN_STRUCT->fis_no ).toInt() );
    DEF_FIS->aciklama.append( P_VIRMAN_STRUCT->aciklama );
    DEF_FIS->fis_tarihi                         = P_VIRMAN_STRUCT->fis_tarihi;
    DEF_FIS->fis_turu                           = ENUM_BANKA_HAREKET_FISI;
    DEF_FIS->modul_id                           = BANKA_MODULU;
    DEF_FIS->program_id                         = E9_PROGRAMI;
    DEF_FIS->base_fis_id                        = virman_fis_id;
    DEF_FIS->hesap_no_id                        = P_VIRMAN_STRUCT->gonderen_hesap_no_id;
    DEF_FIS->para_birim_id                      = DVZ_GET_TEMEL_PARA_BIRIMI_ID(); // deftere temel para birimi olarak gidiyor.
    DEF_FIS->muh_fis_id                         = muh_fis_id;
    DEF_FIS->base_fis_turu                      = ENUM_BANKA_VIRMAN_FISI;
    DEF_FIS->odm_emr_hesabi_etkilenecek_mi      = 0;
    DEF_FIS->para_birim_id                      = para_birim_id;

    int def_fis_id = BNK_DEFTER_FIS_EKLE ( DEF_FIS ,USE_DB);

    // virman guncelleniyor.
    query.PREPARE_UPDATE (  "bnk_virman_fisler","fis_id","def_fis_id",
                            "fis_id   = :fis_id           " );

    query.SET_VALUE      ( ":def_fis_id" , def_fis_id );
    query.SET_VALUE      ( ":fis_id"     , virman_fis_id  );
    query.UPDATE         ();

    // ALICI BANKA ICIN OLUSAN DEFTER FISI
    BNK_DEFTER_FIS_STRUCT * DEF_FIS_2 = new BNK_DEFTER_FIS_STRUCT;
    CLEAR_BNK_DEFTER_FIS_STRUCT ( DEF_FIS_2 );


    DEF_FIS_2->aciklama           = QString ( QObject::tr ( " Fiş BANKA MODÜLÜ ( VİRMAN FİŞİ ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_VIRMAN_STRUCT->fis_no ).toInt() );
    DEF_FIS_2->aciklama.append( P_VIRMAN_STRUCT->aciklama );
    DEF_FIS_2->fis_tarihi                         = P_VIRMAN_STRUCT->fis_tarihi;
    DEF_FIS_2->fis_turu                           = ENUM_BANKA_HAREKET_FISI;
    DEF_FIS_2->modul_id                           = BANKA_MODULU;
    DEF_FIS_2->program_id                         = E9_PROGRAMI;
    DEF_FIS_2->base_fis_id                        = virman_fis_id;
    DEF_FIS_2->hesap_no_id                        = P_VIRMAN_STRUCT->alici_hesap_no_id;
    DEF_FIS_2->para_birim_id                      = DVZ_GET_TEMEL_PARA_BIRIMI_ID(); // deftere temel para birimi olarak gidiyor.
    DEF_FIS_2->muh_fis_id                         = muh_fis_id;
    DEF_FIS_2->base_fis_turu                      = ENUM_BANKA_VIRMAN_FISI;
    DEF_FIS_2->odm_emr_hesabi_etkilenecek_mi    = 0;
    DEF_FIS_2->para_birim_id                      = para_birim_id;

    int alici_def_fis_id = BNK_DEFTER_FIS_EKLE ( DEF_FIS_2 ,USE_DB);

    // virman guncelleniyor.
    query.PREPARE_UPDATE (  "bnk_virman_fisler",
                            "fis_id","alici_def_fis_id",
                            "fis_id   = :fis_id" );

    query.SET_VALUE      ( ":alici_def_fis_id" , alici_def_fis_id );
    query.SET_VALUE      ( ":fis_id"     , virman_fis_id  );
    query.UPDATE         ();

    // muhasebe base_fis_id guncelleniyor

    MUH_CLEAR_FIS_STRUCT ( MUH_FIS );
    MUH_FIS_BILGILERINI_OKU( MUH_FIS,muh_fis_id ,USE_DB);
    MUH_FIS->base_fis_id = virman_fis_id;
    MUH_FISI_GUNCELLE( MUH_FIS, muh_fis_id  ,USE_DB);

    // virman satir ekliyecegiz.

    int satir_id    = p_fis_satiri_id;
    if ( p_base_fis_satiri_eklenecek EQ true ) {
        query.PREPARE_INSERT (  "bnk_virman_fis_satirlari","fis_satiri_id" ,"fis_id, aciklama, gonderen_islem_tutari, alici_islem_tutari,   "
                                "base_record_id, order_number, hesap_turu,                       "
                                "program_id, modul_id, gonderen_hesap_no_id, alici_hesap_no_id,     "
                                "doviz_kuru, islem_tutari ");

        query.SET_VALUE ( ":fis_id"                         , virman_fis_id                                 );
        query.SET_VALUE ( ":aciklama"                       , P_VIRMAN_STRUCT->aciklama                     );
        query.SET_VALUE ( ":gonderen_islem_tutari"          , P_VIRMAN_STRUCT->gonderen_islem_tutari        );
        query.SET_VALUE ( ":alici_islem_tutari"             , P_VIRMAN_STRUCT->alici_islem_tutari           );
        query.SET_VALUE ( ":base_record_id"                 , 0                                             );
        query.SET_VALUE ( ":order_number"                   , 1204                                          );
        query.SET_VALUE ( ":hesap_turu"                     , ENUM_BNK_BANKA_HESABI                         );
        query.SET_VALUE ( ":program_id"                     , E9_PROGRAMI                                   );
        query.SET_VALUE ( ":modul_id"                       , BANKA_MODULU                                  );
        query.SET_VALUE ( ":gonderen_hesap_no_id"          , P_VIRMAN_STRUCT->gonderen_hesap_no_id        );
        query.SET_VALUE ( ":alici_hesap_no_id"             , P_VIRMAN_STRUCT->alici_hesap_no_id           );
        query.SET_VALUE ( ":doviz_kuru"                     , P_VIRMAN_STRUCT->doviz_kuru                   );
        query.SET_VALUE ( ":islem_tutari"                   , P_VIRMAN_STRUCT->islem_tutari                 );

        satir_id = query.INSERT();
    }

    BNK_DEFTER_FIS_SATIRI_STRUCT       * DEF_SATIRI = new BNK_DEFTER_FIS_SATIRI_STRUCT;
    CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT ( DEF_SATIRI );

    DEF_SATIRI->aciklama          = QString( "Virman Islemi Banka Hesap id : %1" ).arg( P_VIRMAN_STRUCT->gonderen_hesap_no_id );
    DEF_SATIRI->fis_id            = def_fis_id;
    DEF_SATIRI->program_id        = E9_PROGRAMI;
    DEF_SATIRI->modul_id          = BANKA_MODULU;
    DEF_SATIRI->base_record_id    = satir_id;
    DEF_SATIRI->hesap_turu        = ENUM_BNK_BANKA_HESABI;
    DEF_SATIRI->hesap_id          = P_VIRMAN_STRUCT->gonderen_hesap_no_id;
    DEF_SATIRI->hesaba_giren      = 0.00;
    DEF_SATIRI->hesaptan_cikan    = islem_tutari;
    DEF_SATIRI->doviz_kuru        = P_VIRMAN_STRUCT->doviz_kuru;

    BNK_DEFTER_FIS_SATIRI_EKLE ( DEF_FIS, DEF_SATIRI ,0,true,USE_DB);

    DEF_SATIRI->fis_id            = alici_def_fis_id;
    DEF_SATIRI->hesaba_giren      = islem_tutari;
    DEF_SATIRI->hesaptan_cikan    = 0.00;
    DEF_SATIRI->aciklama          = QString( "Virman Islemi Banka Hesap id : %1" ).arg( P_VIRMAN_STRUCT->alici_hesap_no_id );
    DEF_SATIRI->hesap_id          = P_VIRMAN_STRUCT->alici_hesap_no_id;

    BNK_DEFTER_FIS_SATIRI_EKLE ( DEF_FIS_2, DEF_SATIRI ,0,true,USE_DB);

    delete DEF_FIS;
    delete DEF_FIS_2;
    delete DEF_SATIRI;
    delete MUH_FIS;

}

/**************************************************************************************
                   CLEAR_BNK_VIRMAN_FIS_SATIRI_STRUCT
***************************************************************************************/

void    CLEAR_BNK_VIRMAN_FIS_SATIRI_STRUCT ( BNK_VIRMAN_FIS_AND_SATIRI_STRUCT * P_VIRMAN_STRUCT  )
{
    P_VIRMAN_STRUCT->fis_id                     = 0;
    P_VIRMAN_STRUCT->fis_no                     = 0;
    P_VIRMAN_STRUCT->aciklama.clear();
    P_VIRMAN_STRUCT->fis_tarihi.clear();
    P_VIRMAN_STRUCT->gonderen_hesap_no_id      = 0;
    P_VIRMAN_STRUCT->alici_hesap_no_id         = 0;

    P_VIRMAN_STRUCT->gonderen_islem_tutari      = 0.00;
    P_VIRMAN_STRUCT->alici_islem_tutari         = 0.00;
    P_VIRMAN_STRUCT->doviz_kuru                 = 0.00;

    P_VIRMAN_STRUCT->gonderen_hesap_doviz_id    = 0;
    P_VIRMAN_STRUCT->alici_hesap_doviz_id       = 0;
    P_VIRMAN_STRUCT->islem_tutari               = 0.00;
}

