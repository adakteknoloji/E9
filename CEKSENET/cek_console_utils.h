#ifndef CEK_CONSOLE_UTILS_H
#define CEK_CONSOLE_UTILS_H

class ADAK_SQL;
class QString;
class QDate;

int         CEK_SIRADAKI_BORDRO_NO_AL                           ( QString   p_bordro_tarihi , ADAK_SQL * P_GELEN_DB = NULL);

int         CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI              ( QDate     vade_tarihi );

int         CEK_ORTALAMA_GUN_SAYISI                             ( int       fis_id, int fis_turu,   int  cek_veya_senet = -1 );

void        CEK_SENET_HAREKETINI_KAYDET                         ( int cek_id, int islem_turu, QDate islem_tarihi  , ADAK_SQL * P_GELEN_DB = NULL);
void        CEK_SON_CEK_SENET_HAREKETINI_SIL                    ( int cek_id  );
int         CEK_SON_CEK_SENET_HAREKETINI_BUL                    ( int cek_id  );
int         CEK_SONDAN_BIR_ONCEKI_CEK_SENET_HAREKETINI_BUL      ( int cek_senet_id );

void        CEK_SENET_DURUMUNU_GUNCELLE                         ( int cek_id, int cekin_durumu, ADAK_SQL * P_GELEN_DB = NULL  );

int         CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_ENUM            ( QString   cek_senet_bordro_islem_turu );
QString     CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING          ( int       cek_senet_bordro_islem_turu );

int         CEK_GET_CEK_SENET_POZISYONU_ENUM                    ( QString   cek_senet_pozisyonu);
QString     CEK_GET_CEK_SENET_POZISYONU_STRING                  ( int       cek_senet_pozisyonu);

int         CEK_GET_CEK_SENET_TURU_ENUM                         ( QString cek_senet_turu );

int         CEK_ISLEM_TURUNE_GORE_CEKIN_DURUMUNU_AL             ( int bordro_islem_turu );

int         CEK_GET_CEK_SENET_ACILIS_BORDROSU_ISLEM_TURU_ENUM   ( QString p_acilis_brd_islem_turu_string    );
QString     CEK_GET_CEK_SENET_ACILIS_BORDROSU_ISLEM_TURU_STRING ( int     p_acilis_brd_islem_turu_enum      );

int         CEK_GET_CEK_SENET_ENT_BORDROSU_ISLEM_TURU_ENUM      ( QString p_ent_brd_islem_turu_string       );
QString     CEK_GET_CEK_SENET_ENT_BORDROSU_ISLEM_TURU_STRING    ( int islem_turu , int cek_senet_mi , int musteri_firma_mi );

int         CEK_GET_CEK_BORDRO_TURU                             ( int bordro_id );

int         CEK_GET_BORDRO_ID                                   ( int p_program_id, int p_modul_id, int p_base_fis_id, int p_base_record_id );

void        CEK_UPDATE_CEK_TUTARI                               ( int p_cek_senet_id , double p_cek_senet_tutari );

double      CEK_GET_CEK_SENET_TUTARI                            ( int cek_senet_id,int mus_ceki_mi ,int cek_senet_durumu , int cek_senet_turu);

int         CEK_GET_CEK_SENET_ID                                ( int portfoy_numarasi);

bool        CEK_CARI_CEK_SENET_ALINABILIR_MI                    ( int p_cek_senet_id, int p_cari_hesap_id );

#endif // CEK_CONSOLE_UTILS_H
