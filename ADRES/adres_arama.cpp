#include <QMessageBox>
#include "adak_std_utils.h"
#include "ui_adres_arama.h"
#include "adres_arama_class.h"
#include "adres_arama_open.h"
#include "adak_gruplar.h"
#include "adres_enum.h"
#include "adres_export_open.h"
#include <QTextCodec>


extern ADAK_SQL *  DB;

#define RECORD_ID_COLUMN    0
#define ISIM_COLUMN         1
#define SEHIR_COLUMN        2
#define ILCE_COLUMN         3
#define SEMT_COLUMN         4
#define TELEFON_COLUMN      5
#define ULKE_COLUMN         6
#define ADRES_COLUMN        7
#define EMAIL_COLUMN        8
#define RECORD_TYPE_COLUMN  9

/**************************************************************************************
                   OPEN_ADRES_ARAMA
***************************************************************************************/

int OPEN_ADRES_ARAMA ( int kayit_turu,int islem_turu, QWidget * parent )
{
    int secilen_id = -1 ;

    ADRES_ARAMA * F = new ADRES_ARAMA ( kayit_turu ,islem_turu , false,parent  ) ;
    F->m_secilen_id = &secilen_id ;
    F->SHOW ( NOT_FULL_SCREEN );
    return secilen_id ;
}

/**************************************************************************************
                   OPEN_ADRES_ARAMA
***************************************************************************************/

QList<int> OPEN_ADRES_ARAMA_LIST ( int kayit_turu, bool is_multi_line, QWidget * parent )
{
    QList<int> secilen_id;

    ADRES_ARAMA * F = new ADRES_ARAMA ( kayit_turu,is_multi_line,parent,NULL ) ;

    F->m_secilen_id_list = &secilen_id ;
    F->EXEC( FULL_SCREEN );

    return secilen_id ;
}

/**************************************************************************************
                   ADRES_ARAMA::ADRES_ARAMA
***************************************************************************************/

ADRES_ARAMA::ADRES_ARAMA ( int kayit_turu, int islem_turu ,bool is_multi_line, QWidget *parent) : ARAMA_KERNEL ( parent ) ,
m_ui ( new Ui::ADRES_ARAMA ), p_kayit_turu ( kayit_turu )
{
    m_is_multi_line = is_multi_line;

    m_grup_idleri   = new QList<int >;
    m_grup_idleri->clear();

    m_islem_turu    = islem_turu;

    m_ui->setupUi       ( this ) ;

    START_ARAMA_KERNEL  ( this, DB );
}

/**************************************************************************************
                   ADRES_ARAMA::SETUP_FORM
***************************************************************************************/

void ADRES_ARAMA::SETUP_FORM()
{    
    REGISTER_TABLE_WIDGET         ( m_ui->table_widget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget       );
    SET_HELP_PAGE    ( tr ( "adres_arama.html" ) );

    m_ui->lineEdit_isim_icerigi->setText ( tr( "*" ) ) ;
    GET_ILLER(m_ui->comboBox_sehir);

    if ( p_kayit_turu EQ SAHIS_KAYDI ) {
        GET_MESLEKLER( m_ui->comboBox_sektor_or_meslek );
    }
    else if ( p_kayit_turu EQ FIRMA_KAYDI) {
        GET_SEKTORLER( m_ui->comboBox_sektor_or_meslek );
    }
    else {
        m_ui->frame_sektor_or_meslek->hide();
        m_ui->checkBox_sektor_or_meslek->hide();
    }

    m_ui->checkBox_adi->setChecked          ( true  );
    m_ui->frame_telefonu->setEnabled        ( false );
    m_ui->frame_ili_ilcesi->setEnabled      ( false );
    m_ui->frame_email->setEnabled           ( false );
    m_ui->frame_grup_filtresi->setEnabled   ( false );
    m_ui->frame_sektor_or_meslek->setEnabled( false );

    SET_ENTER_KEY_FOR_RUN_BATCH   ( true  );
    SET_SORTING                   ( false );

    if ( m_is_multi_line EQ true ) {
        ENABLE_MULTI_ROW_SELECTION();
    }

    SET_HEADERS(QStringList()<<tr ( "adres_id" ) <<tr ( "Firma/Şahıs Adı" ) <<tr( "Şehir" ) <<tr ( "İlçe" ) << tr("Semt") << tr ( "Telefon" )
                            << tr( "Ülke" ) << tr( "Adres" ) << tr ( "Email" ) << tr( "record_type" ) ) ;

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_isim_icerigi );

    m_ui->table_widget->hideColumn ( RECORD_ID_COLUMN );
    m_ui->table_widget->hideColumn ( RECORD_TYPE_COLUMN );

    if ( m_islem_turu EQ ADRES_ARAMA_ISLEMI ) {
        m_ui->table_widget->hideColumn ( ULKE_COLUMN  );
        m_ui->table_widget->hideColumn ( ADRES_COLUMN );
        m_ui->table_widget->hideColumn ( EMAIL_COLUMN );
    }
    else {
        m_ui->table_widget->hideColumn ( SEMT_COLUMN );
    }

    m_ui->table_widget->setColumnWidth ( ISIM_COLUMN    ,  210 );
    m_ui->table_widget->setColumnWidth ( SEHIR_COLUMN   ,  95  );
    m_ui->table_widget->setColumnWidth ( ULKE_COLUMN    ,  95  );
    m_ui->table_widget->setColumnWidth ( ILCE_COLUMN    ,  95  );
    m_ui->table_widget->setColumnWidth ( SEMT_COLUMN    ,  95  );
    m_ui->table_widget->setColumnWidth ( TELEFON_COLUMN ,  120 );
    m_ui->table_widget->setColumnWidth ( ADRES_COLUMN   ,  210 );
    m_ui->table_widget->setColumnWidth ( EMAIL_COLUMN   ,  150 );

    SET_SETTING_NAME     ( "ADRES_ARAMA" );

    SET_FIND_BUTTON_NAME ( tr ( "Bul" ) );

    m_ui->comboBox_dosya_encoding->setVisible(false);
    m_ui->label_dosya_encoding->setVisible(false);

    if ( m_islem_turu EQ ADRES_EXPORT_ISLEMI ) {
        m_ui->comboBox_dosya_encoding->setVisible(true);
        m_ui->label_dosya_encoding->setVisible(true);

        QList< QByteArray > available_coding_list = QTextCodec::availableCodecs() ;
        for( int i = 0 ; i < available_coding_list.size() ; ++i ){
            m_ui->comboBox_dosya_encoding->addItem( available_coding_list.at(i) );
        }

        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_dosya_encoding, tr("UTF-8"));

        SET_PAGE_TITLE ( tr ( "DIŞARIYA KAYIT İHRAÇ ET") );
        AUTO_START();
    }
    else {
        m_ui->tool_button_export->hide();
        SET_PAGE_TITLE ( tr ( "ADRES ARAMA ") );
    }
}

/**************************************************************************************
                   ADRES_ARAMA::CHECK_VAR
***************************************************************************************/

int ADRES_ARAMA::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->comboBox_sehir ) {
        m_ui->comboBox_ilce->clear () ;
        GET_ILCELER ( m_ui->comboBox_ilce, m_ui->comboBox_sehir->currentText() );
    }
    else if ( object EQ m_ui->pushButton_grup_sec ) {

        OPEN_GRUP_SECIM( E9_PROGRAMI, ADRES_MODULU, m_grup_idleri, DB, this );

        QString grup_str = GRP_GRUP_ADLARINI_BUL ( m_grup_idleri, new QStringList() , new QStringList() );
        m_ui->textEdit_gruplar->setPlainText( grup_str );
    }
    else if ( object EQ m_ui->checkBox_grup_filtresi ) {
         if ( m_ui->checkBox_grup_filtresi->isChecked () EQ true ) {
              m_ui->frame_grup_filtresi->setEnabled ( true );
              SET_FOCUS( m_ui->pushButton_grup_sec );
        }
        else {
            m_ui->frame_grup_filtresi->setEnabled ( false );
            m_ui->textEdit_gruplar->clear ();
        }
    }
    else if ( object EQ m_ui->checkBox_adi ) {
         if ( m_ui->checkBox_adi->isChecked () EQ true ) {
              m_ui->frame_adi->setEnabled ( true );
              SET_FOCUS( m_ui->lineEdit_isim_icerigi );
        }
        else {
            m_ui->frame_adi->setEnabled ( false );
            m_ui->lineEdit_isim_icerigi->clear ();
        }
    }
    else if ( object EQ m_ui->checkBox_telefonu ) {

         if ( m_ui->checkBox_telefonu->isChecked () EQ true ) {
             m_ui->frame_telefonu->setEnabled ( true );
             SET_FOCUS( m_ui->lineEdit_telefon );
        }
        else {
            m_ui->frame_telefonu->setEnabled ( false );
            m_ui->lineEdit_telefon->clear ();
        }
    }
    else if ( object EQ m_ui->checkBox_il_ilce ) {
         if ( m_ui->checkBox_il_ilce->isChecked() EQ true ) {
             m_ui->frame_ili_ilcesi->setEnabled ( true );
             SET_FOCUS( m_ui->comboBox_sehir );
        }
        else {
            m_ui->frame_ili_ilcesi->setEnabled ( false );
            m_ui->comboBox_sehir->currentText ().clear ();
            m_ui->comboBox_ilce->currentText ().clear ();
        }
    }
    else if ( object EQ m_ui->check_box_email ) {
        if ( m_ui->check_box_email->isChecked () EQ true ) {
            m_ui->frame_email->setEnabled ( true );
            SET_FOCUS( m_ui->line_edit_email);
       }
       else {
           m_ui->frame_email->setEnabled ( false );
           m_ui->line_edit_email->clear ();
       }
    }
    else if ( object EQ m_ui->lineEdit_isim_icerigi ) {
         if ( m_ui->lineEdit_isim_icerigi->text().contains("'") EQ true ) {
            MSG_WARNING( tr ( "Arama kriterine tek tırnak karakteri (') girilemez" ), m_ui->lineEdit_isim_icerigi );
            return ADAK_FAIL_UNDO;
        }
    }
    else if ( object EQ m_ui->tool_button_export ) {

        if ( m_adr_export_id_list.isEmpty() EQ false ) {
            OPEN_ADRES_EXPORT( m_adr_export_id_list , this);
        }
        else{
            MSG_ERROR("Dışarıya aktarılacak kayıt bulunmadı" , this);
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

    return ADAK_OK;
}


/**************************************************************************************
                   ADRES_ARAMA::CHECK_RUN
***************************************************************************************/

int ADRES_ARAMA::CHECK_RUN ()
{
    if ( m_ui->checkBox_adi->isChecked() EQ true ) {
        if ( m_ui->lineEdit_isim_icerigi->text().isEmpty() EQ true ) {
            MSG_WARNING( tr( "İsim bilgisini boş bırakamazsınız." ), m_ui->lineEdit_isim_icerigi );
            return ADAK_FAIL;
        }
    }

    if ( m_ui->checkBox_telefonu->isChecked() EQ true ) {
        if ( m_ui->lineEdit_telefon->text().isEmpty() EQ true ) {
            MSG_WARNING( tr( "Telefonun bilgisini boş bırakamazsınız." ), m_ui->lineEdit_telefon );
            return ADAK_FAIL ;
        }
    }

    if ( m_ui->checkBox_il_ilce->isChecked() EQ true ) {
        if ( m_ui->comboBox_sehir->currentText().isEmpty() EQ true AND m_ui->comboBox_ilce->currentText().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "İlini veya ilçe bilgisi girmelisiniz" ), m_ui->comboBox_sehir ) ;
            return ADAK_FAIL;
        }
    }

    if ( m_ui->check_box_email->isChecked() EQ true ) {
        if ( m_ui->line_edit_email->text().isEmpty() EQ true ) {
            MSG_WARNING( tr( "E-Mail bilgisini boş bırakamazsınız." ), m_ui->line_edit_email );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}


/**************************************************************************************
                   ADRES_ARAMA::SEARCH
***************************************************************************************/

void ADRES_ARAMA::SEARCH()
{
    SQL_QUERY query (DB);

    //QString column_names = "adres_id, firma_sahis_adi, sehir, ilce, semt,tel_ulke_kodu, tel_alan_kodu, telefon, adres_kart_turu";
    QString column_names = "adres_id, firma_sahis_adi, sehir, ilce, semt,tel_ulke_kodu, tel_alan_kodu, telefon, adres_kart_turu, ulke, email, adres_satiri_1, adres_satiri_2, adres_satiri_3";
    QString group_by     = column_names; //"adres_id, firma_sahis_adi, sehir, ilce, semt,tel_ulke_kodu, tel_alan_kodu, telefon, adres_kart_turu";
    QString order_by     = "firma_sahis_adi";
    QString table_name   = "adr_adresler";
    QByteArray text_codec;
    text_codec.insert( 0 , m_ui->comboBox_dosya_encoding->currentText() );
    QTextCodec *codec = QTextCodec::codecForName( text_codec );

    if ( m_ui->check_box_email->isChecked() EQ true ) {
        table_name.append(",adr_emailler");
    }

    query.PREPARE_SELECT(table_name,column_names,"",group_by);

    if (m_ui->check_box_email->isChecked() EQ true) {
        query.AND_EKLE("adr_emailler.adres_kayit_id = adres_id");
        query.AND_EKLE("( email LIKE :email OR email_adresi LIKE :email_adresi ) ");
        query.SET_LIKE(":email", m_ui->line_edit_email->text() );
        query.SET_LIKE(":email_adresi", m_ui->line_edit_email->text());
    }

    if ( m_ui->checkBox_adi->isChecked() EQ true ) {
        query.AND_EKLE("firma_sahis_adi LIKE :firma_sahis_adi");
        query.SET_LIKE(":firma_sahis_adi",m_ui->lineEdit_isim_icerigi->text());
    }

    if ( m_ui->checkBox_telefonu->isChecked() EQ true ) {
        query.AND_EKLE(" ( telefon LIKE :telefon OR is_telefonu LIKE :is_telefonu OR cep_telefonu LIKE :cep_telefonu ) ");
        query.SET_LIKE(":telefon"      ,m_ui->lineEdit_telefon->text().replace  ( "-", "" ) );
        query.SET_LIKE(":is_telefonu"  ,m_ui->lineEdit_telefon->text().replace  ( "-", "" ) );
        query.SET_LIKE(":cep_telefonu" ,m_ui->lineEdit_telefon->text().replace  ( "-", "" ) );
    }

    if ( m_ui->checkBox_il_ilce->isChecked() EQ true ) {
        query.AND_EKLE("sehir LIKE :sehir AND ilce LIKE :ilce ");
        query.SET_LIKE(":sehir",m_ui->comboBox_sehir->currentText());
        query.SET_LIKE(":ilce",m_ui->comboBox_ilce->currentText());
    }

    if ( m_ui->checkBox_sektor_or_meslek->isChecked()){
        if ( p_kayit_turu EQ SAHIS_KAYDI ) {
            query.AND_EKLE ("meslek = :meslek");
            query.SET_VALUE(":meslek",m_ui->comboBox_sektor_or_meslek->currentText());
        }
        else if ( p_kayit_turu EQ FIRMA_KAYDI ) {
            query.AND_EKLE ("firma_sektoru = :firma_sektoru");
            query.SET_VALUE(":firma_sektoru",m_ui->comboBox_sektor_or_meslek->currentText());
        }
    }

    if ( m_ui->checkBox_grup_filtresi->isChecked() EQ true ) {
        QList < int > gruplardaki_kayit_listesi;
        gruplardaki_kayit_listesi = GRP_GRUBA_EKLI_KAYITLARI_BUL( E9_PROGRAMI, ADRES_MODULU, *m_grup_idleri );
        query.AND_MULTI_EKLE("adres_id",query.TO_QVARIANT(gruplardaki_kayit_listesi),ADAK_OR);
    }

    switch ( p_kayit_turu ) {
        case FIRMA_KAYDI :
        case SAHIS_KAYDI :
            query.AND_EKLE("adres_kart_turu = :adres_kart_turu ");
            query.SET_VALUE(":adres_kart_turu",p_kayit_turu);
            break;
        case SAHIS_ARTI_FIRMA :
        default               :
            break;
    }

    int         current_row = 0;
    QString     telefon = "";
    QString      adres  = "";

     m_adr_export_id_list.clear();

    if ( query.SELECT(order_by) EQ 0 ) {
        return;
    }

    while ( query.NEXT() EQ true ) {

        m_adr_export_id_list << query.VALUE(0).toInt();

        current_row = ADD_NEW_ROW();

        telefon.clear();

        if ( m_islem_turu EQ ADRES_EXPORT_ISLEMI ) {

            if ( query.VALUE ( 7 ).toString().isEmpty() EQ false ) {
                telefon = codec->toUnicode( query.VALUE ( 5 ).toByteArray() )
                        + codec->toUnicode( query.VALUE ( 6 ).toByteArray() )
                        + codec->toUnicode( query.VALUE ( 7 ).toByteArray() ) ;
            }

                adres = codec->toUnicode( query.VALUE( 11 ).toByteArray() )
                      + codec->toUnicode( query.VALUE( 12 ).toByteArray() )
                      + codec->toUnicode( query.VALUE( 13 ).toByteArray() );

            m_ui->table_widget->item ( current_row, RECORD_ID_COLUMN   )->setText ( codec->toUnicode( query.VALUE ( 0 ).toByteArray() ) ) ;
            m_ui->table_widget->item ( current_row, ISIM_COLUMN        )->setText ( codec->toUnicode( query.VALUE ( 1 ).toByteArray() ) ) ;
            m_ui->table_widget->item ( current_row, TELEFON_COLUMN     )->setText ( telefon ) ;
            m_ui->table_widget->item ( current_row, ULKE_COLUMN        )->setText ( codec->toUnicode( query.VALUE ( 9 ).toByteArray() ) ) ;
            m_ui->table_widget->item ( current_row, SEHIR_COLUMN       )->setText ( codec->toUnicode( query.VALUE ( 2 ).toByteArray() ) );
            m_ui->table_widget->item ( current_row, ILCE_COLUMN        )->setText ( codec->toUnicode( query.VALUE ( 3 ).toByteArray() ) );
            m_ui->table_widget->item ( current_row, ADRES_COLUMN       )->setText ( adres );
            m_ui->table_widget->item ( current_row, EMAIL_COLUMN       )->setText ( codec->toUnicode( query.VALUE ( 10 ).toByteArray() ) );
        }
        else {
            m_ui->table_widget->item ( current_row, RECORD_ID_COLUMN   )->setText ( query.VALUE ( 0 ).toString() ) ;
            m_ui->table_widget->item ( current_row, ISIM_COLUMN        )->setText ( query.VALUE ( 1 ).toString() ) ;
            m_ui->table_widget->item ( current_row, SEHIR_COLUMN       )->setText ( query.VALUE ( 2 ).toString() ) ;
            m_ui->table_widget->item ( current_row, ILCE_COLUMN        )->setText ( query.VALUE ( 3 ).toString() ) ;
            m_ui->table_widget->item ( current_row, SEMT_COLUMN        )->setText ( query.VALUE ( 4 ).toString() ) ;
            m_ui->table_widget->item ( current_row, TELEFON_COLUMN     )->setText ( query.VALUE ( 5 ).toString() + "-" + query.VALUE ( 6 ).toString() + "-" + query.VALUE( 7 ).toString() );
            m_ui->table_widget->item ( current_row, RECORD_TYPE_COLUMN )->setText ( query.VALUE ( 8 ).toString() ) ;
        }

    }
}

/**************************************************************************************
                   ADRES_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int ADRES_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    if ( m_islem_turu EQ ADRES_EXPORT_ISLEMI ) {
        return ADAK_CONTINUE;
    }

    int id = m_ui->table_widget->item(selected_row_number, RECORD_ID_COLUMN )->text().toInt();

    if (m_is_multi_line EQ true ) {
        *m_secilen_id_list << id;
    }
    else {
        *m_secilen_id      =  id;
    }

    return ADAK_EXIT;
}

/**************************************************************************************
                   ADRES_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int ADRES_ARAMA::MULTI_LINE_SELECTED(QList<int> selected_rows)
{
    if ( m_islem_turu EQ ADRES_EXPORT_ISLEMI ) {
        return ADAK_CONTINUE;
    }

    for ( int i = 0 ; i < selected_rows.size() ; i++ ) {
        *m_secilen_id_list << m_ui->table_widget->item(selected_rows.at(i), RECORD_ID_COLUMN )->text().toInt();
    }

    return ADAK_EXIT;
}
