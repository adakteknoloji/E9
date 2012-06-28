#include "irs_irsaliye_belgesi_class.h"
#include "irs_irsaliye_belgesi_open.h"
#include "adak_defines.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "belge_kernel_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_belge.h"
#include "cari_console_utils.h"
#include "e9_belge.h"
#include "cari_struct.h"
#include "fat_console_utils.h"
#include "fat_fis_utils.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "sube_struct.h"
#include "sube_console_utils.h"
#include "fat_enum.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   IRS_IRSALIYE_BELGESI::IRS_IRSALIYE_BELGESI
***************************************************************************************/

void PRINT_IRS_IRSALIYE_BELGESI ( int p_irsaliye_id )
{
    int tasarim_id = TASARIM_SEC ( e9_belge_struct [ SEVK_IRSALIYESI ].belge_id, NULL );

    if ( tasarim_id EQ -1 ) {
        return;
    }

    SET_BELGE_TASARIM ( SEVK_IRSALIYESI, tasarim_id, e9_belge_struct );

    IRS_IRSALIYE_BELGESI * tahsilat_makbuzu = new IRS_IRSALIYE_BELGESI ( p_irsaliye_id );
    tahsilat_makbuzu->START_PRINT();
}

/**************************************************************************************
                   IRS_IRSALIYE_BELGESI::IRS_IRSALIYE_BELGESI
***************************************************************************************/

IRS_IRSALIYE_BELGESI::IRS_IRSALIYE_BELGESI ( int p_irsaliye_id )
{
    m_fis_id        = p_irsaliye_id;
    m_line_index    = 0;

    SQL_QUERY f_query   ( DB );
    SQL_QUERY s_query   ( DB );

    f_query.PREPARE_SELECT("stk_fisler" , "fis_id, fis_no, fis_tarihi, islem_saati, aciklama, sevk_tarihi, "
                           "belge_seri, belge_numarasi, cari_hesap_id, cari_hesap_ismi, vergi_dairesi, "
                           "vergi_numarasi, teslim_adresi, brut_tutar, iskonto_tutari, ara_toplam, "
                           "irsaliye_tutari, fis_turu, belge_seri, belge_numarasi" , "fis_id = :fis_id");

    f_query.SET_VALUE ( ":fis_id", m_fis_id );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    f_query.NEXT();

    //HEADER

    int     fis_turu        = f_query.VALUE( "fis_turu" ).toInt();

    QString irs_belge_seri      = f_query.VALUE( "belge_seri").toString();
    QString irs_belge_numarasi  = f_query.VALUE( "belge_numarasi").toString();

    if ( irs_belge_seri.isEmpty() EQ true OR irs_belge_numarasi.isEmpty() EQ true  ) {

        SQL_QUERY s_query ( DB );

        SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;

        SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

        QList <int> fatura_id_list = FAT_GET_FAT_IRS_BAGLANTISI_FATURA_ID( m_fis_id );

        bool irsaliyeli_fatura = false;
        if ( fatura_id_list.size() NE 0 ) {
            int fatura_turu = FAT_GET_FATURA_TURU( fatura_id_list.at(0) );

            if ( fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) {
                irs_belge_seri      = UNITE_BILG.irsaliyeli_fat_belge_seri;
                irs_belge_numarasi  = UNITE_BILG.irsaliyeli_fat_belge_numarasi;
                irsaliyeli_fatura   = true;

            }
            else {
                irs_belge_seri      = UNITE_BILG.irsaliye_belge_seri;
                irs_belge_numarasi  = UNITE_BILG.irsaliye_belge_numarasi;
            }
        }
        else {
            irs_belge_seri      = UNITE_BILG.irsaliye_belge_seri;
            irs_belge_numarasi  = UNITE_BILG.irsaliye_belge_numarasi;
        }

       DB->START_TRANSACTION();

       // FATURAYADA SERI NO VERILMEK ZORUNDA
       if ( irsaliyeli_fatura EQ true ) {

           s_query.PREPARE_UPDATE( "fat_faturalar","fis_id", "belge_seri , belge_numarasi" ,"fis_id =:fis_id");
           s_query.SET_VALUE     ( ":belge_seri",          irs_belge_seri       );
           s_query.SET_VALUE     ( ":belge_numarasi",      irs_belge_numarasi   );
           s_query.SET_VALUE     ( ":fis_id",              fatura_id_list.at(0) );

           s_query.UPDATE();

           SUBE_UPDATE_IRSALIYELI_FAT_BELGE_NO( SUBE_GET_UNITE_ID(), QVariant ( QVariant ( irs_belge_numarasi ).toInt() + 1 ).toString() );
       }

       s_query.PREPARE_UPDATE( "stk_fisler","fis_id", "belge_seri , belge_numarasi" ,"fis_id =:fis_id");
       s_query.SET_VALUE     ( ":belge_seri",          irs_belge_seri      );
       s_query.SET_VALUE     ( ":belge_numarasi",      irs_belge_numarasi  );
       s_query.SET_VALUE     ( ":fis_id",              m_fis_id            );

       s_query.UPDATE();

       SUBE_UPDATE_IRS_BELGE_NO( SUBE_GET_UNITE_ID(), QVariant ( QVariant ( irs_belge_numarasi ).toInt() + 1 ).toString() );

       DB->COMMIT_TRANSACTION();
    }

    CARI_HESAP_STRUCT cari_hesap;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&cari_hesap);

    CARI_KART_BILGILERINI_OKU(f_query.VALUE(8).toInt(),&cari_hesap);

    M_IRS_FATURA_ADRESI = "";

    M_IRS_FATURA_ADRESI = QString (cari_hesap.unvan + cari_hesap.cari_hesap_ismi + "\n" + cari_hesap.adres_satiri_1
                                   + " " + cari_hesap.adres_satiri_2 + " " + cari_hesap.adres_satiri_3 + "\n" + cari_hesap.ilce
                                   + " " + cari_hesap.semt + " " + cari_hesap.sehir + " " + cari_hesap.ulke);

    if ( M_IRS_FATURA_ADRESI.isEmpty() EQ false ) {
        if ( M_IRS_FATURA_ADRESI EQ M_IRS_SEVK_ADRESI ) {
            M_IRS_FATURA_ADRESI.clear();
        }
    }

    M_IRS_SEVK_ADRESI           =   f_query.VALUE ( "teslim_adresi"  ).toString();
    M_IRS_VERGI_DAIRESI         =   f_query.VALUE ( "vergi_dairesi"  ).toString();
    M_IRS_VERGI_NO              =   f_query.VALUE ( "vergi_numarasi" ).toString();
    M_IRS_SERI_SIRA_NO          =   f_query.VALUE ( "belge_seri" ).toString() + " " + f_query.VALUE ( "belge_numarasi" ).toString();
    M_IRS_DUZENLEME_TARIHI      =   QDate::fromString ( f_query.VALUE ( "fis_tarihi" ).toString(), "yyyy.MM.dd" ).toString ( "dd.MM.yyyy" );
    M_IRS_DUZENLEME_SAATI       =   f_query.VALUE ( "islem_saati" ).toString();
    M_IRS_TARIHI                =   QDate::fromString ( f_query.VALUE ( "fis_tarihi" ).toString(), "yyyy.MM.dd" ).toString ( "dd.MM.yyyy" );
    int cari_hesap_id           =   f_query.VALUE ( "cari_hesap_id" ).toInt();

    // FATURA YAZDIRILMISSA FATURA NO VE FATURA TARIHI BILGILERI YAZDIRILIR
    // CUNKU FATURAYA AIT BELGE_SERI VE BELGE_NO FATURA YAZDIRILMAMISSA HENUZ OLUSMAMISTIR

    // İRSALİYE BIRDEN FAZLA FATURAYA REFERANS EDEBILIR BU DURUMDA REFERANS EDEN FATURALAR
    // LISTE HALINDE ALINIP YAZDIRILMALI SIMDILIK ERTELENDI.

    M_IRS_FAT_TARIHI.clear();
    M_IRS_FAT_NO.clear();
    M_IRS_FAT_FIILI_SEVK_TARIHI = QDate::fromString ( f_query.VALUE ( "sevk_tarihi" ).toString(), "yyyy.MM.dd" ).toString ( "dd.MM.yyyy" );

    // Entegre oldugu Fatura Listesi

    // hangi irsaliyeler ile baglantili

    s_query.PREPARE_SELECT("fat_irs_baglantisi" , "fatura_id",
                           "irsaliye_id = :irsaliye_id");

    s_query.SET_VALUE ( ":irsaliye_id", m_fis_id );

    s_query.SELECT();

    FATURA_FISI_STRUCT FAT_FIS;

    FAT_FIS.KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];



    while ( s_query.NEXT() EQ true ) {
        int fatura_id = s_query.VALUE( "fatura_id" ).toInt();
        FATURA_CLEAR_FIS_STRUCT ( &FAT_FIS );
        FATURA_FIS_BILGILERINI_OKU( &FAT_FIS, fatura_id );

        M_IRS_FAT_TARIH_LIST.append( FAT_FIS.fatura_tarihi );
        M_IRS_FAT_NO_LIST.append   ( FAT_FIS.belge_numarasi );
    }

    if ( fis_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS ) {

        s_query.PREPARE_SELECT("stk_fis_satirlari",
                               "fis_satiri_id" ,
                               "fis_id = :fis_id");

        s_query.SET_VALUE ( ":fis_id", m_fis_id );

        FATURA_FISI_STRUCT FAT_FIS;

        s_query.SELECT();

        while ( s_query .NEXT() EQ true ) {

            int irs_fis_satiri_id = s_query.VALUE( "fis_satiri_id" ).toInt();
            int fat_fis_id        = FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_ID( irs_fis_satiri_id );
            FATURA_CLEAR_FIS_STRUCT ( &FAT_FIS );

            FATURA_FIS_BILGILERINI_OKU( &FAT_FIS , fat_fis_id );

            M_IRS_FAT_TARIH_LIST.append ( FAT_FIS.fatura_tarihi );
            M_IRS_FAT_NO_LIST.append    ( QVariant ( FAT_FIS.fis_no ).toString() );
        }
    }

    for ( int i = 0; i < M_IRS_FAT_TARIH_LIST.count(); i++ ) {
        M_IRS_FAT_TARIHI.append ( M_IRS_FAT_TARIH_LIST.at( i ) );
        if ( i < M_IRS_FAT_TARIH_LIST.count() -1 ) {
            M_IRS_FAT_TARIHI.append( ", " );
        }
        M_IRS_FAT_NO.append     ( M_IRS_FAT_NO_LIST.at( i ) );
        if ( i < M_IRS_FAT_TARIH_LIST.count() -1 ) {
            M_IRS_FAT_TARIHI.append( ", " );
        }
    }

    // FOOTER

    M_IRS_BRUT_TUTAR            =   f_query.VALUE ( "brut_tutar"      ).toString();
    M_IRS_ISKONTO_TUTARI        =   f_query.VALUE ( "iskonto_tutari"  ).toString();
    M_IRS_ARA_TOPLAM            =   f_query.VALUE ( "ara_toplam"      ).toString();
    M_IRS_TOPLAM_RAKAMLA        =   f_query.VALUE ( "irsaliye_tutari" ).toString();
    M_IRS_TOPLAM_YAZIYLA        =   ADAK_YAZIYLA_YAZ ( f_query.VALUE ( "irsaliye_tutari" ).toDouble() );

    BORC_BILGISI borc_bilgisi;

    CARI_KART_BORC_BILGISINI_GETIR ( cari_hesap_id, &borc_bilgisi );  // borc bilgisini donderir
    DVZ_GET_DOVIZ_KODU                 ( borc_bilgisi.para_birimi_id );

    if ( borc_bilgisi.borc_bakiye > 0.0 ) {
        M_IRS_SON_CAR_BAKIYE += QObject::tr ( "Borcu : " ) + VIRGUL_EKLE ( QVariant ( borc_bilgisi.borc_bakiye ).toString() , 2 );
    }
    else if ( borc_bilgisi.alacak_bakiye > 0.0 ) {
        M_IRS_SON_CAR_BAKIYE += QObject::tr ( "Alacağı : " ) + VIRGUL_EKLE ( QVariant ( borc_bilgisi.alacak_bakiye ).toString() , 2 );
    }
    else {
        M_IRS_SON_CAR_BAKIYE += QObject::tr ( "Borcu / Alacağı : " ) + VIRGUL_EKLE ( QVariant ( 0 ).toString() , 2 );
    }

    M_IRS_KDV_1.clear();
    M_IRS_KDV_2.clear();
    M_IRS_KDV_3.clear();
    M_IRS_KDV_4.clear();
    M_IRS_KDV_5.clear();
    M_IRS_OTV.clear();
    M_IRS_TESLIM_ALAN.clear();
    M_IRS_TESLIM_EDEN.clear();
    M_IRS_DIP_NOT.clear();


    //LINE

    f_query.PREPARE_SELECT("stk_fis_satirlari" ,
                           "urun_id, urun_adi, satirdaki_urun_miktari, urun_birimi, satir_tutari, "
                           "doviz_id, kdv_orani, satir_iskonto_tutari, satir_vergi_haric_tutar, "
                           "isk_sonrasi_urun_birim_fiyati, isk_oncesi_urun_birim_fiyati, kdv_dahil_mi,"
                           "kdv_orani, otv_orani, satir_kdv_dahil_tutar, satir_otv_dahil_tutar, "
                           "satir_vergi_haric_tutar, satir_kdv_tutari, satir_otv_tutari",
                           "fis_id = :fis_id");

    f_query.SET_VALUE      ( ":fis_id" , m_fis_id );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    QString urun_kodu;

    while ( f_query.NEXT() EQ true ) {

        urun_kodu.clear();

        if ( f_query.VALUE ( "urun_id" ).toInt() > 0 ) {
            s_query.PREPARE_SELECT ( "stk_urunler","urun_kodu" ,"urun_id = :urun_id" );
            s_query.SET_VALUE      ( ":urun_id", f_query.VALUE ( "urun_id" ).toInt() );
            if ( s_query.SELECT()  > 0 ) {
                s_query.NEXT();
                urun_kodu = s_query.VALUE ( 0 ).toString();
            }
        }

        M_IRS_URUN_KODU             << urun_kodu;
        M_IRS_URUN_CINSI            << f_query.VALUE ( "urun_adi"                ).toString();
        M_IRS_BIRIM                 << f_query.VALUE ( "urun_birimi"             ).toString();
        M_IRS_MIKTAR                << f_query.VALUE ( "satirdaki_urun_miktari"  ).toString();
        M_IRS_SATIR_TUTAR                 << f_query.VALUE ( "satir_tutari"            ).toString();
        M_IRS_KDV_ORANI             << f_query.VALUE ( "kdv_orani"               ).toString();
        M_IRS_ISKONTO               << f_query.VALUE ( "satir_iskonto_tutari"    ).toString();
        M_IRS_SATIS_TUTARI          << f_query.VALUE ( "satir_vergi_haric_tutar" ).toString();
        M_IRS_IS_SON_BIRIM_FIYAT    << f_query.VALUE ( "isk_sonrasi_urun_birim_fiyati" ).toString();

        M_IRS_SATIR_DOVIZ_KODU      << DVZ_GET_DOVIZ_KODU( f_query.VALUE ("doviz_id").toInt());
        M_IRS_SATIR_OTV_TUTARI      << f_query.VALUE ( "satir_otv_tutari"           ).toString();
        M_IRS_SATIR_KDV_TUTARI      << f_query.VALUE ( "satir_kdv_tutari"           ).toString();
        M_IRS_SATIR_OTV_DAHIL_TUTAR << f_query.VALUE ( "satir_otv_dahil_tutar"      ).toString();
        M_IRS_SATIR_KDV_DAHIL_TUTAR << f_query.VALUE ( "satir_kdv_dahil_tutar"      ).toString();
        M_IRS_SATIR_KDV_ORANI       << f_query.VALUE ( "kdv_orani"                  ).toString();
        M_IRS_SATIR_VERGI_HRC_TUTAR << f_query.VALUE ( "satir_vergi_haric_tutar"    ).toString();
        M_IRS_SATIR_OTV_ORANI       << f_query.VALUE ( "otv_orani"                  ).toString();
        M_IRS_IS_ON_BIRIM_FIYAT     << f_query.VALUE ( "isk_oncesi_urun_birim_fiyati").toString();

        if ( f_query.VALUE ("kdv_dahil_mi").toInt() EQ 1 ) {
            M_IRS_SATIR_KDV_DAHIL_MI  << QObject::tr ( "Dahil" );
        }
        else {
            M_IRS_SATIR_KDV_DAHIL_MI  << QObject::tr ( "Hariç" );
        }
    }
}

/**************************************************************************************
                   IRS_IRSALIYE_BELGESI::GET_HEADER_INFO
***************************************************************************************/

void IRS_IRSALIYE_BELGESI::GET_HEADER_INFO ()
{

    SET_HEADER_DATA ( IRS_SEVK_ADRESI           , M_IRS_SEVK_ADRESI           );
    SET_HEADER_DATA ( IRS_FATURA_ADRESI         , M_IRS_FATURA_ADRESI         );
    SET_HEADER_DATA ( IRS_VERGI_DAIRESI         , M_IRS_VERGI_DAIRESI         );
    SET_HEADER_DATA ( IRS_VERGI_NO              , M_IRS_VERGI_NO              );
    SET_HEADER_DATA ( IRS_SERI_SIRA_NO          , M_IRS_SERI_SIRA_NO          );
    SET_HEADER_DATA ( IRS_DUZENLEME_TARIHI      , M_IRS_DUZENLEME_TARIHI      );
    SET_HEADER_DATA ( IRS_DUZENLEME_SAATI       , M_IRS_DUZENLEME_SAATI       );
    SET_HEADER_DATA ( IRS_TARIHI                , M_IRS_TARIHI                );
    SET_HEADER_DATA ( IRS_FAT_TARIHI            , M_IRS_FAT_TARIHI            );
    SET_HEADER_DATA ( IRS_FAT_NO                , M_IRS_FAT_NO                );
    SET_HEADER_DATA ( IRS_FAT_FIILI_SEVK_TARIHI , M_IRS_FAT_FIILI_SEVK_TARIHI );
}

/**************************************************************************************
                   IRS_IRSALIYE_BELGESI::GET_FOOTER_INFO
***************************************************************************************/

void IRS_IRSALIYE_BELGESI::GET_FOOTER_INFO ()
{

    SET_FOOTER_DATA ( IRS_BRUT_TUTAR        , M_IRS_BRUT_TUTAR        );
    SET_FOOTER_DATA ( IRS_ISKONTO_TUTARI    , M_IRS_ISKONTO_TUTARI    );
    SET_FOOTER_DATA ( IRS_ARA_TOPLAM        , M_IRS_ARA_TOPLAM        );
    SET_FOOTER_DATA ( IRS_TOPLAM_RAKAMLA    , M_IRS_TOPLAM_RAKAMLA    );
    SET_FOOTER_DATA ( IRS_TOPLAM_YAZIYLA    , M_IRS_TOPLAM_YAZIYLA    );
    SET_FOOTER_DATA ( IRS_SON_CAR_BAKIYE    , M_IRS_SON_CAR_BAKIYE    );
    SET_FOOTER_DATA ( IRS_KDV_1             , M_IRS_KDV_1             );
    SET_FOOTER_DATA ( IRS_KDV_2             , M_IRS_KDV_2             );
    SET_FOOTER_DATA ( IRS_KDV_3             , M_IRS_KDV_3             );
    SET_FOOTER_DATA ( IRS_KDV_4             , M_IRS_KDV_4             );
    SET_FOOTER_DATA ( IRS_KDV_5             , M_IRS_KDV_5             );
    SET_FOOTER_DATA ( IRS_OTV               , M_IRS_OTV               );
    SET_FOOTER_DATA ( IRS_TESLIM_ALAN       , M_IRS_TESLIM_ALAN       );
    SET_FOOTER_DATA ( IRS_TESLIM_EDEN       , M_IRS_TESLIM_EDEN       );
    SET_FOOTER_DATA ( IRS_DIP_NOT           , M_IRS_DIP_NOT           );
}

/**************************************************************************************
                   IRS_IRSALIYE_BELGESI::GET_LINE_INFO
***************************************************************************************/

void IRS_IRSALIYE_BELGESI::GET_LINE_INFO ()
{

    QString IRS_URUN_KODU_STR               = "";
    QString IRS_URUN_CINSI_STR              = "";
    QString IRS_BIRIM_STR                   = "";
    QString IRS_MIKTAR_STR                  = "";
    QString IRS_SATIR_TUTAR_STR             = "";
    QString IRS_KDV_ORANI_STR               = "";
    QString IRS_ISKONTO_STR                 = "";
    QString IRS_SATIS_TUTARI_STR            = "";
    QString IRS_IS_SON_BIRIM_FIYAT_STR      = "";
    QString IRS_SATIR_DOVIZ_KODU_STR        = "";
    QString IRS_SATIR_KDV_TUTARI_STR        = "";
    QString IRS_SATIR_OTV_TUTARI_STR        = "";
    QString IRS_SATIR_OTV_DAHIL_TUTAR_STR   = "";
    QString IRS_SATIR_KDV_DAHIL_TUTAR_STR   = "";
    QString IRS_SATIR_KDV_ORANI_STR         = "";
    QString IRS_SATIR_VERGI_HRC_TUTAR_STR   = "";
    QString IRS_SATIR_OTV_ORANI_STR         = "";
    QString IRS_SATIR_KDV_DAHIL_MI_STR      = "";
    QString IRS_IS_ON_BIRIM_FIYAT_STR       = "";

    if ( m_line_index < M_IRS_URUN_CINSI.size() ) {
        IRS_URUN_KODU_STR               = M_IRS_URUN_KODU.at( m_line_index );
        IRS_URUN_CINSI_STR              = M_IRS_URUN_CINSI.at( m_line_index );
        IRS_BIRIM_STR                   = M_IRS_BIRIM.at( m_line_index );
        IRS_MIKTAR_STR                  = M_IRS_MIKTAR.at( m_line_index );
        IRS_SATIR_TUTAR_STR             = M_IRS_SATIR_TUTAR.at( m_line_index );
        IRS_KDV_ORANI_STR               = M_IRS_KDV_ORANI.at( m_line_index );
        IRS_ISKONTO_STR                 = M_IRS_ISKONTO.at( m_line_index );
        IRS_SATIS_TUTARI_STR            = M_IRS_SATIS_TUTARI.at( m_line_index );
        IRS_IS_SON_BIRIM_FIYAT_STR      = M_IRS_IS_SON_BIRIM_FIYAT.at( m_line_index );
        IRS_SATIR_DOVIZ_KODU_STR        = M_IRS_SATIR_DOVIZ_KODU.at( m_line_index );
        IRS_SATIR_KDV_TUTARI_STR        = M_IRS_SATIR_KDV_TUTARI.at( m_line_index );
        IRS_SATIR_OTV_TUTARI_STR        = M_IRS_SATIR_OTV_TUTARI.at( m_line_index );
        IRS_SATIR_OTV_DAHIL_TUTAR_STR   = M_IRS_SATIR_OTV_DAHIL_TUTAR.at( m_line_index );
        IRS_SATIR_KDV_DAHIL_TUTAR_STR   = M_IRS_SATIR_KDV_DAHIL_TUTAR.at( m_line_index );
        IRS_SATIR_KDV_ORANI_STR         = M_IRS_SATIR_KDV_ORANI.at( m_line_index );
        IRS_SATIR_VERGI_HRC_TUTAR_STR   = M_IRS_SATIR_VERGI_HRC_TUTAR.at( m_line_index );
        IRS_SATIR_OTV_ORANI_STR         = M_IRS_SATIR_OTV_ORANI.at( m_line_index );
        IRS_SATIR_KDV_DAHIL_MI_STR      = M_IRS_SATIR_KDV_DAHIL_MI.at( m_line_index );
        IRS_IS_ON_BIRIM_FIYAT_STR       = M_IRS_IS_ON_BIRIM_FIYAT.at( m_line_index );

    }

    SET_LINE_DATA ( IRS_URUN_KODU             , IRS_URUN_KODU_STR    );
    SET_LINE_DATA ( IRS_URUN_CINSI            , IRS_URUN_CINSI_STR   );
    SET_LINE_DATA ( IRS_BIRIM                 , IRS_BIRIM_STR        );
    SET_LINE_DATA ( IRS_MIKTAR                , IRS_MIKTAR_STR       );
    SET_LINE_DATA ( IRS_SATIR_TUTAR           , IRS_SATIR_TUTAR_STR        );
    SET_LINE_DATA ( IRS_KDV_ORANI             , IRS_KDV_ORANI_STR    );
    SET_LINE_DATA ( IRS_ISKONTO               , IRS_ISKONTO_STR      );
    SET_LINE_DATA ( IRS_SATIS_TUTARI          , IRS_SATIS_TUTARI_STR );
    SET_LINE_DATA ( IRS_IS_SON_BIRIM_FIYAT    , IRS_IS_SON_BIRIM_FIYAT_STR );
    SET_LINE_DATA ( IRS_SATIR_DOVIZ_KODU      , IRS_SATIR_DOVIZ_KODU_STR );
    SET_LINE_DATA ( IRS_SATIR_KDV_TUTARI      , IRS_SATIR_KDV_TUTARI_STR );
    SET_LINE_DATA ( IRS_SATIR_OTV_TUTARI      , IRS_SATIR_OTV_TUTARI_STR );
    SET_LINE_DATA ( IRS_SATIR_OTV_DAHIL_TUTAR , IRS_SATIR_OTV_DAHIL_TUTAR_STR );
    SET_LINE_DATA ( IRS_SATIR_KDV_DAHIL_TUTAR , IRS_SATIR_KDV_DAHIL_TUTAR_STR );
    SET_LINE_DATA ( IRS_SATIR_KDV_ORANI       , IRS_SATIR_KDV_ORANI_STR );
    SET_LINE_DATA ( IRS_SATIR_VERGI_HRC_TUTAR , IRS_SATIR_VERGI_HRC_TUTAR_STR );
    SET_LINE_DATA ( IRS_SATIR_OTV_ORANI       , IRS_SATIR_OTV_ORANI_STR );
    SET_LINE_DATA ( IRS_SATIR_KDV_DAHIL_MI    , IRS_SATIR_KDV_DAHIL_MI_STR );
    SET_LINE_DATA ( IRS_IS_ON_BIRIM_FIYAT     , IRS_IS_ON_BIRIM_FIYAT_STR );

    m_line_index++;
}
