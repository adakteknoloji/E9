#include <QMessageBox>
#include <QHeaderView>
#include "adak_utils.h"
#include "ui_muh_hesap_ekleme_fisi.h"
#include "muh_hesap_ekleme_fisi_class.h"
#include "muh_hesap_ekleme_fisi_open.h"
#include "adak_defines.h"
#include "muh_console_utils.h"
#include "e9_enum.h"
#include "e9_log.h"
#include "muh_enum.h"
#include "adak_gruplar.h"

extern  ADAK_SQL   *           DB;

#define ROW_ID_COLUMN                   0
#define ANA_HESAP_ID_COLUMN             1
#define ORDER_COLUMN                    2
#define GRUP_IDLERI                     3
#define ANA_HESAP_KODU_COLUMN           4
#define HESAP_KODU_COLUMN               5
#define TAM_HESAP_KODU_COLUMN           6
#define HESAP_ISMI_COLUMN               7
#define HESAP_GRUPLAR                   8
#define HESAP_GRUP_SECIMI_COLUMN        9

/**************************************************************************************
                   OPEN_MUH_HESAP_EKLEME_FISI
***************************************************************************************/

bool OPEN_MUH_HESAP_EKLEME_FISI ( int ana_hesap_id, QString delimeter, QWidget * parent )
{
    bool record_updated = true;

    MUH_HESAP_EKLEME_FISI * F = new MUH_HESAP_EKLEME_FISI ( ana_hesap_id, delimeter,&record_updated, ((MUH_HESAP_PLANI *)parent) );
    F->EXEC( NOT_FULL_SCREEN );

    return record_updated;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::MUH_HESAP_EKLEME_FISI
***************************************************************************************/

MUH_HESAP_EKLEME_FISI::MUH_HESAP_EKLEME_FISI (int ana_hesap_id, QString delimeter, bool *hesap_eklendi, MUH_HESAP_PLANI *hesap_plani)
:FIS_KERNEL ( hesap_plani ), m_ui ( new Ui::MUH_HESAP_EKLEME_FISI ), m_ana_hesap_id ( ana_hesap_id ), m_delimeter ( delimeter )
{
    m_ana_hesap_id    = ana_hesap_id;
    m_hesap_eklendi   = hesap_eklendi;
    m_muh_hesap_plani = hesap_plani;

    m_ui->setupUi    ( this );
    START_FIS_KERNEL ( this, DB );
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::SETUP_FORM
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::SETUP_FORM()
{
    SET_HELP_PAGE ( tr("hesap-plani") );
    SET_PAGE_TITLE( tr( "MUH - ALT HESAP EKLEME" ) );

    m_num_of_columns    = 10;

    REGISTER_TABLE_WIDGET   ( m_ui->tablewidget_alt_hesaplar, m_num_of_columns );
    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_button_widget );

    SET_FIRST_FOCUS_WIDGET( m_ui->line_edit_hesap_kodu );

    m_muh_hesap_plani->m_record_updated = false;
    m_muh_hesap_plani->m_line_added     = false;
    m_muh_hesap_plani->m_line_updated   = false;

    m_ui->lineEdit_hesap_ismi->setMaxLength(60);
    m_ui->line_edit_hesap_kodu->setMaxLength(30);

    SET_ROW_ID_COLUMN_POSITION  ( ROW_ID_COLUMN );
    SET_FORM_ID_COLUMN_POSITION ( ANA_HESAP_ID_COLUMN );
    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN ( 4 );

    m_ana_hesap_aktarilmalimi = false;

    SET_TABLE_ROW_WIDGETS ( ANA_HESAP_KODU_COLUMN,      WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( HESAP_KODU_COLUMN,          WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( TAM_HESAP_KODU_COLUMN,      WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( HESAP_ISMI_COLUMN,          WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( HESAP_GRUPLAR ,             WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( HESAP_GRUP_SECIMI_COLUMN,   WIDGET_PUSH_BUTTON  );

    SET_LINE_BUTTON_ICON ( QStringList() << "" << "" );

    m_ui->tablewidget_alt_hesaplar->setHorizontalHeaderLabels ( QStringList()<< "row_id" << "hesap_id" << "satir_order" << "grup_idleri"
                                                                            << tr ( "Ana Hesap Kodu" )
                                                                            << tr ( "Hesap Kodu" )
                                                                            << tr ( "Tam Hesap Kodu" )
                                                                            << tr ( "Hesap İsmi" )
                                                                            << tr ( "Hesabın Grupları" )
                                                                            << "" << "" << "" );

    m_ui->tablewidget_alt_hesaplar->setSelectionMode    ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_alt_hesaplar->setFocus            ( Qt::OtherFocusReason );

    m_ui->tablewidget_alt_hesaplar->setColumnWidth      ( ANA_HESAP_KODU_COLUMN,     140 );
    m_ui->tablewidget_alt_hesaplar->setColumnWidth      ( HESAP_KODU_COLUMN,         130 );
    m_ui->tablewidget_alt_hesaplar->setColumnWidth      ( TAM_HESAP_KODU_COLUMN,     200 );
    m_ui->tablewidget_alt_hesaplar->setColumnWidth      ( HESAP_ISMI_COLUMN,         200 );
    m_ui->tablewidget_alt_hesaplar->setColumnWidth      ( HESAP_GRUPLAR,             170 );
    m_ui->tablewidget_alt_hesaplar->setColumnWidth      ( HESAP_GRUP_SECIMI_COLUMN,  100 );

    SET_SETTING_NAME                                    ( "MUH_HESAP_EKLEME_FISI" );

    if ( m_ana_hesap_id NE -1 ) {
        AUTOSTART_FIS_KERNEL( m_ana_hesap_id );
    }
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::REORGANIZE_ORDER_NUMBER
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::REORGANIZE_ORDER_NUMBER ( int row_number, int new_order_number )
{
    Q_UNUSED ( row_number );
    Q_UNUSED ( new_order_number );
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::CLEAR_FORM_MEMBERS()
{

}


/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::NEW_FIS_RECORD
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::NEW_FIS_RECORD()
{
    m_ana_hesap_id       = 1;
    m_ana_hesap_seviyesi = 1;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::GET_FIS_RECORD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::GET_FIS_RECORD ( int p_fis_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu, hesap_ismi, hesap_seviyesi, hesap_kodu" ,
                         "hesap_id = :hesap_id");
    query.SET_VALUE      (":hesap_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    query.NEXT();

    m_ui->line_edit_hesap_kodu->setText( query.VALUE(0).toString() );
    m_ui->lineEdit_hesap_ismi->setText( query.VALUE(1).toString() );

    m_ana_hesap_seviyesi = query.VALUE(2).toInt();
    m_ana_hesap_kodu     = query.VALUE(3).toString();


    m_ui->line_edit_hesap_kodu->setEnabled( false );
    m_ui->lineEdit_hesap_ismi->setEnabled( false );

    if ( m_ana_hesap_seviyesi EQ 1 ) {
        m_ui->line_edit_hesap_kodu->setEnabled( true );
        m_ui->lineEdit_hesap_ismi->setEnabled ( true );
    }

    query.PREPARE_SELECT("muh_hesaplar" , "hesap_id, hesap_kodu, hesap_ismi" ,
                         "parent_id = :parent_id");
    query.SET_VALUE  ( ":parent_id", p_fis_id );

    if ( query.SELECT("hesap_kodu") EQ 0 ) {
        return ADAK_OK;
    }

    int current_row = 0;

    while ( query.NEXT() EQ true ) {

        current_row        = ADD_NEW_LINE();

        QLineEdit * lineEdit_hesap_kodu     =  ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( current_row, HESAP_KODU_COLUMN      );
        QLineEdit * lineEdit_hesap_ismi     =  ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( current_row, HESAP_ISMI_COLUMN      );
        QLineEdit * lineEdit_gruplar        =  ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( current_row, HESAP_GRUPLAR          );
        QLineEdit * lineEdit_tam_hesap_kodu =  ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( current_row, TAM_HESAP_KODU_COLUMN  );

        QTableWidgetItem * new_item;

        new_item = new QTableWidgetItem         ( query.VALUE(0).toString() );
        m_ui->tablewidget_alt_hesaplar->setItem (  current_row, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem         ( QVariant ( p_fis_id ).toString() );
        m_ui->tablewidget_alt_hesaplar->setItem ( current_row, ANA_HESAP_ID_COLUMN, new_item );

        lineEdit_hesap_kodu->setText        ( query.VALUE(1).toString() );
        lineEdit_hesap_ismi->setText        ( query.VALUE(2).toString() );
        lineEdit_tam_hesap_kodu->setText    ( m_ui->line_edit_hesap_kodu->text() + m_delimeter + query.VALUE(1).toString() );

        QList<int> grup_idleri;

        lineEdit_gruplar->setText(GRP_KAYDIN_GRUPLARINI_BUL(E9_PROGRAMI,MUHASEBE_MODULU,query.VALUE(0).toInt(),&grup_idleri,new QStringList(), new QStringList()));

        QString    grup_id_str = "";

        for ( int i = 0 ; i < grup_idleri.size() ; i++ ) {
            if ( grup_id_str.isEmpty() EQ false ) {
                grup_id_str.append("\t");
            }
            grup_id_str .append( QVariant(grup_idleri.at(i)).toString());
        }

        m_ui->tablewidget_alt_hesaplar->setItem(current_row,GRUP_IDLERI,new QTableWidgetItem(grup_id_str));
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QPushButton * button_grup_secimi      =  ( QPushButton * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, HESAP_GRUP_SECIMI_COLUMN  );
    QLineEdit   * lineEdit_tam_hesap_kodu =  ( QLineEdit   * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, TAM_HESAP_KODU_COLUMN     );
    QLineEdit   * lineEdit_hesap_kodu     =  ( QLineEdit   * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, HESAP_KODU_COLUMN         );
    QLineEdit   * lineEdit_ana_hesap_kodu =  ( QLineEdit   * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, ANA_HESAP_KODU_COLUMN     );
    QLineEdit   * lineEdit_hesap_ismi     =  ( QLineEdit   * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, HESAP_ISMI_COLUMN         );
    QLineEdit   * lineEdit_hesap_gruplar  =  ( QLineEdit   * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, HESAP_GRUPLAR             );

    lineEdit_hesap_ismi->setMaxLength       ( 60  );
    lineEdit_hesap_gruplar->setMaxLength    ( 120 );

    button_grup_secimi->setToolTip          ( tr ( "Grup seçimi için tıklayınız. " ) );
    button_grup_secimi->setText             ( tr("Grup Seç") );

    REGISTER_LINE_CHANGER_BUTTON            ( row_number, HESAP_GRUP_SECIMI_COLUMN );

    lineEdit_tam_hesap_kodu->setReadOnly    ( true );
    lineEdit_tam_hesap_kodu->setMaxLength   ( 30 );
    lineEdit_tam_hesap_kodu->setText        ( m_ui->line_edit_hesap_kodu->text() );

    lineEdit_hesap_kodu->setMaxLength       ( ( 30 - m_ui->line_edit_hesap_kodu->text().size() ) - 1 );

    lineEdit_ana_hesap_kodu->setEnabled     ( false );
    lineEdit_ana_hesap_kodu->setMaxLength   ( 30 );
    lineEdit_ana_hesap_kodu->setText        ( m_ui->line_edit_hesap_kodu->text() );
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( object EQ m_ui->line_edit_hesap_kodu ) {
        QString tam_hesap_kodu = m_ui->line_edit_hesap_kodu->text();
        QRegExp non_digit ( " ( \\D+ )" );
        if ( m_ui->line_edit_hesap_kodu->text().contains(non_digit) EQ true ) {
            MSG_WARNING( tr ( "Hesap kodu sadece numerik karakterlerden oluşabilir" ), m_ui-> line_edit_hesap_kodu );

            m_ui->line_edit_hesap_kodu->undo();
            return ADAK_FAIL;
        }
        SQL_QUERY query ( DB );

        query.PREPARE_SELECT ( "muh_hesaplar","hesap_id","tam_hesap_kodu = :tam_hesap_kodu" );

        query.SET_VALUE      ( ":tam_hesap_kodu", tam_hesap_kodu );

        if (query.SELECT() > 0 ) {
            MSG_WARNING( tr ( "Aynı hesap koduyla kayıtlı başka bir muhasebe hesabı sistemde mevcuttur."
                                                     "Lütfen hesap kodunuzu değiştiriniz.." ), m_ui->line_edit_hesap_kodu );

            m_ui->line_edit_hesap_kodu->undo();
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_FIS_FORM_EMPTY()
{
    SQL_QUERY sql_query  ( DB );

    if ( m_ui->line_edit_hesap_kodu->text() EQ "" ) {
        MSG_WARNING( tr ( "Hesap kodu boş bırakılamaz" ), m_ui->line_edit_hesap_kodu );

        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_hesap_ismi->text() EQ "" ) {
        MSG_WARNING( tr ( "Hesap ismi boş bırakılamaz" ), m_ui->lineEdit_hesap_ismi );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_FIS_RECORD_ADD()
{
    SQL_QUERY sql_query( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id" , "hesap_ismi = :hesap_ismi AND hesap_kodu = :hesap_kodu AND parent_id = :parent_id");

    sql_query.SET_VALUE      ( ":hesap_ismi", m_ui->lineEdit_hesap_ismi->text() );
    sql_query.SET_VALUE      ( ":hesap_kodu", m_ui->line_edit_hesap_kodu->text() );
    sql_query.SET_VALUE      ( ":parent_id ", m_ana_hesap_id );

    if (sql_query.SELECT() > 0) {
        MSG_WARNING( "Aynı hesap ismi ve hesap koduna sahip başka bir alt hesap vardır.", NULL );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::ADD_FIS_RECORD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::ADD_FIS_RECORD()
{
    SQL_QUERY sql_query( DB );
    sql_query.PREPARE_INSERT("muh_hesaplar" , "hesap_id" ,
                             "tali_hesap ,      "
                             "alt_hesap_sayisi, "
                             "parent_id,        "
                             "hesap_seviyesi,   "
                             "hesap_kodu ,      "
                             "hesap_ismi ,      "
                             "tam_hesap_kodu    ");


    sql_query.SET_VALUE ( ":tali_hesap"         , 1 );
    sql_query.SET_VALUE ( ":alt_hesap_sayisi"   , 0 );
    sql_query.SET_VALUE ( ":parent_id"          , 0 );
    sql_query.SET_VALUE ( ":hesap_seviyesi"     , m_ana_hesap_seviyesi );
    sql_query.SET_VALUE ( ":hesap_kodu"         , m_ui->line_edit_hesap_kodu->text() );
    sql_query.SET_VALUE ( ":hesap_ismi"         , m_ui->lineEdit_hesap_ismi->text()  );
    sql_query.SET_VALUE ( ":tam_hesap_kodu"     , m_ui->line_edit_hesap_kodu->text() );

    int p_hesap_id         = sql_query.INSERT();

    m_muh_hesap_plani->m_added_record << m_ui->line_edit_hesap_kodu->text()
                                      << m_ui->lineEdit_hesap_ismi->text()
                                      << "0"
                                      << MUH_GET_BORC_ARRAY  ( p_hesap_id )+" "
                                      << MUH_GET_ALACAK_ARRAY( p_hesap_id )+" "
                                      << QVariant ( p_hesap_id ).toString();

    QString log_detaylari = m_ui->line_edit_hesap_kodu->text() + "  " + m_ui->lineEdit_hesap_ismi->text()
                            + QObject::tr ( " Hesabı Eklendi.");

    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_HESAP_PLANI , LOG_ISLEM_ADD , log_detaylari );

    return p_hesap_id;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_FIS_RECORD_UPDATE ( int fis_id )
{
    SQL_QUERY sql_query( DB );

    int secim = MSG_YES_NO( tr( "Hesap güncellenecektir. Emin misiniz?" ), NULL );

    if ( secim EQ ADAK_NO ) {
        return ADAK_FAIL;
    }

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id" , "hesap_ismi = :hesap_ismi AND hesap_kodu = :hesap_kodu AND parent_id = :parent_id");

    sql_query.SET_VALUE      ( ":hesap_ismi", m_ui->lineEdit_hesap_ismi->text() );
    sql_query.SET_VALUE      ( ":hesap_kodu", m_ui->line_edit_hesap_kodu->text() );
    sql_query.SET_VALUE      ( ":parent_id ", fis_id );

    if (sql_query.SELECT() > 0) {
        MSG_WARNING( "Aynı hesap ismi ve hesap koduna sahip başka bir alt hesap vardır.", NULL );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::UPDATE_FIS_RECORD( int p_fis_id )
{
    QString tam_hesap_kodu;

    SQL_QUERY f_query( DB );
    SQL_QUERY s_query( DB );

    f_query.PREPARE_SELECT("muh_hesaplar" , "parent_id" , "hesap_id = :hesap_id");
    f_query.SET_VALUE     (":hesap_id"    , p_fis_id  );

    if ( f_query.SELECT() > 0 ) {
        f_query.NEXT();
        if ( f_query.VALUE(0).toInt() EQ 0 ) {
            tam_hesap_kodu.clear();
        }
        else {
            s_query.PREPARE_SELECT ( "muh_hesaplar","tam_hesap_kodu","hesap_id = :hesap_id" );

            s_query.SET_VALUE      ( ":hesap_id",f_query.VALUE(0).toInt() );

            if ( s_query.SELECT() > 0 ) {
                s_query.NEXT();
                tam_hesap_kodu = s_query.VALUE(0).toString();
                tam_hesap_kodu.append ( m_delimeter  );
            }
        }
    }

    tam_hesap_kodu.append ( m_ui->line_edit_hesap_kodu->text() );
    s_query.PREPARE_UPDATE (  "muh_hesaplar","hesap_id" ,"hesap_kodu, hesap_ismi, tam_hesap_kodu",
                                "hesap_id = :hesap_id" );

    s_query.SET_VALUE      (  ":hesap_kodu"     , m_ui->line_edit_hesap_kodu->text() );
    s_query.SET_VALUE      (  ":hesap_ismi"     , m_ui->lineEdit_hesap_ismi->text() );
    s_query.SET_VALUE      (  ":tam_hesap_kodu" , tam_hesap_kodu      );
    s_query.SET_VALUE      (  ":hesap_id"       , p_fis_id          );
    s_query.UPDATE();

    s_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id , alt_hesap_sayisi" ,"parent_id = :parent_id");
    s_query.SET_VALUE      ( ":parent_id", p_fis_id );
    int alt_hesap_sayisi = 0;

    if ( s_query.SELECT() NE 0 ) {
        s_query.NEXT();
        alt_hesap_sayisi = s_query.VALUE(1).toInt();
    }

    m_muh_hesap_plani->m_updated_record << m_ui->line_edit_hesap_kodu->text() <<  m_ui->lineEdit_hesap_ismi->text()
                       << QVariant(alt_hesap_sayisi).toString() << MUH_GET_ALACAK_ARRAY( p_fis_id )
                       +" " << MUH_GET_BORC_ARRAY( p_fis_id )+" "<<  QVariant ( p_fis_id ).toString();

    MUH_ALT_HESAPLARIN_TAM_HESAP_KODUNU_GUNCELLE ( p_fis_id, tam_hesap_kodu, m_delimeter );

    QString log_detaylari = tam_hesap_kodu + "  " + m_ui->lineEdit_hesap_ismi->text() + QObject::tr ( " Hesabı Güncellendi." );

    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU,LOG_MUH_HESAP_PLANI,LOG_ISLEM_UPDATE,log_detaylari );

}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_FIS_RECORD_DELETE ( int fis_id )
{
    Q_UNUSED(fis_id);

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::DELETE_FIS_RECORD ( int fis_id )
{
    Q_UNUSED(fis_id);
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::SELECT_FIS_RECORD()
{
    return 1;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::FIND_FIS_RECORD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::FIND_FIS_RECORD()
{
    return 1;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "muh_hesaplar", "hesap_id","hesap_seviyesi = :hesap_seviyesi ");
    query.SET_VALUE      ( ":hesap_seviyesi"   ,  m_ana_hesap_seviyesi );

    if ( query.SELECT("hesap_kodu ASC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "muh_hesaplar", "hesap_id","hesap_seviyesi = :hesap_seviyesi ");
    query.SET_VALUE      ( ":hesap_seviyesi" ,  m_ana_hesap_seviyesi );

    if ( query.SELECT("hesap_kodu DESC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY  query ( DB );

    QStringList alt_hesap_kodu_list = m_ui->line_edit_hesap_kodu->text().split( m_delimeter );

    query.PREPARE_SELECT ( "muh_hesaplar", "hesap_id","hesap_kodu < :hesap_kodu ");
    query.SET_VALUE      ( ":hesap_kodu"   ,  alt_hesap_kodu_list.at( alt_hesap_kodu_list.size() -1 ) );
    query.AND_EKLE       ( "hesap_seviyesi = :hesap_seviyesi");
    query.SET_VALUE      ( ":hesap_seviyesi", m_ana_hesap_seviyesi );

    if ( query.SELECT("hesap_kodu DESC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY  query ( DB );

    QStringList alt_hesap_kodu_list = m_ui->line_edit_hesap_kodu->text().split( m_delimeter );

    query.PREPARE_SELECT ( "muh_hesaplar", "hesap_id","hesap_kodu > :hesap_kodu ");
    query.SET_VALUE      ( ":hesap_kodu"   , alt_hesap_kodu_list.at( alt_hesap_kodu_list.size() -1 ) );

    query.AND_EKLE       ( "hesap_seviyesi = :hesap_seviyesi");
    query.SET_VALUE      ( ":hesap_seviyesi", m_ana_hesap_seviyesi );

    if ( query.SELECT("hesap_kodu ASC", 0, 1 ) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_LINE_VAR
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QString     alt_hesap_kodu;
    QLineEdit * lineEdit_hesap_kodu     = ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, HESAP_KODU_COLUMN     );
    QLineEdit * lineEdit_tam_hesap_kodu = ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, TAM_HESAP_KODU_COLUMN );


    if ( object EQ lineEdit_hesap_kodu ) {

        SQL_QUERY sql_query  ( DB );

        QRegExp non_digit( " ( \\D+ )" );
        if ( lineEdit_hesap_kodu->text().contains ( non_digit ) EQ true ) {
            MSG_WARNING( tr ( "Hesap kodu sadece numerik karakterlerden oluşabilir" ), lineEdit_hesap_kodu );
            lineEdit_hesap_kodu->undo();
            return ADAK_FAIL;
        }

        sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu" , "hesap_id = :hesap_id");
        sql_query.SET_VALUE       (  ":hesap_id", m_ana_hesap_id );

        if ( sql_query.SELECT() > 0 ) {
             sql_query.NEXT();
             alt_hesap_kodu = sql_query.VALUE(0).toString();
             alt_hesap_kodu.append ( m_delimeter );
        }

        alt_hesap_kodu.append ( lineEdit_hesap_kodu->text() );
        int  new_hesap_id = MUH_GET_HESAP_ID ( alt_hesap_kodu );
        if ( new_hesap_id NE 0 ) {
            MSG_WARNING(  tr ( "Bu hesap koduna sahip başka bir hesap bulunmaktadır. \n "
                                                              "Lütfen kontrol ederek tekrar deneyiniz" ), NULL );
            return ADAK_FAIL_UNDO;
        }

        lineEdit_tam_hesap_kodu->setText ( QString ( m_ui->line_edit_hesap_kodu->text() + m_delimeter + lineEdit_hesap_kodu->text() ) );

        return ADAK_OK;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QLineEdit * lineEdit_hesap_kodu  = ( QLineEdit *) m_ui->tablewidget_alt_hesaplar->cellWidget  ( row_number, HESAP_KODU_COLUMN );
    QLineEdit * lineEdit_hesap_ismi  = ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, HESAP_ISMI_COLUMN );

    if ( lineEdit_hesap_kodu->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Hesap kodu boş bırakılamaz." ), lineEdit_hesap_kodu );

        return ADAK_FAIL;
    }

    if ( lineEdit_hesap_ismi->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Hesap ismi boş bırakılamaz." ), lineEdit_hesap_ismi );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}


/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_ADD_LINE
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_ADD_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );
    Q_UNUSED ( row_number );

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("muh_hesaplar" , "alt_hesap_sayisi" , "hesap_id = :hesap_id");

    query.SET_VALUE(":hesap_id" , m_ana_hesap_id );

    int alt_hesap_sayisi = 0;

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        alt_hesap_sayisi = query.VALUE(0).toInt();
    }
    if ( alt_hesap_sayisi EQ 0 ) {
        m_ana_hesap_aktarilmalimi = true;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::ADD_LINE
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::ADD_LINE ( int fis_id, int row_number )
{
    SQL_QUERY sql_query ( DB );

    QString     yeni_tam_hesap_kodu;
    QLineEdit * lineEdit_hesap_kodu = ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget( row_number, HESAP_KODU_COLUMN );
    QString     yeni_alt_hesap_kodu = lineEdit_hesap_kodu->text();
    QLineEdit * lineEdit_hesap_ismi = ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget( row_number, HESAP_ISMI_COLUMN );
    QString     yeni_alt_hesap_ismi = lineEdit_hesap_ismi->text();

    yeni_tam_hesap_kodu.append ( MUH_GET_HESAP_KODU ( fis_id ) );
    yeni_tam_hesap_kodu.append ( m_delimeter );
    yeni_tam_hesap_kodu.append ( yeni_alt_hesap_kodu );

    int parent_id        = fis_id;
    int hesap_seviyesi   = m_ana_hesap_seviyesi + 1;
    int alt_hesap_sayisi = 0;

    sql_query.PREPARE_SELECT("muh_hesaplar" , "alt_hesap_sayisi" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      ( ":hesap_id",fis_id );
    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        alt_hesap_sayisi = sql_query.VALUE(0).toInt();
        alt_hesap_sayisi++;
    }

    sql_query.PREPARE_UPDATE("muh_hesaplar" ,"hesap_id" , "tali_hesap , alt_hesap_sayisi",
                             "hesap_id = :hesap_id");
    sql_query.SET_VALUE      ( ":tali_hesap"      , 0       );
    sql_query.SET_VALUE      ( ":alt_hesap_sayisi", alt_hesap_sayisi );
    sql_query.SET_VALUE      ( ":hesap_id "       , fis_id );
    sql_query.UPDATE();

    sql_query.PREPARE_INSERT("muh_hesaplar" , "hesap_id" , "tali_hesap,alt_hesap_sayisi");
    sql_query.SET_VALUE(":tali_hesap"       , 1);
    sql_query.SET_VALUE(":alt_hesap_sayisi" , 0);

    int alt_hesap_id         = sql_query.INSERT();

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( alt_hesap_id ).toString() );
    m_ui->tablewidget_alt_hesaplar->setItem ( row_number, ROW_ID_COLUMN, new_item );
    new_item = new QTableWidgetItem ( QVariant ( fis_id ).toString() );
    m_ui->tablewidget_alt_hesaplar->setItem ( row_number, ANA_HESAP_ID_COLUMN, new_item );

    sql_query.PREPARE_UPDATE("muh_hesaplar" , "hesap_id" , "parent_id , hesap_seviyesi, "
                             "hesap_kodu , hesap_ismi , tam_hesap_kodu , tali_hesap" ,
                             "hesap_id = :hesap_id");

    sql_query.SET_VALUE      (   ":parent_id"      , parent_id           );
    sql_query.SET_VALUE      (   ":hesap_seviyesi" , hesap_seviyesi      );
    sql_query.SET_VALUE      (   ":hesap_kodu"     , yeni_alt_hesap_kodu );
    sql_query.SET_VALUE      (   ":hesap_ismi"     , yeni_alt_hesap_ismi );
    sql_query.SET_VALUE      (   ":tam_hesap_kodu" , yeni_tam_hesap_kodu );
    sql_query.SET_VALUE      (   ":tali_hesap"     , 1                   );
    sql_query.SET_VALUE      (   ":hesap_id"       , alt_hesap_id        );
    sql_query.UPDATE();

    int yeni_hesap_id           = alt_hesap_id;
    int yeni_hesap_parent_id    = fis_id;

    if ( m_ana_hesap_aktarilmalimi EQ true ) {

        sql_query.PREPARE_SELECT("muh_hesaplar" , "parent_id,hesap_seviyesi" , "hesap_id = :hesap_id");
        sql_query.SET_VALUE      ( ":hesap_id", alt_hesap_id );

        if ( sql_query.SELECT() EQ 0 ) {
            return;
        }
        sql_query.NEXT();
        yeni_hesap_id            = alt_hesap_id;
        yeni_hesap_parent_id     = sql_query.VALUE ( "parent_id" ).toInt();
        int yeni_hesap_seviyesi  = sql_query.VALUE ( "hesap_seviyesi" ).toInt();

        sql_query.PREPARE_SELECT("muh_hesaplar" , "parent_id, hesap_seviyesi ,hesap_kodu , hesap_ismi , tam_hesap_kodu, "
                                 "toplam_borc_array,toplam_alacak_array" , "hesap_id = :hesap_id");

        sql_query.SET_VALUE (  ":hesap_id", yeni_hesap_parent_id  );

        if (sql_query.SELECT() EQ 0 ) {
            return;
        }
        sql_query.NEXT();
        int     temp_parent_id      = sql_query.VALUE(0).toInt();
        QString temp_hesap_seviyesi = sql_query.VALUE(1).toString();
        QString temp_hesap_kodu     = sql_query.VALUE(2).toString();
        QString temp_hesap_ismi     = sql_query.VALUE(3).toString();
        QString temp_tam_hesap_kodu = sql_query.VALUE(4).toString();

        double toplam_borc_array  [ E9_ARRAY_SIZE ];
        double toplam_alacak_array[ E9_ARRAY_SIZE ];

        UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(5).toString(),toplam_borc_array   ,E9_ARRAY_SIZE );
        UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(6).toString(),toplam_alacak_array ,E9_ARRAY_SIZE );

        SQL_QUERY update_query ( DB );

        update_query.PREPARE_UPDATE ( "muh_hesaplar","hesap_id" ,"parent_id, hesap_seviyesi ,hesap_kodu , hesap_ismi,"
                                      "tam_hesap_kodu , tali_hesap , alt_hesap_sayisi", "hesap_id = :hesap_id" );

        update_query.SET_VALUE      (  ":parent_id"       , yeni_hesap_id        );
        update_query.SET_VALUE      (  ":hesap_seviyesi"  , yeni_hesap_seviyesi  );
        update_query.SET_VALUE      (  ":hesap_kodu"      , yeni_alt_hesap_kodu  );
        update_query.SET_VALUE      (  ":hesap_ismi"      , yeni_alt_hesap_ismi  );
        update_query.SET_VALUE      (  ":tam_hesap_kodu"  , yeni_tam_hesap_kodu  );
        update_query.SET_VALUE      (  ":tali_hesap"      , 1                    );
        update_query.SET_VALUE      (  ":hesap_id"        , yeni_hesap_parent_id );
        update_query.SET_VALUE      (  ":alt_hesap_sayisi", 0                    );
        update_query.UPDATE();

        update_query.PREPARE_UPDATE ( "muh_hesaplar","hesap_id","parent_id , hesap_seviyesi ,"
                                      "hesap_kodu , hesap_ismi , tam_hesap_kodu , tali_hesap ,"
                                      "alt_hesap_sayisi, "
                                      "toplam_borc_array, toplam_alacak_array" ,
                                      "hesap_id = :hesap_id" );

        update_query.SET_VALUE (  ":parent_id"                  , temp_parent_id      );
        update_query.SET_VALUE (  ":hesap_seviyesi"             , temp_hesap_seviyesi );
        update_query.SET_VALUE (  ":hesap_kodu"                 , temp_hesap_kodu     );
        update_query.SET_VALUE (  ":hesap_ismi"                 , temp_hesap_ismi     );
        update_query.SET_VALUE (  ":tam_hesap_kodu"             , temp_tam_hesap_kodu );
        update_query.SET_VALUE (  ":tali_hesap"                 , 0                   );
        update_query.SET_VALUE (  ":hesap_id"                   , yeni_hesap_id       );
        update_query.SET_VALUE (  ":alt_hesap_sayisi"           , alt_hesap_sayisi    );
        update_query.SET_VALUE (  ":toplam_borc_array"          , PACK_DOUBLE_ARRAY ( toplam_borc_array,E9_ARRAY_SIZE )   );
        update_query.SET_VALUE (  ":toplam_alacak_array"        , PACK_DOUBLE_ARRAY ( toplam_alacak_array,E9_ARRAY_SIZE ) );
        update_query.UPDATE();


        REFRESH_FIS_RECORD ( yeni_hesap_id );
        int last_column = m_ui->tablewidget_alt_hesaplar->rowCount() -1;

        m_ui->tablewidget_alt_hesaplar->setCurrentCell ( last_column,HESAP_KODU_COLUMN );
        m_ana_hesap_id  = yeni_hesap_id;
    }


    if ( m_ui->tablewidget_alt_hesaplar->item(row_number,GRUP_IDLERI) NE NULL ) {
        QStringList grup_idleri_str = m_ui->tablewidget_alt_hesaplar->item(row_number,GRUP_IDLERI)->text().split("\t");
        QList<int>  grup_idleri;

        for ( int i = 0 ; i < grup_idleri_str.size() ; i++ ) {
            grup_idleri << grup_idleri_str.at(i).toInt();
        }

        GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI , MUHASEBE_MODULU , yeni_hesap_id , &grup_idleri  );
    }

    m_ana_hesap_aktarilmalimi = false;

    m_muh_hesap_plani->m_added_record << yeni_alt_hesap_kodu << yeni_alt_hesap_ismi << "0" <<
            MUH_GET_BORC_ARRAY( yeni_hesap_parent_id ) << " " << MUH_GET_ALACAK_ARRAY( yeni_hesap_parent_id) << " " << QVariant ( yeni_hesap_parent_id ).toString();

    m_muh_hesap_plani->m_updated_record << m_ana_hesap_kodu << m_ui->lineEdit_hesap_ismi->text() <<
            QVariant ( alt_hesap_sayisi ).toString() << MUH_GET_BORC_ARRAY(yeni_hesap_id ) <<
            " " << MUH_GET_ALACAK_ARRAY(yeni_hesap_id)+ " " << QVariant(yeni_hesap_parent_id).toString();

    m_muh_hesap_plani->m_line_added     = true;
    m_muh_hesap_plani->m_record_updated = true;

    QString log_detaylari = yeni_tam_hesap_kodu + "  " + yeni_alt_hesap_ismi + QObject::tr ( " Hesabı Eklendi." );

    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_HESAP_PLANI , LOG_ISLEM_ADD , log_detaylari );
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_UPDATE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED(fis_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::UPDATE_LINE
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::UPDATE_LINE ( int fis_id, int row_number )
{
    QString hesap_kodu;
    QString hesap_ismi;
    QString row_id;

    QLineEdit * lineEdit  = ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget( row_number, HESAP_KODU_COLUMN );
    hesap_kodu = lineEdit->text();
    lineEdit = ( QLineEdit * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, HESAP_ISMI_COLUMN );
    hesap_ismi = lineEdit->text();

    SQL_QUERY sql_query  ( DB );
    QString yeni_tam_hesap_kodu;
    int hesap_id = m_ui->tablewidget_alt_hesaplar->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    yeni_tam_hesap_kodu.append ( m_ui->line_edit_hesap_kodu->text() );
    yeni_tam_hesap_kodu.append ( m_delimeter );
    yeni_tam_hesap_kodu.append ( hesap_kodu );

    sql_query.PREPARE_UPDATE ( "muh_hesaplar","hesap_id","hesap_kodu , hesap_ismi , tam_hesap_kodu","hesap_id = :hesap_id"  );

    sql_query.SET_VALUE (  ":hesap_kodu"     , hesap_kodu );
    sql_query.SET_VALUE (  ":hesap_ismi"     , hesap_ismi );
    sql_query.SET_VALUE (  ":tam_hesap_kodu" , yeni_tam_hesap_kodu );
    sql_query.SET_VALUE (  ":hesap_id"       , hesap_id            );
    sql_query.UPDATE();

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id" , "parent_id = :parent_id");
    sql_query.SET_VALUE      ( ":parent_id",fis_id );

    m_muh_hesap_plani->m_updated_line <<   hesap_kodu << hesap_ismi
                       <<QVariant ( sql_query.SELECT() ).toString() << MUH_GET_BORC_ARRAY( hesap_id )
                       <<" " << MUH_GET_ALACAK_ARRAY( hesap_id ) << " " << QVariant ( hesap_id ).toString();

        MUH_ALT_HESAPLARIN_TAM_HESAP_KODUNU_GUNCELLE ( hesap_id, yeni_tam_hesap_kodu, m_delimeter );

    if ( m_ui->tablewidget_alt_hesaplar->item(row_number,GRUP_IDLERI) NE NULL ) {
        QStringList grup_idleri_str = m_ui->tablewidget_alt_hesaplar->item(row_number,GRUP_IDLERI)->text().split("\t");
        QList<int>  grup_idleri;

        for ( int i = 0 ; i < grup_idleri_str.size() ; i++ ) {
            grup_idleri << grup_idleri_str.at(i).toInt();
        }

        GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI , MUHASEBE_MODULU , hesap_id , &grup_idleri);
    }

    m_muh_hesap_plani->m_line_updated   = true;
    m_muh_hesap_plani->m_record_updated = true;

    QString log_detaylari = yeni_tam_hesap_kodu + "  " + hesap_ismi + QObject::tr ( "Hesabı Güncellendi." );

    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU ,LOG_MUH_HESAP_PLANI,LOG_ISLEM_UPDATE,log_detaylari );
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::CHECK_DELETE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED(fis_id);
    m_hesabin_parentini_aktar  = false;

    int alt_hesap_sayisi       = 0;
    int parent_id              = 0;
    int hesap_id               = m_ui->tablewidget_alt_hesaplar->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    QMessageBox * mesaj        = new QMessageBox ( this );
    mesaj->setButtonText ( QMessageBox::Ok, tr ("Devam") );
    QPushButton * buton_cancel = mesaj->addButton ( QMessageBox::Cancel );
    mesaj->setButtonText ( QMessageBox::Cancel, tr ( "Vazgeç" ) );

    SQL_QUERY sql_query  ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu , hesap_ismi" , "hesap_id = :hesap_id");
    sql_query.SET_VALUE      ( ":hesap_id", hesap_id );

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();

        mesaj->setWindowTitle ( tr ( "DİKKAT" ) );
        mesaj->setText ( tr ( "'%1 %2'` hesabı silinecek." ).arg ( sql_query.VALUE(0).toString()).arg ( sql_query.VALUE(1).toString() ) );
        mesaj->setInformativeText ( "Emin misiniz?" );
        mesaj->exec();

        if (mesaj->clickedButton() EQ buton_cancel ) {
            return ADAK_FAIL;
        }
    }

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id" , "parent_id = :parent_id");
    sql_query.SET_VALUE      ( ":parent_id", hesap_id );

    alt_hesap_sayisi = sql_query.SELECT();

    if ( alt_hesap_sayisi > 0 ) {
        mesaj->setText ( tr ( "Hesabı silmek için,önce alt hesaplarını silmelisiniz!.." ) );
        mesaj->removeButton ( mesaj->button ( QMessageBox::Cancel ) );
        mesaj->setInformativeText ( "" );
        mesaj->setButtonText ( QMessageBox::Ok, tr ( "Tamam" ) );
        mesaj->exec();
        return ADAK_FAIL;
    }


    //Hesabin tek bir alt hesabi varsa,alt hesap ile ana hesap yer degistirecek

    sql_query.PREPARE_SELECT("muh_hesaplar" , "parent_id" , "hesap_id = :hesap_id");
    sql_query.SET_VALUE      ( ":hesap_id", hesap_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();
        parent_id = sql_query.VALUE(0).toInt();
    }

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id" , "parent_id = :parent_id");

    sql_query.SET_VALUE      ( ":parent_id", parent_id );

    if ( sql_query.SELECT() NE 0 ) {

        int hesap_sayisi = sql_query.NUM_OF_ROWS();


        if ( hesap_sayisi EQ 1 ) {
            m_hesabin_parentini_aktar = true;
            return ADAK_OK;
        }
        sql_query.PREPARE_SELECT("muh_fis_satirlari" , "fis_id" , "hesap_id = :hesap_id");

        sql_query.SET_VALUE(":hesap_id" , hesap_id);
        if ( sql_query.SELECT() NE 0 ) {
            DB->CANCEL_TRANSACTION();
            mesaj = new QMessageBox();
            mesaj->setIcon ( QMessageBox::Critical );
            mesaj->setWindowTitle ( tr ( "HATA" ) );
            mesaj->addButton ( QMessageBox::Ok );
            mesaj->setText ( tr ( "Hesap işlem görmüş,silinemez!.." ) );
            mesaj->exec();
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::DELETE_LINE
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::DELETE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );

    SQL_QUERY sql_query  ( DB );

    int hesap_id  = m_ui->tablewidget_alt_hesaplar->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    int alt_hesap_sayisi = 0;

    sql_query.PREPARE_SELECT("muh_hesaplar" , "alt_hesap_sayisi" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      ( ":hesap_id" , fis_id );

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        alt_hesap_sayisi = sql_query.VALUE(0).toInt();
        alt_hesap_sayisi--;
    }

    sql_query.PREPARE_UPDATE ( "muh_hesaplar", "hesap_id" , "alt_hesap_sayisi","hesap_id = :hesap_id" );
    sql_query.SET_VALUE      ( ":alt_hesap_sayisi", alt_hesap_sayisi );
    sql_query.SET_VALUE      ( ":hesap_id "       , fis_id );
    sql_query.UPDATE();



    if ( m_hesabin_parentini_aktar EQ false ) {

        sql_query.PREPARE_DELETE("muh_hesaplar" , "hesap_id = :hesap_id");

        sql_query.SET_VALUE      ( ":hesap_id", hesap_id );
        sql_query.DELETE();
        m_muh_hesap_plani->m_record_updated = true;
        return;
    }

    sql_query.PREPARE_SELECT("muh_hesaplar" , "parent_id" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      ( ":hesap_id", hesap_id );

    if ( sql_query.SELECT() > 0 ) {

        sql_query.NEXT();

        int hesap_parent_id = sql_query.VALUE(0).toInt();

        sql_query.PREPARE_SELECT("muh_hesaplar" , "parent_id, hesap_seviyesi, hesap_kodu, hesap_ismi, tam_hesap_kodu",
                                 "hesap_id = :hesap_id");
        sql_query.SET_VALUE      ( ":hesap_id", hesap_parent_id );
        if ( sql_query.SELECT() > 0 ) {
             sql_query.NEXT();
            int     parent_id       = sql_query.VALUE(0).toInt();
            int     hesap_seviyesi  = sql_query.VALUE(1).toInt();
            QString hesap_kodu      = sql_query.VALUE(2).toString();
            QString hesap_ismi      = sql_query.VALUE(3).toString();
            QString tam_hesap_kodu  = sql_query.VALUE(4).toString();


            SQL_QUERY update_query ( DB );
            update_query.PREPARE_UPDATE ( "muh_hesaplar","hesap_id" ,"parent_id, "
                                          "hesap_seviyesi, hesap_kodu, "
                                          "hesap_ismi , tam_hesap_kodu , "
                                          "tali_hesap","hesap_id = :hesap_id" );

            update_query.SET_VALUE ( ":parent_id"      , parent_id                                              );
            update_query.SET_VALUE ( ":hesap_seviyesi" , hesap_seviyesi                                         );
            update_query.SET_VALUE ( ":hesap_kodu"     , hesap_kodu                                             );
            update_query.SET_VALUE ( ":hesap_ismi"     , hesap_ismi                                             );
            update_query.SET_VALUE ( ":tam_hesap_kodu" , tam_hesap_kodu                                         );
            update_query.SET_VALUE ( ":tali_hesap"     , 1);
            update_query.SET_VALUE ( ":hesap_id"       , hesap_id                                               );

            update_query.UPDATE();

            sql_query.PREPARE_DELETE (  "muh_hesaplar","hesap_id = :hesap_id "  );
            sql_query.SET_VALUE      (  ":hesap_id", hesap_parent_id  );
            sql_query.DELETE();

            QString log_detaylari = tam_hesap_kodu + "  " + hesap_ismi + QObject::tr ( " Hesabı Silindi." );
            E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_HESAP_PLANI , LOG_ISLEM_DELETE , log_detaylari );
        }
    }

    m_muh_hesap_plani->m_record_updated = true;
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::LOCK_FIS_RECORD ( int fis_id )
{
    return DB->LOCK_ROW ( "hesap_id","muh_hesaplar", QString ( "hesap_id = %1" ).arg ( fis_id ) );
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::UNLOCK_FIS_RECORD ( int fis_id )
{
    DB->UNLOCK_ROW (  "hesap_id","muh_hesaplar", QString ( "hesap_id = %1" ).arg ( fis_id  ) );
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::LINE_SAVER_BUTTON_CLICKED
***************************************************************************************/

void MUH_HESAP_EKLEME_FISI::LINE_SAVER_BUTTON_CLICKED ( QAbstractButton *button, int row_id, int , int row_number )
{
   Q_UNUSED ( button );
   Q_UNUSED ( row_id );
   Q_UNUSED ( row_number );
}

/**************************************************************************************
                   MUH_HESAP_EKLEME_FISI::LINE_CHANGER_BUTTON_CLICKED
***************************************************************************************/

int MUH_HESAP_EKLEME_FISI::LINE_CHANGER_BUTTON_CLICKED(QAbstractButton *button, int row_number)
{
    QPushButton * button_grup_secimi      =  ( QPushButton * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, HESAP_GRUP_SECIMI_COLUMN  );
    QLineEdit *   line_edit_gruplar       =  ( QLineEdit   * ) m_ui->tablewidget_alt_hesaplar->cellWidget ( row_number, HESAP_GRUPLAR             );

    if ( button EQ button_grup_secimi ) {
        QList<int> idler;
        if ( m_ui->tablewidget_alt_hesaplar->item(row_number,GRUP_IDLERI) NE NULL ) {
            QStringList item_idleri = m_ui->tablewidget_alt_hesaplar->item(row_number,GRUP_IDLERI)->text().split("\t");
            for (int i = 0 ; i < item_idleri.size() ; i++ ) {
                idler << item_idleri.at(i).toInt();
            }
        }
        if ( OPEN_GRUP_SECIM ( E9_PROGRAMI , MUHASEBE_MODULU , &idler,DB, this ) EQ true ) {
            line_edit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&idler,new QStringList(),new QStringList()));
            QString grup_id_str = "";
            for (int i = 0 ; i < idler.size() ; i++ ) {
                if (grup_id_str.isEmpty() EQ false) {
                    grup_id_str.append("\t");
                }
                grup_id_str .append( QVariant(idler.at(i)).toString() );
            }
            if ( m_ui->tablewidget_alt_hesaplar->item(row_number,GRUP_IDLERI) NE NULL ) {
                m_ui->tablewidget_alt_hesaplar->item(row_number,GRUP_IDLERI)->setText(grup_id_str);
            }
            else {
                m_ui->tablewidget_alt_hesaplar->setItem(row_number,GRUP_IDLERI,new QTableWidgetItem(grup_id_str));
            }
            return ADAK_RECORD_CHANGED;
        }

    }

    return ADAK_RECORD_UNCHANGED;
}
