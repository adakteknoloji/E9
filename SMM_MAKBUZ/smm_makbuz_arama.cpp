#include <adak_utils.h>
#include <adak_sql.h>
#include "smm_makbuz_arama_class.h"
#include "ui_smm_makbuz_arama.h"
#include "e9_console_utils.h"

extern ADAK_SQL * DB;


/**************************************************************************************
                   OPEN_SMM_MAKBUZ_ARAMA
***************************************************************************************/

int OPEN_SMM_MAKBUZ_ARAMA ( int p_makbuz_turu ,QWidget * parent )
{
    int    secilen_id    = -1;

    SMM_MAKBUZ_ARAMA *  F  = new SMM_MAKBUZ_ARAMA( p_makbuz_turu , parent);

    F->m_secilen_id   = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );

    return secilen_id;
}

/**************************************************************************************
                   SMM_MAKBUZ_ARAMA::SMM_MAKBUZ_ARAMA
***************************************************************************************/

SMM_MAKBUZ_ARAMA::SMM_MAKBUZ_ARAMA ( int p_makbuz_turu ,QWidget * parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::SMM_MAKBUZ_ARAMA )
{
    m_ui->setupUi      ( this );

    m_makbuz_turu = p_makbuz_turu;

    START_ARAMA_KERNEL ( this, DB );

}

/**************************************************************************************
                   SMM_MAKBUZ_ARAMA::CHECK_VAR
***************************************************************************************/

void SMM_MAKBUZ_ARAMA::SETUP_FORM ()
{

    REGISTER_TABLE_WIDGET ( m_ui->tableWidget );


    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );


    SET_HEADERS ( QStringList() << tr("makbuz id") << tr("Makbuz Tarihi") << tr("Makbuz No") << tr("Açıklama")
                  << tr("Hizmet Tutarı"));

    SET_SORTING ( false );

    m_ui->tableWidget->hideColumn(0);

    m_ui->tableWidget->setColumnWidth ( 1,        200 );
    m_ui->tableWidget->setColumnWidth ( 2,        110  );
    m_ui->tableWidget->setColumnWidth ( 3,        520 );
    m_ui->tableWidget->setColumnWidth ( 4,        120 );


    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_bas_tarihi );

    m_ui->checkbox_tarih_araligi->setChecked(true);
    m_ui->frame_aciklama->setDisabled(true);

    m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bitis_tarihi->SET_DATE( MALI_YIL_FIRST_DATE());

    SET_PAGE_TITLE    ( tr ( "SMM - SERBEST MESLEK MAKBUZU ARAMA" ) );

    SET_HELP_PAGE     ("smm_makbuz_arama.html");

    SET_SETTING_NAME  ("SMM_MAKBUZ_ARAMA");

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    SET_FIND_BUTTON_NAME ( "Bul" );

}

/**************************************************************************************
                   SMM_MAKBUZ_ARAMA::CHECK_VAR
***************************************************************************************/

int SMM_MAKBUZ_ARAMA::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->adakDate_bas_tarihi ) {
        if (   m_ui->adakDate_bas_tarihi->QDATE() > m_ui->adakDate_bitis_tarihi->QDATE() ) {
              MSG_WARNING(  tr ( "İlk tarih son tarihten büyük olamaz!.." ), m_ui->adakDate_bas_tarihi );

            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->adakDate_bitis_tarihi ) {
        if (   m_ui->adakDate_bas_tarihi->QDATE() > m_ui->adakDate_bitis_tarihi->QDATE() ) {
              MSG_WARNING( tr ( "Son tarih ilk tarihten küçük olamaz!.." ), m_ui->adakDate_bitis_tarihi );

            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->checkbox_aciklama ) {
        if ( m_ui->checkbox_aciklama->isChecked() EQ true ) {
            m_ui->frame_aciklama->setEnabled(true);
            SET_FOCUS(m_ui->lineEdit_aciklama);
        }
        else {
            m_ui->frame_aciklama->setDisabled(true);
            m_ui->lineEdit_aciklama->clear();
        }

    }
    else if ( object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ true ) {
            m_ui->frame_tarih->setEnabled(true);
            SET_FOCUS(m_ui->adakDate_bas_tarihi);
        }
        else {
            m_ui->frame_tarih->setDisabled(true);
            m_ui->adakDate_bas_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
            m_ui->adakDate_bitis_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_MAKBUZ_ARAMA::CHECK_RUN
***************************************************************************************/

int SMM_MAKBUZ_ARAMA::CHECK_RUN ()
{
    if ( m_ui->checkbox_aciklama->isChecked() EQ true ) {
        if ( m_ui->lineEdit_aciklama->text().isEmpty() EQ true ) {
            MSG_WARNING(tr("Açıklama içeriği girmelisiniz"),m_ui->lineEdit_aciklama);
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMM_MAKBUZ_ARAMA::SEARCH
***************************************************************************************/

void SMM_MAKBUZ_ARAMA::SEARCH ()
{

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar" , "makbuz_id,makbuz_no,makbuz_tarihi,aciklama,hizmet_tutari",
                         "makbuz_turu = :makbuz_turu AND makbuz_tarihi BETWEEN :bas_tarihi "
                         "AND :bitis_tarihi");
    query.SET_VALUE(":makbuz_turu"     , m_makbuz_turu);
    query.SET_VALUE(":bas_tarihi"      , m_ui->adakDate_bas_tarihi->DATE());
    query.SET_VALUE(":bitis_tarihi"    , m_ui->adakDate_bitis_tarihi->DATE());

    if ( m_ui->checkbox_aciklama->isChecked() EQ true ) {
        query.AND_EKLE("aciklama LIKE :aciklama");
        query.SET_LIKE(":aciklama" , m_ui->lineEdit_aciklama->text());
    }

    if ( query.SELECT("makbuz_tarihi ASC,makbuz_no ASC") EQ 0 ) {
        return;
    }

    int current_row = 0;

    while ( query.NEXT() ) {

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item(current_row , 0 )->setText(query.VALUE(0).toString());
        m_ui->tableWidget->item(current_row , 1 )->setText(QDate::fromString(query.VALUE(2).toString() , "yyyy.MM.dd").toString("dd MMMM yyyy"));
        m_ui->tableWidget->item(current_row , 2 )->setText(query.VALUE(1).toString());
        m_ui->tableWidget->item(current_row , 3 )->setText(query.VALUE(3).toString());
        m_ui->tableWidget->item(current_row , 4 )->setText(VIRGUL_EKLE(query.VALUE(4).toString()));
    }

}

/**************************************************************************************
                   SMM_MAKBUZ_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int SMM_MAKBUZ_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number, 0 )->text().toInt();
    return ADAK_EXIT;
}
