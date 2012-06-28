#include <QDate>
#include <adak_utils.h>
#include <adak_sql.h>
#include "prk_fis_arama_class.h"
#include "prk_fis_arama_open.h"
#include "ui_prk_fis_arama.h"
#include "e9_enum.h"
#include "adak_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"


#define      FIS_ID_COLUMN                0
#define      FIS_TARIHI_COLUMN            1
#define      FIS_NO_COLUMN                2
#define      ACIKLAMA_COLUMN              3
#define      KDV_HARIC_TOPLAM_COLUMN      4
#define      KDV_TOPLAM_COLUMN            5
#define      KDV_DAHIL_TOPLAM_COLUMN      6

extern ADAK_SQL * DB;


/**************************************************************************************
                   OPEN_PRK_FIS_ARAMA
***************************************************************************************/

int OPEN_PRK_FIS_ARAMA ( int p_gider_fisi_mi ,QWidget * parent )
{
    int    secilen_id    = -1;

    PRK_FIS_ARAMA *  F  = new PRK_FIS_ARAMA( p_gider_fisi_mi , parent);

    F->m_secilen_id   = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );

    return secilen_id;
}

/**************************************************************************************
                   PRK_FIS_ARAMA::PRK_FIS_ARAMA
***************************************************************************************/

PRK_FIS_ARAMA::PRK_FIS_ARAMA ( int p_gider_fisi_mi , QWidget * parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::PRK_FIS_ARAMA )
{
    m_ui->setupUi ( this );

    m_gider_fisi_mi = p_gider_fisi_mi;

    START_ARAMA_KERNEL(this, DB );
}

/**************************************************************************************
                   PRK_FIS_ARAMA::SETUP_FORM
***************************************************************************************/

void PRK_FIS_ARAMA::SETUP_FORM()
{
    REGISTER_TABLE_WIDGET ( m_ui->tableWidget );

    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS ( QStringList() << tr ("Fis id") << tr("Fiş Tarihi") << tr("Fiş No") << tr("Açıklama")
                  << tr("Kdv \n Hariç Toplam") << tr("Kdv Toplam")<<tr("Kdv \n Dahil Toplam"));

    SET_SORTING ( false );

    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_bas_tarihi );

    SET_PAGE_TITLE    ( tr ( "PRK - PERAKENDE FİŞİ ARAMA" ) );

    SET_HELP_PAGE     ("perakende_fisi_arama.html");

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    SET_FIND_BUTTON_NAME ( "Bul" );

    m_ui->checkBox_tarih_araligi->setChecked(true);
    m_ui->frame_aciklama_icerigi->setDisabled(true);

    m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE());
    m_ui->adakDate_bts_tarihi->SET_DATE( MALI_YIL_365_DATE());

    m_ui->tableWidget->setColumnWidth(FIS_ID_COLUMN            , 10 );
    m_ui->tableWidget->setColumnWidth(FIS_TARIHI_COLUMN        , 150);
    m_ui->tableWidget->setColumnWidth(FIS_NO_COLUMN            , 80 );
    m_ui->tableWidget->setColumnWidth(ACIKLAMA_COLUMN          , 200);
    m_ui->tableWidget->setColumnWidth(KDV_HARIC_TOPLAM_COLUMN  , 180);
    m_ui->tableWidget->setColumnWidth(KDV_TOPLAM_COLUMN        , 100);
    m_ui->tableWidget->setColumnWidth(KDV_DAHIL_TOPLAM_COLUMN  , 180);

    SET_SETTING_NAME  ("PERAKENDE_FIS_ARAMA");


    m_ui->tableWidget->hideColumn(FIS_ID_COLUMN);


    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->frame_fis_filtresi->setHidden(false);
    }
}

/**************************************************************************************
                   PRK_FIS_ARAMA::CHECK_VAR
***************************************************************************************/

int PRK_FIS_ARAMA::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->checkBox_tarih_araligi ) {
        if ( m_ui->checkBox_tarih_araligi->isChecked() EQ true ) {
            m_ui->date_frame->setEnabled(true);
        }
        else {
            m_ui->date_frame->setDisabled(true);
            m_ui->adakDate_bas_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
            m_ui->adakDate_bts_tarihi->SET_DATE(MALI_YIL_365_DATE());
        }
    }
    else if ( object EQ m_ui->checkBox_aciklama_icerigi ) {
        if ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ true ) {
            m_ui->frame_aciklama_icerigi->setEnabled(true);

        }
        else {
            m_ui->frame_aciklama_icerigi->setDisabled(true);
            m_ui->lineEdit_aciklama_icerigi->clear();
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_FIS_ARAMA::CHECK_RUN
***************************************************************************************/

int PRK_FIS_ARAMA::CHECK_RUN ()
{
    if ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ true ) {
        if ( m_ui->lineEdit_aciklama_icerigi->text().isEmpty() EQ true ) {
            MSG_WARNING( tr ("Açıklama içeriğini girmelisiniz."), m_ui->lineEdit_aciklama_icerigi );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_FIS_ARAMA::SEARCH
***************************************************************************************/

void PRK_FIS_ARAMA::SEARCH ()
{
    QString where_str = QString (" fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi AND gider_fisi_mi = :gider_fisi_mi");

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler" , "fis_id ,fis_tarihi , fis_no , aciklama , kdv_haric_toplam , kdv_toplam,"
                         "kdv_dahil_toplam" , where_str);

    query.SET_VALUE( ":bas_tarihi"      , m_ui->adakDate_bas_tarihi->DATE()   );
    query.SET_VALUE( ":bts_tarihi"      , m_ui->adakDate_bts_tarihi->DATE() );
    query.SET_VALUE( ":gider_fisi_mi"   , m_gider_fisi_mi );

    if ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ true ) {

        query.AND_EKLE( "aciklama LIKE :aciklama" );
        query.SET_LIKE( ":aciklama", m_ui->lineEdit_aciklama_icerigi->text());
    }

    if ( query.SELECT("fis_tarihi ASC,fis_no ASC") NE 0 ) {
        int current_row = 0;
        while ( query.NEXT() ) {
            current_row = ADD_NEW_ROW();

            m_ui->tableWidget->item(current_row , KDV_HARIC_TOPLAM_COLUMN)->setTextAlignment    ( Qt::AlignRight );
            m_ui->tableWidget->item(current_row , KDV_TOPLAM_COLUMN)->setTextAlignment          ( Qt::AlignRight );
            m_ui->tableWidget->item(current_row , KDV_DAHIL_TOPLAM_COLUMN)->setTextAlignment    ( Qt::AlignRight );
            m_ui->tableWidget->item(current_row , FIS_ID_COLUMN)->setText                       ( query.VALUE(0).toString() );
            m_ui->tableWidget->item(current_row , FIS_TARIHI_COLUMN)->setText                   ( query.VALUE(1).toDate().toString("dd MMMM yyyy"));
            m_ui->tableWidget->item(current_row , FIS_NO_COLUMN)->setText                       ( query.VALUE(2).toString() );
            m_ui->tableWidget->item(current_row , ACIKLAMA_COLUMN)->setText                     ( query.VALUE(3).toString() );
            m_ui->tableWidget->item(current_row , KDV_HARIC_TOPLAM_COLUMN)->setText( VIRGUL_EKLE( query.VALUE(4).toString() ));
            m_ui->tableWidget->item(current_row , KDV_TOPLAM_COLUMN)->setText( VIRGUL_EKLE      ( query.VALUE(5).toString() ));
            m_ui->tableWidget->item(current_row , KDV_DAHIL_TOPLAM_COLUMN)->setText( VIRGUL_EKLE( query.VALUE(6).toString() ));
        }
    }
}

/**************************************************************************************
                        PRK_FIS_ARAMA::SELECTED_LINE
***************************************************************************************/

void PRK_FIS_ARAMA::SELECTED_LINE ( int selected_row_number )
{
    Q_UNUSED(selected_row_number);
}

/**************************************************************************************
                   PRK_FIS_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int PRK_FIS_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{

    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number, FIS_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}
