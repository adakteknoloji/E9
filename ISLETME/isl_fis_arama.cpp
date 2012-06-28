#include "adak_defines.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "isl_fis_arama_class.h"
#include "isl_fis_arama_open.h"
#include "isl_enum.h"
#include "e9_console_utils.h"
#include "isl_console_utils.h"

#define FIS_ID_COLUMN               0
#define FIS_TARIHI_COLUMN           1
#define FIS_NO_COLUMN               2
#define FIS_TURU_COLUMN             3
#define ACIKLAMA_COLUMN             4
#define KDV_HARIC_TUTAR_COLUMN      5
#define KDV_TUTARI_COLUMN           6
#define TOPLAM_TUTAR_COLUMN         7

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_ISL_FIS_ARAMA
***************************************************************************************/

int OPEN_ISL_FIS_ARAMA ( QWidget * parent )
{
    int secilen_id = -1;
    ISL_FIS_ARAMA * F = new ISL_FIS_ARAMA ( parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   ISL_FIS_ARAMA::ISL_FIS_ARAMA
***************************************************************************************/

ISL_FIS_ARAMA::ISL_FIS_ARAMA ( QWidget *parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::ISL_FIS_ARAMA )
{
    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, DB );
}

/**************************************************************************************
                   ISL_FIS_ARAMA::CHECK_VAR
***************************************************************************************/

void ISL_FIS_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE ( tr("gelir-ve-gider-fisleri") );

    m_fis_turu = ENUM_GELIR_FISI;

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS ( QStringList() << tr ( "Fiş Id" )     << tr ( "Fiş Tarihi" ) << tr ( "Fiş No" )
                                << tr ( "Fiş Türü" )   << tr ( "Açıklama" )   << tr ( "Kdv Hariç Toplam" )
                                << tr ( "Kdv Toplam" ) << tr ( "Kdv Dahil Toplam" ) );

    SET_ENTER_KEY_FOR_RUN_BATCH                 ( true );

    SET_FIND_BUTTON_NAME( tr ( "Bul" ) );
    SET_PAGE_TITLE( tr ( "İŞL - İŞLETME FİŞİ ARAMA" ) );

    m_ui->tableWidget->hideColumn               ( 0 );

    m_ui->check_box_tarih_araligi->setChecked   ( true );
    m_ui->frame_aciklama_icerigi->setDisabled   ( true );
    m_ui->frame_fis_turu->setDisabled           ( true );

    m_ui->date_edit_first_date->setDate         ( MALI_YIL_FIRST_DATE() );
    m_ui->date_edit_second_date->setDate        ( MALI_YIL_FIRST_DATE() );

    m_ui->tableWidget->setColumnWidth ( FIS_TARIHI_COLUMN,      150 );
    m_ui->tableWidget->setColumnWidth ( FIS_NO_COLUMN,          60  );
    m_ui->tableWidget->setColumnWidth ( FIS_TURU_COLUMN,        100 );
    m_ui->tableWidget->setColumnWidth ( ACIKLAMA_COLUMN,        250 );
    m_ui->tableWidget->setColumnWidth ( KDV_HARIC_TUTAR_COLUMN, 120 );
    m_ui->tableWidget->setColumnWidth ( KDV_TUTARI_COLUMN,      120 );
    m_ui->tableWidget->setColumnWidth ( TOPLAM_TUTAR_COLUMN,    150 );

    SET_SETTING_NAME                  ( "ISL_FIS_ARAMA" );

    m_ui->tableWidget->hideColumn ( FIS_ID_COLUMN );
}

/**************************************************************************************
                   ISL_FIS_ARAMA::CHECK_VAR
***************************************************************************************/

int ISL_FIS_ARAMA::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->date_edit_first_date ) {
        if (   m_ui->date_edit_first_date->date() > m_ui->date_edit_second_date->date() ) {
              MSG_WARNING(  tr ( "İlk tarih son tarihten büyük olamaz!.." ), m_ui->date_edit_first_date );

            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->date_edit_second_date ) {
        if (   m_ui->date_edit_first_date->date() > m_ui->date_edit_second_date->date() ) {
              MSG_WARNING( tr ( "Son tarih ilk tarihten küçük olamaz!.." ), m_ui->date_edit_second_date );

            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->comboBox_fis_turu ) {
        m_fis_turu = m_ui->comboBox_fis_turu->currentIndex();
    }
    else if ( object EQ m_ui->check_box_tarih_araligi ) {

        if ( m_ui->check_box_tarih_araligi->isChecked() EQ true ) {
            m_ui->frame_tarih_araligi->setEnabled ( true );
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled ( false );
            m_ui->date_edit_first_date->setDate   ( MALI_YIL_FIRST_DATE() );
            m_ui->date_edit_second_date->setDate  ( MALI_YIL_FIRST_DATE() );
        }
    }
    else if ( object EQ m_ui->check_box_aciklama_icerigi ) {

        if ( m_ui->check_box_aciklama_icerigi->isChecked() EQ true ) {
             m_ui->frame_aciklama_icerigi->setEnabled ( true );
        }
        else {
            m_ui->frame_aciklama_icerigi->setEnabled ( false );
            m_ui->lineEdit_aciklama_icerigi->clear();
        }

    }
    else if ( object EQ m_ui->check_box_fis_turu ) {

        if ( m_ui->check_box_fis_turu->isChecked() EQ true ) {
             m_ui->frame_fis_turu->setEnabled ( true );
             m_fis_turu = m_ui->comboBox_fis_turu->currentIndex();
        }
        else {
            m_fis_turu  = -1;
            m_ui->frame_fis_turu->setEnabled ( false );
        }

    }
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_FIS_ARAMA::CHECK_RUN
***************************************************************************************/

int ISL_FIS_ARAMA::CHECK_RUN()
{
    if ( m_ui->check_box_aciklama_icerigi->isChecked() EQ false AND m_ui->check_box_tarih_araligi->isChecked() EQ false AND
         m_ui->check_box_fis_turu->isChecked() EQ false ) {
        MSG_WARNING( tr ( "En az bir kriter seçimi yapmalısınız." ), m_ui->check_box_tarih_araligi );
        return ADAK_FAIL;
    }
    if ( m_ui->check_box_aciklama_icerigi->isChecked() EQ true ) {
        if ( m_ui->lineEdit_aciklama_icerigi->text().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Açıklama içeriği seçili iken boş bırakılamaz" ), m_ui->lineEdit_aciklama_icerigi );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_FIS_ARAMA::SEARCH
***************************************************************************************/

void ISL_FIS_ARAMA::SEARCH()
{
    QString first_date       = m_ui->date_edit_first_date->date().toString ( "yyyy.MM.dd" );
    QString last_date        = m_ui->date_edit_second_date->date().toString ( "yyyy.MM.dd" );
    QString aciklama_icerigi = m_ui->lineEdit_aciklama_icerigi->text();

    QString where_str;


    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT("isl_fisi","fis_id, fis_no, fis_turu, fis_tarihi ,aciklama, kdv_haric_toplam, kdv_toplam, "
                             "kdv_dahil_toplam ",where_str);


    if ( m_ui->check_box_tarih_araligi->isChecked() EQ true ) {       
        sql_query.AND_EKLE  ( "fis_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi" );
        sql_query.SET_VALUE ( ":baslangic_tarihi"   , first_date);
        sql_query.SET_VALUE ( ":bitis_tarihi"       , last_date);
    }


    if ( m_ui->check_box_aciklama_icerigi->isChecked() EQ true ) {
         sql_query.AND_EKLE( "aciklama LIKE :aciklama " );
         sql_query.SET_LIKE( ":aciklama",aciklama_icerigi);
    }

    if ( m_ui->check_box_fis_turu->isChecked() EQ true ) {
        sql_query.AND_EKLE  ( "fis_turu =:fis_turu" );
        sql_query.SET_VALUE ( ":fis_turu",QVariant(m_fis_turu).toInt());
    }

    if ( sql_query.SELECT("fis_tarihi ASC ,fis_no ASC") EQ 0 ) {
        return;
    }
    int current_row;

    while ( sql_query.NEXT() EQ true ) {

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item ( current_row, KDV_HARIC_TUTAR_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item ( current_row, KDV_TUTARI_COLUMN      )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item ( current_row, TOPLAM_TUTAR_COLUMN    )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );

        m_ui->tableWidget->item ( current_row, FIS_ID_COLUMN           )->setText ( sql_query.VALUE(0).toString()                           );
        m_ui->tableWidget->item ( current_row, FIS_NO_COLUMN           )->setText ( sql_query.VALUE(1).toString()                           );
        m_ui->tableWidget->item ( current_row, FIS_TARIHI_COLUMN       )->setText ( sql_query.VALUE(3).toDate().toString ( "dd MMMM yyyy dddd") );
        m_ui->tableWidget->item ( current_row, FIS_TURU_COLUMN         )->setText ( ISL_GET_FIS_TURU_STRING(sql_query.VALUE(2).toInt())       );
        m_ui->tableWidget->item ( current_row, ACIKLAMA_COLUMN         )->setText ( sql_query.VALUE(4).toString()                           );
        m_ui->tableWidget->item ( current_row, KDV_HARIC_TUTAR_COLUMN  )->setText ( VIRGUL_EKLE ( sql_query.VALUE(5).toString())            );
        m_ui->tableWidget->item ( current_row, KDV_TUTARI_COLUMN       )->setText ( VIRGUL_EKLE ( sql_query.VALUE(6).toString())            );
        m_ui->tableWidget->item ( current_row, TOPLAM_TUTAR_COLUMN     )->setText ( VIRGUL_EKLE ( sql_query.VALUE(7).toString())            );
    }
}

/**************************************************************************************
                   ISL_FIS_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int ISL_FIS_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number,FIS_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}

