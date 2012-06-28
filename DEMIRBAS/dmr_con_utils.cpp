#include "adak_sql.h"
#include "e9_enum.h"
#include "adak_utils.h"
#include "adak_gruplar.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "fat_fis_utils.h"
#include "fat_console_utils.h"
#include "cari_console_utils.h"
#include "dmr_con_utils.h"
#include "dmr_struct.h"
#include "dmr_enum.h"
#include "fat_struct.h"

extern ADAK_SQL * DB;

DMR_RAPOR_VARS * DMR_RV = new DMR_RAPOR_VARS;

/**************************************************************************************
                   DMR_GET_DEMIRBAS_ID
***************************************************************************************/

int DMR_GET_DEMIRBAS_ID ( QString demirbas_kodu )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_id","demirbas_kodu = :demirbas_kodu");
    select_query.SET_VALUE      ( ":demirbas_kodu" , demirbas_kodu );
    if(select_query.SELECT() EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}


/**************************************************************************************
                   DMR_GET_DEMIRBAS_KODU_ADI
***************************************************************************************/

void GET_DEMIRBAS_KODU_ADI ( int demirbas_id,QString &demirbas_kodu,QString &demirbas_ismi )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_kodu,demirbas_adi ","demirbas_id = :demirbas_id");
    select_query.SET_VALUE      ( ":demirbas_id" , demirbas_id );

    if ( select_query.SELECT() NE 0 ) {
         select_query.NEXT();
         demirbas_kodu = select_query.VALUE(0).toString();
         demirbas_ismi = select_query.VALUE(1).toString();
    }
}


/**************************************************************************************
                   DMR_AMORTISMAN_TUTARINI_ARTTIR
***************************************************************************************/

void DMR_AMORTISMAN_TUTARINI_ARTTIR ( int demirbas_id , int gun_no,double amortisman_tutari )
{
    double amortisman_tutari_array [ E9_ARRAY_SIZE ];

    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","amortisman_tutari_array ","demirbas_id = :demirbas_id");
    select_query.SET_VALUE      ( ":demirbas_id" , demirbas_id );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }
    select_query.NEXT();
    UNPACK_DOUBLE_ARRAY ( select_query.VALUE(0).toString(),amortisman_tutari_array,E9_ARRAY_SIZE );

    for ( int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
         amortisman_tutari_array[i] += amortisman_tutari;
    }
    SQL_QUERY update_query ( DB );

    update_query.PREPARE_UPDATE ( "dmr_demirbaslar","demirbas_id","amortisman_tutari_array  ",
                                  "demirbas_id = :demirbas_id" );
    update_query.SET_VALUE ( ":amortisman_tutari_array" , PACK_DOUBLE_ARRAY(amortisman_tutari_array,E9_ARRAY_SIZE));
    update_query.SET_VALUE ( ":demirbas_id"             , demirbas_id );
    update_query.UPDATE();

}

/**************************************************************************************
                   DMR_AMORTISMAN_TUTARINI_AZALT
***************************************************************************************/

void DMR_AMORTISMAN_TUTARINI_AZALT ( int demirbas_id,int gun_no,double amortisman_tutari )
{
    double amortisman_tutari_array [ E9_ARRAY_SIZE ];

    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","amortisman_tutari_array","demirbas_id = :demirbas_id");
    select_query.SET_VALUE      ( ":demirbas_id" , demirbas_id );

    if ( select_query.SELECT() EQ 0  ) {
        return;
    }
    select_query.NEXT();
    UNPACK_DOUBLE_ARRAY ( select_query.VALUE(0).toString(),amortisman_tutari_array,E9_ARRAY_SIZE );

    for ( int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
         amortisman_tutari_array[i] -= amortisman_tutari;
    }
    SQL_QUERY update_query ( DB );

    update_query.PREPARE_UPDATE ( "dmr_demirbaslar","demirbas_id" ,"amortisman_tutari_array",
                                  "demirbas_id = :demirbas_id");
    update_query.SET_VALUE      ( ":amortisman_tutari_array" , PACK_DOUBLE_ARRAY(amortisman_tutari_array,E9_ARRAY_SIZE));
    update_query.SET_VALUE      ( ":demirbas_id"             , demirbas_id );
    update_query.UPDATE();
}

/**************************************************************************************
                   DMR_GET_BIRIKMIS_AMORTISMAN_TUTARI
***************************************************************************************/

double DMR_GET_BIRIKMIS_AMORTISMAN_TUTARI ( int demirbas_id,int gun_no ,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    double birikmis_amortisman_tutari_array [ E9_ARRAY_SIZE ] ;

    SQL_QUERY select_query ( USE_DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","amortisman_tutari_array ","demirbas_id = :demirbas_id");
    select_query.SET_VALUE      ( ":demirbas_id" , demirbas_id );

    if ( select_query.SELECT() EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    UNPACK_DOUBLE_ARRAY ( select_query.VALUE(0).toString(),birikmis_amortisman_tutari_array,E9_ARRAY_SIZE );

    return birikmis_amortisman_tutari_array [ gun_no ];

}

/**************************************************************************************
                   DMR_SIRADAKI_FIS_NO_AL
***************************************************************************************/

int DMR_SIRADAKI_FIS_NO_AL ( QString p_fis_tarihi )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "dmr_fisler","fis_no","fis_tarihi = :fis_tarihi");

    sql_query.SET_VALUE      ( ":fis_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT("fis_no DESC",0,1) EQ 0 ) {
        return 1;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt() + 1;
}

/**************************************************************************************
                   DMR_FIS_NO_KULLANILDI_MI
***************************************************************************************/

bool DMR_FIS_NO_KULLANILDI_MI ( int p_fis_no , QString p_fis_tarihi )
{
    SQL_QUERY select_query ( DB ) ;

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id "
                                  "fis_no = :fis_no AND fis_tarihi = :fis_tarihi" );

    select_query.SET_VALUE      ( ":fis_no" ,       p_fis_no );
    select_query.SET_VALUE      ( ":fis_tarihi",    p_fis_tarihi   );

    if ( select_query.SELECT() EQ 0 ) {
        return false;
    }
    return true;
}
/**************************************************************************************
                   DMR_GET_AMORTISMAN_SEKLI_ENUM
***************************************************************************************/

int DMR_GET_AMORTISMAN_SEKLI_ENUM ( QString amortisman_sekli )
{
    if ( amortisman_sekli EQ QObject::tr ( "Normal Bakiye" ) ) {
         return  NORMAL_BAKIYE;
    }
    return AZALAN_BAKIYE;
}

/**************************************************************************************
                   DMR_GET_AMORTISMAN_SEKLI_STRING
***************************************************************************************/

QString DMR_GET_AMORTISMAN_SEKLI_STRING ( int amortisman_sekli )
{
    if ( amortisman_sekli EQ NORMAL_BAKIYE ) {
         return QObject::tr ( "Normal Bakiye" );
    }
    return QObject::tr ( "Azalan Bakiye" );
}

/**************************************************************************************
                   DMR_CREATE_DEMIRBAS_GRUPLARI
***************************************************************************************/


void DMR_CREATE_DEMIRBAS_GRUPLARI()
{

    if (GRP_GRUP_EKLENDIMI(E9_PROGRAMI,DEMIRBAS_MODULU) EQ true)  {
        return;
    }

    const char * DEMIRBAS_ANA_GRUPLAR[][2] = {
        { "01","BİNALAR" },
        { "02","TESİS VE ARAZİ DÜZENLEMELERİ" },
        { "03","DEMİRBAŞLAR" },
        { "04","BİLGİ SISTEMLERİ" },
        { "05","ELLE VERİ İŞLENEN ARAÇLAR" },
        { "06","TAŞIMA ARAÇLARI" },
        { "07","TARIM" },
        { "08","MADENCİLİK" },
        { "09","PETROL VE GAZ SANAYİ" },
        { "10","İNŞAAT İŞLERİ" },
        { "11","HUBUBAT VE UNLU MAMULLER İMALATI" },
        { "12","ŞEKER VE ŞEKER ÜRÜNLERİ İMALATI " },
        { "13","BİTKİSEL YAĞ VE YAĞ ÜRÜNLERİ İMALATI" },
        { "14","BALIK YAĞI,MEYVE VE SÜT ÜRÜNLERİ İMALATI" },
        { "15","ÇAY SANAYİİNDE KULLANILAN İKTİSADİ KIYMETLER" },
        { "16","ALKOLLÜ VE ALKOLSÜZ İÇECEKLER" },
        { "17","DİĞER GIDA VE BENZERİ ÜRÜNLERİN İMALATI" },
        { "18","ÖZEL İŞLEME ARAÇLARI İLE ÜRETİLEN ÜRÜNLERİN İMALATI" },
        { "19","TÜTÜN VE TÜTÜN MAMULLERİ İMALATI" },
        { "20","MENSUCAT ÜRÜNLERİ İMALATI" },
        { "21","TIBBİ MALZEME VE İLAÇ İMALATI" },
        { "22","OPTİK ALET SANAYİİNDE KULLANILAN İKTİSADİ KIYMETLER" },
        { "23","AĞAÇ ÜRÜNLERİ İMALATI" },
        { "24","BASIN YAYIN VE BAĞLANTILI SEKTÖRLER" },
        { "25","KİMYASAL ÜRÜNLER VE BAĞLANTILI ÜRÜNLERİN İMALATI" },
        { "26","KAUÇUK ÜRÜNLERİ İMALATI" },
        { "27","NİHAİ PLASTİK ÜRÜNLERİ İMALATI" },
        { "28","CAM ÜRÜNLERİ İMALATI" },
        { "29","ÇİMENTO İMALATI" },
        { "30","TAŞ,TOPRAK VE KİLDEN YAPILAN ÜRÜNLERİN İMALATI" },
        { "31","DEMİR-ÇELİK VE METAL ÜRÜNLERİ İMALATI" },
        { "32","MADENİ EŞYA VE MAKİNE SANAYİ" },
        { "33","ELEKTRONİK İŞLEMLERE DAYALI İMALAT VE MAKİNE İMALATI" },
        { "34","MOTORLU ARAÇLARIN İMALATI" },
        { "35","HAVACILIKTA KULLANILAN MAMÜLLERİN İMALATI" },
        { "36","GEMİ VE BOT YAPIMI" },
        { "37","DEMİRYOLLARI MAKİNELERİ,ARAÇ VE GEREÇLERİ" },
        { "38","DEMİRYOLU VAGONLARININ İMALATI" },
        { "39","LOKOMOTİF İMALATI" },
        { "40","DEMİRYOLU RAYLARI" },
        { "41","MADALYA,MÜCEVHERAT VE OYUNCAK İMALATI" },
        { "42","BORU HATTI ARACILIĞI İLE TAŞIMACILIK" } ,
        { "43","TELEFONLA VERİLEN İLETİŞİM HİZMETLERİ" },
        { "44","MEDYA VE İLETİŞİM HİZMETLERİ" } ,
        { "45","ELEKTRİK PİYASASI FAALİYETLERİ" },
        { "46","SUYUN ELDE EDİLMESİ,ARITILMASI VE DAĞITILMASI HİZMETLERİ" },
        { "47","DOĞAL GAZ VE GAZ ÜRETİMİ HİZMETLERİ" },
        { "48","BUHARIN ÜRETİM VE DAĞITIMI" },
        { "49","ATIKLARIN AYRIŞTIRILMASININ İKTİSADİ KIYMETLER" },
        { "50","BELEDİYE ATIK SU ARITMA ARAÇLARI" },
        { "51","BELEDİYEYE AİT KANALİZASYONLAR" } ,
        { "52","GERÇEKLEŞTİRİLEN İŞ VE HİZMETLER" },
        { "53","PETROL VE PETROL ÜRÜNLERİ DAĞİTİM VE PAZARLAMA" },
        { "54","İLK TESİS TAAZZUV GİDERLERİ" },
        { "55","GAYRİ MADDİ İKTİSADİ KIYMETLER" },
        { "56","ÖZEL MALİYETLER" },
        { "57","ARAŞTIRMA VE GELİŞTİRME HARCAMALARI" },
        { "58","KONAKLAMA İŞLETMELERİNDE KULLANILAN İKTİSADİ KIYMETLER" },
        { "59","TURİSTİK TESİSLER VE EĞLENCE FAALİYETLERİ" },
        { "60","TİYATRO DEKORLARI" },
        { "61","SUDA YAPILAN FAALİYETLER" },
        { "62","BERBER VE GÜZELLİK SALONLARI İKTİSADİ KIYMETLERİ" },
        { "63","İZOLASYON MALZEMELERİ İMALATI İKTİSADİ KIYMETLERİ" },
        { "64","STERİLİZASYON İŞLEMLERİ İKTİSADİ KIYMETLERİ" },
        { "65","SAVUNMA SANAYİ" },
        { "66","DENİZCİLİK SEKTÖRÜ İKTİSADİ KIYMETLERİ" },
        { "67","BİODİZEL VE GLİSERİN ÜRETİMİ İKTİSADİ KIYMETLERİ"},
        { "","" }

    };

    int i = 0;

    DB->START_TRANSACTION();

    while ( strcmp(DEMIRBAS_ANA_GRUPLAR[i][0] , "") > 0 ) {
        ADAK_GRUP_STRUCT grup;

        grup.grup_parent_id = 0;
        grup.program_id     = E9_PROGRAMI;
        grup.modul_id       = DEMIRBAS_MODULU;
        grup.grup_kodu      = QString(DEMIRBAS_ANA_GRUPLAR[i][0]);
        grup.grup_adi       = QString(DEMIRBAS_ANA_GRUPLAR[i][1]);

        GRP_GRUP_EKLE (&grup);

        i++;
    }

    DB->COMMIT_TRANSACTION();

}
/**************************************************************************************
                   DMR_ENTEGRASYON_FISI_OLUSTUR()
***************************************************************************************/

void DMR_ENTEGRASYON_FISI_OLUSTUR(int fatura_id , int demirbas_id , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    FATURA_TEMEL_BILGILER_STRUCT P_FAT;
    FAT_GET_FAT_TEMEL_BILGILER( &P_FAT, fatura_id ,USE_DB);

    int muh_fis_id = P_FAT.muh_fis_id;

    CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;
    MUH_FIS_STRUCT  * MUH_FIS  = new MUH_FIS_STRUCT;

    CARI_FIS->modul_id      = FATURA_MODULU;
    CARI_FIS->program_id    = E9_PROGRAMI;
    CARI_FIS->base_fis_id   = fatura_id;
    CARI_FIS->muh_fis_id    = muh_fis_id;

    MUH_FIS->modul_id       = FATURA_MODULU;
    MUH_FIS->program_id     = E9_PROGRAMI;
    MUH_FIS->base_fis_id    = fatura_id;

    int cari_fis_id = CARI_GET_ENT_FIS_ID ( E9_PROGRAMI, FATURA_MODULU, fatura_id , USE_DB);

    CARI_FISI_SIL ( CARI_FIS, cari_fis_id ,USE_DB);

    MUH_FIS_TUM_SATIRLARINI_SIL(MUH_FIS , muh_fis_id , USE_DB);

    FATURA_FISI_STRUCT * FAT_FISI = new FATURA_FISI_STRUCT;

    FAT_FISI->KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FISI->KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FISI->KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FISI->OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FISI->OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FISI->OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FISI->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];

    FATURA_FIS_BILGILERINI_OKU(FAT_FISI , fatura_id , USE_DB);

    FATURA_FIS_SATIRI_STRUCT * FAT_FIS_SATIRI = new FATURA_FIS_SATIRI_STRUCT;

    QList <int> fat_fis_satiri_id_list = FAT_GET_FATURA_FIS_SATIRI_ID_LIST( fatura_id ,USE_DB);

    if ( fat_fis_satiri_id_list.size() EQ 0 ) {
        return ;
    }
    for ( int i = 0; i < fat_fis_satiri_id_list.size(); i++ ) {

        FATURA_CLEAR_FIS_SATIRI_STRUCT(FAT_FIS_SATIRI);

        int fatura_satiri_id = fat_fis_satiri_id_list.at(i);

        FATURA_SATIR_BILGILERINI_OKU(FAT_FIS_SATIRI , fatura_satiri_id , USE_DB);

        FATURA_FIS_SATIRI_EKLE(FAT_FISI , FAT_FIS_SATIRI , false , fatura_satiri_id ,
                              USE_DB ,demirbas_id );

    }

}
/**************************************************************************************
                   DMR_GET_RAPOR_VARS()
***************************************************************************************/

DMR_RAPOR_VARS * DMR_GET_RAPOR_VARS()
{
    return DMR_RV;
}

/**************************************************************************************
                   DMR_GET_FATURA_BILGILERI
***************************************************************************************/

QString DMR_GET_FATURA_BILGILERI(int fatura_id)
{
    QString fatura_bilgileri;

    int cari_hesap_id = FAT_GET_FATURA_CARI_HESAP_ID( fatura_id );

    QString cari_hesap_kodu, cari_hesap_ismi;

    CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id , &cari_hesap_kodu, &cari_hesap_ismi );

    FATURA_FISI_STRUCT * FAT_FIS = new FATURA_FISI_STRUCT;

    FAT_FIS->KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS->KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS->OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS->OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];


    FATURA_CLEAR_FIS_STRUCT ( FAT_FIS );

    int result = FATURA_FIS_BILGILERINI_OKU( FAT_FIS, fatura_id );

    if ( result NE 0 ) {

        fatura_bilgileri.append( FAT_FIS->belge_seri +
                                 " " + FAT_FIS->belge_numarasi +
                                 " / " + FAT_FIS->fatura_tarihi
                                 + " / " + cari_hesap_kodu + "  " +
                                 cari_hesap_ismi );
    }

    return fatura_bilgileri;
}

/**************************************************************************************
                        DMR_UPDATE_PERSONEL_DEMIRBAS_DURUM
***************************************************************************************/

void DMR_UPDATE_PERSONEL_DEMIRBAS_DURUM(int p_personel_id, int durum)
{
    SQL_QUERY sql_query( DB );

    sql_query.PREPARE_SELECT("dmr_demirbaslar" , "demirbas_id" , "personel_id = :personel_id");
    sql_query.SET_VALUE(":personel_id" , p_personel_id);

    if ( sql_query.SELECT() NE 0 ) {

        SQL_QUERY query(DB);

        while ( sql_query.NEXT() ) {
            int demirbas_id = sql_query.VALUE(0).toInt();

            query.PREPARE_UPDATE("dmr_demirbaslar" , "demirbas_id" , "personel_id" ,
                                 "demirbas_id = :demirbas_id");
            query.SET_VALUE(":personel_id" , durum );
            query.SET_VALUE(":demirbas_id" , demirbas_id);
            query.UPDATE();
        }
    }
}

/**************************************************************************************
                            DMR_GET_DEMIRBAS_BILGILERI
***************************************************************************************/

void DMR_GET_DEMIRBAS_BILGILERI( DMR_TEMEL_BILGILER_STRUCT *P_DMR, int p_demirbas_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("dmr_demirbaslar", "muh_hesap_id , bir_amor_hesap_id,"
                         "demirbasin_yeni_degeri , dmr_satis_kar_hesap_id , "
                         "dmr_satis_zarar_hesap_id",
                         "demirbas_id = :demirbas_id");
    query.SET_VALUE(":demirbas_id" , p_demirbas_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        P_DMR->muh_hesap_id             = query.VALUE(0).toInt();
        P_DMR->bir_amor_hesap_id        = query.VALUE(1).toInt();
        P_DMR->demirbasin_yeni_degeri   = query.VALUE(2).toDouble();
        P_DMR->dmr_satis_kar_hesap_id   = query.VALUE(3).toInt();
        P_DMR->dmr_satis_zarar_hesap_id = query.VALUE(4).toInt();
    }
}
/**************************************************************************************
                  DMR_GET_AYRILACAK_AMORTISMAN_ORANI
***************************************************************************************/

double DMR_GET_AYRILACAK_AMORTISMAN_ORANI(int dmr_alis_month)
{
    switch (dmr_alis_month ) {
        case 1  :
        case 2  :
        case 3  :
            return 25;

        case 4  :
        case 5  :
        case 6  :
            return 50;

        case 7  :
        case 8  :
        case 9  :
            return 75;
        case 10 :
        case 11 :
        case 12 :
            return 100;
        default:
            break;
    }

    return 0;
}
/**************************************************************************************
                   DMR_GET_ID_KODU_ADI_LIST
***************************************************************************************/

QStringList DMR_GET_ID_KODU_ADI_LIST(int personel_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("dmr_demirbaslar" , "demirbas_id,demirbas_kodu,demirbas_adi",
                         "personel_id = :personel_id");
    query.SET_VALUE(":personel_id" , personel_id);

    if ( query.SELECT() EQ 0 ) {
        return QStringList();
    }

    QStringList demirbas_id_kodu_adi_list;

    while ( query.NEXT() ) {

        QString demirbas_id_kodu_adi = query.VALUE(0).toString() + "\t"
                                     + query.VALUE(1).toString() + "\t"
                                     + query.VALUE(2).toString() ;

        demirbas_id_kodu_adi_list << demirbas_id_kodu_adi;

    }
    return demirbas_id_kodu_adi_list;
}












