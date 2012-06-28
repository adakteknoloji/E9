#include <QComboBox>
#include "adak_sql.h"
#include "adak_utils.h"
#include "stok_gui_utils.h"
#include "stok_enum.h"


extern ADAK_SQL * DB;

/**************************************************************************************
                                STK_FILL_URUN_BIRIMLERI_COMBOBOX
***************************************************************************************/

void STK_FILL_URUN_BIRIMLERI_COMBOBOX ( int p_urun_id, QComboBox * p_comboBox_urun_birimleri, QString * P_STOK_BIRIM_ADLARI )
{

    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("stk_urunler","urun_temel_birimi,urun_birimleri_array" ,"urun_id = :urun_id");
    sql_query.SET_VALUE(":urun_id", p_urun_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString urun_temel_birimi = sql_query.VALUE(0).toString();
    UNPACK_STRING_ARRAY(sql_query.VALUE(1).toString() , P_STOK_BIRIM_ADLARI , URUN_BIRIMLERI_SAYISI);

    p_comboBox_urun_birimleri->clear();

    p_comboBox_urun_birimleri->addItem(urun_temel_birimi);

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        if ( P_STOK_BIRIM_ADLARI[i].isEmpty() EQ false ) {
            p_comboBox_urun_birimleri->addItem(P_STOK_BIRIM_ADLARI[i]);
        }
    }
}

/**************************************************************************************
                   STK_FILL_STOK_FIS_TURU_COMBOBOX
***************************************************************************************/

void STK_FILL_STOK_FIS_TURU_COMBOBOX ( QComboBox * p_comboBox )
{
    p_comboBox->clear();
    p_comboBox->addItems ( QStringList () << QObject::tr("SARF/FİRE FİŞİ")
                                          << QObject::tr("İMALATA SEVK FİŞİ")
                                          << QObject::tr("İMALATTAN DÖNÜŞ FİŞİ")
                                          << QObject::tr("DEPO TRANSFER GİRİŞ" )
                                          << QObject::tr("DEPO TRANSFER ÇIKIŞ" )
                                          << QObject::tr("STOK AÇILIŞ FİŞİ") );

}

/**************************************************************************************
                   STK_FILL_STOK_KART_TURLERI_COMBOBOX
***************************************************************************************/

void STK_FILL_STOK_KART_TURLERI_COMBOBOX ( QComboBox * p_comboBox )
{
    p_comboBox->clear();
    p_comboBox->addItems ( QStringList () << QObject::tr("Tümü")
                                          << QObject::tr("Ürünler")
                                          << QObject::tr("Hizmetler"));
}

/**************************************************************************************
                   STK_GET_STOK_KART_TURLERI_ENUM
***************************************************************************************/

int STK_GET_STOK_KART_TURLERI_ENUM ( QString p_stok_kart_turu )
{

    if ( p_stok_kart_turu EQ  QObject::tr("Tümü") ){
        return ENUM_URUN_HIZMET_KARTI;
    }
    else if ( p_stok_kart_turu EQ QObject::tr("Ürünler") ) {
        return ENUM_URUN_KARTI;
    }
    else if ( p_stok_kart_turu EQ QObject::tr("Hizmetler") ) {
        return ENUM_HIZMET_KARTI;
    }

    return -1;
}

/**************************************************************************************
                   STK_FILL_ALIS_SATIS_DURUMU_COMBO_BOX
***************************************************************************************/

void STK_FILL_ALIS_SATIS_DURUMU_COMBO_BOX(QComboBox *p_combo_box)
{
    p_combo_box->clear();
    p_combo_box->addItems( QStringList() << "Alış ve Satış" << "Alış" << "Satış");
}

/**************************************************************************************
                   STK_GET_ALIS_SATIS_ENUM
***************************************************************************************/

int STK_GET_ALIS_SATIS_ENUM ( QString irsaliye_cinsi )
{
    if ( irsaliye_cinsi EQ QObject::tr("Alış") ) {
        return ENUM_STOK_ALIS;
    }
    else if ( irsaliye_cinsi EQ QObject::tr( "Satış") ) {
        return ENUM_STOK_SATIS;
    }
    return ENUM_STOK_ALIS_VE_SATIS;
}



