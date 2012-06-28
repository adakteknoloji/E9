#include <QTableWidget>
#include <QDate>
#include "adak_sql.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "irs_iade_satir_secimi_class.h"
#include "irs_iade_satir_secimi_open.h"
#include "stok_console_utils.h"
#include "stok_enum.h"
 

#define     IRSALIYE_SATIRI_ID_COLUMN           0
#define     URUN_KODU_COLUMN                    1
#define     URUN_ADI_COLUMN                     2
#define     SERI_NO_PARTI_KODU_COLUMN           3
#define     KALAN_MIKTAR_COLUMN                 4
#define     URUN_BIRIMI_COLUMN                  5
#define     BIRIM_FIYAT_COLUMN                  6
#define     KDV_DAHIL_MI_COLUMN                 7
#define     KDV_ORANI_COLUMN                    8
#define     OTV_ORANI_COLUMN                    9
#define     PARA_BIRIMI_COLUMN                  10

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_IRS_IADE_SATIR_SECIMI
***************************************************************************************/

int OPEN_IRS_IADE_SATIR_SECIMI ( int p_irsaliye_id, QWidget * p_parent )
{
    int                     secilen_id = -1;
    IRS_IADE_SATIR_SECIMI * F          = new IRS_IADE_SATIR_SECIMI ( p_irsaliye_id, p_parent );
    F->m_secilen_id                    = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return ( secilen_id );
}

/**************************************************************************************
                   IRS_IADE_SATIR_SECIMI::IRS_IADE_SATIR_SECIMI
***************************************************************************************/

IRS_IADE_SATIR_SECIMI::IRS_IADE_SATIR_SECIMI ( int p_irsaliye_id, QWidget * p_parent ) : SECIM_KERNELI ( p_parent )
{
    m_irsaliye_id       =   p_irsaliye_id;

    SET_PAGE_TITLE      ( tr ( "İRS - İRSALİYE İADE SATIR SEÇİMİ" ) );
    SET_SETTING_NAME    ( "IRS_IADE_SATIR_SECIMI"             );
    SET_HELP_PAGE       ( tr ( "irs_iade_satir_secimi.html" ) );

    SET_SORTING         ( false );
    SET_WINDOW_SIZE     ( 600, 1100 );

    SET_HEADERS ( QStringList () <<tr("irsaliye_satiri_id")<<tr("Ürün Kodu")<<tr("Ürün Adı")<<tr("S. No/P. Kodu")
                                 <<tr("Kalan Miktar")<<tr("Birimi")<<tr("Birim Fiyat")
                                 <<tr("KDV")<<tr("KDV %")<<tr("ÖTV %")<<tr("Para Brm") );
    INIT_KERNEL ( DB );

    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    tableWidget->setColumnWidth ( URUN_KODU_COLUMN                     , 80  );
    tableWidget->setColumnWidth ( URUN_ADI_COLUMN                      , 160 );
    tableWidget->setColumnWidth ( SERI_NO_PARTI_KODU_COLUMN            , 80  );
    tableWidget->setColumnWidth ( KALAN_MIKTAR_COLUMN                  , 80  );
    tableWidget->setColumnWidth ( URUN_BIRIMI_COLUMN                   , 60  );
    tableWidget->setColumnWidth ( BIRIM_FIYAT_COLUMN                   , 75  );
    tableWidget->setColumnWidth ( KDV_DAHIL_MI_COLUMN                  , 50  );
    tableWidget->setColumnWidth ( KDV_ORANI_COLUMN                     , 47  );
    tableWidget->setColumnWidth ( OTV_ORANI_COLUMN                     , 47  );
    tableWidget->setColumnWidth ( PARA_BIRIMI_COLUMN                   , 60  );

    QFont tableWidget_font = tableWidget->font();
    int   font_point_size  = tableWidget_font.pointSize();
    tableWidget_font.setPointSize ( font_point_size -1 );
    tableWidget->setFont ( tableWidget_font );

    tableWidget->hideColumn( IRSALIYE_SATIRI_ID_COLUMN );

}

/**************************************************************************************
                   IRS_IADE_SATIR_SECIMI::FILL_TABLE
***************************************************************************************/

void IRS_IADE_SATIR_SECIMI::FILL_TABLE ()
{
    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    SQL_QUERY f_query    ( DB );
    SQL_QUERY s_query    ( DB );

    f_query.PREPARE_SELECT("stk_fis_satirlari" , "urun_id, urun_adi, parti_id, satirdaki_urun_miktari, faturalanan_miktar, "
                           "iade_edilen_miktar, urun_birimi, isk_oncesi_urun_birim_fiyati, kdv_dahil_mi, kdv_orani, "
                           "otv_orani, doviz_id, fis_satiri_id" , "fis_id = :fis_id");

    f_query.SET_VALUE ( ":fis_id",        m_irsaliye_id );

    if ( f_query.SELECT("order_number ASC") EQ 0 ) {
        return;
    }

    int current_row = 0;

    while  ( f_query.NEXT() EQ true ) {

        double      satirdaki_urun_miktari  =   f_query.VALUE ( "satirdaki_urun_miktari" ).toDouble();
        double      faturalanan_miktar      =   f_query.VALUE ( "faturalanan_miktar"     ).toDouble();
        double      iade_edilen_miktar      =   f_query.VALUE ( "iade_edilen_miktar"     ).toDouble();
        double      kalan_miktar            =   satirdaki_urun_miktari - faturalanan_miktar - iade_edilen_miktar;

        if ( kalan_miktar <= 0 ) {
            continue;
        }

        QString     urun_kodu;
        QString     seri_or_parti_kodu;
        int         urun_id                 =   f_query.VALUE     ( "urun_id"  ).toInt();
        QString     urun_adi                =   f_query.VALUE     ( "urun_adi" ).toString();
        int         parti_id                =   f_query.VALUE     ( "parti_id" ).toInt();

        QString     urun_birimi             =   f_query.VALUE     ( "urun_birimi" ).toString();
        QString     urun_birim_fiyati       =   VIRGUL_EKLE         ( f_query.VALUE("isk_oncesi_urun_birim_fiyati").toString(), 2 );
        int         kdv_dahil_mi            =   f_query.VALUE     ( "kdv_dahil_mi" ).toInt();
        QString     kdv_orani               =   f_query.VALUE     ( "kdv_orani" ).toString();
        QString     otv_orani               =   f_query.VALUE     ( "otv_orani" ).toString();
        int         doviz_id                =   f_query.VALUE     ( "doviz_id" ).toInt();
        QString     irsaliye_satiri_id      =   f_query.VALUE     ( "fis_satiri_id" ).toString();
        QString     doviz_kodu              =   DVZ_GET_DOVIZ_KODU      ( doviz_id );
        QString     kdv_dahil_mi_str        =   tr("DAHİL");
        QString     kalan_miktar_str        =   VIRGUL_EKLE ( QVariant( kalan_miktar).toString(), 2 );


        if ( kdv_dahil_mi EQ 0 ) {
            kdv_dahil_mi_str = tr("HARİÇ");
        }

        if ( urun_id > 0 ) {

            s_query.PREPARE_SELECT("stk_urunler","urun_kodu, urun_adi" ,"urun_id = :urun_id");
            s_query.SET_VALUE(":urun_id", urun_id );
            if ( s_query.SELECT() > 0 ) {
                s_query.NEXT();
                urun_kodu = s_query.VALUE(0).toString();
                urun_adi  = s_query.VALUE(1).toString();
            }
        }

        if ( STK_GET_URUN_TAKIP_SEKLI( urun_id) EQ ENUM_SERI_NO_ILE_TAKIP ) {
            seri_or_parti_kodu = STK_GET_SERI_NO_BILGILERI( irsaliye_satiri_id.toInt(), ENUM_URUN_DEPODA , -1 );
        }
        else if ( parti_id > 0 ) {
            seri_or_parti_kodu = STK_GET_PARTI_KODU(parti_id);
        }

        current_row = ADD_NEW_ROW();

        tableWidget->item ( current_row, BIRIM_FIYAT_COLUMN              )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );
        tableWidget->item ( current_row, KALAN_MIKTAR_COLUMN             )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );

        tableWidget->item ( current_row, SERI_NO_PARTI_KODU_COLUMN       )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, URUN_BIRIMI_COLUMN              )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, KDV_DAHIL_MI_COLUMN             )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, KDV_ORANI_COLUMN                )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, OTV_ORANI_COLUMN                )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, PARA_BIRIMI_COLUMN              )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );


        tableWidget->item ( current_row, IRSALIYE_SATIRI_ID_COLUMN       )->setText ( irsaliye_satiri_id     );
        tableWidget->item ( current_row, URUN_KODU_COLUMN                )->setText ( urun_kodu              );
        tableWidget->item ( current_row, URUN_ADI_COLUMN                 )->setText ( urun_adi               );
        tableWidget->item ( current_row, SERI_NO_PARTI_KODU_COLUMN       )->setText ( seri_or_parti_kodu             );
        tableWidget->item ( current_row, KALAN_MIKTAR_COLUMN             )->setText ( kalan_miktar_str       );
        tableWidget->item ( current_row, URUN_BIRIMI_COLUMN              )->setText ( urun_birimi            );
        tableWidget->item ( current_row, BIRIM_FIYAT_COLUMN              )->setText ( urun_birim_fiyati      );
        tableWidget->item ( current_row, KDV_DAHIL_MI_COLUMN             )->setText ( kdv_dahil_mi_str       );
        tableWidget->item ( current_row, KDV_ORANI_COLUMN                )->setText ( kdv_orani              );
        tableWidget->item ( current_row, OTV_ORANI_COLUMN                )->setText ( otv_orani              );
        tableWidget->item ( current_row, PARA_BIRIMI_COLUMN              )->setText ( doviz_kodu             );

    }
}

/**************************************************************************************
                   IRS_IADE_SATIR_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int IRS_IADE_SATIR_SECIMI::SINGLE_LINE_SELECTED ( int p_selected_row_number )
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( p_selected_row_number, IRSALIYE_SATIRI_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}
