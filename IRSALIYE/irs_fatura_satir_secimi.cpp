#include <QDate>
#include <QTableWidget>
#include "irs_fatura_satir_secimi_class.h"
#include "irs_fatura_satir_secimi_open.h"
#include "fat_console_utils.h"
#include "fat_enum.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "adak_utils.h"
#include "fat_enum.h"
#include "fat_fis_utils.h"
#include "stok_enum.h"
#include "e9_enum.h"

extern ADAK_SQL * DB;

#define     FATURA_SATIRI_ID_COLUMN            0
#define     FATURA_NO_COLUMN                   1
#define     FATURA_SERI_NO_COLUMN              2
#define     FATURA_TARIHI_COLUMN               3
#define     URUN_KOD_COLUMN                    4
#define     URUN_AD_COLUMN                     5
#define     KALAN_MIKTAR_COLUMN                6
#define     URUN_BIRIM_COLUMN                  7
#define     BIRIM_FIYAT_COLUMN                 8
#define     KDV_DAHIL_COLUMN                   9
#define     KDV_ORAN_COLUMN                    10
#define     OTV_ORAN_COLUMN                    11
#define     FIYAT_COLUMN                       12
#define     PARA_BIRIMI_ID_COLUMN              13
#define     TEMEL_PARA_BIRIMI_TUTARI_COLUMN    14


/************************************************************/
/*               OPEN_IRS_FATURA_SATIR_SECIMI                 */
/************************************************************/

int OPEN_IRS_FATURA_SATIR_SECIMI ( int p_cari_hesap_id, int p_irsaliye_turu, QWidget * p_parent )
{
    int secilen_id = -1;
    IRS_FATURA_SATIR_SECIMI * F = new IRS_FATURA_SATIR_SECIMI ( p_cari_hesap_id, p_irsaliye_turu, p_parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/*******************************************************************************/
/*              IRS_FATURA_SATIR_SECIMI::IRS_FATURA_SATIR_SECIMI               */
/*******************************************************************************/

IRS_FATURA_SATIR_SECIMI::IRS_FATURA_SATIR_SECIMI ( int p_cari_hesap_id, int p_irsaliye_turu, QWidget * p_parent ) : SECIM_KERNELI ( p_parent )
{
    SET_HELP_PAGE ("fatura_satir_secimi.html");

    m_cari_hesap_id = p_cari_hesap_id;

    switch ( p_irsaliye_turu ) {

        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS :
            m_fatura_turu = ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI;
            break;

        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
            m_fatura_turu  = ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI;
            break;

        default :
            m_fatura_turu  = -1;
            break;
    }

    SET_PAGE_TITLE ( tr ( "İRS - FATURA SATIRI SEÇİMİ" ) );

    SET_SORTING      ( false );
    SET_WINDOW_SIZE  ( 600, 1100 );
    SET_HEADERS      ( QStringList () <<tr("fatura_satiri_id")<<tr("Fat. No")<<tr("Fat. S. No")<<tr("Fatura Tarihi")
                                      <<tr("Ürün Kodu")<<tr("Ürün Adı")<<tr("Miktarı")
                                      <<tr("Birimi")<<tr("Birim Fiyat")<<tr("KDV")<<tr("KDV %")<<tr("ÖTV %")
                                      <<tr("Fiyatı")<<tr("Para Brm")<< DVZ_GET_TEMEL_PARA_BIRIMI_KODU() + tr(" Tutarı"));

    INIT_KERNEL( DB );

    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    tableWidget->setColumnWidth ( FATURA_NO_COLUMN                , 55  );
    tableWidget->setColumnWidth ( FATURA_SERI_NO_COLUMN           , 67  );
    tableWidget->setColumnWidth ( FATURA_TARIHI_COLUMN            , 103 );
    tableWidget->setColumnWidth ( URUN_KOD_COLUMN                 , 70  );
    tableWidget->setColumnWidth ( URUN_AD_COLUMN                  , 110 );
    tableWidget->setColumnWidth ( KALAN_MIKTAR_COLUMN             , 85  );
    tableWidget->setColumnWidth ( URUN_BIRIM_COLUMN               , 60  );
    tableWidget->setColumnWidth ( BIRIM_FIYAT_COLUMN              , 75  );
    tableWidget->setColumnWidth ( KDV_DAHIL_COLUMN                , 50  );
    tableWidget->setColumnWidth ( KDV_ORAN_COLUMN                 , 50  );
    tableWidget->setColumnWidth ( OTV_ORAN_COLUMN                 , 50  );
    tableWidget->setColumnWidth ( FIYAT_COLUMN                    , 80  );
    tableWidget->setColumnWidth ( PARA_BIRIMI_ID_COLUMN           , 65  );
    tableWidget->setColumnWidth ( TEMEL_PARA_BIRIMI_TUTARI_COLUMN , 80  );

    SET_SETTING_NAME            ( "IRS_FATURA_SATIR_SECIMI" );


    QFont tableWidget_font = tableWidget->font();
    int   font_point_size  = tableWidget_font.pointSize();
    tableWidget_font.setPointSize ( font_point_size -1 );
    tableWidget->setFont ( tableWidget_font );

    tableWidget->hideColumn( FATURA_SATIRI_ID_COLUMN );
}


/***********************************************************************/
/*                  IRS_FATURA_SATIR_SECIMI::FILL_TABLE                */
/***********************************************************************/

void IRS_FATURA_SATIR_SECIMI::FILL_TABLE()
{
    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    SQL_QUERY second_query ( DB );

    // fis_id ve fis_satiri_id   map'te
    QMap <int, int> fat_fis_ve_satir_id_map = FAT_GET_FATURA_FIS_AND_FIS_SATIRI_ID_MAP( m_cari_hesap_id, m_fatura_turu, 0, "", "" );

    int current_row = 0;

    FATURA_FISI_STRUCT       FAT_FIS;
    FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;

    FAT_FIS.KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];


   for( int i = 0; i < fat_fis_ve_satir_id_map.size(); i++ ) {

        FATURA_CLEAR_FIS_STRUCT        ( &FAT_FIS );
        FATURA_CLEAR_FIS_SATIRI_STRUCT ( &FAT_FIS_SATIRI );

        int fis_id_in_map          = fat_fis_ve_satir_id_map.keys().at( i );
        int fis_satiri_id_in_map   = fat_fis_ve_satir_id_map.values().at( i );

        FATURA_FIS_BILGILERINI_OKU  ( &FAT_FIS, fis_id_in_map );
        FATURA_SATIR_BILGILERINI_OKU( &FAT_FIS_SATIRI, fis_satiri_id_in_map );

        double      satirdaki_urun_miktari  = FAT_FIS_SATIRI.satirdaki_urun_miktari;
        double      irsaliyelenen_miktar    = FAT_FIS_SATIRI.irsaliyelenen_miktar;

        if ( ( satirdaki_urun_miktari - irsaliyelenen_miktar) <= 0  ) {
            continue;
        }

        double      kalan_miktar            = satirdaki_urun_miktari - irsaliyelenen_miktar;

        QString     urun_kodu;
        QString     fis_no                   = QVariant ( FAT_FIS.fis_no ).toString();
        QString     belge_seri               = FAT_FIS.belge_seri;
        QString     belge_numarasi           = FAT_FIS.belge_numarasi;
        QString     fatura_tarihi            = QDate::fromString ( FAT_FIS.fatura_tarihi , "yyyy.MM.dd" ).toString( "dd MMMM yyyy" );
        int         urun_id                  = FAT_FIS_SATIRI.urun_id;
        QString     urun_adi                 = FAT_FIS_SATIRI.urun_adi;
        QString     urun_birimi              = FAT_FIS_SATIRI.urun_birimi;
        QString     urun_birim_fiyati        = VIRGUL_EKLE       ( QVariant ( FAT_FIS_SATIRI.isk_sonrasi_urun_birim_fiyati).toString() , 2 ); //sql_query.VALUE ( "isk_sonrasi_urun_birim_fiyati" ).toString()
        QString     satir_tutari             = VIRGUL_EKLE       ( QVariant ( FAT_FIS_SATIRI.satir_tutari ).toString(), 2 );
        int         doviz_id                 = FAT_FIS_SATIRI.doviz_id;
        QString     doviz_kodu               = DVZ_GET_DOVIZ_KODU    ( doviz_id );
        QString     vergi_haric_tutar        = VIRGUL_EKLE       ( QVariant ( FAT_FIS_SATIRI.satir_vergi_haric_tutar ).toString(), 2 ); // sql_query.VALUE ( "satir_vergi_haric_tutar" ).toString()
        int         kdv_dahil_mi             = FAT_FIS_SATIRI.kdv_dahil_mi;
        QString     kdv_orani                = QVariant ( FAT_FIS_SATIRI.kdv_orani ).toString();
        QString     otv_orani                = QVariant ( FAT_FIS_SATIRI.otv_orani ).toString();
        QString     fis_satiri_id            = QVariant ( fis_satiri_id_in_map ).toString();
        QString     kdv_dahil_mi_str         = tr("DAHİL");
        QString     kalan_miktar_str         = VIRGUL_EKLE ( QVariant( kalan_miktar).toString(), 2 );

        if ( kdv_dahil_mi EQ 0 ) {
            kdv_dahil_mi_str = tr("HARİÇ");
        }

        if ( urun_id > 0 ) {

            second_query.PREPARE_SELECT("stk_urunler","urun_kodu, urun_adi" ,"urun_id = :urun_id");
            second_query.SET_VALUE(":urun_id", urun_id );
            if ( second_query.SELECT() > 0 ) {
                second_query.NEXT();
                urun_kodu = second_query.VALUE(0).toString();
                urun_adi  = second_query.VALUE(1).toString();
            }
        }

        current_row = ADD_NEW_ROW();

        tableWidget->item ( current_row, BIRIM_FIYAT_COLUMN              )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );
        tableWidget->item ( current_row, FIYAT_COLUMN                    )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );
        tableWidget->item ( current_row, TEMEL_PARA_BIRIMI_TUTARI_COLUMN )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );
        tableWidget->item ( current_row, KALAN_MIKTAR_COLUMN             )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );

        tableWidget->item ( current_row, FATURA_NO_COLUMN                )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, FATURA_SERI_NO_COLUMN           )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, FATURA_TARIHI_COLUMN            )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, URUN_BIRIM_COLUMN              )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, KDV_DAHIL_COLUMN             )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, KDV_ORAN_COLUMN                )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, OTV_ORAN_COLUMN                )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, PARA_BIRIMI_ID_COLUMN              )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );

        tableWidget->item ( current_row, FATURA_SATIRI_ID_COLUMN         )->setText ( fis_satiri_id       );
        tableWidget->item ( current_row, FATURA_NO_COLUMN                )->setText ( fis_no                 );
        tableWidget->item ( current_row, FATURA_SERI_NO_COLUMN           )->setText ( belge_seri + " " + belge_numarasi );
        tableWidget->item ( current_row, FATURA_TARIHI_COLUMN            )->setText ( fatura_tarihi          );
        tableWidget->item ( current_row, URUN_KOD_COLUMN                )->setText ( urun_kodu              );
        tableWidget->item ( current_row, URUN_AD_COLUMN                 )->setText ( urun_adi               );
        tableWidget->item ( current_row, KALAN_MIKTAR_COLUMN             )->setText ( kalan_miktar_str       );
        tableWidget->item ( current_row, URUN_BIRIM_COLUMN              )->setText ( urun_birimi            );
        tableWidget->item ( current_row, BIRIM_FIYAT_COLUMN              )->setText ( urun_birim_fiyati      );
        tableWidget->item ( current_row, KDV_DAHIL_COLUMN             )->setText ( kdv_dahil_mi_str       );
        tableWidget->item ( current_row, KDV_ORAN_COLUMN                )->setText ( kdv_orani              );
        tableWidget->item ( current_row, OTV_ORAN_COLUMN                )->setText ( otv_orani              );
        tableWidget->item ( current_row, FIYAT_COLUMN                    )->setText ( satir_tutari           );
        tableWidget->item ( current_row, PARA_BIRIMI_ID_COLUMN              )->setText ( doviz_kodu             );
        tableWidget->item ( current_row, TEMEL_PARA_BIRIMI_TUTARI_COLUMN )->setText ( vergi_haric_tutar      );


    }

}

/************************************************************************/
/*            IRS_FATURA_SATIR_SECIMI::SINGLE_LINE_SELECTED             */
/************************************************************************/

int IRS_FATURA_SATIR_SECIMI::SINGLE_LINE_SELECTED ( int p_selected_row_number )
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( p_selected_row_number, FATURA_SATIRI_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}
