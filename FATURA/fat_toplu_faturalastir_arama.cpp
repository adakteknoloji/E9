#include <QMessageBox>
#include "fat_toplu_faturalastir_arama_class.h"
#include "ui_fat_toplu_faturalastir_arama.h"
#include "sube_console_utils.h"
#include "adak_utils.h"
#include "stok_enum.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
#include "fat_console_utils.h"
#include "e9_log.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "fat_fis_utils.h"
#include "stok_fis_utils.h"
#include "cari_kart_arama_open.h"
#include "sube_arama_open.h"
#include "fat_enum.h"
#include "sube_enum.h"
#include "sube_struct.h"
#include "e9_enum.h"
#include "stok_console_utils.h"
#include "adak_gruplar.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_FAT_TOPLU_FATURALASTIR_ARAMA
***************************************************************************************/

void OPEN_FAT_TOPLU_FATURALASTIR_ARAMA ( QWidget * parent )
{
    //ADAK_WARNING( QObject::tr ( "Sadece satış irsaliyeleri toplu faturalaştırma işlemine tabi tutulabilir.") , NULL, parent );
    FAT_TOPLU_FATURALASTIR_ARAMA * F = new FAT_TOPLU_FATURALASTIR_ARAMA ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   FAT_TOPLU_FATURALASTIR_ARAMA::FAT_TOPLU_FATURALASTIR_ARAMA
***************************************************************************************/

FAT_TOPLU_FATURALASTIR_ARAMA::FAT_TOPLU_FATURALASTIR_ARAMA ( QWidget * parent ) :  ARAMA_KERNEL ( parent ), m_ui ( new Ui::FAT_TOPLU_FATURALASTIR_ARAMA )
{
    m_ui->setupUi       ( this );

    START_ARAMA_KERNEL( this, DB );
}

/**************************************************************************************
                   FAT_TOPLU_FATURALASTIR_ARAMA::SETUP_FORM
***************************************************************************************/

void FAT_TOPLU_FATURALASTIR_ARAMA::SETUP_FORM()
{
    SQL_QUERY query ( DB );

    SET_HELP_PAGE( "fatura-islemleri_satis-toplu-irsaliye-faturalastirma" );
    SET_PAGE_TITLE( tr ( "FAT - TOPLU İRSALİYE FATURALAŞTIRMA" ) );

    SET_NAME_OF_RUN_BATCH_BUTTON    ( "Faturala" );

    REGISTER_TABLE_WIDGET           ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET   ( m_ui->widget_batch_buttons );

    SET_FIRST_FOCUS_WIDGET          ( m_ui->searchEdit_cari_hesap_kodu );

    REGISTER_SAVER_BUTTON           ( m_ui->toolButton_faturalastir );

    SET_FIRST_FOCUS_WIDGET          ( m_ui->adakDate_irs_baslangic_tarihi );

    //! default value
    m_ui->toolButton_faturalastir->setEnabled(  false );

    m_ui->checkBox_irsaliye_tarih_araligi->setChecked( true );
    m_ui->frame_irsaliye_tarih_araligi->setEnabled   ( true );

    m_ui->checkBox_fatura_tarihi->setEnabled( false );
    m_ui->frame_gruplar->setEnabled( false );
    m_ui->frame_cari_hesap_kodu->setEnabled( false );

    //! end

    m_sube_id  = SUBE_GET_SUBE_ID();

    if ( SUBE_GET_SUBE_ISLEM_YETKISI() EQ SADECE_UNITE_SUBESINDE ) {
      m_ui->frame_sube->setVisible( false );
      m_ui->checkBox_sube_kodu->setVisible( false );
    }
    else {
        QString sube_kodu, sube_adi;
        SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu, &sube_adi );

        m_ui->searchEdit_sube_kodu->SET_TEXT( sube_kodu) ;
        m_ui->lineEdit_sube_ismi->setText   ( sube_adi );
    }

    m_irs_faturalandirilacak_gun_sayisi = 0;

    query.PREPARE_SELECT ( "e9_sabit_degerler","irs_faturalandirilacak_gun" ,"sabit_deger_id = :sabit_deger_id" );
    query.SET_VALUE(":sabit_deger_id" , 1);

    if ( query.SELECT() > 0 ) {
        query.NEXT();
        m_irs_faturalandirilacak_gun_sayisi = query.VALUE(0).toInt();
    }

    int bas_tarihi_gunu = m_ui->adakDate_fatura_tarihi->QDATE().day() - m_irs_faturalandirilacak_gun_sayisi;

    if ( bas_tarihi_gunu > 0 ) {
        m_ui->adakDate_irs_baslangic_tarihi->SET_DATE(  QDate(m_ui->adakDate_fatura_tarihi->QDATE().year(),
                                                         m_ui->adakDate_fatura_tarihi->QDATE().month(),
                                                         bas_tarihi_gunu)  );
    }
    else {
        bas_tarihi_gunu += m_irs_faturalandirilacak_gun_sayisi;
        m_ui->adakDate_irs_baslangic_tarihi->SET_DATE( QDate ( m_ui->adakDate_fatura_tarihi->QDATE().year(),
                                                         m_ui->adakDate_fatura_tarihi->QDATE().month(),
                                                         bas_tarihi_gunu ) );
    }

    SET_HEADERS ( QStringList() <<tr("Fiş No")<<tr("Fiş Tarihi")<<tr("Cari Hesap Adı")<<tr("Fiş Tutarı") );
    SET_SORTING ( false );

    m_ui->tableWidget->setColumnWidth ( 0, 50  );
    m_ui->tableWidget->setColumnWidth ( 1, 100 );
    m_ui->tableWidget->setColumnWidth ( 2, 200 );
    m_ui->tableWidget->setColumnWidth ( 3, 100 );

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );    
    SET_SETTING_NAME    ( "FAT_TOPLU_FATURALASTIR_ARAMA" );
}

/**************************************************************************************
                   FAT_TOPLU_FATURALASTIR_ARAMA::CHECK_VAR
***************************************************************************************/

int FAT_TOPLU_FATURALASTIR_ARAMA::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->searchEdit_cari_hesap_kodu ) {

        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_cari_hesap_ismi->clear();
            return ADAK_OK;
        }

        QString cari_hesap_ismi;
        int     cari_hesap_id;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id , &cari_hesap_ismi );

        if ( result EQ 0 ) {
            MSG_WARNING(  tr("Aradığınız cari hesap bulunamadı."), m_ui->searchEdit_cari_hesap_kodu );
            return ADAK_FAIL_UNDO;
        }

        m_ui->lineEdit_cari_hesap_ismi->setText (cari_hesap_ismi );
    }
    else if ( object EQ m_ui->adakDate_fatura_tarihi ) {

        m_ui->adakDate_irs_bitis_tarihi->SET_DATE( m_ui->adakDate_fatura_tarihi->QDATE() );
        int bas_tarihi_gunu = m_ui->adakDate_fatura_tarihi->QDATE().day() - m_irs_faturalandirilacak_gun_sayisi;

        if ( bas_tarihi_gunu > 0 ) {
            m_ui->adakDate_irs_baslangic_tarihi->SET_DATE( QDate ( m_ui->adakDate_fatura_tarihi->QDATE().year(),
                                                             m_ui->adakDate_fatura_tarihi->QDATE().month(),
                                                             bas_tarihi_gunu ) );
        }
        else {
            bas_tarihi_gunu = 1;
            m_ui->adakDate_irs_baslangic_tarihi->SET_DATE( QDate ( m_ui->adakDate_fatura_tarihi->QDATE().year(),
                                                             m_ui->adakDate_fatura_tarihi->QDATE().month(),
                                                             bas_tarihi_gunu ) );
        }
    }
    else if ( object EQ m_ui->adakDate_irs_baslangic_tarihi ) {

        if ( m_ui->adakDate_irs_baslangic_tarihi->QDATE() > m_ui->adakDate_irs_bitis_tarihi->QDATE()) {
            MSG_WARNING( tr("İrsaliye başlangıç tarihi,irsaliye bitiş tarihinden büyük olamaz"), m_ui->adakDate_irs_baslangic_tarihi);

            return ADAK_FAIL;
        }
        if ( m_ui->adakDate_irs_baslangic_tarihi->QDATE().daysTo(m_ui->adakDate_irs_bitis_tarihi->QDATE()) > m_irs_faturalandirilacak_gun_sayisi) {
            MSG_WARNING( tr("İrsaliye başlangıç tarihi aralığı ile ,bitiş tarihi aralığı "
                                                 "%1 günden fazla olamaz!.. ").arg(QVariant(m_irs_faturalandirilacak_gun_sayisi).toString()), m_ui->adakDate_irs_bitis_tarihi );

            return ADAK_FAIL;
        }

        int fatura_ayi = m_ui->adakDate_fatura_tarihi->QDATE().month();

        if ( m_ui->adakDate_irs_baslangic_tarihi->QDATE().month() NE fatura_ayi) {
            MSG_WARNING( tr("İrsaliye başlangıç tarihi ile fatura tarihi aynı aya ait olmalıdır."), m_ui->adakDate_irs_baslangic_tarihi);

            return ADAK_FAIL;
        }

        if ( m_ui->adakDate_irs_baslangic_tarihi->QDATE() < m_ui->adakDate_fatura_tarihi->QDATE()) {
            if (m_ui->adakDate_irs_baslangic_tarihi->QDATE().daysTo(m_ui->adakDate_fatura_tarihi->QDATE()) > m_irs_faturalandirilacak_gun_sayisi) {
                MSG_WARNING( tr("İrsaliye başlangıç tarihi aralığı ile ,fatura tarihi aralığı "
                                                     "%1 günden fazla olamaz!.. ").arg(QVariant(m_irs_faturalandirilacak_gun_sayisi).toString()), m_ui->adakDate_irs_bitis_tarihi );


                return ADAK_FAIL;
            }
        }
    }
    else if ( object EQ m_ui->adakDate_irs_bitis_tarihi ) {

        if ( m_ui->adakDate_irs_bitis_tarihi->QDATE() < m_ui->adakDate_irs_baslangic_tarihi->QDATE()) {
            MSG_WARNING( tr("İrsaliye bitiş tarihi,irsaliye başlangıç tarihinden büyük olamaz"), m_ui->adakDate_irs_bitis_tarihi );

            return ADAK_FAIL;
        }

        int fatura_ayi = m_ui->adakDate_fatura_tarihi->QDATE().month();

        if ( m_ui->adakDate_irs_bitis_tarihi->QDATE().month() NE fatura_ayi ) {
            MSG_WARNING( tr("İrsaliye bitiş tarihi ile fatura tarihi aynı aya ait olmalıdır."), m_ui->adakDate_irs_bitis_tarihi );

            return ADAK_FAIL;
        }

        if ( m_ui->adakDate_irs_bitis_tarihi->QDATE() > m_ui->adakDate_fatura_tarihi->QDATE() ) {
            m_ui->adakDate_irs_bitis_tarihi->SET_DATE(m_ui->adakDate_fatura_tarihi->QDATE());
            MSG_WARNING( tr("İrsaliye bitiş tarihi,fatura tarinden büyük olamaz."), m_ui->adakDate_irs_bitis_tarihi );

            return ADAK_FAIL;

        }

        if ( m_ui->adakDate_irs_bitis_tarihi->QDATE() < m_ui->adakDate_fatura_tarihi->QDATE()) {
            if(m_ui->adakDate_irs_bitis_tarihi->QDATE().daysTo(m_ui->adakDate_fatura_tarihi->QDATE()) > m_irs_faturalandirilacak_gun_sayisi) {
                MSG_WARNING( tr("İrsaliye bitiş tarihi aralığı ile ,fatura tarihi aralığı "
                                                     "%1 günden fazla olamaz!.. ").arg(QVariant(m_irs_faturalandirilacak_gun_sayisi).toString()), m_ui->adakDate_irs_bitis_tarihi );

                return ADAK_FAIL;
            }
        }
    }
    else if( object EQ m_ui->searchEdit_sube_kodu ) {

        if ( m_ui->searchEdit_sube_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_sube_id = 0;
            m_ui->lineEdit_sube_ismi->clear();
            return ADAK_OK;
        }
        QString sube_adi;
        int     sube_id;

        int result = SUBE_GET_SUBE_ID_ADI( m_ui->searchEdit_sube_kodu->GET_TEXT() , &sube_id, &sube_adi );

        if ( result EQ 0 ) {
            MSG_WARNING(  tr ( "Aradığınız şube kodu bulunamadı." ), m_ui->searchEdit_sube_kodu );
            return ADAK_FAIL_UNDO;
        }
        m_ui->lineEdit_sube_ismi->setText ( sube_adi );
        m_sube_id = sube_id;
    }
    else if ( object EQ m_ui->checkBox_cari_hesap_kodu ) {
        if ( m_ui->checkBox_cari_hesap_kodu->isChecked() EQ true ) {
            m_ui->frame_cari_hesap_kodu->setEnabled( true );
            SET_FOCUS( m_ui->searchEdit_cari_hesap_kodu );

            m_ui->checkBox_gruplar->setChecked( false );
            m_ui->frame_gruplar->setEnabled( false );
        }
        else {
            m_ui->frame_cari_hesap_kodu->setEnabled( false );
            m_ui->searchEdit_cari_hesap_kodu->CLEAR();
            m_ui->lineEdit_cari_hesap_ismi->clear();
        }
    }
    else if ( object EQ m_ui->checkBox_gruplar ) {
        if ( m_ui->checkBox_gruplar->isChecked() EQ true ) {
            m_ui->frame_gruplar->setEnabled( true );
            SET_FOCUS( m_ui->pushButton_grup_sec );

            m_ui->checkBox_cari_hesap_kodu->setChecked( false );
            m_ui->frame_cari_hesap_kodu->setEnabled( false );
        }
        else {
            m_ui->frame_gruplar->setEnabled( false );
            m_ui->textEdit_gruplar->clear();
        }
    }
    else if ( object EQ m_ui->checkBox_irsaliye_tarih_araligi ) {
        if ( m_ui->checkBox_irsaliye_tarih_araligi->isChecked() EQ true ) {
            m_ui->frame_irsaliye_tarih_araligi->setEnabled( true );
            SET_FOCUS( m_ui->adakDate_irs_baslangic_tarihi );
        }
        else {
            m_ui->frame_irsaliye_tarih_araligi->setEnabled( false );
        }
    }
    else if ( object EQ m_ui->checkBox_gruplar ) {
        if ( m_ui->checkBox_gruplar->isChecked() EQ true ) {
            m_ui->frame_gruplar->setEnabled( true );
            SET_FOCUS( m_ui->pushButton_grup_sec );
        }
        else {
            m_ui->frame_gruplar->setEnabled( false );
        }
    }
    else if ( object EQ m_ui->pushButton_grup_sec ) {
        OPEN_GRUP_SECIM( E9_PROGRAMI, CARI_MODULU, &m_grup_idleri, DB , this, true );

        QStringList grup_adlari;
        QStringList grup_kodlari;

        m_ui->textEdit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , &grup_adlari, &grup_kodlari));
    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_TOPLU_FATURALASTIR_ARAMA::CHECK_RUN
***************************************************************************************/

int FAT_TOPLU_FATURALASTIR_ARAMA::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_TOPLU_FATURALASTIR_ARAMA::SEARCH
***************************************************************************************/

void FAT_TOPLU_FATURALASTIR_ARAMA::SEARCH()
{
    QString     cari_hesap_ismi;
    int cari_hesap_id;

    QString bas_tarihi = MALI_YIL_FIRST_DATE().toString( "yyyy.MM.dd" );
    QString bts_tarihi = m_ui->adakDate_fatura_tarihi->DATE();
    m_irsaliye_fis_id_list.clear();
    QMap <int , int> IRS_FIS_VE_SATIR_ID_MAP;

    if ( m_ui->checkBox_irsaliye_tarih_araligi->isChecked() EQ true ) {
        bas_tarihi = m_ui->adakDate_irs_baslangic_tarihi->DATE();
        bts_tarihi = m_ui->adakDate_irs_bitis_tarihi->DATE();
    }
    if ( m_ui->checkBox_cari_hesap_kodu->isChecked() EQ true ) {

        CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi );

        IRS_FIS_VE_SATIR_ID_MAP = STK_GET_STK_IRS_FIS_AND_FIS_SATIRI_ID_MAP(cari_hesap_id, ENUM_IRS_NORMAL_CIKIS, -1, bas_tarihi, bts_tarihi, 0 );
        m_irsaliye_fis_id_list << IRS_FIS_VE_SATIR_ID_MAP.uniqueKeys();

    }
    else if ( m_ui->checkBox_gruplar->isChecked() EQ true ) {

        QList<int> cari_hesap_id_list = GRP_GRUBA_EKLI_KAYITLARI_BUL ( E9_PROGRAMI, CARI_MODULU, m_grup_idleri );

        for ( int i = 0; i < cari_hesap_id_list.size(); i++ ) {
            IRS_FIS_VE_SATIR_ID_MAP = STK_GET_STK_IRS_FIS_AND_FIS_SATIRI_ID_MAP( cari_hesap_id_list.at( i ), ENUM_IRS_NORMAL_CIKIS, -1, bas_tarihi, bts_tarihi, 0 );
            m_irsaliye_fis_id_list << IRS_FIS_VE_SATIR_ID_MAP.uniqueKeys();
        }
    }
    else {
        IRS_FIS_VE_SATIR_ID_MAP = STK_GET_STK_IRS_FIS_AND_FIS_SATIRI_ID_MAP( -1, ENUM_IRS_NORMAL_CIKIS, -1, bas_tarihi, bts_tarihi, 0 );
        m_irsaliye_fis_id_list = IRS_FIS_VE_SATIR_ID_MAP.uniqueKeys();
    }

    int current_row = 0;

    for ( int i = 0 ; i < m_irsaliye_fis_id_list.size() ; i++ ) {

        int irs_fis_id   = m_irsaliye_fis_id_list.at(i);

        STK_IRS_FIS_STRUCT  STK_IRS_FIS ;

        STK_IRS_FIS_BILGILERINI_OKU( irs_fis_id, &STK_IRS_FIS  );

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item ( current_row, 0  )->setText ( QVariant( STK_IRS_FIS.fis_no ).toString() );
        m_ui->tableWidget->item ( current_row, 1  )->setText ( QDate::fromString( STK_IRS_FIS.fis_tarihi, "yyyy.MM.dd").toString( " dd MMMM yyyy dddd") );
        m_ui->tableWidget->item ( current_row, 2  )->setText ( STK_IRS_FIS.cari_hesap_ismi );
        m_ui->tableWidget->item ( current_row, 3  )->setText ( VIRGUL_EKLE( QVariant ( STK_IRS_FIS.irsaliye_tutari ).toString() ) );
        m_ui->tableWidget->item ( current_row, 3  )->setTextAlignment( Qt::AlignRight );

    }
    if ( m_irsaliye_fis_id_list.size() > 0 ) {
        m_ui->toolButton_faturalastir->setEnabled( true );
    }
}

/**************************************************************************************
                   FAT_TOPLU_FATURALASTIR_ARAMA::RUN_BATCH
***************************************************************************************/

void FAT_TOPLU_FATURALASTIR_ARAMA::FATURALASTIR( int p_irsaliye_fis_id )
{
    SQL_QUERY query ( DB );

    FATURA_FISI_STRUCT          * FATURA_FISI           =   new FATURA_FISI_STRUCT;
    FATURA_FIS_SATIRI_STRUCT    * FATURA_FIS_SATIRI     =   new FATURA_FIS_SATIRI_STRUCT;

    FATURA_FISI->KDV_ORANLARI_ARRAY                     =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FATURA_FISI->KDV_HESAPLARI_ID_ARRAY                 =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FATURA_FISI->KDV_ORANINA_GORE_TUTARLAR_ARRAY        =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FATURA_FISI->OTV_ORANLARI_ARRAY                     =   new double  [KDV_OTV_ORANLARI_SAYISI];
    FATURA_FISI->OTV_HESAPLARI_ID_ARRAY                 =   new int     [KDV_OTV_ORANLARI_SAYISI];
    FATURA_FISI->OTV_ORANINA_GORE_TUTARLAR_ARRAY        =   new double  [KDV_OTV_ORANLARI_SAYISI];

    FATURA_FISI->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY      =   new int     [KDV_OTV_ORANLARI_SAYISI];

    FATURA_CLEAR_FIS_STRUCT ( FATURA_FISI );

    FATURA_KDV_OTV_DEGISKENLERINE_ILK_DEGERLERINI_YAZ  ( FATURA_FISI );

    STK_IRS_FIS_STRUCT       * STK_FIS        = new STK_IRS_FIS_STRUCT;
    STK_IRS_FIS_SATIR_STRUCT * STK_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

    STK_IRS_CLEAR_FIS_STRUCT( STK_FIS );

    STK_IRS_FIS_BILGILERINI_OKU( p_irsaliye_fis_id, STK_FIS );


    QString     cari_hesap_adresi;

    cari_hesap_adresi = CARI_GET_ADRES_BILGILERI( STK_FIS->cari_hesap_id );


    DB->START_TRANSACTION   ();

    //FATURA KAYDI EKLENECEK

    SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;

    SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

    FATURA_FISI->belge_numarasi                 = UNITE_BILG.fatura_belge_numarasi;
    FATURA_FISI->belge_seri                     = UNITE_BILG.fatura_belge_seri;

    FATURA_FISI->fis_no                         = FAT_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fatura_tarihi->DATE() );
    FATURA_FISI->aciklama                       = tr ( "Fatura, toplu irsaliye faturalaştırma işlemi sonucu üretilmiştir." );
    FATURA_FISI->fatura_tarihi                  = m_ui->adakDate_fatura_tarihi->DATE();
    FATURA_FISI->fatura_turu                    = ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI;
    FATURA_FISI->fatura_alis_satis_turu         = ENUM_SATIS_IRSALIYESI_FATURASI;
    FATURA_FISI->acik_kapali_fatura             = ENUM_ACIK_FATURA;
    FATURA_FISI->cari_hesap_id                  = STK_FIS->cari_hesap_id;
    FATURA_FISI->cari_hesap_ismi                = STK_FIS->cari_hesap_ismi;
    FATURA_FISI->cari_hesap_adresi              = cari_hesap_adresi;
    FATURA_FISI->vergi_dairesi                  = STK_FIS->vergi_dairesi;
    FATURA_FISI->vergi_numarasi                 = STK_FIS->vergi_numarasi;
    FATURA_FISI->modul_id                       = FATURA_MODULU;
    FATURA_FISI->program_id                     = E9_PROGRAMI;
    FATURA_FISI->fatura_irsaliyelestirildi_mi   = 1;

    int fatura_fis_id = FATURA_FISI_EKLE ( FATURA_FISI );

    //FATURA SATIRLARI EKLENECEK
    //faturaya referans eden irsaliyeler kaydediliyor.

    query.PREPARE_INSERT("fat_irs_baglantisi" , "kayit_id" , "fatura_id, irsaliye_id");

    query.SET_VALUE ( ":fatura_id"   , fatura_fis_id      );
    query.SET_VALUE ( ":irsaliye_id" , p_irsaliye_fis_id  );
    query.INSERT    ();

    STK_UPDATE_IRSALIYE_FATURALASTIRILDI_MI( p_irsaliye_fis_id , 1);

    int order_number = 0;

    QList<int> irs_str_id_list = STK_GET_FIS_SATIRI_ID_LIST( p_irsaliye_fis_id );

    for ( int i = 0 ; i < irs_str_id_list.size() ; i++ ) {

        STK_IRS_CLEAR_SATIR_STRUCT   ( STK_FIS_SATIRI );

        STK_IRS_SATIR_BILGILERINI_OKU( irs_str_id_list.at( i ), STK_FIS_SATIRI );

        STK_FATURALANAN_MIKTARI_GUNCELLE( irs_str_id_list.at( i ) , STK_FIS_SATIRI->satirdaki_urun_miktari );

        order_number               += 1024;

        FATURA_CLEAR_FIS_SATIRI_STRUCT ( FATURA_FIS_SATIRI );

        FATURA_FIS_SATIRI->doviz_id   = STK_FIS_SATIRI->doviz_id;
        FATURA_FIS_SATIRI->doviz_kodu = STK_FIS_SATIRI->doviz_kodu;
        FATURA_FIS_SATIRI->fis_id     = fatura_fis_id;

        if ( STK_FIS_SATIRI->satir_tutari > 0 ) {
            FATURA_FIS_SATIRI->fiyat_irsaliyeden_alindi = 1;
        }

        FATURA_FIS_SATIRI->modul_id                         = FATURA_FISI->modul_id;
        FATURA_FIS_SATIRI->program_id                       = FATURA_FISI->program_id;
        FATURA_FIS_SATIRI->order_number                     = order_number;
        FATURA_FIS_SATIRI->irsaliyelenen_miktar             = STK_FIS_SATIRI->satirdaki_urun_miktari;
        FATURA_FIS_SATIRI->isk_oncesi_urun_birim_fiyati     = STK_FIS_SATIRI->isk_oncesi_urun_birim_fiyati;
        FATURA_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati    = STK_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati;
        FATURA_FIS_SATIRI->kdv_dahil_mi                     = STK_FIS_SATIRI->kdv_dahil_mi;
        FATURA_FIS_SATIRI->kdv_orani                        = STK_FIS_SATIRI->kdv_orani;
        FATURA_FIS_SATIRI->otv_orani                        = STK_FIS_SATIRI->otv_orani;
        FATURA_FIS_SATIRI->satirdaki_urun_miktari           = STK_FIS_SATIRI->satirdaki_urun_miktari;
        FATURA_FIS_SATIRI->satir_iskonto_yuzdesi            = STK_FIS_SATIRI->iskonto_yuzdesi;
        FATURA_FIS_SATIRI->satir_iskonto_tutari             = STK_FIS_SATIRI->iskonto_tutari;
        FATURA_FIS_SATIRI->satir_kdv_dahil_tutar            = STK_FIS_SATIRI->satir_kdv_dahil_tutar;
        FATURA_FIS_SATIRI->satir_kdv_tutari                 = STK_FIS_SATIRI->satir_kdv_tutari;
        FATURA_FIS_SATIRI->satir_otv_dahil_tutar            = STK_FIS_SATIRI->satir_otv_dahil_tutar;
        FATURA_FIS_SATIRI->satir_otv_tutari                 = STK_FIS_SATIRI->satir_otv_tutari;
        FATURA_FIS_SATIRI->satir_vergi_haric_tutar          = STK_FIS_SATIRI->satir_vergi_haric_tutar;
        FATURA_FIS_SATIRI->satir_tutari                     = STK_FIS_SATIRI->satir_tutari;
        FATURA_FIS_SATIRI->temel_birim_katsayisi            = STK_FIS_SATIRI->temel_birim_katsayisi;
        FATURA_FIS_SATIRI->t_is_son_birim_fiyati            = STK_FIS_SATIRI->t_is_sonrasi_birim_fiyat;
        FATURA_FIS_SATIRI->t_is_son_satir_tutari            = STK_FIS_SATIRI->t_is_sonrasi_satir_tutari;
        FATURA_FIS_SATIRI->t_is_son_str_vergi_haric_tutar   = STK_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar;
        FATURA_FIS_SATIRI->t_is_son_satir_otv_tutari        = STK_FIS_SATIRI->t_is_sonrasi_otv_tutari;
        FATURA_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar   = STK_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar;
        FATURA_FIS_SATIRI->t_is_son_satir_kdv_tutari        = STK_FIS_SATIRI->t_is_sonrasi_kdv_tutari;
        FATURA_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar   = STK_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar;
        FATURA_FIS_SATIRI->urun_adi                         = STK_FIS_SATIRI->urun_adi;
        FATURA_FIS_SATIRI->urun_birimi                      = STK_FIS_SATIRI->urun_birimi;
        FATURA_FIS_SATIRI->urun_id                          = STK_FIS_SATIRI->urun_id;

        //irsaliye satirinda fiyat varsa ,fatura satirina aynen kaydedilir.Degistirilemez.

        int fis_satiri_id = FATURA_FIS_SATIRI_EKLE ( FATURA_FISI, FATURA_FIS_SATIRI );

        SQL_QUERY insert_query( DB );

        insert_query.PREPARE_INSERT("fat_irs_satir_baglantisi" , "kayit_id" ,
                                    "fatura_satiri_id, irsaliye_satiri_id");

        insert_query.SET_VALUE  ( ":fatura_satiri_id",      fis_satiri_id );
        insert_query.SET_VALUE  ( ":irsaliye_satiri_id",    irs_str_id_list.at( i ) );
        insert_query.INSERT     ();
    }

    int hatali_fatura_mi = 0;


    query.PREPARE_SELECT ( "fat_fatura_satirlari","satir_tutari" ,"fis_id = :fis_id" );
    query.SET_VALUE      ( ":fis_id" , fatura_fis_id );

    if ( query.SELECT() > 0 ) {
        while ( query.NEXT() EQ true )  {
            if ( query.VALUE(0).toDouble() EQ 0 ) {
                //Fatura satirlarinda tutar bilgisi yoksa fatura hatali kabul edilir.
                hatali_fatura_mi = 1;
                break;
            }
        }
    }


    FATURA_FISI->iskonto_tutari  = ROUND ( FATURA_FISI->brut_tutar - FATURA_FISI->ara_toplam );
    FATURA_FISI->iskonto_yuzdesi = ROUND ( ( ( FATURA_FISI->iskonto_tutari ) / FATURA_FISI->brut_tutar ) * 100 );


    query.PREPARE_UPDATE ( "fat_faturalar ","fis_id",
                           "iskonto_yuzdesi , "
                           "iskonto_tutari,  "
                           "hatali_fatura_mi  ",
                           "fis_id = :fis_id" );

    query.SET_VALUE ( ":iskonto_yuzdesi",               FATURA_FISI->iskonto_yuzdesi );
    query.SET_VALUE ( ":iskonto_tutari",                FATURA_FISI->iskonto_tutari  );
    query.SET_VALUE ( ":hatali_fatura_mi",              hatali_fatura_mi             );
    query.SET_VALUE ( ":fis_id",                        fatura_fis_id                       );

    query.UPDATE();

    QString cari_hesap_kodu_adi = m_ui->searchEdit_cari_hesap_kodu->GET_TEXT() + "    " + m_ui->lineEdit_cari_hesap_ismi->text();
    QString fatura_no           = "<font color = \"red\">"+ QVariant ( FATURA_FISI->fis_no ).toString() + "</font>";

    if ( hatali_fatura_mi EQ 1 ) {
        QMessageBox::warning ( this, tr("Hatalı Fatura Oluşturldu"), tr("%1 cari hesabına ait, verilen tarih aralığındaki "
                                                                        "irsaliyeler faturalaştırılırken %2 'nolu "
                                                                        "hatalı fatura oluşturuldu.").arg ( cari_hesap_kodu_adi )
                                                                                                     .arg ( fatura_no ) );
        SET_FOCUS( m_ui->searchEdit_cari_hesap_kodu );

        delete FATURA_FISI;
        delete FATURA_FIS_SATIRI;

        DB->COMMIT_TRANSACTION();
        return;
    }

    FAT_FATURA_ENT_FISLERINI_OLUSTUR( FATURA_FISI, fatura_fis_id );

    QString log_detaylari = QObject::tr("Toplu İrsaliye Faturalaştırma , Cari Hesap Kodu / Adı : ");
    log_detaylari.append ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT() + "  " + m_ui->lineEdit_cari_hesap_ismi->text()
                         + " , Fatura Tarihi : " + m_ui->adakDate_fatura_tarihi->QDATE().toString("dd MMMM yyyy")
                         +" \nİrsaliye Tarih Aralığı : " + m_ui->adakDate_irs_baslangic_tarihi->QDATE().toString("dd MMMM yyyy")
                         + " - " + m_ui->adakDate_irs_bitis_tarihi->QDATE().toString("dd MMMM yyyy") +
                         "\nŞube Kodu / Adı : " + m_ui->searchEdit_sube_kodu->GET_TEXT() + "  " + m_ui->lineEdit_sube_ismi->text());

    E9_LOG_KAYDI_EKLE ( FATURA_MODULU , LOG_FAT_TOPLU_FATURALASTIRMA , -1, log_detaylari );

    DB->COMMIT_TRANSACTION();

    delete FATURA_FISI;
    delete FATURA_FIS_SATIRI;
    delete STK_FIS;
    delete STK_FIS_SATIRI;

    accept();
}

/**************************************************************************************
                   FAT_TOPLU_FATURALASTIR_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int FAT_TOPLU_FATURALASTIR_ARAMA::SINGLE_LINE_SELECTED( int selected_row )
{
    Q_UNUSED( selected_row );
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_TOPLU_FATURALASTIR_ARAMA::SEARCH_EDIT_CLICKED
***************************************************************************************/

void FAT_TOPLU_FATURALASTIR_ARAMA::SEARCH_EDIT_CLICKED ( QWidget * widget , QLineEdit * line_edit )
{
    if ( widget EQ m_ui->searchEdit_cari_hesap_kodu ) {
        int cari_hesap_id = OPEN_CARI_KART_ARAMA ( line_edit->text(), SAHIS_ARTI_FIRMA, this, false , E9_PROGRAMI, CARI_MODULU );

        if ( cari_hesap_id NE -1 ) {
            line_edit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id ) );
        }
    }
    else if ( widget EQ m_ui->searchEdit_sube_kodu ) {
        int sube_id = OPEN_SUBE_ARAMA( line_edit->text(), this);
        if ( sube_id > 0 ) {
            QString sube_kodu;

            SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu );
            line_edit->setText( sube_kodu );
        }
    }
}

/*************************************************************************************************
                   FAT_TOPLU_FATURALASTIR_ARAMA::SAVER_BUTTON_CLICKED
*************************************************************************************************/

void FAT_TOPLU_FATURALASTIR_ARAMA::SAVER_BUTTON_CLICKED( QAbstractButton *p_button )
{
    if ( p_button EQ m_ui->toolButton_faturalastir ) {

        int secim = MSG_YES_NO( "Faturalandırma İşlemi Başlasın mı ?", NULL );
        if ( secim NE ADAK_YES ) {
            return;
        }

        for (int i = 0; i < m_irsaliye_fis_id_list.size(); ++i) {
            // Irsaliye faturastiriliyor
            FATURALASTIR( m_irsaliye_fis_id_list.at( i ) );
        }

        MSG_INFO( tr ( "Verilen tarih aralığındaki irsaliyeler toplu olarak faturalaştırıldı."), NULL );
    }
}
