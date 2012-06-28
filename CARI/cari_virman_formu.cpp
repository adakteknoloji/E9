#include <QMessageBox>
#include "cari_virman_formu_class.h"
#include "cari_virman_formu_open.h"
#include "ui_cari_virman_formu.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_struct.h"
#include "e9_enum.h"
#include "cari_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_yetki.h"
#include "e9_log.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "print.h"
#include "adak_std_utils.h"
#include "adres_console_utils.h"
#include "cari_fis_arama_open.h"
#include "cari_kart_arama_open.h"
#include "cari_fisi_yazdir_raporu_open.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "e9_ent_detaylari_batch_open.h"
#include "cari_enum.h"


extern ADAK_SQL * DB;

/**************************************************************************************
                  OPEN_CARI_VIRMAN_FORMU
***************************************************************************************/

void OPEN_CARI_VIRMAN_FORMU ( int p_form_id, QWidget * parent )
{
    CARI_VIRMAN_FORMU * F = new CARI_VIRMAN_FORMU ( p_form_id, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::CARI_VIRMAN_FORMU
***************************************************************************************/

CARI_VIRMAN_FORMU:: CARI_VIRMAN_FORMU ( int p_form_id, QWidget *parent ) : FORM_KERNEL ( parent ), m_ui ( new Ui:: CARI_VIRMAN_FORMU)
{
    m_form_id = p_form_id;
    m_ui->setupUi     (this );
    START_FORM_KERNEL (this, DB );
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::SETUP_FORM
***************************************************************************************/

void CARI_VIRMAN_FORMU::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "CARİ - VİRMAN FİŞİ" ) );
    SET_HELP_PAGE     ( "cari-hesap-islemleri_virman-fisi" );

    REGISTER_BUTTONS_WIDGET (m_ui->navigation_buttons_widget );

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );

    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }


    M_FIS               =   new CARI_FIS_STRUCT;
    M_FIS_SATIRI        =   new CARI_FIS_SATIRI_STRUCT;
    M_FIS_DETAYLARI     =   new E9_ENT_DETAYLARI_STRUCT;

    M_FIS->modul_id     =   CARI_MODULU;
    M_FIS->program_id   =   E9_PROGRAMI;

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH ( 512 );
    m_ui->lineEdit_fis_no->setMaxLength(10);

    m_ui->lineEdit_fis_no->setText("*");
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_fis_no);
    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_fis_no );
    SET_SETTING_NAME       ( "CARI_VIRMAN_FORMU" );

    m_ui->commaEdit_kur->SET_PRECISION(4);
    m_ui->commaEdit_kur->setHidden(true);

    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_fis_filtresi);

    if( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ HAYIR ){
        m_ui->pushButton_detaylar->setVisible( false );
    }

    if( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ){
        m_ui->comboBox_fis_filtresi->setVisible( false );
        m_ui->label_fis_filtresi->setVisible( false );

        m_ui->comboBox_sirket->setVisible( false );
        m_ui->label_sirket->setVisible( false );
    }


//    m_ui->lineEdit_alacakli_hesap_aciklama->setMaxLength(128);
//    m_ui->lineEdit_borclu_hesap_aciklama->setMaxLength(128);

    REGISTER_CHANGER_BUTTON(m_ui->pushButton_detaylar);

    REGISTER_SAVER_BUTTON(m_ui->pushButton_yazdir);

    FOCUS_FIRST_WIDGET();

    AUTOSTART_FORM_KERNEL( m_form_id );
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void CARI_VIRMAN_FORMU::CLEAR_FORM_MEMBERS ()
{
    CARI_CLEAR_FIS_STRUCT(M_FIS);

    CARI_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    m_borclu_hesap_id               = 0;
    m_alacakli_hesap_id             = 0;
    m_borclu_hesap_para_birimi_id   = 0;
    m_alacakli_hesap_para_birimi_id = 0;

    M_FIS_DETAYLARI->banka_fis_no.clear();
    M_FIS_DETAYLARI->cari_fis_no        = 0;
    M_FIS_DETAYLARI->cek_fis_no         = 0;
    M_FIS_DETAYLARI->firma_cek_fis_no   = 0;
    M_FIS_DETAYLARI->firma_senet_fis_no = 0;
    M_FIS_DETAYLARI->muh_fis_no         = 0;
    M_FIS_DETAYLARI->senet_fis_no       = 0;
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::NEW_RECORD
***************************************************************************************/

void  CARI_VIRMAN_FORMU::NEW_RECORD()
{
    m_ui->commaEdit_kur->setHidden ( true );
    m_ui->lineEdit_fis_no->setText("*");
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::GET_RECORD
***************************************************************************************/

int CARI_VIRMAN_FORMU::GET_RECORD ( int record_id )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "car_fisler",
                                  "fis_no , fis_tarihi , aciklama, doviz_kuru,"
                                  "program_id , modul_id ",
                                  "fis_id = :fis_id" );
    select_query.SET_VALUE      ( ":fis_id" , record_id );

    if ( select_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }
    select_query.NEXT();

    M_FIS->fis_no                     = select_query.VALUE ( "fis_no"             ).toInt();
    M_FIS->fis_tarihi                 = select_query.VALUE ( "fis_tarihi"         ).toString();
    M_FIS->aciklama                   = select_query.VALUE ( "aciklama"           ).toString();
    M_FIS->doviz_kuru                 = select_query.VALUE ( "doviz_kuru"         ).toDouble();
    M_FIS->program_id                 = select_query.VALUE ( "program_id"         ).toInt();
    M_FIS->modul_id                   = select_query.VALUE ( "modul_id"           ).toInt();

    m_ui->lineEdit_fis_no->setText                  ( QVariant ( M_FIS->fis_no ).toString() );
    m_ui->adakDate_fis_tarihi->SET_DATE( M_FIS->fis_tarihi );
    m_ui->limitedTextEdit_aciklama->setPlainText    ( M_FIS->aciklama );


    if ( M_FIS->doviz_kuru NE 0.0 ) {
        m_ui->commaEdit_kur->setHidden  ( false );
        m_ui->commaEdit_kur->SET_DOUBLE ( M_FIS->doviz_kuru );
    }


    select_query.PREPARE_SELECT ( "car_fis_satirlari",
                                  "cari_hesap_id ,order_number, borc_tutari,alacak_tutari,aciklama",
                                  "fis_id = :fis_id" );
    select_query.SET_VALUE      ( ":fis_id" , record_id );

    if ( select_query.SELECT( "order_number ASC ") EQ 0 ) {
        return ADAK_OK;
    }
    QString cari_hesap_ismi;
    QString cari_hesap_kodu;

    while ( select_query.NEXT() EQ true ) {

        int         cari_hesap_id    = select_query.VALUE(0).toInt();
        int         order_number     = select_query.VALUE(1).toInt();
        double      borc_tutari      = select_query.VALUE(2).toDouble();
        double      alacak_tutari    = select_query.VALUE(3).toDouble();

        SQL_QUERY sql_query ( DB );

        int cari_para_birimi_id = 0;

        sql_query.PREPARE_SELECT ( "car_hesaplar",
                                   "cari_hesap_kodu,adres_id,para_birimi_id  ",
                                   "cari_hesap_id = :cari_hesap_id");
        sql_query.SET_VALUE      ( ":cari_hesap_id" , cari_hesap_id );

        if ( sql_query.SELECT() NE 0 ) {
             sql_query.NEXT();
             cari_hesap_kodu = sql_query.VALUE(0).toString();
             cari_hesap_ismi = ADR_GET_FIRMA_SAHIS_ADI(sql_query.VALUE(1).toInt());
             cari_para_birimi_id = sql_query.VALUE(2).toInt();

        }
        if ( order_number EQ 1024 ) {
             m_ui->searchEdit_borclu_cari_hesap->SET_TEXT   ( cari_hesap_kodu );
             m_ui->lineEdit_borclu_cari_hesap_ismi->setText ( cari_hesap_ismi );
//             m_ui->lineEdit_borclu_hesap_aciklama->setText  ( select_query.VALUE(4).toString() );
             m_borclu_hesap_id = cari_hesap_id;
             QString para_birimi_kodu = DVZ_GET_DOVIZ_KODU(cari_para_birimi_id);
             m_ui->lineEdit_borclu_cari_para_birimi->setText(para_birimi_kodu);
             m_ui->lineEdit_makbuz_para_birimi->setText(para_birimi_kodu);
             m_borclu_hesap_para_birimi_id = cari_para_birimi_id;
        }
        else {
            m_ui->searchEdit_alacakli_cari_hesap->SET_TEXT(cari_hesap_kodu);
            m_ui->lineEdit_alacakli_cari_hesap_ismi->setText(cari_hesap_ismi);
//            m_ui->lineEdit_alacakli_hesap_aciklama->setText(select_query.VALUE(4).toString());
            m_alacakli_hesap_id = cari_hesap_id;
            QString para_birimi_kodu = DVZ_GET_DOVIZ_KODU(cari_para_birimi_id);
            m_ui->lineEdit_alacakli_para_birimi->setText(para_birimi_kodu);
            m_ui->lineEdit_makbuz_para_birimi->setText(para_birimi_kodu);
            m_alacakli_hesap_para_birimi_id = cari_para_birimi_id;
        }
        if ( borc_tutari > 0 ) {
            m_ui->commaEdit_tutar->SET_DOUBLE ( borc_tutari );
        }
        else {
            m_ui->commaEdit_tutar->SET_DOUBLE ( alacak_tutari );
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::CHECK_VAR
***************************************************************************************/

int CARI_VIRMAN_FORMU::CHECK_VAR ( QObject * object )
{
     if ( object EQ m_ui->searchEdit_borclu_cari_hesap ) {

        if ( m_ui->searchEdit_borclu_cari_hesap->GET_TEXT().isEmpty() EQ true ) {
            m_borclu_hesap_id = -1;
            m_ui->lineEdit_borclu_cari_hesap_ismi->clear();
            m_ui->lineEdit_borclu_cari_para_birimi->clear();
            return ADAK_OK;
        }

        SQL_QUERY select_query ( DB ) ;

        select_query.PREPARE_SELECT ( "car_hesaplar",
                                      "adres_id,cari_hesap_id,para_birimi_id ",
                                      "cari_hesap_kodu = :cari_hesap_kodu");
        select_query.SET_VALUE      ( ":cari_hesap_kodu" , m_ui->searchEdit_borclu_cari_hesap->GET_TEXT());

        if ( select_query.SELECT() EQ 0 ) {
           MSG_WARNING( tr ( "Aradığınız cari hesap kodu bulunamadı." ), m_ui->searchEdit_borclu_cari_hesap );

            return ADAK_FAIL_UNDO;
        }
        select_query.NEXT();

        m_ui->lineEdit_borclu_cari_hesap_ismi->setText ( ADR_GET_FIRMA_SAHIS_ADI(select_query.VALUE(0).toInt()) );
        m_borclu_hesap_id = select_query.VALUE(1).toInt();
        m_borclu_hesap_para_birimi_id = select_query.VALUE(2).toInt();
        QString para_birimi_kodu = DVZ_GET_DOVIZ_KODU(m_borclu_hesap_para_birimi_id);
        m_ui->lineEdit_borclu_cari_para_birimi->setText(para_birimi_kodu);
        m_ui->lineEdit_makbuz_para_birimi->setText(para_birimi_kodu);

        if ( m_borclu_hesap_para_birimi_id NE DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {
           m_ui->commaEdit_kur->setHidden(false);
           m_ui->commaEdit_kur->SET_DOUBLE(DVZ_DOVIZ_KURUNU_AL(m_borclu_hesap_para_birimi_id,m_ui->adakDate_fis_tarihi->QDATE(),
                                                           ENUM_SATIS));
        }
    }
    else if ( object EQ m_ui->searchEdit_alacakli_cari_hesap ) {

        if ( m_ui->searchEdit_alacakli_cari_hesap->GET_TEXT().isEmpty() EQ true ) {
            m_alacakli_hesap_id = -1;
            m_ui->lineEdit_alacakli_cari_hesap_ismi->clear();
            m_ui->lineEdit_alacakli_para_birimi->clear();
            return ADAK_OK;
        }

        SQL_QUERY select_query ( DB );

        select_query.PREPARE_SELECT ( "car_hesaplar",
                                      "adres_id,cari_hesap_id,para_birimi_id ",
                                      "cari_hesap_kodu = :cari_hesap_kodu");
        select_query.SET_VALUE      ( ":cari_hesap_kodu" , m_ui->searchEdit_alacakli_cari_hesap->GET_TEXT());

        if ( select_query.SELECT() EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız cari hesap kodu bulunamadı." ), m_ui->searchEdit_alacakli_cari_hesap );

            return ADAK_FAIL_UNDO;
        }

        select_query.NEXT();

        m_ui->lineEdit_alacakli_cari_hesap_ismi->setText ( ADR_GET_FIRMA_SAHIS_ADI(select_query.VALUE(0).toInt()) );
        m_alacakli_hesap_id = select_query.VALUE(1).toInt();
        m_alacakli_hesap_para_birimi_id = select_query.VALUE(2).toInt();
        QString para_birimi_kodu = DVZ_GET_DOVIZ_KODU(select_query.VALUE(2).toInt());
        m_ui->lineEdit_alacakli_para_birimi->setText(para_birimi_kodu);
        m_ui->lineEdit_makbuz_para_birimi->setText(para_birimi_kodu);

        if ( m_alacakli_hesap_para_birimi_id NE DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) {

            m_ui->commaEdit_kur->setHidden(false);
            m_ui->commaEdit_kur->SET_DOUBLE(DVZ_DOVIZ_KURUNU_AL(m_alacakli_hesap_para_birimi_id,m_ui->adakDate_fis_tarihi->QDATE(),ENUM_SATIS));

        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::CHECK_EMPTY
***************************************************************************************/

int CARI_VIRMAN_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->searchEdit_borclu_cari_hesap->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Borçlu cari hesabı girmelisiniz!.." ), m_ui->searchEdit_borclu_cari_hesap );

        return ADAK_FAIL;
    }
    if  ( m_ui->searchEdit_alacakli_cari_hesap->GET_TEXT().isEmpty() EQ true )  {
         MSG_WARNING( tr ( "Alacaklı cari hesabı girmelisiniz!.." ), m_ui->searchEdit_alacakli_cari_hesap );

         return ADAK_FAIL;
    }
    if ( m_ui->commaEdit_tutar->GET_DOUBLE() EQ 0.0 ) {
         MSG_WARNING( tr ( "Tutarı girmelisiniz!.." ), m_ui->commaEdit_tutar );

         return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_alacakli_para_birimi->text() NE m_ui->lineEdit_borclu_cari_para_birimi->text() ) {
        MSG_WARNING( tr ( "Cari hesapların para birimleri aynı olmalıdır." ), m_ui->lineEdit_alacakli_para_birimi );

        return ADAK_FAIL;

    }
    return ADAK_OK;
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::CHECK_ADD
***************************************************************************************/

int CARI_VIRMAN_FORMU::CHECK_ADD ()
{
    if ( QString ( m_ui->lineEdit_fis_no->text().at(0) ) NE "*" ) {
        if ( CARI_FIS_NO_KULLANILDI_MI ( m_ui->lineEdit_fis_no->text().toInt(), m_ui->adakDate_fis_tarihi->DATE() ) EQ true ) {
            MSG_WARNING( tr ( "Aynı numarada başka bir fiş var.Fiş eklenemedi!.." ), m_ui->lineEdit_fis_no );
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::ADD_RECORD
***************************************************************************************/

int CARI_VIRMAN_FORMU::ADD_RECORD ()
{
    int fis_no = 0;

    if ( QString( m_ui->lineEdit_fis_no->text().at(0) ) EQ "*" ) {
        fis_no  = CARI_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE());
    }
    else {
        fis_no = m_ui->lineEdit_fis_no->text().toInt();
    }
    CARI_CLEAR_FIS_STRUCT(M_FIS);

    M_FIS->fis_no                = fis_no;
    M_FIS->fis_tarihi            = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama              = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->fis_turu              = ENUM_CARI_VIRMAN_FORMU;
    M_FIS->doviz_kuru            = m_ui->commaEdit_kur->GET_DOUBLE();
    M_FIS->base_fis_id           = 0;
    M_FIS->modul_id              = CARI_MODULU;
    M_FIS->program_id            = E9_PROGRAMI;
    M_FIS->makbuz_para_birimi_id = DVZ_GET_DOVIZ_ID(m_ui->lineEdit_makbuz_para_birimi->text());

    int fis_id = CARI_FIS_EKLE ( M_FIS );

    m_ui->lineEdit_fis_no->setText ( QVariant ( fis_no ).toString() );

    QString log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_VIRMAN_FORMU ) + "," +
                              m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM yyyy") +
                              ",Fiş No : "  + QVariant ( fis_no ).toString() );


    E9_LOG_KAYDI_EKLE ( CARI_MODULU,LOG_CARI_FISLER, LOG_ISLEM_ADD, log_detaylari );

    CARI_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    M_FIS_SATIRI->fis_id                    = fis_id;
    //M_FIS_SATIRI->aciklama                  = m_ui->lineEdit_borclu_hesap_aciklama->text();
    M_FIS_SATIRI->borc_tutari               = m_ui->commaEdit_tutar->GET_DOUBLE();
    M_FIS_SATIRI->alacak_tutari             = 0.0;
    M_FIS_SATIRI->cari_hesap_id             = m_borclu_hesap_id;
    M_FIS_SATIRI->base_record_id            = 0;
    M_FIS_SATIRI->modul_id                  = M_FIS->modul_id;
    M_FIS_SATIRI->program_id                = M_FIS->program_id;
    M_FIS_SATIRI->doviz_id                  = M_FIS->makbuz_para_birimi_id;

    CARI_FIS_SATIRI_EKLE ( M_FIS, M_FIS_SATIRI );

    CARI_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    M_FIS_SATIRI->fis_id                    = fis_id;
    //M_FIS_SATIRI->aciklama                  = m_ui->lineEdit_alacakli_hesap_aciklama->text();
    M_FIS_SATIRI->borc_tutari               = 0.0;
    M_FIS_SATIRI->alacak_tutari             = m_ui->commaEdit_tutar->GET_DOUBLE();
    M_FIS_SATIRI->cari_hesap_id             = m_alacakli_hesap_id;
    M_FIS_SATIRI->base_record_id            = 0;
    M_FIS_SATIRI->modul_id                  = M_FIS->modul_id;
    M_FIS_SATIRI->program_id                = M_FIS->program_id;
    M_FIS_SATIRI->doviz_id                  = M_FIS->makbuz_para_birimi_id;

    CARI_FIS_SATIRI_EKLE ( M_FIS, M_FIS_SATIRI );

    return fis_id;
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::CHECK_UPDATE
***************************************************************************************/

int CARI_VIRMAN_FORMU::CHECK_UPDATE ( int record_id )
{
    if ( QString ( m_ui->lineEdit_fis_no->text().at(0) ) EQ "*" ) {
        MSG_WARNING( tr ( "Fiş numarasını güncellemek için Fiş No kısmına sadece sayı girebilirsiniz." ), m_ui->lineEdit_fis_no );

        return ADAK_FAIL;
    }

    int fis_no = m_ui->lineEdit_fis_no->text().toInt();

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "car_fisler", "fis_id", "fis_no = :fis_no AND fis_tarihi = :fis_tarihi");
    sql_query.SET_VALUE      ( ":fis_no"    ,   fis_no );
    sql_query.SET_VALUE      ( ":fis_tarihi",   m_ui->adakDate_fis_tarihi->DATE() );

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        if ( sql_query.VALUE(0).toInt() NE record_id ) {
            MSG_WARNING( tr ( "Aynı numarada başka bir fiş var.Güncelleme yapılamadı!.."), m_ui->lineEdit_fis_no );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::UPDATE_RECORD
***************************************************************************************/

void CARI_VIRMAN_FORMU::UPDATE_RECORD (int record_id)
{
    SQL_QUERY query ( DB );

    int fis_no = m_ui->lineEdit_fis_no->text().toInt();

    CARI_FIS_BILGILERINI_OKU(M_FIS , record_id);

    M_FIS->fis_no                =   fis_no;
    M_FIS->fis_tarihi            =   m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama              =   m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->toplam_borc           =   0;
    M_FIS->toplam_alacak         =   0;
    M_FIS->doviz_kuru            =   m_ui->commaEdit_kur->GET_DOUBLE();
    M_FIS->makbuz_para_birimi_id =   DVZ_GET_DOVIZ_ID(m_ui->lineEdit_makbuz_para_birimi->text());

    CARI_FISI_GUNCELLE ( M_FIS, record_id );

    query.PREPARE_SELECT ( "car_fis_satirlari","fis_satiri_id",
                           "fis_id = :fis_id " );
    query.SET_VALUE      ( ":fis_id", record_id );

    if ( query.SELECT( "order_number ASC ") EQ 2 ) {

        query.NEXT();

        M_FIS_SATIRI->fis_id                    = record_id;
        //M_FIS_SATIRI->aciklama                  = m_ui->lineEdit_borclu_hesap_aciklama->text();
        M_FIS_SATIRI->borc_tutari               = m_ui->commaEdit_tutar->GET_DOUBLE();
        M_FIS_SATIRI->alacak_tutari             = 0.0;
        M_FIS_SATIRI->cari_hesap_id             = m_borclu_hesap_id;
        M_FIS_SATIRI->modul_id                  = M_FIS->modul_id;
        M_FIS_SATIRI->program_id                = M_FIS->program_id;
        M_FIS_SATIRI->doviz_id                  = M_FIS->makbuz_para_birimi_id;
        M_FIS_SATIRI->base_record_id            = 0;

        CARI_FIS_SATIRINI_GUNCELLE( M_FIS, M_FIS_SATIRI, query.VALUE(0).toInt() );

        query.NEXT();

        M_FIS_SATIRI->fis_id                    = record_id;
        //M_FIS_SATIRI->aciklama                  = m_ui->lineEdit_alacakli_hesap_aciklama->text();
        M_FIS_SATIRI->borc_tutari               = 0.0;
        M_FIS_SATIRI->alacak_tutari             = m_ui->commaEdit_tutar->GET_DOUBLE();
        M_FIS_SATIRI->cari_hesap_id             = m_alacakli_hesap_id;
        M_FIS_SATIRI->modul_id                  = M_FIS->modul_id;
        M_FIS_SATIRI->program_id                = M_FIS->program_id;
        M_FIS_SATIRI->base_record_id            = 0;

        CARI_FIS_SATIRINI_GUNCELLE( M_FIS, M_FIS_SATIRI, query.VALUE(0).toInt() );
        M_FIS_SATIRI->doviz_id                  = M_FIS->makbuz_para_birimi_id;

    }


    QString log_detaylari = (CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_VIRMAN_FORMU ) + "," +
                                 m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM yyyy") +
                                 ",Fiş No : "  + QVariant ( fis_no ).toString() + ",Toplam Borç : " +
                                 VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_tutar->GET_DOUBLE()).toString()) + ",Toplam Alacak : " +
                                 VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_tutar->GET_DOUBLE()).toString()));

    E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_UPDATE , log_detaylari );

}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::CHECK_DELETE
***************************************************************************************/

int CARI_VIRMAN_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );

    return ADAK_OK;
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::DELETE_RECORD
***************************************************************************************/

void CARI_VIRMAN_FORMU::DELETE_RECORD ( int record_id )
{

    CARI_FISI_SIL ( M_FIS, record_id );

    QString log_detaylari = ( CARI_GET_FIS_TURLERI_STRING ( ENUM_CARI_VIRMAN_FORMU ) + "," +
                              m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                              ",Fiş No : "        + m_ui->lineEdit_fis_no->text() +
                              ",Toplam Borç : "   + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_tutar->GET_DOUBLE() ).toString() ) +
                              ",Toplam Alacak : " + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_tutar->GET_DOUBLE() ).toString() ) );

    E9_LOG_KAYDI_EKLE ( CARI_MODULU , LOG_CARI_FISLER , LOG_ISLEM_DELETE , log_detaylari );

}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::SELECT_RECORD
***************************************************************************************/

int CARI_VIRMAN_FORMU::SELECT_RECORD()
{
    return OPEN_CARI_FIS_ARAMA ( ENUM_CARI_VIRMAN_FORMU,CARI_MODULU, ENUM_CARI_FISI, this );
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::FIND_RECORD
***************************************************************************************/

int CARI_VIRMAN_FORMU::FIND_RECORD()
{
   SQL_QUERY select_query ( DB ) ;

   select_query.PREPARE_SELECT ( "car_fisler",
                                 "fis_id",
                                 "fis_no = :fis_no "
                                 "AND fis_turu = :fis_turu AND fis_tarihi = :fis_tarihi" );

   select_query.SET_VALUE      ( ":fis_no"       , m_ui->lineEdit_fis_no->text().toInt() );
   select_query.SET_VALUE      ( ":fis_turu"     , ENUM_CARI_VIRMAN_FORMU );
   select_query.SET_VALUE      ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

   if(select_query.SELECT() EQ 0 ) {
       MSG_WARNING( tr("Virman fişi bulunamadı."), m_ui->lineEdit_fis_no );

       return 0;
   }
   select_query.NEXT();
   return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int CARI_VIRMAN_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY select_query ( DB );

    QString sql_query = QString ("fis_turu  = :fis_turu");

    select_query.PREPARE_SELECT ( "car_fisler", "fis_id", sql_query );

    select_query.SET_VALUE            ( ":fis_turu",    ENUM_CARI_VIRMAN_FORMU    );

    if ( select_query.SELECT( "fis_tarihi, fis_no  ", 0, 1 ) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int CARI_VIRMAN_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY select_query ( DB );

    QString sql_query = QString ("fis_turu  = :fis_turu");

    select_query.PREPARE_SELECT       ( "car_fisler", "fis_id", sql_query );
    select_query.SET_VALUE            ( ":fis_turu"    , ENUM_CARI_VIRMAN_FORMU   );

    if ( select_query.SELECT( "fis_tarihi DESC, fis_no DESC ", 0, 1 ) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int CARI_VIRMAN_FORMU::FIND_PREV_RECORD ()
{
    SQL_QUERY select_query ( DB );

    QString sql_query = QString ("fis_no             < :fis_no "
                                 "AND fis_turu       = :fis_turu "
                                 "AND fis_tarihi     = :fis_tarihi ");

    select_query.PREPARE_SELECT       ( "car_fisler", "fis_id", sql_query );

    select_query.SET_VALUE            ( ":fis_no"       , m_ui->lineEdit_fis_no->text().toInt());
    select_query.SET_VALUE            ( ":fis_turu"     , ENUM_CARI_VIRMAN_FORMU     );
    select_query.SET_VALUE            ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_no DESC", 0, 1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    sql_query = QString ("fis_turu           = :fis_turu "
                         "AND fis_tarihi     < :fis_tarihi ");

    select_query.PREPARE_SELECT       ( "car_fisler", "fis_id", sql_query );

    select_query.SET_VALUE            ( ":fis_turu"     , ENUM_CARI_VIRMAN_FORMU    );
    select_query.SET_VALUE            ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_tarihi DESC, fis_no DESC" , 0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int CARI_VIRMAN_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY select_query ( DB );

    QString sql_query = QString ("fis_no             > :fis_no "
                                 "AND fis_turu       = :fis_turu "
                                 "AND fis_tarihi     = :fis_tarihi ");

    select_query.PREPARE_SELECT       ( "car_fisler", "fis_id", sql_query );

    select_query.SET_VALUE           ( ":fis_no"       , m_ui->lineEdit_fis_no->text().toInt()  );
    select_query.SET_VALUE           ( ":fis_turu"     , ENUM_CARI_VIRMAN_FORMU                  );
    select_query.SET_VALUE           ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT( "fis_no", 0, 1 ) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    sql_query = QString ("fis_turu           = :fis_turu "
                         "AND fis_tarihi     > :fis_tarihi ");

    select_query.PREPARE_SELECT       ( "car_fisler", "fis_id", sql_query );

    select_query.SET_VALUE            ( ":fis_turu"     , ENUM_CARI_VIRMAN_FORMU           );
    select_query.SET_VALUE            ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT("fis_tarihi, fis_no", 0, 1 ) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}


/**************************************************************************************
                  CARI_VIRMAN_FORMU::LOCK_RECORD
***************************************************************************************/

int CARI_VIRMAN_FORMU::LOCK_RECORD ( int record_id)
{
    return DB->LOCK_ROW ( "fis_id","car_fisler", QString ( "fis_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::UNLOCK_RECORD
***************************************************************************************/

void CARI_VIRMAN_FORMU::UNLOCK_RECORD(int record_id)
{
    DB->UNLOCK_ROW ( "fis_id","car_fisler", QString ( "fis_id = %1").arg ( record_id ) );
}



/**************************************************************************************
                  CARI_VIRMAN_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void CARI_VIRMAN_FORMU::SEARCH_EDIT_CLICKED ( QWidget * widget ,QLineEdit * line_edit )
{
   int cari_hesap_id = OPEN_CARI_KART_ARAMA ( line_edit->text(), SAHIS_ARTI_FIRMA, this, true , E9_PROGRAMI, CARI_MODULU );

   if ( cari_hesap_id NE -1 ){
       if ( widget EQ m_ui->searchEdit_borclu_cari_hesap) {
            line_edit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id ) );
       }
       else {
           line_edit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id ) );
       }
   }
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::CHANGER_BUTTON_CLICKED()
***************************************************************************************/

int CARI_VIRMAN_FORMU::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{
    Q_UNUSED(button);

    int record_id = GET_RECORD_ID();

    int muh_fis_id = MUH_GET_ENT_FIS_ID(M_FIS->program_id , M_FIS->modul_id , record_id );

    M_FIS_DETAYLARI->muh_fis_no = E9_GET_FIS_NO(MUHASEBE_MODULU , muh_fis_id);

    OPEN_E9_ENT_DETAYLARI_BATCH(M_FIS_DETAYLARI , false,this);

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                  CARI_VIRMAN_FORMU::SAVER_BUTTON_CLICKED()
***************************************************************************************/

void CARI_VIRMAN_FORMU::SAVER_BUTTON_CLICKED(QAbstractButton *button, int record_id)
{
    if ( button EQ m_ui->pushButton_yazdir ) {
        OPEN_REPORT_SHOWER(GET_CARI_FISI_YAZDIR_RAPORU(record_id , ENUM_CARI_VIRMAN_FORMU),nativeParentWidget());
    }
    else if ( button EQ m_ui->pushButton_urettigi_fisler ) {
         E9_URETILEN_FIS_SEC( E9_PROGRAMI, CARI_MODULU, record_id, this );
    }
}








