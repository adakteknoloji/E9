#include <QMessageBox>
#include "ui_bnk_hareket_fisi.h"
#include "bnk_hareket_fisi_class.h"
#include "bnk_hareket_fisi_open.h"
#include "bnk_hareket_fisi_arama_open.h"
#include "bnk_hesaplari_arama_open.h"
#include "adak_sql.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "bnk_gui_utils.h"
#include "banka_enum.h"
#include "bnk_console_utils.h"
#include "print.h"
#include "adak_utils.h"
#include "muh_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_yetki.h"
#include "bnk_fis_utils.h"
#include "cari_console_utils.h"
#include "adak_std_utils.h"
#include "sube_console_utils.h"
#include "muh_fis_utils.h"
#include "e9_struct.h"
#include "e9_enum.h"
#include "cari_kart_arama_open.h"
#include "bnk_kredi_karti_secimi_open.h"
#include "bnk_pos_secimi_open.h"
 
#include "e9_ent_detaylari_batch_open.h"
#include "muh_hesap_arama_open.h"
#include "bnk_fisi_yazdir_open.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "bnk_defter_fis_utils.h"

extern ADAK_SQL *              DB;

#define ROW_ID_COLUMN              0
#define FIS_ID_COLUMN              1
#define ORDER_COLUMN               2
#define KRS_HESAP_ID_COLUMN        3
#define ETK_HESAP_ID_COLUMN        4
#define PROGRAM_ID_COLUMN          5
#define MODUL_ID_COLUMN            6
#define BASE_RECORD_ID_COLUMN      7


#define ETKILECEK_HESAP_COLUMN     8
#define ETK_HESAP_MUH_KODU_COLUMN  9
#define ACIKLAMA_COLUMN            10
#define HESABA_GIREN_COLUMN        11
#define HESAPTAN_CIKAN_COLUMN      12

#define BANKA_DOVIZ_ID             13
#define DOVIZ_KURU                 14
#define PARITE                     15
#define KRS_HESAP_PARA_BIRIM_ID    16
#define TUTAR                      17


#define KARSI_HESAP_COLUMN         18
#define KRS_HESAP_MUH_KODU_COLUMN  19
#define MUH_HESAP_ISMI_COLUMN      20





/**************************************************************************************
                   OPEN_BNK_HAREKET_FISI
***************************************************************************************/

void OPEN_BNK_HAREKET_FISI ( int fis_turu, int record_id,int fis_kaynagi, QWidget * parent )
{
    BNK_HAREKET_FISI * F = new BNK_HAREKET_FISI( fis_turu, record_id, fis_kaynagi,parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   BNK_HAREKET_FISI::BNK_HAREKET_FISI
***************************************************************************************/

BNK_HAREKET_FISI::BNK_HAREKET_FISI ( int fis_turu, int record_id, int fis_kaynagi, QWidget * parent) :FIS_KERNEL(parent), m_ui ( new Ui::BNK_HAREKET_FISI )
{
    m_modul_id          = fis_kaynagi;
    m_start_record_id   = record_id;
    m_bnk_hesap_no_id   = 0;

    m_fis_turu          = fis_turu;

    M_ENT_DETAYLARI     = new E9_ENT_DETAYLARI_STRUCT;
    M_FIS               = new BNK_HAREKET_FIS_STRUCT;
    M_FIS_SATIRI        = new BNK_HAREKET_FIS_SATIRI_STRUCT;

    m_ui->setupUi     ( this );
    START_FIS_KERNEL  ( this, DB );
 }

/**************************************************************************************
                   BNK_HAREKET_FISI::SETUP_FORM();
***************************************************************************************/

void BNK_HAREKET_FISI::SETUP_FORM()
{
    if ( m_modul_id EQ BANKA_MODULU ) {
        if ( m_fis_turu EQ ENUM_BANKA_PARA_YAT_CEKME_FISI ) {
             SET_PAGE_TITLE( tr ( "BNK - PARA YATIRMA / ÇEKME FİŞİ" ) );
             SET_HELP_PAGE("banka-islemleri_para-yatirmacekme-fisi");
        }
        else {
             SET_PAGE_TITLE( tr ( "BNK - HAREKET FİŞİ" ) );
             SET_HELP_PAGE("banka-islemleri_hareket-fisi");
        }
        m_banka_islem_fisi_tarafindan_olusturuldu = true;

    }
    else {

        switch  ( m_modul_id ) {
            case CARI_MODULU :
                SET_PAGE_TITLE( tr ( "BNK - CARİ ENT. FİŞİ" ) );
                break;
            case CEKSENET_MODULU :
                SET_PAGE_TITLE( tr ( "BNK - ÇEK-SENET ENT. FİŞİ" ) );
                break;

            case PERAKENDE_MODULU :
                SET_PAGE_TITLE( tr ( "BNK - PERAKENDE ENT. FİŞİ" ) );
                break;

            case PERSONEL_MODULU :
                SET_PAGE_TITLE( tr ( "BNK - PERSONEL ENT. FİŞİ") );
                break;

            case SMM_MODULU :
                SET_PAGE_TITLE( tr ( "BNK - SM MAKBUZ ENT. FİŞİ" ) );
                break;

            case ISLETME_MODULU :
                SET_PAGE_TITLE( tr ( "BNK - İŞLETME ENT. FİŞİ" ) );
                break;

            case -1 :
                SET_PAGE_TITLE( tr ( "BNK - TÜM ENT. FİŞLERİ" ) );
                break;

            default :
                SET_PAGE_TITLE( tr ( "BNK - DİĞER ENT. FİŞLERİ" ) );
                break;
        }
    }

    E9_CLEAR_ENT_DETAYLARI_STRUCT ( M_ENT_DETAYLARI );
    M_ENT_DETAYLARI->muh_kasa_hesap_id = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();

    m_ui->lineedit_fis_no->setText("*");

    DISABLE_CLEAR_ON_WIDGET( m_ui->lineedit_fis_no );
    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_sirket );

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );

    if( m_fis_turu EQ ENUM_BANKA_ACILIS_FISI ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }
    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    if( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ HAYIR ){
        m_ui->push_button_detaylar->setVisible( false );
    }

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH ( 512 );

    m_banka_islem_fisi_tarafindan_olusturuldu   = false;

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_fis_no->setValidator(validator);

    QFont tableWidget_font = m_ui->tablewidget_fis_satirlari->font();
    int   font_point_size  = tableWidget_font.pointSize             ();

    tableWidget_font.setPointSize                 ( font_point_size -1 );
    m_ui->tablewidget_fis_satirlari->setFont      ( tableWidget_font   );


    etkilenecek_hesap_turu = 0;
    karsi_hesap_turu       = 0;

    if ( m_modul_id EQ BANKA_MODULU ) {
        REGISTER_BUTTONS_WIDGET             ( m_ui->navigation_button_widget );
    }
    else {
        REGISTER_BUTTONS_WIDGET(m_ui->navigation_button_widget , false , true);
        m_ui->push_button_detaylar->setHidden(true);
    }

    REGISTER_TABLE_WIDGET               ( m_ui->tablewidget_fis_satirlari, 21 );

    SET_FIS_ORDER_COLUMN_NUMBER         ( ORDER_COLUMN );

    SET_FORM_ID_COLUMN_POSITION         ( FIS_ID_COLUMN );

    SET_ROW_ID_COLUMN_POSITION          ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN         ( 8);

    SET_FIS_ORDER_DATABASE ("bnk_fis_satirlari", "order_number", "fis_satiri_id" );

    SET_TABLE_ROW_WIDGETS  ( ETKILECEK_HESAP_COLUMN   , WIDGET_COMBO_BOX   ); // ETKILENECEK HESAP
    SET_TABLE_ROW_WIDGETS  ( ETK_HESAP_MUH_KODU_COLUMN, WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS  ( ACIKLAMA_COLUMN          , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( HESABA_GIREN_COLUMN      , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS  ( HESAPTAN_CIKAN_COLUMN    , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS  ( KARSI_HESAP_COLUMN       , WIDGET_COMBO_BOX   ); // KARSI HESAP
    SET_TABLE_ROW_WIDGETS  ( KRS_HESAP_MUH_KODU_COLUMN, WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS  ( MUH_HESAP_ISMI_COLUMN    , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( KRS_HESAP_ID_COLUMN      , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( ETK_HESAP_ID_COLUMN      , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( PROGRAM_ID_COLUMN        , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( MODUL_ID_COLUMN          , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( BASE_RECORD_ID_COLUMN    , WIDGET_LINE_EDIT   );

    SET_TABLE_ROW_WIDGETS  ( BANKA_DOVIZ_ID           , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( DOVIZ_KURU               , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS  ( PARITE                   , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS  ( KRS_HESAP_PARA_BIRIM_ID  , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( TUTAR                    , WIDGET_COMMA_EDIT  );
     
    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_fis_no );

    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI(m_ui->combo_box_banka_hesap_kodu);

    REGISTER_SAVER_BUTTON  ( m_ui->buton_yazdir );
    REGISTER_CHANGER_BUTTON( m_ui->push_button_detaylar );

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH( 512 );
    m_ui->lineEdit_banka_bilgisi->setMaxLength( 150 );
    m_ui->lineedit_doviz_cinsi->setMaxLength( 10 );
    m_ui->lineedit_fis_no->setMaxLength( 10 );

    SET_COMBOBOX_LENGTH( m_ui->combo_box_banka_hesap_kodu , 50 );

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels(QStringList()<<"row_id"<<"fis_id"<<"order_column"
                                                                            <<"muh_hesap_id" << tr( "etk_hesap_id" )<< tr( "" ) << tr( "" ) << tr ( "" )
                                                                            <<tr( "Etkilenecek\n Hesap" )
                                                                            <<tr( "Etkilenecek \n Hesap Kodu" )
                                                                            <<tr( "Açıklama")
                                                                            <<tr( "Hesaba\n Giren")
                                                                            <<tr( "Hesaptan\n Çıkan")
                                                                            <<tr( "Para \n Birimi" )
                                                                            <<tr( "Döviz Kuru" )
                                                                            <<tr( "Parite" )
                                                                            <<tr( "Cari Para \n Birimi" )
                                                                            <<tr( "Tutar" )
                                                                            <<tr( "Karşı\n Hesap")
                                                                            <<tr( "Karşı Hesap Kodu")
                                                                            <<tr( "Hesap İsmi")
                                                                            <<tr( "" )<<tr( "" ));




    E9_KULLANICI_YETKILERI_STRUCT * E9_KUL_YETKILERI = ( E9_KULLANICI_YETKILERI_STRUCT * ) E9_GET_KULLANICI_YETKILERI();

    m_entegrasyon_hesaplarini_girebilir_mi           = E9_KUL_YETKILERI->ynt_e9_ent_bilgilerini_degistirme_yetkisi;

    if ( m_fis_turu EQ ENUM_BANKA_PARA_YAT_CEKME_FISI ){

        m_ui->tablewidget_fis_satirlari->hideColumn( ETK_HESAP_MUH_KODU_COLUMN );
        m_ui->tablewidget_fis_satirlari->hideColumn( KRS_HESAP_MUH_KODU_COLUMN );
        m_ui->tablewidget_fis_satirlari->hideColumn( KRS_HESAP_PARA_BIRIM_ID   );
        m_ui->tablewidget_fis_satirlari->hideColumn( MUH_HESAP_ISMI_COLUMN     );

        m_ui->tablewidget_fis_satirlari->setColumnWidth( ACIKLAMA_COLUMN            , 150 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( HESABA_GIREN_COLUMN        , 100 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( HESAPTAN_CIKAN_COLUMN      , 100 );

        m_ui->tablewidget_fis_satirlari->setColumnWidth( BANKA_DOVIZ_ID             , 60 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( DOVIZ_KURU                 , 120 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( PARITE                     , 120 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( TUTAR                      , 120 );

        SET_SETTING_NAME    ( "BANKA_HAREKET_FISI_1" );

    }
    else {
        m_ui->tablewidget_fis_satirlari->setColumnWidth( ETKILECEK_HESAP_COLUMN     , 121 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( ETK_HESAP_MUH_KODU_COLUMN  , 140 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( ACIKLAMA_COLUMN            , 150 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( HESABA_GIREN_COLUMN        , 100 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( HESAPTAN_CIKAN_COLUMN      , 100 );

        m_ui->tablewidget_fis_satirlari->setColumnWidth( BANKA_DOVIZ_ID             , 60 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( DOVIZ_KURU                 , 120 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( PARITE                     , 120 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( KRS_HESAP_PARA_BIRIM_ID    , 70 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth( TUTAR                      , 120 );

        m_ui->tablewidget_fis_satirlari->setColumnWidth ( KARSI_HESAP_COLUMN       , 90 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( KRS_HESAP_MUH_KODU_COLUMN, 140 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( MUH_HESAP_ISMI_COLUMN    , 120 );

        SET_SETTING_NAME    ( "BANKA_HAREKET_FISI_2" );
    }

    m_ui->tablewidget_fis_satirlari->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_fis_satirlari->setFocus( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET();

    AUTOSTART_FIS_KERNEL( m_start_record_id );
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void BNK_HAREKET_FISI::CLEAR_FORM_MEMBERS()
{
   m_bnk_hesap_no_id = 0;
   m_ui->lineedit_fis_no->setText("*");
   BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT( M_FIS_SATIRI );
   BNK_CLEAR_HAREKET_FIS_STRUCT( M_FIS );

}

/**************************************************************************************
                   BNK_HAREKET_FISI::NEW_FIS_RECORD
***************************************************************************************/

void BNK_HAREKET_FISI::NEW_FIS_RECORD()
{
    M_FIS->muh_kasa_hesap_id = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();
}

/**************************************************************************************
                   BNK_HAREKET_FISI::GET_FIS_RECORD
***************************************************************************************/

int BNK_HAREKET_FISI::GET_FIS_RECORD ( int record_id )
{

    BNK_CLEAR_HAREKET_FIS_STRUCT          ( M_FIS );

    if ( record_id <= 0 ) {
        return ADAK_FAIL;
    }

    BNK_HAREKET_FIS_BILGILERINI_OKU       ( M_FIS, record_id );

    M_ENT_DETAYLARI->muh_kasa_hesap_id = M_FIS->muh_kasa_hesap_id;

    m_ui->lineedit_fis_no->setText( QString::number( M_FIS->fis_no ) );
    m_ui->adakDate_fis_tarihi->SET_DATE(  M_FIS->fis_tarihi );

    m_ui->lineedit_fis_no->setText( QString::number( M_FIS->fis_no ) );

    m_bnk_hesap_no_id = M_FIS->hesap_no_id;

    m_ui->limitedTextEdit_aciklama->setText( M_FIS->aciklama );

    double toplam_hesaba_giren   = M_FIS->toplam_hesaba_giren;
    double toplam_hesaptan_cikan = M_FIS->toplam_hesaptan_cikan;

    m_ui->commaEdit_toplam_hesaba_giren->SET_DOUBLE( toplam_hesaba_giren );
    m_ui->commaEdit_toplam_hesaptan_cikan->SET_DOUBLE( toplam_hesaptan_cikan );

    SQL_QUERY select_query (DB);

    int row = BNK_HESAPLARI_MODEL_INDEX_BUL ( m_bnk_hesap_no_id, m_ui->combo_box_banka_hesap_kodu );

    if ( row > -1 ) {

        m_ui->combo_box_banka_hesap_kodu->setCurrentIndex ( row );

        m_bnk_hesap_no_id         = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->combo_box_banka_hesap_kodu, row, 0 ).toInt();
        m_banka_hesabi_doviz_id   = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 1 ).toInt();

        m_ui->lineedit_doviz_cinsi->setText ( DVZ_GET_DOVIZ_KODU ( m_banka_hesabi_doviz_id ) );

        QString banka_bilgisi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 3 ).toString() + " / " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 4 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 5 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 6 ).toString();

        m_ui->lineEdit_banka_bilgisi->setText ( banka_bilgisi );

    }

    select_query.PREPARE_SELECT("bnk_hareket_fis_satirlari","fis_satiri_id, order_number ",
                                "fis_id = :fis_id ");
    select_query.SET_VALUE(":fis_id" , record_id);

    if ( select_query.SELECT() EQ 0 ) {
        return ADAK_OK;
    }
    QTableWidgetItem * new_item;

    while (select_query.NEXT()) {

        BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT  ( M_FIS_SATIRI );
        BNK_HAREKET_FIS_SATIR_BILGILERINI_OKU( M_FIS_SATIRI, select_query.VALUE( 0 ).toInt() );

        int row_number = ADD_NEW_LINE();

        QLineEdit   * lineedit_aciklama             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ACIKLAMA_COLUMN             );
        QCommaEdit  * commaedit_hesaba_giren        = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, HESABA_GIREN_COLUMN         );
        QCommaEdit  * commaedit_hesaptan_cikan      = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, HESAPTAN_CIKAN_COLUMN       );
        QSearchEdit * searchedit_krs_hesap_kodu     = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KRS_HESAP_MUH_KODU_COLUMN   );
        QSearchEdit * searchedit_etk_muh_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ETK_HESAP_MUH_KODU_COLUMN   );
        QLineEdit   * lineedit_krs_hesap_ismi       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, MUH_HESAP_ISMI_COLUMN       );
        QLineEdit   * lineedit_muh_hesap_id         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KRS_HESAP_ID_COLUMN         );
        QLineEdit   * lineedit_etk_hesap_id         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ETK_HESAP_ID_COLUMN         );
        QComboBox   * etkilenecek_hesap             = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ETKILECEK_HESAP_COLUMN      );
        QComboBox   * karsi_hesap                   = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KARSI_HESAP_COLUMN          );

        QCommaEdit   * commaEdit_doviz_kuru         = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, DOVIZ_KURU                  );
        QCommaEdit   * commaEdit_parite             = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, PARITE                      );
        QCommaEdit   * commaEdit_tutar              = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, TUTAR                       );

        QLineEdit   * lineedit_program_id           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, PROGRAM_ID_COLUMN           );
        QLineEdit   * lineedit_modul_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, MODUL_ID_COLUMN             );
        QLineEdit   * lineedit_base_record_id       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, BASE_RECORD_ID_COLUMN       );

        QLineEdit   * lineedit_etk_para_birim_kodu  = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,  BANKA_DOVIZ_ID           );
        QLineEdit   * lineedit_krs_para_birim_kodu  = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,  KRS_HESAP_PARA_BIRIM_ID       );


        lineedit_etk_para_birim_kodu->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( M_FIS->hesap_no_id )) );

        int krs_hesap_para_birim_id = CARI_GET_PARA_BIRIM_ID( M_FIS_SATIRI->karsi_hesap_id );

        switch ( M_FIS_SATIRI->karsi_hesap_turu )
        {
        case ENUM_BNK_CARI_HESAP:
            lineedit_krs_para_birim_kodu->setText( DVZ_GET_DOVIZ_KODU( CARI_GET_PARA_BIRIM_ID( M_FIS_SATIRI->karsi_hesap_id ) ));
            break;
        case ENUM_BNK_KREDI_KARTI_HESABI:
        case ENUM_BNK_MUHASEBE_HESABI:
        case ENUM_BNK_POS_HESABI:
            lineedit_krs_para_birim_kodu->setText( DVZ_GET_DOVIZ_KODU( BNK_GET_PARA_BIRIM_ID( M_FIS->hesap_no_id ) ));
            krs_hesap_para_birim_id = BNK_GET_PARA_BIRIM_ID( M_FIS->hesap_no_id );
            break;

        default: break;
        }

        int etk_hesap_para_birim_id = BNK_GET_PARA_BIRIM_ID( m_bnk_hesap_no_id );

        int temel_para_birim_id     = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

        commaEdit_doviz_kuru->setEnabled( false );
        commaEdit_parite->setEnabled( false );

        if ( etk_hesap_para_birim_id NE krs_hesap_para_birim_id ) {

            if ( etk_hesap_para_birim_id EQ temel_para_birim_id ){
                commaEdit_doviz_kuru->setEnabled( true );
                commaEdit_doviz_kuru->SET_DOUBLE( M_FIS_SATIRI->doviz_kuru );
            }
            else if ( krs_hesap_para_birim_id EQ temel_para_birim_id ) {
                commaEdit_doviz_kuru->setEnabled( true );
                commaEdit_doviz_kuru->SET_DOUBLE( M_FIS_SATIRI->doviz_kuru );
            }
            else {
                commaEdit_parite->setEnabled( true );
                commaEdit_parite->SET_DOUBLE( M_FIS_SATIRI->parite );
            }
        }
        else {
            if (etk_hesap_para_birim_id NE temel_para_birim_id ){
                commaEdit_doviz_kuru->setEnabled( true );
                commaEdit_doviz_kuru->SET_DOUBLE( M_FIS_SATIRI->doviz_kuru );
            }
        }


        lineedit_program_id->setText( QVariant( M_FIS_SATIRI->program_id ).toString() );
        lineedit_modul_id->setText( QVariant( M_FIS_SATIRI->modul_id ).toString() );
        lineedit_base_record_id->setText( QVariant( M_FIS_SATIRI->base_record_id ).toString() );

        lineedit_etk_hesap_id->setText( QVariant( M_FIS_SATIRI->etkilenecek_hesap_id).toString() );
        lineedit_muh_hesap_id->setText( QVariant( M_FIS_SATIRI->doviz_kuru ).toString() );
        commaEdit_doviz_kuru->SET_DOUBLE( M_FIS_SATIRI->doviz_kuru );
        commaEdit_parite->SET_DOUBLE( M_FIS_SATIRI->parite );

        lineedit_aciklama->setText( M_FIS_SATIRI->aciklama );
        commaedit_hesaba_giren->SET_DOUBLE( M_FIS_SATIRI->hesaba_giren );
        commaedit_hesaptan_cikan->SET_DOUBLE( M_FIS_SATIRI->hesaptan_cikan );
        lineedit_muh_hesap_id->setText( QString::number( M_FIS_SATIRI->karsi_hesap_id ) );

        etkilenecek_hesap->setCurrentIndex ( etkilenecek_hesap->findText ( BNK_GET_HESAP_TURU_STRING( M_FIS_SATIRI->etkilenecek_hesap_turu ) ) );
        karsi_hesap->setCurrentIndex       ( karsi_hesap->findText       ( BNK_GET_HESAP_TURU_STRING      ( M_FIS_SATIRI->karsi_hesap_turu       ) ) );

        QString tam_hesap_kodu;
        QString hesap_ismi;

        MUH_GET_HESAP_KODU_HESAP_ISMI(M_FIS_SATIRI->karsi_hesap_id ,tam_hesap_kodu , hesap_ismi );


        searchedit_krs_hesap_kodu->SET_TEXT(tam_hesap_kodu);
        lineedit_krs_hesap_ismi->setText(hesap_ismi);



        double hesaba_giren      = commaedit_hesaba_giren->GET_DOUBLE();
        double hesaptan_cikan    = commaedit_hesaptan_cikan->GET_DOUBLE();
        double parite            = commaEdit_parite->GET_DOUBLE();
        double doviz_kuru        = commaEdit_doviz_kuru->GET_DOUBLE();

        if( parite > 0 ) {
            if ( hesaba_giren > 0 ) {
                commaEdit_tutar->SET_DOUBLE ( ROUND ( hesaba_giren * parite ) );
            }
            else if ( hesaptan_cikan > 0 ) {
                commaEdit_tutar->SET_DOUBLE ( ROUND ( hesaptan_cikan * parite ) );
            }
        }
        else if (  doviz_kuru > 0 ) {

            if ( hesaba_giren > 0 ) {
                commaEdit_tutar->SET_DOUBLE ( ROUND ( hesaba_giren * doviz_kuru ) );
            }
            else if ( hesaptan_cikan > 0 ) {
                commaEdit_tutar->SET_DOUBLE ( ROUND ( hesaptan_cikan * doviz_kuru ) );
            }
        }


        new_item = new QTableWidgetItem( select_query.VALUE(0).toString() );
        m_ui->tablewidget_fis_satirlari->setItem( row_number, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem(QString(record_id));
        m_ui->tablewidget_fis_satirlari->setItem( row_number, FIS_ID_COLUMN, new_item );

        SQL_QUERY query( DB );

        int muh_hesap_id = 0;

        switch ( M_FIS_SATIRI->etkilenecek_hesap_turu )
        {
        case ENUM_BNK_BANKA_HESABI:

            query.PREPARE_SELECT("bnk_hesaplar","muh_hesap_id","hesap_no_id = :hesap_no_id");
            query.SET_VALUE(":hesap_no_id" , M_FIS_SATIRI->etkilenecek_hesap_id );

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                muh_hesap_id = query.VALUE(0).toInt();
                searchedit_etk_muh_hesap_kodu->SET_TEXT   ( query.VALUE(0).toString() );

                if ( muh_hesap_id > 0 ) {

                    QString tam_hesap_kodu,hesap_ismi;

                    MUH_GET_HESAP_KODU_HESAP_ISMI(muh_hesap_id , tam_hesap_kodu , hesap_ismi);


                    searchedit_etk_muh_hesap_kodu->SET_TEXT   ( tam_hesap_kodu );
                    //lineedit_muh_hesap_ismi->setText ( hesap_ismi );

                }
            }
            searchedit_etk_muh_hesap_kodu->setEnabled( false );

            break;

        case ENUM_BNK_KREDI_KARTI_HESABI:

            query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari","muh_hesap_id","kredi_karti_id = :kredi_karti_id");
            query.SET_VALUE(":kredi_karti_id" , M_FIS_SATIRI->etkilenecek_hesap_id);

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                muh_hesap_id = query.VALUE(0).toInt();
                searchedit_etk_muh_hesap_kodu->SET_TEXT   ( BNK_GET_KREDI_KARTI_NUMARASI( M_FIS_SATIRI->etkilenecek_hesap_id ) );
            }
            break;

        case ENUM_BNK_POS_HESABI:
            {
                query.PREPARE_SELECT("bnk_hesabin_poslari","muh_hesap_id","pos_id = :pos_id");
                query.SET_VALUE(":pos_id" , M_FIS_SATIRI->etkilenecek_hesap_id);

                if( query.SELECT() NE 0 ) {
                    query.NEXT();
                    muh_hesap_id = query.VALUE(0).toInt();
                    searchedit_etk_muh_hesap_kodu->SET_TEXT   ( BNK_GET_POS_NUMARASI( M_FIS_SATIRI->etkilenecek_hesap_id ));
                }
                break;
            }
        }

        switch ( M_FIS_SATIRI->karsi_hesap_turu ) {
        case ENUM_BNK_CARI_HESAP       : {

                new_item->setText ( tr("Cari Hesap Kodu") );
                QString cari_hesap_kodu, cari_hesap_ismi;
                int     para_birim_id;

                int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FIS_SATIRI->karsi_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi , &para_birim_id );

                if ( result > 0 ) {
                    searchedit_krs_hesap_kodu->SET_TEXT   ( cari_hesap_kodu );
                    lineedit_krs_hesap_ismi->setText      ( cari_hesap_ismi );
                    //lineedit_cari_para_birimi_kodu->setText( para_birim_id ));
                }
                break;
            }
            case ENUM_BNK_MUHASEBE_HESABI :
            {

                new_item->setText ( tr("Muhasebe Hesap Kodu") );

                QString tam_hesap_kodu,hesap_ismi;

                MUH_GET_HESAP_KODU_HESAP_ISMI(M_FIS_SATIRI->karsi_hesap_id , tam_hesap_kodu , hesap_ismi);


                searchedit_krs_hesap_kodu->SET_TEXT   ( tam_hesap_kodu );
                lineedit_krs_hesap_ismi->setText      ( hesap_ismi );
            }

                break;

            case ENUM_BNK_POS_KOMISYON_HESABI:
            case ENUM_BNK_POS_HESABI        :

                new_item->setText("Pos Numarası");
                searchedit_krs_hesap_kodu->SET_TEXT ( BNK_GET_POS_NUMARASI( M_FIS_SATIRI->karsi_hesap_id ) );
                lineedit_krs_hesap_ismi->clear();
                lineedit_krs_hesap_ismi->setEnabled( false );

                break;

            case ENUM_BNK_KREDI_KARTI_HESABI:

                new_item->setText("Kredi Kartı Numarası");
                searchedit_krs_hesap_kodu->SET_TEXT ( BNK_GET_KREDI_KARTI_NUMARASI( M_FIS_SATIRI->karsi_hesap_id ) );
                lineedit_krs_hesap_ismi->clear();
                lineedit_krs_hesap_ismi->setEnabled( false );
                break;
            case ENUM_BNK_BANKA_HESABI :

                new_item->setText ( tr("Banka Hesap Kodu") );

                query.PREPARE_SELECT("bnk_hesaplar","banka_hesap_kodu, hesap_no_id","hesap_no_id = :hesap_no_id");

                query.SET_VALUE(":hesap_no_id" , M_FIS_SATIRI->karsi_hesap_id);

                if ( query.SELECT() > 0 ) {
                    query.NEXT();
                    searchedit_krs_hesap_kodu->SET_TEXT   ( query.VALUE(0).toString() );
                    lineedit_krs_hesap_ismi->setText (  BNK_GET_BANKA_ADI_SUBE_KODU_ADI_HESABI_NO  ( query.VALUE(1).toInt() ) );
                }
                break;

            default :
                break;
        }

    }

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void BNK_HAREKET_FISI::SET_LINE_DEFAULTS ( int row_number )
{

    QComboBox   * combobox_etkilenecek_hesap_turu = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, ETKILECEK_HESAP_COLUMN    );
    QLineEdit   * lineedit_banka_para_birim       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, BANKA_DOVIZ_ID            );
    QLineEdit   * lineedit_cari_para_birim        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, KRS_HESAP_PARA_BIRIM_ID   );

    QCommaEdit   * lineedit_doviz_kuru            = ( QCommaEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, DOVIZ_KURU                );
    QCommaEdit   * lineedit_parite                = ( QCommaEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, PARITE                    );
    QCommaEdit   * lineedit_tutar                 = ( QCommaEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, TUTAR                     );

//    QSearchEdit  * searchedit_etk_hesap_kodu      = ( QSearchEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ETK_HESAP_MUH_KODU_COLUMN );
//    QSearchEdit  * searchedit_krs_hesap_kodu      = ( QSearchEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KRS_HESAP_MUH_KODU_COLUMN );


    lineedit_parite->SET_PRECISION( 6 );
    lineedit_doviz_kuru->SET_PRECISION( 4 );


    lineedit_banka_para_birim->setMaxLength( 6 );
    lineedit_cari_para_birim->setMaxLength( 6 );


    lineedit_banka_para_birim->setEnabled( false );
    lineedit_cari_para_birim->setEnabled( false );

    lineedit_doviz_kuru->setEnabled( true );
    lineedit_parite->setEnabled( false );
    lineedit_tutar->setEnabled( false );

    QStringList etkilenecek_hesaplar;

    etkilenecek_hesaplar << tr( "BANKA" ) << tr( "KREDİ KARTI" ) << tr( "POS" );

    combobox_etkilenecek_hesap_turu->addItems           ( etkilenecek_hesaplar );
    combobox_etkilenecek_hesap_turu->setCurrentIndex    ( -1 );


    QComboBox * combobox_karsi_hesap_turu      = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KARSI_HESAP_COLUMN );

    QStringList karsi_hesaplar;

    karsi_hesaplar << tr( "BANKA" ) << tr( "MUHASEBE" ) << tr( "KREDİ KARTI" ) << tr( "POS" ) << tr( "CARİ" ) << tr( "KASA" ) << tr ( "POS KOMİSYON HESABI" );

    combobox_karsi_hesap_turu->addItems           ( karsi_hesaplar );
    combobox_karsi_hesap_turu->setCurrentIndex    ( -1 );

    // banka para yatirma ve cekme islemi
    if ( m_fis_turu EQ ENUM_BANKA_PARA_YAT_CEKME_FISI ) {
       FIND_AND_SET_COMBOBOX_TEXT( combobox_etkilenecek_hesap_turu, tr( "BANKA" ));
       combobox_etkilenecek_hesap_turu->setEnabled( false );


       FIND_AND_SET_COMBOBOX_TEXT( combobox_karsi_hesap_turu, tr( "KASA" ));
       combobox_karsi_hesap_turu->setEnabled( false );
    }


    QLineEdit * lineedit_aciklama = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ACIKLAMA_COLUMN);
    lineedit_aciklama->setMaxLength(128);

    QLineEdit * lineedit_muh_hesap_ismi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MUH_HESAP_ISMI_COLUMN);
    lineedit_muh_hesap_ismi->setReadOnly(true);
    lineedit_muh_hesap_ismi->setMaxLength( 60 );
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( object EQ m_ui->combo_box_banka_hesap_kodu ) {

        int row = -1;
        if ( object EQ m_ui->combo_box_banka_hesap_kodu ) {
            row = m_ui->combo_box_banka_hesap_kodu->view()->currentIndex().row();
        }
        else {
            int banka_hesap_id = OPEN_BNK_HESAPLARI_ARAMA ( this );
            row = BNK_HESAPLARI_MODEL_INDEX_BUL ( banka_hesap_id, m_ui->combo_box_banka_hesap_kodu );
            if ( row > -1 ) {
                m_ui->combo_box_banka_hesap_kodu->setCurrentIndex ( row );
            }
        }
        if ( row EQ -1 ) {
            return ADAK_FAIL;
        }
        m_bnk_hesap_no_id         = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 0 ).toInt();
        m_banka_hesabi_doviz_id   = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->combo_box_banka_hesap_kodu, row, 1 ).toInt();
        m_ui->lineedit_doviz_cinsi->setText ( DVZ_GET_DOVIZ_KODU ( m_banka_hesabi_doviz_id ) );

        QString banka_bilgisi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->combo_box_banka_hesap_kodu, row, 3 ).toString() + " / " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->combo_box_banka_hesap_kodu, row, 4 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 5 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 6 ).toString();

        m_ui->lineEdit_banka_bilgisi->setText ( banka_bilgisi );
        m_ui->combo_box_banka_hesap_kodu->setCurrentIndex(row);

    }
    else if ( object EQ m_ui->adakDate_fis_tarihi ) {
        M_FIS->fis_tarihi   = m_ui->adakDate_fis_tarihi->DATE();
    }

    return ADAK_OK;    
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_FIS_FORM_EMPTY()
{

    if ( m_ui->lineedit_fis_no->text().isEmpty() EQ true ) {
        MSG_INFO( tr("Yeni Banka Fişi kaydı için (*) girmelisiniz"), m_ui->lineedit_fis_no );

        return ADAK_FAIL;
    }

    if ( m_ui->combo_box_banka_hesap_kodu->currentText().isEmpty() EQ true ) {
        MSG_INFO( tr("Banka Hesap Kodunu boş bırakamazsınız!.."), m_ui->combo_box_banka_hesap_kodu );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_FIS_RECORD_ADD()
{
    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) NE "*") {

        SQL_QUERY sql_query ( DB );

        sql_query.PREPARE_SELECT("bnk_hareket_fisler","fis_id","fis_tarihi = :fis_tarihi "
                                 "AND fis_no = :fis_no ");

        sql_query.SET_VALUE ( ":fis_tarihi",   m_ui->adakDate_fis_tarihi->DATE());
        sql_query.SET_VALUE ( ":fis_no"    ,    m_ui->lineedit_fis_no->text().toInt() );

        if ( sql_query.SELECT() > 0 ) {
            MSG_WARNING( tr("Aynı fiş numarası ile kayıtlı fiş bulundu. Fiş numarasını değiştiriniz."), m_ui->lineedit_fis_no );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::ADD_FIS_RECORD
***************************************************************************************/

int BNK_HAREKET_FISI::ADD_FIS_RECORD()
{
    M_FIS->muh_kasa_hesap_id                = M_ENT_DETAYLARI->muh_kasa_hesap_id;
    M_FIS->fis_no                           = m_ui->lineedit_fis_no->text().toInt();
    M_FIS->fis_tarihi                       = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->hesap_no_id                      = m_bnk_hesap_no_id;
    M_FIS->aciklama                         = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->program_id                       = E9_PROGRAMI;
    M_FIS->modul_id                         = BANKA_MODULU;
    M_FIS->base_fis_id                      = 0;
    M_FIS->fis_turu                         = m_fis_turu;
    M_FIS->muh_fis_id                       = 0;
    M_FIS->odm_emr_hesabi_etkilenecek_mi    = 1;
    M_FIS->para_birim_id                    = m_banka_hesabi_doviz_id;


    if ( m_fis_turu EQ ENUM_BANKA_PARA_YAT_CEKME_FISI) {
        M_FIS->base_fis_turu    = ENUM_BANKA_PARA_YAT_CEKME_FISI;
    }
    else {
        M_FIS->base_fis_turu    = ENUM_BANKA_HAREKET_FISI;
    }


    int fis_id = BNK_HAREKET_FIS_EKLE( M_FIS , 0 );

    m_ui->commaEdit_toplam_hesaba_giren->SET_DOUBLE( M_FIS->toplam_hesaba_giren );
    m_ui->commaEdit_toplam_hesaptan_cikan->SET_DOUBLE( M_FIS->toplam_hesaptan_cikan );

    return fis_id;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_FIS_RECORD_UPDATE(int record_id)
{
    Q_UNUSED ( record_id );

    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) NE "*" ) {

       int fis_no = m_ui->lineedit_fis_no->text().toInt();

       SQL_QUERY select_query ( DB );
       select_query.PREPARE_SELECT("bnk_hareket_fisler","fis_id","fis_no = :fis_no AND fis_tarihi = :fis_tarihi");

       select_query.SET_VALUE(":fis_no"      ,fis_no );
       select_query.SET_VALUE(":fis_tarihi"  ,m_ui->adakDate_fis_tarihi->DATE());

       if ( select_query.SELECT() > 0 ) {
           select_query.NEXT();
           if ( select_query.VALUE(0) NE record_id ) {
               MSG_WARNING( tr("Aynı Fiş No ile kayıtlı başka bir fiş bulundu. Farklı bir fiş numarası giriniz"), m_ui->lineedit_fis_no );
               return ADAK_FAIL;
           }
       }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void BNK_HAREKET_FISI::UPDATE_FIS_RECORD(int record_id)
{

    M_FIS->fis_no            = m_ui->lineedit_fis_no->text().toInt();
    M_FIS->fis_tarihi        = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->hesap_no_id       = m_bnk_hesap_no_id;
    M_FIS->aciklama          = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->program_id        = E9_PROGRAMI;
    M_FIS->modul_id          = BANKA_MODULU;
    M_FIS->fis_turu          = m_fis_turu;
    M_FIS->muh_kasa_hesap_id = M_ENT_DETAYLARI->muh_kasa_hesap_id;


    if ( m_fis_turu EQ ENUM_BANKA_PARA_YAT_CEKME_FISI) {
        M_FIS->base_fis_turu    = ENUM_BANKA_PARA_YAT_CEKME_FISI;
    }
    else {
        M_FIS->base_fis_turu    = ENUM_BANKA_HAREKET_FISI;
    }

    BNK_HAREKET_FISI_GUNCELLE( M_FIS, record_id );

    m_ui->commaEdit_toplam_hesaba_giren->SET_DOUBLE( M_FIS->toplam_hesaba_giren );
    m_ui->commaEdit_toplam_hesaptan_cikan->SET_DOUBLE( M_FIS->toplam_hesaptan_cikan );

    ADD_ADAK_EVENT( ADAK_REFRESH );

}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

void BNK_HAREKET_FISI::DELETE_FIS_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );

    MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT( MUH_FIS );

    MUH_FIS_BILGILERINI_OKU( MUH_FIS, M_FIS->muh_fis_id );

    MUH_FIS_SIL( MUH_FIS, M_FIS->muh_fis_id );

    BNK_HAREKET_FISI_SIL( M_FIS, record_id );

    delete MUH_FIS;


}

/**************************************************************************************
                   BNK_HAREKET_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int BNK_HAREKET_FISI::SELECT_FIS_RECORD()
{
    return OPEN_BNK_HAREKET_FISI_ARAMA ( m_fis_turu, m_modul_id, ENUM_BANKA_FISI, this );
}

/**************************************************************************************
                   BNK_HAREKET_FISI::FIND_FIS_RECORD
***************************************************************************************/

int BNK_HAREKET_FISI::FIND_FIS_RECORD()
{
    if (m_ui->lineedit_fis_no->text().toInt() EQ 0 ) {
        MSG_WARNING( tr("Aranacak fiş numarasını,Fiş No alanına yazmalısınız!.."), m_ui->lineedit_fis_no );

        return 0;
    }

    SQL_QUERY select_query(DB);

    QString where_str = "fis_no = :fis_no "
                        "AND fis_turu = :fis_turu  "
                        "AND fis_tarihi = :fis_tarihi";

    if( m_modul_id EQ 0 ) {
        where_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        where_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT( "bnk_hareket_fisler","fis_id",  where_str );

    select_query.SET_VALUE(":fis_no",    m_ui->lineedit_fis_no->text().toInt()                     );
    select_query.SET_VALUE(":fis_turu",  m_fis_turu                                                );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id", m_modul_id                                            );
    }
    select_query.SET_VALUE(":fis_tarihi", m_ui->adakDate_fis_tarihi->DATE() );

    if (select_query.SELECT() EQ 0 ) {
        MSG_WARNING(tr("Aradığınız Banka Fişi bulunamadı!.."), m_ui->lineedit_fis_no);

        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_HAREKET_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int BNK_HAREKET_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    QString where_str = "fis_turu  = :fis_turu ";

    if( m_modul_id EQ 0 ) {
        where_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        where_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT (  "bnk_hareket_fisler","fis_id",where_str);

    select_query.SET_VALUE(":fis_turu"    , m_fis_turu   );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id" , m_modul_id );
    }

    if ( select_query.SELECT("fis_tarihi, fis_no",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   BNK_HAREKET_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int BNK_HAREKET_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    QString where_str = "fis_turu  = :fis_turu ";

    if( m_modul_id EQ 0 ) {
        where_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        where_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT (  "bnk_hareket_fisler","fis_id",where_str );

    select_query.SET_VALUE(":fis_turu"    , m_fis_turu   );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id"    , m_modul_id );
    }

    if ( select_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_HAREKET_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int BNK_HAREKET_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString where_str = "fis_no   < :fis_no               "
                        "AND   fis_turu       = :fis_turu       "
                        "AND   fis_tarihi     = :fis_tarihi     ";

    if( m_modul_id EQ 0 ) {
        where_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        where_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT("bnk_hareket_fisler","fis_id", where_str  );

    select_query.SET_VALUE(":fis_no"       , m_ui->lineedit_fis_no->text().toInt()                      );
    select_query.SET_VALUE(":fis_turu"     , m_fis_turu                                                 );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id"  , m_modul_id                                                 );
    }
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE()  );

    if ( select_query.SELECT("fis_no DESC",0,1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    where_str = "fis_turu       = :fis_turu      "
                "AND   fis_tarihi     < :fis_tarihi    ";

    if( m_modul_id EQ 0 ) {
        where_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        where_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT( "bnk_hareket_fisler","fis_id",where_str  );

    select_query.SET_VALUE(":fis_turu"     , m_fis_turu         );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id"  , m_modul_id         );
    }
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE());

    if ( select_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_HAREKET_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int BNK_HAREKET_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString where_str = "fis_no   > :fis_no               "
                        "AND   fis_turu       = :fis_turu       "
                        "AND   fis_tarihi     = :fis_tarihi     ";

    if( m_modul_id EQ 0 ) {
        where_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        where_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT( "bnk_hareket_fisler","fis_id",where_str );

    select_query.SET_VALUE(":fis_no"       , m_ui->lineedit_fis_no->text().toInt()              );
    select_query.SET_VALUE(":fis_turu"     , m_fis_turu                                         );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id"  , m_modul_id                                         );
    }
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE());

    if ( select_query.SELECT("fis_no",0,1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    where_str = "fis_turu       = :fis_turu       "
                "AND   fis_tarihi     > :fis_tarihi     ";

    if( m_modul_id EQ 0 ) {
        where_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        where_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT( "bnk_hareket_fisler","fis_id",where_str );

    select_query.SET_VALUE(":fis_turu"     , m_fis_turu         );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id"  , m_modul_id         );
    }
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE());

    if ( select_query.SELECT("fis_tarihi, fis_no",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_LINE_VAR
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{

    QSearchEdit * searchedit_krs_muh_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KRS_HESAP_MUH_KODU_COLUMN    );
    QSearchEdit * searchedit_etk_muh_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ETK_HESAP_MUH_KODU_COLUMN    );
    QLineEdit   * lineedit_krs_hesap_ismi       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, MUH_HESAP_ISMI_COLUMN        );
    QLineEdit   * lineedit_krs_hesap_id         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KRS_HESAP_ID_COLUMN          );
    QLineEdit   * lineedit_etk_hesap_id         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ETK_HESAP_ID_COLUMN          );
    QComboBox   * combo_box_etkilenecek_hesap   = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ETKILECEK_HESAP_COLUMN       );
    QComboBox   * combo_box_karsi_hesap         = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KARSI_HESAP_COLUMN           );
    QLineEdit   * lineedit_banka_para_birim     = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, BANKA_DOVIZ_ID               );
    QCommaEdit  * commaEdit_doviz_kuru          = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, DOVIZ_KURU                   );
    QCommaEdit  * commaEdit_parite              = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, PARITE                       );
    QCommaEdit  * commaEdit_tutar               = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, TUTAR                        );
    QCommaEdit  * commaEdit_hesaba_giren        = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, HESABA_GIREN_COLUMN          );
    QCommaEdit  * commaEdit_hesaptan_cikan      = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, HESAPTAN_CIKAN_COLUMN        );

    // MUTLAKA BIR BANKA SECILMIS OLMASI GEREKIR.
    if( object EQ combo_box_etkilenecek_hesap OR  object EQ combo_box_karsi_hesap ) {
        if ( m_bnk_hesap_no_id EQ 0 ) {
            MSG_INFO( tr( "Banka Hesabi Seçiniz!!!" ), m_ui->combo_box_banka_hesap_kodu );
            return ADAK_FAIL;
        }
        else {
            if ( object EQ combo_box_etkilenecek_hesap ) {
                if ( combo_box_etkilenecek_hesap->currentText() EQ tr("BANKA") ) {
                    searchedit_etk_muh_hesap_kodu->SET_TEXT( MUH_GET_HESAP_KODU( BNK_GET_BANKA_MUH_HESAP_ID( m_bnk_hesap_no_id ) ) );
                }
            }
        }
    }
    if ( m_banka_hesabi_doviz_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
        commaEdit_doviz_kuru->clear();
        commaEdit_doviz_kuru->setEnabled( false );
    }
    else {
        commaEdit_doviz_kuru->setEnabled( true );
        commaEdit_doviz_kuru->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( m_banka_hesabi_doviz_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));
    }
    if( object EQ combo_box_etkilenecek_hesap ) {

        lineedit_etk_hesap_id->setText      ( QVariant( m_bnk_hesap_no_id ).toString() );
        lineedit_banka_para_birim->setText  ( m_ui->lineedit_doviz_cinsi->text() );

        QComboBox * combobox_karsi_hesap_turu      = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KARSI_HESAP_COLUMN );
        combobox_karsi_hesap_turu->clear();

        QStringList karsi_hesaplar;

        if ( combo_box_etkilenecek_hesap->currentText() EQ tr( "POS" ) ) {
            searchedit_etk_muh_hesap_kodu->setEnabled( true );
            etkilenecek_hesap_turu = ENUM_BNK_POS_HESABI;
            karsi_hesaplar << tr( "BANKA" ) << tr( "MUHASEBE" ) << tr( "KREDİ KARTI" ) << tr( "CARİ" ) << tr( "KASA" ) << tr ( "POS KOMİSYON HESABI" );
        }
        else if( combo_box_etkilenecek_hesap->currentText() EQ tr( "KREDİ KARTI" ) ){
            searchedit_etk_muh_hesap_kodu->setEnabled( true );
            etkilenecek_hesap_turu = ENUM_BNK_KREDI_KARTI_HESABI;
            karsi_hesaplar << tr( "BANKA" ) << tr( "MUHASEBE" ) << tr( "POS" ) << tr( "CARİ" ) << tr( "KASA" ) << tr ( "POS KOMİSYON HESABI" );
        }
        else{
            searchedit_etk_muh_hesap_kodu->setEnabled( false );
            etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
            karsi_hesaplar << tr( "MUHASEBE" ) << tr( "KREDİ KARTI" ) << tr( "POS" ) << tr( "CARİ" ) << tr( "KASA" ) << tr ( "POS KOMİSYON HESABI" );
        }

        combobox_karsi_hesap_turu->addItems           ( karsi_hesaplar );
        combobox_karsi_hesap_turu->setCurrentIndex    ( -1 );

    }

    if( object EQ combo_box_karsi_hesap ) {
         karsi_hesap_turu = BNK_GET_HESAP_TURU_ENUM( combo_box_karsi_hesap->currentText() );
         searchedit_krs_muh_hesap_kodu->CLEAR();
    }
    if ( object EQ searchedit_etk_muh_hesap_kodu ) {
        if ( searchedit_etk_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_krs_hesap_id->clear();
            lineedit_krs_hesap_ismi->clear();
            return ADAK_OK;
        }
    }
    if( object EQ searchedit_krs_muh_hesap_kodu ) {

        QString krs_hesap_kodu = searchedit_krs_muh_hesap_kodu->GET_TEXT();

        SQL_QUERY query( DB );

        switch( BNK_GET_HESAP_TURU_ENUM( combo_box_karsi_hesap->currentText() ) ) {

        case ENUM_BNK_CARI_HESAP: {
              QString cari_hesap_ismi;
              int     para_birim_id, cari_hesap_id;

              int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( krs_hesap_kodu, &cari_hesap_id, &cari_hesap_ismi, &para_birim_id );

              if( result EQ 0 ) {
                  MSG_INFO( tr( "Böyle bir Cari Hesap yok!!!"), searchedit_krs_muh_hesap_kodu );
                  return ADAK_FAIL_UNDO;
              }

              lineedit_krs_hesap_id->setText( QVariant( cari_hesap_id ).toString() );
              int cari_para_birim_id    = para_birim_id;

              if ( m_banka_hesabi_doviz_id NE DVZ_GET_TEMEL_PARA_BIRIMI_ID() ){
                  if ( m_banka_hesabi_doviz_id NE cari_para_birim_id ) {
                      if ( cari_para_birim_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {

                          commaEdit_parite->setEnabled( false );
                          commaEdit_doviz_kuru->setEnabled( true );
                          commaEdit_doviz_kuru->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( m_banka_hesabi_doviz_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));

                          if ( commaEdit_hesaba_giren->GET_DOUBLE() > 0 ) {
                              commaEdit_tutar->SET_DOUBLE( ROUND( commaEdit_hesaba_giren->GET_DOUBLE() * commaEdit_doviz_kuru->GET_DOUBLE() ));
                          }
                          else {
                              commaEdit_tutar->SET_DOUBLE( ROUND( commaEdit_hesaptan_cikan->GET_DOUBLE() * commaEdit_doviz_kuru->GET_DOUBLE() ));
                          }

                      }
                      else {

                          commaEdit_parite->setEnabled( true );
                          commaEdit_parite->SET_DOUBLE( DVZ_PARITEYI_AL( cari_para_birim_id, m_banka_hesabi_doviz_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));

                          if ( commaEdit_hesaba_giren->GET_DOUBLE() > 0 ) {
                              commaEdit_tutar->SET_DOUBLE( ROUND( commaEdit_hesaba_giren->GET_DOUBLE() * commaEdit_parite->GET_DOUBLE() ));
                          }
                          else {
                              commaEdit_tutar->SET_DOUBLE( ROUND( commaEdit_hesaptan_cikan->GET_DOUBLE() * commaEdit_parite->GET_DOUBLE() ));
                          }

                      }

                  }
                  else {
                       commaEdit_parite->clear();
                       commaEdit_parite->setEnabled( false );

                       if ( commaEdit_hesaba_giren->GET_DOUBLE() > 0 ) {
                           commaEdit_tutar->SET_DOUBLE( ROUND( commaEdit_hesaba_giren->GET_DOUBLE() ));
                       }
                       else {
                           commaEdit_tutar->SET_DOUBLE( ROUND( commaEdit_hesaptan_cikan->GET_DOUBLE() ));
                       }
                  }
              }
              else {
                  commaEdit_parite->setEnabled( false );
                  commaEdit_doviz_kuru->setEnabled( true );
                  commaEdit_doviz_kuru->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( cari_para_birim_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));

                  if ( commaEdit_hesaba_giren->GET_DOUBLE() > 0 ) {
                      commaEdit_tutar->SET_DOUBLE( ROUND( commaEdit_hesaba_giren->GET_DOUBLE() * commaEdit_doviz_kuru->GET_DOUBLE() ));
                  }
                  else {
                      commaEdit_tutar->SET_DOUBLE( ROUND( commaEdit_hesaptan_cikan->GET_DOUBLE() * commaEdit_doviz_kuru->GET_DOUBLE() ));
                  }
              }

            break;
            }
        case ENUM_BNK_POS_HESABI: {

                query.PREPARE_SELECT( "bnk_hesabin_poslari","pos_id","pos_numarasi =:pos_numarasi" );
                query.SET_VALUE( ":pos_numarasi", krs_hesap_kodu );

                if( query.SELECT() EQ 0 ) {
                    MSG_INFO( tr( "Böyle bir Pos Numarasi yok!!!"), searchedit_krs_muh_hesap_kodu );
                    return ADAK_FAIL_UNDO;
                }
                query.NEXT();

                lineedit_krs_hesap_id->setText( QVariant( query.VALUE( 0 ).toInt() ).toString() );

            break;
            }

        case ENUM_BNK_KREDI_KARTI_HESABI: {

                query.PREPARE_SELECT( "bnk_hesabin_kredi_kartlari","kredi_karti_id","kredi_karti_numarasi =:kredi_karti_numarasi" );
                query.SET_VALUE( ":kredi_karti_numarasi", krs_hesap_kodu );

                if( query.SELECT() EQ 0 ) {
                    MSG_INFO( tr( "Böyle bir Kredi Kart Numarasi yok!!!"), searchedit_krs_muh_hesap_kodu );

                    return ADAK_FAIL_UNDO;
                }
                query.NEXT();

                lineedit_krs_hesap_id->setText( QVariant( query.VALUE( 0 ).toInt() ).toString() );

                break;
            }
        case ENUM_BNK_MUHASEBE_HESABI:
            {

                MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

                MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

                MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , krs_hesap_kodu);


                int muh_id      = MUHASEBE_HESABI->hesap_id;
                int tali_hesap  = MUHASEBE_HESABI->tali_hesap;

                if ( tali_hesap EQ 0 ) {
                    MSG_INFO( tr( "Seçtiğiniz Hesap Tali Hesap değil!!!"), searchedit_krs_muh_hesap_kodu );
                    return ADAK_FAIL;
                }
                else {
                    lineedit_krs_hesap_id->setText( QVariant( muh_id ).toString() );
                    lineedit_krs_hesap_ismi->setText( MUHASEBE_HESABI->hesap_ismi );
                }

                break;
            }
        case ENUM_BNK_BANKA_HESABI: {

                query.PREPARE_SELECT( "bnk_hesaplar","hesap_no_id","banka_hesap_kodu =:banka_hesap_kodu" );
                query.SET_VALUE( ":banka_hesap_kodu", krs_hesap_kodu );

                if( query.SELECT() EQ 0 ) {
                    MSG_INFO( tr( "Böyle bir Banka Hesabı yok!!!"), searchedit_krs_muh_hesap_kodu );

                    return ADAK_FAIL_UNDO;
                }
                query.NEXT();

                int muh_id = query.VALUE( 0 ).toInt();

                lineedit_krs_hesap_id->setText( QVariant( muh_id ).toString() );
                lineedit_krs_hesap_ismi->setText(  BNK_GET_BANKA_ADI_SUBE_KODU_ADI_HESABI_NO  ( query.VALUE( 0 ).toInt() ) );

                break;
            }
        case ENUM_BNK_POS_KOMISYON_HESABI: {

                query.PREPARE_SELECT( "bnk_hesabin_poslari","pos_id","pos_numarasi =:pos_numarasi" );
                query.SET_VALUE( ":pos_numarasi", krs_hesap_kodu );

                if( query.SELECT() EQ 0 ) {
                    MSG_INFO( tr( "Böyle bir Pos Numarasi yok!!!"), searchedit_krs_muh_hesap_kodu );

                    return ADAK_FAIL_UNDO;
                }
                query.NEXT();

                lineedit_krs_hesap_id->setText( QVariant( query.VALUE( 0 ).toInt() ).toString() );

            break;
            }
        case ENUM_BNK_KASA_HESABI :

            {
                M_FIS_SATIRI->karsi_hesap_id = MUH_GET_HESAP_ID(searchedit_krs_muh_hesap_kodu->GET_TEXT());

                lineedit_krs_hesap_ismi->setText(MUH_GET_HESAP_ISMI(searchedit_krs_muh_hesap_kodu->GET_TEXT()));

                lineedit_krs_hesap_id->setText(QVariant(M_FIS_SATIRI->karsi_hesap_id).toString());
            }
            break;
        }

    }

    if( object EQ commaEdit_hesaba_giren OR object EQ commaEdit_hesaptan_cikan ) {

        if( commaEdit_hesaptan_cikan->GET_DOUBLE() > 0  AND commaEdit_hesaba_giren->GET_DOUBLE() > 0 ){
            MSG_INFO( tr( "Hesaba Giren veya Hesaptan Çıkan' dan Yalniz Birini doldurmabız Gerekir..." ), commaEdit_hesaba_giren );
        }

        if ( commaEdit_doviz_kuru->GET_DOUBLE() > 0.00 ) {
            if ( commaEdit_hesaba_giren->GET_DOUBLE() > 0.00 ) {
                commaEdit_tutar->SET_DOUBLE(  ROUND( commaEdit_hesaba_giren->GET_DOUBLE() * commaEdit_doviz_kuru->GET_DOUBLE() ) );
            }
            else {
                commaEdit_tutar->SET_DOUBLE(  ROUND( commaEdit_hesaptan_cikan->GET_DOUBLE() * commaEdit_doviz_kuru->GET_DOUBLE() ) );
            }
        }
        if ( commaEdit_parite->GET_DOUBLE() > 0.00 ) {
            if ( commaEdit_hesaba_giren->GET_DOUBLE() > 0.00 ) {
                commaEdit_tutar->SET_DOUBLE(  ROUND( commaEdit_hesaba_giren->GET_DOUBLE() * commaEdit_parite->GET_DOUBLE() ) );
            }
            else {
                commaEdit_tutar->SET_DOUBLE(  ROUND( commaEdit_hesaptan_cikan->GET_DOUBLE() * commaEdit_parite->GET_DOUBLE() ) );
            }
        }

    }

    if ( object EQ commaEdit_parite ) {

        double hesaba_giren      = commaEdit_hesaba_giren->GET_DOUBLE();
        double hesaptan_cikan    = commaEdit_hesaptan_cikan->GET_DOUBLE();
        double parite            = commaEdit_parite->GET_DOUBLE();

        if ( hesaba_giren > 0 ) {
            commaEdit_tutar->SET_DOUBLE ( ROUND ( hesaba_giren * parite ) );
        }
        else if ( hesaptan_cikan > 0 ) {
            commaEdit_tutar->SET_DOUBLE ( ROUND ( hesaptan_cikan * parite ) );
        }
    }
    else if ( object EQ commaEdit_doviz_kuru ) {

        double hesaba_giren      = commaEdit_hesaba_giren->GET_DOUBLE();
        double hesaptan_cikan    = commaEdit_hesaptan_cikan->GET_DOUBLE();
        double doviz_kuru        = commaEdit_doviz_kuru->GET_DOUBLE();

        if ( hesaba_giren > 0 ) {
            commaEdit_tutar->SET_DOUBLE ( ROUND ( hesaba_giren * doviz_kuru ) );
        }
        else if ( hesaptan_cikan > 0 ) {
            commaEdit_tutar->SET_DOUBLE ( ROUND ( hesaptan_cikan * doviz_kuru ) );
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_LINE_EMPTY(int row_number)
{

    QComboBox   * combo_box_etkilenecek_hesap    = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ETKILECEK_HESAP_COLUMN       );
    QComboBox   * combo_box_karsi_hesap          = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KARSI_HESAP_COLUMN           );
    QSearchEdit * searchedit_krs_muh_hesap_kodu  = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KRS_HESAP_MUH_KODU_COLUMN    );
    QSearchEdit * searchedit_etk_muh_hesap_kodu  = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ETK_HESAP_MUH_KODU_COLUMN    );

    QCommaEdit   * commaEdit_hesaba_giren        = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, HESABA_GIREN_COLUMN         );
    QCommaEdit   * commaEdit_hesaptan_cikan      = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, HESAPTAN_CIKAN_COLUMN       );


    if ( searchedit_etk_muh_hesap_kodu->GET_TEXT().isEmpty() ) {
        if ( BNK_GET_HESAP_TURU_ENUM( combo_box_etkilenecek_hesap->currentText()) NE ENUM_BNK_BANKA_HESABI ) {
            MSG_INFO( tr( "Lütfen Etkilenecek Hesap türünü seçin.." ), searchedit_etk_muh_hesap_kodu );
            return ADAK_FAIL;
        }
    }
    if ( searchedit_krs_muh_hesap_kodu->GET_TEXT().isEmpty() ) {

        if ( BNK_GET_HESAP_TURU_ENUM( combo_box_karsi_hesap->currentText()) NE ENUM_BNK_KASA_HESABI ) {
            MSG_INFO( tr( "Lütfen Karşı Hesap türünü seçin.." ), searchedit_krs_muh_hesap_kodu );
            return ADAK_FAIL;
        }
    }
    if ( combo_box_etkilenecek_hesap->currentIndex() EQ -1 ) {
        MSG_INFO( tr( "Lütfen Etkilenecek Hesap türünü seçin.." ), combo_box_etkilenecek_hesap );
        return ADAK_FAIL;
    }
    if ( commaEdit_hesaba_giren->GET_DOUBLE() EQ 0.00 ) {
        if ( commaEdit_hesaptan_cikan->GET_DOUBLE() NE 0.00 ) {
            return ADAK_OK;
        }
        else {
        MSG_WARNING( tr( "Lütfen Hesaba giren tutarı giriniz. " ), commaEdit_hesaba_giren );
        return ADAK_FAIL;
        }

    }
    else {
        if ( commaEdit_hesaptan_cikan->GET_DOUBLE() NE 0.00 ) {
            MSG_WARNING( tr( "Sadece bir tutar girebilirsiniz." ), commaEdit_hesaba_giren );
            return ADAK_FAIL;
        }
    }

    if ( combo_box_karsi_hesap->currentIndex() EQ -1 ) {
        MSG_INFO( tr( "Lütfen Karşı Hesap türünü seçin.." ), combo_box_karsi_hesap );
        return ADAK_FAIL;
    }
    if ( combo_box_etkilenecek_hesap->currentIndex() NE -1 ){

        switch ( BNK_GET_HESAP_TURU_ENUM( combo_box_etkilenecek_hesap->currentText() ) ) {
        case ENUM_BNK_BANKA_HESABI: {
                break;
            }
        case ENUM_BNK_POS_HESABI:
        case ENUM_BNK_KASA_HESABI:
            {
                if ( searchedit_krs_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true  ){
                    MSG_INFO( tr( "Lütfen Etkilenen Hesabı seçin.." ), searchedit_etk_muh_hesap_kodu );
                    return ADAK_FAIL;
                }
            }
        default: break;

        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_ADD_LINE
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_ADD_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::ADD_LINE
***************************************************************************************/

void BNK_HAREKET_FISI::ADD_LINE(int record_id, int row_number)
{

    BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT( M_FIS_SATIRI );

    QLineEdit   * lineedit_aciklama                 = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ACIKLAMA_COLUMN          );
    QCommaEdit  * commaedit_hesaba_giren            = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESABA_GIREN_COLUMN      );
    QCommaEdit  * commaedit_hesaptan_cikan          = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAPTAN_CIKAN_COLUMN    );
    QLineEdit   * lineedit_krs_hesap_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KRS_HESAP_ID_COLUMN      );
    QLineEdit   * lineedit_etk_hesap_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ETK_HESAP_ID_COLUMN      );
    QLineEdit   * lineedit_banka_para_birim         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BANKA_DOVIZ_ID           );
    QComboBox   * combobox_etkilenecek_hesap_turu   = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ETKILECEK_HESAP_COLUMN  );
    QComboBox   * combobox_kari_hesap_turu          = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KARSI_HESAP_COLUMN      );
    QLineEdit   * lineedit_doviz_kuru               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, DOVIZ_KURU              );
    QLineEdit   * lineedit_parite                   = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, PARITE                  );

    if ( m_fis_turu EQ ENUM_BANKA_PARA_YAT_CEKME_FISI ) {
        lineedit_krs_hesap_id->setText( QVariant ( M_ENT_DETAYLARI->muh_kasa_hesap_id ).toString() );
    }


    M_FIS_SATIRI->fis_id                = record_id;
    M_FIS_SATIRI->aciklama              = lineedit_aciklama->text();
    M_FIS_SATIRI->hesaba_giren          = ROUND( commaedit_hesaba_giren->GET_DOUBLE()   );
    M_FIS_SATIRI->hesaptan_cikan        = ROUND( commaedit_hesaptan_cikan->GET_DOUBLE() );
    M_FIS_SATIRI->etkilenecek_hesap_turu= BNK_GET_HESAP_TURU_ENUM( combobox_etkilenecek_hesap_turu->currentText() );
    M_FIS_SATIRI->program_id            = E9_PROGRAMI;
    M_FIS_SATIRI->modul_id              = BANKA_MODULU;

    lineedit_banka_para_birim->setText( m_ui->lineedit_doviz_cinsi->text() );

    if ( M_FIS_SATIRI->etkilenecek_hesap_turu EQ BNK_GET_HESAP_TURU_ENUM( "BANKA" ) ){
         M_FIS_SATIRI->etkilenecek_hesap_id  = m_bnk_hesap_no_id;
    }
    else {
         M_FIS_SATIRI->etkilenecek_hesap_id  = lineedit_etk_hesap_id->text().toInt();
    }

    M_FIS_SATIRI->karsi_hesap_turu          = BNK_GET_HESAP_TURU_ENUM( combobox_kari_hesap_turu->currentText() );
    M_FIS_SATIRI->karsi_hesap_id            = lineedit_krs_hesap_id->text().toInt();
    M_FIS_SATIRI->doviz_kuru                = lineedit_doviz_kuru->text().toDouble();
    M_FIS_SATIRI->parite                    = lineedit_parite->text().toDouble();
    M_FIS_SATIRI->program_id                = M_FIS->program_id;
    M_FIS_SATIRI->modul_id                  = M_FIS->modul_id;
    M_FIS_SATIRI->base_record_id            = 0;
    M_FIS_SATIRI->karsi_hesap_eklenecek_mi  = 1;

    int fis_satiri_id = BNK_HAREKET_FIS_SATIRI_EKLE( M_FIS, M_FIS_SATIRI );


    QTableWidgetItem * new_item;
    new_item = new QTableWidgetItem( QVariant ( fis_satiri_id ).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number,ROW_ID_COLUMN,new_item);

    new_item = new QTableWidgetItem(QVariant(record_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number,FIS_ID_COLUMN,new_item);

    m_ui->commaEdit_toplam_hesaba_giren->SET_DOUBLE( M_FIS->toplam_hesaba_giren );
    m_ui->commaEdit_toplam_hesaptan_cikan->SET_DOUBLE( M_FIS->toplam_hesaptan_cikan );

}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::UPDATE_LINE
***************************************************************************************/

void BNK_HAREKET_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);

    BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT( M_FIS_SATIRI );

    QLineEdit   * lineedit_aciklama                 = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ACIKLAMA_COLUMN              );
    QCommaEdit  * commaedit_hesaba_giren            = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESABA_GIREN_COLUMN          );
    QCommaEdit  * commaedit_hesaptan_cikan          = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAPTAN_CIKAN_COLUMN        );
    QLineEdit   * lineedit_muh_hesap_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KRS_HESAP_ID_COLUMN          );
    QLineEdit   * lineedit_etk_hesap_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ETK_HESAP_ID_COLUMN          );
    QComboBox   * combobox_etkilenecek_hesap_turu   = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ETKILECEK_HESAP_COLUMN      );
    QComboBox   * combobox_kari_hesap_turu          = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KARSI_HESAP_COLUMN          );
    QLineEdit   * lineedit_doviz_kuru               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, DOVIZ_KURU                  );
    QLineEdit   * lineedit_parite                   = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, PARITE                      );
    QLineEdit   * lineedit_program_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,PROGRAM_ID_COLUMN            );
    QLineEdit   * lineedit_modul_id                 = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MODUL_ID_COLUMN              );
    QLineEdit   * lineedit_base_record_id           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BASE_RECORD_ID_COLUMN        );

    M_FIS_SATIRI->program_id            = lineedit_program_id->text().toInt();
    M_FIS_SATIRI->modul_id              = lineedit_modul_id->text().toInt();
    M_FIS_SATIRI->base_record_id        = lineedit_base_record_id->text().toInt();
    M_FIS_SATIRI->fis_id                = record_id;
    M_FIS_SATIRI->aciklama              = lineedit_aciklama->text();
    M_FIS_SATIRI->hesaba_giren          = ROUND( commaedit_hesaba_giren->GET_DOUBLE() );
    M_FIS_SATIRI->hesaptan_cikan        = ROUND( commaedit_hesaptan_cikan->GET_DOUBLE() );
    M_FIS_SATIRI->etkilenecek_hesap_turu= BNK_GET_HESAP_TURU_ENUM( combobox_etkilenecek_hesap_turu->currentText() );
    M_FIS_SATIRI->etkilenecek_hesap_id  = lineedit_etk_hesap_id->text().toInt();
    M_FIS_SATIRI->karsi_hesap_turu      = BNK_GET_HESAP_TURU_ENUM( combobox_kari_hesap_turu->currentText() );
    M_FIS_SATIRI->karsi_hesap_id        = lineedit_muh_hesap_id->text().toInt();
    M_FIS_SATIRI->doviz_kuru            = lineedit_doviz_kuru->text().toDouble();
    M_FIS_SATIRI->parite                = lineedit_parite->text().toDouble();

    int fis_satir_id = m_ui->tablewidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    int new_satir_id = BNK_HAREKET_FIS_SATIRINI_GUNCELLE( M_FIS, M_FIS_SATIRI, fis_satir_id );


     if( new_satir_id NE 0 )
     {
         QTableWidgetItem * new_item;
         new_item = new QTableWidgetItem( QVariant ( new_satir_id ).toString());
         m_ui->tablewidget_fis_satirlari->setItem(row_number,ROW_ID_COLUMN,new_item);
     }

}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int BNK_HAREKET_FISI::CHECK_DELETE_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::DELETE_LINE
***************************************************************************************/

void BNK_HAREKET_FISI::DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id  );
    Q_UNUSED ( row_number );

   int fis_satir_id = m_ui->tablewidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();

   BNK_HAREKET_FIS_SATIRI_SIL( M_FIS, fis_satir_id   );
}

/**************************************************************************************
                   BNK_HAREKET_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int BNK_HAREKET_FISI::LOCK_FIS_RECORD ( int record_id )
{
    Q_UNUSED ( record_id  );
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HAREKET_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void BNK_HAREKET_FISI::UNLOCK_FIS_RECORD(int record_id)
{
    Q_UNUSED ( record_id );
}

/**************************************************************************************
                   BNK_HAREKET_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void BNK_HAREKET_FISI::LINE_SEARCH_EDIT_CLICKED(int p_row_number, QWidget * p_widget, QLineEdit * p_lineedit)
{
    Q_UNUSED ( p_widget     );
    Q_UNUSED ( p_lineedit   );

    QSearchEdit * searchedit_krs_hesap_kodu         = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(p_row_number,KRS_HESAP_MUH_KODU_COLUMN);
    QLineEdit   * lineedit_muh_hesap_ismi           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(p_row_number,MUH_HESAP_ISMI_COLUMN);
    QComboBox   * etkilenecek_hesap                 = ( QComboBox  *  ) m_ui->tablewidget_fis_satirlari->cellWidget(p_row_number,ETKILECEK_HESAP_COLUMN );

    QLineEdit   * lineedit_etk_hesap_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(p_row_number,ETK_HESAP_ID_COLUMN);
    QLineEdit   * lineedit_krs_hesap_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(p_row_number, KRS_HESAP_ID_COLUMN );

    QSearchEdit * searchedit_etk_muh_hesap_kodu     = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(p_row_number,ETK_HESAP_MUH_KODU_COLUMN);
    QLineEdit   * lineedit_cari_para_birim          = ( QLineEdit  *  ) m_ui->tablewidget_fis_satirlari->cellWidget(p_row_number, KRS_HESAP_PARA_BIRIM_ID );



    if( p_widget EQ searchedit_krs_hesap_kodu )
    {

        switch ( karsi_hesap_turu )
        {

            case ENUM_BNK_CARI_HESAP: {
                    int cari_hesap_id = OPEN_CARI_KART_ARAMA( "", SAHIS_ARTI_FIRMA, this, true, E9_PROGRAMI, CARI_MODULU  );
                    if(cari_hesap_id NE -1) {
                        lineedit_krs_hesap_id->setText( QVariant ( cari_hesap_id ).toString() );
                    }
                    int para_birim_id = CARI_GET_PARA_BIRIM_ID( cari_hesap_id );

                    lineedit_cari_para_birim->setText( DVZ_GET_DOVIZ_KODU( para_birim_id ) );
                    searchedit_krs_hesap_kodu->SET_TEXT( CARI_FIND_HESAP_KODU( cari_hesap_id ) );

                    lineedit_krs_hesap_id->setText( QVariant( cari_hesap_id ).toString() );
                    lineedit_muh_hesap_ismi->setText( CARI_FIND_HESAP_ISMI( cari_hesap_id ));

                    break;
                 }
            case ENUM_BNK_KREDI_KARTI_HESABI: {
                    int kredi_karti_id = OPEN_BNK_KREDI_KARTI_SEC ( m_bnk_hesap_no_id,this);

                    if ( kredi_karti_id NE 0 ) {
                        lineedit_krs_hesap_id->setText( QVariant ( kredi_karti_id ).toString() );
                        searchedit_krs_hesap_kodu->SET_TEXT( BNK_GET_KREDI_KARTI_NUMARASI( kredi_karti_id ) );
                        lineedit_krs_hesap_id->setText( QVariant( kredi_karti_id ).toString() );

                        lineedit_muh_hesap_ismi->clear();
                        lineedit_muh_hesap_ismi->setEnabled( false );
                    }
                    break;
                }
            case ENUM_BNK_MUHASEBE_HESABI :
            case ENUM_BNK_KASA_HESABI     :
                {
                    int muh_hesap_id = OPEN_MUH_HESAP_ARAMA( searchedit_krs_hesap_kodu->GET_TEXT(),this,1);
                    if (muh_hesap_id NE -1) {
                        lineedit_krs_hesap_id->setText( QVariant( muh_hesap_id ).toString() );
                        searchedit_krs_hesap_kodu->SET_TEXT( MUH_GET_HESAP_KODU( muh_hesap_id ) );
                    }
                    break;
                }
            case ENUM_BNK_POS_HESABI: {

                    int pos_id = OPEN_BNK_POS_SEC(m_bnk_hesap_no_id,this);

                    if ( pos_id NE -1 ) {
                        QString pos_numarasi = BNK_GET_POS_NUMARASI(pos_id);

                        if ( pos_numarasi NE NULL ) {
                            lineedit_krs_hesap_id->setText( QVariant( pos_id ).toString() );
                            searchedit_krs_hesap_kodu->SET_TEXT( pos_numarasi );
                            lineedit_muh_hesap_ismi->clear();
                            lineedit_muh_hesap_ismi->setEnabled( false );
                        }
                    }
                    break;
                }
            case ENUM_BNK_POS_KOMISYON_HESABI: {
                        int pos_id = OPEN_BNK_POS_SEC(m_bnk_hesap_no_id,this);

                        if ( pos_id NE -1 ) {
                            QString pos_numarasi = BNK_GET_POS_NUMARASI(pos_id);

                            if ( pos_numarasi NE NULL ) {
                                lineedit_krs_hesap_id->setText( QVariant( pos_id ).toString() );
                                searchedit_krs_hesap_kodu->SET_TEXT( pos_numarasi );
                            }
                        }
                        break;
                 }
            case ENUM_BNK_BANKA_HESABI: {
                        int banka_id = OPEN_BNK_HESAPLARI_ARAMA( p_widget );

                        if ( banka_id NE -1 ) {
                            QString banka_ad = BNK_GET_BANKA_ADI_SUBE_KODU_ADI_HESABI_NO( banka_id );

                            if ( banka_ad NE NULL ) {
                                lineedit_krs_hesap_id->setText( QVariant( banka_id ).toString() );
                                searchedit_krs_hesap_kodu->SET_TEXT( BNK_GET_BANKA_KODU( banka_id ) );
                                lineedit_muh_hesap_ismi->setText( banka_ad );
                            }
                        }
                        break;
                 }
        default: break;
        }

    }

    if( p_widget EQ searchedit_etk_muh_hesap_kodu)
    {

        switch ( etkilenecek_hesap_turu ) {
        case ENUM_BNK_BANKA_HESABI: {
                break;
            }
        case ENUM_BNK_KREDI_KARTI_HESABI: {

                int kredi_karti_id = OPEN_BNK_KREDI_KARTI_SEC ( m_bnk_hesap_no_id,this);

                if ( kredi_karti_id NE -1 ) {
                    lineedit_etk_hesap_id->setText( QVariant ( kredi_karti_id ).toString() );
                    searchedit_etk_muh_hesap_kodu->SET_TEXT( BNK_GET_KREDI_KARTI_NUMARASI( kredi_karti_id ) );
                }

                break;
            }
        case ENUM_BNK_POS_HESABI: {
                int pos_id = OPEN_BNK_POS_SEC(m_bnk_hesap_no_id,this);

                if ( pos_id NE -1 ) {
                    QString pos_numarasi = BNK_GET_POS_NUMARASI(pos_id);

                    if ( pos_numarasi NE NULL ) {
                        lineedit_etk_hesap_id->setText( QVariant( pos_id ).toString() );
                        searchedit_etk_muh_hesap_kodu->SET_TEXT( pos_numarasi );
                    }
                }

                break;
            }
        }

       if( etkilenecek_hesap->currentText() EQ tr( "BANKA" ) ){
            lineedit_etk_hesap_id->setText( QVariant( M_FIS->hesap_no_id ).toString() );
            searchedit_etk_muh_hesap_kodu->SET_TEXT(  QVariant( M_FIS->hesap_no_id ).toString() );
        }
    }


}

/**************************************************************************************
                   BNK_HAREKET_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void BNK_HAREKET_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_record_id )
{
    if( p_button EQ m_ui->buton_yazdir ){
        OPEN_REPORT_SHOWER ( GET_BNK_FISI_YAZDIR ( p_record_id, ENUM_BANKA_HAREKET_FISI ), nativeParentWidget() );
    }
    else if ( p_button EQ m_ui->pushButton_urettigi_fisler ) {
        E9_URETILEN_FIS_SEC( M_FIS->program_id, M_FIS->modul_id, M_FIS->def_fis_id, this );
    }
}

/**************************************************************************************
                   BNK_HAREKET_FISI::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int BNK_HAREKET_FISI::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    if ( p_button EQ m_ui->push_button_detaylar ) {

        if ( m_fis_turu EQ ENUM_BANKA_PARA_YAT_CEKME_FISI ) {
            OPEN_E9_ENT_DETAYLARI_BATCH ( M_ENT_DETAYLARI, true,this );

            if ( M_FIS->muh_kasa_hesap_id NE M_ENT_DETAYLARI->muh_kasa_hesap_id ) {
                return ADAK_RECORD_CHANGED;
            }
        }
        else {
           OPEN_E9_ENT_DETAYLARI_BATCH ( M_ENT_DETAYLARI, false,this );
        }

    }
    return ADAK_RECORD_UNCHANGED;
}





















