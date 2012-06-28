#ifndef E9_DEVIR_ISLEMI_BATCH_H
#define E9_DEVIR_ISLEMI_BATCH_H

#include "batch_kernel.h"
 



namespace Ui {
    class E9_DEVIR_ISLEMI_BATCH;
}

class E9_DEVIR_ISLEMI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::E9_DEVIR_ISLEMI_BATCH *   m_ui;

public:
                           E9_DEVIR_ISLEMI_BATCH                            (QWidget *parent = 0);
                           ~E9_DEVIR_ISLEMI_BATCH                           () {}
private:

    int                    m_muhasebe_yili;

    QDate                  m_arsiv_firma_kapanis_tarihi;
    QDate                  m_calisan_firma_acilis_tarihi;

    int                    m_calisan_firma_last_bnk_hareket_fis_no;
    int                    m_calisan_firma_last_cari_fis_no;
    int                    m_calisan_firma_last_muh_fis_no;
    int                    m_calisan_firma_last_stk_fis_no;
    int                    m_calisan_firma_last_cek_fis_no;

    void                   SETUP_FORM                                       ();

    int                    CHECK_VAR                                        (QObject * object);
    int                    CHECK_RUN                                        ();
    void                   RUN_BATCH                                        ();


    void                   BILGILENDIR                                      (QString message);

    bool                   VERITABANI_BOS_MU                                ( ADAK_SQL * P_DB );


    void                   ESKI_YILA_AIT_BILGILERI_ARSIV_FIRMAYA_KOPYALA    ();

    void                   CALISAN_FIRMANIN_MUHASEBE_YILINI_GUNCELLE        ();

    void                   CALISAN_FIRMADAN_ESKI_YILIN_FISLERINI_SIL        ();

    void                   CALISAN_FIRMANIN_FIS_NUMARALARINI_GUNCELLE       ();

    // ADRES MODULU //

    void                   ADRES_MODULUNU_DEVRET                            ();



    // BANKA MODULU //

    void                   BANKA_MODULUNU_DEVRET                            ();

    void                   BNK_FISLERINI_KOPYALA                            ();

    void                   BNK_ACILIS_FISI_OLUSTUR                          ();


    // CARI MODULU //

    void                   CARI_MODULUNU_DEVRET                             ();

    void                   CARI_FISLERI_KOPYALA                             ();

    void                   CARI_ACILIS_FISI_OLUSTUR                         ();

    //CEK SENET MODULU //

    void                   CEK_SENET_MODULUNU_DEVRET                        ();

    void                   CEKLER_TABLOSUNU_KOPYALA                         ();

    void                   CEK_BORDROLARINI_KOPYALA                         ();

    void                   CEK_SENET_ACILIS_BORDROSU_OLUSTUR                ();


    // MUHASEBE MODULU //

    void                   MUHASEBE_MODULUNU_DEVRET                         ();

    void                   MUHASEBE_FISLERINI_KOPYALA                       ();

    void                   MUH_ACILIS_FISI_OLUSTUR                          ();

    void                   MUH_KAPANIS_FISI_OLUSTUR                         ();

    // ISLETME MODULU //

    void                   ISLETME_MODULUNU_DEVRET                  ();

    void                   ISLETME_DEFTERI_FISLERINI_KOPYALA                ();

    // DOVIZ MODULU //

    void                   DOVIZ_MODULUNU_DEVRET                            ();

    //SUBE DEPO UNITE
    void                   SUBE_DEPO_UNITE_DEVRET                           ();


    //STOK

    void                   STOK_MODULUNU_DEVRET                             ();

    void                   STOK_FISLERINI_KOPYALA                           ();

    void                   STK_DEPODAKI_URUNLER_TABLOSUNU_KOPYALA           ();

    void                   STK_SATIRDAKI_SERI_NUMARALARI_TABLOSUNU_KOPYALA  ();

    void                   STK_URUNUN_PARTILERI_TABLOSUNU_KOPYALA           ();

    void                   STK_URUNUN_SERI_NUMARALARI_TABLOSUNU_KOPYALA     ();

    void                   IRS_IADE_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA      (int irs_satir_id);

    void                   STOK_ACILIS_FISI_OLUSTUR                         ();

    //FATURA MODULU  //

    void                   FATURA_MODULUNU_DEVRET                           ();

    void                   FATURALARI_KOPYALA                               ();

    void                   FAT_IRS_BAGLANTISI_TABLOSUNU_KOPYALA             (int fatura_id);

    void                   FAT_IRS_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA       (int fatura_satiri_id);

    void                   FAT_IADE_SATIR_BAGLANTISI_TABLOSUNU_KOPYALA      (int fatura_satiri_id);


    // PERAKENDE   //

    void                   PERAKENDE_MODULUNU_DEVRET                        ();

    void                   PERAKENDE_MODULU_FISLERINI_KOPYALA               ();

    //PERSONEL

    void                   PERSONEL_MODULUNU_DEVRET                         ();

    void                   PRS_PERSONELLER_TABLOSUNU_KOPYALA                ();

    void                   PRS_BORDROLAR_TABLOSUNU_KOPYALA                  ();

    //DEMIRBAS

    void                   DEMIRBAS_MODULUNU_DEVRET                         ();
    void                   DEMIRBAS_MODULU_FISLERINI_KOPYALA                ();

    //SMM

    void                   SMM_MODULUNU_DEVRET                               ();


    /////////////////////////////

    void                   CALISAN_FIRMADAN_ESKI_YILA_AIT_PRS_BORDROLARI_SIL ();

    int                    FIND_UYGUN_ACILIS_BORDROSU                       (int bordro_islem_turu , int cek_senet_turu,int hesap_no_id);
    bool                   CIRO_EDILEN_CEK_ODENDI_KABUL_EDILECEK_MI         (int cek_senet_id,QDate islem_tarihi);

    bool                   CEK_SENET_ARSIV_FIRMADA_ISLEM_GORDU_MU           (int cek_senet_id);

    bool                   CEK_SENET_CALISAN_FIRMADA_ISLEM_GORDU_MU         (int cek_senet_id);

    QStringList            GET_DEPOLAR_LIST                                 ();

    QStringList            GET_YILA_GORE_SATIRDAKI_SERI_NUMARALARI_ID_LIST  (QDate fis_tarihi);

    QStringList            GET_YILA_GORE_SATIRDAKI_PARTI_ID_LIST            (QDate fis_tarihi);

    void                   HESAP_BAKIYELERINI_SIFIRLA                       ( QString p_table_name , QString id_column_name , QStringList column_names ,  int values_size , ADAK_SQL * P_GELEN_DB);

    void                   SEARCH_EDIT_CLICKED                              ( QWidget *widget, QLineEdit *line_edit);

};

#endif // E9_DEVIR_ISLEMI_BATCH_H













