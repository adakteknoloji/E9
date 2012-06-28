#include "adak_sql.h"
#include "adak_gruplar.h"
#include "prs_karti_export_class.h"
#include "prs_karti_export_open.h"
#include <qtextcodec.h>
#include <QList>
#include "cari_console_utils.h"
#include "cari_struct.h"
#include "prs_enum.h"
#include "dvz_console_utils.h"

extern ADAK_SQL * DB;

/***********************************************************************************
                            PRS_KARTI_EXPORT::PRS_KARTI_EXPORT
************************************************************************************/

void OPEN_PRS_KARTI_EXPORT (QList <int> cari_hesap_id_list ,QWidget *parent , QByteArray text_codec )
{
    PRS_KARTI_EXPORT * A = new PRS_KARTI_EXPORT ( cari_hesap_id_list, parent, text_codec );

    A->START_EXPORT();

}

/***********************************************************************************
                            PRS_KARTI_EXPORT::PRS_KARTI_EXPORT
************************************************************************************/

PRS_KARTI_EXPORT::PRS_KARTI_EXPORT( QList <int> cari_hesap_id_list , QWidget * parent , QByteArray text_codec ) : EXPORT_KERNEL ( "Adres-toplu-mail-smsm_disariya-kayit-ihrac-et", parent)
{
    m_cari_hesap_id_list = cari_hesap_id_list;
    m_text_codec = text_codec ;
}

/***********************************************************************************
                            PRS_KARTI_EXPORT::EXPORT_ROWS
************************************************************************************/

void PRS_KARTI_EXPORT::EXPORT_ROWS()
{
    QStringList row_title ;
    QTextCodec *codec = QTextCodec::codecForName( m_text_codec );

    row_title << tr( "TC Kimlik No" ) << tr( "Sicil No" ) << tr( "SSK No" )<< tr("Adı Soyadı") << tr("İşe Giriş Tarihi")
              << tr( "Brüt Ücret" ) << tr("Kümülatif V.M") <<tr( "Borç" ) << tr( "Alacak" ) << tr( "Para Birimi" )
              << tr( "Cinsiyeti" ) <<  tr( "Medeni Hali" ) << tr( "Şehir" ) << tr( "İlçe" ) << tr( "Semt" ) ;

    SET_ROW_TITLE( row_title );

    QStringList row_values;

    CARI_HESAP_STRUCT    PRS_HESAP;
    BORC_BILGISI         PRS_BORC;

    SQL_QUERY query (DB);

    for (int i = 0; i < m_cari_hesap_id_list.size(); ++i) {

        query.PREPARE_SELECT("prs_personeller, prs_personel_kisisel_blgler",
                             "prs_personeller.personel_id, cari_hesap_id,"
                             "sicil_no,"
                             "sigorta_no,"
                             "brut_ucret, "
                             "kumulatif_vergi_matrahi, ise_giris_tarihi",
                             "prs_personeller.personel_id = prs_personel_kisisel_blgler.personel_id AND cari_hesap_id = :cari_hesap_id");

        query.SET_VALUE( ":cari_hesap_id", m_cari_hesap_id_list.at(i) );

        if ( query.SELECT() EQ 0 ) {
            continue;
        }

        query.NEXT();

        row_values.clear();

        int cari_hesap_id = query.VALUE("cari_hesap_id").toInt();

        CARI_SET_HESAP_STRUCT_DEFAULTS( &PRS_HESAP );

        CARI_KART_BILGILERINI_OKU( cari_hesap_id,  &PRS_HESAP     );
        CARI_KART_BORC_BILGISINI_GETIR( cari_hesap_id, &PRS_BORC  );

        row_values << codec->toUnicode( PRS_HESAP.kimlik_no.toUtf8()              );
        row_values << codec->toUnicode( query.VALUE("sicil_no").toByteArray()      );
        row_values << codec->toUnicode( query.VALUE("sigorta_no").toByteArray()    );
        row_values << codec->toUnicode( PRS_HESAP.cari_hesap_ismi.toUtf8()      );
        row_values << codec->toUnicode( query.VALUE("ise_giris_tarihi").toByteArray() );
        row_values << codec->toUnicode( query.VALUE("brut_ucret").toByteArray() );
        row_values << codec->toUnicode( query.VALUE("kumulatif_vergi_matrahi").toByteArray() );
        row_values << codec->toUnicode( QString::number( PRS_BORC.borc_bakiye ).toUtf8()      );
        row_values << codec->toUnicode( QString::number( PRS_BORC.alacak_bakiye ).toUtf8()      );
        row_values << codec->toUnicode( DVZ_GET_DOVIZ_KODU( PRS_HESAP.para_birimi_id ).toUtf8() );
        row_values << codec->toUnicode( PRS_HESAP.cinsiyet.toUtf8()    );
        row_values << codec->toUnicode( PRS_HESAP.medeni_hali.toUtf8() );
        row_values << codec->toUnicode( PRS_HESAP.sehir.toUtf8() );
        row_values << codec->toUnicode( PRS_HESAP.ilce.toUtf8()  );
        row_values << codec->toUnicode( PRS_HESAP.semt.toUtf8()  );

        ADD_ROW( row_values );

    }
}
