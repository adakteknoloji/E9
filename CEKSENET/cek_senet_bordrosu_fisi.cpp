#include <QHeaderView>
#include <QMessageBox>
#include "adak_utils.h"
#include "dvz_enum.h"
#include "ui_cek_senet_bordrosu_fisi.h"
#include "cek_senet_bordrosu_fisi_class.h"
#include "print.h"
#include "adak_std_utils.h"
#include "dvz_console_utils.h"
#include "e9_struct.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "bnk_gui_utils.h"
#include "cari_console_utils.h"
#include "cek_enum.h"
#include "sube_struct.h"
#include "e9_yetki.h"
#include "e9_log.h"
#include "cek_fis_utils.h"
#include "muh_fis_utils.h"
#include "cari_fis_utils.h"
#include "bnk_fis_utils.h"
#include "cari_kart_arama_open.h"
#include "bnk_hesaplari_arama_open.h"
#include "cek_fisi_yazdir_raporu_open.h"
#include "cek_senet_arama_open.h"
#include "cek_senet_bordrosu_arama_open.h"
#include "cek_senet_formu_open.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "e9_ent_detaylari_batch_open.h"
#include "banka_struct.h"
#include "bnk_console_utils.h"
#include "cek_console_utils.h"
#include "cek_gui_utils.h"
#include "dvz_gui_utils.h"
#include "sube_console_utils.h"

extern ADAK_SQL               *            DB;

#define ROW_ID_COLUMN                   0
#define BORDRO_ID_COLUMN                1
#define ORDER_COLUMN                    2
#define OLD_CEK_SNT_ID_COLUMN           3
#define CURRENT_CEK_SNT_ID_COLUMN       4
#define PORTFOY_NO_COLUMN               5
#define CEK_NO_COLUMN                   6
#define VADE_TARIHI_COLUMN              7
#define VALOR_COLUMN                    8
#define BANKA_COLUMN                    9
#define SUBE_COLUMN                     10
#define ACIKLAMA_COLUMN                 11
#define TUTAR_COLUMN                    12
#define PARA_BIRIMI_COLUMN              13


/**************************************************************************************
                   OPEN_CEK_SENET_BORDROSU_FISI
***************************************************************************************/

void OPEN_CEK_SENET_BORDROSU_FISI ( int bordro_id, int modul_id, int bordro_islem_turu,QWidget * parent, int bordro_turu )
{
    CEK_SENET_BORDROSU_FISI * F = new CEK_SENET_BORDROSU_FISI ( bordro_id, modul_id ,bordro_islem_turu, parent, bordro_turu );
    F->EXEC( FULL_SCREEN );
}
/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CEK_SENET_BORDROSU_FISI
***************************************************************************************/

CEK_SENET_BORDROSU_FISI::CEK_SENET_BORDROSU_FISI ( int bordro_id, int modul_id,int bordro_islem_turu, QWidget * parent, int bordro_turu )
    :FIS_KERNEL ( parent ), m_ui ( new Ui::CEK_SENET_BORDROSU_FISI )
{

    m_bordro_turu               = bordro_turu;
    m_modul_id                  = modul_id;
    m_bordro_id                 = bordro_id;
    m_bordro_islem_turu         = bordro_islem_turu;

    m_ui->setupUi    (this );
    START_FIS_KERNEL (this, DB );
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::SETUP_FORM();
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::SETUP_FORM()
{    
    if ( m_modul_id EQ CEKSENET_MODULU ) {
       REGISTER_BUTTONS_WIDGET   ( m_ui->navigation_button_widget  );
       SET_HELP_PAGE  ( "cek-senet-islemleri" );
    }
    else {
       REGISTER_BUTTONS_WIDGET   ( m_ui->navigation_button_widget  ,false,true);
       m_ui->pushButton_detaylar->setHidden(true);
       SET_HELP_PAGE  ( "cek-senet-islemleri" );
    }

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );

    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }


    m_ui->adakDate_vade->MALI_YIL_KISITI_YOK( true );

    M_BORDRO                = new CEK_BORDRO_STRUCT;
    M_BORDRO_SATIRI         = new CEK_BORDRO_SATIRI_STRUCT;
    M_ENTEGRASYON_DETAYLARI = new E9_ENT_DETAYLARI_STRUCT;
       
    m_ui->lineedit_bordro_no->setText("*");
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_bordro_no);
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_sirket   );

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_bordro_no->setValidator ( validator );


    m_log_kaydi_eklenmeli_mi    = false;
    m_kayit_eklendi             = false;
    m_kayit_silindi             = false;

    if ( m_bordro_turu EQ ENUM_MUSTERI_CEK_BORDROSU OR
         m_bordro_turu EQ ENUM_MUSTERI_SENET_BORDROSU ) {
        m_ui->buton_cek_kaydi->setHidden(true);
        m_ui->buton_senet_kaydi->setHidden(true);
    }

    m_temel_para_birimi_id      = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH ( 512 );

    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI   ( m_ui->combobox_banka_hesap_kodu );
    CEK_FILL_COMBO_BOX_BORDRO_ISLEM_TURU ( m_bordro_turu, m_ui->comboBox_bordro_islem_turu, m_modul_id );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_bordro_islem_turu, CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING( m_bordro_islem_turu ));

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_bordro_no );

    if ( m_modul_id NE CEKSENET_MODULU ) {
        m_bordro_islem_turu = CEK_GET_CEK_SENET_ENT_BORDROSU_ISLEM_TURU_ENUM ( m_ui->comboBox_bordro_islem_turu->currentText() );
    }
    else {
        m_bordro_islem_turu = CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_ENUM  ( m_ui->comboBox_bordro_islem_turu->currentText() );
    }

    m_ui->commaEdit_parite->SET_PRECISION(6);
    m_ui->commaEdit_doviz_kuru->SET_PRECISION(4);

    REGISTER_TABLE_WIDGET             ( m_ui->tablewidget_bordro_satirlari, 14 );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION       ( BORDRO_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN       ( 5 );

    SET_FIS_ORDER_DATABASE ( "cek_bordro_satirlari", "order_number", "bordro_satiri_id" );

    SET_TABLE_ROW_WIDGETS ( OLD_CEK_SNT_ID_COLUMN,     WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( CURRENT_CEK_SNT_ID_COLUMN, WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( PORTFOY_NO_COLUMN,         WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( CEK_NO_COLUMN,             WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( VADE_TARIHI_COLUMN,        WIDGET_ADAK_DATE   );
    SET_TABLE_ROW_WIDGETS ( VALOR_COLUMN,              WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( BANKA_COLUMN,              WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( SUBE_COLUMN,               WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN,           WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( TUTAR_COLUMN,              WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( PARA_BIRIMI_COLUMN,        WIDGET_LINE_EDIT   );

    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_bordro_islem_turu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_bordro_para_birimi);
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_fis_filtresi);

    m_ui->label_temel_para_birimi->setText(DVZ_GET_TEMEL_PARA_BIRIMI_KODU());

    REGISTER_SAVER_BUTTON   ( m_ui->button_yazdir );
    REGISTER_CHANGER_BUTTON ( m_ui->pushButton_detaylar);

    DVZ_FILL_COMBOBOX(m_ui->comboBox_bordro_para_birimi);

    m_ui->pushButton_detaylar->setVisible( false );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ) {
        m_ui->comboBox_fis_filtresi->setVisible( false );
        m_ui->label_fis_filtresi->setVisible( false );

        m_ui->comboBox_sirket->setVisible( false );
        m_ui->label_sirket->setVisible( false );
    }

    BORDRO_EKRANINI_DUZENLE();

    QTableWidgetItem * header_item  = m_ui->tablewidget_bordro_satirlari->horizontalHeaderItem ( PARA_BIRIMI_COLUMN);
    QFont              font         =  header_item->font();

    font.setPointSize       ( font.pointSize() - 2 );
    header_item->setFont    ( font );

    m_ui->tablewidget_bordro_satirlari->setSelectionMode    ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_bordro_satirlari->setFocus            ( Qt::OtherFocusReason );

    m_ui->lineEdit_bnk_sube_bilgisi->setMaxLength      (128);
    m_ui->lineedit_bordro_no->setMaxLength             (10);
    m_ui->lineedit_cari_hesap_ismi->setMaxLength       (60);
    m_ui->lineEdit_cari_para_birimi_kodu->setMaxLength (5);
    m_ui->lineEdit_doviz_cinsi->setMaxLength           (5);
    m_ui->lineedit_ort_gun->setMaxLength               (5);

    FOCUS_FIRST_WIDGET      ();

    AUTOSTART_FIS_KERNEL    ( m_bordro_id );
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::CLEAR_FORM_MEMBERS()
{
    m_doviz_cinsine_gore_toplam_tutar_list.clear();

    m_cari_hesap_id             = 0;
    m_hesap_no_id               = 0;

    m_cari_para_birimi_id       = 0;
    m_bordro_para_birimi_id     = 0;
    m_bnk_hesabi_para_birimi_id = 0;

    m_banka_ismi.clear();
    m_sube_ismi.clear();
    m_sube_kodu.clear();

    M_BORDRO->CLEAR_BORDRO_STRUCT();

    M_ENTEGRASYON_DETAYLARI->banka_fis_no.clear();
    M_ENTEGRASYON_DETAYLARI->cari_fis_no        = 0;
    M_ENTEGRASYON_DETAYLARI->cek_fis_no         = 0;
    M_ENTEGRASYON_DETAYLARI->firma_cek_fis_no   = 0;
    M_ENTEGRASYON_DETAYLARI->firma_senet_fis_no = 0;
    M_ENTEGRASYON_DETAYLARI->muh_fis_no         = 0;
    M_ENTEGRASYON_DETAYLARI->senet_fis_no       = 0;

}
/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::NEW_FIS_RECORD
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::NEW_FIS_RECORD()
{
    m_ui->commaEdit_doviz_kuru->setVisible( false );
    m_ui->label_temel_para_birimi->setVisible( false );
    m_ui->label_parite->setVisible( false );
    m_ui->label_bordro_para_birimi->setVisible( false );
    m_ui->commaEdit_parite->setVisible( false );
    m_ui->label_bnk_cari_para_birimi->setVisible( false );
    m_ui->comboBox_bordro_para_birimi->setCurrentIndex(-1);
    m_ui->lineedit_bordro_no->setText("*");

    BORDRO_EKRANINI_DUZENLE();

}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::GET_FIS_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::GET_FIS_RECORD ( int bordro_id )
{


    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "cek_bordrolar",
                               "bordro_no, bordro_tarihi,aciklama, toplam_tutar, "
                               "hesap_no_id, cari_hesap_id, bordro_para_birimi_id , doviz_kuru ,"
                               "parite , program_id , modul_id , bordro_turu,"
                               "bordro_islem_turu,musteri_cek_senedi_mi,cek_bordrosu_mu ",
                               "bordro_id = :bordro_id" );

    sql_query.SET_VALUE ( ":bordro_id" , bordro_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->lineedit_bordro_no->setText ( sql_query.VALUE(0).toString() );



    m_ui->adakDate_bordro_tarihi->SET_DATE( sql_query.VALUE(1).toString()  );

    m_ui->limitedTextEdit_aciklama->setText ( sql_query.VALUE(2).toString() );

    m_ui->commaedit_toplam_cek_tutari->SET_DOUBLE ( sql_query.VALUE(3).toDouble() );

    m_hesap_no_id   = sql_query.VALUE(4).toInt();

    m_cari_hesap_id = sql_query.VALUE(5).toInt();

    m_bordro_para_birimi_id = sql_query.VALUE(6).toInt();

    m_ui->comboBox_bordro_para_birimi->setCurrentIndex(m_ui->comboBox_bordro_para_birimi->findText(DVZ_GET_DOVIZ_KODU(sql_query.VALUE(6).toInt())));

    m_ui->commaEdit_doviz_kuru->SET_DOUBLE(sql_query.VALUE(7).toDouble());

    m_ui->commaEdit_parite->SET_DOUBLE(sql_query.VALUE(8).toDouble());

    if ( m_modul_id EQ CEKSENET_MODULU ) {
        m_ui->comboBox_bordro_islem_turu->setCurrentIndex(m_ui->comboBox_bordro_islem_turu->findText(CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(sql_query.VALUE("bordro_islem_turu").toInt())));
        m_bordro_islem_turu = sql_query.VALUE("bordro_islem_turu").toInt();
    }
    else {
        QString secilecek_text = CEK_GET_CEK_SENET_ENT_BORDROSU_ISLEM_TURU_STRING(sql_query.VALUE("bordro_islem_turu").toInt(),
                                                                              sql_query.VALUE("musteri_cek_senedi_mi").toInt(),
                                                                              sql_query.VALUE("cek_bordrosu_mu").toInt());

        m_ui->comboBox_bordro_islem_turu->setCurrentIndex(m_ui->comboBox_bordro_islem_turu->findText(secilecek_text));
        m_bordro_islem_turu = sql_query.VALUE("bordro_islem_turu").toInt();
    }



    M_BORDRO->program_id            = sql_query.VALUE(9).toInt();
    M_BORDRO->modul_id              = sql_query.VALUE(10).toInt();
    M_BORDRO->aciklama              = sql_query.VALUE(2).toString();
    M_BORDRO->bordro_islem_turu     = m_bordro_islem_turu;
    M_BORDRO->bordro_no             = sql_query.VALUE(0).toInt();
    M_BORDRO->bordro_tarihi         = sql_query.VALUE(1).toString();
    M_BORDRO->bordro_turu           = sql_query.VALUE(11).toInt();
    M_BORDRO->cari_hesap_id         = m_cari_hesap_id;
    M_BORDRO->hesap_no_id           = m_hesap_no_id;
    M_BORDRO->toplam_tutar          = sql_query.VALUE(3).toDouble();
    M_BORDRO->bordro_para_birimi_id = m_bordro_para_birimi_id;
    M_BORDRO->base_fis_id           = 0;

    BNK_BANKA_BILGILERI_STRUCT BNK_BILGILER;

    BNK_GET_BANKA_TEMEL_BILGILER( &BNK_BILGILER, m_hesap_no_id );

    m_ui->combobox_banka_hesap_kodu->setCurrentIndex(m_ui->combobox_banka_hesap_kodu->findText( BNK_BILGILER.banka_hesap_kodu ));
    m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU( BNK_BILGILER.doviz_id ));
    m_bnk_hesabi_para_birimi_id = BNK_BILGILER.doviz_id;
    m_banka_ismi = BNK_BILGILER.banka_ismi;
    m_sube_kodu  = BNK_BILGILER.sube_kodu;
    m_sube_ismi  = BNK_BILGILER.sube_ismi;
    QString hesap_numarasi =BNK_BILGILER.hesap_numarasi;

    m_ui->lineEdit_bnk_sube_bilgisi->setText(m_banka_ismi + " / " + m_sube_kodu + " " + m_sube_ismi + " "
                                             + hesap_numarasi );


    QString cari_hesap_kodu, cari_hesap_ismi;
    int     para_birim_id;

    int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi, &para_birim_id );

    if ( result NE 0 ) {
        m_ui->searchEdit_cari_hesap_kodu->SET_TEXT( cari_hesap_kodu );
        m_ui->lineedit_cari_hesap_ismi->setText( cari_hesap_ismi );
        m_ui->lineEdit_cari_para_birimi_kodu->setText(DVZ_GET_DOVIZ_KODU( para_birim_id ));
        m_cari_para_birimi_id = para_birim_id;
        M_BORDRO->cari_para_birimi_id = m_cari_para_birimi_id;
    }

    m_ui->label_bordro_para_birimi->setText("1 " + DVZ_GET_DOVIZ_KODU(m_bordro_para_birimi_id));
    if ( m_cari_hesap_id > 0 ) {
       m_ui->label_bnk_cari_para_birimi->setText(DVZ_GET_DOVIZ_KODU(m_cari_para_birimi_id));
    }
    if (m_hesap_no_id > 0 ) {
        m_ui->label_bnk_cari_para_birimi->setText(DVZ_GET_DOVIZ_KODU(m_bnk_hesabi_para_birimi_id));
    }


    if ( ( m_bordro_para_birimi_id EQ m_cari_para_birimi_id) OR ( m_bordro_para_birimi_id EQ m_bnk_hesabi_para_birimi_id) ) {

        if ( m_bordro_para_birimi_id EQ m_temel_para_birimi_id ) {
            m_ui->commaEdit_doviz_kuru->clear();
            m_ui->commaEdit_parite->clear();
        }
        else {
            m_ui->label_temel_para_birimi->setHidden(false);
            m_ui->commaEdit_doviz_kuru->setHidden(false);
            m_ui->label_temel_para_birimi->setText(DVZ_GET_TEMEL_PARA_BIRIMI_KODU());
            m_ui->commaEdit_parite->clear();
        }
    }
    else {
        m_ui->commaEdit_doviz_kuru->setHidden(false);
        m_ui->label_temel_para_birimi->setHidden(false);

        if ( m_bordro_para_birimi_id EQ m_temel_para_birimi_id ) {
            m_ui->label_parite->setHidden(false);
            m_ui->commaEdit_parite->setHidden(false);
            m_ui->label_bordro_para_birimi->setHidden(false);
            m_ui->label_bnk_cari_para_birimi->setHidden(false);
            m_ui->commaEdit_doviz_kuru->clear();
            m_ui->label_temel_para_birimi->setHidden(true);
            m_ui->commaEdit_doviz_kuru->setHidden(true);
        }
        else {
            if ( ( m_cari_para_birimi_id NE m_temel_para_birimi_id ) AND
                 ( m_bnk_hesabi_para_birimi_id NE m_temel_para_birimi_id) ) {
               m_ui->commaEdit_parite->setHidden(false);
               m_ui->label_parite->setHidden(false);
               m_ui->label_bordro_para_birimi->setHidden(false);
               m_ui->label_bnk_cari_para_birimi->setHidden(false);
            }
        }
    }

    sql_query.PREPARE_SELECT ( "cek_bordro_satirlari,cek_cekler_senetler",
                               " bordro_satiri_id, order_number, cek_bordro_satirlari.cek_senet_id, "
                               "portfoy_numarasi, cek_senet_tutari, aciklama, cek_senet_numarasi, vade_tarihi, "
                               "banka_ismi, sube_ismi,doviz_id ",
                               "cek_bordro_satirlari.cek_senet_id = cek_cekler_senetler.cek_senet_id "
                               "AND bordro_id = :bordro_id ");
    sql_query.SET_VALUE     ( ":bordro_id" , bordro_id );

    if ( sql_query.SELECT( "order_number ASC " ) EQ 0 ) {
        return ADAK_OK;
    }

    int                current_row;
    QTableWidgetItem * new_item;

    while ( sql_query.NEXT() EQ true ) {

        current_row  = ADD_NEW_LINE();

        new_item = new QTableWidgetItem ( tr ( "%1" ).arg ( sql_query.VALUE(0).toString() ) );
        m_ui->tablewidget_bordro_satirlari->setItem( current_row, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( tr ( "%1" ).arg ( QVariant ( bordro_id ).toString()) );
        m_ui->tablewidget_bordro_satirlari->setItem ( current_row, BORDRO_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( tr ( "%1" ).arg ( sql_query.VALUE(1).toString() ) );
        m_ui->tablewidget_bordro_satirlari->setItem ( current_row, ORDER_COLUMN, new_item);

        QLineEdit * lineedit_old_cek_senet_id = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( current_row, OLD_CEK_SNT_ID_COLUMN );
        lineedit_old_cek_senet_id->setText(sql_query.VALUE(2).toString());

        QLineEdit * lineedit_current_cek_senet_id = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( current_row, CURRENT_CEK_SNT_ID_COLUMN );
        lineedit_current_cek_senet_id->setText ( sql_query.VALUE(2).toString() );

        QSearchEdit *  search_edit = ( QSearchEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( current_row, PORTFOY_NO_COLUMN );
        search_edit->SET_TEXT ( sql_query.VALUE(3).toString() );

        QCommaEdit * commaedit_cek_tutari = ( QCommaEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( current_row,TUTAR_COLUMN);
        commaedit_cek_tutari->SET_DOUBLE ( sql_query.VALUE(4).toDouble() );

        QLineEdit * lineedit_aciklama = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( current_row, ACIKLAMA_COLUMN );
        lineedit_aciklama->setText ( sql_query.VALUE(5).toString() );

        QLineEdit * lineedit_cek_no = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( current_row, CEK_NO_COLUMN );
        lineedit_cek_no->setText ( sql_query.VALUE(6).toString() );

        QAdakDate * adakDate_vade_tarihi = ( QAdakDate * ) m_ui->tablewidget_bordro_satirlari->cellWidget( current_row, VADE_TARIHI_COLUMN );
        adakDate_vade_tarihi->SET_DATE( sql_query.VALUE(7).toDate() ) ;

        QLineEdit * lineedit_valor = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( current_row, VALOR_COLUMN );
        QDate vade_tarihi          = QDate::fromString(sql_query.VALUE(7).toString(),"yyyy.MM.dd");
        lineedit_valor->setText( QVariant ( CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI( vade_tarihi ) ).toString() );

        QLineEdit * lineedit_banka_ismi = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget(current_row,BANKA_COLUMN);
        lineedit_banka_ismi->setText ( sql_query.VALUE(8).toString() );

        QLineEdit * lineedit_sube_ismi = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( current_row, SUBE_COLUMN );
        lineedit_sube_ismi->setText ( sql_query.VALUE(9).toString());

        QLineEdit * lineedit_para_birimi = (QLineEdit*) m_ui->tablewidget_bordro_satirlari->cellWidget(current_row,PARA_BIRIMI_COLUMN);
        lineedit_para_birimi->setText(DVZ_GET_DOVIZ_KODU(sql_query.VALUE(10).toInt()));

    }

    int ortalama_gun= CEK_ORTALAMA_GUN_SAYISI( bordro_id, -1 );
    m_ui->lineedit_ort_gun->setText(QVariant(ortalama_gun).toString());
    m_ui->adakDate_vade->SET_DATE(QDate::currentDate().addDays(ortalama_gun));

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::SET_LINE_DEFAULTS ( int row_number )
{

    QLineEdit  * lineedit_cek_no         = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number, CEK_NO_COLUMN     );
    QLineEdit  * lineedit_banka_ismi     = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number, BANKA_COLUMN      );
    QLineEdit  * lineedit_sube_ismi      = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number, SUBE_COLUMN       );
    QCommaEdit * commaedit_tutar         = ( QCommaEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number,TUTAR_COLUMN       );
    QAdakDate  * adakDate_vade_tarihi    = (QAdakDate  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number,VADE_TARIHI_COLUMN );
    QLineEdit  * lineedit_valor          = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number,VALOR_COLUMN       );
    QLineEdit  * lineedit_aciklama       = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number,ACIKLAMA_COLUMN    );
    QLineEdit  * lineedit_para_birimi    = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number,PARA_BIRIMI_COLUMN );


    lineedit_cek_no->setReadOnly(true);
    lineedit_banka_ismi->setReadOnly(true);
    lineedit_sube_ismi->setReadOnly(true);
    commaedit_tutar->setReadOnly(true);
    adakDate_vade_tarihi->setDisabled(true);
    //adakDate_vade_tarihi->setDisplayFormat("dd MMMM yyyy");
    lineedit_valor->setReadOnly(true);
    lineedit_aciklama->setMaxLength(128);
    lineedit_para_birimi->setReadOnly(true);
    lineedit_cek_no->setMaxLength(20);
    lineedit_banka_ismi->setMaxLength(50);
    lineedit_sube_ismi->setMaxLength(50);
    lineedit_valor->setMaxLength(5);
    lineedit_aciklama->setMaxLength(128);
    lineedit_para_birimi->setMaxLength(5);

}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
      if ( object EQ m_ui->searchEdit_cari_hesap_kodu ) {

        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineedit_cari_hesap_ismi->clear();
            m_cari_hesap_id = 0;
            return ADAK_OK;
        }

        SQL_QUERY query ( DB );

        QString cari_hesap_ismi;
        int     para_birim_id, cari_hesap_id;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT() , &cari_hesap_id, &cari_hesap_ismi, &para_birim_id );

        if ( result EQ 0 ) {
            MSG_WARNING( tr ( "Cari Hesap Kodu bulunamadı." ), m_ui->searchEdit_cari_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        m_cari_hesap_id = cari_hesap_id;
        m_ui->lineedit_cari_hesap_ismi->setText ( cari_hesap_ismi );

        QString cari_para_birimi_kodu = DVZ_GET_DOVIZ_KODU ( para_birim_id);

        m_ui->lineEdit_cari_para_birimi_kodu->setText( cari_para_birimi_kodu );
        m_cari_para_birimi_id = para_birim_id;

        if ( m_ui->comboBox_bordro_para_birimi->currentIndex() EQ -1 ) {
            m_ui->comboBox_bordro_para_birimi->setCurrentIndex(m_ui->comboBox_bordro_para_birimi->findText(cari_para_birimi_kodu));
            m_bordro_para_birimi_id = m_cari_para_birimi_id;

            if ( m_cari_para_birimi_id NE m_temel_para_birimi_id ) {
                m_ui->label_temel_para_birimi->setHidden(false);
                m_ui->commaEdit_doviz_kuru->setHidden(false);
                m_ui->commaEdit_doviz_kuru->SET_DOUBLE(DVZ_DOVIZ_KURUNU_AL(m_bordro_para_birimi_id,
                                                                       m_ui->adakDate_bordro_tarihi->QDATE(),
                                                                       ENUM_SATIS));
            }
        }
        if ( (m_bordro_para_birimi_id NE m_cari_para_birimi_id) AND m_cari_para_birimi_id NE m_temel_para_birimi_id) {
            m_ui->label_parite->setHidden(false);
            m_ui->commaEdit_parite->setHidden(false);
            m_ui->label_bordro_para_birimi->setHidden(false);
            m_ui->label_bnk_cari_para_birimi->setHidden(false);
            m_ui->commaEdit_parite->SET_DOUBLE(DVZ_PARITEYI_AL(m_cari_para_birimi_id , m_bordro_para_birimi_id,
                                                           m_ui->adakDate_bordro_tarihi->QDATE(),ENUM_SATIS));
            m_ui->label_bordro_para_birimi->setText("1 " + m_ui->comboBox_bordro_para_birimi->currentText() );
            m_ui->label_bnk_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi_kodu->text());

        }
        else {
            m_ui->label_parite->setHidden(true);
            m_ui->commaEdit_parite->setHidden(true);
            m_ui->label_bordro_para_birimi->setHidden(true);
            m_ui->label_bnk_cari_para_birimi->setHidden(true);
        }
    }
    else if ( object EQ m_ui->comboBox_bordro_islem_turu ) {
        if ( m_modul_id EQ CEKSENET_MODULU ) {
            m_bordro_islem_turu = CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_ENUM ( m_ui->comboBox_bordro_islem_turu->currentText() );
        }
        else if ( m_modul_id EQ CARI_MODULU ) {
            m_bordro_islem_turu = CEK_GET_CEK_SENET_ENT_BORDROSU_ISLEM_TURU_ENUM ( m_ui->comboBox_bordro_islem_turu->currentText() );
        }

        return ADAK_NEW_RECORD;
    }
    else if ( object EQ m_ui->buton_cek_kaydi ) {
        return KENDI_CEKLERIMIZ();
    }
    else if ( object EQ m_ui->buton_senet_kaydi ) {
        return KENDI_SENETLERIMIZ();
    }
    else if ( object EQ m_ui->combobox_banka_hesap_kodu  ) {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("cek_bordro_satirlari",
                             "bordro_satiri_id ",
                             "bordro_id = :bordro_id");
        query.SET_VALUE(":bordro_id" , GET_RECORD_ID());

        if ( query.SELECT() NE 0 ) {
            MSG_WARNING( tr("Bordro işlem görmüştür.Banka hesabını değiştiremezsiniz."), m_ui->combobox_banka_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        int row = -1;

        if ( object EQ m_ui->combobox_banka_hesap_kodu ) {
            row = m_ui->combobox_banka_hesap_kodu->view()->currentIndex().row();
        }
        else {
            int bnk_hesap_id = OPEN_BNK_HESAPLARI_ARAMA(this);

            row = BNK_HESAPLARI_MODEL_INDEX_BUL(bnk_hesap_id,m_ui->combobox_banka_hesap_kodu);

            if ( row NE -1 ) {
                m_ui->combobox_banka_hesap_kodu->setCurrentIndex(row);
            }
        }
        if ( row EQ -1 ) {
            return ADAK_FAIL;
        }

        m_hesap_no_id = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu,row,0).toInt();

        m_banka_ismi           = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu,row,3).toString();
        m_sube_kodu            = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu,row,4).toString();
        m_sube_ismi            = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu,row,5).toString();
        QString hesap_numarasi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu,row,6).toString();

        int bnk_doviz_id = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu,row,1).toInt();
        QString banka_para_birimi_kodu = DVZ_GET_DOVIZ_KODU(bnk_doviz_id);
        m_bnk_hesabi_para_birimi_id = bnk_doviz_id;

        m_ui->lineEdit_doviz_cinsi->setText( banka_para_birimi_kodu );


        m_ui->comboBox_bordro_para_birimi->setCurrentIndex(m_ui->comboBox_bordro_para_birimi->findText(banka_para_birimi_kodu));
        m_bordro_para_birimi_id = bnk_doviz_id;
        m_ui->comboBox_bordro_para_birimi->setDisabled(true);


        QString banka_bilgisi = m_banka_ismi + " / " + m_sube_kodu + "  " + m_sube_ismi + " " + hesap_numarasi;
        m_ui->lineEdit_bnk_sube_bilgisi->setText(banka_bilgisi);

        m_ui->combobox_banka_hesap_kodu->setCurrentIndex(row);


        m_ui->comboBox_bordro_para_birimi->setCurrentIndex(m_ui->comboBox_bordro_para_birimi->findText(banka_para_birimi_kodu));
        m_ui->comboBox_bordro_para_birimi->setDisabled(true);
        m_bordro_para_birimi_id = m_bnk_hesabi_para_birimi_id;

        if ( m_bnk_hesabi_para_birimi_id NE m_temel_para_birimi_id ) {
            m_ui->label_temel_para_birimi->setHidden(false);
            m_ui->commaEdit_doviz_kuru->setHidden(false);
            m_ui->commaEdit_doviz_kuru->SET_DOUBLE(DVZ_DOVIZ_KURUNU_AL(m_bordro_para_birimi_id,
                                                                   m_ui->adakDate_bordro_tarihi->QDATE(),
                                                                   ENUM_SATIS));
        }

    }

    else if ( object EQ m_ui->comboBox_bordro_para_birimi ) {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("cek_bordro_satirlari",
                             "bordro_satiri_id ",
                             "bordro_id = :bordro_id");
        query.SET_VALUE(":bordro_id" , GET_RECORD_ID());

        if ( query.SELECT() NE 0 ) {
            MSG_WARNING( tr("Bordro işlem görmüştür.Bordro para birimini değiştiremezsiniz."), m_ui->comboBox_bordro_para_birimi );

            return ADAK_FAIL_UNDO;
        }

        m_bordro_para_birimi_id = DVZ_GET_DOVIZ_ID ( m_ui->comboBox_bordro_para_birimi->currentText());
        m_ui->commaEdit_doviz_kuru->SET_DOUBLE(DVZ_DOVIZ_KURUNU_AL(m_bordro_para_birimi_id ,
                                                               m_ui->adakDate_bordro_tarihi->QDATE() , ENUM_SATIS));
        if ( m_cari_hesap_id EQ 0 AND m_bnk_hesabi_para_birimi_id EQ 0 ) {
            return ADAK_OK;
        }

        if ( ( m_bordro_para_birimi_id EQ m_cari_para_birimi_id ) OR ( m_bordro_para_birimi_id EQ m_bnk_hesabi_para_birimi_id ) ) {

            if ( m_bordro_para_birimi_id EQ m_temel_para_birimi_id ) {

                m_ui->commaEdit_doviz_kuru->setHidden(true);
                m_ui->label_temel_para_birimi->setHidden(true);
                m_ui->label_parite->setHidden(true);
                m_ui->commaEdit_parite->setHidden(true);
                m_ui->label_bnk_cari_para_birimi->setHidden(true);
                m_ui->commaEdit_parite->clear();
                m_ui->commaEdit_doviz_kuru->clear();
                m_ui->label_bordro_para_birimi->setHidden(true);
            }
            else {
                m_ui->commaEdit_doviz_kuru->setHidden(false);
                m_ui->label_temel_para_birimi->setHidden(false);
                m_ui->commaEdit_parite->clear();
                m_ui->commaEdit_parite->setHidden(true);
                m_ui->label_bordro_para_birimi->setHidden(true);
                m_ui->label_bnk_cari_para_birimi->setHidden(true);
                m_ui->label_parite->setHidden(true);
            }
        }
        else {

            if ( m_bordro_para_birimi_id EQ m_temel_para_birimi_id )
            {
                if ( m_cari_para_birimi_id NE 0 ) {
                    m_ui->commaEdit_parite->SET_DOUBLE ( DVZ_PARITEYI_AL ( m_cari_para_birimi_id ,
                                                                       m_bordro_para_birimi_id, m_ui->adakDate_bordro_tarihi->QDATE(),
                                                                       ENUM_SATIS));
                    m_ui->label_bnk_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi_kodu->text());
                }
                if ( m_bnk_hesabi_para_birimi_id NE 0 ) {
                    m_ui->commaEdit_parite->SET_DOUBLE ( DVZ_PARITEYI_AL ( m_bnk_hesabi_para_birimi_id ,
                                                                       m_bordro_para_birimi_id , m_ui->adakDate_bordro_tarihi->QDATE(),
                                                                       ENUM_SATIS));
                    m_ui->label_bnk_cari_para_birimi->setText(DVZ_GET_DOVIZ_KODU(m_bnk_hesabi_para_birimi_id));
                }
                m_ui->label_bordro_para_birimi->setText("1 " + DVZ_GET_DOVIZ_KODU(m_bordro_para_birimi_id));

                m_ui->label_parite->setHidden(false);
                m_ui->commaEdit_parite->setHidden(false);
                m_ui->label_bordro_para_birimi->setHidden(false);
                m_ui->label_bnk_cari_para_birimi->setHidden(false);

                m_ui->commaEdit_doviz_kuru->clear();
                m_ui->commaEdit_doviz_kuru->setHidden(true);
                m_ui->label_temel_para_birimi->setHidden(true);
            }
            else {
                m_ui->commaEdit_doviz_kuru->setHidden(false);
                m_ui->label_temel_para_birimi->setHidden(false);

                if ( (m_cari_para_birimi_id EQ m_temel_para_birimi_id) OR
                     (m_bnk_hesabi_para_birimi_id EQ m_temel_para_birimi_id) ) {

                    m_ui->commaEdit_parite->clear();
                    m_ui->label_parite->setHidden(true);
                    m_ui->commaEdit_parite->setHidden(true);
                    m_ui->label_bordro_para_birimi->setHidden(true);
                    m_ui->label_bnk_cari_para_birimi->setHidden(true);

                }
                else {
                    if ( m_cari_para_birimi_id NE 0 ) {
                        m_ui->commaEdit_parite->SET_DOUBLE ( DVZ_PARITEYI_AL ( m_cari_para_birimi_id ,
                                                                           m_bordro_para_birimi_id, m_ui->adakDate_bordro_tarihi->QDATE(),
                                                                           ENUM_SATIS));
                        m_ui->label_bnk_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi_kodu->text());
                    }
                    if ( m_bnk_hesabi_para_birimi_id NE 0 ) {
                        m_ui->commaEdit_parite->SET_DOUBLE ( DVZ_PARITEYI_AL ( m_bnk_hesabi_para_birimi_id ,
                                                                           m_bordro_para_birimi_id , m_ui->adakDate_bordro_tarihi->QDATE(),
                                                                           ENUM_SATIS));
                        m_ui->label_bnk_cari_para_birimi->setText(DVZ_GET_DOVIZ_KODU(m_bnk_hesabi_para_birimi_id));
                    }


                    m_ui->commaEdit_doviz_kuru->setHidden(true);
                    m_ui->commaEdit_doviz_kuru->clear();
                    m_ui->label_temel_para_birimi->setHidden(true);

                    m_ui->label_parite->setHidden(false);
                    m_ui->commaEdit_parite->setHidden(false);
                    m_ui->label_bordro_para_birimi->setText("1 " + DVZ_GET_DOVIZ_KODU(m_bordro_para_birimi_id));
                    m_ui->label_bordro_para_birimi->setHidden(false);
                    m_ui->label_bnk_cari_para_birimi->setHidden(false);
                }
            }
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineedit_bordro_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Yeni bordro kaydı için (*) girmelisiniz!.." ), m_ui->lineedit_bordro_no );

        return ADAK_FAIL;
    }



    switch ( m_bordro_islem_turu ) {

        case ENUM_CEK_IADE_ALINDI_SATICIDAN     :
        case ENUM_CEK_MUS_IADE_EDILDI            :
        case ENUM_CEK_SATICIYA_VERILDI                :
            if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
                MSG_WARNING( tr ( "Cari hesap kodunu boş bırakamazsınız!.." ), m_ui->searchEdit_cari_hesap_kodu );

                return ADAK_FAIL;
            }
            break;

        case ENUM_CEK_ODENDI_HESAPTAN        :
        case ENUM_CEK_TAHSIL_EDILDI_HESABA   :
        case ENUM_CEK_BNK_TAHSILATA_VERILDI  :
        case ENUM_CEK_BNK_TEMINATA_VERILDI   :
        case ENUM_CEK_IADE_ALINDI_BANKADAN   :
            if ( m_ui->combobox_banka_hesap_kodu->currentIndex() EQ -1 ) {
                MSG_WARNING( tr ( "Banka hesap kodunu boş bırakamazsınız!.." ), m_ui->combobox_banka_hesap_kodu );

                return ADAK_FAIL;
            }

            break;
        default :
            break;
    }

    if ( m_ui->comboBox_bordro_para_birimi->currentIndex() EQ -1 ) {
        MSG_WARNING( tr ("Bordro para birimini boş bırakamazsınız."), m_ui->comboBox_bordro_para_birimi );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_FIS_RECORD_ADD()
{

    if ( m_modul_id EQ CARI_MODULU ) {

        MSG_WARNING( tr ( "Fiş Cari Modülü tarafından üretilmiştir.Fişte değişiklik yapamazsınız!.."), NULL );
        return ADAK_FAIL;
    }

    if ( QString ( m_ui->lineedit_bordro_no->text().at(0)) NE "*" ) {
        int bordro_no = m_ui->lineedit_bordro_no->text().toInt();

        SQL_QUERY select_query ( DB ) ;

        select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id ",
                                      "bordro_no = :bordro_no "
                                      "AND bordro_tarihi     = :bordro_tarihi" );

        select_query.SET_VALUE      ( ":bordro_no"    ,bordro_no );
        select_query.SET_VALUE      ( ":bordro_tarihi",m_ui->adakDate_bordro_tarihi->DATE());

        if ( select_query.SELECT() NE 0 ) {
            MSG_WARNING( tr ( "Aynı bordro numarasıyla kayıtlı bordro bulundu.Lütfen bordro numarasını değiştiriniz!.."), m_ui->lineedit_bordro_no );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::ADD_FIS_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::ADD_FIS_RECORD()
{

    int bordro_no = 0;

    if ( QString( m_ui->lineedit_bordro_no->text().at(0)) EQ "*" ) {
        bordro_no = CEK_SIRADAKI_BORDRO_NO_AL ( m_ui->adakDate_bordro_tarihi->DATE());
    }
    else {
        bordro_no = m_ui->lineedit_bordro_no->text().toInt();
    }
    M_BORDRO->program_id            = E9_PROGRAMI;
    M_BORDRO->modul_id              = CEKSENET_MODULU;
    M_BORDRO->bordro_no             = bordro_no;
    M_BORDRO->bordro_tarihi         = m_ui->adakDate_bordro_tarihi->DATE();
    M_BORDRO->bordro_islem_turu     = m_bordro_islem_turu;
    M_BORDRO->bordro_turu           = m_bordro_turu;
    M_BORDRO->aciklama              = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_BORDRO->cari_hesap_id         = m_cari_hesap_id;
    M_BORDRO->hesap_no_id           = m_hesap_no_id;
    M_BORDRO->cari_para_birimi_id   = m_cari_para_birimi_id;
    M_BORDRO->bordro_para_birimi_id = m_bordro_para_birimi_id;
    M_BORDRO->doviz_kuru            = m_ui->commaEdit_doviz_kuru->GET_DOUBLE();
    M_BORDRO->parite                = m_ui->commaEdit_parite->GET_DOUBLE();
    M_BORDRO->toplam_tutar          = 0.0;
    M_BORDRO->base_fis_id           = 0;


    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        M_BORDRO->cek_bordrosu_mu  = 1;
    }
    else {
        M_BORDRO->cek_bordrosu_mu = 0;
    }

    int bordro_id = CEK_BORDROSU_EKLE ( M_BORDRO );

    m_ui->lineedit_bordro_no->setText ( QVariant ( bordro_no ).toString());

    m_log_kaydi_eklenmeli_mi = true;
    m_kayit_eklendi          = true;

    m_log_detaylari = QObject::tr  ( "Çek Senet Bordrosu , " ) + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING ( m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + m_ui->adakDate_bordro_tarihi->QDATE().toString("dd MMMM YYYY") + " , " +
                             "Bordro No :" + QVariant(bordro_no).toString() );


    return bordro_id;

}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_FIS_RECORD_UPDATE ( int bordro_id )
{
    if ( m_modul_id EQ CARI_MODULU ) {
        MSG_WARNING( tr ( "Fiş Cari Modülü tarafından üretilmiştir!..Değiştiremezsiniz!.."), NULL );
        return ADAK_FAIL;
    }

    SQL_QUERY select_query ( DB );

    if ( QString ( m_ui->lineedit_bordro_no->text().at(0) ) EQ "*" ) {
        MSG_WARNING( tr ( "Bordro numarasını güncellemek için bordro numarası kısmına sadece sayı girebilirsiniz."), m_ui->lineedit_bordro_no );

        return ADAK_FAIL;
    }

    int bordro_no = m_ui->lineedit_bordro_no->text().toInt();

    select_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id ",
                                  "bordro_no = :bordro_no "
                                  "AND bordro_tarihi     = :bordro_tarihi" );

    select_query.SET_VALUE      ( ":bordro_no",bordro_no);
    select_query.SET_VALUE      ( ":bordro_tarihi",  m_ui->adakDate_bordro_tarihi->DATE());

    if ( select_query.SELECT() > 0 ) {
        select_query.NEXT();
        if ( select_query.VALUE(0).toInt() NE bordro_id) {
            MSG_WARNING( tr ( "Aynı bordro numarasıyla kayıtlı bordro bulundu.Lütfen bordro numarasını değiştiriniz!.."), m_ui->lineedit_bordro_no );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::UPDATE_FIS_RECORD ( int bordro_id )
{

    int bordro_no = m_ui->lineedit_bordro_no->text().toInt();

    CEK_BORDRO_BILGILERI_OKU ( M_BORDRO , bordro_id);

    M_BORDRO->bordro_no             = bordro_no;
    M_BORDRO->bordro_tarihi         = m_ui->adakDate_bordro_tarihi->DATE();
    M_BORDRO->aciklama              = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_BORDRO->cari_hesap_id         = m_cari_hesap_id;
    M_BORDRO->hesap_no_id           = m_hesap_no_id;
    M_BORDRO->bordro_para_birimi_id = m_bordro_para_birimi_id;
    M_BORDRO->doviz_kuru            = m_ui->commaEdit_doviz_kuru->GET_DOUBLE();
    M_BORDRO->parite                = m_ui->commaEdit_parite->GET_DOUBLE();
    M_BORDRO->cari_para_birimi_id   = m_cari_para_birimi_id;

    CEK_BORDRO_GUNCELLE(M_BORDRO , bordro_id);


    m_log_kaydi_eklenmeli_mi = true;

    m_log_detaylari = QObject::tr ( "Çek Senet Bordrosu , " ) + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + m_ui->adakDate_bordro_tarihi->QDATE().toString("dd MMMM YYYY") + " , " +
                             "Bordro Numarası :" + QVariant ( bordro_no ).toString() + " , Toplam Tutar : " +
                             VIRGUL_EKLE ( QVariant ( ROUND  ( M_BORDRO->toplam_tutar ) ).toString() , 2 ) );



}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_FIS_RECORD_DELETE ( int bordro_id )
{
    if ( m_modul_id EQ CARI_MODULU) {
        MSG_WARNING( tr ( "Fiş Cari Modülü tarafından üretilmiştir!..Silemezsiniz!.."), NULL );
        return ADAK_FAIL;
    }

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("cek_bordro_satirlari",
                         "cek_senet_id",
                         "bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , bordro_id);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        int cek_senet_id     = query.VALUE(0).toInt();

        if ( SATIR_SILINEBILIR_MI_GUNCELLENEBILIR_MI ( bordro_id , cek_senet_id ) EQ false ) {
            MSG_WARNING( tr ( "Bordrodaki işlem türü çekin son hareketi olmadığından,"
                                                     "bordro satırını silemezsiniz!.." ), NULL );
            return ADAK_FAIL;
        }
    }

    Q_UNUSED (bordro_id );

    return ADAK_OK;

}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::DELETE_FIS_RECORD ( int bordro_id )
{
    CEK_BORDRO_SIL ( M_BORDRO , bordro_id);

    QDate bordro_tarihi = QDate::fromString(M_BORDRO->bordro_tarihi , "yyyy.MM.dd");

    m_log_detaylari = QObject::tr ( "Çek Senet Bordrosu , ") + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING ( m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + bordro_tarihi.toString("dd MMMM yyyy dddd") + " , " +
                             "Bordro No :" + QVariant(M_BORDRO->bordro_no).toString() + " , Toplam Tutar : " +
                             VIRGUL_EKLE ( QVariant( ROUND ( M_BORDRO->toplam_tutar) ).toString() , 2 ) );


}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::SELECT_FIS_RECORD()
{
    int cek_bordrosu_mu = 0;

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
         cek_bordrosu_mu = 1;
    }

    return OPEN_CEK_SENET_BORDROSU_ARAMA ( m_modul_id , m_bordro_turu , m_bordro_islem_turu,
                                           cek_bordrosu_mu, ENUM_CEK_SENET_FISI, this );
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::FIND_FIS_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::FIND_FIS_RECORD()
{
   if ( m_ui->lineedit_bordro_no->text().toInt() EQ 0 ) {
        MSG_WARNING( tr ( "Aradığınız bordro numarasını , Bordro No alanına yazmalısınız!.."), m_ui->lineedit_bordro_no );

        return 0;
   }

   SQL_QUERY select_query ( DB );
   select_query.PREPARE_SELECT ("cek_bordrolar","bordro_id",
                                "bordro_no             = :bordro_no "
                                "AND modul_id          = :modul_id "
                                "AND bordro_turu       = :bordro_turu AND "
                                "bordro_islem_turu     = :bordro_islem_turu "
                                "AND cek_bordrosu_mu   = :cek_bordrosu_mu "
                                "AND bordro_tarihi     = :bordro_tarihi");

   select_query.SET_VALUE     ( ":bordro_no"         , m_ui->lineedit_bordro_no->text().toInt() );
   select_query.SET_VALUE     ( ":modul_id"          , m_modul_id );
   select_query.SET_VALUE     ( ":bordro_turu"       , m_bordro_turu );
   select_query.SET_VALUE     ( ":bordro_islem_turu" , m_bordro_islem_turu );

   if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
       select_query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
   }
   else {
       select_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
   }

   select_query.SET_VALUE     ( ":bordro_tarihi"     , m_ui->adakDate_bordro_tarihi->DATE());


   if ( select_query.SELECT() EQ 0 ) {
       MSG_WARNING( tr ( "Aradığınız Bordro kayıtlarda bulunamadı."), m_ui->lineedit_bordro_no );
       return 0;
   }
   select_query.NEXT();
   return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::FIND_FIRST_FIS_RECORD()
{
   SQL_QUERY query ( DB );

   QString sql_query = QString("modul_id              = :modul_id "
                               "AND bordro_turu       = :bordro_turu "
                               "AND bordro_islem_turu = :bordro_islem_turu "
                               "AND cek_bordrosu_mu   = :cek_bordrosu_mu ");

   query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", sql_query );

   query.SET_VALUE(":modul_id"          , m_modul_id );
   query.SET_VALUE(":bordro_turu"       , m_bordro_turu );
   query.SET_VALUE(":bordro_islem_turu" , m_bordro_islem_turu );

   if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
       query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
   }
   else {
       query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
   }

   if ( query.SELECT( "bordro_tarihi, bordro_no" , 0, 1 ) EQ 0 ) {
       return 0;
   }
   query.NEXT();
   return query.VALUE(0).toInt();

}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::FIND_LAST_FIS_RECORD()
{
   SQL_QUERY query ( DB );

   QString sql_query = QString("modul_id = :modul_id "
                               "AND bordro_turu       = :bordro_turu "
                               "AND bordro_islem_turu = :bordro_islem_turu "
                               "AND cek_bordrosu_mu   = :cek_bordrosu_mu ");

   query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", sql_query );

   query.SET_VALUE            ( ":modul_id"          , m_modul_id );
   query.SET_VALUE            ( ":bordro_turu"       , m_bordro_turu );
   query.SET_VALUE            ( ":bordro_islem_turu" , m_bordro_islem_turu );

   if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
       query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
   }
   else {
       query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
   }

   if ( query.SELECT( "bordro_tarihi DESC, bordro_no DESC", 0, 1 ) EQ 0 ) {
       return 0;
   }
   query.NEXT();
   return query.VALUE(0).toInt();
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );

    QString query = QString ("bordro_no < :bordro_no "
                             "AND modul_id          = :modul_id "
                             "AND bordro_turu       = :bordro_turu "
                             "AND bordro_islem_turu = :bordro_islem_turu "
                             "AND cek_bordrosu_mu   = :cek_bordrosu_mu "
                             "AND bordro_tarihi     = :bordro_tarihi");

    sql_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query );

    sql_query.SET_VALUE          ( ":bordro_no"         , m_ui->lineedit_bordro_no->text().toInt());
    sql_query.SET_VALUE          ( ":modul_id"          , m_modul_id       );
    sql_query.SET_VALUE          ( ":bordro_turu"       , m_bordro_turu       );
    sql_query.SET_VALUE          ( ":bordro_islem_turu" , m_bordro_islem_turu );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        sql_query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
    }
    else {
        sql_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
    }

    sql_query.SET_VALUE     ( ":bordro_tarihi", m_ui->adakDate_bordro_tarihi->DATE());

    if ( sql_query.SELECT( "bordro_no DESC", 0,1) > 0 ) {
        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    query = QString ("modul_id              = :modul_id "
                     "AND bordro_turu       = :bordro_turu "
                     "AND bordro_islem_turu = :bordro_islem_turu "
                     "AND cek_bordrosu_mu   = :cek_bordrosu_mu "
                     "AND bordro_tarihi     < :bordro_tarihi");

    sql_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query );

    sql_query.SET_VALUE             (  ":modul_id"          , m_modul_id       );
    sql_query.SET_VALUE             (  ":bordro_turu"       , m_bordro_turu       );
    sql_query.SET_VALUE             (  ":bordro_islem_turu" , m_bordro_islem_turu );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        sql_query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
    }
    else {
        sql_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
    }

    sql_query.SET_VALUE    ( ":bordro_tarihi", m_ui->adakDate_bordro_tarihi->DATE());

    if ( sql_query.SELECT( "bordro_tarihi DESC, bordro_no DESC", 0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();


}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );

    QString query = QString ("bordro_no             > :bordro_no "
                             "AND modul_id          = :modul_id "
                             "AND bordro_turu       = :bordro_turu "
                             "AND bordro_islem_turu = :bordro_islem_turu "
                             "AND cek_bordrosu_mu   = :cek_bordrosu_mu "
                             "AND bordro_tarihi     = :bordro_tarihi");

    sql_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query );

    sql_query.SET_VALUE            ( ":bordro_no"        , m_ui->lineedit_bordro_no->text().toInt());
    sql_query.SET_VALUE            ( ":modul_id"          , m_modul_id       );
    sql_query.SET_VALUE            ( ":bordro_turu"       , m_bordro_turu       );
    sql_query.SET_VALUE            ( ":bordro_islem_turu" , m_bordro_islem_turu );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        sql_query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
    }
    else {
        sql_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
    }

    sql_query.SET_VALUE     ( ":bordro_tarihi", m_ui->adakDate_bordro_tarihi->DATE());

    if ( sql_query.SELECT( "bordro_no", 0, 1 ) > 0 ) {
        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    query = QString ("modul_id              = :modul_id "
                     "AND bordro_turu       = :bordro_turu "
                     "AND bordro_islem_turu = :bordro_islem_turu "
                     "AND cek_bordrosu_mu   = :cek_bordrosu_mu "
                     "AND bordro_tarihi     > :bordro_tarihi");

    sql_query.PREPARE_SELECT ( "cek_bordrolar","bordro_id", query );

    sql_query.SET_VALUE            ( ":modul_id"          , m_modul_id       );
    sql_query.SET_VALUE            ( ":bordro_turu"       , m_bordro_turu       );
    sql_query.SET_VALUE            ( ":bordro_islem_turu" , m_bordro_islem_turu );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        sql_query.SET_VALUE ( ":cek_bordrosu_mu" , 1 );
    }
    else {
        sql_query.SET_VALUE ( ":cek_bordrosu_mu" , 0 );
    }

    sql_query.SET_VALUE     ( ":bordro_tarihi", m_ui->adakDate_bordro_tarihi->DATE());

    if ( sql_query.SELECT( "bordro_tarihi, bordro_no", 0, 1 ) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CHECK_LINE_VAR
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{

    if ( m_modul_id EQ CARI_MODULU ) {

        MSG_WARNING( tr ( "Fiş Cari Modülü tarafından üretilmiştir.Fişte değişiklik yapamazsınız!.."), NULL );
        return ADAK_FAIL;
    }

    QSearchEdit * searchEdit_portfoy_no = ( QSearchEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, PORTFOY_NO_COLUMN );


    if ( object EQ searchEdit_portfoy_no ) {

        switch ( m_bordro_islem_turu ) {

            case ENUM_CEK_IADE_ALINDI_BANKADAN   :
            case ENUM_CEK_ODENDI_HESAPTAN        :
            case ENUM_CEK_TAHSIL_EDILDI_HESABA   :
            case ENUM_CEK_BNK_TAHSILATA_VERILDI  :
                if ( m_ui->combobox_banka_hesap_kodu->currentText() EQ "" ) {
                    MSG_WARNING( tr ( "Banka hesap kodunu boş bırakamazsınız!.."), m_ui->combobox_banka_hesap_kodu );


                    return ADAK_FAIL;
                }
                break;

            case ENUM_CEK_IADE_ALINDI_SATICIDAN :
            case ENUM_CEK_MUS_IADE_EDILDI        :
            case ENUM_CEK_SATICIYA_VERILDI            :

                    if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true) {
                        MSG_WARNING( tr ( "Önce Cari Hesap Kodunu seçmelisiniz!.. "), m_ui->searchEdit_cari_hesap_kodu );

                        return ADAK_FAIL;
                    }
                break;

            default :
                break;
        }

        if ( m_ui->comboBox_bordro_para_birimi->currentIndex() EQ -1 ) {
            MSG_WARNING( tr("Bordro para birimini boş bırakamazsınız."), m_ui->comboBox_bordro_para_birimi );

            return ADAK_FAIL;
        }

        SQL_QUERY select_query ( DB );

        select_query.PREPARE_SELECT ( "cek_cekler_senetler", "cek_senet_id ,musteri_ceki_mi",
                                      "portfoy_numarasi = :portfoy_numarasi AND doviz_id = :doviz_id");

        select_query.SET_VALUE      ( ":portfoy_numarasi" , searchEdit_portfoy_no->GET_TEXT().toInt());
        select_query.SET_VALUE      ( ":doviz_id"         , m_bordro_para_birimi_id );

        if(select_query.SELECT() EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız çek / senet portföyde bulunamadı."), searchEdit_portfoy_no );

            return ADAK_FAIL;
        }
        select_query.NEXT();
        int cek_senet_id          = select_query.VALUE(0).toInt();
        int musteri_cek_senedi_mi = select_query.VALUE(1).toInt();


        switch ( m_bordro_turu ) {
            case ENUM_FIRMA_CEK_BORDROSU :
            case ENUM_FIRMA_SENET_BORDROSU:

                if ( musteri_cek_senedi_mi EQ 1) {
                     MSG_WARNING( tr ( "Aradığınız çek senet firmaya ait değildir.Seçmiş olduğunuz bordro türü ile kullanılamaz!.."), searchEdit_portfoy_no );

                    return ADAK_FAIL;
                }
                break;
            default :
                if ( musteri_cek_senedi_mi EQ false ) {
                     MSG_WARNING( tr ( "Aradığınız çek senet müşteri çek senedi değildir.Seçmiş olduğunuz bordro türü ile kullanılamaz!.."), searchEdit_portfoy_no );

                    return ADAK_FAIL;
                }
            break;
        }

        SQL_QUERY query(DB);
        query.PREPARE_SELECT ( "cek_cekler_senetler", "cek_senet_numarasi, vade_tarihi, cek_senet_tutari, "
                               "banka_ismi, sube_ismi,doviz_id,hesap_no_id",
                               "cek_senet_id = :cek_senet_id" );

        query.SET_VALUE      ( ":cek_senet_id", cek_senet_id );

        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING( tr("Aradığınız çek/senet bordrodaki işlem türüyle kullanılamaz.. "), NULL );
            return ADAK_FAIL_UNDO;
        }

        query.NEXT();

        QLineEdit  * lineEdit_current_cek_senet_id  = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, CURRENT_CEK_SNT_ID_COLUMN );
        QLineEdit  * line_edit_cek_no               = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, CEK_NO_COLUMN      );
        QAdakDate * adakDate_vade_tarihi          = ( QAdakDate * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, VADE_TARIHI_COLUMN );
        QLineEdit  * lineedit_valor                 = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, VALOR_COLUMN       );
        QCommaEdit * commaedit_cek_tutari           = ( QCommaEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, TUTAR_COLUMN       );
        QLineEdit  * lineedit_banka_ismi            = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, BANKA_COLUMN       );
        QLineEdit  * lineedit_sube_ismi             = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, SUBE_COLUMN        );
        QLineEdit  * lineedit_para_birimi           = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, PARA_BIRIMI_COLUMN );

        lineEdit_current_cek_senet_id->setText(QVariant(cek_senet_id).toString());
        line_edit_cek_no->setText(query.VALUE(0).toString());

        QDate vade_tarihi = QDate::fromString(query.VALUE(1).toString(),"yyyy.MM.dd");
        adakDate_vade_tarihi->SET_DATE(query.VALUE(1).toDate());

        lineedit_valor->setText ( QVariant(CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI(vade_tarihi)).toString());

        double cek_tutari = query.VALUE(2).toDouble();

        commaedit_cek_tutari->SET_DOUBLE(cek_tutari);
        if ( m_musteri_cek_senedi_mi EQ 1 ) {
           lineedit_banka_ismi->setText ( query.VALUE(3).toString());
        }
        else {
            QString banka_ismi =  BNK_GET_BANKA_ISIMLERI( query.VALUE("hesap_no_id").toInt() ).at( 0 );
            lineedit_banka_ismi->setText ( banka_ismi);
        }

        lineedit_sube_ismi->setText ( query.VALUE(4).toString());
        lineedit_para_birimi->setText ( DVZ_GET_DOVIZ_KODU ( query.VALUE(5).toInt()));
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CCEK_SENET_BORDROSU_FISI::CHECK_LINE_EMPT
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QSearchEdit * search_edit_portfoy_no = ( QSearchEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number,PORTFOY_NO_COLUMN );
    if ( search_edit_portfoy_no->GET_TEXT().isEmpty() EQ true) {
        MSG_WARNING( tr ( "Portföyden çek / senet seçimi yapmalısınız.." ), search_edit_portfoy_no );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}


/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CHECK_ADD_LINE
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_ADD_LINE ( int bordro_id, int row_number )
{
    if ( m_modul_id EQ CARI_MODULU ) {
        MSG_WARNING( tr ( "Fiş Cari Modülü tarafından üretilmiştir!..Değiştiremezsiniz.."), NULL );
        return ADAK_FAIL;
    }
    Q_UNUSED ( bordro_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::ADD_LINE
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::ADD_LINE ( int bordro_id, int row_number)
{
    QLineEdit  * lineEdit_cek_senet_id     = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number, CURRENT_CEK_SNT_ID_COLUMN );
    QLineEdit  * line_edit_aciklama        = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN);
    QLineEdit  * lineEdit_old_cek_senet_id = ( QLineEdit  * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number, OLD_CEK_SNT_ID_COLUMN );
    QCommaEdit * commaedit_cek_tutari      = ( QCommaEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number, TUTAR_COLUMN );

    CEK_CLEAR_BORDRO_SATIRI_STRUCT(M_BORDRO_SATIRI);

    M_BORDRO_SATIRI->bordro_id        = bordro_id;
    M_BORDRO_SATIRI->aciklama         = line_edit_aciklama->text();
    M_BORDRO_SATIRI->cek_senet_id     = lineEdit_cek_senet_id->text().toInt();
    M_BORDRO_SATIRI->order_number     = m_ui->tablewidget_bordro_satirlari->item(row_number , ORDER_COLUMN)->text().toInt();
    M_BORDRO_SATIRI->cek_tutari       = commaedit_cek_tutari->GET_DOUBLE();
    M_BORDRO_SATIRI->program_id       = M_BORDRO->program_id;
    M_BORDRO_SATIRI->modul_id         = M_BORDRO->modul_id;


    int bordro_satiri_id = CEK_BORDRO_SATIRI_EKLE ( M_BORDRO , M_BORDRO_SATIRI, true );

    m_ui->commaedit_toplam_cek_tutari->SET_DOUBLE ( M_BORDRO->toplam_tutar);

    lineEdit_old_cek_senet_id->setText ( QVariant ( M_BORDRO_SATIRI->cek_senet_id ).toString());

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( bordro_satiri_id ).toString() );
    m_ui->tablewidget_bordro_satirlari->setItem ( row_number,ROW_ID_COLUMN,new_item );

    new_item = new QTableWidgetItem ( QVariant ( bordro_id ).toString() );
    m_ui->tablewidget_bordro_satirlari->setItem ( row_number,BORDRO_ID_COLUMN,new_item );

    int ortalama_gun = CEK_ORTALAMA_GUN_SAYISI( bordro_id, -1 );

    m_ui->lineedit_ort_gun->setText( QVariant ( ortalama_gun ).toString());

    m_ui->adakDate_vade->SET_DATE( QDate::currentDate().addDays (ortalama_gun));


    QDate bordro_tarihi = QDate::fromString(M_BORDRO->bordro_tarihi , "yyyy.MM.dd");

    m_log_kaydi_eklenmeli_mi = true;

    m_log_detaylari = QObject::tr ( "Çek Senet Bordrosu , ") + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + bordro_tarihi.toString("dd MMMM yyyy dddd") + " , " +
                            "Bordro No :" + M_BORDRO->bordro_no + " , Toplam Tutar : " +
                            VIRGUL_EKLE ( QVariant( ROUND (M_BORDRO->toplam_tutar)).toString() , 2));

}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_UPDATE_LINE ( int bordro_id, int row_number )
{
    if ( m_modul_id EQ CARI_MODULU) {
        MSG_WARNING( tr ( "Fiş Cari Modülü tarafından üretilmiştir!..Değiştiremezsiniz.."), NULL );
        return ADAK_FAIL;
    }
    QLineEdit * lineedit_old_cek_senet_id = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget(row_number,OLD_CEK_SNT_ID_COLUMN);

    int cek_senet_id = lineedit_old_cek_senet_id->text().toInt();

    if ( SATIR_SILINEBILIR_MI_GUNCELLENEBILIR_MI ( bordro_id , cek_senet_id ) EQ false) {
        MSG_WARNING( tr ( "Bordrodaki işlem türü çekin son hareketi olmadığından,"
                                                "bordro satırını değiştiremezsiniz." ), lineedit_old_cek_senet_id );


        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_FISI::UPDATE_LINE
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::UPDATE_LINE ( int fis_id, int row_number )
{
    int bordro_satiri_id = m_ui->tablewidget_bordro_satirlari->item( row_number, ROW_ID_COLUMN )->text().toInt();

    QLineEdit   * lineEdit_current_cek_senet_id = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, CURRENT_CEK_SNT_ID_COLUMN );
    QLineEdit   * lineEdit_aciklama             = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number, ACIKLAMA_COLUMN           );
    QCommaEdit  * commaEdit_cek_tutari          = ( QCommaEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( row_number , TUTAR_COLUMN);

    CEK_CLEAR_BORDRO_SATIRI_STRUCT(M_BORDRO_SATIRI);
    CEK_BORDRO_SATIRINI_OKU(M_BORDRO_SATIRI,bordro_satiri_id);

    M_BORDRO_SATIRI->bordro_id        = fis_id;
    M_BORDRO_SATIRI->cek_senet_id     = lineEdit_current_cek_senet_id->text().toInt();
    M_BORDRO_SATIRI->aciklama         = lineEdit_aciklama->text();
    M_BORDRO_SATIRI->cek_tutari       = commaEdit_cek_tutari->GET_DOUBLE();
    M_BORDRO->cari_para_birimi_id     = m_cari_para_birimi_id;

    CEK_BORDRO_SATIRINI_GUNCELLE ( M_BORDRO , M_BORDRO_SATIRI , bordro_satiri_id );

    m_ui->commaedit_toplam_cek_tutari->SET_DOUBLE(M_BORDRO->toplam_tutar);

}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHECK_DELETE_LINE ( int fis_id, int row_number )
{
    if ( m_modul_id EQ CARI_MODULU ) {
         MSG_WARNING( tr ( "Fiş Cari Modülü tarafından üretilmiştir!..Silemezsiniz.."), NULL );
         return ADAK_FAIL;
    }
    QLineEdit * lineedit_old_cek_senet_id = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget(row_number , OLD_CEK_SNT_ID_COLUMN);
    int cek_senet_id = lineedit_old_cek_senet_id->text().toInt();

    if ( SATIR_SILINEBILIR_MI_GUNCELLENEBILIR_MI ( fis_id , cek_senet_id ) EQ false ) {
        MSG_WARNING( tr ( "Bordrodaki işlem türü çekin son hareketi olmadığından,"
                                                 "bordro satırını silemezsiniz!.." ), lineedit_old_cek_senet_id );


        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::DELETE_LINE
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::DELETE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED (fis_id);

    QLineEdit * lineEdit_cek_senet_id = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget ( row_number, CURRENT_CEK_SNT_ID_COLUMN );

    int bordro_satiri_id = m_ui->tablewidget_bordro_satirlari->item( row_number, ROW_ID_COLUMN )->text().toInt();

    M_BORDRO_SATIRI->cek_senet_id = lineEdit_cek_senet_id->text().toInt();

    CEK_BORDRO_SATIRINI_SIL(M_BORDRO , bordro_satiri_id);

    m_ui->commaedit_toplam_cek_tutari->SET_DOUBLE ( M_BORDRO->toplam_tutar);

    QDate bordro_tarihi = QDate::fromString(M_BORDRO->bordro_tarihi , "yyyy.MM.dd");

    m_log_kaydi_eklenmeli_mi = true;

    m_log_detaylari = QObject::tr ( "Çek Senet Bordrosu , ") + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu);
    m_log_detaylari.append ( "\n" + bordro_tarihi.toString("dd MMMM yyyy dddd") + " , " +
                           "Bordro No :" + QVariant(M_BORDRO->bordro_no).toString() + " , Toplam Tutar : " +
                           VIRGUL_EKLE ( QVariant( ROUND ( M_BORDRO->toplam_tutar)).toString() , 2));


}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::LOCK_FIS_RECORD ( int fis_id)
{
    return DB->LOCK_ROW ( "bordro_id","cek_bordrolar", QString ( "bordro_id = %1" ).arg ( fis_id ));
}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::UNLOCK_FIS_RECORD ( int fis_id )
{
    DB->UNLOCK_ROW ( "bordro_id","cek_bordrolar", QString ( "bordro_id = %1" ).arg ( fis_id ));
}



/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::LINE_SEARCH_EDIT_CLICKED(int row_number,QWidget * widget,QLineEdit * line_edit)
{
    Q_UNUSED ( widget      );
    Q_UNUSED ( row_number  );
    int hesap_id = 0;

    if ( m_modul_id EQ CARI_MODULU ) {
        MSG_WARNING( tr("Fiş Cari Modülü tarafından üretilmiştir.Fişte değişiklik yapamazsınız!.."), NULL );
        return;
    }

    switch ( m_bordro_islem_turu ) {

        case ENUM_CEK_IADE_ALINDI_BANKADAN   :
        case ENUM_CEK_ODENDI_HESAPTAN        :
        case ENUM_CEK_TAHSIL_EDILDI_HESABA   :
        case ENUM_CEK_BNK_TAHSILATA_VERILDI  :
            if ( m_ui->combobox_banka_hesap_kodu->currentText().isEmpty() EQ true) {
                MSG_WARNING( tr("Banka hesap kodunu boş bırakamazsınız!.."), m_ui->combobox_banka_hesap_kodu );

                return;
            }
            hesap_id = m_hesap_no_id;
            break;

        case ENUM_CEK_IADE_ALINDI_SATICIDAN :
        case ENUM_CEK_MUS_IADE_EDILDI        :
        case ENUM_CEK_SATICIYA_VERILDI            :

            if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true) {
                MSG_WARNING( tr("Önce Cari Hesap Kodunu seçmelisiniz!.. "), m_ui->searchEdit_cari_hesap_kodu );

                return;
            }
            hesap_id = m_cari_hesap_id;
            break;
        default :
            break;
    };

    int cek_id = OPEN_CEK_SENET_ARAMA ( m_bordro_islem_turu, m_cek_mi_senet_mi, m_musteri_cek_senedi_mi,
                                        hesap_id,m_bordro_para_birimi_id, this );

    if ( cek_id EQ -1 ) {
        return;
    }

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT ( "cek_cekler_senetler", "portfoy_numarasi",
                                  "cek_senet_id = :cek_senet_id AND doviz_id = :doviz_id" );
    select_query.SET_VALUE      ( ":cek_senet_id", cek_id );
    select_query.SET_VALUE      ( ":doviz_id"    , m_bordro_para_birimi_id);

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }
    select_query.NEXT();
    line_edit->setText( select_query.VALUE(0).toString() );

}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::CHANGER_BUTTON_CLICKED()
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{
    if ( button EQ m_ui->pushButton_detaylar ) {

        int record_id = GET_RECORD_ID();

        if ( record_id > 0 ) {
            int entegrasyon_turu = CEK_GET_BORDRO_ISLEM_TURUNE_GORE_ENTEGRASYON_TURU(m_bordro_islem_turu,
                                                                                 M_BORDRO->modul_id);
            if ( entegrasyon_turu EQ ENUM_MUHASEBE_ENTEGRASYONU ) {

                int muh_fis_id = MUH_GET_ENT_FIS_ID(M_BORDRO->program_id , M_BORDRO->modul_id,
                                                    record_id);
                M_ENTEGRASYON_DETAYLARI->muh_fis_no = E9_GET_FIS_NO(MUHASEBE_MODULU , muh_fis_id);
            }
            else if ( entegrasyon_turu EQ ENUM_CARI_ENTEGRASYON ) {
                int cari_fis_id = CARI_GET_ENT_FIS_ID(M_BORDRO->program_id , M_BORDRO->modul_id,
                                                      record_id);

                M_ENTEGRASYON_DETAYLARI->cari_fis_no = E9_GET_FIS_NO(CARI_MODULU , cari_fis_id);

            }
            else if ( entegrasyon_turu EQ ENUM_BANKA_ENTEGRASYONU ) {

                int bnk_fis_id = BNK_GET_ENT_FIS_ID(M_BORDRO->program_id , M_BORDRO->modul_id,
                                                    record_id);

                M_ENTEGRASYON_DETAYLARI->banka_fis_no << QVariant(E9_GET_FIS_NO(BANKA_MODULU , bnk_fis_id)).toString();

            }
        }
        OPEN_E9_ENT_DETAYLARI_BATCH ( M_ENTEGRASYON_DETAYLARI , false,this);

    }
    return ADAK_RECORD_UNCHANGED;
}


/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::KENDI_CEKLERIMIZ
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::KENDI_CEKLERIMIZ ()
{
    OPEN_CEK_SENET_FORMU ( ENUM_CEK,-1, this );
    return ADAK_OK;
}

/**************************************************************************************
                    CEK_SENET_BORDdateEdit_vade_tarihiROSU_FISI::KENDI_SENETLERIMIZ
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::KENDI_SENETLERIMIZ ()
{
    OPEN_CEK_SENET_FORMU ( ENUM_SENET, -1,this );
    return ADAK_OK;
}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_record_id )
{
    if ( p_button EQ m_ui->button_yazdir ) {

        OPEN_REPORT_SHOWER ( GET_CEK_FISI_YAZDIR_RAPORU ( p_record_id, m_bordro_islem_turu ), nativeParentWidget() );
    }
    else if ( p_button EQ m_ui->pushButton_urettigi_fisler ) {
        E9_URETILEN_FIS_SEC( E9_PROGRAMI, CEKSENET_MODULU, p_record_id, this );
    }

}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::BORDRO_EKRANINI_DUZENLE
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::BORDRO_EKRANINI_DUZENLE()
{
    m_ui->commaEdit_doviz_kuru->setVisible( false );
    m_ui->label_temel_para_birimi->setVisible( false );
    m_ui->label_parite->setVisible( false );
    m_ui->label_bordro_para_birimi->setVisible( false );
    m_ui->commaEdit_parite->setVisible( false );
    m_ui->label_bnk_cari_para_birimi->setVisible( false );

    m_ui->comboBox_bordro_para_birimi->setEnabled(true);

    int index= -1;

    if ( m_modul_id EQ CEKSENET_MODULU ) {
        index = m_ui->comboBox_bordro_islem_turu->findText ( CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING ( m_bordro_islem_turu ) );
        m_ui->comboBox_bordro_islem_turu->setCurrentIndex      ( index );
    }

    if ( m_modul_id EQ CEKSENET_MODULU ) {

        switch ( m_bordro_turu ) {

            case ENUM_FIRMA_CEK_BORDROSU :
            default                      :
                m_cek_mi_senet_mi       = ENUM_CEK;
                m_musteri_cek_senedi_mi = 0;
                break;

            case ENUM_FIRMA_SENET_BORDROSU :
                m_cek_mi_senet_mi       = ENUM_SENET;
                m_musteri_cek_senedi_mi = 0;
                break;

            case ENUM_MUSTERI_CEK_BORDROSU :
                m_cek_mi_senet_mi       = ENUM_CEK;
                m_musteri_cek_senedi_mi = 1;
                break;

            case ENUM_MUSTERI_SENET_BORDROSU :
                m_cek_mi_senet_mi       = ENUM_SENET;
                m_musteri_cek_senedi_mi = 1;
                break;
        }
    }
    else {

        switch ( m_bordro_islem_turu ) {


            case ENUM_CEK_ENT_BRD_SATICIYA_VER_FIRMA_CEK :
            default                                 :
                m_cek_mi_senet_mi       = ENUM_CEK;
                m_musteri_cek_senedi_mi = 0;
                m_bordro_islem_turu     = ENUM_CEK_SATICIYA_VERILDI;
                m_bordro_turu           = ENUM_FIRMA_CEK_BORDROSU;

                break;

            case ENUM_CEK_ENT_BRD_SATICIYA_VER_FIRMA_SENET :
                m_cek_mi_senet_mi       = ENUM_SENET;
                m_musteri_cek_senedi_mi = 0;
                m_bordro_islem_turu     = ENUM_CEK_SATICIYA_VERILDI;
                m_bordro_turu           = ENUM_FIRMA_SENET_BORDROSU;
                break;

            case ENUM_CEK_ENT_BRD_SATICIYA_VER_MUS_CEK :
                m_cek_mi_senet_mi       = ENUM_CEK;
                m_musteri_cek_senedi_mi = 1;
                m_bordro_islem_turu     = ENUM_CEK_SATICIYA_VERILDI;
                m_bordro_turu           = ENUM_MUSTERI_CEK_BORDROSU;
                break;

            case ENUM_CEK_ENT_BRD_SATICIYA_VER_MUS_SENET :
                m_cek_mi_senet_mi       = ENUM_SENET;
                m_musteri_cek_senedi_mi = 1;
                m_bordro_islem_turu     = ENUM_CEK_SATICIYA_VERILDI;
                m_bordro_turu           = ENUM_MUSTERI_SENET_BORDROSU;
                break;

            case ENUM_CEK_ENT_BRD_MUS_CEK_ALINDI_MUS_CEK :
                m_cek_mi_senet_mi       = ENUM_CEK;
                m_musteri_cek_senedi_mi = 1;
                m_bordro_islem_turu     = ENUM_CEK_MUSTERIDEN_ALINDI;
                m_bordro_turu           = ENUM_MUSTERI_CEK_BORDROSU;
                break;

            case ENUM_CEK_ENT_BRD_MUS_CEK_ALINDI_MUS_SENET :
                m_cek_mi_senet_mi       = ENUM_SENET;
                m_musteri_cek_senedi_mi = 1;
                m_bordro_islem_turu     = ENUM_CEK_MUSTERIDEN_ALINDI;
                m_bordro_turu           = ENUM_MUSTERI_SENET_BORDROSU;
                break;
        }
    }

    if ( m_modul_id EQ CARI_MODULU ) {
        SET_PAGE_TITLE ( "ÇEK-SNT -  ( " + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu) + tr(" ) "));
    }

    QStringList headers;

    headers << "row_id" << "bordro_id" << "column_order"<<"old_cek_senet_id"<< "current_cek_senet_id"<<tr("Portföy No");

    if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
        headers << tr ( "Çek Numarası" );
    }
    else {
       headers << tr ( "Senet Numarası" );
    }
    headers <<tr ( "Vade Tarihi" ) << tr ( "Valör" ) << tr ( "Banka İsmi" ) << tr ( "Şube İsmi" );
    headers <<tr ( "Açıklama" )<<tr ( "Tutar" )<< tr ( "Para Birimi" )<< ""<<"";

    m_ui->tablewidget_bordro_satirlari->setHorizontalHeaderLabels(headers);


    if ( m_cek_mi_senet_mi EQ ENUM_SENET ) {
        m_ui->tablewidget_bordro_satirlari->hideColumn ( BANKA_COLUMN );
        m_ui->tablewidget_bordro_satirlari->hideColumn ( SUBE_COLUMN );

        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( PORTFOY_NO_COLUMN  , 120  );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( CEK_NO_COLUMN      , 134 );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( VADE_TARIHI_COLUMN , 320 );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( VALOR_COLUMN       , 60  );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( ACIKLAMA_COLUMN    , 500 );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( TUTAR_COLUMN       , 90  );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( PARA_BIRIMI_COLUMN , 74  );


        SET_SETTING_NAME ( "CEK_SENET_BORDRO_FISI_SENET" );
    }
    else {

        m_ui->tablewidget_bordro_satirlari->showColumn ( BANKA_COLUMN );
        m_ui->tablewidget_bordro_satirlari->showColumn ( SUBE_COLUMN  );

        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( PORTFOY_NO_COLUMN  , 120  );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( CEK_NO_COLUMN      , 134 );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( VADE_TARIHI_COLUMN , 320 );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( VALOR_COLUMN       , 60  );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( BANKA_COLUMN       , 125 );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( SUBE_COLUMN        , 117 );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( ACIKLAMA_COLUMN    , 360 );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( TUTAR_COLUMN       , 90  );
        m_ui->tablewidget_bordro_satirlari->setColumnWidth ( PARA_BIRIMI_COLUMN , 74  );


        SET_SETTING_NAME ( "CEK_SENET_BORDRO_FISI_CEK" );
    }

    switch( m_bordro_islem_turu ) {

        case ENUM_CEK_TAHSIL_EDILDI_HESABA     :
        case ENUM_CEK_ODENDI_HESAPTAN          :
        case ENUM_CEK_BNK_TAHSILATA_VERILDI    :
        case ENUM_CEK_BNK_TEMINATA_VERILDI     :
        case ENUM_CEK_IADE_ALINDI_BANKADAN     :

            m_ui->searchEdit_cari_hesap_kodu->setVisible     ( false );
            m_ui->lineedit_cari_hesap_ismi->setVisible       ( false );
            m_ui->lineEdit_cari_para_birimi_kodu->setVisible ( false );
            m_ui->label_cari_hesap->setVisible               ( false );
            m_ui->combobox_banka_hesap_kodu->setVisible      ( true  );
            m_ui->label_banka_hesap_kodu->setVisible         ( true  );
            m_ui->lineEdit_bnk_sube_bilgisi->setVisible      ( true  );
            m_ui->lineEdit_doviz_cinsi->setVisible           ( true  );

            if ( m_bordro_islem_turu EQ ENUM_CEK_ODENDI_HESAPTAN ) {
                m_ui->comboBox_bordro_para_birimi->setDisabled(true);
            }
            break;

        case ENUM_CEK_IADE_ALINDI_SATICIDAN :
        case ENUM_CEK_MUS_IADE_EDILDI :
        case ENUM_CEK_SATICIYA_VERILDI     :
        case ENUM_CEK_MUSTERIDEN_ALINDI:

            m_ui->combobox_banka_hesap_kodu->setHidden          ( true  );
            m_ui->label_banka_hesap_kodu->setHidden             ( true  );
            m_ui->lineEdit_bnk_sube_bilgisi->setHidden          ( true  );
            m_ui->lineEdit_doviz_cinsi->setHidden               ( true  );
            m_ui->searchEdit_cari_hesap_kodu->setHidden         ( false );
            m_ui->lineedit_cari_hesap_ismi->setHidden           ( false );
            m_ui->lineEdit_cari_para_birimi_kodu->setHidden     ( false );
            m_ui->label_cari_hesap->setHidden                   ( false );
            break;

        default                  :
            m_ui->combobox_banka_hesap_kodu->setHidden          ( true );
            m_ui->label_banka_hesap_kodu->setHidden             ( true );
            m_ui->lineEdit_bnk_sube_bilgisi->setHidden          ( true );
            m_ui->lineEdit_doviz_cinsi->setHidden               ( true );
            m_ui->searchEdit_cari_hesap_kodu->setHidden         ( true );
            m_ui->lineedit_cari_hesap_ismi->setHidden           ( true );
            m_ui->lineEdit_cari_para_birimi_kodu->setHidden     ( true );
            m_ui->label_cari_hesap->setHidden                   ( true );
            break;
    }

    if ( m_modul_id EQ CARI_MODULU) {
        m_ui->buton_cek_kaydi->setHidden(true);
        m_ui->buton_senet_kaydi->setHidden(true);

    }
    SET_WINDOW_TITLE();
}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::SATIR_SILINEBILIR_MI_GUNCELLENEBILIR_MI
***************************************************************************************/

bool CEK_SENET_BORDROSU_FISI::SATIR_SILINEBILIR_MI_GUNCELLENEBILIR_MI(int fis_id , int cek_senet_id)
{
    // BORDRO silinebilmesi veya güncellenebilmesi için,seçili olan bordrodaki işlem türü ile çekin çekler tablosundaki
    // durumu aynı olmalıdır.Çekin son durumu bordrodaki işlem türünden farklıysa bordro silinemezz!..

    SQL_QUERY select_query(DB);

    int cekin_durumu       =  0;
    int bordro_islem_turu;


    select_query.PREPARE_SELECT("cek_cekler_senetler", "cek_senet_son_durumu", "cek_senet_id = :cek_senet_id");
    select_query.SET_VALUE(":cek_senet_id" , cek_senet_id);
    if ( select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        cekin_durumu = select_query.VALUE(0).toInt();
    }

    select_query.PREPARE_SELECT("cek_bordrolar","bordro_islem_turu", "bordro_id = :bordro_id");
    select_query.SET_VALUE(":bordro_id" , fis_id);

    if (select_query.SELECT() EQ 0 ) {
        return false;
    }

    select_query.NEXT();
    bordro_islem_turu = select_query.VALUE(0).toInt();


    int islem_turune_gore_cekin_durumu = CEK_ISLEM_TURUNE_GORE_CEKIN_DURUMUNU_AL ( bordro_islem_turu );

    if ( cekin_durumu NE islem_turune_gore_cekin_durumu ) {
        return false;
    }
    return true;

}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::SEARCH_EDIT_CLICKED
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::SEARCH_EDIT_CLICKED ( QWidget * widget, QLineEdit * lineEdit )
{
    Q_UNUSED ( widget );

    if ( m_modul_id EQ CARI_MODULU ) {
        MSG_WARNING( tr("Fiş Cari Modülü tarafından üretilmiştir.Fişte değişiklik yapamazsınız!.."), NULL );
        return;
    }

    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( lineEdit->text(), SAHIS_ARTI_FIRMA , this , 1 , E9_PROGRAMI, CARI_MODULU );

    if ( cari_hesap_id > 0 ) {
        lineEdit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id) );
    }
}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::CEK_SENET_ENTEGRASYON_HESAPLARI_KONTROLU
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::CEK_SENET_ENTEGRASYON_HESAPLARI_KONTROLU ( int bordro_id, int current_row, int p_cek_senet_id )
{

    SUBE_ENTEGRASYON_HESAPLARI_STRUCT * SUBE_ENTEGRASYON_HESAPLARI = ( SUBE_ENTEGRASYON_HESAPLARI_STRUCT * ) SUBE_GET_ENTEGRASYON_HESAPLARI_STRUCT();

    SQL_QUERY query ( DB );

    int     portfoydeki_cekler_hesap_id                    = SUBE_ENTEGRASYON_HESAPLARI->port_cekler_hsp_id;
    int     tahsile_verilen_cekler_hesap_id                = SUBE_ENTEGRASYON_HESAPLARI->tah_verilen_cekler_hsp_id;
    int     teminata_verilen_cekler_hesap_id               = SUBE_ENTEGRASYON_HESAPLARI->tem_verilen_cekler_hsp_id;
    int     karsiliksiz_cekler_hesap_id                    = SUBE_ENTEGRASYON_HESAPLARI->kar_cekler_hsp_id;
    int     portfoydeki_senetler_hesap_id                  = SUBE_ENTEGRASYON_HESAPLARI->port_senetler_hsp_id;
    int     tahsile_verilen_senetler_hesap_id              = SUBE_ENTEGRASYON_HESAPLARI->tah_senetler_hsp_id;
    int     teminata_verilen_senetler_hesap_id             = SUBE_ENTEGRASYON_HESAPLARI->tem_senetler_hsp_id;
    int     protestolu_senetler_hesap_id                   = SUBE_ENTEGRASYON_HESAPLARI->pro_senetler_hsp_id;
    int     ileri_tarih_portfoydeki_senetler_hesap_id      = SUBE_ENTEGRASYON_HESAPLARI->port_senetler_ileri_trh_hsp_id;
    int     ileri_tarih_tahsile_verilen_senetler_hesap_id  = SUBE_ENTEGRASYON_HESAPLARI->tah_senetler_ileri_trh_hsp_id;
    int     ileri_tarih_teminata_verilen_senetler_hesap_id = SUBE_ENTEGRASYON_HESAPLARI->tem_senetler_ileri_trh_hsp_id;
    int     borc_senetleri_hesap_id                        = SUBE_ENTEGRASYON_HESAPLARI->borc_senetler_hsp_id;
    int     ileri_tarih_borc_senetleri_hesap_id            = SUBE_ENTEGRASYON_HESAPLARI->borc_senetleri_ileri_trh_hsp_id;
    int     supheli_alacaklar_hesap_id                     = SUBE_ENTEGRASYON_HESAPLARI->sup_alacaklar_hsp_id;
    int     tahsil_edilemeyen_cekler_hesap_id              = SUBE_ENTEGRASYON_HESAPLARI->tahsil_edilemeyen_cekler_hsp_id;
    int     tahsil_edilemeyen_senetler_hesap_id            = SUBE_ENTEGRASYON_HESAPLARI->tahsil_edilemeyen_senetler_hsp_id;
    int     kasa_unite_muhasebe_kasa_hesap_id              = SUBE_ENTEGRASYON_HESAPLARI->unite_muh_kasa_hsp_id;



    query.PREPARE_SELECT ( "cek_bordrolar","cari_hesap_id, hesap_no_id",
                           "bordro_id = :bordro_id" );
    query.SET_VALUE(":bordro_id", bordro_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    int     cari_hesap_id                       = query.VALUE(0).toInt();
    int     banka_hesap_id                      = query.VALUE(1).toInt();

    int     banka_muh_hesap_id                  = 0;
    int     banka_verilen_cekler_muh_hesap_id   = 0;

    banka_muh_hesap_id = BNK_GET_BANKA_MUH_HESAP_ID( banka_hesap_id );

    if ( p_cek_senet_id > 0 ) {
        BNK_MUH_HESAPLARI_STRUCT BNK_MUH_HESAPLAR;

        BNK_GET_BANKA_MUH_HESAPLARI( &BNK_MUH_HESAPLAR, banka_hesap_id );
        banka_verilen_cekler_muh_hesap_id = BNK_MUH_HESAPLAR.verilen_cekler_hesap_id;
    }

    int     cari_muh_hesap_id = 0;

    QLineEdit * current_cek_snt_lineEdit = ( QLineEdit * ) m_ui->tablewidget_bordro_satirlari->cellWidget( current_row, CURRENT_CEK_SNT_ID_COLUMN );
    int         cek_senet_id               = current_cek_snt_lineEdit->text().toInt();

    query.PREPARE_SELECT    ( "cek_cekler_senetler","cek_senet_son_durumu, cek_senet_turu,vade_tarihi, musteri_ceki_mi ",
                              "cek_senet_id = :cek_senet_id");
    query.SET_VALUE(":cek_senet_id", cek_senet_id );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING( tr("Çek/Senet mevcut durumu alınamadı!"), current_cek_snt_lineEdit );

        return;
    }
    query.NEXT();
    int     cek_senet_mevcut_durum          = query.VALUE(0).toInt();
    int     cek_senet_turu                  = query.VALUE(1).toInt();
    QDate   vade_tarihi                     = QDate::fromString(query.VALUE(2).toString(), "yyyy.MM.dd");
    int     vade_tarihine_kalan_gun_sayisi  = CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI(vade_tarihi);
    int     cek_senet_yeni_durum            = CEK_ISLEM_TURUNE_GORE_CEKIN_DURUMUNU_AL ( m_bordro_islem_turu );
    int     musteri_ceki_veya_seneti_mi     = query.VALUE(3).toInt();

    bool    senet_vadesi_bir_yildan_fazla_mi = false;

    if ( vade_tarihine_kalan_gun_sayisi > m_ui->adakDate_bordro_tarihi->QDATE().daysInYear() ) {
        senet_vadesi_bir_yildan_fazla_mi = true;
    }

    switch ( cek_senet_mevcut_durum ) {

        case ENUM_PORTFOYDE :
        {  // Portföydeki çeklerin/senetlerin bir sonraki durumlarına göre entegrasyon kontrolü

            if ( musteri_ceki_veya_seneti_mi EQ 1 ) {
                if ( cek_senet_turu EQ ENUM_CEK ) {
                    if ( portfoydeki_cekler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında portföydeki çekler hesabı tanımlı değil."), NULL);
                        break;
                    }
                }
                else { // Çek değilse senettir
                    // Senetlerin ileri tarihli olup olmaması durumuna göre entegre olacağı
                    // hesapların birbirinden ayrılması durumunu belirliyoruz.
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true  ) {
                        if ( ileri_tarih_portfoydeki_senetler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                                 "portföydeki senetler hesabı tanımlı değil."), NULL );
                            break;
                        }
                    }
                    else {
                        if ( portfoydeki_senetler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                                 "portföydeki senetler hesabı tanımlı değil."), NULL );
                            break;
                        }
                    }
                }
            }
            else {
                if ( cek_senet_turu EQ  ENUM_CEK ) {
                    if ( banka_verilen_cekler_muh_hesap_id < 1 ) {
                        MSG_WARNING( tr("Banka hesabında verilen çekler muhabasebe hesabı tanımlı değil."), NULL );
                        break;
                    }
                }
                else {
                    if (senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                        if ( ileri_tarih_borc_senetleri_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                                 "borç senetleri hesabı tanımlı değil."), NULL );
                            break;
                        }
                    }
                    else {
                        if ( borc_senetleri_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                                 "borc senetleri hesabı tanımlı değil."), NULL );
                            break;
                        }
                    }
                }
            }

            switch ( cek_senet_yeni_durum ) {

                case ENUM_BNK_TAHSILAT_VERILDI :

                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( tahsile_verilen_cekler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında tahsile verilen çekler hesabı tanımlı değil."), NULL );
                            break;
                        }
                        break;
                    }
                     // Çek değilse senettir
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                        if ( ileri_tarih_tahsile_verilen_senetler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                                 "tahsile verilen senetler hesabı tanımlı değil."), NULL );
                            break;
                        }
                        break;
                    }
                    // Vade tarihi 1 yıldan az olan senet
                    if ( tahsile_verilen_senetler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                             "tahsile verilen senetler hesabı tanımlı değil."), NULL ) ;
                        break;
                    }
                    break;

                case ENUM_BNK_TEMINAT_VERILDI :

                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( teminata_verilen_cekler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında teminata verilen çekler hesabı tanımlı değil."), NULL );
                            break;
                        }
                        break;
                    }
                     // Çek değilse senettir
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                        if ( ileri_tarih_teminata_verilen_senetler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                                 "teminata verilen senetler hesabı tanımlı değil."), NULL );
                            break;
                        }
                        break;
                    }
                    // Vade tarihi 1 yıldan az olan senet
                    if ( teminata_verilen_senetler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                             "teminata verilen senetler hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;

                case ENUM_KARSILIKSIZ :

                    if ( karsiliksiz_cekler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında karşılıksız çekler hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;

                case ENUM_PROTESTOLU  :
                    if ( protestolu_senetler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında protestolu senetler hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;

                case ENUM_SATICIYA_VERILDI     :
                case ENUM_MUS_IADE_EDILDI :
                    cari_muh_hesap_id = CARI_GET_MUH_HESAP_ID( cari_hesap_id );
                    if ( cari_muh_hesap_id < 1 ) {
                        MSG_WARNING( tr("Cari hesabın muhasebe hesap kodu cari kartta tanımlı değil. "), NULL );
                        break;
                    }
                    break;
                default :
                    MSG_WARNING( tr("Geçersiz işlem seçimi"), NULL );
                    break;
            }
            break;
        }
        case ENUM_BNK_TEMINAT_VERILDI :
        // Bankaya teminata verilen çeklerin bir sonraki durumlarına göre entegrasyon kontrolü
        {

            if ( cek_senet_turu EQ ENUM_CEK ) {
                if ( teminata_verilen_cekler_hesap_id < 1 ) {
                    MSG_WARNING( tr("Entegrasyon hesaplarında teminata verilen çekler hesabı tanımlı değil."), NULL );
                    break;
                }
            }
            else {
                // Çek değilse senettir
                if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                    if ( ileri_tarih_teminata_verilen_senetler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                             "teminata verilen senetler hesabı tanımlı değil."), NULL );
                        break;
                    }
                }
                if ( teminata_verilen_senetler_hesap_id < 1 ) {
                    MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                         "teminata verilen senetler hesabı tanımlı değil."), NULL );
                    break;
                }
            }
            switch ( cek_senet_yeni_durum ) {
                case ENUM_PORTFOYDE :
                    // sadece müşteri çeki/senedi bankaya teminata verilebilir
                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( portfoydeki_cekler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında portföydeki çekler hesabı tanımlı değil."), NULL );
                            break;
                        }
                        break;
                    }
                    // Çek değilse senettir
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                        if ( ileri_tarih_portfoydeki_senetler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                                 "portföydeki senetler hesabı tanımlı değil."), NULL );
                            break;
                        }
                        break;
                    }
                    if ( portfoydeki_senetler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                             "portföydeki senetler hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;
                case ENUM_TAHSIL_EDILDI_HESABA :    // teminata verilen bir müşteri çeki/senedi direkt olarak tahsil edilebilir.
                    if ( banka_muh_hesap_id < 1 ) {
                        MSG_WARNING( tr("Bordroda tanımlı banka hesabının karşılığı olan muhasebe "
                                                             "hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;
                default :
                    break;
                }
            break;
        }

        case ENUM_BNK_TAHSILAT_VERILDI :
        // Bankaya tahsile verilen çeklerin/senetlerin bir sonraki durumlarına göre entegrasyon kontrolü
        // Sadece müşteri çekleri/senetleri bankaya tahsile verileblir.
        // Bankaya tahsile verilen ceklerin/senetler tahsil edilebilir, iade alınabilir veya karşılıksız çıkabilir
        // Tahsilat nakit veya hesaptan olur. İade ve karşılıksız olması durumunda çek/senet ilk olarak portföye düşer
        // Bu durumda entegrasyon için portföy hesabı etkilenecektir.
        {
            // Önceki durumu Bankaya tahsile verildi olan çek/senedin sonraki durumuna göre entegrasyon kontrolü
            if ( cek_senet_turu EQ ENUM_CEK ) {
                if ( tahsile_verilen_cekler_hesap_id < 1 ) {
                    MSG_WARNING( tr("Entegrasyon hesaplarında tahsile verilen çekler hesabı tanımlı değil."), NULL );
                    break;
                }
                break;
            }
             // Çek değilse senettir
            if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                if ( ileri_tarih_tahsile_verilen_senetler_hesap_id < 1 ) {
                    MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                         "tahsile verilen senetler hesabı tanımlı değil."), NULL );
                    break;
                }
                break;
            }
            // Vade tarihi 1 yıldan az olan senet
            if ( tahsile_verilen_senetler_hesap_id < 1 ) {
                MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                     "tahsile verilen senetler hesabı tanımlı değil."), NULL );
                break;
            }
            break;

            switch ( cek_senet_yeni_durum ) {
                case ENUM_TAHSIL_EDILDI_HESABA :        // Bankaya tahsile verilen müşteri/çeki senedi
                                            //tahsil edilirse banka hesabına borç yazılır.
                    switch ( m_bordro_islem_turu ) {
                        case ENUM_CEK_TAHSIL_EDILDI_HESABA :
                            if ( banka_muh_hesap_id < 1 ) {
                                MSG_WARNING( tr("Bordroda tanımlı banka hesabının karşılığı olan muhasebe "
                                                                     "hesabı tanımlı değil."), NULL );
                                break;
                            }
                            break;
                        case ENUM_CEK_TAHSIL_EDILDI_NKT :
                            if ( kasa_unite_muhasebe_kasa_hesap_id < 1 ) {
                                MSG_WARNING( tr("Ünitede entegrasyon için gerekli muhasebe kasa hesabı tanımlı değil."), NULL );
                                break;
                            }
                            break;
                        default :
                            break;
                        }
                    break;
                case ENUM_PORTFOYDE :    // Bankaya tahsile verilen müşteri çeki/senedi iade alınırsa portföye geri döner

                    if ( cek_senet_turu EQ ENUM_CEK ) {
                        if ( portfoydeki_cekler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında portföydeki çekler hesabı tanımlı değil."), NULL );
                            break;
                        }
                        break;
                    } // çek değilse senettir ( müşteri senedi )
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true  ) {
                        if ( ileri_tarih_portfoydeki_senetler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                                 "portföydeki senetler hesabı tanımlı değil."), NULL );
                            break;
                        }
                        break;
                    }
                    if ( portfoydeki_senetler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                             "portföydeki senetler hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;
            }
            break;
        }
        case ENUM_SATICIYA_VERILDI :
        {
            if ( cek_senet_turu EQ ENUM_CEK ) {
                if ( musteri_ceki_veya_seneti_mi EQ 1 ) { // Müşteri çeki
                    if ( portfoydeki_cekler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında portföydeki çekler hesabı tanımlı değil."), NULL );
                        break;
                    }
                }
                else { // Müşteri çeki değilse kendi çekimizdir
                    if ( banka_verilen_cekler_muh_hesap_id < 1 ) {
                        MSG_WARNING( tr("Banka hesabında verilen çekler muhasebe hesabı tanımlı değil."), NULL );
                        break;
                    }
                }
            }
            else {   // Çek değilse senettir
                if ( musteri_ceki_veya_seneti_mi EQ 1 ) { // Müşteri senedi
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                        if ( ileri_tarih_portfoydeki_senetler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                                 "portföydeki senetler hesabı tanımlı değil."), NULL );
                            break;
                        }
                    }
                    else {
                        if ( portfoydeki_senetler_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                                 "portföydeki senetler hesabı tanımlı değil."), NULL );
                            break;
                        }
                    }
                }
                else { // Müşteri senedi değilse kendi senedimizdir
                    if ( senet_vadesi_bir_yildan_fazla_mi EQ true ) {
                        if (ileri_tarih_borc_senetleri_hesap_id < 1 ) {
                            MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan fazla olan "
                                                                 "borç senetleri hesabı tanımlı değil."), NULL );
                            break;
                        }
                        else {
                            if ( borc_senetleri_hesap_id < 1 ) {
                                MSG_WARNING( tr("Entegrasyon hesaplarında vadesi 1 yıldan az olan "
                                                                     "borç senetleri hesabı tanımlı değil."), NULL );
                                break;
                            }
                        }
                    }
                }
            }

            switch ( cek_senet_yeni_durum ) {

                case ENUM_PORTFOYDE :  // Çek/senet müşteriye verildikten sonra portföye düşmüsse iade alınmış demektir.

                    cari_muh_hesap_id = CARI_GET_MUH_HESAP_ID( cari_hesap_id );
                    if ( cari_muh_hesap_id < 1 ) {
                        MSG_WARNING( tr("Cari hesabın muhasebe hesap kodu cari kartta tanımlı değil."), NULL );
                        break;
                    }

                case ENUM_ODENDI_NKT :           // Müşteriye verilmiş bir çek/senet ancak kendi çekimiz veya senedimizse
                                        // elden veya hesaptan ödendi durumuna düşebilir.
                    if ( kasa_unite_muhasebe_kasa_hesap_id < 1 ) {
                    MSG_WARNING( tr("Ünitede entegrasyon için gerekli muhasebe kasa hesabı tanımlı değil."), NULL );
                    break;
                    }
                    break;
                case ENUM_ODENDI_HESAPTAN :


                    if ( banka_muh_hesap_id < 1 ) {
                        MSG_WARNING( tr("Bordroda tanımlı banka hesabının karşılığı olan muhasebe "
                                                             "hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;
                }
            break;
        }
        case ENUM_KARSILIKSIZ :
        case ENUM_PROTESTOLU  :

        // Karşılıksız/protestolu çeklerin/senetlerin bir sonraki durumlarına göre entegrasyon kontrolü
        {
            if ( cek_senet_turu EQ ENUM_CEK ) {
                if ( karsiliksiz_cekler_hesap_id < 1 ) {
                    MSG_WARNING( tr("Entegrasyon hesaplarında karşılıksız çekler hesabı tanımlı değil."), NULL );
                    break;
                }
            }
            else {
                // Çek değilse senettir
                if ( protestolu_senetler_hesap_id < 1 ) {
                    MSG_WARNING( tr("Entegrasyon hesaplarında protestolu senetler hesabı tanımlı değil."), NULL );
                    break;
                }
            }
            switch ( cek_senet_yeni_durum ) {
                case ENUM_SUPHELI_ALACAK :
                    if ( supheli_alacaklar_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında şüpheli alacaklar hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;
                case ENUM_TAHSIL_EDILDI_NKT :    // Karşılıksız/protestolu durumuna düşmüş bir çek/senet
                                        // sadece elden tahsil edilebilir
                    if ( kasa_unite_muhasebe_kasa_hesap_id < 1 ) {
                        MSG_WARNING( tr("Ünitede entegrasyon için gerekli muhasebe kasa hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;
                case ENUM_MUS_IADE_EDILDI :

                    cari_muh_hesap_id = CARI_GET_MUH_HESAP_ID( cari_hesap_id );
                    if ( cari_muh_hesap_id < 1 ) {
                        MSG_WARNING( tr("Cari hesabın muhasebe hesap kodu cari kartta tanımlı değil."), NULL );
                        break;
                    }
                    break;
                default :
                    break;
            }
            break;
        }

        break;
        case ENUM_SUPHELI_ALACAK :
        // Şüpheli alacak durumundaki çeklerin/senetlerin bir sonraki durumlarınına göre entegrasyon kontrolü
        {
            if ( supheli_alacaklar_hesap_id < 1 ) {
                MSG_WARNING( tr("Entegrasyon hesaplarında şüpheli alacaklar hesabı tanımlı değil."), NULL );
                break;
            }
            switch ( cek_senet_yeni_durum ) {
            case ENUM_TAHSIL_EDILEMEYEN   :
                if ( cek_senet_turu EQ ENUM_CEK ) {
                    if ( tahsil_edilemeyen_cekler_hesap_id < 1 ) {
                        MSG_WARNING( tr("Entegrasyon hesaplarında tahsil edilmeyen çekler hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;
                }
                // Çek değilse senettir
                if ( tahsil_edilemeyen_senetler_hesap_id < 1 ) {
                    MSG_WARNING( tr("Entegrasyon hesaplarında tahsil edilmeyen senetler hesabı tanımlı değil."), NULL );
                    break;
                }
                break;
                case ENUM_MUS_IADE_EDILDI :

                cari_muh_hesap_id = CARI_GET_MUH_HESAP_ID( cari_hesap_id );
                    if ( cari_muh_hesap_id < 1 ) {
                        MSG_WARNING( tr("Cari hesabın muhasebe hesap kodu cari kartta tanımlı değil. "), NULL );
                        break;
                    }
                    break;
                case ENUM_TAHSIL_EDILDI_NKT : // Şüpheli alacaklara düşmüş bir çek elden tahsil edilebilir.
                    if ( kasa_unite_muhasebe_kasa_hesap_id < 1 ) {
                        MSG_WARNING( tr("Ünitede entegrasyon için gerekli muhasebe kasa hesabı tanımlı değil."), NULL );
                        break;
                    }
                    break;
                default :
                    break;
            }
            break;
        }
        case ENUM_TAHSIL_EDILDI_NKT     :
        case ENUM_TAHSIL_EDILDI_HESABA  :
        case ENUM_MUS_IADE_EDILDI       :
        case ENUM_ODENDI_NKT            :
        case ENUM_ODENDI_HESAPTAN       :
        case ENUM_TAHSIL_EDILEMEYEN     :
        default                :
                // Müşteriye iade edilen ve ödenen çeklerin/senetlerin bir sonraki durumlarınına göre entegrasyon kontrolü
                // Sonraki durumları olmaz ...
            break;
    }
}


/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::GET_UYGUN_CEKLER_LIST
***************************************************************************************/


QStringList CEK_SENET_BORDROSU_FISI::GET_UYGUN_CEKLER_LIST(int hesap_id)
{
    QString     sql_query;
    QString     where_query;
    QStringList uygun_cekler_list;

    switch (m_bordro_islem_turu)
    {
        case ENUM_CEK_SATICIYA_VERILDI : {

            QList <int> cek_senet_id_list = CARI_GET_CEK_SENET_ID_LIST( hesap_id, CARI_MODULU );

            if ( cek_senet_id_list.size() EQ 0 ) {
                 uygun_cekler_list << QVariant(-1).toString();

                return uygun_cekler_list;
            }
            for ( int i = 0; i < cek_senet_id_list.size(); i++ ) {
                uygun_cekler_list << QVariant ( cek_senet_id_list.at( i ) ).toString();
            }
            return uygun_cekler_list;
            break;
        }
        case ENUM_CEK_MUS_IADE_EDILDI        :
        case ENUM_CEK_IADE_ALINDI_SATICIDAN :
            sql_query = QString ( "cek_bordrolar,cek_bordro_satirlari");

            where_query = QString ( "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                          "cari_hesap_id = %1" ).arg(hesap_id);


            if(m_bordro_islem_turu EQ ENUM_CEK_IADE_ALINDI_SATICIDAN ) {

                where_query += QString (" AND bordro_islem_turu = %1").arg(ENUM_CEK_SATICIYA_VERILDI);
            }
            if(m_bordro_islem_turu EQ ENUM_CEK_MUS_IADE_EDILDI) {

                where_query += QString(" AND bordro_islem_turu = %1").arg(ENUM_CEK_MUSTERIDEN_ALINDI);
            }
            break;
        case ENUM_CEK_TAHSIL_EDILDI_HESABA   :
        case ENUM_CEK_IADE_ALINDI_BANKADAN   :
            sql_query = QString ( "cek_bordrolar,cek_bordro_satirlari" );

            where_query = QString ("cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                                   "hesap_no_id = %1 AND (bordro_islem_turu = %2 OR bordro_islem_turu = %3)").arg(hesap_id)
                                                                                                       .arg(ENUM_CEK_BNK_TAHSILATA_VERILDI)
                                                                                                       .arg(ENUM_CEK_BNK_TEMINATA_VERILDI);


            break;
        case ENUM_CEK_ODENDI_HESAPTAN :
            sql_query = QString ( "cek_cekler_senetler");

            where_query = QString ( "cek_senet_son_durumu = %1 AND "
                                   "hesap_no_id = %2").arg(ENUM_SATICIYA_VERILDI)
                                                    .arg(hesap_id);
            break;

        default:

        return QStringList();

    };

    SQL_QUERY select_query ( DB );


    select_query.PREPARE_SELECT ( sql_query, "cek_senet_id", where_query );

    if ( select_query.SELECT() EQ 0 ) {
         uygun_cekler_list << QVariant(-1).toString();

        return uygun_cekler_list;
    }
    while ( select_query.NEXT() ) {
        uygun_cekler_list << select_query.VALUE(0).toString();
    }
    return uygun_cekler_list;
}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::SET_WINDOW_TITLE
***************************************************************************************/

void CEK_SENET_BORDROSU_FISI::SET_WINDOW_TITLE()
{
    if ( m_modul_id EQ CEKSENET_MODULU) {
        switch ( m_bordro_turu ) {
            case ENUM_MUSTERI_CEK_BORDROSU :
                SET_PAGE_TITLE ( tr ( "MÜŞTERİ ÇEK BORDROSU" ) );
                SET_HELP_PAGE("cek-senet-islemleri_musteri-cek-bordrosu");
                break;
            case ENUM_MUSTERI_SENET_BORDROSU:
                SET_PAGE_TITLE ( tr ( "MÜŞTERİ SENET BORDROSU" ) );
                SET_HELP_PAGE("cek-senet-islemleri_musteri-senet-bordrosu");
                break;
            case ENUM_FIRMA_CEK_BORDROSU:
                SET_PAGE_TITLE ( tr ( "FİRMA ÇEK BORDROSU" ) );
                SET_HELP_PAGE("cek-senet-islemleri_firma-cek-bordrosu");
                break;
            case ENUM_FIRMA_SENET_BORDROSU:
                SET_PAGE_TITLE ( tr ( "FİRMA SENET BORDROSU" ) );
                SET_HELP_PAGE("cek-senet-islemleri_firma-senet-bordrosu");
                break;
        }
    }
    else {
        SET_PAGE_TITLE ( tr ( "ÇEK/SENET ENT. FİŞLERİ" ) );
        SET_HELP_PAGE("cek-senet-islemleri_cek-senet-entegrasyon-fisleri");
    }

}

/**************************************************************************************
                    CEK_SENET_BORDROSU_FISI::LEAVE_RECORD
***************************************************************************************/

int CEK_SENET_BORDROSU_FISI::LEAVE_RECORD()
{
    if ( m_log_kaydi_eklenmeli_mi EQ true ) {
        DB->START_TRANSACTION();
        if ( m_kayit_eklendi EQ true ) {
            E9_LOG_KAYDI_EKLE ( CEKSENET_MODULU,LOG_CEK_BORDROSU,LOG_ISLEM_ADD,m_log_detaylari );
        }
        if ( m_kayit_silindi EQ true ) {
            E9_LOG_KAYDI_EKLE( CEKSENET_MODULU,LOG_CEK_BORDROSU,LOG_ISLEM_DELETE,m_log_detaylari );
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
             E9_LOG_KAYDI_EKLE ( CEKSENET_MODULU,LOG_CEK_BORDROSU,LOG_ISLEM_UPDATE,m_log_detaylari );
        }
        DB->COMMIT_TRANSACTION();
    }

    return ADAK_OK;
}

