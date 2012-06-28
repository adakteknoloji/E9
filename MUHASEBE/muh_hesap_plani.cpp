#include  <QMessageBox>
#include "muh_hesap_plani_class.h"
#include "muh_hesap_plani_open.h"
#include "adak_utils.h"
#include "kernel_utils.h"
#include "ui_muh_hesap_plani.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "muh_enum.h"
#include "e9_log.h"
#include "e9_enum.h"
#include "muh_hesap_ekleme_fisi_open.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_MUH_HESEP_PLANI
***************************************************************************************/

void OPEN_MUH_HESAP_PLANI ( QWidget * parent )
{
    MUH_HESAP_PLANI * F = new MUH_HESAP_PLANI ( parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   MUH_HESAP_PLANI::MUH_HESAP_PLANI
***************************************************************************************/

MUH_HESAP_PLANI::MUH_HESAP_PLANI ( QWidget * parent ) : TREE_KERNEL( parent ),m_ui ( new Ui::MUH_HESAP_PLANI )
{
    m_ui->setupUi( this );

    m_ui->combo_box_sirket_turu->setFocusPolicy (Qt::NoFocus);
    m_ui->btn_alt_hesap_ekle ->setFocusPolicy (Qt::NoFocus);
    m_ui->btn_ana_hesap_ekle ->setFocusPolicy (Qt::NoFocus);
    m_ui->btn_hesabi_sil     ->setFocusPolicy (Qt::NoFocus);

    SET_FIRST_FOCUS_WIDGET( m_ui->btn_alt_hesap_ekle );

    m_ui->btn_alt_hesap_ekle->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Insert ));
    m_ui->btn_ana_hesap_ekle->setShortcut( QKeySequence( Qt::Key_F8 ));
    m_ui->btn_hesabi_sil->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Delete ));

    SET_HELP_PAGE    ("genel-muhasebe_hesap-plani" );
    SET_PAGE_TITLE   ( tr( "MUH - HESAP PLANI" ) );

    m_tum_hesaplar_tables       = " toplam_borc_array,toplam_alacak_array ";

    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT ( "e9_sabit_degerler" ,"e9_ayraci" );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr ( "Muhasebe ayracı bulunamadı" ), NULL );
        return;
    }
    else {
        sql_query.NEXT();
        p_delimeter = sql_query.VALUE(0).toString();
    }

    SET_COLUMN_HEADERS          ( QStringList()<<"Hesap Kodu"<<"Hesap Ismi"<<"Alt Hesap Sayısı"<<"Borç Tutarı"<<"Alacak Tutarı"<<"id" );

    REGISTER_TREE_WIDGET        ( m_ui->tree_widget_hesap_plani );
    REGISTER_ADD_BUTTON         ( m_ui->btn_alt_hesap_ekle );
    REGISTER_ADD_BUTTON         ( m_ui->btn_ana_hesap_ekle );
    REGISTER_DELETE_BUTTON      ( m_ui->btn_hesabi_sil );

    SET_RIGHT_ALIGN_COLUMNS     ( QList<int>() << 3 << 4 );

    m_ui->btn_alt_hesap_ekle -> setIcon ( QIcon ( ":/kernel_icons/kernel_alt_grup_ekleme_ikonu.png" ) );
    m_ui->btn_alt_hesap_ekle -> setIconSize ( QSize ( 32,32 ) );

    m_ui->btn_ana_hesap_ekle -> setIcon ( QIcon ( ":/kernel_icons/kernel_ana_grup_ekleme_ikonu.png" ) );
    m_ui->btn_ana_hesap_ekle -> setIconSize ( QSize ( 32,32 ) );

    m_ui->btn_hesabi_sil     -> setIcon ( QIcon ( ":/kernel_icons/kernel_sil_ikonu.png" ) );
    m_ui->btn_hesabi_sil     -> setIconSize ( QSize ( 32,32 ) );

    INIT_KERNEL( this,DB );

    HIDE_COLUMN( 5 );

    m_tree_widget->setColumnWidth ( 0,  350 );
    m_tree_widget->setColumnWidth ( 1,  700 );
    m_tree_widget->setColumnWidth ( 2,  150 );
    m_tree_widget->setColumnWidth ( 3,  190 );
    m_tree_widget->setColumnWidth ( 4,  190 );


    SET_SETTING_NAME            ( "MUH_HESAP_PLANI" );

    SET_ACTION_MENU_TEXT        ( "Alt Hesap Ekle", "Hesabı Güncelleştir","Hesabı Sil" );

    connect(m_ui->combo_box_sirket_turu , SIGNAL(currentIndexChanged(int)) , this , SLOT (BAKIYE_TURU_COMBO_BOX_CHANGED()));

    m_ui->tree_widget_hesap_plani->setFocus();
}

/**************************************************************************************
                   MUH_HESAP_PLANI::~MUH_HESAP_PLANI
***************************************************************************************/

MUH_HESAP_PLANI::~MUH_HESAP_PLANI ()
{
    delete m_ui;
}

/**************************************************************************************
                   MUH_HESAP_PLANI::ADD_ITEM
***************************************************************************************/

void MUH_HESAP_PLANI::ADD_ITEM (QObject *button, QStringList column_datas)
{
    QString        tam_hesap_kodu;

    int item_id = -1;

    if ( column_datas.size() NE 0 ) {
        item_id = column_datas.at( 5 ).toInt();
    }
    else {
        MSG_INFO("Alt hesap ekleyebilmeniz için önce ana grup seçmeniz gerekmektedir.",NULL);
        return;
    }

    SQL_QUERY query          ( DB );

    int hesap_modu = ENUM_ALT_HESAP;

    if ( button EQ m_ui->btn_ana_hesap_ekle ) {
        hesap_modu = ENUM_ANA_HESAP;
    }

    query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu, hesap_id, hesap_seviyesi",
                         "hesap_id = :hesap_id");

    query.SET_VALUE      ( ":hesap_id",item_id );

    if ( query.SELECT() > 0 ) {

        query.NEXT();
        tam_hesap_kodu        = query.VALUE(0).toString();
    }

    if ( hesap_modu EQ ENUM_ALT_HESAP ) {
        if (  OPEN_MUH_HESAP_EKLEME_FISI ( item_id, p_delimeter, this ) EQ false  ) {
            return;
        }
    }
    else {
        if (  OPEN_MUH_HESAP_EKLEME_FISI ( -1, p_delimeter, this ) EQ false  ) {
            return;
        }
    }

    if (  hesap_modu EQ ENUM_ALT_HESAP ) {
        REFRESH_TREE_WIDGET();
    }
    else {
        ADD_PARENT_ITEM ( m_added_record,0 );
        REFRESH_TREE_WIDGET();
    }

}

/**************************************************************************************
                   MUH_HESAP_PLANI::ADD_PARENT_ITEMS
***************************************************************************************/

void MUH_HESAP_PLANI::ADD_PARENT_ITEMS ()
{
    QString column_names = "hesap_id, parent_id, hesap_seviyesi, hesap_kodu,"
                           "hesap_ismi,alt_hesap_sayisi,";

    column_names += m_tum_hesaplar_tables;

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT  ( "muh_hesaplar" , column_names,"parent_id = :parent_id" );
    sql_query.SET_VALUE(":parent_id" , 0);

    if ( sql_query.SELECT("tam_hesap_kodu ASC") > 0 ) {

        while ( sql_query.NEXT() EQ true ) {

           QString alt_hesap_sayisi = " ";

           if ( sql_query.VALUE(5).toInt() NE 0 ) {
               alt_hesap_sayisi = sql_query.VALUE(5).toString();
           }

           double  BORC_BAKIYESI   [ E9_ARRAY_SIZE ];
           double  ALACAK_BAKIYESI [ E9_ARRAY_SIZE ];

           UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(6).toString()    ,BORC_BAKIYESI     ,E9_ARRAY_SIZE );
           UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(7).toString()    ,ALACAK_BAKIYESI   ,E9_ARRAY_SIZE );

           QStringList hesap;

           hesap << sql_query.VALUE(3).toString() << sql_query.VALUE(4).toString() << alt_hesap_sayisi <<  VIRGUL_EKLE ( QVariant ( BORC_BAKIYESI[E9_SON_BAKIYE ] ).toString() )+" "
                 << VIRGUL_EKLE ( QVariant ( ALACAK_BAKIYESI [ E9_SON_BAKIYE ] ).toString())+" " <<  sql_query.VALUE(0).toString() ;

           ADD_PARENT_ITEM ( hesap,alt_hesap_sayisi.toInt() );
       }
    }
}

/**************************************************************************************
                   MUH_HESAP_PLANI::ADD_CHILD_ITEMS
***************************************************************************************/

void MUH_HESAP_PLANI::ADD_CHILD_ITEMS (QStringList column_datas)
{
    int item_id;

    item_id = column_datas.at(5).toInt();

    QString column_names = "hesap_id, parent_id, hesap_seviyesi, hesap_kodu, hesap_ismi,alt_hesap_sayisi,";

    column_names += m_tum_hesaplar_tables;

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT  ( "muh_hesaplar" , column_names ,"parent_id = :parent_id");
    sql_query.SET_VALUE(":parent_id" , item_id);

    if ( sql_query.SELECT("tam_hesap_kodu ASC") > 0 ) {
        while ( sql_query.NEXT() EQ true ) {

           QString alt_hesap_sayisi = " ";

           if ( sql_query.VALUE(5).toInt() NE 0 ) {
               alt_hesap_sayisi = sql_query.VALUE(5).toString();
           }

           double  BORC_BAKIYESI   [ E9_ARRAY_SIZE ];
           double  ALACAK_BAKIYESI [ E9_ARRAY_SIZE ];

           UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(6).toString()    ,BORC_BAKIYESI     ,E9_ARRAY_SIZE );
           UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(7).toString()    ,ALACAK_BAKIYESI   ,E9_ARRAY_SIZE );

           QStringList hesap;

           hesap << sql_query.VALUE(3).toString() << sql_query.VALUE(4).toString() << alt_hesap_sayisi <<  VIRGUL_EKLE ( QVariant ( BORC_BAKIYESI[E9_SON_BAKIYE ] ).toString() )+" "
                 << VIRGUL_EKLE ( QVariant ( ALACAK_BAKIYESI [ E9_SON_BAKIYE ] ).toString())+" " <<  sql_query.VALUE(0).toString() ;

           ADD_CHILD_ITEM( hesap,alt_hesap_sayisi.toInt() );
       }
    }
}

/**************************************************************************************
                   MUH_HESAP_PLANI::UPDATE_ITEM
***************************************************************************************/

void MUH_HESAP_PLANI::UPDATE_ITEM (QStringList column_datas)
{
    int item_id = -1;

    if ( column_datas.size() NE 0 ) {
        item_id = column_datas.at( 5 ).toInt();
    }

    m_updated_record.clear();

    if ( OPEN_MUH_HESAP_EKLEME_FISI ( item_id, p_delimeter, this ) EQ false ) {
        return;
    }

    if (m_updated_record.isEmpty() EQ false) {
        UPDATE_SELECTED_ITEM ( m_updated_record );
    }
}

/**************************************************************************************
                   MUH_HESAP_PLANI::DELETE_ITEM
***************************************************************************************/

void MUH_HESAP_PLANI::DELETE_ITEM (QStringList column_datas)
{
    int item_id = -1;

    if ( column_datas.size() NE 0 ) {
        item_id = column_datas.at( 5 ).toInt();
    }

    SQL_QUERY sql_query  ( DB );

    DB->START_TRANSACTION();

    QString     hesap_kodu;
    QString     tam_hesap_kodu;
    QString     hesap_ismi;
    int         parent_id       = 0;
    int         hesap_seviyesi  = 0;

    sql_query.PREPARE_SELECT("muh_hesaplar" , "parent_id,tam_hesap_kodu,hesap_ismi" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      ( ":hesap_id", item_id );

    if ( sql_query.SELECT() NE 0 ) {
         sql_query.NEXT();
        if ( sql_query.VALUE(0).toInt() > 0 ) {

            int alt_hesap_sayisi = 0;

            parent_id      = sql_query.VALUE(0).toInt();
            tam_hesap_kodu = sql_query.VALUE(1).toString();
            hesap_ismi     = sql_query.VALUE(2).toString();

            sql_query.PREPARE_SELECT("muh_hesaplar" , "alt_hesap_sayisi" , "hesap_id = :hesap_id");

            sql_query.SET_VALUE      ( ":hesap_id",parent_id );
            if ( sql_query.SELECT() NE 0 ) {
                sql_query.NEXT();
                alt_hesap_sayisi = sql_query.VALUE(0).toInt();
                alt_hesap_sayisi--;
            }

            sql_query.PREPARE_UPDATE ( "muh_hesaplar","hesap_id", "alt_hesap_sayisi", "hesap_id = :hesap_id" );
            sql_query.SET_VALUE      ( ":alt_hesap_sayisi" , alt_hesap_sayisi );
            sql_query.SET_VALUE      ( ":hesap_id "        , parent_id );
            sql_query.UPDATE();
        }
    }
    if ( p_hesabin_parentini_aktar EQ false ) {

        QString log_detaylari = QObject::tr ( "Log Türü - Hesap Planı, " ) + tam_hesap_kodu + "  " + hesap_ismi;


        E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_HESAP_PLANI, LOG_ISLEM_DELETE, log_detaylari );

        sql_query.PREPARE_DELETE ( "muh_hesaplar", "hesap_id = :hesap_id " );
        sql_query.SET_VALUE      ( ":hesap_id", item_id );
        sql_query.DELETE();
        m_record_updated = true;
        ADD_VALUE_TO_DELETED_ITEM_PARENT(2,-1);
        DB->COMMIT_TRANSACTION();
        return;
    }

    sql_query.PREPARE_SELECT("muh_hesaplar" ,"parent_id" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      ( ":hesap_id", item_id );

    if ( sql_query.SELECT() > 0 ) {

        sql_query.NEXT();

        int hesap_parent_id = sql_query.VALUE(0).toInt();

        sql_query.PREPARE_SELECT("muh_hesaplar" , "parent_id, hesap_seviyesi, hesap_kodu, hesap_ismi, tam_hesap_kodu",
                                 "hesap_id = :hesap_id");

        sql_query.SET_VALUE      ( ":hesap_id", hesap_parent_id  );
        if ( sql_query.SELECT() > 0 ) {

            sql_query.NEXT();

            parent_id       =   sql_query.VALUE(0).toInt();
            hesap_seviyesi  =   sql_query.VALUE(1).toInt();
            hesap_kodu      =   sql_query.VALUE(2).toString();
            hesap_ismi      =   sql_query.VALUE(3).toString();
            tam_hesap_kodu  =   sql_query.VALUE(4).toString();

            SQL_QUERY update_query ( DB );
            update_query.PREPARE_UPDATE ( "muh_hesaplar","hesap_id", "parent_id , "
                                          "hesap_seviyesi , hesap_kodu , "
                                          "hesap_ismi , tam_hesap_kodu , "
                                          "tali_hesap",
                                          "hesap_id = :hesap_id" );

            update_query.SET_VALUE ( ":parent_id"      , parent_id      );
            update_query.SET_VALUE ( ":hesap_seviyesi" , hesap_seviyesi );
            update_query.SET_VALUE ( ":hesap_kodu"     , hesap_kodu     );
            update_query.SET_VALUE ( ":hesap_ismi"     , hesap_ismi     );
            update_query.SET_VALUE ( ":tam_hesap_kodu" , tam_hesap_kodu );
            update_query.SET_VALUE ( ":tali_hesap"     , 1);
            update_query.SET_VALUE ( ":hesap_id"       , item_id        );

            update_query.UPDATE();

            sql_query.PREPARE_DELETE ( "muh_hesaplar","hesap_id = :hesap_id " );
            sql_query.SET_VALUE      ( ":hesap_id", hesap_parent_id );
            sql_query.DELETE();
        }
    }

    m_record_updated = true;

    DB->COMMIT_TRANSACTION();

    ADD_VALUE_TO_DELETED_ITEM_PARENT(2,-1);
}

/**************************************************************************************
                   MUH_HESAP_PLANI::CHECK_DELETE_ITEM
***************************************************************************************/

int MUH_HESAP_PLANI::CHECK_DELETE_ITEM (QStringList column_datas)
{
    int item_id                 = -1;

    p_hesabin_parentini_aktar   = false;

    if ( column_datas.size() NE 0 ) {
        item_id = column_datas.at( 5 ).toInt();
    }

    int alt_hesap_sayisi = 0;
    int parent_id        = 0;

    QMessageBox * mesaj = new QMessageBox ( this ) ;
    mesaj->setButtonText ( QMessageBox::Ok, tr ( "Devam" ) );
    QPushButton * buton_cancel = mesaj->addButton ( QMessageBox::Cancel );
    mesaj->setButtonText ( QMessageBox::Cancel, tr ( "Vazgeç" ) ) ;

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      ( ":hesap_id", item_id );
    if (sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();


        mesaj->setWindowTitle ( tr ( "DİKKAT" ) );
        mesaj->setText ( tr ("'%1 %2'` hesabı silinecek." ).arg ( sql_query.VALUE(0).toString() ).arg ( sql_query.VALUE(1).toString() ) ) ;
        mesaj->setInformativeText ( "Emin misiniz?" );
        mesaj->exec();

        if  ( mesaj->clickedButton() EQ buton_cancel ) {
            return ADAK_FAIL;
        }
    }

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id" , "parent_id = :parent_id");

    sql_query.SET_VALUE      ( ":parent_id", item_id );
    if (sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        alt_hesap_sayisi = sql_query.NUM_OF_ROWS();
    }


    if ( alt_hesap_sayisi NE 0 ) {
        mesaj->setText ( tr ( "Hesabı silmek için,önce alt hesaplarını silmelisiniz!.." ) );
        mesaj->removeButton ( mesaj->button ( QMessageBox::Cancel ) );
        mesaj->setInformativeText("");
        mesaj->setButtonText ( QMessageBox::Ok, tr ( "Tamam" ) );
        mesaj->exec();
        return ADAK_FAIL;
    }


    sql_query.PREPARE_SELECT("muh_hesaplar" , "parent_id" , "hesap_id = :hesap_id");
    sql_query.SET_VALUE      ( ":hesap_id", item_id );

    if (sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();
        parent_id = sql_query.VALUE(0).toInt();
    }

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id" , "parent_id = :parent_id");
    sql_query.SET_VALUE      ( ":parent_id", parent_id );

    if (sql_query.SELECT() NE 0) {
        int hesap_sayisi = sql_query.NUM_OF_ROWS();

        if ( hesap_sayisi EQ 1 ) {
            p_hesabin_parentini_aktar = true;
            return ADAK_OK;
        }

        sql_query.PREPARE_SELECT("muh_fis_satirlari" , "fis_id" , "hesap_id = :hesap_id");
        sql_query.SET_VALUE(":hesap_id" , item_id);
        if ( sql_query.SELECT() NE 0 ) {
            MSG_WARNING( tr ( "Hesap işlem görmüş,silinemez!.." ), NULL );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_HESAP_PLANI::BAKIYE_TURU_COMBO_BOX_CHANGED
***************************************************************************************/

void MUH_HESAP_PLANI::BAKIYE_TURU_COMBO_BOX_CHANGED()
{
    REFRESH_TREE_WIDGET();
}
