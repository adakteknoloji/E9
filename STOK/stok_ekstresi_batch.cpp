#include "stok_ekstresi_batch_class.h"
#include "ui_stok_ekstresi_batch.h"
#include "print.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_struct.h"
#include "sube_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "stok_ekstresi_raporu_open.h"
#include "stok_urun_arama_open.h"
#include "sube_depo_secimi_open.h"
#include "stok_enum.h"
#include "sube_enum.h"
#include "stok_gui_utils.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_STOK_EKSTRESI_BATCH
***************************************************************************************/

void OPEN_STOK_EKSTRESI_BATCH ( QWidget * parent)
{
    STOK_EKSTRESI_BATCH * F = new STOK_EKSTRESI_BATCH ( parent);
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   STOK_EKSTRESI_BATCH::STOK_EKSTRESI_BATCH
***************************************************************************************/

STOK_EKSTRESI_BATCH::STOK_EKSTRESI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::STOK_EKSTRESI_BATCH )
{
    m_ui->setupUi       ( this );

    START_BATCH_KERNEL  ( this, DB );
}

/**************************************************************************************
                   STOK_EKSTRESI_BATCH::SETUP_FORM
***************************************************************************************/

void STOK_EKSTRESI_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "STK - STOK EKSTRESİ" ) );
    SET_HELP_PAGE     ("stok-islemleri_stok-ekstresi");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    m_ui->adakDate_bsl_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
    m_ui->adakDate_bts_tarihi->SET_DATE(MALI_YIL_365_DATE());

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );

    SET_FIRST_FOCUS_WIDGET ( m_ui->search_edit_urun_kodu );

    STK_FILL_ALIS_SATIS_DURUMU_COMBO_BOX ( m_ui->comboBox_stok_alis_satis_durumu );

    SET_ENTER_KEY_FOR_RUN_BATCH(true);    

    SET_SETTING_NAME  ("STOK_EKSTRESI_BATCH");

    m_ui->checkBox_urun_kodu->setChecked( true );
    m_ui->frame_urun->setEnabled( true );

    m_ui->checkBox_urun_araligi->setChecked( false );
    m_ui->frame_urun_araligi->setEnabled( false );

    m_depo_id = SUBE_GET_UNITENIN_DEPOSU();

    m_depo_islem_yetkisi = SUBE_GET_DEPO_ISLEM_YETKISI();

    if ( m_depo_islem_yetkisi EQ SADECE_UNITE_DEPOSUNDA ) {

        m_ui->frame_depo->setHidden(true);
        m_ui->checkBox_depo->setHidden(true);
    }
    else {
        QString depo_kodu;
        QString depo_adi;

        SUBE_GET_DEPO_KODU_ADI(m_depo_id, &depo_kodu, &depo_adi);

        m_ui->searchEdit_depo_kodu->SET_TEXT(depo_kodu);
        m_ui->lineedit_depo_ismi->setText(depo_adi);
        m_ui->frame_depo->setDisabled(true);
    }

}

/**************************************************************************************
                   STOK_EKSTRESI_BATCH::CHECK_VAR
***************************************************************************************/

int STOK_EKSTRESI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchEdit_bas_urun_kodu OR object EQ m_ui->searchEdit_bts_urun_kodu OR
         object EQ m_ui->search_edit_urun_kodu) {

        if ( object EQ m_ui->searchEdit_bas_urun_kodu ) {
            if ( m_ui->searchEdit_bas_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bas_urun_ismi->clear();
                return ADAK_OK;
            }
        }
        else if (object EQ m_ui->searchEdit_bts_urun_kodu ) {
            if ( m_ui->searchEdit_bts_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bts_urun_ismi->clear();
                return ADAK_OK;
            }
        }
        else {
            if ( m_ui->search_edit_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_urun_ismi->clear();
                return ADAK_OK;
            }
        }

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("stk_urunler","urun_adi",
                             "(urun_kodu = :urun_kodu OR "
                             "barkod_numarasi = :barkod_numarasi) AND hizmet_mi = 0");

        if ( object EQ m_ui->searchEdit_bas_urun_kodu ) {
            query.SET_VALUE(":urun_kodu"       , m_ui->searchEdit_bas_urun_kodu->GET_TEXT());
            query.SET_VALUE(":barkod_numarasi" , m_ui->searchEdit_bas_urun_kodu->GET_TEXT());
        }
        else if ( object EQ m_ui->searchEdit_bts_urun_kodu ) {
            query.SET_VALUE(":urun_kodu"       , m_ui->searchEdit_bts_urun_kodu->GET_TEXT());
            query.SET_VALUE(":barkod_numarasi" , m_ui->searchEdit_bts_urun_kodu->GET_TEXT());
        }
        else {
            query.SET_VALUE(":urun_kodu" , m_ui->search_edit_urun_kodu->GET_TEXT());
            query.SET_VALUE(":barkod_numarasi" , m_ui->search_edit_urun_kodu->GET_TEXT());
        }


        if ( query.SELECT() EQ 0 ) {
            if ( object EQ m_ui->searchEdit_bas_urun_kodu ) {
                MSG_WARNING(  tr("Girmiş olduğunuz başlangıç ürün kodu bulunamadı."), m_ui->searchEdit_bas_urun_kodu );
                return ADAK_FAIL;

            }
            else if ( object EQ m_ui->searchEdit_bts_urun_kodu ) {
                MSG_WARNING(  tr("Girmiş olduğunuz bitiş ürün kodu bulunamadı."), m_ui->searchEdit_bts_urun_kodu );
                return ADAK_FAIL;
            }
            else {
                MSG_WARNING(  tr("Girmiş olduğunuz ürün kodu bulunamadı."), m_ui->search_edit_urun_kodu );

                return ADAK_FAIL;
            }
        }
        query.NEXT();
        QString urun_adi = query.VALUE(0).toString();

        if ( object EQ m_ui->searchEdit_bas_urun_kodu ) {
            m_ui->lineedit_bas_urun_ismi->setText(urun_adi);
        }
        else if (object EQ m_ui->searchEdit_bts_urun_kodu){
            m_ui->lineedit_bts_urun_ismi->setText(urun_adi);
        }
        else {
            m_ui->lineedit_urun_ismi->setText(urun_adi);
        }
    }
    if ( object EQ m_ui->checkBox_urun_kodu ) {

        if ( m_ui->checkBox_urun_kodu->isChecked() EQ true ) {
            m_ui->frame_urun->setEnabled(true);
            m_ui->checkBox_urun_araligi->setCheckState(Qt::Unchecked);
            m_ui->searchEdit_bas_urun_kodu->SET_TEXT("");
            m_ui->lineedit_bas_urun_ismi->clear();
            m_ui->searchEdit_bts_urun_kodu->SET_TEXT("");
            m_ui->lineedit_bts_urun_ismi->clear();
            m_ui->frame_urun_araligi->setDisabled(true);
            SET_FOCUS( m_ui->search_edit_urun_kodu );
        }
        else {
            m_ui->frame_urun->setDisabled(true);
            m_ui->search_edit_urun_kodu->SET_TEXT("");
            m_ui->lineedit_urun_ismi->clear();
            m_ui->frame_urun_araligi->setEnabled(true);
            m_ui->checkBox_urun_araligi->setCheckState(Qt::Checked);
        }
    }

    if ( object EQ m_ui->searchEdit_depo_kodu ) {

        if ( m_ui->searchEdit_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineedit_depo_ismi->clear();
            m_depo_id = -1;
            return ADAK_OK;
        }

        QString depo_adi;
        int     depo_id;

        int result = SUBE_GET_DEPO_ID_ADI( m_ui->searchEdit_depo_kodu->GET_TEXT(), &depo_id, &depo_adi );

        if ( result EQ 0 ) {
            MSG_WARNING( tr("Aradığınız depo kodu bulunamadı."), m_ui->searchEdit_depo_kodu );
            return ADAK_FAIL;
        }
        m_ui->lineedit_depo_ismi->setText( depo_adi );
        m_depo_id = depo_id;
    }

    if ( object EQ m_ui->checkBox_urun_araligi ) {

        if ( m_ui->checkBox_urun_araligi->isChecked() EQ true ) {
            m_ui->frame_urun_araligi->setEnabled(true);
            m_ui->checkBox_urun_kodu->setCheckState(Qt::Unchecked);
            m_ui->frame_urun->setDisabled(true);
            m_ui->search_edit_urun_kodu->SET_TEXT("");
            m_ui->lineedit_urun_ismi->clear();
            SET_FOCUS( m_ui->searchEdit_bas_urun_kodu );
        }
        else {
            m_ui->frame_urun->setEnabled(true);
            m_ui->frame_urun_araligi->setDisabled(true);
            m_ui->searchEdit_bas_urun_kodu->SET_TEXT("");
            m_ui->lineedit_bas_urun_ismi->clear();
            m_ui->searchEdit_bts_urun_kodu->SET_TEXT("");
            m_ui->lineedit_bts_urun_ismi->clear();
            m_ui->checkBox_urun_kodu->setCheckState(Qt::Checked);
        }
    }
    else if ( object EQ m_ui->checkBox_tarih_araligi ) {

        if ( m_ui->checkBox_tarih_araligi->isChecked() EQ false ) {
             m_ui->adakDate_bsl_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
             m_ui->adakDate_bts_tarihi->SET_DATE( MALI_YIL_365_DATE() );
             m_ui->frame_tarih_araligi->setDisabled ( true );
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled ( true );
            SET_FOCUS( m_ui->adakDate_bsl_tarihi );
        }
    }
    else if ( object EQ m_ui->checkBox_nakli_yekun ) {

        if ( m_ui->checkBox_nakli_yekun->isChecked() EQ false ) {
            m_ui->frame_nakli_yekun->setDisabled ( true );
        }
        else {
            m_ui->frame_nakli_yekun->setEnabled ( true );
        }
    }
    else if ( object EQ m_ui->checkBox_depo ) {

        if ( m_ui->checkBox_depo->isChecked() EQ false ) {
            m_ui->searchEdit_depo_kodu->SET_TEXT("");
            m_ui->lineedit_depo_ismi->clear();
            m_depo_id = SUBE_GET_UNITENIN_DEPOSU();

            QString depo_kodu;
            QString depo_adi;

            SUBE_GET_DEPO_KODU_ADI(m_depo_id , &depo_kodu , &depo_adi);

            m_ui->searchEdit_depo_kodu->SET_TEXT(depo_kodu);
            m_ui->lineedit_depo_ismi->setText(depo_adi);
            m_ui->frame_depo->setDisabled(true);
        }
        else {
            m_ui->frame_depo->setEnabled(true);
            SET_FOCUS( m_ui->searchEdit_depo_kodu );
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   STOK_EKSTRESI_BATCH::CHECK_RUN
***************************************************************************************/

int STOK_EKSTRESI_BATCH::CHECK_RUN ()
{
    if ( m_ui->searchEdit_bas_urun_kodu->GET_TEXT().isEmpty() EQ false AND
         m_ui->searchEdit_bts_urun_kodu->GET_TEXT().isEmpty() EQ false ) {

        if ( m_ui->searchEdit_bas_urun_kodu->GET_TEXT() > m_ui->searchEdit_bts_urun_kodu->GET_TEXT() ) {
            MSG_WARNING( tr("Başlangıç ürün kodu , bitiş ürün kodundan büyük olamaz."), m_ui->searchEdit_bas_urun_kodu );
            return ADAK_FAIL;
        }
    }
    if ( m_ui->adakDate_bsl_tarihi->QDATE() > m_ui->adakDate_bts_tarihi->QDATE() ) {
        MSG_WARNING( tr("Başlangıç tarihi,bitiş tarihinden büyük olamaz."), m_ui->adakDate_bsl_tarihi );
        return ADAK_FAIL;
    }
    if ( m_ui->adakDate_bts_tarihi->QDATE() < m_ui->adakDate_bsl_tarihi->QDATE() ) {
        MSG_WARNING( tr("Bitiş tarihi,başlangıç tarihinden büyük olamaz."), m_ui->adakDate_bts_tarihi );
        return ADAK_FAIL;
    }
    if ( m_ui->checkBox_urun_kodu->isChecked() EQ true ) {
        if ( m_ui->search_edit_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Ürün kodunu/barkod numarasını seçmelisiniz."), m_ui->search_edit_urun_kodu );
            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_urun_araligi->isChecked() EQ true ) {
        if ( m_ui->searchEdit_bas_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Başlangıç ürün kodunu/barkod numarasını seçmelisiniz."), m_ui->searchEdit_bas_urun_kodu );
            return ADAK_FAIL;
        }
        if ( m_ui->searchEdit_bts_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Bitiş ürün kodunu/barkod numarasını seçmelisiniz."), m_ui->searchEdit_bts_urun_kodu );
            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_urun_araligi->isChecked() EQ false AND m_ui->checkBox_urun_kodu->isChecked() EQ false ){
        MSG_WARNING(  tr ("Ürün kriteri seçmelisiniz."), m_ui->checkBox_urun_araligi );
        return ADAK_FAIL;
    }

    if ( m_ui->checkBox_depo->isChecked() EQ true ) {
        if ( m_ui->searchEdit_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Depo Kodunu Boş Bırakamazsınız."), m_ui->searchEdit_depo_kodu );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_EKSTRESI_BATCH::RUN_BATCH
***************************************************************************************/

void STOK_EKSTRESI_BATCH::RUN_BATCH ()
{
    STOK_RAPOR_VARS * STOK_RV = new STOK_RAPOR_VARS;

    STOK_RV->baslangic_tarihi               = m_ui->adakDate_bsl_tarihi->QDATE();
    STOK_RV->bitis_tarihi                   = m_ui->adakDate_bts_tarihi->QDATE();
    STOK_RV->bas_urun_kodu                  = m_ui->searchEdit_bas_urun_kodu->GET_TEXT();
    STOK_RV->bts_urun_kodu                  = m_ui->searchEdit_bts_urun_kodu->GET_TEXT();
    STOK_RV->tek_urun_kodu                  = m_ui->search_edit_urun_kodu->GET_TEXT();
    STOK_RV->nakli_yekun                    = m_ui->checkBox_nakli_yekun->isChecked();
    STOK_RV->depo_id                        = m_depo_id;
    STOK_RV->ent_filtresi                   = m_entegrasyon_durumu;
    STOK_RV->stok_alis_satis_durumu         = STK_GET_ALIS_SATIS_ENUM( m_ui->comboBox_stok_alis_satis_durumu->currentText() );

    OPEN_REPORT_SHOWER ( GET_STOK_EKSTRESI_RAPORU(STOK_RV), nativeParentWidget() );

    delete STOK_RV;
}

/**************************************************************************************
                   STOK_EKSTRESI_BATCH::CHECK_EXIT
***************************************************************************************/

int STOK_EKSTRESI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}


/**************************************************************************************
                   STOK_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void STOK_EKSTRESI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget, QLineEdit * line_edit)
{
    if ( widget EQ m_ui->searchEdit_bas_urun_kodu OR widget EQ m_ui->searchEdit_bts_urun_kodu OR
         widget EQ m_ui->search_edit_urun_kodu ) {

        int urun_id = -1;

        urun_id = OPEN_STOK_URUN_ARAMA(ENUM_URUN_KARTI ,-1, line_edit->text(),-1, this );

        if ( urun_id NE -1) {
            SQL_QUERY query(DB);

            query.PREPARE_SELECT("stk_urunler",
                                 "urun_kodu",
                                 "urun_id = :urun_id");
            query.SET_VALUE(":urun_id" , urun_id);

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                line_edit->setText(query.VALUE(0).toString());
            }
        }
    }

    if ( widget EQ m_ui->searchEdit_depo_kodu ) {
        int depo_id = SUBE_DEPO_SEC ( SUBE_GET_SUBE_ID(),this );

        if ( depo_id NE -1 ) {
            line_edit->setText(SUBE_GET_DEPO_KODU(depo_id));
            m_depo_id = depo_id;
        }
    }
}

