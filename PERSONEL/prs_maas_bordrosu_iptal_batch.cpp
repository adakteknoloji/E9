#include "prs_maas_bordrosu_iptal_batch_class.h"
#include "prs_maas_bordrosu_iptal_batch_open.h"
#include "print.h"
#include "prs_enum.h"
#include "prs_struct.h"
#include "e9_log.h"
#include "adak_std_utils.h"
#include "sube_console_utils.h"
#include "muh_fis_utils.h"
#include "prs_gui_utils.h"
#include "prs_con_utils.h"
#include "cari_fis_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"

extern ADAK_SQL      *      DB;
extern PRS_SABITLER          G_PRS_SABITLER;

/**************************************************************************************
                   SHOW_PRS_MAAS_BORDROSU_IPTAL_BATCH
***************************************************************************************/

void SHOW_PRS_MAAS_BORDROSU_IPTAL_BATCH (QWidget * parent)
{
    PRS_MAAS_BORDROSU_IPTAL_BATCH * B = new PRS_MAAS_BORDROSU_IPTAL_BATCH(parent);

    B->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_IPTAL_BATCH::PRS_MAAS_BORDROSU_IPTAL_BATCH
**************************************************************************************/

PRS_MAAS_BORDROSU_IPTAL_BATCH::PRS_MAAS_BORDROSU_IPTAL_BATCH(QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::PRS_MAAS_BORDROSU_IPTAL_BATCH)
{
    m_ui->setupUi       ( this);
    START_BATCH_KERNEL  ( this, DB );

}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_IPTAL_BATCH::SETUP_FORM
***************************************************************************************/

void PRS_MAAS_BORDROSU_IPTAL_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE    ( "personel-bordrosu_maas-bordrosu-iptal");
    SET_PAGE_TITLE   ( tr ( "PRS - MAAŞ BORDROSU İPTAL" ) );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ("MAAS BORDROSU İPTAL ET");

    E9_FILL_COMBOBOX_AYLAR ( m_ui->combo_box_aylar );

    PRS_FILL_COMBO_BOX_SUBE_SECIMI ( m_ui->combo_box_subeler ) ;

    COMBO_BOX_IPTAL_EDILECEK_BORDRO_AY_VE_YILI_DOLDUR();

    m_ay       = E9_GET_AYLAR_ENUM( m_ui->combo_box_aylar->currentText() );
    m_yil      = m_ui->combo_box_yillar->currentText().toInt();

    m_sube_id   = SUBE_GET_SUBE_ID();

    m_ui->frame_subeler->setVisible( false );
    if ( SUBE_GET_SUBE_ISLEM_YETKISI() EQ 1 ) {
        m_ui->frame_subeler->setVisible( true );
    }

    SET_FIRST_FOCUS_WIDGET ( m_ui->combo_box_subeler );

    SET_SETTING_NAME        ( "PRS_MAAS_BORDROSU_IPTAL_BATCH" );
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_IPTAL_BATCH::CHECK_VAR
**************************************************************************************/

int PRS_MAAS_BORDROSU_IPTAL_BATCH::CHECK_VAR(QObject * object)
{
    if ( object EQ m_ui->combo_box_aylar ) {
        m_ay = E9_GET_AYLAR_ENUM( m_ui->combo_box_aylar->currentText() );
        return BORDRO_IPTAL_EDILEBILIR_MI();
    }
    else if ( object EQ m_ui->combo_box_yillar ) {
        m_yil = m_ui->combo_box_yillar->currentText().toInt();
    }
    else if ( object EQ m_ui->combo_box_subeler ) {
        int result = SUBE_GET_SUBE_ID_ADI( m_ui->combo_box_subeler->currentText().split("-").at(0) , &m_sube_id );

        if ( result EQ 0 ) {
            return 0;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_IPTAL_BATCH::CHECK_RUN
***************************************************************************************/

int PRS_MAAS_BORDROSU_IPTAL_BATCH::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_IPTAL_BATCH::CHECK_EXIT
***************************************************************************************/

int PRS_MAAS_BORDROSU_IPTAL_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_IPTAL_BATCH::RUN_BATCH
***************************************************************************************/

void PRS_MAAS_BORDROSU_IPTAL_BATCH::RUN_BATCH()
{
    if ( BORDRO_IPTAL_EDILEBILIR_MI() EQ ADAK_FAIL ) {
        return;
    }

    PRS_FILL_SABIT_DEGERLER_STRUCT    ();
    PRS_FILL_FIRMA_ODENEKLERI_STRUCT  ();

    if ( PRS_VERILEN_DONEM_BORDROSU_KESILMIS_MI ( m_ay, m_yil, m_sube_id ) EQ ADAK_FAIL ) {

        MSG_WARNING(  E9_GET_AYLAR_STRING ( m_ay ) + " " + QVariant(m_yil).toString() +
                               tr(" Dönemine Ait Bordro Hazırlanmamıştır <br> ""Lütfen Yeniden Dönem Seçiniz !.."), m_ui->combo_box_aylar );
        return;
    }

    int secim = MSG_YES_NO_CANCEL( E9_GET_AYLAR_STRING ( m_ay ) + " " + QVariant(m_yil).toString() +
                               tr("Dönemine Ait Bordro İptal Edilecektir<br> Emin misiniz?"), NULL );

    if ( secim EQ ADAK_NO OR secim EQ ADAK_CANCEL ) {
        return;
    }

    int bordro_id = PRS_GET_BORDRO_ID ( m_ay ,m_yil, m_sube_id );

    DB->START_TRANSACTION();

    CARI_FIS_STRUCT * CARI_FIS = new CARI_FIS_STRUCT;

    CARI_CLEAR_FIS_STRUCT (CARI_FIS);

    int cari_fis_id = CARI_GET_ENT_FIS_ID( E9_PROGRAMI , PERSONEL_MODULU , bordro_id);

    CARI_FIS_BILGILERINI_OKU ( CARI_FIS ,cari_fis_id );

    CARI_FISI_SIL( CARI_FIS, cari_fis_id );

    MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT ;

    MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

    SQL_QUERY         query ( DB );
    SQL_QUERY         s_query ( DB );

    int muh_fis_id = 0;

    // bir onceki donemin kumulatif vergi matrahini bull
    query.PREPARE_SELECT ("prs_bordro_personelleri","personel_id, brd_oncesi_kum_vergi_matrahi, "
                          "bir_ay_once_kalan_ek_odenek, iki_ay_once_kalan_ek_odenek, muh_fis_id ",
                          "bordro_id = :bordro_id ");

    query.SET_VALUE(":bordro_id" , bordro_id );

    if( query.SELECT() NE 0 ){
        while ( query.NEXT() EQ true ) {
            int     personel_id                     =  query.VALUE( 0 ).toInt();
            double  brd_oncesi_kum_vergi_matrahi    =  query.VALUE( 1 ).toDouble();
            double  bir_ay_once_kalan_ek_odenek     =  query.VALUE( 2 ).toDouble();
            double  iki_ay_once_kalan_ek_odenek     =  query.VALUE( 3 ).toDouble();
                    muh_fis_id                      =  query.VALUE( 4 ).toInt();

            // guncelle
            s_query.PREPARE_UPDATE( "prs_personeller","personel_id",
                                    "kumulatif_vergi_matrahi        , "
                                    "bir_ay_once_kalan_ek_odenek    , "
                                    "iki_ay_once_kalan_ek_odenek      ",
                                    "personel_id =:personel_id");

            s_query.SET_VALUE( ":kumulatif_vergi_matrahi"       , brd_oncesi_kum_vergi_matrahi );
            s_query.SET_VALUE( ":bir_ay_once_kalan_ek_odenek"   , bir_ay_once_kalan_ek_odenek );
            s_query.SET_VALUE( ":iki_ay_once_kalan_ek_odenek"   , iki_ay_once_kalan_ek_odenek );
            s_query.SET_VALUE( ":personel_id"                   , personel_id );
            s_query.UPDATE();

            s_query.PREPARE_UPDATE( "prs_ek_kesintiler_odenekler ","kesinti_odenek_id",
                                    "bordrosu_kesildi_mi, bordro_id",
                                    "personel_id = :personel_id" );

            s_query.SET_VALUE( ":bordrosu_kesildi_mi", HAYIR );
            s_query.SET_VALUE( ":bordro_id"          , 0 );
            s_query.SET_VALUE( ":personel_id"        , personel_id );

            s_query.UPDATE();
        }
    }

    MUH_FIS_BILGILERINI_OKU(MUH_FIS , muh_fis_id);

    MUH_FIS_SIL( MUH_FIS ,muh_fis_id );

    BORDRO_IPTAL_ET ( bordro_id );

    DB->COMMIT_TRANSACTION();

    delete MUH_FIS ;
    delete CARI_FIS;
}

/**************************************************************************************
                   PRS_MAAS_BORDROSU_IPTAL_BATCH::BORDRO_IPTAL_ET
***************************************************************************************/

void PRS_MAAS_BORDROSU_IPTAL_BATCH::BORDRO_IPTAL_ET  ( int p_bordro_id )
{
    SQL_QUERY       sql_query                     ( DB );
    QString         log_detaylari;

    sql_query.PREPARE_DELETE("prs_bordrolar", "bordro_id = :bordro_id");
    sql_query.SET_VALUE     (":bordro_id",    p_bordro_id);

    SQL_QUERY delete_query( DB );

    sql_query.DELETE();

    sql_query.PREPARE_SELECT( "prs_bordro_personelleri","puantaj_id", "bordro_id = :bordro_id" );
    sql_query.SET_VALUE     (":bordro_id",    p_bordro_id);

    if ( sql_query.SELECT() NE 0 ) {

        while ( sql_query.NEXT() EQ true ) {
           int puantaj_id = sql_query.VALUE( 0 ).toInt();

           delete_query.PREPARE_DELETE("prs_puantaj", "puantaj_id = :puantaj_id");
           delete_query.SET_VALUE     (":puantaj_id", puantaj_id);

           delete_query.DELETE();
         }
    }

    sql_query.PREPARE_DELETE("prs_bordro_personelleri","bordro_id = :bordro_id");
    sql_query.SET_VALUE     (":bordro_id",    p_bordro_id);
    sql_query.DELETE();

    sql_query.PREPARE_DELETE("prs_brd_kesinti_odenekleri","bordro_id = :bordro_id");
    sql_query.SET_VALUE     (":bordro_id",    p_bordro_id);
    sql_query.DELETE();

    log_detaylari = E9_GET_AYLAR_STRING ( m_ay ) + " " + QVariant( m_yil ).toString() +
                    " Dönemine Ait Bordro İptal Edilmiştir ";

    E9_LOG_KAYDI_EKLE( PERSONEL_MODULU, LOG_PRS_BORDROLAR, -1 , log_detaylari );

    MSG_INFO( log_detaylari, NULL );

}

/**************************************************************************************************
                   PRS_MAAS_BORDROSU_IPTAL_BATCH::COMBO_BOX_IPTAL_EDILECEK_BORDRO_AY_VE_YILI_DOLDUR
****************************************************************************************************/

void PRS_MAAS_BORDROSU_IPTAL_BATCH::COMBO_BOX_IPTAL_EDILECEK_BORDRO_AY_VE_YILI_DOLDUR ()
{

    PRS_FILL_COMBO_BOX_BORDRO_YILLARI( m_ui->combo_box_yillar );

    int current_month = QDate::currentDate().month();
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->combo_box_aylar, E9_GET_AYLAR_STRING( current_month));

}


/**************************************************************************************************
                PRS_MAAS_BORDROSU_IPTAL_BATCH::BORDRO_IPTAL_EDILEBILIR_MI
****************************************************************************************************/

int PRS_MAAS_BORDROSU_IPTAL_BATCH::BORDRO_IPTAL_EDILEBILIR_MI()
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
            MSG_WARNING( tr(" <b>%1</b> Ayına Bordro alındığı için Bordro İptal edilemez. \n "
                            "Bordro İptal etmek için önce %1' ayını iptal etlemisiniz.").arg( E9_GET_AYLAR_STRING( enson_alinan_bordro_ayi )), m_ui->combo_box_aylar );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}
