#include "adak_sql.h"
#include "cari_enum.h"
#include "e9_console_utils.h"
#include "e9_enum.h"
#include "adak_utils.h"
#include "yonetim.h"
#include "adres_console_utils.h"
#include "adres_struct.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_log.h"
#include "adak_gruplar.h"
#include "sube_console_utils.h"
#include "cari_struct.h"
#include "cari_console_utils.h"

extern ADAK_SQL * DB;


/**************************************************************************************
                   CARI_SIRADAKI_FIS_NO_AL
***************************************************************************************/

int CARI_SIRADAKI_FIS_NO_AL ( QString p_fis_tarihi , ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY sql_query ( USE_DB );

    sql_query.PREPARE_SELECT ( "car_fisler", "fis_no",
                               "fis_tarihi = :fis_tarihi" );
    sql_query.SET_VALUE      ( ":fis_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT( "fis_no DESC", 0, 1 ) EQ 0 ) {
        return 1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt() + 1;
}

/**************************************************************************************
                   CARI_GET_FIS_TURLERI_STRING
***************************************************************************************/

QString CARI_GET_FIS_TURLERI_STRING ( int fis_turu )
{
    switch ( fis_turu ) {
        case ENUM_CARI_TAHSILAT_FISI:
            return QObject::tr ( "Tahsilat Fişi");
        case ENUM_CARI_ODEME_FISI  :
            return  QObject::tr ( "Ödeme Fişi");
        case ENUM_CARI_ACILIS_FISI  :
            return QObject::tr ( "Açılış Fişi" );
        case ENUM_CARI_HAREKET_FISI  :
            return QObject::tr ( "Cari Hareket Fişi" );
        case ENUM_CARI_VIRMAN_FORMU  :
            return QObject::tr ( "Cari Virman Formu" );
        default:
            break;
    }
    return "";
}

/**************************************************************************************
                   CARI_FIND_HESAP_ID
***************************************************************************************/

int CARI_FIND_HESAP_ID ( QString cari_hesap_kodu )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "car_hesaplar","cari_hesap_id","cari_hesap_kodu = :cari_hesap_kodu");
    select_query.SET_VALUE      ( ":cari_hesap_kodu" , cari_hesap_kodu );

    if ( select_query.SELECT() NE 0 ) {
         select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }
    return 0;
}

/**************************************************************************************
                   CARI_FIND_HESAP_MUH_HESAP_ID
***************************************************************************************/

int CARI_FIND_HESAP_MUH_HESAP_ID ( int p_cari_hesap_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "car_hesaplar",
                           "muh_hesap_id ",
                           "cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE      ( ":cari_hesap_id" , p_cari_hesap_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   CARI_FIND_HESAP_KODU
***************************************************************************************/

QString CARI_FIND_HESAP_KODU ( int cari_hesap_id )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT (  "car_hesaplar", "cari_hesap_kodu", "cari_hesap_id = :cari_hesap_id");
    select_query.SET_VALUE      (  ":cari_hesap_id" , cari_hesap_id );

    if ( select_query.SELECT() NE 0 ) {
         select_query.NEXT();
        return select_query.VALUE(0).toString();
    }
    return NULL;

}

/**************************************************************************************
                   CARI_FIND_HESAP_ISMI
***************************************************************************************/

QString CARI_FIND_HESAP_ISMI ( int cari_hesap_id )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "car_hesaplar","adres_id","cari_hesap_id = :cari_hesap_id");
    select_query.SET_VALUE      ( ":cari_hesap_id" , cari_hesap_id );

    if ( select_query.SELECT() NE 0 ) {
        select_query.NEXT();

        return ADR_GET_FIRMA_SAHIS_ADI(select_query.VALUE(0).toInt());
    }
    return NULL;
}

/**************************************************************************************
                   CARI_HESABIN_BAKIYESINI_ARTTIR
***************************************************************************************/

void CARI_HESABIN_BAKIYESINI_ARTTIR ( int gun_no, int cari_hesap_id, double borc_tutari, double alacak_tutari, ADAK_SQL * P_GELEN_DB)

{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    double  TOPLAM_BORC_ARRAY            [E9_ARRAY_SIZE];
    double  TOPLAM_ALACAK_ARRAY          [E9_ARRAY_SIZE];

    ROUND ( borc_tutari );
    ROUND ( alacak_tutari );

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT (  "car_hesaplar", "toplam_borc_array, toplam_alacak_array " ,
                            "cari_hesap_id=:cari_hesap_id");
    query.SET_VALUE      (  ":cari_hesap_id" , cari_hesap_id );
    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    UNPACK_DOUBLE_ARRAY ( query.VALUE ( 0 ).toString(), TOPLAM_BORC_ARRAY      , E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( query.VALUE ( 1 ).toString(), TOPLAM_ALACAK_ARRAY    , E9_ARRAY_SIZE );

    for ( int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {

        if ( borc_tutari NE 0.0 ) {
             TOPLAM_BORC_ARRAY[i]  += borc_tutari;
        }
        if ( alacak_tutari NE 0.0 ) {
             TOPLAM_ALACAK_ARRAY[i]  += alacak_tutari;
        }
    }

    if ( borc_tutari NE 0.0 ) {

        query.PREPARE_UPDATE ( "car_hesaplar", "cari_hesap_id",
                               "toplam_borc_array      ",
                               "cari_hesap_id   = :cari_hesap_id" );

        query.SET_VALUE ( ":toplam_borc_array"      , PACK_DOUBLE_ARRAY ( TOPLAM_BORC_ARRAY      , E9_ARRAY_SIZE ) );
        query.SET_VALUE ( ":cari_hesap_id"          , cari_hesap_id                                          );
        query.UPDATE    ();
    }
    else {
        query.PREPARE_UPDATE("car_hesaplar", "cari_hesap_id",
                             "toplam_alacak_array   ",
                             "cari_hesap_id       = :cari_hesap_id");

        query.SET_VALUE ( ":toplam_alacak_array"      , PACK_DOUBLE_ARRAY ( TOPLAM_ALACAK_ARRAY       , E9_ARRAY_SIZE ) );
        query.SET_VALUE ( ":cari_hesap_id"            , cari_hesap_id );
        query.UPDATE();
    }

}

/**************************************************************************************
                   CARI_HESABIN_BAKIYESINI_AZALT
***************************************************************************************/

void CARI_HESABIN_BAKIYESINI_AZALT ( int gun_no, int cari_hesap_id,double borc_tutari, double alacak_tutari, ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    double TOPLAM_BORC_ARRAY        [ E9_ARRAY_SIZE ];
    double TOPLAM_ALACAK_ARRAY      [ E9_ARRAY_SIZE ];

    ROUND ( borc_tutari );
    ROUND ( alacak_tutari );

    SQL_QUERY query ( USE_DB );

    query.PREPARE_SELECT ( "car_hesaplar",
                           "toplam_borc_array, toplam_alacak_array" ,
                           "cari_hesap_id = :cari_hesap_id");

    query.SET_VALUE      ( ":cari_hesap_id",cari_hesap_id );

    if  ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();


    UNPACK_DOUBLE_ARRAY ( query.VALUE(0).toString(), TOPLAM_BORC_ARRAY      , E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( query.VALUE(1).toString(), TOPLAM_ALACAK_ARRAY    , E9_ARRAY_SIZE );


    for  ( int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {

        if ( borc_tutari NE 0.0 ) {
             TOPLAM_BORC_ARRAY[i]   -= borc_tutari;
        }

        if  ( alacak_tutari NE 0.0 ) {
             TOPLAM_ALACAK_ARRAY[i] -= alacak_tutari;
        }
    }

    if ( borc_tutari NE 0.0 ) {
        query.PREPARE_UPDATE("car_hesaplar ", "cari_hesap_id",
                             "toplam_borc_array   " ,
                             "cari_hesap_id   = :cari_hesap_id");

        query.SET_VALUE ( ":toplam_borc_array"      , PACK_DOUBLE_ARRAY ( TOPLAM_BORC_ARRAY      , E9_ARRAY_SIZE ) );
        query.SET_VALUE ( ":cari_hesap_id"          , cari_hesap_id);
        query.UPDATE();

    }
    else {
        query.PREPARE_UPDATE("car_hesaplar","cari_hesap_id",
                             "toplam_alacak_array         ",
                             "cari_hesap_id  = :cari_hesap_id" );

        query.SET_VALUE ( ":toplam_alacak_array"      , PACK_DOUBLE_ARRAY ( TOPLAM_ALACAK_ARRAY      , E9_ARRAY_SIZE));
        query.SET_VALUE ( ":cari_hesap_id"            , cari_hesap_id );
        query.UPDATE();
    }
}

/**************************************************************************************
                   CARI_FIS_NO_KULLANILDI_MI
***************************************************************************************/

bool CARI_FIS_NO_KULLANILDI_MI ( int fis_no, QString fis_tarihi )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "car_fisler","fis_id ",
                               "fis_no = :fis_no AND fis_tarihi = :fis_tarihi");

    sql_query.SET_VALUE      ( ":fis_no"      , fis_no );
    sql_query.SET_VALUE      ( ":fis_tarihi"  , fis_tarihi   );

    if ( sql_query.SELECT() EQ 0 ) {
        return false;
    }

    return true;
}


/**************************************************************************************
                            CARI_KART_BILGILERINI_OKU
***************************************************************************************/

void CARI_SET_HESAP_STRUCT_DEFAULTS (CARI_HESAP_STRUCT *cari_hesap_struct)
{
    cari_hesap_struct->adres_kart_turu          = -1;
    cari_hesap_struct->firma_id                 = -1;
    cari_hesap_struct->telefon_gizli_kayit_id   = -1;
    cari_hesap_struct->is_tel_gizli_kayit_id    = -1;
    cari_hesap_struct->cep_tel_gizli_kayit_id   = -1;
    cari_hesap_struct->fax_gizli_kayit_id       = -1;
    cari_hesap_struct->email_gizli_kayit_id     = -1;

    cari_hesap_struct->cari_hesap_ismi          = "";
    cari_hesap_struct->dogum_tarihi             = "";
    cari_hesap_struct->dogum_yeri               = "";
    cari_hesap_struct->cinsiyet                 = "";
    cari_hesap_struct->ogrenim_durumu           = "";
    cari_hesap_struct->meslek                   = "";
    cari_hesap_struct->unvan                    = "";
    cari_hesap_struct->kimlik_no                = "";
    cari_hesap_struct->kan_grubu                = "";
    cari_hesap_struct->medeni_hali              = "";
    cari_hesap_struct->vergi_dairesi            = "";
    cari_hesap_struct->vergi_no                 = "";
    cari_hesap_struct->adres_satiri_1           = "";
    cari_hesap_struct->adres_satiri_2           = "";
    cari_hesap_struct->adres_satiri_3           = "";
    cari_hesap_struct->ilce                     = "";
    cari_hesap_struct->semt                     = "";
    cari_hesap_struct->ulke                     = "";
    cari_hesap_struct->sehir                    = "";
    cari_hesap_struct->posta_kodu               = "";
    cari_hesap_struct->tel_ulke_kodu            = "";
    cari_hesap_struct->tel_alan_kodu            = "";
    cari_hesap_struct->telefon                  = "";
    cari_hesap_struct->is_tel_ulke_kodu         = "";
    cari_hesap_struct->is_tel_alan_kodu         = "";
    cari_hesap_struct->is_telefonu              = "";
    cari_hesap_struct->cep_tel_ulke_kodu        = "";
    cari_hesap_struct->cep_tel_gsm_kodu         = "";
    cari_hesap_struct->cep_telefonu             = "";
    cari_hesap_struct->fax_ulke_kodu            = "";
    cari_hesap_struct->fax_alan_kodu            = "";
    cari_hesap_struct->fax                      = "";
    cari_hesap_struct->email                    = "";
    cari_hesap_struct->website                  = "";
    cari_hesap_struct->firma_sektoru            = "";
    cari_hesap_struct->is_telefonu_dahili       = "";

    cari_hesap_struct->fotograf_logo            = "";

    cari_hesap_struct->muh_hesap_id             = -1;
    cari_hesap_struct->cari_hesap_kodu          = "";
    cari_hesap_struct->cari_kart_turu           = -1;
    cari_hesap_struct->para_birimi_id           = -1;

    cari_hesap_struct->program_id               = -1;
    cari_hesap_struct->modul_id                 = -1;
    cari_hesap_struct->cari_kart_tipi           = -1;
    cari_hesap_struct->base_record_id           =  0;
}

/**************************************************************************************
                            CARI_KART_BILGILERINI_OKU
***************************************************************************************/

void CARI_KART_BILGILERINI_OKU ( int cari_hesap_id, CARI_HESAP_STRUCT *kayit_bilgileri )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ("car_hesaplar",
                                 "adres_id,cari_hesap_kodu,"
                                 "muh_hesap_id,"
                                 "para_birimi_id , cari_kart_tipi",
                                 "cari_hesap_id=:cari_hesap_id");
    select_query.SET_VALUE      (":cari_hesap_id",cari_hesap_id);

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }

    select_query.NEXT();
    int adres_id = select_query.VALUE(0).toInt();

    //Cari Bilgileri
    kayit_bilgileri->muh_hesap_id       = select_query.VALUE("muh_hesap_id").toInt();
    kayit_bilgileri->cari_hesap_kodu    = select_query.VALUE("cari_hesap_kodu").toString();
    kayit_bilgileri->para_birimi_id     = select_query.VALUE("para_birimi_id").toInt();
    kayit_bilgileri->cari_kart_tipi     = select_query.VALUE("cari_kart_tipi").toInt();

    ADRES_HESAP_STRUCT  * ADR_STRUCT = new ADRES_HESAP_STRUCT;
    ADR_CLEAR_ADRES_STRUCT(ADR_STRUCT);

    ADR_KART_BILGILERINI_OKU( ADR_STRUCT, adres_id );

    kayit_bilgileri->cari_hesap_ismi             = ADR_STRUCT->firma_sahis_adi;
    kayit_bilgileri->dogum_tarihi                = ADR_STRUCT->dogum_tarihi;
    kayit_bilgileri->dogum_yeri                  = ADR_STRUCT->dogum_yeri;
    kayit_bilgileri->cinsiyet                    = ADR_STRUCT->cinsiyet;
    kayit_bilgileri->ogrenim_durumu              = ADR_STRUCT->ogrenim_durumu;
    kayit_bilgileri->medeni_hali                 = ADR_STRUCT->medeni_hali;
    kayit_bilgileri->kan_grubu                   = ADR_STRUCT->kan_grubu;
    kayit_bilgileri->meslek                      = ADR_STRUCT->meslek;
    kayit_bilgileri->unvan                       = ADR_STRUCT->unvan;
    kayit_bilgileri->kimlik_no                   = ADR_STRUCT->kimlik_no;
    kayit_bilgileri->vergi_dairesi               = ADR_STRUCT->vergi_dairesi;
    kayit_bilgileri->vergi_no                    = ADR_STRUCT->vergi_no;
    kayit_bilgileri->adres_satiri_1              = ADR_STRUCT->adres_satiri_1;
    kayit_bilgileri->adres_satiri_2              = ADR_STRUCT->adres_satiri_2;
    kayit_bilgileri->adres_satiri_3              = ADR_STRUCT->adres_satiri_3;
    kayit_bilgileri->ilce                        = ADR_STRUCT->ilce;
    kayit_bilgileri->semt                        = ADR_STRUCT->semt;
    kayit_bilgileri->ulke                        = ADR_STRUCT->ulke;
    kayit_bilgileri->sehir                       = ADR_STRUCT->sehir;
    kayit_bilgileri->posta_kodu                  = ADR_STRUCT->posta_kodu;
    kayit_bilgileri->tel_ulke_kodu               = ADR_STRUCT->tel_ulke_kodu;
    kayit_bilgileri->tel_alan_kodu               = ADR_STRUCT->tel_alan_kodu;
    kayit_bilgileri->telefon                     = ADR_STRUCT->telefon;
    kayit_bilgileri->is_tel_ulke_kodu            = ADR_STRUCT->is_tel_ulke_kodu;
    kayit_bilgileri->is_tel_alan_kodu            = ADR_STRUCT->is_tel_alan_kodu;
    kayit_bilgileri->is_telefonu                 = ADR_STRUCT->is_telefonu;
    kayit_bilgileri->cep_tel_ulke_kodu           = ADR_STRUCT->cep_tel_ulke_kodu;
    kayit_bilgileri->cep_tel_gsm_kodu            = ADR_STRUCT->cep_tel_gsm_kodu;
    kayit_bilgileri->cep_telefonu                = ADR_STRUCT->cep_telefonu;
    kayit_bilgileri->fax_ulke_kodu               = ADR_STRUCT->fax_ulke_kodu;
    kayit_bilgileri->fax_alan_kodu               = ADR_STRUCT->fax_alan_kodu;
    kayit_bilgileri->fax                         = ADR_STRUCT->fax;
    kayit_bilgileri->email                       = ADR_STRUCT->email;
    kayit_bilgileri->modul_id                    = ADR_STRUCT->modul_id;
    kayit_bilgileri->program_id                  = ADR_STRUCT->program_id;
    kayit_bilgileri->is_telefonu_dahili          = ADR_STRUCT->is_telefonu_dahili;
    kayit_bilgileri->adres_kart_turu             = ADR_STRUCT->adres_kart_turu;
    kayit_bilgileri->website                     = ADR_STRUCT->website;
    kayit_bilgileri->fotograf_logo               = ADR_STRUCT->fotograf_logo;
    kayit_bilgileri->cari_kart_turu              = ADR_STRUCT->adres_kart_turu;
    kayit_bilgileri->yetkili_1                   = ADR_STRUCT->yetkili_1;
    kayit_bilgileri->yetkili_1_info              = ADR_STRUCT->yetkili_1_info;
    kayit_bilgileri->yetkili_2                   = ADR_STRUCT->yetkili_2;
    kayit_bilgileri->yetkili_2_info              = ADR_STRUCT->yetkili_2_info;
    kayit_bilgileri->yetkili_3                   = ADR_STRUCT->yetkili_3;
    kayit_bilgileri->yetkili_3_info              = ADR_STRUCT->yetkili_3_info;
    kayit_bilgileri->firma_sektoru               = ADR_STRUCT->firma_sektoru;

}

/**************************************************************************************
                   CARI_KART_OLUSTUR
***************************************************************************************/

int CARI_KART_OLUSTUR ( int cari_kart_turu , CARI_HESAP_STRUCT * kayit_bilgileri )
{
    SQL_QUERY insert_query ( DB );

    insert_query.PREPARE_INSERT ( "car_hesaplar", "cari_hesap_id" ,
                                  "cari_hesap_kodu,"
                                  "muh_hesap_id,para_birimi_id,"
                                  "cari_kart_tipi, program_id, modul_id, base_record_id, cari_hesap_ismi " );

    insert_query.SET_VALUE      ( ":cari_hesap_kodu"      , kayit_bilgileri->cari_hesap_kodu    );
    insert_query.SET_VALUE      ( ":cari_kart_tipi"       , kayit_bilgileri->cari_kart_tipi     );
    insert_query.SET_VALUE      ( ":program_id"           , kayit_bilgileri->program_id         );
    insert_query.SET_VALUE      ( ":modul_id"             , kayit_bilgileri->modul_id           );
    insert_query.SET_VALUE      ( ":base_record_id"       , kayit_bilgileri->base_record_id     );
    insert_query.SET_VALUE      ( ":cari_hesap_ismi"      , kayit_bilgileri->cari_hesap_ismi    );

    if ( kayit_bilgileri->muh_hesap_id EQ 0 ) {
        kayit_bilgileri->muh_hesap_id  = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "cari_alici_ent_hesap_id" );
    }

    insert_query.SET_VALUE      ( ":muh_hesap_id"         , kayit_bilgileri->muh_hesap_id    );
    insert_query.SET_VALUE      ( ":para_birimi_id"       , kayit_bilgileri->para_birimi_id  );

    int cari_hesap_id = insert_query.INSERT();

    if ( kayit_bilgileri->modul_id EQ CARI_MODULU AND kayit_bilgileri->program_id EQ E9_PROGRAMI ) {
        kayit_bilgileri->base_record_id = cari_hesap_id;
    }

    ADRES_HESAP_STRUCT  * ADR_STRUCT = new ADRES_HESAP_STRUCT;
    ADR_CLEAR_ADRES_STRUCT(ADR_STRUCT);

    kayit_bilgileri->cari_kart_turu = cari_kart_turu;

    CARI_COPY_STRUCT_CARI_2_ADRES( kayit_bilgileri, ADR_STRUCT );

    int adres_id  = ADR_KART_OLUSTUR ( ADR_STRUCT );

    SQL_QUERY query( DB );

    query.PREPARE_UPDATE( "car_hesaplar", "cari_hesap_id", "adres_id", "cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE     ( ":adres_id"       , adres_id      );
    query.SET_VALUE     ( ":cari_hesap_id"  , cari_hesap_id );

    query.UPDATE();

    QString log_detaylari = kayit_bilgileri->cari_hesap_kodu + "  " + kayit_bilgileri->cari_hesap_ismi;

    E9_LOG_KAYDI_EKLE(CARI_MODULU,LOG_CARI_HESAPLAR,LOG_ISLEM_ADD,log_detaylari);

    return cari_hesap_id;
}

/**************************************************************************************
                   CARI_KART_GUNCELLE
***************************************************************************************/

bool CARI_KART_GUNCELLE ( int cari_hesap_id,int cari_kart_turu,CARI_HESAP_STRUCT * kayit_bilgileri )
{
    SQL_QUERY select_query ( DB );

    int adres_id = 0;

    QString cari_hesap_kodu;
    QString cari_hesap_ismi;

    select_query.PREPARE_SELECT ( "car_hesaplar",
                                  "adres_id,cari_hesap_kodu ",
                                  "cari_hesap_id = :cari_hesap_id" );
    select_query.SET_VALUE      ( ":cari_hesap_id" , cari_hesap_id );

    if ( select_query.SELECT() NE 0 ) {
         select_query.NEXT();
        adres_id        = select_query.VALUE(0).toInt();
        cari_hesap_kodu = select_query.VALUE(1).toString();
        cari_hesap_ismi = ADR_GET_FIRMA_SAHIS_ADI(adres_id);
    }


    ADRES_HESAP_STRUCT  * ADR_STRUCT = new ADRES_HESAP_STRUCT;
    ADR_CLEAR_ADRES_STRUCT(ADR_STRUCT);

    kayit_bilgileri->cari_kart_turu = cari_kart_turu;

    CARI_COPY_STRUCT_CARI_2_ADRES( kayit_bilgileri, ADR_STRUCT );

    ADR_KART_GUNCELLE( ADR_STRUCT, adres_id );

    SQL_QUERY update_query ( DB );

    update_query.PREPARE_UPDATE ( "car_hesaplar", "cari_hesap_id",
                                  "cari_hesap_kodu              ,"
                                  "muh_hesap_id                 ,"
                                  "para_birimi_id               ,"
                                  "cari_kart_tipi                ",
                                  "cari_hesap_id = :cari_hesap_id");

    update_query.SET_VALUE     ( ":cari_hesap_kodu" , kayit_bilgileri->cari_hesap_kodu );

    if ( kayit_bilgileri->muh_hesap_id EQ 0 ) {
        kayit_bilgileri->muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "cari_alici_ent_hesap_id" );
    }

    update_query.SET_VALUE     ( ":muh_hesap_id"    , kayit_bilgileri->muh_hesap_id    );
    update_query.SET_VALUE     ( ":para_birimi_id"  , kayit_bilgileri->para_birimi_id  );
    update_query.SET_VALUE     ( ":cari_kart_tipi"  , kayit_bilgileri->cari_kart_tipi  );
    update_query.SET_VALUE     ( ":cari_hesap_id"   , cari_hesap_id );
    update_query.UPDATE();

    QString log_detaylari = cari_hesap_kodu + "  " + cari_hesap_ismi;

    E9_LOG_KAYDI_EKLE(CARI_MODULU,LOG_CARI_HESAPLAR,LOG_ISLEM_UPDATE,log_detaylari);

    return true;
}

/**************************************************************************************
                   CARI_KAYIT_SIL
***************************************************************************************/

bool CARI_KART_SIL ( int cari_hesap_id )
{
    SQL_QUERY select_query ( DB );
    SQL_QUERY delete_query ( DB );

    int adres_id = 0;

    QString cari_hesap_kodu ;
    QString cari_hesap_ismi ;

    select_query.PREPARE_SELECT ( "car_hesaplar","adres_id,cari_hesap_kodu ",
                                 "cari_hesap_id = :cari_hesap_id");
    select_query.SET_VALUE      ( ":cari_hesap_id" , cari_hesap_id );

    if (select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        adres_id        = select_query.VALUE(0).toInt();
        cari_hesap_kodu = select_query.VALUE(1).toString();
        cari_hesap_ismi = ADR_GET_FIRMA_SAHIS_ADI(adres_id);

    }
    if (adres_id NE 0 ) {
        ADR_KARTI_SIL( adres_id );
    }

    delete_query.PREPARE_DELETE ( "car_hesaplar","cari_hesap_id = :cari_hesap_id");
    delete_query.SET_VALUE      ( ":cari_hesap_id" , cari_hesap_id);
    delete_query.DELETE();

    QString log_detaylari = cari_hesap_kodu + "  " + cari_hesap_ismi;

    E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_HESAPLAR , LOG_ISLEM_DELETE,log_detaylari);

    return true;
}

/**************************************************************************************
                                    CARI_FIND_RECORD
***************************************************************************************/

int CARI_FIND_RECORD (QString cari_hesap_ismi,int kayit_turu,int program_id,int modul_id)
{
    SQL_QUERY  query(DB);

    QStringList adres_id_list = ADR_GET_ADRES_ID_LIST( kayit_turu, modul_id, program_id );

    query.PREPARE_SELECT( "car_hesaplar","cari_hesap_id",
                          "cari_hesap_kodu = :cari_hesap_kodu"  );

    query.SET_VALUE ( ":cari_hesap_kodu"    , cari_hesap_ismi );

    if ( adres_id_list.isEmpty() EQ false ) {
        query.AND_MULTI_EKLE("adres_id" , query.TO_QVARIANT(adres_id_list) , ADAK_OR);
    }

    if ( query.SELECT( "cari_hesap_kodu" , 0 , 1 ) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                                    CARI_FIND_FIRST_RECORD
***************************************************************************************/

int CARI_FIND_FIRST_RECORD (int kayit_turu,int program_id,int modul_id)
{ 
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "car_hesaplar",
                                  "cari_hesap_id, adres_id" );

    if ( !( program_id EQ E9_PROGRAMI AND modul_id EQ CARI_MODULU ) ) {

        select_query.AND_EKLE  ( "program_id    = :program_id" );
        select_query.AND_EKLE  ( "modul_id      = :modul_id" );
        select_query.SET_VALUE ( ":program_id" , program_id );
        select_query.SET_VALUE ( ":modul_id"   , modul_id );
    }


    if ( select_query.SELECT( "cari_hesap_kodu ASC") NE 0 ) {

        while ( select_query.NEXT() EQ true ) {

            int adres_kart_turu = ADR_GET_ADRES_KART_TURU( select_query.VALUE(1).toInt() );

            if ( adres_kart_turu EQ kayit_turu ) {
                return select_query.VALUE(0).toInt();
            }
        }
    }
    return 0;
}

/**************************************************************************************
                                    CARI_FIND_LAST_RECORD
***************************************************************************************/

int CARI_FIND_LAST_RECORD (int kayit_turu,int program_id,int modul_id)
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "car_hesaplar",
                                  "cari_hesap_id, adres_id" );

    // e9 ve cari degilse girecek
    if ( !( program_id EQ E9_PROGRAMI AND modul_id EQ CARI_MODULU ) ) {

        select_query.AND_EKLE  ( "program_id    = :program_id" );
        select_query.AND_EKLE  ( "modul_id      = :modul_id" );
        select_query.SET_VALUE ( ":program_id" , program_id );
        select_query.SET_VALUE ( ":modul_id"   , modul_id );
    }


    if ( select_query.SELECT( "cari_hesap_kodu DESC") NE 0 ) {

        while ( select_query.NEXT() EQ true ) {

            int adres_kart_turu = ADR_GET_ADRES_KART_TURU( select_query.VALUE(1).toInt() );

            if ( adres_kart_turu EQ kayit_turu ) {
                return select_query.VALUE(0).toInt();
            }
        }
    }
    return 0;
}

/**************************************************************************************
                                    CARI_FIND_PREV_RECORD
***************************************************************************************/

int CARI_FIND_PREV_RECORD (QString cari_hesap_kodu,int kayit_turu,int program_id,int modul_id)
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "car_hesaplar",
                                  "cari_hesap_id,adres_id",
                                  "cari_hesap_kodu < :cari_hesap_kodu   " );

    select_query.SET_VALUE  ( ":cari_hesap_kodu" ,  cari_hesap_kodu );

    if ( !( program_id EQ E9_PROGRAMI AND modul_id EQ CARI_MODULU ) ) {

        select_query.AND_EKLE  ( "program_id    = :program_id" );
        select_query.AND_EKLE  ( "modul_id      = :modul_id" );
        select_query.SET_VALUE ( ":program_id" , program_id );
        select_query.SET_VALUE ( ":modul_id"   , modul_id );
    }

    if ( select_query.SELECT( "cari_hesap_kodu DESC" ) NE 0 ) {
        while ( select_query.NEXT() ) {

            int adres_kart_turu = ADR_GET_ADRES_KART_TURU(select_query.VALUE(1).toInt());

            if ( adres_kart_turu EQ kayit_turu ) {
                return select_query.VALUE(0).toInt();
            }
        }
    }
    return 0;
}

/**************************************************************************************
                                    CARI_FIND_NEXT_RECORD
***************************************************************************************/

int CARI_FIND_NEXT_RECORD (QString cari_hesap_kodu,int kayit_turu,int program_id,int modul_id)
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "car_hesaplar",
                                  "cari_hesap_id, adres_id",
                                  "cari_hesap_kodu >:cari_hesap_kodu    " );

    select_query.SET_VALUE  ( ":cari_hesap_kodu" ,  cari_hesap_kodu );

    if ( !( program_id EQ E9_PROGRAMI AND modul_id EQ CARI_MODULU ) ) {

        select_query.AND_EKLE  ( "program_id    = :program_id" );
        select_query.AND_EKLE  ( "modul_id      = :modul_id" );
        select_query.SET_VALUE ( ":program_id" , program_id );
        select_query.SET_VALUE ( ":modul_id"   , modul_id );
    }

    if ( select_query.SELECT( "cari_hesap_kodu ASC") NE 0 ) {

        while ( select_query.NEXT()) {
            int adres_kart_turu = ADR_GET_ADRES_KART_TURU( select_query.VALUE(1).toInt() );
            if ( adres_kart_turu EQ kayit_turu ) {
                return select_query.VALUE(0).toInt();
            }
        }
    }
    return 0;
}

/**************************************************************************************
                                    CARI_GET_FIRMA_SAHIS_ADI
***************************************************************************************/

QString CARI_GET_FIRMA_SAHIS_ADI (int cari_hesap_id)
{
    SQL_QUERY  query(DB);

    query.PREPARE_SELECT( "car_hesaplar","adres_id",
                          "cari_hesap_id = :cari_hesap_id" );

    query.SET_VALUE ( ":cari_hesap_id", cari_hesap_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return ADR_GET_FIRMA_SAHIS_ADI(query.VALUE(0).toInt());
}

/***************************************************************************************
                   CARI_GET_MUH_HESAP_ID
***************************************************************************************/

int CARI_GET_MUH_HESAP_ID ( int cari_hesap_id )
{

    SQL_QUERY  query(DB);

    query.PREPARE_SELECT( "car_hesaplar", "muh_hesap_id",
                          "cari_hesap_id = :cari_hesap_id" );

    query.SET_VALUE ( ":cari_hesap_id", cari_hesap_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/***************************************************************************************
                   CARI_GET_PARA_BIRIMI_TUTARI
***************************************************************************************/

double CARI_GET_PARA_BIRIMI_TUTARI ( int cari_para_birimi_id, int fis_para_birimi_id, double doviz_kuru,
                                     double parite, double tutar)
{

    double cari_para_birimi_tutari = 0.0;

    if ( cari_para_birimi_id EQ fis_para_birimi_id ) {
        cari_para_birimi_tutari = tutar;
    }
    else {
        if ( cari_para_birimi_id EQ DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
            cari_para_birimi_tutari = tutar * doviz_kuru;
        }
        else {
            cari_para_birimi_tutari = tutar * parite;
        }
    }
    return ROUND ( cari_para_birimi_tutari);
}

/***************************************************************************************
                  CARI_GET_ADRES_RECORD_ID
***************************************************************************************/

int CARI_GET_ADRES_RECORD_ID(int cari_hesap_id)
{
    SQL_QUERY       query( DB );

    query.PREPARE_SELECT( "car_hesaplar", "adres_id", "cari_hesap_id = :cari_hesap_id" );
    query.SET_VALUE     (":cari_hesap_id", cari_hesap_id);

    if ( query.SELECT() EQ 0 ) {
        return -1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}


/**************************************************************************************
                   CARI_FIS_NO_KULLANILDI_MI
***************************************************************************************/

bool CARI_HESAP_KODU_KULLANILDI_MI ( QString hesap_kodu )
{
    SQL_QUERY sql_query ( DB );
    QString query = QString( "cari_hesap_kodu = '%1' ").arg( hesap_kodu );

    sql_query.PREPARE_SELECT ( "car_hesaplar", "cari_hesap_id", query );

    if ( sql_query.SELECT() NE 0 ) {
        return true;
    }

    return false;
}

/**************************************************************************************
                         CARI_HESAP_ISLEM_GORDU_MU
***************************************************************************************/

bool CARI_HESAP_ISLEM_GORDU_MU(int cari_hesap_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("car_fisler","fis_id","cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE(":cari_hesap_id" , cari_hesap_id);

    if ( query.SELECT() NE 0 ) {
        return true;
    }

    query.PREPARE_SELECT("car_fis_satirlari","fis_satiri_id", "cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE     (":cari_hesap_id" , cari_hesap_id);

    if ( query.SELECT() NE 0 ) {
        return true;
    }

    return false;
}

/**************************************************************************************
                         GET_CARI_KART_BORCU
***************************************************************************************/

int  CARI_KART_BORC_BILGISINI_GETIR  ( int cari_hesap_id, BORC_BILGISI * borc_bilgisi )
{

    SQL_QUERY select_query ( DB );

    double  toplam_borc_array   [ E9_ARRAY_SIZE ];
    double  toplam_alacak_array [ E9_ARRAY_SIZE ];
    borc_bilgisi->borc_bakiye   = 0.0;
    borc_bilgisi->alacak_bakiye = 0.0;

    select_query.PREPARE_SELECT ( "car_hesaplar",
                                  "toplam_borc_array , toplam_alacak_array, para_birimi_id" ,
                                  "cari_hesap_id = :cari_hesap_id" );
    select_query.SET_VALUE      ( ":cari_hesap_id" , cari_hesap_id );

    if ( select_query.SELECT() EQ 0 ) {

        return 0;
    }

    select_query.NEXT();

    UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_borc_array").toString(), toplam_borc_array   , E9_ARRAY_SIZE);
    UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_alacak_array").toString(), toplam_alacak_array , E9_ARRAY_SIZE);

    borc_bilgisi->para_birimi_id = select_query.VALUE(2).toInt();

    double toplam_borc   = toplam_borc_array   [E9_SON_BAKIYE];
    double toplam_alacak = toplam_alacak_array [E9_SON_BAKIYE];

    if ( toplam_borc > toplam_alacak )
    {
       borc_bilgisi->borc_bakiye   = ROUND(toplam_borc - toplam_alacak );
       borc_bilgisi->alacak_bakiye = 0.0;
    }
    else if ( toplam_alacak > toplam_borc )
    {
        borc_bilgisi->alacak_bakiye = ROUND(toplam_alacak - toplam_borc );
        borc_bilgisi->borc_bakiye   = 0.0;
    }
    else {
       borc_bilgisi->borc_bakiye = 0.0;
       borc_bilgisi->alacak_bakiye = 0.0;
    }

    return 1;
}

/**************************************************************************************
                         CARI_GET_PARA_BIRIM_ID
***************************************************************************************/

int CARI_GET_PARA_BIRIM_ID ( int cari_hesap_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_hesaplar",
                          "para_birimi_id",
                          "cari_hesap_id = :cari_hesap_id" );

    query.SET_VALUE( ":cari_hesap_id", cari_hesap_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE( 0 ).toInt();

}

/**************************************************************************************
                         CARI_GET_HESAP_KODU_ISMI_PARA_BIRIMI_ID
***************************************************************************************/

int CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( int cari_hesap_id , QString *cari_hesap_kodu, QString *cari_hesap_ismi, int *para_birim_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_hesaplar",
                          "cari_hesap_kodu, adres_id, para_birimi_id ",
                          "cari_hesap_id = :cari_hesap_id" );

    query.SET_VALUE( ":cari_hesap_id", cari_hesap_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    if ( cari_hesap_kodu NE NULL) {
        *cari_hesap_kodu = query.VALUE( "cari_hesap_kodu" ).toString();
    }
    if ( cari_hesap_ismi NE NULL ) {
        int adres_id = query.VALUE("adres_id").toInt();

        *cari_hesap_ismi = ADR_GET_FIRMA_SAHIS_ADI(adres_id);
    }
    if ( para_birim_id NE NULL) {
        *para_birim_id   = query.VALUE( "para_birimi_id" ).toInt();
    }

    return 1;
}

/**************************************************************************************
                         CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID
***************************************************************************************/

int CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( QString cari_hesap_kodu, int *cari_hesap_id, QString *cari_hesap_ismi, int *para_birim_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_hesaplar",
                          "cari_hesap_id, adres_id, para_birimi_id",
                          "cari_hesap_kodu = :cari_hesap_kodu" );

    query.SET_VALUE( ":cari_hesap_kodu", cari_hesap_kodu );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    if ( cari_hesap_id NE NULL) {
        *cari_hesap_id = query.VALUE( "cari_hesap_id" ).toInt();
    }
    if ( cari_hesap_ismi NE NULL ) {
        int adres_id     = query.VALUE("adres_id").toInt();
        *cari_hesap_ismi = ADR_GET_FIRMA_SAHIS_ADI(adres_id);
    }
    if ( para_birim_id NE NULL) {
        *para_birim_id   = query.VALUE( "para_birimi_id" ).toInt();
    }

    return 1;
}

/**************************************************************************************
                         CARI_GET_VERGI_NO_VERGI_DAIRESI
***************************************************************************************/

int CARI_GET_VERGI_NO_VERGI_DAIRESI ( int p_cari_hesap_id, QString * p_vergi_no, QString * p_vergi_dairesi )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_hesaplar",
                          "adres_id",
                          "cari_hesap_id = :cari_hesap_id" );

    query.SET_VALUE( ":cari_hesap_id", p_cari_hesap_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    int adres_id = query.VALUE( "adres_id").toInt();

    ADR_GET_ADRES_VERGI_NO_VERGI_DAIRESI( adres_id, p_vergi_no, p_vergi_dairesi );

    return 1;
}

/**************************************************************************************
                         CARI_GET_ADRES_BILGILERI
***************************************************************************************/

QString CARI_GET_ADRES_BILGILERI ( int p_cari_hesap_id ) {

    SQL_QUERY query( DB );
    query.PREPARE_SELECT( "car_hesaplar",
                          "adres_id",
                          "cari_hesap_id = :cari_hesap_id" );

    query.SET_VALUE( ":cari_hesap_id", p_cari_hesap_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    int adres_id = query.VALUE( "adres_id").toInt();

    return ADR_GET_ADRES_BILGILERI( adres_id );
}

/**************************************************************************************
                         CARI_GET_MUHASEBE_HESABI_ID_AND_PARA_BIRIM_ID
***************************************************************************************/

int CARI_GET_MUHASEBE_ID_AND_PARA_BIRIM_ID ( int p_cari_hesap_id, int * p_muh_hesap_id, int * p_para_birim_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_hesaplar",
                          "muh_hesap_id, para_birimi_id",
                          "cari_hesap_id = :cari_hesap_id" );

    query.SET_VALUE( ":cari_hesap_id", p_cari_hesap_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    if ( p_muh_hesap_id NE NULL ) {
        *p_muh_hesap_id = query.VALUE( "muh_hesap_id" ).toInt();
    }
    if ( p_para_birim_id NE NULL) {
        *p_para_birim_id   = query.VALUE( "para_birimi_id" ).toInt();
    }
    return 1;
}

/**************************************************************************************
                         CARI_GET_VERGI_NO_KIMLIK_NO_ULKE
***************************************************************************************/

int CARI_GET_VERGI_NO_KIMLIK_NO_ULKE( int cari_hesap_id, QString *p_vergi_no, QString *p_kimlik_no, QString *p_ulke )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_hesaplar",
                          "adres_id",
                          "cari_hesap_id = :cari_hesap_id" );

    query.SET_VALUE( ":cari_hesap_id", cari_hesap_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();
    int adres_id = query.VALUE( "adres_id" ).toInt();

    return ADR_GET_ADRES_VERGI_NO_KIMLIK_NO_ULKE( adres_id, p_vergi_no, p_kimlik_no, p_ulke );
}

/**************************************************************************************
                         CARI_GET_FIS_SATIRLAR_CEK_SENET_ID_LIST
***************************************************************************************/

QList < int > CARI_GET_FIS_SATIRLAR_CEK_SENET_ID_LIST( int p_fis_id, int cek_veya_senet )
{
    QList <int > cek_id_list;
    SQL_QUERY select_query( DB );

    select_query.PREPARE_SELECT ("car_fis_satirlari", "cek_senet_id", "fis_id = :fis_id" );
    select_query.SET_VALUE      ( ":fis_id" , p_fis_id );

    if( cek_veya_senet NE -1 ) {
        select_query.AND_EKLE   ( "islem_turu = :islem_turu" );
        select_query.SET_VALUE  ( ":islem_turu" , cek_veya_senet );
    }

    if ( select_query.SELECT() NE 0 ) {
        while ( select_query.NEXT() EQ true ) {
            cek_id_list << select_query.VALUE( 0 ).toInt();
        }
    }

    return cek_id_list;
}

/**************************************************************************************
                         CARI_GET_CEK_SENET_ID_LIST
***************************************************************************************/

QList <int > CARI_GET_CEK_SENET_ID_LIST(int p_cari_hesap_id, int modul_id)
{
    QList <int> cek_senet_id_list;

    SQL_QUERY select_query( DB );

    select_query.PREPARE_SELECT ( "car_fisler,car_fis_satirlari",
                                  "cek_senet_id ",
                                  "car_fisler.fis_id = car_fis_satirlari.fis_id AND "
                                  "car_fisler.modul_id = :modul_id");

    select_query.SET_VALUE      ( ":modul_id", modul_id );

    if( p_cari_hesap_id NE -1 ) {
        select_query.AND_EKLE( "car_fisler.cari_hesap_id = :cari_hesap_id" );
        select_query.SET_VALUE( ":cari_hesap_id", p_cari_hesap_id);
    }

    if ( select_query.SELECT() NE 0 ) {
        while ( select_query.NEXT() ) {
            cek_senet_id_list << select_query.VALUE( 0 ).toInt();
        }
    }
    return cek_senet_id_list;
}

/**************************************************************************************
                         CARI_FILTRE_STR_UPDATE
***************************************************************************************/

void CARI_HESAP_FILTRE_STR_UPDATE ( int p_cari_hesap_id, QString p_filtre_str )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_hesaplar", "adres_id", "cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE     ( ":cari_hesap_id", p_cari_hesap_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int adres_id = query.VALUE(0).toInt();

    ADR_KART_FILTRE_STR_UPDATE( adres_id, p_filtre_str );

}



/**************************************************************************************
                         CARI_GET_CARI_KART_TIPI_ENUM
***************************************************************************************/

int CARI_GET_CARI_KART_TIPI_ENUM(QString p_cari_kart_tipi)
{
    if ( p_cari_kart_tipi EQ "Alıcı" ) {
        return ENUM_CARI_ALICI;
    }
    else if ( p_cari_kart_tipi EQ "Satıcı" ) {
        return ENUM_CARI_SATICI;
    }
    else if ( p_cari_kart_tipi EQ "Personel") {
        return ENUM_CARI_PERSONEL;
    }
    else if ( p_cari_kart_tipi EQ "Firma Ortağı") {
        return ENUM_FIRMA_ORTAGI;
    }

    return -1;
}

/**************************************************************************************
                         CARI_GET_CARI_KART_TIPI_STRING
***************************************************************************************/

QString CARI_GET_CARI_KART_TIPI_STRING(int p_cari_kart_tipi)
{
    if ( p_cari_kart_tipi EQ ENUM_CARI_ALICI ) {
        return "Alıcı";
    }
    else if ( p_cari_kart_tipi EQ ENUM_CARI_SATICI ) {
        return "Satıcı";
    }
    else if ( p_cari_kart_tipi EQ ENUM_CARI_PERSONEL ) {
        return "Personel";
    }
    else if ( p_cari_kart_tipi EQ ENUM_FIRMA_ORTAGI ) {
        return "Firma Ortağı";
    }
    return "NULL";
}

/**************************************************************************************
                         CARI_GET_CARI_HESAP_ID_LIST
***************************************************************************************/

QList < int > CARI_GET_CARI_HESAP_ID_LIST ( int p_program_id, int p_modul_id, QList < int > p_base_record_id_list )
{
    SQL_QUERY query( DB );
    QList < int > cari_hesap_id_list;

    for (int i = 0; i < p_base_record_id_list.size(); ++i) {

        query.PREPARE_SELECT( "car_hesaplar", "cari_hesap_id    ",
                             "base_record_id = :base_record_id  "
                             "AND program_id = :program_id      "
                             "AND modul_id   = :modul_id        ");
        query.SET_VALUE     ( ":base_record_id", p_base_record_id_list.at( i ) );
        query.SET_VALUE     ( ":program_id"    , p_program_id );
        query.SET_VALUE     ( ":modul_id"      , p_modul_id   );

        if ( query.SELECT() NE 0 ) {
            query.NEXT();

            cari_hesap_id_list<< query.VALUE(0).toInt();
        }
    }

    return cari_hesap_id_list;
}

/**************************************************************************************
                         CARI_GET_SM_SICIL_NUMARASI
***************************************************************************************/

QString CARI_GET_SM_SICIL_NUMARASI(int p_cari_hesap_id)
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_hesaplar", "sm_sicil_numarasi", "cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE     ( ":cari_hesap_id", p_cari_hesap_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE( 0 ).toString();
}

/**************************************************************************************
                         CARI_UPDATE_SM_SICIL_NUMARASI
***************************************************************************************/

void CARI_UPDATE_SM_SICIL_NUMARASI(int p_cari_hesap_id, QString p_sm_sicil_numarasi)
{
    SQL_QUERY query( DB );

    query.PREPARE_UPDATE( "car_hesaplar", "cari_hesap_id", "sm_sicil_numarasi", "cari_hesap_id = :cari_hesap_id");
    query.SET_VALUE     ( ":sm_sicil_numarasi"  , p_sm_sicil_numarasi );
    query.SET_VALUE     ( ":cari_hesap_id"      , p_cari_hesap_id     );

    query.UPDATE();
}

/**************************************************************************************
                         CARI_GET_MUH_FIS_ID_LIST
***************************************************************************************/

int CARI_GET_MUH_FIS_ID_LIST ( int p_program_id, int p_modul_id, int p_base_fis_id )
{

    SQL_QUERY query( DB );
    query.PREPARE_SELECT( "car_fisler", "muh_fis_id",
                          "program_id    = :program_id AND   "
                          "modul_id      = :modul_id AND     "
                          "base_fis_id   = :base_fis_id");

    query.SET_VALUE     ( ":program_id" , p_program_id );
    query.SET_VALUE     ( ":modul_id"   , p_modul_id );
    query.SET_VALUE     ( ":base_fis_id", p_base_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                         CARI_GET_ENT_FIS_MODUL_ID
***************************************************************************************/

int CARI_GET_ENT_FIS_MODUL_ID ( QString p_ent_basligi )
{

    if ( p_ent_basligi EQ QObject::tr("Banka Entegrasyon Fişi") ){
        return BANKA_MODULU;
    }
    else if ( p_ent_basligi EQ QObject::tr("Çek-Senet Entegrasyon Fişi ( Ödeme )") ) {
        return CEKSENET_MODULU;
    }
    else if ( p_ent_basligi EQ QObject::tr("Çek-Senet Entegrasyon Fişi") ) {
        return CEKSENET_MODULU;
    }
    else if ( p_ent_basligi EQ QObject::tr("Fatura Entegrasyon Fişi") ) {
        return FATURA_MODULU;
    }
    else if ( p_ent_basligi EQ QObject::tr("Personel Entegrasyon Fişi") ) {
        return PERSONEL_MODULU;
    }
    else if ( p_ent_basligi EQ QObject::tr("SM Makbuz Entegrasyon Fişi") ) {
        return SMM_MODULU;
    }
    else if ( p_ent_basligi EQ QObject::tr("Diğer Entegrasyon Fişleri") ) {
        return 0;
    }
    else if ( p_ent_basligi EQ QObject::tr("Tüm Entegrasyon Fişleri") ) {
        return -1;
    }
    return -1;
}

/**************************************************************************************
                         CARI_COPY_STRUCT_CARI_2_ADRES
***************************************************************************************/

void CARI_COPY_STRUCT_CARI_2_ADRES(CARI_HESAP_STRUCT *P_CARI_STRUCT, ADRES_HESAP_STRUCT *P_ADRES_STRUCT)
{

    P_ADRES_STRUCT->firma_sahis_adi             = P_CARI_STRUCT->cari_hesap_ismi;
    P_ADRES_STRUCT->dogum_tarihi                = P_CARI_STRUCT->dogum_tarihi;
    P_ADRES_STRUCT->dogum_yeri                  = P_CARI_STRUCT->dogum_yeri;
    P_ADRES_STRUCT->cinsiyet                    = P_CARI_STRUCT->cinsiyet;
    P_ADRES_STRUCT->ogrenim_durumu              = P_CARI_STRUCT->ogrenim_durumu;
    P_ADRES_STRUCT->medeni_hali                 = P_CARI_STRUCT->medeni_hali;
    P_ADRES_STRUCT->kan_grubu                   = P_CARI_STRUCT->kan_grubu;
    P_ADRES_STRUCT->meslek                      = P_CARI_STRUCT->meslek;
    P_ADRES_STRUCT->unvan                       = P_CARI_STRUCT->unvan;
    P_ADRES_STRUCT->kimlik_no                   = P_CARI_STRUCT->kimlik_no;
    P_ADRES_STRUCT->vergi_dairesi               = P_CARI_STRUCT->vergi_dairesi;
    P_ADRES_STRUCT->vergi_no                    = P_CARI_STRUCT->vergi_no;
    P_ADRES_STRUCT->adres_satiri_1              = P_CARI_STRUCT->adres_satiri_1;
    P_ADRES_STRUCT->adres_satiri_2              = P_CARI_STRUCT->adres_satiri_2;
    P_ADRES_STRUCT->adres_satiri_3              = P_CARI_STRUCT->adres_satiri_3;
    P_ADRES_STRUCT->ilce                        = P_CARI_STRUCT->ilce;
    P_ADRES_STRUCT->semt                        = P_CARI_STRUCT->semt;
    P_ADRES_STRUCT->ulke                        = P_CARI_STRUCT->ulke;
    P_ADRES_STRUCT->sehir                       = P_CARI_STRUCT->sehir;
    P_ADRES_STRUCT->posta_kodu                  = P_CARI_STRUCT->posta_kodu;
    P_ADRES_STRUCT->tel_ulke_kodu               = P_CARI_STRUCT->tel_ulke_kodu;
    P_ADRES_STRUCT->tel_alan_kodu               = P_CARI_STRUCT->tel_alan_kodu;
    P_ADRES_STRUCT->telefon                     = P_CARI_STRUCT->telefon;
    P_ADRES_STRUCT->is_tel_ulke_kodu            = P_CARI_STRUCT->is_tel_ulke_kodu;
    P_ADRES_STRUCT->is_tel_alan_kodu            = P_CARI_STRUCT->is_tel_alan_kodu;
    P_ADRES_STRUCT->is_telefonu                 = P_CARI_STRUCT->is_telefonu;
    P_ADRES_STRUCT->cep_tel_ulke_kodu           = P_CARI_STRUCT->cep_tel_ulke_kodu;
    P_ADRES_STRUCT->cep_tel_gsm_kodu            = P_CARI_STRUCT->cep_tel_gsm_kodu;
    P_ADRES_STRUCT->cep_telefonu                = P_CARI_STRUCT->cep_telefonu;
    P_ADRES_STRUCT->fax_ulke_kodu               = P_CARI_STRUCT->fax_ulke_kodu;
    P_ADRES_STRUCT->fax_alan_kodu               = P_CARI_STRUCT->fax_alan_kodu;
    P_ADRES_STRUCT->fax                         = P_CARI_STRUCT->fax;
    P_ADRES_STRUCT->email                       = P_CARI_STRUCT->email;
    P_ADRES_STRUCT->firma_id                    = 0;
    P_ADRES_STRUCT->modul_id                    = P_CARI_STRUCT->modul_id;
    P_ADRES_STRUCT->program_id                  = P_CARI_STRUCT->program_id;
    P_ADRES_STRUCT->base_record_id              = P_CARI_STRUCT->base_record_id;
    P_ADRES_STRUCT->is_telefonu_dahili          = P_CARI_STRUCT->is_telefonu_dahili;
    P_ADRES_STRUCT->adres_kart_turu             = P_CARI_STRUCT->cari_kart_turu;
    P_ADRES_STRUCT->website                     = P_CARI_STRUCT->website;
    P_ADRES_STRUCT->fotograf_logo               = P_CARI_STRUCT->fotograf_logo;
    P_ADRES_STRUCT->yetkili_1                   = P_CARI_STRUCT->yetkili_1;
    P_ADRES_STRUCT->yetkili_1_info              = P_CARI_STRUCT->yetkili_1_info;
    P_ADRES_STRUCT->yetkili_2                   = P_CARI_STRUCT->yetkili_2;
    P_ADRES_STRUCT->yetkili_2_info              = P_CARI_STRUCT->yetkili_2_info;
    P_ADRES_STRUCT->yetkili_3                   = P_CARI_STRUCT->yetkili_3;
    P_ADRES_STRUCT->yetkili_3_info              = P_CARI_STRUCT->yetkili_3_info;
    P_ADRES_STRUCT->firma_sektoru               = P_CARI_STRUCT->firma_sektoru;

 }
