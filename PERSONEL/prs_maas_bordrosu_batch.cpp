#include <QDate>
#include "prs_maas_bordrosu_batch_class.h"
#include "prs_maas_bordrosu_batch_open.h"
#include "print.h"
#include "adak_utils.h"
#include "e9_log.h"
#include "sube_console_utils.h"
#include "muh_fis_utils.h"
#include "cari_console_utils.h"
#include "cari_fis_utils.h"
#include "adak_std_utils.h"
#include "prs_gui_utils.h"
#include "e9_gui_utils.h"
#include "prs_puantajlari_gor_ve_duzelt_fisi_open.h"
#include "prs_maas_bordrosu_raporu_open.h"
#include "cari_struct.h"
#include "prs_enum.h"
#include "prs_con_utils.h"
#include "prs_struct.h"
#include "e9_enum.h"
#include "e9_console_utils.h"

extern ADAK_SQL      *      DB;
extern PRS_SABITLER          G_PRS_SABITLER;
extern PRS_FIRMA_ODENEKLERI  G_PRS_FRM_ODENEKLERI;

void SHOW_PRS_MAAS_BORDROSU_BATCH (QWidget * parent)
{
   PRS_MAAS_BORDROSU_BATCH * B = new PRS_MAAS_BORDROSU_BATCH(parent);

    B->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::PRS_MAAS_BORDROSU_BATCH
***************************************************************************************/

PRS_MAAS_BORDROSU_BATCH::PRS_MAAS_BORDROSU_BATCH(QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::PRS_MAAS_BORDROSU_BATCH)
{
    m_ui->setupUi       (this);
    START_BATCH_KERNEL  (this, DB );

}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::SETUP_FORM
**************************************************************************************/

void PRS_MAAS_BORDROSU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE                ("personel-bordrosu_maas-bordrosu");
    SET_PAGE_TITLE               ( tr ( "MAAŞ BORDROSU" ) );

    REGISTER_BUTTON_WIDGET(m_ui->widget_batch_buttons);

    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("MAAŞ BORDROSU AL") );

    E9_FILL_COMBOBOX_AYLAR ( m_ui->combo_box_aylar );

    COMBO_BOX_BORDRO_YILI_DOLDUR ();

    PRS_FILL_COMBO_BOX_SUBE_SECIMI(m_ui->combo_box_subeler) ;

    m_sube_id = SUBE_GET_SUBE_ID();

    m_ui->frame_subeler->setVisible( false );
    if ( SUBE_GET_SUBE_ISLEM_YETKISI() EQ 1 ) {
        m_ui->frame_subeler->setVisible( true );
    }

    SET_ENTER_KEY_FOR_RUN_BATCH  (true);

    SET_FIRST_FOCUS_WIDGET ( m_ui->combo_box_aylar );

    SET_SETTING_NAME        ( "PRS_MAAS_BORDROSU_BATCH" );
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::CHECK_VAR
***************************************************************************************/

int PRS_MAAS_BORDROSU_BATCH::CHECK_VAR(QObject * object)
{
    if ( object EQ m_ui->combo_box_subeler ) {        
        int result = SUBE_GET_SUBE_ID_ADI( m_ui->combo_box_subeler->currentText().split("-").at(0) , &m_sube_id );

        if ( result EQ 0 ) {
            return 0;
        }
    }
    else if ( object EQ m_ui->combo_box_aylar ) {      
        return BORDRO_ALABILIRMI();
    }
    return ADAK_OK;
}

/**************************************************************************************
                  PRS_MAAS_BORDROSU_BATCH::CHECK_RUN
**************************************************************************************/

int PRS_MAAS_BORDROSU_BATCH::CHECK_RUN()
{
    if ( m_ui->combo_box_aylar->currentIndex() < 0 OR
         m_ui->combo_box_yillar->currentIndex() < 0) {

         MSG_WARNING( tr("Ay ve Yıl Bilgilerini Doğru Girmelisiniz !"), m_ui->combo_box_aylar );

         return ADAK_FAIL;
     }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::RUN_BATCH
***************************************************************************************/

void PRS_MAAS_BORDROSU_BATCH::RUN_BATCH()
{
    if ( BORDRO_ALABILIRMI() EQ ADAK_FAIL ) {
        return;
    }

    int   bordro_hazirlansin_mi;

    int ay  =  E9_GET_AYLAR_ENUM( m_ui->combo_box_aylar->currentText() );
    int yil =  m_ui->combo_box_yillar->currentText().toInt();

    if ( m_sube_id EQ -1 ) {
        MSG_WARNING( tr( "Şube Kaydınız Bulunmamaktadır. Lütfen bir Şube' ye Kayıt olun." ), NULL );
        return;
    }

    int bordrosu_alinacak_prs_sayisi = GET_BORDRO_KESILECEK_PERSONEL_SAYISI( ay, yil );

    if ( bordrosu_alinacak_prs_sayisi EQ 0 ) {
        MSG_ERROR(tr("Toplu Bordro Dökümü İçin İşlem Görecek Firma Personeli Bulunmuyor !.."),NULL );
        return;
    }
    if ( PRS_VERILEN_DONEM_BORDROSU_KESILMIS_MI (  ay,  yil, m_sube_id ) EQ ADAK_OK ) {
        MSG_WARNING( E9_GET_AYLAR_STRING( ay ) + " " + QVariant(yil).toString() +
                               tr(" Dönemine Ait Bordro Daha Önce Hazırlanmıştır <br> "
                                  " Bordroyu İptal etmek için <b>Maaş Bordrosu İptal Seçeneğini</b> kullanmalısınız!.."), NULL );
        return;
    }
    if ( PRS_BIR_ONCEKI_AYIN_BORDROSU_KESILMIS_MI ( ay, yil ) EQ ADAK_FAIL ) {

        bordro_hazirlansin_mi = MSG_YES_NO( tr("Bir Önceki Ayın Bordrosu Kesilmedi ! <br><br>"
                                               "Önce Geçmiş Ayı Hazırlamalısınız!..") , NULL );
        if ( bordro_hazirlansin_mi EQ ADAK_NO ) {
            return;
        }
        bordro_hazirlansin_mi = MSG_YES_NO( tr("Bir Önceki Ayın Bordrosu Kesilmedi ! <br><br>"
                                               "Kümülatif Vergi Matrahı Değerleri Doğru Değil ! <br><br> "
                                               "Bordro Hazırlansın Mı ?"), NULL );
        if ( bordro_hazirlansin_mi EQ ADAK_NO ) {
            return;
        }
    }

    PRS_FILL_SABIT_DEGERLER_STRUCT        ();
    PRS_FILL_FIRMA_ODENEKLERI_STRUCT      ();
    if ( m_sube_id EQ -1 ) {
        MSG_WARNING( tr( "Lütfen bir ŞUBE 'ye kayıt olun... " ), NULL );
        return;
    }
    OPEN_PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI ( 1, m_sube_id,ay,yil, this );
    DB->START_TRANSACTION();
    PRS_BORDRO_STRUCT             BORDRO_HESAP;
    PRS_BORDRO_STRUCTINI_SIFIRLA  ( &BORDRO_HESAP );

    // BORDROSU ALINACAK TUM PERSONELLERIN BILGILERI TOPLANILIR ISLEM YAPILMAZ
    BORDRO_BILGILERINI_TOPLA   ( &BORDRO_HESAP, ay, yil);

    // TOPLANAN BILGILER ILE HESAPLAMA SADECE GESAPLAMA YAPILIR
    BORDRO_BILGILERINI_HESAPLA ( &BORDRO_HESAP  );

    // HESAPLANMIS BILGILER DB YAZILIR ISLEM YAPILMAZ.
    BORDRO_BILGILERINI_DB_YAZ  ( &BORDRO_HESAP );

    int bordro_id = PRS_GET_BORDRO_ID ( ay,  yil, m_sube_id );

    // MUHASEBE ENTEGRE OLUNUYOR.
    PRS_ENTEGRASYON_FISLERINI_OLUSTUR( bordro_id , E9_PROGRAMI );

    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER( SHOW_MAAS_BORDROSU_RAPORU( BORDRO_HESAP.bordro_id, "-1", ay, yil ), nativeParentWidget(), QPrinter::Landscape );
}


/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::CHECK_EXIT
***************************************************************************************/

int PRS_MAAS_BORDROSU_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}


/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::BORDRO_BILGILERINI_DB_YAZ
***************************************************************************************/

void PRS_MAAS_BORDROSU_BATCH::BORDRO_BILGILERINI_DB_YAZ (PRS_BORDRO_STRUCT * BORDRO_BILGILERI )
{
    QString             log_detaylari;

    SQL_QUERY           f_query(DB);
    SQL_QUERY           s_query(DB);

    /////////////////////////////////////PERSONEL_UCRETLERI///////////////////////////////////

    f_query.PREPARE_INSERT("prs_bordrolar","bordro_id", "bordro_donemi_ay, "
                           "bordro_donemi_yil, bordro_kesim_tarihi, "
                           "toplam_brut_ucret ,toplam_net_ucret , toplam_brut_odenek_tutari , "
                           "toplam_net_odenek_tutari, toplam_kesinti_tutari, sube_id ");

    f_query.SET_VALUE( ":bordro_donemi_ay"              , BORDRO_BILGILERI->bordro_donemi_ay );
    f_query.SET_VALUE( ":bordro_donemi_yil"             , BORDRO_BILGILERI->bordro_donemi_yil );
    f_query.SET_VALUE( ":bordro_kesim_tarihi"           , QDate::currentDate().toString("yyyy.MM.dd"));
    f_query.SET_VALUE( ":toplam_brut_ucret"             , BORDRO_BILGILERI->toplam_brut_ucret );
    f_query.SET_VALUE( ":toplam_net_ucret"              , BORDRO_BILGILERI->toplam_net_ucret );
    f_query.SET_VALUE( ":toplam_brut_odenek_tutari"     , BORDRO_BILGILERI->toplam_brut_odenek_tutari );
    f_query.SET_VALUE( ":toplam_net_odenek_tutari"      , BORDRO_BILGILERI->toplam_net_odenek_tutari );
    f_query.SET_VALUE( ":toplam_kesinti_tutari"         , BORDRO_BILGILERI->toplam_kesinti_tutari );
    f_query.SET_VALUE( ":sube_id"                       , m_sube_id );


   BORDRO_BILGILERI->bordro_id = f_query.INSERT();

   f_query.PREPARE_UPDATE("prs_ek_kesintiler_odenekler", "kesinti_odenek_id", "bordro_id, bordrosu_kesildi_mi",
                          "bordro_donemi_ay = :bordro_donemi_ay AND bordro_donemi_yil = :bordro_donemi_yil");

   f_query.SET_VALUE(":bordro_donemi_ay",   BORDRO_BILGILERI->bordro_donemi_ay );
   f_query.SET_VALUE(":bordro_donemi_yil",  BORDRO_BILGILERI->bordro_donemi_yil );
   f_query.SET_VALUE(":bordro_id",          BORDRO_BILGILERI->bordro_id );
   f_query.SET_VALUE(":bordrosu_kesildi_mi", EVET );
   f_query.UPDATE();


   // ocak ayinda kunulatif vergi matragi sifirlanilir
   if ( BORDRO_BILGILERI->bordro_donemi_ay EQ ENUM_OCAK ) {
       MSG_ERROR(tr("Personelin Kümülatif Gelir Vergi Matrahları Sıfırlanmıştır !.."),NULL);
   }

   for ( int i = 0 ; i < BORDRO_BILGILERI->personel_sayisi ; i++ ) {

       double toplam_gelir_vergisi_tutari = 0.00;
       for ( int j = 0; j < GELIR_VERGISI_DILIMI_SAYISI; j++ ) {

           toplam_gelir_vergisi_tutari += BORDRO_BILGILERI->PERSONEL[i].gelir_vergisi_tutari[j];
       }

       // ocak ayinda kunulatif vergi matragi sifirlanilir
       if ( BORDRO_BILGILERI->bordro_donemi_ay EQ ENUM_OCAK ) {

           BORDRO_BILGILERI->PERSONEL[i].bordro_sonrasi_kumulatif_vergi_matrahi =   BORDRO_BILGILERI->PERSONEL[i].brut_ucret
                                                                                  - BORDRO_BILGILERI->PERSONEL[i].ssk_primi_tutari
                                                                                  - BORDRO_BILGILERI->PERSONEL[i].issizlik_primi_tutari;
       }

       int calistigi_gun_sayisi = PRS_GET_PUANTAJ_GUN_SAYISI( BORDRO_BILGILERI->PERSONEL[i].personel_id , BORDRO_BILGILERI->bordro_donemi_yil, BORDRO_BILGILERI->bordro_donemi_ay  );

       if ( calistigi_gun_sayisi > 30 ) {
           calistigi_gun_sayisi = 30;
       }

        /////////////// PERSONEL BILGILERI ////////////////////////////////////////////////////

       f_query.PREPARE_INSERT("prs_bordro_personelleri","bordro_personel_id",
                             "bordro_id, personel_id, calisilan_gun_sayisi,brut_ucret, net_ucret, brd_oncesi_kum_vergi_matrahi, "
                             "brd_sonrasi_kum_vergi_matrahi, sigorta_primi, issizlik_primi, gelir_vergi_matrahi , gelir_vergisi, "
                             "asg_gec_indirimi, damga_vergisi,kesintiler_toplami, odenekler_toplami, puantaj_id , ozel_kesintiler_toplami ,"
                             "ssk_matrahi, bir_ay_once_kalan_ek_odenek, iki_ay_once_kalan_ek_odenek ");


        f_query.SET_VALUE( ":bordro_id"                               , BORDRO_BILGILERI->bordro_id );
        f_query.SET_VALUE( ":personel_id"                             , BORDRO_BILGILERI->PERSONEL[i].personel_id );
        f_query.SET_VALUE( ":calisilan_gun_sayisi"                    , calistigi_gun_sayisi );
        f_query.SET_VALUE( ":brut_ucret"                              , BORDRO_BILGILERI->PERSONEL[i].brut_ucret );
        f_query.SET_VALUE( ":net_ucret"                               , BORDRO_BILGILERI->PERSONEL[i].net_ucret );
        f_query.SET_VALUE( ":brd_oncesi_kum_vergi_matrahi"            , BORDRO_BILGILERI->PERSONEL[i].bordro_oncesi_kumulatif_vergi_matrahi );
        f_query.SET_VALUE( ":brd_sonrasi_kum_vergi_matrahi"           , BORDRO_BILGILERI->PERSONEL[i].bordro_sonrasi_kumulatif_vergi_matrahi );
        f_query.SET_VALUE( ":sigorta_primi"                           , BORDRO_BILGILERI->PERSONEL[i].ssk_primi_tutari );
        f_query.SET_VALUE( ":issizlik_primi"                          , BORDRO_BILGILERI->PERSONEL[i].issizlik_primi_tutari );
        f_query.SET_VALUE( ":gelir_vergi_matrahi"                     , BORDRO_BILGILERI->PERSONEL[i].gelir_vergi_matrahi );
        f_query.SET_VALUE( ":gelir_vergisi"                           , toplam_gelir_vergisi_tutari);
        f_query.SET_VALUE( ":asg_gec_indirimi"                        , BORDRO_BILGILERI->PERSONEL[i].asg_gec_indirimi_tutari );
        f_query.SET_VALUE( ":damga_vergisi"                           , BORDRO_BILGILERI->PERSONEL[i].damga_vergisi_tutari );
        f_query.SET_VALUE( ":kesintiler_toplami"                      , BORDRO_BILGILERI->PERSONEL[i].toplam_kesinti_tutari );
        f_query.SET_VALUE( ":odenekler_toplami"                       , BORDRO_BILGILERI->PERSONEL[i].toplam_brut_odenek_tutari );
        f_query.SET_VALUE( ":ozel_kesintiler_toplami"                 , BORDRO_BILGILERI->PERSONEL[i].toplam_ozel_kesintiler_tutari );
        f_query.SET_VALUE( ":ssk_matrahi"                             , BORDRO_BILGILERI->PERSONEL[i].ssk_matrahi );
        f_query.SET_VALUE( ":bir_ay_once_kalan_ek_odenek"             , BORDRO_BILGILERI->PERSONEL[i].bordro_bir_ay_once_kalan_ek_odenek );
        f_query.SET_VALUE( ":iki_ay_once_kalan_ek_odenek"             , BORDRO_BILGILERI->PERSONEL[i].bordro_iki_ay_once_kalan_ek_odenek );
        f_query.SET_VALUE( ":puantaj_id"                              , PRS_GET_PERSONEL_PUANTAJ_ID( BORDRO_BILGILERI->PERSONEL[i].personel_id, BORDRO_BILGILERI->bordro_donemi_yil, BORDRO_BILGILERI->bordro_donemi_ay ));
        f_query.INSERT();

        s_query.PREPARE_UPDATE( "prs_personeller","personel_id","kumulatif_vergi_matrahi, bir_ay_once_kalan_ek_odenek, iki_ay_once_kalan_ek_odenek ",
                                 "personel_id =:personel_id");

        s_query.SET_VALUE( ":kumulatif_vergi_matrahi"       , BORDRO_BILGILERI->PERSONEL[i].bordro_sonrasi_kumulatif_vergi_matrahi );
        s_query.SET_VALUE( ":bir_ay_once_kalan_ek_odenek"   , BORDRO_BILGILERI->PERSONEL[i].bir_ay_once_kalan_ek_odenek );
        s_query.SET_VALUE( ":iki_ay_once_kalan_ek_odenek"   , BORDRO_BILGILERI->PERSONEL[i].iki_ay_once_kalan_ek_odenek );
        s_query.SET_VALUE( ":personel_id"                   , BORDRO_BILGILERI->PERSONEL[i].personel_id );

        s_query.UPDATE();


        ////////////////PERSONEL_ODENEKLERI/////////////////////////////////////////////

       for ( int j = 0 ; j < BORDRO_BILGILERI->PERSONEL[i].odenek_sayisi ; j++ ) {

            f_query.PREPARE_INSERT("prs_brd_kesinti_odenekleri ","brd_kesinti_odenek_id",
                                 "bordro_personel_id ,                      "
                                 "bordro_id ,                               "
                                 "kesinti_odenek_ismi ,                     "
                                 "yuzde_mi ,                                "
                                 "tutari ,                                  "
                                 "kesinti_odenek_yuzdesi ,                  "
                                 "ssk_kesintisi_iceriyor_mu ,               "
                                 "ssk_kesinti_matrahi ,                     "
                                 "ssk_kesinti_tutari ,                      "
                                 "gelir_vergisi_iceriyor_mu ,               "
                                 "gelir_vergisi_matrahi ,                   "
                                 "gelir_vergisi_tutari ,                    "
                                 "damga_vergisi_iceriyor_mu ,               "
                                 "damga_vergisi_matrahi ,                   "
                                 "damga_vergisi_tutari ,                    "
                                 "aciklama ,                                "
                                 "kesinti_odenek_turu                       ");

            f_query.SET_VALUE(":bordro_personel_id"   ,             BORDRO_BILGILERI->PERSONEL[i].personel_id);
            f_query.SET_VALUE(":bordro_id" ,                        BORDRO_BILGILERI->bordro_id);
            f_query.SET_VALUE(":kesinti_odenek_ismi" ,              BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].odenek_adi);
            f_query.SET_VALUE(":yuzde_mi" ,                         BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].yuzde_mi);
            f_query.SET_VALUE(":tutari",                            BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].brut_tutari);
            f_query.SET_VALUE(":kesinti_odenek_yuzdesi" ,           BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].yuzde_orani);
            f_query.SET_VALUE(":ssk_kesintisi_iceriyor_mu",         BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].ssk_kesintisi_iceriyor_mu);
            f_query.SET_VALUE(":ssk_kesinti_matrahi" ,              BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].ssk_kesinti_matrahi);
            f_query.SET_VALUE(":ssk_kesinti_tutari" ,               BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].ssk_kesinti_tutari);
            f_query.SET_VALUE(":gelir_vergisi_iceriyor_mu" ,        BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].gelir_vergisi_iceriyor_mu);
            f_query.SET_VALUE(":gelir_vergisi_matrahi" ,            BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].gelir_vergisi_matrahi);
            f_query.SET_VALUE(":gelir_vergisi_tutari" ,             BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].gelir_vergisi_tutari);
            f_query.SET_VALUE(":damga_vergisi_iceriyor_mu" ,        BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].damga_vergisi_iceriyor_mu);
            f_query.SET_VALUE(":damga_vergisi_matrahi" ,            BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].damga_vergisi_matrahi);
            f_query.SET_VALUE(":damga_vergisi_tutari",              BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].damga_vergisi_tutari);
            f_query.SET_VALUE(":aciklama" ,                         BORDRO_BILGILERI->PERSONEL[i].ODENEKLER[j].aciklama);
            f_query.SET_VALUE(":bordro_id",                         BORDRO_BILGILERI->bordro_id);
            f_query.SET_VALUE(":kesinti_odenek_turu",               ENUM_PRS_ODENEK);

            f_query.INSERT();

        }

        ////////////////////////////////PERSONEL_KESINTILERI////////////////////////////////

       for ( int j = 0 ; j < BORDRO_BILGILERI->PERSONEL[i].kesinti_sayisi ; j++ ) {

           f_query.PREPARE_INSERT("prs_brd_kesinti_odenekleri","brd_kesinti_odenek_id " ,
                                  "bordro_personel_id,bordro_id ,"
                                  "kesinti_odenek_ismi ,yuzde_mi ,kesinti_odenek_yuzdesi ,"
                                  "tutari ,aciklama ,kesinti_odenek_turu ");

            f_query.SET_VALUE(":bordro_personel_id "    ,  BORDRO_BILGILERI->PERSONEL[i].personel_id);
            f_query.SET_VALUE(":bordro_id"              ,  BORDRO_BILGILERI->bordro_id);
            f_query.SET_VALUE(":kesinti_odenek_ismi"    ,  BORDRO_BILGILERI->PERSONEL[i].KESINTILER[j].kesinti_adi);
            f_query.SET_VALUE(":yuzde_mi"               ,  BORDRO_BILGILERI->PERSONEL[i].KESINTILER[j].yuzde_mi);
            f_query.SET_VALUE(":kesinti_odenek_yuzdesi" ,  BORDRO_BILGILERI->PERSONEL[i].KESINTILER[j].yuzde_orani);
            f_query.SET_VALUE(":tutari"                 ,  BORDRO_BILGILERI->PERSONEL[i].KESINTILER[j].tutari);
            f_query.SET_VALUE(":aciklama"               ,  BORDRO_BILGILERI->PERSONEL[i].KESINTILER[j].aciklama);
            f_query.SET_VALUE(":kesinti_odenek_turu"    ,  ENUM_PRS_KESINTI );

            f_query.INSERT() ;
        }
   }


   log_detaylari = E9_GET_AYLAR_STRING( BORDRO_BILGILERI->bordro_donemi_ay) + " " + QVariant(BORDRO_BILGILERI->bordro_donemi_yil).toString() +
                   " Bordrosu Kesilmiştir.";

   E9_LOG_KAYDI_EKLE(PERSONEL_MODULU , LOG_PRS_BORDROLAR , LOG_ISLEM_ADD ,log_detaylari);

}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::PERSONEL_ODENEKLERINI_STRUCTA_EKLE
***************************************************************************************/

void  PRS_MAAS_BORDROSU_BATCH::PERSONEL_ODENEKLERINI_STRUCTA_EKLE ( PRS_BORDRO_STRUCT *BORDRO_BILGILERI, PRS_PERSONEL_STRUCT *PERSONEL )
{
    SQL_QUERY          select_query   ( DB );
    SQL_QUERY          sql_query      ( DB );

    int i ;
    int personel_index = 0;
    PERSONEL->odenek_sayisi=0;

    select_query.PREPARE_SELECT( "prs_personeller",
                                 "dogum_yardimi_var_mi, olum_yardimi_var_mi,        "
                                 "cocuk_yardimi_var_mi, yakacak_yardimi_var_mi,     "
                                 "aile_yardimi_var_mi, yemek_parasi_var_mi,         "
                                 "evlenme_yardimi_var_mi, gorev_harcirahi_var_mi,   "
                                 "ihbar_tazminati_var_mi, kidem_tazminati_var_mi    ",
                                 "personel_id = :personel_id");

    select_query.SET_VALUE( ":personel_id"    ,PERSONEL->personel_id );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }
    select_query.NEXT();

    if (  select_query.VALUE("dogum_yardimi_var_mi").toInt() EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("olum_yardimi_var_mi").toInt() EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("cocuk_yardimi_var_mi").toInt() EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("yakacak_yardimi_var_mi").toInt() EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("aile_yardimi_var_mi").toInt() EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("yemek_parasi_var_mi").toInt() EQ 1 ) {
        PERSONEL->odenek_sayisi++ ;
    }
    if ( select_query.VALUE("evlenme_yardimi_var_mi").toInt() EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("gorev_harcirahi_var_mi").toInt() EQ 1 ) {
       PERSONEL->odenek_sayisi++;
    }

    sql_query.PREPARE_SELECT( "prs_ek_kesintiler_odenekler",
                              "kesinti_odenek_id, personel_id,                          "
                              "odenek_kesinti_adi,                                      "
                              "ssk_kesintisi_iceriyor_mu, gelir_vergisi_iceriyor_mu,    "
                              "damga_vergisi_iceriyor_mu, yuzde_mi, yuzde_orani, tutar, aciklama ",
                              "personel_id = :personel_id                       "
                              "AND bordro_donemi_ay     = :bordro_donemi_ay      "
                              "AND bordro_donemi_yil    = :bordro_donemi_yil     "
                              "AND bordrosu_kesildi_mi  = :bordrosu_kesildi_mi   "
                              "AND kesinti_odenek_turu  = :kesinti_odenek_turu   " );


    sql_query.SET_VALUE( ":personel_id"         ,  BORDRO_BILGILERI->PERSONEL->personel_id);
    sql_query.SET_VALUE( ":bordro_donemi_ay"    ,  BORDRO_BILGILERI->bordro_donemi_ay );
    sql_query.SET_VALUE( ":bordro_donemi_yil"   ,  BORDRO_BILGILERI->bordro_donemi_yil);
    sql_query.SET_VALUE( ":bordrosu_kesildi_mi" ,  0                 );
    sql_query.SET_VALUE( ":kesinti_odenek_turu" ,  ENUM_PRS_ODENEK   );

    int odenek_sayisi = sql_query.SELECT();

    PERSONEL->odenek_sayisi = PERSONEL[ personel_index ].odenek_sayisi + odenek_sayisi;

    // odenek sayisi kadar dizide yer ayriliyor.
    PERSONEL->ODENEKLER  = new PRS_ODENEK_STRUCT [ PERSONEL->odenek_sayisi + 1 ];

    if ( PERSONEL->odenek_sayisi > 0 ) {
        i = 0;
        if (  select_query.VALUE("dogum_yardimi_var_mi").toInt() EQ 1 ) {

            PRS_ODENEK_STRUCTINI_KOPYALA ( &( PERSONEL->ODENEKLER[i] ), &( G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI ) );

             if ( PERSONEL->ODENEKLER[i].ssk_kesintisi_iceriyor_mu EQ 1 ) {
                  PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi    = PERSONEL->ODENEKLER[i].brut_tutari;
                  PERSONEL->ODENEKLER[i].ssk_kesinti_tutari     = ROUND( PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi * G_PRS_SABITLER.ssk_primi_isci_yuzdesi );
             }
             if ( PERSONEL->ODENEKLER[i].damga_vergisi_iceriyor_mu EQ 1 ) {
                  PERSONEL->ODENEKLER[i].damga_vergisi_matrahi  = PERSONEL->ODENEKLER[i].brut_tutari;
                  PERSONEL->ODENEKLER[i].damga_vergisi_tutari   = ROUND( PERSONEL->ODENEKLER[i].damga_vergisi_matrahi * G_PRS_SABITLER.damga_vergisi );
             }
             if( PERSONEL->ODENEKLER[i].gelir_vergisi_iceriyor_mu EQ 1 ) {
                 PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi   = ROUND( PERSONEL->ODENEKLER[i].brut_tutari - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );
                 double kum_gelir_vergi = 0.0 ;
                 PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id ,PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi ,&kum_gelir_vergi);
                 if( PERSONEL->gelir_vergisi_tutari NE NULL ) {
                     for(int k=0; k < GELIR_VERGISI_DILIMI_SAYISI; k++ ) {
                         if( PERSONEL->gelir_vergisi_tutari[k] > 0 )
                             PERSONEL->ODENEKLER[k].gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[k];
                     }
                 }
             }

             PERSONEL->ODENEKLER[i].net_tutari = ROUND(   PERSONEL->ODENEKLER[i].brut_tutari
                                                                    - PERSONEL->ODENEKLER[i].damga_vergisi_tutari
                                                                    - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );

             i++;
        }
        if ( select_query.VALUE("olum_yardimi_var_mi").toInt() EQ 1 ) {

           PRS_ODENEK_STRUCTINI_KOPYALA ( &(PERSONEL->ODENEKLER[i]), &(G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI) );

           if( PERSONEL->ODENEKLER[i].ssk_kesintisi_iceriyor_mu EQ 1 ) {
               PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi   = PERSONEL->ODENEKLER[i].brut_tutari;
               PERSONEL->ODENEKLER[i].ssk_kesinti_tutari    = ROUND( PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi * G_PRS_SABITLER.ssk_primi_isci_yuzdesi );
           }
           if( PERSONEL->ODENEKLER[i].damga_vergisi_iceriyor_mu EQ 1){
               PERSONEL->ODENEKLER[i].damga_vergisi_matrahi = PERSONEL->ODENEKLER[i].brut_tutari;
               PERSONEL->ODENEKLER[i].damga_vergisi_tutari  = ROUND(PERSONEL->ODENEKLER[i].damga_vergisi_matrahi * G_PRS_SABITLER.damga_vergisi );
           }
           if( PERSONEL->ODENEKLER[i].gelir_vergisi_iceriyor_mu EQ 1 ) {
               PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi = ROUND( PERSONEL->ODENEKLER[i].brut_tutari - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );
               double kum_gelir_vergi = 0.0 ;
               PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id ,PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi ,&kum_gelir_vergi);
               if(PERSONEL->gelir_vergisi_tutari NE NULL){
                   for(int k = 0; k < GELIR_VERGISI_DILIMI_SAYISI; k++){
                       if( PERSONEL->gelir_vergisi_tutari[k] > 0 )
                           PERSONEL->ODENEKLER[i].gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[k];
                   }
               }
           }
           PERSONEL->ODENEKLER[i].net_tutari = ROUND( PERSONEL->ODENEKLER[i].brut_tutari
                                                                 -PERSONEL->ODENEKLER[i].damga_vergisi_tutari
                                                                 -PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );

           PERSONEL->damga_vergisi_tutari     +=  PERSONEL->ODENEKLER[i].damga_vergisi_tutari;
           PERSONEL->ssk_primi_tutari         +=  PERSONEL->ODENEKLER[i].ssk_kesinti_tutari;
           i++;
        }
        if ( select_query.VALUE("cocuk_yardimi_var_mi").toInt() EQ 1 ) {

           PRS_ODENEK_STRUCTINI_KOPYALA( &(PERSONEL->ODENEKLER[i]) , &(G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI) );

           if( PERSONEL->ODENEKLER[i].ssk_kesintisi_iceriyor_mu EQ 1) {
               PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi   = PERSONEL->ODENEKLER[i].brut_tutari;
               PERSONEL->ODENEKLER[i].ssk_kesinti_tutari    = ROUND(PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi*G_PRS_SABITLER.ssk_primi_isci_yuzdesi);
           }
           if( PERSONEL->ODENEKLER[i].damga_vergisi_iceriyor_mu EQ 1) {
               PERSONEL->ODENEKLER[i].damga_vergisi_matrahi = PERSONEL->ODENEKLER[i].brut_tutari;
               PERSONEL->ODENEKLER[i].damga_vergisi_tutari  = ROUND(PERSONEL->ODENEKLER[i].damga_vergisi_matrahi*G_PRS_SABITLER.damga_vergisi );
           }
           if( PERSONEL->ODENEKLER[i].gelir_vergisi_iceriyor_mu EQ 1 ) {
               PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi = ROUND(PERSONEL->ODENEKLER[i].brut_tutari - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );
               double kum_gelir_vergi = 0.0 ;
               PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id ,PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi ,&kum_gelir_vergi);
               if(PERSONEL->gelir_vergisi_tutari NE NULL){
                   for(int k = 0; k < GELIR_VERGISI_DILIMI_SAYISI; k++){
                       if( PERSONEL->gelir_vergisi_tutari[k] > 0 )
                           PERSONEL->ODENEKLER[i].gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[k];
                   }
               }
           }
           PERSONEL->ODENEKLER[i].net_tutari =ROUND(PERSONEL->ODENEKLER[i].brut_tutari
                                                                -PERSONEL->ODENEKLER[i].damga_vergisi_tutari
                                                                -PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );

           PERSONEL->damga_vergisi_tutari     +=  PERSONEL->ODENEKLER[i].damga_vergisi_tutari;
           PERSONEL->ssk_primi_tutari         +=  PERSONEL->ODENEKLER[i].ssk_kesinti_tutari;
           PERSONEL->gelir_vergi_matrahi      +=  PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi;
           i++;
        }
        if ( select_query.VALUE("yakacak_yardimi_var_mi").toInt() EQ 1 ) {

            PRS_ODENEK_STRUCTINI_KOPYALA(&(PERSONEL->ODENEKLER[i]) ,&(G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI));

            if(PERSONEL->ODENEKLER[i].ssk_kesintisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi  = PERSONEL->ODENEKLER[i].brut_tutari;
                PERSONEL->ODENEKLER[i].ssk_kesinti_tutari   = ROUND( PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi*G_PRS_SABITLER.ssk_primi_isci_yuzdesi);
            }
            if(PERSONEL->ODENEKLER[i].damga_vergisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].damga_vergisi_matrahi= PERSONEL->ODENEKLER[i].brut_tutari;
                PERSONEL->ODENEKLER[i].damga_vergisi_tutari = ROUND( PERSONEL->ODENEKLER[i].damga_vergisi_matrahi*G_PRS_SABITLER.damga_vergisi);
            }
            if(PERSONEL->ODENEKLER[i].gelir_vergisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi= ROUND( PERSONEL->ODENEKLER[i].brut_tutari - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );
                double kum_gelir_vergi = 0.0 ;
                PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id ,PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi ,&kum_gelir_vergi);
                if(PERSONEL->gelir_vergisi_tutari NE NULL){
                    for(int k = 0; k < GELIR_VERGISI_DILIMI_SAYISI; k++){
                        if( PERSONEL->gelir_vergisi_tutari[k] > 0 )
                            PERSONEL->ODENEKLER[i].gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[k];
                    }
                }
            }
            PERSONEL->ODENEKLER[i].net_tutari =ROUND(PERSONEL->ODENEKLER[i].brut_tutari
                                                                 -PERSONEL->ODENEKLER[i].damga_vergisi_tutari
                                                                 -PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );

            PERSONEL->damga_vergisi_tutari     +=  PERSONEL->ODENEKLER[i].damga_vergisi_tutari;
            PERSONEL->ssk_primi_tutari         +=  PERSONEL->ODENEKLER[i].ssk_kesinti_tutari;
            PERSONEL->gelir_vergi_matrahi      +=  PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi;
            i++;
        }
        if ( select_query.VALUE("aile_yardimi_var_mi").toInt() EQ 1 ) {

           PRS_ODENEK_STRUCTINI_KOPYALA(&(PERSONEL->ODENEKLER[i]) ,&(G_PRS_FRM_ODENEKLERI.AILE_YARDIMI));

           if( PERSONEL->ODENEKLER[i].ssk_kesintisi_iceriyor_mu EQ 1){
               PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi   = PERSONEL->ODENEKLER[i].brut_tutari;
               PERSONEL->ODENEKLER[i].ssk_kesinti_tutari    = ROUND( PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi*G_PRS_SABITLER.ssk_primi_isci_yuzdesi);
           }
           if(PERSONEL->ODENEKLER[i].damga_vergisi_iceriyor_mu EQ 1){
               PERSONEL->ODENEKLER[i].damga_vergisi_matrahi = PERSONEL->ODENEKLER[i].brut_tutari;
               PERSONEL->ODENEKLER[i].damga_vergisi_tutari  = ROUND( PERSONEL->ODENEKLER[i].damga_vergisi_matrahi*G_PRS_SABITLER.damga_vergisi);
           }
           if( PERSONEL->ODENEKLER[i].gelir_vergisi_iceriyor_mu EQ 1){
               PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi = ROUND( PERSONEL->ODENEKLER[i].brut_tutari - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );
               double kum_gelir_vergi = 0.0 ;
               PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id ,PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi ,&kum_gelir_vergi);
               if(PERSONEL->gelir_vergisi_tutari NE NULL){
                   for(int k = 0; k < GELIR_VERGISI_DILIMI_SAYISI; k++){
                       if( PERSONEL->gelir_vergisi_tutari[k] > 0 )
                           PERSONEL->ODENEKLER[i].gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[k];
                   }
               }
           }
           PERSONEL->ODENEKLER[i].net_tutari =ROUND( PERSONEL->ODENEKLER[i].brut_tutari
                                                                -PERSONEL->ODENEKLER[i].damga_vergisi_tutari
                                                                -PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );

           PERSONEL->damga_vergisi_tutari     +=  PERSONEL->ODENEKLER[i].damga_vergisi_tutari;
           PERSONEL->ssk_primi_tutari         +=  PERSONEL->ODENEKLER[i].ssk_kesinti_tutari;
           PERSONEL->gelir_vergi_matrahi      +=  PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi;
           i++;
        }
        if ( select_query.VALUE("yemek_parasi_var_mi").toInt() EQ 1 ) {

            PRS_ODENEK_STRUCTINI_KOPYALA(&(PERSONEL->ODENEKLER[i]) ,&(G_PRS_FRM_ODENEKLERI.YEMEK_PARASI));

            if(PERSONEL->ODENEKLER[i].ssk_kesintisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi  = PERSONEL->ODENEKLER[i].brut_tutari;
                PERSONEL->ODENEKLER[i].ssk_kesinti_tutari   = ROUND( PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi*G_PRS_SABITLER.ssk_primi_isci_yuzdesi);
            }
            if(PERSONEL->ODENEKLER[i].damga_vergisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].damga_vergisi_matrahi= PERSONEL->ODENEKLER[i].brut_tutari;
                PERSONEL->ODENEKLER[i].damga_vergisi_tutari = ROUND( PERSONEL->ODENEKLER[i].damga_vergisi_matrahi*G_PRS_SABITLER.damga_vergisi);
            }
            if(PERSONEL->ODENEKLER[i].gelir_vergisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi= ROUND( PERSONEL->ODENEKLER[i].brut_tutari - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );
                double kum_gelir_vergi = 0.0 ;
                PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id ,PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi ,&kum_gelir_vergi);
                if(PERSONEL->gelir_vergisi_tutari NE NULL){
                    for(int k = 0; k < GELIR_VERGISI_DILIMI_SAYISI; k++){
                        if( PERSONEL->gelir_vergisi_tutari[k] > 0 )
                            PERSONEL->ODENEKLER[i].gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[k];
                    }
                }
            }
            PERSONEL->ODENEKLER[i].net_tutari =ROUND( PERSONEL->ODENEKLER[i].brut_tutari
                                                                 -PERSONEL->ODENEKLER[i].damga_vergisi_tutari
                                                                 -PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );

            PERSONEL->damga_vergisi_tutari     +=  PERSONEL->ODENEKLER[i].damga_vergisi_tutari;
            PERSONEL->ssk_primi_tutari         +=  PERSONEL->ODENEKLER[i].ssk_kesinti_tutari;
            PERSONEL->gelir_vergi_matrahi      +=  PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi;
            i++;
        }
        if ( select_query.VALUE("evlenme_yardimi_var_mi").toInt() EQ 1 ) {

            PRS_ODENEK_STRUCTINI_KOPYALA(&(PERSONEL->ODENEKLER[i]) ,&(G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI));


            if( PERSONEL->ODENEKLER[i].ssk_kesintisi_iceriyor_mu EQ 1 ) {
                PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi  = PERSONEL->ODENEKLER[i].brut_tutari;
                PERSONEL->ODENEKLER[i].ssk_kesinti_tutari   = ROUND( PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi*G_PRS_SABITLER.ssk_primi_isci_yuzdesi);
            }
            if( PERSONEL->ODENEKLER[i].damga_vergisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].damga_vergisi_matrahi= PERSONEL->ODENEKLER[i].brut_tutari;
                PERSONEL->ODENEKLER[i].damga_vergisi_tutari = ROUND( PERSONEL->ODENEKLER[i].damga_vergisi_matrahi*G_PRS_SABITLER.damga_vergisi);
            }
            if( PERSONEL->ODENEKLER[i].gelir_vergisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi= ROUND( PERSONEL->ODENEKLER[i].brut_tutari - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );
                double kum_gelir_vergi = 0.0 ;
                PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id ,PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi ,&kum_gelir_vergi);
                if(PERSONEL->gelir_vergisi_tutari NE NULL){
                    for(int k = 0; k < GELIR_VERGISI_DILIMI_SAYISI; k++){
                        if( PERSONEL->gelir_vergisi_tutari[k] > 0 )
                            PERSONEL->ODENEKLER[i].gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[k];
                    }
                }
            }
            PERSONEL->ODENEKLER[i].net_tutari = ROUND(PERSONEL->ODENEKLER[i].brut_tutari
                                                                 -PERSONEL->ODENEKLER[i].damga_vergisi_tutari
                                                                 -PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );

            PERSONEL->damga_vergisi_tutari     +=  PERSONEL->ODENEKLER[i].damga_vergisi_tutari;
            PERSONEL->ssk_primi_tutari         +=  PERSONEL->ODENEKLER[i].ssk_kesinti_tutari;
            PERSONEL->gelir_vergi_matrahi      +=  PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi;
            i++;

        }
        if ( select_query.VALUE("gorev_harcirahi_var_mi").toInt() EQ 1 ) {

            PRS_ODENEK_STRUCTINI_KOPYALA(&(PERSONEL->ODENEKLER[i]) ,&(G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI));

            if(PERSONEL->ODENEKLER[i].ssk_kesintisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi      = PERSONEL->ODENEKLER[i].brut_tutari;
                PERSONEL->ODENEKLER[i].ssk_kesinti_tutari       = ROUND(PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi*G_PRS_SABITLER.ssk_primi_isci_yuzdesi);
            }
            if(PERSONEL->ODENEKLER[i].damga_vergisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].damga_vergisi_matrahi    = PERSONEL->ODENEKLER[i].brut_tutari;
                PERSONEL->ODENEKLER[i].damga_vergisi_tutari     = ROUND(PERSONEL->ODENEKLER[i].damga_vergisi_matrahi*G_PRS_SABITLER.damga_vergisi);
            }
            if(PERSONEL->ODENEKLER[i].gelir_vergisi_iceriyor_mu EQ 1){
                PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi    = ROUND(PERSONEL->ODENEKLER[i].brut_tutari - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );
                double kum_gelir_vergi = 0.0 ;
                PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id ,PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi ,&kum_gelir_vergi);
                if(PERSONEL->gelir_vergisi_tutari NE NULL){
                    for(int k = 0; k < GELIR_VERGISI_DILIMI_SAYISI; k++){
                        if( PERSONEL->gelir_vergisi_tutari[k] > 0 )
                            PERSONEL->ODENEKLER[i].gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[k];
                    }
                }
            }
            PERSONEL->ODENEKLER[i].net_tutari = ROUND ( PERSONEL->ODENEKLER[i].brut_tutari
                                                                   -PERSONEL->ODENEKLER[i].damga_vergisi_tutari
                                                                   -PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );

            PERSONEL->damga_vergisi_tutari     +=  PERSONEL->ODENEKLER[i].damga_vergisi_tutari;
            PERSONEL->ssk_primi_tutari         +=  PERSONEL->ODENEKLER[i].ssk_kesinti_tutari;
            PERSONEL->gelir_vergi_matrahi      +=  PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi;
            i++;
        }

        // ODENEK SIFIRLANIYOR.
        PRS_ODENEK_STRUCT  ODENEK;
        PRS_ODENEK_STRUCTINI_SIFIRLA ( &ODENEK );



        // BUTUN ODENEKLER HESAPLANDI VE STRUCT EKLENDI
        while ( sql_query.NEXT() EQ true ) {
            ODENEK.yuzde_orani          = ROUND ( sql_query.VALUE("yuzde_orani").toDouble() / 100 );
            switch ( ODENEK.yuzde_mi ) {
                 case PRS_YUZDE:
                   ODENEK.brut_tutari = ROUND( PRS_GET_PUANTAJ_TEMEL_PARA_BIRIM_TUTARI( PERSONEL->personel_id,
                                                                                    BORDRO_BILGILERI->bordro_donemi_yil,
                                                                                    BORDRO_BILGILERI->bordro_donemi_ay ) * ODENEK.yuzde_orani );
                     break;
                 case PRS_TUTAR  :
                    ODENEK.brut_tutari = sql_query.VALUE("tutar").toDouble();
                     break;
                 default :
                     break;
           }

           ODENEK.odenek_id                 = sql_query.VALUE("kesinti_odenek_id").toInt();
           ODENEK.ek_odenek_mi              = 1;
           ODENEK.aciklama                  = sql_query.VALUE("aciklama").toString();
           ODENEK.odenek_adi                = sql_query.VALUE("odenek_kesinti_adi").toString();
           ODENEK.yuzde_mi                  = sql_query.VALUE("yuzde_mi").toInt();
           ODENEK.ssk_kesinti_matrahi       = ROUND(ODENEK.brut_tutari);
           ODENEK.damga_vergisi_matrahi     = ROUND(ODENEK.brut_tutari );
           ODENEK.ssk_kesintisi_iceriyor_mu = sql_query.VALUE("ssk_kesintisi_iceriyor_mu").toInt();
           ODENEK.gelir_vergisi_iceriyor_mu = sql_query.VALUE("gelir_vergisi_iceriyor_mu").toInt();
           ODENEK.damga_vergisi_iceriyor_mu = sql_query.VALUE("damga_vergisi_iceriyor_mu").toInt();

           if(ODENEK.ssk_kesintisi_iceriyor_mu EQ 1){
               ODENEK.ssk_kesinti_tutari = ROUND( ODENEK.ssk_kesinti_matrahi * G_PRS_SABITLER.ssk_primi_isci_yuzdesi );
           }
           if (ODENEK.damga_vergisi_iceriyor_mu EQ 1){
               ODENEK.damga_vergisi_tutari = ROUND(ODENEK.damga_vergisi_matrahi * G_PRS_SABITLER.damga_vergisi);
           }
           ODENEK.gelir_vergisi_matrahi     = ROUND( ODENEK.brut_tutari - ODENEK.ssk_kesinti_tutari );
           if (ODENEK.gelir_vergisi_iceriyor_mu EQ 1){
               double  kum_gel_verg = 0.0;
               PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id, ODENEK.gelir_vergisi_matrahi, &kum_gel_verg) ;
               if( PERSONEL->gelir_vergisi_tutari NE NULL ) {
                   for ( int j=0; j< GELIR_VERGISI_DILIMI_SAYISI; j++ ){
                       if ( PERSONEL->gelir_vergisi_tutari[j] > 0 )
                           ODENEK.gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[j] ;
                   }
               }
           }

           ODENEK.net_tutari = ROUND(ODENEK.brut_tutari
                                -ODENEK.damga_vergisi_tutari
                                -ODENEK.gelir_vergisi_tutari
                                -ODENEK.ssk_kesinti_tutari);

           PRS_ODENEK_STRUCTINI_KOPYALA ( &PERSONEL->ODENEKLER[i], &ODENEK );

           PERSONEL->damga_vergisi_tutari     +=  PERSONEL->ODENEKLER[i].damga_vergisi_tutari;
           PERSONEL->ssk_primi_tutari         +=  PERSONEL->ODENEKLER[i].ssk_kesinti_tutari;
           PERSONEL->gelir_vergi_matrahi      +=  PERSONEL->ODENEKLER[i].gelir_vergisi_matrahi;
           i++;
        }
    }

}



/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::PERSONEL_KESINTILERINI_STRUCTA_EKLE
***************************************************************************************/

void  PRS_MAAS_BORDROSU_BATCH::PERSONEL_KESINTILERINI_STRUCTA_EKLE ( PRS_BORDRO_STRUCT * BORDRO_BILGILERI_STRUCT, PRS_PERSONEL_STRUCT * PERSONEL_BILGILERI_STRUCT )
{
    SQL_QUERY            query ( DB );

    int                  i =0;

    query.PREPARE_SELECT( "prs_ek_kesintiler_odenekler ",
                          "kesinti_odenek_id, personel_id, "
                          "odenek_kesinti_adi, "
                          "yuzde_mi, yuzde_orani, tutar, aciklama ",
                          "personel_id              = :personel_id "
                          "AND bordro_donemi_ay     = :bordro_donemi_ay     "
                          "AND bordro_donemi_yil    = :bordro_donemi_yil    "
                          "AND bordrosu_kesildi_mi  = :bordrosu_kesildi_mi  "
                          "AND kesinti_odenek_turu  = :kesinti_odenek_turu  ");


    query.SET_VALUE( ":personel_id"        ,  PERSONEL_BILGILERI_STRUCT->personel_id);
    query.SET_VALUE( ":bordro_donemi_ay"   ,  BORDRO_BILGILERI_STRUCT->bordro_donemi_ay );
    query.SET_VALUE( ":bordro_donemi_yil"  ,  BORDRO_BILGILERI_STRUCT->bordro_donemi_yil );
    query.SET_VALUE( ":bordrosu_kesildi_mi",  0);
    query.SET_VALUE( ":kesinti_odenek_turu",  ENUM_PRS_KESINTI );

    PERSONEL_BILGILERI_STRUCT->kesinti_sayisi   = query.SELECT() + 9;
    PERSONEL_BILGILERI_STRUCT->KESINTILER       = new PRS_KESINTI_STRUCT [ PERSONEL_BILGILERI_STRUCT->kesinti_sayisi ];

    for ( int i = 0; i < PERSONEL_BILGILERI_STRUCT->kesinti_sayisi; i++ ) {
        PRS_KESINTI_STRUCTINI_SIFIRLA( &PERSONEL_BILGILERI_STRUCT->KESINTILER[i] );
    }

    if ( PERSONEL_BILGILERI_STRUCT->kesinti_sayisi EQ 0 ) {
        return;
    }

    if ( PERSONEL_BILGILERI_STRUCT->kesinti_sayisi > 0 ) {

        // DIGER KESINTILERDE BURDA ATAMASI YAPILACAK

        double * GELIR_VERGILERI = new double [GELIR_VERGISI_DILIMI_SAYISI];

        PERSONEL_BILGILERI_STRUCT->calisilan_gun_sayisi    = PRS_GET_PUANTAJ_GUN_SAYISI( PERSONEL_BILGILERI_STRUCT->personel_id, BORDRO_BILGILERI_STRUCT->bordro_donemi_yil, BORDRO_BILGILERI_STRUCT->bordro_donemi_ay );
        PERSONEL_BILGILERI_STRUCT->brut_ucret              = ( PERSONEL_BILGILERI_STRUCT->brut_ucret / 30.0 ) * PERSONEL_BILGILERI_STRUCT->calisilan_gun_sayisi;

       // Brut ucret
        PERSONEL_BILGILERI_STRUCT->brut_ucret   = PERSONEL_BILGILERI_STRUCT->brut_ucret;

        if ( PRS_GET_PERSONEL_EMEKLI_MI( PERSONEL_BILGILERI_STRUCT->personel_id ) EQ false ) {
            // ssk prim ucreti
            if(PERSONEL_BILGILERI_STRUCT->brut_ucret >= G_PRS_SABITLER.ssk_tavan_degeri) {
                PERSONEL_BILGILERI_STRUCT->ssk_primi_tutari += ROUND(( G_PRS_SABITLER.ssk_tavan_degeri) * (G_PRS_SABITLER.ssk_primi_isci_yuzdesi));
            }
            else {
                PERSONEL_BILGILERI_STRUCT->ssk_primi_tutari += ROUND( G_PRS_SABITLER.ssk_primi_isci_yuzdesi * PERSONEL_BILGILERI_STRUCT->brut_ucret);
            }
            // issizlik primi ucreti
            if(PERSONEL_BILGILERI_STRUCT->brut_ucret >= G_PRS_SABITLER.ssk_tavan_degeri) {
                PERSONEL_BILGILERI_STRUCT->issizlik_primi_tutari = ROUND((G_PRS_SABITLER.ssk_tavan_degeri)*(G_PRS_SABITLER.issizlik_sig_isci_yuzdesi));
            }
            else {
                 PERSONEL_BILGILERI_STRUCT->issizlik_primi_tutari = ROUND( PERSONEL_BILGILERI_STRUCT->brut_ucret * (G_PRS_SABITLER.issizlik_sig_isci_yuzdesi) );
            }
        }
        else { // emekli ise
            // ssk prim ucreti
            if(PERSONEL_BILGILERI_STRUCT->brut_ucret >= G_PRS_SABITLER.ssk_tavan_degeri) {
                PERSONEL_BILGILERI_STRUCT->ssk_primi_tutari += ROUND((G_PRS_SABITLER.ssk_tavan_degeri)*(G_PRS_SABITLER.prs_sgdp_primi_isci_yuzdesi));
            }
            else {
                PERSONEL_BILGILERI_STRUCT->ssk_primi_tutari += ROUND(G_PRS_SABITLER.prs_sgdp_primi_isci_yuzdesi*PERSONEL_BILGILERI_STRUCT->brut_ucret);
            }
            // issizlik primi ucreti
            // emeklilerde issizlik primi yoktur.
            PERSONEL_BILGILERI_STRUCT->issizlik_primi_tutari = 0.00;
        }

        // Gelir vergi matrahi ,gelir vergi tutari
        if(PRS_GET_OZEL_DURUM_TURU_ENUM(PERSONEL_BILGILERI_STRUCT->personel_id ) NE 0){
            PERSONEL_BILGILERI_STRUCT->gelir_vergi_matrahi += ( PERSONEL_BILGILERI_STRUCT->brut_ucret
                                                  - PERSONEL_BILGILERI_STRUCT->ssk_primi_tutari
                                                  - PERSONEL_BILGILERI_STRUCT->issizlik_primi_tutari);

            PERSONEL_BILGILERI_STRUCT->gelir_vergi_matrahi += ROUND ( PERSONEL_BILGILERI_STRUCT->gelir_vergi_matrahi );
            PERSONEL_BILGILERI_STRUCT->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL_BILGILERI_STRUCT->personel_id ,PERSONEL_BILGILERI_STRUCT->gelir_vergi_matrahi,
                                                                                              &(PERSONEL_BILGILERI_STRUCT->bordro_oncesi_kumulatif_vergi_matrahi));

            for ( int i = 0; i < G_PRS_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi; i++ ) {
                 GELIR_VERGILERI[i] = PERSONEL_BILGILERI_STRUCT->gelir_vergisi_tutari[i];
            }
        }
        else {
            PERSONEL_BILGILERI_STRUCT->gelir_vergi_matrahi  += ROUND(PERSONEL_BILGILERI_STRUCT->brut_ucret
                                                   - PERSONEL_BILGILERI_STRUCT->ssk_primi_tutari
                                                   - PERSONEL_BILGILERI_STRUCT->issizlik_primi_tutari);

            PERSONEL_BILGILERI_STRUCT->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA ( PERSONEL_BILGILERI_STRUCT->personel_id, PERSONEL_BILGILERI_STRUCT->gelir_vergi_matrahi,
                                                                                               &(PERSONEL_BILGILERI_STRUCT->bordro_oncesi_kumulatif_vergi_matrahi) );

            for ( int i = 0; i < G_PRS_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi; i++ ) {
                 GELIR_VERGILERI[i] = PERSONEL_BILGILERI_STRUCT->gelir_vergisi_tutari[i];
            }

        }

        PERSONEL_BILGILERI_STRUCT->bordro_sonrasi_kumulatif_vergi_matrahi  = PERSONEL_BILGILERI_STRUCT->bordro_oncesi_kumulatif_vergi_matrahi;
        PERSONEL_BILGILERI_STRUCT->bordro_oncesi_kumulatif_vergi_matrahi   = PERSONEL_BILGILERI_STRUCT->bordro_oncesi_kumulatif_vergi_matrahi - PERSONEL_BILGILERI_STRUCT->gelir_vergi_matrahi;

        PERSONEL_BILGILERI_STRUCT->damga_vergisi_tutari += ROUND( PERSONEL_BILGILERI_STRUCT->brut_ucret * G_PRS_SABITLER.damga_vergisi );

        // Kesintiler
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].kesinti_adi      = tr ( "Damga Vergisi" );
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].tutari           = PERSONEL_BILGILERI_STRUCT->damga_vergisi_tutari;
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_mi         = 1 ;
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_orani      = G_PRS_SABITLER.damga_vergisi ;
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].aciklama         = "Damga Vergisi";

        i++;

        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].kesinti_adi      = tr ( "SSK Primi" );
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_mi         = 1 ;
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].tutari           = PERSONEL_BILGILERI_STRUCT->ssk_primi_tutari ;
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_orani      = G_PRS_SABITLER.ssk_primi_isci_yuzdesi;
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].aciklama         = "SSK Primi";

        i++;

        PERSONEL_BILGILERI_STRUCT->issizlik_primi_tutari = PERSONEL_BILGILERI_STRUCT->issizlik_primi_tutari;

        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].kesinti_adi      = tr ( "İşsizlik Primi" );
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_mi         = 1 ;
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].tutari           = PERSONEL_BILGILERI_STRUCT->issizlik_primi_tutari;
        PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_orani      = G_PRS_SABITLER.issizlik_sig_isci_yuzdesi ;

        i++;

        if ( PRS_PERSONEL_SENDIKALI_MI ( PERSONEL_BILGILERI_STRUCT->personel_id ) EQ ADAK_OK ) {
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].kesinti_adi  =  tr ( "Sendika Ücreti" ) ;
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_mi     = 1 ;
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].tutari       = PRS_GET_SENDIKA_UCRETI( PERSONEL_BILGILERI_STRUCT->personel_id );
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_orani  = 0.07; // Sendika aidati yuzdesi sabitlere cekilecek !!

            i++;
        }

        while ( query.NEXT() ){
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].kesinti_id    = query.VALUE("kesinti_odenek_id").toInt();
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].ek_kesinti_mi = 1;
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].aciklama      =  query.VALUE("aciklama").toString() ;
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].kesinti_adi   =  query.VALUE("odenek_kesinti_adi").toString();
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_mi      =  query.VALUE("yuzde_mi").toInt();
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_orani   =  ROUND(query.VALUE("yuzde_orani").toDouble()/100);
            PERSONEL_BILGILERI_STRUCT->KESINTILER[i].tutari        =  query.VALUE("tutar").toDouble();
            switch(  PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_mi ) {
                case PRS_YUZDE :
                 PERSONEL_BILGILERI_STRUCT->KESINTILER[i].tutari = ROUND( PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_orani * PERSONEL_BILGILERI_STRUCT->KESINTILER[i].tutari);
                    break;
                case PRS_TUTAR  :
                   PERSONEL_BILGILERI_STRUCT->KESINTILER[i].tutari = query.VALUE("tutar").toDouble();
                    break;
                default :
                    break;
            }
            i++;
        }

        if( GELIR_VERGILERI NE NULL ) {
            for ( int j = 0; j < GELIR_VERGISI_DILIMI_SAYISI; j++ ){
                if ( GELIR_VERGILERI[j] > 0 ) {
                    PERSONEL_BILGILERI_STRUCT->KESINTILER[i].kesinti_adi  =  tr( "Gelir Vergisi" ) ;
                    PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_mi     = 1 ;
                    PERSONEL_BILGILERI_STRUCT->KESINTILER[i].tutari       = GELIR_VERGILERI[j];
                    PERSONEL_BILGILERI_STRUCT->KESINTILER[i].yuzde_orani  = G_PRS_SABITLER.GELIR_VERGISI_YUZDELERI[j];
                    PERSONEL_BILGILERI_STRUCT->KESINTILER[i].aciklama     = "Gelir Vergisi";
                    i++;
                }
            }
        }

        double toplam_gelir_vergisi_tutari = 0.00;
        for ( int i = 0; i < GELIR_VERGISI_DILIMI_SAYISI; i++ ) {
            toplam_gelir_vergisi_tutari += PERSONEL_BILGILERI_STRUCT->gelir_vergisi_tutari[ i ];
        }

        PERSONEL_BILGILERI_STRUCT->asg_gec_indirimi_tutari = PRS_ASGARI_GECIM_INDIRIMI_HESAPLA ( PERSONEL_BILGILERI_STRUCT->personel_id );
        PERSONEL_BILGILERI_STRUCT->asg_gec_indirimi_tutari = ROUND ( PERSONEL_BILGILERI_STRUCT->asg_gec_indirimi_tutari );

        if ( PERSONEL_BILGILERI_STRUCT->asg_gec_indirimi_tutari >  toplam_gelir_vergisi_tutari ) {
            PERSONEL_BILGILERI_STRUCT->asg_gec_indirimi_tutari = ROUND ( toplam_gelir_vergisi_tutari );
        }

    }
 }

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH ::MAASI_NETTEN_BRUTE_CEVIR
 **************************************************************************************/

void PRS_MAAS_BORDROSU_BATCH::MAASI_NETTEN_BRUTE_CEVIR (PRS_BORDRO_STRUCT * BORDRO_BILGILERI, PRS_PERSONEL_STRUCT * PERSONEL_BILGILERI )
{

    // Delta Yontemi kullanilip yaklasik deger bulunmaya calisiliyor.

    double asgari_ucret_taban = G_PRS_SABITLER.ssk_16_yas_ustu_asgari_ucret;
    double net_ucret          = ROUND (PERSONEL_BILGILERI->net_ucret);

    double alt_deger      = qMax (net_ucret, asgari_ucret_taban);
    double ust_deger      = net_ucret * 3.0;
    double ortalama_deger = 0.0;
    bool   fark_bir_kurusa_indi_mi = false;
    bool   fark_bir_kurusa_indikten_sonra_fonksiyona_gonderildi_mi = false;

    while (1) {
        ortalama_deger = ROUND((alt_deger + ust_deger ) / 2.0);
        PERSONEL_BILGILERI->brut_ucret = ortalama_deger;

        MAASI_BRUTTEN_NETE_CEVIR ( BORDRO_BILGILERI, PERSONEL_BILGILERI );

        PERSONEL_BILGILERI->net_ucret = ROUND(PERSONEL_BILGILERI->net_ucret);

        if ( PERSONEL_BILGILERI->net_ucret EQ net_ucret) {
            break;
        }
        if ( (ust_deger - alt_deger) <= 0.1 ) {
            fark_bir_kurusa_indi_mi = true;
            if (fark_bir_kurusa_indikten_sonra_fonksiyona_gonderildi_mi EQ true ){
                break;
            }
        }
        if (PERSONEL_BILGILERI->net_ucret > net_ucret) {
            ust_deger = ortalama_deger;
        }
        else {
            alt_deger = ortalama_deger;
        }
        if ( fark_bir_kurusa_indi_mi EQ true){
            fark_bir_kurusa_indikten_sonra_fonksiyona_gonderildi_mi = true;
        }
    }


}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::COMBO_BOX_BORDRO_YILI_DOLDUR
***************************************************************************************/

void PRS_MAAS_BORDROSU_BATCH::COMBO_BOX_BORDRO_YILI_DOLDUR ()
{

    PRS_FILL_COMBO_BOX_BORDRO_YILLARI( m_ui->combo_box_yillar );

    int current_month = QDate::currentDate().month();
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_aylar, E9_GET_AYLAR_STRING( current_month));

}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::GET_BORDRO_KESILECEK_PERSONEL_SAYISI
***************************************************************************************/

int PRS_MAAS_BORDROSU_BATCH::GET_BORDRO_KESILECEK_PERSONEL_SAYISI( int p_bordro_ayi, int p_bordro_yili )
{

   QString bordro_son_tarih    = GET_BORDRO_DONEMI( p_bordro_ayi, p_bordro_yili );

   SQL_QUERY query( DB );

   QDate ilk_tarih_date_format = QDate::fromString( bordro_son_tarih, "yyyy.MM.dd");

   QString bordro_ilk_tarih    = ilk_tarih_date_format.toString( "yyyy.MM");

   bordro_ilk_tarih.append( ".01");

   query.PREPARE_SELECT( "prs_personel_kisisel_blgler,prs_personeller",
                           "prs_personel_kisisel_blgler.personel_id ",
                           "prs_personeller.personel_id  = prs_personel_kisisel_blgler.personel_id "
                           "AND sube_id            = :sube_id           "
                           "AND ise_giris_tarihi   <= :ise_giris_tarihi  "
                           "AND isten_cikis_tarihi >= :isten_cikis_tarihi");


    query.SET_VALUE( ":sube_id"            , m_sube_id );
    query.SET_VALUE( ":ise_giris_tarihi"   , bordro_son_tarih );
    query.SET_VALUE( ":isten_cikis_tarihi" , bordro_ilk_tarih );

   return query.SELECT();
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::GET_BORDRO_DONEMI
***************************************************************************************/

QString PRS_MAAS_BORDROSU_BATCH::GET_BORDRO_DONEMI( int p_bordro_ayi, int p_bordro_yili )
{
    QString bordro_donemi = QString::number( p_bordro_yili ) +"."+QString::number( p_bordro_ayi );
    if ( p_bordro_ayi < 10 ) {
        bordro_donemi = QString::number( p_bordro_yili ) +".0"+QString::number( p_bordro_ayi );
    }
    //AYIN KAC GUN CEK
    QDate gecici_date = QDate::fromString( bordro_donemi, "yyyy.MM");

    int kac_gun_cekiyor = gecici_date.daysInMonth();

    bordro_donemi.append( "."+ QVariant( kac_gun_cekiyor ).toString() );

    return bordro_donemi;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::BORDRO_BILGILERINI_TOPLA
***************************************************************************************/

void  PRS_MAAS_BORDROSU_BATCH::BORDRO_BILGILERINI_TOPLA(PRS_BORDRO_STRUCT *BORDRO_BILGILERI, int bordro_ay, int bordro_yil)
{
    // BU METHOD'DA SADECE BILGI TOPLANILMAKRADIR
    BORDRO_BILGILERI->bordro_donemi_ay   = bordro_ay;
    BORDRO_BILGILERI->bordro_donemi_yil  = bordro_yil;

    SQL_QUERY query( DB );

    QString bordro_donemi_son_tarih = GET_BORDRO_DONEMI( BORDRO_BILGILERI->bordro_donemi_ay, BORDRO_BILGILERI->bordro_donemi_yil );

    QDate   b_donemi_tarih          = QDate::fromString( bordro_donemi_son_tarih, "yyyy.MM.dd");
    QString bordro_donemi_ilk_tarih = b_donemi_tarih.toString( "yyyy.MM" );

    bordro_donemi_ilk_tarih.append( ".01");

    int bordrosu_alinacak_personel_sayisi = 0;

    query.PREPARE_SELECT( "prs_personel_kisisel_blgler,prs_personeller",
                          "prs_personel_kisisel_blgler.personel_id , cari_hesap_id, calisma_sekli , haftalik_calisilan_gun_sayisi, gunluk_yemek_parasi_tutari ",
                          "prs_personeller.personel_id  = prs_personel_kisisel_blgler.personel_id "
                          "AND sube_id              = :sube_id              "
                          "AND ise_giris_tarihi     <= :ise_giris_tarihi    "
                          "AND isten_cikis_tarihi   >= :isten_cikis_tarihi  ");

    query.SET_VALUE( ":sube_id"            , m_sube_id );
    query.SET_VALUE( ":ise_giris_tarihi"   , bordro_donemi_son_tarih );
    query.SET_VALUE( ":isten_cikis_tarihi" , bordro_donemi_ilk_tarih );

    bordrosu_alinacak_personel_sayisi = query.SELECT();

    if ( bordrosu_alinacak_personel_sayisi EQ 0 ) {
         return;
    }

    BORDRO_BILGILERI->personel_sayisi      = bordrosu_alinacak_personel_sayisi;
    // Bordroya kayitli personel kadar struct ayirdik.
    BORDRO_BILGILERI->PERSONEL = new PRS_PERSONEL_STRUCT [ bordrosu_alinacak_personel_sayisi ];

    int personel_index  = 0;

    while ( query.NEXT() EQ true ) {

        int  cari_hesap_id   = query.VALUE( 1 ).toInt();

        CARI_HESAP_STRUCT hesap_bilgileri;
        CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);
        CARI_KART_BILGILERINI_OKU  ( cari_hesap_id , &hesap_bilgileri  );

        PRS_PERSONEL_STRUCTINI_SIFIRLA ( &BORDRO_BILGILERI->PERSONEL[ personel_index ] );

        BORDRO_BILGILERI->PERSONEL[ personel_index ].personel_id          = query.VALUE( 0 ).toInt();
        BORDRO_BILGILERI->PERSONEL[ personel_index ].tc_kimlik_no         = hesap_bilgileri.kimlik_no;
        BORDRO_BILGILERI->PERSONEL[ personel_index ].sicil_no             = PRS_GET_PERSONEL_SICIL_NO (BORDRO_BILGILERI->PERSONEL[ personel_index ].personel_id ) ;
        BORDRO_BILGILERI->PERSONEL[ personel_index ].personel_adi_soyadi  = hesap_bilgileri.cari_hesap_ismi;
        BORDRO_BILGILERI->PERSONEL[ personel_index ].brut_ucret           = PRS_GET_PUANTAJ_TEMEL_PARA_BIRIM_TUTARI( BORDRO_BILGILERI->PERSONEL[ personel_index ].personel_id ,
                                                                                                                             BORDRO_BILGILERI->bordro_donemi_yil    , BORDRO_BILGILERI->bordro_donemi_ay );

        // Netten --> Brut hesaplama kullanilmaktadir
        BORDRO_BILGILERI->PERSONEL[ personel_index ].net_ucret            = BORDRO_BILGILERI->PERSONEL[ personel_index ].brut_ucret ;

        BORDRO_BILGILERI->PERSONEL[ personel_index ].calisilan_gun_sayisi = PRS_GET_PUANTAJ_GUN_SAYISI( query.VALUE(0).toInt(), BORDRO_BILGILERI->bordro_donemi_yil,
                                                                                                                BORDRO_BILGILERI->bordro_donemi_ay );

        BORDRO_BILGILERI->PERSONEL[personel_index].calisma_sekli           = query.VALUE("calisma_sekli").toInt();

        double calisilan_gun_sayisi = BORDRO_BILGILERI->PERSONEL[ personel_index ].calisilan_gun_sayisi;

        if ( BORDRO_BILGILERI->PERSONEL[personel_index].calisma_sekli EQ  ENUM_AYLIK ) {
            BORDRO_BILGILERI->PERSONEL[ personel_index ].brut_ucret = ROUND( ( BORDRO_BILGILERI->PERSONEL[ personel_index ].brut_ucret / 30.0 ) * calisilan_gun_sayisi );
        }

        BORDRO_BILGILERI->PERSONEL[personel_index].bordro_oncesi_kumulatif_vergi_matrahi  = ROUND ( PRS_GET_PERSONEL_KUMULATIF_VERGI_MATRAHI ( BORDRO_BILGILERI->PERSONEL[personel_index].personel_id)  );
        BORDRO_BILGILERI->PERSONEL[personel_index].bordro_sonrasi_kumulatif_vergi_matrahi = BORDRO_BILGILERI->PERSONEL[personel_index].bordro_oncesi_kumulatif_vergi_matrahi;


        double bir_ay_once_kalan_ek_odenek,  iki_ay_once_kalan_ek_odenek;

        PRS_GET_ONCEKI_AYLARDAN_KALAN_EK_ODENEKLER( BORDRO_BILGILERI->PERSONEL[ personel_index ].personel_id, &bir_ay_once_kalan_ek_odenek, &iki_ay_once_kalan_ek_odenek );

        BORDRO_BILGILERI->PERSONEL[ personel_index ].bir_ay_once_kalan_ek_odenek = bir_ay_once_kalan_ek_odenek;
        BORDRO_BILGILERI->PERSONEL[ personel_index ].iki_ay_once_kalan_ek_odenek = iki_ay_once_kalan_ek_odenek;

        BORDRO_BILGILERI->PERSONEL[personel_index].yemek_verilen_gun_sayisi       = PRS_GET_AYLIK_YEMEK_VERILEN_GUN_SAYISI( BORDRO_BILGILERI->PERSONEL[ personel_index ].personel_id, BORDRO_BILGILERI->bordro_donemi_ay, BORDRO_BILGILERI->bordro_donemi_yil );
        BORDRO_BILGILERI->PERSONEL[personel_index].yemek_yardimi_tutari           = query.VALUE( "gunluk_yemek_parasi_tutari").toDouble();

        ODENEKLERI_TOPLA    ( BORDRO_BILGILERI , &( BORDRO_BILGILERI->PERSONEL[ personel_index ] ) );
        KESINTILERI_TOPLA   ( BORDRO_BILGILERI , &( BORDRO_BILGILERI->PERSONEL[ personel_index ] ) );

        personel_index++;
    }

}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::ODENEKLERI_TOPLA
***************************************************************************************/

void PRS_MAAS_BORDROSU_BATCH::ODENEKLERI_TOPLA( PRS_BORDRO_STRUCT* BORDRO_BILGILERI, PRS_PERSONEL_STRUCT* PERSONEL )
{
    SQL_QUERY          select_query   ( DB );
    SQL_QUERY          sql_query      ( DB );

    PERSONEL->odenek_sayisi=0;

    select_query.PREPARE_SELECT( "prs_personeller",
                                 "dogum_yardimi_var_mi, olum_yardimi_var_mi,        "
                                 "cocuk_yardimi_var_mi, yakacak_yardimi_var_mi,     "
                                 "aile_yardimi_var_mi, yemek_parasi_var_mi,         "
                                 "evlenme_yardimi_var_mi, gorev_harcirahi_var_mi,   "
                                 "ihbar_tazminati_var_mi, kidem_tazminati_var_mi    ",
                                 "personel_id = :personel_id");

    select_query.SET_VALUE( ":personel_id", PERSONEL->personel_id );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }
    select_query.NEXT();

    if (  select_query.VALUE("dogum_yardimi_var_mi").toInt()    EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("olum_yardimi_var_mi").toInt()      EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("cocuk_yardimi_var_mi").toInt()     EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("yakacak_yardimi_var_mi").toInt()   EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("aile_yardimi_var_mi").toInt()      EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("yemek_parasi_var_mi").toInt()      EQ 1 ) {
        PERSONEL->odenek_sayisi++ ;
    }
    if ( select_query.VALUE("evlenme_yardimi_var_mi").toInt()   EQ 1 ) {
        PERSONEL->odenek_sayisi ++;
    }
    if ( select_query.VALUE("gorev_harcirahi_var_mi").toInt()   EQ 1 ) {
       PERSONEL->odenek_sayisi++;
    }

    sql_query.PREPARE_SELECT( "prs_ek_kesintiler_odenekler",
                              "kesinti_odenek_id, personel_id,                          "
                              "odenek_kesinti_adi,                                      "
                              "ssk_kesintisi_iceriyor_mu, gelir_vergisi_iceriyor_mu,    "
                              "damga_vergisi_iceriyor_mu, yuzde_mi, yuzde_orani, tutar, aciklama ",
                              "personel_id = :personel_id                       "
                              "AND bordro_donemi_ay     = :bordro_donemi_ay      "
                              "AND bordro_donemi_yil    = :bordro_donemi_yil     "
                              "AND bordrosu_kesildi_mi  = :bordrosu_kesildi_mi   "
                              "AND kesinti_odenek_turu  = :kesinti_odenek_turu   " );


    sql_query.SET_VALUE( ":personel_id"         ,  PERSONEL->personel_id);
    sql_query.SET_VALUE( ":bordro_donemi_ay"    ,  BORDRO_BILGILERI->bordro_donemi_ay );
    sql_query.SET_VALUE( ":bordro_donemi_yil"   ,  BORDRO_BILGILERI->bordro_donemi_yil);
    sql_query.SET_VALUE( ":bordrosu_kesildi_mi" ,  0                 );
    sql_query.SET_VALUE( ":kesinti_odenek_turu" ,  ENUM_PRS_ODENEK   );

    int odenek_sayisi = sql_query.SELECT();

    PERSONEL->odenek_sayisi = PERSONEL->odenek_sayisi + odenek_sayisi;

    // odenek sayisi kadar dizide yer ayriliyor.
    PERSONEL->ODENEKLER  = new PRS_ODENEK_STRUCT [ PERSONEL->odenek_sayisi + 1 ];

    if ( PERSONEL->odenek_sayisi > 0 ) {

        int i = 0;
        if (  select_query.VALUE("dogum_yardimi_var_mi").toInt() EQ 1 ) {
            PRS_ODENEK_STRUCTINI_KOPYALA    ( &(PERSONEL->ODENEKLER[i] ), &( G_PRS_FRM_ODENEKLERI.DOGUM_YARDIMI ) );
            i++;
        }
        if ( select_query.VALUE("olum_yardimi_var_mi").toInt() EQ 1 ) {
            PRS_ODENEK_STRUCTINI_KOPYALA    ( &(PERSONEL->ODENEKLER[i] ), &(G_PRS_FRM_ODENEKLERI.OLUM_YARDIMI) );
           i++;
        }
        if ( select_query.VALUE("cocuk_yardimi_var_mi").toInt() EQ 1 ) {
            PRS_ODENEK_STRUCTINI_KOPYALA    ( &(PERSONEL->ODENEKLER[i] ), &(G_PRS_FRM_ODENEKLERI.COCUK_YARDIMI) );
           i++;
        }
        if ( select_query.VALUE("yakacak_yardimi_var_mi").toInt() EQ 1 ) {
            PRS_ODENEK_STRUCTINI_KOPYALA    ( &(PERSONEL->ODENEKLER[i] ), &(G_PRS_FRM_ODENEKLERI.YAKACAK_YARDIMI));
            i++;
        }
        if ( select_query.VALUE("aile_yardimi_var_mi").toInt() EQ 1 ) {
            PRS_ODENEK_STRUCTINI_KOPYALA    ( &(PERSONEL->ODENEKLER[i] ), &(G_PRS_FRM_ODENEKLERI.AILE_YARDIMI));
           i++;
        }
        if ( select_query.VALUE("yemek_parasi_var_mi").toInt() EQ 1 ) {
            PRS_ODENEK_STRUCTINI_KOPYALA    ( &(PERSONEL->ODENEKLER[i] ), &(G_PRS_FRM_ODENEKLERI.YEMEK_PARASI));

            SQL_QUERY query( DB );

            query.PREPARE_SELECT("prs_personeller","gunluk_yemek_parasi_tutari", "personel_id = :personel_id");
            query.SET_VALUE     (":personel_id", PERSONEL->personel_id );

            query.SELECT();
            query.NEXT();

            PERSONEL->ODENEKLER[i].brut_tutari  = query.VALUE( 0 ).toDouble();

            PERSONEL->ODENEKLER[ i ].yuzde_mi    = 0;
            PERSONEL->ODENEKLER[ i ].yuzde_orani = 0;

            i++;
        }
        if ( select_query.VALUE("evlenme_yardimi_var_mi").toInt() EQ 1 ) {
            PRS_ODENEK_STRUCTINI_KOPYALA    ( &(PERSONEL->ODENEKLER[i] ), &(G_PRS_FRM_ODENEKLERI.EVLENME_YARDIMI));
            i++;
        }
        if ( select_query.VALUE("gorev_harcirahi_var_mi").toInt() EQ 1 ) {
            PRS_ODENEK_STRUCTINI_KOPYALA    ( &(PERSONEL->ODENEKLER[i] ), &(G_PRS_FRM_ODENEKLERI.GOREV_HARCIHARI));
            i++;
        }

        // DIGER EK ODENEKLER
        while ( sql_query.NEXT() EQ true ) {

           PERSONEL->ODENEKLER[i].yuzde_orani               = ROUND ( sql_query.VALUE("yuzde_orani").toDouble() / 100 );
           PERSONEL->ODENEKLER[i].brut_tutari               = sql_query.VALUE("tutar").toDouble();
           PERSONEL->ODENEKLER[i].odenek_id                 = sql_query.VALUE("kesinti_odenek_id").toInt();
           PERSONEL->ODENEKLER[i].ek_odenek_mi              = 1;
           PERSONEL->ODENEKLER[i].aciklama                  = sql_query.VALUE("aciklama").toString();
           PERSONEL->ODENEKLER[i].odenek_adi                = sql_query.VALUE("odenek_kesinti_adi").toString();
           PERSONEL->ODENEKLER[i].yuzde_mi                  = sql_query.VALUE("yuzde_mi").toInt();
           PERSONEL->ODENEKLER[i].ssk_kesinti_matrahi       = 0.00;
           PERSONEL->ODENEKLER[i].damga_vergisi_matrahi     = 0.00;
           PERSONEL->ODENEKLER[i].ssk_kesintisi_iceriyor_mu = sql_query.VALUE("ssk_kesintisi_iceriyor_mu").toInt();
           PERSONEL->ODENEKLER[i].gelir_vergisi_iceriyor_mu = sql_query.VALUE("gelir_vergisi_iceriyor_mu").toInt();
           PERSONEL->ODENEKLER[i].damga_vergisi_iceriyor_mu = sql_query.VALUE("damga_vergisi_iceriyor_mu").toInt();
           i++;
        }
    }
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::KESINTILERI_TOPLA
***************************************************************************************/

void PRS_MAAS_BORDROSU_BATCH::KESINTILERI_TOPLA(PRS_BORDRO_STRUCT* BORDRO_BILGILERI, PRS_PERSONEL_STRUCT* PERSONEL )
{
    SQL_QUERY            query ( DB );

    query.PREPARE_SELECT( "prs_ek_kesintiler_odenekler ",
                          "kesinti_odenek_id, personel_id, "
                          "odenek_kesinti_adi, "
                          "yuzde_mi, yuzde_orani, tutar, aciklama ",
                          "personel_id              = :personel_id "
                          "AND bordro_donemi_ay     = :bordro_donemi_ay     "
                          "AND bordro_donemi_yil    = :bordro_donemi_yil    "
                          "AND bordrosu_kesildi_mi  = :bordrosu_kesildi_mi  "
                          "AND kesinti_odenek_turu  = :kesinti_odenek_turu  ");

    // EK KESINTILER
    query.SET_VALUE( ":personel_id"        ,  PERSONEL->personel_id     );
    query.SET_VALUE( ":bordro_donemi_ay"   ,  BORDRO_BILGILERI->bordro_donemi_ay    );
    query.SET_VALUE( ":bordro_donemi_yil"  ,  BORDRO_BILGILERI->bordro_donemi_yil   );
    query.SET_VALUE( ":bordrosu_kesildi_mi",  0);
    query.SET_VALUE( ":kesinti_odenek_turu",  ENUM_PRS_KESINTI );

    PERSONEL->kesinti_sayisi   = query.SELECT();
    PERSONEL->KESINTILER       = new PRS_KESINTI_STRUCT [ PERSONEL->kesinti_sayisi ];

    for ( int i = 0; i < PERSONEL->kesinti_sayisi; i++ ) {
        PRS_KESINTI_STRUCTINI_SIFIRLA( &PERSONEL->KESINTILER[i] );
    }

    if ( PERSONEL->kesinti_sayisi EQ 0 ) {
        return;
    }

    int i = 0;
    if ( PERSONEL->kesinti_sayisi > 0 ) {

         // TEMEK KESINTIKER : SSK PRIMI, DAMGA VERGISI, ISSIZLIK PRIMI, GELIR VERGISI [5], SENDIKA UCRETI
         // TEMEL KESINTILER DISINDAKI KESINTILER ATANIYOR
          while ( query.NEXT() EQ true ) {
            PERSONEL->KESINTILER[i].kesinti_id    = query.VALUE("kesinti_odenek_id").toInt();
            PERSONEL->KESINTILER[i].ek_kesinti_mi = 1;
            PERSONEL->KESINTILER[i].aciklama      =  query.VALUE("aciklama").toString() ;
            PERSONEL->KESINTILER[i].kesinti_adi   =  query.VALUE("odenek_kesinti_adi").toString();
            PERSONEL->KESINTILER[i].yuzde_mi      =  query.VALUE("yuzde_mi").toInt();
            PERSONEL->KESINTILER[i].yuzde_orani   =  ROUND(query.VALUE("yuzde_orani").toDouble()/100.0);
            PERSONEL->KESINTILER[i].tutari        =  query.VALUE("tutar").toDouble();
            i++;
        }
    }
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::BORDRO_BILGILERINI_HESAPLA
***************************************************************************************/

void PRS_MAAS_BORDROSU_BATCH::BORDRO_BILGILERINI_HESAPLA( PRS_BORDRO_STRUCT* BORDRO_BILGILERI )
{
    for ( int i = 0 ; i < BORDRO_BILGILERI->personel_sayisi ; i++ ) {

        if ( PRS_PERSONEL_MAASI_BRUT_MU ( BORDRO_BILGILERI->PERSONEL[i].personel_id ) EQ true ) {
            BORDRO_BILGILERI->PERSONEL[i].brut_ucret = ROUND( BORDRO_BILGILERI->PERSONEL[i].brut_ucret) ;
            MAASI_BRUTTEN_NETE_CEVIR ( BORDRO_BILGILERI , &BORDRO_BILGILERI->PERSONEL[i] );
        }
        else {
            BORDRO_BILGILERI->PERSONEL[i].net_ucret = ROUND( BORDRO_BILGILERI->PERSONEL[i].calisilan_gun_sayisi*BORDRO_BILGILERI->PERSONEL[i].net_ucret/30) ;
            MAASI_NETTEN_BRUTE_CEVIR ( BORDRO_BILGILERI , &BORDRO_BILGILERI->PERSONEL[i] );
        }

        // BORDRO TOPLAMLARI HESAPLANIYOR.

        BORDRO_BILGILERI->toplam_brut_ucret             += BORDRO_BILGILERI->PERSONEL[i].brut_ucret;
        BORDRO_BILGILERI->toplam_net_ucret              += BORDRO_BILGILERI->PERSONEL[i].net_ucret;
        BORDRO_BILGILERI->toplam_brut_odenek_tutari     += BORDRO_BILGILERI->PERSONEL[i].toplam_brut_odenek_tutari;
        BORDRO_BILGILERI->toplam_net_odenek_tutari      += BORDRO_BILGILERI->PERSONEL[i].toplam_net_odenek_tutari;
        BORDRO_BILGILERI->toplam_damga_vergisi_tutari   += BORDRO_BILGILERI->PERSONEL[i].damga_vergisi_tutari;
        BORDRO_BILGILERI->toplam_kesinti_tutari         += BORDRO_BILGILERI->PERSONEL[i].toplam_kesinti_tutari;
    }
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::BRUTTEN_NETE_CEVIR
***************************************************************************************/

void PRS_MAAS_BORDROSU_BATCH::MAASI_BRUTTEN_NETE_CEVIR( PRS_BORDRO_STRUCT* BORDRO_BILGILERI, PRS_PERSONEL_STRUCT* PERSONEL )
{
    // ODENEKLER HESAPLANIYOR
    double toplam_damga_vergisi_matrahi = 0.00;
    double toplam_ssk_kesintisi_matrahi = 0.00;
    double toplam_gelir_vergisi_matrahi = 0.00;
    double toplam_odenek                = 0.00;

    for ( int i = 0; i < PERSONEL->odenek_sayisi; i++ ) {
        if ( PERSONEL->ODENEKLER[ i ].yuzde_mi EQ 1 ) {
             PERSONEL->ODENEKLER[ i ].brut_tutari = ROUND( PRS_GET_PUANTAJ_TEMEL_PARA_BIRIM_TUTARI( PERSONEL->personel_id, BORDRO_BILGILERI->bordro_donemi_yil,
                                                                                                   BORDRO_BILGILERI->bordro_donemi_ay ) * PERSONEL->ODENEKLER[ i ].yuzde_orani  );
        }

        /*  yemek parasi gunluk verildigi icin calistigi gun * yemek parasi
            Brüt Günlük Asgari Ücret X  % 6 X (Ay içinde Fiilen Çalışılan gün sayısı) Yemek
            Parası Verilen Gün Sayısı = İstisna Tutarı,

            Ödenen Yemek Parası – İstisna Tutarı = Prime Esas Kazanca Dahil Edilecek Yemek Parası,

            bu indirim sadece ssk matrahini etkiler
        */

        double istisna_tutari = 0.00;
        if ( PERSONEL->ODENEKLER[ i ].odenek_adi EQ tr("Yemek Parasi") ) {

            PERSONEL->ODENEKLER[ i ].brut_tutari = ROUND( PERSONEL->yemek_yardimi_tutari * PERSONEL->yemek_verilen_gun_sayisi );
            istisna_tutari = ROUND(( G_PRS_SABITLER.ssk_taban_degeri / 30.0 ) * G_PRS_SABITLER.gunluk_yemek_yardimi_yuzdesi * PERSONEL->yemek_verilen_gun_sayisi);
        }
        if ( PERSONEL->ODENEKLER[ i ].ssk_kesintisi_iceriyor_mu EQ 1 ) {         

             PERSONEL->ODENEKLER[ i ].ssk_kesinti_matrahi    = PERSONEL->ODENEKLER[ i ].brut_tutari - istisna_tutari;
             PERSONEL->ODENEKLER[ i ].ssk_kesinti_tutari     = ROUND( PERSONEL->ODENEKLER[ i ].ssk_kesinti_matrahi   * G_PRS_SABITLER.ssk_primi_isci_yuzdesi );
             toplam_ssk_kesintisi_matrahi += PERSONEL->ODENEKLER[ i ].ssk_kesinti_matrahi;
        }
        if ( PERSONEL->ODENEKLER[ i ].damga_vergisi_iceriyor_mu EQ 1 ) {
             PERSONEL->ODENEKLER[ i ].damga_vergisi_matrahi  = PERSONEL->ODENEKLER[ i ].brut_tutari;
             PERSONEL->ODENEKLER[ i ].damga_vergisi_tutari   = ROUND( PERSONEL->ODENEKLER[ i ].damga_vergisi_matrahi * G_PRS_SABITLER.damga_vergisi );
             toplam_damga_vergisi_matrahi   += PERSONEL->ODENEKLER[ i ].damga_vergisi_matrahi;
        }
        if ( PERSONEL->ODENEKLER[ i ].gelir_vergisi_iceriyor_mu EQ 1 ) {
             PERSONEL->ODENEKLER[ i ].gelir_vergisi_matrahi   = PERSONEL->ODENEKLER[ i ].brut_tutari;  // BURDA SSK KESINTISI DUSMUYORUZ PERSONEL ICIN TOPLU BIR SEKILDE KESILECEK.
             toplam_gelir_vergisi_matrahi   += PERSONEL->ODENEKLER[ i ].gelir_vergisi_matrahi;
             PERSONEL->ODENEKLER[ i ].gelir_vergisi_tutari = 0.00;
        }

        toplam_odenek += PERSONEL->ODENEKLER[ i ].brut_tutari;

        PERSONEL->ODENEKLER[i].net_tutari = ROUND( PERSONEL->ODENEKLER[i].brut_tutari
                                                 - PERSONEL->ODENEKLER[i].damga_vergisi_tutari
                                                 - PERSONEL->ODENEKLER[i].ssk_kesinti_tutari );

    }

    // KESINTILER HESAPLANIYOR
    double * GELIR_VERGILERI = new double [GELIR_VERGISI_DILIMI_SAYISI];

    PERSONEL->bordro_bir_ay_once_kalan_ek_odenek    = PERSONEL->bir_ay_once_kalan_ek_odenek;
    PERSONEL->bordro_iki_ay_once_kalan_ek_odenek    = PERSONEL->iki_ay_once_kalan_ek_odenek;

    // SSK MATRAHI ONCE KI DONEMLERDEN VARSA EKLENIR EGER ONCEKI AYLAR YETMIYOR ISE BU AYDANDA EKLEMEYAPILIR
    // ONCELIK ONCEKI AYLARDAN KALAN EK ODENEKLERIN VERGILENDIRILMESIDIR.
    PERSONEL->ssk_matrahi = PERSONEL->brut_ucret;

    // [ 1 ]
    // ssk tavan degerinden kucukse
    if ( PERSONEL->ssk_matrahi < G_PRS_SABITLER.ssk_tavan_degeri ) {
        // Karttaki 2. aydan kalana bak
        double eklenecek_fark_miktari = G_PRS_SABITLER.ssk_tavan_degeri - PERSONEL->ssk_matrahi;
        double birinci_aydan_kesilen = 0.00;

        if ( eklenecek_fark_miktari <= PERSONEL->iki_ay_once_kalan_ek_odenek ) {
            PERSONEL->ssk_matrahi += eklenecek_fark_miktari;
        }
        else {
            // ikinci aydan kalan  odenekler kullanildi.
            PERSONEL->ssk_matrahi  += PERSONEL->iki_ay_once_kalan_ek_odenek;
            eklenecek_fark_miktari -= PERSONEL->iki_ay_once_kalan_ek_odenek;

            if ( eklenecek_fark_miktari > PERSONEL->bir_ay_once_kalan_ek_odenek ) {
                PERSONEL->ssk_matrahi += PERSONEL->bir_ay_once_kalan_ek_odenek;
                birinci_aydan_kesilen = PERSONEL->bir_ay_once_kalan_ek_odenek;
            }
            else {
                PERSONEL->ssk_matrahi += eklenecek_fark_miktari;
                birinci_aydan_kesilen  = eklenecek_fark_miktari;
            }
        }

        PERSONEL->bir_ay_once_kalan_ek_odenek -= birinci_aydan_kesilen;
        PERSONEL->iki_ay_once_kalan_ek_odenek  = 0.00;

        // bir sonraki aya aktariliyor.
        PERSONEL->iki_ay_once_kalan_ek_odenek  = PERSONEL->bir_ay_once_kalan_ek_odenek;
        PERSONEL->bir_ay_once_kalan_ek_odenek  = 0.00;

        if ( PERSONEL->ssk_matrahi < G_PRS_SABITLER.ssk_tavan_degeri ) {
            eklenecek_fark_miktari = G_PRS_SABITLER.ssk_tavan_degeri - PERSONEL->ssk_matrahi;

            if ( eklenecek_fark_miktari > toplam_ssk_kesintisi_matrahi ) {
                PERSONEL->ssk_matrahi += toplam_ssk_kesintisi_matrahi;
            }
            else {
                PERSONEL->ssk_matrahi += eklenecek_fark_miktari;
                PERSONEL->bir_ay_once_kalan_ek_odenek   = toplam_ssk_kesintisi_matrahi - eklenecek_fark_miktari;
            }
        }
    }
    else {
        double odenmeyen_ssk_matrahi = 0.00;

        odenmeyen_ssk_matrahi    = PERSONEL->ssk_matrahi - G_PRS_SABITLER.ssk_tavan_degeri;
        odenmeyen_ssk_matrahi   += toplam_ssk_kesintisi_matrahi;
        // ek odenek varsa ataniyor
        PERSONEL->iki_ay_once_kalan_ek_odenek   = PERSONEL->bir_ay_once_kalan_ek_odenek;
        PERSONEL->bir_ay_once_kalan_ek_odenek   = odenmeyen_ssk_matrahi;

        PERSONEL->ssk_matrahi   = G_PRS_SABITLER.ssk_tavan_degeri;
    }
    // [ /1 ]

    if ( PRS_GET_PERSONEL_EMEKLI_MI( PERSONEL->personel_id ) EQ false ) {
        // ssk prim ucreti
        if( PERSONEL->ssk_matrahi  >= G_PRS_SABITLER.ssk_tavan_degeri) {
            PERSONEL->ssk_primi_tutari = ROUND( G_PRS_SABITLER.ssk_primi_isci_yuzdesi *  PERSONEL->ssk_matrahi );
        }
        else {
            PERSONEL->ssk_primi_tutari = ROUND( G_PRS_SABITLER.ssk_primi_isci_yuzdesi *  PERSONEL->ssk_matrahi );
        }

        // issizlik primi ucreti
        if(PERSONEL->ssk_matrahi >= G_PRS_SABITLER.ssk_tavan_degeri) {
            PERSONEL->issizlik_primi_tutari = ROUND( ( G_PRS_SABITLER.ssk_tavan_degeri) * (G_PRS_SABITLER.issizlik_sig_isci_yuzdesi) );
        }
        else {
            PERSONEL->issizlik_primi_tutari = ROUND( ( PERSONEL->ssk_matrahi ) * ( G_PRS_SABITLER.issizlik_sig_isci_yuzdesi) );
        }
    }
    else { // emekli ise
        // ssk prim ucreti
        if( PERSONEL->ssk_matrahi >= G_PRS_SABITLER.ssk_tavan_degeri) {
            PERSONEL->ssk_primi_tutari = ROUND( G_PRS_SABITLER.prs_sgdp_primi_isci_yuzdesi * PERSONEL->ssk_matrahi );
        }
        else {
            PERSONEL->ssk_primi_tutari = ROUND( G_PRS_SABITLER.prs_sgdp_primi_isci_yuzdesi * PERSONEL->ssk_matrahi );
        }
        // issizlik primi ucreti
        // emeklilerde issizlik primi yoktur.
        PERSONEL->issizlik_primi_tutari = 0.00;
    }

    // Gelir vergi matrahi ,gelir vergi tutari
    if( PRS_GET_OZEL_DURUM_TURU_ENUM( PERSONEL->personel_id ) NE 0 ) {
        PERSONEL->gelir_vergi_matrahi  = ROUND( PERSONEL->brut_ucret + toplam_gelir_vergisi_matrahi
                                                 - PERSONEL->ssk_primi_tutari
                                                 - PERSONEL->issizlik_primi_tutari );

        PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA( PERSONEL->personel_id ,PERSONEL->gelir_vergi_matrahi,
                                                                                          &( PERSONEL->bordro_oncesi_kumulatif_vergi_matrahi ));

        for ( int i = 0; i < G_PRS_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi; i++ ) {
             GELIR_VERGILERI[i] = PERSONEL->gelir_vergisi_tutari[i];
        }
    }
    else {
        PERSONEL->gelir_vergi_matrahi   = ROUND(PERSONEL->brut_ucret + toplam_gelir_vergisi_matrahi
                                                 - PERSONEL->ssk_primi_tutari
                                                 - PERSONEL->issizlik_primi_tutari);

        PERSONEL->gelir_vergisi_tutari = PRS_GELIR_VERGISI_TUTARINI_HESAPLA ( PERSONEL->personel_id, PERSONEL->gelir_vergi_matrahi,
                                                                                           &( PERSONEL->bordro_oncesi_kumulatif_vergi_matrahi) );

        for ( int i = 0; i < G_PRS_SABITLER.gecerli_gelir_vergisi_dilimi_sayisi; i++ ) {
             GELIR_VERGILERI[i] = PERSONEL->gelir_vergisi_tutari[i];
        }

    }

    // EK KESINTILER
    PERSONEL->toplam_ozel_kesintiler_tutari = 0.00;
    for ( int i = 0;  i < PERSONEL->kesinti_sayisi; i++ ) {
        PERSONEL->toplam_ozel_kesintiler_tutari += PERSONEL->KESINTILER[ i ].tutari;
    }

    PERSONEL->bordro_sonrasi_kumulatif_vergi_matrahi  = PERSONEL->bordro_oncesi_kumulatif_vergi_matrahi;
    PERSONEL->bordro_oncesi_kumulatif_vergi_matrahi   = PERSONEL->bordro_oncesi_kumulatif_vergi_matrahi - PERSONEL->gelir_vergi_matrahi;

    PERSONEL->damga_vergisi_tutari  = ROUND( ( PERSONEL->brut_ucret + toplam_damga_vergisi_matrahi ) * G_PRS_SABITLER.damga_vergisi );

    double toplam_gelir_vergisi_tutari = 0.00;
    for ( int i = 0; i < GELIR_VERGISI_DILIMI_SAYISI; i++ ) {
        toplam_gelir_vergisi_tutari += PERSONEL->gelir_vergisi_tutari[ i ];
    }
    PERSONEL->asg_gec_indirimi_tutari = ROUND( PRS_ASGARI_GECIM_INDIRIMI_HESAPLA ( PERSONEL->personel_id ));

    if ( PERSONEL->asg_gec_indirimi_tutari >  toplam_gelir_vergisi_tutari ) {
        PERSONEL->asg_gec_indirimi_tutari = ROUND ( toplam_gelir_vergisi_tutari );
    }

    PERSONEL->toplam_brut_odenek_tutari = toplam_odenek;

    PERSONEL->toplam_kesinti_tutari =  toplam_gelir_vergisi_tutari
                                       + PERSONEL->damga_vergisi_tutari
                                       + PERSONEL->ssk_primi_tutari
                                       + PERSONEL->issizlik_primi_tutari
                                       - PERSONEL->asg_gec_indirimi_tutari;

    PERSONEL->net_ucret = PERSONEL->brut_ucret + toplam_odenek - PERSONEL->toplam_kesinti_tutari;

    // KESINTI NET MAASTAN DUSULMUSTUR.
    PERSONEL->net_ucret -= PERSONEL->toplam_ozel_kesintiler_tutari;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_BATCH::BORDRO_ALABILIRMI
***************************************************************************************/

int PRS_MAAS_BORDROSU_BATCH::BORDRO_ALABILIRMI()
{
    // hangi ay ?
    int secilen_ay = E9_GET_AYLAR_ENUM( m_ui->combo_box_aylar->currentText() );
    int secilen_yil= QVariant ( m_ui->combo_box_yillar->currentText() ).toInt();
    // ondan sonraki aylar kesilmis mi ?
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT( "prs_bordrolar", "bordro_donemi_ay" );
    query.AND_EKLE      ( "bordro_donemi_yil = :bordro_donemi_yil");
    query.SET_VALUE     ( ":bordro_donemi_yil", secilen_yil );

    if ( query.SELECT("bordro_donemi_ay DESC", 0, 1 ) NE 0 ) {
        query.NEXT();
        int enson_alinan_bordro_ayi = query.VALUE( 0 ).toInt();

        if ( enson_alinan_bordro_ayi > secilen_ay ) {
            MSG_WARNING( tr(" <b>%1</b> Ayına Bordro alındığı için bu Ay için Bordro alamazsınız. \n "
                            "Bordro almak  için %1' ayını iptal etlemisiniz.").arg( E9_GET_AYLAR_STRING( enson_alinan_bordro_ayi )), m_ui->combo_box_aylar );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}
