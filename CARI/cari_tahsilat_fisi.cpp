#include <QHeaderView>
#include <QMessageBox>
#include "adak_utils.h"
#include "ui_cari_tahsilat_fisi.h"
#include "cari_tahsilat_fisi_class.h"
#include "cari_tahsilat_fisi_open.h"
#include "adak_sql.h"
#include "print.h"
#include "sube_console_utils.h"
#include "adak_std_utils.h"
#include "cari_enum.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "sube_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "cek_enum.h"
#include "cek_console_utils.h"
#include "cari_makbuz_belgesi_open.h"
 
#include "e9_yetki.h"
#include "e9_log.h"
#include "adak_std_utils.h"
#include "bnk_fis_utils.h"
#include "cek_senet_utils.h"
#include "cek_fis_utils.h"
#include "e9_struct.h"
#include "e9_enum.h"
#include "adres_console_utils.h"
#include "cari_fis_arama_open.h"
#include "cari_tahsilat_ceksenet_batch_open.h"
#include "cari_kart_arama_open.h"
#include "e9_kk_tahsilat_batch_open.h"
#include "e9_eft_batch_open.h"
#include "e9_ent_detaylari_batch_open.h"
#include "dvz_gui_utils.h"
#include "cari_console_utils.h"
#include "sube_struct.h"
#include "fat_console_utils.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "cari_oto_ekstre_arama_open.h"

extern ADAK_SQL * DB;

//-------------hidelanacak columnlar
#define ROW_ID_COLUMN                       0
#define FIS_ID_COLUMN                       1
#define ORDER_COLUMN                        2
#define VADE_TARIHI_COLUMN                  3
#define CEK_SENET_NO_COLUMN                 4
#define PORTFOY_NO_COLUMN                   5
#define VALOR_COLUMN                        6
#define BANKA_KODU_COLUMN                   7
#define SUBE_KODU_COLUMN                    8
#define SUBE_ADI_COLUMN                     9
#define HESAP_NO_COLUMN                     10
#define KESIDECI_COLUMN                     11
#define KESIDE_TARIHI_COLUMN                12
#define FRM_HESAP_NO_ID_COLUMN              13 //Firmaya ait bankanin hesap numarasi idsini saklar
#define POS_ID_COLUMN                       14
#define KK_NUMARASI_COLUMN                  15
#define KK_SAHIBI_COLUMN                    16
//-----------------------------------
#define TAHSILAT_TURU_COLUMN                17
#define ACIKLAMA_COLUMN                     18
#define TUTAR_COLUMN                        19
#define MAKBUZ_PARA_BIRIMI_COLUMN           20
#define TEMEL_PARA_BIRIMI_TUTARI_COLUMN     21
#define CARI_PARA_BIRIMI_TUTARI_COLUMN      22
#define CARI_PARA_BIRIMI_COLUMN             23
#define DETAYLAR_COLUMN                     24

/**************************************************************************************
                   OPEN_CARI_TAHSILAT_FISI
**************************************************************************************/

void OPEN_CARI_TAHSILAT_FISI ( int fis_id , QWidget * parent )
{
    CARI_TAHSILAT_FISI * F = new CARI_TAHSILAT_FISI ( fis_id, parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CARI_TAHSILAT_FISI
**************************************************************************************/

CARI_TAHSILAT_FISI::CARI_TAHSILAT_FISI(int fis_id, QWidget * parent)
:FIS_KERNEL(parent), m_ui ( new Ui::CARI_TAHSILAT_FISI )
{
    m_start_record_id = fis_id;

    m_ui->setupUi    ( this );
    START_FIS_KERNEL ( this, DB);
}
/**************************************************************************************
                   CARI_TAHSILAT_FISI::~CARI_TAHSILAT_FISI
**************************************************************************************/
CARI_TAHSILAT_FISI::~CARI_TAHSILAT_FISI()
{
    delete M_ISLEM_DETAYLARI;
    delete M_FIS;
    delete M_FIS_SATIRI;
    delete M_FIS_DETAYLARI;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::SETUP_FORM
***************************************************************************************/

void CARI_TAHSILAT_FISI::SETUP_FORM()
{
    SET_PAGE_TITLE( tr ( "CARİ - TAHSİLAT FİŞİ" ) );

    REGISTER_TABLE_WIDGET     ( m_ui->tablewidget_fis_satirlari , 25 );
    REGISTER_BUTTONS_WIDGET   ( m_ui->navigation_button_widget );

    M_FIS                =   new CARI_FIS_STRUCT;
    M_FIS_SATIRI         =   new CARI_FIS_SATIRI_STRUCT;
    M_FIS_DETAYLARI      =   new E9_ENT_DETAYLARI_STRUCT;
    M_ISLEM_DETAYLARI    =   new E9_ISLEM_DETAYLARI;

    m_ui->label_iptal->setVisible( false );

    m_ui->lineedit_fis_no->setText("*");
    DISABLE_CLEAR_ON_WIDGET( m_ui->lineedit_fis_no );
    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_sirket );
    m_ui->lineedit_fis_no->setMaxLength(10);

    m_ui->lineEdit_belge_numarasi->setMaxLength ( 6 );
    m_ui->lineEdit_belge_seri->setMaxLength(3);

    m_satirdaki_islem_turu_degisti = false;

    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    if( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ HAYIR ){
        m_ui->pushButton_detaylar->setVisible( false );
    }
    if( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ){
        m_ui->comboBox_fis_filtresi->setVisible( false );
        m_ui->label_fis_filtresi->setVisible( false );

        m_ui->comboBox_sirket->setVisible( false );
        m_ui->label_sirket->setVisible( false );
    }

    SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;

    SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

    m_ui->lineEdit_belge_seri->setText( UNITE_BILG.tahsilat_makbuzu_belge_seri );

    DISABLE_CLEAR_ON_WIDGET ( m_ui->lineEdit_belge_seri );

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH ( 512 );

    SET_HELP_PAGE     ( "cari-hesap-islemleri_tahsilat-fisi" );

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_fis_no->setValidator(validator);

    m_temel_para_birimi_id   = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

    m_ui->commaEdit_parite->SET_PRECISION(6);
    m_ui->commaEdit_kur->SET_PRECISION(4);

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );

    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN  );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN       ( 17 );

    SET_FIS_ORDER_DATABASE ( "car_fis_satirlari", "order_number", "fis_satiri_id" );

    SET_TABLE_ROW_WIDGETS ( VADE_TARIHI_COLUMN              , WIDGET_ADAK_DATE );
    SET_TABLE_ROW_WIDGETS ( CEK_SENET_NO_COLUMN             , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( PORTFOY_NO_COLUMN               , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( VALOR_COLUMN                    , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( BANKA_KODU_COLUMN               , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( SUBE_KODU_COLUMN                , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( SUBE_ADI_COLUMN                 , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( HESAP_NO_COLUMN                 , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( KESIDECI_COLUMN                 , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( KESIDE_TARIHI_COLUMN            , WIDGET_ADAK_DATE );
    SET_TABLE_ROW_WIDGETS ( TAHSILAT_TURU_COLUMN            , WIDGET_COMBO_BOX  );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN                 , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( TUTAR_COLUMN                    , WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( DETAYLAR_COLUMN                 , WIDGET_PUSH_BUTTON);
    SET_TABLE_ROW_WIDGETS ( FRM_HESAP_NO_ID_COLUMN          , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( POS_ID_COLUMN                   , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( KK_NUMARASI_COLUMN              , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( KK_SAHIBI_COLUMN                , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( TEMEL_PARA_BIRIMI_TUTARI_COLUMN , WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( CARI_PARA_BIRIMI_TUTARI_COLUMN  , WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( CARI_PARA_BIRIMI_COLUMN         , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( MAKBUZ_PARA_BIRIMI_COLUMN       , WIDGET_LINE_EDIT  );


    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_fis_no );

    REGISTER_SAVER_BUTTON   ( m_ui->push_button_tahsilat_makbuzu_yazdir );

    REGISTER_CHANGER_BUTTON ( m_ui->pushButton_detaylar );

    SET_LINE_BUTTON_ICON ( QStringList() << "" );

    DVZ_FILL_COMBOBOX(m_ui->comboBox_makbuz_para_birimi);
    m_ui->comboBox_makbuz_para_birimi->setCurrentIndex(-1);

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH( 512 );
    m_ui->lineedit_cari_hesap_ismi->setMaxLength( 40 );
    m_ui->lineEdit_cari_para_birimi->setMaxLength( 5 );
    m_ui->searchedit_cari_hesap_kodu->SET_MAX_LENGTH( 15 );

    m_ui->tablewidget_fis_satirlari->hideColumn ( TEMEL_PARA_BIRIMI_TUTARI_COLUMN );

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels(QStringList()<<"id"<<"fis_id"<<"satir_order"
                                                         << "" << "" << "" << ""
                                                         << "" << "" << "" << "" << "" << "" <<"" << "" << "" << ""
                                                         <<tr("Tahsilat Şekli")
                                                         <<tr("Açıklama")<<tr("Tutarı") << tr("")
                                                         <<("temel_para_birimi_tutari")
                                                         <<("Cari Para Birimi\n Tutarı")
                                                         <<tr("") << tr("")<<tr("")<<tr("") );


    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->horizontalHeaderItem(CARI_PARA_BIRIMI_TUTARI_COLUMN);
    QFont font         = item->font();
    font.setPointSize ( font.pointSize() - 2 );
    item->setFont     ( font );

    m_ui->tablewidget_fis_satirlari->setColumnWidth     ( TAHSILAT_TURU_COLUMN               , 132 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth     ( ACIKLAMA_COLUMN                    , 565 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth     ( TUTAR_COLUMN                       , 124 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth     ( DETAYLAR_COLUMN                    , 130 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth     ( CARI_PARA_BIRIMI_TUTARI_COLUMN     , 124 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth     ( CARI_PARA_BIRIMI_COLUMN            , 40  );
    m_ui->tablewidget_fis_satirlari->setColumnWidth     ( MAKBUZ_PARA_BIRIMI_COLUMN          , 40  );

    SET_SETTING_NAME                                    ( "CARI_TAHSILAT_FISI" );

    m_ui->tablewidget_fis_satirlari->setSelectionMode   ( QAbstractItemView::NoSelection );

    FOCUS_FIRST_WIDGET                                  ();
    AUTOSTART_FIS_KERNEL                                ( m_start_record_id );
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void CARI_TAHSILAT_FISI::CLEAR_FORM_MEMBERS ()
{
    CARI_CLEAR_FIS_STRUCT(M_FIS);

    CARI_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    M_FIS->modul_id                             = CARI_MODULU;
    M_FIS->program_id                           = E9_PROGRAMI;

    m_makbuz_para_birimi_id                     = 0;
    m_cari_para_birimi_id                       = 0;
    m_cari_hesap_id                             = 0;

    m_log_kaydi_eklenecek_mi                    = false;
    m_kayit_eklendi                             = false;
    m_kayit_silindi                             = false;

    M_FIS_DETAYLARI->muh_fis_no             =   0;
    M_FIS_DETAYLARI->banka_fis_no.clear();
    M_FIS_DETAYLARI->cari_fis_no            =   0;
    M_FIS_DETAYLARI->cek_fis_no             =   0;
    M_FIS_DETAYLARI->senet_fis_no           =   0;
    M_FIS_DETAYLARI->firma_cek_fis_no       =   0;
    M_FIS_DETAYLARI->firma_senet_fis_no     =   0;
    M_FIS_DETAYLARI->muh_kasa_hesap_id      =   0;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::NEW_FIS_RECORD
**************************************************************************************/

void CARI_TAHSILAT_FISI::NEW_FIS_RECORD()
{

    m_ui->label_temel_para_birimi->setText      ( DVZ_GET_TEMEL_PARA_BIRIMI_KODU() );
    m_ui->lineedit_fis_no->setText("*");
    m_ui->label_iptal->setVisible( false );

    m_ui->label_ortalama_gun->setHidden             ( true  );
    m_ui->label_ortalama_vade->setHidden            ( true  );
    m_ui->line_edit_ortalama_gun->setHidden         ( true  );
    m_ui->adakDate_ortalama_gun_tarih->setHidden   ( true  );
    m_ui->label_temel_para_birimi->setHidden        ( true  );
    m_ui->commaEdit_kur->setHidden                  ( true  );
    m_ui->label_cari_para_birimi->setHidden         ( true  );
    m_ui->label_makbuz_para_birimi->setHidden       ( true  );
    m_ui->commaEdit_parite->setHidden               ( true  );
    m_ui->label_parite->setHidden                   ( true  );

    M_FIS->muh_kasa_hesap_id = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();
    M_FIS_DETAYLARI->muh_kasa_hesap_id = M_FIS->muh_kasa_hesap_id;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::GET_FIS_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::GET_FIS_RECORD ( int fis_id )
{
    CARI_FIS_BILGILERINI_OKU( M_FIS, fis_id );

    m_cari_hesap_id = M_FIS->cari_hesap_id;

    m_ui->label_iptal->setVisible( false );
    if ( M_FIS->iptal_mi EQ 1 ) {
        m_ui->label_iptal->setVisible( true );
    }

    SQL_QUERY select_query ( DB );

    m_ui->lineedit_fis_no->setText                  ( QVariant ( M_FIS->fis_no ).toString() );
    m_ui->adakDate_fis_tarihi->SET_DATE(  M_FIS->fis_tarihi  );
    m_ui->limitedTextEdit_aciklama->setPlainText    ( M_FIS->aciklama       );
    m_ui->commaedit_toplam_tutar->SET_DOUBLE        ( M_FIS->toplam_alacak  );
    m_ui->lineEdit_belge_seri->setText              ( M_FIS->belge_seri );
    m_ui->lineEdit_belge_numarasi->setText          ( M_FIS->belge_numarasi   );

    M_FIS_DETAYLARI->muh_kasa_hesap_id = M_FIS->muh_kasa_hesap_id;

    QString makbuz_doviz_kodu = DVZ_GET_DOVIZ_KODU ( M_FIS->makbuz_para_birimi_id );
    m_makbuz_para_birimi_id   = M_FIS->makbuz_para_birimi_id;

    m_ui->comboBox_makbuz_para_birimi->setCurrentIndex  ( m_ui->comboBox_makbuz_para_birimi->findText(makbuz_doviz_kodu));

    m_ui->commaEdit_kur->SET_DOUBLE                     ( M_FIS->doviz_kuru );
    m_ui->commaEdit_parite->SET_DOUBLE                  ( M_FIS->parite     );

    m_ui->label_makbuz_para_birimi->setText ( "1 " + makbuz_doviz_kodu );

    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT ( "car_hesaplar",
                               "cari_hesap_kodu, adres_id, para_birimi_id,"
                               "toplam_borc_array,toplam_alacak_array",
                               "cari_hesap_id = :cari_hesap_id");

    sql_query.SET_VALUE(":cari_hesap_id" , M_FIS->cari_hesap_id );

    QString cari_para_birimi_kodu;

    if ( sql_query.SELECT() > 0 ) {

        sql_query.NEXT();

        m_ui->searchedit_cari_hesap_kodu->SET_TEXT ( sql_query.VALUE(0).toString() );
        m_ui->lineedit_cari_hesap_ismi->setText    ( ADR_GET_FIRMA_SAHIS_ADI(sql_query.VALUE(1).toInt()) );
        m_cari_para_birimi_id = sql_query.VALUE(2).toInt();
        cari_para_birimi_kodu = DVZ_GET_DOVIZ_KODU  ( m_cari_para_birimi_id );
        m_ui->lineEdit_cari_para_birimi->setText    ( cari_para_birimi_kodu );
        M_FIS->cari_para_birimi_id = m_cari_para_birimi_id;


        double borc_array[E9_ARRAY_SIZE];
        double alacak_array [E9_ARRAY_SIZE];

        UNPACK_DOUBLE_ARRAY(sql_query.VALUE(3).toString() , borc_array   , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(sql_query.VALUE(4).toString() , alacak_array , E9_ARRAY_SIZE);

        double hesap_borc   = borc_array[E9_SON_BAKIYE];
        double hesap_alacak = alacak_array[E9_SON_BAKIYE];

        double borc_bakiye   = 0.0;
        double alacak_bakiye = 0.0;

        if ( hesap_borc > hesap_alacak ) {
            borc_bakiye = hesap_borc - hesap_alacak;
        }
        else {
            alacak_bakiye = hesap_alacak - hesap_borc;
        }
        m_ui->commaEdit_toplam_borc->SET_DOUBLE(borc_bakiye);
        m_ui->commaEdit_toplam_alacak->SET_DOUBLE(alacak_bakiye);

    }

     m_ui->label_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi->text());

    if ( m_makbuz_para_birimi_id EQ m_cari_para_birimi_id  ) {

        if ( m_makbuz_para_birimi_id EQ m_temel_para_birimi_id ) {

            m_ui->commaEdit_kur->clear();
            m_ui->commaEdit_parite->clear();
        }
        else {
            m_ui->label_temel_para_birimi->setHidden    ( false );
            m_ui->commaEdit_kur->setHidden              ( false );
            m_ui->label_temel_para_birimi->setText      ( DVZ_GET_TEMEL_PARA_BIRIMI_KODU() );
            m_ui->commaEdit_parite->clear();
        }
    }
    else {

        m_ui->commaEdit_kur->setHidden(false);
        m_ui->label_temel_para_birimi->setHidden(false);

        if ( m_makbuz_para_birimi_id EQ m_temel_para_birimi_id ) {
            m_ui->label_parite->setHidden               ( false );
            m_ui->commaEdit_parite->setHidden           ( false );
            m_ui->label_makbuz_para_birimi->setHidden   ( false );
            m_ui->label_cari_para_birimi->setHidden     ( false );
            m_ui->commaEdit_kur->clear();
            m_ui->commaEdit_kur->setHidden(true);
            m_ui->label_temel_para_birimi->setHidden(true);
        }
        else {

            if ( m_cari_para_birimi_id NE m_temel_para_birimi_id ) {
                m_ui->label_cari_para_birimi->setText       ( m_ui->lineEdit_cari_para_birimi->text() );
                m_ui->label_makbuz_para_birimi->setText     ( "1 " + DVZ_GET_DOVIZ_KODU ( m_makbuz_para_birimi_id ) );
                m_ui->commaEdit_parite->setHidden           ( false );
                m_ui->label_parite->setHidden               ( false );
                m_ui->label_cari_para_birimi->setHidden     ( false );
                m_ui->label_makbuz_para_birimi->setHidden   ( false );
            }
        }
    }


    select_query.PREPARE_SELECT("car_fis_satirlari",
                                "fis_satiri_id, aciklama, alacak_tutari, islem_turu, cek_senet_id, "
                                "hesap_no_id , pos_id , kredi_karti_numarasi, kredi_karti_sahibi, "
                                "cari_para_birimi_tutari",
                                "fis_id = :fis_id " );
    select_query.SET_VALUE(":fis_id" , fis_id);

    if ( select_query.SELECT( "order_number ASC" ) EQ 0 ) {
        return ADAK_OK;
    }

    int                current_row;
    QTableWidgetItem * new_item;

    while ( select_query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE();

        new_item = new QTableWidgetItem(select_query.VALUE(0).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row,ROW_ID_COLUMN,new_item);

        new_item = new QTableWidgetItem(QVariant(fis_id).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row,FIS_ID_COLUMN,new_item);

        QLineEdit  * lineedit_aciklama                  = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, ACIKLAMA_COLUMN                   );
        QCommaEdit * commaedit_tahsilat_tutari          = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, TUTAR_COLUMN                      );
        QComboBox  * combobox_tahsilat_sekli            = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, TAHSILAT_TURU_COLUMN              );
        QLineEdit  * line_edit_frm_hesap_no_id          = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, FRM_HESAP_NO_ID_COLUMN);
        QLineEdit  * lineedit_pos_id                    = ( QLineEdit *  ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , POS_ID_COLUMN);
        QLineEdit  * lineedit_kredi_karti_numarasi      = ( QLineEdit *  ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , KK_NUMARASI_COLUMN);
        QLineEdit  * lineedit_kredi_karti_sahibi        = ( QLineEdit *  ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , KK_SAHIBI_COLUMN);
        QPushButton * push_button                       = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, DETAYLAR_COLUMN      );
        QCommaEdit  * commaedit_cari_para_birimi_tutari = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row, CARI_PARA_BIRIMI_TUTARI_COLUMN);
        QLineEdit  *  lineedit_cari_para_birimi         = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , CARI_PARA_BIRIMI_COLUMN);
        QLineEdit  *  lineedit_makbuz_para_birimi       = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , MAKBUZ_PARA_BIRIMI_COLUMN);

        lineedit_cari_para_birimi->setText(cari_para_birimi_kodu);
        lineedit_makbuz_para_birimi->setText(makbuz_doviz_kodu);


        lineedit_aciklama->setText(select_query.VALUE(1).toString());

        commaedit_tahsilat_tutari->SET_DOUBLE           ( select_query.VALUE(2).toDouble());
        combobox_tahsilat_sekli->setCurrentIndex        ( combobox_tahsilat_sekli->findText ( E9_GET_TAH_ODM_SEKLI_STRING(select_query.VALUE(3).toInt(),ENUM_CARI_TAHSILAT_FISI ) ) );

        line_edit_frm_hesap_no_id->setText              ( select_query.VALUE(5).toString());
        lineedit_pos_id->setText                        ( select_query.VALUE(6).toString());
        lineedit_kredi_karti_numarasi->setText          ( select_query.VALUE(7).toString());
        lineedit_kredi_karti_sahibi->setText            ( select_query.VALUE(8).toString());
        commaedit_cari_para_birimi_tutari->SET_DOUBLE   ( select_query.VALUE(9).toDouble());     

        int cek_senet_id = select_query.VALUE(4).toInt();

        int islem_turu = select_query.VALUE(3).toInt();

        combobox_tahsilat_sekli->setCurrentIndex(combobox_tahsilat_sekli->findText(E9_GET_TAH_ODM_SEKLI_STRING(islem_turu,ENUM_CARI_TAHSILAT_FISI)));

        push_button->setEnabled(true);

        if ( islem_turu EQ ENUM_NAKIT ) {
            if ( push_button NE NULL ) {
                push_button->setDisabled(true);
            }
        }

        if ( islem_turu EQ ENUM_KREDI_KARTI ) {
            push_button->setText("Kredi Kartı Detayları");
        }
        if (islem_turu EQ ENUM_HAVALE ) {
            push_button->setText("Havale");
        }
        if (islem_turu EQ ENUM_EFT ) {
            push_button->setText("Eft");
        }

        if ( cek_senet_id NE 0 ) {

            CEK_SENET_STRUCT CEK_BILG;

            CEK_GET_TEMEL_BILGILER( &CEK_BILG, cek_senet_id );

            QLineEdit   * lineedit_portfoy_no           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, PORTFOY_NO_COLUMN    );
            QAdakDate  * adakDate_vade_tarihi         = ( QAdakDate  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, VADE_TARIHI_COLUMN   );
            QLineEdit   * lineedit_valor                = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, VALOR_COLUMN         );
            QLineEdit   * lineedit_cek_senet_no         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, CEK_SENET_NO_COLUMN  );
            QLineEdit   * line_edit_banka_ismi          = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, BANKA_KODU_COLUMN    );
            QLineEdit   * line_edit_sube_kodu           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, SUBE_KODU_COLUMN     );
            QLineEdit   * line_edit_sube_adi            = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, SUBE_ADI_COLUMN      );
            QLineEdit   * line_edit_hesap_no            = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, HESAP_NO_COLUMN      );
            QAdakDate  * date_edit_keside_tarihi       = ( QAdakDate  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, KESIDE_TARIHI_COLUMN );
            QLineEdit   * line_edit_kesideci            = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, KESIDECI_COLUMN      );
            QLineEdit   * lineedit_makbuz_para_birimi   = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MAKBUZ_PARA_BIRIMI_COLUMN);

            lineedit_portfoy_no->setText        ( QVariant ( CEK_BILG.portfoy_numarasi).toString() );
            adakDate_vade_tarihi->SET_DATE     ( CEK_BILG.vade_tarihi );
            lineedit_valor->setText             ( QVariant ( CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI( adakDate_vade_tarihi->QDATE()) ).toString() );
            lineedit_cek_senet_no->setText      ( QVariant ( CEK_BILG.cek_senet_numarasi ).toString() );
            line_edit_banka_ismi->setText       ( CEK_BILG.banka_ismi );
            line_edit_sube_kodu->setText        ( CEK_BILG.sube_kodu );
            line_edit_sube_adi->setText         ( CEK_BILG.sube_ismi );
            line_edit_hesap_no->setText         ( CEK_BILG.musteri_hesap_numarasi );
            date_edit_keside_tarihi->SET_DATE   ( CEK_BILG.keside_tarihi );
            line_edit_kesideci->setText         ( CEK_BILG.borclu_kesideci );
            push_button->setEnabled             ( true );
            lineedit_makbuz_para_birimi->setText(DVZ_GET_DOVIZ_KODU( CEK_BILG.doviz_id ));

            int tahsilat_sekli = select_query.VALUE(3).toInt();
            if ( tahsilat_sekli EQ ENUM_MUSTERI_CEKI ) {
                push_button->setText("Çek Detayları");
            }
            else {
                push_button->setText("Senet Detayları");
            }

        }
    }


    int ortalama_gun = CEK_ORTALAMA_GUN_SAYISI( fis_id, ENUM_CARI_TAHSILAT_FISI );

    if ( ortalama_gun > 0 ) {
        m_ui->label_ortalama_gun->setHidden(false);
        m_ui->label_ortalama_vade->setHidden(false);
        m_ui->line_edit_ortalama_gun->setHidden(false);
        m_ui->adakDate_ortalama_gun_tarih->setHidden(false);
        m_ui->line_edit_ortalama_gun->setText( QVariant(ortalama_gun).toString() );
        m_ui->adakDate_ortalama_gun_tarih->SET_DATE( QDate::currentDate().addDays ( ortalama_gun ) );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::SET_LINE_DEFAULTS
**************************************************************************************/

void CARI_TAHSILAT_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QComboBox  * combobox_tahsilat_sekli            = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,TAHSILAT_TURU_COLUMN);
    QLineEdit  * lineedit_portfoy_no                = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,PORTFOY_NO_COLUMN);
    QLineEdit  * lineedit_gun                       = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VALOR_COLUMN);
    QAdakDate * adakDate_vade_tarihi              = ( QAdakDate * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,VADE_TARIHI_COLUMN);
    QLineEdit  * lineedit_cek_senet_no              = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CEK_SENET_NO_COLUMN);
    QLineEdit  * lineedit_aciklama                  = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ACIKLAMA_COLUMN);
    QCommaEdit * commaedit_cari_para_birimi_tutari  = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CARI_PARA_BIRIMI_TUTARI_COLUMN);
    QLineEdit  * lineedit_cari_para_birimi          = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CARI_PARA_BIRIMI_COLUMN);
    QLineEdit  * lineedit_makbuz_para_birimi        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MAKBUZ_PARA_BIRIMI_COLUMN);
    //LineEdit  * lineedit_hesap_no_id               = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,FRM_HESAP_NO_ID_COLUMN);

    lineedit_cari_para_birimi->setReadOnly(true);
    lineedit_cari_para_birimi->setMaxLength( 5 );

    lineedit_makbuz_para_birimi->setMaxLength( 5 );


    lineedit_makbuz_para_birimi->setReadOnly(true);

    commaedit_cari_para_birimi_tutari->setDisabled(true);

    combobox_tahsilat_sekli->addItems(QStringList()<<tr("Nakit")<<tr("Çek")<<tr("Senet")
                                      <<tr ("Kredi Kartı") << tr("Havale") << tr("Eft"));
    combobox_tahsilat_sekli->setCurrentIndex(-1);

    lineedit_portfoy_no->setReadOnly(true);
    lineedit_portfoy_no->setText("*");

    lineedit_gun->setReadOnly(true);

    adakDate_vade_tarihi->setDisabled(true);

    lineedit_cek_senet_no->setReadOnly(true);
    lineedit_aciklama->setMaxLength(128);

    QPushButton * push_button = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,DETAYLAR_COLUMN );
    push_button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    push_button->setMinimumWidth(130);
    push_button->setText ( tr("") );
    push_button->setEnabled ( false );


    REGISTER_LINE_CHANGER_BUTTON(row_number,DETAYLAR_COLUMN);
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_FIS_FORM_VAR
**************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{

    if ( object EQ m_ui->searchedit_cari_hesap_kodu ) {

        SQL_QUERY sql_query ( DB );

        if ( m_ui->searchedit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_cari_hesap_id = 0;
            m_ui->lineedit_cari_hesap_ismi->clear();
            m_ui->commaEdit_toplam_alacak->clear();
            m_ui->commaEdit_toplam_borc->clear();

            return ADAK_OK;
        }
        double borc_array[E9_ARRAY_SIZE];
        double alacak_array[E9_ARRAY_SIZE];


        sql_query.PREPARE_SELECT ( "car_hesaplar",
                                   "cari_hesap_id , adres_id,para_birimi_id,"
                                   "toplam_borc_array , toplam_alacak_array ",
                                   "cari_hesap_kodu = :cari_hesap_kodu");
        sql_query.SET_VALUE      ( ":cari_hesap_kodu" , m_ui->searchedit_cari_hesap_kodu->GET_TEXT() );

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr ( "Aradığınız cari hesap kodu bulunamadı" ), m_ui->searchedit_cari_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }
        sql_query.NEXT();
        m_cari_hesap_id   = sql_query.VALUE(0).toInt();
        m_ui->lineedit_cari_hesap_ismi->setText (ADR_GET_FIRMA_SAHIS_ADI(sql_query.VALUE(1).toInt()) );
        m_cari_para_birimi_id  = sql_query.VALUE(2).toInt();

        UNPACK_DOUBLE_ARRAY(sql_query.VALUE(3).toString() , borc_array   , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(sql_query.VALUE(4).toString() , alacak_array , E9_ARRAY_SIZE);

        double hesap_borc   = borc_array[E9_SON_BAKIYE];
        double hesap_alacak = alacak_array[E9_SON_BAKIYE];

        double borc_bakiye   = 0.0;
        double alacak_bakiye = 0.0;

        if ( hesap_borc > hesap_alacak ) {
            borc_bakiye = hesap_borc - hesap_alacak;
        }
        else {
            alacak_bakiye = hesap_alacak - hesap_borc;
        }
        m_ui->commaEdit_toplam_borc->SET_DOUBLE(borc_bakiye);
        m_ui->commaEdit_toplam_alacak->SET_DOUBLE(alacak_bakiye);

        QString cari_para_birimi_kodu = DVZ_GET_DOVIZ_KODU(m_cari_para_birimi_id);
        m_ui->lineEdit_cari_para_birimi->setText( cari_para_birimi_kodu);

        if ( m_ui->comboBox_makbuz_para_birimi->currentIndex() EQ -1 )  {
            m_ui->comboBox_makbuz_para_birimi->setCurrentIndex(m_ui->comboBox_makbuz_para_birimi->findText(cari_para_birimi_kodu));
            m_makbuz_para_birimi_id = m_cari_para_birimi_id;

            if ( m_cari_para_birimi_id NE m_temel_para_birimi_id ) {
                m_ui->label_temel_para_birimi->setHidden(false);
                m_ui->commaEdit_kur->setHidden(false);

                m_ui->commaEdit_kur->SET_DOUBLE(DVZ_DOVIZ_KURUNU_AL(m_makbuz_para_birimi_id,m_ui->adakDate_fis_tarihi->QDATE(),
                                                                ENUM_ALIS));
            }
        }
        else {
            m_makbuz_para_birimi_id = DVZ_GET_DOVIZ_ID(m_ui->comboBox_makbuz_para_birimi->currentText());
        }


        if ( ( DVZ_GET_DOVIZ_ID(m_ui->comboBox_makbuz_para_birimi->currentText()) NE m_cari_para_birimi_id ) AND
             ( m_cari_para_birimi_id NE m_temel_para_birimi_id) ) {
            m_ui->commaEdit_parite->setHidden(false);
            m_ui->label_cari_para_birimi->setHidden(false);
            m_ui->label_makbuz_para_birimi->setHidden(false);
            m_ui->commaEdit_parite->SET_DOUBLE(DVZ_PARITEYI_AL(m_cari_para_birimi_id,m_makbuz_para_birimi_id,
                                                           m_ui->adakDate_fis_tarihi->QDATE(),ENUM_ALIS));

            m_ui->label_makbuz_para_birimi->setText( "1 " +  m_ui->comboBox_makbuz_para_birimi->currentText());
            m_ui->label_cari_para_birimi->setText(DVZ_GET_DOVIZ_KODU(m_cari_para_birimi_id));
        }
        else {
            m_ui->label_makbuz_para_birimi->setHidden(true);
            m_ui->label_cari_para_birimi->setHidden(true);
            m_ui->commaEdit_parite->setHidden(true);
            m_ui->label_parite->setHidden(true);
        }

        EKRANI_GUNCELLE();

    }
    else if ( object EQ m_ui->comboBox_makbuz_para_birimi ) {


        SQL_QUERY sql_query ( DB );

        int fis_id = GET_RECORD_ID();

        if ( fis_id NE -1 ) {
            sql_query.PREPARE_SELECT ( "car_fis_satirlari","fis_satiri_id",
                                       "fis_id = :fis_id" );

            sql_query.SET_VALUE      ( ":fis_id", fis_id );

            if ( sql_query.SELECT() NE 0 ) {
                MSG_WARNING( tr ("Fiş satırlarında işlem bulunmaktadır.Makbuz para birimini değiştiremezsiniz."), m_ui->comboBox_makbuz_para_birimi );
                return ADAK_FAIL_UNDO;
            }
        }
        m_makbuz_para_birimi_id = DVZ_GET_DOVIZ_ID ( m_ui->comboBox_makbuz_para_birimi->currentText() );

        m_ui->commaEdit_kur->SET_DOUBLE(DVZ_DOVIZ_KURUNU_AL(m_makbuz_para_birimi_id,m_ui->adakDate_fis_tarihi->QDATE(),ENUM_ALIS));

        if ( m_cari_para_birimi_id EQ m_makbuz_para_birimi_id ) {
            if ( m_makbuz_para_birimi_id EQ m_temel_para_birimi_id ) {
                //tum widgetlar hidelanacak
                m_ui->commaEdit_kur->setHidden(true);
                m_ui->label_temel_para_birimi->setHidden(true);
                m_ui->label_parite->setHidden(true);
                m_ui->commaEdit_parite->setHidden(true);
                m_ui->label_cari_para_birimi->setHidden(true);
                m_ui->label_makbuz_para_birimi->setHidden(true);
                m_ui->commaEdit_kur->clear();
                m_ui->commaEdit_parite->clear();

            }
            else {
                //kur gorunecek
                m_ui->commaEdit_kur->setHidden(false);
                m_ui->label_temel_para_birimi->setHidden(false);
                m_ui->label_parite->setHidden(true);
                m_ui->commaEdit_parite->setHidden(true);
                m_ui->label_cari_para_birimi->setHidden(true);
                m_ui->label_makbuz_para_birimi->setHidden(true);
                m_ui->commaEdit_parite->clear();
            }
        }
        else {
            //Makbuz para birimi ile cari para birimi farkli ise

            if ( m_makbuz_para_birimi_id EQ m_temel_para_birimi_id ) {
               //sadece parite gorunecek
                m_ui->commaEdit_parite->SET_DOUBLE(DVZ_PARITEYI_AL(m_cari_para_birimi_id,m_makbuz_para_birimi_id,
                                                               m_ui->adakDate_fis_tarihi->QDATE(),ENUM_ALIS));
                m_ui->label_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi->text());
                m_ui->label_makbuz_para_birimi->setText("1 " + DVZ_GET_DOVIZ_KODU(m_makbuz_para_birimi_id));
                m_ui->commaEdit_parite->setHidden(false);
                m_ui->label_parite->setHidden(false);
                m_ui->label_cari_para_birimi->setHidden(false);
                m_ui->label_makbuz_para_birimi->setHidden(false);
                m_ui->label_temel_para_birimi->setHidden(true);
                m_ui->commaEdit_kur->setHidden(true);
                m_ui->commaEdit_kur->clear();
            }
            else {
                //parite ve kur gorunecek
                m_ui->commaEdit_kur->setHidden(false);
                m_ui->label_temel_para_birimi->setHidden(false);

                if ( m_cari_para_birimi_id NE m_temel_para_birimi_id ) {

                    m_ui->commaEdit_parite->SET_DOUBLE(DVZ_PARITEYI_AL(m_cari_para_birimi_id,m_makbuz_para_birimi_id,
                                                                   m_ui->adakDate_fis_tarihi->QDATE(),ENUM_ALIS));
                    m_ui->label_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi->text());
                    m_ui->label_makbuz_para_birimi->setText("1 " + DVZ_GET_DOVIZ_KODU(m_makbuz_para_birimi_id));
                    m_ui->commaEdit_parite->setHidden(false);
                    m_ui->label_parite->setHidden(false);
                    m_ui->label_cari_para_birimi->setHidden(false);
                    m_ui->label_makbuz_para_birimi->setHidden(false);
                }
                else {
                    m_ui->label_parite->setHidden(true);
                    m_ui->commaEdit_parite->setHidden(true);
                    m_ui->label_cari_para_birimi->setHidden(true);
                    m_ui->label_makbuz_para_birimi->setHidden(true);
                    m_ui->commaEdit_parite->clear();
                }
            }
        }

        EKRANI_GUNCELLE();
    }
    else if ( object EQ m_ui->commaEdit_kur ) {
        EKRANI_GUNCELLE();
    }
    else if ( object EQ m_ui->lineEdit_belge_seri OR object EQ m_ui->lineEdit_belge_numarasi ) {
        m_ui->lineEdit_belge_seri->setText( m_ui->lineEdit_belge_seri->text().toUpper() );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineedit_fis_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Yeni Fiş Kaydı Yapabilmek için Fiş No değeri olarak (*) girmelisiniz!.."), m_ui->lineedit_fis_no );

        return ADAK_FAIL;
    }
    if ( m_ui->searchedit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( tr("Cari Hesap Kodunu boş bırakamazsınız!.."), m_ui->searchedit_cari_hesap_kodu );

        return ADAK_FAIL;
    }

    if ( m_ui->searchedit_cari_hesap_kodu->GET_TEXT().isEmpty() NE true ) {
        if( BELGE_SERI_KONTROLU( m_ui->lineEdit_belge_seri->text(), m_ui->lineEdit_belge_numarasi->text(), CARI_MODULU, M_FIS->cari_hesap_id ) EQ false ) {
            int secim = MSG_YES_NO( "Ayni Belge Seri + Belge Numarasi kayıtlı!. Devam Etmek istermisiniz ?", m_ui->lineEdit_belge_seri );
            if( secim EQ ADAK_NO ) {
                return ADAK_FAIL;
            }
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_FIS_RECORD_ADD()
{

    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) NE "*" ) {
        if ( CARI_FIS_NO_KULLANILDI_MI( m_ui->lineedit_fis_no->text().toInt(), m_ui->adakDate_fis_tarihi->DATE() ) EQ true ) {
            MSG_WARNING( tr("Aynı numarada başka bir fiş var.Fiş eklenemedi!.."), m_ui->lineedit_fis_no );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::ADD_FIS_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::ADD_FIS_RECORD()
{
    int fis_no = 0;

    if ( QString( m_ui->lineedit_fis_no->text().at(0) ) EQ "*" ) {
        fis_no  = CARI_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE() );
    }
    else {
        fis_no = m_ui->lineedit_fis_no->text().toInt();
    }

    M_FIS->fis_no                   =   fis_no;
    M_FIS->program_id               =   E9_PROGRAMI;
    M_FIS->modul_id                 =   CARI_MODULU;
    M_FIS->fis_tarihi               =   m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama                 =   m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->fis_turu                 =   ENUM_CARI_TAHSILAT_FISI;
    M_FIS->belge_seri               =   m_ui->lineEdit_belge_seri->text();
    M_FIS->belge_numarasi           =   m_ui->lineEdit_belge_numarasi->text();
    M_FIS->makbuz_para_birimi_id    =   m_makbuz_para_birimi_id;
    M_FIS->doviz_kuru               =   m_ui->commaEdit_kur->GET_DOUBLE();
    M_FIS->parite                   =   m_ui->commaEdit_parite->GET_DOUBLE();
    M_FIS->cari_para_birimi_id      =   m_cari_para_birimi_id;
    M_FIS->cari_hesap_id            =   m_cari_hesap_id;

    int fis_id = CARI_FIS_EKLE ( M_FIS );

    m_ui->lineedit_fis_no->setText ( QVariant ( fis_no ).toString() );

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_eklendi          = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING(ENUM_CARI_TAHSILAT_FISI) + "," +
                        m_ui->lineEdit_belge_seri->text() + "  " +
                        m_ui->lineEdit_belge_numarasi->text() + "\n" +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM yyyy") +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +  ",Fiş No : "  + QVariant ( fis_no ).toString() );


    return fis_id;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_FIS_RECORD_UPDATE(int fis_id)
{
    SQL_QUERY sql_query (DB);

    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) EQ "*" ) {
        MSG_WARNING( tr("Fiş numarasını güncellemek için fiş numarası kısmına sadece sayı girebilirsiniz."), m_ui->lineedit_fis_no );
        return ADAK_FAIL;
    }
    if ( CARI_GET_FIS_YAZDIRILDIMI( fis_id ) EQ ENUM_FIS_YAZILDI ) {
        MSG_WARNING( tr("Fiş Yazdırıldığı için üzerinde Değişiklik yapamazsınız..!"), NULL );
        return ADAK_FAIL;
    }

    int fis_no = m_ui->lineedit_fis_no->text().toInt();

    sql_query.PREPARE_SELECT("car_fisler",
                             "fis_id",
                             "fis_no = :fis_no "
                             "AND fis_tarihi = :fis_tarihi");

    sql_query.SET_VALUE(":fis_no"       ,fis_no );
    sql_query.SET_VALUE(":fis_tarihi"   ,m_ui->adakDate_fis_tarihi->DATE());

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();

        int bulunan_fis_id = sql_query.VALUE(0).toInt();
        //QString fis_tarihi = m_ui->adakDate_fis_tarihi->DATE();

        if ( bulunan_fis_id NE fis_id ) {
            MSG_WARNING( tr("Aynı numarada başka bir fiş var.Güncelleme yapılamadı!.."), m_ui->lineedit_fis_no );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void CARI_TAHSILAT_FISI::UPDATE_FIS_RECORD ( int fis_id )
{

    CARI_FIS_BILGILERINI_OKU ( M_FIS , fis_id );

    M_FIS->fis_no                   =   m_ui->lineedit_fis_no->text().toInt();
    M_FIS->fis_tarihi               =   m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama                 =   m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->toplam_alacak            =   m_ui->commaedit_toplam_tutar->GET_DOUBLE();
    M_FIS->belge_seri               =   m_ui->lineEdit_belge_seri->text();
    M_FIS->belge_numarasi           =   m_ui->lineEdit_belge_numarasi->text();
    M_FIS->makbuz_para_birimi_id    =   m_makbuz_para_birimi_id;
    M_FIS->doviz_kuru               =   m_ui->commaEdit_kur->GET_DOUBLE();
    M_FIS->parite                   =   m_ui->commaEdit_parite->GET_DOUBLE();
    M_FIS->cari_para_birimi_id      =   m_cari_para_birimi_id;
    M_FIS->muh_kasa_hesap_id        =   M_FIS_DETAYLARI->muh_kasa_hesap_id;
    M_FIS->cari_hesap_id            =   m_cari_hesap_id;

    CARI_FISI_GUNCELLE ( M_FIS, fis_id );



    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari =  ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_TAHSILAT_FISI ) + "," +
                         m_ui->lineEdit_belge_seri->text() + " " + m_ui->lineEdit_belge_numarasi->text() + "\n" +
                          m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                          ",Fiş No : " + m_ui->lineedit_fis_no->text() +
                          ",Toplam Alacak : " + VIRGUL_EKLE ( QVariant ( m_ui->commaedit_toplam_tutar->GET_DOUBLE() ).toString()));

    ADD_ADAK_EVENT( ADAK_REFRESH );

}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_FIS_RECORD_DELETE ( int fis_id )
{
    // EGER FIS YAZDIRILMIS ISE SILINEMEZ

    if ( FIS_GUNCELLENEBILIR_MI_SILINEBILIR_MI(fis_id) EQ FALSE ) {
         MSG_WARNING( tr ( "Fiş satırlarındaki müşteri çeki/senedi işlem görmüş,fişi silemezsiniz!.."), NULL );
         return ADAK_FAIL;
    }
    if ( CARI_GET_FIS_YAZDIRILDIMI( fis_id ) EQ ENUM_FIS_YAZILDI ) {
        int secim = MSG_YES_NO( "Bu Makbuz yazdırıldığı için silinmeyecek Sadece Hareketleri Silinecektir. \n Devam Etmek İstermisiniz...", NULL );
        if ( secim NE ADAK_YES ) {
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void CARI_TAHSILAT_FISI::DELETE_FIS_RECORD ( int fis_id )
{
    CARI_FISI_SIL ( M_FIS, fis_id );

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_TAHSILAT_FISI ) + "," +
                        m_ui->lineEdit_belge_seri->text() + " " +
                        m_ui->lineEdit_belge_numarasi->text() + "\n" +
                        m_ui->adakDate_fis_tarihi->DATE() +
                        ",Fiş No : " + QVariant ( m_ui->lineedit_fis_no->text() ).toString() +
                        ",Toplam Alacak : " + VIRGUL_EKLE ( QVariant ( m_ui->commaedit_toplam_tutar->GET_DOUBLE() ).toString() ) );
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::SELECT_FIS_RECORD()
{
    return OPEN_CARI_FIS_ARAMA ( ENUM_CARI_TAHSILAT_FISI, CARI_MODULU, ENUM_CARI_FISI,  this );
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::FIND_FIS_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    QString query_str = "fis_no = :fis_no "
                        "AND fis_turu = :fis_turu AND fis_tarihi = :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );
    
    select_query.SET_VALUE      ( ":fis_no"       , m_ui->lineedit_fis_no->text().toInt() );
    select_query.SET_VALUE      ( ":fis_turu"     , ENUM_CARI_TAHSILAT_FISI );
    select_query.SET_VALUE      ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT() EQ 0 ) {
         QMessageBox::warning ( this,"UYARI",tr ( "Aradığınız Tahsilat Fişi bulunamadı!.." ) );
         SET_FOCUS( m_ui->lineedit_fis_no );
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    QString  query_str = "fis_turu  = :fis_turu ";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE      ( ":fis_turu",    ENUM_CARI_TAHSILAT_FISI );

    if ( select_query.SELECT( "fis_tarihi, fis_no  " , 0, 1 ) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString query_str = "fis_turu  = :fis_turu    ";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE ( ":fis_turu"    , ENUM_CARI_TAHSILAT_FISI   );

    if ( select_query.SELECT( "fis_tarihi DESC, fis_no DESC ", 0, 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::FIND_PREV_FIS_RECORD ()
{
    SQL_QUERY select_query(DB);

    QString  query_str = "fis_no             < :fis_no "
                         "AND fis_turu       = :fis_turu "
                         "AND fis_tarihi     = :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE(":fis_no"       , m_ui->lineedit_fis_no->text().toInt());
    select_query.SET_VALUE(":fis_turu"     , ENUM_CARI_TAHSILAT_FISI     );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_no DESC" , 0, 1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    query_str.clear();
    query_str = "fis_turu           = :fis_turu "
                "AND fis_tarihi     < :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE(":fis_turu"     , ENUM_CARI_TAHSILAT_FISI    );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_tarihi DESC, fis_no DESC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::FIND_NEXT_FIS_RECORD ()
{
    SQL_QUERY select_query(DB);

    QString  query_str = "fis_no             > :fis_no "
                         "AND fis_turu       = :fis_turu "
                         "AND fis_tarihi     = :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE(":fis_no"       , m_ui->lineedit_fis_no->text().toInt());
    select_query.SET_VALUE(":fis_turu"     , ENUM_CARI_TAHSILAT_FISI                      );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_no", 0, 1 ) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    query_str.clear();
    query_str = "fis_turu           = :fis_turu "
                "AND fis_tarihi     > :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE(":fis_turu"     , ENUM_CARI_TAHSILAT_FISI                      );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_tarihi, fis_no", 0, 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_LINE_VAR
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{

    QComboBox   * combobox_tahsilat_sekli             = ( QComboBox   * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TAHSILAT_TURU_COLUMN              );
    QLineEdit   * lineedit_cek_senet_no               = ( QLineEdit   * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CEK_SENET_NO_COLUMN               );
    QAdakDate  * adakDate_vade_tarihi               = ( QAdakDate  * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, VADE_TARIHI_COLUMN                );
    QLineEdit   * lineedit_valor                      = ( QLineEdit   * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, VALOR_COLUMN                      );
    QCommaEdit  * commaedit_tutar                     = ( QCommaEdit  * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TUTAR_COLUMN                      );
    QLineEdit   * lineedit_aciklama                   = ( QLineEdit   * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN                   );
    QPushButton * push_button_detaylar                = ( QPushButton * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DETAYLAR_COLUMN                   );
    QCommaEdit  * commaedit_cari_para_birimi_tutari   = ( QCommaEdit  * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_PARA_BIRIMI_TUTARI_COLUMN    );
    QCommaEdit  * commaedit_temel_para_birimi_tutari  = ( QCommaEdit  * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TEMEL_PARA_BIRIMI_TUTARI_COLUMN   );
    QLineEdit   * lineedit_kesideci                   = ( QLineEdit   * )   m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KESIDECI_COLUMN                   );


    if ( object EQ combobox_tahsilat_sekli ) {

        if ( m_cari_hesap_id EQ 0 ) {
            MSG_WARNING( tr("Önce Cari Hesap kodunu seçmelisiniz."), m_ui->searchedit_cari_hesap_kodu );

            return ADAK_FAIL;
        }


        int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tahsilat_sekli->currentText());

        m_satirdaki_islem_turu_degisti = true;

        switch ( islem_turu ) {
            case ENUM_NAKIT          :
                push_button_detaylar->setText("");

                break;
            case ENUM_MUSTERI_CEKI   :

                m_ui->line_edit_ortalama_gun->clear();
                push_button_detaylar->setText("Çek Detayları");
                lineedit_valor->clear();
                lineedit_cek_senet_no->clear();
                commaedit_cari_para_birimi_tutari->clear();
                break;
            case ENUM_MUSTERI_SENEDI :

                lineedit_valor->clear();
                lineedit_cek_senet_no->clear();

                push_button_detaylar->setText("Senet Detayları");
                commaedit_cari_para_birimi_tutari->clear();
                lineedit_kesideci->clear();
                break;
            case ENUM_KREDI_KARTI :
                push_button_detaylar->setText("Kredi Kartı Detayları");
                break;
            case ENUM_HAVALE      :
                push_button_detaylar->setText("Havale Detayları");
                break;
            case ENUM_EFT :
                push_button_detaylar->setText("EFT Detayları");
                break;

        }

        QLineEdit * lineedit_cari_para_birimi   = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CARI_PARA_BIRIMI_COLUMN);
        QLineEdit * lineedit_makbuz_para_birimi =  ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MAKBUZ_PARA_BIRIMI_COLUMN);

        lineedit_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi->text());
        lineedit_makbuz_para_birimi->setText(m_ui->comboBox_makbuz_para_birimi->currentText());

        QPushButton * push_button = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,DETAYLAR_COLUMN );

        if (islem_turu EQ ENUM_NAKIT) {
            push_button->setDisabled(true);
        }
        else {
            push_button->setEnabled(true);
        }

        M_ISLEM_DETAYLARI->banka_ismi.clear();
        M_ISLEM_DETAYLARI->bnk_hesap_no_id = 0;
        M_ISLEM_DETAYLARI->cek_senet_no.clear();
        M_ISLEM_DETAYLARI->hesap_no.clear();
        M_ISLEM_DETAYLARI->kesideci.clear();
        M_ISLEM_DETAYLARI->keside_tarihi = QDate::currentDate();
        M_ISLEM_DETAYLARI->kredi_karti_id = 0;
        M_ISLEM_DETAYLARI->kredi_karti_numarasi.clear();
        M_ISLEM_DETAYLARI->kredi_karti_sahibi.clear();
        M_ISLEM_DETAYLARI->portfoy_no.clear();
        M_ISLEM_DETAYLARI->pos_id = 0;
        M_ISLEM_DETAYLARI->sube_adi.clear();
        M_ISLEM_DETAYLARI->sube_kodu.clear();
        M_ISLEM_DETAYLARI->vade_tarihi = QDate::currentDate();
        M_ISLEM_DETAYLARI->valor.clear();

        QLineEdit * lineedit_cek_senet_no   = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CEK_SENET_NO_COLUMN     );
        QLineEdit * line_edit_banka_ismi    = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BANKA_KODU_COLUMN       );
        QLineEdit * line_edit_sube_adi      = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_ADI_COLUMN         );
        QLineEdit * line_edit_hesap_no      = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_NO_COLUMN         );
        QLineEdit * line_edit_kesideci      = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KESIDECI_COLUMN         );
        QLineEdit * line_edit_frm_hesap_no_id = (QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,FRM_HESAP_NO_ID_COLUMN  );
        QLineEdit * lineedit_pos_id         = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget  ( row_number,POS_ID_COLUMN);

        lineedit_cek_senet_no->clear();
        line_edit_banka_ismi->clear();
        line_edit_sube_adi->clear();
        line_edit_hesap_no->clear();
        line_edit_kesideci->clear();
        line_edit_frm_hesap_no_id->clear();
        lineedit_pos_id->clear();
    }

    else if ( object EQ adakDate_vade_tarihi ) {
        lineedit_valor->setText ( QVariant ( CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI( adakDate_vade_tarihi->QDATE() ) ).toString());
    }
    else if (object EQ lineedit_aciklama ) {

        if ( E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tahsilat_sekli->currentText() ) EQ ENUM_NAKIT ) {
            lineedit_valor->setText("0");
        }
    }
    else if (  object EQ commaedit_tutar ) {


        double  tutar   =   commaedit_tutar->GET_DOUBLE();
        double  kur     =   m_ui->commaEdit_kur->GET_DOUBLE();

        if ( kur EQ 0.0 ) {
            commaedit_temel_para_birimi_tutari->SET_DOUBLE ( tutar );
        }
        else {
            commaedit_temel_para_birimi_tutari->SET_DOUBLE ( kur * tutar );
        }
        double cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI ( m_cari_para_birimi_id,
                                                                       m_makbuz_para_birimi_id,
                                                                       kur,
                                                                       m_ui->commaEdit_parite->GET_DOUBLE(),
                                                                       tutar );

        commaedit_cari_para_birimi_tutari->SET_DOUBLE ( cari_para_birimi_tutari );


        double max_nakit_tutari = 0.00;
        double islem_tutar      = ROUND( commaedit_temel_para_birimi_tutari->GET_DOUBLE() );

        if ( E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tahsilat_sekli->currentText()) EQ ENUM_NAKIT ) {

            SQL_QUERY query( DB );

            query.PREPARE_SELECT( "e9_sabit_degerler",
                                  "max_nakit_giris_cikis_tutari",
                                  "sabit_deger_id = :sabit_deger_id" );
            query.SET_VALUE     ( ":sabit_deger_id",    1 );

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                max_nakit_tutari    = query.VALUE( 0 ).toDouble();
            }

            if ( M_FIS->toplam_alacak + islem_tutar > max_nakit_tutari OR M_FIS->toplam_borc + islem_tutar > max_nakit_tutari ) {
                int msg_secim   = MSG_YES_NO( tr( "Vergi Usul Kanununca belirlenen tahsilat ve ödeme sınırlarını aştınız. \n "
                                                  "İşlemede Devam etmek istermisiniz ?" ), NULL );

                if ( msg_secim EQ ADAK_NO ) {
                    return ADAK_FAIL;
                }
            }

       }

    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_LINE_EMPTY(int row_number)
{

    QComboBox  * combobox_tahsilat_sekli = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,TAHSILAT_TURU_COLUMN);
    QCommaEdit * commaedit_satir_tutari  = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,TUTAR_COLUMN);

    if ( combobox_tahsilat_sekli->currentIndex() EQ -1 ) {
        MSG_WARNING( tr("Tahsilat Şeklini boş bırakamazsınız!.."), combobox_tahsilat_sekli );

        return ADAK_FAIL;
    }

    if ( commaedit_satir_tutari->GET_DOUBLE() EQ 0.0 ) {
        MSG_WARNING( tr("Tahsilat tutarını boş bırakamazsınız!.."), commaedit_satir_tutari );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}


/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_ADD_LINE
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_ADD_LINE(int fis_id, int row_number)
{
    if ( CARI_GET_FIS_YAZDIRILDIMI( fis_id ) EQ ENUM_FIS_YAZILDI ) {
        MSG_WARNING( tr("Fiş Yazdırıldığı için üzerinde Değişiklik yapamazsınız..!"), NULL );
        return ADAK_FAIL;
    }
    QComboBox * combo_box = (QComboBox *)m_ui->tablewidget_fis_satirlari->cellWidget(row_number,TAHSILAT_TURU_COLUMN);
    int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combo_box->currentText());

    int satir_id = -1;

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN);
    if (item NE NULL) {
        satir_id = item->text().toInt();
    }

    switch (islem_turu) {
        case ENUM_MUSTERI_CEKI   :
        case ENUM_MUSTERI_SENEDI :

            if ( SATIR_KAYDI_YAPILABILIR_MI(row_number,islem_turu) EQ false ) {
                if ( OPEN_CARI_TAHSILAT_CEKSENET_BATCH ( satir_id , islem_turu , M_ISLEM_DETAYLARI,
                                                         m_satirdaki_islem_turu_degisti,this) EQ false) {
                    return ADAK_FAIL;
                }
                ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ(row_number);

                if ( SATIR_KAYDI_YAPILABILIR_MI(row_number,islem_turu) EQ false ) {
                    return ADAK_FAIL;
                }
            }
            break;
        case ENUM_KREDI_KARTI    :
            if ( SATIR_KAYDI_YAPILABILIR_MI(row_number,islem_turu) EQ false ) {

                if ( OPEN_E9_KK_TAHSILAT_BATCH(satir_id ,M_ISLEM_DETAYLARI ,CARI_MODULU,this ) EQ false ) {
                    return ADAK_FAIL;
                }
                ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ(row_number);

                if ( SATIR_KAYDI_YAPILABILIR_MI(row_number,islem_turu) EQ false ) {
                    return ADAK_FAIL;
                }
            }
            break;
        case ENUM_HAVALE   :
        case ENUM_EFT      :

            if ( SATIR_KAYDI_YAPILABILIR_MI( row_number,islem_turu) EQ false ) {

                if ( OPEN_E9_EFT_BATCH(ENUM_CARI_TAHSILAT_FISI , islem_turu ,satir_id ,
                                         m_makbuz_para_birimi_id, M_ISLEM_DETAYLARI , CARI_MODULU,this ) EQ false ) {
                    return ADAK_FAIL;
                }
                ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ( row_number );

                if ( SATIR_KAYDI_YAPILABILIR_MI( row_number,islem_turu ) EQ false ) {
                    return ADAK_FAIL;
                }
            }
            break;
        default :
                break;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::ADD_LINE
***************************************************************************************/


void CARI_TAHSILAT_FISI::ADD_LINE ( int fis_id, int row_number )
{
    QLineEdit  *  lineedit_aciklama         = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN                 );
    QCommaEdit *  commaedit_satir_tutari    = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TUTAR_COLUMN                    );
    QComboBox  *  combobox_tahsilat_sekli   = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TAHSILAT_TURU_COLUMN            );
    QLineEdit  *  lineedit_cek_senet_no     = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CEK_SENET_NO_COLUMN             );
    QAdakDate *  adakDate_vade_tarihi     = ( QAdakDate * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, VADE_TARIHI_COLUMN              );
    QLineEdit  *  lineedit_portfoy_no       = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PORTFOY_NO_COLUMN               );
    QLineEdit  *  line_edit_banka_ismi      = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BANKA_KODU_COLUMN               );
    QLineEdit  *  line_edit_sube_kodu       = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_KODU_COLUMN                );
    QLineEdit  *  line_edit_sube_adi        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_ADI_COLUMN                 );
    QLineEdit  *  line_edit_hesap_no        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_NO_COLUMN                 );
    QLineEdit  *  line_edit_kesideci        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KESIDECI_COLUMN                 );
    QAdakDate *  adakDate_keside_tarihi   = ( QAdakDate * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KESIDE_TARIHI_COLUMN            );
    QLineEdit  *  lineedit_frm_hesap_no_id  = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, FRM_HESAP_NO_ID_COLUMN          );
    QLineEdit  *  lineedit_pos_id           = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, POS_ID_COLUMN                   );
    QLineEdit  *  lineedit_kk_numarasi      = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KK_NUMARASI_COLUMN              );
    QLineEdit  *  lineedit_kk_sahibi        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KK_SAHIBI_COLUMN                );
    QLineEdit  *  lineedit_hesap_no_id      = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, FRM_HESAP_NO_ID_COLUMN          );

    CARI_CLEAR_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );

    M_FIS_SATIRI->islem_turu                            = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tahsilat_sekli->currentText() );
    M_FIS_SATIRI->order_number                          = m_ui->tablewidget_fis_satirlari->item ( row_number, ORDER_COLUMN )->text().toInt();
    M_FIS_SATIRI->musteri_banka_ismi                    = line_edit_banka_ismi->text();
    M_FIS_SATIRI->musteri_sube_kodu                     = line_edit_sube_kodu->text();
    M_FIS_SATIRI->musteri_sube_ismi                     = line_edit_sube_adi->text();
    M_FIS_SATIRI->musteri_hesap_no                      = line_edit_hesap_no->text();
    M_FIS_SATIRI->hesap_no_id                           = lineedit_frm_hesap_no_id->text().toInt();
    M_FIS_SATIRI->cek_senet_no                          = lineedit_cek_senet_no->text();
    M_FIS_SATIRI->cek_senet_vade_tarihi                 = adakDate_vade_tarihi->DATE();
    M_FIS_SATIRI->cek_senet_keside_tarihi               = adakDate_keside_tarihi->DATE();
    M_FIS_SATIRI->kesideci                              = line_edit_kesideci->text();
    M_FIS_SATIRI->alacak_tutari                         = commaedit_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->doviz_id                              = m_makbuz_para_birimi_id;
    M_FIS_SATIRI->aciklama                              = lineedit_aciklama->text();
    M_FIS_SATIRI->kredi_karti_numarasi                  = lineedit_kk_numarasi->text();
    M_FIS_SATIRI->kredi_karti_sahibi                    = lineedit_kk_sahibi->text();
    M_FIS_SATIRI->pos_id                                = lineedit_pos_id->text().toInt();
    M_FIS_SATIRI->fis_id                                = fis_id;
    M_FIS_SATIRI->cari_hesap_id                         = 0;
    M_FIS_SATIRI->hesap_no_id                           = lineedit_hesap_no_id->text().toInt();

    M_FIS_SATIRI->doviz_kuru                            = M_FIS->doviz_kuru;
    M_FIS_SATIRI->parite                                = M_FIS->parite;

    M_FIS_SATIRI->program_id                            = M_FIS->program_id;
    M_FIS_SATIRI->modul_id                              = M_FIS->modul_id;
    M_FIS_SATIRI->base_record_id                        = 0;


    int fis_satiri_id = CARI_FIS_SATIRI_EKLE ( M_FIS, M_FIS_SATIRI );

    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_FIS->toplam_alacak );

    lineedit_portfoy_no->setText ( QVariant ( M_FIS_SATIRI->portfoy_no ).toString() );

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( fis_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( row_number, FIS_ID_COLUMN, new_item );

    new_item = new QTableWidgetItem ( QVariant ( fis_satiri_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem   ( row_number, ROW_ID_COLUMN, new_item );

    int ortalama_gun = CEK_ORTALAMA_GUN_SAYISI( fis_id, ENUM_CARI_TAHSILAT_FISI );

    if ( ortalama_gun > 0 ) {
        m_ui->label_ortalama_gun->setHidden             ( false );
        m_ui->label_ortalama_vade->setHidden            ( false );
        m_ui->line_edit_ortalama_gun->setHidden         ( false );
        m_ui->adakDate_ortalama_gun_tarih->setHidden   ( false );
        m_ui->line_edit_ortalama_gun->setText           ( QVariant ( ortalama_gun ).toString() );
        m_ui->adakDate_ortalama_gun_tarih->SET_DATE(QDate::currentDate().addDays ( ortalama_gun ) );
    }



    m_log_kaydi_eklenecek_mi = true;

    // m_ui->lineedit_muh_fis_no->setText ( QVariant ( E9_GET_FIS_NO ( MUHASEBE_MODULU, M_FIS->muh_fis_id ) ).toString() );

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_TAHSILAT_FISI ) +
                        ",Tahsilat Makbuzu Seri / No :" + m_ui->lineEdit_belge_seri->text() +
                        " " + m_ui->lineEdit_belge_numarasi->text()+ "\n" +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                         ",Fiş No : " + m_ui->lineedit_fis_no->text() +
                         ",Toplam Alacak : " + VIRGUL_EKLE ( QVariant ( m_ui->commaedit_toplam_tutar->GET_DOUBLE() ).toString() ) );

}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_UPDATE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED(fis_id);
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("car_fis_satirlari","islem_turu ,cek_senet_id", "fis_satiri_id = :fis_satiri_id");
    select_query.SET_VALUE(":fis_satiri_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());

    if (select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        int tahsilat_sekli = select_query.VALUE(0).toInt();
        int cek_senet_id   = select_query.VALUE(1).toInt();


        if (tahsilat_sekli EQ ENUM_MUSTERI_CEKI) {
            if (CEK_SON_CEK_SENET_HAREKETINI_BUL(cek_senet_id) NE ENUM_PORTFOYDE) {
                MSG_WARNING( tr("Fiş satırlarındaki müşteri çeki işlem görmüş,satırı değiştiremezsiniz!.."), NULL );
                return ADAK_FAIL;
            }
        }
        if (tahsilat_sekli EQ ENUM_MUSTERI_SENEDI) {
            if (CEK_SON_CEK_SENET_HAREKETINI_BUL(cek_senet_id) NE ENUM_PORTFOYDE) {
                MSG_WARNING( tr("Fiş satırlarındaki müşteri senedi işlem görmüş,satırı değiştiremezsiniz!.."), NULL );
                return ADAK_FAIL;
            }
        }

    }
    if ( CARI_GET_FIS_YAZDIRILDIMI( fis_id ) EQ ENUM_FIS_YAZILDI ) {
        // SONSUZ DONGUYE GIRMEMESI ICIN CARI HESAP KODUNA FOCUSLANDI
        MSG_WARNING( tr("Fiş Yazdırıldığı için üzerinde Değişiklik yapamazsınız..!"), m_ui->searchedit_cari_hesap_kodu );
        return ADAK_FAIL;
    }
    QComboBox * combo_box = (QComboBox *)m_ui->tablewidget_fis_satirlari->cellWidget(row_number,TAHSILAT_TURU_COLUMN);
    int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combo_box->currentText());

    int satir_id = -1;

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN);
    if (item NE NULL) {
        satir_id = item->text().toInt();
    }

    switch (islem_turu) {
        case ENUM_MUSTERI_CEKI   :
        case ENUM_MUSTERI_SENEDI :

            if ( SATIR_KAYDI_YAPILABILIR_MI(row_number,islem_turu) EQ false ) {
                if ( OPEN_CARI_TAHSILAT_CEKSENET_BATCH ( satir_id , islem_turu , M_ISLEM_DETAYLARI,
                                                         m_satirdaki_islem_turu_degisti,this) EQ false) {
                    return ADAK_FAIL;
                }
                ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ(row_number);

                if ( SATIR_KAYDI_YAPILABILIR_MI(row_number,islem_turu) EQ false ) {
                    return ADAK_FAIL;
                }
            }
            break;
        case ENUM_KREDI_KARTI    :
            if ( SATIR_KAYDI_YAPILABILIR_MI(row_number,islem_turu) EQ false ) {

                if ( OPEN_E9_KK_TAHSILAT_BATCH(satir_id ,M_ISLEM_DETAYLARI ,CARI_MODULU,this ) EQ false ) {
                    return ADAK_FAIL;
                }
                ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ(row_number);

                if ( SATIR_KAYDI_YAPILABILIR_MI(row_number,islem_turu) EQ false ) {
                    return ADAK_FAIL;
                }
            }
            break;
        case ENUM_HAVALE   :
        case ENUM_EFT      :

            if ( SATIR_KAYDI_YAPILABILIR_MI( row_number,islem_turu) EQ false ) {

                if ( OPEN_E9_EFT_BATCH(ENUM_CARI_TAHSILAT_FISI , islem_turu ,satir_id ,
                                         m_makbuz_para_birimi_id, M_ISLEM_DETAYLARI ,CARI_MODULU, this ) EQ false ) {
                    return ADAK_FAIL;
                }
                ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ(row_number);

                if ( SATIR_KAYDI_YAPILABILIR_MI(row_number,islem_turu) EQ false ) {
                    return ADAK_FAIL;
                }
            }
            break;
        default :
                break;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::UPDATE_LINE
***************************************************************************************/

void CARI_TAHSILAT_FISI::UPDATE_LINE ( int fis_id, int row_number )
{
    QLineEdit  *  lineedit_aciklama                   = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN                  );
    QCommaEdit *  commaedit_new_satir_tutari          = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TUTAR_COLUMN                     );
    QComboBox  *  combobox_tahsilat_sekli             = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TAHSILAT_TURU_COLUMN             );
    QLineEdit  *  lineedit_cek_senet_no               = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CEK_SENET_NO_COLUMN              );
    QAdakDate  *  adakDate_vade_tarihi              = ( QAdakDate  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, VADE_TARIHI_COLUMN               );
    QLineEdit  *  lineedit_banka_ismi                 = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BANKA_KODU_COLUMN                );
    QLineEdit  *  lineedit_sube_kodu                  = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_KODU_COLUMN                 );
    QLineEdit  *  lineedit_sube_adi                   = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_ADI_COLUMN                  );
    QLineEdit  *  lineedit_hesap_no                   = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_NO_COLUMN                  );
    QLineEdit  *  lineedit_kesideci                   = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KESIDECI_COLUMN                  );
    QAdakDate  *  dateedit_keside_tarihi              = ( QAdakDate  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KESIDE_TARIHI_COLUMN             );
    QLineEdit  *  lineedit_frm_hesap_no_id            = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, FRM_HESAP_NO_ID_COLUMN           );
    QLineEdit  *  lineedit_pos_id                     = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, POS_ID_COLUMN                    );
    QLineEdit  *  lineedit_kk_numarasi                = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KK_NUMARASI_COLUMN               );
    QLineEdit  *  lineedit_kk_sahibi                  = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KK_SAHIBI_COLUMN                 );
    QLineEdit  *  lineedit_portfoy_no                 = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PORTFOY_NO_COLUMN                );


    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    int portfoy_no    = 0;

    if ( lineedit_portfoy_no->text() NE "*" ) {
        portfoy_no = lineedit_portfoy_no->text().toInt();
    }

    CARI_CLEAR_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );

    CARI_FIS_SATIRINI_OKU( M_FIS_SATIRI , fis_satiri_id);

    CARI_FIS_SATIRI_SIL(M_FIS,fis_satiri_id);




    M_FIS_SATIRI->fis_id                    =   fis_id;
    M_FIS_SATIRI->aciklama                  =   lineedit_aciklama->text();
    M_FIS_SATIRI->borc_tutari               =   0.0;
    M_FIS_SATIRI->alacak_tutari             =   commaedit_new_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->kredi_karti_numarasi      =   lineedit_kk_numarasi->text();
    M_FIS_SATIRI->kredi_karti_sahibi        =   lineedit_kk_sahibi->text();
    M_FIS_SATIRI->musteri_banka_ismi        =   lineedit_banka_ismi->text();
    M_FIS_SATIRI->musteri_sube_kodu         =   lineedit_sube_kodu->text();
    M_FIS_SATIRI->musteri_sube_ismi         =   lineedit_sube_adi->text();
    M_FIS_SATIRI->musteri_hesap_no          =   lineedit_hesap_no->text();
    M_FIS_SATIRI->cek_senet_vade_tarihi     =   adakDate_vade_tarihi->DATE();
    M_FIS_SATIRI->cek_senet_keside_tarihi   =   dateedit_keside_tarihi->DATE();
    M_FIS_SATIRI->kesideci                  =   lineedit_kesideci->text();
    M_FIS_SATIRI->cek_senet_no              =   lineedit_cek_senet_no->text();
    M_FIS_SATIRI->islem_turu                =   E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tahsilat_sekli->currentText() );
    M_FIS_SATIRI->hesap_no_id               =   lineedit_frm_hesap_no_id->text().toInt();
    M_FIS_SATIRI->pos_id                    =   lineedit_pos_id->text().toInt();
    M_FIS_SATIRI->doviz_kodu.clear();
    M_FIS_SATIRI->portfoy_no                =   portfoy_no;



    int new_satir_id = CARI_FIS_SATIRI_EKLE(M_FIS,M_FIS_SATIRI);

    if( new_satir_id NE 0 )
    {
        QTableWidgetItem * new_item;
        new_item = new QTableWidgetItem( QVariant ( new_satir_id ).toString());
        m_ui->tablewidget_fis_satirlari->setItem(row_number,ROW_ID_COLUMN,new_item);
    }

    lineedit_portfoy_no->setText                ( QVariant ( M_FIS_SATIRI->portfoy_no ).toString() );
    m_ui->commaedit_toplam_tutar->SET_DOUBLE    ( M_FIS->toplam_alacak );

    int ortalama_gun = CEK_ORTALAMA_GUN_SAYISI( fis_id, ENUM_CARI_TAHSILAT_FISI );

    if ( ortalama_gun > 0 ) {
        m_ui->label_ortalama_gun->setHidden             ( false );
        m_ui->label_ortalama_vade->setHidden            ( false );
        m_ui->line_edit_ortalama_gun->setHidden         ( false );
        m_ui->adakDate_ortalama_gun_tarih->setHidden  ( false );
        m_ui->line_edit_ortalama_gun->setText           ( QVariant ( ortalama_gun ).toString() );
        m_ui->adakDate_ortalama_gun_tarih->SET_DATE( QDate::currentDate().addDays ( ortalama_gun ) );
    }
    else {
        m_ui->label_ortalama_gun->setHidden             ( true );
        m_ui->label_ortalama_vade->setHidden            ( true );
        m_ui->line_edit_ortalama_gun->setHidden         ( true );
        m_ui->adakDate_ortalama_gun_tarih->setHidden  ( true );
    }

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_TAHSILAT_FISI ) + "," +
                        m_ui->lineEdit_belge_seri->text()  + " " +
                        m_ui->lineEdit_belge_numarasi->text() + "\n" +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") + ",Fiş No : "
                        + QVariant ( m_ui->lineedit_fis_no->text() ).toString()
                        + ",Toplam Alacak : " + VIRGUL_EKLE ( QVariant ( m_ui->commaedit_toplam_tutar->GET_DOUBLE() ).toString() ) );
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHECK_DELETE_LINE  ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );
    SQL_QUERY select_query ( DB );

    if ( CARI_GET_FIS_YAZDIRILDIMI( fis_id ) EQ ENUM_FIS_YAZILDI ) {
        MSG_WARNING( tr("Fiş Yazdırıldığı için üzerinde Değişiklik yapamazsınız..!"), NULL );
        return ADAK_FAIL;
    }
    select_query.PREPARE_SELECT ( "car_fis_satirlari","islem_turu,cek_senet_id","fis_satiri_id = :fis_satiri_id");
    select_query.SET_VALUE      ( ":fis_satiri_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());

    if ( select_query.SELECT() NE 0 ) {
         select_query.NEXT();
         int tahsilat_sekli = select_query.VALUE(0).toInt();
         int cek_senet_id   = select_query.VALUE(1).toInt();

        if ( tahsilat_sekli EQ ENUM_MUSTERI_CEKI ) {
            if ( CEK_SON_CEK_SENET_HAREKETINI_BUL ( cek_senet_id ) NE ENUM_PORTFOYDE ) {
                MSG_WARNING( tr("Fiş satırlarındaki müşteri çeki işlem görmüş,satırı silemezsiniz!.."), NULL );
                return ADAK_FAIL;
            }
        }
        if ( tahsilat_sekli EQ ENUM_MUSTERI_SENEDI ) {
            if ( CEK_SON_CEK_SENET_HAREKETINI_BUL ( cek_senet_id ) NE ENUM_PORTFOYDE ) {
                QMessageBox::critical ( this,"HATA",tr ( "Fiş satırlarındaki müşteri senedi işlem görmüş,satırı silemezsiniz!.."));
                return ADAK_FAIL;
            }
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::DELETE_LINE
***************************************************************************************/

void CARI_TAHSILAT_FISI::DELETE_LINE ( int fis_id, int row_number )
{
    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    M_FIS_SATIRI->fis_id = fis_id;

    CARI_FIS_SATIRI_SIL ( M_FIS, fis_satiri_id );

    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_FIS->toplam_alacak );

    int ortalama_gun = CEK_ORTALAMA_GUN_SAYISI( fis_id, ENUM_CARI_TAHSILAT_FISI );

    if ( ortalama_gun > 0 ) {
        m_ui->label_ortalama_gun->setHidden             ( false );
        m_ui->label_ortalama_vade->setHidden            ( false );
        m_ui->line_edit_ortalama_gun->setHidden         ( false );
        m_ui->adakDate_ortalama_gun_tarih->setHidden  ( false );
        m_ui->line_edit_ortalama_gun->setText           ( QVariant ( ortalama_gun ).toString() );
        m_ui->adakDate_ortalama_gun_tarih->SET_DATE( QDate::currentDate().addDays ( ortalama_gun ) );
    }
    else {
        m_ui->label_ortalama_gun->setHidden             ( true );
        m_ui->label_ortalama_vade->setHidden            ( true );
        m_ui->line_edit_ortalama_gun->setHidden         ( true );
        m_ui->adakDate_ortalama_gun_tarih->setHidden  ( true );
    }


    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_TAHSILAT_FISI ) + "," +
                        m_ui->lineEdit_belge_seri->text() + " " +
                        m_ui->lineEdit_belge_numarasi->text() + "\n" +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                        ",Fiş No : " + m_ui->lineedit_fis_no->text() +
                        ",Toplam Alacak : " + VIRGUL_EKLE ( QVariant ( m_ui->commaedit_toplam_tutar->GET_DOUBLE() ).toString() ) );
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::LOCK_FIS_RECORD ( int fis_id )
{
    return DB->LOCK_ROW (  "fis_id","car_fisler", QString ( "fis_id = %1" ).arg ( fis_id ) );
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void CARI_TAHSILAT_FISI::UNLOCK_FIS_RECORD ( int fis_id )
{
    DB->UNLOCK_ROW (  "fis_id","car_fisler", QString ( "fis_id = %1" ).arg ( fis_id ) );
}


/**************************************************************************************
                   CARI_TAHSILAT_FISI::SEARCH_EDIT_CLICKED
***************************************************************************************/

void CARI_TAHSILAT_FISI::SEARCH_EDIT_CLICKED ( QWidget * widget,QLineEdit * lineedit)
{
    Q_UNUSED ( widget );

    int  cari_hesap_id = OPEN_CARI_KART_ARAMA ( lineedit->text(),SAHIS_ARTI_FIRMA , this ,1 , E9_PROGRAMI, CARI_MODULU );
    if ( cari_hesap_id NE -1 ) {
        lineedit->setText(CARI_FIND_HESAP_KODU ( cari_hesap_id ) );


    }
}


/**************************************************************************************
                   CARI_TAHSILAT_FISI::FIS_GUNCELLENEBILIR_MI_SILINEBILIR_MI
***************************************************************************************/

bool CARI_TAHSILAT_FISI::FIS_GUNCELLENEBILIR_MI_SILINEBILIR_MI ( int fis_id )
{
    SQL_QUERY select_query ( DB ) ;

    select_query.PREPARE_SELECT ( "car_fis_satirlari","islem_turu,cek_senet_id","fis_id = :fis_id");
    select_query.SET_VALUE      ( ":fis_id" , fis_id );

    if  ( select_query.SELECT() NE 0  ) {
        while ( select_query.NEXT() ) {
            int tahsilat_sekli = select_query.VALUE(0).toInt();
            int cek_senet_id   = select_query.VALUE(1).toInt();

            if ( tahsilat_sekli EQ ENUM_MUSTERI_CEKI OR tahsilat_sekli EQ ENUM_MUSTERI_SENEDI ) {

                if ( CEK_SON_CEK_SENET_HAREKETINI_BUL ( cek_senet_id ) NE ENUM_PORTFOYDE ) {
                    return false;
                }
            }
        }
    }
    return true;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void CARI_TAHSILAT_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_record_id )
{
    if ( p_button EQ m_ui->push_button_tahsilat_makbuzu_yazdir ) {

        QString cari_tah_belge_seri, cari_tah_belge_numarasi;

        if ( M_FIS->yazdirildi_mi NE 1 ) {

            SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;
            int result = SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

            if( result EQ 0 ) {
                MSG_WARNING( "Ünite bulunamadı", NULL );
                return;
            }

            cari_tah_belge_seri     = m_ui->lineEdit_belge_seri->text();
            cari_tah_belge_numarasi = m_ui->lineEdit_belge_numarasi->text();

            if ( m_ui->lineEdit_belge_seri->text().isEmpty() EQ true OR m_ui->lineEdit_belge_numarasi->text().isEmpty() EQ true ) {
                cari_tah_belge_seri      = UNITE_BILG.tahsilat_makbuzu_belge_seri;
                cari_tah_belge_numarasi  = UNITE_BILG.tahsilat_makbuzu_belge_num;
            }

            int secim = MSG_YES_NO( tr( "Tahsilat Makbuzu; \n"
                                        "Belge Seri          : %1 \n"
                                        "Belge Numarası : %2 \n"
                                        "ile yazdırılacaktır. Onaylıyormusunuz ?").arg( cari_tah_belge_seri ).arg( cari_tah_belge_numarasi ), m_ui->lineEdit_belge_seri );

            if ( secim EQ ADAK_NO ) {
                return;
            }
        }
        else {
            int secim = MSG_YES_NO( tr( "Bu Tahsilat Makbuzu daha önce yazdırıldı. Tekrar yazdırmak istiyormusunuz ?" ), NULL );

            if ( secim EQ ADAK_NO ) {
                return;
            }
        }

        PRINT_CARI_MAKBUZ_BELGESI(p_record_id, this );

        DB->START_TRANSACTION();
        SQL_QUERY query( DB );
        query.PREPARE_UPDATE( "car_fisler", "fis_id", "yazdirildi_mi, belge_seri, belge_numarasi", "fis_id = :fis_id");
        query.SET_VALUE     ( ":yazdirildi_mi"  , 1 );
        query.SET_VALUE     ( ":belge_seri"     , cari_tah_belge_seri );
        query.SET_VALUE     ( ":belge_numarasi" , cari_tah_belge_numarasi );
        query.SET_VALUE     ( ":fis_id"         , p_record_id );
        query.UPDATE();

        SUBE_UPDATE_TAH_MKBZ_BELGE_NO( SUBE_GET_UNITE_ID(), QVariant( QVariant( cari_tah_belge_numarasi ).toInt() + 1 ).toString() );
        DB->COMMIT_TRANSACTION();

        m_ui->lineEdit_belge_seri->setText( cari_tah_belge_seri );
        m_ui->lineEdit_belge_numarasi->setText( cari_tah_belge_numarasi );

        M_FIS->yazdirildi_mi = 1;
    }
    else if ( p_button EQ m_ui->pushButton_urettigi_fisler ) {
        E9_URETILEN_FIS_SEC( M_FIS->program_id, M_FIS->modul_id, p_record_id, this );
    }

}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int CARI_TAHSILAT_FISI::CHANGER_BUTTON_CLICKED (QAbstractButton *button)
{
    if ( button EQ m_ui->pushButton_detaylar ) {

        int record_id = GET_RECORD_ID();

        int muh_fis_id = MUH_GET_ENT_FIS_ID(M_FIS->program_id , M_FIS->modul_id , record_id );

        M_FIS_DETAYLARI->muh_fis_no = E9_GET_FIS_NO ( MUHASEBE_MODULU , muh_fis_id);

        SQL_QUERY query(DB);

        QStringList cek_fis_id_list = CEK_GET_ENT_FIS_ID_LIST ( M_FIS->program_id , M_FIS->modul_id , record_id );

        for ( int i = 0 ; i < cek_fis_id_list.size() ; i++ ) {
           int cek_senet_fis_id = QVariant (cek_fis_id_list.at(i)).toInt();

           int bordro_turu = CEK_GET_CEK_BORDRO_TURU( cek_senet_fis_id );

           if ( bordro_turu EQ ENUM_MUSTERI_CEK_BORDROSU ) {
                M_FIS_DETAYLARI->cek_fis_no = E9_GET_FIS_NO(CEKSENET_MODULU , cek_senet_fis_id);
           }
           else {
               M_FIS_DETAYLARI->senet_fis_no = E9_GET_FIS_NO(CEKSENET_MODULU , cek_senet_fis_id);
           }

        }

        QStringList bnk_fis_no_list ;
        query.PREPARE_SELECT("car_fis_satirlari",
                             "hesap_no_id", "fis_id = :fis_id");
        query.SET_VALUE(":fis_id" , record_id );

        if ( query.SELECT() NE 0 ) {
            while ( query.NEXT() ) {

                int hesap_no_id = query.VALUE(0).toInt();

                if ( hesap_no_id NE 0 ) {
                    int bnk_fis_id = BNK_GET_ENT_FIS_ID(M_FIS->program_id , M_FIS->modul_id,
                                                        record_id , hesap_no_id);

                    int banka_fis_no = E9_GET_FIS_NO(BANKA_MODULU , bnk_fis_id);

                    bnk_fis_no_list << QVariant(banka_fis_no).toString();
                }
            }
        }
        M_FIS_DETAYLARI->banka_fis_no = bnk_fis_no_list;

        OPEN_E9_ENT_DETAYLARI_BATCH(M_FIS_DETAYLARI , true , this);


        if ( M_FIS_DETAYLARI->muh_kasa_hesap_id NE M_FIS->muh_kasa_hesap_id ) {
            return ADAK_RECORD_CHANGED;
        }
        return ADAK_RECORD_UNCHANGED;
    }

    return 0;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::CEK_SENET_BILGILERINI_YAZ
***************************************************************************************/

void CARI_TAHSILAT_FISI::ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ ( int row_number )
{
    QLineEdit * lineedit_portfoy_no       = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PORTFOY_NO_COLUMN       );
    QAdakDate * adakDate_vade_tarihi    = ( QAdakDate * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, VADE_TARIHI_COLUMN      );
    QLineEdit * lineedit_valor            = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, VALOR_COLUMN            );
    QLineEdit * lineedit_cek_senet_no     = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CEK_SENET_NO_COLUMN     );
    QLineEdit * line_edit_banka_ismi      = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BANKA_KODU_COLUMN       );
    QLineEdit * line_edit_sube_kodu       = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_KODU_COLUMN        );
    QLineEdit * line_edit_sube_adi        = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_ADI_COLUMN         );
    QLineEdit * line_edit_hesap_no        = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_NO_COLUMN         );
    QAdakDate * adakDate_keside_tarihi  = ( QAdakDate * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KESIDE_TARIHI_COLUMN    );
    QLineEdit * line_edit_kesideci        = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KESIDECI_COLUMN         );
    QLineEdit * line_edit_frm_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, FRM_HESAP_NO_ID_COLUMN  );
    QLineEdit * lineedit_pos_id           = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, POS_ID_COLUMN           );
    QLineEdit * lineedit_kk_numarasi      = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KK_NUMARASI_COLUMN      );
    QLineEdit * lineedit_kk_sahibi        = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KK_SAHIBI_COLUMN        );


    lineedit_portfoy_no->setText        ( M_ISLEM_DETAYLARI->portfoy_no );
    adakDate_vade_tarihi->SET_DATE     ( M_ISLEM_DETAYLARI->vade_tarihi );
    lineedit_valor->setText             ( M_ISLEM_DETAYLARI->valor         );
    lineedit_cek_senet_no->setText      ( M_ISLEM_DETAYLARI->cek_senet_no  );
    line_edit_banka_ismi->setText       ( M_ISLEM_DETAYLARI->banka_ismi    );
    line_edit_sube_kodu->setText        ( M_ISLEM_DETAYLARI->sube_kodu     );
    line_edit_sube_adi->setText         ( M_ISLEM_DETAYLARI->sube_adi      );
    line_edit_hesap_no->setText         ( M_ISLEM_DETAYLARI->hesap_no      );
    adakDate_keside_tarihi->SET_DATE   ( M_ISLEM_DETAYLARI->keside_tarihi );
    line_edit_kesideci->setText         ( M_ISLEM_DETAYLARI->kesideci      );
    line_edit_frm_hesap_no_id->setText  ( QVariant ( M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());
    lineedit_pos_id->setText            ( QVariant ( M_ISLEM_DETAYLARI->pos_id).toString() );
    lineedit_kk_numarasi->setText       ( M_ISLEM_DETAYLARI->kredi_karti_numarasi);
    lineedit_kk_sahibi->setText         ( M_ISLEM_DETAYLARI->kredi_karti_sahibi);
    adakDate_vade_tarihi->SET_DATE     ( M_ISLEM_DETAYLARI->vade_tarihi );

}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::LINE_SAVER_BUTTON_CLICKED
***************************************************************************************/

int CARI_TAHSILAT_FISI::LINE_CHANGER_BUTTON_CLICKED(QAbstractButton *button, int row_number)
{
    Q_UNUSED ( button );

    QComboBox * combobox_tahsilat_sekli = ( QComboBox* ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,TAHSILAT_TURU_COLUMN );
    int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tahsilat_sekli->currentText());

    if (islem_turu EQ ENUM_NAKIT ) {
        QPushButton * push_button_detaylar = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DETAYLAR_COLUMN );
        if ( push_button_detaylar NE NULL ) {
            push_button_detaylar->setDisabled(true);
        }
        return ADAK_RECORD_UNCHANGED;
    }

    int fis_satiri_id = -1;

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN);
    if ( item NE NULL ) {
        fis_satiri_id = item->text().toInt();
    }


    switch (islem_turu ) {
        case ENUM_MUSTERI_CEKI :
        case ENUM_MUSTERI_SENEDI:
            if ( OPEN_CARI_TAHSILAT_CEKSENET_BATCH ( fis_satiri_id ,islem_turu, M_ISLEM_DETAYLARI, false, this ) EQ true ) {
                ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ ( row_number );

                return ADAK_RECORD_CHANGED;
            }

            break;
        case ENUM_HAVALE :
        case ENUM_EFT:
            if ( OPEN_E9_EFT_BATCH( ENUM_CARI_TAHSILAT_FISI , islem_turu ,fis_satiri_id ,
                                      m_makbuz_para_birimi_id, M_ISLEM_DETAYLARI ,CARI_MODULU, this) EQ true ) {
                ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ(row_number);

                return ADAK_RECORD_CHANGED;
            }

            break;
        case ENUM_KREDI_KARTI:
            if ( OPEN_E9_KK_TAHSILAT_BATCH(fis_satiri_id,M_ISLEM_DETAYLARI ,CARI_MODULU, this) EQ true ) {
                ISLEM_DETAYLARINI_GIZLI_SUTUNLARA_YAZ(row_number);

                return ADAK_RECORD_CHANGED;
            }

            break;
    }

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::LEAVE_RECORD
***************************************************************************************/

int CARI_TAHSILAT_FISI::LEAVE_RECORD()
{
    if ( m_log_kaydi_eklenecek_mi EQ true ) {
        DB->START_TRANSACTION();
        if ( m_kayit_eklendi EQ true ) {
            E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_ADD , m_log_detaylari );
        }
        if ( m_kayit_silindi EQ true ) {
             E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_DELETE , m_log_detaylari );
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
             E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_UPDATE , m_log_detaylari );
        }
        DB->COMMIT_TRANSACTION();
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_TAHSILAT_FISI::SATIR_KAYDI_YAPILABILIR_MI()
***************************************************************************************/

bool CARI_TAHSILAT_FISI::SATIR_KAYDI_YAPILABILIR_MI(int row_number, int islem_turu)
{
    QLineEdit * lineedit_cek_senet_no       = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CEK_SENET_NO_COLUMN     );
    QLineEdit * line_edit_banka_ismi        = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BANKA_KODU_COLUMN       );
    QLineEdit * line_edit_sube_adi          = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_ADI_COLUMN         );
    QLineEdit * line_edit_hesap_no          = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_NO_COLUMN         );
    QLineEdit * line_edit_kesideci          = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KESIDECI_COLUMN         );
    QLineEdit * line_edit_frm_hesap_no_id   = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,FRM_HESAP_NO_ID_COLUMN   );
    QLineEdit * lineedit_pos_id             = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,POS_ID_COLUMN            );

    switch ( islem_turu ) {
        case ENUM_HAVALE:
        case ENUM_EFT:
            if (line_edit_frm_hesap_no_id->text().toInt() EQ 0 ) {
                return false;
            }

            break;
        case ENUM_MUSTERI_CEKI  :
            if ( lineedit_cek_senet_no->text().isEmpty() EQ true ) {
                return false;
            }
            if ( line_edit_kesideci->text().isEmpty() EQ true ) {
                return false;
            }
            if ( line_edit_banka_ismi->text().isEmpty() EQ true ) {
                return false;
            }
            if ( line_edit_sube_adi->text().isEmpty() EQ true ) {
                return false;
            }
            if ( line_edit_hesap_no->text().isEmpty() EQ true ) {
                return false;
            }

            break;
        case ENUM_MUSTERI_SENEDI:
            if ( lineedit_cek_senet_no->text().isEmpty() EQ true ) {
                return false;
            }
            if (line_edit_kesideci->text().isEmpty() EQ true ) {
                return false;
            }
            break;
        case ENUM_KREDI_KARTI:
            if ( lineedit_pos_id->text().toInt() EQ 0 ) {
                return false;
            }
            break;
    }
    return true;
}


/**************************************************************************************
                   CARI_TAHSILAT_FISI::EKRANI_GUNCELLE
***************************************************************************************/

void CARI_TAHSILAT_FISI::EKRANI_GUNCELLE()
{


    CLOSE_ALL_EVENTS();

    for ( int row = 0 ; row < m_ui->tablewidget_fis_satirlari->rowCount() ; row++) {

        QCommaEdit * commaedit_tutar                    = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row , TUTAR_COLUMN                       );
        QCommaEdit * commaedit_cari_para_birimi_tutari  = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row , CARI_PARA_BIRIMI_TUTARI_COLUMN     );
        QCommaEdit * commaedit_temel_para_birimi_tutari = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row , TEMEL_PARA_BIRIMI_TUTARI_COLUMN    );
        QLineEdit  * lineedit_makbuz_para_birimi        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row,  MAKBUZ_PARA_BIRIMI_COLUMN          );
        QLineEdit  * lineedit_cari_para_birimi          = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row , CARI_PARA_BIRIMI_COLUMN            );

        lineedit_makbuz_para_birimi->setText(m_ui->comboBox_makbuz_para_birimi->currentText());
        lineedit_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi->text());

        double tutar = commaedit_tutar->GET_DOUBLE();
        double kur   = m_ui->commaEdit_kur->GET_DOUBLE();

        double cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI(m_cari_para_birimi_id,m_makbuz_para_birimi_id,
                                                                     kur,m_ui->commaEdit_parite->GET_DOUBLE(),tutar);



        if ( kur EQ 0.0 ) {
            commaedit_temel_para_birimi_tutari->SET_DOUBLE ( tutar );
        }
        else {
            commaedit_temel_para_birimi_tutari->SET_DOUBLE ( kur * tutar );
        }
        commaedit_cari_para_birimi_tutari->SET_DOUBLE(cari_para_birimi_tutari);
    }

    OPEN_ALL_EVENTS();
}
