#include <QComboBox>
#include "fat_enum.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "e9_yetki.h"
#include "stok_enum.h"
#include "irs_gui_utils.h"
#include "irs_enum.h"
 

extern ADAK_SQL * DB;

/**************************************************************************************
                   IRS_FILL_IRSALIYE_TURU_COMBOBOX
***************************************************************************************/

void IRS_FILL_IRSALIYE_TURU_COMBOBOX ( QComboBox * p_comboBox, int p_irsaliye_stok_fisi_alis_satis_turu, int p_irsaliye_turu )
{
    p_comboBox->clear();

    QStringList irsaliye_turleri;

    switch ( p_irsaliye_stok_fisi_alis_satis_turu ) {
        case ENUM_ALIS_IRSALIYESI_FATURASI :
        default                                    :

            irsaliye_turleri    << QObject::tr ( "ALIŞ İRSALİYESİ - NORMAL" )
                                << QObject::tr ( "ALIŞ İRSALİYESİ - FASON BAŞLANGIÇ" )
                                << QObject::tr ( "ALIŞ İRSALİYESİ - FASON BİTİŞ" )
                                << QObject::tr ( "ALIŞ İRSALİYESİ - KONSİNYE" )
                                << QObject::tr ( "ALIŞ İRSALİYESİ - HAKEDİŞ" );

            break;

        case ENUM_SATIS_IRSALIYESI_FATURASI :
           irsaliye_turleri << QObject::tr ( "SATIŞ İRSALİYESİ - NORMAL" )
                            << QObject::tr ( "SATIŞ İRSALİYESİ - FASON BAŞLANGIÇ" )
                            << QObject::tr ( "SATIŞ İRSALİYESİ - FASON BİTİŞ" )
                            << QObject::tr ( "SATIŞ İRSALİYESİ - KONSİNYE" )
                            << QObject::tr ( "SATIŞ İRSALİYESİ - HAKEDİŞ" );
            break;
    }

    if ( p_irsaliye_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS OR p_irsaliye_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS OR
         p_irsaliye_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS OR p_irsaliye_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS ) {

        if ( p_irsaliye_stok_fisi_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
            irsaliye_turleri << QObject::tr ( "ELLE FATURA İRSALİYELEŞTİRME - ALIŞ" );
        }
        else {
            irsaliye_turleri << QObject::tr ( "ELLE FATURA İRSALİYELEŞTİRME - SATIŞ" );
        }
        if ( p_irsaliye_stok_fisi_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
            irsaliye_turleri << QObject::tr ( "OTOMATİK FATURA İRSALİYELEŞTİRME - ALIŞ" );
        }
        else {
            irsaliye_turleri << QObject::tr ( "OTOMATİK FATURA İRSALİYELEŞTİRME - SATIŞ" );
        }
    }

    p_comboBox->addItems(irsaliye_turleri);
}

/**************************************************************************************
                   IRS_IADE_IRSALIYE_SATIR_EKLE_ICIN_MIKTAR_KONTROLU
***************************************************************************************/

bool IRS_IADE_IRSALIYE_SATIR_EKLE_ICIN_MIKTAR_KONTROLU ( int p_irsaliye_satiri_id, double p_irsaliye_satir_miktari, QWidget * p_parent )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "stk_fis_satirlari","satirdaki_urun_miktari, iade_edilen_miktar" ,
                               "fis_satiri_id = :fis_satiri_id" );

    sql_query.SET_VALUE ( ":fis_satiri_id", p_irsaliye_satiri_id );

    if ( sql_query.SELECT() EQ 0 ) {
        ADAK_WARNING( QObject::tr("İrsaliye satırı okunamadı"), NULL, p_parent );
        return false;
    }

    sql_query.NEXT();

    double satirdaki_urun_miktari  = sql_query.VALUE(0).toDouble();
    double iade_edilen_miktar      = sql_query.VALUE(1).toDouble();

    double kalan_miktar            = ROUND ( satirdaki_urun_miktari - iade_edilen_miktar );

    if ( p_irsaliye_satir_miktari > kalan_miktar ) {

        ADAK_WARNING( QObject::tr ("İrsaliye satırında iade edilebilecek "
                                 "maksimum stok miktarı aşıldı. \n "
                                 "İade edilebilecek "
                                 "maksimum stok miktarı ") +
                        VIRGUL_EKLE ( QVariant ( kalan_miktar ).toString() ), NULL, p_parent  );
        return false;

    }

    return true;

}

/**************************************************************************************
                   IRS_IADE_IRSALIYE_SATIR_GUNCELLE_ICIN_MIKTAR_KONTROLU
***************************************************************************************/

bool IRS_IADE_IRSALIYE_SATIR_GUNCELLE_ICIN_MIKTAR_KONTROLU ( int p_irsaliye_satiri_id, double p_irsaliye_satir_miktari, QWidget * p_parent )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "stk_fis_satirlari","satirdaki_urun_miktari" ,"fis_satiri_id = :fis_satiri_id" );

    sql_query.SET_VALUE ( ":fis_satiri_id", p_irsaliye_satiri_id );

    if ( sql_query.SELECT() EQ 0 ) {
        ADAK_WARNING( QObject::tr("İrsaliye satırı okunamadı"), NULL, p_parent );
        return false;
    }

    sql_query.NEXT();

    double satirdaki_urun_miktari  = sql_query.VALUE(0).toDouble();

    if ( p_irsaliye_satir_miktari > satirdaki_urun_miktari ) {

        ADAK_WARNING( QObject::tr ("İrsaliye satırında iade edilebilecek "
                                                                          "maksimum stok miktarı aşıldı. \n "
                                                                          "İade edilebilecek "
                                                                          "maksimum stok miktarı ") +
                                                                          VIRGUL_EKLE ( QVariant ( satirdaki_urun_miktari ).toString() ), NULL, p_parent );
        return false;

    }

    return true;

}

/**************************************************************************************
                   IRS_FILL_ALIS_SATIS_COMBO_BOX
***************************************************************************************/

void IRS_FILL_ALIS_SATIS_COMBO_BOX ( QComboBox *p_combo_box )
{
    p_combo_box->clear();
    p_combo_box->addItems( QStringList() << "Alış İrsaliyeleri" << "Satış İrsaliyeleri" << "İptal Edilmiş İrsaliyeler" << "Tüm İrsaliyeler" );
    p_combo_box->setCurrentIndex(-1);
}

/**************************************************************************************
                   IRS_FILL_FATURA_DURUMU_COMBO_BOX
***************************************************************************************/

void IRS_FILL_FATURA_DURUMU_COMBO_BOX ( QComboBox *p_combo_box )
{
    p_combo_box->clear();
    p_combo_box->addItems( QStringList() << "Faturalanmış" << "Faturalanmamış" );
    p_combo_box->setCurrentIndex(-1);
}

/**************************************************************************************
                   IRS_GET_ALIS_SATIS_ENUM
***************************************************************************************/

int IRS_GET_ALIS_SATIS_ENUM ( QString irsaliye_cinsi )
{
    if ( irsaliye_cinsi EQ QObject::tr("Alış İrsaliyeleri") ) {
        return ENUM_ALIS_IRSALIYESI;
    }
    else if ( irsaliye_cinsi EQ QObject::tr( "Satış İrsaliyeleri") ) {
        return ENUM_SATIS_IRSALIYESI;
    }
    else if ( irsaliye_cinsi EQ QObject::tr( "İptal Edilmiş İrsaliyeler") ){
        return ENUM_IPTAL_IRSALIYELERI;
    }
    return ENUM_TUM_IRSALIYELER;
}
