#ifndef E9_DB_H
#define E9_DB_H

ADAK_SQL_STRUCT e9_db = {
    2,
    82,
    {
        {
            "adr_adresler",
            56,
            {
                {"adres_id", INTEGER, 0, false, 1, true, true },
                {"firma_sahis_adi", TEXT, 60, false, 1, false, false },
                {"dogum_tarihi", TEXT, 10, false, 1, false, false },
                {"dogum_yeri", TEXT, 30, false, 1, false, false },
                {"cinsiyet", TEXT, 5, false, 1, false, false },
                {"ogrenim_durumu", TEXT, 20, false, 1, false, false },
                {"meslek", TEXT, 128, false, 1, false, false },
                {"unvan", TEXT, 128, false, 1, false, false },
                {"kimlik_no", TEXT, 15, false, 1, false, false },
                {"kan_grubu", TEXT, 10, false, 1, false, false },
                {"medeni_hali", TEXT, 10, false, 1, false, false },
                {"vergi_dairesi", TEXT, 50, false, 1, false, false },
                {"vergi_no", TEXT, 15, false, 1, false, false },
                {"adres_satiri_1", TEXT, 35, false, 1, false, false },
                {"adres_satiri_2", TEXT, 35, false, 1, false, false },
                {"adres_satiri_3", TEXT, 35, false, 1, false, false },
                {"ilce", TEXT, 30, false, 1, false, false },
                {"semt", TEXT, 30, false, 1, false, false },
                {"ulke", TEXT, 30, false, 1, false, false },
                {"sehir", TEXT, 30, false, 1, false, false },
                {"posta_kodu", TEXT, 10, false, 1, false, false },
                {"tel_ulke_kodu", TEXT, 10, false, 1, false, false },
                {"tel_alan_kodu", TEXT, 10, false, 1, false, false },
                {"telefon", TEXT, 10, false, 1, false, false },
                {"is_tel_ulke_kodu", TEXT, 10, false, 1, false, false },
                {"is_tel_alan_kodu", TEXT, 10, false, 1, false, false },
                {"is_telefonu", TEXT, 10, false, 1, false, false },
                {"cep_tel_ulke_kodu", TEXT, 10, false, 1, false, false },
                {"cep_tel_gsm_kodu", TEXT, 10, false, 1, false, false },
                {"cep_telefonu", TEXT, 10, false, 1, false, false },
                {"fax_ulke_kodu", TEXT, 10, false, 1, false, false },
                {"fax_alan_kodu", TEXT, 10, false, 1, false, false },
                {"fax", TEXT, 10, false, 1, false, false },
                {"email", TEXT, 50, false, 1, false, false },
                {"website", TEXT, 50, false, 1, false, false },
                {"firma_sektoru", TEXT, 30, false, 1, false, false },
                {"fotograf_logo", BLOB, 0, false, 1, false, false },
                {"firma_id", INTEGER, 0, false, 1, false, false },
                {"is_telefonu_dahili", TEXT, 5, false, 1, false, false },
                {"kaydin_olusturulma_tarihi", TEXT, 10, false, 1, false, false },
                {"telefon_gizli_kayit_id", INTEGER, 0, false, 1, false, false },
                {"is_tel_gizli_kayit_id", INTEGER, 0, false, 1, false, false },
                {"cep_tel_gizli_kayit_id", INTEGER, 0, false, 1, false, false },
                {"fax_gizli_kayit_id", INTEGER, 0, false, 1, false, false },
                {"email_gizli_kayit_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"filtre_str", TEXT, 15, false, 1, false, false },
                {"adres_kart_turu", INTEGER, 0, false, 1, false, false },
                {"yetkili_1", TEXT, 60, false, 1, false, false },
                {"yetkili_1_info", TEXT, 128, false, 1, false, false },
                {"yetkili_2", TEXT, 60, false, 1, false, false },
                {"yetkili_2_info", TEXT, 128, false, 1, false, false },
                {"yetkili_3", TEXT, 60, false, 1, false, false },
                {"yetkili_3_info", TEXT, 128, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"adres_id"} }
            }
        },
        {
            "adr_emailler",
            6,
            {
                {"email_id", INTEGER, 0, false, 1, true, true },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"adres_kayit_id", INTEGER, 0, false, 1, false, false },
                {"email_adresi", TEXT, 50, false, 1, false, false },
                {"gizli_kayit", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 250, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"email_id"} }
            }
        },
        {
            "adr_not_defteri",
            8,
            {
                {"not_id", INTEGER, 0, false, 1, true, true },
                {"tarih", TEXT, 10, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"ekleyen_kisi_id", INTEGER, 0, false, 1, false, false },
                {"adres_kayit_id", INTEGER, 0, false, 1, false, false },
                {"not_bilgisi", TEXT, 128, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"not_id"} }
            }
        },
        {
            "adr_telefonlar",
            9,
            {
                {"telefon_id", INTEGER, 0, false, 1, true, true },
                {"adres_kayit_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"telefon_ulke_kodu", TEXT, 10, false, 1, false, false },
                {"telefon_alan_kodu", TEXT, 10, false, 1, false, false },
                {"telefon", TEXT, 10, false, 1, false, false },
                {"telefon_dahili", TEXT, 5, false, 1, false, false },
                {"gizli_kayit", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 250, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"telefon_id"} }
            }
        },
        {
            "adr_yetkili_bilgileri",
            5,
            {
                {"yetkili_id", INTEGER, 0, false, 1, true, true },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"firma_id", INTEGER, 0, false, 1, false, false },
                {"adres_id", INTEGER, 0, false, 1, false, false },
                {"gorevi", TEXT, 30, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"yetkili_id"} }
            }
        },
        {
            "bnk_defter_fisler",
            17,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 10, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"para_birim_id", INTEGER, 0, false, 1, false, false },
                {"fis_turu", INTEGER, 0, false, 1, false, false },
                {"toplam_hesaba_giren", REAL, 0, false, 1, false, false },
                {"toplam_hesaptan_cikan", REAL, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_turu", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"odm_emr_hesabi_etkilenecek_mi", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "bnk_defter_fis_satirlari",
            13,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"hesaba_giren", REAL, 0, false, 1, false, false },
                {"hesaptan_cikan", REAL, 0, false, 1, false, false },
                {"hesap_turu", INTEGER, 0, false, 1, false, false },
                {"hesap_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "bnk_hareket_fisler",
            19,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 30, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"toplam_hesaba_giren", REAL, 0, false, 1, false, false },
                {"toplam_hesaptan_cikan", REAL, 0, false, 1, false, false },
                {"fis_turu", INTEGER, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false },
                {"para_birim_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_turu", INTEGER, 0, false, 1, false, false },
                {"def_fis_id", INTEGER, 0, false, 1, false, false },
                {"odm_emr_hesabi_etkilenecek_mi", INTEGER, 0, false, 1, false, false },
                {"muh_kasa_hesap_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "bnk_hareket_fis_satirlari",
            17,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"hesaba_giren", REAL, 0, false, 1, false, false },
                {"hesaptan_cikan", REAL, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"etkilenecek_hesap_turu", INTEGER, 0, false, 1, false, false },
                {"etkilenecek_hesap_id", INTEGER, 0, false, 1, false, false },
                {"karsi_hesap_turu", INTEGER, 0, false, 1, false, false },
                {"karsi_hesap_id", INTEGER, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false },
                {"parite", REAL, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"karsi_hesap_eklenecek_mi", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "bnk_hesabin_kredi_kartlari",
            7,
            {
                {"kredi_karti_id", INTEGER, 0, false, 1, true, true },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"kredi_karti_numarasi", TEXT, 16, false, 1, false, false },
                {"kredi_karti_sahibi", TEXT, 30, false, 1, false, false },
                {"muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kredi_karti_id"} }
            }
        },
        {
            "bnk_hesabin_poslari",
            9,
            {
                {"pos_id", INTEGER, 0, false, 1, true, true },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"pos_numarasi", TEXT, 20, false, 1, false, false },
                {"uye_isyeri_adresi", TEXT, 50, false, 1, false, false },
                {"uye_isyeri_telefonu", TEXT, 15, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"kom_hesap_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"pos_id"} }
            }
        },
        {
            "bnk_hesaplar",
            14,
            {
                {"hesap_no_id", INTEGER, 0, false, 1, true, true },
                {"banka_hesap_kodu", TEXT, 20, false, 1, false, false },
                {"banka_hesap_unvani", TEXT, 60, false, 1, false, false },
                {"banka_ismi", TEXT, 50, false, 1, false, false },
                {"sube_kodu", TEXT, 20, false, 1, false, false },
                {"sube_ismi", TEXT, 50, false, 1, false, false },
                {"hesap_numarasi", TEXT, 20, false, 1, false, false },
                {"doviz_id", INTEGER, 0, false, 1, false, false },
                {"toplam_borc_array", REAL, 0, true, 501, false, false },
                {"toplam_alacak_array", REAL, 0, true, 501, false, false },
                {"muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"verilen_cekler_hesap_id", INTEGER, 0, false, 1, false, false },
                {"odeme_emirleri_hesap_id", INTEGER, 0, false, 1, false, false },
                {"iban_numarasi", TEXT, 26, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"hesap_no_id"} }
            }
        },
        {
            "bnk_kk_arrayler",
            4,
            {
                {"kayit_id", INTEGER, 0, false, 1, true, true },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"toplam_kk_odeme_borc_array", REAL, 0, true, 501, false, false },
                {"toplam_kk_odeme_alacak_array", REAL, 0, true, 501, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kayit_id"} }
            }
        },
        {
            "bnk_makro_fisler",
            16,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 10, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"toplam_hesaba_giren", REAL, 0, false, 1, false, false },
                {"toplam_hesaptan_cikan", REAL, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"toplam_makro_tutari", REAL, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"def_fis_id", INTEGER, 0, false, 1, false, false },
                {"odm_emr_hesabi_etkilenecek_mi", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "bnk_makro_fis_satirlari",
            18,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"makro_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"makro_tutari", REAL, 0, false, 1, false, false },
                {"otomatik_odeme_tutari", REAL, 0, false, 1, false, false },
                {"temel_tutar", REAL, 0, false, 1, false, false },
                {"hesap_turu", INTEGER, 0, false, 1, false, false },
                {"hesap_id", INTEGER, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false },
                {"parite", REAL, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"satir_tutari", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "bnk_makrolar",
            3,
            {
                {"makro_id", INTEGER, 0, false, 1, true, true },
                {"makro_adi", TEXT, 128, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"makro_id"} }
            }
        },
        {
            "bnk_makro_satirlari",
            11,
            {
                {"makro_satiri_id", INTEGER, 0, false, 1, true, true },
                {"makro_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"fark_satiri_mi", INTEGER, 0, false, 1, false, false },
                {"hesaba_etkisi", INTEGER, 0, false, 1, false, false },
                {"hesap_turu", INTEGER, 0, false, 1, false, false },
                {"muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"sabit_tutar", REAL, 0, false, 1, false, false },
                {"makro_yuzdesi", REAL, 0, false, 1, false, false },
                {"aciklama", TEXT, 250, false, 1, false, false },
                {"round_sekli", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"makro_satiri_id"} }
            }
        },
        {
            "bnk_pos_arrayler",
            4,
            {
                {"kayit_id", INTEGER, 0, false, 1, true, true },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"toplam_kk_tahsilat_borc_array", REAL, 0, true, 501, false, false },
                {"toplam_kk_thslat_alacak_array", REAL, 0, true, 501, false, false },
            },
            1,
            {
                { PRIMARY , 1, {"kayit_id"} }
            }
        },
        {
            "bnk_virman_fisler",
            17,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 10, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"toplam_hesaba_giren", REAL, 0, false, 1, false, false },
                {"toplam_hesaptan_cikan", REAL, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"def_fis_id", INTEGER, 0, false, 1, false, false },
                {"para_birim_id", INTEGER, 0, false, 1, false, false },
                {"odm_emr_hesabi_etkilenecek_mi", INTEGER, 0, false, 1, false, false },
                {"alici_def_fis_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "bnk_virman_fis_satirlari",
            15,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"hesap_turu", INTEGER, 0, false, 1, false, false },
                {"gonderen_hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"alici_hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false },
                {"gonderen_islem_tutari", REAL, 0, false, 1, false, false },
                {"alici_islem_tutari", REAL, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"islem_tutari", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "car_fisler",
            23,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 12, false, 1, false, false },
                {"cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"toplam_borc", REAL, 0, false, 1, false, false },
                {"toplam_alacak", REAL, 0, false, 1, false, false },
                {"belge_seri", TEXT, 3, false, 1, false, false },
                {"fis_turu", INTEGER, 0, false, 1, false, false },
                {"doviz_tutar_bilgileri", TEXT, 512, false, 1, false, false },
                {"makbuz_para_birimi_id", INTEGER, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false },
                {"parite", REAL, 0, false, 1, false, false },
                {"muh_kasa_hesap_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"belge_numarasi", TEXT, 15, false, 1, false, false },
                {"yazdirildi_mi", INTEGER, 0, false, 1, false, false },
                {"iptal_mi", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "car_fis_satirlari",
            28,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"borc_tutari", REAL, 0, false, 1, false, false },
                {"alacak_tutari", REAL, 0, false, 1, false, false },
                {"kredi_karti_numarasi", TEXT, 16, false, 1, false, false },
                {"kredi_karti_sahibi", TEXT, 30, false, 1, false, false },
                {"musteri_banka_ismi", TEXT, 50, false, 1, false, false },
                {"musteri_sube_kodu", TEXT, 20, false, 1, false, false },
                {"musteri_sube_ismi", TEXT, 50, false, 1, false, false },
                {"musteri_hesap_no", TEXT, 20, false, 1, false, false },
                {"cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"cek_senet_id", INTEGER, 0, false, 1, false, false },
                {"islem_turu", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"temel_para_birimi_tutari", REAL, 0, false, 1, false, false },
                {"muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"pos_id", INTEGER, 0, false, 1, false, false },
                {"kredi_karti_id", INTEGER, 0, false, 1, false, false },
                {"cari_para_birimi_tutari", REAL, 0, false, 1, false, false },
                {"doviz_id", INTEGER, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false },
                {"parite", REAL, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "car_hesaplar",
            14,
            {
                {"cari_hesap_id", INTEGER, 0, false, 1, true, true },
                {"cari_hesap_kodu", TEXT, 30, false, 1, false, false },
                {"toplam_borc_array", REAL, 0, true, 501, false, false },
                {"toplam_alacak_array", REAL, 0, true, 501, false, false },
                {"adres_id", INTEGER, 0, false, 1, false, false },
                {"muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"para_birimi_id", INTEGER, 0, false, 1, false, false },
                {"cari_kart_tipi", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"sm_sicil_numarasi", TEXT, 20, false, 1, false, false },
                {"cari_kart_turu", INTEGER, 0, false, 1, false, false },
                {"cari_hesap_ismi", TEXT, 60, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"cari_hesap_id"} }
            }
        },
        {
            "cek_bordrolar",
            21,
            {
                {"bordro_id", INTEGER, 0, false, 1, true, true },
                {"bordro_tarihi", TEXT, 10, false, 1, false, false },
                {"bordro_no", INTEGER, 0, false, 1, false, false },
                {"bordro_islem_turu", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"toplam_tutar", REAL, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"bordro_turu", INTEGER, 0, false, 1, false, false },
                {"doviz_tutar_bilgileri", TEXT, 512, false, 1, false, false },
                {"musteri_cek_senedi_mi", INTEGER, 0, false, 1, false, false },
                {"cek_bordrosu_mu", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false },
                {"bordro_para_birimi_id", INTEGER, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false },
                {"parite", REAL, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"bordro_id"} }
            }
        },
        {
            "cek_bordro_satirlari",
            14,
            {
                {"bordro_satiri_id", INTEGER, 0, false, 1, true, true },
                {"bordro_id", INTEGER, 0, false, 1, false, false },
                {"cek_senet_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"alinan_cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"verilen_cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"cari_para_birimi_tutari", REAL, 0, false, 1, false, false },
                {"temel_para_birimi_tutari", REAL, 0, false, 1, false, false },
                {"satir_tutari", REAL, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"bordro_satiri_id"} }
            }
        },
        {
            "cek_cekler_senetler",
            18,
            {
                {"cek_senet_id", INTEGER, 0, false, 1, true, true },
                {"portfoy_numarasi", INTEGER, 0, false, 1, false, false },
                {"banka_ismi", TEXT, 50, false, 1, false, false },
                {"sube_kodu", TEXT, 20, false, 1, false, false },
                {"sube_ismi", TEXT, 50, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"cek_senet_numarasi", TEXT, 20, false, 1, false, false },
                {"cek_senet_son_durumu", INTEGER, 0, false, 1, false, false },
                {"cek_senet_onceki_durumu", INTEGER, 0, false, 1, false, false },
                {"vade_tarihi", TEXT, 10, false, 1, false, false },
                {"keside_tarihi", TEXT, 10, false, 1, false, false },
                {"borclu_kesideci", TEXT, 50, false, 1, false, false },
                {"musteri_ceki_mi", INTEGER, 0, false, 1, false, false },
                {"cek_senet_tutari", REAL, 0, false, 1, false, false },
                {"musteri_hesap_numarasi", TEXT, 20, false, 1, false, false },
                {"cek_senet_turu", INTEGER, 0, false, 1, false, false },
                {"doviz_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"cek_senet_id"} }
            }
        },
        {
            "cek_hareketleri",
            6,
            {
                {"cek_hareketi_id", INTEGER, 0, false, 1, true, true },
                {"cek_senet_id", INTEGER, 0, false, 1, false, false },
                {"cek_onceki_hareketi", INTEGER, 0, false, 1, false, false },
                {"cek_son_hareketi", INTEGER, 0, false, 1, false, false },
                {"cek_hareketi", INTEGER, 0, false, 1, false, false },
                {"hareket_tarihi", TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"cek_hareketi_id"} }
            }
        },
        {
            "adak_gruplar",
            11,
            {
                {"grup_id", INTEGER, 0, false, 1, true, true },
                {"grup_parent_id", INTEGER, 0, false, 1, false, false },
                {"grup_seviyesi", INTEGER, 0, false, 1, false, false },
                {"grup_kodu", TEXT, 15, false, 1, false, false },
                {"grup_adi", TEXT, 60, false, 1, false, false },
                {"tam_grup_kodu", TEXT, 30, false, 1, false, false },
                {"grup_son_seviye_mi", INTEGER, 0, false, 1, false, false },
                {"gruptaki_kayit_sayisi", INTEGER, 0, false, 1, false, false },
                {"alt_grup_sayisi", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"grup_id"} }
            }
        },
        {
            "adak_kaydin_gruplari",
            5,
            {
                {"id", INTEGER, 0, false, 1, true, true },
                {"record_id", INTEGER, 0, false, 1, false, false },
                {"gruplar", BLOB, 500, false, 1, false, false },
                {"module_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"id"} }
            }
        },
        {
            "dmr_demirbaslar",
            36,
            {
                {"demirbas_id", INTEGER, 0, false, 1, true, true },
                {"demirbas_kodu", TEXT, 20, false, 1, false, false },
                {"demirbas_adi", TEXT, 128, false, 1, false, false },
                {"grup_id", INTEGER, 0, false, 1, false, false },
                {"muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"alis_tarihi", TEXT, 10, false, 1, false, false },
                {"alis_bedeli", REAL, 0, false, 1, false, false },
                {"uretici_firma", TEXT, 128, false, 1, false, false },
                {"marka", TEXT, 128, false, 1, false, false },
                {"model", TEXT, 128, false, 1, false, false },
                {"seri_numarasi", TEXT, 20, false, 1, false, false },
                {"garanti_suresi", INTEGER, 0, false, 1, false, false },
                {"amortisman_yontemi", INTEGER, 0, false, 1, false, false },
                {"amortisman_orani", REAL, 0, false, 1, false, false },
                {"amortisman_suresi", INTEGER, 0, false, 1, false, false },
                {"yeniden_degerleme_yapilacak_mi", INTEGER, 0, false, 1, false, false },
                {"kist_amortisman", INTEGER, 0, false, 1, false, false },
                {"demirbasin_degeri", REAL, 0, false, 1, false, false },
                {"demirbasin_yeni_degeri", REAL, 0, false, 1, false, false },
                {"yeniden_degerleme_yapildi", INTEGER, 0, false, 1, false, false },
                {"yeni_brkms_amortisman_tutari", REAL, 0, false, 1, false, false },
                {"amortisman_ayrilabilir_mi", INTEGER, 0, false, 1, false, false },
                {"amortisman_tutari_array", REAL, 0, true, 501, false, false },
                {"toplamda_ayrilan_amor_yuzdesi", REAL, 0, false, 1, false, false },
                {"yillik_ayrilan_amor_yuzdesi", REAL, 0, false, 1, false, false },
                {"muh_yili_ayrilacak_amor_tutari", REAL, 0, false, 1, false, false },
                {"muh_devir_yili_ayrilacak_amor", REAL, 0, false, 1, false, false },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"fatura_id", INTEGER, 0, false, 1, false, false },
                {"bir_amor_hesap_id", INTEGER, 0, false, 1, false, false },
                {"amor_gider_hesap_id", INTEGER, 0, false, 1, false, false },
                {"sube_id", INTEGER, 0, false, 1, false, false },
                {"demirbas_satildi_mi", INTEGER, 0, false, 1, false, false },
                {"dmr_satis_kar_hesap_id", INTEGER, 0, false, 1, false, false },
                {"amortisman_ayrilacak_mi", INTEGER, 0, false, 1, false, false },
                {"dmr_satis_zarar_hesap_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"demirbas_id"} }
            }
        },
        {
            "dmr_fisler",
            14,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 10, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"fis_turu", INTEGER, 0, false, 1, false, false },
                {"toplam_tutar", REAL, 0, false, 1, false, false },
                {"toplam_birikmis_amor_tutari", REAL, 0, false, 1, false, false },
                {"degerleme_orani", REAL, 0, false, 1, false, false },
                {"dusulecek_amortisman_yuzdesi", REAL, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "dmr_fis_satirlari",
            10,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"demirbas_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"satir_tutari", REAL, 0, false, 1, false, false },
                {"birikmis_amortisman_tutari", REAL, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "dmr_satilan_demirbaslar",
            3,
            {
                {"satis_id", INTEGER, 0, false, 1, true, true },
                {"demirbas_id", INTEGER, 0, false, 1, false, false },
                {"fatura_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"satis_id"} }
            }
        },
        {
            "dvz_degiskenler",
            5,
            {
                {"degisken_id", INTEGER, 0, false, 1, true, true },
                {"son_guncellestirilme_tarihi", TEXT, 10, false, 1, false, false },
                {"kullanilan_xml_adresi", INTEGER, 0, false, 1, false, false },
                {"temel_para_birimi_id", INTEGER, 0, false, 1, false, false },
                {"xml_temel_para_birimi_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"degisken_id"} }
            }
        },
        {
            "dvz_dovizler",
            7,
            {
                {"doviz_id", INTEGER, 0, false, 1, true, true },
                {"doviz_kodu", TEXT, 5, false, 1, false, false },
                {"doviz_adi", TEXT, 64, false, 1, false, false },
                {"doviz_sembolu", TEXT, 5, false, 1, false, false },
                {"alis_eklenecek_yuzde", REAL, 0, false, 1, false, false },
                {"satis_eklenecek_yuzde", REAL, 0, false, 1, false, false },
                {"dovizi_kullan", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"doviz_id"} }
            }
        },
        {
            "dvz_kurlar",
            3,
            {
                {"kur_id", INTEGER, 0, false, 1, true, true },
                {"kur_tarihi", TEXT, 10, false, 1, false, false },
                {"kur_bilgileri", BLOB, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kur_id"} }
            }
        },
        {
            "fat_faturalar",
            65,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fatura_tarihi", TEXT, 10, false, 1, false, false },
                {"fatura_turu", INTEGER, 0, false, 1, false, false },
                {"fatura_alis_satis_turu", INTEGER, 0, false, 1, false, false },
                {"belge_seri", TEXT, 3, false, 1, false, false },
                {"acik_kapali_fatura", INTEGER, 0, false, 1, false, false },
                {"cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"cari_hesap_ismi", TEXT, 60, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"cari_hesap_adresi", TEXT, 512, false, 1, false, false },
                {"vergi_dairesi", TEXT, 50, false, 1, false, false },
                {"vergi_numarasi", TEXT, 15, false, 1, false, false },
                {"islem_saati", TEXT, 5, false, 1, false, false },
                {"isk_oncesi_kdv_tutari", REAL, 0, false, 1, false, false },
                {"isk_oncesi_kdv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"brut_tutar", REAL, 0, false, 1, false, false },
                {"isk_oncesi_otv_tutari", REAL, 0, false, 1, false, false },
                {"isk_oncesi_otv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"yazdirildi_mi", INTEGER, 0, false, 1, false, false },
                {"iptal_mi", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"doviz_tutar_bilgileri", TEXT, 512, false, 1, false, false },
                {"kdv_oran_bilgileri", TEXT, 512, false, 1, false, false },
                {"otv_oran_bilgileri", TEXT, 512, false, 1, false, false },
                {"hatali_fatura_mi", INTEGER, 0, false, 1, false, false },
                {"fatura_irsaliyelestirildi_mi", INTEGER, 0, false, 1, false, false },
                {"yurtici_satislar_hesap_id", INTEGER, 0, false, 1, false, false },
                {"hizmet_urt_maliyeti_hesap_id", INTEGER, 0, false, 1, false, false },
                {"yapilan_indirim_hesap_id", INTEGER, 0, false, 1, false, false },
                {"satistan_iade_hesap_id", INTEGER, 0, false, 1, false, false },
                {"alis_iskontolari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"satis_iskontolari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"iade_faturasi_mi", INTEGER, 0, false, 1, false, false },
                {"iade_edilen_fatura_id", INTEGER, 0, false, 1, false, false },
                {"tevkifatli_fatura_mi", INTEGER, 0, false, 1, false, false },
                {"tevkifat_yuzdesi", REAL, 0, false, 1, false, false },
                {"tevkifatli_kdv_tutari", REAL, 0, false, 1, false, false },
                {"iskonto_yuzdesi", REAL, 0, false, 1, false, false },
                {"iskonto_tutari", REAL, 0, false, 1, false, false },
                {"ara_toplam", REAL, 0, false, 1, false, false },
                {"isk_sonrasi_kdv_tutari", REAL, 0, false, 1, false, false },
                {"fatura_tutari", REAL, 0, false, 1, false, false },
                {"isk_sonrasi_otv_tutari", REAL, 0, false, 1, false, false },
                {"isk_sonrasi_otv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"cari_para_birimi_tutari", REAL, 0, false, 1, false, false },
                {"fatura_para_birimi_id", REAL, 0, false, 1, false, false },
                {"parite", REAL, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false },
                {"kdv_oranlari_array", REAL, 0, true, 10, false, false },
                {"kdv_hesaplari_id_array", INTEGER, 0, true, 10, false, false },
                {"kdv_oran_gore_tutarlar_array", REAL, 0, true, 10, false, false },
                {"otv_oranlari_array", REAL, 0, true, 10, false, false },
                {"otv_hesaplari_id_array", INTEGER, 0, true, 10, false, false },
                {"otv_oran_gore_tutarlar_array", REAL, 0, true, 10, false, false },
                {"kdv_orani_sayisi", INTEGER, 0, false, 1, false, false },
                {"otv_orani_sayisi", INTEGER, 0, false, 1, false, false },
                {"tevkifatli_kdv_hsplar_id_array", INTEGER, 0, true, 10, false, false },
                {"kdv_muaf_mi", INTEGER, 0, false, 1, false, false },
                {"otv_muaf_mi", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"belge_numarasi", TEXT, 15, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "fat_fatura_satirlari",
            38,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"urun_id", INTEGER, 0, false, 1, false, false },
                {"urun_adi", TEXT, 50, false, 1, false, false },
                {"satirdaki_urun_miktari", REAL, 0, false, 1, false, false },
                {"irsaliyelenen_miktar", REAL, 0, false, 1, false, false },
                {"urun_birimi", TEXT, 20, false, 1, false, false },
                {"isk_oncesi_urun_birim_fiyati", REAL, 0, false, 1, false, false },
                {"isk_sonrasi_urun_birim_fiyati", REAL, 0, false, 1, false, false },
                {"satir_iskonto_yuzdesi", REAL, 0, false, 1, false, false },
                {"satir_iskonto_tutari", REAL, 0, false, 1, false, false },
                {"t_is_son_birim_fiyati", REAL, 0, false, 1, false, false },
                {"satir_tutari", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_tutari", REAL, 0, false, 1, false, false },
                {"doviz_id", INTEGER, 0, false, 1, false, false },
                {"kdv_orani", REAL, 0, false, 1, false, false },
                {"satir_kdv_tutari", REAL, 0, false, 1, false, false },
                {"satir_kdv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"otv_orani", REAL, 0, false, 1, false, false },
                {"satir_vergi_haric_tutar", REAL, 0, false, 1, false, false },
                {"satir_otv_tutari", REAL, 0, false, 1, false, false },
                {"satir_otv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_kdv_tutari", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_kdv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"t_is_son_str_vergi_haric_tutar", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_otv_tutari", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_otv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"toplu_iskonto_yuzdesi", REAL, 0, false, 1, false, false },
                {"kdv_dahil_mi", INTEGER, 0, false, 1, false, false },
                {"temel_birim_katsayisi", REAL, 0, false, 1, false, false },
                {"fiyat_irsaliyeden_alindi", INTEGER, 0, false, 1, false, false },
                {"hatali_satir_mi", INTEGER, 0, false, 1, false, false },
                {"iade_edilen_miktar", REAL, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "fat_iade_satir_baglantisi",
            3,
            {
                {"kayit_id", INTEGER, 0, false, 1, true, true },
                {"fat_iade_edilen_satir_id", INTEGER, 0, false, 1, false, false },
                {"fat_satir_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kayit_id"} }
            }
        },
        {
            "fat_irs_baglantisi",
            3,
            {
                {"kayit_id", INTEGER, 0, false, 1, true, true },
                {"fatura_id", INTEGER, 0, false, 1, false, false },
                {"irsaliye_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kayit_id"} }
            }
        },
        {
            "fat_irs_satir_baglantisi",
            3,
            {
                {"kayit_id", INTEGER, 0, false, 1, true, true },
                {"fatura_satiri_id", INTEGER, 0, false, 1, false, false },
                {"irsaliye_satiri_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kayit_id"} }
            }
        },
        {
            "irs_iade_satir_baglantisi",
            3,
            {
                {"kayit_id", INTEGER, 0, false, 1, true, true },
                {"irs_iade_edilen_satir_id", INTEGER, 0, false, 1, false, false },
                {"irs_satir_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kayit_id"} }
            }
        },
        {
            "isl_fisi",
            12,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 10, false, 1, false, false },
                {"fis_turu", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"kdv_haric_toplam", REAL, 0, false, 1, false, false },
                {"kdv_toplam", REAL, 0, false, 1, false, false },
                {"kdv_dahil_toplam", REAL, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "isl_fis_satirlari",
            13,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"belge_no", TEXT, 8, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"gider_turu", INTEGER, 0, false, 1, false, false },
                {"kdv_orani", REAL, 0, false, 1, false, false },
                {"satir_kdv_haric_tutar", REAL, 0, false, 1, false, false },
                {"satir_kdv_tutari", REAL, 0, false, 1, false, false },
                {"satir_kdv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "e9_kullanici_yetkileri",
            160,
            {
                {"yetki_id", INTEGER, 0, false, 1, true, true },
                {"kullanici_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"sis_blg_formuna_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"sis_kul_formuna_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"sis_frm_formuna_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"sis_yonetim_log_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"sis_mail_srv_tanimlayabilirmi", INTEGER, 0, false, 1, false, false },
                {"sis_sms_srv_tanimlayabilirmi", INTEGER, 0, false, 1, false, false },
                {"e9_yetkilendirme_yapabilir_mi", INTEGER, 0, false, 1, false, false },
                {"e9_onar_yapabilir_mi", INTEGER, 0, false, 1, false, false },
                {"e9_devir_islemi_yapabilir_mi", INTEGER, 0, false, 1, false, false },
                {"e9_log_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"e9_sabit_degerlere_girebilirmi", INTEGER, 0, false, 1, false, false },
                {"e9_belge_tasarlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"e9_ent_blg_degistirebilir_mi", INTEGER, 0, false, 1, false, false },
                {"e9_entegre_calisiyor_mu", INTEGER, 0, false, 1, false, false },
                {"e9_kullanicinin_sirketi", INTEGER, 0, false, 1, false, false },
                {"e9_sirket_degistire_bilir_mi", INTEGER, 0, false, 1, false, false },
                {"e9_urettigi_fisleri_grbilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"isl_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"adr_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"dvz_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"cek_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"sub_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"fat_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"irs_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"stk_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"dmr_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"smm_makbuzu_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"faiz_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prk_kullanabilir_mi", INTEGER, 0, false, 1, false, false },
                {"adr_frm_shs_blg_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"adr_not_defterini_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"adr_etiket_yazdirabilir_mi", INTEGER, 0, false, 1, false, false },
                {"adr_fihrist_yazdirabilir_mi", INTEGER, 0, false, 1, false, false },
                {"adr_toplu_mail_gonderebilir_mi", INTEGER, 0, false, 1, false, false },
                {"adr_grup_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"adr_toplu_sms_gonderebilir_mi", INTEGER, 0, false, 1, false, false },
                {"adr_adres_kayitlari_raporu", INTEGER, 0, false, 1, false, false },
                {"adr_tel_web_kayitlari_raporu", INTEGER, 0, false, 1, false, false },
                {"adr_export_yetkisi", INTEGER, 0, false, 1, false, false },
                {"adr_import_yetkisi", INTEGER, 0, false, 1, false, false },
                {"bnk_hesabi_acabilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_ent_fislerini_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_defterini_inceleyebilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_pos_ekst_inceleyebilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_hareket_fisi_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_acilis_fisi_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_makro_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_kk_ekstresi_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_virman_islemi_yapabilir_mi", INTEGER, 0, false, 1, false, false },
                {"bnk_makro_fisi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_tah_fisi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_odeme_fisi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_hareket_fisi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_acilis_fisi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_grup_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_kart_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_ent_fislerini_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_ekstreyi_inceleyebilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_bakiye_rpr_inceleyebilirmi", INTEGER, 0, false, 1, false, false },
                {"car_virman_fisi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"car_hesap_hareketleri_grb_mi", INTEGER, 0, false, 1, false, false },
                {"cek_senet_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"cek_bordro_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"cek_ent_fislerini_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"cek_listesini_inceleyebilir_mi", INTEGER, 0, false, 1, false, false },
                {"cek_mus_ceklerini_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"cek_acilis_bordrosu_yetkisi", INTEGER, 0, false, 1, false, false },
                {"dmr_karti_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"dmr_grubu_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"dmr_yeni_deger_fis_kesebilirmi", INTEGER, 0, false, 1, false, false },
                {"dmr_amrtsman_fisi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"dmr_listesi_raporu_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"dmr_toplu_amortisman_ayrablrmi", INTEGER, 0, false, 1, false, false },
                {"dmr_toplu_yeni_deger_ypblrmi", INTEGER, 0, false, 1, false, false },
                {"dmr_rpr_islm_grup_tanmlyblr_mi", INTEGER, 0, false, 1, false, false },
                {"muh_mhs_fisi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_ent_fislerini_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_hesap_planini_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_yvm_no_olusturabilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_hsp_ekstre_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_mizan_rprunu_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_hsp_plani_rpr_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_gelir_tblsunu_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_bilanco_inceleyebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_yvm_dkmunu_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_dftri_kbr_inceleyebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_fis_dokumunu_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_hata_fisleri_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_hsp_grbu_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_yan_hesabi_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"muh_yan_fisi_olusturabilir_mi", INTEGER, 0, false, 1, false, false },
                {"hesap_ekstresi_nakli_yekun", INTEGER, 0, false, 1, false, false },
                {"mizan_nakli_yekun", INTEGER, 0, false, 1, false, false },
                {"muh_hesap_plani_kopyalama", INTEGER, 0, false, 1, false, false },
                {"muh_acilis_fisi_islem_yetkisi", INTEGER, 0, false, 1, false, false },
                {"prs_tanimlarina_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_ek_odnk_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_ek_ksnti_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_sbt_degerlere_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_maas_brd_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_maas_brd_iptal_edebilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_maas_brd_rpr_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_ihbr_kdm_rpr_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_is_ayrilma_rpr_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_brd_inceleme_yapabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_prsonel_raporu_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_e_bildirge_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_prsonel_grup_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"prs_isten_ayr_prs_arayabilirmi", INTEGER, 0, false, 1, false, false },
                {"prs_eksik_gun_nedeni_girisi", INTEGER, 0, false, 1, false, false },
                {"prs_ucret_pusulasi_raporu", INTEGER, 0, false, 1, false, false },
                {"sub_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"sub_depo_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"sub_depo_islem_yetkisi", INTEGER, 0, false, 1, false, false },
                {"sub_sube_islem_yetkisi", INTEGER, 0, false, 1, false, false },
                {"sub_ent_hesaplari_yetkisi", INTEGER, 0, false, 1, false, false },
                {"stk_urun_kartina_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"stk_hizmet_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"stk_grubu_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"stk_fisine_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"stk_urn_listesini_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"stk_miktar_rprunu_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"stk_ekstre_inceleyebilir_mi", INTEGER, 0, false, 1, false, false },
                {"stk_urun_hareketlerini_grb_mi", INTEGER, 0, false, 1, false, false },
                {"isl_fisi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"isl_raporunu_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"isl_kdv_raporunu_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"fat_alis_faturasi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"fat_sts_faturasi_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"fat_irs_faturalastirma_elle", INTEGER, 0, false, 1, false, false },
                {"fat_irs_faturalastirma_oto", INTEGER, 0, false, 1, false, false },
                {"fat_satir_iskonto_yapabilir_mi", INTEGER, 0, false, 1, false, false },
                {"fat_toplu_iskonto_yapabilir_mi", INTEGER, 0, false, 1, false, false },
                {"fat_ba_bs_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"fat_hizmet_ekstre_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"fat_fatura_listesi_alabilir_mi", INTEGER, 0, false, 1, false, false },
                {"fat_irs_faturalastirma_toplu", INTEGER, 0, false, 1, false, false },
                {"irs_alis_irsaliye_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"irs_sts_irsaliye_kesebilir_mi", INTEGER, 0, false, 1, false, false },
                {"irs_fat_irsaliyelestirme_elle", INTEGER, 0, false, 1, false, false },
                {"irs_fat_irsaliyelestirme_oto", INTEGER, 0, false, 1, false, false },
                {"irs_satir_iskonto_yapabilir_mi", INTEGER, 0, false, 1, false, false },
                {"irs_listesi_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"irs_toplu_iskonto_yapabilir_mi", INTEGER, 0, false, 1, false, false },
                {"smm_makbuzu_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"smm_hizmet_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"smm_tahsil_edildi_odendi", INTEGER, 0, false, 1, false, false },
                {"dvz_kurlarini_gorebilir_mi", INTEGER, 0, false, 1, false, false },
                {"dvz_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"dvz_kuru_guncelleyebilir_mi", INTEGER, 0, false, 1, false, false },
                {"prk_fisi_girebilir_mi", INTEGER, 0, false, 1, false, false },
                {"prk_gdr_tur_tanimlayabilir_mi", INTEGER, 0, false, 1, false, false },
                {"prk_kasa_raporu_alabilir_mi", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"yetki_id"} }
            }
        },
        {
            "e9_kullanici_yetkileri_fisi",
            2,
            {
                {"kullanici_yetkileri_fisi_id", INTEGER, 0, false, 1, true, true },
                {"lock_flag", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kullanici_yetkileri_fisi_id"} }
            }
        },
        {
            "e9_log_bilgileri",
            9,
            {
                {"log_id", INTEGER, 0, false, 1, true, true },
                {"islem_tarihi", TEXT, 10, false, 1, false, false },
                {"islem_zamani", TEXT, 5, false, 1, false, false },
                {"bilgisayar_id", INTEGER, 0, false, 1, false, false },
                {"kullanici_id", INTEGER, 0, false, 1, false, false },
                {"log_turu", INTEGER, 0, false, 1, false, false },
                {"yapilan_islem", INTEGER, 0, false, 1, false, false },
                {"log_detaylari", TEXT, 1024, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"log_id"} }
            }
        },
        {
            "e9_sabit_degerler",
            59,
            {
                {"sabit_deger_id", INTEGER, 0, false, 1, true, true },
                {"adr_mail_server_adi", TEXT, 100, false, 1, false, false },
                {"adr_mail_port", TEXT, 10, false, 1, false, false },
                {"adr_mail_baglanti_guvenligi", TEXT, 10, false, 1, false, false },
                {"cek_ciro_sonrasi_gun_sayisi", INTEGER, 0, false, 1, false, false },
                {"cek_portfoy_numarasi_sayaci", INTEGER, 0, false, 1, false, false },
                {"dmr_amortisman_ayirma_yontemi", INTEGER, 0, false, 1, false, false },
                {"dmr_amor_yntemi_degisebilir_mi", INTEGER, 0, false, 1, false, false },
                {"dmr_max_amortisman_orani", REAL, 0, false, 1, false, false },
                {"fat_acik_fatura_mi_kapali_mi", INTEGER, 0, false, 1, false, false },
                {"irs_faturalandirilacak_gun", INTEGER, 0, false, 1, false, false },
                {"firma_ismi", TEXT, 100, false, 1, false, false },
                {"firma_yetkilisi", TEXT, 50, false, 1, false, false },
                {"firma_adres_satiri_1", TEXT, 35, false, 1, false, false },
                {"firma_adres_satiri_2", TEXT, 35, false, 1, false, false },
                {"firma_adres_satiri_3", TEXT, 35, false, 1, false, false },
                {"firma_ulke", TEXT, 30, false, 1, false, false },
                {"firma_sehir", TEXT, 30, false, 1, false, false },
                {"firma_ilce", TEXT, 30, false, 1, false, false },
                {"firma_semt", TEXT, 30, false, 1, false, false },
                {"firma_posta_kodu", TEXT, 10, false, 1, false, false },
                {"firma_tel_ulke_kodu", TEXT, 10, false, 1, false, false },
                {"firma_fax_ulke_kodu", TEXT, 10, false, 1, false, false },
                {"firma_tel_alan_kodu", TEXT, 10, false, 1, false, false },
                {"firma_fax_alan_kodu", TEXT, 10, false, 1, false, false },
                {"firma_telefon", TEXT, 10, false, 1, false, false },
                {"firma_fax", TEXT, 10, false, 1, false, false },
                {"firma_email", TEXT, 50, false, 1, false, false },
                {"firma_web_adresi", TEXT, 50, false, 1, false, false },
                {"firma_vergi_dairesi", TEXT, 50, false, 1, false, false },
                {"firma_vergi_numarasi", TEXT, 15, false, 1, false, false },
                {"firma_aciklama", TEXT, 512, false, 1, false, false },
                {"mali_yil_ilk_tarih", TEXT, 10, false, 1, false, false },
                {"mali_yil_son_tarih", TEXT, 10, false, 1, false, false },
                {"e9_array_son_tarih", TEXT, 10, false, 1, false, false },
                {"e9_ayraci", TEXT, 1, false, 1, false, false },
                {"kdv_orani_sayisi", INTEGER, 0, false, 1, false, false },
                {"kdv_oranlari_array", REAL, 0, true, 10, false, false },
                {"otv_orani_sayisi", INTEGER, 0, false, 1, false, false },
                {"otv_oranlari_array", REAL, 0, true, 10, false, false },
                {"firma_logosu", BLOB, 0, false, 1, false, false },
                {"ontanimli_kdv_orani", REAL, 0, false, 1, false, false },
                {"program_version", TEXT, 10, false, 1, false, false },
                {"ind_kdv_hesap_id_array", INTEGER, 0, true, 10, false, false },
                {"ind_kdv_iade_hesap_id_array", INTEGER, 0, true, 10, false, false },
                {"hes_kdv_hesap_id_array", INTEGER, 0, true, 10, false, false },
                {"hes_kdv_iade_hesap_id_array", INTEGER, 0, true, 10, false, false },
                {"otv_alis_hesap_id_array", INTEGER, 0, true, 10, false, false },
                {"otv_satis_hesap_id_array", INTEGER, 0, true, 10, false, false },
                {"tevkifat_kdv_hesap_id_array", INTEGER, 0, true, 10, false, false },
                {"isl_fisleri_guncellendi", INTEGER, 0, false, 1, false, false },
                {"max_nakit_giris_cikis_tutari", REAL, 0, false, 1, false, false },
                {"muhasebe_alarmlari_kapalimi", INTEGER, 0, false, 1, false, false },
                {"cari_hesap_kodu_counter", INTEGER, 0, false, 1, false, false },
                {"sm_sicil_numarasi", TEXT, 20, false, 1, false, false },
                {"stok_birim_fiyat_hassasiyeti", INTEGER, 0, false, 1, false, false },
                {"stok_birim_miktar_hassasiyeti", INTEGER, 0, false, 1, false, false },
                {"program_profil_id", INTEGER, 0, false, 1, false, false },
                {"e9_sabit_degerler_atandi_mi", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"sabit_deger_id"} }
            }
        },
        {
            "muh_fisler",
            15,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 10, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"toplam_borc", REAL, 0, false, 1, false, false },
                {"toplam_alacak", REAL, 0, false, 1, false, false },
                {"kasa_hesabi_id", INTEGER, 0, false, 1, false, false },
                {"fis_turu", INTEGER, 0, false, 1, false, false },
                {"yevmiye_numarasi", INTEGER, 0, false, 1, false, false },
                {"yvm_defterine_basildi", INTEGER, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "muh_fis_satirlari",
            13,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"hesap_id", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"borc_tutari", REAL, 0, false, 1, false, false },
                {"alacak_tutari", REAL, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"gizli_satir_mi", INTEGER, 0, false, 1, false, false },
                {"gizli_satir_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "muh_hesaplar",
            10,
            {
                {"hesap_id", INTEGER, 0, false, 1, true, true },
                {"parent_id", INTEGER, 0, false, 1, false, false },
                {"hesap_seviyesi", INTEGER, 0, false, 1, false, false },
                {"hesap_kodu", TEXT, 30, false, 1, false, false },
                {"hesap_ismi", TEXT, 60, false, 1, false, false },
                {"toplam_borc_array", REAL, 0, true, 501, false, false },
                {"toplam_alacak_array", REAL, 0, true, 501, false, false },
                {"tam_hesap_kodu", TEXT, 30, false, 1, false, false },
                {"tali_hesap", INTEGER, 0, false, 1, false, false },
                {"alt_hesap_sayisi", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"hesap_id"} }
            }
        },
        {
            "muh_yansitma_hesaplari",
            5,
            {
                {"kayit_id", INTEGER, 0, false, 1, true, true },
                {"gider_hesabi_id", INTEGER, 0, false, 1, false, false },
                {"yansitma_hesabi_id", INTEGER, 0, false, 1, false, false },
                {"bilanco_gelir_hesabi_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kayit_id"} }
            }
        },
        {
            "prk_fisler",
            16,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 10, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"sube_id", INTEGER, 0, false, 1, false, false },
                {"kdv_haric_toplam", REAL, 0, false, 1, false, false },
                {"kdv_toplam", REAL, 0, false, 1, false, false },
                {"kdv_dahil_toplam", REAL, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"gider_fisi_mi", INTEGER, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false },
                {"muh_kasa_hesap_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "prk_fis_satirlari",
            24,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"belge_no", TEXT, 20, false, 1, false, false },
                {"aciklama", TEXT, 128, false, 1, false, false },
                {"kdv_orani", REAL, 0, false, 1, false, false },
                {"kdv_haric_tutar", REAL, 0, false, 1, false, false },
                {"kdv_tutari", REAL, 0, false, 1, false, false },
                {"kdv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"gider_turu_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"kdv_hesap_id", INTEGER, 0, false, 1, false, false },
                {"tah_odm_sekli", INTEGER, 0, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"kredi_karti_id", INTEGER, 0, false, 1, false, false },
                {"pos_id", INTEGER, 0, false, 1, false, false },
                {"kredi_karti_sahibi", TEXT, 30, false, 1, false, false },
                {"kredi_karti_numarasi", TEXT, 16, false, 1, false, false },
                {"muh_gelir_gider_satiri_id", INTEGER, 0, false, 1, false, false },
                {"muh_kdv_satiri_id", INTEGER, 0, false, 1, false, false },
                {"muh_kasa_satiri_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "prk_gelir_gider_turleri",
            6,
            {
                {"tur_id", INTEGER, 0, false, 1, true, true },
                {"gelir_gider_adi", TEXT, 128, false, 1, false, false },
                {"hesap_id", INTEGER, 0, false, 1, false, false },
                {"sube_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"gider_turu_mu", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"tur_id"} }
            }
        },
        {
            "prs_bordrolar",
            10,
            {
                {"bordro_id", INTEGER, 0, false, 1, true, true },
                {"bordro_donemi_ay", INTEGER, 0, false, 1, false, false },
                {"bordro_donemi_yil", INTEGER, 0, false, 1, false, false },
                {"bordro_kesim_tarihi", TEXT, 10, false, 1, false, false },
                {"toplam_brut_ucret", REAL, 0, false, 1, false, false },
                {"toplam_net_ucret", REAL, 0, false, 1, false, false },
                {"toplam_brut_odenek_tutari", REAL, 0, false, 1, false, false },
                {"toplam_net_odenek_tutari", REAL, 0, false, 1, false, false },
                {"toplam_kesinti_tutari", REAL, 0, false, 1, false, false },
                {"sube_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"bordro_id"} }
            }
        },
        {
            "prs_bordro_personelleri",
            22,
            {
                {"bordro_personel_id", INTEGER, 0, false, 1, true, true },
                {"bordro_id", INTEGER, 0, false, 1, false, false },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"calisilan_gun_sayisi", INTEGER, 0, false, 1, false, false },
                {"brut_ucret", REAL, 0, false, 1, false, false },
                {"net_ucret", REAL, 0, false, 1, false, false },
                {"brd_oncesi_kum_vergi_matrahi", REAL, 0, false, 1, false, false },
                {"brd_sonrasi_kum_vergi_matrahi", REAL, 0, false, 1, false, false },
                {"sigorta_primi", REAL, 0, false, 1, false, false },
                {"issizlik_primi", REAL, 0, false, 1, false, false },
                {"gelir_vergi_matrahi", REAL, 0, false, 1, false, false },
                {"gelir_vergisi", REAL, 0, false, 1, false, false },
                {"asg_gec_indirimi", REAL, 0, false, 1, false, false },
                {"damga_vergisi", REAL, 0, false, 1, false, false },
                {"kesintiler_toplami", REAL, 0, false, 1, false, false },
                {"odenekler_toplami", REAL, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"puantaj_id", INTEGER, 0, false, 1, false, false },
                {"ozel_kesintiler_toplami", REAL, 0, false, 1, false, false },
                {"ssk_matrahi", REAL, 0, false, 1, false, false },
                {"bir_ay_once_kalan_ek_odenek", REAL, 0, false, 1, false, false },
                {"iki_ay_once_kalan_ek_odenek", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"bordro_personel_id"} }
            }
        },
        {
            "prs_brd_kesinti_odenekleri",
            18,
            {
                {"brd_kesinti_odenek_id", INTEGER, 0, false, 1, true, true },
                {"bordro_personel_id", INTEGER, 0, false, 1, false, false },
                {"bordro_id", INTEGER, 0, false, 1, false, false },
                {"kesinti_odenek_ismi", TEXT, 128, false, 1, false, false },
                {"yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"tutari", REAL, 0, false, 1, false, false },
                {"kesinti_odenek_yuzdesi", REAL, 0, false, 1, false, false },
                {"ssk_kesintisi_iceriyor_mu", INTEGER, 0, false, 1, false, false },
                {"ssk_kesinti_matrahi", REAL, 0, false, 1, false, false },
                {"ssk_kesinti_tutari", REAL, 0, false, 1, false, false },
                {"gelir_vergisi_iceriyor_mu", INTEGER, 0, false, 1, false, false },
                {"gelir_vergisi_matrahi", REAL, 0, false, 1, false, false },
                {"gelir_vergisi_tutari", REAL, 0, false, 1, false, false },
                {"damga_vergisi_iceriyor_mu", INTEGER, 0, false, 1, false, false },
                {"damga_vergisi_matrahi", REAL, 0, false, 1, false, false },
                {"damga_vergisi_tutari", REAL, 0, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"kesinti_odenek_turu", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"brd_kesinti_odenek_id"} }
            }
        },
        {
            "prs_ek_kesintiler_odenekler",
            17,
            {
                {"kesinti_odenek_id", INTEGER, 0, false, 1, true, true },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"bordro_id", INTEGER, 0, false, 1, false, false },
                {"odenek_kesinti_adi", TEXT, 128, false, 1, false, false },
                {"bordro_donemi_ay", INTEGER, 0, false, 1, false, false },
                {"bordro_donemi_yil", INTEGER, 0, false, 1, false, false },
                {"yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"yuzde_orani", REAL, 0, false, 1, false, false },
                {"tutar", REAL, 0, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"kesinti_odenek_turu", INTEGER, 0, false, 1, false, false },
                {"bordrosu_kesildi_mi", INTEGER, 0, false, 1, false, false },
                {"ssk_kesintisi_iceriyor_mu", INTEGER, 0, false, 1, false, false },
                {"gelir_vergisi_iceriyor_mu", INTEGER, 0, false, 1, false, false },
                {"damga_vergisi_iceriyor_mu", INTEGER, 0, false, 1, false, false },
                {"kes_ode_tanim_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kesinti_odenek_id"} }
            }
        },
        {
            "prs_eksik_gun_nedenleri",
            8,
            {
                {"eksik_gun_id", INTEGER, 0, false, 1, true, true },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"bordro_donemi_ay", INTEGER, 0, false, 1, false, false },
                {"bordro_donemi_yil", INTEGER, 0, false, 1, false, false },
                {"eksik_gun_sayisi", REAL, 0, false, 1, false, false },
                {"eksik_gun_neden_kodu", TEXT, 5, false, 1, false, false },
                {"bas_tarihi", TEXT, 10, false, 1, false, false },
                {"bts_tarihi", TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"eksik_gun_id"} }
            }
        },
        {
            "prs_firma_odenekleri",
            71,
            {
                {"odenekler_id", INTEGER, 0, false, 1, true, true },
                {"dogum_yardimi_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"dogum_yardimi_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"dogum_yardimi_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"dogum_yardimi_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"dogum_yardimi_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"dogum_yardimi_yuzdesi", REAL, 0, false, 1, false, false },
                {"dogum_yardimi_tutari", REAL, 0, false, 1, false, false },
                {"olum_yardimi_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"olum_yardimi_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"olum_yardimi_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"olum_yardimi_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"olum_yardimi_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"olum_yardimi_yuzdesi", REAL, 0, false, 1, false, false },
                {"olum_yardimi_tutari", REAL, 0, false, 1, false, false },
                {"cocuk_yardimi_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"cocuk_yardimi_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"cocuk_yardimi_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"cocuk_yardimi_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"cocuk_yardimi_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"cocuk_yardimi_yuzdesi", REAL, 0, false, 1, false, false },
                {"cocuk_yardimi_tutari", REAL, 0, false, 1, false, false },
                {"yakacak_yardimi_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"yakacak_yardimi_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"yakacak_yardimi_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"yakacak_yardimi_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"yakacak_yardimi_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"yakacak_yardimi_yuzdesi", REAL, 0, false, 1, false, false },
                {"yakacak_yardimi_tutari", REAL, 0, false, 1, false, false },
                {"aile_yardimi_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"aile_yardimi_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"aile_yardimi_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"aile_yardimi_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"aile_yardimi_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"aile_yardimi_yuzdesi", REAL, 0, false, 1, false, false },
                {"aile_yardimi_tutari", REAL, 0, false, 1, false, false },
                {"yemek_parasi_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"yemek_parasi_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"yemek_parasi_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"yemek_parasi_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"yemek_parasi_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"yemek_parasi_yuzdesi", REAL, 0, false, 1, false, false },
                {"yemek_parasi_tutari", REAL, 0, false, 1, false, false },
                {"evlenme_yardimi_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"evlenme_yardimi_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"evlenme_yardimi_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"evlenme_yardimi_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"evlenme_yardimi_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"evlenme_yardimi_yuzdesi", REAL, 0, false, 1, false, false },
                {"evlenme_yardimi_tutari", REAL, 0, false, 1, false, false },
                {"gorev_harcirahi_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"gorev_harcirahi_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"gorev_harcirahi_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"gorev_harcirahi_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"gorev_harcirahi_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"gorev_harcirahi_yuzdesi", REAL, 0, false, 1, false, false },
                {"gorev_harcirahi_tutari", REAL, 0, false, 1, false, false },
                {"ihbar_tazminati_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"ihbar_tazminati_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"ihbar_tazminati_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"ihbar_tazminati_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"ihbar_tazminati_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"ihbar_tazminati_yuzdesi", REAL, 0, false, 1, false, false },
                {"ihbar_tazminati_tutari", REAL, 0, false, 1, false, false },
                {"kidem_tazminati_var_mi_enum", INTEGER, 0, false, 1, false, false },
                {"kidem_tazminati_ssk_kesintisi", INTEGER, 0, false, 1, false, false },
                {"kidem_tazminati_gelir_vergisi", INTEGER, 0, false, 1, false, false },
                {"kidem_tazminati_damga_vergisi", INTEGER, 0, false, 1, false, false },
                {"kidem_tazminati_yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"kidem_tazminati_yuzdesi", REAL, 0, false, 1, false, false },
                {"kidem_tazminati_tutari", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"odenekler_id"} }
            }
        },
        {
            "prs_isten_ayrilanlar",
            17,
            {
                {"isten_ayrilan_id", INTEGER, 0, false, 1, true, true },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"isten_ayrilma_tarihi", TEXT, 10, false, 1, false, false },
                {"isten_ayrilma_sekli_enum", INTEGER, 0, false, 1, false, false },
                {"isten_ayrilma_sekli_text", TEXT, 128, false, 1, false, false },
                {"calisma_suresi_yil", INTEGER, 0, false, 1, false, false },
                {"calisma_suresi_ay", INTEGER, 0, false, 1, false, false },
                {"calisma_suresi_gun", INTEGER, 0, false, 1, false, false },
                {"ihbar_taz_var_mi", INTEGER, 0, false, 1, false, false },
                {"net_ihbar_taz_tutari", REAL, 0, false, 1, false, false },
                {"brut_ihbar_taz_tutari", REAL, 0, false, 1, false, false },
                {"ihbar_taz_gelir_vergisi_tutari", REAL, 0, false, 1, false, false },
                {"ihbar_taz_damga_vergisi_tutari", REAL, 0, false, 1, false, false },
                {"kidem_taz_var_mi", INTEGER, 0, false, 1, false, false },
                {"net_kidem_taz_tutari", REAL, 0, false, 1, false, false },
                {"brut_kidem_taz_tutari", REAL, 0, false, 1, false, false },
                {"kidem_taz_damga_vergisi_tutari", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"isten_ayrilan_id"} }
            }
        },
        {
            "prs_kesinti_odenek_tanimlar",
            15,
            {
                {"kes_ode_tanim_id", INTEGER, 0, false, 1, true, true },
                {"kesinti_odenek_no", INTEGER, 0, false, 1, false, false },
                {"kesinti_odenek_tarihi", TEXT, 10, false, 1, false, false },
                {"kesinti_odenek_adi", TEXT, 50, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"bordro_donemi_ay", INTEGER, 0, false, 1, false, false },
                {"bordro_donemi_yil", INTEGER, 0, false, 1, false, false },
                {"ssk_kesintisi_iceriyor_mu", INTEGER, 0, false, 1, false, false },
                {"damga_vergisi_iceriyor_mu", INTEGER, 0, false, 1, false, false },
                {"gelir_vergisi_iceriyor_mu", INTEGER, 0, false, 1, false, false },
                {"yuzde_mi", INTEGER, 0, false, 1, false, false },
                {"tutar", REAL, 0, false, 1, false, false },
                {"kesinti_odenek_turu", INTEGER, 0, false, 1, false, false },
                {"last_order_number", INTEGER, 0, false, 1, false, false },
                {"bordrosu_kesildi_mi", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kes_ode_tanim_id"} }
            }
        },
        {
            "prs_personel_aile_uyeleri",
            12,
            {
                {"aile_bilgi_id", INTEGER, 0, false, 1, true, true },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"adi_soyadi", TEXT, 100, false, 1, false, false },
                {"tc_kimlik_no", TEXT, 11, false, 1, false, false },
                {"akrabalik_derecesi", INTEGER, 0, false, 1, false, false },
                {"dogum_tarihi", TEXT, 10, false, 1, false, false },
                {"ogrenim_durumu", TEXT, 50, false, 1, false, false },
                {"calisma_durumu", INTEGER, 0, false, 1, false, false },
                {"asgari_gecim_ind_durumu", INTEGER, 0, false, 1, false, false },
                {"cinsiyet", TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"aile_bilgi_id"} }
            }
        },
        {
            "prs_personel_kisisel_blgler",
            20,
            {
                {"kisisel_bilgi_id", INTEGER, 0, false, 1, true, true },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"uyruk", TEXT, 20, false, 1, false, false },
                {"pozisyon", TEXT, 50, false, 1, false, false },
                {"departman", TEXT, 50, false, 1, false, false },
                {"ozel_durum_enum", INTEGER, 0, false, 1, false, false },
                {"meslek", TEXT, 30, false, 1, false, false },
                {"unvan", TEXT, 30, false, 1, false, false },
                {"ogrenim_durumu", TEXT, 30, false, 1, false, false },
                {"mezun_oldugu_okul", TEXT, 40, false, 1, false, false },
                {"mezun_oldugu_bolum", TEXT, 40, false, 1, false, false },
                {"yabanci_dil_1", TEXT, 20, false, 1, false, false },
                {"yabanci_dil_2", TEXT, 20, false, 1, false, false },
                {"muracaat_tarihi", TEXT, 10, false, 1, false, false },
                {"ise_giris_tarihi", TEXT, 10, false, 1, false, false },
                {"isten_cikis_tarihi", TEXT, 10, false, 1, false, false },
                {"ssk_basvuru_tarihi", TEXT, 10, false, 1, false, false },
                {"ozel_indirim_dilekcesi", TEXT, 512, false, 1, false, false },
                {"sendika_uyesi_mi", INTEGER, 0, false, 1, false, false },
                {"resim", BLOB, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kisisel_bilgi_id"} }
            }
        },
        {
            "prs_personeller",
            30,
            {
                {"personel_id", INTEGER, 0, false, 1, true, true },
                {"cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"sicil_no", TEXT, 20, false, 1, false, false },
                {"sube_id", INTEGER, 0, false, 1, false, false },
                {"prs_avanslar_hesap_id", INTEGER, 0, false, 1, false, false },
                {"prs_borclar_hesap_id", INTEGER, 0, false, 1, false, false },
                {"sigorta_no", TEXT, 20, false, 1, false, false },
                {"emekli_mi", INTEGER, 0, false, 1, false, false },
                {"ucret_sekli", INTEGER, 0, false, 1, false, false },
                {"net_ucret", REAL, 0, false, 1, false, false },
                {"brut_ucret", REAL, 0, false, 1, false, false },
                {"kumulatif_vergi_matrahi", REAL, 0, false, 1, false, false },
                {"isten_ayrildi_mi", INTEGER, 0, false, 1, false, false },
                {"bir_ay_once_kalan_ek_odenek", REAL, 0, false, 1, false, false },
                {"iki_ay_once_kalan_ek_odenek", REAL, 0, false, 1, false, false },
                {"calisma_sekli", INTEGER, 0, false, 1, false, false },
                {"ind_kanunu_kodu", TEXT, 5, false, 1, false, false },
                {"calisma_turu_kodu", TEXT, 2, false, 1, false, false },
                {"dogum_yardimi_var_mi", INTEGER, 0, false, 1, false, false },
                {"olum_yardimi_var_mi", INTEGER, 0, false, 1, false, false },
                {"cocuk_yardimi_var_mi", INTEGER, 0, false, 1, false, false },
                {"yakacak_yardimi_var_mi", INTEGER, 0, false, 1, false, false },
                {"aile_yardimi_var_mi", INTEGER, 0, false, 1, false, false },
                {"yemek_parasi_var_mi", INTEGER, 0, false, 1, false, false },
                {"evlenme_yardimi_var_mi", INTEGER, 0, false, 1, false, false },
                {"gorev_harcirahi_var_mi", INTEGER, 0, false, 1, false, false },
                {"ihbar_tazminati_var_mi", INTEGER, 0, false, 1, false, false },
                {"kidem_tazminati_var_mi", INTEGER, 0, false, 1, false, false },
                {"haftalik_calisilan_gun_sayisi", REAL, 0, false, 1, false, false },
                {"gunluk_yemek_parasi_tutari", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"personel_id"} }
            }
        },
        {
            "prs_personel_notlari",
            3,
            {
                {"notlar_id", INTEGER, 0, false, 1, true, true },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"notlar", TEXT, 512, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"notlar_id"} }
            }
        },
        {
            "prs_personel_nufus_blgler",
            17,
            {
                {"nufus_id", INTEGER, 0, false, 1, true, true },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"nufus_cuzdani_seri_no", TEXT, 10, false, 1, false, false },
                {"baba_adi", TEXT, 50, false, 1, false, false },
                {"anne_adi", TEXT, 50, false, 1, false, false },
                {"dini", TEXT, 10, false, 1, false, false },
                {"ilk_soyadi", TEXT, 30, false, 1, false, false },
                {"kayitli_oldugu_il", TEXT, 30, false, 1, false, false },
                {"kayitli_oldugu_ilce", TEXT, 30, false, 1, false, false },
                {"kayitli_oldugu_mahalle", TEXT, 30, false, 1, false, false },
                {"cilt_no", TEXT, 10, false, 1, false, false },
                {"aile_sira_no", TEXT, 10, false, 1, false, false },
                {"sira_no", TEXT, 10, false, 1, false, false },
                {"verildigi_yer", TEXT, 30, false, 1, false, false },
                {"verilis_nedeni", TEXT, 20, false, 1, false, false },
                {"kayit_no", INTEGER, 0, false, 1, false, false },
                {"verilis_tarihi", TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"nufus_id"} }
            }
        },
        {
            "prs_puantaj",
            10,
            {
                {"puantaj_id", INTEGER, 0, false, 1, true, true },
                {"personel_id", INTEGER, 0, false, 1, false, false },
                {"gun", REAL, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false },
                {"temel_para_birim_tutari", REAL, 0, false, 1, false, false },
                {"bordro_donemi_ay", INTEGER, 0, false, 1, false, false },
                {"bordro_donemi_yil", INTEGER, 0, false, 1, false, false },
                {"gun_sayisi", REAL, 0, false, 1, false, false },
                {"yemek_verilen_gun_sayisi", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"puantaj_id"} }
            }
        },
        {
            "prs_sabit_degerler",
            43,
            {
                {"sabit_degerler_id", INTEGER, 0, false, 1, true, true },
                {"issizlik_sig_isci_yuzdesi", REAL, 0, false, 1, false, false },
                {"issizlik_sig_isveren_yuzdesi", REAL, 0, false, 1, false, false },
                {"sakatlik_ind_derece_1_tutari", REAL, 0, false, 1, false, false },
                {"sakatlik_ind_derece_2_tutari", REAL, 0, false, 1, false, false },
                {"sakatlik_ind_derece_3_tutari", REAL, 0, false, 1, false, false },
                {"tehlike_sinifi", INTEGER, 0, false, 1, false, false },
                {"ssk_primi_isci_yuzdesi", REAL, 0, false, 1, false, false },
                {"ssk_primi_isveren_yuzdesi", REAL, 0, false, 1, false, false },
                {"ssk_taban_degeri", REAL, 0, false, 1, false, false },
                {"ssk_tavan_degeri", REAL, 0, false, 1, false, false },
                {"ssk_16_yas_alti_asgari_ucret", REAL, 0, false, 1, false, false },
                {"ssk_16_yas_ustu_asgari_ucret", REAL, 0, false, 1, false, false },
                {"damga_vergisi", REAL, 0, false, 1, false, false },
                {"glr_ver_dilim_bas_tutar_array", REAL, 0, true, 10, false, false },
                {"glr_ver_dilim_bts_tutar_array", REAL, 0, true, 10, false, false },
                {"gelir_vergisi_yuzdeleri_array", REAL, 0, true, 10, false, false },
                {"gecerli_glr_ver_dilimi_sayisi", INTEGER, 0, false, 1, false, false },
                {"evli_es_clsmyr_asggecind_array", REAL, 0, true, 5, false, false },
                {"evli_es_clsyr_asggecind_array", REAL, 0, true, 5, false, false },
                {"bekar_asg_gec_indirimler_array", REAL, 0, true, 5, false, false },
                {"ihbr_taz_bas_trh_turleri_array", INTEGER, 0, true, 4, false, false },
                {"ihbr_taz_bas_trh_orj_deg_array", REAL, 0, true, 4, false, false },
                {"ihbr_taz_bas_trh_ay_deg_array", REAL, 0, true, 4, false, false },
                {"ihbr_taz_bts_trh_turleri_array", INTEGER, 0, true, 4, false, false },
                {"ihbr_taz_bts_trh_orj_deg_array", REAL, 0, true, 4, false, false },
                {"ihbr_taz_bts_trh_ay_deg_array", REAL, 0, true, 4, false, false },
                {"ihbar_taz_hafta_deg_array", REAL, 0, true, 4, false, false },
                {"frm_tabi_sigorta_kolu", INTEGER, 0, false, 1, false, false },
                {"frm_sendika_uyesi_mi", INTEGER, 0, false, 1, false, false },
                {"sendika_aidati", REAL, 0, false, 1, false, false },
                {"ibraname_maddeleri", TEXT, 950, false, 1, false, false },
                {"tehlike_sinifi_isci_yuzdesi", REAL, 0, false, 1, false, false },
                {"tehlike_sinifi_isveren_yuzdesi", REAL, 0, false, 1, false, false },
                {"ssk_isveren_taban_deger", REAL, 0, false, 1, false, false },
                {"ssk_isveren_tavan_deger", REAL, 0, false, 1, false, false },
                {"muh_fis_hesaplari_birlestir", INTEGER, 0, false, 1, false, false },
                {"prs_sgdp_primi_isci_yuzdesi", REAL, 0, false, 1, false, false },
                {"prs_sgdp_primi_isveren_yuzdesi", REAL, 0, false, 1, false, false },
                {"isy_5510_nolu_kanuna_tabi_mi", INTEGER, 0, false, 1, false, false },
                {"isy_5510_nolu_kanun_yuzdesi", REAL, 0, false, 1, false, false },
                {"haftalik_calisilan_gun_sayisi", REAL, 0, false, 1, false, false },
                {"gunluk_yemek_yardimi_yuzdesi", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"sabit_degerler_id"} }
            }
        },
        {
            "smm_hizmetler",
            7,
            {
                {"hizmet_id", INTEGER, 0, false, 1, true, true },
                {"hizmet_adi", TEXT, 128, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"hizmet_hesap_id", INTEGER, 0, false, 1, false, false },
                {"vergi_hesap_id", INTEGER, 0, false, 1, false, false },
                {"hizmet_turu", INTEGER, 0, false, 1, false, false },
                {"sube_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"hizmet_id"} }
            }
        },
        {
            "smm_makbuzlar",
            37,
            {
                {"makbuz_id", INTEGER, 0, false, 1, true, true },
                {"makbuz_no", INTEGER, 0, false, 1, false, false },
                {"makbuz_tarihi", TEXT, 10, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"cari_hesap_adresi", TEXT, 512, false, 1, false, false },
                {"vergi_dairesi", TEXT, 50, false, 1, false, false },
                {"vergi_numarasi", TEXT, 15, false, 1, false, false },
                {"sm_sicil_numarasi", TEXT, 20, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false },
                {"hizmet_tutari", REAL, 0, false, 1, false, false },
                {"makbuz_turu", INTEGER, 0, false, 1, false, false },
                {"muh_kasa_hesap_id", INTEGER, 0, false, 1, false, false },
                {"tevkifatli_mi", INTEGER, 0, false, 1, false, false },
                {"tevkifat_yuzdesi", REAL, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"belge_seri", TEXT, 3, false, 1, false, false },
                {"belge_numarasi", TEXT, 15, false, 1, false, false },
                {"tahsil_edildi_odendi", INTEGER, 0, false, 1, false, false },
                {"brut_tutar", REAL, 0, false, 1, false, false },
                {"tevkifat_tutari", REAL, 0, false, 1, false, false },
                {"kdv_tevkifat_muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"kdv_orani", REAL, 0, false, 1, false, false },
                {"vergi_stopaj_orani", REAL, 0, false, 1, false, false },
                {"kdv_hesap_id", INTEGER, 0, false, 1, false, false },
                {"vergi_stopaj_tutari", REAL, 0, false, 1, false, false },
                {"net_tutar", REAL, 0, false, 1, false, false },
                {"hizmet_id", INTEGER, 0, false, 1, false, false },
                {"tah_odm_sekli", INTEGER, 0, false, 1, false, false },
                {"hesap_no_id", INTEGER, 0, false, 1, false, false },
                {"kredi_karti_id", INTEGER, 0, false, 1, false, false },
                {"pos_id", INTEGER, 0, false, 1, false, false },
                {"kredi_karti_numarasi", TEXT, 16, false, 1, false, false },
                {"kdv_tutari", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"makbuz_id"} }
            }
        },
        {
            "stk_depodaki_urunler",
            5,
            {
                {"kayit_id", INTEGER, 0, false, 1, true, true },
                {"depo_id", INTEGER, 0, false, 1, false, false },
                {"urun_id", INTEGER, 0, false, 1, false, false },
                {"toplam_depoya_giren_array", REAL, 0, true, 501, false, false },
                {"toplam_depodan_cikan_array", REAL, 0, true, 501, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kayit_id"} }
            }
        },
        {
            "stk_fisler",
            50,
            {
                {"fis_id", INTEGER, 0, false, 1, true, true },
                {"fis_no", INTEGER, 0, false, 1, false, false },
                {"fis_tarihi", TEXT, 10, false, 1, false, false },
                {"islem_saati", TEXT, 5, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"irsaliye_mi_stok_fisi_mi", INTEGER, 0, false, 1, false, false },
                {"fis_turu", INTEGER, 0, false, 1, false, false },
                {"irsaliye_alis_satis_turu", INTEGER, 0, false, 1, false, false },
                {"brut_tutar", REAL, 0, false, 1, false, false },
                {"isk_oncesi_otv_tutari", REAL, 0, false, 1, false, false },
                {"isk_oncesi_otv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"isk_oncesi_kdv_tutari", REAL, 0, false, 1, false, false },
                {"isk_oncesi_kdv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"ara_toplam", REAL, 0, false, 1, false, false },
                {"isk_sonrasi_otv_tutari", REAL, 0, false, 1, false, false },
                {"isk_sonrasi_otv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"isk_sonrasi_kdv_tutari", REAL, 0, false, 1, false, false },
                {"irsaliye_tutari", REAL, 0, false, 1, false, false },
                {"iskonto_yuzdesi", REAL, 0, false, 1, false, false },
                {"iskonto_tutari", REAL, 0, false, 1, false, false },
                {"doviz_tutar_bilgileri", TEXT, 512, false, 1, false, false },
                {"kdv_oran_bilgileri", TEXT, 512, false, 1, false, false },
                {"otv_oran_bilgileri", TEXT, 512, false, 1, false, false },
                {"depo_id", INTEGER, 0, false, 1, false, false },
                {"alan_depo_id", INTEGER, 0, false, 1, false, false },
                {"sevk_tarihi", TEXT, 10, false, 1, false, false },
                {"belge_seri", TEXT, 3, false, 1, false, false },
                {"cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"cari_hesap_ismi", TEXT, 60, false, 1, false, false },
                {"vergi_dairesi", TEXT, 50, false, 1, false, false },
                {"vergi_numarasi", TEXT, 15, false, 1, false, false },
                {"teslim_adresi", TEXT, 512, false, 1, false, false },
                {"yazdirildi_mi", INTEGER, 0, false, 1, false, false },
                {"iptal_mi", INTEGER, 0, false, 1, false, false },
                {"irsaliye_faturalastirildi_mi", INTEGER, 0, false, 1, false, false },
                {"hatali_irsaliye_mi", INTEGER, 0, false, 1, false, false },
                {"base_fis_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"iade_irsaliyesi_mi", INTEGER, 0, false, 1, false, false },
                {"iade_edilen_irs_id", INTEGER, 0, false, 1, false, false },
                {"kdv_muaf_mi", INTEGER, 0, false, 1, false, false },
                {"otv_muaf_mi", INTEGER, 0, false, 1, false, false },
                {"muh_fis_id", INTEGER, 0, false, 1, false, false },
                {"muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"belge_numarasi", TEXT, 15, false, 1, false, false },
                {"stk_irs_para_birim_id", INTEGER, 0, false, 1, false, false },
                {"parite", REAL, 0, false, 1, false, false },
                {"doviz_kuru", REAL, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_id"} }
            }
        },
        {
            "stk_fis_satirlari",
            38,
            {
                {"fis_satiri_id", INTEGER, 0, false, 1, true, true },
                {"fis_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"urun_id", INTEGER, 0, false, 1, false, false },
                {"urun_adi", TEXT, 50, false, 1, false, false },
                {"satirdaki_urun_miktari", REAL, 0, false, 1, false, false },
                {"urun_birimi", TEXT, 20, false, 1, false, false },
                {"temel_birim_katsayisi", REAL, 0, false, 1, false, false },
                {"isk_oncesi_urun_birim_fiyati", REAL, 0, false, 1, false, false },
                {"isk_sonrasi_urun_birim_fiyati", REAL, 0, false, 1, false, false },
                {"satir_iskonto_yuzdesi", REAL, 0, false, 1, false, false },
                {"satir_iskonto_tutari", REAL, 0, false, 1, false, false },
                {"satir_tutari", REAL, 0, false, 1, false, false },
                {"doviz_id", INTEGER, 0, false, 1, false, false },
                {"parti_id", INTEGER, 0, false, 1, false, false },
                {"kdv_orani", REAL, 0, false, 1, false, false },
                {"otv_orani", REAL, 0, false, 1, false, false },
                {"toplu_iskonto_yuzdesi", REAL, 0, false, 1, false, false },
                {"t_is_son_birim_fiyati", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_tutari", REAL, 0, false, 1, false, false },
                {"t_is_son_str_vergi_haric_tutar", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_otv_tutari", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_otv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_kdv_tutari", REAL, 0, false, 1, false, false },
                {"t_is_son_satir_kdv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"satir_vergi_haric_tutar", REAL, 0, false, 1, false, false },
                {"satir_otv_tutari", REAL, 0, false, 1, false, false },
                {"satir_otv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"satir_kdv_tutari", REAL, 0, false, 1, false, false },
                {"satir_kdv_dahil_tutar", REAL, 0, false, 1, false, false },
                {"kdv_dahil_mi", INTEGER, 0, false, 1, false, false },
                {"faturalanan_miktar", REAL, 0, false, 1, false, false },
                {"iade_edilen_miktar", REAL, 0, false, 1, false, false },
                {"hatali_satir_mi", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"base_record_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"fis_satiri_id"} }
            }
        },
        {
            "stk_satirdaki_seri_nolari",
            4,
            {
                {"satir_seri_no_id", INTEGER, 0, false, 1, true, true },
                {"irsaliye_satiri_id", INTEGER, 0, false, 1, false, false },
                {"seri_numarasi_id", INTEGER, 0, false, 1, false, false },
                {"order_number", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"satir_seri_no_id"} }
            }
        },
        {
            "stk_urunler",
            31,
            {
                {"urun_id", INTEGER, 0, false, 1, true, true },
                {"urun_kodu", TEXT, 30, false, 1, false, false },
                {"urun_adi", TEXT, 50, false, 1, false, false },
                {"barkod_numarasi", TEXT, 50, false, 1, false, false },
                {"toplam_stoga_giren_array", REAL, 0, true, 501, false, false },
                {"toplam_stoktan_cikan_array", REAL, 0, true, 501, false, false },
                {"alis_fiyati", REAL, 0, false, 1, false, false },
                {"satis_fiyati", REAL, 0, false, 1, false, false },
                {"urun_alis_doviz_id", INTEGER, 0, false, 1, false, false },
                {"urun_satis_doviz_id", INTEGER, 0, false, 1, false, false },
                {"stok_takip_sekli", INTEGER, 0, false, 1, false, false },
                {"urun_temel_birimi", TEXT, 20, false, 1, false, false },
                {"min_stok_seviyesi", REAL, 0, false, 1, false, false },
                {"kritik_stok_seviyesi", REAL, 0, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"otv_orani", REAL, 0, false, 1, false, false },
                {"kdv_orani", REAL, 0, false, 1, false, false },
                {"alis_otv_orani", REAL, 0, false, 1, false, false },
                {"alis_kdv_orani", REAL, 0, false, 1, false, false },
                {"satis_otv_orani", REAL, 0, false, 1, false, false },
                {"satis_kdv_orani", REAL, 0, false, 1, false, false },
                {"urun_birimleri_array", TEXT, 20, true, 4, false, false },
                {"urun_birim_katsayilari_array", REAL, 0, true, 4, false, false },
                {"urun_capraz_birimler_array", INTEGER, 0, true, 4, false, false },
                {"urun_birim_alis_fiyatlar_array", REAL, 0, true, 4, false, false },
                {"urun_birim_sts_fiyatlar_array", REAL, 0, true, 4, false, false },
                {"gorunecek_urn_birimleri_array", INTEGER, 0, true, 4, false, false },
                {"hizmet_mi", INTEGER, 0, false, 1, false, false },
                {"muh_hesap_id", INTEGER, 0, false, 1, false, false },
                {"hizmet_turu", INTEGER, 0, false, 1, false, false },
                {"fotograf", BLOB, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"urun_id"} }
            }
        },
        {
            "stk_urunun_partileri",
            8,
            {
                {"parti_id", INTEGER, 0, false, 1, true, true },
                {"depo_id", INTEGER, 0, false, 1, false, false },
                {"urun_id", INTEGER, 0, false, 1, false, false },
                {"parti_kodu", TEXT, 30, false, 1, false, false },
                {"urun_uretim_tarihi", TEXT, 10, false, 1, false, false },
                {"urun_son_kul_tarihi", TEXT, 10, false, 1, false, false },
                {"toplam_partiye_giren_array", REAL, 0, true, 501, false, false },
                {"toplam_partiden_cikan_array", REAL, 0, true, 501, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"parti_id"} }
            }
        },
        {
            "stk_urunun_seri_nolari",
            6,
            {
                {"seri_numarasi_id", INTEGER, 0, false, 1, true, true },
                {"depo_id", INTEGER, 0, false, 1, false, false },
                {"urun_id", INTEGER, 0, false, 1, false, false },
                {"seri_numarasi", TEXT, 50, false, 1, false, false },
                {"depoda_mi", INTEGER, 0, false, 1, false, false },
                {"fatura_satiri_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"seri_numarasi_id"} }
            }
        },
        {
            "sub_depolar",
            6,
            {
                {"depo_id", INTEGER, 0, false, 1, true, true },
                {"depo_kodu", TEXT, 10, false, 1, false, false },
                {"depo_adi", TEXT, 20, false, 1, false, false },
                {"depo_adresi", TEXT, 512, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"sube_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"depo_id"} }
            }
        },
        {
            "sub_subeler",
            72,
            {
                {"sube_id", INTEGER, 0, false, 1, true, true },
                {"sube_kodu", TEXT, 10, false, 1, false, false },
                {"sube_adi", TEXT, 30, false, 1, false, false },
                {"aciklama", TEXT, 512, false, 1, false, false },
                {"sube_adresi", TEXT, 512, false, 1, false, false },
                {"sube_telefon", TEXT, 15, false, 1, false, false },
                {"sube_fax", TEXT, 15, false, 1, false, false },
                {"sgk_isyeri_no", TEXT, 15, false, 1, false, false },
                {"sube_port_cekler_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_tah_cekler_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_tem_cekler_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_kar_cekler_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_port_snt_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_tah_snt_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_tem_snt_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_pro_snt_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_port_snt_ileri_trh_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_tah_snt_ileri_trh_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_tem_snt_ileri_trh_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_borc_snt_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_borc_snt_ileri_trh_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_sup_alacaklar_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_tah_edlmyn_cekler_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_tah_edlmyn_snt_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_yurtici_satis_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_hizmet_urt_maliyet_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_yap_indirim_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_prs_gdrlr_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_sarf_malzemeleri_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sube_imalattan_giris_hsp_id", INTEGER, 0, false, 1, false, false },
                {"kasa_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"cari_alici_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"cari_satici_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"banka_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"ver_cek_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"odeme_emirleri_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"pos_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"pos_kom_gideri_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"kk_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"prk_gider_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"urunler_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"hizmetler_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"demirbaslar_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"prs_avanslari_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"personel_borclari_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"prs_damga_vergisi_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"personel_vergi_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"prk_gelir_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"prs_ssk_isci_payi_hesap_id", INTEGER, 0, false, 1, false, false },
                {"prs_ssk_isveren_payi_hesap_id", INTEGER, 0, false, 1, false, false },
                {"prs_issizlik_sgk_isci_hsp_id", INTEGER, 0, false, 1, false, false },
                {"prs_issizlik_sgk_isv_hsp_id", INTEGER, 0, false, 1, false, false },
                {"asg_gecim_ind_hesap_id", INTEGER, 0, false, 1, false, false },
                {"isveren_payi_giderleri_hps_id", INTEGER, 0, false, 1, false, false },
                {"bir_amortismanlar_hsp_id", INTEGER, 0, false, 1, false, false },
                {"amor_giderleri_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sgdp_prim_yuzde_isc_hsp_id", INTEGER, 0, false, 1, false, false },
                {"sgdp_prim_yuzde_isv_hsp_id", INTEGER, 0, false, 1, false, false },
                {"kidem_tazminati_hesap_id", INTEGER, 0, false, 1, false, false },
                {"ihbar_tazminati_hesap_id", INTEGER, 0, false, 1, false, false },
                {"dmr_satis_kar_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"dmr_satis_zarar_ent_hesap_id", INTEGER, 0, false, 1, false, false },
                {"sendika_odeme_hesap_id", INTEGER, 0, false, 1, false, false },
                {"smmm_gider_hesap_id", INTEGER, 0, false, 1, false, false },
                {"smmm_gelir_vergisi_hesap_id", INTEGER, 0, false, 1, false, false },
                {"smmm_alinan_hizmet_hesap_id", INTEGER, 0, false, 1, false, false },
                {"smmm_satilan_hizmet_hesap_id", INTEGER, 0, false, 1, false, false },
                {"isy_5510_nolu_kanun_hesap_id", INTEGER, 0, false, 1, false, false },
                {"satistan_iade_hesap_id", INTEGER, 0, false, 1, false, false },
                {"ortaklara_borclar_hesap_id", INTEGER, 0, false, 1, false, false },
                {"alis_iskontolari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"satis_iskontolari_hesap_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"sube_id"} }
            }
        },
        {
            "sub_uniteler",
            18,
            {
                {"unite_id", INTEGER, 0, false, 1, true, true },
                {"order_number", INTEGER, 0, false, 1, false, false },
                {"unite_adi", TEXT, 32, false, 1, false, false },
                {"sube_id", INTEGER, 0, false, 1, false, false },
                {"bilgisayar_id", INTEGER, 0, false, 1, false, false },
                {"muh_kasa_hesap_id", INTEGER, 0, false, 1, false, false },
                {"cari_hesap_id", INTEGER, 0, false, 1, false, false },
                {"irsaliye_belge_seri", TEXT, 3, false, 1, false, false },
                {"fatura_belge_seri", TEXT, 3, false, 1, false, false },
                {"tahsilat_makbuzu_belge_seri", TEXT, 3, false, 1, false, false },
                {"odeme_makbuzu_belge_seri", TEXT, 3, false, 1, false, false },
                {"depo_id", INTEGER, 0, false, 1, false, false },
                {"tahsilat_makbuzu_belge_num", TEXT, 15, false, 1, false, false },
                {"odeme_makbuzu_belge_num", TEXT, 15, false, 1, false, false },
                {"fatura_belge_numarasi", TEXT, 15, false, 1, false, false },
                {"irsaliye_belge_numarasi", TEXT, 15, false, 1, false, false },
                {"irsaliyeli_fat_belge_seri", TEXT, 3, false, 1, false, false },
                {"irsaliyeli_fat_belge_numarasi", TEXT, 15, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"unite_id"} }
            }
        }
    }
};
#endif // E9_DB_H 
