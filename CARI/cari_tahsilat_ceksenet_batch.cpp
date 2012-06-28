#include <QMessageBox>
#include <QDate>
#include "e9_enum.h"
#include "e9_struct.h"
#include "bnk_console_utils.h"
#include "adak_std_utils.h"
#include "cari_tahsilat_ceksenet_batch_class.h"
#include "cari_tahsilat_ceksenet_batch_open.h"
#include "ui_cari_tahsilat_ceksenet_batch.h"
#include "cek_console_utils.h"
 
#include "cek_senet_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_CARI_TAHSILAT_CEKSENET_BATCH
***************************************************************************************/

bool OPEN_CARI_TAHSILAT_CEKSENET_BATCH ( int fis_satiri_id,int islem_turu,
                                         E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI,bool islem_turu_degisti,QWidget * parent )
{
    bool kayit_degistirildimi = false;
    CARI_TAHSILAT_CEKSENET_BATCH * B = new CARI_TAHSILAT_CEKSENET_BATCH ( fis_satiri_id ,islem_turu,
                                                                          P_ISLEM_DETAYLARI, islem_turu_degisti,
                                                                          parent );
    B->m_kayit_degistirildi_mi = &kayit_degistirildimi;

    B->EXEC( NOT_FULL_SCREEN );

    return kayit_degistirildimi;
}

/**************************************************************************************
                   CARI_TAHSILAT_CEKSENET_BATCH::CARI_TAHSILAT_CEKSENET_BATCH
***************************************************************************************/

CARI_TAHSILAT_CEKSENET_BATCH::CARI_TAHSILAT_CEKSENET_BATCH ( int fis_satiri_id , int islem_turu,
                                                             E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI
                                                             , bool islem_turu_degisti, QWidget * parent ) :  BATCH_KERNEL (parent), m_ui(new Ui::CARI_TAHSILAT_CEKSENET_BATCH)
{
    m_islem_turu_degisti = islem_turu_degisti;
    m_fis_satiri_id   = fis_satiri_id;
    m_islem_turu      = islem_turu;

    M_ISLEM_DETAYLARI = P_ISLEM_DETAYLARI;

    m_ui->setupUi      (this );
    START_BATCH_KERNEL (this, DB );
}

/**************************************************************************************
             CARI_TAHSILAT_CEKSENET_BATCH::SETUP_FORM
***************************************************************************************/

void CARI_TAHSILAT_CEKSENET_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE     ( "cari-tahsilat-ceksenet"       );
    SET_PAGE_TITLE    ( tr ( "CARİ - ÇEK SENET BİLGİLERİ" )   );
    SET_SETTING_NAME  ( "CARI_TAHSILAT_CEKSENET_BATCH" );

    REGISTER_BUTTON_WIDGET  ( m_ui->run_batch_widget );

    SQL_QUERY query ( DB );

    m_ui->adakDate_vade_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_keside_tarihi->MALI_YIL_KISITI_YOK( true );

    m_ui->adakDate_keside_tarihi->SET_DATE( QDate::currentDate() );
    m_ui->adakDate_vade_tarihi->SET_DATE( QDate::currentDate() );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Tamam" );
    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_vade_tarihi);

    SET_AUTO_EXIT_BATCH ( true );

    if ( m_islem_turu EQ ENUM_MUSTERI_SENEDI ) {
        m_ui->frame_musteri_banka_bilgisi->hide();
        setWindowTitle ( "Senet Bilgileri" );
        m_ui->label_cek_senet_no->setText ( "Senet Numarası" );
        m_ui->label_cek_senet_bilgileri->setText ( "Senet Bilgileri" );
        m_ui->label_kesideci->setText ( "Senedin Borçlusu" );
        m_ui->label_blg_kesideci->setText ( "" );
    }
    else {
        GET_BANKALAR ( m_ui->combobox_banka_ismi );
        setWindowTitle ( "Çek Bilgileri" );
        m_ui->label_cek_senet_no->setText ( "Çek Numarası" );
        m_ui->label_cek_senet_bilgileri->setText ( "Çek Bilgileri" );
    }
    m_ui->line_edit_portfoy_no->setText("*");

    if ( m_fis_satiri_id EQ -1 OR m_islem_turu_degisti EQ 1) {

        m_ui->adakDate_vade_tarihi->SET_DATE(M_ISLEM_DETAYLARI->vade_tarihi);
        m_ui->line_edit_valor->setText(M_ISLEM_DETAYLARI->valor);
        m_ui->line_edit_cek_senet_no->setText(M_ISLEM_DETAYLARI->cek_senet_no);
        m_ui->combobox_banka_ismi->setEditText(M_ISLEM_DETAYLARI->banka_ismi);
        m_ui->lineedit_sube_kodu->setText(M_ISLEM_DETAYLARI->sube_kodu);
        m_ui->combobox_sube_ismi->setEditText(M_ISLEM_DETAYLARI->sube_adi);
        m_ui->lineedit_hesap_no->setText(M_ISLEM_DETAYLARI->hesap_no);
        m_ui->adakDate_keside_tarihi->SET_DATE(M_ISLEM_DETAYLARI->keside_tarihi);
        m_ui->lineedit_kesideci->setText(M_ISLEM_DETAYLARI->kesideci);

    }
    else {

        query.PREPARE_SELECT ( "car_fis_satirlari",
                               "cek_senet_id",
                               "fis_satiri_id = :fis_satiri_id");
        query.SET_VALUE      ( ":fis_satiri_id" , m_fis_satiri_id );
        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            m_cek_senet_id = query.VALUE(0).toInt();
        }
        if ( m_cek_senet_id > 0 ) {

            CEK_SENET_STRUCT CEK_STRUCT;

            CEK_GET_TEMEL_BILGILER( &CEK_STRUCT, m_cek_senet_id );

            m_ui->line_edit_portfoy_no->setText ( QVariant ( CEK_STRUCT.portfoy_numarasi ).toString() );
            m_ui->adakDate_vade_tarihi->SET_DATE( CEK_STRUCT.vade_tarihi );
            m_ui->line_edit_valor->setText ( QVariant(CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI( m_ui->adakDate_vade_tarihi->QDATE())).toString()  );
            m_ui->line_edit_cek_senet_no->setText( QVariant ( CEK_STRUCT.cek_senet_numarasi ).toString() );
            m_ui->combobox_banka_ismi->setCurrentIndex ( m_ui->combobox_banka_ismi->findText ( CEK_STRUCT.banka_ismi ));
            GET_BANKA_SUBE_ADLARI(m_ui->combobox_sube_ismi,m_ui->combobox_banka_ismi->currentText());

            m_ui->combobox_sube_ismi->setCurrentIndex ( m_ui->combobox_sube_ismi->findText ( ( CEK_STRUCT.sube_ismi ) ) );
            m_ui->lineedit_sube_kodu->setText ( CEK_STRUCT.sube_kodu );
            m_ui->lineedit_hesap_no->setText ( CEK_STRUCT.musteri_hesap_numarasi );
            m_ui->adakDate_keside_tarihi->SET_DATE( CEK_STRUCT.keside_tarihi );
            m_ui->lineedit_kesideci->setText ( CEK_STRUCT.borclu_kesideci );
        }
    }
}

/**************************************************************************************
             CARI_TAHSILAT_CEKSENET_BATCH::CHECK_VAR
***************************************************************************************/

int CARI_TAHSILAT_CEKSENET_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->combobox_banka_ismi ) {

        m_ui->combobox_sube_ismi->clear();
        m_ui->lineedit_sube_kodu->clear();

        GET_BANKA_SUBE_ADLARI(m_ui->combobox_sube_ismi,m_ui->combobox_banka_ismi->currentText());
    }
    if  ( object EQ m_ui->combobox_sube_ismi) {
        QString sube_ismi = m_ui->combobox_sube_ismi->currentText();

        m_ui->combobox_sube_ismi->setCurrentIndex ( m_ui->combobox_sube_ismi->findText ( ( sube_ismi ) ) );

        m_ui->lineedit_sube_kodu->setText ( GET_BANKA_SUBE_KODU( m_ui->combobox_banka_ismi->currentText(),sube_ismi ) );
    }

    if ( object EQ m_ui->adakDate_vade_tarihi) {
        m_ui->line_edit_valor->setText ( QVariant ( CEK_GET_VADE_TARIHINE_KALAN_GUN_SAYISI( m_ui->adakDate_vade_tarihi->QDATE())).toString());
    }

    if ( object EQ m_ui->combobox_banka_ismi OR object EQ m_ui->line_edit_cek_senet_no OR
         object EQ m_ui->lineedit_hesap_no OR object EQ m_ui->lineedit_kesideci OR
         object EQ m_ui->adakDate_keside_tarihi OR object EQ m_ui->line_edit_portfoy_no OR
         object EQ m_ui->combobox_sube_ismi OR object EQ m_ui->lineedit_sube_kodu OR
         object EQ m_ui->adakDate_vade_tarihi OR object EQ m_ui->line_edit_valor ) {
        *m_kayit_degistirildi_mi = true;

    }

    return ADAK_OK;
}

/**************************************************************************************
             CARI_TAHSILAT_CEKSENET_BATCH::CHECK_RUN
***************************************************************************************/

int CARI_TAHSILAT_CEKSENET_BATCH::CHECK_RUN()
{
    switch ( m_islem_turu ) {
        case ENUM_MUSTERI_CEKI    :
            if ( m_ui->line_edit_cek_senet_no->text().isEmpty() EQ true) {
                MSG_WARNING( tr ( "Çek Numarasını boş bırakamazsınız!.."),  m_ui->line_edit_cek_senet_no );
                 return ADAK_FAIL;
            }

            if ( m_ui->combobox_banka_ismi->currentText().isEmpty() EQ true ) {
                MSG_WARNING( tr ( "Müşterinin banka bilgisini girmelisiniz!.."), m_ui->combobox_banka_ismi );

                 return ADAK_FAIL;
            }
            if ( m_ui->combobox_sube_ismi->currentText().isEmpty() EQ true) {
                MSG_WARNING( tr ( "Bankanın şubesini girmelisiniz!.."), m_ui->combobox_sube_ismi );

                 return ADAK_FAIL;
            }
            if ( m_ui->lineedit_hesap_no->text().isEmpty() EQ true ) {
               MSG_WARNING( tr ( "Hesap numarasını boş bırakamazsınız!.."), m_ui->lineedit_hesap_no );

                return ADAK_FAIL;
            }
            if ( m_ui->lineedit_kesideci->text().isEmpty() EQ true) {
               MSG_WARNING( tr ( "Çekin keşidecisini boş bırakamazsınız!.."), m_ui->lineedit_kesideci );

                return ADAK_FAIL;
            }
            break;
        case ENUM_MUSTERI_SENEDI  :
            if ( m_ui->line_edit_cek_senet_no->text().isEmpty() EQ true) {
                MSG_WARNING( tr ( "Senet numarasını boş bırakamazsınız!.. " ), m_ui->line_edit_cek_senet_no );

                 return ADAK_FAIL;
            }
            if ( m_ui->lineedit_kesideci->text().isEmpty() EQ true ) {
                MSG_WARNING( tr ( "Keşideci bilgisini girmelisiniz!.." ), m_ui->lineedit_kesideci );

                 return ADAK_FAIL;
            }
            break;
    };
    return ADAK_OK;
}

/**************************************************************************************
             CARI_TAHSILAT_CEKSENET_BATCH::RUN_BATCH
***************************************************************************************/

void CARI_TAHSILAT_CEKSENET_BATCH::RUN_BATCH()
{
    if ( *m_kayit_degistirildi_mi EQ true ) {
         M_ISLEM_DETAYLARI->banka_ismi        = m_ui->combobox_banka_ismi->currentText();
         M_ISLEM_DETAYLARI->cek_senet_no      = m_ui->line_edit_cek_senet_no->text();
         M_ISLEM_DETAYLARI->hesap_no          = m_ui->lineedit_hesap_no->text();
         M_ISLEM_DETAYLARI->kesideci          = m_ui->lineedit_kesideci->text();
         M_ISLEM_DETAYLARI->keside_tarihi     = m_ui->adakDate_keside_tarihi->QDATE();
         M_ISLEM_DETAYLARI->portfoy_no        = m_ui->line_edit_portfoy_no->text();
         M_ISLEM_DETAYLARI->sube_kodu         = m_ui->lineedit_sube_kodu->text();
         M_ISLEM_DETAYLARI->sube_adi          = m_ui->combobox_sube_ismi->currentText();
         M_ISLEM_DETAYLARI->vade_tarihi       = m_ui->adakDate_vade_tarihi->QDATE();
         M_ISLEM_DETAYLARI->valor             = m_ui->line_edit_valor->text();
    }
    close();
}

/**************************************************************************************
             CARI_TAHSILAT_CEKSENET_BATCH::CHECK_EXIT
***************************************************************************************/

int CARI_TAHSILAT_CEKSENET_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}
