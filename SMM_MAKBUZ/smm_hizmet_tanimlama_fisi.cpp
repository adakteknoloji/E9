#include <QCommaEdit.h>
#include "adak_sql.h"
#include "smm_hizmet_tanimlama_fisi_class.h"
#include "sube_console_utils.h"
#include "muh_console_utils.h"
 
#include "e9_console_utils.h"
#include "smm_enum.h"
#include "muh_hesap_arama_open.h"
#include "sube_enum.h"

extern ADAK_SQL *           DB;

#define ROW_ID_COLUMN                  0
#define FIS_ID_COLUMN                  1
#define ORDER_NUMBER_COLUMN            2
#define HESAP_ID_COLUMN                3
#define VERGI_HESAP_ID_COLUMN          4
#define HIZMET_ADI_COLUMN              5
#define HESAP_KODU_COLUMN              6
#define HESAP_ISMI_COLUMN              7
#define VERGI_HESAP_KODU_COLUMN        8
#define VERGI_HESAP_ISMI_COLUMN        9


/**************************************************************************************
                   OPEN_SMM_HIZMET_TANIMLAMA_FISI
***************************************************************************************/

void OPEN_SMM_HIZMET_TANIMLAMA_FISI (  int p_sube_id , int p_hizmet_sekli ,QWidget * p_parent )
{
    SMM_HIZMET_TANIMLAMA_FISI * F = new SMM_HIZMET_TANIMLAMA_FISI ( p_sube_id,p_hizmet_sekli, p_parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::SMM_HIZMET_TANIMLAMA_FISI
***************************************************************************************/

SMM_HIZMET_TANIMLAMA_FISI::SMM_HIZMET_TANIMLAMA_FISI(int sube_id, int hizmet_sekli,QWidget * parent) :FIS_KERNEL(parent), m_ui ( new Ui::SMM_HIZMET_TANIMLAMA_FISI )
{
    m_sube_id       = sube_id;

    m_hizmet_sekli  = hizmet_sekli;

    m_ui->setupUi ( this );

    START_FIS_KERNEL(this, DB );

 }

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::SETUP_FORM()
{
    if ( m_hizmet_sekli EQ SMM_ALINAN_HIZMET ) {
        SET_PAGE_TITLE    ( tr ( "SMM - SMMM ALINAN HİZMETLER" ) );
        SET_HELP_PAGE     ("serbest-meslek-makbuzu_alinan-hizmetler");
    }
    else {
        SET_PAGE_TITLE    ( tr ( "SMM - SMMM SATILAN HİZMETLER" ) );
        SET_HELP_PAGE     ("serbest-meslek-makbuzu_satilan-hizmetler");
    }

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_sube_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_sube_ismi);

    SET_SETTING_NAME  ("SMM_HIZMET_TANIMLAMA_FISI");
    QString sube_kodu, sube_adi;

    SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu, &sube_adi );

    m_ui->searchEdit_sube_kodu->SET_TEXT(sube_kodu);
    m_ui->searchEdit_sube_kodu->setDisabled(true);
    m_ui->lineEdit_sube_ismi->setText(sube_adi);
    m_ui->lineEdit_sube_ismi->setDisabled(true);

    if ( SUBE_GET_SUBE_ISLEM_YETKISI() EQ SADECE_UNITE_SUBESINDE ) {
        m_ui->searchEdit_sube_kodu->setHidden(true);
        m_ui->lineEdit_sube_ismi->setHidden(true);
        m_ui->label_sube_kodu->setHidden(true);
    }

    SET_SINGLE_RECORD_ID (m_sube_id);

    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget );

    REGISTER_TABLE_WIDGET ( m_ui->tablewidget_fis_satirlari, 10 );

    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_NUMBER_COLUMN );

    SET_FORM_ID_COLUMN_POSITION ( FIS_ID_COLUMN );

    SET_ROW_ID_COLUMN_POSITION  ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN ( 5 );

    SET_FIS_ORDER_DATABASE ("smm_hizmetler", "order_number", "hizmet_id" );


    SET_TABLE_ROW_WIDGETS ( HIZMET_ADI_COLUMN       , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( HESAP_KODU_COLUMN       , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( HESAP_ISMI_COLUMN       , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( HESAP_ID_COLUMN         , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( VERGI_HESAP_ID_COLUMN   , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( VERGI_HESAP_KODU_COLUMN , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( VERGI_HESAP_ISMI_COLUMN , WIDGET_LINE_EDIT   );

    SET_FIRST_FOCUS_WIDGET (m_ui->tablewidget_fis_satirlari );

    if ( m_hizmet_sekli EQ SMM_ALINAN_HIZMET ) {
        m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels
                (QStringList() << "row_id" << "fis_id"    << "order_number_column"<<"gider hsp id"<<"vergi_hsp_id"
                               << tr("Alınan Hizmet")     << tr("Gider Hesabı Kodu")
                               << tr("Gider Hesabı İsmi") << tr("Vergi Hesabı Kodu")
                               << tr("Vergi Hesabı İsmi") << tr("") << tr("") );
    }
    else {
        m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels
                (QStringList() << "row_id" << "fis_id"    << "order_number_column"<<"gelir hsp id"<<"vergi_hsp_id"
                               << tr("Satılan Hizmet")    << tr("Gelir Hesabı Kodu")
                               << tr("Gelir Hesabı İsmi") << tr("Vergi Hesabı Kodu")
                               << tr("Vergi Hesabı İsmi") << tr("") << tr("") );
    }

    m_ui->tablewidget_fis_satirlari->setColumnWidth(HIZMET_ADI_COLUMN            , 200);
    m_ui->tablewidget_fis_satirlari->setColumnWidth(HESAP_KODU_COLUMN            , 125);
    m_ui->tablewidget_fis_satirlari->setColumnWidth(HESAP_ISMI_COLUMN            , 175);
    m_ui->tablewidget_fis_satirlari->setColumnWidth(VERGI_HESAP_KODU_COLUMN      , 125);
    m_ui->tablewidget_fis_satirlari->setColumnWidth(VERGI_HESAP_ISMI_COLUMN      , 175);

    m_ui->lineEdit_sube_ismi->setMaxLength(30);

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->tablewidget_fis_satirlari->hideColumn(HESAP_KODU_COLUMN);
        m_ui->tablewidget_fis_satirlari->hideColumn(HESAP_ISMI_COLUMN);
        m_ui->tablewidget_fis_satirlari->hideColumn(VERGI_HESAP_KODU_COLUMN);
        m_ui->tablewidget_fis_satirlari->hideColumn(VERGI_HESAP_ISMI_COLUMN);
    }


    FOCUS_FIRST_WIDGET ();
}


/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::CLEAR_FORM_MEMBERS()
{

}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::NEW_FIS_RECORD
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::NEW_FIS_RECORD()
{

}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::GET_FIS_RECORD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::GET_FIS_RECORD ( int sube_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_hizmetler" , "hizmet_adi, hizmet_hesap_id, vergi_hesap_id,"
                         "hizmet_id " ,
                         "sube_id = :sube_id AND hizmet_turu = :hizmet_turu ");

    query.SET_VALUE(":sube_id"       , sube_id);
    query.SET_VALUE(":hizmet_turu"   , m_hizmet_sekli);

    if ( query.SELECT(" order_number ASC ") EQ 0 ) {
        return ADAK_OK;
    }
    int current_row = 0;
    QTableWidgetItem * item;


    while ( query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE();

        QLineEdit   * lineedit_hizmet_adi         = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(current_row, HIZMET_ADI_COLUMN);
        QSearchEdit * searchedit_hesap_kodu       = ( QSearchEdit * )  m_ui->tablewidget_fis_satirlari->cellWidget(current_row,HESAP_KODU_COLUMN);
        QLineEdit   * lineedit_hesap_ismi         = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(current_row,HESAP_ISMI_COLUMN);
        QLineEdit   * lineedit_hesap_id           = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(current_row,HESAP_ID_COLUMN);
        QSearchEdit * searchedit_vergi_hesap_kodu = ( QSearchEdit * )  m_ui->tablewidget_fis_satirlari->cellWidget(current_row , VERGI_HESAP_KODU_COLUMN);
        QLineEdit   * lineedit_vergi_hesap_ismi   = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(current_row , VERGI_HESAP_ISMI_COLUMN);
        QLineEdit   * lineedit_vergi_hesap_id     = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(current_row , VERGI_HESAP_ID_COLUMN);

        lineedit_hizmet_adi->setText(query.VALUE("hizmet_adi").toString());
        lineedit_hesap_id->setText(query.VALUE("hizmet_hesap_id").toString());
        lineedit_vergi_hesap_id->setText(query.VALUE("vergi_hesap_id").toString());

        QString tam_hesap_kodu;
        QString hesap_ismi;

        MUH_GET_HESAP_KODU_HESAP_ISMI(query.VALUE("hizmet_hesap_id").toInt(),tam_hesap_kodu,hesap_ismi);

        searchedit_hesap_kodu->SET_TEXT(tam_hesap_kodu);
        lineedit_hesap_ismi->setText(hesap_ismi);

        MUH_GET_HESAP_KODU_HESAP_ISMI(query.VALUE("vergi_hesap_id").toInt(),tam_hesap_kodu , hesap_ismi );

        searchedit_vergi_hesap_kodu->SET_TEXT(tam_hesap_kodu);
        lineedit_vergi_hesap_ismi->setText(hesap_ismi);

        item = new QTableWidgetItem ( query.VALUE("hizmet_id").toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row,ROW_ID_COLUMN,item);

        item = new QTableWidgetItem ( QVariant(sube_id).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row,FIS_ID_COLUMN,item);

    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QLineEdit   * lineedit_hizmet_adi         = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HIZMET_ADI_COLUMN);
    QSearchEdit * searchedit_hesap_kodu       = ( QSearchEdit * )  m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_KODU_COLUMN);
    QLineEdit   * lineedit_hesap_ismi         = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ISMI_COLUMN);
    QLineEdit   * lineedit_hesap_id           = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ID_COLUMN);
    QSearchEdit * searchedit_vergi_hesap_kodu = ( QSearchEdit * )  m_ui->tablewidget_fis_satirlari->cellWidget(row_number , VERGI_HESAP_KODU_COLUMN);
    QLineEdit   * lineedit_vergi_hesap_ismi   = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(row_number , VERGI_HESAP_ISMI_COLUMN);
    QLineEdit   * lineedit_vergi_hesap_id     = ( QLineEdit   * )  m_ui->tablewidget_fis_satirlari->cellWidget(row_number , VERGI_HESAP_ID_COLUMN);

    lineedit_hizmet_adi->setMaxLength(128);
    lineedit_hesap_ismi->setMaxLength(60);

    lineedit_hesap_ismi->setReadOnly(true);
    lineedit_hesap_ismi->setDisabled(true);

    lineedit_vergi_hesap_ismi->setMaxLength(60);
    lineedit_vergi_hesap_ismi->setReadOnly(true);
    lineedit_vergi_hesap_ismi->setDisabled(true);

    SQL_QUERY query(DB);

    QString smmm_ent_hesap_kodu, smmm_ent_hesap_adi;

    int smmm_ent_hesap_id;

    if ( m_hizmet_sekli EQ SMM_ALINAN_HIZMET ) {
        smmm_ent_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "smmm_alinan_hizmet_hesap_id", &smmm_ent_hesap_kodu, &smmm_ent_hesap_adi );
    }
    else {
        smmm_ent_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "smmm_satilan_hizmet_hesap_id", &smmm_ent_hesap_kodu, &smmm_ent_hesap_adi );
    }

    lineedit_hesap_id->setText          ( QVariant ( smmm_ent_hesap_id ).toString() );
    searchedit_hesap_kodu->SET_TEXT     ( smmm_ent_hesap_kodu );
    lineedit_hesap_ismi->setText        ( smmm_ent_hesap_adi  );

    QString smmm_gelir_hesap_kodu, smmm_gelir_hesap_adi;

    int smmm_gelir_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "smmm_gelir_vergisi_hesap_id", &smmm_gelir_hesap_kodu, &smmm_gelir_hesap_adi );

    lineedit_vergi_hesap_id->setText        ( QVariant ( smmm_gelir_hesap_id ).toString() );
    searchedit_vergi_hesap_kodu->SET_TEXT   ( smmm_gelir_hesap_kodu );
    lineedit_vergi_hesap_ismi->setText      ( smmm_gelir_hesap_adi  );
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    Q_UNUSED ( object );
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_RECORD_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::ADD_FIS_RECORD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::ADD_FIS_RECORD ()
{
   return -1;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_RECORD_UPDATE ( int sube_id )
{
    Q_UNUSED(sube_id);
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::UPDATE_FIS_RECORD ( int sube_id )
{
    Q_UNUSED(sube_id);
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_FIS_RECORD_DELETE(int sube_id)
{
    Q_UNUSED(sube_id);
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::DELETE_FIS_RECORD ( int sube_id )
{
    Q_UNUSED(sube_id);
}
/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::SELECT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::FIND_FIS_RECORD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::FIND_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::FIND_FIRST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::FIND_LAST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
             SMM_HIZMET_TANIMLAMA_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::FIND_PREV_FIS_RECORD()
{
   return 0;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::FIND_NEXT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_LINE_VAR
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QSearchEdit * searchedit_hesap_kodu       = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_KODU_COLUMN);
    QLineEdit   * lineedit_hesap_id           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ID_COLUMN);
    QLineEdit   * lineedit_hesap_ismi         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ISMI_COLUMN);
    QSearchEdit * searchedit_vergi_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , VERGI_HESAP_KODU_COLUMN);
    QLineEdit   * lineedit_vergi_hesap_id     = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , VERGI_HESAP_ID_COLUMN);
    QLineEdit   * lineedit_vergi_hesap_ismi   = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , VERGI_HESAP_ISMI_COLUMN);

    if ( object EQ searchedit_hesap_kodu ) {

        if ( searchedit_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_hesap_ismi->clear();
            lineedit_hesap_id->clear();
            return ADAK_OK;

        }

        QString hesap_ismi;
        int     hesap_id;

        if ( MUH_GET_HESAP_ID_HESAP_ISMI(searchedit_hesap_kodu->GET_TEXT() ,hesap_id , hesap_ismi ) EQ 0 ) {
            lineedit_hesap_id->clear();
            MSG_WARNING(  tr("Aradığınız muhasebe hesabı bulunamadı."), searchedit_hesap_kodu );

            return ADAK_FAIL;
        }

        lineedit_hesap_ismi->setText(hesap_ismi);
        lineedit_hesap_id->setText(QVariant(hesap_id).toString());

    }
    else if ( object EQ searchedit_vergi_hesap_kodu ) {
        if ( searchedit_vergi_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_vergi_hesap_id->clear();
            lineedit_vergi_hesap_ismi->clear();
            return ADAK_OK;
        }
        QString hesap_ismi;
        int     hesap_id;

        if ( MUH_GET_HESAP_ID_HESAP_ISMI(searchedit_vergi_hesap_kodu->GET_TEXT() ,hesap_id , hesap_ismi ) EQ 0 ) {
            lineedit_hesap_id->clear();
            MSG_WARNING(  tr("Aradığınız muhasebe hesabı bulunamadı."), searchedit_vergi_hesap_kodu );

            return ADAK_FAIL;
        }

        lineedit_vergi_hesap_ismi->setText(hesap_ismi);
        lineedit_vergi_hesap_id->setText(QVariant(hesap_id).toString());

    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QSearchEdit * searchedit_hesap_kodu     = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_KODU_COLUMN);
    QLineEdit   * lineedit_hizmet_adi  = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HIZMET_ADI_COLUMN);

    if ( searchedit_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        if ( m_hizmet_sekli EQ SMM_ALINAN_HIZMET ) {
            MSG_WARNING(  tr("Alınan Hizmet hesabını girmelisiniz."), searchedit_hesap_kodu );
        }
        else {
            MSG_WARNING(  tr("Satılan Hizmet hesabını girmelisiniz."), searchedit_hesap_kodu );
        }
        return ADAK_FAIL;
    }

    if ( lineedit_hizmet_adi->text().isEmpty() EQ true ) {

        if ( m_hizmet_sekli EQ SMM_ALINAN_HIZMET ) {
            MSG_WARNING( tr("Alınan Hizmet adını yazmalısınız."), lineedit_hizmet_adi );
        }
        else {
            MSG_WARNING( tr("Satılan Hizmet adını yazmalısınız."), lineedit_hizmet_adi );
        }
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_ADD_LINE
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_ADD_LINE ( int sube_id, int row_number )
{
    QLineEdit * lineedit_hizmet_adi = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HIZMET_ADI_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_hizmetler ","hizmet_id",
                         "hizmet_adi = :hizmet_adi AND sube_id = :sube_id");
    query.SET_VALUE(":hizmet_adi"      , lineedit_hizmet_adi->text());
    query.SET_VALUE(":sube_id"         , sube_id);

    if ( query.SELECT() NE 0 ) {

        if ( m_hizmet_sekli EQ SMM_ALINAN_HIZMET ) {
            MSG_WARNING( tr("Aynı alınan hizmet adı daha önce tanımlanmış,lutfen gider türünü değiştiriniz."), lineedit_hizmet_adi);
        }
        else {
            MSG_WARNING( tr("Aynı satılan hizmet adı daha önce tanımlanmış,lutfen gelir türünü değiştiriniz."), lineedit_hizmet_adi );
        }
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::ADD_LINE
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::ADD_LINE ( int sube_id, int row_number )
{

    QLineEdit * lineedit_hizmet_adi       = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HIZMET_ADI_COLUMN);
    QLineEdit * lineedit_hesap_id         = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ID_COLUMN);
    QLineEdit * lineedit_vergi_hesap_id   = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , VERGI_HESAP_ID_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_INSERT("smm_hizmetler" , "hizmet_id" , "hizmet_adi , hizmet_hesap_id,sube_id,"
                         "order_number , hizmet_turu , vergi_hesap_id");

    query.SET_VALUE(":hizmet_adi"      , lineedit_hizmet_adi->text());
    query.SET_VALUE(":hizmet_hesap_id" , lineedit_hesap_id->text().toInt());
    query.SET_VALUE(":sube_id"         , sube_id);
    query.SET_VALUE(":order_number"    , m_ui->tablewidget_fis_satirlari->item(row_number,ORDER_NUMBER_COLUMN)->text().toInt());
    query.SET_VALUE(":hizmet_turu"     , m_hizmet_sekli);
    query.SET_VALUE(":vergi_hesap_id"  , lineedit_vergi_hesap_id->text().toInt());

    int hizmet_id = query.INSERT();

    QTableWidgetItem * item = new QTableWidgetItem(QVariant(hizmet_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number,ROW_ID_COLUMN,item);

    item = new QTableWidgetItem ( QVariant(sube_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number , FIS_ID_COLUMN,item);

}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_UPDATE_LINE ( int sube_id, int row_number )
{
    QLineEdit * lineedit_hizmet_adi = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HIZMET_ADI_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_hizmetler","hizmet_id",
                         "hizmet_adi = :hizmet_adi AND sube_id = :sube_id");
    query.SET_VALUE(":hizmet_adi"       , lineedit_hizmet_adi->text());
    query.SET_VALUE(":sube_id"          , sube_id);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        if ( query.VALUE(0).toInt() NE m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt()) {

            if ( m_hizmet_sekli EQ SMM_ALINAN_HIZMET ) {
                MSG_WARNING( tr("Aynı alınan hizmet daha önce tanımlanmış,lutfen gider türünü değiştiriniz."), lineedit_hizmet_adi );
            }
            else {
                MSG_WARNING( tr("Aynı satılan hizmet daha önce tanımlanmış,lutfen gelir türünü değiştiriniz."), lineedit_hizmet_adi );
            }
            return ADAK_FAIL;
        }

    }
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::UPDATE_LINE
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::UPDATE_LINE ( int sube_id, int row_number )
{
    Q_UNUSED(sube_id)

    QLineEdit * lineedit_hizmet_adi      = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number      , HIZMET_ADI_COLUMN);
    QLineEdit * lineedit_hesap_id        = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ID_COLUMN);
    QLineEdit * lineedit_vergi_hesap_id  = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , VERGI_HESAP_ID_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_UPDATE("smm_hizmetler","hizmet_id", "hizmet_adi,hizmet_hesap_id,vergi_hesap_id",
                         "hizmet_id = :hizmet_id");
    query.SET_VALUE(":hizmet_adi"      , lineedit_hizmet_adi->text());
    query.SET_VALUE(":hizmet_hesap_id"        , lineedit_hesap_id->text().toInt());
    query.SET_VALUE(":vergi_hesap_id"  , lineedit_vergi_hesap_id->text().toInt());
    query.SET_VALUE(":hizmet_id"       , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());

    query.UPDATE();

}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::CHECK_DELETE_LINE ( int sube_id, int row_number )
{
    Q_UNUSED(sube_id);

    int hizmet_id = m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt();

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar","makbuz_id", "hizmet_id = :hizmet_id");
    query.SET_VALUE(":hizmet_id" , hizmet_id);

    if ( query.SELECT() NE 0 ) {
        MSG_WARNING( tr("Hizmet SMMM fişlerinde kullanılmıştır.Silemezsiniz."), NULL );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::DELETE_LINE
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::DELETE_LINE ( int sube_id, int row_number )
{
    Q_UNUSED ( sube_id );

    SQL_QUERY query(DB);

    query.PREPARE_DELETE("smm_hizmetler", "hizmet_id = :hizmet_id");
    query.SET_VALUE(":hizmet_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());
    query.DELETE();

}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int SMM_HIZMET_TANIMLAMA_FISI::LOCK_FIS_RECORD ( int sube_id )
{
    return DB->LOCK_ROW ( "sube_id", "sub_subeler", QString ( "sube_id = %1" ).arg ( sube_id ));
}

/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::UNLOCK_FIS_RECORD ( int sube_id )
{
    DB->UNLOCK_ROW ( "sube_id", "sub_subeler", QString ( "sube_id = %1" ).arg ( sube_id ));
}


/**************************************************************************************
                   SMM_HIZMET_TANIMLAMA_FISI::LINE_SEARCH_EDIT_CLICKED()
***************************************************************************************/

void SMM_HIZMET_TANIMLAMA_FISI::LINE_SEARCH_EDIT_CLICKED(int row_number, QWidget *, QLineEdit * line_edit)
{
    Q_UNUSED(row_number);

    int hesap_id = OPEN_MUH_HESAP_ARAMA("",this,true);

    if ( hesap_id NE -1 ) {

        line_edit->setText(MUH_GET_HESAP_KODU(hesap_id));
    }
}


