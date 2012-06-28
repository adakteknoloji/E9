#include <cmath>
#include "prs_puantajlari_gor_ve_duzelt_fisi_class.h"
#include "prs_puantajlari_gor_ve_duzelt_fisi_class.h"
#include "prs_puantajlari_gor_ve_duzelt_fisi_open.h"
#include "prs_enum.h"
#include "prs_con_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
 
#include "sube_console_utils.h"
#include "cari_struct.h"

extern ADAK_SQL * DB;



#define ROW_ID_COLUMN                  0
#define RECORD_ID_COLUMN               1
#define ORDER_COLUMN                   2
#define CALISMA_SEKLI                  3
#define GUNLUK_UCRET                   4
#define ADI_COLUMN                     5
#define ALDIGI_UCRET                   6 // Brut
#define EK_ODENEK_TUTARI               7 // Ek odenek Tutari
#define EK_KESINTI_TUTARI              8 // Ek Kesinti tutari
#define KUR_COLUMN                     9
#define TEMEL_PARABIRIMI_UCRET         10
#define YEMEK_VERILEN_GUN_SAYISI       11
#define CALISTIGI_GUN_COLUMN           12



/**************************************************************************************
                   OPEN_PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI
***************************************************************************************/

void OPEN_PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI ( int record_id,int sube_id,int bordro_ayi,int bordro_yili, QWidget * parent )
{
    PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI * F = new PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI ( record_id,sube_id,bordro_ayi,bordro_yili, parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
         PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI
***************************************************************************************/

PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI(int record_id,int sube_id,int bordro_ayi,int bordro_yili, QWidget * parent) :FIS_KERNEL(parent), m_ui ( new Ui::PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI )

{
    m_record_id   = record_id;
    m_sube_id     = sube_id;
    m_bordro_ayi  = bordro_ayi;
    m_bordro_yili = bordro_yili;
    m_doviz_id    = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

    m_ui->setupUi    ( this );

    START_FIS_KERNEL ( this, DB );
 }
 
 /**************************************************************************************
                  PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::SETUP_FORM()
{    
    SET_PAGE_TITLE    ( tr ( "PRS - PUANTAJ TABLOSU" ) );
    SET_HELP_PAGE     (      "maas-bordrosu" );

    SET_SINGLE_RECORD_ID (m_record_id);

    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_button_widget );

    int num_of_columns = 13;

    REGISTER_SAVER_BUTTON( m_ui->push_button_bordro_al );

    REGISTER_TABLE_WIDGET       ( m_ui->table_widget_gun_satirlari , num_of_columns );
    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION ( RECORD_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION  ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN ( 5 );

    SET_FIS_ORDER_DATABASE ("puantaj_bilgileri", "order_number", "personel_id" );

    SET_TABLE_ROW_WIDGETS ( ADI_COLUMN                  , WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS ( CALISTIGI_GUN_COLUMN        , WIDGET_COMMA_EDIT);
    SET_TABLE_ROW_WIDGETS ( ALDIGI_UCRET                , WIDGET_COMMA_EDIT);
    SET_TABLE_ROW_WIDGETS ( EK_ODENEK_TUTARI            , WIDGET_COMMA_EDIT);
    SET_TABLE_ROW_WIDGETS ( EK_KESINTI_TUTARI           , WIDGET_COMMA_EDIT);
    SET_TABLE_ROW_WIDGETS ( KUR_COLUMN                  , WIDGET_COMMA_EDIT);
    SET_TABLE_ROW_WIDGETS ( TEMEL_PARABIRIMI_UCRET      , WIDGET_COMMA_EDIT);
    SET_TABLE_ROW_WIDGETS ( CALISMA_SEKLI               , WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS ( YEMEK_VERILEN_GUN_SAYISI    , WIDGET_COMMA_EDIT);
    SET_TABLE_ROW_WIDGETS ( GUNLUK_UCRET                , WIDGET_COMMA_EDIT);

    SET_FIRST_FOCUS_WIDGET ( m_ui->table_widget_gun_satirlari);

    SET_SETTING_NAME  ("PRS_PUANTAJ_GOR_VE_DUZELT_FISI");

    m_ui->table_widget_gun_satirlari->setHorizontalHeaderLabels (QStringList() << "row_id" << "record_id"
                                                                 << "order_number_column" << tr("") << tr("")<< tr("Adı / Soyadı")
                                                                 << tr( "Brüt/Net Ücret" )<< tr("Ek \nÖdenek Tutarı") << tr("Ek \nKesinti Tutarı")
                                                                 << tr("Kur") << tr( "Personel Ücret" ) << tr("Yemek Verilen\nGün Sayısı ") << tr("Çalıştığı Gün") << "" <<"" << "");

    m_ui->table_widget_gun_satirlari->setColumnWidth( ADI_COLUMN                    , 200 );
    m_ui->table_widget_gun_satirlari->setColumnWidth( ALDIGI_UCRET                  , 100 );
    m_ui->table_widget_gun_satirlari->setColumnWidth( EK_ODENEK_TUTARI              , 100 );
    m_ui->table_widget_gun_satirlari->setColumnWidth( EK_KESINTI_TUTARI             , 100 );
    m_ui->table_widget_gun_satirlari->setColumnWidth( KUR_COLUMN                    , 100 );
    m_ui->table_widget_gun_satirlari->setColumnWidth( TEMEL_PARABIRIMI_UCRET        , 100 );
    m_ui->table_widget_gun_satirlari->setColumnWidth( CALISTIGI_GUN_COLUMN          , 100 );
    m_ui->table_widget_gun_satirlari->setColumnWidth( YEMEK_VERILEN_GUN_SAYISI      , 100 );

    m_ui->lineEdit_ay->setDisabled(true);
    m_ui->lineEdit_yil->setDisabled(true);
    m_ui->lineEdit_sube_adi->setDisabled(true);
    m_ui->lineEdit_sube_kodu->setDisabled(true);

    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_sube_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_sube_adi);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_yil);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_ay);

    QString sube_kodu, sube_adi;

    SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu, &sube_adi );

    m_ui->lineEdit_sube_kodu->setText   ( sube_kodu );
    m_ui->lineEdit_sube_adi->setText    ( sube_adi  );

    m_ui->lineEdit_yil->setText(QVariant(m_bordro_yili).toString());

    m_ui->lineEdit_ay->setText(QDate::longMonthName(m_bordro_ayi));

    FOCUS_FIRST_WIDGET ();
}

/**************************************************************************************
                   PRS_PUANTAJ_GOR_VE_DUZELT_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::NEW_FIS_RECORD
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::NEW_FIS_RECORD()
{

}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::GET_FIS_RECORD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::GET_FIS_RECORD ( int record_id )
{

    SQL_QUERY  query    ( DB );
    SQL_QUERY  s_query  ( DB );

    QString bordro_bas_donemi = PRS_GET_BORDRO_DONEMI_BAS_STRING( m_bordro_yili, m_bordro_ayi );
    QString bordro_bts_donemi = PRS_GET_BORDRO_DONEMI_BTS_STRING( m_bordro_yili, m_bordro_ayi );

    // isten ayrilanlar

    query.PREPARE_SELECT("prs_personel_nufus_blgler, prs_personeller, prs_personel_kisisel_blgler " ,
                         "prs_personel_nufus_blgler.personel_id, "
                         "ucret_sekli,                  "
                         "net_ucret,                    "
                         "brut_ucret ,                  "
                         "ise_giris_tarihi,             "
                         "cari_hesap_id,                "
                         "ise_giris_tarihi,             "
                         "calisma_sekli,                "
                         "haftalik_calisilan_gun_sayisi,"
                         "isten_cikis_tarihi",

                         "prs_personel_nufus_blgler.personel_id         = prs_personeller.personel_id               "
                         "AND prs_personel_kisisel_blgler.personel_id   = prs_personeller.personel_id               "
                         "AND sube_id = :sube_id                              "
                         "AND ise_giris_tarihi   <= :ise_giris_tarihi         "
                         "AND isten_cikis_tarihi >= :isten_cikis_tarihi       " );
    query.SET_VALUE(":sube_id"            , m_sube_id);
    query.SET_VALUE(":ise_giris_tarihi"   , bordro_bts_donemi );
    query.SET_VALUE(":isten_cikis_tarihi" , bordro_bas_donemi );

    if ( query.SELECT() EQ 0){
        return ADAK_OK;
    }

    int current_row     = -1;
    int prs_doviz_id    =  0;

    while ( query.NEXT() NE 0 ) {

        int personel_id = query.VALUE("personel_id").toInt();


        int cari_hesap_id           = query.VALUE( "cari_hesap_id").toInt();
        QDate ise_giris_tarihi      = QDate::fromString( query.VALUE( "ise_giris_tarihi" ).toString(), "yyyy.MM.dd" );
        QString isten_cikis_tarihi  = query.VALUE( "isten_cikis_tarihi" ).toString();

        CARI_HESAP_STRUCT hesap_bilgileri;
        CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

        CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri  );

        current_row = ADD_NEW_LINE();

        QLineEdit * line_edit_adi_soyad       = ( QLineEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(current_row ,ADI_COLUMN);

        line_edit_adi_soyad->setText( hesap_bilgileri.cari_hesap_ismi );

        int calisma_sekli                       = query.VALUE("calisma_sekli").toInt();

        prs_doviz_id                            = CARI_GET_PARA_BIRIM_ID( cari_hesap_id );

        int ucret_sekli_enum                    = query.VALUE( "ucret_sekli" ).toInt();
        double net_ucret                        = query.VALUE( "net_ucret"   ).toDouble();
        double brut_ucret                       = query.VALUE( "brut_ucret"  ).toDouble();
        double haftalik_calisilan_gun_sayisi    = query.VALUE( "haftalik_calisilan_gun_sayisi" ).toDouble();

        double prs_personel_ucret = net_ucret;
        if ( ucret_sekli_enum EQ PRS_BRUT ) {
            prs_personel_ucret    = brut_ucret;
        }

        QTableWidgetItem * new_item;

        new_item = new QTableWidgetItem(tr("%1").arg(personel_id));
        m_ui->table_widget_gun_satirlari->setItem(current_row ,ROW_ID_COLUMN ,new_item);

        new_item=new QTableWidgetItem(tr("%1").arg(record_id));
        m_ui->table_widget_gun_satirlari->setItem(current_row ,RECORD_ID_COLUMN ,new_item);

        QLineEdit * line_edit_calisma_sekli  = ( QLineEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(current_row, CALISMA_SEKLI);
        line_edit_calisma_sekli->setText( QVariant ( calisma_sekli ).toString() );

        s_query.PREPARE_SELECT ("prs_puantaj","gun_sayisi ",
                                "personel_id = :personel_id  "
                                "AND bordro_donemi_yil    = :bordro_donemi_yil  "
                                "AND bordro_donemi_ay     = :bordro_donemi_ay   ");
        s_query.SET_VALUE (":personel_id"       ,  personel_id   );
        s_query.SET_VALUE (":bordro_donemi_yil" ,  m_bordro_yili );
        s_query.SET_VALUE (":bordro_donemi_ay"  ,  m_bordro_ayi );

        QCommaEdit * comma_edit_doviz_kuru              = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget( current_row, KUR_COLUMN );
        QCommaEdit * comma_edit_aldigi_ucret            = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget( current_row, ALDIGI_UCRET );
        QCommaEdit * comma_edit_ucret_tutari            = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget( current_row, TEMEL_PARABIRIMI_UCRET );
        QCommaEdit * comma_edit_gun                     = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(current_row,CALISTIGI_GUN_COLUMN);
        QCommaEdit * comma_edit_ek_odenek               = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(current_row,EK_ODENEK_TUTARI);
        QCommaEdit * comma_edit_ek_kesinti              = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(current_row,EK_KESINTI_TUTARI);
        QCommaEdit * comma_edit_gunluk_ucret            = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(current_row,GUNLUK_UCRET   );
        QCommaEdit * comma_edit_yemek_verilen_gun_sayisi= ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(current_row,YEMEK_VERILEN_GUN_SAYISI   );

        comma_edit_yemek_verilen_gun_sayisi->SET_DOUBLE( haftalik_calisilan_gun_sayisi * 4 );

        double eksik_gun_sayisi = PRS_GET_EKSIK_GUN_SAYISI( personel_id, m_bordro_ayi, m_bordro_yili );

        if ( eksik_gun_sayisi > 0.00 ) {

            if ( isten_cikis_tarihi NE "9999.99.99" ) {
               comma_edit_gun->SET_DOUBLE( QDate::fromString( isten_cikis_tarihi, "yyyy.MM.dd").daysInMonth() -  eksik_gun_sayisi );
            }
            else {
                comma_edit_gun->SET_DOUBLE( QDate::fromString( bordro_bts_donemi, "yyyy.MM.dd").daysInMonth() -  eksik_gun_sayisi );
            }
        }
        else {
            if ( isten_cikis_tarihi NE "9999.99.99" ) {
                QDate isten_cikis_date = QDate::fromString( isten_cikis_tarihi, "yyyy.MM.dd");
                comma_edit_gun->SET_DOUBLE( isten_cikis_date.day() );
            }
            else {
                comma_edit_gun->SET_DOUBLE( 30 );
            }
        }

        int gun_sayisi = comma_edit_gun->GET_DOUBLE();
        if ( ise_giris_tarihi.year() EQ m_bordro_yili )  {

            QDate bordro_tarihi;
            if ( ise_giris_tarihi.month() EQ m_bordro_ayi ) {
                int ise_giris_gunu = ise_giris_tarihi.day();

                bordro_tarihi = QDate::fromString( bordro_bts_donemi, "yyyy.MM.dd" );

                if ( isten_cikis_tarihi NE "9999.99.99" ){
                    bordro_tarihi = QDate::fromString( isten_cikis_tarihi, "yyyy.MM.dd" );
                    gun_sayisi = bordro_tarihi.day();
                }
                else {
                   gun_sayisi = bordro_tarihi.daysInMonth();
                }

                gun_sayisi = gun_sayisi - ise_giris_gunu + 1 ;

                comma_edit_gun->SET_INTEGER( gun_sayisi );
            }

            comma_edit_yemek_verilen_gun_sayisi->SET_DOUBLE( haftalik_calisilan_gun_sayisi * floor( comma_edit_gun->GET_DOUBLE() / 7.0 ) );
        }

        // GUNLUK UCRET PUANTAJ'DA GUN DEGISTIRILIRSE OTOMATIK CARPILACAK
        comma_edit_gunluk_ucret->SET_DOUBLE( ROUND( prs_personel_ucret / 30 ));
        if ( calisma_sekli EQ ENUM_GUNLUK ) {
            comma_edit_gunluk_ucret->SET_DOUBLE( prs_personel_ucret );
        }
        if ( calisma_sekli EQ ENUM_AYLIK AND gun_sayisi > 30 ) {
            comma_edit_gun->SET_DOUBLE( 30 );
        }

        if ( calisma_sekli EQ ENUM_GUNLUK ) {
            prs_personel_ucret = ROUND( comma_edit_gun->GET_DOUBLE() * comma_edit_gunluk_ucret->GET_DOUBLE() );
        }

        if ( prs_doviz_id NE DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
            comma_edit_doviz_kuru->setEnabled( true );
        }

        // personel ek odenek bilgileri sorgulaniyor.

        double toplam_ek_odenek  = 0.00;
        double toplam_ek_kesinti = 0.00;

        SQL_QUERY t_query( DB );

        t_query.PREPARE_SELECT( "prs_ek_kesintiler_odenekler",
                                "yuzde_mi, yuzde_orani, tutar, kesinti_odenek_turu ",
                                "personel_id         = :personel_id       AND "
                                "bordro_donemi_yil   = :bordro_donemi_yil AND "
                                "bordro_donemi_ay    = :bordro_donemi_ay      ");

        t_query.SET_VALUE( ":personel_id"           , personel_id       );
        t_query.SET_VALUE( ":bordro_donemi_yil"     , m_bordro_yili     );
        t_query.SET_VALUE( ":bordro_donemi_ay "     , m_bordro_ayi      );

        if ( t_query.SELECT() NE 0 ) {
            while ( t_query.NEXT() EQ true ) {
                int     yuzde_mi            = t_query.VALUE( "yuzde_mi" ).toInt();
                int     kesinti_odenek_turu = t_query.VALUE( "kesinti_odenek_turu" ).toInt();
                double  yuzde_orani         = t_query.VALUE( "yuzde_orani" ).toDouble();
                double  tutar               = t_query.VALUE( "tutar" ).toDouble();

                if ( yuzde_mi EQ 1 ) {
                    if ( kesinti_odenek_turu EQ ENUM_PRS_ODENEK ) {
                        toplam_ek_odenek    += ROUND( prs_personel_ucret * yuzde_orani );
                    }
                    if ( kesinti_odenek_turu EQ ENUM_PRS_KESINTI ) {
                         toplam_ek_kesinti  += ROUND( prs_personel_ucret * yuzde_orani );
                    }
                }
                else {
                    if ( kesinti_odenek_turu EQ ENUM_PRS_ODENEK ) {
                        toplam_ek_odenek    += tutar;
                    }
                    if ( kesinti_odenek_turu EQ ENUM_PRS_KESINTI ) {
                         toplam_ek_kesinti  += tutar;
                    }
                }
            }
        }

        comma_edit_aldigi_ucret->SET_DOUBLE ( prs_personel_ucret );
        comma_edit_ek_odenek->SET_DOUBLE    ( toplam_ek_odenek   );
        comma_edit_ek_kesinti->SET_DOUBLE   ( toplam_ek_kesinti  );
        comma_edit_doviz_kuru->SET_DOUBLE   ( DVZ_DOVIZ_KURUNU_AL( prs_doviz_id, QDate::currentDate(), ENUM_SATIS ) );
        comma_edit_ucret_tutari->SET_DOUBLE ( ROUND( comma_edit_doviz_kuru->GET_DOUBLE() * prs_personel_ucret ) );

        if ( s_query.SELECT() EQ 0 ) {
            DB->START_TRANSACTION();
            QCommaEdit * comma_edit_kur   =  ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(current_row ,KUR_COLUMN);
            SQL_QUERY insert_query(DB);
            insert_query.PREPARE_INSERT("prs_puantaj", "puantaj_id" ,
                                        "personel_id , gun_sayisi ,doviz_kuru,  "
                                        "temel_para_birim_tutari , bordro_donemi_yil, bordro_donemi_ay, yemek_verilen_gun_sayisi ");

            insert_query.SET_VALUE(":personel_id"               , personel_id);
            insert_query.SET_VALUE(":gun_sayisi"                , comma_edit_gun->GET_DOUBLE());
            insert_query.SET_VALUE(":doviz_kuru"                , comma_edit_kur->GET_DOUBLE());
            insert_query.SET_VALUE(":temel_para_birim_tutari"   , comma_edit_ucret_tutari->GET_DOUBLE() );
            insert_query.SET_VALUE(":bordro_donemi_yil"         , m_bordro_yili );
            insert_query.SET_VALUE(":bordro_donemi_ay"          , m_bordro_ayi );
            insert_query.SET_VALUE(":yemek_verilen_gun_sayisi"  , comma_edit_yemek_verilen_gun_sayisi->GET_DOUBLE() );

            insert_query.INSERT();
            DB->COMMIT_TRANSACTION();
        }

    }
     return ADAK_OK;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::SET_LINE_DEFAULTS ( int row_number )
{
   QLineEdit*  line_edit_adi                      = ( QLineEdit  * ) m_ui->table_widget_gun_satirlari->cellWidget(row_number, ADI_COLUMN              );
   QCommaEdit* comma_edit_doviz_kuru              = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(row_number, KUR_COLUMN              );
   QCommaEdit* comma_edit_aldigi_ucret            = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(row_number, ALDIGI_UCRET            );
   QCommaEdit* comma_edit_ek_odenek_tutari        = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(row_number, EK_ODENEK_TUTARI        );
   QCommaEdit* comma_edit_toplam_aldigi_ucret     = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(row_number, EK_KESINTI_TUTARI       );
   QCommaEdit* comma_edit_temel_para_birim_tutari = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(row_number, TEMEL_PARABIRIMI_UCRET  );

   QCommaEdit* comma_edit_gun                     = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(row_number, CALISTIGI_GUN_COLUMN    );
   QCommaEdit* comma_edit_yemek_gun_sayisi        = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget(row_number, YEMEK_VERILEN_GUN_SAYISI);

   comma_edit_yemek_gun_sayisi->SET_PRECISION( 1 );

   comma_edit_doviz_kuru->SET_PRECISION( 4 );
   comma_edit_doviz_kuru->setEnabled( false );

   comma_edit_gun->SET_PRECISION( 1 );

   comma_edit_aldigi_ucret->setEnabled              ( false );
   comma_edit_temel_para_birim_tutari->setEnabled   ( false );
   comma_edit_ek_odenek_tutari->setEnabled          ( false );
   comma_edit_toplam_aldigi_ucret->setEnabled       ( false );

   line_edit_adi->setMaxLength(50);
   line_edit_adi->setEnabled(false);
   comma_edit_gun->setMaxLength(5);
   comma_edit_doviz_kuru->setMaxLength(50);

}



/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    Q_UNUSED ( object );
    return ADAK_OK;
}

/**************************************************************************************
                   PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_FORM_EMPTY()
{

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_RECORD_ADD ()
{

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::ADD_FIS_RECORD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::ADD_FIS_RECORD ()
{
   return 1;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id )
{
    Q_UNUSED(record_id);
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::UPDATE_FIS_RECORD ( int record_id )
{
    Q_UNUSED(record_id);
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    Q_UNUSED(record_id);

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::DELETE_FIS_RECORD ( int record_id )
{
  Q_UNUSED(record_id);
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::SELECT_FIS_RECORD()
{

    return 0;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_FIS_RECORD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_FIS_RECORD()
{

    return 0;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_FIRST_FIS_RECORD()
{

    return 0;

}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_LAST_FIS_RECORD()
{

    return 0;
}

/**************************************************************************************
             PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_PREV_FIS_RECORD()
{

    return 0;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::FIND_NEXT_FIS_RECORD()
{

    return 0;
}



/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_LINE_VAR
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{

    QCommaEdit  * comma_edit_temel_tutari   = ( QCommaEdit  * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number , TEMEL_PARABIRIMI_UCRET );
    QCommaEdit  * comma_edit_ucret          = ( QCommaEdit  * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number , ALDIGI_UCRET);
    QCommaEdit  * comma_edit_kur            = ( QCommaEdit  * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number , KUR_COLUMN);
    QCommaEdit  * comma_edit_gun            = ( QCommaEdit  * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number , CALISTIGI_GUN_COLUMN );
    QCommaEdit  * comma_edit_gunluk_ucret   = ( QCommaEdit  * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number , GUNLUK_UCRET );
    QLineEdit   * line_edit_calisma_sekli   = ( QLineEdit   * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number , CALISMA_SEKLI );

    int           personel_id               =  m_ui->table_widget_gun_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt();

    double doviz_kuru       = comma_edit_kur->GET_DOUBLE();
    double prs_ucret        = comma_edit_ucret->GET_DOUBLE();
    int    calisma_sekli    = line_edit_calisma_sekli->text().toInt();
    double gunluk_ucret     = comma_edit_gunluk_ucret->GET_DOUBLE();

    if ( object EQ comma_edit_kur ) {
        comma_edit_temel_tutari->SET_DOUBLE( ROUND( doviz_kuru* prs_ucret  ));
    }
    else if( object EQ comma_edit_gun ) {
        double gun_sayisi = comma_edit_gun->GET_DOUBLE();

        if ( calisma_sekli EQ ENUM_AYLIK ) {
            if ( gun_sayisi > 30.0 ) {
                MSG_WARNING( tr( "Aylik Calisan Personel 30 Gunden Fazla Çalişamaz." ), comma_edit_gun );
                return ADAK_FAIL_UNDO;
            }
        }
        if ( line_edit_calisma_sekli->text().toInt() EQ ENUM_AYLIK AND gun_sayisi >= 30.0 ) {
            SQL_QUERY query( DB );
            query.PREPARE_SELECT( "prs_personeller","brut_ucret", "personel_id = :personel_id" );
            query.SET_VALUE     ( ":personel_id", personel_id );

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                double aylik_tutar = query.VALUE( 0 ).toDouble();
                comma_edit_ucret->SET_DOUBLE        ( aylik_tutar );
                comma_edit_temel_tutari->SET_DOUBLE ( aylik_tutar );
            }
        }
        else {
            comma_edit_ucret->SET_DOUBLE        ( ROUND( gunluk_ucret * gun_sayisi ));
            comma_edit_temel_tutari->SET_DOUBLE ( ROUND( gunluk_ucret * gun_sayisi ));
        }

    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_LINE_EMPTY ( int row_number )
{

    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_ADD_LINE
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_FAIL;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::ADD_LINE
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::ADD_LINE ( int record_id, int row_number )
{
 Q_UNUSED(record_id);
 Q_UNUSED(row_number);
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::UPDATE_LINE
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);

    SQL_QUERY query(DB);
    QCommaEdit  * comma_edit_temel_tutari               = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number, TEMEL_PARABIRIMI_UCRET     );
    QCommaEdit  * comma_edit_calistigi_gun              = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number, CALISTIGI_GUN_COLUMN       );
    QCommaEdit  * comma_edit_kur                        = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number, KUR_COLUMN                 );
    QCommaEdit  * comma_edit_yemek_verilen_gun_sayisi   = ( QCommaEdit * ) m_ui->table_widget_gun_satirlari->cellWidget( row_number, YEMEK_VERILEN_GUN_SAYISI   );

    int           personel_id                           =  m_ui->table_widget_gun_satirlari->item(row_number,ROW_ID_COLUMN)->text().toInt();

    query.PREPARE_UPDATE( "prs_puantaj","puantaj_id","gun_sayisi, "
                          "doviz_kuru , "
                          "temel_para_birim_tutari, yemek_verilen_gun_sayisi ",
                          "personel_id = :personel_id " );
    query.SET_VALUE (":gun_sayisi "                 , comma_edit_calistigi_gun->GET_DOUBLE());
    query.SET_VALUE (":personel_id"                 , personel_id );
    query.SET_VALUE (":doviz_kuru"                  , comma_edit_kur->GET_DOUBLE());
    query.SET_VALUE (":temel_para_birim_tutari"     , comma_edit_temel_tutari->GET_DOUBLE() );
    query.SET_VALUE (":yemek_verilen_gun_sayisi"    , comma_edit_yemek_verilen_gun_sayisi->GET_DOUBLE() );

    query.UPDATE();
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_FAIL;
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::DELETE_LINE
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::LOCK_FIS_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
    return ADAK_OK;//DB->LOCK_ROW ( "personel_id", "prs_personeller", QString ( "personel_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
    DB->UNLOCK_ROW ( "personel_id", "prs_personeller", QString ( "personel_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void PRS_PUANTAJLARI_GOR_VE_DUZELT_FISI::SAVER_BUTTON_CLICKED (QAbstractButton * button, int record_id )
{
    Q_UNUSED ( record_id );
    if ( button EQ m_ui->push_button_bordro_al ) {
        close();
    }
}


