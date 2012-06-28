#include "adak_utils.h"
#include "adres_etiketleri_class.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                    GET_ADRES_ETIKETLERI
***************************************************************************************/

QString GET_ADRES_ETIKETLERI ( QStringList P_ADRES_ETIKET_BILGILERI, etiket_sablonlari etiket_sablonu, float yazi_tipi_boyutu)
{
    ADRES_ETIKETLERI * R = new ADRES_ETIKETLERI (  etiket_sablonu, yazi_tipi_boyutu );

    return R->ETIKET_TABLOLARINI_OLUSTUR ( P_ADRES_ETIKET_BILGILERI );
}

/**************************************************************************************
                   ADRES_ETIKETLERI::ADRES_ETIKETLERI
***************************************************************************************/

ADRES_ETIKETLERI::ADRES_ETIKETLERI ( etiket_sablonlari etiket_sablonu,float yazi_tipi_boyutu )
{    
    //SET_WRAP ( NO_WRAP );
    SET_OVERFLOW_TAG( "hidden" );

    SET_ETIKET_SIZE             ( etiket_sablonu.etiket_width,etiket_sablonu.etiket_height );
    SET_ETIKET_MARGIN           ( etiket_sablonu.margin_right,etiket_sablonu.margin_bottom );
    SET_PAGE_SPACE              ( etiket_sablonu.page_top_space,etiket_sablonu.page_left_space );
    SET_SATIRDAKI_ETIKET_SAYISI ( etiket_sablonu.satirdaki_etiket_sayisi );
    SET_SAYFADAKI_SATIR_SAYISI  ( etiket_sablonu.sayfadaki_satir_sayisi );

    m_yazitipi_boyutu  = yazi_tipi_boyutu;
    m_etiket_genisligi = etiket_sablonu.etiket_width;
}

/**************************************************************************************
                   ADRES_ETIKETLERI::ETIKET_TABLOLARINI_OLUSTUR
***************************************************************************************/

QString ADRES_ETIKETLERI::ETIKET_TABLOLARINI_OLUSTUR ( QStringList P_ADRES_ETIKET_BILGILERI )
{
    QStringList ETIKETLER;

    ETIKET_OLUSTUR ( P_ADRES_ETIKET_BILGILERI, &ETIKETLER );

    return ETIKETLERI_OLUSTUR ( ETIKETLER );
}

/**************************************************************************************
                   ADRES_ETIKETLERI::ETIKET_OLUSTUR
***************************************************************************************/

void ADRES_ETIKETLERI::ETIKET_OLUSTUR ( QStringList P_ADRES_ETIKET_BILGILERI, QStringList * ETIKETLER )
{

    QStringList ETIKET_BILGILERI;

    QString adres_satiri_1;
    QString adres_satiri_2;
    QString adres_satiri_3;

    for ( int i = 0; i < P_ADRES_ETIKET_BILGILERI.size(); i++ ) {

        ETIKET_BILGILERI = P_ADRES_ETIKET_BILGILERI.at(i).split( "\t" );

        adres_satiri_1.clear();
        adres_satiri_2.clear();
        adres_satiri_3.clear();

        if ( ETIKET_BILGILERI.at(1).isEmpty() EQ false ) {
            adres_satiri_1 = ETIKET_BILGILERI.at(1);
        }

        if ( ETIKET_BILGILERI.at(2).isEmpty() EQ false ) {
            adres_satiri_2 = ETIKET_BILGILERI.at(2);
        }

        if ( ETIKET_BILGILERI.at(3).isEmpty() EQ false ) {
            adres_satiri_3 = ETIKET_BILGILERI.at(3);
        }

        double table_width  = ( ( m_etiket_genisligi - 3.0 )/ 0.35277777 )*1.4;
        double table_height = ( ( 10 - 3.0 )/ 0.35277777 )*1.4;

        QString str = ETIKET_BILGILERI.at(0);

        str.append ( "<br>" );
        str.append ( adres_satiri_1 );
        if ( adres_satiri_1.size() + adres_satiri_2.size() > 35 ) {
            str.append ( "<br>" );
        }
        if ( adres_satiri_2.isEmpty() EQ false ) {
            str.append ( adres_satiri_2 );
            str.append ( "<br>" );
        }
        if ( adres_satiri_3.isEmpty() EQ false ) {
            str.append ( adres_satiri_3 );
            str.append ( "<br>" );
        }

        str.append ( ETIKET_BILGILERI.at(4) + " - " + ETIKET_BILGILERI.at(5) + " " + ETIKET_BILGILERI.at(6)  + " " + ETIKET_BILGILERI.at(7) );

        SET_TABLE_TAG_STRING    ( QString ( "<table border = 0 style=\"font-size:%1pt; width:%2pt; height:%3pt;\">" ).arg ( m_yazitipi_boyutu ).arg ( QVariant ( table_width ).toString()).arg ( QVariant ( table_height ).toString() ) );
        CREATE_TABLE            ( QStringList() << str ,0,0);

        *ETIKETLER  << GET_TABLE_HTML_STRING();
    }
}

