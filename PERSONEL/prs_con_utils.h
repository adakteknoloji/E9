#ifndef PRS_CON_UTILS_H
#define PRS_CON_UTILS_H

#include <QList>

struct PRS_CALISMA_SURESI;
struct PRS_ODENEK_STRUCT;
struct PRS_KESINTI_STRUCT;
struct PRS_BORDRO_STRUCT;
struct PRS_PERSONEL_STRUCT;
struct PRS_KIDEM_IHBAR_ENT_STRUCT;

class ADAK_SQL;
class QDate;
class QComboBox;
class QString;


int             PRS_CREATE_SABIT_DEGERLER                       ();

void            PRS_CREATE_FIRMA_ODENEKLERI                     ();

void            PRS_SET_SABIT_DEGERLER_DEFAULTS                 ();

void            PRS_SET_SABIT_DEGERLER_DEFAULTS                 ();

int             PRS_FIRMA_SENDIKALI_MI                          ();

int             PRS_PERSONEL_SENDIKALI_MI                       ( int p_personel_id );

void            PRS_FILL_SABIT_DEGERLER_STRUCT                  ();

void            PRS_FILL_FIRMA_ODENEKLERI_STRUCT                ();

int             PRS_PERSONEL_ICIN_KAYITLI_BORDRO_VAR_MI         ( int personel_id );


int             PRS_VERILEN_DONEM_BORDROSU_KESILMIS_MI          ( int ay, int yil, int p_sube_id );


int             PRS_BIR_ONCEKI_AYIN_BORDROSU_KESILMIS_MI        ( int ay, int yil );


int             PRS_KIDEM_TAZMINATI_ALMAYA_HAK_KAZANMIS_MI      ( PRS_CALISMA_SURESI calisma_suresi );

int             PRS_PERSONEL_ISTEN_AYRILDI_MI                   ( int personel_id );


int             PRS_PERSONEL_BORDRODA_MEVCUT_MU                 ( int bordro_id, int personel_id );


PRS_CALISMA_SURESI  PRS_CALISMA_SURESINI_HESAPLA                ( QDate ise_giris_tarihi, QDate isten_cikis_tarihi );

double          PRS_IHBAR_TAZMINATI_TUTARINI_HESAPLA            ( int personel_id, PRS_CALISMA_SURESI calisma_suresi,
                                                                 double * brut_ihbar_tazminati_tutari, double * net_ihbar_tazminati_tutari,
                                                                 double * ihbar_tazminati_gelir_vergisi_tutari, double * ihbar_tazminati_damga_vergisi_tutari );
double          PRS_KIDEM_TAZMINATI_TUTARINI_HESAPLA            ( int personel_id, PRS_CALISMA_SURESI calisma_suresi,
                                                                 double * brut_kidem_tazminati_tutari, double * net_kidem_tazminati_tutari,
                                                                 double * kidem_tazminati_damga_vergisi_tutari );


double          PRS_ASGARI_GECIM_INDIRIMI_HESAPLA               ( int personel_id );


double *        PRS_GELIR_VERGISI_TUTARINI_HESAPLA              ( int personel_id, double gelir_vergi_matrahi, double *kumulatif_vergi_matrahi );

double          PRS_DAMGA_VERGISI_TUTARINI_HESAPLA              ( double brut_ucret );


double          PRS_FARKLI_ZAMAN_TURLERINI_HAFTAYA_DONUSTUR     ( int zaman_turu, double orj_deger );

double          PRS_GET_PERSONEL_KUMULATIF_VERGI_MATRAHI     ( int personel_id );
double          PRS_GET_PERSONEL_BRUT_UCRET                  ( int personel_id );
double          PRS_GET_PERSONEL_NET_UCRET                   ( int personel_id,int bordro_id );
QString         PRS_GET_BORDRO_KESIM_TARIHI                  ( int bordro_id );
QDate           PRS_GET_PERSONEL_ISE_GIRIS_TARIHI            ( int personel_id );
QString         PRS_GET_PERSONEL_CEP_TELEFONU                ( int personel_id );
QString         PRS_GET_PERSONEL_SICIL_NO                    ( int personel_id );
int             PRS_GET_PERSONEL_ID                          ( QString sicil_no );
int             PRS_GET_ISTEN_AYRILAN_ID                     ( int personel_id );
int             PRS_GET_BORDRO_ID                            ( int ay, int yil, int p_sube_id );
int             PRS_GET_OZEL_DURUM_TURU_ENUM                      ( int personel_id );
double          PRS_GET_OZEL_DURUM_INDIRIM_TUTARI            ( int ozel_durum );
QString         PRS_GET_FIRMA_SIGORTA_TURU                   ();
int             PRS_DAYS_IN_MONTH                            ( int month, int year);
int             PRS_GET_CALISILAN_GUN_SAYISI                 ( int personel_id, int bordro_donemi_ay, int bordro_donemi_yil) ;
int             PRS_GET_DOVIZ_ID                             ( int personel_id) ;
double          PRS_GET_SENDIKA_UCRETI                       ( int p_personel_id);
double          PRS_GET_EK_ODENEK_UCRETI                     (int personel_id);
QString         PRS_ADRESI_OLUSTUR                           ( QString mahalle,  QString cadde, QString sokak, QString bina_no, QString kat,
                                                           QString daire_no, QString semt,  QString ilce,  QString sehir,   QString posta_kodu );


void            PRS_ODENEK_STRUCTINI_KOPYALA                 ( PRS_ODENEK_STRUCT * ODENEK_HEDEF, PRS_ODENEK_STRUCT * ODENEK_KAYNAK );

void            PRS_KESINTI_STRUCTINI_KOPYALA                ( PRS_KESINTI_STRUCT * KESINTI_HEDEF, PRS_KESINTI_STRUCT * KESINTI_KAYNAK );

void            PRS_ODENEK_STRUCTINI_SIFIRLA                 (PRS_ODENEK_STRUCT * ODENEK);
void            PRS_KESINTI_STRUCTINI_SIFIRLA                (PRS_KESINTI_STRUCT * KESINTI);

// Bordro Structinden Turetilmis bir elemanin tum alanlarinin
// garbage degerlerden temizlenmesi ve hesaplamalrin dogru olabilmesi icin kullanilmalidir
void            PRS_BORDRO_STRUCTINI_SIFIRLA                 ( PRS_BORDRO_STRUCT * BORDRO_HESAP );

// Personel Strcutinden Turetilmis bir elemanin tum alanlarinin
// garbage degerlerden temizlenmesi ve hesaplamalrin dogru olabilmesi icin kullanilmalidir
void            PRS_PERSONEL_STRUCTINI_SIFIRLA               ( PRS_PERSONEL_STRUCT * PERSONEL_HESAP );

QString         PRS_GET_FIRMA_SIGORTA_KOLU_STRING       ( int frm_sigorta_kolu);
int             PRS_GET_FIRMA_SIGORTA_KOLU_INT          ( QString frm_sigorta_kolu);
int             PRS_GET_CARI_HESAP_ID                       ( int prs_id );

void            PRS_CLEAR_ODENEK_STRUCT                 ( PRS_ODENEK_STRUCT * ODENEK);
bool            PRS_PERSONEL_MAASI_BRUT_MU                  ( int prs_id );
void            PRS_UPDATE_PERSONEL_KUMULATIF_VERGI_MATRAHI ( int personel_id );
int             PRS_GET_PUANTAJ_GUN_SAYISI                  ( int personel_id , int p_bordro_donem_yili, int p_bordro_donem_ay );
double          PRS_GET_PUANTAJ_TEMEL_PARA_BIRIM_TUTARI     ( int personel_id, int p_yil, int p_ay );
int             PRS_COMBO_BOX_AY                            ();

void            PRS_ENTEGRASYON_FISLERINI_OLUSTUR       ( int p_bordro_id , int p_program_id , ADAK_SQL * P_GELEN_DB = NULL);

int             PRS_GET_PERSONEL_PUANTAJ_ID                      ( int p_personel_id, int p_bordro_yili, int p_bordro_ay );

void            PRS_GET_BORDRO_DONEMI_AY_YIL                ( int &bordro_donemi_ay , int &bordro_donemi_yil , int bordro_id , ADAK_SQL * P_GELEN_DB = NULL);

int             PRS_GET_MEDENI_HAL_ENUM                     ( QString medeni_hal );

bool            PRS_GET_PERSONEL_EMEKLI_MI                  ( int   p_personel_id );



QString         PRS_GET_MONTH_LAST_DATE                     ( int p_yil, int p_ay );

QString         PRS_GET_ODENEK_KESINTI_YARDIMLARI_STRING    ( int odenek_kesinti_yardimi_enum );
int             PRS_GET_ODENEK_KESINTI_YARDIMLARI_ENUM      ( QString odenek_kesinti_yardimi_str);

int             PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_ENUM      ( QString p_tutar_yuzde_turu_str);
QString         PRS_GET_PERSONEL_TUTAR_YUZDE_TURU_STRING    ( int     p_tutar_yuzde_turu_enum);

int             PRS_GET_AKRABALIK_DERECESI_ENUM             ( QString p_akrabalik_derecesi );
QString         PRS_GET_AKRABALIK_DERECESI_STRING           ( int     p_akrabalik_derecesi     );


int             PRS_GET_BORDRO_MUH_HESAPLARI_BIRLESTIR_ENUM ( QString p_durum );

int             PRS_GET_CALISMA_DURUMU_ENUM                 ( QString p_calisma_durumu);
QString         PRS_GET_CALISMA_DURUMU_STRING               ( int     p_calisma_durumu);
int             PRS_GET_EVET_HAYIR_DURUMU_ENUM              ( QString p_durum );
QString         PRS_GET_EVET_HAYIR_DURUMU_STRING            ( int     p_durum );

void            PRS_BORDRO_TOPLAMLARI_MUH_ENT               ( int p_bordo_id, int p_muh_fis_id, int p_sube_id, ADAK_SQL * P_GELEN_DB = NULL );

int             PRS_GET_OZEL_DURUM_ENUM                     ( QString p_ozel_durum);
QString         PRS_GET_OZEL_DURUM_STRING                   ( int     p_ozel_durum);

void            PRS_KIDEM_IHBAR_TAZMINATI_ENTEGRASTONU      ( PRS_KIDEM_IHBAR_ENT_STRUCT  * P_STRUCT , ADAK_SQL * P_GELEN_DB = NULL );

void            PRS_CLEAR_KIDEM_IHBAR_ENT_STRUCT            ( PRS_KIDEM_IHBAR_ENT_STRUCT * P_STRUCT );

int             PRS_GET_PERSONEL_CARI_HESAP_ID              ( int p_personel_id );

int             PRS_GET_CARI_PERSONEL_ID                    ( int p_cari_hesap_id );

double          PRS_GET_KUMULATIF_VERGI_ORANI               ( double p_kumulatif_vergi_tutari );

int             PRS_ISYERI_5510_KANUNA_TABI_MI              ();

void            PRS_GET_ONCEKI_AYLARDAN_KALAN_EK_ODENEKLER  ( int p_personel_id, double * p_bir_ay_once_kalan_ek_odenek, double *  iki_ay_once_kalan_ek_odenek);

int             PRS_GET_CALISMA_SEKLI_ENUM                  ( QString p_calisma_sekli );
QString         PRS_GET_CALISMA_SEKLI_STRING                ( int p_calisma_sekli );

int             PRS_SIRADAKI_ODENEK_KESINTI_NO_AL           ( QString p_fis_tarihi );

double          PRS_GET_AYLIK_YEMEK_VERILEN_GUN_SAYISI      ( int p_personel_id, int p_bordro_ay, int p_bordro_yil );

double          PRS_GET_EKSIK_GUN_SAYISI                    (int p_personel_id, int p_bordro_ay, int p_bordro_yil);

QString         PRS_GET_BORDRO_DONEMI_BAS_STRING            ( int p_yil, int p_ay );

QString         PRS_GET_BORDRO_DONEMI_BTS_STRING            ( int p_yil, int p_ay );

QList < int >   PRS_GET_GUNCEL_CALISAN_PERSONEL_IDLERI      ( int p_bordro_ay, int p_bordro_yil );

QString         PRS_GET_BORDRO_DONEMI                       ( int p_bordro_id );

#endif // PRS_CON_UTILS_H
