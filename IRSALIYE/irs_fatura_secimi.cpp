#include <QTableWidget>
#include <QDate>
#include "irs_fatura_secimi_class.h"
#include "irs_fatura_secimi_open.h"
#include "fat_enum.h"
#include "fat_fis_utils.h"
#include "fat_console_utils.h"
#include "stok_enum.h"
#include "e9_enum.h"


extern ADAK_SQL * DB;

/***************************************************/
/*               OPEN_IRS_FATURA_SECIMI            */
/***************************************************/

int OPEN_IRS_FATURA_SECIMI ( int p_cari_hesap_id, int p_irsaliye_turu, QWidget * p_parent )
{
    int g_secilen_id = -1;

    IRS_FATURA_SECIMI * F = new IRS_FATURA_SECIMI ( p_cari_hesap_id, p_irsaliye_turu, p_parent );

    F->EXEC( NOT_FULL_SCREEN );

    g_secilen_id = F->m_secilen_id;

    return g_secilen_id;
}


/**************************************************************/
/*              IRS_FATURA_SECIMI::IRS_FATURA_SECIMI          */
/**************************************************************/

IRS_FATURA_SECIMI::IRS_FATURA_SECIMI ( int p_cari_hesap_id, int p_irsaliye_turu, QWidget * p_parent )
: SECIM_KERNELI ( p_parent )
{

    SET_HELP_PAGE  ( "fatura_secimi.html"  );
    SET_PAGE_TITLE ( tr ( "İRS - FATURA SEÇİMİ" ) );

    m_cari_hesap_id = p_cari_hesap_id;

    switch ( p_irsaliye_turu ) {

        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS :
            m_fatura_turu = ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI;
            break;

        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS :
            m_fatura_turu = ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI;
            break;

        default :
            m_fatura_turu = -1;
            break;
    }

    SET_SORTING      ( false );
    SET_WINDOW_SIZE  ( 300, 500 );
    SET_HEADERS      ( QStringList() <<tr("fis_id") <<tr("Fatura No") <<tr("Fatura Belge Seri")
                       <<tr("Fatura Tarihi"));

    INIT_KERNEL      ( DB );

    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    tableWidget->setColumnWidth ( 1, 100 );
    tableWidget->setColumnWidth ( 2, 100 );
    tableWidget->setColumnWidth ( 3, 100 );

    SET_SETTING_NAME ( "IRS_FATURA_SECIMI" );

    tableWidget->hideColumn(0);
}

/*************************************************/
/*         IRS_FATURA_SECIMI::FILL_TABLE         */
/*************************************************/

void IRS_FATURA_SECIMI::FILL_TABLE ()
{

    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    QList <int > fatura_fis_list = FAT_GET_FATURA_FIS_ID_LIST( m_cari_hesap_id, m_fatura_turu, 0, 0, "", "" );

    FATURA_FISI_STRUCT FAT_FIS;

    FAT_FIS.KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];

    int current_row = 0;

    for ( int i = 0; i < fatura_fis_list.size(); i++ ) {

        current_row = ADD_NEW_ROW ();
        FATURA_CLEAR_FIS_STRUCT ( &FAT_FIS );
        FATURA_FIS_BILGILERINI_OKU( &FAT_FIS, fatura_fis_list.at( i ) );

        tableWidget->item ( current_row, 0 )->setText ( QVariant ( fatura_fis_list.at( i ) ).toString() ); // fis_id
        tableWidget->item ( current_row, 1 )->setText ( QVariant ( FAT_FIS.fis_no ).toString() ); // fis_no
        tableWidget->item ( current_row, 2 )->setText ( FAT_FIS.belge_seri + " " + FAT_FIS.belge_numarasi ); // belge_seri + belge_numarasi
        tableWidget->item ( current_row, 3 )->setText ( QDate::fromString( FAT_FIS.fatura_tarihi , "yyyy.MM.dd").toString("dd MMMM yyyy ") ); // fatura_tarihi
    }
}

/********************************************************/
/*    IRS_FATURA_SECIMI::SINGLE_LINE_SELECTED           */
/********************************************************/

int IRS_FATURA_SECIMI::SINGLE_LINE_SELECTED ( int p_selected_row_number )
{
    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    m_secilen_id = tableWidget->item ( p_selected_row_number, 0 )->text().toInt();

    return ADAK_EXIT;
}
