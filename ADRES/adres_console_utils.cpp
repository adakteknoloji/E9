#include <QComboBox>
#include <QDate>
#include "adres_console_utils.h"
#include "yonetim.h"
#include "e9_yetki.h"
#include "e9_log.h"
#include "adres_enum.h"
#include "adres_struct.h"

extern      ADAK_SQL       *  DB;
extern      ADAK_SQL       *  G_YONETIM_DB;


/**************************************************************************************
                   ADR_GET_USER_MAIL
***************************************************************************************/

QString ADR_GET_USER_MAIL()
{
    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT( "ynt_kullanicilar","email_adresi","kullanici_id = :kullanici_id AND silinmis_mi = :silinmis_mi" ) ;

    sql_query.SET_VALUE( ":silinmis_mi",0);
    sql_query.SET_VALUE( ":kullanici_id", KULLANICI_ID() );

    if ( sql_query.SELECT() EQ 0 ) {
        return "";
    }

    sql_query.NEXT();

    return sql_query.VALUE ( 0 ).toString();
}


/**************************************************************************************
                   ADR_GET_ADDRES_GRUP_ID
***************************************************************************************/

int ADR_GET_ADDRESS_GRUP_ID ( QString grup_adi )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("adak_gruplar","grup_id","grup_adi = :grup_adi");

    sql_query.SET_VALUE( ":grup_adi", grup_adi );

    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE( 0 ).toInt();
}

/**************************************************************************************
                   ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR
***************************************************************************************/

int ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR ( QString p_tel_ulke_kodu, QString p_tel_alan_kodu, QString p_telefon, int p_kayit_id, QString p_tel_dahili )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_INSERT  ( "adr_telefonlar","telefon_id", "adres_kayit_id, order_number, "
                                "telefon_ulke_kodu, telefon_alan_kodu, telefon, telefon_dahili, gizli_kayit");

    sql_query.SET_VALUE ( ":adres_kayit_id"   ,    p_kayit_id      );
    sql_query.SET_VALUE ( ":order_number",    0                    );
    sql_query.SET_VALUE ( ":telefon_ulke_kodu",    p_tel_ulke_kodu );
    sql_query.SET_VALUE ( ":telefon_alan_kodu",    p_tel_alan_kodu );
    sql_query.SET_VALUE ( ":telefon",              p_telefon       );
    sql_query.SET_VALUE ( ":telefon_dahili"   ,    p_tel_dahili    );
    sql_query.SET_VALUE ( ":gizli_kayit"      ,    1               );

    return sql_query.INSERT();
}


/**************************************************************************************
                   ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE
***************************************************************************************/

void ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE  ( QString p_tel_ulke_kodu, QString p_tel_alan_kodu, QString p_telefon, int p_telefon_id, QString p_tel_dahili )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_UPDATE ( "adr_telefonlar","telefon_id","telefon_ulke_kodu,telefon_alan_kodu,telefon,"
                               "telefon_dahili","telefon_id = :telefon_id" );

    sql_query.SET_VALUE ( ":telefon_ulke_kodu", p_tel_ulke_kodu );
    sql_query.SET_VALUE ( ":telefon_alan_kodu", p_tel_alan_kodu );
    sql_query.SET_VALUE ( ":telefon",           p_telefon       );
    sql_query.SET_VALUE ( ":telefon_dahili",    p_tel_dahili    );
    sql_query.SET_VALUE ( ":telefon_id",        p_telefon_id    );

    sql_query.UPDATE();

}

/**************************************************************************************
                   ADR_EMAIL_FISINDE_GIZLI_KAYIT_OLUSTUR
***************************************************************************************/

int ADR_EMAIL_FISINDE_GIZLI_KAYIT_OLUSTUR ( QString p_email_adresi, int p_kayit_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_INSERT ( "adr_emailler","email_id", "order_number, adres_kayit_id, "
                               "email_adresi, gizli_kayit");

    sql_query.SET_VALUE ( ":order_number"    ,   0              );
    sql_query.SET_VALUE ( ":adres_kayit_id"  ,   p_kayit_id     );
    sql_query.SET_VALUE ( ":email_adresi"    ,   p_email_adresi );
    sql_query.SET_VALUE ( ":gizli_kayit"     ,   1 );

    return sql_query.INSERT();
}


/**************************************************************************************
                   ADR_EMAIL_FISINDEKI_GIZLI_KAYDI_GUNCELLE
***************************************************************************************/

void ADR_EMAIL_FISINDEKI_GIZLI_KAYDI_GUNCELLE ( QString p_email_adresi, int p_email_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_UPDATE ( "adr_emailler","email_id","email_adresi","email_id = :email_id");

    sql_query.SET_VALUE     ( ":email_adresi", p_email_adresi );
    sql_query.SET_VALUE     ( ":email_id"    , p_email_id     );

    sql_query.UPDATE();
}



/**************************************************************************************
                   ADR_GET_TAM_GRUP_KODU
***************************************************************************************/

QString ADR_GET_TAM_GRUP_KODU ( int grup_id )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "adak_gruplar","tam_grup_kodu","grup_id = :grup_id" );
    select_query.SET_VALUE      ( ":grup_id" , grup_id );

    if ( select_query.SELECT() EQ 0 ) {
        return NULL;

    }
    select_query.NEXT();
    return select_query.VALUE( 0 ).toString();
}


/**************************************************************************************
                   ADR_GET_GRUP_ISMI
***************************************************************************************/

QString ADR_GET_GRUP_ISMI ( int grup_id )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "adak_gruplar","grup_adi","grup_id = :grup_id" );
    select_query.SET_VALUE      ( ":grup_id" , grup_id );

    if ( select_query.SELECT() EQ 0 ) {
        return NULL;

    }
    select_query.NEXT();
    return select_query.VALUE( 0 ).toString();
}


/**************************************************************************************
                   ADR_GET_GRUP_ID
***************************************************************************************/

int ADR_GET_GRUP_ID ( QString tam_grup_kodu )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "adak_gruplar","grup_id","tam_grup_kodu = :tam_grup_kodu" );
    select_query.SET_VALUE      ( ":grup_id" , tam_grup_kodu );

    if (select_query.SELECT() EQ 0 ) {
        return 0;

    }
    select_query.NEXT();
    return select_query.VALUE( 0 ).toInt();
}



/**************************************************************************************
                    ADR_GET_ADR_LOG_TURLERI_STRING
***************************************************************************************/

QString ADR_GET_ADR_LOG_TURLERI_STRING ( int p_log_turu_enum )
{

    switch ( p_log_turu_enum ) {

        case LOG_ADR_SAHIS_KAYDI :
            return QObject::tr ( "Şahıs Kaydı" );

        case LOG_ADR_FIRMA_KAYDI :
            return QObject::tr ( "Firma Kaydı" );

        case LOG_ADR_ADRES_GRUP_EKLEME :
            return QObject::tr ( "Adres Grup Ekleme" );

        case LOG_ADR_ADRES_GRUPLARI :
            return QObject::tr ( "Adres Grupları" );

        case LOG_ADR_FIRMA_YETKILI_KAYDI :
            return QObject::tr ( "Firma Yetkili Kaydı" );

        case LOG_ADR_YAKIN_BILGILERI_KAYDI :
            return QObject::tr ( "Yakın Bilgileri Kaydı" );

        case LOG_ADR_KULLANICI_YETKILERI :
            return QObject::tr ( "Kullanıcı Yetkileri" );

        case LOG_ADR_ETIKET_YAZDIRMA :
            return QObject::tr ( "Etiket Yazdırma" );

        case LOG_ADR_MAIL_GONDERIMI :
            return QObject::tr ( "Mail Gönderimi" );

        default :
            return "";
    }
}
/**************************************************************************************
                   ADR_GET_KART_TURU_ENUM
***************************************************************************************/

int ADR_GET_KART_TURU_ENUM  ( QString kart_turu )
{
    if (kart_turu EQ "Firma Kartı") {
        return FIRMA_KAYDI;
    }
    else if (kart_turu EQ "Şahıs Kartı"){
        return SAHIS_KAYDI;
    }

    return -1;
}

/**************************************************************************************
                   ADR_GET_KART_TURU_STRING
***************************************************************************************/

QString ADR_GET_KART_TURU_STRING ( int kart_turu )
{
    switch(kart_turu) {
        case FIRMA_KAYDI:
            return "Firma Kartı";
        case SAHIS_KAYDI:
            return "Şahıs Kartı";
    }

    return "";
}



/**************************************************************************************
                ADR_CLEAR_ADRES_STRUCT
***************************************************************************************/

void ADR_CLEAR_ADRES_STRUCT( ADRES_HESAP_STRUCT * P_ADR_STRUCT )
{
    P_ADR_STRUCT->adres_kart_turu            = -1;
    P_ADR_STRUCT->firma_id                   = -1;
    P_ADR_STRUCT->telefon_gizli_kayit_id     = -1;
    P_ADR_STRUCT->is_tel_gizli_kayit_id      = -1;
    P_ADR_STRUCT->cep_tel_gizli_kayit_id     = -1;
    P_ADR_STRUCT->fax_gizli_kayit_id         = -1;
    P_ADR_STRUCT->email_gizli_kayit_id       = -1;

    P_ADR_STRUCT->firma_sahis_adi.clear();
    P_ADR_STRUCT->dogum_tarihi.clear();
    P_ADR_STRUCT->dogum_yeri.clear();
    P_ADR_STRUCT->cinsiyet.clear();
    P_ADR_STRUCT->ogrenim_durumu.clear();
    P_ADR_STRUCT->meslek.clear();
    P_ADR_STRUCT->unvan.clear();
    P_ADR_STRUCT->kimlik_no.clear();
    P_ADR_STRUCT->kan_grubu.clear();
    P_ADR_STRUCT->medeni_hali.clear();
    P_ADR_STRUCT->vergi_dairesi.clear();
    P_ADR_STRUCT->vergi_no.clear();
    P_ADR_STRUCT->adres_satiri_1.clear();
    P_ADR_STRUCT->adres_satiri_2.clear();
    P_ADR_STRUCT->adres_satiri_3.clear();
    P_ADR_STRUCT->ilce.clear();
    P_ADR_STRUCT->semt.clear();
    P_ADR_STRUCT->ulke.clear();
    P_ADR_STRUCT->sehir.clear();
    P_ADR_STRUCT->posta_kodu.clear();
    P_ADR_STRUCT->tel_ulke_kodu.clear();
    P_ADR_STRUCT->tel_alan_kodu.clear();
    P_ADR_STRUCT->telefon.clear();
    P_ADR_STRUCT->is_tel_ulke_kodu.clear();
    P_ADR_STRUCT->is_tel_alan_kodu.clear();
    P_ADR_STRUCT->is_telefonu.clear();
    P_ADR_STRUCT->cep_tel_ulke_kodu.clear();
    P_ADR_STRUCT->cep_tel_gsm_kodu.clear();
    P_ADR_STRUCT->cep_telefonu.clear();
    P_ADR_STRUCT->fax_ulke_kodu.clear();
    P_ADR_STRUCT->fax_alan_kodu.clear();
    P_ADR_STRUCT->fax.clear();
    P_ADR_STRUCT->email.clear();
    P_ADR_STRUCT->website.clear();
    P_ADR_STRUCT->firma_sektoru.clear();
    P_ADR_STRUCT->is_telefonu_dahili.clear();

    P_ADR_STRUCT->yetkili_1.clear();
    P_ADR_STRUCT->yetkili_1_info.clear();
    P_ADR_STRUCT->yetkili_2.clear();
    P_ADR_STRUCT->yetkili_2_info.clear();
    P_ADR_STRUCT->yetkili_3.clear();
    P_ADR_STRUCT->yetkili_3_info.clear();

    P_ADR_STRUCT->fotograf_logo.clear();
    P_ADR_STRUCT->program_id                 =  -1;
    P_ADR_STRUCT->modul_id                   =  -1;
}

/**************************************************************************************
                   ADR_KART_OLUSTUR
***************************************************************************************/

int ADR_KART_OLUSTUR( ADRES_HESAP_STRUCT * P_ADR_STRUCT )
{

    SQL_QUERY sql_query( DB );
    sql_query.PREPARE_INSERT ( "adr_adresler","adres_id",
                               "firma_sahis_adi, dogum_tarihi, dogum_yeri, cinsiyet,"
                               "ogrenim_durumu, medeni_hali, kan_grubu, meslek, unvan, kimlik_no,"
                               "vergi_dairesi, vergi_no, adres_satiri_1, adres_satiri_2, adres_satiri_3, ilce,"
                               "semt, ulke, sehir, posta_kodu, tel_ulke_kodu, tel_alan_kodu, telefon,"
                               "is_tel_ulke_kodu, is_tel_alan_kodu, is_telefonu, cep_tel_ulke_kodu, "
                               "cep_tel_gsm_kodu, cep_telefonu, fax_ulke_kodu, fax_alan_kodu, fax, email, "
                               "firma_id, modul_id, program_id, is_telefonu_dahili, "
                               "kaydin_olusturulma_tarihi, adres_kart_turu, website, "
                               "fotograf_logo, base_record_id, firma_sektoru, "
                               "yetkili_1, yetkili_1_info,  "
                               "yetkili_2, yetkili_2_info,  "
                               "yetkili_3, yetkili_3_info  " );

    sql_query.SET_VALUE ( ":firma_sahis_adi",            P_ADR_STRUCT->firma_sahis_adi              );
    sql_query.SET_VALUE ( ":dogum_tarihi",               P_ADR_STRUCT->dogum_tarihi                 );
    sql_query.SET_VALUE ( ":dogum_yeri",                 P_ADR_STRUCT->dogum_yeri                   );
    sql_query.SET_VALUE ( ":cinsiyet",                   P_ADR_STRUCT->cinsiyet                     );
    sql_query.SET_VALUE ( ":ogrenim_durumu",             P_ADR_STRUCT->ogrenim_durumu               );
    sql_query.SET_VALUE ( ":medeni_hali",                P_ADR_STRUCT->medeni_hali                  );
    sql_query.SET_VALUE ( ":kan_grubu",                  P_ADR_STRUCT->kan_grubu                    );
    sql_query.SET_VALUE ( ":meslek",                     P_ADR_STRUCT->meslek                       );
    sql_query.SET_VALUE ( ":unvan",                      P_ADR_STRUCT->unvan                        );
    sql_query.SET_VALUE ( ":kimlik_no",                  P_ADR_STRUCT->kimlik_no                    );
    sql_query.SET_VALUE ( ":vergi_dairesi",              P_ADR_STRUCT->vergi_dairesi                );
    sql_query.SET_VALUE ( ":vergi_no",                   P_ADR_STRUCT->vergi_no                     );
    sql_query.SET_VALUE ( ":adres_satiri_1",             P_ADR_STRUCT->adres_satiri_1               );
    sql_query.SET_VALUE ( ":adres_satiri_2",             P_ADR_STRUCT->adres_satiri_2               );
    sql_query.SET_VALUE ( ":adres_satiri_3",             P_ADR_STRUCT->adres_satiri_3               );
    sql_query.SET_VALUE ( ":ilce",                       P_ADR_STRUCT->ilce                         );
    sql_query.SET_VALUE ( ":semt",                       P_ADR_STRUCT->semt                         );
    sql_query.SET_VALUE ( ":ulke",                       P_ADR_STRUCT->ulke                         );
    sql_query.SET_VALUE ( ":sehir",                      P_ADR_STRUCT->sehir                        );
    sql_query.SET_VALUE ( ":posta_kodu",                 P_ADR_STRUCT->posta_kodu                   );
    sql_query.SET_VALUE ( ":tel_ulke_kodu",              P_ADR_STRUCT->tel_ulke_kodu                );
    sql_query.SET_VALUE ( ":tel_alan_kodu",              P_ADR_STRUCT->tel_alan_kodu                );
    sql_query.SET_VALUE ( ":telefon",                    P_ADR_STRUCT->telefon                      );
    sql_query.SET_VALUE ( ":is_tel_ulke_kodu",           P_ADR_STRUCT->is_tel_ulke_kodu             );
    sql_query.SET_VALUE ( ":is_tel_alan_kodu",           P_ADR_STRUCT->is_tel_alan_kodu             );
    sql_query.SET_VALUE ( ":is_telefonu",                P_ADR_STRUCT->is_telefonu                  );
    sql_query.SET_VALUE ( ":cep_tel_ulke_kodu",          P_ADR_STRUCT->cep_tel_ulke_kodu            );
    sql_query.SET_VALUE ( ":cep_tel_gsm_kodu",           P_ADR_STRUCT->cep_tel_gsm_kodu             );
    sql_query.SET_VALUE ( ":cep_telefonu",               P_ADR_STRUCT->cep_telefonu                 );
    sql_query.SET_VALUE ( ":fax_ulke_kodu",              P_ADR_STRUCT->fax_ulke_kodu                );
    sql_query.SET_VALUE ( ":fax_alan_kodu",              P_ADR_STRUCT->fax_alan_kodu                );
    sql_query.SET_VALUE ( ":fax",                        P_ADR_STRUCT->fax                          );
    sql_query.SET_VALUE ( ":email",                      P_ADR_STRUCT->email                        );
    sql_query.SET_VALUE ( ":firma_id",                   0                                          );
    sql_query.SET_VALUE ( ":modul_id",                   P_ADR_STRUCT->modul_id                     );
    sql_query.SET_VALUE ( ":program_id",                 P_ADR_STRUCT->program_id                   );
    sql_query.SET_VALUE ( ":is_telefonu_dahili",         P_ADR_STRUCT->is_telefonu_dahili           );
    sql_query.SET_VALUE ( ":kaydin_olusturulma_tarihi",  QDate::currentDate().toString("yyyy.MM.dd"));
    sql_query.SET_VALUE ( ":adres_kart_turu",            P_ADR_STRUCT->adres_kart_turu              );
    sql_query.SET_VALUE ( ":website",                    P_ADR_STRUCT->website                      );
    sql_query.SET_VALUE ( ":base_record_id",             P_ADR_STRUCT->base_record_id               );
    sql_query.SET_VALUE ( ":firma_sektoru",              P_ADR_STRUCT->firma_sektoru                );
    sql_query.SET_VALUE ( ":yetkili_1",                  P_ADR_STRUCT->yetkili_1                    );
    sql_query.SET_VALUE ( ":yetkili_1_info",             P_ADR_STRUCT->yetkili_1_info               );
    sql_query.SET_VALUE ( ":yetkili_2",                  P_ADR_STRUCT->yetkili_2                    );
    sql_query.SET_VALUE ( ":yetkili_2_info",             P_ADR_STRUCT->yetkili_2_info               );
    sql_query.SET_VALUE ( ":yetkili_3",                  P_ADR_STRUCT->yetkili_3                    );
    sql_query.SET_VALUE ( ":yetkili_3_info",             P_ADR_STRUCT->yetkili_3_info               );

    if ( P_ADR_STRUCT->fotograf_logo.isNull() EQ false ) {
        sql_query.SET_VALUE ( ":fotograf_logo"              , QVariant(P_ADR_STRUCT->fotograf_logo) );
    }
    else {
        sql_query.SET_VALUE ( ":fotograf_logo"              , QVariant("").toByteArray());
    }

    int adres_id = sql_query.INSERT();


    int tel_gizli_kayit_id     = ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR ( P_ADR_STRUCT->tel_ulke_kodu      , P_ADR_STRUCT->tel_alan_kodu       , P_ADR_STRUCT->telefon         , adres_id  );
    int cep_tel_gizli_kayit_id = ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR ( P_ADR_STRUCT->cep_tel_ulke_kodu  , P_ADR_STRUCT->cep_tel_gsm_kodu    , P_ADR_STRUCT->cep_telefonu    ,adres_id   );
    int is_tel_gizli_kayit_id  = ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR ( P_ADR_STRUCT->is_tel_ulke_kodu   , P_ADR_STRUCT->is_tel_alan_kodu    , P_ADR_STRUCT->is_telefonu     , adres_id , P_ADR_STRUCT->is_telefonu_dahili );
    int fax_gizli_kayit_id     = ADR_TELEFON_FISINDE_GIZLI_KAYIT_OLUSTUR ( P_ADR_STRUCT->fax_ulke_kodu      , P_ADR_STRUCT->fax_alan_kodu       , P_ADR_STRUCT->fax             , adres_id  );

    int email_gizli_kayit_id   = ADR_EMAIL_FISINDE_GIZLI_KAYIT_OLUSTUR   ( P_ADR_STRUCT->email, adres_id );

    sql_query.PREPARE_UPDATE ( "adr_adresler ", "adres_id",
                               "telefon_gizli_kayit_id , "
                               "cep_tel_gizli_kayit_id     , "
                               "fax_gizli_kayit_id         , "
                               "is_tel_gizli_kayit_id      , "
                               "email_gizli_kayit_id         ",
                               "adres_id   = :adres_id" );

    sql_query.SET_VALUE ( ":telefon_gizli_kayit_id",      tel_gizli_kayit_id     );
    sql_query.SET_VALUE ( ":cep_tel_gizli_kayit_id",      cep_tel_gizli_kayit_id );
    sql_query.SET_VALUE ( ":fax_gizli_kayit_id",          fax_gizli_kayit_id     );
    sql_query.SET_VALUE ( ":is_tel_gizli_kayit_id",       is_tel_gizli_kayit_id  );
    sql_query.SET_VALUE ( ":email_gizli_kayit_id",        email_gizli_kayit_id   );
    sql_query.SET_VALUE ( ":adres_id",                    adres_id           );

    sql_query.UPDATE();

    QString log_detaylari = "";
    if ( P_ADR_STRUCT->adres_kart_turu EQ FIRMA_KAYDI ) {
        log_detaylari      = "Log Türü: CARİ FİRMA KART KAYDI";
        log_detaylari.append (" # Firma Adı: ");
        log_detaylari.append ( P_ADR_STRUCT->firma_sahis_adi );
    }
    else {
        log_detaylari = QObject::tr ( "Log Türü: CARİ ŞAHIS KART KAYDI" );
        log_detaylari.append ( " # Şahıs Adı Soyadı: " );
        log_detaylari.append ( P_ADR_STRUCT->firma_sahis_adi );
    }

    E9_LOG_KAYDI_EKLE(ADRES_MODULU,LOG_ADR_FIRMA_KAYDI,LOG_ISLEM_ADD,log_detaylari);

    return adres_id;
}

/**************************************************************************************
                            ADR_KART_GUNCELLE
***************************************************************************************/

void ADR_KART_GUNCELLE ( ADRES_HESAP_STRUCT * P_ADR_STRUCT , int p_adres_id )
{

    SQL_QUERY   sql_query ( DB );

    sql_query.PREPARE_SELECT ( "adr_adresler","telefon_gizli_kayit_id, cep_tel_gizli_kayit_id, "
                               "fax_gizli_kayit_id, is_tel_gizli_kayit_id, email_gizli_kayit_id,firma_sahis_adi ",
                               "adres_id = :adres_id" );

    sql_query.SET_VALUE ( ":adres_id", p_adres_id );

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();

        int     tel_gizli_kayit_id      =   sql_query.VALUE(0).toInt();
        int     cep_tel_gizli_kayit_id  =   sql_query.VALUE(1).toInt();
        int     fax_gizli_kayit_id      =   sql_query.VALUE(2).toInt();
        int     is_tel_gizli_kayit_id   =   sql_query.VALUE(3).toInt();
        int     email_gizli_kayit_id    =   sql_query.VALUE(4).toInt();

        ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE ( P_ADR_STRUCT->tel_ulke_kodu    , P_ADR_STRUCT->tel_alan_kodu   , P_ADR_STRUCT->telefon     , tel_gizli_kayit_id                 );
        ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE ( P_ADR_STRUCT->cep_tel_ulke_kodu, P_ADR_STRUCT->cep_tel_gsm_kodu, P_ADR_STRUCT->cep_telefonu, cep_tel_gizli_kayit_id             );
        ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE ( P_ADR_STRUCT->is_tel_ulke_kodu , P_ADR_STRUCT->is_tel_alan_kodu, P_ADR_STRUCT->is_telefonu , is_tel_gizli_kayit_id, P_ADR_STRUCT->is_telefonu_dahili);
        ADR_TELEFON_FISINDEKI_GIZLI_KAYDI_GUNCELLE ( P_ADR_STRUCT->fax_ulke_kodu    , P_ADR_STRUCT->fax_alan_kodu   , P_ADR_STRUCT->fax         , fax_gizli_kayit_id                 );
        ADR_EMAIL_FISINDEKI_GIZLI_KAYDI_GUNCELLE   ( P_ADR_STRUCT->email, email_gizli_kayit_id );
    }

    sql_query.PREPARE_UPDATE ( "adr_adresler ", "adres_id",
                               "firma_sahis_adi   , "
                               "dogum_tarihi          , "
                               "dogum_yeri            , "
                               "cinsiyet              , "
                               "ogrenim_durumu        , "
                               "medeni_hali           , "
                               "kan_grubu             , "
                               "meslek                , "
                               "unvan                 , "
                               "kimlik_no             , "
                               "vergi_dairesi         , "
                               "vergi_no              , "
                               "adres_satiri_1        , "
                               "adres_satiri_2        , "
                               "adres_satiri_3        , "
                               "ilce                  , "
                               "semt                  , "
                               "ulke                  , "
                               "sehir                 , "
                               "posta_kodu            , "
                               "tel_ulke_kodu         , "
                               "tel_alan_kodu         , "
                               "telefon               , "
                               "is_tel_ulke_kodu      , "
                               "is_tel_alan_kodu      , "
                               "is_telefonu           , "
                               "is_telefonu_dahili    , "
                               "cep_tel_ulke_kodu     , "
                               "cep_tel_gsm_kodu      , "
                               "cep_telefonu          , "
                               "fax_ulke_kodu         , "
                               "fax_alan_kodu         , "
                               "fax                   , "
                               "email                 , "
                               "website               , "
                               "firma_sektoru         , "
                               "yetkili_1             , "
                               "yetkili_1_info        , "
                               "yetkili_2             , "
                               "yetkili_2_info        , "
                               "yetkili_3             , "
                               "yetkili_3_info          " ,
                               "adres_id = :adres_id" );

    sql_query.SET_VALUE ( ":firma_sahis_adi",           P_ADR_STRUCT->firma_sahis_adi );
    sql_query.SET_VALUE ( ":dogum_tarihi",              P_ADR_STRUCT->dogum_tarihi    );
    sql_query.SET_VALUE ( ":dogum_yeri",                P_ADR_STRUCT->dogum_yeri      );
    sql_query.SET_VALUE ( ":cinsiyet",                  P_ADR_STRUCT->cinsiyet        );
    sql_query.SET_VALUE ( ":ogrenim_durumu",            P_ADR_STRUCT->ogrenim_durumu);
    sql_query.SET_VALUE ( ":medeni_hali",               P_ADR_STRUCT->medeni_hali);
    sql_query.SET_VALUE ( ":kan_grubu",                 P_ADR_STRUCT->kan_grubu);
    sql_query.SET_VALUE ( ":meslek",                    P_ADR_STRUCT->meslek);
    sql_query.SET_VALUE ( ":unvan",                     P_ADR_STRUCT->unvan);
    sql_query.SET_VALUE ( ":kimlik_no",                 P_ADR_STRUCT->kimlik_no);
    sql_query.SET_VALUE ( ":vergi_dairesi",             P_ADR_STRUCT->vergi_dairesi);
    sql_query.SET_VALUE ( ":vergi_no",                  P_ADR_STRUCT->vergi_no);
    sql_query.SET_VALUE ( ":adres_satiri_1",            P_ADR_STRUCT->adres_satiri_1);
    sql_query.SET_VALUE ( ":adres_satiri_2",            P_ADR_STRUCT->adres_satiri_2);
    sql_query.SET_VALUE ( ":adres_satiri_3",            P_ADR_STRUCT->adres_satiri_3);
    sql_query.SET_VALUE ( ":ilce",                      P_ADR_STRUCT->ilce);
    sql_query.SET_VALUE ( ":semt",                      P_ADR_STRUCT->semt);
    sql_query.SET_VALUE ( ":ulke",                      P_ADR_STRUCT->ulke);
    sql_query.SET_VALUE ( ":sehir",                     P_ADR_STRUCT->sehir);
    sql_query.SET_VALUE ( ":posta_kodu",                P_ADR_STRUCT->posta_kodu);
    sql_query.SET_VALUE ( ":tel_ulke_kodu",             P_ADR_STRUCT->tel_ulke_kodu);
    sql_query.SET_VALUE ( ":tel_alan_kodu",             P_ADR_STRUCT->tel_alan_kodu);
    sql_query.SET_VALUE ( ":telefon",                   P_ADR_STRUCT->telefon);
    sql_query.SET_VALUE ( ":is_tel_ulke_kodu",          P_ADR_STRUCT->is_tel_ulke_kodu);
    sql_query.SET_VALUE ( ":is_tel_alan_kodu",          P_ADR_STRUCT->is_tel_alan_kodu);
    sql_query.SET_VALUE ( ":is_telefonu",               P_ADR_STRUCT->is_telefonu);
    sql_query.SET_VALUE ( ":is_telefonu_dahili",        P_ADR_STRUCT->is_telefonu_dahili);
    sql_query.SET_VALUE ( ":cep_tel_ulke_kodu",         P_ADR_STRUCT->cep_tel_ulke_kodu);
    sql_query.SET_VALUE ( ":cep_tel_gsm_kodu",          P_ADR_STRUCT->cep_tel_gsm_kodu);
    sql_query.SET_VALUE ( ":cep_telefonu",              P_ADR_STRUCT->cep_telefonu);
    sql_query.SET_VALUE ( ":fax_ulke_kodu",             P_ADR_STRUCT->fax_ulke_kodu);
    sql_query.SET_VALUE ( ":fax_alan_kodu",             P_ADR_STRUCT->fax_alan_kodu);
    sql_query.SET_VALUE ( ":fax",                       P_ADR_STRUCT->fax);
    sql_query.SET_VALUE ( ":email",                     P_ADR_STRUCT->email);
    sql_query.SET_VALUE ( ":website",                   P_ADR_STRUCT->website);
    sql_query.SET_VALUE ( ":firma_sektoru",             P_ADR_STRUCT->firma_sektoru );
    sql_query.SET_VALUE ( ":adres_id",                  p_adres_id);
    sql_query.SET_VALUE ( ":yetkili_1",                 P_ADR_STRUCT->yetkili_1 );
    sql_query.SET_VALUE ( ":yetkili_1_info",            P_ADR_STRUCT->yetkili_1_info );
    sql_query.SET_VALUE ( ":yetkili_2",                 P_ADR_STRUCT->yetkili_2 );
    sql_query.SET_VALUE ( ":yetkili_2_info",            P_ADR_STRUCT->yetkili_2_info );
    sql_query.SET_VALUE ( ":yetkili_3",                 P_ADR_STRUCT->yetkili_3 );
    sql_query.SET_VALUE ( ":yetkili_3_info",            P_ADR_STRUCT->yetkili_3_info );

    sql_query.UPDATE();

    if ( P_ADR_STRUCT->fotograf_logo.isNull() EQ false) {

        sql_query.PREPARE_UPDATE("adr_adresler", "adres_id",
                                 "fotograf_logo ",
                                 "adres_id = :adres_id");

        sql_query.SET_VALUE ( ":fotograf_logo" , P_ADR_STRUCT->fotograf_logo);
        sql_query.SET_VALUE ( ":adres_id"      , p_adres_id );

        sql_query.UPDATE();
    }

    QString log_detaylari = "";
    if ( P_ADR_STRUCT->adres_kart_turu EQ SAHIS_KAYDI ) {
        log_detaylari = QObject::tr ( "Log Türü: CARİ ŞAHIS KART KAYDI");
        log_detaylari.append ( " # Şahıs Adı Soyadı: " );
        log_detaylari.append ( P_ADR_STRUCT->firma_sahis_adi );
    }
    else {
        log_detaylari = "Log Türü: CARİ FİRMA KART KAYDI";
        log_detaylari.append (" # Firma Adı: ");
        log_detaylari.append ( P_ADR_STRUCT->firma_sahis_adi );

    }

    E9_LOG_KAYDI_EKLE(ADRES_MODULU,LOG_ADR_SAHIS_KAYDI,LOG_ISLEM_UPDATE ,log_detaylari);

}

/**************************************************************************************
                            ADR_KART_BILGILERINI_OKU
***************************************************************************************/

void ADR_KART_BILGILERINI_OKU( ADRES_HESAP_STRUCT *P_ADR_STRUCT, int p_adres_id )
{

    SQL_QUERY select_query( DB );
    select_query.PREPARE_SELECT("adr_adresler",
                                "adres_id,adres_kart_turu, firma_sahis_adi,dogum_tarihi, dogum_yeri,cinsiyet, "
                                "ogrenim_durumu,meslek,unvan,kimlik_no, kan_grubu,medeni_hali, vergi_dairesi, "
                                "vergi_no, adres_satiri_1,adres_satiri_2,adres_satiri_3,ilce,semt,ulke,sehir, "
                                "posta_kodu, tel_ulke_kodu, tel_alan_kodu, telefon,is_tel_ulke_kodu, is_tel_alan_kodu, "
                                "is_telefonu,cep_tel_ulke_kodu, cep_tel_gsm_kodu,cep_telefonu, fax_ulke_kodu, fax_alan_kodu, "
                                "fax, email, website, firma_sektoru, firma_id,is_telefonu_dahili,modul_id, "
                                "program_id, yetkili_1, yetkili_1_info,  "
                                "yetkili_2, yetkili_2_info,  "
                                "yetkili_3, yetkili_3_info , fotograf_logo ",
                                "adres_id = :adres_id");

    select_query.SET_VALUE (":adres_id" , p_adres_id );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }

    select_query.NEXT();

    //Adres Bilgileri
    P_ADR_STRUCT->adres_kart_turu        = select_query.VALUE("adres_kart_turu").toInt();
    P_ADR_STRUCT->firma_id               = select_query.VALUE("firma_id").toInt();
    P_ADR_STRUCT->modul_id               = select_query.VALUE("modul_id").toInt();
    P_ADR_STRUCT->program_id             = select_query.VALUE("program_id").toInt();
    P_ADR_STRUCT->firma_sahis_adi        = select_query.VALUE("firma_sahis_adi").toString();
    P_ADR_STRUCT->dogum_tarihi           = select_query.VALUE("dogum_tarihi").toString();
    P_ADR_STRUCT->dogum_yeri             = select_query.VALUE("dogum_yeri").toString();
    P_ADR_STRUCT->cinsiyet               = select_query.VALUE("cinsiyet").toString();
    P_ADR_STRUCT->ogrenim_durumu         = select_query.VALUE("ogrenim_durumu").toString();
    P_ADR_STRUCT->meslek                 = select_query.VALUE("meslek").toString();
    P_ADR_STRUCT->unvan                  = select_query.VALUE("unvan").toString();
    P_ADR_STRUCT->kimlik_no              = select_query.VALUE("kimlik_no").toString();
    P_ADR_STRUCT->kan_grubu              = select_query.VALUE("kan_grubu").toString();
    P_ADR_STRUCT->medeni_hali            = select_query.VALUE("medeni_hali").toString();
    P_ADR_STRUCT->vergi_dairesi          = select_query.VALUE("vergi_dairesi").toString();
    P_ADR_STRUCT->vergi_no               = select_query.VALUE("vergi_no").toString();
    P_ADR_STRUCT->adres_satiri_1         = select_query.VALUE("adres_satiri_1").toString();
    P_ADR_STRUCT->adres_satiri_2         = select_query.VALUE("adres_satiri_2").toString();
    P_ADR_STRUCT->adres_satiri_3         = select_query.VALUE("adres_satiri_3").toString();
    P_ADR_STRUCT->ilce                   = select_query.VALUE("ilce").toString();
    P_ADR_STRUCT->semt                   = select_query.VALUE("semt").toString();
    P_ADR_STRUCT->ulke                   = select_query.VALUE("ulke").toString();
    P_ADR_STRUCT->sehir                  = select_query.VALUE("sehir").toString();
    P_ADR_STRUCT->posta_kodu             = select_query.VALUE("posta_kodu").toString();
    P_ADR_STRUCT->tel_ulke_kodu          = select_query.VALUE("tel_ulke_kodu").toString();
    P_ADR_STRUCT->tel_alan_kodu          = select_query.VALUE("tel_alan_kodu").toString();
    P_ADR_STRUCT->telefon                = select_query.VALUE("telefon").toString();
    P_ADR_STRUCT->is_tel_ulke_kodu       = select_query.VALUE("is_tel_ulke_kodu").toString();
    P_ADR_STRUCT->is_tel_alan_kodu       = select_query.VALUE("is_tel_alan_kodu").toString();
    P_ADR_STRUCT->is_telefonu            = select_query.VALUE("is_telefonu").toString();
    P_ADR_STRUCT->cep_tel_ulke_kodu      = select_query.VALUE("cep_tel_ulke_kodu").toString();
    P_ADR_STRUCT->cep_tel_gsm_kodu       = select_query.VALUE("cep_tel_gsm_kodu").toString();
    P_ADR_STRUCT->cep_telefonu           = select_query.VALUE("cep_telefonu").toString();
    P_ADR_STRUCT->fax_ulke_kodu          = select_query.VALUE("fax_ulke_kodu").toString();
    P_ADR_STRUCT->fax_alan_kodu          = select_query.VALUE("fax_alan_kodu").toString();
    P_ADR_STRUCT->fax                    = select_query.VALUE("fax").toString();
    P_ADR_STRUCT->email                  = select_query.VALUE("email").toString();
    P_ADR_STRUCT->website                = select_query.VALUE("website").toString();
    P_ADR_STRUCT->firma_sektoru          = select_query.VALUE("firma_sektoru").toString();
    P_ADR_STRUCT->is_telefonu_dahili     = select_query.VALUE("is_telefonu_dahili").toString();

    P_ADR_STRUCT->yetkili_1              = select_query.VALUE("yetkili_1").toString();
    P_ADR_STRUCT->yetkili_1_info         = select_query.VALUE("yetkili_1_info").toString();
    P_ADR_STRUCT->yetkili_2              = select_query.VALUE("yetkili_2").toString();
    P_ADR_STRUCT->yetkili_2_info         = select_query.VALUE("yetkili_2_info").toString();
    P_ADR_STRUCT->yetkili_3              = select_query.VALUE("yetkili_3").toString();
    P_ADR_STRUCT->yetkili_3_info         = select_query.VALUE("yetkili_3_info").toString();

    P_ADR_STRUCT->fotograf_logo          = select_query.VALUE("fotograf_logo").toByteArray();

}

/**************************************************************************************
                            ADR_KARTI_SIL
***************************************************************************************/

void ADR_KARTI_SIL( int p_adres_id )
{
    SQL_QUERY delete_query( DB );

    delete_query.PREPARE_DELETE ( "adr_adresler","adres_id = :adres_id");
    delete_query.SET_VALUE      ( ":adres_id" , p_adres_id);
    delete_query.DELETE();

}

/**************************************************************************************
                            ADR_KART_FILTRE_STR_UPDATE
***************************************************************************************/

void ADR_KART_FILTRE_STR_UPDATE( int p_adres_id, QString p_filtre_str )
{
    SQL_QUERY sql_query( DB );

    sql_query.PREPARE_UPDATE ( "adr_adresler ", "adres_id",
                               "filtre_str",
                               "adres_id = :adres_id" );
    sql_query.SET_VALUE      ( ":adres_id"  , p_adres_id );
    sql_query.SET_VALUE      ( ":filtre_str", p_filtre_str);

    sql_query.UPDATE();
}

/**************************************************************************************
                            ADR_GET_ADRES_IL_ILCE
***************************************************************************************/

QStringList ADR_GET_ADRES_SEHIR_ILCE_GORE_ID_LIST ( QString p_sehir , QString p_ilce  )
{
    SQL_QUERY query( DB );

    QStringList adres_kayit_list;

    query.PREPARE_SELECT( "adr_adresler","adres_id" );

    if ( p_ilce.isEmpty() EQ false ) {
        query.AND_EKLE("ilce LIKE :ilce OR semt LIKE :semt");
        query.SET_LIKE(":ilce", p_ilce );
        query.SET_LIKE(":semt", p_ilce );
    }

    if ( p_sehir.isEmpty() EQ false ) {

        query.AND_EKLE("sehir LIKE :sehir");
        query.SET_LIKE(":sehir", p_sehir );
    }

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            adres_kayit_list <<  QVariant( query.VALUE( "adres_id" ).toInt() ).toString();
        }
    }

    return adres_kayit_list;
}

/**************************************************************************************
                            ADR_GET_ADRES_SEHIR_ILCE_SEMT_LIST
***************************************************************************************/

QStringList ADR_GET_ADRES_SEHIR_ILCE_SEMT_LIST ( int p_adres_id )
{
    QStringList adres_list;
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "adr_adresler", "sehir, ilce, semt ", " adres_id = :adres_id");
    query.SET_VALUE     ( ":adres_id", p_adres_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        adres_list << query.VALUE("sehir").toString();
        adres_list << query.VALUE("ilce").toString();
        adres_list << query.VALUE("semt").toString();
    }

    return adres_list;
}

/**************************************************************************************
                            ADR_GET_ADRES_FILTRE_STR
***************************************************************************************/

QStringList ADR_GET_ADRES_FILTRE_STR( QString p_filtre_str )
{
    QStringList adres_kayit_list;

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "adr_adresler", "adres_id", "filtre_str = :filtre_str");
    query.SET_VALUE     ( ":filtre_str", p_filtre_str );

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
         adres_kayit_list << QVariant ( query.VALUE( "adres_id" ).toInt()).toString();
        }
    }
    return adres_kayit_list;
}

/**************************************************************************************
                            ADR_GET_VERGI_NO_VERGI_DAIRESI
***************************************************************************************/

int ADR_GET_ADRES_VERGI_NO_VERGI_DAIRESI ( int p_adres_id, QString *p_vergi_no, QString *p_vergi_dairesi )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "adr_adresler",
                          "vergi_no, vergi_dairesi",
                          "adres_id = :adres_id" );

    query.SET_VALUE( ":adres_id", p_adres_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    if ( p_vergi_no NE NULL ) {
        *p_vergi_no = query.VALUE( "vergi_no" ).toString();
    }
    if ( p_vergi_dairesi NE NULL) {
        *p_vergi_dairesi = query.VALUE( "vergi_dairesi" ).toString();
    }

    return 1;
}

/**************************************************************************************
                                ADR_GET_ADRES_BILGILERI
***************************************************************************************/

QString ADR_GET_ADRES_BILGILERI ( int p_adres_id )
{
    SQL_QUERY adr_select_query ( DB );

    adr_select_query.PREPARE_SELECT("adr_adresler" , "firma_sahis_adi,adres_satiri_1,adres_satiri_2,adres_satiri_3, "
                                    "semt ,ilce,sehir,ulke" , "adres_id = :adres_id");

    adr_select_query.SET_VALUE      ( ":adres_id" , p_adres_id );

    QString adres_bilgileri;

    if ( adr_select_query.SELECT() EQ 0 ) {
        return NULL;
    }

    adr_select_query.NEXT();

    adres_bilgileri = adr_select_query.VALUE(0).toString();

    adres_bilgileri += "\n";

    if ( adr_select_query.VALUE(1).toString().isEmpty() EQ false) {
       adres_bilgileri += adr_select_query.VALUE(1).toString();
       adres_bilgileri += "\n";
    }
    if ( adr_select_query.VALUE(2).toString().isEmpty() EQ false) {
       adres_bilgileri += adr_select_query.VALUE(2).toString();
       adres_bilgileri += "\n";
    }
    if ( adr_select_query.VALUE(3).toString().isEmpty() EQ false) {
       adres_bilgileri += adr_select_query.VALUE(3).toString();
       adres_bilgileri += "\n";
    }

    if ( adr_select_query.VALUE(4).toString().isEmpty() EQ false) {
       adres_bilgileri += adr_select_query.VALUE(4).toString() + " ";
    }
    if ( adr_select_query.VALUE(5).toString().isEmpty() EQ false) {
       adres_bilgileri += adr_select_query.VALUE(5).toString() + " ";
    }
    if ( adr_select_query.VALUE(6).toString().isEmpty() EQ false) {
       adres_bilgileri += adr_select_query.VALUE(6).toString() + " ";
    }

    if ( adr_select_query.VALUE(7).toString().isEmpty() EQ false) {
       adres_bilgileri += "\n" + adr_select_query.VALUE(7).toString();
    }
    return adres_bilgileri;
}

/**************************************************************************************
                        ADR_GET_ADRES_VERGI_NO_KIMLIK_NO_ULKE
***************************************************************************************/

int ADR_GET_ADRES_VERGI_NO_KIMLIK_NO_ULKE(int p_adres_id, QString *p_vergi_no, QString *p_kimlik_no, QString *p_ulke)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "adr_adresler",
                          "vergi_no, kimlik_no, ulke",
                          "adres_id = :adres_id" );

    query.SET_VALUE( ":adres_id", p_adres_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    if ( p_vergi_no NE NULL) {
        *p_vergi_no     = query.VALUE( "vergi_no" ).toString();
    }
    if ( p_kimlik_no NE NULL ) {
        *p_kimlik_no    = query.VALUE( "kimlik_no" ).toString();
    }
    if ( p_ulke NE NULL) {
        *p_ulke         = query.VALUE( "ulke" ).toString();
    }
    return 1;
}

/**************************************************************************************
                        ADR_GET_FIRST_RECORD_ADRES_ID
***************************************************************************************/

QStringList ADR_GET_ADRES_ID_LIST( int p_kart_turu, int p_modul_id, int p_program_id )
{
    SQL_QUERY query(DB);

    QStringList adres_id_list;

    query.PREPARE_SELECT("adr_adresler" , "adres_id" );

    if ( p_kart_turu NE SAHIS_ARTI_FIRMA AND p_kart_turu  NE -1) {
       query.AND_EKLE("adres_kart_turu = :adres_kart_turu");
       query.SET_VALUE(":adres_kart_turu" , p_kart_turu);
    }
    if ( p_modul_id NE -1 ) {
        query.AND_EKLE  ("modul_id = :modul_id");
        query.SET_VALUE (":modul_id", p_modul_id);
    }
    if( p_program_id NE -1 ) {
        query.AND_EKLE  ("program_id = :program_id ");
        query.SET_VALUE (":program_id", p_program_id);
    }

    if ( query.SELECT("adres_id ASC") EQ 0 ) {
        return adres_id_list;
    }
    while ( query.NEXT() EQ true ) {
        adres_id_list << query.VALUE(0).toString();
    }
    return adres_id_list;
}

/**************************************************************************************
                        ADR_GET_FIRMA_SAHIS_ADI
***************************************************************************************/

QString ADR_GET_FIRMA_SAHIS_ADI(int p_adres_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("adr_adresler" , "firma_sahis_adi" , "adres_id = :adres_id");
    query.SET_VALUE(":adres_id" , p_adres_id);

    if ( query.SELECT() EQ 0 ) {
        return "";
    }
    query.NEXT();

    return query.VALUE(0).toString();
}

/**************************************************************************************
                        ADR_GET_ADRES_ID_LIST
***************************************************************************************/

QStringList ADR_GET_ADRES_ID_LIST(QString p_firma_sahis_adi )
{
    SQL_QUERY query(DB);

    QStringList adres_id_list;

    query.PREPARE_SELECT("adr_adresler" ,"adres_id" ,"firma_sahis_adi LIKE :firma_sahis_adi");
    query.SET_LIKE(":firma_sahis_adi" , p_firma_sahis_adi);

    if ( query.SELECT() EQ 0 ) {
        return adres_id_list;
    }

    while ( query.NEXT() ) {
        adres_id_list << query.VALUE(0).toString();
    }

    return adres_id_list;
}

/**************************************************************************************
                        ADR_GET_ADRES_KART_TURU
***************************************************************************************/

int ADR_GET_ADRES_KART_TURU(int p_adres_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("adr_adresler" ,"adres_kart_turu" ,"adres_id = :adres_id");
    query.SET_VALUE(":adres_id" , p_adres_id);

    if ( query.SELECT() EQ 0 ) {
        return -1;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                        ADR_GET_ADRES_TEL_FILTRE_LIST
***************************************************************************************/

QStringList ADR_GET_ADRES_TEL_FILTRE_LIST( QString p_telefon )
{
    SQL_QUERY query(DB);

    QStringList adres_id_list;

    query.PREPARE_SELECT("adr_adresler" ,"adres_id" ,"telefon LIKE :telefon OR is_telefonu LIKE :is_telefon  OR cep_telefonu LIKE :cep_telefonu ");

    query.SET_LIKE(":telefon"       , p_telefon);
    query.SET_LIKE(":is_telefon"    , p_telefon);
    query.SET_LIKE(":cep_telefonu"  , p_telefon);

    if ( query.SELECT() EQ 0 ) {
        return adres_id_list;
    }

    while ( query.NEXT() ) {
        adres_id_list << query.VALUE(0).toString();
    }

    return adres_id_list;
}

/**************************************************************************************
                        ADR_GET_ADRES_TEL_FILTRE_LIST
***************************************************************************************/

QStringList ADR_GET_ADRES_SEKTOR_OR_MESLEK_FILTRE_LIST( QString p_sektor_or_meslek , ADAK_ADRES_CARI_ENUM p_kart_turu )
{
    SQL_QUERY query(DB);

    QStringList adres_id_list;

    query.PREPARE_SELECT("adr_adresler" ,"adres_id" );

    if ( p_kart_turu EQ FIRMA_KAYDI ) {
        query.AND_EKLE( "firma_sektoru = :firma_sektoru" );
        query.SET_VALUE( ":firma_sektoru", p_sektor_or_meslek );
    }
    else if ( p_kart_turu EQ SAHIS_KAYDI ) {
        query.AND_EKLE( "meslek = :meslek" );
        query.SET_VALUE( ":meslek", p_sektor_or_meslek );
    }

    if ( query.SELECT() EQ 0 ) {
        return adres_id_list;
    }

    while ( query.NEXT() ) {
        adres_id_list << query.VALUE(0).toString();
    }

    return adres_id_list;
}







