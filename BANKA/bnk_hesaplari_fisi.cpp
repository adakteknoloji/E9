#include <QMessageBox>
#include "bnk_hesaplari_fisi_class.h"
#include "bnk_hesaplari_fisi_open.h"
#include "ui_bnk_hesaplari_fisi.h"
#include "e9_console_utils.h"
#include "e9_enum.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "adak_std_utils.h"
#include "banka_enum.h"
#include "bnk_hesabin_kredi_kartlari_fisi_open.h"
#include "muh_console_utils.h"
#include "e9_log.h"
#include "yonetim.h"
#include "sube_console_utils.h"
#include "bnk_hesaplari_arama_open.h"
 
#include "muh_hesap_arama_open.h"
#include "dvz_gui_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"

extern ADAK_SQL *           DB;

#define ROW_ID_COLUMN              0
#define FIS_ID_COLUMN              1
#define ORDER_COLUMN               2
#define POS_MUH_HESAP_ID_COLUMN    3
#define POS_KOM_HESAP_ID_COLUMN    4
#define POS_NO_COLUMN              5
#define ADRES_COLUMN               6
#define TELEFON_COLUMN             7
#define ACIKLAMA_COLUMN            8
#define MUHASEBE_HESABI_COLUMN     9
#define MUHASEBE_HESAP_ISMI_COLUMN 10
#define KOMISYON_HESABI_COLUMN     11
#define KOMISYON_HESAP_ISMI_COLUMN 12

/**************************************************************************************
                   OPEN_BNK_HESAPLARI_FISI
***************************************************************************************/

void OPEN_BNK_HESAPLARI_FISI ( int record_id, QWidget * parent )
{
    BNK_HESAPLARI_FISI * F = new BNK_HESAPLARI_FISI ( record_id, parent );
     
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::BNK_HESAPLARI_FISI
***************************************************************************************/

BNK_HESAPLARI_FISI::BNK_HESAPLARI_FISI(int , QWidget * parent) :FIS_KERNEL(parent), m_ui ( new Ui::BNK_HESAPLARI_FISI )
{
    m_ui->setupUi    ( this );
    START_FIS_KERNEL ( this, DB );
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::SETUP_FORM
***************************************************************************************/

void BNK_HESAPLARI_FISI::SETUP_FORM()
{

    SET_HELP_PAGE  ( "banka-islemleri_banka-hesaplari" );

    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_buttons_widget );
    REGISTER_TABLE_WIDGET   ( m_ui->tablewidget_pos_bilgileri, 13 );

    m_bnk_muh_hesap_id            = 0;
    m_bnk_verilen_cekler_hesap_id = 0;
    m_bnk_odeme_emirleri_hesap_id = 0;

    DISABLE_CLEAR_ON_WIDGET ( m_ui->searchEdit_banka_muhasebe_hesap_kodu             );
    DISABLE_CLEAR_ON_WIDGET ( m_ui->lineedit_banka_muh_hesap_ismi                    );
    DISABLE_CLEAR_ON_WIDGET ( m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu    );
    DISABLE_CLEAR_ON_WIDGET ( m_ui->lineEdit_verilen_cekler_muh_hesap_ismi           );
    DISABLE_CLEAR_ON_WIDGET ( m_ui->searchEdit_odeme_emirleri_muh_kodu               );
    DISABLE_CLEAR_ON_WIDGET ( m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi           );
    DISABLE_CLEAR_ON_WIDGET ( m_ui->lineedit_doviz_cinsi                             );

    m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi->setMaxLength(60);
     
    QString banka_hesap_kodu, banka_hesap_adi;

    m_bnk_muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "banka_ent_hesap_id", &banka_hesap_kodu, &banka_hesap_adi );

    m_ui->searchEdit_banka_muhasebe_hesap_kodu->SET_TEXT( banka_hesap_kodu );
    m_ui->lineedit_banka_muh_hesap_ismi->setText        ( banka_hesap_adi );

    QString ver_cek_hesap_kodu, ver_cek_hesap_adi;

    m_bnk_verilen_cekler_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "ver_cek_ent_hesap_id", &ver_cek_hesap_kodu, &ver_cek_hesap_adi );
    m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu->SET_TEXT   ( ver_cek_hesap_kodu );
    m_ui->lineEdit_verilen_cekler_muh_hesap_ismi->setText           ( ver_cek_hesap_adi  );

    QString odeme_emirleri_hesap_kodu, odeme_emirleri_hesap_adi;

    m_bnk_odeme_emirleri_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "odeme_emirleri_ent_hesap_id", &odeme_emirleri_hesap_kodu, &odeme_emirleri_hesap_adi );

    m_ui->searchEdit_odeme_emirleri_muh_kodu->SET_TEXT      (odeme_emirleri_hesap_kodu);
    m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi->setText   (odeme_emirleri_hesap_adi );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->searchEdit_banka_muhasebe_hesap_kodu->setHidden(true);
        m_ui->lineedit_banka_muh_hesap_ismi->setHidden(true);
        m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu->setHidden(true);
        m_ui->lineEdit_verilen_cekler_muh_hesap_ismi->setHidden(true);
        m_ui->searchEdit_odeme_emirleri_muh_kodu->setHidden(true);
        m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi->setHidden(true);
        m_ui->label_banka_muhasebe_hesabi->setHidden(true);
        m_ui->label_verilen_cekler_muhasebe_hesabi->setHidden(true);
        m_ui->label_odeme_emirleri_muh_hesabi->setHidden(true);
    }

    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION  ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN ( 5 );
    SET_FIS_ORDER_DATABASE      ( "bnk_hesabin_poslari", "order_number", "pos_id" );
    SET_FIRST_FOCUS_WIDGET      ( m_ui->lineedit_banka_hesap_kodu );

    REGISTER_SAVER_BUTTON(m_ui->buton_kredi_kartlari);

    GET_BANKALAR(m_ui->combobox_banka_ismi);

    DISABLE_CLEAR_ON_WIDGET(m_ui->combobox_doviz_cinsi);

    DVZ_FILL_COMBOBOX ( m_ui->combobox_doviz_cinsi );
    m_ui->combobox_doviz_cinsi->setCurrentIndex(m_ui->combobox_doviz_cinsi->findText(DVZ_GET_TEMEL_PARA_BIRIMI_KODU()));
    m_ui->lineedit_doviz_cinsi->setText(DVZ_GET_DOVIZ_ADI(DVZ_GET_TEMEL_PARA_BIRIMI_ID()));

    SET_TABLE_ROW_WIDGETS(MUHASEBE_HESABI_COLUMN     , WIDGET_SEARCH_EDIT);
    SET_TABLE_ROW_WIDGETS(KOMISYON_HESABI_COLUMN     , WIDGET_SEARCH_EDIT);
    SET_TABLE_ROW_WIDGETS(POS_NO_COLUMN              , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(ADRES_COLUMN               , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(TELEFON_COLUMN             , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(ACIKLAMA_COLUMN            , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(MUHASEBE_HESAP_ISMI_COLUMN , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(KOMISYON_HESAP_ISMI_COLUMN , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(POS_MUH_HESAP_ID_COLUMN    , WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(POS_KOM_HESAP_ID_COLUMN    , WIDGET_LINE_EDIT);

    SET_PAGE_TITLE( tr ( "BNK - BANKA HESAPLARI" ) );

    m_ui->tablewidget_pos_bilgileri->setHorizontalHeaderLabels(QStringList()<<"row id"<<"fis id"<<"column order"
                                                                            <<"pos_muh_id"<< "pos_kom_id"<<tr("Üye İşyeri")
                                                                            <<tr("Üye İşyeri Adresi")<<tr("Telefon")
                                                                            <<tr("Açıklama")<<tr("Muhasebe Hesabı")<<tr("/ Hesap İsmi")
                                                                            <<tr("Komisyon Gideri Hesabı")<<tr("/ Hesap İsmi")
                                                                            <<""<<"");

    m_ui->tablewidget_pos_bilgileri->setColumnWidth ( POS_NO_COLUMN,                148 );
    m_ui->tablewidget_pos_bilgileri->setColumnWidth ( ADRES_COLUMN,                 162 );
    m_ui->tablewidget_pos_bilgileri->setColumnWidth ( TELEFON_COLUMN,               111 );
    m_ui->tablewidget_pos_bilgileri->setColumnWidth ( ACIKLAMA_COLUMN,              119 );
    m_ui->tablewidget_pos_bilgileri->setColumnWidth ( MUHASEBE_HESABI_COLUMN,       129 );
    m_ui->tablewidget_pos_bilgileri->setColumnWidth ( KOMISYON_HESABI_COLUMN,       158 );
    m_ui->tablewidget_pos_bilgileri->setColumnWidth ( MUHASEBE_HESAP_ISMI_COLUMN,   160 );
    m_ui->tablewidget_pos_bilgileri->setColumnWidth ( KOMISYON_HESAP_ISMI_COLUMN,   160 );

    SET_SETTING_NAME ( "BANKA_HESAPLARI_FISI" );


    m_ui->tablewidget_pos_bilgileri->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_pos_bilgileri->setFocus( Qt::OtherFocusReason );

    m_ui->lineedit_banka_hesap_kodu->setMaxLength             ( 20 );
    m_ui->lineedit_banka_muh_hesap_ismi->setMaxLength         ( 50 );
    m_ui->lineedit_doviz_cinsi->setMaxLength                  ( 64 );
    m_ui->lineedit_hesap_numarasi->setMaxLength               ( 20 );
    m_ui->lineEdit_iban_no->setMaxLength                      ( 26 );
    m_ui->lineedit_sube_kodu->setMaxLength                    ( 20 );
    m_ui->lineEdit_verilen_cekler_muh_hesap_ismi->setMaxLength( 60 );
    m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi->setMaxLength( 60 );


    SET_COMBOBOX_LENGTH(m_ui->combobox_banka_ismi , 50);
    SET_COMBOBOX_LENGTH(m_ui->combobox_sube_ismi  , 50);

    m_ui->lineEdit_banka_hesap_unvani->setMaxLength( 60 );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->tablewidget_pos_bilgileri->hideColumn(MUHASEBE_HESABI_COLUMN);
        m_ui->tablewidget_pos_bilgileri->hideColumn(MUHASEBE_HESAP_ISMI_COLUMN);
        m_ui->tablewidget_pos_bilgileri->hideColumn(KOMISYON_HESABI_COLUMN);
        m_ui->tablewidget_pos_bilgileri->hideColumn(KOMISYON_HESAP_ISMI_COLUMN);
    }

    FOCUS_FIRST_WIDGET ();
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void BNK_HESAPLARI_FISI::CLEAR_FORM_MEMBERS()
{

}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::NEW_FIS_RECORD
***************************************************************************************/

void BNK_HESAPLARI_FISI::NEW_FIS_RECORD()
{
    m_ui->lineEdit_iban_no->setMaxLength(26);

    m_ui->lineEdit_banka_hesap_unvani->setText( VERITABANI_TANIMI() );

}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::GET_FIS_RECORD
***************************************************************************************/

int BNK_HESAPLARI_FISI::GET_FIS_RECORD ( int record_id )
{
    double toplam_borc_array   [E9_ARRAY_SIZE];
    double toplam_alacak_array [E9_ARRAY_SIZE];

    double borc_bakiye      =   0.0;
    double alacak_bakiye    =   0.0;

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT ( "bnk_hesaplar,bnk_pos_arrayler,bnk_kk_arrayler",
                                  "banka_ismi, sube_kodu, sube_ismi, hesap_numarasi, doviz_id, "
                                  "muh_hesap_id, banka_hesap_kodu, iban_numarasi,verilen_cekler_hesap_id, "
                                  "toplam_kk_tahsilat_borc_array , toplam_kk_thslat_alacak_array,"
                                  "toplam_kk_odeme_borc_array,toplam_kk_odeme_alacak_array,"
                                  "toplam_borc_array , toplam_alacak_array ,"
                                  "odeme_emirleri_hesap_id, banka_hesap_unvani ",
                                  "bnk_hesaplar.hesap_no_id = :hesap_no_id AND "
                                  "bnk_hesaplar.hesap_no_id = bnk_pos_arrayler.hesap_no_id "
                                  "AND bnk_hesaplar.hesap_no_id = bnk_kk_arrayler.hesap_no_id");

    select_query.SET_VALUE(":hesap_no_id",record_id);

    if ( select_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    select_query.NEXT();

    m_ui->combobox_banka_ismi->setCurrentIndex(m_ui->combobox_banka_ismi->findText(select_query.VALUE(0).toString()));

    if (m_ui->combobox_banka_ismi->findText(select_query.VALUE(0).toString()) EQ -1 ) {
        m_ui->combobox_banka_ismi->addItem(select_query.VALUE(0).toString());
    }

    m_ui->lineedit_sube_kodu->setText(select_query.VALUE(1).toString());
    m_ui->lineEdit_banka_hesap_unvani->setText( select_query.VALUE( "banka_hesap_unvani" ).toString() );

    GET_BANKA_SUBE_ADLARI(m_ui->combobox_sube_ismi,select_query.VALUE(0).toString()  );

    if (m_ui->combobox_sube_ismi->findText(select_query.VALUE(2).toString()) EQ -1 ) {
        m_ui->combobox_sube_ismi->addItem(select_query.VALUE(2).toString());
    }
    m_ui->combobox_sube_ismi->setCurrentIndex(m_ui->combobox_sube_ismi->findText(select_query.VALUE(2).toString()));

    m_ui->lineedit_hesap_numarasi->setText(select_query.VALUE(3).toString());

    m_ui->combobox_doviz_cinsi->setCurrentIndex(m_ui->combobox_doviz_cinsi->findText(DVZ_GET_DOVIZ_KODU(select_query.VALUE(4).toInt())));
    m_ui->lineedit_doviz_cinsi->setText(DVZ_GET_DOVIZ_ADI(select_query.VALUE(4).toInt() ));

    m_ui->lineedit_banka_hesap_kodu->setText(select_query.VALUE(6).toString());

    int     banka_muh_hesap_id                  = select_query.VALUE(5).toInt();

    m_ui->lineEdit_iban_no->setText(select_query.VALUE(7).toString());


    m_bnk_muh_hesap_id             = banka_muh_hesap_id;
    m_bnk_verilen_cekler_hesap_id  = select_query.VALUE("verilen_cekler_hesap_id").toInt();
    m_bnk_odeme_emirleri_hesap_id  = select_query.VALUE("odeme_emirleri_hesap_id").toInt();

    UNPACK_DOUBLE_ARRAY(select_query.VALUE("toplam_borc_array").toString(),toplam_borc_array,E9_ARRAY_SIZE);
    UNPACK_DOUBLE_ARRAY(select_query.VALUE("toplam_alacak_array").toString(),toplam_alacak_array,E9_ARRAY_SIZE);
    double toplam_borc   = toplam_borc_array[E9_ARRAY_SIZE-1];
    double toplam_alacak = toplam_alacak_array[E9_ARRAY_SIZE-1];

    if (toplam_borc > toplam_alacak) {
        borc_bakiye = toplam_borc - toplam_alacak;
    }
    else {
        alacak_bakiye = toplam_alacak - toplam_borc;
    }
    m_ui->commaedit_hsp_borc_bky->SET_DOUBLE(borc_bakiye);
    m_ui->commaedit_hsp_alacak_bky->SET_DOUBLE(alacak_bakiye);


    UNPACK_DOUBLE_ARRAY(select_query.VALUE("toplam_kk_tahsilat_borc_array").toString(),toplam_borc_array,E9_ARRAY_SIZE);
    UNPACK_DOUBLE_ARRAY(select_query.VALUE("toplam_kk_thslat_alacak_array").toString(),toplam_alacak_array,E9_ARRAY_SIZE);

    toplam_borc   = toplam_borc_array[E9_ARRAY_SIZE-1];
    toplam_alacak = toplam_alacak_array[E9_ARRAY_SIZE-1];

    borc_bakiye   = 0.0;
    alacak_bakiye = 0.0;

    if (toplam_borc > toplam_alacak) {
        borc_bakiye = toplam_borc - toplam_alacak;
    }
    else {
        alacak_bakiye = toplam_alacak - toplam_borc;
    }
    m_ui->commaedit_kk_tah_borc_bky->SET_DOUBLE(borc_bakiye);
    m_ui->commaedit_kk_tah_alacak_bky->SET_DOUBLE(alacak_bakiye);

    UNPACK_DOUBLE_ARRAY(select_query.VALUE("toplam_kk_odeme_borc_array").toString(),toplam_borc_array,E9_ARRAY_SIZE);
    UNPACK_DOUBLE_ARRAY(select_query.VALUE("toplam_kk_odeme_alacak_array").toString(),toplam_alacak_array,E9_ARRAY_SIZE);

    toplam_borc   = toplam_borc_array[E9_ARRAY_SIZE-1];
    toplam_alacak = toplam_alacak_array[E9_ARRAY_SIZE-1];

    borc_bakiye   = 0.0;
    alacak_bakiye = 0.0;
    if (toplam_borc > toplam_alacak) {
        borc_bakiye = toplam_borc - toplam_alacak;
    }
    else {
        alacak_bakiye = toplam_alacak - toplam_borc;
    }

    m_ui->commaedit_kk_odm_borc_bky->SET_DOUBLE(borc_bakiye);
    m_ui->commaedit_kk_odm_alacak_bky->SET_DOUBLE(alacak_bakiye);

    SQL_QUERY sql_query ( DB );

    QString tam_hesap_kodu,hesap_ismi;

    MUH_GET_HESAP_KODU_HESAP_ISMI(banka_muh_hesap_id , tam_hesap_kodu,hesap_ismi);

    m_ui->searchEdit_banka_muhasebe_hesap_kodu->SET_TEXT(tam_hesap_kodu);
    m_ui->lineedit_banka_muh_hesap_ismi->setText(hesap_ismi);


    MUH_GET_HESAP_KODU_HESAP_ISMI(m_bnk_verilen_cekler_hesap_id , tam_hesap_kodu,hesap_ismi);


    m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu->SET_TEXT ( tam_hesap_kodu );
    m_ui->lineEdit_verilen_cekler_muh_hesap_ismi->setText         ( hesap_ismi );


    MUH_GET_HESAP_KODU_HESAP_ISMI(m_bnk_odeme_emirleri_hesap_id , tam_hesap_kodu,hesap_ismi);


    m_ui->searchEdit_odeme_emirleri_muh_kodu->SET_TEXT ( tam_hesap_kodu );
    m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi->setText ( hesap_ismi );


    select_query.PREPARE_SELECT("bnk_hesabin_poslari","pos_id,pos_numarasi,uye_isyeri_adresi,uye_isyeri_telefonu,aciklama,"
                                "muh_hesap_id,kom_hesap_id ","hesap_no_id = :hesap_no_id");
    select_query.SET_VALUE(":hesap_no_id"  , record_id);

    if (select_query.SELECT("order_number") EQ 0 ) {
        return ADAK_OK;
    }

    int current_row;
    QWidget * cell_widget;

    while (select_query.NEXT()) {
        current_row = ADD_NEW_LINE();

        cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,POS_NO_COLUMN);
        QLineEdit * lineedit_pos_no   = static_cast<QLineEdit*>(cell_widget);
        lineedit_pos_no->setText(select_query.VALUE(1).toString());

        cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,ADRES_COLUMN);
        QLineEdit * lineedit_adres    = static_cast <QLineEdit*> (cell_widget);
        lineedit_adres->setText(select_query.VALUE(2).toString());

        cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,TELEFON_COLUMN);
        QLineEdit * lineedit_telefon  = static_cast<QLineEdit*> (cell_widget);
        lineedit_telefon->setText(select_query.VALUE(3).toString());

        cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,ACIKLAMA_COLUMN);
        QLineEdit * lineedit_aciklama = static_cast <QLineEdit*> (cell_widget);
        lineedit_aciklama->setText(select_query.VALUE(4).toString());

        cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,MUHASEBE_HESABI_COLUMN);
        QSearchEdit * searchedit_muh_hesabi = static_cast <QSearchEdit*> (cell_widget);

        cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,MUHASEBE_HESAP_ISMI_COLUMN);
        QLineEdit * lineedit_muh_hesap_ismi = static_cast <QLineEdit*> (cell_widget);

        QLineEdit * lineedit_pos_muh_hesap_id = (QLineEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,POS_MUH_HESAP_ID_COLUMN);
        lineedit_pos_muh_hesap_id->setText(select_query.VALUE(5).toString());

        QLineEdit * lineedit_pos_kom_hesap_id = (QLineEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,POS_KOM_HESAP_ID_COLUMN);
        lineedit_pos_kom_hesap_id->setText(select_query.VALUE(6).toString());

        QString tam_hesap_kodu,hesap_ismi;

        MUH_GET_HESAP_KODU_HESAP_ISMI(select_query.VALUE(5).toInt(),tam_hesap_kodu,hesap_ismi);

        searchedit_muh_hesabi->SET_TEXT(tam_hesap_kodu);
        lineedit_muh_hesap_ismi->setText(hesap_ismi);

        cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,KOMISYON_HESABI_COLUMN);
        QSearchEdit * searchedit_komisyon_hesabi = static_cast <QSearchEdit*> (cell_widget);


        cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(current_row,KOMISYON_HESAP_ISMI_COLUMN);
        QLineEdit * lineedit_kom_hesap_ismi = static_cast <QLineEdit*> (cell_widget);

        MUH_GET_HESAP_KODU_HESAP_ISMI(select_query.VALUE(6).toInt() , tam_hesap_kodu,hesap_ismi);

        searchedit_komisyon_hesabi->SET_TEXT(tam_hesap_kodu);
        lineedit_kom_hesap_ismi->setText(hesap_ismi);

        QTableWidgetItem * new_item = new QTableWidgetItem(QVariant(record_id).toString());
        m_ui->tablewidget_pos_bilgileri->setItem(current_row,FIS_ID_COLUMN,new_item);

        new_item = new QTableWidgetItem(select_query.VALUE(0).toString());
        m_ui->tablewidget_pos_bilgileri->setItem(current_row,ROW_ID_COLUMN,new_item);

    }

    return ADAK_OK;

}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void BNK_HESAPLARI_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QWidget * cell_widget = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,MUHASEBE_HESAP_ISMI_COLUMN);
    QLineEdit * lineedit_muh_hesap_ismi = static_cast <QLineEdit*> (cell_widget);
    lineedit_muh_hesap_ismi->setReadOnly(true);
    lineedit_muh_hesap_ismi->setMaxLength(60);

    cell_widget = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,KOMISYON_HESAP_ISMI_COLUMN);
    QLineEdit * lineedit_komisyon_hesap_ismi = static_cast <QLineEdit*> (cell_widget);
    lineedit_komisyon_hesap_ismi->setReadOnly(true);
    lineedit_komisyon_hesap_ismi->setMaxLength(60);

    QWidget * cell_widget_pos_no = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_NO_COLUMN);
    QLineEdit * pos_no = static_cast <QLineEdit*> (cell_widget_pos_no);
    pos_no->setMaxLength(20);

    QLineEdit *     lineedit_muh_hesap_id           = ( QLineEdit   * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number , POS_MUH_HESAP_ID_COLUMN );
    QSearchEdit *   searchedit_muh_hesap_kodu       = ( QSearchEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number , MUHASEBE_HESABI_COLUMN  );
    QLineEdit *     lineedit_kom_muh_hesap_id       = ( QLineEdit   * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number , POS_KOM_HESAP_ID_COLUMN );
    QSearchEdit *   searchedit_kom_muh_hesap_kodu   = ( QSearchEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number , KOMISYON_HESABI_COLUMN  );

    QString pos_hesap_kodu, pos_hesap_adi;

    int pos_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "pos_ent_hesap_id", &pos_hesap_kodu, &pos_hesap_adi );

    lineedit_muh_hesap_id->setText      ( QVariant ( pos_hesap_id ).toString());
    searchedit_muh_hesap_kodu->SET_TEXT ( pos_hesap_kodu );
    lineedit_muh_hesap_ismi->setText    ( pos_hesap_adi  );

    QString pos_komisyon_hesap_kodu, pos_komisyon_hesap_adi;

    int pos_komisyon_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "pos_kom_gideri_ent_hesap_id", &pos_komisyon_hesap_kodu, &pos_komisyon_hesap_adi );

    lineedit_kom_muh_hesap_id->setText      ( QVariant ( pos_komisyon_hesap_id ).toString() );
    searchedit_kom_muh_hesap_kodu->SET_TEXT ( pos_komisyon_hesap_kodu );
    lineedit_komisyon_hesap_ismi->setText   ( pos_komisyon_hesap_adi );

    QLineEdit * lineedit_adres      = ( QLineEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number , ADRES_COLUMN      );
    QLineEdit * lineedit_telefon    = ( QLineEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number , TELEFON_COLUMN    );
    QLineEdit * lineedit_acikalama  = ( QLineEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number , ACIKLAMA_COLUMN   );

    lineedit_adres->setMaxLength( 50 );
    lineedit_telefon->setMaxLength( 15 );
    lineedit_acikalama->setMaxLength( 128 );


}


/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{    
    if ( object EQ m_ui->searchEdit_banka_muhasebe_hesap_kodu ) {

        if ( m_ui->searchEdit_banka_muhasebe_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineedit_banka_muh_hesap_ismi->clear();
            m_bnk_muh_hesap_id = 0;
            return ADAK_OK;
        }

        MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

        MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

        if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , m_ui->searchEdit_banka_muhasebe_hesap_kodu->GET_TEXT()) EQ 0 ) {
            MSG_WARNING( tr("Aradığınız banka muhasebe hesap kodu bulunamadı"), m_ui->searchEdit_banka_muhasebe_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {

            MSG_WARNING( tr("Aradığınız muhasebe hesabı tali hesap değil"), m_ui->searchEdit_banka_muhasebe_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        m_bnk_muh_hesap_id = MUHASEBE_HESABI->hesap_id;
        m_ui->lineedit_banka_muh_hesap_ismi->setText ( MUHASEBE_HESABI->hesap_ismi );

    }
    else if ( object EQ m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu ) {

        if ( m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {

            m_ui->lineEdit_verilen_cekler_muh_hesap_ismi->clear();
            m_bnk_verilen_cekler_hesap_id = 0;
            return ADAK_OK;
        }
        MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

        MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

        if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu->GET_TEXT()) EQ 0 ) {
            MSG_WARNING( tr("Aradığınız verilen çekler muhasebe hesap kodu bulunamadı"), m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {

            MSG_WARNING( tr("Aradığınız muhasebe hesabı tali hesap değil"), m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        m_bnk_verilen_cekler_hesap_id = MUHASEBE_HESABI->hesap_id;
        m_ui->lineEdit_verilen_cekler_muh_hesap_ismi->setText ( MUHASEBE_HESABI->hesap_ismi );

    }
    else if ( object EQ m_ui->searchEdit_odeme_emirleri_muh_kodu ) {

        if ( m_ui->searchEdit_odeme_emirleri_muh_kodu->GET_TEXT().isEmpty() EQ true ) {

            m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi->clear();
            m_bnk_odeme_emirleri_hesap_id = 0;
            return ADAK_OK;
        }
        MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

        MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

        if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , m_ui->searchEdit_odeme_emirleri_muh_kodu->GET_TEXT()) EQ 0 ) {
            MSG_WARNING( tr("Aradığınız ödeme emirleri muhasebe hesap kodu bulunamadı"), m_ui->searchEdit_odeme_emirleri_muh_kodu );

            return ADAK_FAIL_UNDO;
        }

        if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {

            MSG_WARNING( tr("Aradığınız muhasebe hesabı tali hesap değil"), m_ui->searchEdit_odeme_emirleri_muh_kodu );

            return ADAK_FAIL_UNDO;
        }

        m_bnk_odeme_emirleri_hesap_id = MUHASEBE_HESABI->hesap_id;
        m_ui->lineEdit_odeme_emirleri_muh_hesap_ismi->setText ( MUHASEBE_HESABI->hesap_ismi );

    }
    else if ( object EQ m_ui->combobox_banka_ismi ) {
        if ( m_ui->combobox_banka_ismi->currentIndex() NE -1 ) {
             m_ui->combobox_sube_ismi->clear();
             m_ui->lineedit_sube_kodu->clear();
             m_ui->lineedit_hesap_numarasi->clear();

             GET_BANKA_SUBE_ADLARI(m_ui->combobox_sube_ismi,m_ui->combobox_banka_ismi->currentText()  );

             m_ui->combobox_sube_ismi->setCurrentIndex(-1);
        }
    }
    else if (object EQ m_ui->combobox_sube_ismi) {
        m_ui->lineedit_sube_kodu->clear();

        QString sube_kodu = GET_BANKA_SUBE_KODU(m_ui->combobox_banka_ismi->currentText(),m_ui->combobox_sube_ismi->currentText());

        m_ui->lineedit_sube_kodu->setText(sube_kodu);

    }
    else if ( object EQ m_ui->combobox_doviz_cinsi ) {

        if ( m_ui->combobox_doviz_cinsi->count() EQ 1 ) {
            MSG_WARNING( tr("Diğer Döviz Cinsleri İçin ,Firmanın Kullanacağı Dövizlerin Döviz Modülünden Seçilmesi Gerekmektedir!.. "), m_ui->combobox_doviz_cinsi );

            return ADAK_OK;
        }
        m_ui->lineedit_doviz_cinsi->clear();
        m_ui->lineedit_doviz_cinsi->setText(DVZ_GET_DOVIZ_ADI(DVZ_GET_DOVIZ_ID( m_ui->combobox_doviz_cinsi->currentText()) ));
    }

    return ADAK_OK;    
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineEdit_banka_hesap_unvani->text().isEmpty() EQ true ) {
        MSG_INFO( tr( "Banka Hesap Ünvan girmek zorundasınız!.." ), m_ui->lineEdit_banka_hesap_unvani );
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_banka_muhasebe_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Banka Muhasebe hesabını girmek zorundasınız!.." ), m_ui->searchEdit_banka_muhasebe_hesap_kodu );
        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Verilen Çekler Muhasebe hesabını girmek zorundasın!.." ), m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu );
        return ADAK_FAIL;
    }
    if (m_ui->lineedit_banka_hesap_kodu->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Banka Kodunu boş bırakamazsınız!.."), m_ui->lineedit_banka_hesap_kodu );

        return ADAK_FAIL;
    }
    if (m_ui->combobox_banka_ismi->currentText().isEmpty() EQ true) {
        MSG_WARNING( tr("Banka hesap ismini boş bırakamazsınız!.."), m_ui->combobox_banka_ismi );

        return ADAK_FAIL;
    }

    if (m_ui->combobox_sube_ismi->currentText().isEmpty() EQ true ) {
        MSG_WARNING( tr("Banka şube ismini boş bırakamazsınız!.."), m_ui->combobox_sube_ismi );

        return ADAK_FAIL;
    }

    if (m_ui->lineedit_hesap_numarasi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Hesap numarasını boş bırakamazsınız!.."), m_ui->lineedit_hesap_numarasi );

        return ADAK_FAIL;
    }
    if(m_ui->combobox_doviz_cinsi->currentIndex() EQ -1) {
        MSG_WARNING( tr("Döviz cinsini seçmelisniz!.."), m_ui->combobox_doviz_cinsi );

        return ADAK_FAIL;
    }
    if ( m_bnk_muh_hesap_id < 1 ) {
        MSG_WARNING( tr ( "Banka hesabının entegre olacağı muhasebe hesabını seçmelisiniz."), m_ui->searchEdit_banka_muhasebe_hesap_kodu  );
        return ADAK_FAIL;
    }

    if ( m_bnk_verilen_cekler_hesap_id < 1 ) {
        MSG_WARNING( tr ( "Verilen çekler muhasebe hesabını seçmelisiniz."), m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu );
        return ADAK_FAIL;
    }

    if ( m_bnk_odeme_emirleri_hesap_id < 1 ) {
        MSG_WARNING( tr ( "Ödeme emirleri muhasebe hesabını seçmelisiniz."), m_ui->searchEdit_odeme_emirleri_muh_kodu );
        return ADAK_FAIL;
    }


        return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_FIS_RECORD_ADD ()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id","banka_hesap_kodu = :banka_hesap_kodu");
    select_query.SET_VALUE(":banka_hesap_kodu" , m_ui->lineedit_banka_hesap_kodu->text());
    if (select_query.SELECT() NE 0 ) {
        MSG_WARNING( tr("Girmiş olduğunuz banka kodu,daha önce tanımlanmış.Lütfen değiştiriniz!.."), m_ui->lineedit_banka_hesap_kodu);

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::ADD_FIS_RECORD
***************************************************************************************/

int BNK_HESAPLARI_FISI::ADD_FIS_RECORD ()
{
    SQL_QUERY insert_query(DB);

    insert_query.PREPARE_INSERT("bnk_hesaplar","hesap_no_id","banka_hesap_kodu, banka_hesap_unvani , banka_ismi, sube_kodu, "
                                "sube_ismi, hesap_numarasi, doviz_id, muh_hesap_id, iban_numarasi, "
                                "verilen_cekler_hesap_id, odeme_emirleri_hesap_id ");

    insert_query.SET_VALUE ( ":banka_hesap_kodu"            , m_ui->lineedit_banka_hesap_kodu->text()       );
    insert_query.SET_VALUE ( ":banka_hesap_unvani"          , m_ui->lineEdit_banka_hesap_unvani->text()     );
    insert_query.SET_VALUE ( ":banka_ismi"                  , m_ui->combobox_banka_ismi->currentText()      );
    insert_query.SET_VALUE ( ":sube_kodu"                   , m_ui->lineedit_sube_kodu->text()              );
    insert_query.SET_VALUE ( ":sube_ismi"                   , m_ui->combobox_sube_ismi->currentText()       );
    insert_query.SET_VALUE ( ":hesap_numarasi"              , m_ui->lineedit_hesap_numarasi->text()         );
    insert_query.SET_VALUE ( ":doviz_id"                    , DVZ_GET_DOVIZ_ID ( m_ui->combobox_doviz_cinsi->currentText() ));
    insert_query.SET_VALUE ( ":muh_hesap_id"                , m_bnk_muh_hesap_id                            );
    insert_query.SET_VALUE ( ":iban_numarasi"               , m_ui->lineEdit_iban_no->text()                );
    insert_query.SET_VALUE ( ":verilen_cekler_hesap_id"     , m_bnk_verilen_cekler_hesap_id                 );
    insert_query.SET_VALUE ( ":odeme_emirleri_hesap_id"     , m_bnk_odeme_emirleri_hesap_id                 );

    int hesap_no_id = insert_query.INSERT();

    insert_query.PREPARE_INSERT("bnk_pos_arrayler","kayit_id" , "hesap_no_id");
    insert_query.SET_VALUE(":hesap_no_id" , hesap_no_id);
    insert_query.INSERT();

    insert_query.PREPARE_INSERT("bnk_kk_arrayler","kayit_id" , "hesap_no_id");
    insert_query.SET_VALUE(":hesap_no_id" , hesap_no_id);
    insert_query.INSERT();


    QString log_detaylari = (m_ui->combobox_banka_ismi->currentText() + " - " +
                             m_ui->lineedit_sube_kodu->text() + " " + m_ui->combobox_sube_ismi->currentText()
                             + "  "+ m_ui->lineedit_hesap_numarasi->text());

    E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_HESAPLAR , LOG_ISLEM_ADD , log_detaylari);

    return hesap_no_id;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id )
{
    Q_UNUSED(record_id);
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id","banka_hesap_kodu = :banka_hesap_kodu");
    select_query.SET_VALUE(":banka_hesap_kodu" , m_ui->lineedit_banka_hesap_kodu->text());
    if (select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        int hesap_no_id = select_query.VALUE(0).toInt();

        if (hesap_no_id NE record_id ) {
            MSG_WARNING( tr("Girmiş olduğunuz banka kodu,daha önce tanımlanmış.Lütfen değiştiriniz!.."), m_ui->lineedit_banka_hesap_kodu );

            return ADAK_FAIL;
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void BNK_HESAPLARI_FISI::UPDATE_FIS_RECORD ( int record_id )
{
    SQL_QUERY update_query(DB);

    update_query.PREPARE_UPDATE ( "bnk_hesaplar","hesap_no_id","banka_hesap_kodu, "
                                  "banka_hesap_unvani    , "
                                  "banka_ismi            , "
                                  "sube_kodu             , "
                                  "sube_ismi            , "
                                  "hesap_numarasi     , "
                                  "doviz_id             , "
                                  "muh_hesap_id        , "
                                  "iban_numarasi          , "
                                  "verilen_cekler_hesap_id , "
                                  "odeme_emirleri_hesap_id ","hesap_no_id        = :hesap_no_id");

    update_query.SET_VALUE ( ":banka_hesap_kodu"            , m_ui->lineedit_banka_hesap_kodu->text()                   );
    update_query.SET_VALUE ( ":banka_hesap_unvani"          , m_ui->lineEdit_banka_hesap_unvani->text()                 );
    update_query.SET_VALUE ( ":banka_ismi"                  , m_ui->combobox_banka_ismi->currentText()                  );
    update_query.SET_VALUE ( ":sube_kodu"                   , m_ui->lineedit_sube_kodu->text()                          );
    update_query.SET_VALUE ( ":sube_ismi"                   , m_ui->combobox_sube_ismi->currentText()                   );
    update_query.SET_VALUE ( ":hesap_numarasi"              , m_ui->lineedit_hesap_numarasi->text()                     );
    update_query.SET_VALUE ( ":doviz_id"                    , DVZ_GET_DOVIZ_ID ( m_ui->combobox_doviz_cinsi->currentText()) );
    update_query.SET_VALUE ( ":hesap_no_id"                 , record_id                                                 );
    update_query.SET_VALUE ( ":muh_hesap_id"                , m_bnk_muh_hesap_id                                        );
    update_query.SET_VALUE ( ":iban_numarasi"               , m_ui->lineEdit_iban_no->text()                            );
    update_query.SET_VALUE ( ":verilen_cekler_hesap_id"     , m_bnk_verilen_cekler_hesap_id                             );
    update_query.SET_VALUE ( ":odeme_emirleri_hesap_id"     , m_bnk_odeme_emirleri_hesap_id                             );

    update_query.UPDATE();

    QString log_detaylari = (m_ui->combobox_banka_ismi->currentText() + " - " +
                             m_ui->lineedit_sube_kodu->text() + " " + m_ui->combobox_sube_ismi->currentText()
                             + "  "+ m_ui->lineedit_hesap_numarasi->text());

    E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_HESAPLAR , LOG_ISLEM_UPDATE , log_detaylari);
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    Q_UNUSED(record_id);
    if (BANKA_HESABI_ISLEM_GORDU_MU(record_id) EQ true) {
        MSG_WARNING( tr("Banka Hesabı İşlem Görmüştür.Silinemez!.."), NULL );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void BNK_HESAPLARI_FISI::DELETE_FIS_RECORD ( int record_id )
{
    //Banka hesabi silindiginde ,hesaba ait olan poslar ve kredi kartlarida silinmelidir.

    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("bnk_hesaplar","banka_ismi,sube_kodu,sube_ismi,hesap_numarasi","hesap_no_id = :hesap_no_id");
    sql_query.SET_VALUE(":hesap_no_id" , record_id);

    if ( sql_query.SELECT() EQ 0) {
        return;
    }
    sql_query.NEXT();
    QString banka_ismi = sql_query.VALUE(0).toString();
    QString sube_kodu  = sql_query.VALUE(1).toString();
    QString sube_ismi  = sql_query.VALUE(2).toString();
    QString hesap_numarasi = sql_query.VALUE(3).toString();

    SQL_QUERY delete_query(DB);

    delete_query.PREPARE_DELETE("bnk_hesabin_poslari","hesap_no_id = :hesap_no_id");
    delete_query.SET_VALUE(":hesap_no_id" , record_id);
    delete_query.DELETE();

    delete_query.PREPARE_DELETE("bnk_hesabin_kredi_kartlari","hesap_no_id = :hesap_no_id");
    delete_query.SET_VALUE(":hesap_no_id" , record_id);
    delete_query.DELETE();

    delete_query.PREPARE_DELETE("bnk_pos_arrayler","hesap_no_id = :hesap_no_id");
    delete_query.SET_VALUE(":hesap_no_id" , record_id);
    delete_query.DELETE();

    delete_query.PREPARE_DELETE("bnk_kk_arrayler","hesap_no_id = :hesap_no_id");
    delete_query.SET_VALUE(":hesap_no_id" , record_id);
    delete_query.DELETE();


    delete_query.PREPARE_DELETE("bnk_hesaplar","hesap_no_id = :hesap_no_id");
    delete_query.SET_VALUE(":hesap_no_id",record_id);

    delete_query.DELETE();

    QString log_detaylari = (banka_ismi + " - " +
                                        sube_kodu + " " + sube_ismi
                                        + "  "+ hesap_numarasi);

    E9_LOG_KAYDI_EKLE ( BANKA_MODULU , LOG_BNK_HESAPLAR , LOG_ISLEM_DELETE , log_detaylari);
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int BNK_HESAPLARI_FISI::SELECT_FIS_RECORD()
{
    return OPEN_BNK_HESAPLARI_ARAMA ( this );
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::FIND_FIS_RECORD
***************************************************************************************/

int BNK_HESAPLARI_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id","banka_hesap_kodu = :banka_hesap_kodu");
    select_query.SET_VALUE(":banka_hesap_kodu" , m_ui->lineedit_banka_hesap_kodu->text());

    if (select_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr("Aradığınız banka kodu bulunamadı!.."), m_ui->lineedit_banka_hesap_kodu );

        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int BNK_HESAPLARI_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id");
    if(select_query.SELECT("banka_hesap_kodu ASC",0,1) NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }
    return 0;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int BNK_HESAPLARI_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id");
    if(select_query.SELECT("banka_hesap_kodu DESC",0,1) NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }
    return 0;
}

/**************************************************************************************
             BNK_HESAPLARI_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int BNK_HESAPLARI_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id","banka_hesap_kodu < :banka_hesap_kodu ");

    select_query.SET_VALUE(":banka_hesap_kodu" , m_ui->lineedit_banka_hesap_kodu->text());
    if ( select_query.SELECT("banka_hesap_kodu DESC",0,1) NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }
    return 0;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int BNK_HESAPLARI_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id","banka_hesap_kodu > :banka_hesap_kodu ");

    select_query.SET_VALUE(":banka_hesap_kodu" , m_ui->lineedit_banka_hesap_kodu->text());
    if ( select_query.SELECT("banka_hesap_kodu ASC",0,1) NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }
    return 0;
}


/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_LINE_VAR
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{

    QWidget * cell_widget_muhasebe_hesabi = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,MUHASEBE_HESABI_COLUMN);
    QSearchEdit * searchedit_muh_hesabi = static_cast <QSearchEdit*> (cell_widget_muhasebe_hesabi);

    QWidget * cell_widget_kom_hesabi = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,KOMISYON_HESABI_COLUMN);
    QSearchEdit * searchedit_kom_hesabi = static_cast <QSearchEdit*> (cell_widget_kom_hesabi);

    QWidget * cell_widget = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,MUHASEBE_HESAP_ISMI_COLUMN);
    QLineEdit * lineedit_muh_hesap_ismi = static_cast <QLineEdit* >(cell_widget);

    cell_widget = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,KOMISYON_HESAP_ISMI_COLUMN);
    QLineEdit * lineedit_kom_hesap_ismi = static_cast <QLineEdit*> (cell_widget);


    QLineEdit * lineedit_pos_muh_hesap_id = (QLineEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_MUH_HESAP_ID_COLUMN);

    QLineEdit * lineedit_pos_kom_hesap_id = (QLineEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_KOM_HESAP_ID_COLUMN);

    if ( object EQ searchedit_muh_hesabi ) {
        lineedit_muh_hesap_ismi->clear();
        int muh_hesap_id = MUH_GET_HESAP_ID( searchedit_muh_hesabi->GET_TEXT() );
        if (muh_hesap_id EQ 0 ) {
            MSG_WARNING( tr("Aradığınız muhasebe hesabı bulunamadı!.."), searchedit_muh_hesabi );

            return ADAK_FAIL;
        }
        lineedit_muh_hesap_ismi->setText(MUH_GET_HESAP_ISMI(muh_hesap_id));
        lineedit_pos_muh_hesap_id->setText(QVariant(muh_hesap_id).toString());
        return ADAK_OK;
    }
    if (object EQ searchedit_kom_hesabi ) {
        lineedit_kom_hesap_ismi->clear();
        int kom_hesap_id = MUH_GET_HESAP_ID(searchedit_kom_hesabi->GET_TEXT());

        if (kom_hesap_id EQ 0 ) {
            MSG_WARNING( tr("Girdiğiniz komisyon hesabı bulunamadı!.."), searchedit_kom_hesabi );

            return ADAK_FAIL;
        }
        lineedit_kom_hesap_ismi->setText(MUH_GET_HESAP_ISMI(kom_hesap_id));
        lineedit_pos_kom_hesap_id->setText(QVariant(kom_hesap_id).toString());
        return ADAK_OK;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_ADD_LINE
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{

    QLineEdit *     lineedit_pos_numarasi           = ( QLineEdit   * ) m_ui->tablewidget_pos_bilgileri->cellWidget( row_number, POS_NO_COLUMN );
    QSearchEdit *   searchedit_muh_hesap_kodu       = ( QSearchEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget( row_number , MUHASEBE_HESABI_COLUMN  );
    QSearchEdit *   searchedit_kom_muh_hesap_kodu   = ( QSearchEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget( row_number , KOMISYON_HESABI_COLUMN  );

    if ( searchedit_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Muhasebe hesap kodunu girmelisiniz!.." ), searchedit_muh_hesap_kodu );
        return ADAK_FAIL;
    }
    if ( searchedit_kom_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Komisyon Muhasebe hesap kodunu girmelisiniz!.." ), searchedit_kom_muh_hesap_kodu );
        return ADAK_FAIL;
    }

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hesabin_poslari","pos_id","pos_numarasi =:pos_numarasi AND hesap_no_id =:hesap_no_id" );
    query.SET_VALUE     ( ":pos_numarasi",  lineedit_pos_numarasi->text() );
    query.SET_VALUE     ( ":hesap_no_id",   record_id  );

    if ( query.SELECT() > 0 ) {

         MSG_INFO( tr( "Aynı POS NUMARASI tanımlanamaz... " ),  lineedit_pos_numarasi );
         return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::ADD_LINE
***************************************************************************************/

void BNK_HESAPLARI_FISI::ADD_LINE ( int record_id, int row_number )
{
    QWidget * cell_widget         = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_NO_COLUMN);
    QLineEdit * lineedit_pos_no   = static_cast<QLineEdit*>(cell_widget);

    cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,ADRES_COLUMN);
    QLineEdit * lineedit_adres    = static_cast <QLineEdit*> (cell_widget);

    cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,TELEFON_COLUMN);
    QLineEdit * lineedit_telefon  = static_cast<QLineEdit*> (cell_widget);

    cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,ACIKLAMA_COLUMN);
    QLineEdit * lineedit_aciklama = static_cast <QLineEdit*> (cell_widget);

    QLineEdit * lineedit_pos_muh_hesap_id = (QLineEdit *) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_MUH_HESAP_ID_COLUMN);

    QLineEdit * lineedit_pos_kom_hesap_id = (QLineEdit *) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_KOM_HESAP_ID_COLUMN);



    SQL_QUERY insert_query(DB);

    insert_query.PREPARE_INSERT("bnk_hesabin_poslari","pos_id" , "hesap_no_id,pos_numarasi,uye_isyeri_adresi,"
                                "uye_isyeri_telefonu,aciklama,muh_hesap_id,kom_hesap_id,order_number");
    insert_query.SET_VALUE(":hesap_no_id"        , record_id );
    insert_query.SET_VALUE(":pos_numarasi"       , lineedit_pos_no->text());
    insert_query.SET_VALUE(":uye_isyeri_adresi"  , lineedit_adres->text());
    insert_query.SET_VALUE(":uye_isyeri_telefonu", lineedit_telefon->text());
    insert_query.SET_VALUE(":aciklama"           , lineedit_aciklama->text());
    insert_query.SET_VALUE(":muh_hesap_id"       , lineedit_pos_muh_hesap_id->text().toInt());
    insert_query.SET_VALUE(":kom_hesap_id"       , lineedit_pos_kom_hesap_id->text().toInt());
    insert_query.SET_VALUE(":order_number"       , m_ui->tablewidget_pos_bilgileri->item(row_number,ORDER_COLUMN)->text().toInt());

    int row_id = insert_query.INSERT();

    QTableWidgetItem * new_item = new QTableWidgetItem(QVariant(row_id).toString());
    m_ui->tablewidget_pos_bilgileri->setItem(row_number,ROW_ID_COLUMN,new_item);

    new_item = new QTableWidgetItem(QVariant(record_id).toString());
    m_ui->tablewidget_pos_bilgileri->setItem(row_number,FIS_ID_COLUMN,new_item);

    QString log_detaylari = QObject::tr("Banka hesap numarası : ")+ m_ui->lineedit_hesap_numarasi->text()
                            + QObject::tr(" , Pos Numarası : ") + lineedit_pos_no->text();
    E9_LOG_KAYDI_EKLE (BANKA_MODULU , LOG_BNK_POS_ISLEMLERI,LOG_ISLEM_ADD,log_detaylari);

}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::UPDATE_LINE
***************************************************************************************/

void BNK_HESAPLARI_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);
    QWidget * cell_widget         = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_NO_COLUMN);
    QLineEdit * lineedit_pos_no   = static_cast<QLineEdit*>(cell_widget);

    cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,ADRES_COLUMN);
    QLineEdit * lineedit_adres    = static_cast <QLineEdit*> (cell_widget);

    cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,TELEFON_COLUMN);
    QLineEdit * lineedit_telefon  = static_cast<QLineEdit*> (cell_widget);

    cell_widget                   = m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,ACIKLAMA_COLUMN);
    QLineEdit * lineedit_aciklama = static_cast <QLineEdit*> (cell_widget);


    QLineEdit * lineedit_pos_muh_hesap_id = (QLineEdit *) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_MUH_HESAP_ID_COLUMN);

    QLineEdit * lineedit_pos_kom_hesap_id = (QLineEdit *) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_KOM_HESAP_ID_COLUMN);

    SQL_QUERY update_query(DB);

    update_query.PREPARE_UPDATE("bnk_hesabin_poslari","pos_id","pos_numarasi , uye_isyeri_adresi ,"
                                "uye_isyeri_telefonu ,aciklama ,muh_hesap_id ,"
                                "kom_hesap_id ","pos_id = :pos_id");


    update_query.SET_VALUE(":pos_numarasi"        , lineedit_pos_no->text());
    update_query.SET_VALUE(":uye_isyeri_adresi"   , lineedit_adres->text());
    update_query.SET_VALUE(":uye_isyeri_telefonu" , lineedit_telefon->text());
    update_query.SET_VALUE(":aciklama"            , lineedit_aciklama->text());
    update_query.SET_VALUE(":muh_hesap_id"        , lineedit_pos_muh_hesap_id->text().toInt());
    update_query.SET_VALUE(":kom_hesap_id"        , lineedit_pos_kom_hesap_id->text().toInt());
    update_query.SET_VALUE(":pos_id"              , m_ui->tablewidget_pos_bilgileri->item(row_number,ROW_ID_COLUMN)->text().toInt());
    update_query.UPDATE();

    QString log_detaylari = QObject::tr("Banka hesap numarası : ")+ m_ui->lineedit_hesap_numarasi->text()
                            + QObject::tr(" , Pos Numarası : ") + lineedit_pos_no->text();
    E9_LOG_KAYDI_EKLE (BANKA_MODULU , LOG_BNK_POS_ISLEMLERI,LOG_ISLEM_UPDATE,log_detaylari);

}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int BNK_HESAPLARI_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{

    Q_UNUSED ( record_id );

    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("bnk_defter_fis_satirlari","fis_satiri_id ",
                             "hesap_id  = :hesap_id "
                             "AND hesap_turu = :hesap_turu");
    sql_query.SET_VALUE(":hesap_id"   , m_ui->tablewidget_pos_bilgileri->item(row_number ,ROW_ID_COLUMN)->text().toInt());
    sql_query.SET_VALUE(":hesap_turu" , ENUM_BNK_POS_HESABI);

    if(sql_query.SELECT() NE 0 ) {
        MSG_WARNING( tr("Pos işlem görmüştür.Silemezsiniz."), NULL );
        return false;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::DELETE_LINE
***************************************************************************************/

void BNK_HESAPLARI_FISI::DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );

    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("bnk_hesabin_poslari","pos_numarasi","pos_id = :pos_id");
    sql_query.SET_VALUE(":pos_id" , m_ui->tablewidget_pos_bilgileri->item(row_number,ROW_ID_COLUMN)->text().toInt());

    if(sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString pos_numarasi = sql_query.VALUE(0).toString();

    SQL_QUERY delete_query(DB);

    delete_query.PREPARE_DELETE("bnk_hesabin_poslari","pos_id = :pos_id");
    delete_query.SET_VALUE(":pos_id" , m_ui->tablewidget_pos_bilgileri->item(row_number,ROW_ID_COLUMN)->text().toInt());
    delete_query.DELETE();

    QString log_detaylari = QObject::tr("Banka hesap numarası : ")+ m_ui->lineedit_hesap_numarasi->text()
                            + QObject::tr(" , Pos Numarası : ") + pos_numarasi;

    E9_LOG_KAYDI_EKLE (BANKA_MODULU , LOG_BNK_POS_ISLEMLERI,LOG_ISLEM_DELETE,log_detaylari);

}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int BNK_HESAPLARI_FISI::LOCK_FIS_RECORD ( int record_id )
{
    return DB->LOCK_ROW( "hesap_no_id","bnk_hesaplar", QString ( "hesap_no_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   BNK_HESAPLARI_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void BNK_HESAPLARI_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
     DB->UNLOCK_ROW( "hesap_no_id","bnk_hesaplar", QString ( "hesap_no_id = %1" ).arg ( record_id ));
}
/****************************************************************************************
            BNK_HESAPLARI_FISI::LINE_SEARCH_EDIT_CLICKED();
*****************************************************************************************/

void BNK_HESAPLARI_FISI::LINE_SEARCH_EDIT_CLICKED(int row_number, QWidget * widget, QLineEdit * line_edit)
{
    Q_UNUSED ( line_edit );

    QSearchEdit * searchedit_muh_hesabi = ( QSearchEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,MUHASEBE_HESABI_COLUMN);
    QSearchEdit * searchedit_kom_hesabi = ( QSearchEdit * ) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,KOMISYON_HESABI_COLUMN);

    QLineEdit * lineedit_pos_muh_hesap_id = (QLineEdit *) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_MUH_HESAP_ID_COLUMN);
    QLineEdit * lineedit_pos_kom_hesap_id = (QLineEdit *) m_ui->tablewidget_pos_bilgileri->cellWidget(row_number,POS_KOM_HESAP_ID_COLUMN);

    if ( widget EQ searchedit_muh_hesabi ) {
        int muh_hesap_id = OPEN_MUH_HESAP_ARAMA(searchedit_muh_hesabi->GET_TEXT(),this,1);
        if (muh_hesap_id NE -1) {
            lineedit_pos_muh_hesap_id->setText(QVariant(muh_hesap_id).toString());
            searchedit_muh_hesabi->SET_TEXT(MUH_GET_HESAP_KODU(muh_hesap_id));
            return;
        }
    }
    if ( widget EQ searchedit_kom_hesabi ) {
        int kom_hesap_id = OPEN_MUH_HESAP_ARAMA(searchedit_kom_hesabi->GET_TEXT(),this,1);

        if (kom_hesap_id NE -1) {
            lineedit_pos_kom_hesap_id->setText(QVariant(kom_hesap_id).toString());
            searchedit_kom_hesabi->SET_TEXT(MUH_GET_HESAP_KODU(kom_hesap_id));
            return;
        }
    }
}

/****************************************************************************************
                      BNK_HESAPLARI_FISI::SEARCH_EDIT_CLICKED
*****************************************************************************************/

void BNK_HESAPLARI_FISI::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{

    if ( widget EQ m_ui->searchEdit_banka_muhasebe_hesap_kodu ) {

        int muh_hesap_id = OPEN_MUH_HESAP_ARAMA("",this,true);

        if ( muh_hesap_id > 0 ) {
            line_edit->setText(  MUH_GET_HESAP_KODU ( muh_hesap_id ) );
        }
    }
    else if ( widget EQ m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu
              OR widget EQ m_ui->searchEdit_odeme_emirleri_muh_kodu) {


        int verilen_cekler_muh_hesap_id = OPEN_MUH_HESAP_ARAMA("" , this, true);

        if ( verilen_cekler_muh_hesap_id > 0 ) {
            if ( widget EQ m_ui->searchEdit_verilen_cekler_muhasebe_hesap_kodu ) {
                line_edit->setText ( MUH_GET_HESAP_KODU ( verilen_cekler_muh_hesap_id ) );
            }
            else {
                line_edit->setText(MUH_GET_HESAP_KODU(verilen_cekler_muh_hesap_id));
            }
        }
    }
}

/****************************************************************************************
                    BNK_HESAPLARI_FISI::SAVER_BUTTON_CLICKED();
*****************************************************************************************/

void BNK_HESAPLARI_FISI::SAVER_BUTTON_CLICKED(QAbstractButton *button, int record_id)
{
    Q_UNUSED(button);
    OPEN_BNK_HESABIN_KREDI_KARTLARI_FISI(record_id,this);
}
/*************************************************************************************
          BNK_HESAPLARI_FISI::BANKA_HESABI_ISLEM_GORDU_MU
**************************************************************************************/

bool BNK_HESAPLARI_FISI::BANKA_HESABI_ISLEM_GORDU_MU(int hesap_no_id)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_defter_fisler","fis_id","hesap_no_id = :hesap_no_id");
    select_query.SET_VALUE(":hesap_no_id",hesap_no_id);
    if (select_query.SELECT() NE 0 ) {
        return true;
    }


    //banka hesabina ait olan poslar islem gordumu

    select_query.PREPARE_SELECT("bnk_hesabin_poslari","pos_id","hesap_no_id = :hesap_no_id");
    select_query.SET_VALUE(":hesap_no_id" ,hesap_no_id );

    if(select_query.SELECT() NE 0 ) {
        while (select_query.NEXT()) {
            int pos_id = select_query.VALUE(0).toInt();

            SQL_QUERY sql_query(DB);

            sql_query.PREPARE_SELECT("bnk_defter_fis_satirlari","fis_satiri_id","hesap_id  = :hesap_id "
                                     "AND hesap_turu = :hesap_turu");
            sql_query.SET_VALUE(":hesap_id"   , pos_id);
            sql_query.SET_VALUE(":hesap_turu" , ENUM_BNK_POS_HESABI);

            if(sql_query.SELECT() NE 0 ) {
                return true;
            }
        }
    }

    //banka hesabina ait olan kredi kartlari islem gordumu

    select_query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari","kredi_karti_id","hesap_no_id = :hesap_no_id");
    select_query.SET_VALUE(":hesap_no_id" , hesap_no_id);

    if(select_query.SELECT() NE 0 ) {
        while (select_query.NEXT()) {
            int kredi_karti_id  = select_query.VALUE(0).toInt();

            SQL_QUERY sql_query(DB);

            sql_query.PREPARE_SELECT("bnk_defter_fis_satirlari","fis_satiri_id","hesap_id  = :hesap_id "
                                     "AND hesap_turu = :hesap_turu");
            sql_query.SET_VALUE(":hesap_id"   , kredi_karti_id);
            sql_query.SET_VALUE(":hesap_turu" , ENUM_BNK_KREDI_KARTI_HESABI);

            if(sql_query.SELECT() NE 0 ) {
                return true;
            }
        }
    }
    return false;
}
