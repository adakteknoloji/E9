#include "prs_personel_formu_class.h"
#include "prs_personel_formu_open.h"
#include "adak_std_utils.h"
#include "dvz_gui_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
#include "e9_log.h"
#include "e9_enum.h"
#include "cari_struct.h"
#include "adak_gruplar.h"
#include "sube_console_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "muh_console_utils.h"
#include "e9_console_utils.h"
#include "dmr_con_utils.h"
#include "e9_gui_utils.h"
#include "cari_kart_arama_open.h"
#include "muh_hesap_arama_open.h"
#include "prs_personel_aile_uyeleri_open.h"
#include "prs_personel_kesinti_ekleme_fisi_open.h"
#include "prs_personel_odenek_ekleme_fisi_open.h"
#include "sube_secimi_open.h"
#include "cari_console_utils.h"
#include "cari_gui_utils.h"
#include "cari_enum.h"
#include "prs_enum.h"
#include "prs_gui_utils.h"
#include "prs_con_utils.h"
#include "prs_struct.h"


extern ADAK_SQL     *       DB;
extern PRS_FIRMA_ODENEKLERI  G_PRS_FRM_ODENEKLERI;

ISTEN_CIKMA_NEDENLERI    ICN;
EKSIK_GUN_NEDENLERI      EGN;

#define    TANIMLANACAK_ENT_HESAP_SAYISI    2

/********************************************************/
/*              SHOW_PRS_PERSONEL_FORMU                 */
/********************************************************/

void SHOW_PRS_PERSONEL_FORMU ( int personel_id, QWidget *parent )
{
    PRS_PERSONEL_FORMU * F = new PRS_PERSONEL_FORMU ( personel_id, parent );
    F->SHOW( FULL_SCREEN );
}

/***********************************************************/
/*          PRS_PERSONEL_FORMU::PRS_PERSONEL_FORMU         */
/***********************************************************/

PRS_PERSONEL_FORMU::PRS_PERSONEL_FORMU ( int personel_id, QWidget *parent ) : FORM_KERNEL( parent ), m_ui( new Ui::PRS_PERSONEL_FORMU )
{
    m_ui->setupUi(this);

    m_personel_id = personel_id;

    START_FORM_KERNEL(this, DB);

}

/********************************************************/
/*          PRS_PERSONEL_FORMU::CLEAR_FORM_MEMBERS      */
/********************************************************/

void PRS_PERSONEL_FORMU::SETUP_FORM()
{

    SET_HELP_PAGE           ( "personel-bordrosu_personel-sicil-karti" );
    SET_PAGE_TITLE          ( tr ( "PRS - PERSONEL KAYIT FORMU" ) );

    REGISTER_BUTTONS_WIDGET( m_ui->widget_navigation_buttons );

    SET_SETTING_NAME        ( "PRS_PERSONEL_FORMU" );

    m_ui->adakDate_dogum_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_ise_giris_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_isten_cikis_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_ssk_basv_tarihi->MALI_YIL_KISITI_YOK( true );\
    m_ui->adakDate_verilis_tarihi->MALI_YIL_KISITI_YOK( true );

    SET_WIDGETS_MAXIMUM_SIZE      ();
    m_ui->limitedTextEdit_adres->SET_MAX_STRING_LENGTH(512);
    m_ui->limitedTextEdit_notlar->SET_MAX_STRING_LENGTH(2048);

    WIDGETLARI_DUZENLE();

   // m_ui->lineEdit_ucret_sekli->setText("AYLIK");
    //DISABLE_CLEAR_ON_WIDGET( m_ui->lineEdit_ucret_sekli);

    REGISTER_SAVER_BUTTON(m_ui->push_button_aile_bireyleri);

    REGISTER_SAVER_BUTTON(m_ui->push_button_ek_kesinti_ekle);

    REGISTER_SAVER_BUTTON(m_ui->push_button_ek_odenek_ekle);
    REGISTER_SAVER_BUTTON( m_ui->push_button_not_defteri );
    REGISTER_CHANGER_BUTTON ( m_ui->pushButton_grup_ekle );

    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_calisma_sekli );
    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_calisma_turleri );
    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_indirim_kanunlari );

    PRS_FILL_COMBO_BOX_CALISMA_TURLERI( m_ui->comboBox_calisma_turleri );
    PRS_FILL_COMBO_BOX_INDIRIM_KANUNLARI ( m_ui->comboBox_indirim_kanunlari );

    REGISTER_CHANGER_BUTTON ( m_ui->push_button_resim  );

    PRS_FILL_COMBO_BOX_CALISMA_SEKLI( m_ui->comboBox_calisma_sekli );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_calisma_sekli, tr("AYLIK"));

    m_ui->line_edit_cilt_no->setValidator       ( new QRegExpValidator( QRegExp("\\d{0,5}"), this) );
    m_ui->line_edit_aile_sira_no->setValidator  ( new QRegExpValidator( QRegExp("\\d{0,5}"), this) );
    m_ui->line_edit_sira_no->setValidator       ( new QRegExpValidator( QRegExp("\\d{0,5}"), this) );
    m_ui->line_edit_kayit_no->setValidator      ( new QRegExpValidator( QRegExp("\\d{0,5}"), this) );
    m_ui->line_edit_ev_telefonu->setValidator   ( new QRegExpValidator( QRegExp("\\d{0,7}"), this) );
    m_ui->line_edit_cep_telefonu->setValidator  ( new QRegExpValidator( QRegExp("\\d{0,7}"), this) );
    m_ui->line_edit_cep_telefonu->setValidator  ( new QRegExpValidator( QRegExp("\\d{0,7}"), this) );

    SET_FIRST_FOCUS_WIDGET ( m_ui->line_edit_sicil_no );

    GET_ULKELER           ( m_ui->combo_box_ulke                );
    GET_ILLER             ( m_ui->combo_box_il                  );
    GET_ILLER             ( m_ui->combo_box_kayitli_oldugu_il    );
    GET_ILLER             ( m_ui->combo_box_dogum_yeri          );

   // GET_ILCELER                 ( m_ui->combo_box_kayitli_oldugu_ilce );
    GET_CINSIYETLER               ( m_ui->combo_box_cinsiyet            );
    GET_OGRENIM_DURUMLARI         ( m_ui->combo_box_ogrenim_durumu      );
    GET_KAN_GRUPLARI              ( m_ui->combo_box_kan_grubu           );
    GET_MEDENI_HALLER             ( m_ui->combo_box_medeni_hal          );
    GET_MESLEKLER                 ( m_ui->combo_box_meslek      );
    GET_UNVANLAR                  ( m_ui->combo_box_unvan       );
    GET_UYRUKLAR                  ( m_ui->combo_box_uyruk );
    PRS_FILL_COMBO_BOX_MEDENI_HAL     (m_ui->combo_box_medeni_hal);
    PRS_FILL_COMBO_BOX_YABANCI_DILLER (m_ui->combo_box_yabanci_dil_1 ,m_ui->combo_box_yabanci_dil_2);
    DVZ_FILL_COMBOBOX             ( m_ui->combo_box_doviz  );
    PRS_FILL_COMBO_BOX_SENDIKA        (m_ui->combo_box_sendika_uyesi_mi);
    //FILL_COMBO_BOX_UCRET_SEKLI    (m_ui->combo_box_ucret_tipi);
    PRS_FILL_COMBO_BOX_UCRET_TIPI      (m_ui->combo_box_ucret_sekli);
    FIND_AND_SET_COMBOBOX_TEXT     (m_ui->combo_box_ucret_sekli,  "BRÜT" );
    FIND_AND_SET_COMBOBOX_TEXT     ( m_ui->combo_box_doviz,  DVZ_GET_TEMEL_PARA_BIRIMI_KODU());

    PRS_FILL_COMBO_BOX_ISTEN_CIKMA_NEDENLERI ( m_ui->combo_box_isten_cikis_kodu );

    m_doviz_id = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

    m_ui->lineEdit_para_birimi->setMaxLength( 25 );


    GET_ULKE_TELEFON_KODLARI ( m_ui->combo_box_ev_telefonunun_ulke_kodlari  );
    GET_ULKE_TELEFON_KODLARI ( m_ui->combo_box_is_telefonunun_ulke_kodlari  );
    GET_ULKE_TELEFON_KODLARI ( m_ui->combo_box_cep_telefonunun_ulke_kodlari );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_cep_telefonunun_ulke_kodlari, "90" );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_telefonunun_ulke_kodlari,  "90" );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_ev_telefonunun_ulke_kodlari,  "90" );

    GET_ALAN_KODLARI(m_ui->combo_box_ev_telefonunun_il_kodlari);
    GET_ALAN_KODLARI(m_ui->combo_box_is_telefonunun_il_kodlari);

    GET_GSM_KODLARI ( m_ui->combo_box_gsm_kodlari );

    m_ui->combo_box_ulke->setEditText( tr ( "Türkiye" ) );

    M_ENT_HESAPLARI_ID_ARRAY[0] = 0;
    M_ENT_HESAPLARI_ID_ARRAY[1] = 0;

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_personel_avanslari_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_personel_avanslari_hesap_ismi);

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_personele_borclar_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_personele_borclar_hesap_ismi);


    QString prs_avanlari_hesap_kodu, prs_avanslari_hesap_adi;

    M_ENT_HESAPLARI_ID_ARRAY[0] = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "prs_avanslari_ent_hesap_id", &prs_avanlari_hesap_kodu, &prs_avanslari_hesap_adi );

    m_ui->searchEdit_personel_avanslari_hesap_kodu->SET_TEXT    ( prs_avanlari_hesap_kodu );
    m_ui->lineEdit_personel_avanslari_hesap_ismi->setText       ( prs_avanslari_hesap_adi );

    QString prs_borclari_hesap_kodu, prs_borclari_hesap_adi;

    M_ENT_HESAPLARI_ID_ARRAY[1] = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "personel_borclari_ent_hesap_id", &prs_borclari_hesap_kodu, &prs_borclari_hesap_adi );

    m_ui->searchEdit_personele_borclar_hesap_kodu->SET_TEXT ( prs_avanlari_hesap_kodu );
    m_ui->lineEdit_personele_borclar_hesap_ismi->setText    ( prs_avanslari_hesap_adi );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->searchEdit_personele_borclar_hesap_kodu->setVisible( false );
        m_ui->searchEdit_personel_avanslari_hesap_kodu->setVisible( false );

        m_ui->label_personele_borclar->setVisible( false );
        m_ui->label_personel_avanslari->setVisible( false );

        m_ui->lineEdit_personele_borclar_hesap_ismi->setVisible( false );
        m_ui->lineEdit_personel_avanslari_hesap_ismi->setVisible( false );

        m_ui->label_ent_label->setVisible( false );
    }

    m_ui->comma_edit_net_ucret->clear();

    AUTOSTART_FORM_KERNEL ( m_personel_id );
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::CLEAR_FORM_MEMBERS      */
/********************************************************/

void PRS_PERSONEL_FORMU::CLEAR_FORM_MEMBERS()
{
    prs_grup_idleri.clear();
    m_fotograf_path.clear();
    m_sube_id = 0 ;


}

/********************************************************/
/*          PRS_PERSONEL_FORMU::NEW_RECORD              */
/********************************************************/

void PRS_PERSONEL_FORMU::NEW_RECORD ()
{
    QString       sigorta_turu;
    m_ui->lineEdit_para_birimi->setEnabled(false);
    m_ui->lineEdit_para_birimi_1->setEnabled(false);
    m_ui->lineEdit_para_birimi_2->setEnabled(false);

    m_ui->comma_edit_net_ucret->setEnabled( false );

    QString sube_kodu, sube_adi;

    int result = SUBE_GET_SUBE_KODU_ADI( 1, &sube_kodu, &sube_adi );

    if( result EQ 0 ) {
        m_sube_id = 0 ;
    }
    if ( result EQ 1 ) {
        m_ui->searchEdit_sube->SET_TEXT ( sube_kodu );
        m_ui->lineEdit_sube_adi->setText( sube_adi  );
        m_sube_id = 1;
    }

    m_ui->groupBox_isten_cikis_bilgileri->setVisible(false);

    PRS_FILL_FIRMA_ODENEKLERI_STRUCT();
    PRS_FILL_SABIT_DEGERLER_STRUCT  ();

    sigorta_turu = PRS_GET_FIRMA_SIGORTA_TURU ();

    m_ui->label_tabi_oldugu_sigorta_kolu->setText( "<font color = darkred> <font style=\"font-size:12pt;\"><b>" + sigorta_turu +"</b>");


    m_ui->label_esi_calisiyor_mu->setEnabled        ( false );
    m_ui->check_box_esi_calisiyor_mu->setChecked    ( false );
    m_ui->check_box_esi_calisiyor_mu->setEnabled    ( false );
    m_ui->check_box_esi_calisiyor_mu->setChecked    ( false );

    PRS_FILL_COMBO_BOX_OZEL_DURUMLAR ( m_ui->combo_box_ozel_durum );

    EVLI_DEGILSE_ES_BILGISINI_DISABLE_ET ();

    if ( PRS_FIRMA_SENDIKALI_MI() EQ ADAK_OK ) {
        m_ui->combo_box_sendika_uyesi_mi->setCurrentIndex(0);
    }

    m_ui->push_button_resim->setText ( tr ( "Fotoğraf Seç" ) );
    m_ui->push_button_resim->setIcon ( QIcon() );

    m_ui->adakDate_ise_giris_tarihi->SET_DATE( QDate::currentDate() );
    m_ui->adakDate_isten_cikis_tarihi->SET_DATE( QDate::currentDate() );
    //m_ui->date_edit_muracaat_tarihi->setDate    ( QDate::currentDate() );
    m_ui->adakDate_ssk_basv_tarihi->SET_DATE( QDate::currentDate() );
    m_ui->adakDate_dogum_tarihi->SET_DATE( QDate::currentDate() );
    m_ui->adakDate_verilis_tarihi->SET_DATE( QDate::currentDate() );

    FIND_AND_SET_COMBOBOX_TEXT  ( m_ui->combo_box_cep_telefonunun_ulke_kodlari, "90" );
    FIND_AND_SET_COMBOBOX_TEXT  ( m_ui->combo_box_is_telefonunun_ulke_kodlari,  "90" );
    FIND_AND_SET_COMBOBOX_TEXT  ( m_ui->combo_box_ev_telefonunun_ulke_kodlari,  "90" );
    FIND_AND_SET_COMBOBOX_TEXT  (m_ui->combo_box_ucret_sekli ,"BRÜT");
    //FIND_AND_SET_COMBOBOX_TEXT  (m_ui->combo_box_ucret_tipi ,"AYLIK");
    FIND_AND_SET_COMBOBOX_TEXT  ( m_ui->combo_box_doviz,  "TL");

    m_ui->groupBox_isten_cikis_bilgileri->setEnabled(false);


    m_ui->lineEdit_para_birimi->setText( m_ui->combo_box_doviz->currentText());
    m_ui->lineEdit_para_birimi_1->setText( m_ui->combo_box_doviz->currentText());
    m_ui->lineEdit_para_birimi_2->setText( m_ui->combo_box_doviz->currentText());
    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.AILE_YARDIMI.var_mi_durumu, m_ui->check_box_aile_yardimi       );
    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI.var_mi_durumu,m_ui->check_box_cocuk_yardimi     );
    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI.var_mi_durumu,m_ui->check_box_dogum_yardimi     );
    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI.var_mi_durumu, m_ui->check_box_evlenme_yardimi );
    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI.var_mi_durumu, m_ui->check_box_gorev_harcirahi );
    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.IHBAR_TAZMINATI.var_mi_durumu,m_ui->check_box_ihbar_tazminati );
    m_ui->check_box_ihbar_tazminati->setEnabled(false);

    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.KIDEM_TAZMINATI.var_mi_durumu, m_ui->check_box_kidem_tazminati );
    m_ui->check_box_kidem_tazminati->setEnabled(false);

    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI.var_mi_durumu, m_ui->check_box_olum_yardimi       );
    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI.var_mi_durumu, m_ui->check_box_yakacak_yardimi );
    FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.var_mi_durumu,m_ui->check_box_yemek_parasi       );
    m_ui->tab_widget->setCurrentIndex(0);

    SQL_QUERY query( DB );

    query.PREPARE_SELECT("prs_sabit_degerler", "haftalik_calisilan_gun_sayisi" );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        m_ui->commaEdit_haftalik_calisilan_gun_sayisi->SET_DOUBLE( query.VALUE(0).toDouble() );
    }

    m_ui->commaEdit_gunluk_yemek_parasi_tutari->SET_DOUBLE( G_PRS_FRM_ODENEKLERI.YEMEK_PARASI.brut_tutari );

}

/********************************************************/
/*          PRS_PERSONEL_FORMU::GET_RECORD              */
/********************************************************/

int PRS_PERSONEL_FORMU::GET_RECORD ( int record_id )
{

    SQL_QUERY       sql_query        ( DB );

    QPalette        palette;
    palette.setColor(QPalette::Text, Qt::darkRed);

    int prs_cari_hesap_id = 0;

    m_personel_id  = record_id;

 /**********************************************************************
                            PERSONEL
 **********************************************************************/


    QString ind_kanunu_kodu, calisma_turu_kodu;

    sql_query.PREPARE_SELECT("prs_personeller", "sicil_no, cari_hesap_id,"
                             "sube_id , "
                             "prs_avanslar_hesap_id                 ,"
                             "prs_borclar_hesap_id                  ,"
                             "sigorta_no                            ,"
                             "emekli_mi                             ,"
                             "ucret_sekli                           ,"
                             "brut_ucret                            ,"
                             "net_ucret                             ,"
                             "kumulatif_vergi_matrahi               ,"
                             "isten_ayrildi_mi, calisma_sekli       ,"
                             "ind_kanunu_kodu, calisma_turu_kodu    ,"
                             "dogum_yardimi_var_mi, olum_yardimi_var_mi, "
                             "cocuk_yardimi_var_mi, yakacak_yardimi_var_mi, "
                             "aile_yardimi_var_mi, yemek_parasi_var_mi, "
                             "evlenme_yardimi_var_mi, gorev_harcirahi_var_mi, "
                             "ihbar_tazminati_var_mi, kidem_tazminati_var_mi ,"
                             "haftalik_calisilan_gun_sayisi ,       "
                             "gunluk_yemek_parasi_tutari ,"
                             "bir_ay_once_kalan_ek_odenek, iki_ay_once_kalan_ek_odenek",
                             "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", record_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        m_ui->commaEdit_bir_ay_once_kalan_odenek->SET_DOUBLE( sql_query.VALUE( "bir_ay_once_kalan_ek_odenek" ).toDouble() );
        m_ui->commaEdit_iki_ay_once_kalan_odenek->SET_DOUBLE( sql_query.VALUE( "iki_ay_once_kalan_ek_odenek" ).toDouble() );

        old_personel_sicil_kodu             = sql_query.VALUE( "sicil_no").toString();
        ind_kanunu_kodu                     = sql_query.VALUE( "ind_kanunu_kodu").toString();
        calisma_turu_kodu                   = sql_query.VALUE( "calisma_turu_kodu").toString();

        m_ui->comboBox_calisma_turleri->setCurrentIndex( m_ui->comboBox_calisma_turleri->findText( calisma_turu_kodu , Qt::MatchContains ) );
        m_ui->comboBox_indirim_kanunlari->setCurrentIndex( m_ui->comboBox_indirim_kanunlari->findText( ind_kanunu_kodu , Qt::MatchContains ) );

        m_ui->line_edit_sicil_no->setText   (sql_query.VALUE(0).toString());
        old_personel_sicil_kodu             = sql_query.VALUE( 0 ).toString();
        prs_cari_hesap_id                   = sql_query.VALUE( 1 ).toInt();
        m_ui->line_edit_sicil_no->setPalette(palette);
        m_sube_id                           = sql_query.VALUE(2).toInt();
        m_doviz_id                          = CARI_GET_PARA_BIRIM_ID(prs_cari_hesap_id );
        M_ENT_HESAPLARI_ID_ARRAY[0]         = sql_query.VALUE("prs_avanslar_hesap_id").toInt();
        M_ENT_HESAPLARI_ID_ARRAY[1]         = sql_query.VALUE("prs_borclar_hesap_id").toInt();

        m_ui->line_edit_sigorta_no->setText( sql_query.VALUE( "sigorta_no" ).toString() );
        int emekli_mi  = sql_query.VALUE( "emekli_mi" ).toInt();

        m_ui->checkBox_emekli_mi->setChecked( false );
        if ( emekli_mi EQ 1 ) {
            m_ui->checkBox_emekli_mi->setChecked( true );
        }

        m_ui->commaEdit_haftalik_calisilan_gun_sayisi->SET_DOUBLE( sql_query.VALUE( "haftalik_calisilan_gun_sayisi" ).toDouble() );
        m_ui->commaEdit_gunluk_yemek_parasi_tutari->SET_DOUBLE( sql_query.VALUE( "gunluk_yemek_parasi_tutari" ).toDouble() );

        m_ui->combo_box_ucret_sekli->setCurrentIndex  ( sql_query.VALUE("ucret_sekli").toInt() );

        if ( sql_query.VALUE("ucret_sekli").toInt() EQ PRS_NET ) {

            m_ui->comma_edit_net_ucret->setEnabled( true );

            m_ui->comma_edit_brut_ucret->clear();
            m_ui->comma_edit_brut_ucret->setEnabled( false );
        }
        else {
            m_ui->comma_edit_brut_ucret->setEnabled( true );

            m_ui->comma_edit_net_ucret->clear();
            m_ui->comma_edit_net_ucret->setEnabled( false );
        }

        m_ui->comma_edit_brut_ucret->SET_DOUBLE       ( sql_query.VALUE("brut_ucret").toDouble() );
        m_ui->comma_edit_net_ucret->SET_DOUBLE        ( sql_query.VALUE("net_ucret").toDouble() );
        m_ui->comma_edit_kum_vergi_matrahi->SET_DOUBLE( sql_query.VALUE("kumulatif_vergi_matrahi").toDouble());

        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_calisma_sekli, PRS_GET_CALISMA_SEKLI_STRING( sql_query.VALUE( "calisma_sekli" ).toInt()));


        /********************************************************/
        //                 PERSONEL ODENEKLERI
        /********************************************************/

        m_ui->check_box_dogum_yardimi->setChecked    ( sql_query.VALUE("dogum_yardimi_var_mi").toBool());
        m_ui->check_box_olum_yardimi->setChecked     ( sql_query.VALUE("olum_yardimi_var_mi").toBool());
        m_ui->check_box_cocuk_yardimi->setChecked    ( sql_query.VALUE("cocuk_yardimi_var_mi").toBool());
        m_ui->check_box_yakacak_yardimi->setChecked  ( sql_query.VALUE("yakacak_yardimi_var_mi").toBool());
        m_ui->check_box_aile_yardimi->setChecked     ( sql_query.VALUE("aile_yardimi_var_mi").toBool());
        m_ui->check_box_yemek_parasi->setChecked     ( sql_query.VALUE("yemek_parasi_var_mi").toBool());
        m_ui->check_box_evlenme_yardimi->setChecked  ( sql_query.VALUE("evlenme_yardimi_var_mi").toBool());
        m_ui->check_box_gorev_harcirahi->setChecked  ( sql_query.VALUE("gorev_harcirahi_var_mi").toBool());
        m_ui->check_box_ihbar_tazminati->setChecked  ( sql_query.VALUE("ihbar_tazminati_var_mi").toBool());
        m_ui->check_box_kidem_tazminati->setChecked  ( sql_query.VALUE("kidem_tazminati_var_mi").toBool());
    }
    FIND_AND_SET_COMBOBOX_TEXT  ( m_ui->combo_box_doviz,  "TL");

    QString sube_kodu, sube_adi;

    SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu, &sube_adi );

    m_ui->searchEdit_sube->SET_TEXT ( sube_kodu );
    m_ui->lineEdit_sube_adi->setText( sube_adi  );

    GRP_KAYDIN_GRUPLARINI_BUL( E9_PROGRAMI, PERSONEL_MODULU, record_id, &prs_grup_idleri, new QStringList(), new QStringList() );

    m_ui->textEdit_gruplar->setText ( GRP_GRUP_ADLARINI_BUL( &prs_grup_idleri , new QStringList() , new QStringList() ) );

    BORC_BILGISI borc_bilgisi;

    CARI_KART_BORC_BILGISINI_GETIR( PRS_GET_CARI_HESAP_ID( record_id ), &borc_bilgisi );

    for ( int i = 0; i < TANIMLANACAK_ENT_HESAP_SAYISI; i++ ) {
        M_SEARCH_EDIT_ENT_ARRAY[i]->SET_TEXT( MUH_GET_HESAP_KODU( M_ENT_HESAPLARI_ID_ARRAY[ i]));
        M_LINE_EDIT_ENT_ARRAY[i]->setText( MUH_GET_HESAP_ISMI( MUH_GET_HESAP_KODU( M_ENT_HESAPLARI_ID_ARRAY[i])));
    }

    int isten_ayrildi_mi = sql_query.VALUE( "isten_ayrildi_mi" ).toInt();

    /********************************************************/
    //                 PERSONEL KISISEL BILGILERI
    /********************************************************/

    sql_query.PREPARE_SELECT("prs_personel_kisisel_blgler"," personel_id, "
                             "uyruk, "
                             "departman, ozel_durum_enum, meslek, unvan, "
                             "ogrenim_durumu, mezun_oldugu_okul, mezun_oldugu_bolum, "
                             "yabanci_dil_1, yabanci_dil_2, muracaat_tarihi, "
                             "ise_giris_tarihi, ssk_basvuru_tarihi, "
                             "ozel_indirim_dilekcesi, "
                             "sendika_uyesi_mi, resim ,kisisel_bilgi_id  ",
                             "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", record_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        m_ui->line_edit_departman->setText                  ( sql_query.VALUE("departman").toString()  );

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->combo_box_ozel_durum ,
                                   PRS_GET_OZEL_DURUM_STRING(sql_query.VALUE("ozel_durum_enum").toInt()));

        m_ui->combo_box_unvan->setCurrentIndex              ( m_ui->combo_box_unvan->findText( sql_query.VALUE("unvan").toString() ) );
        m_ui->combo_box_meslek->setCurrentIndex             ( m_ui->combo_box_meslek->findText( sql_query.VALUE("meslek").toString() ) );
        m_ui->combo_box_uyruk->setCurrentIndex              ( m_ui->combo_box_uyruk->findText( sql_query.VALUE("uyruk").toString() ) );
        m_ui->combo_box_ogrenim_durumu->setCurrentIndex     ( m_ui->combo_box_ogrenim_durumu->findText( sql_query.VALUE("ogrenim_durumu").toString() ) );
        m_ui->line_edit_muzun_oldugu_okul->setText          ( sql_query.VALUE("mezun_oldugu_okul").toString());
        m_ui->combo_box_mezun_oldugu_bolum->setCurrentIndex ( m_ui->combo_box_mezun_oldugu_bolum->findText( sql_query.VALUE("mezun_oldugu_bolum").toString() ) );
        m_ui->combo_box_yabanci_dil_1->setCurrentIndex      ( m_ui->combo_box_yabanci_dil_1->findText( sql_query.VALUE("yabanci_dil_1").toString() ) );
        m_ui->combo_box_yabanci_dil_2->setCurrentIndex      ( m_ui->combo_box_yabanci_dil_2->findText( sql_query.VALUE("yabanci_dil_2").toString() ) );
        m_ui->combo_box_sendika_uyesi_mi->setCurrentIndex   ( sql_query.VALUE("sendika_uyesi_mi").toInt() );
        m_ui->adakDate_ise_giris_tarihi->SET_DATE(  sql_query.VALUE("ise_giris_tarihi").toString() );



        m_ui->groupBox_isten_cikis_bilgileri->setVisible(false);

        if ( isten_ayrildi_mi EQ 1 ) {

           m_ui->groupBox_isten_cikis_bilgileri->setVisible(true);
           m_ui->groupBox_isten_cikis_bilgileri->setEnabled( false );
        }

        SQL_QUERY query( DB );
        query.PREPARE_SELECT( "prs_isten_ayrilanlar", "isten_ayrilma_sekli_text, isten_ayrilma_tarihi ","personel_id = :personel_id" );
        query.SET_VALUE     ( ":personel_id", m_personel_id );

        QString isten_ayrilma_nedeni = "";
        QString isten_ayrilma_tarihi = "";

        if ( query.SELECT() NE 0 ) {
            query.NEXT();

            isten_ayrilma_nedeni = query.VALUE( 0 ).toString();
            isten_ayrilma_tarihi = query.VALUE( 1 ).toString();

        }

        FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_isten_cikis_kodu, isten_ayrilma_nedeni );

        m_ui->adakDate_isten_cikis_tarihi->SET_DATE(isten_ayrilma_tarihi );


        //m_ui->date_edit_muracaat_tarihi->setDate(
                //QDate::fromString( sql_query.VALUE("muracaat_tarihi").toString(), "yyyy.MM.dd") );

        m_ui->adakDate_ssk_basv_tarihi->SET_DATE(
                ( sql_query.VALUE("ssk_basvuru_tarihi").toString()) );

        m_ui->line_edit_ozel_indirim_dilekcesi->setText(  sql_query.VALUE("ozel_indirim_dilekcesi").toString() );


        if (sql_query.VALUE ( "resim" ).toString().isEmpty() EQ false) {
            int w = m_ui->push_button_resim->width();
            int h = m_ui->push_button_resim->height();

            m_ui->push_button_resim->setText ( "" );
            m_ui->push_button_resim->setIcon ( QIcon(ADAK_BYTE_ARRAY_2_PIXMAP(sql_query.VALUE ( "resim" ).toByteArray())) );
            m_ui->push_button_resim->setIconSize   ( QSize(w-4,h-4) );
            m_ui->push_button_resim->setMinimumSize( QSize(w,h) );
            m_ui->push_button_resim->setMaximumSize( QSize(w,h) );
        }
        else {
            m_ui->push_button_resim->setText("Fotoğraf Seç");
            m_ui->push_button_resim->setIcon(QIcon( "" ) );
        }
        m_fotograf_path = "";


    }

         /********************************************************
                       PERSONEL NUFUS BILGILERI
         ********************************************************/

    sql_query.PREPARE_SELECT("prs_personel_nufus_blgler","personel_id, nufus_cuzdani_seri_no, "
                             "baba_adi, anne_adi, "
                             "dini, ilk_soyadi, "
                             "kayitli_oldugu_il, kayitli_oldugu_ilce, kayitli_oldugu_mahalle, "
                             "cilt_no, aile_sira_no, sira_no, "
                             "verildigi_yer, verilis_nedeni, kayit_no, verilis_tarihi ",
                             "personel_id = :personel_id");

    sql_query.SET_VALUE( ":personel_id", record_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        m_ui->line_edit_seri_no->setText        ( sql_query.VALUE("nufus_cuzdani_seri_no").toString() );
        m_ui->line_edit_personel_adi->setPalette( palette );
        m_ui->line_edit_anne_adi->setText       ( sql_query.VALUE("anne_adi").toString() );
        m_ui->line_edit_baba_adi->setText       ( sql_query.VALUE("baba_adi").toString() );

        m_ui->line_edit_din->setText( sql_query.VALUE("dini").toString() );

        m_ui->combo_box_kayitli_oldugu_il->setCurrentIndex(
                m_ui->combo_box_kayitli_oldugu_il->findText( sql_query.VALUE("kayitli_oldugu_il").toString() ) );

        m_ui->combo_box_kayitli_oldugu_ilce->setCurrentIndex(
                m_ui->combo_box_kayitli_oldugu_ilce->findText( sql_query.VALUE("kayitli_oldugu_ilce").toString() ) );

        m_ui->line_edit_kayitli_oldugu_mahalle->setText ( sql_query.VALUE("kayitli_oldugu_mahalle").toString() );
        m_ui->line_edit_cilt_no->setText                ( sql_query.VALUE("cilt_no").toString() );
        m_ui->line_edit_aile_sira_no->setText           ( sql_query.VALUE("aile_sira_no").toString() );
        m_ui->line_edit_sira_no->setText                ( sql_query.VALUE("sira_no").toString() );
        m_ui->line_edit_kayit_no->setText               ( sql_query.VALUE("kayit_no").toString() );
        m_ui->line_edit_verildigi_yer->setText          ( sql_query.VALUE("verildigi_yer").toString() );
        m_ui->line_edit_verilis_nedeni->setText         ( sql_query.VALUE("verilis_nedeni").toString() );

        m_ui->adakDate_verilis_tarihi->SET_DATE(
                 sql_query.VALUE("verilis_tarihi").toString());

        m_ui->line_edit_ilk_soyadi->setText( sql_query.VALUE("ilk_soyadi").toString() );

        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_kayitli_oldugu_il, sql_query.VALUE("kayitli_oldugu_il").toString()    );

        m_ui->combo_box_ilce->clear();

        GET_ILCELER ( m_ui->combo_box_kayitli_oldugu_ilce , m_ui->combo_box_kayitli_oldugu_il->currentText() );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_kayitli_oldugu_ilce , sql_query.VALUE("kayitli_oldugu_ilce").toString());

        EVLI_DEGILSE_ES_BILGISINI_DISABLE_ET();
    }




       /********************************************************
                  PERSONEL ADRES BILGILERI
        ********************************************************/

    CARI_HESAP_STRUCT hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( prs_cari_hesap_id, &hesap_bilgileri  );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_medeni_hal, hesap_bilgileri.medeni_hali );

     m_ui->line_edit_tc_kimlik_no->setText   ( hesap_bilgileri.kimlik_no );
     m_ui->adakDate_dogum_tarihi->SET_DATE(
            ( hesap_bilgileri.dogum_tarihi));
     m_ui->combo_box_dogum_yeri->setCurrentIndex(
         m_ui->combo_box_dogum_yeri->findText( hesap_bilgileri.dogum_yeri ) );

     FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_cinsiyet, hesap_bilgileri.cinsiyet );

     FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_kan_grubu, hesap_bilgileri.kan_grubu );

     m_ui->combo_box_medeni_hal->setCurrentIndex(
            m_ui->combo_box_medeni_hal->findText( hesap_bilgileri.medeni_hali ));

    m_ui->line_edit_personel_adi->setText   ( hesap_bilgileri.cari_hesap_ismi );

    m_ui->combo_box_ev_telefonunun_ulke_kodlari->clear();
    m_ui->combo_box_ev_telefonunun_ulke_kodlari->addItems( m_ulke_tel_kodlari );
    m_ui->combo_box_ev_telefonunun_ulke_kodlari->setCurrentIndex(
                                        m_ui->combo_box_ev_telefonunun_ulke_kodlari->findText( hesap_bilgileri.tel_ulke_kodu));

    GET_ALAN_KODLARI(m_ui->combo_box_ev_telefonunun_il_kodlari,hesap_bilgileri.tel_ulke_kodu);
    m_ui->combo_box_ev_telefonunun_il_kodlari->setCurrentIndex( m_ui->combo_box_ev_telefonunun_il_kodlari->findText( hesap_bilgileri.tel_alan_kodu ));
    m_ui->line_edit_ev_telefonu->setText( hesap_bilgileri.telefon );

    m_ui->combo_box_is_telefonunun_ulke_kodlari->clear();
    m_ui->combo_box_is_telefonunun_ulke_kodlari->addItems( m_ulke_tel_kodlari );
    m_ui->combo_box_is_telefonunun_ulke_kodlari->setCurrentIndex(
            m_ui->combo_box_is_telefonunun_ulke_kodlari->findText( hesap_bilgileri.is_tel_ulke_kodu ));

    GET_ALAN_KODLARI(m_ui->combo_box_is_telefonunun_il_kodlari,hesap_bilgileri.is_tel_ulke_kodu);

    m_ui->combo_box_is_telefonunun_il_kodlari->setCurrentIndex(
            m_ui->combo_box_is_telefonunun_il_kodlari->findText( hesap_bilgileri.is_tel_alan_kodu ));
    m_ui->line_edit_is_telefonu->setText( hesap_bilgileri.is_telefonu );

    m_ui->combo_box_cep_telefonunun_ulke_kodlari->clear();
    m_ui->combo_box_cep_telefonunun_ulke_kodlari->addItems( m_ulke_tel_kodlari );
    m_ui->combo_box_cep_telefonunun_ulke_kodlari->setCurrentIndex(
            m_ui->combo_box_cep_telefonunun_ulke_kodlari->findText( hesap_bilgileri.cep_tel_ulke_kodu ));

    GET_GSM_KODLARI ( m_ui->combo_box_gsm_kodlari , hesap_bilgileri.cep_tel_ulke_kodu );
    m_ui->combo_box_gsm_kodlari->setCurrentIndex(
            m_ui->combo_box_gsm_kodlari->findText( hesap_bilgileri.cep_tel_gsm_kodu ));

    m_ui->line_edit_cep_telefonu->setText( hesap_bilgileri.cep_telefonu );

    m_ui->line_edit_e_mail->setText         ( hesap_bilgileri.email );
    m_ui->line_edit_adres_satiri_1->setText          ( hesap_bilgileri.adres_satiri_1 );
    m_ui->line_edit_adres_satiri_2->setText          ( hesap_bilgileri.adres_satiri_2 );
    m_ui->line_edit_adres_satiri_3->setText          ( hesap_bilgileri.adres_satiri_3 );

    m_ui->line_edit_semt->setText           ( hesap_bilgileri.semt );

    //sehir bilgileri
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_ulke , hesap_bilgileri.ulke);

    m_ui->combo_box_il->clear();

    GET_ILLER                  ( m_ui->combo_box_il ,m_ui->combo_box_ulke->currentText() );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_il, hesap_bilgileri.sehir    );

    m_ui->combo_box_ilce->clear();

    GET_ILCELER ( m_ui->combo_box_ilce , m_ui->combo_box_il->currentText() );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_ilce , hesap_bilgileri.ilce );


    m_ui->line_edit_posta_kodu->setText     ( hesap_bilgileri.posta_kodu );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_doviz, DVZ_GET_DOVIZ_KODU( hesap_bilgileri.para_birimi_id ));

    m_ui->check_box_esi_calisiyor_mu->setVisible( false );
    m_ui->label_esi_calisiyor_mu->setVisible( false );
    if ( PRS_EVLI EQ PRS_GET_MEDENI_HAL_ENUM( hesap_bilgileri.medeni_hali ) ) {
        m_ui->check_box_esi_calisiyor_mu->setVisible( true );
        m_ui->label_esi_calisiyor_mu->setVisible    ( true );

        m_ui->check_box_esi_calisiyor_mu->setEnabled( true );
    }

    ADRES_BILGILERINI_GOSTER();

    /********************************************************/
    //                 PERSONEL NOTLARI
    /********************************************************/

    sql_query.PREPARE_SELECT("prs_personel_notlari"," notlar "," personel_id = :personel_id ");

    sql_query.SET_VALUE( ":personel_id", record_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        m_ui->limitedTextEdit_notlar->setText( sql_query.VALUE("notlar").toString());
    }

    if ( PRS_PERSONEL_ICIN_KAYITLI_BORDRO_VAR_MI( record_id ) EQ ADAK_OK ) {
       m_ui->comma_edit_kum_vergi_matrahi->setEnabled( false );
       m_ui->commaEdit_bir_ay_once_kalan_odenek->setEnabled(false);
       m_ui->commaEdit_iki_ay_once_kalan_odenek->setEnabled(false);
    }

    //Eger Personele Zimmetli demirbas varsa

    QStringList zim_demirbas_list = DMR_GET_ID_KODU_ADI_LIST(record_id);

    if ( zim_demirbas_list.isEmpty() EQ false ) {

        for ( int i = 0 ; i < zim_demirbas_list.size() ; i++ ) {

            QStringList demirbas_id_kodu_adi = zim_demirbas_list.at(i).split("\t");

            QString demirbas = demirbas_id_kodu_adi.at(1) + "  " + demirbas_id_kodu_adi.at(2);

            m_ui->textEdit_zim_demirbaslar->append(demirbas + "\n");

        }
    }

    return ADAK_OK;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::FIND_RECORD             */
/********************************************************/

int PRS_PERSONEL_FORMU::FIND_RECORD ()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id","sicil_no = :sicil_no");


    sql_query.SET_VALUE( ":sicil_no"   , m_ui->line_edit_sicil_no->text() );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::SELECT_RECORD           */
/********************************************************/

int PRS_PERSONEL_FORMU::SELECT_RECORD ()
{

    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_KAYDI, this, 0, E9_PROGRAMI, PERSONEL_MODULU, "Personel Arama", "DELETED"  );


    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id",
                             "cari_hesap_id = :cari_hesap_id");

    sql_query.SET_VALUE( ":cari_hesap_id"   , cari_hesap_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;

}

/********************************************************/
/*            PRS_PERSONEL_FORMU::CHECK_VAR             */
/********************************************************/

int PRS_PERSONEL_FORMU::CHECK_VAR (QObject * object)
{

    if ( object EQ m_ui->combo_box_ev_telefonunun_ulke_kodlari )  {
        GET_ALAN_KODLARI(m_ui->combo_box_ev_telefonunun_il_kodlari,m_ui->combo_box_ev_telefonunun_ulke_kodlari->currentText());
    }
    else if ( object EQ m_ui->combo_box_is_telefonunun_ulke_kodlari )  {
        GET_ALAN_KODLARI(m_ui->combo_box_is_telefonunun_il_kodlari,m_ui->combo_box_is_telefonunun_ulke_kodlari->currentText());
    }
    else if ( object EQ m_ui->combo_box_medeni_hal ) {
        EVLI_DEGILSE_ES_BILGISINI_DISABLE_ET ();
    }
    else if (object EQ m_ui->combo_box_ulke){
        GET_ILLER         ( m_ui->combo_box_il, m_ui->combo_box_ulke->currentText() );
    }
    else if (object EQ m_ui->combo_box_il){
        GET_ILCELER         ( m_ui->combo_box_ilce, m_ui->combo_box_il->currentText() );

        QString il_telefon_kodu = GET_IL_ALAN_KODU ( m_ui->combo_box_il->currentText() , "" );

        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_ev_telefonunun_il_kodlari,    il_telefon_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->combo_box_is_telefonunun_il_kodlari,    il_telefon_kodu );
    }

    else if (object EQ m_ui->combo_box_kayitli_oldugu_il){
        GET_ILCELER ( m_ui->combo_box_kayitli_oldugu_ilce, m_ui->combo_box_kayitli_oldugu_il->currentText() );

    }
    else if ( ( object EQ m_ui->comma_edit_net_ucret   ) OR
              ( object EQ m_ui->comma_edit_brut_ucret  ) OR
              ( object EQ m_ui->combo_box_ucret_sekli ) ) {

        if ( m_ui->combo_box_ucret_sekli->currentIndex() EQ PRS_BRUT ) {

            m_ui->comma_edit_net_ucret->clear();
            m_ui->comma_edit_net_ucret->setEnabled (false);
            m_ui->comma_edit_brut_ucret->setEnabled(true);

        }
        else if ( m_ui->combo_box_ucret_sekli->currentIndex() EQ PRS_NET ) {

            m_ui->comma_edit_brut_ucret->clear();
            m_ui->comma_edit_brut_ucret->setEnabled(false);
            m_ui->comma_edit_net_ucret->setEnabled(true);            

        }
    }

    else if ( object EQ m_ui->check_box_esi_calisiyor_mu ) {


    }


    else if ( object EQ m_ui->push_button_resim ) {
        RESIM_SEC();
    }

    else if (object EQ m_ui->searchEdit_sube ){
            if ( m_ui->searchEdit_sube->GET_TEXT().isEmpty() EQ true ) {
                m_sube_id = -1;
                m_ui->lineEdit_sube_adi->clear();
                return ADAK_OK;
            }        
            QString sube_adi;
            int     sube_id;

            int result = SUBE_GET_SUBE_ID_ADI(  m_ui->searchEdit_sube->GET_TEXT() , &sube_id, &sube_adi );

            if ( result EQ 0 ) {
                MSG_WARNING( tr("Aradığınız sube kodu bulunamadı!.."), m_ui->searchEdit_sube );

                return ADAK_FAIL_UNDO;
            }
            m_ui->lineEdit_sube_adi->setText( sube_adi );
            m_sube_id = sube_id;
        }
    else if (object EQ m_ui->combo_box_doviz){
            m_doviz_id=DVZ_GET_DOVIZ_ID(m_ui->combo_box_doviz->currentText());
            m_ui->lineEdit_para_birimi->setText( m_ui->combo_box_doviz->currentText());
            m_ui->lineEdit_para_birimi->setEnabled(false);
            m_ui->lineEdit_para_birimi_1->setText(m_ui->combo_box_doviz->currentText());
            m_ui->lineEdit_para_birimi_1->setEnabled(false);
            m_ui->lineEdit_para_birimi_2->setText(m_ui->combo_box_doviz->currentText());
            m_ui->lineEdit_para_birimi_2->setEnabled(false);
    }
    SQL_QUERY query ( DB );

    MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;



    for ( int i = 0; i < TANIMLANACAK_ENT_HESAP_SAYISI; i++ ) {

        if ( object EQ M_SEARCH_EDIT_ENT_ARRAY[i] ) {

            if ( M_SEARCH_EDIT_ENT_ARRAY [i]->GET_TEXT().isEmpty() EQ true ) {
                M_ENT_HESAPLARI_ID_ARRAY  [i] = 0;
                M_LINE_EDIT_ENT_ARRAY     [i]->clear();
                return ADAK_OK;
            }
            MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

            if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , M_SEARCH_EDIT_ENT_ARRAY[i]->GET_TEXT()) EQ 0 ) {
                MSG_WARNING(  tr("Hesap bulunamadı"), M_SEARCH_EDIT_ENT_ARRAY[i] );
                return ADAK_FAIL_UNDO;
            }
            if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {
                MSG_WARNING(  tr ( "Seçilen hesap tali değil, tali bir hesap seçmelisiniz." ), M_SEARCH_EDIT_ENT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            M_ENT_HESAPLARI_ID_ARRAY[i]     = MUHASEBE_HESABI->hesap_id;
            M_LINE_EDIT_ENT_ARRAY   [i]->setText ( MUHASEBE_HESABI->hesap_ismi );

            return ADAK_OK;

        }
    }

    ADRES_BILGILERINI_GOSTER();

    return ADAK_OK;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::CHECK_EMPTY             */
/********************************************************/

int PRS_PERSONEL_FORMU::CHECK_EMPTY ()
{

    int kayita_devam_edilsin_mi;

    if ( m_ui->line_edit_sicil_no->text().isEmpty()) {
        m_ui->tab_widget->setCurrentIndex( 0 );
        MSG_WARNING(  tr("Personelin Sicil Numarasini Giriniz !.."), m_ui->line_edit_sicil_no );

        return ADAK_FAIL;
    }
    if ( m_ui->comma_edit_brut_ucret->GET_DOUBLE() EQ 0.0 AND
         m_ui->comma_edit_net_ucret->GET_DOUBLE() EQ 0.0 ) {

        m_ui->tab_widget->setCurrentIndex( 1 );
        MSG_WARNING(  tr("Personelin Brüt yada Net Ücret Bilgisini Giriniz !.."), m_ui->comma_edit_brut_ucret );

        return ADAK_FAIL;
    }
    if ( m_ui->combo_box_cinsiyet->currentIndex() < 0 ) {
        m_ui->tab_widget->setCurrentIndex( 2 );
        MSG_WARNING(  tr("Personelin Cinsiyetini Seçiniz !.."), m_ui->combo_box_cinsiyet );

        return ADAK_FAIL;
    }

    if ( m_ui->line_edit_tc_kimlik_no->text().isEmpty()) {
        m_ui->tab_widget->setCurrentIndex( 2 );
        MSG_WARNING(  tr("Personelin T.C. Kimlik Numarasini Giriniz !.."), m_ui->line_edit_tc_kimlik_no );
        return ADAK_FAIL;
    }

    if ( m_ui->combo_box_medeni_hal->currentIndex() < 0 ) {
        m_ui->tab_widget->setCurrentIndex( 2 );
        MSG_WARNING(  tr("Personelin Medeni Hal Bilgisini Giriniz !.."), m_ui->combo_box_medeni_hal );

        return ADAK_FAIL;
    }
    if ( m_ui->line_edit_sigorta_no->text().isEmpty()) {
        m_ui->tab_widget->setCurrentIndex( 4 );
        MSG_WARNING(  tr("Personelin Sigorta Numarasını Giriniz !.."), m_ui->line_edit_sigorta_no );

        return ADAK_FAIL;
    }
    if ( m_ui->comma_edit_kum_vergi_matrahi->GET_DOUBLE() EQ 0.0 AND QDate::currentDate().month() NE ENUM_OCAK ) {

        kayita_devam_edilsin_mi = MSG_YES_NO_CANCEL( tr("Personelin Kümülatif Vergi Matrahı Tutarını Girmediniz ! <br></br> "
                                                           "Geçmiş Dönem Matrahını Girmeden İşlem Yapmayı Onaylıyor Musunuz ?"),
                                        NULL );

        if ( kayita_devam_edilsin_mi EQ ADAK_NO OR kayita_devam_edilsin_mi EQ ADAK_CANCEL ) {

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/********************************************************/
/*           PRS_PERSONEL_FORMU::CHECK_ADD              */
/********************************************************/

int PRS_PERSONEL_FORMU::CHECK_ADD ()
{

    if ( SICIL_NO_UNIQUE_MI ( 0, m_ui->line_edit_sicil_no->text() ) EQ ADAK_FAIL ) {

        MSG_WARNING(  tr("Girilen Sicil No Şuan Kullanımda ! <br><br>"
                                                    "Personel İçin Yeni Bir Sicil No Giriniz !.."), NULL);
        SET_FOCUS( m_ui->line_edit_sicil_no );
        return ADAK_FAIL;
    }
    else if( CARI_HESAP_KODU_KULLANILDI_MI( m_ui->line_edit_sicil_no->text() ) )
     {
        MSG_WARNING( tr( "Girilen Sicil No Kullanılmaktadır. Lütfen Başka Bir Kod Girin..!!!" ), m_ui->line_edit_sicil_no );
        return ADAK_FAIL;
     }

    return ADAK_OK;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::ADD_RECORD              */
/********************************************************/

int PRS_PERSONEL_FORMU::ADD_RECORD ()
{
    SQL_QUERY       sql_query          ( DB );
    SQL_QUERY       sql_query_kisisel  ( DB );
    SQL_QUERY       sql_query_nufus    ( DB );
    SQL_QUERY       sql_query_notlar   ( DB );

    QString         log_detaylari;

    int emekli_mi = 0;

    if ( m_ui->checkBox_emekli_mi->isChecked() EQ true ) {
        emekli_mi = 1;
    }

    /**********************************************************
                            PERSONEL
    ***********************************************************/

    QString ind_kanunu_kodu, calisma_turu_kodu;

    ind_kanunu_kodu     = m_ui->comboBox_indirim_kanunlari->currentText().split("-").at(0);
    calisma_turu_kodu   = m_ui->comboBox_calisma_turleri->currentText().split("-").at(0);

    ind_kanunu_kodu.truncate( 5 );
    calisma_turu_kodu.truncate( 2 );

    sql_query.PREPARE_INSERT("prs_personeller","personel_id",
                             "cari_hesap_id,  "
                             "sicil_no ,sube_id ,prs_avanslar_hesap_id , "
                             "prs_borclar_hesap_id, sigorta_no,emekli_mi ,"
                             "ucret_sekli , net_ucret , brut_ucret , "
                             "kumulatif_vergi_matrahi , calisma_sekli,"
                             "isten_ayrildi_mi, ind_kanunu_kodu, calisma_turu_kodu,"
                             "dogum_yardimi_var_mi,                     "
                             "olum_yardimi_var_mi,                      "
                             "cocuk_yardimi_var_mi,                     "
                             "yakacak_yardimi_var_mi,                   "
                             "aile_yardimi_var_mi,                      "
                             "yemek_parasi_var_mi,                      "
                             "evlenme_yardimi_var_mi,                   "
                             "gorev_harcirahi_var_mi,                   "
                             "ihbar_tazminati_var_mi,                   "
                             "kidem_tazminati_var_mi,                   "
                             "haftalik_calisilan_gun_sayisi,            "
                             "gunluk_yemek_parasi_tutari,               "
                             "bir_ay_once_kalan_ek_odenek,              "
                             "iki_ay_once_kalan_ek_odenek               ");

    sql_query.SET_VALUE( ":cari_hesap_id"                   , 0                   );
    sql_query.SET_VALUE( ":sicil_no"                        , m_ui->line_edit_sicil_no->text()  );
    sql_query.SET_VALUE( ":sube_id"                         , m_sube_id                         );
    sql_query.SET_VALUE( ":prs_avanslar_hesap_id"           , M_ENT_HESAPLARI_ID_ARRAY[0]       );
    sql_query.SET_VALUE( ":prs_borclar_hesap_id"            , M_ENT_HESAPLARI_ID_ARRAY[1]       );
    sql_query.SET_VALUE( ":sigorta_no"                      , m_ui->line_edit_sigorta_no->text());
    sql_query.SET_VALUE( ":emekli_mi"                       , emekli_mi );
    sql_query.SET_VALUE( ":ucret_sekli"                     , m_ui->combo_box_ucret_sekli->currentIndex());
    sql_query.SET_VALUE( ":net_ucret"                       , m_ui->comma_edit_net_ucret->GET_DOUBLE());
    sql_query.SET_VALUE( ":brut_ucret"                      , m_ui->comma_edit_brut_ucret->GET_DOUBLE());
    sql_query.SET_VALUE( ":kumulatif_vergi_matrahi"         , m_ui->comma_edit_kum_vergi_matrahi->GET_DOUBLE());
    sql_query.SET_VALUE( ":calisma_sekli"                   , PRS_GET_CALISMA_SEKLI_ENUM( m_ui->comboBox_calisma_sekli->currentText()));
    sql_query.SET_VALUE( ":isten_ayrildi_mi"                , 0 );
    sql_query.SET_VALUE( ":ind_kanunu_kodu"                 , ind_kanunu_kodu );
    sql_query.SET_VALUE( ":calisma_turu_kodu"               , calisma_turu_kodu );
    sql_query.SET_VALUE( ":dogum_yardimi_var_mi"            , QVariant(m_ui->check_box_dogum_yardimi->isChecked()).toInt()   );
    sql_query.SET_VALUE( ":olum_yardimi_var_mi"             , QVariant(m_ui->check_box_olum_yardimi->isChecked()).toInt()    );
    sql_query.SET_VALUE( ":cocuk_yardimi_var_mi"            , QVariant(m_ui->check_box_cocuk_yardimi->isChecked()).toInt()   );
    sql_query.SET_VALUE( ":yakacak_yardimi_var_mi"          , QVariant(m_ui->check_box_yakacak_yardimi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":aile_yardimi_var_mi"             , QVariant(m_ui->check_box_aile_yardimi->isChecked()).toInt()    );
    sql_query.SET_VALUE( ":yemek_parasi_var_mi"             , QVariant(m_ui->check_box_yemek_parasi->isChecked()).toInt()    );
    sql_query.SET_VALUE( ":evlenme_yardimi_var_mi"          , QVariant(m_ui->check_box_evlenme_yardimi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":gorev_harcirahi_var_mi"          , QVariant(m_ui->check_box_gorev_harcirahi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":ihbar_tazminati_var_mi"          , QVariant(m_ui->check_box_ihbar_tazminati->isChecked()).toInt() );
    sql_query.SET_VALUE( ":kidem_tazminati_var_mi"          , QVariant(m_ui->check_box_kidem_tazminati->isChecked()).toInt() );
    sql_query.SET_VALUE( ":haftalik_calisilan_gun_sayisi"   , m_ui->commaEdit_haftalik_calisilan_gun_sayisi->GET_DOUBLE()    );
    sql_query.SET_VALUE( ":gunluk_yemek_parasi_tutari"      , m_ui->commaEdit_gunluk_yemek_parasi_tutari->GET_DOUBLE()    );
    sql_query.SET_VALUE( ":bir_ay_once_kalan_ek_odenek"     , m_ui->commaEdit_bir_ay_once_kalan_odenek->GET_DOUBLE());
    sql_query.SET_VALUE( ":iki_ay_once_kalan_ek_odenek"     , m_ui->commaEdit_iki_ay_once_kalan_odenek->GET_DOUBLE());

    m_personel_id = sql_query.INSERT();

                            /********************************************************/
                            //                 PERSONEL KISISEL BILGILERI
                            /********************************************************/

    sql_query_kisisel.PREPARE_INSERT("prs_personel_kisisel_blgler","kisisel_bilgi_id",
                             "personel_id, "
                             "uyruk,  "
                             "departman, ozel_durum_enum, meslek, unvan, "
                             "ogrenim_durumu, mezun_oldugu_okul, mezun_oldugu_bolum, "
                             "yabanci_dil_1, yabanci_dil_2, "
                             "ise_giris_tarihi,ssk_basvuru_tarihi, "
                             "ozel_indirim_dilekcesi, "
                             "sendika_uyesi_mi , resim, isten_cikis_tarihi ");

    sql_query_kisisel.SET_VALUE( ":personel_id"             , m_personel_id );
    sql_query_kisisel.SET_VALUE( ":uyruk"                   , m_ui->combo_box_uyruk->currentText() );
    sql_query_kisisel.SET_VALUE( ":departman"               , m_ui->line_edit_departman->text() );
    sql_query_kisisel.SET_VALUE( ":ozel_durum_enum"         , PRS_GET_OZEL_DURUM_ENUM(m_ui->combo_box_ozel_durum->currentText()) );
    sql_query_kisisel.SET_VALUE( ":meslek"                  , m_ui->combo_box_meslek->currentText() );
    sql_query_kisisel.SET_VALUE( ":unvan"                   , m_ui->combo_box_unvan->currentText() );
    sql_query_kisisel.SET_VALUE( ":ogrenim_durumu"          , m_ui->combo_box_ogrenim_durumu->currentText() );
    sql_query_kisisel.SET_VALUE( ":mezun_oldugu_okul"       , m_ui->line_edit_muzun_oldugu_okul->text() );
    sql_query_kisisel.SET_VALUE( ":mezun_oldugu_bolum"      , m_ui->combo_box_mezun_oldugu_bolum->currentText() );
    sql_query_kisisel.SET_VALUE( ":yabanci_dil_1"           , m_ui->combo_box_yabanci_dil_1->currentText() );
    sql_query_kisisel.SET_VALUE( ":yabanci_dil_2"           , m_ui->combo_box_yabanci_dil_2->currentText() );
    sql_query_kisisel.SET_VALUE( ":ise_giris_tarihi"        , m_ui->adakDate_ise_giris_tarihi->DATE() );
    sql_query_kisisel.SET_VALUE( ":ssk_basvuru_tarihi"      , m_ui->adakDate_ssk_basv_tarihi->DATE() );
    sql_query_kisisel.SET_VALUE( ":ozel_indirim_dilekcesi"  , m_ui->line_edit_ozel_indirim_dilekcesi->text() );
    sql_query_kisisel.SET_VALUE( ":sendika_uyesi_mi"        , m_ui->combo_box_sendika_uyesi_mi->currentIndex() );
    sql_query_kisisel.SET_VALUE( ":resim"                   , ADAK_PIXMAP_2_BYTE_ARRAY(m_fotograf_path) );
    sql_query_kisisel.SET_VALUE( ":isten_cikis_tarihi"      , "9999.99.99" );

    sql_query_kisisel.INSERT();

                    /********************************************************/
                    //                 PERSONEL ADRES BILGILERI
                    /********************************************************/

    int prs_cari_hesap_id = 0;


                CARI_HESAP_STRUCT    hesap_bilgileri;
                CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

                hesap_bilgileri.cari_kart_tipi         = ENUM_CARI_PERSONEL;
                hesap_bilgileri.para_birimi_id         = m_doviz_id;
                hesap_bilgileri.adres_kart_turu        = SAHIS_KAYDI;
                hesap_bilgileri.modul_id               = PERSONEL_MODULU;
                hesap_bilgileri.program_id             = E9_PROGRAMI;
                hesap_bilgileri.base_record_id         = m_personel_id;
                hesap_bilgileri.kimlik_no              = m_ui->line_edit_tc_kimlik_no->text();
                hesap_bilgileri.cinsiyet               = m_ui->combo_box_cinsiyet->currentText();
                hesap_bilgileri.cari_hesap_ismi        = QString(m_ui->line_edit_personel_adi->text() );
                hesap_bilgileri.cari_hesap_kodu        = m_ui->line_edit_sicil_no->text();
                hesap_bilgileri.dogum_tarihi           = m_ui->adakDate_dogum_tarihi->DATE();
                hesap_bilgileri.dogum_yeri             = m_ui->combo_box_dogum_yeri->currentText();
                hesap_bilgileri.cinsiyet               = m_ui->combo_box_cinsiyet->currentText();
                hesap_bilgileri.ogrenim_durumu         = m_ui->combo_box_ogrenim_durumu->currentText();
                hesap_bilgileri.meslek                 = m_ui->combo_box_meslek->currentText();
                hesap_bilgileri.unvan                  = m_ui->combo_box_unvan->currentText();
                hesap_bilgileri.kimlik_no              = m_ui->line_edit_tc_kimlik_no->text();
                hesap_bilgileri.kan_grubu              = m_ui->combo_box_kan_grubu->currentText();
                hesap_bilgileri.medeni_hali            = m_ui->combo_box_medeni_hal->currentText();
                hesap_bilgileri.adres_satiri_1         = m_ui->line_edit_adres_satiri_1->text();
                hesap_bilgileri.adres_satiri_2         = m_ui->line_edit_adres_satiri_2->text();
                hesap_bilgileri.adres_satiri_3         = m_ui->line_edit_adres_satiri_3->text();
                hesap_bilgileri.ilce                   = m_ui->combo_box_ilce->currentText();
                hesap_bilgileri.semt                   = m_ui->line_edit_semt->text();
                hesap_bilgileri.ulke                   = m_ui->combo_box_ulke->currentText();
                hesap_bilgileri.sehir                  = m_ui->combo_box_il->currentText();
                hesap_bilgileri.posta_kodu             = m_ui->line_edit_posta_kodu->text();
                hesap_bilgileri.tel_ulke_kodu          = m_ui->combo_box_ev_telefonunun_ulke_kodlari->currentText();
                hesap_bilgileri.tel_alan_kodu          = m_ui->combo_box_ev_telefonunun_il_kodlari->currentText();
                hesap_bilgileri.telefon                = m_ui->line_edit_ev_telefonu->text();
                hesap_bilgileri.is_tel_ulke_kodu       = m_ui->combo_box_is_telefonunun_ulke_kodlari->currentText();
                hesap_bilgileri.is_tel_alan_kodu       = m_ui->combo_box_is_telefonunun_il_kodlari->currentText();
                hesap_bilgileri.is_telefonu            = m_ui->line_edit_is_telefonu->text();
                hesap_bilgileri.cep_tel_ulke_kodu      = m_ui->combo_box_cep_telefonunun_ulke_kodlari->currentText();
                hesap_bilgileri.cep_tel_gsm_kodu       = m_ui->combo_box_gsm_kodlari->currentText();
                hesap_bilgileri.cep_telefonu           = m_ui->line_edit_cep_telefonu->text();
                hesap_bilgileri.fax_ulke_kodu          = "";
                hesap_bilgileri.fax_alan_kodu          = "";
                hesap_bilgileri.fax                    = "";
                hesap_bilgileri.email                  = m_ui->line_edit_e_mail->text();
                hesap_bilgileri.muh_hesap_id           = MUH_GET_HESAP_ID( m_ui->searchEdit_personele_borclar_hesap_kodu->GET_TEXT() );

                prs_cari_hesap_id = CARI_KART_OLUSTUR( SAHIS_KAYDI , &hesap_bilgileri);



                            /********************************************************/
                            //                 PERSONEL NUFUS BILGILERI
                            /********************************************************/

    sql_query_nufus.PREPARE_INSERT("prs_personel_nufus_blgler ","nufus_id",
                                   " personel_id, nufus_cuzdani_seri_no, "
                                   " baba_adi, anne_adi,  "
                                   " dini, ilk_soyadi, "
                                   "kayitli_oldugu_il, kayitli_oldugu_ilce, kayitli_oldugu_mahalle, "
                                   "cilt_no, aile_sira_no, sira_no, "
                                   "verildigi_yer, verilis_nedeni, kayit_no, verilis_tarihi");


    sql_query_nufus.SET_VALUE( ":personel_id"             , m_personel_id );
    sql_query_nufus.SET_VALUE( ":nufus_cuzdani_seri_no"   , m_ui->line_edit_seri_no->text() );
    sql_query_nufus.SET_VALUE( ":baba_adi"                , m_ui->line_edit_baba_adi->text());
    sql_query_nufus.SET_VALUE( ":anne_adi"                , m_ui->line_edit_anne_adi->text());
    sql_query_nufus.SET_VALUE( ":dini"                    , m_ui->line_edit_din->text() );
    sql_query_nufus.SET_VALUE( ":ilk_soyadi"              , m_ui->line_edit_ilk_soyadi->text() );
    sql_query_nufus.SET_VALUE( ":kayitli_oldugu_il"       , m_ui->combo_box_kayitli_oldugu_il->currentText() );
    sql_query_nufus.SET_VALUE( ":kayitli_oldugu_ilce"     , m_ui->combo_box_kayitli_oldugu_ilce->currentText() );
    sql_query_nufus.SET_VALUE( ":kayitli_oldugu_mahalle"  , m_ui->line_edit_kayitli_oldugu_mahalle->text() );
    sql_query_nufus.SET_VALUE( ":cilt_no"                 , m_ui->line_edit_cilt_no->text() );
    sql_query_nufus.SET_VALUE( ":aile_sira_no"            , m_ui->line_edit_aile_sira_no->text());
    sql_query_nufus.SET_VALUE( ":sira_no"                 , m_ui->line_edit_sira_no->text());
    sql_query_nufus.SET_VALUE( ":verildigi_yer"           , m_ui->line_edit_verildigi_yer->text() );
    sql_query_nufus.SET_VALUE( ":verilis_nedeni"          , m_ui->line_edit_verilis_nedeni->text() );
    sql_query_nufus.SET_VALUE( ":kayit_no"                , m_ui->line_edit_kayit_no->text().toInt() );
    sql_query_nufus.SET_VALUE( ":verilis_tarihi"          , m_ui->adakDate_verilis_tarihi->DATE());


   sql_query_nufus.INSERT();


                            /********************************************************/
                            //                 PERSONEL NOTLARI
                            /********************************************************/

    sql_query_notlar.PREPARE_INSERT("prs_personel_notlari ","notlar_id",
                                     "personel_id, notlar ");

    sql_query_notlar.SET_VALUE( ":personel_id"        , m_personel_id );
    sql_query_notlar.SET_VALUE( ":notlar"             , m_ui->limitedTextEdit_notlar->toPlainText() );

    sql_query_notlar.INSERT();



    SQL_QUERY query( DB );

    query.PREPARE_UPDATE( " prs_personeller","personel_id"," cari_hesap_id" ," personel_id = :personel_id " );
    query.SET_VALUE     ( ":cari_hesap_id", prs_cari_hesap_id );
    query.SET_VALUE     ( ":personel_id", m_personel_id );

    query.UPDATE();


    log_detaylari = "Personel ( " + m_ui->line_edit_personel_adi->text().toUpper() + " "+
                    m_ui->line_edit_personel_adi->text().toUpper() + " ) Eklenmiştir."
                    + QObject::tr("\nPersonel Sicil No : ") + m_ui->line_edit_sicil_no->text();

    E9_LOG_KAYDI_EKLE(PERSONEL_MODULU , LOG_PRS_PERSONEL , LOG_ISLEM_ADD , log_detaylari);

    GRP_KAYDI_GRUPLARA_EKLE( E9_PROGRAMI , PERSONEL_MODULU , m_personel_id ,&prs_grup_idleri);

    return m_personel_id;

}

/********************************************************/
/*          PRS_PERSONEL_FORMU::CHECK_UPDATE            */
/********************************************************/

int PRS_PERSONEL_FORMU::CHECK_UPDATE (int record_id)
{ 
    Q_UNUSED ( record_id );

    if ( old_personel_sicil_kodu NE m_ui->line_edit_sicil_no->text() ) {
        if( CARI_HESAP_KODU_KULLANILDI_MI( m_ui->line_edit_sicil_no->text() ) ) {
           MSG_WARNING( tr( "Girilen Sicil No Kullanılmaktadır. Lütfen Başka Bir Kod Girin..!!!" ), m_ui->line_edit_sicil_no );
           SET_FOCUS( m_ui->line_edit_sicil_no );
           return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::UPDATE_RECORD           */
/********************************************************/

void PRS_PERSONEL_FORMU::UPDATE_RECORD (int record_id)
{
    SQL_QUERY       sql_query          ( DB );
    SQL_QUERY       sql_query_kisisel  ( DB );
    SQL_QUERY       sql_query_nufus    ( DB );
    SQL_QUERY       sql_query_notlar   ( DB );
    QString         log_detaylari;


    int emekli_mi = 0;
    if ( m_ui->checkBox_emekli_mi->isChecked() EQ true ) {
        emekli_mi = 1;
    }

                            /********************************************************/
                            //                 PERSONEL
                            /********************************************************/

    QString ind_kanunu_kodu, calisma_turu_kodu;

    ind_kanunu_kodu     = m_ui->comboBox_indirim_kanunlari->currentText().split("-").at(0);
    calisma_turu_kodu   = m_ui->comboBox_calisma_turleri->currentText().split("-").at(0);

    ind_kanunu_kodu.truncate    ( 5 );
    calisma_turu_kodu.truncate  ( 2 );

    sql_query.PREPARE_UPDATE("prs_personeller","personel_id","sicil_no ,"
                             "sube_id                                           , "
                             "prs_avanslar_hesap_id                             , "
                             "prs_borclar_hesap_id                              , "
                             "sigorta_no                                        , "
                             "emekli_mi                                         , "
                             "ucret_sekli                                       , "
                             "net_ucret                                         , "
                             "brut_ucret                                        , "
                             "kumulatif_vergi_matrahi                           , "
                             "calisma_sekli                                     , "
                             "ind_kanunu_kodu                                   , "
                             "calisma_turu_kodu                                 , "
                             "dogum_yardimi_var_mi , olum_yardimi_var_mi        , "
                             "cocuk_yardimi_var_mi, yakacak_yardimi_var_mi      , "
                             "aile_yardimi_var_mi , yemek_parasi_var_mi         , "
                             "evlenme_yardimi_var_mi, gorev_harcirahi_var_mi    , "
                             "ihbar_tazminati_var_mi, kidem_tazminati_var_mi    , "
                             "haftalik_calisilan_gun_sayisi                     , "
                             "gunluk_yemek_parasi_tutari                        , "
                             "bir_ay_once_kalan_ek_odenek                       , "
                             "iki_ay_once_kalan_ek_odenek                         ",
                             "personel_id = :personel_id");

    sql_query.SET_VALUE( ":sicil_no"                        , m_ui->line_edit_sicil_no->text()      );
    sql_query.SET_VALUE( ":sube_id"                         , m_sube_id                             );
    sql_query.SET_VALUE( ":prs_avanslar_hesap_id"           , M_ENT_HESAPLARI_ID_ARRAY[0]           );
    sql_query.SET_VALUE( ":prs_borclar_hesap_id"            , M_ENT_HESAPLARI_ID_ARRAY[1]           );
    sql_query.SET_VALUE( ":sigorta_no"                      , m_ui->line_edit_sigorta_no->text()    );
    sql_query.SET_VALUE( ":emekli_mi"                       , emekli_mi );
    sql_query.SET_VALUE( ":ucret_sekli"                     , m_ui->combo_box_ucret_sekli->currentIndex());
    sql_query.SET_VALUE( ":net_ucret"                       , m_ui->comma_edit_net_ucret->GET_DOUBLE() );
    sql_query.SET_VALUE( ":brut_ucret"                      , m_ui->comma_edit_brut_ucret->GET_DOUBLE() );
    sql_query.SET_VALUE( ":kumulatif_vergi_matrahi"         , m_ui->comma_edit_kum_vergi_matrahi->GET_DOUBLE() );
    sql_query.SET_VALUE( ":calisma_sekli"                   , PRS_GET_CALISMA_SEKLI_ENUM( m_ui->comboBox_calisma_sekli->currentText()) );
    sql_query.SET_VALUE( ":ind_kanunu_kodu"                 , ind_kanunu_kodu );
    sql_query.SET_VALUE( ":calisma_turu_kodu"               , calisma_turu_kodu );
    sql_query.SET_VALUE( ":dogum_yardimi_var_mi"            , QVariant(m_ui->check_box_dogum_yardimi->isChecked()).toInt()   );
    sql_query.SET_VALUE( ":olum_yardimi_var_mi"             , QVariant(m_ui->check_box_olum_yardimi->isChecked()).toInt()    );
    sql_query.SET_VALUE( ":cocuk_yardimi_var_mi"            , QVariant(m_ui->check_box_cocuk_yardimi->isChecked()).toInt()   );
    sql_query.SET_VALUE( ":yakacak_yardimi_var_mi"          , QVariant(m_ui->check_box_yakacak_yardimi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":aile_yardimi_var_mi"             , QVariant(m_ui->check_box_aile_yardimi->isChecked()).toInt()    );
    sql_query.SET_VALUE( ":yemek_parasi_var_mi"             , QVariant(m_ui->check_box_yemek_parasi->isChecked()).toInt()    );
    sql_query.SET_VALUE( ":evlenme_yardimi_var_mi"          , QVariant(m_ui->check_box_evlenme_yardimi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":gorev_harcirahi_var_mi"          , QVariant(m_ui->check_box_gorev_harcirahi->isChecked()).toInt() );
    sql_query.SET_VALUE( ":ihbar_tazminati_var_mi"          , QVariant(m_ui->check_box_ihbar_tazminati->isChecked()).toInt() );
    sql_query.SET_VALUE( ":kidem_tazminati_var_mi"          , QVariant(m_ui->check_box_kidem_tazminati->isChecked()).toInt() );
    sql_query.SET_VALUE( ":haftalik_calisilan_gun_sayisi"   , m_ui->commaEdit_haftalik_calisilan_gun_sayisi->GET_DOUBLE()    );
    sql_query.SET_VALUE( ":gunluk_yemek_parasi_tutari"      , m_ui->commaEdit_gunluk_yemek_parasi_tutari->GET_DOUBLE()    );
    sql_query.SET_VALUE( ":bir_ay_once_kalan_ek_odenek"     , m_ui->commaEdit_bir_ay_once_kalan_odenek->GET_DOUBLE());
    sql_query.SET_VALUE( ":iki_ay_once_kalan_ek_odenek"     , m_ui->commaEdit_iki_ay_once_kalan_odenek->GET_DOUBLE());

    sql_query.SET_VALUE( ":personel_id"                     , record_id);

    sql_query.UPDATE();
           /********************************************************/
           //            PERSONEL KISISEL BILGILERI
           /********************************************************/


    sql_query_kisisel.PREPARE_UPDATE(" prs_personel_kisisel_blgler","kisisel_bilgi_id",
                             "uyruk, "
                             "departman , ozel_durum_enum , "
                             "meslek , "
                             "unvan  , "
                             "ogrenim_durumu , "
                             "mezun_oldugu_okul , "
                             "mezun_oldugu_bolum , "
                             "yabanci_dil_1 , yabanci_dil_2 , "
                             "ise_giris_tarihi , "
                             "ssk_basvuru_tarihi , "
                             "ozel_indirim_dilekcesi , "
                             "sendika_uyesi_mi  ",
                             "personel_id = :personel_id" );


    sql_query_kisisel.SET_VALUE( ":uyruk"                   , m_ui->combo_box_uyruk->currentText() );
    sql_query_kisisel.SET_VALUE( ":departman"               , m_ui->line_edit_departman->text() );
    sql_query_kisisel.SET_VALUE( ":ozel_durum_enum"         , PRS_GET_OZEL_DURUM_ENUM(m_ui->combo_box_ozel_durum->currentText()));
    sql_query_kisisel.SET_VALUE( ":meslek"                  , m_ui->combo_box_meslek->currentText() );
    sql_query_kisisel.SET_VALUE( ":unvan"                   , m_ui->combo_box_unvan->currentText() );
    sql_query_kisisel.SET_VALUE( ":ogrenim_durumu"          , m_ui->combo_box_ogrenim_durumu->currentText() );
    sql_query_kisisel.SET_VALUE( ":mezun_oldugu_okul"       , m_ui->line_edit_muzun_oldugu_okul->text() );
    sql_query_kisisel.SET_VALUE( ":mezun_oldugu_bolum"      , m_ui->combo_box_mezun_oldugu_bolum->currentText() );
    sql_query_kisisel.SET_VALUE( ":yabanci_dil_1"           , m_ui->combo_box_yabanci_dil_1->currentText() );
    sql_query_kisisel.SET_VALUE( ":yabanci_dil_2"           , m_ui->combo_box_yabanci_dil_2->currentText() );
    sql_query_kisisel.SET_VALUE( ":ise_giris_tarihi"        , m_ui->adakDate_ise_giris_tarihi->DATE());
    sql_query_kisisel.SET_VALUE( ":ssk_basvuru_tarihi"      , m_ui->adakDate_ssk_basv_tarihi->DATE() );
    sql_query_kisisel.SET_VALUE( ":ozel_indirim_dilekcesi"  , m_ui->line_edit_ozel_indirim_dilekcesi->text() );
    sql_query_kisisel.SET_VALUE( ":sendika_uyesi_mi"        , m_ui->combo_box_sendika_uyesi_mi->currentIndex() );
    sql_query_kisisel.SET_VALUE( ":personel_id"             , record_id);

    sql_query_kisisel.UPDATE();

    if (m_fotograf_path.isEmpty() EQ false) {
        sql_query_kisisel.PREPARE_UPDATE("prs_personel_kisisel_blgler","kisisel_bilgi_id"," resim ", " personel_id = :personel_id");
        sql_query_kisisel.SET_VALUE(":resim",ADAK_PIXMAP_2_BYTE_ARRAY(m_fotograf_path));
        sql_query_kisisel.SET_VALUE(":personel_id"     ,record_id);
        sql_query_kisisel.UPDATE();
    }


        /********************************************************/
        //                 PERSONEL ADRES BILGILERI
        /********************************************************/

        CARI_HESAP_STRUCT    hesap_bilgileri;
        CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

        hesap_bilgileri.para_birimi_id         = m_doviz_id;
        hesap_bilgileri.muh_hesap_id           = MUH_GET_HESAP_ID( m_ui->searchEdit_personele_borclar_hesap_kodu->GET_TEXT());
        hesap_bilgileri.adres_kart_turu        = SAHIS_KAYDI;
        hesap_bilgileri.modul_id               = PERSONEL_MODULU;
        hesap_bilgileri.program_id             = E9_PROGRAMI;
        hesap_bilgileri.kimlik_no              = m_ui->line_edit_tc_kimlik_no->text();
        hesap_bilgileri.cinsiyet               = m_ui->combo_box_cinsiyet->currentText();
        hesap_bilgileri.cari_hesap_ismi        = QString(m_ui->line_edit_personel_adi->text());
        hesap_bilgileri.cari_hesap_kodu        = m_ui->line_edit_sicil_no->text();
        hesap_bilgileri.dogum_tarihi           = m_ui->adakDate_dogum_tarihi->DATE();
        hesap_bilgileri.dogum_yeri             = m_ui->combo_box_dogum_yeri->currentText();
        hesap_bilgileri.cinsiyet               = m_ui->combo_box_cinsiyet->currentText();
        hesap_bilgileri.ogrenim_durumu         = m_ui->combo_box_ogrenim_durumu->currentText();
        hesap_bilgileri.meslek                 = m_ui->combo_box_meslek->currentText();
        hesap_bilgileri.unvan                  = m_ui->combo_box_unvan->currentText();
        hesap_bilgileri.kimlik_no              = m_ui->line_edit_tc_kimlik_no->text();
        hesap_bilgileri.kan_grubu              = m_ui->combo_box_kan_grubu->currentText();
        hesap_bilgileri.medeni_hali            = m_ui->combo_box_medeni_hal->currentText();
        hesap_bilgileri.adres_satiri_1         = m_ui->line_edit_adres_satiri_1->text();
        hesap_bilgileri.adres_satiri_2         = m_ui->line_edit_adres_satiri_2->text();
        hesap_bilgileri.adres_satiri_3         = m_ui->line_edit_adres_satiri_3->text();
        hesap_bilgileri.ilce                   = m_ui->combo_box_ilce->currentText();
        hesap_bilgileri.semt                   = m_ui->line_edit_semt->text();
        hesap_bilgileri.ulke                   = m_ui->combo_box_ulke->currentText();
        hesap_bilgileri.sehir                  = m_ui->combo_box_il->currentText();
        hesap_bilgileri.posta_kodu             = m_ui->line_edit_posta_kodu->text();
        hesap_bilgileri.tel_ulke_kodu          = m_ui->combo_box_ev_telefonunun_ulke_kodlari->currentText();
        hesap_bilgileri.tel_alan_kodu          = m_ui->combo_box_ev_telefonunun_il_kodlari->currentText();
        hesap_bilgileri.telefon                = m_ui->line_edit_ev_telefonu->text();
        hesap_bilgileri.is_tel_ulke_kodu       = m_ui->combo_box_is_telefonunun_ulke_kodlari->currentText();
        hesap_bilgileri.is_tel_alan_kodu       = m_ui->combo_box_is_telefonunun_il_kodlari->currentText();
        hesap_bilgileri.is_telefonu            = m_ui->line_edit_is_telefonu->text();
        hesap_bilgileri.cep_tel_ulke_kodu      = m_ui->combo_box_cep_telefonunun_ulke_kodlari->currentText();
        hesap_bilgileri.cep_tel_gsm_kodu       = m_ui->combo_box_gsm_kodlari->currentText();
        hesap_bilgileri.cep_telefonu           = m_ui->line_edit_cep_telefonu->text();
        hesap_bilgileri.fax_ulke_kodu          = "";
        hesap_bilgileri.fax_alan_kodu          = "";
        hesap_bilgileri.fax                    = "";
        hesap_bilgileri.email                  = m_ui->line_edit_e_mail->text();
        hesap_bilgileri.firma_sektoru          = "";

        CARI_KART_GUNCELLE ( PRS_GET_CARI_HESAP_ID( record_id ), SAHIS_KAYDI, &hesap_bilgileri );

        GRP_KAYDI_GRUPLARA_EKLE( E9_PROGRAMI, PERSONEL_MODULU, record_id ,&prs_grup_idleri );

           /********************************************************/
           //                 PERSONEL NUFUS BILGILERI
           /********************************************************/

    sql_query_nufus.PREPARE_UPDATE("prs_personel_nufus_blgler","nufus_id","nufus_cuzdani_seri_no , "
                                    "baba_adi , anne_adi , "
                                    "dini , "
                                    "ilk_soyadi , "
                                    "kayitli_oldugu_il , kayitli_oldugu_ilce , "
                                    "kayitli_oldugu_mahalle , "
                                    "cilt_no , aile_sira_no , sira_no, "
                                    "verildigi_yer , verilis_nedeni , "
                                    "kayit_no , verilis_tarihi"," personel_id = :personel_id");


    sql_query_nufus.SET_VALUE( ":nufus_cuzdani_seri_no"   , m_ui->line_edit_seri_no->text()                                 );
    sql_query_nufus.SET_VALUE( ":baba_adi"                , m_ui->line_edit_baba_adi->text()                                );
    sql_query_nufus.SET_VALUE( ":anne_adi"                , m_ui->line_edit_anne_adi->text()                                );
    sql_query_nufus.SET_VALUE( ":dini"                    , m_ui->line_edit_din->text()                                     );
    sql_query_nufus.SET_VALUE( ":ilk_soyadi"              , m_ui->line_edit_ilk_soyadi->text()                              );
    sql_query_nufus.SET_VALUE( ":kayitli_oldugu_il"       , m_ui->combo_box_kayitli_oldugu_il->currentText()                );
    sql_query_nufus.SET_VALUE( ":kayitli_oldugu_ilce"     , m_ui->combo_box_kayitli_oldugu_ilce->currentText()              );
    sql_query_nufus.SET_VALUE( ":kayitli_oldugu_mahalle"  , m_ui->line_edit_kayitli_oldugu_mahalle->text()                  );
    sql_query_nufus.SET_VALUE( ":cilt_no"                 , m_ui->line_edit_cilt_no->text()                                 );
    sql_query_nufus.SET_VALUE( ":aile_sira_no"            , m_ui->line_edit_aile_sira_no->text()                            );
    sql_query_nufus.SET_VALUE( ":sira_no"                 , m_ui->line_edit_sira_no->text()                                 );
    sql_query_nufus.SET_VALUE( ":verildigi_yer"           , m_ui->line_edit_verildigi_yer->text()                           );
    sql_query_nufus.SET_VALUE( ":verilis_nedeni"          , m_ui->line_edit_verilis_nedeni->text()                          );
    sql_query_nufus.SET_VALUE( ":kayit_no"                , m_ui->line_edit_kayit_no->text()                                );
    sql_query_nufus.SET_VALUE( ":verilis_tarihi"          , m_ui->adakDate_verilis_tarihi->DATE()   );
    sql_query_nufus.SET_VALUE( ":personel_id"             , record_id);


    sql_query_nufus.UPDATE();

            /********************************************************/
            //                 PERSONEL NOTLAR
            /********************************************************/

    sql_query_notlar.PREPARE_UPDATE("prs_personel_notlari","notlar_id","notlar ",
                                    "personel_id = :personel_id");


    sql_query_notlar.SET_VALUE( ":notlar"           , m_ui->limitedTextEdit_notlar->toPlainText() );
    sql_query_notlar.SET_VALUE( ":personel_id"      , record_id );

    sql_query_notlar.UPDATE();

    log_detaylari = "Personel ( " + m_ui->line_edit_personel_adi->text().toUpper()  + " ) 'in bilgileri guncellenmistir."
                    + QObject::tr("\nPersonel Sicil No : ") + m_ui->line_edit_sicil_no->text();

    E9_LOG_KAYDI_EKLE(PERSONEL_MODULU , LOG_PRS_PERSONEL , LOG_ISLEM_UPDATE , log_detaylari);

}

/********************************************************/
/*          PRS_PERSONEL_FORMU::CHECK_DELETE            */
/********************************************************/

int PRS_PERSONEL_FORMU::CHECK_DELETE (int record_id)
{
    int     personel_silinsin_mi;

    Q_UNUSED(record_id);

    personel_silinsin_mi = MSG_YES_NO_CANCEL( tr("Personel Kaydı <b>SİLİNECEK</b>, Devam Edilsin Mi ?"),
                                        NULL );

    if ( personel_silinsin_mi EQ ADAK_NO OR personel_silinsin_mi EQ ADAK_CANCEL ) {

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::DELETE_RECORD           */
/********************************************************/

void PRS_PERSONEL_FORMU::DELETE_RECORD (int record_id)
{
    SQL_QUERY       sql_query( DB );
    QString         log_detaylari;

    sql_query.PREPARE_SELECT("prs_personeller"," sicil_no, cari_hesap_id ",
                             "personel_id = :personel_id");
    sql_query.SET_VALUE (":personel_id" , record_id);

    if(sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        log_detaylari = ("Personel Sicil No : ") + sql_query.VALUE(0).toString();


        int cari_hesap_id = sql_query.VALUE( "cari_hesap_id").toInt();
        CARI_HESAP_STRUCT hesap_bilgileri;
        CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

        CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri  );

        log_detaylari.append(", Adı Soyadı : " + hesap_bilgileri.cari_hesap_ismi );
    }

    E9_LOG_KAYDI_EKLE (PERSONEL_MODULU , LOG_PRS_PERSONEL , LOG_ISLEM_DELETE , log_detaylari);

    sql_query.PREPARE_DELETE("prs_personeller ", "personel_id = :personel_id");
    sql_query.SET_VALUE( ":personel_id", record_id );
    sql_query.DELETE();

    CARI_KART_SIL( PRS_GET_CARI_HESAP_ID( record_id ) );

    sql_query.PREPARE_DELETE("prs_personel_nufus_blgler", "personel_id = :personel_id");
    sql_query.SET_VALUE( ":personel_id", record_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE(" prs_personel_kisisel_blgler"," personel_id = :personel_id");
    sql_query.SET_VALUE( ":personel_id", record_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE("prs_personel_aile_uyeleri","personel_id = :personel_id");
    sql_query.SET_VALUE( ":personel_id", record_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE("prs_ek_kesintiler_odenekler "," personel_id = :personel_id");
    sql_query.SET_VALUE( ":personel_id", record_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE("prs_personel_notlari"," personel_id = :personel_id");
    sql_query.SET_VALUE( ":personel_id", record_id );
    sql_query.DELETE();

    //Personel silindiginde uzerine zimmetlemis demirbaslar varmi diye kontrol edilir.
    //Egere varsa demirbas modulundeki personel id sifirlanir.

    CARI_HESAP_FILTRE_STR_UPDATE( PRS_GET_PERSONEL_CARI_HESAP_ID( m_personel_id ), "DELETED");

    DMR_UPDATE_PERSONEL_DEMIRBAS_DURUM( record_id, 0 );
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::FIND_NEXT_RECORD        */
/********************************************************/

int PRS_PERSONEL_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id ",
                              " sicil_no > :sicil_no AND isten_ayrildi_mi=:isten_ayrildi_mi");

    sql_query.SET_VALUE( ":sicil_no"        , m_ui->line_edit_sicil_no->text() );
    sql_query.SET_VALUE( ":isten_ayrildi_mi", 0                                );

    if ( sql_query.SELECT("sicil_no ASC",0,1 ) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }
    return -1;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::FIND_PREV_RECORD        */
/********************************************************/

int PRS_PERSONEL_FORMU::FIND_PREV_RECORD ()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id ",
                              "sicil_no < :sicil_no AND isten_ayrildi_mi=:isten_ayrildi_mi");

    sql_query.SET_VALUE( ":sicil_no", m_ui->line_edit_sicil_no->text() );
    sql_query.SET_VALUE( ":isten_ayrildi_mi", 0                                );

    if ( sql_query.SELECT("sicil_no DESC",0,1) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::FIND_FIRST_RECORD       */
/********************************************************/

int PRS_PERSONEL_FORMU::FIND_FIRST_RECORD ()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT( "prs_personeller","personel_id","isten_ayrildi_mi=:isten_ayrildi_mi");
    sql_query.SET_VALUE     ( ":isten_ayrildi_mi", 0     );

    if ( sql_query.SELECT("sicil_no ASC",0,1) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::FIND_LAST_RECORD        */
/********************************************************/

int PRS_PERSONEL_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT( "prs_personeller","personel_id","isten_ayrildi_mi=:isten_ayrildi_mi");
    sql_query.SET_VALUE     ( ":isten_ayrildi_mi", 0      );

    if ( sql_query.SELECT("sicil_no DESC",0,1) NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::LOCK_RECORD             */
/********************************************************/

int PRS_PERSONEL_FORMU::LOCK_RECORD (int record_id)
{
    return DB->LOCK_ROW( "personel_id","prs_personeller", QString("personel_id = %1").arg( record_id ) );
}

/********************************************************/
/*          PRS_PERSONEL_FORMU::UNLOCK_RECORD           */
/********************************************************/

void PRS_PERSONEL_FORMU::UNLOCK_RECORD (int record_id)
{
    DB->UNLOCK_ROW( "personel_id","prs_personeller", QString("personel_id = %1").arg( record_id ) );
}

/**************************************************************************************
                   PRS_PERSONEL_FORMU::SAVER_BUTTON_CLICKED
***************************************************************************************/

void PRS_PERSONEL_FORMU::SAVER_BUTTON_CLICKED(QAbstractButton * button, int record_id)
{
    SQL_QUERY       sql_query( DB );

    int calisma_durumu = -1 ;

    if ( button EQ m_ui->push_button_aile_bireyleri ) {

        SHOW_PRS_PERSONEL_AILE_FISI ( record_id, this );

        sql_query.PREPARE_SELECT("prs_personel_aile_uyeleri","calisma_durumu",
                                 "personel_id = :personel_id");

        sql_query.SET_VALUE( ":personel_id", record_id );

        if ( calisma_durumu EQ  PRS_CALISIYOR  ) {
            m_ui->check_box_esi_calisiyor_mu->setChecked ( true );
            m_ui->label_esi_calisiyor_mu->setVisible     ( true );
        }
        else {
            m_ui->check_box_esi_calisiyor_mu->setChecked ( false );
            m_ui->label_esi_calisiyor_mu->setVisible     ( false);

        }
    }
    else if ( button EQ m_ui->push_button_ek_kesinti_ekle ) {

        SHOW_PRS_PERSONEL_KESINTI_EKLEME_FISI( PRS_PERSONEL_SICIL_KARTI, record_id, this );
    }
    else if ( button EQ m_ui->push_button_ek_odenek_ekle ) {

        SHOW_PRS_PERSONEL_ODENEK_EKLEME_FISI( PRS_PERSONEL_SICIL_KARTI, record_id, this );
    }
    else if ( button EQ m_ui->push_button_not_defteri) {

        OPEN_CARI_NOT_DEFTERI( PRS_GET_CARI_HESAP_ID( record_id ), PERSONEL_MODULU, E9_PROGRAMI, this );

    }
}

///************************************************************/
///*          PRS_PERSONEL_FORMU::RESIM_SEC_VE_YUKLE          */
///************************************************************/

//void PRS_PERSONEL_FORMU::RESIM_SEC_VE_YUKLE()
//{
//    QFileDialog         file_dialog(this);
//    QStringList         dosya_uzantilari;
//    double              width;
//    double              height;

//    width   = m_ui->push_button_resim->width();
//    height  = m_ui->push_button_resim->height();

//    file_dialog.setFileMode(QFileDialog::AnyFile);

//    dosya_uzantilari << tr("Resim Dosyaları (*.png *.xpm *.jpg *.jpeg)")
//                     << tr("Tüm Dosyalar (*)");

//    file_dialog.setNameFilters( dosya_uzantilari );
//    file_dialog.setViewMode( QFileDialog::Detail );

//    if ( file_dialog.exec() ) {

//        m_ui->push_button_resim->setText("");
//        m_resim = file_dialog.selectedFiles().at(0) ;
//        m_ui->push_button_resim->setIcon( QIcon( m_resim ) );
//        m_ui->push_button_resim->setIconSize(QSize(width, height));
//    }
//}


/************************************************************/
/*          PRS_PERSONEL_FORMU::SICIL_NO_UNIQUE_MI          */
/************************************************************/

int PRS_PERSONEL_FORMU::SICIL_NO_UNIQUE_MI ( int personel_id, QString sicil_no )
{
    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id ", " sicil_no = :sicil_no "
                             "AND personel_id != :personel_id");

    sql_query.SET_VALUE( ":sicil_no"    , sicil_no );
    sql_query.SET_VALUE( ":personel_id" , personel_id );

    if ( sql_query.SELECT() NE 0 ) {

        return ADAK_FAIL;
    }

    return ADAK_OK;
}


/**************************************************************************************/
/*         PRS_PERSONEL_FORMU::FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA      */
/**************************************************************************************/

void PRS_PERSONEL_FORMU::FIRMA_ODENEK_SECIMLERINI_PERSONELE_DEFAULT_UYGULA ( int yardim_var_mi_durumu, QCheckBox * check_box )
{
    if ( yardim_var_mi_durumu EQ 0 ) {

        check_box->setEnabled(true);
        check_box->setChecked(true);
    }
    else if ( yardim_var_mi_durumu EQ 1 ) {

        check_box->setEnabled(true);
        check_box->setChecked(false);
    }
    else if ( yardim_var_mi_durumu EQ 2 ) {

        check_box->setEnabled(false);
        check_box->setChecked(false);
    }
}

/*******************************************************************************/
/*           PRS_PERSONEL_FORMU::EVLI_DEGILSE_ES_BILGISINI_DISABLE_ET          */
/*******************************************************************************/

void PRS_PERSONEL_FORMU::EVLI_DEGILSE_ES_BILGISINI_DISABLE_ET ()
{

    if ( m_ui->combo_box_medeni_hal->currentIndex() NE PRS_EVLI ) {

        m_ui->label_esi_calisiyor_mu->setEnabled(false);
        m_ui->check_box_esi_calisiyor_mu->setChecked(false);
        m_ui->check_box_esi_calisiyor_mu->setEnabled(false);
        m_ui->label_uyari->setText( "<font style=\"font-size:8pt;\">""<font color = darkred>"
                                    + tr("* Eş Bilgilerinin Girilebilmesi İçin : "
                                    " Personelin Medeni Hali 'Evli' Olarak Kaydedilmelidir.") );
    }
    else if ( m_ui->combo_box_medeni_hal->currentIndex() EQ PRS_EVLI ) {

        m_ui->label_esi_calisiyor_mu->setEnabled(true);
        m_ui->check_box_esi_calisiyor_mu->setEnabled(true);
        m_ui->label_uyari->setText("");
    }
}


/***************************************************************************************
                   PRS_PERSONEL_FORMU::SET_WIDGETS_MAXIMUM_SIZE
 ***************************************************************************************/

void PRS_PERSONEL_FORMU ::SET_WIDGETS_MAXIMUM_SIZE ()
{

    m_ui->lineEdit_para_birimi->setMaxLength             (5);
    m_ui->lineEdit_para_birimi_1->setMaxLength           (5);
    m_ui->lineEdit_para_birimi_2->setMaxLength           (5);
    m_ui->line_edit_departman->setMaxLength              (50);
    m_ui->line_edit_muzun_oldugu_okul->setMaxLength      (40);
    m_ui->line_edit_ozel_indirim_dilekcesi->setMaxLength (512);
    m_ui->line_edit_tc_kimlik_no->setMaxLength           (11);
    m_ui->line_edit_seri_no->setMaxLength                (10);
    m_ui->line_edit_personel_adi->setMaxLength           (50);
   // m_ui->line_edit_personel_soyadi->setMaxLength        (50);
    m_ui->line_edit_baba_adi->setMaxLength               (50);
    m_ui->line_edit_anne_adi->setMaxLength               (50);
    m_ui->line_edit_din->setMaxLength                    (10);
    m_ui->line_edit_kayitli_oldugu_mahalle->setMaxLength (30);
    m_ui->line_edit_cilt_no->setMaxLength                (10);
    m_ui->line_edit_aile_sira_no->setMaxLength           (10);
    m_ui->line_edit_sira_no->setMaxLength                (10);
    m_ui->line_edit_verildigi_yer->setMaxLength          (30);
    m_ui->line_edit_verilis_nedeni->setMaxLength         (20);
    m_ui->line_edit_ilk_soyadi->setMaxLength             (30);
    m_ui->line_edit_sigorta_no->setMaxLength             (20);
    m_ui->line_edit_sicil_no->setMaxLength               (20);
    m_ui->line_edit_adres_satiri_1->setMaxLength         (35);
    m_ui->line_edit_adres_satiri_2->setMaxLength         (35);
    m_ui->line_edit_adres_satiri_3 ->setMaxLength        (35);
    m_ui->line_edit_posta_kodu->setMaxLength             (10);
    m_ui->line_edit_ev_telefonu->setMaxLength            (10);
    m_ui->line_edit_cep_telefonu->setMaxLength           (10);
    m_ui->line_edit_e_mail->setMaxLength                 (50);
    m_ui->line_edit_is_telefonu->setMaxLength            (10);
    m_ui->line_edit_semt->setMaxLength                   (30);
    m_ui->line_edit_kayit_no->setMaxLength               ( 5);
    m_ui->lineEdit_sube_adi->setMaxLength                (30);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_meslek                      ,30);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_unvan                       ,30);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_uyruk                       ,20);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_mezun_oldugu_bolum          ,40);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_yabanci_dil_1               ,20);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_yabanci_dil_2               ,20);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_cinsiyet                    ,10);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_dogum_yeri                  ,30);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_kan_grubu                   ,10);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_medeni_hal                  ,10);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_ogrenim_durumu              ,30);
   //SET_COMBOBOX_LENGTH(m_ui->combo_box_ucret_tipi                  ,20);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_ulke                        ,30);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_kayitli_oldugu_il           ,30);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_il                          ,30);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_gsm_kodlari                 ,10);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_ev_telefonunun_ulke_kodlari ,10);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_ev_telefonunun_il_kodlari   ,10);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_is_telefonunun_ulke_kodlari ,10);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_is_telefonunun_il_kodlari   ,10);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_cep_telefonunun_ulke_kodlari,10);
   SET_COMBOBOX_LENGTH(m_ui->combo_box_gsm_kodlari                 ,10);

   m_ui->commaEdit_haftalik_calisilan_gun_sayisi->SET_PRECISION( 1 );

}

/**************************************************************************************
                   PRS_PERSONEL_FORMU::ADRES_BILGILERINI_GOSTER
***************************************************************************************/

void PRS_PERSONEL_FORMU::ADRES_BILGILERINI_GOSTER()
{
    QString adres_line_1 = m_ui->line_edit_adres_satiri_1->text();
    QString adres_line_2 = m_ui->line_edit_adres_satiri_2->text();
    QString adres_line_3 = m_ui->line_edit_adres_satiri_3->text();
    QString adres_line_4 = "";

    if ( m_ui->line_edit_posta_kodu->text().isEmpty() EQ false ) {
        adres_line_3 += " "+ m_ui->line_edit_posta_kodu->text() + " " ;
    }
    if ( m_ui->line_edit_semt->text().isEmpty() EQ false ) {
        adres_line_3 += " "+ m_ui->line_edit_semt->text() + " " ;
    }
    if ( m_ui->combo_box_ilce->currentText().isEmpty() EQ false ) {
        adres_line_3 += " "+ m_ui->combo_box_ilce->currentText() + " " ;
    }
    if ( m_ui->combo_box_il->currentText().isEmpty() EQ false ) {
        adres_line_4 += m_ui->combo_box_il->currentText() + " ";
    }
    if ( m_ui->combo_box_ulke->currentText().isEmpty() EQ false ) {
        adres_line_4 += m_ui->combo_box_ulke->currentText() + " ";
    }

    m_ui->limitedTextEdit_adres->setText ( adres_line_1 + " \n" + adres_line_2 + " \n" + adres_line_3 + " \n" + adres_line_4 );
}




/**************************************************************************************
                          PRS_PERSONEL_FORMU::RESIM_SEC
***************************************************************************************/


int PRS_PERSONEL_FORMU::RESIM_SEC()
{
    ADAK_ADD_PIXMAP_TO_BUTTON(m_ui->push_button_resim,&m_fotograf_path,this);

   // m_ui->push_button_resim->setText("");

    if ( m_fotograf_path.isEmpty() EQ true ) {
        return ADAK_RECORD_UNCHANGED;
    }

    return ADAK_RECORD_CHANGED;


}



/**************************************************************************************

                   PRS_PERSONEL_FORMU::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int PRS_PERSONEL_FORMU::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{

    if ( p_button EQ m_ui->pushButton_grup_ekle ) {

        SQL_QUERY query ( DB );

        query.PREPARE_SELECT ( "adak_gruplar","grup_id");

        if ( query.SELECT() EQ 0 ) {
           MSG_WARNING( tr ( "Önce grupları tanımlamanız gerekmektedir." ), NULL );
            return ADAK_RECORD_UNCHANGED;
        }

        if ( OPEN_GRUP_SECIM( E9_PROGRAMI, PERSONEL_MODULU , &prs_grup_idleri , DB , this ) EQ true )
        {
            m_ui->textEdit_gruplar->setText ( GRP_GRUP_ADLARINI_BUL(&prs_grup_idleri , new QStringList() , new QStringList() ) );
            return ADAK_RECORD_CHANGED;
        }

        return ADAK_RECORD_UNCHANGED;
    }
    if( p_button EQ m_ui->push_button_resim )
    {
        return RESIM_SEC ();
    }

     return ADAK_RECORD_UNCHANGED;
}

/***************************************************************************************
                         PRS_PERSONEL_FORMU :: SEARCH_EDIT_CLICKED()
 ****************************************************************************************/

void PRS_PERSONEL_FORMU::SEARCH_EDIT_CLICKED (QWidget *widget , QLineEdit * line_edit)
{
    if ( widget EQ m_ui->searchEdit_personel_avanslari_hesap_kodu ) {

        int muh_hesap_id = OPEN_MUH_HESAP_ARAMA("",this,true);

        if ( muh_hesap_id > 0 ) {
            line_edit->setText(  MUH_GET_HESAP_KODU ( muh_hesap_id ) );
        }
    }

    for ( int i = 0; i < TANIMLANACAK_ENT_HESAP_SAYISI; i++ ) {
        if( widget EQ M_SEARCH_EDIT_ENT_ARRAY[ i ] ) {
            int personel_muh_hesap_id = OPEN_MUH_HESAP_ARAMA("" , this, true);

            if ( personel_muh_hesap_id > 0 ) {
                if ( widget EQ m_ui->searchEdit_personele_borclar_hesap_kodu ) {
                    line_edit->setText ( MUH_GET_HESAP_KODU ( personel_muh_hesap_id ) );
                }
                else {
                    line_edit->setText( MUH_GET_HESAP_KODU( personel_muh_hesap_id ) );
                }
            }

        }
    }
    if (widget EQ m_ui->searchEdit_sube){
        int sube_id = SUBE_SEC (this );

        m_sube_id = sube_id;
        if(sube_id NE 0) {

            QString sube_kodu, sube_adi;
            SUBE_GET_SUBE_KODU_ADI( sube_id, &sube_kodu, &sube_adi );

            line_edit->setText              ( sube_kodu );
            m_ui->lineEdit_sube_adi->setText( sube_adi  );
        }
    }
}

/***************************************************************************************
                         PRS_PERSONEL_FORMU ::WIDGETLARI_DUZENLE()
 ****************************************************************************************/

void PRS_PERSONEL_FORMU::WIDGETLARI_DUZENLE()
{
    M_SEARCH_EDIT_ENT_ARRAY         = new QSearchEdit * [ TANIMLANACAK_ENT_HESAP_SAYISI ];
    M_LINE_EDIT_ENT_ARRAY           = new QLineEdit   * [ TANIMLANACAK_ENT_HESAP_SAYISI ];

    M_ENT_HESAPLARI_ID_ARRAY        =  new int [ TANIMLANACAK_ENT_HESAP_SAYISI ];

    M_SEARCH_EDIT_ENT_ARRAY[0]      = m_ui->searchEdit_personel_avanslari_hesap_kodu;
    M_SEARCH_EDIT_ENT_ARRAY[1]      = m_ui->searchEdit_personele_borclar_hesap_kodu;

    M_LINE_EDIT_ENT_ARRAY  [0]     = m_ui->lineEdit_personel_avanslari_hesap_ismi;
    M_LINE_EDIT_ENT_ARRAY  [1]     = m_ui->lineEdit_personele_borclar_hesap_ismi;

}
