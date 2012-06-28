#include <QDate>
#include "dmr_demirbas_karti_class.h"
#include "ui_dmr_demirbas_karti.h"
#include "dmr_grup_tree_secimi_open.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
#include "muh_console_utils.h"
#include "dmr_con_utils.h"
#include "adak_utils.h"
 
#include "e9_console_utils.h"
#include "e9_log.h"
#include "adak_gruplar.h"
#include "fat_enum.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "fat_fis_utils.h"
#include "sube_console_utils.h"
#include "prs_con_utils.h"
#include "cari_kart_arama_open.h"
#include "fat_fatura_arama_open.h"
#include "dmr_demirbas_karti_arama_open.h"
#include "muh_hesap_arama_open.h"
#include "sube_arama_open.h"
#include "dmr_con_utils.h"
#include "fat_console_utils.h"
#include "e9_gui_utils.h"
#include "fat_struct.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   F_DMR_DEMIRBAS_KARTI
***************************************************************************************/


void  OPEN_DMR_DEMIRBAS_KARTI ( QWidget * parent )
{
    DMR_DEMIRBAS_KARTI * demirbas_karti = new DMR_DEMIRBAS_KARTI ( parent );
    demirbas_karti->SHOW( FULL_SCREEN );

}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::DMR_DEMIRBAS_KARTI
***************************************************************************************/

DMR_DEMIRBAS_KARTI:: DMR_DEMIRBAS_KARTI ( QWidget *parent ) : FORM_KERNEL ( parent ), m_ui ( new Ui:: DMR_DEMIRBAS_KARTI)
{
    m_ui->setupUi     (this );
    START_FORM_KERNEL (this, DB );
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::SETUP_FORM
***************************************************************************************/

void DMR_DEMIRBAS_KARTI::SETUP_FORM()
{
    SET_HELP_PAGE  ( "sabit-kiymetler-demirbaslar_sabit-kiymet-kaydi" );
    SET_PAGE_TITLE ( tr ( "DMR - SABİT KIYMET KAYDI" ) );

    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_buttons_widget );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_demirbas_kodu );

    REGISTER_CHANGER_BUTTON(m_ui->pushButton_guplar);
    REGISTER_CHANGER_BUTTON(m_ui->pushButton_fatura_sec);

    m_ui->lineEdit_temel_para_birimi->setText(DVZ_GET_TEMEL_PARA_BIRIMI_KODU());     

    m_ui->comboBox_amortisman_yontemi->addItems ( QStringList() << "Normal Bakiye" << "Azalan Bakiye" );    

    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_temel_para_birimi);
    m_ui->checkBox_yeniden_degerleme->setChecked                ( true );

    SET_SETTING_NAME    ( "DMR_DEMIRBAS_KARTI" );

    FOCUS_FIRST_WIDGET  ();

    m_ui->lineedit_amortisman_suresi->setMaxLength(2);
    m_ui->lineedit_demirbas_adi->setMaxLength(128);
    m_ui->lineedit_demirbas_kodu->setMaxLength(20);
    m_ui->lineedit_garanti_suresi->setMaxLength(2);
    m_ui->lineEdit_marka->setMaxLength(128);
    m_ui->lineEdit_model->setMaxLength(128);
    m_ui->lineedit_muh_hesap_ismi->setMaxLength(60);
    m_ui->lineEdit_seri_no->setMaxLength(20);
    m_ui->lineEdit_uretici_firma->setMaxLength(128);

    DISABLE_CLEAR_ON_WIDGET(m_ui->checkBox_amortisman_ayrilacak_mi);

    m_ui->checkBox_amortisman_ayrilacak_mi->setChecked(true);

    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_amortisman_yontemi);

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchedit_muh_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_muh_hesap_ismi  );

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchedit_amor_gdr_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_amor_gdr_hesap_ismi  );

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchedit_bir_amortisman_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_bir_amortisman_hesap_ismi );

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_sube_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_sube_ismi);

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchedit_dmr_satis_kar_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_dmr_satis_kar_hesap_ismi);

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchedit_dmr_satis_zarar_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_dmr_satis_zarar_hesap_ismi);

    SQL_QUERY query(DB);

    m_grup_id             = -1;
    m_muh_hesap_id        = 0;
    m_fatura_id           = 0;
    m_amor_gider_hesap_id = 0;
    m_bir_amor_hesap_id   = 0;
    m_dmr_satis_kar_hesap_id = 0;

    QString demirbas_hesap_kodu, demirbas_hesap_adi;

    m_muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "demirbaslar_ent_hesap_id", &demirbas_hesap_kodu, &demirbas_hesap_adi );

    m_ui->searchedit_muh_hesap_kodu->SET_TEXT   ( demirbas_hesap_kodu );
    m_ui->lineedit_muh_hesap_ismi->setText      ( demirbas_hesap_adi  );

    QString amortisman_hesap_kodu, amortisman_hesap_adi;

    m_bir_amor_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "bir_amortismanlar_hsp_id", &amortisman_hesap_kodu, &amortisman_hesap_adi );

    m_ui->searchedit_bir_amortisman_hesap_kodu->SET_TEXT    ( amortisman_hesap_kodu );
    m_ui->lineEdit_bir_amortisman_hesap_ismi->setText       ( amortisman_hesap_adi  );

    QString amor_gider_hesap_kodu, amor_gider_hesap_adi;

    m_amor_gider_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "amor_giderleri_hsp_id", &amor_gider_hesap_kodu, &amor_gider_hesap_adi );

    m_ui->searchedit_amor_gdr_hesap_kodu->SET_TEXT  ( amortisman_hesap_kodu );
    m_ui->lineEdit_amor_gdr_hesap_ismi->setText     ( amortisman_hesap_adi  );

    QString dmr_satis_kar_hesap_kodu, dmr_satis_kar_hesap_adi;

    m_dmr_satis_kar_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "dmr_satis_kar_ent_hesap_id", &dmr_satis_kar_hesap_kodu, &dmr_satis_kar_hesap_adi );

    m_ui->searchedit_dmr_satis_kar_hesap_kodu->SET_TEXT ( dmr_satis_kar_hesap_kodu );
    m_ui->lineEdit_dmr_satis_kar_hesap_ismi->setText    ( dmr_satis_kar_hesap_adi  );

    QString dmr_satis_zarar_hesap_kodu, dmr_satis_zarar_hesap_adi;

    m_dmr_satis_zarar_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "dmr_satis_zarar_ent_hesap_id", &dmr_satis_zarar_hesap_kodu, &dmr_satis_zarar_hesap_adi );

    m_ui->searchedit_dmr_satis_zarar_hesap_kodu->SET_TEXT   ( dmr_satis_zarar_hesap_kodu );
    m_ui->lineEdit_dmr_satis_zarar_hesap_ismi->setText      ( dmr_satis_zarar_hesap_adi  );


    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {

        m_ui->searchedit_muh_hesap_kodu->setHidden(true);
        m_ui->lineedit_muh_hesap_ismi->setHidden(true);
        m_ui->label_muhasebe_hesabi->setHidden(true);
    }

    m_sube_id = 1;
    QString sube_kodu, sube_adi;

    SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu, &sube_adi );

    m_ui->searchEdit_sube_kodu->SET_TEXT( sube_kodu );
    m_ui->lineEdit_sube_ismi->setText( sube_adi );

}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void DMR_DEMIRBAS_KARTI::CLEAR_FORM_MEMBERS()
{
    m_grup_id   = -1;
    m_fatura_id = 0;
    m_rpr_isl_grubu_idler.clear();
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::NEW_RECORD
***************************************************************************************/

void  DMR_DEMIRBAS_KARTI::NEW_RECORD()
{
    m_cari_hesap_id    = 0;

    m_ui->checkBox_kist_amortisman->setChecked ( false );
    m_ui->checkBox_yeniden_degerleme->setChecked ( true );

    m_ui->checkBox_amortisman_ayrilacak_mi->setChecked(true);


    SQL_QUERY select_query ( DB ) ;

    select_query.PREPARE_SELECT ( "e9_sabit_degerler","dmr_amor_yntemi_degisebilir_mi, dmr_amortisman_ayirma_yontemi,"
                                  "dmr_max_amortisman_orani  ",
                                  "sabit_deger_id = :sabit_deger_id" );
    select_query.SET_VALUE(":sabit_deger_id",1);

    bool amortisman_ayirma_yontemi_demirbaslara_gore_degisir_mi = 1;

    m_max_amortisman_orani = 50;

    if( select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        amortisman_ayirma_yontemi_demirbaslara_gore_degisir_mi = select_query.VALUE(0).toBool();
        m_max_amortisman_orani = select_query.VALUE(2).toDouble();
        if ( amortisman_ayirma_yontemi_demirbaslara_gore_degisir_mi EQ false ) {
             m_ui->comboBox_amortisman_yontemi->setDisabled ( true );
             m_ui->label_amortisman_yontemi->setDisabled ( true );
             m_ui->comboBox_amortisman_yontemi->setCurrentIndex ( m_ui->comboBox_amortisman_yontemi->findText(DMR_GET_AMORTISMAN_SEKLI_STRING(select_query.VALUE(1).toInt())));
        }

    }
    else {
        m_ui->comboBox_amortisman_yontemi->setCurrentIndex(-1);
    }


}
/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::GET_RECORD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::GET_RECORD ( int record_id )
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar ","demirbas_kodu,demirbas_adi,grup_id,muh_hesap_id,alis_tarihi,alis_bedeli,"
                                  "uretici_firma,marka,model,seri_numarasi,garanti_suresi,"
                                  "amortisman_orani,amortisman_suresi,yeniden_degerleme_yapilacak_mi,kist_amortisman,"
                                  "demirbasin_degeri,amortisman_yontemi,personel_id,"
                                  "grup_id,fatura_id ,bir_amor_hesap_id , "
                                  "amor_gider_hesap_id , dmr_satis_kar_hesap_id ,amortisman_ayrilacak_mi,dmr_satis_zarar_hesap_id",
                                  "demirbas_id = :demirbas_id" );

    select_query.SET_VALUE      ( ":demirbas_id" , record_id );

    if ( select_query.SELECT() EQ 0 ) {
         return ADAK_FAIL;
    }

    select_query.NEXT();

    m_ui->lineedit_demirbas_kodu->setText ( select_query.VALUE(0).toString() );
    m_ui->lineedit_demirbas_adi->setText  ( select_query.VALUE(1).toString() );
    m_ui->searchedit_muh_hesap_kodu->SET_TEXT ( MUH_GET_HESAP_KODU(select_query.VALUE(3).toInt() ) );
    m_ui->lineedit_muh_hesap_ismi->setText ( MUH_GET_HESAP_ISMI(select_query.VALUE(3).toInt() ) );
    m_ui->adakDate_alis_tarihi->SET_DATE(select_query.VALUE(4).toString() );
    m_ui->commaEdit_alis_fiyati->SET_DOUBLE ( select_query.VALUE(5).toDouble() );
    m_ui->lineEdit_uretici_firma->setText(select_query.VALUE(6).toString() );
    m_ui->lineEdit_marka->setText(select_query.VALUE(7).toString() );
    m_ui->lineEdit_model->setText(select_query.VALUE(8).toString() );
    m_ui->lineEdit_seri_no->setText(select_query.VALUE(9).toString() );
    m_ui->lineedit_garanti_suresi->setText(select_query.VALUE(10).toString() );
    m_ui->commaEdit_amortisman_orani->SET_DOUBLE(select_query.VALUE(11).toDouble() );
    m_ui->lineedit_amortisman_suresi->setText(select_query.VALUE(12).toString() );
    m_ui->checkBox_yeniden_degerleme->setChecked(select_query.VALUE(13).toBool() );
    m_ui->checkBox_kist_amortisman->setChecked(select_query.VALUE(14).toBool() );
    m_ui->commaEdit_demirbasin_degeri->SET_DOUBLE(select_query.VALUE(15).toDouble() );

    m_ui->checkBox_amortisman_ayrilacak_mi->setChecked(QVariant(select_query.VALUE("amortisman_ayrilacak_mi")).toBool());

    QList<int> m_grup_idleri;

    m_grup_id = select_query.VALUE("grup_id").toInt();

    m_muh_hesap_id = select_query.VALUE(3).toInt();

    m_grup_idleri << select_query.VALUE("grup_id").toInt();

    if (m_grup_idleri.isEmpty() EQ false) {
        m_ui->search_edit_gruplar->SET_TEXT(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , new QStringList() , new QStringList()));
    }

    m_ui->textEdit_rapor_isl_gruplari->setText ( GRP_KAYDIN_GRUPLARINI_BUL ( E9_PROGRAMI , DEMIRBAS_GRUP_MODULU, record_id, &m_rpr_isl_grubu_idler, new QStringList, new QStringList ) );

    double birikmis_amortisman_tutari = DMR_GET_BIRIKMIS_AMORTISMAN_TUTARI ( record_id, MALI_YIL_ARRAY_INDIS ( QDate::currentDate()));

    double net_deger = select_query.VALUE(15).toDouble() - birikmis_amortisman_tutari;

    m_ui->commaEdit_net_deger->SET_DOUBLE ( net_deger );
    m_ui->commaEdit_birikmis_amortisman->SET_DOUBLE ( birikmis_amortisman_tutari );

    m_ui->comboBox_amortisman_yontemi->setCurrentIndex ( m_ui->comboBox_amortisman_yontemi->findText ( DMR_GET_AMORTISMAN_SEKLI_STRING(select_query.VALUE("amortisman_yontemi").toInt())));

    m_fatura_id = select_query.VALUE("fatura_id").toInt();

    m_ui->lineedit_fatura_bilgileri->setText(DMR_GET_FATURA_BILGILERI(m_fatura_id));

    m_bir_amor_hesap_id      = select_query.VALUE("bir_amor_hesap_id").toInt();
    m_amor_gider_hesap_id    = select_query.VALUE("amor_gider_hesap_id").toInt();
    m_dmr_satis_kar_hesap_id = select_query.VALUE("dmr_satis_kar_hesap_id").toInt();
    m_dmr_satis_zarar_hesap_id = select_query.VALUE("dmr_satis_zarar_hesap_id").toInt();

    int personel_id = select_query.VALUE(17).toInt();

    int cari_hesap_id = PRS_GET_PERSONEL_CARI_HESAP_ID( personel_id );

    QString cari_hesap_kodu, cari_hesap_ismi;

    int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

    if ( result NE 0 ) {
        m_ui->searchEdit_personel_adi->SET_TEXT( cari_hesap_kodu );
        m_ui->lineEdit_personel_soyadi->setText( cari_hesap_ismi );

        m_cari_hesap_id = cari_hesap_id;
    }

    QString tam_hesap_kodu,hesap_ismi;

    MUH_GET_HESAP_KODU_HESAP_ISMI(m_amor_gider_hesap_id , tam_hesap_kodu ,hesap_ismi);


    m_ui->searchedit_amor_gdr_hesap_kodu->SET_TEXT(tam_hesap_kodu);
    m_ui->lineEdit_amor_gdr_hesap_ismi->setText(hesap_ismi);


    MUH_GET_HESAP_KODU_HESAP_ISMI(m_bir_amor_hesap_id , tam_hesap_kodu ,hesap_ismi);

    m_ui->searchedit_bir_amortisman_hesap_kodu->SET_TEXT(tam_hesap_kodu);
    m_ui->lineEdit_bir_amortisman_hesap_ismi->setText(hesap_ismi);


    MUH_GET_HESAP_KODU_HESAP_ISMI(m_dmr_satis_kar_hesap_id , tam_hesap_kodu ,hesap_ismi);


    m_ui->searchedit_dmr_satis_kar_hesap_kodu->SET_TEXT(tam_hesap_kodu);
    m_ui->lineEdit_dmr_satis_kar_hesap_ismi->setText(hesap_ismi);


    MUH_GET_HESAP_KODU_HESAP_ISMI(m_dmr_satis_zarar_hesap_id , tam_hesap_kodu ,hesap_ismi);

    m_ui->searchedit_dmr_satis_zarar_hesap_kodu->SET_TEXT(tam_hesap_kodu);
    m_ui->lineEdit_dmr_satis_zarar_hesap_ismi->setText(hesap_ismi);


    return ADAK_OK;
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::CHECK_VAR
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchedit_muh_hesap_kodu ) {
       m_ui->lineedit_muh_hesap_ismi->clear();

       int     hesap_id;
       QString hesap_ismi;

       if ( MUH_GET_HESAP_ID_HESAP_ISMI(m_ui->searchedit_muh_hesap_kodu->GET_TEXT(),
                                        hesap_id,hesap_ismi) EQ 0 ) {
           MSG_WARNING( tr ( "Aradığınız muhasebe hesap kodu bulunamadı!.." ), m_ui->searchedit_muh_hesap_kodu );

           return ADAK_FAIL;
       }

       m_muh_hesap_id = hesap_id;
       m_ui->lineedit_muh_hesap_ismi->setText ( hesap_ismi );
    }
    else if ( object EQ m_ui->searchedit_amor_gdr_hesap_kodu ) {
        m_ui->lineEdit_amor_gdr_hesap_ismi->clear();

        int     hesap_id;
        QString hesap_ismi;

        if ( MUH_GET_HESAP_ID_HESAP_ISMI(m_ui->searchedit_amor_gdr_hesap_kodu->GET_TEXT(),
                                         hesap_id,hesap_ismi) EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız muhasebe hesap kodu bulunamadı!.." ), m_ui->searchedit_amor_gdr_hesap_kodu );

            return ADAK_FAIL;
        }
        m_amor_gider_hesap_id = hesap_id;
        m_ui->lineEdit_amor_gdr_hesap_ismi->setText ( hesap_ismi);
    }
    else if ( object EQ m_ui->searchedit_bir_amortisman_hesap_kodu ) {
        m_ui->lineEdit_bir_amortisman_hesap_ismi->clear();

        int     hesap_id;
        QString hesap_ismi;

        if ( MUH_GET_HESAP_ID_HESAP_ISMI(m_ui->searchedit_bir_amortisman_hesap_kodu->GET_TEXT(),
                                         hesap_id,hesap_ismi) EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız muhasebe hesap kodu bulunamadı!.." ), m_ui->searchedit_bir_amortisman_hesap_kodu );

            return ADAK_FAIL;
        }

        m_bir_amor_hesap_id = hesap_id;
        m_ui->lineEdit_bir_amortisman_hesap_ismi->setText ( hesap_ismi );
    }
    else if ( object EQ m_ui->searchedit_dmr_satis_kar_hesap_kodu ) {
        m_ui->lineEdit_dmr_satis_kar_hesap_ismi->clear();

        int     hesap_id;
        QString hesap_ismi;

        if ( MUH_GET_HESAP_ID_HESAP_ISMI(m_ui->searchedit_dmr_satis_kar_hesap_kodu->GET_TEXT(),
                                         hesap_id,hesap_ismi) EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız muhasebe hesap kodu bulunamadı!.." ), m_ui->searchedit_dmr_satis_kar_hesap_kodu );

            return ADAK_FAIL;
        }

        m_dmr_satis_kar_hesap_id = hesap_id;
        m_ui->lineEdit_dmr_satis_kar_hesap_ismi->setText ( hesap_ismi);
    }
    else if ( object EQ m_ui->searchedit_dmr_satis_zarar_hesap_kodu ) {
        m_ui->lineEdit_dmr_satis_zarar_hesap_ismi->clear();

        int     hesap_id;
        QString hesap_ismi;

        if ( MUH_GET_HESAP_ID_HESAP_ISMI(m_ui->searchedit_dmr_satis_zarar_hesap_kodu->GET_TEXT(),
                                         hesap_id,hesap_ismi) EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız muhasebe hesap kodu bulunamadı!.." ), m_ui->searchedit_dmr_satis_zarar_hesap_kodu );

            return ADAK_FAIL;
        }

        m_dmr_satis_zarar_hesap_id = hesap_id;
        m_ui->lineEdit_dmr_satis_zarar_hesap_ismi->setText (hesap_ismi);
    }
    else if ( object EQ m_ui->searchEdit_sube_kodu ) {
        if ( m_ui->searchEdit_sube_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_sube_ismi->clear();
            m_sube_id = -1;
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

    else if ( object EQ m_ui->lineedit_amortisman_suresi ) {

        QRegExp rx ( "^\\d\\d?$" );

        if ( rx.indexIn ( m_ui->lineedit_amortisman_suresi->text()) EQ -1 ) {
             MSG_WARNING( tr ( "Amortisman süresine sadece 1-99 arası bir sayı girebilirsiniz!.."), m_ui->lineedit_amortisman_suresi);

            return ADAK_FAIL;
        }

        int amortisman_suresi   = m_ui->lineedit_amortisman_suresi->text().toInt();
        if ( amortisman_suresi EQ 0 ) {
            MSG_WARNING( tr ( "Amortisman süresi sıfır olamaz."), m_ui->lineedit_amortisman_suresi );

            return ADAK_FAIL;
        }
        double amortisman_orani = 100 / amortisman_suresi;

        if ( ( amortisman_orani * 2 ) > m_max_amortisman_orani ) {
            MSG_WARNING( tr ( "Demirbaşın amortisman oranı,max amortisman oranının en fazla yarısı kadar olabilir.Girdiğiniz amortisman süresi çok uzun lütfen düzeltiniz!.."), m_ui->lineedit_amortisman_suresi );

            return ADAK_FAIL;
        }
        m_ui->commaEdit_amortisman_orani->SET_DOUBLE ( amortisman_orani );
    }
    else if ( object EQ m_ui->comboBox_amortisman_yontemi ) {
         SQL_QUERY select_query ( DB );

        select_query.PREPARE_SELECT ( "dmr_demirbaslar","amortisman_ayrilabilir_mi","demirbas_id = :demirbas_id" );
        select_query.SET_VALUE      ( ":demirbas_id" , DMR_GET_DEMIRBAS_ID(m_ui->lineedit_demirbas_kodu->text() ) );

        if ( select_query.SELECT() NE 0 ) {
             select_query.NEXT();
             bool amortisman_ayrilabilir_mi = select_query.VALUE(0).toBool();

            if ( amortisman_ayrilabilir_mi EQ false ) {


                int msg_secim = MSG_YES_NO( tr ( "Demirbaş için amortisman ayrılmıştır..\n Amortisman yöntemini değiştirmek istediğinizden emin misiniz? "), NULL );

                if ( msg_secim EQ ADAK_NO ) {
                     return ADAK_FAIL;
                }
            }
        }
    }

    else if ( object EQ m_ui->comboBox_amortisman_yontemi OR object EQ m_ui->lineedit_amortisman_suresi
         OR object EQ m_ui->commaEdit_alis_fiyati OR object EQ m_ui->checkBox_amortisman_ayrilacak_mi
         OR object EQ m_ui->commaEdit_birikmis_amortisman) {
        SQL_QUERY select_query ( DB );

        select_query.PREPARE_SELECT ( "dmr_fis_satirlari","fis_satiri_id","demirbas_id = :demirbas_id" );
        select_query.SET_VALUE      ( ":demirbas_id" , DMR_GET_DEMIRBAS_ID ( m_ui->lineedit_demirbas_kodu->text() ) );

        if ( select_query.SELECT() NE 0 ) {
             MSG_WARNING( tr ( "Demirbaş fişlerde kullanılmıştır.Amortisman yöntemini,süresini,grubunu,alış fiyatı ve birikmiş amortisman tutarını değiştiremezsiniz!.."), NULL );
            return ADAK_FAIL;
        }
    }

    else if ( object EQ m_ui->searchEdit_personel_adi ) {

        if ( m_ui->searchEdit_personel_adi->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_personel_soyadi->clear();
            m_cari_hesap_id = 0;
            return ADAK_OK;
        }

        QString  cari_hesap_ismi;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_personel_adi->GET_TEXT(), &m_cari_hesap_id, &cari_hesap_ismi );

        if ( result EQ 0 ) {
            MSG_WARNING("Aradığınız personel bulunamadı." , m_ui->searchEdit_personel_adi);
            return ADAK_FAIL;
        }
        m_ui->lineEdit_personel_soyadi->setText( cari_hesap_ismi );

    }

    else if ( object EQ m_ui->checkBox_amortisman_ayrilacak_mi ) {
        if ( m_ui->checkBox_amortisman_ayrilacak_mi->isChecked() EQ true ) {
            m_ui->comboBox_amortisman_yontemi->setEnabled(true);
            m_ui->lineedit_amortisman_suresi->setEnabled(true);
            m_ui->commaEdit_amortisman_orani->setEnabled(true);
        }
        else{
            m_ui->comboBox_amortisman_yontemi->setEnabled(false);
            m_ui->lineedit_amortisman_suresi->setEnabled(false);
            m_ui->commaEdit_amortisman_orani->setEnabled(false);
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::CHECK_EMPTY
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::CHECK_EMPTY ()
{
    if ( m_ui->searchedit_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Muhasebe hesap kodunu boş bırakamazsınız!.." ), m_ui->searchedit_muh_hesap_kodu );
        return ADAK_FAIL;
    }
    if ( m_ui->searchedit_amor_gdr_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Amortisman Giderleri hesap kodunu boş bırakamazsınız!.." ), m_ui->searchedit_amor_gdr_hesap_kodu );
        return ADAK_FAIL;
    }

    if ( m_ui->searchedit_bir_amortisman_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Birikmiş Amortismanlar hesap kodunu boş bırakamazsınız!.." ), m_ui->searchedit_bir_amortisman_hesap_kodu );
        return ADAK_FAIL;
    }

    if ( m_ui->searchedit_dmr_satis_kar_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Sabit Kıymet Satış Karı hesap kodunu boş bırakamazsınız!.." ), m_ui->searchedit_dmr_satis_kar_hesap_kodu );
        return ADAK_FAIL;
    }

    if ( m_ui->lineedit_demirbas_kodu->text().isEmpty() EQ true ) {
         MSG_WARNING( tr ( "Demirbaş kodunu boş bırakamazsınız!.." ), m_ui->lineedit_demirbas_kodu );

        return ADAK_FAIL;
    }
    if ( m_ui->lineedit_demirbas_adi->text().isEmpty() EQ true ) {
         MSG_WARNING( tr ( "Demirbaş adını boş bırakamazsınız!.." ), m_ui->lineedit_demirbas_adi );

        return ADAK_FAIL;
    }
    if ( m_ui->commaEdit_alis_fiyati->GET_DOUBLE() EQ 0.0 ) {
         MSG_WARNING( tr ( "Alış Fiyatını girmelisiniz!.." ), m_ui->commaEdit_alis_fiyati );

        return ADAK_FAIL;
    }

    if ( m_ui->lineedit_amortisman_suresi->text().isEmpty() EQ true
            OR m_ui->lineedit_amortisman_suresi->text().toInt() EQ 0 ) {
         MSG_WARNING( tr ( "Amortisman süresini girmelisiniz!.." ),m_ui->lineedit_amortisman_suresi );

        return ADAK_FAIL;
    }

    if ( m_ui->comboBox_amortisman_yontemi->currentIndex() EQ -1 ) {
         MSG_WARNING( tr ( "Amortisman Yöntemini Boş Bırakamazsınız!.." ), m_ui->comboBox_amortisman_yontemi );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::CHECK_ADD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::CHECK_ADD ()
{
    int demirbas_id = DMR_GET_DEMIRBAS_ID ( m_ui->lineedit_demirbas_kodu->text() );

    if ( demirbas_id NE 0 ) {
        MSG_WARNING( tr ( "Aynı demirbaş kodu daha önce tanımlanmış,lütfen demirbaş kodunu değiştiriniz!.."), m_ui->lineedit_demirbas_kodu );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::ADD_RECORD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::ADD_RECORD ()
{
    SQL_QUERY insert_query ( DB );

    int personel_id = PRS_GET_CARI_PERSONEL_ID( m_cari_hesap_id );

    insert_query.PREPARE_INSERT     ( "dmr_demirbaslar","demirbas_id","demirbas_kodu , demirbas_adi,"
                                      "muh_hesap_id,alis_tarihi,alis_bedeli,"
                                      "uretici_firma,marka,model,seri_numarasi,garanti_suresi,"
                                      "amortisman_orani,amortisman_suresi,yeniden_degerleme_yapilacak_mi,"
                                      "kist_amortisman,demirbasin_degeri,yeniden_degerleme_yapildi, "
                                      "amortisman_ayrilabilir_mi,demirbasin_yeni_degeri,"
                                      "amortisman_yontemi,muh_yili_ayrilacak_amor_tutari,"
                                      "personel_id,grup_id , fatura_id , bir_amor_hesap_id,"
                                      "amor_gider_hesap_id , sube_id,demirbas_satildi_mi ,"
                                      "dmr_satis_kar_hesap_id ,amortisman_ayrilacak_mi,"
                                      "muh_devir_yili_ayrilacak_amor,yeni_brkms_amortisman_tutari,"
                                      "dmr_satis_zarar_hesap_id");

    insert_query.SET_VALUE          ( ":demirbas_kodu"                         , m_ui->lineedit_demirbas_kodu->text() );
    insert_query.SET_VALUE          ( ":demirbas_adi"                          , m_ui->lineedit_demirbas_adi->text() );
    insert_query.SET_VALUE          ( ":muh_hesap_id"                          , m_muh_hesap_id );
    insert_query.SET_VALUE          ( ":alis_tarihi"                           , m_ui->adakDate_alis_tarihi->DATE());
    insert_query.SET_VALUE          ( ":alis_bedeli"                           , m_ui->commaEdit_alis_fiyati->GET_DOUBLE());
    insert_query.SET_VALUE          ( ":uretici_firma"                         , m_ui->lineEdit_uretici_firma->text() );
    insert_query.SET_VALUE          ( ":marka"                                 , m_ui->lineEdit_marka->text() );
    insert_query.SET_VALUE          ( ":model"                                 , m_ui->lineEdit_model->text() );
    insert_query.SET_VALUE          ( ":seri_numarasi"                         , m_ui->lineEdit_seri_no->text() );
    insert_query.SET_VALUE          ( ":garanti_suresi"                        , m_ui->lineedit_garanti_suresi->text().toInt() );
    insert_query.SET_VALUE          ( ":amortisman_orani"                      , m_ui->commaEdit_amortisman_orani->GET_DOUBLE() );
    insert_query.SET_VALUE          ( ":amortisman_suresi"                     , m_ui->lineedit_amortisman_suresi->text().toInt());
    insert_query.SET_VALUE          ( ":yeniden_degerleme_yapilacak_mi"        , QVariant(m_ui->checkBox_yeniden_degerleme->isChecked()).toInt());
    insert_query.SET_VALUE          ( ":kist_amortisman"                       , QVariant(m_ui->checkBox_kist_amortisman->isChecked()).toInt());
    insert_query.SET_VALUE          ( ":demirbasin_degeri"                     , m_ui->commaEdit_alis_fiyati->GET_DOUBLE());
    insert_query.SET_VALUE          ( ":demirbasin_yeni_degeri"                , m_ui->commaEdit_alis_fiyati->GET_DOUBLE());  //Demirbas alindigi yil yeniden degerlendirilemez.O yuzden demirbasin yeniden degerleme sonrasindaki degeri alis fiyetiyla setliyoruz.
    insert_query.SET_VALUE          ( ":amortisman_yontemi"                    , DMR_GET_AMORTISMAN_SEKLI_ENUM(m_ui->comboBox_amortisman_yontemi->currentText()));
    insert_query.SET_VALUE          ( ":muh_yili_ayrilacak_amor_tutari"        , 0);
    insert_query.SET_VALUE          ( ":yeniden_degerleme_yapildi"             , 0);
    insert_query.SET_VALUE          ( ":amortisman_ayrilabilir_mi"             , 1 );
    insert_query.SET_VALUE          ( ":personel_id"                           , personel_id);
    insert_query.SET_VALUE          ( ":grup_id"                               , m_grup_id );
    insert_query.SET_VALUE          ( ":fatura_id"                             , m_fatura_id);
    insert_query.SET_VALUE          ( ":bir_amor_hesap_id"                     , m_bir_amor_hesap_id);
    insert_query.SET_VALUE          ( ":amor_gider_hesap_id"                   , m_amor_gider_hesap_id);
    insert_query.SET_VALUE          ( ":sube_id"                               , m_sube_id);
    insert_query.SET_VALUE          ( ":demirbas_satildi_mi"                   , 0);
    insert_query.SET_VALUE          ( ":dmr_satis_kar_hesap_id"                , m_dmr_satis_kar_hesap_id);
    insert_query.SET_VALUE          ( ":amortisman_ayrilacak_mi"               , QVariant(m_ui->checkBox_amortisman_ayrilacak_mi->isChecked()).toInt());
    insert_query.SET_VALUE          ( ":muh_devir_yili_ayrilacak_amor"         , 0);
    insert_query.SET_VALUE          ( ":yeni_brkms_amortisman_tutari"          , m_ui->commaEdit_birikmis_amortisman->GET_DOUBLE());
    insert_query.SET_VALUE          ( ":dmr_satis_zarar_hesap_id"              , m_dmr_satis_zarar_hesap_id);

    int demirbas_id = insert_query.INSERT();

    DMR_AMORTISMAN_TUTARINI_ARTTIR(demirbas_id,0,m_ui->commaEdit_birikmis_amortisman->GET_DOUBLE());

    GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI,DEMIRBAS_GRUP_MODULU, demirbas_id , &m_rpr_isl_grubu_idler);

    if ( m_grup_id > 0 ) {
        QList<int> grup_id_list ;
        grup_id_list << m_grup_id;

        GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI , DEMIRBAS_MODULU , demirbas_id , &grup_id_list);
    }



    QString log_detaylari = QObject::tr ( "Demirbaş Kodu / Adı : " );
    log_detaylari.append ( m_ui->lineedit_demirbas_kodu->text() + "  " + m_ui->lineedit_demirbas_adi->text());

    E9_LOG_KAYDI_EKLE ( DEMIRBAS_MODULU , LOG_DMR_DEMIRBASLAR , LOG_ISLEM_ADD , log_detaylari );

    if ( m_fatura_id > 0 ) {
        DMR_ENTEGRASYON_FISI_OLUSTUR(m_fatura_id , demirbas_id);
    }

    return demirbas_id;
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::CHECK_UPDATE
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::CHECK_UPDATE ( int record_id )
{
    int demirbas_id = DMR_GET_DEMIRBAS_ID ( m_ui->lineedit_demirbas_kodu->text() );

    if ( demirbas_id NE record_id ) {
         MSG_WARNING( tr ( "Aynı demirbaş kodu daha önce tanımlanmış.Lütfen demirbaş kodunu değiştiriniz!.."), m_ui->lineedit_demirbas_kodu );

        return ADAK_FAIL;
    }


    return ADAK_OK;
}

/**************************************************************************************
            DMR_DEMIRBAS_KARTI::UPDATE_RECORD
***************************************************************************************/

void DMR_DEMIRBAS_KARTI::UPDATE_RECORD ( int record_id )
{
    SQL_QUERY update_query( DB );

    SQL_QUERY query(DB);

    int personel_id = PRS_GET_CARI_PERSONEL_ID( m_cari_hesap_id );

    query.PREPARE_SELECT("dmr_demirbaslar" , "fatura_id,yeni_brkms_amortisman_tutari" , "demirbas_id = :demirbas_id");
    query.SET_VALUE(":demirbas_id" , record_id);

    int old_fatura_id = 0;
    double old_birikmis_amortisman_tutari = 0;

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        old_fatura_id = query.VALUE(0).toInt();
        old_birikmis_amortisman_tutari = query.VALUE(1).toDouble();
    }

    if ( old_birikmis_amortisman_tutari > 0 ) {
        DMR_AMORTISMAN_TUTARINI_AZALT(record_id , 0,old_birikmis_amortisman_tutari);
    }

    if ( old_fatura_id NE m_fatura_id ) {
        if ( old_fatura_id > 0 ) {
            DMR_ENTEGRASYON_FISI_OLUSTUR(old_fatura_id , 0);
        }
    }


    update_query.PREPARE_UPDATE    ( "dmr_demirbaslar","demirbas_id","demirbas_kodu  , demirbas_adi ,"
                                     "muh_hesap_id ,alis_tarihi,alis_bedeli ,"
                                     "uretici_firma ,marka ,model ,seri_numarasi ,"
                                     "garanti_suresi ,amortisman_orani ,"
                                     "amortisman_suresi ,yeniden_degerleme_yapilacak_mi ,"
                                     "kist_amortisman ,demirbasin_degeri,"
                                     "demirbasin_yeni_degeri ,amortisman_yontemi,grup_id,"
                                     "personel_id,fatura_id ,amor_gider_hesap_id ,"
                                     "bir_amor_hesap_id , sube_id , dmr_satis_kar_hesap_id,"
                                     "yeni_brkms_amortisman_tutari,dmr_satis_zarar_hesap_id",
                                     "demirbas_id = :demirbas_id" );

    update_query.SET_VALUE ( ":demirbas_kodu"     , m_ui->lineedit_demirbas_kodu->text() );
    update_query.SET_VALUE ( ":demirbas_adi"      , m_ui->lineedit_demirbas_adi->text() );
    update_query.SET_VALUE ( ":muh_hesap_id"      , m_muh_hesap_id);
    update_query.SET_VALUE ( ":alis_tarihi"       , m_ui->adakDate_alis_tarihi->DATE());
    update_query.SET_VALUE ( ":alis_bedeli"       , m_ui->commaEdit_alis_fiyati->GET_DOUBLE());
    update_query.SET_VALUE ( ":uretici_firma"     , m_ui->lineEdit_uretici_firma->text());
    update_query.SET_VALUE ( ":marka"             , m_ui->lineEdit_marka->text());
    update_query.SET_VALUE ( ":model"             , m_ui->lineEdit_model->text());
    update_query.SET_VALUE ( ":seri_numarasi"     , m_ui->lineEdit_seri_no->text());
    update_query.SET_VALUE ( ":garanti_suresi"    , m_ui->lineedit_garanti_suresi->text().toInt());
    update_query.SET_VALUE ( ":amortisman_orani"  , m_ui->commaEdit_amortisman_orani->GET_DOUBLE());
    update_query.SET_VALUE ( ":amortisman_suresi" , m_ui->lineedit_amortisman_suresi->text().toInt());
    update_query.SET_VALUE ( ":yeniden_degerleme_yapilacak_mi" , QVariant(m_ui->checkBox_yeniden_degerleme->isChecked()).toInt());
    update_query.SET_VALUE ( ":kist_amortisman"   , QVariant(m_ui->checkBox_kist_amortisman->isChecked()).toInt());
    update_query.SET_VALUE ( ":demirbasin_degeri" , m_ui->commaEdit_alis_fiyati->GET_DOUBLE());
    update_query.SET_VALUE ( ":demirbasin_yeni_degeri" , m_ui->commaEdit_alis_fiyati->GET_DOUBLE());
    update_query.SET_VALUE ( ":amortisman_yontemi" , DMR_GET_AMORTISMAN_SEKLI_ENUM ( m_ui->comboBox_amortisman_yontemi->currentText()));
    update_query.SET_VALUE ( ":grup_id"            , m_grup_id);
    update_query.SET_VALUE ( ":personel_id"        , personel_id);
    update_query.SET_VALUE ( ":fatura_id"          , m_fatura_id);
    update_query.SET_VALUE ( ":amor_gider_hesap_id", m_amor_gider_hesap_id);
    update_query.SET_VALUE ( ":bir_amor_hesap_id"  , m_bir_amor_hesap_id);
    update_query.SET_VALUE ( ":sube_id"            , m_sube_id);
    update_query.SET_VALUE ( ":dmr_satis_kar_hesap_id" , m_dmr_satis_kar_hesap_id);
    update_query.SET_VALUE ( ":yeni_brkms_amortisman_tutari" , m_ui->commaEdit_birikmis_amortisman->GET_DOUBLE());
    update_query.SET_VALUE ( ":dmr_satis_zarar_hesap_id" , m_dmr_satis_zarar_hesap_id);;
    update_query.SET_VALUE ( ":demirbas_id"        , record_id );

    update_query.UPDATE();

    DMR_AMORTISMAN_TUTARINI_ARTTIR(record_id , 0 , m_ui->commaEdit_birikmis_amortisman->GET_DOUBLE());

    GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI,DEMIRBAS_GRUP_MODULU, record_id , &m_rpr_isl_grubu_idler);

    if ( m_grup_id > 0 ) {
        QList<int> grup_id_list ;
        grup_id_list << m_grup_id;

        GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI , DEMIRBAS_MODULU , record_id , &grup_id_list);
    }



    QString log_detaylari = QObject::tr ( "Demirbaş Kodu / Adı : " );
    log_detaylari.append ( m_ui->lineedit_demirbas_kodu->text() + "  " + m_ui->lineedit_demirbas_adi->text());

    E9_LOG_KAYDI_EKLE ( DEMIRBAS_MODULU , LOG_DMR_DEMIRBASLAR , LOG_ISLEM_UPDATE , log_detaylari );

    if ( m_fatura_id > 0 ) {
        DMR_ENTEGRASYON_FISI_OLUSTUR(m_fatura_id , record_id);
    }



}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::CHECK_DELETE
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::CHECK_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );

    int demirbas_id = DMR_GET_DEMIRBAS_ID ( m_ui->lineedit_demirbas_kodu->text() );

    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fis_satirlari","fis_id ","demirbas_id = :demirbas_id" );
    select_query.SET_VALUE      ( ":demirbas_id" , demirbas_id );

    if ( select_query.SELECT() NE 0 ) {
         MSG_WARNING( tr ( "Demirbaş fişlerde kullanılmış,kartı silemezsiniz!.." ) , NULL );
         return ADAK_FAIL;
    }



    int msg_secim = MSG_YES_NO( tr ( "Seçili demirbaş kayıtlardan silinecektir.\n Devam etmek istiyor musunuz?" ), NULL );

    if ( msg_secim EQ ADAK_NO ) {

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
             DMR_DEMIRBAS_KARTI::DELETE_RECORD
***************************************************************************************/

void DMR_DEMIRBAS_KARTI::DELETE_RECORD ( int record_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_kodu , demirbas_adi,fatura_id",
                               "demirbas_id = :demirbas_id");
    sql_query.SET_VALUE      ( ":demirbas_id" , record_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString demirbas_kodu = sql_query.VALUE(0).toString();
    QString demirbas_adi  = sql_query.VALUE(1).toString();
    int     fatura_id     = sql_query.VALUE(2).toInt();

    if ( fatura_id > 0 ) {
        DMR_ENTEGRASYON_FISI_OLUSTUR(fatura_id , 0);
    }


    SQL_QUERY delete_query ( DB );

    delete_query.PREPARE_DELETE ( "dmr_demirbaslar","demirbas_id = :demirbas_id" );
    delete_query.SET_VALUE      ( ":demirbas_id" , record_id );
    delete_query.DELETE();

    GRP_KAYDIN_GRUPLARINI_SIL(E9_PROGRAMI,DEMIRBAS_GRUP_MODULU,record_id);

    QString log_detaylari = QObject::tr ( "Demirbaş Kodu / Adı : " );
    log_detaylari.append ( demirbas_kodu + "  " + demirbas_adi );

    E9_LOG_KAYDI_EKLE  ( DEMIRBAS_MODULU , LOG_DMR_DEMIRBASLAR , LOG_ISLEM_DELETE , log_detaylari );
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::FIND_FIRST_RECORD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::FIND_FIRST_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_id " );
    if ( select_query.SELECT("demirbas_kodu ASC",0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::FIND_LAST_RECORD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::FIND_LAST_RECORD ()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_id " );
    if ( select_query.SELECT("demirbas_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();

}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::FIND_NEXT_RECORD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::FIND_NEXT_RECORD ()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_id","demirbas_kodu > :demirbas_kodu ");

    select_query.SET_VALUE            ( ":demirbas_kodu" , m_ui->lineedit_demirbas_kodu->text() );

    if ( select_query.SELECT(" demirbas_kodu ASC",0,1) EQ 0 ) {
         return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::FIND_PREV_RECORD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::FIND_PREV_RECORD ()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_id ","demirbas_kodu < :demirbas_kodu");

    select_query.SET_VALUE            ( ":demirbas_kodu" , m_ui->lineedit_demirbas_kodu->text() );
    if ( select_query.SELECT( "demirbas_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::SELECT_RECORD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::SELECT_RECORD()
{
    return OPEN_DMR_DEMIRBAS_KARTI_ARAMA ( "", this ,TUM_DEMIRBASLAR);
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::LOCK_RECORD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::LOCK_RECORD ( int record_id )
{
    return DB->LOCK_ROW ( "demirbas_id","dmr_demirbaslar", QString ( "demirbas_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::UNLOCK_RECORD
***************************************************************************************/

void DMR_DEMIRBAS_KARTI::UNLOCK_RECORD ( int record_id )
{
    DB->UNLOCK_ROW ( "demirbas_id","dmr_demirbaslar", QString ( "demirbas_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   DMR_DEMIRBAS_KARTI::FIND_ENTERED_RECORD
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::FIND_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_id ","demirbas_kodu = :demirbas_kodu");
    select_query.SET_VALUE      ( ":demirbas_kodu" , m_ui->lineedit_demirbas_kodu->text() );

    if ( select_query.SELECT() EQ 0 ) {
         MSG_WARNING( tr ( "Aradığınız demirbaş kodu bulunamadı!.." ), m_ui->lineedit_demirbas_kodu  );

        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
       DMR_DEMIRBAS_KARTI::SEARCH_EDIT_CLICKED
***************************************************************************************/

void DMR_DEMIRBAS_KARTI::SEARCH_EDIT_CLICKED ( QWidget * widget,QLineEdit * line_edit )
{

    if ( widget EQ m_ui->searchedit_muh_hesap_kodu ) {
        m_muh_hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->searchedit_muh_hesap_kodu->GET_TEXT(),this,1 );
        if ( m_muh_hesap_id NE -1 ) {
            line_edit->setText ( MUH_GET_HESAP_KODU ( m_muh_hesap_id ) );

        }
    }
    else if ( widget EQ m_ui->searchedit_bir_amortisman_hesap_kodu ) {
        int m_bir_amor_hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->searchedit_bir_amortisman_hesap_kodu->GET_TEXT(),this,1 );
        if ( m_bir_amor_hesap_id NE -1 ) {
            line_edit->setText ( MUH_GET_HESAP_KODU ( m_bir_amor_hesap_id ) );
        }
    }
    else if ( widget EQ m_ui->searchedit_amor_gdr_hesap_kodu ) {
        m_amor_gider_hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->searchedit_amor_gdr_hesap_kodu->GET_TEXT(),this,1 );
        if ( m_amor_gider_hesap_id NE -1 ) {
            line_edit->setText ( MUH_GET_HESAP_KODU ( m_amor_gider_hesap_id ) );
        }
    }
    else if ( widget EQ m_ui->searchedit_dmr_satis_kar_hesap_kodu ) {
        m_dmr_satis_kar_hesap_id = OPEN_MUH_HESAP_ARAMA ( m_ui->searchedit_dmr_satis_kar_hesap_kodu->GET_TEXT(),this,1 );
        if ( m_dmr_satis_kar_hesap_id NE -1 ) {
            line_edit->setText ( MUH_GET_HESAP_KODU ( m_dmr_satis_kar_hesap_id ) );
        }
    }
    else if ( widget EQ m_ui->searchEdit_personel_adi ) {
        m_cari_hesap_id = OPEN_CARI_KART_ARAMA("",SAHIS_ARTI_FIRMA ,this,1,E9_PROGRAMI,PERSONEL_MODULU,QObject::tr("PERSONEL ARAMA"));

        if ( m_cari_hesap_id > 0 ) {
            line_edit->setText(CARI_FIND_HESAP_KODU(m_cari_hesap_id));
        }
    }

    else if ( widget EQ m_ui->search_edit_gruplar ) {
        int grup_id = SHOW_DMR_GRUP_TREE_SECIMI( this );

        if ( grup_id EQ m_grup_id ) {
            return;
        }

        m_grup_id = grup_id;

        QList<int> m_grup_idleri;

        m_grup_idleri << m_grup_id;

        if (m_grup_idleri.isEmpty() EQ false) {
            m_ui->search_edit_gruplar->SET_TEXT(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , new QStringList() , new QStringList() ));
        }
    }
    else if ( widget EQ m_ui->searchEdit_sube_kodu ) {
        m_sube_id = OPEN_SUBE_ARAMA(m_ui->searchEdit_sube_kodu->GET_TEXT(),this);

        if ( m_sube_id NE -1 ) {

            QString sube_kodu;

            SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu );

            m_ui->searchEdit_sube_kodu->SET_TEXT( sube_kodu );
        }
    }
}

/**************************************************************************************
                        DMR_DEMIRBAS_KARTI::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int DMR_DEMIRBAS_KARTI::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{
    if ( button EQ m_ui->pushButton_guplar ) {
        if ( OPEN_GRUP_SECIM ( E9_PROGRAMI,DEMIRBAS_GRUP_MODULU, &m_rpr_isl_grubu_idler, DB, this ) EQ true ) {
            QString grup_str = GRP_GRUP_ADLARINI_BUL ( &m_rpr_isl_grubu_idler, new QStringList() , new QStringList() );
            m_ui->textEdit_rapor_isl_gruplari->setPlainText( grup_str );
            return ADAK_RECORD_CHANGED;
        }
    }
    else if ( button EQ m_ui->pushButton_fatura_sec ) {
        int fatura_id = OPEN_FATURA_ARAMA(ENUM_ALIS_IRSALIYESI_FATURASI , ENUM_FAT_NORMAL, this);

        if ( fatura_id EQ m_fatura_id ) {
            return ADAK_RECORD_UNCHANGED;
        }

        if ( fatura_id < 0 ) {
            m_ui->lineedit_fatura_bilgileri->clear();
            m_fatura_id = fatura_id;

            return ADAK_RECORD_CHANGED;
        }
        else {
            m_fatura_id = fatura_id;
            m_ui->lineedit_fatura_bilgileri->setText(DMR_GET_FATURA_BILGILERI(m_fatura_id));

            SQL_QUERY query(DB);

            FATURA_TEMEL_BILGILER_STRUCT P_FAT;

            FAT_GET_FAT_TEMEL_BILGILER( &P_FAT, m_fatura_id );

            m_ui->commaEdit_alis_fiyati->SET_DOUBLE( P_FAT.fatura_tutari - P_FAT.isk_sonrasi_kdv_tutari );

            return ADAK_RECORD_CHANGED;
        }
    }


    return ADAK_RECORD_UNCHANGED;
}

















