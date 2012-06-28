#include "adak_sql.h"
#include "adak_utils.h"
#include "cari_enum.h"
#include "cari_console_utils.h"
#include "cek_enum.h"
#include <QDate>

extern ADAK_SQL * DB;


/**************************************************************************************
                   CEK_SIRADAKI_BORDRO_NO_AL
***************************************************************************************/

int CEK_SIRADAKI_BORDRO_NO_AL ( QString p_bordro_tarihi, ADAK_SQL * P_GELEN_DB )
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY sql_query ( USE_DB );

    sql_query.PREPARE_SELECT ( "cek_bordrolar", "bordro_no",
                                     "bordro_tarihi = :bordro_tarihi" );
    sql_query.SET_VALUE            ( ":bordro_tarihi", p_bordro_tarihi );

    if ( sql_query.SELECT( "bordro_no DESC", 0,1 ) EQ 0 ) {
        return 1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt() + 1;
}

/**************************************************************************************
                 CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI
***************************************************************************************/

int CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI    ( QDate vade_tarihi )
{
    return QDate::currentDate().daysTo ( vade_tarihi);
}

/**************************************************************************************
                   CEK_ORTALAMA_GUN_SAYISI
***************************************************************************************/

int CEK_ORTALAMA_GUN_SAYISI  ( int fis_id, int fis_turu, int cek_veya_senet )
{
    SQL_QUERY       select_query ( DB );
    double          satir_tutari;
    int             cek_senet_vadesi         = 0;
    double          toplam_meblag            = 0.0;
    double          vade_carpi_toplam_meblag = 0.0;
    int             ortalama_gun             = 0;

    QList <int > cek_id_list;
    if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI OR fis_turu EQ ENUM_CARI_ODEME_FISI ) {
        cek_id_list = CARI_GET_FIS_SATIRLAR_CEK_SENET_ID_LIST( fis_id, cek_veya_senet );
       }
    else {
        select_query.PREPARE_SELECT ( "cek_bordro_satirlari", "cek_senet_id","bordro_id = :bordro_id");
        select_query.SET_VALUE      ( ":bordro_id",fis_id );

        if ( select_query.SELECT() EQ 0 ) {
            return 0;
        }
        while ( select_query.NEXT() EQ true ) {
            cek_id_list << select_query.VALUE(0).toInt();
        }
    }

    for ( int i = 0; i < cek_id_list.size(); i++ ) {

        int cek_senet_id = cek_id_list.at( i );

        if ( cek_senet_id EQ 0 ) {
            continue;
        }

        SQL_QUERY query_cek_bilgileri ( DB );

        query_cek_bilgileri.PREPARE_SELECT ( "cek_cekler_senetler", "vade_tarihi,cek_senet_tutari",
                                             "cek_senet_id = :cek_senet_id " );
        query_cek_bilgileri.SET_VALUE      ( ":cek_senet_id"    , cek_senet_id );

        if ( query_cek_bilgileri.SELECT() NE 0 ) {
             query_cek_bilgileri.NEXT();

            satir_tutari     = query_cek_bilgileri.VALUE(1).toDouble();
            cek_senet_vadesi = CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI( QDate::fromString(query_cek_bilgileri.VALUE(0).toString(),"yyyy.MM.dd"));

            if  ( cek_senet_vadesi < 0 ) {
                  cek_senet_vadesi = 0;
                  satir_tutari     = 0.0;
            }
            toplam_meblag            += satir_tutari;
            vade_carpi_toplam_meblag += satir_tutari * cek_senet_vadesi;


        }
    }
    if ( vade_carpi_toplam_meblag NE 0.0 AND toplam_meblag NE 0.0 ) {
         ortalama_gun = vade_carpi_toplam_meblag / toplam_meblag;
    }

    return ortalama_gun;
}

/**************************************************************************************
                   CEK_SENET_HAREKETINI_KAYDET
***************************************************************************************/

void CEK_SENET_HAREKETINI_KAYDET ( int cek_id , int islem_turu , QDate islem_tarihi , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    int cekin_hareketi;

    if (islem_turu NE -1 ) {
        switch (islem_turu) {

           case  ENUM_CEK_BNK_TEMINATA_VERILDI       :
                 cekin_hareketi = ENUM_BNK_TEMINAT_VERILDI;
                 break;
            case ENUM_CEK_BNK_TAHSILATA_VERILDI      :
                 cekin_hareketi = ENUM_BNK_TAHSILAT_VERILDI;
                 break;
            case ENUM_CEK_TAHSIL_EDILDI_NKT          :
                 cekin_hareketi = ENUM_TAHSIL_EDILDI_NKT;
                 break;
            case ENUM_CEK_TAHSIL_EDILDI_HESABA     :
                 cekin_hareketi = ENUM_TAHSIL_EDILDI_HESABA;
                 break;
            case ENUM_CEK_SATICIYA_VERILDI                :
                 cekin_hareketi = ENUM_SATICIYA_VERILDI;
                 break;
            case ENUM_CEK_KARSILIKSIZ                :
                 cekin_hareketi = ENUM_KARSILIKSIZ;
                 break;
            case ENUM_CEK_PROTESTOLU                 :
                cekin_hareketi = ENUM_PROTESTOLU;
                break;
            case ENUM_CEK_MUS_IADE_EDILDI            :
                 cekin_hareketi = ENUM_MUS_IADE_EDILDI;
                 break;
            case ENUM_CEK_ODENDI_NKT                 :
                 cekin_hareketi = ENUM_ODENDI_NKT;
                 break;
            case ENUM_CEK_ODENDI_HESAPTAN            :
                 cekin_hareketi = ENUM_ODENDI_HESAPTAN;
                 break;
            case ENUM_CEK_IADE_ALINDI_BANKADAN       :
            case ENUM_CEK_IADE_ALINDI_SATICIDAN     :
                 cekin_hareketi = ENUM_PORTFOYDE;
                 break;
            case ENUM_CEK_SUPHELI_ALACAK             :
                 cekin_hareketi = ENUM_SUPHELI_ALACAK;
                 break;
            case ENUM_CEK_TAHSIL_EDILEMEYEN         :
                 cekin_hareketi = ENUM_TAHSIL_EDILEMEYEN;
                 break;
            case ENUM_CEK_IPTAL                     :
                 cekin_hareketi = ENUM_IPTAL;
                 break;
            default :
                cekin_hareketi = ENUM_PORTFOYDE;
            break;
        };
    }
    else {
        cekin_hareketi = ENUM_PORTFOYDE;
    }

    //! CEKIN SON HAREKETI OKUNUYOR.vE BIT ONCEKI HAREKET OLARAK KAYDEDILIYOR.

    SQL_QUERY query ( USE_DB);

    int cek_senet_onceki_hareket = -1;

    query.PREPARE_SELECT ( "cek_hareketleri", "cek_son_hareketi, hareket_tarihi", "cek_senet_id = :cek_senet_id" );
    query.SET_VALUE      ( ":cek_senet_id", cek_id );

    if ( query.SELECT( "hareket_tarihi DESC" ) NE 0 ) {

        query.NEXT();
        cek_senet_onceki_hareket = query.VALUE(0).toInt();
    }

    query.PREPARE_INSERT ( "cek_hareketleri","cek_hareketi_id", "cek_senet_id, cek_onceki_hareketi, cek_son_hareketi, hareket_tarihi" );
    query.SET_VALUE      ( ":cek_senet_id"        , cek_id );
    query.SET_VALUE      ( ":cek_onceki_hareketi" , cek_senet_onceki_hareket );
    query.SET_VALUE      ( ":cek_son_hareketi"    , cekin_hareketi );
    query.SET_VALUE      ( ":hareket_tarihi"      , islem_tarihi.toString ( "yyyy.MM.dd" ) );

    query.INSERT();
}

/**************************************************************************************
                   CEK_SON_CEK_SENET_HAREKETINI_SIL
***************************************************************************************/

void CEK_SON_CEK_SENET_HAREKETINI_SIL ( int cek_id )
{
    SQL_QUERY select_query ( DB );
    SQL_QUERY delete_query ( DB );

    select_query.PREPARE_SELECT ( "cek_hareketleri", "cek_hareketi_id", "cek_senet_id = :cek_senet_id" );
    select_query.SET_VALUE      ( ":cek_senet_id", cek_id );

    if ( select_query.SELECT( "cek_hareketi_id DESC", 0, 1) NE 0 ) {
         select_query.NEXT();
         int cek_hareketi_id = select_query.VALUE(0).toInt();

        delete_query.PREPARE_DELETE ( "cek_hareketleri", "cek_hareketi_id = :cek_hareketi_id");
        delete_query.SET_VALUE      (  ":cek_hareketi_id"   , cek_hareketi_id );
        delete_query.DELETE();
    }

}

/**************************************************************************************
                   CEK_SON_CEK_SENET_HAREKETINI_BUL
***************************************************************************************/

int CEK_SON_CEK_SENET_HAREKETINI_BUL ( int cek_id )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "cek_hareketleri","cek_son_hareketi","cek_senet_id = :cek_senet_id  ");
    select_query.SET_VALUE      ( ":cek_senet_id" , cek_id );
    if ( select_query.SELECT( "cek_hareketi_id DESC" ) NE 0 ) {
         select_query.NEXT();

        return select_query.VALUE(0).toInt();
    }

    return 0;
}

/**************************************************************************************
                   CEK_SONDAN_BIR_ONCEKI_CEK_SENET_HAREKETINI_BUL
***************************************************************************************/

int CEK_SONDAN_BIR_ONCEKI_CEK_SENET_HAREKETINI_BUL ( int cek_senet_id )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "cek_hareketleri", "cek_onceki_hareketi", "cek_senet_id = :cek_senet_id " );
    select_query.SET_VALUE      ( ":cek_senet_id" , cek_senet_id );

    if ( select_query.SELECT( "cek_hareketi_id DESC", 0, 1 ) NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }
    return 0;
}

/**************************************************************************************
                   CEK_SENET_DURUMUNU_GUNCELLE
***************************************************************************************/

void CEK_SENET_DURUMUNU_GUNCELLE ( int cek_senet_id, int cek_senet_son_durumu , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB = DB;

    if ( P_GELEN_DB NE NULL ) {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY sql_query ( USE_DB );

    sql_query.PREPARE_UPDATE ( "cek_cekler_senetler", "cek_senet_id", "cek_senet_son_durumu ",
                               "cek_senet_id = :cek_senet_id" );

    sql_query.SET_VALUE      ( ":cek_senet_son_durumu", cek_senet_son_durumu );
    sql_query.SET_VALUE      ( ":cek_senet_id"        , cek_senet_id );
    sql_query.UPDATE();
}

/**************************************************************************************
                   CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_ENUM
***************************************************************************************/

int CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_ENUM ( QString cek_senet_islem_turu )
{
    if ( cek_senet_islem_turu EQ QObject::tr ("Bankaya teminat için verildi") ) {
        return ENUM_CEK_BNK_TEMINATA_VERILDI;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Bankaya tahsilat için verildi") ) {
        return ENUM_CEK_BNK_TAHSILATA_VERILDI;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Tahsil edildi - Nakit") ) {
        return ENUM_CEK_TAHSIL_EDILDI_NKT;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Tahsil edildi - Hesaba") ) {
        return ENUM_CEK_TAHSIL_EDILDI_HESABA;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Satıcıya verildi / Ciro edildi") ) {
        return ENUM_CEK_SATICIYA_VERILDI;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Karşılıksız") ) {
        return ENUM_CEK_KARSILIKSIZ;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Protestolu") ) {
        return ENUM_CEK_PROTESTOLU;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Müşteriye iade edildi") ) {
        return ENUM_CEK_MUS_IADE_EDILDI;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Ödendi - Nakit") ) {
        return ENUM_CEK_ODENDI_NKT;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Ödendi - Hesaptan") ) {
        return ENUM_CEK_ODENDI_HESAPTAN;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Tahsil Edilemeyen") ) {
        return ENUM_CEK_TAHSIL_EDILEMEYEN;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Bankadan iade alındı") ) {
        return ENUM_CEK_IADE_ALINDI_BANKADAN;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Satıcıdan iade alındı") ) {
        return ENUM_CEK_IADE_ALINDI_SATICIDAN;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Şüpheli Alacaklar") ) {
        return ENUM_CEK_SUPHELI_ALACAK;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("İptal") ) {
        return ENUM_CEK_IPTAL;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr ("Müşteriden çek / senet alındı") ) {
        return ENUM_CEK_MUSTERIDEN_ALINDI;
    }
    else if ( cek_senet_islem_turu EQ QObject::tr("Tüm Bordrolar")) {
        return ENUM_TUM_BORDROLAR;
    }
    else {
        return -1;
    }
}

/**************************************************************************************
                   CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING
***************************************************************************************/

QString CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING ( int cek_senet_islem_turu )
{
    switch ( cek_senet_islem_turu ) {

        case ENUM_CEK_BNK_TEMINATA_VERILDI :
            return QObject::tr ( "Bankaya teminat için verildi" );

        case ENUM_CEK_BNK_TAHSILATA_VERILDI :
            return QObject::tr ( "Bankaya tahsilat için verildi" );

        case ENUM_CEK_TAHSIL_EDILDI_NKT :
            return QObject::tr ( "Tahsil edildi - Nakit" );

        case  ENUM_CEK_TAHSIL_EDILDI_HESABA :
            return QObject::tr ( "Tahsil edildi - Hesaba" );

        case  ENUM_CEK_SATICIYA_VERILDI :
            return QObject::tr ( "Satıcıya verildi / Ciro edildi" );

        case  ENUM_CEK_KARSILIKSIZ :
            return QObject::tr ( "Karşılıksız" );

        case  ENUM_CEK_PROTESTOLU :
            return QObject::tr ( "Protestolu" );

        case  ENUM_CEK_MUS_IADE_EDILDI :
            return QObject::tr ( "Müşteriye iade edildi" );

        case  ENUM_CEK_ODENDI_NKT :
            return QObject::tr ( "Ödendi - Nakit" );

        case  ENUM_CEK_ODENDI_HESAPTAN :
            return QObject::tr ( "Ödendi - Hesaptan" );

        case  ENUM_CEK_TAHSIL_EDILEMEYEN :
            return QObject::tr ( "Tahsil Edilemeyen" );

        case  ENUM_CEK_IADE_ALINDI_BANKADAN :
            return QObject::tr ( "Bankadan iade alındı" );

        case  ENUM_CEK_IADE_ALINDI_SATICIDAN :
            return QObject::tr ( "Satıcıdan iade alındı" );

        case  ENUM_CEK_SUPHELI_ALACAK :
            return QObject::tr ( "Şüpheli Alacaklar" );

        case  ENUM_CEK_IPTAL :
            return QObject::tr ( "İptal" );

        case  ENUM_CEK_MUSTERIDEN_ALINDI :
            return QObject::tr  ( "Müşteriden çek / senet alındı" );

        case ENUM_TUM_BORDROLAR :
            return QObject::tr("Tüm Bordrolar");

        default :
            return "";
    }
}

/**************************************************************************************
                   CEK_GET_CEK_SENET_POZISYONU_ENUM
***************************************************************************************/

int CEK_GET_CEK_SENET_POZISYONU_ENUM ( QString cek_senet_pozisyonu )
{
    if ( cek_senet_pozisyonu EQ QObject::tr ( "Portföyde" ) ) {
        return ENUM_PORTFOYDE;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr( "Bankaya Teminata Verildi" )) {
        return ENUM_BNK_TEMINAT_VERILDI;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Bankaya Tahsilata Verildi" ) ) {
        return ENUM_BNK_TAHSILAT_VERILDI;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Tahsil Edildi-Nakit" ) ) {
        return ENUM_TAHSIL_EDILDI_NKT;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Tahsil Edildi-Hesaba" ) ) {
        return ENUM_TAHSIL_EDILDI_HESABA;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Satıcıya Verildi" ) ) {
        return ENUM_SATICIYA_VERILDI;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Karşılıksız" ) ) {
        return ENUM_KARSILIKSIZ;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Protestolu" ) ) {
        return ENUM_PROTESTOLU;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Müşteriye İade Edildi" ) ) {
        return ENUM_MUS_IADE_EDILDI;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Ödendi-Nakit" ) )  {
        return ENUM_ODENDI_NKT;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Ödendi-Hesaptan" ) ) {
        return ENUM_ODENDI_HESAPTAN;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Şüpheli Alacaklar" ) ) {
        return ENUM_SUPHELI_ALACAK;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Tahsil Edilemeyen" ) ) {
        return ENUM_TAHSIL_EDILEMEYEN;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "İptal" ) ) {
        return ENUM_IPTAL;
    }
    else if ( cek_senet_pozisyonu EQ QObject::tr ( "Tümü" )) {
        return ENUM_TUM_POZISYONLAR;
    }
    else {
        return -1;
    }
}

/**************************************************************************************
                   CEK_GET_CEK_SENET_POZISYONU_STRING
***************************************************************************************/

QString CEK_GET_CEK_SENET_POZISYONU_STRING ( int cek_senet_pozisyonu )
{
    switch ( cek_senet_pozisyonu ) {
        case ENUM_PORTFOYDE :
            return  QObject::tr ( "Portföyde" );
        case ENUM_BNK_TEMINAT_VERILDI :
            return QObject::tr ( "Bankaya Teminata Verildi" );
        case ENUM_BNK_TAHSILAT_VERILDI :
            return QObject::tr ( "Bankaya Tahsilata Verildi" );
        case ENUM_TAHSIL_EDILDI_NKT :
            return QObject::tr ( "Tahsil Edildi-Nakit" );
        case ENUM_TAHSIL_EDILDI_HESABA :
            return QObject::tr ( "Tahsil Edildi-Hesaba" );
        case ENUM_SATICIYA_VERILDI :
            return QObject::tr ( "Satıcıya Verildi" );
        case ENUM_KARSILIKSIZ :
            return QObject::tr ( "Karşılıksız" );
        case ENUM_PROTESTOLU :
            return QObject::tr ( "Protestolu" );
        case ENUM_MUS_IADE_EDILDI :
            return QObject::tr ( "Müşteriye İade Edildi" );
        case ENUM_ODENDI_NKT :
            return QObject::tr ( "Ödendi-Nakit" );
        case ENUM_ODENDI_HESAPTAN :
            return QObject::tr ( "Ödendi-Hesaptan" );
        case ENUM_SUPHELI_ALACAK :
            return QObject::tr ( "Şüpheli Alacaklar" );
        case ENUM_TAHSIL_EDILEMEYEN :
            return QObject::tr ( "Tahsil Edilemeyen" );
        case ENUM_IPTAL :
            return QObject::tr ( "İptal" );
    case ENUM_TUM_POZISYONLAR :
            return QObject::tr("Tümü");
        default :
                return "";
    };
}

/**************************************************************************************
                   CEK_GET_CEK_SENET_ACILIS_BORDROSU_ISLEM_TURU_ENUM
***************************************************************************************/

int CEK_GET_CEK_SENET_ACILIS_BORDROSU_ISLEM_TURU_ENUM ( QString p_acilis_brd_islem_turu_string )
{

    if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Satıcıya Verildi ( Firma Çekleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_SATICIYA_VER_FIRMA_CEK;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Satıcıya Verildi ( Firma Senetleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_SATICIYA_VER_FIRMA_SENET;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Satıcıya Verildi ( Müşteri Çekleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_SATICIYA_VER_MUS_CEK;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Satıcıya Verildi ( Müşteri Senetleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_SATICIYA_VER_MUS_SENET;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Bankaya Teminat İçin Verildi ( Müşteri Çekleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_CEK;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Bankaya Teminat İçin Verildi ( Müşteri Senetleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_SENET;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Bankaya Tahsilat İçin Verildi ( Müşteri Çekleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_CEK;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Bankaya Tahsilat İçin Verildi ( Müşteri Senetleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_SENET;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Karşılıksız ( Müşteri Çekleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_KARSILIKSIZ_MUS_CEK;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Protestolu  ( Müşteri Senetleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_PROTESTOLU_MUS_SENET;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Şüpheli Alacaklar ( Müşteri Çekleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_SUP_ALACAK_MUS_CEK;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Şüpheli Alacaklar ( Müşteri Senetleri )" ) ) {
        return ENUM_CEK_ACILIS_BRD_SUP_ALACAK_MUS_SENET;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Portföyde ( Müşteri Çekleri )")) {
        return ENUM_CEK_ACILIS_BRD_PORTFOYDE_MUS_CEK;
    }
    else if ( p_acilis_brd_islem_turu_string EQ QObject::tr ( "Portföyde ( Müşteri Senetleri )")) {
        return ENUM_CEK_ACILIS_BRD_PORTFOYDE_MUS_SENET;
    }
    return -1;
}

/**************************************************************************************
                   CEK_GET_CEK_SENET_ACILIS_BORDROSU_ISLEM_TURU_STRING
***************************************************************************************/

QString CEK_GET_CEK_SENET_ACILIS_BORDROSU_ISLEM_TURU_STRING ( int p_acilis_brd_islem_turu_enum )
{
    switch ( p_acilis_brd_islem_turu_enum ) {

        case ENUM_CEK_ACILIS_BRD_SATICIYA_VER_FIRMA_CEK :
            return QObject::tr ( "Satıcıya Verildi ( Firma Çekleri )" );

        case ENUM_CEK_ACILIS_BRD_SATICIYA_VER_FIRMA_SENET :
            return QObject::tr ( "Satıcıya Verildi ( Firma Senetleri )" );

        case ENUM_CEK_ACILIS_BRD_SATICIYA_VER_MUS_CEK :
            return QObject::tr ( "Satıcıya Verildi ( Müşteri Çekleri )" );

        case ENUM_CEK_ACILIS_BRD_SATICIYA_VER_MUS_SENET :
            return QObject::tr ( "Satıcıya Verildi ( Müşteri Senetleri )" );

        case ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_CEK :
            return QObject::tr ( "Bankaya Teminat İçin Verildi ( Müşteri Çekleri )" );

        case ENUM_CEK_ACILIS_BRD_BNK_TEMINAT_MUS_SENET :
            return QObject::tr ( "Bankaya Teminat İçin Verildi ( Müşteri Senetleri )" );

        case ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_CEK :
            return QObject::tr ( "Bankaya Tahsilat İçin Verildi ( Müşteri Çekleri )" );

        case ENUM_CEK_ACILIS_BRD_BNK_TAHSILAT_MUS_SENET :
            return QObject::tr ( "Bankaya Tahsilat İçin Verildi ( Müşteri Senetleri )" );

        case ENUM_CEK_ACILIS_BRD_KARSILIKSIZ_MUS_CEK :
            return QObject::tr ( "Karşılıksız ( Müşteri Çekleri )" );

        case ENUM_CEK_ACILIS_BRD_PROTESTOLU_MUS_SENET :
            return QObject::tr ( "Protestolu  ( Müşteri Senetleri )" );

        case ENUM_CEK_ACILIS_BRD_SUP_ALACAK_MUS_CEK :
            return QObject::tr ( "Şüpheli Alacaklar ( Müşteri Çekleri )" );

        case ENUM_CEK_ACILIS_BRD_SUP_ALACAK_MUS_SENET :
            return QObject::tr ( "Şüpheli Alacaklar ( Müşteri Senetleri )" );

        case ENUM_CEK_ACILIS_BRD_PORTFOYDE_MUS_CEK    :
                return QObject::tr("Portföyde ( Müşteri Çekleri )");
        case ENUM_CEK_ACILIS_BRD_PORTFOYDE_MUS_SENET :
                return QObject::tr("Portföyde ( Müşteri Senetleri )");

        default :
            return "";
    }
}

/**************************************************************************************
                   CEK_GET_CEK_SENET_ENT_BORDROSU_ISLEM_TURU_ENUM
***************************************************************************************/

int CEK_GET_CEK_SENET_ENT_BORDROSU_ISLEM_TURU_ENUM ( QString p_ent_brd_islem_turu_string )
{
    if ( p_ent_brd_islem_turu_string EQ QObject::tr ( "Satıcıya Verildi ( Firma Çekleri )" ) ) {
        return ENUM_CEK_ENT_BRD_SATICIYA_VER_FIRMA_CEK;
    }
    else if ( p_ent_brd_islem_turu_string EQ QObject::tr ( "Satıcıya Verildi ( Firma Senetleri )" ) ) {
        return ENUM_CEK_ENT_BRD_SATICIYA_VER_FIRMA_SENET;
    }
    else if ( p_ent_brd_islem_turu_string EQ QObject::tr ( "Satıcıya Verildi ( Müşteri Çekleri )" ) ) {
        return ENUM_CEK_ENT_BRD_SATICIYA_VER_MUS_CEK;
    }
    else if ( p_ent_brd_islem_turu_string EQ QObject::tr ( "Satıcıya Verildi ( Müşteri Senetleri )"       ) ) {
        return ENUM_CEK_ENT_BRD_SATICIYA_VER_MUS_SENET;
    }
    else if ( p_ent_brd_islem_turu_string EQ QObject::tr ( "Müşteriden Çek Alındı ( Müşteri Çekleri )"     ) ) {
        return ENUM_CEK_ENT_BRD_MUS_CEK_ALINDI_MUS_CEK;
    }
    else if ( p_ent_brd_islem_turu_string EQ QObject::tr ( "Müşteriden Senet Alındı ( Müşteri Senetleri )" ) ) {
        return ENUM_CEK_ENT_BRD_MUS_CEK_ALINDI_MUS_SENET;
    }
    return -1;
}

/**************************************************************************************
                   CEK_GET_CEK_SENET_ENT_BORDROSU_ISLEM_TURU_STRING
***************************************************************************************/

QString CEK_GET_CEK_SENET_ENT_BORDROSU_ISLEM_TURU_STRING ( int islem_turu , int cek_senet_mi , int musteri_firma_mi )
{
    if (  islem_turu EQ ENUM_CEK_SATICIYA_VERILDI ) {
        if ( musteri_firma_mi EQ 1 ) {
            if (cek_senet_mi EQ 1) {
                return QObject::tr ( "Satıcıya Verildi ( Firma Çekleri )" );
            }
            return QObject::tr ( "Satıcıya Verildi ( Firma Senetleri )" );
        }
        //else Musteri
        if (cek_senet_mi EQ 1 ) {
            return QObject::tr ( "Satıcıya Verildi ( Müşteri Senetleri )" );
        }
        return QObject::tr ( "Satıcıya Verildi ( Müşteri Çekleri )" );
    }
    //else CEK_ALINDI
    if ( cek_senet_mi EQ 1 ) {
        return QObject::tr ( "Müşteriden Senet Alındı ( Müşteri Senetleri )" );
    }

    return QObject::tr ( "Müşteriden Çek Alındı ( Müşteri Çekleri )" );
}

/**************************************************************************************
                   CEK_GET_CEK_SENET_TURU_ENUM
***************************************************************************************/

int CEK_GET_CEK_SENET_TURU_ENUM ( QString cek_senet_turu )
{
    if  ( cek_senet_turu EQ QObject::tr ( "Müşteri Çek / Senetleri")) {
          return ENUM_MUSTERI_CEK_SENEDI;
    }
    else if ( cek_senet_turu EQ QObject::tr ( "Kendi Çek / Senetlerimiz" ) ) {
        return ENUM_KENDI_CEK_SENEDIMIZ;
    }
    else if ( cek_senet_turu EQ QObject::tr ( "Tüm Çekler / Senetler" ) ) {
        return ENUM_TUM_CEK_SENETLER;
    }
    else {
        return -1;
    }
}

/**************************************************************************************
                   CEK_ISLEM_TURUNE_GORE_CEKIN_DURUMUNU_AL
***************************************************************************************/

int CEK_ISLEM_TURUNE_GORE_CEKIN_DURUMUNU_AL ( int bordro_islem_turu )
{
    switch ( bordro_islem_turu ) {
           case  ENUM_CEK_BNK_TEMINATA_VERILDI           :
                 return ENUM_BNK_TEMINAT_VERILDI;

            case ENUM_CEK_BNK_TAHSILATA_VERILDI          :
                 return ENUM_BNK_TAHSILAT_VERILDI;

            case ENUM_CEK_TAHSIL_EDILDI_HESABA           :
                 return ENUM_TAHSIL_EDILDI_HESABA;

            case ENUM_CEK_TAHSIL_EDILDI_NKT              :
                 return ENUM_TAHSIL_EDILDI_NKT;

            case ENUM_CEK_SATICIYA_VERILDI                    :
                 return ENUM_SATICIYA_VERILDI;

            case ENUM_CEK_KARSILIKSIZ                    :
                return ENUM_KARSILIKSIZ;

            case ENUM_CEK_PROTESTOLU                     :
                return ENUM_PROTESTOLU;

            case ENUM_CEK_MUS_IADE_EDILDI                :
                return ENUM_MUS_IADE_EDILDI;

            case ENUM_CEK_ODENDI_NKT                     :
                return ENUM_ODENDI_NKT;

            case ENUM_CEK_ODENDI_HESAPTAN                :
                 return ENUM_ODENDI_HESAPTAN;

            case ENUM_CEK_IADE_ALINDI_BANKADAN           :
            case ENUM_CEK_IADE_ALINDI_SATICIDAN         :
            case ENUM_CEK_MUSTERIDEN_ALINDI              :
                 return ENUM_PORTFOYDE;

            case ENUM_CEK_SUPHELI_ALACAK                 :
                 return ENUM_SUPHELI_ALACAK;

             case ENUM_CEK_TAHSIL_EDILEMEYEN             :
                 return ENUM_TAHSIL_EDILEMEYEN;

            case ENUM_CEK_IPTAL                          :
                 return ENUM_IPTAL;

            default                                      :
                 return -1;
    };
}

/**************************************************************************************
                            CEK_GET_CEK_BORDRO_TURU
***************************************************************************************/

int CEK_GET_CEK_BORDRO_TURU( int bordro_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("cek_bordrolar",
                         "bordro_turu",
                         "bordro_id = :bordro_id");
    query.SET_VALUE(":bordro_id" , bordro_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        return query.VALUE( 0 ).toInt();
    }

    return 0;
}

/**************************************************************************************
                            CEK_GET_BORDRO_ID
***************************************************************************************/

int CEK_GET_BORDRO_ID( int p_program_id, int p_modul_id, int p_base_fis_id, int p_base_record_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT("cek_bordrolar,cek_bordro_satirlari",
                         "cek_bordrolar.bordro_id ",
                         "base_fis_id                = :base_fis_id AND    "
                         "base_record_id             = :base_record_id AND "
                         "cek_bordrolar.program_id   = :program_id         "
                         "AND cek_bordrolar.modul_id = :modul_id           ");
    query.SET_VALUE(":base_fis_id"    , p_base_fis_id       );
    query.SET_VALUE(":base_record_id" , p_base_record_id    );
    query.SET_VALUE(":program_id"     , p_program_id        );
    query.SET_VALUE(":modul_id"       , p_modul_id          );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        return query.VALUE(0).toInt();
    }
    return 0;
}

/**************************************************************************************
                            CEK_UPDATE_CEK_TUTARI
***************************************************************************************/

void CEK_UPDATE_CEK_TUTARI( int p_cek_senet_id, double p_cek_senet_tutari )
{
    SQL_QUERY query(DB);

    query.PREPARE_UPDATE("cek_cekler_senetler", "cek_senet_id", "cek_senet_tutari",
                         "cek_senet_id = :cek_senet_id");
    query.SET_VALUE(":cek_senet_tutari" , p_cek_senet_tutari );
    query.SET_VALUE(":cek_senet_id"     , p_cek_senet_id );
    query.UPDATE();
}

/**************************************************************************************
                            CEK_GET_CEK_SENET_TUTARI
***************************************************************************************/

double CEK_GET_CEK_SENET_TUTARI(int cek_senet_id, int mus_ceki_mi,
                                int cek_senet_durumu, int cek_senet_turu)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT ( "cek_cekler_senetler","cek_senet_tutari ",
                           "cek_senet_id                 = :cek_senet_id "
                           "AND musteri_ceki_mi          = :musteri_ceki_mi "
                           "AND  cek_senet_son_durumu    = :cek_senet_son_durumu "
                           "AND cek_senet_turu           = :cek_senet_turu");

    query.SET_VALUE(":cek_senet_id"         , cek_senet_id );
    query.SET_VALUE(":musteri_ceki_mi"      , mus_ceki_mi  );
    query.SET_VALUE(":cek_senet_son_durumu" , cek_senet_durumu);
    query.SET_VALUE(":cek_senet_turu"       , cek_senet_turu  );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toDouble();
}

/**************************************************************************************
                            CEK_GET_CEK_SENET_ID
***************************************************************************************/

int CEK_GET_CEK_SENET_ID(int portfoy_numarasi)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT ( "cek_cekler_senetler", "cek_senet_id ",
                           "portfoy_numarasi = :portfoy_numarasi" );
    query.SET_VALUE      ( ":portfoy_numarasi", portfoy_numarasi );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                            CEK_SENET_ALINABILIR_MI
***************************************************************************************/

bool CEK_CARI_CEK_SENET_ALINABILIR_MI( int p_cek_senet_id, int p_cari_hesap_id )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "cek_bordrolar, cek_bordro_satirlari", "cari_hesap_id, alinan_cari_hesap_id",
                          "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id "
                          " AND cek_bordro_satirlari.cek_senet_id = :cek_senet_id");
    query.SET_VALUE     ( ":cek_senet_id", p_cek_senet_id );

    if ( query.SELECT() EQ 0 ) {
        return false;
    }

    query.NEXT();

    int cari_hesap_id        = query.VALUE( 0 ).toInt();
    int alinan_cari_hesap_id = query.VALUE( 1 ).toInt();

    if ( p_cari_hesap_id EQ cari_hesap_id OR p_cari_hesap_id EQ alinan_cari_hesap_id ) {
        return false;
    }
    return true;
}



