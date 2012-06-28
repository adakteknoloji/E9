#include <QString>

#include "adak_sql.h"
#include "adak_gruplar.h"

#include "adres_import_class.h"
#include "adres_import_open.h"

extern ADAK_SQL * DB;

/***********************************************************************************
                                 OPEN_ADRES_IMPORT
************************************************************************************/

void OPEN_ADRES_IMPORT (QWidget * parent)
{
    ADRES_IMPORT * A = new ADRES_IMPORT(parent);
    A->EXEC( NOT_FULL_SCREEN );
}

/***********************************************************************************
                            ADRES_IMPORT::ADRES_IMPORT
************************************************************************************/

ADRES_IMPORT::ADRES_IMPORT(QWidget * parent) : IMPORT_KERNEL ( "adres-toplu-mail-sms_iceriye-kayit-al ", parent)
{

    QList<QStringList> baslik_ve_kontrol;

    baslik_ve_kontrol.append( QStringList() <<  "Şahış/Firma Adı" << "60" << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Telefon"         << "10" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Ülke"            << "30" << "");
    baslik_ve_kontrol.append( QStringList() <<  "İl"              << "30" << "");
    baslik_ve_kontrol.append( QStringList() <<  "İlçe"            << "30" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Adres"           << "35" << "");
    baslik_ve_kontrol.append( QStringList() <<  "E-mail"          << "50" << "");

    SET_HEADERS    ( baslik_ve_kontrol );

    SET_PROGRAM_ID ( E9_PROGRAMI   );
    SET_MODUL_ID   ( ADRES_MODULU  );
}

/***********************************************************************************
                            ADRES_IMPORT::IMPORT_ROW
************************************************************************************/

void ADRES_IMPORT::IMPORT_ROW( QStringList kayit_bilgisi_list )
{
    if ( kayit_bilgisi_list.size() <= 0 ) {
        return;
    }

    QString firma_sahis_adi = kayit_bilgisi_list.at(0).mid(0, 60) ;

    SQL_QUERY query (DB);

    query.PREPARE_SELECT( "adr_adresler" , "adres_id" , "firma_sahis_adi =:firma_sahis_adi " );
    query.SET_VALUE( ":firma_sahis_adi" , firma_sahis_adi);

    if( query.SELECT() NE 0 ){
        return;
    }

    QString tel            = "";
    QString tel_alan       = "";
    QString tel_ulke       = "";
    QString adres_satiri_1 = "";
    QString adres_satiri_2 = "";
    QString adres_satiri_3 = "";

    TELEFON_BILGINI_PARSE_ET( kayit_bilgisi_list.at(1), &tel_ulke, &tel_alan, &tel );

    QString tum_adres = kayit_bilgisi_list.at(5);
    adres_satiri_1 = tum_adres.left(34);
    tum_adres.remove(0,34);

    if (tum_adres.size() > 0) {
        adres_satiri_2 = tum_adres.left(34);
        tum_adres.remove(0,34);
    }
    if (tum_adres.size() > 0) {
        adres_satiri_3 = tum_adres.left(34);
    }

    query.PREPARE_INSERT("adr_adresler","adres_id",

                         "adres_kart_turu,  "
                         "firma_sahis_adi,  "
                         "ilce,             "
                         "sehir,            "
                         "ulke,             "
                         "adres_satiri_1,   "
                         "adres_satiri_2,   "
                         "adres_satiri_3,   "
                         "tel_ulke_kodu,    "
                         "tel_alan_kodu,    "
                         "telefon,          "
                         "modul_id,         "
                         "program_id,       "
                         "email             ");

    query.SET_VALUE(":adres_kart_turu", FIRMA_KAYDI                 );
    query.SET_VALUE(":modul_id"       , ADRES_MODULU                );
    query.SET_VALUE(":program_id"     , E9_PROGRAMI                 );
    query.SET_VALUE(":firma_sahis_adi", firma_sahis_adi             );
    query.SET_VALUE(":ulke"           , kayit_bilgisi_list.at(2)    );
    query.SET_VALUE(":sehir"          , kayit_bilgisi_list.at(3)    );
    query.SET_VALUE(":ilce"           , kayit_bilgisi_list.at(4)    );
    query.SET_VALUE(":adres_satiri_1" , adres_satiri_1              );
    query.SET_VALUE(":adres_satiri_2" , adres_satiri_2              );
    query.SET_VALUE(":adres_satiri_3" , adres_satiri_3              );
    query.SET_VALUE(":email"          , kayit_bilgisi_list.at(6)    );
    query.SET_VALUE(":tel_ulke_kodu"  , tel_ulke                    );
    query.SET_VALUE(":tel_alan_kodu"  , tel_alan                    );
    query.SET_VALUE(":telefon"        , tel                         );

    int id = query.INSERT();

    if ( GET_GRUP_IDLERI()->size() > 0 ) {
        GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI, ADRES_MODULU, id, GET_GRUP_IDLERI() );
    }

}

/***********************************************************************************
                     ADRES_EXPORT::TELEFON_BILGINI_PARSE_ET
************************************************************************************/

void ADRES_IMPORT::TELEFON_BILGINI_PARSE_ET(QString p_tum_telefon, QString *p_ulke_kodu, QString *p_alan_kodu, QString *p_telefon)
{
    p_tum_telefon = p_tum_telefon.replace(" ","",Qt::CaseInsensitive);

    if ( p_tum_telefon.size() >= 7 ) {
        *p_telefon = p_tum_telefon.right(7);
        p_tum_telefon.chop(7);
    }
    if ( p_tum_telefon.size() >= 3 ) {
        *p_alan_kodu = p_tum_telefon.right(3);
        p_tum_telefon.chop(3);
    }
    if ( p_tum_telefon.size() > 0 ) {
        *p_ulke_kodu = p_tum_telefon;
    }
}
