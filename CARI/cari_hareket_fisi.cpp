#include <QHeaderView>
#include <QMessageBox>
#include "adak_utils.h"
#include "dvz_enum.h"
#include "cari_fis_utils.h"
#include "ui_cari_hareket_fisi.h"
#include "cari_hareket_fisi_class.h"
#include "e9_struct.h"
#include "e9_enum.h"
#include "print.h"
#include "dvz_console_utils.h"
#include "cari_enum.h"
#include "yonetim.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "muh_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_yetki.h"
#include "e9_log.h"
#include "adak_std_utils.h"
#include "muh_fis_utils.h"
#include "adres_console_utils.h"
#include "cari_kart_arama_open.h"
#include "cari_fis_arama_open.h"
#include "cari_fisi_yazdir_raporu_open.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "e9_ent_detaylari_batch_open.h"
#include "muh_hesap_arama_open.h"
#include "dvz_gui_utils.h"
#include "cari_console_utils.h"

extern ADAK_SQL * DB;


#define     ROW_ID_COLUMN                       0
#define     FIS_ID_COLUMN                       1
#define     ORDER_COLUMN                        2
#define     CARI_HESAP_ID_COLUMN                3
#define     MUHASEBE_HESAP_ID_COLUMN            4
#define     SATIR_DOVIZ_CINSI_ID_COLUMN         5
#define     CARI_DOVIZ_CINSI_ID_COLUMN          6

#define     CARI_HESAP_KODU_COLUMN              7
#define     CARI_HESAP_ISMI_COLUMN              8
#define     CARI_PARA_BIRIMI_COLUMN             9
#define     ACIKLAMA_COLUMN                     10
#define     BORC_COLUMN                         11
#define     ALACAK_COLUMN                       12
#define     SATIR_DOVIZ_KODU_COLUMN             13
#define     DOVIZ_KURU_COLUMN                   14
#define     PARITE_COLUMN                       15
#define     CARI_PARA_BIRIMI_TUTARI_COLUMN      16
#define     TEMEL_PARA_BIRIMI_TUTARI_COLUMN     17
#define     MUHASEBE_HESAP_KODU_COLUMN          18
#define     MUHASEBE_HESAP_ISMI_COLUMN          19

/**************************************************************************************
                   OPEN_CARI_HAREKET_FISI
***************************************************************************************/

void OPEN_CARI_HAREKET_FISI ( int fis_id ,int fis_turu , int modul_id, QWidget * parent )
{
    CARI_HAREKET_FISI * F = new CARI_HAREKET_FISI ( fis_id, fis_turu, modul_id, parent );
     
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   CARI_HAREKET_FISI::CARI_HAREKET_FISI
***************************************************************************************/

CARI_HAREKET_FISI::CARI_HAREKET_FISI ( int p_fis_id, int p_fis_turu, int p_modul_id, QWidget * p_parent )
:FIS_KERNEL ( p_parent ), m_ui ( new Ui::CARI_HAREKET_FISI ), m_modul_id ( p_modul_id )
{
    m_fis_id   = p_fis_id;
    m_fis_turu = p_fis_turu;
    m_ui->setupUi    ( this );
    START_FIS_KERNEL ( this, DB );
}

/**************************************************************************************
                   CARI_HAREKET_FISI::SETUP_FORM
***************************************************************************************/

void CARI_HAREKET_FISI::SETUP_FORM()
{

    //! KERNEL FONK CAGRILDIGI YER
    int num_of_columns       = 20;

    bool is_navigation_screen = false;

    if ( m_modul_id EQ BANKA_MODULU) {
        SET_PAGE_TITLE( tr ( "CARİ - BANKA ENT. FİŞİ" ) );
        is_navigation_screen = true;
    }
    else if ( m_modul_id EQ CEKSENET_MODULU) {
        SET_PAGE_TITLE( tr ( "CARİ - ÇEK-SENET ENT. FİŞİ" ) );
        is_navigation_screen = true;
    }
    else if (m_modul_id EQ CEKSENET_MODULU ){
        SET_PAGE_TITLE( tr ( "CARİ - ÇEK-SENET ENT. FİŞİ ( ÖDEME )" ) );
        is_navigation_screen = true;
    }
    else if ( m_modul_id EQ FATURA_MODULU ) {
        SET_PAGE_TITLE( tr ( "CARİ - FATURA ENT. FİŞİ" ) );
        is_navigation_screen = true;
    }
    else if ( m_modul_id EQ SMM_MODULU ) {
        SET_PAGE_TITLE( tr ( "CARİ - SM MAKBUZ ENT. FİŞİ" ) );
        is_navigation_screen = true;
    }
    else if ( m_modul_id EQ ENUM_CARI_HAREKET_FISI) {
         SET_PAGE_TITLE( tr ( "CARİ - HAREKET FİŞİ" ) );

    }
    else if ( m_modul_id EQ PERSONEL_MODULU) {
         SET_PAGE_TITLE( tr ( "CARİ - PERSONEL ENT. FİŞİ" ) );
    }

    else  if ( m_modul_id EQ 0 ){
        SET_PAGE_TITLE( tr ( "CARİ - DİĞER ENT. FİŞLERİ" ) );
        is_navigation_screen = true;
    }
    else if ( m_modul_id EQ -1 ){
        SET_PAGE_TITLE( tr ( "CARİ - TÜM ENT. FİŞLERİ" ) );
        is_navigation_screen = true; 
    }

    if ( is_navigation_screen EQ true ) {
        m_ui->button_ent_detaylari->setHidden(true);
    }

    REGISTER_TABLE_WIDGET        ( m_ui->tablewidget_fis_satirlari , num_of_columns );
    REGISTER_BUTTONS_WIDGET      ( m_ui->navigation_button_widget,false,is_navigation_screen );

    //! END

    M_FIS                   =   new CARI_FIS_STRUCT;
    M_FIS_SATIRI            =   new CARI_FIS_SATIRI_STRUCT;
    M_ENT_DETAYLARI         =   new E9_ENT_DETAYLARI_STRUCT;

    M_FIS->modul_id         =   CARI_MODULU;
    M_FIS->program_id       =   E9_PROGRAMI;

    m_ui->lineedit_fis_no->setText("*");

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );

    if( m_fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }


    DISABLE_CLEAR_ON_WIDGET( m_ui->lineedit_fis_no );
    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_sirket );

    QFont tableWidget_font = m_ui->tablewidget_fis_satirlari->font();
    int   font_point_size  = tableWidget_font.pointSize             ();

    tableWidget_font.setPointSize                                   ( font_point_size -1 );
    m_ui->tablewidget_fis_satirlari->setFont                        ( tableWidget_font );

    tableWidget_font       = m_ui->tablewidget_fis_satirlari->horizontalHeader()->font();
    font_point_size        = tableWidget_font.pointSize             ();

    tableWidget_font.setPointSize                                   ( font_point_size -1 );
    m_ui->tablewidget_fis_satirlari->horizontalHeader()->setFont    ( tableWidget_font );



    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_fis_no->setValidator(validator);

    m_log_kaydi_eklenecek_mi = false;
    m_kayit_eklendi          = false;
    m_kayit_silindi          = false;

    m_temel_para_birimi_id   = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

    SET_FORM_ID_COLUMN_POSITION  ( FIS_ID_COLUMN );
    SET_FIS_ORDER_COLUMN_NUMBER  ( ORDER_COLUMN  );
    SET_ROW_ID_COLUMN_POSITION   ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN  ( 7 );

    SET_FIS_ORDER_DATABASE       ( "car_fis_satirlari", "order_number", "fis_satiri_id" );

    SET_FIRST_FOCUS_WIDGET       ( m_ui->lineedit_fis_no );


    SET_TABLE_ROW_WIDGETS        ( CARI_HESAP_ID_COLUMN            , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS        ( MUHASEBE_HESAP_ID_COLUMN        , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS        ( SATIR_DOVIZ_CINSI_ID_COLUMN     , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS        ( CARI_DOVIZ_CINSI_ID_COLUMN      , WIDGET_LINE_EDIT   );

    SET_TABLE_ROW_WIDGETS        ( CARI_HESAP_KODU_COLUMN          , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS        ( CARI_HESAP_ISMI_COLUMN          , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS        ( CARI_PARA_BIRIMI_COLUMN         , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS        ( ACIKLAMA_COLUMN                 , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS        ( BORC_COLUMN                     , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS        ( ALACAK_COLUMN                   , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS        ( SATIR_DOVIZ_KODU_COLUMN         , WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS        ( DOVIZ_KURU_COLUMN               , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS        ( PARITE_COLUMN                   , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS        ( CARI_PARA_BIRIMI_TUTARI_COLUMN  , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS        ( TEMEL_PARA_BIRIMI_TUTARI_COLUMN , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS        ( MUHASEBE_HESAP_KODU_COLUMN      , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS        ( MUHASEBE_HESAP_ISMI_COLUMN      , WIDGET_LINE_EDIT   );

    REGISTER_SAVER_BUTTON        ( m_ui->button_yazdir        );
    REGISTER_CHANGER_BUTTON      ( m_ui->button_ent_detaylari );

    switch ( m_fis_turu ) {
        case ENUM_CARI_ACILIS_FISI     :
            SET_PAGE_TITLE ( tr ( "AÇILIŞ FİŞİ" ) );
            SET_HELP_PAGE  ( "cari-hesap-islemleri_acilis-fisi" );

            m_ui->adakDate_fis_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_fis_tarihi->setEnabled           ( false );
            m_ui->label_toplam_tutar->setVisible            ( false );
            m_ui->commaedit_toplam_borc->setHidden(true);
            m_ui->commaedit_toplam_alacak->setHidden(true);

            break;

        case ENUM_CARI_HAREKET_FISI :
        default :
            if ( m_modul_id EQ CARI_MODULU ) {
                SET_PAGE_TITLE  ( tr ( "CARİ HAREKET FİŞİ" ) );
                SET_HELP_PAGE   ( "cari-hesap-islemleri_cari-hareket-fisi" );
            }
            else {
                SET_HELP_PAGE ( "cari-hesap-islemleri_cari-entegrasyon-fisleri" );
            }

            m_ui->label_bilgilendirme->setHidden(true);
            break;
    }

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH                   ( 512  );
    m_ui->limitedTextEdit_doviz_borc_bilgileri->SET_MAX_STRING_LENGTH       ( 1024 );
    m_ui->limitedTextEdit_doviz_alacak_bilgileri->SET_MAX_STRING_LENGTH     ( 1024 );
    m_ui->lineedit_fis_no->setMaxLength                                     ( 6    );


    if ( m_fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
        m_ui->tablewidget_fis_satirlari->hideColumn ( ACIKLAMA_COLUMN );
    }

    m_ui->tablewidget_fis_satirlari->hideColumn ( TEMEL_PARA_BIRIMI_TUTARI_COLUMN );

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels(QStringList() << "id" << "fis_id" << "fis_order"
                                                                             << "cari hes id" << "muh hes id"
                                                                             << "dvz cinsi id"<< "cari dvz cinsi id"
                                                                             << tr("Cari Hesap Kodu")
                                                                             << tr("Cari Hesap İsmi")
                                                                             << tr("   ") << tr("Açıklama")
                                                                             << tr("Borç") << tr("Alacak")
                                                                             << tr("Para Birimi")
                                                                             << tr("Döviz Kuru")
                                                                             << tr("Parite")
                                                                             << tr("Cari Para \n Birimi Tutarı")
                                                                             << tr("temel_para_birimi_tutari")
                                                                             << tr("Karşı Muhasebe \n Hesabı")
                                                                             << tr("Hesap İsmi")
                                                                             << tr("") << tr("") );

    if ( m_modul_id EQ CARI_MODULU ) {
        if ( m_fis_turu EQ ENUM_CARI_HAREKET_FISI AND E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 1 ) {
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_HESAP_KODU_COLUMN           , 113 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_HESAP_ISMI_COLUMN           , 155 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( ACIKLAMA_COLUMN                  , 221 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( BORC_COLUMN                      , 107 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( ALACAK_COLUMN                    , 96  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( SATIR_DOVIZ_KODU_COLUMN          , 70  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_PARA_BIRIMI_TUTARI_COLUMN   , 91  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( MUHASEBE_HESAP_KODU_COLUMN       , 136 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( MUHASEBE_HESAP_ISMI_COLUMN       , 144 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_PARA_BIRIMI_COLUMN          , 40  );
            SET_SETTING_NAME ( "CARI_HAREKET_FISI" );

        }
        else if ( m_fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
            m_ui->tablewidget_fis_satirlari->hideColumn ( MUHASEBE_HESAP_KODU_COLUMN      );
            m_ui->tablewidget_fis_satirlari->hideColumn ( MUHASEBE_HESAP_ISMI_COLUMN      );
            m_ui->tablewidget_fis_satirlari->hideColumn ( CARI_PARA_BIRIMI_TUTARI_COLUMN  );
            m_ui->tablewidget_fis_satirlari->hideColumn ( SATIR_DOVIZ_KODU_COLUMN         );
            m_ui->tablewidget_fis_satirlari->hideColumn ( DOVIZ_KURU_COLUMN               );
            m_ui->tablewidget_fis_satirlari->hideColumn ( PARITE_COLUMN                   );

            m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_HESAP_KODU_COLUMN           , 280 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_HESAP_ISMI_COLUMN           , 380 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( BORC_COLUMN                      , 155 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( ALACAK_COLUMN                    , 155 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( SATIR_DOVIZ_KODU_COLUMN          , 70  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_PARA_BIRIMI_TUTARI_COLUMN   , 91  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_PARA_BIRIMI_COLUMN          , 40  );

            SET_SETTING_NAME ( "CARI_ACILIS_FISI" );
        }
    }
    else {
        m_ui->tablewidget_fis_satirlari->hideColumn     ( MUHASEBE_HESAP_KODU_COLUMN             );
        m_ui->tablewidget_fis_satirlari->hideColumn     ( MUHASEBE_HESAP_ISMI_COLUMN             );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_HESAP_KODU_COLUMN           , 172 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_HESAP_ISMI_COLUMN           , 254 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( ACIKLAMA_COLUMN                  , 339 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( BORC_COLUMN                      , 107 );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( ALACAK_COLUMN                    , 96  );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( SATIR_DOVIZ_KODU_COLUMN          , 70  );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_PARA_BIRIMI_TUTARI_COLUMN   , 91  );
        m_ui->tablewidget_fis_satirlari->setColumnWidth ( CARI_PARA_BIRIMI_COLUMN          , 40  );
        SET_SETTING_NAME ( "CARI_FATURA_ENT_FISI" );
    }

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->horizontalHeaderItem ( CARI_PARA_BIRIMI_TUTARI_COLUMN );
    QFont item_font         = item->font();
    int   point_size        = item_font.pointSize();
    item_font.setPointSize ( point_size - 2 );
    item->setFont          ( item_font );
    item                    = m_ui->tablewidget_fis_satirlari->horizontalHeaderItem ( SATIR_DOVIZ_KODU_COLUMN );
    item_font               = item->font();
    point_size              = item_font.pointSize();
    item_font.setPointSize ( point_size - 2 );
    item->setFont          ( item_font );

    m_ui->tablewidget_fis_satirlari->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_fis_satirlari->setFocus( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET();

    AUTOSTART_FIS_KERNEL ( m_fis_id );
}

/**************************************************************************************
                   CARI_HAREKET_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void CARI_HAREKET_FISI::CLEAR_FORM_MEMBERS ()
{
    CARI_CLEAR_FIS_STRUCT(M_FIS);

    CARI_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    M_FIS->modul_id = CARI_MODULU;
    M_FIS->program_id = E9_PROGRAMI;

    M_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.clear ();
    M_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.clear   ();

    E9_CLEAR_ENT_DETAYLARI_STRUCT ( M_ENT_DETAYLARI );

}

/**************************************************************************************
                   CARI_HAREKET_FISI::NEW_FIS_RECORD
***************************************************************************************/

void CARI_HAREKET_FISI::NEW_FIS_RECORD()
{
    m_ui->lineedit_fis_no->setText("*");
    if ( m_fis_turu NE ENUM_CARI_HAREKET_FISI ) {
        m_ui->adakDate_fis_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
    }
}

/**************************************************************************************
                   CARI_HAREKET_FISI::GET_FIS_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::GET_FIS_RECORD ( int p_fis_id )
{    

    M_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI.clear();
    M_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI.clear();

    SQL_QUERY f_query ( DB );

    f_query.PREPARE_SELECT ( "car_fisler",
                             "fis_no, fis_tarihi, aciklama, toplam_borc, toplam_alacak, "
                             "doviz_tutar_bilgileri, program_id, modul_id, "
                             "muh_fis_id", "fis_id = :fis_id" );
    f_query.SET_VALUE(":fis_id" , p_fis_id);

    if ( f_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    f_query.NEXT();

    M_FIS->fis_no                       = f_query.VALUE ( "fis_no"             ).toInt();
    M_FIS->fis_tarihi                   = f_query.VALUE ( "fis_tarihi"         ).toString();
    M_FIS->aciklama                     = f_query.VALUE ( "aciklama"           ).toString();
    M_FIS->toplam_borc                  = f_query.VALUE ( "toplam_borc"        ).toDouble();
    M_FIS->toplam_alacak                = f_query.VALUE ( "toplam_alacak"      ).toDouble();
    M_FIS->modul_id                     = f_query.VALUE ( "modul_id"           ).toInt();
    M_FIS->program_id                   = f_query.VALUE ( "program_id"         ).toInt();
    M_FIS->muh_fis_id                   = f_query.VALUE ( "muh_fis_id").toInt();

    QString doviz_tutar_bilgileri   = f_query.VALUE(5).toString();

    if ( doviz_tutar_bilgileri.isEmpty() EQ false ) {
        QStringList TOPLAM_TUTAR_BILGILERI = doviz_tutar_bilgileri.split("#");
        if ( TOPLAM_TUTAR_BILGILERI.size() > 1 ) {
            M_FIS->borc_doviz_tutar_bilgileri   = TOPLAM_TUTAR_BILGILERI.at ( 0 );
            M_FIS->alacak_doviz_tutar_bilgileri = TOPLAM_TUTAR_BILGILERI.at ( 1 );
            m_ui->limitedTextEdit_doviz_borc_bilgileri->setText  ( M_FIS->borc_doviz_tutar_bilgileri   );
            m_ui->limitedTextEdit_doviz_alacak_bilgileri->setText( M_FIS->alacak_doviz_tutar_bilgileri );
        }
    }


    m_ui->lineedit_fis_no->setText              ( QVariant ( M_FIS->fis_no ).toString() );
    m_ui->adakDate_fis_tarihi->SET_DATE(  M_FIS->fis_tarihi );
    m_ui->limitedTextEdit_aciklama->setPlainText( M_FIS->aciklama );
    m_ui->commaedit_toplam_borc->SET_DOUBLE     ( M_FIS->toplam_borc   );
    m_ui->commaedit_toplam_alacak->SET_DOUBLE   ( M_FIS->toplam_alacak );

    f_query.PREPARE_SELECT ( "car_fis_satirlari", "fis_satiri_id ", "fis_id = :fis_id " );
    f_query.SET_VALUE      ( ":fis_id" , p_fis_id );

    if ( f_query.SELECT( "order_number" ) EQ 0 ) {
        return ADAK_OK;
    }

    int current_row;
    QTableWidgetItem * new_item;

    while ( f_query.NEXT() EQ true ) {

        current_row = ADD_NEW_LINE();

        int fis_satiri_id = f_query.VALUE ( "fis_satiri_id" ).toInt();

        CARI_CLEAR_FIS_SATIRI_STRUCT( M_FIS_SATIRI );

        CARI_FIS_SATIRINI_OKU( M_FIS_SATIRI, fis_satiri_id );

        new_item    = new QTableWidgetItem ( QString::number( fis_satiri_id )  );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row, ROW_ID_COLUMN, new_item );

        new_item    = new QTableWidgetItem ( QVariant ( p_fis_id ).toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row, FIS_ID_COLUMN, new_item );



        QSearchEdit * searchedit_cari_hesap_kodu            = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, CARI_HESAP_KODU_COLUMN           );
        QLineEdit   * lineedit_cari_hesap_ismi              = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, CARI_HESAP_ISMI_COLUMN           );
        QLineEdit   * lineedit_aciklama                     = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, ACIKLAMA_COLUMN                  );
        QCommaEdit  * commaedit_borc_tutari                 = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, BORC_COLUMN                      );
        QCommaEdit  * commaedit_alacak_tutari               = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, ALACAK_COLUMN                    );
        QComboBox   * combobox_satir_doviz_kodu             = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, SATIR_DOVIZ_KODU_COLUMN          );
        QCommaEdit  * commaEdit_cari_para_birimi_tutari     = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, CARI_PARA_BIRIMI_TUTARI_COLUMN   );
        QLineEdit   * lineedit_cari_hesap_id                = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, CARI_HESAP_ID_COLUMN             );
        QLineEdit   * lineedit_muh_hesap_id                 = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MUHASEBE_HESAP_ID_COLUMN         );
        QSearchEdit * searchedit_muhasebe_hesap_kodu        = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MUHASEBE_HESAP_KODU_COLUMN       );
        QLineEdit   * lineedit_muh_hesap_ismi               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, MUHASEBE_HESAP_ISMI_COLUMN       );
        QLineEdit   * lineedit_satir_doviz_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, SATIR_DOVIZ_CINSI_ID_COLUMN      );
        QLineEdit   * lineedit_cari_para_birimi             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, CARI_PARA_BIRIMI_COLUMN          );
        QCommaEdit  * commaEdit_doviz_kuru                  = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, DOVIZ_KURU_COLUMN                );
        QCommaEdit  * commaEdit_parite                      = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, PARITE_COLUMN                    );
        QLineEdit   * lineedit_cari_doviz_id                = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, CARI_DOVIZ_CINSI_ID_COLUMN       );

        lineedit_aciklama->setText                    ( M_FIS_SATIRI->aciklama          );
        commaedit_borc_tutari->SET_DOUBLE             ( M_FIS_SATIRI->borc_tutari       );
        commaedit_alacak_tutari->SET_DOUBLE           ( M_FIS_SATIRI->alacak_tutari     );
        commaEdit_cari_para_birimi_tutari->SET_DOUBLE ( M_FIS_SATIRI->cari_para_birimi_tutari);
        lineedit_cari_hesap_id->setText               ( QString::number( M_FIS_SATIRI->cari_hesap_id  )   );
        lineedit_muh_hesap_id->setText                ( QString::number( M_FIS_SATIRI->muh_hesap_id   )   );
        lineedit_satir_doviz_id->setText              ( QString::number( M_FIS_SATIRI->doviz_id       )   );
        combobox_satir_doviz_kodu->setCurrentIndex    ( combobox_satir_doviz_kodu->findText ( M_FIS_SATIRI->doviz_kodu ) );
        commaEdit_doviz_kuru->SET_DOUBLE              ( M_FIS_SATIRI->doviz_kuru        );
        commaEdit_parite->SET_DOUBLE                  ( M_FIS_SATIRI->parite            );

        QString cari_hesap_kodu, cari_hesap_ismi;
        int para_birim_id;

        int result =  CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( lineedit_cari_hesap_id->text().toInt(), &cari_hesap_kodu, &cari_hesap_ismi, &para_birim_id );

        if ( result > 0 ) {
            searchedit_cari_hesap_kodu->SET_TEXT            ( cari_hesap_kodu );
            lineedit_cari_hesap_ismi->setText               ( cari_hesap_ismi );
            lineedit_cari_para_birimi->setText              ( DVZ_GET_DOVIZ_KODU ( para_birim_id ) );
            lineedit_cari_doviz_id->setText                 ( QVariant ( para_birim_id ).toString() );
        }

        if ( m_fis_turu EQ ENUM_CARI_HAREKET_FISI ) {

            int satir_doviz_id = lineedit_satir_doviz_id->text().toInt();
            int cari_doviz_id  = lineedit_cari_doviz_id->text().toInt();

            if ( cari_doviz_id EQ satir_doviz_id ) {
                if ( satir_doviz_id EQ m_temel_para_birimi_id ) {
                    commaEdit_doviz_kuru->setDisabled   ( true );
                    commaEdit_parite->setDisabled       ( true );
                }
                else {
                    commaEdit_doviz_kuru->setEnabled ( true       );
                    commaEdit_doviz_kuru->SET_DOUBLE ( M_FIS_SATIRI->doviz_kuru );
                    commaEdit_parite->setDisabled    ( true );
                }
            }
            else {
                if ( satir_doviz_id EQ m_temel_para_birimi_id ) {
                    commaEdit_parite->setEnabled        ( true   );
                    commaEdit_parite->SET_DOUBLE        ( M_FIS_SATIRI->parite );
                    commaEdit_doviz_kuru->setDisabled   ( true   );
                }
                else {
                    commaEdit_doviz_kuru->setEnabled    ( true       );
                    commaEdit_doviz_kuru->SET_DOUBLE    ( M_FIS_SATIRI->doviz_kuru );

                    if ( cari_doviz_id NE m_temel_para_birimi_id ) {
                        commaEdit_parite->setEnabled    ( true );
                        commaEdit_parite->SET_DOUBLE    ( M_FIS_SATIRI->parite );
                    }
                    else {
                        commaEdit_parite->setDisabled   ( true );
                    }
                }
            }

            QString tam_hesap_kodu,hesap_ismi;

            MUH_GET_HESAP_KODU_HESAP_ISMI(lineedit_muh_hesap_id->text().toInt() ,tam_hesap_kodu,hesap_ismi);

            searchedit_muhasebe_hesap_kodu->SET_TEXT    ( tam_hesap_kodu );
            lineedit_muh_hesap_ismi->setText            ( hesap_ismi);

        }



        if ( commaedit_borc_tutari->GET_DOUBLE() EQ 0.0 ) {
            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_ARTIR ( combobox_satir_doviz_kodu->currentText(),
                                                              commaedit_alacak_tutari->GET_DOUBLE(), 0,
                                                              M_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                              M_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
        }
        else {
            CARI_HAREKET_FISI_SATIR_DOVIZ_TUTARLARINI_ARTIR ( combobox_satir_doviz_kodu->currentText(),
                                                              commaedit_borc_tutari->GET_DOUBLE(),1,
                                                              M_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI,
                                                              M_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI );
        }

    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void CARI_HAREKET_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QLineEdit  * lineedit_cari_hsp_ismi             = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_HESAP_ISMI_COLUMN            );
    QLineEdit  * lineedit_aciklama                  = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN                   );
    QComboBox  * comboBox_satir_doviz_kodu          = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SATIR_DOVIZ_KODU_COLUMN           );
    QCommaEdit * commaEdit_cari_para_birimi_tutari  = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_PARA_BIRIMI_TUTARI_COLUMN    );
    QLineEdit  * lineedit_muh_hesap_ismi            = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MUHASEBE_HESAP_ISMI_COLUMN        );
    QLineEdit  * lineedit_cari_para_birimi          = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_PARA_BIRIMI_COLUMN           );
    QCommaEdit * commaEdit_doviz_kuru               = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DOVIZ_KURU_COLUMN                 );
    QCommaEdit * commaEdit_parite                   = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PARITE_COLUMN                     );

    lineedit_cari_hsp_ismi->setReadOnly             ( true );
    lineedit_cari_hsp_ismi->setMaxLength            ( 60   );
    lineedit_cari_para_birimi->setMaxLength         ( 5    );
    lineedit_aciklama->setMaxLength                 ( 128  );
    lineedit_muh_hesap_ismi->setMaxLength(60);
    DVZ_FILL_COMBOBOX                             ( comboBox_satir_doviz_kodu );

    comboBox_satir_doviz_kodu->setCurrentIndex      ( comboBox_satir_doviz_kodu->findText ( DVZ_GET_TEMEL_PARA_BIRIMI_KODU()));
    commaEdit_cari_para_birimi_tutari->setEnabled   ( false );

    commaEdit_doviz_kuru->SET_PRECISION             ( 4 );
    commaEdit_parite->SET_PRECISION                 ( 6 );

    QFont commaEdit_font = commaEdit_cari_para_birimi_tutari->font  ();
    int   point_size     = commaEdit_font.pointSize                 ();

    commaEdit_font.setPointSize                 ( point_size - 2 );
    commaEdit_cari_para_birimi_tutari->setFont  ( commaEdit_font );
    lineedit_muh_hesap_ismi->setReadOnly        ( true );
    lineedit_cari_para_birimi->setReadOnly      ( true );
}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int CARI_HAREKET_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( object EQ m_ui->limitedTextEdit_aciklama ) {
        if ( m_ui->limitedTextEdit_aciklama->toPlainText().size() > 512 ) {
            MSG_WARNING ( tr ( "Açıklamanız 512 karakterden daha uzun olamaz!.. " ), NULL );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int CARI_HAREKET_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineedit_fis_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Yeni Fiş Kaydı Yapabilmek için (*) girmelisiniz!.."),  m_ui->lineedit_fis_no );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/
int CARI_HAREKET_FISI::CHECK_FIS_RECORD_ADD()
{
    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) NE "*" ) {
        int fis_no = m_ui->lineedit_fis_no->text().toInt();
        if ( CARI_FIS_NO_KULLANILDI_MI ( fis_no, m_ui->adakDate_fis_tarihi->DATE()) ) {
            MSG_WARNING( tr("Aynı no ile başka bir fiş var.Fiş eklenemedi!.."), m_ui->lineedit_fis_no );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::ADD_FIS_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::ADD_FIS_RECORD ()
{
    int fis_no = 0;

    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) EQ "*" ) {
        fis_no = CARI_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE());
    }
    else {
        fis_no = m_ui->lineedit_fis_no->text().toInt();
    }

    M_FIS->fis_no         = fis_no;
    M_FIS->fis_tarihi     = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama       = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->fis_turu       = m_fis_turu;
    M_FIS->modul_id       = CARI_MODULU;
    M_FIS->program_id     = E9_PROGRAMI;

    int fis_id = CARI_FIS_EKLE ( M_FIS );

    m_ui->lineedit_fis_no->setText ( QVariant ( fis_no ).toString() );

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_eklendi          = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( m_fis_turu ) + "," +
                       m_ui->adakDate_fis_tarihi->DATE() +
                       ",Fiş No : "  + QVariant ( fis_no ).toString() );

    return fis_id;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int CARI_HAREKET_FISI::CHECK_FIS_RECORD_UPDATE(int fis_id)
{

    SQL_QUERY sql_query (DB);

    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) EQ "*" ) {
        MSG_WARNING(  tr("Fiş numarasını güncellemek için fiş no kısmına sadece sayı girebilirsiniz."), m_ui->lineedit_fis_no );

        return ADAK_FAIL;
    }

    int fis_no = m_ui->lineedit_fis_no->text().toInt();

    sql_query.PREPARE_SELECT("car_fisler","fis_id","fis_no = :fis_no AND fis_tarihi = :fis_tarihi");

    sql_query.SET_VALUE(":fis_no"       ,fis_no );
    sql_query.SET_VALUE(":fis_tarihi"   ,m_ui->adakDate_fis_tarihi->DATE());

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        if ( sql_query.VALUE(0).toInt() NE fis_id ) {
            MSG_WARNING( tr("Aynı numarada başka bir fiş var.Güncelleme yapılamadı!.."), m_ui->lineedit_fis_no );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void CARI_HAREKET_FISI::UPDATE_FIS_RECORD ( int p_fis_id )
{

    CARI_FIS_BILGILERINI_OKU(M_FIS , p_fis_id);

    M_FIS->fis_tarihi           =   m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->fis_no               =   m_ui->lineedit_fis_no->text().toInt();
    M_FIS->aciklama             =   m_ui->limitedTextEdit_aciklama->toPlainText();

    CARI_FISI_GUNCELLE          ( M_FIS, p_fis_id );

    m_log_kaydi_eklenecek_mi    =   true;

    m_log_detaylari             = ( CARI_GET_FIS_TURLERI_STRING ( m_fis_turu ) + "," +
                                m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                                ",Fiş No : "  + QVariant ( M_FIS->fis_no ).toString() + ",Toplam Borç : " +
                                VIRGUL_EKLE ( QVariant   ( m_ui->commaedit_toplam_borc->GET_DOUBLE()   ).toString() )
                                + ",Toplam Alacak : " +
                                VIRGUL_EKLE ( QVariant   ( m_ui->commaedit_toplam_alacak->GET_DOUBLE() ).toString() ) );

    ADD_ADAK_EVENT( ADAK_REFRESH );

}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int CARI_HAREKET_FISI::CHECK_FIS_RECORD_DELETE ( int fis_id )
{

    Q_UNUSED(fis_id);

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void CARI_HAREKET_FISI::DELETE_FIS_RECORD ( int p_fis_id )
{
    CARI_FISI_SIL ( M_FIS, p_fis_id );

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_silindi          = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING  ( m_fis_turu ) + "," +
                       m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                       ",Fiş No : "  + m_ui->lineedit_fis_no->text()+ ",Toplam Borç : " +
                       VIRGUL_EKLE ( QVariant    ( m_ui->commaedit_toplam_borc->GET_DOUBLE() ).toString()  )
                       + ",Toplam Alacak : " +
                       VIRGUL_EKLE ( QVariant    ( m_ui->commaedit_toplam_alacak->GET_DOUBLE() ).toString()) );

}

/**************************************************************************************
                   CARI_HAREKET_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::SELECT_FIS_RECORD()
{
    return OPEN_CARI_FIS_ARAMA ( m_fis_turu,m_modul_id, ENUM_CARI_FISI, this );
}

/**************************************************************************************
                   CARI_HAREKET_FISI::FIND_FIS_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString  query_str = "fis_no = :fis_no "
                         "AND fis_turu = :fis_turu "
                         "AND fis_tarihi = :fis_tarihi ";

    if( m_modul_id EQ 0 ) {
        query_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        query_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT( "car_fisler", "fis_id", query_str );

    select_query.SET_VALUE(":fis_no"        , m_ui->lineedit_fis_no->text().toInt());
    select_query.SET_VALUE(":fis_turu"      , m_fis_turu    );
    select_query.SET_VALUE(":fis_tarihi"    , m_ui->adakDate_fis_tarihi->DATE());

    if( m_modul_id NE 0 ) {
            select_query.SET_VALUE(":modul_id"      , m_modul_id );
    }

    if ( select_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr("Aradığınız Cari Hareket Fişi Bulunamadı!.."), m_ui->lineedit_fis_no );

        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_HAREKET_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT( "car_fisler", "fis_id", "fis_turu  = :fis_turu" );

    select_query.SET_VALUE ( ":fis_turu",    m_fis_turu    );

    if ( m_modul_id NE 0 ) {
        select_query.AND_EKLE("modul_id = :modul_id");
        select_query.SET_VALUE ( ":modul_id",    m_modul_id    );
    }
    else {
        select_query.AND_EKLE("program_id != :program_id");
        select_query.SET_VALUE(":program_id" , E9_PROGRAMI);
    }

    if ( select_query.SELECT( "fis_tarihi ASC, fis_no ASC", 0, 1 ) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_HAREKET_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT( "car_fisler", "fis_id", "fis_turu  = :fis_turu" );

    select_query.SET_VALUE ( ":fis_turu",    m_fis_turu    );

    if ( m_modul_id NE 0 ) {
        select_query.AND_EKLE("modul_id = :modul_id");
        select_query.SET_VALUE ( ":modul_id",    m_modul_id    );
    }
    else {
        select_query.AND_EKLE("program_id != :program_id");
        select_query.SET_VALUE(":program_id" , E9_PROGRAMI);
    }

    if ( select_query.SELECT( "fis_tarihi DESC, fis_no DESC", 0, 1 ) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_HAREKET_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString  query_str = "fis_no             < :fis_no      "
                         "AND fis_turu       = :fis_turu    "
                         "AND fis_tarihi     = :fis_tarihi  ";

    if( m_modul_id EQ 0 ) {
        query_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        query_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT( "car_fisler", "fis_id", query_str );

    select_query.SET_VALUE(":fis_no",       m_ui->lineedit_fis_no->text().toInt()       );
    select_query.SET_VALUE(":fis_turu",     m_fis_turu                                  );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id",     m_modul_id                              );
    }
    select_query.SET_VALUE(":fis_tarihi",   m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_no DESC" , 0, 1 ) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    query_str.clear();
    query_str = "fis_turu           = :fis_turu    "
                "AND fis_tarihi     < :fis_tarihi  ";

    if( m_modul_id EQ 0 ) {
        query_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        query_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT( "car_fisler", "fis_id", query_str );

    select_query.SET_VALUE(":fis_turu"     , m_fis_turu    );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id"  , m_modul_id );
    }
    select_query.SET_VALUE(":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_tarihi DESC, fis_no DESC" , 0, 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_HAREKET_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    QString  query_str = "fis_no             > :fis_no      "
                         "AND fis_turu       = :fis_turu    "
                         "AND fis_tarihi     = :fis_tarihi  ";

    if( m_modul_id EQ 0 ) {
        query_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        query_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT( "car_fisler", "fis_id", query_str );

    select_query.SET_VALUE(":fis_no",       m_ui->lineedit_fis_no->text().toInt()   );
    select_query.SET_VALUE(":fis_turu",     m_fis_turu                              );
    if( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id",     m_modul_id                              );
    }
    select_query.SET_VALUE(":fis_tarihi",   m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_no ASC ", 0 , 1  ) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    query_str.clear();
    query_str = "fis_turu           = :fis_turu         "
                "AND fis_tarihi     > :fis_tarihi       ";

    if( m_modul_id EQ 0 ) {
        query_str.append( " AND program_id != " + QString::number( E9_PROGRAMI ) );
    }
    else {
        query_str.append( " AND modul_id = :modul_id " );
    }

    select_query.PREPARE_SELECT( "car_fisler", "fis_id", query_str );

    select_query.SET_VALUE(":fis_turu",     m_fis_turu );
    if ( m_modul_id NE 0 ) {
        select_query.SET_VALUE(":modul_id",     m_modul_id );
    }
    select_query.SET_VALUE(":fis_tarihi",   m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_tarihi, fis_no" , 0 , 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_LINE_VAR
***************************************************************************************/

int CARI_HAREKET_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{

    QLineEdit   * lineedit_cari_hesap_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_HESAP_ID_COLUMN             );
    QSearchEdit * searchedit_cari_hesap_kodu         = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_HESAP_KODU_COLUMN           );
    QLineEdit   * lineedit_cari_hesap_ismi           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_HESAP_ISMI_COLUMN           );
    QComboBox   * combobox_satir_doviz_kodu          = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SATIR_DOVIZ_KODU_COLUMN          );
    QCommaEdit  * commaEdit_cari_para_birimi_tutari  = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_PARA_BIRIMI_TUTARI_COLUMN   );
    QCommaEdit  * commaEdit_temel_para_birimi_tutari = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, TEMEL_PARA_BIRIMI_TUTARI_COLUMN  );
    QCommaEdit  * commaEdit_borc_tutari              = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BORC_COLUMN                      );
    QCommaEdit  * commaEdit_alacak_tutari            = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ALACAK_COLUMN                    );
    QSearchEdit * search_edit_muh_hesap_kodu         = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MUHASEBE_HESAP_KODU_COLUMN       );
    QLineEdit   * lineedit_satir_doviz_id            = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SATIR_DOVIZ_CINSI_ID_COLUMN      );
    QLineEdit   * lineedit_cari_doviz_id             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_DOVIZ_CINSI_ID_COLUMN       );
    QLineEdit   * lineedit_cari_para_birimi          = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, CARI_PARA_BIRIMI_COLUMN          );
    QCommaEdit  * commaEdit_doviz_kuru               = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DOVIZ_KURU_COLUMN                );
    QCommaEdit  * commaEdit_parite                   = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, PARITE_COLUMN                    );

    SQL_QUERY query ( DB );


    if ( object EQ searchedit_cari_hesap_kodu ) {

        commaEdit_doviz_kuru->clear();
        commaEdit_parite->clear();

        if ( searchedit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_cari_hesap_ismi->clear();
            lineedit_cari_para_birimi->clear();
            lineedit_cari_hesap_id->clear();
            lineedit_cari_doviz_id->clear();

            return ADAK_OK;
        }

        query.PREPARE_SELECT ( "car_hesaplar","adres_id, cari_hesap_id, para_birimi_id ",
                               "cari_hesap_kodu = :cari_hesap_kodu" );
        query.SET_VALUE      ( ":cari_hesap_kodu" , searchedit_cari_hesap_kodu->GET_TEXT() );

        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING( tr("Aradığınız cari hesap kodu bulunamadı!.."), searchedit_cari_hesap_kodu  );

            return ADAK_FAIL_UNDO;
        }

        query.NEXT();

        lineedit_cari_hesap_ismi->setText ( ADR_GET_FIRMA_SAHIS_ADI(query.VALUE(0).toInt()) );
        lineedit_cari_hesap_id->setText   ( query.VALUE(1).toString() );

        int     cari_para_birimi_id     = query.VALUE(2).toInt();
        QString cari_para_birimi_kodu   = DVZ_GET_DOVIZ_KODU ( cari_para_birimi_id );

        lineedit_cari_para_birimi->setText (  cari_para_birimi_kodu );
        lineedit_cari_doviz_id->setText    ( QVariant ( cari_para_birimi_id ).toString() );

        combobox_satir_doviz_kodu->setCurrentIndex  ( combobox_satir_doviz_kodu->findText ( cari_para_birimi_kodu ) );
        lineedit_satir_doviz_id->setText            ( QVariant ( cari_para_birimi_id ).toString() );

        commaEdit_parite->setEnabled     ( false );

        if ( cari_para_birimi_id EQ m_temel_para_birimi_id ) {
            commaEdit_doviz_kuru->setEnabled ( false );
            return ADAK_OK;
        }

        commaEdit_doviz_kuru->setEnabled ( true );
        commaEdit_doviz_kuru->SET_DOUBLE ( DVZ_DOVIZ_KURUNU_AL ( lineedit_satir_doviz_id->text().toInt() , m_ui->adakDate_fis_tarihi->QDATE() ));

        if ( m_fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
            commaEdit_parite->setEnabled     ( false );
            commaEdit_doviz_kuru->setEnabled ( false );
        }
    }
    else if ( object EQ commaEdit_borc_tutari OR object EQ commaEdit_alacak_tutari OR
              object EQ combobox_satir_doviz_kodu ) {

        if ( m_fis_turu EQ ENUM_CARI_ACILIS_FISI ) {
            if ( object EQ commaEdit_alacak_tutari ) {
                commaEdit_cari_para_birimi_tutari->SET_DOUBLE  ( commaEdit_alacak_tutari->GET_DOUBLE() );
                commaEdit_temel_para_birimi_tutari->SET_DOUBLE ( commaEdit_alacak_tutari->GET_DOUBLE() );
            }
            else if ( object EQ commaEdit_borc_tutari ) {
                commaEdit_cari_para_birimi_tutari->SET_DOUBLE  ( commaEdit_borc_tutari->GET_DOUBLE() );
                commaEdit_temel_para_birimi_tutari->SET_DOUBLE ( commaEdit_borc_tutari->GET_DOUBLE() );
            }
            return ADAK_OK;
        }

        if ( object EQ combobox_satir_doviz_kodu ) {
            lineedit_satir_doviz_id->setText ( QVariant ( DVZ_GET_DOVIZ_ID ( combobox_satir_doviz_kodu->currentText() ) ).toString() );
        }

        double doviz_alis_kuru          = 0.00;
        double doviz_satis_kuru         = 0.00;

        double borc_tutari              = commaEdit_borc_tutari->GET_DOUBLE();
        double alacak_tutari            = commaEdit_alacak_tutari->GET_DOUBLE();

        int    cari_doviz_id            = lineedit_cari_doviz_id->text().toInt();
        int    satir_doviz_id           = lineedit_satir_doviz_id->text().toInt();

        double cari_para_birimi_tutari  = 0.00;


        commaEdit_doviz_kuru->clear      ();
        commaEdit_parite->clear          ();

        doviz_alis_kuru        = DVZ_DOVIZ_KURUNU_AL ( satir_doviz_id,m_ui->adakDate_fis_tarihi->QDATE(),ENUM_ALIS);
        doviz_satis_kuru       = DVZ_DOVIZ_KURUNU_AL ( satir_doviz_id,m_ui->adakDate_fis_tarihi->QDATE());
        double parite_alis     = DVZ_PARITEYI_AL     ( cari_doviz_id , satir_doviz_id ,m_ui->adakDate_fis_tarihi->QDATE(),ENUM_ALIS);
        double parite_satis    = DVZ_PARITEYI_AL     ( cari_doviz_id , satir_doviz_id , m_ui->adakDate_fis_tarihi->QDATE(),ENUM_SATIS);


        if ( cari_doviz_id EQ satir_doviz_id ) {
            if ( satir_doviz_id EQ m_temel_para_birimi_id ) {
                commaEdit_doviz_kuru->setDisabled(true);
                commaEdit_parite->setDisabled(true);
            }
            else {
                commaEdit_doviz_kuru->setEnabled(true);

                if ( borc_tutari > 0) {
                    commaEdit_doviz_kuru->SET_DOUBLE(doviz_satis_kuru);
                }
                else {
                    commaEdit_doviz_kuru->SET_DOUBLE(doviz_alis_kuru);
                }

                commaEdit_parite->setDisabled(true);
            }
        }
        else {
            if ( satir_doviz_id EQ m_temel_para_birimi_id ) {
                commaEdit_parite->setEnabled(true);
                if ( borc_tutari > 0 ) {
                    commaEdit_parite->SET_DOUBLE(parite_satis);
                }
                else {
                    commaEdit_parite->SET_DOUBLE(parite_alis);
                }
                commaEdit_doviz_kuru->setDisabled(true);
            }
            else {
                commaEdit_doviz_kuru->setEnabled(true);

                if ( borc_tutari > 0) {
                    commaEdit_doviz_kuru->SET_DOUBLE(doviz_satis_kuru);
                }
                else {
                    commaEdit_doviz_kuru->SET_DOUBLE(doviz_alis_kuru);
                }


                if ( cari_doviz_id NE m_temel_para_birimi_id ) {
                    commaEdit_parite->setEnabled(true);
                    if ( borc_tutari > 0 ) {
                        commaEdit_parite->SET_DOUBLE(parite_satis);
                    }
                    else {
                        commaEdit_parite->SET_DOUBLE(parite_alis);
                    }
                }
                else {
                    commaEdit_parite->setDisabled(true);
                }
            }
        }

        if ( borc_tutari > 0 ) {
            cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI ( cari_doviz_id, satir_doviz_id, commaEdit_doviz_kuru->GET_DOUBLE(), commaEdit_parite->GET_DOUBLE(), borc_tutari );
            commaEdit_cari_para_birimi_tutari->SET_DOUBLE ( cari_para_birimi_tutari );
        }
        else {
            cari_para_birimi_tutari = CARI_GET_PARA_BIRIMI_TUTARI ( cari_doviz_id, satir_doviz_id, doviz_alis_kuru, commaEdit_parite->GET_DOUBLE(), alacak_tutari );
            commaEdit_cari_para_birimi_tutari->SET_DOUBLE         ( cari_para_birimi_tutari );
        }

    }
    else if ( object EQ commaEdit_parite ) {

        double borc_tutari      = commaEdit_borc_tutari->GET_DOUBLE();
        double alacak_tutari    = commaEdit_alacak_tutari->GET_DOUBLE();
        double parite           = commaEdit_parite->GET_DOUBLE();

        if ( borc_tutari > 0 ) {
            commaEdit_cari_para_birimi_tutari->SET_DOUBLE ( ROUND ( borc_tutari * parite ) );
        }
        else if ( alacak_tutari > 0 ) {
            commaEdit_cari_para_birimi_tutari->SET_DOUBLE ( ROUND ( alacak_tutari * parite ) );
        }
    }
    else if ( object EQ commaEdit_doviz_kuru ) {

        double borc_tutari      = commaEdit_borc_tutari->GET_DOUBLE();
        double alacak_tutari    = commaEdit_alacak_tutari->GET_DOUBLE();
        double doviz_kuru       = commaEdit_doviz_kuru->GET_DOUBLE();

        if ( borc_tutari > 0 ) {
            commaEdit_cari_para_birimi_tutari->SET_DOUBLE ( ROUND ( borc_tutari * doviz_kuru) );
        }
        else if ( alacak_tutari > 0 ) {
            commaEdit_cari_para_birimi_tutari->SET_DOUBLE ( ROUND ( alacak_tutari * doviz_kuru ) );
        }
    }
    else if ( object EQ search_edit_muh_hesap_kodu ) {

        //Cari hesabin muhasebe hesabi ile karsi muhasebe hesabi ayni olmamalidir.

        query.PREPARE_SELECT("car_hesaplar","muh_hesap_id","cari_hesap_kodu = :cari_hesap_kodu");
        query.SET_VALUE(":cari_hesap_kodu" , searchedit_cari_hesap_kodu->GET_TEXT());

        int cari_hesap_muh_hesabi = 0;

        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            cari_hesap_muh_hesabi = query.VALUE(0).toInt();
        }

        MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

        MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

        if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , search_edit_muh_hesap_kodu->GET_TEXT()) EQ 0 ) {
            MSG_WARNING( tr("Aradığınız banka muhasebe hesap kodu bulunamadı"), search_edit_muh_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        int muh_hesap_id = MUHASEBE_HESABI->hesap_id;

        if ( muh_hesap_id EQ cari_hesap_muh_hesabi ) {
            MSG_WARNING(  tr ( "Karşı Muhasebe hesabı olarak,cari hesaba ait olan muhasebe hesabını kullanamazsınız!.." ), search_edit_muh_hesap_kodu );

            return ADAK_FAIL;
        }

        QLineEdit * lineedit_muh_hesap_ismi  = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MUHASEBE_HESAP_ISMI_COLUMN );
        QLineEdit * lineedit_muh_hesap_id    = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, MUHASEBE_HESAP_ID_COLUMN   );

        lineedit_muh_hesap_ismi->setText ( MUHASEBE_HESABI->hesap_ismi );
        lineedit_muh_hesap_id->setText   ( QVariant(MUHASEBE_HESABI->hesap_id).toString() );
    }

    if ( commaEdit_borc_tutari->GET_DOUBLE() EQ 0.00 ) {
        if ( lineedit_satir_doviz_id->text().toInt() EQ m_temel_para_birimi_id ) {
            commaEdit_temel_para_birimi_tutari->SET_DOUBLE ( commaEdit_alacak_tutari->GET_DOUBLE() );
        }
        else {
            if ( m_fis_turu EQ ENUM_CARI_HAREKET_FISI ) {
                commaEdit_temel_para_birimi_tutari->SET_DOUBLE ( commaEdit_alacak_tutari->GET_DOUBLE() * commaEdit_doviz_kuru->GET_DOUBLE() );
            }
            else {
                double doviz_kuru = DVZ_DOVIZ_KURUNU_AL ( lineedit_satir_doviz_id->text().toInt(), m_ui->adakDate_fis_tarihi->QDATE(), ENUM_ALIS );
                commaEdit_temel_para_birimi_tutari->SET_DOUBLE ( commaEdit_alacak_tutari->GET_DOUBLE() * doviz_kuru );
            }
        }
    }
    else if ( commaEdit_alacak_tutari->GET_DOUBLE() EQ 0.00 ) {
        if ( lineedit_satir_doviz_id->text().toInt() EQ m_temel_para_birimi_id ) {
            commaEdit_temel_para_birimi_tutari->SET_DOUBLE ( commaEdit_borc_tutari->GET_DOUBLE() );
        }
        else {
            if ( m_fis_turu EQ ENUM_CARI_HAREKET_FISI ) {
                commaEdit_temel_para_birimi_tutari->SET_DOUBLE ( commaEdit_borc_tutari->GET_DOUBLE() * commaEdit_doviz_kuru->GET_DOUBLE() );
            }
            else {
                double doviz_kuru = DVZ_DOVIZ_KURUNU_AL ( lineedit_satir_doviz_id->text().toInt(), m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS );
                commaEdit_temel_para_birimi_tutari->SET_DOUBLE ( commaEdit_borc_tutari->GET_DOUBLE() * doviz_kuru );
            }
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int CARI_HAREKET_FISI::CHECK_LINE_EMPTY(int row_number)
{
    QCommaEdit  * commaedit_borc_tutari      = ( QCommaEdit *  ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BORC_COLUMN                 );
    QCommaEdit  * commaedit_alacak_tutari    = ( QCommaEdit *  ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ALACAK_COLUMN               );
    QSearchEdit * searchedit_cari_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CARI_HESAP_KODU_COLUMN      );
    QSearchEdit * searchedit_muh_hesap_kodu  = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MUHASEBE_HESAP_KODU_COLUMN  );

    if ( searchedit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( tr("Cari Hesap kodunu boş bırakamazsınız!.."), searchedit_cari_hesap_kodu );

        return ADAK_FAIL;
    }

    if ( commaedit_borc_tutari->GET_DOUBLE() EQ 0.0 AND commaedit_alacak_tutari->GET_DOUBLE() EQ 0.0) {
        MSG_WARNING( tr("Hem borç hem de alacak tutarını boş bırakamazsınız!.."), commaedit_borc_tutari );

        return ADAK_FAIL;
    }

    if ( commaedit_borc_tutari->GET_DOUBLE() NE 0.0 AND commaedit_alacak_tutari->GET_DOUBLE() NE 0.0 ){
        MSG_WARNING( tr("Satıra hem borç , hem de alacak tutarı giremezsiniz!.."), commaedit_borc_tutari );

        return ADAK_FAIL;
    }

    if ( m_fis_turu EQ ENUM_CARI_HAREKET_FISI ) {

        if ( searchedit_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            QMessageBox::warning(this,"UYARI" , tr("Fişin muhasebeye entegre edilebilmesi için,muhasebe hesabının girilmesi gerekmektedir."));
            SET_FOCUS( searchedit_muh_hesap_kodu );
            return ADAK_FAIL;
        }

    }

    return ADAK_OK;
}


/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_ADD_LINE
***************************************************************************************/

int CARI_HAREKET_FISI::CHECK_ADD_LINE ( int fis_id, int row_number )
{

    Q_UNUSED ( fis_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::ADD_LINE
***************************************************************************************/


void CARI_HAREKET_FISI::ADD_LINE ( int p_fis_id, int p_row_number )
{
    QLineEdit   * lineedit_aciklama                     = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, ACIKLAMA_COLUMN                  );
    QCommaEdit  * commaedit_borc_tutari                 = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, BORC_COLUMN                      );
    QCommaEdit  * commaedit_alacak_tutari               = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, ALACAK_COLUMN                    );
    QComboBox   * combobox_doviz_kodu                   = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, SATIR_DOVIZ_KODU_COLUMN          );
    QLineEdit   * lineedit_cari_hesap_id                = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, CARI_HESAP_ID_COLUMN             );
    QLineEdit   * lineedit_satir_doviz_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, SATIR_DOVIZ_CINSI_ID_COLUMN      );
    QLineEdit   * lineedit_muh_hesap_id                 = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, MUHASEBE_HESAP_ID_COLUMN         );
    QCommaEdit  * commaEdit_doviz_kuru                  = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, DOVIZ_KURU_COLUMN                );
    QCommaEdit  * commaedit_parite                      = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, PARITE_COLUMN                    );

    CARI_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    M_FIS_SATIRI->fis_id                                = p_fis_id;
    M_FIS_SATIRI->cari_hesap_id                         = lineedit_cari_hesap_id->text().toInt              ();
    M_FIS_SATIRI->aciklama                              = lineedit_aciklama->text                           ();
    M_FIS_SATIRI->borc_tutari                           = commaedit_borc_tutari->GET_DOUBLE                 ();
    M_FIS_SATIRI->alacak_tutari                         = commaedit_alacak_tutari->GET_DOUBLE               ();
    M_FIS_SATIRI->order_number                          = 0;
    M_FIS_SATIRI->doviz_id                              = lineedit_satir_doviz_id->text().toInt             ();
    M_FIS_SATIRI->doviz_kuru                            = commaEdit_doviz_kuru->GET_DOUBLE                  ();
    M_FIS_SATIRI->parite                                = commaedit_parite->GET_DOUBLE                      ();
    M_FIS_SATIRI->muh_hesap_id                          = lineedit_muh_hesap_id->text().toInt               ();
    M_FIS_SATIRI->doviz_kodu                            = combobox_doviz_kodu->currentText();
    M_FIS_SATIRI->modul_id                              = M_FIS->modul_id;
    M_FIS_SATIRI->program_id                            = M_FIS->program_id;

    M_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI        = M_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI;
    M_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI      = M_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI;


    int row_id = CARI_FIS_SATIRI_EKLE ( M_FIS, M_FIS_SATIRI );

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( row_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( p_row_number, ROW_ID_COLUMN, new_item );

    new_item = new QTableWidgetItem ( QVariant ( p_fis_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( p_row_number, FIS_ID_COLUMN, new_item );

    m_ui->commaedit_toplam_borc->SET_DOUBLE   ( M_FIS->toplam_borc   );
    m_ui->commaedit_toplam_alacak->SET_DOUBLE ( M_FIS->toplam_alacak );

    m_ui->limitedTextEdit_doviz_borc_bilgileri->setText     ( M_FIS->borc_doviz_tutar_bilgileri   );
    m_ui->limitedTextEdit_doviz_alacak_bilgileri->setText   ( M_FIS->alacak_doviz_tutar_bilgileri );

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( m_fis_turu ) + "," +
                       m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                       ",Fiş No : "  + m_ui->lineedit_fis_no->text() + ",Toplam Borç : " +
                       VIRGUL_EKLE ( QVariant ( M_FIS->toplam_borc   ).toString() ) + ",Toplam Alacak : " +
                       VIRGUL_EKLE ( QVariant ( M_FIS->toplam_alacak ).toString() ) );

}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int CARI_HAREKET_FISI::CHECK_UPDATE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::UPDATE_LINE
***************************************************************************************/

void CARI_HAREKET_FISI::UPDATE_LINE ( int p_fis_id, int p_row_number )
{
    QCommaEdit  * commaedit_borc_tutari                 = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, BORC_COLUMN                        );
    QCommaEdit  * commaedit_alacak_tutari               = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, ALACAK_COLUMN                      );
    QLineEdit   * lineedit_aciklama                     = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, ACIKLAMA_COLUMN                    );
    QComboBox   * combobox_satir_doviz_kodu             = ( QComboBox   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, SATIR_DOVIZ_KODU_COLUMN            );
    QLineEdit   * lineedit_satir_doviz_id               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, SATIR_DOVIZ_CINSI_ID_COLUMN        );
    QLineEdit   * lineedit_cari_hesap_id                = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, CARI_HESAP_ID_COLUMN               );
    QLineEdit   * lineedit_muh_hesap_id                 = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, MUHASEBE_HESAP_ID_COLUMN           );
    QCommaEdit  * commaEdit_doviz_kuru                  = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, DOVIZ_KURU_COLUMN                  );
    QCommaEdit  * commaedit_parite                      = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, PARITE_COLUMN                      );

    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item ( p_row_number, ROW_ID_COLUMN )->text().toInt();

    CARI_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    CARI_FIS_SATIRINI_OKU(M_FIS_SATIRI,fis_satiri_id);

    M_FIS_SATIRI->fis_id                            = p_fis_id;
    M_FIS_SATIRI->cari_hesap_id                     = lineedit_cari_hesap_id->text().toInt              ();
    M_FIS_SATIRI->aciklama                          = lineedit_aciklama->text                           ();
    M_FIS_SATIRI->borc_tutari                       = commaedit_borc_tutari->GET_DOUBLE                 ();
    M_FIS_SATIRI->alacak_tutari                     = commaedit_alacak_tutari->GET_DOUBLE               ();
    M_FIS_SATIRI->order_number                      = m_ui->tablewidget_fis_satirlari->item             ( p_row_number, ORDER_COLUMN )->text().toInt();
    M_FIS_SATIRI->doviz_id                          = lineedit_satir_doviz_id->text().toInt             ();
    M_FIS_SATIRI->doviz_kuru                        = commaEdit_doviz_kuru->GET_DOUBLE                  ();
    M_FIS_SATIRI->parite                            = commaedit_parite->GET_DOUBLE                      ();
    M_FIS_SATIRI->muh_hesap_id                      = lineedit_muh_hesap_id->text().toInt               ();
    M_FIS_SATIRI->doviz_kodu                        = combobox_satir_doviz_kodu->currentText            ();
    M_FIS_SATIRI->base_record_id                    = 0;
    M_FIS_SATIRI->modul_id                          = M_FIS->modul_id;
    M_FIS_SATIRI->program_id                        = M_FIS->program_id;


    M_FIS->DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI    = M_DOVIZ_CINSINDEN_BORC_SATIR_TOPLAMLARI;
    M_FIS->DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI  = M_DOVIZ_CINSINDEN_ALACAK_SATIR_TOPLAMLARI;

    CARI_FIS_SATIRI_SIL(M_FIS,fis_satiri_id);
    int new_satir_id = CARI_FIS_SATIRI_EKLE(M_FIS,M_FIS_SATIRI);

    if ( new_satir_id NE 0 ) {

        QTableWidgetItem * new_item;
        new_item = new QTableWidgetItem( QVariant ( new_satir_id ).toString());
        m_ui->tablewidget_fis_satirlari->setItem(p_row_number,ROW_ID_COLUMN,new_item);
    }


    m_ui->commaedit_toplam_borc->SET_DOUBLE   ( M_FIS->toplam_borc );
    m_ui->commaedit_toplam_alacak->SET_DOUBLE ( M_FIS->toplam_alacak );

    m_ui->limitedTextEdit_doviz_borc_bilgileri->setText     ( M_FIS->borc_doviz_tutar_bilgileri   );
    m_ui->limitedTextEdit_doviz_alacak_bilgileri->setText   ( M_FIS->alacak_doviz_tutar_bilgileri );


    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( m_fis_turu ) + "," +
                        m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY")+
                        ",Fiş No : "  + m_ui->lineedit_fis_no->text() + ",Toplam Borç : " +
                        VIRGUL_EKLE ( QVariant ( M_FIS->toplam_borc    ).toString() ) + ",Toplam Alacak : " +
                        VIRGUL_EKLE ( QVariant ( M_FIS->toplam_alacak  ).toString() ) );

}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int CARI_HAREKET_FISI::CHECK_DELETE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_HAREKET_FISI::DELETE_LINE
***************************************************************************************/

void CARI_HAREKET_FISI::DELETE_LINE ( int p_fis_id, int p_row_number )
{
    int fis_satiri_id       = m_ui->tablewidget_fis_satirlari->item ( p_row_number, ROW_ID_COLUMN )->text().toInt();

    M_FIS_SATIRI->fis_id    = p_fis_id;

    CARI_FIS_SATIRI_SIL ( M_FIS, fis_satiri_id );

    m_ui->commaedit_toplam_borc->SET_DOUBLE   ( M_FIS->toplam_borc );
    m_ui->commaedit_toplam_alacak->SET_DOUBLE ( M_FIS->toplam_alacak );

    m_ui->limitedTextEdit_doviz_borc_bilgileri->setText     ( M_FIS->borc_doviz_tutar_bilgileri   );
    m_ui->limitedTextEdit_doviz_alacak_bilgileri->setText   ( M_FIS->alacak_doviz_tutar_bilgileri );


    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( m_fis_turu ) + "," +
                       m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                       ",Fiş No : "  + m_ui->lineedit_fis_no->text() + ",Toplam Borç : " +
                       VIRGUL_EKLE  ( QVariant ( M_FIS->toplam_borc   ).toString()) + ",Toplam Alacak : " +
                       VIRGUL_EKLE  ( QVariant ( M_FIS->toplam_alacak ).toString()));
}

/**************************************************************************************
                   CARI_HAREKET_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::LOCK_FIS_RECORD ( int fis_id )
{
    return DB->LOCK_ROW ( "fis_id","car_fisler", QString ( "fis_id = %1" ).arg ( fis_id ));
}

/**************************************************************************************
                   CARI_HAREKET_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void CARI_HAREKET_FISI::UNLOCK_FIS_RECORD ( int fis_id )
{
    DB->UNLOCK_ROW ( "fis_id","car_fisler", QString ( "fis_id = %1" ).arg ( fis_id ));
}


/**************************************************************************************
                   CARI_HAREKET_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void CARI_HAREKET_FISI::LINE_SEARCH_EDIT_CLICKED ( int row_number, QWidget * widget , QLineEdit * lineedit)
{
    Q_UNUSED ( widget );
    Q_UNUSED ( row_number );

    QSearchEdit * searchedit_cari_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,CARI_HESAP_KODU_COLUMN);
    QSearchEdit * searchedit_muh_hesap_kodu  = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,MUHASEBE_HESAP_KODU_COLUMN);

    if ( widget EQ searchedit_cari_hesap_kodu ) {

        int cari_hesap_id = OPEN_CARI_KART_ARAMA ( lineedit->text(), SAHIS_ARTI_FIRMA, this, 1, E9_PROGRAMI, CARI_MODULU );

        if ( cari_hesap_id < 1 ) {
            return;
        }

        lineedit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id) );
    }
    if ( widget EQ searchedit_muh_hesap_kodu ) {
        int muh_hesap_id = OPEN_MUH_HESAP_ARAMA ( searchedit_muh_hesap_kodu->GET_TEXT(),this,1 );

        if( muh_hesap_id > 0 ) {

            lineedit->setText( MUH_GET_HESAP_KODU( muh_hesap_id ) );
        }
    }
}

/**************************************************************************************
                   CARI_HAREKET_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void CARI_HAREKET_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_record_id )
{
    if ( p_button EQ m_ui->button_yazdir ) {
        OPEN_REPORT_SHOWER ( GET_CARI_FISI_YAZDIR_RAPORU ( p_record_id ,ENUM_CARI_HAREKET_FISI ), nativeParentWidget() );
    }
    else if ( p_button EQ m_ui->pushButton_urettigi_fisler ) {
         E9_URETILEN_FIS_SEC( E9_PROGRAMI, CARI_MODULU , p_record_id, this );
    }
}

/**************************************************************************************
                   CARI_HAREKET_FISI::CHANCER_BUTTON_CLICKED
***************************************************************************************/

int  CARI_HAREKET_FISI::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{

    bool degismis_mi = false;

    if ( p_button EQ m_ui->button_ent_detaylari ) {

        int muh_fis_no = E9_GET_FIS_NO(MUHASEBE_MODULU , M_FIS->muh_fis_id);

        M_ENT_DETAYLARI->muh_fis_no = muh_fis_no;

        OPEN_E9_ENT_DETAYLARI_BATCH ( M_ENT_DETAYLARI, false,this );

    }

    if( degismis_mi  EQ true){
        return ADAK_RECORD_CHANGED;
    }

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   CAR_CARI_HAREKET_FISI::LEAVE_RECORD
***************************************************************************************/

int CARI_HAREKET_FISI::LEAVE_RECORD()
{
    if ( m_log_kaydi_eklenecek_mi EQ true ) {
        DB->START_TRANSACTION();
        if ( m_kayit_eklendi EQ true) {
             E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_ADD , m_log_detaylari);
        }
        if ( m_kayit_silindi EQ true ) {
             E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_DELETE , m_log_detaylari);
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
             E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_UPDATE , m_log_detaylari);
        }
        DB->COMMIT_TRANSACTION();
    }
    return ADAK_OK;
}
