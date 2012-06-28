#include "dmr_satis_formu_class.h"
#include "ui_dmr_satis_formu.h"
#include "adak_sql.h"
#include "dmr_con_utils.h"
#include "fat_enum.h"
#include "fat_fatura_arama_open.h"
 
#include "dmr_demirbas_karti_arama_open.h"

extern ADAK_SQL *           DB;



/**************************************************************************************
                   OPEN_DMR_SATIS_FORMU
***************************************************************************************/

void OPEN_DMR_SATIS_FORMU ( QWidget * parent )
{
    DMR_SATIS_FORMU * F = new DMR_SATIS_FORMU ( parent );
    F->SHOW( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   DMR_SATIS_FORMU::DMR_SATIS_FORMU
***************************************************************************************/

DMR_SATIS_FORMU:: DMR_SATIS_FORMU ( QWidget * parent ) : FORM_KERNEL ( parent ), m_ui(new Ui:: DMR_SATIS_FORMU)
{
    m_ui->setupUi     ( this );
    START_FORM_KERNEL ( this , DB );
}

/**************************************************************************************
                   DMR_SATIS_FORMU::SETUP_FORM
***************************************************************************************/

void  DMR_SATIS_FORMU::SETUP_FORM ()
{
    SET_PAGE_TITLE    ( tr ( "DMR - SABİT KIYMETLER SATIŞ" ) );
    SET_SETTING_NAME  ("SABIT_KIYMET_FORMU");
    SET_HELP_PAGE     ("sabit-kiymetler-demirbaslar_sabit-kiymet-satisi");

    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget , true );

    SET_FIRST_FOCUS_WIDGET ( m_ui->searchEdit_dmr_kodu );

    REGISTER_CHANGER_BUTTON(m_ui->pushButton_fatura_sec);

}


/**************************************************************************************
                   DMR_SATIS_FORMU::NEW_RECORD
***************************************************************************************/

void  DMR_SATIS_FORMU::NEW_RECORD ()
{
    m_demirbas_id = 0;
    m_fatura_id   = 0;
}

/**************************************************************************************
                   DMR_SATIS_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void DMR_SATIS_FORMU::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   DMR_SATIS_FORMU::GET_RECORD
***************************************************************************************/

int DMR_SATIS_FORMU::GET_RECORD ( int record_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("dmr_satilan_demirbaslar,dmr_demirbaslar" ,
                         "dmr_satilan_demirbaslar.demirbas_id,dmr_satilan_demirbaslar.fatura_id,"
                         "demirbas_kodu,demirbas_adi" ,
                         "satis_id = :satis_id AND dmr_demirbaslar.demirbas_id = dmr_satilan_demirbaslar.demirbas_id");
    query.SET_VALUE(":satis_id" , record_id);

    if ( query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }
    query.NEXT();

    m_demirbas_id = query.VALUE(0).toInt();
    m_fatura_id   = query.VALUE(1).toInt();

    m_ui->searchEdit_dmr_kodu->SET_TEXT(query.VALUE(2).toString());
    m_ui->lineEdit_dmr_ismi->setText(query.VALUE(3).toString());

    m_ui->lineEdit_faturasi->setText(DMR_GET_FATURA_BILGILERI(m_fatura_id));

    return ADAK_OK;

}

/**************************************************************************************
                   DMR_SATIS_FORMU::CHECK_VAR
***************************************************************************************/

int DMR_SATIS_FORMU::CHECK_VAR (QObject * object)
{
    if ( object EQ m_ui->searchEdit_dmr_kodu ) {

        if ( m_ui->searchEdit_dmr_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_demirbas_id = 0;
            m_ui->lineEdit_dmr_ismi->clear();
            return ADAK_OK;
        }

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("dmr_demirbaslar" , "demirbas_id,demirbas_adi" ,
                             "demirbas_kodu=:demirbas_kodu AND demirbas_satildi_mi=:demirbas_satildi_mi");
        query.SET_VALUE(":demirbas_kodu"       , m_ui->searchEdit_dmr_kodu->GET_TEXT() );
        query.SET_VALUE(":demirbas_satildi_mi" , 0);

        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING(QObject::tr("Aradığınız demirbaş kodu bulunamadı.") , m_ui->searchEdit_dmr_kodu);
            return ADAK_FAIL;
        }

        query.NEXT();

        m_demirbas_id = query.VALUE(0).toInt();
        m_ui->lineEdit_dmr_ismi->setText(query.VALUE(1).toString());

    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_SATIS_FORMU::CHECK_EMPTY
***************************************************************************************/

int DMR_SATIS_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->searchEdit_dmr_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(QObject::tr("Sabit kıymet kodu boş bırakamazsınız") , m_ui->searchEdit_dmr_kodu);
        return ADAK_FAIL;
    }
    if ( m_fatura_id <= 0 ) {
        MSG_WARNING(QObject::tr("Fatura Seçimi Yapmalısınız") , m_ui->pushButton_fatura_sec);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_SATIS_FORMU::CHECK_ADD
***************************************************************************************/

int DMR_SATIS_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_SATIS_FORMU::ADD_RECORD
***************************************************************************************/

int DMR_SATIS_FORMU::ADD_RECORD ()
{
    SQL_QUERY query(DB);

    query.PREPARE_INSERT("dmr_satilan_demirbaslar" , "satis_id" , "demirbas_id,fatura_id");
    query.SET_VALUE(":demirbas_id" , m_demirbas_id);
    query.SET_VALUE(":fatura_id"   , m_fatura_id);

    int satis_id = query.INSERT();

    query.PREPARE_UPDATE("dmr_demirbaslar" , "demirbas_id" , "demirbas_satildi_mi" ,
                         "demirbas_id = :demirbas_id");
    query.SET_VALUE(":demirbas_satildi_mi" , 1);
    query.SET_VALUE(":demirbas_id"         , m_demirbas_id);

    query.UPDATE();

    DMR_ENTEGRASYON_FISI_OLUSTUR(m_fatura_id , m_demirbas_id);

    return satis_id;
}

/**************************************************************************************
                   DMR_SATIS_FORMU::CHECK_UPDATE
***************************************************************************************/

int DMR_SATIS_FORMU::CHECK_UPDATE ( int record_id )
{
    Q_UNUSED(record_id);

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_SATIS_FORMU::UPDATE_RECORD
***************************************************************************************/

void DMR_SATIS_FORMU::UPDATE_RECORD ( int record_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("dmr_satilan_demirbaslar" , "demirbas_id,fatura_id" , "satis_id = :satis_id");
    query.SET_VALUE(":satis_id" , record_id);

    int old_demirbas_id = 0;
    int old_fatura_id   = 0 ;

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        old_demirbas_id = query.VALUE(0).toInt();
        old_fatura_id   = query.VALUE(1).toInt();
    }

    if ( old_demirbas_id NE m_demirbas_id ) {

        query.PREPARE_UPDATE("dmr_demirbaslar" , "demirbas_id" , "demirbas_satildi_mi" ,
                             "demirbas_id = :demirbas_id");
        query.SET_VALUE(":demirbas_satildi_mi" , 0 );
        query.SET_VALUE(":demirbas_id"         , old_demirbas_id);
        query.UPDATE();
    }
    if ( old_fatura_id NE m_fatura_id ) {
        //Fatura entegrasyonu geri alinacak
        DMR_ENTEGRASYON_FISI_OLUSTUR(old_fatura_id , 0);
    }


    query.PREPARE_UPDATE("dmr_satilan_demirbaslar" , "satis_id" , "demirbas_id,fatura_id" ,
                         "satis_id = :satis_id");

    query.SET_VALUE(":demirbas_id" , m_demirbas_id);
    query.SET_VALUE(":fatura_id"   , m_fatura_id);
    query.SET_VALUE(":satis_id"    , record_id);


    query.UPDATE();

    DMR_ENTEGRASYON_FISI_OLUSTUR(m_fatura_id , m_demirbas_id);

}

/**************************************************************************************
                   DMR_SATIS_FORMU::CHECK_DELETE
***************************************************************************************/

int DMR_SATIS_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED(record_id);

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_SATIS_FORMU::DELETE_RECORD
***************************************************************************************/

void DMR_SATIS_FORMU::DELETE_RECORD ( int record_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_UPDATE("dmr_demirbaslar" , "demirbas_id" , "demirbas_satildi_mi" ,
                         "demirbas_id = :demirbas_id");
    query.SET_VALUE(":demirbas_satildi_mi" , 0);
    query.SET_VALUE(":demirbas_id"         , m_demirbas_id);
    query.UPDATE();

    query.PREPARE_DELETE("dmr_satilan_demirbaslar" , "satis_id = :satis_id");
    query.SET_VALUE(":satis_id" , record_id);
    query.DELETE();

    DMR_ENTEGRASYON_FISI_OLUSTUR(m_fatura_id , 0);
}

/**************************************************************************************
                   DMR_SATIS_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int DMR_SATIS_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dmr_satilan_demirbaslar" , "satis_id");

    if ( query.SELECT("satis_id ASC") EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();

}

/**************************************************************************************
                   DMR_SATIS_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int DMR_SATIS_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dmr_satilan_demirbaslar" , "satis_id");

    if ( query.SELECT("satis_id DESC") EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_SATIS_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int DMR_SATIS_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dmr_satilan_demirbaslar" , "satis_id" , "satis_id > :satis_id");
    query.SET_VALUE(":satis_id" , GET_RECORD_ID());

    if ( query.SELECT("satis_id ASC") EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_SATIS_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int DMR_SATIS_FORMU::FIND_PREV_RECORD ()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("dmr_satilan_demirbaslar" , "satis_id" , "satis_id < :satis_id");
    query.SET_VALUE(":satis_id" , GET_RECORD_ID());

    if ( query.SELECT("satis_id DESC") EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_SATIS_FORMU::SELECT_RECORD
***************************************************************************************/

int DMR_SATIS_FORMU::SELECT_RECORD()
{
    return 0;
}

/**************************************************************************************
                   DMR_SATIS_FORMU::LOCK_RECORD
***************************************************************************************/

int DMR_SATIS_FORMU::LOCK_RECORD( int record_id )
{
    Q_UNUSED(record_id);

    return DB->LOCK_ROW ( "demirbas_id", "dmr_demirbaslar", QString ( "demirbas_id = %1").arg ( m_demirbas_id ) );
}

/**************************************************************************************
                   DMR_SATIS_FORMU::UNLOCK_RECORD
***************************************************************************************/

void DMR_SATIS_FORMU::UNLOCK_RECORD( int record_id )
{
    Q_UNUSED(record_id);

    DB->UNLOCK_ROW ( "demirbas_id", "dmr_demirbaslar", QString ( "demirbas_id = %1").arg ( m_demirbas_id ) );
}

/**************************************************************************************
                   DMR_SATIS_FORMU::FIND_ENTERED_RECORD
***************************************************************************************/

int DMR_SATIS_FORMU::FIND_RECORD()
{
    return 0;
}

/**************************************************************************************
                   DMR_SATIS_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void DMR_SATIS_FORMU::SEARCH_EDIT_CLICKED(QWidget *,QLineEdit * line_edit)
{
    m_demirbas_id = OPEN_DMR_DEMIRBAS_KARTI_ARAMA(m_ui->searchEdit_dmr_kodu->GET_TEXT(),this);

    if ( m_demirbas_id > 0 ) {
        SQL_QUERY query(DB);

        query.PREPARE_SELECT("dmr_demirbaslar" , "demirbas_kodu" , "demirbas_id = :demirbas_id");
        query.SET_VALUE(":demirbas_id" , m_demirbas_id);

        if ( query.SELECT() NE 0 ) {
            query.NEXT();

            line_edit->setText(query.VALUE(0).toString());
        }
    }
}

/**************************************************************************************
                   DMR_SATIS_FORMU::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int DMR_SATIS_FORMU::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{
    Q_UNUSED(button);

    int fatura_id = OPEN_FATURA_ARAMA( ENUM_SATIS_IRSALIYESI_FATURASI , ENUM_FAT_NORMAL, this);

    if ( fatura_id EQ m_fatura_id ) {
        return ADAK_RECORD_UNCHANGED;
    }

    if ( fatura_id < 0 ) {
        m_ui->lineEdit_faturasi->clear();
        m_fatura_id = fatura_id;

        return ADAK_RECORD_CHANGED;
    }
    else {
        m_fatura_id = fatura_id;
        m_ui->lineEdit_faturasi->setText(DMR_GET_FATURA_BILGILERI(m_fatura_id));

        return ADAK_RECORD_CHANGED;
    }
    return ADAK_RECORD_UNCHANGED;

}













