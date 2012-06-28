#include "adak_utils.h"
#include "print.h"
#include "e9_struct.h"
#include "bnk_makro_fisi_class.h"
#include "bnk_makro_fisi_open.h"
#include "ui_bnk_makro_fisi.h"
#include "muh_enum.h"
#include "dvz_enum.h"
#include "adak_std_utils.h"
#include "bnk_gui_utils.h"
#include "banka_enum.h"
#include "dvz_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "cari_console_utils.h"
#include "sube_enum.h"
#include "e9_yetki.h"
#include "e9_log.h"
#include "bnk_defter_fis_utils.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "cari_kart_arama_open.h"
#include "bnk_hesaplari_arama_open.h"
#include "bnk_kredi_karti_secimi_open.h"
#include "bnk_makro_fisi_arama_open.h"
#include "bnk_pos_secimi_open.h"
#include "e9_enum.h"
#include "e9_ent_detaylari_batch_open.h"
#include "bnk_fisi_yazdir_open.h"
#include "bnk_console_utils.h"
#include "banka_enum.h"
#include "dvz_struct.h"
#include "cari_enum.h"
#include "sube_console_utils.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "bnk_defter_fis_utils.h"
#include "cari_console_utils.h"

extern ADAK_SQL   *            DB;

#define ROW_ID_COLUMN           0
#define FIS_ID_COLUMN           1
#define ORDER_COLUMN            2
#define MAKRO_ID_COLUMN         3
#define MAKRO_TURU_COLUMN       4
#define HESAP_TURU_COLUMN       5
#define HESAP_TURU_LABEL_COLUMN 6
#define HESAP_COLUMN            7
#define HESAP_ISMI_COLUMN       8
#define CARI_PARA_BIRIMI_COLUMN 9
#define ACIKLAMA_COLUMN         10
#define MAKRO_TUTAR_COLUMN      11
#define OTO_ODEME_TUTAR_COLUMN  12
#define KUR_COLUMN              13
#define PARITE_COLUMN           14
#define SATIR_TUTARI_COLUMN     15

/*****************************************************************************************************
                                        OPEN_BNK_MAKRO_FISI
*****************************************************************************************************/

void  OPEN_BNK_MAKRO_FISI ( int fis_id, int fis_kaynagi, QWidget * parent )
{
    BNK_MAKRO_FISI * F = new BNK_MAKRO_FISI(fis_id,fis_kaynagi,parent);
    F->EXEC( FULL_SCREEN );
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::BNK_MAKRO_FISI
*****************************************************************************************************/

BNK_MAKRO_FISI::BNK_MAKRO_FISI ( int record_id, int fis_kaynagi , QWidget * parent ) : FIS_KERNEL ( parent ),
m_ui ( new Ui::BNK_MAKRO_FISI )
{
    m_ui->setupUi ( this );
    m_record_id     = record_id;
    m_fis_kaynagi   = fis_kaynagi;

    M_ENT_DETAYLARI     = new E9_ENT_DETAYLARI_STRUCT;

    START_FIS_KERNEL( this, DB );
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::SETUP_FORM
*****************************************************************************************************/

void BNK_MAKRO_FISI::SETUP_FORM ()
{
    SET_HELP_PAGE  ( "banka-islemleri_makro-fisi" );
    SET_PAGE_TITLE ( tr ( "BNK - MAKRO FİŞİ" ) );

    REGISTER_BUTTONS_WIDGET  ( m_ui->kernel_buttons_widget );

    m_ui->lineedit_fis_no->setText("*");

    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_fis_no );
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_sirket );

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );
    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    if( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ HAYIR ){
        m_ui->pushButton_detaylar->setHidden(true);

    }
    if( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ){
        m_ui->comboBox_fis_filtresi->setHidden( true );
        m_ui->label_fis_filtresi->setHidden(true);

        m_ui->comboBox_sirket->setVisible( false );
        m_ui->label_sirket->setVisible( false );
    }

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH ( 512 );
    m_ui->lineedit_fis_no->setMaxLength( 20 );

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_fis_no->setValidator(validator);

    m_num_of_columns        = 16;

    m_log_kaydi_eklensin_mi = false;
    m_kayit_eklendi         = false;
    m_kayit_silindi         = false;
    m_bnk_hesap_no_id       = 0;

    m_temel_para_birim_id   = DVZ_GET_TEMEL_PARA_BIRIMI_ID();
    m_temel_para_birim_kodu = DVZ_GET_TEMEL_PARA_BIRIMI_KODU();

    GET_MAKRO_TURLERI();

    REGISTER_TABLE_WIDGET             ( m_ui->tablewidget_fis_satirlari, m_num_of_columns );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN       ( 4 );
    SET_FIS_ORDER_DATABASE            ( "bnk_fis_satirlari", "order_number", "fis_satiri_id" );

    SET_TABLE_ROW_WIDGETS  ( MAKRO_ID_COLUMN         , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( MAKRO_TURU_COLUMN       , WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS  ( MAKRO_TUTAR_COLUMN      , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS  ( OTO_ODEME_TUTAR_COLUMN  , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS  ( ACIKLAMA_COLUMN         , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( HESAP_COLUMN            , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS  ( HESAP_ISMI_COLUMN       , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( CARI_PARA_BIRIMI_COLUMN , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( KUR_COLUMN              , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS  ( PARITE_COLUMN           , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS  ( SATIR_TUTARI_COLUMN     , WIDGET_COMMA_EDIT  );

    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI( m_ui->combo_box_banka_hesap_kodu );

    REGISTER_SAVER_BUTTON           ( m_ui->buton_yazdir );

    SET_FIRST_FOCUS_WIDGET          ( m_ui->lineedit_fis_no );

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels(QStringList()<<"row id"<<"fis id"<<"column order"
                                                                            <<tr("makro_id")<<tr("Makro Türü")
                                                                            <<tr("")<<tr("Hesap Türü Adı")<<tr("")<<tr("")
                                                                            <<tr("")
                                                                            <<tr("Açıklama")<<tr("Makro Tutarı")
                                                                            <<tr("Otomatik \nÖdeme Tutarı")
                                                                            <<tr("Kur") << tr("Parite")
                                                                            <<tr("Tutar") <<""<<"" );

    m_ui->tablewidget_fis_satirlari->setColumnWidth ( MAKRO_TURU_COLUMN      , 250 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( HESAP_TURU_LABEL_COLUMN, 130 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( HESAP_COLUMN           , 120 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( HESAP_ISMI_COLUMN      , 150 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_PARA_BIRIMI_COLUMN, 40  );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( ACIKLAMA_COLUMN        , 211 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( MAKRO_TUTAR_COLUMN     , 110 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( KUR_COLUMN             , 80  );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( PARITE_COLUMN          , 80  );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( SATIR_TUTARI_COLUMN    , 80  );

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->horizontalHeaderItem(7);

    QFont item_font = item->font();

    item_font.setPointSize(item_font.pointSize() -2);
    item->setFont(item_font);

    REGISTER_CHANGER_BUTTON( m_ui->pushButton_detaylar );

    SET_SETTING_NAME ( "BANKA_MAKRO_FISI" );

    m_ui->tablewidget_fis_satirlari->hideColumn ( HESAP_TURU_COLUMN );

    m_ui->lineedit_doviz_cinsi->setDisabled     ( true );

    m_ui->tablewidget_fis_satirlari->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_fis_satirlari->setFocus         ( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET      ();

    AUTOSTART_FIS_KERNEL    ( m_record_id );

}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CLEAR_FORM_MEMBERS
*****************************************************************************************************/

void BNK_MAKRO_FISI::CLEAR_FORM_MEMBERS()
{
    m_muh_fis_id    = 0;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::NEW_FIS_RECORD
*****************************************************************************************************/

void BNK_MAKRO_FISI::NEW_FIS_RECORD()
{
    m_ui->lineedit_fis_no->setText("*");
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::GET_FIS_RECORD
*****************************************************************************************************/

int BNK_MAKRO_FISI::GET_FIS_RECORD ( int fis_id )
{    
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "bnk_makro_fisler","fis_no, fis_tarihi, hesap_no_id, aciklama, "
                               "toplam_makro_tutari, muh_fis_id","fis_id = :fis_id" );

    sql_query.SET_VALUE ( ":fis_id"   , fis_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->lineedit_fis_no->setText              ( QVariant ( sql_query.VALUE(0)).toString() );
    m_ui->adakDate_fis_tarihi->SET_DATE(  sql_query.VALUE(1).toString() );
    m_bnk_hesap_no_id =                           sql_query.VALUE(2).toInt();
    m_ui->limitedTextEdit_aciklama->setText     ( sql_query.VALUE(3).toString() );
    m_ui->commaedit_toplam_tutar->SET_DOUBLE    ( sql_query.VALUE(4).toDouble() );

    m_muh_fis_id            = sql_query.VALUE( 5 ).toInt();

    int row = BNK_HESAPLARI_MODEL_INDEX_BUL ( m_bnk_hesap_no_id, m_ui->combo_box_banka_hesap_kodu );

    if ( row > -1 ) {

        m_ui->combo_box_banka_hesap_kodu->setCurrentIndex ( row );

        m_bnk_hesap_no_id       = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->combo_box_banka_hesap_kodu, row, 0 ).toInt();
        m_banka_hesabi_doviz_id = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 1 ).toInt();

        m_ui->lineedit_doviz_cinsi->setText ( DVZ_GET_DOVIZ_KODU ( m_banka_hesabi_doviz_id ) );

        QString banka_bilgisi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 3 ).toString() + " / " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 4 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 5 ).toString() + " " +
                                BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 6 ).toString();

        m_ui->lineEdit_banka_bilgisi->setText ( banka_bilgisi );

    }

    sql_query.PREPARE_SELECT ( "bnk_makro_fis_satirlari","fis_satiri_id, makro_id, aciklama, makro_tutari, "
                               "hesap_turu, hesap_id, otomatik_odeme_tutari, doviz_kuru, parite, satir_tutari ",
                               "fis_id = :fis_id");

    sql_query.SET_VALUE(":fis_id"  , fis_id);

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_OK;
    }

    int  current_row = -1;

    SQL_QUERY query(DB);

    QTableWidgetItem *  new_item;

    while ( sql_query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE ();

        QComboBox   * combobox_makro_turu             = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MAKRO_TURU_COLUMN      );
        QLineEdit   * lineedit_aciklama               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, ACIKLAMA_COLUMN        );
        QCommaEdit  * commaedit_tutar                 = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MAKRO_TUTAR_COLUMN     );
        QCommaEdit  * commaedit_otomatik_odeme_tutari = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, OTO_ODEME_TUTAR_COLUMN    );
        QSearchEdit * searchedit_hesap                = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, HESAP_COLUMN           );
        QLineEdit   * lineedit_hesap_ismi             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, HESAP_ISMI_COLUMN      );
        QLineEdit   * lineedit_makro_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MAKRO_ID_COLUMN        );
        QCommaEdit  * commaedit_doviz_kuru            = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, KUR_COLUMN             );
        QCommaEdit  * commaedit_parite                = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, PARITE_COLUMN          );
        QCommaEdit  * commaedit_satir_tutari          = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, SATIR_TUTARI_COLUMN    );
        QLineEdit   * lineedit_cari_para_birimi_kodu  = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, CARI_PARA_BIRIMI_COLUMN);

        commaedit_satir_tutari->SET_DOUBLE( sql_query.VALUE( "satir_tutari" ).toDouble() );

        SQL_QUERY fark_satiri_query ( DB );
        fark_satiri_query.PREPARE_SELECT ( "bnk_makro_satirlari","makro_satiri_id","makro_id = :makro_id AND fark_satiri_mi = :fark_satiri_mi");

        fark_satiri_query.SET_VALUE(":makro_id", sql_query.VALUE(1).toString() );
        fark_satiri_query.SET_VALUE(":fark_satiri_mi", 1 );

        if ( fark_satiri_query.SELECT() EQ 0 ) {
            commaedit_otomatik_odeme_tutari->clear();
            commaedit_otomatik_odeme_tutari->setReadOnly ( true );
        }
        else {
            commaedit_otomatik_odeme_tutari->setReadOnly ( false );
        }

        SQL_QUERY makro_turu_query ( DB );
        lineedit_makro_id->setText ( sql_query.VALUE(1).toString() );

        makro_turu_query.PREPARE_SELECT ( "bnk_makrolar","makro_adi","makro_id = :makro_id" );
        makro_turu_query.SET_VALUE      ( ":makro_id", sql_query.VALUE(1).toString() );

        if ( makro_turu_query.SELECT() > 0 ) {
            makro_turu_query.NEXT();
            combobox_makro_turu->setCurrentIndex ( combobox_makro_turu->findText ( makro_turu_query.VALUE(0).toString() ) );
        }
        lineedit_aciklama->setText ( sql_query.VALUE(2).toString());

        commaedit_tutar->SET_DOUBLE         ( sql_query.VALUE(3).toDouble() );

        int hesap_turu     = sql_query.VALUE( 4 ).toInt();
        int hesap_id       = sql_query.VALUE( 5 ).toInt();
        commaedit_otomatik_odeme_tutari->SET_DOUBLE ( sql_query.VALUE("otomatik_odeme_tutari").toDouble() );

        commaedit_doviz_kuru->SET_DOUBLE(sql_query.VALUE( 7 ).toDouble());
        commaedit_parite->SET_DOUBLE(sql_query.VALUE( 8 ).toDouble());

        if ( m_banka_hesabi_doviz_id NE m_temel_para_birim_id ) {
            commaedit_doviz_kuru->setEnabled( true );
        }
        else {
            commaedit_doviz_kuru->setEnabled( false );
        }

        new_item = new QTableWidgetItem ( sql_query.VALUE( 0 ).toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( QVariant ( fis_id ).toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row, FIS_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( QVariant ( hesap_turu ).toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row, HESAP_TURU_COLUMN, new_item );

        new_item = new QTableWidgetItem ( QVariant("").toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row, HESAP_TURU_LABEL_COLUMN, new_item );

        switch ( hesap_turu ) {
        case ENUM_BNK_CARI_HESAP        : {

                new_item->setText ( tr("Cari Hesap Kodu") );

                QString cari_hesap_kodu, cari_hesap_ismi;
                int     cari_para_birim_id;

                int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( hesap_id, &cari_hesap_kodu, &cari_hesap_ismi, &cari_para_birim_id );

                if ( result > 0 ) {
                    searchedit_hesap->SET_TEXT   ( cari_hesap_kodu );
                    lineedit_hesap_ismi->setText ( cari_hesap_ismi );
                    lineedit_cari_para_birimi_kodu->setText(DVZ_GET_DOVIZ_KODU( cari_para_birim_id ));
                }

                if( cari_para_birim_id NE m_temel_para_birim_id AND m_banka_hesabi_doviz_id NE m_temel_para_birim_id ) {
                    commaedit_parite->setEnabled( true );
                }
                else {
                    commaedit_parite->setEnabled( false );
                }

                break;
            }
            case ENUM_BNK_POS_HESABI        :

                new_item->setText("Pos Numarası");

                query.PREPARE_SELECT("bnk_hesabin_poslari","pos_numarasi","pos_id = :pos_id");
                query.SET_VALUE(":pos_id" , hesap_id );

                if ( query.SELECT() > 0 ) {
                    query.NEXT();
                    searchedit_hesap->SET_TEXT ( query.VALUE(0).toString() );
                }
                break;

            case ENUM_BNK_KREDI_KARTI_HESABI:

                new_item->setText("Kredi Kartı Numarası");

                query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari","kredi_karti_numarasi",
                                         "kredi_karti_id = :kredi_karti_id");

                query.SET_VALUE(":kredi_karti_id" , hesap_id );

                if ( query.SELECT() > 0 ) {
                    query.NEXT();
                    searchedit_hesap->SET_TEXT ( query.VALUE(0).toString() );
                }
                break;

            default :
                    searchedit_hesap->setEnabled( false );
                break;
        }
    }
    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::SET_LINE_DEFAULTS
*****************************************************************************************************/

void BNK_MAKRO_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QComboBox  * combobox_makro_turu    = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, MAKRO_TURU_COLUMN        );
    QLineEdit  * lineedit_hesap_ismi    = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, HESAP_ISMI_COLUMN        );
    QLineEdit  * lineedit_para_birimi   = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, CARI_PARA_BIRIMI_COLUMN  );
    QCommaEdit * commaedit_kur          = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, KUR_COLUMN               );
    QCommaEdit * commaedit_tutar        = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, MAKRO_TUTAR_COLUMN       );
    QCommaEdit * commaedit_parite       = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, PARITE_COLUMN            );
    QCommaEdit * commaedit_satir_tutari = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, SATIR_TUTARI_COLUMN      );
    QLineEdit  * lineedit_aciklama      = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, ACIKLAMA_COLUMN          );
    QLineEdit  * lineedit_makro_id      = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, MAKRO_ID_COLUMN          );

    combobox_makro_turu->addItems ( m_makro_turleri );
    combobox_makro_turu->setCurrentIndex ( -1 );

    QTableWidgetItem * hesap_turu_label_item = new QTableWidgetItem();

    m_ui->tablewidget_fis_satirlari->setItem ( row_number, HESAP_TURU_LABEL_COLUMN, hesap_turu_label_item );
    hesap_turu_label_item->setText ( "" );

    QTableWidgetItem * hesap_turu_item = new QTableWidgetItem( QVariant(-1).toString());
    m_ui->tablewidget_fis_satirlari->setItem ( row_number, HESAP_TURU_COLUMN, hesap_turu_item );

    commaedit_satir_tutari->setReadOnly( true );

    lineedit_hesap_ismi->setReadOnly ( true );
    lineedit_hesap_ismi->setMaxLength( 100 );

    lineedit_para_birimi->setReadOnly(true);
    lineedit_para_birimi->setMaxLength( 5 );

    commaedit_kur->setEnabled( false );
    commaedit_kur->SET_PRECISION(4);
    commaedit_tutar->SET_PRECISION(2);

    commaedit_parite->setEnabled( false );
    commaedit_parite->SET_PRECISION(6);

    lineedit_aciklama->setMaxLength( 128 );
    lineedit_makro_id->setMaxLength( 10 );
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_FIS_FORM_VAR
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( m_fis_kaynagi EQ CARI_MODULU ) {
        MSG_WARNING( tr("Fiş Cari Modülü tarafından üretilmiş.Fişte değişiklik yapamazsınız!.."), NULL );
        return ADAK_FAIL;
    }
    if ( object EQ m_ui->combo_box_banka_hesap_kodu  ) {

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
        m_banka_hesabi_doviz_id   = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 1 ).toInt();

        m_ui->lineedit_doviz_cinsi->setText ( DVZ_GET_DOVIZ_KODU ( m_banka_hesabi_doviz_id ) );

        QString banka_bilgisi =BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->combo_box_banka_hesap_kodu, row, 3 ).toString() + " / " +
                               BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 4 ).toString() + " " +
                               BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->combo_box_banka_hesap_kodu, row, 5 ).toString() + " " +
                               BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL( m_ui->combo_box_banka_hesap_kodu, row, 6 ).toString();

        m_ui->lineEdit_banka_bilgisi->setText ( banka_bilgisi );
        m_ui->combo_box_banka_hesap_kodu->setCurrentIndex(row);

        if( m_banka_hesabi_doviz_id NE m_temel_para_birim_id ) {
            SATIR_DUZENLE( true, false );
        }

    }

    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_FIS_FORM_EMPTY
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineedit_fis_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Yeni Banka Fişi kaydı için (*) girmelisiniz"), m_ui->lineedit_fis_no );
        return ADAK_FAIL;
    }

    if ( m_ui->combo_box_banka_hesap_kodu->currentText().isEmpty() EQ true ) {
        MSG_WARNING( tr("Banka Hesap Kodunu boş bırakamazsınız!.."), m_ui->combo_box_banka_hesap_kodu );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_FIS_RECORD_ADD
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_FIS_RECORD_ADD()
{
    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) NE "*") {

        SQL_QUERY sql_query ( DB );

        sql_query.PREPARE_SELECT("bnk_makro_fisler","fis_id","fis_tarihi = :fis_tarihi "
                                 "AND fis_no = :fis_no ");

        sql_query.SET_VALUE ( ":fis_tarihi", m_ui->adakDate_fis_tarihi->DATE());
        sql_query.SET_VALUE ( ":fis_no"    , m_ui->lineedit_fis_no->text().toInt() );

        if ( sql_query.SELECT() > 0 ) {
            MSG_WARNING( tr("Aynı fiş numarası ile kayıtlı fiş bulundu. Fiş numarasını değiştiriniz."), m_ui->lineedit_fis_no );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::ADD_FIS_RECORD
*****************************************************************************************************/

int BNK_MAKRO_FISI::ADD_FIS_RECORD()
{

    MAKRO_FIS_STRUCT * M_FIS    = new MAKRO_FIS_STRUCT;

    CLEAR_MAKRO_FIS( M_FIS );

    M_FIS->fis_tarihi   = m_ui->adakDate_fis_tarihi->DATE();
    if ( m_ui->lineedit_fis_no->text() EQ "*"){
        M_FIS->fis_no       = 0;
    }
    else {
        M_FIS->fis_no       = m_ui->lineedit_fis_no->text().toInt();
    }
    M_FIS->def_fis_id       = 0;
    M_FIS->program_id       = E9_PROGRAMI;
    M_FIS->modul_id         = BANKA_MODULU;
    M_FIS->base_fis_id      = 0;
    M_FIS->hesap_no_id      = m_bnk_hesap_no_id;
    M_FIS->para_birim_id    = m_banka_hesabi_doviz_id;
    M_FIS->muh_fis_id       = 0;
    M_FIS->aciklama         = m_ui->limitedTextEdit_aciklama->toPlainText();

    int fis_id  = MAKRO_ADD_FIS( M_FIS, 0 );

    m_log_kaydi_eklensin_mi = true;
    m_kayit_eklendi         = true;

    m_log_detaylari =  QObject::tr("Log Türü - Banka Fişleri , Banka Makro Fişi") ;

    m_log_detaylari.append(","  +  m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") + "," +
                         QObject::tr("Fiş No : ") + M_FIS->fis_no);



    delete M_FIS;

    return fis_id;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_FIS_RECORD_UPDATE
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_FIS_RECORD_UPDATE ( int fis_id )
{

    if ( m_fis_kaynagi EQ CARI_MODULU ) {
        MSG_WARNING( tr("Fiş Cari Modülü tarafından üretilmiş.Fişte değişiklik yapamazsınız!.."), NULL );
        return ADAK_FAIL;
    }

    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) NE "*" ) {

       int fis_no = m_ui->lineedit_fis_no->text().toInt();

       SQL_QUERY select_query ( DB );
       select_query.PREPARE_SELECT("bnk_makro_fisler","fis_id","fis_no = :fis_no AND fis_tarihi = :fis_tarihi");

       select_query.SET_VALUE(":fis_no"      ,fis_no );
       select_query.SET_VALUE(":fis_tarihi"  ,m_ui->adakDate_fis_tarihi->DATE());

       if ( select_query.SELECT() > 0 ) {
           select_query.NEXT();
           if ( select_query.VALUE(0) NE fis_id ) {
               MSG_WARNING( tr("Aynı Fiş No ile kayıtlı başka bir fiş bulundu. Farklı bir fiş numarası giriniz"), NULL );
               return ADAK_FAIL;
           }
       }
    }

    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::UPDATE_FIS_RECORD
*****************************************************************************************************/

void BNK_MAKRO_FISI::UPDATE_FIS_RECORD ( int fis_id )
{

    SQL_QUERY query ( DB );

    int fis_no = 0;

    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) EQ "*" ) {
        fis_no = BNK_SIRADAKI_MAKRO_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE());
    }
    else {
        fis_no = m_ui->lineedit_fis_no->text().toInt();
    }

    query.PREPARE_SELECT( "bnk_makro_fis_satirlari","makro_id","fis_id = :fis_id" );
    query.SET_VALUE     ( ":fis_id", fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    SQL_QUERY s_query ( DB );

    int def_fis_id  = GET_BNK_DEFTER_FIS_ID( E9_PROGRAMI, BANKA_MODULU, fis_id );

    while ( query.NEXT() EQ true ) {
        int makro_id    = query.VALUE( 0 ).toInt();

        s_query.PREPARE_SELECT( "bnk_makro_satirlari","hesap_turu","makro_id = :makro_id " );
        s_query.SET_VALUE     ( ":makro_id", makro_id );

        if ( s_query.SELECT() EQ 0 ) {
            return;
        }
        int hesap_turu  = 0;
        while ( s_query.NEXT() EQ true ) {

            hesap_turu  = s_query.VALUE( 0 ).toInt();

            switch ( hesap_turu ) {

                case ENUM_BNK_BANKA_HESABI:
                case ENUM_BNK_POS_HESABI:
                case ENUM_BNK_KREDI_KARTI_HESABI: {

                        BNK_DEFTER_FIS_STRUCT        * DEF_FIS        = new BNK_DEFTER_FIS_STRUCT;

                        BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, def_fis_id );

                        DEF_FIS->aciklama           = m_ui->limitedTextEdit_aciklama->toPlainText();
                        DEF_FIS->hesap_no_id        = m_bnk_hesap_no_id;
                        DEF_FIS->fis_tarihi         = m_ui->adakDate_fis_tarihi->DATE() ;

                        BNK_DEFTER_FISI_GUNCELLE( DEF_FIS, def_fis_id );

                        delete DEF_FIS;

                        break;
                    }
                case ENUM_BNK_MUHASEBE_HESABI:
                case ENUM_BNK_KASA_HESABI: {

                        MUH_FIS_STRUCT  * MUH_FIS = new MUH_FIS_STRUCT;

                        MUH_CLEAR_FIS_STRUCT   ( MUH_FIS );

                        int muh_fis_id  = MUH_GET_ENT_FIS_ID( E9_PROGRAMI, BANKA_MODULU, fis_id );

                        MUH_FIS_BILGILERINI_OKU ( MUH_FIS, muh_fis_id );

                        MUH_FIS->aciklama           = m_ui->limitedTextEdit_aciklama->toPlainText();
                        MUH_FIS->fis_tarihi         = m_ui->adakDate_fis_tarihi->DATE();

                        MUH_FISI_GUNCELLE( MUH_FIS, muh_fis_id );

                        delete MUH_FIS;

                    break;
                 }
                case ENUM_BNK_CARI_HESAP: {

                        CARI_FIS_STRUCT * CARI_FIS    = new CARI_FIS_STRUCT;
                        CARI_CLEAR_FIS_STRUCT  ( CARI_FIS );

                        int cari_fis_id = CARI_GET_ENT_FIS_ID( E9_PROGRAMI, BANKA_MODULU, def_fis_id );

                        CARI_FIS_BILGILERINI_OKU( CARI_FIS, cari_fis_id );

                        CARI_FIS->aciklama         = QObject::tr("Fiş BANKA MODÜLÜ ( MAKRO FİŞİ ) tarafından üretilmiştir.Fiş No : %1\n").arg(fis_no) +  m_ui->limitedTextEdit_aciklama->toPlainText();
                        CARI_FIS->fis_tarihi       = m_ui->adakDate_fis_tarihi->DATE();

                        CARI_FISI_GUNCELLE( CARI_FIS, cari_fis_id );

                        delete CARI_FIS;

                    break;
                }
                default: break;

            }
        }

    }

    query.PREPARE_UPDATE("bnk_makro_fisler","fis_id","fis_no ,fis_tarihi,"
                                "hesap_no_id , aciklama",
                                "fis_id = :fis_id");

    query.SET_VALUE ( ":fis_no"           , fis_no                                         );
    query.SET_VALUE ( ":fis_tarihi"       , m_ui->adakDate_fis_tarihi->DATE()             );
    query.SET_VALUE ( ":hesap_no_id"      , m_bnk_hesap_no_id                              );
    query.SET_VALUE ( ":aciklama"         , m_ui->limitedTextEdit_aciklama->toPlainText()  );
    query.SET_VALUE ( ":fis_id"           , fis_id                                         );

    query.UPDATE();

    query.PREPARE_SELECT ( "bnk_makro_fisler","fis_tarihi, hesap_no_id, aciklama, toplam_makro_tutari","fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , fis_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    QDate       fis_tarihi          = QDate::fromString ( query.VALUE(0).toString(),"yyyy.MM.dd");
    double      makro_tutari            = query.VALUE(3).toDouble();


    m_log_kaydi_eklensin_mi = true;

    m_log_detaylari =  QObject::tr("Log Türü - Banka Fişleri , Banka Makro Fişi") ;

    m_log_detaylari.append(","  + fis_tarihi.toString("dd MMMM yyyy dddd") + "," +
                         QObject::tr("Fiş No : ") + QVariant(fis_no).toString() +
                         QObject::tr("Makro Tutarı :") +
                         VIRGUL_EKLE ( QVariant ( makro_tutari ).toString(), 2 ) );

}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_FIS_RECORD_DELETE
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_FIS_RECORD_DELETE(int fis_id)
{
    Q_UNUSED ( fis_id );

    if ( m_fis_kaynagi EQ CARI_MODULU ) {
        MSG_WARNING( tr("Fiş Cari Modülü tarafından üretilmiş.Fişi silemezsiniz!.."), NULL );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::DELETE_FIS_RECORD
*****************************************************************************************************/

void BNK_MAKRO_FISI::DELETE_FIS_RECORD ( int fis_id )
{
    SQL_QUERY query ( DB );
    SQL_QUERY s_query ( DB );

    query.PREPARE_SELECT( "bnk_makro_fisler","fis_tarihi, fis_no, toplam_makro_tutari, def_fis_id, muh_fis_id"," fis_id = :fis_id " );
    query.SET_VALUE     ( ":fis_id", fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    QDate   fis_tarihi            = QDate::fromString( query.VALUE( 0 ).toString(), "yyyy.MM.dd" );
    int     fis_no                = query.VALUE( 1 ).toInt();
    double  toplam_makro_tutari   = query.VALUE( 2 ).toDouble();
    int     def_fis_id            = query.VALUE( 3 ).toInt();
    int     muh_fis_id            = query.VALUE( 4 ).toInt();


    query.PREPARE_SELECT( "bnk_makro_fis_satirlari","fis_satiri_id","fis_id = :fis_id " );
    query.SET_VALUE     ( ":fis_id", fis_id );

    int fis_satiri_id = 0;
    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true ) {

            fis_satiri_id = query.VALUE( 0 ).toInt();

            s_query.PREPARE_SELECT ( "bnk_makro_fis_satirlari",
                                     "makro_id, makro_tutari",
                                     "fis_satiri_id = :fis_satiri_id");
            s_query.SET_VALUE      ( ":fis_satiri_id", fis_satiri_id );

            if ( s_query.SELECT() EQ 0 ) {
                return;
            }

            s_query.NEXT();

            int     makro_id       = s_query.VALUE(0).toInt();
            //double  makro_tutari   = s_query.VALUE(1).toDouble();

            s_query.PREPARE_SELECT( "bnk_makro_satirlari","hesap_turu, hesaba_etkisi, muh_hesap_id, makro_yuzdesi, sabit_tutar, fark_satiri_mi ",
                                  "makro_id = :makro_id " );
            s_query.SET_VALUE     ( ":makro_id", makro_id );

            if ( s_query.SELECT() EQ 0 ) {
                return;
            }
            int makro_hesap_turu = 0;
            while ( s_query.NEXT() EQ true ) {
                makro_hesap_turu = s_query.VALUE( 0 ).toInt();

                switch ( makro_hesap_turu ) {

                case ENUM_BNK_BANKA_HESABI :
                case ENUM_BNK_POS_HESABI:
                case ENUM_BNK_KREDI_KARTI_HESABI: {

                        BNK_DEFTER_FIS_STRUCT * DEF_FIS = new BNK_DEFTER_FIS_STRUCT;
                        //BNK_DEFTER_FIS_SATIRI_STRUCT * BNK_DEF_FIS_SATIRI = new BNK_DEFTER_FIS_SATIRI_STRUCT;

                        BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS , def_fis_id );

                        BNK_DEFTER_FISI_SIL( DEF_FIS, def_fis_id );

                        delete DEF_FIS;
                        break;
                    }

                case ENUM_BNK_MUHASEBE_HESABI:
                case ENUM_BNK_KASA_HESABI: {

                        MUH_FIS_STRUCT   * MUH_FIS  = new MUH_FIS_STRUCT;

                        MUH_CLEAR_FIS_STRUCT( MUH_FIS );

                        MUH_FIS_BILGILERINI_OKU( MUH_FIS, muh_fis_id );

                        MUH_FIS_SIL( MUH_FIS, muh_fis_id );

                        delete MUH_FIS;
                        break;
                    }
                case ENUM_BNK_CARI_HESAP: {

                        CARI_FIS_STRUCT * CARI_FIS    = new CARI_FIS_STRUCT;

                        CARI_FIS->program_id = E9_PROGRAMI;
                        CARI_FIS->modul_id   = BANKA_MODULU;
                        CARI_FIS->base_fis_id= def_fis_id;

                        int cari_fis_id = CARI_GET_ENT_FIS_ID( E9_PROGRAMI, BANKA_MODULU, def_fis_id );

                        CARI_FISI_SIL( CARI_FIS, cari_fis_id );

                        delete CARI_FIS;
                        break;
                    }
                }
            }

            SQL_QUERY s_query ( DB );

            s_query.PREPARE_DELETE("bnk_makro_fis_satirlari","fis_satiri_id = :fis_satiri_id");
            s_query.SET_VALUE(":fis_satiri_id" , fis_satiri_id);
            s_query.DELETE();
        }
    }

    query.PREPARE_DELETE("bnk_makro_fisler","fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , fis_id);
    query.DELETE();

    MUH_FIS_STRUCT   * MUH_FIS  = new MUH_FIS_STRUCT;

    MUH_FIS->program_id  = E9_PROGRAMI;
    MUH_FIS->modul_id    = BANKA_MODULU;
    MUH_FIS->base_fis_id = fis_id;

    MUH_FIS_SIL( MUH_FIS, m_muh_fis_id );

    delete MUH_FIS;

    m_log_kaydi_eklensin_mi = true;
    m_kayit_silindi         = true;

    m_log_detaylari =  QObject::tr("Log Türü - Banka Fişleri , Banka Makro Fişi") ;

    m_log_detaylari.append(","  +  fis_tarihi.toString("dd MMMM yyyy dddd") + "," +
                         QObject::tr("Fiş No : ") + fis_no + QObject::tr("Makro Tutarı :") +
                         VIRGUL_EKLE ( QVariant ( toplam_makro_tutari ).toString(), 2 ) );


}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::SELECT_FIS_RECORD
*****************************************************************************************************/

int BNK_MAKRO_FISI::SELECT_FIS_RECORD()
{
   return OPEN_BNK_MAKRO_FISI_ARAMA ( ENUM_BANKA_MAKRO_FISI, BANKA_MODULU, this );

}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::FIND_FIS_RECORD
*****************************************************************************************************/

int BNK_MAKRO_FISI::FIND_FIS_RECORD()
{
    if ( m_ui->lineedit_fis_no->text().toInt() EQ 0 ) {
        MSG_WARNING( tr("Aranacak fişin numarasını,Fiş No alanına yazmalısınız!.."), m_ui->lineedit_fis_no );
        return 0;
    }

    SQL_QUERY sql_query ( DB );

    QString where_str = "fis_no       = :fis_no             "
                        "AND modul_id       = :modul_id           "
                        "AND fis_tarihi     = :fis_tarihi         ";

    sql_query.PREPARE_SELECT ( "bnk_makro_fisler","fis_id",where_str);

    sql_query.SET_VALUE ( ":fis_no"       , m_ui->lineedit_fis_no->text().toInt()                      );
    sql_query.SET_VALUE ( ":modul_id"     , m_fis_kaynagi                                              );
    sql_query.SET_VALUE ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr("Aradığınız Banka Fişi bulunamadı!.."), NULL );
        return 0;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();

}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::FIND_FIRST_FIS_RECORD
*****************************************************************************************************/

int BNK_MAKRO_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );

    QString where_str = " modul_id = :modul_id  ";

    sql_query.PREPARE_SELECT ( "bnk_makro_fisler","fis_id",where_str);

    sql_query.SET_VALUE(":modul_id"  , m_fis_kaynagi           );

    if ( sql_query.SELECT("fis_tarihi ASC, fis_no ASC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::FIND_LAST_FIS_RECORD
*****************************************************************************************************/

int BNK_MAKRO_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );

    QString where_str = "modul_id = :modul_id ";

    sql_query.PREPARE_SELECT ( "bnk_makro_fisler","fis_id",where_str);

    sql_query.SET_VALUE(":modul_id"  , m_fis_kaynagi           );

    if ( sql_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}


/*****************************************************************************************************
                                BNK_MAKRO_FISI::FIND_PREV_FIS_RECORD
*****************************************************************************************************/

int BNK_MAKRO_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY sql_query(DB);

    QString where_str = "fis_no         < :fis_no         "
                        "AND   modul_id       = :modul_id       "
                        "AND   fis_tarihi     = :fis_tarihi     ";

    sql_query.PREPARE_SELECT ( "bnk_makro_fisler","fis_id",where_str );

    sql_query.SET_VALUE(":fis_no"       , m_ui->lineedit_fis_no->text().toInt());
    sql_query.SET_VALUE(":modul_id"     , m_fis_kaynagi         );
    sql_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE());

    if ( sql_query.SELECT("fis_no DESC",0,1) > 0 ) {
        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    where_str = "modul_id       = :modul_id       "
                "AND   fis_tarihi     < :fis_tarihi     ";

    sql_query.PREPARE_SELECT ("bnk_makro_fisler","fis_id",where_str );

    sql_query.SET_VALUE ( ":modul_id"     , m_fis_kaynagi         );
    sql_query.SET_VALUE ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( sql_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();

}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::FIND_NEXT_FIS_RECORD
*****************************************************************************************************/

int BNK_MAKRO_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY sql_query(DB);

    QString where_str = "fis_no         > :fis_no         "
                        "AND   modul_id       = :modul_id       "
                        "AND   fis_tarihi     = :fis_tarihi     ";

    sql_query.PREPARE_SELECT ( "bnk_makro_fisler","fis_id",where_str);

    sql_query.SET_VALUE ( ":fis_no" ,       m_ui->lineedit_fis_no->text().toInt() );
    sql_query.SET_VALUE ( ":modul_id"     , m_fis_kaynagi                         );
    sql_query.SET_VALUE ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE()    );

    if ( sql_query.SELECT("fis_no",0,1) > 0 ) {
        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    where_str = "modul_id       = :modul_id       "
                "AND   fis_tarihi     > :fis_tarihi     ";

    sql_query.PREPARE_SELECT ( "bnk_makro_fisler","fis_id",where_str);

    sql_query.SET_VALUE ( ":modul_id"     , m_fis_kaynagi                                            );
    sql_query.SET_VALUE ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( sql_query.SELECT("fis_tarihi ASC, fis_no ASC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_LINE_VAR
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{
    Q_UNUSED ( object );
    Q_UNUSED ( row_number );

    if ( m_fis_kaynagi EQ CARI_MODULU ) {
        MSG_WARNING( tr("Fiş Cari Modülü tarafından üretilmiş.Fişte değişiklik yapamazsınız!.."), NULL );
        return ADAK_FAIL;
    }

    SQL_QUERY select_query(DB);


    QComboBox   * combobox_makro_turu        = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_TURU_COLUMN      );
    QSearchEdit * searchedit_hesap           = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_COLUMN           );
    QLineEdit   * lineedit_hesap_ismi        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_ISMI_COLUMN      );
    QCommaEdit  * commaedit_makro_tutari     = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_TUTAR_COLUMN     );
    QLineEdit   * lineedit_makro_id          = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_ID_COLUMN        );
    QCommaEdit  * commaedit_oto_odeme_tutari = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, OTO_ODEME_TUTAR_COLUMN );
    QLineEdit   * lineedit_aciklama          = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN        );
    QLineEdit   * lineedit_para_birimi       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_PARA_BIRIMI_COLUMN);
    QCommaEdit  * commaedit_kur              = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KUR_COLUMN             );
    QCommaEdit  * commaedit_parite           = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PARITE_COLUMN          );
    QCommaEdit  * commaedit_satir_tutari     = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SATIR_TUTARI_COLUMN    );

    if ( m_ui->combo_box_banka_hesap_kodu->currentIndex() EQ -1 ) {
        MSG_WARNING( tr("Önce Banka Hesabı seçin!.."), m_ui->combo_box_banka_hesap_kodu );
        return ADAK_FAIL;
    }

    if ( object EQ combobox_makro_turu ) {

        if  ( combobox_makro_turu->currentText().isEmpty() EQ true ) {
            lineedit_makro_id->clear();
            return ADAK_OK;
        }

        searchedit_hesap->SET_TEXT("");
        lineedit_hesap_ismi->clear();
        commaedit_makro_tutari->clear();
        commaedit_oto_odeme_tutari->clear  ();
        lineedit_aciklama->clear();

        searchedit_hesap->setEnabled( true );
        lineedit_para_birimi->clear();

        QTableWidgetItem * hesap_turu_label_item = m_ui->tablewidget_fis_satirlari->item ( row_number, HESAP_TURU_LABEL_COLUMN );
        QTableWidgetItem * hesap_turu_item       = m_ui->tablewidget_fis_satirlari->item ( row_number, HESAP_TURU_COLUMN       );

        select_query.PREPARE_SELECT ( "bnk_makrolar","makro_id","makro_adi = :makro_adi" );
        select_query.SET_VALUE      ( ":makro_adi", combobox_makro_turu->currentText() );

        int makro_id = 0;

        if ( select_query.SELECT() > 0 ) {
            select_query.NEXT();
            makro_id = select_query.VALUE(0).toInt();
            lineedit_makro_id->setText  ( select_query.VALUE(0).toString() );
        }

        SQL_QUERY sql_query ( DB );

        sql_query.PREPARE_SELECT ( "bnk_makro_satirlari","makro_satiri_id","makro_id = :makro_id AND "
                                   "fark_satiri_mi = 1");
        sql_query.SET_VALUE ( ":makro_id", makro_id  );

        if ( sql_query.SELECT() EQ 0 ) {
            commaedit_oto_odeme_tutari->clear();
            commaedit_oto_odeme_tutari->setReadOnly ( true );
        }
        else {
            commaedit_oto_odeme_tutari->setReadOnly ( false );
        }

        // banka ve muhasebe hesaplarinin disinda sadece bir hesap olabilir
        select_query.PREPARE_SELECT ( "bnk_makro_satirlari","hesap_turu","makro_id = :makro_id "
                                      "AND hesap_turu != :banka_hesap_turu AND hesap_turu != :muh_hesap_turu");

        select_query.SET_VALUE ( ":makro_id"         , makro_id                 );
        select_query.SET_VALUE ( ":banka_hesap_turu" , ENUM_BNK_BANKA_HESABI    );
        select_query.SET_VALUE ( ":muh_hesap_turu"   , ENUM_BNK_MUHASEBE_HESABI );

        if ( select_query.SELECT() EQ 0 ) {

            searchedit_hesap->setDisabled  ( true );
            hesap_turu_label_item->setText ("");
            hesap_turu_item->setText       ( QVariant ( -1 ).toString() );
            return ADAK_OK;
        }

        searchedit_hesap->setDisabled ( false );
        select_query.NEXT();
        int hesap_turu = select_query.VALUE(0).toInt();

        hesap_turu_item->setText ( QVariant ( hesap_turu ).toString() );

        switch ( hesap_turu ) {

            case ENUM_BNK_CARI_HESAP        :
                hesap_turu_label_item->setText("Cari Hesap Kodu");
                break;

            case ENUM_BNK_POS_HESABI        :
                hesap_turu_label_item->setText("Pos Numarası");
                break;

            case ENUM_BNK_KREDI_KARTI_HESABI:
                hesap_turu_label_item->setText("Kredi Kartı Numarası");
                break;

            default :
                hesap_turu_label_item->setText("");
                searchedit_hesap->setEnabled( false );
                break;
        }

        if ( m_banka_hesabi_doviz_id  NE m_temel_para_birim_id ) {
            commaedit_kur->setEnabled( true );
            commaedit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( m_banka_hesabi_doviz_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));
        }
        else {
            commaedit_kur->clear();
            commaedit_kur->setEnabled( false );
        }
    }
    else if ( object EQ searchedit_hesap ) {
        int hesap_turu = m_ui->tablewidget_fis_satirlari->item ( row_number, HESAP_TURU_COLUMN )->text().toInt();

        if ( m_ui->combo_box_banka_hesap_kodu->currentIndex() EQ -1 ) {
            MSG_WARNING( tr("Banka hesap kodunu seçmelisiniz."), m_ui->combo_box_banka_hesap_kodu );
            return ADAK_FAIL;
        }

        switch ( hesap_turu ) {

            case ENUM_BNK_CARI_HESAP        : {

                    QString cari_hesap_ismi;
                    int     para_birim_id, cari_hesap_id;

                    int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID(searchedit_hesap->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi, &para_birim_id );

                    if( result EQ 0 ) {
                        MSG_WARNING( tr("Aradığınız cari hesap kodu bulunamadı."), searchedit_hesap );
                        return ADAK_FAIL;
                    }

                    lineedit_hesap_ismi->setText( cari_hesap_ismi );
                    int cari_para_birim_id = para_birim_id;
                    lineedit_para_birimi->setText( DVZ_GET_DOVIZ_KODU( para_birim_id) );

                    /////////////////

                    if( cari_para_birim_id EQ m_banka_hesabi_doviz_id ) {
                        commaedit_satir_tutari->SET_DOUBLE( commaedit_makro_tutari->GET_DOUBLE() );
                        if( cari_para_birim_id EQ m_temel_para_birim_id ) {
                            // ucu ayni
                            commaedit_kur->clear();
                            commaedit_kur->setDisabled(true);
                            commaedit_parite->clear();
                            commaedit_parite->setDisabled(true);
                        }
                        else {
                            // ikiside temelden farkli
                            commaedit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( m_banka_hesabi_doviz_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));
                            commaedit_kur->setEnabled(true);
                            commaedit_parite->clear();
                            commaedit_parite->setDisabled(true);
                        }
                    }
                    else {
                        if( cari_para_birim_id NE m_temel_para_birim_id AND m_banka_hesabi_doviz_id NE m_temel_para_birim_id ) {
                            // ucu farkli
                            commaedit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( m_banka_hesabi_doviz_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));
                            commaedit_kur->setEnabled(true);
                            commaedit_parite->SET_DOUBLE( DVZ_PARITEYI_AL( cari_para_birim_id, m_banka_hesabi_doviz_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));
                            commaedit_parite->setEnabled(true);
                        }
                        else {
                            if( cari_para_birim_id EQ m_temel_para_birim_id ) {
                                // banka ile temel ayni
                                commaedit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( m_banka_hesabi_doviz_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));
                                commaedit_kur->setEnabled(true);
                                commaedit_parite->clear();
                                commaedit_parite->setDisabled(true);
                            }
                            else {
                                //cari ile temel ayni
                                commaedit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( cari_para_birim_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ));
                                commaedit_kur->setEnabled(true);
                                commaedit_parite->clear();
                                commaedit_parite->setDisabled(true);
                            }
                        }
                    }

                    ////////////

                }
                break;

            case ENUM_BNK_POS_HESABI        :

                select_query.PREPARE_SELECT("bnk_hesabin_poslari","pos_id","pos_numarasi =:pos_numarasi "
                                            "AND hesap_no_id = :hesap_no_id");
                select_query.SET_VALUE(":pos_numarasi" , searchedit_hesap->GET_TEXT());
                select_query.SET_VALUE(":hesap_no_id"  , m_bnk_hesap_no_id);

                if(select_query.SELECT() EQ 0 ) {
                     MSG_WARNING( tr("Aradığınız pos numarası bulunamadı."), searchedit_hesap );
                    return ADAK_FAIL;
                }
                break;

            case ENUM_BNK_KREDI_KARTI_HESABI:
            {
                select_query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari","kredi_karti_id, kredi_karti_sahibi","hesap_no_id = :hesap_no_id "
                                            "AND kredi_karti_numarasi = :kredi_karti_numarasi");
                select_query.SET_VALUE(":hesap_no_id"          , m_bnk_hesap_no_id);
                select_query.SET_VALUE(":kredi_karti_numarasi" , searchedit_hesap->GET_TEXT());

                if(select_query.SELECT() EQ 0 ) {
                    MSG_WARNING( tr("Aradığınız kredi kartı numarası bulunamadı."), searchedit_hesap );
                    return ADAK_FAIL;
                }
                select_query.NEXT();
                QLineEdit * line_edit = (QLineEdit *)m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_ISMI_COLUMN);
                if ( line_edit EQ NULL ) {
                    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->item(row_number,HESAP_ISMI_COLUMN);
                    if (item NE NULL) {
                        item->setText(select_query.VALUE(1).toString());
                        break;
                    }
                    m_ui->tablewidget_fis_satirlari->setItem(row_number,HESAP_ISMI_COLUMN,new QTableWidgetItem(select_query.VALUE(1).toString()));
                }
                else {
                    line_edit->setText(select_query.VALUE(1).toString());
                }
                break;
            }
            case ENUM_BNK_KASA_HESABI :
            default               :
                break;
        }
    }
    else if( object EQ commaedit_kur  OR object EQ commaedit_parite OR object EQ commaedit_makro_tutari ) {
        //!     hesaplama
        int cari_para_birim_id, cari_hesap_id;
        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( searchedit_hesap->GET_TEXT(), &cari_hesap_id, NULL, &cari_para_birim_id );

        if( result EQ 1 ) {
            if( cari_para_birim_id EQ m_banka_hesabi_doviz_id ) {
                commaedit_satir_tutari->SET_DOUBLE( commaedit_makro_tutari->GET_DOUBLE() );
                if( cari_para_birim_id EQ m_temel_para_birim_id ) {
                    // ucu ayni
                    commaedit_satir_tutari->SET_DOUBLE( commaedit_makro_tutari->GET_DOUBLE() );
                }
                else {
                    // ikiside temelden farkli
                }
            }
            else {
                if( cari_para_birim_id NE m_temel_para_birim_id AND m_banka_hesabi_doviz_id NE m_temel_para_birim_id ) {
                    // ucu farkli
                    commaedit_satir_tutari->SET_DOUBLE( ROUND( commaedit_makro_tutari->GET_DOUBLE() * commaedit_parite->GET_DOUBLE() ) );
                }
                else {
                    if( cari_para_birim_id EQ m_temel_para_birim_id ) {
                        //cari ile temel ayni
                        commaedit_satir_tutari->SET_DOUBLE( ROUND( commaedit_makro_tutari->GET_DOUBLE() * commaedit_kur->GET_DOUBLE() ) );
                    }
                    else {
                        // banka ile temel ayni
                        commaedit_satir_tutari->SET_DOUBLE( ROUND( commaedit_makro_tutari->GET_DOUBLE() * commaedit_kur->GET_DOUBLE() ) );
                    }
                }
            }
        }
        else {
            if( m_banka_hesabi_doviz_id EQ m_temel_para_birim_id ) {
                commaedit_satir_tutari->SET_DOUBLE( commaedit_makro_tutari->GET_DOUBLE() );
            }
            else {
                commaedit_satir_tutari->SET_DOUBLE( ROUND( commaedit_makro_tutari->GET_DOUBLE() * commaedit_kur->GET_DOUBLE() ) );
            }
        }
    }

    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_LINE_EMPTY
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_LINE_EMPTY(int row_number)
{
    QComboBox   * combobox_makro_turu       = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_TURU_COLUMN  );
    QCommaEdit  * commaedit_tutar           = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_TUTAR_COLUMN );
    QSearchEdit * searchedit_hesap          = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_COLUMN       );
    QLineEdit   * lineedit_makro_id         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_ID_COLUMN    );

    if ( combobox_makro_turu->currentIndex() EQ -1 ) {
        MSG_WARNING( tr("Banka Makro türünü seçmelisiniz!.."), combobox_makro_turu );
        return ADAK_FAIL;
    }
    if ( lineedit_makro_id->text().toInt() EQ 0 ) {
        MSG_WARNING( tr ( "Banka Makro türü seçilmemiş" ), combobox_makro_turu );
        return ADAK_FAIL;
    }

    if ( commaedit_tutar->GET_DOUBLE() EQ 0.0 ) {
        MSG_WARNING( tr("Makro Tutarı bilgisi boş bırakılamaz!.."), commaedit_tutar );
        return ADAK_FAIL;
    }

    if ( searchedit_hesap->GET_TEXT().isEmpty() EQ true ) {
        int hesap_turu = m_ui->tablewidget_fis_satirlari->item(row_number,HESAP_TURU_COLUMN)->text().toInt();

        if ( hesap_turu NE -1 ) {
            switch ( hesap_turu ) {

                case ENUM_BNK_CARI_HESAP        :
                    MSG_WARNING( tr("Cari hesap kodunu boş bırakamazsınız."), searchedit_hesap );
                    return ADAK_FAIL;

                case ENUM_BNK_POS_HESABI        :
                    MSG_WARNING( tr("Pos Numarasını boş bırakamazsınız."), searchedit_hesap );
                    return ADAK_FAIL;

                case ENUM_BNK_KREDI_KARTI_HESABI:
                    MSG_WARNING( tr("Kredi Kartı Numarası boş bırakamazsınız."), searchedit_hesap );
                    return ADAK_FAIL;

                default:
                    break;
            }
        }
    }

    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_ADD_LINE
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_ADD_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id     );
    Q_UNUSED ( row_number );

    if ( m_fis_kaynagi EQ CARI_MODULU ) {
        MSG_WARNING( tr("Fiş Cari Modülü tarafından üretilmiş.Fişte değişiklik yapamazsınız!.."), NULL );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::ADD_LINE
*****************************************************************************************************/

void BNK_MAKRO_FISI::ADD_LINE ( int fis_id, int row_number )
{  
    QLineEdit   * lineedit_aciklama               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN      );
    QCommaEdit  * commaedit_tutar                 = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_TUTAR_COLUMN   );
    QCommaEdit  * commaedit_otomatik_odeme_tutari = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, OTO_ODEME_TUTAR_COLUMN  );
    QSearchEdit * searchedit_hesap                = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_COLUMN         );
    QLineEdit   * lineedit_makro_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_ID_COLUMN      );
    QCommaEdit  * commaedit_doviz_kuru            = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KUR_COLUMN           );
    QCommaEdit  * commaedit_parite                = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PARITE_COLUMN        );
    QCommaEdit  * commaedit_satir_tutari          = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SATIR_TUTARI_COLUMN  );

    int order_number  =  m_ui->tablewidget_fis_satirlari->item ( row_number, ORDER_COLUMN  )->text().toInt();
    int hesap_turu    =  m_ui->tablewidget_fis_satirlari->item ( row_number, HESAP_TURU_COLUMN )->text().toInt();

    MAKRO_SATIR_STRUCT * M_FIS_SATIRI = new MAKRO_SATIR_STRUCT;

    M_FIS_SATIRI->fis_id                = fis_id;
    M_FIS_SATIRI->islem_tutari          = commaedit_tutar->GET_DOUBLE();
    M_FIS_SATIRI->otomatik_odeme_tutari = commaedit_otomatik_odeme_tutari->GET_DOUBLE();
    M_FIS_SATIRI->makro_id              = lineedit_makro_id->text().toInt();
    M_FIS_SATIRI->order_number          = order_number;
    M_FIS_SATIRI->search_edit_hesap     = searchedit_hesap->GET_TEXT();
    M_FIS_SATIRI->doviz_kuru            = ROUND( commaedit_doviz_kuru->GET_DOUBLE() , -4 );
    M_FIS_SATIRI->parite                = ROUND( commaedit_parite->GET_DOUBLE()     , -4 );
    M_FIS_SATIRI->base_record_id        = 0;
    M_FIS_SATIRI->fis_tarihi            = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS_SATIRI->satir_aciklama        = lineedit_aciklama->text();
    M_FIS_SATIRI->banka_doviz_id        = m_banka_hesabi_doviz_id;
    M_FIS_SATIRI->hesap_turu            = hesap_turu;
    M_FIS_SATIRI->hesap_no_id           = m_bnk_hesap_no_id;
    M_FIS_SATIRI->fis_aciklama          = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS_SATIRI->satir_tutari          = commaedit_satir_tutari->GET_DOUBLE();

    int row_id = MAKRO_ADD_LINE( M_FIS_SATIRI );

    QTableWidgetItem * new_item = new QTableWidgetItem(QVariant(row_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number,ROW_ID_COLUMN,new_item);

    new_item                    = new QTableWidgetItem(QVariant(fis_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number,FIS_ID_COLUMN,new_item);

    m_log_kaydi_eklensin_mi = true;
    m_log_detaylari =  QObject::tr("Log Türü - Banka Fişleri , Banka Makro Fişi") ;
    m_log_detaylari.append(","  +  m_ui->adakDate_fis_tarihi->DATE() + "," +
                         QObject::tr("Fiş No : ") + m_ui->lineedit_fis_no->text() + QObject::tr("Makro Tutarı :") +
                         VIRGUL_EKLE ( QVariant ( ROUND( commaedit_tutar->GET_DOUBLE() ) ).toString(), 2 ) );

}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_UPDATE_LINE
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_UPDATE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id     );
    Q_UNUSED ( row_number );

    if ( m_fis_kaynagi EQ CARI_MODULU ) {
        MSG_WARNING( tr("Fiş Cari Modülü tarafından üretilmiş.Fişte değişiklik yapamazsınız!.."), NULL );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::UPDATE_LINE
*****************************************************************************************************/

void BNK_MAKRO_FISI::UPDATE_LINE ( int fis_id, int row_number )
{
    SQL_QUERY select_query ( DB );
    SQL_QUERY query        ( DB );

    QLineEdit   * lineedit_aciklama               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN      );
    QCommaEdit  * commaedit_tutar                 = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_TUTAR_COLUMN   );
    QCommaEdit  * commaedit_otomatik_odeme_tutari = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, OTO_ODEME_TUTAR_COLUMN  );
    QLineEdit   * lineedit_makro_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_ID_COLUMN      );
    QCommaEdit  * commaedit_doviz_kuru            = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KUR_COLUMN           );
    QCommaEdit  * commaedit_parite                = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PARITE_COLUMN        );

    int hesap_turu    = m_ui->tablewidget_fis_satirlari->item ( row_number, HESAP_TURU_COLUMN )->text().toInt();
    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();
    int makro_id      = lineedit_makro_id->text().toInt();

    select_query.PREPARE_SELECT ( "bnk_makro_fis_satirlari, bnk_makro_fisler","fis_tarihi, bnk_makro_fisler.aciklama, makro_tutari, otomatik_odeme_tutari, hesap_turu, "
                                  "hesap_id, doviz_kuru, parite, bnk_makro_fisler",
                                  "bnk_makro_fisler.fis_id = bnk_makro_fis_satirlari.fis_id "
                                  "AND fis_satiri_id = :fis_satiri_id");

    select_query.SET_VALUE ( ":fis_satiri_id", fis_satiri_id );
    if ( select_query.SELECT() EQ 0 ) {
        return;
    }

    select_query.NEXT();

    QDate   old_fis_tarihi            = QDate::fromString(select_query.VALUE( 0 ).toString(), "yyyy.MM.dd");
    //int     old_aciklama              = select_query.VALUE( 1 ).toInt();
    double  old_makro_tutar           = select_query.VALUE( 2 ).toDouble();
    double  old_otomatik_odeme_tutar  = select_query.VALUE( 3 ).toDouble();
    int     old_hesap_turu            = select_query.VALUE( 4 ).toInt();
    //int     old_hesap_id              = select_query.VALUE( 5 ).toInt();
    double  old_doviz_kuru            = select_query.VALUE( 6 ).toDouble();
    double  old_parite                = select_query.VALUE( 7 ).toDouble();


    if ( old_fis_tarihi NE m_ui->adakDate_fis_tarihi->QDATE() OR
         old_makro_tutar NE commaedit_tutar->GET_DOUBLE() OR
         old_otomatik_odeme_tutar NE commaedit_otomatik_odeme_tutari->GET_DOUBLE() OR
         old_hesap_turu NE  hesap_turu OR
         old_doviz_kuru NE commaedit_doviz_kuru->GET_DOUBLE() OR
         old_parite NE commaedit_parite->GET_DOUBLE() ) {

        DELETE_ENT_LINE( fis_id, row_number, ENUM_UPDATE_LINE );
        ADD_ENT_LINE( fis_id, row_number, ENUM_UPDATE_LINE );

    }
    else {
        // sadece aciklama degisti

        query.PREPARE_SELECT( "bnk_makro_satirlari","hesap_turu, hesaba_etkisi, muh_hesap_id, makro_yuzdesi, sabit_tutar, fark_satiri_mi ",
                              "makro_id = :makro_id " );
        query.SET_VALUE     ( ":makro_id", makro_id );

        if ( query.SELECT() EQ 0 ) {
            return;
        }

        int makro_hesap_turu = 0;

        while ( query.NEXT() EQ true  ) {
            makro_hesap_turu = query.VALUE( 0 ).toInt();

            switch ( makro_hesap_turu ) {

            case ENUM_BNK_BANKA_HESABI:
            case ENUM_BNK_POS_HESABI:
            case ENUM_BNK_KREDI_KARTI_HESABI: {

                    BNK_DEFTER_FIS_STRUCT        * DEF_FIS        = new BNK_DEFTER_FIS_STRUCT;
                    BNK_DEFTER_FIS_SATIRI_STRUCT * DEF_FIS_SATIRI = new BNK_DEFTER_FIS_SATIRI_STRUCT;

                    BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, fis_id );

                    QStringList row_list    = GET_BNK_DEFTER_ROW_ID_LIST( DEF_FIS->program_id, DEF_FIS->modul_id, fis_satiri_id );

                    for ( int i = 0; i < row_list.count(); i++ ) {

                        CLEAR_BNK_DEFTER_FIS_SATIRI_STRUCT( DEF_FIS_SATIRI );
                        BNK_DEFTER_FIS_SATIR_BILGILERINI_OKU( DEF_FIS_SATIRI, QVariant ( row_list.at( i ) ).toInt() );
                        DEF_FIS_SATIRI->aciklama = lineedit_aciklama->text();
                        BNK_DEFTER_FIS_SATIRINI_GUNCELLE( DEF_FIS, DEF_FIS_SATIRI, i );
                    }
                    delete DEF_FIS;
                    delete DEF_FIS_SATIRI;

                    break;
                }
            case ENUM_BNK_MUHASEBE_HESABI:
            case ENUM_BNK_KASA_HESABI: {

                    MUH_FIS_STRUCT         * MUH_FIS          = new MUH_FIS_STRUCT;
                    MUH_FIS_SATIRI_STRUCT  * MUH_FIS_SATIRI   = new MUH_FIS_SATIRI_STRUCT;

                    MUH_FIS_BILGILERINI_OKU( MUH_FIS, fis_id );

                    QStringList row_list = MUH_GET_ENT_ROW_ID_LIST( MUH_FIS->program_id, MUH_FIS->modul_id, fis_satiri_id );

                    for ( int i = 0; i < row_list.count(); i++ ) {

                        MUH_CLEAR_FIS_SATIRI_STRUCT( MUH_FIS_SATIRI );
                        MUH_FIS_SATIRINI_OKU( MUH_FIS_SATIRI, QVariant ( row_list.at( i ) ).toInt() );
                        MUH_FIS_SATIRI->aciklama = lineedit_aciklama->text();
                        MUH_FIS_SATIRINI_GUNCELLE( MUH_FIS, MUH_FIS_SATIRI, i );
                    }

                    delete MUH_FIS;
                    delete MUH_FIS_SATIRI;

                    break;
                }
            case ENUM_BNK_CARI_HESAP: {

                    CARI_FIS_STRUCT        * CARI_FIS           = new CARI_FIS_STRUCT;
                    CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI    = new CARI_FIS_SATIRI_STRUCT;

                    CARI_FIS_BILGILERINI_OKU( CARI_FIS, fis_id );
                    QStringList row_list = CARI_GET_ENT_ROW_ID_LIST( CARI_FIS->program_id, CARI_FIS->modul_id, fis_satiri_id );

                    for ( int i = 0; i < row_list.count(); i++ ) {
                        CARI_CLEAR_FIS_SATIRI_STRUCT ( CARI_FIS_SATIRI );
                        CARI_FIS_SATIRINI_OKU( CARI_FIS_SATIRI, QVariant ( row_list.at( i ) ).toInt() );
                        CARI_FIS_SATIRI->aciklama = lineedit_aciklama->text();
                        CARI_FIS_SATIRINI_GUNCELLE( CARI_FIS, CARI_FIS_SATIRI, i );
                    }

                    delete CARI_FIS;
                    delete CARI_FIS_SATIRI;

                    break;
                }
            }
        }

    }

    SQL_QUERY update_query ( DB );

    update_query.PREPARE_UPDATE ( "bnk_makro_fis_satirlari","fis_satiri_id","aciklama ","fis_satiri_id =:fis_satiri_id " );

    update_query.SET_VALUE ( ":aciklama"              , lineedit_aciklama->text()  );
    update_query.SET_VALUE ( ":fis_satiri_id"         , fis_satiri_id              );

    update_query.UPDATE();

    m_log_kaydi_eklensin_mi = true;
    m_log_detaylari         =  QObject::tr("Log Türü - Banka Fişleri , Banka Makro Fişi") ;

    m_log_detaylari.append(","  +  m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") + "," +
                         QObject::tr("Fiş No : ") + m_ui->lineedit_fis_no->text() );



}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::CHECK_DELETE_LINE
*****************************************************************************************************/

int BNK_MAKRO_FISI::CHECK_DELETE_LINE(int fis_id, int row_number)
{
    Q_UNUSED ( fis_id     );
    Q_UNUSED ( row_number );

    if ( m_fis_kaynagi EQ CARI_MODULU ) {
        MSG_WARNING( tr("Fiş Cari Modülü tarafından üretilmiş.Fişte değişiklik yapamazsınız!.."), NULL );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::DELETE_LINE
*****************************************************************************************************/

void BNK_MAKRO_FISI::DELETE_LINE ( int fis_id, int row_number )
{
    DELETE_ENT_LINE( fis_id, row_number, ENUM_DELETE_LINE );
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::LOCK_FIS_RECORD
*****************************************************************************************************/

int BNK_MAKRO_FISI::LOCK_FIS_RECORD(int fis_id)
{
    return DB->LOCK_ROW ( "fis_id","bnk_makro_fisler", QString ( "fis_id = %1" ).arg ( fis_id ));
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::UNLOCK_FIS_RECORD
*****************************************************************************************************/

void BNK_MAKRO_FISI::UNLOCK_FIS_RECORD(int fis_id)
{
    DB->UNLOCK_ROW ( "fis_id","bnk_makro_fisler", QString ( "fis_id = %1" ).arg ( fis_id ));
}

/****************************************************************************************************
                                BNK_MAKRO_FISI::LEAVE_RECORD()
*****************************************************************************************************/

int BNK_MAKRO_FISI::LEAVE_RECORD()
{
    if ( m_log_kaydi_eklensin_mi EQ true ) {
        DB->START_TRANSACTION();

        if ( m_kayit_eklendi EQ true ) {
            E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_FISLER , LOG_ISLEM_ADD , m_log_detaylari);
        }
        if ( m_kayit_silindi EQ true ) {
            E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_FISLER , LOG_ISLEM_DELETE , m_log_detaylari);
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
            E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_FISLER , LOG_ISLEM_UPDATE , m_log_detaylari);
        }
        DB->COMMIT_TRANSACTION();
    }
    return ADAK_OK;

}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::LINE_SEARCH_EDIT_CLICKED
*****************************************************************************************************/

void BNK_MAKRO_FISI::LINE_SEARCH_EDIT_CLICKED ( int row_number,QWidget * widget, QLineEdit * line_edit )
{
    Q_UNUSED ( widget );

    int hesap_turu = m_ui->tablewidget_fis_satirlari->item ( row_number, HESAP_TURU_COLUMN )->text().toInt();

    switch ( hesap_turu ) {
        case ENUM_BNK_CARI_HESAP :
        {
            int cari_hesap_id = OPEN_CARI_KART_ARAMA( "", SAHIS_ARTI_FIRMA,this,true, E9_PROGRAMI, CARI_MODULU );
            if(cari_hesap_id NE -1) {
                line_edit->setText(CARI_FIND_HESAP_KODU(cari_hesap_id));
            }
            break;
        }

        case ENUM_BNK_POS_HESABI:
        {
            int pos_id = OPEN_BNK_POS_SEC(m_bnk_hesap_no_id,this);

            if ( pos_id NE -1 ) {
                QString pos_numarasi = BNK_GET_POS_NUMARASI(pos_id);

                if ( pos_numarasi NE NULL ) {
                    line_edit->setText(pos_numarasi);
                }
            }
            break;
        }
        case ENUM_BNK_KREDI_KARTI_HESABI:
        {
            int kredi_karti_id = OPEN_BNK_KREDI_KARTI_SEC ( m_bnk_hesap_no_id,this);

            if ( kredi_karti_id NE 0 ) {
                line_edit->setText(BNK_GET_KREDI_KARTI_NUMARASI(kredi_karti_id));
            }
            break;
        }
        default :
            break;
    }

}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::SAVER_BUTTON_CLICKED
*****************************************************************************************************/

void BNK_MAKRO_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_record_id )
{
    if( p_button EQ m_ui->buton_yazdir ) {
        OPEN_REPORT_SHOWER ( GET_BNK_FISI_YAZDIR ( p_record_id , ENUM_BANKA_MAKRO_FISI ), nativeParentWidget() );
    }
    else if ( p_button EQ m_ui->pushButton_urettigi_fisler ) {
        int def_fis_id = GET_BNK_DEFTER_FIS_ID( E9_PROGRAMI, BANKA_MODULU, p_record_id );
        E9_URETILEN_FIS_SEC( E9_PROGRAMI, BANKA_MODULU, def_fis_id, this );
    }
}


/*****************************************************************************************************
                                BNK_MAKRO_FISI::GET_CARI_MODULU_FIS_NO
*****************************************************************************************************/

int BNK_MAKRO_FISI::GET_CARI_MODULU_FIS_NO ( int cari_hesap_id, int fis_id )
{
    SQL_QUERY select_query ( DB );

    int car_fis_id = 0;

    select_query.PREPARE_SELECT ( "bnk_makro_fis_satirlari","car_fis_id,cari_hesap_id","fis_id = :fis_id" );
    select_query.SET_VALUE      ( ":fis_id" , fis_id);

    if ( select_query.SELECT() EQ 0 ) {
        return 0;
    }

    bool fiste_ayni_cari_hesap_bulundu = false;

    while ( select_query.NEXT() EQ true ) {
        car_fis_id = select_query.VALUE(0).toInt();

        int line_cari_hesap_id = select_query.VALUE(1).toInt();
        if ( line_cari_hesap_id EQ cari_hesap_id ) {
            fiste_ayni_cari_hesap_bulundu = true;
            break;
        }
    }
    if ( fiste_ayni_cari_hesap_bulundu EQ true ) {
        return car_fis_id;
    }

    return 0;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::GET_MAKRO_ID
*****************************************************************************************************/

int BNK_MAKRO_FISI::GET_MAKRO_ID ( QString makro_adi )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "bnk_makrolar","makro_id","makro_adi=:makro_adi" );
    select_query.SET_VALUE      ( ":makro_adi",makro_adi);

    if ( select_query.SELECT() EQ 0 ) {
        return -1;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::GET_MAKRO_TURLERI
*****************************************************************************************************/

void BNK_MAKRO_FISI::GET_MAKRO_TURLERI()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT ( "bnk_makrolar","makro_adi" );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }

    while( select_query.NEXT() EQ true ) {
        m_makro_turleri << select_query.VALUE(0).toString();
    }
}


/*****************************************************************************************************
                                BNK_MAKRO_FISI::ISLEME_CARI_HESAP_GEREKLIMI
*****************************************************************************************************/

bool BNK_MAKRO_FISI::ISLEME_CARI_HESAP_GEREKLIMI(int makro_id)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT ( "bnk_makrolar","makro_id","makro_id=:makro_id AND hesap_turu=:hesap_turu" );
    select_query.SET_VALUE      ( ":makro_id",   makro_id        );
    select_query.SET_VALUE      ( ":hesap_turu", ENUM_BNK_CARI_HESAP );

    if ( select_query.SELECT() EQ 0 ) {
        return false;
    }
    return true;
}

/*****************************************************************************************************
                                BNK_MAKRO_FISI::SEARCH_EDIT_CLICKED
*****************************************************************************************************/

void BNK_MAKRO_FISI::SEARCH_EDIT_CLICKED ( QWidget * widget, QLineEdit * line_edit )
{
    Q_UNUSED ( widget );

    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA, this, true , E9_PROGRAMI, CARI_MODULU );

    if ( cari_hesap_id NE -1 ) {
        line_edit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id ) );
    }
}

/**************************************************************************************
                   BNK_MAKRO_FISI::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int BNK_MAKRO_FISI::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    Q_UNUSED ( p_button );

    M_ENT_DETAYLARI->banka_fis_no.clear();
    M_ENT_DETAYLARI->cari_fis_no        = 0;
    M_ENT_DETAYLARI->cek_fis_no         = 0;
    M_ENT_DETAYLARI->firma_cek_fis_no   = 0;
    M_ENT_DETAYLARI->firma_senet_fis_no = 0;
    M_ENT_DETAYLARI->muh_fis_no         = 0;
    M_ENT_DETAYLARI->senet_fis_no       = 0;

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   BNK_MAKRO_FISI::ADD_ENT_LINE
***************************************************************************************/

void BNK_MAKRO_FISI::ADD_ENT_LINE( int fis_id, int row_number, int base_method )
{

    Q_UNUSED ( base_method );
    QLineEdit   * lineedit_aciklama               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN      );
    QCommaEdit  * commaedit_tutar                 = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_TUTAR_COLUMN   );
    QCommaEdit  * commaedit_otomatik_odeme_tutari = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, OTO_ODEME_TUTAR_COLUMN  );
    QSearchEdit * searchedit_hesap                = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_COLUMN         );
    QLineEdit   * lineedit_makro_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKRO_ID_COLUMN      );
    QCommaEdit  * commaedit_doviz_kuru            = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KUR_COLUMN           );
    QCommaEdit  * commaedit_parite                = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PARITE_COLUMN        );

    int fis_satiri_id =  m_ui->tablewidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();
    int order_number  =  m_ui->tablewidget_fis_satirlari->item( row_number, ORDER_COLUMN )->text().toInt();
    int hesap_turu    =  m_ui->tablewidget_fis_satirlari->item ( row_number, HESAP_TURU_COLUMN )->text().toInt();

    MAKRO_SATIR_STRUCT * M_FIS_SATIRI = new MAKRO_SATIR_STRUCT;

    M_FIS_SATIRI->fis_id                = fis_id;
    M_FIS_SATIRI->islem_tutari          = ROUND( commaedit_tutar->GET_DOUBLE());
    M_FIS_SATIRI->otomatik_odeme_tutari = ROUND( commaedit_otomatik_odeme_tutari->GET_DOUBLE());
    M_FIS_SATIRI->makro_id              = lineedit_makro_id->text().toInt();
    M_FIS_SATIRI->order_number          = order_number;
    M_FIS_SATIRI->search_edit_hesap     = searchedit_hesap->GET_TEXT();
    M_FIS_SATIRI->doviz_kuru            = ROUND( commaedit_doviz_kuru->GET_DOUBLE() );
    M_FIS_SATIRI->parite                = ROUND( commaedit_parite->GET_DOUBLE() );
    M_FIS_SATIRI->base_record_id        = fis_satiri_id;
    M_FIS_SATIRI->fis_tarihi            = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS_SATIRI->satir_aciklama        = lineedit_aciklama->text();
    M_FIS_SATIRI->banka_doviz_id        = m_banka_hesabi_doviz_id;
    M_FIS_SATIRI->hesap_turu            = hesap_turu;
    M_FIS_SATIRI->hesap_no_id           = m_bnk_hesap_no_id;
    M_FIS_SATIRI->fis_aciklama          = m_ui->limitedTextEdit_aciklama->toPlainText();

    MAKRO_ADD_LINE( M_FIS_SATIRI );
}

void BNK_MAKRO_FISI::DELETE_ENT_LINE( int fis_id, int row_number, int base_method)
{

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("bnk_makro_fisler","toplam_makro_tutari ,fis_tarihi, fis_no, def_fis_id, muh_fis_id ",
                         "fis_id = :fis_id   ");

    query.SET_VALUE(":fis_id", fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    double      toplam_makro_tutari   = query.VALUE(0).toDouble();
    QDate       fis_tarihi            = QDate::fromString(query.VALUE(1).toString() , "yyyy.MM.dd");
    QString     fis_no                = query.VALUE(2).toString();
    int         fis_satiri_id         = m_ui->tablewidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();
    int         def_fis_id            = query.VALUE( 3 ).toInt();
    int         muh_fis_id            = query.VALUE( 4 ).toInt();


    query.PREPARE_SELECT ( "bnk_makro_fis_satirlari","makro_id, makro_tutari","fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE      ( ":fis_satiri_id", fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    int     makro_id       = query.VALUE(0).toInt();
    double  makro_tutari   = query.VALUE(1).toDouble();

    query.PREPARE_SELECT( "bnk_makro_satirlari","hesap_turu, hesaba_etkisi, muh_hesap_id, makro_yuzdesi, sabit_tutar, fark_satiri_mi ",
                          "makro_id = :makro_id " );
    query.SET_VALUE     ( ":makro_id", makro_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    int makro_hesap_turu = 0;
    while ( query.NEXT() EQ true ) {
        makro_hesap_turu = query.VALUE( 0 ).toInt();

        switch ( makro_hesap_turu ) {

        case ENUM_BNK_BANKA_HESABI :
        case ENUM_BNK_POS_HESABI:
        case ENUM_BNK_KREDI_KARTI_HESABI: {

                BNK_DEFTER_FIS_STRUCT        * DEF_FIS        = new BNK_DEFTER_FIS_STRUCT;

                CLEAR_BNK_DEFTER_FIS_STRUCT ( DEF_FIS );

                BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS , def_fis_id );

                QStringList def_row_list    = GET_BNK_DEFTER_ROW_ID_LIST( DEF_FIS->program_id, DEF_FIS->modul_id, fis_satiri_id );

                for( int i = 0; i < def_row_list.count(); i++ ) {
                    BNK_DEFTER_FIS_SATIRI_SIL( DEF_FIS, QVariant ( def_row_list.at( i ) ).toInt() );
                }

                delete DEF_FIS;
                break;
            }

        case ENUM_BNK_MUHASEBE_HESABI:
        case ENUM_BNK_KASA_HESABI: {

                MUH_FIS_STRUCT   * MUH_FIS  = new MUH_FIS_STRUCT;

                MUH_CLEAR_FIS_STRUCT( MUH_FIS );

                MUH_FIS_BILGILERINI_OKU( MUH_FIS, muh_fis_id );

                QStringList muh_row_list    = MUH_GET_ENT_ROW_ID_LIST( MUH_FIS->program_id, MUH_FIS->modul_id, fis_satiri_id );

                for( int i = 0; i < muh_row_list.count(); i++ ) {
                    MUH_FIS_SATIRINI_SIL( MUH_FIS, QVariant ( muh_row_list.at( i ) ).toInt() );
                }

                delete MUH_FIS;

                break;
            }
        case ENUM_BNK_CARI_HESAP: {

                CARI_FIS_STRUCT * CARI_FIS    = new CARI_FIS_STRUCT;

                CARI_FIS->program_id = E9_PROGRAMI;
                CARI_FIS->modul_id   = BANKA_MODULU;
                CARI_FIS->base_fis_id= def_fis_id;


                QStringList cari_row_list    = CARI_GET_ENT_ROW_ID_LIST( CARI_FIS->program_id, CARI_FIS->modul_id, fis_satiri_id );

                for( int i = 0; i < cari_row_list.count(); i++ ) {
                    CARI_FIS_SATIRI_SIL( CARI_FIS, QVariant ( cari_row_list.at( i ) ).toInt() );
                }

                delete CARI_FIS;
                break;
            }
        }
    }

    SQL_QUERY delete_query ( DB );

    delete_query.PREPARE_DELETE("bnk_makro_fis_satirlari","fis_satiri_id = :fis_satiri_id");
    delete_query.SET_VALUE(":fis_satiri_id" , fis_satiri_id);
    delete_query.DELETE();

    toplam_makro_tutari   = ROUND ( toplam_makro_tutari - makro_tutari );
    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( toplam_makro_tutari );

    SQL_QUERY update_query ( DB );

    update_query.PREPARE_UPDATE ( "bnk_makro_fisler","fis_id","toplam_makro_tutari  ","fis_id = :fis_id");

    update_query.SET_VALUE(":toplam_makro_tutari"   , toplam_makro_tutari);
    update_query.SET_VALUE(":fis_id"                , fis_id);
    update_query.UPDATE();


    if ( base_method EQ ENUM_DELETE_LINE ) {
        m_log_kaydi_eklensin_mi = true;
        m_log_detaylari         =  QObject::tr("Log Türü - Banka Fişleri , Banka Makro Fişi") ;

        m_log_detaylari.append(","  +  fis_tarihi.toString("dd MMMM yyyy dddd") + "," +
                             QObject::tr("Fiş No : ") + fis_no + QObject::tr("Makro Tutarı :") +
                             VIRGUL_EKLE ( QVariant ( toplam_makro_tutari ).toString(), 2 ) );
    }
}


/*****************************************************************************************************
                               MAKRO_ADD_LINE
*****************************************************************************************************/

int MAKRO_ADD_LINE( MAKRO_SATIR_STRUCT * P_FIS_SATIRI, int p_fis_satiri_id , bool base_fis_satiri_eklenecek , ADAK_SQL *P_GELEN_DB )
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    double otomatik_odeme_tutari = ROUND( P_FIS_SATIRI->otomatik_odeme_tutari );
    double islem_tutari          = ROUND( P_FIS_SATIRI->islem_tutari );

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT("bnk_makro_fisler","hesap_no_id, toplam_makro_tutari, fis_no ","fis_id = :fis_id");
    query.SET_VALUE(":fis_id", P_FIS_SATIRI->fis_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    int         hesap_no_id              = query.VALUE(0).toInt();
    double      toplam_makro_tutari      = query.VALUE(1).toDouble();
    QString     fis_no                   = query.VALUE(2).toString();

    int         makro_id                 = P_FIS_SATIRI->makro_id;
    int         hesap_turu               = P_FIS_SATIRI->hesap_turu;

    int         cari_hesap_id            = 0;
    int         pos_id                   = 0;
    int         kredi_karti_id           = 0;
    int         cari_para_birimi_id      = 0;

    switch ( hesap_turu ) {

        case ENUM_BNK_CARI_HESAP : {
                QString cari_hesap_ismi;
                CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( P_FIS_SATIRI->search_edit_hesap, &cari_hesap_id, &cari_hesap_ismi, &cari_para_birimi_id );
            }
            break;
        case ENUM_BNK_POS_HESABI:
            pos_id = BNK_GET_POS_ID( P_FIS_SATIRI->search_edit_hesap );
            break;

        case ENUM_BNK_KREDI_KARTI_HESABI:
            query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari","kredi_karti_id","kredi_karti_numarasi = :kredi_karti_numarasi  "
                                        "AND hesap_no_id = :hesap_no_id");
            query.SET_VALUE(":kredi_karti_numarasi" , P_FIS_SATIRI->search_edit_hesap );
            query.SET_VALUE(":hesap_no_id"          , hesap_no_id);
            if(query.SELECT() NE 0 ) {
                query.NEXT();
                kredi_karti_id = query.VALUE(0).toInt();
            }
            break;

        default :
            break;
    }

    int row_id = p_fis_satiri_id;
    if ( base_fis_satiri_eklenecek EQ true ) {
        query.PREPARE_INSERT("bnk_makro_fis_satirlari","fis_satiri_id" ,
                             "fis_id, makro_id, aciklama, makro_tutari, "
                             "otomatik_odeme_tutari, order_number, "
                             "temel_tutar,hesap_turu, hesap_id, "
                             "doviz_kuru, parite, "
                             "program_id, modul_id, base_record_id, satir_tutari");

        query.SET_VALUE(":fis_id"                , P_FIS_SATIRI->fis_id                 );
        query.SET_VALUE(":makro_id"              , P_FIS_SATIRI->makro_id               );
        query.SET_VALUE(":aciklama"              , P_FIS_SATIRI->satir_aciklama         );
        query.SET_VALUE(":makro_tutari"          , P_FIS_SATIRI->islem_tutari           );
        query.SET_VALUE(":otomatik_odeme_tutari" , P_FIS_SATIRI->otomatik_odeme_tutari  );
        query.SET_VALUE(":order_number"          , P_FIS_SATIRI->order_number           );
        query.SET_VALUE(":temel_tutar"           , P_FIS_SATIRI->islem_tutari           );
        query.SET_VALUE(":hesap_turu"            , hesap_turu                           );
        query.SET_VALUE(":satir_tutari"          , P_FIS_SATIRI->satir_tutari           );
        query.SET_VALUE(":doviz_kuru"            , P_FIS_SATIRI->doviz_kuru             );
        query.SET_VALUE(":parite"                , P_FIS_SATIRI->parite                 );
        query.SET_VALUE(":program_id"            , E9_PROGRAMI                          );
        query.SET_VALUE(":modul_id"              , BANKA_MODULU                         );
        query.SET_VALUE(":base_record_id"        , 0                                    );

        if ( hesap_turu EQ ENUM_BNK_CARI_HESAP ) {
            query.SET_VALUE(":hesap_id", cari_hesap_id);
        }
        else if ( hesap_turu EQ ENUM_BNK_POS_HESABI ) {
            query.SET_VALUE(":hesap_id", pos_id);
        }
        else if ( hesap_turu EQ ENUM_BNK_KREDI_KARTI_HESABI ) {
            query.SET_VALUE(":hesap_id", kredi_karti_id);
        }
        else {
            query.SET_VALUE(":hesap_id", 0);
        }

        row_id = query.INSERT();
    }

    //[ 1  ]

    int program_id     = E9_PROGRAMI;
    int modul_id       = BANKA_MODULU;
    int base_record_id = row_id;

    query.PREPARE_SELECT ( "bnk_makro_fisler","toplam_hesaba_giren,toplam_hesaptan_cikan, muh_fis_id, def_fis_id","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id" , P_FIS_SATIRI->fis_id );
    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    double    toplam_hesaba_giren   = query.VALUE(0).toDouble();
    double    toplam_hesaptan_cikan = query.VALUE(1).toDouble();
    int       muh_fis_id            = query.VALUE(2).toInt();
    int       def_fis_id            = query.VALUE(3).toInt();

    double    ara_toplam_hesaba_giren           = 0.00;
    double    ara_toplam_hesaptan_cikan         = 0.00;

    double    temel_ara_toplam_hesaba_giren     = 0.00;
    double    temel_ara_toplam_hesaptan_cikan   = 0.00;

    int makro_hesap_turu    =  0;
    int hesaba_etkisi       =  0;
    int muh_hesap_id        = -1;
    int makro_yuzdesi       =  0.00;
    int sabit_tutar         =  0.00;
    int fark_satiri_mi      = -1;

    query.PREPARE_SELECT( "bnk_makrolar, bnk_makro_satirlari","hesap_turu, hesaba_etkisi, muh_hesap_id,"
                          "makro_yuzdesi, sabit_tutar, fark_satiri_mi, round_sekli  ",
                          "bnk_makrolar.makro_id = bnk_makro_satirlari.makro_id  AND bnk_makro_satirlari.makro_id = :makro_id");
    query.SET_VALUE     ( ":makro_id", makro_id );

    if ( query.SELECT("hesaba_etkisi ASC, makro_yuzdesi ASC ") EQ 0 ) {
        return 0;
    }

    double  hesaba_giren                = 0.0;
    double  hesaptan_cikan              = 0.0;
    double  temel_hesaba_giren          = 0.0;
    double  temel_hesaptan_cikan        = 0.0;

    int     fark_satiri_id              = -1;
    int     fark_satiri_hesaba_etkisi   = -1;

    double  fark_eklenecek_tutar        = 0.0;
    int     fark_hesabinin_turu         = 0;

    int     fis_satiri_id               = 0;
    double  current_tutar               = 0.00;

    double  bnk_ara_toplam_borc         = 0.00;
    double  bnk_ara_toplam_alacak       = 0.00;


    bool    bnk_fark_satiri_mi          = false;
    int     round_sekli                 = 0;

    int     banka_hesaba_girenin_turu   = 0;
    int     banka_hesaptan_cikanin_turu = 0;

    bool banka_satirlari_eklenecek_mi = false;

    while ( query.NEXT() EQ true ) {

        makro_hesap_turu = query.VALUE( 0 ).toInt();
        hesaba_etkisi    = query.VALUE( 1 ).toInt();
        muh_hesap_id     = query.VALUE( 2 ).toInt();
        makro_yuzdesi    = query.VALUE( 3 ).toDouble();
        sabit_tutar      = query.VALUE( 4 ).toDouble();
        fark_satiri_mi   = query.VALUE( 5 ).toInt();
        round_sekli      = query.VALUE( 6 ).toInt();

        // hesaba giren hesaptan cikan fark satiri

        hesaba_giren             = 0.0;
        hesaptan_cikan           = 0.0;

        if ( hesaba_etkisi EQ ENUM_BNK_BORC_ARTTIR ) {
            if ( sabit_tutar EQ 0 ) {
                                            // MAKRODAKI ROUNDLAMA SEKLINE GORE ISLEM YAPILIYOR
                hesaba_giren              = ROUND_DEGER( round_sekli, islem_tutari * makro_yuzdesi / 100.0 );
                hesaptan_cikan            = 0.00;
                current_tutar             = hesaba_giren;
            }
            else {
                hesaba_giren               = sabit_tutar;
                hesaptan_cikan             = 0.00;
                current_tutar              = hesaba_giren;
            }
        }
        else {
            if ( sabit_tutar EQ 0 ) {
                hesaptan_cikan            = ROUND_DEGER( round_sekli, islem_tutari * makro_yuzdesi / 100.0 );
                hesaba_giren              = 0.00;
                current_tutar             = hesaptan_cikan;
            }
            else {
                hesaptan_cikan            = sabit_tutar;
                hesaba_giren              = 0.00;
                current_tutar             = hesaptan_cikan;
            }
        }

        if ( P_FIS_SATIRI->banka_doviz_id NE DVZ_GET_TEMEL_PARA_BIRIMI_ID() ){
            temel_hesaba_giren      = ROUND( hesaba_giren   * P_FIS_SATIRI->doviz_kuru );
            temel_hesaptan_cikan    = ROUND( hesaptan_cikan * P_FIS_SATIRI->doviz_kuru );
        }
        else {
            temel_hesaba_giren      = hesaba_giren;
            temel_hesaptan_cikan    = hesaptan_cikan;
        }

        switch ( makro_hesap_turu ) {

            case ENUM_BNK_BANKA_HESABI:
            case ENUM_BNK_POS_HESABI:
            case ENUM_BNK_KREDI_KARTI_HESABI: {
                        banka_satirlari_eklenecek_mi = true;

                        bnk_ara_toplam_borc     += hesaba_giren;
                        bnk_ara_toplam_alacak   += hesaptan_cikan;

                        // bir hesaptan diger hesaba olacagi icin hesaplari tutumak zorunda yim
                        // banka ---> kredi karti
                        if ( hesaba_giren > 0.00 ) {
                            banka_hesaba_girenin_turu   = makro_hesap_turu;
                        }
                        if ( hesaptan_cikan > 0.00 ) {
                            banka_hesaptan_cikanin_turu = makro_hesap_turu;
                        }

                        if ( fark_satiri_mi EQ 1 ){
                            bnk_fark_satiri_mi = true;
                        }
                    break;
                }
            case ENUM_BNK_MUHASEBE_HESABI:
            case ENUM_BNK_KASA_HESABI: {

                    MUH_FIS_STRUCT         * MUH_FIS          = new MUH_FIS_STRUCT;
                    MUH_FIS_SATIRI_STRUCT  * MUH_FIS_SATIRI   = new MUH_FIS_SATIRI_STRUCT;

                    MUH_CLEAR_FIS_STRUCT        ( MUH_FIS );
                    MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI );

                    MUH_FIS_BILGILERINI_OKU ( MUH_FIS, muh_fis_id ,USE_DB);

                    MUH_FIS_SATIRI->aciklama             = P_FIS_SATIRI->satir_aciklama;
                    MUH_FIS_SATIRI->borc_tutari          = temel_hesaba_giren;
                    MUH_FIS_SATIRI->alacak_tutari        = temel_hesaptan_cikan;
                    MUH_FIS_SATIRI->fis_id               = muh_fis_id;

                    if ( makro_hesap_turu EQ ENUM_BNK_KASA_HESABI ) {
                        MUH_FIS_SATIRI->hesap_id             = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();
                    }
                    else {
                        MUH_FIS_SATIRI->hesap_id             = muh_hesap_id;
                    }

                    MUH_FIS_SATIRI->base_record_id       = base_record_id;
                    MUH_FIS_SATIRI->program_id           = program_id;
                    MUH_FIS_SATIRI->modul_id             = modul_id;
                    fis_satiri_id = MUH_FIS_SATIRI_EKLE ( MUH_FIS, MUH_FIS_SATIRI ,USE_DB);
                    delete MUH_FIS;
                    delete MUH_FIS_SATIRI;

                break;
             }
            case ENUM_BNK_CARI_HESAP: {

                    CARI_FIS_STRUCT * CARI_FIS    = new CARI_FIS_STRUCT;
                    CARI_CLEAR_FIS_STRUCT        ( CARI_FIS );

                    int temel_para_birim_id = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

                    double cari_hesaba_giren   = temel_hesaba_giren;
                    double cari_hesaptan_cikan = temel_hesaptan_cikan;
                    double cari_doviz_kuru     = P_FIS_SATIRI->doviz_kuru;;
                    if ( P_FIS_SATIRI->banka_doviz_id NE temel_para_birim_id AND cari_para_birimi_id NE temel_para_birim_id ) {
                        cari_hesaba_giren   = ROUND( hesaba_giren * P_FIS_SATIRI->parite );
                        cari_hesaptan_cikan = ROUND( hesaptan_cikan * P_FIS_SATIRI->parite );
                        cari_doviz_kuru = ROUND( P_FIS_SATIRI->doviz_kuru *  ( 1 / P_FIS_SATIRI->parite ) , 4 );
                    }

                    int car_fis_id = CARI_GET_ENT_FIS_ID( program_id, modul_id, P_FIS_SATIRI->fis_id ,USE_DB);

                    if ( car_fis_id EQ 0 ) { // banka hareket fisi inin bir cari hesap yok ise olusturulur

                        CARI_FIS->aciklama              = QString ( QObject::tr ( "Fiş BANKA MODÜLÜ ( MAKRO FİŞİ ) tarafından üretilmiştir. Fiş No = '%1'\n" ) ).arg( fis_no) + P_FIS_SATIRI->fis_aciklama;
                        CARI_FIS->cari_hesap_id         = 0; // banka islem fisi cari hareket fisi olusturur , cari fis satirinda cari hesap id 0 olmalidir
                        CARI_FIS->cari_para_birimi_id   = cari_para_birimi_id;
                        CARI_FIS->makbuz_para_birimi_id = cari_para_birimi_id;
                        CARI_FIS->fis_tarihi            = P_FIS_SATIRI->fis_tarihi;
                        CARI_FIS->modul_id              = modul_id;
                        CARI_FIS->program_id            = program_id;
                        CARI_FIS->fis_turu              = ENUM_CARI_HAREKET_FISI;
                        CARI_FIS->base_fis_id           = def_fis_id;
                        CARI_FIS->muh_fis_id            = muh_fis_id;
                        CARI_FIS->doviz_kuru            = cari_doviz_kuru;
                        CARI_FIS->parite                = P_FIS_SATIRI->parite;

                        car_fis_id = CARI_FIS_EKLE      ( CARI_FIS ,USE_DB);
                    }
                    else {
                        CARI_FIS_BILGILERINI_OKU ( CARI_FIS, car_fis_id ,USE_DB);
                    }


                    CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI    = new CARI_FIS_SATIRI_STRUCT;
                    CARI_CLEAR_FIS_SATIRI_STRUCT ( CARI_FIS_SATIRI );

                    CARI_FIS_SATIRI->aciklama                = P_FIS_SATIRI->satir_aciklama;
                    CARI_FIS_SATIRI->borc_tutari             = cari_hesaba_giren;
                    CARI_FIS_SATIRI->alacak_tutari           = cari_hesaptan_cikan;
                    CARI_FIS_SATIRI->cari_hesap_id           = cari_hesap_id;
                    CARI_FIS_SATIRI->doviz_id                = P_FIS_SATIRI->banka_doviz_id;
                    CARI_FIS_SATIRI->doviz_kuru              = cari_doviz_kuru;
                    CARI_FIS_SATIRI->parite                  = P_FIS_SATIRI->parite;
                    CARI_FIS_SATIRI->doviz_kodu              = P_FIS_SATIRI->banka_doviz_id;
                    CARI_FIS_SATIRI->fis_id                  = car_fis_id;
                    CARI_FIS_SATIRI->program_id              = program_id;
                    CARI_FIS_SATIRI->modul_id                = modul_id;
                    CARI_FIS_SATIRI->base_record_id          = base_record_id;
                    CARI_FIS_SATIRI->doviz_kodu              = DVZ_GET_DOVIZ_KODU( P_FIS_SATIRI->banka_doviz_id );

                    fis_satiri_id = CARI_FIS_SATIRI_EKLE ( CARI_FIS, CARI_FIS_SATIRI ,true, 0, NULL, USE_DB );

                    delete CARI_FIS_SATIRI;
                break;
            }
            default: break;

        }
        if ( fark_satiri_mi EQ 1 ) {
            fark_eklenecek_tutar      = current_tutar;
            fark_satiri_id            = fis_satiri_id;
            fark_satiri_hesaba_etkisi = hesaba_etkisi;
            fark_hesabinin_turu       = makro_hesap_turu;
        }

        ara_toplam_hesaba_giren   = ROUND( hesaba_giren  + ara_toplam_hesaba_giren );
        ara_toplam_hesaptan_cikan = ROUND( hesaptan_cikan + ara_toplam_hesaptan_cikan );

        temel_ara_toplam_hesaba_giren   = ROUND( temel_hesaba_giren +  temel_ara_toplam_hesaba_giren );
        temel_ara_toplam_hesaptan_cikan = ROUND( temel_hesaptan_cikan +  temel_ara_toplam_hesaptan_cikan );
    }

    if ( banka_satirlari_eklenecek_mi EQ true ) {

        BNK_DEFTER_FIS_STRUCT        * DEF_FIS        = new BNK_DEFTER_FIS_STRUCT;
        BNK_DEFTER_FIS_SATIRI_STRUCT * DEF_FIS_SATIRI = new BNK_DEFTER_FIS_SATIRI_STRUCT;

        BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, def_fis_id ,USE_DB);

        DEF_FIS_SATIRI->aciklama            = P_FIS_SATIRI->satir_aciklama;
        DEF_FIS_SATIRI->base_record_id      = base_record_id;
        DEF_FIS_SATIRI->fis_id              = def_fis_id;
        DEF_FIS_SATIRI->hesap_turu          = makro_hesap_turu;
        DEF_FIS_SATIRI->program_id          = program_id;
        DEF_FIS_SATIRI->modul_id            = modul_id;


        if ( bnk_ara_toplam_borc > 0.00  ) {
            if ( otomatik_odeme_tutari > 0.00 ) {
                bnk_ara_toplam_borc     = otomatik_odeme_tutari;
            }

            DEF_FIS_SATIRI->hesaba_giren        = bnk_ara_toplam_borc;
            DEF_FIS_SATIRI->hesaptan_cikan      = 0.00;

            switch ( banka_hesaba_girenin_turu ) {
            case ENUM_BNK_BANKA_HESABI:
                DEF_FIS_SATIRI->hesap_turu  = ENUM_BNK_BANKA_HESABI;
                DEF_FIS_SATIRI->hesap_id    = P_FIS_SATIRI->hesap_no_id;
                break;
            case ENUM_BNK_POS_HESABI:
                DEF_FIS_SATIRI->hesap_turu  = ENUM_BNK_POS_HESABI;
                DEF_FIS_SATIRI->hesap_id    = pos_id;
                break;
            case ENUM_BNK_KREDI_KARTI_HESABI:
                DEF_FIS_SATIRI->hesap_turu  = ENUM_BNK_KREDI_KARTI_HESABI;
                DEF_FIS_SATIRI->hesap_id    = kredi_karti_id;
                break;
                default: break;
            }

            DEF_FIS_SATIRI->doviz_kuru          = P_FIS_SATIRI->doviz_kuru;

            fis_satiri_id = BNK_DEFTER_FIS_SATIRI_EKLE( DEF_FIS, DEF_FIS_SATIRI,0,true,USE_DB );
        }


        if ( bnk_ara_toplam_alacak > 0.00 ) {
            if ( otomatik_odeme_tutari > 0.00 ) {
                bnk_ara_toplam_alacak   = otomatik_odeme_tutari;
            }

            DEF_FIS_SATIRI->hesaba_giren        = 0.00;
            DEF_FIS_SATIRI->hesaptan_cikan      = bnk_ara_toplam_alacak;

            switch ( banka_hesaptan_cikanin_turu ) {
            case ENUM_BNK_BANKA_HESABI:
                DEF_FIS_SATIRI->hesap_turu  = ENUM_BNK_BANKA_HESABI;
                DEF_FIS_SATIRI->hesap_id    = P_FIS_SATIRI->hesap_no_id;
                break;
            case ENUM_BNK_POS_HESABI:
                DEF_FIS_SATIRI->hesap_turu  = ENUM_BNK_POS_HESABI;
                DEF_FIS_SATIRI->hesap_id    = pos_id;
                break;
            case ENUM_BNK_KREDI_KARTI_HESABI:
                DEF_FIS_SATIRI->hesap_turu  = ENUM_BNK_KREDI_KARTI_HESABI;
                DEF_FIS_SATIRI->hesap_id    = kredi_karti_id;
                break;
                default: break;
            }

            DEF_FIS_SATIRI->doviz_kuru          = P_FIS_SATIRI->doviz_kuru;

            fis_satiri_id = BNK_DEFTER_FIS_SATIRI_EKLE( DEF_FIS, DEF_FIS_SATIRI,0,true,USE_DB );
        }

        delete DEF_FIS;
        delete DEF_FIS_SATIRI;

        if ( bnk_ara_toplam_alacak EQ 0.00 ) {
            current_tutar = bnk_ara_toplam_borc;
            hesaba_etkisi = ENUM_BNK_BORC_ARTTIR;
        }
        else {
            current_tutar = bnk_ara_toplam_alacak;
            hesaba_etkisi = ENUM_BNK_ALACAK_ARTTIR;
        }

        if ( bnk_fark_satiri_mi EQ 1 ) {
            fark_eklenecek_tutar      = current_tutar;
            fark_satiri_id            = fis_satiri_id;
            fark_satiri_hesaba_etkisi = hesaba_etkisi;
            fark_hesabinin_turu       = ENUM_BNK_BANKA_HESABI;
        }
        toplam_hesaba_giren        = ara_toplam_hesaba_giren;
    }

    SQL_QUERY update_query ( USE_DB );

    if ( fark_satiri_id NE -1 ) {
        if ( otomatik_odeme_tutari NE 0.00 ) {

            if ( fark_hesabinin_turu EQ ENUM_BNK_MUHASEBE_HESABI OR fark_hesabinin_turu EQ ENUM_BNK_KASA_HESABI OR fark_hesabinin_turu EQ ENUM_BNK_POS_KOMISYON_HESABI ) {

                MUH_FIS_STRUCT         * MUH_FIS          = new MUH_FIS_STRUCT;
                MUH_FIS_SATIRI_STRUCT  * MUH_FIS_SATIRI   = new MUH_FIS_SATIRI_STRUCT;

                MUH_FIS_SATIRINI_OKU( MUH_FIS_SATIRI, fark_satiri_id ,USE_DB);
                MUH_FIS_BILGILERINI_OKU( MUH_FIS , MUH_FIS_SATIRI->fis_id ,USE_DB);

                if (fark_satiri_hesaba_etkisi EQ ENUM_BNK_BORC_ARTTIR) {

                   double toplam = ROUND( fark_eklenecek_tutar + otomatik_odeme_tutari - ara_toplam_hesaba_giren );

                   MUH_FIS_SATIRI->borc_tutari = toplam;
                   MUH_FIS_SATIRI->alacak_tutari   = 0.00;
                   toplam_hesaptan_cikan   = otomatik_odeme_tutari;

                }
                else {
                    double toplam = ROUND( fark_eklenecek_tutar +  otomatik_odeme_tutari -  ara_toplam_hesaba_giren );
                    MUH_FIS_SATIRI->borc_tutari = 0.00;
                    MUH_FIS_SATIRI->alacak_tutari   = toplam;
                    toplam_hesaba_giren = otomatik_odeme_tutari;
                }

                MUH_FIS_SATIRINI_GUNCELLE( MUH_FIS, MUH_FIS_SATIRI, fark_satiri_id ,USE_DB);

                delete MUH_FIS;
                delete MUH_FIS_SATIRI;
            }
            else {

                BNK_DEFTER_FIS_STRUCT        * DEF_FIS        = new BNK_DEFTER_FIS_STRUCT;
                BNK_DEFTER_FIS_SATIRI_STRUCT * DEF_FIS_SATIRI = new BNK_DEFTER_FIS_SATIRI_STRUCT;

                CLEAR_BNK_DEFTER_FIS_STRUCT( DEF_FIS );

                BNK_DEFTER_FIS_SATIR_BILGILERINI_OKU( DEF_FIS_SATIRI, fark_satiri_id ,USE_DB);
                BNK_DEFTER_FIS_BILGILERINI_OKU( DEF_FIS, DEF_FIS_SATIRI->fis_id ,USE_DB);

                double toplam = 0.00;
                if (fark_satiri_hesaba_etkisi EQ ENUM_BNK_BORC_ARTTIR) {
                    if ( otomatik_odeme_tutari NE 0.00 ) {
                        toplam = ROUND( fark_eklenecek_tutar + otomatik_odeme_tutari - ara_toplam_hesaba_giren );
                    }
                    else {
                        toplam = fark_eklenecek_tutar;
                    }
                    DEF_FIS_SATIRI->hesaptan_cikan  =  0.0;
                    DEF_FIS_SATIRI->hesaba_giren    = toplam;
                    toplam_hesaptan_cikan           = otomatik_odeme_tutari;
                }
                else {
                    if ( otomatik_odeme_tutari NE 0.00 ) {
                        toplam = ROUND( fark_eklenecek_tutar + otomatik_odeme_tutari - ara_toplam_hesaba_giren );
                    }
                    else {
                        toplam = fark_eklenecek_tutar;
                    }
                    DEF_FIS_SATIRI->hesaptan_cikan  = toplam;
                    DEF_FIS_SATIRI->hesaba_giren    = 0.0;
                    toplam_hesaba_giren = otomatik_odeme_tutari;
                }

                BNK_DEFTER_FIS_SATIRINI_GUNCELLE( DEF_FIS, DEF_FIS_SATIRI, fark_satiri_id ,USE_DB);

                delete DEF_FIS;
                delete DEF_FIS_SATIRI;
            }

        }
    }

    if ( base_fis_satiri_eklenecek EQ true ) {

        update_query.PREPARE_UPDATE (  "bnk_makro_fisler","fis_id","toplam_hesaba_giren ,toplam_hesaptan_cikan","fis_id = :fis_id");
        update_query.SET_VALUE      ( ":toplam_hesaba_giren"   , toplam_hesaba_giren);
        update_query.SET_VALUE      ( ":toplam_hesaptan_cikan" , toplam_hesaptan_cikan);
        update_query.SET_VALUE      ( ":fis_id"                , P_FIS_SATIRI->fis_id);
        update_query.UPDATE         ();

        //[ /1 ]

        toplam_makro_tutari = ROUND ( toplam_makro_tutari + islem_tutari );

        update_query.PREPARE_UPDATE("bnk_makro_fisler","fis_id","toplam_makro_tutari","fis_id = :fis_id");
        update_query.SET_VALUE(":toplam_makro_tutari" , toplam_makro_tutari );
        update_query.SET_VALUE(":fis_id"              , P_FIS_SATIRI->fis_id);
        update_query.UPDATE();
    }

    return row_id;

}

/*****************************************************************************************************
                               CLEAR_MAKRO_FIS_SATIRI
*****************************************************************************************************/

int  MAKRO_ADD_FIS ( MAKRO_FIS_STRUCT * P_FIS, int p_fis_id , bool base_fis_eklenecek  ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    BNK_DEFTER_FIS_STRUCT * DEF_FIS = new BNK_DEFTER_FIS_STRUCT;

    CLEAR_BNK_DEFTER_FIS_STRUCT ( DEF_FIS );
    // [ 1 ]
    if ( P_FIS->fis_no EQ 0) {
        P_FIS->fis_no = BNK_SIRADAKI_MAKRO_FIS_NO_AL ( P_FIS->fis_tarihi );
    }

    SQL_QUERY insert_query(USE_DB);

    int fis_id = p_fis_id;

    if ( base_fis_eklenecek EQ true  ) {
        insert_query.PREPARE_INSERT("bnk_makro_fisler","fis_id","fis_no,fis_tarihi,                                                                "
                                    "hesap_no_id, aciklama,  program_id, modul_id, base_fis_id, "
                                    "muh_fis_id, def_fis_id, toplam_hesaba_giren, toplam_hesaptan_cikan ");

        insert_query.SET_VALUE ( ":fis_no"                  ,  P_FIS->fis_no               );
        insert_query.SET_VALUE ( ":fis_tarihi"              ,  P_FIS->fis_tarihi           );
        insert_query.SET_VALUE ( ":hesap_no_id"             ,  P_FIS->hesap_no_id          );
        insert_query.SET_VALUE ( ":aciklama"                ,  P_FIS->aciklama             );
        insert_query.SET_VALUE ( ":program_id"              ,  P_FIS->program_id           );
        insert_query.SET_VALUE ( ":modul_id"                ,  P_FIS->modul_id             );
        insert_query.SET_VALUE ( ":base_fis_id"             ,  P_FIS->base_fis_id          );
        insert_query.SET_VALUE ( ":muh_fis_id"              ,  P_FIS->muh_fis_id           );
        insert_query.SET_VALUE ( ":def_fis_id"              ,  P_FIS->def_fis_id           );
        insert_query.SET_VALUE ( ":toplam_hesaba_giren"     ,  0                           );
        insert_query.SET_VALUE ( ":toplam_hesaptan_cikan"   ,  0                           );

        fis_id = insert_query.INSERT();
    }

    if ( P_FIS->base_fis_id EQ  0 ) {
        P_FIS->base_fis_id = fis_id;
    }


    if (  P_FIS->def_fis_id EQ 0 ) {

        if ( P_FIS->modul_id EQ BANKA_MODULU ) {
            DEF_FIS->aciklama           = QString ( QObject::tr ( " Fiş BANKA MODÜLÜ ( MAKRO FİŞİ ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_FIS->fis_no ).toInt() );
            DEF_FIS->aciklama.append(P_FIS->aciklama);
        }
        else {
            DEF_FIS->aciklama = P_FIS->aciklama;
        }

        DEF_FIS->fis_tarihi                         = P_FIS->fis_tarihi;
        DEF_FIS->fis_turu                           = ENUM_BANKA_HAREKET_FISI;
        DEF_FIS->modul_id                           = P_FIS->modul_id;
        DEF_FIS->program_id                         = P_FIS->program_id;
        DEF_FIS->base_fis_id                        = fis_id;
        DEF_FIS->hesap_no_id                        = P_FIS->hesap_no_id;
        DEF_FIS->para_birim_id                      = P_FIS->para_birim_id;
        DEF_FIS->muh_fis_id                         = P_FIS->muh_fis_id;
        DEF_FIS->base_fis_turu                      = ENUM_BANKA_MAKRO_FISI;
        DEF_FIS->odm_emr_hesabi_etkilenecek_mi    = 1; // etkilenecek

        P_FIS->def_fis_id = BNK_DEFTER_FIS_EKLE ( DEF_FIS ,USE_DB);
    }


    MUH_FIS_STRUCT * MUH_FIS    = new MUH_FIS_STRUCT;

    if ( P_FIS->muh_fis_id EQ 0 ) {

        if ( P_FIS->muh_fis_id EQ 0 ) {
            MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

            MUH_FIS->aciklama           = QString ( QObject::tr ( " Fiş BANKA MODÜLÜ ( MAKRO FİŞİ ) tarafından üretilmiştir. Fiş No = %1\n" ) ).arg( QVariant ( P_FIS->fis_no ).toInt() );
            MUH_FIS->aciklama.append    ( P_FIS->aciklama );
            MUH_FIS->fis_tarihi         = P_FIS->fis_tarihi;
            MUH_FIS->fis_turu           = ENUM_MAHSUP_FISI;
            MUH_FIS->modul_id           = P_FIS->modul_id;
            MUH_FIS->program_id         = P_FIS->program_id;
            MUH_FIS->base_fis_id        = P_FIS->def_fis_id;

            P_FIS->muh_fis_id = MUH_FIS_EKLE ( MUH_FIS , USE_DB);
        }
    }

    // [ 1 ]

    SQL_QUERY query(DB);

    query.PREPARE_UPDATE( "bnk_defter_fisler","fis_id","muh_fis_id","fis_id = :fis_id " );
    query.SET_VALUE     ( ":muh_fis_id",   P_FIS->muh_fis_id );
    query.SET_VALUE     ( ":fis_id",       P_FIS->def_fis_id );
    query.UPDATE();
    query.PREPARE_UPDATE( "bnk_makro_fisler","fis_id","def_fis_id , muh_fis_id","fis_id = :fis_id " );
    query.SET_VALUE     ( ":def_fis_id",   P_FIS->def_fis_id );
    query.SET_VALUE     ( ":muh_fis_id",   P_FIS->muh_fis_id );
    query.SET_VALUE     ( ":fis_id",       fis_id );
    query.UPDATE();

    delete MUH_FIS;
    delete DEF_FIS;

    return fis_id;
}

/*****************************************************************************************************
                               ROUNDA_SEKLI
*****************************************************************************************************/

double ROUND_DEGER ( int round_sekli, double p_deger )
{
    if ( round_sekli EQ ENUM_ROUND ) {
        return ROUND( p_deger );
    }
    else if ( round_sekli EQ ENUM_ROUND_UP ) {
        return ROUND_UP( p_deger );
    }
    else if ( round_sekli EQ ENUM_ROUND_DOWN ) {
        return ROUND_DOWN( p_deger );
    }
    return p_deger;
}

/*****************************************************************************************************
                               CLEAR_MAKRO_FIS_SATIRI
*****************************************************************************************************/

void  CLEAR_MAKRO_FIS( MAKRO_FIS_STRUCT * P_FIS )
{
    P_FIS->fis_tarihi.clear();
    P_FIS->fis_no           = 0;
    P_FIS->def_fis_id       = 0;
    P_FIS->program_id       = 0;
    P_FIS->modul_id         = 0;
    P_FIS->base_fis_id      = 0;
    P_FIS->hesap_no_id      = 0;
    P_FIS->para_birim_id    = 0;
    P_FIS->muh_fis_id       = 0;
    P_FIS->aciklama.clear();

}

/*****************************************************************************************************
                               CLEAR_MAKRO_FIS_SATIRI
*****************************************************************************************************/

void  CLEAR_MAKRO_FIS_SATIRI( MAKRO_SATIR_STRUCT * P_FIS_SATIRI )
{

    P_FIS_SATIRI->fis_id                   = 0;
    P_FIS_SATIRI->islem_tutari             = 0.00;
    P_FIS_SATIRI->otomatik_odeme_tutari    = 0.00;
    P_FIS_SATIRI->order_number             = 0;
    P_FIS_SATIRI->order_number             = 0;
    P_FIS_SATIRI->makro_id                 = 0;
    P_FIS_SATIRI->search_edit_hesap.clear();
    P_FIS_SATIRI->doviz_kuru               = 0.00;
    P_FIS_SATIRI->parite                   = 0.00;
    P_FIS_SATIRI->base_record_id           = 0;
    P_FIS_SATIRI->fis_tarihi.clear();
    P_FIS_SATIRI->satir_aciklama.clear();
    P_FIS_SATIRI->banka_doviz_id           = 0;
    P_FIS_SATIRI->hesap_no_id              = 0;
    P_FIS_SATIRI->hesap_turu               = 0;
    P_FIS_SATIRI->fis_aciklama.clear();

}

/*****************************************************************************************************
                            BNK_MAKRO_FISI::SATIR_DUZENLE
*****************************************************************************************************/

void BNK_MAKRO_FISI::SATIR_DUZENLE( bool p_kur, bool p_parite  )
{
    CLOSE_ALL_EVENTS();
    for ( int row = 0 ; row < m_ui->tablewidget_fis_satirlari->rowCount() ; row++) {
        QCommaEdit  * commaedit_kur    = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget( row, KUR_COLUMN    );
        QCommaEdit  * commaedit_parite = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget( row, PARITE_COLUMN );

        commaedit_kur->setEnabled( p_kur );
        commaedit_parite->setEnabled( p_parite );

    }
    OPEN_ALL_EVENTS();
}
