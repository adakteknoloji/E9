#include <QCommaEdit.h>
#include "adak_sql.h"
#include "prk_gelir_gider_turleri_fisi_class.h"
#include "prk_gelir_gider_turleri_fisi_open.h"
#include "sube_console_utils.h"
#include "muh_console_utils.h"
 
#include "e9_console_utils.h"
#include "muh_hesap_arama_open.h"
#include "sube_enum.h"

extern ADAK_SQL *           DB;

#define ROW_ID_COLUMN                  0
#define FIS_ID_COLUMN                  1
#define ORDER_NUMBER_COLUMN            2
#define HESAP_ID_COLUMN                3
#define GELIR_GIDER_ADI_COLUMN         4
#define HESAP_KODU_COLUMN              5
#define HESAP_ISMI_COLUMN              6


/**************************************************************************************
                   OPEN_PRK_GELIR_GIDER_TURLERI_FISI
***************************************************************************************/

void OPEN_PRK_GELIR_GIDER_TURLERI_FISI (  int sube_id , int gider_turu_mu ,QWidget * parent )
{
    PRK_GELIR_GIDER_TURLERI_FISI * F = new PRK_GELIR_GIDER_TURLERI_FISI ( sube_id,gider_turu_mu, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::PRK_GELIR_GIDER_TURLERI_FISI
***************************************************************************************/

PRK_GELIR_GIDER_TURLERI_FISI::PRK_GELIR_GIDER_TURLERI_FISI(int sube_id, int gider_turu_mu,QWidget * parent) :FIS_KERNEL(parent), m_ui ( new Ui::PRK_GELIR_GIDER_TURLERI_FISI )
{
    m_sube_id       = sube_id;

    m_gider_turu_mu = gider_turu_mu;

    m_ui->setupUi ( this );

    START_FIS_KERNEL(this, DB );

 }

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::SETUP_FORM()
{
    if ( m_gider_turu_mu EQ 1 ) {
        SET_PAGE_TITLE    ( tr ( "PRK - ALIŞ / GİDER TÜRLERİ" ) );
        SET_HELP_PAGE     ("kasa-perakende_alisgider-turlari");
    }
    else {
        SET_PAGE_TITLE    ( tr ( "PRK - SATIŞ / GELİR TÜRLERİ" ) );
        SET_HELP_PAGE     ("kasa-perakende_satisgelir-turleri");
    }




    DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_sube_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_sube_ismi);

    SET_SETTING_NAME  ("PRK_GELIR_GIDER_TURLERI_FISI");
    QString sube_kodu, sube_adi;

    SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu, &sube_adi );

    SET_SINGLE_RECORD_ID  ( m_sube_id);

    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget );

    m_ui->searchEdit_sube_kodu->SET_TEXT(sube_kodu);
    m_ui->searchEdit_sube_kodu->setDisabled(true);
    m_ui->lineEdit_sube_ismi->setText(sube_adi);
    m_ui->lineEdit_sube_ismi->setDisabled(true);

    if ( SUBE_GET_SUBE_ISLEM_YETKISI() EQ SADECE_UNITE_SUBESINDE ) {
        m_ui->searchEdit_sube_kodu->setHidden(true);
        m_ui->lineEdit_sube_ismi->setHidden(true);
        m_ui->label_sube_kodu->setHidden(true);
    }

    SET_FIRST_FOCUS_WIDGET( m_ui->tablewidget_fis_satirlari );

    REGISTER_TABLE_WIDGET ( m_ui->tablewidget_fis_satirlari, 7 );

    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_NUMBER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION  ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN ( 4 );

    SET_FIS_ORDER_DATABASE ("prk_gelir_gider_turleri", "order_number", "tur_id" );

    SET_TABLE_ROW_WIDGETS ( GELIR_GIDER_ADI_COLUMN  , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( HESAP_KODU_COLUMN       , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( HESAP_ISMI_COLUMN       , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( HESAP_ID_COLUMN         , WIDGET_LINE_EDIT   );

    if ( m_gider_turu_mu EQ 1 ) {
        m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels
                (QStringList() << "row_id" << "fis_id" << "order_number_column"<<"gider hsp id"
                               << tr("Gider Türü") << tr("Gider Hesabı Kodu")
                               << tr("Gider Hesabı İsmi") << tr("") << tr("") );
    }
    else {
        m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels
                (QStringList() << "row_id" << "fis_id" << "order_number_column"<<"gelir hsp id"
                               << tr("Gelir Türü") << tr("Gelir Hesabı Kodu")
                               << tr("Gelir Hesabı İsmi") << tr("") << tr("") );
    }

    m_ui->tablewidget_fis_satirlari->setColumnWidth(GELIR_GIDER_ADI_COLUMN       , 300);
    m_ui->tablewidget_fis_satirlari->setColumnWidth(HESAP_KODU_COLUMN            , 150);
    m_ui->tablewidget_fis_satirlari->setColumnWidth(HESAP_ISMI_COLUMN            , 300);

    m_ui->lineEdit_sube_ismi->setMaxLength(30);

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->tablewidget_fis_satirlari->hideColumn(HESAP_KODU_COLUMN);
        m_ui->tablewidget_fis_satirlari->hideColumn(HESAP_ISMI_COLUMN);
    }

    FOCUS_FIRST_WIDGET ();
}


/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::CLEAR_FORM_MEMBERS()
{

}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::NEW_FIS_RECORD
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::NEW_FIS_RECORD()
{

}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::GET_FIS_RECORD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::GET_FIS_RECORD ( int sube_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_gelir_gider_turleri" , "gelir_gider_adi, hesap_id, tur_id " ,
                         "sube_id = :sube_id AND gider_turu_mu = :gider_turu_mu ");

    query.SET_VALUE(":sube_id"       , sube_id);
    query.SET_VALUE(":gider_turu_mu" , m_gider_turu_mu);

    if ( query.SELECT(" order_number ASC ") EQ 0 ) {
        return ADAK_OK;
    }
    int current_row = 0;
    QTableWidgetItem * item;


    while ( query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE();

        QLineEdit * lineedit_gelir_gider_adi= ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row, GELIR_GIDER_ADI_COLUMN);
        QSearchEdit * searchedit_hesap_kodu = ( QSearchEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,HESAP_KODU_COLUMN);
        QLineEdit * lineedit_hesap_ismi     = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,HESAP_ISMI_COLUMN);
        QLineEdit * lineedit_hesap_id       = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,HESAP_ID_COLUMN);

        lineedit_gelir_gider_adi->setText(query.VALUE(0).toString());
        lineedit_hesap_id->setText(query.VALUE(1).toString());

        QString tam_hesap_kodu;
        QString hesap_ismi;

        MUH_GET_HESAP_KODU_HESAP_ISMI(query.VALUE(1).toInt(),tam_hesap_kodu,hesap_ismi);

        searchedit_hesap_kodu->SET_TEXT(tam_hesap_kodu);
        lineedit_hesap_ismi->setText(hesap_ismi);

        item = new QTableWidgetItem ( query.VALUE(2).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row,ROW_ID_COLUMN,item);

        item = new QTableWidgetItem ( QVariant(sube_id).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row,FIS_ID_COLUMN,item);

    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QLineEdit * lineedit_hesap_ismi       = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_ISMI_COLUMN);
    QLineEdit * lineedit_gelir_gider_adi  = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , GELIR_GIDER_ADI_COLUMN);
    QSearchEdit * searchedit_hesap_kodu   = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_KODU_COLUMN);
    QLineEdit * lineedit_hesap_id         = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ID_COLUMN);

    lineedit_gelir_gider_adi->setMaxLength(128);
    lineedit_hesap_ismi->setMaxLength(60);

    lineedit_hesap_ismi->setReadOnly(true);
    lineedit_hesap_ismi->setDisabled(true);

    QString prk_ent_hesap_kodu, prk_ent_hesap_adi;

    int prk_ent_hesap_id;

    if ( m_gider_turu_mu EQ 1 ) {  
        prk_ent_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "prk_gider_ent_hesap_id", &prk_ent_hesap_kodu, &prk_ent_hesap_adi );
    }
    else {       
        prk_ent_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "prk_gelir_ent_hesap_id", &prk_ent_hesap_kodu, &prk_ent_hesap_adi );
    }

    lineedit_hesap_id->setText      ( QVariant ( prk_ent_hesap_id ).toString() );
    searchedit_hesap_kodu->SET_TEXT ( prk_ent_hesap_kodu );
    lineedit_hesap_ismi->setText    ( prk_ent_hesap_adi  );

}


/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    Q_UNUSED ( object );
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_RECORD_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::ADD_FIS_RECORD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::ADD_FIS_RECORD ()
{
   return -1;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_RECORD_UPDATE ( int sube_id )
{
    Q_UNUSED(sube_id);
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::UPDATE_FIS_RECORD ( int sube_id )
{
    Q_UNUSED(sube_id);
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_FIS_RECORD_DELETE(int sube_id)
{
    Q_UNUSED(sube_id);
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::DELETE_FIS_RECORD ( int sube_id )
{
    Q_UNUSED(sube_id);
}
/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::SELECT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::FIND_FIS_RECORD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::FIND_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::FIND_FIRST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::FIND_LAST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
             PRK_GELIR_GIDER_TURLERI_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::FIND_PREV_FIS_RECORD()
{
   return 0;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::FIND_NEXT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_LINE_VAR
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QSearchEdit * searchedit_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_KODU_COLUMN);
    QLineEdit  * lineedit_hesap_id      = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_ID_COLUMN);
    QLineEdit  * lineedit_hesap_ismi    = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_ISMI_COLUMN);

    if ( object EQ searchedit_hesap_kodu ) {

        if ( searchedit_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_hesap_ismi->clear();
            lineedit_hesap_id->clear();
            return ADAK_OK;
        }

        int     hesap_id;
        QString hesap_ismi;

        if ( MUH_GET_HESAP_ID_HESAP_ISMI(searchedit_hesap_kodu->GET_TEXT() , hesap_id,hesap_ismi ) EQ 0 ) {
            lineedit_hesap_id->clear();
            MSG_WARNING(  tr("Aradığınız muhasebe hesabı bulunamadı."), searchedit_hesap_kodu );

            return ADAK_FAIL;
        }

        lineedit_hesap_ismi->setText(hesap_ismi);
        lineedit_hesap_id->setText(QVariant(hesap_id).toString());

    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QSearchEdit * searchedit_hesap_kodu     = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_KODU_COLUMN);
    QLineEdit   * lineedit_gelir_gider_adi  = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,GELIR_GIDER_ADI_COLUMN);

    if ( searchedit_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        if ( m_gider_turu_mu EQ 1 ) {
            MSG_WARNING(  tr("Gider hesabını girmelisiniz."), searchedit_hesap_kodu );
        }
        else {
            MSG_WARNING(  tr("Gelir hesabını girmelisiniz."), searchedit_hesap_kodu );
        }
        return ADAK_FAIL;
    }

    if ( lineedit_gelir_gider_adi->text().isEmpty() EQ true ) {

        if ( m_gider_turu_mu EQ 1 ) {
            MSG_WARNING( tr("Gider Türünü yazmalısınız."), lineedit_gelir_gider_adi );
        }
        else {
            MSG_WARNING( tr("Gelir Türünü yazmalısınız."), lineedit_gelir_gider_adi );
        }
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_ADD_LINE
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_ADD_LINE ( int sube_id, int row_number )
{
    QLineEdit * lineedit_gelir_gider_adi = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , GELIR_GIDER_ADI_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_gelir_gider_turleri ","tur_id",
                         "gelir_gider_adi = :gelir_gider_adi AND sube_id = :sube_id");
    query.SET_VALUE(":gelir_gider_adi" , lineedit_gelir_gider_adi->text());
    query.SET_VALUE(":sube_id"         , sube_id);

    if ( query.SELECT() NE 0 ) {

        if ( m_gider_turu_mu EQ 1 ) {
            MSG_WARNING( tr("Aynı gider türü daha önce tanımlanmış,lutfen gider türünü değiştiriniz."), lineedit_gelir_gider_adi );
        }
        else {
            MSG_WARNING( tr("Aynı gelir türü daha önce tanımlanmış,lutfen gelir türünü değiştiriniz."), lineedit_gelir_gider_adi );
        }
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::ADD_LINE
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::ADD_LINE ( int sube_id, int row_number )
{

    QLineEdit * lineedit_gelir_gider_adi  = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , GELIR_GIDER_ADI_COLUMN);
    QLineEdit * lineedit_hesap_id         = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ID_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_INSERT("prk_gelir_gider_turleri" , "tur_id" , "gelir_gider_adi , hesap_id,sube_id,"
                         "order_number , gider_turu_mu");

    query.SET_VALUE(":gelir_gider_adi" , lineedit_gelir_gider_adi->text());
    query.SET_VALUE(":hesap_id"        , lineedit_hesap_id->text().toInt());
    query.SET_VALUE(":sube_id"         , sube_id);
    query.SET_VALUE(":order_number"    , m_ui->tablewidget_fis_satirlari->item(row_number,ORDER_NUMBER_COLUMN)->text().toInt());
    query.SET_VALUE(":gider_turu_mu"   , m_gider_turu_mu);

    int tur_id = query.INSERT();

    QTableWidgetItem * item = new QTableWidgetItem(QVariant(tur_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number,ROW_ID_COLUMN,item);

    item = new QTableWidgetItem ( QVariant(sube_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number , FIS_ID_COLUMN,item);

}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_UPDATE_LINE ( int sube_id, int row_number )
{
    QLineEdit * lineedit_gelir_gider_adi = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , GELIR_GIDER_ADI_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_gelir_gider_turleri","tur_id",
                         "gelir_gider_adi = :gelir_gider_adi AND sube_id = :sube_id");
    query.SET_VALUE(":gelir_gider_adi"  , lineedit_gelir_gider_adi->text());
    query.SET_VALUE(":sube_id"          , sube_id);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        if ( query.VALUE(0).toInt() NE m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt()) {

            if ( m_gider_turu_mu EQ 1 ) {
                MSG_WARNING( tr("Aynı gider türü daha önce tanımlanmış,lutfen gider türünü değiştiriniz."), lineedit_gelir_gider_adi );
            }
            else {
                MSG_WARNING( tr("Aynı gelir türü daha önce tanımlanmış,lutfen gelir türünü değiştiriniz."), lineedit_gelir_gider_adi );
            }
            return ADAK_FAIL;
        }

    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::UPDATE_LINE
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::UPDATE_LINE ( int sube_id, int row_number )
{
    Q_UNUSED(sube_id)

    QLineEdit * lineedit_gelir_gider_adi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , GELIR_GIDER_ADI_COLUMN);
    QLineEdit * lineedit_hesap_id        = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ID_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_UPDATE("prk_gelir_gider_turleri","tur_id", "gelir_gider_adi,hesap_id ",
                         "tur_id = :tur_id");
    query.SET_VALUE(":gelir_gider_adi" , lineedit_gelir_gider_adi->text());
    query.SET_VALUE(":hesap_id"        , lineedit_hesap_id->text().toInt());
    query.SET_VALUE(":tur_id"          , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());

    query.UPDATE();

}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::CHECK_DELETE_LINE ( int sube_id, int row_number )
{
    Q_UNUSED(sube_id);

    int tur_id = m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt();

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fis_satirlari","fis_satiri_id", "gider_turu_id = :gider_turu_id");
    query.SET_VALUE(":gider_turu_id" , tur_id);

    if ( query.SELECT() NE 0 ) {
        MSG_WARNING( tr("Gider türü perakende fişlerinde kullanılmıştır.Silemezsiniz."), NULL );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::DELETE_LINE
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::DELETE_LINE ( int sube_id, int row_number )
{
    Q_UNUSED ( sube_id );

    SQL_QUERY query(DB);

    query.PREPARE_DELETE("prk_gelir_gider_turleri", "tur_id = :tur_id");
    query.SET_VALUE(":tur_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());
    query.DELETE();

}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int PRK_GELIR_GIDER_TURLERI_FISI::LOCK_FIS_RECORD ( int sube_id )
{
    return DB->LOCK_ROW ( "sube_id", "sub_subeler", QString ( "sube_id = %1" ).arg ( sube_id ));
}

/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::UNLOCK_FIS_RECORD ( int sube_id )
{
    DB->UNLOCK_ROW ( "sube_id", "sub_subeler", QString ( "sube_id = %1" ).arg ( sube_id ));
}


/**************************************************************************************
                   PRK_GELIR_GIDER_TURLERI_FISI::LINE_SEARCH_EDIT_CLICKED()
***************************************************************************************/

void PRK_GELIR_GIDER_TURLERI_FISI::LINE_SEARCH_EDIT_CLICKED(int row_number, QWidget *, QLineEdit * line_edit)
{

    int hesap_id = OPEN_MUH_HESAP_ARAMA("",this,true);

    QLineEdit * lineedit_gider_hesabi_id = (QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_ID_COLUMN);
    lineedit_gider_hesabi_id->setText(QVariant(hesap_id).toString());

    line_edit->setText(MUH_GET_HESAP_KODU(hesap_id));

}


