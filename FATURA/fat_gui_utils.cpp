#include <QComboBox>
#include "adak_sql.h"
#include "fat_enum.h"
#include "adak_utils.h"
#include "e9_yetki.h"
 

extern ADAK_SQL * DB;

/**************************************************************************************
                   FAT_FILL_COMBOBOX_FATURA_TURU
***************************************************************************************/

void FAT_FILL_COMBOBOX_FATURA_TURU ( QComboBox * p_comboBox, int p_irsaliye_stok_fisi_alis_satis_turu, int p_fatura_turu, int p_iade_faturasi_mi )
{
    p_comboBox->clear();

    QStringList fatura_turleri;

    fatura_turleri  << QObject::tr ( "FATURA + İRSALİYE" )
                    << QObject::tr ( "İRSALİYELİ FATURA" )
                    << QObject::tr ( "FATURA ( İRSALİYESİZ )" )
                    << QObject::tr ( "HİZMET FATURASI" )
                    << QObject::tr ( "VADE FARKI FATURASI" )
                    << QObject::tr ( "HAKEDİŞ FATURASI" );

    if ( p_irsaliye_stok_fisi_alis_satis_turu EQ ENUM_SATIS_IRSALIYESI_FATURASI AND  p_iade_faturasi_mi NE ENUM_FAT_IADE ) {
        fatura_turleri << QObject::tr ( "PROFORMA FATURA" );
    }

    switch ( p_fatura_turu ) {

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS:
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS:
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS:
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS:
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS:
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS:

        fatura_turleri << QObject::tr ( "ELLE İRSALİYE FATURALAŞTIRMA" )
                       << QObject::tr ( "OTOMATIK İRSALİYE FATURALAŞTIRMA" )
                       << QObject::tr ( "ELLE İRSALİYE FATURALAŞTIRMA KONSİNYE" );
            break;
        default :
            break;
    }

    p_comboBox->addItems(fatura_turleri);
}

/**************************************************************************************
                   FAT_IADE_FATURA_SATIR_EKLE_ICIN_MIKTAR_KONTROLU
***************************************************************************************/

bool FAT_IADE_FATURA_SATIR_EKLE_ICIN_MIKTAR_KONTROLU ( int p_fatura_satiri_id, double p_fatura_satir_miktari, QWidget * p_parent )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "fat_fatura_satirlari","satirdaki_urun_miktari, iade_edilen_miktar",
                               "fis_satiri_id = :fis_satiri_id" );

    sql_query.SET_VALUE ( ":fis_satiri_id", p_fatura_satiri_id );

    if ( sql_query.SELECT() EQ 0 ) {
        ADAK_WARNING( QObject::tr("Fatura satırı okunamadı"), NULL, p_parent );
        return false;
    }

    sql_query.NEXT();

    double satirdaki_urun_miktari  = sql_query.VALUE(0).toDouble();
    double iade_edilen_miktar      = sql_query.VALUE(1).toDouble();

    double kalan_miktar            = ROUND ( satirdaki_urun_miktari - iade_edilen_miktar );

    if ( p_fatura_satir_miktari > kalan_miktar ) {

        ADAK_WARNING( QObject::tr ( "Fatura satırında iade edilebilecek "
                                   "maksimum ürün miktarı aşıldı. \n "
                                   "İade edilebilecek "
                                   "maksimum ürün miktarı ") +
                                   VIRGUL_EKLE ( QVariant ( kalan_miktar ).toString() ), NULL, p_parent );
        return false;

    }

    return true;

}

/**************************************************************************************
                   FAT_IADE_FATURA_SATIR_GUNCELLE_ICIN_MIKTAR_KONTROLU
***************************************************************************************/

bool FAT_IADE_FATURA_SATIR_GUNCELLE_ICIN_MIKTAR_KONTROLU ( int p_fatura_satiri_id, double p_fatura_satir_miktari, QWidget * p_parent )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "fat_fatura_satirlari","satirdaki_urun_miktari" ,"fis_satiri_id = :fis_satiri_id" );

    sql_query.SET_VALUE ( ":fis_satiri_id", p_fatura_satiri_id );

    if ( sql_query.SELECT() EQ 0 ) {
        ADAK_WARNING( QObject::tr("Fatura satırı okunamadı"), NULL, p_parent );
        return false;
    }

    sql_query.NEXT();

    double satirdaki_urun_miktari  = sql_query.VALUE(0).toDouble();

    if ( p_fatura_satir_miktari > satirdaki_urun_miktari ) {

        ADAK_WARNING( QObject::tr ( "Fatura satırında iade edilebilecek "
                                                                               "maksimum ürün miktarı aşıldı. \n "
                                                                               "İade edilebilecek "
                                                                               "maksimum ürün miktarı ") +
                                                                                VIRGUL_EKLE ( QVariant ( satirdaki_urun_miktari ).toString() ), NULL, p_parent );
        return false;

    }

    return true;

}

/**************************************************************************************
                   FAT_FILL_ACIK_KAPALI_COMBO_BOX
***************************************************************************************/

void FAT_FILL_ACIK_KAPALI_COMBO_BOX ( QComboBox *p_combo_box )
{
    p_combo_box->clear();
    p_combo_box->addItems( QStringList() << "Tüm Faturalar" << "Açık Fatura" << "Kapalı Fatura" << "İptal Edilen Faturalar" );
}

/**************************************************************************************
                   FAT_FILL_ALIS_SATIS_FAT_TURU_COMBO_BOX
***************************************************************************************/

void FAT_FILL_ALIS_SATIS_FAT_TURU_COMBO_BOX( QComboBox *p_combo_box )
{
    p_combo_box->clear();
    p_combo_box->addItems(QStringList() << QObject::tr("Alış Faturaları") << QObject::tr("Satış Faturaları"));
}
