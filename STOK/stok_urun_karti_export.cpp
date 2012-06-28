#include "stok_urun_karti_export_class.h"
#include "stok_urun_karti_export_open.h"
#include "adak_sql.h"
#include "dvz_console_utils.h"
#include <qtextcodec.h>
#include "stok_console_utils.h"
#include "stok_enum.h"
#include "stok_fis_utils.h"
#include "stok_struct.h"
#include "e9_enum.h"

extern ADAK_SQL * DB;

/***********************************************************************************
                            STOK_URUN_KARTI_EXPORT::OPEN_STOK_URUN_KARTI_EXPORT
************************************************************************************/

void OPEN_STOK_URUN_KARTI_EXPORT( QList <int> urun_id_list ,QWidget *parent )
{
    STOK_URUN_KARTI_EXPORT * A = new STOK_URUN_KARTI_EXPORT ( urun_id_list ,parent );

    A->START_EXPORT();

}

/***********************************************************************************
                            STOK_URUN_KARTI_EXPORT::STOK_URUN_KARTI_EXPORT
************************************************************************************/

STOK_URUN_KARTI_EXPORT::STOK_URUN_KARTI_EXPORT( QList <int> urun_id_list , QWidget * parent , QByteArray text_codec ) : EXPORT_KERNEL ( "Stok_urun_karti_kayit-ihrac-et", parent)
{
    m_urun_id_list = urun_id_list;
    m_text_codec = text_codec ;
}

/***********************************************************************************
                            STOK_URUN_KARTI_EXPORT::EXPORT_ROWS
************************************************************************************/

void STOK_URUN_KARTI_EXPORT::EXPORT_ROWS()
{
    QTextCodec *codec = QTextCodec::codecForName( m_text_codec );
    QStringList headers;

    headers << "Ürün Kodu";
    headers << "Ürün Adı";
    headers << "Barkod Numarası";
    headers << "Stok Takip Şekli";
    headers << "Stok Miktarı";
    headers << "Temel Birim";
    headers << "Temel Alış F.";
    headers << "Temel Alış Para Brimi";
    headers << "Temel Alış KDV Oranı";
    headers << "Temel Alış ÖTV Oranı";
    headers << "Temel Satiş F.";
    headers << "Temel Satiş Para Birimi";
    headers << "Temel Satiş KDV Oranı";
    headers << "Temel Satiş ÖTV Oranı";
    headers << "Açıklama";

    SET_ROW_TITLE( headers );

    if ( m_urun_id_list.size() EQ 0 ) {
        return;
    }

    int i = 0 ;
    while ( i < m_urun_id_list.size() ) {

        QStringList row_values;

        STK_URUNLER_STRUCT  STOK_URUN ;
        int    stok_urun_id = m_urun_id_list.at( i );
        double stok_miktari  ;

        STK_URUNLER_TABLOSUNU_OKU  ( stok_urun_id, &STOK_URUN, true ) ;

        stok_miktari = STOK_URUN.toplam_stoga_giren_array[E9_ARRAY_SIZE-1] - STOK_URUN.toplam_stoktan_cikan_array[E9_ARRAY_SIZE-1];

        row_values << codec->toUnicode  ( STOK_URUN.urun_kodu.toUtf8()                                ) ;
        row_values << codec->toUnicode  ( STOK_URUN.urun_adi.toUtf8 ()                                 ) ;
        row_values << codec->toUnicode  ( STOK_URUN.barkod_numarasi.toUtf8()                           ) ;
        row_values << codec->toUnicode  ( QString::number( STOK_URUN.stok_takip_sekli ).toUtf8()       ) ;
        row_values << codec->toUnicode  ( QString::number( stok_miktari ).toUtf8()                     ) ;
        row_values << codec->toUnicode  ( STOK_URUN.urun_temel_birimi.toUtf8()                         ) ;
        row_values << codec->toUnicode  ( QString::number( STOK_URUN.alis_fiyati ).toUtf8()            ) ;
        row_values << codec->toUnicode  ( DVZ_GET_DOVIZ_KODU( STOK_URUN.urun_alis_doviz_id ).toUtf8()  ) ;
        row_values << codec->toUnicode  ( QString::number( STOK_URUN.alis_kdv_orani ).toUtf8()         ) ;
        row_values << codec->toUnicode  ( QString::number( STOK_URUN.alis_otv_orani ).toUtf8()         ) ;
        row_values << codec->toUnicode  ( QString::number( STOK_URUN.satis_fiyati   ).toUtf8()         ) ;
        row_values << codec->toUnicode  ( DVZ_GET_DOVIZ_KODU( STOK_URUN.urun_satis_doviz_id ).toUtf8() ) ;
        row_values << codec->toUnicode  ( QString::number( STOK_URUN.satis_kdv_orani ).toUtf8()        ) ;
        row_values << codec->toUnicode  ( QString::number( STOK_URUN.satis_otv_orani ).toUtf8()        ) ;
        row_values << codec->toUnicode  ( STOK_URUN.aciklama.toUtf8()                                  ) ;

        ADD_ROW( row_values );
        ++i;
    }
}
