#include <adak_utils.h>
#include <adak_sql.h>
#include "fat_irsaliye_arama_class.h"
#include "fat_irsaliye_arama_open.h"
#include "ui_fat_irsaliye_arama.h"
#include "fat_enum.h"
#include "stok_fis_utils.h"
#include "stok_enum.h"
#include "stok_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_FAT_IRSALIYE_ARAMA
***************************************************************************************/

void OPEN_FAT_IRSALIYE_ARAMA ( QString p_fatura_tarihi, int p_cari_hesap_id, int p_fatura_alis_satis_turu, QList< int > *p_secilen_irs_list, QWidget * p_parent)
{
    FAT_IRSALIYE_ARAMA *  F  = new FAT_IRSALIYE_ARAMA( p_fatura_tarihi, p_cari_hesap_id, p_fatura_alis_satis_turu, p_secilen_irs_list, p_parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   FAT_IRSALIYE_ARAMA::FAT_IRSALIYE_ARAMA
***************************************************************************************/

FAT_IRSALIYE_ARAMA::FAT_IRSALIYE_ARAMA ( QString p_fatura_tarihi, int p_cari_hesap_id, int p_fatura_alis_satis_turu,
                                         QList< int > *p_secilen_irs_list, QWidget * p_parent  ) : ARAMA_KERNEL ( p_parent ), m_ui ( new Ui::FAT_IRSALIYE_ARAMA )
{

    m_secilen_irs_list          = p_secilen_irs_list;
    m_irsaliye_tarihi_bitis     = p_fatura_tarihi;
    m_cari_hesap_id             = p_cari_hesap_id;
    m_irsaliye_alis_satis_turu  = p_fatura_alis_satis_turu;

    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, DB );

}

/**************************************************************************************
                   FAT_IRSALIYE_ARAMA::SETUP_FORM ()
***************************************************************************************/

void FAT_IRSALIYE_ARAMA::SETUP_FORM ()
{
    SET_PAGE_TITLE( tr ( "FAT - İRSALİYE SEÇİMİ" ) );

    REGISTER_TABLE_WIDGET ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS( QStringList() << tr("irsaliye_id") <<tr("İrs. Seri No") << tr("İrsaliye No") << tr("İrs. Tarihi") );

    SET_SORTING ( false );

    m_ui->tableWidget->hideColumn(0);
    m_ui->tableWidget->setColumnWidth ( 1, 20 );
    m_ui->tableWidget->setColumnWidth ( 2, 10 );
    m_ui->tableWidget->setColumnWidth ( 3, 200 );

    // Programa ilk girildiginde focuslanacak widget setlenmelidir.
    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_bas_tarihi );

    SET_HELP_PAGE ("fat_irsaliye_secimi.html");

    m_ui->tableWidget->setColumnWidth( 3   , 10);

    SET_SETTING_NAME ( "FAT_IRSALIYE_SECIMI" );

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    SET_FIND_BUTTON_NAME ( "İrsaliye Bul" );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "e9_sabit_degerler","irs_faturalandirilacak_gun" ,"sabit_deger_id = :sabit_deger_id");
    sql_query.SET_VALUE(":sabit_deger_id" , 1);

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    int   faturandirilacak_gun            = sql_query.VALUE(0).toInt();

    QDate irsaliye_tarihi_bitis           = QDate::fromString ( m_irsaliye_tarihi_bitis, "yyyy.MM.dd" );
    int   irsaliye_tarihi_bitis_month     = irsaliye_tarihi_bitis.month();

    QDate irsaliye_tarihi_baslangic       = irsaliye_tarihi_bitis.addDays ( -1 * faturandirilacak_gun );
    int   irsaliye_tarihi_baslangic_month = irsaliye_tarihi_baslangic.month();

    if ( irsaliye_tarihi_baslangic_month NE irsaliye_tarihi_bitis_month ) {
        m_irsaliye_tarihi_baslangic = QDate( irsaliye_tarihi_bitis.year(), irsaliye_tarihi_bitis_month, 01 ).toString("yyyy.MM.dd");
    }
    else {
        m_irsaliye_tarihi_baslangic = irsaliye_tarihi_baslangic.toString("yyyy.MM.dd");
    }
   
    m_ui->adakDate_bas_tarihi->SET_DATE( m_irsaliye_tarihi_baslangic );
    m_ui->adakDate_bts_tarihi->SET_DATE( m_irsaliye_tarihi_bitis );

    AUTO_START();
}

/**************************************************************************************
                   FAT_IRSALIYE_ARAMA::CHECK_VAR
***************************************************************************************/

int FAT_IRSALIYE_ARAMA::CHECK_VAR ( QObject * object )
{
    if( object EQ m_ui->adakDate_bas_tarihi ) {
        m_irsaliye_tarihi_baslangic = m_ui->adakDate_bas_tarihi->DATE();
    }
    else if ( object EQ m_ui->adakDate_bts_tarihi ) {
        m_irsaliye_tarihi_bitis = m_ui->adakDate_bts_tarihi->DATE();
    }
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_IRSALIYE_ARAMA::CHECK_RUN
***************************************************************************************/

int FAT_IRSALIYE_ARAMA::CHECK_RUN ()
{
    if( m_ui->adakDate_bas_tarihi->DATE() > m_ui->adakDate_bts_tarihi->DATE() ) {
        MSG_WARNING( "İrsaliye başlangıç tarihi bitiş tarihinden büyük olamaz.!", m_ui->adakDate_bas_tarihi );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_IRSALIYE_ARAMA::SEARCH
***************************************************************************************/

void FAT_IRSALIYE_ARAMA::SEARCH ()
{
    QList<int> IRS_FIS_ID_LIST = STK_GET_IRSALIYE_FIS_ID_LIST(m_cari_hesap_id , m_irsaliye_alis_satis_turu,
                                        0, 0, m_irsaliye_tarihi_baslangic, m_irsaliye_tarihi_bitis );

    if (IRS_FIS_ID_LIST.isEmpty() EQ true ) {
        return;
    }
    STK_IRS_FIS_STRUCT * IRS_FISI = new STK_IRS_FIS_STRUCT;

    for ( int i = 0 ; i < IRS_FIS_ID_LIST.size() ; i++ ) {

        int irsaliye_id = IRS_FIS_ID_LIST.at(i);

        STK_IRS_CLEAR_FIS_STRUCT(IRS_FISI);

        STK_IRS_FIS_BILGILERINI_OKU(irsaliye_id , IRS_FISI);

        //! FASON FISLERI OTOMATIK FATURALANDIRILAMAZ
        switch( IRS_FISI->fis_turu ) {
            case ENUM_IRS_FASON_BASLANGIC_GIRIS:
            case ENUM_IRS_FASON_BASLANGIC_CIKIS:
            case ENUM_IRS_FASON_BITIS_GIRIS    :
            case ENUM_IRS_FASON_BITIS_CIKIS    :
                continue;
                break;
            default:
                break;
        }

        int current_row = ADD_NEW_ROW ();

        m_ui->tableWidget->item ( current_row, 0 )->setText ( QVariant(irsaliye_id).toString() );
        m_ui->tableWidget->item ( current_row, 1 )->setText ( IRS_FISI->belge_seri + " " + IRS_FISI->belge_numarasi );
        m_ui->tableWidget->item ( current_row, 2 )->setText ( QVariant(IRS_FISI->fis_no).toString() );
        m_ui->tableWidget->item ( current_row, 3 )->setText ( QDate::fromString(IRS_FISI->fis_tarihi, "yyyy.MM.dd").toString("dd MMMM yyyy ") );
    }
}

/**************************************************************************************
                        FAT_IRSALIYE_ARAMA::SELECTED_LINE
***************************************************************************************/

void FAT_IRSALIYE_ARAMA::SELECTED_LINE ( int selected_row_number )
{
    Q_UNUSED( selected_row_number );
}

/**************************************************************************************
                   FAT_IRSALIYE_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int FAT_IRSALIYE_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    m_secilen_irs_list->clear();
    m_secilen_irs_list->append( m_ui->tableWidget->item ( selected_row_number, 0 )->text().toInt() );
    return ADAK_EXIT;
}

/**************************************************************************************
                   FAT_IRSALIYE_ARAMA::MULTI_LINE_SELECTED
***************************************************************************************/

int FAT_IRSALIYE_ARAMA::MULTI_LINE_SELECTED(QList<int> selected_rows)
{
    m_secilen_irs_list->clear();

    for (int i = 0; i < selected_rows.size(); ++i) {
        m_secilen_irs_list->append( m_ui->tableWidget->item ( selected_rows.at(i), 0 )->text().toInt() );
    }

    return ADAK_EXIT;
}
