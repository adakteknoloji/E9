#include <adak_sql.h>
#include "stok_urun_arama_class.h"
#include "ui_stok_urun_arama.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "adak_utils.h"
#include "stok_console_utils.h"
#include "stok_gui_utils.h"
#include "adak_gruplar.h"
#include "stok_gui_utils.h"
#include "adak_std_utils.h"
#include <qtextcodec.h>
#include "stok_urun_karti_export_open.h"
#include "stok_urun_hareketleri_arama_open.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                                OPEN_STOK_URUN_ARAMA
***************************************************************************************/

int OPEN_STOK_URUN_ARAMA ( int kart_turu,int p_depo_id, QString p_arama_icerigi,int hizmet_turu, QWidget * p_parent , int islem_turu )
{
    int secilen_id    = -1;
    STOK_URUN_ARAMA * F = new STOK_URUN_ARAMA (kart_turu, p_depo_id, p_arama_icerigi,&secilen_id, hizmet_turu,p_parent , islem_turu );
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                                STOK_URUN_ARAMA::STOK_URUN_ARAMA
***************************************************************************************/

STOK_URUN_ARAMA::STOK_URUN_ARAMA ( int p_kart_turu,int p_depo_id, QString p_arama_icerigi,
                                   int * p_secilen_id,int p_hizmet_turu, QWidget * p_parent , int islem_turu ) : ARAMA_KERNEL ( p_parent ), m_ui ( new Ui::STOK_URUN_ARAMA )
{
    m_depo_id                = p_depo_id;
    m_urun_hizmet_karti_turu = p_kart_turu;
    m_arama_icerigi          = p_arama_icerigi;
    m_secilen_id             = p_secilen_id;
    m_hizmet_turu            = p_hizmet_turu;
    m_islem_turu             = islem_turu ;

    m_ui->setupUi       (this );
    START_ARAMA_KERNEL  (this, DB );
}

/**************************************************************************************
                                STOK_URUN_ARAMA::CHECK_VAR
***************************************************************************************/

void STOK_URUN_ARAMA::SETUP_FORM()
{

    REGISTER_TABLE_WIDGET ( m_ui->tableWidget );

    HEADER_DUZENLE();

    SET_HELP_PAGE ("stok_urun_arama.html");

    //! default arayuz ayarlari

    m_ui->checkBox_urun_hizmet_kodu->setChecked( true );
    m_ui->frame_sirket_filtresi->setVisible( false );
    m_ui->frame_gruplar->setEnabled( false );
    m_ui->frame_urun_hizmet_turu->setEnabled( false );

    STK_FILL_STOK_KART_TURLERI_COMBOBOX( m_ui->comboBox_urun_hizmet_turu );

    //! end default

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->frame_sirket_filtresi->setVisible( true );
    }

    m_ui->lineEdit_urun_kodu_urun_adi->setText ( m_arama_icerigi );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons);

    SET_SORTING ( false );

    SET_FIRST_FOCUS_WIDGET( m_ui->lineEdit_urun_kodu_urun_adi );

    m_ui->tableWidget->hideColumn(0);

    m_ui->tableWidget->setColumnWidth( 1, 50  );
    m_ui->tableWidget->setColumnWidth( 2, 100 );
    m_ui->tableWidget->setColumnWidth( 3, 50  );
    m_ui->tableWidget->setColumnWidth( 4 ,50  );
    m_ui->tableWidget->setColumnWidth( 5 ,50  );
    m_ui->tableWidget->setColumnWidth( 6 ,15  );

    SET_SETTING_NAME ( "STOK_URUN_ARAMA" );

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );
    SET_FIND_BUTTON_NAME        ( tr("Bul") );

    m_ui->lineEdit_urun_kodu_urun_adi->setText ( m_arama_icerigi );

    if ( m_ui->lineEdit_urun_kodu_urun_adi->text().isEmpty() EQ false ) {
        AUTO_START ();
    }
    else {
        m_ui->lineEdit_urun_kodu_urun_adi->setText("*");
    }

    m_ui->label_dosya_encoding->setVisible( false );
    m_ui->comboBox_dosya_encoding->setVisible( false );

    if( m_islem_turu EQ STOK_URUN_KARTI_EXPORT_ISLEMI ){
        m_ui->label_dosya_encoding->setVisible(true);
        m_ui->comboBox_dosya_encoding->setVisible(true);

        QList< QByteArray > codec_name = QTextCodec::availableCodecs() ;

        for( int i = 0 ; i < codec_name.size() ; ++i ){
            m_ui->comboBox_dosya_encoding->addItem( codec_name.at(i) );
        }

        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_dosya_encoding, tr("UTF-8"));
        SET_PAGE_TITLE ( tr ( "DIŞARIYA KAYIT İHRAÇ ET") );
        AUTO_START();
    }
    else{
        m_ui->tool_button_export->hide();
        SET_PAGE_TITLE ( tr ( "URUN ARAMA ") );
    }
}

/**************************************************************************************
                                STOK_URUN_ARAMA::CHECK_VAR
***************************************************************************************/

int STOK_URUN_ARAMA::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->checkBox_urun_hizmet_turu ) {

        if ( m_ui->checkBox_urun_hizmet_turu->isChecked() EQ true ) {
            m_ui->frame_urun_hizmet_turu->setEnabled( true );
            SET_FOCUS( m_ui->comboBox_urun_hizmet_turu );

            m_urun_hizmet_karti_turu = STK_GET_STOK_KART_TURLERI_ENUM( m_ui->comboBox_urun_hizmet_turu->currentText() );
            HEADER_DUZENLE();
        }
        else {
            m_ui->frame_urun_hizmet_turu->setEnabled( false );
            m_ui->comboBox_urun_hizmet_turu->setCurrentIndex( 0 );
        }
    }
    else if ( p_object EQ m_ui->checkBox_urun_hizmet_kodu ) {
        if ( m_ui->checkBox_urun_hizmet_kodu->isChecked() EQ true ) {
            m_ui->frame_urun_hizmet_kodu->setEnabled( true );
            SET_FOCUS( m_ui->lineEdit_urun_kodu_urun_adi );
        }
        else {
            m_ui->frame_urun_hizmet_kodu->setEnabled( false );
        }
    }
    else if ( p_object EQ m_ui->checkBox_gruplar ) {
        if ( m_ui->checkBox_gruplar->isChecked() EQ true ) {
            m_ui->frame_gruplar->setEnabled( true );
            SET_FOCUS( m_ui->pushButton_grup_sec );
        }
        else {
            m_ui->frame_gruplar->setEnabled( false );
        }
    }
    else if ( p_object EQ m_ui->pushButton_grup_sec ) {
        OPEN_GRUP_SECIM( E9_PROGRAMI,STOK_MODULU , &m_grup_idleri, DB , this, true );

        QStringList grup_adlari;
        QStringList grup_kodlari;

        m_ui->textEdit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , &grup_adlari, &grup_kodlari));
    }
    else if ( p_object EQ m_ui->comboBox_urun_hizmet_turu ) {
        m_urun_hizmet_karti_turu = STK_GET_STOK_KART_TURLERI_ENUM( m_ui->comboBox_urun_hizmet_turu->currentText() );
        HEADER_DUZENLE();
    }
    else if ( p_object EQ m_ui->tool_button_export ) {
        if ( m_urun_id_list.isEmpty() EQ false ) {
            OPEN_STOK_URUN_KARTI_EXPORT( m_urun_id_list , this );
            SET_FOCUS( m_ui->lineEdit_urun_kodu_urun_adi );
        }
        else {
            MSG_INFO("Dışarıya aktarılacak kayıt bulunmadı" , this);
        }
    }

    return ADAK_OK;

}

/**************************************************************************************
                                STOK_URUN_ARAMA::CHECK_RUN
***************************************************************************************/

int STOK_URUN_ARAMA::CHECK_RUN ()
{
    if ( m_ui->lineEdit_urun_kodu_urun_adi->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr("Arama için bir içerik girmelisiniz."), m_ui->lineEdit_urun_kodu_urun_adi );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                                STOK_URUN_ARAMA::SEARCH
***************************************************************************************/

void STOK_URUN_ARAMA::SEARCH()
{
    QString stk_girenler_table_name = "";
    QString stk_cikanlar_table_name = "";

    stk_girenler_table_name     = "toplam_stoga_giren_array";
    stk_cikanlar_table_name     = "toplam_stoktan_cikan_array";

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_urunler","urun_id, urun_kodu, urun_adi,urun_temel_birimi, hizmet_mi" );

    sql_query.AND_EKLE( "( urun_kodu LIKE :urun_kodu OR urun_adi LIKE :urun_adi OR barkod_numarasi LIKE :barkod_numarasi )"  );
    sql_query.SET_LIKE( ":urun_kodu"        , m_ui->lineEdit_urun_kodu_urun_adi->text()     );
    sql_query.SET_LIKE( ":urun_adi"         , m_ui->lineEdit_urun_kodu_urun_adi->text()     );
    sql_query.SET_LIKE( ":barkod_numarasi"  , m_ui->lineEdit_urun_kodu_urun_adi->text()     );

    if ( m_ui->checkBox_urun_hizmet_turu->isChecked() EQ true ){
        if ( m_urun_hizmet_karti_turu EQ ENUM_URUN_KARTI ) {
            sql_query.AND_EKLE  ( "hizmet_mi = :hizmet_mi" );
            sql_query.SET_VALUE ( ":hizmet_mi", ENUM_URUN_KARTI );
        }
        if ( m_urun_hizmet_karti_turu EQ ENUM_HIZMET_KARTI ) {
            sql_query.AND_EKLE  ( "hizmet_mi = :hizmet_mi" );
            sql_query.SET_VALUE ( ":hizmet_mi", ENUM_HIZMET_KARTI );
        }
    }
    else {
        int stk_kart_turu = STK_GET_STOK_KART_TURLERI_ENUM( m_ui->comboBox_urun_hizmet_turu->currentText() );
        // eger urun ve hizmet ise filtre uygulanacak
        if ( stk_kart_turu NE ENUM_URUN_HIZMET_KARTI ) {
            int hizmet_mi = 0;
            if ( stk_kart_turu EQ ENUM_HIZMET_KARTI ) {
                hizmet_mi = 1;
            }
            sql_query.AND_EKLE  ( "hizmet_mi = :hizmet_mi" );
            sql_query.SET_VALUE ( ":hizmet_mi", hizmet_mi );
        }
    }
    if ( m_hizmet_turu NE -1 ) {
        sql_query.AND_EKLE  ( "hizmet_turu = :hizmet_turu_1 OR hizmet_turu = :hizmet_turu_2" );
        sql_query.SET_VALUE ( ":hizmet_turu_1", m_hizmet_turu );
        sql_query.SET_VALUE ( ":hizmet_turu_2", ENUM_ALINAN_SATILAN_HIZMET );
    }

    if ( m_ui->checkBox_gruplar->isChecked() EQ true ) {
         QList<int> gruptaki_urun_or_hizmet_id_list = GRP_GRUBA_EKLI_KAYITLARI_BUL ( E9_PROGRAMI , STOK_MODULU , m_grup_idleri );
         if ( gruptaki_urun_or_hizmet_id_list.size() >  0 ) {
            sql_query.AND_MULTI_EKLE("urun_id" , sql_query.TO_QVARIANT( gruptaki_urun_or_hizmet_id_list ), ADAK_OR );
         }
         else {
             return;
         }
    }

    if ( sql_query.SELECT("urun_kodu") EQ 0 ) {
        return;
    }
    int current_row = 0;

    double GIREN_URUN_MIKTARI   [E9_ARRAY_SIZE];
    double CIKAN_URUN_MIKTARI   [E9_ARRAY_SIZE];

    m_urun_id_list.clear();

    while ( sql_query.NEXT() EQ true ) {

        current_row = ADD_NEW_ROW();
        int   urun_id       = sql_query.VALUE(0).toInt();
        int   hizmet_mi     = sql_query.VALUE("hizmet_mi").toInt();
        double urun_miktari = 0.00;

        m_urun_id_list << urun_id ;

        if ( m_urun_hizmet_karti_turu NE ENUM_HIZMET_KARTI ) {
            SQL_QUERY query(DB);

            query.PREPARE_SELECT("stk_urunler",QString(stk_girenler_table_name+","+stk_cikanlar_table_name),"urun_id = :urun_id");
            query.SET_VALUE(":urun_id" , urun_id);

            if ( query.SELECT() NE 0 ) {
                query.NEXT();

                UNPACK_DOUBLE_ARRAY(query.VALUE(0).toString() , GIREN_URUN_MIKTARI,E9_ARRAY_SIZE);
                UNPACK_DOUBLE_ARRAY(query.VALUE(1).toString() , CIKAN_URUN_MIKTARI,E9_ARRAY_SIZE);

                urun_miktari = GIREN_URUN_MIKTARI[E9_SON_BAKIYE] - CIKAN_URUN_MIKTARI[E9_SON_BAKIYE];
            }
        }

        m_ui->tableWidget->item ( current_row, 0 )->setText( QVariant( urun_id ).toString() );
        m_ui->tableWidget->item ( current_row, 1 )->setText( sql_query.VALUE(1).toString());
        m_ui->tableWidget->item ( current_row, 2 )->setText( sql_query.VALUE(2).toString());
        m_ui->tableWidget->item ( current_row, 3 )->setTextAlignment ( Qt::AlignVCenter + Qt::AlignRight );
        m_ui->tableWidget->item ( current_row, 3 )->setText( VIRGUL_EKLE ( QVariant ( ROUND ( urun_miktari )).toString() ) );

        if ( m_urun_hizmet_karti_turu NE ENUM_HIZMET_KARTI ) {
            m_ui->tableWidget->item ( current_row, 4 )->setText(sql_query.VALUE("urun_temel_birimi").toString());
            m_ui->tableWidget->item ( current_row, 4 )->setTextAlignment(Qt::AlignCenter);

            if ( m_urun_hizmet_karti_turu EQ ENUM_URUN_HIZMET_KARTI ) {
                QString turu = "ÜRÜN";
                if( hizmet_mi EQ 1 ) {
                   turu = "HİZMET";
                }
                m_ui->tableWidget->item( current_row, 5 )->setText( turu );
                m_ui->tableWidget->item( current_row, 5 )->setTextAlignment( Qt::AlignCenter );
            }
        }

        QPushButton *button_hareketler = new QPushButton();
        button_hareketler->setIcon(QPixmap(":/e9_icons/E9_ICONS/cari_hareketler.png"));
        button_hareketler->setToolTip("Ürün Hareketler");

        button_hareketler->setObjectName(QString::number(urun_id));

        REGISTER_CHANGER_BUTTON(button_hareketler);

        m_button_list.insert( button_hareketler, urun_id);

        m_ui->tableWidget->setCellWidget(current_row, 6, button_hareketler );
    }
}

/**************************************************************************************
                                STOK_URUN_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int STOK_URUN_ARAMA::SINGLE_LINE_SELECTED ( int p_selected_row_number )
{
    if( m_islem_turu EQ STOK_URUN_KARTI_EXPORT_ISLEMI ){
        return ADAK_CONTINUE ;
    }

    *m_secilen_id = m_ui->tableWidget->item ( p_selected_row_number, 0 )->text().toInt();
    return ADAK_EXIT;
}

/**************************************************************************************
                                STOK_URUN_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

void STOK_URUN_ARAMA::HEADER_DUZENLE()
{

    switch (m_urun_hizmet_karti_turu) {
        case ENUM_URUN_KARTI   :
            SET_PAGE_TITLE( tr ( "ÜRÜN ARAMA" ) );
            SET_HEADERS(QStringList()<<tr("urun_id")<<tr("Ürün Kodu")<<tr("Ürün Adı")<<tr("Ürün Miktarı")
                        << tr("Temel Ürün Birimi") << tr(""));
            break;

        case ENUM_HIZMET_KARTI :
            SET_PAGE_TITLE( tr ( "HİZMET ARAMA" ) );
            SET_HEADERS(QStringList()<<tr("urun_id")<<tr("Hizmet Kodu")<<tr("Hizmet Adı") << tr("Hizmet Miktarı") << tr(""));
            break;

        default           :
            SET_PAGE_TITLE( tr ( "ÜRÜN/HİZMET ARAMA" ) );
            SET_HEADERS(QStringList()<<tr("urun_id")<<tr("Ürün/Hizmet Kodu")<<tr("Ürün/Hizmet Adı")
                        <<tr("Ürün Miktarı") << tr("Temel Ürün Birimi") << tr("Türü") << tr("") );
            break;
    }
}

/**************************************************************************************
                                STOK_URUN_ARAMA::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int STOK_URUN_ARAMA::CHANGER_BUTTON_CLICKED (QAbstractButton *button)
{
    int urun_id = -1;

    if ( m_button_list.contains((QPushButton*)button) EQ false ) {
        return ADAK_RECORD_UNCHANGED;
    }
    for (int i = 0; i < m_button_list.size(); ++i) {
        if ( m_button_list.keys().at(i) EQ button ) {
            urun_id = m_button_list.values().at(i);
        }
    }

    OPEN_STOK_URUN_HAREKETLERI_ARAMA( urun_id, this );
    return ADAK_RECORD_UNCHANGED;
}
