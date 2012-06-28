#include "dmr_demirbas_karti_arama_class.h"
#include "ui_dmr_demirbas_karti_arama.h"
#include "adak_gruplar.h"
#include "sube_console_utils.h"
#include "dmr_con_utils.h"
#include "sube_arama_open.h"
#include "dmr_enum.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   F_DMR_DEMIRBAS_KARTI_ARAMA
***************************************************************************************/

int OPEN_DMR_DEMIRBAS_KARTI_ARAMA ( QString demirbas_kodu, QWidget * parent , int aranacak_demirbaslar)
{
    int secilen_id    = -1;
    DMR_DEMIRBAS_KARTI_ARAMA * F = new DMR_DEMIRBAS_KARTI_ARAMA ( demirbas_kodu, parent ,aranacak_demirbaslar);

    F->m_secilen_id = &secilen_id;

    F->EXEC( NOT_FULL_SCREEN );

    return secilen_id;
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI_ARAMA::DMR_DEMIRBAS_KARTI_ARAMA
**************************************************************************************/

DMR_DEMIRBAS_KARTI_ARAMA::DMR_DEMIRBAS_KARTI_ARAMA ( QString demirbas_kodu, QWidget *parent ,int aranacak_demirbaslar) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::DMR_DEMIRBAS_KARTI_ARAMA)
{
    m_demirbas_kodu        = demirbas_kodu;
    m_aranacak_demirbaslar = aranacak_demirbaslar;
    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this,DB );
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI_ARAMA::SETUP_FORM
***************************************************************************************/

void DMR_DEMIRBAS_KARTI_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE   ( tr("demirbas_karti_arama.html") );
    SET_PAGE_TITLE  ( tr ( "DMR - SABİT KIYMET ARAMA" ) );
    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_demirbas_adi );

    REGISTER_TABLE_WIDGET           ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET   ( m_ui->widget_batch_buttons );

    SET_HEADERS ( QStringList()<<tr ( "demirbas id" )<<tr ( "Demirbaş Kodu" )<<tr ( "Demirbaş Adı" ) );

    SET_SORTING ( false );

    m_sube_id = 0;

    m_ui->demirbas_adi_frame->setDisabled ( true );
    m_ui->demirbas_kodu_frame->setDisabled ( true );
    m_ui->frame_gruplar->setDisabled(true);
    m_ui->frame_sube->setDisabled(true);    

    m_ui->checkbox_demirbas_kodu->setChecked( true );
    m_ui->demirbas_kodu_frame->setEnabled( true );

    m_ui->tableWidget->setColumnWidth   ( 1, 150 );
    m_ui->tableWidget->setColumnWidth   ( 2, 300 );

    SET_SETTING_NAME                    ( "DMR_DEMIRBAS_KARTI_ARAMA" );

    SET_ENTER_KEY_FOR_RUN_BATCH         ( true );
    SET_FIND_BUTTON_NAME                ( tr  ( "Bul" ) );

    m_ui->tableWidget->hideColumn       ( 0 );

    m_ui->lineedit_demirbas_kodu->setText(m_demirbas_kodu);
    if ( m_ui->lineedit_demirbas_kodu->text().isEmpty() EQ false ) {
        AUTO_START ();
    }
    else {
         m_ui->lineedit_demirbas_kodu->setText("*");
    }
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI_ARAMA::CHECK_VAR
***************************************************************************************/

int DMR_DEMIRBAS_KARTI_ARAMA::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->checkbox_demirbas_kodu ) {
        if ( m_ui->checkbox_demirbas_kodu->isChecked() EQ false ) {
            m_ui->demirbas_kodu_frame->setEnabled ( false );
            m_ui->lineedit_demirbas_kodu->clear();
        }
        else {
            m_ui->demirbas_kodu_frame->setEnabled ( true );
            SET_FOCUS( m_ui->lineedit_demirbas_kodu );
        }
    }
    else if ( object EQ m_ui->checkbox_demirbas_adi ) {

        if ( m_ui->checkbox_demirbas_adi->isChecked() EQ false ) {
            m_ui->demirbas_adi_frame->setEnabled ( false );
            m_ui->lineedit_demirbas_adi->clear();
        }
        else {
            m_ui->demirbas_adi_frame->setEnabled ( true );
            SET_FOCUS( m_ui->lineedit_demirbas_adi );
        }
    }
    else if ( object EQ m_ui->checkbox_gruplar ) {
        if ( m_ui->checkbox_gruplar->isChecked() EQ true ) {
            m_ui->frame_gruplar->setEnabled(true);
            SET_FOCUS(m_ui->pushButton_gruplar);
        }
        else {
            m_ui->frame_gruplar->setDisabled(true);
            m_ui->textEdit_gruplar->clear();
        }
    }
    else if ( object EQ m_ui->checkbox_sube ) {

        if ( m_ui->checkbox_sube->isChecked() EQ true ) {
            m_ui->frame_sube->setEnabled(true);
            SET_FOCUS(m_ui->searchEdit_sube_kodu);
        }
        else {
            m_ui->searchEdit_sube_kodu->CLEAR();
            m_ui->lineEdit_sube_adi->clear();
            m_ui->frame_sube->setDisabled(true);
        }
    }
    else if ( object EQ m_ui->pushButton_gruplar ) {
        OPEN_GRUP_SECIM(E9_PROGRAMI , DEMIRBAS_GRUP_MODULU, &m_grup_idler_list , DB,this);
        QString grup_str = GRP_GRUP_ADLARINI_BUL ( &m_grup_idler_list, new QStringList() , new QStringList() );
        m_ui->textEdit_gruplar->setPlainText( grup_str );
    }
    else if ( object EQ m_ui->searchEdit_sube_kodu ) {
        if ( m_ui->searchEdit_sube_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_sube_adi->clear();
            m_sube_id = 0;
            return ADAK_OK;
        }

        QString sube_adi;
        int     sube_id;

        int result = SUBE_GET_SUBE_ID_ADI( m_ui->searchEdit_sube_kodu->GET_TEXT(), &sube_id, &sube_adi );

        if ( result EQ 0 ) {
            MSG_WARNING(tr("Aradığınız sube kodu bulunamadı.") , m_ui->searchEdit_sube_kodu);
            SET_FOCUS( m_ui->searchEdit_sube_kodu );
            return ADAK_FAIL_UNDO;
        }
        m_ui->lineEdit_sube_adi->setText( sube_adi );
        m_sube_id = sube_id;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI_ARAMA::CHECK_RUN
***************************************************************************************/

int DMR_DEMIRBAS_KARTI_ARAMA::CHECK_RUN()
{
    if ( m_ui->checkbox_demirbas_adi->isChecked() EQ true ) {
        if ( m_ui->lineedit_demirbas_adi->text().isEmpty() EQ true ) {
             MSG_WARNING( tr ( "Demirbaş adına arama kriterlerini girmelisiniz!.." ), m_ui->lineedit_demirbas_adi );
             return ADAK_FAIL;
        }
    }
    if ( m_ui->checkbox_demirbas_kodu->isChecked() EQ true ) {
        if ( m_ui->lineedit_demirbas_kodu->text().isEmpty() EQ true ) {
             MSG_WARNING( tr ( "Demirbaş Koduna arama kriterlerini girmelisiniz!.." ), m_ui->lineedit_demirbas_kodu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkbox_sube->isChecked() EQ true ) {
        if ( m_ui->searchEdit_sube_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING(tr("Şube kodunu seçmelisiniz.") , m_ui->searchEdit_sube_kodu);
            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkbox_gruplar->isChecked() EQ true ) {
        if ( m_ui->textEdit_gruplar->toPlainText().isEmpty() EQ true ) {
            MSG_WARNING(QObject::tr("Grup Seçimi Yapmalısınız."),m_ui->pushButton_gruplar);
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI_ARAMA::SEARCH
***************************************************************************************/

void DMR_DEMIRBAS_KARTI_ARAMA::SEARCH()
{
    SQL_QUERY f_query ( DB );


    f_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_id,demirbas_kodu,demirbas_adi","");

    if ( m_ui->checkbox_demirbas_adi->isChecked() EQ true ) {
        f_query.AND_EKLE("demirbas_adi LIKE :demirbas_adi");
        f_query.SET_LIKE(":demirbas_adi"  , m_ui->lineedit_demirbas_adi->text());
    }
    if ( m_ui->checkbox_demirbas_kodu->isChecked() EQ true ) {
        if ( m_ui->lineedit_demirbas_kodu->text() NE "*" ) {
            f_query.AND_EKLE("demirbas_kodu LIKE :demirbas_kodu");
            f_query.SET_LIKE(":demirbas_kodu" , m_ui->lineedit_demirbas_kodu->text());
        }
    }

    if ( m_ui->checkbox_sube->isChecked() EQ true ) {
        f_query.AND_EKLE("sube_id = :sube_id");
        f_query.SET_VALUE(":sube_id" , m_sube_id);
    }
    if ( m_aranacak_demirbaslar NE TUM_DEMIRBASLAR ) {
        f_query.AND_EKLE("demirbas_satildi_mi = :demirbas_satildi_mi");
        f_query.SET_VALUE(":demirbas_satildi_mi" , 0);
    }

    QList < int > gruplardaki_kayit_listesi;
    if ( m_ui->checkbox_gruplar->isChecked() EQ true ) {
        gruplardaki_kayit_listesi = GRP_GRUBA_EKLI_KAYITLARI_BUL( E9_PROGRAMI, DEMIRBAS_GRUP_MODULU,m_grup_idler_list );
    }

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    while ( f_query.NEXT() ) {
        if ( m_ui->checkbox_gruplar->isChecked() EQ true ) {

            if (gruplardaki_kayit_listesi.contains(f_query.VALUE(0).toInt()) EQ false ) {
                continue;
            }
        }

        int current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item ( current_row,0 )->setText ( f_query.VALUE(0).toString() );
        m_ui->tableWidget->item ( current_row,1 )->setText ( f_query.VALUE(1).toString() );
        m_ui->tableWidget->item ( current_row,2 )->setText ( f_query.VALUE(2).toString() );
    }

}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int DMR_DEMIRBAS_KARTI_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number,0)->text().toInt();
    return ADAK_EXIT;
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI_ARAMA::SEARCH_EDIT_CLICKED
***************************************************************************************/

void DMR_DEMIRBAS_KARTI_ARAMA::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    Q_UNUSED(widget);

    m_sube_id = OPEN_SUBE_ARAMA(m_ui->searchEdit_sube_kodu->GET_TEXT(),this);


    if ( m_sube_id NE -1 ) {

        QString sube_kodu;

        SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu );

        line_edit->setText( sube_kodu );
    }
}




















