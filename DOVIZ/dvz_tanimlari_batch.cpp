#include <QLabel>
#include <QLineEdit>
#include "adak_sql.h"
#include "dvz_tanimlari_batch_class.h"
#include "ui_dvz_tanimlari_batch.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "kernel_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_DVZ_TANIMLARI_BATCH
***************************************************************************************/

void OPEN_DVZ_TANIMLARI_BATCH ( QWidget * parent )
{
    DVZ_TANIMLARI_BATCH * F = new DVZ_TANIMLARI_BATCH ( parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   DVZ_TANIMLARI_BATCH::DVZ_TANIMLARI_BATCH
***************************************************************************************/

DVZ_TANIMLARI_BATCH::DVZ_TANIMLARI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::DVZ_TANIMLARI_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   DVZ_TANIMLARI_BATCH::SETUP_FORM
***************************************************************************************/

void DVZ_TANIMLARI_BATCH::SETUP_FORM()
{

    SET_PAGE_TITLE    ( tr ( "DVZ - DÖVİZ TANIMLARI" ) );
    SET_SETTING_NAME  ( "DOVIZ_TANIMLARI_BATCH" );
    SET_HELP_PAGE     ( "doviz-islemleri_doviz-tanimlari" );

    SET_AUTO_EXIT_BATCH ( true );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Kaydet" );

    SET_FIRST_FOCUS_WIDGET( m_ui->comboBox );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "dvz_degiskenler" ,"kullanilan_xml_adresi" );
    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        m_ui->comboBox->setCurrentIndex ( sql_query.VALUE(0).toInt() );
    }

    sql_query.PREPARE_SELECT("dvz_dovizler" , "doviz_kodu,doviz_adi,alis_eklenecek_yuzde,satis_eklenecek_yuzde,dovizi_kullan,doviz_id");


    if ( sql_query.SELECT("doviz_adi ASC") EQ 0 ) {
        return;
    }

    QHBoxLayout * layout = new QHBoxLayout;
    m_ui->widget_dovizler->setLayout ( layout );

    QFrame * frame = new QFrame;
    layout->addWidget ( frame );

    QGridLayout * grid_layout = new QGridLayout;

    int index = 0;

    QLabel * label_dovizi_kullan_str    = new QLabel ( "Dövizi Kullan" );
    QLabel * label_doviz_kodu_str       = new QLabel ( "Döviz Kodu" );
    QLabel * label_doviz_adi_str        = new QLabel ( "Döviz Adı" );
    QLabel * label_doviz_alis_oran_str  = new QLabel ( "Alış-Düşürülecek Oran(%)" );
    QLabel * label_doviz_satis_oran_str = new QLabel ( "Satış-Eklenecek Oran(%)" );

    label_dovizi_kullan_str->setStyleSheet ( "color:green;" );
    label_doviz_kodu_str->setStyleSheet ( "color:green;");
    label_doviz_adi_str->setStyleSheet ( "color:green;" );
    label_doviz_alis_oran_str->setStyleSheet ( "color:green; font:bold;" );
    label_doviz_satis_oran_str->setStyleSheet ( "color:green; font:bold;" );

    grid_layout->addWidget ( label_dovizi_kullan_str      , index,0 );
    grid_layout->addWidget ( new QLabel ( "    " )        , index,1 );
    grid_layout->addWidget ( label_doviz_kodu_str         , index,2 );
    grid_layout->addWidget ( new QLabel ( "    " )        , index,3 );
    grid_layout->addWidget ( label_doviz_adi_str          , index,4 );
    grid_layout->addWidget ( new QLabel ( "    " )        , index,5 );
    grid_layout->addWidget ( label_doviz_alis_oran_str    , index,6 );
    grid_layout->addWidget ( new QLabel ( "    " )        , index,7 );
    grid_layout->addWidget ( label_doviz_satis_oran_str   , index,8 );

    while(sql_query.NEXT()) {
        QCheckBox * check_box_dovizi_kullan  = new QCheckBox;
        check_box_dovizi_kullan->setObjectName ( sql_query.VALUE(5).toString() );
        if ( sql_query.VALUE(4).toInt() EQ 1 ) {
            check_box_dovizi_kullan->setCheckState ( Qt::Checked );
            m_dovizi_kullan_default_values << Qt::Checked;
        }
        else {
            check_box_dovizi_kullan->setCheckState ( Qt::Unchecked );
            m_dovizi_kullan_default_values << Qt::Unchecked;
        }

        m_check_box_dovizi_kullan << check_box_dovizi_kullan;

        QLabel * label_doviz_kodu          = new QLabel ( sql_query.VALUE(0).toString() );
        label_doviz_kodu->setStyleSheet ( "font:bold;" );
        QLabel * label_doviz_adi           = new QLabel ( sql_query.VALUE(1).toString() );
        label_doviz_adi->setStyleSheet ( "font:bold;" );
        QCommaEdit * comma_edit_alis       = new QCommaEdit;
        QCommaEdit * comma_edit_satis      = new QCommaEdit;


        comma_edit_alis->setMaximumSize ( 100,30 );
        comma_edit_satis->setMaximumSize ( 100,30 );
        comma_edit_alis->setMaxLength(5);
        comma_edit_satis->setMaxLength(5);

        comma_edit_alis->SET_PRECISION( 2 );
        comma_edit_satis->SET_PRECISION( 2 );

        grid_layout->addWidget ( check_box_dovizi_kullan      , index+1,0 );
        grid_layout->addWidget ( label_doviz_kodu             , index+1,2 );
        grid_layout->addWidget ( label_doviz_adi              , index+1,4 );
        grid_layout->addWidget ( comma_edit_alis              , index+1,6 );
        grid_layout->addWidget ( comma_edit_satis             , index+1,8 );

        if ( sql_query.VALUE(2).toDouble() EQ 0.00 ) {
            comma_edit_alis->SET_DOUBLE( 2.00 );
        }
        else {
            comma_edit_alis->SET_DOUBLE ( sql_query.VALUE(2).toDouble() );
        }

        if ( sql_query.VALUE(3).toDouble() EQ 0.00 ) {
             comma_edit_satis->SET_DOUBLE( 2.00 );
        }
        else {
            comma_edit_satis->SET_DOUBLE( sql_query.VALUE(3).toDouble() );
        }

        doviz_kodlari            << label_doviz_kodu->text();
        comma_edit_alis_yuzdesi  << comma_edit_alis;
        comma_edit_satis_yuzdesi << comma_edit_satis;

        index++;
    }

    frame->setLayout ( grid_layout );
}

/**************************************************************************************
                   DVZ_TANIMLARI_BATCH::CHECK_VAR
***************************************************************************************/

int DVZ_TANIMLARI_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED ( object );

    DVZ_CLEAR_DOVIZLER_STRUCT();

    return ADAK_OK;
}

/**************************************************************************************
                   DVZ_TANIMLARI_BATCH::CHECK_RUN
***************************************************************************************/

int DVZ_TANIMLARI_BATCH::CHECK_RUN ()
{
    QString mesaj = QObject::tr("Kur bilgileri güncelleştiriliyor.");
    QSplashScreen * splash = CREATE_SPLASH_SCREEN();
    DVZ_DOVIZ_SPLASH_ON(splash,mesaj);

    if  ( m_ui->comboBox->currentIndex() EQ 0 ) {
         DVZ_DOVIZLERI_GUNCELLE (ENUM_TCMB_XML );
    }

    DVZ_DOVIZ_SPLASH_OFF(splash);

    return ADAK_OK;
}

/**************************************************************************************
                   DVZ_TANIMLARI_BATCH::RUN_BATCH
***************************************************************************************/

void DVZ_TANIMLARI_BATCH::RUN_BATCH ()
{
    DB->START_TRANSACTION();

    SQL_QUERY query ( DB );
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_UPDATE ( "dvz_degiskenler"       , "degisken_id", "kullanilan_xml_adresi", "degisken_id = :degisken_id" );
    sql_query.SET_VALUE      ( ":kullanilan_xml_adresi", m_ui->comboBox->currentIndex() );
    sql_query.SET_VALUE      ( ":degisken_id"          , 1);
    sql_query.UPDATE();

    sql_query.PREPARE_SELECT ( "dvz_dovizler","doviz_kodu"  );
    if ( sql_query.SELECT("doviz_adi ASC") EQ 0 ) {
        DB->COMMIT_TRANSACTION();
        return;
    }

    while ( sql_query.NEXT() ) {
        for ( int i = 0 ; i < comma_edit_alis_yuzdesi.size() ; i++  ) {
            if ( sql_query.VALUE(0).toString() EQ doviz_kodlari.at(i) ) {
                double alis_eklenecek_yuzde  = comma_edit_alis_yuzdesi.at(i)->GET_DOUBLE();
                double satis_eklenecek_yuzde = comma_edit_satis_yuzdesi.at(i)->GET_DOUBLE();

                query.PREPARE_UPDATE (   "dvz_dovizler","doviz_id", "alis_eklenecek_yuzde, "
                                         "satis_eklenecek_yuzde", "doviz_kodu=:doviz_kodu" );

                query.SET_VALUE ( ":alis_eklenecek_yuzde"   ,alis_eklenecek_yuzde );
                query.SET_VALUE ( ":satis_eklenecek_yuzde"  ,satis_eklenecek_yuzde );

                query.SET_VALUE    ( ":doviz_kodu",doviz_kodlari.at(i) );
                query.UPDATE();

                break;
            }
        }
    }

    sql_query.PREPARE_UPDATE ( "dvz_dovizler","doviz_id", "dovizi_kullan", "doviz_id=:doviz_id" );

    for ( int i = 0 ; i < m_check_box_dovizi_kullan.size() ; i++ ) {
        if ( m_check_box_dovizi_kullan.at(i)->checkState() NE m_dovizi_kullan_default_values.at(i) ) {
            sql_query.SET_VALUE ( ":doviz_id",m_check_box_dovizi_kullan.at(i)->objectName() );
            if (m_check_box_dovizi_kullan.at(i)->isChecked() EQ true ) {
                sql_query.SET_VALUE ( ":dovizi_kullan",1 );
            }
            else {
                sql_query.SET_VALUE ( ":dovizi_kullan",0 );
            }
            sql_query.UPDATE();
        }
    }

    DB->COMMIT_TRANSACTION();
}
