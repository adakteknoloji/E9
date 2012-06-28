#ifndef E9_UTILS_H
#define E9_UTILS_H

#include <QDate>
#include <QStringList>

class ADAK_SQL;

struct E9_SABIT_DEGERLER_STRUCT {
    int     acik_kapali_fatura;
};

struct E9_ENT_DETAYLARI_STRUCT;
struct E9_KULLANICI_YETKILERI_STRUCT;
struct SYSTEM_MENU_ITEMS;

void        E9_ADD_DEFAULT_BELGELER                        ();

QString     E9_GET_DELIMETER                               ();
void        E9_DELIMETER_UPDATE                            ( const QString p_old_delimeter,
                                                           const QString p_new_delimeter, QWidget * p_parent );
void        E9_CREATE_SABIT_DEGERLER                       ();
void        E9_SET_SABIT_DEGERLER_STRUCT                   ();

void        E9_SET_DEFAULT_VALUES                          ();

int         E9_GET_ACIK_KAPALI_FATURA_SABIT_DEGER           ();
void        E9_SET_ACIK_KAPALI_FATURA_SABIT_DEGER          ( int p_acik_kapali_fatura );

int         E9_GET_ADAK_MODULES_ENUM                      ( QString     p_adak_module_name );
QString     E9_GET_ADAK_MODULES_STRING                    ( int         p_adak_module_enum );

void        E9_KULLANICI_YETKILERI_STRUCT_DOLDUR           ( int kullanici_id       );
void        E9_CLEAR_KULLANICI_YETKILERI_STRUCT            ( E9_KULLANICI_YETKILERI_STRUCT * P_E9_KULLANICI_YETKILERI);

void        E9_SET_KDV_ORANLARI_OKUNDUMU_FLAG              ( bool p_flag_value );
void        E9_SET_OTV_ORANLARI_OKUNDUMU_FLAG              ( bool p_flag_value );

bool        E9_GET_KDV_ORANLARI_OKUNDUMU_FLAG              ();
bool        E9_GET_OTV_ORANLARI_OKUNDUMU_FLAG              ();

int         E9_GET_FIS_NO                                  ( int p_modul_id , int fis_id );

int         E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI         ();

int         E9_KULLANICININ_CALISTIGI_SIRKET               ();

int         E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI ();

int         E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI();

int         E9_KULLANICI_URUN_HAREKETLERINI_GOREBILIR_MI    ();

int         E9_KULLANICI_CARI_HAREKETLERINI_GOREBILIR_MI    ();

void        E9_CLEAR_ENT_DETAYLARI_STRUCT                  ( E9_ENT_DETAYLARI_STRUCT * P_ENT_DETAYLARI );

void        E9_UPDATE_TANIMLANMAMIS_ENT_HESAPLARI          ( ADAK_SQL * P_GELEN_DB = NULL);
void        E9_CHECK_AND_PREPARE                           ();

int         E9_GET_TAH_ODM_SEKLI_ENUM                      ( QString tah_odm_sekli );
QString     E9_GET_TAH_ODM_SEKLI_STRING                    ( int     tah_odm_sekli , int fis_turu = -1);

void        E9_DISABLE_MUHASEBE_ALAMLARI                   ();
void        E9_ENABLE_MUHASEBE_ALAMLARI                    ();
void        E9_RESTORE_MUHASEBE_ALAMLARI_VALUE             ();
bool        E9_MUHASEBE_ALARMLARI_KAPALIMI                 ();

void        E9_SUBE_ENT_HESAPLARINI_GUNCELLE               ( int p_sube_id ,ADAK_SQL * P_GELEN_DB = NULL);

void        E9_COPY_TABLES                                 ( ADAK_SQL * P_SOURCE_DB, ADAK_SQL * P_TARGET_DB,
                                                             QString p_table_name, QString p_where_condition  = NULL, QStringList p_where_set_values = QStringList() );
int         E9_GET_AYLAR_ENUM                              ( QString ay );
QString     E9_GET_AYLAR_STRING                            ( int ay );

void *      E9_GET_KULLANICI_YETKILERI                     ();

QString     E9_GET_SM_SICIL_NUMARASI                       ();

void        E9_GET_SISTEM_KULLANICI_YETKILERI              ( int p_kullanici_id, SYSTEM_MENU_ITEMS &SISTEM_STRUCT );

void        E9_SET_FIRMA_ISMI                              ( QString p_firma_ismi );

QString     E9_GET_FIRMA_ISMI                              ();

void        LOAD_E9_KOMPLE_PAKET                           ();


#endif // E9_UTILS_H
