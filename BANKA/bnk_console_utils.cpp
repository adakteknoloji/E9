#include "adak_sql.h"
#include "adak_utils.h"
#include "muh_console_utils.h"
#include "adak_std_utils.h"
#include "e9_enum.h"
#include "bnk_console_utils.h"
#include "banka_enum.h"
#include "banka_struct.h"

extern ADAK_SQL * DB;

/*****************************************************************************************************
                                        BNK_GET_HESABA_ETKISI_ENUM
*****************************************************************************************************/

int BNK_GET_HESABA_ETKISI_ENUM  ( QString hesaba_etkisi_str )
{
    if (hesaba_etkisi_str EQ "BORCU ARTTIR") {
        return ENUM_BNK_BORC_ARTTIR;
    }
    else if (hesaba_etkisi_str EQ "ALACAGI ARTTIR"){
        return ENUM_BNK_ALACAK_ARTTIR;
    }

    return -1;
}

/*****************************************************************************************************
                                        BNK_GET_HESAP_ETKISI_STRING
*****************************************************************************************************/

QString BNK_GET_HESAP_ETKISI_STRING ( int hesaba_etkisi_enum )
{
    switch(hesaba_etkisi_enum) {
        case ENUM_BNK_BORC_ARTTIR:
            return "BORCU ARTTIR";
        case ENUM_BNK_ALACAK_ARTTIR:
            return "ALACAGI ARTTIR";
    }

    return "";
}

/*****************************************************************************************************
                                        BNK_GET_HESAP_TURU_ENUM
*****************************************************************************************************/

int BNK_GET_HESAP_TURU_ENUM  (QString hesap_tur_adi)
{
    if (hesap_tur_adi EQ "BANKA") {
        return ENUM_BNK_BANKA_HESABI;
    }
    else if (hesap_tur_adi EQ "CARİ") {
        return ENUM_BNK_CARI_HESAP;
    }
    else if (hesap_tur_adi EQ "MUHASEBE") {
        return ENUM_BNK_MUHASEBE_HESABI;
    }
    else if (hesap_tur_adi EQ "POS") {
        return ENUM_BNK_POS_HESABI;
    }
    else if (hesap_tur_adi EQ "KREDİ KARTI"){
        return ENUM_BNK_KREDI_KARTI_HESABI;
    }
    else if(hesap_tur_adi EQ "KASA") {
        return ENUM_BNK_KASA_HESABI;
    }
    else if (hesap_tur_adi EQ "POS KOMİSYON HESABI"){
        return ENUM_BNK_POS_KOMISYON_HESABI;
    }

    return -1;
}

/*****************************************************************************************************
                                        BNK_GET_HESAP_TURU_STRING
*****************************************************************************************************/

QString BNK_GET_HESAP_TURU_STRING(int hesap_tur_enum)
{
    switch(hesap_tur_enum) {
        case ENUM_BNK_BANKA_HESABI:
            return "BANKA";

        case ENUM_BNK_CARI_HESAP:
            return "CARİ";

        case ENUM_BNK_MUHASEBE_HESABI:

            return "MUHASEBE";
        case ENUM_BNK_POS_HESABI  :

            return "POS";
        case ENUM_BNK_KREDI_KARTI_HESABI :

            return "KREDİ KARTI";
        case ENUM_BNK_KASA_HESABI:

            return "KASA";
    }

    return "";
}

/*****************************************************************************************************
                                        BNK_SIRADAKI_FIS_NO_AL
*****************************************************************************************************/

int BNK_SIRADAKI_MAKRO_FIS_NO_AL ( QString p_fis_tarihi )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "bnk_makro_fisler", "fis_no","fis_tarihi = :fis_tarihi");

    sql_query.SET_VALUE ( ":fis_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT("fis_no DESC",0,1) EQ 0 ) {
        return 1;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt() + 1;
}

/*****************************************************************************************************
                                        BNK_SIRADAKI_FIS_NO_AL
*****************************************************************************************************/

int BNK_SIRADAKI_HAREKET_FIS_NO_AL ( QString p_fis_tarihi  ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }


    SQL_QUERY sql_query ( USE_DB );

    sql_query.PREPARE_SELECT ( "bnk_hareket_fisler","fis_no","fis_tarihi = :fis_tarihi");

    sql_query.SET_VALUE ( ":fis_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT("fis_no DESC",0,1) EQ 0 ) {
        return 1;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt() + 1;
}

/*****************************************************************************************************
                                        BNK_SIRADAKI_VIRMAN_FIS_NO_AL
*****************************************************************************************************/

int BNK_SIRADAKI_VIRMAN_FIS_NO_AL ( QString p_fis_tarihi ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY sql_query ( USE_DB );

    sql_query.PREPARE_SELECT ( "bnk_virman_fisler","fis_no","fis_tarihi = :fis_tarihi" );

    sql_query.SET_VALUE ( ":fis_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT("fis_no DESC",0,1) EQ 0 ) {
        return 1;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt() + 1;
}

/*****************************************************************************************************
                                        BNK_CREATE_BANKA_MAKROLARI
*****************************************************************************************************/

void BNK_CREATE_BANKA_MAKROLARI()
{

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_makrolar","makro_id");

    if(select_query.SELECT() NE 0) {
        return;
    }

    struct BANKA_MAKROLARI {
        QString     makro_adi;
        int         hesap_turu;
        int         hesaba_etkisi;
        double      makro_yuzdesi;
        QString     muhasebe_hesap_kodu;
        int         fark_satiri_mi;
        int         round_sekli;


    }
    makrolar []= { {"PARA_YATIRMA"            , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_BORC_ARTTIR    , 100   ,""      , 0,0},
                   {"PARA_YATIRMA"            , ENUM_BNK_KASA_HESABI           , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"PARA ÇEKME"              , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"PARA ÇEKME"              , ENUM_BNK_KASA_HESABI           , ENUM_BNK_BORC_ARTTIR    , 100   ,""      , 0,0},
                   {"GELEN HAVALE"            , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_BORC_ARTTIR    , 100   ,""      , 0,0},
                   {"GELEN HAVALE"            , ENUM_BNK_CARI_HESAP            , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"GİDEN HAVALE"            , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"GİDEN HAVALE"            , ENUM_BNK_CARI_HESAP            , ENUM_BNK_BORC_ARTTIR    , 100   ,""      , 0,0},
                   {"AKBANK-POS BLOKAJ ÇÖZME" , ENUM_BNK_POS_HESABI            , ENUM_BNK_ALACAK_ARTTIR  , 98    ,""      , 0,0},
                   {"AKBANK-POS BLOKAJ ÇÖZME" , ENUM_BNK_POS_HESABI            , ENUM_BNK_ALACAK_ARTTIR  , 2     ,""      , 0,0},
                   {"AKBANK-POS BLOKAJ ÇÖZME" , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_BORC_ARTTIR    , 98    ,""      , 0,0},
                   {"AKBANK-POS BLOKAJ ÇÖZME" , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 2     ,"770"   , 0,0},
                   {"GARANTİ-POS BLOKAJ ÇÖZME", ENUM_BNK_POS_HESABI            , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"GARANTİ-POS BLOKAJ ÇÖZME", ENUM_BNK_BANKA_HESABI          , ENUM_BNK_BORC_ARTTIR    , 100   ,""      , 0,0},
                   {"KREDİ KARTI ÖDEMESİ"     , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"KREDİ KARTI ÖDEMESİ"     , ENUM_BNK_KREDI_KARTI_HESABI    , ENUM_BNK_BORC_ARTTIR    , 100   ,""      , 0,0},
                   {"GELEN EFT"               , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_BORC_ARTTIR    , 100   ,""      , 0,0},
                   {"GELEN EFT"               , ENUM_BNK_CARI_HESAP            , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"GİDEN EFT"               , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"GİDEN EFT"               , ENUM_BNK_CARI_HESAP            , ENUM_BNK_BORC_ARTTIR    , 100   ,""      , 0,0},
                   {"ELEKTRİK ÖDEME"          , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"ELEKTRİK ÖDEME"          , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"740"   , 0,0},
                   {"SU ÖDEME"                , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"SU ÖDEME"                , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"740"   , 0,0},
                   {"TELEFON ÖDEME"           , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"TELEFON ÖDEME"           , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 18    ,""      , 0,0},
                   {"TELEFON ÖDEME"           , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 15    ,""      , 0,0},
                   {"TELEFON ÖDEME"           , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"770"   , 0,0},
                   {"TELEFON ÖDEME"           , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 18    ,"191"   , 0,0},
                   {"TELEFON ÖDEME"           , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 15    ,"770"   , 0,0},
                   {"DOĞALGAZ ÖDEME"          , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"DOĞALGAZ ÖDEME"          , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"740"   , 0,0},
                   {"SSK TAHAKKUKLARI"        , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"SSK TAHAKKUKLARI"        , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"740"   , 0,0},
                   {"VERGİ ÖDEMELERİ"         , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"VERGİ ÖDEMELERİ"         , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"740"   , 0,0},
                   {"MAAŞ ÖDEMELERİ"          , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"MAAŞ ÖDEMELERİ"          , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"740"   , 0,0},
                   {"İŞ YERİ KİRASI"          , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"İŞ YERİ KİRASI"          , ENUM_BNK_CARI_HESAP            , ENUM_BNK_BORC_ARTTIR    , 100   ,""      , 0,0},
                   {"İŞ YERİ KİRASI"          , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"740"   , 0,0},
                   {"İŞ YERİ KİRASI"          , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 25    ,"740"   , 0,0},
                   {"İŞ YERİ KİRASI"          , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_ALACAK_ARTTIR  , 25    ,"360"   , 0,0},
                   {"İŞ YERİ KİRASI"          , ENUM_BNK_CARI_HESAP            , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"TTNET OTOMATİK ÖDEME"    , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"770"   , 1,0},
                   {"TTNET OTOMATİK ÖDEME"    , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 18    ,"191"   , 0,0},
                   {"TTNET OTOMATİK ÖDEME"    , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 5     ,"770"   , 0,0},
                   {"TTNET OTOMATİK ÖDEME"    , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"TTNET OTOMATİK ÖDEME"    , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 18    ,""      , 0,0},
                   {"TTNET OTOMATİK ÖDEME"    , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 5     ,""      , 0,0},
                   {"ELEKTRİK OTOMATİK ÖDEME" , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"770"   , 1,0},
                   {"ELEKTRİK OTOMATİK ÖDEME" , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 18    ,"191"   , 0,0},
                   {"ELEKTRİK OTOMATİK ÖDEME" , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 100   ,""      , 0,0},
                   {"ELEKTRİK OTOMATİK ÖDEME" , ENUM_BNK_BANKA_HESABI          , ENUM_BNK_ALACAK_ARTTIR  , 18    ,""      , 0,0},
                   {"CEP TELEFONU BORÇ ÖDEME" , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 100   ,"770"   , 1,0},
                   {"CEP TELEFONU BORÇ ÖDEME" , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 18    ,"191"   , 0,0},
                   {"CEP TELEFONU BORÇ ÖDEME" , ENUM_BNK_MUHASEBE_HESABI       , ENUM_BNK_BORC_ARTTIR    , 25    ,"770"   , 0,1},
                   {"CEP TELEFONU BORÇ ÖDEME" , ENUM_BNK_KREDI_KARTI_HESABI    , ENUM_BNK_ALACAK_ARTTIR  , 143   ,""      , 0,0}
               };

    //Her yeni makro taniminda banka makrolari satir sayisi arttirilmalidir.

    SQL_QUERY insert_query(DB);

    QString makro_adi;
    int makro_id = 0;
    int order_number = 0;


    for (int i = 0 ; i < BANKA_MAKROLARI_SATIR_SAYISI ; i++) {

        if(makro_adi NE makrolar[i].makro_adi) {

            insert_query.PREPARE_INSERT( "bnk_makrolar","makro_id", "makro_adi");
            insert_query.SET_VALUE( ":makro_adi" , makrolar[i].makro_adi);

            makro_id = insert_query.INSERT();

            makro_adi = makrolar[i].makro_adi;
            order_number = 0;
        }
        order_number += 1024;

        insert_query.PREPARE_INSERT("bnk_makro_satirlari","makro_satiri_id","makro_id, order_number,"
                                    "hesaba_etkisi, hesap_turu, makro_yuzdesi, muh_hesap_id, fark_satiri_mi, round_sekli ");

        insert_query.SET_VALUE(":makro_id"      , makro_id      );
        insert_query.SET_VALUE(":order_number"  , order_number  );
        insert_query.SET_VALUE(":hesaba_etkisi" , makrolar[i].hesaba_etkisi );
        insert_query.SET_VALUE(":hesap_turu"    , makrolar[i].hesap_turu    );
        insert_query.SET_VALUE(":makro_yuzdesi" , makrolar[i].makro_yuzdesi );
        insert_query.SET_VALUE(":muh_hesap_id"  , MUH_GET_HESAP_ID( makrolar[i].muhasebe_hesap_kodu )   );
        insert_query.SET_VALUE(":fark_satiri_mi", makrolar[i].fark_satiri_mi    );
        insert_query.SET_VALUE(":round_sekli"   , makrolar[i].round_sekli       );

        insert_query.INSERT();
    }
}

/*****************************************************************************************************
                                        BNK-GET_POS_ID
*****************************************************************************************************/

int BNK_GET_POS_ID(QString pos_numarasi)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesabin_poslari", "pos_id","pos_numarasi = :pos_numarasi");
    select_query.SET_VALUE(":pos_numarasi" , pos_numarasi);

    if (select_query.SELECT() EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/*****************************************************************************************************
                                        BNK_GET_POS_NUMARASI
*****************************************************************************************************/

QString BNK_GET_POS_NUMARASI(int pos_id)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesabin_poslari","pos_numarasi","pos_id = :pos_id");
    select_query.SET_VALUE(":pos_id" , pos_id);

    if (select_query.SELECT() EQ 0 ) {
        return NULL;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toString();
}

/*****************************************************************************************************
                                        BNK_GET_HESAP_NUMARASI_ID
*****************************************************************************************************/

int BNK_GET_HESAP_NUMARASI_ID(QString banka_hesap_kodu)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id","banka_hesap_kodu = :banka_hesap_kodu");
    select_query.SET_VALUE     (":banka_hesap_kodu"   , banka_hesap_kodu);

    if (select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        return  select_query.VALUE(0).toInt();
    }
    return 0;
}

/*****************************************************************************************************
                                        BNK_GET_HESAP_NUMARASI
*****************************************************************************************************/

QString BNK_GET_HESAP_NUMARASI(int hesap_no_id)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar", "hesap_numarasi","hesap_no_id = :hesap_no_id");
    select_query.SET_VALUE(":hesap_no_id" , hesap_no_id);

    if (select_query.SELECT() NE 0 ) {
       select_query.NEXT();
       return  select_query.VALUE(0).toString();
    }
    return NULL;

}

/*****************************************************************************************************
                                        BNK_GET_KREDI_KARTI_NUMARASI
*****************************************************************************************************/

QString BNK_GET_KREDI_KARTI_NUMARASI(int kredi_karti_id)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari ","kredi_karti_numarasi","kredi_karti_id = :kredi_karti_id");
    select_query.SET_VALUE(":kredi_karti_id" , kredi_karti_id);

    if (select_query.SELECT() EQ 0 ) {
        return NULL;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toString();
}

/*****************************************************************************************************
                                        BANKA_HESAP_BAKIYELERINI_ARTTIR
*****************************************************************************************************/

void BANKA_HESAP_BAKIYELERINI_ARTTIR ( int gun_no ,int hesap_no_id, double borc_tutari , double alacak_tutari,
                                       int bakiye_turu , ADAK_SQL * P_GELEN_DB)
{

    double toplam_borc_array        [ E9_ARRAY_SIZE ];
    double toplam_alacak_array      [ E9_ARRAY_SIZE ];

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }


    SQL_QUERY select_query(USE_DB);
    SQL_QUERY update_query(USE_DB);


    select_query.PREPARE_SELECT("bnk_hesaplar, bnk_pos_arrayler, bnk_kk_arrayler",
                                "toplam_kk_tahsilat_borc_array , toplam_kk_thslat_alacak_array,    "
                                "toplam_kk_odeme_borc_array , toplam_kk_odeme_alacak_array ,                "
                                "toplam_borc_array,toplam_alacak_array,                      "
                                "bnk_hesaplar.hesap_no_id = bnk_pos_arrayler.hesap_no_id              "
                                "AND bnk_hesaplar.hesap_no_id = bnk_kk_arrayler.hesap_no_id                 "
                                "AND bnk_hesaplar.hesap_no_id = :hesap_no_id                                ");

    select_query.SET_VALUE(":hesap_no_id" , hesap_no_id);

    if(select_query.SELECT() EQ 0) {
        return;
    }
    select_query.NEXT();


    switch (bakiye_turu) {
        case ENUM_BNK_BANKA_HESABI :
        {
            UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_borc_array").toString()  , toplam_borc_array  , E9_ARRAY_SIZE );
            UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_alacak_array").toString(), toplam_alacak_array, E9_ARRAY_SIZE );

            for (int i = gun_no ; i < E9_ARRAY_SIZE ; i++) {

                if (borc_tutari NE 0.0) {
                    toplam_borc_array[i] += borc_tutari;
                }
                if ( alacak_tutari NE 0.0) {
                    toplam_alacak_array[i] += alacak_tutari;
                }
            }

            update_query.PREPARE_UPDATE("bnk_hesaplar","hesap_no_id",
                                        "toplam_borc_array,toplam_alacak_array" ,
                                        "hesap_no_id = :hesap_no_id");
            update_query.SET_VALUE(":toplam_borc_array"        , PACK_DOUBLE_ARRAY( toplam_borc_array       , E9_ARRAY_SIZE));
            update_query.SET_VALUE(":toplam_alacak_array"      , PACK_DOUBLE_ARRAY( toplam_alacak_array     , E9_ARRAY_SIZE));
            update_query.SET_VALUE(":hesap_no_id"              , hesap_no_id);

            update_query.UPDATE();

            break;
        }
            //Kredi Kartı ile Ödeme Fişinde ,KK alacak bakiyesinin alacağı artar,banka hesap bakiyesininnde alacağı artar.
        case ENUM_BNK_KREDI_KARTI_HESABI :
        {

                UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_kk_odeme_borc_array").toString()  , toplam_borc_array,   E9_ARRAY_SIZE);
                UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_kk_odeme_alacak_array").toString(), toplam_alacak_array, E9_ARRAY_SIZE);

                for ( int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {

                    if ( borc_tutari NE 0.0) {
                        toplam_borc_array[i] += borc_tutari;
                    }
                    if ( alacak_tutari NE 0.0) {
                        toplam_alacak_array[i] += alacak_tutari;
                    }
                }

                update_query.PREPARE_UPDATE("bnk_kk_arrayler","hesap_no_id","toplam_kk_odeme_borc_array, "
                                            "toplam_kk_odeme_alacak_array " ,
                                            "hesap_no_id = :hesap_no_id");

                update_query.SET_VALUE(":toplam_kk_odeme_borc_array"    , PACK_DOUBLE_ARRAY( toplam_borc_array        , E9_ARRAY_SIZE));
                update_query.SET_VALUE(":toplam_kk_odeme_alacak_array"  , PACK_DOUBLE_ARRAY( toplam_alacak_array      , E9_ARRAY_SIZE));
                update_query.SET_VALUE(":hesap_no_id"            , hesap_no_id);
                update_query.UPDATE();


            break;

        }
        case ENUM_BNK_POS_HESABI         :
        {

                UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_kk_tahsilat_borc_array").toString()   , toplam_borc_array  , E9_ARRAY_SIZE );
                UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_kk_thslat_alacak_array").toString() , toplam_alacak_array, E9_ARRAY_SIZE );

                for (int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
                    if ( alacak_tutari NE 0.0) {
                      toplam_alacak_array[i] += alacak_tutari;
                    }
                    else {
                        toplam_borc_array[i] += borc_tutari;
                    }

                }
                update_query.PREPARE_UPDATE("bnk_pos_arrayler","hesap_no_id","toplam_kk_tahsilat_borc_array,    "
                                            "toplam_kk_thslat_alacak_array ",
                                            "hesap_no_id = :hesap_no_id");

                update_query.SET_VALUE(":toplam_kk_tahsilat_borc_array"    , PACK_DOUBLE_ARRAY( toplam_borc_array    , E9_ARRAY_SIZE));
                update_query.SET_VALUE(":toplam_kk_thslat_alacak_array"  , PACK_DOUBLE_ARRAY( toplam_alacak_array  , E9_ARRAY_SIZE));

                update_query.SET_VALUE(":hesap_no_id"            , hesap_no_id);
                update_query.UPDATE();


            break;
        }
        default :
            break;

    };
}

/*****************************************************************************************************
                                        BANKA_HESAP_BAKIYELERINI_AZALT
*****************************************************************************************************/

void BANKA_HESAP_BAKIYELERINI_AZALT ( int gun_no , int hesap_no_id,double borc_tutari,double alacak_tutari,
                                      int bakiye_turu, ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    if (hesap_no_id EQ 0 ) {
        return;
    };

    double toplam_borc_array        [ E9_ARRAY_SIZE ];
    double toplam_alacak_array      [ E9_ARRAY_SIZE ];


    SQL_QUERY select_query(USE_DB);
    SQL_QUERY update_query(USE_DB);


    select_query.PREPARE_SELECT("bnk_hesaplar , bnk_pos_arrayler , bnk_kk_arrayler",
                                "toplam_kk_tahsilat_borc_array , toplam_kk_thslat_alacak_array,    "
                                "toplam_kk_odeme_borc_array , toplam_kk_odeme_alacak_array ,                "
                                "toplam_borc_array, toplam_alacak_array                     ",
                                "bnk_hesaplar.hesap_no_id = bnk_pos_arrayler.hesap_no_id              "
                                "AND bnk_hesaplar.hesap_no_id = bnk_kk_arrayler.hesap_no_id                 "
                                "AND bnk_hesaplar.hesap_no_id = :hesap_no_id                                ");
    select_query.SET_VALUE(":hesap_no_id" , hesap_no_id);

    if(select_query.SELECT() EQ 0) {
        return;
    }
    select_query.NEXT();


    switch (bakiye_turu) {
        case ENUM_BNK_BANKA_HESABI :
        {

            UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_borc_array").toString()  , toplam_borc_array  , E9_ARRAY_SIZE );
            UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_alacak_array").toString(), toplam_alacak_array, E9_ARRAY_SIZE );

            for (int i = gun_no ; i < E9_ARRAY_SIZE ; i++) {
                if (borc_tutari NE 0.0) {
                    toplam_borc_array[i] -= borc_tutari;
                }
                if ( alacak_tutari NE 0.0) {
                    toplam_alacak_array[i] -= alacak_tutari;
                }
            }

            update_query.PREPARE_UPDATE("bnk_hesaplar","hesap_no_id","toplam_borc_array,   "
                                        "toplam_alacak_array " ,
                                        "hesap_no_id = :hesap_no_id");
            update_query.SET_VALUE(":toplam_borc_array"        , PACK_DOUBLE_ARRAY( toplam_borc_array       , E9_ARRAY_SIZE));
            update_query.SET_VALUE(":toplam_alacak_array"      , PACK_DOUBLE_ARRAY( toplam_alacak_array     , E9_ARRAY_SIZE));
            update_query.SET_VALUE(":hesap_no_id"              , hesap_no_id);

            update_query.UPDATE();

            break;
        }
            //Kredi Kartı ile Ödeme Fişinde ,KK alacak bakiyesinin alacağı artar,banka hesap bakiyesininnde alacağı artar.
        case ENUM_BNK_KREDI_KARTI_HESABI :
        {

                UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_kk_odeme_borc_array").toString()  , toplam_borc_array    , E9_ARRAY_SIZE);
                UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_kk_odeme_alacak_array").toString(), toplam_alacak_array  , E9_ARRAY_SIZE);

                for ( int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {

                    if ( borc_tutari NE 0.0) {
                        toplam_borc_array[i] -= borc_tutari;
                    }
                    if ( alacak_tutari NE 0.0) {
                        toplam_alacak_array[i] -= alacak_tutari;
                    }
                }

                update_query.PREPARE_UPDATE("bnk_kk_arrayler","hesap_no_id", "toplam_kk_odeme_borc_array ,  toplam_kk_odeme_alacak_array" ,
                                            "hesap_no_id = :hesap_no_id");

                update_query.SET_VALUE(":toplam_kk_odeme_borc_array"    , PACK_DOUBLE_ARRAY( toplam_borc_array        , E9_ARRAY_SIZE));
                update_query.SET_VALUE(":toplam_kk_odeme_alacak_array"  , PACK_DOUBLE_ARRAY( toplam_alacak_array      , E9_ARRAY_SIZE));
                update_query.SET_VALUE(":hesap_no_id"            , hesap_no_id);
                update_query.UPDATE();


            break;

        }
        case ENUM_BNK_POS_HESABI         :
        {

                UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_kk_tahsilat_borc_array").toString()   , toplam_borc_array  , E9_ARRAY_SIZE );
                UNPACK_DOUBLE_ARRAY( select_query.VALUE("toplam_kk_thslat_alacak_array").toString() , toplam_alacak_array, E9_ARRAY_SIZE );
                for (int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
                    if ( alacak_tutari NE 0.0) {
                       toplam_alacak_array[i] -= alacak_tutari;
                    }
                    else {
                        toplam_borc_array[i] -= borc_tutari;
                    }

                }
                update_query.PREPARE_UPDATE("bnk_pos_arrayler","hesap_no_id","toplam_kk_tahsilat_borc_array,    "
                                            "toplam_kk_thslat_alacak_array,  " , "hesap_no_id = :hesap_no_id");

                update_query.SET_VALUE(":toplam_kk_tahsilat_borc_array"    , PACK_DOUBLE_ARRAY( toplam_borc_array    , E9_ARRAY_SIZE));
                update_query.SET_VALUE(":toplam_kk_thslat_alacak_array"  , PACK_DOUBLE_ARRAY( toplam_alacak_array  , E9_ARRAY_SIZE));
                update_query.SET_VALUE(":hesap_no_id"            , hesap_no_id);
                update_query.UPDATE();


            break;
        }
        default :
            break;

    };

}

/*****************************************************************************************************
                                        BNK_GET_PARA_BIRIM_ID
*****************************************************************************************************/

int BNK_GET_PARA_BIRIM_ID ( int p_banka_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hesaplar","doviz_id","hesap_no_id = :hesap_no_id " );
    query.SET_VALUE( ":hesap_no_id", p_banka_id );

    if( query.SELECT() EQ 0 )
    {
        return 0;
    }

    query.NEXT();
    return query.VALUE( 0 ).toInt();
}

/*****************************************************************************************************
                                        BNK_GET_BANKA_KODU
*****************************************************************************************************/

QString BNK_GET_BANKA_KODU ( int p_banka_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hesaplar","banka_hesap_kodu","hesap_no_id =:hesap_no_id " );
    query.SET_VALUE( ":hesap_no_id", p_banka_id );

    if( query.SELECT() EQ 0 )
    {
        return 0;
    }

    query.NEXT();
    return query.VALUE( 0 ).toString();

}

/*****************************************************************************************************
                       BNK_GET_BANKA_ADI_SUBE_KODU_ADI_HESABI_NO
*****************************************************************************************************/

QString BNK_GET_BANKA_ADI_SUBE_KODU_ADI_HESABI_NO ( int p_banka_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hesaplar ","banka_ismi, sube_kodu, sube_ismi, hesap_numarasi","hesap_no_id =:hesap_no_id " );
    query.SET_VALUE( ":hesap_no_id", p_banka_id );

    if( query.SELECT() EQ 0 )
    {
        return 0;
    }

    query.NEXT();
    return query.VALUE( 0 ).toString() + " / " + query.VALUE( 1 ).toString() + " " + query.VALUE( 2 ).toString() + " " + query.VALUE( 3 ).toString() ;

}

/*****************************************************************************************************
                                        BNK_SIRADAKI_DEFTER_FIS_NO_AL
*****************************************************************************************************/

int BNK_SIRADAKI_DEFTER_FIS_NO_AL ( QString p_fis_tarihi , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY sql_query ( USE_DB );

    sql_query.PREPARE_SELECT ( "bnk_defter_fisler","fis_no","fis_tarihi = :fis_tarihi");

    sql_query.SET_VALUE ( ":fis_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT("fis_no DESC",0,1) EQ 0 ) {
        return 1;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt() + 1;
}

/*****************************************************************************************************
                  BNK_GET_BANKA_MUH_HESAP_ID
*****************************************************************************************************/

int BNK_GET_BANKA_MUH_HESAP_ID( int p_hesap_no_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "bnk_hesaplar ", "muh_hesap_id", "hesap_no_id =:hesap_no_id " );
    query.SET_VALUE( ":hesap_no_id", p_hesap_no_id );

    if( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE( "muh_hesap_id" ).toInt();
}

/*****************************************************************************************************
                    BNK_GET_BANKA_ISIMLERI
*****************************************************************************************************/

QStringList BNK_GET_BANKA_ISIMLERI( int p_banka_hesap_id )
{
    QStringList banka_list;
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "bnk_hesaplar", "banka_ismi", "" , "banka_ismi" );

    if ( p_banka_hesap_id NE 0 ) {
        select_query.AND_EKLE (" hesap_no_id = :hesap_no_id");
        select_query.SET_VALUE(":hesap_no_id", p_banka_hesap_id );
    }
    if ( select_query.SELECT() NE 0 ) {
        while  ( select_query.NEXT() ) {
            banka_list << select_query.VALUE(0).toString();
        }
    }

    return banka_list;
}

/*****************************************************************************************************
                                        BNK_GET_BANKA_TEMEL_BILGILER
*****************************************************************************************************/

void BNK_GET_BANKA_TEMEL_BILGILER ( BNK_BANKA_BILGILERI_STRUCT *P_BNK, int p_hesap_no_id )
{
    SQL_QUERY select_query( DB );

    select_query.PREPARE_SELECT("bnk_hesaplar",
                                "banka_hesap_kodu,banka_ismi,sube_kodu,sube_ismi,hesap_numarasi, doviz_id ",
                                "hesap_no_id = :hesap_no_id");
    select_query.SET_VALUE(":hesap_no_id" , p_hesap_no_id );

    if ( select_query.SELECT() NE 0 ) {
        select_query.NEXT();

        P_BNK->banka_hesap_kodu = select_query.VALUE(0).toString();
        P_BNK->banka_ismi       = select_query.VALUE(1).toString();
        P_BNK->sube_kodu        = select_query.VALUE(2).toString();
        P_BNK->sube_ismi        = select_query.VALUE(3).toString();
        P_BNK->hesap_numarasi   = select_query.VALUE(4).toString();
        P_BNK->doviz_id         = select_query.VALUE(5).toInt();
    }
}

/*****************************************************************************************************
                               BNK_GET_BANKA_MUH_HESAPLARI
*****************************************************************************************************/

void BNK_GET_BANKA_MUH_HESAPLARI ( BNK_MUH_HESAPLARI_STRUCT *P_BNK_MUH, int p_hesap_no_id )
{
    SQL_QUERY select_query( DB );

    select_query.PREPARE_SELECT("bnk_hesaplar",
                                "verilen_cekler_hesap_id",
                                "hesap_no_id = :hesap_no_id");
    select_query.SET_VALUE(":hesap_no_id" , p_hesap_no_id );

    if ( select_query.SELECT() NE 0 ) {
        select_query.NEXT();

        P_BNK_MUH->verilen_cekler_hesap_id = select_query.VALUE(0).toInt();
    }
}


/*****************************************************************************************************
                                        BNK_GET_ROUND_SEKLI_ENUM
*****************************************************************************************************/

int BNK_GET_ROUND_SEKLI_ENUM ( QString round_sekli )
{
    if ( round_sekli EQ QObject::tr( "Yuvarla" ) ) {
        return ENUM_ROUND;
    }
    else if ( round_sekli EQ QObject::tr( "Yukarı Yuvarla")){
        return ENUM_ROUND_UP;
    }
    else if ( round_sekli EQ QObject::tr( "Aşağı Yuvarla" ) ){
        return ENUM_ROUND_DOWN;
    }
    return 0;
}

/*****************************************************************************************************
                                        BNK_GET_ROUND_SEKLI_STRING
*****************************************************************************************************/

QString BNK_GET_ROUND_SEKLI_STRING( int p_round_sekli)
{
    if ( p_round_sekli EQ ENUM_ROUND ) {
        return QObject::tr( "Yuvarla" );
    }
    else if ( p_round_sekli EQ ENUM_ROUND_UP ){
        return QObject::tr( "Yukarı Yuvarla");
    }
    else if ( p_round_sekli EQ  ENUM_ROUND_DOWN ){
        return QObject::tr( "Aşağı Yuvarla" );
    }
    return 0;
}

/*****************************************************************************************************
                               BNK_GET_FIS_TURLERI_STRING
*****************************************************************************************************/

QString BNK_GET_FIS_TURLERI_STRING(int bnk_fis_turu)
{
    switch(bnk_fis_turu) {

        case ENUM_BANKA_ACILIS_FISI :
            return QObject::tr("AÇILIŞ FİŞİ");
        case ENUM_BANKA_MAKRO_FISI:
            return QObject::tr("MAKRO FİŞİ");
        case ENUM_BANKA_HAREKET_FISI :
            return QObject::tr("HAREKET FİŞİ");
        case ENUM_BANKA_PARA_YAT_CEKME_FISI:
            return QObject::tr("PARA YATIRMA/ÇEKME FİŞİ");
        case ENUM_BANKA_VIRMAN_FISI:
            return QObject::tr("VİRMAN FİŞİ");
        default :
            break;
    }
    return "";
}


/*****************************************************************************************************
                              BNK_GET_FIS_TURLERI_STRING
*****************************************************************************************************/

int BNK_GET_ENT_FIS_MODUL_ID  ( QString p_combo_box_text )
{

    if ( p_combo_box_text EQ QObject::tr ( "Cari Entegrasyon Fişi" ) ) {
        return CARI_MODULU;
    }
    else if ( p_combo_box_text EQ QObject::tr ( "Çek-Senet Entegrasyon Fişi" ) ) {
        return CEKSENET_MODULU;
    }
    else if ( p_combo_box_text EQ QObject::tr ( "Perakende Entegrasyon Fişi" ) ) {
        return PERAKENDE_MODULU;
    }
    else if ( p_combo_box_text EQ QObject::tr ( "Personel Entegrasyon Fişi" ) ) {
        return PERSONEL_MODULU;
    }
    else if ( p_combo_box_text EQ QObject::tr ( "Sm Makbuz Entegrasyon Fişi" ) ) {
        return SMM_MODULU;
    }
    else if ( p_combo_box_text EQ QObject::tr ( "İşletme Entegrasyon Fişleri" ) ) {
        return ISLETME_MODULU;
    }
    else if ( p_combo_box_text EQ QObject::tr ( "Diğer Entegrasyon Fişleri" ) ) {
        return 0;
    }
    else if ( p_combo_box_text EQ QObject::tr ( "Tüm Entegrasyon Fişleri" ) ) {
        return -1;
    }

    return -1;
}



