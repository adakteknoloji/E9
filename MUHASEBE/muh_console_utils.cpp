#include <QDate>
#include <QString>
#include "adak_defines.h"
#include "adak_sql.h"
#include "adak_std_utils.h"
#include "muh_console_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "yonetim.h"
#include "adak_utils.h"
#include "e9_enum.h"
 


extern ADAK_SQL                  * DB;

/**************************************************************************************
                   MUH_HESABININ_BAKIYELERINI_ARTTIR
***************************************************************************************/

void MUH_HESABININ_BAKIYELERINI_ARTTIR ( int hesap_id, int gun_no, double borc_tutari, double alacak_tutari, ADAK_SQL * P_DB )
{
    if  ( hesap_id EQ 0 ) {
        return;
    }
    ADAK_SQL * USE_DB = DB;

    if ( P_DB NE NULL ) {
        USE_DB = P_DB;
    }

    double toplam_borc_array          [ E9_ARRAY_SIZE ];
    double toplam_alacak_array        [ E9_ARRAY_SIZE ];

    double toplam_borc;
    double toplam_alacak;

    ROUND (borc_tutari   );
    ROUND (alacak_tutari );

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("muh_hesaplar" , "toplam_borc_array , toplam_alacak_array , parent_id" , "hesap_id =:hesap_id");

    query.SET_VALUE(":hesap_id" , hesap_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    UNPACK_DOUBLE_ARRAY(query.VALUE("toplam_borc_array").toString() , toplam_borc_array , E9_ARRAY_SIZE);

    UNPACK_DOUBLE_ARRAY(query.VALUE("toplam_alacak_array").toString() , toplam_alacak_array , E9_ARRAY_SIZE);


    int parent_id = query.VALUE("parent_id").toInt();

    for ( int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {
        if ( borc_tutari NE 0.00 ) {
            toplam_borc           = toplam_borc_array[i] + borc_tutari;
            toplam_borc_array[i]  = ROUND (toplam_borc );

        }
        if ( alacak_tutari NE 0.00 ) {
            toplam_alacak           = toplam_alacak_array[i] + alacak_tutari;
            toplam_alacak_array [i] = ROUND ( toplam_alacak );
        }
    }



    query.PREPARE_UPDATE("muh_hesaplar","hesap_id" ,
                         "toplam_borc_array , toplam_alacak_array ",
                         "hesap_id = :hesap_id");
    query.SET_VALUE(":toplam_borc_array"        , PACK_DOUBLE_ARRAY(toplam_borc_array        , E9_ARRAY_SIZE));
    query.SET_VALUE(":toplam_alacak_array"      , PACK_DOUBLE_ARRAY(toplam_alacak_array      , E9_ARRAY_SIZE));
    query.SET_VALUE(":hesap_id"                 , hesap_id);
    query.UPDATE();


    if ( parent_id NE 0 ) {

        MUH_HESABININ_BAKIYELERINI_ARTTIR ( parent_id ,gun_no,borc_tutari , alacak_tutari, USE_DB );
    }

}

/**************************************************************************************
                   MUH_HESABININ_BAKIYELERINI_AZALT
***************************************************************************************/

void MUH_HESABININ_BAKIYELERINI_AZALT ( int hesap_id , int gun_no , double borc_tutari ,double alacak_tutari , ADAK_SQL * P_DB )
{
    if ( hesap_id EQ 0 ) {
        return;
    }

    ADAK_SQL * USE_DB = DB;

    if( P_DB NE NULL ) {
        USE_DB = P_DB;
    }

    double toplam_borc_array       [ E9_ARRAY_SIZE ];
    double toplam_alacak_array     [ E9_ARRAY_SIZE ];

    double toplam_borc;
    double toplam_alacak;

    ROUND ( borc_tutari    );
    ROUND ( alacak_tutari  );

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("muh_hesaplar" , "toplam_borc_array , toplam_alacak_array , parent_id" , "hesap_id = :hesap_id");

    query.SET_VALUE(":hesap_id" , hesap_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    UNPACK_DOUBLE_ARRAY(query.VALUE("toplam_borc_array").toString()   , toplam_borc_array   , E9_ARRAY_SIZE);
    UNPACK_DOUBLE_ARRAY(query.VALUE("toplam_alacak_array").toString() , toplam_alacak_array , E9_ARRAY_SIZE);

    int parent_id = query.VALUE("parent_id").toInt();

    for ( int i = gun_no ; i < E9_ARRAY_SIZE ; i++ ) {

        toplam_borc       = toplam_borc_array[i] - borc_tutari;
        toplam_alacak     = toplam_alacak_array[i] - alacak_tutari;

        toplam_borc_array [i]    = ROUND( toplam_borc );
        toplam_alacak_array [i]  = ROUND( toplam_alacak );
    }

    query.PREPARE_UPDATE("muh_hesaplar","hesap_id",
                         "toplam_borc_array ,toplam_alacak_array" ,
                         "hesap_id = :hesap_id");

    query.SET_VALUE(":toplam_borc_array"        , PACK_DOUBLE_ARRAY(toplam_borc_array   , E9_ARRAY_SIZE));
    query.SET_VALUE(":toplam_alacak_array"      , PACK_DOUBLE_ARRAY(toplam_alacak_array , E9_ARRAY_SIZE));
    query.SET_VALUE(":hesap_id"                 , hesap_id);

    query.UPDATE();

    if ( parent_id NE 0 ) {

        MUH_HESABININ_BAKIYELERINI_AZALT ( parent_id ,gun_no,borc_tutari ,alacak_tutari, USE_DB );
    }

}



/**************************************************************************************
                   MUH_HESABI_TALI_HESAP_MI
***************************************************************************************/

int MUH_HESABI_TALI_HESAP_MI ( int hesap_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tali_hesap" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE ( ":hesap_id", hesap_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return 0; // onceden -1 di.onarda tekrar sorgu yapmamak icin
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}



/**************************************************************************************
                   MUH_GET_HESAP_KODU
***************************************************************************************/

QString MUH_GET_HESAP_KODU ( int p_hesap_id )
{
    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      ( ":hesap_id" , p_hesap_id );

    if (sql_query.SELECT() EQ 0 ) {
        return "";
    }
    sql_query.NEXT();
    return  sql_query.VALUE(0).toString();
}

/**************************************************************************************
                   MUH_GET_HESAP_ID
***************************************************************************************/

int MUH_GET_HESAP_ID ( QString hesap_kodu )
{
    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id" , "tam_hesap_kodu = :tam_hesap_kodu");

    sql_query.SET_VALUE      ( ":tam_hesap_kodu", hesap_kodu );

    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return  sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_GET_HESAP_ISMI
***************************************************************************************/

QString MUH_GET_HESAP_ISMI ( int p_hesap_id )
{
    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      ( ":hesap_id", p_hesap_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return "";
    }
    sql_query.NEXT();
    return  sql_query.VALUE(0).toString();
}


/**************************************************************************************
                   MUH_GET_HESAP_ISMI
***************************************************************************************/

QString MUH_GET_HESAP_ISMI ( QString tam_hesap_kodu )
{
    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi" , "tam_hesap_kodu = :tam_hesap_kodu");
    sql_query.SET_VALUE      ( ":tam_hesap_kodu", tam_hesap_kodu );
    if ( sql_query.SELECT() EQ 0 ) {
        return "";
    }
    sql_query.NEXT();
    return  sql_query.VALUE(0).toString();
}

/**************************************************************************************
                   MUH_GET_HESAP_KODU_HESAP_ISMI
***************************************************************************************/

void MUH_GET_HESAP_KODU_HESAP_ISMI ( int hesap_id, QString &tam_hesap_kodu, QString &hesap_ismi )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu, hesap_ismi" , "hesap_id = :hesap_id");

    sql_query.SET_VALUE      ( ":hesap_id", hesap_id );
    if ( sql_query.SELECT() EQ 0 ) {
        tam_hesap_kodu.clear();
        hesap_ismi.clear();
        return;
    }
    sql_query.NEXT();
    tam_hesap_kodu = sql_query.VALUE(0).toString();
    hesap_ismi     = sql_query.VALUE(1).toString();
}

/**************************************************************************************
                   MUH_GET_HESAP_ID_HESAP_ISMI
***************************************************************************************/

int MUH_GET_HESAP_ID_HESAP_ISMI(QString tam_hesap_kodu, int &hesap_id, QString &hesap_ismi)
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id, hesap_ismi" ,
                             "tam_hesap_kodu = :tam_hesap_kodu");

    sql_query.SET_VALUE      ( ":tam_hesap_kodu", tam_hesap_kodu );
    if ( sql_query.SELECT() EQ 0 ) {
        hesap_id = 0;
        hesap_ismi.clear();
        return 0;
    }
    sql_query.NEXT();
    hesap_id       = sql_query.VALUE(0).toInt();
    hesap_ismi     = sql_query.VALUE(1).toString();

    return 1;
}

/**************************************************************************************
                   MUH_GET_KOK_HESAP_ID
***************************************************************************************/

int MUH_GET_KOK_HESAP_ID ( int hesap_id )
{
    SQL_QUERY parent_query ( DB );
    int current_hesap_id = hesap_id;
    int parent_id        = -1;

    while ( parent_id NE 0 ) {
        parent_query.PREPARE_SELECT("muh_hesaplar" , "parent_id" , "hesap_id = :hesap_id");

        parent_query.SET_VALUE      ( ":hesap_id" , current_hesap_id );
        if ( parent_query.SELECT() EQ 0 ) {
            return 0;
        }
        parent_query.NEXT();
        if ( parent_query.VALUE(0).toInt() EQ 0 ) {
            parent_id = 0;
            return current_hesap_id;
        }
        current_hesap_id = parent_query.VALUE(0).toInt();
    }
    return 0;
}


/**************************************************************************************
                   MUH_SIRADAKI_FIS_NO_AL
***************************************************************************************/

int MUH_SIRADAKI_FIS_NO_AL ( QString p_fis_tarihi , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB NE NULL ) {
        USE_DB = P_GELEN_DB;
    }
    else {
        USE_DB = DB;
    }

    SQL_QUERY sql_query ( USE_DB );

    sql_query.PREPARE_SELECT("muh_fisler" , "fis_no" , "fis_tarihi = :fis_tarihi");

    sql_query.SET_VALUE     ( ":fis_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT("fis_no DESC" , 0 , 1 ) EQ 0 ) {
        return 1;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt() + 1;
}



/**************************************************************************************
                   MUH_GET_PARENT_HESAP_ID
***************************************************************************************/

int MUH_GET_PARENT_HESAP_ID ( int hesap_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("muh_hesaplar" , "parent_id" , "hesap_id = :hesap_id");

    query.SET_VALUE      ( ":hesap_id",hesap_id );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_GET_HESAPLAR
***************************************************************************************/

QStringList MUH_GET_HESAPLAR ( const QString p_baslangic_hesabi, const QString p_bitis_hesabi )
{
    SQL_QUERY       query_muh_hesaplar ( DB );

    QStringList     MUH_HESAPLAR;
    QString         muh_hesap;

    QString where_str;

    if ( p_baslangic_hesabi.isEmpty() EQ false AND p_bitis_hesabi.isEmpty() EQ false ) {
        if ( p_baslangic_hesabi EQ p_bitis_hesabi ) {
            where_str.append ( "tam_hesap_kodu LIKE :bas_hesap_kodu ");
        }
        else {
            where_str.append ( "tam_hesap_kodu BETWEEN :bas_hesap_kodu_1 AND :bitis_hesap_kodu_1 ");
            where_str.append ( "OR tam_hesap_kodu LIKE :bitis_hesap_kodu_2 OR tam_hesap_kodu LIKE :bas_hesap_kodu_2"  );
        }
    }


    query_muh_hesaplar.PREPARE_SELECT( "muh_hesaplar" , "hesap_id, tam_hesap_kodu, hesap_ismi, parent_id, hesap_seviyesi, "
                                       "tali_hesap, toplam_borc_array, toplam_alacak_array " , where_str);

    if ( p_baslangic_hesabi.isEmpty() EQ false AND p_bitis_hesabi.isEmpty() EQ false ) {
        if ( p_baslangic_hesabi NE p_bitis_hesabi ) {
            query_muh_hesaplar.SET_VALUE  (  ":bas_hesap_kodu_1"  , p_baslangic_hesabi );
            query_muh_hesaplar.SET_VALUE  (  ":bas_hesap_kodu_2"  , p_baslangic_hesabi+"%" );
            query_muh_hesaplar.SET_VALUE  (  ":bitis_hesap_kodu_1", p_bitis_hesabi         );
            query_muh_hesaplar.SET_VALUE  (  ":bitis_hesap_kodu_2", p_bitis_hesabi+"%"     );
        }
        else {
            query_muh_hesaplar.SET_VALUE  (  ":bas_hesap_kodu"    , p_baslangic_hesabi+"%" );
        }
    }

    if ( query_muh_hesaplar.SELECT("tam_hesap_kodu") EQ 0 ) {
        return QStringList();
    }
    while ( query_muh_hesaplar.NEXT() EQ true ) {

        QString     hesap_id            =   query_muh_hesaplar.VALUE(0).toString();
        QString     tam_hesap_kodu      =   query_muh_hesaplar.VALUE(1).toString();
        QString     hesap_ismi          =   query_muh_hesaplar.VALUE(2).toString();
        QString     parent_id           =   query_muh_hesaplar.VALUE(3).toString();
        QString     hesap_seviyesi      =   query_muh_hesaplar.VALUE(4).toString();
        QString     tali_hesap          =   query_muh_hesaplar.VALUE(5).toString();
        QString     hesap_borc_bakiye;
        QString     hesap_alacak_bakiye;

        hesap_borc_bakiye   = query_muh_hesaplar.VALUE("toplam_borc_array").toString();
        hesap_alacak_bakiye = query_muh_hesaplar.VALUE("toplam_alacak_array").toString();

        muh_hesap.clear();
        muh_hesap.append ( hesap_id + "\t" + tam_hesap_kodu + "\t" + hesap_ismi + "\t" + parent_id + "\t" +
                           hesap_seviyesi + "\t" + tali_hesap + "\t" +
                           hesap_borc_bakiye + "\t" + hesap_alacak_bakiye );

        MUH_HESAPLAR.append ( muh_hesap );

    }
    return MUH_HESAPLAR;
}

/**************************************************************************************
                   MUH_FIND_AND_GET_HESAP_BILGILERI
***************************************************************************************/

QString MUH_FIND_AND_GET_HESAP_BILGILERI ( int hesap_id, QStringList muh_hesaplar, bool kok_hesap )
{
    int parent_id = -1;

    QStringList muh_hesap;

    while ( parent_id NE 0 ) {
        for ( int i = 0 ; i < muh_hesaplar.size() ; i++ ) {
            muh_hesap = muh_hesaplar.at(i).split ( "\t" );
            if ( QVariant(muh_hesap.at(0)).toInt() EQ hesap_id ) {
                 parent_id = QVariant(muh_hesap.at(3)).toInt();
                if ( kok_hesap EQ false OR parent_id EQ 0 ) {
                    return muh_hesaplar.at(i);
                }
                hesap_id = parent_id;
                break;
            }
        }
    }
    return "";
}

/**************************************************************************************
                   MUH_FIND_AND_GET_HESAP_BILGILERI
***************************************************************************************/

QString MUH_FIND_AND_GET_HESAP_BILGILERI( QString tam_hesap_kodu, QStringList muh_hesaplar, bool kok_hesap )
{
    int parent_id = -1;

    QStringList muh_hesap;

    while ( parent_id NE 0 ) {
        for ( int i = 0 ; i < muh_hesaplar.size() ; i++ ) {
            muh_hesap = muh_hesaplar.at(i).split("\t");
            if ( muh_hesap.at(1) EQ tam_hesap_kodu ) {
                parent_id = QVariant ( muh_hesap.at(3) ).toInt();
                if ( kok_hesap EQ false OR parent_id EQ 0 )  {
                    return muh_hesaplar.at(i);
                }
                tam_hesap_kodu = muh_hesap.at(1);
                break;
            }
        }
    }
    return "";
}



/**************************************************************************************
                   MUH_GET_FIS_TURU
***************************************************************************************/

int MUH_GET_FIS_TURU ( int fis_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("muh_fisler" , "fis_turu" , "fis_id = :fis_id");

    query.SET_VALUE      ( ":fis_id", fis_id );
    if ( query.SELECT() EQ 0 ) {
        return -1;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_GET_BORC_ARRAY
***************************************************************************************/

QString MUH_GET_BORC_ARRAY ( int hesap_id )
{
    SQL_QUERY sql_query ( DB );

    double  BORC_BAKIYESI [ E9_ARRAY_SIZE ];

    sql_query.PREPARE_SELECT ( "muh_hesaplar","toplam_borc_array", "hesap_id=:hesap_id" );
    sql_query.SET_VALUE      ( ":hesap_id",   hesap_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return "";
    }

    sql_query.NEXT();

    UNPACK_DOUBLE_ARRAY( sql_query.VALUE(0).toString(), BORC_BAKIYESI, E9_ARRAY_SIZE );

    return ( VIRGUL_EKLE ( QVariant( BORC_BAKIYESI[E9_SON_BAKIYE]).toString() ) );
}

/**************************************************************************************
                   MUH_GET_ALACAK_ARRAY
***************************************************************************************/

QString MUH_GET_ALACAK_ARRAY ( int hesap_id )
{
    SQL_QUERY sql_query ( DB );

    double  ALACAK_BAKIYESI[E9_ARRAY_SIZE];

    sql_query.PREPARE_SELECT ( "muh_hesaplar" ,"toplam_alacak_array" ,"hesap_id=:hesap_id" );
    sql_query.SET_VALUE      ( ":hesap_id",   hesap_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return "";
    }

    sql_query.NEXT();

    UNPACK_DOUBLE_ARRAY ( sql_query.VALUE(0).toString(), ALACAK_BAKIYESI, E9_ARRAY_SIZE );

    return ( VIRGUL_EKLE ( QVariant ( ALACAK_BAKIYESI[E9_SON_BAKIYE ] ).toString() ) );
}



/**************************************************************************************
                   MUH_ALT_HESAPLARIN_TAM_HESAP_KODUNU_GUNCELLE
***************************************************************************************/

void MUH_ALT_HESAPLARIN_TAM_HESAP_KODUNU_GUNCELLE ( int hesap_id, QString tam_hesap_kodu, QString delimeter )
{
    SQL_QUERY            select_query ( DB );
    int                  num_of_rows;

    select_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id,hesap_kodu" , "parent_id = :parent_id");

    select_query.SET_VALUE      ( ":parent_id" , hesap_id );

    num_of_rows = select_query.SELECT();
    if ( num_of_rows EQ 0 ) {
        return;
    }
    for ( int i = 0; i < num_of_rows; i++ ) {
        select_query.NEXT();
        int     alt_hesap_id       = select_query.VALUE(0).toInt();
        QString alt_hesap_kodu     = select_query.VALUE(1).toString();
        QString alt_tam_hesap_kodu = tam_hesap_kodu + delimeter + alt_hesap_kodu;

        SQL_QUERY update_query ( DB );


        update_query.PREPARE_UPDATE ( "muh_hesaplar","hesap_id","tam_hesap_kodu", "hesap_id = :hesap_id");
        update_query.SET_VALUE      ( ":tam_hesap_kodu" , alt_tam_hesap_kodu );
        update_query.SET_VALUE      ( ":hesap_id"       , alt_hesap_id       );
        update_query.UPDATE();
        MUH_ALT_HESAPLARIN_TAM_HESAP_KODUNU_GUNCELLE ( alt_hesap_id, alt_tam_hesap_kodu, delimeter );
    }
}


/**************************************************************************************
                   MUH_CREATE_MUHASEBE_HESAPLARI
***************************************************************************************/

void MUH_CREATE_MUHASEBE_HESAPLARI ()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "muh_hesaplar" ,"hesap_id" );

    if( query.SELECT() NE 0 ) {
        return ;
    }

    const char * ANA_HESAPLAR [][2] = {
                        { "100" , "KASA"},
                        { "101" , "ALINAN ÇEKLER"},
                        { "102" , "BANKALAR"},
                        { "103" , "VERİLEN ÇEKLER VE ÖDEME EMİRLERİ"},
                        { "108" , "DİĞER HAZIR DEĞERLER"},
                        { "110" , "HİSSE SENETLERİ"},
                        { "111" , "ÖZEL KESİM TAHVİL, SENET VE BONOLARI"},
                        { "112" , "KAMU KESİMİ TAHVİL, SENET VE BONOLARI"},
                        { "118" , "DİĞER MENKUL KIYMETLER"},
                        { "119" , "MENKUL KIYMET DEĞER DÜŞÜKLÜĞÜ KARŞILIĞI"},
                        { "120" , "ALICILAR"},
                        { "121" , "ALACAK SENETLERİ"},
                        { "122" , "ALACAK SENETLERİ REESKONTU"},
                        { "124" , "KAZANILMAMIŞ FİNANSAL KİRALAMA GELİRLERİ"},
                        { "126" , "VERİLEN DEPOZİTO VE TEMİNATLAR"},
                        { "127" , "DİĞER TİCARİ ALACAKLAR"},
                        { "128" , "ŞÜPHELİ TİCARİ ALACAKLAR"},
                        { "129" , "ŞÜPHELİ TİCARİ ALACAKLAR KARŞILIĞI"},
                        { "131" , "ORTAKLARDAN ALACAKLAR"},
                        { "132" , "İŞTİRAKLERDEN ALACAKLAR"},
                        { "133" , "BAĞLI ORTAKLIKLARDAN ALACAKLAR"},
                        { "135" , "PERSONELDEN ALACAKLAR"},
                        { "136" , "DİĞER ÇEŞİTLİ ALACAKLAR"},
                        { "137" , "DİĞER ALACAK SENETLERİ REESKONTU"},
                        { "138" , "ŞÜPHELİ DİĞER ALACAKLAR"},
                        { "139" , "ŞÜPHELİ DİĞER ALACAKLAR KARŞILIĞI"},
                        { "150" , "İLK MADDE VE MALZEME"},
                        { "151" , "YARIMAMULLER - ÜRETİM"},
                        { "152" , "MAMULLER"},
                        { "153" , "TİCARİ MALLAR"},
                        { "157" , "DİĞER STOKLAR"},
                        { "158" , "STOK DEĞER DÜŞÜKLÜĞÜ KARŞILIĞI"},
                        { "159" , "VERİLEN SİPARİŞ AVANSLARI"},
                        { "170" , "YILLARA YAYGIN İNŞAAT VE ONARIM MALİYETLERİ"},
                        { "171" , "YILLARA YAYGIN İNŞAAT VE ONARIM MALİYETLERİ"},
                        { "172" , "YILLARA YAYGIN İNŞAAT VE ONARIM MALİYETLERİ"},
                        { "173" , "YILLARA YAYGIN İNŞAAT VE ONARIM MALİYETLERİ"},
                        { "174" , "YILLARA YAYGIN İNŞAAT VE ONARIM MALİYETLERİ"},
                        { "176" , "YILLARA YAYGIN İNŞAAT VE ONARIM MALİYETLERİ"},
                        { "177" , "YILLARA YAYGIN İNŞAAT VE ONARIM MALİYETLERİ"},
                        { "178" , "YILLARA YAYGIN İNŞAAT VE ENFLASYON DÜZELTME HESABI"},
                        { "179" , "TAŞERONLARA VERİLEN AVANSLAR"},
                        { "180" , "GELECEK AYLARA AİT GİDERLER"},
                        { "181" , "GELİR TAHAKKUKLARI"},
                        { "190" , "DEVREDEN KDV"},
                        { "191" , "İNDİRİLECEK KDV"},
                        { "192" , "DİĞER KDV "},
                        { "193" , "PEŞİN ÖDENEN VERGİ VE FONLAR"},
                        { "194" , "İNDİRİLECEK ÖZEL TÜKETİM VERGİSİ"},
                        { "195" , "İŞ AVANSLARI"},
                        { "196" , "PERSONEL AVANSLARI"},
                        { "197" , "SAYIM VE TESELLÜM NOKSANLARI"},
                        { "198" , "DİĞER ÇEŞİTLİ DÖNEN VARLIKLAR"},
                        { "199" , "DİĞER DÖNEN VARLIKLAR KARŞILIĞI"},
                        { "220" , "ALICILAR"},
                        { "221" , "ALACAK SENETLERİ"},
                        { "222" , "ALACAK SENETLERİ REESKONTU"},
                        { "224" , "KAZANILMAMIŞ FİNANSAL KİRALAMA FAİZ GELİRLERİ"},
                        { "226" , "VERİLEN DEPOZİTO VE TEMİNATLAR"},
                        { "229" , "ŞÜPHELİ TİCARİ ALACAKLAR KARŞILIĞI"},
                        { "231" , "ORTAKLARDAN ALACAKLAR"},
                        { "232" , "İŞTİRAKLERDEN ALACAKLAR"},
                        { "233" , "BAĞLI ORTAKLIKLARDAN ALACAKLAR"},
                        { "235" , "PERSONELDEN ALACAKLAR"},
                        { "236" , "DİĞER ÇEŞİTLİ ALACAKLAR"},
                        { "237" , "DİĞER ALACAK SENETLERİ REESKONTU"},
                        { "239" , "ŞÜPHELİ DİĞER ALACAKLAR KARŞILIĞI"},
                        { "240" , "BAĞLI MENKUL KIYMETLER"},
                        { "241" , "BAĞLI MENKUL KIYMETLER DEĞER DÜŞÜKLÜĞÜ KARŞILIĞI"},
                        { "242" , "İŞTİRAKLER"},
                        { "243" , "İŞTİRAKLERE SERMAYE TAAHHÜTLERİ"},
                        { "244" , "İŞTİRAKLER SERMAYE PAYLARI DEĞER DÜŞÜKLÜĞÜ KARŞILIĞI"},
                        { "245" , "BAĞLI ORTAKLIKLAR"},
                        { "246" , "BAĞLI ORTAKLIKLARA SERMAYE TAAHHÜTLERİ"},
                        { "247" , "BAĞLI ORT. SERMAYE PAYLARI DEĞER DÜŞÜKLÜĞÜ KARŞILIĞI"},
                        { "248" , "DİĞER MALİ DURAN VARLIKLAR"},
                        { "249" , "DİĞER MALİ DURAN VARLIKLAR KARŞILIĞI"},
                        { "250" , "ARAZİ VE ARSALAR"},
                        { "251" , "YERALTI VE YERÜSTÜ DÜZENLERİ"},
                        { "252" , "BİNALAR"},
                        { "253" , "MAKİNE, TESİS VE CİHAZLAR"},
                        { "254" , "TAŞITLAR"},
                        { "255" , "DEMİRBAŞLAR"},
                        { "256" , "DİĞER MADDİ DURAN VARLIKLAR"},
                        { "257" , "BİRİKMİŞ AMORTİSMANLAR"},
                        { "258" , "YAPILMAKTA OLAN YATIRIMLAR"},
                        { "259" , "VERİLEN AVANSLAR"},
                        { "260" , "HAKLAR"},
                        { "261" , "ŞEREFİYE"},
                        { "262" , "KURULUŞ VE ÖRGÜTLENME GİDERLERİ"},
                        { "263" , "ARAŞTIRMA VE GELİŞTİRME GİDERLERİ"},
                        { "264" , "ÖZEL MALİYETLER"},
                        { "267" , "DİĞER MADDİ OLMAYAN DURAN VARLIKLAR"},
                        { "268" , "BİRİKMİŞ AMORTİSMANLAR"},
                        { "269" , "VERİLEN AVANSLAR"},
                        { "271" , "ARAMA GİDERLERİ"},
                        { "272" , "HAZIRLIK VE GELİŞTİRME GİDERLERİ"},
                        { "277" , "DİĞER ÖZEL TÜKENMEYE  TABİ VARLIKLAR"},
                        { "278" , "BİRİKMİŞ TÜKENME PAYLARI"},
                        { "279" , "VERİLEN AVANSLAR"},
                        { "280" , "GELECEK YILLARA AİT GİDERLER"},
                        { "281" , "GELİR TAHAKKUKLARI"},
                        { "291" , "GELECEK YILLARDA İNDİRİLECEK KDV"},
                        { "292" , "DİĞER KDV"},
                        { "293" , "GELECEK YILLAR İHTİYACI STOKLAR"},
                        { "294" , "ELDEN ÇIKARILACAK STOKLAR VE MADDİ DURAN VARLIKLAR"},
                        { "295" , "PEŞİN ÖDENEN VERGİ VE FONLAR"},
                        { "297" , "DİĞER ÇEŞİTLİ DURAN VARLIKLAR"},
                        { "298" , "STOK DEĞER DÜŞÜKLÜĞÜ KARŞILIĞI"},
                        { "299" , "BİRİKMİŞ AMORTİSMANLAR"},
                        { "300" , "BANKA KREDİLERİ"},
                        { "301" , "FİNANSAL KİRALAMA İŞLEMLERİNDEN BORÇLAR"},
                        { "302" , "ERTELENMİŞ FİNANSAL KİRALAMA BORÇLANMA MALİYETLERİ"},
                        { "303" , "UZUN VADELİ KREDİLERİN ANAPARA TAKSİTLERİ VE FAİZLERİ"},
                        { "304" , "TAHVİL ANAPARA BORÇ TAKSİT VE FAİZLERİ"},
                        { "305" , "ÇIKARILMIŞ BONOLAR VE SENETLER"},
                        { "306" , "ÇIKARILMIŞ DİĞER MENKUL KIYMETLER"},
                        { "308" , "MENKUL KIYMETLER İHRAÇ FARKI"},
                        { "309" , "DİĞER MALİ BORÇLAR"},
                        { "320" , "SATICILAR"},
                        { "321" , "BORÇ SENETLERİ"},
                        { "322" , "BORÇ SENETLERİ REESKONTU"},
                        { "326" , "ALINAN DEPOZİTO VE TEMİNATLAR"},
                        { "329" , "DİĞER TİCARİ BORÇLAR"},
                        { "331" , "ORTAKLARA BORÇLAR"},
                        { "332" , "İŞTİRAKLERE BORÇLAR"},
                        { "333" , "BAĞLI ORTAKLIKLARA BORÇLAR"},
                        { "335" , "PERSONELE BORÇLAR"},
                        { "336" , "DİĞER ÇEŞİTLİ BORÇLAR "},
                        { "337" , "DİĞER BORÇ SENETLERİ REESKONTU"},
                        { "340" , "ALINAN SİPARİŞ AVANSLARI"},
                        { "349" , "ALINAN DİĞER AVANSLAR"},
                        { "350" , "YILLARA YAYGIN İNŞAAT VE ONARIM HAKEDİŞ BEDELLERİ"},
                        { "351" , "YILLARA YAYGIN İNŞAAT VE ONARIM HAKEDİŞ BEDELLERİ"},
                        { "352" , "YILLARA YAYGIN İNŞAAT VE ONARIM HAKEDİŞ BEDELLERİ"},
                        { "353" , "YILLARA YAYGIN İNŞAAT VE ONARIM HAKEDİŞ BEDELLERİ"},
                        { "354" , "YILLARA YAYGIN İNŞAAT VE ONARIM HAKEDİŞ BEDELLERİ"},
                        { "355" , "YILLARA YAYGIN İNŞAAT VE ONARIM HAKEDİŞ BEDELLERİ"},
                        { "356" , "YILLARA YAYGIN İNŞAAT VE ONARIM HAKEDİŞ BEDELLERİ"},
                        { "357" , "YILLARA YAYGIN İNŞAAT VE ONARIM HAKEDİŞ BEDELLERİ"},
                        { "358" , "YILLARA YAYGIN İNŞAAT ENFLASYON DÜZELTME HESABI"},
                        { "360" , "ÖDENECEK VERGİ VE FONLAR"},
                        { "361" , "ÖDENECEK SOSYAL GÜVENLİK KESİNTİLERİ"},
                        { "362" , "ÖDENECEK DÖNER SERMAYE YÜKÜMLÜLÜKLERİ"},
                        { "368" , "VADESİ GEÇMİŞ, ERTLNMŞ VEYA TAKSİTLNDRLMŞ VRG DĞR YKMLLKLR"},
                        { "369" , "ÖDENECEK DİĞER YÜKÜMLÜLÜKLER"},
                        { "370" , "DÖNEM KARI VERGİ VE DİĞER YASAL YÜKÜMLÜLÜK KARŞILIKLARI"},
                        { "371" , "DÖNEM KARININ PEŞİN ÖDENEN VERGİ VE DİĞER YÜKÜMLÜLÜKLERİ"},
                        { "372" , "KIDEM TAZMİNATI KARŞILIĞI"},
                        { "373" , "MALİYET GİDERLERİ KARŞILIĞI"},
                        { "379" , "DİĞER BORÇ VE GİDER KARŞILIKLARI"},
                        { "380" , "GELECEK AYLARA AİT GELİRLER"},
                        { "381" , "GİDER TAHAKKUKLARI"},
                        { "391" , "HESAPLANAN KDV "},
                        { "392" , "DİĞER KDV"},
                        { "393" , "MERKEZ VE ŞUBELER CARİ HESABI"},
                        { "394" , "HESAPLANAN ÖZEL TÜKETİM VERGİSİ"},
                        { "397" , "SAYIM VE TESELLÜM FAZLALARI"},
                        { "399" , "DİĞER ÇEŞİTLİ YABANCI KAYNAKLAR"},
                        { "400" , "BANKA KREDİLERİ"},
                        { "401" , "FİNANSAL KİRALAMA İŞLEMLERŞNDEN BORÇLAR"},
                        { "402" , "ERTELENMİŞ FİNANSAL KİRALAMA BORÇLANMA MALİYETLERİ"},
                        { "405" , "ÇIKARILMIŞ TAHVİLLER"},
                        { "407" , "ÇIKARILMIŞ DİĞER MENKUL KIYMETLER"},
                        { "408" , "MENKUL KIYMET İHRAÇ FARKI"},
                        { "409" , "DİĞER MALİ BORÇLAR"},
                        { "420" , "SATICILAR"},
                        { "421" , "BORÇ SENETLERİ"},
                        { "422" , "BORÇ SENETLERİ REESKONTU"},
                        { "426" , "ALINAN DEPOZİTO VE TEMİNATLAR"},
                        { "429" , "DİĞER TİCARİ BORÇLAR"},
                        { "431" , "ORTAKLARA BORÇLAR"},
                        { "432" , "İŞTİRAKLERE BORÇLAR"},
                        { "433" , "BAĞLI ORTAKLIKLARA BORÇLAR"},
                        { "436" , "DİĞER ÇEŞİTLİ BORÇLAR "},
                        { "437" , "DİĞER BORÇ SENETLERİ REESKONTU"},
                        { "438" , "KAMUYA OLAN ERTELENMİŞ VEYA TAKSİTLENDİRİLMİŞ BORÇLAR "},
                        { "440" , "ALINAN SİPARİŞ AVANSLARI"},
                        { "449" , "ALINAN DİĞER AVANSLAR"},
                        { "472" , "KIDEM TAZMİNATI KARŞILIĞI"},
                        { "479" , "DİĞER BORÇ VE GİDER KARŞILIKLARI"},
                        { "480" , "GELECEK YILLARA AİT GELİRLER"},
                        { "481" , "GİDER TAHAKKUKLARI"},
                        { "492" , "GELECEK YILLARA ERTELENEN VEYA TERKİN EDİLEN KDV"},
                        { "493" , "TESİSE KATILMA PAYLARI"},
                        { "499" , "DİĞER ÇEŞİTLİ UZUN VADELİ YABANCI KAYNAKLAR"},
                        { "500" , "SERMAYE"},
                        { "501" , "ÖDENMEMİŞ SERMAYE"},
                        { "502" , "SERMAYE DÜZELTMESİ OLUMLU FARKLARI"},
                        { "503" , "SERMAYE DÜZELTMESİ OLUMSUZ FARKLARI"},
                        { "520" , "HİSSE SENETLERİ İHRAÇ PRİMLERİ"},
                        { "521" , "HİSSE SENEDİ İPTAL KARLARI"},
                        { "522" , "MADDİ DURAN VARLIK YENİDEN DEĞERLEME ARTIŞLARI"},
                        { "523" , "İŞTİRAKLER YENİDEN DEĞERLEME ARTIŞLARI"},
                        { "524" , "MALİYET ARTIŞLARI FONU"},
                        { "526" , "BORSADA OLUŞAN DEĞER ARTIŞLARI"},
                        { "527" , "ÖZKAYNAK YÖNTEMİ DEĞER ARTIŞLARI"},
                        { "529" , "DİĞER SERMAYE YEDEKLERİ"},
                        { "540" , "YASAL YEDEKLER"},
                        { "541" , "STATÜ YEDEKLERİ"},
                        { "542" , "OLAĞANÜSTÜ YEDEKLER"},
                        { "543" , "MALİYET ARTIŞ FONU"},
                        { "544" , "SERMAYEYE EKLENECEK İŞTİRAK HİS. VE GMENKUL STŞ KZNÇLR"},
                        { "548" , "DİĞER KAR YEDEKLERİ"},
                        { "549" , "ÖZEL FONLAR"},
                        { "570" , "GEÇMİŞ YILLAR KARLARI"},
                        { "580" , "GEÇMİŞ YILLAR ZARARLARI"},
                        { "590" , "DÖNEM NET KARI"},
                        { "591" , "DÖNEM NET ZARARI"},
                        { "600" , "YURT İÇİ SATIŞLAR"},
                        { "601" , "YURT DIŞI SATIŞLAR"},
                        { "602" , "DİĞER GELİRLER"},
                        { "610" , "SATIŞTAN İADELER"},
                        { "611" , "SATIŞ İSKONTOLARI"},
                        { "612" , "DİĞER İNDİRİMLER"},
                        { "620" , "SATILAN MAMULLER MALİYETİ"},
                        { "621" , "SATILAN TİCARİ MALLAR MALİYETİ"},
                        { "622" , "SATILAN HİZMET MALİYETİ"},
                        { "623" , "DİĞER SATIŞLARIN MALİYETİ"},
                        { "630" , "ARAŞTIRMA VE GELİŞTİRME GİDERLERİ"},
                        { "631" , "PAZARLAMA, SATIŞ VE DAĞITIM GİDERLERİ"},
                        { "632" , "GENEL YÖNETİM GİDERLERİ"},
                        { "640" , "İŞTİRAKLERDEN TEMETTÜ GELİRLERİ"},
                        { "641" , "BAĞLI ORTAKLIKLARDAN TEMETTÜ GELİRLERİ"},
                        { "642" , "FAİZ GELİRLERİ"},
                        { "643" , "KOMİSYON GELİRLERİ"},
                        { "644" , "KONUSU KALMAYAN KARŞILIKLAR"},
                        { "645" , "MENKUL KIYMET SATIŞ KARLARI"},
                        { "646" , "KAMBİYO KARLARI"},
                        { "647" , "REESKONT FAİZ GELİRLERİ"},
                        { "649" , "DİĞER OLAĞAN GELİR VE KARLAR"},
                        { "653" , "KOMİSYON GİDERLERİ"},
                        { "654",  "KARŞILIK GİDERLERİ"},
                        { "655" , "MENKUL KIYMET SATIŞ ZARARLARI"},
                        { "656" , "KAMBİYO ZARARLARI"},
                        { "657" , "REESKONT FAİZ GİDERLERİ"},
                        { "659" , "DİĞER OLAĞAN GİDER VE ZARARLAR"},
                        { "660" , "KISA VADELİ BORÇLANMA GİDERLERİ"},
                        { "661" , "UZUN VADELİ BORÇLANMA GİDERLERİ"},
                        { "671" , "ÖNCEKİ DÖNEM GELİR VE KARLARI"},
                        { "679" , "DİĞER OLAĞANDIŞI GELİR VE KARLAR"},
                        { "680" , "ÇALIŞMAYAN KISIM GİDER VE ZARARLARI"},
                        { "681" , "ÖNCEKİ DÖNEM GİDER VE ZARARLARI"},
                        { "689" , "DİĞER OLAĞAN DIŞI GİDER VE ZARARLAR"},
                        { "690" , "DÖNEM KAR VEYA ZARARI"},
                        { "691" , "DÖNEM NET KARI VERGİ VE DĞR YASAL YÜKÜMLÜLÜK KRŞLKLR"},
                        { "692" , "DÖNEM NET KARI VEYA ZARARI"},
                        { "697" , "YILLARA YAYGIN İNŞAAT ENFLASYON DÜZ. HESABI"},
                        { "698" , "ENFLASYON DÜZELTME HESABI"},
                        { "700" , "MALİYET MUHASEBESİ BAĞLANTI HESABI"},
                        { "701" , "MALİYET MUHASEBESİ YANSITMA HESABI"},
                        { "710" , "DİREKT İLK MADDE VE MALZEME GİDERLERİ"},
                        { "711" , "DİREKT İLK MADDE VE MALZEME GİDERİ YANSITMA HESABI"},
                        { "712" , "DİREKT İLK MADDE VE MALZEME FİYAT FARKI"},
                        { "713" , "DİREKT İLK MADDE VE MALZEME MİKTAR FARKI"},
                        { "720" , "DİREKT İŞÇİLİK GİDERLERİ"},
                        { "721" , "DİREKT İŞÇİLİK GİDERLERİ YANSITMA HESABI"},
                        { "722" , "DİREKT İŞÇİLİK ÜCRET FARKLARI"},
                        { "723" , "DİREKT İŞÇİLİK SÜRE FARKLARI"},
                        { "730" , "GENEL ÜRETİM GİDERLERİ"},
                        { "731" , "GENEL ÜRETİM GİDERLERİ YANSITMA HESABI"},
                        { "732" , "GENEL ÜRETİM GİDERLERİ BÜTÇE FARKLARI"},
                        { "733" , "GENEL ÜRETİM GİDERLERİ VERİMLİLİK FARKLARI"},
                        { "734" , "GENEL ÜRETİM GİDERLERİ KAPASİTE FARKLARI "},
                        { "740" , "HİZMET ÜRETİM MALİYETİ"},
                        { "741" , "HİZMET ÜRETİM MALİYETİ YANSITMA HESABI"},
                        { "742" , "HİZMET ÜRETİM MALİYETİ FARK HESABI"},
                        { "750" , "ARAŞTIRMA VE GELİŞTİRME GİDERLERİ"},
                        { "751" , "ARAŞTIRMA VE GELİŞTİRME GİDERLERİ YANSITMA HESABI"},
                        { "752" , "ARAŞTIRMA VE GELİŞTİRME GİDER FARKLARI"},
                        { "760" , "PAZARLAMA, SATIŞ VE DAĞITIM GİDERLERİ"},
                        { "761" , "PAZARLAMA SATIŞ VE DAĞITIM GİDERLERİ YANSITMA HESABI"},
                        { "762" , "PAZARLAMA SATIŞ VE DAĞITIM GİDERLERİ FARK HESABI"},
                        { "770" , "GENEL YÖNETİM GİDERLERİ"},
                        { "771" , "GENEL YÖNETİM GİDERLERİ YANSITMA HESABI"},
                        { "772" , "GENEL YÖNETİM GİDER FARKLARI HESABI"},
                        { "780" , "FİNANSMAN GİDERLERİ"},
                        { "781" , "FİNANSMAN GİDERİ YANSITMA HESABI"},
                        { "782" , "FİNANSMAN GİDERLERİ FARK HESABI"},
                        { "790" , "İLK MADDE VE MALZEME GİDERLERİ"},
                        { "791" , "İŞÇİ ÜCRET VE GİDERLERİ"},
                        { "792" , "MEMUR ÜCRET VE GİDERLERİ"},
                        { "793" , "DIŞARIDAN SAĞLANAN FAYDA VE HİZMETLER"},
                        { "794" , "ÇEŞİTLİ GİDERLER"},
                        { "795" , "VERGİ, RESİM VE HARÇLAR"},
                        { "796" , "AMORTİSMAN VE TÜKKENME PAYLARI"},
                        { "797",  "FİNANSMAN GİDERLERİ"},
                        { "798" , "GİDER ÇEŞİTLERİ YANSITMA HESABI"},
                        { "799" , "ÜRETİM MALİYETİ HESABI"},
                        { "900" , "TEMİNAT MEKTUBUNDAN ALACAKLAR"},
                        { "901" , "TEMİNAT MEKTUBUNDAN BORÇLAR"},
                        { "910" , "CİROLARIMIZDAN ALACAKLAR"},
                        { "911" , "CİROLARIMIZDAN BORÇLAR"},
                        { "912" , "KEFALETTEN ALACAKLAR"},
                        { "913" , "KEFALETTEN BORÇLAR "},
                        { "914" , "GARANTİLERİMİZDEN ALACAKLAR"},
                        { "915" , "GARANTİLERİMİZDEN BORÇLAR"},
                        { "920" , "KIYMETİMİZİ TEMİNAT ALANLAR"},
                        { "921" , "TEMİNATTAKİ KIYMETLERİMİZ"},
                        { "922" , "KIYMETLERİMİZİ EMANET ALANLAR"},
                        { "923" , "EMANETTEKİ KIYMETLERİMİZ"},
                        { "930" , "TEMİNAT ALINAN KIYMETLER"},
                        { "931" , "TEMİNAT KIYMET VERENLER"},
                        { "932" , "EMANET ALINAN KIYMETLER"},
                        { "933" , "EMANET KIYMET VERENLER"},
                        { "944" , "YATIRIM HARCAMALARI"},
                        { "945" , "YATIRIM HARCAMALARI ALACAKLI HESABI"},
                        { "946" , "YARARLANILACAK YATIRIM İNDİRİMİ"},
                        { "947" , "YARARLANILMIŞ YATIRIM İNDİRİMİ"},
                        { "948" , "YATIRIM İNDİRİMİ ALACAKLI HESABI"},
                        { "950" , "KANUNEN KABUL EDİLMEYEN GDRLER VE MTRHA EKLENECEK TUTARLAR"},
                        { "951" , "MATRAHA EKLENECEK TUTARLAR ALACAKLI HESABI"},
                        { "952" , "VERGİYE TABİ OLMAYAN GLRLER VE MTRHTAN İNDİRİLECEK TUTARLAR"},
                        { "953" , "MATRAHTAN İNDİRİLECEK TUTARLAR ALACAKLI HESABI "},
                        { "960" , "MENKUL KIYMETLER CÜZDANI"},
                        { "961" , "MENKUL KIYMETLER ALACAKLI HESABI"},
                        { "970" , "ÇIKARILACAK MENKUL KIYMETLER"},
                        { "972" , "ÇIKARILMIŞ MENKUL KIYMETLER"},
                        { "973" , "ÇIKARILACAK MENKUL KIYMETLER ALACAKLI HESABI"},
                        { "980" , "YABANCI PARALI MEVCUTLAR"},
                        { "981" , "YABANCI PARALI YÜKÜMLÜLÜKLER" },
                        { "982" , "NET YABANCI PARA VAZİYETİ" },
                        { ""    ,  "" }
                    };

    query.PREPARE_INSERT("muh_hesaplar" , "hesap_id" , "parent_id,hesap_seviyesi,hesap_kodu,tam_hesap_kodu,"
                         "hesap_ismi,tali_hesap");

    int i = 0;

    while ( strcmp ( ANA_HESAPLAR [i] [0] , "" ) > 0) {

        query.SET_VALUE ( ":parent_id",0 );
        query.SET_VALUE ( ":hesap_seviyesi",1 );
        query.SET_VALUE ( ":hesap_kodu",QString::fromUtf8 ( ANA_HESAPLAR [i] [0] ) );
        query.SET_VALUE ( ":tam_hesap_kodu",QString::fromUtf8 ( ANA_HESAPLAR [i] [0] ) );
        query.SET_VALUE ( ":hesap_ismi",QString::fromUtf8 ( ANA_HESAPLAR [i] [1] ) );
        query.SET_VALUE ( ":tali_hesap",1 );
        query.INSERT();
        i++;
    }
}

/**************************************************************************************
                   MUH_GET_FIS_TURLERI_STRING
***************************************************************************************/

QString MUH_GET_FIS_TURLERI_STRING ( int fis_turu )
{
    switch ( fis_turu ) {
        case ENUM_MAHSUP_FISI :
            return QObject::tr ( "Mahsup Fişi" );
        case ENUM_TAHSIL_FISI :
            return QObject::tr ( "Tahsil Fişi" );
        case ENUM_TEDIYE_FISI :
            return QObject::tr ( "Tediye Fişi" );
        case ENUM_ACILIS_FISI :
            return QObject::tr ( "Açılış Fişi" );
        case ENUM_KAPANIS_FISI :
            return QObject::tr ( "Kapanış Fişi" );
        default:
           break;
    };
    return "";
}


/**************************************************************************************
                   MUH_GET_FIS_TURLERI_ENUM
***************************************************************************************/

int MUH_GET_FIS_TURLERI_ENUM ( QString fis_turu )
{
    if ( fis_turu EQ QObject::tr ( "Mahsup Fişi" ) ) {
        return ENUM_MAHSUP_FISI;
    }
    else if ( fis_turu EQ QObject::tr ( "Tahsil Fişi" ) ) {
        return ENUM_TAHSIL_FISI;
    }
    else if ( fis_turu EQ QObject::tr ( "Tediye Fişi" ) ) {
        return ENUM_TEDIYE_FISI;
    }
    else if ( fis_turu EQ QObject::tr ( "Açılış Fişi" ) ) {
        return ENUM_ACILIS_FISI;
    }
    else if ( fis_turu EQ QObject::tr ( "Kapanış Fişi" ) ) {
        return ENUM_KAPANIS_FISI;
    }
    return -1;
}

/**************************************************************************************
                   MUH_GET_ENTEGRASYON_FIS_KAYNAK_MODUL_ENUM
***************************************************************************************/

int MUH_GET_ENTEGRASYON_FIS_KAYNAK_MODUL_ENUM ( QString fis_turu )
{
    if ( fis_turu EQ        QObject::tr ( "Banka Entegrasyon Fişi" ) ) {
        return BANKA_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "Cari Entegrasyon Fişi" ) ) {
        return CARI_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "Çek-Senet Entegrasyon Fişi" ) ) {
        return CEKSENET_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "Sabit Kıymet Entegrasyon Fişi" ) ) {
        return DEMIRBAS_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "Fatura Entegrasyon Fişi" ) ) {
        return FATURA_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "Perakende Entegrasyon Fişi")){
        return PERAKENDE_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "Personel Entegrasyon Fişi")){
        return PERSONEL_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "Stok Entegrasyon Fişi")){
        return STOK_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "İrsaliye Entegrasyon Fişi")){
        return IRSALIYE_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "SM Makbuz Entegrasyon Fişi")) {
        return SMM_MODULU;
    }
    else if ( fis_turu EQ   QObject::tr ( "Diğer Entegrasyon Fişleri") ) {
        return 0;
    }
    else if ( fis_turu EQ   QObject::tr ( "Tüm Entegrasyon Fişleri") ) {
        return -1;
    }

    return -1;
}

/**************************************************************************************
                   MUH_GET_ENTEGRASYON_FIS_KAYNAK_MODUL_STRING
***************************************************************************************/

QString MUH_GET_ENTEGRASYON_FIS_KAYNAK_MODUL_STRING ( int fis_kaynagi )
{
    switch ( fis_kaynagi ) {
        case BANKA_MODULU :
            return QObject::tr ( "Banka Entegrasyon Fişi" );
        case CARI_MODULU :
            return QObject::tr ( "Cari Entegrasyon Fişi" );
        case DEMIRBAS_MODULU :
            return QObject::tr ( "Sabit Kıymet Entegrasyon Fişi" );
        case CEKSENET_MODULU :
            return QObject::tr ( "Çek-Senet Entegrasyon Fişi" );
        case FATURA_MODULU :
            return QObject::tr ( "Fatura Entegrasyon Fişi" );
        case PERAKENDE_MODULU :
            return QObject::tr ( "Perakende Entegrasyon Fişi");
        case PERSONEL_MODULU :
            return QObject::tr ( "Personel Entegrasyon Fişi");
        case STOK_MODULU :
            return QObject::tr ( "Stok Entegrasyon Fişi");
        case IRSALIYE_MODULU :
            return QObject::tr ( "İrsaliye Entegrasyon Fişi");
        case SMM_MODULU  :
            return QObject::tr ( "SM Makbuz Entegrasyon Fişi");
        case 0 :
            return QObject::tr ( "Diğer Entegrasyon Fişleri");
    }
    return "";
}

/**************************************************************************************
                   MUH_GET_ENTEGRASYON_FIS_TURLERI_ENUM()
***************************************************************************************/

int MUH_GET_ENTEGRASYON_FIS_TURLERI_ENUM(QString fis_turu)
{
    if ( fis_turu EQ QObject::tr ( "Banka Entegrasyon Fişi" ) ) {
        return ENUM_BANKA_ENTEGRASYON_FISI;
    }
    else if ( fis_turu EQ QObject::tr ( "Cari Entegrasyon Fişi" ) ) {
        return ENUM_CARI_ENTEGRASYON_FISI;
    }
    else if ( fis_turu EQ QObject::tr ( "Çek-Senet Entegrasyon Fişi" ) ) {
        return ENUM_CEKSENET_ENTEGRASYON_FISI;
    }
    else if ( fis_turu EQ QObject::tr ( "Fatura Entegrasyon Fişi" ) ) {
        return ENUM_FATURA_ENTEGRASYON_FISI;
    }
    else if ( fis_turu EQ QObject::tr( "Perakende Entegrasyon Fişi")){
        return ENUM_PERAKENDE_ENTEGRASYON_FISI;
    }
    else if( fis_turu EQ QObject::tr( "Personel Entegrasyon Fişi") ) {
        return ENUM_PERSONEL_ENTEGRASYON_FISI;
    }
    else if ( fis_turu EQ QObject::tr( "Sabit Kıymet Entegrasyon Fişi")  ) {
        return ENUM_DEMIRBAS_ENTEGRASYON_FISI;
    }
    else if ( fis_turu EQ QObject::tr( "Stok Entegrasyon Fişi")  ) {
        return ENUM_STOK_ENTEGRASYON_FISI;
    }
    else if ( fis_turu EQ QObject::tr( "İrsaliye Entegrasyon Fişi")  ) {
        return ENUM_IRSALIYE_ENTEGRASYON_FISI;
    }
    else if ( fis_turu EQ QObject::tr( "Diğer Entegrasyon Fişleri")  ) {
        return ENUM_DIGER_ENTEGRASYON_FISLER;
    }
    else if ( fis_turu EQ QObject::tr("SM Makbuz Entegrasyon Fişi") ) {
        return ENUM_SM_MAKBUZ_ENTEGRASYON_FISI;
    }


    return -1;
}


/**************************************************************************************
                   MUH_GET_ENTEGRASYON_FIS_TURLERI_STRING()
***************************************************************************************/

QString MUH_GET_MUH_ENTEGRASYON_FIS_TURLERI_STRING(int fis_turu)
{
    switch ( fis_turu ) {
        case ENUM_BANKA_ENTEGRASYON_FISI   :
            return  QObject::tr("Banka Entegrasyon Fişi");
        case ENUM_CARI_ENTEGRASYON_FISI:
            return QObject::tr("Cari Entegrasyon Fişi");
        case ENUM_CEKSENET_ENTEGRASYON_FISI:
            return QObject::tr("Çek-Senet Entegrasyon Fişi");
        case ENUM_FATURA_ENTEGRASYON_FISI:
            return QObject::tr("Fatura Entegrasyon Fişi");
        case ENUM_PERAKENDE_ENTEGRASYON_FISI:
            return QObject::tr("Perakende Entegrasyon Fişi");
        case ENUM_SM_MAKBUZ_ENTEGRASYON_FISI :
            return QObject::tr("SM Makbuz Entegrasyon Fişi");
        default:
            break;

    };
    return "";

}

/**************************************************************************************
                   MUH_CREATE_YANSITMA_HESAPLARI
***************************************************************************************/

void MUH_CREATE_YANSITMA_HESAPLARI()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_yansitma_hesaplari" , "gider_hesabi_id");

    if ( query.SELECT() > 0 ) {
        return;
    }

    struct {
        int gider_hesabi_id;
        int yansitma_hesabi_id;
        int bilanco_gelir_hesabi_id;
    } ONTANIMLI_YANSITMA_HESAPLARI [] = {{ MUH_GET_HESAP_ID("710"), MUH_GET_HESAP_ID("711") , MUH_GET_HESAP_ID("151")},
                                         {MUH_GET_HESAP_ID("720") ,MUH_GET_HESAP_ID("721") ,MUH_GET_HESAP_ID("151")},
                                         {MUH_GET_HESAP_ID("730") ,MUH_GET_HESAP_ID("731") ,MUH_GET_HESAP_ID("151")},
                                         {MUH_GET_HESAP_ID("740") ,MUH_GET_HESAP_ID("741") ,MUH_GET_HESAP_ID("622")},
                                         {MUH_GET_HESAP_ID("750") ,MUH_GET_HESAP_ID("751") ,MUH_GET_HESAP_ID("630")},
                                         {MUH_GET_HESAP_ID("760") ,MUH_GET_HESAP_ID("761") ,MUH_GET_HESAP_ID("631")},
                                         {MUH_GET_HESAP_ID("770") ,MUH_GET_HESAP_ID("771") ,MUH_GET_HESAP_ID("632")},
                                         {MUH_GET_HESAP_ID("780") ,MUH_GET_HESAP_ID("781") ,MUH_GET_HESAP_ID("660")}};


    int order_number = 1024;

    for ( int i = 0 ; i < ONTANIMLI_YANSITMA_HESAPLARI_SAYISI ; i++ ) {

        query.PREPARE_INSERT("muh_yansitma_hesaplari" , "kayit_id" ,"gider_hesabi_id,yansitma_hesabi_id,"
                             "bilanco_gelir_hesabi_id , order_number");

        query.SET_VALUE(":gider_hesabi_id"         , ONTANIMLI_YANSITMA_HESAPLARI[i].gider_hesabi_id);
        query.SET_VALUE(":yansitma_hesabi_id"      , ONTANIMLI_YANSITMA_HESAPLARI[i].yansitma_hesabi_id);
        query.SET_VALUE(":bilanco_gelir_hesabi_id" , ONTANIMLI_YANSITMA_HESAPLARI[i].bilanco_gelir_hesabi_id);
        query.SET_VALUE(":order_number"            , order_number);

        query.INSERT();

        order_number += 1024;

    }
}

/**************************************************************************************
                   MUH_GET_FROM_GELIR_TABLOSU_DONEM_NET_KARI_ZARARI
***************************************************************************************/

void MUH_GET_FROM_GELIR_TABLOSU_DONEM_NET_KARI_ZARARI(double &donem_kari, double &donem_zarari , int gun_no)
{
    donem_kari   = 0;
    donem_zarari = 0;

    QStringList gelir_tablosu_hesap_gruplari_list;

    gelir_tablosu_hesap_gruplari_list << "60" << "61" << "62" << "63" << "64"
                                      << "65" << "66" << "67" << "68" << "69" ;

    QStringList gelir_hesap_gruplari;

    gelir_hesap_gruplari << "60" << "61" << "64" << "67";

    SQL_QUERY query(DB);

    double borc_array[E9_ARRAY_SIZE];

    double alacak_array[E9_ARRAY_SIZE];

    double gelir_bakiyesi = 0;

    double gider_bakiyesi = 0;

    for ( int i = 0 ; i < gelir_tablosu_hesap_gruplari_list.size() ; i++ ) {

        query.PREPARE_SELECT("muh_hesaplar" , "toplam_borc_array,toplam_alacak_array,"
                             "hesap_id,tam_hesap_kodu" , "tam_hesap_kodu LIKE :tam_hesap_kodu "
                             "AND hesap_seviyesi = :hesap_seviyesi");
        query.SET_VALUE(":tam_hesap_kodu" , gelir_tablosu_hesap_gruplari_list.at(i));
        query.SET_VALUE(":hesap_seviyesi" , 1);

        if ( query.SELECT() EQ 0 ) {
            continue;
        }
        while ( query.NEXT() ) {

            UNPACK_DOUBLE_ARRAY(query.VALUE(0).toString() , borc_array , E9_ARRAY_SIZE);
            UNPACK_DOUBLE_ARRAY(query.VALUE(1).toString() , alacak_array , E9_ARRAY_SIZE);

            int hesap_id           = query.VALUE(2).toInt();
            QString tam_hesap_kodu = query.VALUE(3).toString();

            double borc_bakiye   = 0;
            double alacak_bakiye = 0;

            MUH_GET_FROM_GIDER_HESABI_ISLENMEMIS_BAKIYE(hesap_id , borc_bakiye , alacak_bakiye, gun_no);

            double hesap_toplam_borc   = borc_array[gun_no] + borc_bakiye;
            double hesap_toplam_alacak = alacak_array[gun_no] + alacak_bakiye;

            QString grup_kodu = tam_hesap_kodu.at(0);
            grup_kodu.append(tam_hesap_kodu.at(1));

            if ( gelir_hesap_gruplari.contains(grup_kodu ) EQ true ) {
                if ( hesap_toplam_borc > hesap_toplam_alacak ) {
                    gelir_bakiyesi += hesap_toplam_borc - hesap_toplam_alacak;
                }
                else {
                    gelir_bakiyesi += hesap_toplam_alacak - hesap_toplam_borc;
                }
            }
            else {

                if ( grup_kodu NE "69") {
                    if ( hesap_toplam_borc > hesap_toplam_alacak ) {
                        gider_bakiyesi += hesap_toplam_borc - hesap_toplam_alacak;
                    }
                    else {
                        gider_bakiyesi += hesap_toplam_alacak - hesap_toplam_borc;
                    }
                }
                if ( tam_hesap_kodu EQ "691") {

                    //690-691 = 692 hesabinin bakiyesini verir.
                    if ( hesap_toplam_borc > hesap_toplam_alacak ) {
                        gider_bakiyesi -= hesap_toplam_borc - hesap_toplam_alacak;
                    }
                    else {
                        gider_bakiyesi -= hesap_toplam_alacak - hesap_toplam_borc;
                    }
                }
            }
        }
    }

    //690 hesabinin alacagina gelirler , borcunada giderler yazilir.
    //690 hesabi borc bakiye veriyorsa zarar , alacak bakiye veriyorsa kar olusur.

    //690 hesabinin bakiyesi = 691 + 692 hesap bakiyelerinin toplamina esittir.
    //692 donem net kari/zarari hesabinin bakiyesi bilancoya aktarilir.
    //692 hesabi alacak bakiye veriyorsa 591 hesabinin borcuna,borc bakiye veriyorsa 590 hesabinin alacagina yazilir.

    if ( gelir_bakiyesi > gider_bakiyesi ) {
        donem_kari = gelir_bakiyesi - gider_bakiyesi;
    }
    else {
        donem_zarari = gider_bakiyesi - gelir_bakiyesi;
    }


}
/*************************************************************************
       MUH_GET_FROM_GIDER_HESABI_ISLENMEMIS_BAKIYE
**************************************************************************/

void MUH_GET_FROM_GIDER_HESABI_ISLENMEMIS_BAKIYE (int gelir_tablosu_hesabi_id, double &borc_bakiye, double &alacak_bakiye , int gun_no)
{
    //Gelir tablosu hesabina donem sonunda islenecek olan ,uygun  7 ile baslayan gider hesabinda bulunan bakiyeleri dondurur.

    borc_bakiye   = 0.0;
    alacak_bakiye = 0.0;

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_yansitma_hesaplari,muh_hesaplar" , "tam_hesap_kodu, "
                         "toplam_borc_array , toplam_alacak_array ",
                         "bilanco_gelir_hesabi_id = :bilanco_gelir_hesabi_id AND "
                         "muh_hesaplar.hesap_id = muh_yansitma_hesaplari.gider_hesabi_id");

    query.SET_VALUE(":bilanco_gelir_hesabi_id" , gelir_tablosu_hesabi_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    QString bilanco_gelir_hesabi_kodu = query.VALUE(0).toString();

    if ( QVariant(bilanco_gelir_hesabi_kodu.at(0)).toInt() < 6 ) {
        return;
    }

    double borc_array[E9_ARRAY_SIZE];
    double alacak_array[E9_ARRAY_SIZE];

    UNPACK_DOUBLE_ARRAY ( query.VALUE("toplam_borc_array").toString() , borc_array , E9_ARRAY_SIZE);
    UNPACK_DOUBLE_ARRAY ( query.VALUE("toplam_alacak_array").toString() , alacak_array , E9_ARRAY_SIZE);


    double toplam_borc   = borc_array[gun_no];
    double toplam_alacak = alacak_array[gun_no];

    if ( toplam_borc > toplam_alacak ) {
        borc_bakiye   = toplam_borc - toplam_alacak;
        alacak_bakiye = 0.0;
    }
    else {
        alacak_bakiye = toplam_alacak - toplam_borc;
        borc_bakiye   = 0.0;
    }

}

/*************************************************************************
                  CEK_SENET_ALT_HESAPLARINI_DUZELT
**************************************************************************/

void CEK_SENET_ALT_HESAPLARINI_DUZELT()
{

    #define HESAP_SAYISI   5
    struct  {

        int parent_hesap_id;
        int tali_hesap_id;
    } HESAPLAR [] = { {MUH_GET_HESAP_ID("101") , 0},
                      {MUH_GET_HESAP_ID("121") , 0},
                      {MUH_GET_HESAP_ID("221") , 0},
                      {MUH_GET_HESAP_ID("321") , 0},
                      {MUH_GET_HESAP_ID("421") , 0} };

    SQL_QUERY query(DB);
    SQL_QUERY s_query(DB);


    for ( int i = 0 ; i < HESAP_SAYISI ; i++ ) {

        query.PREPARE_SELECT("muh_hesaplar" , "hesap_id , tali_hesap" , "parent_id = :parent_id");

        query.SET_VALUE(":parent_id" , HESAPLAR[i].parent_hesap_id);

        if ( query.SELECT("tam_hesap_kodu ASC" , 0 , 1) EQ 0 ) {
            continue;
        }
        query.NEXT();

        int hesap_id      = query.VALUE(0).toInt();
        int tali_hesap_mi = query.VALUE(1).toInt();

        if ( tali_hesap_mi EQ false ) {

            bool uygun_tali_hesap_bulundu = false;

            for ( ; ; ) {
                s_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id , tali_hesap" , "parent_id = :parent_id");

                s_query.SET_VALUE(":parent_id" , hesap_id);

                if ( s_query.SELECT("tam_hesap_kodu ASC" , 0 , 1) NE 0 ) {

                    s_query.NEXT();

                    int tali_hesap_id = s_query.VALUE(0).toInt();
                    int hesap_tali_mi = s_query.VALUE(1).toInt();

                    if ( hesap_tali_mi EQ false ) {
                        hesap_id = tali_hesap_id;
                        continue;
                    }
                    else {
                        HESAPLAR[i].tali_hesap_id = tali_hesap_id;
                        uygun_tali_hesap_bulundu = true;
                        break;
                    }

                }
                if ( uygun_tali_hesap_bulundu EQ true ) {
                    break;
                }
            }

        }
        else {
           HESAPLAR[i].tali_hesap_id = hesap_id;
        }

    }


    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_satiri_id,hesap_id");

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    while ( query.NEXT() ) {
        int fis_satiri_id    = query.VALUE(0).toInt();
        int hesap_id         = query.VALUE(1).toInt();

        int parent_hesap_id = MUH_GET_KOK_HESAP_ID(hesap_id);

        for ( int i = 0 ; i < HESAP_SAYISI ; i++ ) {

            if ( parent_hesap_id EQ HESAPLAR[i].parent_hesap_id ) {

                DB->START_TRANSACTION();

                s_query.PREPARE_UPDATE("muh_fis_satirlari","fis_satiri_id","hesap_id",
                                       "fis_satiri_id = :fis_satiri_id");
                s_query.SET_VALUE(":hesap_id"      , HESAPLAR[i].tali_hesap_id);
                s_query.SET_VALUE(":fis_satiri_id" , fis_satiri_id);

                s_query.UPDATE();

                DB->COMMIT_TRANSACTION();
                break;
            }
        }
    }
}

/*************************************************************************
                  MUH_GET_TALI_HESAP_ID
**************************************************************************/

int MUH_GET_TALI_HESAP_ID (QString tam_hesap_kodu)
{

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_hesaplar" , "tali_hesap , hesap_id" , "tam_hesap_kodu = :tam_hesap_kodu");

    query.SET_VALUE(":tam_hesap_kodu" , tam_hesap_kodu);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    int tali_hesap_mi   = query.VALUE(0).toInt();
    int parent_hesap_id = query.VALUE(1).toInt();

    if ( tali_hesap_mi EQ 1 ) {
        return parent_hesap_id;
    }

    int hesap_id = 0;

    for ( ; ; ) {

        query.PREPARE_SELECT("muh_hesaplar" , "hesap_id , tali_hesap","parent_id = :parent_id");

        query.SET_VALUE(":parent_id" , parent_hesap_id);

        if ( query.SELECT() EQ 0 ) {
            break;
        }

        query.NEXT();
        hesap_id          = query.VALUE(0).toInt();
        int tali_hesap_mi = query.VALUE(1).toInt();

        if ( tali_hesap_mi EQ 1 ) {
           break;
        }
        parent_hesap_id = hesap_id;
    }

    return hesap_id;

}

/*************************************************************************
                  MUH_UPDATE_MUH_FIS_ACIKLMA
**************************************************************************/

void MUH_UPDATE_FIS_ACIKLAMA ( int p_muh_fis_id, QString p_aciklama )
{
    SQL_QUERY query( DB );

    query.PREPARE_UPDATE("muh_fisler","fis_id","aciklama ","fis_id = :fis_id");
    query.SET_VALUE(":aciklama" , p_aciklama  );
    query.SET_VALUE(":fis_id"   , p_muh_fis_id);
    query.UPDATE();
}

/*************************************************************************
                  MUH_GET_FIS_LAST_ORDER_NUMBER
**************************************************************************/

int MUH_GET_FIS_LAST_ORDER_NUMBER( int p_fis_id , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query( USE_DB );

    query.PREPARE_SELECT("muh_fisler", "last_order_number", "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id );

    int last_order_number = 1024;
    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        last_order_number = query.VALUE(0).toInt();
    }

    return last_order_number;
}

/*************************************************************************
                  MUH_UPDATE_FIS_BASE_FIS_ID
**************************************************************************/

void MUH_UPDATE_FIS_BASE_FIS_ID( int p_muh_fis_id, int p_base_fis_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_UPDATE("muh_fisler","fis_id","base_fis_id ","fis_id = :fis_id");
    query.SET_VALUE(":base_fis_id" , p_base_fis_id  );
    query.SET_VALUE(":fis_id"      , p_muh_fis_id);
    query.UPDATE();

}
/*************************************************************************
                  MUH_HESAPLAR_TABLOSUNU_OKU
**************************************************************************/

int MUH_HESAPLAR_TABLOSUNU_OKU(MUH_HESAP_STRUCT *P_HESAP,int p_hesap_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_hesaplar" , "parent_id, hesap_seviyesi,"
                         "hesap_ismi,tam_hesap_kodu,tali_hesap,alt_hesap_sayisi,"
                         "toplam_borc_array, toplam_alacak_array" ,
                         "hesap_id = :hesap_id");
    query.SET_VALUE(":hesap_id" , p_hesap_id);

    if ( query.SELECT() EQ 0 ) {
        return 0 ;
    }

    query.NEXT();

    P_HESAP->parent_id           = query.VALUE(0).toInt();
    P_HESAP->hesap_seviyesi      = query.VALUE(1).toInt();
    P_HESAP->hesap_ismi          = query.VALUE(2).toString();
    P_HESAP->tam_hesap_kodu      = query.VALUE(3).toString();
    P_HESAP->tali_hesap          = query.VALUE(4).toInt();
    P_HESAP->alt_hesap_sayisi    = query.VALUE(5).toInt();
    P_HESAP->toplam_borc_array   = query.VALUE(6).toString();
    P_HESAP->toplam_alacak_array = query.VALUE(7).toString();

    return 1;

}
/*************************************************************************
                  MUH_HESAPLAR_TABLOSUNU_OKU
**************************************************************************/
int MUH_HESAPLAR_TABLOSUNU_OKU(MUH_HESAP_STRUCT *P_HESAP, QString p_tam_hesap_kodu)
{

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_hesaplar" , "parent_id,hesap_seviyesi,"
                         "hesap_ismi,hesap_id,tali_hesap,alt_hesap_sayisi,"
                         "toplam_borc_array, toplam_alacak_array",
                         "tam_hesap_kodu = :tam_hesap_kodu");
    query.SET_VALUE(":tam_hesap_kodu" , p_tam_hesap_kodu );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    P_HESAP->parent_id           = query.VALUE(0).toInt();
    P_HESAP->hesap_seviyesi      = query.VALUE(1).toInt();
    P_HESAP->hesap_ismi          = query.VALUE(2).toString();
    P_HESAP->hesap_id            = query.VALUE(3).toInt();
    P_HESAP->tali_hesap          = query.VALUE(4).toInt();
    P_HESAP->alt_hesap_sayisi    = query.VALUE(5).toInt();
    P_HESAP->toplam_borc_array   = query.VALUE(6).toString();
    P_HESAP->toplam_alacak_array = query.VALUE(7).toString();

    return 1;
}

/*************************************************************************
                  MUH_CLEAR_HESAP_STRUCT
**************************************************************************/

void MUH_CLEAR_HESAP_STRUCT(MUH_HESAP_STRUCT *P_HESAP)
{
    P_HESAP->alt_hesap_sayisi = 0;
    P_HESAP->toplam_borc_array.clear();
    P_HESAP->toplam_alacak_array.clear();
    P_HESAP->hesap_id = 0;
    P_HESAP->hesap_ismi.clear();
    P_HESAP->hesap_seviyesi = 0;
    P_HESAP->parent_id = 0;
    P_HESAP->tali_hesap = 0;
    P_HESAP->tam_hesap_kodu.clear();

}












