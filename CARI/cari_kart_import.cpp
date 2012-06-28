#include "adak_sql.h"
#include "adak_gruplar.h"
#include "cari_kart_import_class.h"
#include "cari_kart_import_open.h"
#include "adres_console_utils.h"
#include "e9_enum.h"
#include "dvz_console_utils.h"
#include <QStringList>
#include "adak_utils.h"
#include "cari_console_utils.h"
#include "cari_struct.h"
#include "adres_enum.h"
#include "cari_enum.h"
#include "sube_console_utils.h"

extern ADAK_SQL * DB;

void OPEN_CARI_KART_IMPORT( QWidget *parent )
{
    CARI_KART_IMPORT *F = new CARI_KART_IMPORT( parent );
    F->EXEC( NOT_FULL_SCREEN );
}


/***********************************************************************************
                            CARI_KART_IMPORT::CARI_KART_IMPORT
************************************************************************************/

CARI_KART_IMPORT::CARI_KART_IMPORT ( QWidget *parent ) : IMPORT_KERNEL( "" , parent )
{
    QList<QStringList> baslik_ve_kontrol;

    baslik_ve_kontrol.append( QStringList() <<  "Hesap Kodu"    << "10" << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Hesap İsmi"    << "60" << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Para Birimi"   << ""   << "TL");
    baslik_ve_kontrol.append( QStringList() <<  "Toplam Borç"   << ""   << "");
    baslik_ve_kontrol.append( QStringList() <<  "Toplam Alacak" << ""   << "");
    baslik_ve_kontrol.append( QStringList() <<  "İş Telefonu"   << "10" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Fax"           << "10" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Vergi Dairesi" << "50" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Vergi Numarası"<< "15" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Adres"         << "35" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Şehir"         << "30" << "");
    baslik_ve_kontrol.append( QStringList() <<  "İlçe"          << "30" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Semt"          << "30" << "");

    SET_HEADERS    ( baslik_ve_kontrol );

    SET_PROGRAM_ID      ( E9_PROGRAMI   );
    SET_MODUL_ID        ( CARI_MODULU  );
    m_fis_id = -1 ;
    CARI_FIS = NULL ;
}

/***********************************************************************************
                            CARI_KART_IMPORT::IMPORT_ROW
************************************************************************************/

void CARI_KART_IMPORT::IMPORT_ROW( QStringList kayit_bilgisi_list  )
{
    if ( kayit_bilgisi_list.size() <= 0 ) {
        return;
    }

    QString firma_sahis_kodu = kayit_bilgisi_list.at(0).mid(0, 60) ;

    SQL_QUERY query_select( DB );

    query_select.PREPARE_SELECT( "car_hesaplar" , "cari_hesap_id" , "cari_hesap_kodu=:cari_hesap_kodu");
    query_select.SET_VALUE( ":cari_hesap_kodu"  , firma_sahis_kodu );

    if( query_select.SELECT() NE 0 ){
        return ;
    }

    int sube_id = SUBE_GET_SUBE_ID();

    double cari_borc   = kayit_bilgisi_list.at(3).toDouble();
    double cari_alacak = kayit_bilgisi_list.at(4).toDouble();

    CARI_HESAP_STRUCT         CARI_KART  ;
    CARI_FIS_SATIRI_STRUCT    CARI_FIS_SATIRI ;

    CARI_SET_HESAP_STRUCT_DEFAULTS( &CARI_KART );

    CARI_KART.cari_hesap_kodu =  kayit_bilgisi_list.at( 0 );
    CARI_KART.cari_hesap_ismi =  kayit_bilgisi_list.at( 1 ).mid(0, 60);
    CARI_KART.para_birimi_id  =  DVZ_GET_DOVIZ_ID( kayit_bilgisi_list.at(2) ) ;

    QString tum_telefon = kayit_bilgisi_list.at( 5 );
    QString telefon   = "";
    QString alan_kodu = "";
    QString ulke_kodu = "";

    TELEFON_BILGISINI_PARSE_ET( ulke_kodu, alan_kodu, telefon, tum_telefon );

    CARI_KART.is_tel_ulke_kodu =  ulke_kodu;
    CARI_KART.is_tel_alan_kodu =  alan_kodu;
    CARI_KART.is_telefonu      =  telefon;

    QString tum_fax = kayit_bilgisi_list.at( 6 );
    QString fax   = "";
    QString fax_alan_kodu = "";
    QString fax_ulke_kodu = "";

    TELEFON_BILGISINI_PARSE_ET( fax_ulke_kodu, fax_alan_kodu, fax, tum_fax );

    CARI_KART.fax_ulke_kodu    =  fax_ulke_kodu;
    CARI_KART.fax_alan_kodu    =  fax_alan_kodu;
    CARI_KART.fax              =  fax;

    CARI_KART.vergi_dairesi    =  kayit_bilgisi_list.at( 7 );
    CARI_KART.vergi_no         =  kayit_bilgisi_list.at( 8 );

    QString tum_adres = kayit_bilgisi_list.at( 9 );
    QString adres_satiri_1 = "";
    QString adres_satiri_2 = "";
    QString adres_satiri_3 = "";

    ADRES_BILGISINI_PARSE_ET( adres_satiri_1, adres_satiri_2, adres_satiri_3, tum_adres );

    CARI_KART.adres_satiri_1  =  adres_satiri_1;
    CARI_KART.adres_satiri_2  =  adres_satiri_2;
    CARI_KART.adres_satiri_3  =  adres_satiri_3;

    CARI_KART.sehir           =  kayit_bilgisi_list.at( 10 );
    CARI_KART.ilce            =  kayit_bilgisi_list.at( 11 );
    CARI_KART.semt            =  kayit_bilgisi_list.at( 12 );

    CARI_KART.program_id      =  E9_PROGRAMI ;
    CARI_KART.modul_id        =  CARI_MODULU ;
    CARI_KART.cari_kart_turu  =  FIRMA_KAYDI;
    CARI_KART.cari_kart_tipi  =  ENUM_CARI_ALICI;
    CARI_KART.muh_hesap_id    =  SUBE_GET_SUBE_ENT_HESAP_ID(sube_id, "cari_alici_ent_hesap_id");

    int cari_hesap_id = CARI_KART_OLUSTUR( FIRMA_KAYDI, &CARI_KART ) ;

    if( m_fis_id EQ -1 ){

        CARI_FIS =  new CARI_FIS_STRUCT ;

        CARI_CLEAR_FIS_STRUCT( CARI_FIS );

        CARI_FIS->cari_hesap_id        =  cari_hesap_id ;
        CARI_FIS->program_id           =  E9_PROGRAMI ;
        CARI_FIS->modul_id             =  CARI_MODULU ;
        CARI_FIS->fis_turu             =  ENUM_CARI_ACILIS_FISI ;
        CARI_FIS->fis_no               =  CARI_SIRADAKI_FIS_NO_AL( MALI_YIL_FIRST_DATE().toString("yyyy.MM.dd") ) ;
        CARI_FIS->cari_para_birimi_id  =  CARI_KART.para_birimi_id ;
        CARI_FIS->fis_tarihi           =  MALI_YIL_FIRST_DATE().toString("yyyy.MM.dd");
        CARI_FIS->aciklama             =  tr("CARİ AÇILIŞ FİŞİ - İmport İşlemi Esnasında Üretilmiştir.");
        CARI_FIS->base_fis_id          =  0;

        m_fis_id = CARI_FIS_EKLE( CARI_FIS ) ;
    }

    CARI_CLEAR_FIS_SATIRI_STRUCT( &CARI_FIS_SATIRI );

    CARI_FIS_SATIRI.cari_hesap_id         =  cari_hesap_id ;
    CARI_FIS_SATIRI.fis_id                =  m_fis_id ;
    CARI_FIS_SATIRI.program_id            =  E9_PROGRAMI ;
    CARI_FIS_SATIRI.modul_id              =  CARI_MODULU ;
    CARI_FIS_SATIRI.doviz_id              =  CARI_KART.para_birimi_id;

    //! IMPORT ISLEMINDE SADECE BIRI ALINIR
    if ( cari_borc >= cari_alacak ) {
        cari_borc -= cari_alacak;
        cari_alacak = 0.00;
    }
    else {
        cari_alacak -= cari_borc;
        cari_borc    = 0.00;
    }

    if( cari_borc > 0 ){
        CARI_FIS_SATIRI.cari_para_birimi_tutari = cari_borc;
        CARI_FIS_SATIRI.alacak_tutari           = 0;
        CARI_FIS_SATIRI.borc_tutari             = cari_borc;

        CARI_FIS_SATIRI_EKLE( CARI_FIS , &CARI_FIS_SATIRI );
    }
    if ( cari_alacak > 0 )  {
        CARI_FIS_SATIRI.borc_tutari              = 0 ;
        CARI_FIS_SATIRI.alacak_tutari            = cari_alacak;
        CARI_FIS_SATIRI.cari_para_birimi_tutari  = cari_alacak;

        CARI_FIS_SATIRI_EKLE( CARI_FIS , &CARI_FIS_SATIRI );
    }

    if ( GET_GRUP_IDLERI()->size() > 0 ) {
        GRP_KAYDI_GRUPLARA_EKLE( E9_PROGRAMI, CARI_MODULU, cari_hesap_id, GET_GRUP_IDLERI() );
    }
}

void CARI_KART_IMPORT::TELEFON_BILGISINI_PARSE_ET(QString &ulke_kodu, QString &alan_kodu, QString &telefon, QString &tum_telefon )
{
    tum_telefon = tum_telefon.replace(" ","",Qt::CaseInsensitive);

    if ( tum_telefon.size() >= 7 ) {
        telefon = tum_telefon.right(7);
        tum_telefon.chop(7);
    }
    if ( tum_telefon.size() >= 3 ) {
        alan_kodu = tum_telefon.right(3);
        tum_telefon.chop(3);
    }
    if ( tum_telefon.size() > 0 ) {
        ulke_kodu = tum_telefon;
    }
}

void CARI_KART_IMPORT::ADRES_BILGISINI_PARSE_ET(QString &adres_satiri_1, QString &adres_satiri_2, QString &adres_satiri_3, QString &tum_adres )
{
    adres_satiri_1 = tum_adres.left(34);
    tum_adres.remove(0,34);

    if (tum_adres.size() > 0) {
        adres_satiri_2 = tum_adres.left(34);
        tum_adres.remove(0,34);
    }
    if (tum_adres.size() > 0) {
        adres_satiri_3 = tum_adres.left(34);
    }

}
