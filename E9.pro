include (ADAK/INCLUDE/adak.pri)

QT += sql \
    network \
    webkit \
    xml
TARGET   = E9
TEMPLATE = app

LIBS+= ADAK/$$OBJDIR/$$libADAK
PRE_TARGETDEPS+= ADAK/$$OBJDIR/$$libADAK
win32 {
     LIBS += -liphlpapi
}

DEPENDPATH += . \
    ADRES \
    BANKA \
    CARI \
    CEKSENET \
    DEMIRBAS \
    DOVIZ \
    FAIZ \
    ISLETME \
    MUHASEBE \
    E9_ONAR \
    PERSONEL \
    STOK \
    SUBE_DEPO_UNITE \
    IRSALIYE \
    FATURA \
    PERAKENDE \
    SMM_MAKBUZ

INCLUDEPATH += . \
    ADAK/INCLUDE \
    ADRES \
    BANKA \
    CARI \
    CEKSENET \
    DEMIRBAS \
    DOVIZ \
    FAIZ \
    ISLETME \
    MUHASEBE \
    E9_ONAR \
    PERSONEL \
    STOK \
    SUBE_DEPO_UNITE \
    IRSALIYE \
    FATURA \
    PERAKENDE \
    SMM_MAKBUZ

# ###################################         E9                     #####################
SOURCES += \
    e9_main.cpp \
    e9_kullanici_yetkileri_formu.cpp \
    e9_devir_islemi_batch.cpp \
    e9_sabit_degerler_formu.cpp \
    e9_log_kaydi_arama.cpp \
    e9_ent_detaylari_batch.cpp \
    e9_eft_batch.cpp \
    e9_kk_odeme_batch.cpp \
    e9_kk_tahsilat_batch.cpp \
    e9_version_upgrade.cpp \
    e9_gui_utils.cpp \
    e9_open.cpp \
    e9_console_utils.cpp \
    e9_ana_menu.cpp \
    e9_uretilen_fis_secimi.cpp  \
    e9_kul_kitapcigi_ve_yardim_sayfasi_batch.cpp \
    e9_profil_secme_batch.cpp \

HEADERS += \
    e9_yetki.h \
    e9_log.h \
    e9_version_upgrade.h \
    e9_gui_utils.h \
    e9_devir_islemi_batch_class.h \
    e9_devir_islemi_batch_open.h \
    e9_eft_batch_class.h \
    e9_eft_batch_open.h \
    e9_ent_detaylari_batch_class.h \
    e9_ent_detaylari_batch_open.h \
    e9_kk_odeme_batch_class.h \
    e9_kk_odeme_batch_open.h \
    e9_kk_tahsilat_batch_class.h \
    e9_kk_tahsilat_batch_open.h \
    e9_kullanici_yetkileri_formu_class.h \
    e9_kullanici_yetkileri_formu_open.h \
    e9_log_kaydi_arama_class.h \
    e9_log_kaydi_arama_open.h \
    e9_sabit_degerler_formu_class.h \
    e9_sabit_degerler_formu_open.h \
    e9_open.h \
    e9_struct.h \
    e9_enum.h \
    e9_console_utils.h \
    e9_ana_menu_class.h \
    e9_ana_menu_open.h  \
    e9_uretilen_fis_secimi_class.h \
    e9_uretilen_fis_secimi_open.h   \
    e9_kul_kitapcigi_ve_yardim_sayfasi_batch_class.h   \
    e9_kul_kitapcigi_ve_yardim_sayfasi_batch_open.h \
    e9_profil_secme_batch.h \


FORMS += \
    e9_kullanici_yetkileri_formu.ui \
    e9_devir_islemi_batch.ui \
    e9_sabit_degerler_formu.ui \
    e9_log_kaydi_arama.ui \
    e9_ent_detaylari_batch.ui \
    e9_eft_batch.ui \
    e9_kk_odeme_batch.ui \
    e9_kk_tahsilat_batch.ui \
    e9_profil_secme.ui  \
    e9_kul_kitapcigi_ve_yardim_sayfasi_batch.ui


RESOURCES += e9_icons.qrc

# ####################################       E9_ONAR                #######################
SOURCES += E9_ONAR/e9_onar_batch.cpp \
        E9_ONAR/e9_onar_utils.cpp
HEADERS += \
        E9_ONAR/e9_onar_utils.h \
        E9_ONAR/e9_onar_batch_class.h \
        E9_ONAR/e9_onar_batch_open.h

FORMS   += E9_ONAR/e9_onar_batch.ui

# ###################################         ADRES                  #####################
SOURCES += \
    ADRES/adres_not_defteri_fisi.cpp \
    ADRES/adres_arama.cpp \
    ADRES/adres_etiketleri_batch.cpp \
    ADRES/adres_etiketleri.cpp \
    ADRES/adres_kayitlari_raporu.cpp \
    ADRES/adres_gui_utils.cpp \
    ADRES/adres_console_utils.cpp \
    ADRES/adres_zarf_yazdir_batch.cpp \
    ADRES/adres_toplu_mail_gonderimi_batch.cpp \
    ADRES/adres_exe.cpp \
    ADRES/adres_fihrist_raporu_batch.cpp \
    ADRES/adres_fihrist_raporu.cpp \
    ADRES/adres_telefon_fisi.cpp \
    ADRES/adres_email_fisi.cpp \
    ADRES/adres_rapor_filtresi_batch.cpp    \
    ADRES/adres_kayit_formu.cpp \
    ADRES/adres_email_log_batch.cpp \
    ADRES/adres_email_raporu.cpp \
    ADRES/adres_export.cpp \
    ADRES/adres_import.cpp \
    ADRES/adres_yetkililer_fisi.cpp \
    ADRES/adres_open.cpp

HEADERS += \
    ADRES/adres_gui_utils.h \
    ADRES/adres_console_utils.h \
    ADRES/adres_arama_open.h \
    ADRES/adres_email_fisi_open.h \
    ADRES/adres_email_fisi_class.h \
    ADRES/adres_email_log_batch_open.h \
    ADRES/adres_email_log_batch_class.h \
    ADRES/adres_arama_class.h \
    ADRES/adres_etiketleri_batch_open.h \
    ADRES/adres_etiketleri_batch_class.h \
    ADRES/adres_export_open.h \
    ADRES/adres_export_class.h \
    ADRES/adres_fihrist_raporu_open.h \
    ADRES/adres_fihrist_raporu_class.h \
    ADRES/adres_fihrist_raporu_batch_open.h \
    ADRES/adres_fihrist_raporu_batch_class.h \
    ADRES/adres_import_open.h \
    ADRES/adres_import_class.h \
    ADRES/adres_kayit_formu_open.h \
    ADRES/adres_kayit_formu_class.h \
    ADRES/adres_kayitlari_raporu_open.h \
    ADRES/adres_kayitlari_raporu_class.h \
    ADRES/adres_not_defteri_fisi_open.h \
    ADRES/adres_not_defteri_fisi_class.h \
    ADRES/adres_rapor_filtresi_batch_open.h \
    ADRES/adres_rapor_filtresi_batch_class.h \
    ADRES/adres_telefon_fisi_open.h \
    ADRES/adres_telefon_fisi_class.h \
    ADRES/adres_toplu_mail_gonderimi_batch_open.h \
    ADRES/adres_toplu_mail_gonderimi_batch_class.h \
    ADRES/adres_yetkililer_fisi_open.h \
    ADRES/adres_yetkililer_fisi_class.h \
    ADRES/adres_zarf_yazdir_batch_open.h \
    ADRES/adres_zarf_yazdir_batch_class.h \
    ADRES/adres_open.h \
    ADRES/adres_enum.h \
    ADRES/adres_struct.h \
    ADRES/adres_etiketleri_class.h \
    ADRES/adres_etiketleri_open.h \
    ADRES/adres_email_raporu_class.h \
    ADRES/adres_email_raporu_open.h \
    ADRES/adres_exe.h

FORMS += \
    ADRES/adres_kayit_formu.ui \
    ADRES/adres_not_defteri_fisi.ui \
    ADRES/adres_arama.ui \
    ADRES/adres_etiketleri_batch.ui \
    ADRES/adres_zarf_yazdir_batch.ui \
    ADRES/adres_toplu_mail_gonderimi_batch.ui \
    ADRES/adres_fihrist_raporu_batch.ui \
    ADRES/adres_telefon_fisi.ui \
    ADRES/adres_email_fisi.ui \
    ADRES/adres_rapor_filtresi_batch.ui \
    ADRES/adres_email_log_batch.ui \
    ADRES/adres_yetkililer_fisi.ui

# ###################################         BANKA                  #####################
SOURCES += BANKA/bnk_gui_utils.cpp \
    BANKA/bnk_console_utils.cpp \
    BANKA/bnk_banka_defteri_batch.cpp \
    BANKA/bnk_banka_defteri_raporu.cpp \
    BANKA/bnk_pos_ekstresi_batch.cpp \
    BANKA/bnk_pos_ekstresi.cpp \
    BANKA/bnk_fisi_yazdir.cpp \
    BANKA/bnk_hareket_fisi.cpp \
    BANKA/bnk_makro_tanimlama_fisi.cpp \
    BANKA/bnk_makro_secimi.cpp \
    BANKA/bnk_makro_fisi.cpp \
    BANKA/bnk_hesaplari_fisi.cpp \
    BANKA/bnk_hesaplari_arama.cpp \
    BANKA/bnk_hesabin_kredi_kartlari_fisi.cpp \
    BANKA/bnk_pos_secimi.cpp \
    BANKA/bnk_kredi_karti_secimi.cpp \
    BANKA/bnk_kk_ekstresi_batch.cpp \
    BANKA/bnk_kk_ekstresi_raporu.cpp \
    BANKA/bnk_fis_utils.cpp\
    BANKA/bnk_makro_fisi_arama.cpp \
    BANKA/bnk_hareket_fisi_arama.cpp    \
    BANKA/bnk_acilis_formu.cpp  \
    BANKA/bnk_virman_formu.cpp \
    BANKA/bnk_defter_fis_utils.cpp \
    BANKA/banka_open.cpp


HEADERS += BANKA/bnk_gui_utils.h \
    BANKA/bnk_console_utils.h \
    BANKA/bnk_fis_utils.h \
    BANKA/bnk_acilis_formu_class.h \
    BANKA/bnk_acilis_formu_open.h \
    BANKA/banka_open.h \
    BANKA/bnk_banka_defteri_batch_class.h \
    BANKA/bnk_banka_defteri_batch_open.h \
    BANKA/bnk_hareket_fisi_class.h \
    BANKA/bnk_hareket_fisi_open.h \
    BANKA/bnk_hareket_fisi_arama_class.h \
    BANKA/bnk_hareket_fisi_arama_open.h \
    BANKA/bnk_hesabin_kredi_kartlari_fisi_class.h \
    BANKA/bnk_hesabin_kredi_kartlari_fisi_open.h \
    BANKA/bnk_hesaplari_arama_class.h \
    BANKA/bnk_hesaplari_arama_open.h \
    BANKA/bnk_hesaplari_fisi_class.h \
    BANKA/bnk_hesaplari_fisi_open.h \
    BANKA/bnk_kk_ekstresi_batch_class.h \
    BANKA/bnk_kk_ekstresi_batch_open.h \
    BANKA/bnk_kredi_karti_secimi_class.h \
    BANKA/bnk_kredi_karti_secimi_open.h \
    BANKA/bnk_makro_fisi_class.h \
    BANKA/bnk_makro_fisi_open.h \
    BANKA/bnk_makro_fisi_arama_class.h \
    BANKA/bnk_makro_fisi_arama_open.h \
    BANKA/bnk_makro_secimi_class.h \
    BANKA/bnk_makro_secimi_open.h \
    BANKA/bnk_makro_tanimlama_fisi_class.h \
    BANKA/bnk_makro_tanimlama_fisi_open.h \
    BANKA/bnk_pos_ekstresi_batch_class.h \
    BANKA/bnk_pos_ekstresi_batch_open.h \
    BANKA/bnk_pos_secimi_class.h \
    BANKA/bnk_pos_secimi_open.h \
    BANKA/bnk_virman_formu_class.h \
    BANKA/bnk_virman_formu_open.h \
    BANKA/bnk_banka_defteri_raporu_class.h \
    BANKA/bnk_banka_defteri_raporu_open.h \
    BANKA/bnk_fisi_yazdir_class.h \
    BANKA/bnk_fisi_yazdir_open.h \
    BANKA/bnk_kk_ekstresi_raporu_class.h \
    BANKA/bnk_kk_ekstresi_raporu_open.h \
    BANKA/bnk_pos_ekstresi_class.h \
    BANKA/bnk_pos_ekstresi_open.h \
    BANKA/bnk_defter_fis_utils.h \
    BANKA/banka_enum.h \
    BANKA/banka_struct.h

FORMS += BANKA/bnk_banka_defteri_batch.ui \
    BANKA/bnk_pos_ekstresi_batch.ui \
    BANKA/bnk_hareket_fisi.ui \
    BANKA/bnk_makro_tanimlama_fisi.ui \
    BANKA/bnk_makro_fisi.ui \
    BANKA/bnk_hesaplari_fisi.ui \
    BANKA/bnk_hesaplari_arama.ui \
    BANKA/bnk_hesabin_kredi_kartlari_fisi.ui \
    BANKA/bnk_kk_ekstresi_batch.ui \
    BANKA/bnk_makro_fisi_arama.ui \
    BANKA/bnk_hareket_fisi_arama.ui \
    BANKA/bnk_acilis_formu.ui   \
    BANKA/bnk_virman_formu.ui

# ##################################         CARI                #####################
SOURCES += CARI/cari_tahsilat_fisi.cpp \
    CARI/cari_hareket_fisi.cpp \
    CARI/cari_fis_arama.cpp \
    CARI/cari_odeme_fisi.cpp \
    CARI/cari_console_utils.cpp \
    CARI/cari_gui_utils.cpp \
    CARI/cari_bakiye_raporu_batch.cpp \
    CARI/cari_bakiye_raporu.cpp \
    CARI/cari_fisi_yazdir_raporu.cpp \
    CARI/cari_tahsilat_ceksenet_batch.cpp \
    CARI/cari_odeme_ceksenet_batch.cpp \
    CARI/cari_virman_formu.cpp \
    CARI/cari_kart_arama.cpp \
    CARI/cari_hesap_ekstresi_batch.cpp \
    CARI/cari_hesap_ekstresi_raporu.cpp \
    CARI/cari_fis_utils.cpp \
    CARI/cari_kart_formu.cpp \
    CARI/cari_open.cpp  \
    CARI/cari_oto_ekstre_arama.cpp \
    CARI/cari_kart_export.cpp   \
    CARI/cari_kart_import.cpp   \
    CARI/cari_makbuz_belgesi.cpp

HEADERS += \
    CARI/cari_console_utils.h \
    CARI/cari_gui_utils.h \
    CARI/cari_fis_utils.h   \
    CARI/cari_bakiye_raporu_class.h \
    CARI/cari_bakiye_raporu_open.h  \
    CARI/cari_bakiye_raporu_batch_class.h \
    CARI/cari_bakiye_raporu_batch_open.h \
    CARI/cari_fis_arama_class.h \
    CARI/cari_fis_arama_open.h \
    CARI/cari_fisi_yazdir_raporu_class.h \
    CARI/cari_fisi_yazdir_raporu_open.h \
    CARI/cari_hareket_fisi_class.h \
    CARI/cari_hareket_fisi_open.h \
    CARI/cari_hesap_ekstresi_batch_class.h \
    CARI/cari_hesap_ekstresi_batch_open.h \
    CARI/cari_hesap_ekstresi_raporu_class.h \
    CARI/cari_hesap_ekstresi_raporu_open.h \
    CARI/cari_kart_arama_class.h \
    CARI/cari_kart_arama_open.h \
    CARI/cari_odeme_ceksenet_batch_class.h \
    CARI/cari_odeme_ceksenet_batch_open.h \
    CARI/cari_odeme_fisi_class.h \
    CARI/cari_odeme_fisi_open.h \
    CARI/cari_tahsilat_ceksenet_batch_class.h \
    CARI/cari_tahsilat_ceksenet_batch_open.h \
    CARI/cari_tahsilat_fisi_class.h \
    CARI/cari_tahsilat_fisi_open.h \
    CARI/cari_virman_formu_class.h \
    CARI/cari_virman_formu_open.h   \
    CARI/cari_kart_formu_class.h \
    CARI/cari_kart_formu_open.h \
    CARI/cari_open.h    \
    CARI/cari_enum.h \
    CARI/cari_struct.h  \
    CARI/cari_oto_ekstre_arama_class.h  \
    CARI/cari_oto_ekstre_arama_open.h \
    CARI/cari_kart_export_class.h \
    CARI/cari_kart_export_open.h \
    CARI/cari_kart_import_class.h \
    CARI/cari_kart_import_open.h    \
    CARI/cari_makbuz_belgesi_open.h    \
    CARI/cari_makbuz_belgesi_class.h


FORMS += CARI/cari_tahsilat_fisi.ui \
    CARI/cari_hareket_fisi.ui \
    CARI/cari_fis_arama.ui \
    CARI/cari_odeme_fisi.ui \
    CARI/cari_bakiye_raporu_batch.ui \
    CARI/cari_tahsilat_ceksenet_batch.ui \
    CARI/cari_odeme_ceksenet_batch.ui \
    CARI/cari_virman_formu.ui \
    CARI/cari_kart_formu.ui \
    CARI/cari_kart_arama.ui \
    CARI/cari_hesap_ekstresi_batch.ui   \
    CARI/cari_oto_ekstre_arama.ui


# ###################################         CEKSENET                     #####################
SOURCES += CEKSENET/cek_console_utils.cpp \
    CEKSENET/cek_gui_utils.cpp \
    CEKSENET/cek_senet_bordrosu_fisi.cpp \
    CEKSENET/cek_senet_bordrosu_arama.cpp \
    CEKSENET/cek_musteri_cek_senet_arama.cpp \
    CEKSENET/cek_fisi_yazdir_raporu.cpp \
    CEKSENET/cek_listesi_batch.cpp \
    CEKSENET/cek_listesi_raporu.cpp \
    CEKSENET/cek_acilis_fisi.cpp \
    CEKSENET/cek_senet_arama.cpp \
    CEKSENET/cek_senet_formu.cpp \
    CEKSENET/cek_fis_utils.cpp \
    CEKSENET/cek_senet_utils.cpp \
    CEKSENET/cek_open.cpp
HEADERS += \
    CEKSENET/cek_console_utils.h \
    CEKSENET/cek_gui_utils.h \
    CEKSENET/cek_fis_utils.h \
    CEKSENET/cek_senet_utils.h \
    CEKSENET/cek_acilis_fisi_class.h \
    CEKSENET/cek_acilis_fisi_open.h \
    CEKSENET/cek_fisi_yazdir_raporu_class.h \
    CEKSENET/cek_fisi_yazdir_raporu_open.h \
    CEKSENET/cek_listesi_batch_class.h \
    CEKSENET/cek_listesi_batch_open.h \
    CEKSENET/cek_listesi_raporu_class.h \
    CEKSENET/cek_listesi_raporu_open.h \
    CEKSENET/cek_musteri_cek_senet_arama_class.h \
    CEKSENET/cek_musteri_cek_senet_arama_open.h \
    CEKSENET/cek_senet_arama_class.h \
    CEKSENET/cek_senet_arama_open.h \
    CEKSENET/cek_senet_bordrosu_arama_class.h \
    CEKSENET/cek_senet_bordrosu_arama_open.h \
    CEKSENET/cek_senet_bordrosu_fisi_class.h \
    CEKSENET/cek_senet_bordrosu_fisi_open.h \
    CEKSENET/cek_senet_formu_class.h \
    CEKSENET/cek_senet_formu_open.h \
    CEKSENET/cek_enum.h \
    CEKSENET/cek_open.h \
    CEKSENET/cek_struct.h


FORMS += CEKSENET/cek_senet_bordrosu_fisi.ui \
    CEKSENET/cek_senet_bordrosu_arama.ui \
    CEKSENET/cek_musteri_cek_senet_arama.ui \
    CEKSENET/cek_listesi_batch.ui \
    CEKSENET/cek_acilis_fisi.ui \
    CEKSENET/cek_senet_formu.ui \
    CEKSENET/cek_senet_arama.ui

# ###################################         DEMIRBAS                     #####################
SOURCES += \
    DEMIRBAS/dmr_demirbas_karti.cpp \
    DEMIRBAS/dmr_demirbas_karti_arama.cpp \
    DEMIRBAS/dmr_yeniden_degerleme_fisi.cpp \
    DEMIRBAS/dmr_gui_utils.cpp \
    DEMIRBAS/dmr_con_utils.cpp \
    DEMIRBAS/dmr_yeniden_degerleme_batch.cpp \
    DEMIRBAS/dmr_fis_arama.cpp \
    DEMIRBAS/dmr_amortisman_fisi.cpp \
    DEMIRBAS/dmr_amortisman_ayirma_batch.cpp \
    DEMIRBAS/dmr_demirbas_listesi_batch.cpp \
    DEMIRBAS/dmr_demirbas_listesi.cpp \
    DEMIRBAS/dmr_fis_utils.cpp \
    DEMIRBAS/dmr_grup_tree_secimi.cpp \
    DEMIRBAS/dmr_satis_formu.cpp \
    DEMIRBAS/dmr_open.cpp
HEADERS += \
    DEMIRBAS/dmr_gui_utils.h \
    DEMIRBAS/dmr_con_utils.h \
    DEMIRBAS/dmr_fis_utils.h \
    DEMIRBAS/dmr_amortisman_ayirma_batch_class.h \
    DEMIRBAS/dmr_amortisman_ayirma_batch_open.h \
    DEMIRBAS/dmr_amortisman_fisi_class.h \
    DEMIRBAS/dmr_amortisman_fisi_open.h \
    DEMIRBAS/dmr_demirbas_karti_open.h \
    DEMIRBAS/dmr_demirbas_karti_arama_class.h \
    DEMIRBAS/dmr_demirbas_karti_arama_open.h \
    DEMIRBAS/dmr_demirbas_listesi_class.h \
    DEMIRBAS/dmr_demirbas_listesi_open.h \
    DEMIRBAS/dmr_fis_arama_class.h \
    DEMIRBAS/dmr_fis_arama_open.h \
    DEMIRBAS/dmr_grup_tree_secimi_class.h \
    DEMIRBAS/dmr_grup_tree_secimi_open.h \
    DEMIRBAS/dmr_satis_formu_class.h \
    DEMIRBAS/dmr_satis_formu_open.h \
    DEMIRBAS/dmr_yeniden_degerleme_batch_class.h \
    DEMIRBAS/dmr_yeniden_degerleme_batch_open.h \
    DEMIRBAS/dmr_yeniden_degerleme_fisi_class.h \
    DEMIRBAS/dmr_yeniden_degerleme_fisi_open.h \
    DEMIRBAS/dmr_demirbas_listesi_batch_class.h \
    DEMIRBAS/dmr_demirbas_listesi_batch_open.h  \
    DEMIRBAS/dmr_demirbas_karti_class.h \
    DEMIRBAS/dmr_enum.h \
    DEMIRBAS/dmr_open.h \
    DEMIRBAS/dmr_struct.h

FORMS += \
    DEMIRBAS/dmr_demirbas_karti.ui \
    DEMIRBAS/dmr_demirbas_karti_arama.ui \
    DEMIRBAS/dmr_yeniden_degerleme_fisi.ui \
    DEMIRBAS/dmr_yeniden_degerleme_batch.ui \
    DEMIRBAS/dmr_fis_arama.ui \
    DEMIRBAS/dmr_amortisman_fisi.ui \
    DEMIRBAS/dmr_amortisman_ayirma_batch.ui \
    DEMIRBAS/dmr_demirbas_listesi_batch.ui \
    DEMIRBAS/dmr_satis_formu.ui

# ###################################         DOVIZ                        #####################
HEADERS +=  DOVIZ/dvz_console_utils.h \
            DOVIZ/dvz_dovizler_batch_class.h \
            DOVIZ/dvz_kur_guncelle_batch_class.h \
            DOVIZ/dvz_tanimlari_batch_class.h \
            DOVIZ/dvz_dovizler_batch_open.h \
            DOVIZ/dvz_kur_guncelle_batch_open.h \
            DOVIZ/dvz_tanimlari_batch_open.h \
            DOVIZ/dvz_gui_utils.h \
            DOVIZ/dvz_enum.h \
            DOVIZ/dvz_struct.h \
            DOVIZ/dvz_open.h    \
            DOVIZ/dvz_doviz_kurlari_raporu_class.h \
            DOVIZ/dvz_doviz_kurlari_raporu_open.h

FORMS +=    DOVIZ/dvz_dovizler_batch.ui \
            DOVIZ/dvz_tanimlari_batch.ui \
            DOVIZ/dvz_kur_guncelle_batch.ui

SOURCES +=  DOVIZ/dvz_console_utils.cpp \
            DOVIZ/dvz_dovizler_batch.cpp \
            DOVIZ/dvz_tanimlari_batch.cpp \
            DOVIZ/dvz_kur_guncelle_batch.cpp \
            DOVIZ/dvz_gui_utils.cpp \
            DOVIZ/dvz_open.cpp  \
            DOVIZ/dvz_doviz_kurlari_raporu.cpp

# ###################################         FAIZ                         #####################
SOURCES   += FAIZ/faiz_taksit_raporu.cpp \
             FAIZ/faiz_hesapla_batch.cpp \
             FAIZ/faiz_console_utils.cpp \
             FAIZ/faiz_open.cpp
HEADERS   += \
             FAIZ/faiz_console_utils.h \
             FAIZ/faiz_hesapla_batch_class.h \
             FAIZ/faiz_taksit_raporu_class.h \
             FAIZ/faiz_hesapla_batch_open.h \
             FAIZ/faiz_taksit_raporu_open.h \
             FAIZ/faiz_open.h \
             FAIZ/faiz_enum.h \
             FAIZ/faiz_struct.h

FORMS     += FAIZ/faiz_hesapla_batch.ui

# ###################################         ISLETME                      #####################
SOURCES += \
    ISLETME/isl_defteri_fisi.cpp \
    ISLETME/isl_defteri_raporu_batch.cpp \
    ISLETME/isl_defteri_raporu.cpp \
    ISLETME/isl_fis_arama.cpp \
    ISLETME/isl_fis_yazdir.cpp \
    ISLETME/isl_kdv_raporu_batch.cpp \
    ISLETME/isl_kdv_raporu.cpp \
    ISLETME/isl_gui_utils.cpp \
    ISLETME/isl_console_utils.cpp \
    ISLETME/isl_fis_kopyalama_batch.cpp \
    ISLETME/isl_fis_utils.cpp \
    ISLETME/isl_open.cpp \
    ISLETME/isl_hesap_ozeti_batch.cpp \
    ISLETME/isl_hesap_ozeti_raporu.cpp

HEADERS += \
    ISLETME/isl_gui_utils.h \
    ISLETME/isl_console_utils.h \
    ISLETME/isl_fis_utils.h \
    ISLETME/isl_defteri_fisi_class.h \
    ISLETME/isl_defteri_fisi_open.h \
    ISLETME/isl_defteri_raporu_batch_class.h \
    ISLETME/isl_defteri_raporu_batch_open.h \
    ISLETME/isl_fis_arama_class.h \
    ISLETME/isl_fis_arama_open.h \
    ISLETME/isl_fis_kopyalama_batch_class.h \
    ISLETME/isl_fis_kopyalama_batch_open.h \
    ISLETME/isl_kdv_raporu_batch_class.h \
    ISLETME/isl_kdv_raporu_batch_open.h \
    ISLETME/isl_defteri_raporu_class.h \
    ISLETME/isl_defteri_raporu_open.h \
    ISLETME/isl_fis_yazdir_class.h \
    ISLETME/isl_fis_yazdir_open.h \
    ISLETME/isl_kdv_raporu_class.h \
    ISLETME/isl_kdv_raporu_open.h \
    ISLETME/isl_enum.h \
    ISLETME/isl_open.h \
    ISLETME/isl_struct.h \
    ISLETME/isl_hesap_ozeti_batch_class.h \
    ISLETME/isl_hesap_ozeti_batch_open.h \
    ISLETME/isl_hesap_ozeti_raporu_open.h \
    ISLETME/isl_hesap_ozeti_raporu_class.h
FORMS += \
    ISLETME/isl_defteri_fisi.ui \
    ISLETME/isl_defteri_raporu_batch.ui \
    ISLETME/isl_fis_arama.ui \
    ISLETME/isl_kdv_raporu_batch.ui \
    ISLETME/isl_fis_kopyalama_batch.ui \
    ISLETME/isl_hesap_ozeti_batch.ui

# ###################################         MUHASEBE                     #####################
HEADERS +=   MUHASEBE/muh_console_utils.h \
    MUHASEBE/muh_gui_utils.h \
    MUHASEBE/muh_fis_utils.h \
    MUHASEBE/muh_bilanco_raporu_batch_open.h \
    MUHASEBE/muh_bilanco_raporu_batch_class.h \
    MUHASEBE/muh_defteri_kebir_batch_open.h \
    MUHASEBE/muh_defteri_kebir_batch_class.h \
    MUHASEBE/muh_fis_arama_open.h \
    MUHASEBE/muh_fis_arama_class.h \
    MUHASEBE/muh_fis_kopyalama_batch_open.h \
    MUHASEBE/muh_fis_kopyalama_batch_class.h \
    MUHASEBE/muh_gelir_tablosu_batch_open.h \
    MUHASEBE/muh_gelir_tablosu_batch_class.h \
    MUHASEBE/muh_hatali_fis_secimi_open.h \
    MUHASEBE/muh_hatali_fis_secimi_class.h \
    MUHASEBE/muh_hesap_arama_open.h \
    MUHASEBE/muh_hesap_arama_class.h \
    MUHASEBE/muh_hesap_ekstresi_batch_open.h \
    MUHASEBE/muh_hesap_ekstresi_batch_class.h \
    MUHASEBE/muh_hesap_plani_open.h \
    MUHASEBE/muh_hesap_plani_class.h \
    MUHASEBE/muh_hesap_plani_kopyala_batch_open.h \
    MUHASEBE/muh_hesap_plani_kopyala_batch_class.h \
    MUHASEBE/muh_hesap_plani_raporu_batch_open.h \
    MUHASEBE/muh_hesap_plani_raporu_batch_class.h \
    MUHASEBE/muh_kdv_ayirma_batch_open.h \
    MUHASEBE/muh_kdv_ayirma_batch_class.h \
    MUHASEBE/muh_mizan_raporu_batch_open.h \
    MUHASEBE/muh_mizan_raporu_batch_class.h \
    MUHASEBE/muh_muhasebe_fisi_open.h \
    MUHASEBE/muh_muhasebe_fisi_class.h \
    MUHASEBE/muh_toplu_fis_raporu_batch_open.h \
    MUHASEBE/muh_toplu_fis_raporu_batch_class.h \
    MUHASEBE/muh_yansitma_hesaplari_fisi_open.h \
    MUHASEBE/muh_yansitma_hesaplari_fisi_class.h \
    MUHASEBE/muh_yansitma_islemi_batch_open.h \
    MUHASEBE/muh_yansitma_islemi_batch_class.h \
    MUHASEBE/muh_yevmiye_dokumu_batch_open.h \
    MUHASEBE/muh_yevmiye_dokumu_batch_class.h \
    MUHASEBE/muh_yevmiye_no_olustur_batch_open.h \
    MUHASEBE/muh_yevmiye_no_olustur_batch_class.h \
    MUHASEBE/muh_mizan_raporu_open.h \
    MUHASEBE/muh_mizan_raporu_class.h \
    MUHASEBE/muh_hesap_ekstresi_raporu_open.h \
    MUHASEBE/muh_hesap_ekstresi_raporu_class.h \
    MUHASEBE/muh_hesap_plani_raporu_open.h \
    MUHASEBE/muh_hesap_plani_raporu_class.h \
    MUHASEBE/muh_yevmiye_dokumu_raporu_open.h \
    MUHASEBE/muh_yevmiye_dokumu_raporu_class.h \
    MUHASEBE/muh_defteri_kebir_raporu_open.h \
    MUHASEBE/muh_defteri_kebir_raporu_class.h \
    MUHASEBE/muh_bilanco_raporu_open.h \
    MUHASEBE/muh_bilanco_raporu_class.h \
    MUHASEBE/muh_gelir_tablosu_raporu_open.h \
    MUHASEBE/muh_toplu_fis_raporu_open.h \
    MUHASEBE/muh_toplu_fis_raporu_class.h \
    MUHASEBE/muh_gelir_tablosu_raporu_class.h \
    MUHASEBE/muh_enum.h \
    MUHASEBE/muh_open.h \
    MUHASEBE/muh_struct.h   \
    MUHASEBE/muh_hesap_ekleme_fisi_class.h \
    MUHASEBE/muh_hesap_ekleme_fisi_open.h

FORMS += \
    MUHASEBE/muh_muhasebe_fisi.ui \
    MUHASEBE/muh_kdv_ayirma_batch.ui \
    MUHASEBE/muh_mizan_raporu_batch.ui \
    MUHASEBE/muh_hesap_ekstresi_batch.ui \
    MUHASEBE/muh_fis_kopyalama_batch.ui \
    MUHASEBE/muh_hesap_plani_raporu_batch.ui \
    MUHASEBE/muh_toplu_fis_raporu_batch.ui \
    MUHASEBE/muh_yevmiye_no_olustur_batch.ui \
    MUHASEBE/muh_yevmiye_dokumu_batch.ui \
    MUHASEBE/muh_defteri_kebir_batch.ui \
    MUHASEBE/muh_bilanco_raporu_batch.ui \
    MUHASEBE/muh_gelir_tablosu_batch.ui \
    MUHASEBE/muh_fis_arama.ui \
    MUHASEBE/muh_hesap_arama.ui \
    MUHASEBE/muh_hesap_plani.ui \
    MUHASEBE/muh_yansitma_hesaplari_fisi.ui \
    MUHASEBE/muh_yansitma_islemi_batch.ui \
    MUHASEBE/muh_hesap_plani_kopyala_batch.ui   \
    MUHASEBE/muh_hesap_ekleme_fisi.ui

SOURCES +=  \
    MUHASEBE/muh_muhasebe_fisi.cpp \
    MUHASEBE/muh_kdv_ayirma_batch.cpp \
    MUHASEBE/muh_hatali_fis_secimi.cpp \
    MUHASEBE/muh_toplu_fis_raporu.cpp \
    MUHASEBE/muh_mizan_raporu_batch.cpp \
    MUHASEBE/muh_hesap_ekstresi_batch.cpp \
    MUHASEBE/muh_fis_kopyalama_batch.cpp \
    MUHASEBE/muh_hesap_plani_raporu_batch.cpp \
    MUHASEBE/muh_toplu_fis_raporu_batch.cpp \
    MUHASEBE/muh_yevmiye_no_olustur_batch.cpp \
    MUHASEBE/muh_yevmiye_dokumu_batch.cpp \
    MUHASEBE/muh_hesap_ekstresi_raporu.cpp \
    MUHASEBE/muh_hesap_plani_raporu.cpp \
    MUHASEBE/muh_yevmiye_dokumu_raporu.cpp \
    MUHASEBE/muh_mizan_raporu.cpp \
    MUHASEBE/muh_defteri_kebir_batch.cpp \
    MUHASEBE/muh_defteri_kebir_raporu.cpp \
    MUHASEBE/muh_bilanco_raporu_batch.cpp \
    MUHASEBE/muh_bilanco_raporu.cpp \
    MUHASEBE/muh_gelir_tablosu_batch.cpp \
    MUHASEBE/muh_gelir_tablosu_raporu.cpp \
    MUHASEBE/muh_fis_arama.cpp \
    MUHASEBE/muh_hesap_arama.cpp \
    MUHASEBE/muh_hesap_plani.cpp \
    MUHASEBE/muh_console_utils.cpp \
    MUHASEBE/muh_gui_utils.cpp \
    MUHASEBE/muh_yansitma_hesaplari_fisi.cpp \
    MUHASEBE/muh_yansitma_islemi_batch.cpp \
    MUHASEBE/muh_fis_utils.cpp \
    MUHASEBE/muh_hesap_plani_kopyala_batch.cpp \
    MUHASEBE/muh_open.cpp \
    MUHASEBE/muh_hesap_ekleme_fisi.cpp

# ###################################         PERSONEL                     #####################

SOURCES += \
    PERSONEL/prs_personel_formu.cpp \
    PERSONEL/prs_con_utils.cpp \
    PERSONEL/prs_gui_utils.cpp \
    PERSONEL/prs_maas_bordrosu_batch.cpp \
    PERSONEL/prs_maas_bordrosu_raporu_batch.cpp \
    PERSONEL/prs_maas_bordrosu_iptal_batch.cpp \
    PERSONEL/prs_maas_bordrosu_raporu.cpp \
    PERSONEL/prs_personel_aile_uyeleri.cpp \
    PERSONEL/prs_personel_odenek_ekleme_fisi.cpp \
    PERSONEL/prs_personel_kesinti_ekleme_fisi.cpp \
    PERSONEL/prs_sabit_degerler_formu.cpp \
    PERSONEL/prs_ihbar_ve_kidem_tazminati_batch.cpp \
    PERSONEL/prs_isten_ayrilma_batch.cpp \
    PERSONEL/prs_isten_ayrilma_raporu.cpp \
    PERSONEL/prs_isten_ayrilanlar_arama.cpp \
    PERSONEL/prs_ibraname_maddeleri_batch.cpp \
    PERSONEL/prs_personel_raporu_batch.cpp \
    PERSONEL/prs_personel_raporu.cpp \
    PERSONEL/prs_bordro_inceleme_tree_secimi.cpp\
    PERSONEL/prs_ebildirge_olustur_batch.cpp\
    PERSONEL/prs_ek_odenek_ve_kesinti_fisi.cpp\
    PERSONEL/prs_eksik_gun_nedenleri_batch.cpp\
    PERSONEL/prs_ek_odenek_kesinti_arama.cpp\
    PERSONEL/prs_ucret_pusulasi_batch.cpp\
    PERSONEL/prs_ucret_pusulasi_raporu.cpp\
    PERSONEL/prs_puantajlari_gor_ve_duzelt_fisi.cpp \
    PERSONEL/prs_open.cpp \
    PERSONEL/prs_karti_import.cpp \
    PERSONEL/prs_karti_export.cpp

HEADERS += \
    PERSONEL/prs_con_utils.h \
    PERSONEL/prs_gui_utils.h \
    PERSONEL/prs_ebildirge_olustur_batch_class.h \
    PERSONEL/prs_ebildirge_olustur_batch_open.h \
    PERSONEL/prs_ek_odenek_kesinti_arama_class.h \
    PERSONEL/prs_ek_odenek_kesinti_arama_open.h \
    PERSONEL/prs_ek_odenek_ve_kesinti_fisi_class.h \
    PERSONEL/prs_eksik_gun_nedenleri_batch_class.h \
    PERSONEL/prs_eksik_gun_nedenleri_batch_open.h \
    PERSONEL/prs_puantajlari_gor_ve_duzelt_fisi_class.h \
    PERSONEL/prs_puantajlari_gor_ve_duzelt_fisi_open.h \
    PERSONEL/prs_sabit_degerler_formu_class.h \
    PERSONEL/prs_sabit_degerler_formu_open.h \
    PERSONEL/prs_ucret_pusulasi_batch_class.h \
    PERSONEL/prs_ucret_pusulasi_batch_open.h \
    PERSONEL/prs_open.h \
    PERSONEL/prs_bordro_inceleme_tree_secimi_class.h \
    PERSONEL/prs_ibraname_maddeleri_batch_class.h \
    PERSONEL/prs_ibraname_maddeleri_batch_open.h \
    PERSONEL/prs_ek_odenek_ve_kesinti_fisi_open.h \
    PERSONEL/prs_bordro_inceleme_tree_secimi_open.h \
    PERSONEL/prs_ihbar_ve_kidem_tazminati_batch_class.h \
    PERSONEL/prs_ihbar_ve_kidem_tazminati_batch_open.h \
    PERSONEL/prs_isten_ayrilanlar_arama_class.h \
    PERSONEL/prs_isten_ayrilanlar_arama_open.h \
    PERSONEL/prs_isten_ayrilma_batch_class.h \
    PERSONEL/prs_isten_ayrilma_batch_open.h \
    PERSONEL/prs_isten_ayrilma_raporu_class.h \
    PERSONEL/prs_isten_ayrilma_raporu_open.h \
    PERSONEL/prs_maas_bordrosu_batch_class.h \
    PERSONEL/prs_maas_bordrosu_batch_open.h \
    PERSONEL/prs_maas_bordrosu_iptal_batch_class.h \
    PERSONEL/prs_maas_bordrosu_iptal_batch_open.h \
    PERSONEL/prs_maas_bordrosu_raporu_class.h \
    PERSONEL/prs_maas_bordrosu_raporu_open.h \
    PERSONEL/prs_maas_bordrosu_raporu_batch_class.h \
    PERSONEL/prs_maas_bordrosu_raporu_batch_open.h \
    PERSONEL/prs_personel_aile_uyeleri_class.h \
    PERSONEL/prs_personel_aile_uyeleri_open.h \
    PERSONEL/prs_personel_formu_class.h \
    PERSONEL/prs_personel_formu_open.h \
    PERSONEL/prs_personel_kesinti_ekleme_fisi_class.h \
    PERSONEL/prs_personel_kesinti_ekleme_fisi_open.h \
    PERSONEL/prs_personel_odenek_ekleme_fisi_class.h \
    PERSONEL/prs_personel_odenek_ekleme_fisi_open.h \
    PERSONEL/prs_personel_raporu_class.h \
    PERSONEL/prs_personel_raporu_open.h \
    PERSONEL/prs_personel_raporu_batch_class.h \
    PERSONEL/prs_personel_raporu_batch_open.h \
    PERSONEL/prs_ucret_pusulasi_raporu_class.h \
    PERSONEL/prs_ucret_pusulasi_raporu_open.h\
    PERSONEL/prs_enum.h \
    PERSONEL/prs_struct.h \
    PERSONEL/prs_karti_import_open.h \
    PERSONEL/prs_karti_import_class.h \
    PERSONEL/prs_karti_export_open.h \
    PERSONEL/prs_karti_export_class.h


FORMS += PERSONEL/prs_personel_formu.ui \
    PERSONEL/prs_maas_bordrosu_batch.ui \
    PERSONEL/prs_maas_bordrosu_raporu_batch.ui \
    PERSONEL/prs_maas_bordrosu_iptal_batch.ui \
    PERSONEL/prs_personel_aile_fisi.ui \
    PERSONEL/prs_personel_odenek_ekleme_fisi.ui \
    PERSONEL/prs_personel_kesinti_ekleme_fisi.ui \
    PERSONEL/prs_sabit_degerler_formu.ui \
    PERSONEL/prs_ihbar_ve_kidem_tazminati_batch.ui \
    PERSONEL/prs_isten_ayrilma_batch.ui \
    PERSONEL/prs_isten_ayrilanlar_arama.ui \
    PERSONEL/prs_ibraname_maddeleri_batch.ui \
    PERSONEL/prs_personel_raporu_batch.ui   \
    PERSONEL/prs_ebildirge_olustur_batch.ui   \
    PERSONEL/prs_ek_odenek_ve_kesinti_fisi.ui   \
    PERSONEL/prs_eksik_gun_nedenleri_batch.ui   \
    PERSONEL/prs_ek_odenek_kesinti_arama.ui   \
    PERSONEL/prs_ucret_pusulasi_batch.ui   \
    PERSONEL/prs_puantajlari_gor_ve_duzelt_fisi.ui

# ###################################         STOK                         #####################
HEADERS += \
    STOK/stok_console_utils.h \
    STOK/stok_gui_utils.h \
    STOK/stok_fis_utils.h \
    STOK/stok_ekstresi_batch_class.h \
    STOK/stok_detaylar_batch_class.h \
    STOK/stok_ekstresi_raporu_class.h \
    STOK/stok_fisi_class.h \
    STOK/stok_fisi_arama_class.h \
    STOK/stok_hizmet_karti_formu_class.h \
    STOK/stok_miktarlari_batch_class.h \
    STOK/stok_miktarlari_raporu_class.h \
    STOK/stok_urun_birimleri_batch_class.h \
    STOK/stok_urun_karti_formu_class.h \
    STOK/stok_urun_listesi_batch_class.h \
    STOK/stok_urun_listesi_raporu_class.h \
    STOK/stok_urun_seri_no_arama_class.h \
    STOK/stok_detaylar_batch_open.h \
    STOK/stok_ekstresi_batch_open.h \
    STOK/stok_ekstresi_raporu_open.h \
    STOK/stok_fisi_arama_open.h \
    STOK/stok_hizmet_karti_formu_open.h \
    STOK/stok_miktarlari_batch_open.h \
    STOK/stok_miktarlari_raporu_open.h \
    STOK/stok_urun_arama_class.h \
    STOK/stok_urun_arama_open.h \
    STOK/stok_urun_birimleri_batch_open.h \
    STOK/stok_urun_karti_formu_open.h \
    STOK/stok_urun_listesi_batch_open.h \
    STOK/stok_urun_seri_no_arama_open.h \
    STOK/stok_open.h \
    STOK/stok_urun_listesi_raporu_open.h \
    STOK/stok_fisi_open.h   \
    STOK/stok_enum.h \
    STOK/stok_struct.h  \
    STOK/stok_urun_karti_import_class.h \
    STOK/stok_urun_karti_import_open.h\
    STOK/stok_urun_parti_girisi_batch_class.h \
    STOK/stok_urun_parti_girisi_batch_open.h    \
    STOK/stok_gelen_seri_no_batch_class.h \
    STOK/stok_gelen_seri_no_batch_open.h    \
    STOK/stok_giden_seri_no_secimi_open.h    \
    STOK/stok_giden_seri_no_secimi_class.h    \
    STOK/stok_giden_parti_no_secimi_open.h    \
    STOK/stok_giden_parti_no_secimi_class.h \
    STOK/stok_urun_hareketleri_arama_class.h \
    STOK/stok_urun_hareketleri_arama_open.h \
    STOK/stok_urun_karti_export_class.h \
    STOK/stok_urun_karti_export_open.h

SOURCES += \
    STOK/stok_console_utils.cpp \
    STOK/stok_gui_utils.cpp \
    STOK/stok_urun_karti_formu.cpp \
    STOK/stok_hizmet_karti_formu.cpp \
    STOK/stok_urun_birimleri_batch.cpp \
    STOK/stok_fisi.cpp \
    STOK/stok_urun_arama.cpp \
    STOK/stok_fisi_arama.cpp \
    STOK/stok_urun_seri_no_arama.cpp \
    STOK/stok_urun_listesi_batch.cpp \
    STOK/stok_urun_listesi_raporu.cpp \
    STOK/stok_miktarlari_batch.cpp \
    STOK/stok_miktarlari_raporu.cpp \
    STOK/stok_ekstresi_batch.cpp \
    STOK/stok_ekstresi_raporu.cpp \
    STOK/stok_detaylar_batch.cpp    \
    STOK/stok_fis_utils.cpp \
    STOK/stok_open.cpp  \
    STOK/stok_urun_karti_import.cpp \
    STOK/stok_urun_parti_girisi_batch.cpp   \
    STOK/stok_gelen_seri_no_batch.cpp   \
    STOK/stok_giden_seri_no_secimi.cpp  \
    STOK/stok_giden_parti_no_secimi.cpp \
    STOK/stok_urun_hareketleri_arama.cpp \
    STOK/stok_urun_karti_export.cpp

FORMS += \
    STOK/stok_urun_karti_formu.ui \
    STOK/stok_hizmet_karti_formu.ui \
    STOK/stok_urun_birimleri_batch.ui \
    STOK/stok_fisi.ui \
    STOK/stok_urun_arama.ui \
    STOK/stok_fisi_arama.ui \
    STOK/stok_urun_seri_no_arama.ui \
    STOK/stok_gelen_seri_no_fisi.ui \
    STOK/stok_giden_seri_no_fisi.ui \
    STOK/stok_urun_listesi_batch.ui \
    STOK/stok_miktarlari_batch.ui \
    STOK/stok_ekstresi_batch.ui \
    STOK/stok_detaylar_batch.ui \
    STOK/stok_urun_parti_kodu_arama.ui  \
    STOK/stok_urun_parti_girisi_batch.ui \
    STOK/stok_gelen_seri_no_batch.ui    \
    STOK/stok_urun_hareketleri_arama.ui


# ###################################         IRSALIYE                         #####################
HEADERS += IRSALIYE/irs_console_utils.h \
    IRSALIYE/irs_gui_utils.h \
    IRSALIYE/irs_fatura_satir_secimi_open.h \
    IRSALIYE/irs_fatura_satir_secimi_class.h \
    IRSALIYE/irs_fatura_secimi_open.h \
    IRSALIYE/irs_fatura_secimi_class.h \
    IRSALIYE/irs_iade_irsaliye_secimi_open.h \
    IRSALIYE/irs_iade_irsaliye_secimi_class.h \
    IRSALIYE/irs_iade_satir_secimi_open.h \
    IRSALIYE/irs_iade_satir_secimi_class.h \
    IRSALIYE/irs_irsaliye_listesi_batch_open.h \
    IRSALIYE/irs_irsaliye_listesi_batch_class.h \
    IRSALIYE/irs_irsaliye_listesi_raporu_open.h \
    IRSALIYE/irs_irsaliye_listesi_raporu_class.h \
    IRSALIYE/irs_irsaliye_belgesi_open.h \
    IRSALIYE/irs_irsaliye_belgesi_class.h \
    IRSALIYE/irs_enum.h \
    IRSALIYE/irs_struct.h \
    IRSALIYE/irs_open.h


SOURCES += \
    IRSALIYE/irs_console_utils.cpp \
    IRSALIYE/irs_gui_utils.cpp \
    IRSALIYE/irs_fatura_satir_secimi.cpp \
    IRSALIYE/irs_fatura_secimi.cpp \
    IRSALIYE/irs_iade_irsaliye_secimi.cpp \
    IRSALIYE/irs_iade_satir_secimi.cpp \
    IRSALIYE/irs_irsaliye_listesi_batch.cpp \
    IRSALIYE/irs_irsaliye_listesi_raporu.cpp \
    IRSALIYE/irs_irsaliye_belgesi.cpp \
    IRSALIYE/irs_open.cpp
FORMS += \
         IRSALIYE/irs_irsaliye_listesi_batch.ui

# ######################################## SUBE - DEPO - KASA / UNITE      ######################################
HEADERS += \
    SUBE_DEPO_UNITE/sube_console_utils.h \
    SUBE_DEPO_UNITE/sube_gui_utils.h \
    SUBE_DEPO_UNITE/sube_arama_class.h \
    SUBE_DEPO_UNITE/sube_depo_secimi_class.h \
    SUBE_DEPO_UNITE/sube_depolari_formu_class.h \
    SUBE_DEPO_UNITE/sube_entegrasyon_hesaplari_formu_class.h \
    SUBE_DEPO_UNITE/sube_fisi_class.h \
    SUBE_DEPO_UNITE/sube_secimi_class.h \
    SUBE_DEPO_UNITE/sube_unite_detaylari_formu_class.h \
    SUBE_DEPO_UNITE/sube_arama_open.h \
    SUBE_DEPO_UNITE/sube_depo_secimi_open.h \
    SUBE_DEPO_UNITE/sube_depolari_formu_open.h \
    SUBE_DEPO_UNITE/sube_entegrasyon_hesaplari_formu_open.h \
    SUBE_DEPO_UNITE/sube_fisi_open.h \
    SUBE_DEPO_UNITE/sube_secimi_open.h \
    SUBE_DEPO_UNITE/sube_unite_detaylari_formu_open.h \
    SUBE_DEPO_UNITE/sube_depo_unite_open.h  \
    SUBE_DEPO_UNITE/sube_enum.h \
    SUBE_DEPO_UNITE/sube_struct.h \
    SUBE_DEPO_UNITE/sube_tree_batch_class.h \
    SUBE_DEPO_UNITE/sube_tree_batch_open.h \
    SUBE_DEPO_UNITE/sube_open.h

SOURCES += SUBE_DEPO_UNITE/sube_fisi.cpp \
    SUBE_DEPO_UNITE/sube_depolari_formu.cpp \
    SUBE_DEPO_UNITE/sube_unite_detaylari_formu.cpp \
    SUBE_DEPO_UNITE/sube_arama.cpp \
    SUBE_DEPO_UNITE/sube_entegrasyon_hesaplari_formu.cpp \
    SUBE_DEPO_UNITE/sube_gui_utils.cpp \
    SUBE_DEPO_UNITE/sube_console_utils.cpp \
    SUBE_DEPO_UNITE/sube_secimi.cpp \
    SUBE_DEPO_UNITE/sube_depo_secimi.cpp \
    SUBE_DEPO_UNITE/sube_tree_batch.cpp \
    SUBE_DEPO_UNITE/sube_open.cpp
FORMS += SUBE_DEPO_UNITE/sube_fisi.ui \
    SUBE_DEPO_UNITE/sube_depolari_formu.ui \
    SUBE_DEPO_UNITE/sube_unite_detaylari_formu.ui \
    SUBE_DEPO_UNITE/sube_arama.ui \
    SUBE_DEPO_UNITE/sube_tree_batch.ui \
    SUBE_DEPO_UNITE/sube_entegrasyon_hesaplari_formu.ui

# ######################################       FATURA        ############################################
HEADERS += \
    FATURA/fat_console_utils.h \
    FATURA/fat_gui_utils.h \
    FATURA/fat_ba_bs_formu_listesi_batch_class.h \
    FATURA/fat_ba_bs_formu_listesi_raporu_class.h \
    FATURA/fat_fatura_arama_class.h \
    FATURA/fat_fatura_belgesi_class.h \
    FATURA/fat_fatura_detaylari_batch_class.h \
    FATURA/fat_fatura_fisi_class.h \
    FATURA/fat_fatura_listesi_batch_class.h \
    FATURA/fat_fatura_listesi_raporu_class.h \
    FATURA/fat_hizmet_ekstresi_batch_class.h \
    FATURA/fat_hizmet_ekstresi_raporu_class.h \
    FATURA/fat_iade_fatura_secimi_class.h \
    FATURA/fat_iade_satir_secimi_class.h \
    FATURA/fat_irsaliye_satir_secimi_class.h \
    FATURA/fat_ba_bs_formu_listesi_batch_open.h \
    FATURA/fat_ba_bs_formu_listesi_raporu_open.h \
    FATURA/fat_fatura_arama_open.h \
    FATURA/fat_fatura_belgesi_open.h \
    FATURA/fat_fatura_detaylari_batch_open.h \
    FATURA/fat_fatura_fisi_open.h \
    FATURA/fat_fatura_listesi_batch_open.h \
    FATURA/fat_fatura_listesi_raporu_open.h \
    FATURA/fat_hizmet_ekstresi_batch_open.h \
    FATURA/fat_hizmet_ekstresi_raporu_open.h \
    FATURA/fat_iade_fatura_secimi_open.h \
    FATURA/fat_iade_satir_secimi_open.h \
    FATURA/fat_irsaliye_satir_secimi_open.h \
    FATURA/fat_enum.h \
    FATURA/fat_fis_utils.h \
    FATURA/fat_open.h \
    FATURA/fat_struct.h \
    FATURA/fat_toplu_faturalastir_arama_class.h \
    FATURA/fat_toplu_faturalastir_arama_open.h  \
    FATURA/fat_irsaliye_arama_class.h \
    FATURA/fat_irsaliye_arama_open.h

SOURCES += \
    FATURA/fat_fatura_fisi.cpp \
    FATURA/fat_fatura_arama.cpp \
    FATURA/fat_irsaliye_satir_secimi.cpp \
    FATURA/fat_console_utils.cpp \
    FATURA/fat_gui_utils.cpp \
    FATURA/fat_iade_fatura_secimi.cpp \
    FATURA/fat_iade_satir_secimi.cpp \
    FATURA/fat_fatura_detaylari_batch.cpp \
    FATURA/fat_fatura_belgesi.cpp \
    FATURA/fat_fatura_listesi_batch.cpp \
    FATURA/fat_fatura_listesi_raporu.cpp    \
    FATURA/fat_ba_bs_formu_listesi_raporu.cpp    \
    FATURA/fat_ba_bs_formu_listesi_batch.cpp \
    FATURA/fat_hizmet_ekstresi_batch.cpp \
    FATURA/fat_hizmet_ekstresi_raporu.cpp \
    FATURA/fat_open.cpp \
    FATURA/fat_fis_utils.cpp    \
    FATURA/fat_toplu_faturalastir_arama.cpp \
    FATURA/fat_irsaliye_arama.cpp

FORMS += \
    FATURA/fat_fatura_fisi.ui \
    FATURA/fat_toplu_faturalastir_arama.ui \
    FATURA/fat_fatura_arama.ui \
    FATURA/fat_fatura_detaylari_batch.ui    \
    FATURA/fat_fatura_listesi_batch.ui  \
    FATURA/fat_ba_bs_formu_listesi_batch.ui \
    FATURA/fat_hizmet_ekstresi_batch.ui \
    FATURA/fat_irsaliye_arama.ui

# ########################################    PERAKENDE     ##################################################
HEADERS += \
           PERAKENDE/prk_console_utils.h \
           PERAKENDE/prk_fis_utils.h \
           PERAKENDE/prk_fis_arama_class.h \
           PERAKENDE/prk_fis_arama_open.h \
           PERAKENDE/prk_gelir_gider_turleri_fisi_open.h \
           PERAKENDE/prk_gelir_gider_turleri_fisi_class.h \
           PERAKENDE/prk_perakende_fisi_class.h \
           PERAKENDE/prk_perakende_fisi_open.h  \
           PERAKENDE/prk_fisi_yazdir_raporu_class.h \
           PERAKENDE/prk_fisi_yazdir_raporu_open.h \
           PERAKENDE/prk_enum.h \
           PERAKENDE/prk_open.h \
           PERAKENDE/prk_struct.h

SOURCES += PERAKENDE/prk_perakende_fisi.cpp \
           PERAKENDE/prk_console_utils.cpp \
           PERAKENDE/prk_fis_arama.cpp \
           PERAKENDE/prk_gelir_gider_turleri_fisi.cpp \
           PERAKENDE/prk_fisi_yazdir_raporu.cpp \
           PERAKENDE/prk_fis_utils.cpp \
           PERAKENDE/prk_open.cpp


FORMS   += PERAKENDE/prk_perakende_fisi.ui \
           PERAKENDE/prk_fis_arama.ui \
           PERAKENDE/prk_gelir_gider_turleri_fisi.ui

# ########################################    SM_MAKBUZ   ##################################################
HEADERS +=  \
            SMM_MAKBUZ/smm_console_utils.h \
            SMM_MAKBUZ/smm_makbuz_utils.h \
            SMM_MAKBUZ/smm_makbuz_exe.h \
            SMM_MAKBUZ/smm_ent_detaylari_batch_class.h \
            SMM_MAKBUZ/smm_hizmet_tanimlama_fisi_class.h \
            SMM_MAKBUZ/smm_makbuz_arama_class.h \
            SMM_MAKBUZ/smm_makbuz_belgesi_class.h \
            SMM_MAKBUZ/smm_makbuz_formu_class.h \
            SMM_MAKBUZ/smm_ent_detaylari_batch_open.h \
            SMM_MAKBUZ/smm_hizmet_tanimlama_fisi_open.h \
            SMM_MAKBUZ/smm_makbuz_arama_open.h \
            SMM_MAKBUZ/smm_makbuz_belgesi_open.h \
            SMM_MAKBUZ/smm_makbuz_formu_open.h \
            SMM_MAKBUZ/smm_open.h \
            SMM_MAKBUZ/smm_enum.h \
            SMM_MAKBUZ/smm_struct.h
SOURCES += \
           SMM_MAKBUZ/smm_console_utils.cpp \
           SMM_MAKBUZ/smm_hizmet_tanimlama_fisi.cpp \
           SMM_MAKBUZ/smm_ent_detaylari_batch.cpp \
           SMM_MAKBUZ/smm_makbuz_utils.cpp \
           SMM_MAKBUZ/smm_makbuz_exe.cpp \
           SMM_MAKBUZ/smm_makbuz_belgesi.cpp \
           SMM_MAKBUZ/smm_makbuz_arama.cpp \
           SMM_MAKBUZ/smm_makbuz_formu.cpp \
           SMM_MAKBUZ/smm_open.cpp

FORMS   +=  \
           SMM_MAKBUZ/smm_hizmet_tanimlama_fisi.ui \
           SMM_MAKBUZ/smm_ent_detaylari_batch.ui \
           SMM_MAKBUZ/smm_makbuz_arama.ui \
           SMM_MAKBUZ/smm_makbuz_formu.ui
