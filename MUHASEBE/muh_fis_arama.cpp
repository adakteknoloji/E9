#include <adak_utils.h>
#include <adak_sql.h>
#include "muh_fis_arama_class.h"
#include "muh_fis_arama_open.h"
#include "ui_muh_fis_arama.h"
#include "muh_console_utils.h"
#include "e9_enum.h"
#include "adak_std_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "muh_enum.h"
#include "muh_muhasebe_fisi_open.h"
#include "muh_gui_utils.h"


#define     FIS_ID_COLUMN       0
#define     YEVMIYE_NO_COLUMN   1
#define     FIS_TARIHI_COLUMN   2
#define     FIS_NO_COLUMN       3
#define     ACIKLAMA_COLUMN     4
#define     BORC_COLUMN         5
#define     ALACAK_COLUMN       6
#define     FIS_TURU_COLUMN     7

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_MUH_FIS_ARAMA
***************************************************************************************/

int OPEN_MUH_FIS_ARAMA ( int p_yvm_no_goster, int p_fis_kaynagi, int p_fis_turu, MUH_FIS_TURU p_muh_fis_turu, QWidget * p_parent )
{
    int secilen_id    = -1;
    MUH_FIS_ARAMA * F = new MUH_FIS_ARAMA ( p_yvm_no_goster, p_fis_kaynagi, p_fis_turu, p_muh_fis_turu, p_parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   MUH_FIS_ARAMA::MUH_FIS_ARAMA
***************************************************************************************/

MUH_FIS_ARAMA::MUH_FIS_ARAMA ( int p_yvm_no_goster, int p_fis_kaynagi, int p_fis_turu, MUH_FIS_TURU p_muh_fis_turu, QWidget * p_parent ) : ARAMA_KERNEL ( p_parent ), m_ui ( new Ui::MUH_FIS_ARAMA )
{
    m_yvm_no_goster = p_yvm_no_goster;
    m_modul_id      = p_fis_kaynagi;
    m_fis_turu      = p_fis_turu;
    m_muh_fis_turu  = p_muh_fis_turu;


    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, DB );
}

/**************************************************************************************
                   MUH_FIS_ARAMA::SETUP_FORM
***************************************************************************************/

void MUH_FIS_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE ( tr("muhasebe-fisleri") );
    if ( m_muh_fis_turu EQ ENUM_MUHASEBE_ENT_FISI ) {
        SET_PAGE_TITLE ( tr ( "MUH - MUHASEBE ENTEGRASYON FİŞLERİ" ) );
        SET_HELP_PAGE( "genel-muhasebe_entegrasyon");
        m_ui->checkBox_fis_aciklama->setVisible( false );
        m_ui->fis_aciklama_frame->setVisible( false );

        m_ui->checkBox_ent_fis_turu->setChecked( true );

        MUH_FILL_ENT_FISLERI_COMBO_BOX( m_ui->comboBox_ent_fis_turu );
    }
    else {
        SET_PAGE_TITLE ( tr ( "MUH - MUHASEBE FİŞİ ARAMA" ) );

        m_ui->checkBox_ent_fis_turu->setChecked( true );
        MUH_FILL_FIS_TURLERI_COMBO_BOX( m_ui->comboBox_ent_fis_turu );
        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_ent_fis_turu, MUH_GET_FIS_TURLERI_STRING(m_fis_turu));
    }

    if( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ 1 ){
        m_ui->frame_filtre->setVisible( true );
    }

    SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_first_date );

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS ( QStringList()<<tr ( "Fiş Id" )  <<tr ( "Yvm No" )     <<tr ( "Fiş Tarihi" )   <<tr ( "Fiş No" )
                               <<tr ( "Açıklama" )<<tr ( "Toplam Borç" )<<tr ( "Toplam Alacak" )<<tr ( "Fiş Türü" ) );

    SET_SORTING                           ( false );

    m_ui->adakDate_first_date->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_second_date->SET_DATE( MALI_YIL_365_DATE() );

    m_ui->checkBox_date->setChecked       ( true  );
    m_ui->fis_aciklama_frame->setEnabled  ( false );

    if ( m_yvm_no_goster EQ 0 ) {
        m_ui->tableWidget->hideColumn ( YEVMIYE_NO_COLUMN );
    }

    m_ui->tableWidget->setColumnWidth ( FIS_TARIHI_COLUMN, 240 );
    m_ui->tableWidget->setColumnWidth ( FIS_NO_COLUMN,     60  );
    m_ui->tableWidget->setColumnWidth ( ACIKLAMA_COLUMN,   290 );
    m_ui->tableWidget->setColumnWidth ( BORC_COLUMN,       135 );
    m_ui->tableWidget->setColumnWidth ( ALACAK_COLUMN,     135 );
    m_ui->tableWidget->setColumnWidth ( FIS_TURU_COLUMN,   150 );


    m_ui->tableWidget->hideColumn           ( FIS_ID_COLUMN );

    SET_ENTER_KEY_FOR_RUN_BATCH             ( true );
    SET_FIND_BUTTON_NAME                    ( tr ( "Fişleri Bul" ) );
    SET_SETTING_NAME                        ( "MUH_FIS_ARAMA" );
}

/**************************************************************************************
                   MUH_FIS_ARAMA::CHECK_VAR
***************************************************************************************/

int MUH_FIS_ARAMA::CHECK_VAR ( QObject * object )
{
    QDate       rapor_baslangic_tarihi;
    QDate       rapor_bitis_tarihi;

    if ( object EQ m_ui->adakDate_first_date OR object EQ m_ui->adakDate_second_date ) {

        rapor_baslangic_tarihi    = m_ui->adakDate_first_date->QDATE();
        rapor_bitis_tarihi        = m_ui->adakDate_second_date->QDATE();

        if (   m_ui->adakDate_first_date->DATE() > m_ui->adakDate_second_date->DATE() ) {
            MSG_WARNING( tr ( "İlk tarih son tarihten büyük olamaz!.." ), m_ui->adakDate_first_date  );
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->checkBox_date ) {

        if ( m_ui->checkBox_date->isChecked() EQ true ) {
            m_ui->date_frame->setEnabled ( true );
        }
        else {
            m_ui->date_frame->setEnabled ( false );
            m_ui->adakDate_first_date->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_second_date->SET_DATE(MALI_YIL_365_DATE() );
        }
    }
    else if ( object EQ m_ui->checkBox_fis_aciklama ) {

        if ( m_ui->checkBox_fis_aciklama->isChecked() EQ true ) {
            m_ui->fis_aciklama_frame->setEnabled( true );
        }
        else {
            m_ui->fis_aciklama_frame->setEnabled( false );
            m_ui->lineEdit_fis_aciklama->clear();
        }
    }
    else if ( object EQ m_ui->comboBox_ent_fis_turu ) {
        if ( m_muh_fis_turu EQ ENUM_MUHASEBE_ENT_FISI ) {
            m_modul_id = MUH_GET_ENTEGRASYON_FIS_KAYNAK_MODUL_ENUM( m_ui->comboBox_ent_fis_turu->currentText() );
        }
        else {
            m_fis_turu = MUH_GET_FIS_TURLERI_ENUM( m_ui->comboBox_ent_fis_turu->currentText() );
        }
    }

    return true;
}

/**************************************************************************************
                   MUH_FIS_ARAMA::CHECK_RUN
***************************************************************************************/

int MUH_FIS_ARAMA::CHECK_RUN()
{
    if ( m_ui->checkBox_date->isChecked() EQ false
         AND m_ui->checkBox_fis_aciklama->isChecked() EQ false  ) {
        MSG_WARNING( tr ( "Arama yapabilmek için en az bir seçenek işaretlemelisiniz." ), m_ui->checkBox_date );
        return ADAK_FAIL;
    }

    if ( m_ui->checkBox_fis_aciklama->isChecked() EQ true ) {
        if ( m_ui->lineEdit_fis_aciklama->text().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Açıklama alanı boş bırakılamaz" ), m_ui->lineEdit_fis_aciklama );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_FIS_ARAMA::SEARCH
***************************************************************************************/

void MUH_FIS_ARAMA::SEARCH()
{

    QString first_date   = m_ui->adakDate_first_date->DATE();

    QString last_date    = m_ui->adakDate_second_date->DATE();

    QString where_str;

    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT ( "muh_fisler" , "fis_id, fis_no, fis_tarihi, aciklama, toplam_borc, "
                               "toplam_alacak, fis_turu, yevmiye_numarasi,modul_id" , where_str);

    // modul_id 0 ise diger programlardan gelmistir.
    if ( m_modul_id > 0 ) {
        sql_query.AND_EKLE  ( "modul_id = :modul_id " );
        sql_query.SET_VALUE ( ":modul_id" , m_modul_id );
    }
    // tum ent fisleri gosterilecek
    else {
        sql_query.AND_EKLE  ( "modul_id != :modul_id " );
        sql_query.SET_VALUE ( ":modul_id", MUHASEBE_MODULU  );
    }

    if ( m_ui->checkBox_date->isChecked() EQ true ) {
        sql_query.AND_EKLE  ( "fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi" );
        sql_query.SET_VALUE ( ":bas_tarihi" , first_date    );
        sql_query.SET_VALUE ( ":bts_tarihi" , last_date     );
    }

    if ( m_fis_turu > -1 ) {
        sql_query.AND_EKLE  ( "fis_turu = :fis_turu " );
        sql_query.SET_VALUE ( ":fis_turu" , m_fis_turu);;
    }

    if ( m_ui->checkBox_fis_aciklama->isChecked() EQ true ) {
        sql_query.AND_EKLE  ( "aciklama LIKE :aciklama " );
        sql_query.SET_LIKE  ( ":aciklama", m_ui->lineEdit_fis_aciklama->text() );
    }

    // modul_id 0 ise diger programlardan gelmistir.
    if ( m_modul_id EQ 0 ) {
        sql_query.AND_EKLE  ( "program_id != :program_id" );
        sql_query.SET_VALUE ( ":program_id" , E9_PROGRAMI );
    }

    if ( sql_query.SELECT("fis_tarihi ASC, fis_no ASC") EQ 0 ) {
        return;
    }
    int current_row;

    while ( sql_query.NEXT() EQ true ) {

        current_row          = ADD_NEW_ROW();
        QString fis_turu_str;

        int     fis_turu     = sql_query.VALUE(6).toInt();

        if ( m_modul_id EQ MUHASEBE_MODULU ) {
            fis_turu_str = MUH_GET_FIS_TURLERI_STRING ( fis_turu );
        }
        else {
            fis_turu_str = MUH_GET_ENTEGRASYON_FIS_KAYNAK_MODUL_STRING ( sql_query.VALUE(8).toInt() );
        }


        m_ui->tableWidget->item ( current_row, BORC_COLUMN   )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item ( current_row, ALACAK_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter);

        if ( m_yvm_no_goster EQ 1 ) {
            m_ui->tableWidget->item ( current_row, FIS_ID_COLUMN        )->setText ( sql_query.VALUE(0).toString()                          );
            m_ui->tableWidget->item ( current_row, YEVMIYE_NO_COLUMN    )->setText ( sql_query.VALUE(7).toString()                          );
            m_ui->tableWidget->item ( current_row, FIS_NO_COLUMN        )->setText ( sql_query.VALUE(1).toString()                          );
            m_ui->tableWidget->item ( current_row, FIS_TARIHI_COLUMN    )->setText ( sql_query.VALUE(2).toDate().toString ( "dd MMMM yyyy dddd" )   );
            m_ui->tableWidget->item ( current_row, ACIKLAMA_COLUMN      )->setText ( sql_query.VALUE(3).toString()                          );
            m_ui->tableWidget->item ( current_row, BORC_COLUMN          )->setText ( VIRGUL_EKLE ( sql_query.VALUE(4).toString() )          );
            m_ui->tableWidget->item ( current_row, ALACAK_COLUMN        )->setText ( VIRGUL_EKLE ( sql_query.VALUE(5).toString() )          );
            m_ui->tableWidget->item ( current_row, FIS_TURU_COLUMN      )->setText ( fis_turu_str                                           );
        }
        else {
            m_ui->tableWidget->item ( current_row, FIS_ID_COLUMN        )->setText ( sql_query.VALUE(0).toString()                          );
            m_ui->tableWidget->item ( current_row, FIS_NO_COLUMN        )->setText ( sql_query.VALUE(1).toString()                          );
            m_ui->tableWidget->item ( current_row, FIS_TARIHI_COLUMN    )->setText ( sql_query.VALUE(2).toDate().toString ( "dd MMMM yyyy dddd" )   );
            m_ui->tableWidget->item ( current_row, ACIKLAMA_COLUMN      )->setText ( sql_query.VALUE(3).toString()                          );
            m_ui->tableWidget->item ( current_row, BORC_COLUMN          )->setText ( VIRGUL_EKLE(sql_query.VALUE(4).toString())             );
            m_ui->tableWidget->item ( current_row, ALACAK_COLUMN        )->setText ( VIRGUL_EKLE(sql_query.VALUE(5).toString())             );
            m_ui->tableWidget->item ( current_row, FIS_TURU_COLUMN      )->setText ( fis_turu_str );
        }
    }
}

/**************************************************************************************
                   MUH_FIS_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int MUH_FIS_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number, FIS_ID_COLUMN )->text().toInt();

    if ( m_muh_fis_turu EQ ENUM_MUHASEBE_FISI ) {
        return ADAK_EXIT;
    }

    OPEN_MUHASEBE_FISI ( *m_secilen_id, m_fis_turu, m_modul_id, nativeParentWidget() );

    return ADAK_OK;
}
