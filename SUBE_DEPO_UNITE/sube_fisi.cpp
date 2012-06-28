#include <QHeaderView>
#include "adak_sql.h"
#include "yonetim.h"
#include "ui_sube_fisi.h"
#include "sube_fisi_class.h"
#include "muh_console_utils.h"
#include "sube_console_utils.h"
#include "e9_log.h"
#include "sube_tree_batch_open.h"
#include "e9_yetki.h"
#include "e9_console_utils.h"
#include "muh_hesap_arama_open.h"
#include "sube_depo_unite_open.h"
#include "muh_enum.h"
#include "muh_struct.h"

extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

#define     UNITE_ID_COLUMN                     0
#define     SUBE_ID_COLUMN                      1
#define     ORDER_COLUMN                        2
#define     BILGISAYAR_ID_COLUMN                3
#define     MUH_KASA_HESAP_ID_COLUMN            4
#define     DEPO_ID_COLUMN                      5
#define     UNITE_ADI_COLUMN                    6
#define     BILGISAYAR_KODU_COLUMN              7
#define     BILGISAYAR_ADI_COLUMN               8
#define     MUH_KASA_HESAP_KODU_COLUMN          9
#define     MUH_KASA_HESAP_ADI_COLUMN           10
#define     DEPO_KODU_COLUMN                    11
#define     DEPO_ADI_COLUMN                     12
#define     UNITE_DETAYLARI_COLUMN              13


/**************************************************************************************
                   OPEN_SUBE_FISI
***************************************************************************************/

void OPEN_SUBE_FISI ( QWidget * p_parent )
{
    SUBE_FISI * F = new SUBE_FISI ( p_parent );
    F->EXEC( FULL_SCREEN );
    SUBE_SET_CURRENT_SUBE_UNITE();
}

/**************************************************************************************
                   SUBE_FISI::SUBE_FISI
***************************************************************************************/

SUBE_FISI::SUBE_FISI (  QWidget * p_parent ) : FIS_KERNEL ( p_parent ), m_ui ( new Ui::SUBE_FISI )
{
    m_ui->setupUi       ( this );
    START_FIS_KERNEL    ( this, DB );
 }

/**************************************************************************************
                   SUBE_FISI::SETUP_FORM
***************************************************************************************/

void SUBE_FISI::SETUP_FORM()
{
    SET_HELP_PAGE    ( "subedepokasaunite" );
    SET_PAGE_TITLE   ( tr ( "ŞUBE - ŞUBE / DEPO / KASA / ÜNİTE" ) );

    REGISTER_BUTTONS_WIDGET           ( m_ui->navigation_button_widget  );

    E9_KULLANICI_YETKILERI_STRUCT * SUBE_KULLANICI_YETKILERI = (E9_KULLANICI_YETKILERI_STRUCT *) E9_GET_KULLANICI_YETKILERI();

    if ( SUBE_KULLANICI_YETKILERI->sub_depo_tanimlama_yetkisi NE 1 ) {
        m_ui->tool_button_depolar->hide();
    }
    if ( SUBE_KULLANICI_YETKILERI->sub_ent_hesaplari_guncelleme_yetkisi NE 1 ) {
        m_ui->tool_button_sube_ent_hesaplari->hide();
    }

    SET_SETTING_NAME ( "SUBE_FISI" );     

    int num_of_columns   = 14;

    REGISTER_SAVER_BUTTON             (m_ui->tool_button_depolar);
    REGISTER_SAVER_BUTTON             (m_ui->tool_button_sube_ent_hesaplari);

    REGISTER_TABLE_WIDGET             ( m_ui->tableWidget_subenin_uniteleri, num_of_columns );

    SET_ROW_ID_COLUMN_POSITION        ( UNITE_ID_COLUMN );
    SET_FORM_ID_COLUMN_POSITION       ( SUBE_ID_COLUMN  );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN    );

    SET_NUMBER_OF_HIDDEN_COLUMN       ( 6 );

    SET_FIS_ORDER_DATABASE ( "sub_uniteler", "order_number", "unite_id" );

    SET_TABLE_ROW_WIDGETS  ( SUBE_ID_COLUMN             ,   WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( BILGISAYAR_ID_COLUMN       ,   WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( MUH_KASA_HESAP_ID_COLUMN   ,   WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( DEPO_ID_COLUMN             ,   WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( UNITE_ADI_COLUMN           ,   WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( BILGISAYAR_KODU_COLUMN     ,   WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS  ( BILGISAYAR_ADI_COLUMN      ,   WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( MUH_KASA_HESAP_KODU_COLUMN ,   WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS  ( MUH_KASA_HESAP_ADI_COLUMN  ,   WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( DEPO_KODU_COLUMN           ,   WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS  ( DEPO_ADI_COLUMN            ,   WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS  ( UNITE_DETAYLARI_COLUMN     ,   WIDGET_PUSH_BUTTON );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_sube_kodu );

    SET_LINE_BUTTON_ICON   ( QStringList()<<"" );

    m_ui->tableWidget_subenin_uniteleri->setHorizontalHeaderLabels ( QStringList()<<"unite_id"<<"sube_id"
                                                                  <<"order_number"<<"bilgisayar_id"
                                                                  <<"muh_kasa_hesap_id"<<"depo_id"
                                                                  <<tr ( "Kasa / Ünite Adı")
                                                                  <<tr ( "Bilgisayar Kodu")<<tr("Bilgisayar Adı")
                                                                  <<tr ( "Muh Kasa Hsp Kodu")
                                                                  <<tr ( "Muh Kasa Hsp Adı")
                                                                  <<tr ( "Depo Kodu")<<tr("Depo Adı")
                                                                  <<tr ( "")<<tr("")<<tr("") );


    m_ui->tableWidget_subenin_uniteleri->setColumnWidth    ( UNITE_ADI_COLUMN          , 195  );
    m_ui->tableWidget_subenin_uniteleri->setColumnWidth    ( BILGISAYAR_KODU_COLUMN         , 125  );
    m_ui->tableWidget_subenin_uniteleri->setColumnWidth    ( BILGISAYAR_ADI_COLUMN          , 160  );
    m_ui->tableWidget_subenin_uniteleri->setColumnWidth    ( MUH_KASA_HESAP_KODU_COLUMN     , 140  );
    m_ui->tableWidget_subenin_uniteleri->setColumnWidth    ( MUH_KASA_HESAP_ADI_COLUMN      , 180  );
    m_ui->tableWidget_subenin_uniteleri->setColumnWidth    ( DEPO_KODU_COLUMN               , 130  );
    m_ui->tableWidget_subenin_uniteleri->setColumnWidth    ( DEPO_ADI_COLUMN                , 165  );
    m_ui->tableWidget_subenin_uniteleri->setColumnWidth    ( UNITE_DETAYLARI_COLUMN         , 80   );

    m_ui->lineEdit_sube_adi->setMaxLength(30);
    m_ui->lineEdit_sube_fax->setMaxLength(15);
    m_ui->lineEdit_sube_kodu->setMaxLength(15);
    m_ui->lineEdit_sube_telefonu->setMaxLength(15);
    m_ui->limitedTextEdit_sube_adresi->SET_MAX_STRING_LENGTH(512);
    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH(512);
    m_ui->lineEdit_sgk_isyeri_no->setMaxLength( 15 );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->tableWidget_subenin_uniteleri->hideColumn(MUH_KASA_HESAP_ADI_COLUMN);
        m_ui->tableWidget_subenin_uniteleri->hideColumn(MUH_KASA_HESAP_KODU_COLUMN);
    }

    FOCUS_FIRST_WIDGET();
}

/**************************************************************************************
                   SUBE_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void SUBE_FISI::CLEAR_FORM_MEMBERS()
{
   m_sube_id = 0;
}

/**************************************************************************************
                   SUBE_FISI::NEW_FIS_RECORD
***************************************************************************************/

void SUBE_FISI::NEW_FIS_RECORD ()
{

}

/**************************************************************************************
                   SUBE_FISI::GET_FIS_RECORD
***************************************************************************************/

int SUBE_FISI::GET_FIS_RECORD ( int p_sube_id )
{
    SQL_QUERY sql_query     ( DB );
    SQL_QUERY yonetim_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "sub_subeler ","sube_kodu, sube_adi, sube_adresi, sube_telefon, sube_fax, aciklama, sgk_isyeri_no ",
                               "sube_id = :sube_id ");

    sql_query.SET_VALUE      ( ":sube_id" , p_sube_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_sube_id = p_sube_id;

    m_ui->lineEdit_sube_kodu->setText           ( sql_query.VALUE(0).toString());
    m_ui->lineEdit_sube_adi->setText            ( sql_query.VALUE(1).toString());
    m_ui->limitedTextEdit_sube_adresi->setText  ( sql_query.VALUE(2).toString());
    m_ui->lineEdit_sube_telefonu->setText       ( sql_query.VALUE(3).toString());
    m_ui->lineEdit_sube_fax->setText            ( sql_query.VALUE(4).toString());
    m_ui->limitedTextEdit_aciklama->setText     ( sql_query.VALUE(5).toString());
    m_ui->lineEdit_sgk_isyeri_no->setText       ( sql_query.VALUE(6).toString());

    sql_query.PREPARE_SELECT ( "sub_uniteler","unite_id, order_number, unite_adi, "
                               "bilgisayar_id, "
                               "muh_kasa_hesap_id, depo_id ",
                               "sube_id = :sube_id ");

    sql_query.SET_VALUE      ( ":sube_id", p_sube_id );

    if ( sql_query.SELECT("order_number ASC ") EQ 0 ) {
        return ADAK_OK;
    }
    while ( sql_query.NEXT() EQ true ) {

        int     bilgisayar_id                 = sql_query.VALUE(3).toInt();
        int     kasa_unite_muh_kasa_hesap_id  = sql_query.VALUE(4).toInt();
        int     depo_id                       = sql_query.VALUE(5).toInt();

        QString bilgisayar_kodu = "";
        QString bilgisayar_adi  = "";

        yonetim_query.PREPARE_SELECT ( "ynt_bilgisayarlar ","bilgisayar_kodu, bilgisayar_adi ",
                                       "bilgisayar_id = :bilgisayar_id AND silinmis_mi =:silinmis_mi");
        yonetim_query.SET_VALUE      ( ":bilgisayar_id", bilgisayar_id );
        yonetim_query.SET_VALUE      (":silinmis_mi",0);

        if ( yonetim_query.SELECT() > 0 ) {
            yonetim_query.NEXT();
            bilgisayar_kodu = yonetim_query.VALUE(0).toString();
            bilgisayar_adi  = yonetim_query.VALUE(1).toString();
        }

        QString muh_kasa_hesap_kodu = "";
        QString muh_kasa_hesap_ismi = "";

        if ( kasa_unite_muh_kasa_hesap_id > 0 ) {
            MUH_GET_HESAP_KODU_HESAP_ISMI ( kasa_unite_muh_kasa_hesap_id, muh_kasa_hesap_kodu, muh_kasa_hesap_ismi );
        }

        QString depo_kodu = "";
        QString depo_adi  = "";

        if ( depo_id > 0 ) {
            SUBE_GET_DEPO_KODU_ADI ( depo_id, &depo_kodu, &depo_adi );
        }

        int current_row = ADD_NEW_LINE();

        QTableWidgetItem * new_item = new QTableWidgetItem ( sql_query.VALUE(0).toString() );
        m_ui->tableWidget_subenin_uniteleri->setItem ( current_row, UNITE_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( QVariant ( p_sube_id ).toString() );
        m_ui->tableWidget_subenin_uniteleri->setItem ( current_row, SUBE_ID_COLUMN, new_item );


        new_item = new QTableWidgetItem ( sql_query.VALUE(1).toString() );
        m_ui->tableWidget_subenin_uniteleri->setItem ( current_row, ORDER_COLUMN, new_item );

        QLineEdit   * satir_lineEdit_sube_id             = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, SUBE_ID_COLUMN              );
        QLineEdit   * satir_lineEdit_unite_adi      = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, UNITE_ADI_COLUMN       );
        QLineEdit   * satir_lineEdit_bilgisayar_id       = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, BILGISAYAR_ID_COLUMN        );
        QLineEdit   * satir_lineEdit_muh_kasa_hesap_id   = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, MUH_KASA_HESAP_ID_COLUMN    );
        QLineEdit   * satir_lineEdit_depo_id             = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, DEPO_ID_COLUMN              );
        QSearchEdit * satir_searchEdit_bilgisayar_kodu   = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, BILGISAYAR_KODU_COLUMN      );
        QLineEdit   * satir_lineEdit_bilgisayar_adi      = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, BILGISAYAR_ADI_COLUMN       );
        QSearchEdit * satir_searcEdit_muh_hesap_kodu     = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, MUH_KASA_HESAP_KODU_COLUMN  );
        QLineEdit   * satir_lineEdit_muh_hesap_adi       = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, MUH_KASA_HESAP_ADI_COLUMN   );
        QSearchEdit * satir_searcEdit_depo_kodu          = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, DEPO_KODU_COLUMN            );
        QLineEdit   * satir_lineEdit_depo_adi            = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( current_row, DEPO_ADI_COLUMN             );

        satir_lineEdit_sube_id->setText   ( QVariant ( p_sube_id ).toString() );

        satir_lineEdit_unite_adi->setText ( sql_query.VALUE(2).toString());

        satir_lineEdit_bilgisayar_id->setText ( QVariant ( bilgisayar_id).toString() );
        satir_lineEdit_muh_kasa_hesap_id->setText ( QVariant ( kasa_unite_muh_kasa_hesap_id ).toString() );

        satir_lineEdit_depo_id->setText ( QVariant ( depo_id ).toString() );
        satir_searcEdit_depo_kodu->SET_TEXT ( depo_kodu );
        satir_lineEdit_depo_adi->setText   ( depo_adi );

        satir_searchEdit_bilgisayar_kodu->SET_TEXT ( bilgisayar_kodu );
        satir_lineEdit_bilgisayar_adi->setText ( bilgisayar_adi );

        satir_searcEdit_muh_hesap_kodu->SET_TEXT ( muh_kasa_hesap_kodu );
        satir_lineEdit_muh_hesap_adi->setText ( muh_kasa_hesap_ismi );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void SUBE_FISI::SET_LINE_DEFAULTS ( int p_row_number )
{
    QLineEdit   * satir_lineEdit_bilgisayar_adi     = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_row_number , BILGISAYAR_ADI_COLUMN     );
    QLineEdit   * satir_lineEdit_muh_kasa_hesap_adi = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_row_number , MUH_KASA_HESAP_ADI_COLUMN );
    QLineEdit   * lineEdit_unite_adi                = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_row_number , UNITE_ADI_COLUMN     );
    QLineEdit   * lineEdit_depo_adi                 = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_row_number , DEPO_ADI_COLUMN           );
    QPushButton * button_unite_detaylari            = ( QPushButton * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_row_number , UNITE_DETAYLARI_COLUMN   );
    QSearchEdit * searchedit_muh_hesap_kodu         = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_row_number , MUH_KASA_HESAP_KODU_COLUMN);
    QLineEdit   * lineedit_muh_kasa_hesap_id        = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_row_number , MUH_KASA_HESAP_ID_COLUMN);


    satir_lineEdit_bilgisayar_adi->setReadOnly ( true );
    satir_lineEdit_muh_kasa_hesap_adi->setReadOnly( true );
    lineEdit_unite_adi->setMaxLength(30);
    lineEdit_depo_adi->setReadOnly ( true );
    button_unite_detaylari->setToolTip    ( tr ( "Ünite Detayları" ) );
    button_unite_detaylari->setText       ( tr ( "Detaylar" ) );
    QFont button_font      = button_unite_detaylari->font();
    button_font.setBold( true );
    button_unite_detaylari->setFont ( button_font );
    satir_lineEdit_muh_kasa_hesap_adi->setMaxLength(60);

    REGISTER_LINE_SAVER_BUTTON ( p_row_number,UNITE_DETAYLARI_COLUMN );

    DISABLE_CLEAR_ON_WIDGET( searchedit_muh_hesap_kodu );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {

        QString kasa_ent_hesap_kodu, kasa_ent_hesap_adi;
        // Fis 'in sube_id sorgula yapmamiz lazim !!!!
        int kasa_ent_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( m_sube_id, "kasa_ent_hesap_id", &kasa_ent_hesap_kodu, &kasa_ent_hesap_adi );

        lineedit_muh_kasa_hesap_id->setText         ( QVariant ( kasa_ent_hesap_id ).toString() );
        searchedit_muh_hesap_kodu->SET_TEXT         ( kasa_ent_hesap_kodu );
        satir_lineEdit_muh_kasa_hesap_adi->setText  ( kasa_ent_hesap_adi  );

    }
    else {
        searchedit_muh_hesap_kodu->GET_TEXT().clear();
        satir_lineEdit_muh_kasa_hesap_adi->text().clear();
    }
}

/**************************************************************************************
                   SUBE_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int SUBE_FISI::CHECK_FIS_FORM_VAR ( QObject * p_object )
{

    if ( p_object EQ m_ui->limitedTextEdit_aciklama ) {
        if ( m_ui->limitedTextEdit_aciklama->toPlainText().size() > 512 ) {
            MSG_WARNING( tr ( "Açıklamanız 512 karakterden daha uzun olamaz!.. "), m_ui->limitedTextEdit_aciklama );

            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ m_ui->limitedTextEdit_sube_adresi ) {
        if ( m_ui->limitedTextEdit_sube_adresi->toPlainText().size() > 512 ) {
            MSG_WARNING( tr ( "Şube adresi 512 karakterden daha uzun olamaz!.."), m_ui->limitedTextEdit_sube_adresi );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int SUBE_FISI::CHECK_FIS_FORM_EMPTY()
{

    if ( m_ui->lineEdit_sube_kodu->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Şube kodu boş bırakılamaz." ), m_ui->lineEdit_sube_kodu );

        return ADAK_FAIL;
    }

    if ( m_ui->lineEdit_sube_adi->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Şube adı boş bırakılamaz." ), m_ui->lineEdit_sube_adi );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int SUBE_FISI::CHECK_FIS_RECORD_ADD()
{
    SQL_QUERY sql_query  ( DB );

    sql_query.PREPARE_SELECT ( "sub_subeler","sube_id","sube_kodu = :sube_kodu" );
    sql_query.SET_VALUE      ( ":sube_kodu", m_ui->lineEdit_sube_kodu->text() );
    if ( sql_query.SELECT() > 0 ) {
        MSG_WARNING(  tr ( "Bu kodla bir şube kaydı yapılmış. Lütfen şube kodunu değiştiriniz"), m_ui->lineEdit_sube_kodu );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::ADD_FIS_RECORD
***************************************************************************************/

int SUBE_FISI::ADD_FIS_RECORD()
{

    SQL_QUERY sql_query     ( DB );
    SQL_QUERY query(DB);

    query.PREPARE_SELECT(    "sub_subeler " ,"sube_port_cekler_hsp_id , sube_tah_cekler_hsp_id, sube_tem_cekler_hsp_id,"
                             "sube_kar_cekler_hsp_id , sube_port_snt_hsp_id , sube_tah_snt_hsp_id , "
                             "sube_tem_snt_hsp_id , sube_pro_snt_hsp_id , sube_port_snt_ileri_trh_hsp_id , "
                             "sube_tah_snt_ileri_trh_hsp_id , sube_tem_snt_ileri_trh_hsp_id , sube_borc_snt_hsp_id,"
                             "sube_borc_snt_ileri_trh_hsp_id , sube_sup_alacaklar_hsp_id , sube_tah_edlmyn_cekler_hsp_id,"
                             "sube_tah_edlmyn_snt_hsp_id , sube_yurtici_satis_hsp_id , sube_hizmet_urt_maliyet_hsp_id,   "
                             "sube_prs_gdrlr_hsp_id ,sube_sarf_malzemeleri_hsp_id, sgk_isyeri_no,"
                             "sube_imalattan_giris_hsp_id, "
                             "kasa_ent_hesap_id , cari_alici_ent_hesap_id, cari_satici_ent_hesap_id , banka_ent_hesap_id ,"
                             "ver_cek_ent_hesap_id , odeme_emirleri_ent_hesap_id         ,"
                             "pos_ent_hesap_id , kk_ent_hesap_id ,                        "
                             "prk_gider_ent_hesap_id , urunler_ent_hesap_id ,             "
                             "hizmetler_ent_hesap_id,                                     "
                             "demirbaslar_ent_hesap_id, pos_kom_gideri_ent_hesap_id,      "
                             "prs_avanslari_ent_hesap_id ,                                "
                             "personel_borclari_ent_hesap_id ,                            "
                             "prs_damga_vergisi_ent_hesap_id ,                            "
                             "personel_vergi_ent_hesap_id,                                "
                             "prk_gelir_ent_hesap_id,                                     "
                             "prs_ssk_isci_payi_hesap_id,                                 "
                             "prs_ssk_isveren_payi_hesap_id,                              "
                             "prs_issizlik_sgk_isci_hsp_id,                               "
                             "prs_issizlik_sgk_isv_hsp_id,                                "
                             "asg_gecim_ind_hesap_id,                                     "
                             "isveren_payi_giderleri_hps_id,                              "
                             "bir_amortismanlar_hsp_id,                                   "
                             "amor_giderleri_hsp_id,                                      "
                             "sgdp_prim_yuzde_isc_hsp_id,                                 "
                             "sgdp_prim_yuzde_isv_hsp_id,                                 "
                             "kidem_tazminati_hesap_id,                                   "
                             "ihbar_tazminati_hesap_id,                                   "
                             "dmr_satis_kar_ent_hesap_id,                                 "
                             "dmr_satis_zarar_ent_hesap_id,                               "
                             "sendika_odeme_hesap_id,                                     "
                             "smmm_alinan_hizmet_hesap_id,                                "
                             "smmm_gelir_vergisi_hesap_id,                                "
                             "smmm_satilan_hizmet_hesap_id,                               "
                             "ortaklara_borclar_hesap_id                                 ",
                             "sube_id = :sube_id");
    query.SET_VALUE(":sube_id",1);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();



    sql_query.PREPARE_INSERT ( "sub_subeler","sube_id","sube_kodu, sube_adi, sube_adresi, "
                               "sube_telefon, sube_fax, aciklama ,sube_port_cekler_hsp_id , sube_tah_cekler_hsp_id,"
                               "sube_tem_cekler_hsp_id , sube_kar_cekler_hsp_id , sube_port_snt_hsp_id ,"
                               "sube_tah_snt_hsp_id , sube_tem_snt_hsp_id , sube_pro_snt_hsp_id , "
                               "sube_port_snt_ileri_trh_hsp_id , sube_tah_snt_ileri_trh_hsp_id , "
                               "sube_tem_snt_ileri_trh_hsp_id  , sube_borc_snt_hsp_id , sube_borc_snt_ileri_trh_hsp_id,"
                               "sube_sup_alacaklar_hsp_id , sube_tah_edlmyn_cekler_hsp_id , sube_tah_edlmyn_snt_hsp_id , "
                               "sube_yurtici_satis_hsp_id , sube_hizmet_urt_maliyet_hsp_id , "
                               "sube_prs_gdrlr_hsp_id ,sube_sarf_malzemeleri_hsp_id, sgk_isyeri_no, "
                               "sube_imalattan_giris_hsp_id, "
                               " kasa_ent_hesap_id              ,                                    "
                               " cari_alici_ent_hesap_id        , cari_satici_ent_hesap_id              ,                                    "
                               " banka_ent_hesap_id             , ver_cek_ent_hesap_id             , "
                               " odeme_emirleri_ent_hesap_id    ,                                    "
                               " pos_ent_hesap_id               ,                                    "
                               " kk_ent_hesap_id                , prk_gider_ent_hesap_id           , "
                               " urunler_ent_hesap_id           ,                                    "
                               " hizmetler_ent_hesap_id         , demirbaslar_ent_hesap_id,          "
                               " pos_kom_gideri_ent_hesap_id    ,                                    "
                               " prs_avanslari_ent_hesap_id     , personel_borclari_ent_hesap_id   , "
                               " prs_damga_vergisi_ent_hesap_id , personel_vergi_ent_hesap_id      , "
                               " prk_gelir_ent_hesap_id         ,                                    "
                               " prs_ssk_isci_payi_hesap_id     ,                                    "
                               " prs_ssk_isveren_payi_hesap_id  ,                                    "
                               " prs_issizlik_sgk_isci_hsp_id   ,                                    "
                               " prs_issizlik_sgk_isv_hsp_id    ,                                    "
                               " asg_gecim_ind_hesap_id         ,                                    "
                               " isveren_payi_giderleri_hps_id  ,                                    "
                               " bir_amortismanlar_hsp_id       ,                                    "
                               " amor_giderleri_hsp_id          ,                                    "
                               " sgdp_prim_yuzde_isc_hsp_id     , sgdp_prim_yuzde_isv_hsp_id        ,"
                               " kidem_tazminati_hesap_id       , ihbar_tazminati_hesap_id          ,"
                               " dmr_satis_kar_ent_hesap_id     , dmr_satis_zarar_ent_hesap_id      ,"
                               " sendika_odeme_hesap_id         , smmm_alinan_hizmet_hesap_id       ,"
                               " smmm_gelir_vergisi_hesap_id    , smmm_satilan_hizmet_hesap_id      ,"
                               " ortaklara_borclar_hesap_id          ");

    sql_query.SET_VALUE     ( ":sube_kodu"                              , m_ui->lineEdit_sube_kodu->text());
    sql_query.SET_VALUE     ( ":sube_adi"                               , m_ui->lineEdit_sube_adi->text());
    sql_query.SET_VALUE     ( ":sube_adresi"                            , m_ui->limitedTextEdit_sube_adresi->toPlainText());
    sql_query.SET_VALUE     ( ":sube_telefon"                           , m_ui->lineEdit_sube_telefonu->text());
    sql_query.SET_VALUE     ( ":sube_fax"                               , m_ui->lineEdit_sube_fax->text());
    sql_query.SET_VALUE     ( ":aciklama"                               , m_ui->limitedTextEdit_aciklama->toPlainText());
    sql_query.SET_VALUE     ( ":sgk_isyeri_no"                          , m_ui->lineEdit_sgk_isyeri_no->text());
    sql_query.SET_VALUE     ( ":sube_port_cekler_hsp_id"                , query.VALUE("sube_port_cekler_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_tah_cekler_hsp_id"                 , query.VALUE("sube_tah_cekler_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_tem_cekler_hsp_id"                 , query.VALUE("sube_tem_cekler_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_kar_cekler_hsp_id"                 , query.VALUE("sube_kar_cekler_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_port_snt_hsp_id"                   , query.VALUE("sube_port_snt_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_tah_snt_hsp_id"                    , query.VALUE("sube_tah_snt_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_tem_snt_hsp_id"                    , query.VALUE("sube_tem_snt_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_pro_snt_hsp_id"                    , query.VALUE("sube_pro_snt_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_port_snt_ileri_trh_hsp_id"         , query.VALUE("sube_port_snt_ileri_trh_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_tah_snt_ileri_trh_hsp_id"          , query.VALUE("sube_tah_snt_ileri_trh_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_tem_snt_ileri_trh_hsp_id"          , query.VALUE("sube_tem_snt_ileri_trh_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_borc_snt_hsp_id"                   , query.VALUE("sube_borc_snt_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_borc_snt_ileri_trh_hsp_id"         , query.VALUE("sube_borc_snt_ileri_trh_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_sup_alacaklar_hsp_id"              , query.VALUE("sube_sup_alacaklar_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_tah_edlmyn_cekler_hsp_id"          , query.VALUE("sube_tah_edlmyn_cekler_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_tah_edlmyn_snt_hsp_id"             , query.VALUE("sube_tah_edlmyn_snt_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_yurtici_satis_hsp_id"              , query.VALUE("sube_yurtici_satis_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_hizmet_urt_maliyet_hsp_id"         , query.VALUE("sube_hizmet_urt_maliyet_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_prs_gdrlr_hsp_id"                  , query.VALUE("sube_prs_gdrlr_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_sarf_malzemeleri_hsp_id"           , query.VALUE("sube_sarf_malzemeleri_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":sube_imalattan_giris_hsp_id"            , query.VALUE("sube_imalattan_giris_hsp_id").toInt());
    sql_query.SET_VALUE     ( ":kasa_ent_hesap_id"                      , query.VALUE("kasa_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":cari_alici_ent_hesap_id"                , query.VALUE("cari_alici_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":cari_satici_ent_hesap_id"               , query.VALUE("cari_satici_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":banka_ent_hesap_id"                     , query.VALUE("banka_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":ver_cek_ent_hesap_id"                   , query.VALUE("ver_cek_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":odeme_emirleri_ent_hesap_id"            , query.VALUE("odeme_emirleri_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":pos_ent_hesap_id"                       , query.VALUE("pos_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":kk_ent_hesap_id"                        , query.VALUE("kk_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":prk_gider_ent_hesap_id"                 , query.VALUE("prk_gider_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":urunler_ent_hesap_id"                   , query.VALUE("urunler_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":hizmetler_ent_hesap_id"                 , query.VALUE("hizmetler_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":demirbaslar_ent_hesap_id"               , query.VALUE("demirbaslar_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":pos_kom_gideri_ent_hesap_id"            , query.VALUE("pos_kom_gideri_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":prs_avanslari_ent_hesap_id"             , query.VALUE("prs_avanslari_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":personel_borclari_ent_hesap_id"         , query.VALUE("personel_borclari_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":prs_damga_vergisi_ent_hesap_id"         , query.VALUE("prs_damga_vergisi_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":personel_vergi_ent_hesap_id"            , query.VALUE("personel_vergi_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":prk_gelir_ent_hesap_id"                 , query.VALUE("prk_gelir_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":prs_ssk_isci_payi_hesap_id"             , query.VALUE("prs_ssk_isveren_payi_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":prs_ssk_isveren_payi_hesap_id"          , query.VALUE("prs_ssk_isveren_payi_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":prs_issizlik_sgk_isci_hsp_id"           , query.VALUE("prs_issizlik_sgk_isci_hsp_id").toInt() );
    sql_query.SET_VALUE     ( ":prs_issizlik_sgk_isv_hsp_id"            , query.VALUE("prs_issizlik_sgk_isv_hsp_id").toInt() );
    sql_query.SET_VALUE     ( ":asg_gecim_ind_hesap_id"                 , query.VALUE("asg_gecim_ind_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":isveren_payi_giderleri_hps_id"          , query.VALUE("isveren_payi_giderleri_hps_id").toInt() );
    sql_query.SET_VALUE     ( ":bir_amortismanlar_hsp_id"               , query.VALUE("bir_amortismanlar_hsp_id").toInt() );
    sql_query.SET_VALUE     ( ":amor_giderleri_hsp_id"                  , query.VALUE("amor_giderleri_hsp_id").toInt() );
    sql_query.SET_VALUE     ( ":sgdp_prim_yuzde_isc_hsp_id"             , query.VALUE("sgdp_prim_yuzde_isc_hsp_id").toInt() );
    sql_query.SET_VALUE     ( ":sgdp_prim_yuzde_isv_hsp_id"             , query.VALUE("sgdp_prim_yuzde_isv_hsp_id").toInt() );
    sql_query.SET_VALUE     ( ":kidem_tazminati_hesap_id"               , query.VALUE("kidem_tazminati_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":ihbar_tazminati_hesap_id"               , query.VALUE("ihbar_tazminati_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":dmr_satis_kar_ent_hesap_id"             , query.VALUE("dmr_satis_kar_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":dmr_satis_zarar_ent_hesap_id"           , query.VALUE("dmr_satis_zarar_ent_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":sendika_odeme_hesap_id"                 , query.VALUE("sendika_odeme_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":smmm_alinan_hizmet_hesap_id"            , query.VALUE("smmm_alinan_hizmet_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":smmm_gelir_vergisi_hesap_id"            , query.VALUE("smmm_gelir_vergisi_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":smmm_satilan_hizmet_hesap_id"           , query.VALUE("smmm_satilan_hizmet_hesap_id").toInt() );
    sql_query.SET_VALUE     ( ":ortaklara_borclar_hesap_id"             , query.VALUE("ortaklara_borclar_hesap_id").toInt() );

    m_sube_id = sql_query.INSERT();


    QString log_detaylari = "Şube Kodu / Adı : " + m_ui->lineEdit_sube_kodu->text() + " " + m_ui->lineEdit_sube_adi->text();

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_SUBELER , LOG_ISLEM_ADD , log_detaylari );

    return m_sube_id;

}

/**************************************************************************************
                   SUBE_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int SUBE_FISI::CHECK_FIS_RECORD_UPDATE ( int p_sube_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "sub_subeler ","sube_id ","sube_kodu = :sube_kodu" );
    sql_query.SET_VALUE      ( ":sube_kodu", m_ui->lineEdit_sube_kodu->text() );
    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_OK;
    }

    sql_query.NEXT();
    if ( sql_query.VALUE(0).toInt() NE p_sube_id ) {
        MSG_WARNING(  tr ( "Bu kodla bir şube kaydı yapılmış lütfen şube kodunu değiştiriniz"), NULL );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void SUBE_FISI::UPDATE_FIS_RECORD ( int p_sube_id )
{

    SQL_QUERY sql_query     ( DB );

    sql_query.PREPARE_UPDATE ( "sub_subeler","sube_id",
                               "sube_kodu       , "
                               "sube_adi        , "
                               "sube_adresi     , "
                               "sube_telefon    , "
                               "sube_fax        , "
                               "aciklama        , "
                               "sgk_isyeri_no     ",
                               "sube_id = :sube_id" );

    sql_query.SET_VALUE      ( ":sube_kodu"     , m_ui->lineEdit_sube_kodu->text());
    sql_query.SET_VALUE      ( ":sube_adi"      , m_ui->lineEdit_sube_adi->text());
    sql_query.SET_VALUE      ( ":sube_adresi"   , m_ui->limitedTextEdit_sube_adresi->toPlainText());
    sql_query.SET_VALUE      ( ":sube_telefon"  , m_ui->lineEdit_sube_telefonu->text());
    sql_query.SET_VALUE      ( ":sube_fax"      , m_ui->lineEdit_sube_fax->text());
    sql_query.SET_VALUE      ( ":aciklama"      , m_ui->limitedTextEdit_aciklama->toPlainText());
    sql_query.SET_VALUE      ( ":sgk_isyeri_no" , m_ui->lineEdit_sgk_isyeri_no->text());
    sql_query.SET_VALUE      ( ":sube_id"       , p_sube_id );
    sql_query.UPDATE();

    QString log_detaylari = "Şube Kodu / Adı : " + m_ui->lineEdit_sube_kodu->text() + " " + m_ui->lineEdit_sube_adi->text();

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_SUBELER , LOG_ISLEM_UPDATE , log_detaylari );

}

/**************************************************************************************
                   SUBE_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int SUBE_FISI::CHECK_FIS_RECORD_DELETE ( int p_sube_id )
{
    if ( p_sube_id EQ 1 ) {
        MSG_WARNING( "Silmek istediğiniz şube merkez şubedir.Merkez şubeyi silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    if ( p_sube_id EQ SUBE_GET_SUBE_ID()) {
        MSG_WARNING( "Kendi şubenizi silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("sub_depolar","sube_id","sube_id=:sube_id");
    query.SET_VALUE     (":sube_id",p_sube_id);
    if ( query.SELECT() > 0 ) {
        MSG_WARNING( "Şubeye kayıtlı depolar var.Önce depoları silmeniz gerekmektedir.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("dmr_demirbaslar","sube_id","sube_id=:sube_id");
    query.SET_VALUE     (":sube_id",p_sube_id);
    if ( query.SELECT() > 0 ) {
        MSG_WARNING( "Demirbaş da işlem görmüş şubeyi silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("prk_fisler","sube_id","sube_id=:sube_id");
    query.SET_VALUE     (":sube_id",p_sube_id);
    if ( query.SELECT() > 0 ) {
        MSG_WARNING( "Perakandede da işlem görmüş şubeyi silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("prk_gelir_gider_turleri","sube_id","sube_id=:sube_id");
    query.SET_VALUE     (":sube_id",p_sube_id);
    if ( query.SELECT() > 0 ) {
        MSG_WARNING( "Perakandede da işlem görmüş şubeyi silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("prs_bordrolar","sube_id","sube_id=:sube_id");
    query.SET_VALUE     (":sube_id",p_sube_id);
    if ( query.SELECT() > 0 ) {
        MSG_WARNING( "Personelde işlem görmüş şubeyi silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("prs_personeller","sube_id","sube_id=:sube_id");
    query.SET_VALUE     (":sube_id",p_sube_id);
    if ( query.SELECT() > 0 ) {
        MSG_WARNING( "Personelde işlem görmüş şubeyi silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    query.PREPARE_SELECT("smm_hizmetler","sube_id","sube_id=:sube_id");
    query.SET_VALUE     (":sube_id",p_sube_id);
    if ( query.SELECT() > 0 ) {
        MSG_WARNING( "Serbest meslek de işlem görmüş şubeyi silemezsiniz.",NULL);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void SUBE_FISI::DELETE_FIS_RECORD ( int p_sube_id )
{

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "sub_subeler","sube_kodu,sube_adi","sube_id = :sube_id");
    sql_query.SET_VALUE      ( ":sube_id" , p_sube_id ) ;

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString sube_kodu_adi = "Şube Kodu / Adı : " + sql_query.VALUE(0).toString() + "  "
                            + sql_query.VALUE(1).toString();

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_SUBELER , LOG_ISLEM_DELETE , sube_kodu_adi);

    sql_query.PREPARE_DELETE ( "sub_uniteler","sube_id = :sube_id");
    sql_query.SET_VALUE      ( ":sube_id", p_sube_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE ("sub_depolar" , "sube_id = :sube_id");
    sql_query.SET_VALUE      ( ":sube_id", p_sube_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE ( "sub_subeler","sube_id = :sube_id" );
    sql_query.SET_VALUE      ( ":sube_id", p_sube_id );
    sql_query.DELETE();

    m_sube_id = 0;

}

/**************************************************************************************
                   SUBE_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int SUBE_FISI::SELECT_FIS_RECORD()
{
    return OPEN_SUBE_ARAMA ( "", this );
}

/**************************************************************************************
                   SUBE_FISI::FIND_FIS_RECORD
***************************************************************************************/

int SUBE_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY sql_query  ( DB );
    sql_query.PREPARE_SELECT ( "sub_subeler","sube_id","sube_kodu = :sube_kodu " );
    sql_query.SET_VALUE      ( ":sube_kodu", m_ui->lineEdit_sube_kodu->text() );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr ( "Şube bulunamadı." ), NULL );
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int SUBE_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY sql_query  ( DB );
    sql_query.PREPARE_SELECT ( "sub_subeler","sube_id" );
    if (  sql_query.SELECT("sube_kodu ASC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int SUBE_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_subeler","sube_id");
    if ( sql_query.SELECT("sube_kodu DESC ",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();

}

/**************************************************************************************
                   SUBE_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int SUBE_FISI::FIND_PREV_FIS_RECORD ()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sub_subeler","sube_id ",
                                     "sube_kodu < :sube_kodu");
    sql_query.SET_VALUE            ( ":sube_kodu",  m_ui->lineEdit_sube_kodu->text() );
    if( sql_query.SELECT("sube_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int SUBE_FISI::FIND_NEXT_FIS_RECORD ()
{
    SQL_QUERY sql_query (DB);
    sql_query.PREPARE_SELECT ( "sub_subeler","sube_id ",
                                     "sube_kodu > :sube_kodu " );
    sql_query.SET_VALUE            ( ":sube_kodu",      m_ui->lineEdit_sube_kodu->text() );
    if( sql_query.SELECT("sube_kodu ASC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SUBE_FISI::CHECK_LINE_VAR
***************************************************************************************/

int SUBE_FISI::CHECK_LINE_VAR ( int p_row_number, QObject * p_object )
{
    SQL_QUERY sql_query     ( DB );
    SQL_QUERY yonetim_query ( G_YONETIM_DB );

    QSearchEdit * satir_searchEdit_bilgisayar_kodu          = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, BILGISAYAR_KODU_COLUMN      );
    QLineEdit   * satir_lineEdit_bilgisayar_id              = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, BILGISAYAR_ID_COLUMN        );
    QLineEdit   * satir_lineEdit_bilgisayar_adi             = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, BILGISAYAR_ADI_COLUMN       );
    QSearchEdit * satir_searcEdit_muhasebe_kasa_hesap_kodu  = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, MUH_KASA_HESAP_KODU_COLUMN  );
    QLineEdit   * satir_lineEdit_muh_kasa_hesap_id          = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, MUH_KASA_HESAP_ID_COLUMN    );
    QLineEdit   * satir_lineEdit_muhasebe_hesap_adi         = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, MUH_KASA_HESAP_ADI_COLUMN   );
    QLineEdit   * satir_lineEdit_depo_id                    = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, DEPO_ID_COLUMN              );
    QSearchEdit * satir_searchEdit_depo_kodu                = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, DEPO_KODU_COLUMN            );
    QLineEdit   * satir_lineEdit_depo_adi                   = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, DEPO_ADI_COLUMN             );

    if ( p_object EQ satir_searcEdit_muhasebe_kasa_hesap_kodu ) {

        MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

        MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

        if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , satir_searcEdit_muhasebe_kasa_hesap_kodu->GET_TEXT() ) EQ 0 ) {
            MSG_WARNING(  tr ( "Muhasebe hesabı bulunamadı." ), satir_searcEdit_muhasebe_kasa_hesap_kodu );

            return ADAK_FAIL;
        }

        if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {
            MSG_WARNING(  tr ( "Seçilen hesap tali hesap değil. Entegrasyon yapılamaz."), satir_searcEdit_muhasebe_kasa_hesap_kodu );

            return false;
        }

        satir_lineEdit_muhasebe_hesap_adi->setText ( MUHASEBE_HESABI->hesap_ismi );
        satir_lineEdit_muh_kasa_hesap_id->setText ( QVariant ( MUHASEBE_HESABI->hesap_id ).toString() );
        return ADAK_OK;

    }
    else if ( p_object EQ satir_searchEdit_bilgisayar_kodu ) {
        yonetim_query.PREPARE_SELECT ( "ynt_bilgisayarlar","bilgisayar_id, bilgisayar_adi ",
                                       "bilgisayar_kodu = :bilgisayar_kodu AND silinmis_mi = :silinmis_mi" );

        yonetim_query.SET_VALUE      (":silinmis_mi",0   );
        yonetim_query.SET_VALUE      ( ":bilgisayar_kodu", satir_searchEdit_bilgisayar_kodu->GET_TEXT() );
        if ( yonetim_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr ( "Bu kodla kayıtlı bir bilgisayar bulunamadı." ), satir_searchEdit_bilgisayar_kodu  );

            return ADAK_FAIL;
        }

        yonetim_query.NEXT();
        // CHECHK_ADD_LINE kullanmak icin
        satir_lineEdit_bilgisayar_id->setText ( yonetim_query.VALUE(0).toString() );
        satir_lineEdit_bilgisayar_adi->setText ( yonetim_query.VALUE(1).toString() );
        if ( SECILI_BILGISAYAR_BASKA_UNITEYE_KAYITLI_MI ( yonetim_query.VALUE(0).toInt() ) EQ false ) {
            return ADAK_FAIL_UNDO;
        }

        return ADAK_OK;
    }

    else if  ( p_object EQ satir_searchEdit_depo_kodu ) {
        sql_query.PREPARE_SELECT ( "sub_depolar","depo_id, depo_adi ","depo_kodu = :depo_kodu" );
        sql_query.SET_VALUE      ( ":depo_kodu", satir_searchEdit_depo_kodu->GET_TEXT() );
        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr ( "Depo kodu bulunamadı." ), satir_searchEdit_depo_kodu );

            return ADAK_FAIL;
        }
        sql_query.NEXT();
        satir_lineEdit_depo_id->setText ( sql_query.VALUE(0).toString() );
        satir_lineEdit_depo_adi->setText ( sql_query.VALUE(1).toString() );
        return ADAK_OK;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int SUBE_FISI::CHECK_LINE_EMPTY ( int p_row_number)
{
    QLineEdit   * satir_lineEdit_unite_adi           = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, UNITE_ADI_COLUMN    );
  //QSearchEdit * satir_searchEdit_bilgisayar_kodu   = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, BILGISAYAR_KODU_COLUMN   );
  //QLineEdit   * satir_lineEdit_bilgisayar_id       = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, BILGISAYAR_ID_COLUMN     );
  //QSearchEdit * searchedit_muh_hesap_kodu          = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget(p_row_number , MUH_KASA_HESAP_KODU_COLUMN);

//    if ( searchedit_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
//        MSG_INFO( tr( "Muhasebe hesap kodu boş bırakılamaz." ), searchedit_muh_hesap_kodu );
//        return ADAK_FAIL;
//    }
    if ( satir_lineEdit_unite_adi->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Ünite adı boş bırakılamaz." ), satir_lineEdit_unite_adi );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}


/**************************************************************************************
                   SUBE_FISI::CHECK_ADD_LINE
***************************************************************************************/

int SUBE_FISI::CHECK_ADD_LINE ( int p_sube_id, int p_row_number )
{
    SQL_QUERY sql_query ( DB );

    QLineEdit   * satir_lineEdit_unite_adi          = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, UNITE_ADI_COLUMN );
    QLineEdit   * satir_lineEdit_bilgisayar_id      = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, BILGISAYAR_ID_COLUMN  );
    QSearchEdit * satir_searchEdit_kasa_hesap_kodu  = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_row_number, MUH_KASA_HESAP_KODU_COLUMN  );
    QLineEdit   * satir_lineEdit_muh_kasa_hesap_id  = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, MUH_KASA_HESAP_ID_COLUMN    );
    QLineEdit   * satir_lineEdit_muhasebe_hesap_adi = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, MUH_KASA_HESAP_ADI_COLUMN   );


    sql_query.PREPARE_SELECT ( "sub_uniteler","unite_id ",
                               "unite_adi = :unite_adi AND sube_id = :sube_id" );
    sql_query.SET_VALUE      ( ":unite_adi", satir_lineEdit_unite_adi->text() );
    sql_query.SET_VALUE      ( ":sube_id"  , p_sube_id );
    if ( sql_query.SELECT() > 0 ) {
       MSG_WARNING( tr ( "Bu şubede bu adla bir kasa / ünite kaydı yapılmış. Lütfen ünite adını değiştiriniz"), satir_lineEdit_unite_adi );

        return ADAK_FAIL;
    }

    if ( satir_searchEdit_kasa_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {

        MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

        MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

        QString kasa_muhasebe_hesap_kodu;

        SUBE_GET_SUBE_ENT_HESAP_ID( m_sube_id, "kasa_ent_hesap_id", &kasa_muhasebe_hesap_kodu );

        if ( MUH_HESAPLAR_TABLOSUNU_OKU( MUHASEBE_HESABI , kasa_muhasebe_hesap_kodu ) EQ 0 ) {
            MSG_WARNING(  tr ( "Muhasebe hesabı bulunamadı." ), satir_searchEdit_kasa_hesap_kodu );

            return ADAK_FAIL;
        }

        if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {
            MSG_WARNING(  tr ( "Seçilen hesap tali hesap değil. Entegrasyon yapılamaz."), satir_searchEdit_kasa_hesap_kodu );

            return false;
        }

        satir_lineEdit_muhasebe_hesap_adi->setText ( MUHASEBE_HESABI->hesap_ismi );
        satir_lineEdit_muh_kasa_hesap_id->setText ( QVariant ( MUHASEBE_HESABI->hesap_id ).toString() );
    }

    if ( SECILI_BILGISAYAR_BASKA_UNITEYE_KAYITLI_MI (  satir_lineEdit_bilgisayar_id->text().toInt() ) EQ false ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::ADD_LINE
***************************************************************************************/

void SUBE_FISI::ADD_LINE ( int p_sube_id, int p_row_number )
{
    SQL_QUERY sql_query     ( DB );

    int           order_number                  =  m_ui->tableWidget_subenin_uniteleri->item( p_row_number, ORDER_COLUMN )->text().toInt();
    QLineEdit   * satir_lineEdit_unite_adi    = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, UNITE_ADI_COLUMN    );
    QLineEdit   * satir_lineEdit_sube_id           = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, SUBE_ID_COLUMN           );
    QLineEdit   * satir_lineEdit_bilgisayar_id     = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, BILGISAYAR_ID_COLUMN     );
    QLineEdit   * satir_lineEdit_muh_hesap_id      = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, MUH_KASA_HESAP_ID_COLUMN );
    QLineEdit   * satir_lineEdit_depo_id           = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, DEPO_ID_COLUMN           );

    int unite_muh_kasa_hesap_id                    = satir_lineEdit_muh_hesap_id->text().toInt();
    int bilgisayar_id                              = satir_lineEdit_bilgisayar_id->text().toInt();

    sql_query.PREPARE_INSERT ( "sub_uniteler","unite_id","order_number, "
                               "unite_adi, sube_id, "
                               "bilgisayar_id, muh_kasa_hesap_id, depo_id  ");


    sql_query.SET_VALUE      ( ":order_number"          ,   order_number );
    sql_query.SET_VALUE      ( ":unite_adi"                ,   satir_lineEdit_unite_adi->text());
    sql_query.SET_VALUE      ( ":sube_id"                  ,   p_sube_id );
    sql_query.SET_VALUE      ( ":bilgisayar_id"            ,   bilgisayar_id );
    sql_query.SET_VALUE      ( ":muh_kasa_hesap_id"        ,   unite_muh_kasa_hesap_id );
    sql_query.SET_VALUE      ( ":depo_id"                  ,   satir_lineEdit_depo_id->text().toInt() );

    int unite_id = sql_query.INSERT();

    sql_query.PREPARE_SELECT ( "sub_subeler","sube_kodu,sube_adi","sube_id = :sube_id");
    sql_query.SET_VALUE      ( ":sube_id" , p_sube_id );

    if(sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString sube_kodu_adi = "Şube Kodu / Adı : " + sql_query.VALUE(0).toString() + "  "
                            + sql_query.VALUE(1).toString() + " , Kasa Ünite Adı : "
                            + satir_lineEdit_unite_adi->text();

    E9_LOG_KAYDI_EKLE(SUBE_MODULU , LOG_SUBE_UNITELER , LOG_ISLEM_ADD , sube_kodu_adi);

    satir_lineEdit_sube_id->setText ( QVariant ( p_sube_id ).toString() );

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( unite_id ).toString());


    m_ui->tableWidget_subenin_uniteleri->setItem ( p_row_number, UNITE_ID_COLUMN, new_item );

    new_item = new QTableWidgetItem ( QVariant ( p_sube_id ).toString());

    m_ui->tableWidget_subenin_uniteleri->setItem ( p_row_number, SUBE_ID_COLUMN, new_item );


}

/**************************************************************************************
                   SUBE_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int SUBE_FISI::CHECK_UPDATE_LINE ( int p_sube_id, int p_row_number )
{

    SQL_QUERY sql_query     ( DB );

    QLineEdit   * satir_lineEdit_unite_adi = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, UNITE_ADI_COLUMN     );
    QLineEdit   * satir_lineEdit_bilgisayar_id  = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, BILGISAYAR_ID_COLUMN );

    int unite_id = m_ui->tableWidget_subenin_uniteleri->item ( p_row_number, UNITE_ID_COLUMN )->text().toInt();

    sql_query.PREPARE_SELECT ( "sub_uniteler","unite_id","unite_adi = :unite_adi "
                               "AND sube_id = :sube_id AND unite_id != :unite_id" );

    sql_query.SET_VALUE      ( ":unite_adi"  ,   satir_lineEdit_unite_adi->text() );
    sql_query.SET_VALUE      ( ":sube_id"    ,   p_sube_id  );
    sql_query.SET_VALUE      ( ":unite_id"   ,   unite_id   );

    if ( sql_query.SELECT() > 0 ) {
        MSG_WARNING(  tr("Bu şubede bu adla bir kasa / ünite kaydı yapılmış. Lütfen ünite adını değiştiriniz"), satir_lineEdit_unite_adi );

        return ADAK_FAIL;
    }

    if ( SECILI_BILGISAYAR_BASKA_UNITEYE_KAYITLI_MI ( satir_lineEdit_bilgisayar_id->text().toInt(), unite_id ) EQ false ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::UPDATE_LINE
***************************************************************************************/

void SUBE_FISI::UPDATE_LINE ( int p_sube_id, int p_row_number )
{
    SQL_QUERY sql_query ( DB );



    QLineEdit   * satir_lineEdit_unite_adi = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, UNITE_ADI_COLUMN    );
    QLineEdit   * satir_lineEdit_bilgisayar_id  = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, BILGISAYAR_ID_COLUMN     );
    QLineEdit   * satir_lineEdit_muh_hesap_id   = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, MUH_KASA_HESAP_ID_COLUMN );
    QLineEdit   * satir_lineEdit_depo_id        = ( QLineEdit   * ) m_ui->tableWidget_subenin_uniteleri->cellWidget( p_row_number, DEPO_ID_COLUMN           );

    int           unite_id                 = m_ui->tableWidget_subenin_uniteleri->item ( p_row_number, UNITE_ID_COLUMN )->text().toInt();
    int           bilgisayar_id                 = satir_lineEdit_bilgisayar_id->text().toInt();
    int           unite_muh_kasa_hesap_id       = satir_lineEdit_muh_hesap_id->text().toInt();

    sql_query.PREPARE_UPDATE ( "sub_uniteler","unite_id","unite_adi , sube_id , "
                               "bilgisayar_id, muh_kasa_hesap_id , "
                               "depo_id","unite_id = :unite_id");

    sql_query.SET_VALUE      ( ":unite_adi"                     ,       satir_lineEdit_unite_adi->text());
    sql_query.SET_VALUE      ( ":sube_id"                       ,       p_sube_id  );
    sql_query.SET_VALUE      ( ":bilgisayar_id"                 ,       bilgisayar_id  );
    sql_query.SET_VALUE      ( ":muh_kasa_hesap_id"             ,       unite_muh_kasa_hesap_id );
    sql_query.SET_VALUE      ( ":depo_id"                       ,       satir_lineEdit_depo_id->text().toInt() );
    sql_query.SET_VALUE      ( ":unite_id"                      ,       unite_id );

    sql_query.UPDATE();

    sql_query.PREPARE_SELECT ( "sub_subeler","sube_kodu,sube_adi","sube_id = :sube_id");
    sql_query.SET_VALUE      ( ":sube_id" , p_sube_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString sube_kodu_adi = "Şube Kodu / Adı : " + sql_query.VALUE(0).toString() + "  "
                            + sql_query.VALUE(1).toString() + " , Kasa Ünite Adı : "
                            + satir_lineEdit_unite_adi->text();

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_UNITELER , LOG_ISLEM_UPDATE , sube_kodu_adi );
}

/**************************************************************************************
                   SUBE_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int SUBE_FISI::CHECK_DELETE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_FISI::DELETE_LINE
***************************************************************************************/

void SUBE_FISI::DELETE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );

    int unite_id      = m_ui->tableWidget_subenin_uniteleri->item(row_number, UNITE_ID_COLUMN )->text().toInt();

    int sube_id = m_ui->tableWidget_subenin_uniteleri->item ( row_number,SUBE_ID_COLUMN )->text().toInt();

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "sub_uniteler","unite_adi","unite_id = :unite_id");
    sql_query.SET_VALUE      ( ":unite_id" , unite_id )  ;

    if(sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString unite_adi = sql_query.VALUE(0).toString();

    sql_query.PREPARE_SELECT ( "sub_subeler","sube_kodu,sube_adi ","sube_id = :sube_id" );
    sql_query.SET_VALUE      ( ":sube_id" , sube_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString sube_kodu_adi = "Şube Kodu / Adı : " + sql_query.VALUE(0).toString() + "  "
                            + sql_query.VALUE(1).toString() + " , Kasa Ünite Adı : "
                            + unite_adi;

    E9_LOG_KAYDI_EKLE ( SUBE_MODULU , LOG_SUBE_UNITELER , LOG_ISLEM_DELETE , sube_kodu_adi );


    sql_query.PREPARE_DELETE ( "sub_uniteler","unite_id = :unite_id" );
    sql_query.SET_VALUE      ( ":unite_id", unite_id );
    sql_query.DELETE();

    SUBE_SET_CURRENT_SUBE_UNITE();
}

/**************************************************************************************
                   SUBE_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int SUBE_FISI::LOCK_FIS_RECORD ( int sube_id )
{
    return DB->LOCK_ROW ( "sube_id","sub_subeler", QString ( "sube_id = %1" ).arg ( sube_id ) );
}

/**************************************************************************************
                   SUBE_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void SUBE_FISI::UNLOCK_FIS_RECORD ( int sube_id )
{
    DB->UNLOCK_ROW ( "sube_id","sub_subeler", QString ( "sube_id = %1" ).arg ( sube_id ) );
}


/**************************************************************************************
                   SUBE_FISI::SECILI_BILGISAYAR_BASKA_KASAYA_UNITEYE_KAYITLI_MI
***************************************************************************************/

bool SUBE_FISI::SECILI_BILGISAYAR_BASKA_UNITEYE_KAYITLI_MI ( int p_bilgisayar_id, int p_unite_id )
{
    SQL_QUERY sql_query     ( DB );

    QString where_str = "bilgisayar_id = :bilgisayar_id";
    if ( p_unite_id NE -1 ) {

        where_str += " AND unite_id != :unite_id";

    }

    sql_query.PREPARE_SELECT("sub_uniteler","unite_id",where_str);

    sql_query.SET_VALUE      ( ":bilgisayar_id", p_bilgisayar_id );

    if ( p_unite_id NE -1 ) {
        sql_query.SET_VALUE ( ":unite_id", p_unite_id );
    }

    if ( sql_query.SELECT() > 0 ) {
        MSG_WARNING(  tr ( "Bu bilgisayar bir kasada / ünitede kayıtlı. "
                                                      "Lütfen başka bir bilgisayar seçimi yapınız." ), NULL );
        return false;
    }
    return true;
}

/**************************************************************************************
                   SUBE_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void SUBE_FISI::LINE_SEARCH_EDIT_CLICKED ( int p_current_row, QWidget * p_searchEdit, QLineEdit * p_lineEdit )
{
    Q_UNUSED ( p_lineEdit );

    SQL_QUERY yonetim_query ( G_YONETIM_DB );

    QSearchEdit * satir_searchEdit_bilgisayar_kodu  = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_current_row, BILGISAYAR_KODU_COLUMN      );
    QSearchEdit * satir_searchEdit_kasa_hesap_kodu  = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_current_row, MUH_KASA_HESAP_KODU_COLUMN  );
    QSearchEdit * satir_searchEdit_depo_kodu        = ( QSearchEdit * ) m_ui->tableWidget_subenin_uniteleri->cellWidget ( p_current_row, DEPO_KODU_COLUMN            );

    if ( p_searchEdit EQ satir_searchEdit_bilgisayar_kodu ) {

        int bilgisayar_id = BILGISAYAR_SEC ( this );
        if ( bilgisayar_id > 0 ) {

            yonetim_query.PREPARE_SELECT ( "ynt_bilgisayarlar","bilgisayar_kodu, bilgisayar_adi  ",
                                           "bilgisayar_id = :bilgisayar_id AND silinmis_mi = :silinmis_mi " );

            yonetim_query.SET_VALUE      ( ":bilgisayar_id", bilgisayar_id );
            yonetim_query.SET_VALUE      ( ":silinmis_mi",0);

            if ( yonetim_query.SELECT() > 0 ) {
                yonetim_query.NEXT();
                p_lineEdit->setText ( yonetim_query.VALUE(0).toString() );
            }
        }
        return;
    }
    else if ( p_searchEdit EQ satir_searchEdit_depo_kodu ) {

        int sube_id = GET_RECORD_ID();

        if ( sube_id < 1 ) {
            MSG_WARNING(  tr ( "Şubeyi kaydetmeden depoyu seçemezsiniz." ), NULL );
            return;
        }

        int depo_id = SUBE_DEPO_SEC(sube_id , this);

        if ( depo_id < 1 ) {
            return;
        }

        QString depo_kodu;
        QString depo_adi;

        SUBE_GET_DEPO_KODU_ADI ( depo_id, &depo_kodu, &depo_adi );
        p_lineEdit->setText ( depo_kodu );

    }
    else if ( p_searchEdit EQ satir_searchEdit_kasa_hesap_kodu ) {

        int unite_muh_kasa_hesap_id = OPEN_MUH_HESAP_ARAMA( satir_searchEdit_kasa_hesap_kodu->GET_TEXT(), this, 1 );

        if ( unite_muh_kasa_hesap_id > 0 ) {
            satir_searchEdit_kasa_hesap_kodu->SET_TEXT ( MUH_GET_HESAP_KODU ( unite_muh_kasa_hesap_id ) );
        }
    }

}

/**************************************************************************************
                   SUBE_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void SUBE_FISI::SAVER_BUTTON_CLICKED(QAbstractButton *button, int record_id)
{
    if ( button EQ m_ui->tool_button_sube_ent_hesaplari ) {
        OPEN_SUBE_ENTEGRASYON_HESAPLARI_FORMU ( record_id , this );
    }
    else if ( button EQ m_ui->tool_button_depolar ) {
        OPEN_SUBE_DEPOLARI_FORMU( record_id,-1, NULL );
    }

}

/**************************************************************************************
                   SUBE_FISI::LINE_SAVER_BUTTON_CLICKED
***************************************************************************************/

void SUBE_FISI::LINE_SAVER_BUTTON_CLICKED ( QAbstractButton *button, int row_id, int record_id, int row_number )
{
    Q_UNUSED ( button );
    Q_UNUSED ( row_id );
    Q_UNUSED ( record_id );

    int unite_id = m_ui->tableWidget_subenin_uniteleri->item ( row_number, UNITE_ID_COLUMN )->text().toInt();
    OPEN_SUBE_UNITE_DETAYLARI_FORMU ( unite_id, SUBE_MODULU, this );
}

