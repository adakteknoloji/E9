#include <math.h>
#include "adak_sql.h"
#include "adak_utils.h"
#include "print.h"
#include "faiz_taksit_raporu_class.h"
#include "faiz_hesapla_batch_class.h"
#include "ui_faiz_hesapla_batch.h"
#include "faiz_console_utils.h"
#include "faiz_taksit_raporu_open.h"
#include "faiz_enum.h"
 

#define  FRAME_SAYISI            10
#define  FRAMEDEKI_SATIR_SAYISI  12

void OPEN_FAIZ_HESAPLA( int islem_turu, QWidget * parent)
{
    FAIZ_HESAPLA_BATCH * B = new FAIZ_HESAPLA_BATCH( islem_turu, parent);
    B->setModal(false);
    B->EXEC( FULL_SCREEN );
}

/**************************************************/
/*  FAIZ_HESAPLA_BATCH::FAIZ_HESAPLA_BATCH          */
/**************************************************/

FAIZ_HESAPLA_BATCH::FAIZ_HESAPLA_BATCH( int islem_turu, QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::FAIZ_HESAPLA_BATCH)
{
    m_islem_turu    = islem_turu;

    m_ui->setupUi      ( this);
    START_BATCH_KERNEL ( this, NULL );
}

/*****************************************************/
/*         FAIZ_HESAPLA_BATCH::SETUP_FORM             */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::SETUP_FORM()
{

    m_ui->adak_date_baslama_tarihi->MALI_YIL_KISITI_YOK( true );

    switch( m_islem_turu )
    {
        case ANA_PARAYI_BUL :
                                m_ui->label_title->setText( "<center>Ana Parayı Bulma</center>" );
                                // ana para bold yapilacaka

                                m_ui->comma_edit_anapara->setEnabled( false );
                                m_ui->comma_edit_anapara->setStyleSheet(  "font:15pt;color:blue;" );
                                m_ui->label_ana_para->setStyleSheet( "font:15pt;color:blue;" );

                                SET_FIRST_FOCUS_WIDGET( m_ui->comma_edit_aylik_faiz_orani );
                                SET_PAGE_TITLE ( tr ( "FAIZ - ANA PARAYI BULMA" ) );
                                page_title = QString( "ANA PARAYI BULMA" );
                                SET_HELP_PAGE ( tr("faiz-taksit-hesaplama_ana-parayi-bul") );

                                break;
        case AYLIK_FAIZI_BUL:
                                m_ui->label_title->setText( "<center>Aylık Faizi Bulma</center>" );
                                // aylik faiz bold olacak
                                m_ui->comma_edit_aylik_faiz_orani->setStyleSheet( "font:13pt;color:blue;" );

                                m_ui->comma_edit_aylik_faiz_orani->setEnabled( false );
                                SET_FIRST_FOCUS_WIDGET( m_ui->comma_edit_anapara );
                                SET_PAGE_TITLE ( tr("FAIZ - AYLIK FAİZ ORANINI BULMA") );
                                page_title = QString( "AYLIK FAiZ ORANINI BULMA" );
                                SET_HELP_PAGE("faiz-taksit-hesaplama_aylik-faiz-orani-bul");
                                break;

        case TAKSITLENDIR   :
                                m_ui->label_title->setText( "<center>Taksitlendirme</center>" );
                                // faiz miktari
                                m_ui->commaEdit_faiz_tutari->setStyleSheet( "font:13pt;color:blue;");
                                m_ui->comma_edit_toplam_tutar->setStyleSheet( "font:13pt;color:blue;");

                                SET_FIRST_FOCUS_WIDGET( m_ui->comma_edit_anapara );
                                SET_PAGE_TITLE ( tr( "FAIZ - TAKSİTLENDİRME" ) );
                                page_title = QString( "TAKSİTLENDİRME" );
                                SET_HELP_PAGE("faiz-taksit-hesaplama_taksitlendir");
                                break;
        default:
                                break;
    }

    REGISTER_BUTTON_WIDGET(m_ui->widget_batch_button);

    m_faiz_hesaplama_sekli =  AYLIK_FAIZ;

    m_ui->commaEdit_gunluk_faiz->setEnabled( false );

    m_ui->commaEdit_faiz_hesaplama_sayisi->SET_INTEGER( 1 );

    QStringList hesaplama_sekli = FAIZ_GET_HESAPLAMA_TURU_LIST();

    m_ui->comboBox_hesaplama_sekli->addItems( hesaplama_sekli );


    m_ui->comma_edit_anapara->setMaxLength( 10 );
    m_ui->comma_edit_aylik_faiz_orani->setMaxLength( 6 );
    m_ui->comma_edit_toplam_tutar->setMaxLength( 10 );
    m_ui->comma_edit_taksit_sayisi->setMaxLength( 3 );

    //SET_HELP_PAGE ( tr("ana-parayi-bul") );

    SET_NAME_OF_RUN_BATCH_BUTTON("Yazdır");

    SET_ICON_OF_BATCH_BUTTON(":/e9_icons/E9_ICONS/yazdir_ikonu.png");    

    m_ui->comma_edit_taksit_sayisi->VIRGULSUZ_CALIS(true);
    m_ui->comma_edit_taksit_sayisi->SET_PRECISION(0);
    m_ui->comma_edit_taksit_sayisi->setAlignment(Qt::AlignLeft);


    m_ui->commaEdit_faiz_hesaplama_sayisi->VIRGULSUZ_CALIS(true);
    m_ui->commaEdit_faiz_hesaplama_sayisi->SET_PRECISION(0);
    m_ui->commaEdit_faiz_hesaplama_sayisi->setAlignment(Qt::AlignLeft);

    m_ui->commaEdit_gunluk_faiz->VIRGULSUZ_CALIS(true);
    m_ui->commaEdit_gunluk_faiz->SET_PRECISION(4);
    //m_ui->commaEdit_gunluk_faiz->setAlignment(Qt::AlignLeft);

    m_ui->comma_edit_aylik_faiz_orani->SET_RETURN_EMPTY(false);
    m_ui->comma_edit_anapara->SET_RETURN_EMPTY(false);


    // default degerler
    m_ui->comma_edit_taksit_sayisi->SET_INTEGER    ( 5 );
    m_ui->comma_edit_aylik_faiz_orani->SET_DOUBLE  ( 0.00 );
    m_ui->comma_edit_anapara->SET_DOUBLE           ( 0.00 );

    event_started = false;

    m_ui->adak_date_baslama_tarihi->SET_DATE(QDate::currentDate() );

    WIDGETLARI_EKLE     ();
    WIDGETLARI_DUZENLE  ();
    SET_WIDGET_COLORS   ();

    SET_SETTING_NAME    ( "FAIZ_HESAPLA_BATCH" );

    connect             ( m_ui->comma_edit_taksit_sayisi,    SIGNAL ( textChanged ( QString ) ), this, SLOT ( WIDGETLARI_DUZENLE())      );
    connect             ( m_ui->comma_edit_taksit_sayisi,    SIGNAL ( textChanged ( QString ) ), this, SLOT ( SLOT_TAKSITLERI_HESAPLA()) );
    connect             ( m_ui->comma_edit_aylik_faiz_orani, SIGNAL ( textChanged ( QString ) ), this, SLOT ( SLOT_TAKSITLERI_HESAPLA()) );
    connect             ( m_ui->comma_edit_anapara,          SIGNAL ( textChanged ( QString ) ), this, SLOT ( SLOT_TAKSITLERI_HESAPLA()) );
    connect             ( m_ui->comma_edit_aylik_faiz_orani, SIGNAL ( textChanged ( QString ) ), this, SLOT ( FAIZ_ORANLARINI_AYARLA() ) );
}

/*****************************************************/
/*         FAIZ_HESAPLA_BATCH::CHECK_VAR             */
/*****************************************************/

int FAIZ_HESAPLA_BATCH::CHECK_VAR(QObject * object)
{

    if ( object EQ m_ui->pushButton_hesapla )
    {
       SLOT_TAKSITLERI_HESAPLA();
    }
    else if( object EQ m_ui->comboBox_hesaplama_sekli OR object EQ m_ui->commaEdit_faiz_hesaplama_sayisi )
    {

        m_faiz_hesaplama_sekli = FAIZ_GET_HESAPLAMA_TURU_ENUM( m_ui->comboBox_hesaplama_sekli->currentText() );

       if( m_faiz_hesaplama_sekli EQ AYLIK_FAIZ )
        {
           SET_TARIHLER_ENABLE( false );
        }
        else
        {
           SET_TARIHLER_ENABLE( true );
        }

        TUM_GUN_VE_TARIHLERI_AYARLA();

    }
    else if ( object EQ m_ui->comma_edit_aylik_faiz_orani )
    {
        FAIZ_ORANLARINI_AYARLA();
    }
    else if (object EQ m_ui->commaEdit_yillik_faiz )
    {
       double yillik_faiz = m_ui->commaEdit_yillik_faiz->GET_DOUBLE();

       double faiz_orani = 1 + ( yillik_faiz/100.00 );
       double gunluk_faiz_orani = pow( faiz_orani,(1.00/365.00) );

       m_ui->commaEdit_gunluk_faiz->SET_DOUBLE( ROUND( gunluk_faiz_orani ) );

       double aylik_faiz = pow( faiz_orani,(1.00/12.00) );
       m_ui->comma_edit_aylik_faiz_orani->SET_DOUBLE( ROUND( aylik_faiz ) );

    }
    else if (object EQ m_ui->commaEdit_gunluk_faiz )
    {
       double gunluk_faiz = m_ui->commaEdit_gunluk_faiz->GET_DOUBLE();

       double faiz_orani = 1 + ( gunluk_faiz/100.00 );
       double yillik_faiz = pow( faiz_orani, 365);

       m_ui->commaEdit_yillik_faiz->SET_DOUBLE( ROUND( yillik_faiz ) );

       double aylik_faiz = pow( faiz_orani,30 );

       m_ui->comma_edit_aylik_faiz_orani->SET_DOUBLE( ROUND( aylik_faiz ) );

    }

    else if (object EQ m_ui->comma_edit_taksit_sayisi)
    {
        if ( m_ui->comma_edit_taksit_sayisi->GET_INTEGER() >= 100 )
        {
            m_ui->comma_edit_taksit_sayisi->SET_INTEGER( 100 );
        }

        if ( m_islem_turu EQ ANA_PARAYI_BUL)
        {
            ANA_PARA_HESAPLA();
        }
        else if ( m_islem_turu EQ AYLIK_FAIZI_BUL )
        {
            FAIZ_ORANINI_BUL();;
        }
        else if( m_islem_turu EQ TAKSITLENDIR )
        {
            TAKSITLERI_HESAPLA_FUNC();
        }

        TOPLAM_TUTARI_GOSTER();

    }


    else if (object EQ m_ui->adak_date_baslama_tarihi)
    {

        TUM_GUN_VE_TARIHLERI_AYARLA();

    }

    for ( int i = 0 ; i < m_ui->comma_edit_taksit_sayisi->GET_INTEGER(); i++ ) {

        if ( object EQ m_date_edit_tarih.at(i)) {

            int obj_index   = QVariant( sender()->objectName() ).toInt();
            int tarih_artis_miktari = m_ui->commaEdit_faiz_hesaplama_sayisi->GET_INTEGER();

            if( m_faiz_hesaplama_sekli EQ GUNLUK_FAIZ )
            {
                if( obj_index EQ 0)
                {

                    QDate gecici_date = m_date_edit_tarih.at( 0 )->QDATE();
                    int gun_farki   = gecici_date.daysTo( m_ui->adak_date_baslama_tarihi->QDATE() );

                    m_comma_edit_gunler.at( 0 )->SET_INTEGER( gun_farki );

                    for( int i= 1 ; i < m_ui->comma_edit_taksit_sayisi->GET_INTEGER() ; i++ )
                    {
                            m_comma_edit_gunler.at( i )->SET_INTEGER( m_comma_edit_gunler.at( i - 1 )->GET_INTEGER() + tarih_artis_miktari );
                           // m_date_edit_tarih.at( i )->blockSignals(true);
                            m_date_edit_tarih.at( i )->SET_DATE(  m_date_edit_tarih.at( i -1 )->QDATE().addDays(  tarih_artis_miktari ) );
                            //m_date_edit_tarih.at( i )->blockSignals(false);
                    }

                }
                else
                {

                    for( int i= obj_index ; i < m_ui->comma_edit_taksit_sayisi->GET_INTEGER()  ; i++ )
                    {

                            QDate gecici_tarih = m_date_edit_tarih.at( i - 1 )->QDATE();
                            int gun_farki = gecici_tarih.daysTo( m_date_edit_tarih.at( i )->QDATE() );

                            m_comma_edit_gunler.at( i )->SET_INTEGER( m_comma_edit_gunler.at( i - 1 )->GET_INTEGER() + gun_farki );;

                            m_date_edit_tarih.at( i + 1 )->blockSignals(true);
                            m_date_edit_tarih.at( i + 1 )->SET_DATE(m_date_edit_tarih.at( i )->QDATE().addDays( tarih_artis_miktari ) );
                            m_date_edit_tarih.at( i + 1 )->blockSignals(false);

                    }
                }
            }

            if ( m_islem_turu EQ ANA_PARAYI_BUL)
            {
                ANA_PARA_HESAPLA();
            }
            else if ( m_islem_turu EQ AYLIK_FAIZI_BUL )
            {
                FAIZ_ORANINI_BUL();;
            }
            else if( m_islem_turu EQ TAKSITLENDIR )
            {
                TAKSITLERI_HESAPLA_FUNC();
            }

            TOPLAM_TUTARI_GOSTER();
            break;
        }
    }

    SET_WIDGET_COLORS();

    return ADAK_OK;
}

/*****************************************************/
/*       FAIZ_HESAPLA_BATCH::CHECK_RUN              */
/*****************************************************/

int FAIZ_HESAPLA_BATCH::CHECK_RUN()
{
    return ADAK_OK;
}

/*****************************************************/
/*          FAIZ_HESAPLA_BATCH::RUN_BATCH             */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::RUN_BATCH()
{
    int             taksit_sayisi       = m_ui->comma_edit_taksit_sayisi->GET_INTEGER();
    double          anapara             = m_ui->comma_edit_anapara->GET_DOUBLE();
    double          aylik_faiz_orani    = m_ui->comma_edit_aylik_faiz_orani->GET_DOUBLE();
    double          toplam_tutar        = m_ui->comma_edit_toplam_tutar->GET_DOUBLE();

    QList < int >      odeme_gunleri;
    QList < double >   odeme_tutarlari;
    QList < QString >  odeme_tarihleri;

    for (int i = 0 ; i < taksit_sayisi ; i++) {
        odeme_gunleri    << m_comma_edit_gunler.at(i)->GET_INTEGER();
        odeme_tutarlari  << m_comma_edit_miktarlar.at(i)->GET_DOUBLE();
        odeme_tarihleri  << m_date_edit_tarih.at( i )->QDATE().toString( "dd.MMMM.yyyy dddd" );
    }

    OPEN_REPORT_SHOWER(F_FAIZ_TAKSIT_RAPORU(anapara,aylik_faiz_orani,toplam_tutar,odeme_gunleri,odeme_tutarlari, odeme_tarihleri , page_title , m_faiz_hesaplama_sekli ),nativeParentWidget());
}

/*****************************************************/
/*      FAIZ_HESAPLA_BATCH::SLOT_TAKSITLERI_HESAPLA   */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::SLOT_TAKSITLERI_HESAPLA()
{
    if (event_started EQ true) {
        return;
    }

    event_started = true;

    TAKSITLERI_HESAPLA();

    event_started = false;

}

/*****************************************************/
/*       FAIZ_HESAPLA_BATCH::TAKSITLERI_HESAPLA       */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::TAKSITLERI_HESAPLA()
{

    if ( m_ui->comma_edit_taksit_sayisi->GET_INTEGER() EQ 0 ) {
        return;
    }

    int    taksit_sayisi = 0;
    taksit_sayisi = m_ui->comma_edit_taksit_sayisi->GET_INTEGER();

    if (taksit_sayisi EQ 0 OR taksit_sayisi >= m_comma_edit_gunler.size()) {
        return;
    }

    if ( m_islem_turu EQ ANA_PARAYI_BUL)
    {
        ANA_PARA_HESAPLA();
        TOPLAM_TUTARI_GOSTER();
        return;
    }
    else if ( m_islem_turu EQ AYLIK_FAIZI_BUL )
    {
        FAIZ_ORANINI_BUL();
        TOPLAM_TUTARI_GOSTER();
        return;
    }
    else if( m_islem_turu EQ TAKSITLENDIR )
    {
        TAKSITLERI_HESAPLA_FUNC();
        TOPLAM_TUTARI_GOSTER();
    }

    FAIZ_ORANLARINI_AYARLA();
   // TOPLAM_TUTARI_GOSTER();
}

/*****************************************************/
/*       FAIZ_HESAPLA_BATCH::FAIZ_ORANINI_BUL         */
/*****************************************************/

double FAIZ_HESAPLA_BATCH::FAIZ_ORANINI_BUL()
{

    double eklenecek_faiz_orani = 10.00;

    int taksit_sayisi = m_ui->comma_edit_taksit_sayisi->GET_INTEGER();

    double anapara = m_ui->comma_edit_anapara->GET_DOUBLE();
    double tahmini_faiz_orani = 0.00;

   if( m_faiz_hesaplama_sekli EQ AYLIK_FAIZ )
    {

       for ( ;  ; ) {
           double tahmini_anapara = 0.00;
           double faiz_orani = 1 + ( tahmini_faiz_orani/100.00 );
           double aylik_faiz_orani = 1 + ( tahmini_faiz_orani/100.00 );

           for (int i = 0; i < taksit_sayisi ;i++) {
                   faiz_orani = 1.00;
                   for (int j = 0 ; j < m_comma_edit_gunler.at(i)->GET_INTEGER(); j++ )
                   {
                       faiz_orani = faiz_orani * aylik_faiz_orani;
                   }

                   if ( m_comma_edit_miktarlar.at(i)->GET_DOUBLE() NE 0.0 )
                   {
                       double miktar = m_comma_edit_miktarlar.at(i)->GET_DOUBLE();
                       tahmini_anapara += ( miktar / faiz_orani );
                   }
                   else {
                       return 0.00;
                   }
           }

           if (tahmini_anapara < ( anapara + 0.1 ) AND tahmini_anapara > ( anapara - 0.1 ))
           {
               m_ui->comma_edit_aylik_faiz_orani->SET_DOUBLE ( tahmini_faiz_orani );
               break;
           }
           else if (tahmini_anapara > anapara)
           {
               tahmini_faiz_orani  += eklenecek_faiz_orani;
           }
           else if (tahmini_anapara < anapara)
           {
               tahmini_faiz_orani  -= eklenecek_faiz_orani;
               eklenecek_faiz_orani = (eklenecek_faiz_orani / 10.00);
               if (tahmini_faiz_orani < 0)
               {
                   m_ui->comma_edit_aylik_faiz_orani->clear();
                   return 0.00;
               }
           }
       }
    }

    if( m_faiz_hesaplama_sekli EQ GUNLUK_FAIZ )
    {

        for ( ;  ; ) {
            double tahmini_anapara = 0.00;
            double faiz_orani = 1 + ( tahmini_faiz_orani/100.00 );

            double gunluk_faiz_orani;
            gunluk_faiz_orani = pow(faiz_orani,(1.00/30.00));

            for (int i = 0; i < taksit_sayisi ;i++) {
                    faiz_orani = 1.00;
                    for (int j = 0 ; j < m_comma_edit_gunler.at(i)->GET_INTEGER(); j++ ) {
                        faiz_orani = faiz_orani * gunluk_faiz_orani;
                    }

                    if ( m_comma_edit_miktarlar.at(i)->GET_DOUBLE() NE 0.0 ) {
                        double miktar = m_comma_edit_miktarlar.at(i)->GET_DOUBLE();
                        tahmini_anapara += (miktar / faiz_orani );
                    }
                    else {
                        return 0.00;
                    }
            }

            if (tahmini_anapara < ( anapara + 0.1 ) AND tahmini_anapara > ( anapara - 0.1 )) {
                m_ui->comma_edit_aylik_faiz_orani->SET_DOUBLE ( tahmini_faiz_orani );
                break;
            }
            else if (tahmini_anapara > anapara) {
                tahmini_faiz_orani  += eklenecek_faiz_orani;
            }
            else if (tahmini_anapara < anapara) {
                tahmini_faiz_orani  -= eklenecek_faiz_orani;
                eklenecek_faiz_orani = (eklenecek_faiz_orani / 10.00);
                if (tahmini_faiz_orani < 0) {
                    m_ui->comma_edit_aylik_faiz_orani->clear();
                    return 0.00;
                }
            }
        }
    }

    m_ui->comma_edit_aylik_faiz_orani->SET_DOUBLE( tahmini_faiz_orani );

    return tahmini_faiz_orani;
}

/*****************************************************/
/*       FAIZ_HESAPLA_BATCH::WIDGETLARI_DUZENLE       */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::WIDGETLARI_DUZENLE()
{
    bool is_visible;
    int taksit_sayisi = m_ui->comma_edit_taksit_sayisi->GET_INTEGER();

    for (int i = 0; i < ( FRAMEDEKI_SATIR_SAYISI*FRAME_SAYISI ) ; i++)
    {
        if ( i < taksit_sayisi ) {
            is_visible = true;
        }
        else {
            is_visible = false;
        }

        m_comma_edit_gunler.at    ( i )->setVisible( is_visible );
        m_date_edit_tarih.at      ( i )->setVisible( is_visible );
        m_comma_edit_miktarlar.at ( i )->setVisible( is_visible );
        m_combo_box_odeme.at      ( i )->setVisible( is_visible );
        m_tl_label_list.at        ( i )->setVisible( is_visible );
        m_taksit_label_list.at    ( i )->setVisible( is_visible );
    }

    for (int i = 0; i < m_frame_container_list.size() ; i++)
    {
        if (taksit_sayisi > ((i)*FRAMEDEKI_SATIR_SAYISI)) {
            m_frame_container_list.at(i)->setVisible(true);
        }
        else {
            m_frame_container_list.at(i)->setVisible(false);
        }
    }

}

/*****************************************************/
/*       FAIZ_HESAPLA_BATCH::WIDGETLARI_EKLE          */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::WIDGETLARI_EKLE()
{
    int day_index = 1;

    QHBoxLayout * layout = new QHBoxLayout;
    m_ui->widget->setLayout(layout);


    for (int i = 0 ; i < FRAME_SAYISI ; i++) {
        QFrame * frame = new QFrame;
        frame->setFrameStyle(QFrame::Box);

        layout->addWidget( frame );
        layout->addStretch( 20 );
        m_frame_container_list << frame;
    }


    for (int i=0; i < FRAME_SAYISI ; i++)
    {
        QGridLayout * grid_layout = new QGridLayout;
        grid_layout->setObjectName(tr("%1").arg( QVariant((day_index-1)).toString() ) );

        m_grid_layout_list << grid_layout;


        m_frame_container_list.at(i)->setLayout(grid_layout);

        QLabel * label_1 = new QLabel(" ");
        QLabel * label_2 = new QLabel("Ödeme\nPeryodu");
        QLabel * label_3 = new QLabel("Ödeme\nTarihi");
        QLabel * label_4 = new QLabel("Ödeme\nTürü");
        QLabel * label_5 = new QLabel("Ödenecek\nTutar");
        QLabel * label_6 = new QLabel(" ");
        QLabel * label_7 = new QLabel(" ");


        label_1->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        label_2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        label_2->setStyleSheet("color:green;");
        label_4->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        label_5->setStyleSheet("color:green;");
        label_5->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        label_5->setStyleSheet("color:green;");
        label_6->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

        grid_layout->addWidget(label_1,0,0);
        grid_layout->addWidget(label_2,0,1);
        grid_layout->addWidget(label_3,0,2);
        grid_layout->addWidget(label_4,0,3);
        grid_layout->addWidget(label_5,0,4);
        grid_layout->addWidget(label_6,0,5);
        grid_layout->addWidget(label_7,0,6);

        for (int j=0; j < FRAMEDEKI_SATIR_SAYISI ; j++) {
            QCommaEdit * gun = new QCommaEdit;
            gun->SET_PRECISION(0);
            connect( gun, SIGNAL( textEdited( QString ) ), this, SLOT( TARIHLERI_AYARLA() ) );
            gun->setObjectName(tr("%1").arg( QVariant((day_index-1)).toString() ) );
            m_comma_edit_gunler << gun;


            gun->VIRGULSUZ_CALIS(true);
            gun->setAlignment(Qt::AlignLeft);
            gun->SET_INTEGER( day_index );

            QAdakDate * date    = new QAdakDate;
            date->MALI_YIL_KISITI_YOK( true );
            date->SET_DATE(FAIZ_ADD_MONTH( QDate::currentDate(),day_index ) );
            date->setEnabled( false );
            date->setObjectName(tr("%1").arg( QVariant( (day_index-1)).toString() ) );
            date->setMinimumWidth( 242 );
            date->setMaximumWidth( 242 );
            m_date_edit_tarih << date;

            QComboBox * odeme = new QComboBox;
            odeme->addItem("Eşit Taksit");
            odeme->addItem("Sabit Tutar");
            odeme->setCurrentIndex(0);
            connect( odeme, SIGNAL( currentIndexChanged(int) ), this, SLOT( SLOT_TAKSITLERI_HESAPLA() ));
            connect( odeme, SIGNAL( currentIndexChanged(int) ), this, SLOT( REMOVE_COMBO_BOX_COLOR() ));
            m_combo_box_odeme << odeme;
            odeme->setObjectName(tr("%1").arg(QVariant((day_index-1)).toString()));
            odeme->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

            QCommaEdit * miktar = new QCommaEdit;

            miktar->VIRGULSUZ_CALIS(true);
            miktar->SET_PRECISION(2);
            //miktar->setAlignment(Qt::AlignLeft);

            connect(miktar,SIGNAL(textEdited(QString)),this,SLOT(SLOT_TAKSITLERI_HESAPLA()));
            connect(miktar,SIGNAL(textEdited(QString)),this,SLOT(SLOT_ESIT_TAKSITLERI_AYARLA()));
            miktar->setObjectName(tr("%1").arg(QVariant((day_index-1)).toString()));
            m_comma_edit_miktarlar << miktar;

            QLabel * tl_label = new QLabel(tr("TL"));
            m_tl_label_list << tl_label;

            QLabel * taksit_label = new QLabel(tr("%1. Taksit ").arg(QVariant(day_index).toString()));
            m_taksit_label_list << taksit_label;
            taksit_label->setFont(QFont("Sans Serif",10,QFont::Bold));

            grid_layout->addWidget(taksit_label   ,j+1,0);
            grid_layout->addWidget(gun            ,j+1,1);
            grid_layout->addWidget(date           ,j+1,2);
            grid_layout->addWidget(odeme          ,j+1,3);
            grid_layout->addWidget(miktar         ,j+1,4);
            grid_layout->addWidget(tl_label       ,j+1,5);
            grid_layout->addWidget(label_6        ,j+1,6);

            grid_layout->setColumnStretch( 0, 1);
            grid_layout->setColumnStretch( 1, 1);
            grid_layout->setColumnStretch( 2, 3);
            grid_layout->setColumnStretch( 3, 5);
            grid_layout->setColumnStretch( 4, 5);
            grid_layout->setColumnStretch( 5, 0);
            grid_layout->setColumnStretch( 6, 0);
            grid_layout->setColumnStretch( 7, 1);

            grid_layout->setRowStretch( 11, 10 );  // asagidan yukariya ittirmesi icin

            day_index++;
        }


    }
}

/*****************************************************/
/*       FAIZ_HESAPLA_BATCH::REMOVE_COMBO_BOX_COLOR   */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::REMOVE_COMBO_BOX_COLOR()
{
    QComboBox * combo_box = qobject_cast<QComboBox *>(sender());

    if (combo_box->currentIndex() EQ 0) {
        combo_box->setStyleSheet("");
    }
    else {
        combo_box->setStyleSheet("background-color:yellow;");
    }
}

/*****************************************************/
/*       FAIZ_HESAPLA_BATCH::SET_WIDGET_COLORS        */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::SET_WIDGET_COLORS()
{
    if ( m_islem_turu EQ TAKSITLENDIR OR m_islem_turu EQ  ANA_PARAYI_BUL ) {
        m_ui->label_faiz_orani->setStyleSheet("color:red;");
        m_ui->comma_edit_aylik_faiz_orani->setReadOnly(false);
    }
    else {
        m_ui->label_faiz_orani->setStyleSheet("color:black;");
        m_ui->comma_edit_aylik_faiz_orani->setReadOnly(true);
    }

    if ( m_islem_turu EQ TAKSITLENDIR OR m_islem_turu EQ AYLIK_FAIZI_BUL) {
        m_ui->label_ana_para->setStyleSheet("color:red;");
        m_ui->comma_edit_anapara->setReadOnly(false);
    }
    else {
        m_ui->label_ana_para->setStyleSheet("color:black;");
        m_ui->comma_edit_anapara->setReadOnly(true);
    }


    if ( m_islem_turu EQ  ANA_PARAYI_BUL OR m_islem_turu EQ  AYLIK_FAIZI_BUL ) {
        for (int i=0 ; i < m_taksit_label_list.size(); i++) {
            m_taksit_label_list.at(i)->setStyleSheet("color:red;");
            m_comma_edit_miktarlar.at(i)->setReadOnly(false);
        }
    }
    else {
        for (int i=0 ; i < m_taksit_label_list.size(); i++) {
            m_taksit_label_list.at(i)->setStyleSheet("color:black;");
            m_comma_edit_miktarlar.at(i)->setReadOnly(true);
        }
    }
}

/*****************************************************/
/*  FAIZ_HESAPLA_BATCH::SLOT_ESIT_TAKSITLERI_AYARLA   */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::SLOT_ESIT_TAKSITLERI_AYARLA()
{
    if ( m_islem_turu EQ TAKSITLENDIR ) {
        return;
    }

    int obj_index           = QVariant(sender()->objectName()).toInt();
    QCommaEdit * comma_edit = static_cast<QCommaEdit *>(sender());
    double           miktar = comma_edit->GET_DOUBLE();
    if ( m_combo_box_odeme.at(obj_index)->currentIndex() EQ 0) {
        for(int i=0;i < m_combo_box_odeme.size();i++){
            if ( m_combo_box_odeme.at(i)->currentIndex() EQ 0) {
                m_comma_edit_miktarlar.at(i)->blockSignals(true);
                m_comma_edit_miktarlar.at(i)->SET_DOUBLE ( miktar );
                m_comma_edit_miktarlar.at(i)->blockSignals(false);
            }
        }
    }

    //Tamamem dinamik bir hesaplama yapildigi icin bazen yukarda islemler yapilirken toplam tutar,anapara ve faiz oranlari
    //hesaplaniyor. Biz burda hesaplanmamis deger olma ihtimaline karsin tekrar hesaplama yaptiriyoruz.
    TOPLAM_TUTARI_GOSTER();

    if ( m_islem_turu EQ ANA_PARAYI_BUL ) {
        ANA_PARA_HESAPLA();
    }
    else if ( m_islem_turu EQ AYLIK_FAIZI_BUL) {
        FAIZ_ORANINI_BUL();
    } 

}

/*****************************************************/
/*  FAIZ_HESAPLA_BATCH::TOPLAM_TUTARI_GOSTER          */
/*****************************************************/

void FAIZ_HESAPLA_BATCH::TOPLAM_TUTARI_GOSTER()
{
    double toplam_tutar = 0.00;
    int    taksit_sayisi = m_ui->comma_edit_taksit_sayisi->GET_INTEGER();


    for (int i = 0; i < taksit_sayisi ; i++) {
        toplam_tutar += m_comma_edit_miktarlar.at(i)->GET_DOUBLE();
    }

    m_ui->comma_edit_toplam_tutar->SET_DOUBLE(ROUND(toplam_tutar ));


    if( m_ui->comma_edit_toplam_tutar->GET_DOUBLE() > 0.00 OR  m_ui->comma_edit_toplam_tutar->GET_DOUBLE() > m_ui->comma_edit_anapara->GET_DOUBLE() )
    {
        m_ui->commaEdit_faiz_tutari->SET_DOUBLE(  m_ui->comma_edit_toplam_tutar->GET_DOUBLE() - m_ui->comma_edit_anapara->GET_DOUBLE() );
    }

    if ( m_islem_turu EQ AYLIK_FAIZI_BUL )
    {
        if( toplam_tutar < m_ui->comma_edit_anapara->GET_DOUBLE() )
        {
            //m_ui->lineEdit_sonuc->setText( "" );
        }
    }
}

/*****************************************************/
/*  FAIZ_HESAPLA_BATCH::TARIHLERI_AYARLA          */
/*****************************************************/

void  FAIZ_HESAPLA_BATCH::TARIHLERI_AYARLA()
{
    int obj_index     = QVariant(sender()->objectName()).toInt();

    int peryot  = m_ui->commaEdit_faiz_hesaplama_sayisi->GET_INTEGER();

    if( m_faiz_hesaplama_sekli EQ AYLIK_FAIZ )
    {
        if( obj_index EQ 0)
        {

            m_date_edit_tarih.at( 0 )->SET_DATE(FAIZ_ADD_MONTH( m_ui->adak_date_baslama_tarihi->QDATE(), m_comma_edit_gunler.at( 0 )->GET_INTEGER() ) );

            for( int i= 1 ; i < m_ui->comma_edit_taksit_sayisi->GET_INTEGER() ; i++ )
            {

                //m_date_edit_tarih.at( i )->blockSignals(true);
                m_date_edit_tarih.at( i )->SET_DATE( FAIZ_ADD_MONTH( m_date_edit_tarih.at( i - 1 )->QDATE(), peryot ) );
                //m_date_edit_tarih.at( i )->blockSignals(false);

                m_comma_edit_gunler.at( i )->SET_INTEGER( m_comma_edit_gunler.at( i -1 )->GET_INTEGER() + peryot );
            }
        }
        else
        {
            int gun_farki   = m_comma_edit_gunler.at( obj_index )->GET_INTEGER() - m_comma_edit_gunler.at( obj_index -1 )->GET_INTEGER();

            //m_date_edit_tarih.at( obj_index )->blockSignals( true );
            m_date_edit_tarih.at( obj_index )->SET_DATE( FAIZ_ADD_MONTH( m_date_edit_tarih.at( obj_index - 1 )->QDATE(), gun_farki ) );
           // m_date_edit_tarih.at( obj_index )->blockSignals( false );

            for( int i= obj_index + 1 ; i < m_comma_edit_gunler.size(); i++ )
            {
                    //m_date_edit_tarih.at( i )->blockSignals(true);
                    m_date_edit_tarih.at( i )->SET_DATE( FAIZ_ADD_MONTH( m_date_edit_tarih.at( obj_index - 1 )->QDATE(), peryot ) );
                   // m_date_edit_tarih.at( i )->blockSignals(false);

                    // digerleri artiriliyor
                    m_comma_edit_gunler.at( i )->SET_INTEGER( m_comma_edit_gunler.at( i - 1 )->GET_INTEGER() + peryot );
            }
        }

    }
    else if( m_faiz_hesaplama_sekli EQ GUNLUK_FAIZ )
    {
        if( obj_index EQ 0)
        {
            m_date_edit_tarih.at( 0 )->SET_DATE(QDate::currentDate().addDays( m_comma_edit_gunler.at( 0 )->GET_INTEGER() ));
            for( int i= obj_index ; i < m_ui->comma_edit_taksit_sayisi->GET_INTEGER() ; i++ )
            {

                    int gun_farki   = m_comma_edit_gunler.at( i + 1 )->GET_INTEGER() - m_comma_edit_gunler.at( i )->GET_INTEGER();

                    m_date_edit_tarih.at( i + 1 )->blockSignals(true);
                    m_date_edit_tarih.at( i + 1 )->SET_DATE( m_date_edit_tarih.at( i)->QDATE().addDays( gun_farki ) );
                    m_date_edit_tarih.at( i + 1 )->blockSignals(false);

                    m_comma_edit_gunler.at( i + 1)->SET_INTEGER( m_comma_edit_gunler.at( i )->GET_INTEGER() + 30 );
            }
        }
        else
        {
            int gun_farki   = m_comma_edit_gunler.at( obj_index )->GET_INTEGER() - m_comma_edit_gunler.at( obj_index -1 )->GET_INTEGER();
            m_date_edit_tarih.at( obj_index )->blockSignals(true);
            m_date_edit_tarih.at( obj_index )->SET_DATE( m_date_edit_tarih.at( obj_index - 1 )->QDATE().addDays( gun_farki ) );
            m_date_edit_tarih.at( obj_index )->blockSignals(false);

            for( int i= obj_index + 1 ; i < m_comma_edit_gunler.size(); i++ )
            {
                    m_date_edit_tarih.at( i )->blockSignals(true);
                    m_date_edit_tarih.at( i )->SET_DATE( m_date_edit_tarih.at( i )->QDATE().addDays( 30 ) );
                    m_date_edit_tarih.at( i )->blockSignals(false);

                    // digerleri artiriliyor
                    m_comma_edit_gunler.at( i )->SET_INTEGER( m_comma_edit_gunler.at( i-1 )->GET_INTEGER() + 30 );

                    m_date_edit_tarih.at( i )->blockSignals(true);
                    m_date_edit_tarih.at( i )->SET_DATE( m_date_edit_tarih.at( i -1 )->QDATE().addDays( 30 ) );
                    m_date_edit_tarih.at( i )->blockSignals(false);
            }
        }
    }



    if ( m_islem_turu EQ ANA_PARAYI_BUL ) {
        ANA_PARA_HESAPLA();
    }
    else if ( m_islem_turu EQ AYLIK_FAIZI_BUL) {
        FAIZ_ORANINI_BUL();
    }
    else if( m_islem_turu EQ TAKSITLENDIR )
    {
        TAKSITLERI_HESAPLA_FUNC();
    }
    TOPLAM_TUTARI_GOSTER();
}

/**********************************************************/
/*  FAIZ_HESAPLA_BATCH::ANA_PARA_HESAPLA                   */
/**********************************************************/

double FAIZ_HESAPLA_BATCH::ANA_PARA_HESAPLA ()
{
    int    taksit_sayisi = 0;
    double aylik_faiz_orani = 0.00;

    taksit_sayisi    = m_ui->comma_edit_taksit_sayisi->GET_INTEGER();
    aylik_faiz_orani = m_ui->comma_edit_aylik_faiz_orani->GET_DOUBLE();
    double ana_para = 0.00;
    if( m_faiz_hesaplama_sekli EQ AYLIK_FAIZ )
    {
        double faiz_orani = 1 + (aylik_faiz_orani/100.00);
        double aylik_faiz = 1 + (aylik_faiz_orani/100.00);

        for (int i = 0; i < taksit_sayisi  ; i++) {
            faiz_orani = 1.00;

            for ( int j = 0 ; j < m_comma_edit_gunler.at(i)->GET_INTEGER(); j++ ) {
                faiz_orani = faiz_orani * aylik_faiz;
            }

            if ( m_comma_edit_miktarlar.at(i)->GET_DOUBLE() NE 0.0 ) {
                double miktar = m_comma_edit_miktarlar.at(i)->GET_DOUBLE();
                ana_para += (miktar / faiz_orani );
            }
            else {
                break;
            }
        }
    }
    else if( m_faiz_hesaplama_sekli EQ GUNLUK_FAIZ )
    {
        double faiz_orani = 1 + (aylik_faiz_orani/100.00);
        double gunluk_faiz_orani = pow(faiz_orani,(1.00/30.00));



        for (int i = 0; i < taksit_sayisi  ; i++) {
            faiz_orani = 1.00;

            for ( int j = 0 ; j < m_comma_edit_gunler.at(i)->GET_INTEGER(); j++ ) {
                faiz_orani = faiz_orani * gunluk_faiz_orani;
            }

            if ( m_comma_edit_miktarlar.at(i)->GET_DOUBLE() NE 0.0 ) {
                double miktar = m_comma_edit_miktarlar.at(i)->GET_DOUBLE();
                ana_para += (miktar / faiz_orani );
            }
            else {
                break;
            }
        }
    }

    m_ui->comma_edit_anapara->SET_DOUBLE ( ROUND ( ana_para ) );

    event_started = false;
    return ana_para;
}

/**********************************************************/
/*  FAIZ_HESAPLA_BATCH::TUM_GUNLERI_SIFIRLA                  */
/**********************************************************/

void    FAIZ_HESAPLA_BATCH::TUM_GUNLERI_SIFIRLA()
{
    for( int i = 0; i < m_ui->comma_edit_taksit_sayisi->GET_INTEGER(); i++ )
    {
        m_comma_edit_gunler.at( i )->SET_INTEGER( 0 );
    }
}

/**********************************************************/
/*  FAIZ_HESAPLA_BATCH::GUN_VE_TARIHLERI_AYARLA           */
/**********************************************************/

void    FAIZ_HESAPLA_BATCH::TUM_GUN_VE_TARIHLERI_AYARLA ()
{

    m_faiz_hesaplama_sekli = FAIZ_GET_HESAPLAMA_TURU_ENUM( m_ui->comboBox_hesaplama_sekli->currentText() );

    if( m_faiz_hesaplama_sekli EQ AYLIK_FAIZ )
    {
        int peryot = m_ui->commaEdit_faiz_hesaplama_sayisi->GET_INTEGER();

        TUM_GUNLERI_SIFIRLA();

        m_date_edit_tarih.at( 0 )->blockSignals(true);
        m_date_edit_tarih.at( 0 )->SET_DATE( FAIZ_ADD_MONTH( m_ui->adak_date_baslama_tarihi->QDATE(), peryot ) );
        m_date_edit_tarih.at( 0 )->blockSignals(false);

        m_comma_edit_gunler.at( 0 )->SET_INTEGER( peryot );


        for( int i= 1 ; i < FRAME_SAYISI*FRAMEDEKI_SATIR_SAYISI ; i++ )
        {
                m_date_edit_tarih.at( i )->blockSignals(true);
                m_date_edit_tarih.at( i )->SET_DATE( FAIZ_ADD_MONTH( m_date_edit_tarih.at( i - 1 )->QDATE(), peryot ) );
                m_date_edit_tarih.at( i )->blockSignals(false);

                m_comma_edit_gunler.at( i )->SET_INTEGER( m_comma_edit_gunler.at( i - 1)->GET_INTEGER() + peryot );
        }
    }
    else if( m_faiz_hesaplama_sekli EQ GUNLUK_FAIZ )
    {
        int peryot = m_ui->commaEdit_faiz_hesaplama_sayisi->GET_INTEGER();

        TUM_GUNLERI_SIFIRLA();
        m_date_edit_tarih.at( 0 )->blockSignals(true);
        m_date_edit_tarih.at( 0 )->SET_DATE(m_ui->adak_date_baslama_tarihi->QDATE().addDays( peryot ) );
        m_date_edit_tarih.at( 0 )->blockSignals(false);

        m_comma_edit_gunler.at( 0 )->SET_INTEGER( peryot );

        for( int i= 1 ; i < FRAME_SAYISI*FRAMEDEKI_SATIR_SAYISI ; i++ )
        {
               // m_date_edit_tarih.at( i )->blockSignals(true);
                m_date_edit_tarih.at( i )->SET_DATE( m_date_edit_tarih.at( i - 1 )->QDATE().addDays( peryot ) );
                //m_date_edit_tarih.at( i )->blockSignals(false);

                m_comma_edit_gunler.at( i )->SET_INTEGER( m_comma_edit_gunler.at( i - 1)->GET_INTEGER() + peryot );
        }

    }

}

/**********************************************************/
/*  FAIZ_HESAPLA_BATCH::TAKSITLERI_HESAPLA_FUNC           */
/**********************************************************/

void  FAIZ_HESAPLA_BATCH::TAKSITLERI_HESAPLA_FUNC()
{
    int taksit_sayisi   = m_ui->comma_edit_taksit_sayisi->GET_INTEGER();
    double aylik_faiz_orani = 0.00;
    double anapara = 0.00;

    if (m_ui->comma_edit_aylik_faiz_orani->GET_DOUBLE() EQ 0.0 OR
        m_ui->comma_edit_anapara->GET_DOUBLE() EQ 0.0 ) {
        return;
    }

    if( m_faiz_hesaplama_sekli EQ AYLIK_FAIZ )
    {

        aylik_faiz_orani = m_ui->comma_edit_aylik_faiz_orani->GET_DOUBLE();
        anapara = m_ui->comma_edit_anapara->GET_DOUBLE();

        double faiz_orani = 1 + ( aylik_faiz_orani/100.00 );

        double taksit_tutari  = ( anapara * ( aylik_faiz_orani / 100.0 ) ) / ( 1 - ( 1 / ( pow( faiz_orani , taksit_sayisi ) ) ) );

        for (int i = 0; i < taksit_sayisi ; i++ )
        {
            if ( m_combo_box_odeme.at(i)->currentIndex() EQ 0 )
            {
                m_comma_edit_miktarlar.at(i)->clear();
            }
        }
        for (int i = 0; i < taksit_sayisi ; i++ ) {
            if ( m_comma_edit_miktarlar.at(i)->GET_DOUBLE() EQ 0.0 )
            {
                m_comma_edit_miktarlar.at(i)->SET_DOUBLE ( ROUND ( taksit_tutari ) );
            }
        }

    }
    else  if( m_faiz_hesaplama_sekli EQ GUNLUK_FAIZ )
    {
        aylik_faiz_orani = m_ui->comma_edit_aylik_faiz_orani->GET_DOUBLE();
        anapara = m_ui->comma_edit_anapara->GET_DOUBLE();

        double faiz_orani = 1 + (aylik_faiz_orani/100.00);
        double gunluk_faiz_orani;

        gunluk_faiz_orani = pow(faiz_orani,(1.00/30.00));

        double faiz_toplamlari = 0.00;
        double kalanpara = anapara;

        for (int i = 0; i < taksit_sayisi ; i++ ) {
            if ( m_combo_box_odeme.at(i)->currentIndex() EQ 0 ) {
                m_comma_edit_miktarlar.at(i)->clear();
            }
        }

        for (int i = 0; i < taksit_sayisi  ; i++) {
            double faiz_orani = 1.00;

            for (int j = 0 ; j < m_comma_edit_gunler.at(i)->GET_INTEGER(); j++ ) {
                faiz_orani = faiz_orani * gunluk_faiz_orani;
            }

            if ( m_comma_edit_miktarlar.at(i)->GET_DOUBLE() NE 0.0 ) {
                double miktar = m_comma_edit_miktarlar.at(i)->GET_DOUBLE();

                kalanpara = kalanpara - (miktar / faiz_orani);
            }
            else {
                faiz_toplamlari = faiz_toplamlari + (1.00 / faiz_orani);
            }
        }

        double aylik = kalanpara / faiz_toplamlari;

        for (int i = 0; i < m_comma_edit_gunler.at(taksit_sayisi - 1)->GET_INTEGER() ; i++) {
            kalanpara = kalanpara * gunluk_faiz_orani;
        }

        for (int i = 0; i < taksit_sayisi ; i++ ) {
            if ( m_comma_edit_miktarlar.at(i)->GET_DOUBLE() EQ 0.0 ) {
                m_comma_edit_miktarlar.at(i)->SET_DOUBLE ( ROUND ( aylik ) );
            }
        }
    }

}

/**********************************************************/
/*  FAIZ_HESAPLA_BATCH::FAIZ_ORANLARINI_AYARLA           */
/**********************************************************/

void FAIZ_HESAPLA_BATCH::FAIZ_ORANLARINI_AYARLA()
{
    double aylik_faiz = m_ui->comma_edit_aylik_faiz_orani->GET_DOUBLE();


    double gunluk_faiz_orani = ( pow( 1 + aylik_faiz / 100.0 , (1.0 / 30.0) ) - 1 )*100;

    m_ui->commaEdit_gunluk_faiz->SET_DOUBLE(  gunluk_faiz_orani );

    double yillik_faiz_orani = ( pow( ( 1.0 + aylik_faiz/100.0 ),12 ) - 1.0) * 100.0;

    m_ui->commaEdit_yillik_faiz->SET_DOUBLE( ROUND( yillik_faiz_orani ) );

}

/**********************************************************/
/*  FAIZ_HESAPLA_BATCH::SET_TARIHLER_ENABLE               */
/**********************************************************/

void    FAIZ_HESAPLA_BATCH::SET_TARIHLER_ENABLE( bool durum )
{
    for ( int i = 0 ; i < FRAMEDEKI_SATIR_SAYISI*FRAME_SAYISI; i++ )
    {
        m_date_edit_tarih.at( i )->setEnabled( durum );
    }
}



