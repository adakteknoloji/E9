#include <QTableWidget>
#include "adak_sql.h"
#include "fat_iade_satir_secimi_class.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "stok_console_utils.h"
#include "fat_enum.h"


extern ADAK_SQL *           DB;

#define     FATURA_SATIRI_ID_COLUMN             0
#define     URUN_HIZMET_KODU_COLUMN             1
#define     URUN_HIZMET_ADI_COLUMN              2
#define     KALAN_MIKTAR_COLUMN                 3
#define     URUN_BIRIMI_COLUMN                  4
#define     BIRIM_FIYAT_COLUMN                  5
#define     KDV_DAHIL_MI_COLUMN                 6
#define     KDV_ORANI_COLUMN                    7
#define     OTV_ORANI_COLUMN                    8
#define     PARA_BIRIMI_COLUMN                  9


/**************************************************************************************
                   OPEN_FAT_IADE_SATIR_SECIMI
***************************************************************************************/

int OPEN_FAT_IADE_SATIR_SECIMI ( int p_fatura_id, QWidget * p_parent )
{
    int          secilen_id = -1;

    FAT_IADE_SATIR_SECIMI * F = new FAT_IADE_SATIR_SECIMI ( p_fatura_id, p_parent );

    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );

    return ( secilen_id );
}

/**************************************************************************************
                   FAT_IADE_SATIR_SECIMI::FAT_IADE_SATIR_SECIMI
***************************************************************************************/

FAT_IADE_SATIR_SECIMI::FAT_IADE_SATIR_SECIMI ( int p_fatura_id, QWidget * parent ) : SECIM_KERNELI ( parent )
{
    m_fatura_id = p_fatura_id;

    SET_PAGE_TITLE      ( tr ( "FAT - FATURA İADE SATIR SEÇİMİ" ) );
    SET_SETTING_NAME    ( "FAT_IADE_SATIR_SECIMI"             );
    SET_HELP_PAGE       ( tr ( "fat_iade_satir_secimi.html" ) );

    SET_SORTING         ( false );
    SET_WINDOW_SIZE     ( 600, 1100 );

    SET_HEADERS ( QStringList () <<tr("fatura_satiri_id")<<tr("Ürün / Hizmet Kodu")<<tr("Ürün / Hizmet Adı")
                                 <<tr("İade Edilebilecek Miktar")<<tr("Birimi")<<tr("Birim Fiyat")
                                 <<tr("KDV")<<tr("KDV %")<<tr("ÖTV %")<<tr("Para Brm") );
    INIT_KERNEL ( DB );

    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    tableWidget->setColumnWidth ( URUN_HIZMET_KODU_COLUMN , 100 );
    tableWidget->setColumnWidth ( URUN_HIZMET_ADI_COLUMN  , 160 );
    tableWidget->setColumnWidth ( KALAN_MIKTAR_COLUMN     , 120 );
    tableWidget->setColumnWidth ( URUN_BIRIMI_COLUMN      , 60  );
    tableWidget->setColumnWidth ( BIRIM_FIYAT_COLUMN      , 75  );
    tableWidget->setColumnWidth ( KDV_DAHIL_MI_COLUMN     , 50  );
    tableWidget->setColumnWidth ( KDV_ORANI_COLUMN        , 47  );
    tableWidget->setColumnWidth ( OTV_ORANI_COLUMN        , 47  );
    tableWidget->setColumnWidth ( PARA_BIRIMI_COLUMN      , 60  );

    QFont tableWidget_font = tableWidget->font();
    int   font_point_size  = tableWidget_font.pointSize();
    tableWidget_font.setPointSize ( font_point_size -1 );
    tableWidget->setFont ( tableWidget_font );

    tableWidget->hideColumn( FATURA_SATIRI_ID_COLUMN );
}

/**************************************************************************************
                   FAT_IADE_SATIR_SECIMI::FILL_TABLE
***************************************************************************************/

void FAT_IADE_SATIR_SECIMI::FILL_TABLE()
{
    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    SQL_QUERY f_query    ( DB );

    f_query.PREPARE_SELECT("fat_fatura_satirlari, fat_faturalar" , "urun_id, urun_adi, satirdaki_urun_miktari, irsaliyelenen_miktar, "
                           "iade_edilen_miktar, urun_birimi, isk_sonrasi_urun_birim_fiyati, "
                           "kdv_dahil_mi, kdv_orani, "
                           "otv_orani, doviz_id, fis_satiri_id , fatura_turu" ,
                           "fat_fatura_satirlari.fis_id = fat_faturalar.fis_id "
                           "AND fat_fatura_satirlari.fis_id = :fis_id");

    f_query.SET_VALUE ( ":fis_id",        m_fatura_id );

    if ( f_query.SELECT("order_number ASC") EQ 0 ) {
        return;
    }

    int current_row = 0;

    while  ( f_query.NEXT() EQ true ) {

        int         fatura_turu             = f_query.VALUE( "fatura_turu").toInt();
        double      satirdaki_urun_miktari  = f_query.VALUE ( "satirdaki_urun_miktari"    ).toDouble();
        double      irsaliyelenen_miktar    = f_query.VALUE ( "irsaliyelenen_miktar"  ).toDouble();
        double      iade_edilen_miktar      = f_query.VALUE ( "iade_edilen_miktar"    ).toDouble();

        double      kalan_miktar = 0.00;
        // HAKEDIS FATURASI FARKLI CALISIYOR
        if ( fatura_turu NE ENUM_FAT_HAKEDIS_ALIS_FATURASI AND fatura_turu NE ENUM_FAT_HAKEDIS_SATIS_FATURASI ) {
            if ( ( irsaliyelenen_miktar - iade_edilen_miktar ) <= 0 ) {
                continue;
            }
            kalan_miktar = irsaliyelenen_miktar - iade_edilen_miktar;
        }
        else {
            kalan_miktar = satirdaki_urun_miktari - iade_edilen_miktar;
        }

        QString     urun_kodu;
        int         urun_id                 =   f_query.VALUE     ( "urun_id"  ).toInt();
        QString     urun_adi                =   f_query.VALUE     ( "urun_adi" ).toString();

        QString     urun_birimi             =   f_query.VALUE     ( "urun_birimi" ).toString();
        QString     urun_birim_fiyati       =   VIRGUL_EKLE         ( f_query.VALUE("isk_sonrasi_urun_birim_fiyati").toString(), 2 );
        int         kdv_dahil_mi            =   f_query.VALUE     ( "kdv_dahil_mi" ).toInt();
        QString     kdv_orani               =   f_query.VALUE     ( "kdv_orani" ).toString();
        QString     otv_orani               =   f_query.VALUE     ( "otv_orani" ).toString();
        int         doviz_id                =   f_query.VALUE     ( "doviz_id" ).toInt();
        QString     fis_satiri_id           =   f_query.VALUE     ( "fis_satiri_id" ).toString();
        QString     doviz_kodu              =   DVZ_GET_DOVIZ_KODU      ( doviz_id );
        QString     kdv_dahil_mi_str        =   tr("DAHİL");
        QString     kalan_miktar_str        =   VIRGUL_EKLE ( QVariant( kalan_miktar).toString(), 2 );


        if ( kdv_dahil_mi EQ 0 ) {
            kdv_dahil_mi_str = tr("HARİÇ");
        }

        if ( urun_id > 0 ) {

            STK_GET_URUN_KODU_ADI(urun_id ,&urun_kodu , &urun_adi );
        }


        current_row = ADD_NEW_ROW();

        tableWidget->item ( current_row, BIRIM_FIYAT_COLUMN              )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );
        tableWidget->item ( current_row, KALAN_MIKTAR_COLUMN             )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );

        tableWidget->item ( current_row, URUN_BIRIMI_COLUMN              )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, KDV_DAHIL_MI_COLUMN             )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, KDV_ORANI_COLUMN                )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, OTV_ORANI_COLUMN                )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, PARA_BIRIMI_COLUMN              )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );


        tableWidget->item ( current_row, FATURA_SATIRI_ID_COLUMN         )->setText ( fis_satiri_id          );
        tableWidget->item ( current_row, URUN_HIZMET_KODU_COLUMN         )->setText ( urun_kodu              );
        tableWidget->item ( current_row, URUN_HIZMET_ADI_COLUMN          )->setText ( urun_adi               );
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
                   FAT_IADE_SATIR_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int FAT_IADE_SATIR_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( selected_row_number, FATURA_SATIRI_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}
