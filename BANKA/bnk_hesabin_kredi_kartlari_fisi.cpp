#include <QMessageBox>
#include "ui_bnk_hesabin_kredi_kartlari_fisi.h"
#include "bnk_hesabin_kredi_kartlari_fisi_class.h"
#include "bnk_hesabin_kredi_kartlari_fisi_open.h"
#include "bnk_console_utils.h"
#include "banka_enum.h"
#include "muh_console_utils.h"
#include "e9_log.h"
#include "e9_console_utils.h"
 
#include "sube_console_utils.h"
#include "muh_hesap_arama_open.h"

extern ADAK_SQL   *            DB;

#define ROW_ID_COLUMN                  0
#define FIS_ID_COLUMN                  1
#define ORDER_COLUMN                   2
#define MUH_HESAP_ID_COLUMN            3
#define KREDI_KARTI_NUMARASI_COLUMN    4
#define KK_SAHIBI_COLUMN               5
#define ACIKLAMA_COLUMN                6
#define MUH_HESAP_KODU_COLUMN          7
#define MUH_HESAP_ISMI_COLUMN          8

/*************************************************/
/*           OPEN_BNK_HESABIN_KREDI_KARTLARI_FISI        */
/*************************************************/

void OPEN_BNK_HESABIN_KREDI_KARTLARI_FISI( int record_id ,QWidget * parent )
{
    BNK_HESABIN_KREDI_KARTLARI_FISI * F = new BNK_HESABIN_KREDI_KARTLARI_FISI(record_id,parent);
     
    F->EXEC( NOT_FULL_SCREEN );
}

/************************************************************/
/*  BNK_HESABIN_KREDI_KARTLARI_FISI::BNK_HESABIN_KREDI_KARTLARI_FISI  */
/************************************************************/

BNK_HESABIN_KREDI_KARTLARI_FISI::BNK_HESABIN_KREDI_KARTLARI_FISI ( int record_id, QWidget * parent )
:FIS_KERNEL ( parent ), m_ui ( new Ui::BNK_HESABIN_KREDI_KARTLARI_FISI )
{
    m_start_record_id = record_id;

    m_ui->setupUi    ( this );
    START_FIS_KERNEL ( this, DB);
}

/*****************************************************/
/*      BNK_HESABIN_KREDI_KARTLARI_FISI::SETUP_FORM   */
/*****************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::SETUP_FORM()
{
    SET_HELP_PAGE        ( "banka-hesaplari" );
    SET_SINGLE_RECORD_ID ( m_start_record_id );
    SET_PAGE_TITLE       ( tr ( "BNK - HESABIN KREDİ KARTLARI" ) );

    REGISTER_BUTTONS_WIDGET  ( m_ui->navigation_button_widget );
     
    m_num_of_columns   = 9;

    REGISTER_TABLE_WIDGET             ( m_ui->tablewidget_fis_satirlari, m_num_of_columns );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN       ( 4);

    SET_FIS_ORDER_DATABASE ( "bnk_hesabin_kredi_kartlari", "order_number", "kredi_karti_id" );

    SET_TABLE_ROW_WIDGETS ( MUH_HESAP_KODU_COLUMN         , WIDGET_SEARCH_EDIT);
    SET_TABLE_ROW_WIDGETS ( KREDI_KARTI_NUMARASI_COLUMN   , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS ( KK_SAHIBI_COLUMN              , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN               , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS ( MUH_HESAP_ISMI_COLUMN         , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS ( MUH_HESAP_ID_COLUMN           , WIDGET_LINE_EDIT);

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels(QStringList()<<"row id"<<"fis id"<<"column order"
                                                                            << "muh hes id"<<tr("Kredi Kartı Numarası")
                                                                            <<tr("Kredi Kartı Sahibi")<<tr("Açıklama")<<
                                                                            tr("Muhasebe Hesap Kodu")<<tr("Muhasebe Hesap İsmi")
                                                                            <<""<<"");

    m_ui->tablewidget_fis_satirlari->setColumnWidth (KREDI_KARTI_NUMARASI_COLUMN   ,159);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (KK_SAHIBI_COLUMN              ,222);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (ACIKLAMA_COLUMN               ,205);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (MUH_HESAP_KODU_COLUMN         ,164);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (MUH_HESAP_ISMI_COLUMN         ,245);

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->tablewidget_fis_satirlari->hideColumn(MUH_HESAP_ISMI_COLUMN);
        m_ui->tablewidget_fis_satirlari->hideColumn(MUH_HESAP_KODU_COLUMN);
    }

    SET_SETTING_NAME    ( "BANKA_HES_KREDI_KARTLARI" );
}

/*****************************************************/
/*      BNK_HESABIN_KREDI_KARTLARI_FISI::CLEAR_FORM_MEMBERS   */
/*****************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::CLEAR_FORM_MEMBERS ()
{

}


/*****************************************************/
/*      BNK_HESABIN_KREDI_KARTLARI_FISI::NEW_FIS_RECORD   */
/*****************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::NEW_FIS_RECORD()
{

}

/*******************************************************/
/*       BNK_HESABIN_KREDI_KARTLARI_FISI::GET_FIS_RECORD    */
/*******************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::GET_FIS_RECORD ( int fis_id )
{

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari","kredi_karti_id, kredi_karti_numarasi, kredi_karti_sahibi, aciklama, muh_hesap_id ",
                                "hesap_no_id = :hesap_no_id");
    select_query.SET_VALUE(":hesap_no_id"  , fis_id);

    if ( select_query.SELECT("order_number") EQ 0 ) {
        return ADAK_OK;
    }

    int current_row;

    while ( select_query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE();

        QLineEdit   * kredi_karti_numarasi    = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, KREDI_KARTI_NUMARASI_COLUMN );
        QLineEdit   * lineedit_kredi_karti_sahibi      = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, KK_SAHIBI_COLUMN            );
        QLineEdit   * lineedit_aciklama       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, ACIKLAMA_COLUMN             );
        QSearchEdit * searchedit_muh_hesabi   = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MUH_HESAP_KODU_COLUMN       );
        QLineEdit   * lineedit_muh_hesap_id   = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MUH_HESAP_ID_COLUMN         );
        QLineEdit   * lineedit_muh_hesap_ismi = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MUH_HESAP_ISMI_COLUMN       );

        kredi_karti_numarasi->setText       ( select_query.VALUE(1).toString()                  );
        lineedit_kredi_karti_sahibi->setText         ( select_query.VALUE(2).toString()                  );
        lineedit_aciklama->setText          ( select_query.VALUE(3).toString()                  );
        searchedit_muh_hesabi->SET_TEXT     ( MUH_GET_HESAP_KODU(select_query.VALUE(4).toInt()) );
        lineedit_muh_hesap_id->setText      ( select_query.VALUE(4).toString()                  );
        lineedit_muh_hesap_ismi->setText    ( MUH_GET_HESAP_ISMI(select_query.VALUE(4).toInt()) );

        QTableWidgetItem * new_item = new QTableWidgetItem(QVariant(fis_id).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row,FIS_ID_COLUMN,new_item);

        new_item = new QTableWidgetItem(select_query.VALUE(0).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row,ROW_ID_COLUMN,new_item);

    }

    return ADAK_OK;
}

/******************************************************************/
/*       BNK_HESABIN_KREDI_KARTLARI_FISI::SET_LINE_DEFAULTS            */
/******************************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::SET_LINE_DEFAULTS ( int row_number )
{

    QLineEdit * lineedit_kk_numarasi        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, KREDI_KARTI_NUMARASI_COLUMN  );
    QLineEdit * lineedit_kk_sahibi          = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, KK_SAHIBI_COLUMN             );
    QLineEdit * lineedit_aciklama           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, ACIKLAMA_COLUMN              );
    QLineEdit * lineedit_muh_hesap_ismi     = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, MUH_HESAP_ISMI_COLUMN        );
    QSearchEdit * searchedit_muh_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , MUH_HESAP_KODU_COLUMN        );
    QLineEdit * lineedit_muh_hesap_id       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , MUH_HESAP_ID_COLUMN          );

    lineedit_kk_numarasi->setMaxLength ( 16 );
    lineedit_kk_sahibi->setMaxLength( 30 );
    lineedit_aciklama->setMaxLength( 128 );
    lineedit_muh_hesap_ismi->setMaxLength( 50 );

    QString kk_hesap_kodu, kk_hesap_adi;

    int kk_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "kk_ent_hesap_id", &kk_hesap_kodu, &kk_hesap_adi );

    lineedit_muh_hesap_id->setText      ( QVariant ( kk_hesap_id ).toString() );
    searchedit_muh_hesap_kodu->SET_TEXT ( kk_hesap_kodu );
    lineedit_muh_hesap_ismi->setText    ( kk_hesap_adi  );
}

/****************************************************************/
/*        BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_FORM_VAR   */
/****************************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    Q_UNUSED(object);
    return ADAK_OK;
}

/***********************************************************/
/*        BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_FORM_EMPTY */
/***********************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}

/***********************************************************/
/*     BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_RECORD_ADD    */
/***********************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_RECORD_ADD()
{
    return ADAK_OK;
}

/*****************************************************/
/*      BNK_HESABIN_KREDI_KARTLARI_FISI::ADD_FIS_RECORD   */
/*****************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::ADD_FIS_RECORD()
{
    return -1;
}

/***************************************************************/
/*      BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_RECORD_UPDATE    */
/***************************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_RECORD_UPDATE(int fis_id)
{
    Q_UNUSED ( fis_id );
    return ADAK_OK;
}

/******************************************************/
/*   BNK_HESABIN_KREDI_KARTLARI_FISI::UPDATE_FIS_RECORD    */
/******************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::UPDATE_FIS_RECORD(int fis_id)
{
    Q_UNUSED ( fis_id );
}

/***************************************************************/
/*      BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_RECORD_DELETE    */
/***************************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_FIS_RECORD_DELETE(int fis_id)
{
    Q_UNUSED ( fis_id );
    return ADAK_OK;
}

/************************************************************/
/*       BNK_HESABIN_KREDI_KARTLARI_FISI::DELETE_FIS_RECORD      */
/************************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::DELETE_FIS_RECORD ( int fis_id )
{
    Q_UNUSED ( fis_id );
}

/*************************************************************/
/*      BNK_HESABIN_KREDI_KARTLARI_FISI::SELECT_FIS_RECORD        */
/*************************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::SELECT_FIS_RECORD()
{
    return 0;

}

/***********************************************************/
/*       BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_FIS_RECORD       */
/***********************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_FIS_RECORD()
{
    return 0;

}

/*************************************************************/
/*      BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_FIRST_FIS_RECORD    */
/*************************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_FIRST_FIS_RECORD()
{
    return 0;
}


/*********************************************************/
/*   BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_LAST_FIS_RECORD    */
/*********************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_LAST_FIS_RECORD()
{
    return 0;
}

/*********************************************************/
/*     BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_PREV_FIS_RECORD  */
/*********************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_PREV_FIS_RECORD()
{
    return 0;
}

/*********************************************************/
/*   BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_NEXT_FIS_RECORD    */
/*********************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::FIND_NEXT_FIS_RECORD()
{
    return 0;
}

/***********************************************************/
/*      BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_LINE_VAR         */
/***********************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{

    QLineEdit   * lineEditKredi_karti_numarasi  = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KREDI_KARTI_NUMARASI_COLUMN );
    QSearchEdit * searchedit_muh_kodu           = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MUH_HESAP_KODU_COLUMN       );
    QLineEdit   * lineedit_muh_hesap_ismi       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MUH_HESAP_ISMI_COLUMN       );


    if ( object EQ lineEditKredi_karti_numarasi ) {

        if ( lineEditKredi_karti_numarasi->text().size() < 16 ) {
            MSG_WARNING( tr("Kredi kartı numarası 16 haneden oluşmalı ve 1234XXXXYYYY5678 "
                                                       "formatında olmalıdır."), lineEditKredi_karti_numarasi );

            return ADAK_FAIL;
        }
    }
    else if ( object EQ searchedit_muh_kodu ) {
        int muh_hesap_id = MUH_GET_HESAP_ID(searchedit_muh_kodu->GET_TEXT());

        if (muh_hesap_id EQ 0 ) {
            MSG_WARNING( tr("Aradığınız muhasebe hesap kodu bulunamadı!.."), searchedit_muh_kodu );

            return ADAK_FAIL;
        }
        lineedit_muh_hesap_ismi->setText(MUH_GET_HESAP_ISMI(muh_hesap_id));
        QLineEdit * lineedit_muh_hesap_id = (QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MUH_HESAP_ID_COLUMN);
        lineedit_muh_hesap_id->setText(QVariant(muh_hesap_id).toString());

        return ADAK_OK;
    }
    return ADAK_OK;
}

/***************************************************************/
/*        BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_LINE_EMPTY         */
/***************************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_LINE_EMPTY(int row_number)
{

    QLineEdit * lineedit_kredi_karti_numarasi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KREDI_KARTI_NUMARASI_COLUMN );
    QLineEdit * lineedit_kredi_karti_sahibi   = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KK_SAHIBI_COLUMN            );
    QSearchEdit * searchedit_muh_hesap_kodu   = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , MUH_HESAP_KODU_COLUMN      );

    if ( searchedit_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Kredi Kartı Muhasebe hesap kodunu girmelisiniz!.." ), searchedit_muh_hesap_kodu );
        return ADAK_FAIL;
    }

    if ( lineedit_kredi_karti_numarasi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Kredi kartı numarasını girmelisiniz!.."), lineedit_kredi_karti_numarasi );

        return ADAK_FAIL;
    }

    if ( lineedit_kredi_karti_sahibi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Kredi Kartının Sahibini Girmelisiniz!.."), lineedit_kredi_karti_sahibi );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}


/***************************************************************/
/*        BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_ADD_LINE           */
/***************************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_ADD_LINE(int fis_id, int row_number)
{
    Q_UNUSED(fis_id);
    Q_UNUSED(row_number);


    QLineEdit * lineedit_kk_numarasi = (QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, KREDI_KARTI_NUMARASI_COLUMN );

    QString kredi_karti_numarasi = lineedit_kk_numarasi->text().mid(0,4);
    kredi_karti_numarasi.append("XXXXYYYY");
    kredi_karti_numarasi.append( lineedit_kk_numarasi->text().mid(12, 4));

    //1111XXXXYYYY1111

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hesabin_kredi_kartlari","kredi_karti_id","kredi_karti_numarasi =:kredi_karti_numarasi "
                          " AND hesap_no_id =:hesap_no_id" );
    query.SET_VALUE     ( ":kredi_karti_numarasi",  kredi_karti_numarasi );
    query.SET_VALUE     ( ":hesap_no_id",           fis_id  );

    if ( query.SELECT() > 0 ) {

         MSG_INFO( tr( "Aynı KREDİ KARTI NUMARASI tanımlanamaz... " ),  lineedit_kk_numarasi );
         return ADAK_FAIL;
    }


    return ADAK_OK;
}

/***************************************************************/
/*            BNK_HESABIN_KREDI_KARTLARI_FISI::ADD_LINE             */
/***************************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::ADD_LINE(int fis_id, int row_number)
{
    QWidget * cell_widget;

    cell_widget                               = m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KREDI_KARTI_NUMARASI_COLUMN);
    QLineEdit * lineedit_kredi_karti_numarasi = static_cast<QLineEdit*>(cell_widget);

    cell_widget                               = m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KK_SAHIBI_COLUMN);
    QLineEdit * lineedit_kredi_karti_sahibi            = static_cast <QLineEdit*> (cell_widget);

    cell_widget                               = m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ACIKLAMA_COLUMN);
    QLineEdit * lineedit_aciklama             = static_cast <QLineEdit*> (cell_widget);

    QLineEdit *  lineedit_muh_hesap_id        = (QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MUH_HESAP_ID_COLUMN);



    QString kredi_karti_numarasi = lineedit_kredi_karti_numarasi->text().mid(0,4);
    kredi_karti_numarasi.append("XXXXYYYY");
    kredi_karti_numarasi.append(lineedit_kredi_karti_numarasi->text().mid(12, 4));

    SQL_QUERY insert_query(DB);

    insert_query.PREPARE_INSERT("bnk_hesabin_kredi_kartlari","kredi_karti_id" , "hesap_no_id, kredi_karti_numarasi, "
                                "kredi_karti_sahibi,aciklama,muh_hesap_id,order_number");

    insert_query.SET_VALUE(":hesap_no_id"           , fis_id);
    insert_query.SET_VALUE(":kredi_karti_numarasi"  , kredi_karti_numarasi);
    insert_query.SET_VALUE(":kredi_karti_sahibi"             , lineedit_kredi_karti_sahibi->text());
    insert_query.SET_VALUE(":aciklama"              , lineedit_aciklama->text());
    insert_query.SET_VALUE(":muh_hesap_id"            , lineedit_muh_hesap_id->text().toInt());
    insert_query.SET_VALUE(":order_number"         , m_ui->tablewidget_fis_satirlari->item(row_number,ORDER_COLUMN)->text().toInt());

    int row_id = insert_query.INSERT();

    QTableWidgetItem * new_item = new QTableWidgetItem(QVariant(row_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number,ROW_ID_COLUMN,new_item);

    new_item = new QTableWidgetItem(QVariant(fis_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number,FIS_ID_COLUMN,new_item);

    QString log_detaylari = QObject::tr("Banka Hesap Numarası : ") + BNK_GET_HESAP_NUMARASI(fis_id) + " , " +
                            QObject::tr("Kredi Kartı Numarası : ") + kredi_karti_numarasi;
    E9_LOG_KAYDI_EKLE (BANKA_MODULU , LOG_BNK_KREDI_KARTLARI , LOG_ISLEM_ADD , log_detaylari);
}

/***************************************************************/
/*         BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_UPDATE_LINE       */
/***************************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_UPDATE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED(fis_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/***************************************************************/
/*        BNK_HESABIN_KREDI_KARTLARI_FISI::UPDATE_LINE              */
/***************************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::UPDATE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED(fis_id);
    QWidget * cell_widget;
    cell_widget                               = m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KREDI_KARTI_NUMARASI_COLUMN);
    QLineEdit * lineEdit_kredi_karti_numarasi = static_cast<QLineEdit*>(cell_widget);

    cell_widget                        = m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KK_SAHIBI_COLUMN);
    QLineEdit * lineedit_kredi_karti_sahibi     = static_cast <QLineEdit*> (cell_widget);

    cell_widget                        = m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ACIKLAMA_COLUMN);
    QLineEdit * lineedit_aciklama      = static_cast <QLineEdit*> (cell_widget);

    QLineEdit * lineedit_muh_hesap_id = (QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MUH_HESAP_ID_COLUMN);

    QString kredi_karti_numarasi = lineEdit_kredi_karti_numarasi->text().mid(0,4);
    kredi_karti_numarasi.append("XXXXYYYY");
    kredi_karti_numarasi.append(lineEdit_kredi_karti_numarasi->text().mid(12, 4));

    SQL_QUERY update_query(DB);

    update_query.PREPARE_UPDATE("bnk_hesabin_kredi_kartlari","kredi_karti_id","kredi_karti_numarasi ,"
                                "kredi_karti_sahibi ,aciklama ,muh_hesap_id ","kredi_karti_id = :kredi_karti_id");


    update_query.SET_VALUE(":kredi_karti_numarasi"  , kredi_karti_numarasi);
    update_query.SET_VALUE(":kredi_karti_sahibi"             , lineedit_kredi_karti_sahibi->text());
    update_query.SET_VALUE(":aciklama"              , lineedit_aciklama->text());
    update_query.SET_VALUE(":muh_hesap_id"            , lineedit_muh_hesap_id->text().toInt());
    update_query.SET_VALUE(":kredi_karti_id"        , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());
    update_query.UPDATE();

    QString log_detaylari = QObject::tr("Banka Hesap Numarası : ") + BNK_GET_HESAP_NUMARASI(fis_id) + " , " +
                            QObject::tr("Kredi Kartı Numarası : ") + kredi_karti_numarasi;
    E9_LOG_KAYDI_EKLE (BANKA_MODULU , LOG_BNK_KREDI_KARTLARI , LOG_ISLEM_UPDATE , log_detaylari);
}

/***************************************************************/
/*        BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_DELETE_LINE        */
/***************************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_DELETE_LINE(int fis_id, int row_number)
{

    Q_UNUSED(fis_id);

    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("bnk_hareket_fis_satirlari","fis_satiri_id","etkilenecek_hesap_id  = :etkilenecek_hesap_id "
                             "AND etkilenecek_hesap_turu = :etkilenecek_hesap_turu");
    sql_query.SET_VALUE(":etkilenecek_hesap_id"   , m_ui->tablewidget_fis_satirlari->item(row_number ,ROW_ID_COLUMN)->text().toInt());
    sql_query.SET_VALUE(":etkilenecek_hesap_turu" , ENUM_BNK_KREDI_KARTI_HESABI);

    if(sql_query.SELECT() NE 0 ) {
        MSG_INFO( tr("Pos işlem görmüştür.Silemezsiniz."), NULL );
        return false;
    }
    return ADAK_OK;
}

/***************************************************************/
/*        BNK_HESABIN_KREDI_KARTLARI_FISI::DELETE_LINE              */
/***************************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::DELETE_LINE(int fis_id, int row_number)
{
    Q_UNUSED(fis_id);

    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari","kredi_karti_numarasi","kredi_karti_id = :kredi_karti_id");
    sql_query.SET_VALUE(":kredi_karti_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());

    if(sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString kredi_karti_numarasi = sql_query.VALUE(0).toString();

    SQL_QUERY delete_query(DB);

    delete_query.PREPARE_DELETE("bnk_hesabin_kredi_kartlari","kredi_karti_id = :kredi_karti_id");
    delete_query.SET_VALUE(":kredi_karti_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());
    delete_query.DELETE();

    QString log_detaylari = QObject::tr("Banka Hesap Numarası : ") + BNK_GET_HESAP_NUMARASI(fis_id) + " , " +
                            QObject::tr("Kredi Kartı Numarası : ") + kredi_karti_numarasi;
    E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_KREDI_KARTLARI , LOG_ISLEM_DELETE , log_detaylari);

}

/***********************************************************/
/*         BNK_HESABIN_KREDI_KARTLARI_FISI::LOCK_FIS_RECORD    */
/***********************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::LOCK_FIS_RECORD(int fis_id)
{

    return DB->LOCK_ROW( "hesap_no_id","bnk_hesaplar", QString ( "hesap_no_id = %1" ).arg ( fis_id ));
}

/*******************************************************************/
/*             BNK_HESABIN_KREDI_KARTLARI_FISI::UNLOCK_FIS_RECORD       */
/*******************************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::UNLOCK_FIS_RECORD(int fis_id)
{
    DB->UNLOCK_ROW( "hesap_no_id","bnk_hesaplar", QString ( "hesap_no_id = %1" ).arg ( fis_id ));
}

/***********************************************************/
/*         BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_EXIT          */
/***********************************************************/

int BNK_HESABIN_KREDI_KARTLARI_FISI::CHECK_EXIT()
{
    return ADAK_OK;
}
/********************************************************************/
/*          BNK_HESABIN_KREDI_KARTLARI_FISI::LINE_SEARCH_EDIT_CLICKED    */
/********************************************************************/

void BNK_HESABIN_KREDI_KARTLARI_FISI::LINE_SEARCH_EDIT_CLICKED(int row_number,QWidget * widget,QLineEdit * line_edit)
{
    Q_UNUSED(widget);
    Q_UNUSED(line_edit);


    QSearchEdit * searchedit_muh_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MUH_HESAP_KODU_COLUMN);
    int           muh_hesap_id              = OPEN_MUH_HESAP_ARAMA( searchedit_muh_hesap_kodu->GET_TEXT(),this,1);

    if ( muh_hesap_id NE -1 ) {
        searchedit_muh_hesap_kodu->SET_TEXT(MUH_GET_HESAP_KODU(muh_hesap_id));
        return;
    }
}
