#ifndef FAT_FATURA_FISI_H
#define FAT_FATURA_FISI_H

#include <fis_kernel.h>

class  QString;
class  QStringList;
struct FATURA_FISI_STRUCT;
struct FATURA_FIS_SATIRI_STRUCT;
struct FATURA_DETAYLARI_STRUCT;

namespace Ui {
    class FAT_FATURA_FISI;
}

class FAT_FATURA_FISI : public FIS_KERNEL

{
    Q_OBJECT

public:

     FAT_FATURA_FISI                                                  ( int p_fatura_turu, int p_fis_id, int p_iade_faturasi_mi,
                                                                        QWidget * p_parent = 0 );
    ~FAT_FATURA_FISI                                                  (){}

private:

    Ui::FAT_FATURA_FISI *   m_ui;

    FATURA_FISI_STRUCT          *M_FIS;
    FATURA_FIS_SATIRI_STRUCT    *M_FIS_SATIRI;
    FATURA_DETAYLARI_STRUCT     *M_FATURA_DETAYLARI;

    int                     m_fis_id;
    int                     m_fatura_turu;
    bool                    m_log_kaydi_eklenecek_mi;
    bool                    m_kayit_eklendi;
    bool                    m_kayit_silindi;
    QString                 m_log_detaylari;
    int                     m_siradaki_belge_no;
    QString                 m_temel_para_birimi_kodu;
    int                     m_temel_para_birimi_id;

    QString                 m_fatura_turu_string;
    QString                 m_fatura_belge_seri;
    int                     m_last_urun_id;
    int                     m_fat_iade_edilecek_gun_sayisi;

    int                     m_last_fis_id;
    int                     m_ent_filtresi_turu;

    int                     m_depo_islem_yetkisi;
    int                     m_iade_faturasi_mi;

    double                 m_ontanimli_kdv_orani;

    QString             *   M_STOK_BIRIM_ADLARI;
    double              *   M_STOK_BIRIM_KATSAYILARI;
    int                 *   M_STOK_CAPRAZ_BIRIMLER;
    double              *   M_STOK_BIRIM_ALIS_FIYATLARI;
    double              *   M_STOK_BIRIM_SATIS_FIYATLARI;

    double                  m_irs_faturalastirma_str_max_urun_sayi;

    void                    SETUP_FORM                                          ();
    void                    CLEAR_FORM_MEMBERS                                  ();
    void                    NEW_FIS_RECORD                                      ();
    int                     GET_FIS_RECORD                                      ( int p_fis_id );

    int                     CHECK_FIS_FORM_VAR                                  ( QObject * p_object );
    int                     CHECK_FIS_FORM_EMPTY                                ();

    int                     CHECK_FIS_RECORD_ADD                                ();
    int                     ADD_FIS_RECORD                                      ();
    int                     CHECK_FIS_RECORD_UPDATE                             ( int p_fis_id );
    void                    UPDATE_FIS_RECORD                                   ( int p_fis_id );
    int                     CHECK_FIS_RECORD_DELETE                             ( int p_fis_id );
    void                    DELETE_FIS_RECORD                                   ( int p_fis_id );

    int                     SELECT_FIS_RECORD                                   ();
    int                     FIND_FIS_RECORD                                     ();
    int                     FIND_NEXT_FIS_RECORD                                ();
    int                     FIND_PREV_FIS_RECORD                                ();
    int                     FIND_FIRST_FIS_RECORD                               ();
    int                     FIND_LAST_FIS_RECORD                                ();

    int                     LOCK_FIS_RECORD                                     ( int p_fis_id );
    void                    UNLOCK_FIS_RECORD                                   ( int p_fis_id );

    void                    SET_LINE_DEFAULTS                                   ( int p_row_number );
    int                     CHECK_LINE_VAR                                      ( int p_row_number, QObject * p_object );
    int                     CHECK_LINE_EMPTY                                    ( int p_row_number );
    int                     CHECK_ADD_LINE                                      ( int p_fis_id, int p_row_number );
    void                    ADD_LINE                                            ( int p_fis_id, int p_row_number );
    int                     CHECK_UPDATE_LINE                                   ( int p_fis_id, int p_row_number );
    void                    UPDATE_LINE                                         ( int p_fis_id, int p_row_number );
    int                     CHECK_DELETE_LINE                                   ( int p_fis_id, int p_row_number );
    void                    DELETE_LINE                                         ( int p_fis_id, int p_row_number );

    void                    SELECTED_LINE                                       ( int p_row_number, int p_row_save_status);

    void                    SEARCH_EDIT_CLICKED                                 ( QWidget     * p_widget,
                                                                                QLineEdit   * p_line_edit );

    void                    LINE_SEARCH_EDIT_CLICKED                            ( int           p_row_number,
                                                                                QWidget     * p_widget,
                                                                                QLineEdit   * p_lineEdit );

    int                     CHANGER_BUTTON_CLICKED                              ( QAbstractButton * p_button );
    void                    SAVER_BUTTON_CLICKED                                ( QAbstractButton * p_button, int p_fis_id );

    int                     LEAVE_RECORD                                        ();

    int                     LINE_CHANGER_BUTTON_CLICKED                         ( QAbstractButton * p_button, int p_row_number );

    void                    URUN_BILGILERINI_AL                                 ( int p_row_number );
    void                    TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA               ( int p_row_number );
    void                    URUN_BIRIMI_FIYATLARINI_HESAPLA                     ( int p_row_number );

    void                    SATIR_DOVIZ_TUTARLARINI_YAZDIR                      ();

    void                    KDV_ORANLARI_BILGILERINI_YAZDIR                     ();
    void                    OTV_ORANLARI_BILGILERINI_YAZDIR                     ();

    bool                    FATURADA_DEGISIKLIK_YAPILABILIR_MI                  ( int p_fis_id, int p_delete_record_check = 0 );

    void                    FATURA_EKRANINI_DUZENLE                             ();


    void                    SECILI_IRSALIYENIN_SATIRLARINI_FATURADAN_SIL        ( int p_current_row );

    bool                    IADE_ISLEMI_YAPILMIS_SATIR_MI                       ( int p_fis_satiri_id );

    void                    TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA     ( int p_row_number );

    void                    KDV_ORANLARI_COMBOBOX_DOLDUR                        ( QComboBox * p_comboBox );
    void                    OTV_ORANLARI_COMBOBOX_DOLDUR                        ( QComboBox * p_comboBox );

    void                    FATURA_BAGLANTI_BILGISINI_GOSTER                    ( int p_fatura_turu, int p_fis_id );

    void                    CARI_BORC_BILGISINI_GOSTER                          ( int p_cari_hesap_id );

    void                    FAT_KUR_VE_PARITE_AYARLA                            ( bool p_degerleri_oto_ata );

    void                    FAT_FIS_TOPLAMLARINI_YENILE                         ( FATURA_FISI_STRUCT * P_FIS );

    int                     FATURA_SATIR_DEGERLERI_HESAPLAMASI                  ( int p_row_number, QObject * p_object );

    void                    SATIR_PARA_BIRIMINI_GUNCELLE                        ();

};

#endif // FAT_FATURA_FISI_H
