#include "smm_makbuz_formu_class.h"
#include "ui_smm_makbuz_formu.h"
#include "adak_sql.h"
#include "smm_enum.h"
#include "e9_enum.h"
#include "smm_makbuz_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "adak_std_utils.h"
#include "sube_console_utils.h"
#include "smm_console_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
#include "muh_fis_utils.h"
#include "cari_fis_utils.h"
#include "bnk_gui_utils.h"
#include "banka_struct.h"
#include "bnk_console_utils.h"
#include "cari_kart_arama_open.h"
#include "bnk_kredi_karti_secimi_open.h"
#include "bnk_pos_secimi_open.h"
#include "smm_makbuz_arama_open.h"
#include "smm_ent_detaylari_batch_open.h"
#include "smm_makbuz_belgesi_open.h"
#include "smm_struct.h"
#include "e9_uretilen_fis_secimi_open.h"


extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_SMM_MAKBUZ_FORMU
***************************************************************************************/

void OPEN_SMM_MAKBUZ_FORMU ( int p_form_id, int form_turu, QWidget * parent )
{
    SMM_MAKBUZ_FORMU * F = new SMM_MAKBUZ_FORMU ( p_form_id, form_turu , parent );
    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::SMM_MAKBUZ_FORMU
***************************************************************************************/

SMM_MAKBUZ_FORMU:: SMM_MAKBUZ_FORMU ( int p_form_id, int p_makbuz_turu , QWidget * parent ) : FORM_KERNEL ( parent ), m_ui(new Ui:: SMM_MAKBUZ_FORMU)
{
    m_makbuz_turu = p_makbuz_turu;
    m_form_id     = p_form_id;

    m_ui->setupUi     ( this );
    START_FORM_KERNEL ( this , DB );
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::SETUP_FORM
***************************************************************************************/

void  SMM_MAKBUZ_FORMU::SETUP_FORM ()
{
    if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
        SET_PAGE_TITLE    ( tr ( "SMM - SERBEST MESLEK MAKBUZU (ALIŞ)" ) );
        SET_HELP_PAGE     ("serbest-meslek-makbuzu-alis");
    }
    else {
        SET_PAGE_TITLE    ( tr ( "SMM - SERBEST MESLEK MAKBUZU (SATIŞ)" ) );
        SET_HELP_PAGE     ("serbest-meslek-makbuzu-satis");
    }

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );
    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget );

    SET_SETTING_NAME       ( "SMM_MAKBUZ_FORMU" );
    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_smmm_no );

    M_ENT_DETAYLARI   = new SMM_ENT_DETAYLARI;
    M_MAKBUZ          = new SMM_MAKBUZ_STRUCT;

    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI( m_ui->comboBox_banka_hesap_kodu );

    SMM_CLEAR_MAKBUZ_STRUCT(M_MAKBUZ);

    m_ui->lineEdit_smmm_no->setText("*");
    m_ui->lineEdit_smmm_no->setMaxLength(10);

    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_smmm_no);

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_detaylar->setVisible( false );
    }

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ) {
        m_ui->label_fis_filtresi->setVisible( false);
        m_ui->comboBox_fis_filtresi->setVisible( false );

        m_ui->comboBox_sirket->setVisible( false );
        m_ui->label_sirket->setVisible( false );
    }

    M_KDV_ORANLARI_ARRAY              = new double [KDV_OTV_ORANLARI_SAYISI];
    M_INDIRILECEK_KDV_HESABI_ID_ARRAY = new int [KDV_OTV_ORANLARI_SAYISI];
    M_TEVKIFAT_HESAPLARI_ID_ARRAY     = new int [KDV_OTV_ORANLARI_SAYISI];

    GET_VERGI_DAIRELERI(m_ui->comboBox_vergi_dairesi);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("e9_sabit_degerler" , "ontanimli_kdv_orani,kdv_oranlari_array,ind_kdv_hesap_id_array,"
                         "tevkifat_kdv_hesap_id_array",
                         "sabit_deger_id = :sabit_deger_id");
    query.SET_VALUE(":sabit_deger_id" , 1);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        m_ontanimli_kdv_orani = query.VALUE(0).toDouble();
        UNPACK_DOUBLE_ARRAY(query.VALUE(1).toString() , M_KDV_ORANLARI_ARRAY , KDV_OTV_ORANLARI_SAYISI);
        UNPACK_INT_ARRAY(query.VALUE(2).toString()    , M_INDIRILECEK_KDV_HESABI_ID_ARRAY , KDV_OTV_ORANLARI_SAYISI );
        UNPACK_INT_ARRAY(query.VALUE(3).toString()    , M_TEVKIFAT_HESAPLARI_ID_ARRAY , KDV_OTV_ORANLARI_SAYISI);
    }


    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH(512);
    m_ui->limitedTextEdit_adres->SET_MAX_STRING_LENGTH(512);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_vergi_dairesi , 50);
    m_ui->lineEdit_vergi_numarasi->setMaxLength(15);
    m_ui->lineEdit_sm_sicil_numarasi->setMaxLength(20);
    m_ui->lineEdit_belge_seri->setMaxLength(3);
    m_ui->lineEdit_belge_numarasi->setMaxLength(15);



    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_kdv_orani);

    E9_FILL_KDV_ORANLARI_COMBOBOX(m_ui->comboBox_kdv_orani);

    m_sube_kasa_hesap_id                   = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();


    SET_COMBOBOX_LENGTH(m_ui->comboBox_hizmet_turu , 128);

    query.PREPARE_SELECT("smm_hizmetler" ,"hizmet_adi" ,"hizmet_turu = :hizmet_turu");

    if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
        query.SET_VALUE(":hizmet_turu" , SMM_ALINAN_HIZMET);
    }
    else {
        query.SET_VALUE(":hizmet_turu" , SMM_SATILAN_HIZMET);
    }
    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() ) {
            m_ui->comboBox_hizmet_turu->addItem(query.VALUE(0).toString());
        }
    }
    else {
        MSG_WARNING( tr("Hizmet Alış/Satış tanımlamalısınız.!"), NULL );
    }
    m_ui->comboBox_hizmet_turu->setCurrentIndex(-1);

    m_ui->comboBox_tah_odm_sekli->addItems(QStringList() << QObject::tr("Nakit")<< QObject::tr("Kredi Kartı")
                                   << QObject::tr("Havale")<< QObject::tr("Eft"));

    m_ui->comboBox_tah_odm_sekli->setCurrentIndex(-1);

    m_ui->comboBox_tah_odm_sekli->setEditable(false);

    m_ui->comboBox_hizmet_turu->setEditable(false);

    REGISTER_CHANGER_BUTTON ( m_ui->pushButton_detaylar );
    REGISTER_SAVER_BUTTON   ( m_ui->pushButton_yazdir   );

    AUTOSTART_FORM_KERNEL( m_form_id );
}


/**************************************************************************************
                   SMM_MAKBUZ_FORMU::NEW_RECORD
***************************************************************************************/

void  SMM_MAKBUZ_FORMU::NEW_RECORD ()
{
    m_ui->lineEdit_smmm_no->setText("*");

    m_ui->commaEdit_vergi_orani->SET_DOUBLE(20);

    m_ui->comboBox_kdv_orani->setCurrentIndex(m_ui->comboBox_kdv_orani->findText(QVariant(m_ontanimli_kdv_orani).toString()));

    M_MAKBUZ->program_id    = E9_PROGRAMI;
    M_MAKBUZ->modul_id      = SMM_MODULU;
    M_MAKBUZ->makbuz_turu   = m_makbuz_turu;

    if ( m_makbuz_turu EQ SMM_SATIS_MAKBUZU ) {
        QString sm_sicil_numarasi = E9_GET_SM_SICIL_NUMARASI();
        if ( sm_sicil_numarasi.isEmpty() EQ true ) {
            MSG_WARNING(tr("Serbest Meslek Sicil numaranizi tanımlanıyınız.."), NULL );
            return;
        }

        m_ui->lineEdit_sm_sicil_numarasi->setText( sm_sicil_numarasi );
        m_ui->lineEdit_sm_sicil_numarasi->setEnabled( false );
    }

    M_ENT_DETAYLARI->tahsil_edildi_odendi = SMM_TAHSIL_EDILDI_ODENDI_SECILI_GELSIN();
    M_ENT_DETAYLARI->fis_turu             = m_makbuz_turu;

    m_ui->label_tevkifat_tutari->setHidden(true);
    m_ui->commaEdit_tevkifat_tutari->setHidden(true);

    M_ENT_DETAYLARI->tevkifat_yuzdesi          = 50;
    M_ENT_DETAYLARI->muh_kasa_hesap_id         = m_sube_kasa_hesap_id;
    M_ENT_DETAYLARI->tevkifatli_mi             = 0;

    M_MAKBUZ->muh_kasa_hesap_id                   = M_ENT_DETAYLARI->muh_kasa_hesap_id;
    M_MAKBUZ->tevkifat_yuzdesi                    = M_ENT_DETAYLARI->tevkifat_yuzdesi;
    M_MAKBUZ->tahsil_edildi_odendi                = M_ENT_DETAYLARI->tahsil_edildi_odendi;
    M_MAKBUZ->tevkifatli_mi                       = M_ENT_DETAYLARI->tevkifatli_mi;

    m_ui->comboBox_banka_hesap_kodu->setHidden(true);
    m_ui->label_banka_hesap_kodu->setHidden(true);
    m_ui->lineEdit_bnk_sube_bilgisi->setHidden(true);
    m_ui->lineEdit_doviz_cinsi->setHidden(true);

    m_ui->searchEdit_kk_pos_no->setHidden(true);
    m_ui->label_kk_pos_no->setHidden(true);

}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void SMM_MAKBUZ_FORMU::CLEAR_FORM_MEMBERS()
{
    M_ENT_DETAYLARI->bnk_fis_no           = 0;
    M_ENT_DETAYLARI->cari_fis_no          = 0;
    M_ENT_DETAYLARI->muh_fis_no           = 0;
    M_ENT_DETAYLARI->tevkifatli_mi        = 0;
    M_ENT_DETAYLARI->tevkifat_yuzdesi     = 0;
    M_ENT_DETAYLARI->muh_kasa_hesap_id    = 0;
    M_ENT_DETAYLARI->fis_turu             = -1;
    M_ENT_DETAYLARI->tahsil_edildi_odendi = 0;

    m_hesap_no_id    = 0;
    m_kredi_karti_id = 0;
    m_pos_id         = 0;
    m_cari_hesap_id  = 0;
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::GET_RECORD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::GET_RECORD ( int record_id )
{
    m_ui->label_tevkifat_tutari->setHidden(true);
    m_ui->commaEdit_tevkifat_tutari->setHidden(true);

    SMM_MAKBUZ_BILGILERINI_OKU( M_MAKBUZ , record_id);

    m_ui->lineEdit_smmm_no->setText(QVariant(M_MAKBUZ->makbuz_no).toString());
    m_ui->adakDate_fis_tarihi->SET_DATE(M_MAKBUZ->makbuz_tarihi);
    m_ui->limitedTextEdit_aciklama->setPlainText(M_MAKBUZ->aciklama);

    m_cari_hesap_id = M_MAKBUZ->cari_hesap_id;

    m_ui->limitedTextEdit_adres->setPlainText(M_MAKBUZ->cari_hesap_adresi);

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->comboBox_vergi_dairesi , M_MAKBUZ->vergi_dairesi);

    m_ui->lineEdit_vergi_numarasi->setText(M_MAKBUZ->vergi_numarasi);

    m_ui->lineEdit_sm_sicil_numarasi->setText(M_MAKBUZ->sm_sicil_numarasi);



    if ( M_MAKBUZ->tevkifat_tutari > 0 ) {
        m_ui->commaEdit_tevkifat_tutari->setHidden(false);
        m_ui->label_tevkifat_tutari->setHidden(false);
        m_ui->commaEdit_tevkifat_tutari->SET_DOUBLE(M_MAKBUZ->tevkifat_tutari);
    }

    m_ui->comboBox_kdv_orani->setCurrentIndex(m_ui->comboBox_kdv_orani->findText(QVariant(M_MAKBUZ->kdv_orani).toString()));

    m_ui->commaEdit_vergi_orani->SET_DOUBLE(M_MAKBUZ->vergi_orani);

    QString cari_hesap_kodu, cari_hesap_ismi;
    int     cari_para_birim_id;

    int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi, &cari_para_birim_id );

    m_sm_sicil_numarasi = CARI_GET_SM_SICIL_NUMARASI( m_cari_hesap_id );

    if ( result NE 0 ) {
        m_ui->searchEdit_cari_hesap_kodu->SET_TEXT( cari_hesap_kodu );
        m_ui->lineEdit_cari_hesap_ismi->setText( cari_hesap_ismi );
        m_ui->lineEdit_cari_para_birimi->setText(DVZ_GET_DOVIZ_KODU( cari_para_birim_id ));
    }

    M_ENT_DETAYLARI->muh_kasa_hesap_id    = M_MAKBUZ->muh_kasa_hesap_id;
    M_ENT_DETAYLARI->tevkifatli_mi        = M_MAKBUZ->tevkifatli_mi;
    M_ENT_DETAYLARI->tevkifat_yuzdesi     = M_MAKBUZ->tevkifat_yuzdesi;
    M_ENT_DETAYLARI->tahsil_edildi_odendi = M_MAKBUZ->tahsil_edildi_odendi;

    m_makbuz_turu                = M_MAKBUZ->makbuz_turu;
    M_ENT_DETAYLARI->fis_turu  = m_makbuz_turu;

    m_ui->lineEdit_belge_seri->setText(M_MAKBUZ->belge_seri);
    m_ui->lineEdit_belge_numarasi->setText(M_MAKBUZ->belge_numarasi);

    m_ui->comboBox_tah_odm_sekli->setCurrentIndex(m_ui->comboBox_tah_odm_sekli->findText(E9_GET_TAH_ODM_SEKLI_STRING(M_MAKBUZ->tah_odm_sekli)));

    m_ui->label_banka_hesap_kodu->setHidden(true);
    m_ui->lineEdit_bnk_sube_bilgisi->setHidden(true);
    m_ui->comboBox_banka_hesap_kodu->setHidden(true);
    m_ui->lineEdit_doviz_cinsi->setHidden(true);
    m_ui->label_kk_pos_no->setHidden(true);
    m_ui->searchEdit_kk_pos_no->setHidden(true);

    if ( M_MAKBUZ->tah_odm_sekli EQ ENUM_KREDI_KARTI ) {
        m_ui->label_kk_pos_no->setHidden(false);
        m_ui->searchEdit_kk_pos_no->setHidden(false);

        if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
            m_ui->label_kk_pos_no->setText("Kredi Kartı Numarası");
            m_ui->searchEdit_kk_pos_no->SET_TEXT(BNK_GET_KREDI_KARTI_NUMARASI(M_MAKBUZ->kredi_karti_id));
            m_kredi_karti_id = M_MAKBUZ->kredi_karti_id;
        }
        else {
            m_ui->label_kk_pos_no->setText("Pos Numarası");
            m_ui->searchEdit_kk_pos_no->SET_TEXT(BNK_GET_POS_NUMARASI(M_MAKBUZ->pos_id));
            m_pos_id = M_MAKBUZ->pos_id;
        }

    }
    else if ( M_MAKBUZ->tah_odm_sekli EQ ENUM_HAVALE OR
             M_MAKBUZ->tah_odm_sekli EQ ENUM_EFT ) {
        m_ui->label_banka_hesap_kodu->setHidden(false);
        m_ui->lineEdit_bnk_sube_bilgisi->setHidden(false);
        m_ui->comboBox_banka_hesap_kodu->setHidden(false);
        m_ui->lineEdit_doviz_cinsi->setHidden(false);

        BNK_BANKA_BILGILERI_STRUCT * BANKA_INFO = new BNK_BANKA_BILGILERI_STRUCT;

        BNK_GET_BANKA_TEMEL_BILGILER(BANKA_INFO , M_MAKBUZ->hesap_no_id);

        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(m_ui->comboBox_banka_hesap_kodu->findText(BANKA_INFO->banka_hesap_kodu));

        QString banka_bilgisi = BANKA_INFO->banka_ismi +" / " + BANKA_INFO->sube_kodu +
                " " + BANKA_INFO->sube_ismi + " " + BANKA_INFO->hesap_numarasi;

        m_ui->lineEdit_bnk_sube_bilgisi->setText(banka_bilgisi);

        m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(BANKA_INFO->doviz_id));

        m_hesap_no_id = M_MAKBUZ->hesap_no_id;
    }

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar,smm_hizmetler","hizmet_adi" ,"smm_hizmetler.hizmet_id = :hizmet_id "
                         "AND smm_makbuzlar.hizmet_id = smm_hizmetler.hizmet_id");
    query.SET_VALUE(":hizmet_id" , M_MAKBUZ->hizmet_id);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        m_ui->comboBox_hizmet_turu->setCurrentIndex(m_ui->comboBox_hizmet_turu->findText(query.VALUE(0).toString()));
    }

    m_ui->commaEdit_kdv_tutari->SET_DOUBLE(M_MAKBUZ->kdv_tutari);
    m_ui->commaEdit_vergi_stopaj_tutari->SET_DOUBLE(M_MAKBUZ->vergi_stopaj_tutari);
    m_ui->commaEdit_hizmet_tutari->SET_DOUBLE(M_MAKBUZ->hizmet_tutari);
    m_ui->commaEdit_net_ucret->SET_DOUBLE(M_MAKBUZ->net_tutar);
    m_ui->commaEdit_brut_ucret->SET_DOUBLE(M_MAKBUZ->brut_tutar);

    return ADAK_OK;

}



/**************************************************************************************
                   SMM_MAKBUZ_FORMU::CHECK_VAR
***************************************************************************************/

int SMM_MAKBUZ_FORMU::CHECK_VAR (QObject * object)
{
    if ( object EQ m_ui->searchEdit_cari_hesap_kodu ) {

        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_cari_hesap_id = 0;
            m_ui->lineEdit_cari_hesap_ismi->clear();
            m_ui->lineEdit_cari_para_birimi->clear();
            return ADAK_FAIL;
        }

        QString cari_hesap_ismi, vergi_numarasi, vergi_dairesi;
        int     cari_hesap_id, para_birim_id;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi, &para_birim_id );

        if ( result EQ 0 ) {
            MSG_WARNING(QObject::tr("Aradığınız cari hesap kodu bulunamadı.") , m_ui->searchEdit_cari_hesap_kodu);
            return ADAK_FAIL;
        }

        CARI_GET_VERGI_NO_VERGI_DAIRESI( cari_hesap_id, &vergi_numarasi, &vergi_dairesi );

        m_ui->lineEdit_cari_hesap_ismi->setText( cari_hesap_ismi );
        m_cari_hesap_id = cari_hesap_id;
        m_ui->lineEdit_cari_para_birimi->setText( DVZ_GET_DOVIZ_KODU( para_birim_id ) );

        if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
            m_ui->lineEdit_sm_sicil_numarasi->setEnabled( true );
            m_sm_sicil_numarasi     = CARI_GET_SM_SICIL_NUMARASI( cari_hesap_id );
            m_ui->lineEdit_sm_sicil_numarasi->setText( m_sm_sicil_numarasi );
        }

        //adres bilgilerini goster

        m_ui->limitedTextEdit_adres->setPlainText( CARI_GET_ADRES_BILGILERI( cari_hesap_id ) );

    }
    else if ( object EQ m_ui->comboBox_kdv_orani OR object EQ m_ui->commaEdit_vergi_orani
             OR object EQ m_ui->commaEdit_brut_ucret OR object EQ m_ui->commaEdit_kdv_tutari
             OR object EQ m_ui->commaEdit_net_ucret ) {

        double brut_tutar           = 0;
        double net_tutar            = 0;
        double kdv_tutari           = 0;
        double gelir_vergisi_tutari = 0;

        double gelir_vergisi_orani  = m_ui->commaEdit_vergi_orani->GET_DOUBLE();
        double kdv_orani            = QVariant(m_ui->comboBox_kdv_orani->currentText()).toDouble();


        if ( object EQ m_ui->commaEdit_brut_ucret ) {
            brut_tutar           = m_ui->commaEdit_brut_ucret->GET_DOUBLE();
            gelir_vergisi_tutari = brut_tutar * (gelir_vergisi_orani/100);
            net_tutar            = brut_tutar - gelir_vergisi_tutari;
        }
        else if ( object EQ m_ui->commaEdit_net_ucret ) {
            net_tutar            = m_ui->commaEdit_net_ucret->GET_DOUBLE();
            brut_tutar           = m_ui->commaEdit_net_ucret->GET_DOUBLE() / ( ((100 - gelir_vergisi_orani) + kdv_orani)/100);
            gelir_vergisi_tutari = brut_tutar * (gelir_vergisi_orani/100);
        }
        else if ( object EQ m_ui->commaEdit_kdv_tutari ) {
            kdv_tutari           = m_ui->commaEdit_kdv_tutari->GET_DOUBLE();
            brut_tutar           = kdv_tutari * (100/kdv_orani);
            gelir_vergisi_tutari = brut_tutar * (gelir_vergisi_orani / 100);
            net_tutar            = brut_tutar - gelir_vergisi_tutari;
        }
        else if ( object EQ m_ui->comboBox_kdv_orani OR
                 object EQ m_ui->commaEdit_vergi_orani ) {
            brut_tutar = m_ui->commaEdit_brut_ucret->GET_DOUBLE();
            net_tutar  = m_ui->commaEdit_net_ucret->GET_DOUBLE();

            if ( brut_tutar > 0 ) {
                gelir_vergisi_tutari = brut_tutar * (gelir_vergisi_orani/100);
                net_tutar            = brut_tutar - gelir_vergisi_tutari;
            }
            else {
                if ( net_tutar > 0 ) {
                    brut_tutar           = m_ui->commaEdit_net_ucret->GET_DOUBLE() / ( ((100 - gelir_vergisi_orani) + kdv_orani)/100);
                    gelir_vergisi_tutari = brut_tutar * (gelir_vergisi_orani/100);
                }
            }


        }

        m_ui->commaEdit_vergi_stopaj_tutari->SET_DOUBLE(gelir_vergisi_tutari);
        m_ui->commaEdit_net_ucret->SET_DOUBLE    (net_tutar);
        m_ui->commaEdit_brut_ucret->SET_DOUBLE   (brut_tutar);

        kdv_tutari = brut_tutar * ( kdv_orani / 100);

        m_ui->commaEdit_kdv_tutari->SET_DOUBLE(kdv_tutari);

        m_ui->commaEdit_hizmet_tutari->SET_DOUBLE(kdv_tutari + net_tutar);

    }
    else if ( object EQ m_ui->comboBox_tah_odm_sekli ) {
        int tah_odm_sekli = E9_GET_TAH_ODM_SEKLI_ENUM(m_ui->comboBox_tah_odm_sekli->currentText());

        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(-1);
        m_ui->lineEdit_bnk_sube_bilgisi->clear();
        m_ui->lineEdit_doviz_cinsi->clear();

        m_ui->searchEdit_kk_pos_no->CLEAR();

        switch ( tah_odm_sekli ) {
        case ENUM_NAKIT :
            m_ui->label_banka_hesap_kodu->setHidden(true);
            m_ui->lineEdit_bnk_sube_bilgisi->setHidden(true);
            m_ui->comboBox_banka_hesap_kodu->setHidden(true);
            m_ui->lineEdit_doviz_cinsi->setHidden(true);

            m_ui->label_kk_pos_no->setHidden(true);
            m_ui->searchEdit_kk_pos_no->setHidden(true);
            break;

        case ENUM_HAVALE :
        case ENUM_EFT    :

            m_ui->label_banka_hesap_kodu->setHidden(false);
            m_ui->lineEdit_bnk_sube_bilgisi->setHidden(false);
            m_ui->comboBox_banka_hesap_kodu->setHidden(false);
            m_ui->lineEdit_doviz_cinsi->setHidden(false);

            m_ui->label_kk_pos_no->setHidden(true);
            m_ui->searchEdit_kk_pos_no->setHidden(true);

            break;

        case ENUM_KREDI_KARTI :

            m_ui->label_banka_hesap_kodu->setHidden(true);
            m_ui->lineEdit_bnk_sube_bilgisi->setHidden(true);
            m_ui->comboBox_banka_hesap_kodu->setHidden(true);
            m_ui->lineEdit_doviz_cinsi->setHidden(true);

            m_ui->label_kk_pos_no->setHidden(false);
            m_ui->searchEdit_kk_pos_no->setHidden(false);

            if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
                m_ui->label_kk_pos_no->setText("Kredi Kartı Numarası");
            }
            else {
                m_ui->label_kk_pos_no->setText("Pos Numarası");
            }
            break;
        }
    }
    else if ( object EQ m_ui->searchEdit_kk_pos_no ) {

        if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
            SQL_QUERY select_query(DB);

            select_query.PREPARE_SELECT ( "bnk_hesabin_kredi_kartlari, bnk_hesaplar ",
                                          "kredi_karti_id,kredi_karti_sahibi,banka_hesap_kodu,banka_ismi,"
                                          "sube_kodu,sube_ismi,hesap_numarasi,doviz_id,bnk_hesaplar.hesap_no_id",
                                          "kredi_karti_numarasi = :kredi_karti_numarasi AND "
                                          "bnk_hesaplar.hesap_no_id = bnk_hesabin_kredi_kartlari.hesap_no_id");
            select_query.SET_VALUE      ( ":kredi_karti_numarasi" , m_ui->searchEdit_kk_pos_no->GET_TEXT());

            if( select_query.SELECT() EQ 0 ) {
                MSG_WARNING( tr ( "Aradığınız Kredi Kartı bulunamadı."), m_ui->searchEdit_kk_pos_no );
                return ADAK_FAIL;
            }
            select_query.NEXT();
            m_kredi_karti_id = select_query.VALUE(0).toInt();
        }
        else {

            m_pos_id = BNK_GET_POS_ID(m_ui->searchEdit_kk_pos_no->GET_TEXT());

            if ( m_pos_id EQ 0 ) {
                MSG_WARNING(tr("Aradığınız pos numarası bulunamadı."), m_ui->searchEdit_kk_pos_no);
                return ADAK_FAIL;
            }

        }

    }
    else if ( object EQ m_ui->comboBox_banka_hesap_kodu ) {

        int row = m_ui->comboBox_banka_hesap_kodu->view()->currentIndex().row();

        if ( row EQ -1 ) {
            return ADAK_FAIL;
        }
        m_hesap_no_id = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu , row , 0).toInt();

        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(m_ui->comboBox_banka_hesap_kodu->findText(BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,2).toString()));

        m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,1).toInt()));

        QString banka_sube_bilgisi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,3).toString() +
                                     " / " + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,4).toString()
                                     +" " + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,5).toString()
                                     + " " + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,6).toString();
        m_ui->lineEdit_bnk_sube_bilgisi->setText(banka_sube_bilgisi);
        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(row);

    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::CHECK_EMPTY
***************************************************************************************/

int SMM_MAKBUZ_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->lineEdit_smmm_no->text().isEmpty() EQ true ) {
        MSG_WARNING(QObject::tr("Yeni Fiş Kaydı Yapabilmek için Fiş No değeri olarak (*) girmelisiniz!..") , m_ui->lineEdit_smmm_no );
        return ADAK_FAIL;
    }

    if ( m_ui->lineEdit_belge_seri->text().isEmpty() EQ true ) {
        MSG_WARNING(QObject::tr("SM Makbuzu Seri Boş geçilemez.") , m_ui->lineEdit_belge_seri );
        return ADAK_FAIL;
    }

    if ( m_ui->lineEdit_belge_numarasi->text().isEmpty() EQ true  ) {
        MSG_WARNING(QObject::tr("SM Makbuzu No Boş geçilemez.") , m_ui->lineEdit_belge_numarasi );
        return ADAK_FAIL;
    }

    if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(QObject::tr("Cari hesabı boş bırakamazsınız.") , m_ui->searchEdit_cari_hesap_kodu );
        return ADAK_FAIL;
    }

    if ( m_ui->commaEdit_brut_ucret->GET_DOUBLE() EQ 0 ) {
        MSG_WARNING(QObject::tr("Brüt tutarı girmelisiniz") , m_ui->commaEdit_brut_ucret);
        return ADAK_FAIL;
    }
    if ( m_ui->commaEdit_net_ucret->GET_DOUBLE() EQ 0 ) {
        MSG_WARNING(QObject::tr("Net tutarı girmelisiniz") , m_ui->commaEdit_net_ucret);
        return ADAK_FAIL;
    }

    if ( m_ui->comboBox_hizmet_turu->currentText().isEmpty() EQ  true ) {
        MSG_WARNING(QObject::tr("Hizmet Seçmelisiniz.!") , m_ui->comboBox_hizmet_turu );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::CHECK_ADD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::CHECK_ADD ()
{
    if ( QString ( m_ui->lineEdit_smmm_no->text().at(0)) NE "*") {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("smm_makbuzlar","makbuz_id" ,"makbuz_tarihi = :makbuz_tarihi "
                             "AND makbuz_no = :makbuz_no");
        query.SET_VALUE(":makbuz_tarihi" , m_ui->adakDate_fis_tarihi->DATE());
        query.SET_VALUE(":makbuz_no"     , m_ui->lineEdit_smmm_no->text().toInt());

        if ( query.SELECT() NE 0 ) {
            MSG_WARNING(tr("Aynı smm numarası ile kayıtlı başka bir kayıt bulunmaktadır.Kayıt yapılamadı") , m_ui->lineEdit_smmm_no);

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::ADD_RECORD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::ADD_RECORD ()
{
    int makbuz_no = 0;

    if ( QString( m_ui->lineEdit_smmm_no->text().at(0) ) EQ "*" ) {
        makbuz_no  = SMM_SIRADAKI_MAKBUZ_NO_AL ( m_ui->adakDate_fis_tarihi->DATE());
    }
    else {
        makbuz_no = m_ui->lineEdit_smmm_no->text().toInt();
    }

    M_MAKBUZ->kdv_orani = QVariant(m_ui->comboBox_kdv_orani->currentText()).toDouble();


    for ( int i = 0 ; i < KDV_OTV_ORANLARI_SAYISI ; i++ ) {

        if ( M_KDV_ORANLARI_ARRAY[i] EQ M_MAKBUZ->kdv_orani ) {
           M_MAKBUZ->kdv_hesap_id              = M_INDIRILECEK_KDV_HESABI_ID_ARRAY[i];
           M_MAKBUZ->kdv_tevkifat_muh_hesap_id = M_TEVKIFAT_HESAPLARI_ID_ARRAY[i];
           break;
        }
    }

    int hizmet_id = 0;

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_hizmetler" ,"hizmet_id" ,"hizmet_turu = :hizmet_turu AND "
                         "hizmet_adi = :hizmet_adi");
    if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
        query.SET_VALUE(":hizmet_turu" , SMM_ALINAN_HIZMET);
    }
    else {
        query.SET_VALUE(":hizmet_turu" , SMM_SATILAN_HIZMET);
    }
    query.SET_VALUE(":hizmet_adi" , m_ui->comboBox_hizmet_turu->currentText());

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        hizmet_id = query.VALUE(0).toInt();
    }

    if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
        M_MAKBUZ->kredi_karti_id = m_kredi_karti_id;
    }
    else {
        M_MAKBUZ->pos_id         = m_pos_id;
    }

    M_MAKBUZ->makbuz_no                   = makbuz_no;
    M_MAKBUZ->makbuz_tarihi               = m_ui->adakDate_fis_tarihi->DATE();
    M_MAKBUZ->aciklama                    = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_MAKBUZ->cari_hesap_id               = m_cari_hesap_id;
    M_MAKBUZ->cari_hesap_adresi           = m_ui->limitedTextEdit_adres->toPlainText();
    M_MAKBUZ->vergi_dairesi               = m_ui->comboBox_vergi_dairesi->currentText();
    M_MAKBUZ->vergi_numarasi              = m_ui->lineEdit_vergi_numarasi->text();
    M_MAKBUZ->sm_sicil_numarasi           = m_ui->lineEdit_sm_sicil_numarasi->text();
    M_MAKBUZ->modul_id                    = SMM_MODULU;
    M_MAKBUZ->program_id                  = E9_PROGRAMI;
    M_MAKBUZ->muh_kasa_hesap_id           = M_ENT_DETAYLARI->muh_kasa_hesap_id;
    M_MAKBUZ->tevkifatli_mi               = M_ENT_DETAYLARI->tevkifatli_mi;
    M_MAKBUZ->tevkifat_yuzdesi            = M_ENT_DETAYLARI->tevkifat_yuzdesi;
    M_MAKBUZ->makbuz_turu                 = m_makbuz_turu;
    M_MAKBUZ->muh_fis_id                  = 0;
    M_MAKBUZ->belge_seri                  = m_ui->lineEdit_belge_seri->text();
    M_MAKBUZ->belge_numarasi              = m_ui->lineEdit_belge_numarasi->text();
    M_MAKBUZ->tahsil_edildi_odendi        = M_ENT_DETAYLARI->tahsil_edildi_odendi;
    M_MAKBUZ->kdv_orani                   = QVariant(m_ui->comboBox_kdv_orani->currentText()).toDouble();
    M_MAKBUZ->vergi_orani                 = m_ui->commaEdit_vergi_orani->GET_DOUBLE();
    M_MAKBUZ->hizmet_tutari               = m_ui->commaEdit_hizmet_tutari->GET_DOUBLE();
    M_MAKBUZ->brut_tutar                  = m_ui->commaEdit_brut_ucret->GET_DOUBLE();
    M_MAKBUZ->vergi_orani                 = m_ui->commaEdit_vergi_orani->GET_DOUBLE();
    M_MAKBUZ->vergi_stopaj_tutari         = m_ui->commaEdit_vergi_stopaj_tutari->GET_DOUBLE();
    M_MAKBUZ->net_tutar                   = m_ui->commaEdit_net_ucret->GET_DOUBLE();
    M_MAKBUZ->hizmet_id                   = hizmet_id;
    M_MAKBUZ->tah_odm_sekli               = E9_GET_TAH_ODM_SEKLI_ENUM(m_ui->comboBox_tah_odm_sekli->currentText());
    M_MAKBUZ->hesap_no_id                 = m_hesap_no_id;
    M_MAKBUZ->kdv_tutari                  = m_ui->commaEdit_kdv_tutari->GET_DOUBLE();

    int makbuz_id = SMM_MAKBUZ_EKLE(M_MAKBUZ);

    if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
        if ( m_ui->lineEdit_sm_sicil_numarasi->text() NE m_sm_sicil_numarasi ) {
            CARI_UPDATE_SM_SICIL_NUMARASI( m_cari_hesap_id, m_ui->lineEdit_sm_sicil_numarasi->text() );
        }
    }

    return makbuz_id;
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::CHECK_UPDATE
***************************************************************************************/

int SMM_MAKBUZ_FORMU::CHECK_UPDATE ( int record_id )
{
    if ( QString ( m_ui->lineEdit_smmm_no->text().at(0)) NE "*") {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("smm_makbuzlar","makbuz_id"  ,"makbuz_tarihi = :makbuz_tarihi "
                             "AND makbuz_no = :makbuz_no");
        query.SET_VALUE(":makbuz_tarihi" , m_ui->adakDate_fis_tarihi->DATE());
        query.SET_VALUE(":makbuz_no"     , m_ui->lineEdit_smmm_no->text().toInt());

        if ( query.SELECT() NE 0 ) {
            query.NEXT();

            if ( query.VALUE(0).toInt() NE record_id ) {

                MSG_WARNING(tr("Aynı smm numarası ile kayıtlı başka bir kayıt bulunmaktadır.Kayıt güncellenemedi.") , m_ui->lineEdit_smmm_no);
                SET_FOCUS( m_ui->lineEdit_smmm_no );
                return ADAK_FAIL;
            }
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::UPDATE_RECORD
***************************************************************************************/

void SMM_MAKBUZ_FORMU::UPDATE_RECORD ( int record_id )
{
    SMM_MAKBUZ_BILGILERINI_OKU ( M_MAKBUZ , record_id);

    if ( M_MAKBUZ->kdv_orani NE 0 ) {

        for ( int i = 0 ; i < KDV_OTV_ORANLARI_SAYISI ; i++ ) {

            if ( M_KDV_ORANLARI_ARRAY[i] EQ M_MAKBUZ->kdv_orani ) {
               M_MAKBUZ->kdv_hesap_id          = M_INDIRILECEK_KDV_HESABI_ID_ARRAY[i];
               M_MAKBUZ->kdv_tevkifat_muh_hesap_id = M_TEVKIFAT_HESAPLARI_ID_ARRAY[i];
               break;
            }
        }
    }
    else {
        M_MAKBUZ->kdv_hesap_id = 0;
    }

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_hizmetler" ,"hizmet_id" ,"hizmet_turu = :hizmet_turu AND "
                         "hizmet_adi = :hizmet_adi");
    if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
        query.SET_VALUE(":hizmet_turu" , SMM_ALINAN_HIZMET);
    }
    else {
        query.SET_VALUE(":hizmet_turu" , SMM_SATILAN_HIZMET);
    }
    query.SET_VALUE(":hizmet_adi" , m_ui->comboBox_hizmet_turu->currentText());

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        M_MAKBUZ->hizmet_id = query.VALUE(0).toInt();
    }

    if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
        M_MAKBUZ->kredi_karti_id = m_kredi_karti_id;
    }
    else {
        M_MAKBUZ->pos_id         = m_pos_id;
    }

    M_MAKBUZ->makbuz_no                   = m_ui->lineEdit_smmm_no->text().toInt();
    M_MAKBUZ->makbuz_tarihi               = m_ui->adakDate_fis_tarihi->DATE();
    M_MAKBUZ->aciklama                    = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_MAKBUZ->cari_hesap_id               = m_cari_hesap_id;
    M_MAKBUZ->cari_hesap_adresi           = m_ui->limitedTextEdit_adres->toPlainText();
    M_MAKBUZ->vergi_dairesi               = m_ui->comboBox_vergi_dairesi->currentText();
    M_MAKBUZ->vergi_numarasi              = m_ui->lineEdit_vergi_numarasi->text();
    M_MAKBUZ->sm_sicil_numarasi           = m_ui->lineEdit_sm_sicil_numarasi->text();
    M_MAKBUZ->muh_kasa_hesap_id           = M_ENT_DETAYLARI->muh_kasa_hesap_id;
    M_MAKBUZ->tevkifatli_mi               = M_ENT_DETAYLARI->tevkifatli_mi;
    M_MAKBUZ->tevkifat_yuzdesi            = M_ENT_DETAYLARI->tevkifat_yuzdesi;
    M_MAKBUZ->makbuz_turu                 = m_makbuz_turu;
    M_MAKBUZ->belge_numarasi              = m_ui->lineEdit_belge_numarasi->text();
    M_MAKBUZ->belge_seri                  = m_ui->lineEdit_belge_seri->text();
    M_MAKBUZ->tahsil_edildi_odendi        = M_ENT_DETAYLARI->tahsil_edildi_odendi;
    M_MAKBUZ->kdv_orani                   = QVariant(m_ui->comboBox_kdv_orani->currentText()).toDouble();
    M_MAKBUZ->vergi_orani                 = m_ui->commaEdit_vergi_orani->GET_DOUBLE();
    M_MAKBUZ->hizmet_tutari               = m_ui->commaEdit_hizmet_tutari->GET_DOUBLE();
    M_MAKBUZ->brut_tutar                  = m_ui->commaEdit_brut_ucret->GET_DOUBLE();
    M_MAKBUZ->vergi_stopaj_tutari         = m_ui->commaEdit_vergi_stopaj_tutari->GET_DOUBLE();
    M_MAKBUZ->net_tutar                   = m_ui->commaEdit_net_ucret->GET_DOUBLE();
    M_MAKBUZ->tah_odm_sekli               = E9_GET_TAH_ODM_SEKLI_ENUM(m_ui->comboBox_tah_odm_sekli->currentText());
    M_MAKBUZ->hesap_no_id                 = m_hesap_no_id;
    M_MAKBUZ->kdv_tutari                  = m_ui->commaEdit_kdv_tutari->GET_DOUBLE();

    SMM_MAKBUZ_GUNCELLE(M_MAKBUZ , record_id);

    if ( M_MAKBUZ->tevkifat_tutari > 0 ) {
        m_ui->label_tevkifat_tutari->setHidden(false);
        m_ui->commaEdit_tevkifat_tutari->setHidden(false);

        m_ui->commaEdit_tevkifat_tutari->SET_DOUBLE(M_MAKBUZ->tevkifat_tutari);
    }
    else {
        m_ui->label_tevkifat_tutari->setHidden(true);
        m_ui->commaEdit_tevkifat_tutari->setHidden(true);

    }

    if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
        if ( m_ui->lineEdit_sm_sicil_numarasi->text() NE m_sm_sicil_numarasi ) {
            CARI_UPDATE_SM_SICIL_NUMARASI( m_cari_hesap_id, m_ui->lineEdit_sm_sicil_numarasi->text() );
        }
    }
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::CHECK_DELETE
***************************************************************************************/

int SMM_MAKBUZ_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED(record_id);

    return ADAK_OK;
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::DELETE_RECORD
***************************************************************************************/

void SMM_MAKBUZ_FORMU::DELETE_RECORD ( int record_id )
{
    SMM_MAKBUZ_SIL(M_MAKBUZ , record_id);
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar" , "makbuz_id" , "makbuz_turu = :makbuz_turu");
    query.SET_VALUE(":makbuz_turu" , m_makbuz_turu);

    if ( query.SELECT("makbuz_tarihi ASC,makbuz_no ASC" , 0 ,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar" , "makbuz_id" , "makbuz_turu = :makbuz_turu");
    query.SET_VALUE(":makbuz_turu" , m_makbuz_turu);

    if ( query.SELECT("makbuz_tarihi DESC,makbuz_no DESC" , 0 ,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar" , "makbuz_id" , "makbuz_tarihi = :makbuz_tarihi "
                         "AND makbuz_no < :makbuz_no AND makbuz_turu = :makbuz_turu");
    query.SET_VALUE(":makbuz_tarihi" , m_ui->adakDate_fis_tarihi->DATE());
    query.SET_VALUE(":makbuz_no"     , m_ui->lineEdit_smmm_no->text().toInt());
    query.SET_VALUE(":makbuz_turu"   , m_makbuz_turu);

    if ( query.SELECT("makbuz_no DESC" , 0 ,1) NE 0 ) {
        query.NEXT();
        return query.VALUE(0).toInt();
    }

    query.PREPARE_SELECT("smm_makbuzlar" , "makbuz_id" , "makbuz_tarihi < :makbuz_tarihi "
                         "AND makbuz_turu = :makbuz_turu");
    query.SET_VALUE(":makbuz_tarihi" , m_ui->adakDate_fis_tarihi->DATE());
    query.SET_VALUE(":makbuz_turu"   , m_makbuz_turu);

    if ( query.SELECT("makbuz_tarihi DESC,makbuz_no DESC" , 0 ,1) EQ 0 ) {
       return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::FIND_PREV_RECORD ()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar" , "makbuz_id" , "makbuz_tarihi = :makbuz_tarihi "
                         "AND makbuz_no > :makbuz_no AND makbuz_turu = :makbuz_turu");
    query.SET_VALUE(":makbuz_tarihi" , m_ui->adakDate_fis_tarihi->DATE());
    query.SET_VALUE(":makbuz_no"     , m_ui->lineEdit_smmm_no->text().toInt());
    query.SET_VALUE(":makbuz_turu"   , m_makbuz_turu);

    if ( query.SELECT("makbuz_no ASC" , 0 ,1) NE 0 ) {
        query.NEXT();
        return query.VALUE(0).toInt();
    }

    query.PREPARE_SELECT("smm_makbuzlar" , "makbuz_id" , "makbuz_tarihi > :makbuz_tarihi "
                         "AND makbuz_turu = :makbuz_turu");
    query.SET_VALUE(":makbuz_tarihi" , m_ui->adakDate_fis_tarihi->DATE());
    query.SET_VALUE(":makbuz_turu"   , m_makbuz_turu);

    if ( query.SELECT("makbuz_tarihi ASC,makbuz_no ASC" , 0 ,1) EQ 0 ) {
       return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::SELECT_RECORD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::SELECT_RECORD()
{

    return OPEN_SMM_MAKBUZ_ARAMA(m_makbuz_turu , this);
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::LOCK_RECORD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::LOCK_RECORD( int record_id )
{
    return DB->LOCK_ROW ( "makbuz_id", "smm_makbuzlar", QString ( "makbuz_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::UNLOCK_RECORD
***************************************************************************************/

void SMM_MAKBUZ_FORMU::UNLOCK_RECORD( int record_id )
{
    DB->UNLOCK_ROW ( "makbuz_id", "smm_makbuzlar", QString ( "makbuz_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   SMM_MAKBUZ_FORMU::FIND_ENTERED_RECORD
***************************************************************************************/

int SMM_MAKBUZ_FORMU::FIND_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar","makbuz_id" ,"makbuz_no = :makbuz_no AND makbuz_tarihi = :makbuz_tarihi ");
    query.SET_VALUE(":makbuz_no"        , m_ui->lineEdit_smmm_no->text().toInt());
    query.SET_VALUE(":makbuz_tarihi"    , m_ui->adakDate_fis_tarihi->DATE());

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(tr("Aradığınız serbest meslek makbuzu bulunamadı.") ,m_ui->lineEdit_smmm_no);
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}
/**************************************************************************************
                SMM_MAKBUZ_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void SMM_MAKBUZ_FORMU::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    if ( widget EQ m_ui->searchEdit_cari_hesap_kodu ) {

        int cari_hesap_id = OPEN_CARI_KART_ARAMA( line_edit->text(), SAHIS_ARTI_FIRMA,this,true , E9_PROGRAMI, CARI_MODULU );

        if ( cari_hesap_id NE -1 ) {
            line_edit->setText ( CARI_FIND_HESAP_KODU(cari_hesap_id) );
        }
        m_cari_hesap_id = cari_hesap_id;
    }
    else if ( widget EQ m_ui->searchEdit_kk_pos_no ) {
        if ( m_makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
            m_kredi_karti_id = OPEN_BNK_KREDI_KARTI_SEC(-1,this);

            if ( m_kredi_karti_id NE -1 ) {
                line_edit->setText(BNK_GET_KREDI_KARTI_NUMARASI(m_kredi_karti_id));
            }
        }
        else {
            int m_pos_id = OPEN_BNK_POS_SEC ( -1,this );//hesap no id degerine -1 veriyoruz.Pos Tablosunda kayitli olan tum poslari listeliyor.
            if ( m_pos_id NE -1 ) {
                line_edit->setText(BNK_GET_POS_NUMARASI(m_pos_id));
            }
        }

    }
}

/**************************************************************************************
                SMM_MAKBUZ_FORMU::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int SMM_MAKBUZ_FORMU::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{

    int record_id = GET_RECORD_ID();

    if ( button EQ m_ui->pushButton_detaylar ) {


        if ( record_id > 0 ) {
            int muh_fis_id = MUH_GET_ENT_FIS_ID(E9_PROGRAMI , SMM_MODULU,
                                                record_id);
            M_ENT_DETAYLARI->muh_fis_no = E9_GET_FIS_NO(MUHASEBE_MODULU , muh_fis_id);

            int cari_fis_id = CARI_GET_ENT_FIS_ID(E9_PROGRAMI , SMM_MODULU,
                                                  record_id);

            M_ENT_DETAYLARI->cari_fis_no = E9_GET_FIS_NO(CARI_MODULU , cari_fis_id);
        }

        OPEN_SMM_ENT_DETAYLARI_BATCH(M_ENT_DETAYLARI , this);


        if ( M_ENT_DETAYLARI->muh_kasa_hesap_id NE M_MAKBUZ->muh_kasa_hesap_id ) {
            return ADAK_RECORD_CHANGED;
        }

        if ( M_ENT_DETAYLARI->tevkifatli_mi NE M_MAKBUZ->tevkifatli_mi ) {
            return ADAK_RECORD_CHANGED;
        }
        if ( M_ENT_DETAYLARI->tevkifat_yuzdesi NE M_MAKBUZ->tevkifat_yuzdesi ) {
            return ADAK_RECORD_CHANGED;
        }
        if ( M_ENT_DETAYLARI->tahsil_edildi_odendi NE M_MAKBUZ->tahsil_edildi_odendi ) {
            return ADAK_RECORD_CHANGED;
        }

    }

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                SMM_MAKBUZ_FORMU::SAVER_BUTTON_CLICKED
***************************************************************************************/

void SMM_MAKBUZ_FORMU::SAVER_BUTTON_CLICKED(QAbstractButton *button, int record_id)
{
    if ( button EQ m_ui->pushButton_yazdir ) {
        PRINT_SMM_MAKBUZ_BELGESI(record_id);
    }
    else if ( button EQ m_ui->pushButton_urettigi_fisler ) {
        E9_URETILEN_FIS_SEC( E9_PROGRAMI, SMM_MODULU, record_id, this );
    }
}







