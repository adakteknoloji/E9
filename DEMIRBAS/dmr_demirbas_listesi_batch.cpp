#include "dmr_demirbas_listesi_batch_class.h"
#include "ui_dmr_demirbas_listesi_batch.h"
#include "print.h"
#include "dmr_struct.h"
#include "dmr_con_utils.h"
#include "e9_log.h"
#include "adak_gruplar.h"
#include "sube_console_utils.h"
#include "dmr_demirbas_karti_arama_open.h"
#include "sube_arama_open.h"
#include "dmr_demirbas_listesi_open.h"

extern ADAK_SQL * DB;


void OPEN_DMR_DEMIRBAS_LISTESI_BATCH ( QWidget * parent )
{
    DMR_DEMIRBAS_LISTESI_BATCH * F = new DMR_DEMIRBAS_LISTESI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI_BATCH::DMR_DEMIRBAS_LISTESI_BATCH
***************************************************************************************/

DMR_DEMIRBAS_LISTESI_BATCH::DMR_DEMIRBAS_LISTESI_BATCH ( QWidget *parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::DMR_DEMIRBAS_LISTESI_BATCH)
{
    m_ui->setupUi      (this );

    START_BATCH_KERNEL (this,DB );
}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI_BATCH::SETUP_FORM
***************************************************************************************/

void DMR_DEMIRBAS_LISTESI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE ("sabit-kiymetler-demirbaslar_sabit-kiymetler-listesi" );
    SET_PAGE_TITLE( tr ( "DMR - SABİT KIYMETLER LİSTESİ") );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );
    SET_FIRST_FOCUS_WIDGET ( m_ui->searchEdit_bas_dmr_kodu );
    SET_SETTING_NAME        ( "DMR_DEMIRBAS_LISTESI_BATCH" );

    m_sube_id = 0;

    m_ui->frame_gruplar->setDisabled(true);
    m_ui->frame_sube->setDisabled(true);
    m_ui->checkBox_sabit_kiymet_araligi->setChecked(true);
}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI_BATCH::CHECK_VAR
***************************************************************************************/

int DMR_DEMIRBAS_LISTESI_BATCH::CHECK_VAR ( QObject *  object)
{
    if ( object EQ m_ui->pushButton_grup_sec ) {
        OPEN_GRUP_SECIM( E9_PROGRAMI,DEMIRBAS_GRUP_MODULU, &m_grup_idleri , DB , this , true);

        QStringList grup_adlari;
        QStringList grup_kodlari;

        m_ui->textEdit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri, &grup_adlari, &grup_kodlari));
    }
    else if ( object EQ m_ui->checkBox_sube ) {

        if ( m_ui->checkBox_sube->isChecked() EQ true ) {
            m_ui->frame_sube->setEnabled(true);
            SET_FOCUS(m_ui->searchEdit_sube_kodu);
        }
        else {
            m_ui->searchEdit_sube_kodu->CLEAR();
            m_ui->lineEdit_sube_ismi->clear();
            m_ui->frame_sube->setDisabled(true);
            m_sube_id = 0;
        }
    }
    else if ( object EQ m_ui->searchEdit_sube_kodu ) {
        if ( m_ui->searchEdit_sube_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_sube_ismi->clear();
            m_sube_id = 0;
            return ADAK_OK;
        }     
        QString sube_adi;
        int     sube_id;

        int result = SUBE_GET_SUBE_ID_ADI( m_ui->searchEdit_sube_kodu->GET_TEXT() , &sube_id, &sube_adi );

        if ( result EQ 0 ) {
            MSG_WARNING(tr("Aradığınız sube kodu bulunamadı.") , m_ui->searchEdit_sube_kodu);
            SET_FOCUS( m_ui->searchEdit_sube_kodu );
            return ADAK_FAIL_UNDO;
        }
        m_ui->lineEdit_sube_ismi->setText( sube_adi );
        m_sube_id = sube_id;
    }
    else if ( object EQ m_ui->checkBox_gruplar ) {
        if ( m_ui->checkBox_gruplar->isChecked() EQ true ) {
            m_ui->frame_gruplar->setEnabled(true);
            SET_FOCUS(m_ui->pushButton_grup_sec);
        }
        else {
            m_ui->frame_gruplar->setDisabled(true);
            m_ui->textEdit_gruplar->clear();
            m_grup_idleri.clear();
        }
    }
    else if ( object EQ m_ui->checkBox_sabit_kiymet_araligi ) {
        if ( m_ui->checkBox_sabit_kiymet_araligi->isChecked() EQ true ) {
            m_ui->frame_sabit_kiymetler->setEnabled(true);
            SET_FOCUS(m_ui->searchEdit_bas_dmr_kodu);
        }
        else {
            m_ui->frame_sabit_kiymetler->setEnabled(false);
            m_ui->searchEdit_bas_dmr_kodu->CLEAR();
            m_ui->searchEdit_bts_dmr_kodu->CLEAR();
            m_ui->lineEdit_bas_dmr_ismi->clear();
            m_ui->lineEdit_bts_dmr_ismi->clear();
            m_ui->checkBox_sabit_kiymet_araligi->setChecked(false);
        }
    }
    else if ( object EQ m_ui->searchEdit_bas_dmr_kodu OR
              object EQ m_ui->searchEdit_bts_dmr_kodu ) {

        QSearchEdit * search_edit = (QSearchEdit * ) object;

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("dmr_demirbaslar" , "demirbas_adi" , "demirbas_kodu = :demirbas_kodu");
        query.SET_VALUE(":demirbas_kodu" , search_edit->GET_TEXT());

        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING("Aradığınız demirbaş kodu bulunamadı." , search_edit);
            return ADAK_FAIL;
        }
        query.NEXT();

        if ( object EQ m_ui->searchEdit_bas_dmr_kodu ) {
            m_ui->lineEdit_bas_dmr_ismi->setText(query.VALUE(0).toString());
        }
        else {
            m_ui->lineEdit_bts_dmr_ismi->setText(query.VALUE(0).toString());
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI_BATCH::CHECK_RUN
***************************************************************************************/

int DMR_DEMIRBAS_LISTESI_BATCH::CHECK_RUN()
{
    if ( m_ui->checkBox_sabit_kiymet_araligi->isChecked() EQ true ) {

        if ( m_ui->searchEdit_bas_dmr_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING(QObject::tr("Başlangıç Sabit Kıymet Kodunu Seçmelisiniz.") ,
                        m_ui->searchEdit_bas_dmr_kodu);
            return ADAK_FAIL;
        }
        if ( m_ui->searchEdit_bts_dmr_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING(QObject::tr("Bitiş Sabit Kıymet Kodunu Seçmelisiniz.") ,
                        m_ui->searchEdit_bts_dmr_kodu);
            return ADAK_FAIL;
        }
    }

    if ( m_ui->checkBox_gruplar->isChecked() EQ true ) {
        if ( m_ui->textEdit_gruplar->toPlainText().isEmpty() EQ true ) {
           MSG_WARNING( tr ( "Grup Kodunu Seçmelisiniz!.." ), m_ui->pushButton_grup_sec );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_sube->isChecked() EQ true ) {
        if ( m_ui->searchEdit_sube_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING(tr("Şube kodunu seçmelisiniz.") , m_ui->searchEdit_sube_kodu);
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI_BATCH::RUN_BATCH
***************************************************************************************/

void DMR_DEMIRBAS_LISTESI_BATCH::RUN_BATCH()
{
    QString log_detaylari = QObject::tr ( "Demirbaş Listesi , Grup Kodu / Adı : " )
                               + m_ui->textEdit_gruplar->toPlainText();
    DB->START_TRANSACTION();
    E9_LOG_KAYDI_EKLE  ( DEMIRBAS_MODULU,LOG_DMR_RAPORLAR,LOG_ISLEM_RAPOR_ALINDI,log_detaylari );
    DB->COMMIT_TRANSACTION();

    DMR_RAPOR_VARS * DMR_RV = DMR_GET_RAPOR_VARS();

    DMR_RV->grup_id_list = m_grup_idleri;
    DMR_RV->sube_id      = m_sube_id;
    DMR_RV->bas_dmr_kodu = m_ui->searchEdit_bas_dmr_kodu->GET_TEXT();
    DMR_RV->bts_dmr_kodu = m_ui->searchEdit_bts_dmr_kodu->GET_TEXT();


    OPEN_REPORT_SHOWER(OPEN_DMR_DEMIRBAS_LISTESI ( DMR_RV ),nativeParentWidget() );
}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI_BATCH::CHECK_EXIT
***************************************************************************************/

int DMR_DEMIRBAS_LISTESI_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void DMR_DEMIRBAS_LISTESI_BATCH::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    if ( widget EQ m_ui->searchEdit_sube_kodu ) {

        m_sube_id = OPEN_SUBE_ARAMA(m_ui->searchEdit_sube_kodu->GET_TEXT(),this);

        if ( m_sube_id NE -1 ) {           
            QString sube_kodu;

            SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu );
            line_edit->setText( sube_kodu );
        }
    }

    else {

        QSearchEdit * search_edit = (QSearchEdit *) widget;

        int demirbas_id = OPEN_DMR_DEMIRBAS_KARTI_ARAMA(search_edit->GET_TEXT(),this);

        if ( demirbas_id > 0 ) {
            SQL_QUERY query(DB);

            query.PREPARE_SELECT("dmr_demirbaslar" , "demirbas_kodu" , "demirbas_id = :demirbas_id");
            query.SET_VALUE(":demirbas_id" , demirbas_id);

            if ( query.SELECT() NE 0 ) {
                query.NEXT();

                line_edit->setText(query.VALUE(0).toString());
            }

        }
    }
}
























