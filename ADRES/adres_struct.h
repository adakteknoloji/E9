#ifndef ADRES_STRUCT_H
#define ADRES_STRUCT_H

class QString;
class QByteArray;

struct ADRES_HESAP_STRUCT {

    int     adres_kart_turu;
    int     firma_id;
    int     telefon_gizli_kayit_id;
    int     is_tel_gizli_kayit_id;
    int     cep_tel_gizli_kayit_id;
    int     fax_gizli_kayit_id;
    int     email_gizli_kayit_id;

    QString firma_sahis_adi;
    QString dogum_tarihi;
    QString dogum_yeri;
    QString cinsiyet;
    QString ogrenim_durumu;
    QString meslek;
    QString unvan;
    QString kimlik_no;
    QString kan_grubu;
    QString medeni_hali;
    QString vergi_dairesi;
    QString vergi_no;
    QString adres_satiri_1;
    QString adres_satiri_2;
    QString adres_satiri_3;
    QString ilce;
    QString semt;
    QString ulke;
    QString sehir;
    QString posta_kodu;
    QString tel_ulke_kodu;
    QString tel_alan_kodu;
    QString telefon;
    QString is_tel_ulke_kodu;
    QString is_tel_alan_kodu;
    QString is_telefonu;
    QString cep_tel_ulke_kodu;
    QString cep_tel_gsm_kodu;
    QString cep_telefonu;
    QString fax_ulke_kodu;
    QString fax_alan_kodu;
    QString fax;
    QString email;
    QString website;
    QString firma_sektoru;
    QString is_telefonu_dahili;

    QString yetkili_1;
    QString yetkili_1_info;
    QString yetkili_2;
    QString yetkili_2_info;
    QString yetkili_3;
    QString yetkili_3_info;

    QByteArray fotograf_logo;
    int     program_id;
    int     modul_id;
    int     base_record_id;
};

#endif // ADRES_STRUCT_H
