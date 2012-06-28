#include "yonetim.h"
#include "e9_kullanici_yetkileri_formu_class.h"
#include "ui_e9_kullanici_yetkileri_formu.h"
#include "sube_gui_utils.h"
#include "e9_log.h"
#include "e9_gui_utils.h"
#include "e9_console_utils.h"
#include "sube_console_utils.h"
#include "sube_enum.h"
#include "e9_yetki.h"
#include "e9_profil_secme_batch.h"
#include "adak_std_utils.h"


extern ADAK_SQL *   DB;
extern ADAK_SQL *   G_YONETIM_DB;

/**************************************************************************************
                   OPEN_E9_KULLANICI_YETKILERI_FORMU
***************************************************************************************/

void OPEN_E9_KULLANICI_YETKILERI_FORMU ( int p_kullanici_id, bool * yetkileri_tazele, QWidget * parent)
{
    E9_KULLANICI_YETKILERI_FORMU * F = new E9_KULLANICI_YETKILERI_FORMU ( p_kullanici_id, yetkileri_tazele, parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::E9_KULLANICI_YETKILERI_FORMU
***************************************************************************************/

E9_KULLANICI_YETKILERI_FORMU::E9_KULLANICI_YETKILERI_FORMU ( int p_kullanici_id, bool * yetkileri_tazele, QWidget * parent ) : FORM_KERNEL(parent), m_ui(new Ui::E9_KULLANICI_YETKILERI_FORMU)
{
    m_kullanici_id      = p_kullanici_id;
    m_yetkileri_tazele  = yetkileri_tazele;

    m_ui->setupUi     ( this );
    START_FORM_KERNEL ( this, DB  );
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::SETUP_FORM
***************************************************************************************/

void E9_KULLANICI_YETKILERI_FORMU::SETUP_FORM()
{
    SET_HELP_PAGE( "e9_kullanici_yetkileri_formu" );
    SET_PAGE_TITLE( tr ( "KULLANICI YETKİLERİ" )   );

    SET_SETTING_NAME( "E9_KULLANICI_YETKILERI_FORMU" );

    E9_FILL_KULLANICININ_CALISTIGI_SIRKETLER( m_ui->yonetim_comboBox_e9_kullancinin_calistigi_sirket );
     
    SET_FIRST_FOCUS_WIDGET( m_ui->tabWidget_yetkiler );

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "e9_kullanici_yetkileri", "yetki_id", " kullanici_id = :kullanici_id" );
    query.SET_VALUE     ( ":kullanici_id", m_kullanici_id );

    if ( query.SELECT() EQ 0 ) {
        //  EGER KULLANICI YOK ISE OLUSTURULUYOR
        SQL_QUERY s_query( DB );
        DB->START_TRANSACTION();
        s_query.PREPARE_INSERT( "e9_kullanici_yetkileri", "yetki_id", "kullanici_id" );
        s_query.SET_VALUE     ( ":kullanici_id", m_kullanici_id );
        s_query.INSERT();
        DB->COMMIT_TRANSACTION();
    }

    query.PREPARE_SELECT( "e9_kullanici_yetkileri", "yetki_id", " kullanici_id = :kullanici_id" );
    query.SET_VALUE     ( ":kullanici_id", m_kullanici_id );

    query.SELECT();
    query.NEXT();

    m_yetki_id = query.VALUE( 0 ).toInt();

    REGISTER_CHANGER_BUTTON                     ( m_ui->pushButton_profil_secin );

    SET_SINGLE_RECORD_ID                        ( m_yetki_id                    );

    REGISTER_BUTTONS_WIDGET  ( m_ui->navigation_buttons_widget);

    SUBE_FILL_DEPO_ISLEM_YETKISI_COMBOBOX       ( m_ui->sub_comboBox_depo_islem_yetkisi );
    SUBE_FILL_SUBE_ISLEM_YETKISI_COMBOBOX       ( m_ui->sub_comboBox_sube_islem_yetkisi );

    m_ui->tabWidget_yetkiler->setCurrentIndex   ( 0 );

    EKRANI_DUZENLE();

}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void E9_KULLANICI_YETKILERI_FORMU::CLEAR_FORM_MEMBERS ()
{
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::NEW_RECORD
***************************************************************************************/

void E9_KULLANICI_YETKILERI_FORMU::NEW_RECORD()
{
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::GET_RECORD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::GET_RECORD ( int p_yetki_id )
{
    SQL_QUERY query_yonetim ( G_YONETIM_DB );
    SQL_QUERY sql_query     ( DB );

/************************************************** SISTEM  ***************************************************/

    sql_query.PREPARE_SELECT( "e9_kullanici_yetkileri",
                              "sis_blg_formuna_girebilir_mi,        "
                              "sis_kul_formuna_girebilir_mi,        "
                              "sis_frm_formuna_girebilir_mi,        "
                              "sis_yonetim_log_gorebilir_mi,        "
                              "sis_mail_srv_tanimlayabilirmi,       "
                              "sis_sms_srv_tanimlayabilirmi,        "
                              "e9_yetkilendirme_yapabilir_mi,       "
                              "muh_kullanabilir_mi,                 "
                              "isl_kullanabilir_mi,                 "
                              "adr_kullanabilir_mi,                 "
                              "dvz_kullanabilir_mi,                 "
                              "car_kullanabilir_mi,                 "
                              "cek_kullanabilir_mi,                 "
                              "bnk_kullanabilir_mi,                 "
                              "sub_kullanabilir_mi,                 "
                              "fat_kullanabilir_mi,                 "
                              "irs_kullanabilir_mi,                 "
                              "stk_kullanabilir_mi,                 "
                              "prs_kullanabilir_mi,                 "
                              "dmr_kullanabilir_mi,                 "
                              "smm_makbuzu_kullanabilir_mi,          "
                              "faiz_kullanabilir_mi,                "
                              "prk_kullanabilir_mi,                 "
                              "e9_onar_yapabilir_mi,                "
                              "e9_devir_islemi_yapabilir_mi   ,     "
                              "e9_log_gorebilir_mi,                 "
                              "e9_sabit_degerlere_girebilirmi,      "
                              "e9_belge_tasarlayabilir_mi,          "
                              "e9_ent_blg_degistirebilir_mi,        "
                              "e9_kullanicinin_sirketi,             "
                              "e9_sirket_degistire_bilir_mi,        "
                              "e9_urettigi_fisleri_grbilir_mi,      "

 /***************************************************** ADRES  ****************************************************/

                              "adr_frm_shs_blg_gorebilir_mi,      "
                              "adr_not_defterini_gorebilir_mi,  "
                              "adr_adres_kayitlari_raporu,      "
                              "adr_tel_web_kayitlari_raporu,    "
                              "adr_etiket_yazdirabilir_mi,      "
                              "adr_fihrist_yazdirabilir_mi,     "
                              "adr_toplu_mail_gonderebilir_mi,  "
                              "adr_grup_tanimlayabilir_mi,      "
                              "adr_toplu_sms_gonderebilir_mi,   "
                              "adr_export_yetkisi,              "
                              "adr_import_yetkisi,              "

 /********************************************************* BANKA  ***************************************************/

                              "bnk_hesabi_acabilir_mi,           "
                              "bnk_ent_fislerini_gorebilir_mi,   "
                              "bnk_defterini_inceleyebilir_mi,   "
                              "bnk_pos_ekst_inceleyebilir_mi,    "
                              "bnk_hareket_fisi_girebilir_mi,    "
                              "bnk_acilis_fisi_girebilir_mi,     "
                              "bnk_makro_tanimlayabilir_mi,      "
                              "bnk_kk_ekstresi_alabilir_mi,      "
                              "bnk_virman_islemi_yapabilir_mi,   "
                              "bnk_makro_fisi_kesebilir_mi,      "

 /************************************************************** CARI  **********************************************/

                              "car_tah_fisi_kesebilir_mi,        "
                              "car_odeme_fisi_kesebilir_mi,      "
                              "car_hareket_fisi_kesebilir_mi,    "
                              "car_acilis_fisi_kesebilir_mi,     "
                              "car_grup_tanimlayabilir_mi,       "
                              "car_kart_tanimlayabilir_mi,       "
                              "car_ent_fislerini_gorebilir_mi,   "
                              "car_ekstreyi_inceleyebilir_mi,    "
                              "car_bakiye_rpr_inceleyebilirmi,  "
                              "car_virman_fisi_kesebilir_mi,     "
                              "car_hesap_hareketleri_grb_mi,     "

 /************************************************************** CEKSENET ******************************************/
                              "cek_senet_girebilir_mi ,        "
                              "cek_bordro_girebilir_mi,        "
                              "cek_ent_fislerini_gorebilir_mi, "
                              "cek_listesini_inceleyebilir_mi, "
                              "cek_mus_ceklerini_gorebilir_mi, "
                              "cek_acilis_bordrosu_yetkisi     ,"

 /*************************************************************** DEMIRBAS  *****************************************/

                              "dmr_karti_tanimlayabilir_mi,      "
                              "dmr_grubu_tanimlayabilir_mi,      "
                              "dmr_yeni_deger_fis_kesebilirmi,   "
                              "dmr_amrtsman_fisi_kesebilir_mi,   "
                              "dmr_listesi_raporu_alabilir_mi,   "
                              "dmr_toplu_amortisman_ayrablrmi ,  "
                              "dmr_toplu_yeni_deger_ypblrmi ,    "
                              "dmr_rpr_islm_grup_tanmlyblr_mi ,  "

 /************************************************************** MUHASEBE  ******************************************/

                              "muh_mhs_fisi_kesebilir_mi,        "
                              "muh_ent_fislerini_gorebilir_mi,   "
                              "muh_hesap_planini_gorebilir_mi,   "
                              "muh_yvm_no_olusturabilir_mi,      "
                              "muh_hsp_ekstre_gorebilir_mi,      "
                              "muh_mizan_rprunu_gorebilir_mi,    "
                              "muh_hsp_plani_rpr_gorebilir_mi,   "
                              "muh_gelir_tblsunu_gorebilir_mi,   "
                              "muh_bilanco_inceleyebilir_mi,     "
                              "muh_yvm_dkmunu_gorebilir_mi,      "
                              "muh_dftri_kbr_inceleyebilir_mi,   "
                              "muh_fis_dokumunu_gorebilir_mi,    "
                              "muh_hata_fisleri_gorebilir_mi,    "
                              "muh_hsp_grbu_tanimlayabilir_mi,   "
                              "muh_yan_hesabi_girebilir_mi,      "
                              "muh_yan_fisi_olusturabilir_mi,    "
                              "hesap_ekstresi_nakli_yekun,       "
                              "mizan_nakli_yekun       ,         "
                              "muh_hesap_plani_kopyalama,        "
                              "muh_acilis_fisi_islem_yetkisi,    "

 /********************************************************** PERSONEL  ***********************************************/


                              "prs_tanimlarina_girebilir_mi,     "
                              "prs_ek_odnk_tanimlayabilir_mi,    "
                              "prs_ek_ksnti_tanimlayabilir_mi,   "
                              "prs_sbt_degerlere_girebilir_mi,   "
                              "prs_maas_brd_alabilir_mi,         "
                              "prs_maas_brd_iptal_edebilir_mi,   "
                              "prs_maas_brd_rpr_alabilir_mi,     "
                              "prs_ihbr_kdm_rpr_alabilir_mi,     "
                              "prs_is_ayrilma_rpr_alabilir_mi,   "
                              "prs_brd_inceleme_yapabilir_mi,    "
                              "prs_prsonel_raporu_alabilir_mi,   "
                              "prs_e_bildirge_alabilir_mi ,      "
                              "prs_prsonel_grup_girebilir_mi,    "
                              "prs_isten_ayr_prs_arayabilirmi,   "
                              "prs_eksik_gun_nedeni_girisi ,     "
                              "prs_ucret_pusulasi_raporu,        "

     /********************************************************* SUBE/ DEPO / UNITE  *************************************/

                              "sub_tanimlayabilir_mi, "
                              "sub_depo_tanimlayabilir_mi,"
                              "sub_depo_islem_yetkisi, "
                              "sub_sube_islem_yetkisi,   "
                              "sub_ent_hesaplari_yetkisi, "

 /************************************************************** STOK **************************************************/

                              "stk_urun_kartina_girebilir_mi, "
                              "stk_hizmet_tanimlayabilir_mi  ,"
                              "stk_grubu_tanimlayabilir_mi, "
                              "stk_fisine_girebilir_mi, "
                              "stk_urn_listesini_gorebilir_mi,"
                              "stk_miktar_rprunu_gorebilir_mi,"
                              "stk_ekstre_inceleyebilir_mi,"
                              "stk_urun_hareketlerini_grb_mi,"

  /****************************************************    FATURA       *********************************************/

                              "fat_alis_faturasi_kesebilir_mi    ,"
                              "fat_sts_faturasi_kesebilir_mi     ,"
                              "fat_irs_faturalastirma_elle       ,"
                              "fat_irs_faturalastirma_oto        ,"
                              "fat_irs_faturalastirma_toplu      ,"
                              "fat_satir_iskonto_yapabilir_mi    ,"
                              "fat_toplu_iskonto_yapabilir_mi    ,"
                              "fat_hizmet_ekstre_alabilir_mi     ,"
                              "fat_ba_bs_alabilir_mi             ,"
                              "fat_fatura_listesi_alabilir_mi    ,"

  /**************************************** İRSALİYE ***************************************************************/

                              "irs_alis_irsaliye_kesebilir_mi,"
                              "irs_sts_irsaliye_kesebilir_mi ,"
                              "irs_fat_irsaliyelestirme_elle   ,"
                              "irs_fat_irsaliyelestirme_oto    ,"
                              "irs_satir_iskonto_yapabilir_mi  ,"
                              "irs_listesi_gorebilir_mi ,"
                              "irs_toplu_iskonto_yapabilir_mi  ,"



 /***************************************************** ISLETME ****************************************************/

                              "isl_raporunu_gorebilir_mi,     "
                              "isl_kdv_raporunu_gorebilir_mi, "
                              "isl_fisi_kesebilir_mi, "

/***************************************************** SM_MAKBUZ ****************************************************/
                              "smm_makbuzu_gorebilir_mi ,"
                              "smm_hizmet_tanimlayabilir_mi,"
                              "smm_tahsil_edildi_odendi, "

/***************************************************** DOVIZ ****************************************************/
                               "dvz_kurlarini_gorebilir_mi,"
                               "dvz_tanimlayabilir_mi,"
                               "dvz_kuru_guncelleyebilir_mi,"

 /*************************************************   PERAKENDE ***************************************************/

                              "prk_fisi_girebilir_mi,"
                              "prk_gdr_tur_tanimlayabilir_mi,"
                              "prk_kasa_raporu_alabilir_mi,"
                              "kullanici_id ",
                              "yetki_id = :yetki_id");

    sql_query.SET_VALUE ( ":yetki_id", p_yetki_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();



    query_yonetim.PREPARE_SELECT("ynt_kullanicilar", "kullanici_kodu, kullanici_adi",
                                 "kullanici_id = :kullanici_id AND silinmis_mi = 0");

    query_yonetim.SET_VALUE(":kullanici_id", sql_query.VALUE("kullanici_id").toInt());

    if ( query_yonetim.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    query_yonetim.NEXT();

    m_ui->label_kullanici_kodu->setText( query_yonetim.VALUE(0).toString() );
    m_ui->label_kullanici_adi->setText ( query_yonetim.VALUE(1).toString() );

/*************************************************************** SISTEM *****************************************/


    m_ui->sistem_comboBox_bilgisayarlar_formunu_gorebilir_mi->setCurrentIndex        ( sql_query.VALUE("sis_blg_formuna_girebilir_mi").toInt()     );
    m_ui->sistem_comboBox_kullanicilar_formunu_gorebilir_mi->setCurrentIndex         ( sql_query.VALUE("sis_kul_formuna_girebilir_mi").toInt()     );
    m_ui->sistem_comboBox_firmalar_formunu_gorebilir_mi->setCurrentIndex             ( sql_query.VALUE("sis_frm_formuna_girebilir_mi").toInt()     );
    m_ui->sistem_comboBox_007_yonetim_bilgilerini_gorebilir_mi->setCurrentIndex      ( sql_query.VALUE("sis_yonetim_log_gorebilir_mi").toInt()     );
    m_ui->sistem_combo_box_posta_sunucusu_tanimlayabilirmi->setCurrentIndex          ( sql_query.VALUE("sis_mail_srv_tanimlayabilirmi").toInt() );
    m_ui->sistem_combo_box_sms_sunucusu_tanimlayabilirmi->setCurrentIndex            ( sql_query.VALUE("sis_sms_srv_tanimlayabilirmi").toInt()  );
    m_ui->yonetim_combo_box_e9_yetkilendirme->setCurrentIndex                        ( sql_query.VALUE("e9_yetkilendirme_yapabilir_mi").toInt()    );
    m_ui->yonetim_combo_box_e9_onar->setCurrentIndex                                 ( sql_query.VALUE("e9_onar_yapabilir_mi").toInt());
    m_ui->yonetim_combo_box_e9_devir_islemi->setCurrentIndex                         ( sql_query.VALUE("e9_devir_islemi_yapabilir_mi").toInt());
    m_ui->yonetim_combo_box_e9_log_bilgileri->setCurrentIndex                        ( sql_query.VALUE("e9_log_gorebilir_mi").toInt());
    m_ui->yonetim_combo_box_e9_sabit_degerler->setCurrentIndex                       ( sql_query.VALUE("e9_sabit_degerlere_girebilirmi").toInt());
    m_ui->yonetim_combo_box_belge_tasarimcisi->setCurrentIndex                       ( sql_query.VALUE("e9_belge_tasarlayabilir_mi").toInt());
    m_ui->yonetim_comboBox_e9_ent_bilgilerini_degistirebilir_mi->setCurrentIndex     ( sql_query.VALUE("e9_ent_blg_degistirebilir_mi").toInt());
    m_ui->yonetim_comboBox_e9_urettigi_fisleri_grbilir_mi->setCurrentIndex           ( sql_query.VALUE("e9_urettigi_fisleri_grbilir_mi").toInt());

    m_ui->yonetim_combo_box_kullanici_sirket_degistire_bilir_mi->setCurrentIndex     ( sql_query.VALUE("e9_sirket_degistire_bilir_mi" ).toInt());

/************************************************** ADRES ********************************************/

    m_ui->adr_comboBox_adres_karti_tanimlama->setCurrentIndex                       ( sql_query.VALUE ( "adr_frm_shs_blg_gorebilir_mi").toInt()      );
    m_ui->adr_comboBox_not_defterini_gorebilir_mi->setCurrentIndex                  ( sql_query.VALUE ( "adr_not_defterini_gorebilir_mi").toInt()  );
    m_ui->adr_comboBox_adres_kayit_raporu->setCurrentIndex                          ( sql_query.VALUE ( "adr_adres_kayitlari_raporu").toInt() );
    m_ui->adr_comboBox_tel_web_raporu->setCurrentIndex                              ( sql_query.VALUE ( "adr_tel_web_kayitlari_raporu").toInt() );
    m_ui->adr_comboBox_etiket_yazdirabilir_mi->setCurrentIndex                      ( sql_query.VALUE ( "adr_etiket_yazdirabilir_mi").toInt()      );
    m_ui->adr_comboBox_fihrist_yazdirabilir_mi->setCurrentIndex                     ( sql_query.VALUE ( "adr_fihrist_yazdirabilir_mi").toInt()     );
    m_ui->adr_comboBox_toplu_mail_gonderebilir_mi->setCurrentIndex                  ( sql_query.VALUE ( "adr_toplu_mail_gonderebilir_mi").toInt()  );
    m_ui->adr_comboBox_adres_grubu_tanimlayabilir_mi->setCurrentIndex               ( sql_query.VALUE ( "adr_grup_tanimlayabilir_mi").toInt()      );
    m_ui->adr_comboBox_toplu_sms_gonderme->setCurrentIndex                          ( sql_query.VALUE ( "adr_toplu_sms_gonderebilir_mi").toInt()   );
    m_ui->adr_comboBox_adres_export->setCurrentIndex                                ( sql_query.VALUE ( "adr_export_yetkisi").toInt());
    m_ui->adr_comboBox_adres_import->setCurrentIndex                                ( sql_query.VALUE ( "adr_import_yetkisi").toInt());

/********************************************************* BANKA ******************************************/

    m_ui->bnk_combobox_banka_hesabi->setCurrentIndex                        ( sql_query.VALUE ( "bnk_hesabi_acabilir_mi").toInt()           );
    m_ui->bnk_combobox_ent_fisleri->setCurrentIndex                         ( sql_query.VALUE ( "bnk_ent_fislerini_gorebilir_mi").toInt()   );
    m_ui->bnk_combobox_banka_defteri->setCurrentIndex                       ( sql_query.VALUE ( "bnk_defterini_inceleyebilir_mi").toInt()   );
    m_ui->bnk_combobox_pos_ekstresi->setCurrentIndex                        ( sql_query.VALUE ( "bnk_pos_ekst_inceleyebilir_mi").toInt()    );
    m_ui->bnk_combobox_bnk_hareket_fisi->setCurrentIndex                    ( sql_query.VALUE ( "bnk_hareket_fisi_girebilir_mi").toInt()    );
    m_ui->bnk_combobox_acilis_fisi->setCurrentIndex                         ( sql_query.VALUE ( "bnk_acilis_fisi_girebilir_mi").toInt()     );
    m_ui->bnk_combobox_bnk_makro_tanimlama->setCurrentIndex                 ( sql_query.VALUE ( "bnk_makro_tanimlayabilir_mi").toInt()      );
    m_ui->bnk_combobox_bnk_makro_fisi->setCurrentIndex                      ( sql_query.VALUE ( "bnk_makro_fisi_kesebilir_mi").toInt()      );
    m_ui->bnk_combobox_kk_ekstresi_gorebilir_mi->setCurrentIndex            ( sql_query.VALUE ( "bnk_kk_ekstresi_alabilir_mi").toInt()      );
    m_ui->bnk_combobox_virman_islemi_yapabilir_mi->setCurrentIndex          ( sql_query.VALUE ( "bnk_virman_islemi_yapabilir_mi").toInt()   );

/********************************************************** CARI ********************************************/

    m_ui->car_combobox_tahsilat_fisi->setCurrentIndex                   ( sql_query.VALUE ( "car_tah_fisi_kesebilir_mi").toInt()        );
    m_ui->car_combobox_odeme_fisi->setCurrentIndex                      ( sql_query.VALUE ( "car_odeme_fisi_kesebilir_mi").toInt()      );
    m_ui->car_combobox_hareket_fisi->setCurrentIndex                    ( sql_query.VALUE ( "car_hareket_fisi_kesebilir_mi").toInt()    );
    m_ui->car_combobox_cari_acilis_fisi->setCurrentIndex                ( sql_query.VALUE ( "car_acilis_fisi_kesebilir_mi").toInt()     );
    m_ui->car_combobox_cari_grup->setCurrentIndex                       ( sql_query.VALUE ( "car_grup_tanimlayabilir_mi").toInt()       );
    m_ui->car_combobox_cari_hesap->setCurrentIndex                      ( sql_query.VALUE ( "car_kart_tanimlayabilir_mi").toInt()       );
    m_ui->car_combobox_ent_fisleri->setCurrentIndex                     ( sql_query.VALUE ( "car_ent_fislerini_gorebilir_mi").toInt()   );
    m_ui->car_combobox_cari_hesap_ekstresi->setCurrentIndex             ( sql_query.VALUE ( "car_ekstreyi_inceleyebilir_mi").toInt()    );
    m_ui->car_combobox_cari_bakiye_raporu->setCurrentIndex              ( sql_query.VALUE ( "car_bakiye_rpr_inceleyebilirmi").toInt()  );
    m_ui->car_combobox_virman_fisi->setCurrentIndex                     ( sql_query.VALUE ( "car_virman_fisi_kesebilir_mi").toInt()     );
    m_ui->car_combobox_cari_hesap_hareketleri_grb_mi->setCurrentIndex    ( sql_query.VALUE ( "car_hesap_hareketleri_grb_mi").toInt()     );

/************************************************************ CEK ******************************************/

    m_ui->cek_combobox_cek_girisi->setCurrentIndex                      ( sql_query.VALUE ( "cek_senet_girebilir_mi").toInt()         );
    m_ui->cek_combobox_bordro_girisi->setCurrentIndex                   ( sql_query.VALUE ( "cek_bordro_girebilir_mi").toInt()        );
    m_ui->cek_combobox_ent_fisleri->setCurrentIndex                     ( sql_query.VALUE ( "cek_ent_fislerini_gorebilir_mi").toInt() );
    m_ui->cek_combobox_cek_senet_listesi->setCurrentIndex               ( sql_query.VALUE ( "cek_listesini_inceleyebilir_mi").toInt() );
    m_ui->cek_combobox_musteri_cek_ve_senetleri->setCurrentIndex        ( sql_query.VALUE ( "cek_mus_ceklerini_gorebilir_mi").toInt() );
    m_ui->cek_combobox_acilis_bordrosu_yetkisi->setCurrentIndex         ( sql_query.VALUE ( "cek_acilis_bordrosu_yetkisi").toInt()    );

/*********************************************************** DEMIRBAS ****************************************/

    m_ui->dmr_comboBox_demirbas_karti->setCurrentIndex                  ( sql_query.VALUE ( "dmr_karti_tanimlayabilir_mi").toInt()      );
    m_ui->dmr_comboBox_demirbas_grubu->setCurrentIndex                  ( sql_query.VALUE ( "dmr_grubu_tanimlayabilir_mi").toInt()      );
    m_ui->dmr_comboBox_yeniden_degerleme->setCurrentIndex               ( sql_query.VALUE ( "dmr_yeni_deger_fis_kesebilirmi").toInt() );
    m_ui->dmr_comboBox_amortisman_fisi->setCurrentIndex                 ( sql_query.VALUE ( "dmr_amrtsman_fisi_kesebilir_mi").toInt() );
    m_ui->dmr_comboBox_demirbas_listesi->setCurrentIndex                ( sql_query.VALUE ( "dmr_listesi_raporu_alabilir_mi").toInt()   );
    m_ui->dmr_comboBox_toplu_amortisman_ayirma->setCurrentIndex         ( sql_query.VALUE ( "dmr_toplu_amortisman_ayrablrmi").toInt()   );
    m_ui->dmr_comboBox_toplu_yeniden_degerleme->setCurrentIndex         ( sql_query.VALUE ( "dmr_toplu_yeni_deger_ypblrmi").toInt());
    m_ui->dmr_comboBox_rapor_grubu_tanimlayabilir_mi->setCurrentIndex   ( sql_query.VALUE ( "dmr_rpr_islm_grup_tanmlyblr_mi").toInt());

/*********************************************************** MUHASEBE *****************************************/

    m_ui->muh_combobox_mhs_fisi_kesebilir_mi->setCurrentIndex            ( sql_query.VALUE ( "muh_mhs_fisi_kesebilir_mi").toInt()        );
    m_ui->muh_combobox_ent_fisi->setCurrentIndex                         ( sql_query.VALUE ( "muh_ent_fislerini_gorebilir_mi").toInt()   );
    m_ui->muh_combobox_hesap_planini_gorebilir_mi->setCurrentIndex       ( sql_query.VALUE ( "muh_hesap_planini_gorebilir_mi").toInt()   );
    m_ui->muh_combobox_yvm_no_olusturabilir_mi->setCurrentIndex          ( sql_query.VALUE ( "muh_yvm_no_olusturabilir_mi").toInt()      );
    m_ui->muh_combobox_hsp_ekstresini_gorebilir_mi->setCurrentIndex      ( sql_query.VALUE ( "muh_hsp_ekstre_gorebilir_mi").toInt()  );
    m_ui->muh_combobox_mizan_raporunu_gorebilir_mi->setCurrentIndex      ( sql_query.VALUE ( "muh_mizan_rprunu_gorebilir_mi").toInt()  );
    m_ui->muh_combobox_hesap_plani_raporunu_gorebilir_mi->setCurrentIndex( sql_query.VALUE ( "muh_hsp_plani_rpr_gorebilir_mi").toInt() );
    m_ui->muh_combobox_gelir_tablosunu_gorebilir_mi->setCurrentIndex     ( sql_query.VALUE ( "muh_gelir_tblsunu_gorebilir_mi").toInt() );
    m_ui->muh_combobox_bilanco_inceleyebilir_mi->setCurrentIndex         ( sql_query.VALUE ( "muh_bilanco_inceleyebilir_mi").toInt()     );
    m_ui->muh_combobox_yvm_dokumunu_gorebilir_mi->setCurrentIndex        ( sql_query.VALUE ( "muh_yvm_dkmunu_gorebilir_mi").toInt()    );
    m_ui->muh_combobox_defteri_kebiri_inceleyebilir_mi->setCurrentIndex  ( sql_query.VALUE ( "muh_dftri_kbr_inceleyebilir_mi").toInt() );
    m_ui->muh_combobox_fis_dokumunu_gorebilir_mi->setCurrentIndex        ( sql_query.VALUE ( "muh_fis_dokumunu_gorebilir_mi").toInt()    );
    m_ui->muh_combobox_hatali_fisleri_gorebilir_mi->setCurrentIndex      ( sql_query.VALUE ( "muh_hata_fisleri_gorebilir_mi").toInt()  );
    m_ui->muh_combobox_hesap_grubu->setCurrentIndex                      ( sql_query.VALUE ( "muh_hsp_grbu_tanimlayabilir_mi").toInt()  );
    m_ui->muh_combobox_yansitma_fisi_olusturabilir_mi->setCurrentIndex   ( sql_query.VALUE ( "muh_yan_fisi_olusturabilir_mi").toInt()    );
    m_ui->muh_combobox_yansitma_hesaplari_girebilir_mi->setCurrentIndex  ( sql_query.VALUE ( "muh_yan_hesabi_girebilir_mi").toInt()      );
    m_ui->muh_combobox_hsp_ekstresi_nakli_yekun->setCurrentIndex         ( sql_query.VALUE ( "hesap_ekstresi_nakli_yekun").toInt());
    m_ui->muh_combobox_mizan_nakli_yekun->setCurrentIndex                ( sql_query.VALUE ( "mizan_nakli_yekun").toInt());
    m_ui->muh_combobox_hesap_plani_kopyalama->setCurrentIndex            ( sql_query.VALUE ( "muh_hesap_plani_kopyalama").toInt() );
    m_ui->muh_combobox_acilis_fisi_islem_yetkisi->setCurrentIndex        ( sql_query.VALUE ( "muh_acilis_fisi_islem_yetkisi").toInt());

/************************************************************* PERSONEL ***********************************************/

    m_ui->prs_comboBox_prs_sicil_karti_tanimlama->setCurrentIndex                    ( sql_query.VALUE("prs_tanimlarina_girebilir_mi").toInt()     );
    m_ui->prs_comboBox_ek_odenek_tanimlama->setCurrentIndex                          ( sql_query.VALUE("prs_ek_odnk_tanimlayabilir_mi").toInt()    );
    m_ui->prs_comboBox_ek_kesinti_tanimlama->setCurrentIndex                         ( sql_query.VALUE("prs_ek_ksnti_tanimlayabilir_mi").toInt()   );
    m_ui->prs_comboBox_sabit_degerler->setCurrentIndex                               ( sql_query.VALUE("prs_sbt_degerlere_girebilir_mi").toInt()   );
    m_ui->prs_combo_box_maas_bordrosu_alabilir_mi->setCurrentIndex                   ( sql_query.VALUE("prs_maas_brd_alabilir_mi").toInt()         );
    m_ui->prs_combo_box_maas_bordrosu_iptal_edebilir_mi->setCurrentIndex             ( sql_query.VALUE("prs_maas_brd_iptal_edebilir_mi").toInt()   );
    m_ui->prs_combo_box_maas_bordrosu_raporu_alabilir_mi->setCurrentIndex            ( sql_query.VALUE("prs_maas_brd_rpr_alabilir_mi").toInt()     );
    m_ui->prs_combo_box_ihbar_ve_kidem_tazminati_raporu_alabilir_mi->setCurrentIndex ( sql_query.VALUE("prs_ihbr_kdm_rpr_alabilir_mi").toInt()     );
    m_ui->prs_comboBox_isten_ayrilma_islemi->setCurrentIndex                         ( sql_query.VALUE("prs_is_ayrilma_rpr_alabilir_mi").toInt()   );
    m_ui->prs_combo_box_bordro_inceleme_yapabilir_mi->setCurrentIndex                ( sql_query.VALUE("prs_brd_inceleme_yapabilir_mi").toInt()    );
    m_ui->prs_combo_box_personel_raporu_alabilir_mi->setCurrentIndex                 ( sql_query.VALUE("prs_prsonel_raporu_alabilir_mi").toInt()   );
    m_ui->prs_comboBox_e_bildirge_alabilir_mi->setCurrentIndex                       ( sql_query.VALUE("prs_e_bildirge_alabilir_mi").toInt()       );
    m_ui->prs_comboBox_isten_ayrilan_arama->setCurrentIndex                          ( sql_query.VALUE("prs_isten_ayr_prs_arayabilirmi").toInt()   );
    m_ui->prs_comboBox_grup_tanimlama->setCurrentIndex                               ( sql_query.VALUE("prs_prsonel_grup_girebilir_mi").toInt()    );
    m_ui->prs_comboBox_eksik_gun_nedenleri->setCurrentIndex                          ( sql_query.VALUE("prs_eksik_gun_nedeni_girisi").toInt()      );
    m_ui->prs_comboBox_ucret_pusulasi->setCurrentIndex                               ( sql_query.VALUE("prs_ucret_pusulasi_raporu").toInt()        );

/*************************************************** STOK *****************************************************/

    m_ui->stk_comboBox_urun_kartini_gorebilir_mi->setCurrentIndex       ( sql_query.VALUE("stk_urun_kartina_girebilir_mi").toInt());
    m_ui->stk_combobox_hizmet_tanimlama->setCurrentIndex                ( sql_query.VALUE("stk_hizmet_tanimlayabilir_mi").toInt());
    m_ui->stk_comboBox_grup_tanimlayabilir_mi->setCurrentIndex          ( sql_query.VALUE("stk_grubu_tanimlayabilir_mi").toInt());
    m_ui->stk_comboBox_stok_fisine_girebilir_mi->setCurrentIndex        ( sql_query.VALUE("stk_fisine_girebilir_mi").toInt());
    m_ui->stk_comboBox_urun_listesi->setCurrentIndex                    ( sql_query.VALUE("stk_urn_listesini_gorebilir_mi").toInt());
    m_ui->stk_comboBox_stok_miktarlari->setCurrentIndex                 ( sql_query.VALUE("stk_miktar_rprunu_gorebilir_mi").toInt());
    m_ui->stk_comboBox_stok_ekstresi->setCurrentIndex                   ( sql_query.VALUE("stk_ekstre_inceleyebilir_mi").toInt());
    m_ui->stk_comboBox_stok_urun_hareketlerini_grb_mi->setCurrentIndex   ( sql_query.VALUE("stk_urun_hareketlerini_grb_mi").toInt());

/****************************************       FATURA      ***************************************************************/

    m_ui->fat_combobox_alis_faturasi->setCurrentIndex                   ( sql_query.VALUE("fat_alis_faturasi_kesebilir_mi").toInt() );
    m_ui->fat_combobox_satis_faturasi->setCurrentIndex                  ( sql_query.VALUE("fat_sts_faturasi_kesebilir_mi").toInt()  );
    m_ui->fat_combobox_elle_irs_faturalastirma->setCurrentIndex         ( sql_query.VALUE("fat_irs_faturalastirma_elle").toInt()    );
    m_ui->fat_combobox_oto_irs_faturalastirma_->setCurrentIndex         ( sql_query.VALUE("fat_irs_faturalastirma_oto").toInt()     );
    m_ui->fat_combobox_toplu_irs_faturalastirma->setCurrentIndex        ( sql_query.VALUE("fat_irs_faturalastirma_toplu").toInt()   );
    m_ui->fat_comboBox_satir_iskontosu_yapabilir_mi->setCurrentIndex    ( sql_query.VALUE("fat_satir_iskonto_yapabilir_mi").toInt()   );
    m_ui->fat_comboBox_toplu_iskonto_yapabilir_mi->setCurrentIndex      ( sql_query.VALUE("fat_toplu_iskonto_yapabilir_mi").toInt()   );
    m_ui->fat_comboBox_hizmet_ekstre_alabilir_mi->setCurrentIndex       ( sql_query.VALUE("fat_hizmet_ekstre_alabilir_mi").toInt());
    m_ui->fat_comboBox_ba_bs_alabilir_mi->setCurrentIndex               ( sql_query.VALUE("fat_ba_bs_alabilir_mi").toInt());
    m_ui->fat_comboBox_fatura_listesi->setCurrentIndex                  ( sql_query.VALUE("fat_fatura_listesi_alabilir_mi").toInt());

/*************************************        İRSALİYE     *************************************************************/

    m_ui->irs_combobox_alis_irsaliyesi->setCurrentIndex                 ( sql_query.VALUE("irs_alis_irsaliye_kesebilir_mi").toInt());
    m_ui->irs_combobox_satis_irsaliyesi->setCurrentIndex                ( sql_query.VALUE("irs_sts_irsaliye_kesebilir_mi").toInt());
    m_ui->irs_combobox_elle_fatura_irsaliyelestirme->setCurrentIndex    ( sql_query.VALUE("irs_fat_irsaliyelestirme_elle").toInt());
    m_ui->irs_combobox_oto_fatura_irsaliyelestirme->setCurrentIndex     ( sql_query.VALUE("irs_fat_irsaliyelestirme_oto").toInt());
    m_ui->irs_comboBox_satir_iskontosu_yapabilir_mi->setCurrentIndex    ( sql_query.VALUE("irs_satir_iskonto_yapabilir_mi").toInt());
    m_ui->irs_comboBox_irsaliye_listesi_gorebilir_mi->setCurrentIndex   ( sql_query.VALUE("irs_listesi_gorebilir_mi").toInt());
    m_ui->irs_comboBox_toplu_iskonto_yapabilir_mi->setCurrentIndex      ( sql_query.VALUE("irs_toplu_iskonto_yapabilir_mi").toInt());


/******************************************************* SUBE/ DEPO / UNITE  ***********************************/

    m_ui->sub_combobox_sube_tanimlayabilir_mi->setCurrentIndex          ( sql_query.VALUE("sub_tanimlayabilir_mi").toInt());
    m_ui->sub_comboBox_depo_kartini_gorebilir_mi->setCurrentIndex       ( sql_query.VALUE("sub_depo_tanimlayabilir_mi").toInt());
    m_ui->sub_comboBox_depo_islem_yetkisi->setCurrentIndex              ( m_ui->sub_comboBox_depo_islem_yetkisi->findText ( SUBE_GET_DEPO_ISLEM_YETKISI_STRING ( sql_query.VALUE("sub_depo_islem_yetkisi").toInt())));
    m_ui->sub_comboBox_sube_islem_yetkisi->setCurrentIndex              ( m_ui->sub_comboBox_sube_islem_yetkisi->findText ( SUBE_GET_ISLEM_YETKISI_STRING ( sql_query.VALUE("sub_sube_islem_yetkisi").toInt())));
    m_ui->sub_comboBox_sube_ent_hesaplari->setCurrentIndex              ( sql_query.VALUE("sub_ent_hesaplari_yetkisi").toInt());

/************************************************************** ISLETME ***************************************/

    m_ui->isl_combobox_isletme_defteri_raporunu_gorebilir_mi->setCurrentIndex ( sql_query.VALUE ( "isl_raporunu_gorebilir_mi").toInt()     );
    m_ui->isl_combobox_kdv_raporunu_gorebilir_mi->setCurrentIndex             ( sql_query.VALUE ( "isl_kdv_raporunu_gorebilir_mi").toInt() );
    m_ui->isl_combobox_fis_kesebilir_mi->setCurrentIndex                      ( sql_query.VALUE ( "isl_fisi_kesebilir_mi").toInt());

 /************************************************************** SM_MAKBUZU ***************************************/
    m_ui->comboBox_sm_gorebilir_mi->setCurrentIndex(sql_query.VALUE("smm_makbuzu_gorebilir_mi").toInt());
    m_ui->comboBox_smm_hizmet_tanimlayabilir_mi->setCurrentIndex(sql_query.VALUE("smm_hizmet_tanimlayabilir_mi").toInt());
    m_ui->comboBox_smm_tahsil_edildi_odendi->setCurrentIndex(sql_query.VALUE("smm_tahsil_edildi_odendi").toInt());

 /************************************************************** DOVIZ ***************************************/

    m_ui->dvz_comboBox_doviz_kurlari_gorebilir_mi->setCurrentIndex(sql_query.VALUE("dvz_kurlarini_gorebilir_mi").toInt());
    m_ui->dvz_comboBox_doviz_tanimlayabilir_mi->setCurrentIndex(sql_query.VALUE("dvz_tanimlayabilir_mi").toInt());
    m_ui->dvz_comboBox_kur_guncelleyebilir_mi->setCurrentIndex(sql_query.VALUE("dvz_kuru_guncelleyebilir_mi").toInt());


/****************************************************  PERAKENDE       ****************************************/

    m_ui->prk_comboBox_fis_girebilir_mi->setCurrentIndex(sql_query.VALUE("prk_fisi_girebilir_mi").toInt());
    m_ui->prk_comboBox_gider_turu_tanimlayabilir_mi->setCurrentIndex(sql_query.VALUE("prk_gdr_tur_tanimlayabilir_mi").toInt());
    m_ui->prk_comboBox_kasa_raporu->setCurrentIndex(sql_query.VALUE("prk_kasa_raporu_alabilir_mi").toInt());

    if ( sql_query.VALUE ( "muh_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_muhasebe->setChecked ( true );
    }
    else {
        m_ui->check_box_muhasebe->setChecked ( false );
    }

    if ( sql_query.VALUE ( "isl_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_isletme->setChecked ( true );
    }
    else {
        m_ui->check_box_isletme->setChecked ( false );
    }

    if ( sql_query.VALUE ( "adr_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_adres->setChecked ( true );
    }
    else {
        m_ui->check_box_adres->setChecked ( false );
    }

    if ( sql_query.VALUE ( "dvz_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_doviz->setChecked ( true );
    }
    else {
        m_ui->check_box_doviz->setChecked(false);
    }

    if ( sql_query.VALUE ( "car_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_cari->setChecked ( true );
    }
    else {
        m_ui->check_box_cari->setChecked ( false );
    }

    if ( sql_query.VALUE ( "cek_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_cek->setChecked ( true );
    }
    else {
        m_ui->check_box_cek->setChecked ( false );
    }

    if ( sql_query.VALUE ( "bnk_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_banka->setChecked ( true );
    }
    else {
        m_ui->check_box_banka->setChecked ( false );
    }
    if ( sql_query.VALUE ( "sub_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_sube->setChecked ( true );
    }
    else {
        m_ui->check_box_sube->setChecked ( false );
    }

    if ( sql_query.VALUE ( "fat_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_fatura->setChecked ( true );
    }
    else {
        m_ui->check_box_fatura->setChecked(false);
    }

    if ( sql_query.VALUE ( "irs_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_irsaliye->setChecked ( true );
    }
    else {
        m_ui->check_box_irsaliye->setChecked ( false );
    }

    if ( sql_query.VALUE ( "stk_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_stok->setChecked ( true );
    }
    else {
        m_ui->check_box_stok->setChecked ( false );
    }

    if ( sql_query.VALUE ( "prs_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_personel->setChecked ( true );
    }
    else {
        m_ui->check_box_personel->setChecked(false);
    }

    if ( sql_query.VALUE ( "dmr_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_demirbas->setChecked ( true );
    }
    else {
        m_ui->check_box_demirbas->setChecked ( false );
    }

    if (sql_query.VALUE("smm_makbuzu_kullanabilir_mi").toInt() EQ 1){
        m_ui->check_box_sm_makbuzu->setChecked(true);
    }
    else {
        m_ui->check_box_sm_makbuzu->setChecked(false);
    }

    if ( sql_query.VALUE ( "faiz_kullanabilir_mi" ).toInt() EQ 1 ) {
        m_ui->check_box_faiz->setChecked ( true );
    }
    else {
        m_ui->check_box_faiz->setChecked ( false );
    }

    if ( sql_query.VALUE( "prk_kullanabilir_mi").toInt() EQ 1 ) {
        m_ui->check_box_perakende->setChecked(true);
    }
    else {
        m_ui->check_box_perakende->setChecked(false);
    }

    return ADAK_OK;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::SELECT_RECORD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::SELECT_RECORD()
{
    return 0;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::CHECK_VAR
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->push_button_evet_yap ) {
        OPEN_ALL_EVENTS();
        TUM_COMBOXLARI_DEGISTIR(1);
        CLOSE_ALL_EVENTS();
    }
    else if ( p_object EQ m_ui->push_button_hayir_yap ) {
        OPEN_ALL_EVENTS();
        TUM_COMBOXLARI_DEGISTIR(0);
        CLOSE_ALL_EVENTS();
    }

    return ADAK_OK;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::CHECK_EMPTY
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::CHECK_EMPTY ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::CHECK_ADD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::ADD_RECORD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::ADD_RECORD ()
{
    return -1;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::CHECK_UPDATE
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::CHECK_UPDATE ( int p_record_id )
{
    Q_UNUSED ( p_record_id );
    return ADAK_OK;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::UPDATE_RECORD
***************************************************************************************/

void E9_KULLANICI_YETKILERI_FORMU::UPDATE_RECORD ( int p_yetki_id )
{

    E9_KULLANICI_YETKILERI_STRUCT * E9_KULLANICI_YETKILERI = ( E9_KULLANICI_YETKILERI_STRUCT * ) E9_GET_KULLANICI_YETKILERI();

 /*********************************************************  ADRES ************************************************/

    E9_KULLANICI_YETKILERI->adr_adres_karti_tanimlama_yetkisi        = m_ui->adr_comboBox_adres_karti_tanimlama->currentIndex();
    E9_KULLANICI_YETKILERI->adr_not_defteri_inceleme_yetkisi         = m_ui->adr_comboBox_not_defterini_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->adr_adres_kayitlari_rapor_alma_yetkisi   = m_ui->adr_comboBox_adres_kayit_raporu->currentIndex();
    E9_KULLANICI_YETKILERI->adr_tel_web_kayitlari_rapor_alma_yetkisi = m_ui->adr_comboBox_tel_web_raporu->currentIndex();
    E9_KULLANICI_YETKILERI->adr_etiket_yazdirma_yetkisi              = m_ui->adr_comboBox_etiket_yazdirabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->adr_fihrist_raporu_alma_yetkisi          = m_ui->adr_comboBox_fihrist_yazdirabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->adr_toplu_mail_gonderme_yetkisi          = m_ui->adr_comboBox_toplu_mail_gonderebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->adr_grup_tanimlama_yetkisi               = m_ui->adr_comboBox_adres_grubu_tanimlayabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->adr_toplu_sms_gonderme_yetkisi           = m_ui->adr_comboBox_toplu_sms_gonderme->currentIndex();
    E9_KULLANICI_YETKILERI->adr_export_yetkisi                       = m_ui->adr_comboBox_adres_export->currentIndex();
    E9_KULLANICI_YETKILERI->adr_import_yetkisi                       = m_ui->adr_comboBox_adres_import->currentIndex();



/************************************************************** BANKA  *****************************************/

    E9_KULLANICI_YETKILERI->bnk_banka_hesabi_acma_yetkisi    = m_ui->bnk_combobox_banka_hesabi->currentIndex();
    E9_KULLANICI_YETKILERI->bnk_ent_fisleri_inceleme_yetkisi = m_ui->bnk_combobox_ent_fisleri->currentIndex();
    E9_KULLANICI_YETKILERI->bnk_defter_raporu_alma_yetkisi   = m_ui->bnk_combobox_banka_defteri->currentIndex();
    E9_KULLANICI_YETKILERI->bnk_pos_ekstresi_alma_yetkisi    = m_ui->bnk_combobox_pos_ekstresi->currentIndex();
    E9_KULLANICI_YETKILERI->bnk_hareket_fisi_islem_yetkisi   = m_ui->bnk_combobox_bnk_hareket_fisi->currentIndex();
    E9_KULLANICI_YETKILERI->bnk_acilis_fisi_islem_yetkisi    = m_ui->bnk_combobox_acilis_fisi->currentIndex();
    E9_KULLANICI_YETKILERI->bnk_makro_tanimlama_yetkisi      = m_ui->bnk_combobox_bnk_makro_tanimlama->currentIndex();
    E9_KULLANICI_YETKILERI->bnk_kk_ekstresi_alma_yetkisi     = m_ui->bnk_combobox_kk_ekstresi_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->bnk_virman_islem_yetkisi         = m_ui->bnk_combobox_virman_islemi_yapabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->bnk_makro_fisi_islem_yetkisi     = m_ui->bnk_combobox_bnk_makro_fisi->currentIndex();


/******************************************************************* CARI   *************************************/

    E9_KULLANICI_YETKILERI->cari_tahsilat_fisi_islem_yetkisi  = m_ui->car_combobox_tahsilat_fisi->currentIndex();
    E9_KULLANICI_YETKILERI->cari_odeme_fisi_islem_yetkisi     = m_ui->car_combobox_odeme_fisi->currentIndex() ;
    E9_KULLANICI_YETKILERI->cari_hareket_fisi_islem_yetkisi   = m_ui->car_combobox_hareket_fisi->currentIndex();
    E9_KULLANICI_YETKILERI->cari_acilis_fisi_islem_yetkisi    = m_ui->car_combobox_cari_acilis_fisi->currentIndex();
    E9_KULLANICI_YETKILERI->cari_grup_tanimlama_yetkisi       = m_ui->car_combobox_cari_grup->currentIndex();
    E9_KULLANICI_YETKILERI->cari_kart_tanimlama_yetkisi       = m_ui->car_combobox_cari_hesap->currentIndex();
    E9_KULLANICI_YETKILERI->cari_ent_fisleri_inceleme_yetkisi = m_ui->car_combobox_ent_fisleri->currentIndex();
    E9_KULLANICI_YETKILERI->cari_ekstre_alma_yetkisi          = m_ui->car_combobox_cari_hesap_ekstresi->currentIndex();
    E9_KULLANICI_YETKILERI->cari_bakiye_raporu_alma_yetkisi   = m_ui->car_combobox_cari_bakiye_raporu->currentIndex();
    E9_KULLANICI_YETKILERI->cari_virman_islem_yetkisi         = m_ui->car_combobox_virman_fisi->currentIndex();
    E9_KULLANICI_YETKILERI->cari_hesap_hareketleri_gorebilir_mi= m_ui->car_combobox_cari_hesap_hareketleri_grb_mi->currentIndex();


/********************************************************* CEKSENET *****************************************************************/

    E9_KULLANICI_YETKILERI->cek_senet_girebilir_mi            = m_ui->cek_combobox_cek_girisi->currentIndex();
    E9_KULLANICI_YETKILERI->cek_bordrosu_islem_yetkisi        = m_ui->cek_combobox_bordro_girisi->currentIndex();
    E9_KULLANICI_YETKILERI->cek_ent_fisleri_inceleme_yetkisi  = m_ui->cek_combobox_ent_fisleri->currentIndex();
    E9_KULLANICI_YETKILERI->cek_listesi_rapor_alma_yetkisi    = m_ui->cek_combobox_cek_senet_listesi->currentIndex();
    E9_KULLANICI_YETKILERI->cek_mus_cek_senet_arama_yetkisi   = m_ui->cek_combobox_musteri_cek_ve_senetleri->currentIndex();
    E9_KULLANICI_YETKILERI->cek_acilis_bordrosu_islem_yetkisi = m_ui->cek_combobox_acilis_bordrosu_yetkisi->currentIndex();


/***************************************************** DEMIRBAS *****************************************************/

    E9_KULLANICI_YETKILERI->dmr_demirbas_karti_tanimlama_yetkisi    = m_ui->dmr_comboBox_demirbas_karti->currentIndex();
    E9_KULLANICI_YETKILERI->dmr_grup_tanimlama_yetkisi              = m_ui->dmr_comboBox_demirbas_grubu->currentIndex();
    E9_KULLANICI_YETKILERI->dmr_yen_degerleme_fisi_islem_yetkisi    = m_ui->dmr_comboBox_yeniden_degerleme->currentIndex();
    E9_KULLANICI_YETKILERI->dmr_amortisman_fisi_islem_yetkisi       = m_ui->dmr_comboBox_amortisman_fisi->currentIndex();
    E9_KULLANICI_YETKILERI->dmr_demirbas_listesi_rapor_alma_yetkisi = m_ui->dmr_comboBox_demirbas_listesi->currentIndex();
    E9_KULLANICI_YETKILERI->dmr_toplu_amortisman_ayirma_yetkisi     = m_ui->dmr_comboBox_toplu_amortisman_ayirma->currentIndex();
    E9_KULLANICI_YETKILERI->dmr_toplu_yeniden_degerleme_yetkisi     = m_ui->dmr_comboBox_toplu_yeniden_degerleme->currentIndex();
    E9_KULLANICI_YETKILERI->dmr_rapor_islem_grubu_tanimlama_yetkisi = m_ui->dmr_comboBox_rapor_grubu_tanimlayabilir_mi->currentIndex();


/*********************************************************** MUHASEBE *******************************************/

    E9_KULLANICI_YETKILERI->muh_muhasebe_fisi_islem_yetkisi       = m_ui->muh_combobox_mhs_fisi_kesebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_ent_fisleri_inceleme_yetkisi      = m_ui->muh_combobox_ent_fisi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_hesap_plani_islem_yetkisi         = m_ui->muh_combobox_hesap_planini_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_yvm_no_olusturma_yetkisi          = m_ui->muh_combobox_yvm_no_olusturabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_hesap_ekstresi_alma_yetkisi       = m_ui->muh_combobox_hsp_ekstresini_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_mizan_raporu_alma_yetkisi         = m_ui->muh_combobox_mizan_raporunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_hesap_plani_rapor_alma_yetkisi    = m_ui->muh_combobox_hesap_plani_raporunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_gelir_tablosu_rapor_alma_yetkisi  = m_ui->muh_combobox_gelir_tablosunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_bilanco_rapor_alma_yetkisi        = m_ui->muh_combobox_bilanco_inceleyebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_yvm_dokumu_alma_yetkisi           = m_ui->muh_combobox_yvm_dokumunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_defteri_kebir_raporu_alma_yetkisi = m_ui->muh_combobox_defteri_kebiri_inceleyebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_fis_dokumu_alma_yetkisi           = m_ui->muh_combobox_fis_dokumunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_hatali_fisleri_inceleme_yetkisi   = m_ui->muh_combobox_hatali_fisleri_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_grup_tanimlama_yetkisi            = m_ui->muh_combobox_hesap_grubu->currentIndex();
    E9_KULLANICI_YETKILERI->muh_yansitma_hesabi_tanimlama_yetkisi = m_ui->muh_combobox_yansitma_hesaplari_girebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_yansitma_fisi_olusturma_yetkisi   = m_ui->muh_combobox_yansitma_fisi_olusturabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->muh_hesap_ekstresi_nakli_yekun        = m_ui->muh_combobox_hsp_ekstresi_nakli_yekun->currentIndex();
    E9_KULLANICI_YETKILERI->muh_mizan_nakli_yekun                 = m_ui->muh_combobox_mizan_nakli_yekun->currentIndex();
    E9_KULLANICI_YETKILERI->muh_hesap_plani_kopyalama_yetkisi     = m_ui->muh_combobox_hesap_plani_kopyalama->currentIndex();
    E9_KULLANICI_YETKILERI->muh_acilis_fisi_islem_yetkisi         = m_ui->muh_combobox_acilis_fisi_islem_yetkisi->currentIndex();



/***************************************************************** PERSONEL ***************************************/

    E9_KULLANICI_YETKILERI->prs_sicil_karti_tanimlama_yetkisi        = m_ui->prs_comboBox_prs_sicil_karti_tanimlama->currentIndex();
    E9_KULLANICI_YETKILERI->prs_ek_odenek_tanimlama_yetkisi          = m_ui->prs_comboBox_ek_odenek_tanimlama->currentIndex();
    E9_KULLANICI_YETKILERI->prs_ek_kesinti_tanimlama_yetkisi         = m_ui->prs_comboBox_ek_kesinti_tanimlama->currentIndex();
    E9_KULLANICI_YETKILERI->prs_sabit_deger_guncelleme_yetkisi       = m_ui->prs_comboBox_sabit_degerler->currentIndex();
    E9_KULLANICI_YETKILERI->prs_maas_bordrosu_alma_yetkisi           = m_ui->prs_combo_box_maas_bordrosu_alabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->prs_maas_bordrosu_iptal_yetkisi          = m_ui->prs_combo_box_maas_bordrosu_iptal_edebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->prs_maas_bordrosu_raporu_alma_yetkisi    = m_ui->prs_combo_box_maas_bordrosu_raporu_alabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->prs_ihbar_ve_kidem_tazminati_raporu_alma_yetkisi = m_ui->prs_combo_box_ihbar_ve_kidem_tazminati_raporu_alabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->prs_isten_ayrilma_islem_yetkisi          = m_ui->prs_comboBox_isten_ayrilma_islemi->currentIndex();
    E9_KULLANICI_YETKILERI->prs_bordro_inceleme_yetkisi              = m_ui->prs_combo_box_bordro_inceleme_yapabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->prs_personel_raporu_alma_yetkisi         = m_ui->prs_combo_box_personel_raporu_alabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->prs_e_bildirge_alma_yetkisi              = m_ui->prs_comboBox_e_bildirge_alabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->prs_grup_tanimlama_yetkisi               = m_ui->prs_comboBox_grup_tanimlama->currentIndex();
    E9_KULLANICI_YETKILERI->prs_isten_ayrilan_personel_arama_yetkisi = m_ui->prs_comboBox_isten_ayrilan_arama->currentIndex();
    E9_KULLANICI_YETKILERI->prs_eksik_gun_nedenleri_giris_yetkisi    = m_ui->prs_comboBox_eksik_gun_nedenleri->currentIndex();
    E9_KULLANICI_YETKILERI->prs_ucret_pusulasi_raporu_alma_yetkisi   = m_ui->prs_comboBox_ucret_pusulasi->currentIndex();

/*************************************************** SUBE / DEPO / UNITE  ****************************************/

    E9_KULLANICI_YETKILERI->sub_sube_tanimlama_yetkisi           = m_ui->sub_combobox_sube_tanimlayabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->sub_depo_tanimlama_yetkisi           = m_ui->sub_comboBox_depo_kartini_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->sub_depo_islem_yetkisi               = SUBE_GET_DEPO_ISLEM_YETKISI_ENUM ( m_ui->sub_comboBox_depo_islem_yetkisi->currentText());
    E9_KULLANICI_YETKILERI->sub_sube_islem_yetkisi               = SUBE_GET_ISLEM_YETKISI_ENUM ( m_ui->sub_comboBox_sube_islem_yetkisi->currentText());
    E9_KULLANICI_YETKILERI->sub_ent_hesaplari_guncelleme_yetkisi = m_ui->sub_comboBox_sube_ent_hesaplari->currentIndex();


/********************************************************* STOK *************************************************/

    E9_KULLANICI_YETKILERI->stok_urun_karti_tanimlama_yetkisi   = m_ui->stk_comboBox_urun_kartini_gorebilir_mi->currentIndex() ;
    E9_KULLANICI_YETKILERI->stok_hizmet_karti_tanimlama_yetkisi = m_ui->stk_combobox_hizmet_tanimlama->currentIndex();
    E9_KULLANICI_YETKILERI->stok_grup_tanimlama_yetkisi         = m_ui->stk_comboBox_grup_tanimlayabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->stok_fisi_islem_yetkisi             = m_ui->stk_comboBox_stok_fisine_girebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->stok_urun_listesi_alma_yetkisi      = m_ui->stk_comboBox_urun_listesi->currentIndex();
    E9_KULLANICI_YETKILERI->stok_miktar_raporu_alma_yetkisi     = m_ui->stk_comboBox_stok_miktarlari->currentIndex();
    E9_KULLANICI_YETKILERI->stok_ekstresi_alma_yetkisi          = m_ui->stk_comboBox_stok_ekstresi->currentIndex();
    E9_KULLANICI_YETKILERI->stok_urun_hareketlerini_gorebilir_mi= m_ui->stk_comboBox_stok_urun_hareketlerini_grb_mi->currentIndex();


/*********************************************    FATURA          **************************************************/

    E9_KULLANICI_YETKILERI->fat_alis_faturasi_kesebilir_mi   = m_ui->fat_combobox_alis_faturasi->currentIndex();
    E9_KULLANICI_YETKILERI->fat_sts_faturasi_kesebilir_mi    = m_ui->fat_combobox_satis_faturasi->currentIndex();
    E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_elle      = m_ui->fat_combobox_elle_irs_faturalastirma->currentIndex();
    E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_oto       = m_ui->fat_combobox_oto_irs_faturalastirma_->currentIndex();
    E9_KULLANICI_YETKILERI->fat_irs_faturalastirma_toplu     = m_ui->fat_combobox_toplu_irs_faturalastirma->currentIndex();
    E9_KULLANICI_YETKILERI->fat_satir_iskontosu_yapabilir_mi = m_ui->fat_comboBox_satir_iskontosu_yapabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->fat_toplu_iskonto_yapabilir_mi   = m_ui->fat_comboBox_toplu_iskonto_yapabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->fat_hizmet_ekstresi_alma_yetkisi = m_ui->fat_comboBox_hizmet_ekstre_alabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->fat_ba_bs_listesi_alma_yetkisi   = m_ui->fat_comboBox_ba_bs_alabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->fat_fatura_listesi_alma_yetkisi  = m_ui->fat_comboBox_fatura_listesi->currentIndex();


/******************************************   İRSALİYE *************************************************************/

    E9_KULLANICI_YETKILERI->irs_alis_irsaliyesi_islem_yetkisi = m_ui->irs_combobox_alis_irsaliyesi->currentIndex();
    E9_KULLANICI_YETKILERI->irs_sts_irsaliyesi_islem_yetkisi  = m_ui->irs_combobox_satis_irsaliyesi->currentIndex();
    E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_elle     = m_ui->irs_combobox_elle_fatura_irsaliyelestirme->currentIndex();
    E9_KULLANICI_YETKILERI->irs_fat_irsaliyelestirme_oto      = m_ui->irs_combobox_oto_fatura_irsaliyelestirme->currentIndex();
    E9_KULLANICI_YETKILERI->irs_satir_iskontosu_yapabilir_mi  = m_ui->irs_comboBox_satir_iskontosu_yapabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->irs_listesi_gorebilir_mi          = m_ui->irs_comboBox_irsaliye_listesi_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->irs_toplu_iskonto_yapabilir_mi    = m_ui->irs_comboBox_toplu_iskonto_yapabilir_mi->currentIndex();


/******************************************************** ISLETME ****************************************************/

    E9_KULLANICI_YETKILERI->isl_isletme_defteri_rapor_alma_yetkisi = m_ui->isl_combobox_isletme_defteri_raporunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->isl_kdv_raporu_alma_yetkisi            = m_ui->isl_combobox_kdv_raporunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->isl_fisi_islem_yetkisi                 = m_ui->isl_combobox_fis_kesebilir_mi->currentIndex();


/******************************************************** SM_MAKBUZU ****************************************************/

    E9_KULLANICI_YETKILERI->smm_makbuzu_islem_yetkisi              = m_ui->comboBox_sm_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->smm_hizmet_tanimlama_yetkisi           = m_ui->comboBox_smm_hizmet_tanimlayabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->smm_tahsil_edildi_odendi_secili_gelsin = m_ui->comboBox_smm_tahsil_edildi_odendi->currentIndex();

/******************************************************** DOVIZ ****************************************************/

    E9_KULLANICI_YETKILERI->dvz_dovizleri_inceleme_yetkisi = m_ui->dvz_comboBox_doviz_kurlari_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->dvz_doviz_tanimlama_yetkisi    = m_ui->dvz_comboBox_doviz_tanimlayabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->dvz_kur_guncelleme_yetkisi     = m_ui->dvz_comboBox_kur_guncelleyebilir_mi->currentIndex();

/********************************************       PERAKENDE      ************************************************/

    E9_KULLANICI_YETKILERI->prk_fis_girebilir_mi          = m_ui->prk_comboBox_fis_girebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->prk_gdr_tur_tanimlayabilir_mi = m_ui->prk_comboBox_gider_turu_tanimlayabilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->prk_kasa_raporu_alabilir_mi   = m_ui->prk_comboBox_kasa_raporu->currentIndex();


/************************************************************* SISTEM **********************************************/

    E9_KULLANICI_YETKILERI->sis_blg_formuna_girebilir_mi        = m_ui->sistem_comboBox_bilgisayarlar_formunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->sis_kul_formuna_girebilir_mi        = m_ui->sistem_comboBox_kullanicilar_formunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->sis_veritabani_formuna_girebilir_mi = m_ui->sistem_comboBox_firmalar_formunu_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->sis_yonetim_log_inceleme_yetkisi    = m_ui->sistem_comboBox_007_yonetim_bilgilerini_gorebilir_mi->currentIndex();
    E9_KULLANICI_YETKILERI->sis_mail_srv_tanimlayabilirmi       = m_ui->sistem_combo_box_posta_sunucusu_tanimlayabilirmi->currentIndex();
    E9_KULLANICI_YETKILERI->sis_sms_srv_tanimlayabilirmi        = m_ui->sistem_combo_box_sms_sunucusu_tanimlayabilirmi->currentIndex() ;


    E9_KULLANICI_YETKILERI->ynt_e9_yetkilendirme_yapabilir_mi   = m_ui->yonetim_combo_box_e9_yetkilendirme->currentIndex();
    E9_KULLANICI_YETKILERI->ynt_e9_onar_yetkisi                 = m_ui->yonetim_combo_box_e9_onar->currentIndex();
    E9_KULLANICI_YETKILERI->ynt_e9_devir_islemi_yapabilir_mi    = m_ui->yonetim_combo_box_e9_devir_islemi->currentIndex();
    E9_KULLANICI_YETKILERI->ynt_e9_log_inceleme_yetkisi         = m_ui->yonetim_combo_box_e9_log_bilgileri->currentIndex();
    E9_KULLANICI_YETKILERI->ynt_e9_sabit_degerler_guncelleme_yetkisi = m_ui->yonetim_combo_box_e9_sabit_degerler->currentIndex();
    E9_KULLANICI_YETKILERI->ynt_e9_belge_tasarlama_yetkisi      = m_ui->yonetim_combo_box_belge_tasarimcisi->currentIndex();
    E9_KULLANICI_YETKILERI->ynt_e9_ent_bilgilerini_degistirme_yetkisi = m_ui->yonetim_comboBox_e9_ent_bilgilerini_degistirebilir_mi->currentIndex() ;

    E9_KULLANICI_YETKILERI->ynt_e9_kullanici_sirket_degistire_bilir_mi = m_ui->yonetim_combo_box_kullanici_sirket_degistire_bilir_mi->currentIndex();

    E9_KULLANICI_YETKILERI->ynt_e9_urettigi_fisleri_grbilir_mi  = m_ui->yonetim_comboBox_e9_urettigi_fisleri_grbilir_mi->currentIndex();


    if ( m_ui->check_box_muhasebe->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_muh_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_muh_kullanabilir_mi = 0;
    }

    if ( m_ui->check_box_isletme->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_isl_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_isl_kullanabilir_mi = 0;
    }

    if ( m_ui->check_box_adres->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_adr_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_adr_kullanabilir_mi = 0;
    }

    if ( m_ui->check_box_doviz->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_dvz_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_dvz_kullanabilir_mi = 0;
    }

    if ( m_ui->check_box_cari->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi = 0;
    }
    if ( m_ui->check_box_cek->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi = 0;
    }
    if ( m_ui->check_box_banka->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi = 0;
    }
    if ( m_ui->check_box_sube->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_sub_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_sub_kullanabilir_mi = 0;
    }
    if ( m_ui->check_box_fatura->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi = 0;
    }
    if ( m_ui->check_box_irsaliye->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_irs_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_irs_kullanabilir_mi = 0;
    }
    if ( m_ui->check_box_stok->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_stk_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_stk_kullanabilir_mi = 0;
    }
    if ( m_ui->check_box_personel->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_prs_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_prs_kullanabilir_mi = 0;
    }
    if ( m_ui->check_box_demirbas->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_dmr_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_dmr_kullanabilir_mi = 0;
    }

    if (m_ui->check_box_sm_makbuzu->isChecked() EQ true ){
        E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi = 0;
    }
    if ( m_ui->check_box_faiz->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi = 0;
    }

    if ( m_ui->check_box_perakende->isChecked() EQ true ) {
        E9_KULLANICI_YETKILERI->e9_prk_kullanabilir_mi = 1;
    }
    else {
        E9_KULLANICI_YETKILERI->e9_prk_kullanabilir_mi = 0;
    }

    SQL_QUERY sql_query ( DB );

    int kullanici_id = 0;

    sql_query.PREPARE_SELECT ( "e9_kullanici_yetkileri", "kullanici_id","yetki_id = :yetki_id");

    sql_query.SET_VALUE(":yetki_id", p_yetki_id );

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        kullanici_id = sql_query.VALUE(0).toInt();
    }

    if ( kullanici_id EQ KULLANICI_ID() ) {
        if ( m_yetkileri_tazele NE NULL ) {
            *m_yetkileri_tazele = true;
        }
    }

    E9_KULLANICIYI_YETKILERI_UPDATE(E9_KULLANICI_YETKILERI,kullanici_id);

    SQL_QUERY query_yonetim (G_YONETIM_DB);

    query_yonetim.PREPARE_SELECT("ynt_kullanicilar", "kullanici_kodu, kullanici_adi",
                                 "kullanici_id = :kullanici_id AND silinmis_mi = 0" );

    query_yonetim.SET_VALUE(":kullanici_id", kullanici_id);

    if ( query_yonetim.SELECT() EQ 0 ) {
        return;
    }

    query_yonetim.NEXT();

    QString kullanici_kodu_adi = "Kullanıcı Kodu / Adı : " + query_yonetim.VALUE(0).toString() +
                                 "  " + query_yonetim.VALUE(1).toString();


    E9_LOG_KAYDI_EKLE (E9_YONETIM , LOG_E9_YETKILENDIRME , -1 , kullanici_kodu_adi);


}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::CHECK_DELETE
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED(record_id);
    return ADAK_OK;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::DELETE_RECORD
***************************************************************************************/

void E9_KULLANICI_YETKILERI_FORMU::DELETE_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::FIND_RECORD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::FIND_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::FIND_FIRST_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::FIND_LAST_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::FIND_NEXT_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::FIND_PREV_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::LOCK_RECORD
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::LOCK_RECORD ( int p_yetki_id )
{
    return DB->LOCK_ROW( "yetki_id","e9_kullanici_yetkileri", QString( "yetki_id = %1" ).arg( p_yetki_id ) );
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::UNLOCK_RECORD
***************************************************************************************/

void E9_KULLANICI_YETKILERI_FORMU::UNLOCK_RECORD(int p_yetki_id )
{
    DB->UNLOCK_ROW( "yetki_id","e9_kullanici_yetkileri", QString( "yetki_id = %1" ).arg( p_yetki_id  ) );
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::TUM_COMBOXLARI_DEGISTIR
***************************************************************************************/

void E9_KULLANICI_YETKILERI_FORMU::TUM_COMBOXLARI_DEGISTIR ( int value )
{
/*************************************************** ADRES  **************************************************/

    m_ui->adr_comboBox_adres_karti_tanimlama->setCurrentIndex    ( value );
    m_ui->adr_comboBox_not_defterini_gorebilir_mi->setCurrentIndex              ( value );
    m_ui->adr_comboBox_adres_kayit_raporu->setCurrentIndex                      ( value );
    m_ui->adr_comboBox_tel_web_raporu->setCurrentIndex                          ( value );
    m_ui->adr_comboBox_etiket_yazdirabilir_mi->setCurrentIndex                  ( value );
    m_ui->adr_comboBox_fihrist_yazdirabilir_mi->setCurrentIndex                 ( value );
    m_ui->adr_comboBox_toplu_mail_gonderebilir_mi->setCurrentIndex              ( value );
    m_ui->adr_comboBox_adres_grubu_tanimlayabilir_mi->setCurrentIndex           ( value );
    m_ui->adr_comboBox_toplu_sms_gonderme->setCurrentIndex                      ( value );
    m_ui->adr_comboBox_adres_export->setCurrentIndex(value);
    m_ui->adr_comboBox_adres_import->setCurrentIndex(value);


/************************************************************** BANKA  *******************************************/

    m_ui->bnk_combobox_banka_hesabi->setCurrentIndex                            ( value );
    m_ui->bnk_combobox_ent_fisleri->setCurrentIndex                             ( value );
    m_ui->bnk_combobox_banka_defteri->setCurrentIndex                           ( value );
    m_ui->bnk_combobox_pos_ekstresi->setCurrentIndex                            ( value );
    m_ui->bnk_combobox_bnk_hareket_fisi->setCurrentIndex                        ( value );
    m_ui->bnk_combobox_acilis_fisi->setCurrentIndex                             ( value );
    m_ui->bnk_combobox_bnk_makro_tanimlama->setCurrentIndex                     ( value );
    m_ui->bnk_combobox_kk_ekstresi_gorebilir_mi->setCurrentIndex                ( value );
    m_ui->bnk_combobox_virman_islemi_yapabilir_mi->setCurrentIndex              ( value );
    m_ui->bnk_combobox_bnk_makro_fisi->setCurrentIndex                          ( value );

/********************************************************* CARI *************************************************/

    m_ui->car_combobox_tahsilat_fisi->setCurrentIndex                           ( value );
    m_ui->car_combobox_odeme_fisi->setCurrentIndex                              ( value );
    m_ui->car_combobox_hareket_fisi->setCurrentIndex                            ( value );
    m_ui->car_combobox_cari_acilis_fisi->setCurrentIndex                        ( value );
    m_ui->car_combobox_cari_grup->setCurrentIndex                               ( value );
    m_ui->car_combobox_cari_hesap->setCurrentIndex                              ( value );
    m_ui->car_combobox_ent_fisleri->setCurrentIndex                             ( value );
    m_ui->car_combobox_cari_hesap_ekstresi->setCurrentIndex                     ( value );
    m_ui->car_combobox_cari_bakiye_raporu->setCurrentIndex                      ( value );
    m_ui->car_combobox_virman_fisi->setCurrentIndex                             ( value );


/******************************************************************** CEK *******************************************/

    m_ui->cek_combobox_cek_girisi->setCurrentIndex                              ( value );
    m_ui->cek_combobox_bordro_girisi->setCurrentIndex                           ( value );
    m_ui->cek_combobox_ent_fisleri->setCurrentIndex                             ( value );
    m_ui->cek_combobox_cek_senet_listesi->setCurrentIndex                       ( value );
    m_ui->cek_combobox_musteri_cek_ve_senetleri->setCurrentIndex                ( value );
    m_ui->cek_combobox_acilis_bordrosu_yetkisi->setCurrentIndex                 ( value );

/****************************************************************** ISLETME ******************************************/

    m_ui->isl_combobox_isletme_defteri_raporunu_gorebilir_mi->setCurrentIndex    ( value );
    m_ui->isl_combobox_kdv_raporunu_gorebilir_mi->setCurrentIndex                ( value );
    m_ui->isl_combobox_fis_kesebilir_mi->setCurrentIndex                         ( value );

/****************************************************************** MUHASEBE *****************************************/

    m_ui->muh_combobox_mhs_fisi_kesebilir_mi->setCurrentIndex                    ( value );
    m_ui->muh_combobox_hesap_planini_gorebilir_mi->setCurrentIndex               ( value );
    m_ui->muh_combobox_yvm_no_olusturabilir_mi->setCurrentIndex                  ( value );
    m_ui->muh_combobox_hsp_ekstresini_gorebilir_mi->setCurrentIndex              ( value );
    m_ui->muh_combobox_mizan_raporunu_gorebilir_mi->setCurrentIndex              ( value );
    m_ui->muh_combobox_hesap_plani_raporunu_gorebilir_mi->setCurrentIndex        ( value );
    m_ui->muh_combobox_gelir_tablosunu_gorebilir_mi->setCurrentIndex             ( value );
    m_ui->muh_combobox_bilanco_inceleyebilir_mi->setCurrentIndex                 ( value );
    m_ui->muh_combobox_yvm_dokumunu_gorebilir_mi->setCurrentIndex                ( value );
    m_ui->muh_combobox_defteri_kebiri_inceleyebilir_mi->setCurrentIndex          ( value );
    m_ui->muh_combobox_fis_dokumunu_gorebilir_mi->setCurrentIndex                ( value );
    m_ui->muh_combobox_hatali_fisleri_gorebilir_mi->setCurrentIndex              ( value );

    m_ui->muh_combobox_ent_fisi->setCurrentIndex                                 ( value );
    m_ui->muh_combobox_hesap_grubu->setCurrentIndex                              ( value );
    m_ui->muh_combobox_yansitma_fisi_olusturabilir_mi->setCurrentIndex           ( value );
    m_ui->muh_combobox_yansitma_hesaplari_girebilir_mi->setCurrentIndex          ( value );

    m_ui->muh_combobox_hsp_ekstresi_nakli_yekun->setCurrentIndex                 ( value );
    m_ui->muh_combobox_mizan_nakli_yekun->setCurrentIndex                        ( value );
    m_ui->muh_combobox_hesap_plani_kopyalama->setCurrentIndex                    ( value );
    m_ui->muh_combobox_acilis_fisi_islem_yetkisi->setCurrentIndex                ( value );

/***************************************************** PERSONEL ****************************************************/

    m_ui->prs_combo_box_bordro_inceleme_yapabilir_mi->setCurrentIndex                 ( value );
    m_ui->prs_combo_box_ihbar_ve_kidem_tazminati_raporu_alabilir_mi->setCurrentIndex  ( value );
    m_ui->prs_comboBox_isten_ayrilan_arama->setCurrentIndex                           ( value );
    m_ui->prs_comboBox_isten_ayrilma_islemi->setCurrentIndex                          ( value );
    m_ui->prs_combo_box_maas_bordrosu_alabilir_mi->setCurrentIndex                    ( value );
    m_ui->prs_combo_box_maas_bordrosu_iptal_edebilir_mi->setCurrentIndex              ( value );
    m_ui->prs_combo_box_maas_bordrosu_raporu_alabilir_mi->setCurrentIndex             ( value );
    m_ui->prs_comboBox_ek_kesinti_tanimlama->setCurrentIndex                          ( value );
    m_ui->prs_comboBox_ek_odenek_tanimlama->setCurrentIndex                           ( value );
    m_ui->prs_combo_box_personel_raporu_alabilir_mi->setCurrentIndex                  ( value );
    m_ui->prs_comboBox_prs_sicil_karti_tanimlama->setCurrentIndex                     ( value );
    m_ui->prs_comboBox_sabit_degerler->setCurrentIndex                                ( value );
    m_ui->prs_comboBox_e_bildirge_alabilir_mi->setCurrentIndex                        ( value );
    m_ui->prs_comboBox_grup_tanimlama->setCurrentIndex                                ( value );
    m_ui->prs_comboBox_eksik_gun_nedenleri->setCurrentIndex                           ( value );
    m_ui->prs_comboBox_ucret_pusulasi->setCurrentIndex                                ( value );

/****************************************************************** STOK ******************************************/

    m_ui->stk_comboBox_stok_fisine_girebilir_mi->setCurrentIndex        ( value );
    m_ui->stk_comboBox_urun_kartini_gorebilir_mi->setCurrentIndex       ( value );
    m_ui->stk_combobox_hizmet_tanimlama->setCurrentIndex                ( value );
    m_ui->stk_comboBox_grup_tanimlayabilir_mi->setCurrentIndex          ( value );
    m_ui->stk_comboBox_stok_ekstresi->setCurrentIndex                   ( value );
    m_ui->stk_comboBox_stok_miktarlari->setCurrentIndex                 ( value );
    m_ui->stk_comboBox_urun_listesi->setCurrentIndex                    ( value );

/********************************************         FATURA             ********************************************/
    m_ui->fat_combobox_alis_faturasi->setCurrentIndex                   ( value );
    m_ui->fat_combobox_satis_faturasi->setCurrentIndex                  ( value );
    m_ui->fat_combobox_elle_irs_faturalastirma->setCurrentIndex         ( value );
    m_ui->fat_combobox_oto_irs_faturalastirma_->setCurrentIndex         ( value );
    m_ui->fat_combobox_toplu_irs_faturalastirma->setCurrentIndex        ( value );
    m_ui->fat_comboBox_satir_iskontosu_yapabilir_mi->setCurrentIndex    ( value );
    m_ui->fat_comboBox_toplu_iskonto_yapabilir_mi->setCurrentIndex      ( value );
    m_ui->fat_comboBox_hizmet_ekstre_alabilir_mi->setCurrentIndex       ( value );
    m_ui->fat_comboBox_ba_bs_alabilir_mi->setCurrentIndex               ( value );
    m_ui->fat_comboBox_fatura_listesi->setCurrentIndex                  ( value );

/*******************************************         İRSALİYE            ********************************************/

    m_ui->irs_combobox_alis_irsaliyesi->setCurrentIndex                 ( value );
    m_ui->irs_combobox_satis_irsaliyesi->setCurrentIndex                ( value );
    m_ui->irs_combobox_elle_fatura_irsaliyelestirme->setCurrentIndex    ( value );
    m_ui->irs_combobox_oto_fatura_irsaliyelestirme->setCurrentIndex     ( value );
    m_ui->irs_comboBox_satir_iskontosu_yapabilir_mi->setCurrentIndex    ( value );
    m_ui->irs_comboBox_irsaliye_listesi_gorebilir_mi->setCurrentIndex   ( value );
    m_ui->irs_comboBox_toplu_iskonto_yapabilir_mi->setCurrentIndex      ( value );


/************************************************************** DEMIRBAS ********************************************/

   m_ui->dmr_comboBox_demirbas_karti->setCurrentIndex                   ( value );
   m_ui->dmr_comboBox_demirbas_grubu->setCurrentIndex                   ( value );
   m_ui->dmr_comboBox_amortisman_fisi->setCurrentIndex                  ( value );
   m_ui->dmr_comboBox_yeniden_degerleme->setCurrentIndex                ( value );
   m_ui->dmr_comboBox_demirbas_listesi->setCurrentIndex                 ( value );
   m_ui->dmr_comboBox_toplu_amortisman_ayirma->setCurrentIndex          ( value );
   m_ui->dmr_comboBox_toplu_yeniden_degerleme->setCurrentIndex          ( value );
   m_ui->dmr_comboBox_rapor_grubu_tanimlayabilir_mi->setCurrentIndex    ( value );

/****************************************************** SUBE / DEPO / UNITE *****************************************/

    m_ui->sub_combobox_sube_tanimlayabilir_mi->setCurrentIndex          ( value );
    m_ui->sub_comboBox_depo_kartini_gorebilir_mi->setCurrentIndex       ( value );
    if ( value EQ 1 ) {
        m_ui->sub_comboBox_depo_islem_yetkisi->setCurrentIndex              ( TUM_SUBELERDE );
        m_ui->sub_comboBox_sube_islem_yetkisi->setCurrentIndex              ( TUM_SUBELERDE );
    } else {
        m_ui->sub_comboBox_depo_islem_yetkisi->setCurrentIndex              ( SADECE_UNITE_SUBESINDE );
        m_ui->sub_comboBox_sube_islem_yetkisi->setCurrentIndex              ( SADECE_UNITE_SUBESINDE );
    }
    m_ui->sub_comboBox_sube_ent_hesaplari->setCurrentIndex              ( value );

/****************************************************** SMM_MAKBUZU *****************************************/
    m_ui->comboBox_sm_gorebilir_mi->setCurrentIndex                     ( value );
    m_ui->comboBox_smm_hizmet_tanimlayabilir_mi->setCurrentIndex        (value);
    m_ui->comboBox_smm_tahsil_edildi_odendi->setCurrentIndex            (value);

/****************************************************** DOVIZ *****************************************/
    m_ui->dvz_comboBox_doviz_kurlari_gorebilir_mi->setCurrentIndex       ( value );
    m_ui->dvz_comboBox_doviz_tanimlayabilir_mi->setCurrentIndex          ( value );
    m_ui->dvz_comboBox_kur_guncelleyebilir_mi->setCurrentIndex           ( value );

/*****************************************************   PERAKENDE  ***********************************************/

    m_ui->prk_comboBox_fis_girebilir_mi->setCurrentIndex                ( value );
    m_ui->prk_comboBox_gider_turu_tanimlayabilir_mi->setCurrentIndex    ( value);
    m_ui->prk_comboBox_kasa_raporu->setCurrentIndex                     ( value );

/*********************************************************** SISTEM **************************************************/

    m_ui->sistem_comboBox_bilgisayarlar_formunu_gorebilir_mi->setCurrentIndex          ( value );
    m_ui->sistem_comboBox_kullanicilar_formunu_gorebilir_mi->setCurrentIndex           ( value );
    m_ui->sistem_comboBox_firmalar_formunu_gorebilir_mi->setCurrentIndex               ( value );
    m_ui->sistem_comboBox_007_yonetim_bilgilerini_gorebilir_mi->setCurrentIndex        ( value );
    m_ui->sistem_combo_box_posta_sunucusu_tanimlayabilirmi->setCurrentIndex            ( value );
    m_ui->sistem_combo_box_sms_sunucusu_tanimlayabilirmi->setCurrentIndex              ( value );
    m_ui->yonetim_combo_box_e9_yetkilendirme->setCurrentIndex                          ( value );
    m_ui->yonetim_combo_box_e9_devir_islemi->setCurrentIndex                           ( value );
    m_ui->yonetim_combo_box_belge_tasarimcisi->setCurrentIndex                         ( value );
    m_ui->yonetim_combo_box_e9_log_bilgileri->setCurrentIndex                          ( value );
    m_ui->yonetim_combo_box_e9_onar->setCurrentIndex                                   ( value );
    m_ui->yonetim_combo_box_e9_sabit_degerler->setCurrentIndex                         ( value );
    m_ui->yonetim_comboBox_e9_ent_bilgilerini_degistirebilir_mi->setCurrentIndex       ( value );
    m_ui->yonetim_combo_box_kullanici_sirket_degistire_bilir_mi->setCurrentIndex       ( value );

    if ( value EQ 1 ) {
        m_ui->check_box_muhasebe->setChecked        ( true );
        m_ui->check_box_isletme->setChecked         ( true );
        m_ui->check_box_adres->setChecked           ( true );
        m_ui->check_box_doviz->setChecked           ( true );
        m_ui->check_box_cari->setChecked            ( true );
        m_ui->check_box_cek->setChecked             ( true );
        m_ui->check_box_banka->setChecked           ( true );
        m_ui->check_box_sube->setChecked            ( true );
        m_ui->check_box_fatura->setChecked          ( true );
        m_ui->check_box_irsaliye->setChecked        ( true );
        m_ui->check_box_stok->setChecked            ( true );
        m_ui->check_box_personel->setChecked        ( true );
        m_ui->check_box_demirbas->setChecked        ( true );
        m_ui->check_box_sm_makbuzu->setChecked      ( true );
        m_ui->check_box_faiz->setChecked            ( true );
        m_ui->check_box_perakende->setChecked       ( true );
    }
    else {
        m_ui->check_box_muhasebe->setChecked        ( false );
        m_ui->check_box_isletme->setChecked         ( false );
        m_ui->check_box_adres->setChecked           ( false );
        m_ui->check_box_doviz->setChecked           ( false );
        m_ui->check_box_cari->setChecked            ( false );
        m_ui->check_box_cek->setChecked             ( false );
        m_ui->check_box_banka->setChecked           ( false );
        m_ui->check_box_sube->setChecked            ( false );
        m_ui->check_box_fatura->setChecked          ( false );
        m_ui->check_box_irsaliye->setChecked        ( false );
        m_ui->check_box_stok->setChecked            ( false );
        m_ui->check_box_personel->setChecked        ( false );
        m_ui->check_box_demirbas->setChecked        ( false );
        m_ui->check_box_sm_makbuzu->setChecked      ( false );
        m_ui->check_box_faiz->setChecked            ( false );
        m_ui->check_box_perakende->setChecked       ( false );
    }
}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::EKRANI_DUZENLE
***************************************************************************************/

void E9_KULLANICI_YETKILERI_FORMU::EKRANI_DUZENLE()
{

#ifdef ADRES_BAGIMSIZ
    int tab_size = m_ui->tabWidget_yetkiler->count();
    tab_size = tab_size - 1;
    for ( int i = tab_size; i >= 0 ; i-- ) {
        if ( m_ui->tabWidget_yetkiler->tabText(i) EQ tr("Adres") OR m_ui->tabWidget_yetkiler->tabText(i) EQ tr("Sistem") ) {
            continue;
        }
        m_ui->tabWidget_yetkiler->removeTab( i );

        m_ui->yonetim_combo_box_belge_tasarimcisi->setHidden(true);
        m_ui->label_yonetim_belge_tasarimcisi->setHidden(true);

        m_ui->yonetim_combo_box_e9_devir_islemi->setHidden(true);
        m_ui->label_ynt_devir_islemi->setHidden(true);

        m_ui->yonetim_combo_box_e9_onar->setHidden(true);
        m_ui->label_ynt_e9_onar->setHidden(true);

    }
#endif

#ifdef SM_MAKBUZU_BAGIMSIZ
    int tab_size = m_ui->tabWidget_yetkiler->count();
    tab_size = tab_size - 1;
    int smm_bulundugu_tab_no = 0;
    for ( int i = tab_size; i >= 0 ; i-- ) {
        if ( m_ui->tabWidget_yetkiler->tabText(i) EQ tr("Diğerleri") ) {
            smm_bulundugu_tab_no = i;
        }
    }

    m_ui->tabWidget_yetkiler->setCurrentIndex( smm_bulundugu_tab_no );
#endif

}

/**************************************************************************************
                   E9_KULLANICI_YETKILERI_FORMU::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int E9_KULLANICI_YETKILERI_FORMU::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    if ( p_button EQ m_ui->pushButton_profil_secin ) {
           bool degisiklik_yapildimi = false;

           OPEN_E9_PROFIL_SECME( ENUM_E9_KULLANICI_YETKILERI, NULL, &degisiklik_yapildimi );

           E9_KULLANICI_YETKILERI_STRUCT * E9_KULLANICI_YETKILERI = ( E9_KULLANICI_YETKILERI_STRUCT * ) E9_GET_KULLANICI_YETKILERI();

           if ( degisiklik_yapildimi EQ true ) {

               m_ui->check_box_muhasebe->setChecked        ( QVariant( E9_KULLANICI_YETKILERI->e9_muh_kullanabilir_mi).toBool() );
               m_ui->check_box_isletme->setChecked         ( QVariant( E9_KULLANICI_YETKILERI->e9_isl_kullanabilir_mi).toBool() );
               m_ui->check_box_adres->setChecked           ( QVariant( E9_KULLANICI_YETKILERI->e9_adr_kullanabilir_mi).toBool() );
               m_ui->check_box_doviz->setChecked           ( QVariant( E9_KULLANICI_YETKILERI->e9_dvz_kullanabilir_mi).toBool() );
               m_ui->check_box_cari->setChecked            ( QVariant( E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi).toBool() );
               m_ui->check_box_cek->setChecked             ( QVariant( E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi).toBool() );
               m_ui->check_box_banka->setChecked           ( QVariant( E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi).toBool() );
               m_ui->check_box_sube->setChecked            ( QVariant( E9_KULLANICI_YETKILERI->e9_sub_kullanabilir_mi).toBool() );
               m_ui->check_box_fatura->setChecked          ( QVariant( E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi).toBool() );
               m_ui->check_box_irsaliye->setChecked        ( QVariant( E9_KULLANICI_YETKILERI->e9_irs_kullanabilir_mi).toBool() );
               m_ui->check_box_stok->setChecked            ( QVariant( E9_KULLANICI_YETKILERI->e9_stk_kullanabilir_mi).toBool() );
               m_ui->check_box_personel->setChecked        ( QVariant( E9_KULLANICI_YETKILERI->e9_prs_kullanabilir_mi).toBool() );
               m_ui->check_box_demirbas->setChecked        ( QVariant( E9_KULLANICI_YETKILERI->e9_dmr_kullanabilir_mi).toBool() );
               m_ui->check_box_sm_makbuzu->setChecked      ( QVariant( E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi).toBool() );
               m_ui->check_box_faiz->setChecked            ( QVariant( E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi).toBool() );
               m_ui->check_box_perakende->setChecked       ( QVariant( E9_KULLANICI_YETKILERI->e9_prk_kullanabilir_mi).toBool() );

               return ADAK_RECORD_CHANGED;
           }
    }

    return ADAK_RECORD_UNCHANGED;
}
