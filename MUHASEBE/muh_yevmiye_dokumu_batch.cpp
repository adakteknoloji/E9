#include "muh_yevmiye_dokumu_batch_class.h"
#include "muh_yevmiye_dokumu_batch_open.h"
#include "ui_muh_yevmiye_dokumu_batch.h"
#include "adak_utils.h"
#include "print.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_enum.h"
#include "e9_log.h"
#include "e9_console_utils.h"
#include "muh_fis_arama_open.h"
#include "muh_yevmiye_dokumu_raporu_open.h"
#include "e9_gui_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_MUH_YEVMIYE_DOKUMU_BATCH
***************************************************************************************/

void OPEN_MUH_YEVMIYE_DOKUMU_BATCH ( QWidget * parent )
{
    MUH_YEVMIYE_DOKUMU_BATCH * B = new MUH_YEVMIYE_DOKUMU_BATCH ( parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   MUH_YEVMIYE_DOKUMU_BATCH::MUH_YEVMIYE_DOKUMU_BATCH
***************************************************************************************/

MUH_YEVMIYE_DOKUMU_BATCH::MUH_YEVMIYE_DOKUMU_BATCH ( QWidget *parent ) : BATCH_KERNEL ( parent ),
m_ui ( new Ui::MUH_YEVMIYE_DOKUMU_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   MUH_YEVMIYE_DOKUMU_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_YEVMIYE_DOKUMU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE  ( "genel-muhasebe_yevmiye-dokumu" );
    SET_PAGE_TITLE ( tr ( "MUH - YEVMİYE DÖKÜMÜ" ) );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    m_ui->search_edit_baslangic_yevmiye_no->SET_TEXT ( "1" );
    m_ui->combobox_basim_turu->addItem ( tr ( "Deneme Baskısı" ) );
    m_ui->combobox_basim_turu->addItem ( tr ( "Resmi Baskı" ) );

    m_ui->adak_date_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adak_date_bts_tarihi->SET_DATE( MALI_YIL_365_DATE()  );

    DATE_FRAME_AYARLA();

    YEVMIYE_NO_FRAME_AYARLA();

    m_ui->frame_ozet->setDisabled ( true );
    m_ui->frame_yevmiye_no_araligi->setDisabled ( true );

    SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Başla" ) );
    SET_FIRST_FOCUS_WIDGET ( m_ui->adak_date_bas_tarihi );

    m_ui->frame_ozet->setDisabled                   ( true );
    m_ui->frame_yevmiye_no_araligi->setDisabled     ( true );

    FILL_PAPERSIZE_COMBOBOX                         ( m_ui->combo_box_sayfa_boyutu );
    m_ui->comma_edit_genisligi->SET_DOUBLE          ( 210.00 );
    m_ui->comma_edit_sayfa_yuksekligi->SET_DOUBLE   ( 297.00 );

    SET_SETTING_NAME        ( "MUH_YEVMIYE_DOKUMU_BATCH" );
}

/**************************************************************************************
                   MUH_YEVMIYE_DOKUMU_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_YEVMIYE_DOKUMU_BATCH::CHECK_VAR ( QObject * object )
{
    if (object EQ m_ui->combo_box_sayfa_boyutu) {
        int page_id = GET_PAPERSIZE_ENUM ( m_ui->combo_box_sayfa_boyutu->currentText() );
        QPrinter printer;
        printer.setPaperSize((QPrinter::PaperSize)page_id);
        QSizeF size = printer.paperSize(QPrinter::Millimeter);
        m_ui->comma_edit_genisligi->SET_DOUBLE(size.width());
        m_ui->comma_edit_sayfa_yuksekligi->SET_DOUBLE(size.height());
    }

    if (object EQ m_ui->comma_edit_genisligi OR m_ui->comma_edit_sayfa_yuksekligi ) {
        m_ui->combo_box_sayfa_boyutu->setCurrentIndex(-1);
    }

    if ( object EQ m_ui->adak_date_bas_tarihi OR object EQ m_ui->adak_date_bts_tarihi ) {
        DATE_FRAME_AYARLA ( false );
    }
    else if ( object EQ m_ui->search_edit_baslangic_yevmiye_no ) {
        QRegExp non_digit ( " ( \\D+ ) " );
        if ( m_ui->search_edit_baslangic_yevmiye_no->GET_TEXT().contains ( non_digit ) EQ true ) {
            MSG_WARNING( tr ( "Yevmiye numarasına rakam dışında bir giriş yapılamaz" ), m_ui->search_edit_baslangic_yevmiye_no );

            return ADAK_FAIL;
        }

        YEVMIYE_NO_FRAME_AYARLA ( false );
    }
    else if ( object EQ m_ui->search_edit_bitis_yevmiye_no  ) {

        QRegExp non_digit ( " (\\D+) " );

        if ( m_ui->search_edit_bitis_yevmiye_no->GET_TEXT().contains ( non_digit ) EQ true ) {
            MSG_WARNING( tr ( "Yevmiye numarasına rakam dışında bir giriş yapılamaz" ), m_ui->search_edit_bitis_yevmiye_no );

            return ADAK_FAIL;
        }

        YEVMIYE_NO_FRAME_AYARLA ( false );
    }

    if ( object EQ m_ui->checkbox_tarih_araligi ) {
        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ false ) {
             m_ui->frame_tarih_araligi->setDisabled ( true );
             m_ui->frame_yevmiye_no_araligi->setEnabled ( true );
             m_ui->checkbox_yvm_no_araligi->setChecked ( true );
             YEVMIYE_NO_FRAME_AYARLA();
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled ( true );
            m_ui->frame_yevmiye_no_araligi->setDisabled ( true );
            m_ui->checkbox_yvm_no_araligi->setChecked ( false );
            DATE_FRAME_AYARLA();
            SET_FOCUS( m_ui->adak_date_bas_tarihi );
        }
    }

    if ( object EQ m_ui->checkbox_sadece_ana_hesaplari_goster ) {
        if (m_ui->checkbox_sadece_ana_hesaplari_goster->isChecked() EQ false ) {
            m_ui->frame_ozet->setDisabled ( true );
            SET_FOCUS( m_ui->search_edit_baslangic_yevmiye_no );
        }
        else {
            m_ui->frame_ozet->setDisabled ( false );
        }
    }

    if ( object EQ m_ui->checkbox_yvm_no_araligi ) {
        if ( m_ui->checkbox_yvm_no_araligi->isChecked() EQ false ) {
             m_ui->search_edit_baslangic_yevmiye_no->SET_TEXT ( "1" );
             m_ui->search_edit_bitis_yevmiye_no->SET_TEXT ( "" );
            m_ui->frame_yevmiye_no_araligi->setDisabled ( true );
            m_ui->frame_tarih_araligi->setEnabled ( true );
            m_ui->checkbox_tarih_araligi->setChecked ( true );
            DATE_FRAME_AYARLA();
        }
        else {
            m_ui->frame_yevmiye_no_araligi->setEnabled ( true );
            m_ui->frame_tarih_araligi->setEnabled ( false );
            m_ui->checkbox_tarih_araligi->setChecked ( false);
            YEVMIYE_NO_FRAME_AYARLA();
            SET_FOCUS( m_ui->search_edit_baslangic_yevmiye_no );
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YEVMIYE_DOKUMU_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_YEVMIYE_DOKUMU_BATCH::CHECK_RUN()
{
    if ( m_ui->search_edit_baslangic_yevmiye_no->GET_TEXT().toInt() > m_ui->search_edit_bitis_yevmiye_no->GET_TEXT().toInt()) {
        if ( m_ui->search_edit_bitis_yevmiye_no->GET_TEXT().isEmpty() EQ true ) {
            return ADAK_FAIL;
        }
        MSG_WARNING( tr ( "Başlangıç yevmiye numarası bitiş yevmiye numarasından büyük olamaz"), m_ui->search_edit_baslangic_yevmiye_no );
        return ADAK_FAIL;
    }
    if ( m_ui->search_edit_bitis_yevmiye_no->GET_TEXT().toInt() < m_ui->search_edit_baslangic_yevmiye_no->GET_TEXT().toInt() ) {
        if ( m_ui->search_edit_baslangic_yevmiye_no->GET_TEXT().isEmpty() EQ true ) {
            return ADAK_OK;
        }
        MSG_WARNING( tr ( "Bitiş yevmiye numarası başlangıç yevmiye numarasından küçük olamaz" ), m_ui->search_edit_bitis_yevmiye_no );
        return ADAK_FAIL;
    }
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adak_date_bas_tarihi, m_ui->adak_date_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }
    if ( m_ui->frame_yevmiye_no_araligi->isEnabled() EQ true ) {
        if ( m_ui->search_edit_baslangic_yevmiye_no->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Başlangıç yevmiye numarası boş bırakılamaz" ), m_ui->search_edit_baslangic_yevmiye_no );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YEVMIYE_DOKUMU_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_YEVMIYE_DOKUMU_BATCH::RUN_BATCH()
{
    MUH_RAPOR_VARS * MUH_RV   = new MUH_RAPOR_VARS;

    MUH_RV->baslangic_tarihi = m_ui->adak_date_bas_tarihi->QDATE();
    MUH_RV->bitis_tarihi     = m_ui->adak_date_bts_tarihi->QDATE();
    MUH_RV->type_hesap_order = ENUM_ORDER_BORC_ALACAK;
    MUH_RV->deneme_baskisi   = true;

    if (m_ui->checkbox_yvm_no_araligi->isChecked() EQ true) {
        MUH_RV->baslangic_yevmiye_no = m_ui->search_edit_baslangic_yevmiye_no->GET_TEXT().toInt();
        MUH_RV->bitis_yevmiye_no     = m_ui->search_edit_bitis_yevmiye_no->GET_TEXT().toInt();
    }
    else {
        MUH_RV->baslangic_yevmiye_no = 0;
        MUH_RV->bitis_yevmiye_no     = -1;
    }

    MUH_RV->sadece_ana_hesaplar  = false;
    if ( m_ui->checkbox_sadece_ana_hesaplari_goster->isChecked() EQ true ) {
        MUH_RV->sadece_ana_hesaplar = true;
    }
    QString log_detaylari = QObject::tr ( "Yevmiye Dökümü , \n" ) +
                            QObject::tr ( "Tarih Aralığı    : " ) +
                            m_ui->adak_date_bas_tarihi->QDATE().toString ( "dd MMMM yyyy dddd" ) + " - " +
                            m_ui->adak_date_bts_tarihi->QDATE().toString ( "dd MMMM yyyy dddd" ) ;
    if ( MUH_RV->deneme_baskisi EQ true ) {
         log_detaylari.append ( "\n Deneme Baskısı" );
    }
    if ( MUH_RV->sadece_ana_hesaplar EQ true ) {
        if ( MUH_RV->deneme_baskisi EQ true ) {
              log_detaylari.append ( " , " );
        }
        log_detaylari.append ( "Sadece Ana Hesapları Göster" );
    }

    DB->START_TRANSACTION   ();
    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_RAPORLAR ,LOG_ISLEM_RAPOR_ALINDI ,  log_detaylari );
    DB->COMMIT_TRANSACTION  ();

    GET_YEVMIYE_DOKUMU_RAPORU ( m_ui->line_edit_yazi_tipi_boyutu->text().toDouble() , m_ui->comma_edit_genisligi->GET_DOUBLE() , m_ui->comma_edit_sayfa_yuksekligi->GET_DOUBLE() , MUH_RV );

    delete MUH_RV;
}

/**************************************************************************************
                   MUH_YEVMIYE_DOKUMU_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void MUH_YEVMIYE_DOKUMU_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget, QLineEdit * )
{
    int       fis_id    = OPEN_MUH_FIS_ARAMA ( 1 , MUHASEBE_MODULU, -1, ENUM_MUHASEBE_FISI, this );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "muh_fisler","yevmiye_numarasi","fis_id = :fis_id" );
    sql_query.SET_VALUE      ( ":fis_id" , fis_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    if ( widget EQ m_ui->search_edit_baslangic_yevmiye_no )  {
        m_ui->search_edit_baslangic_yevmiye_no->SET_TEXT ( sql_query.VALUE(0).toString() );
        YEVMIYE_NO_FRAME_AYARLA ( false );
    }
    else {
        m_ui->search_edit_bitis_yevmiye_no->SET_TEXT ( sql_query.VALUE(0).toString() );
        YEVMIYE_NO_FRAME_AYARLA ( false );
    }
}

/**************************************************************************************
      MUH_YEVMIYE_DOKUMU_BATCH::DATE_FRAME_AYARLA
***************************************************************************************/

void MUH_YEVMIYE_DOKUMU_BATCH::DATE_FRAME_AYARLA ( bool default_degerleri_ata )
{
    SQL_QUERY sql_query ( DB );

    if ( default_degerleri_ata EQ true ) {
        sql_query.PREPARE_SELECT("muh_fisler" , "fis_no,fis_tarihi");

        if ( sql_query.SELECT("fis_tarihi ASC,fis_no ASC" , 0 , 1) > 0 ) {
            sql_query.NEXT();
            m_ui->line_edit_ilk_fis_no->setText ( sql_query.VALUE(0).toString() );
        }

        sql_query.PREPARE_SELECT("muh_fisler" , "fis_no,fis_tarihi");


        if ( sql_query.SELECT("fis_tarihi DESC,fis_no DESC" , 0 , 1) > 0 ) {
            sql_query.NEXT();
            m_ui->line_edit_son_fis_no->setText ( sql_query.VALUE(0).toString() );
        }
    }
    else {

        sql_query.PREPARE_SELECT("muh_fisler" , "fis_no" , "fis_tarihi = :fis_tarihi");

        sql_query.SET_VALUE             (":fis_tarihi" , m_ui->adak_date_bas_tarihi->DATE() );

        if ( sql_query.SELECT("fis_tarihi ASC , fis_no ASC" , 0 , 1) > 0 ) {
            sql_query.NEXT();
            m_ui->line_edit_ilk_fis_no->setText ( sql_query.VALUE(0).toString() );
        }

        sql_query.PREPARE_SELECT("muh_fisler" , "fis_no" , "fis_tarihi = :fis_tarihi");
        sql_query.SET_VALUE             ( ":fis_tarihi" , m_ui->adak_date_bts_tarihi->DATE() );

        if ( sql_query.SELECT("fis_tarihi DESC , fis_no DESC" , 0 , 1) > 0 ) {
            sql_query.NEXT();
            m_ui->line_edit_son_fis_no->setText ( sql_query.VALUE(0).toString() );
        }
    }
}

/**************************************************************************************
                   MUH_YEVMIYE_DOKUMU_BATCH::YEVMIYE_NO_FRAME_AYARLA
***************************************************************************************/

void MUH_YEVMIYE_DOKUMU_BATCH::YEVMIYE_NO_FRAME_AYARLA ( bool default_degerleri_ata )
{
    SQL_QUERY sql_query ( DB );

    if ( default_degerleri_ata EQ true ) {

        sql_query.PREPARE_SELECT("muh_fisler" , "yevmiye_numarasi,fis_tarihi");

        if ( sql_query.SELECT("yevmiye_numarasi ASC" , 0 , 1) > 0 ) {
            sql_query.NEXT();
            m_ui->adak_date_fis_bas_tarihi->SET_DATE(sql_query.VALUE(1).toString());
            m_ui->search_edit_baslangic_yevmiye_no->SET_TEXT ( sql_query.VALUE(0).toString() );
        }

        sql_query.PREPARE_SELECT("muh_fisler" , "yevmiye_numarasi,fis_tarihi");
        if ( sql_query.SELECT("yevmiye_numarasi DESC" , 0,1) > 0 ) {
            sql_query.NEXT();
           m_ui->adak_date_fis_bts_tarihi->SET_DATE ( QDate::fromString ( sql_query.VALUE(1).toString(),"yyyy.MM.dd" ) );
            m_ui->search_edit_bitis_yevmiye_no->SET_TEXT(sql_query.VALUE(0).toString() );
        }
    }
    else {
        sql_query.PREPARE_SELECT ( "muh_fisler" , "fis_tarihi" , "yevmiye_numarasi=:yevmiye_numarasi");
        sql_query.SET_VALUE      (":yevmiye_numarasi" , m_ui->search_edit_baslangic_yevmiye_no->GET_TEXT() );

        if ( sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
            m_ui->adak_date_fis_bas_tarihi->SET_DATE( sql_query.VALUE(0).toString() );
        }

        sql_query.PREPARE_SELECT ( "muh_fisler" , "fis_tarihi" , "yevmiye_numarasi=:yevmiye_numarasi" );
        sql_query.SET_VALUE      ( ":yevmiye_numarasi",m_ui->search_edit_bitis_yevmiye_no->GET_TEXT() );
        if ( sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
           m_ui->adak_date_fis_bts_tarihi->SET_DATE ( sql_query.VALUE(0).toString()  );
        }
    }
}


