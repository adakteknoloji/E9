#include "dmr_yeniden_degerleme_batch_class.h"
#include "ui_dmr_yeniden_degerleme_batch.h"
#include "dmr_enum.h"
#include "sube_console_utils.h"
#include "e9_log.h"
#include "adak_gruplar.h"
#include "dmr_fis_utils.h"
#include "sube_arama_open.h"
 

extern ADAK_SQL * DB;


void OPEN_DMR_YENIDEN_DEGERLEME_BATCH (QWidget * parent)
{
    DMR_YENIDEN_DEGERLEME_BATCH * F = new DMR_YENIDEN_DEGERLEME_BATCH(parent);
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_BATCH::DMR_YENIDEN_DEGERLEME_BATCH
***************************************************************************************/

DMR_YENIDEN_DEGERLEME_BATCH::DMR_YENIDEN_DEGERLEME_BATCH ( QWidget *parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::DMR_YENIDEN_DEGERLEME_BATCH )
{
    m_ui->setupUi        ( this );
    START_BATCH_KERNEL   ( this, DB );
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_BATCH::SETUP_FORM
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE ("sabir-kiymetler-demirbaslar_toplu-yeniden-degerleme" );
    SET_PAGE_TITLE( tr ( "DMR - TOPLU YENİDEN DEĞERLENDİRME" ) );

    REGISTER_BUTTON_WIDGET  ( m_ui->widget_batch_buttons    );

    SET_SETTING_NAME( "DMR_YENIDEN_DEGERLEME_BATCH" );
    SET_FIRST_FOCUS_WIDGET ( m_ui->pushButton_grup_sec );

    m_ui->textEdit_aciklama->setHidden ( true );
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Başla" );

    m_sube_id = 0;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_BATCH::CHECK_VAR
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->commaEdit_degerleme_orani) {
        if ( m_ui->commaEdit_degerleme_orani->GET_DOUBLE() > 100 ) {
             MSG_WARNING( tr ( "Değerleme Oranı 100'den büyük olamaz!.." ), m_ui->commaEdit_degerleme_orani );

             return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->pushButton_grup_sec ) {
        OPEN_GRUP_SECIM( E9_PROGRAMI,DEMIRBAS_GRUP_MODULU, &m_grup_idleri, DB , this , true );
        QStringList grup_adlari;
        QStringList grup_kodlari;
        m_ui->textEdit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , &grup_adlari, &grup_kodlari));
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

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_BATCH::CHECK_RUN
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_BATCH::CHECK_RUN()
{
    if ( m_ui->textEdit_gruplar->toPlainText().isEmpty() EQ true ) {
         MSG_WARNING( tr ( "Grup Kodunu Seçmelisiniz!.." ), m_ui->pushButton_grup_sec );

         return ADAK_FAIL;
    }
    if ( m_ui->commaEdit_degerleme_orani->GET_DOUBLE() EQ 0 ) {
        MSG_WARNING( tr ( "Yeniden Değerleme Oranını Girmelisniz!.." ), m_ui->commaEdit_degerleme_orani );

        return ADAK_FAIL;
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
                   DMR_YENIDEN_DEGERLEME_BATCH::RUN_BATCH
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_BATCH::RUN_BATCH()
{
    SQL_QUERY select_query ( DB );

    QList<int> gruplara_kayitli_demirbaslar_id_list;

    gruplara_kayitli_demirbaslar_id_list = GRP_GRUBA_EKLI_KAYITLARI_BUL(E9_PROGRAMI , DEMIRBAS_GRUP_MODULU,m_grup_idleri);


    //Gruba ve grubun alt gruplarina kayitli olan ve yeniden degerlemeye tabi olan ve henuz yeniden degerlendirilmemis olan  demirbaslar bulunuyor.

    for ( int i = 0 ; i < gruplara_kayitli_demirbaslar_id_list.size() ; i++ ) {

        QString where_str = "demirbas_id = :demirbas_id AND "
                            "yeniden_degerleme_yapilacak_mi = :yeniden_degerleme_yapilacak_mi AND "
                            "yeniden_degerleme_yapildi = :yeniden_degerleme_yapildi";

        if ( m_sube_id > 0 ) {
            where_str.append(" AND sube_id = :sube_id");
        }

        select_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_id,alis_tarihi",where_str );
        select_query.SET_VALUE      ( ":demirbas_id"                   ,     QVariant ( gruplara_kayitli_demirbaslar_id_list.at(i) ).toInt() );
        select_query.SET_VALUE      ( ":yeniden_degerleme_yapilacak_mi",     1 );
        select_query.SET_VALUE      ( ":yeniden_degerleme_yapildi"     ,     0 );

        if ( m_sube_id > 0 ) {
            select_query.SET_VALUE(":sube_id" , m_sube_id);
        }

        if ( select_query.SELECT() NE 0 ) {
            while ( select_query.NEXT() ) {
                QDate demirbas_alis_tarihi = QDate::fromString ( select_query.VALUE(1).toString(),"yyyy.MM.dd" );

                int demirbas_alim_yili = demirbas_alis_tarihi.year();
                int current_year       = QDate::currentDate().year();
                //Demirbaslar alindiklari yil yeniden degerlemeye tabi degillerdir.
                if ( demirbas_alim_yili EQ current_year ) {
                    gruplara_kayitli_demirbaslar_id_list.removeAt(i);
                    continue;
                }

            }
        }
    }



    if ( gruplara_kayitli_demirbaslar_id_list.size() EQ 0 ) {
        m_ui->textEdit_aciklama->setHidden ( false );
        m_ui->textEdit_aciklama->setPlainText ( "Gruba Kayıtlı,Yeniden Değerleme Yapılacak Demirbaş Bulunmamaktadır.Toplu Yeniden Değerleme Yapılamadı.");

        return;
    }

    DMR_FIS_STRUCT * DMR_FISI = new DMR_FIS_STRUCT;

    DMR_FIS_SATIRI_STRUCT * DMR_FIS_SATIRI = new DMR_FIS_SATIRI_STRUCT;

    DMR_CLEAR_FIS_STRUCT(DMR_FISI);

    DMR_FISI->program_id      = E9_PROGRAMI;
    DMR_FISI->modul_id        = DEMIRBAS_MODULU;
    DMR_FISI->fis_tarihi      = QDate::currentDate().toString("yyyy.MM.dd");
    DMR_FISI->aciklama        = QObject::tr("Toplu Yeniden Değerleme Fişi");
    DMR_FISI->fis_turu        = YENIDEN_DEGERLEME_FISI;
    DMR_FISI->degerleme_orani = m_ui->commaEdit_degerleme_orani->GET_DOUBLE();

    DB->START_TRANSACTION();

    int fis_id = DMR_FIS_EKLE(DMR_FISI);


    for ( int i = 0 ; i < gruplara_kayitli_demirbaslar_id_list.size() ; i++ ) {

        int demirbas_id = QVariant ( gruplara_kayitli_demirbaslar_id_list.at(i) ).toInt();

        DMR_CLEAR_FIS_SATIRI_STRUCT(DMR_FIS_SATIRI);

        DMR_FIS_SATIRI->program_id = E9_PROGRAMI;
        DMR_FIS_SATIRI->modul_id   = DEMIRBAS_MODULU;
        DMR_FIS_SATIRI->fis_id     = fis_id;
        DMR_FIS_SATIRI->demirbas_id= demirbas_id;

        DMR_FIS_SATIRI_EKLE(DMR_FISI , DMR_FIS_SATIRI);
    }


    QString log_detaylari = QObject::tr ( "Toplu Yeniden Değerleme  , Grup Kodular  : " ) +
                            m_ui->textEdit_gruplar->toPlainText() + "  " + "\nYeniden Değerleme Oranı : % " +
                            m_ui->commaEdit_degerleme_orani->GET_TEXT();
    E9_LOG_KAYDI_EKLE ( DEMIRBAS_MODULU , LOG_DMR_YENIDEN_DEGERLEME,-1,log_detaylari );

    DB->COMMIT_TRANSACTION();

    m_ui->textEdit_aciklama->setHidden ( false );

    m_ui->textEdit_aciklama->setPlainText ( QString ( "'%1' nolu Yeniden Değerleme Fişi Oluşturuldu." ).arg ( QVariant(DMR_FISI->fis_no).toString()));

    delete DMR_FISI;
    delete DMR_FIS_SATIRI;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_BATCH::CHECK_EXIT
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_BATCH::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    Q_UNUSED(widget);

    m_sube_id = OPEN_SUBE_ARAMA(m_ui->searchEdit_sube_kodu->GET_TEXT(),this);

    if ( m_sube_id NE -1 ) {

        QString sube_kodu;

        SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu );
        line_edit->setText( sube_kodu );
    }
}


















