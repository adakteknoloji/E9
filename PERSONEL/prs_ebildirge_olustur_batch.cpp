#include <QtXml>
#include <QFileDialog>
#include "prs_ebildirge_olustur_batch_class.h"
#include "prs_ebildirge_olustur_batch_open.h"
#include "ui_prs_ebildirge_olustur_batch.h"
#include "print.h"
#include "sube_console_utils.h"
#include "e9_gui_utils.h"
#include "cari_console_utils.h"
#include "PERSONEL/prs_gui_utils.h"
#include "adak_std_utils.h"
#include "prs_enum.h"
#include "cari_struct.h"
#include "e9_console_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_PRS_EBILDIRGE_OLUSTUR_BATCH
***************************************************************************************/

void OPEN_PRS_EBILDIRGE_OLUSTUR_BATCH ( QWidget * parent )
{
    PRS_EBILDIRGE_OLUSTUR_BATCH * F = new PRS_EBILDIRGE_OLUSTUR_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRS_EBILDIRGE_OLUSTUR_BATCH::PRS_EBILDIRGE_OLUSTUR_BATCH
***************************************************************************************/

PRS_EBILDIRGE_OLUSTUR_BATCH::PRS_EBILDIRGE_OLUSTUR_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::PRS_EBILDIRGE_OLUSTUR_BATCH )
{
    m_ui->setupUi      ( this );    
    START_BATCH_KERNEL ( this, DB );

    m_sube_id   = -1;
}

/**************************************************************************************
                   PRS_EBILDIRGE_OLUSTUR_BATCH::SETUP_FORM
***************************************************************************************/

void PRS_EBILDIRGE_OLUSTUR_BATCH::SETUP_FORM ()
{
    SET_PAGE_TITLE    ( tr ( "PRS - E-BİLDİRGE" ) );
    SET_HELP_PAGE     ( "personel-bordrosu_e-bildirge" );

    // Butonumuzun eklenecegi widgeti register ediyoruz.
    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "E-Bildirge" );

    // Programa ilk girildiginde focuslanacak widget setlenmelidir.
    SET_FIRST_FOCUS_WIDGET ( m_ui->combo_box_aylar );

    E9_FILL_COMBOBOX_AYLAR( m_ui->combo_box_aylar );
    PRS_FILL_COMBO_BOX_BORDRO_YILLARI( m_ui->combo_box_yillar );
    PRS_FILL_COMBO_BOX_SUBE_SECIMI(m_ui->combo_box_subeler) ;

    int current_month = QDate::currentDate().month();
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_aylar, E9_GET_AYLAR_STRING( current_month));

    SET_SETTING_NAME  ("E-Bildirge");
}

/**************************************************************************************
                   PRS_EBILDIRGE_OLUSTUR_BATCH::CHECK_VAR
***************************************************************************************/

int PRS_EBILDIRGE_OLUSTUR_BATCH::CHECK_VAR ( QObject * object )
{   
    if ( object EQ m_ui->combo_box_subeler ) {
        int result = SUBE_GET_SUBE_ID_ADI( m_ui->combo_box_subeler->currentText().split("-").at(0) , &m_sube_id );

        if ( result EQ 0 ) {
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EBILDIRGE_OLUSTUR_BATCH::CHECK_RUN
***************************************************************************************/

int PRS_EBILDIRGE_OLUSTUR_BATCH::CHECK_RUN ()
{
    SUBE_GET_SUBE_ID_ADI( m_ui->combo_box_subeler->currentText().split("-").at(0) , &m_sube_id );
    if ( m_sube_id EQ -1 ) {
        MSG_WARNING( tr( "Lütfen bir Şube seçiniz." ), m_ui->combo_box_subeler );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EBILDIRGE_OLUSTUR_BATCH::RUN_BATCH
***************************************************************************************/

void PRS_EBILDIRGE_OLUSTUR_BATCH::RUN_BATCH ()
{
    m_ay = E9_GET_AYLAR_ENUM( m_ui->combo_box_aylar->currentText() );
    m_yil= QVariant ( m_ui->combo_box_yillar->currentText() ).toInt();

    QString onerilen_dosya_ismi;
    QString str_ay = QVariant ( m_ay ).toString();

    if ( m_ay < 10 ) {
        str_ay = "0"+ QVariant ( m_ay ).toString();
    }

    onerilen_dosya_ismi.append( QVariant ( m_yil ).toString() );
    onerilen_dosya_ismi.append( str_ay );

    QFileDialog::Options options;
            options |= QFileDialog::DontUseNativeDialog;
        QString selectedFilter;
        QString dosya_yolu_ve_adi = QFileDialog::getSaveFileName(this,
                                    tr("E-Bildirge"),
                                    QDir::homePath()+ "/" +onerilen_dosya_ismi + ".xml",
                                    tr("XML files (*.xml);;"),
                                    &selectedFilter,
                                    options);

    m_file_path = dosya_yolu_ve_adi;

    E_BILDIRGE_OLUSTUR();
}

/**************************************************************************************
                   PRS_EBILDIRGE_OLUSTUR_BATCH::CHECK_EXIT
***************************************************************************************/

int PRS_EBILDIRGE_OLUSTUR_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_EBILDIRGE_OLUSTUR_BATCH::E_BILDIRGE_OLUSTUR
***************************************************************************************/

void PRS_EBILDIRGE_OLUSTUR_BATCH::E_BILDIRGE_OLUSTUR()
{
    // ISYERI BILGILERI
    QString sicil_no;
    QString kontrol_no;
    QString araci_no;
    QString unvan;
    QString adres;
    QString vergi_no;

    // SIGORTALI BILGILERI
    QString belge_turu;
    QString kanun_kodu;
    QString sigortali_sicil_no;
    QString tc_no;
    QString sigortali_adi;
    QString sigortali_soyadi;
    QString ilk_soyadi;
    double  pirime_esas_kazanc;         // prime Esas kazanc
    int     gun;                        // Bu alana sigortalının hizmet günü girilmelidir.sigortalı için gün sayısı 30 girilmelidir.

    QString     giris_gun_ddMM;         // Bir ay icinde ise girmisse  ddMM
    QString     cikis_gun_ddMM;         // Bir ay icinde isten cikmis ise  ddMM
    //int     uig;                      // ucretli izin kullanmis ise kulladigi gun sayisi
    //double  uig_pek;                  // ucretli izin gunune karsilik gelen prime esas kazanc girilmelidir.

    QString egn;                        // hizmet gunu 30 degilse eksik gun neden kodu
    QString isten_cikis_nedeni_kodu;    // hizmet belgesi bildirilen ay isten cikmis ise nedeni kodu

    // Bu degiskenler butun personeller icin doldurulacak ve xml hazirlanacak

    // Isyeri Bilgileri okunuyor

    sicil_no    = SUBE_GET_SUBE_SGK_NO( m_sube_id );
    kontrol_no  = "0";
    araci_no    = "000"; // araci kurum veya tasaron iseniz. yazilir yoksa 000
    adres       = SUBE_GET_SUBE_ADRES_BILGILERI( SUBE_GET_SUBE_ID() );

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "e9_sabit_degerler", "firma_ismi, firma_vergi_numarasi", "sabit_deger_id = :sabit_deger_id");
    query.SET_VALUE     ( ":sabit_deger_id", 1 );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        unvan       = query.VALUE( "firma_ismi" ).toString();
        vergi_no    = query.VALUE( "firma_vergi_numarasi" ).toString();
    }

    QFile file( m_file_path );
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;

    QXmlStreamWriter xml_writer ( &file );

    xml_writer.setAutoFormatting( true );
    xml_writer.writeStartDocument();

    xml_writer.writeStartElement("AYLIKBILDIRGELER");
        // ISYERI BILGILERI
        xml_writer.writeStartElement("ISYERI");
            xml_writer.writeAttribute("ISYERISICIL"     , sicil_no      );
            xml_writer.writeAttribute("KONTROLNO"       , kontrol_no    );
            xml_writer.writeAttribute("ISYERIARACINO"   , araci_no      );
            xml_writer.writeAttribute("ISYERIUNVAN"     , unvan         );
            xml_writer.writeAttribute("ISYERIADRES"     , adres         );
            xml_writer.writeAttribute("ISYERIVERGINO"   , vergi_no      );
        xml_writer.writeEndElement();

        xml_writer.writeStartElement("BORDRO");
            xml_writer.writeAttribute( "DONEMAY"        , QVariant ( m_ay ).toString()          );
            xml_writer.writeAttribute( "DONEMYIL"       , QVariant ( m_yil ).toString()         );
            xml_writer.writeAttribute( "BELGEMAHIYET"   , "A"           );
        xml_writer.writeEndElement();

    // Personel bilgileri
    SQL_QUERY f_query( DB );

    int  index_sira = 1;
    query.PREPARE_SELECT( "prs_personeller, prs_puantaj",
                          "calisma_turu_kodu, ind_kanunu_kodu",
                          "prs_personeller.personel_id = prs_puantaj.personel_id "
                          "AND sube_id = :sube_id                       "
                          "AND bordro_donemi_ay  = :bordro_donemi_ay    "
                          "AND bordro_donemi_yil = :bordro_donemi_yil   ",
                          "calisma_turu_kodu, ind_kanunu_kodu");
    query.SET_VALUE     ( ":sube_id", m_sube_id );
    query.SET_VALUE     ( ":bordro_donemi_ay", m_ay );
    query.SET_VALUE     ( ":bordro_donemi_yil", m_yil );

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() EQ true  ) {

            belge_turu   = query.VALUE( "calisma_turu_kodu" ).toString();
            kanun_kodu     = query.VALUE( "ind_kanunu_kodu" ).toString();

            // burasi icinde olan personeller gore tekrarlanacak
            xml_writer.writeStartElement("BILDIRGELER");
                xml_writer.writeAttribute("BELGETURU"       , belge_turu );
                xml_writer.writeAttribute("KANUN"           , kanun_kodu );
                xml_writer.writeStartElement("SIGORTALILAR");

            f_query.PREPARE_SELECT( "prs_personeller, prs_personel_nufus_blgler     , "
                                    "prs_puantaj, prs_personel_kisisel_blgler       , "
                                    "prs_bordro_personelleri    ",

                                    "prs_personeller.personel_id, cari_hesap_id, sicil_no, ilk_soyadi, gun_sayisi, "
                                    "prs_personel_kisisel_blgler.ise_giris_tarihi, "
                                    "prs_bordro_personelleri.brut_ucret, odenekler_toplami,calisma_sekli",

                                    /* WHERE */
                                    "prs_personeller.personel_id        = prs_personel_nufus_blgler.personel_id "
                                    "AND prs_personeller.personel_id    = prs_bordro_personelleri.personel_id  "
                                    "AND prs_personeller.personel_id    = prs_personel_kisisel_blgler.personel_id  "
                                    "AND prs_personeller.personel_id    = prs_puantaj.personel_id       "
                                    "AND calisma_turu_kodu              = :calisma_turu_kodu            "
                                    "AND ind_kanunu_kodu                = :ind_kanunu_kodu              "
                                    "AND sube_id                        = :sube_id                      "
                                    "AND bordro_donemi_ay               = :bordro_donemi_ay             "
                                    "AND bordro_donemi_yil              = :bordro_donemi_yil            ");

            f_query.SET_VALUE   ( ":calisma_turu_kodu"  , belge_turu    );
            f_query.SET_VALUE   ( ":ind_kanunu_kodu"    , kanun_kodu    );
            f_query.SET_VALUE   ( ":sube_id"            , m_sube_id     );
            f_query.SET_VALUE   ( ":bordro_donemi_ay"   , m_ay          );
            f_query.SET_VALUE   ( ":bordro_donemi_yil"  , m_yil         );


            if ( f_query.SELECT() NE 0 ) {

                while ( f_query.NEXT() EQ true ) {

                    int personel_id     = f_query.VALUE( "personel_id" ).toInt();
                    int cari_hesap_id   = f_query.VALUE( "cari_hesap_id" ).toInt();
                    sigortali_sicil_no  = f_query.VALUE( "sicil_no" ).toString();
                    ilk_soyadi          = f_query.VALUE( "ilk_soyadi" ).toString();
                    gun                 = f_query.VALUE( "gun_sayisi").toDouble();
                    int calisma_sekli   = f_query.VALUE( "calisma_sekli").toInt();

                    double brut_ucret               = f_query.VALUE( "brut_ucret").toDouble();
                    double odenekler_toplami        = f_query.VALUE( "odenekler_toplami").toDouble();

                    if ( calisma_sekli EQ ENUM_AYLIK ) {
                        if ( gun > 30 ) {
                            gun = 30;
                        }
                    }
                    pirime_esas_kazanc =  floor( brut_ucret + odenekler_toplami );

                    QDate ise_giris_tarihi        =  QDate::fromString( f_query.VALUE( "ise_giris_tarihi" ).toString(), "yyyy.MM.dd" );

                    giris_gun_ddMM.clear();
                    if ( ise_giris_tarihi.month() EQ m_ay  AND ise_giris_tarihi.year() EQ m_yil ) {
                       giris_gun_ddMM = ise_giris_tarihi.toString("ddMM");
                    }

                    cikis_gun_ddMM.clear();
                    isten_cikis_nedeni_kodu.clear();
                    SQL_QUERY s_query( DB );

                    s_query.PREPARE_SELECT( "prs_isten_ayrilanlar", "isten_ayrilma_tarihi, isten_ayrilma_sekli_enum ","personel_id = :personel_id" );
                    s_query.SET_VALUE     ( ":personel_id", personel_id );

                    if (  s_query.SELECT() NE 0 ) {
                        s_query.NEXT();

                        QDate isten_cikis_tarihi = QDate::fromString( s_query.VALUE( 0 ).toString(), "yyyy.MM.dd");
                        if ( isten_cikis_tarihi.month() EQ m_ay AND isten_cikis_tarihi.year() EQ m_yil ) {
                            cikis_gun_ddMM = isten_cikis_tarihi.toString( "ddMM");
                        }
                        int isten_ayrilma_kodu   = s_query.VALUE( 1 ).toInt();
                        if ( isten_ayrilma_kodu < 10 ) {
                            isten_cikis_nedeni_kodu = "0" + QVariant( isten_ayrilma_kodu ).toString();
                        }
                        else {
                            isten_cikis_nedeni_kodu = QVariant( isten_ayrilma_kodu ).toString();
                        }
                    }

                    // gun sayisi 30 az ise eksik gun nedeni aranilir.
                    if ( gun < 30 ) {
                        egn.clear();
                        s_query.PREPARE_SELECT( "prs_eksik_gun_nedenleri",
                                                "eksik_gun_sayisi, eksik_gun_neden_kodu     ",
                                                "personel_id = :personel_id                 "
                                                "AND bordro_donemi_ay = :bordro_donemi_ay   "
                                                "AND bordro_donemi_yil = :bordro_donemi_yil ");

                        s_query.SET_VALUE     ( ":bordro_donemi_ay" , m_ay );
                        s_query.SET_VALUE     ( ":bordro_donemi_yil", m_yil );
                        s_query.SET_VALUE     ( ":personel_id"      , personel_id );

                        if (  s_query.SELECT() NE 0 ) {
                            s_query.NEXT();

                            egn = s_query.VALUE("eksik_gun_neden_kodu").toString();
                        }
                    }
                    CARI_HESAP_STRUCT PERSONEL_BILG;

                    CARI_SET_HESAP_STRUCT_DEFAULTS ( &PERSONEL_BILG );

                    CARI_KART_BILGILERINI_OKU( cari_hesap_id, &PERSONEL_BILG );

                    QStringList ad_soyad_list = PERSONEL_BILG.cari_hesap_ismi.split(" ");

                    sigortali_adi.clear();
                    for ( int i = 0; i < ad_soyad_list.size() -1; i++ ) {
                        sigortali_adi += ad_soyad_list.at(i);
                    }

                    sigortali_soyadi  = ad_soyad_list.at( ad_soyad_list.size() -1 );
                    tc_no       = PERSONEL_BILG.kimlik_no;
                    ilk_soyadi  = "aaa"; // ilk soyad ?

                    // PERSONEL BILGILERI
                        xml_writer.writeStartElement("SIGORTALI");
                            xml_writer.writeAttribute( "SIRA"               , QVariant ( index_sira ).toString()                        );
                            xml_writer.writeAttribute( "SIGORTALISICIL"     , sigortali_sicil_no                                        );
                            xml_writer.writeAttribute( "TCKNO"              , tc_no                                                     );
                            xml_writer.writeAttribute( "AD"                 , sigortali_adi                                             );
                            xml_writer.writeAttribute( "SOYAD"              , sigortali_soyadi                                          );
                            xml_writer.writeAttribute( "PEK"                , QVariant ( (int ) pirime_esas_kazanc ).toString()         );
                            xml_writer.writeAttribute( "GUN"                , QVariant( gun ).toString()                                );

                            if( giris_gun_ddMM.isEmpty() EQ false ) {
                                xml_writer.writeAttribute( "GIRISGUN"           , giris_gun_ddMM                                        );
                            }

                            if ( cikis_gun_ddMM.isEmpty() EQ false ) {
                                xml_writer.writeAttribute( "CIKISGUN"           , cikis_gun_ddMM                                        );
                            }

                            xml_writer.writeAttribute( "EKSIKGUNSAYISI"     , ""                                                        );

                            if ( odenekler_toplami > 0.00 ) {
                                xml_writer.writeAttribute( "PRIM_IKRAMIYE"      , QVariant ( ( int ) floor( odenekler_toplami ) ).toString());
                            }

                            if ( egn.isEmpty() EQ false ) {
                                xml_writer.writeAttribute( "EKSIKGUNNEDENI"     , egn                                                   );
                            }

                            if ( isten_cikis_nedeni_kodu.isEmpty() EQ false ) {
                                xml_writer.writeAttribute( "ISTENCIKISNEDENI"   , isten_cikis_nedeni_kodu                               );
                            }
                        xml_writer.writeEndElement(); // end sigortali
                   index_sira++;
                }
            }
            else {
                MSG_WARNING( tr("E-Bildirge Alınacak Personel Bulunamadı"), NULL );

                file.close();
                file.remove( m_file_path );
                return;
            }
           xml_writer.writeEndElement(); // end sigortalilar
           xml_writer.writeEndElement(); // end bildirgeler
        }
    }
    xml_writer.writeEndDocument(); // end xml file
    file.close();
    MSG_INFO( tr("E-Bildirge Başarılı Şekilde Oluşturuldu."), NULL );
}
