#include <QHeaderView>
#include <QDate>
#include <QMessageBox>
#include "adak_utils.h"
#include "dvz_enum.h"
#include "ui_cari_odeme_fisi.h"
#include "cari_odeme_fisi_class.h"
#include "cari_odeme_fisi_open.h"
#include "adak_sql.h"
#include "print.h"
#include "e9_struct.h"
#include "sube_struct.h"
#include "adak_std_utils.h"
#include "cari_enum.h"
#include "cari_fis_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "sube_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "cek_enum.h"
#include "cek_console_utils.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "e9_yetki.h"
#include "e9_log.h"
#include "cek_senet_utils.h"
#include "muh_fis_utils.h"
#include "cari_fis_utils.h"
#include "BANKA/bnk_fis_utils.h"
#include "cek_fis_utils.h"
#include "adres_console_utils.h"
#include "cari_fis_arama_open.h"
#include "cari_kart_arama_open.h"
#include "cari_makbuz_belgesi_open.h"
#include "cari_odeme_ceksenet_batch_open.h"
#include "cek_senet_arama_open.h"
#include "cek_senet_formu_open.h"
#include "e9_enum.h"
#include "cek_console_utils.h"
#include "e9_eft_batch_open.h"
#include "e9_ent_detaylari_batch_open.h"
#include "e9_kk_odeme_batch_open.h"
#include "dvz_gui_utils.h"
#include "cari_console_utils.h"
#include "fat_console_utils.h"
#include "cari_oto_ekstre_arama_open.h"

extern ADAK_SQL * DB;

#define ROW_ID_COLUMN                       0
#define FIS_ID_COLUMN                       1
#define ORDER_COLUMN                        2
#define HESAP_NO_ID_COLUMN                  3
#define CEK_SENET_ID_COLUMN                 4
#define KREDI_KARTI_ID_COLUMN               5
#define BANKA_ISMI_COLUMN                   6
#define SUBE_ISMI_COLUMN                    7
#define SUBE_KODU_COLUMN                    8
#define HESAP_NO_COLUMN                     9
#define ODEME_TURU_COLUMN                   10
#define PORTFOY_NO_COLUMN                   11
#define ACIKLAMA_COLUMN                     12
#define TUTAR_COLUMN                        13
#define MAKBUZ_PARA_BIRIMI_COLUMN           14
#define CARI_PARA_BIRIMI_TUTARI_COLUMN      15
#define TEMEL_PARA_BIRIMI_TUTARI_COLUMN     16
#define CARI_PARA_BIRIMI_COLUMN             17
#define DETAYLAR_COLUMN                     18

/**************************************************************************************
                   OPEN_CARI_ODEME_FISI
***************************************************************************************/

void  OPEN_CARI_ODEME_FISI( int fis_id , int p_modul_id,  QWidget * parent )
{
    CARI_ODEME_FISI * F = new CARI_ODEME_FISI ( fis_id, p_modul_id, parent );
    F->SHOW( FULL_SCREEN );    
}

/**************************************************************************************
                   CARI_ODEME_FISI::CARI_ODEME_FISI
***************************************************************************************/

CARI_ODEME_FISI::CARI_ODEME_FISI(int fis_id, int p_modul_id, QWidget * parent) :FIS_KERNEL(parent), m_ui ( new Ui::CARI_ODEME_FISI ),
m_modul_id ( p_modul_id )
{
    m_fis_id = fis_id;

    m_ui->setupUi    ( this );
    START_FIS_KERNEL (this, DB );
 }
/**************************************************************************************
                   CARI_ODEME_FISI::~CARI_ODEME_FISI
***************************************************************************************/
CARI_ODEME_FISI::~CARI_ODEME_FISI()
{
    delete M_ISLEM_DETAYLARI;
}

/**************************************************************************************
                   CARI_ODEME_FISI::SETUP_FORM
***************************************************************************************/

void CARI_ODEME_FISI::SETUP_FORM()
{
    SET_HELP_PAGE    ( "cari-hesap-islemleri_odeme-fisi" );
    SET_PAGE_TITLE   ( tr ( "CARİ - ÖDEME FİŞİ" ) );
    SET_SETTING_NAME ( "CARI_ODEME_FISI" );
    bool is_navigation_screen = false;

    if ( m_modul_id EQ CEKSENET_MODULU ) {
        SET_PAGE_TITLE ( tr ( "CARİ -> ÇEK-SENET ENT. FİŞİ ( ÖDEME )" ) );
        SET_HELP_PAGE    ( "cari-entegrasyon-fisleri" );
        is_navigation_screen = true;

        m_ui->label_makbuz_seri_no->setHidden(true);
        m_ui->lineEdit_belge_seri->setHidden(true);
        m_ui->lineEdit_belge_numarasi->setHidden(true);
        m_ui->buton_kendi_ceklerimiz->setHidden(true);
        m_ui->buton_kendi_senetlerimiz->setHidden(true);
        m_ui->push_button_odeme_makbuzu_yazdir->setHidden(true);
        m_ui->pushButton_detaylar->setHidden(true);
    }

    m_ui->lineedit_ort_gun->setMaxLength(4);

    REGISTER_BUTTONS_WIDGET( m_ui->navigation_button_widget ,false,is_navigation_screen);

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );

    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }


    M_FIS                =   new CARI_FIS_STRUCT;
    M_FIS_SATIRI         =   new CARI_FIS_SATIRI_STRUCT;

    M_FIS->modul_id      = CARI_MODULU;
    M_FIS->program_id    = E9_PROGRAMI;

    M_FIS_DETAYLARI      = new E9_ENT_DETAYLARI_STRUCT;
    M_ISLEM_DETAYLARI    = new E9_ISLEM_DETAYLARI;

    m_ui->lineedit_fis_no->setText("*");
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_fis_no);

    m_ui->label_iptal->setVisible( false );

    m_ui->lineEdit_belge_numarasi->setMaxLength      ( 6 );

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH ( 512 );

    m_ui->lineEdit_belge_seri->setMaxLength(3);

    if( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ){
        m_ui->comboBox_fis_filtresi->setVisible( false );
        m_ui->label_fis_filtresi->setVisible( false );

        m_ui->comboBox_sirket->setVisible( false );
        m_ui->label_sirket->setVisible( false );
    }

    DVZ_FILL_COMBOBOX ( m_ui->comboBox_makbuz_para_birimi );

    SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;

    SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

    m_ui->lineEdit_belge_seri->setText      ( UNITE_BILG.odeme_makbuzu_belge_seri );

    DISABLE_CLEAR_ON_WIDGET ( m_ui->lineEdit_belge_seri );

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_fis_no->setValidator(validator);


    m_temel_para_birimi_kodu   = DVZ_GET_DOVIZ_KODU             ( DVZ_GET_TEMEL_PARA_BIRIMI_ID() );
    m_temel_para_birimi_id     = DVZ_GET_TEMEL_PARA_BIRIMI_ID   ();

    m_ui->commaEdit_kur->SET_PRECISION      ( 4 );
    m_ui->commaEdit_parite->SET_PRECISION   ( 6 );


    REGISTER_TABLE_WIDGET               ( m_ui->tablewidget_fis_satirlari , 19 );
    REGISTER_CHANGER_BUTTON             ( m_ui->pushButton_detaylar            );

    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN  );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN       ( 10 );

    SET_FIS_ORDER_DATABASE            ( "car_fis_satirlari", "order_number", "fis_satiri_id" );


    SET_TABLE_ROW_WIDGETS ( ODEME_TURU_COLUMN               , WIDGET_COMBO_BOX    );
    SET_TABLE_ROW_WIDGETS ( PORTFOY_NO_COLUMN               , WIDGET_SEARCH_EDIT  );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN                 , WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( TUTAR_COLUMN                    , WIDGET_COMMA_EDIT   );
    SET_TABLE_ROW_WIDGETS ( DETAYLAR_COLUMN                 , WIDGET_PUSH_BUTTON  );
    SET_TABLE_ROW_WIDGETS ( HESAP_NO_ID_COLUMN              , WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( CEK_SENET_ID_COLUMN             , WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( KREDI_KARTI_ID_COLUMN           , WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( BANKA_ISMI_COLUMN               , WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( SUBE_KODU_COLUMN                , WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( SUBE_ISMI_COLUMN                , WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( HESAP_NO_COLUMN                 , WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( CARI_PARA_BIRIMI_TUTARI_COLUMN  , WIDGET_COMMA_EDIT   );
    SET_TABLE_ROW_WIDGETS ( TEMEL_PARA_BIRIMI_TUTARI_COLUMN , WIDGET_COMMA_EDIT   );
    SET_TABLE_ROW_WIDGETS ( CARI_PARA_BIRIMI_COLUMN         , WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( MAKBUZ_PARA_BIRIMI_COLUMN       , WIDGET_LINE_EDIT    );

    REGISTER_SAVER_BUTTON  ( m_ui->push_button_odeme_makbuzu_yazdir );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_fis_no );

    SET_LINE_BUTTON_ICON ( QStringList () << "" );

    m_ui->lineedit_fis_no->setMaxLength ( 10);

    m_ui->tablewidget_fis_satirlari->hideColumn ( TEMEL_PARA_BIRIMI_TUTARI_COLUMN );

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels(QStringList() << "id" << "fis_id" << "satir_order"
                                                                             << "Hesap No id" << "cek_senet_id"
                                                                             << "Kredi Karti id" << "" << "" << "" << ""
                                                                             << tr ( "Ödeme Şekli" )
                                                                             << tr ( "Portföy No" )     << tr ( "Açıklama" )
                                                                             << tr ( "Tutarı" ) << ""
                                                                             << tr ( "Cari Para Birimi\n Tutarı" )
                                                                             << tr ( "temel_para_birimi_tutari" )
                                                                             << "" << tr ( "Detaylar" ) << tr("") << tr ("") );

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->horizontalHeaderItem(CARI_PARA_BIRIMI_TUTARI_COLUMN);
    QFont font         = item->font();
    font.setPointSize ( font.pointSize() - 2 );
    item->setFont     ( font );

    m_ui->tablewidget_fis_satirlari->setColumnWidth ( ODEME_TURU_COLUMN                  , 139 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( PORTFOY_NO_COLUMN                  , 100 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( ACIKLAMA_COLUMN                    , 379 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( TUTAR_COLUMN                       , 124 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( DETAYLAR_COLUMN                    , 128 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_PARA_BIRIMI_TUTARI_COLUMN     , 124);
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_PARA_BIRIMI_COLUMN            , 40  );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( MAKBUZ_PARA_BIRIMI_COLUMN          , 40 );

    m_ui->tablewidget_fis_satirlari->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_fis_satirlari->setFocus( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET();

    AUTOSTART_FIS_KERNEL(m_fis_id);
}

/**************************************************************************************
                   CARI_ODEME_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void CARI_ODEME_FISI::CLEAR_FORM_MEMBERS ()
{
    CARI_CLEAR_FIS_STRUCT(M_FIS);

    M_FIS->program_id = E9_PROGRAMI;
    M_FIS->modul_id   = CARI_MODULU;

    CARI_CLEAR_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );


    m_log_kaydi_eklenecek_mi                =   false;
    m_kayit_eklendi                         =   false;
    m_kayit_silindi                         =   false;
    m_cari_hesap_id                         =   0;
    m_makbuz_para_birimi_id                 =   0;
    m_cari_para_birimi_id                   =   0;
    m_doviz_cinsi_id                        =   0;

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
                   CARI_ODEME_FISI::NEW_FIS_RECORD
***************************************************************************************/

void CARI_ODEME_FISI::NEW_FIS_RECORD()
{
    m_ui->label_iptal->setVisible( false );
    m_ui->lineedit_fis_no->setText("*");
    m_ui->label_temel_para_birimi->setText      ( DVZ_GET_TEMEL_PARA_BIRIMI_KODU() );

    m_ui->label_vadesi->setHidden               ( true  );
    m_ui->adakDate_ortalama_vade->setHidden     ( true  );
    m_ui->label_ort_gun->setHidden              ( true  );
    m_ui->lineedit_ort_gun->setHidden           ( true  );
    m_ui->label_temel_para_birimi->setHidden    ( true );
    m_ui->commaEdit_kur->setHidden              ( true );
    m_ui->label_cari_para_birimi->setHidden     ( true );
    m_ui->label_makbuz_para_birimi->setHidden   ( true );
    m_ui->commaEdit_parite->setHidden           ( true );
    m_ui->label_parite->setHidden               ( true );

    M_FIS->program_id          = E9_PROGRAMI;
    M_FIS->modul_id            = CARI_MODULU;


    M_FIS->muh_kasa_hesap_id   = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();
    M_FIS_DETAYLARI->muh_kasa_hesap_id = M_FIS->muh_kasa_hesap_id;
}

/**************************************************************************************
                   CARI_ODEME_FISI::GET_FIS_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::GET_FIS_RECORD ( int fis_id )
{
    m_fis_id = fis_id;

    m_ui->commaEdit_kur->setHidden              ( true );
    m_ui->label_temel_para_birimi->setHidden    ( true );
    m_ui->label_parite->setHidden               ( true );
    m_ui->commaEdit_parite->setHidden           ( true );
    m_ui->label_makbuz_para_birimi->setHidden   ( true );
    m_ui->label_cari_para_birimi->setHidden     ( true );


    CARI_CLEAR_FIS_STRUCT   ( M_FIS );
    CARI_FIS_BILGILERINI_OKU( M_FIS, fis_id );

    m_ui->label_iptal->setVisible( false );
    if ( M_FIS->iptal_mi EQ 1 ) {
        m_ui->label_iptal->setVisible( true );
    }

    M_FIS_DETAYLARI->muh_kasa_hesap_id    = M_FIS->muh_kasa_hesap_id;

    m_cari_hesap_id = M_FIS->cari_hesap_id;

    m_ui->lineedit_fis_no->setText                  ( QVariant ( M_FIS->fis_no ).toString() );
    m_ui->adakDate_fis_tarihi->SET_DATE(M_FIS->fis_tarihi  );
    m_ui->limitedTextEdit_aciklama->setPlainText    ( M_FIS->aciklama );
    m_ui->commaedit_toplam_tutar->SET_DOUBLE        ( M_FIS->toplam_borc );

    m_ui->lineEdit_belge_seri->setText              ( M_FIS->belge_seri );
    m_ui->lineEdit_belge_numarasi->setText          ( M_FIS->belge_numarasi   );

    QString makbuz_doviz_kodu = DVZ_GET_DOVIZ_KODU ( M_FIS->makbuz_para_birimi_id );
    m_makbuz_para_birimi_id   = M_FIS->makbuz_para_birimi_id;
    m_ui->comboBox_makbuz_para_birimi->setCurrentIndex ( m_ui->comboBox_makbuz_para_birimi->findText ( makbuz_doviz_kodu ) );

    m_ui->commaEdit_kur->SET_DOUBLE    ( M_FIS->doviz_kuru );
    m_ui->commaEdit_parite->SET_DOUBLE ( M_FIS->parite     );

    m_ui->label_makbuz_para_birimi->setText ( "1 " + makbuz_doviz_kodu );

    SQL_QUERY select_query(DB);
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("car_hesaplar",
                             "cari_hesap_kodu,adres_id,para_birimi_id,"
                             "toplam_borc_array,toplam_alacak_array",
                             "cari_hesap_id = :cari_hesap_id");

    sql_query.SET_VALUE ( ":cari_hesap_id" , m_cari_hesap_id );

    QString cari_para_birimi_kodu = "";

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        m_ui->searchedit_cari_hesap_kodu->SET_TEXT ( sql_query.VALUE(0).toString() );
        m_ui->lineedit_cari_hesap_ismi->setText    ( ADR_GET_FIRMA_SAHIS_ADI(sql_query.VALUE(1).toInt()) );
        m_cari_para_birimi_id = sql_query.VALUE(2).toInt();
        cari_para_birimi_kodu = DVZ_GET_DOVIZ_KODU(m_cari_para_birimi_id);
        m_ui->lineEdit_cari_para_birimi->setText(cari_para_birimi_kodu);


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
        m_ui->commaEdit_hesap_borc->SET_DOUBLE(borc_bakiye);
        m_ui->commaEdit_hesap_alacak->SET_DOUBLE(alacak_bakiye);

    }

    m_ui->label_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi->text());

   if ( m_makbuz_para_birimi_id EQ m_cari_para_birimi_id  ) {

       if ( m_makbuz_para_birimi_id NE m_temel_para_birimi_id) {
           m_ui->label_temel_para_birimi->setHidden(false);
           m_ui->commaEdit_kur->setHidden(false);
           m_ui->label_temel_para_birimi->setText(DVZ_GET_TEMEL_PARA_BIRIMI_KODU());
           m_ui->commaEdit_parite->clear();
       }
       else {
           m_ui->commaEdit_kur->clear();
           m_ui->commaEdit_parite->clear();
       }
   }
   else {
       m_ui->commaEdit_kur->setHidden(false);
       m_ui->label_temel_para_birimi->setHidden(false);

       if ( m_makbuz_para_birimi_id EQ m_temel_para_birimi_id ) {
           m_ui->label_parite->setHidden(false);
           m_ui->commaEdit_parite->setHidden(false);
           m_ui->label_makbuz_para_birimi->setHidden(false);
           m_ui->label_cari_para_birimi->setHidden(false);
           m_ui->commaEdit_kur->clear();
           m_ui->commaEdit_kur->setHidden(true);
           m_ui->label_temel_para_birimi->setHidden(true);
       }
       else {
           if ( m_cari_para_birimi_id NE m_temel_para_birimi_id ) {


               m_ui->label_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi->text());
               m_ui->label_makbuz_para_birimi->setText("1 " + DVZ_GET_DOVIZ_KODU(m_makbuz_para_birimi_id));
               m_ui->commaEdit_parite->setHidden(false);
               m_ui->label_parite->setHidden(false);
               m_ui->label_cari_para_birimi->setHidden(false);
               m_ui->label_makbuz_para_birimi->setHidden(false);
           }

       }
   }

    select_query.PREPARE_SELECT("car_fis_satirlari",
                                "fis_satiri_id, aciklama, borc_tutari, islem_turu, cek_senet_id, "
                                "temel_para_birimi_tutari,hesap_no_id,kredi_karti_id,musteri_banka_ismi, "
                                "musteri_sube_kodu,musteri_sube_ismi,musteri_hesap_no,cari_para_birimi_tutari  ",
                                "fis_id = :fis_id ");
    select_query.SET_VALUE(":fis_id" , fis_id);

    if (select_query.SELECT( "order_number ASC") EQ 0 ) {
        return ADAK_OK;
    }

    QTableWidgetItem *  new_item;

    while ( select_query.NEXT() EQ true ) {

        int current_row = ADD_NEW_LINE();

        new_item = new QTableWidgetItem ( select_query.VALUE(0).toString() );
        m_ui->tablewidget_fis_satirlari->setItem(current_row,ROW_ID_COLUMN,new_item);

        new_item = new QTableWidgetItem ( QVariant(fis_id).toString() );
        m_ui->tablewidget_fis_satirlari->setItem(current_row,FIS_ID_COLUMN,new_item);

        QLineEdit   * lineedit_aciklama                   = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , ACIKLAMA_COLUMN                 );
        QCommaEdit  * commaedit_tutar                     = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , TUTAR_COLUMN                    );
        QComboBox   * combobox_odeme_turu                 = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , ODEME_TURU_COLUMN               );
        QLineEdit   * lineedit_hesap_no_id                = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , HESAP_NO_ID_COLUMN              );
        QPushButton * push_button                         = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , DETAYLAR_COLUMN                 );
        QLineEdit   * lineedit_kredi_karti_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , KREDI_KARTI_ID_COLUMN           );
        QLineEdit   * lineedit_mus_banka_ismi             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , BANKA_ISMI_COLUMN               );
        QLineEdit   * lineedit_mus_sube_ismi              = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , SUBE_ISMI_COLUMN                );
        QLineEdit   * lineedit_mus_sube_kodu              = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , SUBE_KODU_COLUMN                );
        QLineEdit   * lineedit_mus_hesap_no               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , HESAP_NO_COLUMN                 );
        QCommaEdit  * commaedit_cari_para_birimi_tutari   = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , CARI_PARA_BIRIMI_TUTARI_COLUMN  );
        QCommaEdit  * commaedit_temel_para_birimi_tutari  = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , CARI_PARA_BIRIMI_TUTARI_COLUMN  );
        QLineEdit   * lineedit_cari_para_birimi           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , CARI_PARA_BIRIMI_COLUMN         );
        QLineEdit   * lineedit_makbuz_para_birimi         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , MAKBUZ_PARA_BIRIMI_COLUMN       );
        QLineEdit   * lineedit_cek_senet_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , CEK_SENET_ID_COLUMN);

        QPushButton * push_button_detaylar                = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, DETAYLAR_COLUMN                  );
        QComboBox   * combobox_odeme_sekli                = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, ODEME_TURU_COLUMN                );
        QSearchEdit * searchedit_portfoy_no               = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, PORTFOY_NO_COLUMN                );

        lineedit_cari_para_birimi->setText(cari_para_birimi_kodu);
        lineedit_makbuz_para_birimi->setText(makbuz_doviz_kodu);

        lineedit_aciklama->setText  ( select_query.VALUE(1).toString() );

        commaedit_tutar->SET_DOUBLE ( select_query.VALUE(2).toDouble() );

        combobox_odeme_turu->setCurrentIndex            ( combobox_odeme_turu->findText ( E9_GET_TAH_ODM_SEKLI_STRING(select_query.VALUE(3).toInt(),ENUM_CARI_ODEME_FISI)));
        int cek_senet_id = select_query.VALUE(4).toInt();
        lineedit_cek_senet_id->setText(QVariant(cek_senet_id).toString());

        commaedit_temel_para_birimi_tutari->SET_DOUBLE  ( select_query.VALUE(5).toDouble() );
        lineedit_hesap_no_id->setText                   ( select_query.VALUE(6).toString() );
        lineedit_kredi_karti_id->setText                ( select_query.VALUE(7).toString() );


        lineedit_mus_banka_ismi->setText(select_query.VALUE(8).toString());
        lineedit_mus_sube_kodu->setText(select_query.VALUE(9).toString());
        lineedit_mus_sube_ismi->setText(select_query.VALUE(10).toString());
        lineedit_mus_hesap_no->setText(select_query.VALUE(11).toString());

        commaedit_cari_para_birimi_tutari->SET_DOUBLE(select_query.VALUE(12).toDouble());

        int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_odeme_turu->currentText());

        if ( islem_turu EQ ENUM_MUSTERI_CEKI OR islem_turu EQ ENUM_MUSTERI_SENEDI ) {
            commaedit_tutar->setReadOnly(true);
        }

        push_button->setEnabled(true);

        switch (islem_turu) {
            case ENUM_KREDI_KARTI:
                push_button->setText("Kredi Kartı Detayları");
                break;
            case ENUM_HAVALE :
                push_button->setText("Havale Detayları");
                break;
            case ENUM_EFT:
                push_button->setText("EFT Detayları");
                break;
            case ENUM_MUSTERI_CEKI:
            case ENUM_KENDI_CEKIMIZ:
                push_button->setText("Çek Detayları");
                break;
            case ENUM_MUSTERI_SENEDI:
            case ENUM_KENDI_SENEDIMIZ:
                push_button->setText("Senet Detayları");
                break;
            case ENUM_NAKIT:
                if ( push_button NE NULL ) {
                    push_button->setDisabled(true);
                }
                break;
            default:
                push_button->setText("");
                break;
        }

        if ( cek_senet_id NE 0 ) {

            CEK_SENET_STRUCT CEK_BILG;

            CEK_GET_TEMEL_BILGILER( &CEK_BILG, cek_senet_id );

            QSearchEdit * searchedit_portfoy_no  = ( QSearchEdit *)  m_ui->tablewidget_fis_satirlari->cellWidget(current_row,PORTFOY_NO_COLUMN);
            searchedit_portfoy_no->SET_TEXT( QVariant ( CEK_BILG.portfoy_numarasi ).toString() );
            searchedit_portfoy_no->setEnabled(true);
        }

        if ( m_modul_id EQ CEKSENET_MODULU ) {
            combobox_odeme_sekli->setEnabled( false );
            searchedit_portfoy_no->setEnabled( false );
            push_button_detaylar->setEnabled( false );
        }
    }

    int ortalama_gun = CEK_ORTALAMA_GUN_SAYISI( fis_id, ENUM_CARI_ODEME_FISI );

    if ( ortalama_gun > 0 ) {
        m_ui->lineedit_ort_gun->setHidden       ( false );
        m_ui->label_ort_gun->setHidden          ( false );
        m_ui->label_vadesi->setHidden           ( false );
        m_ui->adakDate_ortalama_vade->setHidden ( false );
        m_ui->lineedit_ort_gun->setText(QVariant(ortalama_gun).toString());

        QDate ortalama_vade_tarihi = QDate::currentDate().addDays(ortalama_gun);
        m_ui->adakDate_ortalama_vade->SET_DATE(ortalama_vade_tarihi);
    }
    else {
        m_ui->lineedit_ort_gun->setHidden       ( true );
        m_ui->label_ort_gun->setHidden          ( true );
        m_ui->label_vadesi->setHidden           ( true );
        m_ui->adakDate_ortalama_vade->setHidden ( true );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void CARI_ODEME_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QPushButton * push_button_detaylar                  = ( QPushButton  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DETAYLAR_COLUMN );
    QComboBox   * combobox_odeme_sekli                  = ( QComboBox    * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ODEME_TURU_COLUMN              );
    QSearchEdit * searchedit_portfoy_no                 = ( QSearchEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PORTFOY_NO_COLUMN              );
    QLineEdit   * lineedit_aciklama                     = ( QLineEdit    * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN                );
    QLineEdit   * lineedit_cari_para_birimi             = ( QLineEdit    * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_PARA_BIRIMI_COLUMN       );
    QLineEdit   * lineedit_makbuz_para_birimi           = ( QLineEdit    * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MAKBUZ_PARA_BIRIMI_COLUMN       );
    QCommaEdit  * commaedit_cari_para_birimi_tutari     = ( QCommaEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_PARA_BIRIMI_TUTARI_COLUMN );



    lineedit_cari_para_birimi->setReadOnly      ( true );
    lineedit_makbuz_para_birimi->setReadOnly    ( true );
    lineedit_cari_para_birimi->setMaxLength     ( 5 );
    lineedit_makbuz_para_birimi->setMaxLength   ( 5 );

    lineedit_makbuz_para_birimi->setText    ( m_ui->comboBox_makbuz_para_birimi->currentText()  );
    lineedit_cari_para_birimi->setText      ( m_ui->lineEdit_cari_para_birimi->text()           );

    combobox_odeme_sekli->addItems ( QStringList()  << tr ( "Nakit" )
                                                    << tr ( "Müşteri Çeki"    )
                                                    << tr ( "Müşteri Senedi"  )
                                                    << tr ( "Kendi Çekimiz"   )
                                                    << tr ( "Kendi Senedimiz" )
                                                    << tr ( "Havale" )
                                                    << tr ( "Eft" )
                                                    << tr ( "Kredi Kartı" ) );

    combobox_odeme_sekli->setCurrentIndex (-1);
    searchedit_portfoy_no->setEnabled     ( false );
    lineedit_aciklama->setMaxLength       ( 128 );
    commaedit_cari_para_birimi_tutari->setDisabled ( true );

    push_button_detaylar->setEnabled      ( false );
    push_button_detaylar->setMinimumWidth ( 150 );

    REGISTER_LINE_CHANGER_BUTTON ( row_number, DETAYLAR_COLUMN );

}

/**************************************************************************************
                   CARI_ODEME_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{

    if ( object EQ m_ui->searchedit_cari_hesap_kodu ) {

        if ( m_ui->searchedit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_cari_hesap_id = -1;
            m_ui->lineedit_cari_hesap_ismi->clear();
            m_ui->commaEdit_hesap_borc->clear();
            m_ui->commaEdit_hesap_alacak->clear();
            return ADAK_OK;
        }

        SQL_QUERY select_query ( DB );

        select_query.PREPARE_SELECT ( "car_hesaplar",
                                      "adres_id,cari_hesap_id,para_birimi_id,"
                                      "toplam_borc_array,toplam_alacak_array" ,
                                      "cari_hesap_kodu = :cari_hesap_kodu");
        select_query.SET_VALUE      ( ":cari_hesap_kodu" , m_ui->searchedit_cari_hesap_kodu->GET_TEXT() );

        if ( select_query.SELECT() EQ 0 ) {
            MSG_WARNING( tr("Aradığınız cari hesap kodu bulunamadı!.."), m_ui->searchedit_cari_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        select_query.NEXT();
        m_ui->lineedit_cari_hesap_ismi->setText(ADR_GET_FIRMA_SAHIS_ADI(select_query.VALUE(0).toInt()));
        m_cari_hesap_id = select_query.VALUE(1).toInt();
        m_cari_para_birimi_id = select_query.VALUE(2).toInt();

        double borc_array[E9_ARRAY_SIZE];
        double alacak_array [E9_ARRAY_SIZE];

        UNPACK_DOUBLE_ARRAY(select_query.VALUE(3).toString() , borc_array   , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(select_query.VALUE(4).toString() , alacak_array , E9_ARRAY_SIZE);

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
        m_ui->commaEdit_hesap_borc->SET_DOUBLE(borc_bakiye);
        m_ui->commaEdit_hesap_alacak->SET_DOUBLE(alacak_bakiye);

        QString cari_para_birimi_kodu = DVZ_GET_DOVIZ_KODU(m_cari_para_birimi_id);
        m_ui->lineEdit_cari_para_birimi->setText( cari_para_birimi_kodu);

        if ( m_ui->comboBox_makbuz_para_birimi->currentIndex() EQ -1 ) {
            m_ui->comboBox_makbuz_para_birimi->setCurrentIndex(m_ui->comboBox_makbuz_para_birimi->findText(cari_para_birimi_kodu));
            m_makbuz_para_birimi_id = m_cari_para_birimi_id;

            if ( m_cari_para_birimi_id NE m_temel_para_birimi_id ) {
                m_ui->label_temel_para_birimi->setHidden(false);
                m_ui->commaEdit_kur->setHidden(false);

                m_ui->commaEdit_kur->SET_DOUBLE(DVZ_DOVIZ_KURUNU_AL(m_makbuz_para_birimi_id,m_ui->adakDate_fis_tarihi->QDATE()));

            }
        }
        if ( ( DVZ_GET_DOVIZ_ID(m_ui->comboBox_makbuz_para_birimi->currentText()) NE m_cari_para_birimi_id ) AND
             ( m_cari_para_birimi_id NE m_temel_para_birimi_id) ) {
            m_ui->commaEdit_parite->setHidden(false);
            m_ui->label_cari_para_birimi->setHidden(false);
            m_ui->label_makbuz_para_birimi->setHidden(false);
            m_ui->commaEdit_parite->SET_DOUBLE(DVZ_PARITEYI_AL(m_cari_para_birimi_id,m_makbuz_para_birimi_id,
                                                           m_ui->adakDate_fis_tarihi->QDATE(),ENUM_SATIS));

            m_ui->label_makbuz_para_birimi->setText( "1 " +  m_ui->comboBox_makbuz_para_birimi->currentText());
            m_ui->label_cari_para_birimi->setText(DVZ_GET_DOVIZ_KODU(m_cari_para_birimi_id));
        }
        else {
            m_ui->label_makbuz_para_birimi->setHidden(true);
            m_ui->label_cari_para_birimi->setHidden(true);
            m_ui->commaEdit_parite->setHidden(true);
            m_ui->label_parite->setHidden(true);
            m_ui->commaEdit_parite->clear();
        }

        EKRANI_GUNCELLE();

    }
    else if ( object EQ m_ui->buton_kendi_ceklerimiz ) {
        return KENDI_CEKLERIMIZ();
    }
    else if ( object EQ m_ui->buton_kendi_senetlerimiz ) {
        return KENDI_SENETLERIMIZ();
    }
    else if ( object EQ m_ui->comboBox_makbuz_para_birimi ) {

        SQL_QUERY sql_query ( DB );
        int fis_id = GET_RECORD_ID();

        if ( fis_id > 0 ) {
            sql_query.PREPARE_SELECT ( "car_fis_satirlari",
                                       "islem_turu",
                                       "fis_id = :fis_id");
            sql_query.SET_VALUE      ( ":fis_id" , fis_id );

            if ( sql_query.SELECT() > 0 ) {
                while ( sql_query.NEXT() EQ true ) {

                    int islem_turu = sql_query.VALUE(0).toInt();
                    if ( islem_turu EQ ENUM_HAVALE OR islem_turu EQ ENUM_EFT ) {
                        MSG_WARNING(  tr ( "Havale/EFT ile ödeme yapılmış.Makbuz para birimini değiştiremezsiniz." ), m_ui->comboBox_makbuz_para_birimi );
                        return ADAK_FAIL;
                    }
                    if ( islem_turu EQ ENUM_KREDI_KARTI ) {
                        MSG_WARNING(  tr ( "Kredi Kartı ile ödeme yapılmış.Makbuz para birimini değiştiremezsiniz." ), m_ui->comboBox_makbuz_para_birimi );
                        return ADAK_FAIL;
                    }
                }
            }
        }
        m_makbuz_para_birimi_id = DVZ_GET_DOVIZ_ID(m_ui->comboBox_makbuz_para_birimi->currentText());

        m_ui->commaEdit_kur->SET_DOUBLE(DVZ_DOVIZ_KURUNU_AL(m_makbuz_para_birimi_id,m_ui->adakDate_fis_tarihi->QDATE(),
                                                        ENUM_SATIS));

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
                                                               m_ui->adakDate_fis_tarihi->QDATE(),ENUM_SATIS));
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
                                                                   m_ui->adakDate_fis_tarihi->QDATE(),ENUM_SATIS));
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
    else if ( object EQ m_ui->lineEdit_belge_seri OR object EQ m_ui->lineEdit_belge_numarasi ) {
       m_ui->lineEdit_belge_seri->setText( m_ui->lineEdit_belge_seri->text().toUpper() );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineedit_fis_no->text().isEmpty() ) {
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
                   CARI_ODEME_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_FIS_RECORD_ADD()
{
    if ( QString( m_ui->lineedit_fis_no->text().at(0)) NE "*" ) {
        if ( CARI_FIS_NO_KULLANILDI_MI( m_ui->lineedit_fis_no->text().toInt(), m_ui->adakDate_fis_tarihi->DATE() ) EQ true ) {
            MSG_WARNING( tr("Aynı numarada başka bir fiş var.Fiş eklenemedi!.."), m_ui->searchedit_cari_hesap_kodu );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::ADD_FIS_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::ADD_FIS_RECORD()
{
    int fis_no = 0;

    if ( QString( m_ui->lineedit_fis_no->text().at(0) ) EQ "*" ) {
        fis_no  = CARI_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE() );
    }
    else {
        fis_no = m_ui->lineedit_fis_no->text().toInt();
    }

    M_FIS->fis_no                   = fis_no;
    M_FIS->fis_tarihi               = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->cari_hesap_id            = m_cari_hesap_id;
    M_FIS->aciklama                 = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->belge_seri               = m_ui->lineEdit_belge_seri->text();
    M_FIS->belge_numarasi           = m_ui->lineEdit_belge_numarasi->text();
    M_FIS->fis_turu                 = ENUM_CARI_ODEME_FISI;
    M_FIS->makbuz_para_birimi_id    = m_makbuz_para_birimi_id;
    M_FIS->doviz_kuru               = m_ui->commaEdit_kur->GET_DOUBLE();
    M_FIS->parite                   = m_ui->commaEdit_parite->GET_DOUBLE();
    M_FIS->muh_kasa_hesap_id        = M_FIS_DETAYLARI->muh_kasa_hesap_id;
    M_FIS->modul_id                 = CARI_MODULU;
    M_FIS->program_id               = E9_PROGRAMI;

    int fis_id = CARI_FIS_EKLE ( M_FIS );

    m_ui->lineedit_fis_no->setText ( QVariant ( fis_no ).toString() );

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_eklendi          = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_ODEME_FISI ) + "," +
                        m_ui->lineEdit_belge_seri->text() + "  " + m_ui->lineEdit_belge_numarasi->text() + "\n" +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                        ",Fiş No : "  + QVariant(fis_no).toString());


    return fis_id;
}

/**************************************************************************************
                   CARI_ODEME_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_FIS_RECORD_UPDATE(int fis_id)
{
    SQL_QUERY sql_query (DB);

    if ( CARI_GET_FIS_YAZDIRILDIMI( fis_id ) EQ ENUM_FIS_YAZILDI ) {
        MSG_WARNING( tr("Fiş Yazdırıldığı için üzerinde Değişiklik yapamazsınız..!"), NULL );
        return ADAK_FAIL;
    }
    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) EQ "*" ) {
        MSG_WARNING(  tr("Fiş numarasını güncellemek için fiş numarası kısmına sadece sayı girebilirsiniz."), m_ui->lineedit_fis_no );

        return ADAK_FAIL;
    }

    sql_query.PREPARE_SELECT ( "car_fisler",
                               "fis_id",
                               "fis_no   = :fis_no "
                               "AND fis_tarihi = :fis_tarihi" );

    sql_query.SET_VALUE      ( ":fis_no",       m_ui->lineedit_fis_no->text().toInt() );
    sql_query.SET_VALUE      ( ":fis_tarihi",   m_ui->adakDate_fis_tarihi->DATE() );

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        if ( sql_query.VALUE(0).toInt() NE fis_id ) {
            MSG_WARNING(  tr ( "Aynı numarada başka bir fiş var.Güncelleme yapılamadı!.." ), m_ui->lineedit_fis_no );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void CARI_ODEME_FISI::UPDATE_FIS_RECORD ( int p_fis_id )
{

    M_FIS->aciklama                 = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->fis_no                   = m_ui->lineedit_fis_no->text().toInt();
    M_FIS->fis_tarihi               = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->cari_hesap_id            = m_cari_hesap_id;
    M_FIS->toplam_borc              = m_ui->commaedit_toplam_tutar->GET_DOUBLE();
    M_FIS->belge_seri               = m_ui->lineEdit_belge_seri->text();
    M_FIS->belge_numarasi           = m_ui->lineEdit_belge_numarasi->text();
    M_FIS->makbuz_para_birimi_id    = m_makbuz_para_birimi_id;
    M_FIS->doviz_kuru               = m_ui->commaEdit_kur->GET_DOUBLE();
    M_FIS->parite                   = m_ui->commaEdit_parite->GET_DOUBLE();
    M_FIS->muh_kasa_hesap_id        = M_FIS_DETAYLARI->muh_kasa_hesap_id;

    CARI_FISI_GUNCELLE              ( M_FIS, p_fis_id );

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari =  ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_ODEME_FISI ) + "," + m_ui->lineEdit_belge_seri->text() + "  "
                       + m_ui->lineEdit_belge_numarasi->text() + "\n" +
                       m_ui->adakDate_fis_tarihi->QDATE().toString() + ",Fiş No : "
                       + m_ui->lineedit_fis_no->text()
                       + ",Toplam Borç : " + VIRGUL_EKLE(QVariant(m_ui->commaedit_toplam_tutar->GET_DOUBLE()).toString()));


    ADD_ADAK_EVENT( ADAK_REFRESH );

}

/**************************************************************************************
                   CARI_ODEME_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_FIS_RECORD_DELETE(int fis_id)
{
    // EGER FIS YAZDIRILMIS ISE SILINEMEZ

    if ( FIS_GUNCELLENEBILIR_MI_SILINEBILIR_MI(fis_id) EQ FALSE ) {
        MSG_WARNING( tr("Fiş satırlarındaki Çek / Senet işlem görmüş,fişi silemezsiniz!.."), NULL );
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
                   CARI_ODEME_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void CARI_ODEME_FISI::DELETE_FIS_RECORD ( int p_fis_id )
{
    CARI_FISI_SIL ( M_FIS, p_fis_id );

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_ODEME_FISI ) + "," +
                       m_ui->lineEdit_belge_seri->text() + " " +
                       m_ui->lineEdit_belge_numarasi->text() + "\n" +
                       m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY")     +
                       ",Fiş No " + m_ui->lineedit_fis_no->text() +
                       ",Toplam Borç : " + VIRGUL_EKLE ( QVariant ( m_ui->commaedit_toplam_tutar->GET_DOUBLE() ).toString() ) );

}

/**************************************************************************************
                   CARI_ODEME_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::SELECT_FIS_RECORD()
{
    return OPEN_CARI_FIS_ARAMA ( ENUM_CARI_ODEME_FISI, m_modul_id, ENUM_CARI_FISI, this );
}

/**************************************************************************************
                   CARI_ODEME_FISI::FIND_FIS_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString query_str = "fis_no = :fis_no "
                        "AND fis_turu = :fis_turu AND modul_id = :modul_id "
                        "AND fis_tarihi = :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler", "fis_id", query_str );

    select_query.SET_VALUE(":fis_no"     , m_ui->lineedit_fis_no->text().toInt());
    select_query.SET_VALUE(":fis_turu"         , ENUM_CARI_ODEME_FISI);
    select_query.SET_VALUE(":modul_id"      , m_modul_id );
    select_query.SET_VALUE(":fis_tarihi"       , m_ui->adakDate_fis_tarihi->DATE());


    if (select_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Aradığınız Ödeme Fişi bulunamadı!.."), m_ui->lineedit_fis_no );

        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_ODEME_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    QString query_str = "fis_turu  = :fis_turu    "
                        "AND modul_id = :modul_id ";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE ( ":fis_turu",    ENUM_CARI_ODEME_FISI    );
    select_query.SET_VALUE ( ":modul_id", m_modul_id           );

    if ( select_query.SELECT( "fis_tarihi, fis_no", 0 , 1 ) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   CARI_ODEME_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString query_str = "fis_turu  = :fis_turu    "
                        "AND modul_id = :modul_id ";

    select_query.PREPARE_SELECT( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE ( ":fis_turu"    , ENUM_CARI_ODEME_FISI   );
    select_query.SET_VALUE ( ":modul_id"    , m_modul_id             );

    if ( select_query.SELECT( "fis_tarihi DESC, fis_no DESC ", 0 , 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_ODEME_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString query_str = "fis_no             < :fis_no "
                        "AND fis_turu       = :fis_turu "
                        "AND modul_id       = :modul_id "
                        "AND fis_tarihi     = :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE ( ":fis_no",       m_ui->lineedit_fis_no->text().toInt() );
    select_query.SET_VALUE ( ":fis_turu",     ENUM_CARI_ODEME_FISI                  );
    select_query.SET_VALUE ( ":modul_id",     m_modul_id                            );
    select_query.SET_VALUE ( ":fis_tarihi",   m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_no DESC", 0, 1 ) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    query_str.clear();
    query_str = "fis_turu           = :fis_turu "
                "AND modul_id       = :modul_id "
                "AND fis_tarihi     < :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE ( ":fis_turu",     ENUM_CARI_ODEME_FISI      );
    select_query.SET_VALUE ( ":modul_id",     m_modul_id                );
    select_query.SET_VALUE ( ":fis_tarihi",   m_ui->adakDate_fis_tarihi->DATE());

    if ( select_query.SELECT( "fis_tarihi DESC, fis_no DESC", 0 , 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_ODEME_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString query_str = "fis_no             > :fis_no "
                        "AND fis_turu       = :fis_turu "
                        "AND modul_id       = :modul_id "
                        "AND fis_tarihi     = :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE ( ":fis_no",         m_ui->lineedit_fis_no->text().toInt()                    );
    select_query.SET_VALUE ( ":fis_turu",       ENUM_CARI_ODEME_FISI                                     );
    select_query.SET_VALUE ( ":modul_id",       m_modul_id                                               );
    select_query.SET_VALUE ( ":fis_tarihi",     m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_no" , 0, 1 ) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    query_str.clear();
    query_str = "fis_turu           = :fis_turu "
                "AND modul_id       = :modul_id "
                "AND fis_tarihi     > :fis_tarihi";

    select_query.PREPARE_SELECT ( "car_fisler","fis_id", query_str );

    select_query.SET_VALUE(":fis_turu"     , ENUM_CARI_ODEME_FISI            );
    select_query.SET_VALUE(":modul_id"  , m_modul_id                   );
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_tarihi, fis_no ASC ", 0, 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   CARI_ODEME_FISI::CHECK_LINE_VAR
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{
    if ( m_modul_id EQ CEKSENET_MODULU ) {
        MSG_WARNING(  tr( " Fiş Çek Senet modülünen üretilmiş. Değişiklik yapamazsınız."), NULL );
        return ADAK_FAIL;
    }

    QComboBox   * combobox_odeme_sekli                = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ODEME_TURU_COLUMN               );
    QSearchEdit * searchedit_portfoy_no               = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PORTFOY_NO_COLUMN               );
    QCommaEdit  * commaedit_tutar                     = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TUTAR_COLUMN                    );
    QLineEdit   * lineedit_aciklama                   = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN                 );
    QLineEdit   * lineedit_cek_senet_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CEK_SENET_ID_COLUMN             );
    QPushButton * push_button_detaylar                = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DETAYLAR_COLUMN                 );
    QCommaEdit  * commaedit_cari_para_birimi_tutari   = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_PARA_BIRIMI_TUTARI_COLUMN  );
    QCommaEdit  * commaedit_temel_para_birimi_tutari  = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TEMEL_PARA_BIRIMI_TUTARI_COLUMN );

    int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_odeme_sekli->currentText());

    if ( object EQ combobox_odeme_sekli) {

        push_button_detaylar->setEnabled(true);
        switch (islem_turu) {
            case ENUM_NAKIT          :
                searchedit_portfoy_no->SET_TEXT("");
                searchedit_portfoy_no->setDisabled(true);
                lineedit_aciklama->clear();
                commaedit_tutar->clear();
                push_button_detaylar->setEnabled(false);
                push_button_detaylar->setText("");
                lineedit_cek_senet_id->clear();
                push_button_detaylar->setDisabled(true);
                break;
            case ENUM_MUSTERI_CEKI   :
            case ENUM_KENDI_CEKIMIZ  :
                m_ui->lineedit_ort_gun->clear();
                lineedit_aciklama->clear();
                searchedit_portfoy_no->SET_TEXT("");
                searchedit_portfoy_no->setEnabled(true);

                commaedit_tutar->clear();
                push_button_detaylar->setText("Çek Detayları");
                lineedit_cek_senet_id->clear();

                break;
            case ENUM_MUSTERI_SENEDI  :
            case ENUM_KENDI_SENEDIMIZ :
                searchedit_portfoy_no->SET_TEXT("");
                searchedit_portfoy_no->setEnabled(true);
                commaedit_tutar->clear();
                lineedit_aciklama->clear();
                m_ui->lineedit_ort_gun->clear();
                push_button_detaylar->setText("Senet Detayları");
                lineedit_cek_senet_id->clear();
                break;
            case ENUM_HAVALE     :
            case ENUM_EFT        :
            case ENUM_KREDI_KARTI:
                searchedit_portfoy_no->SET_TEXT("");
                searchedit_portfoy_no->setEnabled(false);
                lineedit_aciklama->clear();
                commaedit_tutar->clear();
                lineedit_cek_senet_id->clear();
                if ( islem_turu EQ ENUM_HAVALE ) {
                    push_button_detaylar->setText("Havale Detayları");
                }
                if ( islem_turu EQ ENUM_EFT ) {
                    push_button_detaylar->setText("EFT Detayları");
                }
                if ( islem_turu EQ ENUM_KREDI_KARTI ) {
                    push_button_detaylar->setText("Kredi Kartı Detayları");
                }
                push_button_detaylar->setEnabled(true);
                break;
        };

        M_ISLEM_DETAYLARI->bnk_hesap_no_id   =   0;
        M_ISLEM_DETAYLARI->kredi_karti_id    =   0;
        M_ISLEM_DETAYLARI->banka_ismi.clear();
        M_ISLEM_DETAYLARI->sube_kodu.clear();
        M_ISLEM_DETAYLARI->sube_adi.clear();
        M_ISLEM_DETAYLARI->hesap_no.clear();
        commaedit_cari_para_birimi_tutari->clear();
        if ( islem_turu EQ ENUM_MUSTERI_CEKI OR islem_turu EQ ENUM_MUSTERI_SENEDI ) {
            commaedit_tutar->setReadOnly(true);
        }
    }

    else if ( object EQ searchedit_portfoy_no ) {

        if ( searchedit_portfoy_no->GET_TEXT().isEmpty() EQ true ) {
            lineedit_cek_senet_id->clear();
            return ADAK_OK;
        }

        if ( combobox_odeme_sekli->currentIndex() EQ -1 ) {
            MSG_WARNING( tr("Önce ödeme türünü seçmelisiniz!.."), combobox_odeme_sekli );

            return ADAK_FAIL;
        }
        int odeme_sekli = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_odeme_sekli->currentText() );

        if ( odeme_sekli EQ ENUM_MUSTERI_CEKI OR odeme_sekli EQ ENUM_MUSTERI_SENEDI ) {
            if ( m_ui->searchedit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
                MSG_WARNING( tr("Önce cari hesap kodunu seçmelisiniz."), m_ui->searchedit_cari_hesap_kodu );

                return ADAK_FAIL;
            }
        }
        switch (odeme_sekli) {
            case ENUM_NAKIT :
            case ENUM_KREDI_KARTI:
            case ENUM_HAVALE:
            case ENUM_EFT:
                return ADAK_OK;
            default :
                break;
        }

        SQL_QUERY select_query ( DB );

        int cek_senet_id = CEK_GET_CEK_SENET_ID(searchedit_portfoy_no->GET_TEXT().toInt());

        if ( cek_senet_id EQ 0 ) {
            if ( odeme_sekli EQ ENUM_MUSTERI_CEKI OR odeme_sekli EQ ENUM_KENDI_CEKIMIZ ) {
                MSG_WARNING(  tr ( "Aradığınız Çek Bulunamadı."), searchedit_portfoy_no );

            }
            else {
                MSG_WARNING(  tr ( "Aradığınız Senet Bulunamadı."), searchedit_portfoy_no );

            }
            return ADAK_FAIL_UNDO;
        }

        lineedit_cek_senet_id->setText ( QVariant(cek_senet_id).toString() );

        if ( odeme_sekli EQ ENUM_MUSTERI_CEKI OR odeme_sekli EQ ENUM_MUSTERI_SENEDI ) {

            if ( CEK_CARI_CEK_SENET_ALINABILIR_MI( cek_senet_id, m_cari_hesap_id ) EQ false ) {
                MSG_WARNING( tr("Aradığınız çek/senet fişteki cari hesaba aittir.Ödeme işleminde kullanamazsınız!.. "), lineedit_cek_senet_id );
                return ADAK_FAIL;
            }
        }

       int musteri_ceki_mi = -1;

       if ( odeme_sekli EQ ENUM_MUSTERI_CEKI OR odeme_sekli EQ ENUM_MUSTERI_SENEDI ) {
            musteri_ceki_mi = 1;
       }
       else {
           musteri_ceki_mi  = 0;
       }

       int cek_senet_turu = -1;

       if ( odeme_sekli EQ ENUM_MUSTERI_CEKI OR odeme_sekli EQ ENUM_KENDI_CEKIMIZ ) {
            cek_senet_turu = ENUM_CEK;
       }
       else {
            cek_senet_turu = ENUM_SENET;
       }

       double  cek_tutari = CEK_GET_CEK_SENET_TUTARI(lineedit_cek_senet_id->text().toInt(),musteri_ceki_mi,
                                ENUM_PORTFOYDE , cek_senet_turu);

       if ( cek_tutari EQ 0 ) {
           searchedit_portfoy_no->SET_TEXT("");
           MSG_WARNING( tr("Aradığınız Çek portföyde bulunamadı!.."), searchedit_portfoy_no );

           return ADAK_FAIL_UNDO;
       }

       commaedit_tutar->SET_DOUBLE(cek_tutari);

       commaedit_cari_para_birimi_tutari->SET_DOUBLE( CARI_GET_PARA_BIRIMI_TUTARI(m_cari_para_birimi_id,
                                                                                 m_makbuz_para_birimi_id ,
                                                                                 m_ui->commaEdit_kur->GET_DOUBLE(),
                                                                                 m_ui->commaEdit_parite->GET_DOUBLE(),
                                                                                 cek_tutari));

       m_ui->tablewidget_fis_satirlari->setCurrentCell(row_number,ACIKLAMA_COLUMN);

       if (lineedit_cek_senet_id->text().toInt() EQ 0 ) {
           push_button_detaylar->setDisabled(true);
       }
       else {
           push_button_detaylar->setEnabled(true);
       }
    }

    else if (  object EQ commaedit_tutar  ) {

        int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_odeme_sekli->currentText());

        switch ( islem_turu ) {
            case ENUM_NAKIT       :
            case ENUM_HAVALE      :
            case ENUM_EFT         :
            case ENUM_KREDI_KARTI :
            {
                double tutar = commaedit_tutar->GET_DOUBLE();

                double kur = m_ui->commaEdit_kur->GET_DOUBLE();

                if ( kur EQ 0.0 ) {
                    commaedit_temel_para_birimi_tutari->SET_DOUBLE ( tutar );

                }
                else {
                    commaedit_temel_para_birimi_tutari->SET_DOUBLE ( kur * tutar );
                }
                double cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI ( m_cari_para_birimi_id, m_makbuz_para_birimi_id,
                                                                               kur, m_ui->commaEdit_parite->GET_DOUBLE(), tutar );

                commaedit_cari_para_birimi_tutari->SET_DOUBLE ( cari_para_birimi_tutari );
            }
            break;

            case ENUM_KENDI_CEKIMIZ    :
            case ENUM_KENDI_SENEDIMIZ  :
                {
                   int cek_senet_id = lineedit_cek_senet_id->text().toInt();

                   DB->START_TRANSACTION();

                   CEK_UPDATE_CEK_TUTARI( cek_senet_id, commaedit_tutar->GET_DOUBLE() );

                   DB->COMMIT_TRANSACTION();
                }
                break;

        default :
            break;

        }

        double max_nakit_tutari = 0.00;
        double islem_tutar      = ROUND( commaedit_temel_para_birimi_tutari->GET_DOUBLE() );


        if ( islem_turu EQ ENUM_NAKIT ) {

            SQL_QUERY query( DB );

            query.PREPARE_SELECT( "e9_sabit_degerler", "max_nakit_giris_cikis_tutari","sabit_deger_id = :sabit_deger_id" );
            query.SET_VALUE     ( ":sabit_deger_id", 1 );

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                max_nakit_tutari    = query.VALUE( 0 ).toDouble();
            }

            if ( M_FIS->toplam_alacak + islem_tutar > max_nakit_tutari OR M_FIS->toplam_borc + islem_tutar > max_nakit_tutari ) {
                int msg_secim   = MSG_YES_NO( tr( "Vergi Usul Kanununca belirlenen tahsilat ve ödeme sınırlarını aştınız. \n "
                                                  "İşlemede Devam etmek istermisiniz ?" ), NULL );

                if ( msg_secim EQ ADAK_NO ) {
                    return ADAK_FAIL_UNDO;
                }
             }
       }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_LINE_EMPTY(int row_number)
{

    QComboBox   * combobox_odeme_sekli    = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ODEME_TURU_COLUMN);
    QCommaEdit  * commaedit_satir_tutari  = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,TUTAR_COLUMN);
    QSearchEdit * searchedit_portfoy_no   = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,PORTFOY_NO_COLUMN);

    if ( combobox_odeme_sekli->currentIndex() EQ -1) {
        MSG_WARNING( tr("Ödeme Şeklini Seçmelisiniz!.."), combobox_odeme_sekli );
        return ADAK_FAIL;
    }

    int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_odeme_sekli->currentText());

    if ( islem_turu EQ ENUM_KENDI_CEKIMIZ OR islem_turu EQ ENUM_KENDI_SENEDIMIZ OR islem_turu EQ ENUM_MUSTERI_CEKI
         OR islem_turu EQ ENUM_MUSTERI_SENEDI ) {
        if (searchedit_portfoy_no->GET_TEXT() EQ "") {
            MSG_WARNING( tr("Portföyden Çek / Senet seçimi yapmalısınız!.."), searchedit_portfoy_no );

            return ADAK_FAIL;
        }
    }

    if ( commaedit_satir_tutari->GET_DOUBLE() EQ 0 ) {
        MSG_WARNING( tr("Ödeme Tutarını girmelisiniz!.."), commaedit_satir_tutari );

        return ADAK_FAIL;
    }


    return ADAK_OK;
}


/**************************************************************************************
                   CARI_ODEME_FISI::CHECK_ADD_LINE
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_ADD_LINE(int fis_id, int row_number)
{
    Q_UNUSED(fis_id);

    if ( CARI_GET_FIS_YAZDIRILDIMI( fis_id ) EQ ENUM_FIS_YAZILDI ) {
        MSG_WARNING( tr("Fiş Yazdırıldığı için üzerinde Değişiklik yapamazsınız..!"), NULL );
        return ADAK_FAIL;
    }

    QComboBox * combo_box = (QComboBox *)m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ODEME_TURU_COLUMN);
    int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combo_box->currentText());

    int satir_id = -1;

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN);
    if (item NE NULL) {
        satir_id = item->text().toInt();
    }

    switch (islem_turu) {
        case ENUM_KREDI_KARTI :
        {
            QLineEdit * lineedit_kredi_karti_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KREDI_KARTI_ID_COLUMN);
            QLineEdit * lineedit_bnk_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN );

            if (lineedit_kredi_karti_id->text().toInt() EQ 0 ) {
                if ( OPEN_E9_KK_ODEME_BATCH(satir_id,M_ISLEM_DETAYLARI,CARI_MODULU,this ) EQ false ) {
                    return ADAK_FAIL;
                }
                lineedit_kredi_karti_id->setText(QVariant(M_ISLEM_DETAYLARI->kredi_karti_id).toString());
                lineedit_bnk_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());

                if (lineedit_kredi_karti_id->text().toInt() EQ 0 ) {
                    return ADAK_FAIL;
                }
            }
        }
            break;
        case ENUM_HAVALE   :
        case ENUM_EFT      :
        {
            QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, HESAP_NO_ID_COLUMN);
            QLineEdit * lineedit_banka_ismi  = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, BANKA_ISMI_COLUMN );
            QLineEdit * lineedit_sube_kodu   = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, SUBE_KODU_COLUMN  );
            QLineEdit * lineedit_sube_ismi   = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, SUBE_ISMI_COLUMN  );
            QLineEdit * lineedit_hesap_no    = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_COLUMN  );

            if (lineedit_hesap_no_id->text().toInt() EQ 0 ) {

                if ( OPEN_E9_EFT_BATCH(ENUM_CARI_ODEME_FISI ,islem_turu , satir_id ,
                                         m_makbuz_para_birimi_id, M_ISLEM_DETAYLARI ,
                                         CARI_MODULU ,this) EQ false ) {
                    return ADAK_FAIL;
                }
                lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());
                lineedit_banka_ismi->setText(M_ISLEM_DETAYLARI->banka_ismi);
                lineedit_sube_kodu->setText(M_ISLEM_DETAYLARI->sube_kodu);
                lineedit_sube_ismi->setText(M_ISLEM_DETAYLARI->sube_adi);
                lineedit_hesap_no->setText(M_ISLEM_DETAYLARI->hesap_no);

                if (lineedit_hesap_no_id->text().toInt() EQ 0 ) {
                    return ADAK_FAIL;
                }
            }
        }
            break;
        default :
                break;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::ADD_LINE
***************************************************************************************/


void CARI_ODEME_FISI::ADD_LINE ( int p_fis_id, int p_row_number )
{
    QLineEdit   * lineEdit_cek_senet_id                 = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, CEK_SENET_ID_COLUMN             );
    QLineEdit   * lineedit_aciklama                     = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, ACIKLAMA_COLUMN                 );
    QCommaEdit  * commaedit_tutar                       = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, TUTAR_COLUMN                    );
    QComboBox   * combobox_odeme_turu                   = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, ODEME_TURU_COLUMN               );
    QLineEdit   * lineedit_hesap_no_id                  = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, HESAP_NO_ID_COLUMN              );
    QLineEdit   * lineedit_kk_id                        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, KREDI_KARTI_ID_COLUMN           );
    QLineEdit   * lineedit_musteri_banka_ismi           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, BANKA_ISMI_COLUMN               );
    QLineEdit   * lineedit_musteri_sube_ismi            = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, SUBE_ISMI_COLUMN                );
    QLineEdit   * lineedit_musteri_sube_kodu            = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, SUBE_KODU_COLUMN                );
    QLineEdit   * lineedit_musteri_hesap_no             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, HESAP_NO_COLUMN                 );

    CARI_CLEAR_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );

    M_FIS_SATIRI->fis_id                    = p_fis_id;
    M_FIS_SATIRI->aciklama                  = lineedit_aciklama->text();
    M_FIS_SATIRI->borc_tutari               = commaedit_tutar->GET_DOUBLE();
    M_FIS_SATIRI->islem_turu                = E9_GET_TAH_ODM_SEKLI_ENUM (combobox_odeme_turu->currentText() );
    M_FIS_SATIRI->cek_senet_id              = lineEdit_cek_senet_id->text().toInt();
    M_FIS_SATIRI->order_number              = m_ui->tablewidget_fis_satirlari->item ( p_row_number, ORDER_COLUMN )->text().toInt();
    M_FIS_SATIRI->doviz_id                  = m_makbuz_para_birimi_id;
    M_FIS_SATIRI->hesap_no_id               = lineedit_hesap_no_id->text().toInt();
    M_FIS_SATIRI->kredi_karti_id            = lineedit_kk_id->text().toInt();
    M_FIS_SATIRI->musteri_banka_ismi        = lineedit_musteri_banka_ismi->text();
    M_FIS_SATIRI->musteri_sube_kodu         = lineedit_musteri_sube_kodu->text();
    M_FIS_SATIRI->musteri_sube_ismi         = lineedit_musteri_sube_ismi->text();
    M_FIS_SATIRI->musteri_hesap_no          = lineedit_musteri_hesap_no->text();
    M_FIS_SATIRI->modul_id                  = M_FIS->modul_id;
    M_FIS_SATIRI->program_id                = M_FIS->program_id;

    int fis_satiri_id = CARI_FIS_SATIRI_EKLE ( M_FIS, M_FIS_SATIRI );

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( fis_satiri_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( p_row_number, ROW_ID_COLUMN, new_item );

    new_item                   = new QTableWidgetItem ( QVariant ( p_fis_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( p_row_number, FIS_ID_COLUMN, new_item );

    int ortalama_gun = CEK_ORTALAMA_GUN_SAYISI( p_fis_id, ENUM_CARI_ODEME_FISI );

    if ( ortalama_gun > 0 ) {
        m_ui->label_vadesi->setHidden           ( false );
        m_ui->adakDate_ortalama_vade->setHidden ( false );
        m_ui->label_ort_gun->setHidden          ( false );
        m_ui->lineedit_ort_gun->setHidden       ( false );

        m_ui->lineedit_ort_gun->setText         ( QVariant ( ortalama_gun ).toString() );
        m_ui->adakDate_ortalama_vade->SET_DATE( QDate::currentDate().addDays ( ortalama_gun ) );
    }

    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_FIS->toplam_borc );

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_ODEME_FISI ) + "," +
                        m_ui->lineEdit_belge_seri->text() + " "  +
                        m_ui->lineEdit_belge_numarasi->text()+ "\n" +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                        ",Fiş No : " + m_ui->lineedit_fis_no->text() +
                        ",Toplam Borç : " + VIRGUL_EKLE ( QVariant ( m_ui->commaedit_toplam_tutar->GET_DOUBLE() ).toString() ) );

}

/**************************************************************************************
                   CARI_ODEME_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_UPDATE_LINE ( int fis_id, int row_number )
{

    if ( CARI_GET_FIS_YAZDIRILDIMI( fis_id ) EQ ENUM_FIS_YAZILDI ) {
        MSG_WARNING( tr("Fiş Yazdırıldığı için üzerinde Değişiklik yapamazsınız..!"), NULL );
        return ADAK_FAIL;
    }

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("car_fis_satirlari",
                                "islem_turu, cek_senet_id ",
                                "fis_satiri_id = :fis_satiri_id");
    select_query.SET_VALUE(":fis_satiri_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());

    if (select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        int odeme_sekli    = select_query.VALUE(0).toInt();
        int cek_senet_id   = select_query.VALUE(1).toInt();

        if ( odeme_sekli EQ ENUM_MUSTERI_CEKI ) {
            if ( CEK_SON_CEK_SENET_HAREKETINI_BUL ( cek_senet_id ) NE ENUM_SATICIYA_VERILDI ) {
                MSG_WARNING( tr("Fiş satırlarındaki müşteri çeki işlem görmüş,satırı silemezsiniz!.."), NULL );
                return ADAK_FAIL;
            }
        }
        if ( odeme_sekli EQ ENUM_MUSTERI_SENEDI ) {
            if ( CEK_SON_CEK_SENET_HAREKETINI_BUL ( cek_senet_id ) NE ENUM_SATICIYA_VERILDI ) {
                MSG_WARNING ( tr ( "Fiş satırlarındaki müşteri senedi işlem görmüş,satırı silemezsiniz!.."), NULL );
                return ADAK_FAIL;
            }
        }

    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::UPDATE_LINE
***************************************************************************************/

void CARI_ODEME_FISI::UPDATE_LINE ( int fis_id, int row_number )
{
    QCommaEdit  * commaedit_new_tutar                   = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TUTAR_COLUMN                    );
    QLineEdit   * lineedit_aciklama                     = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN                 );
    QComboBox   * combobox_odeme_turu                   = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ODEME_TURU_COLUMN               );
    QLineEdit   * lineedit_hesap_no_id                  = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_NO_ID_COLUMN              );
    QLineEdit   * lineedit_kk_id                        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KREDI_KARTI_ID_COLUMN           );
    QLineEdit   * lineedit_banka_ismi                   = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BANKA_ISMI_COLUMN               );
    QLineEdit   * lineedit_sube_kodu                    = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_KODU_COLUMN                );
    QLineEdit   * lineedit_sube_ismi                    = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SUBE_ISMI_COLUMN                );
    QLineEdit   * lineedit_hesap_no                     = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_NO_COLUMN                 );
    QLineEdit   * lineEdit_cek_senet_id                 = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CEK_SENET_ID_COLUMN             );

    int fis_satiri_id  = m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt();
    int new_islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_odeme_turu->currentText() );

    CARI_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    CARI_FIS_SATIRINI_OKU(M_FIS_SATIRI , fis_satiri_id);

    M_FIS_SATIRI->fis_id                    = fis_id;
    M_FIS_SATIRI->cek_senet_id              = lineEdit_cek_senet_id->text().toInt();
    M_FIS_SATIRI->borc_tutari               = commaedit_new_tutar->GET_DOUBLE();
    M_FIS_SATIRI->islem_turu                = new_islem_turu;
    M_FIS_SATIRI->doviz_id                  = m_makbuz_para_birimi_id;
    M_FIS_SATIRI->hesap_no_id               = lineedit_hesap_no_id->text().toInt();
    M_FIS_SATIRI->kredi_karti_id            = lineedit_kk_id->text().toInt();
    M_FIS_SATIRI->musteri_banka_ismi        = lineedit_banka_ismi->text();
    M_FIS_SATIRI->musteri_sube_kodu         = lineedit_sube_kodu->text();
    M_FIS_SATIRI->musteri_sube_ismi         = lineedit_sube_ismi->text();
    M_FIS_SATIRI->musteri_hesap_no          = lineedit_hesap_no->text();
    M_FIS_SATIRI->aciklama                  = lineedit_aciklama->text();
    M_FIS_SATIRI->modul_id                  = M_FIS->modul_id;
    M_FIS_SATIRI->program_id                = M_FIS->program_id;


    CARI_FIS_BILGILERINI_OKU( M_FIS,  fis_id );
    CARI_FIS_SATIRI_SIL ( M_FIS,fis_satiri_id);

    int new_satir_id = CARI_FIS_SATIRI_EKLE(M_FIS , M_FIS_SATIRI);

    if( new_satir_id NE 0 )
    {
        QTableWidgetItem * new_item;
        new_item = new QTableWidgetItem( QVariant ( new_satir_id ).toString());
        m_ui->tablewidget_fis_satirlari->setItem(row_number,ROW_ID_COLUMN,new_item);
    }

    switch ( new_islem_turu ) {

        case ENUM_KENDI_CEKIMIZ     :
        case ENUM_KENDI_SENEDIMIZ   :
        case ENUM_MUSTERI_CEKI      :
        case ENUM_MUSTERI_SENEDI    :
        {
            int ortalama_gun = CEK_ORTALAMA_GUN_SAYISI( fis_id, ENUM_CARI_ODEME_FISI );

            m_ui->lineedit_ort_gun->setText         ( QVariant ( ortalama_gun ).toString() );
            m_ui->adakDate_ortalama_vade->SET_DATE( QDate::currentDate().addDays ( ortalama_gun ) );
        }
            break;

        default :
            break;
    }

    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_FIS->toplam_borc );

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_ODEME_FISI ) + "," +
                        m_ui->lineEdit_belge_seri->text() + " " +
                        m_ui->lineEdit_belge_numarasi->text() + "\n" +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                        ",Fiş No : "+ m_ui->lineedit_fis_no->text() +
                        ",Toplam Borç : " + VIRGUL_EKLE ( QVariant ( m_ui->commaedit_toplam_tutar->GET_DOUBLE() ).toString() ) );

}

/**************************************************************************************
                   CARI_ODEME_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int CARI_ODEME_FISI::CHECK_DELETE_LINE(int fis_id, int row_number)
{
    if ( CARI_GET_FIS_YAZDIRILDIMI( fis_id ) EQ ENUM_FIS_YAZILDI ) {
        MSG_WARNING( tr("Fiş Yazdırıldığı için üzerinde Değişiklik yapamazsınız..!"), NULL );
        return ADAK_FAIL;
    }

    QWidget * cell_widget = m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ODEME_TURU_COLUMN);
    QComboBox * combobox_odeme_turu = static_cast <QComboBox*> (cell_widget);

    if ( E9_GET_TAH_ODM_SEKLI_ENUM(combobox_odeme_turu->currentText()) EQ ENUM_NAKIT ) {
        return ADAK_OK;
    }

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("car_fis_satirlari",
                                "islem_turu,cek_senet_id",
                                "fis_satiri_id = :fis_satiri_id");
    select_query.SET_VALUE(":fis_satiri_id" , m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt());

    if (select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        int odeme_sekli    = select_query.VALUE(0).toInt();
        int cek_senet_id   = select_query.VALUE(1).toInt();


        if (odeme_sekli EQ ENUM_MUSTERI_CEKI) {
            if (CEK_SON_CEK_SENET_HAREKETINI_BUL(cek_senet_id) NE ENUM_SATICIYA_VERILDI) {
                MSG_WARNING( tr("Fiş satırlarındaki müşteri çeki işlem görmüş,satırı silemezsiniz!.."), NULL );
                return ADAK_FAIL;
            }
        }
        if (odeme_sekli EQ ENUM_MUSTERI_SENEDI) {
            if (CEK_SON_CEK_SENET_HAREKETINI_BUL(cek_senet_id) NE ENUM_SATICIYA_VERILDI) {
                MSG_WARNING( tr("Fiş satırlarındaki müşteri senedi işlem görmüş,satırı silemezsiniz!.."), NULL );
                return ADAK_FAIL;
            }
        }

    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::DELETE_LINE
***************************************************************************************/

void CARI_ODEME_FISI::DELETE_LINE ( int fis_id, int row_number )
{
    int    fis_satiri_id   = m_ui->tablewidget_fis_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt();

    M_FIS_SATIRI->fis_id   = fis_id;

    CARI_FIS_SATIRI_SIL ( M_FIS, fis_satiri_id );

    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_FIS->toplam_borc );

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_ODEME_FISI ) + "," +
                        m_ui->lineEdit_belge_seri->text() + " " +
                        m_ui->lineEdit_belge_numarasi->text() + "\n" +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                        ",Fiş No : " + m_ui->lineedit_fis_no->text() +
                        ",Toplam Borç : " + VIRGUL_EKLE ( QVariant ( m_ui->commaedit_toplam_tutar->GET_DOUBLE() ).toString() ) );
}

/**************************************************************************************
                   CARI_ODEME_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::LOCK_FIS_RECORD(int fis_id)
{
    return DB->LOCK_ROW( "fis_id","car_fisler", QString ( "fis_id = %1" ).arg ( fis_id ));
}

/**************************************************************************************
                   CARI_ODEME_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void CARI_ODEME_FISI::UNLOCK_FIS_RECORD(int fis_id)
{
    DB->UNLOCK_ROW( "fis_id","car_fisler", QString ( "fis_id = %1" ).arg ( fis_id ));
}


/**************************************************************************************
                   CARI_ODEME_FISI::SELECTED_LINE
***************************************************************************************/

void CARI_ODEME_FISI::SELECTED_LINE ( int row_number , int row_save_status )
{
    Q_UNUSED(row_save_status);
    Q_UNUSED(row_number);
}

/**************************************************************************************
                   CARI_ODEME_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/


void  CARI_ODEME_FISI::LINE_SEARCH_EDIT_CLICKED (int row_number,QWidget * ,QLineEdit *lineedit )
{
    QWidget  * cell_widget ;

    cell_widget                          = m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ODEME_TURU_COLUMN);
    QComboBox * combobox_odeme_sekli     = static_cast <QComboBox*> (cell_widget);

    if (combobox_odeme_sekli->currentIndex() EQ -1 ) {
        MSG_WARNING( tr("Önce Ödeme Şeklini seçmelisiniz!.."), NULL);
        return;
    }
    int odeme_sekli = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_odeme_sekli->currentText());

    if ( odeme_sekli EQ ENUM_MUSTERI_CEKI OR odeme_sekli EQ ENUM_MUSTERI_SENEDI ) {

        if (m_ui->searchedit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Önce cari hesap ismini seçmelisiniz!.."), NULL );
            return;
        }
    }

    if ( odeme_sekli EQ ENUM_MUSTERI_CEKI OR odeme_sekli EQ ENUM_KENDI_CEKIMIZ) {

        int cek_id = 0;

        if (odeme_sekli EQ ENUM_MUSTERI_CEKI) {
            cek_id = OPEN_CEK_SENET_ARAMA ( ENUM_MUSTERI_CEKI , ENUM_CEK , true , m_cari_hesap_id , m_makbuz_para_birimi_id ,this );
        }
        else {
            cek_id = OPEN_CEK_SENET_ARAMA ( ENUM_KENDI_CEKIMIZ , ENUM_CEK , false , 0 , m_makbuz_para_birimi_id ,this);
        }

        if (cek_id EQ -1) {
            lineedit->setText("");
            return;
        }      
        CEK_SENET_STRUCT CEK_BILG;

        CEK_GET_TEMEL_BILGILER( &CEK_BILG, cek_id );

        lineedit->setText( QVariant ( CEK_BILG.portfoy_numarasi ).toString() );

    }

    if ( odeme_sekli EQ ENUM_MUSTERI_SENEDI OR odeme_sekli EQ ENUM_KENDI_SENEDIMIZ ) {
        int senet_id;

        if (odeme_sekli EQ ENUM_MUSTERI_SENEDI ) {
            senet_id = OPEN_CEK_SENET_ARAMA(ENUM_MUSTERI_SENEDI,ENUM_SENET,true, m_cari_hesap_id, m_makbuz_para_birimi_id, this);
        }
        else {
            senet_id = OPEN_CEK_SENET_ARAMA(ENUM_KENDI_SENEDIMIZ,ENUM_SENET,false,1, m_makbuz_para_birimi_id,this);
        }

        if (senet_id < 0) {
            lineedit->setText("");
            return;
        }
        CEK_SENET_STRUCT CEK_BILG;

        CEK_GET_TEMEL_BILGILER( &CEK_BILG, senet_id );

        lineedit->setText( QVariant ( CEK_BILG.portfoy_numarasi ).toString() );
    }
}

/**************************************************************************************
                   CARI_ODEME_FISI::FIS_GUNCELLENEBILIR_MI_SILINEBILIR_MI
***************************************************************************************/

bool CARI_ODEME_FISI::FIS_GUNCELLENEBILIR_MI_SILINEBILIR_MI(int fis_id)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("car_fis_satirlari",
                                "islem_turu,cek_senet_id",
                                "fis_id = :fis_id");
    select_query.SET_VALUE(":fis_id" , fis_id);

    if (select_query.SELECT() NE 0 ) {
        while(select_query.NEXT()) {

            int odeme_sekli    = select_query.VALUE(0).toInt();
            int cek_senet_id   = select_query.VALUE(1).toInt();


            if (odeme_sekli EQ ENUM_MUSTERI_CEKI) {
                if (CEK_SON_CEK_SENET_HAREKETINI_BUL(cek_senet_id) NE ENUM_SATICIYA_VERILDI) {
                    return false;
                }
            }
            if (odeme_sekli EQ ENUM_MUSTERI_SENEDI) {
                if (CEK_SON_CEK_SENET_HAREKETINI_BUL(cek_senet_id) NE ENUM_SATICIYA_VERILDI) {
                    return false;
                }
            }
        }

    }
    return true;
}

/**************************************************************************************
                   CARI_ODEME_FISI::KENDI_CEKLERIMIZ
***************************************************************************************/

int CARI_ODEME_FISI::KENDI_CEKLERIMIZ ()
{
    OPEN_CEK_SENET_FORMU ( ENUM_CEK, -1,this );
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::KENDI_SENETLERIMIZ
***************************************************************************************/

int CARI_ODEME_FISI::KENDI_SENETLERIMIZ ()
{
   OPEN_CEK_SENET_FORMU ( ENUM_SENET, -1,this );
   return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::SEARCH_EDIT_CLICKED
***************************************************************************************/

void CARI_ODEME_FISI::SEARCH_EDIT_CLICKED(QWidget * widget,QLineEdit * lineedit)
{
    Q_UNUSED(widget);

    int hesap_id = OPEN_CARI_KART_ARAMA ( lineedit->text(), SAHIS_ARTI_FIRMA, this, 1, E9_PROGRAMI, CARI_MODULU );

    if (hesap_id NE -1 ) {
        lineedit->setText(CARI_FIND_HESAP_KODU(hesap_id));

    }
}

/**************************************************************************************
                   CARI_ODEME_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void CARI_ODEME_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_record_id)
{ 
    if ( p_button EQ m_ui->push_button_odeme_makbuzu_yazdir ) {

        QString cari_odm_belge_seri, cari_odm_belge_numarasi;

        if ( M_FIS->yazdirildi_mi NE 1 ) {

            SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;
            int result = SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

            if( result EQ 0 ) {
                MSG_WARNING( "Ünite bulunamadı", NULL );
                return;
            }

            cari_odm_belge_seri     = m_ui->lineEdit_belge_seri->text();
            cari_odm_belge_numarasi = m_ui->lineEdit_belge_numarasi->text();

            if ( m_ui->lineEdit_belge_seri->text().isEmpty() EQ true OR m_ui->lineEdit_belge_numarasi->text().isEmpty() EQ true ) {
                cari_odm_belge_seri      = UNITE_BILG.odeme_makbuzu_belge_seri;
                cari_odm_belge_numarasi  = UNITE_BILG.odeme_makbuzu_belge_num;
            }

            int secim = MSG_YES_NO( tr( "Tahsilat Makbuzu; \n"
                                        "Belge Seri          : %1 \n"
                                        "Belge Numarası : %2 \n"
                                        "ile yazdırılacaktır. Onaylıyormusunuz ?").arg( cari_odm_belge_seri ).arg( cari_odm_belge_numarasi ), m_ui->lineEdit_belge_seri );

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

        PRINT_CARI_MAKBUZ_BELGESI( p_record_id, this );
        DB->START_TRANSACTION();
        SQL_QUERY query( DB );
        query.PREPARE_UPDATE( "car_fisler", "fis_id", "yazdirildi_mi, belge_seri, belge_numarasi", "fis_id = :fis_id");
        query.SET_VALUE     ( ":yazdirildi_mi"  , 1 );
        query.SET_VALUE     ( ":belge_seri"     , cari_odm_belge_seri );
        query.SET_VALUE     ( ":belge_numarasi" , cari_odm_belge_numarasi );
        query.SET_VALUE     ( ":fis_id"         , p_record_id );
        query.UPDATE();

        SUBE_UPDATE_ODM_MKBZ_BELGE_NO( SUBE_GET_UNITE_ID(), QVariant( QVariant( cari_odm_belge_numarasi ).toInt() + 1 ).toString() );
        DB->COMMIT_TRANSACTION();

        m_ui->lineEdit_belge_seri->setText( cari_odm_belge_seri );
        m_ui->lineEdit_belge_numarasi->setText( cari_odm_belge_numarasi );

        M_FIS->yazdirildi_mi = 1;
    }
    else if ( p_button EQ m_ui->pushButton_urettigi_fisler ) {
         E9_URETILEN_FIS_SEC( E9_PROGRAMI, CARI_MODULU, p_record_id, this );
    }
}

/**************************************************************************************
                   CARI_ODEME_FISI::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int CARI_ODEME_FISI::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    if ( p_button EQ m_ui->pushButton_detaylar ) {

        int record_id = GET_RECORD_ID();

        int muh_fis_id = MUH_GET_ENT_FIS_ID(M_FIS->program_id , M_FIS->modul_id , record_id );

        M_FIS_DETAYLARI->muh_fis_no = E9_GET_FIS_NO ( MUHASEBE_MODULU , muh_fis_id);

        SQL_QUERY query(DB);

        QStringList cek_fis_id_list = CEK_GET_ENT_FIS_ID_LIST ( M_FIS->program_id , M_FIS->modul_id , record_id );

        for ( int i = 0 ; i < cek_fis_id_list.size() ; i++ ) {
           int cek_senet_fis_id = QVariant (cek_fis_id_list.at(i)).toInt();

           int bordo_turu = CEK_GET_CEK_BORDRO_TURU( cek_senet_fis_id );

           if ( bordo_turu EQ ENUM_MUSTERI_CEK_BORDROSU ) {
                M_FIS_DETAYLARI->cek_fis_no = E9_GET_FIS_NO(CEKSENET_MODULU , cek_senet_fis_id);
           }
           else if( bordo_turu EQ ENUM_MUSTERI_SENET_BORDROSU ){
               M_FIS_DETAYLARI->senet_fis_no = E9_GET_FIS_NO(CEKSENET_MODULU , cek_senet_fis_id);
           }
           else if( bordo_turu EQ ENUM_FIRMA_CEK_BORDROSU) {
               M_FIS_DETAYLARI->firma_cek_fis_no = E9_GET_FIS_NO(CEKSENET_MODULU , cek_senet_fis_id);
           }
           else if( bordo_turu EQ ENUM_FIRMA_SENET_BORDROSU ){
               M_FIS_DETAYLARI->firma_senet_fis_no = E9_GET_FIS_NO(CEKSENET_MODULU , cek_senet_fis_id);
           }
        }

        QStringList bnk_fis_no_list ;
        query.PREPARE_SELECT("car_fis_satirlari","hesap_no_id","fis_id = :fis_id");
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

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   CARI_ODEME_FISI::LINE_SAVER_BUTTON_CLICKED
***************************************************************************************/

int CARI_ODEME_FISI::LINE_CHANGER_BUTTON_CLICKED(QAbstractButton *button, int row_number)
{
    QComboBox * combobox_odeme_sekli = (QComboBox*) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ODEME_TURU_COLUMN);
    QLineEdit * lineedit_cek_senet_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CEK_SENET_ID_COLUMN);

    Q_UNUSED(button);

    int islem_turu = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_odeme_sekli->currentText());


    if ( islem_turu EQ ENUM_NAKIT ) {
        QPushButton * push_button_detaylar = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DETAYLAR_COLUMN );
        if ( push_button_detaylar NE NULL ) {
            push_button_detaylar->setDisabled(true);
        }
        return ADAK_RECORD_UNCHANGED;
    }

    int fis_satiri_id = -1;

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->item(row_number ,ROW_ID_COLUMN);

    if ( item NE NULL ) {
        fis_satiri_id = item->text().toInt();
    }
    int cek_senet_id = lineedit_cek_senet_id->text().toInt();

    switch (islem_turu) {
    case ENUM_KENDI_CEKIMIZ :
    case ENUM_KENDI_SENEDIMIZ:
    case ENUM_MUSTERI_SENEDI:
    case ENUM_MUSTERI_CEKI:

        if (OPEN_CARI_ODEME_CEKSENET_BATCH(fis_satiri_id,cek_senet_id,islem_turu,this) EQ true) {

            CEK_SENET_STRUCT CEK_STRUCT;

            CEK_GET_TEMEL_BILGILER( &CEK_STRUCT, cek_senet_id );

            double cek_senet_tutari = CEK_STRUCT.cek_senet_tutari;

            QCommaEdit * commaedit_tutar  = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , TUTAR_COLUMN);
            QCommaEdit * commaedit_cari_para_birimi_tutari = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , CARI_PARA_BIRIMI_TUTARI_COLUMN);

            double cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI(m_cari_para_birimi_id,m_makbuz_para_birimi_id,
                                        m_ui->commaEdit_kur->GET_DOUBLE(),m_ui->commaEdit_parite->GET_DOUBLE(),cek_senet_tutari);

            commaedit_tutar->SET_DOUBLE(cek_senet_tutari);
            commaedit_cari_para_birimi_tutari->SET_DOUBLE(cari_para_birimi_tutari);


            return ADAK_RECORD_CHANGED;
        }
        break;

    case ENUM_HAVALE:
    case ENUM_EFT:
        if ( OPEN_E9_EFT_BATCH(ENUM_CARI_ODEME_FISI,islem_turu ,fis_satiri_id ,m_makbuz_para_birimi_id,
                                 M_ISLEM_DETAYLARI,CARI_MODULU , this) EQ true ) {

            QLineEdit * lineedit_hesap_no_id = (QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_NO_ID_COLUMN);
            lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());

            QLineEdit * lineedit_banka_ismi = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BANKA_ISMI_COLUMN);
            QLineEdit * lineedit_sube_kodu  = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,SUBE_KODU_COLUMN);
            QLineEdit * lineedit_sube_ismi  = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,SUBE_ISMI_COLUMN);
            QLineEdit * lineedit_hesap_no   = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_NO_COLUMN);

            lineedit_banka_ismi->setText ( M_ISLEM_DETAYLARI->banka_ismi );
            lineedit_sube_kodu->setText  ( M_ISLEM_DETAYLARI->sube_kodu  );
            lineedit_sube_ismi->setText  ( M_ISLEM_DETAYLARI->sube_adi   );
            lineedit_hesap_no->setText   ( M_ISLEM_DETAYLARI->hesap_no   );

            return ADAK_RECORD_CHANGED;
        }
        break;
    case ENUM_KREDI_KARTI:

        if ( OPEN_E9_KK_ODEME_BATCH(fis_satiri_id , M_ISLEM_DETAYLARI ,CARI_MODULU, this ) EQ true ) {
            QLineEdit * lineedit_kredi_karti_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,KREDI_KARTI_ID_COLUMN);
            lineedit_kredi_karti_id->setText(QVariant(M_ISLEM_DETAYLARI->kredi_karti_id).toString());
            QLineEdit * lineedit_hesap_no_id = (QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_NO_ID_COLUMN);
            lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());

            return ADAK_RECORD_CHANGED;
        }

        break;
    }

    return ADAK_RECORD_UNCHANGED;
}
/**************************************************************************************
                   CARI_ODEME_FISI::LEAVE_RECORD
***************************************************************************************/

int CARI_ODEME_FISI::LEAVE_RECORD()
{
    if ( m_log_kaydi_eklenecek_mi EQ true ) {
        DB->START_TRANSACTION();
        if(m_kayit_eklendi EQ true) {
            E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_ADD , m_log_detaylari);
        }
        if(m_kayit_silindi EQ true ) {
            E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_DELETE , m_log_detaylari);
        }
        if(m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
            E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_UPDATE , m_log_detaylari);
        }
        DB->COMMIT_TRANSACTION();
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_ODEME_FISI::EKRANI_GUNCELLE();
***************************************************************************************/

void CARI_ODEME_FISI::EKRANI_GUNCELLE()
{

    CLOSE_ALL_EVENTS();

    for ( int row = 0 ; row < m_ui->tablewidget_fis_satirlari->rowCount() ; row++) {

        QCommaEdit * commaedit_tutar                        = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row ,  TUTAR_COLUMN                    );
        QCommaEdit * commaedit_cari_para_birimi_tutari      = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row ,  CARI_PARA_BIRIMI_TUTARI_COLUMN  );
        QCommaEdit * commaedit_temel_para_birimi_tutari     = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row ,  TEMEL_PARA_BIRIMI_TUTARI_COLUMN );
        QLineEdit  * lineedit_makbuz_para_birimi            = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row,   MAKBUZ_PARA_BIRIMI_COLUMN       );
        QLineEdit  * lineedit_cari_para_birimi              = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row ,  CARI_PARA_BIRIMI_COLUMN         );


        lineedit_makbuz_para_birimi->setText(m_ui->comboBox_makbuz_para_birimi->currentText());
        lineedit_cari_para_birimi->setText(m_ui->lineEdit_cari_para_birimi->text());

        double tutar = commaedit_tutar->GET_DOUBLE();

        double kur = m_ui->commaEdit_kur->GET_DOUBLE();

        double cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI(m_cari_para_birimi_id,m_makbuz_para_birimi_id,
                                                                     kur,m_ui->commaEdit_parite->GET_DOUBLE(),tutar);



        if ( kur EQ 0.0 ) {
            commaedit_temel_para_birimi_tutari->SET_DOUBLE ( tutar );
        }
        else {
            commaedit_temel_para_birimi_tutari->SET_DOUBLE ( kur * tutar );
        }
        commaedit_cari_para_birimi_tutari->SET_DOUBLE ( cari_para_birimi_tutari );

    }

    OPEN_ALL_EVENTS();
}

