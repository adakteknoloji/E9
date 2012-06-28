#include "e9_ent_detaylari_batch_class.h"
#include "ui_e9_ent_detaylari_batch.h"
#include "e9_console_utils.h"
#include "muh_console_utils.h"
#include "e9_struct.h"
#include "muh_hesap_arama_open.h"
#include "e9_gui_utils.h"


extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_E9_ENT_DETAYLARI_BATCH
***************************************************************************************/

void OPEN_E9_ENT_DETAYLARI_BATCH ( E9_ENT_DETAYLARI_STRUCT * P_ENT_DETAYLARI, bool kasa_hesabi_gosterilecek  , QWidget * p_parent )
{
    E9_ENT_DETAYLARI_BATCH * F = new E9_ENT_DETAYLARI_BATCH ( P_ENT_DETAYLARI,kasa_hesabi_gosterilecek ,p_parent);
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   E9_ENT_DETAYLARI_BATCH::E9_ENT_DETAYLARI_BATCH
***************************************************************************************/

E9_ENT_DETAYLARI_BATCH::E9_ENT_DETAYLARI_BATCH ( E9_ENT_DETAYLARI_STRUCT * P_ENT_DETAYLARI, bool kasa_hesabi_gosterilecek,QWidget * p_parent ) :  BATCH_KERNEL ( p_parent ), m_ui ( new Ui::E9_ENT_DETAYLARI_BATCH )
{
    M_ENT_DETAYLARI = P_ENT_DETAYLARI;

    m_kasa_hesabi_gosterilecek = kasa_hesabi_gosterilecek;

    m_ui->setupUi      (this );
    START_BATCH_KERNEL (this, DB );
}

/**************************************************************************************
                   E9_ENT_DETAYLARI_BATCH::CHECK_VAR
***************************************************************************************/

void E9_ENT_DETAYLARI_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "ENTEGRASYON DETAYLARI" )        );
    SET_HELP_PAGE     ( "e9_entegrasyon_detaylari_batch.html" );

    REGISTER_BUTTON_WIDGET          ( m_ui->widget_batch_buttons  );

    SET_NAME_OF_RUN_BATCH_BUTTON    ( tr ( "Kaydet" ) );
    SET_FIRST_FOCUS_WIDGET          ( m_ui->searchEdit_kasa_hesap_kodu );
    SET_ENTER_KEY_FOR_RUN_BATCH     ( true );

    SET_AUTO_EXIT_BATCH ( true );

    SET_SETTING_NAME  ( "e9_entegrasyon_detaylari_batch"      );

    m_ui->label_muh_fis_no->setVisible              ( false );
    m_ui->label_banka_fis_no->setVisible            ( false );
    m_ui->label_cari_fis_no->setVisible             ( false );
    m_ui->label_cek_fis_no->setVisible              ( false );
    m_ui->label_senet_fis_no->setVisible            ( false );
    m_ui->label_firma_cek_fis_no->setVisible        ( false );
    m_ui->label_firma_senet_fis_no->setVisible      ( false );

    m_ui->commaEdit_muh_fis_no->setVisible          ( false );
    m_ui->lineEdit_banka_fis_no->setVisible         ( false );
    m_ui->commaEdit_cari_fis_no->setVisible         ( false );
    m_ui->commaEdit_cek_fis_no->setVisible          ( false );
    m_ui->commaEdit_senet_fis_no->setVisible        ( false );
    m_ui->commaEdit_firma_cek_fis_no->setVisible    ( false );
    m_ui->commaEdit_firma_senet_fis_no->setVisible  ( false );

    m_ui->commaEdit_muh_fis_no->VIRGULSUZ_CALIS(true);

    if ( M_ENT_DETAYLARI->muh_fis_no > 0 ) {
        m_ui->label_muh_fis_no->setVisible          ( true );
        m_ui->commaEdit_muh_fis_no->setVisible      ( true );
        m_ui->commaEdit_muh_fis_no->SET_INTEGER     ( M_ENT_DETAYLARI->muh_fis_no );
    }

    if ( M_ENT_DETAYLARI->banka_fis_no.isEmpty() EQ false ) {
        m_ui->label_banka_fis_no->setVisible        ( true );
        m_ui->lineEdit_banka_fis_no->setVisible    ( true );

        m_ui->lineEdit_banka_fis_no->setDisabled(true);

        QString bnk_fis_no_str;

        for ( int i = 0 ; i < M_ENT_DETAYLARI->banka_fis_no.size() ; i++ ) {

            QString banka_fis_no = QVariant (M_ENT_DETAYLARI->banka_fis_no.at(i)).toString();

            if (bnk_fis_no_str.isEmpty() EQ false ) {
                bnk_fis_no_str.append(" , ");
            }
            bnk_fis_no_str.append(banka_fis_no);
        }

        m_ui->lineEdit_banka_fis_no->setText(bnk_fis_no_str);
    }

    if ( M_ENT_DETAYLARI->cari_fis_no > 0 ) {
        m_ui->label_cari_fis_no->setVisible         ( true );
        m_ui->commaEdit_cari_fis_no->setVisible     ( true );
        m_ui->commaEdit_cari_fis_no->SET_INTEGER    ( M_ENT_DETAYLARI->cari_fis_no );
    }

    if ( M_ENT_DETAYLARI->cek_fis_no > 0 ) {
        m_ui->label_cek_fis_no->setVisible          ( true );
        m_ui->commaEdit_cek_fis_no->setVisible      ( true );
        m_ui->commaEdit_cek_fis_no->SET_INTEGER     ( M_ENT_DETAYLARI->cek_fis_no );
    }

    if ( M_ENT_DETAYLARI->senet_fis_no > 0 ) {
        m_ui->label_senet_fis_no->setVisible        ( true );
        m_ui->commaEdit_senet_fis_no->setVisible    ( true );
        m_ui->commaEdit_senet_fis_no->SET_INTEGER   ( M_ENT_DETAYLARI->senet_fis_no );
    }

    if ( M_ENT_DETAYLARI->firma_cek_fis_no > 0 ) {
        m_ui->label_firma_cek_fis_no->setVisible        ( true );
        m_ui->commaEdit_firma_cek_fis_no->setVisible    ( true );
        m_ui->commaEdit_firma_cek_fis_no->SET_INTEGER   ( M_ENT_DETAYLARI->firma_cek_fis_no );
    }

    if ( M_ENT_DETAYLARI->firma_senet_fis_no > 0 ) {
        m_ui->label_firma_senet_fis_no->setVisible        ( true );
        m_ui->commaEdit_firma_senet_fis_no->setVisible    ( true );
        m_ui->commaEdit_firma_senet_fis_no->SET_INTEGER   ( M_ENT_DETAYLARI->firma_senet_fis_no );
    }

    if ( m_kasa_hesabi_gosterilecek EQ true ) {

        DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_kasa_hesap_kodu);
        DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_kasa_hesap_ismi);

        QString hesap_kodu;
        QString hesap_ismi;

        MUH_GET_HESAP_KODU_HESAP_ISMI(M_ENT_DETAYLARI->muh_kasa_hesap_id ,hesap_kodu , hesap_ismi );

        m_ui->searchEdit_kasa_hesap_kodu->SET_TEXT(hesap_kodu);
        m_ui->lineEdit_kasa_hesap_ismi->setText(hesap_ismi);
    }
    else {
        m_ui->frame_kasa_hesabi->setHidden(true);
    }

}

/**************************************************************************************
                   E9_ENT_DETAYLARI_BATCH::CHECK_VAR
***************************************************************************************/

int E9_ENT_DETAYLARI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchEdit_kasa_hesap_kodu ) {

        if ( m_ui->searchEdit_kasa_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_kasa_hesap_ismi->clear();
            M_ENT_DETAYLARI->muh_kasa_hesap_id = 0;
            return ADAK_OK;
        }

        M_ENT_DETAYLARI->muh_kasa_hesap_id = MUH_GET_HESAP_ID(m_ui->searchEdit_kasa_hesap_kodu->GET_TEXT());

        if ( M_ENT_DETAYLARI->muh_kasa_hesap_id <= 0 ) {
            MSG_WARNING(QObject::tr("Aradığınız muhasebe hesabı bulunamadı.") ,
                        m_ui->searchEdit_kasa_hesap_kodu);
            return ADAK_FAIL;
        }
        m_ui->lineEdit_kasa_hesap_ismi->setText(MUH_GET_HESAP_ISMI(m_ui->searchEdit_kasa_hesap_kodu->GET_TEXT()));

    }
    return ADAK_OK;
}

/**************************************************************************************
                   E9_ENT_DETAYLARI_BATCH::CHECK_RUN
***************************************************************************************/

int E9_ENT_DETAYLARI_BATCH::CHECK_RUN ()
{
    if ( m_kasa_hesabi_gosterilecek EQ true ) {
        if ( m_ui->searchEdit_kasa_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING(QObject::tr("Muhasebe hesabını boş bırakamazsınız.") ,
                        m_ui->searchEdit_kasa_hesap_kodu);
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   E9_ENT_DETAYLARI_BATCH::RUN_BATCH
***************************************************************************************/

void E9_ENT_DETAYLARI_BATCH::RUN_BATCH ()
{

}

/**************************************************************************************
                   E9_ENT_DETAYLARI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void E9_ENT_DETAYLARI_BATCH::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    Q_UNUSED ( widget );

    int kasa_hesap_id = OPEN_MUH_HESAP_ARAMA ( line_edit->text(), this, true );

    if ( kasa_hesap_id > 0 ) {
        line_edit->setText ( MUH_GET_HESAP_KODU ( kasa_hesap_id ) );
    }
}





















