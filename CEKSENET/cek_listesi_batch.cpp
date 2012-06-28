#include <QDate>
#include "cek_listesi_batch_class.h"
#include "ui_cek_listesi_batch.h"
#include "print.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "cari_console_utils.h"
#include "cek_enum.h"
#include "e9_log.h"
#include "bnk_console_utils.h"
#include "cari_kart_arama_open.h"
#include "cek_listesi_raporu_open.h"
#include "cek_struct.h"
#include "cek_console_utils.h"


extern      ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_CEK_LISTESI_BATCH
***************************************************************************************/

void OPEN_CEK_LISTESI_BATCH  ( QWidget * parent )
{
    CEK_LISTESI_BATCH * B = new CEK_LISTESI_BATCH ( parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   CEK_LISTESI_BATCH::CEK_LISTESI_BATCH
***************************************************************************************/

CEK_LISTESI_BATCH::CEK_LISTESI_BATCH ( QWidget *parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::CEK_LISTESI_BATCH)
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   CEK_LISTESI_BATCH::CHECK_VAR
***************************************************************************************/

void CEK_LISTESI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ( "cek-senet-islemleri_cek-senet-listesi" );
    SET_PAGE_TITLE      ( tr ( "ÇEK SENET LİSTESİ" ) );
    SET_SETTING_NAME    ( "CEK_LISTESI_BATCH" );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );
    SET_FIRST_FOCUS_WIDGET ( m_ui->comboBox_cek_pozisyonu );    

    m_ui->adakDate_vade_bas_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_vade_bitis_tarih->MALI_YIL_KISITI_YOK( true );

    FILL_COMBOBOX_CEK_SENET_POZISYONU();
    FILL_COMBOBOX_CEK_SENET_TURU ( ENUM_PORTFOYDE );

    m_ui->frame->setHidden( true );
    m_ui->comboBox_sirket_filtresi->setHidden(true);
    m_ui->label_entegre_filtresi->setHidden(true);

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->frame->setHidden( false );
        m_ui->comboBox_sirket_filtresi->setHidden(false);
        m_ui->label_entegre_filtresi->setHidden(false);
    }

    m_ui->adakDate_vade_bas_tarihi->SET_DATE ( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_vade_bitis_tarih->SET_DATE( MALI_ARRAY_LAST_DATE() );

    m_alinan_cari_hesap_id  = 0;
    m_verilen_cari_hesap_id = 0;

    m_ui->frame_alinan_cari_hesap->setHidden(true);
    m_ui->checkbox_alinan_cari_hesap->setHidden(true);
    m_ui->frame_banka_ismi->setHidden(true);
    m_ui->checkbox_banka->setHidden(true);
    m_ui->frame_verilen_cari_hesap->setHidden(true);
    m_ui->checkbox_verilen_cari_hesap->setHidden(true);

    QStringList kullanilan_banka_list = BNK_GET_BANKA_ISIMLERI();

    for ( int i = 0; i < kullanilan_banka_list.size(); i++ ) {
        m_ui->combobox_banka_ismi->addItem ( kullanilan_banka_list.at( i ) );
    }
    m_ui->combobox_banka_ismi->setCurrentIndex(-1);

    m_ui->frame_tarih_araligi->setEnabled ( false );
    m_ui->frame_alinan_cari_hesap->setEnabled ( false );
    m_ui->frame_banka_ismi->setEnabled ( false );
    m_ui->frame_verilen_cari_hesap->setEnabled ( false );
    m_ui->lineedit_alinan_cari_hesap_ismi->setMaxLength  ( 60 );
    m_ui->lineedit_verilen_cari_hesap_ismi->setMaxLength ( 60 );
}

/**************************************************************************************
                   CEK_LISTESI_BATCH::CHECK_VAR
***************************************************************************************/

int CEK_LISTESI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchedit_alinan_cari_hesap ) {
        int cari_hesap_id = CARI_FIND_HESAP_ID ( m_ui->searchedit_alinan_cari_hesap->GET_TEXT() );
        if  ( cari_hesap_id EQ 0 ) {
              MSG_WARNING( tr ( "Cari Hesap Kodu bulunamadı" ), m_ui->searchedit_alinan_cari_hesap );

              return ADAK_FAIL;
        }
        m_alinan_cari_hesap_id = cari_hesap_id;
        m_ui->lineedit_alinan_cari_hesap_ismi->setText ( CARI_FIND_HESAP_ISMI ( cari_hesap_id ) );

    }
    else if ( object EQ m_ui->searchedit_verilen_cari_hesap ) {
        int cari_hesap_id = CARI_FIND_HESAP_ID ( m_ui->searchedit_verilen_cari_hesap->GET_TEXT() );
        if ( cari_hesap_id EQ 0 ) {
            MSG_WARNING( tr ( "Cari Hesap Kodu bulunamadı" ), m_ui->searchedit_verilen_cari_hesap );

            return ADAK_FAIL;
        }
        m_verilen_cari_hesap_id = m_verilen_cari_hesap_id;
        m_ui->lineedit_verilen_cari_hesap_ismi->setText ( CARI_FIND_HESAP_ISMI ( cari_hesap_id ) );

    }
    else if ( object EQ m_ui->comboBox_cek_pozisyonu ) {


        int cek_pozisyonu = CEK_GET_CEK_SENET_POZISYONU_ENUM ( m_ui->comboBox_cek_pozisyonu->currentText());

        switch ( cek_pozisyonu ) {

        case ENUM_BNK_TEMINAT_VERILDI :
        case ENUM_BNK_TAHSILAT_VERILDI:

            m_ui->combobox_banka_ismi->setCurrentIndex(-1);
            m_ui->frame_banka_ismi->setEnabled ( false );
            m_ui->checkbox_banka->setChecked ( false );

            m_ui->frame_banka_ismi->setHidden ( false );
            m_ui->checkbox_banka->setHidden ( false );

            m_ui->frame_alinan_cari_hesap->setHidden ( true );
            m_ui->checkbox_alinan_cari_hesap->setHidden ( true );
            m_ui->frame_verilen_cari_hesap->setHidden ( true );
            m_ui->checkbox_verilen_cari_hesap->setHidden ( true );

            break;

        case ENUM_SATICIYA_VERILDI:

            m_ui->frame_verilen_cari_hesap->setHidden ( true );
            m_ui->frame_alinan_cari_hesap->setHidden ( true );
            m_ui->checkbox_alinan_cari_hesap->setHidden ( true );
            m_ui->checkbox_verilen_cari_hesap->setHidden ( true );

            m_ui->frame_banka_ismi->setHidden ( true );
            m_ui->checkbox_banka->setHidden ( true );
            m_ui->combobox_banka_ismi->setCurrentIndex(-1);

            break;

        case ENUM_MUS_IADE_EDILDI:

            m_ui->frame_verilen_cari_hesap->setHidden ( false );
            m_ui->checkbox_verilen_cari_hesap->setHidden ( false );

            m_ui->frame_alinan_cari_hesap->setHidden ( true );
            m_ui->checkbox_alinan_cari_hesap->setHidden ( true );
            m_ui->frame_banka_ismi->setHidden ( true );
            m_ui->checkbox_banka->setHidden ( true );
            m_ui->combobox_banka_ismi->setCurrentIndex(-1);

            break;

        case ENUM_ODENDI_HESAPTAN      :
        case ENUM_TAHSIL_EDILDI_HESABA :

            m_ui->combobox_banka_ismi->setCurrentIndex(-1);
            m_ui->frame_banka_ismi->setEnabled ( false );
            m_ui->checkbox_banka->setChecked ( false );

            m_ui->frame_banka_ismi->setHidden ( false );
            m_ui->checkbox_banka->setHidden ( false );
            m_ui->frame_alinan_cari_hesap->setHidden ( true ) ;
            m_ui->checkbox_alinan_cari_hesap->setHidden ( true ) ;
            m_ui->frame_verilen_cari_hesap->setHidden ( true );
            m_ui->checkbox_verilen_cari_hesap->setHidden ( true );
            m_ui->combobox_banka_ismi->setCurrentIndex(-1);

            break;
        case ENUM_TUM_POZISYONLAR :
        default :

                m_ui->frame_alinan_cari_hesap->setHidden ( true );
                m_ui->checkbox_alinan_cari_hesap->setHidden ( true ) ;
                m_ui->frame_banka_ismi->setHidden ( true );
                m_ui->checkbox_banka->setHidden ( true );
                m_ui->frame_verilen_cari_hesap->setHidden ( true );
                m_ui->checkbox_verilen_cari_hesap->setHidden ( true );
                m_ui->combobox_banka_ismi->setCurrentIndex(-1);
                break;
        }

        m_ui->combobox_cek_senet_turu->clear();
        FILL_COMBOBOX_CEK_SENET_TURU ( cek_pozisyonu );

        if ( m_ui->combobox_cek_senet_turu->count() > 1 ) {
            m_ui->combobox_cek_senet_turu->setCurrentIndex(-1);
        }
    }

    else if ( object EQ m_ui->combobox_cek_senet_turu ) {
        if ( CEK_GET_CEK_SENET_POZISYONU_ENUM ( m_ui->comboBox_cek_pozisyonu->currentText()) EQ ENUM_SATICIYA_VERILDI ) {
            switch(CEK_GET_CEK_SENET_TURU_ENUM ( m_ui->combobox_cek_senet_turu->currentText() ) ) {
            case ENUM_MUSTERI_CEK_SENEDI :
            case ENUM_TUM_CEK_SENETLER   :
                m_ui->frame_verilen_cari_hesap->setHidden ( false ) ;
                m_ui->frame_alinan_cari_hesap->setHidden ( false );
                m_ui->checkbox_alinan_cari_hesap->setHidden ( false );
                m_ui->checkbox_verilen_cari_hesap->setHidden ( false );
                break;
            default:
                m_ui->frame_alinan_cari_hesap->setHidden ( true );
                m_ui->checkbox_alinan_cari_hesap->setHidden ( true );
                m_ui->frame_verilen_cari_hesap->setHidden ( false );
                m_ui->checkbox_verilen_cari_hesap->setHidden ( false );
                break;


            };
        }
    }
    else if ( object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ true ) {
            m_ui->frame_tarih_araligi->setEnabled ( true );
            m_ui->adakDate_vade_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_vade_bitis_tarih->SET_DATE( MALI_ARRAY_LAST_DATE() );
        }
        else {
            m_ui->frame_tarih_araligi->setEnabled ( false );
            m_ui->adakDate_vade_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_vade_bitis_tarih->SET_DATE( MALI_ARRAY_LAST_DATE() );
        }
    }
    else if ( object EQ m_ui->checkbox_banka ) {

        if ( m_ui->checkbox_banka->isChecked() EQ true ) {
            m_ui->frame_banka_ismi->setEnabled ( true );
            m_ui->combobox_banka_ismi->setCurrentIndex(-1);

        }
        else {
            m_ui->frame_banka_ismi->setDisabled(true);
            m_ui->combobox_banka_ismi->setCurrentIndex(-1);
        }
    }
    else if ( object EQ m_ui->checkbox_alinan_cari_hesap ) {

        if ( m_ui->checkbox_alinan_cari_hesap->isChecked() EQ true ) {
            m_ui->frame_alinan_cari_hesap->setEnabled(true);
        }
        else {
            m_ui->frame_alinan_cari_hesap->setDisabled ( true );
            m_ui->searchedit_alinan_cari_hesap->SET_TEXT("");
            m_ui->lineedit_alinan_cari_hesap_ismi->clear();
        }
    }
    else if ( object EQ m_ui->checkbox_verilen_cari_hesap ) {

        if ( m_ui->checkbox_verilen_cari_hesap->isChecked() EQ true ) {
           m_ui->frame_verilen_cari_hesap->setEnabled(true);
        }
        else {
            m_ui->frame_verilen_cari_hesap->setDisabled(true);
            m_ui->searchedit_verilen_cari_hesap->SET_TEXT("");
            m_ui->lineedit_verilen_cari_hesap_ismi->clear();
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_LISTESI_BATCH::CHECK_RUN
***************************************************************************************/

int CEK_LISTESI_BATCH::CHECK_RUN()
{
    if ( m_ui->comboBox_cek_pozisyonu->currentIndex() EQ -1 ) {
        MSG_WARNING( tr ( "Çek / Senedin pozisyonunu seçmelisiniz!.." ) , m_ui->comboBox_cek_pozisyonu );

        return ADAK_FAIL;
    }

    if ( m_ui->combobox_cek_senet_turu->currentIndex() EQ -1 ) {
        MSG_WARNING( tr("Çek Senet Türünü seçmelisiniz!.."), m_ui->combobox_cek_senet_turu );

        return ADAK_FAIL;
    }

    if ( m_ui->checkbox_banka->isChecked() EQ true ) {
        if(m_ui->combobox_banka_ismi->currentIndex() EQ -1) {
            MSG_WARNING( tr("Banka İsmini Seçmelisiniz!.."), m_ui->combobox_banka_ismi );

            return ADAK_FAIL;
        }
    }

    if ( m_ui->checkbox_alinan_cari_hesap->isChecked() EQ true ) {
        if(m_ui->searchedit_alinan_cari_hesap->GET_TEXT().isEmpty() EQ true) {
            MSG_WARNING( tr("Alınan Cari Hesabı girmelisiniz!.."), m_ui->searchedit_alinan_cari_hesap );

            return ADAK_FAIL;
        }
    }

    if ( m_ui->checkbox_verilen_cari_hesap->isChecked() EQ true ) {
        if ( m_ui->searchedit_verilen_cari_hesap->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Verilen Cari Hesabı girmelisiniz!.."), m_ui->searchedit_verilen_cari_hesap );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_LISTESI_BATCH::RUN_BATCH
***************************************************************************************/

void CEK_LISTESI_BATCH::RUN_BATCH()
{
    CEK_RAPOR_VARS * CEK_RV = new CEK_RAPOR_VARS;

    CEK_RV->bas_vade_tarihi       = m_ui->adakDate_vade_bas_tarihi->QDATE();
    CEK_RV->bts_vade_tarihi       = m_ui->adakDate_vade_bitis_tarih->QDATE();
    CEK_RV->cek_senet_turu        = CEK_GET_CEK_SENET_TURU_ENUM(m_ui->combobox_cek_senet_turu->currentText());
    CEK_RV->cek_senet_pozisyonu   = CEK_GET_CEK_SENET_POZISYONU_ENUM(m_ui->comboBox_cek_pozisyonu->currentText());
    CEK_RV->alinan_cari_hesap_id  = m_alinan_cari_hesap_id;
    CEK_RV->verilen_cari_hesap_id = m_verilen_cari_hesap_id;
    CEK_RV->banka_ismi            = m_ui->combobox_banka_ismi->currentText();

    if (CEK_RV->alinan_cari_hesap_id EQ 0 AND CEK_RV->verilen_cari_hesap_id EQ 0 ) {
        CEK_RV->tum_cari_hesaplar = true;
    }
    else {
        CEK_RV->tum_cari_hesaplar = false;
    }
    if (m_ui->combobox_banka_ismi->currentIndex() EQ -1) {
        CEK_RV->tum_bankalar = true;
    }
    else {
        CEK_RV->tum_bankalar = false;
    }
    QString log_detaylari = QObject::tr("Çek Raporları , Çek Listesi\n");
    log_detaylari.append(QObject::tr("Vade Tarihi Aralığı : ") + m_ui->adakDate_vade_bas_tarihi->QDATE().toString("dd MMMM YYYY")
                         + " - " + m_ui->adakDate_vade_bitis_tarih->QDATE().toString("dd MMMM yyyy") + "\n" +
                         "Pozisyonu      : " + m_ui->comboBox_cek_pozisyonu->currentText() + " , " +
                         "Türü  : " + m_ui->combobox_cek_senet_turu->currentText() + "\n");
    if(CEK_RV->alinan_cari_hesap_id NE 0 ) {
        log_detaylari.append("Alınan Cari Hesap : " + m_ui->searchedit_alinan_cari_hesap->GET_TEXT() + " "
                             + CARI_FIND_HESAP_ISMI(CEK_RV->alinan_cari_hesap_id) + "\n");
    }
    if(CEK_RV->verilen_cari_hesap_id NE 0 ) {
        log_detaylari.append("Verilen Cari Hesap : " + m_ui->searchedit_verilen_cari_hesap->GET_TEXT() + " "
                             + CARI_FIND_HESAP_ISMI(CEK_RV->verilen_cari_hesap_id));
    }

    DB->START_TRANSACTION();

    E9_LOG_KAYDI_EKLE(CEKSENET_MODULU,LOG_CEK_RAPORLAR,LOG_ISLEM_RAPOR_ALINDI,log_detaylari);

    DB->COMMIT_TRANSACTION();

    OPEN_REPORT_SHOWER ( GET_CEK_LISTESI_RAPORU(CEK_RV),nativeParentWidget());

    delete CEK_RV;
}


/**************************************************************************************
                   CEK_LISTESI_BATCH::FILL_COMBOBOX_CEK_SENET_POZISYONU
***************************************************************************************/


void CEK_LISTESI_BATCH::FILL_COMBOBOX_CEK_SENET_POZISYONU()
{
    QStringList cek_senet_pozisyonlari;

    cek_senet_pozisyonlari << tr("Portföyde") << tr("Bankaya Teminata Verildi") << tr("Bankaya Tahsilata Verildi")
                           << tr("Tahsil Edildi-Nakit")<< tr("Tahsil Edildi-Hesaba") << tr("Satıcıya Verildi")
                           << tr("Karşılıksız") << tr ("Protestolu")
                           << tr("Müşteriye İade Edildi") << tr("Ödendi-Nakit")<< tr("Ödendi-Hesaptan") << tr("Şüpheli Alacaklar")
                           << tr("Tahsil Edilemeyen") << tr("İptal") << tr("Tümü");
    m_ui->comboBox_cek_pozisyonu->addItems(cek_senet_pozisyonlari);

}

/**************************************************************************************
                   CEK_LISTESI_BATCH::FILL_COMBOBOX_CEK_SENET_TURU
***************************************************************************************/

void CEK_LISTESI_BATCH::FILL_COMBOBOX_CEK_SENET_TURU(int cek_senet_pozisyonu)
{
    QStringList cek_senet_turleri;

    switch(cek_senet_pozisyonu) {
        case ENUM_PORTFOYDE   :
        case ENUM_SATICIYA_VERILDI :
        case ENUM_TUM_POZISYONLAR :
            cek_senet_turleri << "Müşteri Çek / Senetleri" << "Kendi Çek / Senetlerimiz" << "Tüm Çekler / Senetler";
            break;
        case ENUM_BNK_TEMINAT_VERILDI    :
        case ENUM_BNK_TAHSILAT_VERILDI   :
        case ENUM_TAHSIL_EDILDI_NKT      :
        case ENUM_TAHSIL_EDILDI_HESABA   :
        case ENUM_KARSILIKSIZ            :
        case ENUM_PROTESTOLU             :
        case ENUM_MUS_IADE_EDILDI        :
        case ENUM_SUPHELI_ALACAK         :
        case ENUM_TAHSIL_EDILEMEYEN      :
            cek_senet_turleri << "Müşteri Çek / Senetleri";
            break;
        case ENUM_ODENDI_NKT      :
        case ENUM_ODENDI_HESAPTAN :
        case ENUM_IPTAL           :
            cek_senet_turleri << "Kendi Çek / Senetlerimiz";
            break;
    };
    m_ui->combobox_cek_senet_turu->addItems(cek_senet_turleri);

}

/**************************************************************************************
                   CEK_LISTESI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void CEK_LISTESI_BATCH::SEARCH_EDIT_CLICKED(QWidget * widget,QLineEdit * lineedit)
{
    Q_UNUSED ( widget );

    int cari_hesap_id  = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA, this , 1, E9_PROGRAMI, CARI_MODULU );

    if ( cari_hesap_id NE -1 ) {
        lineedit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id ) );
    }
}


