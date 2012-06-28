#include "adres_export_class.h"
#include "adres_export_open.h"
#include "adres_arama_open.h"
#include "export_kernel.h"
#include <qtextcodec.h>

extern ADAK_SQL * DB;

/***********************************************************************************
                            ADRES_EXPORT::ADRES_EXPORT
************************************************************************************/

void OPEN_ADRES_EXPORT (QList <int> adres_id_list ,QWidget *parent)
{
    ADRES_EXPORT * A = new ADRES_EXPORT (adres_id_list ,parent);

    A->START_EXPORT();

}

/***********************************************************************************
                            ADRES_EXPORT::ADRES_EXPORT
************************************************************************************/

ADRES_EXPORT::ADRES_EXPORT( QList <int> adres_id_list , QWidget * parent , QByteArray text_codec ) : EXPORT_KERNEL ( "Adres-toplu-mail-smsm_disariya-kayit-ihrac-et", parent)
{
    m_adres_id_list = adres_id_list;
    m_text_codec = text_codec ;
}

/***********************************************************************************
                            ADRES_EXPORT::ADRES_EXPORT
************************************************************************************/

void ADRES_EXPORT::EXPORT_ROWS()
{
    QStringList row_title ;
    QTextCodec *codec = QTextCodec::codecForName( m_text_codec );

    row_title << tr( "Firma Şahıs Adı" ) << tr( "Telefon" )<< tr( "Ülke" )
              << tr( "Şehir" )<< tr( "İlçe" )<< tr( "Adres" ) << tr("E-Posta")  ;

    SET_ROW_TITLE( row_title );

    SQL_QUERY query (DB);

    query.PREPARE_SELECT("adr_adresler","firma_sahis_adi,tel_ulke_kodu, tel_alan_kodu, telefon ,"
                                        "ulke,sehir,ilce, adres_satiri_1,adres_satiri_2,"
                                        "adres_satiri_3,email" );

    query.AND_MULTI_EKLE("adres_id",query.TO_QVARIANT(m_adres_id_list),ADAK_OR);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    while ( query.NEXT() EQ true ) {

        QStringList row_values;

        row_values << codec->toUnicode( query.VALUE ( 0 ).toByteArray() ) ;

        row_values << codec->toUnicode( query.VALUE ( 1 ).toByteArray() )+  codec->toUnicode( query.VALUE ( 2 ).toByteArray() )+  codec->toUnicode( query.VALUE ( 3 ).toByteArray() );

        row_values << codec->toUnicode( query.VALUE ( 4 ).toByteArray() );

        row_values << codec->toUnicode( query.VALUE ( 5 ).toByteArray() );

        row_values << codec->toUnicode( query.VALUE ( 6 ).toByteArray() );

        row_values << codec->toUnicode( query.VALUE ( 7 ).toByteArray() ) + codec->toUnicode( query.VALUE ( 8 ).toByteArray() ) + codec->toUnicode( query.VALUE ( 9 ).toByteArray() );

        row_values << codec->toUnicode( query.VALUE ( 10 ).toByteArray() ) ;

        ADD_ROW(row_values);
    }
}
