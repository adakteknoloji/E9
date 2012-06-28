#include <QtGui>
#include <QListWidget>
#include "yonetim.h"
#include "adak_defines.h"
#include "adak_sql.h"
#include "e9_yetki.h"
#include "e9_profil_secme_batch.h"
#include "ui_e9_profil_secme.h"
#include "e9_console_utils.h"
#include <QKeySequence>
#include <QAbstractItemView>

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_E9_PROFIL_SECME
***************************************************************************************/

void  OPEN_E9_PROFIL_SECME ( PROFIL_CAGRILAN_YER p_cagrilan_yer, QWidget * parent, bool * p_degisiklik_yapildimi )
{
    E9_PROFIL_SECME * p = new E9_PROFIL_SECME( p_cagrilan_yer, parent, p_degisiklik_yapildimi );
    p->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PROFIL_SECME::PROFIL_SECME
***************************************************************************************/

E9_PROFIL_SECME::E9_PROFIL_SECME ( PROFIL_CAGRILAN_YER p_cagrilan_yer, QWidget *parent, bool * p_degisiklik_yapildimi ) :
    BATCH_KERNEL( parent ), m_ui(new Ui::E9_PROFIL_SECME)
{
    m_ui->setupUi(this);

    m_cagrilan_yer = p_cagrilan_yer;

    m_degisiklik_yapildimi = p_degisiklik_yapildimi;

    *m_degisiklik_yapildimi  = false;

    this->setWindowTitle( QObject::tr( "E9 PROFİL SEÇME" ));

    m_ui->label_sirket_kodu_adi->setText( VERITABANI_ISMI() + " / " + VERITABANI_TANIMI() );

    FILL_LIST_WIDGET( m_ui->listWidget_profil );

    SET_AUTO_EXIT_BATCH(true);

    m_ui->listWidget_profil->setCurrentRow(0);
    m_ui->groupBox_profil_adi->setTitle(QObject::tr( "Komple Ticari Paket Programı" ));
    m_profil_id = 1;

    m_ui->textEdit_profil_aciklama->setText( GET_PROFIL_BILGILERI( m_profil_id ) );

    connect( m_ui->listWidget_profil, SIGNAL( currentRowChanged ( int))                 , this, SLOT( SLOT_CHANGE_CURRENT_ROW(int)) );
    connect( m_ui->listWidget_profil, SIGNAL( itemClicked       ( QListWidgetItem* ) )  , this, SLOT( SLOT_CHANGE_PROFILE( QListWidgetItem * )) );
    connect( m_ui->listWidget_profil, SIGNAL( itemDoubleClicked ( QListWidgetItem* ) )  , this, SLOT( SLOT_DOUBLE_CLICK_PROFILE( QListWidgetItem * )) );

    START_BATCH_KERNEL( this, DB );
}

/**************************************************************************************
                   E9_PROFIL_SECME::SETUP_FORM
***************************************************************************************/

void E9_PROFIL_SECME::SETUP_FORM()
{
    SET_HELP_PAGE  ( "yonetim-islemleri_e9-profil-secme" );
    SET_PAGE_TITLE ( tr ( "E9 Profil Seçme" ) );

    REGISTER_BUTTON_WIDGET(m_ui->widget_batch_button );

    SET_NAME_OF_RUN_BATCH_BUTTON( "Kaydet" );
    SET_ENTER_KEY_FOR_RUN_BATCH(true);

    SET_FIRST_FOCUS_WIDGET(m_ui->listWidget_profil );

}

/**************************************************************************************
                   E9_PROFIL_SECME::CHECK_VAR
***************************************************************************************/
int E9_PROFIL_SECME::CHECK_VAR(QObject *p_object)
{
    Q_UNUSED( p_object );
    return ADAK_OK;
}

/**************************************************************************************
                   E9_PROFIL_SECME::CHECK_RUN
***************************************************************************************/

int E9_PROFIL_SECME::CHECK_RUN()
{

    return ADAK_OK;
}

/**************************************************************************************
                   E9_PROFIL_SECME::RUN_BATCH
***************************************************************************************/

void E9_PROFIL_SECME::RUN_BATCH()
{   
    if ( m_cagrilan_yer EQ ENUM_E9_KULLANICI_YETKILERI ) {
        E9_KULLANICI_STRUCT_GUNCELLE();
    }
    else {
        E9_PROFIL_BILGILERINI_DB_YAZ();
    }

    close();
}

/**************************************************************************************
                   E9_PROFIL_SECME::SLOT_CHANGE_PROFILE
***************************************************************************************/

void E9_PROFIL_SECME::SLOT_CHANGE_PROFILE( QListWidgetItem * current_item )
{
    m_ui->groupBox_profil_adi->setTitle( current_item->text() );

    m_profil_id = GET_PROFIL_TIPI( current_item->text() );

    m_ui->textEdit_profil_aciklama->setText( GET_PROFIL_BILGILERI( m_profil_id ) );
}

/**************************************************************************************
                   E9_PROFIL_SECME::SLOT_DOUBLE_CLICK_PROFILE
***************************************************************************************/

void E9_PROFIL_SECME::SLOT_DOUBLE_CLICK_PROFILE( QListWidgetItem * current_item )
{
    m_profil_id = GET_PROFIL_TIPI( current_item->text() );

    RUN_BATCH();
}

/**************************************************************************************
                   E9_PROFIL_SECME::SLOT_CHANGE_CURRENT_ROW
***************************************************************************************/

void E9_PROFIL_SECME::SLOT_CHANGE_CURRENT_ROW( int p_current_row )
{
    SLOT_CHANGE_PROFILE( m_ui->listWidget_profil->item( p_current_row ) );
}

/**************************************************************************************
                   E9_PROFIL_SECME::GET_PROFIL_TIPI
***************************************************************************************/

int E9_PROFIL_SECME::GET_PROFIL_TIPI( QString p_profil_name )
{
    if ( p_profil_name EQ QObject::tr( "Adres / Etiket / Toplu Mail / Toplu SMS Programı" ) ) {
        return ADRES_ETIKET_TOPLU_MAIL_TOPLU_SMS_PROGRAMI;
    }
    else if ( p_profil_name EQ QObject::tr( "Ön Muhasebe Programı" ) ) {
        return FATURA_CARI_STOK_TAKIP_PROGRAMI;
    }
    else if ( p_profil_name EQ QObject::tr( "Mali Müşavir Programı" ) ) {
        return MALI_MUSAVIR_PROGRAMI;
    }
    else if ( p_profil_name EQ QObject::tr( "Serbest Meslek Programı" ) ) {
        return SERBEST_MESLEK_PROGRAMI;
    }
    else if ( p_profil_name EQ QObject::tr( "Komple Ticari Paket Programı" ) ) {
        return KOMPLE_TICARI_PAKET_PROGRAM;
    }

    return KOMPLE_TICARI_PAKET_PROGRAM;
}

/**************************************************************************************
                   E9_PROFIL_SECME::FILL_LIST_WIDGET
***************************************************************************************/

void E9_PROFIL_SECME::FILL_LIST_WIDGET( QListWidget *p_list_widget )
{
    p_list_widget->clear();

    p_list_widget->addItems( QStringList()  <<  QObject::tr( "Komple Ticari Paket " )
                                            <<  QObject::tr( "Ön Muhasebe Programı" )
                                            <<  QObject::tr( "Mali Müşavir Programı" )
                                            <<  QObject::tr( "Adres / Etiket / Toplu Mail / Toplu SMS Programı" )
                                            <<  QObject::tr( "Serbest Meslek Programı" ));
}

/**************************************************************************************
                   E9_PROFIL_SECME::GET_PROFIL_BILGILERI
***************************************************************************************/

QString E9_PROFIL_SECME::GET_PROFIL_BILGILERI( int p_profil_id )
{
    QString program_icerigi = "";
    if ( p_profil_id EQ FATURA_CARI_STOK_TAKIP_PROGRAMI ) {
        program_icerigi = "E9 ' u Ön Muhasebe Programı olarak kullanmak isteyenler bu seçeneği seçmeli.\n\n"
                          "Fatura , İrsaliye ve Stok Modülleri ile stoklarınızı takip ederken , "
                          "Cari Hesaplar , Çek/Senet ve Banka Modülleri ile de "
                          "borç ve alacaklarınızı takip  edebilirsiniz.\n\n"
                          "Döviz Modülü sayesinde tüm bu işlemleri her türlü dövizle yapabilirsiniz. Üstelik hergün TCMB kurlarını "
                          "otomatik olarak alıp , sizin belirlediğiniz oranda arttırıp azaltır.\n\n"
                          "Perakende Modülü ile de cari hesap açmadığınız işlemleri takip etme imkanı sunar."
                          "";
    }
    if ( p_profil_id EQ ADRES_ETIKET_TOPLU_MAIL_TOPLU_SMS_PROGRAMI ) {
        program_icerigi = "E9 ' u bir Adres / Etiket programı olarak ta kullanabilirsiniz.\n\n"
                          "Bu durumda Adres Modülü dışında diğer tüm modüller gizlenecektir.\n\n"
                          "Adreslerinize etiket veya zarf yazdırabilir , Toplu Mail veya "
                          "Toplu SMS atabilirsiniz.\n\n"
                          "İsterseniz firma içinde kullanmak için telefon fihristi de yazdırabilirsiniz."
                          "";
    }
    if ( p_profil_id EQ MALI_MUSAVIR_PROGRAMI ) {
        program_icerigi = "Serbest Muhasebeci ve Mali Müşavirler bu yapıyı seçmeli.\n\n"
                          "Genel Muhasebe , İşletme Defteri , Personel Bordrosu , Sabit Kıymet/Demirbaş Takibi ve Serbest Meslek Makbuzu modüllerinden oluşur.\n\n"
                          "Yeni şirket/veritabanı açtığınızda daha önce başka şirket için açtığınız hesap planını kopyalama özelliği size zaman kazandıracaktır.\n\n"
                          "Fiş Kopyalama , Mahsup Fişi Dengeleme , KDV Ayırma , Üst Satırdan Kopyalama gibi özellikleri hızlı ve rahat bir çalışma ortamı sağlar.\n\n"
                          "";
    }
    if ( p_profil_id EQ SERBEST_MESLEK_PROGRAMI ) {
        program_icerigi = "Avukatlar , Doktorlar, Mühendisler , Mimarlar vs verdikleri hizmetin karşılığında "
                          "Serbest Meslek Makbuzu kesmek zorundadırlar. \n\n"
                          "E9 ' da Serbest Meslek Makbuzunu bilgisayardan yazdırabilir , hizmet kalitenizi arttırabilirsiniz.\n\n"
                          "Tasarımcı Modülü sayesinde her türlü makbuzu E9 ' da tasarlamak çok kolay. "
                          "Özellikle Laser yazıcı için makbuzu en ince ayrıntısına kadar özelliştirmek mümkün."
                          "";
    }
    if ( p_profil_id EQ KOMPLE_TICARI_PAKET_PROGRAM ) {
        program_icerigi = "E9 çok detaylı bir Ticari Paket Program ' dır. Orta büyüklükteki bir işletmenin tüm ihtiyaçlarına cevap verebilecek şekilde tasarlandı.\n\n"
                          "16 Modülü birbiri ile entegre çalışır. Bir çok işletme 8-10 modüle ihityaç duyar. "
                          "Kullanılmayan modülleri kapatarak E9 ' u sizin istediğiniz şekle sokmak hiç te zor değildir.\n\n"
                          "";
    }
    return program_icerigi;
}

/**************************************************************************************
                   E9_PROFIL_SECME::E9_KULLANICI_STRUCT_GUNCELLE
***************************************************************************************/

void E9_PROFIL_SECME::E9_KULLANICI_STRUCT_GUNCELLE()
{
    E9_KULLANICI_YETKILERI_STRUCT * E9_KULLANICI_YETKILERI = ( E9_KULLANICI_YETKILERI_STRUCT * ) E9_GET_KULLANICI_YETKILERI();

    E9_KULLANICI_YETKILERI->e9_adr_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_dmr_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_dvz_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi= 0;
    E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_irs_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_isl_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_stk_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_sub_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_prk_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_prs_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi = 0;
    E9_KULLANICI_YETKILERI->e9_muh_kullanabilir_mi = 0;

    if ( m_profil_id EQ ADRES_ETIKET_TOPLU_MAIL_TOPLU_SMS_PROGRAMI ) {
        E9_KULLANICI_YETKILERI->e9_adr_kullanabilir_mi = 1;
    }
    else if ( m_profil_id EQ FATURA_CARI_STOK_TAKIP_PROGRAMI ) {
        E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_irs_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_stk_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_prk_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_dvz_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi= 1;
        E9_KULLANICI_YETKILERI->e9_sub_kullanabilir_mi = 1;
    }
    else if ( m_profil_id EQ MALI_MUSAVIR_PROGRAMI ){
        E9_KULLANICI_YETKILERI->e9_prs_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_muh_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_isl_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_dmr_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi= 1;

    }
    else if ( m_profil_id EQ SERBEST_MESLEK_PROGRAMI ) {
        E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi = 1;
    }
    else if ( m_profil_id EQ KOMPLE_TICARI_PAKET_PROGRAM ) {
        E9_KULLANICI_YETKILERI->e9_adr_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_car_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_bnk_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_dmr_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_dvz_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_faiz_kullanabilir_mi= 1;
        E9_KULLANICI_YETKILERI->e9_fat_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_irs_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_isl_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_smm_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_stk_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_sub_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_prk_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_prs_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_cek_kullanabilir_mi = 1;
        E9_KULLANICI_YETKILERI->e9_muh_kullanabilir_mi = 1;
    }

    *m_degisiklik_yapildimi = true;

    accept();
}

/**************************************************************************************
                   E9_PROFIL_SECME::E9_PROFIL_BILGILERINI_DB_YAZ
***************************************************************************************/

void E9_PROFIL_SECME::E9_PROFIL_BILGILERINI_DB_YAZ()
{

    DB->START_TRANSACTION();
    SQL_QUERY query( DB );

    query.PREPARE_UPDATE( "e9_kullanici_yetkileri", "yetki_id",
                          "muh_kullanabilir_mi          ,"
                          "isl_kullanabilir_mi          ,"
                          "adr_kullanabilir_mi          ,"
                          "dvz_kullanabilir_mi          ,"
                          "car_kullanabilir_mi          ,"
                          "cek_kullanabilir_mi          ,"
                          "bnk_kullanabilir_mi          ,"
                          "sub_kullanabilir_mi          ,"
                          "fat_kullanabilir_mi          ,"
                          "irs_kullanabilir_mi          ,"
                          "stk_kullanabilir_mi          ,"
                          "prs_kullanabilir_mi          ,"
                          "dmr_kullanabilir_mi          ,"
                          "smm_makbuzu_kullanabilir_mi  ,"
                          "faiz_kullanabilir_mi         ,"
                          "prk_kullanabilir_mi           ",
                          "kullanici_id = :kullanici_id" );
    query.SET_VALUE     ( ":kullanici_id", KULLANICI_ID() );

    query.SET_VALUE     ( ":muh_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":isl_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":adr_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":dvz_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":car_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":cek_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":bnk_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":sub_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":fat_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":irs_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":stk_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":prs_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":dmr_kullanabilir_mi             ", 0 );
    query.SET_VALUE     ( ":smm_makbuzu_kullanabilir_mi     ", 0 );
    query.SET_VALUE     ( ":faiz_kullanabilir_mi            ", 0 );
    query.SET_VALUE     ( ":prk_kullanabilir_mi             ", 0 );

    query.UPDATE();

    if ( m_profil_id EQ ADRES_ETIKET_TOPLU_MAIL_TOPLU_SMS_PROGRAMI ) {

            query.PREPARE_UPDATE( "e9_kullanici_yetkileri", "yetki_id",
                                  "adr_kullanabilir_mi          ",
                                  "kullanici_id = :kullanici_id" );

            query.SET_VALUE     ( ":kullanici_id", KULLANICI_ID() );
            query.SET_VALUE     ( ":adr_kullanabilir_mi   ", 1 );

            query.UPDATE();

    }
    else if ( m_profil_id EQ FATURA_CARI_STOK_TAKIP_PROGRAMI ) {

        query.PREPARE_UPDATE( "e9_kullanici_yetkileri", "yetki_id",
                              "dvz_kullanabilir_mi          ,"
                              "car_kullanabilir_mi          ,"
                              "cek_kullanabilir_mi          ,"
                              "bnk_kullanabilir_mi          ,"
                              "sub_kullanabilir_mi          ,"
                              "fat_kullanabilir_mi          ,"
                              "irs_kullanabilir_mi          ,"
                              "stk_kullanabilir_mi          ,"
                              "faiz_kullanabilir_mi          ",
                              "kullanici_id = :kullanici_id" );
        query.SET_VALUE     ( ":kullanici_id", KULLANICI_ID() );


        query.SET_VALUE     ( ":dvz_kullanabilir_mi             ", 1 );
        query.SET_VALUE     ( ":car_kullanabilir_mi             ", 1 );
        query.SET_VALUE     ( ":cek_kullanabilir_mi             ", 1 );
        query.SET_VALUE     ( ":bnk_kullanabilir_mi             ", 1 );
        query.SET_VALUE     ( ":sub_kullanabilir_mi             ", 1 );
        query.SET_VALUE     ( ":fat_kullanabilir_mi             ", 1 );
        query.SET_VALUE     ( ":irs_kullanabilir_mi             ", 1 );
        query.SET_VALUE     ( ":stk_kullanabilir_mi             ", 1 );
        query.SET_VALUE     ( ":faiz_kullanabilir_mi            ", 1 );

        query.UPDATE();

    }
    else if ( m_profil_id EQ MALI_MUSAVIR_PROGRAMI ) {

            query.PREPARE_UPDATE( "e9_kullanici_yetkileri", "yetki_id",
                                  "muh_kullanabilir_mi          ,"
                                  "isl_kullanabilir_mi          ,"
                                  "prs_kullanabilir_mi          ,"
                                  "dmr_kullanabilir_mi          ,"
                                  "smm_makbuzu_kullanabilir_mi  ,"
                                  "faiz_kullanabilir_mi         ,"
                                  "fat_kullanabilir_mi           ",

                                  "kullanici_id = :kullanici_id" );
            query.SET_VALUE     ( ":kullanici_id", KULLANICI_ID() );

            query.SET_VALUE     ( ":prs_kullanabilir_mi             ", 1 );
            query.SET_VALUE     ( ":muh_kullanabilir_mi             ", 1 );
            query.SET_VALUE     ( ":isl_kullanabilir_mi             ", 1 );
            query.SET_VALUE     ( ":dmr_kullanabilir_mi             ", 1 );
            query.SET_VALUE     ( ":smm_makbuzu_kullanabilir_mi     ", 1 );
            query.SET_VALUE     ( ":faiz_kullanabilir_mi            ", 1 );
            query.SET_VALUE     ( ":fat_kullanabilir_mi             ", 1 );

            query.UPDATE();


        }
    else if ( m_profil_id EQ SERBEST_MESLEK_PROGRAMI ) {
            SQL_QUERY query( DB );

            query.PREPARE_UPDATE( "e9_kullanici_yetkileri", "yetki_id",
                                  "car_kullanabilir_mi          ,"
                                  "cek_kullanabilir_mi          ,"
                                  "bnk_kullanabilir_mi          ,"
                                  "smm_makbuzu_kullanabilir_mi   ",
                                  "kullanici_id = :kullanici_id" );
            query.SET_VALUE     ( ":kullanici_id", KULLANICI_ID() );

            query.SET_VALUE     ( ":car_kullanabilir_mi             ", 1 );
            query.SET_VALUE     ( ":cek_kullanabilir_mi             ", 1 );
            query.SET_VALUE     ( ":bnk_kullanabilir_mi             ", 1 );
            query.SET_VALUE     ( ":smm_makbuzu_kullanabilir_mi     ", 1 );

            query.UPDATE();

    }
    else if ( m_profil_id EQ KOMPLE_TICARI_PAKET_PROGRAM ) {
        LOAD_E9_KOMPLE_PAKET();
    }

    query.PREPARE_UPDATE( "e9_sabit_degerler", "sabit_deger_id", "program_profil_id", "sabit_deger_id = :sabit_deger_id");
    query.SET_VALUE     ( ":program_profil_id", m_profil_id );
    query.SET_VALUE     ( ":sabit_deger_id"   , 1         );
    query.UPDATE();
    DB->COMMIT_TRANSACTION();

    *m_degisiklik_yapildimi = true;
}

