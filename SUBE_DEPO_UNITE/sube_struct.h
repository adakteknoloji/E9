#ifndef SUBE_STRUCT_H
#define SUBE_STRUCT_H

#include <QString>

struct SUBE_ENTEGRASYON_HESAPLARI_STRUCT {
    int port_cekler_hsp_id;
    int tah_verilen_cekler_hsp_id;
    int tem_verilen_cekler_hsp_id;
    int kar_cekler_hsp_id;
    int port_senetler_hsp_id;
    int tah_senetler_hsp_id;
    int tem_senetler_hsp_id;
    int pro_senetler_hsp_id;
    int port_senetler_ileri_trh_hsp_id;
    int tah_senetler_ileri_trh_hsp_id;
    int tem_senetler_ileri_trh_hsp_id;
    int borc_senetler_hsp_id;
    int borc_senetleri_ileri_trh_hsp_id;
    int sup_alacaklar_hsp_id;
    int tahsil_edilemeyen_cekler_hsp_id;
    int tahsil_edilemeyen_senetler_hsp_id;
    int unite_muh_kasa_hsp_id;
    int yurtici_sts_hsp_id;
    int hizmet_uretim_maliyeti_hsp_id;
    int imalattan_giris_hesap_id;
    int prs_giderler_hesap_id;
    int sarf_malzemeleri_hesap_id;
    int satistan_iade_hesap_id;
};

struct SUBE_UNITE_BILGILERI_STRUCT {
    QString unite_adi;
    int     sube_id;
    int     bilgisayar_id;
    int     muh_kasa_hesap_id;
    int     cari_hesap_id;
    QString irsaliye_belge_seri;
    QString fatura_belge_seri;
    QString tahsilat_makbuzu_belge_seri;
    QString odeme_makbuzu_belge_seri;
    int     depo_id;
    QString tahsilat_makbuzu_belge_num;
    QString odeme_makbuzu_belge_num;
    QString fatura_belge_numarasi;
    QString irsaliye_belge_numarasi;
    QString irsaliyeli_fat_belge_seri;
    QString irsaliyeli_fat_belge_numarasi;
};


#endif // SUBE_STRUCT_H
