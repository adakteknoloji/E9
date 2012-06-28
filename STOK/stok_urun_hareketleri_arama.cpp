#include <adak_utils.h>
#include <adak_sql.h>
#include "stok_urun_hareketleri_arama_class.h"
#include "stok_urun_hareketleri_arama_open.h"
#include "ui_stok_urun_hareketleri_arama.h"
#include "fat_fis_utils.h"
#include "fat_enum.h"
#include "fat_console_utils.h"
#include "stok_fis_utils.h"
#include "stok_console_utils.h"
#include "stok_enum.h"
#include "e9_enum.h"
#include "fat_fatura_fisi_open.h"
#include "stok_fisi_open.h"

extern ADAK_SQL * DB;

#define MODUL_ID_COLUMN         0
#define FIS_TURU_COLUMN         1
#define IADE_FISI_MI_COLUMN     2
#define IRS_MI_STOK_MU_COLUMN   3
#define FIS_ID_COLUMN           4
#define FIS_TARIHI_COLUMN       5
#define FIS_NO_COLUMN           6
#define ACIKLAMA_COLUMN         7
#define FIS_TUTARI_COLUMN       8



/**************************************************************************************
                   OPEN_STOK_URUN_HAREKETLERI_ARAMA
***************************************************************************************/

void OPEN_STOK_URUN_HAREKETLERI_ARAMA ( int p_urun_id, QWidget * parent )
{
    STOK_URUN_HAREKETLERI_ARAMA *  F  = new STOK_URUN_HAREKETLERI_ARAMA( p_urun_id, parent);
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   STOK_URUN_HAREKETLERI_ARAMA::STOK_URUN_HAREKETLERI_ARAMA
***************************************************************************************/

STOK_URUN_HAREKETLERI_ARAMA::STOK_URUN_HAREKETLERI_ARAMA ( int p_urun_id, QWidget * parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::STOK_URUN_HAREKETLERI_ARAMA )
{
    m_urun_id = p_urun_id;
    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, DB );
}

/**************************************************************************************
                   STOK_URUN_HAREKETLERI_ARAMA::SETUP_FORM
***************************************************************************************/

void STOK_URUN_HAREKETLERI_ARAMA::SETUP_FORM ()
{

    REGISTER_TABLE_WIDGET ( m_ui->tableWidget );

    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS ( QStringList() << tr("") << tr("") << tr("") << tr("") << tr("fis_id") << tr("Fiş Tarihi") << tr("Fiş No") << tr("Açıklama") << tr("Tutar"));

    SET_SORTING ( false );

    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_bas_tarihi );

    SET_PAGE_TITLE    (tr("ÜRÜN HAREKETLERİ"));

    SET_HELP_PAGE     ("urun_hareketleri_arama");

    m_ui->tableWidget->hideColumn( MODUL_ID_COLUMN );
    m_ui->tableWidget->hideColumn( FIS_ID_COLUMN );
    m_ui->tableWidget->hideColumn( FIS_TURU_COLUMN );
    m_ui->tableWidget->hideColumn( IADE_FISI_MI_COLUMN );
    m_ui->tableWidget->hideColumn( IRS_MI_STOK_MU_COLUMN );
    m_ui->tableWidget->setColumnWidth( FIS_TARIHI_COLUMN   , 100);
    m_ui->tableWidget->setColumnWidth( FIS_NO_COLUMN       , 40 );
    m_ui->tableWidget->setColumnWidth( ACIKLAMA_COLUMN     , 200);
    m_ui->tableWidget->setColumnWidth( FIS_TUTARI_COLUMN   , 100);

    SET_SETTING_NAME  ("STOK_URUN_HAREKETLERI_ARAMA");

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    SET_FIND_BUTTON_NAME ( "Bul" );

    AUTO_START();
}

/**************************************************************************************
                   STOK_URUN_HAREKETLERI_ARAMA::CHECK_VAR
***************************************************************************************/

int STOK_URUN_HAREKETLERI_ARAMA::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->adakDate_bts_tarihi ) {
        if ( m_ui->adakDate_bas_tarihi->DATE() > m_ui->adakDate_bts_tarihi->DATE() ) {
            MSG_ERROR(tr("Son tarih ilk tarihten küçük olamaz!..") , NULL);
            return ADAK_FAIL;
        }
     }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_HAREKETLERI_ARAMA::CHECK_RUN
***************************************************************************************/

int STOK_URUN_HAREKETLERI_ARAMA::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_HAREKETLERI_ARAMA::SEARCH
***************************************************************************************/

void STOK_URUN_HAREKETLERI_ARAMA::SEARCH ()
{
    SQL_QUERY query( DB );

    QList<int> irs_id_list;

    query.PREPARE_SELECT( "stk_fisler, stk_fis_satirlari",
                          "stk_fisler.fis_id, fis_tarihi", "stk_fisler.fis_id = stk_fis_satirlari.fis_id AND urun_id = :urun_id");
    query.SET_VALUE     ( ":urun_id", m_urun_id );

    if( query.SELECT( "fis_tarihi") NE 0 ) {
        while( query.NEXT() EQ true ) {
            int fis_id        = query.VALUE(0).toInt();
            if( irs_id_list.contains( fis_id) EQ false ) {
                irs_id_list << fis_id;
            }
        }
    }

    FATURA_FISI_STRUCT FAT_FIS;

    FAT_FIS.KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];

    STK_IRS_FIS_STRUCT IRS_FIS;

    QList<int> fat_fis_list = FAT_GET_FIS_ID_LIST( m_urun_id );

    int current_row = 0;
    for (int i = 0; i < fat_fis_list.size(); ++i) {
        FATURA_CLEAR_FIS_STRUCT( &FAT_FIS );

        FATURA_FIS_BILGILERINI_OKU( &FAT_FIS, fat_fis_list.at(i));

        if( FAT_FIS.fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI OR
            FAT_FIS.fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) {

            int irsaliye_id = STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID( fat_fis_list.at(i));

            irs_id_list.removeOne( irsaliye_id );
        }

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item(current_row, MODUL_ID_COLUMN )->setText   ( QString::number( FAT_FIS.modul_id ));
        m_ui->tableWidget->item(current_row, FIS_TURU_COLUMN )->setText   ( QString::number( FAT_FIS.fatura_turu ));
        m_ui->tableWidget->item(current_row, IADE_FISI_MI_COLUMN )->setText   ( QString::number( FAT_FIS.iade_faturasi_mi ));
        m_ui->tableWidget->item(current_row, IRS_MI_STOK_MU_COLUMN )->setText   ( QString::number( -1 ));
        m_ui->tableWidget->item(current_row, FIS_ID_COLUMN )->setText     ( QString::number( fat_fis_list.at(i) ));
        m_ui->tableWidget->item(current_row, FIS_TARIHI_COLUMN )->setText ( QDate::fromString( FAT_FIS.fatura_tarihi, "yyyy.MM.dd" ).toString( "dd MMMM yyyy" ) );
        m_ui->tableWidget->item(current_row, FIS_NO_COLUMN )->setText     ( QString::number( FAT_FIS.fis_no ) );
        m_ui->tableWidget->item(current_row, ACIKLAMA_COLUMN )->setTextAlignment( Qt::AlignLeft + Qt::AlignVCenter );
        m_ui->tableWidget->item(current_row, ACIKLAMA_COLUMN )->setText   ( FAT_GET_FATURA_TURLERI_STRING( FAT_FIS.fatura_turu ));
        m_ui->tableWidget->item(current_row, FIS_TUTARI_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item(current_row, FIS_TUTARI_COLUMN )->setText ( VIRGUL_EKLE( QVariant( FAT_FIS.fatura_tutari ).toString(), 2 ) );
    }

    for (int i = 0; i < irs_id_list.size(); ++i) {

        STK_IRS_CLEAR_FIS_STRUCT( &IRS_FIS );

        STK_IRS_FIS_BILGILERINI_OKU( irs_id_list.at(i), &IRS_FIS );

        if( IRS_FIS.modul_id NE STOK_MODULU ) {
            IRS_FIS.modul_id = IRSALIYE_MODULU;
        }

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item(current_row, MODUL_ID_COLUMN )->setText   ( QString::number( IRS_FIS.modul_id ));
        m_ui->tableWidget->item(current_row, FIS_TURU_COLUMN )->setText   ( QString::number( IRS_FIS.fis_turu ));
        m_ui->tableWidget->item(current_row, IADE_FISI_MI_COLUMN )->setText( QString::number( IRS_FIS.iade_irsaliyesi_mi ));
        m_ui->tableWidget->item(current_row, IRS_MI_STOK_MU_COLUMN )->setText( QString::number( IRS_FIS.irsaliye_mi_stok_fisi_mi ));
        m_ui->tableWidget->item(current_row, FIS_ID_COLUMN )->setText     ( QString::number( irs_id_list.at(i) ));
        m_ui->tableWidget->item(current_row, FIS_TARIHI_COLUMN )->setText ( QDate::fromString( IRS_FIS.fis_tarihi, "yyyy.MM.dd" ).toString( "dd MMMM yyyy" ) );
        m_ui->tableWidget->item(current_row, FIS_NO_COLUMN )->setText     ( QString::number( IRS_FIS.fis_no ) );
        m_ui->tableWidget->item(current_row, ACIKLAMA_COLUMN )->setTextAlignment( Qt::AlignLeft + Qt::AlignVCenter );
        m_ui->tableWidget->item(current_row, ACIKLAMA_COLUMN )->setText   ( STK_GET_STOK_FIS_TURU( IRS_FIS.fis_turu ));
        m_ui->tableWidget->item(current_row, FIS_TUTARI_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item(current_row, FIS_TUTARI_COLUMN )->setText ( VIRGUL_EKLE( QVariant( IRS_FIS.irsaliye_tutari ).toString(), 2 ) );

    }
}

/**************************************************************************************
                        STOK_URUN_HAREKETLERI_ARAMA::SELECTED_LINE
***************************************************************************************/

void STOK_URUN_HAREKETLERI_ARAMA::SELECTED_LINE ( int selected_row_number )
{
    Q_UNUSED( selected_row_number );
}

/**************************************************************************************
                   STOK_URUN_HAREKETLERI_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int STOK_URUN_HAREKETLERI_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    int modul_id        = m_ui->tableWidget->item ( selected_row_number, MODUL_ID_COLUMN )->text().toInt();
    int fis_id          = m_ui->tableWidget->item ( selected_row_number, FIS_ID_COLUMN   )->text().toInt();
    int iade_fisi_mi    = m_ui->tableWidget->item ( selected_row_number, IADE_FISI_MI_COLUMN   )->text().toInt();
    int fis_turu        = m_ui->tableWidget->item ( selected_row_number, FIS_TURU_COLUMN   )->text().toInt();
    int irs_mi_stok_mu  = m_ui->tableWidget->item ( selected_row_number, IRS_MI_STOK_MU_COLUMN   )->text().toInt();

    switch( modul_id ) {

    case FATURA_MODULU  :
        OPEN_FATURA_FISI( fis_turu, fis_id, iade_fisi_mi, nativeParentWidget() );
        break;

    case IRSALIYE_MODULU:
    case STOK_MODULU    :
        OPEN_STOK_FISI( fis_id, irs_mi_stok_mu, fis_turu, iade_fisi_mi, nativeParentWidget() );
        break;

    default:
        break;
    }



    return ADAK_CONTINUE;
}
