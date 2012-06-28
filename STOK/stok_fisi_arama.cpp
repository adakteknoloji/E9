#include <adak_utils.h>
#include <adak_sql.h>
#include "stok_fisi_arama_class.h"
#include "ui_stok_fisi_arama.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "stok_enum.h"
#include "stok_console_utils.h"
#include "irs_gui_utils.h"
#include "stok_gui_utils.h"
#include "irs_console_utils.h"
#include "cari_kart_arama_open.h"
#include "cari_console_utils.h"

#define     FIS_ID_COLUMN           0
#define     FIS_TARIHI_COLUMN       1
#define     CARI_HESAP_ISMI_COLUMN  2
#define     FIS_NO_COLUMN           3
#define     FIS_TURU_COLUMN         4

extern ADAK_SQL * DB;

/**************************************************************************************
                                OPEN_STOK_FISI_ARAMA
***************************************************************************************/

int OPEN_STOK_FISI_ARAMA ( int p_irsaliye_stok_fisi_alis_satis_turu, int p_irsaliye_mi_stok_fisi_mi, int p_iade_irsaliyesi_mi, int p_fis_turu, QWidget * p_parent )
{
    int secilen_id    = -1;
    STOK_FISI_ARAMA * F = new STOK_FISI_ARAMA (  p_irsaliye_stok_fisi_alis_satis_turu, p_irsaliye_mi_stok_fisi_mi,
                                                 &secilen_id, p_iade_irsaliyesi_mi, p_fis_turu, p_parent );
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                                STOK_FISI_ARAMA::STOK_FISI_ARAMA
***************************************************************************************/

STOK_FISI_ARAMA::STOK_FISI_ARAMA ( int p_irsaliye_stok_fisi_alis_satis_turu, int p_irsaliye_mi_stok_fisi_mi,int * secilen_id,
                                  int p_iade_irsaliyesi_mi, int p_fis_turu, QWidget * p_parent )
: ARAMA_KERNEL ( p_parent ), m_ui ( new Ui::STOK_FISI_ARAMA )
{
    m_irsaliye_stok_fisi_alis_satis_turu    = p_irsaliye_stok_fisi_alis_satis_turu;
    m_irsaliye_mi_stok_fisi_mi              = p_irsaliye_mi_stok_fisi_mi;
    m_secilen_id                            = secilen_id;
    m_iade_irsaliyesi_mi                    = p_iade_irsaliyesi_mi;

    m_fis_turu                              = p_fis_turu;

    m_ui->setupUi       (this );
    START_ARAMA_KERNEL  (this, DB );

}

/**************************************************************************************
                                STOK_FISI_ARAMA::SETUP_FORM
***************************************************************************************/

void STOK_FISI_ARAMA::SETUP_FORM()
{
    switch ( m_irsaliye_mi_stok_fisi_mi ) {

        case ENUM_STOK_FISI :
        default             :
            SET_HELP_PAGE  ( "stok_fisi_arama.html"  );
            SET_PAGE_TITLE ( tr ( "STK - STOK FİŞİ ARAMA") );
            break;

        case ENUM_IRSALIYE :
            SET_HELP_PAGE  ( "irsaliye_arama.html"  );
            SET_PAGE_TITLE ( tr ( "İRSALİYE ARAMA") );
            break;
    }


    m_ui->frame_cari_hesap_kodu->setEnabled( false );

    m_ui->adakDate_baslangic_tarihi->SET_DATE( QDate::currentDate().addDays( -7 ) );
    m_ui->adakDate_bitis_tarihi->SET_DATE( QDate::currentDate() );

    if ( m_irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        IRS_FILL_IRSALIYE_TURU_COMBOBOX ( m_ui->comboBox_fis_turu, m_irsaliye_stok_fisi_alis_satis_turu, m_fis_turu );
        m_ui->label_fis_turu->setText( "İrsaliye Türü        " );
    }
    else {
        STK_FILL_STOK_FIS_TURU_COMBOBOX ( m_ui->comboBox_fis_turu );
    }

    m_ui->comboBox_fis_turu->setCurrentIndex(-1);

    m_ui->checkBox_tarih_araligi->setChecked ( true  );
    m_ui->frame_aciklama_icerigi->setEnabled ( false );
    m_ui->frame_fis_turu->setEnabled         ( false );

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS(QStringList()<<tr("fis_id")<<tr("Fiş Tarihi") << tr( "Cari Hesap İsmi" )<<tr("Fiş No")<<tr("Fiş Türü"));

    SET_SORTING                 ( false );

    SET_FIRST_FOCUS_WIDGET      ( m_ui->adakDate_baslangic_tarihi );

    m_ui->tableWidget->setColumnWidth ( FIS_TARIHI_COLUMN,      100 );
    m_ui->tableWidget->setColumnWidth ( CARI_HESAP_ISMI_COLUMN, 180 );
    m_ui->tableWidget->setColumnWidth ( FIS_NO_COLUMN,          60  );
    m_ui->tableWidget->setColumnWidth ( FIS_TURU_COLUMN,        140 );

    switch ( m_irsaliye_mi_stok_fisi_mi ) {

        case ENUM_STOK_FISI :
        default             :
            SET_SETTING_NAME  ( "STOK_FIS_ARAMA" );
            break;

        case ENUM_IRSALIYE :
            SET_SETTING_NAME  ( "IRSALIYE_ARAMA" );
            break;
    }

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->frame_fis_filtresi->setHidden(false);
    }

    m_ui->tableWidget->hideColumn( FIS_ID_COLUMN );

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );
    SET_FIND_BUTTON_NAME        ( tr ( "Ara" ) );

}

/**************************************************************************************
                                STOK_FISI_ARAMA::CHECK_VAR
***************************************************************************************/

int STOK_FISI_ARAMA::CHECK_VAR ( QObject * p_object )
{
    if (  ( p_object EQ m_ui->adakDate_baslangic_tarihi ) OR ( p_object EQ m_ui->adakDate_bitis_tarihi ) ) {
        if ( m_ui->adakDate_baslangic_tarihi->QDATE() > m_ui->adakDate_bitis_tarihi->QDATE() ) {
            MSG_WARNING(  tr("Başlangıç tarihi bitiş tarihinden büyük olamaz!"), m_ui->adakDate_baslangic_tarihi );

            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ m_ui->checkBox_tarih_araligi ) {

        if ( m_ui->checkBox_tarih_araligi->isChecked() EQ true ) {
            m_ui->frame_tarih_araligi->setEnabled(true);
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled(false);
        }
    }
    else if ( p_object EQ m_ui->checkBox_aciklama_icerigi ) {

        if ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ true ) {
            m_ui->frame_aciklama_icerigi->setEnabled(true);
        }
        else {
            m_ui->frame_aciklama_icerigi->setEnabled(false);
            m_ui->lineEdit_aciklama_icerigi->clear();
        }
    }
    else if ( p_object EQ m_ui->checkBox_fis_turu ) {

        if ( m_ui->checkBox_fis_turu->isChecked() EQ true ) {
            m_ui->frame_fis_turu->setEnabled(true);
        }
        else {
            m_ui->frame_fis_turu->setEnabled(false);
            m_ui->comboBox_fis_turu->setCurrentIndex(-1);
        }
    }
    else if ( p_object EQ m_ui->checkBox_cari_hesap_kodu ) {
        if ( m_ui->checkBox_cari_hesap_kodu->isChecked() EQ true ) {
            m_ui->frame_cari_hesap_kodu->setEnabled( true );
        }
        else {
            m_ui->frame_cari_hesap_kodu->setEnabled( false );
        }

        m_ui->searchEdit_cari_hesap_kodu->CLEAR();
        m_ui->lineEdit_cari_hesap_ismi->clear();
    }
    else if ( p_object EQ m_ui->searchEdit_cari_hesap_kodu ) {
        QString cari_hesap_ismi;

        int sonuc = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &m_cari_hesap_id, &cari_hesap_ismi );

        if ( sonuc EQ 0 ) {
            MSG_WARNING( "Cari Hesap bulunamadı.", m_ui->searchEdit_cari_hesap_kodu );
            m_ui->lineEdit_cari_hesap_ismi->clear();
            return ADAK_FAIL;
        }

        m_ui->lineEdit_cari_hesap_ismi->setText( cari_hesap_ismi );
    }
    return ADAK_OK;
}

/**************************************************************************************
                                STOK_FISI_ARAMA::CHECK_RUN
***************************************************************************************/

int STOK_FISI_ARAMA::CHECK_RUN()
{
    if ( ( m_ui->checkBox_tarih_araligi->isChecked() EQ false )
        AND ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ false )
        AND ( m_ui->checkBox_fis_turu->isChecked() EQ false ) ) {
        MSG_WARNING(  tr("En az bir kriter seçimi yapmalısınız."), m_ui->checkBox_tarih_araligi );
        return ADAK_FAIL;
    }
    if ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ true ) {
        if ( m_ui->lineEdit_aciklama_icerigi->text().isEmpty() EQ true ) {
            MSG_WARNING(  tr("Açıklama içeriği boş bırakılamaz!"), m_ui->lineEdit_aciklama_icerigi );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_fis_turu->isChecked() EQ true ) {
        if ( m_ui->comboBox_fis_turu->currentIndex() EQ -1 ) {
            MSG_WARNING(  tr("Fiş türünü seçmelisiniz!"), m_ui->comboBox_fis_turu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_cari_hesap_kodu->isChecked() EQ true ) {
        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true  ) {
            MSG_WARNING(  tr("Cari Hesap seçmelisiniz!"), m_ui->searchEdit_cari_hesap_kodu );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                                STOK_FISI_ARAMA::SEARCH
***************************************************************************************/

void STOK_FISI_ARAMA::SEARCH()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT( "stk_fisler", "fis_id, fis_no, fis_tarihi, fis_turu, cari_hesap_ismi " );

    sql_query.AND_EKLE  ( "irsaliye_mi_stok_fisi_mi = :irsaliye_mi_stok_fisi_mi" );
    sql_query.AND_EKLE  ( "irsaliye_alis_satis_turu = :irsaliye_alis_satis_turu" );

    sql_query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi", m_irsaliye_mi_stok_fisi_mi );
    sql_query.SET_VALUE ( ":irsaliye_alis_satis_turu", m_irsaliye_stok_fisi_alis_satis_turu );


    if ( m_ui->checkBox_cari_hesap_kodu->isChecked() EQ true ) {
        sql_query.AND_EKLE   ( "cari_hesap_id = :cari_hesap_id ");
        sql_query.SET_VALUE  ( ":cari_hesap_id", m_cari_hesap_id );
    }

    if ( m_ui->checkBox_tarih_araligi->isChecked() EQ true ) {

        sql_query.AND_EKLE  ( "fis_tarihi BETWEEN :fis_tarihi_1 AND :fis_tarihi_2 " );
        sql_query.SET_VALUE ( ":fis_tarihi_1" , m_ui->adakDate_baslangic_tarihi->QDATE().toString("yyyy.MM.dd") );
        sql_query.SET_VALUE ( ":fis_tarihi_2" , m_ui->adakDate_bitis_tarihi->QDATE().toString("yyyy.MM.dd")     );

    }
    if ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ true ) {      
        sql_query.AND_EKLE( "aciklama LIKE :aciklama ");
        sql_query.SET_LIKE( ":aciklama", m_ui->lineEdit_aciklama_icerigi->text() );
    }
    if ( m_ui->checkBox_fis_turu->isChecked() EQ true ) {  
        sql_query.AND_EKLE  ( "fis_turu = :fis_turu" );
        sql_query.SET_VALUE ( ":fis_turu" , STK_GET_STOK_FIS_TURU_ENUM( m_ui->comboBox_fis_turu->currentText() ) );
    }
    if ( m_irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ){
        sql_query.AND_EKLE   ( " iade_irsaliyesi_mi = :iade_irsaliyesi_mi ");
        sql_query.SET_VALUE  ( ":iade_irsaliyesi_mi", m_iade_irsaliyesi_mi );
    }

    if ( sql_query.SELECT( "fis_tarihi, fis_no" ) EQ 0 ) {
        return;
    }
    int current_row = 0;

    while ( sql_query.NEXT() EQ true ) {

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item ( current_row, FIS_ID_COLUMN          )->setText(sql_query.VALUE(0).toString());
        m_ui->tableWidget->item ( current_row, FIS_TARIHI_COLUMN      )->setText(QDate::fromString(sql_query.VALUE(2).toString(), "yyyy.MM.dd").toString("dd MMMM yyyy dddd"));
        m_ui->tableWidget->item ( current_row, CARI_HESAP_ISMI_COLUMN )->setText(sql_query.VALUE(4).toString());
        m_ui->tableWidget->item ( current_row, FIS_NO_COLUMN          )->setText(sql_query.VALUE(1).toString());
        m_ui->tableWidget->item ( current_row, FIS_TURU_COLUMN        )->setText(STK_GET_STOK_FIS_TURU(sql_query.VALUE(3).toInt()));

    }

}

/**************************************************************************************
                                STOK_FISI_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int STOK_FISI_ARAMA::SINGLE_LINE_SELECTED ( int p_selected_row_number)
{
    *m_secilen_id = m_ui->tableWidget->item ( p_selected_row_number, 0 )->text().toInt();
    return ADAK_EXIT;
}

/**************************************************************************************
                                STOK_FISI_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

void STOK_FISI_ARAMA::SEARCH_EDIT_CLICKED( QWidget *widget, QLineEdit *line_edit )
{
    Q_UNUSED ( line_edit );

    if ( widget EQ m_ui->searchEdit_cari_hesap_kodu ) {
        m_cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA , this, 0, E9_PROGRAMI, CARI_MODULU );// SAHIS + FIRMA
        QString cari_hesap_kodu;
        CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_cari_hesap_id, &cari_hesap_kodu );
        line_edit->setText( cari_hesap_kodu );
    }

}
