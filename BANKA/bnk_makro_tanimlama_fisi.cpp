#include "bnk_makro_tanimlama_fisi_class.h"
#include "bnk_makro_tanimlama_fisi_open.h"
#include "adak_sql.h"
#include "e9_log.h"
#include "BANKA/bnk_console_utils.h"
#include "BANKA/bnk_gui_utils.h"
 
#include "ui_bnk_makro_tanimlama_fisi.h"
#include "ui_bnk_makro_fisi.h"
#include "bnk_makro_secimi_open.h"
#include "muh_hesap_arama_open.h"
#include <QStandardItemModel>
#include "muh_console_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"

extern ADAK_SQL *                 DB;

#define ROW_ID_COLUMN              0
#define FIS_ID_COLUMN              1
#define ORDER_COLUMN               2
#define MUH_HESAP_ID_COLUMN        3
#define ETKILENECEK_HESAP_TURU     4
#define HESABA_ETKISI_COLUMN       5
#define YUZDE                      6
#define TUTAR                      7
#define ACIKLAMA                   8
#define ROUND_SEKLI                9
#define ETKILENECEK_HESAP_KODU     10
#define ETKILENECEK_HESAP_ADI      11

/************************************************************************/
/*                       F_BNK_MAKRO_TANIMLA                            */
/************************************************************************/

void OPEN_BNK_MAKRO_TANIMLA( int record_id, QWidget * parent )
{
    BNK_MAKRO_TANIMLAMA_FISI * F = new BNK_MAKRO_TANIMLAMA_FISI( record_id, parent );
     
    F->EXEC( FULL_SCREEN );
}

/***********************************************************************/
/*            BNK_MAKRO_TANIMLAMA_FISI::BNK_MAKRO_TANIMLAMA_FISI                 */
/***********************************************************************/

BNK_MAKRO_TANIMLAMA_FISI::BNK_MAKRO_TANIMLAMA_FISI(int record_id, QWidget * parent) :FIS_KERNEL(parent)
{
    setupUi ( this );
    m_record_id = record_id;

    START_FIS_KERNEL( this, DB );
}

/******************************************************************
               BNK_MAKRO_TANIMLAMA_FISI::SETUP_FORM();
******************************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::SETUP_FORM ()
{
    SET_HELP_PAGE  ( "banka-islemleri_makro-tanimlama" );
    SET_PAGE_TITLE ( tr ( "BNK - MAKRO TANIMLAMA" ) );

    REGISTER_BUTTONS_WIDGET ( navigation_button_widget);
    REGISTER_TABLE_WIDGET   ( table_widget_makrolar, 12 );

    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN  );
    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN       ( 4 );

    SET_FIRST_FOCUS_WIDGET ( line_edit_makro_adi );

    SET_FIS_ORDER_DATABASE ("bnk_makro_satirlari", "order_number", "makro_satiri_id" );

    SET_TABLE_ROW_WIDGETS ( ETKILENECEK_HESAP_TURU  ,WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( HESABA_ETKISI_COLUMN    ,WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( TUTAR                   ,WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( YUZDE                   ,WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA                ,WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( ROUND_SEKLI             ,WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( ETKILENECEK_HESAP_KODU  ,WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( ETKILENECEK_HESAP_ADI   ,WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( MUH_HESAP_ID_COLUMN     ,WIDGET_LINE_EDIT   ) ;


    table_widget_makrolar->setHorizontalHeaderLabels(QStringList()
                                                     <<"row_id"<<"fis_id"<<"order_column"<<"muh_hes_id"
                                                     <<tr("Hesap Türü"   )
                                                     <<tr("Hesaba Etkisi")
                                                     <<tr("İşlem Tutarının\nYüzdesi(%)"            )
                                                     <<tr("İşlem Tutarından Bağımsız\nSabit Tutar")
                                                     <<tr("Açıklama")
                                                     <<tr("Yuvarlama\nŞekli")
                                                     <<tr("Hesap Kodu"   )
                                                     <<tr("Hesap Adı "   )
                                                     <<tr("")<<tr("") );


    table_widget_makrolar->setColumnWidth ( ETKILENECEK_HESAP_TURU  , 181 );
    table_widget_makrolar->setColumnWidth ( HESABA_ETKISI_COLUMN    , 160 );
    table_widget_makrolar->setColumnWidth ( YUZDE                   , 135 );
    table_widget_makrolar->setColumnWidth ( TUTAR                   , 185 );
    table_widget_makrolar->setColumnWidth ( ACIKLAMA                , 200 );
    table_widget_makrolar->setColumnWidth ( ROUND_SEKLI             , 104 );
    table_widget_makrolar->setColumnWidth ( ETKILENECEK_HESAP_KODU  , 140 );
    table_widget_makrolar->setColumnWidth ( ETKILENECEK_HESAP_ADI   , 239 );

    SET_SETTING_NAME    ( "BANKA_MAKRO_TANIMLAMA_FISI" );

    table_widget_makrolar->setSelectionMode ( QAbstractItemView::NoSelection );
    table_widget_makrolar->setFocus( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET();

    widget_kusurat_satir->hide();

    line_edit_makro_adi->setMaxLength( 128 );
    limited_text_edit_aciklama->SET_MAX_STRING_LENGTH( 512 );
    AUTOSTART_FIS_KERNEL( m_record_id );

}

/******************************************************************/
/*          BNK_MAKRO_TANIMLAMA_FISI::CLEAR_FORM_MEMBERS();       */
/******************************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::CLEAR_FORM_MEMBERS()
{

}

/*****************************************************/
/*          BNK_MAKRO_TANIMLAMA_FISI::NEW_FIS_RECORD      */
/*****************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::NEW_FIS_RECORD()
{
    if (widget_kusurat_satir->isVisible() EQ false) {
        combo_box_hesaba_etkisi->setEditText("");
        combo_box_kusurat_hesabi->setCurrentIndex(-1);
        search_edit_hesap_kodu->SET_TEXT("");
        line_edit_hesap_ismi->clear();

        push_button_kusurat_farki->setText("Fark Satırı İşlemi Kaldır");
        widget_kusurat_satir->show();

    }
    else {
        push_button_kusurat_farki->setText("Fark Satırını Seç");
        widget_kusurat_satir->hide();
    }
    push_button_kusurat_farki->setText ( tr ( "Fark Satırını Seç" ) );
    widget_kusurat_satir->hide();
}

/***********************************************************/
/*           BNK_MAKRO_TANIMLAMA_FISI::GET_FIS_RECORD           */
/***********************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::GET_FIS_RECORD ( int record_id )
{
    SQL_QUERY   query(DB);
    SQL_QUERY   sql_query(DB);

    query.PREPARE_SELECT( "bnk_makrolar","makro_adi,aciklama","makro_id  = :makro_id " );

    query.SET_VALUE(":makro_id",record_id);

    if ( query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    query.NEXT();

    line_edit_makro_adi ->setText(query.VALUE(0).toString());
    limited_text_edit_aciklama->setText(query.VALUE(1).toString());


    query.PREPARE_SELECT ( "bnk_makro_satirlari","makro_satiri_id,order_number,hesap_turu,muh_hesap_id,sabit_tutar,makro_yuzdesi, "
                           "hesaba_etkisi, fark_satiri_mi, aciklama, round_sekli ","makro_id = :makro_id ");

    query.SET_VALUE      ( ":makro_id", record_id );

    if ( query.SELECT("order_number") EQ 0 ) {
        return ADAK_OK;
    }

    int current_row  ;
    int table_view_select_row = 0;

    while ( query.NEXT() EQ true ) {
        current_row = ADD_NEW_LINE ();

        QComboBox  *  line_combo_box_hesap_turu     = ( QComboBox  * ) table_widget_makrolar->cellWidget ( current_row, ETKILENECEK_HESAP_TURU   );
        QComboBox  *  line_combo_box_hesaba_etkisi  = ( QComboBox  * ) table_widget_makrolar->cellWidget ( current_row, HESABA_ETKISI_COLUMN     );
        QComboBox  *  combo_box_round_sekli         = ( QComboBox  * ) table_widget_makrolar->cellWidget ( current_row, ROUND_SEKLI              );
        QCommaEdit *  commaEdit_tutar               = ( QCommaEdit * ) table_widget_makrolar->cellWidget ( current_row, TUTAR                    );
        QCommaEdit *  commaEdit_yuzde               = ( QCommaEdit * ) table_widget_makrolar->cellWidget ( current_row, YUZDE                    );
        QLineEdit  *  lineedit_muh_hesap_id         = ( QLineEdit  * ) table_widget_makrolar->cellWidget ( current_row, MUH_HESAP_ID_COLUMN      );
        QLineEdit  *  aciklama                      = ( QLineEdit  * ) table_widget_makrolar->cellWidget ( current_row, ACIKLAMA                 );

        line_combo_box_hesap_turu->setCurrentIndex    ( line_combo_box_hesap_turu->findText( BNK_GET_HESAP_TURU_STRING(query.VALUE(2).toInt())));

        line_combo_box_hesaba_etkisi->setCurrentIndex ( line_combo_box_hesaba_etkisi->findText( BNK_GET_HESAP_ETKISI_STRING(query.VALUE(6).toInt())));

        combo_box_round_sekli->setCurrentIndex( combo_box_round_sekli->findText( BNK_GET_ROUND_SEKLI_STRING( query.VALUE("round_sekli").toInt())));

        if ( line_combo_box_hesaba_etkisi->currentText() EQ "AZALT" ) {
            line_combo_box_hesaba_etkisi->setStyleSheet("background-color:red;");
        }

        if ( query.VALUE(4).toDouble() NE 0.00 ) {
            commaEdit_tutar->SET_DOUBLE(query.VALUE(4).toDouble());
        }

        if ( query.VALUE(5).toDouble() NE 0.00 ) {
            commaEdit_yuzde->SET_DOUBLE(query.VALUE(5).toDouble());
        }

        aciklama->setText(query.VALUE(8).toString());

        QTableWidgetItem * new_item;

        QString row_id    = query.VALUE("makro_satiri_id").toString();
        QString order_number = query.VALUE("order_number").toString();

        new_item = new QTableWidgetItem(tr("%1").arg(row_id));
        table_widget_makrolar->setItem(current_row, ROW_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(record_id));
        table_widget_makrolar->setItem(current_row, FIS_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(order_number));
        table_widget_makrolar->setItem(current_row, ORDER_COLUMN, new_item);

        int muh_hesap_id = query.VALUE(3).toInt();

        lineedit_muh_hesap_id->setText(QVariant(muh_hesap_id).toString());

        QSearchEdit * search_edit = NULL;
        QLineEdit  *   line_edit  = NULL;

        if (muh_hesap_id > 0) {
            QString tam_hesap_kodu,hesap_ismi;

            MUH_GET_HESAP_KODU_HESAP_ISMI(muh_hesap_id,tam_hesap_kodu,hesap_ismi);


            search_edit = ( QSearchEdit * ) table_widget_makrolar->cellWidget( current_row, ETKILENECEK_HESAP_KODU );
            search_edit->SET_TEXT(tam_hesap_kodu);

            line_edit   = ( QLineEdit * ) table_widget_makrolar->cellWidget( current_row, ETKILENECEK_HESAP_ADI );
            line_edit->setText(hesap_ismi);

            search_edit->setEnabled(true);
            line_edit->setEnabled(true);

        }

        int fark_satiri_mi = query.VALUE(7).toInt();
        if ( fark_satiri_mi EQ 1) {
            table_view_select_row = current_row;
            combo_box_hesaba_etkisi->setEditText( line_combo_box_hesaba_etkisi->currentText() );
            if (search_edit NE NULL AND line_edit NE NULL) {
                search_edit_hesap_kodu->SET_TEXT(search_edit->GET_TEXT());
                line_edit_hesap_ismi->setText(line_edit->text());
            }

            push_button_kusurat_farki->setText("Fark Satırı İşlemini Kaldır");
            widget_kusurat_satir->show();
        }
    }

    REFRESH_KUSURAT_COMBO_BOX();

    QTableView * table_view = (QTableView *)combo_box_kusurat_hesabi->view();
    table_view->selectRow(table_view_select_row);

    return ADAK_OK;
}

/******************************************************************/
/*           BNK_MAKRO_TANIMLAMA_FISI::SET_LINE_DEFAULTS               */
/******************************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QComboBox   *  combo_box_hesap_turu     = ( QComboBox   * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_TURU  );
    QComboBox   *  combo_box_hesaba_etkisi  = ( QComboBox   * ) table_widget_makrolar->cellWidget ( row_number, HESABA_ETKISI_COLUMN    );
    QComboBox   *  combo_box_round_sekli    = ( QComboBox   * ) table_widget_makrolar->cellWidget ( row_number, ROUND_SEKLI             );
    QSearchEdit *  search_edit              = ( QSearchEdit * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_KODU  );
    QLineEdit   *  line_edit                = ( QLineEdit   * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_ADI   );
    QLineEdit   *  aciklama                 = ( QLineEdit   * ) table_widget_makrolar->cellWidget ( row_number, ACIKLAMA                 );

    BNK_FILL_COMBOBOX_HESAP_TURU    ( combo_box_hesap_turu );
    combo_box_hesap_turu->setCurrentIndex    ( -1 );

    BNK_FILL_COMBO_BOX_HESABA_ETKISI ( combo_box_hesaba_etkisi );
    combo_box_hesaba_etkisi->setCurrentIndex ( -1 );

    QStringList round_sekli;
    round_sekli << tr( "Yuvarla" ) << tr( "Yukarı Yuvarla") << tr( "Aşağı Yuvarla" );

    combo_box_round_sekli->addItems( round_sekli );

    combo_box_round_sekli->setCurrentIndex( 0 );


    search_edit->setEnabled ( false );
    line_edit->setReadOnly  ( true );
    line_edit->setEnabled   ( false );
    aciklama->setMaxLength(250);

}

/******************************************************************/
/*          BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_FORM_VAR               */
/******************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( object EQ push_button_kusurat_farki ) {
        if (widget_kusurat_satir->isVisible() EQ false) {
            combo_box_hesaba_etkisi->setEditText("");
            combo_box_kusurat_hesabi->setCurrentIndex(-1);
            search_edit_hesap_kodu->SET_TEXT("");
            line_edit_hesap_ismi->clear();

            push_button_kusurat_farki->setText("Fark Satırı İşlemi Kaldır");
            widget_kusurat_satir->show();

        }
        else {
            push_button_kusurat_farki->setText("Fark Satırını Seç");
            widget_kusurat_satir->hide();
        }
    }

    if ( object EQ combo_box_kusurat_hesabi ) {
        int row = combo_box_kusurat_hesabi->view()->currentIndex().row();

        QStandardItemModel * model = static_cast<QStandardItemModel *> (combo_box_kusurat_hesabi->model());

        combo_box_kusurat_hesabi->setEditText(model->item(row,0)->text());
        combo_box_hesaba_etkisi->setEditText( model->item(row,1)->text() );
        search_edit_hesap_kodu->SET_TEXT(model->item(row,4)->text());
        line_edit_hesap_ismi->setText(model->item(row,5)->text());
    }



    return ADAK_OK;
}

/**************************************************************/
/*          BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_FORM_EMPTY         */
/**************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( line_edit_makro_adi->text().isEmpty() EQ true ) {
      MSG_WARNING(  tr("İşlem adı boş bırakılamaz"), line_edit_makro_adi );

      return ADAK_FAIL;
    }

    return ADAK_OK;
}

/************************************************************/
/*          BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_RECORD_ADD       */
/************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_RECORD_ADD()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "bnk_makrolar","makro_adi","makro_adi = :makro_adi");
    sql_query.SET_VALUE      ( ":makro_adi", line_edit_makro_adi->text() );

    if ( sql_query.SELECT() NE 0 ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/***********************************************************/
/*           BNK_MAKRO_TANIMLAMA_FISI::ADD_FIS_RECORD           */
/***********************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::ADD_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );


    sql_query.PREPARE_INSERT ( "bnk_makrolar","makro_id","makro_adi, aciklama ");

    sql_query.SET_VALUE ( ":makro_adi"                         , line_edit_makro_adi->text()                            );
    sql_query.SET_VALUE ( ":aciklama"                          , limited_text_edit_aciklama->document()->toPlainText()  );

    QString log_detaylari = QObject::tr("Banka İşlem Tanımlama , ");
    log_detaylari.append(line_edit_makro_adi->text());

    int makro_id = sql_query.INSERT();

    E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_ISLEMLER , LOG_ISLEM_ADD , log_detaylari);

    return makro_id;
}

/***********************************************************/
/*          BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_RECORD_UPDATE   */
/***********************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_RECORD_UPDATE(int record_id)
{
    Q_UNUSED ( record_id );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "bnk_makrolar","makro_adi","makro_adi=:makro_adi AND makro_id != :makro_id");

    sql_query.SET_VALUE      ( ":makro_adi", line_edit_makro_adi->text() );
    sql_query.SET_VALUE      ( ":makro_id" , record_id );

    if ( sql_query.SELECT() NE 0 ) {
        MSG_ERROR("Aynı isimde makro tanımlanmış.Lütfen makronun ismini değiştirin.",line_edit_makro_adi);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/*************************************************************/
/*      BNK_MAKRO_TANIMLAMA_FISI::UPDATE_FIS_RECORD               */
/*************************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::UPDATE_FIS_RECORD(int record_id)
{
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_UPDATE ( "bnk_makrolar","makro_id", "makro_adi,aciklama",
                               "makro_id = :makro_id");

    sql_query.SET_VALUE ( ":makro_adi"                     , line_edit_makro_adi->text()                            );
    sql_query.SET_VALUE ( ":aciklama"                      , limited_text_edit_aciklama->document()->toPlainText()  );
    sql_query.SET_VALUE ( ":makro_id"                      , record_id                                              );

    sql_query.UPDATE();

    QString log_detaylari = QObject::tr("Banka İşlem Tanımlama , ");
    log_detaylari.append(line_edit_makro_adi->text());

    E9_LOG_KAYDI_EKLE (BANKA_MODULU , LOG_BNK_ISLEMLER , LOG_ISLEM_UPDATE , log_detaylari);

    sql_query.PREPARE_UPDATE("bnk_makro_satirlari","makro_id","fark_satiri_mi","makro_id = :makro_id");
    sql_query.SET_VALUE(":fark_satiri_mi",0);
    sql_query.SET_VALUE(":makro_id",record_id);
    sql_query.UPDATE();

    if ( widget_kusurat_satir->isVisible() EQ true) {
        if (combo_box_kusurat_hesabi->currentText().isEmpty() EQ false) {
            int satir_index = combo_box_kusurat_hesabi->view()->currentIndex().row();
            QTableWidgetItem * item_satir_id = table_widget_makrolar->item( satir_index, ROW_ID_COLUMN );

            if ( item_satir_id EQ NULL ) {
                return;
            }

            int makro_satiri_id = item_satir_id->text().toInt();

            sql_query.PREPARE_UPDATE("bnk_makro_satirlari","makro_satiri_id","fark_satiri_mi","makro_satiri_id = :makro_satiri_id");
            sql_query.SET_VALUE(":fark_satiri_mi",1);
            sql_query.SET_VALUE(":makro_satiri_id",makro_satiri_id);
            sql_query.UPDATE();
        }
    }
}

/*************************************************************/
/*         BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_RECORD_DELETE      */
/*************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    Q_UNUSED ( record_id );

    return ADAK_OK;
}

/******************************************************/
/*         BNK_MAKRO_TANIMLAMA_FISI::DELETE_FIS_RECORD     */
/******************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::DELETE_FIS_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "bnk_makrolar","makro_adi","makro_id = :makro_id" );
    sql_query.SET_VALUE      ( ":makro_id" , record_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString makro_adi = sql_query.VALUE(0).toString();

    sql_query.PREPARE_DELETE ( "bnk_makro_satirlari","makro_id = :makro_id" );
    sql_query.SET_VALUE      ( ":makro_id"  , record_id );
    sql_query.DELETE         ();

    sql_query.PREPARE_DELETE ( "bnk_makrolar","makro_id = :makro_id" );
    sql_query.SET_VALUE      ( ":makro_id" ,record_id );
    sql_query.DELETE         ();

    QString log_detaylari = QObject::tr("Banka İşlem Tanımlama , ");
    log_detaylari.append ( makro_adi );

    E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_ISLEMLER , LOG_ISLEM_DELETE , log_detaylari );

}

/*******************************************************/
/*         BNK_MAKRO_TANIMLAMA_FISI::SELECT_FIS_RECORD     */
/*******************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::SELECT_FIS_RECORD()
{
    return OPEN_BNK_MAKRO_SEC ( this );
}

/*******************************************************/
/*          BNK_MAKRO_TANIMLAMA_FISI::FIND_FIS_RECORD       */
/*******************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY   sql_query ( DB );

    sql_query.PREPARE_SELECT ( " bnk_makrolar","makro_id","makro_adi = :makro_adi" );
    sql_query.SET_VALUE      ( ":makro_adi"   , line_edit_makro_adi->text());

    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/*****************************************************************/
/*           BNK_MAKRO_TANIMLAMA_FISI::FIND_FIRST_FIS_RECORD          */
/*****************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT( "bnk_makrolar","makro_id");

    if ( sql_query.SELECT("makro_adi",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/*****************************************************************/
/*           BNK_MAKRO_TANIMLAMA_FISI::FIND_LAST_FIS_RECORD           */
/*****************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT( "bnk_makrolar","makro_id");

    if ( sql_query.SELECT("makro_adi DESC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/****************************************************************/
/*           BNK_MAKRO_TANIMLAMA_FISI::FIND_PREV_FIS_RECORD          */
/****************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY  sql_query(DB);

    sql_query.PREPARE_SELECT        ( "bnk_makrolar" , "makro_id","makro_adi < :makro_adi");

    sql_query.SET_VALUE             ( ":makro_adi"  , line_edit_makro_adi->text() );

    if ( sql_query.SELECT("makro_adi DESC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/***************************************************************/
/*           BNK_MAKRO_TANIMLAMA_FISI::FIND_NEXT_FIS_RECORD         */
/***************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY  sql_query(DB);

    sql_query.PREPARE_SELECT        ( "bnk_makrolar" , "makro_id","makro_adi > :makro_adi");

    sql_query.SET_VALUE             ( ":makro_adi"  , line_edit_makro_adi->text() );

    if ( sql_query.SELECT("makro_adi ASC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/*****************************************************/
/*         BNK_MAKRO_TANIMLAMA_FISI::CHECK_LINE_VAR       */
/*****************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{
    QComboBox   *  combo_box_etkilenecek_hesap  = ( QComboBox   * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_TURU );
    QSearchEdit *  search_edit                  = ( QSearchEdit * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_KODU );
    QLineEdit   *  line_edit                    = ( QLineEdit   * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_ADI  );
    QCommaEdit  *  comma_edit_tutar             = ( QCommaEdit  * ) table_widget_makrolar->cellWidget ( row_number, TUTAR                  );
    QCommaEdit  *  comma_edit_yuzde             = ( QCommaEdit  * ) table_widget_makrolar->cellWidget ( row_number, YUZDE                  );

    if ( object EQ combo_box_etkilenecek_hesap ) {
        if ( combo_box_etkilenecek_hesap->currentText() NE tr("MUHASEBE") ) {
            search_edit->setEnabled ( false );
            line_edit->setEnabled   ( false );
            search_edit->SET_TEXT("");
            line_edit->clear();
        }
        else {
            search_edit->setEnabled(true);
            line_edit->setEnabled(true);
        }
    }
    else if (object EQ comma_edit_tutar) {
        if (comma_edit_yuzde->GET_DOUBLE() NE 0.0 ) {
            comma_edit_yuzde->clear();
        }
    }
    else if (object EQ comma_edit_yuzde) {
        if (comma_edit_tutar->GET_DOUBLE() NE 0.0 ) {
            comma_edit_tutar->clear();
        }
    }
    else if (object EQ search_edit) {

        MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

        MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

        if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , search_edit->GET_TEXT()) EQ 0 ) {
            MSG_WARNING( tr("Aradığınız banka muhasebe hesap kodu bulunamadı"), search_edit );

            return ADAK_FAIL_UNDO;
        }

        if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {

            MSG_WARNING( tr("Aradığınız muhasebe hesabı tali hesap değil"), search_edit );

            return ADAK_FAIL_UNDO;
        }

        QLineEdit * lineedit_muh_hesap_id = (QLineEdit * ) table_widget_makrolar->cellWidget(row_number,MUH_HESAP_ID_COLUMN);
        lineedit_muh_hesap_id->setText(QVariant(MUHASEBE_HESABI->hesap_id).toString());

        line_edit->setText(MUHASEBE_HESABI->hesap_ismi);
    }

    return ADAK_OK;
}

/**********************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::CHECK_LINE_EMPTY   */
/**********************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_LINE_EMPTY(int row_number)
{
    QSearchEdit *   search_edit                  = ( QSearchEdit * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_KODU   );
    QCommaEdit  *   comma_edit_tutar             = ( QCommaEdit  * ) table_widget_makrolar->cellWidget ( row_number, TUTAR                    );
    QCommaEdit  *   comma_edit_yuzde             = ( QCommaEdit  * ) table_widget_makrolar->cellWidget ( row_number, YUZDE                    );
    QComboBox   *   combo_box_etkilenecek_hesap  = ( QComboBox   * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_TURU   );
    QComboBox   *   combo_box_hesaba_etkisi      = ( QComboBox   * ) table_widget_makrolar->cellWidget ( row_number, HESABA_ETKISI_COLUMN     );

    if ( combo_box_etkilenecek_hesap->currentIndex() EQ -1) {
        MSG_WARNING( tr("Hesap türü boş bırakılamaz."), combo_box_etkilenecek_hesap );

        return ADAK_FAIL;
    }

    if ( combo_box_hesaba_etkisi->currentIndex() EQ -1 ) {
        MSG_WARNING( tr("Hesaba etkisi boş bırakılamaz."), combo_box_hesaba_etkisi );

        return ADAK_FAIL;
    }

    if ( comma_edit_tutar->GET_DOUBLE() EQ 0.0 AND comma_edit_yuzde->GET_DOUBLE() EQ 0.0 ) {
        MSG_WARNING( tr("Yüzde yada tutar boş bırakılamaz."), comma_edit_yuzde);

        return ADAK_FAIL;
    }

    if ( combo_box_etkilenecek_hesap->currentText() EQ tr("MUHASEBE") ) {
        if(search_edit->GET_TEXT().isEmpty() EQ true) {
            MSG_WARNING( tr("Hesap kodu boş bırakılmaz."), search_edit_hesap_kodu );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}


/*********************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::CHECK_ADD_LINE    */
/*********************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_ADD_LINE(int record_id, int row_number)
{
    Q_UNUSED ( record_id  );
    Q_UNUSED ( row_number );

    return ADAK_OK;
}

/***********************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::ADD_LINE            */
/***********************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::ADD_LINE(int record_id, int row_number)
{
    REFRESH_KUSURAT_COMBO_BOX ();

    int order_number            = table_widget_makrolar->item( row_number , ORDER_COLUMN )->text().toInt();

    QComboBox   * comboBox_etkilenecek_hesap_turu   = ( QComboBox  * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_TURU );
    QComboBox   * comboBox_hesaba_etkisi            = ( QComboBox  * ) table_widget_makrolar->cellWidget ( row_number, HESABA_ETKISI_COLUMN   );
    QComboBox   * comboBox_round_sekli              = ( QComboBox  * ) table_widget_makrolar->cellWidget ( row_number, ROUND_SEKLI            );
    QCommaEdit  * commaEdit_tutar                   = ( QCommaEdit * ) table_widget_makrolar->cellWidget ( row_number, TUTAR                  );
    QCommaEdit  * commaEdit_yuzde                   = ( QCommaEdit * ) table_widget_makrolar->cellWidget( row_number, YUZDE );
    QLineEdit   * lineedit_muh_hesap_id             = ( QLineEdit  * ) table_widget_makrolar->cellWidget ( row_number, MUH_HESAP_ID_COLUMN    );
    QLineEdit  *  aciklama                          = ( QLineEdit  * ) table_widget_makrolar->cellWidget ( row_number, ACIKLAMA                 );

    int     hesap_turu      = BNK_GET_HESAP_TURU_ENUM(comboBox_etkilenecek_hesap_turu->currentText());
    int     hesaba_etkisi   = BNK_GET_HESABA_ETKISI_ENUM(comboBox_hesaba_etkisi->currentText());
    double  tutar           = commaEdit_tutar->GET_DOUBLE();
    double  yuzde           = commaEdit_yuzde->GET_DOUBLE();

    int     round_sekli     = BNK_GET_ROUND_SEKLI_ENUM( comboBox_round_sekli->currentText() );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_INSERT ( "bnk_makro_satirlari","makro_satiri_id","makro_id, order_number, hesap_turu, muh_hesap_id, sabit_tutar, makro_yuzdesi, "
                               "hesaba_etkisi,aciklama, round_sekli ");

    sql_query.SET_VALUE ( ":makro_id",            record_id                               );
    sql_query.SET_VALUE ( ":order_number",        order_number                            );

    sql_query.SET_VALUE ( ":hesap_turu",          hesap_turu                              );
    sql_query.SET_VALUE ( ":muh_hesap_id",        lineedit_muh_hesap_id->text().toInt()   );
    sql_query.SET_VALUE ( ":sabit_tutar",         tutar                                   );
    sql_query.SET_VALUE ( ":makro_yuzdesi",       yuzde                                   );
    sql_query.SET_VALUE ( ":hesaba_etkisi",       hesaba_etkisi                           );
    sql_query.SET_VALUE ( ":aciklama",            aciklama->text()                        );
    sql_query.SET_VALUE ( ":round_sekli",         round_sekli                             );

    int row_id = sql_query.INSERT();

    table_widget_makrolar->setItem ( row_number, ROW_ID_COLUMN, new QTableWidgetItem ( ( QVariant ( row_id ) ).toString() ) );
}

/*******************************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::CHECK_UPDATE_LINE           */
/*******************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id  );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/*******************************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::UPDATE_LINE                 */
/*******************************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );

    int row_id            = table_widget_makrolar->item ( row_number , ROW_ID_COLUMN )->text().toInt();

    QComboBox  * comboBox_etkilenecek_hesap_turu    = ( QComboBox  * ) table_widget_makrolar->cellWidget ( row_number, ETKILENECEK_HESAP_TURU   );
    QComboBox  * comboBox_hesaba_etkisi             = ( QComboBox  * ) table_widget_makrolar->cellWidget ( row_number, HESABA_ETKISI_COLUMN     );
    QComboBox  * comboBox_round_sekli               = ( QComboBox  * ) table_widget_makrolar->cellWidget ( row_number, ROUND_SEKLI              );
    QCommaEdit * commaEdit_tutar                    = ( QCommaEdit * ) table_widget_makrolar->cellWidget ( row_number, TUTAR                    );
    QCommaEdit * commaEdit_yuzde                    = ( QCommaEdit * ) table_widget_makrolar->cellWidget ( row_number, YUZDE                    );
    QLineEdit  * lineedit_muh_hesap_id              = ( QLineEdit  * ) table_widget_makrolar->cellWidget ( row_number, MUH_HESAP_ID_COLUMN      );
    QLineEdit  * aciklama                           = ( QLineEdit  * ) table_widget_makrolar->cellWidget ( row_number, ACIKLAMA                 );

    int     hesap_turu      = BNK_GET_HESAP_TURU_ENUM         ( comboBox_etkilenecek_hesap_turu->currentText() );
    double  tutar           = commaEdit_tutar->GET_DOUBLE ();
    double  yuzde           = commaEdit_yuzde->GET_DOUBLE ();
    int     hesaba_etkisi   = BNK_GET_HESABA_ETKISI_ENUM      ( comboBox_hesaba_etkisi->currentText() );
    int     round_sekli     = BNK_GET_ROUND_SEKLI_ENUM( comboBox_round_sekli->currentText() );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_UPDATE ( "bnk_makro_satirlari","makro_satiri_id","hesap_turu , muh_hesap_id, "
                               "sabit_tutar , makro_yuzdesi, "
                               "hesaba_etkisi,aciklama, round_sekli","makro_satiri_id = :makro_satiri_id");

    sql_query.SET_VALUE ( ":hesap_turu",          hesap_turu                             );
    sql_query.SET_VALUE ( ":muh_hesap_id",        lineedit_muh_hesap_id->text().toInt()  );
    sql_query.SET_VALUE ( ":sabit_tutar",         tutar                                  );
    sql_query.SET_VALUE ( ":makro_yuzdesi",       yuzde                                  );
    sql_query.SET_VALUE ( ":hesaba_etkisi",       hesaba_etkisi                          );
    sql_query.SET_VALUE ( ":makro_satiri_id",     row_id                                 );
    sql_query.SET_VALUE ( ":aciklama",            aciklama->text()                       );
    sql_query.SET_VALUE ( ":round_sekli",         round_sekli                            );

    sql_query.UPDATE();
}

/*******************************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::CHECK_DELETE_LINE           */
/*******************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id  );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/*******************************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::DELETE_LINE                 */
/*******************************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::DELETE_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);

    SQL_QUERY sql_query ( DB );

    int row_id = table_widget_makrolar->item(row_number, ROW_ID_COLUMN)->text().toInt();

    sql_query.PREPARE_DELETE ( "bnk_makro_satirlari","makro_satiri_id = :makro_satiri_id");
    sql_query.SET_VALUE      ( ":makro_satiri_id", row_id );

    sql_query.DELETE();
}

/*****************************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::LOCK_FIS_RECORD           */
/*****************************************************************/

int BNK_MAKRO_TANIMLAMA_FISI::LOCK_FIS_RECORD(int record_id)
{
    return DB->LOCK_ROW( "makro_id","bnk_makrolar", QString ( "makro_id = %1" ).arg ( record_id ));
}

/*****************************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::UNLOCK_FIS_RECORD         */
/*****************************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::UNLOCK_FIS_RECORD(int record_id)
{
    DB->UNLOCK_ROW( "makro_id","bnk_makrolar", QString ( "makro_id = %1" ).arg ( record_id ));
}

/*****************************************************************/
/*                BNK_MAKRO_TANIMLAMA_FISI::LINE_SEARCH_EDIT_CLICKED  */
/*****************************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::LINE_SEARCH_EDIT_CLICKED(int row_number, QWidget * search_edit, QLineEdit * line_edit)
{
    Q_UNUSED ( search_edit );
    Q_UNUSED ( row_number  );

    int hesap_id = OPEN_MUH_HESAP_ARAMA(line_edit->text(),this,1);

    line_edit->setText ( MUH_GET_HESAP_KODU(hesap_id) );
}

/*****************************************************************
             BNK_MAKRO_TANIMLAMA_FISI::REFRESH_KUSURAT_COMBO_BOX
******************************************************************/

void BNK_MAKRO_TANIMLAMA_FISI::REFRESH_KUSURAT_COMBO_BOX()
{
    QStandardItemModel * item_model = new QStandardItemModel;
    item_model->setColumnCount(6);
    item_model->setHorizontalHeaderLabels( QStringList() <<tr("Hesap Türü"                            )
                                                         <<tr("Hesaba Etkisi"                         )
                                                         <<tr("İşlem Tutarının Yüzdesi(%)"            )
                                                         <<tr("İşlem Tutarından Bağımsız\nSabit Tutar")
                                                         <<tr("Hesap Kodu"                            )
                                                         <<tr("Hesap Adı "                            ));

    for ( int i = 0 ; i < table_widget_makrolar->rowCount() ; i++ ) {
        item_model->insertRow(i);

        QComboBox   *   combo_box_etkilenecek_hesap_turu = ( QComboBox   * ) table_widget_makrolar->cellWidget(i,ETKILENECEK_HESAP_TURU);
        QComboBox   *   combo_box_hesaba_etkisi          = ( QComboBox   * ) table_widget_makrolar->cellWidget(i,HESABA_ETKISI_COLUMN);
        QCommaEdit  *   comma_edit_tutar                 = ( QCommaEdit  * ) table_widget_makrolar->cellWidget(i,TUTAR);
        QCommaEdit  *   comma_edit_yuzde                 = ( QCommaEdit  * ) table_widget_makrolar->cellWidget(i,YUZDE);
        QSearchEdit *   search_edit_etkilenecek_hesap    = ( QSearchEdit * ) table_widget_makrolar->cellWidget(i,ETKILENECEK_HESAP_KODU);
        QLineEdit   *   etkilenecek_hesap_adi            = ( QLineEdit   * ) table_widget_makrolar->cellWidget(i,ETKILENECEK_HESAP_ADI);


        item_model->setItem( i, 0, new QStandardItem( combo_box_etkilenecek_hesap_turu->currentText()) );
        item_model->setItem( i, 1, new QStandardItem( combo_box_hesaba_etkisi->currentText())          );
        item_model->setItem( i, 2, new QStandardItem( comma_edit_yuzde->GET_TEXT())                    );
        item_model->setItem( i, 3, new QStandardItem( comma_edit_tutar->GET_TEXT())                    );
        item_model->setItem( i, 4, new QStandardItem( search_edit_etkilenecek_hesap->GET_TEXT())       );
        item_model->setItem( i, 5, new QStandardItem( etkilenecek_hesap_adi->text())                   );
    }

    QTableView * table_view = new QTableView ();
    combo_box_kusurat_hesabi->setView(table_view);

    table_view->setMinimumWidth(table_widget_makrolar->size().width());
    table_view->setMinimumHeight(100);

    table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

    combo_box_kusurat_hesabi->setModel(item_model);

    table_view->resizeColumnsToContents();
    table_view->resizeRowsToContents();

    table_view->setColumnWidth ( 0  , 181);
    table_view->setColumnWidth ( 1  , 160);
    table_view->setColumnWidth ( 2  , 195);
    table_view->setColumnWidth ( 3  , 179);
    table_view->setColumnWidth ( 4  , 210);
    table_view->setColumnWidth ( 5  , 319);
}
