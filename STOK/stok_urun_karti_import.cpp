#include <QString>
#include "adak_sql.h"
#include "adak_gruplar.h"
#include "stok_urun_karti_import_class.h"
#include "stok_urun_karti_import_open.h"
#include "stok_enum.h"
#include "dvz_console_utils.h"
#include "stok_struct.h"
#include "stok_console_utils.h"
#include "stok_fis_utils.h"
#include "sube_console_utils.h"
#include "fat_enum.h"
#include "e9_enum.h"

extern ADAK_SQL * DB;

/***********************************************************************************
                                 OPEN_STOK_URUN_KARTI_IMPORT
************************************************************************************/

void OPEN_STOK_URUN_KARTI_IMPORT ( QWidget * parent )
{
    STOK_URUN_KARTI_IMPORT * A = new STOK_URUN_KARTI_IMPORT(parent);
    A->EXEC( NOT_FULL_SCREEN );
}

/***********************************************************************************
                            ADRES_EXPORT::ADRES_EXPORT
************************************************************************************/

STOK_URUN_KARTI_IMPORT::STOK_URUN_KARTI_IMPORT(QWidget * parent) : IMPORT_KERNEL ( "", parent)
{
    QList<QStringList> baslik_ve_kontrol;

    baslik_ve_kontrol.append( QStringList() <<  "Ürün Kodu"                 << "30" << "*"  );
    baslik_ve_kontrol.append( QStringList() <<  "Ürün Adı"                  << "50" << "*"  );
    baslik_ve_kontrol.append( QStringList() <<  "Barkod Numarası"           << "50" << ""   );
    baslik_ve_kontrol.append( QStringList() <<  "Stok Takip Şekli"          << ""   << "0"  ); // temel birim defaut ENUM_BASIT_TAKIP
    baslik_ve_kontrol.append( QStringList() <<  "Stok Miktarı"              << ""   << ""   );
    baslik_ve_kontrol.append( QStringList() <<  "Temel Birim"               << ""   << "ADET");
    baslik_ve_kontrol.append( QStringList() <<  "Temel Alış F."             << ""   << ""   );
    baslik_ve_kontrol.append( QStringList() <<  "Temel Alış Para Brimi"     << ""   << "TL" );
    baslik_ve_kontrol.append( QStringList() <<  "Temel Alış KDV Oranı"      << ""   << ""   );
    baslik_ve_kontrol.append( QStringList() <<  "Temel Alış ÖTV Oranı"      << ""   << ""   );
    baslik_ve_kontrol.append( QStringList() <<  "Temel Satiş F."            << ""   << ""   );
    baslik_ve_kontrol.append( QStringList() <<  "Temel Satiş Para Birimi"   << ""   << "TL" );
    baslik_ve_kontrol.append( QStringList() <<  "Temel Satiş KDV Oranı"     << ""   << ""   );
    baslik_ve_kontrol.append( QStringList() <<  "Temel Satiş ÖTV Oranı"     << ""   << ""   );
    baslik_ve_kontrol.append( QStringList() <<  "Açıklama"                  << "512"<< ""   );

    SET_HEADERS    ( baslik_ve_kontrol );

    m_fis_id = -1 ;

    SET_PROGRAM_ID      ( E9_PROGRAMI );
    SET_MODUL_ID        ( STOK_MODULU );
}

/***********************************************************************************
                            ADRES_EXPORT::IMPORT_ROW
************************************************************************************/

void STOK_URUN_KARTI_IMPORT::IMPORT_ROW( QStringList p_row_data )
{
    SQL_QUERY query_select( DB );
    query_select.PREPARE_SELECT( "stk_urunler" , "urun_id , urun_adi" , "urun_kodu=:urun_kodu" );
    query_select.SET_VALUE     ( ":urun_kodu"  ,  p_row_data.at( 0 )                );

    if( query_select.SELECT() NE 0 ){
        return ;
    }

    int sube_id = SUBE_GET_SUBE_ID();
    int temel_para_birimi_id = DVZ_GET_TEMEL_PARA_BIRIMI_ID();
    QString temel_para_birimi_kodu = DVZ_GET_TEMEL_PARA_BIRIMI_KODU();

    STK_URUNLER_STRUCT  STOK_URUN ;

    STOK_URUN.urun_kodu           = p_row_data.at( 0 );
    STOK_URUN.urun_adi            = p_row_data.at( 1 );
    STOK_URUN.barkod_numarasi     = p_row_data.at( 2 );
    STOK_URUN.stok_takip_sekli    = ENUM_BASIT_TAKIP;
    STOK_URUN.urun_temel_birimi   = p_row_data.at( 5 );
    STOK_URUN.alis_fiyati         = p_row_data.at( 6 ).toDouble();
    STOK_URUN.urun_alis_doviz_id  = DVZ_GET_DOVIZ_ID( p_row_data.at( 7 ) );
    STOK_URUN.alis_kdv_orani      = p_row_data.at( 8 ).toDouble();
    STOK_URUN.alis_otv_orani      = p_row_data.at( 9 ).toDouble();
    STOK_URUN.satis_fiyati        = p_row_data.at( 10 ).toDouble();
    STOK_URUN.urun_satis_doviz_id = DVZ_GET_DOVIZ_ID( p_row_data.at( 11 ) );
    STOK_URUN.satis_kdv_orani     = p_row_data.at( 12 ).toDouble();
    STOK_URUN.satis_otv_orani     = p_row_data.at( 13 ).toDouble();
    STOK_URUN.aciklama            = p_row_data.at( 14 );
    STOK_URUN.muh_hesap_id        = SUBE_GET_SUBE_ENT_HESAP_ID(sube_id, "urunler_ent_hesap_id"); // default hesap
    STOK_URUN.hizmet_mi           = ENUM_URUN_KARTI ;

    int urun_id = STK_URUN_KARTI_EKLE( &STOK_URUN ) ;

    if( m_fis_id EQ -1 ){
        STOK_FIS = new STK_IRS_FIS_STRUCT;

        STK_IRS_CLEAR_FIS_STRUCT(STOK_FIS);

        STOK_FIS->fis_no        = STK_SIRADAKI_FIS_NO_AL( MALI_YIL_FIRST_DATE().toString("yyyy.MM.dd") ) ;
        STOK_FIS->fis_turu      = ENUM_STK_STOK_ACILIS_FISI ;
        STOK_FIS->aciklama      = QString("İmport İşlemi Esnasında Üretilmiştir.");
        STOK_FIS->fis_tarihi    = MALI_YIL_FIRST_DATE().toString("yyyy.MM.dd");
        STOK_FIS->program_id    = E9_PROGRAMI ;
        STOK_FIS->modul_id      = STOK_MODULU ;
        STOK_FIS->irsaliye_mi_stok_fisi_mi = ENUM_STOK_FISI;
        STOK_FIS->depo_id       = SUBE_GET_UNITENIN_DEPOSU();
        STOK_FIS->iade_irsaliyesi_mi = 0;
        STOK_FIS->irsaliye_alis_satis_turu = ENUM_ALIS_IRSALIYESI_FATURASI;
        STOK_FIS->doviz_tutar_bilgileri.clear();
        STOK_FIS->kdv_oran_bilgileri.clear();
        STOK_FIS->otv_oran_bilgileri.clear();
        STOK_FIS->stk_irs_para_birimi_id = temel_para_birimi_id;

        m_fis_id = STK_IRS_FIS_EKLE( STOK_FIS );
    }

    STK_IRS_FIS_SATIR_STRUCT STOK_FIS_SATIRI ;

    STK_IRS_CLEAR_SATIR_STRUCT(&STOK_FIS_SATIRI);

    STOK_FIS_SATIRI.fis_id                 = m_fis_id ;
    STOK_FIS_SATIRI.urun_adi               = STOK_URUN.urun_adi;
    STOK_FIS_SATIRI.urun_id                = urun_id ;
    STOK_FIS_SATIRI.urun_birimi            = STOK_URUN.urun_temel_birimi ;
    STOK_FIS_SATIRI.muh_hesap_id           = STOK_URUN.muh_hesap_id ;
    STOK_FIS_SATIRI.satirdaki_urun_miktari = p_row_data.at( 4 ).toDouble();
    STOK_FIS_SATIRI.isk_oncesi_urun_birim_fiyati = STOK_URUN.satis_fiyati;
    STOK_FIS_SATIRI.isk_sonrasi_urun_birim_fiyati= STOK_URUN.satis_fiyati;
    STOK_FIS_SATIRI.temel_birim_katsayisi  = 1;
    STOK_FIS_SATIRI.iade_edilen_miktar     = 0;
    STOK_FIS_SATIRI.stok_takip_sekli       = STOK_URUN.stok_takip_sekli ;
    STOK_FIS_SATIRI.base_record_id         = 0 ;
    STOK_FIS_SATIRI.modul_id               = STOK_MODULU ;
    STOK_FIS_SATIRI.program_id             = E9_PROGRAMI ;
    STOK_FIS_SATIRI.doviz_id               = temel_para_birimi_id;
    STOK_FIS_SATIRI.doviz_kodu             = temel_para_birimi_kodu;

    STK_FIS_SATIR_TUTARLARINI_HESAPLA( STOK_FIS, &STOK_FIS_SATIRI );

    int irsaliye_satir_id = STK_IRS_FIS_SATIRI_EKLE ( STOK_FIS, &STOK_FIS_SATIRI );

    STK_IRS_FIS_SATIR_HAREKETLERI_OLUSTUR( STOK_FIS, &STOK_FIS_SATIRI, irsaliye_satir_id );

    GRP_KAYDI_GRUPLARA_EKLE( E9_PROGRAMI, STOK_MODULU, urun_id, GET_GRUP_IDLERI() );

}
