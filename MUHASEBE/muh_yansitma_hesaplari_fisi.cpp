#include "adak_sql.h"
#include "muh_yansitma_hesaplari_fisi_class.h"
#include "muh_yansitma_hesaplari_fisi_open.h"
#include "muh_console_utils.h"
 
#include "ui_muh_yansitma_hesaplari_fisi.h"
#include "muh_hesap_arama_open.h"


extern ADAK_SQL *           DB;

#define ROW_ID_COLUMN                         0
#define ORDER_COLUMN                          1
#define GIDER_HESABI_ID_COLUMN                2
#define YANSITMA_HESABI_ID_COLUMN             3
#define BILANCO_GELIR_HESABI_ID_COLUMN        4

#define GIDER_HESABI_KODU_COLUMN              5
#define GIDER_HESABI_ISMI_COLUMN              6
#define BILANCO_GELIR_HESABI_KODU_COLUMN      7
#define BILANCO_GELIR_HESABI_ISMI_COLUMN      8
#define YANSITMA_HESABI_KODU_COLUMN           9
#define YANSITMA_HESABI_ISMI_COLUMN           10




/**************************************************************************************
                   OPEN_MUH_YANSITMA_HESAPLARI_FISI
***************************************************************************************/

void OPEN_MUH_YANSITMA_HESAPLARI_FISI (  QWidget * parent )
{
    MUH_YANSITMA_HESAPLARI_FISI * F = new MUH_YANSITMA_HESAPLARI_FISI ( parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::MUH_YANSITMA_HESAPLARI_FISI
***************************************************************************************/

MUH_YANSITMA_HESAPLARI_FISI::MUH_YANSITMA_HESAPLARI_FISI(QWidget * parent) :FIS_KERNEL(parent)
{
    setupUi          ( this );
    START_FIS_KERNEL ( this, DB );
 }

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::SETUP_FORM
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::SETUP_FORM()
{
    SET_HELP_PAGE    ( "genel-muhasebe_yansitma-hesaplari" );
    SET_PAGE_TITLE   ( tr ( "MUH - YANSITMA HESAPLARI" ) );
    SET_SETTING_NAME ( "MUH_YANSITMA_HESAPLARI_FISI" );

    SET_SINGLE_RECORD_ID ( -1);

    REGISTER_BUTTONS_WIDGET ( kernel_buttons_widget );

    REGISTER_TABLE_WIDGET ( tablewidget_yansitma_hesaplari, 11 );

    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_COLUMN  );
    SET_ROW_ID_COLUMN_POSITION  ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN ( 5 );

    SET_FIS_ORDER_DATABASE ("muh_yansitma_hesaplari", "order_number", "kayit_id" );

    SET_TABLE_ROW_WIDGETS ( GIDER_HESABI_ID_COLUMN          , WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS ( YANSITMA_HESABI_ID_COLUMN       , WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS ( BILANCO_GELIR_HESABI_ID_COLUMN  , WIDGET_LINE_EDIT );

    SET_TABLE_ROW_WIDGETS ( GIDER_HESABI_KODU_COLUMN        , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( GIDER_HESABI_ISMI_COLUMN        , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( YANSITMA_HESABI_KODU_COLUMN     , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( YANSITMA_HESABI_ISMI_COLUMN     , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( BILANCO_GELIR_HESABI_KODU_COLUMN, WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( BILANCO_GELIR_HESABI_ISMI_COLUMN, WIDGET_LINE_EDIT   );


    SET_FIRST_FOCUS_WIDGET( tablewidget_yansitma_hesaplari );

    tablewidget_yansitma_hesaplari->setHorizontalHeaderLabels (QStringList() << "row_id" << "order column" << "gider hes id"
                                                               << "yansitma hes id" << "bilanco hes id"
                                                               << tr("Gider Hesabı") << tr("")
                                                               << tr("Bilanço / Gelir Hesabı") << tr("")
                                                               << tr("Yansıtma Hesabı") << tr("")
                                                               << tr("") << tr("") );

    tablewidget_yansitma_hesaplari->setColumnWidth (GIDER_HESABI_KODU_COLUMN           , 160);
    tablewidget_yansitma_hesaplari->setColumnWidth (GIDER_HESABI_ISMI_COLUMN           , 250);
    tablewidget_yansitma_hesaplari->setColumnWidth (YANSITMA_HESABI_KODU_COLUMN        , 150);
    tablewidget_yansitma_hesaplari->setColumnWidth (YANSITMA_HESABI_ISMI_COLUMN        , 250);
    tablewidget_yansitma_hesaplari->setColumnWidth (BILANCO_GELIR_HESABI_KODU_COLUMN   , 150);
    tablewidget_yansitma_hesaplari->setColumnWidth (BILANCO_GELIR_HESABI_ISMI_COLUMN   , 240);


    tablewidget_yansitma_hesaplari->setSelectionMode ( QAbstractItemView::NoSelection );
    tablewidget_yansitma_hesaplari->setFocus ( Qt::OtherFocusReason );
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::CLEAR_FORM_MEMBERS()
{

}


/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::NEW_FIS_RECORD
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::NEW_FIS_RECORD()
{

}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::GET_FIS_RECORD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::GET_FIS_RECORD ( int record_id )
{
    SQL_QUERY sql_query(DB);
    Q_UNUSED (record_id);

    sql_query.PREPARE_SELECT("muh_yansitma_hesaplari" , "gider_hesabi_id , yansitma_hesabi_id , bilanco_gelir_hesabi_id ,"
                             "kayit_id");

    if ( sql_query.SELECT("order_number ASC") EQ 0 ) {
        return ADAK_OK;
    }

    QString hesap_kodu;
    QString hesap_ismi;
    QTableWidgetItem * item;

    while ( sql_query.NEXT() EQ true ) {

        int current_row = ADD_NEW_LINE ();

        QSearchEdit * searchedit_gider_hesap_kodu    = ( QSearchEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( current_row, GIDER_HESABI_KODU_COLUMN   );
        QLineEdit   * lineedit_gider_hesap_ismi      = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( current_row, GIDER_HESABI_ISMI_COLUMN   );
        QLineEdit   * lineedit_gider_hesabi_id       = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( current_row, GIDER_HESABI_ID_COLUMN     );

        QSearchEdit * searchedit_yansitma_hesabi_kodu       = ( QSearchEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( current_row, YANSITMA_HESABI_KODU_COLUMN   );
        QLineEdit   * lineedit_yansitma_hesabi_ismi         = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( current_row, YANSITMA_HESABI_ISMI_COLUMN   );
        QLineEdit   * lineedit_yansitma_hesabi_id           = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( current_row, YANSITMA_HESABI_ID_COLUMN     );

        QSearchEdit * searchedit_bilanco_gelir_hesabi_kodu     = ( QSearchEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( current_row, BILANCO_GELIR_HESABI_KODU_COLUMN );
        QLineEdit   * lineedit_bilanco_gelir_hesabi_ismi       = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( current_row, BILANCO_GELIR_HESABI_ISMI_COLUMN );
        QLineEdit   * lineedit_bilanco_gelir_hesabi_id         = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( current_row, BILANCO_GELIR_HESABI_ID_COLUMN   );

        MUH_GET_HESAP_KODU_HESAP_ISMI                   ( sql_query.VALUE(0).toInt() , hesap_kodu , hesap_ismi );

        searchedit_gider_hesap_kodu->SET_TEXT        ( hesap_kodu );
        lineedit_gider_hesap_ismi->setText           ( hesap_ismi );
        lineedit_gider_hesabi_id->setText            ( sql_query.VALUE(0).toString() );
        lineedit_gider_hesap_ismi->setCursorPosition ( 0 );

        MUH_GET_HESAP_KODU_HESAP_ISMI                   ( sql_query.VALUE(1).toInt() , hesap_kodu , hesap_ismi);

        searchedit_yansitma_hesabi_kodu->SET_TEXT           ( hesap_kodu );
        lineedit_yansitma_hesabi_ismi->setText              ( hesap_ismi );
        lineedit_yansitma_hesabi_id->setText                ( sql_query.VALUE(2).toString() );
        lineedit_yansitma_hesabi_ismi->setCursorPosition    ( 0 );

        MUH_GET_HESAP_KODU_HESAP_ISMI                   ( sql_query.VALUE(2).toInt() , hesap_kodu , hesap_ismi);

        lineedit_bilanco_gelir_hesabi_ismi->setCursorPosition  ( 0 );
        searchedit_bilanco_gelir_hesabi_kodu->SET_TEXT         ( hesap_kodu );
        lineedit_bilanco_gelir_hesabi_ismi->setText            ( hesap_ismi );
        lineedit_bilanco_gelir_hesabi_ismi->setCursorPosition  ( 0 );
        lineedit_bilanco_gelir_hesabi_id->setText              ( sql_query.VALUE(2).toString() );

        item = new QTableWidgetItem                     ( sql_query.VALUE(3).toString() );
        tablewidget_yansitma_hesaplari->setItem         ( current_row, ROW_ID_COLUMN, item );
    }


    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QLineEdit * gider_hesap_ismi =  ( QLineEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, GIDER_HESABI_ISMI_COLUMN   );
    QLineEdit * yansitma_hesabi_ismi   =  ( QLineEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, YANSITMA_HESABI_ISMI_COLUMN   );
    QLineEdit * bilanco_gelir_hesabi_ismi =  ( QLineEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, BILANCO_GELIR_HESABI_ISMI_COLUMN );

    gider_hesap_ismi->setReadOnly ( true );
    yansitma_hesabi_ismi->setReadOnly   ( true );
    bilanco_gelir_hesabi_ismi->setReadOnly ( true );

    gider_hesap_ismi->setMaxLength(60);
    yansitma_hesabi_ismi->setMaxLength(60);
    bilanco_gelir_hesabi_ismi->setMaxLength(60);
}



/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{

    Q_UNUSED(object);
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_RECORD_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::ADD_FIS_RECORD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::ADD_FIS_RECORD ()
{
    return -1;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::UPDATE_FIS_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    Q_UNUSED ( record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::DELETE_FIS_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::SELECT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::FIND_FIS_RECORD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::FIND_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::FIND_FIRST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::FIND_LAST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
             MUH_YANSITMA_HESAPLARI_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::FIND_PREV_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::FIND_NEXT_FIS_RECORD()
{
    return 0;
}



/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_LINE_VAR
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QSearchEdit *   searchedit_gider_hesap_kodu         = ( QSearchEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, GIDER_HESABI_KODU_COLUMN    );
    QSearchEdit *   searchedit_yansitma_hesabi_kodu     = ( QSearchEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, YANSITMA_HESABI_KODU_COLUMN    );
    QSearchEdit *   searchedit_bilanco_gelir_hesabi_kodu = ( QSearchEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, BILANCO_GELIR_HESABI_KODU_COLUMN  );
    QLineEdit   *   lineedit_gider_hesap_ismi           = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, GIDER_HESABI_ISMI_COLUMN    );
    QLineEdit   *   lineedit_yansitma_hesabi_ismi       = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, YANSITMA_HESABI_ISMI_COLUMN    );
    QLineEdit   *   lineedit_bilanco_gelir_hesabi_ismi  = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, BILANCO_GELIR_HESABI_ISMI_COLUMN  );
    QLineEdit   *   lineedit_gider_hesabi_id            = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, GIDER_HESABI_ID_COLUMN      );
    QLineEdit   *   lineedit_yansitma_hesabi_id         = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, YANSITMA_HESABI_ID_COLUMN      );
    QLineEdit   *   lineedit_bilanco_gelir_hesabi_id           = ( QLineEdit   * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, BILANCO_GELIR_HESABI_ID_COLUMN    );

    SQL_QUERY sql_query ( DB );


    if ( object EQ  searchedit_gider_hesap_kodu ) {

        if ( searchedit_gider_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_gider_hesabi_id->clear();
            lineedit_gider_hesap_ismi->clear();
            return ADAK_OK;
        }

        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id,hesap_ismi,tali_hesap" , "tam_hesap_kodu = :tam_hesap_kodu");

        sql_query.SET_VALUE(":tam_hesap_kodu" , searchedit_gider_hesap_kodu->GET_TEXT());

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr ("Girdiğiniz gider hesabı bulunamadı."), searchedit_gider_hesap_kodu );

            return ADAK_FAIL_UNDO;

        }
        sql_query.NEXT();

        if ( sql_query.VALUE("tali_hesap").toInt() EQ 0 ) {
            MSG_WARNING(  tr ("Girdiğiniz muhasebe hesabı tali değil. Tali hesap girmelisiniz"), searchedit_yansitma_hesabi_kodu);

            return ADAK_FAIL_UNDO;
        }

        lineedit_gider_hesabi_id->setText(sql_query.VALUE(0).toString());
        lineedit_gider_hesap_ismi->setText(sql_query.VALUE(1).toString());

    }
    if ( object EQ  searchedit_yansitma_hesabi_kodu ) {

        if ( searchedit_yansitma_hesabi_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_yansitma_hesabi_id->clear();
            lineedit_yansitma_hesabi_ismi->clear();
            return ADAK_OK;
        }
        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id, hesap_ismi, tali_hesap",
                                 "tam_hesap_kodu = :tam_hesap_kodu");

        sql_query.SET_VALUE(":tam_hesap_kodu" , searchedit_yansitma_hesabi_kodu->GET_TEXT());

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr ("Girdiğiniz muhasebe hesabı bulunamadı."), searchedit_yansitma_hesabi_kodu );

            return ADAK_FAIL_UNDO;
        }

        sql_query.NEXT();

        if ( sql_query.VALUE("tali_hesap").toInt() EQ 0 ) {
            MSG_WARNING(  tr ("Girdiğiniz muhasebe hesabı tali değil. Yansıtma ve bilanço-gelir tablosu hesaplarına sadece tali hesaplar girilebilir."), searchedit_yansitma_hesabi_kodu);

            return ADAK_FAIL_UNDO;
        }

        lineedit_yansitma_hesabi_id->setText(sql_query.VALUE(0).toString());
        lineedit_yansitma_hesabi_ismi->setText(sql_query.VALUE(1).toString());

    }

    if ( object EQ  searchedit_bilanco_gelir_hesabi_kodu ) {

        if ( searchedit_bilanco_gelir_hesabi_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_bilanco_gelir_hesabi_id->clear();
            lineedit_bilanco_gelir_hesabi_ismi->clear();
            return ADAK_OK;
        }

        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id, hesap_ismi, tali_hesap",
                                 "tam_hesap_kodu = :tam_hesap_kodu");
        sql_query.SET_VALUE(":tam_hesap_kodu" , searchedit_bilanco_gelir_hesabi_kodu->GET_TEXT());

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr ("Girdiğiniz bilanço-gelir tablosu hesabı bulunamadı."), searchedit_bilanco_gelir_hesabi_kodu );

            return ADAK_FAIL_UNDO;
        }
        sql_query.NEXT();

        if ( sql_query.VALUE("tali_hesap").toInt() EQ 0 ) {
            MSG_WARNING(  tr ("Girdiğiniz muhasebe hesabı tali değil. Yansıtma ve bilanço - gelir tablosu hesaplarına sadece tali hesaplar girilebilir."), searchedit_bilanco_gelir_hesabi_kodu );

            return ADAK_FAIL_UNDO;
        }

        lineedit_bilanco_gelir_hesabi_id->setText   ( sql_query.VALUE(0).toString());
        lineedit_bilanco_gelir_hesabi_ismi->setText ( sql_query.VALUE(1).toString());

    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QSearchEdit * searchedit_gider_hesap_kodu = ( QSearchEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, GIDER_HESABI_KODU_COLUMN   );
    QSearchEdit * searchedit_yansitma_hesabi_kodu    = ( QSearchEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, YANSITMA_HESABI_KODU_COLUMN   );
    QSearchEdit * searchedit_bilanco_gelir_hesabi_kodu  = ( QSearchEdit * ) tablewidget_yansitma_hesaplari->cellWidget ( row_number, BILANCO_GELIR_HESABI_KODU_COLUMN );

    if ( searchedit_gider_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(  tr ("Gider hesabını girmelisiniz."), searchedit_gider_hesap_kodu );

        return ADAK_FAIL;
    }

    if ( searchedit_yansitma_hesabi_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(  tr ("Yansıtma hesabını girmelisiniz."), searchedit_yansitma_hesabi_kodu );

        return ADAK_FAIL;
    }

    if ( searchedit_bilanco_gelir_hesabi_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(  tr ("Bilanço - gelir tablosu hesabını girmelisiniz."), searchedit_bilanco_gelir_hesabi_kodu );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_ADD_LINE
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::ADD_LINE
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::ADD_LINE ( int record_id, int row_number )
{
    Q_UNUSED (record_id);

    QLineEdit * lineedit_gider_hesabi_id      =  ( QLineEdit * ) tablewidget_yansitma_hesaplari->cellWidget(row_number,GIDER_HESABI_ID_COLUMN);
    QLineEdit * lineedit_yansitma_hesabi_id   =  ( QLineEdit * ) tablewidget_yansitma_hesaplari->cellWidget(row_number,YANSITMA_HESABI_ID_COLUMN);
    QLineEdit * lineedit_bilanco_gelir_hesabi_id  =  ( QLineEdit * ) tablewidget_yansitma_hesaplari->cellWidget(row_number,BILANCO_GELIR_HESABI_ID_COLUMN);

    SQL_QUERY insert_query(DB);

    insert_query.PREPARE_INSERT("muh_yansitma_hesaplari" , "kayit_id" ,
                                "gider_hesabi_id,"
                                "yansitma_hesabi_id , bilanco_gelir_hesabi_id , order_number");

    insert_query.SET_VALUE(":gider_hesabi_id"           , QVariant ( lineedit_gider_hesabi_id->text() ).toInt());
    insert_query.SET_VALUE(":yansitma_hesabi_id"        , QVariant ( lineedit_yansitma_hesabi_id->text()).toInt());
    insert_query.SET_VALUE(":bilanco_gelir_hesabi_id"   , QVariant ( lineedit_bilanco_gelir_hesabi_id->text()).toInt());
    insert_query.SET_VALUE(":order_number"               , tablewidget_yansitma_hesaplari->item(row_number,ORDER_COLUMN)->text().toInt());

    int row_id = insert_query.INSERT();

    QTableWidgetItem * item = new QTableWidgetItem (QVariant(row_id).toString());
    tablewidget_yansitma_hesaplari->setItem(row_number , ROW_ID_COLUMN , item);

}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::UPDATE_LINE
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED (record_id);

    QLineEdit * lineedit_gider_hesabi_id      =  ( QLineEdit * ) tablewidget_yansitma_hesaplari->cellWidget(row_number,GIDER_HESABI_ID_COLUMN);
    QLineEdit * lineedit_yansitma_hesabi_id          =  ( QLineEdit * ) tablewidget_yansitma_hesaplari->cellWidget(row_number,YANSITMA_HESABI_ID_COLUMN);
    QLineEdit * lineedit_bilanco_gelir_hesabi_id        =  ( QLineEdit * ) tablewidget_yansitma_hesaplari->cellWidget(row_number,BILANCO_GELIR_HESABI_ID_COLUMN);

    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_UPDATE("muh_yansitma_hesaplari" , "kayit_id" , "gider_hesabi_id ,"
                             "yansitma_hesabi_id  , bilanco_gelir_hesabi_id" , "kayit_id = :kayit_id");


    sql_query.SET_VALUE(":gider_hesabi_id"           , QVariant(lineedit_gider_hesabi_id->text()).toInt());
    sql_query.SET_VALUE(":yansitma_hesabi_id"        , QVariant(lineedit_yansitma_hesabi_id->text()).toInt());
    sql_query.SET_VALUE(":bilanco_gelir_hesabi_id"   , QVariant(lineedit_bilanco_gelir_hesabi_id->text()).toInt());
    sql_query.SET_VALUE(":kayit_id"                  , tablewidget_yansitma_hesaplari->item(row_number,ROW_ID_COLUMN)->text().toInt());

    sql_query.UPDATE();
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::DELETE_LINE
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );

    SQL_QUERY delete_query(DB);

    delete_query.PREPARE_DELETE("muh_yansitma_hesaplari", "kayit_id = :kayit_id");
    delete_query.SET_VALUE(":kayit_id" , tablewidget_yansitma_hesaplari->item(row_number,ROW_ID_COLUMN)->text().toInt());
    delete_query.DELETE();

}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int MUH_YANSITMA_HESAPLARI_FISI::LOCK_FIS_RECORD ( int record_id )
{
    Q_UNUSED (record_id);

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
    Q_UNUSED (record_id);
}

/**************************************************************************************
                   MUH_YANSITMA_HESAPLARI_FISI::LINE_SEARCH_EDIT_CLICKED()
***************************************************************************************/

void MUH_YANSITMA_HESAPLARI_FISI::LINE_SEARCH_EDIT_CLICKED(int , QWidget *, QLineEdit * line_edit)
{

    QString value = line_edit->text();
    int hesap_id = OPEN_MUH_HESAP_ARAMA( value, this ,true);

    if ( hesap_id EQ -1 ) {
        return;
    }

    line_edit->setText(MUH_GET_HESAP_KODU(hesap_id));
}








