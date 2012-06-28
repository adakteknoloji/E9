#include <QString>

#include "adak_sql.h"
#include "adak_gruplar.h"

#include "prs_karti_import_class.h"
#include "prs_karti_import_open.h"
#include "cari_console_utils.h"
#include "cari_struct.h"
#include "cari_enum.h"
#include "cari_fis_utils.h"
#include "dvz_console_utils.h"
#include "prs_con_utils.h"
#include "prs_enum.h"
#include "sube_console_utils.h"

#include <QList>

extern ADAK_SQL * DB;

/***********************************************************************************
                                 OPEN_PRS_KARTI_IMPORT
************************************************************************************/

void OPEN_PRS_KARTI_IMPORT (QWidget * parent)
{
    PRS_KARTI_IMPORT * A = new PRS_KARTI_IMPORT(parent);
    A->EXEC( NOT_FULL_SCREEN );
}

/***********************************************************************************
                            PRS_KARTI_IMPORT::PRS_KARTI_IMPORT
************************************************************************************/

PRS_KARTI_IMPORT::PRS_KARTI_IMPORT(QWidget * parent) : IMPORT_KERNEL ( "personel_karti_iceriye-kayit-al ", parent)
{
    QList<QStringList> baslik_ve_kontrol;

    baslik_ve_kontrol.append( QStringList() <<  "TC Kimlik No"      << "11" << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Sicil No"          << "15" << "*");
    baslik_ve_kontrol.append( QStringList() <<  "SSK No"            << "15" << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Adı Soyadı"        << "60" << "");
    baslik_ve_kontrol.append( QStringList() <<  "İşe Giriş Tarihi"  << "10" << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Brüt Ücret"        << ""   << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Kümülatif V.M"     << ""   << "");
    baslik_ve_kontrol.append( QStringList() <<  "Toplam Borç"       << ""   << "");
    baslik_ve_kontrol.append( QStringList() <<  "Toplam Alacak"     << ""   << "");
    baslik_ve_kontrol.append( QStringList() <<  "Para Birimi"       << ""   << "TL");
    baslik_ve_kontrol.append( QStringList() <<  "Cinsiyeti"         << "5"  << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Medeni Hali"       << "10" << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Şehir"             << "30" << "");
    baslik_ve_kontrol.append( QStringList() <<  "İlçe"              << "30" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Semt"              << "30" << "");

    SET_HEADERS    ( baslik_ve_kontrol );

    SET_PROGRAM_ID ( E9_PROGRAMI   );
    SET_MODUL_ID   ( ADRES_MODULU  );

    m_fis_id = -1 ;
}

/***********************************************************************************
                            PRS_KARTI_IMPORT::IMPORT_ROW
************************************************************************************/

void PRS_KARTI_IMPORT::IMPORT_ROW( QStringList kayit_bilgisi_list )
{
    if ( kayit_bilgisi_list.size() <= 0 ) {
        return;
    }

    SQL_QUERY query_select( DB );

    query_select.PREPARE_SELECT( "prs_personeller" , "personel_id" , "sicil_no=:sicil_no");
    query_select.SET_VALUE( ":sicil_no"  , kayit_bilgisi_list.at( 1 ) );

    if( query_select.SELECT() NE 0 ){
        return ;
    }

    double cari_borc   = kayit_bilgisi_list.at(7).toDouble();
    double cari_alacak = kayit_bilgisi_list.at(8).toDouble();

    int sube_id = SUBE_GET_SUBE_ID();
    int personel_borclari_ent_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID(sube_id, "personel_borclari_ent_hesap_id");
    int prs_avanslari_ent_hesap_id     = SUBE_GET_SUBE_ENT_HESAP_ID(sube_id, "prs_avanslari_ent_hesap_id");

    CARI_HESAP_STRUCT         PRS_HESAP_KARTI  ;
    CARI_FIS_SATIRI_STRUCT    PRS_FIS_SATIRI ;

    //! [1] CARI HESAP EKLENIYOR
    CARI_SET_HESAP_STRUCT_DEFAULTS( &PRS_HESAP_KARTI );

    PRS_HESAP_KARTI.kimlik_no       =  kayit_bilgisi_list.at( 0 );
    PRS_HESAP_KARTI.cari_hesap_kodu =  kayit_bilgisi_list.at( 1 );
    PRS_HESAP_KARTI.cari_hesap_ismi =  kayit_bilgisi_list.at( 3 ).mid(0, 60);
    PRS_HESAP_KARTI.para_birimi_id  =  DVZ_GET_DOVIZ_ID( kayit_bilgisi_list.at(9) ) ;
    PRS_HESAP_KARTI.cinsiyet        =  kayit_bilgisi_list.at( 10 );
    PRS_HESAP_KARTI.medeni_hali     =  kayit_bilgisi_list.at( 11 );
    PRS_HESAP_KARTI.sehir           =  kayit_bilgisi_list.at( 12 );
    PRS_HESAP_KARTI.ilce            =  kayit_bilgisi_list.at( 13 );
    PRS_HESAP_KARTI.semt            =  kayit_bilgisi_list.at( 14 );
    PRS_HESAP_KARTI.program_id      =  E9_PROGRAMI ;
    PRS_HESAP_KARTI.modul_id        =  PERSONEL_MODULU ;
    PRS_HESAP_KARTI.cari_kart_turu  =  SAHIS_KAYDI;
    PRS_HESAP_KARTI.cari_kart_tipi  =  ENUM_CARI_PERSONEL;
    PRS_HESAP_KARTI.muh_hesap_id    =  personel_borclari_ent_hesap_id;


    int cari_hesap_id = CARI_KART_OLUSTUR( SAHIS_KAYDI, &PRS_HESAP_KARTI ) ;
    //! [1] END CARI HESAP

    //! [2] PERSONEL KAYDI EKLENIYOR

    SQL_QUERY query( DB );
    query.PREPARE_INSERT("prs_personeller","personel_id",
                         "sube_id, cari_hesap_id,"
                         "sicil_no,"
                         "sigorta_no,"
                         "ucret_sekli, brut_ucret, "
                         "calisma_sekli, kumulatif_vergi_matrahi, "
                         "bir_ay_once_kalan_ek_odenek, iki_ay_once_kalan_ek_odenek, "
                         "prs_avanslar_hesap_id, prs_borclar_hesap_id, isten_ayrildi_mi");

    query.SET_VALUE( ":cari_hesap_id",               cari_hesap_id );
    query.SET_VALUE( ":sicil_no",                    kayit_bilgisi_list.at( 1 ) );
    query.SET_VALUE( ":sigorta_no",                  kayit_bilgisi_list.at( 2 ) );
    query.SET_VALUE( ":ucret_sekli",                 PRS_BRUT );
    query.SET_VALUE( ":calisma_sekli",               ENUM_AYLIK   );
    query.SET_VALUE( ":brut_ucret",                  kayit_bilgisi_list.at( 5 ).toDouble() );
    query.SET_VALUE( ":kumulatif_vergi_matrahi",     kayit_bilgisi_list.at( 6 ).toDouble() );
    query.SET_VALUE( ":bir_ay_once_kalan_ek_odenek", 0.00 );
    query.SET_VALUE( ":iki_ay_once_kalan_ek_odenek", 0.00 );
    query.SET_VALUE( ":prs_borclar_hesap_id",        personel_borclari_ent_hesap_id);
    query.SET_VALUE( ":prs_avanslar_hesap_id",       prs_avanslari_ent_hesap_id);
    query.SET_VALUE( ":isten_ayrildi_mi",            0 );
    query.SET_VALUE( ":sube_id",                     sube_id );

    int personel_id = query.INSERT();

    query.PREPARE_INSERT("prs_personel_kisisel_blgler", "kisisel_bilgi_id", "personel_id, ise_giris_tarihi");
    query.SET_VALUE     (":personel_id", personel_id );
    query.SET_VALUE     (":ise_giris_tarihi", kayit_bilgisi_list.at( 4 ) );
    query.INSERT();

    query.PREPARE_INSERT("prs_personel_nufus_blgler ","nufus_id", "personel_id");
    query.SET_VALUE( ":personel_id" , personel_id );
    query.INSERT();

    query.PREPARE_INSERT("prs_personel_notlari ","notlar_id", "personel_id ");
    query.SET_VALUE( ":personel_id" , personel_id );
    query.INSERT();

   //! [2]



   //! NOT : CARI ALISI FISINI SADECE CARI MODULU OLUSTURA BILIR
   //! BUNDAN DLLAYI MODUL ID OLARAK CARI GITMESI LAZIM......

    if ( cari_borc NE 0.00 OR cari_alacak NE 0.00 ) {

        if( m_fis_id EQ -1 ){

            PRS_FIS =  new CARI_FIS_STRUCT ;

            CARI_CLEAR_FIS_STRUCT( PRS_FIS );

            PRS_FIS->cari_hesap_id        =  0;
            PRS_FIS->program_id           =  E9_PROGRAMI ;
            PRS_FIS->modul_id             =  CARI_MODULU ;
            PRS_FIS->fis_turu             =  ENUM_CARI_ACILIS_FISI ;
            PRS_FIS->fis_no               =  CARI_SIRADAKI_FIS_NO_AL( MALI_YIL_FIRST_DATE().toString("yyyy.MM.dd") ) ;
            PRS_FIS->cari_para_birimi_id  =  PRS_HESAP_KARTI.para_birimi_id ;
            PRS_FIS->fis_tarihi           =  MALI_YIL_FIRST_DATE().toString("yyyy.MM.dd");
            PRS_FIS->aciklama             =  tr("PERSONEL KARTI - İmport İşlemi Esnasında Üretilmiştir.");
            PRS_FIS->base_fis_id          =  0;

            m_fis_id = CARI_FIS_EKLE( PRS_FIS ) ;
        }

        CARI_CLEAR_FIS_SATIRI_STRUCT( &PRS_FIS_SATIRI );

        PRS_FIS_SATIRI.cari_hesap_id         =  cari_hesap_id ;
        PRS_FIS_SATIRI.fis_id                =  m_fis_id ;
        PRS_FIS_SATIRI.program_id            =  E9_PROGRAMI ;
        PRS_FIS_SATIRI.modul_id              =  CARI_MODULU ;
        PRS_FIS_SATIRI.doviz_id              =  PRS_HESAP_KARTI.para_birimi_id;

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
            PRS_FIS_SATIRI.cari_para_birimi_tutari = cari_borc;
            PRS_FIS_SATIRI.alacak_tutari           = 0;
            PRS_FIS_SATIRI.borc_tutari             = cari_borc;

            CARI_FIS_SATIRI_EKLE( PRS_FIS , &PRS_FIS_SATIRI );
        }
        if ( cari_alacak > 0 )  {
            PRS_FIS_SATIRI.borc_tutari              = 0 ;
            PRS_FIS_SATIRI.alacak_tutari            = cari_alacak;
            PRS_FIS_SATIRI.cari_para_birimi_tutari  = cari_alacak;

            CARI_FIS_SATIRI_EKLE( PRS_FIS , &PRS_FIS_SATIRI );
        }
    }

    if ( GET_GRUP_IDLERI()->size() > 0 ) {

        GRP_KAYDI_GRUPLARA_EKLE( E9_PROGRAMI, PERSONEL_MODULU, personel_id , GET_GRUP_IDLERI() );
    }
}
