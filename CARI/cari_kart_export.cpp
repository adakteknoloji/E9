#include "cari_kart_export_class.h"
#include "cari_kart_export_open.h"
#include "qtextcodec.h"
#include "adres_console_utils.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
#include "cari_struct.h"

extern ADAK_SQL * DB;

void OPEN_CARI_KART_EXPORT ( QList <int> cari_hesap_id_list , QWidget * parent , QByteArray text_codec )
{
    CARI_KART_EXPORT *F = new CARI_KART_EXPORT( cari_hesap_id_list , parent , text_codec ) ;

    F->START_EXPORT();
}

CARI_KART_EXPORT::CARI_KART_EXPORT( QList <int> cari_hesap_id_list , QWidget * parent , QByteArray text_codec_name ) : EXPORT_KERNEL( "" , parent)
{
    m_cari_hesap_id_list = cari_hesap_id_list ;
    m_text_codec_name = text_codec_name ;
}

void CARI_KART_EXPORT::EXPORT_ROWS()
{
    QTextCodec *codec = QTextCodec::codecForName( m_text_codec_name );

    SET_ROW_TITLE( QStringList() << tr( "Hesap Kodu "      )
                                 << tr( "Hesap İsmi"       )
                                 << tr( "Para Birimi"      )
                                 << tr( "Toplam Borç"      )
                                 << tr( "Toplam Alacak"    )
                                 << tr( "İş Telefonu"      )
                                 << tr( "Fax"              )
                                 << tr( "Vergi Dairesi"    )
                                 << tr( "Vergi Numarası"   )
                                 << tr( "Adres"            )
                                 << tr( "Şehir"            )
                                 << tr( "İlçe"             )
                                 << tr( "Semt"             ) );


    if ( m_cari_hesap_id_list.size() <= 0 ) {
        return;
    }

    int i = 0 ;
    while ( i < m_cari_hesap_id_list.size() ) {

        QStringList row_values;
        CARI_HESAP_STRUCT hesap_bilgileri;
        BORC_BILGISI borc_bilgisi ;
        int cari_hesap_id =  m_cari_hesap_id_list.at(i) ;

        CARI_KART_BILGILERINI_OKU( cari_hesap_id , &hesap_bilgileri );

        row_values << codec->toUnicode( hesap_bilgileri.cari_hesap_kodu.toUtf8() );

        row_values << codec->toUnicode( hesap_bilgileri.cari_hesap_ismi.toUtf8() );

        int para_birimi_id = CARI_GET_PARA_BIRIM_ID( cari_hesap_id ) ;
        row_values << codec->toUnicode( DVZ_GET_DOVIZ_KODU( para_birimi_id ).toUtf8() );

        CARI_KART_BORC_BILGISINI_GETIR( cari_hesap_id , &borc_bilgisi );

        row_values << codec->toUnicode( ( QString::number( borc_bilgisi.borc_bakiye ) ).toUtf8()  );

        row_values << codec->toUnicode( ( QString::number( borc_bilgisi.alacak_bakiye ) ).toUtf8()  );

        QString telefon = hesap_bilgileri.is_tel_ulke_kodu ;
        telefon.append( hesap_bilgileri.is_tel_alan_kodu );
        telefon.append( hesap_bilgileri.is_telefonu );
        row_values << codec->toUnicode( telefon.toUtf8() );

        QString fax = hesap_bilgileri.fax_ulke_kodu;
        fax.append( hesap_bilgileri.fax_alan_kodu );
        fax.append( hesap_bilgileri.fax );
        row_values << codec->toUnicode( fax.toUtf8()  );

        row_values << codec->toUnicode( hesap_bilgileri.vergi_dairesi.toUtf8() );
        row_values << codec->toUnicode( hesap_bilgileri.vergi_no.toUtf8()      );

        QString adres = hesap_bilgileri.adres_satiri_1;
        adres.append( hesap_bilgileri.adres_satiri_2 );
        adres.append( hesap_bilgileri.adres_satiri_3 );
        row_values << codec->toUnicode( adres.toUtf8() );

        row_values << codec->toUnicode( hesap_bilgileri.sehir.toUtf8() );  // sehir
        row_values << codec->toUnicode( hesap_bilgileri.ilce.toUtf8()  );  // ilce
        row_values << codec->toUnicode( hesap_bilgileri.semt.toUtf8()  );  // semt

        ADD_ROW( row_values );
        ++i ;
    }
}
