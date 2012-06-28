#include "adak_utils.h"
#include "muh_defteri_kebir_batch_class.h"
#include "muh_defteri_kebir_batch_open.h"
#include "ui_muh_defteri_kebir_batch.h"
#include "print.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_enum.h"
#include "e9_log.h"
#include "e9_console_utils.h"
#include "muh_hesap_arama_open.h"
#include "muh_defteri_kebir_raporu_open.h"
#include "e9_gui_utils.h"

extern ADAK_SQL *     DB;

 /*************************************************************************************
                   OPEN_MUH_DEFTERI_KEBIR_BATCH
***************************************************************************************/

void OPEN_MUH_DEFTERI_KEBIR_BATCH ( QWidget * parent )
{
    MUH_DEFTERI_KEBIR_BATCH * B = new MUH_DEFTERI_KEBIR_BATCH ( parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   MUH_DEFTERI_KEBIR_BATCH::MUH_DEFTERI_KEBIR_BATCH
***************************************************************************************/

MUH_DEFTERI_KEBIR_BATCH::MUH_DEFTERI_KEBIR_BATCH ( QWidget *parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::MUH_DEFTERI_KEBIR_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   MUH_DEFTERI_KEBIR_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_DEFTERI_KEBIR_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE    ( "genel-muhasebe_defteri-kebir" );
    SET_PAGE_TITLE   ( tr ( "MUH - DEFTER-İ KEBİR" ) );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    m_ui->adakDate_baslangic_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bitis_tarihi->SET_DATE    ( MALI_YIL_365_DATE() );

    m_ui->comma_edit_yazitipi_boyutu->SET_INTEGER     ( 11 );
    m_ui->comma_edit_yazitipi_boyutu->VIRGULSUZ_CALIS ( true );
    m_ui->comma_edit_yazitipi_boyutu->SET_PRECISION   ( 0 );

    SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Rapor Al" ) );
    SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_baslangic_tarihi );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" ,"parent_id = :parent_id");
    sql_query.SET_VALUE(":parent_id" , 0);

    if ( sql_query.SELECT("tam_hesap_kodu ASC" , 0 , 1) > 0 ) {
        sql_query.NEXT();
        m_ui->search_edit_bas_hesabi->SET_TEXT ( sql_query.VALUE(0).toString() );
        m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(1).toString() );

    }
    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" , "parent_id = :parent_id");
    sql_query.SET_VALUE(":parent_id" , 0);

    if (sql_query.SELECT("tam_hesap_kodu DESC" , 0 , 1) > 0 ) {
        sql_query.NEXT();
        m_ui->search_edit_bts_hesabi->SET_TEXT ( sql_query.VALUE(0).toString() );
        m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(1).toString() );
    }

    FILL_PAPERSIZE_COMBOBOX (m_ui->combo_box_sayfa_boyutu);
    m_ui->comma_edit_genisligi->SET_DOUBLE(210.00);
    m_ui->comma_edit_yuksekligi->SET_DOUBLE(297.00);

    SET_SETTING_NAME                                ( "MUH_DEFTERI_KEBIR_BATCH" );
    DATE_FRAME_AYARLA                               ();
    m_ui->comma_edit_yazitipi_boyutu->SET_PRECISION ( 0 );
}

/**************************************************************************************
                   MUH_DEFTERI_KEBIR_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_DEFTERI_KEBIR_BATCH::CHECK_VAR ( QObject * object )
{
    SQL_QUERY   sql_query ( DB );

    if ( object EQ m_ui->checkbox_hesap_araligi ) {

        if ( m_ui->checkbox_hesap_araligi->isChecked() EQ false ) {

            sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" , "parent_id = :parent_id");
            sql_query.SET_VALUE(":parent_id" , 0);

            if ( sql_query.SELECT("tam_hesap_kodu ASC" , 0 , 1) > 0 ) {
                sql_query.NEXT();
                m_ui->search_edit_bas_hesabi->SET_TEXT ( sql_query.VALUE(0).toString() );
                m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(1).toString() );

            }


            sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" , "parent_id = :parent_id");
            sql_query.SET_VALUE(":parent_id" , 0);

            if ( sql_query.SELECT("tam_hesap_kodu DESC" , 0 , 1) > 0 ) {
                 sql_query.NEXT();
                m_ui->search_edit_bts_hesabi->SET_TEXT ( sql_query.VALUE(0).toString() );
                m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(1).toString() );
            }
            m_ui->frame_hesap_araligi->setDisabled ( true );
        }
        else {
            m_ui->frame_hesap_araligi->setEnabled ( true );
            SET_FOCUS( m_ui->search_edit_bas_hesabi );
        }
    }
    else if ( object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ false ) {
            m_ui->frame_tarih_araligi->setDisabled ( true );

            m_ui->adakDate_baslangic_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
            m_ui->adakDate_bitis_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());

        }
        else {
            m_ui->frame_tarih_araligi->setEnabled ( true );
            SET_FOCUS( m_ui->adakDate_baslangic_tarihi);
        }
    }
    else if (object EQ m_ui->combo_box_sayfa_boyutu) {
        int page_id = GET_PAPERSIZE_ENUM ( m_ui->combo_box_sayfa_boyutu->currentText() );
        QPrinter printer;
        printer.setPaperSize((QPrinter::PaperSize)page_id);
        QSizeF size = printer.paperSize(QPrinter::Millimeter);
        m_ui->comma_edit_genisligi->SET_DOUBLE(size.width());
        m_ui->comma_edit_yuksekligi->SET_DOUBLE(size.height());
    }
    else if (object EQ m_ui->comma_edit_genisligi OR m_ui->comma_edit_yuksekligi ) {
        m_ui->combo_box_sayfa_boyutu->setCurrentIndex(-1);
    }
    else if ( object EQ m_ui->adakDate_baslangic_tarihi OR object EQ m_ui->adakDate_bitis_tarihi ) {
        DATE_FRAME_AYARLA ( false );
    }
    else if ( object EQ m_ui->search_edit_bas_hesabi ) {

        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi" , "tam_hesap_kodu = :tam_hesap_kodu");
        sql_query.SET_VALUE      ( ":tam_hesap_kodu" , m_ui->search_edit_bas_hesabi->GET_TEXT() );

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr("Hesap kodu bulunamadı." ), m_ui->search_edit_bas_hesabi );

            return ADAK_FAIL;
        }
        sql_query.NEXT();

        m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(0).toString() );

    }
    else if ( object EQ m_ui->search_edit_bts_hesabi ) {

        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi" , "tam_hesap_kodu = :tam_hesap_kodu");
        sql_query.SET_VALUE      ( ":tam_hesap_kodu" , m_ui->search_edit_bts_hesabi->GET_TEXT() );

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr ( "Hesap kodu bulunamadı." ), m_ui->search_edit_bts_hesabi );

            return ADAK_FAIL;
        }
        sql_query.NEXT();

        m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_DEFTERI_KEBIR_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_DEFTERI_KEBIR_BATCH::CHECK_RUN()
{
    if ( m_ui->search_edit_bas_hesabi->GET_TEXT() > m_ui->search_edit_bts_hesabi->GET_TEXT() ) {
        MSG_WARNING( tr ( "Başlangıç hesap kodu ,bitiş hesap kodundan büyük olamaz." ), m_ui->search_edit_bas_hesabi );
        return ADAK_FAIL;
    }
    if ( m_ui->search_edit_bts_hesabi->GET_TEXT() < m_ui->search_edit_bas_hesabi->GET_TEXT() ) {
        MSG_WARNING(  tr ( "Bitiş hesap kodu başlangıç hesap kodundan küçük olamaz." ), m_ui->search_edit_bts_hesabi );
        return ADAK_FAIL;
    }
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_baslangic_tarihi, m_ui->adakDate_bitis_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_DEFTERI_KEBIR_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_DEFTERI_KEBIR_BATCH::RUN_BATCH()
{
    MUH_RAPOR_VARS * MUH_RV   = new MUH_RAPOR_VARS;

    MUH_RV->bas_hesap_kodu        = m_ui->search_edit_bas_hesabi->GET_TEXT();
    MUH_RV->bts_hesap_kodu        = m_ui->search_edit_bts_hesabi->GET_TEXT();
    MUH_RV->baslangic_tarihi      = m_ui->adakDate_baslangic_tarihi->QDATE();
    MUH_RV->bitis_tarihi          = m_ui->adakDate_bitis_tarihi->QDATE();
    MUH_RV->calismamis_hesaplar   = 0;
    MUH_RV->bakiyesiz_hesaplar    = true;
    MUH_RV->bas_gun_no            = m_ui->adakDate_baslangic_tarihi->MALI_YIL_ARRAY_INDIS();
    MUH_RV->bts_gun_no            = m_ui->adakDate_bitis_tarihi->MALI_YIL_ARRAY_INDIS();

    QString log_detaylari = QObject::tr ( "Defter-i Kebir Raporu , \nHesap Aralığı : " ) +
                            m_ui->search_edit_bas_hesabi->GET_TEXT() + " " + m_ui->lineedit_bas_hesap_ismi->text()
                            + " - " + m_ui->search_edit_bts_hesabi->GET_TEXT() + " " +
                            m_ui->lineedit_bts_hesap_ismi->text() + "\n" + QObject::tr ( "Tarih Aralığı    : " ) +
                            m_ui->adakDate_baslangic_tarihi->QDATE().toString("dd MMMM yyyy") + " - " +
                            m_ui->adakDate_bitis_tarihi->QDATE().toString("dd MMMM yyyy") + "\n";

    DB->START_TRANSACTION   ();
    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_RAPORLAR, LOG_ISLEM_RAPOR_ALINDI , log_detaylari ) ;
    DB->COMMIT_TRANSACTION  ();

    GET_DEFTERI_KEBIR_RAPORU ( m_ui->comma_edit_yazitipi_boyutu->GET_INTEGER() , m_ui->comma_edit_genisligi->GET_DOUBLE() , m_ui->comma_edit_yuksekligi->GET_DOUBLE() , MUH_RV);

    delete MUH_RV;
}

/**************************************************************************************
                   MUH_DEFTERI_KEBIR_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void MUH_DEFTERI_KEBIR_BATCH::SEARCH_EDIT_CLICKED ( QWidget * p_widget, QLineEdit * p_lineedit )
{
    Q_UNUSED ( p_lineedit );

    SQL_QUERY sql_query ( DB );

    if ( p_widget EQ m_ui->search_edit_bas_hesabi )  {

        int hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->search_edit_bas_hesabi->GET_TEXT(),this );

        if ( hesap_id EQ -1 ) {
            return;
        }

        sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" , "hesap_id = :hesap_id");
        sql_query.SET_VALUE      ( ":hesap_id" , hesap_id );

        if (sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
            m_ui->search_edit_bas_hesabi->SET_TEXT ( sql_query.VALUE(0).toString() );
            m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(1).toString() );
        }
    }
    else {
        int hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->search_edit_bts_hesabi->GET_TEXT(),this );

        if ( hesap_id EQ -1 ) {
            return;
        }
        sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" , "hesap_id = :hesap_id");
        sql_query.SET_VALUE      ( ":hesap_id" , hesap_id );

        if (sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
            m_ui->search_edit_bts_hesabi->SET_TEXT ( sql_query.VALUE(0).toString() );
            m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(1).toString() );
        }
    }
}

/**************************************************************************************
                   MUH_DEFTERI_KEBIR_BATCH::DATE_FRAME_AYARLA
***************************************************************************************/

void MUH_DEFTERI_KEBIR_BATCH::DATE_FRAME_AYARLA (  bool default_degerleri_ata )
{
    SQL_QUERY sql_query ( DB );

    if ( default_degerleri_ata EQ true ) {

        sql_query.PREPARE_SELECT("muh_fisler" , "fis_no,fis_tarihi" );


        if ( sql_query.SELECT("fis_tarihi ASC,fis_no ASC" , 0,1) > 0 ) {
            sql_query.NEXT();
            m_ui->line_edit_ilk_fis_no->setText ( sql_query.VALUE(0).toString() );
        }

       sql_query.PREPARE_SELECT("muh_fisler" , "fis_no,fis_tarihi" );

        if ( sql_query.SELECT("fis_tarihi DESC,fis_no DESC" , 0,1) > 0 ) {
            sql_query.NEXT();
            m_ui->line_edit_son_fis_no->setText ( sql_query.VALUE(0).toString() ) ;
        }
    }
    else {

        sql_query.PREPARE_SELECT("muh_fisler" , "fis_no" , "fis_tarihi = :fis_tarihi");

        sql_query.SET_VALUE            ( ":fis_tarihi" , m_ui->adakDate_baslangic_tarihi->DATE());

        if ( sql_query.SELECT("fis_tarihi ASC,fis_no ASC" , 0, 1) > 0 ) {
            sql_query.NEXT();

            m_ui->line_edit_ilk_fis_no->setText ( sql_query.VALUE(0).toString() );
        }


        sql_query.PREPARE_SELECT("muh_fisler" , "fis_no" , "fis_tarihi = :fis_tarihi");

        sql_query.SET_VALUE            ( ":fis_tarihi" , m_ui->adakDate_bitis_tarihi->DATE() );

        if ( sql_query.SELECT("fis_tarihi DESC,fis_no DESC" , 0, 1) > 0 ) {
            sql_query.NEXT();
            m_ui->line_edit_son_fis_no->setText( sql_query.VALUE(0).toString() );
        }
    }
}
