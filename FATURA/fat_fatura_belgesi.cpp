#include "fat_fatura_belgesi_class.h"
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
#include "fat_enum.h"
#include "sube_console_utils.h"
#include "stok_fis_utils.h"
#include "sube_struct.h"
#include "stok_console_utils.h"
#include "e9_enum.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   FAT_FATURA_BELGESI::FAT_FATURA_BELGESI
***************************************************************************************/

void PRINT_FAT_FATURA_BELGESI ( int fat_fis_id , QWidget * parent)
{
    int tasarim_id = TASARIM_SEC ( e9_belge_struct[FATURA].belge_id, parent );

    if (tasarim_id EQ -1) {
        return;
    }

    SET_BELGE_TASARIM ( FATURA, tasarim_id, e9_belge_struct );

    FAT_FATURA_BELGESI * tahsilat_makbuzu = new FAT_FATURA_BELGESI ( fat_fis_id );
    tahsilat_makbuzu->START_PRINT  ();
}

/**************************************************************************************
                   FAT_FATURA_BELGESI::FAT_FATURA_BELGESI
***************************************************************************************/

FAT_FATURA_BELGESI::FAT_FATURA_BELGESI ( int fat_fis_id )
{
    m_fis_id        = fat_fis_id;
    m_line_index    = 0;

    SQL_QUERY f_query (DB);
    SQL_QUERY s_query (DB);

    M_FAT_KDV_1_STR = "";
    M_FAT_KDV_2_STR = "";
    M_FAT_KDV_3_STR = "";
    M_FAT_KDV_4_STR = "";
    M_FAT_KDV_5_STR = "";

    f_query.PREPARE_SELECT("fat_faturalar" , "fis_no, fatura_tarihi, belge_seri, belge_numarasi, acik_kapali_fatura,   "
                           "cari_hesap_ismi, aciklama, cari_hesap_adresi, vergi_dairesi,              "
                           "vergi_numarasi, islem_saati, brut_tutar, iskonto_yuzdesi, iskonto_tutari, "
                           "ara_toplam, isk_sonrasi_otv_tutari, isk_sonrasi_otv_dahil_tutar,      "
                           "isk_sonrasi_kdv_tutari, fatura_tutari, cari_hesap_id, fatura_turu, kdv_oran_gore_tutarlar_array" , "fis_id = :fis_id");

    f_query.SET_VALUE ( ":fis_id", m_fis_id );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    f_query.NEXT();

    double  *  KDV_ORANINA_GORE_TUTARLAR_ARRAY = new double  [KDV_OTV_ORANLARI_SAYISI];;

    UNPACK_DOUBLE_ARRAY ( f_query.VALUE ( "kdv_oran_gore_tutarlar_array" ).toString(), KDV_ORANINA_GORE_TUTARLAR_ARRAY,   KDV_OTV_ORANLARI_SAYISI );

    // %0 yazdirmiyoruz
    M_FAT_KDV_1_STR     = QVariant( KDV_ORANINA_GORE_TUTARLAR_ARRAY[1] ).toString();
    M_FAT_KDV_2_STR     = QVariant( KDV_ORANINA_GORE_TUTARLAR_ARRAY[2] ).toString();
    M_FAT_KDV_3_STR     = QVariant( KDV_ORANINA_GORE_TUTARLAR_ARRAY[3] ).toString();
    M_FAT_KDV_4_STR     = QVariant( KDV_ORANINA_GORE_TUTARLAR_ARRAY[4] ).toString();
    M_FAT_KDV_5_STR     = QVariant( KDV_ORANINA_GORE_TUTARLAR_ARRAY[5] ).toString(); // %18

    //HEADER
    M_FAT_FIS_NO_STR    = f_query.VALUE ( "fis_no" ).toString();
    M_FAT_TARIHI_STR    = QDate::fromString ( f_query.VALUE("fatura_tarihi").toString(), "yyyy.MM.dd" ).toString ( "dd MM yyyy");
    M_FAT_DUZEN_TAR_STR = QDate::fromString ( f_query.VALUE("fatura_tarihi").toString(), "yyyy.MM.dd" ).toString ( "dd MM yyyy");

    QString fatura_belge_seri       = f_query.VALUE( "belge_seri" ).toString();
    QString fatura_belge_numarasi   = f_query.VALUE( "belge_numarasi" ).toString();

    int fatura_turu = f_query.VALUE( "fatura_turu" ).toInt();

    SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;

    SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

    DB->START_TRANSACTION();
    if ( fatura_belge_numarasi.isEmpty() EQ true ) {

        SQL_QUERY s_query ( DB );

        if ( fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI OR fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) {
            fatura_belge_seri      = UNITE_BILG.irsaliyeli_fat_belge_seri;
            fatura_belge_numarasi  = UNITE_BILG.irsaliyeli_fat_belge_numarasi;
        }
        else {
            fatura_belge_seri      = UNITE_BILG.fatura_belge_seri;
            fatura_belge_numarasi  = UNITE_BILG.fatura_belge_numarasi;
        }
        s_query.PREPARE_UPDATE( "fat_faturalar","fis_id", "belge_seri , belge_numarasi" ,"fis_id =:fis_id");
        s_query.SET_VALUE     ( ":belge_seri",          fatura_belge_seri       );
        s_query.SET_VALUE     ( ":belge_numarasi",      fatura_belge_numarasi   );
        s_query.SET_VALUE     ( ":fis_id",              m_fis_id                );

        s_query.UPDATE();

        // irsaliyeli faturada ayrica irsaliyede guncellenmeli
        if ( fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) {

            int irsaliye_fat_bag_id = STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID( m_fis_id );

            s_query.PREPARE_UPDATE( "stk_fisler","fis_id", "belge_seri , belge_numarasi" ,"fis_id =:fis_id");
            s_query.SET_VALUE     ( ":belge_seri",          fatura_belge_seri       );
            s_query.SET_VALUE     ( ":belge_numarasi",      fatura_belge_numarasi   );
            s_query.SET_VALUE     ( ":fis_id",              irsaliye_fat_bag_id     );

            s_query.UPDATE();
        }

        if ( fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) {
            SUBE_UPDATE_IRSALIYELI_FAT_BELGE_NO( SUBE_GET_UNITE_ID(), QVariant ( QVariant ( fatura_belge_numarasi ).toInt() + 1 ).toString() );
        }
        else {
            SUBE_UPDATE_FAT_BELGE_NO( SUBE_GET_UNITE_ID(), QVariant ( QVariant ( fatura_belge_numarasi ).toInt() + 1 ).toString() );
        }
    }

    M_FAT_SERISIRA_STR              = fatura_belge_seri + " " + QVariant ( fatura_belge_numarasi ).toString();

    int acik_kapali_fatura = f_query.VALUE ( "acik_kapali_fatura" ).toInt();

    if ( acik_kapali_fatura EQ ENUM_ACIK_FATURA ) {
        M_FAT_ACIK_KAPALI_FATURA_STR    =  QObject::tr ( "Açık Fatura" );
    }
    else {
        M_FAT_ACIK_KAPALI_FATURA_STR    =  QObject::tr ( "Kapalı Fatura" );
    }

    M_FAT_ADR_STR           = f_query.VALUE ( "cari_hesap_adresi" ).toString();
    M_FAT_ACIKLAMA_STR      = f_query.VALUE ( "aciklama"          ).toString();
    M_FAT_SEVK_ADR_STR      = f_query.VALUE ( "cari_hesap_adresi" ).toString();
    M_FAT_VERGI_DAIRE_STR   = f_query.VALUE ( "vergi_dairesi"     ).toString();
    M_FAT_VERGI_NO_STR      = f_query.VALUE ( "vergi_numarasi"    ).toString();
    M_FAT_DUZEN_SAAT_STR    = f_query.VALUE ( "islem_saati"       ).toString();

    s_query.PREPARE_SELECT("fat_irs_baglantisi", "irsaliye_id", "fatura_id = :fatura_id ");
    s_query.SET_VALUE ( ":fatura_id", m_fis_id );

    int irsaliye_id = 0 ;

    if ( s_query.SELECT() NE 0 ) {
        s_query.NEXT();
        irsaliye_id = s_query.VALUE(0).toInt();
    }

    STK_IRS_FIS_STRUCT * IRS_FISI = new STK_IRS_FIS_STRUCT;

    STK_IRS_CLEAR_FIS_STRUCT(IRS_FISI);

    STK_IRS_FIS_BILGILERINI_OKU(irsaliye_id , IRS_FISI);

    if ( fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS ) {
        if ( IRS_FISI->belge_seri.isEmpty() EQ true ) {
            IRS_FISI->belge_seri     = UNITE_BILG.irsaliye_belge_seri;
            IRS_FISI->belge_numarasi = UNITE_BILG.irsaliye_belge_numarasi;

            s_query.PREPARE_UPDATE( "stk_fisler","fis_id", "belge_seri , belge_numarasi" ,"fis_id =:fis_id");
            s_query.SET_VALUE     ( ":belge_seri",          IRS_FISI->belge_seri       );
            s_query.SET_VALUE     ( ":belge_numarasi",      IRS_FISI->belge_numarasi   );
            s_query.SET_VALUE     ( ":fis_id",              irsaliye_id     );
            s_query.UPDATE();

            SUBE_UPDATE_IRS_BELGE_NO( SUBE_GET_UNITE_ID(), QVariant ( QVariant ( IRS_FISI->belge_numarasi ).toInt() + 1 ).toString() );
        }
    }

    DB->COMMIT_TRANSACTION();

    M_FAT_IRS_TARIHI_LIST.append( QDate::fromString( IRS_FISI->fis_tarihi, "yyyy.MM.dd" ).toString( "dd MM yyyy") );
    if ( IRS_FISI->fis_no EQ 0 ) {
        M_FAT_IRS_NO_LIST.append    ( "" );
    }
    else {
        M_FAT_IRS_NO_LIST.append    ( IRS_FISI->belge_numarasi );
    }

    if ( fatura_turu EQ ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS ) {

        s_query.PREPARE_SELECT("fat_irs_satir_baglantisi, fat_fatura_satirlari",
                               "irsaliye_satiri_id" ,
                               "fat_irs_satir_baglantisi.fatura_satiri_id = fat_fatura_satirlari.fis_satiri_id     "
                               "fat_fatura_satirlari.fis_id = :fis_id" );

        s_query.SET_VALUE ( ":fis_id", m_fis_id );

        s_query.SELECT();

        while ( s_query .NEXT() EQ true ) {

            int irs_satiri_id = s_query.VALUE(0).toInt();

            STK_IRS_CLEAR_FIS_STRUCT(IRS_FISI);

            STK_IRS_FIS_BILGILERINI_OKU(  STK_GET_IRS_ID ( irs_satiri_id ), IRS_FISI);

            M_FAT_IRS_TARIHI_LIST.append( QDate::fromString( IRS_FISI->fis_tarihi, "yyyy.MM.dd" ).toString( "dd MM yyyy") );
            M_FAT_IRS_NO_LIST.append    ( QVariant ( IRS_FISI->fis_no ).toString() );
        }
    }

    for ( int i = 0; i < M_FAT_IRS_TARIHI_LIST.count(); i++ ) {
        M_FAT_IRS_TARIHI_STR.append     ( M_FAT_IRS_TARIHI_LIST.at( i ) );
        if ( i < M_FAT_IRS_TARIHI_LIST.count() -1 ) {
            M_FAT_IRS_TARIHI_STR.append( ", " );
        }
        M_FAT_IRSALIYE_NO_STR.append    ( M_FAT_IRS_NO_LIST.at( i ) );
        if ( i < M_FAT_IRS_TARIHI_LIST.count() -1 ) {
            M_FAT_IRSALIYE_NO_STR.append( ", " );
        }
    }

    //FOOTER
    M_FAT_BRUT_TUTAR_STR        = f_query.VALUE( "brut_tutar"                   ).toString();
    M_FAT_ISKONTO_YUZDESI_STR   = f_query.VALUE( "iskonto_yuzdesi"              ).toString();
    M_FAT_ISKONTO_TUTARI_STR    = f_query.VALUE( "iskonto_tutari"               ).toString();
    M_FAT_ARA_TOPLAM_STR        = f_query.VALUE( "ara_toplam"                   ).toString();
    M_FAT_OTV_TUTARI_STR        = f_query.VALUE( "isk_sonrasi_otv_tutari"       ).toString();
    M_FAT_OTV_DAHIL_TUTAR_STR   = f_query.VALUE( "isk_sonrasi_otv_dahil_tutar"  ).toString();
    M_FAT_KDV_TUTARI_STR        = f_query.VALUE( "isk_sonrasi_kdv_tutari"       ).toString();
    M_FAT_TUTARI_RAKAMLA_STR    = f_query.VALUE( "fatura_tutari"                ).toString();

    M_FAT_TUTARI_YAZIYLA_STR    = ADAK_YAZIYLA_YAZ ( f_query.VALUE ( "fatura_tutari" ).toDouble() );

    int cari_hesap_id           = f_query.VALUE ( "cari_hesap_id" ).toInt();

    BORC_BILGISI borc_bilgisi;

    CARI_KART_BORC_BILGISINI_GETIR ( cari_hesap_id, &borc_bilgisi );  // borc bilgisini donderir
    DVZ_GET_DOVIZ_KODU                 ( borc_bilgisi.para_birimi_id );

    if ( borc_bilgisi.borc_bakiye > 0.0 ) {
        M_FAT_SON_CAR_BAK_STR += QObject::tr ( "Borcu : " ) + VIRGUL_EKLE ( QVariant(borc_bilgisi.borc_bakiye  ).toString(), 2 );
    }
    else if ( borc_bilgisi.alacak_bakiye > 0.0 ) {
        M_FAT_SON_CAR_BAK_STR += QObject::tr ( "Alacağı : " ) + VIRGUL_EKLE ( QVariant ( borc_bilgisi.alacak_bakiye ).toString() , 2 );
    }
    else {
        M_FAT_SON_CAR_BAK_STR += QObject::tr ( "Borcu / Alacağı : 0.00" );
    }


    f_query.PREPARE_SELECT("fat_fatura_satirlari" ,
                           "urun_id, urun_adi, satirdaki_urun_miktari, "
                           "urun_birimi, isk_oncesi_urun_birim_fiyati, isk_sonrasi_urun_birim_fiyati, "
                           "satir_iskonto_yuzdesi, satir_iskonto_tutari, satir_tutari, doviz_id, "
                           "kdv_dahil_mi, kdv_orani, otv_orani, satir_vergi_haric_tutar, satir_otv_tutari, "
                           "satir_otv_dahil_tutar, satir_kdv_tutari, satir_kdv_dahil_tutar ",
                           "fis_id = :fis_id");

    f_query.SET_VALUE      ( ":fis_id", m_fis_id );

    if ( f_query.SELECT("order_number ASC") EQ 0 ) {
        return;
    }

    while ( f_query.NEXT() EQ true ) {

        //LINE
        int urun_id = f_query.VALUE ( "urun_id" ).toInt();

        if ( urun_id > 0 ) {

            QString urun_kodu;
            QString urun_adi;

            STK_GET_URUN_KODU_ADI(urun_id , &urun_kodu , &urun_adi);

            M_FAT_URUN_KODU    << urun_kodu;
            M_FAT_URUN_CINSI   << urun_adi;

        }
        else {
            M_FAT_URUN_CINSI << f_query.VALUE ( "urun_adi" ).toString();
        }

        // SATIR TUTAR 0 ISE ACIKLAMA SATIRIDIR
        if ( f_query.VALUE ( "satir_tutari" ).toDouble() NE 0.00 ) {
            M_FAT_URUN_BIRIMI                   << f_query.VALUE ( "urun_birimi"                    ).toString();
            M_FAT_URUN_MIKTARI                  << f_query.VALUE ( "satirdaki_urun_miktari"         ).toString();
            M_FAT_SAT_ISK_ONCESI_BIRIM_FIYAT    << f_query.VALUE ( "isk_oncesi_urun_birim_fiyati"   ).toString();
            M_FAT_SAT_ISK_SONRASI_BIRIM_FIYAT   << f_query.VALUE ( "isk_sonrasi_urun_birim_fiyati"  ).toString();
            M_FAT_SAT_ISKONTO_YUZDESI           << f_query.VALUE ( "satir_iskonto_yuzdesi"          ).toString();
            M_FAT_SAT_ISKONTO_TUTARI            << f_query.VALUE ( "satir_iskonto_tutari"           ).toString();
            M_FAT_SAT_SATIR_TUTARI              << f_query.VALUE ( "satir_tutari"                   ).toString();
            M_FAT_KDV_ORANI                     << f_query.VALUE ( "kdv_orani"                      ).toString();
            M_FAT_OTV_ORANI                     << f_query.VALUE ( "otv_orani"                      ).toString();
            M_FAT_SAT_VERGI_HARIC_TUTAR         << f_query.VALUE ( "satir_vergi_haric_tutar"        ).toString();
            M_FAT_SAT_OTV_TUTARI                << f_query.VALUE ( "satir_otv_tutari"               ).toString();
            M_FAT_SAT_OTV_DAHIL_TUTAR           << f_query.VALUE ( "satir_otv_dahil_tutar"          ).toString();
            M_FAT_SAT_KDV_TUTARI                << f_query.VALUE ( "satir_kdv_tutari"               ).toString();
            M_FAT_SAT_KDV_DAHIL_TUTAR           << f_query.VALUE ( "satir_kdv_dahil_tutar"          ).toString();
        }
        else {
            M_FAT_URUN_BIRIMI                   << "";
            M_FAT_URUN_MIKTARI                  << "";
            M_FAT_SAT_ISK_ONCESI_BIRIM_FIYAT    << "";
            M_FAT_SAT_ISK_SONRASI_BIRIM_FIYAT   << "";
            M_FAT_SAT_ISKONTO_YUZDESI           << "";
            M_FAT_SAT_ISKONTO_TUTARI            << "";
            M_FAT_SAT_SATIR_TUTARI              << "";
            M_FAT_KDV_ORANI                     << "";
            M_FAT_OTV_ORANI                     << "";
            M_FAT_SAT_VERGI_HARIC_TUTAR         << "";
            M_FAT_SAT_OTV_TUTARI                << "";
            M_FAT_SAT_OTV_DAHIL_TUTAR           << "";
            M_FAT_SAT_KDV_TUTARI                << "";
            M_FAT_SAT_KDV_DAHIL_TUTAR           << "";
        }

        M_FAT_SAT_DOVIZ_KODU                << DVZ_GET_DOVIZ_KODU ( f_query.VALUE ( "doviz_id" ).toInt() );

        if ( f_query.VALUE ( "kdv_dahil_mi" ).toInt() EQ 1 ) {
            M_FAT_KDV_DAHIL_MI  << QObject::tr ( "Dahil" );
        }
        else {
            M_FAT_KDV_DAHIL_MI  << QObject::tr ( "Hariç" );
        }
    }
}

/**************************************************************************************
                   FAT_FATURA_BELGESI::GET_HEADER_INFO
***************************************************************************************/

void FAT_FATURA_BELGESI::GET_HEADER_INFO ()
{
    SET_HEADER_DATA ( FAT_ADR         , M_FAT_ADR_STR         );
    SET_HEADER_DATA ( FAT_VERGI_DAIRE , M_FAT_VERGI_DAIRE_STR );
    SET_HEADER_DATA ( FAT_VERGI_NO    , M_FAT_VERGI_NO_STR    );
    SET_HEADER_DATA ( FAT_SERISIRA    , M_FAT_SERISIRA_STR    );
    SET_HEADER_DATA ( FAT_TARIHI      , M_FAT_TARIHI_STR      );
    SET_HEADER_DATA ( FAT_IRS_TARIHI  , M_FAT_IRS_TARIHI_STR  );
    SET_HEADER_DATA ( FAT_IRSALIYE_NO , M_FAT_IRSALIYE_NO_STR );
    SET_HEADER_DATA ( FAT_DUZEN_TAR   , M_FAT_DUZEN_TAR_STR   );
    SET_HEADER_DATA ( FAT_DUZEN_SAAT  , M_FAT_DUZEN_SAAT_STR  );
    SET_HEADER_DATA ( FAT_SEVK_ADR    , M_FAT_SEVK_ADR_STR    );
}

/**************************************************************************************
                   FAT_FATURA_BELGESI::GET_FOOTER_INFO
***************************************************************************************/

void FAT_FATURA_BELGESI::GET_FOOTER_INFO ()
{
    SET_FOOTER_DATA ( FAT_BRUT_TUTAR        , VIRGUL_EKLE(M_FAT_BRUT_TUTAR_STR      ));

    if (M_FAT_ISKONTO_YUZDESI_STR.isEmpty() EQ false) {
        M_FAT_ISKONTO_YUZDESI_STR.prepend("%");
    }
    SET_FOOTER_DATA ( FAT_ISKONTO_YUZDESI   , M_FAT_ISKONTO_YUZDESI_STR              );
    SET_FOOTER_DATA ( FAT_ISKONTO_TUTARI    , VIRGUL_EKLE(M_FAT_ISKONTO_TUTARI_STR ) );
    SET_FOOTER_DATA ( FAT_ARA_TOPLAM        , VIRGUL_EKLE(M_FAT_ARA_TOPLAM_STR     ) );
    SET_FOOTER_DATA ( FAT_OTV_TUTARI        , VIRGUL_EKLE(M_FAT_OTV_TUTARI_STR     ) );
    SET_FOOTER_DATA ( FAT_OTV_DAHIL_TUTAR   , VIRGUL_EKLE(M_FAT_OTV_DAHIL_TUTAR_STR) );
    SET_FOOTER_DATA ( FAT_KDV_TUTARI        , VIRGUL_EKLE(M_FAT_KDV_TUTARI_STR     ) );
    SET_FOOTER_DATA ( FAT_TUTARI_RAKAMLA    , VIRGUL_EKLE(M_FAT_TUTARI_RAKAMLA_STR ) );
    SET_FOOTER_DATA ( FAT_TUTARI_YAZIYLA    , M_FAT_TUTARI_YAZIYLA_STR               );
    SET_FOOTER_DATA ( FAT_SON_CAR_BAK       , M_FAT_SON_CAR_BAK_STR                  );
    SET_FOOTER_DATA ( FAT_KDV_1             , M_FAT_KDV_1_STR                        );
    SET_FOOTER_DATA ( FAT_KDV_2             , M_FAT_KDV_2_STR                        );
    SET_FOOTER_DATA ( FAT_KDV_3             , M_FAT_KDV_3_STR                        );
    SET_FOOTER_DATA ( FAT_KDV_4             , M_FAT_KDV_4_STR                        );
    SET_FOOTER_DATA ( FAT_KDV_5             , M_FAT_KDV_5_STR                        );
}

/**************************************************************************************
                   FAT_FATURA_BELGESI::GET_LINE_INFO
***************************************************************************************/

void FAT_FATURA_BELGESI::GET_LINE_INFO ()
{
    QString FAT_URUN_KODU_STR                    = "";
    QString FAT_URUN_CINSI_STR                   = "";
    QString FAT_URUN_BIRIMI_STR                  = "";
    QString FAT_URUN_MIKTARI_STR                 = "";
    QString FAT_SAT_ISK_ONCESI_BIRIM_FIYAT_STR   = "";
    QString FAT_SAT_ISK_SONRASI_BIRIM_FIYAT_STR  = "";
    QString FAT_SAT_ISKONTO_YUZDESI_STR          = "";
    QString FAT_SAT_ISKONTO_TUTARI_STR           = "";
    QString FAT_SAT_SATIR_TUTARI_STR             = "";
    QString FAT_SAT_DOVIZ_KODU_STR               = "";
    QString FAT_KDV_DAHIL_MI_STR                 = "";
    QString FAT_SAT_VERGI_HARIC_TUTAR_STR        = "";
    QString FAT_SAT_OTV_TUTARI_STR               = "";
    QString FAT_SAT_OTV_DAHIL_TUTAR_STR          = "";
    QString FAT_SAT_KDV_TUTARI_STR               = "";
    QString FAT_SAT_KDV_DAHIL_TUTAR_STR          = "";
    QString FAT_KDV_ORANI_STR                    = "";
    QString FAT_OTV_ORANI_STR                    = "";

    if ( m_line_index < M_FAT_URUN_KODU.size() ) {
        FAT_URUN_KODU_STR                   = M_FAT_URUN_KODU.at                    ( m_line_index );
    }
    if ( m_line_index < M_FAT_URUN_CINSI.size() ) {
        FAT_URUN_CINSI_STR                  = M_FAT_URUN_CINSI.at                   ( m_line_index );
        FAT_URUN_BIRIMI_STR                 = M_FAT_URUN_BIRIMI.at                  ( m_line_index );
        FAT_URUN_MIKTARI_STR                = M_FAT_URUN_MIKTARI.at                 ( m_line_index );
        FAT_SAT_ISK_ONCESI_BIRIM_FIYAT_STR  = M_FAT_SAT_ISK_ONCESI_BIRIM_FIYAT.at   ( m_line_index );
        FAT_SAT_ISK_SONRASI_BIRIM_FIYAT_STR = M_FAT_SAT_ISK_SONRASI_BIRIM_FIYAT.at  ( m_line_index );
        FAT_SAT_ISKONTO_YUZDESI_STR         = M_FAT_SAT_ISKONTO_YUZDESI.at          ( m_line_index );
        FAT_SAT_ISKONTO_TUTARI_STR          = M_FAT_SAT_ISKONTO_TUTARI.at           ( m_line_index );
        FAT_SAT_SATIR_TUTARI_STR            = M_FAT_SAT_SATIR_TUTARI.at             ( m_line_index );
        FAT_KDV_DAHIL_MI_STR                = M_FAT_KDV_DAHIL_MI.at                 ( m_line_index );
        FAT_SAT_VERGI_HARIC_TUTAR_STR       = M_FAT_SAT_VERGI_HARIC_TUTAR.at        ( m_line_index );
        FAT_SAT_OTV_TUTARI_STR              = M_FAT_SAT_OTV_TUTARI.at               ( m_line_index );
        FAT_SAT_OTV_DAHIL_TUTAR_STR         = M_FAT_SAT_OTV_DAHIL_TUTAR.at          ( m_line_index );
        FAT_SAT_KDV_TUTARI_STR              = M_FAT_SAT_KDV_TUTARI.at               ( m_line_index );
        FAT_SAT_KDV_DAHIL_TUTAR_STR         = M_FAT_SAT_KDV_DAHIL_TUTAR.at          ( m_line_index );
        FAT_KDV_ORANI_STR                   = M_FAT_KDV_ORANI.at                    ( m_line_index );
        FAT_OTV_ORANI_STR                   = M_FAT_OTV_ORANI.at                    ( m_line_index );

        if ( FAT_SAT_SATIR_TUTARI_STR.isEmpty() NE true ) {
            FAT_SAT_DOVIZ_KODU_STR          = M_FAT_SAT_DOVIZ_KODU.at               ( m_line_index );
        }
    }

    SET_LINE_DATA ( FAT_URUN_KODU,              FAT_URUN_KODU_STR                                 );
    SET_LINE_DATA ( FAT_URUN_CINSI,             FAT_URUN_CINSI_STR                                );
    SET_LINE_DATA ( FAT_URUN_BIRIMI,            FAT_URUN_BIRIMI_STR                               );
    SET_LINE_DATA ( FAT_URUN_MIKTARI,           FAT_URUN_MIKTARI_STR                              );
    SET_LINE_DATA ( FAT_IS_ON_BIRIM_FIYAT,      VIRGUL_EKLE(FAT_SAT_ISK_ONCESI_BIRIM_FIYAT_STR )  );
    SET_LINE_DATA ( FAT_IS_SON_BIRIM_FIYAT,     VIRGUL_EKLE(FAT_SAT_ISK_SONRASI_BIRIM_FIYAT_STR)  );
    SET_LINE_DATA ( FAT_SATIR_IS_TUTARI,        VIRGUL_EKLE(FAT_SAT_ISKONTO_TUTARI_STR   )        );
    SET_LINE_DATA ( FAT_SATIR_TUTARI,           VIRGUL_EKLE(FAT_SAT_SATIR_TUTARI_STR     )        );
    SET_LINE_DATA ( FAT_SATIR_DOVIZ_KODU,       FAT_SAT_DOVIZ_KODU_STR                            );
    SET_LINE_DATA ( FAT_SATIR_KDV_DAHIL_MI,     FAT_KDV_DAHIL_MI_STR                              );
    SET_LINE_DATA ( FAT_SATIR_VERGI_HRC_TUTAR,  VIRGUL_EKLE(FAT_SAT_VERGI_HARIC_TUTAR_STR)        );
    SET_LINE_DATA ( FAT_SATIR_OTV_TUTARI,       VIRGUL_EKLE(FAT_SAT_OTV_TUTARI_STR     )          );
    SET_LINE_DATA ( FAT_SATIR_OTV_DAHIL_TUTAR,  VIRGUL_EKLE(FAT_SAT_OTV_DAHIL_TUTAR_STR)          );
    SET_LINE_DATA ( FAT_SATIR_KDV_TUTARI,       VIRGUL_EKLE(FAT_SAT_KDV_TUTARI_STR   )            );
    SET_LINE_DATA ( FAT_SATIR_KDV_DAHIL_TUTAR,  VIRGUL_EKLE(FAT_SAT_KDV_DAHIL_TUTAR_STR)          );

    if (FAT_SAT_ISKONTO_YUZDESI_STR.isEmpty() EQ false) {
        FAT_SAT_ISKONTO_YUZDESI_STR.prepend("%");
    }
    SET_LINE_DATA ( FAT_SATIR_IS_YUZDESI,       FAT_SAT_ISKONTO_YUZDESI_STR                       );

    if (FAT_KDV_ORANI_STR.isEmpty() EQ false) {
        FAT_KDV_ORANI_STR.prepend("%");
    }
    SET_LINE_DATA ( FAT_SATIR_KDV_ORANI, FAT_KDV_ORANI_STR );

    if (FAT_OTV_ORANI_STR.isEmpty() EQ false) {
        FAT_OTV_ORANI_STR.prepend("%");
    }
    SET_LINE_DATA ( FAT_SATIR_OTV_ORANI, FAT_OTV_ORANI_STR );

    m_line_index++;
}
