#include <QSplashScreen>
#include "yonetim.h"
#include "e9_console_utils.h"
#include "muh_console_utils.h"
#include "dmr_con_utils.h"
#include "sube_console_utils.h"
#include "fat_enum.h"
#include "fat_console_utils.h"
#include "e9_log.h"
#include "e9_yetki.h"
#include "banka_enum.h"
#include "kernel_utils.h"
#include "e9_version_upgrade.h"
#include "adak_utils.h"
#include "QAdakDate.h"
#include "prs_con_utils.h"
#include "cari_enum.h"
#include "e9_enum.h"
#include "e9_struct.h"
#include "bnk_console_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "dmr_enum.h"
#include "sube_enum.h"
#include "belge_struct.h"

extern ADAK_SQL        *DB;
extern ADAK_SQL        *G_YONETIM_DB;

extern ADAK_SQL_STRUCT  e9_db;

static bool              g_kdv_oranlari_veritabanindan_okundu_mu = false;
static bool              g_otv_oranlari_veritabanindan_okundu_mu = false;
static bool              g_muhasebe_alarmlari_kapalimi           = true;

static QString           g_firma_ismi = "";

static E9_SABIT_DEGERLER_STRUCT G_E9_SABIT_DEGERLER;
struct E9_KULLANICI_YETKILERI_STRUCT * E9_KULLANICI_YETKILERI = NULL;

/**************************************************************************************
                   E9_SET_DEFAULT_VALUES
***************************************************************************************/

void E9_SET_DEFAULT_VALUES()
{
    DB->START_TRANSACTION();

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("e9_sabit_degerler", "e9_sabit_degerler_atandi_mi" );
    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        if ( query.VALUE(0).toInt() EQ EVET ) {
            // KURULU ISE DEGERLERIN YUKLENMESI LAZIM
            E9_SET_SABIT_DEGERLER_STRUCT            ();
            SUBE_SET_CURRENT_SUBE_UNITE             ();
            SUBE_FILL_ENTEGRASYON_HESAPLARI_STRUCT  ();
            DB->CANCEL_TRANSACTION();
            return;
        }
    }

    MUH_CREATE_MUHASEBE_HESAPLARI                   ();
    E9_CREATE_SABIT_DEGERLER                        ();
    E9_SET_SABIT_DEGERLER_STRUCT                    ();
    MUH_CREATE_YANSITMA_HESAPLARI                   ();

    SUBE_CREATE_DEFAULT_SUBE_DEPO_UNITE_ENT_HESAPLARI();
    SUBE_SET_CURRENT_SUBE_UNITE                     ();
    SUBE_FILL_ENTEGRASYON_HESAPLARI_STRUCT          ();
    BNK_CREATE_BANKA_MAKROLARI                      ();
    PRS_CREATE_SABIT_DEGERLER                       ();
    PRS_CREATE_FIRMA_ODENEKLERI                     ();

    E9_KULLANICI_YETKILERI_FISI_VE_KULLANICI_YETKILERI_ORDER_NUM_OLUSTUR();

    QSplashScreen * splash = CREATE_SPLASH_SCREEN();
    splash->show();
    splash->showMessage((QString("Program ilk kullanım için hazırlanıyor...")),Qt::AlignCenter,Qt::white);

    int doviz_id = DVZ_GET_DOVIZ_ID ( "TL" , true);

    query.PREPARE_INSERT("dvz_degiskenler", "degisken_id","temel_para_birimi_id");
    query.SET_VALUE     (":temel_para_birimi_id", doviz_id );
    query.INSERT();

    DVZ_SET_TEMEL_PARA_BIRIMI_ID ( doviz_id );
    E9_ADD_DEFAULT_BELGELER();
    splash->finish(NULL);

    query.PREPARE_UPDATE("e9_sabit_degerler", "sabit_deger_id", "e9_sabit_degerler_atandi_mi", "");
    query.SET_VALUE     (":e9_sabit_degerler_atandi_mi", EVET );
    query.UPDATE();

    DB->COMMIT_TRANSACTION();
}


/**************************************************************************************
                  E9_GET_KULLANICI_YETKILERI
***************************************************************************************/

void *  E9_GET_KULLANICI_YETKILERI()
{
    return E9_KULLANICI_YETKILERI;
}

/*********************************************************/
/*        E9_KULLANICIYI_TAM_YETKILENDIR()           */
/*********************************************************/

void E9_KULLANICIYI_TAM_YETKILENDIR()
{
    SQL_QUERY query ( DB );

    int kullanici_id = KULLANICI_ID();

    query.PREPARE_SELECT("e9_kullanici_yetkileri","yetki_id","kullanici_id = :kullanici_id");
    query.SET_VALUE     (":kullanici_id", kullanici_id );

    if (query.SELECT() > 0) {
        return;
    }

    DB->START_TRANSACTION();

    query.PREPARE_INSERT( "e9_kullanici_yetkileri","yetki_id","kullanici_id");

    query.SET_VALUE(":kullanici_id", KULLANICI_ID());
    query.INSERT();

    query.PREPARE_INSERT("e9_kullanici_yetkileri_fisi",
                         "kullanici_yetkileri_fisi_id",
                         "lock_flag " );
    query.SET_VALUE(":lock_flag",1);
    query.INSERT();

    if ( E9_KULLANICI_YETKILERI EQ NULL ) {
        E9_KULLANICI_YETKILERI = new E9_KULLANICI_YETKILERI_STRUCT;
    }

 /*********************************************************  ADRES ************************************************/

    E9_KULLANICI_YETKILERI->adr_adres_karti_tanimlama_yetkisi       = 1;
    E9_KULLANICI_YETKILERI->adr_not_defteri_inceleme_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->adr_adres_kayitlari_rapor_alma_yetkisi  = 1;
    E9_KULLANICI_YETKILERI->adr_tel_web_kayitlari_rapor_alma_yetkisi= 1;
    E9_KULLANICI_YETKILERI->adr_etiket_yazdirma_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->adr_fihrist_raporu_alma_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->adr_toplu_mail_gonderme_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->adr_grup_tanimlama_yetkisi              = 1;
    E9_KULLANICI_YETKILERI->adr_toplu_sms_gonderme_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->adr_export_yetkisi                      = 1;
    E9_KULLANICI_YETKILERI->adr_import_yetkisi                      = 1;



/************************************************************** BANKA  *****************************************/

    E9_KULLANICI_YETKILERI->bnk_banka_hesabi_acma_yetkisi           = 1;
    E9_KULLANICI_YETKILERI->bnk_ent_fisleri_inceleme_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->bnk_defter_raporu_alma_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->bnk_pos_ekstresi_alma_yetkisi           = 1;
    E9_KULLANICI_YETKILERI->bnk_hareket_fisi_islem_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->bnk_acilis_fisi_islem_yetkisi           = 1;
    E9_KULLANICI_YETKILERI->bnk_makro_tanimlama_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->bnk_kk_ekstresi_alma_yetkisi            = 1;
    E9_KULLANICI_YETKILERI->bnk_virman_islem_yetkisi                = 1;
    E9_KULLANICI_YETKILERI->bnk_makro_fisi_islem_yetkisi            = 1;


/******************************************************************* CARI   *************************************/

    E9_KULLANICI_YETKILERI->cari_tahsilat_fisi_islem_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->cari_odeme_fisi_islem_yetkisi           = 1;
    E9_KULLANICI_YETKILERI->cari_hareket_fisi_islem_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->cari_acilis_fisi_islem_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->cari_grup_tanimlama_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->cari_kart_tanimlama_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->cari_ent_fisleri_inceleme_yetkisi       = 1;
    E9_KULLANICI_YETKILERI->cari_ekstre_alma_yetkisi                = 1;
    E9_KULLANICI_YETKILERI->cari_bakiye_raporu_alma_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->cari_virman_islem_yetkisi               = 1;
    E9_KULLANICI_YETKILERI->cari_hesap_hareketleri_gorebilir_mi     = 1;



/********************************************************* CEKSENET *****************************************************************/

    E9_KULLANICI_YETKILERI->cek_senet_girebilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->cek_bordrosu_islem_yetkisi              = 1;
    E9_KULLANICI_YETKILERI->cek_ent_fisleri_inceleme_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->cek_listesi_rapor_alma_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->cek_mus_cek_senet_arama_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->cek_acilis_bordrosu_islem_yetkisi       = 1;


/***************************************************** DEMIRBAS *****************************************************/

    E9_KULLANICI_YETKILERI->dmr_demirbas_karti_tanimlama_yetkisi    = 1;
    E9_KULLANICI_YETKILERI->dmr_grup_tanimlama_yetkisi              = 1;
    E9_KULLANICI_YETKILERI->dmr_yen_degerleme_fisi_islem_yetkisi    = 1;
    E9_KULLANICI_YETKILERI->dmr_amortisman_fisi_islem_yetkisi       = 1;
    E9_KULLANICI_YETKILERI->dmr_demirbas_listesi_rapor_alma_yetkisi = 1;
    E9_KULLANICI_YETKILERI->dmr_toplu_amortisman_ayirma_yetkisi     = 1;
    E9_KULLANICI_YETKILERI->dmr_toplu_yeniden_degerleme_yetkisi     = 1;
    E9_KULLANICI_YETKILERI->dmr_rapor_islem_grubu_tanimlama_yetkisi = 1;


/*********************************************************** MUHASEBE *******************************************/

    E9_KULLANICI_YETKILERI->muh_muhasebe_fisi_islem_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->muh_ent_fisleri_inceleme_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->muh_hesap_plani_islem_yetkisi           = 1;
    E9_KULLANICI_YETKILERI->muh_yvm_no_olusturma_yetkisi            = 1;
    E9_KULLANICI_YETKILERI->muh_hesap_ekstresi_alma_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->muh_mizan_raporu_alma_yetkisi           = 1;
    E9_KULLANICI_YETKILERI->muh_hesap_plani_rapor_alma_yetkisi      = 1;
    E9_KULLANICI_YETKILERI->muh_gelir_tablosu_rapor_alma_yetkisi    = 1;
    E9_KULLANICI_YETKILERI->muh_bilanco_rapor_alma_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->muh_yvm_dokumu_alma_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->muh_defteri_kebir_raporu_alma_yetkisi   = 1;
    E9_KULLANICI_YETKILERI->muh_fis_dokumu_alma_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->muh_hatali_fisleri_inceleme_yetkisi     = 1;
    E9_KULLANICI_YETKILERI->muh_grup_tanimlama_yetkisi              = 1;
    E9_KULLANICI_YETKILERI->muh_yansitma_hesabi_tanimlama_yetkisi   = 1;
    E9_KULLANICI_YETKILERI->muh_yansitma_fisi_olusturma_yetkisi     = 1;
    E9_KULLANICI_YETKILERI->muh_hesap_ekstresi_nakli_yekun          = 1;
    E9_KULLANICI_YETKILERI->muh_mizan_nakli_yekun                   = 1;
    E9_KULLANICI_YETKILERI->muh_hesap_plani_kopyalama_yetkisi       = 1;
    E9_KULLANICI_YETKILERI->muh_acilis_fisi_islem_yetkisi           = 1;



/***************************************************************** PERSONEL ***************************************/

    E9_KULLANICI_YETKILERI->prs_sicil_karti_tanimlama_yetkisi       = 1;
    E9_KULLANICI_YETKILERI->prs_ek_odenek_tanimlama_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->prs_ek_kesinti_tanimlama_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->prs_sabit_deger_guncelleme_yetkisi      = 1;
    E9_KULLANICI_YETKILERI->prs_maas_bordrosu_alma_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->prs_maas_bordrosu_iptal_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->prs_maas_bordrosu_raporu_alma_yetkisi   = 1;
    E9_KULLANICI_YETKILERI->prs_ihbar_ve_kidem_tazminati_raporu_alma_yetkisi = 1;
    E9_KULLANICI_YETKILERI->prs_isten_ayrilma_islem_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->prs_bordro_inceleme_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->prs_personel_raporu_alma_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->prs_e_bildirge_alma_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->prs_grup_tanimlama_yetkisi              = 1;
    E9_KULLANICI_YETKILERI->prs_isten_ayrilan_personel_arama_yetkisi= 1;
    E9_KULLANICI_YETKILERI->prs_eksik_gun_nedenleri_giris_yetkisi   = 1;
    E9_KULLANICI_YETKILERI->prs_ucret_pusulasi_raporu_alma_yetkisi  = 1;

/*************************************************** SUBE / DEPO / UNITE  ****************************************/

    E9_KULLANICI_YETKILERI->sub_sube_tanimlama_yetkisi              = 1;
    E9_KULLANICI_YETKILERI->sub_depo_tanimlama_yetkisi              = 1;
    E9_KULLANICI_YETKILERI->sub_depo_islem_yetkisi                  = TUM_DEPOLARDA;
    E9_KULLANICI_YETKILERI->sub_sube_islem_yetkisi                  = TUM_SUBELERDE;
    E9_KULLANICI_YETKILERI->sub_ent_hesaplari_guncelleme_yetkisi    = 0; // ent esaplarini degistiremez


/********************************************************* STOK *************************************************/

    E9_KULLANICI_YETKILERI->stok_urun_karti_tanimlama_yetkisi       = 1;
    E9_KULLANICI_YETKILERI->stok_hizmet_karti_tanimlama_yetkisi     = 1;
    E9_KULLANICI_YETKILERI->stok_grup_tanimlama_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->stok_fisi_islem_yetkisi                 = 1;
    E9_KULLANICI_YETKILERI->stok_urun_listesi_alma_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->stok_miktar_raporu_alma_yetkisi         = 1;
    E9_KULLANICI_YETKILERI->stok_ekstresi_alma_yetkisi              = 1;
    E9_KULLANICI_YETKILERI->stok_urun_hareketlerini_gorebilir_mi    = 1;


/*********************************************    FATURA          **************************************************/

    E9_KULLANICI_YETKILERI->fat_alis_faturasi_kesebilir_mi          = 1;
    E9_KULLANICI_YETKILERI->fat_sts_faturasi_kesebilir_mi           = 1;
    E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_elle             = 1;
    E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_oto              = 1;
    E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_toplu            = 1;
    E9_KULLANICI_YETKILERI->fat_satir_iskontosu_yapabilir_mi        = 1; // default'ta gosterilyor
    E9_KULLANICI_YETKILERI->fat_toplu_iskonto_yapabilir_mi          = 1;
    E9_KULLANICI_YETKILERI->fat_hizmet_ekstresi_alma_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->fat_ba_bs_listesi_alma_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->fat_fatura_listesi_alma_yetkisi         = 1;


/******************************************   İRSALİYE *************************************************************/

    E9_KULLANICI_YETKILERI->irs_alis_irsaliyesi_islem_yetkisi       = 1;
    E9_KULLANICI_YETKILERI->irs_sts_irsaliyesi_islem_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_elle           = 1;
    E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_oto            = 1;
    E9_KULLANICI_YETKILERI->irs_satir_iskontosu_yapabilir_mi        = 1;
    E9_KULLANICI_YETKILERI->irs_listesi_gorebilir_mi                = 1;
    E9_KULLANICI_YETKILERI->irs_toplu_iskonto_yapabilir_mi          = 1;


/******************************************************** ISLETME ****************************************************/

    E9_KULLANICI_YETKILERI->isl_isletme_defteri_rapor_alma_yetkisi  = 1;
    E9_KULLANICI_YETKILERI->isl_kdv_raporu_alma_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->isl_fisi_islem_yetkisi                  = 1;


/******************************************************** SM_MAKBUZU ****************************************************/

    E9_KULLANICI_YETKILERI->smm_makbuzu_islem_yetkisi               = 1;
    E9_KULLANICI_YETKILERI->smm_hizmet_tanimlama_yetkisi            = 1;
    E9_KULLANICI_YETKILERI->smm_tahsil_edildi_odendi_secili_gelsin  = 1;

/******************************************************** DOVIZ ****************************************************/

    E9_KULLANICI_YETKILERI->dvz_dovizleri_inceleme_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->dvz_doviz_tanimlama_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->dvz_kur_guncelleme_yetkisi              = 1;

/********************************************       PERAKENDE      ************************************************/

    E9_KULLANICI_YETKILERI->prk_fis_girebilir_mi                    = 1;
    E9_KULLANICI_YETKILERI->prk_gdr_tur_tanimlayabilir_mi           = 1;
    E9_KULLANICI_YETKILERI->prk_kasa_raporu_alabilir_mi             = 1;


/************************************************************* SISTEM **********************************************/

    E9_KULLANICI_YETKILERI->sis_blg_formuna_girebilir_mi            = 1;
    E9_KULLANICI_YETKILERI->sis_kul_formuna_girebilir_mi            = 1;
    E9_KULLANICI_YETKILERI->sis_veritabani_formuna_girebilir_mi     = 1;
    E9_KULLANICI_YETKILERI->sis_yonetim_log_inceleme_yetkisi        = 1;
    E9_KULLANICI_YETKILERI->sis_mail_srv_tanimlayabilirmi           = 1;
    E9_KULLANICI_YETKILERI->sis_sms_srv_tanimlayabilirmi            = 1;


    E9_KULLANICI_YETKILERI->ynt_e9_yetkilendirme_yapabilir_mi       = 1;
    E9_KULLANICI_YETKILERI->ynt_e9_onar_yetkisi                     = 1;
    E9_KULLANICI_YETKILERI->ynt_e9_devir_islemi_yapabilir_mi        = 1;
    E9_KULLANICI_YETKILERI->ynt_e9_log_inceleme_yetkisi             = 1;
    E9_KULLANICI_YETKILERI->ynt_e9_sabit_degerler_guncelleme_yetkisi= 1;
    E9_KULLANICI_YETKILERI->ynt_e9_belge_tasarlama_yetkisi          = 1;
    E9_KULLANICI_YETKILERI->ynt_e9_ent_bilgilerini_degistirme_yetkisi= 0;
    E9_KULLANICI_YETKILERI->ynt_e9_kullanicinin_calistigi_sirket     = 1;
    E9_KULLANICI_YETKILERI->ynt_e9_kullanici_sirket_degistire_bilir_mi= 0;
    E9_KULLANICI_YETKILERI->ynt_e9_urettigi_fisleri_grbilir_mi         = 1;

    E9_KULLANICI_YETKILERI->e9_adr_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_dmr_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_dvz_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi                 = 1;
    E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_irs_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_isl_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_muh_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_prk_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_prs_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_stk_kullanabilir_mi                  = 1;
    E9_KULLANICI_YETKILERI->e9_sub_kullanabilir_mi                  = 1;

    E9_KULLANICIYI_YETKILERI_UPDATE ( E9_KULLANICI_YETKILERI , KULLANICI_ID() );

    E9_LOG_KAYDI_EKLE(E9_YONETIM , LOG_E9_YETKILENDIRME , LOG_ISLEM_ADD , "Kullanıcı Yetkileri");

    DB->COMMIT_TRANSACTION();
}


/**************************************************************************************/
/*          E9_KULLANICI_YETKILERI_FISI_VE_KULLANICI_YETKILERI_ORDER_NUM_OLUSTUR      */
/**************************************************************************************/

void E9_KULLANICI_YETKILERI_FISI_VE_KULLANICI_YETKILERI_ORDER_NUM_OLUSTUR ()
{
    SQL_QUERY sql_query    ( DB );
    SQL_QUERY second_query ( DB );

    sql_query.PREPARE_SELECT("e9_kullanici_yetkileri_fisi", "kullanici_yetkileri_fisi_id");
    if ( sql_query.SELECT() > 0 ) {
        return;
    }

    sql_query.PREPARE_INSERT("e9_kullanici_yetkileri_fisi", "kullanici_yetkileri_fisi_id", "lock_flag");
    sql_query.SET_VALUE     (":lock_flag", 1 );
    sql_query.INSERT();

    int order_number = 0;
    sql_query.PREPARE_SELECT("e9_kullanici_yetkileri", "yetki_id");
    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    while ( sql_query.NEXT() EQ true ) {

        order_number += 1024;
        second_query.PREPARE_UPDATE("e9_kullanici_yetkileri ", "sabit_deger_id", "order_number ",
                                    "yetki_id = :yetki_id"  );

        second_query.SET_VALUE(":order_number", order_number            );
        second_query.SET_VALUE(":yetki_id",     sql_query.VALUE(0).toInt() );

        second_query.UPDATE();
    }
}

/**********************************************************************/
/*                  E9_ADD_DEFAULT_BELGELER();                           */
/**********************************************************************/

void E9_ADD_DEFAULT_BELGELER()
{

    G_YONETIM_DB->START_TRANSACTION();

    SQL_QUERY select_query(G_YONETIM_DB);
    SQL_QUERY insert_query(G_YONETIM_DB);

    struct BELGE_DEGISKENLERI_STRUCT{
        QString degisken_id;
        QString font_size;
        QString align;
        QString is_visible_variable;
        int     grup_enum;
        QString size_vertical;
        QString size_horizontal;
        QString pos_x;
        QString pos_y;
        QString text;
        int     belge_index;
        int     monospace_mi;
        int     degisken_uzunlugu;
        QString font_family;
        int     is_bold;
        int     is_under_line;
        int     is_italic;
        int     is_strikeout;
    };

    struct BELGELER_STRUCT {
        QString belge_id;
        QString row_count;
        QString row_space_satir;
        QString belge_satir_sayisi;
        QString belge_toplam_karakter_sayisi;
        QString kopya_sayisi;
        QString kopya_konumu;
        QString printer_type;
        QString line_headers_visible;
        int     degisken_sayisi;
        QString belge_width;
        QString belge_height;
        QString row_space_mm;
        QString tasarim_adi;
        QString html_str;
    };

/////// !!!!!!! TUM TASARIMLARI EKLER.
/////// !!!!!!! GEREKSIZ EKLENMEMESI ISTENILEN TASARIMLARIN DEGISKEN_SAYISI -2 YAPILARAK
/////// !!!!!!! TUM BELGELER BOS BIR DB YE EKLENMELI. SONRASINDA TEKRAR CTRL-7 ILE TASARIMCI CIKTISI ALINMALI.
/////// !!!!!!! TASARIM ADLARININ BASLARINA 'ADAK' EKLENMELI VE SON ALINAN CIKTI KULLANILMALI

    #include "e9_default_belge_degiskenleri.h"

    QStringList kayitli_tasarimlar;

    select_query.PREPARE_SELECT("ynt_belge_tasarimlari","tasarim_adi");
    if (select_query.SELECT() NE 0) {
        while (select_query.NEXT() EQ true) {
            kayitli_tasarimlar << select_query.VALUE(0).toString();
        }
    }

    for ( int i = 0 ; ; i++ ) {
        if (belgeler[i].degisken_sayisi EQ -2) {
            continue;
        }

        if (belgeler[i].degisken_sayisi EQ -1) {
            break;
        }

        if(kayitli_tasarimlar.contains(belgeler[i].tasarim_adi) EQ false OR kayitli_tasarimlar.size() EQ 0 ) {

            insert_query.PREPARE_INSERT("ynt_belge_tasarimlari" ,
                                        "tasarim_id"            ,
                                        "belge_id,tasarim_adi,"
                                        "html_str,belge_width,belge_height,row_count,row_space_mm,row_space_satir,"
                                        "belge_satir_sayisi,"
                                        "belge_toplam_karakter_sayisi,kopya_sayisi,kopya_konumu,printer_type,"
                                        "line_headers_visible,readonly"
                                        );

            insert_query.SET_VALUE(":belge_id"                      , belgeler[i].belge_id );
            insert_query.SET_VALUE(":tasarim_adi"                   , belgeler[i].tasarim_adi );
            insert_query.SET_VALUE(":html_str"                      , belgeler[i].html_str );
            insert_query.SET_VALUE(":belge_width"                   , belgeler[i].belge_width );
            insert_query.SET_VALUE(":belge_height"                  , belgeler[i].belge_height );
            insert_query.SET_VALUE(":row_count"                     , belgeler[i].row_count );
            insert_query.SET_VALUE(":row_space_mm"                  , belgeler[i].row_space_mm );
            insert_query.SET_VALUE(":row_space_satir"               , belgeler[i].row_space_satir );
            insert_query.SET_VALUE(":belge_satir_sayisi"            , belgeler[i].belge_satir_sayisi );
            insert_query.SET_VALUE(":belge_toplam_karakter_sayisi"  , belgeler[i].belge_toplam_karakter_sayisi );
            insert_query.SET_VALUE(":kopya_sayisi"                  , belgeler[i].kopya_sayisi );
            insert_query.SET_VALUE(":kopya_konumu"                  , belgeler[i].kopya_konumu );
            insert_query.SET_VALUE(":printer_type"                  , belgeler[i].printer_type );
            insert_query.SET_VALUE(":line_headers_visible"          , belgeler[i].line_headers_visible );
            insert_query.SET_VALUE(":readonly"                      , 1 );

            int tasarim_id =  insert_query.INSERT();

            int db_eklenen_degisken_sayisi = 0;

            for (int j = 0 ;  ; j++ ) {

                if ( db_eklenen_degisken_sayisi EQ belgeler[i].degisken_sayisi OR belgeler_degiskenleri[j].belge_index EQ -1) {
                    break;
                }

                if (belgeler_degiskenleri[j].belge_index NE i) {
                    continue;
                }

                insert_query.PREPARE_INSERT("ynt_belge_degiskenleri",
                                            "belge_degisken_id" ,
                                            "tasarim_id,degisken_id,font_size,align, pos_x,pos_y,size_vertical,"
                                            "size_horizontal,text,is_visible_variable, font_family, is_bold, is_under_line,"
                                            "is_italic, is_strikeout, grup_enum, monospace_mi, text_size" );

                insert_query.SET_VALUE(":tasarim_id"          , tasarim_id                                           );
                insert_query.SET_VALUE(":degisken_id"         , belgeler_degiskenleri[j].degisken_id.toInt()         );
                insert_query.SET_VALUE(":font_size"           , belgeler_degiskenleri[j].font_size.toInt()           );
                insert_query.SET_VALUE(":align"               , belgeler_degiskenleri[j].align.toInt()               );
                insert_query.SET_VALUE(":pos_x"               , belgeler_degiskenleri[j].pos_x.toDouble()            );
                insert_query.SET_VALUE(":pos_y"               , belgeler_degiskenleri[j].pos_y.toDouble()            );
                insert_query.SET_VALUE(":size_vertical"       , belgeler_degiskenleri[j].size_vertical.toInt()       );
                insert_query.SET_VALUE(":size_horizontal"     , belgeler_degiskenleri[j].size_horizontal.toInt()     );
                insert_query.SET_VALUE(":text"                , belgeler_degiskenleri[j].text                        );
                insert_query.SET_VALUE(":is_visible_variable" , belgeler_degiskenleri[j].is_visible_variable.toInt() );
                insert_query.SET_VALUE(":grup_enum"           , belgeler_degiskenleri[j].grup_enum                   );
                insert_query.SET_VALUE(":monospace_mi"        , belgeler_degiskenleri[j].monospace_mi                );
                insert_query.SET_VALUE(":text_size"           , belgeler_degiskenleri[j].degisken_uzunlugu           );
                insert_query.SET_VALUE(":font_family"         , belgeler_degiskenleri[j].font_family                 );
                insert_query.SET_VALUE(":is_bold"             , belgeler_degiskenleri[j].is_bold                     );
                insert_query.SET_VALUE(":is_under_line"       , belgeler_degiskenleri[j].is_under_line               );
                insert_query.SET_VALUE(":is_italic"           , belgeler_degiskenleri[j].is_italic                   );
                insert_query.SET_VALUE(":is_strikeout"        , belgeler_degiskenleri[j].is_strikeout                );

                insert_query.INSERT();

                db_eklenen_degisken_sayisi++;
            }
        }
    }

    G_YONETIM_DB->COMMIT_TRANSACTION();
}

/**************************************************************************************************************
                                        E9_DELIMETER_UPDATE
***************************************************************************************************************/

void E9_DELIMETER_UPDATE ( const QString p_old_delimeter, const QString p_new_delimeter, QWidget * p_parent )
{

    SQL_QUERY sql_query    ( DB );
    SQL_QUERY update_query ( DB );

    sql_query.PREPARE_SELECT ( "muh_hesaplar",
                               "tam_hesap_kodu, hesap_id ",
                               "hesap_seviyesi != 1" );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    int guncellenen_kayit_sayisi = 0;

    while ( sql_query.NEXT() EQ true ) {

        QString tam_hesap_kodu = sql_query.VALUE(0).toString();

        guncellenen_kayit_sayisi++;

        tam_hesap_kodu.replace ( p_old_delimeter, p_new_delimeter, Qt::CaseSensitive );


        update_query.PREPARE_UPDATE ( "muh_hesaplar ",
                                      "hesap_id",
                                      "tam_hesap_kodu",
                                      "hesap_id     = :hesap_id");

        update_query.SET_VALUE ( ":tam_hesap_kodu", tam_hesap_kodu             );
        update_query.SET_VALUE ( ":hesap_id"      , sql_query.VALUE(1).toInt() );
        update_query.UPDATE();
    }

   ADAK_INFO( QObject::tr("%n adet kayıt güncellendi", "", guncellenen_kayit_sayisi ), NULL, p_parent );

}

/**************************************************************************************************************
                                        E9_SET_SABIT_DEGERLER_STRUCT
***************************************************************************************************************/

void E9_SET_SABIT_DEGERLER_STRUCT ()
{
    SQL_QUERY sql_query     ( DB );

    sql_query.PREPARE_SELECT("e9_sabit_degerler","fat_acik_fatura_mi_kapali_mi ","");

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    G_E9_SABIT_DEGERLER.acik_kapali_fatura        = sql_query.VALUE ( 0 ).toInt();

    E9_RESTORE_MUHASEBE_ALAMLARI_VALUE();
}

/**************************************************************************************************************
                                        E9_GET_ACIK_KAPALI_FATURA_SABIT_DEGER
***************************************************************************************************************/

int E9_GET_ACIK_KAPALI_FATURA_SABIT_DEGER ()
{
    return G_E9_SABIT_DEGERLER.acik_kapali_fatura;
}

/**************************************************************************************************************
                                        E9_SET_ACIK_KAPALI_FATURA_SABIT_DEGER
***************************************************************************************************************/

void E9_SET_ACIK_KAPALI_FATURA_SABIT_DEGER ( int p_acik_kapali_fatura )
{
    G_E9_SABIT_DEGERLER.acik_kapali_fatura = p_acik_kapali_fatura;
}

/**************************************************************************************************************
                                        E9_CREATE_SABIT_DEGERLER
***************************************************************************************************************/

void E9_CREATE_SABIT_DEGERLER ()
{
    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT("e9_sabit_degerler","sabit_deger_id");

    if ( sql_query.SELECT() > 0 ) {
        return;
    }

    QString e9_ayraci = " ";

    QString mali_yil_ilk_tarih = QVariant ( QDate::currentDate().year() ).toString() + ".01.01";
    QString mali_yil_son_tarih = QVariant ( QDate::currentDate().year() ).toString() + ".12.31";
    QString e9_array_son_tarih = QDate    ( QDate::currentDate().year(), 01, 01 ).addDays ( E9_ARRAY_SIZE -2 ).toString("yyyy.MM.dd");

    int     ciro_sonrasi_gun_sayisi     = 15;
    double  max_amortisman_orani        = 50;
    int     irs_faturalandirilacak_gun  = 7;


    double  * KDV_ORANLARI = new double [KDV_OTV_ORANLARI_SAYISI];
    double  * OTV_ORANLARI = new double [KDV_OTV_ORANLARI_SAYISI];



    KDV_ORANLARI[0] =  0.00;
    KDV_ORANLARI[1] =  1.00;
    KDV_ORANLARI[2] =  8.00;
    KDV_ORANLARI[3] =  9.00;
    KDV_ORANLARI[4] = 12.00;
    KDV_ORANLARI[5] = 18.00;

    OTV_ORANLARI[0] =  0.00;
    OTV_ORANLARI[1] =  1.00;
    OTV_ORANLARI[2] =  4.00;
    OTV_ORANLARI[3] =  6.70;
    OTV_ORANLARI[4] =  9.00;
    OTV_ORANLARI[5] =  20.00;
    OTV_ORANLARI[6] =  25.00;
    OTV_ORANLARI[7] =  30.00;
    OTV_ORANLARI[8] =  63.00;

    int kdv_orani_sayisi = 6;
    int otv_orani_sayisi = 9;

    int ind_kdv_hesap_id        = MUH_GET_HESAP_ID ( "191" );
    int ind_kdv_iade_hesap_id   = MUH_GET_HESAP_ID ( "192" );

    int hes_kdv_hesap_id        = MUH_GET_HESAP_ID ( "391" );
    int hes_kdv_iade_hesap_id   = MUH_GET_HESAP_ID ( "392" );

    int otv_alis_hesap_id       = MUH_GET_HESAP_ID ( "194" );
    int otv_satis_hesap_id      = MUH_GET_HESAP_ID ( "394" );
    int tevkifat_kdv_hesap_id   = MUH_GET_HESAP_ID ( "360" );


    int IND_KDV_HESAPLARI_ID_ARRAY      [ KDV_OTV_ORANLARI_SAYISI ];
    int IND_KDV_HESAPLARI_IADE_ID_ARRAY [ KDV_OTV_ORANLARI_SAYISI ];
    int HES_KDV_HESAPLARI_ID_ARRAY      [ KDV_OTV_ORANLARI_SAYISI ];
    int HES_KDV_HESAPLARI_IADE_ID_ARRAY [ KDV_OTV_ORANLARI_SAYISI ];

    int OTV_ALIS_HESAPLARI_ID_ARRAY     [ KDV_OTV_ORANLARI_SAYISI ];
    int OTV_SATIS_HESAPLARI_ID_ARRAY    [ KDV_OTV_ORANLARI_SAYISI ];

    int TEVKIFAT_KDV_HESAPLARI_ID_ARRAY [ KDV_OTV_ORANLARI_SAYISI ];

    IND_KDV_HESAPLARI_ID_ARRAY      [0] = 0;
    IND_KDV_HESAPLARI_IADE_ID_ARRAY [0] = 0;
    HES_KDV_HESAPLARI_ID_ARRAY      [0] = 0;
    HES_KDV_HESAPLARI_IADE_ID_ARRAY [0] = 0;
    OTV_ALIS_HESAPLARI_ID_ARRAY     [0] = 0;
    OTV_SATIS_HESAPLARI_ID_ARRAY    [0] = 0;
    TEVKIFAT_KDV_HESAPLARI_ID_ARRAY [0] = 0;

    for ( int i = 1; i < kdv_orani_sayisi; i++ ) {
        IND_KDV_HESAPLARI_ID_ARRAY      [i] = ind_kdv_hesap_id;
        IND_KDV_HESAPLARI_IADE_ID_ARRAY [i] = ind_kdv_iade_hesap_id;
        HES_KDV_HESAPLARI_ID_ARRAY      [i] = hes_kdv_hesap_id;
        HES_KDV_HESAPLARI_IADE_ID_ARRAY [i] = hes_kdv_iade_hesap_id;
        TEVKIFAT_KDV_HESAPLARI_ID_ARRAY [i] = tevkifat_kdv_hesap_id;
    }

    for ( int i = 1; i < otv_orani_sayisi; i++ ) {
        OTV_ALIS_HESAPLARI_ID_ARRAY     [i] = otv_alis_hesap_id;
        OTV_SATIS_HESAPLARI_ID_ARRAY    [i] = otv_satis_hesap_id;
    }

    sql_query.PREPARE_INSERT ( "e9_sabit_degerler", "sabit_deger_id",
                               "  cek_ciro_sonrasi_gun_sayisi    , cek_portfoy_numarasi_sayaci      , "
                               "  dmr_amortisman_ayirma_yontemi  , dmr_amor_yntemi_degisebilir_mi   , "
                               "  dmr_max_amortisman_orani       , fat_acik_fatura_mi_kapali_mi     , "
                               "  irs_faturalandirilacak_gun     , mali_yil_ilk_tarih               , "
                               "  mali_yil_son_tarih             , e9_array_son_tarih               , "
                               "  e9_ayraci                      , kdv_orani_sayisi                 , "
                               "  otv_orani_sayisi               , kdv_oranlari_array               , "
                               "  otv_oranlari_array             , firma_ismi                       , "
                               "  ontanimli_kdv_orani            , program_version                  , "
                               "  ind_kdv_hesap_id_array         , "
                               "  ind_kdv_iade_hesap_id_array    , hes_kdv_hesap_id_array           , "
                               "  hes_kdv_iade_hesap_id_array    , otv_alis_hesap_id_array          , "
                               "  otv_satis_hesap_id_array       , tevkifat_kdv_hesap_id_array      , "
                               "  max_nakit_giris_cikis_tutari   , "
                               "  isl_fisleri_guncellendi        , "
                               "  firma_logosu                   , "
                               "  muhasebe_alarmlari_kapalimi    , "
                               "  cari_hesap_kodu_counter        , stok_birim_fiyat_hassasiyeti     ,"
                               "  stok_birim_miktar_hassasiyeti    ");



    sql_query.SET_VALUE ( ":cek_ciro_sonrasi_gun_sayisi"     , ciro_sonrasi_gun_sayisi    );
    sql_query.SET_VALUE ( ":cek_portfoy_numarasi_sayaci"     , 0);
    sql_query.SET_VALUE ( ":dmr_amortisman_ayirma_yontemi"   , NORMAL_BAKIYE);
    sql_query.SET_VALUE ( ":dmr_amor_yntemi_degisebilir_mi"  , 1);
    sql_query.SET_VALUE ( ":dmr_max_amortisman_orani"        , max_amortisman_orani);
    sql_query.SET_VALUE ( ":fat_acik_fatura_mi_kapali_mi"    , ENUM_ACIK_FATURA);
    sql_query.SET_VALUE ( ":irs_faturalandirilacak_gun"      , irs_faturalandirilacak_gun);
    sql_query.SET_VALUE ( ":mali_yil_ilk_tarih"              , mali_yil_ilk_tarih);
    sql_query.SET_VALUE ( ":mali_yil_son_tarih"              , mali_yil_son_tarih);
    sql_query.SET_VALUE ( ":e9_array_son_tarih"              , e9_array_son_tarih);
    sql_query.SET_VALUE ( ":e9_ayraci"                       , e9_ayraci);
    sql_query.SET_VALUE ( ":kdv_orani_sayisi"                , kdv_orani_sayisi);
    sql_query.SET_VALUE ( ":otv_orani_sayisi"                , otv_orani_sayisi);
    sql_query.SET_VALUE ( ":kdv_oranlari_array"              , QVariant(PACK_DOUBLE_ARRAY ( KDV_ORANLARI, KDV_OTV_ORANLARI_SAYISI )).toByteArray());
    sql_query.SET_VALUE ( ":otv_oranlari_array"              , QVariant(PACK_DOUBLE_ARRAY ( OTV_ORANLARI, KDV_OTV_ORANLARI_SAYISI )).toByteArray());
    sql_query.SET_VALUE ( ":firma_ismi"                      , VERITABANI_TANIMI());
    sql_query.SET_VALUE ( ":ontanimli_kdv_orani"             , 18);
    sql_query.SET_VALUE ( ":program_version"                 , ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID()));
    sql_query.SET_VALUE ( ":ind_kdv_hesap_id_array",           QVariant(PACK_INT_ARRAY ( IND_KDV_HESAPLARI_ID_ARRAY,      KDV_OTV_ORANLARI_SAYISI )).toByteArray() );
    sql_query.SET_VALUE ( ":ind_kdv_iade_hesap_id_array",      QVariant(PACK_INT_ARRAY ( IND_KDV_HESAPLARI_IADE_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI )).toByteArray() );
    sql_query.SET_VALUE ( ":hes_kdv_hesap_id_array",           QVariant(PACK_INT_ARRAY ( HES_KDV_HESAPLARI_ID_ARRAY,      KDV_OTV_ORANLARI_SAYISI )).toByteArray() );
    sql_query.SET_VALUE ( ":hes_kdv_iade_hesap_id_array",      QVariant(PACK_INT_ARRAY ( HES_KDV_HESAPLARI_IADE_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI )).toByteArray() );
    sql_query.SET_VALUE ( ":otv_alis_hesap_id_array",          QVariant(PACK_INT_ARRAY ( OTV_ALIS_HESAPLARI_ID_ARRAY,     KDV_OTV_ORANLARI_SAYISI )).toByteArray() );
    sql_query.SET_VALUE ( ":otv_satis_hesap_id_array",         QVariant(PACK_INT_ARRAY ( OTV_SATIS_HESAPLARI_ID_ARRAY,    KDV_OTV_ORANLARI_SAYISI )).toByteArray() );
    sql_query.SET_VALUE ( ":tevkifat_kdv_hesap_id_array",      QVariant(PACK_INT_ARRAY ( TEVKIFAT_KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI )).toByteArray() );
    sql_query.SET_VALUE ( ":max_nakit_giris_cikis_tutari"    , 8000.00                );
    sql_query.SET_VALUE ( ":isl_fisleri_guncellendi"         , 1                      );
    sql_query.SET_VALUE ( ":firma_logosu"                    , QVariant(" ").toByteArray());
    sql_query.SET_VALUE ( ":muhasebe_alarmlari_kapalimi"     , 1                      );
    sql_query.SET_VALUE ( ":cari_hesap_kodu_counter"         , 0                      );
    sql_query.SET_VALUE ( ":stok_birim_fiyat_hassasiyeti"    , 2                      );
    sql_query.SET_VALUE ( ":stok_birim_miktar_hassasiyeti"   , 2                      );

    sql_query.INSERT();

    delete [] KDV_ORANLARI;
    delete [] OTV_ORANLARI;

    E9_LOG_KAYDI_EKLE(E9_YONETIM , LOG_E9_SABIT_DEGERLER , LOG_ISLEM_ADD , "E9 Sabit Değerler");
}
/********************************************************************************************************
                                       E9_GET_KDV_ORANLARI_OKUNDUMU_FLAG
*********************************************************************************************************/

bool E9_GET_KDV_ORANLARI_OKUNDUMU_FLAG()
{
    return g_kdv_oranlari_veritabanindan_okundu_mu;
}

/********************************************************************************************************
                                       E9_GET_OTV_ORANLARI_OKUNDUMU_FLAG
*********************************************************************************************************/

bool E9_GET_OTV_ORANLARI_OKUNDUMU_FLAG()
{
    return g_otv_oranlari_veritabanindan_okundu_mu;
}


/********************************************************************************************************
                                       E9_SET_KDV_ORANLARI_OKUNDUMU_FLAG
*********************************************************************************************************/

void E9_SET_KDV_ORANLARI_OKUNDUMU_FLAG ( bool p_flag_value )
{
    g_kdv_oranlari_veritabanindan_okundu_mu = p_flag_value;
}

/********************************************************************************************************
                                            E9_SET_OTV_ORANLARI_OKUNDUMU_FLAG
*********************************************************************************************************/

void E9_SET_OTV_ORANLARI_OKUNDUMU_FLAG ( bool p_flag_value )
{
    g_otv_oranlari_veritabanindan_okundu_mu = p_flag_value;
}



/********************************************************************************************************
                                    E9_GET_ADAK_MODULES_ENUM
*********************************************************************************************************/

int E9_GET_ADAK_MODULES_ENUM ( QString p_adak_module_name )
{
    if ( p_adak_module_name EQ QObject::tr ("Muhasebe") ) {
        return MUHASEBE_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("İşletme") ) {
        return ISLETME_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Döviz") ) {
        return DOVIZ_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Adres") ) {
        return ADRES_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Cari") ) {
        return CARI_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("ÇekSenet") ) {
        return CEKSENET_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Banka") ) {
        return BANKA_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Şube / Ünite / Depo") ) {
        return SUBE_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Fatura") ) {
        return FATURA_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("İrsaliye") ) {
        return IRSALIYE_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Stok") ) {
        return STOK_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Demirbaş") ) {
        return DEMIRBAS_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Personel") ) {
        return PERSONEL_MODULU;
    }
    else if ( p_adak_module_name EQ QObject::tr ("Faiz") ) {
        return FAIZ_MODULU;
    }
    else if (p_adak_module_name EQ QObject::tr("E9 Yönetim")) {
        return E9_YONETIM;
    }
    return -1;
}

/********************************************************************************************************
                                   E9_GET_ADAK_MODULES_STRING
*********************************************************************************************************/

QString E9_GET_ADAK_MODULES_STRING ( int p_adak_module_enum )
{
    switch ( p_adak_module_enum ) {
        case MUHASEBE_MODULU :
            return QObject::tr ("Muhasebe");
        case ISLETME_MODULU :
            return QObject::tr ("İşletme");
        case DOVIZ_MODULU :
            return QObject::tr ("Döviz");
        case ADRES_MODULU :
            return QObject::tr ("Adres");
        case CARI_MODULU :
            return QObject::tr ("Cari");
        case CEKSENET_MODULU :
            return QObject::tr ("ÇekSenet");
        case BANKA_MODULU :
            return QObject::tr ("Banka");
        case SUBE_MODULU :
            return QObject::tr ("Şube / Ünite / Depo");
        case  FATURA_MODULU :
            return QObject::tr ("Fatura");
        case IRSALIYE_MODULU :
            return QObject::tr ("İrsaliye");
        case STOK_MODULU:
            return QObject::tr ("Stok");
        case DEMIRBAS_MODULU:
            return QObject::tr ("Demirbaş");
        case PERSONEL_MODULU:
            return QObject::tr ("Personel");
        case FAIZ_MODULU:
            return QObject::tr ("Faiz");
        case E9_YONETIM  :
            return QObject::tr("E9 Yönetim");
        default :
            return "";
    }
}

/********************************************************************************************************
                                    E9_GET_LOG_ISLEMLERI_STRING
*********************************************************************************************************/

QString E9_GET_LOG_ISLEMLERI_STRING ( int p_e9_log_islemi_enum )
{
    switch ( p_e9_log_islemi_enum ) {
        case LOG_ISLEM_ADD :
            return QObject::tr("Kayıt eklendi");
        case LOG_ISLEM_DELETE :
            return QObject::tr("Kayıt silindi");
        case LOG_ISLEM_UPDATE :
            return QObject::tr("Kayıt güncellendi");
        case LOG_ISLEM_RAPOR_ALINDI :
            return QObject::tr("Rapor Alındı");
        case LOG_ISLEM_KDV_AYIRMA :
            return QObject::tr("KDV Ayırma");

        default :
            return "";
    }
}

/********************************************************************************************************
                                    E9_LOG_KAYDI_EKLE()
*********************************************************************************************************/

void E9_LOG_KAYDI_EKLE (int p_modul_id,int p_log_turu, int p_log_islemi, QString p_log_detaylari)
{
    SQL_QUERY sql_query(DB);

    QString islem_tarihi = QDate::currentDate().toString("yyyy.MM.dd");
    QString islem_zamani = QTime::currentTime().toString("hh:mm");

    if ( p_log_detaylari.size() > 1024 ) {
        p_log_detaylari.resize(1024);
    }

    sql_query.PREPARE_INSERT("e9_log_bilgileri",
                             "log_id" ,
                             "islem_tarihi, islem_zamani, log_turu, "
                             "bilgisayar_id, kullanici_id, yapilan_islem, log_detaylari, modul_id " );

    sql_query.SET_VALUE(":islem_tarihi" , islem_tarihi       );
    sql_query.SET_VALUE(":islem_zamani" , islem_zamani       );
    sql_query.SET_VALUE(":log_turu"     , p_log_turu         );
    sql_query.SET_VALUE(":bilgisayar_id", BILGISAYAR_ID()    );
    sql_query.SET_VALUE(":kullanici_id" , KULLANICI_ID()     );
    sql_query.SET_VALUE(":yapilan_islem", p_log_islemi       );
    sql_query.SET_VALUE(":log_detaylari", p_log_detaylari    );
    sql_query.SET_VALUE(":modul_id"     , p_modul_id         );

    sql_query.INSERT();

}

/*******************************************************************************************************
                                  E9_GET_LOG_TURLERI_STRING
********************************************************************************************************/

QString E9_GET_LOG_TURLERI_STRING ( int p_log_turu_enum )
{
    switch ( p_log_turu_enum ) {
        case LOG_MUH_FISLER :
            return QObject::tr("Muhasebe Fişleri");

        case LOG_MUH_HESAP_PLANI :
            return QObject::tr("Hesap Planı");

        case LOG_MUH_HESAP_GRUPLARI :
            return QObject::tr("Hesap Grupları");

        case LOG_MUH_ENT_HESAPLARI :
            return QObject::tr("Entegrasyon Ana Hesapları");

        case LOG_MUH_FIS_KOPYALAMA :
            return QObject::tr("Fiş Kopyalama");

        case LOG_MUH_YVM_NO_OLUSTUR :
            return QObject::tr("Yevmiye No Oluştur");

        case LOG_ADR_SAHIS_KAYDI :
            return QObject::tr("Şahıs Kaydı");

        case LOG_ADR_FIRMA_KAYDI :
            return QObject::tr("Firma Kaydı");

        case LOG_ADR_ADRES_GRUP_EKLEME :
            return QObject::tr("Adres Grup Ekleme");

        case LOG_ADR_ADRES_GRUPLARI :
            return QObject::tr("Adres Grupları");

        case LOG_ADR_FIRMA_YETKILI_KAYDI :
            return QObject::tr("Firma Yetkili Kaydı");

        case LOG_ADR_YAKIN_BILGILERI_KAYDI :
            return QObject::tr("Yakın Bilgileri Kaydı");

        case LOG_ADR_KULLANICI_YETKILERI :
            return QObject::tr("Kullanıcı Yetkileri");

        case LOG_ADR_ETIKET_YAZDIRMA :
            return QObject::tr("Etiket Yazdırma");

        case LOG_ADR_MAIL_GONDERIMI :
            return QObject::tr("Mail Gönderimi");

        case LOG_CARI_FISLER:
            return QObject::tr("Cari Fişler");

        case LOG_CARI_HESAPLAR:
            return QObject::tr("Cari Hesaplar");

        case LOG_CARI_GRUPLAR:
            return QObject::tr("Cari Hesap Grupları");

        case LOG_CEK_BORDROSU:
            return QObject::tr("Çek Senet Bordrosu");

        case LOG_CEK_SENETLER:
            return QObject::tr("Kendi Çek Senetlerimiz");

        case LOG_BNK_FISLER :
            return QObject::tr("Banka Fişleri");

        case LOG_BNK_HESAPLAR:
            return QObject::tr("Banka Heapları");

        case LOG_BNK_ISLEMLER:
            return QObject::tr("Banka İşlemleri");

        case LOG_BNK_KREDI_KARTLARI:
            return QObject::tr("Kredi Kartları");

        case LOG_BNK_POS_ISLEMLERI:
            return QObject::tr("Pos İşlemleri");

        case LOG_ISL_FISLER :
            return QObject::tr("İşletme Fişleri");

        case LOG_ISL_FIS_KOPYALAMA:
            return QObject::tr("İşletme Fiş Kopyalama");

        case LOG_DMR_AMORTISMAN_AYIRMA :
            return QObject::tr("Amortisman Ayırma");

        case LOG_DMR_DEMIRBASLAR :
            return QObject::tr("Demirbaşlar");

        case LOG_DMR_FISLER:
            return QObject::tr("Demirbaş Modülü Fişleri");

        case LOG_DMR_YENIDEN_DEGERLEME:
            return QObject::tr("Yeniden Değerleme");

        case LOG_FAT_FATURALAR:
            return QObject::tr("Faturalar");

        case LOG_FAT_HIZMETLER:
            return QObject::tr("Hizmetler");

        case LOG_FAT_TOPLU_FATURALASTIRMA:
            return QObject::tr("Toplu Faturalaştırma");

        case LOG_STK_FISLER:
            return QObject::tr("Stok Fişleri");

        case LOG_STK_URUNLER:
            return QObject::tr("Ürünler");

        case LOG_IRS_FISLER:
            return QObject::tr("İrsaliyeler");

        case LOG_STK_URUN_BIRIMLERI :
            return QObject::tr("Ürün Birimleri");

        case LOG_SUBE_SUBELER:
            return QObject::tr("Şubeler");

        case LOG_SUBE_DEPOLAR:
            return QObject::tr("Depolar");

        case LOG_SUBE_UNITELER:
            return QObject::tr("Üniteler");

        case LOG_SUBE_ENT_HESAPLARI:
            return QObject::tr("Şube Entegrasyon Hesapları");

        case LOG_E9_ONAR:
            return QObject::tr("E9 Onar");

        case LOG_E9_DEVIR:
            return QObject::tr("Devir İşlemi");

        case LOG_E9_KDV_OTV_ORANLARI:
            return QObject::tr("Kdv Ötv Tanımları");

        case LOG_E9_SABIT_DEGERLER:
            return QObject::tr("Sabit Değerler");

        case LOG_E9_YETKILENDIRME:
            return QObject::tr("Yetkilendirme");

        case LOG_PRS_PERSONEL :
            return QObject::tr("Personeller");

        case LOG_PRS_BORDROLAR :
            return QObject::tr("Personel Bordroları");

        case LOG_PRS_IBRANAME_MADDELERI:
            return QObject::tr("İbraname Maddeleri");

        case LOG_PRS_SABIT_DEGERLER:
            return QObject::tr("Personel Sabit Değerler");
        default :
            return "";
    }
}

/*******************************************************************************************************
                                  E9_GET_LOG_TURLERI_ENUM
********************************************************************************************************/

int E9_GET_LOG_TURLERI_ENUM ( QString p_log_turu )
{
    if (p_log_turu EQ QObject::tr("Muhasebe Fişleri")) {
        return LOG_MUH_FISLER;
    }
    else if (p_log_turu EQ QObject::tr("Hesap Planı")) {
        return LOG_MUH_HESAP_PLANI;
    }
    else if (p_log_turu EQ QObject::tr("Hesap Grupları")) {
        return LOG_MUH_HESAP_GRUPLARI;
    }
    else if (p_log_turu EQ QObject::tr("Entegrasyon Ana Hesapları")) {
        return LOG_MUH_ENT_HESAPLARI;
    }
    else if (p_log_turu EQ QObject::tr("Fiş Kopyalama")) {
        return LOG_MUH_FIS_KOPYALAMA;
    }
    else if (p_log_turu EQ QObject::tr("Yevmiye No Oluştur")) {
        return LOG_MUH_YVM_NO_OLUSTUR;
    }
    else if (p_log_turu EQ QObject::tr("Muhasebe Raporları")) {
        return LOG_MUH_RAPORLAR;
    }
    if ( p_log_turu EQ QObject::tr("Şahıs Kaydı") ) {
        return LOG_ADR_SAHIS_KAYDI;
    }
    else if ( p_log_turu EQ QObject::tr("Firma Kaydı") ) {
        return LOG_ADR_FIRMA_KAYDI;
    }
    else if ( p_log_turu EQ QObject::tr("Adres Grup Ekleme") ) {
        return LOG_ADR_ADRES_GRUP_EKLEME;
    }
    else if ( p_log_turu EQ QObject::tr("Adres Grupları") ) {
        return LOG_ADR_ADRES_GRUPLARI;
    }
    else if ( p_log_turu EQ QObject::tr("Firma Yetkili Kaydı") ) {
        return LOG_ADR_FIRMA_YETKILI_KAYDI;
    }
    else if ( p_log_turu EQ QObject::tr("Yakın Bilgileri Kaydı") ) {
        return LOG_ADR_YAKIN_BILGILERI_KAYDI;
    }
    else if ( p_log_turu EQ QObject::tr("Kullanıcı Yetkileri") ) {
        return LOG_ADR_KULLANICI_YETKILERI;
    }
    else if ( p_log_turu EQ QObject::tr("Etiket Yazdırma") ) {
        return LOG_ADR_ETIKET_YAZDIRMA;
    }
    else if ( p_log_turu EQ QObject::tr("Mail Gönderimi") ) {
        return LOG_ADR_MAIL_GONDERIMI;
    }
    else if ( p_log_turu EQ QObject::tr("Cari Fişler")) {
        return LOG_CARI_FISLER;
    }
    else if ( p_log_turu EQ QObject::tr("Cari Hesaplar")) {
        return LOG_CARI_HESAPLAR;
    }
    else if ( p_log_turu EQ QObject::tr("Cari Hesap Grupları")) {
        return LOG_CARI_GRUPLAR;
    }
    else if (p_log_turu EQ QObject::tr("Cari Raporları")) {
        return LOG_CARI_RAPORLAR;
    }
    else if ( p_log_turu EQ QObject::tr("Banka Fişleri")) {
        return LOG_BNK_FISLER;
    }
    else if ( p_log_turu EQ QObject::tr("Banka Hesapları")) {
        return LOG_BNK_HESAPLAR;
    }
    else if ( p_log_turu EQ QObject::tr("Banka İşlemleri")) {
        return LOG_BNK_ISLEMLER;
    }

    else if ( p_log_turu EQ QObject::tr("Kredi Kartları")) {
        return LOG_BNK_KREDI_KARTLARI;
    }
    else if (p_log_turu EQ QObject::tr("Pos İşlemleri")) {
        return LOG_BNK_POS_ISLEMLERI;
    }
    else if (p_log_turu EQ QObject::tr("Banka Raporları")) {
        return LOG_BNK_RAPORLAR;
    }
    else if ( p_log_turu EQ QObject::tr("Çek-Senet Bordrosu")) {
        return LOG_CEK_BORDROSU;
    }
    else if ( p_log_turu EQ QObject::tr("Kendi Çek/Senetlerimiz")) {
        return LOG_CEK_SENETLER;
    }
    else if ( p_log_turu EQ QObject::tr("Çek Raporları")) {
        return LOG_CEK_RAPORLAR;
    }
    else if (p_log_turu EQ QObject::tr("İşletme Fişleri")) {
        return LOG_ISL_FISLER;
    }
    else if (p_log_turu EQ QObject::tr("İşletme Fiş Kopyalama")) {
        return LOG_ISL_FIS_KOPYALAMA;
    }
    else if (p_log_turu EQ QObject::tr("İşletme Raporları")) {
        return LOG_ISL_RAPORLAR;
    }
    else if (p_log_turu EQ QObject::tr("Amortisman Ayırma")) {
        return LOG_DMR_AMORTISMAN_AYIRMA;
    }
    else if ( p_log_turu EQ QObject::tr("Demirbaş Modülü Fişleri")) {
        return LOG_DMR_FISLER;
    }
    else if ( p_log_turu EQ QObject::tr("Demirbaşlar")) {
        return LOG_DMR_DEMIRBASLAR;
    }
    else if ( p_log_turu EQ QObject::tr("Yeniden Değerleme")) {
        return LOG_DMR_YENIDEN_DEGERLEME;
    }
    else if (p_log_turu EQ QObject::tr("Demirbaş Raporları")) {
        return LOG_DMR_RAPORLAR;
    }
    else if ( p_log_turu EQ QObject::tr("Faturalar")) {
        return LOG_FAT_FATURALAR;
    }
    else if ( p_log_turu EQ QObject::tr("Hizmetler")) {
        return LOG_FAT_HIZMETLER;
    }
    else if ( p_log_turu EQ QObject::tr("Toplu Faturalaştırma")) {
        return LOG_FAT_TOPLU_FATURALASTIRMA;
    }
    else if ( p_log_turu EQ QObject::tr("Stok Fişleri")) {
        return LOG_STK_FISLER;
    }
    else if ( p_log_turu EQ QObject::tr("Ürünler")) {
        return LOG_STK_URUNLER;
    }
    else if ( p_log_turu EQ QObject::tr("İrsaliyeler")) {
        return LOG_IRS_FISLER;
    }
    else if ( p_log_turu EQ QObject::tr("Ürün Birimleri")) {
        return LOG_STK_URUN_BIRIMLERI;
    }
    else if ( p_log_turu EQ QObject::tr("Şubeler")) {
        return LOG_SUBE_SUBELER ;
    }
    else if ( p_log_turu EQ QObject::tr("Depolar")) {
        return LOG_SUBE_DEPOLAR;
    }
    else if ( p_log_turu EQ QObject::tr("Üniteler")) {
        return LOG_SUBE_UNITELER;
    }
    else if ( p_log_turu EQ QObject::tr("Şube Entegrasyon Hesapları")) {
        return LOG_SUBE_ENT_HESAPLARI;
    }
    else if ( p_log_turu EQ QObject::tr("E9 Onar")) {
        return LOG_E9_ONAR;
    }
    else if ( p_log_turu EQ QObject::tr("Devir İşlemi")) {
        return LOG_E9_DEVIR;
    }
    else if ( p_log_turu EQ QObject::tr("Sabit Değerler")) {
        return LOG_E9_SABIT_DEGERLER;
    }
    else if ( p_log_turu EQ QObject::tr("Kdv Ötv Tanımları")) {
        return LOG_E9_KDV_OTV_ORANLARI;
    }
    else if ( p_log_turu EQ QObject::tr("Yetkilendirme")) {
        return LOG_E9_YETKILENDIRME;
    }
    else if (p_log_turu EQ QObject::tr("Personeller")) {
        return LOG_PRS_PERSONEL;
    }
    else if (p_log_turu EQ QObject::tr("Personel Bordroları")) {
        return LOG_PRS_BORDROLAR;
    }
    else if (p_log_turu EQ QObject::tr("Personel Sabit Değerler")) {
        return LOG_PRS_SABIT_DEGERLER;
    }
    else if (p_log_turu EQ QObject::tr("İbraname Maddeleri")) {
        return LOG_PRS_IBRANAME_MADDELERI;
    }
    else if (p_log_turu EQ QObject::tr("Personel Raporları")) {
        return LOG_PRS_RAPORLAR;
    }

    return -1;

}

/**************************************************************************************
                   E9_GET_DELIMETER
***************************************************************************************/

QString E9_GET_DELIMETER()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "e9_sabit_degerler","e9_ayraci" );
    if ( select_query.SELECT() EQ 0 ) {
        return "";
    }
    select_query.NEXT();
    return select_query.VALUE(0).toString();
}

/********************************************************/
/*                      E9_GET_FIS_NO                      */
/********************************************************/

int E9_GET_FIS_NO ( int p_modul_id , int fis_id ) //Fislerin entegre edildikleri moduldeki fis_idlerine gore fis_no dondurur
{
    QString where_query;
    QString table_name;
    QString column_name;
    QString set_value_str;

    column_name     = "fis_no";
    where_query     = "fis_id = :fis_id";
    set_value_str   = ":fis_id";

    switch (p_modul_id) {
        case CARI_MODULU     :
            table_name      = "car_fisler";
            break;
        case BANKA_MODULU    :
            table_name      = "bnk_hareket_fisler";
            break;
        case CEKSENET_MODULU :
            table_name      = "cek_bordrolar";
            column_name     = "bordro_no";
            where_query     = "bordro_id = :bordro_id";
            set_value_str   = ":bordro_id";
            break;
        case MUHASEBE_MODULU :
            table_name      = "muh_fisler";
            break;
        case FATURA_MODULU   :
            table_name      = "fat_faturalar";
            break;
        case PERAKENDE_MODULU :
            table_name      = "prk_fisler";
            break;
        default :
            return 0;

    };

    SQL_QUERY select_query( DB );

    select_query.PREPARE_SELECT( table_name, column_name, where_query );
    select_query.SET_VALUE( set_value_str, fis_id );

    if(select_query.SELECT() EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI()
***************************************************************************************/

int E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI()
{
    return E9_KULLANICI_YETKILERI->ynt_e9_kullanici_sirket_degistire_bilir_mi;
}

/**************************************************************************************
                   E9_KULLANICININ_CALISTIGI_SIRKET()
***************************************************************************************/

int E9_KULLANICININ_CALISTIGI_SIRKET()
{
    return E9_KULLANICI_YETKILERI->ynt_e9_kullanicinin_calistigi_sirket;
}
/*
*************************************************************************************
                   E9_KULLANICI_URETTIGI_ENT_FISLERINI_GOREBILIR_MI
***************************************************************************************/

int E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI()
{
    return E9_KULLANICI_YETKILERI->ynt_e9_urettigi_fisleri_grbilir_mi;
}

/**************************************************************************************
                   E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI()
***************************************************************************************/

int E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI()
{
    return E9_KULLANICI_YETKILERI->ynt_e9_ent_bilgilerini_degistirme_yetkisi;
}

/**************************************************************************************
                   E9_KULLANICI_URUN_HAREKETLERINI_GOREBILIR_MI()
***************************************************************************************/

int E9_KULLANICI_URUN_HAREKETLERINI_GOREBILIR_MI()
{
    return E9_KULLANICI_YETKILERI->stok_urun_hareketlerini_gorebilir_mi;
}

/**************************************************************************************
                   E9_KULLANICI_CARI_HAREKETLERINI_GOREBILIR_MI
***************************************************************************************/

int E9_KULLANICI_CARI_HAREKETLERINI_GOREBILIR_MI()
{
    return E9_KULLANICI_YETKILERI->cari_hesap_hareketleri_gorebilir_mi;
}

/**************************************************************************************
                   E9_CLEAR_ENT_DETAYLARI_STRUCT
***************************************************************************************/

void E9_CLEAR_ENT_DETAYLARI_STRUCT ( E9_ENT_DETAYLARI_STRUCT * P_ENT_DETAYLARI )
{
    P_ENT_DETAYLARI->muh_fis_no         =  0;
    P_ENT_DETAYLARI->banka_fis_no.clear();
    P_ENT_DETAYLARI->cari_fis_no        =  0;
    P_ENT_DETAYLARI->cek_fis_no         =  0;
    P_ENT_DETAYLARI->senet_fis_no       =  0;
    P_ENT_DETAYLARI->firma_cek_fis_no   =  0;
    P_ENT_DETAYLARI->firma_senet_fis_no =  0;
    P_ENT_DETAYLARI->muh_kasa_hesap_id  = 0;
}

/**************************************************************************************
                   E9_KULLANICILARI_YETKILERI_STRUCT_DOLDUR()
***************************************************************************************/

void E9_KULLANICI_YETKILERI_STRUCT_DOLDUR(int kullanici_id)
{
    if ( E9_KULLANICI_YETKILERI EQ NULL ) {
        E9_KULLANICI_YETKILERI = new E9_KULLANICI_YETKILERI_STRUCT;
    }

    E9_CLEAR_KULLANICI_YETKILERI_STRUCT ( E9_KULLANICI_YETKILERI );

    if (KULLANICI_ID() EQ 1) {
        return;
    }

    SQL_QUERY sql_query ( DB );

    /************************************************** SISTEM  ***************************************************/

        sql_query.PREPARE_SELECT("e9_kullanici_yetkileri",
                                 "sis_blg_formuna_girebilir_mi,         "
                                 "sis_kul_formuna_girebilir_mi,         "
                                 "sis_frm_formuna_girebilir_mi,         "
                                 "sis_yonetim_log_gorebilir_mi,         "
                                 "sis_mail_srv_tanimlayabilirmi,        "
                                 "sis_sms_srv_tanimlayabilirmi,         "
                                 "e9_yetkilendirme_yapabilir_mi,        "
                                 "muh_kullanabilir_mi,                  "
                                 "isl_kullanabilir_mi,                  "
                                 "adr_kullanabilir_mi,                  "
                                 "dvz_kullanabilir_mi,                  "
                                 "car_kullanabilir_mi,                  "
                                 "cek_kullanabilir_mi,                  "
                                 "bnk_kullanabilir_mi,                  "
                                 "sub_kullanabilir_mi,                  "
                                 "fat_kullanabilir_mi,                  "
                                 "irs_kullanabilir_mi,                  "
                                 "stk_kullanabilir_mi,                  "
                                 "prs_kullanabilir_mi,                  "
                                 "dmr_kullanabilir_mi,                  "
                                 "smm_makbuzu_kullanabilir_mi,           "
                                 "faiz_kullanabilir_mi,                 "
                                 "prk_kullanabilir_mi,                  "
                                 "e9_onar_yapabilir_mi,                 "
                                 "e9_devir_islemi_yapabilir_mi,         "
                                 "e9_log_gorebilir_mi ,                 "
                                 "e9_sabit_degerlere_girebilirmi,       "
                                 "e9_belge_tasarlayabilir_mi,           "
                                 "e9_ent_blg_degistirebilir_mi,         "
                                 "e9_kullanicinin_sirketi,              "
                                 "e9_sirket_degistire_bilir_mi,         "
                                 "e9_urettigi_fisleri_grbilir_mi,       "

    /***************************************************** ADRES  ****************************************************/

                             "adr_frm_shs_blg_gorebilir_mi,             "
                             "adr_not_defterini_gorebilir_mi,           "
                             "adr_adres_kayitlari_raporu,               "
                             "adr_tel_web_kayitlari_raporu,"
                             "adr_etiket_yazdirabilir_mi,               "
                             "adr_fihrist_yazdirabilir_mi,              "
                             "adr_toplu_mail_gonderebilir_mi,           "
                             "adr_grup_tanimlayabilir_mi,               "
                             "adr_toplu_sms_gonderebilir_mi,            "
                             "adr_export_yetkisi,                       "
                             "adr_import_yetkisi,                       "

    /********************************************************* BANKA  ***************************************************/

                             "bnk_hesabi_acabilir_mi,                   "
                             "bnk_ent_fislerini_gorebilir_mi,           "
                             "bnk_defterini_inceleyebilir_mi,           "
                             "bnk_pos_ekst_inceleyebilir_mi,            "
                             "bnk_hareket_fisi_girebilir_mi,            "
                             "bnk_acilis_fisi_girebilir_mi,             "
                             "bnk_makro_tanimlayabilir_mi,              "
                             "bnk_kk_ekstresi_alabilir_mi,              "
                             "bnk_virman_islemi_yapabilir_mi,           "
                             "bnk_makro_fisi_kesebilir_mi,              "

    /************************************************************** CARI  **********************************************/

                             "car_tah_fisi_kesebilir_mi,                "
                             "car_odeme_fisi_kesebilir_mi,              "
                             "car_hareket_fisi_kesebilir_mi,            "
                             "car_acilis_fisi_kesebilir_mi,             "
                             "car_grup_tanimlayabilir_mi,               "
                             "car_kart_tanimlayabilir_mi,               "
                             "car_ent_fislerini_gorebilir_mi,           "
                             "car_ekstreyi_inceleyebilir_mi,            "
                             "car_bakiye_rpr_inceleyebilirmi,           "
                             "car_virman_fisi_kesebilir_mi,             "
                             "car_hesap_hareketleri_grb_mi,             "

    /************************************************************** CEKSENET ******************************************/
                             "cek_senet_girebilir_mi ,                  "
                             "cek_bordro_girebilir_mi,                  "
                             "cek_ent_fislerini_gorebilir_mi,           "
                             "cek_listesini_inceleyebilir_mi,           "
                             "cek_mus_ceklerini_gorebilir_mi,           "
                             "cek_acilis_bordrosu_yetkisi,              "

    /*************************************************************** DEMIRBAS  *****************************************/

                             "dmr_karti_tanimlayabilir_mi,              "
                             "dmr_grubu_tanimlayabilir_mi,              "
                             "dmr_yeni_deger_fis_kesebilirmi,           "
                             "dmr_amrtsman_fisi_kesebilir_mi,           "
                             "dmr_listesi_raporu_alabilir_mi,           "
                             "dmr_toplu_amortisman_ayrablrmi,           "
                             "dmr_toplu_yeni_deger_ypblrmi  ,           "
                             "dmr_rpr_islm_grup_tanmlyblr_mi,           "

    /************************************************************** MUHASEBE  ******************************************/

                             "muh_mhs_fisi_kesebilir_mi,                "
                             "muh_ent_fislerini_gorebilir_mi,           "
                             "muh_hesap_planini_gorebilir_mi,           "
                             "muh_yvm_no_olusturabilir_mi,              "
                             "muh_hsp_ekstre_gorebilir_mi,              "
                             "muh_mizan_rprunu_gorebilir_mi,            "
                             "muh_hsp_plani_rpr_gorebilir_mi,           "
                             "muh_gelir_tblsunu_gorebilir_mi,           "
                             "muh_bilanco_inceleyebilir_mi,             "
                             "muh_yvm_dkmunu_gorebilir_mi,              "
                             "muh_dftri_kbr_inceleyebilir_mi,           "
                             "muh_fis_dokumunu_gorebilir_mi,            "
                             "muh_hata_fisleri_gorebilir_mi,            "
                             "muh_hsp_grbu_tanimlayabilir_mi,           "
                             "muh_yan_hesabi_girebilir_mi,              "
                             "muh_yan_fisi_olusturabilir_mi,            "
                             "muh_hesap_plani_kopyalama,                "
                             "muh_acilis_fisi_islem_yetkisi,            "

    /********************************************************** PERSONEL  ***********************************************/

                             "prs_tanimlarina_girebilir_mi,             "
                             "prs_ek_odnk_tanimlayabilir_mi,            "
                             "prs_ek_ksnti_tanimlayabilir_mi,           "
                             "prs_sbt_degerlere_girebilir_mi,           "
                             "prs_maas_brd_alabilir_mi,                 "
                             "prs_maas_brd_iptal_edebilir_mi,           "
                             "prs_maas_brd_rpr_alabilir_mi,             "
                             "prs_ihbr_kdm_rpr_alabilir_mi,             "
                             "prs_is_ayrilma_rpr_alabilir_mi,           "
                             "prs_brd_inceleme_yapabilir_mi,            "
                             "prs_prsonel_raporu_alabilir_mi,           "
                             "prs_e_bildirge_alabilir_mi,               "
                             "prs_prsonel_grup_girebilir_mi,            "
                             "prs_isten_ayr_prs_arayabilirmi,           "
                             "prs_eksik_gun_nedeni_girisi,              "
                             "prs_ucret_pusulasi_raporu ,               "

    /********************************************************* SUBE/ DEPO / UNITE  *************************************/

                             "sub_tanimlayabilir_mi,                    "
                             "sub_depo_tanimlayabilir_mi,               "
                             "sub_depo_islem_yetkisi,                   "
                             "sub_sube_islem_yetkisi,                   "
                             "sub_ent_hesaplari_yetkisi,                "

    /************************************************************** STOK **************************************************/

                             "stk_urun_kartina_girebilir_mi,            "
                             "stk_hizmet_tanimlayabilir_mi,             "
                             "stk_grubu_tanimlayabilir_mi,              "
                             "stk_fisine_girebilir_mi,                  "
                             "stk_urn_listesini_gorebilir_mi,           "
                             "stk_miktar_rprunu_gorebilir_mi,           "
                             "stk_ekstre_inceleyebilir_mi,              "
                             "stk_urun_hareketlerini_grb_mi,              "

    /***************************************               FATURA                  *************************************/

                             "fat_alis_faturasi_kesebilir_mi  ,         "
                             "fat_sts_faturasi_kesebilir_mi   ,         "
                             "fat_irs_faturalastirma_elle     ,         "
                             "fat_irs_faturalastirma_oto      ,         "
                             "fat_irs_faturalastirma_toplu    ,         "
                             "fat_satir_iskonto_yapabilir_mi  ,         "
                             "fat_toplu_iskonto_yapabilir_mi  ,         "
                             "fat_hizmet_ekstre_alabilir_mi   ,         "
                             "fat_ba_bs_alabilir_mi           ,         "
                             "fat_fatura_listesi_alabilir_mi  ,         "

    /**************************************                İRSALİYE                *************************************/

                             "irs_alis_irsaliye_kesebilir_mi,           "
                             "irs_sts_irsaliye_kesebilir_mi ,           "
                             "irs_fat_irsaliyelestirme_elle   ,         "
                             "irs_fat_irsaliyelestirme_oto    ,         "
                             "irs_satir_iskonto_yapabilir_mi  ,         "
                             "irs_listesi_gorebilir_mi        ,         "
                             "irs_toplu_iskonto_yapabilir_mi  ,         "


    /***************************************************** ISLETME ****************************************************/

                             "isl_raporunu_gorebilir_mi,                "
                             "isl_kdv_raporunu_gorebilir_mi,            "
                             "isl_fisi_kesebilir_mi,                    "
     /***************************************************** SM_MAKBUZU ****************************************************/

                             "smm_makbuzu_gorebilir_mi,"
                             "smm_hizmet_tanimlayabilir_mi,"
                             "smm_tahsil_edildi_odendi,    "

     /***************************************************** DOVIZ ****************************************************/
                             "dvz_kurlarini_gorebilir_mi,"
                             "dvz_tanimlayabilir_mi ,"
                             "dvz_kuru_guncelleyebilir_mi,"


    /**********************************************       PERAKENDE     ************************************************/

                             "prk_fisi_girebilir_mi,                    "
                             "prk_gdr_tur_tanimlayabilir_mi,            "
                             "prk_kasa_raporu_alabilir_mi               ",

                             "kullanici_id = :kullanici_id" );


    sql_query.SET_VALUE(":kullanici_id",kullanici_id);

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();

    E9_KULLANICI_YETKILERI->sis_blg_formuna_girebilir_mi         = sql_query.VALUE ( "sis_blg_formuna_girebilir_mi"     ).toInt();
    E9_KULLANICI_YETKILERI->sis_kul_formuna_girebilir_mi         = sql_query.VALUE ( "sis_kul_formuna_girebilir_mi"     ).toInt();
    E9_KULLANICI_YETKILERI->sis_veritabani_formuna_girebilir_mi  = sql_query.VALUE ( "sis_frm_formuna_girebilir_mi"     ).toInt();
    E9_KULLANICI_YETKILERI->sis_yonetim_log_inceleme_yetkisi     = sql_query.VALUE ( "sis_yonetim_log_gorebilir_mi"     ).toInt();
    E9_KULLANICI_YETKILERI->sis_mail_srv_tanimlayabilirmi        = sql_query.VALUE ( "sis_mail_srv_tanimlayabilirmi" ).toInt();
    E9_KULLANICI_YETKILERI->sis_sms_srv_tanimlayabilirmi         = sql_query.VALUE ( "sis_sms_srv_tanimlayabilirmi"  ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_yetkilendirme_yapabilir_mi    = sql_query.VALUE ( "e9_yetkilendirme_yapabilir_mi"    ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_onar_yetkisi                  = sql_query.VALUE ( "e9_onar_yapabilir_mi"             ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_devir_islemi_yapabilir_mi     = sql_query.VALUE ( "e9_devir_islemi_yapabilir_mi"     ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_log_inceleme_yetkisi          = sql_query.VALUE ( "e9_log_gorebilir_mi"              ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_sabit_degerler_guncelleme_yetkisi  = sql_query.VALUE ( "e9_sabit_degerlere_girebilirmi"  ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_belge_tasarlama_yetkisi            = sql_query.VALUE ( "e9_belge_tasarlayabilir_mi"       ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_ent_bilgilerini_degistirme_yetkisi = sql_query.VALUE ( "e9_ent_blg_degistirebilir_mi"     ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_kullanicinin_calistigi_sirket      = sql_query.VALUE ( "e9_kullanicinin_sirketi"          ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_kullanici_sirket_degistire_bilir_mi= sql_query.VALUE ( "e9_sirket_degistire_bilir_mi"     ).toInt();
    E9_KULLANICI_YETKILERI->ynt_e9_urettigi_fisleri_grbilir_mi        = sql_query.VALUE ( "e9_urettigi_fisleri_grbilir_mi"     ).toInt();

    E9_KULLANICI_YETKILERI->e9_muh_kullanabilir_mi             = sql_query.VALUE ( "muh_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_isl_kullanabilir_mi             = sql_query.VALUE ( "isl_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_adr_kullanabilir_mi             = sql_query.VALUE ( "adr_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_dvz_kullanabilir_mi             = sql_query.VALUE ( "dvz_kullanabilir_mi").toInt();
    E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi             = sql_query.VALUE ( "car_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi             = sql_query.VALUE ( "cek_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi             = sql_query.VALUE ( "bnk_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_sub_kullanabilir_mi             = sql_query.VALUE ( "sub_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi             = sql_query.VALUE ( "fat_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_irs_kullanabilir_mi             = sql_query.VALUE ( "irs_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_stk_kullanabilir_mi             = sql_query.VALUE ( "stk_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_prs_kullanabilir_mi             = sql_query.VALUE ( "prs_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_dmr_kullanabilir_mi             = sql_query.VALUE ( "dmr_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi             = sql_query.VALUE ("smm_makbuzu_kullanabilir_mi").toInt();
    E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi            = sql_query.VALUE ( "faiz_kullanabilir_mi" ).toInt();
    E9_KULLANICI_YETKILERI->e9_prk_kullanabilir_mi             = sql_query.VALUE ( "prk_kullanabilir_mi" ).toInt();

    //Modul yetkileri

    /************************************************************** ADRES ************************************************/

        E9_KULLANICI_YETKILERI->adr_adres_karti_tanimlama_yetkisi                   = sql_query.VALUE ( "adr_frm_shs_blg_gorebilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->adr_not_defteri_inceleme_yetkisi                    = sql_query.VALUE ( "adr_not_defterini_gorebilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->adr_adres_kayitlari_rapor_alma_yetkisi              = sql_query.VALUE ( "adr_adres_kayitlari_raporu"      ).toInt();
        E9_KULLANICI_YETKILERI->adr_tel_web_kayitlari_rapor_alma_yetkisi            = sql_query.VALUE ( "adr_tel_web_kayitlari_raporu"    ).toInt();
        E9_KULLANICI_YETKILERI->adr_etiket_yazdirma_yetkisi                         = sql_query.VALUE ( "adr_etiket_yazdirabilir_mi"      ).toInt();
        E9_KULLANICI_YETKILERI->adr_fihrist_raporu_alma_yetkisi                     = sql_query.VALUE ( "adr_fihrist_yazdirabilir_mi"     ).toInt();
        E9_KULLANICI_YETKILERI->adr_toplu_mail_gonderme_yetkisi                     = sql_query.VALUE ( "adr_toplu_mail_gonderebilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->adr_grup_tanimlama_yetkisi                          = sql_query.VALUE ( "adr_grup_tanimlayabilir_mi"      ).toInt();
        E9_KULLANICI_YETKILERI->adr_toplu_sms_gonderme_yetkisi                      = sql_query.VALUE ( "adr_toplu_sms_gonderebilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->adr_export_yetkisi                                  = sql_query.VALUE ( "adr_export_yetkisi").toInt();
        E9_KULLANICI_YETKILERI->adr_import_yetkisi                                  = sql_query.VALUE ( "adr_import_yetkisi").toInt();


    /************************************************************** BANKA *************************************************/

        E9_KULLANICI_YETKILERI->bnk_banka_hesabi_acma_yetkisi                       = sql_query.VALUE ( "bnk_hesabi_acabilir_mi"           ).toInt();
        E9_KULLANICI_YETKILERI->bnk_ent_fisleri_inceleme_yetkisi                    = sql_query.VALUE ( "bnk_ent_fislerini_gorebilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->bnk_defter_raporu_alma_yetkisi                      = sql_query.VALUE ( "bnk_defterini_inceleyebilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->bnk_pos_ekstresi_alma_yetkisi                       = sql_query.VALUE ( "bnk_pos_ekst_inceleyebilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->bnk_hareket_fisi_islem_yetkisi                      = sql_query.VALUE ( "bnk_hareket_fisi_girebilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->bnk_acilis_fisi_islem_yetkisi                       = sql_query.VALUE ( "bnk_acilis_fisi_girebilir_mi"     ).toInt();
        E9_KULLANICI_YETKILERI->bnk_makro_fisi_islem_yetkisi                        = sql_query.VALUE ( "bnk_makro_fisi_kesebilir_mi"      ).toInt();
        E9_KULLANICI_YETKILERI->bnk_kk_ekstresi_alma_yetkisi                        = sql_query.VALUE ( "bnk_kk_ekstresi_alabilir_mi"      ).toInt();
        E9_KULLANICI_YETKILERI->bnk_virman_islem_yetkisi                            = sql_query.VALUE ( "bnk_virman_islemi_yapabilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->bnk_makro_tanimlama_yetkisi                         = sql_query.VALUE ( "bnk_makro_tanimlayabilir_mi"      ).toInt();


    /***************************************************************** CARI ****************************************************/


        E9_KULLANICI_YETKILERI->cari_tahsilat_fisi_islem_yetkisi                      = sql_query.VALUE ( "car_tah_fisi_kesebilir_mi"        ).toInt();
        E9_KULLANICI_YETKILERI->cari_odeme_fisi_islem_yetkisi                         = sql_query.VALUE ( "car_odeme_fisi_kesebilir_mi"      ).toInt();
        E9_KULLANICI_YETKILERI->cari_hareket_fisi_islem_yetkisi                       = sql_query.VALUE ( "car_hareket_fisi_kesebilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->cari_acilis_fisi_islem_yetkisi                        = sql_query.VALUE ( "car_acilis_fisi_kesebilir_mi"     ).toInt();
        E9_KULLANICI_YETKILERI->cari_grup_tanimlama_yetkisi                           = sql_query.VALUE ( "car_grup_tanimlayabilir_mi"       ).toInt();
        E9_KULLANICI_YETKILERI->cari_kart_tanimlama_yetkisi                           = sql_query.VALUE ( "car_kart_tanimlayabilir_mi"       ).toInt();
        E9_KULLANICI_YETKILERI->cari_ent_fisleri_inceleme_yetkisi                     = sql_query.VALUE ( "car_ent_fislerini_gorebilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->cari_ekstre_alma_yetkisi                              = sql_query.VALUE ( "car_ekstreyi_inceleyebilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->cari_bakiye_raporu_alma_yetkisi                       = sql_query.VALUE ( "car_bakiye_rpr_inceleyebilirmi"  ).toInt();
        E9_KULLANICI_YETKILERI->cari_virman_islem_yetkisi                             = sql_query.VALUE ( "car_virman_fisi_kesebilir_mi"     ).toInt();
        E9_KULLANICI_YETKILERI->cari_hesap_hareketleri_gorebilir_mi                   = sql_query.VALUE ( "car_hesap_hareketleri_grb_mi"     ).toInt();

    /**************************************************************** CEKSENET ***********************************************/

        E9_KULLANICI_YETKILERI->cek_senet_girebilir_mi                              = sql_query.VALUE ( "cek_senet_girebilir_mi"         ).toInt();
        E9_KULLANICI_YETKILERI->cek_bordrosu_islem_yetkisi                          = sql_query.VALUE ( "cek_bordro_girebilir_mi"        ).toInt();
        E9_KULLANICI_YETKILERI->cek_ent_fisleri_inceleme_yetkisi                    = sql_query.VALUE ( "cek_ent_fislerini_gorebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->cek_listesi_rapor_alma_yetkisi                      = sql_query.VALUE ( "cek_listesini_inceleyebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->cek_mus_cek_senet_arama_yetkisi                     = sql_query.VALUE ( "cek_mus_ceklerini_gorebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->cek_acilis_bordrosu_islem_yetkisi                   = sql_query.VALUE ( "cek_acilis_bordrosu_yetkisi"    ).toInt();

    /*************************************************************** DEMIRBAS ************************************************/

        E9_KULLANICI_YETKILERI->dmr_demirbas_karti_tanimlama_yetkisi                = sql_query.VALUE ( "dmr_karti_tanimlayabilir_mi"      ).toInt();
        E9_KULLANICI_YETKILERI->dmr_grup_tanimlama_yetkisi                          = sql_query.VALUE ( "dmr_grubu_tanimlayabilir_mi"      ).toInt();
        E9_KULLANICI_YETKILERI->dmr_yen_degerleme_fisi_islem_yetkisi                = sql_query.VALUE ( "dmr_yeni_deger_fis_kesebilirmi" ).toInt();
        E9_KULLANICI_YETKILERI->dmr_amortisman_fisi_islem_yetkisi                   = sql_query.VALUE   ( "dmr_amrtsman_fisi_kesebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->dmr_demirbas_listesi_rapor_alma_yetkisi             = sql_query.VALUE ( "dmr_listesi_raporu_alabilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->dmr_toplu_amortisman_ayirma_yetkisi                 = sql_query.VALUE ( "dmr_toplu_amortisman_ayrablrmi").toInt();
        E9_KULLANICI_YETKILERI->dmr_toplu_yeniden_degerleme_yetkisi                 = sql_query.VALUE ( "dmr_toplu_yeni_deger_ypblrmi").toInt();
        E9_KULLANICI_YETKILERI->dmr_rapor_islem_grubu_tanimlama_yetkisi             = sql_query.VALUE ( "dmr_rpr_islm_grup_tanmlyblr_mi").toInt();

    /**************************************************************** MUHASEBE ************************************************/


        E9_KULLANICI_YETKILERI->muh_muhasebe_fisi_islem_yetkisi                     = sql_query.VALUE ( "muh_mhs_fisi_kesebilir_mi"        ).toInt();
        E9_KULLANICI_YETKILERI->muh_ent_fisleri_inceleme_yetkisi                    = sql_query.VALUE ( "muh_ent_fislerini_gorebilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->muh_hesap_plani_islem_yetkisi                       = sql_query.VALUE ( "muh_hesap_planini_gorebilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->muh_yvm_no_olusturma_yetkisi                        = sql_query.VALUE ( "muh_yvm_no_olusturabilir_mi"      ).toInt();
        E9_KULLANICI_YETKILERI->muh_hesap_ekstresi_alma_yetkisi                     = sql_query.VALUE ( "muh_hsp_ekstre_gorebilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->muh_mizan_raporu_alma_yetkisi                       = sql_query.VALUE ( "muh_mizan_rprunu_gorebilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->muh_hesap_plani_rapor_alma_yetkisi                  = sql_query.VALUE ( "muh_hsp_plani_rpr_gorebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->muh_gelir_tablosu_rapor_alma_yetkisi                = sql_query.VALUE ( "muh_gelir_tblsunu_gorebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->muh_bilanco_rapor_alma_yetkisi                      = sql_query.VALUE ( "muh_bilanco_inceleyebilir_mi"     ).toInt();
        E9_KULLANICI_YETKILERI->muh_yvm_dokumu_alma_yetkisi                         = sql_query.VALUE ( "muh_yvm_dkmunu_gorebilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->muh_defteri_kebir_raporu_alma_yetkisi               = sql_query.VALUE ( "muh_dftri_kbr_inceleyebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->muh_fis_dokumu_alma_yetkisi                         = sql_query.VALUE ( "muh_fis_dokumunu_gorebilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->muh_hatali_fisleri_inceleme_yetkisi                 = sql_query.VALUE ( "muh_hata_fisleri_gorebilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->muh_grup_tanimlama_yetkisi                          = sql_query.VALUE ( "muh_hsp_grbu_tanimlayabilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->muh_yansitma_fisi_olusturma_yetkisi                 = sql_query.VALUE ( "muh_yan_fisi_olusturabilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->muh_yansitma_hesabi_tanimlama_yetkisi               = sql_query.VALUE ( "muh_yan_hesabi_girebilir_mi"      ).toInt();
        E9_KULLANICI_YETKILERI->muh_hesap_plani_kopyalama_yetkisi                   = sql_query.VALUE ( "muh_hesap_plani_kopyalama"        ).toInt();
        E9_KULLANICI_YETKILERI->muh_acilis_fisi_islem_yetkisi                       = sql_query.VALUE ( "muh_acilis_fisi_islem_yetkisi"    ).toInt();

    /******************************************************************* PERSONEL ******************************************/

        E9_KULLANICI_YETKILERI->prs_bordro_inceleme_yetkisi                         =  sql_query.VALUE ( "prs_brd_inceleme_yapabilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->prs_ihbar_ve_kidem_tazminati_raporu_alma_yetkisi    =  sql_query.VALUE ( "prs_ihbr_kdm_rpr_alabilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->prs_isten_ayrilan_personel_arama_yetkisi            =  sql_query.VALUE ( "prs_isten_ayr_prs_arayabilirmi"  ).toInt();
        E9_KULLANICI_YETKILERI->prs_isten_ayrilma_islem_yetkisi                     =  sql_query.VALUE ( "prs_is_ayrilma_rpr_alabilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->prs_maas_bordrosu_alma_yetkisi                      =  sql_query.VALUE ( "prs_maas_brd_alabilir_mi"         ).toInt();
        E9_KULLANICI_YETKILERI->prs_maas_bordrosu_iptal_yetkisi                     =  sql_query.VALUE ( "prs_maas_brd_iptal_edebilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->prs_maas_bordrosu_raporu_alma_yetkisi               =  sql_query.VALUE ( "prs_maas_brd_rpr_alabilir_mi"     ).toInt();
        E9_KULLANICI_YETKILERI->prs_ek_kesinti_tanimlama_yetkisi                    =  sql_query.VALUE ( "prs_ek_ksnti_tanimlayabilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->prs_ek_odenek_tanimlama_yetkisi                     =  sql_query.VALUE ( "prs_ek_odnk_tanimlayabilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->prs_personel_raporu_alma_yetkisi                    =  sql_query.VALUE ( "prs_prsonel_raporu_alabilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->prs_sicil_karti_tanimlama_yetkisi                   =  sql_query.VALUE ( "prs_tanimlarina_girebilir_mi"     ).toInt();
        E9_KULLANICI_YETKILERI->prs_sabit_deger_guncelleme_yetkisi                  =  sql_query.VALUE ( "prs_sbt_degerlere_girebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->prs_e_bildirge_alma_yetkisi                         =  sql_query.VALUE ( "prs_e_bildirge_alabilir_mi").toInt();
        E9_KULLANICI_YETKILERI->prs_grup_tanimlama_yetkisi                          =  sql_query.VALUE ( "prs_prsonel_grup_girebilir_mi").toInt();
        E9_KULLANICI_YETKILERI->prs_eksik_gun_nedenleri_giris_yetkisi               =  sql_query.VALUE ( "prs_eksik_gun_nedeni_girisi").toInt();
        E9_KULLANICI_YETKILERI->prs_ucret_pusulasi_raporu_alma_yetkisi              =  sql_query.VALUE ( "prs_ucret_pusulasi_raporu").toInt();

    /******************************************************************* STOK *************************************************/

        E9_KULLANICI_YETKILERI->stok_urun_karti_tanimlama_yetkisi                    =  sql_query.VALUE ( "stk_urun_kartina_girebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->stok_hizmet_karti_tanimlama_yetkisi                  =  sql_query.VALUE ( "stk_hizmet_tanimlayabilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->stok_fisi_islem_yetkisi                              =  sql_query.VALUE ( "stk_fisine_girebilir_mi"       ).toInt();
        E9_KULLANICI_YETKILERI->stok_grup_tanimlama_yetkisi                          =  sql_query.VALUE ( "stk_grubu_tanimlayabilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->stok_urun_listesi_alma_yetkisi                       =  sql_query.VALUE ( "stk_urn_listesini_gorebilir_mi").toInt();
        E9_KULLANICI_YETKILERI->stok_miktar_raporu_alma_yetkisi                      =  sql_query.VALUE ( "stk_miktar_rprunu_gorebilir_mi").toInt();
        E9_KULLANICI_YETKILERI->stok_ekstresi_alma_yetkisi                           =  sql_query.VALUE ( "stk_ekstre_inceleyebilir_mi").toInt();
        E9_KULLANICI_YETKILERI->stok_urun_hareketlerini_gorebilir_mi                 =  sql_query.VALUE ( "stk_urun_hareketlerini_grb_mi").toInt();

    /****************************************************        FATURA             *********************************************/

        E9_KULLANICI_YETKILERI->fat_alis_faturasi_kesebilir_mi                     = sql_query.VALUE ( "fat_alis_faturasi_kesebilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->fat_sts_faturasi_kesebilir_mi                      = sql_query.VALUE ( "fat_sts_faturasi_kesebilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_elle                        = sql_query.VALUE ( "fat_irs_faturalastirma_elle"      ).toInt();
        E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_oto                         = sql_query.VALUE ( "fat_irs_faturalastirma_oto"       ).toInt();
        E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_toplu                       = sql_query.VALUE ( "fat_irs_faturalastirma_toplu"     ).toInt();
        E9_KULLANICI_YETKILERI->fat_satir_iskontosu_yapabilir_mi                   = sql_query.VALUE ( "fat_satir_iskonto_yapabilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->fat_toplu_iskonto_yapabilir_mi                     = sql_query.VALUE ( "fat_toplu_iskonto_yapabilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->fat_hizmet_ekstresi_alma_yetkisi                      = sql_query.VALUE ( "fat_hizmet_ekstre_alabilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->fat_ba_bs_listesi_alma_yetkisi                              = sql_query.VALUE ( "fat_ba_bs_alabilir_mi"            ).toInt();
        E9_KULLANICI_YETKILERI->fat_fatura_listesi_alma_yetkisi                     = sql_query.VALUE ( "fat_fatura_listesi_alabilir_mi"   ).toInt();

    /******************************************************        İRSALİYE         ************************************************/

        E9_KULLANICI_YETKILERI->irs_alis_irsaliyesi_islem_yetkisi                  = sql_query.VALUE ( "irs_alis_irsaliye_kesebilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->irs_sts_irsaliyesi_islem_yetkisi                   = sql_query.VALUE ( "irs_sts_irsaliye_kesebilir_mi"    ).toInt();
        E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_elle                      = sql_query.VALUE ( "irs_fat_irsaliyelestirme_elle"    ).toInt();
        E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_oto                       = sql_query.VALUE ( "irs_fat_irsaliyelestirme_oto"     ).toInt();
        E9_KULLANICI_YETKILERI->irs_satir_iskontosu_yapabilir_mi                   = sql_query.VALUE ( "irs_satir_iskonto_yapabilir_mi"   ).toInt();
        E9_KULLANICI_YETKILERI->irs_listesi_gorebilir_mi                           = sql_query.VALUE ( "irs_listesi_gorebilir_mi"         ).toInt();
        E9_KULLANICI_YETKILERI->irs_toplu_iskonto_yapabilir_mi                     = sql_query.VALUE ( "irs_toplu_iskonto_yapabilir_mi"   ).toInt();



    /***************************************************************** SUBE DEPO UNITE ***************************************/


        E9_KULLANICI_YETKILERI->sub_sube_tanimlama_yetkisi                          = sql_query.VALUE ( "sub_tanimlayabilir_mi"       ).toInt();
        E9_KULLANICI_YETKILERI->sub_depo_tanimlama_yetkisi                          = sql_query.VALUE ( "sub_depo_tanimlayabilir_mi"  ).toInt();
        E9_KULLANICI_YETKILERI->sub_depo_islem_yetkisi                              = sql_query.VALUE ( "sub_depo_islem_yetkisi"      ).toInt();
        E9_KULLANICI_YETKILERI->sub_sube_islem_yetkisi                              = sql_query.VALUE ( "sub_sube_islem_yetkisi"      ).toInt();
        E9_KULLANICI_YETKILERI->sub_ent_hesaplari_guncelleme_yetkisi                = sql_query.VALUE ( "sub_ent_hesaplari_yetkisi"   ).toInt();

    /**************************************************************** ISLETME ************************************************/

        E9_KULLANICI_YETKILERI->isl_isletme_defteri_rapor_alma_yetkisi               = sql_query.VALUE ( "isl_raporunu_gorebilir_mi"     ).toInt();
        E9_KULLANICI_YETKILERI->isl_kdv_raporu_alma_yetkisi                          = sql_query.VALUE ( "isl_kdv_raporunu_gorebilir_mi" ).toInt();
        E9_KULLANICI_YETKILERI->isl_fisi_islem_yetkisi                               = sql_query.VALUE ( "isl_fisi_kesebilir_mi"         ).toInt();



    /**************************************************************** SM_MAKBUZU ************************************************/

        E9_KULLANICI_YETKILERI->smm_makbuzu_islem_yetkisi                           = sql_query.VALUE("smm_makbuzu_gorebilir_mi").toInt();
        E9_KULLANICI_YETKILERI->smm_hizmet_tanimlama_yetkisi                        = sql_query.VALUE("smm_hizmet_tanimlayabilir_mi").toInt();
        E9_KULLANICI_YETKILERI->smm_tahsil_edildi_odendi_secili_gelsin              = sql_query.VALUE("smm_tahsil_edildi_odendi").toInt();

    /**************************************************************** DOVIZ ************************************************/

        E9_KULLANICI_YETKILERI->dvz_dovizleri_inceleme_yetkisi                        = sql_query.VALUE("dvz_kurlarini_gorebilir_mi").toInt();
        E9_KULLANICI_YETKILERI->dvz_kur_guncelleme_yetkisi                            = sql_query.VALUE("dvz_kuru_guncelleyebilir_mi").toInt();
        E9_KULLANICI_YETKILERI->dvz_doviz_tanimlama_yetkisi                           = sql_query.VALUE("dvz_tanimlayabilir_mi").toInt();


    /*******************************************************    PERAKENDE      *****************************************/

        E9_KULLANICI_YETKILERI->prk_fis_girebilir_mi                               = sql_query.VALUE  ( "prk_fisi_girebilir_mi").toInt();
        E9_KULLANICI_YETKILERI->prk_gdr_tur_tanimlayabilir_mi                      = sql_query.VALUE  ( "prk_gdr_tur_tanimlayabilir_mi").toInt();
        E9_KULLANICI_YETKILERI->prk_kasa_raporu_alabilir_mi                        = sql_query.VALUE  ( "prk_kasa_raporu_alabilir_mi").toInt();
}

/**************************************************************************************
                   E9_UPDATE_TANIMLANMAMIS_ENT_HESAPLARI
***************************************************************************************/
void E9_UPDATE_TANIMLANMAMIS_ENT_HESAPLARI( ADAK_SQL * P_GELEN_DB)
{
    //PERSONEL MAAS BORDROSU ENTEGRASYON KONTROLU

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }


    SQL_QUERY query(USE_DB);
    SQL_QUERY sql_query ( USE_DB );

    int sube_id = SUBE_GET_SUBE_ID();

    query.PREPARE_SELECT("sub_subeler",
                         "sube_prs_gdrlr_hsp_id,sube_yurtici_satis_hsp_id,"
                         "sube_hizmet_urt_maliyet_hsp_id , "
                         "sube_sarf_malzemeleri_hsp_id , sube_port_cekler_hsp_id,"
                         "sube_tah_cekler_hsp_id, sube_tem_cekler_hsp_id,sube_kar_cekler_hsp_id,"
                         "sube_port_snt_hsp_id,sube_tah_snt_hsp_id ,"
                         "sube_tem_snt_hsp_id , sube_pro_snt_hsp_id , "
                         "sube_port_snt_ileri_trh_hsp_id , sube_tah_snt_ileri_trh_hsp_id,"
                         "sube_tem_snt_ileri_trh_hsp_id  , sube_borc_snt_hsp_id,"
                         "sube_borc_snt_ileri_trh_hsp_id , sube_sup_alacaklar_hsp_id,"
                         "sube_tah_edlmyn_cekler_hsp_id  , sube_tah_edlmyn_snt_hsp_id ",
                         "sube_id =:sube_id ");
    query.SET_VALUE(":sube_id" , sube_id );

    if ( query.SELECT() NE 0 ) {

        query.NEXT();
        int prs_giderler_hesap_id = query.VALUE("sube_prs_gdrlr_hsp_id").toInt();
        if ( prs_giderler_hesap_id EQ 0 ) {
            prs_giderler_hesap_id = MUH_GET_TALI_HESAP_ID("740");
        }

        int yurtici_satislar_hesap_id = query.VALUE("sube_yurtici_satis_hsp_id").toInt();
        if ( yurtici_satislar_hesap_id EQ 0 ) {
            yurtici_satislar_hesap_id = MUH_GET_TALI_HESAP_ID("600");
        }
        int hizmet_uretim_maliyeti_hesap_id = query.VALUE("sube_hizmet_urt_maliyet_hsp_id").toInt();
        if ( hizmet_uretim_maliyeti_hesap_id EQ 0 ) {
            hizmet_uretim_maliyeti_hesap_id = MUH_GET_TALI_HESAP_ID("740");
        }
        int sarf_malzemeleri_hesap_id = query.VALUE("sube_sarf_malzemeleri_hsp_id").toInt();
        if ( sarf_malzemeleri_hesap_id EQ 0 ) {
            sarf_malzemeleri_hesap_id = MUH_GET_TALI_HESAP_ID("710");
        }
        int port_cekler_hesap_id = query.VALUE("sube_port_cekler_hsp_id").toInt();
        if ( port_cekler_hesap_id EQ 0 ) {
            port_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("101");
        }
        int tah_cekler_hesap_id = query.VALUE("sube_tah_cekler_hsp_id").toInt();

        if ( tah_cekler_hesap_id EQ 0 ) {
            tah_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("101");
        }
        int tem_cekler_hesap_id = query.VALUE("sube_tem_cekler_hsp_id").toInt();
        if ( tem_cekler_hesap_id EQ 0 ) {
            tem_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("sube_tem_cekler_hsp_id");
        }
        int kar_cekler_hesap_id = query.VALUE("sube_kar_cekler_hsp_id").toInt();
        if ( kar_cekler_hesap_id EQ 0 ) {
            kar_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("101");
        }
        int port_senetler_hesap_id = query.VALUE("sube_port_snt_hsp_id").toInt();
        if ( port_senetler_hesap_id EQ 0 ) {
            port_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        int tah_senetler_hesap_id = query.VALUE("sube_tah_snt_hsp_id").toInt();
        if ( tah_senetler_hesap_id EQ 0 ) {
            tah_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        int tem_senetler_hesap_id = query.VALUE("sube_tem_snt_hsp_id").toInt();
        if ( tem_senetler_hesap_id EQ 0 ) {
            tem_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        int pro_senetler_hesap_id = query.VALUE("sube_pro_snt_hsp_id").toInt();
        if ( pro_senetler_hesap_id EQ 0 ) {
            pro_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        int port_senetler_ileri_tarih_hesap_id = query.VALUE("sube_port_snt_ileri_trh_hsp_id").toInt();
        if ( port_senetler_ileri_tarih_hesap_id EQ 0 ) {
            port_senetler_ileri_tarih_hesap_id = MUH_GET_TALI_HESAP_ID("221");
        }
        int tah_senetler_ileri_tarih_hesap_id = query.VALUE("sube_tah_snt_ileri_trh_hsp_id").toInt();
        if ( tah_senetler_ileri_tarih_hesap_id EQ 0 ) {
            tah_senetler_ileri_tarih_hesap_id = MUH_GET_TALI_HESAP_ID("221");
        }
        int tem_senetler_ileri_tarih_hesap_id = query.VALUE("sube_tem_snt_ileri_trh_hsp_id").toInt();
        if ( tem_senetler_ileri_tarih_hesap_id EQ 0 ) {
            tem_senetler_ileri_tarih_hesap_id = MUH_GET_TALI_HESAP_ID("221");
        }

        int borc_senetleri_hesap_id = query.VALUE("sube_borc_snt_hsp_id").toInt();
        if ( borc_senetleri_hesap_id EQ 0 ) {
            borc_senetleri_hesap_id = MUH_GET_TALI_HESAP_ID("321");
        }
        int borc_senetleri_ileri_tarih_hesap_id = query.VALUE("sube_borc_snt_ileri_trh_hsp_id").toInt();
        if ( borc_senetleri_ileri_tarih_hesap_id EQ 0 ) {
            borc_senetleri_ileri_tarih_hesap_id = MUH_GET_TALI_HESAP_ID("421");
        }
        int sup_alacaklar_hesap_id = query.VALUE("sube_sup_alacaklar_hsp_id").toInt();
        if ( sup_alacaklar_hesap_id EQ 0 ) {
            sup_alacaklar_hesap_id = MUH_GET_TALI_HESAP_ID("128");
        }

        int tah_edilemeyen_cekler_hesap_id = query.VALUE("sube_tah_edlmyn_cekler_hsp_id").toInt();
        if ( tah_edilemeyen_cekler_hesap_id EQ 0 ) {
            tah_edilemeyen_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("101");
        }

        int tah_edilemeyen_senetler_hesap_id = query.VALUE("sube_tah_edlmyn_snt_hsp_id").toInt();
        if ( tah_edilemeyen_senetler_hesap_id EQ 0 ) {
            tah_edilemeyen_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        sql_query.PREPARE_UPDATE ( "sub_subeler ",
                                   "sube_id",
                                   "sube_port_cekler_hsp_id                , "
                                   "sube_tah_cekler_hsp_id                 , "
                                   "sube_tem_cekler_hsp_id                 , "
                                   "sube_kar_cekler_hsp_id                 , "
                                   "sube_port_snt_hsp_id                   , "
                                   "sube_tah_snt_hsp_id                    , "
                                   "sube_tem_snt_hsp_id                    , "
                                   "sube_pro_snt_hsp_id                    , "
                                   "sube_port_snt_ileri_trh_hsp_id         , "
                                   "sube_tah_snt_ileri_trh_hsp_id          , "
                                   "sube_tem_snt_ileri_trh_hsp_id          , "
                                   "sube_borc_snt_hsp_id                   , "
                                   "sube_borc_snt_ileri_trh_hsp_id         , "
                                   "sube_sup_alacaklar_hsp_id              , "
                                   "sube_tah_edlmyn_cekler_hsp_id          , "
                                   "sube_tah_edlmyn_snt_hsp_id             , "
                                   "sube_yurtici_satis_hsp_id              , "
                                   "sube_hizmet_urt_maliyet_hsp_id         , "
                                   "sube_prs_gdrlr_hsp_id                  , "
                                   "sube_sarf_malzemeleri_hsp_id           ",

                                   "sube_id = :sube_id " );

        sql_query.SET_VALUE      ( ":sube_port_cekler_hsp_id",                  port_cekler_hesap_id                    );
        sql_query.SET_VALUE      ( ":sube_tah_cekler_hsp_id",                   tah_cekler_hesap_id                     );
        sql_query.SET_VALUE      ( ":sube_tem_cekler_hsp_id",                   tem_cekler_hesap_id                     );
        sql_query.SET_VALUE      ( ":sube_kar_cekler_hsp_id",                   kar_cekler_hesap_id                     );
        sql_query.SET_VALUE      ( ":sube_port_snt_hsp_id",                     port_senetler_hesap_id                  );
        sql_query.SET_VALUE      ( ":sube_tah_snt_hsp_id",                      tah_senetler_hesap_id                   );
        sql_query.SET_VALUE      ( ":sube_tem_snt_hsp_id",                      tem_senetler_hesap_id                   );
        sql_query.SET_VALUE      ( ":sube_pro_snt_hsp_id",                      pro_senetler_hesap_id                   );
        sql_query.SET_VALUE      ( ":sube_port_snt_ileri_trh_hsp_id",           port_senetler_ileri_tarih_hesap_id      );
        sql_query.SET_VALUE      ( ":sube_tah_snt_ileri_trh_hsp_id",            tah_senetler_ileri_tarih_hesap_id       );
        sql_query.SET_VALUE      ( ":sube_tem_snt_ileri_trh_hsp_id",            tem_senetler_ileri_tarih_hesap_id       );
        sql_query.SET_VALUE      ( ":sube_borc_snt_hsp_id",                     borc_senetleri_hesap_id                 );
        sql_query.SET_VALUE      ( ":sube_borc_snt_ileri_trh_hsp_id",           borc_senetleri_ileri_tarih_hesap_id     );
        sql_query.SET_VALUE      ( ":sube_sup_alacaklar_hsp_id",                sup_alacaklar_hesap_id                  );
        sql_query.SET_VALUE      ( ":sube_tah_edlmyn_cekler_hsp_id",            tah_edilemeyen_cekler_hesap_id          );
        sql_query.SET_VALUE      ( ":sube_tah_edlmyn_snt_hsp_id",               tah_edilemeyen_senetler_hesap_id        );
        sql_query.SET_VALUE      ( ":sube_yurtici_satis_hsp_id",                yurtici_satislar_hesap_id               );
        sql_query.SET_VALUE      ( ":sube_hizmet_urt_maliyet_hsp_id",           hizmet_uretim_maliyeti_hesap_id         );
        sql_query.SET_VALUE      ( ":sube_prs_gdrlr_hsp_id",                    prs_giderler_hesap_id                   );
        sql_query.SET_VALUE      ( ":sube_sarf_malzemeleri_hsp_id",             sarf_malzemeleri_hesap_id               );
        sql_query.SET_VALUE      ( ":sube_id",                                  sube_id                                 );



        sql_query.UPDATE();

    }

    query.PREPARE_SELECT( "prs_personeller",
                          "prs_borclar_hesap_id,"
                          "prs_avanslar_hesap_id, "
                          "personel_id");

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() ) {

            int personele_borclar_hsp_id = query.VALUE(0).toInt();
            int prs_avanslar_hesap_id    = query.VALUE(1).toInt();
            int personel_id              = query.VALUE(2).toInt();

            if ( personele_borclar_hsp_id EQ 0 ) {
                personele_borclar_hsp_id = MUH_GET_TALI_HESAP_ID("335");
            }
            if ( prs_avanslar_hesap_id EQ 0 ) {
                prs_avanslar_hesap_id = MUH_GET_TALI_HESAP_ID("196");
            }

            sql_query.PREPARE_UPDATE("prs_personeller",
                                     "personel_id",
                                     "prs_avanslar_hesap_id        , "
                                     "prs_borclar_hesap_id           ",
                                     "personel_id = :personel_id");
            sql_query.SET_VALUE(":prs_avanslar_hesap_id"        , prs_avanslar_hesap_id);
            sql_query.SET_VALUE(":prs_borclar_hesap_id"         , personele_borclar_hsp_id);
            sql_query.SET_VALUE(":personel_id"                  , personel_id);

            sql_query.UPDATE();

        }
    }

    int IND_KDV_HESAPLARI_ID_ARRAY      [ KDV_OTV_ORANLARI_SAYISI ];
    int IND_KDV_HESAPLARI_IADE_ID_ARRAY [ KDV_OTV_ORANLARI_SAYISI ];
    int HES_KDV_HESAPLARI_ID_ARRAY      [ KDV_OTV_ORANLARI_SAYISI ];
    int HES_KDV_HESAPLARI_IADE_ID_ARRAY [ KDV_OTV_ORANLARI_SAYISI ];

    int OTV_ALIS_HESAPLARI_ID_ARRAY     [ KDV_OTV_ORANLARI_SAYISI ];
    int OTV_SATIS_HESAPLARI_ID_ARRAY    [ KDV_OTV_ORANLARI_SAYISI ];

    int TEVKIFAT_KDV_HESAPLARI_ID_ARRAY [ KDV_OTV_ORANLARI_SAYISI ];

    query.PREPARE_SELECT("e9_sabit_degerler",
                         "ind_kdv_hesap_id_array, "
                         "ind_kdv_iade_hesap_id_array , hes_kdv_hesap_id_array,"
                         "hes_kdv_iade_hesap_id_array,otv_alis_hesap_id_array ,"
                         "otv_satis_hesap_id_array,tevkifat_kdv_hesap_id_array,"
                         "kdv_orani_sayisi,otv_orani_sayisi "
                         );
    if ( query.SELECT() NE 0 ) {

        query.NEXT();


        UNPACK_INT_ARRAY(query.VALUE("ind_kdv_hesap_id_array").toString()      , IND_KDV_HESAPLARI_ID_ARRAY            , KDV_OTV_ORANLARI_SAYISI);
        UNPACK_INT_ARRAY(query.VALUE("ind_kdv_iade_hesap_id_array").toString() , IND_KDV_HESAPLARI_IADE_ID_ARRAY       , KDV_OTV_ORANLARI_SAYISI);
        UNPACK_INT_ARRAY(query.VALUE("hes_kdv_hesap_id_array").toString()      , HES_KDV_HESAPLARI_ID_ARRAY            , KDV_OTV_ORANLARI_SAYISI);
        UNPACK_INT_ARRAY(query.VALUE("hes_kdv_iade_hesap_id_array").toString() , HES_KDV_HESAPLARI_IADE_ID_ARRAY       , KDV_OTV_ORANLARI_SAYISI);
        UNPACK_INT_ARRAY(query.VALUE("otv_alis_hesap_id_array").toString()     , OTV_ALIS_HESAPLARI_ID_ARRAY           , KDV_OTV_ORANLARI_SAYISI);
        UNPACK_INT_ARRAY(query.VALUE("otv_satis_hesap_id_array").toString()    , OTV_SATIS_HESAPLARI_ID_ARRAY          , KDV_OTV_ORANLARI_SAYISI);
        UNPACK_INT_ARRAY(query.VALUE("tevkifat_kdv_hesap_id_array").toString() , TEVKIFAT_KDV_HESAPLARI_ID_ARRAY       , KDV_OTV_ORANLARI_SAYISI);


        int kdv_orani_sayisi = query.VALUE("kdv_orani_sayisi").toInt();
        int otv_orani_sayisi = query.VALUE("otv_orani_sayisi").toInt();

        int ind_kdv_hesap_id        = MUH_GET_HESAP_ID ( "191" );
        int hes_kdv_hesap_id        = MUH_GET_HESAP_ID ( "391" );
        int ind_kdv_iade_hesap_id   = MUH_GET_HESAP_ID ( "392" );
        int hes_kdv_iade_hesap_id   = MUH_GET_HESAP_ID ( "192" );

        int otv_alis_hesap_id       = MUH_GET_HESAP_ID ( "194" );
        int otv_satis_hesap_id      = MUH_GET_HESAP_ID ( "394" );
        int tevkifat_kdv_hesap_id   = MUH_GET_HESAP_ID ( "360" );


        for ( int i = 1; i < kdv_orani_sayisi; i++ ) {

            if ( IND_KDV_HESAPLARI_ID_ARRAY[i] EQ 0 ) {
                IND_KDV_HESAPLARI_ID_ARRAY      [i] = ind_kdv_hesap_id;
            }
            if ( IND_KDV_HESAPLARI_IADE_ID_ARRAY[i] EQ 0 ) {
                IND_KDV_HESAPLARI_IADE_ID_ARRAY [i] = ind_kdv_iade_hesap_id;
            }
            if ( HES_KDV_HESAPLARI_ID_ARRAY[i] EQ 0 ) {
                HES_KDV_HESAPLARI_ID_ARRAY      [i] = hes_kdv_hesap_id;
            }
            if ( HES_KDV_HESAPLARI_IADE_ID_ARRAY[i] EQ 0 ) {
                HES_KDV_HESAPLARI_IADE_ID_ARRAY [i] = hes_kdv_iade_hesap_id;
            }
            if ( TEVKIFAT_KDV_HESAPLARI_ID_ARRAY[i] EQ 0 ) {
                TEVKIFAT_KDV_HESAPLARI_ID_ARRAY [i] = tevkifat_kdv_hesap_id;
            }
        }

        for ( int i = 1; i < otv_orani_sayisi; i++ ) {

            if ( OTV_ALIS_HESAPLARI_ID_ARRAY[i] EQ 0 ) {
               OTV_ALIS_HESAPLARI_ID_ARRAY     [i] = otv_alis_hesap_id;
            }
            if ( OTV_SATIS_HESAPLARI_ID_ARRAY[i] EQ 0 ) {
               OTV_SATIS_HESAPLARI_ID_ARRAY    [i] = otv_satis_hesap_id;
            }
        }

        sql_query.PREPARE_UPDATE("e9_sabit_degerler",
                                 "sabit_deger_id",
                                 "ind_kdv_hesap_id_array               , "
                                 "ind_kdv_iade_hesap_id_array          , "
                                 "hes_kdv_hesap_id_array               , "
                                 "hes_kdv_iade_hesap_id_array          , "
                                 "otv_alis_hesap_id_array              , "
                                 "otv_satis_hesap_id_array             , "
                                 "tevkifat_kdv_hesap_id_array            ",
                                 "sabit_deger_id    = :sabit_deger_id " );


        sql_query.SET_VALUE ( ":ind_kdv_hesap_id_array"        , PACK_INT_ARRAY ( IND_KDV_HESAPLARI_ID_ARRAY,      KDV_OTV_ORANLARI_SAYISI )    );
        sql_query.SET_VALUE ( ":ind_kdv_iade_hesap_id_array"   , PACK_INT_ARRAY ( IND_KDV_HESAPLARI_IADE_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI )    );
        sql_query.SET_VALUE ( ":hes_kdv_hesap_id_array"        , PACK_INT_ARRAY ( HES_KDV_HESAPLARI_ID_ARRAY,      KDV_OTV_ORANLARI_SAYISI )    );
        sql_query.SET_VALUE ( ":hes_kdv_iade_hesap_id_array"   , PACK_INT_ARRAY ( HES_KDV_HESAPLARI_IADE_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI )    );
        sql_query.SET_VALUE ( ":otv_alis_hesap_id_array"       , PACK_INT_ARRAY ( OTV_ALIS_HESAPLARI_ID_ARRAY,     KDV_OTV_ORANLARI_SAYISI )    );
        sql_query.SET_VALUE ( ":otv_satis_hesap_id_array"      , PACK_INT_ARRAY ( OTV_SATIS_HESAPLARI_ID_ARRAY,    KDV_OTV_ORANLARI_SAYISI )    );
        sql_query.SET_VALUE ( ":tevkifat_kdv_hesap_id_array"   , PACK_INT_ARRAY ( TEVKIFAT_KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI )    );
        sql_query.SET_VALUE ( ":sabit_deger_id"                , 1                                                                              );

        sql_query.UPDATE();
    }
}

static void UPGRADE_TO_VERSION (QString version);

/**************************************************************************************
                   UPGRADE_TO_VERSION
***************************************************************************************/

static void UPGRADE_TO_VERSION (QString version)
{
    E9_ADD_DEFAULT_BELGELER();

    if ( version < "2.84" ) {
        ADAK_INFO("Veritabanı güncelleştirilemedi.Program veritabanı çok eski (Versiyon 2.32 'den küçük)\n"
                 "Veritabanında ki bilgi kaybını önlemek ve yeni versiyonu kurabilmek için\n"
                 "Adak Teknoloji ile irtibata geçiniz: info@adak.com.tr",NULL,NULL);
        exit(1);
    }
    // if ( version EQ "2.81" ) {
        // E9_V2_81_TO_V2_82_UPGRADE();
    // }

}

/**************************************************************************************
                   E9_CHECK_AND_PREPARE
***************************************************************************************/

void E9_CHECK_AND_PREPARE()
{
    Q_INIT_RESOURCE(e9_icons);

    CHECK_VERSION_UPGRADES ( "program_version","e9_sabit_degerler",&UPGRADE_TO_VERSION );

    E9_SET_DEFAULT_VALUES();

    // E9 yetkileri yukleniyor
    E9_KULLANICI_YETKILERI_STRUCT_DOLDUR( KULLANICI_ID() );

    // Sube unite yukleniyor
    SUBE_SET_CURRENT_SUBE_UNITE();

    //! Raporlarda Firma ismi kullaniliyor.
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "e9_sabit_degerler", "firma_ismi, mali_yil_ilk_tarih", "sabit_deger_id = :sabit_deger_id");
    query.SET_VALUE     ( ":sabit_deger_id", 1 );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

         g_firma_ismi = query.VALUE( 0 ).toString();
         QDate mali_yil_ilk_tarih = QDate::fromString( query.VALUE( 1 ).toString(), "yyyy.MM.dd");
         SET_MALI_YIL_FIRST_DATE ( mali_yil_ilk_tarih );
    }
    //! end
}

/**************************************************************************************
                   E9_GET_TAH_ODM_SEKLI_ENUM
***************************************************************************************/

int E9_GET_TAH_ODM_SEKLI_ENUM(QString tah_odm_sekli)
{
    if ( tah_odm_sekli EQ "Çek" OR tah_odm_sekli EQ "Müşteri Çeki") {
        return ENUM_MUSTERI_CEKI;
    }
    else if ( tah_odm_sekli EQ "Senet" OR tah_odm_sekli EQ "Müşteri Senedi"){
        return ENUM_MUSTERI_SENEDI;
    }
    else if ( tah_odm_sekli EQ "Kendi Çekimiz" ){
        return ENUM_KENDI_CEKIMIZ;
    }
    else if ( tah_odm_sekli EQ "Kendi Senedimiz" ){
        return ENUM_KENDI_SENEDIMIZ;
    }
    else if ( tah_odm_sekli EQ "Nakit" ){
        return ENUM_NAKIT;
    }
    else if ( tah_odm_sekli EQ "Kredi Kartı") {
        return ENUM_KREDI_KARTI;
    }
    else if ( tah_odm_sekli EQ "Havale") {
        return ENUM_HAVALE;
    }
    else if ( tah_odm_sekli EQ "Eft") {
        return ENUM_EFT;
    }

    return -1;
}

/**************************************************************************************
                   GET_ISLEM_TURU_STRING
***************************************************************************************/

QString E9_GET_TAH_ODM_SEKLI_STRING(int tah_odm_sekli , int fis_turu)
{

    switch ( tah_odm_sekli ) {

        case ENUM_MUSTERI_CEKI         :
            if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                return QObject::tr("Çek");
            }
            return QObject::tr ( "Müşteri Çeki" );

        case ENUM_MUSTERI_SENEDI:
            if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI) {
                return QObject::tr("Senet");
            }
            return QObject::tr ( "Müşteri Senedi" );

        case ENUM_KENDI_CEKIMIZ:
            return QObject::tr ( "Kendi Çekimiz" );

        case ENUM_KENDI_SENEDIMIZ:
            return QObject::tr ( "Kendi Senedimiz" );

        case ENUM_NAKIT:
            return QObject::tr ( "Nakit" );

        case ENUM_KREDI_KARTI  :
            return QObject::tr ( "Kredi Kartı" );

        case ENUM_HAVALE:
            return QObject::tr ( "Havale" );

        case ENUM_EFT:
            return QObject::tr ( "Eft" );

        default :
            return "";
    };
}

/**************************************************************************************
                   E9_DISABLE_MUHASEBE_ALAMLARI
***************************************************************************************/

void E9_DISABLE_MUHASEBE_ALAMLARI ()
{
    g_muhasebe_alarmlari_kapalimi = true;
}

/**************************************************************************************
                   E9_ENABLE_MUHASEBE_ALAMLARI
***************************************************************************************/

void E9_ENABLE_MUHASEBE_ALAMLARI ()
{
    g_muhasebe_alarmlari_kapalimi = false;
}

/**************************************************************************************
                   E9_RESTORE_MUHASEBE_ALAMLARI_VALUE
***************************************************************************************/

void E9_RESTORE_MUHASEBE_ALAMLARI_VALUE ()
{
    SQL_QUERY query (DB);

    query.PREPARE_SELECT("e9_sabit_degerler","muhasebe_alarmlari_kapalimi");
    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        if ( query.VALUE(0).toInt() EQ 1 ) {
            g_muhasebe_alarmlari_kapalimi = true;
        }
    }
    g_muhasebe_alarmlari_kapalimi = false;
}

/**************************************************************************************
                   E9_MUHASEBE_ALARMLARI_KAPALIMI
***************************************************************************************/

bool E9_MUHASEBE_ALARMLARI_KAPALIMI ()
{
    return g_muhasebe_alarmlari_kapalimi;
}


/**************************************************************************************
                   E9_SUBE_ENT_HESAPLARINI_GUNCELLE
***************************************************************************************/

void E9_SUBE_ENT_HESAPLARINI_GUNCELLE( int p_sube_id , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }


    SQL_QUERY query(USE_DB);
    SQL_QUERY sql_query ( USE_DB );

    int sube_id = p_sube_id;

    query.PREPARE_SELECT("sub_subeler",
                         "sube_prs_gdrlr_hsp_id                         , "
                         "sube_yurtici_satis_hsp_id                     , "
                         "sube_hizmet_urt_maliyet_hsp_id                , "
                         "sube_sarf_malzemeleri_hsp_id                  , "
                         "sube_port_cekler_hsp_id                       , "
                         "sube_tah_cekler_hsp_id                        , "
                         "sube_tem_cekler_hsp_id                        , "
                         "sube_kar_cekler_hsp_id                        , "
                         "sube_port_snt_hsp_id                          , "
                         "sube_tah_snt_hsp_id                           , "
                         "sube_tem_snt_hsp_id                           , "
                         "sube_pro_snt_hsp_id                           , "
                         "sube_port_snt_ileri_trh_hsp_id                , "
                         "sube_tah_snt_ileri_trh_hsp_id                 , "
                         "sube_tem_snt_ileri_trh_hsp_id                 , "
                         "sube_borc_snt_hsp_id                          , "
                         "sube_borc_snt_ileri_trh_hsp_id                , "
                         "sube_sup_alacaklar_hsp_id                     , "
                         "sube_tah_edlmyn_cekler_hsp_id                 , "
                         "sube_tah_edlmyn_snt_hsp_id                    , "
                         "sube_imalattan_giris_hsp_id                   , "
                         "kasa_ent_hesap_id                             , "
                         "cari_alici_ent_hesap_id                       , "
                         "cari_satici_ent_hesap_id                      , "
                         "banka_ent_hesap_id                            , "
                         "ver_cek_ent_hesap_id                          , "
                         "odeme_emirleri_ent_hesap_id                   , "
                         "pos_ent_hesap_id                              , "
                         "pos_kom_gideri_ent_hesap_id                   , "
                         "kk_ent_hesap_id                               , "
                         "prk_gider_ent_hesap_id                        , "
                         "urunler_ent_hesap_id                          , "
                         "hizmetler_ent_hesap_id                        , "
                         "demirbaslar_ent_hesap_id                      , "
                         "prs_avanslari_ent_hesap_id                    , "
                         "personel_borclari_ent_hesap_id                , "
                         "prs_damga_vergisi_ent_hesap_id                , "
                         "personel_vergi_ent_hesap_id                   , "
                         "prk_gelir_ent_hesap_id                        , "
                         "prs_ssk_isci_payi_hesap_id                    , "
                         "prs_ssk_isveren_payi_hesap_id                 , "
                         "prs_issizlik_sgk_isci_hsp_id                  , "
                         "prs_issizlik_sgk_isv_hsp_id                   , "
                         "asg_gecim_ind_hesap_id                        , "
                         "isveren_payi_giderleri_hps_id                 , "
                         "bir_amortismanlar_hsp_id                      , "
                         "amor_giderleri_hsp_id                         , "
                         "sgdp_prim_yuzde_isc_hsp_id                    , "
                         "sgdp_prim_yuzde_isv_hsp_id                    , "
                         "kidem_tazminati_hesap_id                      , "
                         "ihbar_tazminati_hesap_id                      , "
                         "dmr_satis_kar_ent_hesap_id                    , "
                         "dmr_satis_zarar_ent_hesap_id                  , "
                         "sendika_odeme_hesap_id                        , "
                         "smmm_alinan_hizmet_hesap_id                   , "
                         "smmm_gelir_vergisi_hesap_id                   , "
                         "smmm_satilan_hizmet_hesap_id                  , "
                         "isy_5510_nolu_kanun_hesap_id                  , "
                         "satistan_iade_hesap_id                        , "
                         "ortaklara_borclar_hesap_id                      ",
                         "sube_id =:sube_id ");
    query.SET_VALUE(":sube_id" , sube_id );

    if ( query.SELECT() NE 0 ) {

        query.NEXT();
        int prs_giderler_hesap_id = query.VALUE("sube_prs_gdrlr_hsp_id").toInt();
        if ( prs_giderler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( prs_giderler_hesap_id ) EQ HAYIR ) {
            prs_giderler_hesap_id = MUH_GET_TALI_HESAP_ID("740");
        }


        int yurtici_satislar_hesap_id = query.VALUE("sube_yurtici_satis_hsp_id").toInt();
        if ( yurtici_satislar_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( yurtici_satislar_hesap_id ) EQ HAYIR ) {
            yurtici_satislar_hesap_id = MUH_GET_TALI_HESAP_ID("600");
        }

        int hizmet_uretim_maliyeti_hesap_id = query.VALUE("sube_hizmet_urt_maliyet_hsp_id").toInt();
        if ( hizmet_uretim_maliyeti_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( hizmet_uretim_maliyeti_hesap_id ) EQ HAYIR ) {
            hizmet_uretim_maliyeti_hesap_id = MUH_GET_TALI_HESAP_ID("740");
        }

        int sarf_malzemeleri_hesap_id = query.VALUE("sube_sarf_malzemeleri_hsp_id").toInt();
        if ( sarf_malzemeleri_hesap_id EQ 0  OR MUH_HESABI_TALI_HESAP_MI( sarf_malzemeleri_hesap_id ) EQ HAYIR) {
            sarf_malzemeleri_hesap_id = MUH_GET_TALI_HESAP_ID("710");
        }

        int port_cekler_hesap_id = query.VALUE("sube_port_cekler_hsp_id").toInt();
        if ( port_cekler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( port_cekler_hesap_id ) EQ HAYIR ) {
            port_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("101");
        }

        int tah_cekler_hesap_id = query.VALUE("sube_tah_cekler_hsp_id").toInt();
        if ( tah_cekler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( tah_cekler_hesap_id ) EQ HAYIR ) {
            tah_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("101");
        }

        int tem_cekler_hesap_id = query.VALUE("sube_tem_cekler_hsp_id").toInt();
        if ( tem_cekler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( tem_cekler_hesap_id ) EQ HAYIR ) {
            tem_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("101");
        }

        int kar_cekler_hesap_id = query.VALUE("sube_kar_cekler_hsp_id").toInt();
        if ( kar_cekler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI(  kar_cekler_hesap_id ) EQ HAYIR ) {
            kar_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("101");
        }

        int port_senetler_hesap_id = query.VALUE("sube_port_snt_hsp_id").toInt();
        if ( port_senetler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( port_senetler_hesap_id ) EQ HAYIR ) {
            port_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        int tah_senetler_hesap_id = query.VALUE("sube_tah_snt_hsp_id").toInt();
        if ( tah_senetler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( tah_senetler_hesap_id ) EQ HAYIR ) {
            tah_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        int tem_senetler_hesap_id = query.VALUE("sube_tem_snt_hsp_id").toInt();
        if ( tem_senetler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( tem_senetler_hesap_id ) EQ HAYIR ) {
            tem_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        int pro_senetler_hesap_id = query.VALUE("sube_pro_snt_hsp_id").toInt();
        if ( pro_senetler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( pro_senetler_hesap_id ) EQ HAYIR ) {
            pro_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        int port_senetler_ileri_tarih_hesap_id = query.VALUE("sube_port_snt_ileri_trh_hsp_id").toInt();
        if ( port_senetler_ileri_tarih_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( port_senetler_ileri_tarih_hesap_id ) EQ HAYIR ) {
            port_senetler_ileri_tarih_hesap_id = MUH_GET_TALI_HESAP_ID("221");
        }
        int tah_senetler_ileri_tarih_hesap_id = query.VALUE("sube_tah_snt_ileri_trh_hsp_id").toInt();
        if ( tah_senetler_ileri_tarih_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( tah_senetler_ileri_tarih_hesap_id ) EQ HAYIR ) {
            tah_senetler_ileri_tarih_hesap_id = MUH_GET_TALI_HESAP_ID("221");
        }
        int tem_senetler_ileri_tarih_hesap_id = query.VALUE("sube_tem_snt_ileri_trh_hsp_id").toInt();
        if ( tem_senetler_ileri_tarih_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( tem_senetler_ileri_tarih_hesap_id ) EQ HAYIR ) {
            tem_senetler_ileri_tarih_hesap_id = MUH_GET_TALI_HESAP_ID("221");
        }

        int borc_senetleri_hesap_id = query.VALUE("sube_borc_snt_hsp_id").toInt();
        if ( borc_senetleri_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( borc_senetleri_hesap_id ) EQ HAYIR ) {
            borc_senetleri_hesap_id = MUH_GET_TALI_HESAP_ID("321");
        }
        int borc_senetleri_ileri_tarih_hesap_id = query.VALUE("sube_borc_snt_ileri_trh_hsp_id").toInt();
        if ( borc_senetleri_ileri_tarih_hesap_id EQ 0  OR MUH_HESABI_TALI_HESAP_MI( borc_senetleri_ileri_tarih_hesap_id ) EQ HAYIR ) {
            borc_senetleri_ileri_tarih_hesap_id = MUH_GET_TALI_HESAP_ID("421");
        }
        int sup_alacaklar_hesap_id = query.VALUE("sube_sup_alacaklar_hsp_id").toInt();
        if ( sup_alacaklar_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( sup_alacaklar_hesap_id ) EQ HAYIR ) {
            sup_alacaklar_hesap_id = MUH_GET_TALI_HESAP_ID("128");
        }

        int tah_edilemeyen_cekler_hesap_id = query.VALUE("sube_tah_edlmyn_cekler_hsp_id").toInt();
        if ( tah_edilemeyen_cekler_hesap_id EQ 0  OR MUH_HESABI_TALI_HESAP_MI( tah_edilemeyen_cekler_hesap_id ) EQ HAYIR ) {
            tah_edilemeyen_cekler_hesap_id = MUH_GET_TALI_HESAP_ID("101");
        }

        int tah_edilemeyen_senetler_hesap_id = query.VALUE("sube_tah_edlmyn_snt_hsp_id").toInt();
        if ( tah_edilemeyen_senetler_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( tah_edilemeyen_senetler_hesap_id ) EQ HAYIR ) {
            tah_edilemeyen_senetler_hesap_id = MUH_GET_TALI_HESAP_ID("121");
        }

        int sube_imalattan_giris_hsp_id = query.VALUE("sube_imalattan_giris_hsp_id").toInt();
        if ( sube_imalattan_giris_hsp_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( sube_imalattan_giris_hsp_id ) EQ HAYIR ) {
            sube_imalattan_giris_hsp_id = MUH_GET_TALI_HESAP_ID("150");
        }

        int kasa_ent_hesap_id = query.VALUE("kasa_ent_hesap_id").toInt();
        if ( kasa_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( kasa_ent_hesap_id ) EQ HAYIR ) {
            kasa_ent_hesap_id = MUH_GET_TALI_HESAP_ID("100");
        }

        int cari_alici_ent_hesap_id = query.VALUE("cari_alici_ent_hesap_id").toInt();
        if ( cari_alici_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( cari_alici_ent_hesap_id ) EQ HAYIR ) {
            cari_alici_ent_hesap_id = MUH_GET_TALI_HESAP_ID("120");
        }

        int cari_satici_ent_hesap_id = query.VALUE("cari_satici_ent_hesap_id").toInt();
        if ( cari_satici_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( cari_satici_ent_hesap_id ) EQ HAYIR ) {
            cari_satici_ent_hesap_id = MUH_GET_TALI_HESAP_ID("320");
        }

        int banka_ent_hesap_id = query.VALUE("banka_ent_hesap_id").toInt();
        if ( banka_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( banka_ent_hesap_id ) EQ HAYIR ) {
            banka_ent_hesap_id = MUH_GET_TALI_HESAP_ID("102");
        }

        int ver_cekler_ent_hesap_id = query.VALUE("ver_cek_ent_hesap_id").toInt();
        if ( ver_cekler_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( ver_cekler_ent_hesap_id ) EQ HAYIR ) {
            ver_cekler_ent_hesap_id = MUH_GET_TALI_HESAP_ID("103");
        }

        int odm_emirleri_hsp_id = query.VALUE("odeme_emirleri_ent_hesap_id").toInt();
        if ( odm_emirleri_hsp_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( odm_emirleri_hsp_id ) EQ HAYIR ) {
            odm_emirleri_hsp_id = MUH_GET_TALI_HESAP_ID("103");
        }

        int pos_ent_hesap_id = query.VALUE("pos_ent_hesap_id").toInt();
        if ( pos_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( pos_ent_hesap_id ) EQ HAYIR ) {
            pos_ent_hesap_id = MUH_GET_TALI_HESAP_ID("108");
        }

        int pos_kom_gideri_ent_hesap_id = query.VALUE("pos_kom_gideri_ent_hesap_id").toInt();
        if ( pos_kom_gideri_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( pos_kom_gideri_ent_hesap_id ) EQ HAYIR ) {
            pos_kom_gideri_ent_hesap_id = MUH_GET_TALI_HESAP_ID("653");
        }

        int kk_ent_hesap_id = query.VALUE("kk_ent_hesap_id").toInt();
        if ( kk_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( kk_ent_hesap_id ) EQ HAYIR ) {
            kk_ent_hesap_id = MUH_GET_TALI_HESAP_ID("300");
        }

        int prk_giderler_hesap_id = query.VALUE("prk_gider_ent_hesap_id").toInt();
        if ( prk_giderler_hesap_id EQ 0  OR MUH_HESABI_TALI_HESAP_MI( prk_giderler_hesap_id ) EQ HAYIR ) {
            prk_giderler_hesap_id = MUH_GET_TALI_HESAP_ID("770");
        }

        int urunler_ent_hesap_id = query.VALUE("urunler_ent_hesap_id").toInt();
        if ( urunler_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( urunler_ent_hesap_id ) EQ HAYIR ) {
            urunler_ent_hesap_id = MUH_GET_TALI_HESAP_ID("153");
        }

        int hizmetler_ent_hesap_id = query.VALUE("hizmetler_ent_hesap_id").toInt();
        if ( hizmetler_ent_hesap_id EQ 0  OR MUH_HESABI_TALI_HESAP_MI( hizmetler_ent_hesap_id ) EQ HAYIR) {
            hizmetler_ent_hesap_id = MUH_GET_TALI_HESAP_ID("770");
        }

        int demirbaslar_hesap_id = query.VALUE("demirbaslar_ent_hesap_id").toInt();
        if ( demirbaslar_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( demirbaslar_hesap_id ) EQ HAYIR ) {
            demirbaslar_hesap_id = MUH_GET_TALI_HESAP_ID("255");
        }

        int bir_amortismanlar_hsp_id = query.VALUE("bir_amortismanlar_hsp_id").toInt();
        if ( bir_amortismanlar_hsp_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( bir_amortismanlar_hsp_id ) EQ HAYIR ) {
            bir_amortismanlar_hsp_id = MUH_GET_TALI_HESAP_ID("257");
        }

        int amortisman_giderleri_hsp_id = query.VALUE("amor_giderleri_hsp_id").toInt();
        if ( amortisman_giderleri_hsp_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( amortisman_giderleri_hsp_id ) EQ HAYIR ) {
            amortisman_giderleri_hsp_id = MUH_GET_TALI_HESAP_ID("770");
        }

        int prs_avanslar_ent_hesap_id = query.VALUE("prs_avanslari_ent_hesap_id").toInt();
        if ( prs_avanslar_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( prs_avanslar_ent_hesap_id ) EQ HAYIR ) {
            prs_avanslar_ent_hesap_id = MUH_GET_TALI_HESAP_ID("196");
        }

        int prs_borclar_ent_hesap_id = query.VALUE("personel_borclari_ent_hesap_id").toInt();
        if ( prs_borclar_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI(  prs_borclar_ent_hesap_id ) EQ HAYIR ) {
            prs_borclar_ent_hesap_id = MUH_GET_TALI_HESAP_ID("335");
        }

        int prs_sosyal_ent_hesap_id = query.VALUE("prs_damga_vergisi_ent_hesap_id").toInt();
        if ( prs_sosyal_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( prs_sosyal_ent_hesap_id ) EQ HAYIR ) {
            prs_sosyal_ent_hesap_id = MUH_GET_TALI_HESAP_ID("361");
        }

        int prs_vergi_ent_hesap_id = query.VALUE("personel_vergi_ent_hesap_id").toInt();
        if ( prs_vergi_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( prs_vergi_ent_hesap_id ) EQ HAYIR ) {
            prs_vergi_ent_hesap_id = MUH_GET_TALI_HESAP_ID("360");
        }

        int prk_gelirler_ent_hesap_id = query.VALUE("prk_gelir_ent_hesap_id").toInt();
        if ( prk_gelirler_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( prk_gelirler_ent_hesap_id ) EQ HAYIR ) {
            prk_gelirler_ent_hesap_id = MUH_GET_TALI_HESAP_ID("600");
        }

        int prs_ssk_isci_payi_hesap_id = query.VALUE("prs_ssk_isci_payi_hesap_id").toInt();
        if ( prs_ssk_isci_payi_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( prs_ssk_isci_payi_hesap_id ) EQ HAYIR ) {
            prs_ssk_isci_payi_hesap_id = MUH_GET_TALI_HESAP_ID("361");
        }

        int prs_ssk_isveren_payi_hesap_id = query.VALUE("prs_ssk_isveren_payi_hesap_id").toInt();

        if ( prs_ssk_isveren_payi_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( prs_ssk_isveren_payi_hesap_id ) EQ HAYIR ) {
            prs_ssk_isveren_payi_hesap_id = MUH_GET_TALI_HESAP_ID("361");
        }

        int prs_issizlik_sgk_isci_hsp_id = query.VALUE("prs_issizlik_sgk_isci_hsp_id").toInt();
        if ( prs_issizlik_sgk_isci_hsp_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( prs_issizlik_sgk_isci_hsp_id ) EQ HAYIR ) {
            prs_issizlik_sgk_isci_hsp_id = MUH_GET_TALI_HESAP_ID("361");
        }

        int prs_issizlik_sgk_isv_hsp_id = query.VALUE("prs_issizlik_sgk_isv_hsp_id").toInt();
        if ( prs_issizlik_sgk_isv_hsp_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( prs_issizlik_sgk_isv_hsp_id ) EQ HAYIR ) {
            prs_issizlik_sgk_isv_hsp_id = MUH_GET_TALI_HESAP_ID("361");
        }

        int asg_gecim_ind_hesap_id = query.VALUE("asg_gecim_ind_hesap_id").toInt();

        if ( asg_gecim_ind_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( asg_gecim_ind_hesap_id ) EQ HAYIR ) {
            asg_gecim_ind_hesap_id = MUH_GET_TALI_HESAP_ID("360");
        }

        int isveren_payi_giderleri_hps_id = query.VALUE("isveren_payi_giderleri_hps_id").toInt();
        if ( isveren_payi_giderleri_hps_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( isveren_payi_giderleri_hps_id ) EQ HAYIR ) {
            isveren_payi_giderleri_hps_id = MUH_GET_TALI_HESAP_ID("770");
        }

        int sgdp_prim_yuzde_isc_hsp_id = query.VALUE("sgdp_prim_yuzde_isc_hsp_id").toInt();
        if ( sgdp_prim_yuzde_isc_hsp_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( sgdp_prim_yuzde_isc_hsp_id ) EQ HAYIR ) {
            sgdp_prim_yuzde_isc_hsp_id = MUH_GET_TALI_HESAP_ID("361");
        }

        int sgdp_prim_yuzde_isv_hsp_id = query.VALUE("sgdp_prim_yuzde_isv_hsp_id").toInt();
        if ( sgdp_prim_yuzde_isv_hsp_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( sgdp_prim_yuzde_isv_hsp_id ) EQ HAYIR ) {
            sgdp_prim_yuzde_isv_hsp_id = MUH_GET_TALI_HESAP_ID("361");
        }

        int kidem_tazminati_hesap_id = query.VALUE("kidem_tazminati_hesap_id").toInt();
        if ( kidem_tazminati_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( kidem_tazminati_hesap_id ) EQ HAYIR ) {
            kidem_tazminati_hesap_id = MUH_GET_TALI_HESAP_ID("770");
        }

        int ihbar_tazminati_hesap_id = query.VALUE("ihbar_tazminati_hesap_id").toInt();

        if ( ihbar_tazminati_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( ihbar_tazminati_hesap_id ) EQ HAYIR ) {
            ihbar_tazminati_hesap_id = MUH_GET_TALI_HESAP_ID("770");
        }

        int dmr_satis_kari_ent_hesap_id = query.VALUE("dmr_satis_kar_ent_hesap_id").toInt();
        if ( dmr_satis_kari_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( dmr_satis_kari_ent_hesap_id ) EQ HAYIR ) {
            dmr_satis_kari_ent_hesap_id = MUH_GET_TALI_HESAP_ID("679");
        }

        int dmr_satis_zarari_ent_hesap_id = query.VALUE("dmr_satis_zarar_ent_hesap_id").toInt();

        if ( dmr_satis_zarari_ent_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI(  dmr_satis_zarari_ent_hesap_id ) EQ HAYIR ) {
            dmr_satis_zarari_ent_hesap_id = MUH_GET_TALI_HESAP_ID("689");
        }

        int sendika_odeme_hesap_id = query.VALUE("sendika_odeme_hesap_id").toInt();
        if ( sendika_odeme_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( sendika_odeme_hesap_id ) EQ HAYIR ) {
            sendika_odeme_hesap_id = MUH_GET_TALI_HESAP_ID("770");
        }

        int smmm_alinan_hizmet_hesap_id = query.VALUE("smmm_alinan_hizmet_hesap_id").toInt();
        if ( smmm_alinan_hizmet_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( smmm_alinan_hizmet_hesap_id ) EQ HAYIR ) {
            smmm_alinan_hizmet_hesap_id = MUH_GET_TALI_HESAP_ID("770");
        }

        int smmm_gelir_vergisi_hesap_id = query.VALUE("smmm_gelir_vergisi_hesap_id").toInt();
        if ( smmm_gelir_vergisi_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( smmm_gelir_vergisi_hesap_id ) EQ HAYIR ) {
            smmm_gelir_vergisi_hesap_id = MUH_GET_TALI_HESAP_ID("360");
        }

        int smmm_satilan_hizmet_hesap_id = query.VALUE("smmm_satilan_hizmet_hesap_id").toInt();
        if ( smmm_satilan_hizmet_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( smmm_satilan_hizmet_hesap_id ) EQ HAYIR ) {
            smmm_satilan_hizmet_hesap_id = MUH_GET_TALI_HESAP_ID("600");
        }

        int isy_5510_nolu_kanun_hesap_id = query.VALUE("isy_5510_nolu_kanun_hesap_id").toInt();
        if ( isy_5510_nolu_kanun_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( isy_5510_nolu_kanun_hesap_id ) EQ HAYIR ) {
            isy_5510_nolu_kanun_hesap_id = MUH_GET_TALI_HESAP_ID("602");
        }

        int satistan_iade_hesap_id = query.VALUE("satistan_iade_hesap_id").toInt();
        if ( satistan_iade_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( satistan_iade_hesap_id ) EQ HAYIR ) {
            satistan_iade_hesap_id = MUH_GET_TALI_HESAP_ID("610");
        }

        int ortaklara_borclar_hesap_id = query.VALUE("ortaklara_borclar_hesap_id").toInt();
        if ( ortaklara_borclar_hesap_id EQ 0 OR MUH_HESABI_TALI_HESAP_MI( ortaklara_borclar_hesap_id ) EQ HAYIR ) {
            ortaklara_borclar_hesap_id = MUH_GET_TALI_HESAP_ID("331");
        }

        sql_query.PREPARE_UPDATE ( "sub_subeler ",
                                   "sube_id",
                                   "sube_port_cekler_hsp_id                 , "
                                   "sube_tah_cekler_hsp_id                  , "
                                   "sube_tem_cekler_hsp_id                  , "
                                   "sube_kar_cekler_hsp_id                  , "
                                   "sube_port_snt_hsp_id                    , "
                                   "sube_tah_snt_hsp_id                     , "
                                   "sube_tem_snt_hsp_id                     , "
                                   "sube_pro_snt_hsp_id                     , "
                                   "sube_port_snt_ileri_trh_hsp_id          , "
                                   "sube_tah_snt_ileri_trh_hsp_id           , "
                                   "sube_tem_snt_ileri_trh_hsp_id           , "
                                   "sube_borc_snt_hsp_id                    , "
                                   "sube_borc_snt_ileri_trh_hsp_id          , "
                                   "sube_sup_alacaklar_hsp_id               , "
                                   "sube_tah_edlmyn_cekler_hsp_id           , "
                                   "sube_tah_edlmyn_snt_hsp_id              , "
                                   "sube_yurtici_satis_hsp_id               , "
                                   "sube_hizmet_urt_maliyet_hsp_id          , "
                                   "sube_prs_gdrlr_hsp_id                   , "
                                   "sube_sarf_malzemeleri_hsp_id            , "
                                   "sube_imalattan_giris_hsp_id             , "
                                   "kasa_ent_hesap_id                       , "
                                   "cari_alici_ent_hesap_id                 , "
                                   "cari_satici_ent_hesap_id                , "
                                   "banka_ent_hesap_id                      , "
                                   "ver_cek_ent_hesap_id                    , "
                                   "odeme_emirleri_ent_hesap_id             , "
                                   "pos_ent_hesap_id                        , "
                                   "kk_ent_hesap_id                         , "
                                   "prk_gider_ent_hesap_id                  , "
                                   "urunler_ent_hesap_id                    , "
                                   "hizmetler_ent_hesap_id                  , "
                                   "demirbaslar_ent_hesap_id                , "
                                   "pos_kom_gideri_ent_hesap_id             , "
                                   "prs_avanslari_ent_hesap_id              , "
                                   "personel_borclari_ent_hesap_id          , "
                                   "prs_damga_vergisi_ent_hesap_id          , "
                                   "personel_vergi_ent_hesap_id             , "
                                   "prk_gelir_ent_hesap_id                  , "
                                   "prs_ssk_isci_payi_hesap_id              , "
                                   "prs_ssk_isveren_payi_hesap_id           , "
                                   "prs_issizlik_sgk_isci_hsp_id            , "
                                   "prs_issizlik_sgk_isv_hsp_id             , "
                                   "asg_gecim_ind_hesap_id                  , "
                                   "isveren_payi_giderleri_hps_id           , "
                                   "bir_amortismanlar_hsp_id                , "
                                   "amor_giderleri_hsp_id                   , "
                                   "sgdp_prim_yuzde_isc_hsp_id              , "
                                   "sgdp_prim_yuzde_isv_hsp_id              , "
                                   "kidem_tazminati_hesap_id                , "
                                   "ihbar_tazminati_hesap_id                , "
                                   "dmr_satis_kar_ent_hesap_id              , "
                                   "dmr_satis_zarar_ent_hesap_id            , "
                                   "sendika_odeme_hesap_id                  , "
                                   "smmm_alinan_hizmet_hesap_id             , "
                                   "smmm_gelir_vergisi_hesap_id             , "
                                   "smmm_satilan_hizmet_hesap_id            , "
                                   "isy_5510_nolu_kanun_hesap_id            , "
                                   "satistan_iade_hesap_id                  , "
                                   "ortaklara_borclar_hesap_id               ",

                                   "sube_id = :sube_id " );

        sql_query.SET_VALUE     ( ":sube_port_cekler_hsp_id"                    , port_cekler_hesap_id                    );
        sql_query.SET_VALUE     ( ":sube_tah_cekler_hsp_id"                     , tah_cekler_hesap_id                     );
        sql_query.SET_VALUE     ( ":sube_tem_cekler_hsp_id"                     , tem_cekler_hesap_id                     );
        sql_query.SET_VALUE     ( ":sube_kar_cekler_hsp_id"                     , kar_cekler_hesap_id                     );
        sql_query.SET_VALUE     ( ":sube_port_snt_hsp_id"                       , port_senetler_hesap_id                  );
        sql_query.SET_VALUE     ( ":sube_tah_snt_hsp_id"                        , tah_senetler_hesap_id                   );
        sql_query.SET_VALUE     ( ":sube_tem_snt_hsp_id"                        , tem_senetler_hesap_id                   );
        sql_query.SET_VALUE     ( ":sube_pro_snt_hsp_id"                        , pro_senetler_hesap_id                   );
        sql_query.SET_VALUE     ( ":sube_port_snt_ileri_trh_hsp_id"             , port_senetler_ileri_tarih_hesap_id      );
        sql_query.SET_VALUE     ( ":sube_tah_snt_ileri_trh_hsp_id"              , tah_senetler_ileri_tarih_hesap_id       );
        sql_query.SET_VALUE     ( ":sube_tem_snt_ileri_trh_hsp_id"              , tem_senetler_ileri_tarih_hesap_id       );
        sql_query.SET_VALUE     ( ":sube_borc_snt_hsp_id"                       , borc_senetleri_hesap_id                 );
        sql_query.SET_VALUE     ( ":sube_borc_snt_ileri_trh_hsp_id"             , borc_senetleri_ileri_tarih_hesap_id     );
        sql_query.SET_VALUE     ( ":sube_sup_alacaklar_hsp_id"                  , sup_alacaklar_hesap_id                  );
        sql_query.SET_VALUE     ( ":sube_tah_edlmyn_cekler_hsp_id"              , tah_edilemeyen_cekler_hesap_id          );
        sql_query.SET_VALUE     ( ":sube_tah_edlmyn_snt_hsp_id"                 , tah_edilemeyen_senetler_hesap_id        );
        sql_query.SET_VALUE     ( ":sube_yurtici_satis_hsp_id"                  , yurtici_satislar_hesap_id               );
        sql_query.SET_VALUE     ( ":sube_hizmet_urt_maliyet_hsp_id"             , hizmet_uretim_maliyeti_hesap_id         );
        sql_query.SET_VALUE     ( ":sube_prs_gdrlr_hsp_id"                      , prs_giderler_hesap_id                   );
        sql_query.SET_VALUE     ( ":sube_sarf_malzemeleri_hsp_id"               , sarf_malzemeleri_hesap_id               );
        sql_query.SET_VALUE     ( ":sube_imalattan_giris_hsp_id"                , sube_imalattan_giris_hsp_id             );
        sql_query.SET_VALUE     ( ":kasa_ent_hesap_id"                          , kasa_ent_hesap_id                       );
        sql_query.SET_VALUE     ( ":cari_alici_ent_hesap_id"                    , cari_alici_ent_hesap_id                 );
        sql_query.SET_VALUE     ( ":cari_satici_ent_hesap_id"                   , cari_satici_ent_hesap_id                );
        sql_query.SET_VALUE     ( ":banka_ent_hesap_id"                         , banka_ent_hesap_id                      );
        sql_query.SET_VALUE     ( ":ver_cek_ent_hesap_id"                       , ver_cekler_ent_hesap_id                 );
        sql_query.SET_VALUE     ( ":odeme_emirleri_ent_hesap_id"                , odm_emirleri_hsp_id                     );
        sql_query.SET_VALUE     ( ":pos_ent_hesap_id"                           , pos_ent_hesap_id                        );
        sql_query.SET_VALUE     ( ":kk_ent_hesap_id"                            , kk_ent_hesap_id                         );
        sql_query.SET_VALUE     ( ":prk_gider_ent_hesap_id"                     , prk_giderler_hesap_id                   );
        sql_query.SET_VALUE     ( ":urunler_ent_hesap_id"                       , urunler_ent_hesap_id                    );
        sql_query.SET_VALUE     ( ":hizmetler_ent_hesap_id"                     , hizmetler_ent_hesap_id                  );
        sql_query.SET_VALUE     ( ":demirbaslar_ent_hesap_id"                   , demirbaslar_hesap_id                    );
        sql_query.SET_VALUE     ( ":pos_kom_gideri_ent_hesap_id"                , pos_kom_gideri_ent_hesap_id             );
        sql_query.SET_VALUE     ( ":prs_avanslari_ent_hesap_id"                 , prs_avanslar_ent_hesap_id               );
        sql_query.SET_VALUE     ( ":personel_borclari_ent_hesap_id"             , prs_borclar_ent_hesap_id                );
        sql_query.SET_VALUE     ( ":prs_damga_vergisi_ent_hesap_id"             , prs_sosyal_ent_hesap_id                 );
        sql_query.SET_VALUE     ( ":personel_vergi_ent_hesap_id"                , prs_vergi_ent_hesap_id                  );
        sql_query.SET_VALUE     ( ":prk_gelir_ent_hesap_id"                     , prk_gelirler_ent_hesap_id               );
        sql_query.SET_VALUE     ( ":prs_ssk_isci_payi_hesap_id"                 , prs_ssk_isci_payi_hesap_id              );
        sql_query.SET_VALUE     ( ":prs_ssk_isveren_payi_hesap_id"              , prs_ssk_isveren_payi_hesap_id           );
        sql_query.SET_VALUE     ( ":prs_issizlik_sgk_isci_hsp_id"               , prs_issizlik_sgk_isci_hsp_id            );
        sql_query.SET_VALUE     ( ":prs_issizlik_sgk_isv_hsp_id"                , prs_issizlik_sgk_isv_hsp_id             );
        sql_query.SET_VALUE     ( ":asg_gecim_ind_hesap_id"                     , asg_gecim_ind_hesap_id                  );
        sql_query.SET_VALUE     ( ":isveren_payi_giderleri_hps_id"              , isveren_payi_giderleri_hps_id           );
        sql_query.SET_VALUE     ( ":bir_amortismanlar_hsp_id"                   , bir_amortismanlar_hsp_id                );
        sql_query.SET_VALUE     ( ":amor_giderleri_hsp_id"                      , amortisman_giderleri_hsp_id             );
        sql_query.SET_VALUE     ( ":sgdp_prim_yuzde_isc_hsp_id"                 , sgdp_prim_yuzde_isc_hsp_id              );
        sql_query.SET_VALUE     ( ":sgdp_prim_yuzde_isv_hsp_id"                 , sgdp_prim_yuzde_isv_hsp_id              );
        sql_query.SET_VALUE     ( ":kidem_tazminati_hesap_id"                   , kidem_tazminati_hesap_id                );
        sql_query.SET_VALUE     ( ":ihbar_tazminati_hesap_id"                   , ihbar_tazminati_hesap_id                );
        sql_query.SET_VALUE     ( ":dmr_satis_kar_ent_hesap_id"                 , dmr_satis_kari_ent_hesap_id             );
        sql_query.SET_VALUE     ( ":dmr_satis_zarar_ent_hesap_id"               , dmr_satis_zarari_ent_hesap_id           );
        sql_query.SET_VALUE     ( ":sendika_odeme_hesap_id"                     , sendika_odeme_hesap_id                  );
        sql_query.SET_VALUE     ( ":smmm_alinan_hizmet_hesap_id"                , smmm_alinan_hizmet_hesap_id             );
        sql_query.SET_VALUE     ( ":smmm_gelir_vergisi_hesap_id"                , smmm_gelir_vergisi_hesap_id             );
        sql_query.SET_VALUE     ( ":smmm_satilan_hizmet_hesap_id"               , smmm_satilan_hizmet_hesap_id            );
        sql_query.SET_VALUE     ( ":isy_5510_nolu_kanun_hesap_id"               , isy_5510_nolu_kanun_hesap_id            );
        sql_query.SET_VALUE     ( ":satistan_iade_hesap_id"                     , satistan_iade_hesap_id                  );
        sql_query.SET_VALUE     ( ":ortaklara_borclar_hesap_id"                 , ortaklara_borclar_hesap_id              );
        sql_query.SET_VALUE     ( ":sube_id"                                    , sube_id                                 );

        sql_query.UPDATE();
    }
}

/**************************************************************************************
                   E9_CLEAR_KULLANICI_YETKILERI_STRUCT
***************************************************************************************/

void E9_CLEAR_KULLANICI_YETKILERI_STRUCT( E9_KULLANICI_YETKILERI_STRUCT * P_E9_KULLANICI_YETKILERI )
{

    P_E9_KULLANICI_YETKILERI->sis_blg_formuna_girebilir_mi                                 = 0;
    P_E9_KULLANICI_YETKILERI->sis_kul_formuna_girebilir_mi                                 = 0;
    P_E9_KULLANICI_YETKILERI->sis_veritabani_formuna_girebilir_mi                          = 0;
    P_E9_KULLANICI_YETKILERI->sis_yonetim_log_inceleme_yetkisi                             = 0;
    P_E9_KULLANICI_YETKILERI->sis_mail_srv_tanimlayabilirmi                                = 0;
    P_E9_KULLANICI_YETKILERI->sis_sms_srv_tanimlayabilirmi                                 = 0;
    P_E9_KULLANICI_YETKILERI->ynt_e9_yetkilendirme_yapabilir_mi                            = 0;
    P_E9_KULLANICI_YETKILERI->ynt_e9_onar_yetkisi                                          = 0;
    P_E9_KULLANICI_YETKILERI->ynt_e9_devir_islemi_yapabilir_mi                             = 0;
    P_E9_KULLANICI_YETKILERI->ynt_e9_log_inceleme_yetkisi                                  = 0;
    P_E9_KULLANICI_YETKILERI->ynt_e9_sabit_degerler_guncelleme_yetkisi                     = 0;
    P_E9_KULLANICI_YETKILERI->ynt_e9_belge_tasarlama_yetkisi                               = 0;
    P_E9_KULLANICI_YETKILERI->ynt_e9_ent_bilgilerini_degistirme_yetkisi                    = 0;
    P_E9_KULLANICI_YETKILERI->ynt_e9_email_log_inceleme_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->ynt_e9_urettigi_fisleri_grbilir_mi                           = 0;

    //ADRES

    P_E9_KULLANICI_YETKILERI->adr_adres_karti_tanimlama_yetkisi                         = 0;
    P_E9_KULLANICI_YETKILERI->adr_not_defteri_inceleme_yetkisi                          = 0;
    P_E9_KULLANICI_YETKILERI->adr_etiket_yazdirma_yetkisi                               = 0;
    P_E9_KULLANICI_YETKILERI->adr_fihrist_raporu_alma_yetkisi                           = 0;
    P_E9_KULLANICI_YETKILERI->adr_toplu_mail_gonderme_yetkisi                           = 0;
    P_E9_KULLANICI_YETKILERI->adr_grup_tanimlama_yetkisi                                = 0;
    P_E9_KULLANICI_YETKILERI->adr_toplu_sms_gonderme_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->adr_adres_kayitlari_rapor_alma_yetkisi                    = 0;
    P_E9_KULLANICI_YETKILERI->adr_tel_web_kayitlari_rapor_alma_yetkisi                  = 0;

    //BANKA

    P_E9_KULLANICI_YETKILERI->bnk_banka_hesabi_acma_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->bnk_ent_fisleri_inceleme_yetkisi                         = 0;
    P_E9_KULLANICI_YETKILERI->bnk_defter_raporu_alma_yetkisi                           = 0;
    P_E9_KULLANICI_YETKILERI->bnk_pos_ekstresi_alma_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->bnk_hareket_fisi_islem_yetkisi                           = 0;
    P_E9_KULLANICI_YETKILERI->bnk_acilis_fisi_islem_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->bnk_makro_tanimlama_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->bnk_makro_fisi_islem_yetkisi                             = 0;
    P_E9_KULLANICI_YETKILERI->bnk_kk_ekstresi_alma_yetkisi                             = 0;
    P_E9_KULLANICI_YETKILERI->bnk_virman_islem_yetkisi                                 = 0;

    //CARI

    P_E9_KULLANICI_YETKILERI->cari_tahsilat_fisi_islem_yetkisi                         = 0;
    P_E9_KULLANICI_YETKILERI->cari_odeme_fisi_islem_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->cari_hareket_fisi_islem_yetkisi                          = 0;
    P_E9_KULLANICI_YETKILERI->cari_acilis_fisi_islem_yetkisi                           = 0;
    P_E9_KULLANICI_YETKILERI->cari_grup_tanimlama_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->cari_kart_tanimlama_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->cari_ent_fisleri_inceleme_yetkisi                        = 0;
    P_E9_KULLANICI_YETKILERI->cari_ekstre_alma_yetkisi                                 = 0;
    P_E9_KULLANICI_YETKILERI->cari_bakiye_raporu_alma_yetkisi                          = 0;
    P_E9_KULLANICI_YETKILERI->cari_virman_islem_yetkisi                                = 0;

    //CEKSENET

    P_E9_KULLANICI_YETKILERI->cek_senet_girebilir_mi                                  = 0;
    P_E9_KULLANICI_YETKILERI->cek_bordrosu_islem_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->cek_ent_fisleri_inceleme_yetkisi                        = 0;
    P_E9_KULLANICI_YETKILERI->cek_listesi_rapor_alma_yetkisi                          = 0;
    P_E9_KULLANICI_YETKILERI->cek_mus_cek_senet_arama_yetkisi                         = 0;
    P_E9_KULLANICI_YETKILERI->cek_acilis_bordrosu_islem_yetkisi                       = 0;

    //DEMIRBAS

    P_E9_KULLANICI_YETKILERI->dmr_amortisman_fisi_islem_yetkisi                       = 0;
    P_E9_KULLANICI_YETKILERI->dmr_grup_tanimlama_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->dmr_demirbas_karti_tanimlama_yetkisi                    = 0;
    P_E9_KULLANICI_YETKILERI->dmr_yen_degerleme_fisi_islem_yetkisi                    = 0;
    P_E9_KULLANICI_YETKILERI->dmr_demirbas_listesi_rapor_alma_yetkisi                 = 0;
    P_E9_KULLANICI_YETKILERI->dmr_toplu_amortisman_ayirma_yetkisi                     = 0;
    P_E9_KULLANICI_YETKILERI->dmr_toplu_yeniden_degerleme_yetkisi                     = 0;
    P_E9_KULLANICI_YETKILERI->dmr_rapor_islem_grubu_tanimlama_yetkisi                 = 0;


    //ISLETME

    P_E9_KULLANICI_YETKILERI->isl_isletme_defteri_rapor_alma_yetkisi                 = 0;
    P_E9_KULLANICI_YETKILERI->isl_kdv_raporu_alma_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->isl_fisi_islem_yetkisi                                 = 0;

    //MUHASEBE

    P_E9_KULLANICI_YETKILERI->muh_muhasebe_fisi_islem_yetkisi                          = 0;
    P_E9_KULLANICI_YETKILERI->muh_hesap_plani_islem_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->muh_yvm_no_olusturma_yetkisi                             = 0;
    P_E9_KULLANICI_YETKILERI->muh_hesap_ekstresi_alma_yetkisi                          = 0;
    P_E9_KULLANICI_YETKILERI->muh_mizan_raporu_alma_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->muh_hesap_plani_rapor_alma_yetkisi                       = 0;
    P_E9_KULLANICI_YETKILERI->muh_gelir_tablosu_rapor_alma_yetkisi                     = 0;
    P_E9_KULLANICI_YETKILERI->muh_bilanco_rapor_alma_yetkisi                           = 0;
    P_E9_KULLANICI_YETKILERI->muh_yvm_dokumu_alma_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->muh_defteri_kebir_raporu_alma_yetkisi                    = 0;
    P_E9_KULLANICI_YETKILERI->muh_fis_dokumu_alma_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->muh_hatali_fisleri_inceleme_yetkisi                      = 0;
    P_E9_KULLANICI_YETKILERI->muh_ent_fisleri_inceleme_yetkisi                         = 0;
    P_E9_KULLANICI_YETKILERI->muh_grup_tanimlama_yetkisi                               = 0;

    //PERSONEL

    P_E9_KULLANICI_YETKILERI->prs_bordro_inceleme_yetkisi                        = 0;
    P_E9_KULLANICI_YETKILERI->prs_ihbar_ve_kidem_tazminati_raporu_alma_yetkisi   = 0;
    P_E9_KULLANICI_YETKILERI->prs_isten_ayrilan_personel_arama_yetkisi           = 0;
    P_E9_KULLANICI_YETKILERI->prs_isten_ayrilma_islem_yetkisi                    = 0;
    P_E9_KULLANICI_YETKILERI->prs_maas_bordrosu_alma_yetkisi                     = 0;
    P_E9_KULLANICI_YETKILERI->prs_maas_bordrosu_iptal_yetkisi                    = 0;
    P_E9_KULLANICI_YETKILERI->prs_maas_bordrosu_raporu_alma_yetkisi              = 0;
    P_E9_KULLANICI_YETKILERI->prs_ek_kesinti_tanimlama_yetkisi                   = 0;
    P_E9_KULLANICI_YETKILERI->prs_ek_odenek_tanimlama_yetkisi                    = 0;
    P_E9_KULLANICI_YETKILERI->prs_personel_raporu_alma_yetkisi                   = 0;
    P_E9_KULLANICI_YETKILERI->prs_sicil_karti_tanimlama_yetkisi                  = 0;
    P_E9_KULLANICI_YETKILERI->prs_sabit_deger_guncelleme_yetkisi                 = 0;
    P_E9_KULLANICI_YETKILERI->prs_grup_tanimlama_yetkisi                         = 0;
    P_E9_KULLANICI_YETKILERI->prs_e_bildirge_alma_yetkisi                        = 0;
    P_E9_KULLANICI_YETKILERI->prs_ucret_pusulasi_raporu_alma_yetkisi             = 0;
    P_E9_KULLANICI_YETKILERI->prs_eksik_gun_nedenleri_giris_yetkisi              = 0;

    //STOK

    P_E9_KULLANICI_YETKILERI->stok_urun_karti_tanimlama_yetkisi                 =  0;
    P_E9_KULLANICI_YETKILERI->stok_fisi_islem_yetkisi                           =  0;
    P_E9_KULLANICI_YETKILERI->stok_grup_tanimlama_yetkisi                       =  0;
    P_E9_KULLANICI_YETKILERI->stok_hizmet_karti_tanimlama_yetkisi               =  0;
    P_E9_KULLANICI_YETKILERI->stok_urun_listesi_alma_yetkisi                    =  0;
    P_E9_KULLANICI_YETKILERI->stok_miktar_raporu_alma_yetkisi                   =  0;
    P_E9_KULLANICI_YETKILERI->stok_ekstresi_alma_yetkisi                        =  0;

    //FATURA

    P_E9_KULLANICI_YETKILERI->fat_alis_faturasi_kesebilir_mi                         = 0;
    P_E9_KULLANICI_YETKILERI->fat_sts_faturasi_kesebilir_mi                          = 0;
    P_E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_elle                            = 0;
    P_E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_oto                             = 0;
    P_E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_toplu                           = 0;
    P_E9_KULLANICI_YETKILERI->fat_satir_iskontosu_yapabilir_mi                       = 0;
    P_E9_KULLANICI_YETKILERI->fat_toplu_iskonto_yapabilir_mi                         = 0;
    P_E9_KULLANICI_YETKILERI->fat_hizmet_ekstresi_alma_yetkisi                       = 0;
    P_E9_KULLANICI_YETKILERI->fat_ba_bs_listesi_alma_yetkisi                         = 0;
    P_E9_KULLANICI_YETKILERI->fat_fatura_listesi_alma_yetkisi                        = 0;

    //İRSALİYE

    P_E9_KULLANICI_YETKILERI->irs_alis_irsaliyesi_islem_yetkisi                         = 0;
    P_E9_KULLANICI_YETKILERI->irs_sts_irsaliyesi_islem_yetkisi                          = 0;
    P_E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_elle                          = 0;
    P_E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_oto                           = 0;
    P_E9_KULLANICI_YETKILERI->irs_satir_iskontosu_yapabilir_mi                       = 0;
    P_E9_KULLANICI_YETKILERI->irs_listesi_gorebilir_mi                               = 0;
    P_E9_KULLANICI_YETKILERI->irs_toplu_iskonto_yapabilir_mi                         = 0;



    //SUBE_DEPO_UNITE

    P_E9_KULLANICI_YETKILERI->sub_sube_tanimlama_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->sub_depo_tanimlama_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->sub_depo_islem_yetkisi                                  = 0;
    P_E9_KULLANICI_YETKILERI->sub_sube_islem_yetkisi                                  = 0;
    P_E9_KULLANICI_YETKILERI->sub_ent_hesaplari_guncelleme_yetkisi                    = 0;

    //SM_MAKBUZU

    P_E9_KULLANICI_YETKILERI->smm_makbuzu_islem_yetkisi                               = 0;
    P_E9_KULLANICI_YETKILERI->smm_hizmet_tanimlama_yetkisi                            = 0;
    P_E9_KULLANICI_YETKILERI->smm_tahsil_edildi_odendi_secili_gelsin                  = 0;

    //DOVIZ

    P_E9_KULLANICI_YETKILERI->dvz_dovizleri_inceleme_yetkisi                          = 0;
    P_E9_KULLANICI_YETKILERI->dvz_kur_guncelleme_yetkisi                              = 0;
    P_E9_KULLANICI_YETKILERI->dvz_doviz_tanimlama_yetkisi                             = 0;

    //PERAKENDE

    P_E9_KULLANICI_YETKILERI->prk_fis_girebilir_mi                                     = 0;
}

/*************************************************************************************************
                                 E9_COPY_TABLES
*************************************************************************************************/

void E9_COPY_TABLES ( ADAK_SQL * P_SOURCE_DB, ADAK_SQL * P_TARGET_DB, QString p_table_name , QString p_where_condition, QStringList p_where_set_values )
{
    /*
     *  P_SOURCE_DB->GET_SQL_STRUCT(E9_PROGRAMI);
     *  parametre olarak e9 gonderilmesinin sebebi e9 devir ettigimiz icin.
     *  ADAK_VERITABANI_ID() kullannirsa SYS den giris yapildiginda sys tablolarinda
     *  kopyalama islemi yapmaya calisiyor.
     */

    ADAK_SQL_STRUCT  * SOURCE_STRUCT = P_SOURCE_DB->GET_SQL_STRUCT(E9_PROGRAMI);

    QStringList table_columns;

    // table sayisi kadar
    for ( int i = 0; i < SOURCE_STRUCT->table_count; i++ ) {

        // tablo mu?
        if ( SOURCE_STRUCT->tables[ i ].name EQ p_table_name ) {
            for ( int j = 0; j < SOURCE_STRUCT->tables[ i ].column_count; j++ ) {
                // columnlari ata
                table_columns << SOURCE_STRUCT->tables[ i ].columns[j].name;
            }
            break;
        }
    }

    if ( table_columns.size() EQ 0 ) {
        ADAK_WARNING( QObject::tr("Tablo kolonlari okunamadi...!"), NULL, NULL );
        return;
    }

    QString column_str;
    // queryde kullanmak icin aralarina virgul ataniyor.
    for( int i = 0; i < table_columns.size(); i++ ) {
        column_str.append( table_columns.at( i ) );
        if ( i NE table_columns.size() - 1 ) {
            column_str.append( ", " );
        }
    }

    SQL_QUERY source_query( P_SOURCE_DB );
    SQL_QUERY target_query( P_TARGET_DB );

    // source datalar okunuyor
    source_query.PREPARE_SELECT( p_table_name, column_str, p_where_condition  );

    if (p_where_condition NE NULL AND p_where_set_values NE QStringList()) {
        for ( int i = 0; i < p_where_set_values.size(); i++ ) {
            source_query.SET_VALUE( QString ( ":arg_%1").arg( i ), p_where_set_values.at( i ));
        }
    }


    if ( source_query.SELECT() NE 0 ) {
        int id = 0;

        while ( source_query.NEXT() EQ true ) {

            target_query.PREPARE_INSERT( p_table_name, table_columns.at(0), column_str );

            // target_db datalar insert ediliyor.
            for( int i = 0; i < table_columns.size(); i++ ) {

                target_query.SET_VALUE( QString ( ":%1" ).arg( table_columns.at( i )),  source_query.VALUE( QString ( "%1" ).arg( table_columns.at( i ) ) ).toString()   );
            }


            id = target_query.INSERT();
        }

        if ( id NE 0 ) {

            P_TARGET_DB->SET_TABLE_AUTO_INCREMENT_ID_VALUE(p_table_name , table_columns.at(0) ,id);

        }
    }

}

//*******************************************************************************
//             E9_GET_AYLAR_STRING
//*******************************************************************************

QString E9_GET_AYLAR_STRING ( int ay )
{
    QString    month_name;

    switch( ay ) {
        case ENUM_OCAK:
            month_name = "OCAK";
            break;
         case ENUM_SUBAT:
            month_name = "ŞUBAT";
            break;
        case ENUM_MART  :
            month_name = "MART";
            break;
        case ENUM_NISAN  :
            month_name = "NİSAN";
            break;
        case  ENUM_MAYIS :
            month_name = "MAYIS";
            break;
        case  ENUM_HAZIRAN      :
            month_name = "HAZİRAN";
            break;
        case  ENUM_TEMMUZ :
            month_name = "TEMMUZ";
            break;
        case ENUM_AGUSTOS :
            month_name = "AĞUSTOS";
            break;
        case ENUM_EYLUL  :
            month_name = "EYLÜL";
            break;
        case  ENUM_EKIM :
            month_name = "EKİM";
            break;
        case  ENUM_KASIM :
            month_name = "KASIM";
            break;
        case  ENUM_ARALIK :
            month_name = "ARALIK";
            break;
        default :
            month_name = "Belirtilmemiş Ay Adı";
            break;
    }

    return month_name;
}

//*******************************************************************************
//             E9_GET_AYLAR_ENUM
//*******************************************************************************

int E9_GET_AYLAR_ENUM ( QString p_ay )
{
    if ( p_ay EQ  "OCAK" ) {
        return ENUM_OCAK;
    }
    else if ( p_ay EQ  "ŞUBAT" ) {
        return ENUM_SUBAT;
    }
    else if ( p_ay EQ  "MART" ) {
        return ENUM_MART;
    }
    else if ( p_ay EQ  "NİSAN" ) {
        return ENUM_NISAN;
    }
    else if ( p_ay EQ  "MAYIS" ) {
        return ENUM_MAYIS;
    }
    else if ( p_ay EQ  "HAZİRAN" ) {
        return ENUM_HAZIRAN;
    }
    else if ( p_ay EQ  "TEMMUZ" ) {
        return ENUM_TEMMUZ;
    }
    else if ( p_ay EQ  "AĞUSTOS" ) {
       return ENUM_AGUSTOS;
    }
    else if ( p_ay EQ  "EYLÜL" ) {
       return ENUM_EYLUL;
    }
    else if ( p_ay EQ  "EKİM" ) {
       return ENUM_EKIM;
    }
    else if ( p_ay EQ  "KASIM" ) {
       return ENUM_KASIM;
    }
    else if ( p_ay EQ  "ARALIK" ) {
       return ENUM_ARALIK;
    }

    return 0;
}

//*******************************************************************************
//             E9_KULLANICIYI_YETKILERI_UPDATE
//*******************************************************************************

void  E9_KULLANICIYI_YETKILERI_UPDATE ( E9_KULLANICI_YETKILERI_STRUCT * P_YETKI_STRUCT , int kullanici_id )
{

    SQL_QUERY query ( DB );


    query.PREPARE_UPDATE ( "e9_kullanici_yetkileri",
                          "yetki_id",

/**************************************************** ADRES ********************************************************/

                        "adr_frm_shs_blg_gorebilir_mi     , adr_not_defterini_gorebilir_mi   , "
                        "adr_adres_kayitlari_raporu       , adr_tel_web_kayitlari_raporu     , "
                        "adr_etiket_yazdirabilir_mi       , "
                        "adr_fihrist_yazdirabilir_mi      , adr_toplu_mail_gonderebilir_mi   , "
                        "adr_grup_tanimlayabilir_mi       , adr_toplu_sms_gonderebilir_mi    , "
                        "adr_export_yetkisi               , adr_import_yetkisi               , "

/********************************************************* BANKA  ***************************************************/


                        "bnk_hesabi_acabilir_mi           , bnk_ent_fislerini_gorebilir_mi,   "
                        "bnk_defterini_inceleyebilir_mi   , bnk_pos_ekst_inceleyebilir_mi,    "
                        "bnk_hareket_fisi_girebilir_mi    , bnk_acilis_fisi_girebilir_mi,     "
                        "bnk_makro_tanimlayabilir_mi      , bnk_makro_fisi_kesebilir_mi,      "
                        "bnk_kk_ekstresi_alabilir_mi      , bnk_virman_islemi_yapabilir_mi,   "

/************************************************************** CARI  **********************************************/

                        "car_tah_fisi_kesebilir_mi        , car_odeme_fisi_kesebilir_mi,     "
                        "car_hareket_fisi_kesebilir_mi    , car_acilis_fisi_kesebilir_mi,    "
                        "car_grup_tanimlayabilir_mi       ,      "
                        "car_kart_tanimlayabilir_mi       , car_ent_fislerini_gorebilir_mi,  "
                        "car_ekstreyi_inceleyebilir_mi    , car_bakiye_rpr_inceleyebilirmi, "
                        "car_virman_fisi_kesebilir_mi,    car_hesap_hareketleri_grb_mi , "

/************************************************************** CEKSENET ******************************************/

                        "cek_senet_girebilir_mi           , cek_bordro_girebilir_mi,        "
                        "cek_ent_fislerini_gorebilir_mi   , cek_listesini_inceleyebilir_mi, "
                        "cek_mus_ceklerini_gorebilir_mi   , cek_acilis_bordrosu_yetkisi     ,"

/*************************************************************** DEMIRBAS  *****************************************/


                        "dmr_karti_tanimlayabilir_mi      , dmr_grubu_tanimlayabilir_mi,      "
                        "dmr_yeni_deger_fis_kesebilirmi   , dmr_amrtsman_fisi_kesebilir_mi, "
                        "dmr_listesi_raporu_alabilir_mi   , dmr_toplu_amortisman_ayrablrmi,"
                        "dmr_toplu_yeni_deger_ypblrmi     , dmr_rpr_islm_grup_tanmlyblr_mi ,"

/************************************************************** MUHASEBE  ******************************************/


                        "muh_mhs_fisi_kesebilir_mi        , muh_ent_fislerini_gorebilir_mi   , "
                        "muh_hesap_planini_gorebilir_mi   , muh_yvm_no_olusturabilir_mi      ,  "
                        "muh_hsp_ekstre_gorebilir_mi      ,  "
                        "muh_mizan_rprunu_gorebilir_mi    , muh_hsp_plani_rpr_gorebilir_mi   , "
                        "muh_gelir_tblsunu_gorebilir_mi   , muh_bilanco_inceleyebilir_mi     , "
                        "muh_yvm_dkmunu_gorebilir_mi      , muh_dftri_kbr_inceleyebilir_mi   , "
                        "muh_fis_dokumunu_gorebilir_mi    , muh_hata_fisleri_gorebilir_mi    , "
                        "muh_hsp_grbu_tanimlayabilir_mi   , "
                        "muh_yan_fisi_olusturabilir_mi    , muh_yan_hesabi_girebilir_mi      , "
                        "hesap_ekstresi_nakli_yekun       , mizan_nakli_yekun                , "
                        "muh_hesap_plani_kopyalama        , muh_acilis_fisi_islem_yetkisi    , "

/********************************************************** PERSONEL  ***********************************************/


                        "prs_tanimlarina_girebilir_mi     , prs_ek_odnk_tanimlayabilir_mi  , "
                        "prs_ek_ksnti_tanimlayabilir_mi   , prs_sbt_degerlere_girebilir_mi , "
                        "prs_maas_brd_alabilir_mi         , prs_maas_brd_iptal_edebilir_mi , "
                        "prs_maas_brd_rpr_alabilir_mi     , prs_ihbr_kdm_rpr_alabilir_mi   , "
                        "prs_is_ayrilma_rpr_alabilir_mi   , prs_brd_inceleme_yapabilir_mi  , "
                        "prs_prsonel_raporu_alabilir_mi   , prs_e_bildirge_alabilir_mi     , "
                        "prs_prsonel_grup_girebilir_mi    , prs_isten_ayr_prs_arayabilirmi , "
                        "prs_eksik_gun_nedeni_girisi      , prs_ucret_pusulasi_raporu      , "

/********************************************************* SUBE/ DEPO / UNITE  *************************************/

                    "sub_tanimlayabilir_mi   , sub_depo_tanimlayabilir_mi,  "
                    "sub_depo_islem_yetkisi  , sub_sube_islem_yetkisi,      "
                    "sub_ent_hesaplari_yetkisi , "

/************************************************************** STOK **************************************************/

                    "stk_urun_kartina_girebilir_mi   , stk_grubu_tanimlayabilir_mi,   "
                    "stk_fisine_girebilir_mi         ,       "
                    "stk_hizmet_tanimlayabilir_mi    ,  "
                    "stk_urn_listesini_gorebilir_mi  , stk_miktar_rprunu_gorebilir_mi, "
                    "stk_ekstre_inceleyebilir_mi     , stk_urun_hareketlerini_grb_mi,  "

 /*****************************************       FATURA         ***************************************************/

                    "fat_alis_faturasi_kesebilir_mi , fat_sts_faturasi_kesebilir_mi  ,"
                    "fat_irs_faturalastirma_elle    , "
                    "fat_irs_faturalastirma_oto     , fat_irs_faturalastirma_toplu   ,"
                    "fat_satir_iskonto_yapabilir_mi , fat_toplu_iskonto_yapabilir_mi ,"
                    "fat_hizmet_ekstre_alabilir_mi  , "
                    "fat_ba_bs_alabilir_mi          , fat_fatura_listesi_alabilir_mi ,"

/****************************************         İRSALİYE      **************************************************/

                    "irs_alis_irsaliye_kesebilir_mi , irs_sts_irsaliye_kesebilir_mi     ,"
                    "irs_fat_irsaliyelestirme_elle  , irs_fat_irsaliyelestirme_oto      ,"
                    "irs_satir_iskonto_yapabilir_mi , irs_listesi_gorebilir_mi          ,"
                    "irs_toplu_iskonto_yapabilir_mi ,"


/***************************************************** ISLETME ****************************************************/

                    "isl_raporunu_gorebilir_mi       ,  isl_kdv_raporunu_gorebilir_mi,    "
                    "isl_fisi_kesebilir_mi           , "

/***************************************************** SM_MAKBUZU ****************************************************/

                    "smm_makbuzu_gorebilir_mi        , "
                    "smm_hizmet_tanimlayabilir_mi    , "
                    "smm_tahsil_edildi_odendi        , "

 /***************************************************** DOVIZ ****************************************************/
                    "dvz_kurlarini_gorebilir_mi      ,"
                    "dvz_tanimlayabilir_mi           ,"
                    "dvz_kuru_guncelleyebilir_mi     ,"

/*******************************************  PERAKENDE      *****************************************************/

                    "prk_fisi_girebilir_mi          ,   prk_gdr_tur_tanimlayabilir_mi    , "
                    "prk_kasa_raporu_alabilir_mi    ,"

/************************************************** SISTEM  ***************************************************/

                    "sis_blg_formuna_girebilir_mi       , sis_kul_formuna_girebilir_mi,     "
                    "sis_frm_formuna_girebilir_mi       , sis_yonetim_log_gorebilir_mi,     "
                    "sis_mail_srv_tanimlayabilirmi      , sis_sms_srv_tanimlayabilirmi,     "
                    "e9_yetkilendirme_yapabilir_mi      , muh_kullanabilir_mi,              "
                    "isl_kullanabilir_mi                , adr_kullanabilir_mi,              "
                    "dvz_kullanabilir_mi                , car_kullanabilir_mi,              "
                    "cek_kullanabilir_mi                , bnk_kullanabilir_mi,              "
                    "sub_kullanabilir_mi                , fat_kullanabilir_mi,              "
                    "irs_kullanabilir_mi                , stk_kullanabilir_mi,              "
                    "prs_kullanabilir_mi                ,                                   "
                    "dmr_kullanabilir_mi                , smm_makbuzu_kullanabilir_mi,       "
                    "faiz_kullanabilir_mi               ,                                   "
                    "e9_onar_yapabilir_mi               , e9_devir_islemi_yapabilir_mi,     "
                    "e9_log_gorebilir_mi                , "
                    "e9_sabit_degerlere_girebilirmi     , e9_belge_tasarlayabilir_mi,       "
                    "e9_ent_blg_degistirebilir_mi       , e9_kullanicinin_sirketi ,         "
                    "e9_sirket_degistire_bilir_mi            ,"
                    "prk_kullanabilir_mi,                e9_urettigi_fisleri_grbilir_mi " ,
                     "kullanici_id = :kullanici_id");




        // *********************** ADRES ***********************

        query.SET_VALUE( ":adr_frm_shs_blg_gorebilir_mi"           , P_YETKI_STRUCT->adr_adres_karti_tanimlama_yetkisi         );
        query.SET_VALUE( ":adr_not_defterini_gorebilir_mi"         , P_YETKI_STRUCT->adr_not_defteri_inceleme_yetkisi          );
        query.SET_VALUE( ":adr_adres_kayitlari_raporu"             , P_YETKI_STRUCT->adr_adres_kayitlari_rapor_alma_yetkisi    );
        query.SET_VALUE( ":adr_tel_web_kayitlari_raporu"           , P_YETKI_STRUCT->adr_tel_web_kayitlari_rapor_alma_yetkisi  );
        query.SET_VALUE( ":adr_etiket_yazdirabilir_mi"             , P_YETKI_STRUCT->adr_etiket_yazdirma_yetkisi               );
        query.SET_VALUE( ":adr_fihrist_yazdirabilir_mi"            , P_YETKI_STRUCT->adr_fihrist_raporu_alma_yetkisi           );
        query.SET_VALUE( ":adr_toplu_mail_gonderebilir_mi"         , P_YETKI_STRUCT->adr_toplu_mail_gonderme_yetkisi           );
        query.SET_VALUE( ":adr_grup_tanimlayabilir_mi"             , P_YETKI_STRUCT->adr_grup_tanimlama_yetkisi                );
        query.SET_VALUE( ":adr_toplu_sms_gonderebilir_mi"          , P_YETKI_STRUCT->adr_toplu_sms_gonderme_yetkisi            );
        query.SET_VALUE( ":adr_export_yetkisi"                     , P_YETKI_STRUCT->adr_export_yetkisi);
        query.SET_VALUE( ":adr_import_yetkisi"                     , P_YETKI_STRUCT->adr_import_yetkisi);

        // ********************** BANKA **************************


        query.SET_VALUE( ":bnk_hesabi_acabilir_mi"                 , P_YETKI_STRUCT->bnk_banka_hesabi_acma_yetkisi            );
        query.SET_VALUE( ":bnk_ent_fislerini_gorebilir_mi"         , P_YETKI_STRUCT->bnk_ent_fisleri_inceleme_yetkisi         );
        query.SET_VALUE( ":bnk_defterini_inceleyebilir_mi"         , P_YETKI_STRUCT->bnk_defter_raporu_alma_yetkisi           );
        query.SET_VALUE( ":bnk_pos_ekst_inceleyebilir_mi"          , P_YETKI_STRUCT->bnk_pos_ekstresi_alma_yetkisi            );
        query.SET_VALUE( ":bnk_hareket_fisi_girebilir_mi"          , P_YETKI_STRUCT->bnk_hareket_fisi_islem_yetkisi           );
        query.SET_VALUE( ":bnk_acilis_fisi_girebilir_mi"           , P_YETKI_STRUCT->bnk_acilis_fisi_islem_yetkisi            );
        query.SET_VALUE( ":bnk_makro_tanimlayabilir_mi"            , P_YETKI_STRUCT->bnk_makro_tanimlama_yetkisi              );
        query.SET_VALUE( ":bnk_makro_fisi_kesebilir_mi"            , P_YETKI_STRUCT->bnk_makro_fisi_islem_yetkisi             );
        query.SET_VALUE( ":bnk_kk_ekstresi_alabilir_mi"            , P_YETKI_STRUCT->bnk_kk_ekstresi_alma_yetkisi             );
        query.SET_VALUE( ":bnk_virman_islemi_yapabilir_mi"         , P_YETKI_STRUCT->bnk_virman_islem_yetkisi                 );


        // ********************** CARI **************************

        query.SET_VALUE( ":car_tah_fisi_kesebilir_mi"              , P_YETKI_STRUCT->cari_tahsilat_fisi_islem_yetkisi            );
        query.SET_VALUE( ":car_odeme_fisi_kesebilir_mi"            , P_YETKI_STRUCT->cari_odeme_fisi_islem_yetkisi               );
        query.SET_VALUE( ":car_hareket_fisi_kesebilir_mi"          , P_YETKI_STRUCT->cari_hareket_fisi_islem_yetkisi             );
        query.SET_VALUE( ":car_acilis_fisi_kesebilir_mi"           , P_YETKI_STRUCT->cari_acilis_fisi_islem_yetkisi              );
        query.SET_VALUE( ":car_grup_tanimlayabilir_mi"             , P_YETKI_STRUCT->cari_grup_tanimlama_yetkisi                 );
        query.SET_VALUE( ":car_kart_tanimlayabilir_mi"             , P_YETKI_STRUCT->cari_kart_tanimlama_yetkisi                 );
        query.SET_VALUE( ":car_ent_fislerini_gorebilir_mi"         , P_YETKI_STRUCT->cari_ent_fisleri_inceleme_yetkisi           );
        query.SET_VALUE( ":car_ekstreyi_inceleyebilir_mi"          , P_YETKI_STRUCT->cari_ekstre_alma_yetkisi                    );
        query.SET_VALUE( ":car_bakiye_rpr_inceleyebilirmi"         , P_YETKI_STRUCT->cari_bakiye_raporu_alma_yetkisi             );
        query.SET_VALUE( ":car_virman_fisi_kesebilir_mi"           , P_YETKI_STRUCT->cari_virman_islem_yetkisi                   );
        query.SET_VALUE( ":car_hesap_hareketleri_grb_mi"           , P_YETKI_STRUCT->cari_hesap_hareketleri_gorebilir_mi         );

        // ********************** CEK  SENET **************************

        query.SET_VALUE( ":cek_senet_girebilir_mi"                , P_YETKI_STRUCT->cek_senet_girebilir_mi                       );
        query.SET_VALUE( ":cek_bordro_girebilir_mi"               , P_YETKI_STRUCT->cek_bordrosu_islem_yetkisi                   );
        query.SET_VALUE( ":cek_ent_fislerini_gorebilir_mi"        , P_YETKI_STRUCT->cek_ent_fisleri_inceleme_yetkisi             );
        query.SET_VALUE( ":cek_listesini_inceleyebilir_mi"        , P_YETKI_STRUCT->cek_listesi_rapor_alma_yetkisi               );
        query.SET_VALUE( ":cek_mus_ceklerini_gorebilir_mi"        , P_YETKI_STRUCT->cek_mus_cek_senet_arama_yetkisi              );
        query.SET_VALUE( ":cek_acilis_bordrosu_yetkisi"           , P_YETKI_STRUCT->cek_acilis_bordrosu_islem_yetkisi            );

        // ********************** DEMIRBAS **************************

        query.SET_VALUE( ":dmr_karti_tanimlayabilir_mi"           , P_YETKI_STRUCT->dmr_demirbas_karti_tanimlama_yetkisi      );
        query.SET_VALUE( ":dmr_grubu_tanimlayabilir_mi"           , P_YETKI_STRUCT->dmr_grup_tanimlama_yetkisi                );
        query.SET_VALUE( ":dmr_yeni_deger_fis_kesebilirmi"        , P_YETKI_STRUCT->dmr_yen_degerleme_fisi_islem_yetkisi      );
        query.SET_VALUE( ":dmr_amrtsman_fisi_kesebilir_mi"        , P_YETKI_STRUCT->dmr_amortisman_fisi_islem_yetkisi         );
        query.SET_VALUE( ":dmr_listesi_raporu_alabilir_mi"        , P_YETKI_STRUCT->dmr_demirbas_listesi_rapor_alma_yetkisi   );
        query.SET_VALUE( ":dmr_toplu_amortisman_ayrablrmi"        , P_YETKI_STRUCT->dmr_toplu_amortisman_ayirma_yetkisi       );
        query.SET_VALUE( ":dmr_toplu_yeni_deger_ypblrmi"          , P_YETKI_STRUCT->dmr_toplu_yeniden_degerleme_yetkisi       );
        query.SET_VALUE( ":dmr_rpr_islm_grup_tanmlyblr_mi"        , P_YETKI_STRUCT->dmr_rapor_islem_grubu_tanimlama_yetkisi   );

        // ********************** MUHASEBE **************************

        query.SET_VALUE( ":muh_mhs_fisi_kesebilir_mi"             , P_YETKI_STRUCT->muh_muhasebe_fisi_islem_yetkisi            );
        query.SET_VALUE( ":muh_ent_fislerini_gorebilir_mi"        , P_YETKI_STRUCT->muh_ent_fisleri_inceleme_yetkisi           );
        query.SET_VALUE( ":muh_hesap_planini_gorebilir_mi"        , P_YETKI_STRUCT->muh_hesap_plani_islem_yetkisi              );
        query.SET_VALUE( ":muh_yvm_no_olusturabilir_mi"           , P_YETKI_STRUCT->muh_yvm_no_olusturma_yetkisi               );
        query.SET_VALUE( ":muh_hsp_ekstre_gorebilir_mi"           , P_YETKI_STRUCT->muh_hesap_ekstresi_alma_yetkisi            );
        query.SET_VALUE( ":muh_mizan_rprunu_gorebilir_mi"         , P_YETKI_STRUCT->muh_mizan_raporu_alma_yetkisi              );
        query.SET_VALUE( ":muh_hsp_plani_rpr_gorebilir_mi"        , P_YETKI_STRUCT->muh_hesap_plani_rapor_alma_yetkisi         );
        query.SET_VALUE( ":muh_gelir_tblsunu_gorebilir_mi"        , P_YETKI_STRUCT->muh_gelir_tablosu_rapor_alma_yetkisi       );
        query.SET_VALUE( ":muh_bilanco_inceleyebilir_mi"          , P_YETKI_STRUCT->muh_bilanco_rapor_alma_yetkisi             );
        query.SET_VALUE( ":muh_yvm_dkmunu_gorebilir_mi"           , P_YETKI_STRUCT->muh_yvm_dokumu_alma_yetkisi                );
        query.SET_VALUE( ":muh_dftri_kbr_inceleyebilir_mi"        , P_YETKI_STRUCT->muh_defteri_kebir_raporu_alma_yetkisi      );
        query.SET_VALUE( ":muh_fis_dokumunu_gorebilir_mi"         , P_YETKI_STRUCT->muh_fis_dokumu_alma_yetkisi                );
        query.SET_VALUE( ":muh_hata_fisleri_gorebilir_mi"         , P_YETKI_STRUCT->muh_hatali_fisleri_inceleme_yetkisi        );
        query.SET_VALUE( ":muh_hsp_grbu_tanimlayabilir_mi"        , P_YETKI_STRUCT->muh_grup_tanimlama_yetkisi                 );
        query.SET_VALUE( ":muh_yan_fisi_olusturabilir_mi"         , P_YETKI_STRUCT->muh_yansitma_fisi_olusturma_yetkisi        );
        query.SET_VALUE( ":muh_yan_hesabi_girebilir_mi"           , P_YETKI_STRUCT->muh_yansitma_hesabi_tanimlama_yetkisi      );
        query.SET_VALUE( ":hesap_ekstresi_nakli_yekun"            , P_YETKI_STRUCT->muh_hesap_ekstresi_nakli_yekun             );
        query.SET_VALUE( ":mizan_nakli_yekun"                     , P_YETKI_STRUCT->muh_mizan_nakli_yekun                      );
        query.SET_VALUE( ":muh_hesap_plani_kopyalama"             , P_YETKI_STRUCT->muh_hesap_plani_kopyalama_yetkisi          );
        query.SET_VALUE( ":muh_acilis_fisi_islem_yetkisi"         , P_YETKI_STRUCT->muh_acilis_fisi_islem_yetkisi              );

        // ********************** PERSONEL **************************


        query.SET_VALUE( ":prs_tanimlarina_girebilir_mi"          , P_YETKI_STRUCT->prs_sicil_karti_tanimlama_yetkisi                 );
        query.SET_VALUE( ":prs_ek_odnk_tanimlayabilir_mi"         , P_YETKI_STRUCT->prs_ek_odenek_tanimlama_yetkisi                   );
        query.SET_VALUE( ":prs_ek_ksnti_tanimlayabilir_mi"        , P_YETKI_STRUCT->prs_ek_kesinti_tanimlama_yetkisi                  );
        query.SET_VALUE( ":prs_sbt_degerlere_girebilir_mi"        , P_YETKI_STRUCT->prs_sabit_deger_guncelleme_yetkisi                );
        query.SET_VALUE( ":prs_maas_brd_alabilir_mi"              , P_YETKI_STRUCT->prs_maas_bordrosu_alma_yetkisi                    );
        query.SET_VALUE( ":prs_maas_brd_iptal_edebilir_mi"        , P_YETKI_STRUCT->prs_maas_bordrosu_iptal_yetkisi                   );
        query.SET_VALUE( ":prs_maas_brd_rpr_alabilir_mi"          , P_YETKI_STRUCT->prs_maas_bordrosu_raporu_alma_yetkisi             );
        query.SET_VALUE( ":prs_ihbr_kdm_rpr_alabilir_mi"          , P_YETKI_STRUCT->prs_ihbar_ve_kidem_tazminati_raporu_alma_yetkisi  );
        query.SET_VALUE( ":prs_is_ayrilma_rpr_alabilir_mi"        , P_YETKI_STRUCT->prs_isten_ayrilma_islem_yetkisi                   );
        query.SET_VALUE( ":prs_brd_inceleme_yapabilir_mi"         , P_YETKI_STRUCT->prs_bordro_inceleme_yetkisi                       );
        query.SET_VALUE( ":prs_prsonel_raporu_alabilir_mi"        , P_YETKI_STRUCT->prs_personel_raporu_alma_yetkisi                  );
        query.SET_VALUE( ":prs_e_bildirge_alabilir_mi"            , P_YETKI_STRUCT->prs_e_bildirge_alma_yetkisi                       );
        query.SET_VALUE( ":prs_prsonel_grup_girebilir_mi"         , P_YETKI_STRUCT->prs_grup_tanimlama_yetkisi                        );
        query.SET_VALUE( ":prs_isten_ayr_prs_arayabilirmi"        , P_YETKI_STRUCT->prs_isten_ayrilan_personel_arama_yetkisi          );
        query.SET_VALUE( ":prs_eksik_gun_nedeni_girisi"           , P_YETKI_STRUCT->prs_eksik_gun_nedenleri_giris_yetkisi             );
        query.SET_VALUE( ":prs_ucret_pusulasi_raporu"             , P_YETKI_STRUCT->prs_ucret_pusulasi_raporu_alma_yetkisi);

        // ********************** SUBE DEPO UNITE ********************

        query.SET_VALUE( ":sub_tanimlayabilir_mi"                 , P_YETKI_STRUCT->sub_sube_tanimlama_yetkisi                        );
        query.SET_VALUE( ":sub_depo_tanimlayabilir_mi"            , P_YETKI_STRUCT->sub_depo_tanimlama_yetkisi                        );
        query.SET_VALUE( ":sub_depo_islem_yetkisi"                , P_YETKI_STRUCT->sub_depo_islem_yetkisi                            );
        query.SET_VALUE( ":sub_sube_islem_yetkisi"                , P_YETKI_STRUCT->sub_sube_islem_yetkisi                            );
        query.SET_VALUE( ":sub_ent_hesaplari_yetkisi"             , P_YETKI_STRUCT->sub_ent_hesaplari_guncelleme_yetkisi              );

        // ********************** STOK  **************************

        query.SET_VALUE( ":stk_urun_kartina_girebilir_mi"         , P_YETKI_STRUCT->stok_urun_karti_tanimlama_yetkisi                  );
        query.SET_VALUE( ":stk_grubu_tanimlayabilir_mi"           , P_YETKI_STRUCT->stok_grup_tanimlama_yetkisi                        );
        query.SET_VALUE( ":stk_fisine_girebilir_mi"               , P_YETKI_STRUCT->stok_fisi_islem_yetkisi                            );
        query.SET_VALUE( ":stk_hizmet_tanimlayabilir_mi"          , P_YETKI_STRUCT->stok_hizmet_karti_tanimlama_yetkisi                );
        query.SET_VALUE( ":stk_urn_listesini_gorebilir_mi"        , P_YETKI_STRUCT->stok_urun_listesi_alma_yetkisi                     );
        query.SET_VALUE( ":stk_miktar_rprunu_gorebilir_mi"        , P_YETKI_STRUCT->stok_miktar_raporu_alma_yetkisi                    );
        query.SET_VALUE( ":stk_ekstre_inceleyebilir_mi"           , P_YETKI_STRUCT->stok_ekstresi_alma_yetkisi                         );
        query.SET_VALUE( ":stk_urun_hareketlerini_grb_mi"         , P_YETKI_STRUCT->stok_urun_hareketlerini_gorebilir_mi               );

        // ********************** FATURA  **************************

        query.SET_VALUE( ":fat_alis_faturasi_kesebilir_mi"       , P_YETKI_STRUCT->fat_alis_faturasi_kesebilir_mi                    );
        query.SET_VALUE( ":fat_sts_faturasi_kesebilir_mi"        , P_YETKI_STRUCT->fat_sts_faturasi_kesebilir_mi                     );
        query.SET_VALUE( ":fat_irs_faturalastirma_elle"          , P_YETKI_STRUCT->fat_irs_faturalastirma_elle                       );
        query.SET_VALUE( ":fat_irs_faturalastirma_oto"           , P_YETKI_STRUCT->fat_irs_faturalastirma_oto                        );
        query.SET_VALUE( ":fat_irs_faturalastirma_toplu"         , P_YETKI_STRUCT->fat_irs_faturalastirma_toplu                      );
        query.SET_VALUE( ":fat_satir_iskonto_yapabilir_mi"       , P_YETKI_STRUCT->fat_satir_iskontosu_yapabilir_mi                  );
        query.SET_VALUE( ":fat_toplu_iskonto_yapabilir_mi"       , P_YETKI_STRUCT->fat_toplu_iskonto_yapabilir_mi                    );
        query.SET_VALUE( ":fat_hizmet_ekstre_alabilir_mi"        , P_YETKI_STRUCT->fat_hizmet_ekstresi_alma_yetkisi                  );
        query.SET_VALUE( ":fat_ba_bs_alabilir_mi"                , P_YETKI_STRUCT->fat_ba_bs_listesi_alma_yetkisi                    );
        query.SET_VALUE( ":fat_fatura_listesi_alabilir_mi"       , P_YETKI_STRUCT->fat_fatura_listesi_alma_yetkisi                   );

        // ********************** İRSALİYE  **************************

        query.SET_VALUE( ":irs_alis_irsaliye_kesebilir_mi"       , P_YETKI_STRUCT->irs_alis_irsaliyesi_islem_yetkisi                    );
        query.SET_VALUE( ":irs_sts_irsaliye_kesebilir_mi"        , P_YETKI_STRUCT->irs_sts_irsaliyesi_islem_yetkisi                     );
        query.SET_VALUE( ":irs_fat_irsaliyelestirme_elle"        , P_YETKI_STRUCT->irs_fat_irsaliyelestirme_elle                     );
        query.SET_VALUE( ":irs_fat_irsaliyelestirme_oto"         , P_YETKI_STRUCT->irs_fat_irsaliyelestirme_oto                      );
        query.SET_VALUE( ":irs_satir_iskonto_yapabilir_mi"       , P_YETKI_STRUCT->irs_satir_iskontosu_yapabilir_mi                  );
        query.SET_VALUE( ":irs_listesi_gorebilir_mi"             , P_YETKI_STRUCT->irs_listesi_gorebilir_mi                          );
        query.SET_VALUE( ":irs_toplu_iskonto_yapabilir_mi"       , P_YETKI_STRUCT->irs_toplu_iskonto_yapabilir_mi                    );


        // ********************** ISLETME  **************************

        query.SET_VALUE( ":isl_raporunu_gorebilir_mi"            , P_YETKI_STRUCT->isl_isletme_defteri_rapor_alma_yetkisi         );
        query.SET_VALUE( ":isl_kdv_raporunu_gorebilir_mi"        , P_YETKI_STRUCT->isl_kdv_raporu_alma_yetkisi                     );
        query.SET_VALUE( ":isl_fisi_kesebilir_mi"                , P_YETKI_STRUCT->isl_fisi_islem_yetkisi                          );

        // ********************** SM_MAKBUZU  **************************

        query.SET_VALUE(":smm_makbuzu_gorebilir_mi"              , P_YETKI_STRUCT->smm_makbuzu_islem_yetkisi                         );
        query.SET_VALUE(":smm_hizmet_tanimlayabilir_mi"          , P_YETKI_STRUCT->smm_hizmet_tanimlama_yetkisi                      );
        query.SET_VALUE(":smm_tahsil_edildi_odendi"              , P_YETKI_STRUCT->smm_tahsil_edildi_odendi_secili_gelsin            );

        // ********************** DOVIZ   **************************

        query.SET_VALUE(":dvz_kurlarini_gorebilir_mi"            , P_YETKI_STRUCT->dvz_dovizleri_inceleme_yetkisi                      );
        query.SET_VALUE(":dvz_tanimlayabilir_mi"                 , P_YETKI_STRUCT->dvz_doviz_tanimlama_yetkisi                           );
        query.SET_VALUE(":dvz_kuru_guncelleyebilir_mi"           , P_YETKI_STRUCT->dvz_kur_guncelleme_yetkisi                     );

        // ********************** PERAKENDE  ***********************

        query.SET_VALUE( ":prk_fisi_girebilir_mi"               , P_YETKI_STRUCT->prk_fis_girebilir_mi                              );
        query.SET_VALUE( ":prk_gdr_tur_tanimlayabilir_mi"       , P_YETKI_STRUCT->prk_gdr_tur_tanimlayabilir_mi                     );
        query.SET_VALUE( ":prk_kasa_raporu_alabilir_mi"         , P_YETKI_STRUCT->prk_kasa_raporu_alabilir_mi                       );

        // ********************** SISTEM  **************************

        query.SET_VALUE( ":sis_blg_formuna_girebilir_mi"        , P_YETKI_STRUCT->sis_blg_formuna_girebilir_mi                      );
        query.SET_VALUE( ":sis_kul_formuna_girebilir_mi"        , P_YETKI_STRUCT->sis_kul_formuna_girebilir_mi                      );
        query.SET_VALUE( ":sis_frm_formuna_girebilir_mi"        , P_YETKI_STRUCT->sis_veritabani_formuna_girebilir_mi               );
        query.SET_VALUE( ":sis_yonetim_log_gorebilir_mi"        , P_YETKI_STRUCT->sis_yonetim_log_inceleme_yetkisi                  );
        query.SET_VALUE( ":sis_mail_srv_tanimlayabilirmi"       , P_YETKI_STRUCT->sis_mail_srv_tanimlayabilirmi                     );
        query.SET_VALUE( ":sis_sms_srv_tanimlayabilirmi"        , P_YETKI_STRUCT->sis_sms_srv_tanimlayabilirmi                      );
        query.SET_VALUE( ":e9_yetkilendirme_yapabilir_mi"       , P_YETKI_STRUCT->ynt_e9_yetkilendirme_yapabilir_mi                 );
        query.SET_VALUE( ":muh_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_muh_kullanabilir_mi                            );
        query.SET_VALUE( ":isl_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_isl_kullanabilir_mi                            );
        query.SET_VALUE( ":adr_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_adr_kullanabilir_mi                            );
        query.SET_VALUE( ":dvz_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_dvz_kullanabilir_mi                            );
        query.SET_VALUE( ":car_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_car_kullanabilir_mi                            );
        query.SET_VALUE( ":cek_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_cek_kullanabilir_mi                            );
        query.SET_VALUE( ":bnk_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_bnk_kullanabilir_mi                            );
        query.SET_VALUE( ":sub_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_sub_kullanabilir_mi                            );
        query.SET_VALUE( ":fat_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_fat_kullanabilir_mi                            );
        query.SET_VALUE( ":irs_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_irs_kullanabilir_mi                            );
        query.SET_VALUE( ":stk_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_stk_kullanabilir_mi                            );
        query.SET_VALUE( ":prs_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_prs_kullanabilir_mi                            );
        query.SET_VALUE( ":dmr_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_dmr_kullanabilir_mi                            );
        query.SET_VALUE( ":smm_makbuzu_kullanabilir_mi"         , P_YETKI_STRUCT->e9_smm_kullanabilir_mi                            );
        query.SET_VALUE( ":faiz_kullanabilir_mi"                , P_YETKI_STRUCT->e9_faiz_kullanabilir_mi                           );
        query.SET_VALUE( ":e9_onar_yapabilir_mi"                , P_YETKI_STRUCT->ynt_e9_onar_yetkisi                               );
        query.SET_VALUE( ":e9_devir_islemi_yapabilir_mi"        , P_YETKI_STRUCT->ynt_e9_devir_islemi_yapabilir_mi                  );
        query.SET_VALUE( ":e9_log_gorebilir_mi"                 , P_YETKI_STRUCT->ynt_e9_log_inceleme_yetkisi                       );
        query.SET_VALUE( ":e9_sabit_degerlere_girebilirmi"      , P_YETKI_STRUCT->ynt_e9_sabit_degerler_guncelleme_yetkisi          );
        query.SET_VALUE( ":e9_belge_tasarlayabilir_mi"          , P_YETKI_STRUCT->ynt_e9_belge_tasarlama_yetkisi                    );
        query.SET_VALUE( ":e9_ent_blg_degistirebilir_mi"        , P_YETKI_STRUCT->ynt_e9_ent_bilgilerini_degistirme_yetkisi         );
        query.SET_VALUE( ":e9_kullanicinin_sirketi"             , P_YETKI_STRUCT->ynt_e9_kullanicinin_calistigi_sirket              );
        query.SET_VALUE( ":e9_sirket_degistire_bilir_mi"        , P_YETKI_STRUCT->ynt_e9_kullanici_sirket_degistire_bilir_mi        );
        query.SET_VALUE( ":prk_kullanabilir_mi"                 , P_YETKI_STRUCT->e9_prk_kullanabilir_mi                            );
        query.SET_VALUE( ":e9_urettigi_fisleri_grbilir_mi"      , P_YETKI_STRUCT->ynt_e9_urettigi_fisleri_grbilir_mi                );

        query.SET_VALUE(":kullanici_id"       , kullanici_id );

        query.UPDATE();

        E9_LOG_KAYDI_EKLE(E9_YONETIM , LOG_E9_YETKILENDIRME , LOG_ISLEM_UPDATE , "Kullanıcı Yetkileri");

}
/*************************************************************************************************
                                 E9_GET_SM_SICIL_NUMARASI
*************************************************************************************************/

QString E9_GET_SM_SICIL_NUMARASI()
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "e9_sabit_degerler","sm_sicil_numarasi");
    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE( "sm_sicil_numarasi" ).toString();
}

/*************************************************************************************************
                                 E9_GET_SISTEM_KULLANICI_YETKILERI
*************************************************************************************************/

void E9_GET_SISTEM_KULLANICI_YETKILERI( int p_kullanici_id, SYSTEM_MENU_ITEMS &SISTEM_STRUCT )
{
    E9_KULLANICI_YETKILERI_STRUCT_DOLDUR( p_kullanici_id );

    SISTEM_STRUCT.sis_blg_formuna_girebilir_mi          = E9_KULLANICI_YETKILERI->sis_blg_formuna_girebilir_mi;
    SISTEM_STRUCT.sis_kul_formuna_girebilir_mi          = E9_KULLANICI_YETKILERI->sis_kul_formuna_girebilir_mi;
    SISTEM_STRUCT.sis_mail_srv_tanimlayabilirmi         = E9_KULLANICI_YETKILERI->sis_mail_srv_tanimlayabilirmi;
    SISTEM_STRUCT.sis_sms_srv_tanimlayabilirmi          = E9_KULLANICI_YETKILERI->sis_sms_srv_tanimlayabilirmi;
    SISTEM_STRUCT.sis_veritabani_formuna_girebilir_mi   = E9_KULLANICI_YETKILERI->sis_veritabani_formuna_girebilir_mi;
    SISTEM_STRUCT.sis_yonetim_log_inceleme_yetkisi      = E9_KULLANICI_YETKILERI->sis_yonetim_log_inceleme_yetkisi;

}

/*************************************************************************************************
                                 E9_SET_FIRMA_ISMI
*************************************************************************************************/

void E9_SET_FIRMA_ISMI(QString p_firma_ismi)
{
    g_firma_ismi = p_firma_ismi;
}

/*************************************************************************************************
                                 E9_GET_FIRMA_ISMI
*************************************************************************************************/

QString E9_GET_FIRMA_ISMI()
{
    return g_firma_ismi;
}

/*************************************************************************************************
                                 LOAD_E9_KOMPLE_PAKET
*************************************************************************************************/

void LOAD_E9_KOMPLE_PAKET()
{
    SQL_QUERY query( DB );
    query.PREPARE_UPDATE( "e9_kullanici_yetkileri", "yetki_id",
                          "muh_kullanabilir_mi          ,"
                          "isl_kullanabilir_mi          ,"
                          "adr_kullanabilir_mi          ,"
                          "dvz_kullanabilir_mi          ,"
                          "car_kullanabilir_mi          ,"
                          "cek_kullanabilir_mi          ,"
                          "bnk_kullanabilir_mi          ,"
                          "sub_kullanabilir_mi          ,"
                          "fat_kullanabilir_mi          ,"
                          "irs_kullanabilir_mi          ,"
                          "stk_kullanabilir_mi          ,"
                          "prs_kullanabilir_mi          ,"
                          "dmr_kullanabilir_mi          ,"
                          "smm_makbuzu_kullanabilir_mi  ,"
                          "faiz_kullanabilir_mi         ,"
                          "prk_kullanabilir_mi           ",
                          "kullanici_id = :kullanici_id" );
    query.SET_VALUE     ( ":kullanici_id", KULLANICI_ID() );

    query.SET_VALUE     ( ":muh_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":isl_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":adr_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":dvz_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":car_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":cek_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":bnk_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":sub_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":fat_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":irs_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":stk_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":prs_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":dmr_kullanabilir_mi             ", 1 );
    query.SET_VALUE     ( ":smm_makbuzu_kullanabilir_mi     ", 1 );
    query.SET_VALUE     ( ":faiz_kullanabilir_mi            ", 1 );
    query.SET_VALUE     ( ":prk_kullanabilir_mi             ", 1 );

    query.UPDATE();
}
