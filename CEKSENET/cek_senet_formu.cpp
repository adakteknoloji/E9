#include <QHeaderView>
#include <QTableWidget>
#include "cek_senet_formu_class.h"
#include "ui_cek_senet_formu.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "adak_std_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "bnk_gui_utils.h"
#include "cek_enum.h"
#include "e9_log.h"
#include "cek_senet_utils.h"
#include "bnk_hesaplari_arama_open.h"
#include "cek_senet_arama_open.h"
#include "cek_console_utils.h"
#include "bnk_console_utils.h"
#include "dvz_gui_utils.h"

extern  ADAK_SQL *  DB;

/**************************************************************************************
                   OPEN_CEK_SENET_FORMU
***************************************************************************************/

void OPEN_CEK_SENET_FORMU ( int cek_senet_turu, int cek_senet_id , QWidget * parent )
{
    CEK_SENET_FORMU * F = new CEK_SENET_FORMU ( cek_senet_turu,cek_senet_id , parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                    CEK_SENET_FORMU::CEK_SENET_FORMU
***************************************************************************************/

CEK_SENET_FORMU::CEK_SENET_FORMU ( int cek_senet_turu ,int cek_senet_id, QWidget * parent ) : FORM_KERNEL ( parent ), m_ui ( new Ui::CEK_SENET_FORMU )
{
    m_cek_senet_turu         = cek_senet_turu;
    m_cek_senet_id           = cek_senet_id;

    m_ui->setupUi     ( this );
    START_FORM_KERNEL ( this, DB );
}

/**************************************************************************************
                    CEK_SENET_FORMU::SETUP_FORM
***************************************************************************************/

void CEK_SENET_FORMU::SETUP_FORM()
{
    SET_HELP_PAGE  ( "cek-senet-islemleri" );
    if ( m_cek_senet_turu EQ ENUM_CEK ) {

        SET_PAGE_TITLE ( tr ( "ÇEK-SNT - KENDİ ÇEKLERİMİZ" ) );
        SET_HELP_PAGE  ( "cek-senet-islemleri_kendi-ceklerimiz" );
        m_ui->label_cek_senet_no->setText           ( "Çek Numarası" );

        BNK_FILL_COMBO_BOX_BANKA_HESAPLARI( m_ui->combobox_banka_hesap_kodu );
        m_ui->comboBox_doviz_kodu->setDisabled      ( true );
    }
    else {  //senet ise
        m_ui->label_banka_ismi->setHidden           ( true );
        m_ui->combobox_banka_hesap_kodu->setHidden    ( true );
        m_ui->lineedit_bnk_sube_bilgisi->setHidden  ( true );
        m_ui->lineEdit_doviz_cinsi->setHidden       ( true );

        SET_PAGE_TITLE ( tr ( "ÇEK-SNT - KENDİ SENETLERİMİZ" ) );
        SET_HELP_PAGE  ( "cek-senet-islemleri_kendi-senetlerimiz" );
        m_ui->label_cek_senet_no->setText           ( tr ( "Senet Numarası" ) );
    }
    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_buttons_widget );

    m_ui->adakDate_vade_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_keside_tarihi->MALI_YIL_KISITI_YOK( true );

    M_CEK_SENET = new CEK_SENET_STRUCT;

    DVZ_FILL_COMBOBOX ( m_ui->comboBox_doviz_kodu );

    m_ui->comboBox_doviz_kodu->setEditable ( false );

    m_ui->lineedit_portfoy_no->setText("*");
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_portfoy_no);

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_portfoy_no->setValidator ( validator );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ 1 ) {
        m_ui->comboBox_fis_filtresi->setHidden(false);
        m_ui->label_fis_filtresi->setHidden(false);
    }

    m_ui->comboBox_doviz_kodu->setCurrentIndex ( m_ui->comboBox_doviz_kodu->findText ( DVZ_GET_TEMEL_PARA_BIRIMI_KODU() ) );

    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_fis_filtresi);


    SET_FIRST_FOCUS_WIDGET                      ( m_ui->lineedit_portfoy_no );

    SET_SETTING_NAME                            ( "CEK_SENET_FORMU" );

    m_ui->lineedit_bnk_sube_bilgisi->setMaxLength(128);
    m_ui->lineedit_cek_no->setMaxLength          (20);
    m_ui->lineEdit_doviz_cinsi->setMaxLength     (5);
    m_ui->lineedit_portfoy_no->setMaxLength      (10);

    AUTOSTART_FORM_KERNEL(m_cek_senet_id);
}

/**************************************************************************************
                    CEK_SENET_FORMU::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void CEK_SENET_FORMU::CLEAR_FORM_MEMBERS()
{
    m_banka_ismi.clear();
    m_sube_ismi.clear();
    m_sube_kodu.clear();

   m_ui->lineedit_portfoy_no->setText("*");

    m_doviz_id           = 0;
    m_hesap_no_id        = 0;

    M_CEK_SENET->cek_senet_turu   = -1;
    M_CEK_SENET->cek_senet_tutari = 0;
    M_CEK_SENET->doviz_id         = 0;
    M_CEK_SENET->hesap_no_id      = 0;
    M_CEK_SENET->musteri_ceki_mi = -1;
    M_CEK_SENET->portfoy_numarasi = 0;
    M_CEK_SENET->keside_tarihi.clear();
    M_CEK_SENET->musteri_hesap_numarasi.clear();
    M_CEK_SENET->sube_ismi.clear();
    M_CEK_SENET->sube_kodu.clear();
    M_CEK_SENET->vade_tarihi.clear();
    M_CEK_SENET->banka_ismi.clear();
    M_CEK_SENET->borclu_kesideci.clear();
    M_CEK_SENET->cek_senet_numarasi.clear();

}


/**************************************************************************************
                    CEK_SENET_FORMU::NEW_RECORD
***************************************************************************************/

void CEK_SENET_FORMU::NEW_RECORD()
{
    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ 0 ) {
        m_ui->comboBox_sirket->setHidden(true);
        m_ui->label_sirket->setHidden(true);
    }

    m_ui->comboBox_doviz_kodu->setCurrentIndex ( m_ui->comboBox_doviz_kodu->findText(DVZ_GET_TEMEL_PARA_BIRIMI_KODU()) );
}

/**************************************************************************************
                    CEK_SENET_FORMU::GET_RECORD
***************************************************************************************/

int CEK_SENET_FORMU::GET_RECORD ( int cek_senet_id )
{
    SQL_QUERY sql_query ( DB );


    sql_query.PREPARE_SELECT ( "cek_cekler_senetler",
                               "portfoy_numarasi, cek_senet_numarasi, vade_tarihi, keside_tarihi, "
                               "cek_senet_tutari, hesap_no_id, banka_ismi, sube_kodu, sube_ismi, "
                               "doviz_id",
                               "cek_senet_id = :cek_senet_id" );
    sql_query.SET_VALUE      ( ":cek_senet_id", cek_senet_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }
    sql_query.NEXT();

    m_ui->lineedit_portfoy_no->setText ( sql_query.VALUE(0).toString() );

    m_ui->lineedit_cek_no->setText ( sql_query.VALUE(1).toString() );

    m_ui->adakDate_vade_tarihi->SET_DATE( sql_query.VALUE(2).toString());

    m_ui->adakDate_keside_tarihi->SET_DATE( sql_query.VALUE(3).toString()  );

    m_doviz_id = sql_query.VALUE(9).toInt();
    QString doviz_kodu = DVZ_GET_DOVIZ_KODU(m_doviz_id);
    m_ui->comboBox_doviz_kodu->setCurrentIndex ( m_ui->comboBox_doviz_kodu->findText (doviz_kodu ) );


    m_ui->commaedit_tutar->SET_DOUBLE( sql_query.VALUE(4).toDouble() );

    M_CEK_SENET->portfoy_numarasi   = sql_query.VALUE(0).toInt();
    M_CEK_SENET->cek_senet_numarasi = sql_query.VALUE(1).toString();
    M_CEK_SENET->vade_tarihi        = sql_query.VALUE(2).toString();
    M_CEK_SENET->keside_tarihi      = sql_query.VALUE(3).toString();
    M_CEK_SENET->cek_senet_tutari   = sql_query.VALUE(4).toDouble();

    if ( m_cek_senet_turu EQ ENUM_CEK ) {
        m_hesap_no_id = sql_query.VALUE(5).toInt();
        m_banka_ismi  = sql_query.VALUE(6).toString();
        m_sube_kodu   = sql_query.VALUE(7).toString();
        m_sube_ismi   = sql_query.VALUE(8).toString();

        m_ui->lineEdit_doviz_cinsi->setText(doviz_kodu);

        m_ui->lineedit_bnk_sube_bilgisi->setText ( m_banka_ismi + " / "+ m_sube_kodu + "  " +
                                                   m_sube_ismi + "  " + BNK_GET_HESAP_NUMARASI(m_hesap_no_id));

        QString bnk_hesap_kodu =  BNK_GET_BANKA_KODU( m_hesap_no_id );
        m_ui->combobox_banka_hesap_kodu->setCurrentIndex(m_ui->combobox_banka_hesap_kodu->findText ( bnk_hesap_kodu ) );

    }

    M_CEK_SENET->hesap_no_id = m_hesap_no_id;
    M_CEK_SENET->banka_ismi  = m_banka_ismi;
    M_CEK_SENET->sube_ismi   = m_sube_ismi;
    M_CEK_SENET->sube_kodu   = m_sube_kodu;


    return ADAK_OK;
}

/**************************************************************************************
                    CEK_SENET_FORMU::SELECT_RECORD
***************************************************************************************/

int CEK_SENET_FORMU::SELECT_RECORD()
{
    if ( m_cek_senet_turu EQ ENUM_CEK ) {
        return OPEN_CEK_SENET_ARAMA ( ENUM_KENDI_CEKIMIZ,ENUM_CEK,false,-1,-1,this );
    }
    return OPEN_CEK_SENET_ARAMA ( ENUM_KENDI_SENEDIMIZ,ENUM_SENET,false,-1,-1,this );
}

/**************************************************************************************
                    CEK_SENET_FORMU::CHECK_VAR
***************************************************************************************/

int CEK_SENET_FORMU::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->combobox_banka_hesap_kodu  ) {

        int row = -1;

        if ( object EQ m_ui->combobox_banka_hesap_kodu ) {
            row = m_ui->combobox_banka_hesap_kodu->view()->currentIndex().row();
        }
        else {
            int bnk_hesap_id = OPEN_BNK_HESAPLARI_ARAMA(this);
            row = BNK_HESAPLARI_MODEL_INDEX_BUL(bnk_hesap_id , m_ui->combobox_banka_hesap_kodu);

            if ( row NE -1 ) {
                m_ui->combobox_banka_hesap_kodu->setCurrentIndex(row);
            }
        }

        if ( row EQ -1 ) {
            return ADAK_FAIL;
        }

        m_hesap_no_id    = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu , row , 0).toInt();
        int bnk_doviz_cinsi_id = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu , row , 1).toInt();
        QString bnk_doviz_kodu = DVZ_GET_DOVIZ_KODU(bnk_doviz_cinsi_id);
        m_ui->lineEdit_doviz_cinsi->setText(bnk_doviz_kodu);

        m_ui->comboBox_doviz_kodu->setCurrentIndex(m_ui->comboBox_doviz_kodu->findText(bnk_doviz_kodu));


        m_banka_ismi     = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu,row,4).toString();
        m_sube_kodu      = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu,row,5).toString();
        m_sube_ismi      = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu,row,6).toString();
        QString hesap_numarasi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->combobox_banka_hesap_kodu, row, 7).toString();

        QString banka_bilgisi =  m_banka_ismi + " / "+ m_sube_kodu + " " + m_sube_ismi + " " + hesap_numarasi;
        m_ui->lineedit_bnk_sube_bilgisi->setText(banka_bilgisi);
        m_ui->combobox_banka_hesap_kodu->setCurrentIndex(row);

    }
    else if ( object EQ m_ui->commaedit_tutar OR object EQ m_ui->comboBox_doviz_kodu ) {
        m_doviz_id = DVZ_GET_DOVIZ_ID(m_ui->comboBox_doviz_kodu->currentText());
    }

    return ADAK_OK;
}

/**************************************************************************************
                    CEK_SENET_FORMU::CHECK_EMPTY
***************************************************************************************/

int CEK_SENET_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->lineedit_portfoy_no->text().isEmpty() EQ true ) {
         MSG_WARNING( tr ( "Yeni çek / senet kaydı için (*) girmelisiniz!.." ), m_ui->lineedit_portfoy_no );

         return ADAK_FAIL;
    }
    if  ( m_cek_senet_turu EQ ENUM_CEK ) {
        if ( m_ui->combobox_banka_hesap_kodu->currentIndex() EQ -1) {
             MSG_WARNING( tr ( "Banka Hesap Kodunu boş bırakamazsınız!.." ), m_ui->combobox_banka_hesap_kodu );

             return ADAK_FAIL;
        }
    }

    if  ( m_ui->commaedit_tutar->GET_DOUBLE() EQ 0.0 ) {

        if ( m_cek_senet_turu EQ ENUM_CEK ) {
            MSG_WARNING( tr ( "Çek tutarını girmelisiniz!.." ), m_ui->commaedit_tutar );

        }
        else  {
            MSG_WARNING( tr ( "Senet tutarını girmelisiniz!.." ), m_ui->commaedit_tutar );

        }
        return ADAK_FAIL;
    }
    if ( m_ui->lineedit_cek_no->text().isEmpty() EQ true ) {
        if ( m_cek_senet_turu EQ ENUM_CEK ) {
            MSG_WARNING( tr ( "Çek Numarasını boş bırakamazsınız!.." ), m_ui->lineedit_cek_no );

        }
        else {
            MSG_WARNING( tr ( "Senet Numarasını boş bırakamazsınız!.." ), m_ui->lineedit_cek_no );

        }

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                    CEK_SENET_FORMU::CHECK_ADD
***************************************************************************************/

int CEK_SENET_FORMU::CHECK_ADD ()
{
    SQL_QUERY sql_query ( DB );

    int value = DB->LOCK_ROW ( "sabit_deger_id", "e9_sabit_degerler", QString ( "sabit_deger_id = 1" ) );
    if ( value NE ADAK_OK ) {
        return ADAK_FAIL;
    }

    if ( QString ( m_ui->lineedit_portfoy_no->text().at(0)) NE "*" ) {
        int portfoy_numarasi = m_ui->lineedit_portfoy_no->text().toInt();

        sql_query.PREPARE_SELECT ( "cek_cekler_senetler", "cek_senet_id", "portfoy_numarasi = :portfoy_numarasi");
        sql_query.SET_VALUE      ( ":portfoy_numarasi", portfoy_numarasi );
        if ( sql_query.SELECT() NE 0 ) {
            DB->UNLOCK_ROW ( "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = 1" ) );
            MSG_WARNING( tr ( "Aynı portföy numarası ile kayıtlı başka bir çek/senet var."
                                                     "Lütfen portföy numarasını değiştiriniz." ), m_ui->lineedit_portfoy_no );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                    CEK_SENET_FORMU::ADD_RECORD
***************************************************************************************/

int CEK_SENET_FORMU::ADD_RECORD ()
{
    DB->UNLOCK_ROW ( "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = 1" ) );

    int portfoy_numarasi = 0;


    if ( QString( m_ui->lineedit_portfoy_no->text().at(0) ) EQ "*" ) {
        M_CEK_SENET->portfoy_numarasi = 0;
    }
    else {
        M_CEK_SENET->portfoy_numarasi = m_ui->lineedit_portfoy_no->text().toInt();
    }

    M_CEK_SENET->banka_ismi         = m_banka_ismi;
    M_CEK_SENET->sube_ismi          = m_sube_ismi;
    M_CEK_SENET->sube_kodu          = m_sube_kodu;
    M_CEK_SENET->hesap_no_id        = m_hesap_no_id;
    M_CEK_SENET->cek_senet_numarasi = m_ui->lineedit_cek_no->text();
    M_CEK_SENET->vade_tarihi        = m_ui->adakDate_vade_tarihi->DATE();
    M_CEK_SENET->keside_tarihi      = m_ui->adakDate_keside_tarihi->DATE();
    M_CEK_SENET->musteri_ceki_mi    = 0;
    M_CEK_SENET->cek_senet_tutari   = m_ui->commaedit_tutar->GET_DOUBLE();
    M_CEK_SENET->cek_senet_turu     = m_cek_senet_turu;
    M_CEK_SENET->doviz_id           = m_doviz_id;

    int cek_id = CEK_SENET_EKLE ( M_CEK_SENET );

    CEK_SENET_HAREKETINI_KAYDET  ( cek_id, -1, QDate::fromString ( M_CEK_SENET->keside_tarihi, "yyyy.MM.dd" ) );

    QString log_detaylari;

    if ( m_cek_senet_turu EQ ENUM_CEK ) {
        log_detaylari = "Log Türü - Kendi Çek / Senetlerimiz , Çek";
        log_detaylari.append ( ",Portföy No : " + QVariant(portfoy_numarasi).toString() + "," +
                        m_ui->combobox_banka_hesap_kodu->currentText() + " " + m_ui->lineedit_bnk_sube_bilgisi->text() + "  "
                        + m_ui->lineEdit_doviz_cinsi->text() + "\nÇek Numarası :" +
                        m_ui->lineedit_cek_no->text() + "Vade Tarihi :" +
                        m_ui->adakDate_vade_tarihi->QDATE().toString("dd MMMM yyyy") + "Keşide Tarihi :"
                        + m_ui->adakDate_keside_tarihi->QDATE().toString("dd MMMM yyyy") + "Çek Tutarı :"
                        + m_ui->commaedit_tutar->GET_TEXT());

    }
    else {
        log_detaylari = "Log Türü - Kendi Çek / Senetlerimiz ,Senet";
        log_detaylari.append ( ",Portföy No : " + QVariant ( portfoy_numarasi).toString() + ", Senet Numarası :" +
                        m_ui->lineedit_cek_no->text() + "Vade Tarihi :" +
                        m_ui->adakDate_vade_tarihi->QDATE().toString("dd MMMM YYYY") + "Keşide Tarihi :"
                        + m_ui->adakDate_keside_tarihi->QDATE().toString("dd MMMM YYYY") + "Senet Tutarı :"
                        + m_ui->commaedit_tutar->GET_TEXT() );
    }

    E9_LOG_KAYDI_EKLE ( CEKSENET_MODULU , LOG_CEK_SENETLER , LOG_ISLEM_ADD , log_detaylari ) ;

    return cek_id;

}

/**************************************************************************************
                    CEK_SENET_FORMU::CHECK_UPDATE
***************************************************************************************/

int CEK_SENET_FORMU::CHECK_UPDATE ( int record_id )
{
    SQL_QUERY sql_query  ( DB );

    if ( QString ( m_ui->lineedit_portfoy_no->text().at(0) ) EQ "*" ) {
        MSG_WARNING( tr ( "Portföy numarasını güncellemek için portföy numarası kısmına sadece sayı girebilirsiniz."), m_ui->lineedit_portfoy_no );

        return ADAK_FAIL;
    }

    int lock_return_value = DB->LOCK_ROW (  "sabit_deger_id", "e9_sabit_degerler", QString ( "sabit_deger_id = 1" ));

    if ( lock_return_value NE ADAK_OK ) {
        return ADAK_FAIL;
    }

    int portfoy_numarasi = m_ui->lineedit_portfoy_no->text().toInt();

    sql_query.PREPARE_SELECT ( "cek_cekler_senetler", "cek_senet_id","portfoy_numarasi = :portfoy_numarasi");
    sql_query.SET_VALUE      ( ":portfoy_numarasi" ,portfoy_numarasi );
    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        if ( sql_query.VALUE(0).toInt() NE record_id ) {
            DB->UNLOCK_ROW ( "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = 1" ));
            MSG_WARNING( tr ( "Aynı portföy numarası ile kayıtlı başka bir çek/senet var. "
                                                 "Lütfen portföy numarasını değiştiriniz."), m_ui->lineedit_portfoy_no );

            return ADAK_FAIL;

        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                    CEK_SENET_FORMU::UPDATE_RECORD
***************************************************************************************/

void CEK_SENET_FORMU::UPDATE_RECORD (int cek_senet_id)
{
    DB->UNLOCK_ROW ( "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = 1" ));

    M_CEK_SENET->portfoy_numarasi = m_ui->lineedit_portfoy_no->text().toInt();
    M_CEK_SENET->banka_ismi       = m_banka_ismi;
    M_CEK_SENET->sube_kodu        = m_sube_kodu;
    M_CEK_SENET->sube_ismi        = m_sube_ismi;
    M_CEK_SENET->hesap_no_id      = m_hesap_no_id;
    M_CEK_SENET->cek_senet_numarasi = m_ui->lineedit_cek_no->text();
    M_CEK_SENET->vade_tarihi        = m_ui->adakDate_vade_tarihi->DATE();
    M_CEK_SENET->keside_tarihi      = m_ui->adakDate_keside_tarihi->DATE();
    M_CEK_SENET->cek_senet_tutari   = m_ui->commaedit_tutar->GET_DOUBLE();
    M_CEK_SENET->doviz_id           = m_doviz_id;

    CEK_SENET_GUNCELLE( M_CEK_SENET , cek_senet_id);

    QString log_detaylari;

    if ( m_cek_senet_turu EQ ENUM_CEK ) {
        log_detaylari = "Log Türü - Kendi Çek / Senetlerimiz , Çek";
        log_detaylari.append (  ",Portföy No : " + QVariant(M_CEK_SENET->portfoy_numarasi).toString() + "," +
                        m_ui->combobox_banka_hesap_kodu->currentText() + " " + m_ui->lineedit_bnk_sube_bilgisi->text() + " "
                        + m_ui->lineEdit_doviz_cinsi->text() + "\nÇek Numarası :" +
                        m_ui->lineedit_cek_no->text() + "Vade Tarihi :" +
                        m_ui->adakDate_vade_tarihi->QDATE().toString("dd MMMM YYYY") + "Keşide Tarihi :"
                        + m_ui->adakDate_keside_tarihi->QDATE().toString("dd MMMM YYYY") + "Çek Tutarı :"
                        + m_ui->commaedit_tutar->GET_TEXT() );

    }
    else {
        log_detaylari = "Log Türü - Kendi Çek / Senetlerimiz ,Senet";
        log_detaylari.append (  ",Portföy No : " + QVariant ( M_CEK_SENET->portfoy_numarasi).toString() + ", Senet Numarası :" +
                        m_ui->lineedit_cek_no->text() + "Vade Tarihi :" +
                        m_ui->adakDate_vade_tarihi->QDATE().toString("dd MMMM YYYY") + "Keşide Tarihi :"
                        + m_ui->adakDate_keside_tarihi->QDATE().toString("dd MMMM YYYY") + "Senet Tutarı :"
                        + m_ui->commaedit_tutar->GET_TEXT() );
    }

    E9_LOG_KAYDI_EKLE ( CEKSENET_MODULU , LOG_CEK_SENETLER , LOG_ISLEM_UPDATE , log_detaylari );

}

/**************************************************************************************
                    CEK_SENET_FORMU::CHECK_DELETE
***************************************************************************************/

int CEK_SENET_FORMU::CHECK_DELETE ( int record_id )
{
    //cek fislerde kullanilmis ise silinememelidir

    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "cek_bordro_satirlari","cek_senet_id ","cek_senet_id = :cek_senet_id");
    select_query.SET_VALUE      ( ":cek_senet_id" , record_id );
    if (select_query.SELECT() NE 0 ) {
        MSG_WARNING( tr ( "Silmek istediğiniz çek işlem görmüştür,silinemez!.." ), NULL );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                    CEK_SENET_FORMU::DELETE_RECORD
***************************************************************************************/

void CEK_SENET_FORMU::DELETE_RECORD ( int cek_senet_id )
{
    CEK_SENET_SIL ( cek_senet_id );

    QDate vade_tarihi    = QDate::fromString(M_CEK_SENET->vade_tarihi , "yyyy.MM.dd");
    QDate keside_tarihi  = QDate::fromString(M_CEK_SENET->keside_tarihi , "yyyy.MM.dd");

    QString log_detaylari;

    if ( m_cek_senet_turu EQ ENUM_CEK ) {
        log_detaylari = "Log Türü - Kendi Çek / Senetlerimiz , Çek";
        log_detaylari.append (  ",Portföy No : " + QVariant(M_CEK_SENET->portfoy_numarasi).toString() + "," +
                        M_CEK_SENET->banka_ismi + "," + M_CEK_SENET->sube_kodu + " "
                        + M_CEK_SENET->sube_ismi +
                        BNK_GET_HESAP_NUMARASI ( M_CEK_SENET->hesap_no_id) + ",Çek Numarası :" +
                        M_CEK_SENET->cek_senet_numarasi + "Vade Tarihi :" +
                        vade_tarihi.toString("dd MMMM yyyy") + "Keşide Tarihi :"
                        + keside_tarihi.toString ( "dd MMMM yyyy" ) + "Çek Tutarı :"
                        + VIRGUL_EKLE ( QVariant ( ROUND ( M_CEK_SENET->cek_senet_tutari) ).toString() ) );

    }
    else {
        log_detaylari = "Log Türü - Kendi Çek / Senetlerimiz ,Senet";
        log_detaylari.append (  ",Portföy No : " + QVariant(M_CEK_SENET->portfoy_numarasi).toString()
                                + ", Senet Numarası :" +
                        M_CEK_SENET->cek_senet_numarasi + "Vade Tarihi :" +
                        vade_tarihi.toString ( "dd MMMM yyyy") + "Keşide Tarihi :"
                        + keside_tarihi.toString ( "dd MMMM yyyy") + "Senet Tutarı :"
                        + VIRGUL_EKLE ( QVariant ( ROUND ( M_CEK_SENET->cek_senet_tutari ) ).toString() ) );
    }

    E9_LOG_KAYDI_EKLE ( CEKSENET_MODULU , LOG_CEK_SENETLER , LOG_ISLEM_DELETE , log_detaylari );

}

/**************************************************************************************
                    CEK_SENET_FORMU::FIND_RECORD
***************************************************************************************/

int CEK_SENET_FORMU::FIND_RECORD()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "cek_cekler_senetler", "cek_senet_id",
                           "portfoy_numarasi            = :portfoy_numarasi "
                           "AND musteri_ceki_mi         = :musteri_ceki_mi "
                           "AND cek_senet_turu          = :cek_senet_turu "
                           "AND cek_senet_son_durumu    = :cek_senet_son_durumu" );

    query.SET_VALUE      ( ":portfoy_numarasi"     , m_ui->lineedit_portfoy_no->text().toInt() );
    query.SET_VALUE      ( ":cek_senet_turu"       , m_cek_senet_turu );
    query.SET_VALUE      ( ":musteri_ceki_mi"      , 0 );
    query.SET_VALUE      ( ":cek_senet_son_durumu" , ENUM_PORTFOYDE );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING( tr ( "Aradığınız çek/senet bulunamadı!.." ), m_ui->lineedit_portfoy_no );

        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                    CEK_SENET_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int CEK_SENET_FORMU::FIND_FIRST_RECORD()
{
    QString sql_query = QString("musteri_ceki_mi = :musteri_ceki_mi "
                                "AND cek_senet_turu = :cek_senet_turu AND cek_senet_son_durumu = :cek_senet_son_durumu ");

    SQL_QUERY query  ( DB );

    query.PREPARE_SELECT ( "cek_cekler_senetler" ,"cek_senet_id" , sql_query );

    query.SET_VALUE            ( ":cek_senet_turu"       , m_cek_senet_turu );
    query.SET_VALUE            ( ":cek_senet_son_durumu" , ENUM_PORTFOYDE );
    query.SET_VALUE            ( ":musteri_ceki_mi"      , 0 );

    if ( query.SELECT( "portfoy_numarasi", 0, 1 ) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                    CEK_SENET_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int CEK_SENET_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY query ( DB );

    QString sql_query = QString("musteri_ceki_mi = :musteri_ceki_mi "
                               "AND cek_senet_turu = :cek_senet_turu AND cek_senet_son_durumu = :cek_senet_son_durumu ");

    query.PREPARE_SELECT ( "cek_cekler_senetler" ,"cek_senet_id" , sql_query );

    query.SET_VALUE             ( ":cek_senet_turu"         , m_cek_senet_turu );
    query.SET_VALUE             ( ":cek_senet_son_durumu"   , ENUM_PORTFOYDE );
    query.SET_VALUE             ( ":musteri_ceki_mi"        , 0 );

    if ( query.SELECT( "portfoy_numarasi DESC", 0, 1 ) EQ 0){
       return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                    CEK_SENET_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int CEK_SENET_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY query ( DB );

    QString sql_query = QString("musteri_ceki_mi = :musteri_ceki_mi "
                                "AND portfoy_numarasi > :portfoy_numarasi AND cek_senet_turu = :cek_senet_turu "
                                "AND cek_senet_son_durumu = :cek_senet_son_durumu ");

    query.PREPARE_SELECT ( "cek_cekler_senetler" ,"cek_senet_id" , sql_query );

    query.SET_VALUE            ( ":portfoy_numarasi"        , m_ui->lineedit_portfoy_no->text().toInt() );
    query.SET_VALUE            ( ":cek_senet_turu"          , m_cek_senet_turu );
    query.SET_VALUE            ( ":cek_senet_son_durumu"    , ENUM_PORTFOYDE );
    query.SET_VALUE            ( ":musteri_ceki_mi"         , 0 );

    if ( query.SELECT(  "portfoy_numarasi" , 0 ,1 ) EQ 0){
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                    CEK_SENET_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int CEK_SENET_FORMU::FIND_PREV_RECORD ()
{
    SQL_QUERY query ( DB );

    QString sql_query = QString("musteri_ceki_mi = :musteri_ceki_mi "
                                "AND portfoy_numarasi     < :portfoy_numarasi AND cek_senet_turu = :cek_senet_turu "
                                "AND cek_senet_son_durumu = :cek_senet_son_durumu ");


    query.PREPARE_SELECT ( "cek_cekler_senetler" ,"cek_senet_id" , sql_query );

    query.SET_VALUE            ( ":portfoy_numarasi"   , m_ui->lineedit_portfoy_no->text().toInt() );
    query.SET_VALUE            ( ":cek_senet_turu"     , m_cek_senet_turu );
    query.SET_VALUE            ( ":cek_senet_son_durumu" , ENUM_PORTFOYDE );
    query.SET_VALUE            ( ":musteri_ceki_mi" , 0 );

    if ( query.SELECT( "portfoy_numarasi DESC", 0, 1) EQ 0){
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                    CEK_SENET_FORMU::LOCK_RECORD
***************************************************************************************/

int CEK_SENET_FORMU::LOCK_RECORD ( int record_id )
{
    return DB->LOCK_ROW  ( "cek_senet_id","cek_cekler_senetler", QString ( "cek_senet_id = %1" ).arg ( record_id ) );

}

/**************************************************************************************
                    CEK_SENET_FORMU::UNLOCK_RECORD
***************************************************************************************/

void CEK_SENET_FORMU::UNLOCK_RECORD ( int record_id )
{
    DB->UNLOCK_ROW ( "cek_senet_id","cek_cekler_senetler", QString ("cek_senet_id = %1").arg ( record_id ) );
}


