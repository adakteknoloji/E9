#include <QHeaderView>
#include <QMessageBox>
#include "adak_utils.h"
#include "ui_cari_kart_formu.h"
#include "cari_kart_formu_class.h"
#include "cari_kart_formu_open.h"
#include "adak_sql.h"
#include "print.h"
#include "adak_std_utils.h"
#include "cari_oto_ekstre_arama_open.h"
#include "e9_console_utils.h"
#include "muh_console_utils.h"
#include "cari_console_utils.h"
#include "adres_gui_utils.h"
#include "adres_console_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_yetki.h"
#include "adak_gruplar.h"
#include "sube_console_utils.h"
#include "cari_kart_arama_open.h"

#include "muh_hesap_arama_open.h"
#include "dvz_gui_utils.h"
#include "cari_struct.h"
#include "cari_enum.h"
#include "cari_console_utils.h"
#include "cari_gui_utils.h"

extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   OPEN_CARI_KART_FORMU
***************************************************************************************/

void OPEN_CARI_KART_FORMU ( int fis_id , int cari_kart_turu , QWidget * parent )
{
    CARI_KART_FORMU * F = new CARI_KART_FORMU ( fis_id, cari_kart_turu, parent );
    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   CARI_KART_FORMU::CARI_KART_FORMU
***************************************************************************************/

CARI_KART_FORMU::CARI_KART_FORMU ( int fis_id, int cari_kart_turu,QWidget * parent ): FORM_KERNEL ( parent ), m_ui  ( new Ui::CARI_KART_FORMU )
{
    m_fis_id                = fis_id;
    m_cari_kart_turu        = cari_kart_turu;
    m_modul_id              = CARI_MODULU;
    m_program_id            = E9_PROGRAMI;

    m_ui->setupUi    ( this );
    START_FORM_KERNEL ( this, DB );


 }

/**************************************************************************************
                   CARI_KART_FORMU::SETUP_FORM
***************************************************************************************/

void CARI_KART_FORMU::SETUP_FORM()
{

    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_button_widget );

    m_ui->adakDate_dogum_tarihi->MALI_YIL_KISITI_YOK( true );

    m_ui->textEdit_gruplar->setReadOnly( true );

    m_muh_hesap_id          =  0;

    REGISTER_CHANGER_BUTTON(m_ui->push_button_fotograf_sec);

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchedit_muhasebe_hesabi);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_muh_hesap_ismi);

    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_cari_kart_tipi );

    CARI_FILL_CARI_KART_TIPI_COMBO_BOX( m_ui->comboBox_cari_kart_tipi );

    // FIRMA ISE PERSONEL SECENEGI GORUNMEMELI
    if ( m_cari_kart_turu EQ FIRMA_KAYDI ) {
        m_ui->comboBox_cari_kart_tipi->removeItem(2);
        SET_HELP_PAGE     ( "cari-hesap-islemleri_firma-cari-karti" );
        GET_SEKTORLER ( m_ui->comboBox_sektor_or_meslek );
    }
    else {
        GET_MESLEKLER ( m_ui->comboBox_sektor_or_meslek );
        m_ui->label_sektor_or_meslek->setText( "Meslek" );
        SET_HELP_PAGE     ( "cari-hesap-islemleri_sahis-cari-karti" );
    }

    m_ui->lineedit_muh_hesap_ismi->setMaxLength(60);

    QString cari_ent_hesap_kodu, cari_ent_hesap_adi;

    m_muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "cari_alici_ent_hesap_id", &cari_ent_hesap_kodu, &cari_ent_hesap_adi );

    m_ui->searchedit_muhasebe_hesabi->SET_TEXT  ( cari_ent_hesap_kodu );
    m_ui->lineedit_muh_hesap_ismi->setText      ( cari_ent_hesap_adi  );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->searchedit_muhasebe_hesabi->setVisible ( false );
        m_ui->lineedit_muh_hesap_ismi->setVisible    ( false );
        m_ui->label_muh_hesab->setVisible           ( false );
    }

    m_ui->commaedit_borc_alacak->SET_RETURN_EMPTY ( false );
    m_ui->lineedit_vergi_no->setMaxLength(15);

    GET_ILLER           ( m_ui->combo_box_sehir        );
    GET_ULKELER         ( m_ui->combo_box_ulke         );

    GET_CINSIYETLER         ( m_ui->combo_box_cinsiyet      );
    GET_OGRENIM_DURUMLARI   ( m_ui->combo_box_egitim_durumu );
    GET_MEDENI_HALLER       ( m_ui->combo_box_medeni_hali );
    GET_KAN_GRUPLARI        ( m_ui->combo_box_kan_grubu   );

    EKRANI_DUZENLE();

    REGISTER_CHANGER_BUTTON ( m_ui->push_button_gruplar          );
    REGISTER_SAVER_BUTTON   ( m_ui->push_button_zarf_yazdir      );
    REGISTER_SAVER_BUTTON   ( m_ui->push_button_not_defteri      );
    REGISTER_SAVER_BUTTON   ( m_ui->push_button_telefon          );
    REGISTER_SAVER_BUTTON   ( m_ui->push_button_email            );
    REGISTER_SAVER_BUTTON   ( m_ui->pushButton_cari_hareketleri  );

    if ( E9_KULLANICI_CARI_HAREKETLERINI_GOREBILIR_MI() EQ 0 ) {
        m_ui->pushButton_cari_hareketleri->setVisible( false );
    }

    DVZ_FILL_COMBOBOX ( m_ui->comboBox_para_birimi );

    m_ui->comboBox_para_birimi->setCurrentIndex ( m_ui->comboBox_para_birimi->findText(DVZ_GET_TEMEL_PARA_BIRIMI_KODU()));
    m_ui->comboBox_para_birimi->setEditable(false);

    GET_ULKE_TELEFON_KODLARI ( m_ui->combobox_ev_tel_ulke_kodu  );
    GET_ULKE_TELEFON_KODLARI ( m_ui->combo_box_cep_ulke_kodu    );
    GET_ULKE_TELEFON_KODLARI ( m_ui->combo_box_fax_ulke_kodu    );
    GET_ULKE_TELEFON_KODLARI ( m_ui->combo_box_is_tel_ulke_kodu );

    DISABLE_CLEAR_ON_WIDGET(m_ui->combo_box_ulke);

    SET_COMBOBOX_LENGTH( m_ui->combo_box_ulke                , 30 );
    SET_COMBOBOX_LENGTH( m_ui->comboBox_sektor_or_meslek     , 30 );

    m_ui->combo_box_ulke->setEditText( tr ( "Türkiye" ) );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_tel_ulke_kodu, "90" );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cep_ulke_kodu,    "90" );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_ulke_kodu,    "90" );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combobox_ev_tel_ulke_kodu,  "90" );

    GET_GSM_KODLARI ( m_ui->combo_box_cep_gsm_kodu     );
    GET_ALAN_KODLARI( m_ui->combobox_ev_tel_alan_kodu  );
    GET_ALAN_KODLARI( m_ui->combo_box_is_tel_alan_kodu );
    GET_ALAN_KODLARI( m_ui->combo_box_fax_alan_kodu    );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_cari_hesap_kodu );

    m_ui->lineedit_cari_hesap_ismi->setMaxLength(60);
    m_ui->lineedit_cari_hesap_kodu->setMaxLength(30);
    m_ui->lineedit_ev_telefonu->setMaxLength(10);
    m_ui->lineedit_is_telefonu->setMaxLength(10);
    m_ui->lineedit_muh_hesap_ismi->setMaxLength(60);
    m_ui->lineedit_vergi_no->setMaxLength(15);
    m_ui->line_edit_adres_satiri->setMaxLength(35);
    m_ui->line_edit_adres_satiri_2->setMaxLength(35);
    m_ui->line_edit_adres_satiri_3->setMaxLength(35);
    m_ui->line_edit_cep_telefonu->setMaxLength(10);
    m_ui->line_edit_email->setMaxLength(50);
    m_ui->line_edit_fax->setMaxLength(10);
    m_ui->line_edit_posta_kodu->setMaxLength(10);
    m_ui->line_edit_semt->setMaxLength(30);
    m_ui->line_edit_web_adresi->setMaxLength(50);

    m_ui->lineEdit_kimlik_no->setMaxLength ( 11 );
    m_ui->line_edit_dogum_yeri->setMaxLength    ( 20 );

    SET_COMBOBOX_LENGTH(m_ui->combo_box_cinsiyet      , 10);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_kan_grubu     , 10);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_medeni_hali   , 30);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_egitim_durumu , 50);

    SET_COMBOBOX_LENGTH(m_ui->combobox_ev_tel_alan_kodu , 10);
    SET_COMBOBOX_LENGTH(m_ui->combobox_ev_tel_ulke_kodu , 10);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_ilce             , 30);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_para_birimi      , 5);
    SET_COMBOBOX_LENGTH(m_ui->combobox_vergi_dairesi    , 50);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_cep_gsm_kodu    , 10);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_cep_ulke_kodu   , 10);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_fax_alan_kodu   , 10);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_fax_ulke_kodu   , 10);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_is_tel_alan_kodu, 10);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_is_tel_ulke_kodu, 10);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_sehir           , 30);
    SET_COMBOBOX_LENGTH(m_ui->combo_box_ulke            , 30);

    SET_WIDGETS_MAXIMUM_SIZE ();


    FOCUS_FIRST_WIDGET();
}

/**************************************************************************************
                   CARI_KART_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void CARI_KART_FORMU::CLEAR_FORM_MEMBERS()
{
    m_grup_idleri.clear();
    m_fotograf_path.clear();
}

/**************************************************************************************
                   CARI_KART_FORMU::NEW_RECORD
***************************************************************************************/

void CARI_KART_FORMU::NEW_RECORD()
{
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_tel_ulke_kodu, "90" );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cep_ulke_kodu,    "90" );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_ulke_kodu,    "90" );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combobox_ev_tel_ulke_kodu,  "90" );

    m_ui->push_button_fotograf_sec->setText( tr( "Fotoğraf/Logo Seç" ) );


    m_ui->label_borc_alacak->setText                    ("<font color = #000000 > Borcu / </font> <font color = #FF0000 > Alacağı </font>");
    m_ui->commaedit_borc_alacak->setStyleSheet          ( "color: black" );

    m_para_birimi_id = DVZ_GET_TEMEL_PARA_BIRIMI_ID();
    m_ui->comboBox_para_birimi->setCurrentIndex(m_ui->comboBox_para_birimi->findText(DVZ_GET_TEMEL_PARA_BIRIMI_KODU()));

}

/**************************************************************************************
                   CARI_KART_FORMU::GET_RECORD
***************************************************************************************/

int CARI_KART_FORMU::GET_RECORD ( int fis_id )
{
    SQL_QUERY select_query ( DB );

    m_fis_id = fis_id;

    select_query.PREPARE_SELECT ( "car_hesaplar",
                                  "cari_hesap_kodu, adres_id, muh_hesap_id ",
                                  "cari_hesap_id = :cari_hesap_id" );
    select_query.SET_VALUE      ( ":cari_hesap_id" , fis_id );

    if ( select_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    select_query.NEXT();

    BORC_BILGISI borc_bilgisi;

    CARI_KART_BORC_BILGISINI_GETIR( fis_id, &borc_bilgisi );  // borc bilgisini donderir

    m_ui->lineedit_cari_hesap_kodu->setText ( select_query.VALUE("cari_hesap_kodu").toString() );
    m_ui->lineedit_cari_hesap_ismi->setText ( ADR_GET_FIRMA_SAHIS_ADI(select_query.VALUE("adres_id").toInt()) );


    m_ui->textEdit_gruplar->setText ( GRP_KAYDIN_GRUPLARINI_BUL ( E9_PROGRAMI , CARI_MODULU, fis_id, &m_grup_idleri, new QStringList, new QStringList ) );


    int muh_hesap_id = select_query.VALUE("muh_hesap_id").toInt();
    m_muh_hesap_id = muh_hesap_id;

    m_ui->searchedit_muhasebe_hesabi->SET_TEXT ( MUH_GET_HESAP_KODU ( muh_hesap_id ) );
    m_ui->lineedit_muh_hesap_ismi->setText ( MUH_GET_HESAP_ISMI ( muh_hesap_id ) );

    m_para_birimi_id =  borc_bilgisi.para_birimi_id;
    m_ui->comboBox_para_birimi->setCurrentIndex(m_ui->comboBox_para_birimi->findText(DVZ_GET_DOVIZ_KODU(m_para_birimi_id)));


    if ( borc_bilgisi.borc_bakiye > 0.0 ) {
       m_ui->label_borc_alacak->setText ( "Borcu" );
       m_ui->commaedit_borc_alacak->SET_DOUBLE( borc_bilgisi.borc_bakiye );
       m_ui->commaedit_borc_alacak->setStyleSheet ( "color: black");
    }
    else if ( borc_bilgisi.alacak_bakiye > 0.0 ) {

        m_ui->label_borc_alacak->setText ( "<font color = #FF0000 > Alacağı </font>");
        m_ui->commaedit_borc_alacak->SET_DOUBLE (  borc_bilgisi.alacak_bakiye );
        m_ui->commaedit_borc_alacak->setStyleSheet ( "color: red");
    }
    else {
        m_ui->commaedit_borc_alacak->clear                  ();
        m_ui->label_borc_alacak->setText                    ("<font color = #000000 > Borcu / </font> <font color = #FF0000 > Alacağı </font>");
        m_ui->commaedit_borc_alacak->setStyleSheet          ( "color: black" );
    }


    CARI_HESAP_STRUCT * CARI_HESAP  =  new CARI_HESAP_STRUCT;
    CARI_SET_HESAP_STRUCT_DEFAULTS(CARI_HESAP);

    CARI_KART_BILGILERINI_OKU( fis_id, CARI_HESAP );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_cari_kart_tipi, CARI_GET_CARI_KART_TIPI_STRING( CARI_HESAP->cari_kart_tipi ));

    m_cari_kart_turu    = CARI_HESAP->adres_kart_turu;

    EKRANI_DUZENLE();

    m_ui->lineEdit_yetkili_1->setText( CARI_HESAP->yetkili_1 );
    m_ui->lineEdit_yetkili_1_info->setText( CARI_HESAP->yetkili_1_info );
    m_ui->lineEdit_yetkili_2->setText( CARI_HESAP->yetkili_2 );
    m_ui->lineEdit_yetkili_2_info->setText( CARI_HESAP->yetkili_2_info );
    m_ui->lineEdit_yetkili_3->setText( CARI_HESAP->yetkili_3 );
    m_ui->lineEdit_yetkili_3_info->setText( CARI_HESAP->yetkili_3_info );

    m_ui->adakDate_dogum_tarihi->SET_DATE( CARI_HESAP->dogum_tarihi );

    //int kaydi_ureten_modul = sql_query.VALUE ( "kaydi_ureten_modul" ).toInt();

    m_ui->lineedit_vergi_no  ->setText          ( CARI_HESAP->vergi_no               );
    m_ui->line_edit_adres_satiri->setText       ( CARI_HESAP->adres_satiri_1          );
    m_ui->line_edit_adres_satiri_2->setText     ( CARI_HESAP->adres_satiri_2         );
    m_ui->line_edit_adres_satiri_3->setText     ( CARI_HESAP->adres_satiri_3         );
    m_ui->line_edit_semt->setText               ( CARI_HESAP->semt                   );
    m_ui->line_edit_posta_kodu->setText         ( CARI_HESAP->posta_kodu             );
    m_ui->line_edit_email->setText              ( CARI_HESAP->email                  );
    m_ui->line_edit_web_adresi->setText         ( CARI_HESAP->website                );

    m_modul_id         = CARI_HESAP->modul_id;
    m_program_id       = CARI_HESAP->program_id;

    //FIND_AND_SET_COMBOBOX_TEXT                  ( m_ui->comboBox_sektoru, sql_query.VALUE ( "firma_sektoru" ).toString()       );
    FIND_AND_SET_COMBOBOX_TEXT  ( m_ui->combo_box_ulke, CARI_HESAP->ulke );

    // Şehir Bilgileri

    m_ui->combo_box_sehir->clear();

    GET_ILLER                    ( m_ui->combo_box_sehir, m_ui->combo_box_ulke->currentText() );
    FIND_AND_SET_COMBOBOX_TEXT   ( m_ui->combo_box_sehir, CARI_HESAP->sehir );

    m_ui->comboBox_ilce->clear();

    GET_ILCELER                  ( m_ui->comboBox_ilce, m_ui->combo_box_sehir->currentText() );
    FIND_AND_SET_COMBOBOX_TEXT   ( m_ui->comboBox_ilce, CARI_HESAP->ilce );

    m_ui->combobox_vergi_dairesi->clear();

    GET_VERGI_DAIRELERI          ( m_ui->combobox_vergi_dairesi, m_ui->combo_box_sehir->currentText() );
    FIND_AND_SET_COMBOBOX_TEXT   ( m_ui->combobox_vergi_dairesi, CARI_HESAP->vergi_dairesi );

    // Is Tel Ulke kodu

    m_ui->combo_box_is_tel_alan_kodu->clear();

    FIND_AND_SET_COMBOBOX_TEXT   ( m_ui->combo_box_is_tel_ulke_kodu, CARI_HESAP->is_tel_ulke_kodu );
    GET_ALAN_KODLARI             ( m_ui->combo_box_is_tel_alan_kodu, "" , m_ui->combo_box_ulke->currentText() );
    FIND_AND_SET_COMBOBOX_TEXT   ( m_ui->combo_box_is_tel_alan_kodu, CARI_HESAP->is_tel_alan_kodu );

    m_ui->lineedit_is_telefonu->setText( CARI_HESAP->is_telefonu );

    m_ui->combobox_ev_tel_alan_kodu->clear();

    FIND_AND_SET_COMBOBOX_TEXT   ( m_ui->combobox_ev_tel_ulke_kodu, CARI_HESAP->tel_ulke_kodu );
    GET_ALAN_KODLARI             ( m_ui->combobox_ev_tel_alan_kodu, "" , m_ui->combo_box_ulke->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT   ( m_ui->combobox_ev_tel_alan_kodu, CARI_HESAP->tel_alan_kodu );

    m_ui->lineedit_ev_telefonu->setText( CARI_HESAP->telefon );

    // Cep Telefon kodları

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cep_ulke_kodu, CARI_HESAP->cep_tel_ulke_kodu );

    GET_GSM_KODLARI ( m_ui->combo_box_cep_gsm_kodu, m_ui->combo_box_cep_ulke_kodu->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cep_gsm_kodu, CARI_HESAP->cep_tel_gsm_kodu );

    m_ui->line_edit_cep_telefonu->setText ( CARI_HESAP->cep_telefonu );


    // Fax Telefon kodları

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_ulke_kodu, CARI_HESAP->fax_ulke_kodu );

    m_ui->combo_box_fax_alan_kodu->clear();

    GET_ALAN_KODLARI ( m_ui->combo_box_fax_alan_kodu, "" , m_ui->combo_box_ulke->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_alan_kodu, CARI_HESAP->fax_alan_kodu);

    m_ui->line_edit_fax->setText ( CARI_HESAP->fax );

    if ( m_cari_kart_turu EQ FIRMA_KAYDI ) {
        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_sektor_or_meslek, CARI_HESAP->firma_sektoru );
    }
    else {
        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_sektor_or_meslek, CARI_HESAP->meslek );
    }

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_kan_grubu     , CARI_HESAP->kan_grubu      );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_medeni_hali   , CARI_HESAP->medeni_hali    );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_egitim_durumu , CARI_HESAP->ogrenim_durumu );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cinsiyet      , CARI_HESAP->cinsiyet       );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_ulke, CARI_HESAP->ulke );

    m_ui->lineEdit_kimlik_no->setText( CARI_HESAP->kimlik_no );
    m_ui->line_edit_dogum_yeri->setText( CARI_HESAP->dogum_yeri );

    m_fotograf_path = "";
    m_ui->push_button_fotograf_sec->setIcon( QIcon( m_fotograf_path ) );
    m_ui->push_button_fotograf_sec->setText( tr( "Fotoğraf/Logo Seç" ) );

    if ( QVariant(CARI_HESAP->fotograf_logo).toString().isEmpty() EQ false ) {
        m_ui->push_button_fotograf_sec->setText ( "" );
        m_ui->push_button_fotograf_sec->setIcon ( QIcon(ADAK_BYTE_ARRAY_2_PIXMAP(CARI_HESAP->fotograf_logo)) );
        int w = m_ui->push_button_fotograf_sec->width();
        int h = m_ui->push_button_fotograf_sec->height();
        m_ui->push_button_fotograf_sec->setIconSize   ( QSize(w-4,h-4) );
        m_ui->push_button_fotograf_sec->setMinimumSize( QSize(w,h) );
        m_ui->push_button_fotograf_sec->setMaximumSize( QSize(w,h) );
    }

    ADRES_BILGILERINI_GOSTER();

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_KART_FORMU::CHECK_VAR
***************************************************************************************/

int CARI_KART_FORMU::CHECK_VAR ( QObject * object )
{

   if ( object EQ m_ui->searchedit_muhasebe_hesabi ) {

        if ( m_ui->searchedit_muhasebe_hesabi->GET_TEXT().isEmpty() EQ true) {
            m_ui->lineedit_muh_hesap_ismi->clear();
            m_muh_hesap_id = 0;
            return ADAK_OK;
        }

        int muh_hesap_id = MUH_GET_HESAP_ID ( m_ui->searchedit_muhasebe_hesabi->GET_TEXT() );
        if ( muh_hesap_id < 1 ) {
            MSG_WARNING(  tr ( "Aradığınız muhasebe hesabı bulunamadı" ), m_ui->searchedit_muhasebe_hesabi );

            return ADAK_FAIL_UNDO;
        }
        m_ui->lineedit_muh_hesap_ismi->setText(MUH_GET_HESAP_ISMI(muh_hesap_id));
        m_muh_hesap_id = muh_hesap_id;
    }
    else if ( object EQ m_ui->combobox_ev_tel_ulke_kodu ) {

        GET_ALAN_KODLARI(m_ui->combobox_ev_tel_alan_kodu , m_ui->combobox_ev_tel_ulke_kodu->currentText() );

        return ADAK_OK;

    }
    else if ( object EQ m_ui->combo_box_cep_ulke_kodu) {

        GET_GSM_KODLARI(m_ui->combo_box_cep_gsm_kodu , m_ui->combo_box_cep_ulke_kodu->currentText() );

        return ADAK_OK;
    }
    else if ( object EQ m_ui->combo_box_fax_ulke_kodu ) {

        GET_ALAN_KODLARI(m_ui->combo_box_fax_alan_kodu, m_ui->combo_box_fax_ulke_kodu->currentText() );

        return ADAK_OK;
    }
    else if ( object EQ m_ui->combo_box_is_tel_ulke_kodu ) {

        GET_ALAN_KODLARI(m_ui->combo_box_is_tel_alan_kodu, m_ui->combo_box_is_tel_ulke_kodu->currentText() );

        return ADAK_OK;

    }
    else if ( object EQ m_ui->combo_box_sehir ) {

        if ( m_ui->combo_box_sehir->currentText().size() > 30 ) {
            MSG_WARNING ( tr("Şehir isminin uzunluğu 30 karakterden fazla olamaz."), m_ui->combo_box_sehir );
            return ADAK_FAIL;
        }

        m_ui->comboBox_ilce->clear();

        QString il_telefon_kodu = GET_IL_ALAN_KODU ( m_ui->combo_box_sehir->currentText() , "");

        m_ui->combobox_vergi_dairesi->clear();

        GET_ILCELER         ( m_ui->comboBox_ilce, m_ui->combo_box_sehir->currentText() );
        GET_VERGI_DAIRELERI ( m_ui->combobox_vergi_dairesi, m_ui->combo_box_sehir->currentText() );

        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combobox_ev_tel_alan_kodu, il_telefon_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_alan_kodu,    il_telefon_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_tel_alan_kodu, il_telefon_kodu );

        m_ui->comboBox_ilce->setCurrentIndex(-1);
        return ADAK_OK;
    }
    else if ( object EQ m_ui->combo_box_ulke ) {

        QString ulke_tel_kodu = GET_ULKE_TELEFON_KODU ( m_ui->combo_box_ulke->currentText() );

        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cep_ulke_kodu    , ulke_tel_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_fax_ulke_kodu    , ulke_tel_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_tel_ulke_kodu , ulke_tel_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combobox_ev_tel_ulke_kodu  , ulke_tel_kodu );
    }

    else if( object EQ m_ui->comboBox_cari_kart_tipi ) {

        QString cari_ent_hesap_kodu, cari_ent_hesap_adi;
        if ( CARI_GET_CARI_KART_TIPI_ENUM( m_ui->comboBox_cari_kart_tipi->currentText()) EQ ENUM_CARI_SATICI ) {
            m_muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "cari_satici_ent_hesap_id", &cari_ent_hesap_kodu, &cari_ent_hesap_adi );;
        }
        else if ( CARI_GET_CARI_KART_TIPI_ENUM( m_ui->comboBox_cari_kart_tipi->currentText()) EQ ENUM_CARI_ALICI ) {
            m_muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "cari_alici_ent_hesap_id", &cari_ent_hesap_kodu, &cari_ent_hesap_adi );
        }
        else if ( CARI_GET_CARI_KART_TIPI_ENUM( m_ui->comboBox_cari_kart_tipi->currentText()) EQ ENUM_CARI_PERSONEL) {
            m_muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "personel_borclari_ent_hesap_id", &cari_ent_hesap_kodu, &cari_ent_hesap_adi );
        }
        else if ( CARI_GET_CARI_KART_TIPI_ENUM( m_ui->comboBox_cari_kart_tipi->currentText()) EQ ENUM_FIRMA_ORTAGI ) {
            m_muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "ortaklara_borclar_hesap_id", &cari_ent_hesap_kodu, &cari_ent_hesap_adi );
        }

        m_ui->searchedit_muhasebe_hesabi->SET_TEXT  ( cari_ent_hesap_kodu );
        m_ui->lineedit_muh_hesap_ismi->setText      ( cari_ent_hesap_adi  );
    }

    ADRES_BILGILERINI_GOSTER();

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_KART_FORMU::CHECK_EMPTY
***************************************************************************************/

int CARI_KART_FORMU::CHECK_EMPTY()
{

    if ( m_ui->searchedit_muhasebe_hesabi->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Muhasebe Hesabını boş bırakamazsınız." ), m_ui->searchedit_muhasebe_hesabi );
        return ADAK_FAIL;
    }
    if ( m_ui->lineedit_cari_hesap_kodu->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Cari Hesap Kodunu girmelisiniz."), m_ui->lineedit_cari_hesap_kodu );

        return ADAK_FAIL;
    }
    if ( m_ui->lineedit_cari_hesap_ismi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Cari Hesap İsmini girmelisiniz."), m_ui->lineedit_cari_hesap_ismi );

        return ADAK_FAIL;
    }


    if ( m_muh_hesap_id < 1 ) {
        MSG_WARNING( tr ( "Cari Hesabın entegre olacağı muhasebe hesabını seçmelisiniz" ), m_ui->searchedit_muhasebe_hesabi );
        return ADAK_FAIL;
    }


    return ADAK_OK;
}

/**************************************************************************************
                   CARI_KART_FORMU::CHECK_ADD
***************************************************************************************/

int CARI_KART_FORMU::CHECK_ADD()
{

    int cari_hesap_id = CARI_FIND_HESAP_ID ( m_ui->lineedit_cari_hesap_kodu->text() );

    if ( cari_hesap_id NE 0 ) {
        MSG_WARNING( tr("Aynı cari hesap kodu daha önce tanımlanmış.Lütfen cari hesap kodunu değiştiriniz!.."), m_ui->lineedit_cari_hesap_kodu );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_KART_FORMU::ADD_RECORD
***************************************************************************************/

int CARI_KART_FORMU::ADD_RECORD()
{
    CARI_HESAP_STRUCT cari_hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&cari_hesap_bilgileri);

    cari_hesap_bilgileri.cari_kart_tipi         = CARI_GET_CARI_KART_TIPI_ENUM( m_ui->comboBox_cari_kart_tipi->currentText() );
    cari_hesap_bilgileri.cari_hesap_ismi        = m_ui->lineedit_cari_hesap_ismi->text();
    cari_hesap_bilgileri.vergi_no               = m_ui->lineedit_vergi_no->text();
    cari_hesap_bilgileri.vergi_dairesi          = m_ui->combobox_vergi_dairesi->currentText();
    cari_hesap_bilgileri.adres_satiri_1         = m_ui->line_edit_adres_satiri->text();
    cari_hesap_bilgileri.adres_satiri_2         = m_ui->line_edit_adres_satiri_2->text();
    cari_hesap_bilgileri.adres_satiri_3         = m_ui->line_edit_adres_satiri_3->text();
    cari_hesap_bilgileri.ilce                   = m_ui->comboBox_ilce->currentText();
    cari_hesap_bilgileri.semt                   = m_ui->line_edit_semt->text();
    cari_hesap_bilgileri.ulke                   = m_ui->combo_box_ulke->currentText();
    cari_hesap_bilgileri.sehir                  = m_ui->combo_box_sehir->currentText();
    cari_hesap_bilgileri.posta_kodu             = m_ui->line_edit_posta_kodu->text();
    cari_hesap_bilgileri.is_tel_ulke_kodu       = m_ui->combo_box_is_tel_ulke_kodu->currentText();
    cari_hesap_bilgileri.is_tel_alan_kodu       = m_ui->combo_box_is_tel_alan_kodu->currentText();
    cari_hesap_bilgileri.is_telefonu            = m_ui->lineedit_is_telefonu->text().replace  ( "-", "" );
    cari_hesap_bilgileri.tel_ulke_kodu          = m_ui->combobox_ev_tel_ulke_kodu->currentText();
    cari_hesap_bilgileri.tel_alan_kodu          = m_ui->combobox_ev_tel_alan_kodu->currentText();
    cari_hesap_bilgileri.telefon                = m_ui->lineedit_ev_telefonu->text().replace  ( "-", "" );
    cari_hesap_bilgileri.cep_tel_ulke_kodu      = m_ui->combo_box_cep_ulke_kodu->currentText();
    cari_hesap_bilgileri.cep_tel_gsm_kodu       = m_ui->combo_box_cep_gsm_kodu->currentText();
    cari_hesap_bilgileri.cep_telefonu           = m_ui->line_edit_cep_telefonu->text().replace  ( "-", "" );
    cari_hesap_bilgileri.fax_ulke_kodu          = m_ui->combo_box_fax_ulke_kodu->currentText();
    cari_hesap_bilgileri.fax_alan_kodu          = m_ui->combo_box_fax_alan_kodu->currentText() ;
    cari_hesap_bilgileri.fax                    = m_ui->line_edit_fax->text().replace  ( "-", "" );
    cari_hesap_bilgileri.email                  = m_ui->line_edit_email->text();
    cari_hesap_bilgileri.website                = m_ui->line_edit_web_adresi->text();
    cari_hesap_bilgileri.cari_hesap_kodu        = m_ui->lineedit_cari_hesap_kodu->text();
    cari_hesap_bilgileri.muh_hesap_id           = m_muh_hesap_id;
    cari_hesap_bilgileri.para_birimi_id         = DVZ_GET_DOVIZ_ID( m_ui->comboBox_para_birimi->currentText() );

    cari_hesap_bilgileri.kimlik_no              = m_ui->lineEdit_kimlik_no->text();
    cari_hesap_bilgileri.cinsiyet               = m_ui->combo_box_cinsiyet->currentText();
    cari_hesap_bilgileri.dogum_yeri             = m_ui->line_edit_dogum_yeri->text();
    cari_hesap_bilgileri.medeni_hali            = m_ui->combo_box_medeni_hali->currentText();
    cari_hesap_bilgileri.kan_grubu              = m_ui->combo_box_kan_grubu->currentText();
    cari_hesap_bilgileri.ogrenim_durumu         = m_ui->combo_box_egitim_durumu->currentText();
    cari_hesap_bilgileri.dogum_tarihi           = m_ui->adakDate_dogum_tarihi->DATE();
    cari_hesap_bilgileri.yetkili_1              = m_ui->lineEdit_yetkili_1->text();
    cari_hesap_bilgileri.yetkili_1_info         = m_ui->lineEdit_yetkili_1_info->text();
    cari_hesap_bilgileri.yetkili_2              = m_ui->lineEdit_yetkili_2->text();
    cari_hesap_bilgileri.yetkili_2_info         = m_ui->lineEdit_yetkili_2_info->text();
    cari_hesap_bilgileri.yetkili_3              = m_ui->lineEdit_yetkili_3->text();
    cari_hesap_bilgileri.yetkili_3_info         = m_ui->lineEdit_yetkili_3_info->text();

    cari_hesap_bilgileri.program_id             = E9_PROGRAMI;
    cari_hesap_bilgileri.modul_id               = CARI_MODULU;
    cari_hesap_bilgileri.base_record_id         = 0;

    if ( m_cari_kart_turu EQ FIRMA_KAYDI ) {
        cari_hesap_bilgileri.firma_sektoru      = m_ui->comboBox_sektor_or_meslek->currentText();
    }
    else {
        cari_hesap_bilgileri.meslek             = m_ui->comboBox_sektor_or_meslek->currentText();
    }

    if ( m_fotograf_path.isEmpty() EQ false ) {
        cari_hesap_bilgileri.fotograf_logo          = ADAK_PIXMAP_2_BYTE_ARRAY(m_fotograf_path);
    }
    else {
        cari_hesap_bilgileri.fotograf_logo          = QByteArray();
    }
    int fis_id = CARI_KART_OLUSTUR(m_cari_kart_turu , &cari_hesap_bilgileri);

    GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI,CARI_MODULU , fis_id , &m_grup_idleri);

    return fis_id;
}

/**************************************************************************************
                   CARI_KART_FORMU::CHECK_UPDATE
***************************************************************************************/

int CARI_KART_FORMU::CHECK_UPDATE ( int fis_id )
{
    int cari_hesap_id = CARI_FIND_HESAP_ID ( m_ui->lineedit_cari_hesap_kodu->text() );

    if ( cari_hesap_id > 0 ) {
        if ( cari_hesap_id NE fis_id ) {
            MSG_WARNING( tr ( "Aynı cari hesap kodu daha önce başka bir cari kartta tanımlanmış. "
                                                      "Lütfen cari hesap kodunu değiştiriniz!.."), m_ui->lineedit_cari_hesap_kodu  );

            return ADAK_FAIL;
        }
    }

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "car_hesaplar",
                           "para_birimi_id",
                           "cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE      ( ":cari_hesap_id", fis_id );
    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Cari hesap bilgileri okunamadı"), m_ui->lineedit_cari_hesap_kodu );

        return ADAK_FAIL;
    }
    query.NEXT();

    int old_cari_para_birimi_id = query.VALUE(0).toInt();
    int new_cari_para_birimi_id = DVZ_GET_DOVIZ_ID ( m_ui->comboBox_para_birimi->currentText() );

    if ( new_cari_para_birimi_id NE old_cari_para_birimi_id ) {
        if ( CARI_HESAP_ISLEM_GORDU_MU ( fis_id ) EQ true ) {
            MSG_WARNING(  tr("Cari hesap işlem görmüştür. Para birimi değiştiremezsiniz."), m_ui->comboBox_para_birimi );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_KART_FORMU::UPDATE_RECORD
***************************************************************************************/

void CARI_KART_FORMU::UPDATE_RECORD ( int fis_id )
{
    m_fis_id = fis_id;

    CARI_HESAP_STRUCT cari_hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&cari_hesap_bilgileri);

    cari_hesap_bilgileri.cari_kart_tipi         = CARI_GET_CARI_KART_TIPI_ENUM( m_ui->comboBox_cari_kart_tipi->currentText() );
    cari_hesap_bilgileri.cari_hesap_ismi        = m_ui->lineedit_cari_hesap_ismi->text();
    cari_hesap_bilgileri.vergi_no               = m_ui->lineedit_vergi_no->text();
    cari_hesap_bilgileri.vergi_dairesi          = m_ui->combobox_vergi_dairesi->currentText();
    cari_hesap_bilgileri.adres_satiri_1         = m_ui->line_edit_adres_satiri->text();
    cari_hesap_bilgileri.adres_satiri_2         = m_ui->line_edit_adres_satiri_2->text();
    cari_hesap_bilgileri.adres_satiri_3         = m_ui->line_edit_adres_satiri_3->text();
    cari_hesap_bilgileri.ilce                   = m_ui->comboBox_ilce->currentText();
    cari_hesap_bilgileri.semt                   = m_ui->line_edit_semt->text();
    cari_hesap_bilgileri.ulke                   = m_ui->combo_box_ulke->currentText();
    cari_hesap_bilgileri.sehir                  = m_ui->combo_box_sehir->currentText();
    cari_hesap_bilgileri.posta_kodu             = m_ui->line_edit_posta_kodu->text();
    cari_hesap_bilgileri.tel_ulke_kodu          = m_ui->combobox_ev_tel_ulke_kodu->currentText();
    cari_hesap_bilgileri.tel_alan_kodu          = m_ui->combobox_ev_tel_alan_kodu->currentText();
    cari_hesap_bilgileri.telefon                = m_ui->lineedit_ev_telefonu->text().replace  ( "-", "" );
    cari_hesap_bilgileri.is_tel_ulke_kodu       = m_ui->combo_box_is_tel_ulke_kodu->currentText();
    cari_hesap_bilgileri.is_tel_alan_kodu       = m_ui->combo_box_is_tel_alan_kodu->currentText();
    cari_hesap_bilgileri.is_telefonu            = m_ui->lineedit_is_telefonu->text().replace  ( "-", "" );
    cari_hesap_bilgileri.cep_tel_ulke_kodu      = m_ui->combo_box_cep_ulke_kodu->currentText();
    cari_hesap_bilgileri.cep_tel_gsm_kodu       = m_ui->combo_box_cep_gsm_kodu->currentText();
    cari_hesap_bilgileri.cep_telefonu           = m_ui->line_edit_cep_telefonu->text().replace  ( "-", "" );
    cari_hesap_bilgileri.fax_ulke_kodu          = m_ui->combo_box_fax_ulke_kodu->currentText();
    cari_hesap_bilgileri.fax_alan_kodu          = m_ui->combo_box_fax_alan_kodu->currentText() ;
    cari_hesap_bilgileri.fax                    = m_ui->line_edit_fax->text().replace  ( "-", "" );
    cari_hesap_bilgileri.email                  = m_ui->line_edit_email->text();
    cari_hesap_bilgileri.website                = m_ui->line_edit_web_adresi->text();
    cari_hesap_bilgileri.cari_hesap_kodu        = m_ui->lineedit_cari_hesap_kodu->text();
    cari_hesap_bilgileri.muh_hesap_id           = m_muh_hesap_id;
    cari_hesap_bilgileri.para_birimi_id         = DVZ_GET_DOVIZ_ID(m_ui->comboBox_para_birimi->currentText());
    cari_hesap_bilgileri.kimlik_no              = m_ui->lineEdit_kimlik_no->text();
    cari_hesap_bilgileri.cinsiyet               = m_ui->combo_box_cinsiyet->currentText();
    cari_hesap_bilgileri.dogum_yeri             = m_ui->line_edit_dogum_yeri->text();
    cari_hesap_bilgileri.medeni_hali            = m_ui->combo_box_medeni_hali->currentText();
    cari_hesap_bilgileri.kan_grubu              = m_ui->combo_box_kan_grubu->currentText();
    cari_hesap_bilgileri.ogrenim_durumu         = m_ui->combo_box_egitim_durumu->currentText();
    cari_hesap_bilgileri.dogum_tarihi           = m_ui->adakDate_dogum_tarihi->DATE();
    cari_hesap_bilgileri.yetkili_1              = m_ui->lineEdit_yetkili_1->text();
    cari_hesap_bilgileri.yetkili_1_info         = m_ui->lineEdit_yetkili_1_info->text();
    cari_hesap_bilgileri.yetkili_2              = m_ui->lineEdit_yetkili_2->text();
    cari_hesap_bilgileri.yetkili_2_info         = m_ui->lineEdit_yetkili_2_info->text();
    cari_hesap_bilgileri.yetkili_3              = m_ui->lineEdit_yetkili_3->text();
    cari_hesap_bilgileri.yetkili_3_info         = m_ui->lineEdit_yetkili_3_info->text();

    if ( m_cari_kart_turu EQ FIRMA_KAYDI ) {
        cari_hesap_bilgileri.firma_sektoru      = m_ui->comboBox_sektor_or_meslek->currentText();
    }
    else {
        cari_hesap_bilgileri.meslek             = m_ui->comboBox_sektor_or_meslek->currentText();
    }

    if ( m_fotograf_path.isEmpty() EQ false ) {
        cari_hesap_bilgileri.fotograf_logo          =   ADAK_PIXMAP_2_BYTE_ARRAY(m_fotograf_path);
    }
    else {
        cari_hesap_bilgileri.fotograf_logo          = QByteArray();
    }

    CARI_KART_GUNCELLE     ( fis_id, m_cari_kart_turu, &cari_hesap_bilgileri );
    GRP_KAYDI_GRUPLARA_EKLE( E9_PROGRAMI,CARI_MODULU , fis_id , &m_grup_idleri);
}

/**************************************************************************************
                   CARI_KART_FORMU::CHECK_DELETE
***************************************************************************************/

int CARI_KART_FORMU::CHECK_DELETE ( int fis_id )
{
    Q_UNUSED ( fis_id );
    if ( m_modul_id NE CARI_MODULU AND m_program_id NE E9_PROGRAMI ) {
        MSG_WARNING( tr ( "Kayıt cari modulü tarafından oluşturulmamış. Kaydı oluşturan program tarafından silinebilir!.." ), NULL );
        return ADAK_FAIL;
    }

    if ( CARI_HESAP_ISLEM_GORDU_MU ( fis_id ) EQ true ) {
        MSG_WARNING( tr ( "Cari Hesap İşlem Görmüştür,Silinemez!.." ), NULL );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_KART_FORMU::DELETE_RECORD
***************************************************************************************/

void CARI_KART_FORMU::DELETE_RECORD ( int fis_id )
{
    CARI_KART_SIL( fis_id );
    GRP_KAYDIN_GRUPLARINI_SIL(E9_PROGRAMI,CARI_MODULU , fis_id);
    m_muh_hesap_id = 0;
}

/**************************************************************************************
                   CARI_KART_FORMU::SELECT_RECORD
***************************************************************************************/

int CARI_KART_FORMU::SELECT_RECORD()
{
   return OPEN_CARI_KART_ARAMA ( "", m_cari_kart_turu , this, 0, E9_PROGRAMI, CARI_MODULU );// SAHIS + FIRMA
}

/**************************************************************************************
                   CARI_KART_FORMU::FIND_RECORD
***************************************************************************************/

int CARI_KART_FORMU::FIND_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT  ( "car_hesaplar",
                                   "cari_hesap_id ,adres_id",
                                   "cari_hesap_kodu = :cari_hesap_kodu ");
    select_query.SET_VALUE       ( ":cari_hesap_kodu", m_ui->lineedit_cari_hesap_kodu->text());

    if ( select_query.SELECT() NE 0 ) {

        select_query.NEXT();

        int adres_kart_turu = ADR_GET_ADRES_KART_TURU(select_query.VALUE(1).toInt());

        if ( m_cari_kart_turu EQ adres_kart_turu ) {
            return select_query.VALUE(0).toInt();
        }

    }
    return 0;
}

/**************************************************************************************
                   CARI_KART_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int CARI_KART_FORMU::FIND_FIRST_RECORD()
{
    return   CARI_FIND_FIRST_RECORD( m_cari_kart_turu, E9_PROGRAMI, CARI_MODULU );
}

/**************************************************************************************
                   CARI_KART_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int CARI_KART_FORMU::FIND_LAST_RECORD()
{
    return   CARI_FIND_LAST_RECORD( m_cari_kart_turu, E9_PROGRAMI, CARI_MODULU );
}

/**************************************************************************************
                   CARI_KART_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int CARI_KART_FORMU::FIND_PREV_RECORD ()
{
    return CARI_FIND_PREV_RECORD( m_ui->lineedit_cari_hesap_kodu->text(), m_cari_kart_turu, E9_PROGRAMI, CARI_MODULU );
}

/**************************************************************************************
                   CARI_KART_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int CARI_KART_FORMU::FIND_NEXT_RECORD ()
{
    return CARI_FIND_NEXT_RECORD( m_ui->lineedit_cari_hesap_kodu->text(), m_cari_kart_turu, E9_PROGRAMI, CARI_MODULU );
}

/**************************************************************************************
                   CARI_KART_FORMU::LOCK_RECORD
***************************************************************************************/

int CARI_KART_FORMU::LOCK_RECORD ( int fis_id )
{
    return DB->LOCK_ROW ( "cari_hesap_id","car_hesaplar", QString ( "cari_hesap_id = %1" ).arg ( fis_id ));
}


/**************************************************************************************
                   CARI_KART_FORMU::UNLOCK_RECORD
***************************************************************************************/
void CARI_KART_FORMU::UNLOCK_RECORD ( int fis_id )
{
    DB->UNLOCK_ROW ( "cari_hesap_id","car_hesaplar", QString ( "cari_hesap_id = %1" ).arg ( fis_id ));
}


/**************************************************************************************
                   CARI_KART_FORMU::ADRES_BILGILERINI_GOSTER
***************************************************************************************/

void CARI_KART_FORMU::ADRES_BILGILERINI_GOSTER()
{

    QString adres_line_1 = m_ui->line_edit_adres_satiri->text();
    QString adres_line_2 = m_ui->line_edit_adres_satiri_2->text();
    QString adres_line_3 = m_ui->line_edit_adres_satiri_3->text();
    QString adres_line_4 = "";

    if ( m_ui->line_edit_posta_kodu->text().isEmpty() EQ false ) {
        adres_line_3 += " " + m_ui->line_edit_posta_kodu->text() + " " ;
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

    QString adres = "";

    if ( adres_line_1.isEmpty() EQ false ) {
        adres.append(adres_line_1 + "\n");
    }
    if ( adres_line_2.isEmpty() EQ false ) {
        adres.append(adres_line_2 + "\n");
    }
    if ( adres_line_3.isEmpty() EQ false ) {
        adres.append(adres_line_3 + "\n");
    }
    if ( adres_line_4.isEmpty() EQ false ) {
        adres.append(adres_line_4 );
    }

    m_ui->text_edit_adres_3->setText ( adres );
}


/**************************************************************************************
                   CARI_KART_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void CARI_KART_FORMU::SEARCH_EDIT_CLICKED ( QWidget * widget,QLineEdit * lineedit )
{

    if ( widget EQ m_ui->searchedit_muhasebe_hesabi ) {

        int muh_hesap_id = OPEN_MUH_HESAP_ARAMA( lineedit->text(), this,1 );
        if ( muh_hesap_id NE -1 ) {
            lineedit->setText ( MUH_GET_HESAP_KODU ( muh_hesap_id ) );
        }
    }

}

/**************************************************************************************
                      CARI_KART_FORMU::SAVER_BUTTON_CLICKED
***************************************************************************************/

void CARI_KART_FORMU::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_adres_id  )
{
    if ( p_button EQ m_ui->push_button_not_defteri) {
        OPEN_CARI_NOT_DEFTERI( p_adres_id, CARI_MODULU, E9_PROGRAMI, this );
    }
    else if ( p_button EQ m_ui->push_button_zarf_yazdir ) {
        OPEN_CARI_ZARF_YAZDIR ( p_adres_id, QString( tr( "Cari Zarfı" ) ), this );
    }
    else if ( p_button EQ m_ui->push_button_email ) {
        OPEN_CARI_EMAIL_FISI  ( p_adres_id,this );
    }
    else if ( p_button EQ m_ui->push_button_telefon ) {
        OPEN_CARI_TELEFON_FISI( p_adres_id,this );
    }
    else if ( p_button EQ m_ui->pushButton_cari_hareketleri ) {
        OPEN_CARI_OTO_EKSTRE_ARAMA( p_adres_id, this );
    }
//    else if ( p_button EQ m_ui->push_button_firma_yetkilileri ) {
//        OPEN_CARI_FIRMA_YETKILILERI( p_adres_id , this );
//    }
}

/**************************************************************************************
                      CARI_KART_FORMU::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int CARI_KART_FORMU::CHANGER_BUTTON_CLICKED(QAbstractButton * button)
{
    if ( button EQ m_ui->push_button_gruplar ) {
        if ( OPEN_GRUP_SECIM ( E9_PROGRAMI,CARI_MODULU , &m_grup_idleri, DB, this ) EQ true ) {
            QString grup_str = GRP_GRUP_ADLARINI_BUL ( &m_grup_idleri, new QStringList() , new QStringList() );
            m_ui->textEdit_gruplar->setPlainText( grup_str );
            return ADAK_RECORD_CHANGED;
        }
    }
    else if ( button EQ m_ui->push_button_fotograf_sec ) {
        return FOTOGRAF_SEC();
    }
    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   CARI_KART_FORMU::SET_WIDGETS_MAXIMUM_SIZE
***************************************************************************************/

void CARI_KART_FORMU::SET_WIDGETS_MAXIMUM_SIZE ()
{
    m_ui->lineedit_cari_hesap_kodu->setMaxLength ( 30 );
    m_ui->lineedit_cari_hesap_ismi->setMaxLength ( 60 );
    m_ui->line_edit_adres_satiri->setMaxLength   ( 35 );
    m_ui->line_edit_adres_satiri_2->setMaxLength ( 35 );
    m_ui->line_edit_adres_satiri_3->setMaxLength ( 35 );
    m_ui->line_edit_posta_kodu->setMaxLength     ( 10 );
    m_ui->lineedit_vergi_no->setMaxLength        ( 15 );
    m_ui->line_edit_semt->setMaxLength           ( 30 );
    m_ui->line_edit_web_adresi->setMaxLength     ( 50 );
    m_ui->line_edit_email->setMaxLength          ( 50 );
    m_ui->lineEdit_yetkili_1->setMaxLength       ( 60 );
    m_ui->lineEdit_yetkili_1_info->setMaxLength  ( 128);
    m_ui->lineEdit_yetkili_2->setMaxLength       ( 60 );
    m_ui->lineEdit_yetkili_2_info->setMaxLength  ( 128);
    m_ui->lineEdit_yetkili_3->setMaxLength       ( 60 );
    m_ui->lineEdit_yetkili_3_info->setMaxLength  ( 128);

}

/**************************************************************************************
                   CARI_KART_FORMU::SET_WIDGETS_MAXIMUM_SIZE
***************************************************************************************/

void CARI_KART_FORMU::EKRANI_DUZENLE()
{
    if ( m_cari_kart_turu EQ FIRMA_KAYDI ) {
         SET_PAGE_TITLE ( tr ( "CARİ - FİRMA CARİ KARTI" ) );

         m_ui->label_ev_telefonu_3->setHidden         ( true );
         m_ui->combobox_ev_tel_alan_kodu->setHidden   ( true );
         m_ui->combobox_ev_tel_ulke_kodu->setHidden   ( true );
         m_ui->lineedit_ev_telefonu->setHidden        ( true );
         m_ui->label_tire_ev_tel_3->setHidden         ( true );
         m_ui->label_dogum_tarihi->setHidden          ( true );
         m_ui->adakDate_dogum_tarihi->setHidden      ( true );
         m_ui->line_edit_dogum_yeri->setHidden        ( true );
         m_ui->label_dogum_yeri->setHidden            ( true );
         m_ui->label_tc_kimlik_no->setHidden          ( true );
         m_ui->lineEdit_kimlik_no->setHidden          ( true );
         m_ui->combo_box_cinsiyet->setHidden          ( true );
         m_ui->label_cinsiyet->setHidden              ( true );
         m_ui->label_kangrubu->setHidden              ( true );
         m_ui->combo_box_kan_grubu->setHidden         ( true );
         m_ui->combo_box_egitim_durumu->setHidden     ( true );
         m_ui->label_egitim_durumu->setHidden         ( true );
         m_ui->label_medeni_hali->setHidden           ( true );
         m_ui->combo_box_medeni_hali->setHidden       ( true );

         //! YETKILI BILGILERI
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
        SET_PAGE_TITLE ( tr ( "CARİ - ŞAHIS CARİ KARTI" ) );
        m_ui->label_ev_telefonu_3->setHidden         ( false );
        m_ui->combobox_ev_tel_alan_kodu->setHidden   ( false );
        m_ui->label_tire_ev_tel_3->setHidden         ( false );
        m_ui->combobox_ev_tel_ulke_kodu->setHidden   ( false );
        m_ui->lineedit_ev_telefonu->setHidden        ( false );
        m_ui->label_dogum_tarihi->setHidden          ( false );
        m_ui->adakDate_dogum_tarihi->setHidden      ( false );
        m_ui->line_edit_dogum_yeri->setHidden        ( false );
        m_ui->label_dogum_yeri->setHidden            ( false );
        m_ui->label_tc_kimlik_no->setHidden          ( false );
        m_ui->lineEdit_kimlik_no->setHidden          ( false );
        m_ui->combo_box_cinsiyet->setHidden          ( false );
        m_ui->label_cinsiyet->setHidden              ( false );
        m_ui->label_kangrubu->setHidden              ( false );
        m_ui->combo_box_kan_grubu->setHidden         ( false );
        m_ui->combo_box_egitim_durumu->setHidden     ( false );
        m_ui->label_egitim_durumu->setHidden         ( false );
        m_ui->label_medeni_hali->setHidden           ( false );
        m_ui->combo_box_medeni_hali->setHidden       ( false );

        //! YETKILI BILGILERI
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
                   CARI_KART_FORMU::FOTOGRAF_SEC()
***************************************************************************************/

int CARI_KART_FORMU::FOTOGRAF_SEC()
{
    ADAK_ADD_PIXMAP_TO_BUTTON(m_ui->push_button_fotograf_sec ,&m_fotograf_path,this);

    if ( m_fotograf_path.isEmpty() EQ false ) {
        return ADAK_RECORD_CHANGED;
    }
    return ADAK_RECORD_UNCHANGED;
}

