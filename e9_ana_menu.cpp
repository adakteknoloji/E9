#include <QWebPluginFactory>
#include <QWebView>
#include <QDesktopServices>
#include <QUrl> 
#include <QAction>
#include <QSettings>
#include "sekme_menu_kernel.h"
#include "e9_console_utils.h"
#include "adak_defines.h"
#include "adak_sql.h"
#include "yonetim.h"
#include "adak_utils.h"
#include "e9_open.h"
#include "adres_open.h"
#include "banka_open.h"
#include "cari_open.h"
#include "cek_open.h"
#include "dmr_open.h"
#include "dvz_open.h"
#include "faiz_open.h"
#include "fat_struct.h"
#include "fat_open.h"
#include "irs_open.h"
#include "isl_open.h"
#include "muh_open.h"
#include "prk_open.h"
#include "prs_open.h"
#include "smm_open.h"
#include "stok_open.h"
#include "sube_open.h"
#include "e9_ana_menu_class.h"
#include "e9_yetki.h"
#include "kernel_utils.h"
#include "e9_profil_secme_batch.h"
#include "e9_kul_kitapcigi_ve_yardim_sayfasi_batch_open.h"

#define MODULE_SAYISI    16

#define ROW_BUTTON_SAYISI 4

extern ADAK_SQL_STRUCT e9_db;
extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

extern struct E9_KULLANICI_YETKILERI_STRUCT * E9_KULLANICI_YETKILERI;

/**************************************************************************************
                   OPEN_E9_ANA_MENU
***************************************************************************************/

void OPEN_E9_ANA_MENU ( QWidget * p_parent )
{
    E9_ANA_MENU * w = new E9_ANA_MENU(p_parent);
    w->showMaximized();
    ADAK_CREATE_TEMP_FILE (NULL);
    // DIGER PROGRAMLARDAN GIRIS YAPILDIGI ZAMAN PROGRAM ISMI DUZGUN GORUNSUN DIYE EKLENMISTIR
    w->setWindowTitle( VERITABANI_ISMI() + " ( " + ADAK_PROGRAM_LONGNAME( ADAK_DISPLAY_ID() ) + " )" );
}

/**************************************************************************************
                   E9_ANA_MENU::E9_ANA_MENU
***************************************************************************************/

E9_ANA_MENU::E9_ANA_MENU(QWidget * parent) : SEKME_MENU_KERNEL(E9_PROGRAMI,parent)
{

#ifdef ADRES_BAGIMSIZ
    this->setWindowIcon( QIcon( ":/adres_icons/ADRES_ICONS/adres_logo.png" ) );
#endif

    //! [ 1 ]  Eger Profil secmemis ise profil ekrani acilacak

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "e9_sabit_degerler", "program_profil_id", "sabit_deger_id = :sabit_deger_id" );
    query.SET_VALUE     ( ":sabit_deger_id", 1 );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    int profil_id = query.VALUE( 0 ).toInt();

    if ( profil_id EQ 0 ) {
        DB->START_TRANSACTION();
        LOAD_E9_KOMPLE_PAKET();

        query.PREPARE_UPDATE( "e9_sabit_degerler", "sabit_deger_id", "program_profil_id", "sabit_deger_id = :sabit_deger_id");
        query.SET_VALUE     ( ":program_profil_id", 1 ); // KOMPLA PAKETz
        query.SET_VALUE     ( ":sabit_deger_id"   , 1 );
        query.UPDATE();
        DB->COMMIT_TRANSACTION();
    }

    //! [ 1 ] end profile

    START_MENU_KERNEL ();
}

/**************************************************************************************
                   E9_ANA_MENU::~E9_ANA_MENU
***************************************************************************************/

E9_ANA_MENU::~E9_ANA_MENU()
{
    // SYS bu fonksiyonlari kullandigi icin gecici olarak eklendi
    ADAK_REMOVE_TEMP_FILES ();
    //delete ui;
}

/**************************************************************************************
                   E9_ANA_MENU::PREPARE_PROGRAM_FOR_USER
***************************************************************************************/

void E9_ANA_MENU::PREPARE_PROGRAM_FOR_USER()
{
    KULLANICI_YETKILERINI_AYARLA ();
}

/**************************************************************************************
                   E9_ANA_MENU::KULLANICI_YETKILERINI_AYARLA
***************************************************************************************/

void E9_ANA_MENU::KULLANICI_YETKILERINI_AYARLA()
{

    SYSTEM_MENU_ITEMS SISTEM_MENUSU;
    QSettings settings ( "ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()));

    E9_KULLANICI_YETKILERI_STRUCT_DOLDUR(KULLANICI_ID());

#ifdef ADRES_BAGIMSIZ
    E9_KULLANICI_YETKILERI_STRUCT * E9_KULLANICI_YETKILERI = ( E9_KULLANICI_YETKILERI_STRUCT * ) E9_GET_KULLANICI_YETKILERI();

    if ( E9_KULLANICI_YETKILERI->ynt_e9_yetkilendirme_yapabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_YNT_E9_KULLANICI_YETKILERI);
    }

    if ( E9_KULLANICI_YETKILERI->ynt_e9_log_inceleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_YNT_E9_LOG_BILGILERI );
    }
    if ( E9_KULLANICI_YETKILERI->ynt_e9_email_log_inceleme_yetkisi EQ 1 ){
         SHOW_MENU_ITEM( MENU_YNT_E9_EMAIL_LOGLARI );
    }
    if ( E9_KULLANICI_YETKILERI->adr_adres_karti_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_SAHIS_KARTI );
    }

    SHOW_MENU_ITEM ( MENU_KULLANIM_KITAPCIGI );
    SHOW_MENU_ITEM ( MENU_SIK_SORUNLAN_SORUNLAR );

    SHOW_MENU_ITEM ( MENU_YONETIM );

    E9_GET_SISTEM_KULLANICI_YETKILERI( KULLANICI_ID(), SISTEM_MENUSU );

    SISTEM_MENUSU.sis_otomatik_giris_gorunsun_mu      = 0;

    if ( QVariant ( settings.value( "is_auto_login" )).toBool() EQ true ) {
          SISTEM_MENUSU.sis_otomatik_giris_gorunsun_mu   = 1;
    }

    SISTEM_MENU_SHOWS( SISTEM_MENUSU );

#elif SM_MAKBUZU_BAGIMSIZ
    SHOW_MENU_ITEM ( MENU_SMM_E9 );

#else
    E9_KULLANICI_YETKILERI_STRUCT * E9_KULLANICI_YETKILERI = ( E9_KULLANICI_YETKILERI_STRUCT * ) E9_GET_KULLANICI_YETKILERI();
#endif

    if ( KULLANICI_ID () EQ 1 ) {

        SHOW_MENU_ITEM ( MENU_SIS_KULLANICI_DEGISTIR     );
        SHOW_MENU_ITEM ( MENU_SIS_SIFRE_DEGISTIR         );
        SHOW_MENU_ITEM ( MENU_SIS_VERITABANI_DEGISTIR    );
        SHOW_MENU_ITEM ( MENU_SIS_HAKKIMIZDA             );
        SHOW_MENU_ITEM ( MENU_YNT_E9_KULLANICI_YETKILERI );
        SHOW_MENU_ITEM ( MENU_SIS_YONETIM_LOG_BILGILERI  );
        SHOW_MENU_ITEM ( MENU_SIS_POSTA_SUNUCUSU         );
        SHOW_MENU_ITEM ( MENU_SIS_SMS_SUNUCUSU           );
        SHOW_MENU_ITEM ( MENU_SIS_KULLANICILAR           );
        SHOW_MENU_ITEM ( MENU_SIS_VERITABANLARI          );
        SHOW_MENU_ITEM ( MENU_SIS_BILGISAYARLAR          );
        SHOW_MENU_ITEM ( MENU_YONETIM                    );
        SHOW_MENU_ITEM ( MENU_YNT_E9_ONAR                );

        //! SQL LITE'ta SIFRE MENUSUNU GOSTERMEYE GEREK YOK
        //! OTOMATIK SIFRE DEVREDE ISE BIR DAHA GORUNMEYECEK
        if ( SQL_PASSWORD() NE SQL_DEFAULT_PASSWORD() AND  SQL_MOTORU() NE "YOK" ) {
            SHOW_MENU_ITEM ( MENU_SIS_OTOMATIK_SIFRE_URET );
        }
        return;
    }

    /////   ADRES

    if ( E9_KULLANICI_YETKILERI->adr_adres_karti_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_ADRES_KARTLARI_HEADER );
        SHOW_MENU_ITEM ( MENU_ADR_SAHIS_KARTI);
        SHOW_MENU_ITEM ( MENU_ADR_FIRMA_KARTI);
    }

    if ( E9_KULLANICI_YETKILERI->adr_grup_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_ADR_GRUPLAR );
    }

    if ( E9_KULLANICI_YETKILERI->adr_etiket_yazdirma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_ADR_ETIKETLERI );
    }

    if ( E9_KULLANICI_YETKILERI->adr_toplu_mail_gonderme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_TOPLU_MAIL_GONDERME );
    }

    if ( E9_KULLANICI_YETKILERI->adr_toplu_sms_gonderme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_ADR_TOPLU_SMS_GONDERME );
    }

    if ( E9_KULLANICI_YETKILERI->adr_adres_kayitlari_rapor_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_ADR_KAYITLARI );
    }
    if ( E9_KULLANICI_YETKILERI->adr_tel_web_kayitlari_rapor_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_ADR_TEL_WEB_KAYITLARI);
    }

    if ( E9_KULLANICI_YETKILERI->adr_fihrist_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_ADR_FIHRIST  );
    }

    if ( E9_KULLANICI_YETKILERI->adr_export_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_ADR_EXPORT );
    }

    if ( E9_KULLANICI_YETKILERI->adr_import_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADR_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_ADR_IMPORT );
    }

    ////// BANKA

    if ( E9_KULLANICI_YETKILERI->bnk_hareket_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BNK_FISLER_HEADER );
        SHOW_MENU_ITEM ( MENU_BNK_HAREKET_FISI );
        SHOW_MENU_ITEM ( MENU_BNK_PARA_YAT_CEKME_FISI );
    }

    if ( E9_KULLANICI_YETKILERI->bnk_acilis_fisi_islem_yetkisi  EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BNK_FISLER_HEADER );
        SHOW_MENU_ITEM ( MENU_BNK_ACILIS_FISI );
    }

    if ( E9_KULLANICI_YETKILERI->bnk_banka_hesabi_acma_yetkisi  EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BNK_ISLEMLER_HEADER );
        SHOW_MENU_ITEM ( MENU_BNK_HESAPLARI );
    }

    if ( E9_KULLANICI_YETKILERI->bnk_ent_fisleri_inceleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BNK_ENT_FISLER_HEADER );
        SHOW_MENU_ITEM ( MENU_BNK_ENT_FISLERI       );
    }

    if ( E9_KULLANICI_YETKILERI->bnk_defter_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BNK_RAPORLAR_HEADER   );
        SHOW_MENU_ITEM ( MENU_BNK_DEFTERI           );
    }

    if ( E9_KULLANICI_YETKILERI->bnk_pos_ekstresi_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BNK_RAPORLAR_HEADER   );
        SHOW_MENU_ITEM ( MENU_BNK_POS_EKSTRESI      );
    }

    if ( E9_KULLANICI_YETKILERI->bnk_makro_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BNK_ISLEMLER_HEADER   );
        SHOW_MENU_ITEM ( MENU_BNK_MAKRO_TANIMLAMA   );
    }

    if ( E9_KULLANICI_YETKILERI->bnk_makro_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BNK_FISLER_HEADER     );
        SHOW_MENU_ITEM ( MENU_BNK_MAKRO_FISI        );
    }

    if ( E9_KULLANICI_YETKILERI->bnk_kk_ekstresi_alma_yetkisi EQ  1 ){
        SHOW_MENU_ITEM ( MENU_BNK_RAPORLAR_HEADER   );
        SHOW_MENU_ITEM ( MENU_BNK_KK_EKSTRESI       );
    }

    if ( E9_KULLANICI_YETKILERI->bnk_virman_islem_yetkisi EQ  1 ){
        SHOW_MENU_ITEM ( MENU_BNK_FISLER_HEADER     );
        SHOW_MENU_ITEM ( MENU_BNK_VIRMAN_FORMU      );
    }

    //CARI
    if ( E9_KULLANICI_YETKILERI->cari_tahsilat_fisi_islem_yetkisi EQ 1 ) {    
        SHOW_MENU_ITEM ( MENU_CARI_FISLER_HEADER    );
        SHOW_MENU_ITEM ( MENU_CARI_TAHSILAT_FISI    );
    }
    if ( E9_KULLANICI_YETKILERI->cari_odeme_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI_FISLER_HEADER    );
        SHOW_MENU_ITEM ( MENU_CARI_ODEME_FISI       );
    }
    if ( E9_KULLANICI_YETKILERI->cari_hareket_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI_FISLER_HEADER    );
        SHOW_MENU_ITEM ( MENU_CARI_HAREKET_FISI     );
    }
    if ( E9_KULLANICI_YETKILERI->cari_acilis_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI_FISLER_HEADER    );
        SHOW_MENU_ITEM ( MENU_CARI_ACILIS_FISI      );
    }
    if ( E9_KULLANICI_YETKILERI->cari_grup_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI_KARTLAR_HEADER   );
        SHOW_MENU_ITEM ( MENU_CARI_GRUPLAR          );
    }
    if ( E9_KULLANICI_YETKILERI->cari_kart_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI_KARTLAR_HEADER   );
        SHOW_MENU_ITEM ( MENU_CARI_FIRMA_KARTI      );
        SHOW_MENU_ITEM ( MENU_CARI_SAHIS_KARTI      );
    }
    if ( E9_KULLANICI_YETKILERI->cari_bakiye_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI_RAPORLAR_HEADER  );
        SHOW_MENU_ITEM ( MENU_CARI_BAKIYE_RAPORU    );
    }
    if ( E9_KULLANICI_YETKILERI->cari_ekstre_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI_RAPORLAR_HEADER  );
        SHOW_MENU_ITEM ( MENU_CARI_HESAP_EKSTRESI   );
    }
    if ( E9_KULLANICI_YETKILERI->cari_virman_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI_VIRMAN_FISI      );
    }
    if ( E9_KULLANICI_YETKILERI->cari_ent_fisleri_inceleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI_ENT_FISLERI_HEADER   );
        SHOW_MENU_ITEM ( MENU_CARI_ENT_FISLERI          );
    }

    SHOW_MENU_ITEM ( MENU_CARI_ISLEMLER_HEADER       );
    SHOW_MENU_ITEM ( MENU_CARI_YAZICIDAKI_BLG_NOLARI );
    SHOW_MENU_ITEM ( MENU_CARI_KART_EXPORT           );
    SHOW_MENU_ITEM ( MENU_CARI_KART_IMPORT           );


    //CEK
    if ( E9_KULLANICI_YETKILERI->cek_senet_girebilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CEK_ISLEMLER_HEADER );
        SHOW_MENU_ITEM ( MENU_CEK_KENDI_CEKLERIMIZ      );
        SHOW_MENU_ITEM ( MENU_CEK_KENDI_SENETLERIMIZ    );
        SHOW_MENU_ITEM ( MENU_CEK_MUS_CEKLERI_SENETLERI );

    }

    if ( E9_KULLANICI_YETKILERI->cek_mus_cek_senet_arama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CEK_RAPORLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_CEK_MUS_CEK_ARAMA   );
        SHOW_MENU_ITEM ( MENU_CEK_MUS_SNT_ARAMA );

    }

    if ( E9_KULLANICI_YETKILERI->cek_listesi_rapor_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CEK_RAPORLAR_HEADER );
        SHOW_MENU_ITEM( MENU_CEK_SENET_LISTESI );
    }

    if ( E9_KULLANICI_YETKILERI->cek_bordrosu_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CIKIS_BRD_HEADER);
        SHOW_MENU_ITEM ( MENU_MUSTERI_CEK_BORDROSU);
        SHOW_MENU_ITEM ( MENU_FIRMA_CEK_BORDROSU );

        SHOW_MENU_ITEM ( MENU_GIRIS_BRD_HEADER );
        SHOW_MENU_ITEM ( MENU_MUSTERI_SENET_BORDROSU );
        SHOW_MENU_ITEM ( MENU_FIRMA_SENET_BORDROSU );

        SHOW_MENU_ITEM ( MENU_CEK_MUS_SNT_BRD_TAH_EDILEMEYEN );
    }

    if ( E9_KULLANICI_YETKILERI->cek_acilis_bordrosu_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CEK_ACILIS_BORDROSU_HEADER );
        SHOW_MENU_ITEM ( MENU_CEK_ACILIS_BORDROSU );

    }

    if ( E9_KULLANICI_YETKILERI->cek_ent_fisleri_inceleme_yetkisi EQ 1 ) {

        SHOW_MENU_ITEM ( MENU_CEK_ENT_FISLERI_HEADER );
        SHOW_MENU_ITEM ( MENU_CEK_CARI_ENT_FISILERI );
    }

    //DEMIRBAS
    if ( E9_KULLANICI_YETKILERI->dmr_amortisman_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_DMR_FISLER_HEADER);
        SHOW_MENU_ITEM ( MENU_DMR_AMORTISMAN_FISI);

    }
    if ( E9_KULLANICI_YETKILERI->dmr_toplu_amortisman_ayirma_yetkisi EQ 1){
        SHOW_MENU_ITEM ( MENU_DMR_ISLEMLER_HEADER );
        SHOW_MENU_ITEM ( MENU_DMR_TOPLU_AMORTISMAN );
    }

    if ( E9_KULLANICI_YETKILERI->dmr_yen_degerleme_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_DMR_FISLER_HEADER);
        SHOW_MENU_ITEM ( MENU_DMR_YEN_DEGERLEME_FISI );
    }

    if ( E9_KULLANICI_YETKILERI->dmr_toplu_yeniden_degerleme_yetkisi EQ 1) {
         SHOW_MENU_ITEM ( MENU_DMR_ISLEMLER_HEADER );
         SHOW_MENU_ITEM ( MENU_DMR_TOPLU_YEN_DEGERLEME );
    }
    if ( E9_KULLANICI_YETKILERI->dmr_grup_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_DMR_TANIMLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_DMR_GRUPLARI );
    }
    if ( E9_KULLANICI_YETKILERI->dmr_rapor_islem_grubu_tanimlama_yetkisi EQ 1 ){
        SHOW_MENU_ITEM ( MENU_DMR_TANIMLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_DMR_RAPOR_ISLEM_GRUPLARI);
    }
    if ( E9_KULLANICI_YETKILERI->dmr_demirbas_karti_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_DMR_TANIMLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_DMR_KAYDI);
        SHOW_MENU_ITEM ( MENU_DMR_SATISI);
    }
    if ( E9_KULLANICI_YETKILERI->dmr_demirbas_listesi_rapor_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_DMR_RAPORLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_DMR_LISTESI);
    }

    //DOVIZ
    SHOW_MENU_ITEM ( MENU_DVZ_ISLEM_HEADER );
    if ( E9_KULLANICI_YETKILERI->dvz_dovizleri_inceleme_yetkisi EQ 1 ){
        SHOW_MENU_ITEM ( MENU_DVZ_DOVIZLER );
    }
    if ( E9_KULLANICI_YETKILERI->dvz_kur_guncelleme_yetkisi EQ 1 ){
        SHOW_MENU_ITEM ( MENU_DVZ_KURLARI_GUNCELLE  );
    }
    if  ( E9_KULLANICI_YETKILERI->dvz_doviz_tanimlama_yetkisi EQ 1){
        SHOW_MENU_ITEM ( MENU_DVZ_TANIMLARI   );
    }

    //FAIZ YETKILENDIRME YOK
    if ( E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_FAIZ_ISLEM_HEADER );
        SHOW_MENU_ITEM ( MENU_FAIZ_ANA_PARAYI_BUL );
        SHOW_MENU_ITEM ( MENU_FAIZ_AYLIK_FAIZ_ORANI_BUL);
        SHOW_MENU_ITEM ( MENU_FAIZ_TAKSITLENDIR );
    }

    //FATURA
     SHOW_MENU_ITEM ( MENU_FAT_YAZICIDAKI_BELGE_NOLARI );
    if ( E9_KULLANICI_YETKILERI->fat_alis_faturasi_kesebilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_FAT_ALIS_FAT_HEADER);
        SHOW_MENU_ITEM ( MENU_FAT_ALIS_FATURALARI );
        SHOW_MENU_ITEM ( MENU_FAT_SATISTAN_IADE   );
    }

    if ( E9_KULLANICI_YETKILERI->fat_sts_faturasi_kesebilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_FAT_SATIS_FAT_HEADER );
        SHOW_MENU_ITEM ( MENU_FAT_SATIS_FATURALARI );
        SHOW_MENU_ITEM ( MENU_FAT_ALISTAN_IADE     );
    }

    if ( E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_toplu EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_FAT_TOPLU_IRS_FATURALASTIRMA );
    }
    if ( E9_KULLANICI_YETKILERI->fat_hizmet_ekstresi_alma_yetkisi EQ 1 ) {

        SHOW_MENU_ITEM ( MENU_FAT_HIZMET_EKSTRESI );
    }

    if ( E9_KULLANICI_YETKILERI->fat_ba_bs_listesi_alma_yetkisi  EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_FAT_RAPORLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_FAT_BA_BS_FORMU );
    }

    if ( E9_KULLANICI_YETKILERI->fat_fatura_listesi_alma_yetkisi EQ 1 ){
        SHOW_MENU_ITEM ( MENU_FAT_RAPORLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_FAT_LISTESI );
    }

    if ( E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_elle EQ 1 ) {

        SHOW_MENU_ITEM ( MENU_FAT_SATIS_FAT_ISLEM_HEADER);
        SHOW_MENU_ITEM ( MENU_FAT_ALIS_FAT_ISLEM_HEADER);

        SHOW_MENU_ITEM ( MENU_FAT_SATIS_ELLE_IRS_FATURALASTIRMA );
        SHOW_MENU_ITEM ( MENU_FAT_ALIS_ELLE_IRS_FATURALASTIRMA );
    }

    if ( E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_oto EQ 1 ) {

        SHOW_MENU_ITEM ( MENU_FAT_ISLEM_HEADER);
        SHOW_MENU_ITEM ( MENU_FAT_SATIS_FAT_ISLEM_HEADER);
        SHOW_MENU_ITEM ( MENU_FAT_ALIS_FAT_ISLEM_HEADER);

        SHOW_MENU_ITEM ( MENU_FAT_SATIS_OTOMATIK_IRS_FATURALASTIRMA );
        SHOW_MENU_ITEM ( MENU_FAT_SATIS_ELLE_IRS_FATURALASTIRMA_KONSINYE );

        SHOW_MENU_ITEM ( MENU_FAT_ALIS_OTOMATIK_IRS_FATURALASTIRMA );
        SHOW_MENU_ITEM ( MENU_FAT_ALIS_ELLE_IRS_FATURALASTIRMA_KONSINYE );
    }


    //İRSALİYE
    if ( E9_KULLANICI_YETKILERI->irs_alis_irsaliyesi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_IRS_ALIS_IRS_HEADER );
        SHOW_MENU_ITEM ( MENU_IRS_ALIS_IRSALIYELERI );
        SHOW_MENU_ITEM ( MENU_IRS_SATISTAN_IADE );
        SHOW_MENU_ITEM ( MENU_IRS_FASON_BASLANGIC_GIRIS);
        SHOW_MENU_ITEM ( MENU_IRS_FASON_BITIS_GIRIS );

    }

    if ( E9_KULLANICI_YETKILERI->irs_sts_irsaliyesi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_IRS_SATIS_IRS_HEADER );
        SHOW_MENU_ITEM ( MENU_IRS_SATIS_IRSALIYELERI );
        SHOW_MENU_ITEM ( MENU_IRS_ALISTAN_IADE );
        SHOW_MENU_ITEM ( MENU_IRS_FASON_BASLANGIC_CIKIS );
        SHOW_MENU_ITEM ( MENU_IRS_FASON_BITIS_CIKIS );
    }

    if ( E9_KULLANICI_YETKILERI->irs_listesi_gorebilir_mi  EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_IRS_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_IRS_LISTESI );
    }
    SHOW_MENU_ITEM ( MENU_IRS_YAZICIDAKI_BELGE_NOLARI );

    if ( E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_elle EQ 1 ) {

        SHOW_MENU_ITEM ( MENU_IRS_ISLEM_HEADER );
        SHOW_MENU_ITEM ( MENU_IRS_SATIS_IRS_ISLEM_HEADER );
        SHOW_MENU_ITEM ( MENU_IRS_ALIS_IRS_ISLEM_HEADER );
        SHOW_MENU_ITEM ( MENU_IRS_SATIS_ELLE_FAT_IRSALIYELESTIRME );
        SHOW_MENU_ITEM ( MENU_IRS_ALIS_ELLE_FAT_IRSALIYELESTIRME  );
    }

    if ( E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_oto EQ 1 ) {

        SHOW_MENU_ITEM ( MENU_IRS_ISLEM_HEADER );
        SHOW_MENU_ITEM ( MENU_IRS_SATIS_IRS_ISLEM_HEADER );
        SHOW_MENU_ITEM ( MENU_IRS_ALIS_IRS_ISLEM_HEADER );

        SHOW_MENU_ITEM ( MENU_IRS_ALIS_OTOMATIK_FAT_IRSALIYELESTIRME );
        SHOW_MENU_ITEM ( MENU_IRS_SATIS_OTOMATIK_FAT_IRSALIYELESTIRME );
    }

    //ISLETME
    if ( E9_KULLANICI_YETKILERI->isl_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ISL_FISLER_HEADER );
        SHOW_MENU_ITEM ( MENU_ISL_GELIR_FISI );
        SHOW_MENU_ITEM ( MENU_ISL_GIDER_FISI );
    }

    if ( E9_KULLANICI_YETKILERI->isl_isletme_defteri_rapor_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ISL_RAPORLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_ISL_DEFTERI_RAPORU );
    }

    if ( E9_KULLANICI_YETKILERI->isl_kdv_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ISL_RAPORLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_ISL_KDV_RAPORU );
    }


    SHOW_MENU_ITEM ( MENU_ISL_RAPORLAR_HEADER );
    SHOW_MENU_ITEM ( MENU_ISL_HESAP_OZETI );

    //MUHASEBE
    if ( E9_KULLANICI_YETKILERI->muh_muhasebe_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_FISLER_HEADER  );
        SHOW_MENU_ITEM ( MENU_MUH_MAHSUP_FISI    );
        SHOW_MENU_ITEM ( MENU_MUH_TAHSIL_FISI    );
        SHOW_MENU_ITEM ( MENU_MUH_TEDIYE_FISI    );
        SHOW_MENU_ITEM ( MENU_MUH_KAPANIS_FISI );
    }

    if ( E9_KULLANICI_YETKILERI->muh_acilis_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_FISLER_HEADER );
        SHOW_MENU_ITEM ( MENU_MUH_ACILIS_FISI  );
    }

    if ( E9_KULLANICI_YETKILERI->muh_hesap_plani_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM (MENU_MUH_HESAPLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_HESAP_PLANI );
    }

    if ( E9_KULLANICI_YETKILERI->muh_hesap_ekstresi_alma_yetkisi EQ 1 ) {
         SHOW_MENU_ITEM ( MENU_MUH_RAPORLAR_HEADER);
         SHOW_MENU_ITEM ( MENU_MUH_HESAP_EKSTRESI );
    }

    if ( E9_KULLANICI_YETKILERI->muh_mizan_raporu_alma_yetkisi EQ 1 ) {
         SHOW_MENU_ITEM ( MENU_MUH_RAPORLAR_HEADER);
         SHOW_MENU_ITEM ( MENU_MUH_MIZAN );
    }

    if ( E9_KULLANICI_YETKILERI->muh_hesap_plani_rapor_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_HESAP_PLANI_RAPORU );
    }

    if ( E9_KULLANICI_YETKILERI->muh_bilanco_rapor_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_BILANCO );
    }

    if ( E9_KULLANICI_YETKILERI->muh_defteri_kebir_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_DEFTERI_KEBIR );
    }

    if ( E9_KULLANICI_YETKILERI->muh_gelir_tablosu_rapor_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_GELIR_TABLOSU );
    }

    if ( E9_KULLANICI_YETKILERI->muh_yvm_dokumu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_YEVMIYE_DOKUMU );
    }

    if ( E9_KULLANICI_YETKILERI->muh_hatali_fisleri_inceleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_HATALI_FISLER );
    }

    if ( E9_KULLANICI_YETKILERI->muh_fis_dokumu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_TOPLU_FIS_DOKUMU );
    }

    if ( E9_KULLANICI_YETKILERI->muh_grup_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM (MENU_MUH_HESAPLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_HESAP_GRUPLARI );
    }

    if ( E9_KULLANICI_YETKILERI->muh_yansitma_fisi_olusturma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_YANSITMA_FISI_OLUSTUR );
    }

    if ( E9_KULLANICI_YETKILERI->muh_yvm_no_olusturma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_YVM_NO_OLUSTUR );
    }

    if ( E9_KULLANICI_YETKILERI->muh_yansitma_hesabi_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_YANSITMA_HESAPLARI);
    }

    if ( E9_KULLANICI_YETKILERI->muh_hesap_plani_kopyalama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_MUH_HESAP_PLANI_KOPYALA );
    }

    if ( E9_KULLANICI_YETKILERI->muh_ent_fisleri_inceleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUH_ENT_FISLERI_HEADER );
        SHOW_MENU_ITEM ( MENU_MUH_ENT_FISLERI );
    }

    //PERAKENDE
    if ( E9_KULLANICI_YETKILERI->prk_fis_girebilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRK_FISLER_HEADER);
        SHOW_MENU_ITEM ( MENU_PRK_ALIS_FISI );
        SHOW_MENU_ITEM ( MENU_PRK_SATIS_FISI);
    }
    if ( E9_KULLANICI_YETKILERI->prk_gdr_tur_tanimlayabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRK_TANIMLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRK_GIDER_TURLERI );
        SHOW_MENU_ITEM ( MENU_PRK_GELIR_TURLERI );
    }
    if ( E9_KULLANICI_YETKILERI->prk_kasa_raporu_alabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRK_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRK_KASA_RAPORU );
    }

    //PERSONEL
    SHOW_MENU_ITEM( MENU_PRS_KARTI_IMPORT );
    SHOW_MENU_ITEM( MENU_PRS_KARTI_EXPORT );
    if ( E9_KULLANICI_YETKILERI->prs_eksik_gun_nedenleri_giris_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_EKSIK_GUN_NEDENLERI);
    }

    if ( E9_KULLANICI_YETKILERI->prs_ucret_pusulasi_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_UCRET_PUSULASI );
    }

    if ( E9_KULLANICI_YETKILERI->prs_sicil_karti_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_TANIMLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_SICIL_KARTI_TANIMLAMA );
    }

    if ( E9_KULLANICI_YETKILERI->prs_ek_odenek_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_TANIMLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_EK_ODENEK_TANIMLAMA );
    }

    if ( E9_KULLANICI_YETKILERI->prs_maas_bordrosu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_MAAS_BORDROSU );
    }

    if ( E9_KULLANICI_YETKILERI-> prs_ek_kesinti_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_TANIMLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_EK_KESINTI_TANIMLAMA );
    }

    if ( E9_KULLANICI_YETKILERI->prs_sabit_deger_guncelleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_TANIMLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_SABIT_DEGERLER );
    }

    if ( E9_KULLANICI_YETKILERI->prs_maas_bordrosu_iptal_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_MAAS_BORDROSU_IPTAL );
    }

    if ( E9_KULLANICI_YETKILERI->prs_personel_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_PERSONEL_RAPORU );
    }

    if ( E9_KULLANICI_YETKILERI->prs_isten_ayrilan_personel_arama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_ISTEN_AYRILAN_ARAMA );
    }

    if ( E9_KULLANICI_YETKILERI->prs_maas_bordrosu_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_MAAS_BORDROSU_RAPORU );
    }

    if ( E9_KULLANICI_YETKILERI->prs_isten_ayrilma_islem_yetkisi EQ 1 ) {
         SHOW_MENU_ITEM ( MENU_PRS_ISLEMLER_HEADER);
         SHOW_MENU_ITEM ( MENU_PRS_ISTEN_AYRILMA );
    }

    if ( E9_KULLANICI_YETKILERI->prs_e_bildirge_alma_yetkisi EQ 1) {
        SHOW_MENU_ITEM ( MENU_PRS_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_EBILDIRGE);
    }

    if ( E9_KULLANICI_YETKILERI->prs_ihbar_ve_kidem_tazminati_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_IHBAR_VE_KIDEM_TAZMINATI_RAPORU );
    }

    if ( E9_KULLANICI_YETKILERI->prs_bordro_inceleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_ISLEMLER_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_BORDRO_INCELEME );
    }

    if ( E9_KULLANICI_YETKILERI->prs_grup_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PRS_TANIMLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_PRS_GRUPLAR );
    }

    if ( E9_KULLANICI_YETKILERI->smm_makbuzu_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_SMM_FISLER_HEADER );
        SHOW_MENU_ITEM ( MENU_SMM_MAKBUZ_ALIS);
        SHOW_MENU_ITEM ( MENU_SMM_MAKBUZ_SATIS);
    }

    if ( E9_KULLANICI_YETKILERI->smm_hizmet_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_SMM_TANIMLAR_HEADER   );
        SHOW_MENU_ITEM ( MENU_SMM_ALINAN_HIZMETLER  );
        SHOW_MENU_ITEM ( MENU_SMM_SATILAN_HIZMETLER );
    }

    //STOK
    SHOW_MENU_ITEM( MENU_STOK_ISLEMLER_HEADER );
    SHOW_MENU_ITEM( MENU_STOK_URUN_IMPORT     );
    SHOW_MENU_ITEM( MENU_STOK_URUN_EXPORT     );
    if ( E9_KULLANICI_YETKILERI->stok_grup_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_STOK_TANIMLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_STOK_GRUPLARI        );
    }

    if ( E9_KULLANICI_YETKILERI->stok_hizmet_karti_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_STOK_TANIMLAR_HEADER );
        SHOW_MENU_ITEM ( MENU_STOK_HIZMET_KARTI    );
    }

    if ( E9_KULLANICI_YETKILERI->stok_fisi_islem_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_STOK_FISLER_HEADER );
        SHOW_MENU_ITEM ( MENU_STOK_ACILIS_FISI   );
        SHOW_MENU_ITEM ( MENU_STOK_SARF_FISI     );
        SHOW_MENU_ITEM ( MENU_STOK_IMALATA_SEVK_FISI);
        SHOW_MENU_ITEM ( MENU_STOK_IMALATTAN_DONUS_FISI);
        SHOW_MENU_ITEM ( MENU_STOK_DEPO_TRANSFER_GIRIS_FISI );
        SHOW_MENU_ITEM ( MENU_STOK_DEPO_TRANSFER_CIKIS_FISI );
    }
    if ( E9_KULLANICI_YETKILERI->stok_urun_karti_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_STOK_TANIMLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_STOK_URUN_KARTI );
    }

    if ( E9_KULLANICI_YETKILERI->stok_ekstresi_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_STOK_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_STOK_EKSTRESI );
    }
    if ( E9_KULLANICI_YETKILERI->stok_miktar_raporu_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_STOK_RAPORLAR_HEADER);
        SHOW_MENU_ITEM ( MENU_STOK_MIKTARLARI_RAPORU );
    }
    if ( E9_KULLANICI_YETKILERI->stok_urun_listesi_alma_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_STOK_RAPORLAR_HEADER );
        SHOW_MENU_ITEM( MENU_STOK_URUN_LISTESI );
        SHOW_MENU_ITEM( MENU_STOK_HIZMET_LISTESI );
    }

    //SUBE
    if ( E9_KULLANICI_YETKILERI->sub_sube_tanimlama_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_SUBE_SUBELER        );
        SHOW_MENU_ITEM ( MENU_SUBE_ORGANIZASYON_SEMASI );
    }

    //MODUL YETKILERI
    if ( E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi EQ 1 ){
        SHOW_MENU_ITEM(MENU_SMM_MAKBUZ);
    }

    if ( E9_KULLANICI_YETKILERI->e9_muh_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_MUHASEBE );
    }

    if ( E9_KULLANICI_YETKILERI->e9_isl_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ISLETME );
    }

    if ( E9_KULLANICI_YETKILERI->e9_adr_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_ADRES );
    }

    if ( E9_KULLANICI_YETKILERI->e9_dvz_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_DOVIZ );
    }

    if ( E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CARI );
    }

    if ( E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_CEK );
    }

    if ( E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BANKA );
    }

    if ( E9_KULLANICI_YETKILERI->e9_sub_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_SUBE );
    }

    if ( E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_FATURA );
    }

    if ( E9_KULLANICI_YETKILERI->e9_irs_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_IRSALIYE );
    }

    if ( E9_KULLANICI_YETKILERI->e9_stk_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_STOK );
    }

    if ( E9_KULLANICI_YETKILERI->e9_prs_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PERSONEL );
    }

    if ( E9_KULLANICI_YETKILERI->e9_dmr_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_DEMIRBAS );
    }

    if ( E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_FAIZ );
    }

    if ( E9_KULLANICI_YETKILERI->e9_prk_kullanabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_PERAKENDE);
    }

    //YONETIM  E9

    // YETKI YAZILCAK MI
    SHOW_MENU_ITEM( MENU_YNT_E9_PROFIL_SECME );

    if ( E9_KULLANICI_YETKILERI->ynt_e9_onar_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_YNT_E9_ONAR);
    }

    if ( E9_KULLANICI_YETKILERI->ynt_e9_devir_islemi_yapabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_YNT_E9_DEVIR_ISLEMI);
    }

    if ( E9_KULLANICI_YETKILERI->ynt_e9_log_inceleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_YNT_E9_LOG_BILGILERI);
    }

    if ( E9_KULLANICI_YETKILERI->ynt_e9_sabit_degerler_guncelleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_YNT_E9_SABIT_DEGERLER);
    }

    if ( E9_KULLANICI_YETKILERI->ynt_e9_belge_tasarlama_yetkisi EQ 1) {
        SHOW_MENU_ITEM ( MENU_YNT_E9_BELGE_TASARIMCISI );
    }

    if ( E9_KULLANICI_YETKILERI->ynt_e9_yetkilendirme_yapabilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_YNT_E9_KULLANICI_YETKILERI );
    }

    if ( E9_KULLANICI_YETKILERI->ynt_e9_email_log_inceleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_YNT_E9_EMAIL_LOGLARI );
    }

    SHOW_MENU_ITEM ( MENU_YONETIM );

    SHOW_MENU_ITEM ( MENU_KULLANIM_KITAPCIGI );
    SHOW_MENU_ITEM ( MENU_SIK_SORUNLAN_SORUNLAR );
    SHOW_MENU_ITEM ( MENU_HATA_ISTEK );

    E9_GET_SISTEM_KULLANICI_YETKILERI( KULLANICI_ID(), SISTEM_MENUSU );

    SISTEM_MENUSU.sis_otomatik_giris_gorunsun_mu      = 0;

    if ( QVariant ( settings.value( "is_auto_login" )).toBool() EQ true ) {
          SISTEM_MENUSU.sis_otomatik_giris_gorunsun_mu   = 1;
    }

    SISTEM_MENU_SHOWS( SISTEM_MENUSU );
}

/**************************************************************************************
                   E9_ANA_MENU::MENU_ITEM_CLICKED
***************************************************************************************/

MENU_STRUCT * E9_ANA_MENU::GET_MENU_STRUCT(int parent_enum_id)
{

    switch ( parent_enum_id ) {

        case MENU_YONETIM :
            return YONETIM_GET_MENU_STRUCT();

#ifdef  ADRES_BAGIMSIZ
        case MENU_ROOT:
            return ADRES_GET_MENU_STRUCT();
#else

        case MENU_MUHASEBE :
            return MUHASEBE_GET_MENU_STRUCT();

        case MENU_CARI:
            return CARI_GET_MENU_STRUCT();

        case MENU_FATURA:
            return FATURA_GET_MENU_STRUCT();

        case MENU_PERSONEL:
            return PERSONEL_GET_MENU_STRUCT();

        case MENU_ISLETME:
            return ISLETME_GET_MENU_STRUCT();

        case MENU_PERAKENDE :
            return PERAKENDE_GET_MENU_STRUCT();

        case MENU_CEK:
            return CEKSENET_GET_MENU_STRUCT();

        case MENU_IRSALIYE:
            return IRSALIYE_GET_MENU_STRUCT();

        case MENU_DEMIRBAS:
            return DEMIRBAS_GET_MENU_STRUCT();

        case MENU_DOVIZ:
            return DOVIZ_GET_MENU_STRUCT();

        case MENU_BANKA:
            return BANKA_GET_MENU_STRUCT();

        case MENU_STOK:
            return STOK_GET_MENU_STRUCT();

        case MENU_FAIZ:
            return FAIZ_GET_MENU_STRUCT();

        case MENU_ADRES:
            return ADRES_GET_MENU_STRUCT();

        case MENU_SUBE:
            return SUBE_GET_MENU_STRUCT();

        case MENU_SMM_MAKBUZ:
            return SMM_GET_MENU_STRUCT();

        case MENU_ROOT:
            return E9_GET_MENU_STRUCT();
#endif
        default:
            return NULL;

    }


}

/**************************************************************************************
                   E9_ANA_MENU::MENU_ITEM_CLICKED
***************************************************************************************/

void E9_ANA_MENU::MENU_ITEM_CLICKED (int parent_item_enum, int menu_item_enum)
{
    switch ( parent_item_enum ) {
        case MENU_ROOT :
        default:
            if (menu_item_enum EQ MENU_HATA_ISTEK ) {
                OPEN_HATA_ISTEK_BATCH( this );
            }
            else if ( menu_item_enum EQ MENU_KULLANIM_KITAPCIGI ) {
                OPEN_E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH( 1, this );
            }
            else if ( menu_item_enum EQ MENU_SIK_SORUNLAN_SORUNLAR ) {
                OPEN_E9_KUL_KITAPCIGI_VE_YARDIM_SAYFASI_BATCH( 0, this );
            }

#if defined ( ADRES_BAGIMSIZ )
    #ifdef ADRES_BAGIMSIZ
            case MENU_ADRES:
                ADRES_ITEM_CLICKED(menu_item_enum, this);
                break;
    #endif
#else
            break;

        case MENU_MUHASEBE :
            MUHASEBE_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_CARI:
            CARI_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_FATURA:
            FATURA_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_PERSONEL:
            PERSONEL_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_ISLETME:
            ISLETME_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_PERAKENDE :
            PERAKENDE_ITEM_CLICKED( menu_item_enum, this);
            break;

        case MENU_CEK:
            CEKSENET_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_IRSALIYE:
            IRSALIYE_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_DEMIRBAS:
            DEMIRBAS_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_DOVIZ:
            DOVIZ_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_BANKA:
            BANKA_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_STOK:
            STOK_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_FAIZ:
            FAIZ_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_ADRES:
            ADRES_ITEM_CLICKED(menu_item_enum,this);
            break;

        case MENU_SMM_MAKBUZ:
            SMM_ITEM_CLICKED( menu_item_enum ,this);
            break;

        case MENU_SUBE :
                SUBE_ITEM_CLICKED(menu_item_enum, this );
            break;
#endif
        case MENU_YONETIM :
            YONETIM_ITEM_CLICKED ( menu_item_enum,this);
            break;
    }

}
