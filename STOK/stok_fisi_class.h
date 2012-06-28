#ifndef STOK_FISI_H
#define STOK_FISI_H

#include <fis_kernel.h>
#include "stok_enum.h"

struct STOK_DETAYLAR_STRUCT;
struct STK_IRS_FIS_STRUCT;
struct STK_IRS_FIS_SATIR_STRUCT;
struct SUBE_ENTEGRASYON_HESAPLARI_STRUCT;

namespace Ui {
    class STOK_FISI;
}

class STOK_FISI : public FIS_KERNEL

{
    Q_OBJECT

public:

     STOK_FISI           ( int p_fis_id, int p_irsaliye_mi_stok_fisi_mi,
                           int p_fis_turu, int p_iade_irsaliyesi_mi, QWidget * p_parent = 0 );
    ~STOK_FISI           (){}

private:
    int                     m_ent_filtresi_turu;

private:

    Ui::STOK_FISI   *   m_ui;

    STOK_DETAYLAR_STRUCT * M_STOK_DETAYLARI;

    SUBE_ENTEGRASYON_HESAPLARI_STRUCT * M_SUBE_ENT_HESAPLARI;

    STK_IRS_FIS_STRUCT       * M_FIS;
    STK_IRS_FIS_SATIR_STRUCT * M_FIS_SATIRI;

    bool                    m_log_kaydi_eklenecek_mi;
    bool                    m_kayit_eklendi;
    bool                    m_kayit_silindi;
    QString                 m_log_detaylari;

    int                     m_fis_id;
    QString                 m_siradaki_belge_no;

    double                  m_ontanimli_kdv_orani;

    int                     m_last_urun_id;
    QString                 m_temel_para_birimi_kodu;
    int                     m_temel_para_birimi_id;

    QString             *   M_STOK_BIRIM_ADLARI;
    double              *   M_STOK_BIRIM_KATSAYILARI;
    int                 *   M_STOK_CAPRAZ_BIRIMLER;
    double              *   M_STOK_BIRIM_ALIS_FIYATLARI;
    double              *   M_STOK_BIRIM_SATIS_FIYATLARI;

    QStringList             M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI;
    QStringList             M_KDV_ORANLARINA_GORE_KDV_TUTARLARI;
    QStringList             M_OTV_ORANLARINA_GORE_OTV_TUTARLARI;

    int                     m_irs_iade_edilecek_gun_sayisi;
    QString                 m_irsaliye_belge_seri;

    int                     m_depo_islem_yetkisi;
    bool                    m_fis_tarihi_degistirildi_mi;
    bool                    m_iade_irsaliyesi_degistirildi_mi;

    void                    SETUP_FORM                                      ();
    void                    CLEAR_FORM_MEMBERS                              ();
    void                    NEW_FIS_RECORD                                  ();
    int                     GET_FIS_RECORD                                  ( int p_fis_id );

    int                     CHECK_FIS_FORM_VAR                              ( QObject * object );
    int                     CHECK_FIS_FORM_EMPTY                            ();

    int                     CHECK_FIS_RECORD_ADD                            ();
    int                     ADD_FIS_RECORD                                  ();
    int                     CHECK_FIS_RECORD_UPDATE                         ( int p_fis_id );
    void                    UPDATE_FIS_RECORD                               ( int p_fis_id );
    int                     CHECK_FIS_RECORD_DELETE                         ( int p_fis_id );
    void                    DELETE_FIS_RECORD                               ( int p_fis_id );

    int                     SELECT_FIS_RECORD                               ();
    int                     FIND_FIS_RECORD                                 ();
    int                     FIND_NEXT_FIS_RECORD                            ();
    int                     FIND_PREV_FIS_RECORD                            ();
    int                     FIND_FIRST_FIS_RECORD                           ();
    int                     FIND_LAST_FIS_RECORD                            ();

    int                     LOCK_FIS_RECORD                                 ( int p_fis_id );
    void                    UNLOCK_FIS_RECORD                               ( int p_fis_id );

    void                    SET_LINE_DEFAULTS                               ( int p_row_number );
    int                     CHECK_LINE_VAR                                  ( int p_row_number, QObject * p_object );
    int                     CHECK_LINE_EMPTY                                ( int p_row_number );
    int                     CHECK_ADD_LINE                                  ( int p_fis_id, int p_row_number );
    void                    ADD_LINE                                        ( int p_fis_id, int p_row_number );
    int                     CHECK_UPDATE_LINE                               ( int p_fis_id, int p_row_number );
    void                    UPDATE_LINE                                     ( int p_fis_id, int p_row_number );
    int                     CHECK_DELETE_LINE                               ( int p_fis_id, int p_row_number );
    void                    DELETE_LINE                                     ( int p_fis_id, int p_row_number );

    void                    FIS_EKRANINI_DUZENLE                            ();
    void                    URUN_BILGILERINI_AL                             ( int p_row_number );
    void                    TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA           ( int p_row_number );
    void                    URUN_BIRIMI_FIYATLARINI_HESAPLA                 ( int p_row_number );


    void                    SATIR_DOVIZ_TUTARLARINI_GUNCELLE                ( QString p_satir_doviz_kodu, double p_satir_doviz_tutari, int p_ekle_sil );
    void                    SATIR_DOVIZ_TUTARLARINI_YAZDIR                  ();

    void                    LINE_SEARCH_EDIT_CLICKED                        ( int p_row_number, QWidget * p_widget, QLineEdit * p_lineEdit );
    void                    SEARCH_EDIT_CLICKED                             ( QWidget * p_widget, QLineEdit * p_lineEdit );

    void                    SELECTED_LINE                                   ( int p_row_number, int p_row_save_status );

    int                     LEAVE_RECORD                                    ();

    void                    KDV_ORANLARI_BILGILERINI_GUNCELLE               ( QString p_satir_kdv_orani, double p_satir_kdv_tutari, int p_ekle_sil );
    void                    KDV_ORANLARI_BILGILERINI_YAZDIR                 ();
    void                    OTV_ORANLARI_BILGILERINI_GUNCELLE               ( QString p_satir_otv_orani, double p_satir_otv_tutari, int p_ekle_sil );
    void                    OTV_ORANLARI_BILGILERINI_YAZDIR                 ();

    void                    SIRADAKI_IRSALIYE_SERI_NUMARASINI_SETLE         ();

    void                    STOK_FISINE_YENI_SATIR_KAYDET                   ( STK_IRS_FIS_SATIR_STRUCT * P_IRSALIYE_SATIR_BILGILERI );
    void                    STOK_FIS_SATIR_BILGILERINI_GUNCELLE             ( int p_fis_id, int p_row_number );
    void                    STOK_FIS_SATIRINI_SIL                           ( int p_stok_fisi_id, int p_stok_fis_satiri_id );

    void                    SECILI_FATURANIN_SATIRLARINI_IRSALIYEDEN_SIL    ( int p_current_row );

    int                     LINE_CHANGER_BUTTON_CLICKED                     ( QAbstractButton * p_button, int p_row_number );

    void                    SAVER_BUTTON_CLICKED                            ( QAbstractButton * p_button, int p_record_id );

    int                     CHANGER_BUTTON_CLICKED                          ( QAbstractButton * p_button );

    bool                    STOK_FISINDE_DEGISIKLIK_YAPILABILIR_MI          ( int p_fis_id, int p_delete_record_check = 0 );

    bool                    IADE_ISLEMI_YAPILMIS_SATIR_MI                   ( int p_irsaliye_satiri_id );

    int                     IRSALIYE_ALIS_SATIS_TURUNU_BELIRLE              ();

    void                    ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA         ();

    void                    TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( int p_row_number );

    void                    IRS_BAGLANTI_BILGISINI_GOSTER                   ( int p_irs_turu, int p_fis_id );

    void                    IRS_KUR_VE_PARITE_AYARLA                        ( bool p_degerleri_oto_ata );

    void                    FIS_TOPLAMLARINI_HESAPLA                        ();

    void                    CARI_BORC_BILGISINI_GOSTER                      ( int p_cari_hesap_id );

    int                     IRSALIYE_SATIR_TUTARLARI_HESAPLA                ( int p_row_number, QObject * p_object );

    void                    SATIR_PARA_BIRIMINI_GUNCELLE                    ();

};

#endif // STOK_FISI_H
