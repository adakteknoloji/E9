#include <QTableWidget>
#include "adak_utils.h"
#include "fat_irsaliye_satir_secimi_class.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h" 
#include "fat_enum.h"
#include "stok_fis_utils.h"
#include "stok_enum.h"
#include "stok_console_utils.h"

extern ADAK_SQL * DB;


#define     IRSALIYE_SATIRI_ID_COLUMN           0
#define     IRSALIYE_NO_COLUMN                  1
#define     IRSALIYE_BELGE_SERI_NO_COLUMN       2
#define     IRSALIYE_TARIHI_COLUMN              3
#define     URUN_KODU_COLUMN                    4
#define     URUN_ADI_COLUMN                     5
#define     SERI_NO_PARTI_KODU_COLUMN           6
#define     KALAN_MIKTAR_COLUMN                 7
#define     URUN_BIRIMI_COLUMN                  8
#define     BIRIM_FIYAT_COLUMN                  9
#define     KDV_DAHIL_MI_COLUMN                 10
#define     KDV_ORANI_COLUMN                    11
#define     OTV_ORANI_COLUMN                    12
#define     FIYAT_COLUMN                        13
#define     PARA_BIRIMI_COLUMN                  14
#define     TEMEL_PARA_BIRIMI_TUTARI_COLUMN     15

/*************************************************************/
/*              OPEN_FAT_IRSALIYE_SATIR_SECIMI               */
/*************************************************************/

int OPEN_FAT_IRSALIYE_SATIR_SECIMI ( QString p_fatura_tarihi, int p_cari_hesap_id, int p_fatura_turu, QWidget * p_parent )
{
    int secilen_id = -1;
    FAT_IRSALIYE_SATIR_SECIMI * F = new FAT_IRSALIYE_SATIR_SECIMI ( p_fatura_tarihi, p_cari_hesap_id, p_fatura_turu, p_parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/*********************************************************************************/
/*              FAT_IRSALIYE_SATIR_SECIMI::FAT_IRSALIYE_SATIR_SECIMI             */
/*********************************************************************************/

FAT_IRSALIYE_SATIR_SECIMI::FAT_IRSALIYE_SATIR_SECIMI ( QString p_fatura_tarihi, int p_cari_hesap_id, int p_fatura_turu, QWidget * p_parent ) : SECIM_KERNELI ( p_parent )
{

    SQL_QUERY sql_query ( DB );

    SET_HELP_PAGE  ( "fat_irsaliye_satir_secimi.html" );
    SET_PAGE_TITLE ( "FAT - İRSALİYE SATIRI SEÇİMİ " );

    m_irsaliye_tarihi_bitis = p_fatura_tarihi;
    m_cari_hesap_id         = p_cari_hesap_id;

    switch ( p_fatura_turu ) {

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS :
            m_irsaliye_turu = ENUM_IRS_NORMAL_GIRIS;
            break;

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS :
            m_irsaliye_turu = ENUM_IRS_NORMAL_CIKIS;
            break;

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS :
            m_irsaliye_turu = ENUM_IRS_KONSINYE_GIRIS;
            break;

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS:
            m_irsaliye_turu = ENUM_IRS_KONSINYE_CIKIS;
            break;

        default :
            m_irsaliye_turu = -1;
            break;
    }

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

    SET_SORTING ( false );

    SET_WINDOW_SIZE ( 600, 1100 );

    SET_HEADERS ( QStringList () <<tr("irsaliye_satiri_id")<<tr("İrs. No")<<tr("İrs. S. No")<<tr("İrsaliye Tarihi")
                                 <<tr("Ürün Kodu")<<tr("Ürün Adı")<<tr("S. No/P. Kodu")<<tr("Kalan Miktar")
                                 <<tr("Birimi")<<tr("Birim Fiyat")<<tr("KDV")<<tr("KDV %")<<tr("ÖTV %")
                                 <<tr("Fiyatı")<<tr("Para Brm")<< DVZ_GET_TEMEL_PARA_BIRIMI_KODU() + tr(" Tutarı"));
    INIT_KERNEL ( DB );

    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    tableWidget->setColumnWidth ( IRSALIYE_NO_COLUMN                   , 45  );
    tableWidget->setColumnWidth ( IRSALIYE_BELGE_SERI_NO_COLUMN        , 58  );
    tableWidget->setColumnWidth ( IRSALIYE_TARIHI_COLUMN               , 103 );
    tableWidget->setColumnWidth ( URUN_KODU_COLUMN                     , 70  );
    tableWidget->setColumnWidth ( URUN_ADI_COLUMN                      , 100 );
    tableWidget->setColumnWidth ( SERI_NO_PARTI_KODU_COLUMN            , 80  );
    tableWidget->setColumnWidth ( KALAN_MIKTAR_COLUMN                  , 80  );
    tableWidget->setColumnWidth ( URUN_BIRIMI_COLUMN                   , 60  );
    tableWidget->setColumnWidth ( BIRIM_FIYAT_COLUMN                   , 75  );
    tableWidget->setColumnWidth ( KDV_DAHIL_MI_COLUMN                  , 50  );
    tableWidget->setColumnWidth ( KDV_ORANI_COLUMN                     , 47  );
    tableWidget->setColumnWidth ( OTV_ORANI_COLUMN                     , 47  );
    tableWidget->setColumnWidth ( FIYAT_COLUMN                         , 80  );
    tableWidget->setColumnWidth ( PARA_BIRIMI_COLUMN                   , 60  );
    tableWidget->setColumnWidth ( TEMEL_PARA_BIRIMI_TUTARI_COLUMN      , 80  );

    SET_SETTING_NAME            ( "FAT_IRSALIYE_SATIR_SECIMI" );

    QFont tableWidget_font = tableWidget->font();
    int   font_point_size  = tableWidget_font.pointSize();
    tableWidget_font.setPointSize ( font_point_size -1 );
    tableWidget->setFont ( tableWidget_font );

    tableWidget->hideColumn( IRSALIYE_SATIRI_ID_COLUMN );
}

/*********************************************************************/
/*              FAT_IRSALIYE_SATIR_SECIMI::FILL_TABLE                */
/*********************************************************************/

void FAT_IRSALIYE_SATIR_SECIMI::FILL_TABLE()
{
    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    QMap <int , int> irs_fis_ve_satir_id_map = STK_GET_STK_IRS_FIS_AND_FIS_SATIRI_ID_MAP( m_cari_hesap_id , m_irsaliye_turu , 0 , m_irsaliye_tarihi_baslangic,
                                          m_irsaliye_tarihi_bitis );

    STK_IRS_FIS_STRUCT * IRS_FISI = new STK_IRS_FIS_STRUCT;

    STK_IRS_FIS_SATIR_STRUCT * IRS_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

    int current_row = 0;

    for ( int i = 0 ; i < irs_fis_ve_satir_id_map.size() ; i++ ) {

        STK_IRS_CLEAR_FIS_STRUCT   ( IRS_FISI);
        STK_IRS_CLEAR_SATIR_STRUCT ( IRS_FIS_SATIRI);

        int irsaliye_id   = irs_fis_ve_satir_id_map.keys().at(i);
        int irs_satiri_id = irs_fis_ve_satir_id_map.values().at(i);

        STK_IRS_FIS_BILGILERINI_OKU  (irsaliye_id   , IRS_FISI);
        STK_IRS_SATIR_BILGILERINI_OKU(irs_satiri_id , IRS_FIS_SATIRI);

        double      satirdaki_urun_miktari  =   IRS_FIS_SATIRI->satirdaki_urun_miktari;
        double      faturalanan_miktar      =   IRS_FIS_SATIRI->faturalanan_miktar;
        double      kalan_miktar            =   ROUND ( satirdaki_urun_miktari - faturalanan_miktar );

        if ( kalan_miktar <= 0 ) {
            continue;
        }
        QString kdv_dahil_mi_str = tr("DAHİL");

        if ( IRS_FIS_SATIRI->kdv_dahil_mi EQ 0 ) {
            kdv_dahil_mi_str = tr("HARİÇ");
        }

        QString urun_kodu;
        QString urun_adi = IRS_FIS_SATIRI->urun_adi;
        QString seri_or_parti_kodu;


        if ( IRS_FIS_SATIRI->urun_id > 0 ) {

            STK_GET_URUN_KODU_ADI(IRS_FIS_SATIRI->urun_id , &urun_kodu , &urun_adi);
        }

        if ( IRS_FIS_SATIRI->parti_id > 0 ) {
            seri_or_parti_kodu = STK_GET_PARTI_KODU(IRS_FIS_SATIRI->parti_id);
        }
        else {
           seri_or_parti_kodu = STK_GET_SERI_NO_BILGILERI( irs_satiri_id , ENUM_URUN_HER_IKISI, -1 );
        }

        current_row = ADD_NEW_ROW();

        tableWidget->item ( current_row, BIRIM_FIYAT_COLUMN              )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );
        tableWidget->item ( current_row, FIYAT_COLUMN                    )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );
        tableWidget->item ( current_row, TEMEL_PARA_BIRIMI_TUTARI_COLUMN )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );
        tableWidget->item ( current_row, KALAN_MIKTAR_COLUMN             )->setTextAlignment ( Qt::AlignRight  | Qt::AlignVCenter );

        tableWidget->item ( current_row, IRSALIYE_NO_COLUMN              )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, IRSALIYE_BELGE_SERI_NO_COLUMN   )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, IRSALIYE_TARIHI_COLUMN          )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, SERI_NO_PARTI_KODU_COLUMN       )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, URUN_BIRIMI_COLUMN              )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, KDV_DAHIL_MI_COLUMN             )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, KDV_ORANI_COLUMN                )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, OTV_ORANI_COLUMN                )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );
        tableWidget->item ( current_row, PARA_BIRIMI_COLUMN              )->setTextAlignment ( Qt::AlignCenter | Qt::AlignVCenter );


        tableWidget->item ( current_row, IRSALIYE_SATIRI_ID_COLUMN       )->setText ( QVariant( irs_satiri_id ).toString()     );
        tableWidget->item ( current_row, IRSALIYE_NO_COLUMN              )->setText ( QVariant(IRS_FISI->fis_no).toString()                 );
        tableWidget->item ( current_row, IRSALIYE_BELGE_SERI_NO_COLUMN   )->setText ( IRS_FISI->belge_seri + " " + IRS_FISI->belge_numarasi );
        tableWidget->item ( current_row, IRSALIYE_TARIHI_COLUMN          )->setText ( QDate::fromString(IRS_FISI->fis_tarihi , "yyyy.MM.dd").toString("dd MMMM yyyy")        );
        tableWidget->item ( current_row, URUN_KODU_COLUMN                )->setText ( urun_kodu              );
        tableWidget->item ( current_row, URUN_ADI_COLUMN                 )->setText ( urun_adi              );
        tableWidget->item ( current_row, SERI_NO_PARTI_KODU_COLUMN       )->setText ( seri_or_parti_kodu       );
        tableWidget->item ( current_row, KALAN_MIKTAR_COLUMN             )->setText ( VIRGUL_EKLE(QVariant(kalan_miktar).toString(),2)       );
        tableWidget->item ( current_row, URUN_BIRIMI_COLUMN              )->setText ( IRS_FIS_SATIRI->urun_birimi            );
        tableWidget->item ( current_row, BIRIM_FIYAT_COLUMN              )->setText ( VIRGUL_EKLE(QVariant(IRS_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati).toString(),2)     );
        tableWidget->item ( current_row, KDV_DAHIL_MI_COLUMN             )->setText ( kdv_dahil_mi_str      );
        tableWidget->item ( current_row, KDV_ORANI_COLUMN                )->setText ( QVariant(IRS_FIS_SATIRI->kdv_orani).toString()             );
        tableWidget->item ( current_row, OTV_ORANI_COLUMN                )->setText ( QVariant(IRS_FIS_SATIRI->otv_orani).toString()              );
        tableWidget->item ( current_row, FIYAT_COLUMN                    )->setText ( VIRGUL_EKLE(QVariant(IRS_FIS_SATIRI->satir_tutari).toString(),2)           );
        tableWidget->item ( current_row, PARA_BIRIMI_COLUMN              )->setText ( DVZ_GET_DOVIZ_KODU ( IRS_FIS_SATIRI->doviz_id )             );
        tableWidget->item ( current_row, TEMEL_PARA_BIRIMI_TUTARI_COLUMN )->setText ( VIRGUL_EKLE(QVariant(IRS_FIS_SATIRI->satir_vergi_haric_tutar).toString() ,2)       );

    }
}

/***************************************************************************/
/*            FAT_IRSALIYE_SATIR_SECIMI::SINGLE_LINE_SELECTED              */
/***************************************************************************/

int FAT_IRSALIYE_SATIR_SECIMI::SINGLE_LINE_SELECTED ( int p_selected_row_number )
{
    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    *m_secilen_id = tableWidget->item ( p_selected_row_number, IRSALIYE_SATIRI_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}

