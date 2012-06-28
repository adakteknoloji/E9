#include <QMessageBox>
#include <adak_utils.h>
#include <adak_sql.h>
#include "e9_enum.h"
#include "cari_kart_arama_class.h"
#include "ui_cari_kart_arama.h"
#include "adak_std_utils.h"
#include "dvz_enum.h"
#include "yonetim.h"
#include "dvz_console_utils.h"
#include "adres_console_utils.h"
#include "adak_sql_query.h"
#include "cari_kart_formu_open.h"
#include "adak_gruplar.h"
#include "e9_console_utils.h"
#include "cari_kart_export_open.h"
#include "prs_karti_export_open.h"
#include <qtextcodec.h>
#include "cari_oto_ekstre_arama_open.h"


extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_CARI_KART_ARAMA
***************************************************************************************/

int OPEN_CARI_KART_ARAMA( QString cari_hesap_ismi_hesap_kodu, int cari_kart_turu, QWidget * parent,
                           int show_add_button, int prog_id,int modul_id,QString ekran_ismi , QString filtre_str , int islem_turu  )
{
    int secilen_id    = -1;
    CARI_KART_ARAMA * F = new CARI_KART_ARAMA ( cari_hesap_ismi_hesap_kodu, cari_kart_turu, show_add_button, parent,prog_id,modul_id , ekran_ismi, filtre_str , islem_turu );

    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   CARI_KART_ARAMA::CARI_KART_ARAMA
***************************************************************************************/

CARI_KART_ARAMA::CARI_KART_ARAMA ( QString cari_hesap_ismi_hesap_kodu, int cari_kart_turu,  int show_add_button,
                                    QWidget * parent,int prog_id,int modul_id ,QString ekran_ismi, QString filtre_str , int islem_turu ) : ARAMA_KERNEL (parent), m_ui(new Ui::CARI_KART_ARAMA)
{
    m_cari_kart_turu             = cari_kart_turu;
    m_cari_hesap_ismi_hesap_kodu = cari_hesap_ismi_hesap_kodu;
    m_show_add_button            = show_add_button;
    m_program_id                 = prog_id;
    m_modul_id                   = modul_id;
    m_ekran_ismi                 = ekran_ismi;
    m_filtre_str                 = filtre_str;
    m_islem_turu                 = islem_turu;

    m_ui->setupUi      (this);
    START_ARAMA_KERNEL ( this, DB );
}

/**************************************************************************************
                   CARI_KART_ARAMA::SETUP_FORM
***************************************************************************************/

void CARI_KART_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE     ( "cari_kart_arama.html" );

    // PERSOANEL MODULU BU ARAMAYI KULLANIYOR KENDI BASLIGI GELMESI LAZIM
    if ( m_program_id EQ E9_PROGRAMI AND m_modul_id NE PERSONEL_MODULU ) {
        if ( m_cari_kart_turu EQ SAHIS_KAYDI ) {
            SET_PAGE_TITLE("CARİ - ŞAHIS KARTI ARAMA");
        }
        else if( m_cari_kart_turu EQ FIRMA_KAYDI ) {
            SET_PAGE_TITLE("CARİ - FİRMA KARTI ARAMA");
        }
        else {
            SET_PAGE_TITLE("CARİ - CARİ KART ARAMA");
        }
    }

    m_ui->checkbox_cari_hesap_ismi_hesap_kodu->setChecked ( true  );
    m_ui->frame_ili_ilcesi->setEnabled                    ( false );

    m_ui->frame_telefon->setEnabled( false );
    m_ui->frame_sektor_or_meslek->setEnabled( false );

    if ( m_cari_kart_turu EQ SAHIS_KAYDI ) {
        GET_MESLEKLER( m_ui->comboBox_sektor_or_meslek );
    }
    else if ( m_cari_kart_turu EQ FIRMA_KAYDI ) {
        GET_SEKTORLER( m_ui->comboBox_sektor_or_meslek );
    }
    else {
        m_ui->frame_sektor_or_meslek->hide();
        m_ui->checkBox_sektor_or_meslek->hide();
    }

    GET_ILLER ( m_ui->combobox_sehir_ismi );

    REGISTER_TABLE_WIDGET         ( m_ui->table_widget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_button );

    SET_HEADERS(QStringList()<<tr("Cari \n Hesap Id")<<tr("Kodu")<<tr("Adı")
                << QObject::tr("Borcu") << "" << QObject::tr("Alacağı") << ""
                <<tr("Şehir")<<tr("İlçe")<<tr("Semt") << tr(""));

    SET_SORTING            ( false );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_hesap_ismi_hesap_kodu );

    m_ui->table_widget->hideColumn(0);

    m_ui->lineedit_hesap_ismi_hesap_kodu->setText( m_cari_hesap_ismi_hesap_kodu );

    if ( m_ui->lineedit_hesap_ismi_hesap_kodu->text().isEmpty() EQ false ) {
        AUTO_START ();
    }
    else {
        m_ui->lineedit_hesap_ismi_hesap_kodu->setText("*");
    }

    m_ui->checkbox_gruplar->setChecked  ( false );
    m_ui->frame_gruplar->setEnabled     ( false );

    m_ui->table_widget->setColumnWidth  ( 1, 90  );
    m_ui->table_widget->setColumnWidth  ( 2, 190 );
    m_ui->table_widget->setColumnWidth  ( 3, 90  );
    m_ui->table_widget->setColumnWidth  ( 4, 30  );
    m_ui->table_widget->setColumnWidth  ( 5, 90  );
    m_ui->table_widget->setColumnWidth  ( 6, 30  );
    m_ui->table_widget->setColumnWidth  ( 7, 90  );
    m_ui->table_widget->setColumnWidth  ( 8, 90  );
    m_ui->table_widget->setColumnWidth  ( 9, 90  );
    m_ui->table_widget->setColumnWidth  ( 10,25  );

    if ( m_islem_turu EQ CARI_KART_ARAMA_ISLEMI ) {

        m_ui->label_dosya_encoding->setVisible   ( false );
        m_ui->comboBox_dosya_encoding->setVisible( false );
        m_ui->tool_button_export->setVisible     ( false );

        SET_PAGE_TITLE    ( m_ekran_ismi );
    }
    else {
        QList< QByteArray > available_coding_list = QTextCodec::availableCodecs() ;
        for( int i = 0 ; i < available_coding_list.size() ; ++i ){
            m_ui->comboBox_dosya_encoding->addItem( available_coding_list.at(i) );
        }

        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_dosya_encoding, tr("UTF-8"));
        SET_PAGE_TITLE ( tr ( "DIŞARIYA KAYIT İHRAÇ ET") );
        AUTO_START();
    }

    SET_SETTING_NAME                    ( "CARI_KART_ARAMA"    );
    SET_ENTER_KEY_FOR_RUN_BATCH         ( true                 );
    SET_FIND_BUTTON_NAME                ( tr ( "    Bul    " ) );
}


/**************************************************************************************
                   CARI_KART_ARAMA::CHECK_VAR
***************************************************************************************/

int CARI_KART_ARAMA::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->checkbox_cari_hesap_ismi_hesap_kodu ) {
        if (m_ui->checkbox_cari_hesap_ismi_hesap_kodu->isChecked() EQ true) {
            m_ui->frame_cari_hesap_ismi_hesap_kodu->setEnabled(true);
            SET_FOCUS( m_ui->lineedit_hesap_ismi_hesap_kodu );
        }
        else {
            m_ui->frame_cari_hesap_ismi_hesap_kodu->setEnabled(false);
            m_ui->lineedit_hesap_ismi_hesap_kodu->clear();
        }
    }
    else if ( object EQ m_ui->checkbox_ili_ilcesi ) {

        if ( m_ui->checkbox_ili_ilcesi->isChecked() EQ true ) {
            m_ui->frame_ili_ilcesi->setEnabled(true);
            SET_FOCUS( m_ui->combobox_sehir_ismi );
        }
        else {
            m_ui->frame_ili_ilcesi->setEnabled(false);
            m_ui->combobox_sehir_ismi->setCurrentIndex(-1);;
            m_ui->lineedit_ilce->clear();
        }
    }
    else if ( object EQ m_ui->checkbox_gruplar ) {
        if ( m_ui->checkbox_gruplar->isChecked() EQ true ) {
            m_ui->frame_gruplar->setEnabled( true );
        }
        else {
            m_ui->frame_gruplar->setEnabled( false );
        }
    }
    else if ( object EQ m_ui->pushButton_grup_ekle ) {

        QStringList  grup_adlari;
        QStringList  grup_kodlari;

        if ( OPEN_GRUP_SECIM( m_program_id, m_modul_id , &m_grup_idleri, DB , this , true ) EQ true ) {
            m_ui->textEdit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri,&grup_adlari,&grup_kodlari));
        }
    }
    else if ( object EQ m_ui->checkBox_telefon ) {
        if ( m_ui->checkBox_telefon->isChecked() EQ true ) {
            m_ui->frame_telefon->setEnabled( true );
            m_ui->lineEdit_telefon->clear();
            SET_FOCUS( m_ui->lineEdit_telefon );
        }
        else {
            m_ui->frame_telefon->setEnabled( false );
        }
    }
    else if ( object EQ m_ui->checkBox_sektor_or_meslek ) {
        if ( m_ui->checkBox_sektor_or_meslek->isChecked() EQ true ) {
            m_ui->frame_sektor_or_meslek->setEnabled( true );
            m_ui->comboBox_sektor_or_meslek->setCurrentIndex( -1 );
            SET_FOCUS( m_ui->comboBox_sektor_or_meslek );
        }
        else {
            m_ui->frame_sektor_or_meslek->setEnabled( false );
        }
    }
    else if( object EQ m_ui->tool_button_export ){
        if ( m_cari_hesap_id_list.size() < 0 ) {
            MSG_ERROR("Dışarıya aktarılacak kayıt bulunmadı" , this);
            return ADAK_OK;
        }

        QByteArray array_for_encoding;
        array_for_encoding.insert( 0 , m_ui->comboBox_dosya_encoding->currentText() );
        if( m_islem_turu EQ CARI_KART_EXPORT_ISLEMI ){
            OPEN_CARI_KART_EXPORT( m_cari_hesap_id_list , this , array_for_encoding );
            SET_FOCUS( m_ui->tool_button_export );
        }
        if( m_islem_turu EQ PRS_KARTI_EXPORT_ISLEMI ){
            OPEN_PRS_KARTI_EXPORT( m_cari_hesap_id_list , this , array_for_encoding );
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_KART_ARAMA::CHECK_RUN
***************************************************************************************/

int CARI_KART_ARAMA::CHECK_RUN()
{
    if ( m_ui->checkbox_cari_hesap_ismi_hesap_kodu->isChecked() EQ true ) {
        if (m_ui->lineedit_hesap_ismi_hesap_kodu->text().isEmpty() EQ true) {
            MSG_WARNING( tr("Cari Hesap İsmine arama kriterini girmelisiniz!.."), m_ui->lineedit_hesap_ismi_hesap_kodu );

            return ADAK_FAIL;
        }
    }

    if ( m_ui->checkbox_ili_ilcesi->isChecked() EQ true ) {
        if (m_ui->combobox_sehir_ismi->currentIndex() EQ 0 AND
            m_ui->lineedit_ilce->text().isEmpty() EQ true ) {
            MSG_WARNING( tr("İl veya Semt / İlçe arama kriterini girmelisiniz!.."), m_ui->combobox_sehir_ismi );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CARI_KART_ARAMA::SEARCH
***************************************************************************************/

void CARI_KART_ARAMA::SEARCH()
{
    SQL_QUERY       f_query(DB);

    QStringList adres_id_sehir_ilce_list;

    f_query.PREPARE_SELECT( "car_hesaplar","cari_hesap_id, cari_hesap_kodu, adres_id,"
                                           "para_birimi_id ,toplam_borc_array, toplam_alacak_array" );


    if ( m_ui->checkbox_cari_hesap_ismi_hesap_kodu->isChecked() EQ true ) {
        if ( m_ui->lineedit_hesap_ismi_hesap_kodu->text() NE "*"){
            f_query.AND_EKLE( " ( cari_hesap_kodu  LIKE :cari_hesap_kodu OR cari_hesap_ismi LIKE :cari_hesap_ismi ) ");
            f_query.SET_LIKE( ":cari_hesap_kodu", m_ui->lineedit_hesap_ismi_hesap_kodu->text() );
            f_query.SET_LIKE( ":cari_hesap_ismi", m_ui->lineedit_hesap_ismi_hesap_kodu->text() );
        }
    }
    if ( m_program_id EQ E9_PROGRAMI ) {
        if ( m_modul_id NE CARI_MODULU ) {
            f_query.AND_EKLE    ( "modul_id = :modul_id ");
            f_query.SET_VALUE   ( ":modul_id", m_modul_id );
        }
    }
    else {
         f_query.AND_EKLE    ( "modul_id = :modul_id AND program_id =:program_id ");
         f_query.SET_VALUE   ( ":modul_id"  , m_modul_id );
         f_query.SET_VALUE   ( ":program_id", m_program_id );
    }

    if ( m_ui->combobox_sehir_ismi->currentIndex() NE -1 OR m_ui->lineedit_ilce->text().isEmpty() EQ false ) {
        adres_id_sehir_ilce_list = ADR_GET_ADRES_SEHIR_ILCE_GORE_ID_LIST( m_ui->combobox_sehir_ismi->currentText(), m_ui->lineedit_ilce->text() );
        if ( adres_id_sehir_ilce_list.size() > 0 ) {
            f_query.AND_MULTI_EKLE("adres_id" , f_query.TO_QVARIANT( adres_id_sehir_ilce_list ), ADAK_OR );
        }
        else {
            return;
        }
    }

    // filtre uyan degerleri gonderir.
    QStringList filtre_id_list;
    if ( m_filtre_str.isEmpty() EQ false ) {
        filtre_id_list = ADR_GET_ADRES_FILTRE_STR( m_filtre_str );
    }

    if ( m_ui->checkbox_gruplar->isChecked() EQ true ) {
         QList<int> gruptaki_cari_kart_id_list = GRP_GRUBA_EKLI_KAYITLARI_BUL ( m_program_id , m_modul_id , m_grup_idleri );
         if ( gruptaki_cari_kart_id_list.size() >  0 ) {
            f_query.AND_MULTI_EKLE("cari_hesap_id" , f_query.TO_QVARIANT( gruptaki_cari_kart_id_list ), ADAK_OR );
         }
         else {
             return;
         }
    }

    QStringList telefon_filtre_id_list;

    if ( m_ui->checkBox_telefon->isChecked() EQ true ) {
        telefon_filtre_id_list = ADR_GET_ADRES_TEL_FILTRE_LIST( m_ui->lineEdit_telefon->text().replace  ( "-", "" ) );

        if ( telefon_filtre_id_list.size() > 0 ) {
            f_query.AND_MULTI_EKLE("adres_id" , f_query.TO_QVARIANT( telefon_filtre_id_list ), ADAK_OR );
        }
        else {
            return;
        }
    }

    QStringList sektor_or_meslek_adres_id_list;

    if( m_ui->checkBox_sektor_or_meslek->isChecked() EQ true ) {
        ADAK_ADRES_CARI_ENUM kart_turu = SAHIS_KAYDI;
        if ( m_cari_kart_turu EQ FIRMA_KAYDI ) {
            kart_turu = FIRMA_KAYDI;
        }
        sektor_or_meslek_adres_id_list = ADR_GET_ADRES_SEKTOR_OR_MESLEK_FILTRE_LIST( m_ui->comboBox_sektor_or_meslek->currentText(), kart_turu );
        if ( sektor_or_meslek_adres_id_list.size() > 0 ) {
            f_query.AND_MULTI_EKLE("adres_id" , f_query.TO_QVARIANT( sektor_or_meslek_adres_id_list ), ADAK_OR );
        }
        else {
            return;
        }
    }

    if ( f_query.SELECT( "cari_hesap_kodu  ASC" ) EQ 0 ) {
        return;
    }

    double toplam_borc_array  [E9_ARRAY_SIZE];
    double toplam_alacak_array[E9_ARRAY_SIZE];

    m_cari_hesap_id_list.clear();

    while (f_query.NEXT() EQ true ) {

        int current_row = ADD_NEW_ROW();
        QString cari_hesap_id = f_query.VALUE("cari_hesap_id").toString() ;
        m_cari_hesap_id_list.append( cari_hesap_id.toInt() ) ;

        m_ui->table_widget->item(current_row,0)->setText(   cari_hesap_id );
        m_ui->table_widget->item(current_row,1)->setText(   f_query.VALUE("cari_hesap_kodu").toString());
        m_ui->table_widget->item(current_row,2)->setText(   ADR_GET_FIRMA_SAHIS_ADI(f_query.VALUE("adres_id").toInt()));

        UNPACK_DOUBLE_ARRAY(f_query.VALUE("toplam_borc_array").toString() , toplam_borc_array , E9_ARRAY_SIZE);
        UNPACK_DOUBLE_ARRAY(f_query.VALUE("toplam_alacak_array").toString() , toplam_alacak_array , E9_ARRAY_SIZE);

        double toplam_borc   = 0.00;
        double toplam_alacak = 0.00;

        toplam_borc   = ROUND ( toplam_borc_array  [ E9_SON_BAKIYE ] );
        toplam_alacak = ROUND ( toplam_alacak_array[ E9_SON_BAKIYE ] );

        m_ui->table_widget->item(current_row , 3)->setTextAlignment(Qt::AlignRight + Qt::AlignVCenter );
        m_ui->table_widget->item(current_row , 5)->setTextAlignment(Qt::AlignRight + Qt::AlignVCenter);

        if ( toplam_borc > toplam_alacak ) {

            m_ui->table_widget->item(current_row , 3)->setText(VIRGUL_EKLE(QVariant(ROUND(toplam_borc - toplam_alacak)).toString(),2));

            m_ui->table_widget->item(current_row , 4)->setText(DVZ_GET_DOVIZ_KODU(f_query.VALUE("para_birimi_id").toInt()));
            m_ui->table_widget->item(current_row , 5)->setText("");
            m_ui->table_widget->item(current_row , 6)->setText("");

        }
        else {
            m_ui->table_widget->item(current_row , 3)->setText("");
            m_ui->table_widget->item(current_row , 4)->setText("");
            m_ui->table_widget->item(current_row , 5)->setText(VIRGUL_EKLE(QVariant(ROUND(toplam_alacak - toplam_borc)).toString(),2));

            if ( toplam_alacak - toplam_borc EQ 0 ) {
                m_ui->table_widget->item(current_row , 6)->setText("");
            }
            else {
                m_ui->table_widget->item(current_row , 6)->setText(DVZ_GET_DOVIZ_KODU(f_query.VALUE("para_birimi_id").toInt()));
            }
        }

        int adres_id = f_query.VALUE("adres_id").toInt();

        QStringList adr_sehir_bilgileri = ADR_GET_ADRES_SEHIR_ILCE_SEMT_LIST( adres_id );

        if ( adr_sehir_bilgileri.isEmpty() EQ false ) {
            m_ui->table_widget->item(current_row,7)->setText( adr_sehir_bilgileri.at(0));// sehir
            m_ui->table_widget->item(current_row,8)->setText( adr_sehir_bilgileri.at(1));// ilce
            m_ui->table_widget->item(current_row,9)->setText( adr_sehir_bilgileri.at(2));// semt
        }
        else {
            m_ui->table_widget->item(current_row,7)->setText( "");// sehir
            m_ui->table_widget->item(current_row,8)->setText( "");// ilce
            m_ui->table_widget->item(current_row,9)->setText( "");// semt
        }

        QPushButton *button_hareketler = new QPushButton();

        button_hareketler->setIcon(QPixmap(":/e9_icons/E9_ICONS/cari_hareketler.png"));
        button_hareketler->setToolTip("Cari Hareketler");

        button_hareketler->setObjectName(cari_hesap_id);
        REGISTER_CHANGER_BUTTON(button_hareketler);

        m_button_list.insert( button_hareketler, cari_hesap_id.toInt());
        m_ui->table_widget->setCellWidget(current_row, 10, button_hareketler );
    }
}

/**************************************************************************************
                   CARI_KART_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int CARI_KART_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    if( m_islem_turu EQ CARI_KART_EXPORT_ISLEMI ){
        return ADAK_CONTINUE ;
    }

    *m_secilen_id = m_ui->table_widget->item(selected_row_number,0)->text().toInt();
    return ADAK_EXIT;
}

/**************************************************************************************
                   CARI_KART_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int CARI_KART_ARAMA::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{
    int cari_hesap_id = -1;

    if ( m_button_list.contains((QPushButton*)button) EQ false ) {
        return ADAK_RECORD_UNCHANGED;
    }
    for (int i = 0; i < m_button_list.size(); ++i) {
        if ( m_button_list.keys().at(i) EQ button ) {
            cari_hesap_id = m_button_list.values().at(i);
        }
    }

    OPEN_CARI_OTO_EKSTRE_ARAMA( cari_hesap_id, this );
    return ADAK_RECORD_UNCHANGED;
}
