#ifndef E9_BELGE_H
#define E9_BELGE_H

#include "belge_struct.h"




//---------FATURA BELGESI ---------//


#define FATURA_HEADER_VARIABLE_COUNT 10
#define FATURA_LINE_VARIABLE_COUNT 18
#define FATURA_FOOTER_VARIABLE_COUNT 15

enum FATURA_HEADER_ENUM {
	FAT_ADR		 = 81,
	FAT_VERGI_DAIRE		 = 82,
	FAT_VERGI_NO		 = 83,
	FAT_SERISIRA		 = 84,
	FAT_TARIHI		 = 85,
	FAT_IRS_TARIHI		 = 86,
	FAT_IRSALIYE_NO		 = 87,
	FAT_DUZEN_TAR		 = 214,
	FAT_DUZEN_SAAT		 = 215,
	FAT_SEVK_ADR		 = 216
};

enum FATURA_LINE_ENUM {
	FAT_URUN_KODU		 = 88,
	FAT_URUN_CINSI		 = 89,
	FAT_URUN_BIRIMI		 = 90,
	FAT_URUN_MIKTARI		 = 91,
	FAT_IS_ON_BIRIM_FIYAT		 = 92,
	FAT_IS_SON_BIRIM_FIYAT		 = 218,
	FAT_SATIR_IS_YUZDESI		 = 219,
	FAT_SATIR_IS_TUTARI		 = 220,
	FAT_SATIR_TUTARI		 = 221,
	FAT_SATIR_DOVIZ_KODU		 = 222,
	FAT_SATIR_KDV_DAHIL_MI		 = 223,
	FAT_SATIR_VERGI_HRC_TUTAR		 = 224,
	FAT_SATIR_OTV_TUTARI		 = 225,
	FAT_SATIR_OTV_DAHIL_TUTAR		 = 226,
	FAT_SATIR_KDV_TUTARI		 = 227,
	FAT_SATIR_KDV_DAHIL_TUTAR		 = 228,
	FAT_SATIR_KDV_ORANI		 = 229,
	FAT_SATIR_OTV_ORANI		 = 230
};

enum FATURA_FOOTER_ENUM {
	FAT_BRUT_TUTAR		 = 96,
	FAT_ISKONTO_YUZDESI		 = 97,
	FAT_ARA_TOPLAM		 = 98,
	FAT_TUTARI_YAZIYLA		 = 99,
	FAT_TUTARI_RAKAMLA		 = 100,
	FAT_SON_CAR_BAK		 = 102,
	FAT_KDV_1		 = 173,
	FAT_KDV_2		 = 174,
	FAT_KDV_3		 = 175,
	FAT_KDV_4		 = 176,
	FAT_KDV_5		 = 177,
	FAT_ISKONTO_TUTARI		 = 231,
	FAT_OTV_TUTARI		 = 232,
	FAT_OTV_DAHIL_TUTAR		 = 233,
	FAT_KDV_TUTARI		 = 234
};

static ADAK_BELGE_DEFAULTS_STRUCT FATURA_header[10] = {
	 { 81,"fat_adr","Fatura Adresi",13,10,true,0,0,0,60 },{ 82,"fat_vergi_daire","Vergi Dairesi",13,10,true,0,0,0,30 },{ 83,"fat_vergi_no","Vergi Numarası",13,10,true,0,0,0,10 },{ 84,"fat_serisira","Seri Sıra Numarası",13,10,true,0,0,0,3 },{ 85,"fat_tarihi","Fatura Tarihi",13,10,true,0,0,0,15 },{ 86,"fat_irs_tarihi","İrsaliye Tarihi",13,10,true,0,0,0,15 },{ 87,"fat_irsaliye_no","İrsaliye No",13,10,true,0,0,0,15 },{ 214,"fat_duzen_tar","Düzenlenme Tarihi",13,10,true,0,0,0,15 },{ 215,"fat_duzen_saat","Düzenlenme Saati",13,10,true,0,0,0,5 },{ 216,"fat_sevk_adr","Sevk Adresi",13,10,true,0,0,0,60 }
};

static ADAK_BELGE_DEFAULTS_STRUCT FATURA_line[18] = {
	 { 88,"fat_urun_kodu","Ürün Kodu",13,30,true,0,0,0,10 },{ 89,"fat_urun_cinsi","Ürün Cinsi ve Açıklama",13,10,true,0,0,0,60 },{ 90,"fat_urun_birimi","Ürün birimi",13,20,true,0,0,0,10 },{ 91,"fat_urun_miktari","Ürün miktarı",13,20,true,0,0,0,10 },{ 92,"fat_is_on_birim_fiyat","İskonto öncesi birim fiyat",13,20,true,0,0,0,15 },{ 218,"fat_is_son_birim_fiyat","İskonto sonrası birim fiyat",13,20,true,0,0,0,15 },{ 219,"fat_satir_is_yuzdesi","Satır iskonto yüzdesi",13,20,true,0,0,0,5 },{ 220,"fat_satir_is_tutari","Satır iskonto tutarı",13,20,true,0,0,0,15 },{ 221,"fat_satir_tutari","Satır tutarı",13,20,true,0,0,0,15 },{ 222,"fat_satir_doviz_kodu","Satır döviz kodu",13,30,true,0,0,0,5 },{ 223,"fat_satir_kdv_dahil_mi","Satır KDV dahil mi",13,10,true,0,0,0,1 },{ 224,"fat_satir_vergi_hrc_tutar","Satır vergi hariç tutar",13,20,true,0,0,0,12 },{ 225,"fat_satir_otv_tutari","Satır ÖTV tutarı",13,20,true,0,0,0,12 },{ 226,"fat_satir_otv_dahil_tutar","Satır ÖTV dahil tutar",13,20,true,0,0,0,12 },{ 227,"fat_satir_kdv_tutari","Satır KDV tutarı",13,20,true,0,0,0,15 },{ 228,"fat_satir_kdv_dahil_tutar","Satır KDV dahil tutar",13,20,true,0,0,0,12 },{ 229,"fat_satir_kdv_orani","Satır KDV oranı",13,20,true,0,0,0,5 },{ 230,"fat_satir_otv_orani","Satır ÖTV oranı",13,20,true,0,0,0,12 }
};

static ADAK_BELGE_DEFAULTS_STRUCT FATURA_footer[15] = {
	 { 96,"fat_brut_tutar","Brüt Tutar",13,20,true,0,0,0,12 },{ 97,"fat_iskonto_yuzdesi","Toplam iskonto yüzdesi",13,20,true,0,0,0,12 },{ 98,"fat_ara_toplam","Ara Toplam",13,20,true,0,0,0,12 },{ 99,"fat_tutari_yaziyla","Fatura tutarı yazı İle",13,10,true,0,0,0,60 },{ 100,"fat_tutari_rakamla","Fatura tutarı rakam İle",13,10,true,0,0,0,12 },{ 102,"fat_son_car_bak","Son Cari Hesap Bakiyesi",13,20,true,0,0,0,15 },{ 173,"fat_kdv_1","KDV Oranı 1",13,20,true,0,0,0,12 },{ 174,"fat_kdv_2","KDV Oranı 2",13,20,true,0,0,0,12 },{ 175,"fat_kdv_3","KDV Oranı 3",13,20,true,0,0,0,12 },{ 176,"fat_kdv_4","KDV Oranı 4",13,20,true,0,0,0,12 },{ 177,"fat_kdv_5","KDV Oranı 5",13,20,true,0,0,0,12 },{ 231,"fat_iskonto_tutari","Toplam iskonto tutarı",13,20,true,0,0,0,12 },{ 232,"fat_otv_tutari","Fatura ÖTV tutarı",13,20,true,0,0,0,12 },{ 233,"fat_otv_dahil_tutar","Fatura ÖTV dahil tutar",13,20,true,0,0,0,12 },{ 234,"fat_kdv_tutari","Fatura KDV tutarı",13,20,true,0,0,0,12 }
};

//-------------------------------------------//







//---------SEVK_IRSALIYESI BELGESI ---------//


#define SEVK_IRSALIYESI_HEADER_VARIABLE_COUNT 11
#define SEVK_IRSALIYESI_LINE_VARIABLE_COUNT 19
#define SEVK_IRSALIYESI_FOOTER_VARIABLE_COUNT 15

enum SEVK_IRSALIYESI_HEADER_ENUM {
	IRS_SEVK_ADRESI		 = 181,
	IRS_VERGI_DAIRESI		 = 182,
	IRS_VERGI_NO		 = 183,
	IRS_SERI_SIRA_NO		 = 184,
	IRS_DUZENLEME_TARIHI		 = 185,
	IRS_DUZENLEME_SAATI		 = 186,
	IRS_TARIHI		 = 217,
	IRS_FAT_TARIHI		 = 237,
	IRS_FAT_NO		 = 238,
	IRS_FAT_FIILI_SEVK_TARIHI		 = 239,
	IRS_FATURA_ADRESI		 = 240
};

enum SEVK_IRSALIYESI_LINE_ENUM {
	IRS_URUN_KODU		 = 188,
	IRS_URUN_CINSI		 = 189,
	IRS_BIRIM		 = 190,
	IRS_MIKTAR		 = 191,
	IRS_SATIR_TUTAR		 = 192,
	IRS_KDV_ORANI		 = 193,
	IRS_ISKONTO		 = 194,
	IRS_SATIS_TUTARI		 = 195,
	IRS_IS_SON_BIRIM_FIYAT		 = 260,
	IRS_SATIR_DOVIZ_KODU		 = 261,
	IRS_SATIR_OTV_TUTARI		 = 262,
	IRS_SATIR_OTV_DAHIL_TUTAR		 = 263,
	IRS_SATIR_KDV_DAHIL_TUTAR		 = 264,
	IRS_SATIR_KDV_ORANI		 = 265,
	IRS_SATIR_VERGI_HRC_TUTAR		 = 266,
	IRS_SATIR_OTV_ORANI		 = 267,
	IRS_SATIR_KDV_DAHIL_MI		 = 268,
	IRS_IS_ON_BIRIM_FIYAT		 = 269,
	IRS_SATIR_KDV_TUTARI		 = 270
};

enum SEVK_IRSALIYESI_FOOTER_ENUM {
	IRS_BRUT_TUTAR		 = 196,
	IRS_ISKONTO_TUTARI		 = 197,
	IRS_ARA_TOPLAM		 = 198,
	IRS_TOPLAM_RAKAMLA		 = 199,
	IRS_TOPLAM_YAZIYLA		 = 200,
	IRS_SON_CAR_BAKIYE		 = 203,
	IRS_KDV_1		 = 204,
	IRS_KDV_2		 = 205,
	IRS_KDV_3		 = 206,
	IRS_KDV_4		 = 207,
	IRS_KDV_5		 = 208,
	IRS_OTV		 = 209,
	IRS_TESLIM_ALAN		 = 211,
	IRS_TESLIM_EDEN		 = 212,
	IRS_DIP_NOT		 = 213
};

static ADAK_BELGE_DEFAULTS_STRUCT SEVK_IRSALIYESI_header[11] = {
	 { 181,"irs_sevk_adresi","Sevk Adresi",13,10,true,0,0,0,512 },{ 182,"irs_vergi_dairesi","Vergi Dairesi",13,10,true,0,0,0,50 },{ 183,"irs_vergi_no","Vergi No",13,10,true,0,0,0,15 },{ 184,"irs_seri_sira_no","Seri Sıra No",13,10,true,0,0,0,15 },{ 185,"irs_duzenleme_tarihi","Düzenlenme Tarihi",13,10,true,0,0,0,10 },{ 186,"irs_duzenleme_saati","Düzenlenme Saati",13,10,true,0,0,0,5 },{ 217,"irs_tarihi","İrsaliye Tarihi",13,10,true,0,0,0,10 },{ 237,"irs_fat_tarihi","Fatura Tarihi",13,10,true,0,0,0,10 },{ 238,"irs_fat_no","Fatura No",13,10,true,0,0,0,10 },{ 239,"irs_fat_fiili_sevk_tarihi","Fiili Sevk Tarihi",13,10,true,0,0,0,10 },{ 240,"irs_fatura_adresi","Fatura Adresi",13,10,true,0,0,0,512 }
};

static ADAK_BELGE_DEFAULTS_STRUCT SEVK_IRSALIYESI_line[19] = {
	 { 188,"irs_urun_kodu","Ürün Kodu",13,30,true,0,0,0,30 },{ 189,"irs_urun_cinsi","Ürün Cinsi ve Açıklaması",13,10,true,0,0,0,50 },{ 190,"irs_birim","Birim",13,30,true,0,0,0,20 },{ 191,"irs_miktar","Miktar",13,20,true,0,0,0,3 },{ 192,"irs_satir_tutar","Satır Tutar",13,20,true,0,0,0,10 },{ 193,"irs_kdv_orani","KDV Oranı",13,20,true,0,0,0,5 },{ 194,"irs_iskonto","İskonto",13,20,true,0,0,0,12 },{ 195,"irs_satis_tutari","Satış Tutarı",13,20,true,0,0,0,12 },{ 260,"irs_is_son_birim_fiyat","İskonto Sonrası Birim Fiyat",13,10,true,0,0,1,15 },{ 261,"irs_satir_doviz_kodu","Satır döviz kodu",13,10,true,0,0,1,15 },{ 262,"irs_satir_otv_tutari","Satır ÖTV tutarı",13,10,true,0,0,1,15 },{ 263,"irs_satir_otv_dahil_tutar","Satır ÖTV dahil tutar",13,10,true,0,0,1,15 },{ 264,"irs_satir_kdv_dahil_tutar","Satır KDV dahil tutar",13,10,true,0,0,1,15 },{ 265,"irs_satir_kdv_orani","Satır KDV oranı",13,10,true,0,0,1,15 },{ 266,"irs_satir_vergi_hrc_tutar","Satır vergi hariç tutar",13,10,true,0,0,1,15 },{ 267,"irs_satir_otv_orani","Satır ÖTV oranı",13,10,true,0,0,1,15 },{ 268,"irs_satir_kdv_dahil_mi","Satır KDV dahil mi",13,10,true,0,0,1,15 },{ 269,"irs_is_on_birim_fiyat","İskonto öncesi birim fiyat",13,10,true,0,0,1,15 },{ 270,"irs_satir_kdv_tutari","Satır KDV tutarı",13,10,true,0,0,1,15 }
};

static ADAK_BELGE_DEFAULTS_STRUCT SEVK_IRSALIYESI_footer[15] = {
	 { 196,"irs_brut_tutar","Brüt Tutar",13,20,true,0,0,0,12 },{ 197,"irs_iskonto_tutari","İskonto Tutarı",13,20,true,0,0,0,12 },{ 198,"irs_ara_toplam","Ara Toplam",13,20,true,0,0,0,12 },{ 199,"irs_toplam_rakamla","Toplam ( Rakam ile )",13,20,true,0,0,0,12 },{ 200,"irs_toplam_yaziyla","Toplam ( Yazı İle )",13,10,true,0,0,0,60 },{ 203,"irs_son_car_bakiye","Son Cari Bakiye",13,20,true,0,0,0,15 },{ 204,"irs_kdv_1","KDV Oranı 1",13,20,true,0,0,0,12 },{ 205,"irs_kdv_2","KDV Oranı 2",13,20,true,0,0,0,12 },{ 206,"irs_kdv_3","KDV Oranı 3",13,20,true,0,0,0,12 },{ 207,"irs_kdv_4","KDV Oranı 4",13,20,true,0,0,0,12 },{ 208,"irs_kdv_5","KDV Oranı 5",13,20,true,0,0,0,12 },{ 209,"irs_otv","ÖTV",13,20,true,0,0,0,12 },{ 211,"irs_teslim_alan","Teslim Alan",13,10,true,0,0,0,60 },{ 212,"irs_teslim_eden","Teslim Eden",13,10,true,0,0,0,60 },{ 213,"irs_dip_not","Dip Not",13,10,true,0,0,0,50 }
};

//-------------------------------------------//







//---------SM_MAKBUZU BELGESI ---------//


#define SM_MAKBUZU_HEADER_VARIABLE_COUNT 6
#define SM_MAKBUZU_LINE_VARIABLE_COUNT 1
#define SM_MAKBUZU_FOOTER_VARIABLE_COUNT 6

enum SM_MAKBUZU_HEADER_ENUM {
	SMM_UNVAN		 = 241,
	SMM_SERI_SIRA		 = 242,
	SMM_TARIH		 = 243,
	SMM_ADRES		 = 244,
	SMM_VERGI_DAIRESI		 = 245,
	SMM_VERGI_NO		 = 246
};

enum SM_MAKBUZU_LINE_ENUM {
	SMM_YAPILAN_HIZMET		 = 259
};

enum SM_MAKBUZU_FOOTER_ENUM {
	SMM_BRUT_UCRET		 = 253,
	SMM_VERGI_STOPAJ		 = 254,
	SMM_NET_UCRET		 = 255,
	SMM_KDV		 = 256,
	SMM_HIZMET_TUTARI_Y		 = 257,
	SMM_HIZMET_TUTARI_R		 = 258
};

static ADAK_BELGE_DEFAULTS_STRUCT SM_MAKBUZU_header[6] = {
	 { 241,"smm_unvan","Ünvanı",13,10,true,0,0,0,60 },{ 242,"smm_seri_sira","Seri-Sıra No",13,10,true,0,0,0,10 },{ 243,"smm_tarih","Tarihi",13,10,true,0,0,0,10 },{ 244,"smm_adres","Adresi",13,10,true,0,0,0,512 },{ 245,"smm_vergi_dairesi","Vergi Dairesi",13,10,true,0,0,0,50 },{ 246,"smm_vergi_no","Vergi No",13,10,true,0,0,0,15 }
};

static ADAK_BELGE_DEFAULTS_STRUCT SM_MAKBUZU_line[1] = {
	 { 259,"smm_yapilan_hizmet","Yapılan Hizmet",13,10,true,0,0,0,128 }
};

static ADAK_BELGE_DEFAULTS_STRUCT SM_MAKBUZU_footer[6] = {
	 { 253,"smm_brut_ucret","Brüt Ücret",13,20,true,0,0,0,12 },{ 254,"smm_vergi_stopaj","Kesintiler Stopaj",13,20,true,0,0,0,12 },{ 255,"smm_net_ucret","Net Ücret",13,20,true,0,0,0,12 },{ 256,"smm_kdv","KDV",13,20,true,0,0,0,5 },{ 257,"smm_hizmet_tutari_y","Toplam Hizmet Tutarı ( Yazı )",13,10,true,0,0,0,12 },{ 258,"smm_hizmet_tutari_r","Toplam Hizmet Tutarı ( Rakam )",13,20,true,0,0,0,21 }
};

//-------------------------------------------//







//---------CARI_MAKBUZLAR BELGESI ---------//


#define CARI_MAKBUZLAR_HEADER_VARIABLE_COUNT 3
#define CARI_MAKBUZLAR_LINE_VARIABLE_COUNT 8
#define CARI_MAKBUZLAR_FOOTER_VARIABLE_COUNT 3

enum CARI_MAKBUZLAR_HEADER_ENUM {
	ISLEM_UNVAN		 = 71,
	ISLEM_TARIH		 = 72,
	ISLEM_SERI_SIRA		 = 73
};

enum CARI_MAKBUZLAR_LINE_ENUM {
	ISLEM_ODEME_SEKLI		 = 74,
	ISLEM_ACIKLAMA		 = 75,
	ISLEM_TUTAR		 = 76,
	ISLEM_PARA_BIRIMI		 = 77,
	ISLEM_BANKA		 = 271,
	ISLEM_SUBE		 = 272,
	ISLEM_CEK_NO		 = 273,
	ISLEM_CEK_TARIHI		 = 274
};

enum CARI_MAKBUZLAR_FOOTER_ENUM {
	ISLEM_TOP_TUTAR_Y		 = 78,
	ISLEM_TOP_TUTAR_R		 = 79,
	ISLEM_TAHSIL_EDEN		 = 80
};

static ADAK_BELGE_DEFAULTS_STRUCT CARI_MAKBUZLAR_header[3] = {
	 { 71,"islem_unvan","Ünvan",13,10,true,0,0,0,15 },{ 72,"islem_tarih","Tarih",13,10,true,0,0,0,10 },{ 73,"islem_seri_sira","Seri - Sıra No",13,10,true,0,0,0,15 }
};

static ADAK_BELGE_DEFAULTS_STRUCT CARI_MAKBUZLAR_line[8] = {
	 { 74,"islem_odeme_sekli","Ödeme Şekli",13,10,true,0,0,0,30 },{ 75,"islem_aciklama","Açıklama",13,10,true,0,0,0,128 },{ 76,"islem_tutar","Tutar",13,10,true,0,0,0,15 },{ 77,"islem_para_birimi","Para Birimi",13,20,true,0,0,0,12 },{ 271,"islem_banka","Banka",13,10,true,0,0,1,30 },{ 272,"islem_sube","Şube",13,10,true,0,0,1,40 },{ 273,"islem_cek_no","Çek No",13,10,true,0,0,1,15 },{ 274,"islem_cek_tarihi","Çek Tarihi",13,10,true,0,0,1,15 }
};

static ADAK_BELGE_DEFAULTS_STRUCT CARI_MAKBUZLAR_footer[3] = {
	 { 78,"islem_top_tutar_y","Toplam (Yazı ile)",13,10,true,0,0,0,12 },{ 79,"islem_top_tutar_r","Toplam (Rakam ile)",13,20,true,0,0,0,15 },{ 80,"islem_tahsil_eden","Tahsil Eden Kişi",13,10,true,0,0,0,60 }
};

//-------------------------------------------//





#define ADAK_BELGE_COUNT 4


enum E9_BELGE_ENUM {
	FATURA		 = 0,
	SEVK_IRSALIYESI		 = 1,
	SM_MAKBUZU		 = 2,
	CARI_MAKBUZLAR		 = 3
};

static ADAK_BELGELER_STRUCT e9_belge_struct[ADAK_BELGE_COUNT] = {
	{ 5, "FATURA", FATURA_HEADER_VARIABLE_COUNT, FATURA_FOOTER_VARIABLE_COUNT, FATURA_LINE_VARIABLE_COUNT,FATURA_header,FATURA_footer,FATURA_line },
	{ 9, "SEVK_IRSALIYESI", SEVK_IRSALIYESI_HEADER_VARIABLE_COUNT, SEVK_IRSALIYESI_FOOTER_VARIABLE_COUNT, SEVK_IRSALIYESI_LINE_VARIABLE_COUNT,SEVK_IRSALIYESI_header,SEVK_IRSALIYESI_footer,SEVK_IRSALIYESI_line },
	{ 13, "SM_MAKBUZU", SM_MAKBUZU_HEADER_VARIABLE_COUNT, SM_MAKBUZU_FOOTER_VARIABLE_COUNT, SM_MAKBUZU_LINE_VARIABLE_COUNT,SM_MAKBUZU_header,SM_MAKBUZU_footer,SM_MAKBUZU_line },
	{ 3, "CARI_MAKBUZLAR", CARI_MAKBUZLAR_HEADER_VARIABLE_COUNT, CARI_MAKBUZLAR_FOOTER_VARIABLE_COUNT, CARI_MAKBUZLAR_LINE_VARIABLE_COUNT,CARI_MAKBUZLAR_header,CARI_MAKBUZLAR_footer,CARI_MAKBUZLAR_line } 
};

#endif // E9_BELGE_H
