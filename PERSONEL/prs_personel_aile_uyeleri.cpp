#include <QHeaderView>
#include "prs_personel_aile_uyeleri_class.h"
#include "prs_personel_aile_uyeleri_open.h"
#include "print.h"
#include "prs_con_utils.h"
#include "e9_log.h"
#include "cari_console_utils.h"
#include "adak_std_utils.h"
 
#include "cari_kart_arama_open.h"
#include "cari_struct.h"

extern ADAK_SQL * DB;


#define ROW_ID_COLUMN                   0
#define RECORD_ID_COLUMN                1
#define ORDER_COLUMN                    2
#define AD_SOYAD_COLUMN                 3
#define TC_KIMLIK_NO_COLUMN             4
#define AKRABALIK_DERECESI_COLUMN       5
#define CALISMA_DURUMU_COLUMN           6
#define DOGUM_TARIHI_COLUMN             7
#define EGITIM_DURUMU_COLUMN            8
#define CINSIYET_COLUMN                 9
#define ASGARI_GECIM_IND_DURUMU_COLUMN  10
#define ACIKLAMA_COLUMN                 11

/**************************************************************************************
                   SHOW_PRS_PERSONEL_AILE_FISI
***************************************************************************************/

void SHOW_PRS_PERSONEL_AILE_FISI( int record_id , QWidget * parent )
{
    PRS_PERSONEL_AILE_FISI * F = new PRS_PERSONEL_AILE_FISI(record_id, parent);

    F->EXEC( NOT_FULL_SCREEN );
}
/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::PRS_PERSONEL_AILE_FISI
***************************************************************************************/

PRS_PERSONEL_AILE_FISI::PRS_PERSONEL_AILE_FISI(int record_id, QWidget * parent) :FIS_KERNEL(parent)
{
    m_record_id = record_id;

    setupUi             ( this );
    START_FIS_KERNEL    ( this, DB );
 }

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::SETUP_FORM
**************************************************************************************/

void PRS_PERSONEL_AILE_FISI::SETUP_FORM()
{
    SET_HELP_PAGE ( "personel-sicil-karti" );
    SET_PAGE_TITLE ( tr ( "PRS - AİLE FİŞİ" ) );

    int num_of_columns   = 12;
    SET_SINGLE_RECORD_ID              (m_record_id);

    REGISTER_BUTTONS_WIDGET           ( navigation_button_widget  );

    REGISTER_TABLE_WIDGET             ( table_widget_aile_satirlari , num_of_columns );

    SET_FORM_ID_COLUMN_POSITION       ( RECORD_ID_COLUMN );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN  );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN       ( 3 );

    SET_FIS_ORDER_DATABASE ( "prs_personel_aile_bilgileri", "order_number", "aile_bilgi_id" );

    SET_TABLE_ROW_WIDGETS ( AD_SOYAD_COLUMN                 , WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS ( TC_KIMLIK_NO_COLUMN             , WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS ( AKRABALIK_DERECESI_COLUMN       , WIDGET_COMBO_BOX );
    SET_TABLE_ROW_WIDGETS ( CALISMA_DURUMU_COLUMN           , WIDGET_COMBO_BOX );
    SET_TABLE_ROW_WIDGETS ( DOGUM_TARIHI_COLUMN             , WIDGET_ADAK_DATE);
    SET_TABLE_ROW_WIDGETS ( EGITIM_DURUMU_COLUMN            , WIDGET_COMBO_BOX );
    SET_TABLE_ROW_WIDGETS ( CINSIYET_COLUMN                 , WIDGET_COMBO_BOX );
    SET_TABLE_ROW_WIDGETS ( ASGARI_GECIM_IND_DURUMU_COLUMN  , WIDGET_COMBO_BOX );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN                 , WIDGET_LINE_EDIT );

    table_widget_aile_satirlari->setHorizontalHeaderLabels(QStringList()<<"id"<<"record_id"<<"satir_order"
                                                           <<tr("Ad Soyad") <<tr("TC Kimlik No")<<tr("Akrabalık\n Derecesi")<<tr("Çalışma Durumu")
                                                           <<tr("Doğum Tarihi")<<tr("Eğitim Düzeyi")
                                                           <<tr("Cinsiyet") << tr("Asgari Geçim \n İndirimi")<<tr("Açıklama")<<tr("")<<tr("") );

    table_widget_aile_satirlari->setColumnWidth ( AD_SOYAD_COLUMN                   , 150  );
    table_widget_aile_satirlari->setColumnWidth ( TC_KIMLIK_NO_COLUMN               , 150  );
    table_widget_aile_satirlari->setColumnWidth ( AKRABALIK_DERECESI_COLUMN         , 140  );
    table_widget_aile_satirlari->setColumnWidth ( CALISMA_DURUMU_COLUMN             , 180  );
    table_widget_aile_satirlari->setColumnWidth ( DOGUM_TARIHI_COLUMN               , 250  );
    table_widget_aile_satirlari->setColumnWidth ( EGITIM_DURUMU_COLUMN              , 150  );
    table_widget_aile_satirlari->setColumnWidth ( CINSIYET_COLUMN                   , 120  );
    table_widget_aile_satirlari->setColumnWidth ( ASGARI_GECIM_IND_DURUMU_COLUMN    , 140  );
    table_widget_aile_satirlari->setColumnWidth ( ACIKLAMA_COLUMN                   , 150  );

    SET_SETTING_NAME ( "PRS_PERSONEL_VE_AILE_FISI" );

    SET_FIRST_FOCUS_WIDGET ( table_widget_aile_satirlari );

    FOCUS_FIRST_WIDGET();
    SET_WIDGETS_MAXIMUM_SIZE      ();

    line_edit_personel_sicil_no->setReadOnly(true);
    line_edit_personel_adi->setReadOnly(true);
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void PRS_PERSONEL_AILE_FISI::CLEAR_FORM_MEMBERS()
{
}

/*************************************************************************************
                   PRS_PERSONEL_AILE_FISI::NEW_FIS_RECORD
*************************************************************************************/

void PRS_PERSONEL_AILE_FISI::NEW_FIS_RECORD()
{

}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::GET_FIS_RECORD
**************************************************************************************/

int PRS_PERSONEL_AILE_FISI::GET_FIS_RECORD ( int record_id )
{
    SQL_QUERY   sql_query_personel (DB);
    SQL_QUERY   query              (DB);


    int cari_hesap_id =  0 ;
    sql_query_personel.PREPARE_SELECT( "prs_personeller ","sicil_no, cari_hesap_id","personel_id = :personel_id" );

    sql_query_personel.SET_VALUE(":personel_id", record_id);

    if ( sql_query_personel.SELECT() NE 0 ) {

        sql_query_personel.NEXT();

        line_edit_personel_sicil_no->setText ( sql_query_personel.VALUE(0).toString() );
        cari_hesap_id     =   sql_query_personel.VALUE( 1 ).toInt();
    }

    CARI_HESAP_STRUCT hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri  );

    line_edit_personel_adi->setText         ( hesap_bilgileri.cari_hesap_ismi);

    query.PREPARE_SELECT ( "prs_personel_aile_uyeleri ",
                           "aile_bilgi_id , personel_id, order_number, aciklama, "
                           "adi_soyadi,  tc_kimlik_no,akrabalik_derecesi,dogum_tarihi, "
                           "ogrenim_durumu, calisma_durumu,"
                           "cinsiyet, asgari_gecim_ind_durumu ",
                           "personel_id = :personel_id ");

    query.SET_VALUE      ( ":personel_id",  record_id );

    if ( query.SELECT("order_number ASC" ) NE 0 ) {

        while ( query.NEXT() EQ true ) {

            int p_current_row;

            p_current_row = ADD_NEW_LINE ();

            QWidget    *   cell_widget = table_widget_aile_satirlari->cellWidget( p_current_row, AD_SOYAD_COLUMN );
            QLineEdit  *   line_edit_ad_soyad    = static_cast < QLineEdit * > ( cell_widget );
            line_edit_ad_soyad->setText( query.VALUE("adi_soyadi").toString() );

            cell_widget = table_widget_aile_satirlari->cellWidget( p_current_row, TC_KIMLIK_NO_COLUMN );
            QLineEdit  *   line_edit_tc_kimlik_no    = static_cast < QLineEdit * > ( cell_widget );
            line_edit_tc_kimlik_no->setText( query.VALUE("tc_kimlik_no").toString() );

            cell_widget = table_widget_aile_satirlari->cellWidget( p_current_row, AKRABALIK_DERECESI_COLUMN );
            QComboBox  *   combo_box_akrabalik_derecesi    = static_cast < QComboBox * > ( cell_widget );

            FIND_AND_SET_COMBOBOX_TEXT(combo_box_akrabalik_derecesi ,
                                       PRS_GET_AKRABALIK_DERECESI_STRING(query.VALUE("akrabalik_derecesi").toInt()));

            cell_widget = table_widget_aile_satirlari->cellWidget( p_current_row, CALISMA_DURUMU_COLUMN );
            QComboBox  *   combo_box_calisma_durumu    = static_cast < QComboBox * > ( cell_widget );

            FIND_AND_SET_COMBOBOX_TEXT(combo_box_calisma_durumu , PRS_GET_CALISMA_DURUMU_STRING(query.VALUE("calisma_durumu").toInt()));

            cell_widget = table_widget_aile_satirlari->cellWidget( p_current_row, DOGUM_TARIHI_COLUMN);
            QAdakDate * adakDate_dogum_tarihi = static_cast<QAdakDate*> (cell_widget);
            adakDate_dogum_tarihi->SET_DATE( query.VALUE("dogum_tarihi").toString());

            cell_widget = table_widget_aile_satirlari->cellWidget( p_current_row, EGITIM_DURUMU_COLUMN );
            QComboBox  *   combo_box_egitim_durumu    = static_cast < QComboBox * > ( cell_widget );

            FIND_AND_SET_COMBOBOX_TEXT(combo_box_egitim_durumu , query.VALUE("ogrenim_durumu").toString());

            cell_widget = table_widget_aile_satirlari->cellWidget( p_current_row, CINSIYET_COLUMN );
            QComboBox  *   combo_box_cinsiyet    = static_cast < QComboBox * > ( cell_widget );

            FIND_AND_SET_COMBOBOX_TEXT(combo_box_cinsiyet , query.VALUE("cinsiyet").toString());

            cell_widget = table_widget_aile_satirlari->cellWidget( p_current_row, ACIKLAMA_COLUMN );
            QLineEdit  *   line_edit_aciklama    = static_cast < QLineEdit * > ( cell_widget );
            line_edit_aciklama->setText( query.VALUE("aciklama").toString() );

            cell_widget = table_widget_aile_satirlari->cellWidget( p_current_row, ASGARI_GECIM_IND_DURUMU_COLUMN );
            QComboBox  *   combo_box_asagari_gecim_ind_durumu  = static_cast < QComboBox * > ( cell_widget );

            int asgari_gecim_ind_durumu = query.VALUE( "asgari_gecim_ind_durumu" ).toInt();

            FIND_AND_SET_COMBOBOX_TEXT( combo_box_asagari_gecim_ind_durumu , PRS_GET_EVET_HAYIR_DURUMU_STRING(asgari_gecim_ind_durumu));


            QString id                      = query.VALUE("aile_bilgi_id").toString();
            QString personel_id             = query.VALUE("personel_id").toString();
            QString order_number            = query.VALUE("order_number").toString();


            QTableWidgetItem * new_item;

            table_widget_aile_satirlari->setHorizontalHeaderLabels(QStringList()<<"id"<<"record_id"<<"satir_order"
                                                                    <<tr("Ad Soyad")<<tr("TC Kimlik No")<<tr("Akrabalık Derecesi")<<("Çalışma Durumu")
                                                                    <<tr("Doğum Tarihi")<<tr("Eğitim Düzeyi")
                                                                    <<tr("Cinsiyet")<< tr("Asgari Geçim \n İndirimi") << tr("Açıklama")<<tr("")<<tr("") );

            new_item = new QTableWidgetItem(tr("%1").arg(id));
            table_widget_aile_satirlari->setItem(p_current_row, ROW_ID_COLUMN, new_item);

            new_item = new QTableWidgetItem(tr("%1").arg(personel_id));
            table_widget_aile_satirlari->setItem(p_current_row, RECORD_ID_COLUMN, new_item);

            new_item = new QTableWidgetItem(tr("%1").arg(order_number));
            table_widget_aile_satirlari->setItem(p_current_row, ORDER_COLUMN, new_item);

        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void PRS_PERSONEL_AILE_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QWidget   * cell_widget;

    QComboBox * combobox_akrabalik_derecesi;
    QComboBox * combobox_egitim_durumu;
    QComboBox * combobox_cinsiyet;
    QComboBox * combobox_calisma_durumu;

    QStringList akrabalik_derecesi;
    QStringList calisma_durumu;

    QLineEdit  * line_edit_adi_soyadi   = ((QLineEdit *) table_widget_aile_satirlari->cellWidget(row_number, AD_SOYAD_COLUMN ));
    QLineEdit  * line_edit_tc_kimlik_no = ((QLineEdit *) table_widget_aile_satirlari->cellWidget(row_number, TC_KIMLIK_NO_COLUMN ));
    QLineEdit  * line_edit_aciklama     = ((QLineEdit *) table_widget_aile_satirlari->cellWidget(row_number, ACIKLAMA_COLUMN ));
    QAdakDate * adak_date_dogum_tarihi= ((QAdakDate *) table_widget_aile_satirlari->cellWidget(row_number, DOGUM_TARIHI_COLUMN ));

    adak_date_dogum_tarihi->MALI_YIL_KISITI_YOK(true);

    line_edit_aciklama->setMaxLength( 512 );
    line_edit_adi_soyadi->setMaxLength( 100 );
    line_edit_tc_kimlik_no->setMaxLength( 11 );

    akrabalik_derecesi << tr("Eş")<< tr("Çocuk")<< tr("Anne") << tr("Baba");
    cell_widget                   = table_widget_aile_satirlari->cellWidget(row_number, AKRABALIK_DERECESI_COLUMN);
    combobox_akrabalik_derecesi   = static_cast <QComboBox*> (cell_widget);
    combobox_akrabalik_derecesi->addItems(akrabalik_derecesi);

    cell_widget                   = table_widget_aile_satirlari->cellWidget(row_number, EGITIM_DURUMU_COLUMN);
    combobox_egitim_durumu        = static_cast <QComboBox*> (cell_widget);
    GET_OGRENIM_DURUMLARI(combobox_egitim_durumu);


    cell_widget              = table_widget_aile_satirlari->cellWidget(row_number, CINSIYET_COLUMN);
    combobox_cinsiyet        = static_cast <QComboBox*> (cell_widget);

    GET_CINSIYETLER(combobox_cinsiyet);

    calisma_durumu << tr("Çalışıyor") << tr("Çalışmıyor") << tr("Öğrenci");
    cell_widget              = table_widget_aile_satirlari->cellWidget(row_number, CALISMA_DURUMU_COLUMN);
    combobox_calisma_durumu  = static_cast <QComboBox*> (cell_widget);
    combobox_calisma_durumu->addItems(calisma_durumu);


    QStringList asgari_gecim_indirimi_durumu ;
    asgari_gecim_indirimi_durumu << tr("Evet") << tr("Hayır");

    QComboBox * combo_box_asg_gecim_ind_durumu = ( QComboBox * ) table_widget_aile_satirlari->cellWidget(row_number, ASGARI_GECIM_IND_DURUMU_COLUMN );

    combo_box_asg_gecim_ind_durumu->addItems( asgari_gecim_indirimi_durumu );

}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    Q_UNUSED ( object );
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_FIS_FORM_EMPTY()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_FIS_RECORD_ADD
**************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_FIS_RECORD_ADD()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::ADD_FIS_RECORD
**************************************************************************************/

int PRS_PERSONEL_AILE_FISI::ADD_FIS_RECORD()
{
    return 1;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_FIS_RECORD_UPDATE(int record_id)
{
    Q_UNUSED(record_id);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::UPDATE_FIS_RECORD
**************************************************************************************/

void PRS_PERSONEL_AILE_FISI::UPDATE_FIS_RECORD(int record_id)
{
    Q_UNUSED(record_id);
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    Q_UNUSED(record_id);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void PRS_PERSONEL_AILE_FISI::DELETE_FIS_RECORD ( int record_id )
{
    Q_UNUSED(record_id);
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::SELECT_FIS_RECORD
**************************************************************************************/

int PRS_PERSONEL_AILE_FISI::SELECT_FIS_RECORD()
{
    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA, this, 0, E9_PROGRAMI, PERSONEL_MODULU , "Personel Arama", "DELETED" );


    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT("prs_personeller","personel_id",
                             "cari_hesap_id = :cari_hesap_id");

    sql_query.SET_VALUE( ":cari_hesap_id"   , cari_hesap_id );

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    return -1;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::FIND_FIS_RECORD
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::FIND_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::FIND_FIRST_FIS_RECORD
**************************************************************************************/

int PRS_PERSONEL_AILE_FISI::FIND_FIRST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::FIND_LAST_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::FIND_PREV_FIS_RECORD
**************************************************************************************/

int PRS_PERSONEL_AILE_FISI::FIND_PREV_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::FIND_NEXT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_LINE_VAR
**************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{
    Q_UNUSED(row_number);
    Q_UNUSED(object);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_LINE_EMPTY(int row_number)
{
    Q_UNUSED(row_number);
    return ADAK_OK;
}


/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_ADD_LINE
**************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_ADD_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::ADD_LINE
***************************************************************************************/

void PRS_PERSONEL_AILE_FISI::ADD_LINE(int record_id, int row_number)
{
    SQL_QUERY   sql_query(DB);

    QComboBox * combobox_akrabalik_derecesi = ( QComboBox * ) table_widget_aile_satirlari->cellWidget(row_number , AKRABALIK_DERECESI_COLUMN);
    QComboBox * combobox_egitim_durumu      = ( QComboBox * ) table_widget_aile_satirlari->cellWidget(row_number , EGITIM_DURUMU_COLUMN );
    QComboBox * combobox_calisma_durumu     = ( QComboBox * ) table_widget_aile_satirlari->cellWidget(row_number , CALISMA_DURUMU_COLUMN );
    QComboBox * combobox_cinsiyet           = ( QComboBox * ) table_widget_aile_satirlari->cellWidget(row_number , CINSIYET_COLUMN);

    int     order_number                            = table_widget_aile_satirlari->item ( row_number, ORDER_COLUMN )->text().toInt();
    QString adi_soyadi                              = ((QLineEdit *) table_widget_aile_satirlari->cellWidget(row_number, AD_SOYAD_COLUMN ))->text();
    QString tc_kimlik_no                            = ((QLineEdit *) table_widget_aile_satirlari->cellWidget(row_number, TC_KIMLIK_NO_COLUMN ))->text();
    QString aciklama                                = ((QLineEdit *) table_widget_aile_satirlari->cellWidget(row_number, ACIKLAMA_COLUMN ))->text();
    QComboBox  * combo_box_asgari_gecim_ind_durumu  = (QComboBox  *) table_widget_aile_satirlari->cellWidget(row_number, ASGARI_GECIM_IND_DURUMU_COLUMN );




    QWidget   * cell_widget          = table_widget_aile_satirlari->cellWidget( row_number, DOGUM_TARIHI_COLUMN);
    QDateEdit * date_edit            = static_cast < QDateEdit * > ( cell_widget );
    QString     dogum_tarihi         = date_edit->date().toString("yyyy.MM.dd");


    sql_query.PREPARE_INSERT("prs_personel_aile_uyeleri","aile_bilgi_id",
                             "personel_id, order_number, aciklama, adi_soyadi, tc_kimlik_no, "
                             "akrabalik_derecesi, dogum_tarihi, "
                             "ogrenim_durumu, calisma_durumu, "
                             "cinsiyet, asgari_gecim_ind_durumu ");

    sql_query.SET_VALUE(":personel_id"              , record_id                 );
    sql_query.SET_VALUE(":order_number"             , order_number              );
    sql_query.SET_VALUE(":aciklama"                 , aciklama                  );
    sql_query.SET_VALUE(":adi_soyadi"               , adi_soyadi                );
    sql_query.SET_VALUE(":tc_kimlik_no"             , tc_kimlik_no              );
    sql_query.SET_VALUE(":akrabalik_derecesi"       , PRS_GET_AKRABALIK_DERECESI_ENUM(combobox_akrabalik_derecesi->currentText())   );
    sql_query.SET_VALUE(":dogum_tarihi"             , dogum_tarihi              );
    sql_query.SET_VALUE(":ogrenim_durumu"       , combobox_egitim_durumu->currentText()       );
    sql_query.SET_VALUE(":calisma_durumu"           , PRS_GET_CALISMA_DURUMU_ENUM(combobox_calisma_durumu->currentText())     );
    sql_query.SET_VALUE(":cinsiyet"                 , combobox_cinsiyet->currentText()           );
    sql_query.SET_VALUE(":asgari_gecim_ind_durumu"  , PRS_GET_EVET_HAYIR_DURUMU_ENUM(combo_box_asgari_gecim_ind_durumu->currentText())  );

    int satir_id = sql_query.INSERT();

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( satir_id ).toString() );
    table_widget_aile_satirlari->setItem ( row_number, ROW_ID_COLUMN, new_item );

    QString personel_adi_soyadi = line_edit_personel_adi->text();
    QString personel_sicil_no   = line_edit_personel_sicil_no->text();

    QString log_detaylari = "Aile Bireyi Ekleme";

    log_detaylari.append("\nPersonel Adı / Soyadı : " + personel_adi_soyadi +
                         "Sicil No : " + personel_sicil_no);
    log_detaylari.append("\nAile Bireyi : " + adi_soyadi);

    E9_LOG_KAYDI_EKLE ( PERSONEL_MODULU , LOG_PRS_PERSONEL , LOG_ISLEM_ADD , log_detaylari);

}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::UPDATE_LINE
**************************************************************************************/

void PRS_PERSONEL_AILE_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    SQL_QUERY   sql_query(DB);

    QComboBox * combobox_akrabalik_derecesi = ( QComboBox * ) table_widget_aile_satirlari->cellWidget(row_number , AKRABALIK_DERECESI_COLUMN);
    QComboBox * combobox_egitim_durumu      = ( QComboBox * ) table_widget_aile_satirlari->cellWidget(row_number , EGITIM_DURUMU_COLUMN );
    QComboBox * combobox_calisma_durumu     = ( QComboBox * ) table_widget_aile_satirlari->cellWidget(row_number , CALISMA_DURUMU_COLUMN);
    QComboBox * combobox_cinsiyet           = ( QComboBox * ) table_widget_aile_satirlari->cellWidget(row_number , CINSIYET_COLUMN);

    int     order_number             = table_widget_aile_satirlari->item ( row_number, ORDER_COLUMN )->text().toInt();
    QString adi_soyadi               = ((QLineEdit *) table_widget_aile_satirlari->cellWidget(row_number, AD_SOYAD_COLUMN ))->text();
    QString tc_kimlik_no             = ((QLineEdit *) table_widget_aile_satirlari->cellWidget(row_number, TC_KIMLIK_NO_COLUMN ))->text();
    QString aciklama                 = ((QLineEdit *) table_widget_aile_satirlari->cellWidget(row_number, ACIKLAMA_COLUMN ))->text();


    QComboBox  * combo_box_asgari_gecim_ind_durumu   = (QComboBox *) table_widget_aile_satirlari->cellWidget(row_number, ASGARI_GECIM_IND_DURUMU_COLUMN);

    QWidget   * cell_widget          = table_widget_aile_satirlari->cellWidget( row_number, DOGUM_TARIHI_COLUMN);
    QAdakDate* adakDate            = static_cast < QAdakDate * > ( cell_widget );
    QString     dogum_tarihi         = adakDate->DATE();


    sql_query.PREPARE_UPDATE("prs_personel_aile_uyeleri ","aile_bilgi_id",
                             "personel_id, aciklama , "
                             "adi_soyadi , tc_kimlik_no , "
                             "akrabalik_derecesi , "
                             "dogum_tarihi , "
                             "ogrenim_durumu , "
                             "calisma_durumu , "
                             "cinsiyet, "
                             "asgari_gecim_ind_durumu",
                             "order_number = :order_number");

    sql_query.SET_VALUE(":personel_id"            , record_id            );
    sql_query.SET_VALUE(":aciklama"               , aciklama             );
    sql_query.SET_VALUE(":adi_soyadi"             , adi_soyadi           );
    sql_query.SET_VALUE(":tc_kimlik_no"           , tc_kimlik_no         );
    sql_query.SET_VALUE(":akrabalik_derecesi"     , PRS_GET_AKRABALIK_DERECESI_ENUM(combobox_akrabalik_derecesi->currentText()));
    sql_query.SET_VALUE(":dogum_tarihi"           , dogum_tarihi         );
    sql_query.SET_VALUE(":ogrenim_durumu"         , combobox_egitim_durumu->currentText()  );
    sql_query.SET_VALUE(":calisma_durumu"         , PRS_GET_CALISMA_DURUMU_ENUM(combobox_calisma_durumu->currentText()) );
    sql_query.SET_VALUE(":cinsiyet"               , combobox_cinsiyet->currentText()     );
    sql_query.SET_VALUE(":asgari_gecim_ind_durumu", PRS_GET_EVET_HAYIR_DURUMU_ENUM(combo_box_asgari_gecim_ind_durumu->currentText())        );
    sql_query.SET_VALUE(":order_number"           , order_number            );

    sql_query.UPDATE();

    QString personel_adi_soyadi = line_edit_personel_adi->text();
    QString personel_sicil_no   = line_edit_personel_sicil_no->text();

    QString log_detaylari = "Aile Bireyi Ekleme";

    log_detaylari.append("\nPersonel Adı / Soyadı : " + personel_adi_soyadi +
                         "Sicil No : " + personel_sicil_no);
    log_detaylari.append("\nAile Bireyi : " + adi_soyadi);

    E9_LOG_KAYDI_EKLE ( PERSONEL_MODULU , LOG_PRS_PERSONEL , LOG_ISLEM_UPDATE , log_detaylari);

}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_DELETE_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::DELETE_LINE
***************************************************************************************/

void PRS_PERSONEL_AILE_FISI::DELETE_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);

    int aile_bilgi_id = table_widget_aile_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt();

    SQL_QUERY sql_query (DB);

    sql_query.PREPARE_SELECT(" prs_personel_aile_uyeleri","adi_soyadi",
                             "aile_bilgi_id = :aile_bilgi_id");
    sql_query.SET_VALUE(":aile_bilgi_id" , aile_bilgi_id);

    if (sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString adi_soyadi = sql_query.VALUE(0).toString();

    sql_query.PREPARE_DELETE("prs_personel_aile_uyeleri",  "aile_bilgi_id = :aile_bilgi_id");
    sql_query.SET_VALUE(":aile_bilgi_id", aile_bilgi_id );
    sql_query.DELETE();

    QString personel_adi_soyadi = line_edit_personel_adi->text();
    QString personel_sicil_no   = line_edit_personel_sicil_no->text();

    QString log_detaylari = "Aile Bireyi Ekleme";

    log_detaylari.append("\nPersonel Adı / Soyadı : " + personel_adi_soyadi +
                         "Sicil No : " + personel_sicil_no);
    log_detaylari.append("\nAile Bireyi : " + adi_soyadi);

    E9_LOG_KAYDI_EKLE ( PERSONEL_MODULU , LOG_PRS_PERSONEL , LOG_ISLEM_DELETE , log_detaylari);

}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::LOCK_FIS_RECORD
**************************************************************************************/

int PRS_PERSONEL_AILE_FISI::LOCK_FIS_RECORD(int record_id)
{
    return DB->LOCK_ROW( "personel_id","prs_personeller", QString ( "personel_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void PRS_PERSONEL_AILE_FISI::UNLOCK_FIS_RECORD(int record_id)
{
    DB->UNLOCK_ROW( "personel_id","prs_personeller", QString ( "personel_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRS_PERSONEL_AILE_FISI::CHECK_EXIT
***************************************************************************************/

int PRS_PERSONEL_AILE_FISI::CHECK_EXIT()
{
    return ADAK_OK;
}

/***************************************************************************************
                   PRS_PERSONEL_FORMU::SET_WIDGETS_MAXIMUM_SIZE
 ***************************************************************************************/

void PRS_PERSONEL_AILE_FISI::SET_WIDGETS_MAXIMUM_SIZE ()
{


    line_edit_personel_adi->setMaxLength      (60);
    line_edit_personel_sicil_no->setMaxLength (20);
}

