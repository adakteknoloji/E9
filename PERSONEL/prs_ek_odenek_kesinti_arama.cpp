#include "adak_defines.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "prs_ek_odenek_kesinti_arama_class.h"
#include "prs_ek_odenek_kesinti_arama_open.h"
#include "e9_console_utils.h"

#define KES_ODE_TANIM_ID_COLUMN              0
#define KESINTI_ODENEK_TARIHI_COLUMN         1
#define KESINTI_ODENEK_ADI_COLUMN            2
#define ACIKLAMA_COLUMN                      3
#define YUZDE_MI_COLUMN                      4
#define TUTAR_COLUMN                         5

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_PRS_EK_ODENEK_KESINTI_ARAMA
***************************************************************************************/

int OPEN_PRS_EK_ODENEK_KESINTI_ARAMA ( int p_kesinti_odenek_turu, QWidget * parent )
{
    int secilen_id = -1;
    PRS_EK_ODENEK_KESINTI_ARAMA * F = new PRS_EK_ODENEK_KESINTI_ARAMA ( p_kesinti_odenek_turu, parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   PRS_EK_ODENEK_KESINTI_ARAMA::PRS_EK_ODENEK_KESINTI_ARAMA
***************************************************************************************/

PRS_EK_ODENEK_KESINTI_ARAMA::PRS_EK_ODENEK_KESINTI_ARAMA (int p_kesinti_odenek_turu, QWidget *parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::PRS_EK_ODENEK_KESINTI_ARAMA )
{
    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, DB );

    m_kesinti_odenek_turu   = p_kesinti_odenek_turu;
}

/**************************************************************************************
                   PRS_EK_ODENEK_KESINTI_ARAMA::CHECK_VAR
***************************************************************************************/

void PRS_EK_ODENEK_KESINTI_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE  ( tr("personel_ek_kesinti_odenek_arama") );
    SET_PAGE_TITLE ( tr ( "PRS - ÖDENEK / KESİNTİ ARAMA" ) );

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS ( QStringList() << tr ( "Fiş Id" )     << tr ( "Tarih" ) << tr ( "Ödenek / Kesinti" )
                                << tr ( "Açıklama" )   << tr ( "Yuzde / Tutar" )
                                << tr ( "Tutar" )  );

    SET_ENTER_KEY_FOR_RUN_BATCH                 ( true );

    SET_FIND_BUTTON_NAME                        ( tr ( "Bul" ) );
    m_ui->tableWidget->hideColumn               ( 0 );

    m_ui->check_box_tarih_araligi->setChecked   ( true );
    m_ui->frame_aciklama_icerigi->setDisabled   ( true );

    m_ui->adakDate_first_date->SET_DATE  ( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_second_date->SET_DATE ( MALI_ARRAY_LAST_DATE());

    m_ui->tableWidget->setColumnWidth ( KESINTI_ODENEK_TARIHI_COLUMN,   150 );
    m_ui->tableWidget->setColumnWidth ( KESINTI_ODENEK_ADI_COLUMN,      60  );
    m_ui->tableWidget->setColumnWidth ( ACIKLAMA_COLUMN,                250 );
    m_ui->tableWidget->setColumnWidth ( YUZDE_MI_COLUMN,                120 );
    m_ui->tableWidget->setColumnWidth ( TUTAR_COLUMN,                   120 );

    SET_SETTING_NAME( "PRS_EK_ODENEK_KESINTI_ARAMA" );

    m_ui->tableWidget->hideColumn ( KES_ODE_TANIM_ID_COLUMN );
}

/**************************************************************************************
                   PRS_EK_ODENEK_KESINTI_ARAMA::CHECK_VAR
***************************************************************************************/

int PRS_EK_ODENEK_KESINTI_ARAMA::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->adakDate_first_date ) {
        if (   m_ui->adakDate_first_date->QDATE() > m_ui->adakDate_second_date->QDATE() ) {
              MSG_WARNING(  tr ( "İlk tarih son tarihten büyük olamaz!.." ), m_ui->adakDate_first_date );

            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->adakDate_second_date ) {
        if (   m_ui->adakDate_first_date->QDATE() > m_ui->adakDate_second_date->QDATE() ) {
              MSG_WARNING( tr ( "Son tarih ilk tarihten küçük olamaz!.." ), m_ui->adakDate_second_date );

            return ADAK_FAIL;
        }
    }

    else if ( object EQ m_ui->check_box_tarih_araligi ) {

        if ( m_ui->check_box_tarih_araligi->isChecked() EQ true ) {
            m_ui->frame_tarih_araligi->setEnabled ( true );
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled ( false );

            m_ui->adakDate_first_date->SET_DATE  ( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_second_date->SET_DATE ( MALI_ARRAY_LAST_DATE());
        }
    }
    else if ( object EQ m_ui->check_box_kesinti_odenek ) {

        if ( m_ui->check_box_kesinti_odenek->isChecked() EQ true ) {
             m_ui->frame_aciklama_icerigi->setEnabled ( true );
        }
        else {
            m_ui->frame_aciklama_icerigi->setEnabled ( false );
            m_ui->lineEdit_kesinti_odenek_adi->clear();
        }

    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_KESINTI_ARAMA::CHECK_RUN
***************************************************************************************/

int PRS_EK_ODENEK_KESINTI_ARAMA::CHECK_RUN()
{
    if ( m_ui->check_box_kesinti_odenek->isChecked() EQ false AND m_ui->check_box_tarih_araligi->isChecked() EQ false ) {
        MSG_WARNING( tr ( "En az bir kriter seçimi yapmalısınız." ), m_ui->check_box_tarih_araligi );
        return ADAK_FAIL;
    }
    if ( m_ui->check_box_kesinti_odenek->isChecked() EQ true ) {
        if ( m_ui->lineEdit_kesinti_odenek_adi->text().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Açıklama içeriği seçili iken boş bırakılamaz" ), m_ui->lineEdit_kesinti_odenek_adi );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EK_ODENEK_KESINTI_ARAMA::SEARCH
***************************************************************************************/

void PRS_EK_ODENEK_KESINTI_ARAMA::SEARCH()
{
    QString first_date       = m_ui->adakDate_first_date->QDATE().toString ( "yyyy.MM.dd" );
    QString last_date        = m_ui->adakDate_second_date->QDATE().toString ( "yyyy.MM.dd" );
    QString kesinti_odenek_adi = m_ui->lineEdit_kesinti_odenek_adi->text();


    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT("prs_kesinti_odenek_tanimlar",
                             "kes_ode_tanim_id, kesinti_odenek_tarihi, kesinti_odenek_adi ,aciklama, yuzde_mi, tutar " );


    if ( m_ui->check_box_tarih_araligi->isChecked() EQ true ) {       
        sql_query.AND_EKLE  ( "kesinti_odenek_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi" );
        sql_query.SET_VALUE ( ":baslangic_tarihi"   , first_date);
        sql_query.SET_VALUE ( ":bitis_tarihi"       , last_date);
    }

    if ( m_ui->check_box_kesinti_odenek->isChecked() EQ true ) {
         sql_query.AND_EKLE( "kesinti_odenek_adi LIKE :kesinti_odenek_adi " );
         sql_query.SET_LIKE( ":kesinti_odenek_adi", kesinti_odenek_adi);
    }

    if ( sql_query.SELECT("kesinti_odenek_tarihi ASC ,kesinti_odenek_no ASC") EQ 0 ) {
        return;
    }
    int current_row;

    while ( sql_query.NEXT() EQ true ) {

        current_row = ADD_NEW_ROW();

//        m_ui->tableWidget->item ( current_row, YUZDE_MI_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
//        m_ui->tableWidget->item ( current_row, TUTAR_COLUMN    )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );

        m_ui->tableWidget->item ( current_row, KES_ODE_TANIM_ID_COLUMN      )->setText ( sql_query.VALUE(0).toString()                           );
        m_ui->tableWidget->item ( current_row, KESINTI_ODENEK_ADI_COLUMN    )->setText ( sql_query.VALUE(2).toString()                           );
        m_ui->tableWidget->item ( current_row, KESINTI_ODENEK_TARIHI_COLUMN )->setText ( sql_query.VALUE(1).toDate().toString ( "dd MMMM yyyy dddd") );
        m_ui->tableWidget->item ( current_row, ACIKLAMA_COLUMN              )->setText ( sql_query.VALUE(3).toString()                           );
        m_ui->tableWidget->item ( current_row, YUZDE_MI_COLUMN              )->setText ( VIRGUL_EKLE ( sql_query.VALUE(4).toString())            );
        m_ui->tableWidget->item ( current_row, TUTAR_COLUMN                 )->setText ( VIRGUL_EKLE ( sql_query.VALUE(5).toString())            );
    }
}

/**************************************************************************************
                   PRS_EK_ODENEK_KESINTI_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int PRS_EK_ODENEK_KESINTI_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number, KES_ODE_TANIM_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}

