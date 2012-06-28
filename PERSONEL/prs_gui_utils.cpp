#include <QComboBox>
#include <QSearchEdit.h>
#include "prs_gui_utils.h"
#include "adak_utils.h"
#include "adak_sql.h"
#include "prs_enum.h"
#include "prs_struct.h"
#include "cari_enum.h"
#include "e9_console_utils.h"
#include "sube_console_utils.h"
#include "cari_struct.h"
#include "cari_console_utils.h"

extern ADAK_SQL     *       DB;
extern PRS_SABITLER          G_PRS_SABITLER;
extern PRS_SABITLER          G_PRS_DEFAULT_SABITLER;


/***************************************************************************************
                 PRS_FILL_COMBO_BOX_EKSIK_GUN_NEDENLERI
***************************************************************************************/

void   PRS_FILL_COMBO_BOX_EKSIK_GUN_NEDENLERI ( QComboBox * combo_box_eksik_gun_nedenleri )
{
    struct
    {
        int         neden_kodu;
        QString     neden_aciklamasi;
    }
    isten_cikma_nedenleri[ EKSIK_GUN_NEDENLERI_SAYISI ] = {
                {PRS_ISTIRAHAT,                         "01 - İstirahat"},
                {PRS_DISIPLIN_CEZASI,                   "03 - Disiplin Cezası" },
                {PRS_GOZALTINA_ALINMA,                  "04 - Gözaltına Alınma" },
                {PRS_TUTUKLULUK,                        "05 - Tutukluluk" },
                {PRS_KISMI_ISTIHDAM,                    "06 - Kısmi İstihdam" },
                {PRS_PUANTAJ_KAYITLARI,                 "07 - Puantaj Kayıtları" },
                {PRS_GREV,                              "08 - Grev" },
                {PRS_LOKAVT,                            "09 - Lokavt" },
                {PRS_GENEL_HAYATI_ETK_OLAYLAR,          "10 - Genel Hayatı Etkileyen Olaylar" },
                {PRS_DOGAL_AFET,                        "11 - Doğal Afet" },
                {PRS_BIRDEN_FAZLA,                      "12 - Birden Fazla" },
                {PRS_DIGER,                             "13 - Diğer" },
                {PRS_DEVAMSIZLIK,                       "15 - Devamsızlık" },
                {PRS_FESIH_TRHNDE_CALISMAMIS,           "16 - Fesih Tarihinde Çalışmamış" },
                {PRS_EV_HZMTLRNDE_30_GUNDEN_AZ_CALISMA, "17 - Ev Hizmetlerinde 30 günden az çalışma" },
                {PRS_KISA_CALISMA_ODENEGI,              "18 - Kısa Çalışma Ödeneği" } ,
                {PRS_UCRETSIZ_DOGUM_IZNI,               "19 - Ücretsiz Doğum İzni" } ,
                {PRS_UCRETSIZ_YOL_IZNI,                 "20 - Ücretsiz Yol İzni" } ,
                {PRS_DIGER_UCRETSIZ_IZIN,               "21 - Diğer Ücretsiz İzin" }

            };

    for ( int i=0; i< EKSIK_GUN_NEDENLERI_SAYISI; i++ ) {
        if ( isten_cikma_nedenleri[i].neden_aciklamasi NE "" ) {
            combo_box_eksik_gun_nedenleri->addItem( isten_cikma_nedenleri[i].neden_aciklamasi );
        }
    }
}

/***************************************************************************************
                 PRS_GET_EKSIK_GUN_NEDENLERI_ENUM
***************************************************************************************/

int PRS_GET_EKSIK_GUN_NEDENLERI_ENUM ( QString p_eksik_gun_nedenleri )
{
    if ( p_eksik_gun_nedenleri EQ QObject::tr("01 - İstirahat")) {
        return PRS_ISTIRAHAT;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("03 - Disiplin Cezası")) {
        return PRS_DISIPLIN_CEZASI;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("04 - Gözaltına Alınma")) {
        return PRS_GOZALTINA_ALINMA;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("05 - Tutukluluk")) {
        return PRS_TUTUKLULUK;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("06 - Kısmi İstihdam")) {
        return PRS_KISMI_ISTIHDAM;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("07 - Puantaj Kayıtları")) {
        return PRS_PUANTAJ_KAYITLARI;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("08 - Grev")) {
        return PRS_GREV;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("09 - Lokavt")) {
        return PRS_LOKAVT;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("10 - Genel Hayatı Etkileyen Olaylar")) {
        return PRS_GENEL_HAYATI_ETK_OLAYLAR;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("11 - Doğal Afet")) {
        return PRS_DOGAL_AFET;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("12 - Birden Fazla")) {
        return PRS_BIRDEN_FAZLA;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("13 - Diğer")) {
        return PRS_DIGER;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("15 - Devamsızlık")) {
        return PRS_DEVAMSIZLIK;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("16 - Fesih Tarihinde Çalışmamış")) {
        return PRS_FESIH_TRHNDE_CALISMAMIS;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("17 - Ev Hizmetlerinde 30 günden az çalışma")) {
        return PRS_EV_HZMTLRNDE_30_GUNDEN_AZ_CALISMA;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("18 - Kısa Çalışma Ödeneği")) {
        return PRS_UCRETSIZ_DOGUM_IZNI;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("19 - Ücretsiz Doğum İzni")) {
        return PRS_UCRETSIZ_YOL_IZNI;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("20 - Ücretsiz Yol İzni")) {
        return PRS_UCRETSIZ_YOL_IZNI;
    }
    else if ( p_eksik_gun_nedenleri EQ QObject::tr("21 - Diğer Ücretsiz İzin")) {
        return PRS_DIGER_UCRETSIZ_IZIN;
    }

    return -1;
}

/***************************************************************************************
                   PRS_FILL_COMBO_BOX_INDIRIM_KANUNLARI
****************************************************************************************/

void    PRS_FILL_COMBO_BOX_INDIRIM_KANUNLARI ( QComboBox * p_combo_box )
{
    struct {
        QString indirim_kodu;
        QString indirim_aciklamasi;
    }
    indirimler [ INDIRIM_KANUNU_SAYISI ] = {
        { "00000", "Sigortalı Herhangi Bir İndirim Kanununa Tabi Değil"},
        { "05084", "Sigortalı %100 Hazine İndirimine Tabi"},
        { "85084", "Sigortalı %80 Hazine İndirimi Tabi"},
        { "04857", "Sigortalı Sakatlık, Eski Hükümlü-Terör İndirimine Tabi"}
    };

    for ( int i=0; i< INDIRIM_KANUNU_SAYISI; i++ ) {

        if ( indirimler[i].indirim_kodu NE "" ) {
            p_combo_box->addItem( indirimler[i].indirim_kodu + " - " + indirimler[i].indirim_aciklamasi );
        }
    }

}

/***************************************************************************************
                   PRS_FILL_COMBO_BOX_INDIRIM_KANUNLARI
****************************************************************************************/

void    PRS_FILL_COMBO_BOX_CALISMA_TURLERI ( QComboBox * p_combo_box )
{
    struct {
        QString calisma_kodu;
        QString calisma_aciklamasi;
    }
    calisma_turleri [ BELGE_TURLERI_SAYISI ] = {
        { "01", "Tüm Sigorta Kollarına Tabi Çalışanlar"},
        { "02", "Sosyal Güvenlik Destek Primine tabi Çalışanlar ( Emekli )"},
        { "03", "(Deniz,Basım,Azot,Şeker) 2098 Nolu Kanuna Tabi Çalışanlar Yeraltı Sürekli Çalışanlar"},
        { "04", "Yeraltı Sürekli Çalışanlar"},
        { "05", "Yeraltı Gruplu Çalışanlar"},
        { "06", "Yerüstü Gruplu Çalışanlar"},
        { "07", "Çırak/Stajyer Öğrenci Olarak Çalışanlar"},
        { "09", "Yurtdışında Çalışıp, Topluluk Sigortasına Tabi Olanlar"},
        { "10", "İkili Sosyal Güvenlik Sözleşmesi Olmayan Ülke Vatandaşları"},
        { "11", "Yüksek Öğrenim Kurumlarında Kısmi İstihdam Edilen Öğrenciler"},
        { "13", "Tüm Sigorta Kollarına Tabi Olup, İşsizlik Sigorta Primi Kesilmeyenler"},
        { "14", "Libya’da Çalışanlar"},
        { "15", "Anlaşmalı Ülkelerde Çalışanlar, Yabancı Uyruklular"},
        { "16", "%22 Oranında Malüllük, Yaşlılık ve Ölüm Sigortalarına Tabi Olup, İşsizlik Sigorta Primi Kesilmeyenler"},
        { "17", "Görev Malüllüğü Aylığı Alıp, %22 Oranında Malüllük, Yaşlılık ve Ölüm Sigortalarına Tabi Olanlar"},
        { "18", "Görev Malüllüğü Aylığı Alıp, Malüllük, Yaşlılık ve Ölüm Sigortalarına Tabi Olanlar"},
        { "19", "İş Kazası,Meslek Hastalığı,Hastalık ve Analık Sigortalarına Tabi Çalışanlar"},
    };

    for ( int i=0; i< BELGE_TURLERI_SAYISI; i++ ) {

        if ( calisma_turleri[i].calisma_kodu NE "" ) {
            p_combo_box->addItem( calisma_turleri[i].calisma_kodu + " - " + calisma_turleri[i].calisma_aciklamasi );
        }
    }

}


/***************************************************************************************
                   PRS_FILL_COMBO_BOX_ISTEN_CIKMA_NEDENLERI
****************************************************************************************/

void   PRS_FILL_COMBO_BOX_ISTEN_CIKMA_NEDENLERI ( QComboBox * combo_box_isten_cikma_nedenleri )
{
    struct
    {
        int         neden_kodu;
        QString     neden_aciklamasi;
    }
    eksik_gun_nedenleri[ ISTEN_CIKMA_NEDENLERI_SAYISI ] ={
                {PRS_DENEME_SURELI_IS_SOZLESMESININ_ISVERENCE_FESHI,
                "Deneme süreli iş sözleşmesinin işverence feshi" },
                {PRS_DENEME_SURELI_IS_SOZLESMESININ_ISCI_TARAFINDAN_FESHI,
                "Deneme süreli iş sözleşmesinin işçi tarafından feshi" },
                {PRS_BELIRSIZ_SURELI_IS_SOZLESMESININ_ISCI_TARAFINDAN_FESHI,
                "Belirsiz süreli iş sözleşmesinin işçi tarafından feshi (istifa)" },
                {PRS_BELIRSIZ_SURELI_IS_SOZLESMESININ_ISVERENCE_FESHI,
                "Belirsiz süreli iş sözleşmesinin işveren tarafından haklı sebep bildirilmeden feshi" },
                {PRS_BELIRLI_SURELI_IS_SOZLESMESININ_SONA_ERMESI,
                "Belirli süreli iş sözleşmesinin sona ermesi" },
                {PRS_EMEKLILIK_VEYA_TOPTAN_ODEME,
                "Emeklilik (yaşlılık) veya toptan ödeme" },
                {PRS_MALULEN_EMEKLILIK,
                "Malulen emeklilik" },
                {PRS_OLUM,
                "Ölüm" },
                {PRS_IS_KAZASI_SONUCU_OLUM,
                "İş kazası sonucu ölüm" },
                {PRS_ASKERLK,
                "Askerlik" },
                {PRS_KADIN_ISCININ_EVLENMESI,
                "Kadın işçinin evlenmesi" },
                {PRS_EMEKLILIK_ICIN_YAS_DISINDA_DGR_SARTLARIN_TAMAMLANMASI,
                "Emeklilik için yaş dışında diğer şartların tamamlanması" },
                {PRS_TOPLU_ISCI_CIKARMA,
                "Toplu işçi çıkarma" },
                {PRS_SOZLESME_SONA_ERMEDEN_AYNI_ISVEREN_AIT_DIGER_ISYERINE_NAKIL,
                "Sözleşme sona ermeden sigortalının aynı işverene ait diğer işyerine nakli" },
                {PRS_ISYERININ_KAPANMASI,
                "İşyerinin kapanması" },
                {PRS_ISIN_SONA_ERMESI,
                "İşin sona ermesi" },
                {PRS_MEVSIM_BITIMI,
                "Mevsim bitimi"},
                {PRS_KAMPANYA_BITIMI,
                "Kampanya bitimi" },
                {PRS_STATU_DEGISIKLIGI,
                "Statü değişikliği" },
                {PRS_DIGER_NEDENLER,
                "Diğer nedenler" },
                {PRS_ISCI_TARAFINDAN_ZORUNLU_NEDENLE_FESIH,
                "İşçi tarafından zorunlu nedenle fesih" },
                {PRS_ISCI_TARAFINDAN_SAGLIK_NEDENIYLE_FESIH,
                "İşçi tarafından sağlık nedeniyle fesih" },
                {PRS_ISCI_TARAFINDAN_ISVERENIN_AHLAK_IYINIYET_KURALLARINA_UYMAMASI_NEDENIYLE_FESIH,
                "İşçi tarafından işverenin ahlak ve iyiniyet kurallarına aykırı davranışı nedeni ile fesih" },
                {PRS_DISIPLIN_KURULU_KARARI_ILE_FESIH,
                "Disiplin kurulu kararı ile fesih" },
                {PRS_ISVEREN_TARAFINDAN_ZORUNLU_NEDENLERLE_VE_TUTUKLULUK_NEDENIYLE_FESIH,
                "İşveren tarafından zorunlu nedenlerle ve tutukluluk nedeniyle fesih" },
                {PRS_ISVEREN_TARAFINDAN_SAGLIK_NEDENIYLE_FESIH,
                "İşveren tarafından sağlık nedeni ile fesih" },
                {PRS_ISVEREN_TARAFINDAN_ISCININ_AHLAK_IYINIYET_KURALLARINA_UYMAMASI_NEDENIYLE_FESIH,
                "İşveren tarafından işçinin ahlak ve iyiniyet kurallarına aykırı davranışı nedeni ile fesih" },
                {PRS_VIZE_SURESININ_BITIMI,
                "Vize süresinin bitimi" },
                {PRS_BORCLAR_KANUNU_SENDIKALAR_KANUNU_GREV_LOKAVT_KANUNU_KAPSAMINDA_KENDI_ISTEK_KUSURU_DISINDA_FESIH,
                "Borçlar Kanunu, Sendikalar Kanunu, Grev ve Lokavt Kanunu kapsamında kendi istek ve kusuru dışında fesih" },
                {PRS_OZELLESTIRME_NEDENI_ILE_4096_SAYILI_KANUNUN_21_MADDESINE_GORE_FESIH,
                "4046 sayılı Kanunun 21. maddesine göre özelleştirme nedeni ile feshi" },
                {PRS_GAZETECI_TARAFINDAN_SOZLESMENIN_FESHI,
                "Gazeteci tarafından sözleşmenin feshi" },
                {PRS_ISYERININ_DEVRI_ISIN_VEYA_ISYERININ_NITELIGININ_DEGISMESI_NEDENIYLE_FESIH,
                "İşyerinin devri, işin veya işyerinin niteliğinin değişmesi nedeniyle fesih" }};

    combo_box_isten_cikma_nedenleri->addItem( "..." );
    for ( int i=0; i< ISTEN_CIKMA_NEDENLERI_SAYISI; i++ ) {

        if ( eksik_gun_nedenleri[i].neden_aciklamasi NE "" ) {
            combo_box_isten_cikma_nedenleri->addItem( QVariant(eksik_gun_nedenleri[i].neden_kodu).toString()
                                + " - " + eksik_gun_nedenleri[i].neden_aciklamasi );
        }
    }
}

/***************************************************************************************
                   PRS_FILL_COMBO_BOX_OZEL_DURUMLAR
****************************************************************************************/

void   PRS_FILL_COMBO_BOX_OZEL_DURUMLAR ( QComboBox * combo_box_ozel_durumlar )
{

    QStringList ozel_durumlar;

    ozel_durumlar << QObject::tr("0 - Yok");
    ozel_durumlar << QObject::tr("1 - 1. Derece Özürlü");
    ozel_durumlar << QObject::tr("2 - 2. Derece Özürlü");
    ozel_durumlar << QObject::tr("3 - 3. Derece Özürlü");
    ozel_durumlar << QObject::tr("4 - Eski Hükümlü");
    ozel_durumlar << QObject::tr("5 - Terör Mağduru");

    combo_box_ozel_durumlar->addItems(ozel_durumlar);

}



/*********************************************************************************
              PRS_FILL_COMBO_BOX_SUBE_SECIMI
***********************************************************************************/
  void  PRS_FILL_COMBO_BOX_SUBE_SECIMI(QComboBox*combo_box_subeler)
  {
    QMap <QString, QString > sube_kodu_adi_map = SUBE_GET_SUBE_KODU_ADI_MAP();

    if ( sube_kodu_adi_map.size() EQ 0 ) {
        return;
    }
    for ( int i = 0; i < sube_kodu_adi_map.size(); i++ ) {
        combo_box_subeler->addItem ( sube_kodu_adi_map.keys().at(i) + "-" + sube_kodu_adi_map.values().at(i) );
    }
}



/***************************************************************************************
                   PRS_GET_SICILNO_AND_SHOW_INFO
****************************************************************************************/

int PRS_GET_SICILNO_AND_SHOW_INFO ( QSearchEdit *search_edit_sicil_no, QLineEdit *line_edit_personel_adi_soyadi )
{
    SQL_QUERY       sql_query( DB );
    int             personel_id;

    sql_query.PREPARE_SELECT( "prs_personeller","personel_id, cari_hesap_id ", "sicil_no = :sicil_no" );

    sql_query.SET_VALUE( ":sicil_no"  , search_edit_sicil_no->GET_TEXT()  );

    if ( sql_query.SELECT() EQ 0 ) {
        return -1;
    }

    sql_query.NEXT();

    personel_id   = sql_query.VALUE(0).toInt();

    line_edit_personel_adi_soyadi->clear();

    int cari_hesap_id =  sql_query.VALUE( 1 ).toInt();

    CARI_HESAP_STRUCT hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri );

    line_edit_personel_adi_soyadi->setText   ( hesap_bilgileri.cari_hesap_ismi );

    line_edit_personel_adi_soyadi->setText   ( hesap_bilgileri.cari_hesap_ismi );

    line_edit_personel_adi_soyadi->setReadOnly(true);

    return personel_id;
}

/***************************************************************************************
                   PRS_GET_PERSONELID_AND_SHOW_INFO
****************************************************************************************/

int PRS_GET_PERSONELID_AND_SHOW_INFO (int personel_id, QSearchEdit *search_edit_sicil_no,
                                         QLineEdit *line_edit_personel_adi )
{

    SQL_QUERY       sql_query( DB );
    int             p_sicil_no;


    sql_query.PREPARE_SELECT( "prs_personeller",
                              "cari_hesap_id, sicil_no ",
                              "personel_id = :personel_id" );

    sql_query.SET_VALUE( ":personel_id"  , personel_id  );

    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    int cari_hesap_id =  sql_query.VALUE( 0 ).toInt();

    CARI_HESAP_STRUCT hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri );

    line_edit_personel_adi->setText    ( hesap_bilgileri.cari_hesap_ismi );

    p_sicil_no = sql_query.VALUE(0).toInt();

    line_edit_personel_adi->setReadOnly(true);

    search_edit_sicil_no->SET_TEXT( sql_query.VALUE(1).toString() );

    return p_sicil_no;
}



/***************************************************************************************
                   PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT
***************************************************************************************/

void PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT ( QCommaEdit * comma_edit, double default_value,
                                        int sabit_deger_turu )
{
    QPalette highlight_palette;
    QPalette normal_palette;

    highlight_palette.setColor( highlight_palette.Base, Qt::yellow );

    switch( sabit_deger_turu ) {
        case PRS_TUTAR :
        {
            if ( comma_edit->GET_DOUBLE() NE default_value ) {

                comma_edit->setPalette( highlight_palette );
                comma_edit->setToolTip("<font color = darkred>""Sabit Değer SGK Değerinden Farklı");
            }
            else {
                comma_edit->setPalette( normal_palette );
            }
            break;
        }
        case PRS_YUZDE :
        {
            if ( (comma_edit->GET_DOUBLE() /100) NE default_value ) {

                comma_edit->setPalette( highlight_palette );
                comma_edit->setToolTip("<font color = darkred>""Sabit Değer SGK Değerinden Farklı");
            }
            else {
                comma_edit->setPalette( normal_palette );
            }
            break;
        }
        default :
        {
            return;
        }
    }
}





/***************************************************************************************
                   PRS_FILL_COMBO_BOX_MEDENI_HAL
 ***************************************************************************************/

void PRS_FILL_COMBO_BOX_MEDENI_HAL (QComboBox*p_combo_box_medeni_hal)

{
    p_combo_box_medeni_hal->clear();


     p_combo_box_medeni_hal->addItems  ( QStringList () << QObject::tr ( "Evli" )
                                                        << QObject::tr ( "Bekar" )
                                                        << QObject::tr ( "Dul" )
                                                        << QObject::tr ( "Boşanmış" ) );


}

/****************************************************************************************
                  PRS_FILL_COMBO_BOX_YABANCI_DILLER
  ***************************************************************************************/

void PRS_FILL_COMBO_BOX_YABANCI_DILLER (QComboBox*p_combo_box_yabanci_dil_1 ,QComboBox*p_combo_box_yabanci_dil_2)

{
   p_combo_box_yabanci_dil_1->clear();

   p_combo_box_yabanci_dil_2->clear();

   p_combo_box_yabanci_dil_1->addItems(QStringList() << QObject ::tr("Almanca")
                                                     <<QObject ::tr ("Aramca")
                                                     <<QObject ::tr("Azerice")
                                                     <<QObject ::tr ("Arapça")
                                                     <<QObject ::tr ("Bulgarca")
                                                     <<QObject ::tr ("Çekçe")
                                                     <<QObject ::tr ("Çince")
                                                     <<QObject ::tr ("Dzongkha")
                                                     <<QObject ::tr ("Estonca")
                                                     <<QObject ::tr ("Ermenice")
                                                     <<QObject ::tr ("Endonezya")
                                                     <<QObject ::tr ("Fransızca")
                                                     <<QObject ::tr ("Farsça")
                                                     <<QObject ::tr ("Felemenkçe")
                                                     <<QObject ::tr ("Fince")
                                                     <<QObject ::tr ("Gürcüce")
                                                     <<QObject ::tr ("Hindi")
                                                     <<QObject ::tr ("Hırvatça")
                                                     <<QObject ::tr ("İngilizce")
                                                     <<QObject ::tr ("Japonca")
                                                     <<QObject ::tr ("Kırgızca")
                                                     <<QObject ::tr ("Kurmanca")
                                                     <<QObject ::tr ("Latince")
                                                     <<QObject ::tr ("Litvanca")
                                                     <<QObject ::tr ("Makedonca")
                                                     <<QObject ::tr ("Malagasy")
                                                     <<QObject ::tr ("Malay")
                                                     <<QObject ::tr ("Nepal")
                                                     <<QObject ::tr ("Portekizce")
                                                     <<QObject ::tr ("Rusça")
                                                     <<QObject ::tr ("Romence")
                                                     <<QObject ::tr ("İngilizce")
                                                     <<QObject ::tr ("Slovakça")
                                                     <<QObject ::tr ("Thai")
                                                     <<QObject ::tr ("Tongan")
                                                     <<QObject ::tr ("Türkçe"));

   p_combo_box_yabanci_dil_2->addItems(QStringList() << QObject ::tr("Almanca")
                                                     <<QObject ::tr ("Aramca")
                                                     <<QObject ::tr("Azerice")
                                                     <<QObject ::tr ("Arapça")
                                                     <<QObject ::tr ("Bulgarca")
                                                     <<QObject ::tr ("Çekçe")
                                                     <<QObject ::tr ("Çince")
                                                     <<QObject ::tr ("Dzongkha")
                                                     <<QObject ::tr ("Estonca")
                                                     <<QObject ::tr ("Ermenice")
                                                     <<QObject ::tr ("Endonezya")
                                                     <<QObject ::tr ("Fransızca")
                                                     <<QObject ::tr ("Farsça")
                                                     <<QObject ::tr ("Felemenkçe")
                                                     <<QObject ::tr ("Fince")
                                                     <<QObject ::tr ("Gürcüce")
                                                     <<QObject ::tr ("Hindi")
                                                     <<QObject ::tr ("Hırvatça")
                                                     <<QObject ::tr ("İngilizce")
                                                     <<QObject ::tr ("Japonca")
                                                     <<QObject ::tr ("Kırgızca")
                                                     <<QObject ::tr ("Kurmanca")
                                                     <<QObject ::tr ("Latince")
                                                     <<QObject ::tr ("Litvanca")
                                                     <<QObject ::tr ("Makedonca")
                                                     <<QObject ::tr ("Malagasy")
                                                     <<QObject ::tr ("Malay")
                                                     <<QObject ::tr ("Nepal")
                                                     <<QObject ::tr ("Portekizce")
                                                     <<QObject ::tr ("Rusça")
                                                     <<QObject ::tr ("Romence")
                                                     <<QObject ::tr ("İngilizce")
                                                     <<QObject ::tr ("Slovakça")
                                                     <<QObject ::tr ("Thai")
                                                     <<QObject ::tr ("Tongan")
                                                     <<QObject ::tr ("Türkçe"));



}

/************************************************************************************
                             PRS_FILL_COMBO_BOX_SENDIKA
 ************************************************************************************/

void   PRS_FILL_COMBO_BOX_SENDIKA (QComboBox*p_combo_box_sendika_uyesi_mi)
{

      // p_combo_box_sendika_uyesi_mi->clear();
       p_combo_box_sendika_uyesi_mi->addItems(QStringList() << QObject ::tr("Evet")
                                                            <<QObject ::tr ("Hayır") );

}


/************************************************************************************
                             PRS_FILL_COMBO_BOX_CALISMA_SEKLI
 ************************************************************************************/
void PRS_FILL_COMBO_BOX_CALISMA_SEKLI (QComboBox*   p_combo_box_ucret_tipi )
{
    p_combo_box_ucret_tipi->addItems(QStringList()<<QObject ::tr("AYLIK")
                                     <<QObject::tr("GÜNLÜK"));
}


/**************************************************************************************
                   PRS_FILL_COMBO_BOX_UCRET_TIPI
 **************************************************************************************/
void PRS_FILL_COMBO_BOX_UCRET_TIPI (QComboBox*p_combo_box_ucret_sekli)
{
    p_combo_box_ucret_sekli->addItems(QStringList()<<QObject ::tr("BRÜT")
                                      <<QObject ::tr("NET"));

}
/***************************************************************************************
                   PRS_FILL_COMBO_BOX_BORDRO_YILLARI
 ***************************************************************************************/

void  PRS_FILL_COMBO_BOX_BORDRO_YILLARI  ( QComboBox * p_combo_box )
{
    // mali yil ilk tarih
    QDate borda_ilk_yil = MALI_YIL_FIRST_DATE();

    p_combo_box->addItem( QVariant ( borda_ilk_yil.year() ).toString() );

   // e9_array son tarih
   QDate bordro_son_yil = MALI_ARRAY_LAST_DATE();

   p_combo_box->addItem( QVariant ( bordro_son_yil.year() ).toString() );

}
/***************************************************************************************
                   PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI
***************************************************************************************/

void PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI(QComboBox *p_combobox_odenek_kesinti_yardimi)
{
    QStringList items;

    items << QObject::tr("Tüm Personele Verilir");
    items << QObject::tr("Yalnızca Belirli Personele Verilir");
    items << QObject::tr("Firmada Uygulanmaz");

    p_combobox_odenek_kesinti_yardimi->addItems(items);
    p_combobox_odenek_kesinti_yardimi->setCurrentIndex(-1);
}
