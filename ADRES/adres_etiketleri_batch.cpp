#include <QMessageBox>
#include "adak_utils.h"
#include "print.h"
#include "adres_etiketleri_batch_class.h"
#include "adres_etiketleri_batch_open.h"
#include "ui_adres_etiketleri_batch.h"
#include "yonetim.h"
#include "adres_enum.h"
#include "e9_log.h"
#include "adres_etiketleri_open.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_ADRES_ETIKETLERI_BATCH
***************************************************************************************/

void OPEN_ADRES_ETIKETLERI_BATCH ( QStringList P_ADRES_ETIKET_BILGILERI, QWidget * parent )
{
    ADRES_ETIKETLERI_BATCH * B = new ADRES_ETIKETLERI_BATCH ( P_ADRES_ETIKET_BILGILERI, parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   ADRES_ETIKETLERI_BATCH::ADRES_ETIKETLERI_BATCH
***************************************************************************************/

ADRES_ETIKETLERI_BATCH::ADRES_ETIKETLERI_BATCH ( QStringList P_ADRES_ETIKET_BILGILERI, QWidget * parent ) :  BATCH_KERNEL (parent), m_ui(new Ui::ADRES_ETIKETLERI_BATCH)
{
    M_ADRES_ETIKET_BILGILERI = P_ADRES_ETIKET_BILGILERI;

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this,DB );
}

/**************************************************************************************
                    ADRES_ETIKETLERI_BATCH::CHECK_VAR
***************************************************************************************/

void ADRES_ETIKETLERI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE                ("adres-toplu-mail-smsm_adres-etiketleri-yazdir");
    SET_PAGE_TITLE               ( tr ( "ADR - ADRES ETİKETLERİ" ) );

    SET_SETTING_NAME             ( "ADRES_ETIKETLERI_BATCH" );
    REGISTER_BUTTON_WIDGET       ( m_ui->widget_batch_button );

    sablonlar = GET_ETIKET_SABLONLARI();

    m_ui->combo_box_sablonlar->addItem (tr ( "Kullanıcı Tanımlı" ) );

    for ( int i = 0; i < sablonlar.size(); i++ ) {
        m_ui->combo_box_sablonlar->addItem ( sablonlar.at ( i ).name );
    }

    m_ui->comma_edit_satirdaki_etiket_sayisi->VIRGULSUZ_CALIS();
    m_ui->comma_edit_satir_sayisi->VIRGULSUZ_CALIS();

    m_ui->comma_edit_etiketler_arasi_bosluk->SET_RETURN_EMPTY   ( false );
    m_ui->comma_edit_etiket_genisligi->SET_RETURN_EMPTY         ( false );
    m_ui->comma_edit_etiket_yukseligi->SET_RETURN_EMPTY         ( false );
    m_ui->comma_edit_satirdaki_etiket_sayisi->SET_RETURN_EMPTY  ( false );
    m_ui->comma_edit_satirlar_arasi_bosluk->SET_RETURN_EMPTY    ( false );
    m_ui->comma_edit_satir_sayisi->SET_RETURN_EMPTY             ( false );
    m_ui->comma_edit_sol_bosluk->SET_RETURN_EMPTY               ( false );
    m_ui->comma_edit_ust_bosluk->SET_RETURN_EMPTY               ( false );

    SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Etiketleri Oluştur" ) );

    SET_ENTER_KEY_FOR_RUN_BATCH  ( true );

    SET_FIRST_FOCUS_WIDGET       ( m_ui->combo_box_sablonlar );
}

/**************************************************************************************
                    ADRES_ETIKETLERI_BATCH::CHECK_VAR
***************************************************************************************/

int ADRES_ETIKETLERI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->combo_box_sablonlar ) {
        int selected_index = m_ui->combo_box_sablonlar->currentIndex();

        if ( selected_index EQ 0 ) {
            m_ui->comma_edit_etiketler_arasi_bosluk->SET_DOUBLE   ( 0.00 );
            m_ui->comma_edit_etiket_yukseligi->SET_DOUBLE         ( 0.00 );
            m_ui->comma_edit_etiket_genisligi->SET_DOUBLE         ( 0.00 );
            m_ui->comma_edit_satirdaki_etiket_sayisi->SET_INTEGER ( 0 );
            m_ui->comma_edit_satirlar_arasi_bosluk->SET_DOUBLE    ( 0.00 );
            m_ui->comma_edit_satir_sayisi->SET_INTEGER            ( 0 );
            m_ui->comma_edit_sol_bosluk->SET_DOUBLE               ( 0.00 );
            m_ui->comma_edit_ust_bosluk->SET_DOUBLE               ( 0.00 );
            m_ui->push_button_etiket->setIcon(QIcon() );
        }
        else {
            m_ui->comma_edit_satirlar_arasi_bosluk->SET_DOUBLE           ( ROUND  (sablonlar.at ( selected_index  - 1 ).margin_right )   );
            m_ui->comma_edit_etiket_yukseligi->SET_DOUBLE                ( ROUND  ( sablonlar.at ( selected_index - 1 ).etiket_height)   );
            m_ui->comma_edit_etiket_genisligi->SET_DOUBLE                ( ROUND  ( sablonlar.at ( selected_index - 1 ).etiket_width )   );
            m_ui->comma_edit_satirdaki_etiket_sayisi->SET_INTEGER        ( sablonlar.at ( selected_index - 1 ).satirdaki_etiket_sayisi   );
            m_ui->comma_edit_satirlar_arasi_bosluk->SET_DOUBLE           ( ROUND ( sablonlar.at ( selected_index - 1 ).margin_bottom )   );
            m_ui->comma_edit_satir_sayisi->SET_INTEGER                   ( sablonlar.at(selected_index - 1).sayfadaki_satir_sayisi       );
            m_ui->comma_edit_sol_bosluk->SET_DOUBLE                      ( ROUND(sablonlar.at(selected_index - 1).page_left_space    )   );
            m_ui->comma_edit_ust_bosluk->SET_DOUBLE                      ( ROUND(sablonlar.at(selected_index - 1).page_top_space     )   );
            m_ui->push_button_etiket->setIcon ( QIcon ( QString          ( ":/e9_icons/E9_ICONS/etiketler/%1" ).arg ( sablonlar.at ( selected_index - 1 ).icon_path ) ) );
            //Etiket icin kullanilan push button in boyutlari 65x87
            m_ui->push_button_etiket->setIconSize ( QSize ( 65,87 ) );
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_ETIKETLERI_BATCH::CHECK_RUN
***************************************************************************************/

int ADRES_ETIKETLERI_BATCH::CHECK_RUN()
{
    if ( m_ui->combo_box_sablonlar->currentIndex() EQ 0 ) {
        if ( m_ui->comma_edit_etiketler_arasi_bosluk->GET_DOUBLE() EQ 0.0 ) {
            MSG_WARNING( tr ( "Etiketler arası boşluk boş bırakılamaz." ), m_ui->comma_edit_etiketler_arasi_bosluk   );
            return ADAK_FAIL;
        }
        if ( m_ui->comma_edit_etiket_genisligi->GET_DOUBLE() EQ 0.0 ) {
            MSG_WARNING( tr( "Etiket genişliği boş bırakılamaz." ) , m_ui->comma_edit_etiket_genisligi );
            return ADAK_FAIL;
        }
        if ( m_ui->comma_edit_satirdaki_etiket_sayisi->GET_INTEGER() EQ 0 ) {
            MSG_WARNING( tr ( "Etiket sayısı boş bırakılamaz." ), m_ui-> comma_edit_satirdaki_etiket_sayisi );
            return ADAK_FAIL;
        }
        if ( m_ui->comma_edit_etiket_yukseligi->GET_DOUBLE() EQ 0.0 ) {
            MSG_WARNING( tr ( "Etiket yüksekliği boş bırakılamaz." ), m_ui->comma_edit_etiket_yukseligi );
            return ADAK_FAIL;
        }
        if ( m_ui->comma_edit_satirlar_arasi_bosluk->GET_DOUBLE() EQ 0.0 ) {
            MSG_WARNING( tr ( "Satırlar arası boşluk boş bırakılamaz." ), m_ui->comma_edit_satirlar_arasi_bosluk );
            return ADAK_FAIL;
        }
        if ( m_ui->comma_edit_satir_sayisi->GET_INTEGER() EQ 0.0 ) {
            MSG_WARNING( tr ( "Satır sayısı boş bırakılamaz." ), m_ui->comma_edit_satir_sayisi );
            return ADAK_FAIL;
        }
        if ( m_ui->comma_edit_sol_bosluk->GET_DOUBLE() EQ 0.0 ) {
            MSG_WARNING( tr ( "Sol boşluk boş bırakılamaz." ), m_ui->comma_edit_sol_bosluk );
            return ADAK_FAIL;
        }
        if ( m_ui->comma_edit_ust_bosluk->GET_DOUBLE() EQ 0.0 ) {
           MSG_WARNING( tr("Üst boşluk boş bırakılamaz."), m_ui->comma_edit_ust_bosluk );
           return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_ETIKETLERI_BATCH::RUN_BATCH
***************************************************************************************/

void ADRES_ETIKETLERI_BATCH::RUN_BATCH()
{

    int selected_index = m_ui->combo_box_sablonlar->currentIndex();

    if ( selected_index EQ 0 ) {

        etiket_sablonlari etiket_sablonu;

        etiket_sablonu.margin_right             =   m_ui->comma_edit_etiketler_arasi_bosluk->GET_DOUBLE();
        etiket_sablonu.etiket_height            =   m_ui->comma_edit_etiket_yukseligi->GET_DOUBLE();
        etiket_sablonu.etiket_width             =   m_ui->comma_edit_etiket_genisligi->GET_DOUBLE();
        etiket_sablonu.satirdaki_etiket_sayisi  =   m_ui->comma_edit_satirdaki_etiket_sayisi->GET_INTEGER();
        etiket_sablonu.margin_bottom            =   m_ui->comma_edit_satirlar_arasi_bosluk->GET_DOUBLE();
        etiket_sablonu.sayfadaki_satir_sayisi   =   m_ui->comma_edit_satir_sayisi->GET_INTEGER();
        etiket_sablonu.page_left_space          =   m_ui->comma_edit_sol_bosluk->GET_DOUBLE();
        etiket_sablonu.page_top_space           =   m_ui->comma_edit_ust_bosluk->GET_DOUBLE();

        OPEN_REPORT_SHOWER ( ( GET_ADRES_ETIKETLERI( M_ADRES_ETIKET_BILGILERI,etiket_sablonu,m_ui->line_edit_yazi_tipi_boyutu->text().toFloat() ) ),nativeParentWidget(), QPrinter::Portrait, ( m_ui->comma_edit_sol_bosluk->GET_DOUBLE() ),(m_ui->comma_edit_ust_bosluk->GET_DOUBLE() ),0.0,0.0);
    }
    else {
        OPEN_REPORT_SHOWER ( GET_ADRES_ETIKETLERI (M_ADRES_ETIKET_BILGILERI, sablonlar.at(selected_index - 1),m_ui->line_edit_yazi_tipi_boyutu->text().toFloat()),
                           nativeParentWidget(), QPrinter::Portrait, (sablonlar.at ( selected_index - 1 ).page_left_space),
                           (sablonlar.at(selected_index - 1 ).page_top_space ),0.0,0.0 );
    }
    QString log_detaylari;
    log_detaylari.append ( " Log Türü: ETİKET YAZDIRMA " );

    DB->START_TRANSACTION();
    E9_LOG_KAYDI_EKLE ( ADRES_MODULU, LOG_ADR_ETIKET_YAZDIRMA, -1, log_detaylari);
    DB->COMMIT_TRANSACTION();
}


