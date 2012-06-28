#include "ui_cek_acilis_fisi.h"
#include "cek_acilis_fisi_class.h"
#include "adak_sql.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "adak_std_utils.h"
#include "e9_enum.h"
#include "bnk_gui_utils.h"
#include "banka_struct.h"
#include "cari_console_utils.h"
#include "adak_utils.h"
#include "cek_enum.h"
#include "e9_log.h"
#include "cek_fis_utils.h"
#include "cek_senet_utils.h"
#include "cari_kart_arama_open.h"
#include "bnk_hesaplari_arama_open.h"
#include "cek_senet_bordrosu_arama_open.h"
 
#include "bnk_console_utils.h"
#include "cek_gui_utils.h"
#include "cek_console_utils.h"
#include "dvz_gui_utils.h"

extern ADAK_SQL *              DB;

#define ROW_ID_COLUMN                  0
#define FIS_ID_COLUMN                  1
#define ORDER_COLUMN                   2
#define CARI_HESAP_ID_COLUMN           3
#define VERILEN_CARI_HESAP_ID_COLUMN   4
#define CARI_HESAP_KODU_COLUMN         5
#define VERILEN_CARI_HESAP_KODU_COLUMN 6
#define CEK_SENET_NO_COLUMN            7
#define KESIDE_TARIHI_COLUMN           8
#define VADE_TARIHI_COLUMN             9
#define BANKA_ISMI_COLUMN              10
#define SUBE_ISMI_COLUMN               11
#define SUBE_KODU_COLUMN               12
#define HESAP_NO_COLUMN                13
#define CEK_SENET_TUTARI_COLUMN        14
#define DOVIZ_CINSI_COLUMN             15


/**************************************************************************************
                   OPEN_CEK_ACILIS_FISI
***************************************************************************************/

void OPEN_CEK_ACILIS_FISI ( int record_id, QWidget * parent )
{
    CEK_ACILIS_FISI * F = new CEK_ACILIS_FISI ( record_id, parent );
    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   CEK_ACILIS_FISI::CEK_ACILIS_FISI
***************************************************************************************/

CEK_ACILIS_FISI::CEK_ACILIS_FISI ( int record_id, QWidget * parent) : FIS_KERNEL ( parent ), m_ui ( new Ui::CEK_ACILIS_FISI )
{
    m_start_record_id = record_id;

    m_ui->setupUi    ( this );
    START_FIS_KERNEL ( this, DB );
}

/**************************************************************************************
                   CEK_ACILIS_FISI::SETUP_FORM();
***************************************************************************************/

void CEK_ACILIS_FISI::SETUP_FORM()
{
    REGISTER_BUTTONS_WIDGET      ( m_ui->navigation_button_widget );
    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH ( 512 );     

    m_ui->lineedit_bordro_no->setText("*");
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_bordro_no);

    M_BORDRO        = new CEK_BORDRO_STRUCT;

    M_BORDRO_SATIRI = new CEK_BORDRO_SATIRI_STRUCT;

    M_CEK_SENET     = new CEK_SENET_STRUCT;

    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI( m_ui->comboBox_banka_hesap_kodu );

    CEK_FILL_COMBO_BOX_BORDRO_ISLEM_TURU ( ENUM_CEK_SENET_ACILIS_BORDROSU, m_ui->comboBox_bordro_islem_turu  );

    m_cek_acilis_bordrosu_islem_turu = CEK_GET_CEK_SENET_ACILIS_BORDROSU_ISLEM_TURU_ENUM ( m_ui->comboBox_bordro_islem_turu->currentText() );

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_bordro_no->setValidator ( validator );

    QFont tableWidget_font = m_ui->tablewidget_fis_satirlari->font();
    int   font_point_size  = tableWidget_font.pointSize();
    tableWidget_font.setPointSize ( font_point_size -1 );
    m_ui->tablewidget_fis_satirlari->setFont ( tableWidget_font );

    REGISTER_TABLE_WIDGET        ( m_ui->tablewidget_fis_satirlari, 16 );

    SET_FIS_ORDER_COLUMN_NUMBER  ( ORDER_COLUMN  );
    SET_FORM_ID_COLUMN_POSITION  ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION   ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN  ( 5 );

    SET_FIS_ORDER_DATABASE       ( "cek_bordro_satirlari", "order_number", "bordro_satiri_id" );

    REGISTER_LINE_WIDGET         ( m_ui->lineEdit_cari_hesap_ismi         );
    REGISTER_LINE_WIDGET         ( m_ui->lineEdit_verilen_cari_hesap_ismi );

    SET_TABLE_ROW_WIDGETS ( CARI_HESAP_KODU_COLUMN          , WIDGET_SEARCH_EDIT    );
    SET_TABLE_ROW_WIDGETS ( CEK_SENET_NO_COLUMN             , WIDGET_LINE_EDIT      );
    SET_TABLE_ROW_WIDGETS ( KESIDE_TARIHI_COLUMN            , WIDGET_ADAK_DATE     );
    SET_TABLE_ROW_WIDGETS ( VADE_TARIHI_COLUMN              , WIDGET_ADAK_DATE     );
    SET_TABLE_ROW_WIDGETS ( BANKA_ISMI_COLUMN               , WIDGET_COMBO_BOX      );
    SET_TABLE_ROW_WIDGETS ( SUBE_KODU_COLUMN                , WIDGET_LINE_EDIT      );
    SET_TABLE_ROW_WIDGETS ( SUBE_ISMI_COLUMN                , WIDGET_COMBO_BOX      );
    SET_TABLE_ROW_WIDGETS ( HESAP_NO_COLUMN                 , WIDGET_LINE_EDIT      );
    SET_TABLE_ROW_WIDGETS ( DOVIZ_CINSI_COLUMN              , WIDGET_COMBO_BOX      );
    SET_TABLE_ROW_WIDGETS ( CEK_SENET_TUTARI_COLUMN         , WIDGET_COMMA_EDIT     );
    SET_TABLE_ROW_WIDGETS ( VERILEN_CARI_HESAP_KODU_COLUMN  , WIDGET_SEARCH_EDIT    );
    SET_TABLE_ROW_WIDGETS ( CARI_HESAP_ID_COLUMN            , WIDGET_LINE_EDIT      );
    SET_TABLE_ROW_WIDGETS ( VERILEN_CARI_HESAP_ID_COLUMN    , WIDGET_LINE_EDIT      );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_bordro_no );

    m_ui->adakDate_bordro_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );


    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_bordro_islem_turu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_banka_hesap_kodu );
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_fis_filtresi );
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_sirket_turu);

    m_ui->lineEdit_bnk_sube_bilgisi->setMaxLength(128);
    m_ui->lineedit_bordro_no->setMaxLength       (10);
    m_ui->lineEdit_cari_hesap_ismi->setMaxLength (60);
    m_ui->lineEdit_doviz_cinsi->setMaxLength     (5);
    m_ui->lineEdit_verilen_cari_hesap_ismi->setMaxLength(60);

    m_ui->comboBox_fis_filtresi->setHidden(true);
    m_ui->label_fis_filtresi->setHidden(true);

    m_ui->comboBox_sirket_turu->setHidden(true);
    m_ui->label_sirket->setHidden(true);

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ 1 ) {
        m_ui->comboBox_fis_filtresi->setHidden(false);
        m_ui->label_fis_filtresi->setHidden(false);

        m_ui->comboBox_sirket_turu->setHidden(false);
        m_ui->label_sirket->setHidden(false);
    }

    m_ui->adakDate_bordro_tarihi->setDisabled(true);


    m_ui->tablewidget_fis_satirlari->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_fis_satirlari->setFocus ( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET();

    AUTOSTART_FIS_KERNEL( m_start_record_id );
}

/**************************************************************************************
                   CEK_ACILIS_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void CEK_ACILIS_FISI::CLEAR_FORM_MEMBERS()
{
    m_frm_bnk_ismi.clear       ();
    m_frm_sube_ismi.clear      ();
    m_frm_sube_kodu.clear      ();

    m_log_kaydi_eklenmeli_mi    = false;
    m_kayit_eklendi             = false;
    m_kayit_silindi             = false;
    m_hesap_no_id               = 0;
    m_frm_bnk_doviz_id          = 0;

    M_BORDRO->CLEAR_BORDRO_STRUCT();

    M_BORDRO->modul_id   = CEKSENET_MODULU;
    M_BORDRO->program_id = E9_PROGRAMI;

}

/**************************************************************************************
                   CEK_ACILIS_FISI::NEW_FIS_RECORD
***************************************************************************************/

void CEK_ACILIS_FISI::NEW_FIS_RECORD()
{
    FIS_EKRANINI_DUZENLE();
    m_ui->lineedit_bordro_no->setText("*");
}

/**************************************************************************************
                   CEK_ACILIS_FISI::GET_FIS_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::GET_FIS_RECORD ( int record_id )
{


    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_no , bordro_tarihi,aciklama,bordro_islem_turu,"
                                  "hesap_no_id,toplam_tutar",
                                  "bordro_id = :bordro_id");
    select_query.SET_VALUE      ( ":bordro_id"  , record_id );

    if ( select_query.SELECT() EQ 0 ) {
         return ADAK_FAIL;
    }
    select_query.NEXT();

    m_ui->lineedit_bordro_no->setText               ( select_query.VALUE(0).toString() );
    m_ui->adakDate_bordro_tarihi->SET_DATE( select_query.VALUE(1).toString() );
    m_ui->limitedTextEdit_aciklama->setPlainText    ( select_query.VALUE(2).toString() );

    int bordro_islem_turu = select_query.VALUE(3).toInt();

    int hesap_no_id = select_query.VALUE(4).toInt();
    m_hesap_no_id = hesap_no_id;

    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( select_query.VALUE(5).toDouble() );

    switch(bordro_islem_turu) {
    case ENUM_CEK_SATICIYA_VERILDI :
        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
            if ( m_musteri_cek_senedi_mi EQ 1 ) {
                m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_SATICIYA_VER_MUS_CEK;
            }
            else {
                m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_SATICIYA_VER_FIRMA_CEK;
            }
        }
        else {
            if ( m_musteri_cek_senedi_mi EQ 1 ) {
                m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_SATICIYA_VER_MUS_SENET;
            }
            else {
                m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_SATICIYA_VER_FIRMA_SENET;
            }
        }
        break;
    case ENUM_CEK_BNK_TEMINATA_VERILDI:

        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_CEK;
        }
        else {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_SENET;
        }
        break;

    case ENUM_CEK_BNK_TAHSILATA_VERILDI:

        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_CEK;
        }
        else {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_SENET;
        }
        break;
    case ENUM_CEK_KARSILIKSIZ :
    case ENUM_CEK_PROTESTOLU  :

        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_KARSILIKSIZ_MUS_CEK;
        }
        else {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_PROTESTOLU_MUS_SENET;
        }
        break;

    case ENUM_CEK_SUPHELI_ALACAK:

        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_SUP_ALACAK_MUS_CEK;
        }
        else {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_SUP_ALACAK_MUS_SENET;
        }
        break;

    case ENUM_CEK_MUSTERIDEN_ALINDI :

        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_PORTFOYDE_MUS_CEK;
        }
        else {
            m_cek_acilis_bordrosu_islem_turu = ENUM_CEK_ACILIS_BRD_PORTFOYDE_MUS_SENET;
        }
        break;

    }
    FIS_EKRANINI_DUZENLE();

    //Bordro Satirlari

    select_query.PREPARE_SELECT ( "cek_cekler_senetler,cek_bordro_satirlari",
                                  "bordro_satiri_id,alinan_cari_hesap_id,verilen_cari_hesap_id,"
                                  "banka_ismi,sube_kodu,sube_ismi,hesap_no_id,cek_senet_numarasi,vade_tarihi,"
                                  "keside_tarihi,cek_senet_tutari,musteri_hesap_numarasi,doviz_id ",
                                  "bordro_id = :bordro_id AND "
                                  "cek_cekler_senetler.cek_senet_id = cek_bordro_satirlari.cek_senet_id " );
    select_query.SET_VALUE      ( ":bordro_id" , record_id );

    if ( select_query.SELECT(  "order_number ASC " ) EQ 0 ) {
         return ADAK_OK;
    }

    QTableWidgetItem * new_item;

    while  ( select_query.NEXT() ) {

        int current_row  = ADD_NEW_LINE();

        new_item = new QTableWidgetItem ( select_query.VALUE(0).toString() );
        m_ui->tablewidget_fis_satirlari->setItem(current_row , ROW_ID_COLUMN , new_item);

        new_item = new QTableWidgetItem ( QVariant ( record_id).toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row , FIS_ID_COLUMN , new_item );

        QComboBox   * combobox_line_banka_ismi      = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,BANKA_ISMI_COLUMN );
        QComboBox   * combobox_line_sube_ismi       = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,SUBE_ISMI_COLUMN );
        QLineEdit   * lineedit_line_sube_kodu       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,SUBE_KODU_COLUMN );
        QLineEdit   * lineedit_line_hesap_no        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,HESAP_NO_COLUMN ) ;
        QComboBox   * combobox_line_doviz_cinsi     = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,DOVIZ_CINSI_COLUMN ) ;
        QLineEdit   * lineedit_cek_senet_no         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,CEK_SENET_NO_COLUMN );
        QAdakDate   * adakDate_vade_tarihi        = ( QAdakDate   * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,VADE_TARIHI_COLUMN );
        QAdakDate   * adakDate_keside_tarihi        = ( QAdakDate   * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,KESIDE_TARIHI_COLUMN ) ;
        QCommaEdit  * commaedit_cek_senet_tutari      = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,CEK_SENET_TUTARI_COLUMN );
        QSearchEdit * searchedit_alinan_cari_hesap  = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,CARI_HESAP_KODU_COLUMN );
        QSearchEdit * searchedit_verilen_cari_hesap = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,VERILEN_CARI_HESAP_KODU_COLUMN );
        QLineEdit * lineedit_cari_hesap_id          = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,CARI_HESAP_ID_COLUMN);
        QLineEdit * lineedit_verilen_cari_hesap_id  = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,VERILEN_CARI_HESAP_ID_COLUMN);

        lineedit_cari_hesap_id->setText(select_query.VALUE(1).toString());
        lineedit_verilen_cari_hesap_id->setText(select_query.VALUE(2).toString());

        searchedit_alinan_cari_hesap->SET_TEXT ( CARI_FIND_HESAP_KODU(select_query.VALUE(1).toInt() ) );
        searchedit_verilen_cari_hesap->SET_TEXT ( CARI_FIND_HESAP_KODU(select_query.VALUE(2).toInt() ) );

        combobox_line_banka_ismi->setCurrentIndex (combobox_line_banka_ismi->findText(select_query.VALUE(3).toString()));

        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
            if (m_musteri_cek_senedi_mi EQ 1 ) {

                GET_BANKA_SUBE_ADLARI(combobox_line_sube_ismi,combobox_line_banka_ismi->currentText());

                combobox_line_sube_ismi->setCurrentIndex ( combobox_line_sube_ismi->findText(select_query.VALUE(5).toString()));

                lineedit_line_sube_kodu->setText(select_query.VALUE(4).toString() );
                lineedit_line_hesap_no->setText(select_query.VALUE(11).toString() );
            }

        }
        lineedit_cek_senet_no->setText ( select_query.VALUE(7).toString() );
        adakDate_vade_tarihi->SET_DATE( select_query.VALUE(8).toString()  );
        adakDate_keside_tarihi->SET_DATE( select_query.VALUE(9).toString()  );

        commaedit_cek_senet_tutari->SET_DOUBLE ( select_query.VALUE(10).toDouble() );

        combobox_line_doviz_cinsi->setCurrentIndex ( combobox_line_doviz_cinsi->findText(DVZ_GET_DOVIZ_KODU(select_query.VALUE(12).toInt())));

    }

    if ( bordro_islem_turu EQ ENUM_CEK_BNK_TAHSILATA_VERILDI OR bordro_islem_turu EQ ENUM_CEK_BNK_TEMINATA_VERILDI
         OR m_bordro_islem_turu EQ ENUM_CEK_SATICIYA_VERILDI ) {

        BNK_BANKA_BILGILERI_STRUCT BNK_BILGILER;

        BNK_GET_BANKA_TEMEL_BILGILER( &BNK_BILGILER, m_hesap_no_id );

        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(m_ui->comboBox_banka_hesap_kodu->findText( BNK_BILGILER.banka_hesap_kodu ));
        m_frm_bnk_doviz_id = BNK_BILGILER.doviz_id;
        m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(m_frm_bnk_doviz_id));
        m_frm_bnk_ismi  = BNK_BILGILER.banka_ismi;
        m_frm_sube_kodu = BNK_BILGILER.sube_kodu;
        m_frm_sube_ismi = BNK_BILGILER.sube_ismi;
        QString banka_bilgisi = m_frm_bnk_ismi + " / " + m_frm_sube_kodu + " " + m_frm_sube_ismi;
        m_ui->lineEdit_bnk_sube_bilgisi->setText(banka_bilgisi);

    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void CEK_ACILIS_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QAdakDate * adakDate_keside_tarihi = ( QAdakDate *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KESIDE_TARIHI_COLUMN);

    adakDate_keside_tarihi->MALI_YIL_KISITI_YOK( true );
    adakDate_keside_tarihi->SET_DATE( QDate::currentDate() );
    //dateedit_keside_tarihi->setDisplayFormat ( "dd MMMM yyyy" );

    //Acilis fisinde girilecek olan cek senedin keside tarihi ve vade tarihi muhasebe yili disindada olabilir.

    QAdakDate * adakDate_vade_tarihi = ( QAdakDate *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VADE_TARIHI_COLUMN);

    adakDate_vade_tarihi->MALI_YIL_KISITI_YOK( true );

    adakDate_vade_tarihi->SET_DATE( QDate::currentDate() );
    //dateedit_vade_tarihi->setDisplayFormat ( "dd MMMM yyyy" );

    QComboBox * combobox_line_banka_ismi = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BANKA_ISMI_COLUMN);
    QComboBox * combobox_doviz_cinsi     = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,DOVIZ_CINSI_COLUMN);

    DVZ_FILL_COMBOBOX ( combobox_doviz_cinsi );
    combobox_doviz_cinsi->setCurrentIndex(-1);

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        if ( m_musteri_cek_senedi_mi EQ 1 ) {
            GET_BANKALAR ( combobox_line_banka_ismi );
            combobox_line_banka_ismi->setCurrentIndex(-1);
        }
    }

    combobox_doviz_cinsi->setCurrentIndex ( combobox_doviz_cinsi->findText ( DVZ_GET_TEMEL_PARA_BIRIMI_KODU() ) );

    if ( m_hesap_no_id > 0 ) {
        if ( m_cek_acilis_bordrosu_islem_turu EQ ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_CEK OR
             m_cek_acilis_bordrosu_islem_turu EQ ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_SENET ) {
            combobox_doviz_cinsi->setDisabled(true);
            combobox_doviz_cinsi->setCurrentIndex(combobox_doviz_cinsi->findText(DVZ_GET_DOVIZ_KODU(m_frm_bnk_doviz_id)));
      }
    }

    QLineEdit * lineedit_cek_senet_no = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , CEK_SENET_NO_COLUMN);
    QLineEdit * lineedit_sube_kodu    = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , SUBE_KODU_COLUMN);
    QLineEdit * lineedit_hesap_no     = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_COLUMN);


    lineedit_cek_senet_no->setMaxLength         (20);
    lineedit_sube_kodu->setMaxLength            (20);
    lineedit_hesap_no->setMaxLength             (20);


}

/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( object  EQ m_ui->comboBox_banka_hesap_kodu ) {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("cek_bordro_satirlari",
                             "bordro_satiri_id ",
                             "bordro_id = :bordro_id");
        query.SET_VALUE(":bordro_id" , GET_RECORD_ID());

        if ( query.SELECT() > 0 ) {
            MSG_WARNING(  tr("Bordro satırlarında kayıt bulunmaktadır.Banka hesabını değiştiremezsiniz."), m_ui->comboBox_banka_hesap_kodu );
            return ADAK_FAIL;
        }

        m_ui->lineEdit_doviz_cinsi->clear();
        m_ui->lineEdit_bnk_sube_bilgisi->clear();
        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(-1);

        int row = -1;

        if (object EQ m_ui->comboBox_banka_hesap_kodu ) {
            row = m_ui->comboBox_banka_hesap_kodu->view()->currentIndex().row();
        }
        else {
            int bnk_hesap_no_id = OPEN_BNK_HESAPLARI_ARAMA(this);
            row = BNK_HESAPLARI_MODEL_INDEX_BUL(bnk_hesap_no_id,m_ui->comboBox_banka_hesap_kodu);

            if ( row NE -1 ) {
                m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(row);
            }
        }
        if ( row EQ -1 ) {
            return ADAK_FAIL;
        }
        m_hesap_no_id =BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,0).toInt();
        m_frm_bnk_doviz_id  = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,1).toInt();
        m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(m_frm_bnk_doviz_id));

        m_frm_bnk_ismi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,3).toString();

        m_frm_sube_kodu = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_banka_hesap_kodu,row,4).toString();

        m_frm_sube_ismi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,5).toString();


        QString banka_bilgisi = m_frm_bnk_ismi + " / " +  m_frm_sube_kodu + " " + m_frm_sube_ismi +
                                " " + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,6).toString();
        m_ui->lineEdit_bnk_sube_bilgisi->setText(banka_bilgisi);

        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(row);

        if ( m_cek_acilis_bordrosu_islem_turu EQ ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_CEK OR
             m_cek_acilis_bordrosu_islem_turu EQ ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_SENET ) {

            EKRANI_GUNCELLE();
        }
    }
    else if ( object EQ m_ui->comboBox_bordro_islem_turu ) {
        m_cek_acilis_bordrosu_islem_turu = CEK_GET_CEK_SENET_ACILIS_BORDROSU_ISLEM_TURU_ENUM ( m_ui->comboBox_bordro_islem_turu->currentText() );
        return ADAK_NEW_RECORD;
    }
    return ADAK_OK;    
}

/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineedit_bordro_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Yeni Açılış Bordrosu kaydı için (*) girmelisiniz!.."), m_ui->lineedit_bordro_no );

        return ADAK_FAIL;
    }

    if ( m_bordro_islem_turu EQ ENUM_CEK_BNK_TAHSILATA_VERILDI OR m_bordro_islem_turu EQ ENUM_CEK_BNK_TEMINATA_VERILDI ) {
        if ( m_ui->comboBox_banka_hesap_kodu->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr("Banka Hesap Kodunu Boş Bırakamazsınız!.."), m_ui->comboBox_banka_hesap_kodu);

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_FIS_RECORD_ADD()
{
    if ( QString ( m_ui->lineedit_bordro_no->text().at(0) ) NE "*" ) {
        int bordro_no = m_ui->lineedit_bordro_no->text().toInt();

        SQL_QUERY select_query ( DB );

        select_query.PREPARE_SELECT ( "cek_bordrolar",
                                      "bordro_id",
                                      "bordro_no = :bordro_no "
                                      "AND bordro_tarihi     = :bordro_tarihi");

        select_query.SET_VALUE       ( ":bordro_no"    , bordro_no );
        select_query.SET_VALUE       ( ":bordro_tarihi",  m_ui->adakDate_bordro_tarihi->DATE());

        if ( select_query.SELECT() NE 0 ) {
            MSG_WARNING( tr ( "Aynı bordro numarasıyla kayıtlı bordro bulundu.Lütfen bordro numarasını değiştiriniz!.." ), m_ui->lineedit_bordro_no );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::ADD_FIS_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::ADD_FIS_RECORD()
{
    int bordro_no = 0;

    if ( QString ( m_ui->lineedit_bordro_no->text().at(0 ) ) EQ "*" ) {
        bordro_no = CEK_SIRADAKI_BORDRO_NO_AL ( m_ui->adakDate_bordro_tarihi->DATE());
    }
    else {
        bordro_no = m_ui->lineedit_bordro_no->text().toInt();
    }

    M_BORDRO->program_id            = E9_PROGRAMI;
    M_BORDRO->modul_id              = CEKSENET_MODULU;
    M_BORDRO->bordro_no             = bordro_no;
    M_BORDRO->bordro_tarihi         = m_ui->adakDate_bordro_tarihi->DATE();
    M_BORDRO->bordro_islem_turu     = m_bordro_islem_turu;
    M_BORDRO->aciklama              = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_BORDRO->bordro_turu           = ENUM_CEK_SENET_ACILIS_BORDROSU;
    M_BORDRO->musteri_cek_senedi_mi = m_musteri_cek_senedi_mi;
    M_BORDRO->hesap_no_id           = m_hesap_no_id;
    M_BORDRO->cari_hesap_id         = 0;
    M_BORDRO->cari_para_birimi_id   = 0;
    M_BORDRO->bordro_para_birimi_id = 0;
    M_BORDRO->doviz_kuru            = 0;
    M_BORDRO->parite                = 0;
    M_BORDRO->base_fis_id           = 0;

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        M_BORDRO->cek_bordrosu_mu = 1;
    }
    else {
        M_BORDRO->cek_bordrosu_mu = 0;
    }


    int bordro_id = CEK_BORDROSU_EKLE(M_BORDRO);

    m_ui->lineedit_bordro_no->setText ( QVariant ( M_BORDRO->bordro_no ).toString() );

    m_log_detaylari = QObject::tr ( "Çek Senet Açılış Bordrosu , " ) + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + m_ui->adakDate_bordro_tarihi->QDATE().toString("dd MMMM YYYY") + " , "
                            "Bordro No :" + QVariant ( bordro_no ).toString() );

    m_kayit_eklendi = true;
    m_log_kaydi_eklenmeli_mi = true;


    return bordro_id;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id )
{
    SQL_QUERY select_query ( DB );

    if ( QString ( m_ui->lineedit_bordro_no->text().at(0) ) EQ "*" ) {
        MSG_WARNING( tr ( "Bordro numarasını güncellemek için bordro numarası kısmına sadece sayı girebilirsiniz."), m_ui->lineedit_bordro_no );

        return ADAK_FAIL;
    }

    int bordro_no = m_ui->lineedit_bordro_no->text().toInt();

    select_query.PREPARE_SELECT ( "cek_bordrolar",
                                  "bordro_id",
                                  "bordro_no = :bordro_no "
                                  "AND bordro_tarihi     = :bordro_tarihi");

    select_query.SET_VALUE      ( ":bordro_no"    ,  bordro_no);
    select_query.SET_VALUE      ( ":bordro_tarihi",  m_ui->adakDate_bordro_tarihi->DATE());

    if ( select_query.SELECT() > 0 ) {
        select_query.NEXT();
        if ( select_query.VALUE(0).toInt() NE record_id ) {
            MSG_WARNING( tr ( "Aynı bordro numarasıyla kayıtlı bordro bulundu.Lütfen bordro numarasını değiştiriniz!.."), m_ui->lineedit_bordro_no );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void CEK_ACILIS_FISI::UPDATE_FIS_RECORD ( int bordro_id )
{

    M_BORDRO->bordro_no        = m_ui->lineedit_bordro_no->text().toInt();
    M_BORDRO->aciklama         = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_BORDRO->hesap_no_id      = m_hesap_no_id;

    CEK_BORDRO_GUNCELLE(M_BORDRO , bordro_id);


    m_log_kaydi_eklenmeli_mi = true;

    m_log_detaylari = QObject::tr ( "Çek Senet Açılış Bordrosu , " ) + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + m_ui->adakDate_bordro_tarihi->QDATE().toString("dd MMMM YYYY") + " , "
                            "Bordro Numarası :" + m_ui->lineedit_bordro_no->text() + " , Toplam Tutar : " +
                            VIRGUL_EKLE ( QVariant( ROUND (M_BORDRO->toplam_tutar ) ).toString() , 2 ) );

}

/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_FIS_RECORD_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );


    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void CEK_ACILIS_FISI::DELETE_FIS_RECORD ( int bordro_id )
{
    CEK_BORDRO_SIL(M_BORDRO , bordro_id);

    m_log_kaydi_eklenmeli_mi = true;
    m_kayit_silindi          = true;

    m_log_detaylari = QObject::tr ( "Çek Senet Açılış Bordrosu , " ) + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu);
    m_log_detaylari.append ("\n" + m_ui->adakDate_bordro_tarihi->QDATE().toString("dd MMMM YYYY") + " , "
                            "Bordro No :" + M_BORDRO->bordro_no + " , Toplam Tutar : " +
                            VIRGUL_EKLE ( QVariant ( ROUND ( M_BORDRO->toplam_tutar ) ).toString() , 2 ) );

}

/**************************************************************************************
                   CEK_ACILIS_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::SELECT_FIS_RECORD()
{
    int cek_bordrosu_mu = 0;
    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
         cek_bordrosu_mu = 1;
    }


    return OPEN_CEK_SENET_BORDROSU_ARAMA ( CEKSENET_MODULU,ENUM_CEK_SENET_ACILIS_BORDROSU,m_bordro_islem_turu,
                                           cek_bordrosu_mu, ENUM_CEK_SENET_FISI, this );

}

/**************************************************************************************
                   CEK_ACILIS_FISI::FIND_FIS_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::FIND_FIS_RECORD()
{
    if ( m_ui->lineedit_bordro_no->text().toInt() EQ 0 ) {
         MSG_WARNING( tr ( "Aradığınız bordro numarasını , Bordro No alanına yazmalısınız!.."), m_ui->lineedit_bordro_no );

         return 0;
    }
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id",
                                  "bordro_no = :bordro_no "
                                  "AND bordro_turu           = :bordro_turu "
                                  "AND bordro_islem_turu     = :bordro_islem_turu "
                                  "AND musteri_cek_senedi_mi = :musteri_cek_senedi_mi "
                                  "AND cek_bordrosu_mu       = :cek_bordrosu_mu "
                                  "AND bordro_tarihi         = :bordro_tarihi" );

    select_query.SET_VALUE      ( ":bordro_no"       , m_ui->lineedit_bordro_no->text().toInt() );
    select_query.SET_VALUE      ( ":bordro_turu"           , ENUM_CEK_SENET_ACILIS_BORDROSU );
    select_query.SET_VALUE      ( ":bordro_islem_turu"     , m_bordro_islem_turu );
    select_query.SET_VALUE      ( ":musteri_cek_senedi_mi" , m_musteri_cek_senedi_mi );
    select_query.SET_VALUE      ( ":bordro_tarihi"         , m_ui->adakDate_bordro_tarihi->DATE());

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        select_query.SET_VALUE(":cek_bordrosu_mu" ,1 );
    }
    else {
        select_query.SET_VALUE(":cek_bordrosu_mu" ,0);
    }


    if ( select_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr ( "Aradığınız Bordro kayıtlarda bulunamadı." ), m_ui->lineedit_bordro_no );

        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CEK_ACILIS_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::FIND_FIRST_FIS_RECORD()
{

    SQL_QUERY select_query ( DB );

    QString query = QString ("bordro_turu               = :bordro_turu AND "
                             "bordro_islem_turu         = :bordro_islem_turu "
                             "AND musteri_cek_senedi_mi = :musteri_cek_senedi_mi "
                             "AND cek_bordrosu_mu       = :cek_bordrosu_mu ");

    select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query);

    select_query.SET_VALUE             ( ":bordro_turu"           , ENUM_CEK_SENET_ACILIS_BORDROSU );
    select_query.SET_VALUE             ( ":bordro_islem_turu"     , m_bordro_islem_turu       );
    select_query.SET_VALUE             ( ":musteri_cek_senedi_mi" , m_musteri_cek_senedi_mi   );


    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        select_query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
    }
    else {
        select_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
    }

    if ( select_query.SELECT( " bordro_tarihi, bordro_no",0 ,1 ) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CEK_ACILIS_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB ) ;

    QString query = QString ("bordro_turu               = :bordro_turu "
                             "AND bordro_islem_turu     = :bordro_islem_turu "
                             "AND musteri_cek_senedi_mi = :musteri_cek_senedi_mi "
                             "AND cek_bordrosu_mu       = :cek_bordrosu_mu ");

    select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query);

    select_query.SET_VALUE            ( ":bordro_turu"           , ENUM_CEK_SENET_ACILIS_BORDROSU );
    select_query.SET_VALUE            ( ":bordro_islem_turu"     , m_bordro_islem_turu );
    select_query.SET_VALUE            ( ":musteri_cek_senedi_mi" , m_musteri_cek_senedi_mi );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        select_query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
    }
    else {
        select_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
    }

    if ( select_query.SELECT( "bordro_tarihi DESC, bordro_no DESC", 0, 1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CEK_ACILIS_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString query = QString ("bordro_turu         = :bordro_turu "
                             "AND bordro_no       < :bordro_no "
                             "AND bordro_islem_turu     = :bordro_islem_turu "
                             "AND musteri_cek_senedi_mi = :musteri_cek_senedi_mi "
                             "AND cek_bordrosu_mu       = :cek_bordrosu_mu "
                             "AND bordro_tarihi         = :bordro_tarihi");

    select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query);

    select_query.SET_VALUE           ( ":bordro_turu"           , ENUM_CEK_SENET_ACILIS_BORDROSU );
    select_query.SET_VALUE           ( ":bordro_no"       , m_ui->lineedit_bordro_no->text().toInt() );
    select_query.SET_VALUE           ( ":bordro_islem_turu"     , m_bordro_islem_turu );
    select_query.SET_VALUE           ( ":musteri_cek_senedi_mi" , m_musteri_cek_senedi_mi );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        select_query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
    }
    else {
        select_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
    }

    select_query.SET_VALUE     ( ":bordro_tarihi"  , m_ui->adakDate_bordro_tarihi->DATE());


    if ( select_query.SELECT( " bordro_no DESC", 0, 1 ) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    query = QString ("bordro_turu               = :bordro_turu "
                     "AND bordro_islem_turu     = :bordro_islem_turu "
                     "AND musteri_cek_senedi_mi = :musteri_cek_senedi_mi "
                     "AND cek_bordrosu_mu       = :cek_bordrosu_mu "
                     "AND bordro_tarihi         < :bordro_tarihi");

    select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query);


    select_query.SET_VALUE            ( ":bordro_turu"           , ENUM_CEK_SENET_ACILIS_BORDROSU            );
    select_query.SET_VALUE            ( ":bordro_islem_turu"     , m_bordro_islem_turu                       );
    select_query.SET_VALUE            ( ":musteri_cek_senedi_mi" , m_musteri_cek_senedi_mi                   );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        select_query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
    }
    else {
        select_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
    }

    select_query.SET_VALUE     ( ":bordro_tarihi"  , m_ui->adakDate_bordro_tarihi->DATE());

    if ( select_query.SELECT( " bordro_tarihi DESC, bordro_no DESC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   CEK_ACILIS_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    QString query = QString ("bordro_turu         = :bordro_turu "
                             "AND bordro_no       > :bordro_no "
                             "AND bordro_islem_turu     = :bordro_islem_turu "
                             "AND musteri_cek_senedi_mi = :musteri_cek_senedi_mi "
                             "AND cek_bordrosu_mu       = :cek_bordrosu_mu "
                             "AND bordro_tarihi         = :bordro_tarihi");

    select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query);

    select_query.SET_VALUE           ( ":bordro_turu"           , ENUM_CEK_SENET_ACILIS_BORDROSU );
    select_query.SET_VALUE           ( ":bordro_no"       , m_ui->lineedit_bordro_no->text().toInt() );
    select_query.SET_VALUE           ( ":bordro_islem_turu"     , m_bordro_islem_turu );
    select_query.SET_VALUE           ( ":musteri_cek_senedi_mi" , m_musteri_cek_senedi_mi );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        select_query.SET_VALUE(":cek_bordrosu_mu" , 1);
    }
    else {
        select_query.SET_VALUE(":cek_bordrosu_mu" , 0);
    }

    select_query.SET_VALUE(":bordro_tarihi"  , m_ui->adakDate_bordro_tarihi->DATE());


    if ( select_query.SELECT( "bordro_no", 0, 1 ) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    query = QString ("bordro_turu         = :bordro_turu "
                     "AND bordro_islem_turu     = :bordro_islem_turu "
                     "AND musteri_cek_senedi_mi = :musteri_cek_senedi_mi "
                     "AND cek_bordrosu_mu       = :cek_bordrosu_mu "
                     "AND bordro_tarihi         > :bordro_tarihi");

    select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query);

    select_query.SET_VALUE           ( ":bordro_turu"           , ENUM_CEK_SENET_ACILIS_BORDROSU  );
    select_query.SET_VALUE           ( ":bordro_islem_turu"     , m_bordro_islem_turu      );
    select_query.SET_VALUE           ( ":musteri_cek_senedi_mi" , m_musteri_cek_senedi_mi  );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        select_query.SET_VALUE  ( ":cek_bordrosu_mu" , 1 );
    }
    else {
        select_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
    }

    select_query.SET_VALUE     ( ":bordro_tarihi"  , m_ui->adakDate_bordro_tarihi->DATE() );


    if ( select_query.SELECT( " bordro_tarihi, bordro_no", 0, 1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_LINE_VAR
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_LINE_VAR  ( int row_number, QObject * object )
{
    QSearchEdit * searchedit_cari_hesap_kodu         = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CARI_HESAP_KODU_COLUMN);
    QSearchEdit * searchedit_verilen_cari_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VERILEN_CARI_HESAP_KODU_COLUMN);
    QComboBox   * combobox_line_banka_ismi           = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BANKA_ISMI_COLUMN);
    QComboBox   * combobox_line_sube_ismi            = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,SUBE_ISMI_COLUMN);
    QLineEdit   * lineedit_line_sube_kodu            = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,SUBE_KODU_COLUMN);
    QLineEdit   * lineedit_line_hesap_no             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_NO_COLUMN);
    QComboBox   * combobox_line_doviz_cinsi          = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,DOVIZ_CINSI_COLUMN);
    QAdakDate   * dateedit_vade_tarihi               = ( QAdakDate   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VADE_TARIHI_COLUMN);
    QAdakDate   * dateedit_keside_tarihi             = ( QAdakDate   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KESIDE_TARIHI_COLUMN);
    QLineEdit * lineedit_cari_hesap_id               = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CARI_HESAP_ID_COLUMN);
    QLineEdit * lineedit_verilen_cari_hesap_id       = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VERILEN_CARI_HESAP_ID_COLUMN);

    SQL_QUERY select_query ( DB );

    if ( object EQ searchedit_cari_hesap_kodu ) {

        if ( searchedit_verilen_cari_hesap_kodu->GET_TEXT().isEmpty() EQ false ) {
            if ( searchedit_cari_hesap_kodu->GET_TEXT() EQ searchedit_verilen_cari_hesap_kodu->GET_TEXT()) {
                MSG_WARNING( tr ( "Çekin alındığı cari hesap ile verildiği cari hesap aynı olamaz!.."), searchedit_verilen_cari_hesap_kodu );

                return ADAK_FAIL;
            }
        }
        m_ui->label_cari_hesap_ismi->setHidden(false);
        m_ui->lineEdit_cari_hesap_ismi->setHidden(false);
        m_ui->lineEdit_cari_hesap_ismi->clear();

        QString cari_hesap_ismi;
        int     cari_hesap_id;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( searchedit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi );

        if ( result EQ 0 ) {
             MSG_WARNING( tr ( "Aradığınız cari hesap kodu bulunamadı!.."), searchedit_verilen_cari_hesap_kodu );

             return ADAK_FAIL;
        }
        m_ui->lineEdit_cari_hesap_ismi->setText ( cari_hesap_ismi );
        lineedit_cari_hesap_id->setText         ( QVariant ( cari_hesap_id ).toString() );

    }
    if ( object EQ searchedit_verilen_cari_hesap_kodu ) {
        if ( searchedit_verilen_cari_hesap_kodu->GET_TEXT().isEmpty() EQ false ) {
            if ( searchedit_cari_hesap_kodu->GET_TEXT() EQ searchedit_verilen_cari_hesap_kodu->GET_TEXT()) {
                 MSG_WARNING( tr ( "Çekin alındığı cari hesap ile verildiği cari hesap aynı olamaz!.."), searchedit_verilen_cari_hesap_kodu );

                 return ADAK_FAIL;
            }

            m_ui->label_verilen_cari_hesap_ismi->setHidden ( false );
            m_ui->lineEdit_verilen_cari_hesap_ismi->setHidden ( false );
            m_ui->lineEdit_verilen_cari_hesap_ismi->clear();

            QString cari_hesap_ismi;
            int     cari_hesap_id;

            int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( searchedit_verilen_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi );

            if ( result EQ 0 ) {
                MSG_WARNING( tr ( "Aradığınız cari hesap kodu bulunamadı!.."), searchedit_verilen_cari_hesap_kodu );

                return ADAK_FAIL;
            }
            m_ui->lineEdit_verilen_cari_hesap_ismi->setText( cari_hesap_ismi );
            lineedit_verilen_cari_hesap_id->setText        ( QVariant ( cari_hesap_id ).toString() );
        }
    }

    if ( object EQ combobox_line_banka_ismi ) {
         combobox_line_sube_ismi->clear();
         lineedit_line_sube_kodu->clear();
         lineedit_line_hesap_no->clear();
         combobox_line_doviz_cinsi->setCurrentIndex(-1);

        if ( m_cek_mi_senet_mi EQ ENUM_CEK) {

            GET_BANKA_SUBE_ADLARI(combobox_line_sube_ismi,combobox_line_banka_ismi->currentText());

        }
    }

    if ( object EQ combobox_line_sube_ismi ) {

        lineedit_line_sube_kodu->clear();
        lineedit_line_hesap_no->clear();
        combobox_line_doviz_cinsi->setCurrentIndex(-1);

        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
             lineedit_line_sube_kodu->setText ( GET_BANKA_SUBE_KODU( combobox_line_banka_ismi->currentText() , combobox_line_sube_ismi->currentText()));
        }
    }

    if ( object EQ lineedit_line_hesap_no ) {

        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
            if ( m_musteri_cek_senedi_mi EQ 0 ) {
                return ADAK_OK;
            }
            int bnk_para_birim_id = BNK_GET_PARA_BIRIM_ID(  BNK_GET_HESAP_NUMARASI_ID ( lineedit_line_hesap_no->text() ) );
            combobox_line_doviz_cinsi->setCurrentIndex ( combobox_line_doviz_cinsi->findText ( DVZ_GET_DOVIZ_KODU( bnk_para_birim_id )));
        }
    }
    if ( object EQ dateedit_vade_tarihi) {
        if ( dateedit_keside_tarihi->QDATE() > dateedit_vade_tarihi->QDATE() ) {
            MSG_WARNING( tr ( "Keşide tarihi,vade tarihinden büyük olamaz!.."), dateedit_vade_tarihi );

            return ADAK_FAIL;

        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QSearchEdit * searcedit_cari_hesap_kodu          = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CARI_HESAP_KODU_COLUMN);
    QComboBox   * combobox_banka_ismi                = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BANKA_ISMI_COLUMN);
    QComboBox   * combobox_sube_ismi                 = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,SUBE_ISMI_COLUMN);
    QLineEdit   * lineedit_hesap_no                  = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_NO_COLUMN);
    QCommaEdit  * commaedit_tutar                    = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CEK_SENET_TUTARI_COLUMN);
    QLineEdit   * lineedit_cek_senet_no              = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CEK_SENET_NO_COLUMN);
    QComboBox   * combobox_doviz_cinsi               = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,DOVIZ_CINSI_COLUMN);
    QSearchEdit * searchedit_verilen_cari_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VERILEN_CARI_HESAP_KODU_COLUMN);

    if( m_cek_mi_senet_mi EQ ENUM_CEK ) {

        if ( m_musteri_cek_senedi_mi EQ 1 ) {

            if ( combobox_banka_ismi->currentIndex() EQ -1 ) {
                MSG_WARNING( tr ( "Banka İsmini Seçmelisniz!.."), combobox_banka_ismi );

                return ADAK_FAIL;
            }
            if ( combobox_sube_ismi->currentIndex() EQ -1 ) {
                MSG_WARNING( tr ( "Şube ismini Seçmelisniz!.." ), combobox_sube_ismi );

                return ADAK_FAIL;
            }
            if ( lineedit_hesap_no->text().isEmpty() EQ true ) {
                MSG_WARNING( tr ( "Hesap Numarasını Seçmelisiniz!.." ), lineedit_hesap_no );

                return ADAK_FAIL;
            }
        }
    }

    if ( m_musteri_cek_senedi_mi EQ 1 ) {
        if ( searcedit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Cari Hesap Kodunu boş bırakamazsınız!.."), searcedit_cari_hesap_kodu );

            return ADAK_FAIL;
        }
    }

    if ( m_bordro_islem_turu EQ ENUM_CEK_SATICIYA_VERILDI ) {
        if ( searchedit_verilen_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr ( " Verilen Cari Hesap Kodunu boş bırakamazsınız!.."), searchedit_verilen_cari_hesap_kodu );

            return ADAK_FAIL;
        }
    }

    if ( lineedit_cek_senet_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Çek / Senet Numarasını Boş Bırakamazsınız!.."), lineedit_cek_senet_no );

        return ADAK_FAIL;
    }
    if ( commaedit_tutar->GET_DOUBLE() EQ 0.0 ) {
        MSG_WARNING( tr ( "Çek Senet Tutarını Girmelisiniz!.."), commaedit_tutar );

        return ADAK_FAIL;
    }
    if ( m_cek_mi_senet_mi EQ ENUM_SENET ) {
        if ( combobox_doviz_cinsi->currentIndex() EQ -1 ) {
            MSG_WARNING( tr ( "Para Birimini Seçmelisiniz!.."), combobox_doviz_cinsi );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}


/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_ADD_LINE
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::ADD_LINE
***************************************************************************************/

void CEK_ACILIS_FISI::ADD_LINE ( int bordro_id, int row_number )
{
    QComboBox   * combobox_line_banka_ismi      = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BANKA_ISMI_COLUMN);
    QComboBox   * combobox_line_sube_ismi       = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,SUBE_ISMI_COLUMN);
    QLineEdit   * lineedit_line_sube_kodu       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,SUBE_KODU_COLUMN);
    QLineEdit   * lineedit_line_hesap_no        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_NO_COLUMN);
    QComboBox   * combobox_line_doviz_cinsi     = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,DOVIZ_CINSI_COLUMN);
    QLineEdit   * lineedit_cek_senet_no         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CEK_SENET_NO_COLUMN);
    QAdakDate  * dateedit_vade_tarihi          = ( QAdakDate  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VADE_TARIHI_COLUMN);
    QAdakDate  * dateedit_keside_tarihi        = ( QAdakDate  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KESIDE_TARIHI_COLUMN);
    QCommaEdit  * commaedit_cek_senet_tutari    = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CEK_SENET_TUTARI_COLUMN);

    //Cek senet kaydi satirda yapiliyor

    M_CEK_SENET->portfoy_numarasi = 0;


    if ( m_musteri_cek_senedi_mi EQ 1 ) {
        M_CEK_SENET->banka_ismi       = combobox_line_banka_ismi->currentText();
        M_CEK_SENET->sube_kodu        = lineedit_line_sube_kodu->text();
        M_CEK_SENET->sube_ismi        = combobox_line_sube_ismi->currentText();
        M_CEK_SENET->hesap_no_id      = 0;
        M_CEK_SENET->musteri_hesap_numarasi = lineedit_line_hesap_no->text();
    }
    else {
        M_CEK_SENET->banka_ismi       = m_frm_bnk_ismi;
        M_CEK_SENET->sube_kodu        = m_frm_sube_kodu;
        M_CEK_SENET->sube_ismi        = m_frm_sube_ismi;
        M_CEK_SENET->hesap_no_id      = m_hesap_no_id;
        M_CEK_SENET->musteri_hesap_numarasi = "";
    }

    M_CEK_SENET->cek_senet_numarasi = lineedit_cek_senet_no->text();
    M_CEK_SENET->vade_tarihi        = dateedit_vade_tarihi->DATE();
    M_CEK_SENET->keside_tarihi      = dateedit_keside_tarihi->DATE();
    M_CEK_SENET->musteri_ceki_mi    = m_musteri_cek_senedi_mi;
    M_CEK_SENET->cek_senet_tutari   = commaedit_cek_senet_tutari->GET_DOUBLE();
    M_CEK_SENET->cek_senet_turu     = m_cek_mi_senet_mi;

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        if ( m_musteri_cek_senedi_mi EQ 1 ) {
            M_CEK_SENET->doviz_id = DVZ_GET_DOVIZ_ID(combobox_line_doviz_cinsi->currentText());
        }
        else {
            M_CEK_SENET->doviz_id = m_frm_bnk_doviz_id;
        }
    }
    else {
        M_CEK_SENET->doviz_id = DVZ_GET_DOVIZ_ID(combobox_line_doviz_cinsi->currentText());
    }


    int cek_senet_id = CEK_SENET_EKLE(M_CEK_SENET);


    QLineEdit * lineedit_cari_hesap_id          = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , CARI_HESAP_ID_COLUMN);
    QLineEdit * lineedit_verilen_cari_hesap_id  = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , VERILEN_CARI_HESAP_ID_COLUMN);

    M_BORDRO_SATIRI->bordro_id             = bordro_id;
    M_BORDRO_SATIRI->cek_senet_id          = cek_senet_id;
    M_BORDRO_SATIRI->order_number          = m_ui->tablewidget_fis_satirlari->item(row_number , ORDER_COLUMN)->text().toInt();
    M_BORDRO_SATIRI->alinan_cari_hesap_id  = lineedit_cari_hesap_id->text().toInt();
    M_BORDRO_SATIRI->verilen_cari_hesap_id = lineedit_verilen_cari_hesap_id->text().toInt();
    M_BORDRO_SATIRI->cek_tutari            = commaedit_cek_senet_tutari->GET_DOUBLE();
    M_BORDRO_SATIRI->base_record_id        = 0;
    M_BORDRO_SATIRI->program_id            = M_BORDRO->program_id;
    M_BORDRO_SATIRI->modul_id              = M_BORDRO->modul_id;

    int bordro_satiri_id = CEK_BORDRO_SATIRI_EKLE(M_BORDRO , M_BORDRO_SATIRI, false);

    QTableWidgetItem * new_item;

    new_item = new QTableWidgetItem ( QVariant ( bordro_satiri_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( row_number,ROW_ID_COLUMN,new_item );

    new_item = new QTableWidgetItem ( QVariant ( bordro_id ).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number,FIS_ID_COLUMN,new_item ) ;

    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_BORDRO->toplam_tutar );

    m_log_kaydi_eklenmeli_mi = true;

    QDate bordro_tarihi = QDate::fromString(M_BORDRO->bordro_tarihi , "yyyy.MM.dd");

    m_log_detaylari = QObject::tr  ( "Çek Senet Açılış Bordrosu , " ) + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + bordro_tarihi.toString ( "dd MMMM yyyy dddd" ) + " , " +
                             "Bordro No :" + QVariant(M_BORDRO->bordro_no).toString() + " , Toplam Tutar : " +
                             VIRGUL_EKLE ( QVariant ( ROUND  ( M_BORDRO->toplam_tutar ) ).toString() , 2));

}

/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );

    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "cek_bordro_satirlari", "cek_senet_id","bordro_satiri_id = :bordro_satiri_id");
    select_query.SET_VALUE      ( ":bordro_satiri_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());


    if ( select_query.SELECT() NE 0 ) {
         select_query.NEXT();

        int cek_senet_id = select_query.VALUE(0).toInt();

        select_query.PREPARE_SELECT ( "cek_bordro_satirlari,cek_bordrolar","bordro_satiri_id",
                                      "cek_senet_id = :cek_senet_id AND "
                                      "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id "
                                      "AND bordro_turu != :bordro_turu");
        select_query.SET_VALUE      ( ":cek_senet_id"  , cek_senet_id);
        select_query.SET_VALUE      ( ":bordro_turu" , ENUM_CEK_SENET_ACILIS_BORDROSU);

        if ( select_query.SELECT() NE 0 ) {
             MSG_WARNING( tr ( "Satırdaki çek / senet işlem görmüştür.Satırda değişiklik yapamazsınız!..!.."), NULL );
             return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::UPDATE_LINE
***************************************************************************************/

void CEK_ACILIS_FISI::UPDATE_LINE ( int bordro_id, int row_number )
{

    QComboBox   * combobox_line_banka_ismi       = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BANKA_ISMI_COLUMN);
    QComboBox   * combobox_line_sube_ismi        = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,SUBE_ISMI_COLUMN);
    QLineEdit   * lineedit_line_sube_kodu        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,SUBE_KODU_COLUMN);
    QLineEdit   * lineedit_line_hesap_no         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_NO_COLUMN);
    QComboBox   * combobox_line_doviz_cinsi      = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,DOVIZ_CINSI_COLUMN);
    QLineEdit   * lineedit_cek_senet_no          = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CEK_SENET_NO_COLUMN);
    QAdakDate   * dateedit_vade_tarihi           = ( QAdakDate   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VADE_TARIHI_COLUMN);
    QAdakDate   * dateedit_keside_tarihi         = ( QAdakDate   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KESIDE_TARIHI_COLUMN);
    QCommaEdit  * commaedit_cek_senet_tutari     = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CEK_SENET_TUTARI_COLUMN);
    QLineEdit * lineedit_cari_hesap_id           = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CARI_HESAP_ID_COLUMN);
    QLineEdit * lineedit_verilen_cari_hesap_id   = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VERILEN_CARI_HESAP_ID_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("cek_cekler_senetler,cek_bordro_satirlari","portfoy_numarasi , cek_bordro_satirlari.cek_senet_id ",
                         "cek_cekler_senetler.cek_senet_id = cek_bordro_satirlari.cek_senet_id "
                         "AND bordro_satiri_id = :bordro_satiri_id");
    query.SET_VALUE(":bordro_satiri_id" , m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt());

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    M_CEK_SENET->portfoy_numarasi = query.VALUE(0).toInt();
    int cek_senet_id              = query.VALUE(1).toInt();

    M_BORDRO_SATIRI->alinan_cari_hesap_id = lineedit_cari_hesap_id->text().toInt();
    M_BORDRO_SATIRI->verilen_cari_hesap_id = lineedit_verilen_cari_hesap_id->text().toInt();

    M_BORDRO_SATIRI->bordro_id      = bordro_id;
    M_BORDRO_SATIRI->cek_senet_id   = cek_senet_id;
    M_BORDRO_SATIRI->cek_tutari     = commaedit_cek_senet_tutari->GET_DOUBLE();

    int bordro_satiri_id = m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt();

    CEK_BORDRO_SATIRINI_GUNCELLE(M_BORDRO , M_BORDRO_SATIRI , bordro_satiri_id);



    if ( m_musteri_cek_senedi_mi EQ 1 ) {
        M_CEK_SENET->banka_ismi  = combobox_line_banka_ismi->currentText();
        M_CEK_SENET->sube_kodu   = lineedit_line_sube_kodu->text();
        M_CEK_SENET->sube_ismi   = combobox_line_sube_ismi->currentText();
        M_CEK_SENET->hesap_no_id = 0;
        M_CEK_SENET->musteri_hesap_numarasi = lineedit_line_hesap_no->text();
    }
    else {
        M_CEK_SENET->banka_ismi  = m_frm_bnk_ismi;
        M_CEK_SENET->sube_kodu   = m_frm_sube_kodu;
        M_CEK_SENET->sube_ismi   = m_frm_sube_ismi;
        M_CEK_SENET->hesap_no_id = m_hesap_no_id;
        M_CEK_SENET->musteri_hesap_numarasi = "";
    }
    M_CEK_SENET->cek_senet_numarasi = lineedit_cek_senet_no->text();
    M_CEK_SENET->vade_tarihi        = dateedit_vade_tarihi->DATE();
    M_CEK_SENET->keside_tarihi      = dateedit_keside_tarihi->DATE();
    M_CEK_SENET->cek_senet_tutari   = commaedit_cek_senet_tutari->GET_DOUBLE();

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        if (m_musteri_cek_senedi_mi EQ 1 ) {
            M_CEK_SENET->doviz_id = DVZ_GET_DOVIZ_ID(combobox_line_doviz_cinsi->currentText());
        }
        else {
            M_CEK_SENET->doviz_id = m_frm_bnk_doviz_id;
        }
    }
    else {
        M_CEK_SENET->doviz_id = DVZ_GET_DOVIZ_ID(combobox_line_doviz_cinsi->currentText());
    }

    CEK_SENET_GUNCELLE(M_CEK_SENET , cek_senet_id);

    m_ui->commaedit_toplam_tutar->SET_DOUBLE(M_BORDRO->toplam_tutar);

    m_log_kaydi_eklenmeli_mi = true;

    QDate bordro_tarihi = QDate::fromString(M_BORDRO->bordro_tarihi , "yyyy.MM.dd");

    m_log_detaylari = QObject::tr ( "Çek Senet Açılış Bordrosu , " ) + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + bordro_tarihi.toString ( "dd MMMM yyyy dddd" ) + " , " +
                            "Bordro No :" + QVariant(M_BORDRO->bordro_no).toString() + " , Toplam Tutar : " +
                            VIRGUL_EKLE ( QVariant (  ROUND ( M_BORDRO->toplam_tutar ) ).toString() , 2 ) );
}

/**************************************************************************************
                   CEK_ACILIS_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int CEK_ACILIS_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );

    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "cek_bordro_satirlari","cek_senet_id","bordro_satiri_id = :bordro_satiri_id");
    select_query.SET_VALUE      ( ":bordro_satiri_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());

    if ( select_query.SELECT() NE 0 ) {
         select_query.NEXT();

        int cek_senet_id = select_query.VALUE(0).toInt();

        select_query.PREPARE_SELECT ( "cek_bordro_satirlari,cek_bordrolar",
                                      "bordro_satiri_id ",
                                      "cek_senet_id = :cek_senet_id AND bordro_turu != :bordro_turu AND "
                                      "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id");
        select_query.SET_VALUE      ( ":cek_senet_id"  , cek_senet_id );
        select_query.SET_VALUE      ( ":bordro_turu" , ENUM_CEK_SENET_ACILIS_BORDROSU );

        if ( select_query.SELECT() NE 0 ) {
             MSG_WARNING( tr ( "Satırdaki çek / senet işlem görmüştür.Satırı Silemezsiniz!.."), m_ui->tablewidget_fis_satirlari );
             return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::DELETE_LINE
***************************************************************************************/

void CEK_ACILIS_FISI::DELETE_LINE ( int bordro_id, int row_number)
{
    Q_UNUSED ( bordro_id );

    int bordro_satiri_id = m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt();

    CEK_BORDRO_SATIRINI_SIL(M_BORDRO ,bordro_satiri_id);

    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_BORDRO->toplam_tutar );

    m_log_kaydi_eklenmeli_mi = true;

    QDate bordro_tarihi = QDate::fromString(M_BORDRO->bordro_tarihi , "yyyy.MM.dd");

    m_log_detaylari = QObject::tr  ( "Çek Senet Açılış Bordrosu , ") + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + bordro_tarihi.toString("dd MMMM yyyy dddd") + " , " +
                             "Bordro No :" + QVariant( M_BORDRO->bordro_no).toString() + " , Toplam Tutar : " +
                             VIRGUL_EKLE ( QVariant ( ROUND (M_BORDRO->toplam_tutar ) ).toString() , 2));

}

/**************************************************************************************
                   CEK_ACILIS_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::LOCK_FIS_RECORD ( int record_id )
{
    return DB->LOCK_ROW ( "bordro_id","cek_bordrolar", QString ( "bordro_id = %1" ).arg ( record_id ) );
}

/**************************************************************************************
                   CEK_ACILIS_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void CEK_ACILIS_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
    DB->UNLOCK_ROW (  "bordro_id","cek_bordrolar", QString ( "bordro_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   CEK_ACILIS_FISI::SELECTED_LINE
***************************************************************************************/

void CEK_ACILIS_FISI::SELECTED_LINE ( int line_id,int row_save_status )
{
    Q_UNUSED ( row_save_status);

    QSearchEdit * searchedit_cari_hesap_kodu         = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(line_id,CARI_HESAP_KODU_COLUMN);
    QSearchEdit * searchedit_verilen_cari_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(line_id,VERILEN_CARI_HESAP_KODU_COLUMN);

    QLineEdit * lineedit_cari_hesap_id = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(line_id,CARI_HESAP_ID_COLUMN);
    QLineEdit * lineedit_verilen_cari_hesap_id = (QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(line_id,VERILEN_CARI_HESAP_ID_COLUMN);

    if ( searchedit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ false ) {
        m_ui->label_cari_hesap_ismi->setHidden ( false );
        m_ui->lineEdit_cari_hesap_ismi->setHidden ( false );
        int cari_hesap_id = lineedit_cari_hesap_id->text().toInt();
        m_ui->lineEdit_cari_hesap_ismi->setText ( CARI_FIND_HESAP_ISMI ( cari_hesap_id ) );
    }
    if ( searchedit_verilen_cari_hesap_kodu->GET_TEXT().isEmpty() EQ false ) {
        m_ui->label_verilen_cari_hesap_ismi->setHidden ( false );
        m_ui->lineEdit_verilen_cari_hesap_ismi->setHidden ( false );

        int cari_hesap_id = lineedit_verilen_cari_hesap_id->text().toInt();
        m_ui->lineEdit_verilen_cari_hesap_ismi->setText ( CARI_FIND_HESAP_ISMI ( cari_hesap_id ) );
    }
}


/**************************************************************************************
                   CEK_ACILIS_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void CEK_ACILIS_FISI::LINE_SEARCH_EDIT_CLICKED ( int row_number ,QWidget * widget, QLineEdit * lineedit )
{
    Q_UNUSED ( row_number );
    Q_UNUSED ( widget     );

    int cari_hesap_id  = OPEN_CARI_KART_ARAMA ( lineedit->text(), SAHIS_ARTI_FIRMA,this,true , E9_PROGRAMI, CARI_MODULU  );

    if ( cari_hesap_id NE -1 ) {
         lineedit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id ) );
    }
}

/**************************************************************************************
                   CEK_ACILIS_FISI::LEAVE_RECORD
***************************************************************************************/

int CEK_ACILIS_FISI::LEAVE_RECORD()
{
    if ( m_log_kaydi_eklenmeli_mi EQ true ) {
         DB->START_TRANSACTION();
        if ( m_kayit_eklendi EQ true ) {
            E9_LOG_KAYDI_EKLE ( CEKSENET_MODULU,LOG_CEK_BORDROSU,LOG_ISLEM_ADD,m_log_detaylari );
        }
        if ( m_kayit_silindi EQ true ) {
             E9_LOG_KAYDI_EKLE ( CEKSENET_MODULU,LOG_CEK_BORDROSU,LOG_ISLEM_DELETE,m_log_detaylari );
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
             E9_LOG_KAYDI_EKLE ( CEKSENET_MODULU,LOG_CEK_BORDROSU,LOG_ISLEM_UPDATE,m_log_detaylari );
        }
        DB->COMMIT_TRANSACTION();
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_ACILIS_FISI::FIS_EKRANINI_DUZENLE
***************************************************************************************/

void CEK_ACILIS_FISI::FIS_EKRANINI_DUZENLE ()
{

    int index = m_ui->comboBox_bordro_islem_turu->findText ( CEK_GET_CEK_SENET_ACILIS_BORDROSU_ISLEM_TURU_STRING ( m_cek_acilis_bordrosu_islem_turu ) );

    m_ui->comboBox_bordro_islem_turu->setCurrentIndex ( index );

    m_ui->lineEdit_cari_hesap_ismi->setHidden           ( true );
    m_ui->label_cari_hesap_ismi->setHidden              ( true );
    m_ui->lineEdit_verilen_cari_hesap_ismi->setHidden   ( true );
    m_ui->label_verilen_cari_hesap_ismi->setHidden      ( true );

    m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(-1);

    m_ui->label_banka_hesap_kodu->setVisible                ( false );
    m_ui->comboBox_banka_hesap_kodu->setVisible             ( false );
    m_ui->lineEdit_doviz_cinsi->setVisible                  ( false );
    m_ui->lineEdit_bnk_sube_bilgisi->setVisible             ( false );

    m_ui->tablewidget_fis_satirlari->hideColumn             ( BANKA_ISMI_COLUMN );
    m_ui->tablewidget_fis_satirlari->hideColumn             ( SUBE_KODU_COLUMN );
    m_ui->tablewidget_fis_satirlari->hideColumn             ( SUBE_ISMI_COLUMN );
    m_ui->tablewidget_fis_satirlari->hideColumn             ( HESAP_NO_COLUMN ) ;

    m_ui->tablewidget_fis_satirlari->hideColumn             ( VERILEN_CARI_HESAP_KODU_COLUMN );
    m_ui->tablewidget_fis_satirlari->hideColumn             ( CARI_HESAP_KODU_COLUMN );

    m_ui->lineEdit_cari_hesap_ismi->setVisible              ( false );
    m_ui->label_cari_hesap_ismi->setVisible                 ( false );

    SET_PAGE_TITLE ( tr("ÇEK-SNT - AÇILIŞ BORDROSU") );
    SET_HELP_PAGE  ( "cek-senet-islemleri_cek-acilis-bordrosu" );

    switch ( m_cek_acilis_bordrosu_islem_turu ) {

        case ENUM_CEK_ACILIS_BRD_SATICIYA_VER_FIRMA_CEK :

            m_cek_mi_senet_mi       = ENUM_CEK;
            m_musteri_cek_senedi_mi = 0;
            m_bordro_islem_turu     = ENUM_CEK_SATICIYA_VERILDI;

            m_ui->label_banka_hesap_kodu->setVisible                ( true );
            m_ui->comboBox_banka_hesap_kodu->setVisible             ( true );
            m_ui->lineEdit_doviz_cinsi->setVisible                  ( true );
            m_ui->lineEdit_bnk_sube_bilgisi->setVisible             ( true );

            m_ui->tablewidget_fis_satirlari->showColumn             ( VERILEN_CARI_HESAP_KODU_COLUMN );


            m_ui->lineEdit_verilen_cari_hesap_ismi->setVisible      ( true );
            m_ui->label_verilen_cari_hesap_ismi->setVisible         ( true );


            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VERILEN_CARI_HESAP_KODU_COLUMN , 240 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 280 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 210 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 220 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 180 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 70  );

            SET_SETTING_NAME                                        ( "ENUM_CEK_BRD_MUS_VER_FIRMA_CEK" );



            break;

        case ENUM_CEK_ACILIS_BRD_SATICIYA_VER_FIRMA_SENET :

            m_cek_mi_senet_mi       = ENUM_SENET;
            m_musteri_cek_senedi_mi = 0;
            m_bordro_islem_turu     = ENUM_CEK_SATICIYA_VERILDI;

            m_ui->tablewidget_fis_satirlari->showColumn             ( VERILEN_CARI_HESAP_KODU_COLUMN );
            m_ui->lineEdit_verilen_cari_hesap_ismi->setVisible      ( true );
            m_ui->label_verilen_cari_hesap_ismi->setVisible         ( true );

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VERILEN_CARI_HESAP_KODU_COLUMN , 240 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 280 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 210 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 220 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 180 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 70  );

            SET_SETTING_NAME                                        ( "ENUM_CEK_BRD_MUS_VER_FIRMA_SENET" );

            break;

        case ENUM_CEK_ACILIS_BRD_SATICIYA_VER_MUS_CEK      :

            m_cek_mi_senet_mi       = ENUM_CEK;
            m_musteri_cek_senedi_mi = 1;
            m_bordro_islem_turu     = ENUM_CEK_SATICIYA_VERILDI;

            m_ui->tablewidget_fis_satirlari->showColumn ( BANKA_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn ( SUBE_KODU_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn ( SUBE_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn ( HESAP_NO_COLUMN ) ;

            m_ui->tablewidget_fis_satirlari->showColumn             ( VERILEN_CARI_HESAP_KODU_COLUMN );
            m_ui->lineEdit_verilen_cari_hesap_ismi->setVisible      ( true );
            m_ui->label_verilen_cari_hesap_ismi->setVisible         ( true );

            m_ui->tablewidget_fis_satirlari->showColumn             ( CARI_HESAP_KODU_COLUMN );

            m_ui->lineEdit_cari_hesap_ismi->setVisible              ( true );
            m_ui->label_cari_hesap_ismi->setVisible                 ( true );

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 120 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VERILEN_CARI_HESAP_KODU_COLUMN , 140 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 115 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 110 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 110 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( BANKA_ISMI_COLUMN              , 100 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_KODU_COLUMN               , 70  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_ISMI_COLUMN               , 95 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( HESAP_NO_COLUMN                , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 150);
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 60 );

            SET_SETTING_NAME  ( "ENUM_CEK_BRD_MUS_VER_MUS_CEK" );

            break;

        case ENUM_CEK_ACILIS_BRD_SATICIYA_VER_MUS_SENET      :

            m_cek_mi_senet_mi       = ENUM_SENET;
            m_musteri_cek_senedi_mi = 1;
            m_bordro_islem_turu     = ENUM_CEK_SATICIYA_VERILDI;

            m_ui->tablewidget_fis_satirlari->showColumn             ( VERILEN_CARI_HESAP_KODU_COLUMN );
            m_ui->lineEdit_verilen_cari_hesap_ismi->setVisible      ( true );
            m_ui->label_verilen_cari_hesap_ismi->setVisible         ( true );

            m_ui->tablewidget_fis_satirlari->showColumn             ( CARI_HESAP_KODU_COLUMN );

            m_ui->lineEdit_cari_hesap_ismi->setVisible              ( true );
            m_ui->label_cari_hesap_ismi->setVisible                 ( true );

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VERILEN_CARI_HESAP_KODU_COLUMN , 210 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 240 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 140 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 140 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 180 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 80  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 210 );

            SET_SETTING_NAME                                        ( "ENUM_CEK_BRD_MUS_VER_MUS_SENET" );


            break;

        case ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_CEK  :

            m_cek_mi_senet_mi       = ENUM_CEK;
            m_musteri_cek_senedi_mi = 1;
            m_bordro_islem_turu     = ENUM_CEK_BNK_TEMINATA_VERILDI;

            m_ui->label_banka_hesap_kodu->setVisible                ( true );
            m_ui->comboBox_banka_hesap_kodu->setVisible             ( true );
            m_ui->lineEdit_doviz_cinsi->setVisible                  ( true );
            m_ui->lineEdit_bnk_sube_bilgisi->setVisible             ( true );

            m_ui->tablewidget_fis_satirlari->showColumn             ( BANKA_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( SUBE_KODU_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( SUBE_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( HESAP_NO_COLUMN ) ;

            m_ui->tablewidget_fis_satirlari->showColumn             ( CARI_HESAP_KODU_COLUMN );

            m_ui->lineEdit_cari_hesap_ismi->setVisible              ( true );
            m_ui->label_cari_hesap_ismi->setVisible                 ( true );

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 170 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 115 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 115 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( BANKA_ISMI_COLUMN              , 120 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_KODU_COLUMN               , 70  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_ISMI_COLUMN               , 120 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( HESAP_NO_COLUMN                , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 150);
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 80 );


            SET_SETTING_NAME                                        ( "ENUM_CEK_BRD_BNK_TEMINAT_MUS_CEK" );
            break;

        case ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_SENET  :

            m_cek_mi_senet_mi       = ENUM_SENET;
            m_musteri_cek_senedi_mi = 1;
            m_bordro_islem_turu     = ENUM_CEK_BNK_TEMINATA_VERILDI;

            m_ui->label_cari_hesap_ismi->setHidden(false);
            m_ui->lineEdit_cari_hesap_ismi->setHidden(false);

            m_ui->label_banka_hesap_kodu->setVisible                ( true );
            m_ui->comboBox_banka_hesap_kodu->setVisible             ( true );
            m_ui->lineEdit_doviz_cinsi->setVisible                  ( true );
            m_ui->lineEdit_bnk_sube_bilgisi->setVisible             ( true );

            m_ui->tablewidget_fis_satirlari->showColumn     ( CARI_HESAP_KODU_COLUMN);

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 240 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 280 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 210 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 220 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 180 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 70  );

            SET_SETTING_NAME ( "ENUM_CEK_BRD_BNK_TEMINAT_MUS_SENET" );

            break;

        case ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_CEK :

            m_cek_mi_senet_mi       = ENUM_CEK;
            m_musteri_cek_senedi_mi = 1;
            m_bordro_islem_turu     = ENUM_CEK_BNK_TAHSILATA_VERILDI;

            m_ui->label_banka_hesap_kodu->setVisible                ( true );
            m_ui->comboBox_banka_hesap_kodu->setVisible             ( true );
            m_ui->lineEdit_doviz_cinsi->setVisible                  ( true );
            m_ui->lineEdit_bnk_sube_bilgisi->setVisible             ( true );

            m_ui->tablewidget_fis_satirlari->showColumn             ( BANKA_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( SUBE_KODU_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( SUBE_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( HESAP_NO_COLUMN ) ;

            m_ui->tablewidget_fis_satirlari->showColumn             ( CARI_HESAP_KODU_COLUMN );

            m_ui->lineEdit_cari_hesap_ismi->setVisible              ( true );
            m_ui->label_cari_hesap_ismi->setVisible                 ( true );

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 140 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 120 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 120 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( BANKA_ISMI_COLUMN              , 140 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_KODU_COLUMN               , 80  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_ISMI_COLUMN               , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( HESAP_NO_COLUMN                , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 120);
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 90 );

            SET_SETTING_NAME ( "ENUM_CEK_BRD_BNK_TAHSILAT_MUS_CEK" );

            break;

        case ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_SENET :

            m_cek_mi_senet_mi       = ENUM_SENET;
            m_musteri_cek_senedi_mi = 1;
            m_bordro_islem_turu     = ENUM_CEK_BNK_TAHSILATA_VERILDI;

            m_ui->label_banka_hesap_kodu->setVisible                ( true );
            m_ui->comboBox_banka_hesap_kodu->setVisible             ( true );
            m_ui->lineEdit_doviz_cinsi->setVisible                  ( true );
            m_ui->lineEdit_bnk_sube_bilgisi->setVisible             ( true );

            m_ui->tablewidget_fis_satirlari->showColumn             ( CARI_HESAP_KODU_COLUMN );

            m_ui->lineEdit_cari_hesap_ismi->setVisible               ( true );
            m_ui->label_cari_hesap_ismi->setVisible                  ( true );

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 300 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 155 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 155 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 220 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 100  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 270 );

            SET_SETTING_NAME                                        ( "ENUM_CEK_BRD_BNK_TAHSILAT_MUS_SENET" );


            break;

        case ENUM_CEK_ACILIS_BRD_KARSILIKSIZ_MUS_CEK  :
        case ENUM_CEK_ACILIS_BRD_PORTFOYDE_MUS_CEK    :

            m_cek_mi_senet_mi       = ENUM_CEK;
            m_musteri_cek_senedi_mi = 1;

            if ( m_cek_acilis_bordrosu_islem_turu EQ ENUM_CEK_ACILIS_BRD_KARSILIKSIZ_MUS_CEK ) {
                m_bordro_islem_turu     = ENUM_CEK_KARSILIKSIZ;
            }
            else {
                m_bordro_islem_turu    = ENUM_CEK_MUSTERIDEN_ALINDI;
            }


            m_ui->tablewidget_fis_satirlari->showColumn             ( BANKA_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( SUBE_KODU_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( SUBE_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( HESAP_NO_COLUMN ) ;

            m_ui->tablewidget_fis_satirlari->showColumn             ( CARI_HESAP_KODU_COLUMN );

            m_ui->lineEdit_cari_hesap_ismi->setVisible               ( true );
            m_ui->label_cari_hesap_ismi->setVisible                  ( true );

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 150 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 125 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 120 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 120 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( BANKA_ISMI_COLUMN              , 140 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_KODU_COLUMN               , 70  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_ISMI_COLUMN               , 125 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( HESAP_NO_COLUMN                , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 150);
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 70 );

            if ( m_cek_acilis_bordrosu_islem_turu EQ ENUM_CEK_ACILIS_BRD_KARSILIKSIZ_MUS_CEK ) {

                SET_SETTING_NAME( "ENUM_CEK_BRD_KARSILIKSIZ_MUS_CEK" );
            }
            else {
                SET_SETTING_NAME( "ENUM_CEK_BRD_PORTFOYDE_MUS_CEK");
            }

            break;


        case ENUM_CEK_ACILIS_BRD_PROTESTOLU_MUS_SENET   :
        case ENUM_CEK_ACILIS_BRD_PORTFOYDE_MUS_SENET    :

            m_cek_mi_senet_mi       = ENUM_SENET;
            m_musteri_cek_senedi_mi = 1;

            if ( m_cek_acilis_bordrosu_islem_turu EQ ENUM_CEK_ACILIS_BRD_PROTESTOLU_MUS_SENET ) {

                m_bordro_islem_turu     = ENUM_CEK_PROTESTOLU;
            }
            else {
                m_bordro_islem_turu = ENUM_CEK_MUSTERIDEN_ALINDI;
            }


            m_ui->tablewidget_fis_satirlari->showColumn ( CARI_HESAP_KODU_COLUMN );

            m_ui->lineEdit_cari_hesap_ismi->setVisible               ( true );
            m_ui->label_cari_hesap_ismi->setVisible                  ( true );


            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 350 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 155 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 155 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 200 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 100  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 240 );

            if ( m_cek_acilis_bordrosu_islem_turu EQ ENUM_CEK_ACILIS_BRD_PROTESTOLU_MUS_SENET ) {

                SET_SETTING_NAME ( "ENUM_CEK_BRD_PROTESTOLU_MUS_SENET" );
            }
            else {
                SET_SETTING_NAME ( "ENUM_CEK_BRD_PORTFOYDE_MUS_SENET");
            }


            break;

        case ENUM_CEK_ACILIS_BRD_SUP_ALACAK_MUS_CEK   :

            m_cek_mi_senet_mi       = ENUM_CEK;
            m_musteri_cek_senedi_mi = 1;
            m_bordro_islem_turu     = ENUM_CEK_SUPHELI_ALACAK;

            m_ui->tablewidget_fis_satirlari->showColumn             ( BANKA_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( SUBE_KODU_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( SUBE_ISMI_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn             ( HESAP_NO_COLUMN ) ;

            m_ui->tablewidget_fis_satirlari->showColumn             ( CARI_HESAP_KODU_COLUMN );

            m_ui->lineEdit_cari_hesap_ismi->setVisible               ( true );
            m_ui->label_cari_hesap_ismi->setVisible                  ( true );

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 140 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 120 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 120 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( BANKA_ISMI_COLUMN              , 140 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_KODU_COLUMN               , 80  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( SUBE_ISMI_COLUMN               , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( HESAP_NO_COLUMN                , 130 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 120);
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 90 );

            SET_SETTING_NAME                                        ( "ENUM_CEK_BRD_SUP_ALACAK_MUS_CEK" );

            break;

        case ENUM_CEK_ACILIS_BRD_SUP_ALACAK_MUS_SENET   :

            m_cek_mi_senet_mi       = ENUM_SENET;
            m_musteri_cek_senedi_mi = 1;
            m_bordro_islem_turu     = ENUM_CEK_SUPHELI_ALACAK;
            SET_HELP_PAGE       ( "cek-senet-islemleri" );


            m_ui->tablewidget_fis_satirlari->showColumn             ( CARI_HESAP_KODU_COLUMN );

            m_ui->lineEdit_cari_hesap_ismi->setVisible              ( true );
            m_ui->label_cari_hesap_ismi->setVisible                 ( true );

            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_NO_COLUMN            , 350 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( KESIDE_TARIHI_COLUMN           , 155 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( VADE_TARIHI_COLUMN             , 155 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CEK_SENET_TUTARI_COLUMN        , 200 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( DOVIZ_CINSI_COLUMN             , 100  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth         ( CARI_HESAP_KODU_COLUMN         , 240 );


            SET_SETTING_NAME                                        ( "ENUM_CEK_BRD_SUP_ALACAK_MUS_SENET" );

            break;

        default :
            break;
    }

    QStringList headers;

    headers <<"row_id"<<"fis_id" <<"order_column"<< "cari_hes_id" << "ver_car_hes_id";

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {

        headers << tr("Cari Hesap Kodu") << tr ( "Verilen Cari Hsp Kodu" );
        headers << tr ( "Çek Numarası" )<<tr ( "Düzenleme Tarihi") << tr ( "Vade Tarihi" ) <<tr ( "Banka İsmi" )<<tr ( "Şube İsmi" );
        headers << tr ( "Şube Kodu" )     << tr ( "Hesap Numarası")<< tr ( "Çek Tutarı");
    }
    else {
        headers << tr("Cari Hesap Kodu") << tr ( "Verilen Cari Hsp Kodu" );
        headers << tr ( "Senet Numarası" )<<tr ( "Düzenleme Tarihi") << tr ( "Vade Tarihi" ) <<tr ( "Banka İsmi" )<<tr ( "Şube İsmi" );
        headers << tr ( "Şube Kodu" )     << tr ( "Hesap Numarası")<< tr ( "Senet Tutarı");
    }

    headers << tr("Para Brm") << "" << "";

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels ( headers );


}
/**************************************************************************************
                   CEK_ACILIS_FISI::EKRANI_GUNCELLE();
***************************************************************************************/

void CEK_ACILIS_FISI::EKRANI_GUNCELLE()
{
    CLOSE_ALL_EVENTS();

    for ( int i = 0 ; i < m_ui->tablewidget_fis_satirlari->rowCount() ; i++ ) {

        QComboBox * combobox_para_birimi = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget( i , DOVIZ_CINSI_COLUMN);

        combobox_para_birimi->setDisabled(true);

        combobox_para_birimi->setCurrentIndex(combobox_para_birimi->findText(DVZ_GET_DOVIZ_KODU(m_frm_bnk_doviz_id)));
    }


    OPEN_ALL_EVENTS();

}











