#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include "print.h"
#include "adak_utils.h"
#include "kernel_utils.h"
#include "dvz_dovizler_batch_class.h"
#include "ui_dvz_dovizler_batch.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "dvz_doviz_kurlari_raporu_open.h"

 

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_DVZ_DOVIZLER_BATCH
***************************************************************************************/

void OPEN_DOVIZLER_BATCH ( QWidget * parent )
{
    DVZ_DOVIZLER_BATCH * F = new DVZ_DOVIZLER_BATCH ( parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   DVZ_DOVIZLER_BATCH::DVZ_DOVIZLER_BATCH
***************************************************************************************/

DVZ_DOVIZLER_BATCH::DVZ_DOVIZLER_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::DVZ_DOVIZLER_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   DVZ_DOVIZLER_BATCH::SETUP_FORM
***************************************************************************************/

void DVZ_DOVIZLER_BATCH::SETUP_FORM()
{
    m_last_created_frame = NULL;
    m_main_layout        = NULL;

    SET_NAME_OF_RUN_BATCH_BUTTON    ( tr ( "Güncelle" ) );

    REGISTER_BUTTON_WIDGET          ( m_ui->widget_batch_buttons );

    SET_PAGE_TITLE                  ( tr ( "DVZ - DÖVİZLER" ) );
    SET_SETTING_NAME                ( "DOVIZ_ANA_MENU" );
    SET_HELP_PAGE                   ( "doviz-islemleri_doviz-kurlari" );

    SET_AUTO_EXIT_BATCH ( true );

    SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_kur_tarihi );

    SQL_QUERY sql_query ( DB );

    DVZ_DOVIZLERI_GUNCELLE ();

    DB->START_TRANSACTION();

    sql_query.PREPARE_INSERT("dvz_degiskenler" , "degisken_id" , "son_guncellestirilme_tarihi");

    sql_query.SET_VALUE      ( ":son_guncellestirilme_tarihi",QDate::currentDate().toString ( "yyyy.MM.dd" ) );
    sql_query.INSERT();
    DB->COMMIT_TRANSACTION();

    setFont ( QFont ( "Monospace" ) );

    sql_query.PREPARE_SELECT ( "dvz_degiskenler", "kullanilan_xml_adresi" );
    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        m_ui->comboBox->setCurrentIndex ( sql_query.VALUE(0).toInt() );
    }

    m_ui->adakDate_kur_tarihi->SET_DATE( QDate::currentDate() ) ;

    m_main_layout = new QHBoxLayout;
    m_ui->widget_dovizler->setLayout ( m_main_layout );

    CREATE_SCREEN();
}

/**************************************************************************************
                   DVZ_DOVIZLER_BATCH::CREATE_SCREEN
***************************************************************************************/

void DVZ_DOVIZLER_BATCH::CREATE_SCREEN ()
{
    SQL_QUERY f_query ( DB );
    SQL_QUERY s_query ( DB );

    QStringList kurlar;

    f_query.PREPARE_SELECT("dvz_dovizler" , "doviz_kodu,doviz_adi,dovizi_kullan,doviz_id" ,
                           "dovizi_kullan = :dovizi_kullan");
    f_query.SET_VALUE(":dovizi_kullan" , 1);


    if ( f_query.SELECT("doviz_adi ASC") EQ 0 ) {
        return;
    }

    s_query.PREPARE_SELECT( "dvz_kurlar", "kur_bilgileri", "kur_tarihi = :kur_tarihi" );
    s_query.SET_VALUE(":kur_tarihi" , m_ui->adakDate_kur_tarihi->DATE());
    if ( s_query.SELECT() NE 0 ) {
        kurlar.clear();
        while ( s_query.NEXT() EQ true ) {
            //QByteArray doviz_bilgisi =  s_query.VALUE(0).toByteArray();
            kurlar << s_query.VALUE(0).toString().split("\t");
        }
    }
    int index = 0;

    if ( m_last_created_frame NE NULL ) {
        m_last_created_frame->setAttribute ( Qt::WA_DeleteOnClose );
        label_alis                 .clear();
        label_satis                .clear();
        label_efektif_alis         .clear();
        label_efektif_satis        .clear();
        comma_edit_alis_degerleri  .clear();
        comma_edit_satis_degerleri .clear();
        doviz_kodlari              .clear();

        m_last_created_frame->close();
        m_last_created_frame = NULL;
    }

    m_last_created_frame = new QFrame;
    m_main_layout->addWidget ( m_last_created_frame );

    QGridLayout * layout_doviz_degerleri    = new QGridLayout;

    QLabel * label_doviz_kodu_str           = new QLabel ( "Döviz Kodu" );
    QLabel * label_doviz_adi_str            = new QLabel ( "Döviz Adı" );
    QLabel * label_doviz_alis_str           = new QLabel ( "Döviz Alış" ) ;
    QLabel * label_doviz_satis_str          = new QLabel ( "Döviz Satış" );
    QLabel * label_doviz_efektif_alis_str   = new QLabel ( "Efektif Alış" );
    QLabel * label_doviz_efektif_satis_str  = new QLabel ( "Efektif Satış" );
    QLabel * label_doviz_alis_degeri_str    = new QLabel ( "Alış Değeri" );
    QLabel * label_doviz_satis_degeri_str   = new QLabel ( "Satış Değeri" );

    label_doviz_adi_str->setStyleSheet           ( "color:green;" );
    label_doviz_kodu_str->setStyleSheet          ( "color:green;" );
    label_doviz_alis_str->setStyleSheet          ( "color:green;" );
    label_doviz_satis_str->setStyleSheet         ( "color:green;" );
    label_doviz_efektif_alis_str->setStyleSheet  ( "color:green;" );
    label_doviz_efektif_satis_str->setStyleSheet ( "color:green;" );
    label_doviz_alis_degeri_str->setStyleSheet   ( "color:green; font:bold;" );
    label_doviz_satis_degeri_str->setStyleSheet  ( "color:green; font:bold;" );

    layout_doviz_degerleri->addWidget ( label_doviz_kodu_str          , index,0 );
    layout_doviz_degerleri->addWidget ( new QLabel ( "    " )         , index,1 );
    layout_doviz_degerleri->addWidget ( label_doviz_adi_str           , index,2 );
    layout_doviz_degerleri->addWidget ( new QLabel ( "    " )         , index,3 );
    layout_doviz_degerleri->addWidget ( label_doviz_alis_str          , index,4 );
    layout_doviz_degerleri->addWidget ( new QLabel ( "    " )         , index,5 );
    layout_doviz_degerleri->addWidget ( label_doviz_satis_str         , index,6 );
    layout_doviz_degerleri->addWidget ( new QLabel ( "    " )         , index,7 );
    layout_doviz_degerleri->addWidget ( label_doviz_efektif_alis_str  , index,8 );
    layout_doviz_degerleri->addWidget ( new QLabel ( "    " )         , index,9 );
    layout_doviz_degerleri->addWidget ( label_doviz_efektif_satis_str , index,10 );
    layout_doviz_degerleri->addWidget ( new QLabel ( "    " )         , index,11 );
    layout_doviz_degerleri->addWidget ( label_doviz_alis_degeri_str   , index,12 );
    layout_doviz_degerleri->addWidget ( new QLabel ( "    " )         , index,13 );
    layout_doviz_degerleri->addWidget ( label_doviz_satis_degeri_str  , index,14 );

    while( f_query.NEXT()) {
        QStringList kur_bilgileri;

        QString alis_fiyati       = "0.0";
        QString satis_fiyati      = "0.0";
        QString efk_alis_fiyati   = "0.0";
        QString efk_satis_fiyati  = "0.0";
        QString kull_alis_fiyati  = "0.0";
        QString kull_satis_fiyati = "0.0";

        for ( int i = 0 ; i < kurlar.size() ; i++ ) {
            kur_bilgileri = kurlar.at(i).split("+");
            if ( kur_bilgileri.at(0) EQ f_query.VALUE(0).toString() ) {
                alis_fiyati       = kur_bilgileri.at(2);
                satis_fiyati      = kur_bilgileri.at(3);
                efk_alis_fiyati   = kur_bilgileri.at(4);
                efk_satis_fiyati  = kur_bilgileri.at(5);
                kull_alis_fiyati  = kur_bilgileri.at(6);
                kull_satis_fiyati = kur_bilgileri.at(7);
                break;
            }
        }

        QLabel * label_doviz_kodu  = new QLabel ( f_query.VALUE(0).toString() );
        label_doviz_kodu->setSizePolicy ( QSizePolicy::Fixed,QSizePolicy::Fixed );
        label_doviz_kodu->setStyleSheet ( "font:bold;" );
        QLabel * label_doviz_adi   = new QLabel ( f_query.VALUE(1).toString() );
        label_doviz_adi->setStyleSheet ( "font:bold;" );
        label_doviz_adi->setSizePolicy ( QSizePolicy::Fixed,QSizePolicy::Fixed );
        QLabel * label_doviz_alis  = new QLabel(VIRGUL_EKLE(alis_fiyati,5 ) );
        label_doviz_alis->setSizePolicy ( QSizePolicy::Fixed,QSizePolicy::Fixed );
        QLabel * label_doviz_satis = new QLabel ( VIRGUL_EKLE(satis_fiyati,5 ) );
        label_doviz_satis->setSizePolicy ( QSizePolicy::Fixed,QSizePolicy::Fixed );
        QLabel * label_doviz_efektif_alis  = new QLabel ( VIRGUL_EKLE ( efk_alis_fiyati,5 ) );
        label_doviz_efektif_alis->setSizePolicy ( QSizePolicy::Fixed,QSizePolicy::Fixed );
        QLabel * label_doviz_efektif_satis = new QLabel ( VIRGUL_EKLE(efk_satis_fiyati,5 ) );
        label_doviz_efektif_satis->setSizePolicy ( QSizePolicy::Fixed,QSizePolicy::Fixed );

        if ( label_doviz_alis->text().isEmpty() EQ true ) {
            label_doviz_alis->setText ( "-" );
        }
        if ( label_doviz_satis->text().isEmpty() EQ true ) {
            label_doviz_satis->setText ( "-" );
        }
        if ( label_doviz_efektif_alis->text().isEmpty() EQ true ) {
            label_doviz_efektif_alis->setText ( "-" );
        }
        if ( label_doviz_efektif_satis->text().isEmpty() EQ true ) {
            label_doviz_efektif_satis->setText ( "-" );
        }

        layout_doviz_degerleri->addWidget ( label_doviz_kodu          , index+1,0 );
        layout_doviz_degerleri->addWidget ( label_doviz_adi           , index+1,2 );
        layout_doviz_degerleri->addWidget ( label_doviz_alis          , index+1,4 );
        layout_doviz_degerleri->addWidget ( label_doviz_satis         , index+1,6 );
        layout_doviz_degerleri->addWidget ( label_doviz_efektif_alis  , index+1,8 );
        layout_doviz_degerleri->addWidget ( label_doviz_efektif_satis , index+1,10 );

    //////////////////////////////////////////////////////////////////////////////////

        QCommaEdit * comma_edit_doviz_alis   = new QCommaEdit;
        QCommaEdit * comma_edit_doviz_satis  = new QCommaEdit;

        comma_edit_doviz_alis ->setMaxLength (10);
        comma_edit_doviz_satis->setMaxLength (10);

        label_alis              << label_doviz_alis;
        label_satis             << label_doviz_satis;
        label_efektif_alis      << label_doviz_efektif_alis;
        label_efektif_satis     << label_doviz_efektif_satis;

        comma_edit_alis_degerleri   << comma_edit_doviz_alis;
        comma_edit_satis_degerleri  << comma_edit_doviz_satis;
        doviz_kodlari               << label_doviz_kodu->text();

        comma_edit_doviz_alis  ->SET_PRECISION(5);
        comma_edit_doviz_satis ->SET_PRECISION(5);
        comma_edit_doviz_alis  ->SET_DOUBLE ( kull_alis_fiyati.toDouble() );
        comma_edit_doviz_satis ->SET_DOUBLE ( kull_satis_fiyati.toDouble() );

        layout_doviz_degerleri->addWidget ( comma_edit_doviz_alis   , index+1,12 );
        layout_doviz_degerleri->addWidget ( comma_edit_doviz_satis  , index+1,14 );

        index++;
    }
    layout_doviz_degerleri->addItem ( new QSpacerItem (40,20,QSizePolicy::Fixed,QSizePolicy::Expanding ),layout_doviz_degerleri->rowCount(),0 );

    // Temel para birimi comma editlerini disable et.
    for ( int i = 0 ; i < doviz_kodlari.size() ; i++ ) {
        if (doviz_kodlari.at(i) EQ DVZ_GET_TEMEL_PARA_BIRIMI_KODU()) {
            comma_edit_alis_degerleri.at(i)->setDisabled(true);
            comma_edit_satis_degerleri.at(i)->setDisabled(true);
            break;
        }
    }

    m_last_created_frame->setLayout ( layout_doviz_degerleri );
}

/**************************************************************************************
                   DVZ_DOVIZLER_BATCH::CHECK_VAR
***************************************************************************************/

int DVZ_DOVIZLER_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->adakDate_kur_tarihi) {
        DVZ_DOVIZLERI_GUNCELLE(ENUM_TCMB_XML,m_ui->adakDate_kur_tarihi->QDATE());
         if ( DOVIZLERI_TARIHE_GORE_GOSTER () EQ false ) {
             QString mesaj = QObject::tr("Kur Bilgileri Güncelleştiriliyor...");
             QSplashScreen * splash = CREATE_SPLASH_SCREEN();

             DVZ_DOVIZ_SPLASH_ON(splash , mesaj);
             DVZ_DOVIZ_SPLASH_OFF(splash);
        }
    }
    else if ( object EQ m_ui->toolButton_rapor ) {
        OPEN_REPORT_SHOWER ( OPEN_DVZ_KUR_RAPORU ( m_ui->adakDate_kur_tarihi->DATE() ), nativeParentWidget());
    }

    DVZ_CLEAR_DOVIZLER_STRUCT();

    return ADAK_OK;
}

/**************************************************************************************
                   DVZ_DOVIZLER_BATCH::CHECK_RUN
***************************************************************************************/

int DVZ_DOVIZLER_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   DVZ_DOVIZLER_BATCH::RUN_BATCH
***************************************************************************************/

void DVZ_DOVIZLER_BATCH::RUN_BATCH ()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "dvz_dovizler", "doviz_kodu" );

    if ( sql_query.SELECT("doviz_adi ASC") EQ 0 ) {
        return;
    }

    while ( sql_query.NEXT() ) {
        for ( int i = 0 ; i < comma_edit_alis_degerleri.size() ; i++  ) {
            if ( sql_query.VALUE(0).toString() EQ doviz_kodlari.at(i) ) {
                double yeni_alis_degeri      =  comma_edit_alis_degerleri.at(i)->GET_DOUBLE();
                double yeni_satis_degeri     =  comma_edit_satis_degerleri.at(i)->GET_DOUBLE();

                if (yeni_alis_degeri EQ 0.00) {
                    yeni_alis_degeri = -1;
                }

                if (yeni_satis_degeri EQ 0.00) {
                    yeni_satis_degeri = -1;
                }

                DVZ_UPDATE_DOVIZ_KURLAR(sql_query.VALUE(0).toString(),m_ui->adakDate_kur_tarihi->QDATE(),yeni_alis_degeri,yeni_satis_degeri);

                break;
            }
        }
    }    

    MSG_WARNING( tr ( "Dövizler Kurları Güncellendi." ), NULL );
}

/**************************************************************************************
                   DVZ_DOVIZLER_BATCH::DOVIZLERI_TARIHE_GORE_GOSTER
***************************************************************************************/

bool DVZ_DOVIZLER_BATCH::DOVIZLERI_TARIHE_GORE_GOSTER ()
{
    CREATE_SCREEN();
    return true;
}
