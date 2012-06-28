#ifndef E9_YETKI_H
#define E9_YETKI_H



struct E9_KULLANICI_YETKILERI_STRUCT {
    int        sis_blg_formuna_girebilir_mi;
    int        sis_kul_formuna_girebilir_mi;
    int        sis_veritabani_formuna_girebilir_mi;
    int        sis_yonetim_log_inceleme_yetkisi;
    int        sis_mail_srv_tanimlayabilirmi;
    int        sis_sms_srv_tanimlayabilirmi;



    int        ynt_e9_yetkilendirme_yapabilir_mi;
    int        ynt_e9_devir_islemi_yapabilir_mi;
    int        ynt_e9_log_inceleme_yetkisi;
    int        ynt_e9_sabit_degerler_guncelleme_yetkisi;
    int        ynt_e9_belge_tasarlama_yetkisi;
    int        ynt_e9_ent_bilgilerini_degistirme_yetkisi;
    int        ynt_e9_kullanicinin_calistigi_sirket;
    int        ynt_e9_kullanici_sirket_degistire_bilir_mi;
    int        ynt_e9_onar_yetkisi;
    int        ynt_e9_email_log_inceleme_yetkisi;
    int        ynt_e9_urettigi_fisleri_grbilir_mi;

    int        e9_muh_kullanabilir_mi;
    int        e9_isl_kullanabilir_mi;
    int        e9_adr_kullanabilir_mi;
    int        e9_dvz_kullanabilir_mi;
    int        e9_car_kullanabilir_mi;
    int        e9_cek_kullanabilir_mi;
    int        e9_bnk_kullanabilir_mi;
    int        e9_sub_kullanabilir_mi;
    int        e9_fat_kullanabilir_mi;
    int        e9_irs_kullanabilir_mi;
    int        e9_stk_kullanabilir_mi;
    int        e9_prs_kullanabilir_mi;
    int        e9_dmr_kullanabilir_mi;
    int        e9_faiz_kullanabilir_mi;
    int        e9_prk_kullanabilir_mi;
    int        e9_smm_kullanabilir_mi;

    //ADRES
    int         adr_adres_karti_tanimlama_yetkisi;
    int         adr_grup_tanimlama_yetkisi;
    int         adr_not_defteri_inceleme_yetkisi;
    int         adr_adres_kayitlari_rapor_alma_yetkisi;
    int         adr_tel_web_kayitlari_rapor_alma_yetkisi;
    int         adr_etiket_yazdirma_yetkisi;
    int         adr_fihrist_raporu_alma_yetkisi;
    int         adr_toplu_mail_gonderme_yetkisi;
    int         adr_toplu_sms_gonderme_yetkisi;
    int         adr_export_yetkisi;
    int         adr_import_yetkisi;



    //BANKA
    int         bnk_banka_hesabi_acma_yetkisi;
    int         bnk_ent_fisleri_inceleme_yetkisi;
    int         bnk_defter_raporu_alma_yetkisi;
    int         bnk_pos_ekstresi_alma_yetkisi;
    int         bnk_hareket_fisi_islem_yetkisi;
    int         bnk_acilis_fisi_islem_yetkisi;
    int         bnk_makro_tanimlama_yetkisi;
    int         bnk_makro_fisi_islem_yetkisi;
    int         bnk_kk_ekstresi_alma_yetkisi;
    int         bnk_virman_islem_yetkisi;
    //CARI
    int         cari_tahsilat_fisi_islem_yetkisi;
    int         cari_odeme_fisi_islem_yetkisi;
    int         cari_hareket_fisi_islem_yetkisi;
    int         cari_acilis_fisi_islem_yetkisi;
    int         cari_grup_tanimlama_yetkisi;
    int         cari_kart_tanimlama_yetkisi;
    int         cari_ent_fisleri_inceleme_yetkisi;
    int         cari_ekstre_alma_yetkisi;
    int         cari_bakiye_raporu_alma_yetkisi;
    int         cari_virman_islem_yetkisi;
    int         cari_hesap_hareketleri_gorebilir_mi;
    //CEKSENET
    int         cek_senet_girebilir_mi;
    int         cek_bordrosu_islem_yetkisi;
    int         cek_acilis_bordrosu_islem_yetkisi;
    int         cek_ent_fisleri_inceleme_yetkisi;
    int         cek_listesi_rapor_alma_yetkisi;
    int         cek_mus_cek_senet_arama_yetkisi;
    //DEMIRBAS
    int         dmr_demirbas_karti_tanimlama_yetkisi;
    int         dmr_grup_tanimlama_yetkisi;
    int         dmr_amortisman_fisi_islem_yetkisi;
    int         dmr_yen_degerleme_fisi_islem_yetkisi;
    int         dmr_demirbas_listesi_rapor_alma_yetkisi;
    int         dmr_toplu_amortisman_ayirma_yetkisi;
    int         dmr_toplu_yeniden_degerleme_yetkisi;
    int         dmr_rapor_islem_grubu_tanimlama_yetkisi;

    //ISLETME
    int         isl_isletme_defteri_rapor_alma_yetkisi;
    int         isl_kdv_raporu_alma_yetkisi;
    int         isl_fisi_islem_yetkisi;

    //MUHASEBE
    int         muh_muhasebe_fisi_islem_yetkisi;
    int         muh_hesap_plani_islem_yetkisi;
    int         muh_yvm_no_olusturma_yetkisi;
    int         muh_hesap_ekstresi_alma_yetkisi;
    int         muh_mizan_raporu_alma_yetkisi;
    int         muh_hesap_plani_rapor_alma_yetkisi;
    int         muh_gelir_tablosu_rapor_alma_yetkisi;
    int         muh_bilanco_rapor_alma_yetkisi;
    int         muh_yvm_dokumu_alma_yetkisi;
    int         muh_defteri_kebir_raporu_alma_yetkisi;
    int         muh_fis_dokumu_alma_yetkisi;
    int         muh_hatali_fisleri_inceleme_yetkisi;
    int         muh_ent_fisleri_inceleme_yetkisi;
    int         muh_grup_tanimlama_yetkisi;
    int         muh_yansitma_fisi_olusturma_yetkisi;
    int         muh_yansitma_hesabi_tanimlama_yetkisi;
    int         muh_hesap_plani_kopyalama_yetkisi;
    int         muh_hesap_ekstresi_nakli_yekun;
    int         muh_mizan_nakli_yekun;
    int         muh_acilis_fisi_islem_yetkisi;

    //PERSONEL

    int         prs_sicil_karti_tanimlama_yetkisi;
    int         prs_ek_odenek_tanimlama_yetkisi;
    int         prs_ek_kesinti_tanimlama_yetkisi;
    int         prs_sabit_deger_guncelleme_yetkisi;
    int         prs_maas_bordrosu_alma_yetkisi;
    int         prs_maas_bordrosu_iptal_yetkisi;
    int         prs_maas_bordrosu_raporu_alma_yetkisi;
    int         prs_isten_ayrilma_islem_yetkisi;
    int         prs_ihbar_ve_kidem_tazminati_raporu_alma_yetkisi;
    int         prs_bordro_inceleme_yetkisi;
    int         prs_personel_raporu_alma_yetkisi;
    int         prs_isten_ayrilan_personel_arama_yetkisi;
    int         prs_grup_tanimlama_yetkisi;
    int         prs_e_bildirge_alma_yetkisi;
    int         prs_eksik_gun_nedenleri_giris_yetkisi;
    int         prs_ucret_pusulasi_raporu_alma_yetkisi;

    //STOK
    int         stok_urun_karti_tanimlama_yetkisi;
    int         stok_hizmet_karti_tanimlama_yetkisi;
    int         stok_fisi_islem_yetkisi;
    int         stok_grup_tanimlama_yetkisi;
    int         stok_urun_listesi_alma_yetkisi;
    int         stok_miktar_raporu_alma_yetkisi;
    int         stok_ekstresi_alma_yetkisi;
    int         stok_urun_hareketlerini_gorebilir_mi;

    //FATURA

    int         fat_alis_faturasi_kesebilir_mi;
    int         fat_sts_faturasi_kesebilir_mi;

    int         fat_irs_faturalastirma_elle;
    int         fat_irs_faturalastirma_oto;
    int         fat_irs_faturalastirma_toplu;
    int         fat_satir_iskontosu_yapabilir_mi;
    int         fat_toplu_iskonto_yapabilir_mi;
    int         fat_hizmet_ekstresi_alma_yetkisi;
    int         fat_ba_bs_listesi_alma_yetkisi;
    int         fat_fatura_listesi_alma_yetkisi;

    //İRSALİYE

    int         irs_alis_irsaliyesi_islem_yetkisi;
    int         irs_sts_irsaliyesi_islem_yetkisi;
    int         irs_fat_irsaliyelestirme_elle;
    int         irs_fat_irsaliyelestirme_oto;
    int         irs_satir_iskontosu_yapabilir_mi;
    int         irs_listesi_gorebilir_mi ;
    int         irs_toplu_iskonto_yapabilir_mi;


    //SUBE DEPO UNITE
    int         sub_sube_tanimlama_yetkisi;
    int         sub_depo_tanimlama_yetkisi;
    int         sub_depo_islem_yetkisi;
    int         sub_sube_islem_yetkisi;
    int         sub_ent_hesaplari_guncelleme_yetkisi;

    //SMM

    int         smm_makbuzu_islem_yetkisi;
    int         smm_hizmet_tanimlama_yetkisi;
    int         smm_tahsil_edildi_odendi_secili_gelsin;

    //DOVIZ
    int         dvz_dovizleri_inceleme_yetkisi;
    int         dvz_doviz_tanimlama_yetkisi;
    int         dvz_kur_guncelleme_yetkisi;

    //PERAKENDE

    int         prk_fis_girebilir_mi;
    int         prk_gdr_tur_tanimlayabilir_mi;
    int         prk_kasa_raporu_alabilir_mi;
};

void        E9_KULLANICIYI_TAM_YETKILENDIR          ();

void        E9_KULLANICI_YETKILERI_FISI_VE_KULLANICI_YETKILERI_ORDER_NUM_OLUSTUR ();

void    *   E9_GET_KULLANICI_YETKILERI              ();

void        E9_KULLANICIYI_YETKILERI_UPDATE         ( E9_KULLANICI_YETKILERI_STRUCT * P_YETKI_STRUCT , int kullanici_id );



#endif // E9_YETKI_H
