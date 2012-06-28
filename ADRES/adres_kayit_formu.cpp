#include <QHeaderView>
#include <QFileDialog>
#include <QBuffer>
#include <QMessageBox>
#include "adak_utils.h"
#include "ui_adres_kayit_formu.h"
#include "adres_struct.h"
#include "adres_gui_utils.h"
#include "adres_console_utils.h"
#include "adres_kayit_formu_class.h"
#include "adres_kayit_formu_open.h"
#include "adak_std_utils.h"
#include "yonetim.h"
#include "e9_log.h"
#include "adak_gruplar.h"
#include "e9_yetki.h"
#include "adres_enum.h"
 
#include "adres_email_fisi_open.h"
#include "adres_arama_open.h"
#include "adres_not_defteri_fisi_open.h"
#include "adres_telefon_fisi_open.h"
#include "adres_yetkililer_fisi_open.h"
#include "adres_zarf_yazdir_batch_open.h"


extern ADAK_SQL *              DB;

/**************************************************************************************
                      OPEN_ADRES_KAYIT_FORMU
***************************************************************************************/

void OPEN_ADRES_KAYIT_FORMU ( int id, int kart_turu , QWidget * p_parent )
{
    ADRES_KAYIT_FORMU * F = new ADRES_KAYIT_FORMU ( id, kart_turu,p_parent );
    F->SHOW( FULL_SCREEN );    
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::ADRES_KAYIT_FORMU
***************************************************************************************/

ADRES_KAYIT_FORMU::ADRES_KAYIT_FORMU ( int record_id,int kart_turu, QWidget * parent ) : FORM_KERNEL ( parent ), m_ui ( new Ui::ADRES_KAYIT_FORMU )
{
    m_start_record_id = record_id;

    m_kart_turu = kart_turu;

    m_ui->setupUi       ( this );
    START_FORM_KERNEL   ( this, DB );
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::SETUP_FORM
***************************************************************************************/
void ADRES_KAYIT_FORMU::SETUP_FORM()
{

    SET_SETTING_NAME    ( "ADRES_KAYIT_FORMU" );

    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_button_widget );

    m_ui->adak_date_dogum_tarihi->MALI_YIL_KISITI_YOK( true );

    M_ADR_STRUCT = new ADRES_HESAP_STRUCT;

    ADR_CLEAR_ADRES_STRUCT( M_ADR_STRUCT );

    KULLANICI_YETKILERINI_AYARLA ();

    if ( m_kart_turu EQ SAHIS_KAYDI ) {
        SET_PAGE_TITLE      ( tr ( "ADR - ŞAHIS KARTI" ) );
        SET_HELP_PAGE       ( tr("adres-toplu-mail-sms_sahis-karti") );
    }
    else if ( m_kart_turu EQ FIRMA_KAYDI ){
        SET_HELP_PAGE       ( tr("adres-toplu-mail-sms_firma-karti") );
        SET_PAGE_TITLE      ( tr ( "ADR - FİRMA KARTI" ) );
    }
    else {
        SET_PAGE_TITLE      ( tr ( "ADR - FİRMA/ŞAHIS KARTI" ) );
        SET_HELP_PAGE       ( tr("adres-toplu-mail-sms_sahis-karti") );
    }

    EKRANI_DUZENLE();

    SET_WIDGETS_MAXIMUM_SIZE();

    m_modul_id      = ADRES_MODULU;
    m_program_id    = E9_PROGRAMI;

    REGISTER_SAVER_BUTTON   ( m_ui->button_not_defteri           );
    REGISTER_SAVER_BUTTON   ( m_ui->button_telefonlar            );
    REGISTER_SAVER_BUTTON   ( m_ui->button_emailler              );
    REGISTER_SAVER_BUTTON   ( m_ui->push_button_zarf_yazdir      );

    REGISTER_CHANGER_BUTTON ( m_ui->push_button_gruplar          );
    REGISTER_CHANGER_BUTTON ( m_ui->push_button_fotograf_sec     );

    SET_FIRST_FOCUS_WIDGET  ( m_ui->lineEdit_sahis_adi_soyadi );

    GET_CINSIYETLER         ( m_ui->combo_box_cinsiyet      );
    GET_OGRENIM_DURUMLARI   ( m_ui->combo_box_egitim_durumu );

    if ( m_kart_turu EQ FIRMA_KAYDI ) {
        GET_SEKTORLER ( m_ui->combo_box_unvan );
    }
    else {
        GET_UNVANLAR  ( m_ui->combo_box_unvan       );
    }
    GET_MEDENI_HALLER       ( m_ui->combo_box_medeni_hali );
    GET_KAN_GRUPLARI        ( m_ui->combo_box_kan_grubu   );
    GET_MESLEKLER           ( m_ui->combo_box_meslek      );

    GET_ILLER               ( m_ui->combo_box_sehir  );
    GET_ULKELER             ( m_ui->combo_box_ulke   );

    GET_ULKE_TELEFON_KODLARI( m_ui->combo_box_cep_tel_ulke_kodu );
    GET_ULKE_TELEFON_KODLARI( m_ui->combo_box_ev_tel_ulke_kodu  );
    GET_ULKE_TELEFON_KODLARI( m_ui->combo_box_fax_ulke_kodu     );
    GET_ULKE_TELEFON_KODLARI( m_ui->combo_box_is_tel_ulke_kodu  );

    GET_GSM_KODLARI         ( m_ui->combo_box_cep_tel_gsm_kodu );

    FOCUS_FIRST_WIDGET();

    AUTOSTART_FORM_KERNEL( m_start_record_id );
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/
void ADRES_KAYIT_FORMU::CLEAR_FORM_MEMBERS()
{
    m_fotograf_path.clear();
    m_grup_idleri.clear();
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::NEW_RECORD
***************************************************************************************/

void ADRES_KAYIT_FORMU::NEW_RECORD ()
{

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_ulke, tr ( "Türkiye" ) );

    QString ulke_kodu = GET_ULKE_TELEFON_KODU( m_ui->combo_box_ulke->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cep_tel_ulke_kodu, ulke_kodu );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_ev_tel_ulke_kodu , ulke_kodu );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_tel_ulke_kodu , ulke_kodu );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_ulke_kodu    , ulke_kodu );

    m_ui->combo_box_ev_tel_alan_kodu->clear();

    GET_ALAN_KODLARI ( m_ui->combo_box_ev_tel_alan_kodu, "" , m_ui->combo_box_ulke->currentText() );

    m_ui->combo_box_cep_tel_gsm_kodu->clear();

    GET_GSM_KODLARI ( m_ui->combo_box_cep_tel_gsm_kodu, "", m_ui->combo_box_ulke->currentText() );

    m_ui->combo_box_is_tel_alan_kodu->clear();

    GET_ALAN_KODLARI ( m_ui->combo_box_is_tel_alan_kodu, "", m_ui->combo_box_ulke->currentText() );

    m_ui->combo_box_fax_alan_kodu->clear();

    GET_ALAN_KODLARI ( m_ui->combo_box_fax_alan_kodu, "", m_ui->combo_box_ulke->currentText() );

    m_ui->combo_box_ulke->setCurrentIndex ( m_ui->combo_box_ulke->findText ( tr ( "Türkiye" ) ) );
    m_ui->adak_date_dogum_tarihi->SET_DATE( QDate ( 1980, 1, 1 ) );

    m_ui->push_button_fotograf_sec->setText ( tr ( "Fotoğraf Seç" ) );
    m_ui->push_button_fotograf_sec->setIcon ( QIcon() );

    m_modul_id      = ADRES_MODULU;
    m_program_id    = E9_PROGRAMI;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::GET_RECORD
***************************************************************************************/

int ADRES_KAYIT_FORMU::GET_RECORD ( int record_id )
{

    ADR_CLEAR_ADRES_STRUCT( M_ADR_STRUCT );

    ADR_KART_BILGILERINI_OKU( M_ADR_STRUCT, record_id );


    int adres_kart_turu   = M_ADR_STRUCT->adres_kart_turu;
    if ( adres_kart_turu EQ 1  ) {
        m_kart_turu = SAHIS_KAYDI;
    }
    else {
        m_kart_turu = FIRMA_KAYDI;
    }

    m_modul_id    = M_ADR_STRUCT->modul_id;

    m_ui->lineEdit_sahis_adi_soyadi->setText ( M_ADR_STRUCT->firma_sahis_adi );
    m_ui->adak_date_dogum_tarihi->SET_DATE( M_ADR_STRUCT->dogum_tarihi );

    m_ui->line_edit_dogum_yeri->setText( M_ADR_STRUCT->dogum_yeri);

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_meslek, M_ADR_STRUCT->meslek );

    if( m_kart_turu EQ SAHIS_KAYDI ) {
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_unvan, M_ADR_STRUCT->unvan );
    }
    else {
        FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_unvan, M_ADR_STRUCT->firma_sektoru );
    }

    m_ui->lineEdit_yetkili_1->setText( M_ADR_STRUCT->yetkili_1 );
    m_ui->lineEdit_yetkili_1_info->setText( M_ADR_STRUCT->yetkili_1_info );
    m_ui->lineEdit_yetkili_2->setText( M_ADR_STRUCT->yetkili_2 );
    m_ui->lineEdit_yetkili_2_info->setText( M_ADR_STRUCT->yetkili_2_info );
    m_ui->lineEdit_yetkili_3->setText( M_ADR_STRUCT->yetkili_3 );
    m_ui->lineEdit_yetkili_3_info->setText( M_ADR_STRUCT->yetkili_3_info );

    m_ui->lineEdit_kimlik_no->setText( M_ADR_STRUCT->kimlik_no );
    m_ui->lineEdit_vergi_no->setText( M_ADR_STRUCT->vergi_dairesi );
    m_ui->lineEdit_adres_satiri_1->setText( M_ADR_STRUCT->adres_satiri_1 );
    m_ui->lineEdit_adres_satiri_2->setText( M_ADR_STRUCT->adres_satiri_2 );
    m_ui->lineEdit_adres_satiri_3->setText( M_ADR_STRUCT->adres_satiri_3 );
    m_ui->line_edit_semt->setText( M_ADR_STRUCT->semt );
    m_ui->line_edit_posta_kodu->setText( M_ADR_STRUCT->posta_kodu );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_kan_grubu, M_ADR_STRUCT->kan_grubu     );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_medeni_hali, M_ADR_STRUCT->medeni_hali    );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_egitim_durumu, M_ADR_STRUCT->ogrenim_durumu );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cinsiyet, M_ADR_STRUCT->cinsiyet     );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_ulke, M_ADR_STRUCT->ulke );

    // Şehir
    m_ui->combo_box_sehir->clear();

    GET_ILLER( m_ui->combo_box_sehir, m_ui->combo_box_ulke->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_sehir, M_ADR_STRUCT->sehir );

    // İlçe
    m_ui->comboBox_ilce->clear();

    GET_ILCELER( m_ui->comboBox_ilce, m_ui->combo_box_sehir->currentText() );
    FIND_AND_SET_COMBOBOX_TEXT   ( m_ui->comboBox_ilce, M_ADR_STRUCT->ilce );

    // Vergi Dairesi

    GET_VERGI_DAIRELERI( m_ui->combo_box_vergi_dairesi, m_ui->combo_box_sehir->currentText() );
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_vergi_dairesi, M_ADR_STRUCT->vergi_dairesi );

    // Ev telefon kodları

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_ev_tel_ulke_kodu, M_ADR_STRUCT->tel_ulke_kodu );

    GET_ALAN_KODLARI( m_ui->combo_box_ev_tel_alan_kodu, "", m_ui->combo_box_ulke->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_ev_tel_alan_kodu, M_ADR_STRUCT->tel_alan_kodu );

    m_ui->line_edit_ev_telefonu->setText( M_ADR_STRUCT->telefon );

    // Cep Telefon kodları

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_cep_tel_ulke_kodu, M_ADR_STRUCT->cep_tel_ulke_kodu );

    GET_GSM_KODLARI ( m_ui->combo_box_cep_tel_gsm_kodu , m_ui->combo_box_cep_tel_ulke_kodu->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cep_tel_gsm_kodu, M_ADR_STRUCT->cep_tel_gsm_kodu );

    m_ui->line_edit_cep_telefonu->setText( M_ADR_STRUCT->cep_telefonu );

    // İş Telefon kodları

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_is_tel_ulke_kodu, M_ADR_STRUCT->is_tel_ulke_kodu );

    GET_ALAN_KODLARI ( m_ui->combo_box_is_tel_alan_kodu, m_ui->combo_box_ev_tel_ulke_kodu->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_tel_alan_kodu, M_ADR_STRUCT->is_tel_alan_kodu );

    m_ui->line_edit_is_telefonu->setText( M_ADR_STRUCT->is_telefonu );
    m_ui->lineEdit_dahili->setText( M_ADR_STRUCT->is_telefonu_dahili );

    m_ui->lineEdit_website->setText( M_ADR_STRUCT->website );

    // Fax Telefon kodları

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_fax_ulke_kodu, M_ADR_STRUCT->fax_ulke_kodu );

    GET_ALAN_KODLARI( m_ui->combo_box_fax_alan_kodu, m_ui->combo_box_ev_tel_ulke_kodu->currentText() , m_ui->combo_box_ulke->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_alan_kodu, M_ADR_STRUCT->fax_alan_kodu );

    m_ui->line_edit_fax_telefonu->setText( M_ADR_STRUCT->fax );

    m_ui->line_edit_email->setText( M_ADR_STRUCT->email );

    m_fotograf_path = "";
    m_ui->push_button_fotograf_sec->setIcon( QIcon( m_fotograf_path ) );
    m_ui->push_button_fotograf_sec->setText( tr( "Fotoğraf Seç" ) );

    if ( M_ADR_STRUCT->fotograf_logo.isEmpty() EQ false) {
        int w = m_ui->push_button_fotograf_sec->width();
        int h = m_ui->push_button_fotograf_sec->height();

        m_ui->push_button_fotograf_sec->setText ( "" );
        m_ui->push_button_fotograf_sec->setIcon ( QIcon(ADAK_BYTE_ARRAY_2_PIXMAP( M_ADR_STRUCT->fotograf_logo )) );
        m_ui->push_button_fotograf_sec->setIconSize   ( QSize(w-4,h-4) );
        m_ui->push_button_fotograf_sec->setMinimumSize( QSize(w,h) );
        m_ui->push_button_fotograf_sec->setMaximumSize( QSize(w,h) );
    }
    m_ui->textEdit_gruplar->setText ( GRP_KAYDIN_GRUPLARINI_BUL(E9_PROGRAMI, ADRES_MODULU,record_id , &m_grup_idleri, new QStringList() , new QStringList() ) );

    ADRES_BILGILERINI_OLUSTUR();

    return ADAK_OK;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::CHECK_FORM_VAR
***************************************************************************************/

int ADRES_KAYIT_FORMU::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->combo_box_cep_tel_ulke_kodu ) {

        if ( m_ui->combo_box_cep_tel_ulke_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr ( "Ülke kodu 10 karakterden daha uzun olamaz." ), m_ui->combo_box_cep_tel_ulke_kodu );
            return ADAK_FAIL;
        }

        GET_GSM_KODLARI ( m_ui->combo_box_cep_tel_gsm_kodu,  m_ui->combo_box_cep_tel_ulke_kodu->currentText() );
    }
    else if ( object EQ m_ui->combo_box_fax_ulke_kodu ) {

        if ( m_ui->combo_box_fax_ulke_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr ( "Ülke kodu 10 karakterden daha uzun olamaz." ), m_ui->combo_box_fax_ulke_kodu );
            return ADAK_FAIL;
        }

        GET_ALAN_KODLARI ( m_ui->combo_box_fax_alan_kodu,  m_ui->combo_box_fax_ulke_kodu->currentText() );
    }
    else if ( object EQ m_ui->combo_box_ev_tel_ulke_kodu ) {

        if ( m_ui->combo_box_ev_tel_ulke_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr ( "Ülke kodu 10 karakterden daha uzun olamaz." ), m_ui->combo_box_ev_tel_ulke_kodu );
            return ADAK_FAIL;
        }
        GET_ALAN_KODLARI ( m_ui->combo_box_ev_tel_alan_kodu, m_ui->combo_box_ev_tel_ulke_kodu->currentText() );

    }
    else if ( object EQ m_ui->combo_box_is_tel_ulke_kodu ) {


        if ( m_ui->combo_box_is_tel_ulke_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr ( "Ülke kodu 10 karakterden daha uzun olamaz."), m_ui->combo_box_is_tel_ulke_kodu );
            return ADAK_FAIL;
        }

        GET_ALAN_KODLARI ( m_ui->combo_box_is_tel_alan_kodu, m_ui->combo_box_is_tel_ulke_kodu->currentText() );

    }
    else if ( object EQ m_ui->combo_box_ulke ) {

        if ( m_ui->combo_box_ulke->currentText().size() > 30 ) {
            MSG_WARNING( tr ( "Ülke 30 karakterden daha uzun olamaz." ), m_ui->combo_box_ulke );

            return ADAK_FAIL;
        }

        m_ui->combo_box_sehir->clear();

        GET_ILLER         ( m_ui->combo_box_sehir, m_ui->combo_box_ulke->currentText() );

        QString ulke_tel_kodu = GET_ULKE_TELEFON_KODU ( m_ui->combo_box_ulke->currentText() );

        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cep_tel_ulke_kodu, ulke_tel_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_ulke_kodu    , ulke_tel_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_ev_tel_ulke_kodu , ulke_tel_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_tel_ulke_kodu , ulke_tel_kodu );

    }
    else if ( object EQ m_ui->combo_box_sehir ) {

         if ( m_ui->combo_box_sehir->currentText().size() > 30 ) {
            MSG_WARNING( tr ( "Şehir 30 karakterden daha uzun olamaz."), m_ui->combo_box_sehir );

            return ADAK_FAIL;
        }

        QString il_telefon_kodu = GET_IL_ALAN_KODU ( m_ui->combo_box_sehir->currentText(),"" );

        m_ui->comboBox_ilce->clear();
        m_ui->combo_box_vergi_dairesi->clear();

        GET_ILCELER         ( m_ui->comboBox_ilce, m_ui->combo_box_sehir->currentText() );
        GET_VERGI_DAIRELERI ( m_ui->combo_box_vergi_dairesi, m_ui->combo_box_sehir->currentText() );

        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_ev_tel_alan_kodu, il_telefon_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_alan_kodu,    il_telefon_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_tel_alan_kodu, il_telefon_kodu );
    }
    else if ( object EQ m_ui->lineEdit_sahis_adi_soyadi ) {


        if ( m_modul_id NE ADRES_MODULU OR m_program_id NE E9_PROGRAMI  ) {
            MSG_WARNING( tr ( "Kayıt başka bir modül tarafından oluşturulmuş.\nŞahıs adında herhangi bir değişiklik yapamazsınız"), m_ui->lineEdit_sahis_adi_soyadi );
            return ADAK_FAIL_UNDO;
        }

     }
    else if ( object EQ m_ui->combo_box_cinsiyet ) {
         if ( m_ui->combo_box_cinsiyet->currentText().size() > 5 ) {
            MSG_WARNING( tr ( "Cinsiyet bilgisi 5 karakterden daha uzun olamaz."), m_ui->combo_box_cinsiyet );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->combo_box_egitim_durumu ) {
         if ( m_ui->combo_box_egitim_durumu->currentText().size() > 20 ) {
            MSG_WARNING( tr ( "Eğitim durmu 20 karakterden daha uzun olamaz." ), m_ui->combo_box_egitim_durumu );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->combo_box_kan_grubu ) {
        if ( m_ui->combo_box_kan_grubu->currentText().size() > 10 ) {
            MSG_WARNING( tr ( "Kan grubu 10 karakterden daha uzun olamaz."), m_ui->combo_box_kan_grubu );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->combo_box_medeni_hali ) {
        if ( m_ui->combo_box_medeni_hali->currentText().size() > 10 ) {
            MSG_WARNING( tr ( "Medeni hal bilgisi 10 karakterden daha uzun olamaz." ), m_ui->combo_box_medeni_hali );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->combo_box_vergi_dairesi ) {
         if ( m_ui->combo_box_vergi_dairesi->currentText().size() > 50 ) {
            MSG_WARNING( tr ("Vergi dairesi 50 karakterden daha uzun olamaz." ), m_ui->combo_box_vergi_dairesi );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->comboBox_ilce ) {
         if ( m_ui->comboBox_ilce->currentText().size() > 30 ) {
            MSG_WARNING( tr ( "İlçe 30 karakterden daha uzun olamaz."), m_ui->comboBox_ilce );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->combo_box_ev_tel_alan_kodu) {
         if ( m_ui->combo_box_ev_tel_alan_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr ( "Alan kodu 10 karakterden daha uzun olamaz."), m_ui->combo_box_ev_tel_alan_kodu );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->combo_box_is_tel_alan_kodu ) {
         if ( m_ui->combo_box_is_tel_alan_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr ( "Alan kodu 10 karakterden daha uzun olamaz."), m_ui->combo_box_is_tel_alan_kodu );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->combo_box_cep_tel_gsm_kodu ) {
         if ( m_ui->combo_box_cep_tel_gsm_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr("Gsm kodu 10 karakterden daha uzun olamaz."), m_ui->combo_box_cep_tel_gsm_kodu );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->combo_box_fax_alan_kodu ) {
         if ( m_ui->combo_box_fax_alan_kodu->currentText().size() > 10 ) {
            MSG_WARNING( tr ( "Alan kodu 10 karakterden daha uzun olamaz."), m_ui->combo_box_fax_alan_kodu );
            return ADAK_FAIL;
        }
    }

    ADRES_BILGILERINI_OLUSTUR();

    return ADAK_OK;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::CHECK_FORM_EMPTY
***************************************************************************************/

int ADRES_KAYIT_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->lineEdit_sahis_adi_soyadi->text().isEmpty() EQ true ) {

        MSG_WARNING( tr ( "Adres Kart İsmi yazmalısınız.!" ), m_ui->lineEdit_sahis_adi_soyadi );
        return ADAK_FAIL;
    }

    if ( m_ui->line_edit_ev_telefonu->text().replace (  "-", ""  ).isEmpty() EQ false ) {

        if ( m_ui->line_edit_ev_telefonu->text().replace (  "-", "" ).size() < 7 ) {
            MSG_WARNING( tr ( "Ev telefonu bilgisi eksik girilmiş. Telefon 7 haneli olmalıdır."), m_ui->line_edit_ev_telefonu );
            return ADAK_FAIL;
        }
        if ( m_ui->combo_box_ev_tel_ulke_kodu->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Ev telefonu bilgisi eksik girilmiş. Ülke kodu boş bırakılamaz."), m_ui->combo_box_ev_tel_ulke_kodu );
            return ADAK_FAIL;
        }
        if ( m_ui->combo_box_ev_tel_alan_kodu->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Ev telefonu bilgisi eksik girilmiş. Alan kodu boş bırakılamaz."),m_ui->combo_box_ev_tel_alan_kodu );
            return ADAK_FAIL;
        }
    }

    if ( m_ui->line_edit_is_telefonu->text().replace (  "-", ""  ).isEmpty() EQ false ) {

        if ( m_ui->line_edit_is_telefonu->text().replace (  "-", "" ).size() < 7 ) {
            MSG_WARNING( tr ( "İş telefonu bilgisi eksik girilmiş. İş telefonu 7 haneli olmalıdır."), m_ui->line_edit_is_telefonu );
            return ADAK_FAIL;
        }
        if ( m_ui->combo_box_is_tel_ulke_kodu->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "İş telefonu bilgisi eksik girilmiş. Ülke kodu boş bırakılamaz."),m_ui->combo_box_is_tel_ulke_kodu );
            return ADAK_FAIL;
        }
        if ( m_ui->combo_box_is_tel_ulke_kodu->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "İş telefonu bilgisi eksik girilmiş. Alan kodu boş bırakılamaz."), m_ui->combo_box_is_tel_alan_kodu );
            return ADAK_FAIL;
        }
    }

    if ( m_ui->line_edit_cep_telefonu->text().replace (  "-", ""  ).isEmpty() EQ false ) {

        if ( m_ui->line_edit_cep_telefonu->text().replace (  "-", "" ).size() < 7 ) {
            MSG_WARNING( tr ( "Cep telefonu bilgisi eksik girilmiş. Cep telefonu 7 haneli olmalıdır."), m_ui->line_edit_cep_telefonu );
            return ADAK_FAIL;
        }
        if ( m_ui->combo_box_cep_tel_ulke_kodu->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr("Cep telefonu bilgisi eksik girilmiş. Cep telefonu ülke kodu boş bırakılamaz."), m_ui->combo_box_cep_tel_ulke_kodu );
            return ADAK_FAIL;
        }
        if ( m_ui->combo_box_cep_tel_gsm_kodu->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Cep telefonu bilgisi eksik girilmiş. Gsm kodu boş bırakılamaz."), m_ui->combo_box_cep_tel_gsm_kodu );
            return ADAK_FAIL;
        }
    }

    if ( m_ui->line_edit_fax_telefonu->text().replace ( "-", "" ).isEmpty() EQ false ) {

        if ( m_ui->line_edit_fax_telefonu->text().replace ( "-", "" ).size() < 7 ) {
            MSG_WARNING( tr ( "Fax bilgisi eksik girilmiş. Fax 7 haneli olmalıdır."), m_ui->line_edit_fax_telefonu );
            return ADAK_FAIL;
        }
        if ( m_ui->combo_box_fax_ulke_kodu->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Fax bilgisi eksik girilmiş. Fax ülke kodu boş bırakılamaz."), m_ui->combo_box_fax_ulke_kodu );
            return ADAK_FAIL;
        }
        if ( m_ui->combo_box_fax_alan_kodu->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Fax bilgisi eksik girilmiş. Fax alan kodu boş bırakılamaz."), m_ui->combo_box_fax_alan_kodu );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::CHECK_ADD
***************************************************************************************/

int ADRES_KAYIT_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::ADD_RECORD
***************************************************************************************/

int ADRES_KAYIT_FORMU::ADD_RECORD ()
{
    ADR_CLEAR_ADRES_STRUCT ( M_ADR_STRUCT );

    QString ev_telefonu  = m_ui->line_edit_ev_telefonu->text();
    QString is_telefonu  = m_ui->line_edit_is_telefonu->text();
    QString cep_telefonu = m_ui->line_edit_cep_telefonu->text();
    QString fax          = m_ui->line_edit_fax_telefonu->text();

    ev_telefonu.replace  ( "-", "" );
    is_telefonu.replace  ( "-", "" );
    cep_telefonu.replace ( "-", "" );
    fax.replace          ( "-", "" );

    QString     telefon_ulke_kodu   = m_ui->combo_box_ev_tel_ulke_kodu->currentText();
    QString     telefon_alan_kodu   = m_ui->combo_box_ev_tel_alan_kodu->currentText();

    QString     is_tel_ulke_kodu    = m_ui->combo_box_is_tel_ulke_kodu->currentText();
    QString     is_tel_alan_kodu    = m_ui->combo_box_is_tel_alan_kodu->currentText();

    QString     cep_tel_ulke_kodu   = m_ui->combo_box_cep_tel_ulke_kodu->currentText();
    QString     cep_tel_gsm_kodu    = m_ui->combo_box_cep_tel_gsm_kodu->currentText();

    QString     fax_ulke_kodu       = m_ui->combo_box_fax_ulke_kodu->currentText();
    QString     fax_alan_kodu       = m_ui->combo_box_fax_alan_kodu->currentText();

    QString     is_telefonu_dahili  = m_ui->lineEdit_dahili->text();

    M_ADR_STRUCT->firma_sahis_adi             = m_ui->lineEdit_sahis_adi_soyadi->text();
    M_ADR_STRUCT->dogum_tarihi                = m_ui->adak_date_dogum_tarihi->DATE();
    M_ADR_STRUCT->dogum_yeri                  = m_ui->line_edit_dogum_yeri->text();
    M_ADR_STRUCT->cinsiyet                    = m_ui->combo_box_cinsiyet->currentText();
    M_ADR_STRUCT->ogrenim_durumu              = m_ui->combo_box_egitim_durumu->currentText();
    M_ADR_STRUCT->medeni_hali                 = m_ui->combo_box_medeni_hali->currentText();
    M_ADR_STRUCT->kan_grubu                   = m_ui->combo_box_kan_grubu->currentText();
    M_ADR_STRUCT->meslek                      = m_ui->combo_box_meslek->currentText();
    M_ADR_STRUCT->kimlik_no                   = m_ui->lineEdit_kimlik_no->text();
    M_ADR_STRUCT->vergi_dairesi               = m_ui->combo_box_vergi_dairesi->currentText();
    M_ADR_STRUCT->vergi_no                    = m_ui->lineEdit_vergi_no->text();
    M_ADR_STRUCT->adres_satiri_1              = m_ui->lineEdit_adres_satiri_1->text();
    M_ADR_STRUCT->adres_satiri_2              = m_ui->lineEdit_adres_satiri_2->text();
    M_ADR_STRUCT->adres_satiri_3              = m_ui->lineEdit_adres_satiri_3->text();
    M_ADR_STRUCT->ilce                        = m_ui->comboBox_ilce->currentText();
    M_ADR_STRUCT->semt                        = m_ui->line_edit_semt->text();
    M_ADR_STRUCT->ulke                        = m_ui->combo_box_ulke->currentText();
    M_ADR_STRUCT->sehir                       = m_ui->combo_box_sehir->currentText();
    M_ADR_STRUCT->posta_kodu                  = m_ui->line_edit_posta_kodu->text();
    M_ADR_STRUCT->tel_ulke_kodu               = telefon_ulke_kodu;
    M_ADR_STRUCT->tel_alan_kodu               = telefon_alan_kodu;
    M_ADR_STRUCT->telefon                     = ev_telefonu;
    M_ADR_STRUCT->is_tel_ulke_kodu            = is_tel_ulke_kodu;
    M_ADR_STRUCT->is_tel_alan_kodu            = is_tel_alan_kodu;
    M_ADR_STRUCT->is_telefonu                 = is_telefonu;
    M_ADR_STRUCT->cep_tel_ulke_kodu           = cep_tel_ulke_kodu;
    M_ADR_STRUCT->cep_tel_gsm_kodu            = cep_tel_gsm_kodu;
    M_ADR_STRUCT->cep_telefonu                = cep_telefonu;
    M_ADR_STRUCT->fax_ulke_kodu               = fax_ulke_kodu;
    M_ADR_STRUCT->fax_alan_kodu               = fax_alan_kodu;
    M_ADR_STRUCT->fax                         = fax;
    M_ADR_STRUCT->email                       = m_ui->line_edit_email->text();
    M_ADR_STRUCT->firma_id                    = 0;
    M_ADR_STRUCT->modul_id                    = ADRES_MODULU;
    M_ADR_STRUCT->is_telefonu_dahili          = is_telefonu_dahili;
    M_ADR_STRUCT->adres_kart_turu             = m_kart_turu;
    M_ADR_STRUCT->website                     = m_ui->lineEdit_website->text();
    M_ADR_STRUCT->program_id                  = E9_PROGRAMI;
    M_ADR_STRUCT->yetkili_1                   = m_ui->lineEdit_yetkili_1->text();
    M_ADR_STRUCT->yetkili_1_info              = m_ui->lineEdit_yetkili_1_info->text();
    M_ADR_STRUCT->yetkili_2                   = m_ui->lineEdit_yetkili_2->text();
    M_ADR_STRUCT->yetkili_2_info              = m_ui->lineEdit_yetkili_2_info->text();
    M_ADR_STRUCT->yetkili_3                   = m_ui->lineEdit_yetkili_3->text();
    M_ADR_STRUCT->yetkili_3_info              = m_ui->lineEdit_yetkili_3_info->text();

    if ( m_kart_turu EQ SAHIS_KAYDI ) {
        M_ADR_STRUCT->unvan                   = m_ui->combo_box_unvan->currentText();
    }
    else {
        M_ADR_STRUCT->firma_sektoru           = m_ui->combo_box_unvan->currentText();
    }

    if ( m_fotograf_path.isEmpty() EQ false ) {
        M_ADR_STRUCT->fotograf_logo          = ADAK_PIXMAP_2_BYTE_ARRAY(m_fotograf_path);
    }
    else {
        M_ADR_STRUCT->fotograf_logo          = QByteArray();
    }

    int record_id = ADR_KART_OLUSTUR ( M_ADR_STRUCT );

    GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI,ADRES_MODULU,record_id,&m_grup_idleri);


    m_modul_id = ADRES_MODULU;

    QString log_detaylari = QObject::tr("Log Türü: ŞAHIS KAYDI");
    log_detaylari.append ( " # Şahıs Adı Soyadı: " );
    log_detaylari.append ( m_ui->lineEdit_sahis_adi_soyadi->text() );

    E9_LOG_KAYDI_EKLE(ADRES_MODULU,LOG_ADR_SAHIS_KAYDI,LOG_ISLEM_ADD,log_detaylari);

    return record_id;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::CHECK_UPDATE
***************************************************************************************/

int ADRES_KAYIT_FORMU::CHECK_UPDATE ( int record_id )
{
    Q_UNUSED ( record_id )
    return ADAK_OK;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::UPDATE_RECORD
***************************************************************************************/

void ADRES_KAYIT_FORMU::UPDATE_RECORD ( int p_adres_id )
{
    QString ev_telefonu  = m_ui->line_edit_ev_telefonu->text();
    QString is_telefonu  = m_ui->line_edit_is_telefonu->text();
    QString cep_telefonu = m_ui->line_edit_cep_telefonu->text();
    QString fax          = m_ui->line_edit_fax_telefonu->text();

    ev_telefonu.replace  ( "-", "" );
    is_telefonu.replace  ( "-", "" );
    cep_telefonu.replace ( "-", "" );
    fax.replace          ( "-", "" );

    QString tel_ulke_kodu             = m_ui->combo_box_ev_tel_ulke_kodu->currentText();
    QString tel_alan_kodu             = m_ui->combo_box_ev_tel_alan_kodu->currentText();
    QString is_tel_ulke_kodu          = m_ui->combo_box_is_tel_ulke_kodu->currentText();
    QString is_tel_alan_kodu          = m_ui->combo_box_is_tel_alan_kodu->currentText();
    QString cep_tel_ulke_kodu         = m_ui->combo_box_cep_tel_ulke_kodu->currentText();
    QString cep_tel_gsm_kodu          = m_ui->combo_box_cep_tel_gsm_kodu->currentText();
    QString fax_ulke_kodu             = m_ui->combo_box_fax_ulke_kodu->currentText();
    QString fax_alan_kodu             = m_ui->combo_box_fax_alan_kodu->currentText();
    QString is_tel_dahili             = m_ui->lineEdit_dahili->text();

    M_ADR_STRUCT->firma_sahis_adi     = m_ui->lineEdit_sahis_adi_soyadi->text();
    M_ADR_STRUCT->dogum_tarihi        = m_ui->adak_date_dogum_tarihi->DATE();
    M_ADR_STRUCT->dogum_yeri          = m_ui->line_edit_dogum_yeri->text();
    M_ADR_STRUCT->cinsiyet            = m_ui->combo_box_cinsiyet->currentText();
    M_ADR_STRUCT->ogrenim_durumu      = m_ui->combo_box_egitim_durumu->currentText();
    M_ADR_STRUCT->medeni_hali         = m_ui->combo_box_medeni_hali->currentText();
    M_ADR_STRUCT->kan_grubu           = m_ui->combo_box_kan_grubu->currentText();
    M_ADR_STRUCT->meslek              = m_ui->combo_box_meslek->currentText();
    M_ADR_STRUCT->kimlik_no           = m_ui->lineEdit_kimlik_no->text();
    M_ADR_STRUCT->vergi_dairesi       = m_ui->combo_box_vergi_dairesi->currentText();
    M_ADR_STRUCT->vergi_no            = m_ui->lineEdit_vergi_no->text();
    M_ADR_STRUCT->adres_satiri_1      = m_ui->lineEdit_adres_satiri_1->text();
    M_ADR_STRUCT->adres_satiri_2      = m_ui->lineEdit_adres_satiri_2->text();
    M_ADR_STRUCT->adres_satiri_3      = m_ui->lineEdit_adres_satiri_3->text();
    M_ADR_STRUCT->ilce                = m_ui->comboBox_ilce->currentText();
    M_ADR_STRUCT->semt                = m_ui->line_edit_semt->text();
    M_ADR_STRUCT->ulke                = m_ui->combo_box_ulke->currentText();
    M_ADR_STRUCT->sehir               = m_ui->combo_box_sehir->currentText();
    M_ADR_STRUCT->posta_kodu          = m_ui->line_edit_posta_kodu->text();
    M_ADR_STRUCT->tel_ulke_kodu       = tel_ulke_kodu;
    M_ADR_STRUCT->tel_alan_kodu       = tel_alan_kodu;
    M_ADR_STRUCT->telefon             = ev_telefonu;
    M_ADR_STRUCT->is_tel_ulke_kodu    = is_tel_ulke_kodu;
    M_ADR_STRUCT->is_tel_alan_kodu    = is_tel_alan_kodu;
    M_ADR_STRUCT->is_telefonu         = is_telefonu;
    M_ADR_STRUCT->is_telefonu_dahili  = is_tel_dahili;
    M_ADR_STRUCT->cep_tel_ulke_kodu   = cep_tel_ulke_kodu;
    M_ADR_STRUCT->cep_tel_gsm_kodu    = cep_tel_gsm_kodu;
    M_ADR_STRUCT->cep_telefonu        = cep_telefonu;
    M_ADR_STRUCT->fax_ulke_kodu       = fax_ulke_kodu;
    M_ADR_STRUCT->fax_alan_kodu       = fax_alan_kodu;
    M_ADR_STRUCT->fax                 = fax;
    M_ADR_STRUCT->email               = m_ui->line_edit_email->text();
    M_ADR_STRUCT->website             = m_ui->lineEdit_website->text();
    M_ADR_STRUCT->yetkili_1           = m_ui->lineEdit_yetkili_1->text();
    M_ADR_STRUCT->yetkili_1_info      = m_ui->lineEdit_yetkili_1_info->text();
    M_ADR_STRUCT->yetkili_2           = m_ui->lineEdit_yetkili_2->text();
    M_ADR_STRUCT->yetkili_2_info      = m_ui->lineEdit_yetkili_2_info->text();
    M_ADR_STRUCT->yetkili_3           = m_ui->lineEdit_yetkili_3->text();
    M_ADR_STRUCT->yetkili_3_info      = m_ui->lineEdit_yetkili_3_info->text();

    if ( m_kart_turu EQ SAHIS_KAYDI ) {
        M_ADR_STRUCT->unvan           = m_ui->combo_box_unvan->currentText();
    }
    else {
        M_ADR_STRUCT->firma_sektoru   = m_ui->combo_box_unvan->currentText();
    }

    if ( m_fotograf_path.isEmpty() EQ false ) {
        M_ADR_STRUCT->fotograf_logo   =   ADAK_PIXMAP_2_BYTE_ARRAY(m_fotograf_path);
    }
    else {
        M_ADR_STRUCT->fotograf_logo   = QByteArray();
    }

    ADR_KART_GUNCELLE( M_ADR_STRUCT, p_adres_id );

    GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI,ADRES_MODULU,p_adres_id,&m_grup_idleri);

    QString log_detaylari = QObject::tr("Log Türü: ŞAHIS KAYDI");
    log_detaylari.append ( " # Şahıs Adı Soyadı: " );
    log_detaylari.append ( m_ui->lineEdit_sahis_adi_soyadi->text() );

    E9_LOG_KAYDI_EKLE(ADRES_MODULU,LOG_ADR_SAHIS_KAYDI,LOG_ISLEM_UPDATE ,log_detaylari);

}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::CHECK_DELETE
***************************************************************************************/

int ADRES_KAYIT_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );

    if ( m_modul_id NE ADRES_MODULU OR m_program_id NE E9_PROGRAMI ) {
        MSG_WARNING( tr ( "Kayıt başka bir modülünden oluşturulmuş. Buradan silme işlemi gerçekleştirilemez"), NULL );
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "adr_adresler","firma_id","adres_id = :adres_id");
    sql_query.SET_VALUE      ( ":adres_id", record_id );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr ( "Silinecek kayıt bulunamadı" ), NULL );
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    if ( sql_query.VALUE(0).toInt() > 0 ) {

        int firma_id = sql_query.VALUE(0).toInt();

        sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi","adres_id = :adres_id" );
        sql_query.SET_VALUE      ( ":adres_id", firma_id );

        QString firma_adi;

        if ( sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
            firma_adi = sql_query.VALUE(0).toString();
        }

        int clicked_button = QMessageBox::question ( this, tr ( "Dikkat" ), tr ( "Seçili şahıs aynı zamanda <b>" + firma_adi.toUtf8() + "</b> firmasının yetkilisi. "
                                                                                 "Bu kaydı silmek şahsın firma yetkililerinden de silinmesine sebep olacaktır. "
                                                                                 "Emin misiniz ? " ), QMessageBox::Ok, QMessageBox::Cancel );

        if ( clicked_button EQ QMessageBox::Cancel ) {
            return ADAK_FAIL;
        }

    }

    return ADAK_OK;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::DELETE_RECORD
***************************************************************************************/

void ADRES_KAYIT_FORMU::DELETE_RECORD ( int p_adres_id )
{
    SQL_QUERY sql_query ( DB );

    QString firma_sahis_adi;
    sql_query.PREPARE_SELECT("adr_adresler","firma_sahis_adi","adres_id = :adres_id");
    sql_query.SET_VALUE(":adres_id" , p_adres_id);

    if(sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        firma_sahis_adi = sql_query.VALUE(0).toString();
    }

    QString log_detaylari = QObject::tr("Log Türü: ŞAHIS KAYDI");
    log_detaylari.append ( " # Şahıs Adı Soyadı: " );
    log_detaylari.append ( firma_sahis_adi );

    E9_LOG_KAYDI_EKLE(ADRES_MODULU,LOG_ADR_SAHIS_KAYDI,LOG_ISLEM_DELETE ,log_detaylari);

    sql_query.PREPARE_DELETE ( "adr_telefonlar","adres_kayit_id = :adres_kayit_id" );
    sql_query.SET_VALUE      ( ":adres_kayit_id", p_adres_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE ( "adr_emailler","adres_kayit_id = :adres_kayit_id" );
    sql_query.SET_VALUE      ( ":adres_kayit_id", p_adres_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE ( "adr_not_defteri","adres_kayit_id = :adres_kayit_id" );
    sql_query.SET_VALUE      ( ":adres_kayit_id", p_adres_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE ( "adr_adresler","adres_id = :adres_id" );
    sql_query.SET_VALUE      ( ":adres_id",p_adres_id );
    sql_query.DELETE();

    GRP_KAYDIN_GRUPLARINI_SIL(E9_PROGRAMI , ADRES_MODULU , p_adres_id);
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::SELECT_RECORD
***************************************************************************************/

int ADRES_KAYIT_FORMU::SELECT_RECORD ()
{
    return OPEN_ADRES_ARAMA ( m_kart_turu ,ADRES_ARAMA_ISLEMI , this);
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::FIND_RECORD
***************************************************************************************/

int ADRES_KAYIT_FORMU::FIND_RECORD ()
{
    SQL_QUERY   query(DB);

    query.PREPARE_SELECT ( "adr_adresler","adres_id","firma_sahis_adi = :firma_sahis_adi "
                           "AND adres_kart_turu = :adres_kart_turu" );

    query.SET_VALUE      ( ":firma_sahis_adi",   m_ui->lineEdit_sahis_adi_soyadi->text() );
    if ( m_kart_turu EQ SAHIS_KAYDI ) {
        query.SET_VALUE      ( ":adres_kart_turu", SAHIS_KAYDI );
    }
    else {
        query.SET_VALUE      ( ":adres_kart_turu", FIRMA_KAYDI );
    }


    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int ADRES_KAYIT_FORMU::FIND_FIRST_RECORD ()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT ( "adr_adresler",
                           "adres_id",
                           "adres_kart_turu = :adres_kart_turu");

    if ( m_kart_turu EQ SAHIS_KAYDI ) {
        query.SET_VALUE      ( ":adres_kart_turu", SAHIS_KAYDI );
    }
    else {
        query.SET_VALUE      ( ":adres_kart_turu", FIRMA_KAYDI );
    }

    if ( query.SELECT("firma_sahis_adi",0,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int ADRES_KAYIT_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT ( "adr_adresler","adres_id","adres_kart_turu = :adres_kart_turu");

    if ( m_kart_turu EQ SAHIS_KAYDI ) {
        query.SET_VALUE      ( ":adres_kart_turu", SAHIS_KAYDI );
    }
    else {
        query.SET_VALUE      ( ":adres_kart_turu", FIRMA_KAYDI );
    }

    if ( query.SELECT("firma_sahis_adi DESC",0,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int ADRES_KAYIT_FORMU::FIND_PREV_RECORD ()
{
    SQL_QUERY  query ( DB );

    query.PREPARE_SELECT       ( "adr_adresler","adres_id","firma_sahis_adi < :firma_sahis_adi "
                                 "AND adres_kart_turu = :adres_kart_turu" );

    query.SET_VALUE            ( ":firma_sahis_adi",         m_ui->lineEdit_sahis_adi_soyadi->text() );
    if ( m_kart_turu EQ SAHIS_KAYDI ) {
        query.SET_VALUE      ( ":adres_kart_turu", SAHIS_KAYDI );
    }
    else {
        query.SET_VALUE      ( ":adres_kart_turu", FIRMA_KAYDI );
    }

    if ( query.SELECT("firma_sahis_adi DESC",0,1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int ADRES_KAYIT_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY  query ( DB );

    query.PREPARE_SELECT       ( "adr_adresler","adres_id","firma_sahis_adi > :firma_sahis_adi "
                                 "AND adres_kart_turu = :adres_kart_turu" );

    query.SET_VALUE            ( ":firma_sahis_adi",         m_ui->lineEdit_sahis_adi_soyadi->text() );
    if ( m_kart_turu EQ SAHIS_KAYDI ) {
        query.SET_VALUE      ( ":adres_kart_turu", SAHIS_KAYDI );
    }
    else {
        query.SET_VALUE      ( ":adres_kart_turu", FIRMA_KAYDI );
    }

    if ( query.SELECT("firma_sahis_adi ASC",0,1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::LOCK_RECORD
***************************************************************************************/

int ADRES_KAYIT_FORMU::LOCK_RECORD ( int record_id )
{
    return DB->LOCK_ROW ( "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( record_id ) );
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::UNLOCK_RECORD
***************************************************************************************/

void ADRES_KAYIT_FORMU::UNLOCK_RECORD ( int record_id )
{
    DB->UNLOCK_ROW ( "adres_id","adr_adresler", QString ( "adres_id = %1" ).arg ( record_id ) );
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::SAVER_BUTTON_CLICKED
***************************************************************************************/

void ADRES_KAYIT_FORMU::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_adres_id  )
{
    if ( p_button EQ m_ui->button_not_defteri ) {
        OPEN_ADRES_NOT_DEFTERI_FISI ( p_adres_id,ADRES_MODULU,E9_PROGRAMI, this );
    }
    else if ( p_button EQ m_ui->button_telefonlar ) {
        OPEN_ADRES_TELEFON_FISI ( p_adres_id, this  );
    }
    else if ( p_button EQ m_ui->button_emailler ) {
        OPEN_ADRES_EMAIL_FISI ( p_adres_id, this );
    }
    else if ( p_button EQ m_ui->push_button_zarf_yazdir ) {
        OPEN_ADRES_ZARF_YAZDIR_BATCH ( p_adres_id, "" , this );
    }
//    else if ( p_button EQ m_ui->push_button_firma_yetkilileri ) {
//        OPEN_ADRES_YETKILILER_FISI( p_adres_id , this );
//    }
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int ADRES_KAYIT_FORMU::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    Q_UNUSED ( p_button );

    if ( p_button EQ m_ui->push_button_gruplar ) {

        if ( OPEN_GRUP_SECIM( E9_PROGRAMI,ADRES_MODULU , &m_grup_idleri , DB , this ) EQ true ) {
            m_ui->textEdit_gruplar->setText ( GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , new QStringList() , new QStringList()) );
            return ADAK_RECORD_CHANGED;
        }

        return ADAK_RECORD_UNCHANGED;
    }
    else {
        return FOTOGRAF_SEC ();
    }

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::ADRES_BILGILERINI_OLUSTUR
***************************************************************************************/

void ADRES_KAYIT_FORMU::ADRES_BILGILERINI_OLUSTUR()
{
    QString adres_line_1 = "";
    QString adres_line_2 = "";
    QString adres_line_3 = "";
    QString adres_line_4 = "";

    if ( m_ui->lineEdit_adres_satiri_1->text().isEmpty() EQ false ) {
        adres_line_1 = m_ui->lineEdit_adres_satiri_1->text();
    }
    if ( m_ui->lineEdit_adres_satiri_2->text().isEmpty() EQ false ) {
        adres_line_2 = m_ui->lineEdit_adres_satiri_2->text();
    }
    if ( m_ui->lineEdit_adres_satiri_3->text().isEmpty() EQ false ) {
        adres_line_3 = m_ui->lineEdit_adres_satiri_3->text();
    }
    if ( m_ui->line_edit_posta_kodu->text().isEmpty() EQ false ) {
        adres_line_3 += m_ui->line_edit_posta_kodu->text() + " " ;
    }
    if ( m_ui->line_edit_semt->text().isEmpty() EQ false ) {
        adres_line_3 += m_ui->line_edit_semt->text() + " " ;
    }
    if ( m_ui->comboBox_ilce->currentText().isEmpty() EQ false ) {
        adres_line_3 += m_ui->comboBox_ilce->currentText() + " " ;
    }
    if ( m_ui->combo_box_sehir->currentText().isEmpty() EQ false ) {
        adres_line_4 += m_ui->combo_box_sehir->currentText() + " ";
    }
    if ( m_ui->combo_box_ulke->currentText().isEmpty() EQ false ) {
        adres_line_4 += m_ui->combo_box_ulke->currentText() + " ";
    }

    m_ui->text_edit_adres->blockSignals ( true );
    m_ui->text_edit_adres->setText (adres_line_1 + "\n" + adres_line_2 + "\n" + adres_line_3 + "\n" + adres_line_4 );
    m_ui->text_edit_adres->blockSignals ( false );
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::FOTOGRAF_SEC
***************************************************************************************/

int ADRES_KAYIT_FORMU::FOTOGRAF_SEC()
{
    ADAK_ADD_PIXMAP_TO_BUTTON(m_ui->push_button_fotograf_sec,&m_fotograf_path,this);

    if ( m_fotograf_path.isEmpty() EQ true ) {
        return ADAK_RECORD_UNCHANGED;
    }

    return ADAK_RECORD_CHANGED;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::SET_WIDGETS_MAXIMUM_SIZE
***************************************************************************************/

void ADRES_KAYIT_FORMU::SET_WIDGETS_MAXIMUM_SIZE ()
{
    m_ui->lineEdit_sahis_adi_soyadi->setMaxLength ( 60 );
    m_ui->line_edit_dogum_yeri->setMaxLength      ( 30 );
    m_ui->lineEdit_kimlik_no->setMaxLength        ( 11 );
    m_ui->lineEdit_vergi_no->setMaxLength         ( 15 );
    m_ui->lineEdit_adres_satiri_1->setMaxLength   ( 35 );
    m_ui->lineEdit_adres_satiri_2->setMaxLength   ( 35 );
    m_ui->lineEdit_adres_satiri_3->setMaxLength   ( 35 );
    m_ui->line_edit_semt->setMaxLength            ( 30 );
    m_ui->line_edit_posta_kodu->setMaxLength      ( 10 );
    m_ui->line_edit_email->setMaxLength           ( 50 );
    m_ui->lineEdit_website->setMaxLength          ( 50 );
    m_ui->lineEdit_dahili->setMaxLength           ( 5  );

    m_ui->lineEdit_yetkili_1->setMaxLength        ( 60 );
    m_ui->lineEdit_yetkili_1_info->setMaxLength   ( 128);
    m_ui->lineEdit_yetkili_2->setMaxLength        ( 60 );
    m_ui->lineEdit_yetkili_2_info->setMaxLength   ( 128);
    m_ui->lineEdit_yetkili_3->setMaxLength        ( 60 );
    m_ui->lineEdit_yetkili_3_info->setMaxLength   ( 128);

    SET_COMBOBOX_LENGTH( m_ui->comboBox_ilce                 , 30 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_sehir               , 30 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_ulke                , 30 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_cep_tel_gsm_kodu    , 20 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_cep_tel_ulke_kodu   , 20 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_is_tel_ulke_kodu    , 10 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_is_tel_alan_kodu    , 10 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_fax_ulke_kodu       , 10 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_fax_alan_kodu       , 10 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_cinsiyet            , 5  ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_egitim_durumu       , 20 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_vergi_dairesi       , 50 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_kan_grubu           , 10 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_ev_tel_ulke_kodu    , 10 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_ev_tel_alan_kodu    , 10 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_medeni_hali         , 10 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_meslek              , 128 ) ;
    SET_COMBOBOX_LENGTH( m_ui->combo_box_unvan               , 128 ) ;
}

/**************************************************************************************
                      ADRES_KAYIT_FORMU::EKRANI_DUZENLE
***************************************************************************************/

void ADRES_KAYIT_FORMU::EKRANI_DUZENLE ()
{

    if ( m_kart_turu EQ FIRMA_KAYDI ) {
        // Firma kaydi

        m_ui->label_sahis_adi_soyadi->setText( "Firma Adı" );
        m_ui->label_unvan->setText( "Sektör" );

        //m_ui->frame_kimlik_bilgileri->setVisible    ( false );
        m_ui->label_meslek->setHidden               ( true );
        m_ui->combo_box_meslek->setHidden           ( true );
        m_ui->label_dogum_tarihi->setHidden         ( true );
        m_ui->adak_date_dogum_tarihi->setHidden    ( true );
        m_ui->label_cinsiyet->setHidden             ( true );
        m_ui->label_dogum_yeri->setHidden           ( true );
        m_ui->line_edit_dogum_yeri->setHidden       ( true );
        m_ui->label_egitim_durumu->setHidden        ( true );
        m_ui->combo_box_egitim_durumu->setHidden    ( true );
        m_ui->label_tc_kimlik_no->setHidden         ( true );
        m_ui->lineEdit_kimlik_no->setHidden         ( true );
        m_ui->label_kan_grubu->setHidden            ( true );
        m_ui->combo_box_kan_grubu->setHidden        ( true );
        m_ui->combo_box_medeni_hali->setHidden      ( true );
        m_ui->label_medeni_hal->setHidden           ( true );
        m_ui->combo_box_cinsiyet->setHidden         ( true );

        // ev tel hidelandi
        m_ui->label_ev_tel->setText("Telefon-1");

        // is telefon telefon yapildi

        m_ui->label_is_telefon->setText( "Telefon-2" );

        // unvan silinip yerine sektor dolduruluyor
        m_ui->combo_box_unvan->clear();
        GET_SEKTORLER ( m_ui->combo_box_unvan );

        m_ui->label_yetkili_1->setVisible( true );
        m_ui->label_yetkili_2->setVisible( true );
        m_ui->label_yetkili_3->setVisible( true );
        m_ui->lineEdit_yetkili_1->setVisible( true );
        m_ui->lineEdit_yetkili_1_info->setVisible( true );
        m_ui->lineEdit_yetkili_2->setVisible( true );
        m_ui->lineEdit_yetkili_2_info->setVisible( true );
        m_ui->lineEdit_yetkili_3->setVisible( true );
        m_ui->lineEdit_yetkili_3_info->setVisible( true );


    }
    else {
        m_ui->label_ev_tel->setText("Ev Telefonu");
        m_ui->label_is_telefon->setText( "İş Telefonu" );

        m_ui->label_sahis_adi_soyadi->setText( "Adı Soyadı" );
        m_ui->label_unvan->setText( "Ünvan" );

        m_ui->label_meslek->setHidden               ( false );
        m_ui->combo_box_meslek->setHidden           ( false );
        m_ui->label_dogum_tarihi->setHidden         ( false );
        m_ui->adak_date_dogum_tarihi->setHidden    ( false );
        m_ui->label_cinsiyet->setHidden             ( false );
        m_ui->label_dogum_yeri->setHidden           ( false );
        m_ui->line_edit_dogum_yeri->setHidden       ( false );
        m_ui->label_egitim_durumu->setHidden        ( false );
        m_ui->combo_box_egitim_durumu->setHidden    ( false );
        m_ui->label_tc_kimlik_no->setHidden         ( false );
        m_ui->lineEdit_kimlik_no->setHidden         ( false );
        m_ui->label_kan_grubu->setHidden            ( false );
        m_ui->combo_box_kan_grubu->setHidden        ( false );
        m_ui->combo_box_medeni_hali->setHidden      ( false );
        m_ui->label_medeni_hal->setHidden           ( false );
        m_ui->combo_box_cinsiyet->setHidden         ( false );

        GET_UNVANLAR            ( m_ui->combo_box_unvan       );


        m_ui->label_yetkili_1->setVisible( false );
        m_ui->label_yetkili_2->setVisible( false );
        m_ui->label_yetkili_3->setVisible( false );
        m_ui->lineEdit_yetkili_1->setVisible( false );
        m_ui->lineEdit_yetkili_1_info->setVisible( false);
        m_ui->lineEdit_yetkili_2->setVisible( false);
        m_ui->lineEdit_yetkili_2_info->setVisible( false );
        m_ui->lineEdit_yetkili_3->setVisible( false );
        m_ui->lineEdit_yetkili_3_info->setVisible( false );
    }
}

/**************************************************************************************
                   ADRES_KAYIT_FORMU::KULLANICI_YETKILERINI_AYARLA
***************************************************************************************/

void ADRES_KAYIT_FORMU::KULLANICI_YETKILERINI_AYARLA ()
{
    E9_KULLANICI_YETKILERI_STRUCT *KULLANICI_YETKILERI = ( E9_KULLANICI_YETKILERI_STRUCT * ) E9_GET_KULLANICI_YETKILERI();

    if ( KULLANICI_YETKILERI-> adr_not_defteri_inceleme_yetkisi EQ false ) {
        m_ui->button_not_defteri->hide();
    }
}
