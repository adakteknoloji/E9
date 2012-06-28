#include <QDate>
#include "dmr_amortisman_ayirma_batch_class.h"
#include "ui_dmr_amortisman_ayirma_batch.h"
#include "dmr_con_utils.h"
#include "sube_console_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "adak_utils.h"
#include "e9_log.h"
#include "adak_gruplar.h"
#include "dmr_fis_utils.h"
#include "sube_arama_open.h"
#include "dmr_enum.h"

extern ADAK_SQL * DB;


void OPEN_DMR_AMORTISMAN_AYIRMA_BATCH ( QWidget * parent )
{
    DMR_AMORTISMAN_AYIRMA_BATCH * F = new DMR_AMORTISMAN_AYIRMA_BATCH(parent);
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   DMR_AMORTISMAN_AYIRMA_BATCH::DMR_AMORTISMAN_AYIRMA_BATCH
***************************************************************************************/

DMR_AMORTISMAN_AYIRMA_BATCH::DMR_AMORTISMAN_AYIRMA_BATCH ( QWidget *parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::DMR_AMORTISMAN_AYIRMA_BATCH )
{
    m_ui->setupUi       ( this );
    START_BATCH_KERNEL  ( this, DB );
}

/**************************************************************************************
                   DMR_AMORTISMAN_AYIRMA_BATCH::SETUP_FORM
***************************************************************************************/

void DMR_AMORTISMAN_AYIRMA_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE                       ( "sabit-kiymetler-demirbaslar_toplu-amortisman-ayirma"  );
    SET_PAGE_TITLE                      ( tr ( "DMR - TOPLU AMORTİSMAN AYIRMA" ) );

    REGISTER_BUTTON_WIDGET              ( m_ui->widget_batch_buttons );

    SET_FIRST_FOCUS_WIDGET              ( m_ui->adakDate_fis_tarihi );
    SET_NAME_OF_RUN_BATCH_BUTTON        ( tr ( "Başla" ) );

    SET_SETTING_NAME                    ( "DMR_AMORT_AYIR_BATCH" );

    m_ui->textEdit_aciklama->setHidden  ( true );

    m_ui->frame_sube->setDisabled(true);
    m_ui->checkBox_sube->setChecked(false);

    m_sube_id = 0;
}

/**************************************************************************************
                   DMR_AMORTISMAN_AYIRMA_BATCH::CHECK_VAR
***************************************************************************************/

int DMR_AMORTISMAN_AYIRMA_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->commaEdit_dusulecek_amortisman_yuzdesi ) {
        if ( m_ui->commaEdit_dusulecek_amortisman_yuzdesi->GET_DOUBLE() > 100 ) {
            MSG_WARNING( tr ( "Yıllık Amortismanın en fazla % 100'ü düşülebilir." ), m_ui->commaEdit_dusulecek_amortisman_yuzdesi );

            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->pushButton_grup_sec ) {
        OPEN_GRUP_SECIM( E9_PROGRAMI,DEMIRBAS_GRUP_MODULU, &m_grup_idleri, DB , this , true );
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

        int result = SUBE_GET_SUBE_ID_ADI( m_ui->searchEdit_sube_kodu->GET_TEXT(), &sube_id, &sube_adi );

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
                   DMR_AMORTISMAN_AYIRMA_BATCH::CHECK_RUN
***************************************************************************************/

int DMR_AMORTISMAN_AYIRMA_BATCH::CHECK_RUN()
{
    if ( m_ui->textEdit_gruplar->toPlainText().isEmpty() EQ true ) {
         MSG_WARNING( tr ( "Grup Kodunu Seçmelisiniz!.." ), m_ui->pushButton_grup_sec );

        return ADAK_FAIL;
    }

    if ( m_ui->commaEdit_dusulecek_amortisman_yuzdesi->GET_DOUBLE() EQ 0.0 ) {
        MSG_WARNING( tr ( "Yıllık Düşülecek Amortisman Yüzdesini Giriniz!.." ), m_ui->commaEdit_dusulecek_amortisman_yuzdesi );

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
                   DMR_AMORTISMAN_AYIRMA_BATCH::RUN_BATCH
***************************************************************************************/

void DMR_AMORTISMAN_AYIRMA_BATCH::RUN_BATCH()
{
    SQL_QUERY query ( DB ) ;

    QList<int> gruplara_kayitli_demirbaslar_id_list;

    gruplara_kayitli_demirbaslar_id_list = GRP_GRUBA_EKLI_KAYITLARI_BUL(E9_PROGRAMI,DEMIRBAS_GRUP_MODULU,m_grup_idleri);

    QString where_str;

    for ( int i = 0 ; i < gruplara_kayitli_demirbaslar_id_list.size() ; i++ ) {

        where_str = "demirbas_id = :demirbas_id ";

        if (m_sube_id > 0) {
            where_str.append(" AND sube_id = :sube_id");
        }
        where_str.append(" AND amortisman_ayrilacak_mi = :amortisman_ayrilacak_mi");

        query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_id,yeniden_degerleme_yapilacak_mi,"
                               "yeniden_degerleme_yapildi, alis_tarihi , yillik_ayrilan_amor_yuzdesi",
                                where_str);
        query.SET_VALUE      ( ":demirbas_id" , QVariant ( gruplara_kayitli_demirbaslar_id_list.at(i)).toInt() );

        if ( m_sube_id > 0) {
            query.SET_VALUE(":sube_id" , m_sube_id);
        }
        query.SET_VALUE(":amortisman_ayrilacak_mi" , 1);

        if ( query.SELECT() EQ 0 ) {
            continue;
        }

        while ( query.NEXT() ) {
            int yeniden_degerleme_yapilacak_mi = query.VALUE(1).toInt();
            int yeniden_degerleme_yapildi      = query.VALUE(2).toInt();
            QDate demirbas_alis_tarihi         = QDate::fromString(query.VALUE(3).toString(),"yyyy.MM.dd");

            if ( demirbas_alis_tarihi.year() EQ m_ui->adakDate_fis_tarihi->QDATE().year() ) {
                if ( demirbas_alis_tarihi.month() > m_ui->adakDate_fis_tarihi->QDATE().month()) {
                    gruplara_kayitli_demirbaslar_id_list.removeAt(i);
                    continue;
                }

                double ayrilacak_amor_orani = DMR_GET_AYRILACAK_AMORTISMAN_ORANI(demirbas_alis_tarihi.month());

                if ( ayrilacak_amor_orani NE m_ui->commaEdit_dusulecek_amortisman_yuzdesi->GET_DOUBLE()) {
                    gruplara_kayitli_demirbaslar_id_list.removeAt(i);
                    continue;
                }
            }

            if ( yeniden_degerleme_yapilacak_mi EQ 1 ) {
                if ( yeniden_degerleme_yapildi EQ 0 ) {
                    //Demirbasin alis tarihine bakilir.Islem yili ile demirbas ayni yil alinmissa,
                    //demirbas yeniden degerlemeye tabi degildir


                    int demirbas_alim_yili = demirbas_alis_tarihi.year();
                    int current_year       = m_ui->adakDate_fis_tarihi->QDATE().year();

                    if ( demirbas_alim_yili NE current_year ) {
                        gruplara_kayitli_demirbaslar_id_list.removeAt(i);
                        continue;
                    }
                }
            }

            double yillik_ayrilan_amortisman_yuzdesi = query.VALUE("yillik_ayrilan_amor_yuzdesi").toDouble();

            if ( yillik_ayrilan_amortisman_yuzdesi EQ 100 ) {
                //Demirbasin yillik amortismani ayrilmis demektir
                gruplara_kayitli_demirbaslar_id_list.removeAt(i);
                continue;
            }

            yillik_ayrilan_amortisman_yuzdesi += m_ui->commaEdit_dusulecek_amortisman_yuzdesi->GET_DOUBLE();

            if ( yillik_ayrilan_amortisman_yuzdesi > 100 ) {
                gruplara_kayitli_demirbaslar_id_list.removeAt(i);
                continue;
            }


        }

    }

    if ( gruplara_kayitli_demirbaslar_id_list.size() EQ 0 ) {
        m_ui->textEdit_aciklama->setHidden ( false );
        m_ui->textEdit_aciklama->setPlainText ( "Gruba Kayıtlı,Amortismanı Ayrılacak Demirbaş Bulunmamaktadır.Amortisman Ayırma İşlemi Gerçekleşmedi!..");

        return;
    }

    DB->START_TRANSACTION();

    DMR_FIS_STRUCT * DMR_FISI = new DMR_FIS_STRUCT;

    DMR_CLEAR_FIS_STRUCT(DMR_FISI);

    DMR_FISI->program_id             = E9_PROGRAMI;
    DMR_FISI->modul_id               = DEMIRBAS_MODULU;
    DMR_FISI->fis_tarihi             = m_ui->adakDate_fis_tarihi->DATE();
    DMR_FISI->aciklama               = QObject::tr("Toplu Amortisman Ayırma Fişi" );
    DMR_FISI->fis_turu               = AMORTISMAN_AYIRMA_FISI;
    DMR_FISI->dusulecek_amor_yuzdesi = m_ui->commaEdit_dusulecek_amortisman_yuzdesi->GET_DOUBLE();

    int fis_id = DMR_FIS_EKLE(DMR_FISI);


    DMR_FIS_SATIRI_STRUCT * DMR_FIS_SATIRI = new DMR_FIS_SATIRI_STRUCT;



    for ( int i = 0 ; i < gruplara_kayitli_demirbaslar_id_list.size() ; i++ ) {

        int demirbas_id = QVariant(gruplara_kayitli_demirbaslar_id_list.at(i)).toInt();

        DMR_CLEAR_FIS_SATIRI_STRUCT(DMR_FIS_SATIRI);

        DMR_FIS_SATIRI->demirbas_id        = demirbas_id;
        DMR_FIS_SATIRI->satir_tutari       = 0.0;
        //Ayrilacak amortisman tutari fonksiyonda hesaplanacak
        DMR_FIS_SATIRI->fis_id             = fis_id;
        DMR_FIS_SATIRI->modul_id           = DMR_FISI->modul_id;
        DMR_FIS_SATIRI->program_id         = DMR_FISI->program_id;

        DMR_FIS_SATIRI_EKLE(DMR_FISI , DMR_FIS_SATIRI);

    }


    m_ui->textEdit_aciklama->setHidden ( false );

    query.PREPARE_SELECT ( "dmr_fis_satirlari","fis_satiri_id","fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id" , fis_id );

    if ( query.SELECT() EQ 0 ) {
        //Olusturulan amortisman fisinin hic satiri yoksa,fisin kendiside silinmeli

        query.PREPARE_DELETE ( "dmr_fisler","fis_id = :fis_id" );
        query.SET_VALUE      ( ":fis_id" , fis_id );
        query.DELETE();

        m_ui->textEdit_aciklama->setPlainText ( "Ayrılacak amortisman yüzdesine uygun demirbaş,seçilen grupta bulunamadı.Fiş oluşturulamadı!..");
    }
    else {
        m_ui->textEdit_aciklama->setPlainText ( QString ( "'%1' nolu Amortisman Fişi Oluşturuldu." ).arg ( QVariant(DMR_FISI->fis_no).toString()));
    }



    QString log_detaylari = QObject::tr ( "Toplu Amortisman Ayırma , " );
    log_detaylari.append ( "Gruplar : " + m_ui->textEdit_gruplar->toPlainText() + "  " + "\n");
    log_detaylari.append ( "Düşülecek Amortisman Yüzdesi : % "  +
                         m_ui->commaEdit_dusulecek_amortisman_yuzdesi->GET_TEXT() );

    E9_LOG_KAYDI_EKLE ( DEMIRBAS_MODULU,LOG_DMR_AMORTISMAN_AYIRMA,LOG_ISLEM_ADD,log_detaylari );


    DB->COMMIT_TRANSACTION();


}

/**************************************************************************************
                   DMR_AMORTISMAN_AYIRMA_BATCH::CHECK_EXIT
***************************************************************************************/

int DMR_AMORTISMAN_AYIRMA_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}
/**************************************************************************************
                   DMR_AMORTISMAN_AYIRMA_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void DMR_AMORTISMAN_AYIRMA_BATCH::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    Q_UNUSED(widget);

    m_sube_id = OPEN_SUBE_ARAMA(m_ui->searchEdit_sube_kodu->GET_TEXT(),this);

    if ( m_sube_id NE -1 ) {
        QString sube_kodu;
        SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu );
        line_edit->setText( sube_kodu );
    }
}

