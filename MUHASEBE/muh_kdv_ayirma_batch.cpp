#include "adak_sql.h"
#include "muh_kdv_ayirma_batch_class.h"
#include "muh_kdv_ayirma_batch_open.h"
#include "ui_muh_kdv_ayirma_batch.h"
#include "adak_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "e9_log.h"
 

extern          ADAK_SQL       *   DB;

/**************************************************************************************
                   OPEN_KDV_AYIRMA
***************************************************************************************/

bool OPEN_KDV_AYIRMA ( int    p_fis_satiri_id,     double    p_kdv_dahil_tutar, QString p_aciklama,
                       void * P_KDV_AYIRMA_STRUCT, QWidget * p_parent )
{
    MUH_KDV_AYIRMA_STRUCT * KDV_AYIRMA_STRUCT     = ( MUH_KDV_AYIRMA_STRUCT * ) P_KDV_AYIRMA_STRUCT;
    bool                    is_accepted           = false;

    MUH_KDV_AYIRMA_BATCH * B = new MUH_KDV_AYIRMA_BATCH ( p_fis_satiri_id, p_kdv_dahil_tutar, p_aciklama, p_parent);
    B->M_KDV_AYIRMA_STRUCT = KDV_AYIRMA_STRUCT;
    B->m_accepted          = &is_accepted;
    B->EXEC( NOT_FULL_SCREEN );
    return is_accepted;
}

/**************************************************************************************
                   MUH_KDV_AYIRMA_BATCH::MUH_KDV_AYIRMA_BATCH
***************************************************************************************/

MUH_KDV_AYIRMA_BATCH::MUH_KDV_AYIRMA_BATCH ( int p_fis_satiri_id , double  p_kdv_dahil_tutar , QString p_aciklama , QWidget * p_parent )
: BATCH_KERNEL ( p_parent ), m_ui ( new Ui::MUH_KDV_AYIRMA_BATCH )

{
    m_fis_satiri_id         =    p_fis_satiri_id;
    m_kdv_dahil_tutar       =    p_kdv_dahil_tutar;
    m_aciklama              =    p_aciklama;

    m_ui->setupUi      (this );
    START_BATCH_KERNEL (this, DB );
}

/**************************************************************************************
                   MUH_KDV_AYIRMA_BATCH::CHECK_VAR
***************************************************************************************/

void MUH_KDV_AYIRMA_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE     ( tr("kdv_ayirma_batch.html") );
    SET_PAGE_TITLE    ( tr ( "MUH - KDV AYIRMA" ) );

    REGISTER_BUTTON_WIDGET  ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME  ( "MUH_KDV_AYIRMA_BATCH" );

    m_kdv_hesap_id          =    0;

    E9_FILL_KDV_ORANLARI_COMBOBOX                      ( m_ui->comboBox_kdv_orani );
    m_ui->lineEdit_aciklama->setText                ( m_aciklama );
    m_ui->commaEdit_kdv_dahil_tutar->SET_DOUBLE     ( m_kdv_dahil_tutar );

    INDIRILECEK_KDV_HESAPLARI_COMBOBOX_DOLDUR       ();
    HESAPLANAN_KDV_HESAPLARI_COMBOBOX_DOLDUR        ();

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi,hesap_id" ,"tam_hesap_kodu = :tam_hesap_kodu" );

    sql_query.SET_VALUE      ( ":tam_hesap_kodu", m_ui->comboBox_kdv_hesabi->currentText() );

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        m_ui->lineEdit_kdv_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
        m_kdv_hesap_id = sql_query.VALUE(1).toInt();
    }

    SET_NAME_OF_RUN_BATCH_BUTTON    ( tr  ( "Kaydet" ) );
    SET_FIRST_FOCUS_WIDGET          ( m_ui->comboBox_kdv_hesabi );
}

/**************************************************************************************
                   MUH_KDV_AYIRMA_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_KDV_AYIRMA_BATCH::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->comboBox_kdv_hesabi ) {

        SQL_QUERY query ( DB );

        query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi,hesap_id" ,"tam_hesap_kodu = :tam_hesap_kodu" );

        query.SET_VALUE      ( ":tam_hesap_kodu", m_ui->comboBox_kdv_hesabi->currentText() );

        if ( query.SELECT() EQ 0 ) {

            MSG_WARNING(  tr ( "Hesap ismi bulunamadı" ), m_ui->comboBox_kdv_hesabi );

            return ADAK_FAIL;
        }

        query.NEXT();
        m_ui->lineEdit_kdv_hesap_ismi->setText ( query.VALUE(0).toString() );
        m_kdv_hesap_id = query.VALUE(1).toInt();
    }
    else if ( p_object EQ m_ui->comboBox_kdv_orani ) {
        KDV_HESAPLA ( m_ui->comboBox_kdv_orani->currentText().toDouble() );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_KDV_AYIRMA_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_KDV_AYIRMA_BATCH::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_KDV_AYIRMA_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_KDV_AYIRMA_BATCH::RUN_BATCH()
{
    SQL_QUERY query     ( DB );

    double      kdv_alacak      = 0.00;
    double      kdv_borc        = 0.00;

    double      borc_tutari     = 0.00;
    double      alacak_tutari   = 0.00;

    int         order_number   = 0;


    query.PREPARE_SELECT("muh_fis_satirlari,muh_fisler" , "order_number, alacak_tutari,fis_no",
                         "fis_satiri_id = :fis_satiri_id AND "
                         "muh_fisler.fis_id = muh_fis_satirlari.fis_id");

    query.SET_VALUE      ( ":fis_satiri_id", m_fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    order_number       =   query.VALUE(0).toInt();
    alacak_tutari         =   query.VALUE(1).toInt();
    QString fis_no        =   query.VALUE(2).toString();

    if ( alacak_tutari NE 0.00 ) {

        alacak_tutari = m_ui->commaEdit_kdv_haric_tutar->GET_DOUBLE();
        kdv_alacak    = m_ui->commaEdit_kdv_tutari->GET_DOUBLE();
    }
    else {

        borc_tutari   = m_ui->commaEdit_kdv_haric_tutar->GET_DOUBLE();
        kdv_borc      = m_ui->commaEdit_kdv_tutari->GET_DOUBLE();
    }

    if ( kdv_borc EQ 0 AND kdv_alacak EQ 0 ) {
        close();
        return;
    }

    M_KDV_AYIRMA_STRUCT->kdv_alacak_tutari             = kdv_alacak;
    M_KDV_AYIRMA_STRUCT->kdv_borc_tutari               = kdv_borc;
    M_KDV_AYIRMA_STRUCT->satir_kdv_haric_borc_tutari   = borc_tutari;
    M_KDV_AYIRMA_STRUCT->satir_kdv_haric_alacak_tutari = alacak_tutari;
    M_KDV_AYIRMA_STRUCT->aciklama                      = m_ui->lineEdit_aciklama->text();
    M_KDV_AYIRMA_STRUCT->kdv_hesap_id                  = m_kdv_hesap_id;
    M_KDV_AYIRMA_STRUCT->order_number               = order_number + 8;

    *m_accepted = true;

    QString log_detaylari = QObject::tr ( "Fiş No : " ) + fis_no;

    DB->START_TRANSACTION();
    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU,-1 ,LOG_ISLEM_KDV_AYIRMA,log_detaylari );
    DB->COMMIT_TRANSACTION();

    close();
}

/**************************************************************************************
                   MUH_KDV_AYIRMA_BATCH::INDIRILECEK_KDV_HESAPLARI_COMBOBOX_DOLDUR
***************************************************************************************/

void MUH_KDV_AYIRMA_BATCH::INDIRILECEK_KDV_HESAPLARI_COMBOBOX_DOLDUR ()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("e9_sabit_degerler" , "ind_kdv_hesap_id_array" );

    int *    INDIRILECEK_KDV_HESABI_ID_ARRAY = new int [KDV_OTV_ORANLARI_SAYISI];

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        UNPACK_INT_ARRAY(query.VALUE(0).toString() ,INDIRILECEK_KDV_HESABI_ID_ARRAY , KDV_OTV_ORANLARI_SAYISI );
    }

    QString where_str = "";
    for ( int i = 0 ; i < KDV_OTV_ORANLARI_SAYISI ; i++ ) {
        if (i NE 0) {
            where_str.append(" OR ");
        }

        where_str.append(QString("hesap_id = %1").arg(INDIRILECEK_KDV_HESABI_ID_ARRAY[i]));
    }

    query.PREPARE_SELECT ("muh_hesaplar" , "tam_hesap_kodu, hesap_ismi, tali_hesap" , where_str);

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING( tr ( "Kdv hesabı bulunamadı" ), NULL );
        return;
    }

    int i = 0;

    while ( query.NEXT() EQ true ) {
        int         parent_id             =     INDIRILECEK_KDV_HESABI_ID_ARRAY[i];
        QString     parent_tam_hesap_kodu =     query.VALUE(0).toString();
        QString     parent_hesap_ismi     =     query.VALUE(1).toString();
        int         tali_hesap_mi         =     query.VALUE(2).toInt();

        if ( tali_hesap_mi EQ 1 ) {

            m_ui->comboBox_kdv_hesabi->addItem ( parent_tam_hesap_kodu       );
            KDV_HESAPLA ( m_ui->comboBox_kdv_orani->currentText().toDouble() );
            m_ui->lineEdit_kdv_hesap_ismi->setText ( parent_hesap_ismi       );
            return;
        }

        QStringList KDV_HESAP_LISTESI;

        TALI_KDV_HESAPLARINI_BUL ( parent_id, KDV_HESAP_LISTESI );

        m_ui->comboBox_kdv_hesabi->addItems ( KDV_HESAP_LISTESI );

        i++;
    }
}

/**************************************************************************************
                   MUH_KDV_AYIRMA_BATCH::HESAPLANAN_KDV_HESAPLARI_COMBOBOX_DOLDUR
***************************************************************************************/

void MUH_KDV_AYIRMA_BATCH::HESAPLANAN_KDV_HESAPLARI_COMBOBOX_DOLDUR ()
{

    SQL_QUERY query ( DB );


    query.PREPARE_SELECT("e9_sabit_degerler" , "hes_kdv_hesap_id_array" );

    int *    HESAPLANACAK_KDV_HESABI_ID_ARRAY = new int [KDV_OTV_ORANLARI_SAYISI];

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        UNPACK_INT_ARRAY(query.VALUE(0).toString() ,HESAPLANACAK_KDV_HESABI_ID_ARRAY , KDV_OTV_ORANLARI_SAYISI );
    }

    QString where_str = "";

    for ( int i = 0 ; i < KDV_OTV_ORANLARI_SAYISI ; i++ ) {
        if (i NE 0) {
            where_str.append(" OR ");
        }

        where_str.append(QString("hesap_id = %1").arg(HESAPLANACAK_KDV_HESABI_ID_ARRAY[i]));
    }

    query.PREPARE_SELECT ( "muh_hesaplar" , "tam_hesap_kodu, hesap_ismi, tali_hesap" , where_str );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING( tr ( "Kdv hesabı bulunamadı" ), NULL );
        return;
    }

    int i = 0;

    while ( query.NEXT() EQ true ) {
        int         parent_id             =     HESAPLANACAK_KDV_HESABI_ID_ARRAY[i];
        QString     parent_tam_hesap_kodu =     query.VALUE(0).toString();
        QString     parent_hesap_ismi     =     query.VALUE(1).toString();
        int         tali_hesap_mi         =     query.VALUE(2).toInt();

        if ( tali_hesap_mi EQ 1 ) {

            m_ui->comboBox_kdv_hesabi->addItem ( parent_tam_hesap_kodu       );
            KDV_HESAPLA ( m_ui->comboBox_kdv_orani->currentText().toDouble() );
            m_ui->lineEdit_kdv_hesap_ismi->setText ( parent_hesap_ismi       );
            return;
        }

        QStringList KDV_HESAP_LISTESI;

        TALI_KDV_HESAPLARINI_BUL ( parent_id, KDV_HESAP_LISTESI );

        m_ui->comboBox_kdv_hesabi->addItems ( KDV_HESAP_LISTESI );

        i++;
    }
}

/**************************************************************************************
                   MUH_KDV_AYIRMA_BATCH::FIND_KDV
***************************************************************************************/

void MUH_KDV_AYIRMA_BATCH::FIND_KDV ( double kdv_orani )
{
    KDV_HESAPLA ( kdv_orani );
}

/**************************************************************************************
                   MUH_KDV_AYIRMA_BATCH::KDV_HESAPLA
***************************************************************************************/

void MUH_KDV_AYIRMA_BATCH::KDV_HESAPLA ( double kdv_orani )
{
    double      kdv_dahil_tutar     =       m_ui->commaEdit_kdv_dahil_tutar->GET_DOUBLE();
    double      kdv_haric_tutar     =       ROUND ( kdv_dahil_tutar * ( 1 / ( 1 + ( kdv_orani / 100 ) ) ) ) ;
    double      kdv_tutari          =       ROUND ( kdv_dahil_tutar - kdv_haric_tutar );

    m_ui->commaEdit_kdv_tutari->SET_DOUBLE      ( kdv_tutari      );
    m_ui->commaEdit_kdv_haric_tutar->SET_DOUBLE ( kdv_haric_tutar );
}

/**************************************************************************************
                   TALI_KDV_HESAPLARINI_BUL
***************************************************************************************/

void MUH_KDV_AYIRMA_BATCH::TALI_KDV_HESAPLARINI_BUL ( int p_parent_id, QStringList & P_KDV_HESDAP_LISTESI )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id, tam_hesap_kodu, tali_hesap" , "parent_id = :parent_id");

    sql_query.SET_VALUE       ( ":parent_id", p_parent_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    while ( sql_query.NEXT() EQ true ) {

        if ( sql_query.VALUE(2).toInt() EQ 1 ) {
            P_KDV_HESDAP_LISTESI.append ( sql_query.VALUE(1).toString() );
            continue;
        }
        TALI_KDV_HESAPLARINI_BUL ( sql_query.VALUE(0).toInt(), P_KDV_HESDAP_LISTESI );
    }
}
